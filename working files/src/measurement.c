#include "header.h"

#ifdef _TEST_DURATION
uint64_t durMeas_L1Cur = 0;
uint64_t durMeas_L2Cur = 0;
#endif

float frequency_tmp = NAN;
float frequency_irq = -1.0f;
float frequency_high = -1.0f;
unsigned int freq_mutex = false;

/*****************************************************/
//Пошук нового каналу для оцифровки
/*****************************************************/
inline void find_new_ADC_canal_to_read(unsigned int command_word_adc_diff, unsigned int *point_active_index_command_word_adc)
{
  unsigned int command_word_adc_diff_tmp = command_word_adc_diff;
  unsigned int command_word_adc_diff_fapch = command_word_adc_diff_tmp & maska_canaliv_fapch;

  if (
    ((status_adc_read_work & DATA_VAL_READ) != 0) &&
    (command_word_adc_diff_fapch != 0))
  {
    command_word_adc_diff_tmp = command_word_adc_diff_fapch;
  }
  else
  {
    command_word_adc_diff_tmp &= (unsigned int) (~maska_canaliv_fapch);
  }

  while ((command_word_adc_diff_tmp & (1 << (*point_active_index_command_word_adc))) == 0)
  {
    /*
    Ще не знайдено каналу, який треба оцифровувати
    */
    *point_active_index_command_word_adc = (*point_active_index_command_word_adc + 1) % NUMBER_INPUTs_ADCs;
  }
}
/*****************************************************/

/*****************************************************/
//Управління читанням даних з АЦП
/*****************************************************/
void control_reading_ADCs(void)
{
  //Обновляємо робоче командне слово і вибираємо які канали треба оцифровувати
  if (adc_DATA_VAL_read != 0)
  {
    adc_DATA_VAL_read = false;
    status_adc_read_work |= DATA_VAL_READ;

    /*
    Канали по яких буде розраховуватися частота мають оцифровуватися як 
    омога ближче до спрацювання таймеру подачі команди оцифровки
    */
    command_word_adc &= (unsigned int) (~maska_canaliv_fapch);
    command_word_adc_work &= (unsigned int) (~maska_canaliv_fapch);

    command_word_adc |= READ_DATA_VAL;
  }

  if (adc_TEST_VAL_read != 0)
  {
    adc_TEST_VAL_read = false;
    status_adc_read_work |= TEST_VAL_READ;

    command_word_adc |= (READ_TEST_VAL | READ_ADC2_VAL);
  }

  unsigned int command_word_adc_diff = command_word_adc ^ command_word_adc_work;
  if (command_word_adc_diff != 0)
  {
    /*
    Є канали, які чекають на оцифровку
    */
    find_new_ADC_canal_to_read(command_word_adc_diff, &active_index_command_word_adc);

    /*
    Зараз active_index_command_word_adc вказує у масиві input_adc на канал, 
    який треба оцифрувати
    */

    /*
    Визначаємо, який зараз активний АЦП
    */
    unsigned int active_adc_old = ((GPIO_SELECT_ADC->ODR & GPIO_SELECTPin_ADC) == 0) ? 1 : 2;

    /*
    Визначаємо, який зараз треба активовувати АЦП і які дані треба передати
    */
    unsigned int active_adc_new = input_adc[active_index_command_word_adc][0];
    unsigned int command_word = input_adc[active_index_command_word_adc][1];

    //Визначаємо наступний стан
    if (
      (state_reading_ADCs == STATE_READING_ADCs_NONE) ||
      (state_reading_ADCs == STATE_READING_READ))
    {
      state_reading_ADCs = STATE_READING_WRITE;
      /*
      Вибирваємо відповідний АЦП
      */
      switch (active_adc_new)
      {
        case 1:
          {
            //АЦП1
            GPIO_SELECT_ADC->BSRRH = GPIO_SELECTPin_ADC;
            break;
          }
        case 2:
          {
            //АЦП2
            GPIO_SELECT_ADC->BSRRL = GPIO_SELECTPin_ADC;
            break;
          }
        default:
          {
            //Якщо сюди дійшла програма, значить відбулася недопустива помилка, тому треба зациклити програму, щоб вона пішла на перезагрузку
            total_error_sw_fixed();
          }
      }
    }
    else
    {
      if (active_adc_old == active_adc_new)
      {
        state_reading_ADCs = STATE_READING_WRITE_READ;
      }
      else
      {
        state_reading_ADCs = STATE_READING_READ;
        command_word = 0;
      }
    }

    if (command_word != 0)
    {
      /*
      Помічаємо, що зараз будемо нове командне слово передавати
      */
      command_word_adc_work |= (1 << active_index_command_word_adc);
    }

    //Передаємо командне слово
    while ((SPI_ADC->SR & SPI_I2S_FLAG_TXE) == RESET)
      ;                                    //Очікуємо, поки SPI стане вільним
    tick_output_adc_p = TIM5->CNT;         //Фіксуємо час початку оцифровки
    GPIO_SPI_ADC->BSRRH = GPIO_NSSPin_ADC; //Виставляємо chip_select
    SPI_ADC->DR = (uint16_t) command_word; //Відправляємо командне число

    channel_answer = channel_request;
    if (active_adc_new == 1)
    {
      channel_request = (command_word >> 10) & 0xf;
    }
    else
    {
      channel_request = NUMBER_CANALs_ADC1 + ((command_word >> 10) & 0x1);
    }
  }
  else
  {
    if (
      (state_reading_ADCs == STATE_READING_READ) ||
      (state_reading_ADCs == STATE_READING_ADCs_NONE))
    {
      //Усі канали вже оцифровані
      state_reading_ADCs = STATE_READING_ADCs_NONE;

      command_word_adc = 0;
      command_word_adc_work = 0;
      active_index_command_word_adc = 0;
    }
    else
    {
      /*
      Треба ще отримати значення з останнього каналуна, який зараз тільки
      щоно оцифровувався
      */
      state_reading_ADCs = STATE_READING_READ;
      while ((SPI_ADC->SR & SPI_I2S_FLAG_TXE) == RESET)
        ;                                    //Очікуємо, поки SPI стане вільним
      tick_output_adc_p = TIM5->CNT;         //Фіксуємо час початку оцифровки
      GPIO_SPI_ADC->BSRRH = GPIO_NSSPin_ADC; //Виставляємо chip_select
      SPI_ADC->DR = 0;                       //Відправляємо число (але так, щоб нове контрольне слово не записувалося)

      channel_answer = channel_request;
    }
  }
}
/*****************************************************/

/*************************************************************************
Опрацьовуємо інтеграільні величини
 *************************************************************************/
void operate_integral_values_ADCs(void)
{
  /*******************************************************
  //Всі масиви одної величини ми вже опрацювали  
  *******************************************************/

  /*******************************************************/
  //Вираховування середнього значення контрольних точок
  /*******************************************************/
  //VREF1 (Опорна напруга на вході АЦП)
  int temp = output_adc[C_VREF1].value;
  vref_averange_sum += temp;
  vref_averange_sum -= vref_moment_value[index_array_of_one_value];
  vref_moment_value[index_array_of_one_value] = temp;
  if ((temp < 0x614) || (temp > 0x9EB))
    _SET_BIT(set_diagnostyka, ERROR_VREF_ADC_TEST_COARSE_BIT);
  else
    _SET_BIT(clear_diagnostyka, ERROR_VREF_ADC_TEST_COARSE_BIT);

  //V_K_3_3 (Контроль напруги живлення АЦП)
  temp = output_adc[C_V_K_3_3].value;
  v_k_3_3_averange_sum += temp;
  v_k_3_3_averange_sum -= v_k_3_3_moment_value[index_array_of_one_value];
  v_k_3_3_moment_value[index_array_of_one_value] = temp;
  if ((temp < 0x6F2) || (temp > 0xD48))
    _SET_BIT(set_diagnostyka, ERROR_V_K_3_3_ADC_TEST_COARSE_BIT);
  else
    _SET_BIT(clear_diagnostyka, ERROR_V_K_3_3_ADC_TEST_COARSE_BIT);

  //adc2_channel0 (Напруга на логометрі)
  adc2_channel0 = output_adc[C_ADC2_Ch0].value;
  adc2_channel0_averange_sum += adc2_channel0;
  if (completion_of_first_period == 1)
  {
    adc2_channel0_averange_sum -= adc2_channel0_moment_value[index_array_of_one_value];
  }
  adc2_channel0_moment_value[index_array_of_one_value] = adc2_channel0;

  //adc2_channel1 (Напруга з выходу логометра)
  adc2_channel1 = output_adc[C_ADC2_Ch1].value;
  adc2_channel1_averange_sum += adc2_channel1;
  if (completion_of_first_period == 1)
  {
    adc2_channel1_averange_sum -= adc2_channel1_moment_value[index_array_of_one_value];
  }
  adc2_channel1_moment_value[index_array_of_one_value] = adc2_channel1;

  if ((++index_array_of_one_value) == NUMBER_POINT)
  {
    index_array_of_one_value = 0;
    if (completion_of_first_period == 0)
    {
      completion_of_first_period = 1;
    }
    //Вираховуємо значення контрольних точок АЦП
    vref_averange = vref_averange_sum >> VAGA_NUMBER_POINT;
    v_k_3_3_averange = v_k_3_3_averange_sum >> VAGA_NUMBER_POINT;
    adc2_channel0_averange_irq = adc2_channel0_averange_sum >> VAGA_NUMBER_POINT;
    adc2_channel1_averange_irq = adc2_channel1_averange_sum >> VAGA_NUMBER_POINT;
  }
  else if ((index_array_of_one_value > 1) &&
           (index_array_of_one_value < NUMBER_POINT) &&
           (completion_of_first_period == 0))
  {
    if (index_array_of_one_value == 2)
    {
      adc2_channel0_averange_irq = adc2_channel0_averange_sum >> 1;
      adc2_channel1_averange_irq = adc2_channel1_averange_sum >> 1;
    }
    else if (index_array_of_one_value == 4)
    {
      adc2_channel0_averange_irq = adc2_channel0_averange_sum >> 2;
      adc2_channel1_averange_irq = adc2_channel1_averange_sum >> 2;
    }
    else if (index_array_of_one_value == 8)
    {
      adc2_channel0_averange_irq = adc2_channel0_averange_sum >> 3;
      adc2_channel1_averange_irq = adc2_channel1_averange_sum >> 3;
    }
    else if (index_array_of_one_value == 16)
    {
      adc2_channel0_averange_irq = adc2_channel0_averange_sum >> 4;
      adc2_channel1_averange_irq = adc2_channel1_averange_sum >> 4;
    }
    else
    {
      adc2_channel0_averange_irq = adc2_channel0_averange_sum / index_array_of_one_value;
      adc2_channel1_averange_irq = adc2_channel1_averange_sum / index_array_of_one_value;
    }
  }
  else if ((index_array_of_one_value == 1) && (completion_of_first_period == 0))
  {
    adc2_channel0_averange_irq = adc2_channel0_averange_sum;
    adc2_channel1_averange_irq = adc2_channel1_averange_sum;
  }
  else if ((index_array_of_one_value < NUMBER_POINT) && (completion_of_first_period == 1))
  {
    adc2_channel0_averange_irq = adc2_channel0_averange_sum >> VAGA_NUMBER_POINT;
    adc2_channel1_averange_irq = adc2_channel1_averange_sum >> VAGA_NUMBER_POINT;
  }
  else if (index_array_of_one_value > NUMBER_POINT)
  {
    //Сюди, по ідеї програма ніколи не малаб заходити, тому зациклюємо тут програму. щоб вона пішла на перезагрузку
    total_error_sw_fixed();
  }
  /*******************************************************/

  if ((vref_averange < 0x709) || (vref_averange > 0x8f5))
    _SET_BIT(set_diagnostyka, ERROR_VREF_ADC_TEST_BIT);
  else
    _SET_BIT(clear_diagnostyka, ERROR_VREF_ADC_TEST_BIT);

  if ((v_k_3_3_averange < 0x8F9) || (v_k_3_3_averange > 0xC24))
    _SET_BIT(set_diagnostyka, ERROR_V_K_3_3_ADC_TEST_BIT);
  else
    _SET_BIT(clear_diagnostyka, ERROR_V_K_3_3_ADC_TEST_BIT);

  //Копіювання для інших систем
  unsigned int bank_ortogonal_tmp = bank_ortogonal;
  adc2_channel0_averange[bank_ortogonal_tmp] = adc2_channel0_averange_irq;
  adc2_channel1_averange[bank_ortogonal_tmp] = adc2_channel1_averange_irq;
}
/*************************************************************************/

