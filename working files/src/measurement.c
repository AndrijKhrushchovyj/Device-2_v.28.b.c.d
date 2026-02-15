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

  //  if (adc_TEST_VAL_read != 0)
  //  {
  //    adc_TEST_VAL_read = false;
  //    status_adc_read_work |= TEST_VAL_READ;
  //
  //    command_word_adc |= READ_TEST_VAL;
  //  }

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
    channel_request = (active_adc_new - 1) * NUMBER_CANALs_ADC + ((command_word >> 10) & 0xf);
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
Опрацьовуємо дані для перетворення Фур'є
 *************************************************************************/
void Fourier(void)
{
  unsigned int index_data_sin_cos_array_tmp = index_data_sin_cos_array;
  unsigned int index_data_sin2_cos2_array_tmp = index_data_sin2_cos2_array;
  unsigned int index_sin_cos_array_tmp = index_sin_cos_array;

  long long data64_new = (long long) ADCs_data[I_3I0];
  unsigned long long square_new = data64_new * data64_new;

  sum_sqr_data_3I0_irq += square_new;
  sum_sqr_data_3I0_irq -= sqr_current_data_3I0[index_array_of_one_value_fourier];
  sqr_current_data_3I0[index_array_of_one_value_fourier] = square_new;

  if ((++index_array_of_one_value_fourier) == NUMBER_POINT)
    index_array_of_one_value_fourier = 0;
  else if (index_array_of_one_value_fourier > NUMBER_POINT)
  {
    //Якщо сюди дійшла програма, значить відбулася недопустива помилка, тому треба зациклити програму, щоб вона пішла на перезагрузку
    total_error_sw_fixed();
  }

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

    if (
      (i == I_Ia) ||
      (i == I_Ic) ||
      (i == I_Ib_I04))
    {
      uint32_t i2 = 0;
      //Розрахунок другої гармоніки
      switch (i)
      {
        case I_Ia:
          {
            i2 = 0;
            break;
          }
        case I_Ib_I04:
          {
            i2 = 1;
            break;
          }
        case I_Ic:
          {
            i2 = 2;
            break;
          }
        default:
          {
            total_error_sw_fixed();
          }
      }
      unsigned int i_ort2_tmp = 2 * i2;

      //Ортогональні SIN
      ortogonal2_irq[i_ort2_tmp] -= data_sin2[index_data_sin2_cos2_array_tmp];
      temp_value_2 = (int) ((float) temp_value_1 * sin_data_f[(index_sin_cos_array_tmp << 1) & 0x1f]);
      data_sin2[index_data_sin2_cos2_array_tmp] = temp_value_2;
      ortogonal2_irq[i_ort2_tmp] += temp_value_2;

      //Ортогональні COS
      ortogonal2_irq[i_ort2_tmp + 1] -= data_cos2[index_data_sin2_cos2_array_tmp];
      temp_value_2 = (int) ((float) temp_value_1 * sin_data_f[((index_sin_cos_array_tmp << 1) + (NUMBER_POINT >> 2)) & 0x1f]);
      data_cos2[index_data_sin2_cos2_array_tmp] = temp_value_2;
      ortogonal2_irq[i_ort2_tmp + 1] += temp_value_2;

      if ((++index_data_sin2_cos2_array_tmp) == (NUMBER_POINT * NUMBER_I2G_CANALES))
        index_data_sin2_cos2_array_tmp = 0;
    }

    if ((++index_data_sin_cos_array_tmp) >= (NUMBER_POINT * NUMBER_ANALOG_CANALES))
      index_data_sin_cos_array_tmp = 0;
  }
  index_data_sin_cos_array = index_data_sin_cos_array_tmp;
  index_data_sin2_cos2_array = index_data_sin2_cos2_array_tmp;

  if ((++index_sin_cos_array_tmp) >= NUMBER_POINT)
    index_sin_cos_array_tmp = 0;
  index_sin_cos_array = index_sin_cos_array_tmp;

  //Копіювання для інших систем
  unsigned int bank_ortogonal_tmp = bank_ortogonal;
  for (unsigned int i = 0; i < (2 * NUMBER_ANALOG_CANALES); i++)
    ortogonal[i][bank_ortogonal_tmp] = ortogonal_irq[i];
  for (unsigned int i = 0; i < (2 * NUMBER_I2G_CANALES); i++)
    ortogonal2[i][bank_ortogonal_tmp] = ortogonal2_irq[i];
  sum_sqr_data_3I0[bank_ortogonal_tmp] = sum_sqr_data_3I0_irq;
}
/*************************************************************************/

