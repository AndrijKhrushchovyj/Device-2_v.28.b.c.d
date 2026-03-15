#include "header.h"

#ifdef _TEST_DURATION
uint64_t durLogicCur = 0;
uint64_t periodLogicCur = 0;
#endif

static int32_t p_rele_reprogram_command = -1;
static int32_t p_rele_staff_command = -1;
static int32_t timerWaitReprogram = -1;

size_t head_queue_dr = 0;

/*****************************************************/
//Функція обробки таймерів
/*****************************************************/
inline void clocking_global_timers(void)
{
  //  //Опрацьовуємо дискретні входи
  //  input_scan();

  //опрацьовуємо всі решта таймери логіки
  for (int *p = (global_timers + _INDEX_TIMER_LOGIC_BEGIN); p != (global_timers + _MAX_NUMBER_GLOBAL_TIMERS); ++p)
  {
    if (*p >= 0)
    {
      //Першою умовою того, що таймер треба тактувати є той факт, що величина таймеру не від'ємна

      //Перевіряємо чи треба збільшувати величину таймеру, якщо він ще не досягнув свого максимуму
      if (*p <= (0x7fffffff - DELTA_TIME_FOR_TIMERS))
        *p += DELTA_TIME_FOR_TIMERS;
    }
  }

  if ((POWER_CTRL->IDR & POWER_CTRL_PIN) != (uint32_t) Bit_RESET)
  {
    timePowerDown_total = timePowerDown = -1;
  }
  else
  {
    if (timePowerDown < 0)
      timePowerDown = 0;
    else if (
      (timePowerDown >= 0) &&
      (timePowerDown <= (0x7fffffff - DELTA_TIME_FOR_TIMERS)))
    {
      timePowerDown += DELTA_TIME_FOR_TIMERS;
    }

    if (timePowerDown_total < 0)
      timePowerDown_total = 0;
    else if (
      (timePowerDown_total >= 0) &&
      (timePowerDown_total <= (0x7fffffff - DELTA_TIME_FOR_TIMERS)))
    {
      timePowerDown_total += DELTA_TIME_FOR_TIMERS;
    }
  }

  timer_prt_signal_output_mode_2 += DELTA_TIME_FOR_TIMERS;
  if (timer_prt_signal_output_mode_2 >= PERIOD_SIGNAL_OUTPUT_MODE_2)
  {
    timer_prt_signal_output_mode_2 = 0;
    output_timer_prt_signal_output_mode_2 ^= true;
  }

  //Управління поляризованим реле для перепрограмування
  if (
    (p_rele_reprogram_command < 0) &&
    (p_rele_staff_command < 0))
  {
    if (_GET_STATE(reprogram_device, REPROGRAM_COMMAND))
    {
      p_rele_reprogram_command = 0;
      GPIO_REPROGRAM->BSRRL = GPIO_PIN_REPROGRAM;
    }

    if (_GET_STATE(reprogram_device, STAFF_COMMAND))
    {
      p_rele_staff_command = 0;
      GPIO_STAFF->BSRRL = GPIO_PIN_STAFF;
    }
  }
  else
  {
    int32_t *point_p_rele = NULL;
    if (p_rele_reprogram_command >= 0)
      point_p_rele = &p_rele_reprogram_command;
    else
      point_p_rele = &p_rele_staff_command;

    if (*point_p_rele < (0x7fffffff - DELTA_TIME_FOR_TIMERS))
      *point_p_rele += DELTA_TIME_FOR_TIMERS;
    if (*point_p_rele >= TIMEOUT_IMPULSE_P_RELE_REPROGRAM_STAFF)
    {
      *point_p_rele = -1;

      if (point_p_rele == &p_rele_reprogram_command)
      {
        _CLEAR_STATE(reprogram_device, REPROGRAM_COMMAND);
        GPIO_REPROGRAM->BSRRH = GPIO_PIN_REPROGRAM;
      }
      else if (point_p_rele == &p_rele_staff_command)
      {
        _CLEAR_STATE(reprogram_device, STAFF_COMMAND);
        GPIO_STAFF->BSRRH = GPIO_PIN_STAFF;
      }
      else
      {
        //Теоретично цього ніколи не мало б бути
        total_error_sw_fixed();
      }
    }
  }

  if (p_rele_reprogram_command == 0)
    timerWaitReprogram = 0;
  else
  {
    if (timerWaitReprogram >= 0)
    {
      if (++timerWaitReprogram > (2 * TIMEOUT_IMPULSE_P_RELE_REPROGRAM_STAFF))
      {
        //Режим перепрограмування не підтримується
        timerWaitReprogram = -1;
        _SET_STATE(reprogram_device, UNSUPPORT_REPROGRAM);
      }
    }
  }
}
/*****************************************************/

/*****************************************************/
//Опрацювання Ориділювальних функцій - має запускатися після відкрпацювання блоків всіх захистів
/*****************************************************/
inline void df_handler(unsigned int *p_active_functions)
{
  unsigned int logic_df[NUMBER_DEFINED_FUNCTIONS];

  //Визначаємо, чи активовуються опреділювані функції через свої ранжовані функції-джерела
  for (size_t i = 0; i < NUMBER_DEFINED_FUNCTIONS; ++i)
  {
    unsigned int *const p_logic_df = (logic_df + i);
    *p_logic_df = 0;
    unsigned int const shift = 3 * i;

    unsigned int const rang_df_in = RANG_DF1_IN + shift;
    unsigned int const number_byte_in = rang_df_in >> 5;
    unsigned int const number_bit_in = rang_df_in & 0x1f;

    /***
    Джерело активації ОФ-ії
    ***/
    *p_logic_df |= ((p_active_functions[number_byte_in] & (1 << number_bit_in)) >> number_bit_in) << 0;
    //Перевіряємо ще, чи не іде утимування активним джерела ОФ через таймер-утримування (для активації через кнопки або інтерфейс)
    if (global_timers[INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START + i] >= 0)
    {
      //Таймер запущений, або вже зупинився
      //Факт запуску цього таймеру означає, що активація відбувалася через кнопку, або інтерфейс
      //Тому для забеспечення роботи логічної схеми до кінця роботи цього таймеру виставляємо, що джерело активації активне
      *p_logic_df |= (1 << 0);

      //Відмічаємо, джерело активації утримуємться у активному стані у масиві активуючих функцій
      p_active_functions[number_byte_in] |= (1 << number_bit_in);

      //У випадку, якщо таймер дійшов до свого макисального значення, то скидаємо роботу цього таймеру
      if (global_timers[INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START + i] >= ((int) current_settings_prt.timeout_pause_df[i]))
        global_timers[INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START + i] = -1;
    }
    /***/

    int flag = 0;
    for (int m = 0; m < N_BIG; m++)
      if ((current_settings_prt.ranguvannja_df_source_plus[N_BIG * i + m] & p_active_functions[m]) != 0)
      {
        flag = 1;
        break;
      }
    if (flag)
    {
      *p_logic_df |= (1 << 0);
    }

    flag = 0;
    for (int m = 0; m < N_BIG; m++)
      if ((current_settings_prt.ranguvannja_df_source_minus[N_BIG * i + m] & ((unsigned int) (~p_active_functions[m]))) != 0)
      {
        flag = 1;
        break;
      }
    if (flag)
    {
      *p_logic_df |= (1 << 0);
    }

    flag = 1;
    for (int m = 0; m < N_BIG; m++)
      if ((current_settings_prt.ranguvannja_df_source_blk[N_BIG * i + m] & p_active_functions[m]) != 0)
      {
        flag = 0;
        break;
      }
    if (flag)
    {
      *p_logic_df |= (1 << 1);
    }

    /***
    Виконуємо у цьому місці обробку логіки без встанвлення/скидання бітів про стан Визначуваної функції
    Щоб новий стан попередньої функції не був включений у стан наступної
    ***/
    *p_logic_df |= (_CHECK_SET_BIT(p_active_functions, (RANG_DF1_R + shift)) != 0) << 5;
    flag = 0;
    for (size_t m = 0; m < N_BIG; ++m)
      if ((current_settings_prt.ranguvannja_df_source_reset[N_BIG * i + m] & p_active_functions[m]) != 0)
      {
        flag = 1;
        break;
      }
    if (flag)
    {
      *p_logic_df |= (1u << 5);
    }

    _INVERTOR((*p_logic_df), 5, (*p_logic_df), 6);
    _AND2((*p_logic_df), 0, (*p_logic_df), 6, (*p_logic_df), 7);

    _TIMER_T_0(INDEX_TIMER_DF_PAUSE_START + i, current_settings_prt.timeout_pause_df[i], (*p_logic_df), 7, (*p_logic_df), 2);

    if ((current_settings_prt.type_df & (1 << i)) == 0)
    {
      _TIMER_IMPULSE_R(INDEX_TIMER_DF_WORK_START + i, current_settings_prt.timeout_work_df[i], (*p_logic_df), 5, static_logic_df, i, (*p_logic_df), 2, (*p_logic_df), 3);
    }
    else
    {
      _TIMER_0_T_R(INDEX_TIMER_DF_WORK_START + i, current_settings_prt.timeout_work_df[i], (*p_logic_df), 5, (*p_logic_df), 2, (*p_logic_df), 3);
      static_logic_df &= ~(1u << i);
    }

    _AND2((*p_logic_df), 3, (*p_logic_df), 1, (*p_logic_df), 4);
    /***/
  }

  //Установлюємо, або скидаємо ОФ у масиві функцій, які зараз будуть активовуватися
  /*
  Цей цикл і попередній не об'єднаі в один, а навпаки розєднані, бо у першому ми використовуємо
  масив p_active_functions у якому ще не встановлені виходи ОФ-ій, тому що інші ОФ-ії
  можуть бути джерелом активації, але джерелом активації може буте попереднє значення ОФ, а не те,
  що зараз встановлюється. А оскілдьки у другому масиві ми встановлюємо значення у масиві
  p_active_functions, які набувають зараз тільки ваги, то щоб не вийшло об'єднання попереднього значення
  і теперішнього то цикли роз'єднані (цикл аналізу джерел і логіки з циклом активації/деактивації)
  */
  for (size_t i = 0; i < NUMBER_DEFINED_FUNCTIONS; ++i)
  {
    //Установлюємо, або скидаємо ОФ
    unsigned int const index_df = RANG_DF1_OUT + 3 * i;

    if ((logic_df[i] & (1u << 4)) != 0)
      _SET_BIT(p_active_functions, index_df);
    else
      _CLEAR_BIT(p_active_functions, index_df);
  }
}
/*****************************************************/

/*****************************************************/
//Опрацювання Ориділювальних триґерів - має запускатися після відкрпацювання опреділювальних функцій
/*****************************************************/
inline void dt_handler(unsigned int *p_active_functions)
{
  /*
  Попередній стан визначуваних триґерів формуємо у state_defined_triggers
  Джерела встановлення формуємо в source_set_dt
  Джерела скидання формуємо в source_reset_dt
  */
  unsigned int state_defined_triggers = 0, source_set_dt = 0, source_reset_dt = 0;
  for (size_t i = 0; i < NUMBER_DEFINED_TRIGGERS /*current_settings_prt.number_defined_dt*/; ++i)
  {
    unsigned int const shift = 3 * i;

    unsigned int const rang_df_set = RANG_DT1_SET + shift;
    unsigned int const number_byte_set = rang_df_set >> 5;
    unsigned int const number_bit_set = rang_df_set & 0x1f;

    unsigned int const rang_df_reset = RANG_DT1_RESET + shift;
    unsigned int const number_byte_reset = rang_df_reset >> 5;
    unsigned int const number_bit_reset = rang_df_reset & 0x1f;

    unsigned int const index_dt = RANG_DT1_OUT + shift;

    state_defined_triggers |= ((p_active_functions[index_dt >> 5] & (1 << (index_dt & 0x1f))) >> (index_dt & 0x1f)) << i;
    /***
    Джерела встановлення і скидання ОТ
    ***/
    source_set_dt |= ((p_active_functions[number_byte_set] & (1 << number_bit_set)) >> number_bit_set) << i;
    source_reset_dt |= ((p_active_functions[number_byte_reset] & (1 << number_bit_reset)) >> number_bit_reset) << i;
    /***/
  }

  //Визначаємо, чи встановлюються/скидаються опреділювані триґери через свої ранжовані функції-джерела
  for (size_t i = 0; i < NUMBER_DEFINED_TRIGGERS /*current_settings_prt.number_defined_dt*/; ++i)
  {
    int flag = 0;
    for (int m = 0; m < N_BIG; m++)
      if (current_settings_prt.ranguvannja_set_dt_source_plus[N_BIG * i + m] != 0)
      {
        flag = 1;
        break;
      }
    if (flag)
    {
      //Випадок, якщо функції зранжовані на джерело прямих функцій
      flag = 0;
      for (int m = 0; m < N_BIG; m++)
        if ((current_settings_prt.ranguvannja_set_dt_source_plus[N_BIG * i + m] & p_active_functions[m]) != 0)
        {
          flag = 1;
          break;
        }
      if (flag)
      {
        source_set_dt |= (1 << i);
      }
    }

    flag = 0;
    for (int m = 0; m < N_BIG; m++)
      if (current_settings_prt.ranguvannja_set_dt_source_minus[N_BIG * i + m] != 0)
      {
        flag = 1;
        break;
      }
    if (flag)
    {
      //Випадок, якщо функції зранжовані на джерело інверсних функцій
      flag = 0;
      for (int m = 0; m < N_BIG; m++)
        if ((current_settings_prt.ranguvannja_set_dt_source_minus[N_BIG * i + m] & ((unsigned int) (~p_active_functions[m]))) != 0)
        {
          flag = 1;
          break;
        }
      if (flag)
      {
        source_set_dt |= (1 << i);
      }
    }

    flag = 0;
    for (int m = 0; m < N_BIG; m++)
      if (current_settings_prt.ranguvannja_reset_dt_source_plus[N_BIG * i + m] != 0)
      {
        flag = 1;
        break;
      }
    if (flag)
    {
      //Випадок, якщо функції зранжовані на джерело прямих функцій
      flag = 0;
      for (int m = 0; m < N_BIG; m++)
        if ((current_settings_prt.ranguvannja_reset_dt_source_plus[N_BIG * i + m] & p_active_functions[m]) != 0)
        {
          flag = 1;
          break;
        }
      if (flag)
      {
        source_reset_dt |= (1 << i);
      }
    }

    flag = 0;
    for (int m = 0; m < N_BIG; m++)
      if (current_settings_prt.ranguvannja_reset_dt_source_minus[N_BIG * i + m] != 0)
      {
        flag = 1;
        break;
      }
    if (flag)
    {
      //Випадок, якщо функції зранжовані на джерело інверсних функцій
      flag = 0;
      for (int m = 0; m < N_BIG; m++)
        if ((current_settings_prt.ranguvannja_reset_dt_source_minus[N_BIG * i + m] & ((unsigned int) (~p_active_functions[m]))) != 0)
        {
          flag = 1;
          break;
        }
      if (flag)
      {
        source_reset_dt |= (1 << i);
      }
    }

    //Запускаємо у роботу лоргічну схему роботи опреділюваного триґера (встановлення має пріоритет)
    if ((source_set_dt & (1 << i)) != 0)
      state_defined_triggers |= (1 << i);
    if ((source_reset_dt & (1 << i)) != 0)
      state_defined_triggers &= (unsigned int) (~(1 << i));
  }

  //Установлюємо, або скидаємо ОТ у масиві функцій, які зараз будуть активовуватися
  /*
  Цей цикл і попередній не об'єднаі в один, а навпаки розєднані, бо у першому ми використовуємо
  масив p_active_functions у якому ще не встановлені нові значення виходыв ОТ-ів, тому що інші ОТ-и
  можуть бути джерелом встановлення/скидання, але джерелом встановлення/скидання може буте попереднє значення ОТ, а не те,
  що зараз встановлюється. А оскілдьки у ми встановлюємо значення у масиві
  p_active_functions, які набувають зараз тільки ваги, то щоб не вийшло об'єднання попереднього значення
  і теперішнього то цикли роз'єднані (цикл аналізу джерел і логіки з циклом встановлення/скидання)
  */
  for (unsigned int i = 0; i < NUMBER_DEFINED_TRIGGERS; i++)
  {
    //Установлюємо, або скидаємо ОТ
    unsigned int index_dt = RANG_DT1_OUT + 3 * i;

    if ((state_defined_triggers & (1 << i)) != 0)
      _SET_BIT(p_active_functions, index_dt);
    else
      _CLEAR_BIT(p_active_functions, index_dt);
  }
}
/*****************************************************/

/*****************************************************/
//Опрацювання визначуваних "І"
/*****************************************************/
inline void d_and_handler(unsigned int *p_active_functions)
{
  unsigned int state_defined_and = 0;

  //Визначаємо стан всіх визначуваних "І" (не виставляючи поки що їх у робочому масиві)
  for (unsigned int i = 0; i < NUMBER_DEFINED_AND /*current_settings_prt.number_defined_and*/; i++)
  {
    int flag = 1;
    for (int m = 0; m < N_BIG; m++)
      if ((current_settings_prt.ranguvannja_d_and[N_BIG * i + m] & p_active_functions[m]) != current_settings_prt.ranguvannja_d_and[N_BIG * i + m])
      {
        flag = 0;
        break;
      }
    if (flag)
    {
      state_defined_and |= (1 << i);
    }
  }

  //Установлюємо, або скидаємо визначувані "І" у масиві функцій, які зараз будуть активовуватися
  /*
  Цей цикл і попередній не об'єднаі в один, а навпаки розєднані, бо у першому ми використовуємо
  масив p_active_functions у якому ще не встановлені нові значення виходів В-"І", тому що інші В-"І"
  можуть бути джерелом , але джерелом може буте попереднє значення В-"І", а не те,
  що зараз встановлюється. А оскілдьки ми встановлюємо значення у масиві
  p_active_functions, які набувають зараз тільки ваги, то щоб не вийшло об'єднання попереднього значення
  і теперішнього то цикли роз'єднані (цикл аналізу джерел і логіки з циклом встановлення/скидання)
  */
  for (unsigned int i = 0; i < NUMBER_DEFINED_AND; i++)
  {
    //Установлюємо, або скидаємо В-"І"
    unsigned int index_d_and = RANG_D_AND1 + i;

    if ((state_defined_and & (1 << i)) != 0)
      _SET_BIT(p_active_functions, index_d_and);
    else
      _CLEAR_BIT(p_active_functions, index_d_and);
  }
}
/*****************************************************/

/*****************************************************/
//Опрацювання визначуваних "АБО"
/*****************************************************/
inline void d_or_handler(unsigned int *p_active_functions)
{
  unsigned int state_defined_or = 0;

  //Визначаємо стан всіх визначуваних "АБО" (не виставляючи поки що їх у робочому масиві)
  for (unsigned int i = 0; i < NUMBER_DEFINED_OR /*current_settings_prt.number_defined_or*/; i++)
  {
    int flag = 0;
    for (int m = 0; m < N_BIG; m++)
      if ((current_settings_prt.ranguvannja_d_or[N_BIG * i + m] & p_active_functions[m]) != 0)
      {
        flag = 1;
        break;
      }
    if (flag)
    {
      state_defined_or |= (1 << i);
    }
  }

  //Установлюємо, або скидаємо визначувані "АБО" у масиві функцій, які зараз будуть активовуватися
  /*
  Цей цикл і попередній не об'єднаі в один, а навпаки розєднані, бо у першому ми використовуємо
  масив p_active_functions у якому ще не встановлені нові значення виходів В-"АБО", тому що інші В-"АБО"
  можуть бути джерелом , але джерелом може буте попереднє значення В-"АБО", а не те,
  що зараз встановлюється. А оскілдьки ми встановлюємо значення у масиві
  p_active_functions, які набувають зараз тільки ваги, то щоб не вийшло об'єднання попереднього значення
  і теперішнього то цикли роз'єднані (цикл аналізу джерел і логіки з циклом встановлення/скидання)
  */
  for (unsigned int i = 0; i < NUMBER_DEFINED_OR; i++)
  {
    //Установлюємо, або скидаємо В-"АБО"
    unsigned int index_d_or = RANG_D_OR1 + i;

    if ((state_defined_or & (1 << i)) != 0)
      _SET_BIT(p_active_functions, index_d_or);
    else
      _CLEAR_BIT(p_active_functions, index_d_or);
  }
}
/*****************************************************/

/*****************************************************/
//Опрацювання визначуваних "Викл.АБО"
/*****************************************************/
inline void d_xor_handler(unsigned int *p_active_functions)
{
  unsigned int state_defined_xor = 0;

  //Визначаємо стан всіх визначуваних "Викл.АБО" (не виставляючи поки що їх у робочому масиві)
  for (unsigned int i = 0; i < NUMBER_DEFINED_XOR /*current_settings_prt.number_defined_xor*/; i++)
  {
    unsigned int temp_array[N_BIG];
    for (int m = 0; m < N_BIG; m++)
      temp_array[m] = current_settings_prt.ranguvannja_d_xor[N_BIG * i + m] & p_active_functions[m];

    int flag = 0;
    for (int m = 0; m < N_BIG; m++)
      if (temp_array[m] != 0)
      {
        flag = 1;
        break;
      }
    if (flag)
    {
      unsigned int signals = 0;
      for (unsigned int j = 0; j < N_BIG; j++)
      {
        if (temp_array[j] == 0)
          continue;
        else
        {
          for (unsigned int k = 0; k < 32; k++)
          {
            if ((temp_array[j] & (1 << k)) != 0)
            {
              signals++;
              if (signals > 1)
                break;
            }
          }
          if (signals > 1)
            break;
        }
      }
      if (signals == 1)
        state_defined_xor |= (1 << i);
    }
  }

  //Установлюємо, або скидаємо визначувані "Викл.АБО" у масиві функцій, які зараз будуть активовуватися
  /*
  Цей цикл і попередній не об'єднаі в один, а навпаки розєднані, бо у першому ми використовуємо
  масив p_active_functions у якому ще не встановлені нові значення виходів В-"Викл.АБО", тому що інші В-"Викл.АБО"
  можуть бути джерелом , але джерелом може буте попереднє значення В-"Викл.АБО", а не те,
  що зараз встановлюється. А оскілдьки ми встановлюємо значення у масиві
  p_active_functions, які набувають зараз тільки ваги, то щоб не вийшло об'єднання попереднього значення
  і теперішнього то цикли роз'єднані (цикл аналізу джерел і логіки з циклом встановлення/скидання)
  */
  for (unsigned int i = 0; i < NUMBER_DEFINED_XOR; i++)
  {
    //Установлюємо, або скидаємо В-"Викл.АБО"
    unsigned int index_d_xor = RANG_D_XOR1 + i;

    if ((state_defined_xor & (1 << i)) != 0)
      _SET_BIT(p_active_functions, index_d_xor);
    else
      _CLEAR_BIT(p_active_functions, index_d_xor);
  }
}
/*****************************************************/

/*****************************************************/
//Опрацювання визначуваних "НЕ"
/*****************************************************/
inline void d_not_handler(unsigned int *p_active_functions)
{
  unsigned int state_defined_not = 0;

  //Визначаємо стан всіх визначуваних "НЕ" (не виставляючи поки що їх у робочому масиві)
  for (unsigned int i = 0; i < NUMBER_DEFINED_NOT /*current_settings_prt.number_defined_not*/; i++)
  {
    int flag = 1;
    for (int m = 0; m < N_BIG; m++)
      if ((current_settings_prt.ranguvannja_d_not[N_BIG * i + m] & p_active_functions[m]) != 0)
      {
        flag = 0;
        break;
      }
    if (flag)
    {
      state_defined_not |= (1 << i);
    }
  }

  //Установлюємо, або скидаємо визначувані "НЕ" у масиві функцій, які зараз будуть активовуватися
  /*
  Цей цикл і попередній не об'єднаі в один, а навпаки розєднані, бо у першому ми використовуємо
  масив p_active_functions у якому ще не встановлені нові значення виходів В-"НЕ", тому що інші В-"НЕ"
  можуть бути джерелом , але джерелом може буте попереднє значення В-"НЕ", а не те,
  що зараз встановлюється. А оскілдьки ми встановлюємо значення у масиві
  p_active_functions, які набувають зараз тільки ваги, то щоб не вийшло об'єднання попереднього значення
  і теперішнього то цикли роз'єднані (цикл аналізу джерел і логіки з циклом встановлення/скидання)
  */
  for (unsigned int i = 0; i < NUMBER_DEFINED_NOT; i++)
  {
    //Установлюємо, або скидаємо В-"НЕ"
    unsigned int index_d_not = RANG_D_NOT1 + i;

    if ((state_defined_not & (1 << i)) != 0)
      _SET_BIT(p_active_functions, index_d_not);
    else
      _CLEAR_BIT(p_active_functions, index_d_not);
  }
}
/*****************************************************/

/*****************************************************/
//Опрацювання Передавальних функцій
/*****************************************************/
//#pragma optimize=none
inline void tf_handler(unsigned int *p_previous_active_functions, unsigned int *p_active_functions, unsigned int *p_active_inputs_grupa_ustavok)
{
  for (size_t i = 0; i < NUMBER_TRANSFER_FUNCTIONS; i++)
  {
    uint32_t value = current_settings_prt.ranguvannja_tf[i];
    uint32_t value_input = value & 0xffff;
    if (value_input > 0)
    {
      value_input--;
      if (value_input < NUMBER_TOTAL_SIGNAL_FOR_RANG)
      {
        if (_CHECK_SET_BIT(p_previous_active_functions, value_input) != 0)
        {
          uint32_t value_output = (value >> 16) & 0xffff;
          if (value_output > 0)
          {
            value_output--;
            if (value_output < NUMBER_TOTAL_SIGNAL_FOR_RANG_SMALL)
            {
              uint32_t const small_big_rang_tmp = small_big_rang[value_output];
              if (
                (small_big_rang_tmp >= RANG_1_GRUPA_USTAVOK) &&
                (small_big_rang_tmp < (RANG_1_GRUPA_USTAVOK + NUMBER_GROUP_USTAVOK)))
              {
                *p_active_inputs_grupa_ustavok |= 1u << (small_big_rang_tmp - RANG_1_GRUPA_USTAVOK);
              }
              else
              {
                _SET_BIT(p_active_functions, small_big_rang_tmp);
              }
            }
            else
            {
              //Відбулася невизначена помилка, тому треба піти на перезавантаження
              total_error_sw_fixed();
            }
          }
        }
      }
      else
      {
        //Відбулася невизначена помилка, тому треба піти на перезавантаження
        total_error_sw_fixed();
      }
    }
  }
}
/*****************************************************/

/*****************************************************/
// Готовность к ТУ
/*****************************************************/
inline void ready_tu(unsigned int *p_active_functions)
{
  unsigned int tmp_value = (_CHECK_SET_BIT(p_active_functions, RANG_PRYVID_VV) == 0) << 0;
  tmp_value |= (_CHECK_SET_BIT(p_active_functions, RANG_VIDKL_VID_ZAKHYSTIV) != 0) << 1;
  tmp_value |= (_CHECK_SET_BIT(p_active_functions, RANG_RESET_BLOCK_READY_TU_VID_ZAHYSTIV) != 0) << 2;
  tmp_value |= (_CHECK_SET_BIT(p_active_functions, RANG_AVAR_DEFECT) == 0) << 3;
  tmp_value |= (_CHECK_SET_BIT(p_active_functions, RANG_MISCEVE_DYSTANCIJNE) == 0) << 6;

  _Bool ctrl_ready_tu = ((current_settings_prt.control_extra_settings_1 & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_READY_TU)) == 0);

  if (!previous_states_ready_tu && _GET_STATE(tmp_value, 1))
  {
    previous_states_ready_tu = 0;
  }

  _OR2(tmp_value, 2, ctrl_ready_tu, 0, tmp_value, 7);
  _D_TRIGGER(1, 0, _GET_STATE(tmp_value, 7), previous_states_ready_tu, 0, tmp_value, 1, trigger_ready_tu, 0);

  _AND4(tmp_value, 0, tmp_value, 3, !trigger_ready_tu, 0, tmp_value, 6, tmp_value, 5);

  //Готовность к ТУ
  if (_GET_STATE(tmp_value, 5))
    _SET_BIT(p_active_functions, RANG_READY_TU);
  else
    _CLEAR_BIT(p_active_functions, RANG_READY_TU);
}
/*****************************************************/