/*************************************************************************
Опрацьовуємо дані для перетворення Фур'є
 *************************************************************************/
void Fourier(void)
{
  unsigned int index_data_sin_cos_array_tmp = index_data_sin_cos_array;
  unsigned int index_sin_cos_array_tmp = index_sin_cos_array;

  for (unsigned int i = 0; i < NUMBER_ANALOG_CANALES; i++)
  {
    //Зчитуємо миттєве значення яке треба опрацювати
    int temp_value_1 = ADCs_data[i];
    int temp_value_2;
    unsigned int i_ort_tmp = 2 * i;

    //Ортогональні SIN
    ortogonal_irq[i_ort_tmp] -= data_sin[index_data_sin_cos_array_tmp];
    temp_value_2 = (int) ((float) temp_value_1 * sin_data_f[index_sin_cos_array_tmp]);
    data_sin[index_data_sin_cos_array_tmp] = temp_value_2;
    ortogonal_irq[i_ort_tmp] += temp_value_2;

    //Ортогональні COS
    ortogonal_irq[i_ort_tmp + 1] -= data_cos[index_data_sin_cos_array_tmp];
    temp_value_2 = (int) ((float) temp_value_1 * cos_data_f[index_sin_cos_array_tmp]);
    data_cos[index_data_sin_cos_array_tmp] = temp_value_2;
    ortogonal_irq[i_ort_tmp + 1] += temp_value_2;

    if ((++index_data_sin_cos_array_tmp) >= (NUMBER_POINT * NUMBER_ANALOG_CANALES))
      index_data_sin_cos_array_tmp = 0;
  }
  index_data_sin_cos_array = index_data_sin_cos_array_tmp;

  if ((++index_sin_cos_array_tmp) >= NUMBER_POINT)
    index_sin_cos_array_tmp = 0;
  index_sin_cos_array = index_sin_cos_array_tmp;

  //Копіювання для інших систем
  unsigned int bank_ortogonal_tmp = bank_ortogonal;
  for (unsigned int i = 0; i < (2 * NUMBER_ANALOG_CANALES); i++)
    ortogonal[i][bank_ortogonal_tmp] = ortogonal_irq[i];
}
/*************************************************************************/

/*************************************************************************
Детектор частоти для каналів групи 1
*************************************************************************/
//#pragma optimize=none
void fapch(void)
{
  unsigned int bank_measurement_high_tmp = bank_measurement_high;
  int index_1 = -1;
  unsigned int maska_canaliv_fapch_tmp = 0;

  /*****
  Шучаємо сигнал по якому будемо розраховувати частоту
  *****/
  if (measurement_high[bank_measurement_high_tmp][IM_UAB_TN1] >= PORIG_FOR_FAPCH)
  {
    index_1 = INDEX_PhK_UAB_TN1;
    maska_canaliv_fapch_tmp = READ_UAB_TN1;
  }
  else if (measurement_high[bank_measurement_high_tmp][IM_UAB_TN2] >= PORIG_FOR_FAPCH)
  {
    index_1 = INDEX_PhK_UAB_TN2;
    maska_canaliv_fapch_tmp = READ_UAB_TN2;
  }
  maska_canaliv_fapch = maska_canaliv_fapch_tmp;
  /*****/

  /*****/
  //Частота
  /*****/
  static uint32_t ind_freq;
  static size_t count;
  uint32_t step_timer_adc_tmp = step_timer_adc;
  if (
    (index_1 >= 0) &&
    (fix_perechid_cherez_nul[index_1] != 0))
  {
    count = 0;

    fix_perechid_cherez_nul[index_1] = 0;

    unsigned int delta_tick;
    long long tick_tmp;
    int delta_value;
    unsigned int tick_p, x1_tmp, x2_tmp;

    /*Знаходимо час переходу через 0 попереднього разу з врахуванням лінійної апроксимації*/
    delta_value = perechid_cherez_nul[index_1][0].y2 - perechid_cherez_nul[index_1][0].y1;
    x1_tmp = perechid_cherez_nul[index_1][0].x1;
    x2_tmp = perechid_cherez_nul[index_1][0].x2;
    if (x2_tmp > x1_tmp)
      delta_tick = x2_tmp - x1_tmp;
    else
    {
      long long delta_tick_64 = x2_tmp + 0x100000000 - x1_tmp;
      delta_tick = delta_tick_64;
    }
    tick_tmp = ((long long) perechid_cherez_nul[index_1][0].x1) - ((long long) perechid_cherez_nul[index_1][0].y1) * ((long long) delta_tick) / ((long long) delta_value);
    if (tick_tmp < 0)
    {
      tick_tmp += 0x100000000;
      tick_p = (unsigned int) tick_tmp;
    }
    else
    {
      if (tick_tmp < 0x100000000)
        tick_p = (unsigned int) tick_tmp;
      else
      {
        tick_tmp -= 0x100000000;
        tick_p = (unsigned int) tick_tmp;
      }
    }

    /*Знаходимо час переходу через 0 поточного разу з врахуванням лінійної апроксимації*/
    delta_value = perechid_cherez_nul[index_1][1].y2 - perechid_cherez_nul[index_1][1].y1;
    x1_tmp = perechid_cherez_nul[index_1][1].x1;
    x2_tmp = perechid_cherez_nul[index_1][1].x2;
    if (x2_tmp > x1_tmp)
      delta_tick = x2_tmp - x1_tmp;
    else
    {
      long long delta_tick_64 = x2_tmp + 0x100000000 - x1_tmp;
      delta_tick = delta_tick_64;
    }
    tick_tmp = ((long long) perechid_cherez_nul[index_1][1].x1) - ((long long) perechid_cherez_nul[index_1][1].y1) * ((long long) delta_tick) / ((long long) delta_value);
    if (tick_tmp < 0)
    {
      tick_tmp += 0x100000000;
      tick_c = (unsigned int) tick_tmp;
    }
    else
    {
      if (tick_tmp < 0x100000000)
        tick_c = (unsigned int) tick_tmp;
      else
      {
        tick_tmp -= 0x100000000;
        tick_c = (unsigned int) tick_tmp;
      }
    }
    /***/

    if (tick_c > tick_p)
      delta_tick = tick_c - tick_p;
    else
    {
      long long delta_tick_64 = tick_c + 0x100000000 - tick_p;
      delta_tick = delta_tick_64;
    }
    tick_period = delta_tick;

    /*****
    Розрахунок частоти
    *****/
    if (
      (tick_period <= MAX_TICK_PERIOD) &&
      (tick_period >= MIN_TICK_PERIOD))
    {
      frequency_tmp = (float) MEASUREMENT_TIM_FREQUENCY / (float) tick_period;

      sum_freq_arr -= freq_arr[index_freq_arr];
      freq_arr[index_freq_arr] = frequency_tmp;
      sum_freq_arr += frequency_tmp;

      index_freq_arr = (index_freq_arr + 1) % N_F_AVER;

      if (freq_arr[index_freq_arr] > 0)
      {
        //Це означає, що весь масив для усереднення зкаповнений значеннями (немає випадку від'ємних чисел. щог означає, що частота тільки з'явилася)
        if (frequency_irq < 0)
          ++ind_freq;
        frequency_irq = sum_freq_arr / (float) N_F_AVER;
        unsigned int tick_period_tmp = (unsigned int) roundf((float) MEASUREMENT_TIM_FREQUENCY / frequency_irq);

        step_timer_adc_tmp = tick_period_tmp >> VAGA_NUMBER_POINT;
        if ((tick_period_tmp - (step_timer_adc_tmp << VAGA_NUMBER_POINT)) >= (1 << (VAGA_NUMBER_POINT - 1)))
          step_timer_adc_tmp++;
      }
    }
    else
    {
      step_timer_adc_tmp = TIM5_CCR1_2_VAL;
      if (tick_period > MAX_TICK_PERIOD)
        frequency_tmp = -2.0f; /*Частота нижче порогу визначеного константою MIN_FREQUENCY*/
      else
        frequency_tmp = -3.0f; /*Частота вище порогу визначеного константою MAX_FREQUENCY*/

      if (frequency_irq != frequency_tmp)
        ++ind_freq;
      frequency_irq = frequency_tmp;
    }
    /****/
  }
  else
  {
    if (index_1 < 0)
    {
      if (count == 0)
      {
        step_timer_adc_tmp = TIM5_CCR1_2_VAL;
        tick_c = TIM5->CNT;
        frequency_tmp = -1.0f; /*Частота не визначена*/

        if (frequency_irq != frequency_tmp)
          ++ind_freq;
        frequency_irq = frequency_tmp;
      }
      if (++count >= NUMBER_POINT)
        count = 0;
    }
    else
      count = 0;
  }
  if (ind_freq == 0)
    ind_freq = 1; /*нулем ідентифікатор бути не може, бо нуль означає, що частота не визначена*/
                  /*****/

  if (!isnan(frequency_tmp))
  {
    //Нову вираховану частоту фіксуємо
    if (frequency_tmp < 0)
    {
      index_freq_arr = 0;
      for (size_t i = 0; i < N_F_AVER; ++i)
        freq_arr[i] = 0.0f;
      sum_freq_arr = 0;
    }

    //		__f_ext const f_ext = {ind_freq*(frequency_irq >= 0), tick_c, frequency_irq};
    //		f_ext_arr[index_f_ext] = f_ext;
    //		if (++index_f_ext >= SIZE_F_EXT_ARR) index_f_ext = 0;

    frequency_tmp = NAN;
  }

  /*****/
  //ФАПЧ
  /*****/
  if (step_timer_adc != step_timer_adc_tmp)
  {
    //Треба змінити частоту дискретизації
    step_timer_adc = step_timer_adc_tmp;
  }

  if ((command_restart_monitoring_frequency & (1 << 0)) != 0)
  {
    frequency_min = 50;
    frequency_max = 50;

    command_restart_monitoring_frequency &= (unsigned int) (~(1 << 0));
  }
  else
  {
    if (frequency_irq >= 0)
    {
      if (frequency_irq > frequency_max)
        frequency_max = frequency_irq;
      if (frequency_irq < frequency_min)
        frequency_min = frequency_irq;
    }
  }
  /*****/
}
/*****************************************************/