/*************************************************************************
Детектор частоти для каналів групи 1
*************************************************************************/
//#pragma optimize=none
void fapch(void)
{
  unsigned int bank_measurement_high_tmp = bank_measurement_high;
  unsigned int canal_phase_line = current_settings.control_extra_settings_1 & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE);
  int index_1 = -1;
  unsigned int maska_canaliv_fapch_tmp = 0;

  /*****
  Шучаємо сигнал по якому будемо розраховувати частоту
  *****/
  if (
    (
      (canal_phase_line == 0) &&
      (measurement_high[bank_measurement_high_tmp][IM_UA] >= PORIG_FOR_FAPCH)) ||
    ((canal_phase_line != 0) &&
     (measurement_high[bank_measurement_high_tmp][IM_UAB] >= PORIG_FOR_FAPCH)))
  {
    index_1 = INDEX_PhK_UA;
    maska_canaliv_fapch_tmp = READ_Ua;
  }
  else if (
    (
      (canal_phase_line == 0) &&
      (measurement_high[bank_measurement_high_tmp][IM_UB] >= PORIG_FOR_FAPCH)) ||
    ((canal_phase_line != 0) &&
     (measurement_high[bank_measurement_high_tmp][IM_UBC] >= PORIG_FOR_FAPCH)))
  {
    index_1 = INDEX_PhK_UB;
    maska_canaliv_fapch_tmp = READ_Ub;
  }
  else if (
    (
      (canal_phase_line == 0) &&
      (measurement_high[bank_measurement_high_tmp][IM_UC] >= PORIG_FOR_FAPCH)) ||
    ((canal_phase_line != 0) &&
     (measurement_high[bank_measurement_high_tmp][IM_UCA] >= PORIG_FOR_FAPCH)))
  {
    index_1 = INDEX_PhK_UC;
    maska_canaliv_fapch_tmp = READ_Uc;
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
    unsigned int shift = ((GPIO_SELECT_ADC->ODR & GPIO_SELECTPin_ADC) == 0) ? 0 : NUMBER_CANALs_ADC;
    unsigned int number_canal = shift + ((read_value >> 12) & 0xf);

    output_adc[number_canal].tick = tick_output_adc_p;

    static uint32_t error_spi_adc;
    if (channel_answer != number_canal)
    {
      if (error_spi_adc < 3)
        error_spi_adc++;
      if (error_spi_adc >= 3)
        _SET_BIT(set_diagnostyka, ERROR_SPI_ADC_BIT);
    }
    else
    {
      error_spi_adc = 0;

      _SET_BIT(clear_diagnostyka, ERROR_SPI_ADC_BIT);
      output_adc[number_canal].value = read_value & 0xfff;
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
      command_word |= (1 << I_3I0) |
                      (1 << I_Ia) | (1 << I_Ib_I04) | (1 << I_Ic) |
                      (1 << I_Ua) | (1 << I_Ub) | (1 << I_Uc) |
                      (1 << I_3U0);
    }

    uint32_t _x1, _x2, _DX, _dx;
    int _y1, _y2;
    long long _y;

    //    unsigned int gnd_adc  = gnd_adc1;
    //    unsigned int vref_adc/* = vref_adc1*/;

    uint32_t _x = previous_tick_DATA_VAL;
    /*****/
    //Формуємо значення 3I0
    /*****/
    if ((command_word & (1 << I_3I0)) != 0)
    {
      _x1 = ADCs_data_raw[I_3I0].tick;
      _y1 = ADCs_data_raw[I_3I0].value;

      static uint32_t index_array_of_one_value_3I0;

      uint32_t val_C_3I0_16 = output_adc[C_3I0_16].value;
      vref_adc_averange_sum[I_3I0] += val_C_3I0_16;

      if ((++index_array_of_one_value_3I0) == NUMBER_POINT)
      {
        index_array_of_one_value_3I0 = 0;
        uint32_t vref_adc_period = vref_adc_averange_sum[I_3I0] >> VAGA_NUMBER_POINT;
        vref_adc_averange_sum[I_3I0] = 0;

        //Робимо тепер усереднення за секунду
        static uint32_t index_array_of_one_value_3I0_1s;

        vref_adc_averange_sum_1s[I_3I0] += vref_adc_period;
        vref_adc_averange_sum_1s[I_3I0] -= vref_adc_moment_value_1s[I_3I0][index_array_of_one_value_3I0_1s];
        vref_adc_moment_value_1s[I_3I0][index_array_of_one_value_3I0_1s] = vref_adc_period;
        vref_adc[I_3I0] = vref_adc_averange_sum_1s[I_3I0] / MAIN_FREQUENCY;

        if ((++index_array_of_one_value_3I0_1s) == MAIN_FREQUENCY)
          index_array_of_one_value_3I0_1s = 0;
        else if (index_array_of_one_value_3I0_1s > MAIN_FREQUENCY)
        {
          //Якщо сюди дійшла програма, значить відбулася недопустива помилка, тому треба зациклити програму, щоб вона пішла на перезагрузку
          total_error_sw_fixed();
        }
      }
      else if (index_array_of_one_value_3I0 > NUMBER_POINT)
      {
        //Якщо сюди дійшла програма, значить відбулася недопустива помилка, тому треба зациклити програму, щоб вона пішла на перезагрузку
        total_error_sw_fixed();
      }

      //      _y2 = output_adc[C_3I0_1].value - gnd_adc - vref_adc;
      //      if (abs(_y2) > 87)
      //      {
      //        _x2 = output_adc[C_3I0_1].tick;
      //        _y2 = (int)(_y2*ustuvannja_meas[I_3I0])>>(USTUVANNJA_VAGA - 2*4);
      //      }
      //      else
      //      {
      _y2 = val_C_3I0_16 - /*gnd_adc - */ vref_adc[I_3I0];
      if (abs(_y2) > 87)
      {
        _x2 = output_adc[C_3I0_16].tick;
        _y2 = (int) (_y2 * ustuvannja_meas[I_3I0]) >> (USTUVANNJA_VAGA - 4);
      }
      else
      {
        _y2 = output_adc[C_3I0_256].value - /*gnd_adc - */ vref_adc[I_3I0];

        _x2 = output_adc[C_3I0_256].tick;
        _y2 = (int) ((-_y2) * ustuvannja_meas[I_3I0]) >> (USTUVANNJA_VAGA);
      }
      //      }

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

      ADCs_data[I_3I0] = _y;

      ADCs_data_raw[I_3I0].tick = _x2;
      ADCs_data_raw[I_3I0].value = _y2;
    }
    /*****/

    /*****/
    //Формуємо значення Ia
    /*****/
    if ((command_word & (1 << I_Ia)) != 0)
    {
      _x1 = ADCs_data_raw[I_Ia].tick;
      _y1 = ADCs_data_raw[I_Ia].value;

      static uint32_t index_array_of_one_value_Ia;

      uint32_t val_C_Ia_1 = output_adc[C_Ia_1].value;
      vref_adc_averange_sum[I_Ia] += val_C_Ia_1;

      if ((++index_array_of_one_value_Ia) == NUMBER_POINT)
      {
        index_array_of_one_value_Ia = 0;
        uint32_t vref_adc_period = vref_adc_averange_sum[I_Ia] >> VAGA_NUMBER_POINT;
        vref_adc_averange_sum[I_Ia] = 0;

        //Робимо тепер усереднення за секунду
        static uint32_t index_array_of_one_value_Ia_1s;

        vref_adc_averange_sum_1s[I_Ia] += vref_adc_period;
        vref_adc_averange_sum_1s[I_Ia] -= vref_adc_moment_value_1s[I_Ia][index_array_of_one_value_Ia_1s];
        vref_adc_moment_value_1s[I_Ia][index_array_of_one_value_Ia_1s] = vref_adc_period;
        vref_adc[I_Ia] = vref_adc_averange_sum_1s[I_Ia] / MAIN_FREQUENCY;

        if ((++index_array_of_one_value_Ia_1s) == MAIN_FREQUENCY)
          index_array_of_one_value_Ia_1s = 0;
        else if (index_array_of_one_value_Ia_1s > MAIN_FREQUENCY)
        {
          //Якщо сюди дійшла програма, значить відбулася недопустива помилка, тому треба зациклити програму, щоб вона пішла на перезагрузку
          total_error_sw_fixed();
        }
      }
      else if (index_array_of_one_value_Ia > NUMBER_POINT)
      {
        //Якщо сюди дійшла програма, значить відбулася недопустива помилка, тому треба зациклити програму, щоб вона пішла на перезагрузку
        total_error_sw_fixed();
      }

      _y2 = val_C_Ia_1 - /*gnd_adc - */ vref_adc[I_Ia];
      if (abs(_y2) > 87)
      {
        _x2 = output_adc[C_Ia_1].tick;
        _y2 = (int) (_y2 * ustuvannja_meas[I_Ia]) >> (USTUVANNJA_VAGA - 4);
      }
      else
      {
        _y2 = output_adc[C_Ia_16].value - /*gnd_adc - */ vref_adc[I_Ia];

        _x2 = output_adc[C_Ia_16].tick;
        _y2 = (int) ((-_y2) * ustuvannja_meas[I_Ia]) >> (USTUVANNJA_VAGA);
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

      ADCs_data[I_Ia] = _y;

      ADCs_data_raw[I_Ia].tick = _x2;
      ADCs_data_raw[I_Ia].value = _y2;
    }
    /*****/

    /*****/
    //Формуємо значення Ib/I0.4
    /*****/
    if ((command_word & (1 << I_Ib_I04)) != 0)
    {
      _x1 = ADCs_data_raw[I_Ib_I04].tick;
      _y1 = ADCs_data_raw[I_Ib_I04].value;

      static uint32_t index_array_of_one_value_Ib_I04;

      uint32_t val_C_Ib_I04_1 = output_adc[C_Ib_1].value;
      vref_adc_averange_sum[I_Ib_I04] += val_C_Ib_I04_1;

      if ((++index_array_of_one_value_Ib_I04) == NUMBER_POINT)
      {
        index_array_of_one_value_Ib_I04 = 0;
        uint32_t vref_adc_period = vref_adc_averange_sum[I_Ib_I04] >> VAGA_NUMBER_POINT;
        vref_adc_averange_sum[I_Ib_I04] = 0;

        //Робимо тепер усереднення за секунду
        static uint32_t index_array_of_one_value_Ib_I04_1s;

        vref_adc_averange_sum_1s[I_Ib_I04] += vref_adc_period;
        vref_adc_averange_sum_1s[I_Ib_I04] -= vref_adc_moment_value_1s[I_Ib_I04][index_array_of_one_value_Ib_I04_1s];
        vref_adc_moment_value_1s[I_Ib_I04][index_array_of_one_value_Ib_I04_1s] = vref_adc_period;
        vref_adc[I_Ib_I04] = vref_adc_averange_sum_1s[I_Ib_I04] / MAIN_FREQUENCY;

        if ((++index_array_of_one_value_Ib_I04_1s) == MAIN_FREQUENCY)
          index_array_of_one_value_Ib_I04_1s = 0;
        else if (index_array_of_one_value_Ib_I04_1s > MAIN_FREQUENCY)
        {
          //Якщо сюди дійшла програма, значить відбулася недопустива помилка, тому треба зациклити програму, щоб вона пішла на перезагрузку
          total_error_sw_fixed();
        }
      }
      else if (index_array_of_one_value_Ib_I04 > NUMBER_POINT)
      {
        //Якщо сюди дійшла програма, значить відбулася недопустива помилка, тому треба зациклити програму, щоб вона пішла на перезагрузку
        total_error_sw_fixed();
      }

      _y2 = val_C_Ib_I04_1 - /*gnd_adc - */ vref_adc[I_Ib_I04];
      if (abs(_y2) > 87)
      {
        _x2 = output_adc[C_Ib_1].tick;
        _y2 = (int) (_y2 * ustuvannja_meas[I_Ib_I04]) >> (USTUVANNJA_VAGA - 4);
      }
      else
      {
        _y2 = output_adc[C_Ib_16].value - /*gnd_adc - */ vref_adc[I_Ib_I04];

        _x2 = output_adc[C_Ib_16].tick;
        _y2 = (int) ((-_y2) * ustuvannja_meas[I_Ib_I04]) >> (USTUVANNJA_VAGA);
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

      ADCs_data[I_Ib_I04] = _y;

      ADCs_data_raw[I_Ib_I04].tick = _x2;
      ADCs_data_raw[I_Ib_I04].value = _y2;
    }
    /*****/

    /*****/
    //Формуємо значення Ic
    /*****/
    if ((command_word & (1 << I_Ic)) != 0)
    {
      _x1 = ADCs_data_raw[I_Ic].tick;
      _y1 = ADCs_data_raw[I_Ic].value;

      static uint32_t index_array_of_one_value_Ic;

      uint32_t val_C_Ic_1 = output_adc[C_Ic_1].value;
      vref_adc_averange_sum[I_Ic] += val_C_Ic_1;

      if ((++index_array_of_one_value_Ic) == NUMBER_POINT)
      {
        index_array_of_one_value_Ic = 0;
        uint32_t vref_adc_period = vref_adc_averange_sum[I_Ic] >> VAGA_NUMBER_POINT;
        vref_adc_averange_sum[I_Ic] = 0;

        //Робимо тепер усереднення за секунду
        static uint32_t index_array_of_one_value_Ic_1s;

        vref_adc_averange_sum_1s[I_Ic] += vref_adc_period;
        vref_adc_averange_sum_1s[I_Ic] -= vref_adc_moment_value_1s[I_Ic][index_array_of_one_value_Ic_1s];
        vref_adc_moment_value_1s[I_Ic][index_array_of_one_value_Ic_1s] = vref_adc_period;
        vref_adc[I_Ic] = vref_adc_averange_sum_1s[I_Ic] / MAIN_FREQUENCY;

        if ((++index_array_of_one_value_Ic_1s) == MAIN_FREQUENCY)
          index_array_of_one_value_Ic_1s = 0;
        else if (index_array_of_one_value_Ic_1s > MAIN_FREQUENCY)
        {
          //Якщо сюди дійшла програма, значить відбулася недопустива помилка, тому треба зациклити програму, щоб вона пішла на перезагрузку
          total_error_sw_fixed();
        }
      }
      else if (index_array_of_one_value_Ic > NUMBER_POINT)
      {
        //Якщо сюди дійшла програма, значить відбулася недопустива помилка, тому треба зациклити програму, щоб вона пішла на перезагрузку
        total_error_sw_fixed();
      }

      _y2 = val_C_Ic_1 - /*gnd_adc - */ vref_adc[I_Ic];
      if (abs(_y2) > 87)
      {
        _x2 = output_adc[C_Ic_1].tick;
        _y2 = (int) (_y2 * ustuvannja_meas[I_Ic]) >> (USTUVANNJA_VAGA - 4);
      }
      else
      {
        _y2 = output_adc[C_Ic_16].value - /*gnd_adc - */ vref_adc[I_Ic];

        _x2 = output_adc[C_Ic_16].tick;
        _y2 = (int) ((-_y2) * ustuvannja_meas[I_Ic]) >> (USTUVANNJA_VAGA);
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

      ADCs_data[I_Ic] = _y;

      ADCs_data_raw[I_Ic].tick = _x2;
      ADCs_data_raw[I_Ic].value = _y2;
    }
    /*****/

    //    gnd_adc  = gnd_adc2;
    //    vref_adc = vref_adc2;

    /*****/
    //Формуємо значення 3U0
    /*****/
    if ((command_word & (1 << I_3U0)) != 0)
    {
      _x1 = ADCs_data_raw[I_3U0].tick;
      _y1 = ADCs_data_raw[I_3U0].value;

      static uint32_t index_array_of_one_value_3U0;

      uint32_t val_C_3U0_1 = output_adc[C_3U0_1].value;
      vref_adc_averange_sum[I_3U0] += val_C_3U0_1;

      if ((++index_array_of_one_value_3U0) == NUMBER_POINT)
      {
        index_array_of_one_value_3U0 = 0;
        uint32_t vref_adc_period = vref_adc_averange_sum[I_3U0] >> VAGA_NUMBER_POINT;
        vref_adc_averange_sum[I_3U0] = 0;

        //Робимо тепер усереднення за секунду
        static uint32_t index_array_of_one_value_3U0_1s;

        vref_adc_averange_sum_1s[I_3U0] += vref_adc_period;
        vref_adc_averange_sum_1s[I_3U0] -= vref_adc_moment_value_1s[I_3U0][index_array_of_one_value_3U0_1s];
        vref_adc_moment_value_1s[I_3U0][index_array_of_one_value_3U0_1s] = vref_adc_period;
        vref_adc[I_3U0] = vref_adc_averange_sum_1s[I_3U0] / MAIN_FREQUENCY;

        if ((++index_array_of_one_value_3U0_1s) == MAIN_FREQUENCY)
          index_array_of_one_value_3U0_1s = 0;
        else if (index_array_of_one_value_3U0_1s > MAIN_FREQUENCY)
        {
          //Якщо сюди дійшла програма, значить відбулася недопустива помилка, тому треба зациклити програму, щоб вона пішла на перезагрузку
          total_error_sw_fixed();
        }
      }
      else if (index_array_of_one_value_3U0 > NUMBER_POINT)
      {
        //Якщо сюди дійшла програма, значить відбулася недопустива помилка, тому треба зациклити програму, щоб вона пішла на перезагрузку
        total_error_sw_fixed();
      }

      _y2 = val_C_3U0_1 - /*gnd_adc - */ vref_adc[I_3U0];
      if (abs(_y2) > 87)
      {
        _x2 = output_adc[C_3U0_1].tick;
        _y2 = (int) (_y2 * ustuvannja_meas[I_3U0]) >> (USTUVANNJA_VAGA - 4);
      }
      else
      {
        _y2 = output_adc[C_3U0_16].value - /*gnd_adc - */ vref_adc[I_3U0];

        _x2 = output_adc[C_3U0_16].tick;
        _y2 = (int) ((-_y2) * ustuvannja_meas[I_3U0]) >> (USTUVANNJA_VAGA);
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

      ADCs_data[I_3U0] = _y;

      ADCs_data_raw[I_3U0].tick = _x2;
      ADCs_data_raw[I_3U0].value = _y2;
    }
    /*****/

    /*****/
    //Формуємо значення Ua
    /*****/
    if ((command_word & (1 << I_Ua)) != 0)
    {
      _x1 = ADCs_data_raw[I_Ua].tick;
      _y1 = ADCs_data_raw[I_Ua].value;

      static uint32_t index_array_of_one_value_Ua;

      uint32_t val_C_Ua_1 = output_adc[C_Ua_1].value;
      vref_adc_averange_sum[I_Ua] += val_C_Ua_1;

      if ((++index_array_of_one_value_Ua) == NUMBER_POINT)
      {
        index_array_of_one_value_Ua = 0;
        uint32_t vref_adc_period = vref_adc_averange_sum[I_Ua] >> VAGA_NUMBER_POINT;
        vref_adc_averange_sum[I_Ua] = 0;

        //Робимо тепер усереднення за секунду
        static uint32_t index_array_of_one_value_Ua_1s;

        vref_adc_averange_sum_1s[I_Ua] += vref_adc_period;
        vref_adc_averange_sum_1s[I_Ua] -= vref_adc_moment_value_1s[I_Ua][index_array_of_one_value_Ua_1s];
        vref_adc_moment_value_1s[I_Ua][index_array_of_one_value_Ua_1s] = vref_adc_period;
        vref_adc[I_Ua] = vref_adc_averange_sum_1s[I_Ua] / MAIN_FREQUENCY;

        if ((++index_array_of_one_value_Ua_1s) == MAIN_FREQUENCY)
          index_array_of_one_value_Ua_1s = 0;
        else if (index_array_of_one_value_Ua_1s > MAIN_FREQUENCY)
        {
          //Якщо сюди дійшла програма, значить відбулася недопустива помилка, тому треба зациклити програму, щоб вона пішла на перезагрузку
          total_error_sw_fixed();
        }
      }
      else if (index_array_of_one_value_Ua > NUMBER_POINT)
      {
        //Якщо сюди дійшла програма, значить відбулася недопустива помилка, тому треба зациклити програму, щоб вона пішла на перезагрузку
        total_error_sw_fixed();
      }

      _y2 = val_C_Ua_1 - /*gnd_adc - */ vref_adc[I_Ua];
      if (abs(_y2) > 87)
      {
        _x2 = output_adc[C_Ua_1].tick;
        _y2 = (int) (_y2 * ustuvannja_meas[I_Ua]) >> (USTUVANNJA_VAGA - 4);
      }
      else
      {
        _y2 = output_adc[C_Ua_16].value - /*gnd_adc - */ vref_adc[I_Ua];

        _x2 = output_adc[C_Ua_16].tick;
        _y2 = (int) ((-_y2) * ustuvannja_meas[I_Ua]) >> (USTUVANNJA_VAGA);
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

      if ((_y >= 0) && (ADCs_data[I_Ua] < 0))
      {
        /*
        Зафіксований перехід через нуль
        */
        //Попередній перехід
        perechid_cherez_nul[INDEX_PhK_UA][0].x1 = poperednij_perechid.Ua_x1;
        perechid_cherez_nul[INDEX_PhK_UA][0].y1 = poperednij_perechid.Ua_y1;
        perechid_cherez_nul[INDEX_PhK_UA][0].x2 = poperednij_perechid.Ua_x2;
        perechid_cherez_nul[INDEX_PhK_UA][0].y2 = poperednij_perechid.Ua_y2;

        //Поточний перехід
        poperednij_perechid.Ua_x1 = perechid_cherez_nul[INDEX_PhK_UA][1].x1 = penultimate_tick_DATA_VAL;
        poperednij_perechid.Ua_y1 = perechid_cherez_nul[INDEX_PhK_UA][1].y1 = ADCs_data[I_Ua];
        poperednij_perechid.Ua_x2 = perechid_cherez_nul[INDEX_PhK_UA][1].x2 = _x;
        poperednij_perechid.Ua_y2 = perechid_cherez_nul[INDEX_PhK_UA][1].y2 = _y;

        //Помічаємо, що перехід зафіксований
        fix_perechid_cherez_nul[INDEX_PhK_UA] = 0xff;
      }
      ADCs_data[I_Ua] = _y;

      ADCs_data_raw[I_Ua].tick = _x2;
      ADCs_data_raw[I_Ua].value = _y2;
    }
    /*****/

    /*****/
    //Формуємо значення Ub
    /*****/
    if ((command_word & (1 << I_Ub)) != 0)
    {
      _x1 = ADCs_data_raw[I_Ub].tick;
      _y1 = ADCs_data_raw[I_Ub].value;

      static uint32_t index_array_of_one_value_Ub;

      uint32_t val_C_Ub_1 = output_adc[C_Ub_1].value;
      vref_adc_averange_sum[I_Ub] += val_C_Ub_1;

      if ((++index_array_of_one_value_Ub) == NUMBER_POINT)
      {
        index_array_of_one_value_Ub = 0;
        uint32_t vref_adc_period = vref_adc_averange_sum[I_Ub] >> VAGA_NUMBER_POINT;
        vref_adc_averange_sum[I_Ub] = 0;

        //Робимо тепер усереднення за секунду
        static uint32_t index_array_of_one_value_Ub_1s;

        vref_adc_averange_sum_1s[I_Ub] += vref_adc_period;
        vref_adc_averange_sum_1s[I_Ub] -= vref_adc_moment_value_1s[I_Ub][index_array_of_one_value_Ub_1s];
        vref_adc_moment_value_1s[I_Ub][index_array_of_one_value_Ub_1s] = vref_adc_period;
        vref_adc[I_Ub] = vref_adc_averange_sum_1s[I_Ub] / MAIN_FREQUENCY;

        if ((++index_array_of_one_value_Ub_1s) == MAIN_FREQUENCY)
          index_array_of_one_value_Ub_1s = 0;
        else if (index_array_of_one_value_Ub_1s > MAIN_FREQUENCY)
        {
          //Якщо сюди дійшла програма, значить відбулася недопустива помилка, тому треба зациклити програму, щоб вона пішла на перезагрузку
          total_error_sw_fixed();
        }
      }
      else if (index_array_of_one_value_Ub > NUMBER_POINT)
      {
        //Якщо сюди дійшла програма, значить відбулася недопустива помилка, тому треба зациклити програму, щоб вона пішла на перезагрузку
        total_error_sw_fixed();
      }

      _y2 = val_C_Ub_1 - /*gnd_adc - */ vref_adc[I_Ub];
      if (abs(_y2) > 87)
      {
        _x2 = output_adc[C_Ub_1].tick;
        _y2 = (int) (_y2 * ustuvannja_meas[I_Ub]) >> (USTUVANNJA_VAGA - 4);
      }
      else
      {
        _y2 = output_adc[C_Ub_16].value - /*gnd_adc - */ vref_adc[I_Ub];

        _x2 = output_adc[C_Ub_16].tick;
        _y2 = (int) ((-_y2) * ustuvannja_meas[I_Ub]) >> (USTUVANNJA_VAGA);
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

      if ((_y >= 0) && (ADCs_data[I_Ub] < 0))
      {
        /*
        Зафіксований перехід через нуль
        */
        //Попередній перехід
        perechid_cherez_nul[INDEX_PhK_UB][0].x1 = poperednij_perechid.Ub_x1;
        perechid_cherez_nul[INDEX_PhK_UB][0].y1 = poperednij_perechid.Ub_y1;
        perechid_cherez_nul[INDEX_PhK_UB][0].x2 = poperednij_perechid.Ub_x2;
        perechid_cherez_nul[INDEX_PhK_UB][0].y2 = poperednij_perechid.Ub_y2;

        //Поточний перехід
        poperednij_perechid.Ub_x1 = perechid_cherez_nul[INDEX_PhK_UB][1].x1 = penultimate_tick_DATA_VAL;
        poperednij_perechid.Ub_y1 = perechid_cherez_nul[INDEX_PhK_UB][1].y1 = ADCs_data[I_Ub];
        poperednij_perechid.Ub_x2 = perechid_cherez_nul[INDEX_PhK_UB][1].x2 = _x;
        poperednij_perechid.Ub_y2 = perechid_cherez_nul[INDEX_PhK_UB][1].y2 = _y;

        //Помічаємо, що перехід зафіксований
        fix_perechid_cherez_nul[INDEX_PhK_UB] = 0xff;
      }
      ADCs_data[I_Ub] = _y;

      ADCs_data_raw[I_Ub].tick = _x2;
      ADCs_data_raw[I_Ub].value = _y2;
    }
    /*****/

    /*****/
    //Формуємо значення Uc
    /*****/
    if ((command_word & (1 << I_Uc)) != 0)
    {
      _x1 = ADCs_data_raw[I_Uc].tick;
      _y1 = ADCs_data_raw[I_Uc].value;

      static uint32_t index_array_of_one_value_Uc;

      uint32_t val_C_Uc_1 = output_adc[C_Uc_1].value;
      vref_adc_averange_sum[I_Uc] += val_C_Uc_1;

      if ((++index_array_of_one_value_Uc) == NUMBER_POINT)
      {
        index_array_of_one_value_Uc = 0;
        uint32_t vref_adc_period = vref_adc_averange_sum[I_Uc] >> VAGA_NUMBER_POINT;
        vref_adc_averange_sum[I_Uc] = 0;

        //Робимо тепер усереднення за секунду
        static uint32_t index_array_of_one_value_Uc_1s;

        vref_adc_averange_sum_1s[I_Uc] += vref_adc_period;
        vref_adc_averange_sum_1s[I_Uc] -= vref_adc_moment_value_1s[I_Uc][index_array_of_one_value_Uc_1s];
        vref_adc_moment_value_1s[I_Uc][index_array_of_one_value_Uc_1s] = vref_adc_period;
        vref_adc[I_Uc] = vref_adc_averange_sum_1s[I_Uc] / MAIN_FREQUENCY;

        if ((++index_array_of_one_value_Uc_1s) == MAIN_FREQUENCY)
          index_array_of_one_value_Uc_1s = 0;
        else if (index_array_of_one_value_Uc_1s > MAIN_FREQUENCY)
        {
          //Якщо сюди дійшла програма, значить відбулася недопустива помилка, тому треба зациклити програму, щоб вона пішла на перезагрузку
          total_error_sw_fixed();
        }
      }
      else if (index_array_of_one_value_Uc > NUMBER_POINT)
      {
        //Якщо сюди дійшла програма, значить відбулася недопустива помилка, тому треба зациклити програму, щоб вона пішла на перезагрузку
        total_error_sw_fixed();
      }

      _y2 = val_C_Uc_1 - /*gnd_adc - */ vref_adc[I_Uc];
      if (abs(_y2) > 87)
      {
        _x2 = output_adc[C_Uc_1].tick;
        _y2 = (int) (_y2 * ustuvannja_meas[I_Uc]) >> (USTUVANNJA_VAGA - 4);
      }
      else
      {
        _y2 = output_adc[C_Uc_16].value - /*gnd_adc - */ vref_adc[I_Uc];

        _x2 = output_adc[C_Uc_16].tick;
        _y2 = (int) ((-_y2) * ustuvannja_meas[I_Uc]) >> (USTUVANNJA_VAGA);
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

      if ((_y >= 0) && (ADCs_data[I_Uc] < 0))
      {
        /*
        Зафіксований перехід через нуль
        */
        //Попередній перехід
        perechid_cherez_nul[INDEX_PhK_UC][0].x1 = poperednij_perechid.Uc_x1;
        perechid_cherez_nul[INDEX_PhK_UC][0].y1 = poperednij_perechid.Uc_y1;
        perechid_cherez_nul[INDEX_PhK_UC][0].x2 = poperednij_perechid.Uc_x2;
        perechid_cherez_nul[INDEX_PhK_UC][0].y2 = poperednij_perechid.Uc_y2;

        //Поточний перехід
        poperednij_perechid.Uc_x1 = perechid_cherez_nul[INDEX_PhK_UC][1].x1 = penultimate_tick_DATA_VAL;
        poperednij_perechid.Uc_y1 = perechid_cherez_nul[INDEX_PhK_UC][1].y1 = ADCs_data[I_Uc];
        poperednij_perechid.Uc_x2 = perechid_cherez_nul[INDEX_PhK_UC][1].x2 = _x;
        poperednij_perechid.Uc_y2 = perechid_cherez_nul[INDEX_PhK_UC][1].y2 = _y;

        //Помічаємо, що перехід зафіксований
        fix_perechid_cherez_nul[INDEX_PhK_UC] = 0xff;
      }
      ADCs_data[I_Uc] = _y;

      ADCs_data_raw[I_Uc].tick = _x2;
      ADCs_data_raw[I_Uc].value = _y2;
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
            data_for_oscylograph[DATA_VAL_tail_data_for_oscylograph_tmp].data[I_3I0 + i] = y;
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

    //    if ((status_adc_read_work & TEST_VAL_READ) != 0)
    {
      //Виділяємо мінімальне і максимальне значення опори по всіх каналах
      uint32_t min_vref_adc = vref_adc[0];
      uint32_t max_vref_adc = min_vref_adc;

      for (size_t i = 1; i < NUMBER_ANALOG_CANALES; i++)
      {
        if (min_vref_adc > vref_adc[i])
          min_vref_adc = vref_adc[i];
        if (max_vref_adc < vref_adc[i])
          max_vref_adc = vref_adc[i];
      }

      //Перевіряємо допустимість діапазону
      if ((min_vref_adc < 0x709) || (max_vref_adc > 0x8f5))
        _SET_BIT(set_diagnostyka, ERROR_VREF_ADC_TEST_BIT);
      else
        _SET_BIT(clear_diagnostyka, ERROR_VREF_ADC_TEST_BIT);

      //      status_adc_read_work &= (unsigned int)(~TEST_VAL_READ);

      //      /**************************************************/
      //      //Виставляємо повідомлення про завершення тестових величин
      //      /**************************************************/
      //      control_word_of_watchdog |= WATCHDOG_MEASURE_STOP_TEST_VAL;
      //      /**************************************************/

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
//Визначенням кутів
/*****************************************************/
void calc_angle(void)
{
  //Копіюємо вимірювання
  semaphore_measure_values_low1 = 1;
  for (unsigned int i = 0; i < _NUMBER_IM; i++)
  {
    measurement_low[i] = measurement_middle[i];
  }
  semaphore_measure_values_low1 = 0;

  int ortogonal_low_tmp[2 * FULL_ORT_MAX];

  //Виставляємо семафор заборони обновлення значень з системи захистів
  semaphore_measure_values_low = 1;
  for (unsigned int i = 0; i < (2 * FULL_ORT_MAX); i++)
  {
    ortogonal_low_tmp[i] = ortogonal_calc_low[i];
  }
  //Знімаємо семафор заборони обновлення значень з системи захистів
  semaphore_measure_values_low = 0;

  state_calc_phi_angle = true;
  bank_for_calc_phi_angle = (bank_for_calc_phi_angle ^ 0x1) & 0x1;

  //Визначаємо, який вектор беремо за осному
  __full_ort_index index_base;
  if ((current_settings.control_extra_settings_1 & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE)) == 0)
    index_base = FULL_ORT_Ua;
  else
  {
    index_base = FULL_ORT_Uab;

    //У цьому випадку кути між фазними напругами невизначкені
    phi_angle[bank_for_calc_phi_angle][FULL_ORT_Uc] = phi_angle[bank_for_calc_phi_angle][FULL_ORT_Ub] = phi_angle[bank_for_calc_phi_angle][FULL_ORT_Ua] = -1;
  }

  /***
  Визначаємо, який останній вектор можна брати за основу
  ***/
  __full_ort_index index_last = FULL_ORT_3U0;
  /***/

  int base_index_for_angle_tmp = -1;
  __full_ort_index index = index_base;
  while (index <= index_last)
  {
    unsigned int index_m = 0;
    switch (index)
    {
      case FULL_ORT_Ua:
        {
          index_m = IM_UA;
          break;
        }
      case FULL_ORT_Ub:
        {
          index_m = IM_UB;
          break;
        }
      case FULL_ORT_Uc:
        {
          index_m = IM_UC;
          break;
        }
      case FULL_ORT_Uab:
        {
          index_m = IM_UAB;
          break;
        }
      case FULL_ORT_Ubc:
        {
          index_m = IM_UBC;
          break;
        }
      case FULL_ORT_Uca:
        {
          index_m = IM_UCA;
          break;
        }
      case FULL_ORT_3U0:
        {
          index_m = IM_3U0;
          break;
        }
      default:
        {
          //Теоретично цього ніколи не мало б бути
          total_error_sw_fixed();
        }
    }

    if (measurement_low[index_m] >= PORIG_CHUTLYVOSTI_VOLTAGE_ANGLE)
    {
      base_index_for_angle_tmp = index;
      break; //Вихід із циклу while
    }
    index++;
  }
  base_index_for_angle = base_index_for_angle_tmp;

  if (base_index_for_angle_tmp >= 0)
  {
    //Координати базового вектору, відносно якого будемо шукати кути
    int ortogonal_base[2] = {ortogonal_low_tmp[2 * base_index_for_angle_tmp], ortogonal_low_tmp[2 * base_index_for_angle_tmp + 1]};

#define SIN_BASE ortogonal_base[0]
#define COS_BASE ortogonal_base[1]

    unsigned int amplituda_base = sqrt_64((unsigned long long) ((long long) SIN_BASE * (long long) SIN_BASE) + (unsigned long long) ((long long) COS_BASE * (long long) COS_BASE));
    if (amplituda_base != 0)
    {
      for (__full_ort_index index_tmp = index_base; index_tmp < FULL_ORT_MAX; index_tmp++)
      {
        if (index_tmp == index)
        {
          phi_angle[bank_for_calc_phi_angle][index_tmp] = 0;
          continue;
        }
        else
        {
          unsigned int porig_chutlyvosti = 0;
          unsigned int index_m = 0;
          switch (index_tmp)
          {
            case FULL_ORT_Ua:
              {
                porig_chutlyvosti = PORIG_CHUTLYVOSTI_VOLTAGE_ANGLE;
                index_m = IM_UA;
                break;
              }
            case FULL_ORT_Ub:
              {
                porig_chutlyvosti = PORIG_CHUTLYVOSTI_VOLTAGE_ANGLE;
                index_m = IM_UB;
                break;
              }
            case FULL_ORT_Uc:
              {
                porig_chutlyvosti = PORIG_CHUTLYVOSTI_VOLTAGE_ANGLE;
                index_m = IM_UC;
                break;
              }
            case FULL_ORT_Uab:
              {
                porig_chutlyvosti = PORIG_CHUTLYVOSTI_VOLTAGE_ANGLE;
                index_m = IM_UAB;
                break;
              }
            case FULL_ORT_Ubc:
              {
                porig_chutlyvosti = PORIG_CHUTLYVOSTI_VOLTAGE_ANGLE;
                index_m = IM_UBC;
                break;
              }
            case FULL_ORT_Uca:
              {
                porig_chutlyvosti = PORIG_CHUTLYVOSTI_VOLTAGE_ANGLE;
                index_m = IM_UCA;
                break;
              }
            case FULL_ORT_3U0:
              {
                porig_chutlyvosti = PORIG_CHUTLYVOSTI_VOLTAGE_ANGLE;
                index_m = IM_3U0;
                break;
              }
            case FULL_ORT_Ia:
              {
                porig_chutlyvosti = PORIG_CHUTLYVOSTI_CURRENT;
                index_m = IM_IA;
                break;
              }
            case FULL_ORT_Ib:
              {
                porig_chutlyvosti = PORIG_CHUTLYVOSTI_CURRENT;
                index_m = IM_IB;
                break;
              }
            case FULL_ORT_Ic:
              {
                porig_chutlyvosti = PORIG_CHUTLYVOSTI_CURRENT;
                index_m = IM_IC;
                break;
              }
            case FULL_ORT_3I0:
              {
                porig_chutlyvosti = PORIG_CHUTLYVOSTI_CURRENT;
                index_m = IM_3I0;
                break;
              }
            case FULL_ORT_3I0_r:
              {
                porig_chutlyvosti = PORIG_CHUTLYVOSTI_CURRENT;
                index_m = IM_3I0_r;
                break;
              }
            case FULL_ORT_I04:
              {
                porig_chutlyvosti = PORIG_CHUTLYVOSTI_CURRENT;
                index_m = IM_I04;
                break;
              }
            default:
              {
                //Теоретично цього ніколи не мало б бути
                total_error_sw_fixed();
              }
          }

          if (measurement_low[index_m] >= porig_chutlyvosti)
          {
            //Розраховуємо кут
#define SIN_TARGET ortogonal_low_tmp[2 * index_tmp]
#define COS_TARGET ortogonal_low_tmp[2 * index_tmp + 1]

            unsigned int amplituda_target = sqrt_64((unsigned long long) ((long long) SIN_TARGET * (long long) SIN_TARGET) + (unsigned long long) ((long long) COS_TARGET * (long long) COS_TARGET));

            if (amplituda_target != 0)
            {
              //Вираховуємо COS і SIN кута різниці між векторами
              long long cos_fi = COS_TARGET * COS_BASE + SIN_TARGET * SIN_BASE;
              long long sin_fi = -(SIN_TARGET * COS_BASE - COS_TARGET * SIN_BASE); //З розділу: "Неймовірно, але факт", тобто що я не можу пояснити

#undef SIN_TARGET
#undef COS_TARGET

              float sin_fi_f = (((float) sin_fi) / ((float) amplituda_target)) / ((float) amplituda_base);
              if (sin_fi_f > 1)
                sin_fi_f = 1;
              else if (sin_fi_f < -1)
                sin_fi_f = -1;

              float angle_f = asinf(sin_fi_f) * 180 / PI;
              int angle_int = (int) (angle_f * 10); //беремо точність до десятих
              if (angle_int < 0)
                angle_int = -angle_int;

              if ((sin_fi >= 0) && (cos_fi >= 0))
              {
                //1-ий квадрант
                //angle_int залишається без зміни
              }
              else if ((sin_fi >= 0) && (cos_fi < 0))
              {
                //2-ий квадрант
                angle_int = 1800 - angle_int;
              }
              else if ((sin_fi < 0) && (cos_fi < 0))
              {
                //3-ий квадрант
                angle_int = 1800 + angle_int;
              }
              else
              {
                //4-ий квадрант
                angle_int = 3600 - angle_int;
              }

              if (angle_int >= 3600)
                angle_int -= 3600;
              else if (angle_int < 0)
                angle_int += 3600;

              phi_angle[bank_for_calc_phi_angle][index_tmp] = angle_int;
            }
            else
            {
              phi_angle[bank_for_calc_phi_angle][index_tmp] = -1;
            }
          }
          else
          {
            //Модуль досліджуваного вектора менше порогу - кут невизначений
            phi_angle[bank_for_calc_phi_angle][index_tmp] = -1;
          }
        }
      }
    }
    else
    {
      //Амплітуда базового вектору вимірювання по незрозумілій для мене причини рівна 0 (я думаю, що сюди програма не мала б ніколи заходити). Це перестарховка.
      for (__full_ort_index index_tmp = FULL_ORT_Ua; index_tmp < FULL_ORT_MAX; index_tmp++)
        phi_angle[bank_for_calc_phi_angle][index_tmp] = -1;
    }

#undef SIN_BASE
#undef COS_BASE
  }
  else
  {
    //Не зафіксовано вектора вимірювання, відносно якого можна розраховувати кути
    for (__full_ort_index index_tmp = FULL_ORT_Ua; index_tmp < FULL_ORT_MAX; index_tmp++)
      phi_angle[bank_for_calc_phi_angle][index_tmp] = -1;
  }

  state_calc_phi_angle = false;
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
  bank_ortogonal = bank_ortogonal_tmp;

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
  for (unsigned int i = 0; i < NUMBER_ANALOG_CANALES; i++)
  {
    if ((i >= I_Ia) && (i <= I_Ic))
    {
      unsigned int index_m = 0, index_ort = 0;
      switch (i)
      {
        case I_Ia:
          {
            index_m = IM_IA;
            index_ort = FULL_ORT_Ia;

            break;
          }
        case I_Ib_I04:
          {
            index_m = IM_IB;
            index_ort = FULL_ORT_Ib;

            break;
          }
        case I_Ic:
          {
            index_m = IM_IC;
            index_ort = FULL_ORT_Ic;

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
        case I_Ua:
        case I_Ub:
        case I_Uc:
          {
            unsigned int delta_index = (i - I_Ua);

            index_m = IM_UA + delta_index;
            index_ort = FULL_ORT_Ua + delta_index;

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