/*****************************************************/
//Перевірка на необхідність завершення роботи аналогового/дискретного реєстраторів
/*****************************************************/
inline unsigned int stop_regisrator(unsigned int *carrent_active_functions, unsigned int *ranguvannja_registrator)
{
  if (global_timers[INDEX_TIMER_DR_WORK] < 0)
    global_timers[INDEX_TIMER_DR_WORK] = DELTA_TIME_FOR_TIMERS; /*вже, к мінімум з моменту запуску ДР процшло стільки мілісекунд*/

  int flag = 0;
  for (size_t m = 0; m < N_BIG; ++m)
  {
    if ((carrent_active_functions[m] & ranguvannja_registrator[m]) != 0)
    {
      flag = (1u << 0);
      break;
    }
  }

  _TIMER_0_T(INDEX_TIMER_DR_WORK, current_settings_prt.timeout_prolongation_work_digital_registrator + DELTA_TIME_FOR_TIMERS, flag, 0, flag, 1);

  return (flag == 0);
}
/*****************************************************/

/*****************************************************/
//Зафіксована невизначена помилка роботи дискретного реєстратора
/*****************************************************/
inline void fix_undefined_error_dr(unsigned int *carrent_active_functions)
{
  //Виставляємо помилку з записом в дисретний реєстратор
  _SET_BIT(set_diagnostyka, ERROR_DR_UNDEFINED_BIT);
  _SET_BIT(carrent_active_functions, RANG_DEFECT);
  //Переводимо режим роботи з реєстратором у сатн "На даний момент ніких дій з дискретним реєстратором не виконується"
  state_dr_record = STATE_DR_NO_RECORD;
  //Скидаєсо сигнал роботи дискретного реєстратора
  _CLEAR_BIT(carrent_active_functions, RANG_WORK_D_REJESTRATOR);
}
/*****************************************************/

/*****************************************************/
//Серіалізація unsigned int у масив у unsigned char
/*****************************************************/
inline unsigned char *serialization_uint_uchar(unsigned int const *const p_input, unsigned char *p_output, size_t const cut)
{
  unsigned char const *p_data = (unsigned char const *) (p_input);
  for (size_t i = 0; i < (sizeof(unsigned int) - cut); ++i)
  {
    *p_output++ = *p_data++;
  }

  return p_output;
}
/*****************************************************/

/*****************************************************/
//Серіалізація unsigned int у масив у unsigned char
/*****************************************************/
inline unsigned int *deserialization_uchar_uint(unsigned char const *p_input, unsigned int *p_output)
{
  *p_output = 0;
  for (size_t i = 0; i < sizeof(unsigned int); ++i)
  {
    *p_output |= (*p_input++) << (8 * i);
  }
  return ++p_output;
}
/*****************************************************/

/*****************************************************/
//Завершення моніторингу максимального струму
/*****************************************************/
inline void end_monitoring_min_max_measurement(unsigned char **const arr_identifiers, unsigned char const *const buffer_for_save_dr_record, unsigned int const type_current, unsigned int *const carrent_active_functions)
{
  if (
    /* (
      (type_current == IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE) ||
      (type_current == IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE04) ||
      (type_current == IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZNAM) ||
      (type_current == IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0_1G) ||
      (type_current == IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0_OTHERG) ||
      (type_current == IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0_r) ||
      (type_current == IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE_3U0) ||
      (type_current == IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE) ||
      (type_current == IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE) ||
      (type_current == IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZOP) ||
      (type_current == IDENTIFIER_BIT_ARRAY_MIN_FREQUENCY_ACHR) ||
      (type_current == IDENTIFIER_BIT_ARRAY_FREQUENCY_CHAPV)) && */
    (arr_identifiers[type_current - 1] != NULL))
  {
    unsigned char *buffer = arr_identifiers[type_current - 1];
    if (
      (buffer >= (buffer_for_save_dr_record + FIRST_INDEX_FIRST_BLOCK_DR)) &&
      ((buffer + SIZE_ARRAY_FIX_MAX_MEASUREMENTS * sizeof(unsigned int) - 1) < (buffer_for_save_dr_record + FIRST_INDEX_FIRST_DATA_DR)))
    {
      //Знімаємо помітку, що ми на стадії моніторингу
      arr_identifiers[type_current - 1] = NULL;
    }
    else
    {
      //Відбулася незрозуміла ситуація - сюди програма теоретично ніколи не мала б заходити
      fix_undefined_error_dr(carrent_active_functions);
    }
  }
  else
  {
    //Відбулася незрозуміла ситуація - сюди програма теоретично ніколи не мала б заходити
    fix_undefined_error_dr(carrent_active_functions);
  }
}
/*****************************************************/

/*****************************************************/
//Подача команди почати запис у DataFalsh
/*****************************************************/
inline void command_start_saving_record_dr_into_dataflash(unsigned int *const p_saving_record_dr)
{
  //У структурі по інформації стану реєстраторів виставляємо повідомлення, що почався запис і ще не закінчився
  _SET_BIT(control_spi1_taskes, TASK_START_WRITE_INFO_REJESTRATOR_DR_EEPROM_BIT);
  info_rejestrator_dr.saving_execution = 1;

  //Виставляємо першу частину запису
  part_writing_dr_into_dataflash = 0;
  //Виставляємо команду запису у мікросхему DataFlash
  *p_saving_record_dr = true;
  control_tasks_dataflash |= TASK_MAMORY_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_DR;
}
/*****************************************************/

/*****************************************************/
//Функція обробки черги зформованих записів дискретного реєстратора
/*****************************************************/
inline void routine_for_queue_dr(unsigned int *const p_saving_record_dr)
{
  if (
    (state_dr_record == STATE_DR_MAKE_RECORD) || /*Стоїть умова сформовані дані передати на запис у DataFlsh без повторного початку  нового запису, бо умова завершення запиу    досягнкта, а не стояла умова примусового завершення даного сформованого запису*/
    (state_dr_record == STATE_DR_CUT_RECORD)     /*Стоїть умова сформовані дані передати на запис у DataFlsh   з повторним  початком нового запису, бо умова завершення запиу не досягнкта, а    стояла умова примусового завершення даного сформованого запису*/
  )
  {
    if (++head_queue_dr >= SIZE_QUEUE_DR)
      head_queue_dr = 0;

    if (state_dr_record == STATE_DR_MAKE_RECORD)
    {
      //Помічаємо, що ми готові приймати наступний запис
      state_dr_record = STATE_DR_NO_RECORD;
    }
    else if (state_dr_record == STATE_DR_CUT_RECORD)
    {
      //Помічаємо, що треба примусово запустити новий запис
      state_dr_record = STATE_DR_FORCE_START_NEW_RECORD;
    }
  }

  if ((control_tasks_dataflash & TASK_MAMORY_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_DR) == 0)
  {
    if (head_queue_dr != tail_queue_dr)
    {
      //Подаємо команду почати запис у DataFalsh
      command_start_saving_record_dr_into_dataflash(p_saving_record_dr);
    }
  }
}
/*****************************************************/

long tstBrrFooSelector = 0; //змінна для тестів і перевірок

/*****************************************************/
//Функція обробки логіки дискретного реєстратора
/*****************************************************/
inline void digital_registrator(unsigned int *carrent_active_functions)
{

  /***
  Стан оперативного живлення від якого залежить як Дискретний реєстратор буде працювати
  ***/
  enum _statePowerDown
  {
    STATE_POWER_DOWN_NONE = 0,
    STATE_POWER_DOWN_ETAP_BEFORE,
    STATE_POWER_DOWN_ETAP_CUT,
    STATE_POWER_DOWN_ETAP_CUT_CONFIRMED,
    STATE_POWER_DOWN_ETAP_AFTER
  };

  static enum _statePowerDown statePowerDown;
  if (((POWER_CTRL->IDR & POWER_CTRL_PIN) != (uint32_t) Bit_RESET))
    statePowerDown = STATE_POWER_DOWN_NONE;
  else
  {
    if (timePowerDown < POWER_DOWN_TIME)
      statePowerDown = STATE_POWER_DOWN_ETAP_BEFORE;
    else
    {
      if (statePowerDown == STATE_POWER_DOWN_ETAP_BEFORE)
        statePowerDown = STATE_POWER_DOWN_ETAP_CUT;
      else if (statePowerDown == STATE_POWER_DOWN_ETAP_CUT_CONFIRMED)
        statePowerDown = STATE_POWER_DOWN_ETAP_AFTER;
    }
  }
  /***/

  static unsigned int previous_active_functions[N_BIG];

  static unsigned int number_items_dr;
  static unsigned int number_changes_into_dr_record;
  static unsigned int time_from_start_record_dr;
  static unsigned int blocking_continue_monitoring_min_U;

  DigRegUniqVarsAddreses drUniqVarsAddreses = {
    &number_items_dr,
    &number_changes_into_dr_record,
    &time_from_start_record_dr,
    &blocking_continue_monitoring_min_U,
    previous_active_functions,
    carrent_active_functions};

  unsigned char *buffer_for_save_dr_record = queue_dr[head_queue_dr];
  static unsigned int saving_record_dr = false;
  static unsigned char *arr_identifiers[_NUMBER_IDENTIFIER] =
    {
      NULL,
      NULL};
  // static unsigned char *next_free_array = NULL;

  //Цю перевірку виконуємо тільки у тому випадку, коли іде процес формування нового запису
  if (state_dr_record == STATE_DR_EXECUTING_RECORD)
  {
    //Перевіряємо чи не виникла умова, що зарараз буде перебір фіксації максимальних струмів
    unsigned int temp_value_for_max_min_fix_measurement = (number_min_U_dr +
                                                           number_max_U_dr);
    if (temp_value_for_max_min_fix_measurement > MAX_NUMBER_FIX_MAX_MEASUREMENTS)
    {
      //Сюди, теоретично програма нікол не мала б заходити, але якщо зайшла, тоиреба перервати роботу дискретного реєстратора
      fix_undefined_error_dr(carrent_active_functions);

      /*
      Скидаємо сигнал роботи дискретного реєстратора у масиві попередніх активних функцій
      Це ми робимо для того, щоб у першому записі нового запису було зафіксовано активацію роботу дискретного реєстратора
      */
      _CLEAR_BIT(previous_active_functions, RANG_WORK_D_REJESTRATOR);
    }
    else
    {
      if (temp_value_for_max_min_fix_measurement > MAX_NUMBER_FIX_MAX_MEASUREMENTS)
      {
        //Виникла ситуація, що зарараз буде перебір фіксації максимальних вимірювань
        //Треба примусово завершити текучий запис і почати наступний запис

        buffer_for_save_dr_record[FIRST_INDEX_NUMBER_ITEMS_DR] = number_items_dr;
        buffer_for_save_dr_record[FIRST_INDEX_NUMBER_CHANGES_DR] = number_changes_into_dr_record & 0xff;
        buffer_for_save_dr_record[FIRST_INDEX_NUMBER_CHANGES_DR + 1] = (number_changes_into_dr_record >> 8) & 0xff;

        //Перевіряємо чи треба завершити моніторинг мінімальної напруги
        if (arr_identifiers[IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE - 1] != NULL)
          end_monitoring_min_max_measurement(arr_identifiers, buffer_for_save_dr_record, IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE, carrent_active_functions);

        //Перевіряємо чи треба завершити моніторинг максимальної напруги
        if (arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE - 1] != NULL)
          end_monitoring_min_max_measurement(arr_identifiers, buffer_for_save_dr_record, IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE, carrent_active_functions);

        /*
        Ми не первіряємо чи треба завершити моніторинг фіксації частоти для ЧАПВ,
        бо коли фіксується ця частота, то зразу моніторинг припиняється (ми не
        шукаємо мінімальну чи максимальну частоту - а фікчуємо частоту при запуску АПВ
        від ЧАПВ)
        */

        //Дальші дії виконуємо тіьлки у тому випадку, якщо функція end_monitoring_min_max_measurement не зафіксувала помилку і не скинула state_dr_record у STATE_DR_NO_RECORD
        if (state_dr_record != STATE_DR_NO_RECORD)
        {
          //Записуємо кількість зафіксованих максимальних струмів всіх типів
          buffer_for_save_dr_record[FIRST_INDEX_NUMBER_MIN_U_DR] = number_min_U_dr;
          buffer_for_save_dr_record[FIRST_INDEX_NUMBER_MAX_U_DR] = number_max_U_dr;

          //Помічаємо, що треба при першій же нагоді почати новий запис, бо попередній запис був примусово зупинений
          state_dr_record = STATE_DR_CUT_RECORD;

          //Скидаємо сигнал роботи дискретного реєстратора
          _CLEAR_BIT(carrent_active_functions, RANG_WORK_D_REJESTRATOR);

          //Сформований запис ставим в чергу для запису
          routine_for_queue_dr(&saving_record_dr);
          buffer_for_save_dr_record = queue_dr[head_queue_dr];
        }
        /*
        Скидаємо сигнал роботи дискретного реєстратора у масиві попередніх активних функцій
        Це ми робимо для того, щоб у першому записі нового запису було зафіксовано активацію роботу дискретного реєстратора
        */
        _CLEAR_BIT(previous_active_functions, RANG_WORK_D_REJESTRATOR);
      }
    }
  }

  //При потребі переміщуємо "хвіст" черги
  if (move_tail_queue_dr != false)
  {
    saving_record_dr = false;
    if (++tail_queue_dr >= SIZE_QUEUE_DR)
      tail_queue_dr = 0;

    move_tail_queue_dr = false;
  }

  switch (state_dr_record)
  {
    case STATE_DR_NO_RECORD:
    case STATE_DR_FORCE_START_NEW_RECORD:
      {
        //Попередньо скидаємо невизначену помилку  роботи дискретного реєстратора
        _SET_BIT(clear_diagnostyka, ERROR_DR_UNDEFINED_BIT);
        if ((saving_record_dr == false) || (tail_queue_dr != head_queue_dr))
        {
          //Ця ситуація означає, що як мінімум на один новий запис у нас є вільне місце, тому скидаємо сигналізацію про втрату даних
          _SET_BIT(clear_diagnostyka, ERROR_DR_TEMPORARY_BUSY_BIT);
        }

        //На початок аналізу покищо ще дискретний реєстратор не запущений

        //Аналізуємо, чи стоїть умова запуску дискретного реєстратора
        unsigned int cur_active_sources[N_BIG];
        for (size_t i = 0; i != N_BIG; ++i)
        {
          cur_active_sources[i] = carrent_active_functions[i] & current_settings_prt.ranguvannja_digital_registrator[i];
        }
        unsigned int comp = false;
        NOT_ZERO_OR(comp, cur_active_sources, N_BIG)
        if (
          (comp) ||
          (state_dr_record == STATE_DR_FORCE_START_NEW_RECORD))
        {
          //Є умова запуску дискретного реєстратора
          //global_timers[INDEX_TIMER_DR_WORK] = 0;//_TIMER_0_T(INDEX_TIMER_DR_WORK,current_settings_prt.timeout_prolongation_work_digital_registrator,1,1,0,0);
          //Перевіряємо, чи при початку нового запису ми не втратимо попередню інформацію
          if ((saving_record_dr == false) || (tail_queue_dr != head_queue_dr))
          {
            //Можна починати новий запис
            //.asm volatile(
            //.     "bkpt 1"
            //.     );
            //Виставляємо активну функцію
            _SET_BIT(carrent_active_functions, RANG_WORK_D_REJESTRATOR);

            //Записуємо мітку початку запису
            buffer_for_save_dr_record[FIRST_INDEX_START_START_RECORD_DR] = LABEL_START_RECORD_DR;

            //Записуємо час початку запису
            drDateTimeState = (realDateTime != 0) ? AVAT_DATE_TIMR_FIX_LEVEL2 : AVAR_DATE_TIME_NONE;

            for (size_t i = 0; i < sizeof(time_t); i++)
              buffer_for_save_dr_record[FIRST_INDEX_DATA_TIME_DR + i] = *((unsigned char *) (&time_dat) + i);
            for (size_t i = 0; i < sizeof(int32_t); i++)
              buffer_for_save_dr_record[FIRST_INDEX_DATA_TIME_DR + sizeof(time_t) + i] = *((unsigned char *) (&time_ms) + i);

            //Додаткові налаштування при яких було запущено дискретний реєстратор
            unsigned int control_extra_settings_1_tmp = current_settings_prt.control_extra_settings_1 & (MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE) | MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_IB_I04));
            unsigned char *point_to_extra_settings = (unsigned char *) (&control_extra_settings_1_tmp);
            for (unsigned int i = 0; i < sizeof(control_extra_settings_1_tmp); i++)
              buffer_for_save_dr_record[FIRST_INDEX_EXTRA_SETTINGS_DR + i] = *(point_to_extra_settings + i);

            //І'мя комірки
            for (unsigned int i = 0; i < MAX_CHAR_IN_NAME_OF_CELL; i++)
              buffer_for_save_dr_record[FIRST_INDEX_NAME_OF_CELL_DR + i] = current_settings_prt.name_of_cell[i] & 0xff;

            //Коефіцієнти трансформації
            {
              unsigned char *ptr_target = buffer_for_save_dr_record + FIRST_INDEX_C1;
              unsigned char *ptr_source = (unsigned char *) (&current_settings_prt.TCurrent1);
              for (size_t i = 0; i < sizeof(current_settings_prt.TCurrent1); ++i)
                *ptr_target++ = *ptr_source++;

              ptr_target = buffer_for_save_dr_record + FIRST_INDEX_TC2;
              ptr_source = (unsigned char *) (&current_settings_prt.TCurrent2);
              for (size_t i = 0; i < sizeof(current_settings_prt.TCurrent2); ++i)
                *ptr_target++ = *ptr_source++;

              ptr_target = buffer_for_save_dr_record + FIRST_INDEX_TV1;
              ptr_source = (unsigned char *) (&current_settings_prt.TVoltage1);
              for (size_t i = 0; i < sizeof(current_settings_prt.TVoltage1); ++i)
                *ptr_target++ = *ptr_source++;

              ptr_target = buffer_for_save_dr_record + FIRST_INDEX_TV2;
              ptr_source = (unsigned char *) (&current_settings_prt.TVoltage2);
              for (size_t i = 0; i < sizeof(current_settings_prt.TVoltage2); ++i)
                *ptr_target++ = *ptr_source++;
            }

            //Джерела запуску
            for (unsigned int i = 0; i < NUMBER_BYTES_SAMPLE_DR; i++)
              buffer_for_save_dr_record[FIRST_INDEX_SOURCE_DR + i] = *(((unsigned char *) cur_active_sources) + i);

            put_before_info_in_buf_from_queue(buffer_for_save_dr_record, &drUniqVarsAddreses);
            //put_before_info_in_buf(&drUniqVarsAddreses);FillBeforeBufinDirectOrder(&drUniqVarsAddreses);
            //Переводимо режим роботи із дискретним реєстратором у стан "Іде процес запису реєстратора"
            state_dr_record = STATE_DR_EXECUTING_RECORD;
            //Скидаємо кількість фіксацій максимальних струмів/напруг
            number_min_U_dr = 0;
            number_max_U_dr = 0;
            for (size_t i = 0; i < _NUMBER_IDENTIFIER; ++i)
              arr_identifiers[i] = NULL;
            // next_free_array = (buffer_for_save_dr_record + FIRST_INDEX_FIRST_BLOCK_DR);

            /*
            Перевірку необхідності запису фіксації вимірювань при роботі ЧАПВ здійснювати
            не потрібно, тому що ми тільки запустили дискретний реєстратор, а умова запису
            вимірювань при запуску ЧАПВ складається з двох зрізів у яких сигнал ЧАПВ
            здійснює перехід "Активний"->"Пасивний"
            */
          }
          else
          {
            //Виставляємо помилку, що є умова на роботу дискретного реєстратора тоді, як всі вільні буфери зайняті
            _SET_BIT(set_diagnostyka, ERROR_DR_TEMPORARY_BUSY_BIT);
            _SET_BIT(carrent_active_functions, RANG_DEFECT);
          }
        }
        else
          state_dr_record = STATE_DR_NO_RECORD;

        break;
      }
    case STATE_DR_EXECUTING_RECORD:
      {
        if (
          (drDateTimeState == AVAR_DATE_TIME_NONE) &&
          (realDateTime != 0))
        {
          drDateTimeState = AVAT_DATE_TIMR_FIX_LEVEL2;
          for (size_t i = 0; i < sizeof(time_t); i++)
            buffer_for_save_dr_record[FIRST_INDEX_DATA_TIME_DR + i] = *((unsigned char *) (&time_dat) + i);
          for (size_t i = 0; i < sizeof(int32_t); i++)
            buffer_for_save_dr_record[FIRST_INDEX_DATA_TIME_DR + sizeof(time_t) + i] = *((unsigned char *) (&time_ms) + i);
        }

        //Збільшуємо час з початку запуску запису
        time_from_start_record_dr += DELTA_TIME_FOR_TIMERS;

        //Включно до цього часу іде процес запису

        //Контроль-фіксація максимальних аналогових сигналів

        unsigned int comp = false;

        //Дальші дії виконуємо тіьлки у тому випадку, якщо функція end_monitoring_min_max_measurement не зафіксувала помилку і не скинула state_dr_record у STATE_DR_NO_RECORD
        if (state_dr_record != STATE_DR_NO_RECORD)
        {
          //Перевіряємо, чи ще існує умова продовження запису
          //Якщо такої умови немає - то скидаємо сигнал запущеного дискретного реєстратора, що це зафіксувати у змінених сигналах
          if (stop_regisrator(carrent_active_functions, current_settings_prt.ranguvannja_digital_registrator) != 0)
          {
            //Скидаємо сигнал роботи дискретного реєстратора
            _CLEAR_BIT(carrent_active_functions, RANG_WORK_D_REJESTRATOR);
            //.asm volatile(
            //.    "bkpt 1"
            //.    );
            //Переводимо режим роботи із дискретним реєстратором у стан "Виконується безпосередній запис у послідовну DataFlash"
            state_dr_record = STATE_DR_MAKE_RECORD;
          }

          //Перевіряємо чи відбуласа зміна сигналів у порівнянні із попереднім станом. Якщо така зміна є, то формуєм новий зріз сигналів у записі
          COMPARE_OR(comp, carrent_active_functions, previous_active_functions, N_BIG)
          if (comp)
          {
            //Теперішній стан сигналів не співпадає з попереднім станом сигналів

            //Збільшуємо на один кількість нових зрізів
            number_items_dr++;

            //Вираховуємо кількість змін сигналів
            unsigned int number_changes_into_current_item;
            _NUMBER_CHANGES_INTO_UNSIGNED_INT_ARRAY(previous_active_functions, carrent_active_functions, N_BIG, number_changes_into_current_item);
            number_changes_into_dr_record += number_changes_into_current_item;

            //Записуємо поточний cтан сигналів
            buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr * SD_DR + 0] = time_from_start_record_dr & 0xff;
            buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr * SD_DR + 1] = (time_from_start_record_dr >> 8) & 0xff;
            buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr * SD_DR + 2] = (time_from_start_record_dr >> 16) & 0xff;

            for (size_t i = 0; i < NUMBER_BYTES_SAMPLE_DR; ++i)
            {
              size_t offset = i >> 2;
              size_t shift = 8 * (i & ((1u << 2) - 1));

              buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr * SD_DR + 3 + i] = (carrent_active_functions[offset] >> shift) & 0xff;
            }
            //Кількість змін сигналів у порівнянні із попереднім станом
            buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr * SD_DR + 3 + NUMBER_BYTES_SAMPLE_DR + 0] = number_changes_into_current_item & 0xff;
            buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr * SD_DR + 3 + NUMBER_BYTES_SAMPLE_DR + 1] = (number_changes_into_current_item >> 8) & 0xff;
          }

          //Перевіряємо, чи стоїть умова завершення запису
          if (
            (state_dr_record == STATE_DR_MAKE_RECORD) ||
            (time_from_start_record_dr >= (MAX_TIME_OFFSET_FROM_START - DELTA_TIME_FOR_TIMERS + 1)) ||
            ((number_items_dr + 1) >= MAX_EVENTS_IN_ONE_RECORD) ||
            (statePowerDown == STATE_POWER_DOWN_ETAP_CUT))
          {
            //Підтверджуємо примусовий запис у енергонезалежну пам'ять через пропадання живлення
            if (statePowerDown == STATE_POWER_DOWN_ETAP_CUT)
              statePowerDown = STATE_POWER_DOWN_ETAP_CUT_CONFIRMED;

            //Немає умови продовження запису, або є умова завершення запису - завершуємо формування запису і подаємо команду на запис
            buffer_for_save_dr_record[FIRST_INDEX_NUMBER_ITEMS_DR] = number_items_dr;
            buffer_for_save_dr_record[FIRST_INDEX_NUMBER_CHANGES_DR] = number_changes_into_dr_record & 0xff;
            buffer_for_save_dr_record[FIRST_INDEX_NUMBER_CHANGES_DR + 1] = (number_changes_into_dr_record >> 8) & 0xff;

            /*
          Перевірку необхідності запису фіксація вимірювань при роботі ЧАПВ здійснювати
          не потрібно, тому що зараз іде перевірка чи треба примусово зупинити робити формування
          поточного запису. Якщо була ситуація заауску АПВ від ЧАПВ, то ця подія мала б зафіксованою бути
          у місці програми, де аналізувалися блоки інтегральних величин
          */

            //Дальші дії виконуємо тіьлки у тому випадку, якщо функція end_monitoring_min_max_measurement не зафіксувала помилку і не скинула state_dr_record у STATE_DR_NO_RECORD
            if (state_dr_record != STATE_DR_NO_RECORD)
            {
              //Записуємо кількість зафіксованих максимальних вимірювань всіх типів
              buffer_for_save_dr_record[FIRST_INDEX_NUMBER_MIN_U_DR] = number_min_U_dr;
              buffer_for_save_dr_record[FIRST_INDEX_NUMBER_MAX_U_DR] = number_max_U_dr;

              //Переводимо режим роботи із дискретним реєстратором у стан "Виконується безпосередній запис у DataFlash"
              if (state_dr_record != STATE_DR_MAKE_RECORD)
              {
                if (time_from_start_record_dr >= (MAX_TIME_OFFSET_FROM_START - DELTA_TIME_FOR_TIMERS + 1))
                {
                  //Якщо відбулося перевищення по часу запису, то подаємо команду завершити запис без продовження потім цього запису у наступному записі
                  state_dr_record = STATE_DR_MAKE_RECORD;
                }
                else
                {
                  //Якщо відбулося перевищення по досягнкнні максимальної кількості зрізів (або іншої причини, яка покищо не оговорена, але може з'явитися у майбутньому), то подаємо команду завершити запис але на наступному проході почати новий запис
                  state_dr_record = STATE_DR_CUT_RECORD;
                }
              }
            }

            //Скидаємо сигнал роботи дискретного реєстратора
            _CLEAR_BIT(carrent_active_functions, RANG_WORK_D_REJESTRATOR);
          }
        }

        break;
      }
    default:
      {
        //По ідеї сюди програма ніколи не мала б зайти
        fix_undefined_error_dr(carrent_active_functions);
        break;
      }
  }

  //Перевіряємо, чи стоїть умова сформований запис передати на запис у DataFlash
  routine_for_queue_dr(&saving_record_dr);
  PuCmdinRawBuf(carrent_active_functions);
  /*********************/
  //Формуємо попереденій стан сигналів для функції увімкнення/вимкнення
  /*********************/
  for (unsigned int i = 0; i < N_BIG; i++)
    previous_active_functions[i] = carrent_active_functions[i];
  /*********************/
}
/*****************************************************/