/*****************************************************/
//Переривання від прийнятого байту по канаду SPI, який обслуговує вимірювальну систему
/*****************************************************/
void SPI_ADC_IRQHandler(void)
{
  //#ifdef SYSTEM_VIEWER_ENABLE
  //  SEGGER_SYSVIEW_RecordEnterISR();
  //#endif

  //Фіксуємо вхід у переривання обробки даних від АЦП
  semaphore_adc_irq = true;

  //На самому початку знімаємо chip_select
  GPIO_SPI_ADC->BSRRL = GPIO_NSSPin_ADC;
  uint16_t read_value = SPI_ADC->DR;

  /*
  Аналізуємо прийняті дані
  */
  if (
    (state_reading_ADCs == STATE_READING_WRITE_READ) ||
    (state_reading_ADCs == STATE_READING_READ))
  {
    int const adc_number = ((GPIO_SELECT_ADC->ODR & GPIO_SELECTPin_ADC) != 0); // 0 = 0!=0; 1 = 1!=0
    unsigned int const number_canal = (adc_number == 0) ? ((read_value >> 12) & 0xf) : (NUMBER_CANALs_ADC1 + ((read_value >> 13) & 0x1));

    output_adc[number_canal].tick = tick_output_adc_p;

    static uint32_t error_spi_adc[2] = {0, 0};
    if (channel_answer != number_canal)
    {
      if (error_spi_adc[adc_number] < 3)
        error_spi_adc[adc_number]++;
      if (error_spi_adc[adc_number] >= 3)
        _SET_BIT(set_diagnostyka, ((adc_number == 0) ? ERROR_SPI_ADC1_BIT : ERROR_SPI_ADC2_BIT));
    }
    else
    {
      error_spi_adc[adc_number] = 0;

      _SET_BIT(clear_diagnostyka, ((adc_number == 0) ? ERROR_SPI_ADC1_BIT : ERROR_SPI_ADC2_BIT));
      if (adc_number == 0)
      {
        output_adc[number_canal].value = read_value & 0xfff;
      }
      else
      {
        output_adc[number_canal].value =
          ((int32_t)((uint32_t) read_value << (32 - 13))) >> (32 - 13);
      }
    }
  }
  /***/

  //Виконуємо операції з читання АЦП
  control_reading_ADCs();

  /*
  Подальші діх виконуємо тільки тоді, коли всі канали вже оцифровані
  */
  if (state_reading_ADCs == STATE_READING_ADCs_NONE)
  {
#ifdef SYSTEM_VIEWER_ENABLE
    SEGGER_SYSVIEW_RecordEnterISR();
#endif

#ifdef _TEST_DURATION
    uint32_t const start_tick = TIM2->CNT;
#endif

    /*******************************************************/
    //Перевіряємо, чи відбувалися зміни юстування
    /*******************************************************/
    if (changed_ustuvannja == CHANGED_ETAP_ENDED) /*Це є умова, що нові дані підготовлені для передачі їх у роботу вимірювальною системою (і при цьому зараз дані не змінюються)*/
    {
      //Копіюємо масив юстування у копію цього масиву але з яким працює (читає і змінює) тільки вимірювальна захистема
      for (unsigned int k = 0; k < NUMBER_ANALOG_CANALES; k++)
      {
        ustuvannja_meas[k] = ustuvannja[k];
      }

      //Помічаємо, що зміни прийняті вимірювальною системою, але ще треба прийняти системою захистів
      changed_ustuvannja = CHANGED_ETAP_NONE;
    }
    /*****************************************************/

    /*******************************************************/
    //Перевіряємо, чи відбувалися зміни налаштувань
    /*******************************************************/
    if (changed_settings == CHANGED_ETAP_ENDED_EXTRA_ETAP) /*Це є умова, що нові дані підготовлені для передачі їх у роботу системою захистів(і при цьому зараз дані не змінюються)*/
    {
      //Копіюємо налаштування, які потрібні вимірювальній системі
      type_of_input_prt = current_settings_prt.type_of_input;
      type_of_input_signal_prt = current_settings_prt.type_of_input_signal;
      for (size_t i = 0; i < NUMBER_INPUTS; ++i)
        dopusk_dv_prt[i] = current_settings_prt.dopusk_dv[i];

      //Помічаємо, що зміни прийняті системою захистів
      changed_settings = CHANGED_ETAP_NONE;
    }
    /*****************************************************/

    /*
    Формуємо значення оцифровуваних каналів
    */
    unsigned int command_word = 0;
    if ((status_adc_read_work & DATA_VAL_READ) != 0)
    {
      command_word |= (1 << I_IA_1) | (1 << I_IA_2) |
                      (1 << I_UAB_TN1) | (1 << I_UAB_TN2) |
                      (1 << I_UC1C2) | (1 << I_UP1P2) | (1 << I_UP2P3);
    }

    uint32_t _x1, _x2, _DX, _dx;
    int _y1, _y2;
    long long _y;

    uint32_t _x = previous_tick_DATA_VAL;
    /*****/
    //Формуємо значення IA_1
    /*****/
    if ((command_word & (1 << I_IA_1)) != 0)
    {
      _x1 = ADCs_data_raw[I_IA_1].tick;
      _y1 = ADCs_data_raw[I_IA_1].value;

      _y2 = output_adc[C_IA_1].value - /*gnd_adc - */ vref_averange;
      if (abs(_y2) > 87)
      {
        _x2 = output_adc[C_IA_1].tick;
        _y2 = (int) (_y2 * ustuvannja_meas[I_IA_1]) >> (USTUVANNJA_VAGA - 4);
      }
      else
      {
        _y2 = output_adc[C_IA_1_16].value - /*gnd_adc - */ vref_averange;

        _x2 = output_adc[C_IA_1_16].tick;
        _y2 = (int) ((-_y2) * ustuvannja_meas[I_IA_1]) >> (USTUVANNJA_VAGA);
      }

      if (_x2 > _x1)
        _DX = _x2 - _x1;
      else
      {
        uint64_t _DX_64 = _x2 + 0x100000000 - _x1;
        _DX = _DX_64;
      }
      if (_x >= _x1)
        _dx = _x - _x1;
      else
      {
        uint64_t _dx_64 = _x + 0x100000000 - _x1;
        _dx = _dx_64;
      }
      _y = ((long long) _y1) + ((long long) (_y2 - _y1)) * ((long long) _dx) / ((long long) _DX);

      ADCs_data[I_IA_1] = _y;

      ADCs_data_raw[I_IA_1].tick = _x2;
      ADCs_data_raw[I_IA_1].value = _y2;
    }
    /*****/

    /*****/
    //Формуємо значення IA_2
    /*****/
    if ((command_word & (1 << I_IA_2)) != 0)
    {
      _x1 = ADCs_data_raw[I_IA_2].tick;
      _y1 = ADCs_data_raw[I_IA_2].value;

      _y2 = output_adc[C_IA_2].value - /*gnd_adc - */ vref_averange;
      if (abs(_y2) > 87)
      {
        _x2 = output_adc[C_IA_2].tick;
        _y2 = (int) (_y2 * ustuvannja_meas[I_IA_2]) >> (USTUVANNJA_VAGA - 4);
      }
      else
      {
        _y2 = output_adc[C_IA_2_16].value - /*gnd_adc - */ vref_averange;

        _x2 = output_adc[C_IA_2_16].tick;
        _y2 = (int) ((-_y2) * ustuvannja_meas[I_IA_2]) >> (USTUVANNJA_VAGA);
      }

      if (_x2 > _x1)
        _DX = _x2 - _x1;
      else
      {
        uint64_t _DX_64 = _x2 + 0x100000000 - _x1;
        _DX = _DX_64;
      }
      if (_x >= _x1)
        _dx = _x - _x1;
      else
      {
        uint64_t _dx_64 = _x + 0x100000000 - _x1;
        _dx = _dx_64;
      }
      _y = ((long long) _y1) + ((long long) (_y2 - _y1)) * ((long long) _dx) / ((long long) _DX);

      ADCs_data[I_IA_2] = _y;

      ADCs_data_raw[I_IA_2].tick = _x2;
      ADCs_data_raw[I_IA_2].value = _y2;
    }
    /*****/

    /*****/
    //Формуємо значення UAB_TN1
    /*****/
    if ((command_word & (1 << I_UAB_TN1)) != 0)
    {
      _x1 = ADCs_data_raw[I_UAB_TN1].tick;
      _y1 = ADCs_data_raw[I_UAB_TN1].value;

      _y2 = output_adc[C_UAB_TN1].value - /*gnd_adc - */ vref_averange;
      if (abs(_y2) > 87)
      {
        _x2 = output_adc[C_UAB_TN1].tick;
        _y2 = (int) (_y2 * ustuvannja_meas[I_UAB_TN1]) >> (USTUVANNJA_VAGA - 4);
      }
      else
      {
        _y2 = output_adc[C_UAB_TN1_16].value - /*gnd_adc - */ vref_averange;

        _x2 = output_adc[C_UAB_TN1_16].tick;
        _y2 = (int) ((-_y2) * ustuvannja_meas[I_UAB_TN1]) >> (USTUVANNJA_VAGA);
      }

      if (_x2 > _x1)
        _DX = _x2 - _x1;
      else
      {
        uint64_t _DX_64 = _x2 + 0x100000000 - _x1;
        _DX = _DX_64;
      }
      if (_x >= _x1)
        _dx = _x - _x1;
      else
      {
        uint64_t _dx_64 = _x + 0x100000000 - _x1;
        _dx = _dx_64;
      }
      _y = ((long long) _y1) + ((long long) (_y2 - _y1)) * ((long long) _dx) / ((long long) _DX);

      if ((_y >= 0) && (ADCs_data[I_UAB_TN1] < 0))
      {
        /*
        Зафіксований перехід через нуль
        */
        //Попередній перехід
        perechid_cherez_nul[INDEX_PhK_UAB_TN1][0].x1 = poperednij_perechid.Uab_TN1_x1;
        perechid_cherez_nul[INDEX_PhK_UAB_TN1][0].y1 = poperednij_perechid.Uab_TN1_y1;
        perechid_cherez_nul[INDEX_PhK_UAB_TN1][0].x2 = poperednij_perechid.Uab_TN1_x2;
        perechid_cherez_nul[INDEX_PhK_UAB_TN1][0].y2 = poperednij_perechid.Uab_TN1_y2;

        //Поточний перехід
        poperednij_perechid.Uab_TN1_x1 = perechid_cherez_nul[INDEX_PhK_UAB_TN1][1].x1 = penultimate_tick_DATA_VAL;
        poperednij_perechid.Uab_TN1_y1 = perechid_cherez_nul[INDEX_PhK_UAB_TN1][1].y1 = ADCs_data[I_UAB_TN1];
        poperednij_perechid.Uab_TN1_x2 = perechid_cherez_nul[INDEX_PhK_UAB_TN1][1].x2 = _x;
        poperednij_perechid.Uab_TN1_y2 = perechid_cherez_nul[INDEX_PhK_UAB_TN1][1].y2 = _y;

        //Помічаємо, що перехід зафіксований
        fix_perechid_cherez_nul[INDEX_PhK_UAB_TN1] = 0xff;
      }

      ADCs_data[I_UAB_TN1] = _y;

      ADCs_data_raw[I_UAB_TN1].tick = _x2;
      ADCs_data_raw[I_UAB_TN1].value = _y2;
    }
    /*****/

    /*****/
    //Формуємо значення UAB_TN2
    /*****/
    if ((command_word & (1 << I_UAB_TN2)) != 0)
    {
      _x1 = ADCs_data_raw[I_UAB_TN2].tick;
      _y1 = ADCs_data_raw[I_UAB_TN2].value;

      _y2 = output_adc[C_UAB_TN2].value - /*gnd_adc - */ vref_averange;
      if (abs(_y2) > 87)
      {
        _x2 = output_adc[C_UAB_TN2].tick;
        _y2 = (int) (_y2 * ustuvannja_meas[I_UAB_TN2]) >> (USTUVANNJA_VAGA - 4);
      }
      else
      {
        _y2 = output_adc[C_UAB_TN2_16].value - /*gnd_adc - */ vref_averange;

        _x2 = output_adc[C_UAB_TN2_16].tick;
        _y2 = (int) ((-_y2) * ustuvannja_meas[I_UAB_TN2]) >> (USTUVANNJA_VAGA);
      }

      if (_x2 > _x1)
        _DX = _x2 - _x1;
      else
      {
        uint64_t _DX_64 = _x2 + 0x100000000 - _x1;
        _DX = _DX_64;
      }
      if (_x >= _x1)
        _dx = _x - _x1;
      else
      {
        uint64_t _dx_64 = _x + 0x100000000 - _x1;
        _dx = _dx_64;
      }
      _y = ((long long) _y1) + ((long long) (_y2 - _y1)) * ((long long) _dx) / ((long long) _DX);

      if ((_y >= 0) && (ADCs_data[I_UAB_TN2] < 0))
      {
        /*
        Зафіксований перехід через нуль
        */
        //Попередній перехід
        perechid_cherez_nul[INDEX_PhK_UAB_TN2][0].x1 = poperednij_perechid.Uab_TN1_x1;
        perechid_cherez_nul[INDEX_PhK_UAB_TN2][0].y1 = poperednij_perechid.Uab_TN1_y1;
        perechid_cherez_nul[INDEX_PhK_UAB_TN2][0].x2 = poperednij_perechid.Uab_TN1_x2;
        perechid_cherez_nul[INDEX_PhK_UAB_TN2][0].y2 = poperednij_perechid.Uab_TN1_y2;

        //Поточний перехід
        poperednij_perechid.Uab_TN1_x1 = perechid_cherez_nul[INDEX_PhK_UAB_TN2][1].x1 = penultimate_tick_DATA_VAL;
        poperednij_perechid.Uab_TN1_y1 = perechid_cherez_nul[INDEX_PhK_UAB_TN2][1].y1 = ADCs_data[I_UAB_TN2];
        poperednij_perechid.Uab_TN1_x2 = perechid_cherez_nul[INDEX_PhK_UAB_TN2][1].x2 = _x;
        poperednij_perechid.Uab_TN1_y2 = perechid_cherez_nul[INDEX_PhK_UAB_TN2][1].y2 = _y;

        //Помічаємо, що перехід зафіксований
        fix_perechid_cherez_nul[INDEX_PhK_UAB_TN2] = 0xff;
      }
      ADCs_data[I_UAB_TN2] = _y;

      ADCs_data_raw[I_UAB_TN2].tick = _x2;
      ADCs_data_raw[I_UAB_TN2].value = _y2;
    }
    /*****/

    /*****/
    //Формуємо значення UC1C2
    /*****/
    if ((command_word & (1 << I_UC1C2)) != 0)
    {
      _x1 = ADCs_data_raw[I_UC1C2].tick;
      _y1 = ADCs_data_raw[I_UC1C2].value;

      _y2 = output_adc[C_UC1C2_1].value - /*gnd_adc - */ vref_averange;
      if (abs(_y2) > 87)
      {
        _x2 = output_adc[C_UC1C2_1].tick;
        _y2 = (int) (_y2 * ustuvannja_meas[I_UC1C2]) >> (USTUVANNJA_VAGA - 4);
      }
      else
      {
        _y2 = output_adc[C_UC1C2_16].value - /*gnd_adc - */ vref_averange;

        _x2 = output_adc[C_UC1C2_16].tick;
        _y2 = (int) ((-_y2) * ustuvannja_meas[I_UC1C2]) >> (USTUVANNJA_VAGA);
      }

      if (_x2 > _x1)
        _DX = _x2 - _x1;
      else
      {
        uint64_t _DX_64 = _x2 + 0x100000000 - _x1;
        _DX = _DX_64;
      }
      if (_x >= _x1)
        _dx = _x - _x1;
      else
      {
        uint64_t _dx_64 = _x + 0x100000000 - _x1;
        _dx = _dx_64;
      }
      _y = ((long long) _y1) + ((long long) (_y2 - _y1)) * ((long long) _dx) / ((long long) _DX);
      ADCs_data[I_UC1C2] = _y;

      ADCs_data_raw[I_UC1C2].tick = _x2;
      ADCs_data_raw[I_UC1C2].value = _y2;
    }
    /*****/

    /*****/
    //Формуємо значення UP1P2
    /*****/
    if ((command_word & (1 << I_UP1P2)) != 0)
    {
      _x1 = ADCs_data_raw[I_UP1P2].tick;
      _y1 = ADCs_data_raw[I_UP1P2].value;

      _y2 = output_adc[C_UP1P2_1].value - /*gnd_adc - */ vref_averange;
      if (abs(_y2) > 87)
      {
        _x2 = output_adc[C_UP1P2_1].tick;
        _y2 = (int) (_y2 * ustuvannja_meas[I_UP1P2]) >> (USTUVANNJA_VAGA - 4);
      }
      else
      {
        _y2 = output_adc[C_UP1P2_16].value - /*gnd_adc - */ vref_averange;

        _x2 = output_adc[C_UP1P2_16].tick;
        _y2 = (int) ((-_y2) * ustuvannja_meas[I_UP1P2]) >> (USTUVANNJA_VAGA);
      }

      if (_x2 > _x1)
        _DX = _x2 - _x1;
      else
      {
        uint64_t _DX_64 = _x2 + 0x100000000 - _x1;
        _DX = _DX_64;
      }
      if (_x >= _x1)
        _dx = _x - _x1;
      else
      {
        uint64_t _dx_64 = _x + 0x100000000 - _x1;
        _dx = _dx_64;
      }
      _y = ((long long) _y1) + ((long long) (_y2 - _y1)) * ((long long) _dx) / ((long long) _DX);
      ADCs_data[I_UP1P2] = _y;

      ADCs_data_raw[I_UP1P2].tick = _x2;
      ADCs_data_raw[I_UP1P2].value = _y2;
    }
    /*****/

    /*****/
    //Формуємо значення UP2P3
    /*****/
    if ((command_word & (1 << I_UP2P3)) != 0)
    {
      _x1 = ADCs_data_raw[I_UP2P3].tick;
      _y1 = ADCs_data_raw[I_UP2P3].value;

      _y2 = output_adc[C_UP2P3_1].value - /*gnd_adc - */ vref_averange;
      if (abs(_y2) > 87)
      {
        _x2 = output_adc[C_UP2P3_1].tick;
        _y2 = (int) (_y2 * ustuvannja_meas[I_UP2P3]) >> (USTUVANNJA_VAGA - 4);
      }
      else
      {
        _y2 = output_adc[C_UP2P3_16].value - /*gnd_adc - */ vref_averange;

        _x2 = output_adc[C_UP2P3_16].tick;
        _y2 = (int) ((-_y2) * ustuvannja_meas[I_UP2P3]) >> (USTUVANNJA_VAGA);
      }

      if (_x2 > _x1)
        _DX = _x2 - _x1;
      else
      {
        uint64_t _DX_64 = _x2 + 0x100000000 - _x1;
        _DX = _DX_64;
      }
      if (_x >= _x1)
        _dx = _x - _x1;
      else
      {
        uint64_t _dx_64 = _x + 0x100000000 - _x1;
        _dx = _dx_64;
      }
      _y = ((long long) _y1) + ((long long) (_y2 - _y1)) * ((long long) _dx) / ((long long) _DX);
      ADCs_data[I_UP2P3] = _y;

      ADCs_data_raw[I_UP2P3].tick = _x2;
      ADCs_data_raw[I_UP2P3].value = _y2;
    }
    /*****/

    unsigned int head_data_for_oscylograph_tmp = head_data_for_oscylograph;
    unsigned int x2, x1, delta_x;

    if ((status_adc_read_work & DATA_VAL_READ) != 0)
    {
      /*
      Необхідно опрацювати оцифровані дані для перетворення Фур'є
      */
      Fourier();

      /*
      Виконуємо операції по визначенню частоти і підстройці частоти
      */
      fapch();
      if (freq_mutex == false)
        frequency_high = frequency_irq;

      //Формуємо дані для розширеної виборки
      x1 = rozshyrena_vyborka.time_p = penultimate_tick_DATA_VAL;
      x2 = rozshyrena_vyborka.time_c = previous_tick_DATA_VAL;
      for (unsigned int i = 0; i < NUMBER_ANALOG_CANALES; i++)
      {
        rozshyrena_vyborka.data_p[i] = rozshyrena_vyborka.data_c[i];
        rozshyrena_vyborka.frequency_p = rozshyrena_vyborka.frequency_c;
        rozshyrena_vyborka.data_c[i] = ADCs_data[i];
        rozshyrena_vyborka.frequency_c = frequency_irq;
      }

      /*******************************************************
      Формування апроксимованих значень
      *******************************************************/
      if (head_data_for_oscylograph_tmp != DATA_VAL_tail_data_for_oscylograph)
      {
        if (x2 > x1)
          delta_x = x2 - x1;
        else
        {
          long long delta_x_64 = x2 + 0x100000000 - x1;
          delta_x = delta_x_64;
        }

        while (head_data_for_oscylograph_tmp != DATA_VAL_tail_data_for_oscylograph)
        {
          unsigned int DATA_VAL_tail_data_for_oscylograph_tmp = DATA_VAL_tail_data_for_oscylograph;

          unsigned int x = data_for_oscylograph[DATA_VAL_tail_data_for_oscylograph_tmp].time_stemp;

          unsigned int dx;
          if (x >= x1)
            dx = x - x1;
          else
          {
            long long dx_64 = x + 0x100000000 - x1;
            dx = dx_64;
          }

          if (dx > delta_x)
          {
            break;
          }

          for (unsigned int i = 0; i < NUMBER_ANALOG_CANALES; i++)
          {
            int y1 = rozshyrena_vyborka.data_p[i], y2 = rozshyrena_vyborka.data_c[i];
            long long y;
            if (dx <= delta_x)
            {
              y = ((long long) (y2 - y1)) * ((long long) dx) / ((long long) delta_x) + ((long long) y1);
            }
            else
            {
              y = 0;
            }
            data_for_oscylograph[DATA_VAL_tail_data_for_oscylograph_tmp].data[I_IA_1 + i] = y;
          }
          //Частота
          {
            int const y1 = (int) (roundf(rozshyrena_vyborka.frequency_p * 100.0f));
            int const y2 = (int) (roundf(rozshyrena_vyborka.frequency_c * 100.0f));
            int y = 0x8000;
            if (
              (y1 > 0) &&
              (y2 > 0) &&
              (dx <= delta_x))
            {
              y = ((long long) (y2 - y1)) * ((long long) dx) / ((long long) delta_x) + ((long long) y1);
            }
            data_for_oscylograph[DATA_VAL_tail_data_for_oscylograph_tmp].data[NUMBER_ANALOG_CANALES + 0] = y;
          }
          data_for_oscylograph[DATA_VAL_tail_data_for_oscylograph_tmp].DATA_fix = 0xff;

          if (++DATA_VAL_tail_data_for_oscylograph >= MAX_INDEX_DATA_FOR_OSCYLOGRAPH)
            DATA_VAL_tail_data_for_oscylograph = 0;
        }
      }
      /******************************************************/

      status_adc_read_work &= (unsigned int) (~DATA_VAL_READ);

      /**************************************************/
      //Виставляємо повідомлення про завершення оброки першої групи вимірювальних величин
      /**************************************************/
      control_word_of_watchdog |= WATCHDOG_MEASURE_STOP_DATA_VAL;
      /**************************************************/
    }

    /********************************************************
    Формуємо масив миттєвих значень і виконуємо операції для аналогового реєстратора
    ********************************************************/
    if (head_data_for_oscylograph_tmp != tail_data_for_oscylograph)
    {
      unsigned int working_ar = false; /*по замовчуванню ставимо, що Аналоговий реєстратор не працює*/
      unsigned int const index_array_ar_current_before = index_array_ar_current;
      while (
        (head_data_for_oscylograph_tmp != tail_data_for_oscylograph) &&
        (data_for_oscylograph[tail_data_for_oscylograph].DATA_fix != 0))
      {
        unsigned int const tail_data_for_oscylograph_tmp = tail_data_for_oscylograph;

        if ((prescaler_ar & MASKA_BIT_FOR_PRESCALER) == 0)
        {
          for (unsigned int i = 0; i < NUMBER_ANALOG_CANALES; i++)
          {
            int data_tmp = data_for_oscylograph[tail_data_for_oscylograph_tmp].data[i];
            //Масив миттєвих аналогових виборок для аналогового реєстратора
            //            array_ar[index_array_ar_current++] = data_tmp;
            AR_WRITE(index_array_ar_current, data_tmp);
          }
          //Частота
          int data_tmp = data_for_oscylograph[tail_data_for_oscylograph_tmp].data[NUMBER_ANALOG_CANALES + 0];
          AR_WRITE(index_array_ar_current, data_tmp);
        }

        //Масив дискретних сигналів для аналогового реєстратора
        unsigned int *label_to_active_functions_source = data_for_oscylograph[tail_data_for_oscylograph_tmp].active_functions;
        if ((prescaler_ar & MASKA_BIT_FOR_PRESCALER) != 0)
        {
          for (unsigned int i = 0; i < N_BIG; i++)
            active_functions_trg[i] = *(label_to_active_functions_source + i);
        }
        else
        {
          for (unsigned int i = 0; i < N_BIG; i++)
            active_functions_trg[i] |= *(label_to_active_functions_source + i);

          unsigned short int *label_to_active_functions_trg = (unsigned short int *) active_functions_trg;
          for (unsigned int i = 0; i < NUMBER_WORD_DIGITAL_PART_AR; i++)
          {
            AR_WRITE(index_array_ar_current, *(label_to_active_functions_trg + i));
          }
          //Індекс масиву об'єднаних виборок для аналогового реєстратора
          if (index_array_ar_current >= SIZE_BUFFER_FOR_AR)
            index_array_ar_current = 0; /*Умова мала б бути ==, але щоб перестахуватися на невизначену помилку я поставив >=*/

          unsigned int state_ar_record_m_tmp = data_for_oscylograph[tail_data_for_oscylograph_tmp].state_ar_record;
          if (
            (state_ar_record_m_tmp == STATE_AR_NONE_M) &&
            ((state_ar_record_fatfs == STATE_AR_NONE_FATFS) ||
             (state_ar_record_fatfs == STATE_AR_STOP_WRITE_FATFS) ||
             (state_ar_record_fatfs == STATE_AR_MEMORY_FULL_FATFS) ||
             (state_ar_record_fatfs == STATE_AR_BLOCK_FATFS)) /*умова, що на даний момент часу не ішов запис даних у енергонезалежну пам'ять*/
          )
          {
            working_ar = false;
            //Випадок, коли Аналоговий реєстратор не працює
            index_array_ar_tail = index_array_tail_min = index_array_ar_heat = index_array_ar_current;
            tail_to_heat = current_to_tail = false;
          }
          else
          {
            working_ar = true;

            if (state_ar_record_m_tmp == STATE_AR_WORK_M)
            {
              /*
              Вже новий зріз післяаварійного масиву доданий у масив
              */
              if (
                (prev_state_ar_record_m == STATE_AR_NONE_M) &&
                (state_ar_record_fatfs == STATE_AR_WAIT_TO_WRITE_FATFS))
              {
                //Умова, що треба включити доаварійний масив для запису

                int difference;
                /*
                оскільки 1 післяаварійних зрізів доданио у масив,
                то для визначення першої мітки післяаварійного масиву від index_array_ar_current
                відняти кількість миттєвих значень у одному зрізі
                */
                /*
                Встановлюємо мітку першого миттєвого значееня післяаваріного масиву і 
                тимчасово помісчаємо її у змінну "вигрузки" для того, щоб дальша програма 
                мала "універсальний", тобто прстіший, вигляд
                */
                difference = index_array_ar_current - AR_TOTAL_NUMBER_CANALES;
                if (difference >= 0)
                  index_array_ar_tail = difference;
                else
                  index_array_ar_tail = difference + SIZE_BUFFER_FOR_AR;

                //Встановлюємо мітку "вигрузки"
                //                difference = index_array_ar_tail - (current_settings_prt.prefault_number_periods << VAGA_NUMBER_POINT_AR)*AR_TOTAL_NUMBER_CANALES;

                //Встановлюємо мітку "вигрузки" в умовах холодного або близького КЗ з пропаданням оперативного живлення
                difference = index_array_ar_tail - (AR_TAIL_MIN_NUMBER_PERIOD << VAGA_NUMBER_POINT_AR) * AR_TOTAL_NUMBER_CANALES;
                if (difference >= 0)
                  index_array_tail_min = difference;
                else
                  index_array_tail_min = difference + SIZE_BUFFER_FOR_AR;

                //Встановлюємо мітку "вигрузки"
                difference = index_array_ar_tail - (header_ar.prefault_number_periods << VAGA_NUMBER_POINT_AR) * AR_TOTAL_NUMBER_CANALES;

                if (difference >= 0)
                  index_array_ar_tail = difference;
                else
                  index_array_ar_tail = difference + SIZE_BUFFER_FOR_AR;
              }

              index_array_ar_heat = index_array_ar_current;
              tail_to_heat = false;
            }
          }

          prev_state_ar_record_m = state_ar_record_m_tmp;
        }
        prescaler_ar++;

        data_for_oscylograph[tail_data_for_oscylograph_tmp].DATA_fix = 0;

        if (++tail_data_for_oscylograph >= MAX_INDEX_DATA_FOR_OSCYLOGRAPH)
          tail_data_for_oscylograph = 0;
      }

      /***
      Визначаємо, чи не відбулося переповнення      
      working_ar - значення у цьому місці відповідає останньому часовому зрізу, який доданий у буфер
      ***/

      if (working_ar != false)
      {
        /***
        Початок відділку беремо від index_array_ar_current_before;
        ***/

        int end_tmp = index_array_ar_current - index_array_ar_current_before;
        if (end_tmp < 0)
          end_tmp += SIZE_BUFFER_FOR_AR;

        int tail_tmp = index_array_ar_tail - index_array_ar_current_before;
        if (
          (tail_tmp < 0) ||
          ((tail_tmp == 0) &&
           (current_to_tail == false)))
        {
          tail_tmp += SIZE_BUFFER_FOR_AR;
        }

        if (tail_tmp <= end_tmp)
        {
          current_to_tail = true;

          if (tail_tmp < end_tmp)
          {
            //Помилкова ситуація, яка викликана переповненням
            _SET_BIT(set_diagnostyka, ERROR_AR_OVERLOAD_BUFFER_BIT);

            state_ar_record_m = STATE_AR_BLOCK_M;
          }
        }
        else
          current_to_tail = false;

        diff_index_heat_tail = tail_tmp - end_tmp;
      }
      else
        diff_index_heat_tail = -1;

      diff_index_heat_tail = index_array_ar_current - index_array_ar_tail;
      if (diff_index_heat_tail < 0)
        diff_index_heat_tail += SIZE_BUFFER_FOR_AR;
    }
    /*******************************************************/

    if ((status_adc_read_work & TEST_VAL_READ) != 0)
    {
      //Треба опрацювати інтегральні величини
      operate_integral_values_ADCs();

      status_adc_read_work &= (unsigned int) (~TEST_VAL_READ);

      /**************************************************/
      //Виставляємо повідомлення про завершення тестових величин
      /**************************************************/
      control_word_of_watchdog |= WATCHDOG_MEASURE_STOP_TEST_VAL;
      /**************************************************/

      /**************************************************/
      //Якщо зараз стоїть блокування то його знімаємо
      /**************************************************/
      if (_CHECK_SET_BIT(control_i2c_taskes, TASK_BLK_OPERATION_BIT) != 0)
      {
        //Повне роозблоковування обміну з мікросхемами для драйверу I2C
        _CLEAR_BIT(control_i2c_taskes, TASK_BLK_OPERATION_BIT);
      }
      /**************************************************/
    }

#ifdef SYSTEM_VIEWER_ENABLE
    SEGGER_SYSVIEW_RecordExitISR();
#endif

#ifdef _TEST_DURATION
    uint32_t const stop_tick = TIM2->CNT;
    uint64_t const delta_tick = (stop_tick > start_tick) ? (stop_tick - start_tick) : (0x100000000ull + stop_tick - start_tick);

    static uint64_t durMeas_L1Max = 0;
    static uint64_t durMeas_L1Min = 0x100000000ull;
    static uint32_t durMeas_L1Reset;

    if (durMeas_L1Reset != 0)
    {
      durMeas_L1Max = 0;
      durMeas_L1Min = 0x100000000ull;
      durMeas_L1Reset = 0;
    }
    durMeas_L1Cur = delta_tick;
    if (durMeas_L1Max < delta_tick)
      durMeas_L1Max = delta_tick;
    if (durMeas_L1Min > delta_tick)
      durMeas_L1Min = delta_tick;
#endif
  }

  if ((GPIO_SPI_ADC->ODR & GPIO_NSSPin_ADC) != 0)
  {
    //Новий обмін не почався функцією control_reading_ADCs

    /*
    Можливо з часу останнього виклику функції control_reading_ADCs до даного
    моментк переривання від таймера поставило нові задачі на оцифровку
    */

    /*
    Забороняємо генерацію переривань, щоб поки ми аналізуємо чяи немає нових даних
    на оцифровку ще новіші не появилися
    */
    __disable_interrupt();

    /*
    Аналізуємо, чи немає нових даних на оцифровку і якщо до даного моменту нових
    даних не появилося, то розблоковуємо можливість початку оцифровки з переривання
    таймеру ( chip select виставлений у 1)
    */
    if (
      (adc_DATA_VAL_read == false) /*&&
        (adc_TEST_VAL_read == false)*/
    )
    {
      semaphore_adc_irq = false;
    }

    /*
    Дозволяємо генерацію переривань
    */
    __enable_interrupt();
  }
  else
    semaphore_adc_irq = false;

  if (semaphore_adc_irq != false)
  {
    /*
    Ця умова може бути тыльки у одному випадку: якщо при аналізі на нові дані на оцифровку
    з моменту отаннього запуску функції control_reading_ADCs до заборони переривань
    функцією __disable_interrupt дані всетаки появилися.
    
    Модемо і маємо запусти функцію control_reading_ADCs. Конфлікту не буде, бо
    chip_select покищо виставлений у високий стан (це дає остання перевірка 
    if ((GPIO_SPI_ADC->ODR & GPIO_NSSPin_ADC) != 0)) і змінна semaphore_adc_irq ще
    не рівна false, а тим чином блокує запуск оцифровки х переривання таймеру
    */

    control_reading_ADCs();

    /*
    Скидаємо прапорець. який сигналізує що ми у перериванні обробки оцифрованих даних, які прийшли по SPI
    інтерфейсу. Але цей прапорець вже нічого не блокує, бо запуск функції control_reading_ADCs
    при гарантованій умові, що дані на оцифровку "стоять у черзі"  мусить почати
    оцифровку - а це можливе тільки тоді, коли chip_select цією функцією буде вустановлено у низький рівень
    
    А встановиться chip_select назад у високий рівень тільки коли знову згенерується переривання від
    каналі SPI
    */
    semaphore_adc_irq = false;
  }

  //#ifdef SYSTEM_VIEWER_ENABLE
  //  SEGGER_SYSVIEW_RecordExitISR();
  //#endif
}
/*****************************************************/