/*****************************************************/
//Функція обробки логіки дискретного реєстратора
/*****************************************************/
inline void analog_registrator(unsigned int *carrent_active_functions)
{
  static unsigned int prev_active_sources[N_BIG];
  unsigned int cur_active_sources[N_BIG];
  for (size_t i = 0; i < N_BIG; ++i)
    cur_active_sources[i] = carrent_active_functions[i] & current_settings_prt.ranguvannja_analog_registrator[i];

  int comp = true;

  if (forbidden_new_record_ar_mode_0 != 0)
  {
    /*
    Ця ситуація означає, що були активними джерела аналогового реєстратора, які запустили
    в роботу аналоговий реєстратор, і тепер для розблокування можливості запускати новий запис ми 
    чекаємо ситуації, що
    - всі джерела активації деактивуються 
    - появитьс новий сигнал запуску, якого не було раніше
    */
    ZERO_AND(comp, cur_active_sources, N_BIG)
    if (comp)
    {
      //Перша умова розблокування можливості початку нового запису виконана
      forbidden_new_record_ar_mode_0 = 0;
    }
    else if ((current_settings_prt.control_ar & MASKA_FOR_BIT(INDEX_ML_CTR_AR_AVAR_STATE)) == 0)
    {
      //Попередній запис повністю записаний у DataFlash, але ще деякі джерела активації не деакттивувалися
      unsigned int diff_active_sources[N_BIG];
      for (size_t i = 0; i < N_BIG; ++i)
        diff_active_sources[i] = prev_active_sources[i] ^ cur_active_sources[i];

      COMPARE_NOT_ZERO_OR(comp, diff_active_sources, cur_active_sources, N_BIG)
      if (comp)
      {
        //Друга умова розблокування можливості початку нового запису виконана
        forbidden_new_record_ar_mode_0 = 0;
      }
    }
  }

  static int prefault_number_periods_tmp;

  if (
    ((global_timers[INDEX_TIMER_FULL_AR_RECORD] >= MAX_TIME_FULL_AR_RECORD) && (_CHECK_SET_BIT(carrent_active_functions, RANG_WORK_A_REJESTRATOR) != 0)) ||
    (state_ar_record_m == STATE_AR_BLOCK_M) ||
    (state_ar_record_fatfs == STATE_AR_MEMORY_FULL_FATFS) ||
    (state_ar_record_fatfs == STATE_AR_BLOCK_FATFS))
  {
    if (global_timers[INDEX_TIMER_FULL_AR_RECORD] >= MAX_TIME_FULL_AR_RECORD)
      _SET_BIT(set_diagnostyka, ERROR_AR_TEMPORARY_BUSY_BIT);

    state_ar_record_prt = STATE_AR_BLOCK_PRT;
    global_timers[INDEX_TIMER_POSTFAULT] = -1;
    global_timers[INDEX_TIMER_FULL_AR_RECORD] = -1;
  }

  if (
    (arDateTimeState == AVAR_DATE_TIME_NONE) &&
    (state_ar_record_fatfs != STATE_AR_NONE_FATFS) &&
    (state_ar_record_fatfs != STATE_AR_BLOCK_FATFS) &&
    (realDateTime != 0))
  {
    arDateTimeState = AVAT_DATE_TIMR_FIX_LEVEL1;

    header_ar.time_dat = time_dat;
    header_ar.time_ms = time_ms;
  }

  switch (state_ar_record_prt)
  {
    case STATE_AR_NONE_PRT:
      {
        if (state_ar_record_fatfs == STATE_AR_NONE_FATFS)
        {
          global_timers[INDEX_TIMER_FULL_AR_RECORD] = -1;
          _SET_BIT(clear_diagnostyka, ERROR_AR_TEMPORARY_BUSY_BIT);
        }

        //Аналізуємо, чи стоїть умова запуску аналогового реєстратора
        NOT_ZERO_OR(comp, cur_active_sources, N_BIG)
        if (
          (
            (comp) &&
            (forbidden_new_record_ar_mode_0 == 0) /*при попередній роботі ан.реєстротора (якщо така була) вже всі джерела активації були зняті і зароз вони знову виникли*/
            ) ||
          (start_ar != 0))
        {
          //Є умова запуску аналогового реєстратора

          if (state_ar_record_fatfs != STATE_AR_STOP_WRITE_FATFS)
          {
            //Переводимо режим роботи із аналоговим реєстратором у стан "Запуск нового запису"
            if (current_settings_prt.control_ar & MASKA_FOR_BIT(INDEX_ML_CTR_AR_AVAR_STATE))
            {
              state_ar_record_prt = STATE_AR_AVAR_PRT;
            }
            else
            {
              state_ar_record_prt = STATE_AR_POSTAVAR_PRT;
              global_timers[INDEX_TIMER_POSTFAULT] = 0; //Запускаємо таймер післяаварійного процесу
            }

            if (state_ar_record_fatfs == STATE_AR_NONE_FATFS)
            {
              //запис на рівні FATFs зараз не проводиться, тому треба підготувати інформацію про умову старту нового запису

              prefault_number_periods_tmp = ((POWER_CTRL->IDR & POWER_CTRL_PIN) != (uint32_t) Bit_RESET) ? current_settings_prt.prefault_number_periods : AR_TAIL_MIN_NUMBER_PERIOD;
              global_timers[INDEX_TIMER_FULL_AR_RECORD] = 20 * prefault_number_periods_tmp; //Запускаємо таймер цілого запису  з врахуванням що буде доданий доаварійний масив

              //Записуємо мітку початку запису
              header_ar.label_start_record = LABEL_START_RECORD_AR;
              //Записуємо час початку запису
              arDateTimeState = (realDateTime != 0) ? AVAT_DATE_TIMR_FIX_LEVEL2 : AVAR_DATE_TIME_NONE;
              header_ar.time_dat = time_dat;
              header_ar.time_ms = time_ms;

              //Коефіцієнт трансформації TCurrent1
              header_ar.TCurrent1 = current_settings_prt.TCurrent1;
              //Коефіцієнт трансформації TCurrent2
              header_ar.TCurrent2 = current_settings_prt.TCurrent2;
              //Коефіцієнт трансформації TVoltage1
              header_ar.TVoltage1 = current_settings_prt.TVoltage1;
              //Коефіцієнт трансформації TVoltage2
              header_ar.TVoltage2 = current_settings_prt.TVoltage2;

              //Додаткові налаштування при яких було запущено аналоговий реєстратор
              header_ar.control_extra_settings_1 = current_settings_prt.control_extra_settings_1 & (MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_IB_I04) | (MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE)));

              //Час доаварійного масиву
              header_ar.prefault_number_periods = prefault_number_periods_tmp;

              //І'мя комірки
              for (unsigned int i = 0; i < MAX_CHAR_IN_NAME_OF_CELL; i++)
                header_ar.name_of_cell[i] = current_settings_prt.name_of_cell[i] & 0xff;
              //Сигнали, які запустили в роботу Аналоговий реєстратор
              for (size_t i = 0; i < N_BIG; i++)
                header_ar.cur_active_sources[i] = (prev_active_sources[i] ^ cur_active_sources[i]) & cur_active_sources[i];
            }
          }
          else
          {
            _SET_BIT(set_diagnostyka, ERROR_AR_TEMPORARY_BUSY_BIT);
          }
        }

        break;
      }
    case STATE_AR_AVAR_PRT:
      {
        ZERO_AND(comp, cur_active_sources, N_BIG)
        if (
          (comp) ||
          ((current_settings_prt.control_ar & MASKA_FOR_BIT(INDEX_ML_CTR_AR_AVAR_STATE)) == 0) /*може статися хіба, коли під час роботи ан.реєстратора змінено це налаштування*/
        )
        {
          state_ar_record_prt = STATE_AR_POSTAVAR_PRT;
          global_timers[INDEX_TIMER_POSTFAULT] = 0; //Запускаємо таймер післяаварійного процесу
        }

        break;
      }
    case STATE_AR_POSTAVAR_PRT:
      {
        NOT_ZERO_OR(comp, cur_active_sources, N_BIG)

        if (
          ((current_settings_prt.control_ar & MASKA_FOR_BIT(INDEX_ML_CTR_AR_AVAR_STATE)) != 0) &&
          (comp))
        {
          //Повертаємося до аварійного процесу
          state_ar_record_prt = STATE_AR_AVAR_PRT;
          global_timers[INDEX_TIMER_POSTFAULT] = -1; //Зупиняємо таймер післяаварійного процесу
        }
        else if (global_timers[INDEX_TIMER_POSTFAULT] >= (int) (20 * current_settings_prt.postfault_number_periods))
        {
          //Завершився післяаварійний процес
          global_timers[INDEX_TIMER_POSTFAULT] = -1; //Зупиняємо таймер післяаварійного процесу
          state_ar_record_prt = STATE_AR_NONE_PRT;

          if (
            /*перевірку на те, що режим "Власнеаварійний процес" увімкнутий не треба, бо при умові активних джерел ми б попали у попередню умову де з післяаваріного процесу йде поворот до аварійного процесу*/
            (comp))
          {
            forbidden_new_record_ar_mode_0 = 0xff; /*помічаємо будь-яким числом, що є активними деякі сигнали від попереднього записту*/
          }
        }

        break;
      }
    case STATE_AR_BLOCK_PRT:
      {
        //Аналізуємо чи немає умови почати новий запис поки ми не вийшли з блокованого стану
        if (
          (state_ar_record_fatfs == STATE_AR_NONE_FATFS) &&
          (state_ar_record_m == STATE_AR_NONE_M))
        {
          state_ar_record_prt = STATE_AR_NONE_PRT;

          NOT_ZERO_OR(comp, cur_active_sources, N_BIG)
          if (comp)
          {
            forbidden_new_record_ar_mode_0 = 0xff; /*помічаємо будь-яким числом, що є активними деякі сигнали від попереднього записту*/
          }
        }

        break;
      }
    default:
      {
        //Теоретично цього ніколи не мало б бути
        total_error_sw_fixed();
        break;
      }
  }

  //Виставляння/скидання функції Роботи Аналогового реєстратора
  if (
    (state_ar_record_prt == STATE_AR_AVAR_PRT) ||
    (state_ar_record_prt == STATE_AR_POSTAVAR_PRT))
  {
    _SET_BIT(carrent_active_functions, RANG_WORK_A_REJESTRATOR);
    if (truncPrefault)
    {
      truncPrefault = false;
      int trunc = 20 * (prefault_number_periods_tmp - AR_TAIL_MIN_NUMBER_PERIOD);
      if (
        (trunc > 0) &&                                       //перестраховка, бо завжди б так мало бути у цій ситуації
        (global_timers[INDEX_TIMER_FULL_AR_RECORD] >= trunc) //перестраховка, бо завжди б так мало бути у цій ситуації
      )
      {
        global_timers[INDEX_TIMER_FULL_AR_RECORD] -= trunc;
      }
    }
  }
  else
  {
    _CLEAR_BIT(carrent_active_functions, RANG_WORK_A_REJESTRATOR);
  }

  for (size_t i = 0; i != N_BIG; ++i)
    prev_active_sources[i] = cur_active_sources[i];

  start_ar = 0;
}
/*****************************************************/