/*****************************************************/
//Пошук розрядності числа
/*****************************************************/
inline unsigned int norma_value(unsigned long long y)
{
  unsigned long long temp = y;
  unsigned int rezultat = 0;
  if (temp == 0)
    return 0;

  while (temp != 0)
  {
    temp = temp >> 1;
    rezultat++;
  }
  return rezultat - 1;
}
/*****************************************************/

/*****************************************************/
//Пошук розрядності числа типу int
/*****************************************************/
//#pragma optimize=none
inline unsigned int get_order(int value)
{
  unsigned int i = 0;

  if (value == 0)
    return 1;
  else if (value == 0x80000000)
    return 32;

  if (value < 0)
    value = -value;

  while ((value >> (++i)) != 0)
    ;

  return i;
}
/*****************************************************/

/*****************************************************/
//Розрахунок кореня квадратного методом половинного ділення  з прогнозуванням розрядності числа
/*****************************************************/
unsigned int sqrt_64(unsigned long long y)
{
  unsigned int b;
  unsigned int a;
  unsigned int c;
  unsigned int norma_rez;
  unsigned long long temp;

  norma_rez = norma_value(y) >> 1;

  a = (1 << norma_rez) - 1;
  b = (1 << (norma_rez + 1));

  do
  {
    c = (a + b) >> 1;
    temp = (unsigned long long) c * (unsigned long long) c;
    if (temp != y)
    {
      if (temp > y)
        b = c;
      else
        a = c;
    }
    else
      return c;
  } while ((b - a) > 1);

  c = (a + b) >> 1;

  return c;
}
/*****************************************************/