/*****************************************************/
//Функція захистів з якої здійснюються всі інші операції
/*****************************************************/
//#pragma optimize=none
inline void main_protection(void)
{
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
  static unsigned int const command_permitted_from_KP[N_SMALL] =
    {
      COMMAND_PERMITTED_FROM_KP_SIGNALES_0,
      COMMAND_PERMITTED_FROM_KP_SIGNALES_1,
      COMMAND_PERMITTED_FROM_KP_SIGNALES_2};
#endif

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
  /***
  Зафіксувати чи є активними сигнали блокування Вх.GOOSE блоків і Вх.MMS блоків
  ***/
  enum WRP_GOOSE_BIT_POS
  {
    GS_BLOCK_ORD_NUM_00, //0
    GS_BLOCK_ORD_NUM_01, //1
    GS_BLOCK_ORD_NUM_02, //2
    GS_BLOCK_ORD_NUM_03, //3
    GS_BLOCK_ORD_NUM_04, //4
    GS_BLOCK_ORD_NUM_05, //5
    GS_BLOCK_ORD_NUM_06, //6
    GS_BLOCK_ORD_NUM_07, //7
    GS_BLOCK_ORD_NUM_08, //8
    GS_BLOCK_ORD_NUM_09, //9
    GS_BLOCK_ORD_NUM_10, //10
    GS_BLOCK_ORD_NUM_11, //11
    GS_BLOCK_ORD_NUM_12, //12
    GS_BLOCK_ORD_NUM_13, //13
    GS_BLOCK_ORD_NUM_14, //14
    GS_BLOCK_ORD_NUM_15, //15

    TOTAL_GOOSE_BLOCK
    //
  };
  enum WRP_MMS_BIT_POS
  {
    MMS_BLOCK_ORD_NUM_00, //0
    MMS_BLOCK_ORD_NUM_01, //1
    MMS_BLOCK_ORD_NUM_02, //2
    MMS_BLOCK_ORD_NUM_03, //3
    TOTAL_MMS_BLOCK
    //
  };
  enum WRP_LAN_BIT_POS
  {
    LAN_BLOCK_ORD_NUM_00, //0
    LAN_BLOCK_ORD_NUM_01, //1
    LAN_BLOCK_ORD_NUM_02, //2
    LAN_BLOCK_ORD_NUM_03, //3
    TOTAL_LAN_BLOCK
    //
  };

  //.typedef union lan_block_Unn{
  //.    unsigned char arCh[4];
  //.    unsigned short arSh[2];
  //.    unsigned long Lan_Vl;
  //.    struct{
  //.        struct {
  //.            unsigned int in1 : 1; //0
  //.            unsigned int in2 : 1; //1
  //.            unsigned int in3 : 1; //2
  //.            unsigned int in4 : 1; //3
  //.            unsigned int in5 : 1; //4
  //.            unsigned int in6 : 1; //5
  //.            unsigned int in7 : 1; //6
  //.            unsigned int in8 : 1; //7
  //.        } in_bool; //
  //.    unsigned char Blk;
  //.    struct {
  //.        unsigned int out1 : 1; //0
  //.        unsigned int out2 : 1; //1
  //.        unsigned int out3 : 1; //2
  //.        unsigned int out4 : 1; //3
  //.        unsigned int out5 : 1; //4
  //.        unsigned int out6 : 1; //5
  //.        unsigned int out7 : 1; //6
  //.        unsigned int out8 : 1; //7
  //.    } out_bool; //unsigned char Out;
  //.    unsigned char Res;
  //.   }Lan_Hld;
  //.
  //.} LanBlock_stt;

  typedef union mms_block_Unn
  {
    unsigned char arCh[4];
    unsigned short arSh[2];
    unsigned long Mms_Vl;
    struct
    {
      struct
      {
        unsigned int in1 : 1; //0
        unsigned int in2 : 1; //1
        unsigned int in3 : 1; //2
        unsigned int in4 : 1; //3
        unsigned int in5 : 1; //4
        unsigned int in6 : 1; //5
        unsigned int in7 : 1; //6
        unsigned int in8 : 1; //7
      } in_bool;              //
      unsigned char Blk;
      struct
      {
        unsigned int out1 : 1; //0
        unsigned int out2 : 1; //1
        unsigned int out3 : 1; //2
        unsigned int out4 : 1; //3
        unsigned int out5 : 1; //4
        unsigned int out6 : 1; //5
        unsigned int out7 : 1; //6
        unsigned int out8 : 1; //7
      } out_bool;              //unsigned char Out;
      unsigned char Res;
    } Mms_Hld;

  } MmsBlock_stt;
  typedef union goose_block_Unn
  {
    unsigned char arCh[4];
    unsigned short arSh[2];
    unsigned long GS_Vl;
    struct
    {
      struct
      {
        unsigned int in1 : 1; //0
        unsigned int in2 : 1; //1
        unsigned int in3 : 1; //2
        unsigned int in4 : 1; //3
        unsigned int in5 : 1; //4
        unsigned int in6 : 1; //5
        unsigned int in7 : 1; //6
        unsigned int in8 : 1; //7
      } in_bool;              //
      unsigned char Blk;
      struct
      {
        unsigned int out1 : 1; //0
        unsigned int out2 : 1; //1
        unsigned int out3 : 1; //2
        unsigned int out4 : 1; //3
        unsigned int out5 : 1; //4
        unsigned int out6 : 1; //5
        unsigned int out7 : 1; //6
        unsigned int out8 : 1; //7
      } out_bool;              //unsigned char Out;
      unsigned char Res;
    } GS_Hld;

  } GsBlock_stt; //
  typedef struct tag_GsBlkParam
  {
    union
    {
      unsigned char u8Ar[4];
      unsigned short u16Ar[2];
      unsigned long ulV;
      struct
      {
        unsigned char OrdNumGsBlk;
        //unsigned char Res;
      } GsBlkHdr;
    } unnV1[N_IN_GOOSE];
    //void* pAddrGsBlk[N_IN_GOOSE];
    char ch_amount_blk_src;
    char ch_amount_active_src;
    short sh_amount_gs_active_src; //use whith arrOrdNumsGsSignal
  } GsBlkParamDsc;
  typedef struct tag_MmsBlkParam
  {
    union
    {
      unsigned char u8Ar[4];
      unsigned short u16Ar[2];
      unsigned long ulV;
      struct
      {
        unsigned char OrdNumMmsBlk;
        //unsigned char Res;
      } MmsBlkHdr;
    } unnV1[N_IN_MMS];
    //void* pAddrGsBlk[N_IN_MMS];
    char ch_amount_blk_src;
    char ch_amount_active_src;
    short sh_amount_mms_active_src; //use whith arrOrdNumsGsSignal
  } MmsBlkParamDsc;
  typedef struct tag_LanBlkParam
  {
    union
    {
      unsigned char u8Ar[4];
      unsigned short u16Ar[2];
      unsigned long ulV;
      struct
      {
        unsigned char OrdNumLanBlk;
        //unsigned char Res;
      } LanBlkHdr;
    } unnV1[N_OUT_LAN];
    //void* pAddrGsBlk[N_OUT_LAN];

  } LanBlkParamDsc;

  static GsBlock_stt arrGsBlk[N_IN_GOOSE];
  static MmsBlock_stt arrMmsBlk[N_IN_MMS];
  //static LanBlock_stt arrLanBlk[N_OUT_LAN ];

  static GsBlkParamDsc hldGsBlkParam;
  static MmsBlkParamDsc hldMmsBlkParam;
  static LanBlkParamDsc hldLanBlkParam = {
    LAN_BLOCK_ORD_NUM_00, 0, 0, 0, LAN_BLOCK_ORD_NUM_01, 0, 0, 0, LAN_BLOCK_ORD_NUM_02, 0, 0, 0, LAN_BLOCK_ORD_NUM_03, 0, 0, 0};

  static short arrOrdNumsGsSignal[N_IN_GOOSE * N_IN_GOOSE_MMS_OUT];
  static short arrOrdNumsMmsSignal[N_IN_MMS * N_IN_GOOSE_MMS_OUT];

  struct
  {
    char ch_while_breaker;
    char ch_amount_blk_src, ch_amount_mms_blk_src;
    char ch_amount_active_src, ch_amount_mms_active_src;
    char ch_amount_GsSignal, ch_amount_MmsSignal;
    __SETTINGS *p_current_settings_prt;
    unsigned int *p_active_functions;

  } sLV;
  sLV.p_active_functions = &active_functions[0];
  //Check GOOSE Blk Cmd
  //RANG_BLOCK_IN_GOOSE1
  //=====================================================================================================
  //''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
  //     GOOSE BLOCK
  //....................................................................................................
  //=====================================================================================================
  do
  {
    sLV.ch_amount_active_src = sLV.ch_amount_blk_src = sLV.ch_while_breaker = 0; //! optimize then
    sLV.ch_amount_GsSignal = sLV.ch_amount_MmsSignal = 0;
    register long i, lV;
    //register void* pv;
    register unsigned int *r_p_active_functions;
    // ----------------    -------------------------
    //    i = (RANG_BLOCK_IN_GOOSE1>>5);               <- old code
    //    lV = sLV.p_active_functions[i];//whole word  <- old code
    //    sLV.wrp.lVl = lV;                            <- old code
    //pv = (void*) sLV.p_active_functions;
    r_p_active_functions = sLV.p_active_functions;

    if (_CHECK_SET_BIT(((unsigned int *) r_p_active_functions), RANG_BLOCK_IN_GOOSE1) != 0)
    {
      //clr block
      i = GS_BLOCK_ORD_NUM_15 - sLV.ch_amount_blk_src;
      hldGsBlkParam.unnV1[i].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_00;
      sLV.ch_amount_blk_src++;

      arrGsBlk[GS_BLOCK_ORD_NUM_00].GS_Hld.Blk = 1;
    }
    else
    {
      lV = sLV.ch_amount_active_src;
      hldGsBlkParam.unnV1[lV].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_00;
      sLV.ch_amount_active_src++;
      arrGsBlk[GS_BLOCK_ORD_NUM_00].GS_Hld.Blk = 0;
    }

    if (_CHECK_SET_BIT(((unsigned int *) r_p_active_functions), (RANG_BLOCK_IN_GOOSE1 + 1)) != 0)
    {
      //clr block
      i = GS_BLOCK_ORD_NUM_15 - sLV.ch_amount_blk_src;
      hldGsBlkParam.unnV1[i].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_01;
      sLV.ch_amount_blk_src++;
      arrGsBlk[GS_BLOCK_ORD_NUM_01].GS_Hld.Blk = 1;
    }
    else
    {
      //Insert
      lV = sLV.ch_amount_active_src;
      hldGsBlkParam.unnV1[lV].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_01;
      sLV.ch_amount_active_src++;
      arrGsBlk[GS_BLOCK_ORD_NUM_01].GS_Hld.Blk = 0;
    }
    if (_CHECK_SET_BIT(((unsigned int *) r_p_active_functions), (RANG_BLOCK_IN_GOOSE1 + 2)) != 0)
    {
      //clr block
      i = GS_BLOCK_ORD_NUM_15 - sLV.ch_amount_blk_src;
      hldGsBlkParam.unnV1[i].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_02;
      sLV.ch_amount_blk_src++;
      arrGsBlk[GS_BLOCK_ORD_NUM_02].GS_Hld.Blk = 1;
    }
    else
    {
      lV = sLV.ch_amount_active_src;
      hldGsBlkParam.unnV1[lV].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_02;
      sLV.ch_amount_active_src++;
      arrGsBlk[GS_BLOCK_ORD_NUM_02].GS_Hld.Blk = 0;
    }
    if (_CHECK_SET_BIT(((unsigned int *) r_p_active_functions), (RANG_BLOCK_IN_GOOSE1 + 3)) != 0)
    {
      //clr block
      i = GS_BLOCK_ORD_NUM_15 - sLV.ch_amount_blk_src;
      hldGsBlkParam.unnV1[i].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_03;
      sLV.ch_amount_blk_src++;
      arrGsBlk[GS_BLOCK_ORD_NUM_03].GS_Hld.Blk = 1;
    }
    else
    {
      lV = sLV.ch_amount_active_src;
      hldGsBlkParam.unnV1[lV].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_03;
      sLV.ch_amount_active_src++;
      arrGsBlk[GS_BLOCK_ORD_NUM_03].GS_Hld.Blk = 0;
    }
    if (_CHECK_SET_BIT(((unsigned int *) r_p_active_functions), (RANG_BLOCK_IN_GOOSE1 + 4)) != 0)
    {
      //clr block
      i = GS_BLOCK_ORD_NUM_15 - sLV.ch_amount_blk_src;
      hldGsBlkParam.unnV1[i].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_04;
      sLV.ch_amount_blk_src++;
      arrGsBlk[GS_BLOCK_ORD_NUM_04].GS_Hld.Blk = 1;
    }
    else
    {
      lV = sLV.ch_amount_active_src;
      hldGsBlkParam.unnV1[lV].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_04;
      sLV.ch_amount_active_src++;
      arrGsBlk[GS_BLOCK_ORD_NUM_04].GS_Hld.Blk = 0;
    }
    if (_CHECK_SET_BIT(((unsigned int *) r_p_active_functions), (RANG_BLOCK_IN_GOOSE1 + 5)) != 0)
    {
      //clr block
      i = GS_BLOCK_ORD_NUM_15 - sLV.ch_amount_blk_src;
      hldGsBlkParam.unnV1[i].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_05;
      sLV.ch_amount_blk_src++;
      arrGsBlk[GS_BLOCK_ORD_NUM_05].GS_Hld.Blk = 1;
    }
    else
    {
      lV = sLV.ch_amount_active_src;
      hldGsBlkParam.unnV1[lV].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_05;
      sLV.ch_amount_active_src++;
      arrGsBlk[GS_BLOCK_ORD_NUM_05].GS_Hld.Blk = 0;
    }
    if (_CHECK_SET_BIT(((unsigned int *) r_p_active_functions), (RANG_BLOCK_IN_GOOSE1 + 6)) != 0)
    {
      //clr block
      i = GS_BLOCK_ORD_NUM_15 - sLV.ch_amount_blk_src;
      hldGsBlkParam.unnV1[i].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_06;
      sLV.ch_amount_blk_src++;
      arrGsBlk[GS_BLOCK_ORD_NUM_06].GS_Hld.Blk = 1;
    }
    else
    {
      lV = sLV.ch_amount_active_src;
      hldGsBlkParam.unnV1[lV].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_06;
      sLV.ch_amount_active_src++;
      arrGsBlk[GS_BLOCK_ORD_NUM_06].GS_Hld.Blk = 0;
    }
    if (_CHECK_SET_BIT(((unsigned int *) r_p_active_functions), (RANG_BLOCK_IN_GOOSE1 + 7)) != 0)
    {
      //clr block
      i = GS_BLOCK_ORD_NUM_15 - sLV.ch_amount_blk_src;
      hldGsBlkParam.unnV1[i].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_07;
      sLV.ch_amount_blk_src++;
      arrGsBlk[GS_BLOCK_ORD_NUM_07].GS_Hld.Blk = 1;
    }
    else
    {
      lV = sLV.ch_amount_active_src;
      hldGsBlkParam.unnV1[lV].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_07;
      sLV.ch_amount_active_src++;
      arrGsBlk[GS_BLOCK_ORD_NUM_07].GS_Hld.Blk = 0;
    }
    if (_CHECK_SET_BIT(((unsigned int *) r_p_active_functions), (RANG_BLOCK_IN_GOOSE1 + 8)) != 0)
    {
      //clr block
      i = GS_BLOCK_ORD_NUM_15 - sLV.ch_amount_blk_src;
      hldGsBlkParam.unnV1[i].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_08;
      sLV.ch_amount_blk_src++;
      arrGsBlk[GS_BLOCK_ORD_NUM_08].GS_Hld.Blk = 1;
    }
    else
    {
      lV = sLV.ch_amount_active_src;
      hldGsBlkParam.unnV1[lV].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_08;
      sLV.ch_amount_active_src++;
      arrGsBlk[GS_BLOCK_ORD_NUM_08].GS_Hld.Blk = 0;
    }
    if (_CHECK_SET_BIT(((unsigned int *) r_p_active_functions), (RANG_BLOCK_IN_GOOSE1 + 9)) != 0)
    {
      //clr block
      i = GS_BLOCK_ORD_NUM_15 - sLV.ch_amount_blk_src;
      hldGsBlkParam.unnV1[i].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_09;
      sLV.ch_amount_blk_src++;
      arrGsBlk[GS_BLOCK_ORD_NUM_09].GS_Hld.Blk = 1;
    }
    else
    {
      lV = sLV.ch_amount_active_src;
      hldGsBlkParam.unnV1[lV].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_09;
      sLV.ch_amount_active_src++;
      arrGsBlk[GS_BLOCK_ORD_NUM_09].GS_Hld.Blk = 0;
    }
    if (_CHECK_SET_BIT(((unsigned int *) r_p_active_functions), (RANG_BLOCK_IN_GOOSE1 + 10)) != 0)
    {
      //clr block
      i = GS_BLOCK_ORD_NUM_15 - sLV.ch_amount_blk_src;
      hldGsBlkParam.unnV1[i].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_10;
      sLV.ch_amount_blk_src++;
      arrGsBlk[GS_BLOCK_ORD_NUM_10].GS_Hld.Blk = 1;
    }
    else
    {
      lV = sLV.ch_amount_active_src;
      hldGsBlkParam.unnV1[lV].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_10;
      sLV.ch_amount_active_src++;
      arrGsBlk[GS_BLOCK_ORD_NUM_10].GS_Hld.Blk = 0;
    }
    if (_CHECK_SET_BIT(((unsigned int *) r_p_active_functions), (RANG_BLOCK_IN_GOOSE1 + 11)) != 0)
    {
      //clr block
      i = GS_BLOCK_ORD_NUM_15 - sLV.ch_amount_blk_src;
      hldGsBlkParam.unnV1[i].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_11;
      sLV.ch_amount_blk_src++;
      arrGsBlk[GS_BLOCK_ORD_NUM_11].GS_Hld.Blk = 1;
    }
    else
    {
      lV = sLV.ch_amount_active_src;
      hldGsBlkParam.unnV1[lV].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_11;
      sLV.ch_amount_active_src++;
      arrGsBlk[GS_BLOCK_ORD_NUM_11].GS_Hld.Blk = 0;
    }
    if (_CHECK_SET_BIT(((unsigned int *) r_p_active_functions), (RANG_BLOCK_IN_GOOSE1 + 12)) != 0)
    {
      //clr block
      i = GS_BLOCK_ORD_NUM_15 - sLV.ch_amount_blk_src;
      hldGsBlkParam.unnV1[i].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_12;
      sLV.ch_amount_blk_src++;
      arrGsBlk[GS_BLOCK_ORD_NUM_12].GS_Hld.Blk = 1;
    }
    else
    {
      lV = sLV.ch_amount_active_src;
      hldGsBlkParam.unnV1[lV].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_12;
      sLV.ch_amount_active_src++;
      arrGsBlk[GS_BLOCK_ORD_NUM_12].GS_Hld.Blk = 0;
    }
    if (_CHECK_SET_BIT(((unsigned int *) r_p_active_functions), (RANG_BLOCK_IN_GOOSE1 + 13)) != 0)
    {
      //clr block
      i = GS_BLOCK_ORD_NUM_15 - sLV.ch_amount_blk_src;
      hldGsBlkParam.unnV1[i].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_13;
      sLV.ch_amount_blk_src++;
      arrGsBlk[GS_BLOCK_ORD_NUM_13].GS_Hld.Blk = 1;
    }
    else
    {
      lV = sLV.ch_amount_active_src;
      hldGsBlkParam.unnV1[lV].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_13;
      sLV.ch_amount_active_src++;
      arrGsBlk[GS_BLOCK_ORD_NUM_13].GS_Hld.Blk = 0;
    }
    if (_CHECK_SET_BIT(((unsigned int *) r_p_active_functions), (RANG_BLOCK_IN_GOOSE1 + 14)) != 0)
    {
      //clr block
      i = GS_BLOCK_ORD_NUM_15 - sLV.ch_amount_blk_src;
      hldGsBlkParam.unnV1[i].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_14;
      sLV.ch_amount_blk_src++;
      arrGsBlk[GS_BLOCK_ORD_NUM_14].GS_Hld.Blk = 1;
    }
    else
    {
      lV = sLV.ch_amount_active_src;
      hldGsBlkParam.unnV1[lV].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_14;
      sLV.ch_amount_active_src++;
      arrGsBlk[GS_BLOCK_ORD_NUM_14].GS_Hld.Blk = 0;
    }
    if (_CHECK_SET_BIT(((unsigned int *) r_p_active_functions), (RANG_BLOCK_IN_GOOSE1 + 15)) != 0)
    {
      //clr block
      i = GS_BLOCK_ORD_NUM_15 - sLV.ch_amount_blk_src;
      hldGsBlkParam.unnV1[i].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_15;
      sLV.ch_amount_blk_src++;
      arrGsBlk[GS_BLOCK_ORD_NUM_15].GS_Hld.Blk = 1;
    }
    else
    {
      lV = sLV.ch_amount_active_src;
      hldGsBlkParam.unnV1[lV].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_15;
      sLV.ch_amount_active_src++;
      arrGsBlk[GS_BLOCK_ORD_NUM_15].GS_Hld.Blk = 0;
    }
    hldGsBlkParam.ch_amount_blk_src = sLV.ch_amount_blk_src;
    hldGsBlkParam.ch_amount_active_src = sLV.ch_amount_active_src;

  } while (sLV.ch_while_breaker);
  //
  //--------------------------------------------------------------------------------------------------------
  //````````````````````````````````````````````````````````````````````````````````````````````````````````

  //=====================================================================================================
  //''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
  //             MMS BLOCK
  //....................................................................................................
  //=====================================================================================================
  do
  {
    sLV.ch_while_breaker = 0;
    sLV.ch_amount_mms_blk_src = sLV.ch_amount_mms_active_src = 0; //! optimize then
    register long i, lV;
    register void *pv;

    register unsigned int *r_p_active_functions;
    // ----------------    -------------------------
    r_p_active_functions = sLV.p_active_functions;
    pv = (void *) &hldMmsBlkParam;
    if (_CHECK_SET_BIT(((unsigned int *) r_p_active_functions), (RANG_BLOCK_IN_MMS1 + ((unsigned int) MMS_BLOCK_ORD_NUM_00))) != 0)
    {
      //clr block
      i = MMS_BLOCK_ORD_NUM_03 - sLV.ch_amount_mms_blk_src;
      ((MmsBlkParamDsc *) pv)->unnV1[i].MmsBlkHdr.OrdNumMmsBlk = MMS_BLOCK_ORD_NUM_00;
      sLV.ch_amount_mms_blk_src++;
      arrMmsBlk[MMS_BLOCK_ORD_NUM_00].Mms_Hld.Blk = 1;
    }
    else
    {
      lV = sLV.ch_amount_mms_active_src;
      //hldGsBlkParam.pAddrGsBlk[lV] = (void*)(&arrMmsBlk[GS_BLOCK_ORD_NUM_00]);
      hldMmsBlkParam.unnV1[lV].MmsBlkHdr.OrdNumMmsBlk = MMS_BLOCK_ORD_NUM_00;
      //hldMmsBlkParam.unnV1[MMS_BLOCK_ORD_NUM_00].u8Ar[] =
      //arrMmsBlk[MMS_BLOCK_ORD_NUM_00].arCh[2] = 0;
      sLV.ch_amount_mms_active_src++;
      arrMmsBlk[MMS_BLOCK_ORD_NUM_00].Mms_Hld.Blk = 0;
    }

    if (_CHECK_SET_BIT(((unsigned int *) r_p_active_functions), (RANG_BLOCK_IN_MMS1 + ((unsigned int) MMS_BLOCK_ORD_NUM_01))) != 0)
    {
      //clr block
      i = MMS_BLOCK_ORD_NUM_03 - sLV.ch_amount_mms_blk_src;
      hldMmsBlkParam.unnV1[i].MmsBlkHdr.OrdNumMmsBlk = MMS_BLOCK_ORD_NUM_01;
      sLV.ch_amount_mms_blk_src++;
      arrMmsBlk[MMS_BLOCK_ORD_NUM_01].Mms_Hld.Blk = 1;
    }
    else
    {
      lV = sLV.ch_amount_mms_active_src;
      //hldGsBlkParam.pAddrGsBlk[lV] = (void*)(&arrMmsBlk[GS_BLOCK_ORD_NUM_00]);
      hldMmsBlkParam.unnV1[lV].MmsBlkHdr.OrdNumMmsBlk = MMS_BLOCK_ORD_NUM_01;
      sLV.ch_amount_mms_active_src++;
      arrMmsBlk[MMS_BLOCK_ORD_NUM_01].Mms_Hld.Blk = 0;
    }

    if (_CHECK_SET_BIT(((unsigned int *) r_p_active_functions), (RANG_BLOCK_IN_MMS1 + ((unsigned int) MMS_BLOCK_ORD_NUM_02))) != 0)
    {
      //clr block
      i = MMS_BLOCK_ORD_NUM_03 - sLV.ch_amount_mms_blk_src;
      hldMmsBlkParam.unnV1[i].MmsBlkHdr.OrdNumMmsBlk = MMS_BLOCK_ORD_NUM_02;
      sLV.ch_amount_mms_blk_src++;
      arrMmsBlk[MMS_BLOCK_ORD_NUM_02].Mms_Hld.Blk = 1;
    }
    else
    {
      lV = sLV.ch_amount_mms_active_src;
      //hldGsBlkParam.pAddrGsBlk[lV] = (void*)(&arrMmsBlk[GS_BLOCK_ORD_NUM_00]);
      hldMmsBlkParam.unnV1[lV].MmsBlkHdr.OrdNumMmsBlk = MMS_BLOCK_ORD_NUM_02;
      sLV.ch_amount_mms_active_src++;
      arrMmsBlk[MMS_BLOCK_ORD_NUM_02].Mms_Hld.Blk = 0;
    }

    if (_CHECK_SET_BIT(((unsigned int *) r_p_active_functions), (RANG_BLOCK_IN_MMS1 + ((unsigned int) MMS_BLOCK_ORD_NUM_03))) != 0)
    {
      //clr block
      i = MMS_BLOCK_ORD_NUM_03 - sLV.ch_amount_mms_blk_src;
      hldMmsBlkParam.unnV1[i].MmsBlkHdr.OrdNumMmsBlk = MMS_BLOCK_ORD_NUM_03;
      sLV.ch_amount_mms_blk_src++;
      arrMmsBlk[MMS_BLOCK_ORD_NUM_03].Mms_Hld.Blk = 1;
    }
    else
    {
      lV = sLV.ch_amount_mms_active_src;
      //hldGsBlkParam.pAddrGsBlk[lV] = (void*)(&arrMmsBlk[GS_BLOCK_ORD_NUM_00]);
      hldMmsBlkParam.unnV1[lV].MmsBlkHdr.OrdNumMmsBlk = MMS_BLOCK_ORD_NUM_03;
      sLV.ch_amount_mms_active_src++;
      arrMmsBlk[MMS_BLOCK_ORD_NUM_03].Mms_Hld.Blk = 0;
    }

    hldMmsBlkParam.ch_amount_blk_src = sLV.ch_amount_blk_src;
    hldMmsBlkParam.ch_amount_active_src = sLV.ch_amount_active_src;
    if (_CHECK_SET_BIT(((unsigned int *) r_p_active_functions), (RANG_MISCEVE_DYSTANCIJNE)) != 1)
      hldMmsBlkParam.ch_amount_blk_src |= 0x80;

  } while (sLV.ch_while_breaker);
  //
  //--------------------------------------------------------------------------------------------------------
  //````````````````````````````````````````````````````````````````````````````````````````````````````````

  /***/
#endif

  copying_active_functions = 1; //Помічаємо, що зараз обновляємо значення активних функцій

  //Скижаємо ті сигнали, які відповідають за входи, кнопки і активацію з інтерфейсу
  static unsigned int const maska_input_signals[N_BIG] =
    {
      MASKA_FOR_INPUT_SIGNALS_0,
      MASKA_FOR_INPUT_SIGNALS_1,
      MASKA_FOR_INPUT_SIGNALS_2,
      MASKA_FOR_INPUT_SIGNALS_3,
      MASKA_FOR_INPUT_SIGNALS_4,
      MASKA_FOR_INPUT_SIGNALS_5,
      MASKA_FOR_INPUT_SIGNALS_6};
  for (unsigned int i = 0; i < N_BIG; i++)
    active_functions[i] &= (unsigned int) (~maska_input_signals[i]);

  /**************************/
  //Перевірка, чи треба очистити трігерні функції
  /**************************/
  if (reset_trigger_function_from_interface != 0)
  {
    if ((reset_trigger_function_from_interface & (1 << USB_RECUEST)) != 0)
    {
      for (unsigned int i = 0; i < N_BIG; i++)
        trigger_functions_USB[i] = 0;

      information_about_restart_counter &= (unsigned int) (~(1 << USB_RECUEST));
    }
    if ((reset_trigger_function_from_interface & (1 << RS485_RECUEST)) != 0)
    {
      for (unsigned int i = 0; i < N_BIG; i++)
        trigger_functions_RS485[i] = 0;

      information_about_restart_counter &= (unsigned int) (~(1 << RS485_RECUEST));
    }
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
    if ((reset_trigger_function_from_interface & (1 << LAN_RECUEST)) != 0)
    {
      for (unsigned int i = 0; i < N_BIG; i++)
        trigger_functions_LAN[i] = 0;

      information_about_restart_counter &= (unsigned int) (~(1 << LAN_RECUEST));
      information_about_clean_energy &= (unsigned int) (~(1 << LAN_RECUEST));
    }
#endif

    //Помічаємо що ми виконали очистку по ВСІХ інтерфейсах
    reset_trigger_function_from_interface = 0;
  }
  /**************************/

  unsigned int active_inputs_grupa_ustavok = 0;
  /**************************/
  //Опрацьовуємо ФК, дискретні входи і верхній рівень
  /**************************/
  //Опрцьовуємо логіку натиснутих кнопок
  uint32_t pressed_buttons_tmp = 0;
  uint32_t buttons_mode_tmp = current_settings_prt.buttons_mode;
  if (
    (mutex_buttons == false) &&
    (pressed_buttons != 0))
  {
    pressed_buttons_tmp = pressed_buttons;
    //Очищаємо натиснуті кнопкb, інформацію про яких ми вже забрали у роботу (аде ще не опрацювали)
    pressed_buttons = 0;
  }

  uint32_t pressed_buttons_switcher = pressed_buttons_tmp & buttons_mode_tmp; /*натиснуті в даний момент кнопки-ключі*/
  pressed_buttons_tmp &= (uint32_t)(~buttons_mode_tmp);                       /*натиснуті в даний момент всі інші типи кнопок*/

  //Опрацьвуємо спочатку кнопки-ключі
  uint32_t const fix_active_buttons_tmp = (_CHECK_SET_BIT(active_functions, RANG_AVAR_DEFECT) == 0) ? ((fix_active_buttons ^ pressed_buttons_switcher) & buttons_mode_tmp) : fix_active_buttons;

  if (
    (_CHECK_SET_BIT(active_functions, RANG_AVAR_DEFECT) == 0) &&
    (fix_active_buttons_tmp != fix_active_buttons))
  {
    fix_active_buttons = fix_active_buttons_tmp;
    /*
    Змінилися стани кнопок-ключів
    */
    _SET_BIT(control_spi1_taskes, TASK_START_WRITE_TRG_FUNC_EEPROM_BIT);
  }

  //Формуємо стан натиснутих кнопок з урахуванням нового стану копок клічів і інших кнопок
  uint32_t pressed_buttons_united = pressed_buttons_tmp | fix_active_buttons_tmp;

  //Світлова індикація натиснутих кнопок-ключів
  for (size_t i = 0; i < NUMBER_DEFINED_BUTTONS; i++)
  {
    uint32_t maska_yellow = ((1 << (i * NUMBER_LED_COLOR + LED_COLOR_RED_BIT)) | (1 << (i * NUMBER_LED_COLOR + LED_COLOR_GREEN_BIT)));
    if (((buttons_mode_tmp >> i) & 0x1) == BUTTON_MODE_SWITCHER)
    {
      if (((fix_active_buttons_tmp >> i) & 0x1) != 0)
      {
#ifndef KEYBOARD_VER_2_1
        state_leds_Fx[0] &= (uint32_t)(~maska_yellow);
        state_leds_Fx[1] |= maska_yellow;
#else
        if (i != (4 - 1))
        {
          state_leds_Fx[0] &= (uint32_t)(~maska_yellow);
          state_leds_Fx[1] |= maska_yellow;
        }
        else
        {
          state_leds_Fx[1] &= (uint32_t)(~maska_yellow);
          state_leds_Fx[0] |= maska_yellow;
        }
#endif
      }
      else
      {
#ifndef KEYBOARD_VER_2_1
        state_leds_Fx[1] &= (uint32_t)(~maska_yellow);
        state_leds_Fx[0] |= maska_yellow;
#else
        if (i != (4 - 1))
        {
          state_leds_Fx[1] &= (uint32_t)(~maska_yellow);
          state_leds_Fx[0] |= maska_yellow;
        }
        else
        {
          state_leds_Fx[0] &= (uint32_t)(~maska_yellow);
          state_leds_Fx[1] |= maska_yellow;
        }
#endif
      }
    }
    else
    {
      state_leds_Fx[0] &= (uint32_t)(~maska_yellow);
      state_leds_Fx[1] &= (uint32_t)(~maska_yellow);
    }
  }

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
  /***
  Зафіксувати чи є активними виходи Вх.GOOSE блоків (з врахуванням, що вхід міг бути активований попердньо а зараз утримуєтьсґя у активному стані) і Вх.MMS блоків
  ***/

  //=====================================================================================================
  //''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
  //    state_outs_copy = state_outs;
  //    state_outs_copy &= Input_ctrl_In_GOOSE_block;//First clear changed bits
  //    j = state_outs^Input_In_GOOSE_block;//Select changed position
  //    l = j"Select changed position" & Input_In_GOOSE_block;//Select Ones in bits
  //    state_outs_copy |= l&Input_In_GOOSE_block;//Move changed ones; <-have Error
  //....................................................................................................
  //=====================================================================================================
  do
  {
    // ----------------    -------------------------

    register long i, lV, j;
    register long lIdxBlk;

    register void *pvl;

    pvl = (void *) &hldGsBlkParam;
    sLV.ch_amount_active_src = i = N_IN_GOOSE; //hldGsBlkParam.ch_amount_active_src;
    sLV.ch_amount_GsSignal = 0;
    if (i > 0)
    {
      ((GsBlkParamDsc *) pvl)->sh_amount_gs_active_src = lV = lIdxBlk = 0;
      asm("nop" ::"r"(lIdxBlk), "r"(lV));
      while (lIdxBlk < N_IN_GOOSE)
      { //do{
        //--i = ((GsBlkParamDsc*)pvl)-> unnV1[lIdxBlk].GsBlkHdr.OrdNumGsBlk;;
        //--//Check State Inputs                                            ;
        //--lV = arrGsBlk[i].arCh[2];//Outs                                 ;
        i = lIdxBlk;
        lV = arrGsBlk[i].arCh[2]; //!!Outs arrGsBlk[i].arCh[0]

        lV &= ~((unsigned long) (Input_ctrl_In_GOOSE_block[i]));      //Clear all changeble bits
        lV |= Input_In_GOOSE_block[i] & Input_ctrl_In_GOOSE_block[i]; //Set ones only
                                                                      //if(noerror)
        arrGsBlk[i].arCh[2] = lV;                                     //arrGsBlk[i].arCh[0]
        if (arrGsBlk[i].GS_Hld.Blk == 0)
        {
          register unsigned long wrpLV;
          wrpLV = arrGsBlk[i].arCh[2]; //!!arrGsBlk[i].arCh[2] = arrGsBlk[i].arCh[0]
          j = 0;
          lV = ((GsBlkParamDsc *) pvl)->sh_amount_gs_active_src;
          while ((wrpLV != 0))
          { //&& j < 8
            if ((wrpLV & (1 << j)) != 0)
            {
              arrOrdNumsGsSignal[lV] = (lIdxBlk << 3) + j;
              lV++;
              wrpLV &= ~(1 << j);
            }
            j++;
          }
          ((GsBlkParamDsc *) pvl)->sh_amount_gs_active_src = lV;
        }
        lIdxBlk++; //  += lV;//! ????
      }            //while(lIdxBlk);//(--sLV.ch_amount_active_src) != 0
      sLV.ch_amount_GsSignal = ((GsBlkParamDsc *) pvl)->sh_amount_gs_active_src;
    }
    //Try bield list of outs which sets in ones.

  } while (sLV.ch_while_breaker);
  //
  //--------------------------------------------------------------------------------------------------------
  //````````````````````````````````````````````````````````````````````````````````````````````````````````

  //=====================================================================================================
  //''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
  //
  //
  //
  //
  //
  //....................................................................................................
  //=====================================================================================================
  do
  {
    // ----------------    -------------------------
    register long i, lV, j;
    register long lIdxBlk;
    register void *pvl;
    pvl = (void *) &hldMmsBlkParam;
    sLV.ch_amount_active_src = i = hldMmsBlkParam.ch_amount_active_src;
    if (i > 0)
    {
      ((MmsBlkParamDsc *) pvl)->sh_amount_mms_active_src = lV = lIdxBlk = 0;
      asm("nop" ::"r"(lIdxBlk), "r"(lV));
      while (lIdxBlk < N_IN_MMS)
      { //do{
        //--;
        //--;
        //--;
        i = lIdxBlk; //.i = ((MmsBlkParamDsc*)pvl)-> unnV1[lIdxBlk].MmsBlkHdr.OrdNumMmsBlk;
        //Check State Inputs
        lV = arrMmsBlk[i].arCh[2];                                //Outs
        lV &= ~((unsigned long) (Input_In_MMS_block[i]));         //Clear all changeble bits
        lV |= Input_In_MMS_block[i] & Input_ctrl_In_MMS_block[i]; //Set ones only
                                                                  //if(noerror)
        arrMmsBlk[i].arCh[2] = lV;                                //
        if (arrMmsBlk[i].Mms_Hld.Blk == 0 && (((MmsBlkParamDsc *) pvl)->ch_amount_blk_src & 0x80))
        {
          register unsigned long wrpLV;

          wrpLV = lV; //arrMmsBlk[i].arCh[2];
          j = 0;
          lV = ((MmsBlkParamDsc *) pvl)->sh_amount_mms_active_src;
          while ((wrpLV != 0))
          { //&& j < 8
            if ((wrpLV & (1 << j)) != 0)
            {
              arrOrdNumsMmsSignal[lV] = (lIdxBlk << 3) + j;
              lV++;
              wrpLV &= ~(1 << j);
            }
            j++;
          }
        }
        ((MmsBlkParamDsc *) pvl)->sh_amount_mms_active_src = lV;
        lIdxBlk++; //  += lV;//! ????
      }            //while(lIdxBlk);//(--sLV.ch_amount_active_src) != 0
      sLV.ch_amount_MmsSignal = ((MmsBlkParamDsc *) pvl)->sh_amount_mms_active_src;
    }

    //Try bield list of outs which sets in ones.

  } while (sLV.ch_while_breaker);
  //
  //--------------------------------------------------------------------------------------------------------
  //````````````````````````````````````````````````````````````````````````````````````````````````````````

  /***/
#endif

  //Перевіряємо чи є зараз активні входи
  active_inputs_lock_guard_prt = 0xff;
  unsigned int const active_inputs_local = active_inputs_prt;
  active_inputs_lock_guard_prt = 0x0;

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
  int flagCommandKP = false;
  unsigned int command_from_KP[N_SMALL];
  for (size_t m = 0; m < N_SMALL; ++m)
  {
    command_from_KP[m] = command_permitted_from_KP[m] & activation_function_from_KP[m] & ctrl_activation_function_from_KP[m];
    activation_function_from_KP[m] = 0;
    ctrl_activation_function_from_KP[m] = 0;

    if (command_from_KP[m] != 0)
      flagCommandKP = true;
  }
#endif

  _CLEAR_BIT(active_functions, RANG_USB_ACTIVE);
  _CLEAR_BIT(active_functions, RANG_RS485_ACTIVE);
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
  _CLEAR_BIT(active_functions, RANG_LAN_ACTIVE);
#endif

  unsigned int active_functions_bi_small_tmp[N_SMALL];
  unsigned int active_functions_fc_small_tmp[N_SMALL];

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
  unsigned int active_functions_goose_small_tmp[N_SMALL];
#endif
  for (size_t i = 0; i < N_SMALL; ++i)
  {
    active_functions_bi_small_tmp[i] = 0;
    active_functions_fc_small_tmp[i] = 0;

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
    active_functions_goose_small_tmp[i] = 0;
#endif
  }

  int flag = 0;
  for (int m = 0; m < N_SMALL; m++)
    if (activation_function_from_interface[m] != 0)
    {
      flag = 1;
      break;
    }
  if (
    (pressed_buttons_united != 0) ||
    ((mutex_interface == false) &&
     (flag) &&
     (dostup_z_VR)) ||
    (active_inputs_local != 0)
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
    /* якщо є активація виходів від Вх.GOOSE блоків і Вх.MMS блоків*/
    || ((sLV.ch_amount_MmsSignal + sLV.ch_amount_GsSignal) > 0) || (flagCommandKP)
#endif
  )
  {
    unsigned int temp_value_for_activated_function_button_interface[N_SMALL];
    unsigned int temp_value_for_activated_function[N_SMALL];
    for (int m = 0; m < N_SMALL; m++)
    {
      temp_value_for_activated_function_button_interface[m] = 0;
      temp_value_for_activated_function[m] = 0;
    }

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
    /***
    Опрацювати логіку Вх.GOOSE блоків і Вх.MMS блоків і виставити сигнали, які ними активуються
    ***/

    /**************************/
    //Опрацьовуємо входи для Вх.GOOSE блоків і виставити потрібний біт у temp_value_for_activated_function масиві
    /**************************/
    /**************************/
    //=====================================================================================================
    //''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
    //
    //....................................................................................................
    //=====================================================================================================
    do
    {
      sLV.ch_while_breaker = 0;
      // ----------------    -------------------------
      //        register unsigned long i,lV,j;
      //         unsigned long lCtrGsSrc;
      //         register void *pvlc,*pvll;
      //        pvlc = (void*)temp_value_for_activated_function;//ui32
      //        pvll = (void*)&current_settings_prt.ranguvannja_In_GOOSE;//uint32_t
      //        i = j = lV = 0;
      //        //sLV.ch_amount_active_src = hldGsBlkParam.ch_amount_active_src;
      //        //pvl = (void*)&hldGsBlkParam;((GsBlkParamDsc*)pvl)->
      //        lCtrGsSrc = hldGsBlkParam.sh_amount_gs_active_src;
      //        asm ("nop" ::"r"(i),"r"(j),"r"(lV));
      //        while(lCtrGsSrc){
      //            lCtrGsSrc--;
      //            lV = arrOrdNumsGsSignal[lCtrGsSrc];
      //            i = lV>>3;j = lV - (i<<3);
      //
      //
      //            //temp_value_for_activated_function[N_SMALL];//;ranguvannja_In_GOOSE[i][j][0]
      //            lV = i*N_IN_GOOSE_MMS_OUT*N_SMALL*sizeof(long) +j*N_SMALL*sizeof(long);
      //            for(register unsigned long k = 0; k < N_SMALL; k++){
      //                ((unsigned long*)pvlc)[k] |= ((unsigned long*)pvll+lV) [k];
      //            }
      proc_Gs_blk_out((void *) &temp_value_for_activated_function, hldGsBlkParam.sh_amount_gs_active_src, arrOrdNumsGsSignal);
      //        }
    } while (sLV.ch_while_breaker);
    memcpy((void *) active_functions_goose_small_tmp, (const void *) temp_value_for_activated_function, ((N_SMALL) << 2));
    //
    //--------------------------------------------------------------------------------------------------------
    //````````````````````````````````````````````````````````````````````````````````````````````````````````

    /**************************/
    //Опрацьовуємо входи для Вх.MMS блоківі виставити потрібний біт у temp_value_for_activated_function_button_interface масиві
    /**************************/
    /**************************/
    //=====================================================================================================
    //''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
    //
    //....................................................................................................
    //=====================================================================================================
    do
    {
      sLV.ch_while_breaker = 0;
      // ----------------    -------------------------
      //        register long i,lV,j,lCtrMmsSrc;
      //         register void *pvlc,*pvll;
      //        pvlc = (void*)temp_value_for_activated_function;
      //        pvll = (void*)&current_settings_prt.ranguvannja_In_MMS;
      //        //sLV.ch_amount_active_src = hldGsBlkParam.ch_amount_active_src;
      //        //pvl = (void*)&hldGsBlkParam;((GsBlkParamDsc*)pvl)->
      //        lCtrMmsSrc = hldMmsBlkParam.sh_amount_mms_active_src;
      proc_Mms_blk_out((void *) &temp_value_for_activated_function_button_interface, hldMmsBlkParam.sh_amount_mms_active_src, arrOrdNumsMmsSignal);
      //        while(lCtrMmsSrc){
      //            lV = arrOrdNumsMmsSignal[lCtrMmsSrc];
      //            i = lV>>3;j = lV - i;
      //            lCtrMmsSrc--;
      //            //temp_value_for_activated_function[N_SMALL];//;ranguvannja_In_GOOSE[i][j][0]
      //            lV = i*N_IN_GOOSE_MMS_OUT*N_SMALL*sizeof(long) +j*N_SMALL*sizeof(long);
      //            for(long k = 0; k < N_SMALL; k++){
      //                ((long*)pvlc)[k] |= ((long*)pvll+lV) [k];
      //            }
      //
      //        }
    } while (sLV.ch_while_breaker);
    long idx_n_small_arr = 0;
    do
    {
      if (temp_value_for_activated_function_button_interface[idx_n_small_arr] != 0)
      {
        _SET_BIT(sLV.p_active_functions, RANG_LAN_ACTIVE);
        idx_n_small_arr += N_SMALL; //break;
      }

    } while (++idx_n_small_arr < N_SMALL);

    //
    //--------------------------------------------------------------------------------------------------------
    //````````````````````````````````````````````````````````````````````````````````````````````````````````

    /***/
#endif

    //Активація з кнопуки
    if (pressed_buttons_united != 0)
    {
      for (unsigned int i = 0; i < NUMBER_DEFINED_BUTTONS; i++)
      {
        if ((pressed_buttons_united & (1 << i)) != 0)
        {
          unsigned int *const point_temp_value_for_activated_function = ((pressed_buttons_tmp & (1 << i)) != 0) ? temp_value_for_activated_function_button_interface : temp_value_for_activated_function;
          for (size_t j = 0; j < N_SMALL; ++j)
          {
            point_temp_value_for_activated_function[j] |= current_settings_prt.ranguvannja_buttons[N_SMALL * i + j];
            active_functions_fc_small_tmp[j] |= current_settings_prt.ranguvannja_buttons[N_SMALL * i + j];
          }
        }
      }
      if ((pressed_buttons_united & (1 << (BIT_KEY_C - BIT_KEY_1))) != 0)
      {
        _SET_BIT(temp_value_for_activated_function, RANG_SMALL_RESET_LEDS);
        _SET_BIT(active_functions_fc_small_tmp, RANG_SMALL_RESET_LEDS);

        _SET_BIT(temp_value_for_activated_function, RANG_SMALL_RESET_RELES);
        _SET_BIT(active_functions_fc_small_tmp, RANG_SMALL_RESET_RELES);
      }
      if ((pressed_buttons_united & (1 << (BIT_KEY_I - BIT_KEY_1))) != 0)
      {
        _SET_BIT(temp_value_for_activated_function, RANG_SMALL_VKL_VV);
        _SET_BIT(active_functions_fc_small_tmp, RANG_SMALL_VKL_VV);
      }
      if ((pressed_buttons_united & (1 << (BIT_KEY_O - BIT_KEY_1))) != 0)
      {
        _SET_BIT(temp_value_for_activated_function, RANG_SMALL_OTKL_VV);
        _SET_BIT(active_functions_fc_small_tmp, RANG_SMALL_OTKL_VV);
      }
    }

    //Активація з Д.Входу
    if (active_inputs_local != 0)
    {
      for (unsigned int i = 0; i < NUMBER_INPUTS; i++)
      {
        if ((active_inputs_local & (1 << i)) != 0)
        {
          for (size_t j = 0; j < N_SMALL; ++j)
          {
            temp_value_for_activated_function[j] |= current_settings_prt.ranguvannja_inputs[N_SMALL * i + j];
            active_functions_bi_small_tmp[j] |= current_settings_prt.ranguvannja_inputs[N_SMALL * i + j];
          }
        }
      }
    }
    //Чи активний зараз місцевий режим від Д.Входу, або від ФК-Ключ
    unsigned int dystancijnyj = (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_MISCEVE_DYSTANCIJNE) == 0);

    //Активація з інтерфейсу
    if (
      (mutex_interface == false) &&
      (flag) &&
      (dostup_z_VR))
    {
      for (size_t i = 0; i < N_SMALL; ++i)
      {
        if (dystancijnyj)
          temp_value_for_activated_function_button_interface[i] |= activation_function_from_interface[i];
        activation_function_from_interface[i] = 0;
      }

      if (dystancijnyj)
      {
        if (_GET_STATE(dostup_z_VR, USB_RECUEST))
          _SET_BIT(active_functions, RANG_USB_ACTIVE);
        if (_GET_STATE(dostup_z_VR, RS485_RECUEST))
          _SET_BIT(active_functions, RANG_RS485_ACTIVE);
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
        if (_GET_STATE(dostup_z_VR, LAN_RECUEST))
          _SET_BIT(active_functions, RANG_LAN_ACTIVE);
#endif
      }
      dostup_z_VR = 0;
    }

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
    //Активація з КП
    if ((flagCommandKP) && (dystancijnyj))
    {
      for (size_t i = 0; i < N_SMALL; ++i)
        temp_value_for_activated_function_button_interface[i] |= command_from_KP[i];
      _SET_BIT(active_functions, RANG_LAN_ACTIVE);
    }
#endif

    //Обєднуємо активації з кнопок (режим кнопка) + активації з інтерфейсу і кнопок (режим ключ) + кнопки фіксованого функціоналу
    for (size_t i = 0; i < N_SMALL; ++i)
      temp_value_for_activated_function[i] |= temp_value_for_activated_function_button_interface[i];

    //Опреділювані функції
    for (size_t i = 0; i != NUMBER_DEFINED_FUNCTIONS; ++i)
    {
      size_t index_big = RANG_DF1_IN + 3 * i;
      size_t index_small = RANG_SMALL_DF1_IN + 2 * i;
      active_functions[index_big >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, index_small) != 0) << (index_big & 0x1f);

      //Перевірка на необхідність пролонгації активації В-функції на час  таймеру павзи
      if (_CHECK_SET_BIT(temp_value_for_activated_function_button_interface, index_small) != 0)
      {
        //Зараз має активуватися В-ФункціяХ, тому треба запустити таймер її утримування,
        //для того, щоб потім час цей можна було зрівняти з часом таймера павзи
        if (global_timers[INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START + i] < 0)
        {
          //Запускаємо таймер таймер утримування цієї функції в активному стані (емітація активного входу)
          //Запуск робимо тільки ту тому випадкук, якщо він ще не почався
          global_timers[INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START + i] = 0;
        }
      }

      index_big = RANG_DF1_R + 3 * i;
      index_small = RANG_SMALL_DF1_R + 2 * i;
      active_functions[index_big >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, index_small) != 0) << (index_big & 0x1f);
    }

    //Опреділювані триґери
    for (size_t i = 0; i != NUMBER_DEFINED_TRIGGERS; ++i)
    {
      size_t index_big = RANG_DT1_SET + 3 * i;
      size_t index_small = RANG_SMALL_DT1_SET + 2 * i;
      active_functions[index_big >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, index_small) != 0) << (index_big & 0x1f);
      active_functions[(index_big + 1) >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, (index_small + 1)) != 0) << ((index_big + 1) & 0x1f);

      index_big = RANG_DT1_RESET + 3 * i;
      index_small = RANG_SMALL_DT1_RESET + 2 * i;
      active_functions[index_big >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, index_small) != 0) << (index_big & 0x1f);
    }

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
    {
      size_t word_n_small = RANG_SMALL_BLOCK_IN_GOOSE1 >> 5;
      unsigned int maska_small = (unsigned int) (1 << (RANG_SMALL_BLOCK_IN_GOOSE1 & 0x1f));

      size_t word_n_full = RANG_BLOCK_IN_GOOSE1 >> 5;
      unsigned int maska_full = (unsigned int) (1 << (RANG_BLOCK_IN_GOOSE1 & 0x1f));

      for (size_t i = 0; i < (N_IN_GOOSE + N_IN_MMS + N_OUT_LAN); i++)
      {
        if (temp_value_for_activated_function[word_n_small] & maska_small)
        {
          active_functions[word_n_full] |= maska_full;
        }

        maska_small <<= 1;
        if (maska_small == 0)
        {
          word_n_small++;
          maska_small = 1;
        }

        maska_full <<= 1;
        if (maska_full == 0)
        {
          word_n_full++;
          maska_full = 1;
        }
      }
    }
#endif

    //Функції зв'язку
    for (size_t i = 0; i < NUMBER_LINK_FUNCTIONS; ++i)
    {
      unsigned int index_big = RANG_LF1 + i;
      unsigned int index_small = RANG_SMALL_LF1 + i;
      active_functions[index_big >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, index_small) != 0) << (index_big & 0x1f);
    }

    //Загальні функції (без ОФ-ій і функцій, які можуть блокуватися у місцевому управлінні)
    active_functions[RANG_BLOCK_VKL_VV >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_BLOCK_VKL_VV) != 0) << (RANG_BLOCK_VKL_VV & 0x1f);
    active_functions[RANG_RESET_LEDS >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_RESET_LEDS) != 0) << (RANG_RESET_LEDS & 0x1f);
    active_functions[RANG_RESET_RELES >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_RESET_RELES) != 0) << (RANG_RESET_RELES & 0x1f);
    active_functions[RANG_MISCEVE_DYSTANCIJNE >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_MISCEVE_DYSTANCIJNE) != 0) << (RANG_MISCEVE_DYSTANCIJNE & 0x1f);
    active_functions[RANG_STATE_VV >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_STATE_VV) != 0) << (RANG_STATE_VV & 0x1f);
    active_functions[RANG_CTRL_VKL >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_CTRL_VKL) != 0) << (RANG_CTRL_VKL & 0x1f);
    active_functions[RANG_CTRL_OTKL >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_CTRL_OTKL) != 0) << (RANG_CTRL_OTKL & 0x1f);
    active_functions[RANG_RESET_BLOCK_READY_TU_VID_ZAHYSTIV >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_RESET_BLOCK_READY_TU_VID_ZAHYSTIV) != 0) << (RANG_RESET_BLOCK_READY_TU_VID_ZAHYSTIV & 0x1f);
    active_functions[RANG_OTKL_VID_ZOVN_ZAHYSTIV >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_OTKL_VID_ZOVN_ZAHYSTIV) != 0) << (RANG_OTKL_VID_ZOVN_ZAHYSTIV & 0x1f);

    active_inputs_grupa_ustavok |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_1_GRUPA_USTAVOK) != 0) << (RANG_SMALL_1_GRUPA_USTAVOK - RANG_SMALL_1_GRUPA_USTAVOK);
    active_inputs_grupa_ustavok |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_2_GRUPA_USTAVOK) != 0) << (RANG_SMALL_2_GRUPA_USTAVOK - RANG_SMALL_1_GRUPA_USTAVOK);
    active_inputs_grupa_ustavok |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_3_GRUPA_USTAVOK) != 0) << (RANG_SMALL_3_GRUPA_USTAVOK - RANG_SMALL_1_GRUPA_USTAVOK);
    active_inputs_grupa_ustavok |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_4_GRUPA_USTAVOK) != 0) << (RANG_SMALL_4_GRUPA_USTAVOK - RANG_SMALL_1_GRUPA_USTAVOK);

    //Увімкнення ВВ
    active_functions[RANG_VKL_VV >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_VKL_VV) != 0) << (RANG_VKL_VV & 0x1f);

    //Вимкнення ВВ
    active_functions[RANG_OTKL_VV >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_OTKL_VV) != 0) << (RANG_OTKL_VV & 0x1f);
  }
  /**************************/

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)

#define N_SOURCE 3

#else

#define N_SOURCE 2

#endif

  for (size_t i = 0; i < N_SOURCE; ++i)
  {
    static unsigned int const command_signales[N_SMALL] =
      {
        COMMAND_SIGNALES_0,
        COMMAND_SIGNALES_1,
        COMMAND_SIGNALES_2};

    static unsigned int prev_active_functions_small[N_SOURCE][N_SMALL];

    static enum __rang_output_led_df_reg const n_signal[N_SOURCE] =
    {
      RANG_DI_ACTIVE,
      RANG_FK_ACTIVE

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
      ,
      RANG_GOOSE_ACTIVE
#endif
    };
    unsigned int const *const ref[N_SOURCE] =
    {
      active_functions_bi_small_tmp,
      active_functions_fc_small_tmp

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
      ,
      active_functions_goose_small_tmp
#endif
    };

    _CLEAR_BIT(active_functions, n_signal[i]);

    unsigned int temp_activating_functions_small[N_SMALL];
    for (unsigned int *p = temp_activating_functions_small; p < (temp_activating_functions_small + N_SMALL); ++p)
      *p = 0;
    for (size_t j = 0; j < N_SMALL; ++j)
    {
      //З масиву попередніх станів виділяємо тільки ті функції, якиї нас цікавить фронт змін і поміщаємо їх у тимчасовий масив
      temp_activating_functions_small[j] = prev_active_functions_small[i][j] & command_signales[j];

      //У тимчасовому масиві виділяємо тільки ті функції, у яких зафіксовано або передній, або задній фронти
      temp_activating_functions_small[j] ^= (ref[i][j] & command_signales[j]);

      //Тепер виділяємо у тимчасовому масиві тільки ті функції у яких зараз значення стоять рівні "1" (тобто відбувся перехід з "0" в "1")
      temp_activating_functions_small[j] &= ref[i][j];

      /*
      Перед тим, як виділити ті функції, які у цьому циклі будуть відмічені як активні 
      з урахуванням того, що деякі функції активоються по передньому фронті
      копіюємо статичний стан функцій (без врахування фронтів) у тимчасовий масив
      щоб при наступному аналізі мати попередній статичний стан активних функцій
      */
      prev_active_functions_small[i][j] = ref[i][j];
    }
    for (unsigned int *p = temp_activating_functions_small; p < (temp_activating_functions_small + N_SMALL); ++p)
    {
      if (*p != 0)
      {
        _SET_BIT(active_functions, n_signal[i]);
        break;
      }
    }
  }

#undef N_SOURCE

  /**************************/
  //Виконуємо фільтрацію переднього фронту для тих сигналів, які мають активуватися тільки по передньому фронтові
  /**************************/
  {
    static unsigned int previous_activating_functions[N_BIG];
    //Формуємо маску сигналів, які треба позначити, як активні тільки в момент переходу з "0" в "1"
    unsigned int temp_maska_filter_function[N_BIG];
    for (int i = 0; i < N_BIG; i++)
      temp_maska_filter_function[i] = 0;
    unsigned int temp_activating_functions[N_BIG];
    for (int i = 0; i < N_BIG; i++)
      temp_activating_functions[i] = 0;

    //Сигнал "Сблос индикации"
    _SET_BIT(temp_maska_filter_function, RANG_RESET_LEDS);

    //Сигнал "Сблос реле"
    _SET_BIT(temp_maska_filter_function, RANG_RESET_RELES);

    //Сигнал "Включить ВВ"
    _SET_BIT(temp_maska_filter_function, RANG_VKL_VV);

    //Сигнал "Отключить ВВ"
    _SET_BIT(temp_maska_filter_function, RANG_OTKL_VV);

    //Сигнал "Скидання блокування готовності до ТУ"
    _SET_BIT(temp_maska_filter_function, RANG_RESET_BLOCK_READY_TU_VID_ZAHYSTIV);

    for (unsigned int i = 0; i < N_BIG; i++)
    {
      //З масиву попередніх станів виділяємо тільки ті функції, якиї нас цікавить фронт змін і поміщаємо їх у тимчасовий масив
      temp_activating_functions[i] = previous_activating_functions[i] & temp_maska_filter_function[i];

      //У тимчасовому масиві виділяємо тільки ті функції, у яких зафіксовано або передній, або задній фронти
      temp_activating_functions[i] ^= (active_functions[i] & temp_maska_filter_function[i]);

      //Тепер виділяємо у тимчасовому масиві тільки ті функції у яких зараз значення стоять рівні "1" (тобто відбувся перехід з "0" в "1")
      temp_activating_functions[i] &= active_functions[i];

      /*
      Перед тим, як виділити ті функції, які у цьому циклі будуть відмічені як активні 
      з урахуванням того, що деякі функції активоються по передньому фронті
      копіюємо статичний стан функцій (без врахування фронтів) у тимчасовий масив
      щоб при наступному аналізі мати попередній статичний стан активних функцій
      */
      previous_activating_functions[i] = active_functions[i];

      //Обновляємо масив функцій, які зараз активуються з врахуванням того, що серед виділених функцій маскою активними мають юути тільки ті, у яких перехід був з "0" в "1"
      active_functions[i] = (active_functions[i] & (~temp_maska_filter_function[i])) | temp_activating_functions[i];
    }
  }
  /**************************/

  /**************************
  Світлова індикація стану вимикача
  **************************/
  {
    uint32_t state_vv_dv = false;
    for (size_t i = 0; i < NUMBER_INPUTS; i++)
    {
      if (_CHECK_SET_BIT((current_settings_prt.ranguvannja_inputs + N_SMALL * i), RANG_SMALL_STATE_VV) != 0)
      {
        state_vv_dv = true;
        break;
      }
    }
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
    //Шукаємо рандування "Стан ВВ" на блоках вхідних GOOSE
    for (size_t i = 0; (state_vv_dv == false) && (i < N_IN_GOOSE); ++i)
    {
      for (size_t j = 0; j < N_IN_GOOSE_MMS_OUT; ++j)
      {
        if (_CHECK_SET_BIT(current_settings_prt.ranguvannja_In_GOOSE[i][j], RANG_SMALL_STATE_VV) != 0)
        {
          state_vv_dv = true;
          break;
        }
      }
    }
#endif

    if (state_vv_dv)
    {
      if (_CHECK_SET_BIT(active_functions, RANG_STATE_VV) != 0)
      {
        state_leds_ctrl &= (uint32_t)(~((1 << LED_COLOR_GREEN_BIT) << ((uint32_t) NUMBER_LED_COLOR * (uint32_t) LED_CTRL_O)));
        state_leds_ctrl |= (uint32_t)((1 << LED_COLOR_RED_BIT) << ((uint32_t) NUMBER_LED_COLOR * (uint32_t) LED_CTRL_I));
      }
      else
      {
        state_leds_ctrl &= (uint32_t)(~((1 << LED_COLOR_RED_BIT) << ((uint32_t) NUMBER_LED_COLOR * (uint32_t) LED_CTRL_I)));
        state_leds_ctrl |= (uint32_t)((1 << LED_COLOR_GREEN_BIT) << ((uint32_t) NUMBER_LED_COLOR * (uint32_t) LED_CTRL_O));
      }
    }
    else
    {
      state_leds_ctrl &= (uint32_t)(~(((1 << LED_COLOR_GREEN_BIT) << ((uint32_t) NUMBER_LED_COLOR * (uint32_t) LED_CTRL_O)) | ((1 << LED_COLOR_RED_BIT) << ((uint32_t) NUMBER_LED_COLOR * (uint32_t) LED_CTRL_I))));
    }
  }
  /**************************/

  /**************************/
  /*Режим перепрограмування*/
  /**************************/
  //#ifndef DEBUG_TEST
  if ((GPIO_STAFF_REPROGRAM->IDR & GPIO_PIN_STAFF_REPROGRAM) != (uint32_t) Bit_RESET)
  //#else
  //  if (reprogram)
  //#endif
  {
    _SET_BIT(set_diagnostyka, WARNING_REPROGRAM);
    timerWaitReprogram = -1;
  }
  else
    _SET_BIT(clear_diagnostyka, WARNING_REPROGRAM);
  /**************************/

  //  //Діагностика справності раз на період
  //  diagnostyca_adc_execution();

  /**************************/
  //Сигнал "Несправность Общая"
  /**************************/
  if (
    (_CHECK_SET_BIT(active_functions, RANG_RESET_LEDS) != 0) ||
    (_CHECK_SET_BIT(active_functions, RANG_RESET_RELES) != 0))
  {
    _SET_BIT(clear_diagnostyka, ERROR_DR_LOSS_INFORMATION_BIT);
    _SET_BIT(clear_diagnostyka, ERROR_PR_ERR_LOSS_INFORMATION_BIT);
  }

  unsigned int diagnostyka_tmp[N_DIAGN];
  for (size_t i = 0; i < N_DIAGN; i++)
  {
    diagnostyka_tmp[i] = diagnostyka[i];

    diagnostyka_tmp[i] &= (unsigned int) (~clear_diagnostyka[i]);
    diagnostyka_tmp[i] |= set_diagnostyka[i];
  }

  _CLEAR_BIT(diagnostyka_tmp, EVENT_START_SYSTEM_BIT);
  _CLEAR_BIT(diagnostyka_tmp, EVENT_SOFT_RESTART_SYSTEM_BIT);
  _CLEAR_BIT(diagnostyka_tmp, EVENT_DROP_POWER_BIT);
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
  _CLEAR_BIT(diagnostyka_tmp, EVENT_RESTART_CB_BIT);
#endif
  unsigned int not_null = false;
  for (size_t i = 0; i < N_DIAGN; i++)
  {
    not_null |= (diagnostyka_tmp[i] != 0);
    if (not_null)
      break;
  }

  if (not_null)
  {
    _SET_BIT(active_functions, RANG_DEFECT);
    /**************************/
    //Сигнал "Несправность Аварийная"
    /**************************/
    static unsigned int const maska_avar_error[N_DIAGN] =
      {
        MASKA_AVAR_ERROR_0,
        MASKA_AVAR_ERROR_1,
        MASKA_AVAR_ERROR_2,
        MASKA_AVAR_ERROR_3
#ifdef MASKA_AVAR_ERROR_4
        ,
        MASKA_AVAR_ERROR_4
#endif
      };

    not_null = false;
    for (size_t i = 0; i < N_DIAGN; i++)
    {
      not_null |= ((diagnostyka_tmp[i] & maska_avar_error[i]) != 0);
      if (not_null)
        break;
    }
    if (not_null)
    {
      _SET_BIT(active_functions, RANG_AVAR_DEFECT);
      //#warning "No Avar Error"
    }
    else
    {
      _CLEAR_BIT(active_functions, RANG_AVAR_DEFECT);
    }
    /**************************/
  }
  else
  {
    _CLEAR_BIT(active_functions, RANG_DEFECT);
    _CLEAR_BIT(active_functions, RANG_AVAR_DEFECT);
  }
  /**************************/

  static unsigned int previous_active_functions[N_BIG];
  if (_CHECK_SET_BIT(active_functions, RANG_AVAR_DEFECT) == 0)
  {
    //Аварійна ситуація не зафіксована

    /**************************/
    //Обробка передавальних функцій
    /**************************/
    tf_handler(previous_active_functions, active_functions, &active_inputs_grupa_ustavok);
    /**************************/
  }

  /**************************/
  //Вибір групи уставок
  /**************************/
  for (size_t i = 0; i < NUMBER_GROUP_USTAVOK; i++)
  {
    if (active_inputs_grupa_ustavok & (1 << i))
    {
      _CLEAR_BIT(active_functions, RANG_INVERS_DV_GRUPA_USTAVOK);
      break;
    }
    else
    {
      if (i == (NUMBER_GROUP_USTAVOK - 1))
        _SET_BIT(active_functions, RANG_INVERS_DV_GRUPA_USTAVOK);
    }
  }

  if (count_number_set_bit(&active_inputs_grupa_ustavok, NUMBER_GROUP_USTAVOK) > 1)
  {
    _SET_BIT(set_diagnostyka, ERROR_SELECT_GRUPY_USRAVOK);
    _SET_BIT(diagnostyka_tmp, ERROR_SELECT_GRUPY_USRAVOK);
    _SET_BIT(active_functions, RANG_DEFECT);
  }
  else
  {
    _SET_BIT(clear_diagnostyka, ERROR_SELECT_GRUPY_USRAVOK);
    _CLEAR_BIT(diagnostyka_tmp, ERROR_SELECT_GRUPY_USRAVOK);

    not_null = false;
    for (size_t i = 0; i < N_DIAGN; i++)
    {
      not_null |= (diagnostyka_tmp[i] != 0);
      if (not_null)
        break;
    }
    if (not_null == false)
    {
      _CLEAR_BIT(active_functions, RANG_DEFECT);
    }
  }

  static unsigned int const maska_signals_for_lock_group[N_BIG] =
    {
      MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_0,
      MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_1,
      MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_2,
      MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_3,
      MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_4,
      MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_5,
      MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_6};
  unsigned int comp = false;
  COMPARE_NOT_ZERO_OR(comp, active_functions, maska_signals_for_lock_group, N_BIG)
  if (comp)
  {
    //Іде блокування груп уставок - група уставок залишається тою, яка вибрана попередньо
    _SET_BIT(active_functions, RANG_BLK_GRUP_USTAVOK_VID_ZACHYSTIV);

    unsigned int number_group_stp_tmp = 0;
    if ((_CHECK_SET_BIT(active_functions, RANG_1_GRUPA_USTAVOK)) != 0)
      number_group_stp_tmp++;
    if ((_CHECK_SET_BIT(active_functions, RANG_2_GRUPA_USTAVOK)) != 0)
      number_group_stp_tmp++;
    if ((_CHECK_SET_BIT(active_functions, RANG_3_GRUPA_USTAVOK)) != 0)
      number_group_stp_tmp++;
    if ((_CHECK_SET_BIT(active_functions, RANG_4_GRUPA_USTAVOK)) != 0)
      number_group_stp_tmp++;

    if (number_group_stp_tmp != 1)
    {
      //Теоретично цього ніколи не мало б бути
      total_error_sw_fixed();
    }
  }
  else
  {
    //Можна вибирати групу уставок - захисти цю операцію не блокують
    _CLEAR_BIT(active_functions, RANG_BLK_GRUP_USTAVOK_VID_ZACHYSTIV);
    setpoints_selecting(active_functions, active_inputs_grupa_ustavok);
  }

  unsigned int number_group_stp = 0;
  if ((_CHECK_SET_BIT(active_functions, RANG_1_GRUPA_USTAVOK)) != 0)
    number_group_stp = 0;
  else if ((_CHECK_SET_BIT(active_functions, RANG_2_GRUPA_USTAVOK)) != 0)
    number_group_stp = 1;
  else if ((_CHECK_SET_BIT(active_functions, RANG_3_GRUPA_USTAVOK)) != 0)
    number_group_stp = 2;
  else if ((_CHECK_SET_BIT(active_functions, RANG_4_GRUPA_USTAVOK)) != 0)
    number_group_stp = 3;
  else
  {
    //Теоретично цього ніколи не мало б бути
    total_error_sw_fixed();
  }
  /**************************/

  /***********************************************************/
  //Розрахунок вимірювань
  /***********************************************************/
  calc_measurement(number_group_stp);

  //Копіюємо вимірювання для низькопріоритетних і високопріоритетних завдань
  unsigned int bank_measurement_high_tmp = (bank_measurement_high ^ 0x1) & 0x1;
  if (semaphore_measure_values_low == 0)
  {
    for (unsigned int i = 0; i < _NUMBER_IM; i++)
    {
      measurement_high[bank_measurement_high_tmp][i] = measurement_middle[i] = measurement[i];
    }
    frequency_middle = frequency;
  }
  else
  {
    for (unsigned int i = 0; i < _NUMBER_IM; i++)
    {
      measurement_high[bank_measurement_high_tmp][i] = measurement[i];
    }
  }
  bank_measurement_high = bank_measurement_high_tmp;
  /***********************************************************/

  //Логічні схеми мають працювати тільки у тому випадку, якщо немє сигналу "Аварийная неисправность"
  if (_CHECK_SET_BIT(active_functions, RANG_AVAR_DEFECT) == 0)
  {
    //Аварійна ситуація не зафіксована

    /**************************/
    //Розширена логіка
    /**************************/
    if ((current_settings_prt.configuration & (1 << EL_BIT_CONFIGURATION)) != 0)
    {
      unsigned int active_functions_tmp[NUMBER_ITERATION_EL_MAX][N_BIG];
      unsigned int iteration = 0;
      unsigned int repeat_state = false;
      while (iteration < current_settings_prt.number_iteration_el)
      {
        if (iteration > 0)
        {
          //Перевірка на рівність поточного стану і попереднього
          COMPARE_AND(comp, active_functions_tmp[iteration - 1], active_functions, N_BIG)
          if (comp)
            break;

          //Пошук, чи вже був поточний стан зафіксований у попередніх ітераціях
          for (int i = (iteration - 2); ((repeat_state == false) && (i >= 0)); --i)
          {
            COMPARE_AND(repeat_state, active_functions_tmp[i], active_functions, N_BIG)
          }
        }
        if (repeat_state)
          break;

        for (size_t i = 0; i != N_BIG; ++i)
          active_functions_tmp[iteration][i] = active_functions[i];

        d_and_handler(active_functions);
        d_or_handler(active_functions);
        d_xor_handler(active_functions);
        d_not_handler(active_functions);
        df_handler(active_functions);
        dt_handler(active_functions);

        ++iteration;
      }

      if (
        (repeat_state) ||
        (iteration >= current_settings_prt.number_iteration_el))
      {
        _SET_BIT(active_functions, RANG_ERROR_CONF_EL);
      }
      else
      {
        _CLEAR_BIT(active_functions, RANG_ERROR_CONF_EL);
      }
    }
    else
    {
      //Очищуємо сигнали, які не можуть бути у даній конфігурації
      static const unsigned int maska_el_signals[N_BIG] =
        {
          MASKA_EL_SIGNALS_0,
          MASKA_EL_SIGNALS_1,
          MASKA_EL_SIGNALS_2,
          MASKA_EL_SIGNALS_3,
          MASKA_EL_SIGNALS_4,
          MASKA_EL_SIGNALS_5,
          MASKA_EL_SIGNALS_6};
      for (size_t i = 0; i < N_BIG; ++i)
        active_functions[i] &= (unsigned int) (~maska_el_signals[i]);

      //Скидаємо всі таймери, які відповідають за розширену логіку
      for (int *p = (global_timers + INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START); p <= (global_timers + INDEX_TIMER_DF_WORK_START + NUMBER_DEFINED_FUNCTIONS - 1); ++p)
        *p = -1;

      static_logic_df = 0;
    }
    /**************************/

    /**************************/
    //Готовность к ТУ
    /**************************/
    ready_tu(active_functions);
    /**************************/
  }
  else
  {
    //Аварійна ситуація зафіксована

    //Скидаємо всі активні функції, крім інформативних
    static unsigned int const maska_info_signals[N_BIG] =
      {
        MASKA_INFO_SIGNALES_0,
        MASKA_INFO_SIGNALES_1,
        MASKA_INFO_SIGNALES_2,
        MASKA_INFO_SIGNALES_3,
        MASKA_INFO_SIGNALES_4,
        MASKA_INFO_SIGNALES_5,
        MASKA_INFO_SIGNALES_6};

    for (size_t i = 0; i != N_BIG; ++i)
      active_functions[i] &= maska_info_signals[i];

    //Скидаємо всі таймери, які присутні у лозіці
    for (int *p = (global_timers + _INDEX_TIMER_LOGIC_BEGIN); p != (global_timers + _MAX_NUMBER_GLOBAL_TIMERS_WITHOUT_REGS); ++p)
      *p = -1;

    //Деактивовуємо всі реле
    state_outputs = 0;
#ifdef NUMBER_DS
    ds = 0;
#endif

    //Переводимо у початковий стан деякі глобальні змінні

    static_logic_APV_0 = 0;
    previous_states_APV_0 = 0;
    trigger_APV_0 = 0;
    previous_state_po_achr_chapv_uaf1 = 0;
    previous_state_po_achr_chapv_ubf1 = 0;
    previous_state_po_achr_chapv_ucf1 = 0;
    previous_states_CHAPV1 = 0;
    trigger_CHAPV1 = 0;
    previous_states_ready_tu = 0;
    trigger_ready_tu = 0;

    static_logic_df = 0;
  }

  /**************************/
  //Обробка аналогового реєстратора
  /**************************/
  analog_registrator(active_functions);
  /**************************/

  /**************************/
  //Обробка дискретного реєстратора
  /**************************/
  digital_registrator(active_functions);
  /**************************/

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
  /***
  Опрацювати логіку Вихідного Мережевого Блоку
  ***/

  //=====================================================================================================
  //''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
  //             LAN BLOCK
  //....................................................................................................
  //=====================================================================================================
  do
  {
    sLV.ch_while_breaker = 0; //! optimize then

    register void *pvl;

    register unsigned int *r_p_active_functions;
    // ----------------    -------------------------
    r_p_active_functions = sLV.p_active_functions;
    pvl = (void *) &hldLanBlkParam; //hldMmsBlkParam;
    if (_CHECK_SET_BIT(((unsigned int *) r_p_active_functions), (RANG_BLOCK_OUT_LAN1 + ((unsigned int) LAN_BLOCK_ORD_NUM_00))) != 0)
    {
      //clr block
      Output_Out_LAN_block[LAN_BLOCK_ORD_NUM_00] = 0;
      //
    }
    else
    {
      proc_Lan_blk_out(&current_settings_prt.ranguvannja_Out_LAN[0][0][0], r_p_active_functions, (void *) &(((GsBlkParamDsc *) pvl)->unnV1[LAN_BLOCK_ORD_NUM_00].ulV));
    }

    if (_CHECK_SET_BIT(((unsigned int *) r_p_active_functions), (RANG_BLOCK_OUT_LAN1 + ((unsigned int) LAN_BLOCK_ORD_NUM_01))) != 0)
    {
      //clr block
      Output_Out_LAN_block[LAN_BLOCK_ORD_NUM_01] = 0;
    }
    else
    {
      proc_Lan_blk_out(&current_settings_prt.ranguvannja_Out_LAN[0][0][0], r_p_active_functions, (void *) &(((GsBlkParamDsc *) pvl)->unnV1[LAN_BLOCK_ORD_NUM_01].ulV));
    }

    if (_CHECK_SET_BIT(((unsigned int *) r_p_active_functions), (RANG_BLOCK_OUT_LAN1 + ((unsigned int) LAN_BLOCK_ORD_NUM_02))) != 0)
    {
      //clr block
      Output_Out_LAN_block[LAN_BLOCK_ORD_NUM_02] = 0;
    }
    else
    {
      proc_Lan_blk_out(&current_settings_prt.ranguvannja_Out_LAN[0][0][0], r_p_active_functions, (void *) &(((GsBlkParamDsc *) pvl)->unnV1[LAN_BLOCK_ORD_NUM_02].ulV));
    }

    if (_CHECK_SET_BIT(((unsigned int *) r_p_active_functions), (RANG_BLOCK_OUT_LAN1 + ((unsigned int) LAN_BLOCK_ORD_NUM_03))) != 0)
    {
      //clr block
      Output_Out_LAN_block[LAN_BLOCK_ORD_NUM_03] = 0;
    }
    else
    {
      proc_Lan_blk_out(&current_settings_prt.ranguvannja_Out_LAN[0][0][0], r_p_active_functions, (void *) &(((GsBlkParamDsc *) pvl)->unnV1[LAN_BLOCK_ORD_NUM_03].ulV));
    }

  } while (sLV.ch_while_breaker);
  //
  //--------------------------------------------------------------------------------------------------------
  //````````````````````````````````````````````````````````````````````````````````````````````````````````

  /***/