/*****************************************************/
//Розрахунок кореня квадратного методом половинного ділення  з прогнозуванням розрядності числа
/*****************************************************/
inline unsigned int sqrt_32(unsigned int y)
{
  unsigned int b;
  unsigned int a;
  unsigned int c;
  unsigned int norma_rez;
  unsigned int temp;

  norma_rez = norma_value(y) >> 1;

  a = (1 << norma_rez) - 1;
  b = (1 << (norma_rez + 1));

  do
  {
    c = (a + b) >> 1;
    temp = c * c;
    if (temp != y)
    {
      if (temp > y)
        b = c;
      else
        a = c;
    }
    else
      return c;
  } while ((b - a) > 1);

  c = (a + b) >> 1;

  return c;
}
/*****************************************************/

/*****************************************************/
//Детектор кутів
/*****************************************************/
void angle_determination(void)
{
  int rezult_angle_1 = 0;
  int rezult_angle_2 = 0;
  int ortogonal_low_tmp[6];

  /***
   Копіювання масивів ортогональних
   ***/
  //Виставляємо семафор заборони обновлення значень з вимірювальної системи
  semaphore_measure_values_low = 1;
  ortogonal_low_tmp[0] = ortogonal_calc_low[2 * FULL_ORT_UC1C2];     //a1
  ortogonal_low_tmp[1] = ortogonal_calc_low[2 * FULL_ORT_UC1C2 + 1]; //b1
  ortogonal_low_tmp[2] = ortogonal_calc_low[2 * FULL_ORT_UP1P2];     //a2
  ortogonal_low_tmp[3] = ortogonal_calc_low[2 * FULL_ORT_UP1P2 + 1]; //b2
  ortogonal_low_tmp[4] = ortogonal_calc_low[2 * FULL_ORT_UP2P3];     //a1
  ortogonal_low_tmp[5] = ortogonal_calc_low[2 * FULL_ORT_UP2P3 + 1]; //b1
  //Знімаємо семафор заборони обновлення значень з вимірювальної системи
  semaphore_measure_values_low = 0;
  /***/

  unsigned int amplituda_UC1C2 = sqrt_32((unsigned int) (ortogonal_low_tmp[0] * ortogonal_low_tmp[0]) + (unsigned int) (ortogonal_low_tmp[1] * ortogonal_low_tmp[1]));
  unsigned int amplituda_UP1P2 = sqrt_32((unsigned int) (ortogonal_low_tmp[2] * ortogonal_low_tmp[2]) + (unsigned int) (ortogonal_low_tmp[3] * ortogonal_low_tmp[3]));
  unsigned int amplituda_UP2P3 = sqrt_32((unsigned int) (ortogonal_low_tmp[4] * ortogonal_low_tmp[4]) + (unsigned int) (ortogonal_low_tmp[5] * ortogonal_low_tmp[5]));

  int cos_fi_1, sin_fi_1, cos_fi_2, sin_fi_2;
  float sin_fi_f_1, sin_fi_f_2, angle_f_1, angle_f_2;
  int angle_int_1, angle_int_2;
  if ((amplituda_UP1P2 != 0) && (amplituda_UC1C2 != 0))
  {
    /***
        //Тригонометричні координати вектора різниці
        ***/
    /*
        За розрахунком описаним при розрахунку діючих значень наші ортогональні є у форматі (15 біт + знак) = 16-розрядне число
        Тоді максимальний результат може бути 2*(15 біт + знак) = 32-розряжне число зі знаком (31 біт + знак)
        */
    cos_fi_1 = (ortogonal_low_tmp[1] * ortogonal_low_tmp[3] + ortogonal_low_tmp[0] * ortogonal_low_tmp[2]);
    sin_fi_1 = (ortogonal_low_tmp[0] * ortogonal_low_tmp[3] - ortogonal_low_tmp[1] * ortogonal_low_tmp[2]);
    /***/

    sin_fi_f_1 = (((float) sin_fi_1) / ((float) amplituda_UP1P2)) / ((float) amplituda_UC1C2);
    if (sin_fi_f_1 > 1)
      sin_fi_f_1 = 1;
    else if (sin_fi_f_1 < -1)
      sin_fi_f_1 = -1;
    angle_f_1 = asinf(sin_fi_f_1) * 180.0f / PI;
    angle_int_1 = (int) angle_f_1;

    if (angle_int_1 < 0)
      angle_int_1 = -angle_int_1;

    if ((sin_fi_1 >= 0) && (cos_fi_1 >= 0))
    {
      //1-ий квадрант
      rezult_angle_1 = angle_int_1;
    }
    else if ((sin_fi_1 >= 0) && (cos_fi_1 < 0))
    {
      //2-ий квадрант
      rezult_angle_1 = 180 - angle_int_1;
    }
    else if ((sin_fi_1 < 0) && (cos_fi_1 < 0))
    {
      //3-ий квадрант
      rezult_angle_1 = 180 + angle_int_1;
    }
    else
    {
      //4-ий квадрант
      rezult_angle_1 = 360 - angle_int_1;
    }

    if (rezult_angle_1 >= 360)
      rezult_angle_1 -= 360;
    else if (rezult_angle_1 < 0)
      rezult_angle_1 += 360;
  }
  else
  {
    rezult_angle_1 = 0;
  }

  if ((amplituda_UC1C2 != 0) && (amplituda_UP2P3 != 0))
  {

    /***
        //Тригонометричні координати вектора різниці
        ***/
    /*
        За розрахунком описаним при розрахунку діючих значень наші ортогональні є у ворматі (15 біт + знак) = 16-розряжне число
        Тоді максимальний результат може бути 2*(15 біт + знак) = 32-розряжне число зі знаком (31 біт + знак)
        */
    cos_fi_2 = (ortogonal_low_tmp[1] * ortogonal_low_tmp[5] + ortogonal_low_tmp[0] * ortogonal_low_tmp[4]);
    sin_fi_2 = (ortogonal_low_tmp[0] * ortogonal_low_tmp[5] - ortogonal_low_tmp[1] * ortogonal_low_tmp[4]);
    /***/

    sin_fi_f_2 = (((float) sin_fi_2) / ((float) amplituda_UP2P3)) / ((float) amplituda_UC1C2);
    if (sin_fi_f_2 > 1)
      sin_fi_f_2 = 1;
    else if (sin_fi_f_2 < -1)
      sin_fi_f_2 = -1;
    angle_f_2 = asinf(sin_fi_f_2) * 180 / PI;
    angle_int_2 = (int) angle_f_2;

    if (angle_int_2 < 0)
      angle_int_2 = -angle_int_2;

    if ((sin_fi_2 >= 0) && (cos_fi_2 >= 0))
    {
      //1-ий квадрант
      rezult_angle_2 = angle_int_2;
    }
    else if ((sin_fi_2 >= 0) && (cos_fi_2 < 0))
    {
      //2-ий квадрант
      rezult_angle_2 = 180 - angle_int_2;
    }
    else if ((sin_fi_2 < 0) && (cos_fi_2 < 0))
    {
      //3-ий квадрант
      rezult_angle_2 = 180 + angle_int_2;
    }
    else
    {
      //4-ий квадрант
      rezult_angle_2 = 360 - angle_int_2;
    }

    if (rezult_angle_2 >= 360)
      rezult_angle_2 -= 360;
    else if (rezult_angle_2 < 0)
      rezult_angle_2 += 360;
  }
  else
  {
    rezult_angle_2 = 0;
  }
  angle_UP1P2_UC1C2 = rezult_angle_1;
  angle_UP2P3_UC1C2 = rezult_angle_2;
}
/*****************************************************/