#endif

  /**************************/
  //Робота з функціями, які мають записуватися у енергонезалежну пам'ять
  /**************************/
  if (_CHECK_SET_BIT(active_functions, RANG_AVAR_DEFECT) == 0)
  {
    static const unsigned int maska_trg_func_array[N_BIG] =
      {
        MASKA_TRIGGER_SIGNALES_0,
        MASKA_TRIGGER_SIGNALES_1,
        MASKA_TRIGGER_SIGNALES_2,
        MASKA_TRIGGER_SIGNALES_3,
        MASKA_TRIGGER_SIGNALES_4,
        MASKA_TRIGGER_SIGNALES_5,
        MASKA_TRIGGER_SIGNALES_6};
    comp = true;
    for (size_t i = 0; i != N_BIG; ++i)
    {
      unsigned int tmp_data = active_functions[i] & maska_trg_func_array[i];
      if (trigger_active_functions[i] != tmp_data)
      {
        comp = false;
        trigger_active_functions[i] = tmp_data;
      }
    }
    if (comp != true)
    {
      /*
      Сигнали, значення яких записується у енергонезалежну пам'ять змінилися.
      Подаємо команду на їх запис у енергонезалежну пам'ять
      */
      _SET_BIT(control_spi1_taskes, TASK_START_WRITE_TRG_FUNC_EEPROM_BIT);
    }
  }
  /**************************/

  /**************************/
  //Перекидання інфомації у масиви активних і тригерних функцій
  /**************************/
  for (size_t i = 0; i != N_BIG; ++i)
  {
    unsigned int temp_data = active_functions[i];
    trigger_functions_USB[i] |= temp_data;
    trigger_functions_RS485[i] |= temp_data;
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
    trigger_functions_LAN[i] |= temp_data;
#endif
  }

  copying_active_functions = 0; //Помічаємо, що обновлення значення активних функцій завершене

  /*
  Робимо копію значення активних функцій для того, щоб коли ці знаення будуть
  обновлятися, то можна було б іншим модулям  (запис у об'єднаний аналоговий
  реєстратор) взяти попереднє, але достовірне значення
  */
  for (unsigned int i = 0; i < N_BIG; i++)
  {
    unsigned int temp_data = active_functions[i];
    active_functions_copy[i] = temp_data;
    previous_active_functions[i] = temp_data;
  }
  /**************************/
  CmdPlusTimeLogHundler(active_functions);
  /**************************/
  //Вивід інформації на виходи
  /**************************/
  //Спочатку перевіряємо, чи не активовувалвся команда "Скид.реле" - і якщо так, то попередньо скидаємо всі реле
  if (_CHECK_SET_BIT(active_functions, RANG_RESET_RELES) != 0)
  {
    state_outputs = 0;
  }

  if (_CHECK_SET_BIT(active_functions, RANG_AVAR_DEFECT) == 0)
  {
    //Не зафіксовано аварійної ситуації, тому встановлювати реле можна

    //Визначаємо, які реле зараз мають бути замкнутими
    for (unsigned int i = 0; i < NUMBER_SIMPLE_OUTPUTS; i++)
    {
      //У тимчасовий масив поміщаємо ЛОГІЧНЕ І ранжування виходу, який індексується інедексом "i" і функцій, які зараз є активними
      unsigned int temp_array_of_outputs[N_BIG];

      for (unsigned int j = 0; j < N_BIG; j++)
        temp_array_of_outputs[j] = current_settings_prt.ranguvannja_outputs[N_BIG * i + j] & active_functions[j];

      //Сигнал "Аварійна несправність" працює у інверсному режимі: замикає реле на якому зранжована у випадку, коли даний сигнал не активинй
      if (_CHECK_SET_BIT((current_settings_prt.ranguvannja_outputs + N_BIG * i), RANG_AVAR_DEFECT) != 0)
      {
        //Сигнал "Aварийная неисправность"  справді зранжовано на даний вихід
        if (_CHECK_SET_BIT(temp_array_of_outputs, RANG_AVAR_DEFECT) == 0)
        {
          //Сигнал "Aварийная неисправность" не є активним
          //Приимусово встановлюємо його у активний стан у масиві, який є  ЛОГІЧНИМ І анжування виходу, який індексується інедексом "i" і функцій, які зараз є активними
          _SET_BIT(temp_array_of_outputs, RANG_AVAR_DEFECT);
        }
        else
        {
          //Сигнал "Aварийная неисправность" є активним
          //Приимусово переводимо його у пасивний стан у масиві, який є  ЛОГІЧНИМ І анжування виходу, який індексується інедексом "i" і функцій, які зараз є активними
          _CLEAR_BIT(temp_array_of_outputs, RANG_AVAR_DEFECT);
        }
      }

      //Сигнал "Загальна несправність" працює у інверсному режимі: замикає реле на якому зранжована у випадку, коли даний сигнал не активинй
      if (_CHECK_SET_BIT((current_settings_prt.ranguvannja_outputs + N_BIG * i), RANG_DEFECT) != 0)
      {
        //Сигнал "Загальна несправність"  справді зранжовано на даний вихід
        if (_CHECK_SET_BIT(temp_array_of_outputs, RANG_DEFECT) == 0)
        {
          //Сигнал "Загальна несправність" не є активним
          //Приимусово встановлюємо його у активний стан у масиві, який є  ЛОГІЧНИМ І анжування виходу, який індексується інедексом "i" і функцій, які зараз є активними
          _SET_BIT(temp_array_of_outputs, RANG_DEFECT);
        }
        else
        {
          //Сигнал "Загальна несправність" є активним
          //Приимусово переводимо його у пасивний стан у масиві, який є  ЛОГІЧНИМ І анжування виходу, який індексується інедексом "i" і функцій, які зараз є активними
          _CLEAR_BIT(temp_array_of_outputs, RANG_DEFECT);
        }
      }

      //Перевіряємо, чи є співпадіння між ранжованими функціями на цьому виході і функціями, які зараз є активними - умова активації виходу
      flag = 0;
      for (int m = 0; m < N_BIG; m++)
        if (temp_array_of_outputs[m] != 0)
        {
          flag = 1;
          break;
        }
      if (flag)
      {
        //Для сигнального реле виконуємо його замикання, а для командного перевіряємо чи нема спроби активувати реле при умові що на нього заведено блок включення, причому він блокований
        if ((current_settings_prt.type_of_output & (1 << i)) != 0)
        {
          //Вихід сигнальний, тому у буль якому разі замикаємо реле
          //Відмічаємо, що даний вихід - ЗАМКНУТИЙ
          state_outputs |= (1 << i);
        }
        else
        {
          //Вихід командний, тому перевіряємо чи не йде спроба активувати реле, на яке заведено БВ, причому блок БВ з пеквних причин блокований (неактивний)
          if (_CHECK_SET_BIT((current_settings_prt.ranguvannja_outputs + N_BIG * i), RANG_WORK_BV) == 0)
          {
            //На дане реле не заводиться сигнал БВ (блок включення)

            //Відмічаємо, що даний вихід - ЗАМКНУТИЙ
            state_outputs |= (1 << i);
          }
          else
          {
            //На дане реле заводиться сигнал БВ (блок включення)

            //Відмічаємо, що даний вихід - ЗАМКНУТИЙ тільки тоді, коли функція БВ активна зараз
            if (_CHECK_SET_BIT(active_functions, RANG_WORK_BV) != 0)
              state_outputs |= (1 << i);
            else
              state_outputs &= ~(1 << i);
          }
        }
      }
      else
      {
        //Перевіряємо, чи вихід командний, чи сигнальний
        if ((current_settings_prt.type_of_output & (1 << i)) == 0)
        {
          //Вихід командний

          //Відмічаємо, що даний вихід - РОЗІМКНУТИЙ
          state_outputs &= ~(1 << i);
        }
      }
    }

#ifdef NUMBER_DS
    if ((current_settings_prt.configuration & (1 << DS_BIT_CONFIGURATION)) != 0)
    {
      for (size_t i = NUMBER_SIMPLE_OUTPUTS; i != NUMBER_OUTPUTS; ++i)
      {
        //У тимчасовий масив поміщаємо ЛОГІЧНЕ І ранжування виходу, який індексується інедексом "i" і функцій, які зараз є активними
        unsigned int temp_array_of_outputs[N_BIG];

        for (unsigned int j = 0; j < N_BIG; j++)
          temp_array_of_outputs[j] = current_settings_prt.ranguvannja_outputs[N_BIG * i + j] & active_functions[j];

        //Сигнал "Аварійна несправність" працює у інверсному режимі: замикає реле на якому зранжована у випадку, коли даний сигнал не активинй
        if (_CHECK_SET_BIT((current_settings_prt.ranguvannja_outputs + N_BIG * i), RANG_AVAR_DEFECT) != 0)
        {
          //Сигнал "Aварийная неисправность"  справді зранжовано на даний вихід
          if (_CHECK_SET_BIT(temp_array_of_outputs, RANG_AVAR_DEFECT) == 0)
          {
            //Сигнал "Aварийная неисправность" не є активним
            //Приимусово встановлюємо його у активний стан у масиві, який є  ЛОГІЧНИМ І анжування виходу, який індексується інедексом "i" і функцій, які зараз є активними
            _SET_BIT(temp_array_of_outputs, RANG_AVAR_DEFECT);
          }
          else
          {
            //Сигнал "Aварийная неисправность" є активним
            //Приимусово переводимо його у пасивний стан у масиві, який є  ЛОГІЧНИМ І анжування виходу, який індексується інедексом "i" і функцій, які зараз є активними
            _CLEAR_BIT(temp_array_of_outputs, RANG_AVAR_DEFECT);
          }
        }

        //Сигнал "Загальна несправність" працює у інверсному режимі: замикає реле на якому зранжована у випадку, коли даний сигнал не активинй
        if (_CHECK_SET_BIT((current_settings_prt.ranguvannja_outputs + N_BIG * i), RANG_DEFECT) != 0)
        {
          //Сигнал "Загальна несправність"  справді зранжовано на даний вихід
          if (_CHECK_SET_BIT(temp_array_of_outputs, RANG_DEFECT) == 0)
          {
            //Сигнал "Загальна несправність" не є активним
            //Приимусово встановлюємо його у активний стан у масиві, який є  ЛОГІЧНИМ І анжування виходу, який індексується інедексом "i" і функцій, які зараз є активними
            _SET_BIT(temp_array_of_outputs, RANG_DEFECT);
          }
          else
          {
            //Сигнал "Загальна несправність" є активним
            //Приимусово переводимо його у пасивний стан у масиві, який є  ЛОГІЧНИМ І анжування виходу, який індексується інедексом "i" і функцій, які зараз є активними
            _CLEAR_BIT(temp_array_of_outputs, RANG_DEFECT);
          }
        }

        //Перевіряємо, чи є співпадіння між ранжованими функціями на цьому виході і функціями, які зараз є активними - умова активації виходу
        NOT_ZERO_OR(comp, temp_array_of_outputs, N_BIG)
        if (comp)
        {
          if (_CHECK_SET_BIT((current_settings_prt.ranguvannja_outputs + N_BIG * i), RANG_WORK_BV) == 0)
          {
            //На дане реле не заводиться сигнал БВ (блок включення)

            //Відмічаємо, що даний вихід - ЗАМКНУТИЙ
            _SET_STATE(ds, (i - NUMBER_SIMPLE_OUTPUTS));
          }
          else
          {
            //На дане реле заводиться сигнал БВ (блок включення)

            //Відмічаємо, що даний вихід - ЗАМКНУТИЙ тільки тоді, коли функція БВ активна зараз
            if (_CHECK_SET_BIT(active_functions, RANG_WORK_BV) != 0)
              _SET_STATE(ds, (i - NUMBER_SIMPLE_OUTPUTS));
            else
              _CLEAR_STATE(ds, (i - NUMBER_SIMPLE_OUTPUTS));
          }
        }
        else
        {
          //Відмічаємо, що даний вихід - РОЗІМКНУТИЙ
          _CLEAR_STATE(ds, (i - NUMBER_SIMPLE_OUTPUTS));
        }
      }
    }
    else
      ds = 0;
#endif
  }
  else
  {
    //Зафіксовано аварійнe ситуацію, тому деактивуємо всі реле!!!

    //Деактивовуємо всі реле
    state_outputs = 0;
#ifdef NUMBER_DS
    ds = 0;
#endif
  }

  //Перевіряємо чи треба записувати стан сигнальних виходів у EEPROM
  if (
    (_CHECK_SET_BIT(active_functions, RANG_AVAR_DEFECT) == 0) &&
    ((state_outputs & current_settings_prt.type_of_output) != state_signal_outputs))
  {
    state_signal_outputs = state_outputs & current_settings_prt.type_of_output;
    //Виставляємо повідомлення про те, що в EEPROM треба записати нові значення сигнальних виходів і тригерних світлоіндикаторів
    _SET_BIT(control_spi1_taskes, TASK_START_WRITE_STATE_LEDS_OUTPUTS_EEPROM_BIT);
  }

  //Стан виходу з уразуванням імпульсного режиму роботи сигнальних виходів
  unsigned int output_signal_modif = (current_settings_prt.type_of_output_modif & current_settings_prt.type_of_output);
  state_outputs_raw = (state_outputs & ((unsigned int) (~output_signal_modif))) | ((state_outputs & output_signal_modif) * output_timer_prt_signal_output_mode_2);

//Оновлюємо поріг чутливості для тих випаків, коли на цей регістр більше нічого не заводиться
#if (                              \
  (MODYFIKACIA_VERSII_PZ == 0) ||  \
  (MODYFIKACIA_VERSII_PZ == 3) ||  \
  (MODYFIKACIA_VERSII_PZ == 4) ||  \
  (MODYFIKACIA_VERSII_PZ == 6) ||  \
  (MODYFIKACIA_VERSII_PZ == 10) || \
  (MODYFIKACIA_VERSII_PZ == 13) || \
  (MODYFIKACIA_VERSII_PZ == 14) || \
  (MODYFIKACIA_VERSII_PZ == 14))
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD25_DD27_DD28_DD30) = ((zdz_ovd_porig & 0xf) << 8) | (test_OVD << 12);
#endif

#if (                              \
  (MODYFIKACIA_VERSII_PZ != 7) &&  \
  (MODYFIKACIA_VERSII_PZ != 17) && \
  (MODYFIKACIA_VERSII_PZ != 8) &&  \
  (MODYFIKACIA_VERSII_PZ != 18))
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37) = state_outputs_raw;
#elif (                           \
  (MODYFIKACIA_VERSII_PZ != 8) && \
  (MODYFIKACIA_VERSII_PZ != 18))
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37) =
    (state_outputs_raw & ((1u << 9) - 1)) |
    (((zdz_ovd_porig & 0xf) << 9) | ((test_OVD != 0) << 13)) |
    (((state_outputs_raw >> 9) & ((1u << 2) - 1)) << 14);
#endif

#if (                             \
  (MODYFIKACIA_VERSII_PZ == 5) || \
  (MODYFIKACIA_VERSII_PZ == 15))

  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD25_DD27_DD28_DD30) = (state_outputs_raw >> 16) << 8;

#else

#ifdef NUMBER_DS
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD25_DD27_DD28_DD30) =
#if (                             \
  (MODYFIKACIA_VERSII_PZ == 7) || \
  (MODYFIKACIA_VERSII_PZ == 17))

    (((state_outputs_raw >> 11) & ((1u << 1) - 1)) << 1) |

#endif
    (ds & (MASKA_FOR_BIT(NUMBER_DS) - 1)) << 8;
#endif

#endif

#if (                             \
  (MODYFIKACIA_VERSII_PZ == 8) || \
  (MODYFIKACIA_VERSII_PZ == 18))

#define A_01_ONB 0 // 1
#define A_02_ONB (A_01_ONB + 1) // 2
#define E_01_ONB (A_02_ONB + 1) // 3
#define E_02_ONB (E_01_ONB + 1) // 4
#define E_03_ONB (E_02_ONB + 1) // 5
#define E_04_ONB (E_03_ONB + 1) // 6
#define E_05_ONB (E_04_ONB + 1) // 7
#define E_06_ONB (E_05_ONB + 1) // 8
#define E_07_ONB (E_06_ONB + 1) // 9
#define E_08_ONB (E_07_ONB + 1) // 10
#define E_09_ONB (E_08_ONB + 1) // 11
#define E_10_ONB (E_09_ONB + 1) // 12
#define E_11_ONB (E_10_ONB + 1) // 13
#define E_12_ONB (E_11_ONB + 1) // 14
#define E_13_ONB (E_12_ONB + 1) // 15
#define E_14_ONB (E_13_ONB + 1) // 16
#define E_15_ONB (E_14_ONB + 1) // 17
#define E_16_ONB (E_15_ONB + 1) // 18
#define E_17_ONB (E_16_ONB + 1) // 19
#define E_18_ONB (E_17_ONB + 1) // 20
#define G_01_ONB (E_18_ONB + 1) // 21
#define G_02_ONB (G_01_ONB + 1) // 22
#define G_03_ONB (G_02_ONB + 1) // 23
#define G_04_ONB (G_03_ONB + 1) // 24

#define REL3 (3 - 1)
#define REL5 (5 - 1)
#define REL4 (4 - 1)
#define REL6 (6 - 1)

#define REL16 (16 - 1)
#define CHDO2 (2 - 1)
#define CHDO3 (3 - 1)

#define REL3_REL4_REL5_REL6_BIT_VAL 0x3C // 0x3C <-  0011 1100
#define REL_3_4_5_6_SLOT_E_OUT_1_9_WR_VAL ((1 << REL3) | (1 << REL5) | (1 << REL4) | (0 << REL6) | (1 << REL16))
#define REL_3456_1010_SLOT_E_SELECT_FOR_WR ((1 << REL3) | (1 << REL5) | (0 << REL4) | (0 << REL6) | (1 << REL16))
#define REL_3_4_5_6_SLOT_E_OUT_10_18_WR_VAL ((1 << REL3) | (1 << REL5) | (0 << REL4) | (1 << REL6) | (1 << REL16))

  state_outputs_raw |= state_outputs_raw_dbg;

  unsigned long u32_rele_val = state_outputs_raw;                                                       //<- state out Rele
                                                                                                        //``````````````````````````````````````````````````````````````````
                                                                                                        //...........БП3
                                                                                                        //...........Двих.А.1      1
                                                                                                        //...........Двих.А.2      2
                                                                                                        //-REL1 REL2 - на блок хивлення
                                                                                                        //``````````````````````````````````````````````````````````````````
  unsigned char ch_REL1_REL2_VAL = (u32_rele_val & (1 << A_01_ONB)) | (u32_rele_val & (1 << A_02_ONB)); //А.1,А.2,

  volatile unsigned char ch_REL7_REL8_VAL = (((u32_rele_val & (1 << E_01_ONB)) >> E_01_ONB) << 6) | (((u32_rele_val & (1 << E_02_ONB)) >> E_02_ONB) << 7); //E1 E2 out

  volatile unsigned char ch_REL_3_4_5_6_WR_VAL = 0;

  extern volatile unsigned char chGbl__CDHO1_CDHO7__W_VAL;
  extern volatile unsigned char chGbl__REL1_REL8__W_VAL;
  //?chGbl__REL1_REL8__W_VAL &= 3;chGbl__REL1_REL8__W_VAL |= ch_REL1_REL2_VAL;
  //define DBG_MODE
  //Set CHDO
  unsigned char chLocal__CDHO1_CDHO7__W_VAL = (~((1 << CHDO2) | (1 << CHDO3))) & chGbl__CDHO1_CDHO7__W_VAL;
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD39_DD40_DD47) = chLocal__CDHO1_CDHO7__W_VAL | (1 << CHDO2) | (0 << CHDO3);
#ifdef DBG_MODE
#else
  unsigned short Rel_1__2_E1_E2__E3_E9_SLOT_E_WR_Val = ch_REL1_REL2_VAL | ch_REL7_REL8_VAL | (((u32_rele_val >> E_03_ONB) & 0x7f) << 8);
  //..=:>>_Установити на виходах регістрів БВ3 дані, потрібні для запису в виходи 1 – 9;<<-..
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37) = Rel_1__2_E1_E2__E3_E9_SLOT_E_WR_Val | (1 << REL16);
  //..^
  _DELAY_ABOUT_40NS()
  //..=:>>_Установити REL4 в «0», REL6 в «0»;<<-..
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37) = Rel_1__2_E1_E2__E3_E9_SLOT_E_WR_Val | (1 << REL16);
//..^
#endif

#ifdef DBG_MODE

#else
  //..=:>>_Установити REL3, REL5 в «1»<<-..
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37) =
    REL_3456_1010_SLOT_E_SELECT_FOR_WR | Rel_1__2_E1_E2__E3_E9_SLOT_E_WR_Val | (1 << REL16);
  //..^
  _DELAY_ABOUT_40NS()
#endif

#ifdef DBG_MODE

#else
  //..=:>>_Установити REL4 в «1» (записати дані у виходи 1-9)<<-..
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37) =
    REL_3_4_5_6_SLOT_E_OUT_1_9_WR_VAL | Rel_1__2_E1_E2__E3_E9_SLOT_E_WR_Val | (1 << REL16);
//..^
#endif
  //

#ifdef DBG_MODE
#else
  //..=:>>_Установити REL4 в «0»;<<-..
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37) =
    REL_3456_1010_SLOT_E_SELECT_FOR_WR | Rel_1__2_E1_E2__E3_E9_SLOT_E_WR_Val | (1 << REL16);
//..^
#endif

  ch_REL7_REL8_VAL = (((u32_rele_val & (1 << E_10_ONB)) >> E_10_ONB) << 6) | (((u32_rele_val & (1 << E_11_ONB)) >> E_11_ONB) << 7); //E10 E11 out

#ifdef DBG_MODE
#else
  //..=:>>_Установити на виходах регістрів БВ3 дані, потрібні для запису в виходи 10– 18;<<-..
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37) =
    ch_REL1_REL2_VAL | ch_REL7_REL8_VAL | (((u32_rele_val >> E_12_ONB) & 0x7f) << 8) | (1 << REL16);
//..^
#endif

  _DELAY_ABOUT_40NS()

#ifdef DBG_MODE

#else
  //..=:>>_Установити  REL6 в «1» (записати дані у виходи 10-18);<<-..
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37) =
    REL_3_4_5_6_SLOT_E_OUT_10_18_WR_VAL | ch_REL1_REL2_VAL | ch_REL7_REL8_VAL | (((u32_rele_val >> E_12_ONB) & 0x7f) << 8) | (1 << REL16);
//..^
#endif
#ifdef DBG_MODE

#else
  //..=:>>_Установити REL6 в «0»<<-..
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37) =
    REL_3456_1010_SLOT_E_SELECT_FOR_WR | ch_REL1_REL2_VAL | ch_REL7_REL8_VAL | (((u32_rele_val >> E_12_ONB) & 0x7f) << 8) | (1 << REL16);
//..^
#endif

  //REL9    Двих.E3 Двих.E12
  //REL10   Двих.E4 Двих.E13
  //REL11   Двих.E5 Двих.E14
  //REL12   Двих.E6 Двих.E15
  //REL13   Двих.E7 Двих.E16
  //REL14   Двих.E8 Двих.E17
  //REL15   Двих.E9 Двих.E18

#ifdef DBG_MODE
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37) = (unsigned short) (arU32[2] | ch_REL1_REL2_VAL | ch_REL7_REL8_VAL) | (1 << REL16);
#else
  //..=:>>_Установити REL3, REL5 в «0».<<-..
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37) = //CLEAR REL_3_4_5_6_SLOT_E_OUT ESTABLISH DATA
    ch_REL1_REL2_VAL | ch_REL7_REL8_VAL | (((u32_rele_val >> E_12_ONB) & 0x7f) << 8) | (1 << REL16);
//..^
#endif
  //БДВВ6
  //AOUT1-AOUT8 (DD28)
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD25_DD27_DD28_DD30) = ((u32_rele_val >> G_01_ONB) & 0xf) << 8; //
  chGbl__REL1_REL8__W_VAL = ch_REL1_REL2_VAL;                                                                  //
                                                                                                               //////////////////////////////////////////////////////////////////////////////////////////
  //?chGbl__REL1_REL8__W_VAL &= 3;chGbl__REL1_REL8__W_VAL |= ch_REL1_REL2_VAL;
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37) = chGbl__REL1_REL8__W_VAL | (1 << REL16); //Restore REL1_REL8?

  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD39_DD40_DD47) = chGbl__CDHO1_CDHO7__W_VAL; //Restore CDHO

  ///////////////////////////////////////////////////////////////////////////////////////////

#endif
  TIM_PRT_write_tick = TIM2->CNT;
  /**************************/

  /**************************/
  //Вивід інформації на світлодіоди
  /**************************/
  //Спочатку перевіряємо, чи не активовувалвся команда "Сблос индикации" - і якщо так, то попередньо скидаємо всю індикацію
  if (_CHECK_SET_BIT(active_functions, RANG_RESET_LEDS) != 0)
  {
    state_leds = 0;
  }

  //Визначаємо, які світлоіндикатори зараз мають бути активними
  for (unsigned int i = 0; i < NUMBER_LEDS; i++)
  {
    //У тимчасовий масив копіюємо ранжування світлоіндикатора, який індексується інедексом "i"
    unsigned int temp_array_of_leds[N_BIG];

    //Перевіряємо, чи є співпадіння між ранжованими функціями на цьому світлоіндикаторі і функціями, які зараз є активними - умова активації виходу
    for (unsigned int j = 0; j < N_BIG; j++)
      temp_array_of_leds[j] = current_settings_prt.ranguvannja_leds[N_BIG * i + j] & active_functions[j];

    flag = 0;
    for (int m = 0; m < N_BIG; m++)
      if (temp_array_of_leds[m] != 0)
      {
        flag = 1;
        break;
      }
    if (flag)
    {
      //Відмічаємо, що даний світлоіндикатор - ГОРИТЬ
      state_leds |= (1 << i);
    }
    else
    {
      //Перевіряємо, чи даний світлоіндикатор нормальний, чи тригерний
      if ((current_settings_prt.type_of_led & (1 << i)) == 0)
      {
        //Світлоіндикатор нормальний

        //Відмічаємо, що даний світлоіндикатор - ПОГАШЕНИЙ
        state_leds &= ~(1 << i);
      }
    }
  }
  //Перевіряємо чи треба записувати стан тригерних світлоіндикаторів у EEPROM
  if (
    (_CHECK_SET_BIT(active_functions, RANG_AVAR_DEFECT) == 0) &&
    ((state_leds & current_settings_prt.type_of_led) != state_trigger_leds))
  {
    state_trigger_leds = state_leds & current_settings_prt.type_of_led;
    //Виставляємо повідомлення про те, що в EEPROM треба записати нові значення сигнальних виходів і тригерних світлоіндикаторів
    _SET_BIT(control_spi1_taskes, TASK_START_WRITE_STATE_LEDS_OUTPUTS_EEPROM_BIT);
  }

  //Сервісні світлоіндикатори
  /*Run_Error*/
  if (_CHECK_SET_BIT(active_functions, RANG_AVAR_DEFECT) == 0)
    state_leds_ctrl |= (1 << LED_COLOR_GREEN_BIT) << ((uint32_t) NUMBER_LED_COLOR * (uint32_t) LED_CTRL_R_E);
  else
    state_leds_ctrl &= (uint32_t)(~((1 << LED_COLOR_GREEN_BIT) << ((uint32_t) NUMBER_LED_COLOR * (uint32_t) LED_CTRL_R_E)));
  if (
    (_CHECK_SET_BIT(active_functions, RANG_DEFECT) != 0) ||
    (_CHECK_SET_BIT(active_functions, RANG_AVAR_DEFECT) != 0))
    state_leds_ctrl |= (1 << LED_COLOR_RED_BIT) << ((uint32_t) NUMBER_LED_COLOR * (uint32_t) LED_CTRL_R_E);
  else
    state_leds_ctrl &= (uint32_t)(~((1 << LED_COLOR_RED_BIT) << ((uint32_t) NUMBER_LED_COLOR * (uint32_t) LED_CTRL_R_E)));

  static uint32_t state_leds_lock[2];
  static uint32_t state_leds_ctrl_lock[2];
  static uint32_t state_leds_Fx_lock[2][2];
  static size_t bank_lock;

  size_t bank_lock_tmp1 = bank_lock;
  size_t bank_lock_tmp2 = (bank_lock_tmp1 + 1) & 0x1;

  state_leds_lock[bank_lock_tmp2] |= state_leds;
  state_leds_ctrl_lock[bank_lock_tmp2] |= state_leds_ctrl;
  state_leds_Fx_lock[bank_lock_tmp2][0] |= state_leds_Fx[0];
  state_leds_Fx_lock[bank_lock_tmp2][1] |= state_leds_Fx[1];

  static uint32_t current_LED_N_COL;

  //Очищаємо попередню інформацію
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD32_DD38) = ((1 << current_LED_N_COL) << LED_N_ROW) | ((uint32_t)(~0) & ((1 << LED_N_ROW) - 1));

  uint32_t state_leds_tmp = 0;

  switch (current_LED_N_COL)
  {
    case 0:
      {
        uint32_t state_leds_auto = state_leds_lock[bank_lock_tmp1];

        state_leds_tmp = (((state_leds_auto >> 0) & 0x1) << 0) |
                         (((state_leds_auto >> 2) & 0x1) << 1) |
                         (((state_leds_auto >> 4) & 0x1) << 2) |
                         (((state_leds_auto >> 6) & 0x1) << 3) |
                         (((state_leds_auto >> 8) & 0x1) << 4) |
                         (((state_leds_auto >> 10) & 0x1) << 5) |
                         (((state_leds_auto >> 12) & 0x1) << 6) |
                         (((state_leds_auto >> 14) & 0x1) << 7);
        break;
      }
    case 1:
      {
        uint32_t state_leds_auto = state_leds_lock[bank_lock_tmp1];
        uint32_t state_leds_ctrl_auto = state_leds_ctrl_lock[bank_lock_tmp1];

        state_leds_tmp = (((state_leds_auto >> 1) & 0x1) << 0) |
                         (((state_leds_auto >> 3) & 0x1) << 1) |
                         (((state_leds_auto >> 5) & 0x1) << 2) |
                         (((state_leds_auto >> 7) & 0x1) << 3) |
                         (((state_leds_auto >> 9) & 0x1) << 4) |
                         (((state_leds_auto >> 11) & 0x1) << 5) |
                         (((state_leds_auto >> 13) & 0x1) << 6) |
                         ((((state_leds_ctrl_auto >> ((uint32_t) NUMBER_LED_COLOR * (uint32_t) LED_CTRL_O)) & (1 << LED_COLOR_RED_BIT)) != 0) << 7);
        break;
      }
    case 2:
      {
        uint32_t state_leds_auto = state_leds_lock[bank_lock_tmp1];
        uint32_t state_leds_ctrl_auto = state_leds_ctrl_lock[bank_lock_tmp1];
        uint32_t state_leds_Fx0_auto = state_leds_Fx_lock[bank_lock_tmp1][0];

        state_leds_tmp = ((((state_leds_ctrl_auto >> ((uint32_t) NUMBER_LED_COLOR * (uint32_t) LED_CTRL_R_E)) & (1 << LED_COLOR_RED_BIT)) != 0) << 0) |

                         (((state_leds_auto >> 15) & 0x1) << 1) |
                         (((state_leds_auto >> 16) & 0x1) << 2) |

                         ((((state_leds_Fx0_auto >> ((uint32_t) NUMBER_LED_COLOR * (uint32_t)(1 - 1))) & (1 << LED_COLOR_RED_BIT)) != 0) << 3) |
                         ((((state_leds_Fx0_auto >> ((uint32_t) NUMBER_LED_COLOR * (uint32_t)(2 - 1))) & (1 << LED_COLOR_RED_BIT)) != 0) << 4) |
                         ((((state_leds_Fx0_auto >> ((uint32_t) NUMBER_LED_COLOR * (uint32_t)(3 - 1))) & (1 << LED_COLOR_RED_BIT)) != 0) << 5) |
                         ((((state_leds_Fx0_auto >> ((uint32_t) NUMBER_LED_COLOR * (uint32_t)(4 - 1))) & (1 << LED_COLOR_RED_BIT)) != 0) << 6) |
                         ((((state_leds_Fx0_auto >> ((uint32_t) NUMBER_LED_COLOR * (uint32_t)(5 - 1))) & (1 << LED_COLOR_RED_BIT)) != 0) << 7);
        break;
      }
    case 3:
      {
        uint32_t state_leds_ctrl_auto = state_leds_ctrl_lock[bank_lock_tmp1];
        uint32_t state_leds_Fx0_auto = state_leds_Fx_lock[bank_lock_tmp1][0];

        state_leds_tmp = ((((state_leds_ctrl_auto >> ((uint32_t) NUMBER_LED_COLOR * (uint32_t) LED_CTRL_R_E)) & (1 << LED_COLOR_GREEN_BIT)) != 0) << 0) |

                         ((((state_leds_Fx0_auto >> ((uint32_t) NUMBER_LED_COLOR * (uint32_t)(1 - 1))) & (1 << LED_COLOR_GREEN_BIT)) != 0) << 3) |
                         ((((state_leds_Fx0_auto >> ((uint32_t) NUMBER_LED_COLOR * (uint32_t)(2 - 1))) & (1 << LED_COLOR_GREEN_BIT)) != 0) << 4) |
                         ((((state_leds_Fx0_auto >> ((uint32_t) NUMBER_LED_COLOR * (uint32_t)(3 - 1))) & (1 << LED_COLOR_GREEN_BIT)) != 0) << 5) |
                         ((((state_leds_Fx0_auto >> ((uint32_t) NUMBER_LED_COLOR * (uint32_t)(4 - 1))) & (1 << LED_COLOR_GREEN_BIT)) != 0) << 6) |
                         ((((state_leds_Fx0_auto >> ((uint32_t) NUMBER_LED_COLOR * (uint32_t)(5 - 1))) & (1 << LED_COLOR_GREEN_BIT)) != 0) << 7);

        break;
      }
    case 4:
      {
        uint32_t state_leds_ctrl_auto = state_leds_ctrl_lock[bank_lock_tmp1];
        uint32_t state_leds_Fx0_auto = state_leds_Fx_lock[bank_lock_tmp1][0];
        uint32_t state_leds_Fx1_auto = state_leds_Fx_lock[bank_lock_tmp1][1];

        state_leds_tmp = ((((state_leds_Fx0_auto >> ((uint32_t) NUMBER_LED_COLOR * (uint32_t)(6 - 1))) & (1 << LED_COLOR_RED_BIT)) != 0) << 0) |
                         ((((state_leds_Fx1_auto >> ((uint32_t) NUMBER_LED_COLOR * (uint32_t)(1 - 1))) & (1 << LED_COLOR_RED_BIT)) != 0) << 1) |
                         ((((state_leds_Fx1_auto >> ((uint32_t) NUMBER_LED_COLOR * (uint32_t)(2 - 1))) & (1 << LED_COLOR_RED_BIT)) != 0) << 2) |
                         ((((state_leds_Fx1_auto >> ((uint32_t) NUMBER_LED_COLOR * (uint32_t)(3 - 1))) & (1 << LED_COLOR_RED_BIT)) != 0) << 3) |
                         ((((state_leds_Fx1_auto >> ((uint32_t) NUMBER_LED_COLOR * (uint32_t)(4 - 1))) & (1 << LED_COLOR_RED_BIT)) != 0) << 4) |
                         ((((state_leds_Fx1_auto >> ((uint32_t) NUMBER_LED_COLOR * (uint32_t)(5 - 1))) & (1 << LED_COLOR_RED_BIT)) != 0) << 5) |
                         ((((state_leds_Fx1_auto >> ((uint32_t) NUMBER_LED_COLOR * (uint32_t)(6 - 1))) & (1 << LED_COLOR_RED_BIT)) != 0) << 6) |
                         ((((state_leds_ctrl_auto >> ((uint32_t) NUMBER_LED_COLOR * (uint32_t) LED_CTRL_I)) & (1 << LED_COLOR_RED_BIT)) != 0) << 7);
        break;
      }
    case 5:
      {
        uint32_t state_leds_ctrl_auto = state_leds_ctrl_lock[bank_lock_tmp1];
        uint32_t state_leds_Fx0_auto = state_leds_Fx_lock[bank_lock_tmp1][0];
        uint32_t state_leds_Fx1_auto = state_leds_Fx_lock[bank_lock_tmp1][1];

        state_leds_tmp = ((((state_leds_Fx0_auto >> ((uint32_t) NUMBER_LED_COLOR * (uint32_t)(6 - 1))) & (1 << LED_COLOR_GREEN_BIT)) != 0) << 0) |
                         ((((state_leds_Fx1_auto >> ((uint32_t) NUMBER_LED_COLOR * (uint32_t)(1 - 1))) & (1 << LED_COLOR_GREEN_BIT)) != 0) << 1) |
                         ((((state_leds_Fx1_auto >> ((uint32_t) NUMBER_LED_COLOR * (uint32_t)(2 - 1))) & (1 << LED_COLOR_GREEN_BIT)) != 0) << 2) |
                         ((((state_leds_Fx1_auto >> ((uint32_t) NUMBER_LED_COLOR * (uint32_t)(3 - 1))) & (1 << LED_COLOR_GREEN_BIT)) != 0) << 3) |
                         ((((state_leds_Fx1_auto >> ((uint32_t) NUMBER_LED_COLOR * (uint32_t)(4 - 1))) & (1 << LED_COLOR_GREEN_BIT)) != 0) << 4) |
                         ((((state_leds_Fx1_auto >> ((uint32_t) NUMBER_LED_COLOR * (uint32_t)(5 - 1))) & (1 << LED_COLOR_GREEN_BIT)) != 0) << 5) |
                         ((((state_leds_Fx1_auto >> ((uint32_t) NUMBER_LED_COLOR * (uint32_t)(6 - 1))) & (1 << LED_COLOR_GREEN_BIT)) != 0) << 6) |
                         ((((state_leds_ctrl_auto >> ((uint32_t) NUMBER_LED_COLOR * (uint32_t) LED_CTRL_I)) & (1 << LED_COLOR_GREEN_BIT)) != 0) << 7);
        break;
      }
    case 6:
      {
        uint32_t state_leds_ctrl_auto = state_leds_ctrl_lock[bank_lock_tmp1];

        state_leds_tmp = ((((state_leds_ctrl_auto >> ((uint32_t) NUMBER_LED_COLOR * (uint32_t) LED_CTRL_O)) & (1 << LED_COLOR_GREEN_BIT)) != 0) << 7);
        break;
      }
    default:
      {
        //Теоретично цього ніколи не мало б бути
        total_error_sw_fixed();
      }
  }

  //Виводимо інформацію по світлоіндикаторах на світлодіоди
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD32_DD38) = ((1 << current_LED_N_COL) << LED_N_ROW) | ((uint32_t)(~state_leds_tmp) & ((1 << LED_N_ROW) - 1));

  //Переходимо на наступний стовбець
  if (++current_LED_N_COL >= LED_N_COL)
  {
    current_LED_N_COL = 0;
    bank_lock = bank_lock_tmp2;

    state_leds_lock[bank_lock_tmp1] = 0;
    state_leds_ctrl_lock[bank_lock_tmp1] = 0;
    state_leds_Fx_lock[bank_lock_tmp1][0] = 0;
    state_leds_Fx_lock[bank_lock_tmp1][1] = 0;
  }
  /**************************/

  /**************************/
  //
  /**************************/
  /**************************/
}
/*****************************************************/

/*****************************************************/
//Переривання від таймеру TIM2, який обслуговує систему захистів
/*****************************************************/
void TIM2_IRQHandler(void)
{
#ifdef SYSTEM_VIEWER_ENABLE
  SEGGER_SYSVIEW_RecordEnterISR();
#endif

  if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
  {
    /***********************************************************************************************/
    //Переривання відбулося вік каналу 1, який генерує переривання кожні 1 мс, для опраціьовування таймерів і систем захистів
    /***********************************************************************************************/
    TIM2->SR = (uint16_t)((~(uint32_t) TIM_IT_CC1) & 0xffff);
    uint32_t current_tick = TIM2->CCR1;

    //Знімаємо мітку, що на останньому перериванні відбулася фіксація прийняття цілого фрейму по 485
    mark_current_tick_RS_485 = 0;
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
    //Знімаємо мітку, що на останньому перериванні відбулася фіксація прийняття цілого фрейму по LAN
    mark_current_tick_LAN = 0;
#endif

#ifdef _TEST_DURATION
    uint32_t const start_tick = TIM2->CNT;
#endif

    /*************************************
    Управління чсом у UNIX-форматі
    *************************************/
    clk_count += DELTA_TIME_FOR_TIMERS;

    time_ms += DELTA_TIME_FOR_TIMERS;
    if (time_ms >= 1000)
    {
      time_ms -= 1000;
      time_dat++;
    }

    if ((save_time_dat_h == 3) || (save_time_dat_l == 3))
    {
      //Процес запису нового часу
      if (save_time_dat_l == 3)
      {
        time_ms = time_ms_save_l;
        time_dat = time_dat_save_l;

        save_time_dat_l = 2;
      }
      if (save_time_dat_h == 3)
      {
        time_ms = time_ms_save_h;
        time_dat = time_dat_save_h;

        save_time_dat_h = 2;
      }

      if (copying_time_to_RTC == 1)
        copying_time_to_RTC = 0;
      realDateTime = true;
    }
    else
    {
      //Перевірка чи не потрібно забрати час з RTC
      if (copying_time_to_RTC == 1)
      {
        int32_t diff_ms = time_ms - time_ms_RTC;
        time_t diff_s = time_dat - time_dat_RTC;
        if (diff_ms < 0)
        {
          diff_ms += 1000;
          --diff_s;
        }
        if (llabs(diff_s * 1000 + diff_ms) > 2000)
        {
          time_ms = time_ms_RTC;
          time_dat = time_dat_RTC;
        }
        copying_time_to_RTC = 0;
        realDateTime = true;
      }
    }

    if (!copying_time_dat)
    {
      time_ms_copy = time_ms;
      time_dat_copy = time_dat;
    }
    /*************************************/

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
    /***********************************************************/
    //Прийом інформації з комунікаційної плати
    /***********************************************************/
    start_receive_data_via_CANAL1_MO();
    Canal1 = true;
    /***********************************************************/
#endif

    /***********************************************************/
    //Перевіряємо, чи відбувалися зміни настройок
    /***********************************************************/
    if (changed_settings == CHANGED_ETAP_ENDED) /*Це є умова, що нові дані підготовлені для передачі їх у роботу системою захистів (і при цьому зараз дані не змінюються)*/
    {
      //Копіюємо таблицю настройок у копію цієї таблиці але з якою працює (читає і змінює) тільки система захистів
      current_settings_prt = current_settings;

      //Помічаємо, що зміни прийняті системою захистів, але ще треба прийняти вимірювальною системою
      changed_settings = CHANGED_ETAP_ENDED_EXTRA_ETAP;
    }
    /***********************************************************/

    /***********************************************************/
    //Перевіряємо необхідність очистки дискретного реєстраторів
    /***********************************************************/
    if (
      ((clean_rejestrators & CLEAN_DR) != 0) &&
      ((control_tasks_dataflash & (TASK_MAMORY_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_DR |
                                   TASK_MAMORY_READ_DATAFLASH_FOR_DR_USB |
                                   TASK_MAMORY_READ_DATAFLASH_FOR_DR_RS485 |
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                                   TASK_MAMORY_READ_DATAFLASH_FOR_DR_LAN |
#endif
                                   TASK_MAMORY_READ_DATAFLASH_FOR_DR_MENU |
                                   TASK_MAMORY_READ_DATAFLASH_FOR_DR_MENU_SHORT)) == 0))
    {
      //Виставлено каманда очистити дискретного реєстратор

      //Виставляємо команду запису цієї структури у EEPROM
      _SET_BIT(control_spi1_taskes, TASK_START_WRITE_INFO_REJESTRATOR_DR_EEPROM_BIT);

      //Очищаємо структуру інформації по дискретному реєстраторі
      info_rejestrator_dr.next_address = MIN_ADDRESS_DR_AREA;
      info_rejestrator_dr.saving_execution = 0;
      info_rejestrator_dr.number_records = 0;

      //Помічаємо, що номер запису не вибраний
      number_record_of_dr_for_menu = 0xffff;
      number_record_of_dr_for_USB = 0xffff;
      number_record_of_dr_for_RS485 = 0xffff;
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
      number_record_of_dr_for_LAN = 0xffff;
#endif

      //Знімаємо команду очистки дискретного реєстратора
      clean_rejestrators &= (unsigned int) (~CLEAN_DR);
    }
    /***********************************************************/

    /***********************************************************/
    //Опрцювання логічних тайменрів і дискретних входів тільки коли настройки успішно прочитані
    /***********************************************************/
    clocking_global_timers();
    /***********************************************************/

    /***********************************************************/
    //Опрцювання функцій захистів
    /***********************************************************/
    //Діагностика вузлів, яку треба проводити кожен раз перед початком опрацьовуванням логіки пристрою
    uint32_t TIM_PRT_read_tick = TIM2->CNT;
#pragma diag_suppress = Pa082
    uint64_t TIM_PRT_delta_write_read;
    if (TIM_PRT_read_tick < TIM_PRT_write_tick)
      TIM_PRT_delta_write_read = (uint64_t) TIM_PRT_read_tick + 0x100000000ull - (uint64_t) TIM_PRT_write_tick;
    else
      TIM_PRT_delta_write_read = TIM_PRT_read_tick - TIM_PRT_write_tick;
    if (TIM_PRT_delta_write_read > (TIM2_MIN_PERIOD_WRITE_READ + 1))
    {
      unsigned int control_state_outputs =
#if (                             \
  (MODYFIKACIA_VERSII_PZ != 7) && \
  (MODYFIKACIA_VERSII_PZ != 17))
        ((~((unsigned int) (_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37)))) & ((1u << 16) - 1))
#else
        ((~((unsigned int) (_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37)))) & ((1u << 9) - 1)) |
        ((((~((unsigned int) (_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37)))) >> 14) & ((1u << 2) - 1)) << 9)
#endif

#if (                             \
  (MODYFIKACIA_VERSII_PZ == 5) || \
  (MODYFIKACIA_VERSII_PZ == 15))
        | (((~((unsigned int) (_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD25_DD27_DD28_DD30) >> 8))) & 0xf) << 16)
#elif (                           \
  (MODYFIKACIA_VERSII_PZ == 7) || \
  (MODYFIKACIA_VERSII_PZ == 17))
        | ((((~((unsigned int) (_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD25_DD27_DD28_DD30)))) >> 1) & ((1u << 1) - 1)) << 11)

#endif
        ;

#if (                             \
  (MODYFIKACIA_VERSII_PZ == 8) || \
  (MODYFIKACIA_VERSII_PZ == 18))
      extern volatile unsigned char chGbl__CDHO1_CDHO7__W_VAL;
      extern volatile unsigned char chGbl__REL1_REL8__W_VAL;
      //Set CHDO
      unsigned char chLocal__CDHO1_CDHO7__W_VAL = (~((1 << CHDO2) | (1 << CHDO3))) & chGbl__CDHO1_CDHO7__W_VAL;
      _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD39_DD40_DD47) = chLocal__CDHO1_CDHO7__W_VAL; //0-now
                                                                                                  //..=:>>_Установити REL3, REL4, REL5, REL6 в «0»;<<-..
#define REL_3_4_5_6_SLOT_E_IN_1_9_RD_VAL ((0 << REL3) | (1 << REL5) | (0 << REL4) | (0 << REL6))
      //#define COMENT
      //???#ifndef COMENT
      _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37) = (chGbl__REL1_REL8__W_VAL & 3) //CLEAR REL_3_4_5_6_SLOT_E_OUT ESTABLISH DATA
                                                                          | (1 << REL16);
      //..^
      unsigned char ch_REL1_REL2_R_VAL = (unsigned char) (_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37)); //регістр контролю REL1-REL8 (DD34)
      unsigned long u32_ctrl_rele_val = (~((unsigned long) ch_REL1_REL2_R_VAL)) & 0x3;                                        //А.1 А.2
      //..=:>>_Установити, REL5 в «1»;//Select Slot E
      _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37) = (chGbl__REL1_REL8__W_VAL & 3) | REL_3_4_5_6_SLOT_E_IN_1_9_RD_VAL | (1 << REL16);
      //..^
      _DELAY_ABOUT_40NS()

      //?u32_ctrl_rele_val |= ( ~( (unsigned long)(_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37)) ) )&0x3;//А.1 А.2
      //? asm volatile(
      //?   "bkpt 1"
      //?   );
      //Регістр контролю ДВ AOUT1-AOUT8 (кола AIN1-AIN8)   Регістр контролю ДВAOUT1-AOUT8(DD30)(кола AIN1-AIN8)
      unsigned long u32_ctrl_G_01_G_04 =
        ((((unsigned long) (_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD25_DD27_DD28_DD30)))) & 0xf00) >> 8;
      u32_ctrl_G_01_G_04 = (~((unsigned long) u32_ctrl_G_01_G_04)) & 0xf; //..