/*****************************************************/
//Розрахунки віимірювань
/*****************************************************/
void calc_measurement(unsigned int number_group_stp)
{
  UNUSED(number_group_stp);

#ifdef _TEST_DURATION
  uint32_t const start_tick = TIM2->CNT;
#endif

  int ortogonal_local[2 * NUMBER_ANALOG_CANALES];

  //Виставляємо семафор заборони обновлення значень з вимірювальної системи
  //  semaphore_measure_values = 1;

  //Копіюємо вхідні велечини у локальні змінні
  /*
  оскільки для дискретного перетворення Фурє коефіцієнти діляться на число виборок і множиться на 2 (еквівалент 2/Т),
  то це ми можемо зробити зміщенням
  Крім того ми використовували табуляцію сінуса з амплітудою AMPLITUDA_SINUS=(1<<SINUS_VAGA)
  Зараз цю амплытуду треба прибрати
  */
  /*
  Проведені мною розрахунки показують, що якщо просумувати добуток миттєвих значень на синус/косинус за період,
  а потім результат поділити на 2/Т (зробити це відповідним зсуваом, про який я писав вище) і забрати з результату множення аплітуду синуса/косинуса,
  то максимана розрядність резутьтату буде рівна макисальній розрядності вхідного сигналу
  Тобто для 3I0            - це 19 біт + знак = ((11 біт + знак)*16*16)
        для фазних струмів - це 15 біт + знак = ((11 біт + знак)*16   )
  оскільки нам ще треба це число піднімати до квадрату а аж потім добувати корінь квадратний з суми квадратів, то
  фазний струм можна підносити до кваррату - переповнення не буде, бо (15 біт *2) = 30 біт < 32 біт unsigned int
  А аж потім забрати множенння на 16, щоб збільшити точність вимірювання
  
  Для 3I0 можливе переповнення
  Тому я пропоную перше 16-кратне підсилення забрати прямо з ортогональних для 3I0,
  тоді ортогоанльні стануть не більше 15-розрядного числа + знак.
  Другк 16-кратне підсилення забрати вже в остаточному результаті
  */

  unsigned int bank_ortogonal_tmp = (bank_ortogonal + 1) & 0x1;
  for (unsigned int i = 0; i < (2 * NUMBER_ANALOG_CANALES); i++)
  {
    ortogonal_local[i] = ortogonal[i][bank_ortogonal_tmp];
  }
  adc2_channel0_averange_prt = adc2_channel0_averange[bank_ortogonal_tmp];
  adc2_channel1_averange_prt = adc2_channel0_averange[bank_ortogonal_tmp];
  bank_ortogonal = bank_ortogonal_tmp;
  adc2_read_after_start = true;

  freq_mutex = true;
  frequency = frequency_high;
  freq_mutex = false;

  //Знімаємо семафор заборони обновлення значень з вимірювальної системи
  //  semaphore_measure_values = 0;

  /***
  Довертаємо кути і копіюємо ортогональні для низькопріоритетних задач
  ***/
  unsigned int copy_to_low_tasks = (semaphore_measure_values_low == 0) ? true : false;
  for (unsigned int i = 0; i < NUMBER_ANALOG_CANALES; i++)
  {
    unsigned int new_index = index_converter[i];
    int ortogonal_sin = ortogonal_calc[2 * new_index] = (int) (((float) ortogonal_local[2 * i]) / ((float) ((1 << (VAGA_NUMBER_POINT - 1)))));
    int ortogonal_cos = ortogonal_calc[2 * new_index + 1] = (int) (((float) ortogonal_local[2 * i + 1]) / ((float) ((1 << (VAGA_NUMBER_POINT - 1)))));

    //Копіюємо ортогональні для розрахунку кутів
    if (copy_to_low_tasks == true)
    {
      ortogonal_calc_low[2 * new_index] = ortogonal_sin;
      ortogonal_calc_low[2 * new_index + 1] = ortogonal_cos;
    }
  }
  if (copy_to_low_tasks == true)
  {
    adc2_channel0_averange_low = adc2_channel0_averange_prt;
    adc2_channel1_averange_low = adc2_channel1_averange_prt;
  }
  /***/

  /*
  ---------------------------------------------------------------------------------------------------------
  150А (150 000мА) - максимальний фазний струм
  Коефіцієнст переведення в мА  - Koef_1 = 84,978173543997808495193432804655 для фазних струмів (5439/64 = 84,984375)
  
  Тоді для 150А максимально можливе значення ортогональних може бути
  150000/Koef_1 = 1765,0303364589078874793160507446
  Якщо врахувати, що сигнал є підсиленим у 16 раз, то максимальне значення ортогональних може бути
  16*150000/Koef_1 = 28240,485383342526199669056811914 < 28241(0x6E51) це є 15 бітне число (+ можливий знак)
  ---------------------------------------------------------------------------------------------------------

  ---------------------------------------------------------------------------------------------------------
  150В (150 000мВ) - максимальна фазна напруга
  Коефіцієнст переведення в мВ  - Koef_1 = 64,883134509545420915167731259667 для фазних напруг (4152/64 = 64,875)
  
  Тоді для 150В максимально можливе значення ортогональних може бути
  150000/Koef_1 = 2311,848851536795430557291797995
  Якщо врахувати, що сигнал є підсиленим у 16 раз, то максимальне значення ортогональних може бути
  16*150000/Koef_1 = 36989,581624588726888916668767919 < 36990(0x907E) це є 16 бітне число (+ можливий знак) - тобто число виходить 17-бітне
  
  Якщо з фазної напруги розраховується лінійна напруга, то, якзо припустити що вектори розврнуті у різні сторони, то  максимальне
  значення ортогональних може бути
  2*16*150000/Koef_1 = 73979,163249177453777833337535838 < 73980(0x120FC) це є 17 бітне число (+ можливий знак) - тобто число виходить 18-бітне
  ---------------------------------------------------------------------------------------------------------
  
  ---------------------------------------------------------------------------------------------------------
  2А (2 000мА * 10 = 20 000(десятих мА)) - максимальний струм 3I0
  Коефіцієнст переведення в десяті мА  - Koef_1* = 169,95634708799561699038686560931 для 3I0  для 3I0 при вираженні у десятих міліамперів (170/1 = 170)

  
  Тоді для 2А максимально можливе значення ортогональних може бути
  20000/Koef_1* = 117,67727621049018824880803941698
  Якщо врахувати, що сигнал є підсиленим у 16 раз (підсилення в 256 раз ми вже зменшили до 16), то максимальне значення ортогональних може бути
  16*20000/Koef_1* = 1882,8364193678430119809286306717 < 1883(0x075B) це є 11 бітне число (+ можливий знак)
  ---------------------------------------------------------------------------------------------------------
  */

  /***/
  //Розраховуємо діюче значення через перетворення Фур'є
  /***/
  for (enum _index_channel i = I_IA_1; i < NUMBER_ANALOG_CANALES; i++)
  {
    if (i <= I_IA_2)
    {
      unsigned int index_m = 0, index_ort = 0;
      switch (i)
      {
        case I_IA_1:
          {
            index_m = IM_IA_1;
            index_ort = FULL_ORT_IA_1;

            break;
          }
        case I_IA_2:
          {
            index_m = IM_IA_2;
            index_ort = FULL_ORT_IA_2;

            break;
          }
        default:
          {
            //Теоретично цього ніколи не мало б бути
            total_error_sw_fixed();
          }
      }

      measurement[index_m] = (MNOGNYK_I_DIJUCHE * (sqrt_32((unsigned int) (ortogonal_calc[2 * index_ort] * ortogonal_calc[2 * index_ort]) + (unsigned int) (ortogonal_calc[2 * index_ort + 1] * ortogonal_calc[2 * index_ort + 1])))) >> (VAGA_DILENNJA_I_DIJUCHE + 4);
    }
    else
    {
      unsigned int index_m = 0, index_ort = 0;
      switch (i)
      {
        case I_UAB_TN1:
          {
            index_m = IM_UAB_TN1;
            index_ort = FULL_ORT_UAB_TN1;

            break;
          }
        case I_UAB_TN2:
          {
            index_m = IM_UAB_TN2;
            index_ort = FULL_ORT_UAB_TN2;

            break;
          }
        case I_UC1C2:
          {
            index_m = IM_UC1C2;
            index_ort = FULL_ORT_UC1C2;

            break;
          }
        case I_UP1P2:
          {
            index_m = IM_UP1P2;
            index_ort = FULL_ORT_UP1P2;

            break;
          }
        case I_UP2P3:
          {
            index_m = IM_UP2P3;
            index_ort = FULL_ORT_UP2P3;

            break;
          }
        default:
          {
            //Теоретично цього ніколи не мало б бути
            total_error_sw_fixed();
          }
      }

      measurement[index_m] = (MNOGNYK_U_DIJUCHE * (sqrt_32((unsigned int) (ortogonal_calc[2 * index_ort] * ortogonal_calc[2 * index_ort]) + (unsigned int) (ortogonal_calc[2 * index_ort + 1] * ortogonal_calc[2 * index_ort + 1])))) >> (VAGA_DILENNJA_U_DIJUCHE + 3);
    }
  }

#ifdef _TEST_DURATION
  uint32_t const stop_tick = TIM2->CNT;
  uint64_t const delta_tick = (stop_tick > start_tick) ? (stop_tick - start_tick) : (0x100000000ull + stop_tick - start_tick);

  static uint64_t durMeas_L2Max = 0;
  static uint64_t durMeas_L2Min = 0x100000000ull;
  static uint32_t durMeas_L2Reset;

  if (durMeas_L2Reset != 0)
  {
    durMeas_L2Max = 0;
    durMeas_L2Min = 0x100000000ull;
    durMeas_L2Reset = 0;
  }
  durMeas_L2Cur = delta_tick;
  if (durMeas_L2Max < delta_tick)
    durMeas_L2Max = delta_tick;
  if (durMeas_L2Min > delta_tick)
    durMeas_L2Min = delta_tick;
#endif
}
/*****************************************************/

/*****************************************************/
//
/*****************************************************/
/*****************************************************/