#ifdef DBG_MODE
#else
      unsigned long u32_ctrl_E_01_E_16 = (_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD33_DD36)) & 0xffff;
      _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD39_DD40_DD47) = chLocal__CDHO1_CDHO7__W_VAL;
      unsigned long u32_ctrl_E_16_E_18 =
        ((unsigned short) (_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37)) & ((1 << 2) | (1 << 3)) //C_R3  C_R4
         ) >>
        2;
#endif
      u32_ctrl_rele_val |= (u32_ctrl_E_01_E_16 << 2) | (u32_ctrl_E_16_E_18 << (16 + 2)) | (u32_ctrl_G_01_G_04 << (16 + 2 + 2));

      _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37) = chGbl__REL1_REL8__W_VAL | (1 << REL16); //Restore REL1_REL8?
      _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD39_DD40_DD47) = chGbl__CDHO1_CDHO7__W_VAL;                   //Restore CDHO
                                                                                                                  //???#endif
                                                                                                                  ///////////////////////////////////////////////////////////////////////////////////////////
      control_state_outputs = u32_ctrl_rele_val;                                                                  //^state_outputs_rawunsigned int control_state_outputs =
#endif

#ifdef NUMBER_DS
      unsigned int control_ds = (~((unsigned int) (_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD25_DD27_DD28_DD30) >> 8))) & (MASKA_FOR_BIT(NUMBER_DS) - 1);
#endif

      static uint32_t error_rele[NUMBER_OUTPUTS];
      if (control_state_outputs != state_outputs_raw)
      {
        for (size_t index = 0; index < NUMBER_SIMPLE_OUTPUTS; ++index)
        {
          uint32_t maska = 1 << index;

          if ((control_state_outputs & maska) != (state_outputs_raw & maska))
          {
            if (error_rele[index] < 3)
              ++error_rele[index];
            if (error_rele[index] >= 3)
              _SET_BIT(set_diagnostyka, (ERROR_DIGITAL_OUTPUT_1_BIT + index));
          }
          else
            error_rele[index] = 0;
        }
      }
      else
      {
        for (size_t index = 0; index < NUMBER_SIMPLE_OUTPUTS; ++index)
          error_rele[index] = 0;
      }

#ifdef NUMBER_DS
      if (control_ds != ds)
      {
        for (size_t index = 0; index < NUMBER_DS; ++index)
        {
          uint32_t maska = 1 << index;

          if ((control_ds & maska) != (ds & maska))
          {
            if (error_rele[NUMBER_SIMPLE_OUTPUTS + index] < 3)
              ++error_rele[NUMBER_SIMPLE_OUTPUTS + index];
            if (error_rele[NUMBER_SIMPLE_OUTPUTS + index] >= 3)
              _SET_BIT(set_diagnostyka, (ERROR_DS_OUTPUT_BIT + index));
          }
          else
            error_rele[NUMBER_SIMPLE_OUTPUTS + index] = 0;
        }
      }
      else
      {
        for (size_t index = 0; index < NUMBER_DS; ++index)
          error_rele[NUMBER_SIMPLE_OUTPUTS + index] = 0;
      }
#endif
    }

    //Функції захистів
    main_protection();

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
    /***
    Очікування, щоб попередній пакет гарантовано завершив передаватися
    ***/
    //    while ((DMA_StreamCANAL1_MO_Tx->CR & DMA_IT_TC) != 0);
    while (DMA_StreamCANAL1_MO_Tx->NDTR != 0)
      ;

    //    uint32_t tick_for_Canal_1_tmp = tick_for_Canal_1;
    uint32_t tick_for_Canal_1_tmp = TIM2->CNT;
    uint64_t delta_tmp = 0;
    do
    {
      uint32_t tick_tmp = TIM2->CNT;
      delta_tmp = (tick_tmp < tick_for_Canal_1_tmp) ? ((uint64_t) tick_tmp + 0x100000000llu - (uint64_t) tick_for_Canal_1_tmp) : (tick_tmp - tick_for_Canal_1_tmp);
    } while (delta_tmp < 2);
    /***/

    if (
      (_CHECK_SET_BIT(diagnostyka, WARNING_REPROGRAM) == 0) &&
      (_CHECK_SET_BIT(set_diagnostyka, WARNING_REPROGRAM) == 0))
    {
      //Ініціюємо передачу даних по каналу CANAL1_MO у комунікаційну плату
      start_transmint_data_via_CANAL1_MO();
    }
#endif
    /***********************************************************/

    /***********************************************************/
    //Перевірка на необхідність зроботи резервні копії даних для самоконтролю
    /***********************************************************/
    //Триґерна інформація
    if (periodical_tasks_TEST_TRG_FUNC != 0)
    {
      //Стоїть у черзі активна задача зроботи резервні копії даних
      if ((state_spi1_task & STATE_TRG_FUNC_EEPROM_GOOD) != 0)
      {
        //Робимо копію тільки тоді, коли триґерна інформація успішно зчитана і сформована контрольна сума
        if (
          (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_WRITE_TRG_FUNC_EEPROM_BIT) == 0) &&
          (_CHECK_SET_BIT(control_spi1_taskes, TASK_WRITING_TRG_FUNC_EEPROM_BIT) == 0) &&
          (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_READ_TRG_FUNC_EEPROM_BIT) == 0) &&
          (_CHECK_SET_BIT(control_spi1_taskes, TASK_READING_TRG_FUNC_EEPROM_BIT) == 0))
        {
          //На даний моммент не іде читання-запис триґерної інформації, тому можна здійснити копіювання
          fix_active_buttons_ctrl = fix_active_buttons;
          for (unsigned int i = 0; i < N_BIG; i++)
            trigger_active_functions_ctrl[i] = trigger_active_functions[i];
          crc_trg_func_ctrl = crc_trg_func;

          //Скидаємо активну задачу формування резервної копії
          periodical_tasks_TEST_TRG_FUNC = false;
          //Виставляємо активну задачу контролю достовірності по резервній копії
          periodical_tasks_TEST_TRG_FUNC_LOCK = true;
        }
      }
      else
      {
        //Скидаємо активну задачу формування резервної копії
        periodical_tasks_TEST_TRG_FUNC = false;
      }
    }

    //Аналоговий реєстратор
    if (periodical_tasks_TEST_INFO_REJESTRATOR_AR != 0)
    {
      //Стоїть у черзі активна задача зроботи резервні копії даних
      if ((state_spi1_task & STATE_INFO_REJESTRATOR_AR_EEPROM_GOOD) != 0)
      {
        //Робимо копію тільки тоді, коли структура інформації реєстратора успішно зчитана і сформована контрольна сума
        if (
          (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_WRITE_INFO_REJESTRATOR_AR_EEPROM_BIT) == 0) &&
          (_CHECK_SET_BIT(control_spi1_taskes, TASK_WRITING_INFO_REJESTRATOR_AR_EEPROM_BIT) == 0) &&
          (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_READ_INFO_REJESTRATOR_AR_EEPROM_BIT) == 0) &&
          (_CHECK_SET_BIT(control_spi1_taskes, TASK_READING_INFO_REJESTRATOR_AR_EEPROM_BIT) == 0))
        {
          //На даний моммент не іде читання-запис структури інформації реєстратора, тому можна здійснити копіювання
          info_rejestrator_ar_ctrl = info_rejestrator_ar;
          crc_info_rejestrator_ar_ctrl = crc_info_rejestrator_ar;

          //Скидаємо активну задачу формування резервної копії
          periodical_tasks_TEST_INFO_REJESTRATOR_AR = false;
          //Виставляємо активну задачу контролю достовірності по резервній копії
          periodical_tasks_TEST_INFO_REJESTRATOR_AR_LOCK = true;
        }
      }
      else
      {
        //Скидаємо активну задачу формування резервної копії
        periodical_tasks_TEST_INFO_REJESTRATOR_AR = false;
      }
    }

    //Дискретний реєстратор
    if (periodical_tasks_TEST_INFO_REJESTRATOR_DR != 0)
    {
      //Стоїть у черзі активна задача зроботи резервні копії даних
      if ((state_spi1_task & STATE_INFO_REJESTRATOR_DR_EEPROM_GOOD) != 0)
      {
        //Робимо копію тільки тоді, коли структура інформації реєстратора успішно зчитана і сформована контрольна сума
        if (
          (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_WRITE_INFO_REJESTRATOR_DR_EEPROM_BIT) == 0) &&
          (_CHECK_SET_BIT(control_spi1_taskes, TASK_WRITING_INFO_REJESTRATOR_DR_EEPROM_BIT) == 0) &&
          (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_READ_INFO_REJESTRATOR_DR_EEPROM_BIT) == 0) &&
          (_CHECK_SET_BIT(control_spi1_taskes, TASK_READING_INFO_REJESTRATOR_DR_EEPROM_BIT) == 0))
        {
          //На даний моммент не іде читання-запис структури інформації реєстратора, тому можна здійснити копіювання
          info_rejestrator_dr_ctrl = info_rejestrator_dr;
          crc_info_rejestrator_dr_ctrl = crc_info_rejestrator_dr;

          //Скидаємо активну задачу формування резервної копії
          periodical_tasks_TEST_INFO_REJESTRATOR_DR = false;
          //Виставляємо активну задачу контролю достовірності по резервній копії
          periodical_tasks_TEST_INFO_REJESTRATOR_DR_LOCK = true;
        }
      }
      else
      {
        //Скидаємо активну задачу формування резервної копії
        periodical_tasks_TEST_INFO_REJESTRATOR_DR = false;
      }
    }
    /***********************************************************/

    //Лічильник ресурсу
    if (periodical_tasks_TEST_RESURS != 0)
    {
      //Стоїть у черзі активна задача зроботи резервні копії даних
      if ((state_spi1_task & STATE_RESURS_EEPROM_GOOD) != 0)
      {
        //Робимо копію тільки тоді, коли інформаціz успішно зчитана і сформована контрольна сума
        if (
          (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_WRITE_RESURS_EEPROM_BIT) == 0) &&
          (_CHECK_SET_BIT(control_spi1_taskes, TASK_WRITING_RESURS_EEPROM_BIT) == 0) &&
          (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_READ_RESURS_EEPROM_BIT) == 0) &&
          (_CHECK_SET_BIT(control_spi1_taskes, TASK_READING_RESURS_EEPROM_BIT) == 0))
        {
          //На даний моммент не іде читання-запис структури інформації реєстратора, тому можна здійснити копіювання
          counter_today_ctrl = counter_today;
          counter_previous_day_ctrl = counter_previous_day;
          counter_total_ctrl = counter_total;
          crc_resurs_ctrl = crc_resurs;

          //Скидаємо активну задачу формування резервної копії
          periodical_tasks_TEST_RESURS = false;
          //Виставляємо активну задачу контролю достовірності по резервній копії
          periodical_tasks_TEST_RESURS_LOCK = true;
        }
      }
      else
      {
        //Скидаємо активну задачу формування резервної копії
        periodical_tasks_TEST_RESURS = false;
      }
    }

    /***********************************************************/
    //Встановлюємо "значення лічильника для наступного переривання"
    /***********************************************************/
    uint32_t capture_new;
    unsigned int delta;
    TIM2->CCR1 = (capture_new = (current_tick + (delta = TIM2_CCR1_VAL)));

    unsigned int repeat;
    unsigned int previous_tick = current_tick;
    do
    {
      repeat = 0;
      current_tick = TIM2->CNT;

      uint64_t delta_time = 0;
      if (capture_new < current_tick)
        delta_time = (uint64_t) capture_new + 0x100000000ull - (uint64_t) current_tick;
      else
        delta_time = capture_new - current_tick;

      if ((delta_time > delta) || (delta_time == 0))
      {

        //?asm volatile(
        //?  "bkpt 1"
        //?);
        if (TIM_GetITStatus(TIM2, TIM_IT_CC1) == RESET)
        {
          if (delta < TIM2_CCR1_VAL)
          {
            uint64_t delta_tick;
            if (current_tick < previous_tick)
              delta_tick = (uint64_t) current_tick + 0x100000000ull - (uint64_t) previous_tick;
            else
              delta_tick = current_tick - previous_tick;

            delta = delta_tick + 1;
          }
          else if (delta == TIM2_CCR1_VAL)
            delta = 1; /*Намагаємося, щоб нове переивання запустилося як омога скоріше*/
          else
          {
            //Теоретично цього ніколи не мало б бути
            total_error_sw_fixed();
          }
          TIM2->CCR1 = (capture_new = (TIM2->CNT + delta));
          previous_tick = current_tick;
          repeat = 0xff;
        }
      }
    } while (repeat != 0);
    /***********************************************************/

    /***********************************************************/
    //Виставляємо повідомлення про те, система захисів праціює
    /***********************************************************/
    control_word_of_watchdog |= WATCHDOG_PROTECTION;
    /***********************************************************/
    /***********************************************************************************************/

#ifdef _TEST_DURATION
    uint32_t const stop_tick = TIM2->CNT;
    uint64_t const delta_tick = (stop_tick > start_tick) ? (stop_tick - start_tick) : (0x100000000ull + stop_tick - start_tick);

    static uint64_t durLogicMax = 0;
    static uint64_t durLogicMin = 0x100000000ull;
    static uint32_t durLogicReset;

    if (durLogicReset != 0)
    {
      durLogicMax = 0;
      durLogicMin = 0x100000000ull;
      durLogicReset = 0;
    }
    durLogicCur = delta_tick;
    if (durLogicCur > (3000))
    {
      //.  while(1);
      //.      //asm volatile(
      //.      //  "bkpt 1"
      //.      //);
      _SET_BIT(set_diagnostyka, TEST_OVD3);
    }
    if (durLogicMax < delta_tick)
      durLogicMax = delta_tick;
    if (durLogicMin > delta_tick)
      durLogicMin = delta_tick;

    static uint64_t periodLogicMax = 0;
    static uint64_t periodLogicMin = 0x100000000ull;
    static uint32_t periodLogicReset = 1; /*щоб першу точку неправильно гарантовано пропустити*/

    static uint32_t prev_start_tick;
    uint64_t const period_tick = (start_tick > prev_start_tick) ? (start_tick - prev_start_tick) : (0x100000000ull + start_tick - prev_start_tick);
    prev_start_tick = start_tick;

    periodLogicCur = period_tick;
    if (periodLogicMax < period_tick)
      periodLogicMax = period_tick;
    if (periodLogicMin > period_tick)
      periodLogicMin = period_tick;
    if (periodLogicReset != 0)
    {
      periodLogicMax = 0;
      periodLogicMin = 0x100000000ull;
      periodLogicReset = 0;
    }
#endif
  }
  else
  {
    total_error_sw_fixed();
  }

#ifdef SYSTEM_VIEWER_ENABLE
  SEGGER_SYSVIEW_RecordExitISR();
#endif
}
/*****************************************************/

/*****************************************************/
//Вибір групи уставок
/*****************************************************/
void setpoints_selecting(unsigned int *p_active_functions, unsigned int act_inp_gr_ustavok)
{
  unsigned int grupa_ustavok = 0;
  act_inp_gr_ustavok &= 0xf;
  if (current_settings_prt.grupa_ustavok < SETPOINT_GRUPA_USTAVOK_MIN ||
      current_settings_prt.grupa_ustavok > SETPOINT_GRUPA_USTAVOK_MAX)
  {
    //Неопределенная ошибка
    while (1)
      ;
  }
  else
  {
    grupa_ustavok = 1 << (current_settings_prt.grupa_ustavok - 1);
  }

  if (_CHECK_SET_BIT(p_active_functions, RANG_INVERS_DV_GRUPA_USTAVOK))
    _SET_STATE(act_inp_gr_ustavok, 4);

  _AND2(grupa_ustavok, 0, act_inp_gr_ustavok, 4, grupa_ustavok, 4);
  _AND2(grupa_ustavok, 1, act_inp_gr_ustavok, 4, grupa_ustavok, 5);
  _AND2(grupa_ustavok, 2, act_inp_gr_ustavok, 4, grupa_ustavok, 6);
  _AND2(grupa_ustavok, 3, act_inp_gr_ustavok, 4, grupa_ustavok, 7);

  unsigned int tmp = gr_ustavok_tmp;
  do
  {
    _AND4(act_inp_gr_ustavok, 0, gr_ustavok_tmp, 1, gr_ustavok_tmp, 2, gr_ustavok_tmp, 3, act_inp_gr_ustavok, 5);
    _INVERTOR(act_inp_gr_ustavok, 5, tmp, 0);

    _AND4(tmp, 0, act_inp_gr_ustavok, 1, tmp, 2, tmp, 3, act_inp_gr_ustavok, 6);
    _INVERTOR(act_inp_gr_ustavok, 6, tmp, 1);

    _AND4(tmp, 0, tmp, 1, act_inp_gr_ustavok, 2, tmp, 3, act_inp_gr_ustavok, 7);
    _INVERTOR(act_inp_gr_ustavok, 7, tmp, 2);

    _AND4(tmp, 0, tmp, 1, tmp, 2, act_inp_gr_ustavok, 3, act_inp_gr_ustavok, 8);
    _INVERTOR(act_inp_gr_ustavok, 8, tmp, 3);
    if (tmp == gr_ustavok_tmp)
    {
      break;
    }
    gr_ustavok_tmp = tmp;
  } while (1);

  _OR2(grupa_ustavok, 4, act_inp_gr_ustavok, 5, grupa_ustavok, 8);
  _OR2(grupa_ustavok, 5, act_inp_gr_ustavok, 6, grupa_ustavok, 9);
  _OR2(grupa_ustavok, 6, act_inp_gr_ustavok, 7, grupa_ustavok, 10);
  _OR2(grupa_ustavok, 7, act_inp_gr_ustavok, 8, grupa_ustavok, 11);

  if (_GET_STATE(grupa_ustavok, 8))
    _SET_BIT(p_active_functions, RANG_1_GRUPA_USTAVOK);
  else
    _CLEAR_BIT(p_active_functions, RANG_1_GRUPA_USTAVOK);

  if (_GET_STATE(grupa_ustavok, 9))
    _SET_BIT(p_active_functions, RANG_2_GRUPA_USTAVOK);
  else
    _CLEAR_BIT(p_active_functions, RANG_2_GRUPA_USTAVOK);

  if (_GET_STATE(grupa_ustavok, 10))
    _SET_BIT(p_active_functions, RANG_3_GRUPA_USTAVOK);
  else
    _CLEAR_BIT(p_active_functions, RANG_3_GRUPA_USTAVOK);

  if (_GET_STATE(grupa_ustavok, 11))
    _SET_BIT(p_active_functions, RANG_4_GRUPA_USTAVOK);
  else
    _CLEAR_BIT(p_active_functions, RANG_4_GRUPA_USTAVOK);
}
/*****************************************************/

/*****************************************************/

/*****************************************************/
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
void proc_Gs_blk_out(void *pv, unsigned long lCtrGsSrc, short *p_arrOrdNumsGsSignal)
{

  // ----------------    -------------------------
  register unsigned long i, lV, j;

  register void *pvll;

  pvll = (void *) &current_settings_prt.ranguvannja_In_GOOSE; //
  i = j = lV = 0;

  asm("nop" ::"r"(i), "r"(j), "r"(lV));
  while (lCtrGsSrc)
  {
    lCtrGsSrc--;
    lV = p_arrOrdNumsGsSignal[lCtrGsSrc];
    i = lV >> 3;
    j = lV - (i << 3);

    lV = i * N_IN_GOOSE_MMS_OUT * N_SMALL + j * N_SMALL;
    for (register unsigned long k = 0; k < N_SMALL; k++)
    {
      ((unsigned long *) pv)[k] |= ((unsigned long *) pvll + lV)[k];
    }
  }
}
void proc_Mms_blk_out(void *pv, unsigned long lCtrMmsSrc, short *p_arrOrdNumsMmsSignal)
{

  // ----------------    -------------------------
  register unsigned long i, lV, j;

  register void *pvll;

  pvll = (void *) &current_settings_prt.ranguvannja_In_MMS; //
  i = j = lV = 0;

  asm("nop" ::"r"(i), "r"(j), "r"(lV));
  while (lCtrMmsSrc)
  {
    lCtrMmsSrc--;
    lV = p_arrOrdNumsMmsSignal[lCtrMmsSrc];
    i = lV >> 3;
    j = lV - (i << 3);

    lV = i * N_IN_GOOSE_MMS_OUT * N_SMALL + j * N_SMALL;
    for (register unsigned long k = 0; k < N_SMALL; k++)
    {
      ((unsigned long *) pv)[k] |= ((unsigned long *) pvll + lV)[k];
    }
  }
  //
  /*
		2. У момент ,коли ти активовуєш "Вхід ВФх" (вхід будь-якої визначуваної функції) з MMS (GOOSE це не стосується!), то тобі треба запустити таймер:

global_timers[INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START + _n] = 0;

Де _n - це номер визначуваної функції

Все.
-----------------------------------------

Для прикладу, як я це роблю у рядках 10140-10152 protection.c

Це недолік у програмі. Бо неможливо активуватьи визначувану функцію з MMS, якщо у неї виставлено таймер павзи ненульовий. Тому, я вважаю, що це неайпріоритетніша робота зараз для тебе мала б бути.
		*/
  //.		if (_CHECK_SET_BIT(((unsigned long*)pv), RANG_SMALL_DF1_IN) == 0){
  //.			//Зараз має активуватися В-ФункціяХ, тому треба запустити таймер її утримування,
  //.			//для того, щоб потім час цей можна було зрівняти з часом таймера павзи
  //.			if (global_timers[INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START + 0] < 0)
  //.			{
  //.			//Запускаємо таймер таймер утримування цієї функції в активному стані (емітація активного входу)
  //.			//Запуск робимо тільки ту тому випадкук, якщо він ще не почався
  //.			global_timers[INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START + 0] = 0;
  //.			}
  //.		}
  //.		if (_CHECK_SET_BIT(((unsigned long*)pv), RANG_SMALL_DF2_IN) == 0){
  //.			//Зараз має активуватися В-ФункціяХ, тому треба запустити таймер її утримування,
  //.			//для того, щоб потім час цей можна було зрівняти з часом таймера павзи
  //.			if (global_timers[INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START + 1] < 0)
  //.			{
  //.			//Запускаємо таймер таймер утримування цієї функції в активному стані (емітація активного входу)
  //.			//Запуск робимо тільки ту тому випадкук, якщо він ще не почався
  //.			global_timers[INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START + 1] = 0;
  //.			}
  //.		}
  //.		if (_CHECK_SET_BIT(((unsigned long*)pv), RANG_SMALL_DF3_IN) == 0){
  //.			//Зараз має активуватися В-ФункціяХ, тому треба запустити таймер її утримування,
  //.			//для того, щоб потім час цей можна було зрівняти з часом таймера павзи
  //.			if (global_timers[INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START + 2] < 0)
  //.			{
  //.			//Запускаємо таймер таймер утримування цієї функції в активному стані (емітація активного входу)
  //.			//Запуск робимо тільки ту тому випадкук, якщо він ще не почався
  //.			global_timers[INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START + 2] = 0;
  //.			}
  //.		}
  //.		if (_CHECK_SET_BIT(((unsigned long*)pv), RANG_SMALL_DF4_IN) == 0){
  //.			//Зараз має активуватися В-ФункціяХ, тому треба запустити таймер її утримування,
  //.			//для того, щоб потім час цей можна було зрівняти з часом таймера павзи
  //.			if (global_timers[INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START + 3] < 0)
  //.			{
  //.			//Запускаємо таймер таймер утримування цієї функції в активному стані (емітація активного входу)
  //.			//Запуск робимо тільки ту тому випадкук, якщо він ще не почався
  //.			global_timers[INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START + 3] = 0;
  //.			}
  //.		}
  //.		if (_CHECK_SET_BIT(((unsigned long*)pv), RANG_SMALL_DF5_IN) == 0){
  //.			//Зараз має активуватися В-ФункціяХ, тому треба запустити таймер її утримування,
  //.			//для того, щоб потім час цей можна було зрівняти з часом таймера павзи
  //.			if (global_timers[INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START + 4] < 0)
  //.			{
  //.			//Запускаємо таймер таймер утримування цієї функції в активному стані (емітація активного входу)
  //.			//Запуск робимо тільки ту тому випадкук, якщо він ще не почався
  //.			global_timers[INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START + 4] = 0;
  //.			}
  //.		}
  //.		if (_CHECK_SET_BIT(((unsigned long*)pv), RANG_SMALL_DF6_IN) == 0){
  //.			//Зараз має активуватися В-ФункціяХ, тому треба запустити таймер її утримування,
  //.			//для того, щоб потім час цей можна було зрівняти з часом таймера павзи
  //.			if (global_timers[INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START + 5] < 0)
  //.			{
  //.			//Запускаємо таймер таймер утримування цієї функції в активному стані (емітація активного входу)
  //.			//Запуск робимо тільки ту тому випадкук, якщо він ще не почався
  //.			global_timers[INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START + 5] = 0;
  //.			}
  //.		}
  //.		if (_CHECK_SET_BIT(((unsigned long*)pv), RANG_SMALL_DF7_IN) == 0){
  //.			//Зараз має активуватися В-ФункціяХ, тому треба запустити таймер її утримування,
  //.			//для того, щоб потім час цей можна було зрівняти з часом таймера павзи
  //.			if (global_timers[INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START + 6] < 0)
  //.			{
  //.			//Запускаємо таймер таймер утримування цієї функції в активному стані (емітація активного входу)
  //.			//Запуск робимо тільки ту тому випадкук, якщо він ще не почався
  //.			global_timers[INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START + 6] = 0;
  //.			}
  //.		}
  //.		if (_CHECK_SET_BIT(((unsigned long*)pv), RANG_SMALL_DF8_IN) == 0){
  //.			//Зараз має активуватися В-ФункціяХ, тому треба запустити таймер її утримування,
  //.			//для того, щоб потім час цей можна було зрівняти з часом таймера павзи
  //.			if (global_timers[INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START + 7] < 0)
  //.			{
  //.			//Запускаємо таймер таймер утримування цієї функції в активному стані (емітація активного входу)
  //.			//Запуск робимо тільки ту тому випадкук, якщо він ще не почався
  //.			global_timers[INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START + 7] = 0;
  //.			}
  //.		}
}

void proc_Lan_blk_out(unsigned short *p_rang_Out_LAN, unsigned int *p_active_functions, void *pLanDsc)
{
  register unsigned long rU_V, rU_bit, rU_out, rU_Idx;

  unsigned long IdxBlk, l_O;
  // ----------------    -------------------------
  IdxBlk = *((unsigned char *) pLanDsc);
  l_O = 0; //sLV.IdxBlk = LAN_BLOCK_ORD_NUM_00;
  for (rU_out = 0; rU_out < 8; rU_out++)
  {
    rU_bit = rU_V = 0;
    while (rU_bit < MAX_FUNCTIONS_IN_OUT_LAN)
    {
      //
      rU_V = (IdxBlk * N_OUT_LAN_IN * MAX_FUNCTIONS_IN_OUT_LAN) + (rU_out * MAX_FUNCTIONS_IN_OUT_LAN) + rU_bit; //find index
      rU_Idx = p_rang_Out_LAN[rU_V];                                                                            //find index
      if (rU_Idx > 0)
      {
        rU_Idx--;
        //Create 32 bit mask
        rU_V = p_active_functions[rU_Idx >> 5] & ((unsigned int) (1 << (rU_Idx & 0x1f)));
        if (rU_V != 0)
        {
          ; //Set Val
          rU_bit = MAX_FUNCTIONS_IN_OUT_LAN + 1;
        }
      }
      rU_bit++;
    }
    rU_Idx = IdxBlk; //sLV.IdxBlk;
    if (rU_bit == (MAX_FUNCTIONS_IN_OUT_LAN + 2))
    { //Activate Out
      //((unsigned char*)pLanDsc+1)[rU_Idx] |= 1<< rU_out;
      l_O |= 1 << rU_out;
    }
  }
  Output_Out_LAN_block[IdxBlk] = l_O;
}

#endif
/*****************************************************/

#ifdef DEBUG_TEST
#warning "TEST VARIABLES IS PRESENT"
#endif
