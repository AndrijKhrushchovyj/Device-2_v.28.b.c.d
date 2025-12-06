#include "header.h"

#ifdef _TEST_DURATION    
uint64_t durLogicCur = 0;
uint64_t periodLogicCur = 0;
#endif

static int32_t p_rele_reprogram_command = -1;
static int32_t p_rele_staff_command = -1;
static int32_t timerWaitReprogram = -1;

unsigned int const monitoring_3I0_1g_signals[N_BIG] =
{
  MASKA_MONITOTYNG_3I0_1G_SIGNALES_0,
  MASKA_MONITOTYNG_3I0_1G_SIGNALES_1,
  MASKA_MONITOTYNG_3I0_1G_SIGNALES_2,
  MASKA_MONITOTYNG_3I0_1G_SIGNALES_3,
  MASKA_MONITOTYNG_3I0_1G_SIGNALES_4,
  MASKA_MONITOTYNG_3I0_1G_SIGNALES_5,
  MASKA_MONITOTYNG_3I0_1G_SIGNALES_6,
  MASKA_MONITOTYNG_3I0_1G_SIGNALES_7,
  MASKA_MONITOTYNG_3I0_1G_SIGNALES_8
    
#ifdef MASKA_MONITOTYNG_3I0_1G_SIGNALES_9
                                    ,
  MASKA_MONITOTYNG_3I0_1G_SIGNALES_9
    
#endif
};

unsigned int const monitoring_3I0_otherg_signals[N_BIG] =
{
  MASKA_MONITOTYNG_3I0_OTHERG_SIGNALES_0,
  MASKA_MONITOTYNG_3I0_OTHERG_SIGNALES_1,
  MASKA_MONITOTYNG_3I0_OTHERG_SIGNALES_2,
  MASKA_MONITOTYNG_3I0_OTHERG_SIGNALES_3,
  MASKA_MONITOTYNG_3I0_OTHERG_SIGNALES_4,
  MASKA_MONITOTYNG_3I0_OTHERG_SIGNALES_5,
  MASKA_MONITOTYNG_3I0_OTHERG_SIGNALES_6,
  MASKA_MONITOTYNG_3I0_OTHERG_SIGNALES_7,
  MASKA_MONITOTYNG_3I0_OTHERG_SIGNALES_8
    
#ifdef MASKA_MONITOTYNG_3I0_OTHERG_SIGNALES_9
                                        ,
  MASKA_MONITOTYNG_3I0_OTHERG_SIGNALES_9
    
#endif
};

size_t head_queue_dr = 0;
unsigned int const vmp_start_val[2] = {(unsigned int)UNDEF_VMP, 0};

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
      if (*p <= (0x7fffffff - DELTA_TIME_FOR_TIMERS)) *p += DELTA_TIME_FOR_TIMERS;
    }
  }


  static unsigned int power_is;
  if ((POWER_CTRL->IDR & POWER_CTRL_PIN) != (uint32_t)Bit_RESET) 
  {
    power_is = true;

    timePowerDown_total = timePowerDown = -1;
  }
  else
  {
    if (
        (measurement[IM_IA] > POWEER_IS_FROM_IA_IC) ||
        (measurement[IM_IC] > POWEER_IS_FROM_IA_IC)
       )
    {
      power_is = true;
    }
    else if (
             (measurement[IM_IA] < POWEER_ISNOT_FROM_IA_IC) &&
             (measurement[IM_IC] < POWEER_ISNOT_FROM_IA_IC)
            )
    {
      power_is = false;
    }
    else
    {
      //power_is залишається без змін
    }
    
    if (timePowerDown < 0) timePowerDown = 0;
    else if (power_is) timePowerDown = 0;
    else if (
             (timePowerDown >= 0) &&
             (timePowerDown <= (0x7fffffff - DELTA_TIME_FOR_TIMERS))  
            )
    {
      timePowerDown += DELTA_TIME_FOR_TIMERS;
    }

    if (timePowerDown_total < 0) timePowerDown_total = 0;
    else if (
             (timePowerDown_total >= 0) &&
             (timePowerDown_total <= (0x7fffffff - DELTA_TIME_FOR_TIMERS))  
            )
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
      (p_rele_staff_command < 0)  
     )
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
    int32_t * point_p_rele = NULL;
    if (p_rele_reprogram_command >= 0) point_p_rele = &p_rele_reprogram_command;
    else point_p_rele = &p_rele_staff_command;

    if (*point_p_rele < (0x7fffffff - DELTA_TIME_FOR_TIMERS)) *point_p_rele += DELTA_TIME_FOR_TIMERS;
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
  
  if (p_rele_reprogram_command == 0) timerWaitReprogram = 0;
  else
  {
    if (timerWaitReprogram >= 0) 
    {
      if (++timerWaitReprogram > (2*TIMEOUT_IMPULSE_P_RELE_REPROGRAM_STAFF))
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
    unsigned int * const p_logic_df = (logic_df + i);
    *p_logic_df = 0;
    unsigned int const shift = 3*i;
    
    unsigned int const rang_df_in = RANG_DF1_IN + shift;
    unsigned int const number_byte_in = rang_df_in >> 5;
    unsigned int const number_bit_in = rang_df_in & 0x1f;

    /***
    Джерело активації ОФ-ії
    ***/
    *p_logic_df |= ((p_active_functions[number_byte_in] & (1 << number_bit_in) ) >> number_bit_in ) << 0;
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
      if (global_timers[INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START + i] >= ((int)current_settings_prt.timeout_pause_df[i]))
        global_timers[INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START + i] = -1;
    }
    /***/

    int flag=0;
    for(int m=0; m<N_BIG; m++) if( ( current_settings_prt.ranguvannja_df_source_plus[N_BIG*i    +m] & p_active_functions[m] ) != 0) {flag=1; break;}
    if(flag)
    {
      *p_logic_df |= (1 << 0);
    }

    flag=0;
    for(int m=0; m<N_BIG; m++) if( ( current_settings_prt.ranguvannja_df_source_minus[N_BIG*i    +m] & ((unsigned int)(~p_active_functions[m])) ) != 0 ) {flag=1;break;}
    if(flag)
    {
      *p_logic_df |= (1 << 0);
    }

    flag=1;
    for(int m=0; m<N_BIG; m++) if( ( current_settings_prt.ranguvannja_df_source_blk[N_BIG*i    +m] & p_active_functions[m] ) != 0 ) {flag=0; break;}
    if(flag)
    {
      *p_logic_df |= (1 << 1);
    }
    
    /***
    Виконуємо у цьому місці обробку логіки без встанвлення/скидання бітів про стан Визначуваної функції
    Щоб новий стан попередньої функції не був включений у стан наступної
    ***/
    *p_logic_df |= (_CHECK_SET_BIT(p_active_functions, (RANG_DF1_R + shift)) != 0) << 5;
    flag = 0;
    for(size_t m = 0; m < N_BIG; ++m) if( ( current_settings_prt.ranguvannja_df_source_reset[N_BIG*i + m] & p_active_functions[m] ) != 0) {flag = 1; break;}
    if(flag)
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
      _TIMER_0_T_R    (INDEX_TIMER_DF_WORK_START + i, current_settings_prt.timeout_work_df[i], (*p_logic_df), 5,                     (*p_logic_df), 2, (*p_logic_df), 3);
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
    unsigned int const index_df = RANG_DF1_OUT + 3*i;
      
    if ((logic_df[i] & (1u << 4)) != 0 ) _SET_BIT(p_active_functions, index_df);
    else _CLEAR_BIT(p_active_functions, index_df);
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
  for (size_t i = 0; i < NUMBER_DEFINED_TRIGGERS/*current_settings_prt.number_defined_dt*/; ++i)
  {
    unsigned int const shift = 3*i;

    unsigned int const rang_df_set   = RANG_DT1_SET + shift;
    unsigned int const number_byte_set = rang_df_set >> 5;
    unsigned int const number_bit_set  = rang_df_set & 0x1f;

    unsigned int const rang_df_reset = RANG_DT1_RESET + shift;
    unsigned int const number_byte_reset = rang_df_reset >> 5;
    unsigned int const number_bit_reset  = rang_df_reset & 0x1f;
    
    unsigned int const index_dt = RANG_DT1_OUT + shift;

    state_defined_triggers |= ( (p_active_functions[index_dt >> 5] & ( 1 << (index_dt & 0x1f) ) ) >> (index_dt & 0x1f) ) << i;
    /***
    Джерела встановлення і скидання ОТ
    ***/
    source_set_dt   |= ((p_active_functions[number_byte_set  ] & (1 << number_bit_set  ) ) >> number_bit_set   ) << i;
    source_reset_dt |= ((p_active_functions[number_byte_reset] & (1 << number_bit_reset) ) >> number_bit_reset ) << i;
    /***/
  }
  
  //Визначаємо, чи встановлюються/скидаються опреділювані триґери через свої ранжовані функції-джерела
  for (size_t i = 0; i < NUMBER_DEFINED_TRIGGERS/*current_settings_prt.number_defined_dt*/; ++i)
  {
    int flag=0;
    for(int m=0; m<N_BIG; m++) if(current_settings_prt.ranguvannja_set_dt_source_plus[N_BIG*i    +m] !=0) {flag=1; break;}
    if (flag)
    {
      //Випадок, якщо функції зранжовані на джерело прямих функцій
      flag=0;
      for(int m=0; m<N_BIG; m++) if(( current_settings_prt.ranguvannja_set_dt_source_plus[N_BIG*i    +m] & p_active_functions[m]) != 0) {flag=1; break;}
      if(flag)
      {
        source_set_dt |= (1 << i);
      }
    }

    flag=0;
    for(int m=0; m<N_BIG; m++) if(current_settings_prt.ranguvannja_set_dt_source_minus[N_BIG*i    +m] !=0) {flag=1; break;}
    if (flag)
    {
      //Випадок, якщо функції зранжовані на джерело інверсних функцій
      flag=0;
      for(int m=0; m<N_BIG; m++) if( ( current_settings_prt.ranguvannja_set_dt_source_minus[N_BIG*i    +m] & ((unsigned int)(~p_active_functions[m])) ) != 0 ) {flag=1; break;}
      if(flag)
      {
        source_set_dt |= (1<< i);
      }
    }

    flag=0;
    for(int m=0; m<N_BIG; m++) if(current_settings_prt.ranguvannja_reset_dt_source_plus[N_BIG*i    +m] !=0) {flag=1; break;}
    if (flag)
    {
      //Випадок, якщо функції зранжовані на джерело прямих функцій
      flag=0;
      for(int m=0; m<N_BIG; m++) if(( current_settings_prt.ranguvannja_reset_dt_source_plus[N_BIG*i    +m] & p_active_functions[m]) != 0) {flag=1; break;}
      if(flag)
      {
        source_reset_dt |= (1 << i);
      }
    }

    flag=0;
    for(int m=0; m<N_BIG; m++) if(current_settings_prt.ranguvannja_reset_dt_source_minus[N_BIG*i    +m] !=0) {flag=1; break;}
    if (flag)
    {
      //Випадок, якщо функції зранжовані на джерело інверсних функцій
      flag=0;
      for(int m=0; m<N_BIG; m++) if( ( current_settings_prt.ranguvannja_reset_dt_source_minus[N_BIG*i    +m] & ((unsigned int)(~p_active_functions[m])) ) != 0 ) {flag=1; break;}
      if(flag)
      {
        source_reset_dt |= (1<< i);
      }
    }

    //Запускаємо у роботу лоргічну схему роботи опреділюваного триґера (встановлення має пріоритет)
    if ((source_set_dt   & (1 << i)) != 0) state_defined_triggers |= (1 << i);
    if ((source_reset_dt & (1 << i)) != 0) state_defined_triggers &= (unsigned int)(~(1 << i));
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
    unsigned int index_dt =  RANG_DT1_OUT + 3*i;
      
    if ((state_defined_triggers & (1 << i)) != 0 ) _SET_BIT(p_active_functions, index_dt);
    else _CLEAR_BIT(p_active_functions, index_dt);
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
  for (unsigned int i = 0; i < NUMBER_DEFINED_AND/*current_settings_prt.number_defined_and*/; i++)
  {
    int flag=1;
    for(int m=0; m<N_BIG; m++) if((current_settings_prt.ranguvannja_d_and[N_BIG*i + m] & p_active_functions[m]) != current_settings_prt.ranguvannja_d_and[N_BIG*i + m]) {flag=0; break;}
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
      
    if ((state_defined_and & (1 << i)) != 0 ) _SET_BIT(p_active_functions, index_d_and);
    else _CLEAR_BIT(p_active_functions, index_d_and);
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
  for (unsigned int i = 0; i < NUMBER_DEFINED_OR/*current_settings_prt.number_defined_or*/; i++)
  {
    int flag=0;
    for(int m=0; m<N_BIG; m++) if((current_settings_prt.ranguvannja_d_or[N_BIG*i + m] & p_active_functions[m]) != 0) {flag=1; break;}
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
      
    if ((state_defined_or & (1 << i)) != 0 ) _SET_BIT(p_active_functions, index_d_or);
    else _CLEAR_BIT(p_active_functions, index_d_or);
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
  for (unsigned int i = 0; i < NUMBER_DEFINED_XOR/*current_settings_prt.number_defined_xor*/; i++)
  {
    unsigned int temp_array[N_BIG];
    for(int m=0; m<N_BIG; m++) temp_array[m] = current_settings_prt.ranguvannja_d_xor[N_BIG*i + m] & p_active_functions[m];
    
    int flag=0;
    for(int m=0; m<N_BIG; m++) if(temp_array[m] != 0) {flag=1; break;}
    if (flag)
    {
      unsigned int signals = 0;
      for (unsigned int j = 0; j < N_BIG; j++)
      {
        if (temp_array[j] == 0) continue; 
        else
        {
          for (unsigned int k = 0; k < 32; k++)
          {
            if ((temp_array[j] & (1 << k)) != 0) 
            {
              signals++;
              if (signals > 1) break;
            }
          }
          if (signals > 1) break;
        }
      }
      if (signals == 1) state_defined_xor |= (1 << i);
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
      
    if ((state_defined_xor & (1 << i)) != 0 ) _SET_BIT(p_active_functions, index_d_xor);
    else _CLEAR_BIT(p_active_functions, index_d_xor);
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
  for (unsigned int i = 0; i < NUMBER_DEFINED_NOT/*current_settings_prt.number_defined_not*/; i++)
  {
    int flag=1;
    for(int m=0; m<N_BIG; m++) if((current_settings_prt.ranguvannja_d_not[N_BIG*i + m] & p_active_functions[m]) != 0) {flag=0; break;}
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
      
    if ((state_defined_not & (1 << i)) != 0 ) _SET_BIT(p_active_functions, index_d_not);
    else _CLEAR_BIT(p_active_functions, index_d_not);
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
									(small_big_rang_tmp < (RANG_1_GRUPA_USTAVOK + NUMBER_GROUP_USTAVOK))
								 )
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
//Вираховування часової витримки  для залежної МСЗ2 (універсальна формула)
/*
-------------------------------------------------
Т(c) = K*Tу/( ((I/Iу)^alpha) - 1 )
-------------------------------------------------
*/
/*****************************************************/
//#pragma optimize=none
inline int timeout_dependent_general(const unsigned int i, const uint32_t Iust, const int32_t time_out, const int32_t max_time_out, const int32_t type_mtz)
{
  int timeout_result = 0;
  
  if (
      (type_mtz >= TYPE_MTZ_DEPENDENT_A) &&
      (type_mtz <= TYPE_MTZ_DEPENDENT_RTV_I)
     )   
  {
    if (i > Iust)
    {
      register float timeout = (((float)time_out)/1000.0f);
      register float timeout_dependent = 0;
      
      if (
          (type_mtz >= TYPE_MTZ_DEPENDENT_A) &&
          (type_mtz <= TYPE_MTZ_DEPENDENT_C)
         )   
      {
        register float K = 0.0f;
        register float alpha = 0.0f;

        switch (type_mtz)
        {
        case TYPE_MTZ_DEPENDENT_A:
          {
            K = 0.14f;
            alpha = 0.02f;
            break;
          }
        case TYPE_MTZ_DEPENDENT_B:
          {
            K = 13.5f;
            alpha = 1.0f;
            break;
          }
        case TYPE_MTZ_DEPENDENT_C:
          {
            K = 80.0f;
            alpha = 2.0f;
            break;
          }
        default:
          {
            //Теоретично цього ніколи не мало б бути
            total_error_sw_fixed();
          }
        }
    
        register float I_div_Iusy = ((float)i)/((float)Iust);
        register float rising_to_power = powf(I_div_Iusy, alpha);
        if (
            (isnan(rising_to_power) != 0) || 
            (isinf(rising_to_power) != 0) ||
            (rising_to_power <= 1)
          )
        {
          //Теоретично цього ніколи не мало б бути
          total_error_sw_fixed();
        }
  
        timeout_dependent = K*timeout/(rising_to_power - 1);
      }
      else if (
               (type_mtz >= TYPE_MTZ_DEPENDENT_RT_80) &&
               (type_mtz <= TYPE_MTZ_DEPENDENT_RTV_I)
              )   
      {
        register float I_div_Iusy = ((float)i)/((float)Iust);
        I_div_Iusy = I_div_Iusy - 1.0f;

        register float K = 0.0f;
        register float alpha = 0.0f;

        switch (type_mtz)
        {
        case TYPE_MTZ_DEPENDENT_RT_80:
          {
            K = 20.0f;
            alpha = 1.8f;
          
            I_div_Iusy /= 6.0f;
            break;
          }
        case TYPE_MTZ_DEPENDENT_RTV_I:
          {
            K = 30.0f;
            alpha = 3.0f;
            break;
          } 
        default:
          {
            //Теоретично цього ніколи не мало б бути
            total_error_sw_fixed();
          }
        }
      
        register float rising_to_power = powf(I_div_Iusy, alpha);
        if (
            (isnan(rising_to_power) != 0) || 
            (isinf(rising_to_power) != 0) ||
            (rising_to_power <= 0)
           )
        {
          //Теоретично цього ніколи не мало б бути
          total_error_sw_fixed();
        }
      
        timeout_dependent = 1.0f/(K*rising_to_power)+ timeout;
      }

      int timeout_dependent_int = (int)timeout_dependent;
      if (timeout_dependent_int > (max_time_out/1000)) timeout_dependent = (max_time_out/1000);
      else if (timeout_dependent_int < 0) timeout_dependent = 0;
    
      timeout_result = (int)(timeout_dependent*1000.0f);
    }
    else timeout_result = max_time_out;
  }
  
  return timeout_result;
}
/*****************************************************/

/*****************************************************/
// ЗНам
/*****************************************************/
inline void znam_handler(unsigned int *p_active_functions, unsigned int number_group_stp)
{
  static uint32_t const index_i1[NUMBER_I2G_CANALES] = {IM_IA,  IM_IB,  IM_IC };
  static uint32_t const index_i2[NUMBER_I2G_CANALES] = {IM2_IA, IM2_IB, IM2_IC};
  
  unsigned int po_znam = (_CHECK_SET_BIT(p_active_functions, RANG_PO_ZNAM) != 0);
  
  uint32_t logic_ZNAM = 0;
  for (size_t i = 0; i < NUMBER_I2G_CANALES; i++)
  {
    /*static*/ unsigned int internal_po[NUMBER_I2G_CANALES];
    
    uint32_t i1g = measurement[index_i1[i]];
    uint32_t i2g = measurement[index_i2[i]];

    unsigned int setpoint_i1g;
    if (i1g_bilshe_porogu[i] == 0) setpoint_i1g = MIN_LIMIT_FOR_I1G_AND_I2G*KOEF_POVERNENNJA_ZNAM_BLK/100;
    else setpoint_i1g = MIN_LIMIT_FOR_I1G_AND_I2G;
    unsigned int i1g_bilshe_porogu_tmp = i1g_bilshe_porogu[i] = (i1g >= setpoint_i1g);
    
    unsigned int setpoint_i2g;
    if (i2g_bilshe_porogu[i] == 0) setpoint_i2g = MIN_LIMIT_FOR_I1G_AND_I2G*KOEF_POVERNENNJA_ZNAM_BLK/100;
    else setpoint_i2g = MIN_LIMIT_FOR_I1G_AND_I2G;
    unsigned int i2g_bilshe_porogu_tmp = i2g_bilshe_porogu[i] = (i2g >= setpoint_i2g);

    unsigned int setpoint = (po_znam/*internal_po[i]*/ == 0) ? current_settings_prt.setpoint_znam[number_group_stp] : current_settings_prt.setpoint_znam[number_group_stp]*KOEF_POVERNENNJA_GENERAL_UP/100;

    /*
    I2
    -- >= K
    I1
    оскільки величина устаки у нас є перемножена на 1000, ,бо 0,010 відповідає 10, а 1,000 відповідає 1000,
    то K*1000 = SETPOINT =>
        SETPOINT
    K = --------  =>
         1000
    I2    SETPOINT
    -- >= ---------
    I1     1000
    
    струми зворотньої і прямої послідовності не треба ні нащо перемножувати, бо вони є у однакових величинах (міліампери), а нас цікавить їх відношення
    
    (I2*1000) >= (SETPOINT*I1)
    */
    internal_po[i] = ((i1g_bilshe_porogu_tmp != 0) && (i2g_bilshe_porogu_tmp != 0) && ((i2g*1000) >= (i1g*setpoint)));
    logic_ZNAM |= (internal_po[i] != 0) << 0;
  }
  
  
  logic_ZNAM |= (
                 ((current_settings_prt.control_znam & MASKA_FOR_BIT(CTR_ZNAM_STATE_BIT)) != 0) &&
                 (_CHECK_SET_BIT(p_active_functions, RANG_BLOCK_ZNAM) == 0) &&
                 _GET_STATE(logic_ZNAM, 0)
                ) << 1;
  _TIMER_0_T(INDEX_TIMER_ZNAM, current_settings_prt.timeout_znam[number_group_stp], logic_ZNAM, 1, logic_ZNAM, 2);
  
  if ( _GET_STATE(logic_ZNAM, 1)) _SET_BIT(p_active_functions, RANG_PO_ZNAM);
  else _CLEAR_BIT(p_active_functions, RANG_PO_ZNAM);

  if ( _GET_STATE(logic_ZNAM, 2)) _SET_BIT(p_active_functions, RANG_ZNAM);
  else _CLEAR_BIT(p_active_functions, RANG_ZNAM);
}
/*****************************************************/

/*****************************************************/
// МТЗ
/*****************************************************/
inline void ocp_handler(unsigned int *p_active_functions, unsigned int number_group_stp)
{
	unsigned int const Ia = measurement[IM_IA];
	unsigned int const Ib = measurement[IM_IB];
	unsigned int const Ic = measurement[IM_IC];

	unsigned int const Uab = measurement[IM_UAB];
	unsigned int const Ubc = measurement[IM_UBC];
	unsigned int const Uca = measurement[IM_UCA];
	
	unsigned int Imax = Ia;
	if (Imax < Ib) Imax = Ib;
	if (Imax < Ic) Imax = Ic;
	
  /****** Несправність кіл напруги *******/
#define I_NOM_CONST (I_NOM * KOEF_1_2_I)

  //ПО Iнцн
  unsigned int const po_i_ncn_setpoint = (unsigned int)(_GET_STATE(ocp_general_bits, 0) ? (I_NOM_CONST * KOEF_POVERNENNJA_MTZ_I_DOWN / 100) : I_NOM_CONST);
  if ((Ia <= po_i_ncn_setpoint) && (Ib <= po_i_ncn_setpoint) && (Ic <= po_i_ncn_setpoint)) _SET_STATE(ocp_general_bits, 0);
	else _CLEAR_STATE(ocp_general_bits, 0);
	
#undef I_NOM_CONST
  
  //ПО Uнцн
  if ((current_settings_prt.control_extra_settings_1 & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE)) == 0)
  {
#define U_F_NOM_CONST (U_F_NOM * KOEF_0_2_U)

		unsigned int const po_u_ncn_setpoint = (unsigned int)(_GET_STATE(ocp_general_bits, 1) ? (U_F_NOM_CONST * U_DOWN  / 100) : U_F_NOM_CONST);
    if ((measurement[IM_UA] <= po_u_ncn_setpoint) || (measurement[IM_UB] <= po_u_ncn_setpoint) || (measurement[IM_UC] <= po_u_ncn_setpoint))  _SET_STATE(ocp_general_bits, 1);
		else _CLEAR_STATE(ocp_general_bits, 1);
		
#undef U_F_NOM_CONST
  }
  else
  {
#define U_LINER_NOM_CONST (U_LINEAR_NOM * KOEF_0_2_U)

    unsigned int const po_u_ncn_setpoint = (unsigned int)(_GET_STATE(ocp_general_bits, 1) ? (U_LINER_NOM_CONST * U_DOWN / 100) : U_LINER_NOM_CONST);
    if ((Uab <= po_u_ncn_setpoint) || (Ubc <= po_u_ncn_setpoint) || (Uca <= po_u_ncn_setpoint))  _SET_STATE(ocp_general_bits, 1);
		else _CLEAR_STATE(ocp_general_bits, 1);
		
#undef U_LINER_NOM_CONST
  }
  
  if (
			((current_settings_prt.control_mtz & MASKA_FOR_BIT(N_BIT_CTRMTZ_NESPR_KIL_NAPR)) != 0) &&
			((ocp_general_bits & ((1u << 0) | (1u << 1))) == ((1u << 0) | (1u << 1)))
		)
	{
		_SET_BIT(p_active_functions, RANG_NCN_MTZ);
	}
	else
	{
		_CLEAR_BIT(p_active_functions, RANG_NCN_MTZ);
	}
  /****** Несправність кіл напруги *******/
	
  /****** ПО U блок. МСЗС *******/
  unsigned int po_block_u_ocp_setpoint = (_CHECK_SET_BIT(p_active_functions, RANG_PO_BLOCK_U_MTZN) == 0) ? PORIG_CHUTLYVOSTI_VOLTAGE : PORIG_CHUTLYVOSTI_VOLTAGE * U_DOWN / 100;
  
  //ПО U блок. МСЗС
  if (
      (Uab <= po_block_u_ocp_setpoint) ||
      (Ubc <= po_block_u_ocp_setpoint) ||
      (Uca <= po_block_u_ocp_setpoint)
     )
  {
    _SET_BIT(p_active_functions, RANG_PO_BLOCK_U_MTZN);
  }
  else
  {
    _CLEAR_BIT(p_active_functions, RANG_PO_BLOCK_U_MTZN);
  }
  /****** ПО U блок. МСЗС *******/
	
	static int const ocp_signals[NUMBER_LEVEL_OCP][OCP_SIGNALS_LENGTH] = 
	{
  	{
    	RANG_BLOCK_MTZ1, 
    	-1,
    	RANG_SECTOR_VPERED_MTZN1,
    	RANG_SECTOR_NAZAD_MTZN1,
    	RANG_PO_MTZ1,
    	RANG_PO_MTZN1_VPERED,
    	RANG_PO_MTZN1_NAZAD,
    	RANG_PO_U_MTZPN1,
    	RANG_PO_MTZPN1,
    	RANG_MTZ1
  	},
  	{
    	RANG_BLOCK_MTZ2, 
    	RANG_BLOCK_USK_MTZ2,
    	RANG_SECTOR_VPERED_MTZN2,
    	RANG_SECTOR_NAZAD_MTZN2,
    	RANG_PO_MTZ2,
    	RANG_PO_MTZN2_VPERED,
    	RANG_PO_MTZN2_NAZAD,
    	RANG_PO_U_MTZPN2,
    	RANG_PO_MTZPN2,
    	RANG_MTZ2
  	},
  	{
    	RANG_BLOCK_MTZ3, 
    	-1,
    	RANG_SECTOR_VPERED_MTZN3,
    	RANG_SECTOR_NAZAD_MTZN3,
    	RANG_PO_MTZ3,
    	RANG_PO_MTZN3_VPERED,
    	RANG_PO_MTZN3_NAZAD,
    	RANG_PO_U_MTZPN3,
    	RANG_PO_MTZPN3,
    	RANG_MTZ3
  	},
  	{
    	RANG_BLOCK_MTZ4, 
    	-1,
    	RANG_SECTOR_VPERED_MTZN4,
    	RANG_SECTOR_NAZAD_MTZN4,
    	RANG_PO_MTZ4,
    	RANG_PO_MTZN4_VPERED,
    	RANG_PO_MTZN4_NAZAD,
    	RANG_PO_U_MTZPN4,
    	RANG_PO_MTZPN4,
    	RANG_MTZ4
  	}
	};
	static int const ocp_tmr_index[NUMBER_LEVEL_OCP][NUMBER_LEVEL_OCP_TMR_CONST] = 
	{
  	{
    	INDEX_TIMER_MTZ1,
    	INDEX_TIMER_MTZ1_N_VPERED,
    	INDEX_TIMER_MTZ1_N_NAZAD,
    	INDEX_TIMER_MTZ1_PO_NAPRUZI,
			-1,
			-1,
			-1,
			-1,
			-1,
			-1,
			-1,
			-1,
			-1,
			-1
  	},
  	{
    	INDEX_TIMER_MTZ2,
    	INDEX_TIMER_MTZ2_N_VPERED,
    	INDEX_TIMER_MTZ2_N_NAZAD,
    	INDEX_TIMER_MTZ2_PO_NAPRUZI,

			INDEX_TIMER_MTZ2_DEPENDENT_A,
  		INDEX_TIMER_MTZ2_DEPENDENT_B,
  		INDEX_TIMER_MTZ2_DEPENDENT_C,
  		INDEX_TIMER_MTZ2_DEPENDENT_RT_80,
  		INDEX_TIMER_MTZ2_DEPENDENT_RTV_I,
	
			INDEX_TIMER_MTZ2_PR,
			INDEX_TIMER_MTZ2_N_VPERED_PR,
			INDEX_TIMER_MTZ2_N_NAZAD_PR,
			INDEX_TIMER_MTZ2_PO_NAPRUZI_PR,
			INDEX_TIMER_MTZ2_VVID_PR,
  	},
  	{
    	INDEX_TIMER_MTZ3,
    	INDEX_TIMER_MTZ3_N_VPERED,
    	INDEX_TIMER_MTZ3_N_NAZAD,
    	INDEX_TIMER_MTZ3_PO_NAPRUZI,
			-1,
			-1,
			-1,
			-1,
			-1,
			-1,
			-1,
			-1,
			-1,
			-1
  	},
  	{
    	INDEX_TIMER_MTZ4,
    	INDEX_TIMER_MTZ4_N_VPERED,
    	INDEX_TIMER_MTZ4_N_NAZAD,
    	INDEX_TIMER_MTZ4_PO_NAPRUZI,
			-1,
			-1,
			-1,
			-1,
			-1,
			-1,
			-1,
			-1,
			-1,
			-1
  	},
	};

	static int const * const p_type_ocp[NUMBER_LEVEL_OCP] = 
	{
  	&current_settings_prt.type_mtz1,
  	&current_settings_prt.type_mtz2,
  	&current_settings_prt.type_mtz3,
  	&current_settings_prt.type_mtz4
	};
	static int const ocp_ctr_setting[NUMBER_LEVEL_OCP][MTZ_CONST_MENU_SETTINGS_LENGTH] = 
	{
  	{
    	MASKA_FOR_BIT(N_BIT_CTRMTZ_1),
    	MASKA_FOR_BIT(N_BIT_CTRMTZ_1_VPERED),
    	MASKA_FOR_BIT(N_BIT_CTRMTZ_1_NAZAD),
			-1,
			-1,
    	MASKA_FOR_BIT(N_BIT_CTRMTZ_1_ZNAM),
    	MASKA_FOR_BIT(N_BIT_CTRMTZ_1_ZNAM_TYPE)
  	},
  	{
    	MASKA_FOR_BIT(N_BIT_CTRMTZ_2),
    	MASKA_FOR_BIT(N_BIT_CTRMTZ_2_VPERED),
    	MASKA_FOR_BIT(N_BIT_CTRMTZ_2_NAZAD),
			MASKA_FOR_BIT(N_BIT_CTRMTZ_2_PRYSKORENNJA),
			MASKA_FOR_BIT(N_BIT_CTRMTZ_2_PRYSKORENA),
    	MASKA_FOR_BIT(N_BIT_CTRMTZ_2_ZNAM),
    	MASKA_FOR_BIT(N_BIT_CTRMTZ_2_ZNAM_TYPE)
  	},
  	{
    	MASKA_FOR_BIT(N_BIT_CTRMTZ_3),
    	MASKA_FOR_BIT(N_BIT_CTRMTZ_3_VPERED),
    	MASKA_FOR_BIT(N_BIT_CTRMTZ_3_NAZAD),
			-1,
			-1,
    	MASKA_FOR_BIT(N_BIT_CTRMTZ_3_ZNAM),
    	MASKA_FOR_BIT(N_BIT_CTRMTZ_3_ZNAM_TYPE)
  	},
  	{
    	MASKA_FOR_BIT(N_BIT_CTRMTZ_4),
    	MASKA_FOR_BIT(N_BIT_CTRMTZ_4_VPERED),
    	MASKA_FOR_BIT(N_BIT_CTRMTZ_4_NAZAD),
			-1,
			-1,
    	MASKA_FOR_BIT(N_BIT_CTRMTZ_4_ZNAM),
    	MASKA_FOR_BIT(N_BIT_CTRMTZ_4_ZNAM_TYPE)
  	}
	};
	
	static unsigned int const * const setpoint_ocp_k_znam[NUMBER_LEVEL_OCP] = 
	{
  	current_settings_prt.setpoint_mtz_1_k_znam,
  	current_settings_prt.setpoint_mtz_2_k_znam,
  	current_settings_prt.setpoint_mtz_3_k_znam,
  	current_settings_prt.setpoint_mtz_4_k_znam
	};
	static unsigned int const * const setpoint_ocp[NUMBER_LEVEL_OCP] = 
	{
  	current_settings_prt.setpoint_mtz_1,
  	current_settings_prt.setpoint_mtz_2,
  	current_settings_prt.setpoint_mtz_3,
  	current_settings_prt.setpoint_mtz_4
	};
	static unsigned int const * const setpoint_ocp_fw[NUMBER_LEVEL_OCP] = 
	{
  	current_settings_prt.setpoint_mtz_1_n_vpered,
  	current_settings_prt.setpoint_mtz_2_n_vpered,
  	current_settings_prt.setpoint_mtz_3_n_vpered,
  	current_settings_prt.setpoint_mtz_4_n_vpered
	};
	static unsigned int const * const setpoint_ocp_bw[NUMBER_LEVEL_OCP] = 
	{
  	current_settings_prt.setpoint_mtz_1_n_nazad,
  	current_settings_prt.setpoint_mtz_2_n_nazad,
  	current_settings_prt.setpoint_mtz_3_n_nazad,
  	current_settings_prt.setpoint_mtz_4_n_nazad
	};
	static unsigned int const * const setpoint_ocp_U_by_U[NUMBER_LEVEL_OCP] = 
	{
  	current_settings_prt.setpoint_mtz_1_U,
  	current_settings_prt.setpoint_mtz_2_U,
  	current_settings_prt.setpoint_mtz_3_U,
  	current_settings_prt.setpoint_mtz_4_U
	};
	static unsigned int const * const setpoint_ocp_I_by_U[NUMBER_LEVEL_OCP] = 
	{
  	current_settings_prt.setpoint_mtz_1_po_napruzi,
  	current_settings_prt.setpoint_mtz_2_po_napruzi,
  	current_settings_prt.setpoint_mtz_3_po_napruzi,
  	current_settings_prt.setpoint_mtz_4_po_napruzi
	};

	static unsigned int const * const timeout_ocp_k_znam[NUMBER_LEVEL_OCP] = 
	{
  	current_settings_prt.timeout_mtz_1_k_znam,
  	current_settings_prt.timeout_mtz_2_k_znam,
  	current_settings_prt.timeout_mtz_3_k_znam,
  	current_settings_prt.timeout_mtz_4_k_znam
	};
	static int const * const timeout_ocp[NUMBER_LEVEL_OCP] =
	{
  	current_settings_prt.timeout_mtz_1,
  	current_settings_prt.timeout_mtz_2,
  	current_settings_prt.timeout_mtz_3,
  	current_settings_prt.timeout_mtz_4
	};
	static int const * const timeout_ocp_fw[NUMBER_LEVEL_OCP] = 
	{
  	current_settings_prt.timeout_mtz_1_n_vpered,
  	current_settings_prt.timeout_mtz_2_n_vpered,
  	current_settings_prt.timeout_mtz_3_n_vpered,
  	current_settings_prt.timeout_mtz_4_n_vpered
	};
	static int const * const timeout_ocp_bw[NUMBER_LEVEL_OCP] = 
	{
  	current_settings_prt.timeout_mtz_1_n_nazad,
  	current_settings_prt.timeout_mtz_2_n_nazad,
  	current_settings_prt.timeout_mtz_3_n_nazad,
  	current_settings_prt.timeout_mtz_4_n_nazad
	};
	static int const * const timeout_ocp_by_U[NUMBER_LEVEL_OCP] = 
	{
  	current_settings_prt.timeout_mtz_1_po_napruzi,
  	current_settings_prt.timeout_mtz_2_po_napruzi,
  	current_settings_prt.timeout_mtz_3_po_napruzi,
  	current_settings_prt.timeout_mtz_4_po_napruzi
	};
	static int const * const timeout_ocp_vvid_pr[NUMBER_LEVEL_OCP] =
	{
  	NULL,
  	current_settings_prt.timeout_mtz_2_vvid_pr,
  	NULL,
  	NULL
	};
	static int const * const timeout_ocp_pr[NUMBER_LEVEL_OCP] =
	{
  	NULL,
  	current_settings_prt.timeout_mtz_2_pr,
  	NULL,
  	NULL
	};
	static int const * const timeout_ocp_pr_fw[NUMBER_LEVEL_OCP] = 
	{
  	NULL,
  	current_settings_prt.timeout_mtz_2_n_vpered_pr,
  	NULL,
  	NULL
	};
	static int const * const timeout_ocp_pr_bw[NUMBER_LEVEL_OCP] = 
	{
  	NULL,
  	current_settings_prt.timeout_mtz_2_n_nazad_pr,
  	NULL,
  	NULL
	};
	static int const * const timeout_ocp_pr_by_U[NUMBER_LEVEL_OCP] = 
	{
  	NULL,
  	current_settings_prt.timeout_mtz_2_po_napruzi_pr,
  	NULL,
  	NULL
	};
	

	enum _ext_par 
	{
		DEP = 0,
		ACC,
		
		NUM_EXT_PAR
	};
	
	static unsigned int const ocp_ext[NUMBER_LEVEL_OCP][NUM_EXT_PAR] = 
	{
		{0, 0},
		{1, 1},
		{0, 0},
		{0, 0}
	};
	
	if (_CHECK_SET_BIT(p_active_functions, RANG_STATE_VV) != 0) _SET_STATE(ocp_general_bits, (2 + NUMBER_LEVEL_OCP));
	else _CLEAR_STATE(ocp_general_bits, (2 + NUMBER_LEVEL_OCP));
	
	unsigned int const znam_state = (_CHECK_SET_BIT(p_active_functions, RANG_ZNAM) != 0);
	
	unsigned int const control_ocp = current_settings_prt.control_mtz;
	uint32_t static_ocp_dep_tmp_bits = 0; //цю змінну спочатку було об'явлено, як глобальну, але потім функціонал змінився і я її переніс сюди, а логіку її роботи не переробляв
	for (size_t i = 0; i != NUMBER_LEVEL_OCP; ++i)
	{
		uint32_t l_ocp_0 = 0;
		uint32_t dep_ocp_0 = 0;
		uint32_t acc_ocp_0 = 0;
		
		uint32_t l_ocp_znam_0 = 0;
		if ( ((control_ocp & ocp_ctr_setting[i][CTR_MTZ_ZNAM]) != 0) && (znam_state != 0) )
		{
			l_ocp_znam_0 = ((control_ocp & ocp_ctr_setting[i][CTR_MTZ_ZNAM_TYPE]) != 0) ? MASKA_FOR_BIT(1) : MASKA_FOR_BIT(0);
		}
		
				
		int const type_ocp = *(p_type_ocp[i]);
		if (type_ocp == TYPE_MTZ_SIMPLE          ) _SET_STATE(l_ocp_0, 0);
		else if (type_ocp == TYPE_MTZ_DIRECTIONAL) _SET_STATE(l_ocp_0, 1);
		else if (type_ocp == TYPE_MTZ_VOLTAGE    ) _SET_STATE(l_ocp_0, 2);
		else if (ocp_ext[i][DEP] != 0)
		{
			if (type_ocp == TYPE_MTZ_DEPENDENT_A         )  _SET_STATE(dep_ocp_0, 0);
			else if (type_ocp == TYPE_MTZ_DEPENDENT_B    )  _SET_STATE(dep_ocp_0, 1);
			else if (type_ocp == TYPE_MTZ_DEPENDENT_C    )  _SET_STATE(dep_ocp_0, 2);
			else if (type_ocp == TYPE_MTZ_DEPENDENT_RT_80)  _SET_STATE(dep_ocp_0, 3);
			else if (type_ocp == TYPE_MTZ_DEPENDENT_RTV_I)  _SET_STATE(dep_ocp_0, 4);
			
			_OR5(dep_ocp_0, 0, dep_ocp_0, 1, dep_ocp_0, 2, dep_ocp_0, 3, dep_ocp_0, 4, dep_ocp_0, 5);
		}
		_INVERTOR(dep_ocp_0, 5, dep_ocp_0, 6); //у випадку, якщо заоежної немає, то цей біт має розблокувати можливість працювати простої МСЗ у випадку наявного присорення
		
		//Проста МСЗ
		unsigned int const pickup_ocp_base = (_GET_STATE(l_ocp_znam_0, 0)) 
																					? setpoint_ocp[i][number_group_stp] * setpoint_ocp_k_znam[i][number_group_stp]/ 100
																					: setpoint_ocp[i][number_group_stp];
    unsigned int const pickup_ocp = (_CHECK_SET_BIT(p_active_functions, ocp_signals[i][RANG_PO_MTZ]) != 0) ?
            pickup_ocp_base * KOEF_POVERNENNJA_MTZ_I_UP / 100 :
            pickup_ocp_base;
	  l_ocp_0 |= (Imax >= pickup_ocp) << 3;
		l_ocp_0 |= ((control_ocp & ocp_ctr_setting[i][CTR_MTZ]) != 0) << 4;
		l_ocp_0 |= ( (_CHECK_SET_BIT(p_active_functions, ocp_signals[i][RANG_BLOCK_MTZ]) == 0) && (_GET_STATE(l_ocp_znam_0, 1) == 0) ) << 5;
		_AND2(l_ocp_0, 4, l_ocp_0, 5, l_ocp_0, 6);
		
		l_ocp_0 |= ((control_ocp & ocp_ctr_setting[i][CTR_MTZ_VPERED]) != 0) << 7;
		l_ocp_0 |= ((control_ocp & ocp_ctr_setting[i][CTR_MTZ_NAZAD]) != 0) << 8;

		//Спрямований МСЗ
		unsigned int dir_ocp = 0;
		unsigned int const * const sector_ocp = sector_directional_mtz[i];

		dir_ocp |= (sector_ocp[PHASE_A_INDEX] == MTZ_VPERED) << 0;
		dir_ocp |= (sector_ocp[PHASE_B_INDEX] == MTZ_VPERED) << 1;
		dir_ocp |= (sector_ocp[PHASE_C_INDEX] == MTZ_VPERED) << 2;

		_OR3(dir_ocp, 0, dir_ocp, 1, dir_ocp, 2, dir_ocp, 3);
    if (_GET_STATE(dir_ocp, 3))
      _SET_BIT(p_active_functions, ocp_signals[i][RANG_SECTOR_VPERED_MTZN]);
    else
      _CLEAR_BIT(p_active_functions, ocp_signals[i][RANG_SECTOR_VPERED_MTZN]);
		
		unsigned int const pickup_ocp_fw_base = (_GET_STATE(l_ocp_znam_0, 0))
																						? setpoint_ocp_fw[i][number_group_stp] * setpoint_ocp_k_znam[i][number_group_stp]/ 100
																						: setpoint_ocp_fw[i][number_group_stp];
    unsigned int const pickup_ocp_fw = (_CHECK_SET_BIT(p_active_functions, ocp_signals[i][RANG_PO_MTZN_VPERED]) != 0) ?
            pickup_ocp_fw_base * KOEF_POVERNENNJA_MTZ_I_UP / 100 :
            pickup_ocp_fw_base;
		l_ocp_0 |= (
								(_GET_STATE(dir_ocp, 0) && (Ia >= pickup_ocp_fw)) ||
								(_GET_STATE(dir_ocp, 1) && (Ib >= pickup_ocp_fw)) ||
								(_GET_STATE(dir_ocp, 2) && (Ic >= pickup_ocp_fw))
							 ) << 9;

		dir_ocp |= (sector_ocp[PHASE_A_INDEX] == MTZ_NAZAD) << 4;
		dir_ocp |= (sector_ocp[PHASE_B_INDEX] == MTZ_NAZAD) << 5;
		dir_ocp |= (sector_ocp[PHASE_C_INDEX] == MTZ_NAZAD) << 6;

		_OR3(dir_ocp, 4, dir_ocp, 5, dir_ocp, 6, dir_ocp, 7);
    if (_GET_STATE(dir_ocp, 7))
      _SET_BIT(p_active_functions, ocp_signals[i][RANG_SECTOR_NAZAD_MTZN]);
    else
      _CLEAR_BIT(p_active_functions, ocp_signals[i][RANG_SECTOR_NAZAD_MTZN]);

		unsigned int const pickup_ocp_bw_base = (_GET_STATE(l_ocp_znam_0, 0))
																						? setpoint_ocp_bw[i][number_group_stp] * setpoint_ocp_k_znam[i][number_group_stp]/ 100
																						: setpoint_ocp_bw[i][number_group_stp];
    unsigned int const pickup_ocp_bw = (_CHECK_SET_BIT(p_active_functions, ocp_signals[i][RANG_PO_MTZN_NAZAD]) != 0) ?
            pickup_ocp_bw_base * KOEF_POVERNENNJA_MTZ_I_UP / 100 :
            pickup_ocp_bw_base;
		l_ocp_0 |= (
								(_GET_STATE(dir_ocp, 4) && (Ia >= pickup_ocp_bw)) ||
								(_GET_STATE(dir_ocp, 5) && (Ib >= pickup_ocp_bw)) ||
								(_GET_STATE(dir_ocp, 6) && (Ic >= pickup_ocp_bw))
							 ) << 10;
		
		//З пуском за напругою МСЗ
    unsigned int const pickup_ocp_U_by_U = (_CHECK_SET_BIT(p_active_functions, ocp_signals[i][RANG_PO_U_MTZPN]) != 0) ?
            setpoint_ocp_U_by_U[i][number_group_stp] * U_DOWN / 100:
            setpoint_ocp_U_by_U[i][number_group_stp] ;
	  l_ocp_0 |= ((Uab <= pickup_ocp_U_by_U) || (Ubc <= pickup_ocp_U_by_U) ||(Uca <= pickup_ocp_U_by_U)) << 11;
    if (_GET_STATE(l_ocp_0, 11))
      _SET_BIT(p_active_functions, ocp_signals[i][RANG_PO_U_MTZPN]);
    else
      _CLEAR_BIT(p_active_functions, ocp_signals[i][RANG_PO_U_MTZPN]);

		unsigned int const pickup_ocp_I_by_U_base = (_GET_STATE(l_ocp_znam_0, 0))
																						     ? setpoint_ocp_I_by_U[i][number_group_stp] * setpoint_ocp_k_znam[i][number_group_stp]/ 100
																						     : setpoint_ocp_I_by_U[i][number_group_stp];
		unsigned int const pickup_ocp_I_by_U  = (_CHECK_SET_BIT(p_active_functions, ocp_signals[i][RANG_PO_MTZPN]) != 0) ?
            pickup_ocp_I_by_U_base * KOEF_POVERNENNJA_MTZ_I_UP / 100:
            pickup_ocp_I_by_U_base;
	  l_ocp_0 |= (Imax >= pickup_ocp_I_by_U) << 12;
		
		uint32_t ocp_global_var_copy;
		do
		{
			ocp_global_var_copy = ocp_general_bits; //копія на почтку ітераційного процесу
			
			l_ocp_0 |= (
									(_CHECK_SET_BIT(p_active_functions, RANG_PO_BLOCK_U_MTZN) == 0) &&
									(_GET_STATE(ocp_general_bits, (2 + i)) == 0)	
								 ) << 13;
			_INVERTOR(l_ocp_0, 13, l_ocp_0, 14);
			_AND2(l_ocp_0, 1, l_ocp_0, 14, l_ocp_0, 15);
			
			_AND5(l_ocp_0, 1, l_ocp_0, 6, l_ocp_0, 7, l_ocp_0,  9, l_ocp_0, 13, l_ocp_0, 16);
			_AND5(l_ocp_0, 1, l_ocp_0, 6, l_ocp_0, 8, l_ocp_0, 10, l_ocp_0, 13, l_ocp_0, 17);
			
			_INVERTOR(ocp_general_bits, (2 + i), l_ocp_0, 18);
			_AND2(l_ocp_0, 11, l_ocp_0, 18, l_ocp_0, 19);
			
			_AND4(l_ocp_0, 2, l_ocp_0, 6, l_ocp_0, 19, l_ocp_0, 12, l_ocp_0, 20);
			
			uint32_t tmp_select_bits;
			do
			{
				tmp_select_bits = static_ocp_select_tmp_bits & (1u << i);
				
				_AND3(static_ocp_select_tmp_bits, i, l_ocp_0, 3, l_ocp_0, 6, l_ocp_0, 21);
				_OR5(l_ocp_0, 15, l_ocp_0, 0, l_ocp_0, 21, dep_ocp_0, 5, ocp_general_bits, (2 + i), static_ocp_select_tmp_bits, i);
			}
			while((static_ocp_select_tmp_bits & (1u << i)) != tmp_select_bits);
			
			_INVERTOR(l_ocp_0, 21, l_ocp_0, 22);
			_AND2(l_ocp_0, 22, l_ocp_0, 16, l_ocp_0, 23);
			_AND2(l_ocp_0, 22, l_ocp_0, 17, l_ocp_0, 24);
			_AND2(l_ocp_0, 22, l_ocp_0, 20, l_ocp_0, 25);
			
			_OR3_INVERTOR(l_ocp_0, 23, l_ocp_0, 24, l_ocp_0, 25, l_ocp_0, 26);
			
			if(
				  _GET_STATE(l_ocp_0, 26) &&
					(_CHECK_SET_BIT(p_active_functions, RANG_NCN_MTZ) != 0)	 
				)
			{
				_SET_STATE(ocp_general_bits, (2 + i));
			}
			else
			{
				_CLEAR_STATE(ocp_general_bits, (2 + i));
			}
		}
		while (ocp_global_var_copy != ocp_general_bits);

		//ПО МСЗх
		if (_GET_STATE(l_ocp_0, 21))
      _SET_BIT(p_active_functions, ocp_signals[i][RANG_PO_MTZ]);
    else
      _CLEAR_BIT(p_active_functions, ocp_signals[i][RANG_PO_MTZ]);
		
		//ПО МСЗCх Прямий
		if (_GET_STATE(l_ocp_0, 23))
      _SET_BIT(p_active_functions, ocp_signals[i][RANG_PO_MTZN_VPERED]);
    else
      _CLEAR_BIT(p_active_functions, ocp_signals[i][RANG_PO_MTZN_VPERED]);
		
		//ПО МСЗCх Звороний
		if (_GET_STATE(l_ocp_0, 24))
      _SET_BIT(p_active_functions, ocp_signals[i][RANG_PO_MTZN_NAZAD]);
    else
      _CLEAR_BIT(p_active_functions, ocp_signals[i][RANG_PO_MTZN_NAZAD]);
		
		//ПО МСЗПНх
		if (_GET_STATE(l_ocp_0, 25))
      _SET_BIT(p_active_functions, ocp_signals[i][RANG_PO_MTZPN]);
    else
      _CLEAR_BIT(p_active_functions, ocp_signals[i][RANG_PO_MTZPN]);
		
		int const timeout_ocp_base = (_GET_STATE(l_ocp_znam_0, 0))
																	? (unsigned int)timeout_ocp[i][number_group_stp] * timeout_ocp_k_znam[i][number_group_stp]/ 100
																	: timeout_ocp[i][number_group_stp];
		int const timeout_ocp_fw_base = (_GET_STATE(l_ocp_znam_0, 0))
																	   ? (unsigned int)timeout_ocp_fw[i][number_group_stp] * timeout_ocp_k_znam[i][number_group_stp]/ 100
																	   : timeout_ocp_fw[i][number_group_stp];
		int const timeout_ocp_bw_base = (_GET_STATE(l_ocp_znam_0, 0))
																	   ? (unsigned int)timeout_ocp_bw[i][number_group_stp] * timeout_ocp_k_znam[i][number_group_stp]/ 100
																	   : timeout_ocp_bw[i][number_group_stp];
		int const timeout_ocp_by_U_base = (_GET_STATE(l_ocp_znam_0, 0))
															  		   ? (unsigned int)timeout_ocp_by_U[i][number_group_stp] * timeout_ocp_k_znam[i][number_group_stp]/ 100
																  	   : timeout_ocp_by_U[i][number_group_stp];
		//Функціонал, який відповідає за прискорення
		if(ocp_ext[i][ACC] != 0)
		{
			acc_ocp_0 |= (_CHECK_SET_BIT(p_active_functions, ocp_signals[i][RANG_BLOCK_USK_MTZ]) == 0) << 0;
			
			acc_ocp_0 |= ((control_ocp & ocp_ctr_setting[i][CTR_MTZ_PRYSKORENA]) != 0) << 1;
			acc_ocp_0 |= ((control_ocp & ocp_ctr_setting[i][CTR_MTZ_PRYSKORENNJA]) != 0) << 2;

			_AND2(ocp_general_bits, (2 + NUMBER_LEVEL_OCP), acc_ocp_0, 2, acc_ocp_0, 3);

			_TIMER_IMPULSE(ocp_tmr_index[i][INDEX_TIMER_MTZ_VVID_PR], timeout_ocp_vvid_pr[i][number_group_stp], ocp_general_bits, (2 + NUMBER_LEVEL_OCP + 1 + i), acc_ocp_0, 3, acc_ocp_0, 4);
			_OR2(acc_ocp_0, 1, acc_ocp_0, 4, acc_ocp_0, 5);
			_AND2(acc_ocp_0, 0, acc_ocp_0, 5, acc_ocp_0, 6);
			_INVERTOR(acc_ocp_0, 6, acc_ocp_0, 7);

			//Прямий напрямок струмозалежної МСЗ
			uint32_t ocp_fw_tmp_bits;
			do
			{
				ocp_fw_tmp_bits = static_ocp_fw_tmp_bits & (/*(1u << (2*i)) |*/ (1u << (2*i + 1)));
				
				//_OR2(acc_ocp_0, 7, static_ocp_fw_tmp_bits, (2*i), acc_ocp_0, 8);
				//_INVERTOR(static_ocp_fw_tmp_bits, (2*i), acc_ocp_0, 9);
				
				_OR2(acc_ocp_0, 6, static_ocp_fw_tmp_bits, (2*i + 1), acc_ocp_0, 10);
				_INVERTOR(static_ocp_fw_tmp_bits, (2*i + 1), acc_ocp_0, 11);
				
				_AND3(acc_ocp_0, 7/*8*/, acc_ocp_0, 11, l_ocp_0, 23, static_ocp_fw_tmp_bits, (2*i    ));
				_AND2/*3*/(/*acc_ocp_0, 9, */acc_ocp_0, 10, l_ocp_0, 23, static_ocp_fw_tmp_bits, (2*i + 1));
			}
			while((static_ocp_fw_tmp_bits & (/*(1u << (2*i)) |*/ (1u << (2*i + 1)))) != ocp_fw_tmp_bits);
			_TIMER_T_0(ocp_tmr_index[i][INDEX_TIMER_MTZ_N_VPERED   ], timeout_ocp_fw_base                   , static_ocp_fw_tmp_bits, (2*i    ), l_ocp_0  , 28);
			_TIMER_T_0(ocp_tmr_index[i][INDEX_TIMER_MTZ_N_VPERED_PR], timeout_ocp_pr_fw[i][number_group_stp], static_ocp_fw_tmp_bits, (2*i + 1), acc_ocp_0, 12);

			//Зворотний напрямок струмозалежної МСЗ
			uint32_t ocp_bw_tmp_bits;
			do
			{
				ocp_bw_tmp_bits = static_ocp_bw_tmp_bits & (/*(1u << (2*i)) |*/ (1u << (2*i + 1)));
				
				//_OR2(acc_ocp_0, 7, static_ocp_bw_tmp_bits, (2*i), acc_ocp_0, 13);
				//_INVERTOR(static_ocp_bw_tmp_bits, (2*i), acc_ocp_0, 14);
				
				_OR2(acc_ocp_0, 6, static_ocp_bw_tmp_bits, (2*i + 1), acc_ocp_0, 15);
				_INVERTOR(static_ocp_bw_tmp_bits, (2*i + 1), acc_ocp_0, 16);
				
				_AND3(acc_ocp_0, 7/*13*/, acc_ocp_0, 16, l_ocp_0, 24, static_ocp_bw_tmp_bits, (2*i    ));
				_AND2/*3*/(/*acc_ocp_0, 14, */acc_ocp_0, 15, l_ocp_0, 24, static_ocp_bw_tmp_bits, (2*i + 1));
			}
			while((static_ocp_bw_tmp_bits & (/*(1u << (2*i)) |*/ (1u << (2*i + 1)))) != ocp_bw_tmp_bits);
			_TIMER_T_0(ocp_tmr_index[i][INDEX_TIMER_MTZ_N_NAZAD   ], timeout_ocp_bw_base                   , static_ocp_bw_tmp_bits, (2*i    ), l_ocp_0  , 29);
			_TIMER_T_0(ocp_tmr_index[i][INDEX_TIMER_MTZ_N_NAZAD_PR], timeout_ocp_pr_bw[i][number_group_stp], static_ocp_bw_tmp_bits, (2*i + 1), acc_ocp_0, 17);

			//МСЗ з пуском за напругою
			uint32_t ocp_by_U_tmp_bits;
			do
			{
				ocp_by_U_tmp_bits = static_ocp_by_U_tmp_bits & (/*(1u << (2*i)) |*/ (1u << (2*i + 1)));
				
				//_OR2(acc_ocp_0, 7, static_ocp_by_U_tmp_bits, (2*i), acc_ocp_0, 18);
				//_INVERTOR(static_ocp_by_U_tmp_bits, (2*i), acc_ocp_0, 19);
				
				_OR2(acc_ocp_0, 6, static_ocp_by_U_tmp_bits, (2*i + 1), acc_ocp_0, 20);
				_INVERTOR(static_ocp_by_U_tmp_bits, (2*i + 1), acc_ocp_0, 21);
				
				_AND3(acc_ocp_0, 7/*18*/, acc_ocp_0, 21, l_ocp_0, 25, static_ocp_by_U_tmp_bits, (2*i    ));
				_AND2/*3*/(/*acc_ocp_0, 19, */acc_ocp_0, 20, l_ocp_0, 25, static_ocp_by_U_tmp_bits, (2*i + 1));
			}
			while((static_ocp_by_U_tmp_bits & (/*(1u << (2*i)) |*/ (1u << (2*i + 1)))) != ocp_by_U_tmp_bits);
			_TIMER_T_0(ocp_tmr_index[i][INDEX_TIMER_MTZ_PO_NAPRUZI   ], timeout_ocp_by_U_base                   , static_ocp_by_U_tmp_bits, (2*i    ), l_ocp_0  , 30);
			_TIMER_T_0(ocp_tmr_index[i][INDEX_TIMER_MTZ_PO_NAPRUZI_PR], timeout_ocp_pr_by_U[i][number_group_stp], static_ocp_by_U_tmp_bits, (2*i + 1), acc_ocp_0, 22);

			//МСЗ простий або струмозалежний
			uint32_t ocp_tmp_bits;
//			uint32_t ocp_dep_tmp_bits;
			do
			{
				ocp_tmp_bits     = static_ocp_tmp_bits     & (/*(1u << (2*i)) |*/ (1u << (2*i + 1)));
//				ocp_dep_tmp_bits = static_ocp_dep_tmp_bits & ((1u << (NUMBER_LEVEL_OCP*(TYPE_MTZ_DEPENDENT_RTV_I - TYPE_MTZ_DEPENDENT_A + 1)) - 1));
				
				//_OR2(acc_ocp_0, 7, static_ocp_tmp_bits, (2*i), acc_ocp_0, 23);
				/*
				_OR6_INVERTOR(
											static_ocp_tmp_bits, (2*i), 
											static_ocp_dep_tmp_bits, (NUMBER_LEVEL_OCP*(TYPE_MTZ_DEPENDENT_A     - TYPE_MTZ_DEPENDENT_A) + i),
											static_ocp_dep_tmp_bits, (NUMBER_LEVEL_OCP*(TYPE_MTZ_DEPENDENT_B     - TYPE_MTZ_DEPENDENT_A) + i),
											static_ocp_dep_tmp_bits, (NUMBER_LEVEL_OCP*(TYPE_MTZ_DEPENDENT_C     - TYPE_MTZ_DEPENDENT_A) + i),
											static_ocp_dep_tmp_bits, (NUMBER_LEVEL_OCP*(TYPE_MTZ_DEPENDENT_RT_80 - TYPE_MTZ_DEPENDENT_A) + i),
											static_ocp_dep_tmp_bits, (NUMBER_LEVEL_OCP*(TYPE_MTZ_DEPENDENT_RTV_I - TYPE_MTZ_DEPENDENT_A) + i),
											acc_ocp_0, 24
										 );
				*/
				
				_OR2(acc_ocp_0, 6, static_ocp_tmp_bits, (2*i + 1), acc_ocp_0, 25);
				_INVERTOR(static_ocp_tmp_bits, (2*i + 1), acc_ocp_0, 26);
				
				_AND4(dep_ocp_0, 6, acc_ocp_0, 7/*23*/, acc_ocp_0, 26, l_ocp_0, 21, static_ocp_tmp_bits, (2*i    ));
				_AND2/*3*/(/*acc_ocp_0, 24, */acc_ocp_0, 25, l_ocp_0, 21, static_ocp_tmp_bits, (2*i + 1));
				
				if(ocp_ext[i][DEP] != 0)
				{
					/*
					_OR2(acc_ocp_0, 7, static_ocp_dep_tmp_bits, (NUMBER_LEVEL_OCP*(TYPE_MTZ_DEPENDENT_A     - TYPE_MTZ_DEPENDENT_A) + i), dep_ocp_0, 12);
					_OR2(acc_ocp_0, 7, static_ocp_dep_tmp_bits, (NUMBER_LEVEL_OCP*(TYPE_MTZ_DEPENDENT_B     - TYPE_MTZ_DEPENDENT_A) + i), dep_ocp_0, 13);
					_OR2(acc_ocp_0, 7, static_ocp_dep_tmp_bits, (NUMBER_LEVEL_OCP*(TYPE_MTZ_DEPENDENT_C     - TYPE_MTZ_DEPENDENT_A) + i), dep_ocp_0, 14);
					_OR2(acc_ocp_0, 7, static_ocp_dep_tmp_bits, (NUMBER_LEVEL_OCP*(TYPE_MTZ_DEPENDENT_RT_80 - TYPE_MTZ_DEPENDENT_A) + i), dep_ocp_0, 15);
					_OR2(acc_ocp_0, 7, static_ocp_dep_tmp_bits, (NUMBER_LEVEL_OCP*(TYPE_MTZ_DEPENDENT_RTV_I - TYPE_MTZ_DEPENDENT_A) + i), dep_ocp_0, 16);
					*/

					_AND4(acc_ocp_0, 7,/* dep_ocp_0, 12, */acc_ocp_0, 26, dep_ocp_0, 0, l_ocp_0, 21, static_ocp_dep_tmp_bits, (NUMBER_LEVEL_OCP*(TYPE_MTZ_DEPENDENT_A     - TYPE_MTZ_DEPENDENT_A) + i));
					_AND4(acc_ocp_0, 7,/* dep_ocp_0, 13, */acc_ocp_0, 26, dep_ocp_0, 1, l_ocp_0, 21, static_ocp_dep_tmp_bits, (NUMBER_LEVEL_OCP*(TYPE_MTZ_DEPENDENT_B     - TYPE_MTZ_DEPENDENT_A) + i));
					_AND4(acc_ocp_0, 7,/* dep_ocp_0, 14, */acc_ocp_0, 26, dep_ocp_0, 2, l_ocp_0, 21, static_ocp_dep_tmp_bits, (NUMBER_LEVEL_OCP*(TYPE_MTZ_DEPENDENT_C     - TYPE_MTZ_DEPENDENT_A) + i));
					_AND4(acc_ocp_0, 7,/* dep_ocp_0, 15, */acc_ocp_0, 26, dep_ocp_0, 3, l_ocp_0, 21, static_ocp_dep_tmp_bits, (NUMBER_LEVEL_OCP*(TYPE_MTZ_DEPENDENT_RT_80 - TYPE_MTZ_DEPENDENT_A) + i));
					_AND4(acc_ocp_0, 7,/* dep_ocp_0, 16, */acc_ocp_0, 26, dep_ocp_0, 4, l_ocp_0, 21, static_ocp_dep_tmp_bits, (NUMBER_LEVEL_OCP*(TYPE_MTZ_DEPENDENT_RTV_I - TYPE_MTZ_DEPENDENT_A) + i));
				}
			}
			while(
						((static_ocp_tmp_bits     & (/*(1u << (2*i)) | */(1u << (2*i + 1)))                                                   ) != ocp_tmp_bits    )/* ||
						((static_ocp_dep_tmp_bits & ((1u << (NUMBER_LEVEL_OCP*(TYPE_MTZ_DEPENDENT_RTV_I - TYPE_MTZ_DEPENDENT_A + 1)) - 1))) != ocp_dep_tmp_bits)*/
					 );
			_TIMER_T_0(ocp_tmr_index[i][INDEX_TIMER_MTZ   ], timeout_ocp_base                   , static_ocp_tmp_bits, (2*i    ), l_ocp_0  , 27);
			_TIMER_T_0(ocp_tmr_index[i][INDEX_TIMER_MTZ_PR], timeout_ocp_pr[i][number_group_stp], static_ocp_tmp_bits, (2*i + 1), acc_ocp_0, 27);
		}
		else
		{
			if(ocp_ext[i][DEP] != 0)
			{
				_AND2(dep_ocp_0, 0, l_ocp_0, 21, static_ocp_dep_tmp_bits, (NUMBER_LEVEL_OCP*(TYPE_MTZ_DEPENDENT_A     - TYPE_MTZ_DEPENDENT_A) + i));
				_AND2(dep_ocp_0, 1, l_ocp_0, 21, static_ocp_dep_tmp_bits, (NUMBER_LEVEL_OCP*(TYPE_MTZ_DEPENDENT_B     - TYPE_MTZ_DEPENDENT_A) + i));
				_AND2(dep_ocp_0, 2, l_ocp_0, 21, static_ocp_dep_tmp_bits, (NUMBER_LEVEL_OCP*(TYPE_MTZ_DEPENDENT_C     - TYPE_MTZ_DEPENDENT_A) + i));
				_AND2(dep_ocp_0, 3, l_ocp_0, 21, static_ocp_dep_tmp_bits, (NUMBER_LEVEL_OCP*(TYPE_MTZ_DEPENDENT_RT_80 - TYPE_MTZ_DEPENDENT_A) + i));
				_AND2(dep_ocp_0, 4, l_ocp_0, 21, static_ocp_dep_tmp_bits, (NUMBER_LEVEL_OCP*(TYPE_MTZ_DEPENDENT_RTV_I - TYPE_MTZ_DEPENDENT_A) + i));
			}
			
			_TIMER_T_0(ocp_tmr_index[i][INDEX_TIMER_MTZ_N_VPERED  ], timeout_ocp_fw_base  , l_ocp_0, 23, l_ocp_0, 28);
			_TIMER_T_0(ocp_tmr_index[i][INDEX_TIMER_MTZ_N_NAZAD   ], timeout_ocp_bw_base  , l_ocp_0, 24, l_ocp_0, 29);
			_TIMER_T_0(ocp_tmr_index[i][INDEX_TIMER_MTZ_PO_NAPRUZI], timeout_ocp_by_U_base, l_ocp_0, 25, l_ocp_0, 30);
			_TIMER_T_0(ocp_tmr_index[i][INDEX_TIMER_MTZ]           , timeout_ocp_base     , l_ocp_0, 21, l_ocp_0, 27);
		}
		
		if(ocp_ext[i][DEP] != 0)
		{
      static const int32_t max_time_out_array[NUMBER_LEVEL_OCP] = {TIMEOUT_MTZ1_MAX, TIMEOUT_MTZ2_MAX, TIMEOUT_MTZ3_MAX, TIMEOUT_MTZ4_MAX};
      int32_t const max_time_out = max_time_out_array[i];
			for(int type_ocp_tmp = TYPE_MTZ_DEPENDENT_A; type_ocp_tmp <= TYPE_MTZ_DEPENDENT_RTV_I; ++type_ocp_tmp)
			{
				int const shift = type_ocp_tmp - TYPE_MTZ_DEPENDENT_A;
				if (_GET_STATE(static_ocp_dep_tmp_bits, (NUMBER_LEVEL_OCP * shift + i)))
				{
					_TIMER_T_0_LOCK(ocp_tmr_index[i][INDEX_TIMER_MTZ_DEPENDENT_A  + shift], timeout_dependent_general(Imax, pickup_ocp_base, timeout_ocp_base, max_time_out, type_ocp_tmp), static_ocp_dep_tmp_bits, (NUMBER_LEVEL_OCP * shift + i), static_ocp_dep_rez_bits, (NUMBER_LEVEL_OCP * shift + i));
				}
				else
				{
					global_timers[ocp_tmr_index[i][INDEX_TIMER_MTZ_DEPENDENT_A  + shift]] = -1;
					_CLEAR_STATE(static_ocp_dep_rez_bits, (NUMBER_LEVEL_OCP * shift + i));
				}
			}
		}
		
		if (
				(
				 _GET_STATE(l_ocp_0, 27) ||
				 _GET_STATE(l_ocp_0, 28) ||
				 _GET_STATE(l_ocp_0, 29) ||
				 _GET_STATE(l_ocp_0, 30)
				)
				||
				(
				 (ocp_ext[i][DEP] != 0) &&
				 (
					_GET_STATE(static_ocp_dep_rez_bits, (NUMBER_LEVEL_OCP * (TYPE_MTZ_DEPENDENT_A     - TYPE_MTZ_DEPENDENT_A) + i)) ||
					_GET_STATE(static_ocp_dep_rez_bits, (NUMBER_LEVEL_OCP * (TYPE_MTZ_DEPENDENT_B     - TYPE_MTZ_DEPENDENT_A) + i)) ||
					_GET_STATE(static_ocp_dep_rez_bits, (NUMBER_LEVEL_OCP * (TYPE_MTZ_DEPENDENT_C     - TYPE_MTZ_DEPENDENT_A) + i)) ||
					_GET_STATE(static_ocp_dep_rez_bits, (NUMBER_LEVEL_OCP * (TYPE_MTZ_DEPENDENT_RT_80 - TYPE_MTZ_DEPENDENT_A) + i)) ||
					_GET_STATE(static_ocp_dep_rez_bits, (NUMBER_LEVEL_OCP * (TYPE_MTZ_DEPENDENT_RTV_I - TYPE_MTZ_DEPENDENT_A) + i))
				 )	
				) 
				||
				(
				 (ocp_ext[i][ACC] != 0) &&
				 (
  				_GET_STATE(acc_ocp_0, 12) ||
	  			_GET_STATE(acc_ocp_0, 17) ||
		  		_GET_STATE(acc_ocp_0, 22) ||
			  	_GET_STATE(acc_ocp_0, 27)
				 )	
				) 
			 )	
		{
      _SET_BIT(p_active_functions, ocp_signals[i][RANG_MTZ]);
		}
		else
		{
      _CLEAR_BIT(p_active_functions, ocp_signals[i][RANG_MTZ]);
		}
	}
}
/*****************************************************/

/*****************************************************/
// ЗДЗ
/*****************************************************/
inline void zdz_handler(unsigned int *p_active_functions, unsigned int number_group_stp)
{
  
  uint32_t control_zdz_tmp = current_settings_prt.control_zdz;

#if (                                   \
       (MODYFIKACIA_VERSII_PZ == 0) ||  \
       (MODYFIKACIA_VERSII_PZ == 3) ||  \
       (MODYFIKACIA_VERSII_PZ == 4) ||  \
       (MODYFIKACIA_VERSII_PZ == 6) ||  \
       (MODYFIKACIA_VERSII_PZ == 7) ||  \
       (MODYFIKACIA_VERSII_PZ == 10)||  \
       (MODYFIKACIA_VERSII_PZ == 13)||  \
       (MODYFIKACIA_VERSII_PZ == 14)||  \
       (MODYFIKACIA_VERSII_PZ == 17)    \
      )   

  static uint32_t swiched_on_OVD;
  static uint32_t new_OVD;

  uint32_t swiched_on_OVD_tmp = 
    (
     control_zdz_tmp & 
       (
        (1u << CTR_ZDZ_OVD1_STATE_BIT) 
        | 
        (1u << CTR_ZDZ_OVD2_STATE_BIT) 
        | 
        (1u << CTR_ZDZ_OVD3_STATE_BIT)
#if (                                   \
       (MODYFIKACIA_VERSII_PZ != 7)&&   \
       (MODYFIKACIA_VERSII_PZ != 17)    \
    )
        | 
        (1u << CTR_ZDZ_OVD4_STATE_BIT)
#endif
       )
    ) >> CTR_ZDZ_OVD1_STATE_BIT;

  new_OVD |= (swiched_on_OVD ^ swiched_on_OVD_tmp) & swiched_on_OVD_tmp;
  swiched_on_OVD = swiched_on_OVD_tmp;

  /***
  Перевірка, чи не вимкнутий зараз є канал по якому не проходи тест ОВД
  ***/
  {
    uint32_t temp_value = zdz_ovd_diagnostyka & (uint32_t)(~swiched_on_OVD);
    if (temp_value)
    {
      if (temp_value & (1u << 0))
      {
        zdz_ovd_diagnostyka &= ~(1u << 0);
      
        _SET_BIT(clear_diagnostyka, TEST_OVD1);
      }
    
      if (temp_value & (1u << 1))
      {
        zdz_ovd_diagnostyka &= ~(1u << 1);
      
        _SET_BIT(clear_diagnostyka, TEST_OVD2);
      }

      if (temp_value & (1u << 2))
      {
        zdz_ovd_diagnostyka &= ~(1u << 2);
      
        _SET_BIT(clear_diagnostyka, TEST_OVD3);
      }

#if (                                   \
       (MODYFIKACIA_VERSII_PZ != 7)&&   \
       (MODYFIKACIA_VERSII_PZ != 17)    \
    )
      if (temp_value & (1u << 3))
      {
        zdz_ovd_diagnostyka &= ~(1u << 3);
      
        _SET_BIT(clear_diagnostyka, TEST_OVD4);
      }
#endif
      
    }
  }
  /***/
   
  //Знімаємо стан з оптоприймача
#if (                                   \
     (MODYFIKACIA_VERSII_PZ == 7) ||    \
     (MODYFIKACIA_VERSII_PZ == 17)      \
    ) 
  uint32_t light = (_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37) >> 11) & 0x7;
#else

  uint32_t light = (_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD26_DD29) >> 12) 
    & 
#if (                                   \
       (MODYFIKACIA_VERSII_PZ != 7)&&   \
       (MODYFIKACIA_VERSII_PZ != 17)    \
    )
    0xf
#else
    0x7
#endif
    ;
  
#endif

  /***
  Код програми, який відповідає за діагностику оптичної системи
  ***/
  if (test_OVD != 0)
  {
    //Стан діагностики
    zdz_ovd_diagnostyka = (light ^ test_OVD) & test_OVD;
    
    if (zdz_ovd_diagnostyka & (1u << 0)) _SET_BIT(set_diagnostyka, TEST_OVD1);
    else _SET_BIT(clear_diagnostyka, TEST_OVD1);
      
    if (zdz_ovd_diagnostyka & (1u << 1)) _SET_BIT(set_diagnostyka, TEST_OVD2);
    else _SET_BIT(clear_diagnostyka, TEST_OVD2);
      
    if (zdz_ovd_diagnostyka & (1u << 2)) _SET_BIT(set_diagnostyka, TEST_OVD3);
    else _SET_BIT(clear_diagnostyka, TEST_OVD3);
    
#if (                                   \
       (MODYFIKACIA_VERSII_PZ != 7)&&   \
       (MODYFIKACIA_VERSII_PZ != 17)    \
    )
      
    if (zdz_ovd_diagnostyka & (1u << 3)) _SET_BIT(set_diagnostyka, TEST_OVD4);
    else _SET_BIT(clear_diagnostyka, TEST_OVD4);
    
#endif    
      
    //Так, як світло випромінювали ми своїм випромінювачем, то для логіки ми  його не приймаємо
    light &= (uint32_t)(~test_OVD);
    
    //Скидаємо тест
    test_OVD = 0;
  }
  
  //Відлік часу між послідовними запусками тестів ОВД
  delta_time_test += DELTA_TIME_FOR_TIMERS;
  
  //Фіксація початку нового тесту всіх ОВД
  if (
      (light == 0)
      &&  
      (
       (new_OVD) ||
       (delta_time_test >= PERIOD_ZDZ_TEST)
      )   
     )   
  {
    new_OVD = 0; /*скидаємо фіксацію введення в роботу нового каналу*/
    delta_time_test = 0;
    
    //Починаємо тест
    test_OVD = 
      swiched_on_OVD 
      &
#if (                                   \
       (MODYFIKACIA_VERSII_PZ != 7)&&   \
       (MODYFIKACIA_VERSII_PZ != 17)    \
    )
    0xf
#else
    0x7
#endif
      ;
  }

  if (test_OVD != 0) zdz_ovd_porig = 1;
  else zdz_ovd_porig = current_settings_prt.zdz_ovd_porig + 1;
  /***/

#endif  

  uint32_t logic_zdz_0 = 0;
  
#if (                                   \
       (MODYFIKACIA_VERSII_PZ == 0) ||  \
       (MODYFIKACIA_VERSII_PZ == 3) ||  \
       (MODYFIKACIA_VERSII_PZ == 4) ||  \
       (MODYFIKACIA_VERSII_PZ == 6) ||  \
       (MODYFIKACIA_VERSII_PZ == 7) ||  \
       (MODYFIKACIA_VERSII_PZ == 10)||  \
       (MODYFIKACIA_VERSII_PZ == 13)||  \
       (MODYFIKACIA_VERSII_PZ == 14)||  \
       (MODYFIKACIA_VERSII_PZ == 17)    \
      )   
  
#if (                                   \
       (MODYFIKACIA_VERSII_PZ != 7)&&   \
       (MODYFIKACIA_VERSII_PZ != 17)    \
    )
  uint32_t logic_zdz_1 = 0;
#endif
  
#endif

  logic_zdz_0 |= (_CHECK_SET_BIT(p_active_functions, RANG_PUSK_ZDZ_VID_DV) != 0) << 0; //Пуск ЗДЗ от ДВ

  logic_zdz_0 |= (_CHECK_SET_BIT(p_active_functions, RANG_BLOCK_ZDZ) == 0) << 1; //Блок. ЗДЗ
  
  _AND2(control_zdz_tmp, CTR_ZDZ_STATE_BIT, logic_zdz_0, 1, logic_zdz_0, 2);
  _AND2(logic_zdz_0, 2, logic_zdz_0, 0, logic_zdz_0, 3);
  //"Св.ЗДЗ від ДВ"
  if (_GET_STATE(logic_zdz_0, 3))
    _SET_BIT(p_active_functions, RANG_LIGHT_ZDZ_FROM_DV);
  else
    _CLEAR_BIT(p_active_functions, RANG_LIGHT_ZDZ_FROM_DV);

#if (                                   \
       (MODYFIKACIA_VERSII_PZ == 0) ||  \
       (MODYFIKACIA_VERSII_PZ == 3) ||  \
       (MODYFIKACIA_VERSII_PZ == 4) ||  \
       (MODYFIKACIA_VERSII_PZ == 6) ||  \
       (MODYFIKACIA_VERSII_PZ == 7) ||  \
       (MODYFIKACIA_VERSII_PZ == 10)||  \
       (MODYFIKACIA_VERSII_PZ == 13)||  \
       (MODYFIKACIA_VERSII_PZ == 14)||  \
       (MODYFIKACIA_VERSII_PZ == 17)    \
      ) 

  uint32_t zdz_ovd_diagnostyka_inv = (uint32_t)(~zdz_ovd_diagnostyka);
  _AND4(logic_zdz_0, 2, light, 0, zdz_ovd_diagnostyka_inv, 0, control_zdz_tmp, CTR_ZDZ_OVD1_STATE_BIT, logic_zdz_0, 4);
  //"Св.ЗДЗ від ОВД1"
  if (_GET_STATE(logic_zdz_0, 4))
    _SET_BIT(p_active_functions, RANG_LIGHT_ZDZ_FROM_OVD1);
  else
    _CLEAR_BIT(p_active_functions, RANG_LIGHT_ZDZ_FROM_OVD1);

  _AND4(logic_zdz_0, 2, light, 1, zdz_ovd_diagnostyka_inv, 1, control_zdz_tmp, CTR_ZDZ_OVD2_STATE_BIT, logic_zdz_0, 5);
  //"Св.ЗДЗ від ОВД2"
  if (_GET_STATE(logic_zdz_0, 5))
    _SET_BIT(p_active_functions, RANG_LIGHT_ZDZ_FROM_OVD2);
  else
    _CLEAR_BIT(p_active_functions, RANG_LIGHT_ZDZ_FROM_OVD2);

  _AND4(logic_zdz_0, 2, light, 2, zdz_ovd_diagnostyka_inv, 2, control_zdz_tmp, CTR_ZDZ_OVD3_STATE_BIT, logic_zdz_0, 6);
  //"Св.ЗДЗ від ОВД3"
  if (_GET_STATE(logic_zdz_0, 6))
    _SET_BIT(p_active_functions, RANG_LIGHT_ZDZ_FROM_OVD3);
  else
    _CLEAR_BIT(p_active_functions, RANG_LIGHT_ZDZ_FROM_OVD3);
  
#if (                                   \
       (MODYFIKACIA_VERSII_PZ != 7)&&   \
       (MODYFIKACIA_VERSII_PZ != 17)    \
    )

  //"Св.ЗДЗ від ОВД4"
  _AND4(logic_zdz_0, 2, light, 3, zdz_ovd_diagnostyka_inv, 3, control_zdz_tmp, CTR_ZDZ_OVD4_STATE_BIT, logic_zdz_1, 0);
  if (_GET_STATE(logic_zdz_1, 0))
    _SET_BIT(p_active_functions, RANG_LIGHT_ZDZ_FROM_OVD4);
  else
    _CLEAR_BIT(p_active_functions, RANG_LIGHT_ZDZ_FROM_OVD4);

  _OR5(logic_zdz_0, 3, logic_zdz_0, 4, logic_zdz_0, 5, logic_zdz_0, 6, logic_zdz_1, 0, logic_zdz_0, 7);

#else

  _OR4(logic_zdz_0, 3, logic_zdz_0, 4, logic_zdz_0, 5, logic_zdz_0, 6, logic_zdz_0, 7);
#endif
  
#else
  if (_GET_STATE(logic_zdz_0, 3)) _SET_STATE(logic_zdz_0, 7);
#endif
  
  uint32_t logic_zdz_rez = 0;
  if (current_settings_prt.ctrl_zdz_type == ZDZ_CTRL_NONE)
  {
    logic_zdz_rez |= _GET_STATE(logic_zdz_0, 7) << 0;
  }
  else if (current_settings_prt.ctrl_zdz_type < _ZDZ_CTRL_NUMBER)
  {
    if (
        (current_settings_prt.ctrl_zdz_type == ZDZ_CTRL_I) ||
        (current_settings_prt.ctrl_zdz_type == ZDZ_CTRL_I_OR_U) ||
        (current_settings_prt.ctrl_zdz_type == ZDZ_CTRL_I_AND_U)
       )   
    {
      if(
         (_GET_STATE(control_zdz_tmp, (CTR_ZDZ_STARTED_FROM_MTZ1_BIT - (_CTR_ZDZ_PART_III - _CTR_ZDZ_PART_II))))
         &&
         (
          (_CHECK_SET_BIT(p_active_functions, RANG_PO_MTZ1) != 0) ||
          (_CHECK_SET_BIT(p_active_functions, RANG_PO_MTZN1_VPERED) != 0) ||
          (_CHECK_SET_BIT(p_active_functions, RANG_PO_MTZN1_NAZAD) != 0) ||
          (_CHECK_SET_BIT(p_active_functions, RANG_PO_MTZPN1) != 0)
         ) 
        )   
      {
        _SET_STATE(logic_zdz_0, 8);
      }

      if(
         (_GET_STATE(control_zdz_tmp, (CTR_ZDZ_STARTED_FROM_MTZ2_BIT - (_CTR_ZDZ_PART_III - _CTR_ZDZ_PART_II))))
         &&
         (
          (_CHECK_SET_BIT(p_active_functions, RANG_PO_MTZ2) != 0) ||
          (_CHECK_SET_BIT(p_active_functions, RANG_PO_MTZN2_VPERED) != 0) ||
          (_CHECK_SET_BIT(p_active_functions, RANG_PO_MTZN2_NAZAD) != 0) ||
          (_CHECK_SET_BIT(p_active_functions, RANG_PO_MTZPN2) != 0)
         ) 
        )   
      {
        _SET_STATE(logic_zdz_0, 9);
      }

      if(
         (_GET_STATE(control_zdz_tmp, (CTR_ZDZ_STARTED_FROM_MTZ3_BIT - (_CTR_ZDZ_PART_III - _CTR_ZDZ_PART_II))))
         &&
         (
          (_CHECK_SET_BIT(p_active_functions, RANG_PO_MTZ3) != 0) ||
          (_CHECK_SET_BIT(p_active_functions, RANG_PO_MTZN3_VPERED) != 0) ||
          (_CHECK_SET_BIT(p_active_functions, RANG_PO_MTZN3_NAZAD) != 0) ||
          (_CHECK_SET_BIT(p_active_functions, RANG_PO_MTZPN3) != 0)
         ) 
        )   
      {
        _SET_STATE(logic_zdz_0, 10);
      }

      if(
         (_GET_STATE(control_zdz_tmp, (CTR_ZDZ_STARTED_FROM_MTZ4_BIT - (_CTR_ZDZ_PART_III - _CTR_ZDZ_PART_II))))
         &&
         (
          (_CHECK_SET_BIT(p_active_functions, RANG_PO_MTZ4) != 0) ||
          (_CHECK_SET_BIT(p_active_functions, RANG_PO_MTZN4_VPERED) != 0) ||
          (_CHECK_SET_BIT(p_active_functions, RANG_PO_MTZN4_NAZAD) != 0) ||
          (_CHECK_SET_BIT(p_active_functions, RANG_PO_MTZPN4) != 0)
         ) 
        )   
      {
        _SET_STATE(logic_zdz_0, 11);
      }
      
      _OR4(logic_zdz_0, 8, logic_zdz_0, 9, logic_zdz_0, 10, logic_zdz_0, 11, logic_zdz_0, 12);
    }

    if (
        (current_settings_prt.ctrl_zdz_type == ZDZ_CTRL_U) ||
        (current_settings_prt.ctrl_zdz_type == ZDZ_CTRL_I_OR_U) ||
        (current_settings_prt.ctrl_zdz_type == ZDZ_CTRL_I_AND_U)
       )   
    {
      if(
         (_GET_STATE(control_zdz_tmp, (CTR_ZDZ_STARTED_FROM_UMIN1_BIT - (_CTR_ZDZ_PART_III - _CTR_ZDZ_PART_II)))) &&
         (_CHECK_SET_BIT(p_active_functions, RANG_PO_UMIN1) != 0)
        )   
      {
        _SET_STATE(logic_zdz_0, 13);
      }
      
      if(
         (_GET_STATE(control_zdz_tmp, (CTR_ZDZ_STARTED_FROM_UMIN2_BIT - (_CTR_ZDZ_PART_III - _CTR_ZDZ_PART_II)))) &&
         (_CHECK_SET_BIT(p_active_functions, RANG_PO_UMIN2) != 0)
        )   
      {
        _SET_STATE(logic_zdz_0, 14);
      }
      
      _OR2(logic_zdz_0, 13, logic_zdz_0, 14, logic_zdz_0, 15);
    }

    if (current_settings_prt.ctrl_zdz_type == ZDZ_CTRL_I)
    {
      _AND2(logic_zdz_0, 7, logic_zdz_0, 12, logic_zdz_rez, 0);
    }
    else if (current_settings_prt.ctrl_zdz_type == ZDZ_CTRL_U)
    {
      _AND2(logic_zdz_0, 7, logic_zdz_0, 15, logic_zdz_rez, 0);
    }
    else if (current_settings_prt.ctrl_zdz_type == ZDZ_CTRL_3I0)
    {
      if(
         (_CHECK_SET_BIT(p_active_functions, RANG_PO_NZZ) != 0) ||
         (_CHECK_SET_BIT(p_active_functions, RANG_PO_3I0) != 0)
        )   
      {
        _SET_STATE(logic_zdz_0, 17);
      }
      _AND2(logic_zdz_0, 7, logic_zdz_0, 17, logic_zdz_rez, 0);
    }
    else
    {
      if (current_settings_prt.ctrl_zdz_type == ZDZ_CTRL_I_OR_U)
      {
        _OR2(logic_zdz_0, 12, logic_zdz_0, 15, logic_zdz_0, 16);
      }
      else if (current_settings_prt.ctrl_zdz_type == ZDZ_CTRL_I_AND_U)
      {
        _AND2(logic_zdz_0, 12, logic_zdz_0, 15, logic_zdz_0, 16);
      }
      else
      {
        //Якщо сюди дійшла програма, значить відбулася недопустива помилка, тому треба зациклити програму, щоб вона пішла на перезагрузку
        total_error_sw_fixed();
      }

      _AND2(logic_zdz_0, 7, logic_zdz_0, 16, logic_zdz_rez, 0);
    }
  }
  else
  {
    //Якщо сюди дійшла програма, значить відбулася недопустива помилка, тому треба зациклити програму, щоб вона пішла на перезагрузку
    total_error_sw_fixed();
  }
  
  //"ПО ЗДЗ"
  if (_GET_STATE(logic_zdz_rez, 0))
    _SET_BIT(p_active_functions, RANG_PO_ZDZ);
  else
    _CLEAR_BIT(p_active_functions, RANG_PO_ZDZ);
  
  _TIMER_T_0(INDEX_TIMER_ZDZ, current_settings_prt.timeout_zdz[number_group_stp], logic_zdz_rez, 0, logic_zdz_rez, 1);
  //"ЗДЗ"
  if (_GET_STATE(logic_zdz_rez, 1))
    _SET_BIT(p_active_functions, RANG_ZDZ);
  else
    _CLEAR_BIT(p_active_functions, RANG_ZDZ);
}
/*****************************************************/

/*****************************************************/
// ЗЗ
/*****************************************************/
inline void zz_handler(unsigned int *p_active_functions, unsigned int number_group_stp)
{
  /********************************
  Сектор СЗЗ

  "Сектор СЗЗ" визначається при порогах:
  
  Струм 3I0 (вимірюється у десятих міліамперів) > 5 (мА)
  Струм 3U0 (вимірюється у         мілівольтах) > 6 (В)
  
  *********************************/
  unsigned int porig_3I0;
  if (Nzz_3I0_bilshe_porogu == 0) porig_3I0 = PORIG_CHUTLYVOSTI_3I0*KOEF_POVERNENNJA_3I0_SECTOR_BLK/100;
  else porig_3I0 = PORIG_CHUTLYVOSTI_3I0;
  unsigned int Nzz_3I0_bilshe_porogu_tmp = Nzz_3I0_bilshe_porogu = (measurement[IM_3I0] >= porig_3I0);

  unsigned int porig_3U0;
  if (Nzz_3U0_bilshe_porogu == 0) porig_3U0 = PORIG_CHUTLYVOSTI_VOLTAGE_ANGLE*KOEF_POVERNENNJA_U_SECTOR_BLK/100;
  else porig_3U0 = PORIG_CHUTLYVOSTI_VOLTAGE_ANGLE;
  unsigned int Nzz_3U0_bilshe_porogu_tmp = Nzz_3U0_bilshe_porogu = (measurement[IM_3U0] >= porig_3U0);
      
  if (
      ((current_settings_prt.control_zz & CTR_ZZ1_TYPE) == 0) &&
      (Nzz_3I0_bilshe_porogu_tmp != 0) &&
      (Nzz_3U0_bilshe_porogu_tmp != 0)
     )
  {
    unsigned int sector_i_minus_u;
    if (sector_NZZ == 0)
    {
      //Працюємо по сектору спрацювання для СЗЗ
      sector_i_minus_u = sector_i_minus_u_1;
    }
    else
    {
      //Працюємо по сектору відпускання для СЗЗ
      sector_i_minus_u = sector_i_minus_u_2;
    }
  
    if ((current_settings_prt.control_zz & CTR_ZZ1_SECTOR) != 0)
    {
      //Працюємо по сектору "ВПЕРЕД" (квадранти 1 і 2 без "сірої області")
      if ((sector_i_minus_u == 1) || (sector_i_minus_u == 2))
      {
        /*
        Будь-яке ненульове значення означає, що ми в секторі спрацювання
        */
        sector_NZZ = 0xff;
      }
      else
      {
        /*
        Нульове значення означає, що ми не в секторі спрацювання
        */
        sector_NZZ = 0;
      }
    }
    else
    {
      //Працюємо по сектору "НАЗАД" (квадранти 3 і 4 без "сірої області")
      if ((sector_i_minus_u == 3) || (sector_i_minus_u == 4))
      {
        /*
        Будь-яке ненульове значення означає, що ми в секторі спрацювання
        */
        sector_NZZ = 0xff;
      }
      else
      {
        /*
        Нульове значення означає, що ми не в секторі спрацювання
        */
        sector_NZZ = 0;
      }
    }
  }
  else sector_NZZ = 0;

  
  //Встановлюємо сигнал "Сектор СЗЗ",якщо він потрібний - у іншому випадку він скинутий
  if (sector_NZZ != 0)
    _SET_BIT(p_active_functions, RANG_SECTOR_NZZ);
  else
    _CLEAR_BIT(p_active_functions, RANG_SECTOR_NZZ);
  /*******************************/

  /*******************************/
  //Визначаємо по якому вимірюванню будемо аналізувати і який коефіцієнт повернення
  /*******************************/
  unsigned int max_3i0_current, koef_povernennja;
  
  if((current_settings_prt.control_zz & CTR_ZZ1_TYPE) == 0)
  {
    max_3i0_current = measurement[IM_3I0];
    koef_povernennja = KOEF_POVERNENNJA_GENERAL_UP;
  }
  else
  {
    max_3i0_current = measurement[IM_3I0_other_g];
    koef_povernennja = KOEF_POVERNENNJA_3I0_OTHER;
  }
  /*******************************/
  
  unsigned int setpoint;
  /********************************
  ПО 3I0 (на рівні вимірювальної величини, уставкм спрацювання/відпускання)
  ********************************/
  /*
  Оскільки вимірювання у нас з точністю до десятих міліампер, а уставка у нас у міліамперах,
  то множимо устаку на 10, щоб отримати її також у десятих міліамперів
  */
  if (po_3I0 == 0)
  {
    //Працюємо по уставці спрацювання для 3I0
    setpoint = current_settings_prt.setpoint_zz_3I0[number_group_stp]*10;
  }
  else
  {
    //Працюємо по уставці відпускання для 3I0
    /*
    Алгебраїчне спрощення виразу
    setpoint = (current_settings_prt.setpoint_zz_3I0[number_group_stp]*koef_povernennja/100)*10 =  current_settings_prt.setpoint_zz_3I0[number_group_stp]*koef_povernennja/10
    */
    setpoint = current_settings_prt.setpoint_zz_3I0[number_group_stp]*koef_povernennja/10;
  }
  
  if (max_3i0_current >= setpoint)
  {
    /*
    Будь-яке ненульове значення означає, що вимірюване значення вище-рівне порогу порівнювальної уставки
    */
    po_3I0 = 0xff;
  }
  else
  {
    /*
    Нульове значення означає, що вимірюване значення нижче порогу порівнювальної уставки
    */
    po_3I0 = 0;
  }
  /*******************************/

  /********************************
  ПО 3U0 (на рівні вимірювальної величини, уставкм спрацювання/відпускання)
  ********************************/
  if (po_3U0 == 0)
  {
    //Працюємо по уставці спрацювання для 3U0
    setpoint = current_settings_prt.setpoint_zz_3U0[number_group_stp];
  }
  else
  {
    //Працюємо по уставці відпускання для 3U0
    setpoint = current_settings_prt.setpoint_zz_3U0[number_group_stp]*KOEF_POVERNENNJA_3U0/100;
  }
  
  if (measurement[IM_3U0] >= setpoint)
  {
    /*
    Будь-яке ненульове значення означає, що вимірюване значення вище-рівне порогу порівнювальної уставки
    */
    po_3U0 = 0xff;
  }
  else
  {
    /*
    Нульове значення означає, що вимірюване значення нижче порогу порівнювальної уставки
    */
    po_3U0 = 0;
  }
  /*******************************/
  
  if (_CHECK_SET_BIT(p_active_functions, RANG_BLOCK_NZZ) == 0)
  {
    //СЗЗ, ЗЗ/3I0 і ЗЗ/3U0 не блокується з дискретного входу
    /*******************************/
    //СЗЗ
    /*******************************/
    if (
        ((current_settings_prt.control_zz & CTR_ZZ1_NZZ_STATE) != 0) &&
        ((current_settings_prt.control_zz & CTR_ZZ1_TYPE) == 0)
       )   
    {
      //1 ступінь ЗЗ/3U0 включена, дозволена і не блокується
    
      unsigned int previous_state_po_nzz = _CHECK_SET_BIT(p_active_functions, RANG_PO_NZZ);
    
      //Виставляємо, або скидаємо сигнал "ПО NZZ"
      if (
          (po_3I0     != 0) &&
          (po_3U0     != 0) &&
          (sector_NZZ != 0)
         )
      {
        //Існує умова роботи ПО СЗЗ
        if(previous_state_po_nzz == 0)
        {
          //Встановлюємо сигнал "ПО NZZ"
          _SET_BIT(p_active_functions, RANG_PO_NZZ);
      
          //Запускаємо таймер СЗЗ, якщо він ще не запущений
          global_timers[INDEX_TIMER_NZZ] = 0;
        }
      }
      else 
      {
        //Не існує умова роботи ПО СЗЗ
        if(previous_state_po_nzz != 0)
        {
          //Скидаємо сигнал "ПО СЗЗ"
          _CLEAR_BIT(p_active_functions, RANG_PO_NZZ);
          //Це є умовою також скидання сигналу "Сраб. СЗЗ"
          _CLEAR_BIT(p_active_functions, RANG_NZZ);
          //Якщо таймер ще не скинутий, то скидаємо його
          if ( global_timers[INDEX_TIMER_NZZ] >=0) global_timers[INDEX_TIMER_NZZ] = -1;
        }
      }
    
      if(global_timers[INDEX_TIMER_NZZ] >= current_settings_prt.timeout_nzz[number_group_stp])
      {
        //Якщо витримана "Витримка СЗЗ" то встановлюємо сигнал "Сраб. СЗЗ"
        _SET_BIT(p_active_functions, RANG_NZZ);

        //Скидаємо таймер ПО СЗЗ
        global_timers[INDEX_TIMER_NZZ] = -1;
      }
    }
    else
    {
      //Якщо СЗЗ не встановлена, то треба скинути всі таймери і сигнали, які за неї відповідають
      _CLEAR_BIT(p_active_functions, RANG_PO_NZZ);
      _CLEAR_BIT(p_active_functions, RANG_NZZ);
      global_timers[INDEX_TIMER_NZZ] = -1;
    }  
    /*******************************/

    /*******************************/
    //1 ступінь ЗЗ/3U0
    /*******************************/
    if ((current_settings_prt.control_zz & CTR_ZZ1_3U0_STATE) != 0)
    {
      //1 ступінь ЗЗ/3U0 включена і не блокується
    
      unsigned int previous_state_po_3U0 = _CHECK_SET_BIT(p_active_functions, RANG_PO_3U0);
    
      //Виставляємо, або скидаємо сигнал "ПО ЗЗ/3U0"
      if (po_3U0 != 0)
      {
        //Максимальна напругу 3U0 перевищує свою уставку
        if(previous_state_po_3U0 == 0)
        {
          //Встановлюємо сигнал "ПО ЗЗ/3U0"
          _SET_BIT(p_active_functions, RANG_PO_3U0);
        
          //Запускаємо таймер ЗЗ/3U0, якщо він ще не запущений
          global_timers[INDEX_TIMER_ZZ_3U0] = 0;
        }
      }
      else 
      {
        //Максимальна напругу 3I0 нижче уставки
        if(previous_state_po_3U0 != 0)
        {
          //Скидаємо сигнал "ПО ЗЗ/3U0"
          _CLEAR_BIT(p_active_functions, RANG_PO_3U0);
          //Це є умовою також скидання сигналу "Сраб. ЗЗ/3U0"
          _CLEAR_BIT(p_active_functions, RANG_3U0);
          //Якщо таймер ще не скинутий, то скидаємо його
          if ( global_timers[INDEX_TIMER_ZZ_3U0] >=0) global_timers[INDEX_TIMER_ZZ_3U0] = -1;
        }
      }
    
      if(global_timers[INDEX_TIMER_ZZ_3U0] >= current_settings_prt.timeout_zz_3U0[number_group_stp])
      {
        //Якщо витримана "Витримка ЗЗ/3U0" то встановлюємо сигнал "Сраб. ЗЗ/3U0"
        _SET_BIT(p_active_functions, RANG_3U0);

        //Скидаємо таймер ПО ЗЗ/3U0
        global_timers[INDEX_TIMER_ZZ_3U0] = -1;
      }
    }
    else
    {
      //Якщо 1 ступінь ЗЗ/3U0 не встановлена, то треба скинути всі таймери і сигнали, які за неї відповідають
      _CLEAR_BIT(p_active_functions, RANG_PO_3U0);
      _CLEAR_BIT(p_active_functions, RANG_3U0);
      global_timers[INDEX_TIMER_ZZ_3U0] = -1;
    }    
    /*******************************/
  
    /*******************************/
    //1 ступінь ЗЗ/3I0
    /*******************************/
    if ((current_settings_prt.control_zz & CTR_ZZ1_3I0_STATE) != 0)
    {
      //1 ступінь ЗЗ/3I0 включена і не блокується
    
      unsigned int previous_state_po_3I0 = _CHECK_SET_BIT(p_active_functions, RANG_PO_3I0);
    
      //Виставляємо, або скидаємо сигнал "ПО ЗЗ/3I0"
      if (po_3I0 != 0)
      {
        //Максимальний стум 3I0 перевищує свою уставку
        if(previous_state_po_3I0 == 0)
        {
          //Встановлюємо сигнал "ПО ЗЗ/3I0"
          _SET_BIT(p_active_functions, RANG_PO_3I0);
      
          //Запускаємо таймер ЗЗ/3I0, якщо він ще не запущений
          global_timers[INDEX_TIMER_ZZ_3I0] = 0;
        }
      }
      else 
      {
        //Максимальний стум 3I0 нижче уставки
        if(previous_state_po_3I0 != 0)
        {
          //Скидаємо сигнал "ПО ЗЗ/3I0"
          _CLEAR_BIT(p_active_functions, RANG_PO_3I0);
          //Це є умовою також скидання сигналу "Сраб. ЗЗ/3I0"
          _CLEAR_BIT(p_active_functions, RANG_3I0);
          //Якщо таймер ще не скинутий, то скидаємо його
          if ( global_timers[INDEX_TIMER_ZZ_3I0] >=0) global_timers[INDEX_TIMER_ZZ_3I0] = -1;
        }
      }
    
      if(global_timers[INDEX_TIMER_ZZ_3I0] >= current_settings_prt.timeout_zz_3I0[number_group_stp])
      {
        //Якщо витримана "Витримка ЗЗ/3I0" то встановлюємо сигнал "Сраб. ЗЗ"
        _SET_BIT(p_active_functions, RANG_3I0);

        //Скидаємо таймер ПО ЗЗ/3I0
        global_timers[INDEX_TIMER_ZZ_3I0] = -1;
      }
    }
    else
    {
      //Якщо 1 ступінь ЗЗ/3I0 не встановлена, то треба скинути всі таймери і сигнали, які за неї відповідають
      _CLEAR_BIT(p_active_functions, RANG_PO_3I0);
      _CLEAR_BIT(p_active_functions, RANG_3I0);
      global_timers[INDEX_TIMER_ZZ_3I0] = -1;
    }  
    /*******************************/
  }
  else
  {
    //Захисти ЗЗ/СЗЗ блокується з дискретного входу, то треба скинути всі таймери і сигнали, які за них відповідають
    _CLEAR_BIT(p_active_functions, RANG_PO_NZZ);
    _CLEAR_BIT(p_active_functions, RANG_NZZ);
    _CLEAR_BIT(p_active_functions, RANG_PO_3I0);
    _CLEAR_BIT(p_active_functions, RANG_3I0);
    _CLEAR_BIT(p_active_functions, RANG_PO_3U0);
    _CLEAR_BIT(p_active_functions, RANG_3U0);

    global_timers[INDEX_TIMER_NZZ   ] = -1;
    global_timers[INDEX_TIMER_ZZ_3U0] = -1;
    global_timers[INDEX_TIMER_ZZ_3I0] = -1;
  }  
}
/*****************************************************/

/*****************************************************/
//ТЗНП
/*****************************************************/
inline void tznp_handler(unsigned int *p_active_functions, unsigned int number_group_stp)
{
#define CTR_TZNP                CTR_TZNP1      
#define CTR_TZNP_VPERED         CTR_TZNP1_VPERED      
#define CTR_TZNP_NAZAD          CTR_TZNP1_NAZAD      

#define RANG_BLOCK_TZNP         RANG_BLOCK_TZNP1
#define RANG_SECTOR_TZNP_VPERED RANG_SECTOR_TZNP1_VPERED
#define RANG_SECTOR_TZNP_NAZAD  RANG_SECTOR_TZNP1_NAZAD
#define RANG_PO_3I0_TZNP_VPERED RANG_PO_3I0_TZNP1_VPERED
#define RANG_PO_3I0_TZNP_NAZAD  RANG_PO_3I0_TZNP1_NAZAD
#define RANG_PO_3U0_TZNP_VPERED RANG_PO_3U0_TZNP1_VPERED
#define RANG_PO_3U0_TZNP_NAZAD  RANG_PO_3U0_TZNP1_NAZAD
#define RANG_PO_TZNP_VPERED     RANG_PO_TZNP1_VPERED
#define RANG_PO_TZNP_NAZAD      RANG_PO_TZNP1_NAZAD
#define RANG_TZNP               RANG_TZNP1
  
  unsigned int maska_ctrl_bits = CTR_TZNP | CTR_TZNP_VPERED | CTR_TZNP_NAZAD;
  
  for (unsigned int tznp = 0; tznp < 3; tznp++)
  {
    unsigned int setpoint_tznp_3I0_vpered = 0;
    unsigned int setpoint_tznp_3U0_vpered = 0;
    unsigned int setpoint_tznp_3I0_nazad = 0;
    unsigned int setpoint_tznp_3U0_nazad = 0;

    int timeout_tznp_vpered = 0;
    int timeout_tznp_nazad = 0;
    
    unsigned int index_timer_vpered = 0;
    unsigned int index_timer_nazad = 0;
    
    unsigned int control_tznp = 0;
    
    unsigned int shift_to_base_rang_index = 0;
    
    switch(tznp)
    {
    case 0:
      {
        setpoint_tznp_3I0_vpered = current_settings_prt.setpoint_tznp_1_3I0_vpered[number_group_stp];
        setpoint_tznp_3U0_vpered = current_settings_prt.setpoint_tznp_1_3U0_vpered[number_group_stp];
        setpoint_tznp_3I0_nazad  = current_settings_prt.setpoint_tznp_1_3I0_nazad[number_group_stp];
        setpoint_tznp_3U0_nazad  = current_settings_prt.setpoint_tznp_1_3U0_nazad[number_group_stp];
        
        timeout_tznp_vpered = current_settings_prt.timeout_tznp_1_vpered[number_group_stp];
        timeout_tznp_nazad  = current_settings_prt.timeout_tznp_1_nazad[number_group_stp]; 
        
        index_timer_vpered = INDEX_TIMER_TZNP1_VPERED; 
        index_timer_nazad  = INDEX_TIMER_TZNP1_NAZAD;
        
        control_tznp = (current_settings_prt.control_tznp >> INDEX_ML_CTR_TZNP1) & maska_ctrl_bits;
        
        shift_to_base_rang_index = RANG_BLOCK_TZNP1 - RANG_BLOCK_TZNP;
        
        break;
      }
    case 1:
      {
        setpoint_tznp_3I0_vpered = current_settings_prt.setpoint_tznp_2_3I0_vpered[number_group_stp];
        setpoint_tznp_3U0_vpered = current_settings_prt.setpoint_tznp_2_3U0_vpered[number_group_stp];
        setpoint_tznp_3I0_nazad  = current_settings_prt.setpoint_tznp_2_3I0_nazad[number_group_stp];
        setpoint_tznp_3U0_nazad  = current_settings_prt.setpoint_tznp_2_3U0_nazad[number_group_stp];
        
        timeout_tznp_vpered = current_settings_prt.timeout_tznp_2_vpered[number_group_stp];
        timeout_tznp_nazad  = current_settings_prt.timeout_tznp_2_nazad[number_group_stp]; 
        
        index_timer_vpered = INDEX_TIMER_TZNP2_VPERED; 
        index_timer_nazad  = INDEX_TIMER_TZNP2_NAZAD;
        
        control_tznp = (current_settings_prt.control_tznp >> INDEX_ML_CTR_TZNP2) & maska_ctrl_bits;
        
        shift_to_base_rang_index = RANG_BLOCK_TZNP2 - RANG_BLOCK_TZNP;
        
        break;
      }
    case 2:
      {
        setpoint_tznp_3I0_vpered = current_settings_prt.setpoint_tznp_3_3I0_vpered[number_group_stp];
        setpoint_tznp_3U0_vpered = current_settings_prt.setpoint_tznp_3_3U0_vpered[number_group_stp];
        setpoint_tznp_3I0_nazad  = current_settings_prt.setpoint_tznp_3_3I0_nazad[number_group_stp];
        setpoint_tznp_3U0_nazad  = current_settings_prt.setpoint_tznp_3_3U0_nazad[number_group_stp];
        
        timeout_tznp_vpered = current_settings_prt.timeout_tznp_3_vpered[number_group_stp];
        timeout_tznp_nazad  = current_settings_prt.timeout_tznp_3_nazad[number_group_stp]; 
        
        index_timer_vpered = INDEX_TIMER_TZNP3_VPERED; 
        index_timer_nazad  = INDEX_TIMER_TZNP3_NAZAD;
        
        control_tznp = (current_settings_prt.control_tznp >> INDEX_ML_CTR_TZNP3) & maska_ctrl_bits;
        
        shift_to_base_rang_index = RANG_BLOCK_TZNP3 - RANG_BLOCK_TZNP;
        
        break;
      }
    default:
      {
        //Теоретично цього ніколи не мало б бути
        total_error_sw_fixed();
      }
    }

    unsigned int logic_TZNP_0 = 0;
    unsigned int setpoint;
    
    /***
    Сектор вперед
    ***/
    if(sector_directional_tznp[tznp] == 1)
    {
      logic_TZNP_0 |= 1 << 0;
      _SET_BIT(p_active_functions, (shift_to_base_rang_index + RANG_SECTOR_TZNP_VPERED));
    }
    else _CLEAR_BIT(p_active_functions, (shift_to_base_rang_index + RANG_SECTOR_TZNP_VPERED));
    /***/
    
    /***
    Сектор назад
    ***/
    if(sector_directional_tznp[tznp] == 2)
    {
      logic_TZNP_0 |= 1 << 1;
      _SET_BIT(p_active_functions, (shift_to_base_rang_index + RANG_SECTOR_TZNP_NAZAD));
    }
    else _CLEAR_BIT(p_active_functions, (shift_to_base_rang_index + RANG_SECTOR_TZNP_NAZAD));
    /***/
    
    /***
    ПО 3I0 ВПЕРЕД
    ***/
    if (_CHECK_SET_BIT(p_active_functions, (shift_to_base_rang_index + RANG_PO_3I0_TZNP_VPERED)) == 0)
    {
      //Працюємо по уставці спрацювання
      setpoint = setpoint_tznp_3I0_vpered;
    }
    else
    {
      //Працюємо по уставці відпускання
      setpoint = setpoint_tznp_3I0_vpered*KOEF_POVERNENNJA_GENERAL_UP/100;
    }
    if (measurement[IM_3I0_r] >= setpoint)
    {
      logic_TZNP_0 |=  1 << 3;
      _SET_BIT(p_active_functions, (shift_to_base_rang_index + RANG_PO_3I0_TZNP_VPERED));
    }
    else _CLEAR_BIT(p_active_functions, (shift_to_base_rang_index + RANG_PO_3I0_TZNP_VPERED));
    /***/
    
    /***
    ПО 3U0 ВПЕРЕД
    ***/
    if (_CHECK_SET_BIT(p_active_functions, (shift_to_base_rang_index + RANG_PO_3U0_TZNP_VPERED)) == 0)
    {
      //Працюємо по уставці спрацювання
      setpoint = setpoint_tznp_3U0_vpered;
    }
    else
    {
      //Працюємо по уставці відпускання
      setpoint = setpoint_tznp_3U0_vpered*KOEF_POVERNENNJA_3U0/100;
    }
    if (measurement[IM_3U0] >= setpoint)
    {
      logic_TZNP_0 |=  1 << 4;
      _SET_BIT(p_active_functions, (shift_to_base_rang_index + RANG_PO_3U0_TZNP_VPERED));
    }
    else _CLEAR_BIT(p_active_functions, (shift_to_base_rang_index + RANG_PO_3U0_TZNP_VPERED));
    /***/
    
    /***
    ПО 3I0 НАЗАД
    ***/
    if (_CHECK_SET_BIT(p_active_functions, (shift_to_base_rang_index + RANG_PO_3I0_TZNP_NAZAD)) == 0)
    {
      //Працюємо по уставці спрацювання
      setpoint = setpoint_tznp_3I0_nazad;
    }
    else
    {
      //Працюємо по уставці відпускання
      setpoint = setpoint_tznp_3I0_nazad*KOEF_POVERNENNJA_GENERAL_UP/100;
    }
    if (measurement[IM_3I0_r] >= setpoint)
    {
      logic_TZNP_0 |=  1 << 5;
      _SET_BIT(p_active_functions, (shift_to_base_rang_index + RANG_PO_3I0_TZNP_NAZAD));
    }
    else _CLEAR_BIT(p_active_functions, (shift_to_base_rang_index + RANG_PO_3I0_TZNP_NAZAD));
    /***/
    
    /***
    ПО 3U0 НАЗАД
    ***/
    if (_CHECK_SET_BIT(p_active_functions, (shift_to_base_rang_index + RANG_PO_3U0_TZNP_NAZAD)) == 0)
    {
      //Працюємо по уставці спрацювання
      setpoint = setpoint_tznp_3U0_nazad;
    }
    else
    {
      //Працюємо по уставці відпускання
      setpoint = setpoint_tznp_3U0_nazad*KOEF_POVERNENNJA_3U0/100;
    }
    if (measurement[IM_3U0] >= setpoint)
    {
      logic_TZNP_0 |=  1 << 6;
      _SET_BIT(p_active_functions, (shift_to_base_rang_index + RANG_PO_3U0_TZNP_NAZAD));
    }
    else _CLEAR_BIT(p_active_functions, (shift_to_base_rang_index + RANG_PO_3U0_TZNP_NAZAD));
    /***/
    
    //Стан ступеню захисту
    logic_TZNP_0 |= ((control_tznp & CTR_TZNP) != 0) << 7;
    
    //Стан блокування захисту
    logic_TZNP_0 |= (_CHECK_SET_BIT(p_active_functions, (shift_to_base_rang_index + RANG_BLOCK_TZNP)) == 0) << 8;
    
    //Стан напрямку ВПЕРЕД
    logic_TZNP_0 |= ((control_tznp & CTR_TZNP_VPERED) != 0) << 10;
    
    //Стан напрямку НАЗАД
    logic_TZNP_0 |= ((control_tznp & CTR_TZNP_NAZAD) != 0) << 11;

    _AND2(logic_TZNP_0, 7, logic_TZNP_0, 8, logic_TZNP_0, 9);

    //ПО ТЗНП вперед
    _AND5(logic_TZNP_0, 9, logic_TZNP_0, 10, logic_TZNP_0, 3, logic_TZNP_0, 4, logic_TZNP_0, 0, logic_TZNP_0, 12);
    if (_GET_STATE(logic_TZNP_0, 12)) _SET_BIT(p_active_functions, (shift_to_base_rang_index + RANG_PO_TZNP_VPERED));
    else _CLEAR_BIT(p_active_functions, (shift_to_base_rang_index + RANG_PO_TZNP_VPERED));
      
    //ПО ТЗНП назад
    _AND5(logic_TZNP_0, 9, logic_TZNP_0, 11, logic_TZNP_0, 5, logic_TZNP_0, 6, logic_TZNP_0, 1, logic_TZNP_0, 13);
    if (_GET_STATE(logic_TZNP_0, 13)) _SET_BIT(p_active_functions, (shift_to_base_rang_index + RANG_PO_TZNP_NAZAD));
    else _CLEAR_BIT(p_active_functions, (shift_to_base_rang_index + RANG_PO_TZNP_NAZAD));

    _TIMER_T_0(index_timer_vpered, timeout_tznp_vpered, logic_TZNP_0, 12, logic_TZNP_0, 14);
    _TIMER_T_0(index_timer_nazad,  timeout_tznp_nazad,  logic_TZNP_0, 13, logic_TZNP_0, 15);
    
    //ТЗНП
    _OR2(logic_TZNP_0, 14, logic_TZNP_0, 15, logic_TZNP_0, 16);
    if (_GET_STATE(logic_TZNP_0, 16)) _SET_BIT(p_active_functions, (shift_to_base_rang_index + RANG_TZNP));
    else _CLEAR_BIT(p_active_functions, (shift_to_base_rang_index + RANG_TZNP));
  }

#undef CTR_TZNP
#undef CTR_TZNP_VPERED
#undef CTR_TZNP_NAZAD

#undef RANG_BLOCK_TZNP
#undef RANG_SECTOR_TZNP_VPERED
#undef RANG_SECTOR_TZNP_NAZAD
#undef RANG_PO_3I0_TZNP_VPERED
#undef RANG_PO_3I0_TZNP_NAZAD
#undef RANG_PO_3U0_TZNP_VPERED
#undef RANG_PO_3U0_TZNP_NAZAD
#undef RANG_PO_TZNP_VPERED
#undef RANG_PO_TZNP_NAZAD
#undef RANG_TZNP
}
/*****************************************************/

/*****************************************************/
// ЗОП(КОФ)
/*****************************************************/
inline void zop_handler(unsigned int *p_active_functions, unsigned int number_group_stp)
{
  /*******************************/
  //Фуксуємо у локальній змінній текучі струми зворотньої послідовності і прямоїпослідовності
  /*******************************/
  unsigned int i2_current = measurement[IM_I2], i1_current = measurement[IM_I1];
  /*******************************/
  
  /*******************************/
  //1 ступінь ЗОП(КОФ)
  /*******************************/
  if ((current_settings_prt.control_zop & CTR_ZOP_STATE) != 0)
  {
    //1 ступінь ЗОП(КОФ) включена
    unsigned int setpoint; //уставка - з якою зрівнюється вимірювальна величина
    unsigned int setpoint_i1, setpoint_i2;
    unsigned int previous_state_po_zop1;
    
    if (i1_bilshe_porogu == 0) setpoint_i1 = MIN_LIMIT_FOR_I1_AND_I2*KOEF_POVERNENNJA_ZOP_BLK/100;
    else setpoint_i1 = MIN_LIMIT_FOR_I1_AND_I2;
    unsigned int i1_bilshe_porogu_tmp = i1_bilshe_porogu = (i1_current >= setpoint_i1);
    
    if (i2_bilshe_porogu == 0) setpoint_i2 = MIN_LIMIT_FOR_I1_AND_I2*KOEF_POVERNENNJA_ZOP_BLK/100;
    else setpoint_i2 = MIN_LIMIT_FOR_I1_AND_I2;
    unsigned int i2_bilshe_porogu_tmp = i2_bilshe_porogu = (i2_current >= setpoint_i2);
    
    //Якщо ПО ЗОП(КОФ) ще не активне, то треба працювати по устаці спацювання - уставці, яка вводиться як основна з системи меню чи верхнього рівня
    //Якщо ПО ЗОП(КОФ) вже спрацювало, то треба працювати по уставці відпускання - береться процент від основної утанки по коефіцієнту повернення
    if(( previous_state_po_zop1 = _CHECK_SET_BIT(p_active_functions, RANG_PO_ZOP) ) == 0 )
    {
      //Працюємо по утавці спрацювання
      setpoint = current_settings_prt.setpoint_zop[number_group_stp];
    }
    else
    {
      //Працюємо по утавці відпускання
      setpoint = current_settings_prt.setpoint_zop[number_group_stp]*KOEF_POVERNENNJA_GENERAL_UP/100;
    }
    
    //Виставляємо, або скидаємо сигнал "ПО КОФ"
    /*
    I2
    -- >= K
    I1
    оскільки величина устаки у нас є перемножена на 1000, ,бо 0,010 відповідає 10, а 1,000 відповідає 1000,
    то K*1000 = SETPOINT =>
        SETPOINT
    K = --------  =>
         1000
    I2    SETPOINT
    -- >= ---------
    I1     1000
    
    струми зворотньої і прямої послідовності не треба ні нащо перемножувати, бо вони є у однакових величинах (міліампери), а нас цікавить їх відношення
    
    (I2*1000) >= (SETPOINT*I1)
    */
    if (
        (i1_bilshe_porogu_tmp != 0) &&
        (i2_bilshe_porogu_tmp != 0) &&
        ((i2_current*1000) >= (i1_current*setpoint))                            && 
        (_CHECK_SET_BIT(p_active_functions, RANG_BLOCK_ZOP) == 0)
       )
    {
      //Існує умова активного пускового органу зворотньої послідовності
      if(previous_state_po_zop1 == 0)
      {
        //Встановлюємо сигнал "ПО КОФ"
        _SET_BIT(p_active_functions, RANG_PO_ZOP);
      
        //Запускаємо таймер ЗОП(КОФ), якщо він ще не запущений
        global_timers[INDEX_TIMER_ZOP] = 0;
      }
    }
    else 
    {
      //Не існує умови активного пускового органу зворотньої послідовності
      if(previous_state_po_zop1 != 0)
      {
        //Скидаємо сигнал "ПО КОФ"
        _CLEAR_BIT(p_active_functions, RANG_PO_ZOP);
        //Це є умовою також скидання сигналу "Сраб. КОФ"
        _CLEAR_BIT(p_active_functions, RANG_ZOP);
        //Якщо таймер ще не скинутий? то скидаємо його
        if ( global_timers[INDEX_TIMER_ZOP] >=0) global_timers[INDEX_TIMER_ZOP] = -1;
      }
    }
    
    if(global_timers[INDEX_TIMER_ZOP] >= current_settings_prt.timeout_zop[number_group_stp])
    {
      //Якщо витримана Витримка ЗОП(КОФ) то встановлюємо сигнал "Сраб. КОФ"
      _SET_BIT(p_active_functions, RANG_ZOP);

      //Скидаємо таймер ЗОП(КОФ)
      global_timers[INDEX_TIMER_ZOP] = -1;
    }
  }
  else
  {
    //Треба скинути всі таймери і сигнали, які за 1 ступінь ЗОП(КОФ) відповідають
    _CLEAR_BIT(p_active_functions, RANG_PO_ZOP);
    _CLEAR_BIT(p_active_functions, RANG_ZOP);
    global_timers[INDEX_TIMER_ZOP] = -1;

  }  
}
/*****************************************************/

/*****************************************************/
// ЗНМИН1
/*****************************************************/
inline void umin1_handler(unsigned int *p_active_functions, unsigned int number_group_stp)
{
  _Bool previous_state_po_umin1 = _CHECK_SET_BIT(p_active_functions, RANG_PO_UMIN1) > 0;
  _Bool previous_state_po_ublk_umin1 = _CHECK_SET_BIT(p_active_functions, RANG_PO_UBLK_UMIN1) > 0;
  _Bool previous_state_po_iblk_umin1 = _CHECK_SET_BIT(p_active_functions, RANG_PO_IBLK_UMIN1) > 0;
  
  unsigned int setpoint1 = previous_state_po_umin1 ?
          current_settings_prt.setpoint_Umin1[number_group_stp] * U_DOWN / 100 :
          current_settings_prt.setpoint_Umin1[number_group_stp];
  
  _Bool Uab_is_smaller_than_Umin1 = measurement[IM_UAB] <= setpoint1;
  _Bool Ubc_is_smaller_than_Umin1 = measurement[IM_UBC] <= setpoint1;
  _Bool Uca_is_smaller_than_Umin1 = measurement[IM_UCA] <= setpoint1;
  
  _Bool Ua_is_smaller_than_Umin1 = measurement[IM_UA] <= setpoint1;
  _Bool Ub_is_smaller_than_Umin1 = measurement[IM_UB] <= setpoint1;
  _Bool Uc_is_smaller_than_Umin1 = measurement[IM_UC] <= setpoint1;
  
  unsigned int setpoint2 = previous_state_po_ublk_umin1 ?
          KOEF_MIN_UMIN * U_DOWN_1 / 100 : KOEF_MIN_UMIN;
          
  _Bool Ua_or_Ub_or_Uc_is_smaller_than_250mV = (measurement[IM_UA] <= setpoint2) || (measurement[IM_UB] <= setpoint2) || (measurement[IM_UC] <= setpoint2);
  _Bool Uab_or_Ubc_or_Uca_is_smaller_than_250mV = (measurement[IM_UAB] <= setpoint2) || (measurement[IM_UBC] <= setpoint2) || (measurement[IM_UCA] <= setpoint2);
  
  unsigned int setpoint3 = previous_state_po_iblk_umin1 ?
          current_settings_prt.setpoint_Umin1_Iblk[number_group_stp] * KOEF_POVERNENNJA_GENERAL_DOWN / 100 :
          current_settings_prt.setpoint_Umin1_Iblk[number_group_stp];
  
  _Bool Ia_and_Ic_is_smaller_than_Iust = (measurement[IM_IA] <= setpoint3) &&
                                         (measurement[IM_IB] <= setpoint3) &&
                                         (measurement[IM_IC] <= setpoint3);
  //М
  unsigned int tmp_value = (((current_settings_prt.control_extra_settings_1 & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE)) == 0) && ((current_settings_prt.control_transformator & MASKA_FOR_BIT(INDEX_ML_CTR_TRANSFORMATOR_PHASE_LINE)) == 0)) << 0;
//  tmp_value |= ((current_settings_prt.control_Umin & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE)) != 0)                                 << 1;
//  _INVERTOR(tmp_value, 1, tmp_value, 1);
  tmp_value |= ((current_settings_prt.control_Umin & CTR_UMIN1) != 0)                                                            << 2;
  tmp_value |= ((current_settings_prt.control_Umin & CTR_PO_UMIN1_OR_AND) != 0)                                                  << 3;
  tmp_value |= ((current_settings_prt.control_Umin & CTR_UMIN1_UBLK) != 0)                                                       << 4;
  tmp_value |= ((current_settings_prt.control_Umin & CTR_UMIN1_IBLK) != 0)                                                       << 5;
  
  //ДВ
  tmp_value |= (_CHECK_SET_BIT(p_active_functions, RANG_BLOCK_UMIN1) != 0)                                     << 6;
  _INVERTOR(tmp_value, 6, tmp_value, 6);
  tmp_value |= (_CHECK_SET_BIT(p_active_functions, RANG_START_UMIN1) != 0)                                     << 7;
  
//  _AND2(tmp_value, 0, tmp_value, 1, tmp_value, 8);
  
  if (_GET_STATE(tmp_value, 0)) 
  {
    //Фазные
    _AND4(Ua_is_smaller_than_Umin1, 0, Ub_is_smaller_than_Umin1, 0, Uc_is_smaller_than_Umin1, 0, tmp_value, 3, tmp_value, 9);
    _OR3(Ua_is_smaller_than_Umin1, 0, Ub_is_smaller_than_Umin1, 0, Uc_is_smaller_than_Umin1, 0, tmp_value, 10);
    _INVERTOR(tmp_value, 3, tmp_value, 3);
    _AND2(tmp_value, 10, tmp_value, 3, tmp_value, 11);
    _AND2(tmp_value, 5, Ia_and_Ic_is_smaller_than_Iust, 0, tmp_value, 12);
    _INVERTOR(tmp_value, 5, tmp_value, 5);
    _AND3(Ua_or_Ub_or_Uc_is_smaller_than_250mV, 0, tmp_value, 4, tmp_value, 5, tmp_value, 13);
    
    //ПО Uблк. Umin1
    if (Ua_or_Ub_or_Uc_is_smaller_than_250mV)
      _SET_BIT(p_active_functions, RANG_PO_UBLK_UMIN1);
    else
      _CLEAR_BIT(p_active_functions, RANG_PO_UBLK_UMIN1);
  } 
  else 
  {
    //Линейные
    _AND4(Uab_is_smaller_than_Umin1, 0, Ubc_is_smaller_than_Umin1, 0, Uca_is_smaller_than_Umin1, 0, tmp_value, 3, tmp_value, 9);
    _OR3(Uab_is_smaller_than_Umin1, 0, Ubc_is_smaller_than_Umin1, 0, Uca_is_smaller_than_Umin1, 0, tmp_value, 10);
    _INVERTOR(tmp_value, 3, tmp_value, 3);
    _AND2(tmp_value, 10, tmp_value, 3, tmp_value, 11);
    _AND2(tmp_value, 5, Ia_and_Ic_is_smaller_than_Iust, 0, tmp_value, 12);
    _INVERTOR(tmp_value, 5, tmp_value, 5);
    _AND3(Uab_or_Ubc_or_Uca_is_smaller_than_250mV, 0, tmp_value, 4, tmp_value, 5, tmp_value, 13);
    
    //ПО Uблк. Umin1
    if (Uab_or_Ubc_or_Uca_is_smaller_than_250mV)
      _SET_BIT(p_active_functions, RANG_PO_UBLK_UMIN1);
    else
      _CLEAR_BIT(p_active_functions, RANG_PO_UBLK_UMIN1);
  }
  _INVERTOR(tmp_value, 12, tmp_value, 12);
  _INVERTOR(tmp_value, 13, tmp_value, 13);
  _OR3(tmp_value, 7, tmp_value, 9, tmp_value, 11, tmp_value, 14);
  
  _AND5(tmp_value, 6, tmp_value, 2, tmp_value, 14, tmp_value, 13, tmp_value, 12, tmp_value, 15);
  
  //ПО Iблк. Umin1
  if (Ia_and_Ic_is_smaller_than_Iust)
    _SET_BIT(p_active_functions, RANG_PO_IBLK_UMIN1);
  else
    _CLEAR_BIT(p_active_functions, RANG_PO_IBLK_UMIN1);
  
  //ПО Umin1
  if (_GET_STATE(tmp_value, 15))
    _SET_BIT(p_active_functions, RANG_PO_UMIN1);
  else
    _CLEAR_BIT(p_active_functions, RANG_PO_UMIN1);
  
  _TIMER_T_0(INDEX_TIMER_UMIN1, current_settings_prt.timeout_Umin1[number_group_stp], tmp_value, 15, tmp_value, 16);
  
  //Сраб. Umin1
  if (_GET_STATE(tmp_value, 16))
    _SET_BIT(p_active_functions, RANG_UMIN1);
  else
    _CLEAR_BIT(p_active_functions, RANG_UMIN1);
}
/*****************************************************/

/*****************************************************/
// ЗНМИН2
/*****************************************************/
inline void umin2_handler(unsigned int *p_active_functions, unsigned int number_group_stp)
{
  _Bool previous_state_po_umin2 = _CHECK_SET_BIT(p_active_functions, RANG_PO_UMIN2) > 0;
  _Bool previous_state_po_ublk_umin2 = _CHECK_SET_BIT(p_active_functions, RANG_PO_UBLK_UMIN2) > 0;
  _Bool previous_state_po_iblk_umin2 = _CHECK_SET_BIT(p_active_functions, RANG_PO_IBLK_UMIN2) > 0;
  
  unsigned int setpoint1 = previous_state_po_umin2 ?
          current_settings_prt.setpoint_Umin2[number_group_stp] * U_DOWN / 100 :
          current_settings_prt.setpoint_Umin2[number_group_stp];
  
  _Bool Uab_is_smaller_than_Umin2 = measurement[IM_UAB] <= setpoint1;
  _Bool Ubc_is_smaller_than_Umin2 = measurement[IM_UBC] <= setpoint1;
  _Bool Uca_is_smaller_than_Umin2 = measurement[IM_UCA] <= setpoint1;
  
  _Bool Ua_is_smaller_than_Umin2 = measurement[IM_UA] <= setpoint1;
  _Bool Ub_is_smaller_than_Umin2 = measurement[IM_UB] <= setpoint1;
  _Bool Uc_is_smaller_than_Umin2 = measurement[IM_UC] <= setpoint1;
  
  unsigned int setpoint2 = previous_state_po_ublk_umin2 ?
          KOEF_MIN_UMIN * U_DOWN_1 / 100 : KOEF_MIN_UMIN;
          
  _Bool Ua_or_Ub_or_Uc_is_smaller_than_250mV = (measurement[IM_UA] <= setpoint2) || (measurement[IM_UB] <= setpoint2) || (measurement[IM_UC] <= setpoint2);
  _Bool Uab_or_Ubc_or_Uca_is_smaller_than_250mV = (measurement[IM_UAB] <= setpoint2) || (measurement[IM_UBC] <= setpoint2) || (measurement[IM_UCA] <= setpoint2);
  
  unsigned int setpoint3 = previous_state_po_iblk_umin2 ?
          current_settings_prt.setpoint_Umin2_Iblk[number_group_stp] * KOEF_POVERNENNJA_GENERAL_DOWN / 100 :
          current_settings_prt.setpoint_Umin2_Iblk[number_group_stp];
  
  _Bool Ia_and_Ic_is_smaller_than_Iust = (measurement[IM_IA] <= setpoint3) &&
                                         (measurement[IM_IB] <= setpoint3) &&
                                         (measurement[IM_IC] <= setpoint3);
  //М
  unsigned int tmp_value = (((current_settings_prt.control_extra_settings_1 & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE)) == 0) && ((current_settings_prt.control_transformator & MASKA_FOR_BIT(INDEX_ML_CTR_TRANSFORMATOR_PHASE_LINE)) == 0)) << 0;
//  tmp_value |= ((current_settings_prt.control_Umin & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE)) != 0)                                 << 1;
//  _INVERTOR(tmp_value, 1, tmp_value, 1);
  tmp_value |= ((current_settings_prt.control_Umin & CTR_UMIN2) != 0)                                                            << 2;
  tmp_value |= ((current_settings_prt.control_Umin & CTR_PO_UMIN2_OR_AND) != 0)                                                  << 3;
  tmp_value |= ((current_settings_prt.control_Umin & CTR_UMIN2_UBLK) != 0)                                                       << 4;
  tmp_value |= ((current_settings_prt.control_Umin & CTR_UMIN2_IBLK) != 0)                                                       << 5;
  
  //ДВ
  tmp_value |= (_CHECK_SET_BIT(p_active_functions, RANG_BLOCK_UMIN2) != 0)                                    << 6;
  _INVERTOR(tmp_value, 6, tmp_value, 6);
  tmp_value |= (_CHECK_SET_BIT(p_active_functions, RANG_START_UMIN2) != 0)                                    << 7;
  
//  _AND2(tmp_value, 0, tmp_value, 1, tmp_value, 8);
  
  if (_GET_STATE(tmp_value, 0))
  {
    //Фазные
    _AND4(Ua_is_smaller_than_Umin2, 0, Ub_is_smaller_than_Umin2, 0, Uc_is_smaller_than_Umin2, 0, tmp_value, 3, tmp_value, 9);
    _OR3(Ua_is_smaller_than_Umin2, 0, Ub_is_smaller_than_Umin2, 0, Uc_is_smaller_than_Umin2, 0, tmp_value, 10);
    _INVERTOR(tmp_value, 3, tmp_value, 3);
    _AND2(tmp_value, 10, tmp_value, 3, tmp_value, 11);
    _AND2(tmp_value, 5, Ia_and_Ic_is_smaller_than_Iust, 0, tmp_value, 12);
    _INVERTOR(tmp_value, 5, tmp_value, 5);
    _AND3(Ua_or_Ub_or_Uc_is_smaller_than_250mV, 0, tmp_value, 4, tmp_value, 5, tmp_value, 13);
    
    //ПО Uблк. Umin2
    if (Ua_or_Ub_or_Uc_is_smaller_than_250mV)
      _SET_BIT(p_active_functions, RANG_PO_UBLK_UMIN2);
    else
      _CLEAR_BIT(p_active_functions, RANG_PO_UBLK_UMIN2);
    
  } 
  else 
  {
    //Линейные
    _AND4(Uab_is_smaller_than_Umin2, 0, Ubc_is_smaller_than_Umin2, 0, Uca_is_smaller_than_Umin2, 0, tmp_value, 3, tmp_value, 9);
    _OR3(Uab_is_smaller_than_Umin2, 0, Ubc_is_smaller_than_Umin2, 0, Uca_is_smaller_than_Umin2, 0, tmp_value, 10);
    _INVERTOR(tmp_value, 3, tmp_value, 3);
    _AND2(tmp_value, 10, tmp_value, 3, tmp_value, 11);
    _AND2(tmp_value, 5, Ia_and_Ic_is_smaller_than_Iust, 0, tmp_value, 12);
    _INVERTOR(tmp_value, 5, tmp_value, 5);
    _AND3(Uab_or_Ubc_or_Uca_is_smaller_than_250mV, 0, tmp_value, 4, tmp_value, 5, tmp_value, 13);
    
    //ПО Uблк. Umin2
    if (Uab_or_Ubc_or_Uca_is_smaller_than_250mV)
      _SET_BIT(p_active_functions, RANG_PO_UBLK_UMIN2);
    else
      _CLEAR_BIT(p_active_functions, RANG_PO_UBLK_UMIN2);
  }
  _INVERTOR(tmp_value, 12, tmp_value, 12);
  _INVERTOR(tmp_value, 13, tmp_value, 13);
  _OR3(tmp_value, 7, tmp_value, 9, tmp_value, 11, tmp_value, 14);
  
  _AND5(tmp_value, 6, tmp_value, 2, tmp_value, 14, tmp_value, 13, tmp_value, 12, tmp_value, 15);
  
  //ПО Iблк. Umin2
  if (Ia_and_Ic_is_smaller_than_Iust)
    _SET_BIT(p_active_functions, RANG_PO_IBLK_UMIN2);
  else
    _CLEAR_BIT(p_active_functions, RANG_PO_IBLK_UMIN2);
  
  //ПО Umin2
  if (_GET_STATE(tmp_value, 15))
    _SET_BIT(p_active_functions, RANG_PO_UMIN2);
  else
    _CLEAR_BIT(p_active_functions, RANG_PO_UMIN2);
  
  _TIMER_T_0(INDEX_TIMER_UMIN2, current_settings_prt.timeout_Umin2[number_group_stp], tmp_value, 15, tmp_value, 16);
  
  //Сраб. Umin2
  if (_GET_STATE(tmp_value, 16))
    _SET_BIT(p_active_functions, RANG_UMIN2);
  else
    _CLEAR_BIT(p_active_functions, RANG_UMIN2);
}
/*****************************************************/

/*****************************************************/
// МТЗ04
/*****************************************************/
inline void ocp04_handler(unsigned int *p_active_functions, unsigned int number_group_stp)
{
	unsigned int const I04 = measurement[IM_I04];
	
	static int const ocp04_signals[NUMBER_LEVEL_OCP04][OCP04_SIGNALS_LENGTH] = 
	{
  	{
    	RANG_BLOCK_MTZ04_1, 
    	-1,
    	RANG_PO_MTZ04_1,
    	RANG_MTZ04_1
  	},
  	{
    	RANG_BLOCK_MTZ04_2, 
    	RANG_BLOCK_USK_MTZ04_2,
    	RANG_PO_MTZ04_2,
    	RANG_MTZ04_2
  	}
	};
	static int const ocp04_tmr_index[NUMBER_LEVEL_OCP04][NUMBER_LEVEL_OCP04_TMR_CONST] = 
	{
  	{
    	INDEX_TIMER_MTZ04_1,
			-1,
			-1,
			-1,
			-1,
			-1
  	},
  	{
    	INDEX_TIMER_MTZ04_2,

			INDEX_TIMER_MTZ04_2_DEPENDENT_A,
  		INDEX_TIMER_MTZ04_2_DEPENDENT_B,
  		INDEX_TIMER_MTZ04_2_DEPENDENT_C,
	
			INDEX_TIMER_MTZ04_2_PR,
			INDEX_TIMER_MTZ04_2_VVID_PR,
  	}
	};

	static int const * const p_type_ocp04[NUMBER_LEVEL_OCP04] = 
	{
		NULL,
  	&current_settings_prt.type_mtz04_2
	};
	static int const ocp04_ctr_setting[NUMBER_LEVEL_OCP04][MTZ04_CONST_MENU_SETTINGS_LENGTH] = 
	{
  	{
    	CTR_MTZ04_1,
			-1,
			-1
  	},
  	{
    	CTR_MTZ04_2,
			CTR_MTZ04_2_PRYSKORENNJA,
			CTR_MTZ04_2_PRYSKORENA
  	}
	};
	
	static unsigned int const * const setpoint_ocp04[NUMBER_LEVEL_OCP04] = 
	{
  	current_settings_prt.setpoint_mtz04_1,
  	current_settings_prt.setpoint_mtz04_2
	};

	static int const * const timeout_ocp04[NUMBER_LEVEL_OCP04] =
	{
  	current_settings_prt.timeout_mtz04_1,
  	current_settings_prt.timeout_mtz04_2
	};
	static int const * const timeout_ocp04_vvid_pr[NUMBER_LEVEL_OCP04] =
	{
  	NULL,
  	current_settings_prt.timeout_mtz04_2_vvid_pr
	};
	static int const * const timeout_ocp04_pr[NUMBER_LEVEL_OCP04] =
	{
  	NULL,
  	current_settings_prt.timeout_mtz04_2_pr
	};

	enum _ext_par 
	{
		DEP = 0,
		ACC,
		
		NUM_EXT_PAR
	};
	
	static unsigned int const ocp04_ext[NUMBER_LEVEL_OCP04][NUM_EXT_PAR] = 
	{
		{0, 0},
		{1, 1}
	};
	
	if (_CHECK_SET_BIT(p_active_functions, RANG_STATE_VV) != 0) _SET_STATE(ocp04_general_bits, 0);
	else _CLEAR_STATE(ocp04_general_bits, 0);
	
	unsigned int const control_ocp04 = current_settings_prt.control_mtz04;
	uint32_t static_ocp04_dep_tmp_bits = 0; //цю змінну спочатку було об'явлено, як глобальну, але потім функціонал змінився і я її переніс сюди, а логіку її роботи не переробляв
	for (size_t i = 0; i != NUMBER_LEVEL_OCP04; ++i)
	{
		uint32_t l_ocp04_0 = 0;
		uint32_t dep_ocp04_0 = 0;
		uint32_t acc_ocp04_0 = 0;
				
		int const type_ocp04 = (p_type_ocp04[i] != NULL) ? *(p_type_ocp04[i]) : TYPE_MTZ_SIMPLE;
		if (type_ocp04 == TYPE_MTZ_SIMPLE) _SET_STATE(l_ocp04_0, 0);
		else if (ocp04_ext[i][DEP] != 0)
		{
			if (type_ocp04 == TYPE_MTZ_DEPENDENT_A     )  _SET_STATE(dep_ocp04_0, 0);
			else if (type_ocp04 == TYPE_MTZ_DEPENDENT_B)  _SET_STATE(dep_ocp04_0, 1);
			else if (type_ocp04 == TYPE_MTZ_DEPENDENT_C)  _SET_STATE(dep_ocp04_0, 2);
		}
		
		//Проста МСЗ
    unsigned int const pickup_ocp04 = (_CHECK_SET_BIT(p_active_functions, ocp04_signals[i][RANG_PO_MTZ04]) != 0) ?
            setpoint_ocp04[i][number_group_stp] * U_UP / 100 :
            setpoint_ocp04[i][number_group_stp];
	  l_ocp04_0 |= (I04 >= pickup_ocp04) << 1;
		l_ocp04_0 |= ((control_ocp04 & ocp04_ctr_setting[i][CTR_MTZ04]) != 0) << 2;
		l_ocp04_0 |= (_CHECK_SET_BIT(p_active_functions, ocp04_signals[i][RANG_BLOCK_MTZ04]) == 0) << 3;
		_AND3(l_ocp04_0, 1, l_ocp04_0, 2, l_ocp04_0, 3, l_ocp04_0, 4);
		
		//ПО МСЗх
		if (_GET_STATE(l_ocp04_0, 4))
      _SET_BIT(p_active_functions, ocp04_signals[i][RANG_PO_MTZ04]);
    else
      _CLEAR_BIT(p_active_functions, ocp04_signals[i][RANG_PO_MTZ04]);
		
		//Функціонал, який відповідає за прискорення
		if(ocp04_ext[i][ACC] != 0)
		{
			acc_ocp04_0 |= (_CHECK_SET_BIT(p_active_functions, ocp04_signals[i][RANG_BLOCK_USK_MTZ04]) == 0) << 0;
			
			acc_ocp04_0 |= ((control_ocp04 & ocp04_ctr_setting[i][CTR_MTZ04_PRYSKORENA]) != 0) << 1;
			acc_ocp04_0 |= ((control_ocp04 & ocp04_ctr_setting[i][CTR_MTZ04_PRYSKORENNJA]) != 0) << 2;

			_AND2(ocp04_general_bits, 0, acc_ocp04_0, 2, acc_ocp04_0, 3);

			_TIMER_IMPULSE(ocp04_tmr_index[i][INDEX_TIMER_MTZ04_VVID_PR], timeout_ocp04_vvid_pr[i][number_group_stp], ocp04_general_bits, (1 + i), acc_ocp04_0, 3, acc_ocp04_0, 4);
			_OR2(acc_ocp04_0, 1, acc_ocp04_0, 4, acc_ocp04_0, 5);
			_AND2(acc_ocp04_0, 0, acc_ocp04_0, 5, acc_ocp04_0, 6);
			_INVERTOR(acc_ocp04_0, 6, acc_ocp04_0, 7);

			//МСЗ проста або струмозалежна
			uint32_t ocp04_tmp_bits;
//			uint32_t ocp04_dep_tmp_bits;
			do
			{
				ocp04_tmp_bits     = static_ocp04_tmp_bits     & (/*(1u << (2*i)) | */(1u << (2*i + 1)));
//				ocp04_dep_tmp_bits = static_ocp04_dep_tmp_bits & ((1u << (NUMBER_LEVEL_OCP04*(TYPE_MTZ_DEPENDENT_C - TYPE_MTZ_DEPENDENT_A + 1)) - 1));
				
				//_OR2(acc_ocp04_0, 7, static_ocp04_tmp_bits, (2*i), acc_ocp04_0, 8);
				/*
				_OR4_INVERTOR(
											static_ocp04_tmp_bits, (2*i), 
											static_ocp04_dep_tmp_bits, (NUMBER_LEVEL_OCP04*(TYPE_MTZ_DEPENDENT_A - TYPE_MTZ_DEPENDENT_A) + i),
											static_ocp04_dep_tmp_bits, (NUMBER_LEVEL_OCP04*(TYPE_MTZ_DEPENDENT_B - TYPE_MTZ_DEPENDENT_A) + i),
											static_ocp04_dep_tmp_bits, (NUMBER_LEVEL_OCP04*(TYPE_MTZ_DEPENDENT_C - TYPE_MTZ_DEPENDENT_A) + i),
											acc_ocp04_0, 9
										 );
				*/
				
				_OR2(acc_ocp04_0, 6, static_ocp04_tmp_bits, (2*i + 1), acc_ocp04_0, 10);
				_INVERTOR(static_ocp04_tmp_bits, (2*i + 1), acc_ocp04_0, 11);
				
				_AND4(l_ocp04_0, 0, acc_ocp04_0, 7/*8*/, acc_ocp04_0, 11, l_ocp04_0, 4, static_ocp04_tmp_bits, (2*i    ));
				_AND2/*3*/(/*acc_ocp04_0, 9, */acc_ocp04_0, 10, l_ocp04_0, 4, static_ocp04_tmp_bits, (2*i + 1));
				
				if(ocp04_ext[i][DEP] != 0)
				{
					/*
					_OR2(acc_ocp04_0, 7, static_ocp04_dep_tmp_bits, (NUMBER_LEVEL_OCP04*(TYPE_MTZ_DEPENDENT_A - TYPE_MTZ_DEPENDENT_A) + i), dep_ocp04_0, 3);
					_OR2(acc_ocp04_0, 7, static_ocp04_dep_tmp_bits, (NUMBER_LEVEL_OCP04*(TYPE_MTZ_DEPENDENT_B - TYPE_MTZ_DEPENDENT_A) + i), dep_ocp04_0, 4);
					_OR2(acc_ocp04_0, 7, static_ocp04_dep_tmp_bits, (NUMBER_LEVEL_OCP04*(TYPE_MTZ_DEPENDENT_C - TYPE_MTZ_DEPENDENT_A) + i), dep_ocp04_0, 5);
					*/

					_AND4(acc_ocp04_0, 7, /*dep_ocp04_0, 3, */acc_ocp04_0, 11, dep_ocp04_0, 0, l_ocp04_0, 4, static_ocp04_dep_tmp_bits, (NUMBER_LEVEL_OCP04*(TYPE_MTZ_DEPENDENT_A     - TYPE_MTZ_DEPENDENT_A) + i));
					_AND4(acc_ocp04_0, 7, /*dep_ocp04_0, 4, */acc_ocp04_0, 11, dep_ocp04_0, 1, l_ocp04_0, 4, static_ocp04_dep_tmp_bits, (NUMBER_LEVEL_OCP04*(TYPE_MTZ_DEPENDENT_B     - TYPE_MTZ_DEPENDENT_A) + i));
					_AND4(acc_ocp04_0, 7, /*dep_ocp04_0, 5, */acc_ocp04_0, 11, dep_ocp04_0, 2, l_ocp04_0, 4, static_ocp04_dep_tmp_bits, (NUMBER_LEVEL_OCP04*(TYPE_MTZ_DEPENDENT_C     - TYPE_MTZ_DEPENDENT_A) + i));
				}
			}
			while(
						((static_ocp04_tmp_bits     & (/*(1u << (2*i)) | */(1u << (2*i + 1)))                                                 ) != ocp04_tmp_bits    )/* ||
						((static_ocp04_dep_tmp_bits & ((1u << (NUMBER_LEVEL_OCP04*(TYPE_MTZ_DEPENDENT_C - TYPE_MTZ_DEPENDENT_A + 1)) - 1))) != ocp04_dep_tmp_bits)*/
					 );
			_TIMER_T_0(ocp04_tmr_index[i][INDEX_TIMER_MTZ04   ], timeout_ocp04[i][number_group_stp]   , static_ocp04_tmp_bits, (2*i    ), l_ocp04_0  ,  5);
			_TIMER_T_0(ocp04_tmr_index[i][INDEX_TIMER_MTZ04_PR], timeout_ocp04_pr[i][number_group_stp], static_ocp04_tmp_bits, (2*i + 1), acc_ocp04_0, 12);
		}
		else
		{
			if(ocp04_ext[i][DEP] != 0)
			{
				_AND2(dep_ocp04_0, 0, l_ocp04_0, 4, static_ocp04_dep_tmp_bits, (NUMBER_LEVEL_OCP04*(TYPE_MTZ_DEPENDENT_A     - TYPE_MTZ_DEPENDENT_A) + i));
				_AND2(dep_ocp04_0, 1, l_ocp04_0, 4, static_ocp04_dep_tmp_bits, (NUMBER_LEVEL_OCP04*(TYPE_MTZ_DEPENDENT_B     - TYPE_MTZ_DEPENDENT_A) + i));
				_AND2(dep_ocp04_0, 2, l_ocp04_0, 4, static_ocp04_dep_tmp_bits, (NUMBER_LEVEL_OCP04*(TYPE_MTZ_DEPENDENT_C     - TYPE_MTZ_DEPENDENT_A) + i));
			}
			
			_TIMER_T_0(ocp04_tmr_index[i][INDEX_TIMER_MTZ04], timeout_ocp04[i][number_group_stp], l_ocp04_0, 4, l_ocp04_0, 5);
		}
		
		if(ocp04_ext[i][DEP] != 0)
		{
			for(int type_ocp04_tmp = TYPE_MTZ_DEPENDENT_A; type_ocp04_tmp <= TYPE_MTZ_DEPENDENT_C; ++type_ocp04_tmp)
			{
				int const shift = type_ocp04_tmp - TYPE_MTZ_DEPENDENT_A;
				if (_GET_STATE(static_ocp04_dep_tmp_bits, (NUMBER_LEVEL_OCP04 * shift + i)))
				{
					unsigned int const pickup  = setpoint_ocp04[i][number_group_stp];
					int const timeout = timeout_ocp04[i][number_group_stp];
					
					_TIMER_T_0_LOCK(ocp04_tmr_index[i][INDEX_TIMER_MTZ04_DEPENDENT_A  + shift], timeout_dependent_general(I04, pickup, timeout, TIMEOUT_MTZ04_2_MAX, type_ocp04_tmp), static_ocp04_dep_tmp_bits, (NUMBER_LEVEL_OCP04 * shift + i), static_ocp04_dep_rez_bits, (NUMBER_LEVEL_OCP04 * shift + i));
				}
				else
				{
					global_timers[ocp04_tmr_index[i][INDEX_TIMER_MTZ04_DEPENDENT_A  + shift]] = -1;
					_CLEAR_STATE(static_ocp04_dep_rez_bits, (NUMBER_LEVEL_OCP04 * shift + i));
				}
			}
		}
		
		if (
				(
				 _GET_STATE(l_ocp04_0, 5)
				)
				||
				(
				 (ocp04_ext[i][DEP] != 0) &&
				 (
					_GET_STATE(static_ocp04_dep_rez_bits, (NUMBER_LEVEL_OCP04 * (TYPE_MTZ_DEPENDENT_A     - TYPE_MTZ_DEPENDENT_A) + i)) ||
					_GET_STATE(static_ocp04_dep_rez_bits, (NUMBER_LEVEL_OCP04 * (TYPE_MTZ_DEPENDENT_B     - TYPE_MTZ_DEPENDENT_A) + i)) ||
					_GET_STATE(static_ocp04_dep_rez_bits, (NUMBER_LEVEL_OCP04 * (TYPE_MTZ_DEPENDENT_C     - TYPE_MTZ_DEPENDENT_A) + i))
				 )	
				) 
				||
				(
				 (ocp04_ext[i][ACC] != 0) &&
				 (
			  	_GET_STATE(acc_ocp04_0, 12)
				 )	
				) 
			 )	
		{
      _SET_BIT(p_active_functions, ocp04_signals[i][RANG_MTZ04]);
		}
		else
		{
      _CLEAR_BIT(p_active_functions, ocp04_signals[i][RANG_MTZ04]);
		}
	}
}

/*****************************************************/
// ЗНМАКС1
/*****************************************************/
inline void umax1_handler(unsigned int *p_active_functions, unsigned int number_group_stp)
{
  _Bool previous_state_po_umax1 = _CHECK_SET_BIT(p_active_functions, RANG_PO_UMAX1) > 0;
  
  unsigned int setpoint1 = previous_state_po_umax1 ?
          current_settings_prt.setpoint_Umax1[number_group_stp] * current_settings_prt.setpoint_kp_Umax[number_group_stp] / 100 :
          current_settings_prt.setpoint_Umax1[number_group_stp];
  
  _Bool Uab_is_larger_than_Umax1 = measurement[IM_UAB] >= setpoint1;
  _Bool Ubc_is_larger_than_Umax1 = measurement[IM_UBC] >= setpoint1;
  _Bool Uca_is_larger_than_Umax1 = measurement[IM_UCA] >= setpoint1;
  
  _Bool Ua_is_larger_than_Umax1 = measurement[IM_UA] >= setpoint1;
  _Bool Ub_is_larger_than_Umax1 = measurement[IM_UB] >= setpoint1;
  _Bool Uc_is_larger_than_Umax1 = measurement[IM_UC] >= setpoint1;
  
  //М
  unsigned int tmp_value = (((current_settings_prt.control_extra_settings_1 & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE)) == 0) && ((current_settings_prt.control_transformator & MASKA_FOR_BIT(INDEX_ML_CTR_TRANSFORMATOR_PHASE_LINE)) == 0)) << 0;
//  tmp_value |= ((current_settings_prt.control_Umax & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE)) != 0)                                 << 1;
//  _INVERTOR(tmp_value, 1, tmp_value, 1);
  tmp_value |= ((current_settings_prt.control_Umax & CTR_PO_UMAX1_OR_AND) != 0)                                                  << 2;
  tmp_value |= ((current_settings_prt.control_Umax & CTR_UMAX1) != 0)                                                            << 3;
  
  //ДВ
  tmp_value |= (_CHECK_SET_BIT(p_active_functions, RANG_BLOCK_UMAX1) != 0)                                    << 4;
  
  if (_GET_STATE(tmp_value, 0)) {
    _AND4(Ua_is_larger_than_Umax1, 0, Ub_is_larger_than_Umax1, 0, Uc_is_larger_than_Umax1, 0, tmp_value, 2, tmp_value, 5);
    _OR3(Ua_is_larger_than_Umax1, 0, Ub_is_larger_than_Umax1, 0, Uc_is_larger_than_Umax1, 0, tmp_value, 6);
  } else {
    _AND4(Uab_is_larger_than_Umax1, 0, Ubc_is_larger_than_Umax1, 0, Uca_is_larger_than_Umax1, 0, tmp_value, 2, tmp_value, 5);
    _OR3(Uab_is_larger_than_Umax1, 0, Ubc_is_larger_than_Umax1, 0, Uca_is_larger_than_Umax1, 0, tmp_value, 6);
  }
  
  _INVERTOR(tmp_value, 2, tmp_value, 2);
  _AND2(tmp_value, 6, tmp_value, 2, tmp_value, 7);
  _OR2(tmp_value, 5, tmp_value, 7, tmp_value, 8);
  _INVERTOR(tmp_value, 4, tmp_value, 4);
  _AND3(tmp_value, 8, tmp_value, 4, tmp_value, 3, tmp_value, 9);
  
  //ПО Umax1
  if (_GET_STATE(tmp_value, 9))
    _SET_BIT(p_active_functions, RANG_PO_UMAX1);
  else
    _CLEAR_BIT(p_active_functions, RANG_PO_UMAX1);
  
  _TIMER_T_0(INDEX_TIMER_UMAX1, current_settings_prt.timeout_Umax1[number_group_stp], tmp_value, 9, tmp_value, 10);
  
  //Сраб. Umax1
  if (_GET_STATE(tmp_value, 10))
    _SET_BIT(p_active_functions, RANG_UMAX1);
  else
    _CLEAR_BIT(p_active_functions, RANG_UMAX1);
}
/*****************************************************/

/*****************************************************/
// ЗНМАКС2
/*****************************************************/
inline void umax2_handler(unsigned int *p_active_functions, unsigned int number_group_stp)
{
  _Bool previous_state_po_umax2 = _CHECK_SET_BIT(p_active_functions, RANG_PO_UMAX2) > 0;
  
  unsigned int setpoint1 = previous_state_po_umax2 ?
          current_settings_prt.setpoint_Umax2[number_group_stp] * current_settings_prt.setpoint_kp_Umax[number_group_stp] / 100 :
          current_settings_prt.setpoint_Umax2[number_group_stp];
  
  _Bool Uab_is_larger_than_Umax2 = measurement[IM_UAB] >= setpoint1;
  _Bool Ubc_is_larger_than_Umax2 = measurement[IM_UBC] >= setpoint1;
  _Bool Uca_is_larger_than_Umax2 = measurement[IM_UCA] >= setpoint1;
  
  _Bool Ua_is_larger_than_Umax2 = measurement[IM_UA] >= setpoint1;
  _Bool Ub_is_larger_than_Umax2 = measurement[IM_UB] >= setpoint1;
  _Bool Uc_is_larger_than_Umax2 = measurement[IM_UC] >= setpoint1;
  
  //М
  unsigned int tmp_value = (((current_settings_prt.control_extra_settings_1 & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE)) == 0) && ((current_settings_prt.control_transformator & MASKA_FOR_BIT(INDEX_ML_CTR_TRANSFORMATOR_PHASE_LINE)) == 0)) << 0;
//  tmp_value |= ((current_settings_prt.control_Umax & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE)) != 0)                                 << 1;
//  _INVERTOR(tmp_value, 1, tmp_value, 1);
  tmp_value |= ((current_settings_prt.control_Umax & CTR_PO_UMAX2_OR_AND) != 0)                                                  << 2;
  tmp_value |= ((current_settings_prt.control_Umax & CTR_UMAX2) != 0)                                                            << 3;
  
  //ДВ
  tmp_value |= (_CHECK_SET_BIT(p_active_functions, RANG_BLOCK_UMAX2) != 0)                                    << 4;
  
  if (_GET_STATE(tmp_value, 0)) {
    _AND4(Ua_is_larger_than_Umax2, 0, Ub_is_larger_than_Umax2, 0, Uc_is_larger_than_Umax2, 0, tmp_value, 2, tmp_value, 5);
    _OR3(Ua_is_larger_than_Umax2, 0, Ub_is_larger_than_Umax2, 0, Uc_is_larger_than_Umax2, 0, tmp_value, 6);
  } else {
    _AND4(Uab_is_larger_than_Umax2, 0, Ubc_is_larger_than_Umax2, 0, Uca_is_larger_than_Umax2, 0, tmp_value, 2, tmp_value, 5);
    _OR3(Uab_is_larger_than_Umax2, 0, Ubc_is_larger_than_Umax2, 0, Uca_is_larger_than_Umax2, 0, tmp_value, 6);
  }
  
  _INVERTOR(tmp_value, 2, tmp_value, 2);
  _AND2(tmp_value, 6, tmp_value, 2, tmp_value, 7);
  _OR2(tmp_value, 5, tmp_value, 7, tmp_value, 8);
  _INVERTOR(tmp_value, 4, tmp_value, 4);
  _AND3(tmp_value, 8, tmp_value, 4, tmp_value, 3, tmp_value, 9);
  
  //ПО Umax1
  if (_GET_STATE(tmp_value, 9))
    _SET_BIT(p_active_functions, RANG_PO_UMAX2);
  else
    _CLEAR_BIT(p_active_functions, RANG_PO_UMAX2);
  
  _TIMER_T_0(INDEX_TIMER_UMAX2, current_settings_prt.timeout_Umax2[number_group_stp], tmp_value, 9, tmp_value, 10);
  
  //Сраб. Umax1
  if (_GET_STATE(tmp_value, 10))
    _SET_BIT(p_active_functions, RANG_UMAX2);
  else
    _CLEAR_BIT(p_active_functions, RANG_UMAX2);
}
/*****************************************************/

/*****************************************************/
// АЧР ЧАПВ
/*****************************************************/
inline void achr_chapv_handler(unsigned int *p_active_functions, unsigned int number_group_stp)
{
//RANG_ACHR_CHAPV_VID_DV,
/*
  //пред сост ПО АЧР
  _Bool previous_state_po_f1_achr = _CHECK_SET_BIT(p_active_functions, RANG_PO_ACHR1);
  //пред сост ПО ЧАПВ
  _Bool previous_state_po_f1_chapv = _CHECK_SET_BIT(p_active_functions, RANG_PO_CHAPV1);
 // 
раздвоение
*/
  //пред сост ПО АЧР1
  _Bool previous_state_po_f1_achr1  = _CHECK_SET_BIT(p_active_functions, RANG_PO_ACHR1);
  //пред сост ПО ЧАПВ1
  _Bool previous_state_po_f1_chapv1 = _CHECK_SET_BIT(p_active_functions, RANG_PO_CHAPV1);
  //пред сост ПО АЧР2
  _Bool previous_state_po_f1_achr2  = _CHECK_SET_BIT(p_active_functions, RANG_PO_ACHR2);
  //пред сост ПО ЧАПВ2
  _Bool previous_state_po_f1_chapv2 = _CHECK_SET_BIT(p_active_functions, RANG_PO_CHAPV2);
//----------
/*
неизменно
*/

  unsigned int setpoint1 = previous_state_po_achr_chapv_uaf1 ?
          current_settings_prt.setpoint_achr_chapv_uf[number_group_stp] * U_UP / 100 :
          current_settings_prt.setpoint_achr_chapv_uf[number_group_stp];
          
  unsigned int setpoint2 = previous_state_po_achr_chapv_ubf1 ?
          current_settings_prt.setpoint_achr_chapv_uf[number_group_stp] * U_UP / 100 :
          current_settings_prt.setpoint_achr_chapv_uf[number_group_stp];
  unsigned int setpoint3 = previous_state_po_achr_chapv_ucf1 ?
          current_settings_prt.setpoint_achr_chapv_uf[number_group_stp] * U_UP / 100 :
          current_settings_prt.setpoint_achr_chapv_uf[number_group_stp];
  
  //Линейные
  _Bool Uab_is_larger_than_UF1 = measurement[IM_UAB] >= setpoint1;
  _Bool Ubc_is_larger_than_UF1 = measurement[IM_UBC] >= setpoint2;
  _Bool Uca_is_larger_than_UF1 = measurement[IM_UCA] >= setpoint3;
  
  //Фазные
//? _Bool Ua_is_larger_than_UF1 = measurement[IM_UA] >= setpoint1;
//? _Bool Ub_is_larger_than_UF1 = measurement[IM_UB] >= setpoint2;
//? _Bool Uc_is_larger_than_UF1 = measurement[IM_UC] >= setpoint3;
  
  /*----------------Выбор фазные/линейные-------------------------------------*/
  
//?  _Bool UF1_phase = (((current_settings_prt.control_extra_settings_1 & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE)) == 0) && ((current_settings_prt.control_transformator & MASKA_FOR_BIT(INDEX_ML_CTR_TRANSFORMATOR_PHASE_LINE)) == 0));
  /*--------------------------------------------------------------------------*/
  
  _Bool timer1 = 0;
  unsigned int UF1_is_larger_than_U_setpoint_F1 = 0;
//  _Bool UF1_is_smaller_than_U_setpoint_F1 = 0;
  //ПО ФАЗНЫЕ-ЛИНЕЙНЫЕ 
//..if (UF1_phase)
//..{
//..  //ПО UAF1
//..  previous_state_po_achr_chapv_uaf1 = Ua_is_larger_than_UF1;
//..  //ПО UBF1
//..  previous_state_po_achr_chapv_ubf1 = Ub_is_larger_than_UF1;
//..  //ПО UCF1
//..  previous_state_po_achr_chapv_ucf1 = Uc_is_larger_than_UF1;
//..  _AND3(Ua_is_larger_than_UF1, 0, Ub_is_larger_than_UF1, 0, Uc_is_larger_than_UF1, 0, timer1, 0);
//..}
//..else 
  {
    //ПО UAF1
    previous_state_po_achr_chapv_uaf1 = Uab_is_larger_than_UF1;
    //ПО UBF1
    previous_state_po_achr_chapv_ubf1 = Ubc_is_larger_than_UF1;
    //ПО UCF1
    previous_state_po_achr_chapv_ucf1 = Uca_is_larger_than_UF1;
    _AND3(Uab_is_larger_than_UF1, 0, Ubc_is_larger_than_UF1, 0, Uca_is_larger_than_UF1, 0, timer1, 0);
  }
  _TIMER_T_0(INDEX_TIMER_ACHR_CHAPV_100MS_1, TIMEOUT_ACHR_CHAPV_100MS, timer1, 0, UF1_is_larger_than_U_setpoint_F1, 0);
  _INVERTOR(UF1_is_larger_than_U_setpoint_F1, 0, UF1_is_larger_than_U_setpoint_F1, 1);
  //L1
//  UF1_is_smaller_than_U_setpoint_F1 = !UF1_is_larger_than_U_setpoint_F1;
 //L2, 3 
  int F1 = (int) (frequency * 100);
/*
  _Bool po_f1_achr_rab = F1 <= ((int)current_settings_prt.setpoint_achr1_f_rab[number_group_stp]);
  _Bool po_f1_chapv_rab = F1 >= ((int)current_settings_prt.setpoint_chapv1_f_rab[number_group_stp]);
  _Bool po_f1_achr = 0;
раздвоение
*/
  _Bool po_f1_achr1_rab = F1 <= ((int)current_settings_prt.setpoint_achr1_f_rab[number_group_stp]);
  _Bool po_f1_chapv1_rab = F1 >= ((int)current_settings_prt.setpoint_chapv1_f_rab[number_group_stp]);
  _Bool po_f1_achr1 = 0;
  _Bool po_f1_achr2_rab = F1 <= ((int)current_settings_prt.setpoint_achr2_f_rab[number_group_stp]);
  _Bool po_f1_chapv2_rab = F1 >= ((int)current_settings_prt.setpoint_chapv2_f_rab[number_group_stp]);
  _Bool po_f1_achr2 = 0;
//----------

/*  
  //ДВ
  unsigned int tmp_value = (_CHECK_SET_BIT(p_active_functions, RANG_BLOCK_ACHR1) != 0) << 0;
  //L5
  _INVERTOR(tmp_value, 0, tmp_value, 0);
раздвоение
*/
  //ДВ
  unsigned int tmp_value1 = (_CHECK_SET_BIT(p_active_functions, RANG_BLOCK_ACHR1) != 0) << 0;
  //L5.1
  _INVERTOR(tmp_value1, 0, tmp_value1, 0);
  //ДВ
  unsigned int tmp_value2 = (_CHECK_SET_BIT(p_active_functions, RANG_BLOCK_ACHR2) != 0) << 0;
  //L5
  _INVERTOR(tmp_value2, 0, tmp_value2, 0);
//----------

/*  
  //М
  tmp_value |= ((current_settings_prt.control_achr_chapv & CTR_ACHR1) != 0) << 1;
  //L4
  _AND3_INVERTOR(tmp_value, 0, tmp_value, 1, UF1_is_larger_than_U_setpoint_F1, 0, tmp_value, 3);
раздвоение
*/
  //М
  tmp_value1 |= ((current_settings_prt.control_achr_chapv & CTR_ACHR1) != 0) << 1;
  //L4.1
  _AND3_INVERTOR(tmp_value1, 0, tmp_value1, 1, UF1_is_larger_than_U_setpoint_F1, 0, tmp_value1, 3);
  //М
  tmp_value2 |= ((current_settings_prt.control_achr_chapv & CTR_ACHR2) != 0) << 1;
  //L4.2
  _AND3_INVERTOR(tmp_value2, 0, tmp_value2, 1, UF1_is_larger_than_U_setpoint_F1, 0, tmp_value2, 3);
//----------
/*
  //Reset канал L2
  if (!_GET_STATE(tmp_value, 3))
  {
    //ПО АЧР1
    if (previous_state_po_f1_achr) 
    {
      //отпускание АЧР
//  _Bool po_f1_chapv_rab = F1 >= ((int)current_settings_prt.setpoint_chapv1_f_rab[number_group_stp]);
      if (!po_f1_chapv_rab) 
      {
        //после сработки 1 будет держаться до тех пор пока не сработает po_f1_chapv_rab (условие отпускания)
        _SET_BIT(p_active_functions, RANG_PO_ACHR1);
        po_f1_achr = 1;
      }
    }
    else
    {
      //сработка
//  _Bool po_f1_achr_rab = F1 <= ((int)current_settings_prt.setpoint_achr1_f_rab[number_group_stp]);
      
      if (po_f1_achr_rab)
      {
        _SET_BIT(p_active_functions, RANG_PO_ACHR1);
        po_f1_achr = 1;
      }
    }
  }//if
раздвоение
*/
//---1----
  //Reset канал L2
  if (!_GET_STATE(tmp_value1, 3))
  {
    //ПО АЧР1
    if (previous_state_po_f1_achr1) 
    {
      //отпускание АЧР
//  _Bool po_f1_chapv_rab = F1 >= ((int)current_settings_prt.setpoint_chapv1_f_rab[number_group_stp]);
      if (!po_f1_chapv1_rab) 
      {
        //после сработки 1 будет держаться до тех пор пока не сработает po_f1_chapv_rab (условие отпускания)
        _SET_BIT(p_active_functions, RANG_PO_ACHR1);
        po_f1_achr1 = 1;
      }
      else _CLEAR_BIT(p_active_functions, RANG_PO_ACHR1);
    }
    else
    {
      //сработка
//  _Bool po_f1_achr_rab = F1 <= ((int)current_settings_prt.setpoint_achr1_f_rab[number_group_stp]);
      
      if (po_f1_achr1_rab)
      {
        _SET_BIT(p_active_functions, RANG_PO_ACHR1);
        po_f1_achr1 = 1;
      }
      else _CLEAR_BIT(p_active_functions, RANG_PO_ACHR1);
    }
  }//if
  else
  {
    _CLEAR_BIT(p_active_functions, RANG_PO_ACHR1);
  }
//----2----
  //Reset канал L2
  if (!_GET_STATE(tmp_value2, 3))
  {
    //ПО АЧР1
    if (previous_state_po_f1_achr2) 
    {
      //отпускание АЧР
//  _Bool po_f1_chapv_rab = F1 >= ((int)current_settings_prt.setpoint_chapv1_f_rab[number_group_stp]);
      if (!po_f1_chapv2_rab) 
      {
        //после сработки 1 будет держаться до тех пор пока не сработает po_f1_chapv_rab (условие отпускания)
        _SET_BIT(p_active_functions, RANG_PO_ACHR2);
        po_f1_achr2 = 1;
      }
      else _CLEAR_BIT(p_active_functions, RANG_PO_ACHR2);
    }
    else
    {
      //сработка
//  _Bool po_f1_achr_rab = F1 <= ((int)current_settings_prt.setpoint_achr1_f_rab[number_group_stp]);
      
      if (po_f1_achr2_rab)
      {
        _SET_BIT(p_active_functions, RANG_PO_ACHR2);
        po_f1_achr2 = 1;
      }
      else _CLEAR_BIT(p_active_functions, RANG_PO_ACHR2);
    }
  }//if
  else
  {
    _CLEAR_BIT(p_active_functions, RANG_PO_ACHR2);
  }
//--------------  
/*
  _TIMER_T_0(INDEX_TIMER_ACHR1, current_settings_prt.timeout_achr_1[number_group_stp], po_f1_achr, 0, tmp_value, 5);
раздвоение
*/
//---1---
  _TIMER_T_0(INDEX_TIMER_ACHR1, current_settings_prt.timeout_achr_1[number_group_stp], po_f1_achr1, 0, tmp_value1, 5);
//---2---
  _TIMER_T_0(INDEX_TIMER_ACHR2, current_settings_prt.timeout_achr_2[number_group_stp], po_f1_achr2, 0, tmp_value2, 5);
//-------
  
  //Разр ЧАПВ
  if (_GET_STATE(UF1_is_larger_than_U_setpoint_F1, 0))
    _SET_BIT(p_active_functions, RANG_RAZR_CHAPV);
  else
    _CLEAR_BIT(p_active_functions, RANG_RAZR_CHAPV);

/*  
  _Bool tmp1 = 0;
  do 
  {
    //L7
    _OR2(tmp_value, 3, !trigger_CHAPV1, 0, tmp_value, 30);
    
    //ПО ЧАПВ1
    _Bool po_f1_chapv = 0;
    if (previous_state_po_f1_chapv) 
    {
      po_f1_chapv = !po_f1_achr_rab;
    }
    else 
    {
      po_f1_chapv = po_f1_chapv_rab;
    }
    //L3
    _Bool po_f1_chapv_tmp = (!_GET_STATE(tmp_value, 30) && po_f1_chapv);
    if (po_f1_chapv_tmp) 
    {
      _SET_BIT(p_active_functions, RANG_PO_CHAPV1);
    }
    
    _TIMER_T_0(INDEX_TIMER_CHAPV1, current_settings_prt.timeout_chapv_1[number_group_stp], po_f1_chapv_tmp, 0, tmp_value, 6);
    //L8
    _AND2(po_f1_achr, 0, !po_f1_chapv_tmp, 0, tmp_value, 12);
    //L9
    _OR2(tmp_value, 5, tmp_value, 6, tmp_value, 13);

    _D_TRIGGER(_GET_STATE(tmp_value, 12), 0, _GET_STATE(UF1_is_smaller_than_U_setpoint_F1, 0), previous_states_CHAPV1, 0, tmp_value, 13, trigger_CHAPV1, 0);
    if (tmp1 == trigger_CHAPV1) break;
    
    tmp1 = trigger_CHAPV1;
  } 
  while(1);
раздвоение
*/
//----1----
  _Bool tmp1_1 = 0;
  do 
  {
    //L7_1
    _OR2(tmp_value1, 3, !trigger_CHAPV1, 0, tmp_value1, 30);
    
    //ПО ЧАПВ1
    _Bool po_f1_chapv1 = 0;
    if (previous_state_po_f1_chapv1) 
    {
      po_f1_chapv1 = !po_f1_achr1_rab;
    }
    else 
    {
      po_f1_chapv1 = po_f1_chapv1_rab;
    }
    //L3
    _Bool po_f1_chapv1_tmp = (!_GET_STATE(tmp_value1, 30) && po_f1_chapv1);
    if (po_f1_chapv1_tmp) 
      _SET_BIT(p_active_functions, RANG_PO_CHAPV1);
    else
      _CLEAR_BIT(p_active_functions, RANG_PO_CHAPV1);
    
    _TIMER_T_0(INDEX_TIMER_CHAPV1, current_settings_prt.timeout_chapv_1[number_group_stp], po_f1_chapv1_tmp, 0, tmp_value1, 6);
    //L8
    _AND2(po_f1_achr1, 0, !po_f1_chapv1_tmp, 0, tmp_value1, 12);
    //L9
    _OR2(tmp_value1, 5, tmp_value1, 6, tmp_value1, 13);

    _D_TRIGGER(_GET_STATE(tmp_value1, 12), 0, _GET_STATE(UF1_is_larger_than_U_setpoint_F1, 1), previous_states_CHAPV1, 0, tmp_value1, 13, trigger_CHAPV1, 0);
    if (tmp1_1 == trigger_CHAPV1) break;
    
    tmp1_1 = trigger_CHAPV1;
  } 
  while(1);
//----2----
  _Bool tmp1_2 = 0;
  do 
  {
    //L7_2
    _OR2(tmp_value2, 3, !trigger_CHAPV2, 0, tmp_value2, 30);
    
    //ПО ЧАПВ2
    _Bool po_f1_chapv2 = 0;
    if (previous_state_po_f1_chapv2) 
    {
      po_f1_chapv2 = !po_f1_achr2_rab;
    }
    else 
    {
      po_f1_chapv2 = po_f1_chapv2_rab;
    }
    //L3_2
    _Bool po_f1_chapv2_tmp = (!_GET_STATE(tmp_value2, 30) && po_f1_chapv2);
    if (po_f1_chapv2_tmp) 
      _SET_BIT(p_active_functions, RANG_PO_CHAPV2);
    else
      _CLEAR_BIT(p_active_functions, RANG_PO_CHAPV2);
    
    _TIMER_T_0(INDEX_TIMER_CHAPV2, current_settings_prt.timeout_chapv_2[number_group_stp], po_f1_chapv2_tmp, 0, tmp_value2, 6);
    //L8_2
    _AND2(po_f1_achr2, 0, !po_f1_chapv2_tmp, 0, tmp_value2, 12);
    //L9_2
    _OR2(tmp_value2, 5, tmp_value2, 6, tmp_value2, 13);

    _D_TRIGGER(_GET_STATE(tmp_value2, 12), 0, _GET_STATE(UF1_is_larger_than_U_setpoint_F1, 1), previous_states_CHAPV2, 0, tmp_value2, 13, trigger_CHAPV2, 0);
    if (tmp1_2 == trigger_CHAPV2) break;
    
    tmp1_2 = trigger_CHAPV2;
  } 
  while(1);
//---------
/*  
  //АЧР/ЧАПВ
  if (_GET_STATE(trigger_CHAPV1, 0)) 
  {
    _SET_BIT(p_active_functions, RANG_ACHR_CHAPV1);
  }
  
  _Bool chapv_timer_1ms = 0;
  _Bool razr_chapv_inv = 0;
  _TIMER_0_T(INDEX_TIMER_CHAPV_1MS, TIMEOUT_CHAPV_1MS, trigger_CHAPV1, 0, chapv_timer_1ms, 0);
  _INVERTOR(UF1_is_larger_than_U_setpoint_F1, 0, razr_chapv_inv, 0);
  _AND2(chapv_timer_1ms, 0, razr_chapv_inv, 0, tmp_value, 24);
  _TIMER_0_T(INDEX_TIMER_BLOCK_CHAPV_5MS, TIMEOUT_BLOCK_CHAPV_5MS, tmp_value, 24, tmp_value, 25);
раздвоение
*/
//----1----
  //АЧР/ЧАПВ
  if (_GET_STATE(trigger_CHAPV1, 0)) 
    _SET_BIT(p_active_functions, RANG_ACHR_CHAPV1);
  else
    _CLEAR_BIT(p_active_functions, RANG_ACHR_CHAPV1);

//----2----
  //АЧР/ЧАПВ
  if (_GET_STATE(trigger_CHAPV2, 0)) 
    _SET_BIT(p_active_functions, RANG_ACHR_CHAPV2);
  else
    _CLEAR_BIT(p_active_functions, RANG_ACHR_CHAPV2);

  //Блокування ЧАПВ
  unsigned int tmp_value_g = 0;
  _TIMER_0_T(INDEX_TIMER_CHAPV1_2MS, TIMEOUT_CHAPV_2MS, trigger_CHAPV1, 0, tmp_value_g, 0);
  _TIMER_0_T(INDEX_TIMER_CHAPV2_2MS, TIMEOUT_CHAPV_2MS, trigger_CHAPV2, 0, tmp_value_g, 1);
  
  _AND2(tmp_value_g, 0, UF1_is_larger_than_U_setpoint_F1, 1, tmp_value_g, 2);
  _AND2(tmp_value_g, 1, UF1_is_larger_than_U_setpoint_F1, 1, tmp_value_g, 3);
  
  _TIMER_0_T(INDEX_TIMER_BLOCK_CHAPV1_6MS, TIMEOUT_BLOCK_CHAPV_6MS, tmp_value_g, 2, tmp_value_g, 4);
  _TIMER_0_T(INDEX_TIMER_BLOCK_CHAPV2_6MS, TIMEOUT_BLOCK_CHAPV_6MS, tmp_value_g, 3, tmp_value_g, 5);
  
  _OR2(tmp_value_g, 4, tmp_value_g, 5, tmp_value_g, 6);

  if (_GET_STATE(tmp_value_g, 6))
      _SET_BIT(p_active_functions, RANG_VN_BLOCK_CHAPV);
  else
      _CLEAR_BIT(p_active_functions, RANG_VN_BLOCK_CHAPV);
}

/*****************************************************/

/*****************************************************/
// Готовность к ТУ
/*****************************************************/
inline void ready_tu(unsigned int *p_active_functions)
{
  unsigned int tmp_value = (_CHECK_SET_BIT(p_active_functions, RANG_PRYVID_VV) == 0)                  << 0;
  tmp_value |= (_CHECK_SET_BIT(p_active_functions, RANG_VIDKL_VID_ZAKHYSTIV) != 0)                    << 1;
  tmp_value |= (_CHECK_SET_BIT(p_active_functions, RANG_RESET_BLOCK_READY_TU_VID_ZAHYSTIV) != 0)      << 2;
  tmp_value |= (_CHECK_SET_BIT(p_active_functions, RANG_AVAR_DEFECT) == 0)                            << 3;
  tmp_value |= (_CHECK_SET_BIT(p_active_functions, RANG_MISCEVE_DYSTANCIJNE) == 0)                    << 6;
  
  _Bool ctrl_ready_tu = ((current_settings_prt.control_extra_settings_1 & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_READY_TU)) == 0);
  
  if (!previous_states_ready_tu && _GET_STATE(tmp_value, 1)) {
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
// УРОВ
/*****************************************************/
inline void urov_handler(unsigned int *p_active_functions, unsigned int number_group_stp)
{
  /*******************************/
  //Визначаємо максимальний фазовий струм для УРОВ
  /*******************************/
  unsigned int max_faze_current = measurement[IM_IA];
  if (max_faze_current < measurement[IM_IB]) max_faze_current = measurement[IM_IB];
  if (max_faze_current < measurement[IM_IC]) max_faze_current = measurement[IM_IC];
  /*******************************/
  
  /*******************************/
  uint32_t state_UROV = 
		(( current_settings_prt.control_urov & MASKA_FOR_BIT(INDEX_ML_CTRUROV_STATE)) != 0) 
		&&
		(_CHECK_SET_BIT(p_active_functions, RANG_BLOCK_UROV) == 0);
	
  uint32_t start_from_UP = false;
  if (
      (state_UROV != 0) &&
      ((current_settings_prt.control_urov & ((MASKA_FOR_BIT(NUMBER_UP) - 1) << INDEX_ML_CTRUROV_STARTED_FROM_UP1)) != 0)
     )   
  {
    //Налаштовано запуск ПРЗЗ від УЗ
    for (size_t n_UP = 0; n_UP < NUMBER_UP; n_UP++)
    {
      if ( 
          ((current_settings_prt.control_urov & MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_UP1 + n_UP)) != 0) && 
          (_CHECK_SET_BIT(p_active_functions, (RANG_UP1 + 3*n_UP)      ) != 0)
         )
      {
        start_from_UP = true;
      }
    }
  }
  
  if(
     (state_UROV != 0) &&
     (
      (start_from_UP == true) ||
      (_CHECK_SET_BIT(p_active_functions, RANG_PUSK_UROV_VID_DV) != 0) ||
      ( ((current_settings_prt.control_urov & MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_MTZ1)   ) != 0) && (_CHECK_SET_BIT(p_active_functions, RANG_MTZ1             ) != 0)) ||
      ( ((current_settings_prt.control_urov & MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_MTZ2)   ) != 0) && (_CHECK_SET_BIT(p_active_functions, RANG_MTZ2             ) != 0)) ||
      ( ((current_settings_prt.control_urov & MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_MTZ3)   ) != 0) && (_CHECK_SET_BIT(p_active_functions, RANG_MTZ3             ) != 0)) ||
      ( ((current_settings_prt.control_urov & MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_MTZ4)   ) != 0) && (_CHECK_SET_BIT(p_active_functions, RANG_MTZ4             ) != 0)) ||
      ( ((current_settings_prt.control_urov & MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_MTZ04_1)) != 0) && (_CHECK_SET_BIT(p_active_functions, RANG_MTZ04_1          ) != 0)) ||
      ( ((current_settings_prt.control_urov & MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_MTZ04_2)) != 0) && (_CHECK_SET_BIT(p_active_functions, RANG_MTZ04_2          ) != 0)) ||
      ( ((current_settings_prt.control_urov & MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_ZDZ)    ) != 0) && (_CHECK_SET_BIT(p_active_functions, RANG_ZDZ              ) != 0)) ||
      ( ((current_settings_prt.control_urov & MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_3I0)    ) != 0) && (_CHECK_SET_BIT(p_active_functions, RANG_3I0              ) != 0)) ||
      ( ((current_settings_prt.control_urov & MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_3U0)    ) != 0) && (_CHECK_SET_BIT(p_active_functions, RANG_3U0              ) != 0)) ||
      ( ((current_settings_prt.control_urov & MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_NZZ)    ) != 0) && (_CHECK_SET_BIT(p_active_functions, RANG_NZZ              ) != 0)) ||
      ( ((current_settings_prt.control_urov & MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_TZNP1)  ) != 0) && (_CHECK_SET_BIT(p_active_functions, RANG_TZNP1            ) != 0)) ||
      ( ((current_settings_prt.control_urov & MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_TZNP2)  ) != 0) && (_CHECK_SET_BIT(p_active_functions, RANG_TZNP2            ) != 0)) ||
      ( ((current_settings_prt.control_urov & MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_TZNP3)  ) != 0) && (_CHECK_SET_BIT(p_active_functions, RANG_TZNP3            ) != 0)) ||
      ( ((current_settings_prt.control_urov & MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_ZOP1)   ) != 0) && (_CHECK_SET_BIT(p_active_functions, RANG_ZOP              ) != 0)) ||
      ( ((current_settings_prt.control_urov & MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_UMIN1)  ) != 0) && (_CHECK_SET_BIT(p_active_functions, RANG_UMIN1            ) != 0)) ||
      ( ((current_settings_prt.control_urov & MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_UMIN2)  ) != 0) && (_CHECK_SET_BIT(p_active_functions, RANG_UMIN2            ) != 0)) ||
      ( ((current_settings_prt.control_urov & MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_UMAX1)  ) != 0) && (_CHECK_SET_BIT(p_active_functions, RANG_UMAX1            ) != 0)) ||
      ( ((current_settings_prt.control_urov & MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_UMAX2)  ) != 0) && (_CHECK_SET_BIT(p_active_functions, RANG_UMAX2            ) != 0)) ||
      ( ((current_settings_prt.control_urov & MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_ACHR1)  ) != 0) && (_CHECK_SET_BIT(p_active_functions, RANG_ACHR_CHAPV1      ) != 0)) ||
      ( ((current_settings_prt.control_urov & MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_ACHR2)  ) != 0) && (_CHECK_SET_BIT(p_active_functions, RANG_ACHR_CHAPV2      ) != 0))
     )     
    )
  {
    //Є умова запуску УРОВ
    
    unsigned int setpoint; //уставка - з якою зрівнюється вимірювальна величина
    unsigned int previous_state_po_urov;
    
    //Якщо ПО УРОВ ще не активне, то треба працювати по устаці спацювання - уставці, яка вводиться як основна з системи меню чи верхнього рівня
    //Якщо ПО УРОВ вже спрацювало, то треба працювати по уставці відпускання - береться процент від основної утанки по коефіцієнту повернення
    if(( previous_state_po_urov = _CHECK_SET_BIT(p_active_functions, RANG_PO_UROV) ) ==0 )
    {
      //Працюємо по утавці спрацювання
      setpoint = current_settings_prt.setpoint_urov[number_group_stp];
    }
    else
    {
      //Працюємо по утавці відпускання
      setpoint = current_settings_prt.setpoint_urov[number_group_stp]*KOEF_POVERNENNJA_GENERAL_UP/100;
    }
    
    //Виставляємо, або скидаємо сигнал "ПО УРОВ"
    if (max_faze_current >= setpoint)
    {
      //Максимальний фазний стум перевищує свою уставку
      if(previous_state_po_urov == 0)
      {
        //Встановлюємо сигнал "ПО УРОВ"
        _SET_BIT(p_active_functions, RANG_PO_UROV);
      
        //Запускаємо таймери УРОВ1 і УРОВ2, якщо вони ще не запущені
        global_timers[INDEX_TIMER_UROV1] = 0;
        global_timers[INDEX_TIMER_UROV2] = 0;
      }
    }
    else 
    {
      //Максимальний фазний стум нижче уставки
      if(previous_state_po_urov != 0)
      {
        //Скидаємо сигнал "ПО УРОВ"
        _CLEAR_BIT(p_active_functions, RANG_PO_UROV);
        //Це є умовою також скидання сигналів "Сраб. УРОВ1" і "Сраб. УРОВ2"
        _CLEAR_BIT(p_active_functions, RANG_UROV1);
        _CLEAR_BIT(p_active_functions, RANG_UROV2);
        //Якщо таймери ще не скинуті, то скидаємо їх
        if ( global_timers[INDEX_TIMER_UROV1] >=0) global_timers[INDEX_TIMER_UROV1] = -1;
        if ( global_timers[INDEX_TIMER_UROV2] >=0) global_timers[INDEX_TIMER_UROV2] = -1;
      }
    }
    
    //Перевіряємо чи таймер УРОВ1 досягнув значення своєї витримки
    if(global_timers[INDEX_TIMER_UROV1] >= current_settings_prt.timeout_urov_1[number_group_stp])
    {
      //Якщо витримана Витримка УРОВ1 то встановлюємо сигнал "Сраб. УРОВ1"
      _SET_BIT(p_active_functions, RANG_UROV1);

      //Скидаємо таймер УРОВ1
      global_timers[INDEX_TIMER_UROV1] = -1;
    }

    //Перевіряємо чи таймер УРОВ2 досягнув значення своєї витримки
    if(global_timers[INDEX_TIMER_UROV2] >= current_settings_prt.timeout_urov_2[number_group_stp])
    {
      //Якщо витримана Витримка УРОВ2 то встановлюємо сигнал "Сраб. УРОВ2"
      _SET_BIT(p_active_functions, RANG_UROV2);

      //Скидаємо таймер УРОВ2
      global_timers[INDEX_TIMER_UROV2] = -1;
    }
  }
  else
  {
    //Треба скинути всі таймери і сигнали, які за УРОВ відповідають
    _CLEAR_BIT(p_active_functions, RANG_PO_UROV);
    _CLEAR_BIT(p_active_functions, RANG_UROV1);
    _CLEAR_BIT(p_active_functions, RANG_UROV2);
    global_timers[INDEX_TIMER_UROV1] = -1;
    global_timers[INDEX_TIMER_UROV2] = -1;
  }  
}
  /*******************************/
/*****************************************************/

/*****************************************************/
//АПВ
/*****************************************************/
//#pragma optimize=none
inline void apv_handler(unsigned int *p_active_functions, unsigned int number_group_stp)
{
  unsigned int logic_APV_0 = 0;
  unsigned int logic_APV_1 = 0;
  
  unsigned int extra_logic_APV_0 = 0;

  //Рестарт пристрою
  logic_APV_1 |= (start_restart != 0) << 19;
  //Очищаємо повідомлення про рестрат пристрою
  start_restart = 0;

  //"Стан вимикача"
  logic_APV_0 |= (_CHECK_SET_BIT(p_active_functions, RANG_STATE_VV) != 0) << 21;
  _TIMER_IMPULSE(INDEX_TIMER_APV_BLOCK_VID_VV, current_settings_prt.timeout_apv_block_vid_VV[number_group_stp], previous_states_APV_0, 0, logic_APV_0, 21, logic_APV_0, 22);

  //"Статичне блокування"
  logic_APV_0 |= (_CHECK_SET_BIT(p_active_functions, RANG_STAT_BLK_APV) != 0) << 0;
  _INVERTOR(logic_APV_0, 0, logic_APV_0, 1);

  //М:"Пуск від МТЗ1"
  logic_APV_0 |= ((current_settings_prt.control_apv & CTR_APV_STARTED_FROM_MTZ1) != 0) << 2;
  //"МТЗ1"
  logic_APV_0 |= (_CHECK_SET_BIT(p_active_functions, RANG_MTZ1) != 0) << 3;
  _AND2(logic_APV_0, 2, logic_APV_0, 3, logic_APV_0, 4);

  //М:"Пуск від МТЗ2"
  logic_APV_0 |= ((current_settings_prt.control_apv & CTR_APV_STARTED_FROM_MTZ2) != 0) << 5;
  //"МТЗ2"
  logic_APV_0 |= (_CHECK_SET_BIT(p_active_functions, RANG_MTZ2) != 0) << 6;
  _AND2(logic_APV_0, 5, logic_APV_0, 6, logic_APV_0, 7);

  //М:"Пуск від МТЗ3"
  logic_APV_0 |= ((current_settings_prt.control_apv & CTR_APV_STARTED_FROM_MTZ3) != 0) << 8;
  //"МТЗ3"
  logic_APV_0 |= (_CHECK_SET_BIT(p_active_functions, RANG_MTZ3) != 0) << 9;
  _AND2(logic_APV_0, 8, logic_APV_0, 9, logic_APV_0, 10);

  //М:"Пуск від МТЗ4"
  logic_APV_0 |= ((current_settings_prt.control_apv & CTR_APV_STARTED_FROM_MTZ4) != 0) << 11;
  //"МТЗ4"
  logic_APV_0 |= (_CHECK_SET_BIT(p_active_functions, RANG_MTZ4) != 0) << 12;
  _AND2(logic_APV_0, 11, logic_APV_0, 12, logic_APV_0, 13);

  _OR4(logic_APV_0, 4, logic_APV_0, 7, logic_APV_0, 10, logic_APV_0, 13, logic_APV_0, 14);

  //М:"Блок.контр.ст.ВВ"
  extra_logic_APV_0 |= ((current_settings_prt.control_apv & CTR_APV_CTRAPV_BLK_CTRL_VV) != 0) << 8;
  _INVERTOR(extra_logic_APV_0, 8, extra_logic_APV_0, 9);
  _AND2(logic_APV_0, 21, extra_logic_APV_0, 8, extra_logic_APV_0, 10);
  _OR3(extra_logic_APV_0, 10, extra_logic_APV_0, 9, static_logic_APV_0, 0, extra_logic_APV_0, 0);
  _AND2(logic_APV_0, 14, extra_logic_APV_0, 0, static_logic_APV_0, 0);
  
  _TIMER_0_T(INDEX_TIMER_APV_TMP1, 2, static_logic_APV_0, 0, logic_APV_0, 15);

  //М:"ЧАПВ 1"
  logic_APV_0 |= ((current_settings_prt.control_achr_chapv & CTR_CHAPV1) != 0) << 16;
  //"АЧР/ЧАПВ 1"
  logic_APV_1 |= (_CHECK_SET_BIT(p_active_functions, RANG_ACHR_CHAPV1) != 0) << 21;
  //М:"ЧАПВ 2"
  logic_APV_1 |= ((current_settings_prt.control_achr_chapv & CTR_CHAPV2) != 0) << 27;
  //"АЧР/ЧАПВ 2"
  logic_APV_1 |= (_CHECK_SET_BIT(p_active_functions, RANG_ACHR_CHAPV2) != 0) << 28;
  //"Разр ЧАПВ"
  logic_APV_1 |= (_CHECK_SET_BIT(p_active_functions, RANG_RAZR_CHAPV) != 0) << 20;

  //"АЧР/ЧАПВ від ДВ"
  logic_APV_0 |= (_CHECK_SET_BIT(p_active_functions, RANG_ACHR_CHAPV_VID_DV) != 0) << 17;
  //М:"ЧАПВ від ДВ"
  logic_APV_1 |= ((current_settings_prt.control_achr_chapv & CTR_CHAPV_VID_DV) != 0) << 22;
  
  _AND3(logic_APV_0, 16, logic_APV_1, 21, logic_APV_1, 20, logic_APV_1, 23);
  _AND3(logic_APV_1, 27, logic_APV_1, 28, logic_APV_1, 20, logic_APV_1, 29);
  _AND2(logic_APV_0, 17, logic_APV_1, 22, logic_APV_1, 24);
  _OR3(logic_APV_1, 23, logic_APV_1, 29, logic_APV_1, 24, logic_APV_0, 18)

  //"Блок.ЧАПВ"
  logic_APV_1 |= (_CHECK_SET_BIT(p_active_functions, RANG_VN_BLOCK_CHAPV) != 0) << 25;
  _OR2(logic_APV_0, 18, logic_APV_1, 25, logic_APV_1, 26);
    
  _TIMER_0_T(INDEX_TIMER_ACHR_CHAPV, MAX_VALUE_TIMER_FOR_COUNT_SIGNAL_ACHR_CHAPV, logic_APV_0, 18, logic_APV_0, 19);

  _OR2_INVERTOR(logic_APV_0, 15, logic_APV_0, 19, logic_APV_0, 20);

  //М:"АПВ"
  logic_APV_0 |= ((current_settings_prt.control_apv & CTR_APV_STAGE_1) != 0) << 23;
  _INVERTOR(logic_APV_0, 23, logic_APV_0, 24);

  //М:"АПВ2"
  logic_APV_0 |= ((current_settings_prt.control_apv & CTR_APV_STAGE_2) != 0) << 25;
  _INVERTOR(logic_APV_0, 25, logic_APV_0, 26);

  //М:"АПВ3"
  logic_APV_0 |= ((current_settings_prt.control_apv & CTR_APV_STAGE_3) != 0) << 27;
  _INVERTOR(logic_APV_0, 27, logic_APV_0, 28);

  //М:"АПВ4"
  logic_APV_0 |= ((current_settings_prt.control_apv & CTR_APV_STAGE_4) != 0) << 29;
  _INVERTOR(logic_APV_0, 29, logic_APV_0, 30);

  //Наступні операції виконуються ітераційно поки всі сигнали не встановляться у фіксований стан
  logic_APV_0 |= _GET_STATE(previous_states_APV_0, 1) << 31;
  logic_APV_1 |= _GET_STATE(previous_states_APV_0, 2) <<  0;
  logic_APV_1 |= _GET_STATE(previous_states_APV_0, 3) <<  1;
  logic_APV_1 |= _GET_STATE(previous_states_APV_0, 4) <<  2;
  
  extra_logic_APV_0 |= (_CHECK_SET_BIT(p_active_functions, RANG_UROV2) != 0) << 1;
  extra_logic_APV_0 |= ((current_settings_prt.control_apv & CTR_APV_CTRAPV_BLK_CTRL_PRVV2) != 0) << 3;
  _AND2(extra_logic_APV_0, 1, extra_logic_APV_0, 3, extra_logic_APV_0, 11);
  _TIMER_0_T(INDEX_TIMER_APV_TMP2, 40, extra_logic_APV_0, 11, extra_logic_APV_0, 2);

  unsigned int previous_trigger_APV_0;
  
  do
  {
    /***
    Фіксуємо теперішній стан сигналів, які потім, можливо, зміняться і від зміни
    стану яких залежить завершення ітераційного процесу
    ***/
    _CLEAR_STATE(previous_states_APV_0, 1);
    previous_states_APV_0 |= _GET_STATE(logic_APV_0, 31) << 1;
    _CLEAR_STATE(previous_states_APV_0, 2);
    previous_states_APV_0 |= _GET_STATE(logic_APV_1,  0) << 2;
    _CLEAR_STATE(previous_states_APV_0, 3);
    previous_states_APV_0 |= _GET_STATE(logic_APV_1,  1) << 3;
    _CLEAR_STATE(previous_states_APV_0, 4);
    previous_states_APV_0 |= _GET_STATE(logic_APV_1,  2) << 4;
    
    previous_trigger_APV_0 = trigger_APV_0;
    /***/
    
    _OR2(logic_APV_1, 2, logic_APV_1, 1, logic_APV_1, 3);
    
    
    //АПВ1
    _OR6(logic_APV_1, 19, static_logic_APV_0, 0, trigger_APV_0, 1, trigger_APV_0, 2, trigger_APV_0, 3, logic_APV_0, 31, logic_APV_1, 4);
    _OR6(logic_APV_1, 26, logic_APV_0, 22, logic_APV_0, 24, logic_APV_1, 3, logic_APV_1, 0, logic_APV_1, 4, logic_APV_1, 8);
    _OR2(logic_APV_1, 8, extra_logic_APV_0, 2, extra_logic_APV_0, 4);
    _D_TRIGGER(_GET_STATE(logic_APV_0, 1),  0, _GET_STATE(extra_logic_APV_0, 4), previous_states_APV_0, 5, logic_APV_0, 20, trigger_APV_0, 0);                 
    _TIMER_T_0(INDEX_TIMER_APV_1, current_settings_prt.timeout_apv_1[number_group_stp], trigger_APV_0, 0, logic_APV_1, 9);

    _TIMER_T_0(INDEX_TIMER_APV_1_2MS, 2, logic_APV_1, 9, extra_logic_APV_0, 12);
    _TIMER_0_T(INDEX_TIMER_APV_BLOCK_VID_APV1, current_settings_prt.timeout_apv_block_vid_apv1[number_group_stp], extra_logic_APV_0, 12, logic_APV_0, 31);

    //АПВ2
    _AND2_INVERTOR(logic_APV_0, 31, logic_APV_0, 15, logic_APV_1, 10);
    _OR6(logic_APV_1, 19, static_logic_APV_0, 0, trigger_APV_0, 0, trigger_APV_0, 2, trigger_APV_0, 3, logic_APV_1,  0, logic_APV_1, 5);
    _OR6(logic_APV_1, 26, logic_APV_0, 24, logic_APV_0, 26, logic_APV_1, 2, logic_APV_1, 1, logic_APV_1, 5, logic_APV_1, 11);
    _OR2(logic_APV_1, 11, extra_logic_APV_0, 2, extra_logic_APV_0, 5);
    _D_TRIGGER(1,  0, _GET_STATE(extra_logic_APV_0, 5), previous_states_APV_0, 6, logic_APV_1, 10, trigger_APV_0, 1);    
    _TIMER_T_0(INDEX_TIMER_APV_2, current_settings_prt.timeout_apv_2[number_group_stp], trigger_APV_0, 1, logic_APV_1, 12);

    _TIMER_T_0(INDEX_TIMER_APV_2_2MS, 2, logic_APV_1, 12, extra_logic_APV_0, 13);
    _TIMER_0_T(INDEX_TIMER_APV_BLOCK_VID_APV2, current_settings_prt.timeout_apv_block_vid_apv2[number_group_stp], extra_logic_APV_0, 13, logic_APV_1, 0);

    //АПВ3
    _AND2_INVERTOR(logic_APV_1, 0, logic_APV_0, 15, logic_APV_1, 13);
    _OR6(logic_APV_1, 19, static_logic_APV_0, 0, trigger_APV_0, 0, trigger_APV_0, 1, trigger_APV_0, 3, logic_APV_1,  1, logic_APV_1, 6);
    _OR5(logic_APV_1, 26, logic_APV_0, 24, logic_APV_0, 28, logic_APV_1, 2, logic_APV_1, 6, logic_APV_1, 14);
    _OR2(logic_APV_1, 14, extra_logic_APV_0, 2, extra_logic_APV_0, 6);
    _D_TRIGGER(1,  0, _GET_STATE(extra_logic_APV_0, 6), previous_states_APV_0, 7, logic_APV_1, 13, trigger_APV_0, 2);                 
    _TIMER_T_0(INDEX_TIMER_APV_3, current_settings_prt.timeout_apv_3[number_group_stp], trigger_APV_0, 2, logic_APV_1, 15);

    _TIMER_T_0(INDEX_TIMER_APV_3_2MS, 2, logic_APV_1, 15, extra_logic_APV_0, 14);
    _TIMER_0_T(INDEX_TIMER_APV_BLOCK_VID_APV3, current_settings_prt.timeout_apv_block_vid_apv3[number_group_stp], extra_logic_APV_0, 14, logic_APV_1, 1);

    //АПВ4
    _AND2_INVERTOR(logic_APV_1, 1, logic_APV_0, 15, logic_APV_1, 16);
    _OR6(logic_APV_1, 19, static_logic_APV_0, 0, trigger_APV_0, 0, trigger_APV_0, 1, trigger_APV_0, 2, logic_APV_1,  2, logic_APV_1, 7);
    _OR4(logic_APV_1, 26, logic_APV_0, 24, logic_APV_0, 30, logic_APV_1, 7, logic_APV_1, 17);
    _OR2(logic_APV_1, 17, extra_logic_APV_0, 2, extra_logic_APV_0, 7);
    _D_TRIGGER(1,  0, _GET_STATE(extra_logic_APV_0, 7), previous_states_APV_0, 8, logic_APV_1, 16, trigger_APV_0, 3);                 
    _TIMER_T_0(INDEX_TIMER_APV_4, current_settings_prt.timeout_apv_4[number_group_stp], trigger_APV_0, 3, logic_APV_1, 18);

    _TIMER_T_0(INDEX_TIMER_APV_4_2MS, 2, logic_APV_1, 18, extra_logic_APV_0, 15);
    _TIMER_0_T(INDEX_TIMER_APV_BLOCK_VID_APV4, current_settings_prt.timeout_apv_block_vid_apv4[number_group_stp], extra_logic_APV_0, 15, logic_APV_1, 2);
  }
  while (
         (_GET_STATE(logic_APV_0, 31) != _GET_STATE(previous_states_APV_0, 1)) ||
         (_GET_STATE(logic_APV_1,  0) != _GET_STATE(previous_states_APV_0, 2)) ||
         (_GET_STATE(logic_APV_1,  1) != _GET_STATE(previous_states_APV_0, 3)) ||
         (_GET_STATE(logic_APV_1,  2) != _GET_STATE(previous_states_APV_0, 4)) ||
         (previous_trigger_APV_0 != trigger_APV_0)  
        );
  //АПВ1
 if (_GET_STATE(logic_APV_1, 9)) _SET_BIT(p_active_functions, RANG_APV1);
 else _CLEAR_BIT(p_active_functions, RANG_APV1);
 
  //АПВ2
 if (_GET_STATE(logic_APV_1, 12)) _SET_BIT(p_active_functions, RANG_APV2);
 else _CLEAR_BIT(p_active_functions, RANG_APV2);
 
  //АПВ3
 if (_GET_STATE(logic_APV_1, 15)) _SET_BIT(p_active_functions, RANG_APV3);
 else _CLEAR_BIT(p_active_functions, RANG_APV3);
 
  //АПВ4
 if (_GET_STATE(logic_APV_1, 18)) _SET_BIT(p_active_functions, RANG_APV4);
 else _CLEAR_BIT(p_active_functions, RANG_APV4);
  
  _Bool work_apv = 0;
  _OR4(trigger_APV_0, 0, trigger_APV_0, 1, trigger_APV_0, 2, trigger_APV_0, 3, work_apv, 0);
  
  //Работа АПВ
  if (work_apv) _SET_BIT(p_active_functions, RANG_APV_WORK);
  else  _CLEAR_BIT(p_active_functions, RANG_APV_WORK);
  
}
/*****************************************************/

/*****************************************************/
// Універсальний Захист
/*****************************************************/
inline void up_handler(unsigned int *p_active_functions, unsigned int number_group_stp)
{
  for (size_t n_UP = 0; n_UP < NUMBER_UP; n_UP++)
  {
    uint32_t logic_UP_0 = 0;
    
    logic_UP_0 |= (
                   ((current_settings_prt.control_UP & MASKA_FOR_BIT(n_UP*(_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_STATE_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I))) != 0)
                   &&
                   (_CHECK_SET_BIT(p_active_functions, (RANG_BLOCK_UP1 + 3*n_UP)) == 0)
                  ) << 0;

    int32_t pickup = current_settings_prt.setpoint_UP[n_UP][0][number_group_stp];
    if (
        (current_settings_prt.ctrl_UP_input[n_UP] == UP_CTRL_3I0) ||
        (current_settings_prt.ctrl_UP_input[n_UP] == UP_CTRL_3I0_others)
       ) 
    {
      if (_CHECK_SET_BIT(p_active_functions, (RANG_PO_UP1 + 3*n_UP)) != 0)
      {
        /*
        Алгебраїчне спрощення виразу
        setpoint = (pickup*koef_povernennja/100)*10 =  pickup*koef_povernennja/10
        */
        pickup = (pickup * (int32_t)current_settings_prt.setpoint_UP_KP[n_UP][0][number_group_stp])/10;
      }
      else
      {
        pickup *= 10;
      }
        
    }
    else
    {
      if (_CHECK_SET_BIT(p_active_functions, (RANG_PO_UP1 + 3*n_UP)) != 0) pickup = (pickup * (int32_t)current_settings_prt.setpoint_UP_KP[n_UP][0][number_group_stp])/100;
    }

    unsigned int more_less = ((current_settings_prt.control_UP & MASKA_FOR_BIT(n_UP*(_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_MORE_LESS_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I))) != 0);
    
    int32_t analog_value = 0;
    uint32_t PQ = false;
    uint32_t bank_for_calc_power_tmp = (state_calc_power == false ) ? bank_for_calc_power : ((bank_for_calc_power ^ 0x1) & 0x1);

    switch (current_settings_prt.ctrl_UP_input[n_UP])
    {
    case UP_CTRL_Ia_Ib_Ic:
      {
        analog_value = measurement[IM_IA];

        unsigned int or_and = ((current_settings_prt.control_UP & MASKA_FOR_BIT(n_UP*(_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_OR_AND_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I))) != 0);
        if (
            ((more_less == 0) && (or_and == 0)) ||
            ((more_less != 0) && (or_and != 0))
           )   
        {
         if ((uint32_t)analog_value < measurement[IM_IB]) analog_value = measurement[IM_IB];
         if ((uint32_t)analog_value < measurement[IM_IC]) analog_value = measurement[IM_IC];
        }
        else
        {
         if ((uint32_t)analog_value > measurement[IM_IB]) analog_value = measurement[IM_IB];
         if ((uint32_t)analog_value > measurement[IM_IC]) analog_value = measurement[IM_IC];
        }
        
        break;
      }
    case UP_CTRL_Ia:
      {
        analog_value = measurement[IM_IA];
        
        break;
      }
    case UP_CTRL_Ib:
      {
        analog_value = measurement[IM_IB];
        
        break;
      }
    case UP_CTRL_Ic:
      {
        analog_value = measurement[IM_IC];
        
        break;
      }
    case UP_CTRL_I1:
      {
        analog_value = measurement[IM_I1];
        
        break;
      }
    case UP_CTRL_I2:
      {
        analog_value = measurement[IM_I2];
        
        break;
      }
    case UP_CTRL_I04:
      {
        analog_value = measurement[IM_I04];
        
        break;
      }
    case UP_CTRL_3I0_r:
      {
        analog_value = measurement[IM_3I0_r];
        
        break;
      }
    case UP_CTRL_3I0:
      {
        analog_value = measurement[IM_3I0];
        
        break;
      }
    case UP_CTRL_3I0_others:
      {
        analog_value = measurement[IM_3I0_other_g];
        
        break;
      }
    case UP_CTRL_Ua_Ub_Uc:
      {
        analog_value = measurement[IM_UA];
        
        unsigned int or_and = ((current_settings_prt.control_UP & MASKA_FOR_BIT(n_UP*(_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_OR_AND_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I))) != 0);
        uint32_t analog_value_tmp = measurement[IM_UB];
        if (
            ((more_less == 0) && (or_and == 0)) ||
            ((more_less != 0) && (or_and != 0))
           )   
        {
          if ((uint32_t)analog_value < analog_value_tmp) analog_value = analog_value_tmp;
         
          analog_value_tmp = measurement[IM_UC];
          if ((uint32_t)analog_value < analog_value_tmp) analog_value = analog_value_tmp;
        }
        else
        {
          if ((uint32_t)analog_value > analog_value_tmp) analog_value = analog_value_tmp;
         
          analog_value_tmp = measurement[IM_UC];
          if ((uint32_t)analog_value > analog_value_tmp) analog_value = analog_value_tmp;
        }
        
        break;
      }
    case UP_CTRL_Ua:
      {
        analog_value = measurement[IM_UA];
        
        break;
      }
    case UP_CTRL_Ub:
      {
        analog_value = measurement[IM_UB];
        
        break;
      }
    case UP_CTRL_Uc:
      {
        analog_value = measurement[IM_UC];
        
        break;
      }
    case UP_CTRL_Uab_Ubc_Uca:
      {
        analog_value = measurement[IM_UAB];
        
        unsigned int or_and = ((current_settings_prt.control_UP & MASKA_FOR_BIT(n_UP*(_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_OR_AND_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I))) != 0);
        uint32_t analog_value_tmp = measurement[IM_UBC];
        if (
            ((more_less == 0) && (or_and == 0)) ||
            ((more_less != 0) && (or_and != 0))
           )   
        {
          if ((uint32_t)analog_value < analog_value_tmp) analog_value = analog_value_tmp;
         
          analog_value_tmp = measurement[IM_UCA];
          if ((uint32_t)analog_value < analog_value_tmp) analog_value = analog_value_tmp;
        }
        else
        {
          if ((uint32_t)analog_value > analog_value_tmp) analog_value = analog_value_tmp;
         
          analog_value_tmp = measurement[IM_UCA];
          if ((uint32_t)analog_value > analog_value_tmp) analog_value = analog_value_tmp;
        }
        
        break;
      }
    case UP_CTRL_Uab:
      {
        analog_value = measurement[IM_UAB];
        
        break;
      }
    case UP_CTRL_Ubc:
      {
        analog_value = measurement[IM_UBC];
        
        break;
      }
    case UP_CTRL_Uca:
      {
        analog_value = measurement[IM_UCA];
        
        break;
      }
    case UP_CTRL_U1:
      {
        analog_value = measurement[IM_U1];
        
        break;
      }
    case UP_CTRL_U2:
      {
        analog_value = measurement[IM_U2];
        
        break;
      }
    case UP_CTRL_3U0:
      {
        analog_value = measurement[IM_3U0];
        
        break;
      }
    case UP_CTRL_P:
      {
        PQ = true;
        analog_value = P[bank_for_calc_power_tmp];
        
        break;
      }
    case UP_CTRL_Q:
      {
        PQ = true;
        analog_value = Q[bank_for_calc_power_tmp];
        
        break;
      }
    case UP_CTRL_S:
      {
        analog_value = S[bank_for_calc_power_tmp];
        
        break;
      }
    default:
      {
        //Теоретично цього ніколи не мало б бути
        total_error_sw_fixed();
      }
    }
    
    if (more_less)
    {
      if (PQ)
      {
        if (
            (
             (analog_value >= 0) &&
             (pickup >= 0)  
            )
            ||
            (
             (analog_value < 0) &&
             (pickup < 0)  
            ) 
           ) 
        {
          logic_UP_0 |= (abs(analog_value) <= abs(pickup)) << 1;
        }
      }
      else
      {
        logic_UP_0 |= (analog_value <= pickup) << 1;
      }
    }
    else
    {
      if (PQ)
      {
        if (
            (
             (analog_value >= 0) &&
             (pickup >= 0)  
            )
            ||
            (
             (analog_value < 0) &&
             (pickup < 0)  
            ) 
           ) 
        {
          logic_UP_0 |= (abs(analog_value) >= abs(pickup)) << 1;
        }
      }
      else
      {
        logic_UP_0 |= (analog_value >= pickup) << 1;
      }
    }

    _AND2(logic_UP_0, 0, logic_UP_0, 1, logic_UP_0, 2);
    if (_GET_STATE(logic_UP_0, 2)) _SET_BIT(p_active_functions, (RANG_PO_UP1 + 3*n_UP));
    else _CLEAR_BIT(p_active_functions, (RANG_PO_UP1 + 3*n_UP));
    
    _TIMER_T_0(INDEX_TIMER_UP1 + n_UP, current_settings_prt.timeout_UP[n_UP][0][number_group_stp], logic_UP_0, 2, logic_UP_0, 3);
    if (_GET_STATE(logic_UP_0, 3)) _SET_BIT(p_active_functions, (RANG_UP1 + 3*n_UP));
    else _CLEAR_BIT(p_active_functions, (RANG_UP1 + 3*n_UP));
  }
}
/*****************************************************/

/*****************************************************/
//Функція управління блоками включення і відключення
/*****************************************************/
inline void on_off_handler(unsigned int *p_active_functions)
{
  static unsigned int previous_active_functions[N_BIG];

  /*********************/
  //Спочатку опрацьовуємо таймери
  /*********************/
  //Таймер  відключення
  if (global_timers[INDEX_TIMER_VIDKL_VV] >= 0)
  {
    //Таймер БО зараз активний і як мінімум тільки зараз завершить свою роботу
    if (global_timers[INDEX_TIMER_VIDKL_VV] >= current_settings_prt.timeout_swch_off)
    {
      //Таймер досягнув свого максимального значення
      global_timers[INDEX_TIMER_VIDKL_VV] = -1;
      //Відмічаємо у масиві функцій, які зараз активуються, що блок БО має бути деативованим
      _CLEAR_BIT(p_active_functions, RANG_WORK_BO);
    }
    //Незавершена робота блоку БО означає, що таймер блокування БВ має бути запущений і знаходитися у свому початковому значенні,
    //щоб як тільки блок БО відпрацює, щоб блокування включення почалося на весь час з моменту закінчення роботи блоку БО
    global_timers[INDEX_TIMER_BLK_VKL_VV] = 0;
  }
    
  //Таймер  блокування включення
  if (global_timers[INDEX_TIMER_BLK_VKL_VV] >= 0)
  {
    //Таймер блокування включення БВ зараз активний і як мінімум тільки зараз завершить свою роботу
    if (global_timers[INDEX_TIMER_BLK_VKL_VV] >= current_settings_prt.timeout_swch_udl_blk_on)
    {
      //Таймер досягнув свого максимального значення
      global_timers[INDEX_TIMER_BLK_VKL_VV] = -1;
    }
  }

  //Таймер  включення
  if (global_timers[INDEX_TIMER_VKL_VV] >= 0)
  {
    //Таймер БВ зараз активний і як мінімум тільки зараз завершить свою роботу

    //Якщо по якійсь причині таймер включення працює, при умові, що таймери БО і блокування включення ще не скинуті, то таймер включення треба скинути
    if ((global_timers[INDEX_TIMER_VIDKL_VV] >= 0) || (global_timers[INDEX_TIMER_BLK_VKL_VV] >=0))
    {
      global_timers[INDEX_TIMER_VKL_VV] = -1;
      //Відмічаємо у масиві функцій, які зараз активуються, що блок БB має бути деативованим
      _CLEAR_BIT(p_active_functions, RANG_WORK_BV);
    }
    else
    {
      //Перевіряємо, чи таймер включення не досягнув свого масимального значення
      if (global_timers[INDEX_TIMER_VKL_VV] >= current_settings_prt.timeout_swch_on)
      {
        //Таймер досягнув свого максимального значення
        global_timers[INDEX_TIMER_VKL_VV] = -1;
        //Відмічаємо у масиві функцій, які зараз активуються, що блок БB має бути деативованим
        _CLEAR_BIT(p_active_functions, RANG_WORK_BV);
      }
    }
  }
  /*********************/

  /*********************/
  //Першим розглядається блок відключення, бо він може блокувати включення вимикача
  /*********************/
  uint32_t off_cb_tmp[N_BIG];
  for(size_t m = 0; m < N_BIG; ++m) 
  {
    off_cb_tmp[m] = (p_active_functions[m] & current_settings_prt.ranguvannja_off_cb[m]);
  }
  
  /*
  Цей сигнал встановлюється тільки у певних випадках, тому по замовчуванню його треба скинута,
  а коли буде потрібно - він встановиться
  */
  _CLEAR_BIT(p_active_functions, RANG_VIDKL_VID_ZAKHYSTIV);
  int flag = 0;
  for(size_t m = 0; m < N_BIG; ++m)
  {
    if(off_cb_tmp[m] != 0) 
    {
      flag=1; 
      break;
    }
  }
  if (flag)
  {
    //Є умова активації блку вимкнення
    _SET_BIT(p_active_functions, RANG_WORK_BO);

    //Запускаємо (або продовжуємо утримувати у 0, поки не пропаде сигнал активації БО) таймери: блоку БО, блокуванння БВ.
    global_timers[INDEX_TIMER_VIDKL_VV  ] = 0;
    global_timers[INDEX_TIMER_BLK_VKL_VV] = 0;
    
    //Скидаємо активацію блоку увімкнення
    _CLEAR_BIT(p_active_functions, RANG_WORK_BV);
    //Скидаємо таймер блку вимкнення
    global_timers[INDEX_TIMER_VKL_VV] = -1;  

    /*
    Формуємо сигнал "Відключення від захистів" (він рівний наявності умови команди
    активації команди "Робота БО" будь-якою командою за виключенняв "Вимкн. ВВ")
    */
    _CLEAR_BIT(off_cb_tmp, RANG_OTKL_VV);
    flag = 0;
    for(size_t m = 0; m < N_BIG; ++m) 
    {
      if(off_cb_tmp[m] != 0) 
      {
        flag = 1; 
        break;
      }
    }
    if (flag)
    {
      //Вимкнення від захистів
      _SET_BIT(p_active_functions, RANG_VIDKL_VID_ZAKHYSTIV);
      
      /*****************************************************
      Формуванні інформації про причину відключення для меню
      *****************************************************/
      __info_vymk info_vymk_tmp = {time_dat, time_ms};
          
      //МТЗ1
      if(
         (_CHECK_SET_BIT(off_cb_tmp, RANG_MTZ1) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_MTZ1) == 0) /*умова, що сигнал тільки активується (щоб зафіксувати час старту)*/
        )   
      {
        _SET_BIT(info_vidkluchennja_vymykacha, VYMKNENNJA_VID_MTZ1);
        info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_MTZ1] = info_vymk_tmp;

        _CLEAR_BIT(off_cb_tmp, RANG_MTZ1);
      }
      
      //МТЗ2
      if(
         (_CHECK_SET_BIT(off_cb_tmp, RANG_MTZ2) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_MTZ2) == 0) /*умова, що сигнал тільки активується (щоб зафіксувати час старту)*/
        )   
      {
        _SET_BIT(info_vidkluchennja_vymykacha, VYMKNENNJA_VID_MTZ2);
        info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_MTZ2] = info_vymk_tmp;

        _CLEAR_BIT(off_cb_tmp, RANG_MTZ2);
      }
      
      //МТЗ3
      if(
         (_CHECK_SET_BIT(off_cb_tmp, RANG_MTZ3) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_MTZ3) == 0) /*умова, що сигнал тільки активується (щоб зафіксувати час старту)*/
        )   
      {
        _SET_BIT(info_vidkluchennja_vymykacha, VYMKNENNJA_VID_MTZ3);
        info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_MTZ3] = info_vymk_tmp;

        _CLEAR_BIT(off_cb_tmp, RANG_MTZ3);
      }
      
      //МТЗ4
      if(
         (_CHECK_SET_BIT(off_cb_tmp, RANG_MTZ4) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_MTZ4) == 0) /*умова, що сигнал тільки активується (щоб зафіксувати час старту)*/
        )   
      {
        _SET_BIT(info_vidkluchennja_vymykacha, VYMKNENNJA_VID_MTZ4);
        info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_MTZ4] = info_vymk_tmp;

        _CLEAR_BIT(off_cb_tmp, RANG_MTZ4);
      }
      
          //МТЗ1 04 кВ
      if(
         (_CHECK_SET_BIT(off_cb_tmp, RANG_MTZ04_1) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_MTZ04_1) == 0) /*умова, що сигнал тільки активується (щоб зафіксувати час старту)*/
        )   
      {
        _SET_BIT(info_vidkluchennja_vymykacha, VYMKNENNJA_VID_MTZ04_1);
        info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_MTZ04_1] = info_vymk_tmp;

        _CLEAR_BIT(off_cb_tmp, RANG_MTZ04_1);
      }
          
      //МТЗ2 04кВ
      if(
         (_CHECK_SET_BIT(off_cb_tmp, RANG_MTZ04_2) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_MTZ04_2) == 0) /*умова, що сигнал тільки активується (щоб зафіксувати час старту)*/
        )   
      {
        _SET_BIT(info_vidkluchennja_vymykacha, VYMKNENNJA_VID_MTZ04_2);
        info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_MTZ04_2] = info_vymk_tmp;

        _CLEAR_BIT(off_cb_tmp, RANG_MTZ04_2);
      }
          
      //ЗДЗ
      if(
         (_CHECK_SET_BIT(off_cb_tmp, RANG_ZDZ) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_ZDZ) == 0) /*умова, що сигнал тільки активується (щоб зафіксувати час старту)*/
        )   
      {
        _SET_BIT(info_vidkluchennja_vymykacha, VYMKNENNJA_VID_ZDZ);
        info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_ZDZ] = info_vymk_tmp;

        _CLEAR_BIT(off_cb_tmp, RANG_ZDZ);
      }
      
      //ЗЗ/3I0
      if(
         (_CHECK_SET_BIT(off_cb_tmp, RANG_3I0) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_3I0) == 0) /*умова, що сигнал тільки активується (щоб зафіксувати час старту)*/
        )   
      {
        _SET_BIT(info_vidkluchennja_vymykacha, VYMKNENNJA_VID_3I0);
        info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_3I0] = info_vymk_tmp;

        _CLEAR_BIT(off_cb_tmp, RANG_3I0);
      }
          
      //ЗЗ/3U0
      if(
         (_CHECK_SET_BIT(off_cb_tmp, RANG_3U0) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_3U0) == 0) /*умова, що сигнал тільки активується (щоб зафіксувати час старту)*/
        )   
      {
        _SET_BIT(info_vidkluchennja_vymykacha, VYMKNENNJA_VID_3U0);
        info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_3U0] = info_vymk_tmp;

        _CLEAR_BIT(off_cb_tmp, RANG_3U0);
      }
          
      //СЗЗ
      if(
         (_CHECK_SET_BIT(off_cb_tmp, RANG_NZZ) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_NZZ) == 0) /*умова, що сигнал тільки активується (щоб зафіксувати час старту)*/
        )   
      {
        _SET_BIT(info_vidkluchennja_vymykacha, VYMKNENNJA_VID_NZZ);
        info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_NZZ] = info_vymk_tmp;

        _CLEAR_BIT(off_cb_tmp, RANG_NZZ);
      }
          
      //ТЗНП1
      if(
         (_CHECK_SET_BIT(off_cb_tmp, RANG_TZNP1) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_TZNP1) == 0) /*умова, що сигнал тільки активується (щоб зафіксувати час старту)*/
        )   
      {
        _SET_BIT(info_vidkluchennja_vymykacha, VYMKNENNJA_VID_TZNP1);
        info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_TZNP1] = info_vymk_tmp;

        _CLEAR_BIT(off_cb_tmp, RANG_TZNP1);
      }

      //ТЗНП2
      if(
         (_CHECK_SET_BIT(off_cb_tmp, RANG_TZNP2) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_TZNP2) == 0) /*умова, що сигнал тільки активується (щоб зафіксувати час старту)*/
        )   
      {
        _SET_BIT(info_vidkluchennja_vymykacha, VYMKNENNJA_VID_TZNP2);
        info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_TZNP2] = info_vymk_tmp;

        _CLEAR_BIT(off_cb_tmp, RANG_TZNP2);
      }

      //ТЗНП3
      if(
         (_CHECK_SET_BIT(off_cb_tmp, RANG_TZNP3) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_TZNP3) == 0) /*умова, що сигнал тільки активується (щоб зафіксувати час старту)*/
        )   
      {
        _SET_BIT(info_vidkluchennja_vymykacha, VYMKNENNJA_VID_TZNP3);
        info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_TZNP3] = info_vymk_tmp;

        _CLEAR_BIT(off_cb_tmp, RANG_TZNP3);
      }

      //АЧР/ЧАПВ від ДВ
      if(
         (_CHECK_SET_BIT(off_cb_tmp, RANG_ACHR_CHAPV_VID_DV) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_ACHR_CHAPV_VID_DV) == 0) /*умова, що сигнал тільки активується (щоб зафіксувати час старту)*/
        )   
      {
        _SET_BIT(info_vidkluchennja_vymykacha, VYMKNENNJA_VID_ACHR_CHAPV_VID_DV);
        info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_ACHR_CHAPV_VID_DV] = info_vymk_tmp;

        _CLEAR_BIT(off_cb_tmp, RANG_ACHR_CHAPV_VID_DV);
      }
          
      //АЧР/ЧАПВ1
      if(
         (_CHECK_SET_BIT(off_cb_tmp, RANG_ACHR_CHAPV1) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_ACHR_CHAPV1) == 0) /*умова, що сигнал тільки активується (щоб зафіксувати час старту)*/
        )   
      {
        _SET_BIT(info_vidkluchennja_vymykacha, VYMKNENNJA_VID_ACHR_CHAPV1);
        info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_ACHR_CHAPV1] = info_vymk_tmp;

        _CLEAR_BIT(off_cb_tmp, RANG_ACHR_CHAPV1);
      }
          
      //АЧР/ЧАПВ2
      if(
         (_CHECK_SET_BIT(off_cb_tmp, RANG_ACHR_CHAPV2) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_ACHR_CHAPV2) == 0) /*умова, що сигнал тільки активується (щоб зафіксувати час старту)*/
        )   
      {
        _SET_BIT(info_vidkluchennja_vymykacha, VYMKNENNJA_VID_ACHR_CHAPV2);
        info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_ACHR_CHAPV2] = info_vymk_tmp;

        _CLEAR_BIT(off_cb_tmp, RANG_ACHR_CHAPV2);
      }
          
      //УРОВ1
      if(
         (_CHECK_SET_BIT(off_cb_tmp, RANG_UROV1) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_UROV1) == 0) /*умова, що сигнал тільки активується (щоб зафіксувати час старту)*/
        )   
      {
        _SET_BIT(info_vidkluchennja_vymykacha, VYMKNENNJA_VID_UROV1);
        info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_UROV1] = info_vymk_tmp;

        _CLEAR_BIT(off_cb_tmp, RANG_UROV1);
      }
      
      //УРОВ2
      if(
         (_CHECK_SET_BIT(off_cb_tmp, RANG_UROV2) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_UROV2) == 0) /*умова, що сигнал тільки активується (щоб зафіксувати час старту)*/
        )   
      {
        _SET_BIT(info_vidkluchennja_vymykacha, VYMKNENNJA_VID_UROV2);
        info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_UROV2] = info_vymk_tmp;

        _CLEAR_BIT(off_cb_tmp, RANG_UROV2);
      }
      
      //ЗОП
      if(
         (_CHECK_SET_BIT(off_cb_tmp, RANG_ZOP) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_ZOP) == 0) /*умова, що сигнал тільки активується (щоб зафіксувати час старту)*/
        )   
      {
        _SET_BIT(info_vidkluchennja_vymykacha, VYMKNENNJA_VID_ZOP);
        info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_ZOP] = info_vymk_tmp;

        _CLEAR_BIT(off_cb_tmp, RANG_ZOP);
      }
      
      //Umin1
      if(
         (_CHECK_SET_BIT(off_cb_tmp, RANG_UMIN1) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_UMIN1) == 0) /*умова, що сигнал тільки активується (щоб зафіксувати час старту)*/
        )   
      {
        _SET_BIT(info_vidkluchennja_vymykacha, VYMKNENNJA_VID_UMIN1);
        info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_UMIN1] = info_vymk_tmp;

        _CLEAR_BIT(off_cb_tmp, RANG_UMIN1);
      }
      
      //Umin2
      if(
         (_CHECK_SET_BIT(off_cb_tmp, RANG_UMIN2) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_UMIN2) == 0) /*умова, що сигнал тільки активується (щоб зафіксувати час старту)*/
        )   
      {
        _SET_BIT(info_vidkluchennja_vymykacha, VYMKNENNJA_VID_UMIN2);
        info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_UMIN2] = info_vymk_tmp;

        _CLEAR_BIT(off_cb_tmp, RANG_UMIN2);
      }
      
      //Umax1
      if(
         (_CHECK_SET_BIT(off_cb_tmp, RANG_UMAX1) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_UMAX1) == 0) /*умова, що сигнал тільки активується (щоб зафіксувати час старту)*/
        )   
      {
        _SET_BIT(info_vidkluchennja_vymykacha, VYMKNENNJA_VID_UMAX1);
        info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_UMAX1] = info_vymk_tmp;

        _CLEAR_BIT(off_cb_tmp, RANG_UMAX1);
      }
      
      //Umax2
      if(
         (_CHECK_SET_BIT(off_cb_tmp, RANG_UMAX2) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_UMAX2) == 0) /*умова, що сигнал тільки активується (щоб зафіксувати час старту)*/
        )   
      {
        _SET_BIT(info_vidkluchennja_vymykacha, VYMKNENNJA_VID_UMAX2);
        info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_UMAX2] = info_vymk_tmp;

        _CLEAR_BIT(off_cb_tmp, RANG_UMAX2);
      }
      
      //Універсальний захист
      for (size_t n_UP = 0; n_UP < NUMBER_UP; n_UP++)
      {
        if(
           (_CHECK_SET_BIT(off_cb_tmp, (RANG_UP1 + 3*n_UP)) != 0) &&
           (_CHECK_SET_BIT(previous_active_functions, (RANG_UP1 + 3*n_UP)) == 0) /*умова, що сигнал тільки активується (щоб зафіксувати час старту)*/
          )   
        {
          _SET_BIT(info_vidkluchennja_vymykacha, (VYMKNENNJA_VID_UP1 + n_UP));
          info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_UP1 + n_UP] = info_vymk_tmp;

          _CLEAR_BIT(off_cb_tmp, (RANG_UP1 + 3*n_UP));
        }
      }
      
      //Відключення від зовнішніх захистів
      if(
         (_CHECK_SET_BIT(off_cb_tmp, RANG_OTKL_VID_ZOVN_ZAHYSTIV) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_OTKL_VID_ZOVN_ZAHYSTIV) == 0) /*умова, що сигнал тільки активується (щоб зафіксувати час старту)*/
        )   
      {
        _SET_BIT(info_vidkluchennja_vymykacha, VYMKNENNJA_VID_ZOVNISHNIKH_ZAKHYSTIV);
        info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_ZOVNISHNIKH_ZAKHYSTIV] = info_vymk_tmp;

        _CLEAR_BIT(off_cb_tmp, RANG_OTKL_VID_ZOVN_ZAHYSTIV);
      }
      
      //Відключення від інших сигналів (крім відключення від сигналу "Вимк.ВВ")
      flag = 0;
      for(size_t m = 0; m < N_BIG; ++m) 
      {
        if(off_cb_tmp[m] != 0) 
        {
          flag = 1; 
          break;
        }
      }
      
      if (flag)
      {
        flag = 0;
        for(size_t m = 0; m < N_BIG; ++m) 
        {
          if(((previous_active_functions[m] & off_cb_tmp[m])!= off_cb_tmp[m])) 
          {
            flag = 1; 
            break;
          }
        }
        if(flag)   
        {
          _SET_BIT(info_vidkluchennja_vymykacha, VYMKNENNJA_VID_INSHYKH_SYGNALIV);
          info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_INSHYKH_SYGNALIV] = info_vymk_tmp;
        }
      }
      /*****************************************************/
    }
  }

  /*********************/
  //Потім розглядається блок включення
  /*********************/
  if (
      (global_timers[INDEX_TIMER_VIDKL_VV  ] < 0) && 
      (global_timers[INDEX_TIMER_BLK_VKL_VV] < 0) &&
      (_CHECK_SET_BIT(p_active_functions, RANG_BLOCK_VKL_VV) == 0)
     )
  {
    //Оскільки не працюють таймери БО і блокування включення БВ, а також немає сигналу блокування включення ВВ
    //тому перевіряємо, чи немає умови запуску БВ

    flag = 0;
    for(size_t m = 0; m < N_BIG; ++m) 
    {
      if((p_active_functions[m] & current_settings_prt.ranguvannja_on_cb[m]) != 0) 
      {
        flag = 1; 
        break;
      }
    }
    if (flag)
    {
      //Відмічаємо у масиві функцій, які зараз активуються, що ще треба активувати блок БВ (якщо він ще не активний)
      _SET_BIT(p_active_functions, RANG_WORK_BV);

      //Запускаємо (або продовжуємо утримувати у 0, поки не пропаде сигнал активації БВ) таймер роботи БВ
      global_timers[INDEX_TIMER_VKL_VV] = 0;
    }
  }
  else
  {
    //На даний момент існує одна або більше умов блокування БВ
    global_timers[INDEX_TIMER_VKL_VV] = -1;
    _CLEAR_BIT(p_active_functions, RANG_WORK_BV);
  }
  /*********************/

  /*********************/
  //Формуємо попереденій стан сигналів для функції увімкнення/вимкнення
  /*********************/
  for (unsigned int i = 0; i < N_BIG; i++) previous_active_functions[i] = p_active_functions[i];
  /*********************/
}
/*****************************************************/

/*****************************************************/
//Визначення місця до пошкодження
/*****************************************************/
inline void vmp_handler(unsigned int p_active_functions[])
{
  //Перевіряємо чи стоїть фіксуються спрацювання від КЗ на фазних лініях
  static unsigned int const maska_vmp_signals[N_BIG] =
  {
    MASKA_MONITOTYNG_PHASE_SIGNALES_KZ_0,
    MASKA_MONITOTYNG_PHASE_SIGNALES_KZ_1,
    MASKA_MONITOTYNG_PHASE_SIGNALES_KZ_2,
    MASKA_MONITOTYNG_PHASE_SIGNALES_KZ_3,
    MASKA_MONITOTYNG_PHASE_SIGNALES_KZ_4,
    MASKA_MONITOTYNG_PHASE_SIGNALES_KZ_5,
    MASKA_MONITOTYNG_PHASE_SIGNALES_KZ_6,
    MASKA_MONITOTYNG_PHASE_SIGNALES_KZ_7,
		MASKA_MONITOTYNG_PHASE_SIGNALES_KZ_8
      
#ifdef MASKA_MONITOTYNG_PHASE_SIGNALES_KZ_9
                                        ,
    MASKA_MONITOTYNG_PHASE_SIGNALES_KZ_9
#endif
  };
  
  unsigned int comp = false;
  COMPARE_NOT_ZERO_OR(comp, p_active_functions, maska_vmp_signals, N_BIG)
  
  if(comp)
  {
    //Є фазне КЗ
    VMP_last_KZ = UNDEF_VMP; /*Помічаємо, що визначення місця до пошкодження ще не визначене*/
    
    //Перевіряємо чи подається команда на вимкнення ВВ ("Робота БО")
    vymknennja_vid_KZ_prt |= (_CHECK_SET_BIT(p_active_functions, RANG_WORK_BO) != 0);

    
    //Фіксуємо максимальний фазний струм при КЗ
    unsigned int max_faze_current = measurement[IM_IA];
    if (max_faze_current < measurement[IM_IB]) max_faze_current = measurement[IM_IB];
    if (max_faze_current < measurement[IM_IC]) max_faze_current = measurement[IM_IC];
    
    if (max_faze_current > I_max_KZ_prt)
    {
      //Зафіксовано нове значення максимального фазного струму при КЗ
      I_max_KZ_prt = max_faze_current;
      
      //Визначаємо скільки фаз бере учать у КЗ
      unsigned int I_max_KZ_0_9 = (unsigned int)(0.9f*((float)max_faze_current));
      
      unsigned int number_of_phases_tmp = 0;
      if (measurement[IM_IA] >= I_max_KZ_0_9) number_of_phases_tmp++;
      if (measurement[IM_IB] >= I_max_KZ_0_9) number_of_phases_tmp++;
      if (measurement[IM_IC] >= I_max_KZ_0_9) number_of_phases_tmp++;
      if (number_of_phases_KZ_prt < number_of_phases_tmp) number_of_phases_KZ_prt = number_of_phases_tmp;
        
      int X_resistance[3] = {resistance[X_AB], resistance[X_BC], resistance[X_CA]};
      if (
          (((unsigned int)X_resistance[0]) != ((unsigned int)UNDEF_RESISTANCE)) ||
          (((unsigned int)X_resistance[1]) != ((unsigned int)UNDEF_RESISTANCE)) ||
          (((unsigned int)X_resistance[2]) != ((unsigned int)UNDEF_RESISTANCE))
         )
      {
        //Є реально розраховані міжфазні опори

        //Фіксуємо мінімальний міжфазний реактивний опір при КЗ
        //Етап 1: Знаходим перший визначений фіжфазний опір і помічаємо його значення як найменше
        unsigned int min_interphase_X = 0;
        int R_KZ_tmp = 0;
        if (((unsigned int)X_resistance[0]) != ((unsigned int)UNDEF_RESISTANCE))
        {
          min_interphase_X = abs(X_resistance[0]);
          R_KZ_tmp = resistance[R_AB];
        }
        else if (((unsigned int)X_resistance[1]) != ((unsigned int)UNDEF_RESISTANCE))
        {
          min_interphase_X = abs(X_resistance[1]);
          R_KZ_tmp = resistance[R_BC];
        }
        else if (((unsigned int)X_resistance[2]) != ((unsigned int)UNDEF_RESISTANCE))
        {
          min_interphase_X = abs(X_resistance[2]);
          R_KZ_tmp = resistance[R_CA];
        }
        else
        {
          //Теоретично цього ніколи не мало б бути
          total_error_sw_fixed();
        }
      
        //Етап 2: Серед всіх визначених міжфазних опорів знаходимо найменше
        unsigned int X_mod;
        if ((((unsigned int)X_resistance[0]) != ((unsigned int)UNDEF_RESISTANCE)) && (min_interphase_X > (X_mod = abs(X_resistance[0])))) 
        {
          min_interphase_X = X_mod;
          R_KZ_tmp = resistance[R_AB];
        }
        if ((((unsigned int)X_resistance[1]) != ((unsigned int)UNDEF_RESISTANCE)) && (min_interphase_X > (X_mod = abs(X_resistance[1]))))
        {
          min_interphase_X = X_mod;
          R_KZ_tmp = resistance[R_BC];
        }
        if ((((unsigned int)X_resistance[2]) != ((unsigned int)UNDEF_RESISTANCE)) && (min_interphase_X > (X_mod = abs(X_resistance[2]))))
        {
          min_interphase_X = X_mod;
          R_KZ_tmp = resistance[R_CA];
        }
      
        //Етап 3: Перевіряємо чи мінімальний опір на цей момент не є мінімальним з початку виникнення КЗ
        if (
            (min_interphase_X != ((unsigned int)UNDEF_RESISTANCE))
            &&
            (
             (X_min_KZ_prt == ((unsigned int)UNDEF_RESISTANCE)) || /*Це є ознакою, що для даного КЗ ми перший раз фіксуємо мінімальний опір, тому і його значення помічаємо як мінімальне*/
             (X_min_KZ_prt > min_interphase_X)
            )   
           )
        {
          //Зафіксовано нове значення мінімального міжфазного опору при КЗ
          X_min_KZ_prt = min_interphase_X;
          R_KZ_prt = R_KZ_tmp; //Поки що це число потрібно тільки для визначення знаку (щоб визначити у якому напямку відбулося КЗ)
        }
      }
      else
      {
        X_min_KZ_prt = (unsigned int)UNDEF_RESISTANCE;
      }
    }
  }
  else
  {
    //На даний момент немає фазного КЗ
    if(
       (vymknennja_vid_KZ_prt != 0) &&  /*Умова, що відбувалося вимкнення під час останнього КЗ*/
       ((number_of_phases_last_KZ = number_of_phases_KZ_prt) > 1) && /*Умова, що КЗ є міжфазним*/
       ((current_settings_prt.control_vmp & CTR_VMP_STATE) != 0) && /*ВМП увімкнено*/ 
       (X_min_KZ_prt != ((unsigned int)UNDEF_RESISTANCE)) /*Умова, що хоча б один міжфазний опір був визначений, а тому і є зафіксований мінімальний реактивний міжфазний опір*/
      )
    {
      //Визначаємо віддаль до мсця пошкодження поки що без напрямку 
      unsigned int X_tmp = X_min_KZ_prt*current_settings_prt.TVoltage/current_settings_prt.TCurrent; /*Перерозраховуємо опір на первинну обмотку. Значення залишаємо у мОм*/
      
      unsigned int forward_backward = (R_KZ_prt < 0);
      unsigned int lines_tmp = current_settings_prt.lines[forward_backward];
      unsigned int defined_VMP = false;
      unsigned int i = 0;
      int VMP_tmp = 0;
      while ((defined_VMP == false) && (i < lines_tmp))
      {
        unsigned int dovgyna_tmp = current_settings_prt.dovgyna[forward_backward][i];
        unsigned int opir_tmp = current_settings_prt.opir[forward_backward][i];
        unsigned int X_current_line = dovgyna_tmp*opir_tmp/1000; /*розмірності*[м]x[мОм/км] =  [м]x[мОм/1000м] =[мОм]/1000 */
        if (X_tmp <= X_current_line)
        {
          //На даній ділянці зафіксовано пошкодження
//          VMP_tmp += (int)(((unsigned long long)dovgyna_tmp)*((unsigned long long)X_tmp)/((unsigned long long)X_current_line));
          VMP_tmp += X_tmp*1000/opir_tmp;
          defined_VMP = true;
        }
        else
        {
          //На даній ділянці ще не зафіксовано пошкодження
          VMP_tmp += dovgyna_tmp;
          X_tmp -= X_current_line;
        }
          
        i++;
      }
      
      //Остаточно визначаємо ВМП
      equal_more_KZ = defined_VMP;
      if(forward_backward == 0) VMP_last_KZ = VMP_tmp;
      else VMP_last_KZ = -VMP_tmp;
    }

    /*
    Повертаємо пограмне забезпечення у стан очікування нового КЗ
    
    Цю операцію може б треба було виконувати тільки раз після попереднрього КЗ,
    але оскільки  я не фіксую наявнясть КЗ, а тільки вимкнення при КЗ (короткочасне КЗ
    може не привести до вимкнення, або якщо сигнали працювали на сигналізацію, а не
    на вимкнення) - то щоб не ускладнювати програму, я зробив ці операції постійними коли немає КЗ
    Я думаю, що їх не так багато і вони б не мали сильно знизити ресурс процесора
    */
    vymknennja_vid_KZ_prt = 0;
    I_max_KZ_prt = 0;
    number_of_phases_KZ_prt = 0;
    X_min_KZ_prt = (unsigned int)UNDEF_RESISTANCE;
    R_KZ_prt = 0;
  }
      
}
/*****************************************************/

/*****************************************************/
//Контроль приводу вимикача
/*****************************************************/
inline void control_VV(unsigned int *p_active_functions)
{
  unsigned int logic_control_VV_0 = 0;

  //"Контроль Вкл."
  logic_control_VV_0 |= (_CHECK_SET_BIT(p_active_functions, RANG_CTRL_VKL ) != 0) << 0;
  //"Контроль Откл."
  logic_control_VV_0 |= (_CHECK_SET_BIT(p_active_functions, RANG_CTRL_OTKL) != 0) << 1;
  
  _XOR_INVERTOR(logic_control_VV_0, 0, logic_control_VV_0, 1, logic_control_VV_0, 2);

  _TIMER_T_0(INDEX_TIMER_PRYVOD_VV, current_settings_prt.timeout_pryvoda_VV, logic_control_VV_0, 2, logic_control_VV_0, 3);

  //М:"Контроль ВВ"
  logic_control_VV_0 |= ((current_settings_prt.control_switch & CTR_PRYVOD_VV) != 0) << 4;
  
  _AND2(logic_control_VV_0, 3, logic_control_VV_0, 4, logic_control_VV_0, 5);
  
  if (_GET_STATE(logic_control_VV_0, 5))
    _SET_BIT(p_active_functions, RANG_PRYVID_VV);
  else
    _CLEAR_BIT(p_active_functions, RANG_PRYVID_VV);
}
/*****************************************************/

/*****************************************************/
//Розрахунок коефіцієнтрів для підрахунку ресурсу вимикача
/*****************************************************/
void make_koef_for_resurs(void)
{
  float tmp_1, tmp_2;
  
  tmp_1 = (float)current_settings.setpoint_r_kom_st_Inom/(float)current_settings.setpoint_r_kom_st_Inom_vymk;
  tmp_1  = log10f(tmp_1);

  tmp_2 = (float)current_settings.setpoint_Inom_vymk/(float)current_settings.setpoint_Inom;
  tmp_2  = log10f(tmp_2);
  
  //Помічаємо, що коефіцієнти для розрахунку ресурсу вимикача зараз будуть змінені
  koef_resurs_changed = CHANGED_ETAP_EXECUTION;

  K_resurs = tmp_1/tmp_2;
  
  //Помічаємо, що коефіцієнти для розрахунку ресурсу вимикача змінені і чекають на прийом системою захистів
  koef_resurs_changed = CHANGED_ETAP_ENDED;
}
/*****************************************************/

/*****************************************************/
//Лічильник ресурсу
/*****************************************************/
inline void resurs_vymykacha_handler(unsigned int *p_active_functions)
{
  static unsigned int previous_active_functions[N_BIG];
  
  if (restart_counter != 0)
  {
    //Треба подати команду очистки ресурсу вимикача
    resurs_vymykacha = current_settings_prt.setpoint_pochatkovyj_resurs;
    resurs_vidkljuchennja = current_settings_prt.setpoint_pochatkova_k_vymk;
    
     restart_counter = 0;

    //Запускаємо запис у EEPROM
    _SET_BIT(control_spi1_taskes, TASK_START_WRITE_RESURS_EEPROM_BIT);

    //Помічаємо, що відбулася очистка ресурсу вимикача
    information_about_restart_counter |= (
                                            (1 << USB_RECUEST)
                                          | (1 << RS485_RECUEST)
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                                          | (1 << LAN_RECUEST)
#endif
                                         );
  }
  
  /*******************************/
  //Визначаємо максимальний фазовий струм під час роботи Блоку Вимкнення (БО)
  /*******************************/
  static unsigned int max_faze_current_vv;
  if (_CHECK_SET_BIT(p_active_functions, RANG_WORK_BO) != 0)
  {
    unsigned int max_faze_current_tmp = measurement[IM_IA];
    if (max_faze_current_tmp < measurement[IM_IB]) max_faze_current_tmp = measurement[IM_IB];
    if (max_faze_current_tmp < measurement[IM_IC]) max_faze_current_tmp = measurement[IM_IC];
    max_faze_current_tmp = lroundf((float)(max_faze_current_tmp*current_settings_prt.TCurrent)/1000.0f);
  
    if (_CHECK_SET_BIT(previous_active_functions, RANG_WORK_BO) == 0)
    {
      //Зафіксовано старт роботи БО
      max_faze_current_vv = max_faze_current_tmp;
    }
    else if (_CHECK_SET_BIT(previous_active_functions, RANG_WORK_BO) != 0)
    {
      //БО продовжує свою роботу
      if (max_faze_current_vv < max_faze_current_tmp) max_faze_current_vv = max_faze_current_tmp;
    }
    
    if (max_faze_current_vv > current_settings_prt.setpoint_Inom_vymk) 
    {
      _SET_BIT(p_active_functions, RANG_PEREVYSHCHENNJA_Inom_VYMK);
    }
    else
    {
      _CLEAR_BIT(p_active_functions, RANG_PEREVYSHCHENNJA_Inom_VYMK);
    }
  }
  else _CLEAR_BIT(p_active_functions, RANG_PEREVYSHCHENNJA_Inom_VYMK);
  /*******************************/
  
  if (
      ((current_settings_prt.control_switch & CTR_RESURS_VV) != 0) &&
      (_CHECK_SET_BIT(previous_active_functions, RANG_WORK_BO) != 0) &&
      (_CHECK_SET_BIT(p_active_functions , RANG_WORK_BO) == 0)
     ) 
  {
    //Зафіксовано завершення "Робота БО"
    
    /*******************************/
    //Обчислюється ресурс вимикачa
    /*******************************/
    unsigned int N = 0;
    if (max_faze_current_vv >= current_settings_prt.setpoint_Inom_vymk) {
      N = (unsigned int) ceilf((float)current_settings_prt.setpoint_r_kom_st_Inom/(float)current_settings_prt.setpoint_r_kom_st_Inom_vymk);
    } else if (max_faze_current_vv <= current_settings_prt.setpoint_Inom) {
      N = 1;
    } else {
      float I_div_Inom = (float)max_faze_current_vv / (float)current_settings_prt.setpoint_Inom;
      float lg_I_div_Inom = log10f(I_div_Inom);
      float lg_N = K_resurs_prt * lg_I_div_Inom;
      float N_ = powf(10, lg_N);
      N = (unsigned int) ceilf(N_);
      if ((((float)N) - N_) >= 0.5f)
      {
        N--;
      }
      
    }
    
    if (resurs_vymykacha <= N) {
      resurs_vymykacha = 0;
    } else {
      resurs_vymykacha -= N;
    }
    
    //Загальне число переключень
    if ((0xffffffff - resurs_vidkljuchennja) >= 1)
    {
      //Додавання одиниці не викличе переповнення
      resurs_vidkljuchennja++;
    }

    //Запускаємо запис у EEPROM
    _SET_BIT(control_spi1_taskes, TASK_START_WRITE_RESURS_EEPROM_BIT);
  }
  
  if (resurs_vymykacha <= current_settings_prt.setpoint_krytychnyj_resurs) 
  {
    _SET_BIT(p_active_functions, RANG_KRYTYCHNYJ_RESURS_VYMYKACHA);
  }
  else
  {
    _CLEAR_BIT(p_active_functions, RANG_KRYTYCHNYJ_RESURS_VYMYKACHA);
  }

  if (resurs_vymykacha == 0) 
  {
    _SET_BIT(p_active_functions, RANG_VYCHERPANYJ_RESURS_VYMYKACHA);
  }
  else
  {
    _CLEAR_BIT(p_active_functions, RANG_VYCHERPANYJ_RESURS_VYMYKACHA);
  }
 

  /*********************/
  //Формуємо попереденій стан сигналів для функції увімкнення/вимкнення
  /*********************/
  for (unsigned int i = 0; i < N_BIG; i++) previous_active_functions[i] = p_active_functions[i];
  /*********************/
}
/*****************************************************/

/*****************************************************/
//Перевірка на необхідність завершення роботи аналогового/дискретного реєстраторів
/*****************************************************/
inline unsigned int stop_regisrator(unsigned int* carrent_active_functions, unsigned int* ranguvannja_registrator)
{
//  unsigned int stop = 0;
//  long tm_o = 0;

//  {
    if (global_timers[INDEX_TIMER_DR_WORK] < 0) global_timers[INDEX_TIMER_DR_WORK] = DELTA_TIME_FOR_TIMERS; /*вже, к мінімум з моменту запуску ДР процшло стільки мілісекунд*/
    
    int flag = 0;
    for(size_t m = 0; m < N_BIG; ++m) 
    {
      if((carrent_active_functions[m] & ranguvannja_registrator[m]) != 0) 
      {
        flag = (1u << 0);
        break;
      }
    }
    /*
    if (flag)
    {
      //Зафіксовано, що ні одне джерело активації реєстратора зараз не активне
      
        //if (global_timers[INDEX_TIMER_DR_WORK] == 0)
        _TIMER_0_T(INDEX_TIMER_DR_WORK,current_settings_prt.timeout_prolongation_work_digital_registrator,0,0,tm_o,1);	
        if ( tm_o == 0)
        {
          //Зафіксовано, що  таймер  неактивний
           //.asm volatile(
           //.    "bkpt 1"
           //.    );
          //Помічаємо, що реєстратор може бути зупиненим
          stop = 0xff;
        }
    }
    else{
		tm_o = 1;
		_TIMER_0_T(INDEX_TIMER_DR_WORK,current_settings_prt.timeout_prolongation_work_digital_registrator,1,0,tm_o,1);
	}
    */
    _TIMER_0_T(INDEX_TIMER_DR_WORK, current_settings_prt.timeout_prolongation_work_digital_registrator + DELTA_TIME_FOR_TIMERS, flag, 0, flag, 1);
//  }
  
  return (flag == 0);
}
/*****************************************************/

/*****************************************************/
//Зафіксована невизначена помилка роботи дискретного реєстратора
/*****************************************************/
inline void fix_undefined_error_dr(unsigned int* carrent_active_functions)
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
inline unsigned char * serialization_uint_uchar(unsigned int const * const p_input, unsigned char * p_output, size_t const cut)
{
  unsigned char const *p_data = (unsigned char const *)(p_input);
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
inline unsigned int *  deserialization_uchar_uint(unsigned char const * p_input, unsigned int * p_output)
{
  *p_output = 0;
  for (size_t i = 0; i < sizeof(unsigned int); ++i)
  {
    *p_output |= (*p_input++) << (8*i);
  }
  return ++p_output;
}
/*****************************************************/

/*****************************************************/
//Завершення моніторингу максимального струму
/*****************************************************/
inline void end_monitoring_min_max_measurement(unsigned char ** const arr_identifiers, unsigned char const * const buffer_for_save_dr_record, unsigned int const type_current, unsigned int* const carrent_active_functions)
{
  if(
     (
      (type_current == IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE     ) ||
      (type_current == IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE04   ) ||
      (type_current == IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZNAM      ) ||
      (type_current == IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0_1G    ) ||
      (type_current == IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0_OTHERG) ||
      (type_current == IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0_r     ) ||
      (type_current == IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE_3U0       ) ||
      (type_current == IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE           ) ||
      (type_current == IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE           ) ||
      (type_current == IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZOP       ) ||
      (type_current == IDENTIFIER_BIT_ARRAY_MIN_FREQUENCY_ACHR    ) ||
      (type_current == IDENTIFIER_BIT_ARRAY_FREQUENCY_CHAPV       )
     )
     && 
     (arr_identifiers[type_current - 1] != NULL)
    )
  {
    unsigned char * buffer = arr_identifiers[type_current - 1];
    if (
        (buffer >= (buffer_for_save_dr_record + FIRST_INDEX_FIRST_BLOCK_DR)) && 
        ((buffer + SIZE_ARRAY_FIX_MAX_MEASUREMENTS*sizeof(unsigned int) - 1) < (buffer_for_save_dr_record + FIRST_INDEX_FIRST_DATA_DR))  
       )    
    {
      
      if(type_current == IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE)
      {
        //ВМП
        buffer += IDM_VMP_last_KZ*sizeof(unsigned int);
        unsigned int const vmp_start_val_local[2] = {(unsigned int)VMP_last_KZ, equal_more_KZ};
        unsigned int const * p_arr = vmp_start_val_local;
        for (size_t i = 0; i < 2; ++i)
        {
          buffer = serialization_uint_uchar(p_arr++, buffer, 0);
        }
      }

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
//Початок моніторингу максимального фазного струму 
/*****************************************************/
inline void start_monitoring_max_phase_current(unsigned int const time_tmp, unsigned char * const buffer_for_save_dr_record, unsigned char ** const arr_identifiers, unsigned char ** const p_next_free_array, unsigned int* const carrent_active_functions)
{
  unsigned char * buffer = *p_next_free_array;
  if (
      (arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE - 1] == NULL) &&
      (buffer >= (buffer_for_save_dr_record + FIRST_INDEX_FIRST_BLOCK_DR)) && 
      ((buffer + SIZE_ARRAY_FIX_MAX_MEASUREMENTS*sizeof(unsigned int) - 1) < (buffer_for_save_dr_record + FIRST_INDEX_FIRST_DATA_DR))  
     )   
  {
    //Збільшуємо кількість фіксованих значень максимального фазного струму
    ++number_max_phase_dr;
    arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE - 1] = buffer;
    *p_next_free_array += SIZE_ARRAY_FIX_MAX_MEASUREMENTS*sizeof(unsigned int);
  
    //Помічаємо, що будем виходити з того, що зараз значення тільки починають моніторитися, тому приймаємо їх за найбільші
    __meas_to_d_meas const * p_link = im_to_idm;
    for(size_t i = 0; i <_SIZE_ARRAY_FIX_MAX_MEASUREMENTS_TMP; ++i)
    {
      buffer = serialization_uint_uchar(&(p_link->arr[p_link->ind]), buffer, 0);
      
      ++p_link;
    }
  
    //Помічаємо, що будем виходити з того, що зараз значення тільки починають моніторитися, тому приймаємо їх за найбільші
    int frequency_int = (int)frequency;
    if (frequency_int >= 0) frequency_int = (int)(frequency*1000);
    buffer = serialization_uint_uchar((unsigned int *)(&frequency_int), buffer, 0);

    //ВМП
    unsigned int const * p_arr = vmp_start_val;
    for (size_t i = 0; i < 2; ++i)
    {
      buffer = serialization_uint_uchar(p_arr++, buffer, 0);
    }
  
    //Фіксуємо час з моменту початку аварійного запису
    buffer = serialization_uint_uchar(&time_tmp, buffer, 1);

    //Фіксуємо причину запису
    *buffer = IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE;

    /***
    Визначаємо макисальний фазний струм між трьома фазами
    ***/
    unsigned int tmp_data[3] = {0, 0, 0};
    buffer = arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE - 1];
    deserialization_uchar_uint(&buffer[IDM_IA*sizeof(unsigned int)], &tmp_data[0]);
    deserialization_uchar_uint(&buffer[IDM_IB*sizeof(unsigned int)], &tmp_data[1]);
    deserialization_uchar_uint(&buffer[IDM_IC*sizeof(unsigned int)], &tmp_data[2]);
    max_phase_current_dr = tmp_data[0];
    if (max_phase_current_dr < tmp_data[1]) max_phase_current_dr = tmp_data[1];
    if (max_phase_current_dr < tmp_data[2]) max_phase_current_dr = tmp_data[2];
    /***/

//    //Помічаємо, що ми на стадії моніторингу максимального фазного струму
//    state_current_monitoring |= (1<<IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE);
  }
  else
  {
    //Відбулася незрозуміла ситуація - сюди програма теоретично ніколи не мала б заходити
    fix_undefined_error_dr(carrent_active_functions);
  }
}
/*****************************************************/

/*****************************************************/
//Продовження моніторингу максимального фазного струму
/*****************************************************/
inline void continue_monitoring_max_phase_current(unsigned int const time_tmp, unsigned char * const buffer_for_save_dr_record, unsigned char ** const arr_identifiers, unsigned int* const carrent_active_functions)
{
  //Перевірка, чи не є зарза фазний струм більший, ніж той що помічений максимальним
  if (
      (max_phase_current_dr < measurement[IM_IA]) ||
      (max_phase_current_dr < measurement[IM_IB]) ||
      (max_phase_current_dr < measurement[IM_IC])
     )
  {
    unsigned char * buffer = arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE - 1];
    if (
        (buffer >= (buffer_for_save_dr_record + FIRST_INDEX_FIRST_BLOCK_DR)) && 
        ((buffer + SIZE_ARRAY_FIX_MAX_MEASUREMENTS*sizeof(unsigned int) - 1) < (buffer_for_save_dr_record + FIRST_INDEX_FIRST_DATA_DR))  
      )   
    {
      __meas_to_d_meas const * p_link = im_to_idm;
      for(size_t i = 0; i <_SIZE_ARRAY_FIX_MAX_MEASUREMENTS_TMP; ++i)
      {
        buffer = serialization_uint_uchar(&(p_link->arr[p_link->ind]), buffer, 0);
      
        ++p_link;
      }

      //Зафіксовано зріз при найвищому фазовому струмі з моменту початку спостереження за ним
      int frequency_int = (int)frequency;
      if (frequency_int >= 0) frequency_int = (int)(frequency*1000);
      buffer = serialization_uint_uchar((unsigned int *)(&frequency_int), buffer, 0);
  
      //Фіксуємо час з моменту початку аварійного запису
      buffer = arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE - 1] + IDM_TM_STAMPE*sizeof(unsigned int);
      buffer = serialization_uint_uchar(&time_tmp, buffer, 1);

      unsigned int tmp_data[3] = {0, 0, 0};
      buffer = arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE - 1];
      deserialization_uchar_uint(&buffer[IDM_IA*sizeof(unsigned int)], &tmp_data[0]);
      deserialization_uchar_uint(&buffer[IDM_IB*sizeof(unsigned int)], &tmp_data[1]);
      deserialization_uchar_uint(&buffer[IDM_IC*sizeof(unsigned int)], &tmp_data[2]);
      max_phase_current_dr = tmp_data[0];
      if (max_phase_current_dr < tmp_data[1]) max_phase_current_dr = tmp_data[1];
      if (max_phase_current_dr < tmp_data[2]) max_phase_current_dr = tmp_data[2];
    }
    else
    {
      //Відбулася незрозуміла ситуація - сюди програма теоретично ніколи не мала б заходити
      fix_undefined_error_dr(carrent_active_functions);
    }
  }
}
/*****************************************************/

/*****************************************************/
//Початок моніторингу максимального фазного струму зі сторони 0.4кВ
/*****************************************************/
inline void start_monitoring_max_phase04_current(unsigned int const time_tmp, unsigned char * const buffer_for_save_dr_record, unsigned char ** const arr_identifiers, unsigned char ** const p_next_free_array, unsigned int* const carrent_active_functions)
{
  unsigned char * buffer = *p_next_free_array;
  if (
      (arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE04 - 1] == NULL) &&
      (buffer >= (buffer_for_save_dr_record + FIRST_INDEX_FIRST_BLOCK_DR)) && 
      ((buffer + SIZE_ARRAY_FIX_MAX_MEASUREMENTS*sizeof(unsigned int) - 1) < (buffer_for_save_dr_record + FIRST_INDEX_FIRST_DATA_DR))  
     )   
  {
    //Збільшуємо кількість фіксованих значень максимального фазного струму зі сторони 0.4кВ
    ++number_max_phase04_dr;
    arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE04 - 1] = buffer;
    *p_next_free_array += SIZE_ARRAY_FIX_MAX_MEASUREMENTS*sizeof(unsigned int);
  
    //Помічаємо, що будем виходити з того, що зараз значення тільки починають моніторитися, тому приймаємо їх за найбільші
    __meas_to_d_meas const * p_link = im_to_idm;
    for(size_t i = 0; i <_SIZE_ARRAY_FIX_MAX_MEASUREMENTS_TMP; ++i)
    {
      buffer = serialization_uint_uchar(&(p_link->arr[p_link->ind]), buffer, 0);
      
      ++p_link;
    }
  
    //Помічаємо, що будем виходити з того, що зараз значення тільки починають моніторитися, тому приймаємо їх за найбільші
    int frequency_int = (int)frequency;
    if (frequency_int >= 0) frequency_int = (int)(frequency*1000);
    buffer = serialization_uint_uchar((unsigned int *)(&frequency_int), buffer, 0);

    //ВМП
    unsigned int const * p_arr = vmp_start_val;
    for (size_t i = 0; i < 2; ++i)
    {
      buffer = serialization_uint_uchar(p_arr++, buffer, 0);
    }
  
    //Фіксуємо час з моменту початку аварійного запису
    buffer = serialization_uint_uchar(&time_tmp, buffer, 1);

    //Фіксуємо причину запису
    *buffer = IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE04;

//    //Помічаємо, що ми на стадії моніторингу максимального фазного струму
//    state_current_monitoring |= (1<<IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE04);
  }
  else
  {
    //Відбулася незрозуміла ситуація - сюди програма теоретично ніколи не мала б заходити
    fix_undefined_error_dr(carrent_active_functions);
  }
}
/*****************************************************/

/*****************************************************/
//Продовження моніторингу максимального фазного струму зі сторони 0.4кВ
/*****************************************************/
inline void continue_monitoring_max_phase04_current(unsigned int const time_tmp, unsigned char * const buffer_for_save_dr_record, unsigned char ** const arr_identifiers, unsigned int* const carrent_active_functions)
{
  unsigned char * buffer = arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE04 - 1];
  if (
      (buffer >= (buffer_for_save_dr_record + FIRST_INDEX_FIRST_BLOCK_DR)) && 
      ((buffer + SIZE_ARRAY_FIX_MAX_MEASUREMENTS*sizeof(unsigned int) - 1) < (buffer_for_save_dr_record + FIRST_INDEX_FIRST_DATA_DR))  
    )   
  {
    //Перевірка, чи не є зарза фазний струм більший, ніж той що помічений максимальним
    unsigned int tmp_data = 0;
    deserialization_uchar_uint(&buffer[IDM_I04*sizeof(unsigned int)], &tmp_data);
    if(tmp_data < measurement[IM_I04]) 
    {
      __meas_to_d_meas const * p_link = im_to_idm;
      for(size_t i = 0; i <_SIZE_ARRAY_FIX_MAX_MEASUREMENTS_TMP; ++i)
      {
        buffer = serialization_uint_uchar(&(p_link->arr[p_link->ind]), buffer, 0);
      
        ++p_link;
      }

      //Зафіксовано зріз при найвищому фазовому струмі з моменту початку спостереження за ним
      int frequency_int = (int)frequency;
      if (frequency_int >= 0) frequency_int = (int)(frequency*1000);
      buffer = serialization_uint_uchar((unsigned int *)(&frequency_int), buffer, 0);
  
      //Фіксуємо час з моменту початку аварійного запису
      buffer = arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE04 - 1] + IDM_TM_STAMPE*sizeof(unsigned int);
      buffer = serialization_uint_uchar(&time_tmp, buffer, 1);
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
//Початок моніторингу максимального відношення фаного струму другої гармоніки до фазного струму першої гармоніки
/*****************************************************/
inline void start_monitoring_max_ZNAM(unsigned int const time_tmp, unsigned char * const buffer_for_save_dr_record, unsigned char ** const arr_identifiers, unsigned char ** const p_next_free_array, unsigned int* const carrent_active_functions)
{
  //Визначаємо макисальний фазний струм між трьома фазами
/*
  _I2g - струм другої гармоніки (базового "комплекту")
  _I1g - струм першої гармоніки (базового "комплекту")
      
  I2g -  струм другої гармоніки (порівнюваного "комплекту")
  I1g -  струм першої гармоніки (порівнюваного "комплекту")
      
  Умова перезапису параметрів:
  _I2g     I2g
  ---    < --- /x на _I1g*I1g  
  _I1g     Ig1
      
  (_I2g*I1g < I2g*_I1g)  - це є умова, що зараз відношення порівнювального "комплекту" є більшим з базовий комплект
*/

  max_I2g_to_I1g = -1; /*це означає, що поки що не зафіксовано пару з найбільшим відношенням*/
  if ((i1g_bilshe_porogu[0] != 0) && (i2g_bilshe_porogu[0] != 0)) max_I2g_to_I1g = 0;
  
  unsigned int I2g, I1g, _I1g, _I2g;
  _I2g = measurement[IM2_IA];
  _I1g = measurement[IM_IA];

  I2g = measurement[IM2_IB];
  I1g = measurement[IM_IB];
  if (
      ((i1g_bilshe_porogu[1] != 0) && (i2g_bilshe_porogu[1] != 0)) &&
      (
       (max_I2g_to_I1g < 0) ||
       (_I2g*I1g < I2g*_I1g)  
      )
     )
  {
    max_I2g_to_I1g = 1;

    _I2g = I2g;
    _I1g = I1g;
  }

  I2g = measurement[IM2_IC];
  I1g = measurement[IM_IC];
  if (
      ((i1g_bilshe_porogu[2] != 0) && (i2g_bilshe_porogu[2] != 0)) &&
      (
       (max_I2g_to_I1g < 0) ||
       (_I2g*I1g < I2g*_I1g)  
      )
     )
  {
    max_I2g_to_I1g = 2;
  }
  
	if (max_I2g_to_I1g >= 0)
  {
    unsigned char * buffer = *p_next_free_array;
    if (
        (arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZNAM - 1] == NULL) &&
        (buffer >= (buffer_for_save_dr_record + FIRST_INDEX_FIRST_BLOCK_DR)) && 
        ((buffer + SIZE_ARRAY_FIX_MAX_MEASUREMENTS*sizeof(unsigned int) - 1) < (buffer_for_save_dr_record + FIRST_INDEX_FIRST_DATA_DR))  
      )   
    {
      //Збільшуємо кількість фіксованих значень максимального відношення другої гармоніки до першої
      number_max_ZNAM_dr++;
      arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZNAM - 1] = buffer;
      *p_next_free_array += SIZE_ARRAY_FIX_MAX_MEASUREMENTS*sizeof(unsigned int);
  
      //Помічаємо, що будем виходити з того, що зараз значення тільки починають моніторитися, тому приймаємо їх за найбільші
      __meas_to_d_meas const * p_link = im_to_idm;
      for(size_t i = 0; i <_SIZE_ARRAY_FIX_MAX_MEASUREMENTS_TMP; ++i)
      {
        buffer = serialization_uint_uchar(&(p_link->arr[p_link->ind]), buffer, 0);
      
        ++p_link;
      }
  
      //Помічаємо, що будем виходити з того, що зараз значення тільки починають моніторитися, тому приймаємо їх за найбільші
      int frequency_int = (int)frequency;
      if (frequency_int >= 0) frequency_int = (int)(frequency*1000);
      buffer = serialization_uint_uchar((unsigned int *)(&frequency_int), buffer, 0);

      //ВМП
      unsigned int const * p_arr = vmp_start_val;
      for (size_t i = 0; i < 2; ++i)
      {
        buffer = serialization_uint_uchar(p_arr++, buffer, 0);
      }
  
      //Фіксуємо час з моменту початку аварійного запису
      buffer = serialization_uint_uchar(&time_tmp, buffer, 1);

      //Фіксуємо причину запису
      *buffer = IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZNAM;

//      //Помічаємо, що ми на стадії моніторингу максимального фазного струму
//      state_current_monitoring |= (1<<IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZNAM);
    }
    else
    {
      //Відбулася незрозуміла ситуація - сюди програма теоретично ніколи не мала б заходити
      fix_undefined_error_dr(carrent_active_functions);
    }
  }
}
/*****************************************************/

/*****************************************************/
//Продовження моніторингу максимального відношення фаного струму другої гармоніки до фазного струму першої гармоніки
/*****************************************************/
inline void continue_monitoring_max_ZNAM(unsigned int const time_tmp, unsigned char * const buffer_for_save_dr_record, unsigned char ** const arr_identifiers, unsigned int* const carrent_active_functions)
{
  unsigned char * buffer = arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZNAM - 1];
  if (
      (buffer >= (buffer_for_save_dr_record + FIRST_INDEX_FIRST_BLOCK_DR)) && 
      ((buffer + SIZE_ARRAY_FIX_MAX_MEASUREMENTS*sizeof(unsigned int) - 1) < (buffer_for_save_dr_record + FIRST_INDEX_FIRST_DATA_DR))  
    )   
  {
    //Перевірка, чи не є зарза струм зворотньої послідовності більший, ніж той що помічений максимальним
/*
    _I2g - струм другої гармоніки (базового "комплекту")
    _I1g - струм першої гармоніки (базового "комплекту")
      
    I2g -  струм другої гармоніки (порівнюваного "комплекту")
    I1g -  струм першої гармоніки (порівнюваного "комплекту")
      
    Умова перезапису параметрів:
    _I2g     I2g
    ---    < --- /x на _I1g*I1g  
    _I1g     Ig1
      
    (_I2g*I1g < I2g*_I1g)  - це є умова, що зараз відношення порівнювального "комплекту" є більшим з базовий комплект
*/
    unsigned int I2g = 0, I1g = 0, _I1g = 0, _I2g = 0;
    int max_I2g_to_I1g_tmp = -1;
    unsigned int new_fix = false;
  
    switch (max_I2g_to_I1g)
    {
    case -1:
      {
        if ((i1g_bilshe_porogu[0] != 0) && (i2g_bilshe_porogu[0] != 0))
        {
          max_I2g_to_I1g_tmp = 0;
          _I2g = measurement[IM2_IA];
          _I1g = measurement[IM_IA];
        }
        else if ((i1g_bilshe_porogu[1] != 0) && (i2g_bilshe_porogu[1] != 0))
        {
          max_I2g_to_I1g_tmp = 1;
          _I2g = measurement[IM2_IB];
          _I1g = measurement[IM_IB];
        }
        else if ((i1g_bilshe_porogu[2] != 0) && (i2g_bilshe_porogu[2] != 0))
        {
          max_I2g_to_I1g_tmp = 2;
          _I2g = measurement[IM2_IC];
          _I1g = measurement[IM_IC];
        }
      
        if (max_I2g_to_I1g_tmp >= 0) new_fix = true;
      
        break;
      }
    case 0:
      {
        deserialization_uchar_uint(&buffer[IDM_IA *sizeof(unsigned int)], &_I1g);
        deserialization_uchar_uint(&buffer[IDM2_IA*sizeof(unsigned int)], &_I2g);
      
        break;
      }
    case 1:
      {
        deserialization_uchar_uint(&buffer[IDM_IB *sizeof(unsigned int)], &_I1g);
        deserialization_uchar_uint(&buffer[IDM2_IB*sizeof(unsigned int)], &_I2g);
      
        break;
      }
    case 2:
      {
        deserialization_uchar_uint(&buffer[IDM_IC *sizeof(unsigned int)], &_I1g);
        deserialization_uchar_uint(&buffer[IDM2_IC*sizeof(unsigned int)], &_I2g);
      
        break;
      }
    default:
      {
        //Теоретично цього ніколи не мало б бути
        total_error_sw_fixed();
      }
    }
  
    if ((max_I2g_to_I1g >= 0) || (max_I2g_to_I1g_tmp >= 0))
    {
      if (max_I2g_to_I1g < 0) max_I2g_to_I1g = max_I2g_to_I1g_tmp;
      for (intptr_t i = (max_I2g_to_I1g_tmp + 1); i < NUMBER_I2G_CANALES; i++)
      {
        switch (i)
        {
        case 0:
          {
            I2g = measurement[IM2_IA];
            I1g = measurement[IM_IA];
      
            break;
          }
        case 1:
          {
            I2g = measurement[IM2_IB];
            I1g = measurement[IM_IB];
      
            break;
          }
        case 2:
          {
            I2g = measurement[IM2_IC];
            I1g = measurement[IM_IC];
      
            break;
          }
        default:
          {
            //Теоретично цього ніколи не мало б бути
            total_error_sw_fixed();
          }
        }
      
        if ((i1g_bilshe_porogu[i] != 0) && (i2g_bilshe_porogu[i] != 0))
        {
          if (_I2g*I1g < I2g*_I1g)
          {
            new_fix = true;
            max_I2g_to_I1g = i;
          
            _I2g = I2g;
            _I1g = I1g;
          }
        }
      }
    }
  
    if (new_fix != false)
    {
      __meas_to_d_meas const * p_link = im_to_idm;
      for(size_t i = 0; i <_SIZE_ARRAY_FIX_MAX_MEASUREMENTS_TMP; ++i)
      {
        buffer = serialization_uint_uchar(&(p_link->arr[p_link->ind]), buffer, 0);
      
        ++p_link;
      }

      //Зафіксовано зріз при найвищому фазовому струмі з моменту початку спостереження за ним
      int frequency_int = (int)frequency;
      if (frequency_int >= 0) frequency_int = (int)(frequency*1000);
      buffer = serialization_uint_uchar((unsigned int *)(&frequency_int), buffer, 0);
  
      //Фіксуємо час з моменту початку аварійного запису
      buffer = arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZNAM - 1] + IDM_TM_STAMPE*sizeof(unsigned int);
      buffer = serialization_uint_uchar(&time_tmp, buffer, 1);
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
//Моніторинг максимального струму 3I0
/*****************************************************/
inline void monitoring_max_3I0(unsigned int const time_tmp, unsigned char * const buffer_for_save_dr_record, unsigned char ** const arr_identifiers, unsigned char ** const p_next_free_array, unsigned int* const carrent_active_functions)
{
//  unsigned char * buffer = *p_next_free_array;
//  if (
//      (buffer >= (buffer_for_save_dr_record + FIRST_INDEX_FIRST_BLOCK_DR)) && 
//      ((buffer + SIZE_ARRAY_FIX_MAX_MEASUREMENTS*sizeof(unsigned int) - 1) < (buffer_for_save_dr_record + FIRST_INDEX_FIRST_DATA_DR))  
//     )   
  {
  
    //Перша гармоніка
    unsigned int comp = false;
    if(!_GET_STATE(current_settings_prt.control_zz, INDEX_ML_CTR_ZZ1_TYPE_BIT))
    {
      COMPARE_NOT_ZERO_OR(comp, carrent_active_functions, monitoring_3I0_1g_signals, N_BIG)
    }
    if(comp)
    {
      //Стоїть умова моніторити першу гармоніку
      
      unsigned char * buffer = arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0_1G - 1];
      unsigned int tmp_data = 0;
      if (buffer != NULL)
      {
        if (
            (buffer >= (buffer_for_save_dr_record + FIRST_INDEX_FIRST_BLOCK_DR)) && 
            ((buffer + SIZE_ARRAY_FIX_MAX_MEASUREMENTS*sizeof(unsigned int) - 1) < (buffer_for_save_dr_record + FIRST_INDEX_FIRST_DATA_DR))  
           )
        {
          deserialization_uchar_uint(&buffer[IDM_3I0*sizeof(unsigned int)], &tmp_data);
        }
        else
        {
          //Відбулася незрозуміла ситуація - сюди програма теоретично ніколи не мала б заходити
          fix_undefined_error_dr(carrent_active_functions);
          
          return;
        }
      }
      
      if (
          (buffer == NULL) ||
          (tmp_data < measurement[IM_3I0])  
         )   
      {
        if (buffer == NULL)
        {
          //Треба почати моніторинг
          buffer = *p_next_free_array;
          if (
              (buffer >= (buffer_for_save_dr_record + FIRST_INDEX_FIRST_BLOCK_DR)) && 
              ((buffer + SIZE_ARRAY_FIX_MAX_MEASUREMENTS*sizeof(unsigned int) - 1) < (buffer_for_save_dr_record + FIRST_INDEX_FIRST_DATA_DR))  
             )   
          {
            //Збільшуємо кількість фіксованих значень максимального 3I0
            ++number_max_3I0_dr_1g;
            arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0_1G - 1] = buffer;
            *p_next_free_array += SIZE_ARRAY_FIX_MAX_MEASUREMENTS*sizeof(unsigned int);

            //ВМП
            unsigned int const * p_arr = vmp_start_val;
            buffer +=IDM_VMP_last_KZ*sizeof(unsigned int);
            for (size_t i = 0; i < 2; ++i)
            {
              buffer = serialization_uint_uchar(p_arr++, buffer, 0);
            }
//        
//            //Помічаємо, що ми на стадії моніторингу максимального струму 3I0 першої гармоніки
//            _SET_STATE(state_current_monitoring, IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0_1G);
          }
          else
          {
            //Відбулася незрозуміла ситуація - сюди програма теоретично ніколи не мала б заходити
            fix_undefined_error_dr(carrent_active_functions);
            
            return;
          }
        }
  
        buffer = arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0_1G - 1];
        __meas_to_d_meas const * p_link = im_to_idm;
        for(size_t i = 0; i <_SIZE_ARRAY_FIX_MAX_MEASUREMENTS_TMP; ++i)
        {
          buffer = serialization_uint_uchar(&(p_link->arr[p_link->ind]), buffer, 0);
      
          ++p_link;
        }

        //Зафіксовано зріз при найвищому фазовому струмі з моменту початку спостереження за ним
        int frequency_int = (int)frequency;
        if (frequency_int >= 0) frequency_int = (int)(frequency*1000);
        buffer = serialization_uint_uchar((unsigned int *)(&frequency_int), buffer, 0);
  
        //Фіксуємо час з моменту початку аварійного запису
        buffer = arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0_1G - 1] + IDM_TM_STAMPE*sizeof(unsigned int);
        buffer = serialization_uint_uchar(&time_tmp, buffer, 1);


        //Фіксуємо причину запису
        *buffer = IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0_1G;
      }
    }
    else if (arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0_1G - 1] != NULL)
    {
      //Стоїть умова зупинити моніторинг першої гармоніки
      end_monitoring_min_max_measurement(arr_identifiers, buffer_for_save_dr_record, IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0_1G, carrent_active_functions);
    }

    //Вищі гармоніка
    comp = false;
    if(_GET_STATE(current_settings_prt.control_zz, INDEX_ML_CTR_ZZ1_TYPE_BIT))
    {
      COMPARE_NOT_ZERO_OR(comp, carrent_active_functions, monitoring_3I0_otherg_signals, N_BIG)
    }
    if(comp)
    {
      //Стоїть умова моніторити по вищих гармоніках
      
      unsigned char * buffer = arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0_OTHERG - 1];
      unsigned int tmp_data = 0;
      if (buffer != NULL)
      {
        if (
            (buffer >= (buffer_for_save_dr_record + FIRST_INDEX_FIRST_BLOCK_DR)) && 
            ((buffer + SIZE_ARRAY_FIX_MAX_MEASUREMENTS*sizeof(unsigned int) - 1) < (buffer_for_save_dr_record + FIRST_INDEX_FIRST_DATA_DR))  
           )
        {
          deserialization_uchar_uint(&buffer[IDM_3I0_other_g*sizeof(unsigned int)], &tmp_data);
        }
        else
        {
          //Відбулася незрозуміла ситуація - сюди програма теоретично ніколи не мала б заходити
          fix_undefined_error_dr(carrent_active_functions);
          
          return;
        }
      }
      
      if (
          (buffer == NULL) ||
          (tmp_data < measurement[IM_3I0_other_g])  
         )   
      {
        if (buffer == NULL)
        {
          //Треба почати моніторинг
          buffer = *p_next_free_array;
          if (
              (buffer >= (buffer_for_save_dr_record + FIRST_INDEX_FIRST_BLOCK_DR)) && 
              ((buffer + SIZE_ARRAY_FIX_MAX_MEASUREMENTS*sizeof(unsigned int) - 1) < (buffer_for_save_dr_record + FIRST_INDEX_FIRST_DATA_DR))  
             )   
          {
            //Збільшуємо кількість фіксованих значень максимального 3I0
            ++number_max_3I0_dr_otherg;
            arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0_OTHERG - 1] = buffer;
            *p_next_free_array += SIZE_ARRAY_FIX_MAX_MEASUREMENTS*sizeof(unsigned int);

            //ВМП
            unsigned int const * p_arr = vmp_start_val;
            buffer +=IDM_VMP_last_KZ*sizeof(unsigned int);
            for (size_t i = 0; i < 2; ++i)
            {
              buffer = serialization_uint_uchar(p_arr++, buffer, 0);
            }

//            //Помічаємо, що ми на стадії моніторингу максимального струму 3I0 вищих гармонік
//            _SET_STATE(state_current_monitoring, IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0_OTHERG);
          }
          else
          {
            //Відбулася незрозуміла ситуація - сюди програма теоретично ніколи не мала б заходити
            fix_undefined_error_dr(carrent_active_functions);
            
            return;
          }
        }
  
        buffer = arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0_OTHERG - 1];
        __meas_to_d_meas const * p_link = im_to_idm;
        for(size_t i = 0; i <_SIZE_ARRAY_FIX_MAX_MEASUREMENTS_TMP; ++i)
        {
          buffer = serialization_uint_uchar(&(p_link->arr[p_link->ind]), buffer, 0);
      
          ++p_link;
        }

        //Зафіксовано зріз при найвищому фазовому струмі з моменту початку спостереження за ним
        int frequency_int = (int)frequency;
        if (frequency_int >= 0) frequency_int = (int)(frequency*1000);
        buffer = serialization_uint_uchar((unsigned int *)(&frequency_int), buffer, 0);
  
        //Фіксуємо час з моменту початку аварійного запису
        buffer = arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0_OTHERG - 1] + IDM_TM_STAMPE*sizeof(unsigned int);
        buffer = serialization_uint_uchar(&time_tmp, buffer, 1);


        //Фіксуємо причину запису
        *buffer = IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0_OTHERG;
      }
    }
    else if (arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0_OTHERG - 1] != NULL)
    {
      //Стоїть умова зупинити моніторинг вищих гармонік
      end_monitoring_min_max_measurement(arr_identifiers, buffer_for_save_dr_record, IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0_OTHERG, carrent_active_functions);
    }
  }
//  else
//  {
//    //Відбулася незрозуміла ситуація - сюди програма теоретично ніколи не мала б заходити
//    fix_undefined_error_dr(carrent_active_functions);
//  }
}
/*****************************************************/

/*****************************************************/
//Початок моніторингу максимального струму 3I0_r
/*****************************************************/
inline void start_monitoring_max_3I0_r(unsigned int const time_tmp, unsigned char * const buffer_for_save_dr_record, unsigned char ** const arr_identifiers, unsigned char ** const p_next_free_array, unsigned int* const carrent_active_functions)
{
  unsigned char * buffer = *p_next_free_array;
  if (
      (arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0_r - 1] == NULL) &&
      (buffer >= (buffer_for_save_dr_record + FIRST_INDEX_FIRST_BLOCK_DR)) && 
      ((buffer + SIZE_ARRAY_FIX_MAX_MEASUREMENTS*sizeof(unsigned int) - 1) < (buffer_for_save_dr_record + FIRST_INDEX_FIRST_DATA_DR))  
     )   
  {
    //Збільшуємо кількість фіксованих значень максимального 3I0_r
    ++number_max_3I0_r_dr;
    arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0_r - 1] = buffer;
    *p_next_free_array += SIZE_ARRAY_FIX_MAX_MEASUREMENTS*sizeof(unsigned int);
  
    //Помічаємо, що будем виходити з того, що зараз значення тільки починають моніторитися, тому приймаємо їх за найбільші
    __meas_to_d_meas const * p_link = im_to_idm;
    for(size_t i = 0; i <_SIZE_ARRAY_FIX_MAX_MEASUREMENTS_TMP; ++i)
    {
      buffer = serialization_uint_uchar(&(p_link->arr[p_link->ind]), buffer, 0);
      
      ++p_link;
    }
  
    //Помічаємо, що будем виходити з того, що зараз значення тільки починають моніторитися, тому приймаємо їх за найбільші
    int frequency_int = (int)frequency;
    if (frequency_int >= 0) frequency_int = (int)(frequency*1000);
    buffer = serialization_uint_uchar((unsigned int *)(&frequency_int), buffer, 0);

    //ВМП
    unsigned int const * p_arr = vmp_start_val;
    for (size_t i = 0; i < 2; ++i)
    {
      buffer = serialization_uint_uchar(p_arr++, buffer, 0);
    }
  
    //Фіксуємо час з моменту початку аварійного запису
    buffer = serialization_uint_uchar(&time_tmp, buffer, 1);

    //Фіксуємо причину запису
    *buffer = IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0_r;

//    //Помічаємо, що ми на стадії моніторингу максимального фазного струму
//    state_current_monitoring |= (1<<IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0_r);
  }
  else
  {
    //Відбулася незрозуміла ситуація - сюди програма теоретично ніколи не мала б заходити
    fix_undefined_error_dr(carrent_active_functions);
  }
}
/*****************************************************/

/*****************************************************/
//Продовження моніторингу максимального струму 3I0_r
/*****************************************************/
inline void continue_monitoring_max_3I0_r(unsigned int const time_tmp, unsigned char * const buffer_for_save_dr_record, unsigned char ** const arr_identifiers, unsigned int* const carrent_active_functions)
{
  unsigned char * buffer = arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0_r - 1];
  if (
      (buffer >= (buffer_for_save_dr_record + FIRST_INDEX_FIRST_BLOCK_DR)) && 
      ((buffer + SIZE_ARRAY_FIX_MAX_MEASUREMENTS*sizeof(unsigned int) - 1) < (buffer_for_save_dr_record + FIRST_INDEX_FIRST_DATA_DR))  
    )   
  {
    //Перевірка, чи не є зарза фазний струм більший, ніж той що помічений максимальним
    unsigned int tmp_data = 0;
    deserialization_uchar_uint(&buffer[IDM_3I0_r*sizeof(unsigned int)], &tmp_data);
    if(tmp_data < measurement[IM_3I0_r]) 
    {
      __meas_to_d_meas const * p_link = im_to_idm;
      for(size_t i = 0; i <_SIZE_ARRAY_FIX_MAX_MEASUREMENTS_TMP; ++i)
      {
        buffer = serialization_uint_uchar(&(p_link->arr[p_link->ind]), buffer, 0);
      
        ++p_link;
      }

      //Зафіксовано зріз при найвищому фазовому струмі з моменту початку спостереження за ним
      int frequency_int = (int)frequency;
      if (frequency_int >= 0) frequency_int = (int)(frequency*1000);
      buffer = serialization_uint_uchar((unsigned int *)(&frequency_int), buffer, 0);
  
      //Фіксуємо час з моменту початку аварійного запису
      buffer = arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0_r - 1] + IDM_TM_STAMPE*sizeof(unsigned int);
      buffer = serialization_uint_uchar(&time_tmp, buffer, 1);
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
//Початок моніторингу максимальну напругу 3U0
/*****************************************************/
inline void start_monitoring_max_3U0(unsigned int const time_tmp, unsigned char * const buffer_for_save_dr_record, unsigned char ** const arr_identifiers, unsigned char ** const p_next_free_array, unsigned int* const carrent_active_functions)
{
  unsigned char * buffer = *p_next_free_array;
  if (
      (arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE_3U0 - 1] == NULL) &&
      (buffer >= (buffer_for_save_dr_record + FIRST_INDEX_FIRST_BLOCK_DR)) && 
      ((buffer + SIZE_ARRAY_FIX_MAX_MEASUREMENTS*sizeof(unsigned int) - 1) < (buffer_for_save_dr_record + FIRST_INDEX_FIRST_DATA_DR))  
     )   
  {
    //Збільшуємо кількість фіксованих значень максимальної 3U0
    ++number_max_3U0_dr;
    arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE_3U0 - 1] = buffer;
    *p_next_free_array += SIZE_ARRAY_FIX_MAX_MEASUREMENTS*sizeof(unsigned int);
  
    //Помічаємо, що будем виходити з того, що зараз значення тільки починають моніторитися, тому приймаємо їх за найбільші
    __meas_to_d_meas const * p_link = im_to_idm;
    for(size_t i = 0; i <_SIZE_ARRAY_FIX_MAX_MEASUREMENTS_TMP; ++i)
    {
      buffer = serialization_uint_uchar(&(p_link->arr[p_link->ind]), buffer, 0);
      
      ++p_link;
    }
  
    //Помічаємо, що будем виходити з того, що зараз значення тільки починають моніторитися, тому приймаємо їх за найбільші
    int frequency_int = (int)frequency;
    if (frequency_int >= 0) frequency_int = (int)(frequency*1000);
    buffer = serialization_uint_uchar((unsigned int *)(&frequency_int), buffer, 0);

    //ВМП
    unsigned int const * p_arr = vmp_start_val;
    for (size_t i = 0; i < 2; ++i)
    {
      buffer = serialization_uint_uchar(p_arr++, buffer, 0);
    }
  
    //Фіксуємо час з моменту початку аварійного запису
    buffer = serialization_uint_uchar(&time_tmp, buffer, 1);

    //Фіксуємо причину запису
    *buffer = IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE_3U0;

//    //Помічаємо, що ми на стадії моніторингу максимального фазного струму
//    state_current_monitoring |= (1<<IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE_3U0);
  }
  else
  {
    //Відбулася незрозуміла ситуація - сюди програма теоретично ніколи не мала б заходити
    fix_undefined_error_dr(carrent_active_functions);
  }
}
/*****************************************************/

/*****************************************************/
//Продовження моніторингу максимального струму 3U0
/*****************************************************/
inline void continue_monitoring_max_3U0(unsigned int const time_tmp, unsigned char * const buffer_for_save_dr_record, unsigned char ** const arr_identifiers, unsigned int* const carrent_active_functions)
{
  unsigned char * buffer = arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE_3U0 - 1];
  if (
      (buffer >= (buffer_for_save_dr_record + FIRST_INDEX_FIRST_BLOCK_DR)) && 
      ((buffer + SIZE_ARRAY_FIX_MAX_MEASUREMENTS*sizeof(unsigned int) - 1) < (buffer_for_save_dr_record + FIRST_INDEX_FIRST_DATA_DR))  
    )   
  {
    //Перевірка, чи не є зарза фазний струм більший, ніж той що помічений максимальним
    unsigned int tmp_data = 0;
    deserialization_uchar_uint(&buffer[IDM_3U0*sizeof(unsigned int)], &tmp_data);
    if(tmp_data < measurement[IM_3U0]) 
    {
      __meas_to_d_meas const * p_link = im_to_idm;
      for(size_t i = 0; i <_SIZE_ARRAY_FIX_MAX_MEASUREMENTS_TMP; ++i)
      {
        buffer = serialization_uint_uchar(&(p_link->arr[p_link->ind]), buffer, 0);
      
        ++p_link;
      }

      //Зафіксовано зріз при найвищому фазовому струмі з моменту початку спостереження за ним
      int frequency_int = (int)frequency;
      if (frequency_int >= 0) frequency_int = (int)(frequency*1000);
      buffer = serialization_uint_uchar((unsigned int *)(&frequency_int), buffer, 0);
  
      //Фіксуємо час з моменту початку аварійного запису
      buffer = arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE_3U0 - 1] + IDM_TM_STAMPE*sizeof(unsigned int);
      buffer = serialization_uint_uchar(&time_tmp, buffer, 1);
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
//Початок моніторингу мінімальної фазної/лінійної напруги 
/*****************************************************/
inline void start_monitoring_min_U(unsigned int const time_tmp, unsigned char * const buffer_for_save_dr_record, unsigned char ** const arr_identifiers, unsigned char ** const p_next_free_array, unsigned int* const carrent_active_functions)
{
  unsigned char * buffer = *p_next_free_array;
  if (
      (arr_identifiers[IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE - 1] == NULL) &&
      (buffer >= (buffer_for_save_dr_record + FIRST_INDEX_FIRST_BLOCK_DR)) && 
      ((buffer + SIZE_ARRAY_FIX_MAX_MEASUREMENTS*sizeof(unsigned int) - 1) < (buffer_for_save_dr_record + FIRST_INDEX_FIRST_DATA_DR))  
     )   
  {
    //Збільшуємо кількість фіксованих значень мінімальної фазної/лінійної напруги
    ++number_min_U_dr;
    arr_identifiers[IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE - 1] = buffer;
    *p_next_free_array += SIZE_ARRAY_FIX_MAX_MEASUREMENTS*sizeof(unsigned int);
  
    //Помічаємо, що будем виходити з того, що зараз значення тільки починають моніторитися, тому приймаємо їх за найбільші
    __meas_to_d_meas const * p_link = im_to_idm;
    for(size_t i = 0; i <_SIZE_ARRAY_FIX_MAX_MEASUREMENTS_TMP; ++i)
    {
      buffer = serialization_uint_uchar(&(p_link->arr[p_link->ind]), buffer, 0);
      
      ++p_link;
    }
  
    //Помічаємо, що будем виходити з того, що зараз значення тільки починають моніторитися, тому приймаємо їх за найбільші
    int frequency_int = (int)frequency;
    if (frequency_int >= 0) frequency_int = (int)(frequency*1000);
    buffer = serialization_uint_uchar((unsigned int *)(&frequency_int), buffer, 0);

    //ВМП
    unsigned int const * p_arr = vmp_start_val;
    for (size_t i = 0; i < 2; ++i)
    {
      buffer = serialization_uint_uchar(p_arr++, buffer, 0);
    }
  
    //Фіксуємо час з моменту початку аварійного запису
    buffer = serialization_uint_uchar(&time_tmp, buffer, 1);

    //Фіксуємо причину запису
    *buffer = IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE;

    /***
    Визначаємо мінімальну напругу
    ***/
    unsigned int tmp_data[3] = {0, 0, 0};
    buffer = arr_identifiers[IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE - 1];
    if (
        ((current_settings_prt.control_extra_settings_1 & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE)) == 0) &&
        ((current_settings_prt.control_transformator & MASKA_FOR_BIT(INDEX_ML_CTR_TRANSFORMATOR_PHASE_LINE)) == 0)
       )   
    {
      deserialization_uchar_uint(&buffer[IDM_UA*sizeof(unsigned int)], &tmp_data[0]);
      deserialization_uchar_uint(&buffer[IDM_UB*sizeof(unsigned int)], &tmp_data[1]);
      deserialization_uchar_uint(&buffer[IDM_UC*sizeof(unsigned int)], &tmp_data[2]);
    }
    else
    {
      deserialization_uchar_uint(&buffer[IDM_UAB*sizeof(unsigned int)], &tmp_data[0]);
      deserialization_uchar_uint(&buffer[IDM_UBC*sizeof(unsigned int)], &tmp_data[1]);
      deserialization_uchar_uint(&buffer[IDM_UCA*sizeof(unsigned int)], &tmp_data[2]);
    }
    min_voltage_dr = tmp_data[0];
    if (min_voltage_dr > tmp_data[1]) min_voltage_dr = tmp_data[1];
    if (min_voltage_dr > tmp_data[2]) min_voltage_dr = tmp_data[2];
    /***/

//    //Помічаємо, що ми на стадії моніторингу максимального фазного струму
//    state_current_monitoring |= (1<<IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE);
  }
  else
  {
    //Відбулася незрозуміла ситуація - сюди програма теоретично ніколи не мала б заходити
    fix_undefined_error_dr(carrent_active_functions);
  }
}
/*****************************************************/

/*****************************************************/
//Продовження моніторингу мінімальної фазної/лінійної напруги
/*****************************************************/
inline void continue_monitoring_min_U(unsigned int const time_tmp, unsigned char * const buffer_for_save_dr_record, unsigned char ** const arr_identifiers, unsigned int* const carrent_active_functions)
{
  //Перевірка, чи не є зарза досліджувана напуга менша, ніж та що помічена мінімальною
  if (
      (
       (
        ((current_settings_prt.control_extra_settings_1 & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE)) == 0) &&
        ((current_settings_prt.control_transformator & MASKA_FOR_BIT(INDEX_ML_CTR_TRANSFORMATOR_PHASE_LINE)) == 0)
       )
       &&  
       (  
        (min_voltage_dr > measurement[IM_UA]) ||
        (min_voltage_dr > measurement[IM_UB]) ||
        (min_voltage_dr > measurement[IM_UC])
       )
      )   
      || 
      (
       (
        ((current_settings_prt.control_extra_settings_1 & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE)) != 0) ||
        ((current_settings_prt.control_transformator & MASKA_FOR_BIT(INDEX_ML_CTR_TRANSFORMATOR_PHASE_LINE)) != 0)
       )
       &&  
       (  
        (min_voltage_dr > measurement[IM_UAB]) ||
        (min_voltage_dr > measurement[IM_UBC]) ||
        (min_voltage_dr > measurement[IM_UCA])
       )
     )
    )    
  {
    unsigned char * buffer = arr_identifiers[IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE - 1];
    if (
        (buffer >= (buffer_for_save_dr_record + FIRST_INDEX_FIRST_BLOCK_DR)) && 
        ((buffer + SIZE_ARRAY_FIX_MAX_MEASUREMENTS*sizeof(unsigned int) - 1) < (buffer_for_save_dr_record + FIRST_INDEX_FIRST_DATA_DR))  
      )   
    {
      __meas_to_d_meas const * p_link = im_to_idm;
      for(size_t i = 0; i <_SIZE_ARRAY_FIX_MAX_MEASUREMENTS_TMP; ++i)
      {
        buffer = serialization_uint_uchar(&(p_link->arr[p_link->ind]), buffer, 0);
      
        ++p_link;
      }

      //Зафіксовано зріз при найвищому фазовому струмі з моменту початку спостереження за ним
      int frequency_int = (int)frequency;
      if (frequency_int >= 0) frequency_int = (int)(frequency*1000);
      buffer = serialization_uint_uchar((unsigned int *)(&frequency_int), buffer, 0);
  
      //Фіксуємо час з моменту початку аварійного запису
      buffer = arr_identifiers[IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE - 1] + IDM_TM_STAMPE*sizeof(unsigned int);
      buffer = serialization_uint_uchar(&time_tmp, buffer, 1);

      unsigned int tmp_data[3] = {0, 0, 0};
      buffer = arr_identifiers[IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE - 1];
      if (
          ((current_settings_prt.control_extra_settings_1 & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE)) == 0) &&
          ((current_settings_prt.control_transformator & MASKA_FOR_BIT(INDEX_ML_CTR_TRANSFORMATOR_PHASE_LINE)) == 0)
         )   
      {
        deserialization_uchar_uint(&buffer[IDM_UA*sizeof(unsigned int)], &tmp_data[0]);
        deserialization_uchar_uint(&buffer[IDM_UB*sizeof(unsigned int)], &tmp_data[1]);
        deserialization_uchar_uint(&buffer[IDM_UC*sizeof(unsigned int)], &tmp_data[2]);
      }
      else
      {
        deserialization_uchar_uint(&buffer[IDM_UAB*sizeof(unsigned int)], &tmp_data[0]);
        deserialization_uchar_uint(&buffer[IDM_UBC*sizeof(unsigned int)], &tmp_data[1]);
        deserialization_uchar_uint(&buffer[IDM_UCA*sizeof(unsigned int)], &tmp_data[2]);
      }
      min_voltage_dr = tmp_data[0];
      if (min_voltage_dr > tmp_data[1]) min_voltage_dr = tmp_data[1];
      if (min_voltage_dr > tmp_data[2]) min_voltage_dr = tmp_data[2];
    }
    else
    {
      //Відбулася незрозуміла ситуація - сюди програма теоретично ніколи не мала б заходити
      fix_undefined_error_dr(carrent_active_functions);
    }
  }
}
/*****************************************************/

/*****************************************************/
//Початок моніторингу максимальної фазної/лінійної напруги 
/*****************************************************/
inline void start_monitoring_max_U(unsigned int const time_tmp, unsigned char * const buffer_for_save_dr_record, unsigned char ** const arr_identifiers, unsigned char ** const p_next_free_array, unsigned int* const carrent_active_functions)
{
  unsigned char * buffer = *p_next_free_array;
  if (
      (arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE - 1] == NULL) &&
      (buffer >= (buffer_for_save_dr_record + FIRST_INDEX_FIRST_BLOCK_DR)) && 
      ((buffer + SIZE_ARRAY_FIX_MAX_MEASUREMENTS*sizeof(unsigned int) - 1) < (buffer_for_save_dr_record + FIRST_INDEX_FIRST_DATA_DR))  
     )   
  {
    //Збільшуємо кількість фіксованих значень максимальної фазної/лінійної напруги
    ++number_max_U_dr;
    arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE - 1] = buffer;
    *p_next_free_array += SIZE_ARRAY_FIX_MAX_MEASUREMENTS*sizeof(unsigned int);
  
    //Помічаємо, що будем виходити з того, що зараз значення тільки починають моніторитися, тому приймаємо їх за найбільші
    __meas_to_d_meas const * p_link = im_to_idm;
    for(size_t i = 0; i <_SIZE_ARRAY_FIX_MAX_MEASUREMENTS_TMP; ++i)
    {
      buffer = serialization_uint_uchar(&(p_link->arr[p_link->ind]), buffer, 0);
      
      ++p_link;
    }
  
    //Помічаємо, що будем виходити з того, що зараз значення тільки починають моніторитися, тому приймаємо їх за найбільші
    int frequency_int = (int)frequency;
    if (frequency_int >= 0) frequency_int = (int)(frequency*1000);
    buffer = serialization_uint_uchar((unsigned int *)(&frequency_int), buffer, 0);

    //ВМП
    unsigned int const * p_arr = vmp_start_val;
    for (size_t i = 0; i < 2; ++i)
    {
      buffer = serialization_uint_uchar(p_arr++, buffer, 0);
    }
  
    //Фіксуємо час з моменту початку аварійного запису
    buffer = serialization_uint_uchar(&time_tmp, buffer, 1);

    //Фіксуємо причину запису
    *buffer = IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE;

    /***
    Визначаємо мінімальну напругу
    ***/
    unsigned int tmp_data[3] = {0, 0, 0};
    buffer = arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE - 1];
    if (
        ((current_settings_prt.control_extra_settings_1 & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE)) == 0) &&
        ((current_settings_prt.control_transformator & MASKA_FOR_BIT(INDEX_ML_CTR_TRANSFORMATOR_PHASE_LINE)) == 0)
       )   
    {
      deserialization_uchar_uint(&buffer[IDM_UA*sizeof(unsigned int)], &tmp_data[0]);
      deserialization_uchar_uint(&buffer[IDM_UB*sizeof(unsigned int)], &tmp_data[1]);
      deserialization_uchar_uint(&buffer[IDM_UC*sizeof(unsigned int)], &tmp_data[2]);
    }
    else
    {
      deserialization_uchar_uint(&buffer[IDM_UAB*sizeof(unsigned int)], &tmp_data[0]);
      deserialization_uchar_uint(&buffer[IDM_UBC*sizeof(unsigned int)], &tmp_data[1]);
      deserialization_uchar_uint(&buffer[IDM_UCA*sizeof(unsigned int)], &tmp_data[2]);
    }
    max_voltage_dr = tmp_data[0];
    if (max_voltage_dr < tmp_data[1]) max_voltage_dr = tmp_data[1];
    if (max_voltage_dr < tmp_data[2]) max_voltage_dr = tmp_data[2];
    /***/

//    //Помічаємо, що ми на стадії моніторингу максимального фазного струму
//    state_current_monitoring |= (1<<IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE);
  }
  else
  {
    //Відбулася незрозуміла ситуація - сюди програма теоретично ніколи не мала б заходити
    fix_undefined_error_dr(carrent_active_functions);
  }
}
/*****************************************************/

/*****************************************************/
//Продовження моніторингу максимальної фазної/лінійної напруги
/*****************************************************/
inline void continue_monitoring_max_U(unsigned int const time_tmp, unsigned char * const buffer_for_save_dr_record, unsigned char ** const arr_identifiers, unsigned int* const carrent_active_functions)
{
  //Перевірка, чи не є зарза досліджувана напуга менша, ніж та що помічена мінімальною
  if (
      (
       (
        ((current_settings_prt.control_extra_settings_1 & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE)) == 0) &&
        ((current_settings_prt.control_transformator & MASKA_FOR_BIT(INDEX_ML_CTR_TRANSFORMATOR_PHASE_LINE)) == 0)
       )
       &&  
       (  
        (max_voltage_dr < measurement[IM_UA]) ||
        (max_voltage_dr < measurement[IM_UB]) ||
        (max_voltage_dr < measurement[IM_UC])
       )
      )   
      || 
      (
       (
        ((current_settings_prt.control_extra_settings_1 & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE)) != 0) ||
        ((current_settings_prt.control_transformator & MASKA_FOR_BIT(INDEX_ML_CTR_TRANSFORMATOR_PHASE_LINE)) != 0)
       )
       &&  
       (  
        (max_voltage_dr < measurement[IM_UAB]) ||
        (max_voltage_dr < measurement[IM_UBC]) ||
        (max_voltage_dr < measurement[IM_UCA])
       )
     )
    )    
  {
    unsigned char * buffer = arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE - 1];
    if (
        (buffer >= (buffer_for_save_dr_record + FIRST_INDEX_FIRST_BLOCK_DR)) && 
        ((buffer + SIZE_ARRAY_FIX_MAX_MEASUREMENTS*sizeof(unsigned int) - 1) < (buffer_for_save_dr_record + FIRST_INDEX_FIRST_DATA_DR))  
      )   
    {
      __meas_to_d_meas const * p_link = im_to_idm;
      for(size_t i = 0; i <_SIZE_ARRAY_FIX_MAX_MEASUREMENTS_TMP; ++i)
      {
        buffer = serialization_uint_uchar(&(p_link->arr[p_link->ind]), buffer, 0);
      
        ++p_link;
      }

      //Зафіксовано зріз при найвищому фазовому струмі з моменту початку спостереження за ним
      int frequency_int = (int)frequency;
      if (frequency_int >= 0) frequency_int = (int)(frequency*1000);
      buffer = serialization_uint_uchar((unsigned int *)(&frequency_int), buffer, 0);
  
      //Фіксуємо час з моменту початку аварійного запису
      buffer = arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE - 1] + IDM_TM_STAMPE*sizeof(unsigned int);
      buffer = serialization_uint_uchar(&time_tmp, buffer, 1);

      unsigned int tmp_data[3] = {0, 0, 0};
      buffer = arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE - 1];
      if (
          ((current_settings_prt.control_extra_settings_1 & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE)) == 0) &&
          ((current_settings_prt.control_transformator & MASKA_FOR_BIT(INDEX_ML_CTR_TRANSFORMATOR_PHASE_LINE)) == 0)
         )   
      {
        deserialization_uchar_uint(&buffer[IDM_UA*sizeof(unsigned int)], &tmp_data[0]);
        deserialization_uchar_uint(&buffer[IDM_UB*sizeof(unsigned int)], &tmp_data[1]);
        deserialization_uchar_uint(&buffer[IDM_UC*sizeof(unsigned int)], &tmp_data[2]);
      }
      else
      {
        deserialization_uchar_uint(&buffer[IDM_UAB*sizeof(unsigned int)], &tmp_data[0]);
        deserialization_uchar_uint(&buffer[IDM_UBC*sizeof(unsigned int)], &tmp_data[1]);
        deserialization_uchar_uint(&buffer[IDM_UCA*sizeof(unsigned int)], &tmp_data[2]);
      }
      max_voltage_dr = tmp_data[0];
      if (max_voltage_dr < tmp_data[1]) max_voltage_dr = tmp_data[1];
      if (max_voltage_dr < tmp_data[2]) max_voltage_dr = tmp_data[2];
    }
    else
    {
      //Відбулася незрозуміла ситуація - сюди програма теоретично ніколи не мала б заходити
      fix_undefined_error_dr(carrent_active_functions);
    }
  }
}
/*****************************************************/

/*****************************************************/
//Початок моніторингу максимального струму  зворотньої послідовності
/*****************************************************/
inline void start_monitoring_max_ZOP(unsigned int const time_tmp, unsigned char * const buffer_for_save_dr_record, unsigned char ** const arr_identifiers, unsigned char ** const p_next_free_array, unsigned int* const carrent_active_functions)
{
  unsigned char * buffer = *p_next_free_array;
  if (
      (arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZOP - 1] == NULL) &&
      (buffer >= (buffer_for_save_dr_record + FIRST_INDEX_FIRST_BLOCK_DR)) && 
      ((buffer + SIZE_ARRAY_FIX_MAX_MEASUREMENTS*sizeof(unsigned int) - 1) < (buffer_for_save_dr_record + FIRST_INDEX_FIRST_DATA_DR))  
     )   
  {
    //Збільшуємо кількість фіксованих значень максимального відношення I2 до I1
    ++number_max_ZOP_dr;
    arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZOP - 1] = buffer;
    *p_next_free_array += SIZE_ARRAY_FIX_MAX_MEASUREMENTS*sizeof(unsigned int);
  
    //Помічаємо, що будем виходити з того, що зараз значення тільки починають моніторитися, тому приймаємо їх за найбільші
    __meas_to_d_meas const * p_link = im_to_idm;
    for(size_t i = 0; i <_SIZE_ARRAY_FIX_MAX_MEASUREMENTS_TMP; ++i)
    {
      buffer = serialization_uint_uchar(&(p_link->arr[p_link->ind]), buffer, 0);
      
      ++p_link;
    }
  
    //Помічаємо, що будем виходити з того, що зараз значення тільки починають моніторитися, тому приймаємо їх за найбільші
    int frequency_int = (int)frequency;
    if (frequency_int >= 0) frequency_int = (int)(frequency*1000);
    buffer = serialization_uint_uchar((unsigned int *)(&frequency_int), buffer, 0);

    //ВМП
    unsigned int const * p_arr = vmp_start_val;
    for (size_t i = 0; i < 2; ++i)
    {
      buffer = serialization_uint_uchar(p_arr++, buffer, 0);
    }
  
    //Фіксуємо час з моменту початку аварійного запису
    buffer = serialization_uint_uchar(&time_tmp, buffer, 1);

    //Фіксуємо причину запису
    *buffer = IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZOP;

//    //Помічаємо, що ми на стадії моніторингу максимального фазного струму
//    state_current_monitoring |= (1<<IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZOP);
  }
  else
  {
    //Відбулася незрозуміла ситуація - сюди програма теоретично ніколи не мала б заходити
    fix_undefined_error_dr(carrent_active_functions);
  }
}
/*****************************************************/

/*****************************************************/
//Продовження моніторингу максимального струму зворотньої послідовності
/*****************************************************/
inline void continue_monitoring_max_ZOP(unsigned int const time_tmp, unsigned char * const buffer_for_save_dr_record, unsigned char ** const arr_identifiers, unsigned int* const carrent_active_functions)
{
  unsigned char * buffer = arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZOP - 1];
  if (
      (buffer >= (buffer_for_save_dr_record + FIRST_INDEX_FIRST_BLOCK_DR)) && 
      ((buffer + SIZE_ARRAY_FIX_MAX_MEASUREMENTS*sizeof(unsigned int) - 1) < (buffer_for_save_dr_record + FIRST_INDEX_FIRST_DATA_DR))  
    )   
  {
    //Перевірка, чи не є зарза струм зворотньої послідовності більший, ніж той що помічений максимальним
/*
        _I2 - струм зворотньої послідовності при попередньому зафіксованому максимальному відношенні I2/I1
        _I1 - струм прямої послідовності при попередньому зафіксованому максимальному відношенні I2/I1
      
        I2 -  текучий струм зворотньої послідовності
        I1 -  текучий струм прямої послідовності
      
        Умова перезапису параметрів:
        _I2     I2
        ---  < --- /x на _I1*I1  
        _I1     I1
      
        (_I2*I1 < I2*_I1)  - це є умова, що зараз є більше ЗОП(КОФ) ніж попередній раз
*/
    unsigned int I2, I1, _I1, _I2;
    I2 = measurement[IM_I2];
    I1 = measurement[IM_I1];
    deserialization_uchar_uint(&buffer[IDM_I2*sizeof(unsigned int)], &_I2);
    deserialization_uchar_uint(&buffer[IDM_I1*sizeof(unsigned int)], &_I1);
    if (
        ( (_I1 >  0) && ( ((_I2*I1) < (I2*_I1)) || (I1 == 0) ) ) ||
        ( (_I1 == 0) && (I1 == 0) && ( _I2 < I2 ) ) 
       )
    {
      __meas_to_d_meas const * p_link = im_to_idm;
      for(size_t i = 0; i <_SIZE_ARRAY_FIX_MAX_MEASUREMENTS_TMP; ++i)
      {
        buffer = serialization_uint_uchar(&(p_link->arr[p_link->ind]), buffer, 0);
      
        ++p_link;
      }

      //Зафіксовано зріз при найвищому фазовому струмі з моменту початку спостереження за ним
      int frequency_int = (int)frequency;
      if (frequency_int >= 0) frequency_int = (int)(frequency*1000);
      buffer = serialization_uint_uchar((unsigned int *)(&frequency_int), buffer, 0);
  
      //Фіксуємо час з моменту початку аварійного запису
      buffer = arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZOP - 1] + IDM_TM_STAMPE*sizeof(unsigned int);
      buffer = serialization_uint_uchar(&time_tmp, buffer, 1);
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
//Початок моніторингу мінімальної частоти 1 
/*****************************************************/
inline void start_monitoring_min_f(unsigned int const time_tmp, unsigned char * const buffer_for_save_dr_record, unsigned char ** const arr_identifiers, unsigned char ** const p_next_free_array, unsigned int* const carrent_active_functions)
{
  unsigned char * buffer = *p_next_free_array;
  if (
      (arr_identifiers[IDENTIFIER_BIT_ARRAY_MIN_FREQUENCY_ACHR - 1] == NULL) &&
      (buffer >= (buffer_for_save_dr_record + FIRST_INDEX_FIRST_BLOCK_DR)) && 
      ((buffer + SIZE_ARRAY_FIX_MAX_MEASUREMENTS*sizeof(unsigned int) - 1) < (buffer_for_save_dr_record + FIRST_INDEX_FIRST_DATA_DR))  
     )   
  {
    //Збільшуємо кількість фіксованих значень мінімальної частоти 1
    ++number_min_f_achr_dr;
    arr_identifiers[IDENTIFIER_BIT_ARRAY_MIN_FREQUENCY_ACHR - 1] = buffer;
    *p_next_free_array += SIZE_ARRAY_FIX_MAX_MEASUREMENTS*sizeof(unsigned int);
  
    //Помічаємо, що будем виходити з того, що зараз значення тільки починають моніторитися, тому приймаємо їх за найбільші
    __meas_to_d_meas const * p_link = im_to_idm;
    for(size_t i = 0; i <_SIZE_ARRAY_FIX_MAX_MEASUREMENTS_TMP; ++i)
    {
      buffer = serialization_uint_uchar(&(p_link->arr[p_link->ind]), buffer, 0);
      
      ++p_link;
    }
  
    //Помічаємо, що будем виходити з того, що зараз значення тільки починають моніторитися, тому приймаємо їх за найбільші
    int frequency_int = (int)frequency;
    if (frequency_int >= 0) frequency_int = (int)(frequency*1000);
    buffer = serialization_uint_uchar((unsigned int *)(&frequency_int), buffer, 0);

    //ВМП
    unsigned int const * p_arr = vmp_start_val;
    for (size_t i = 0; i < 2; ++i)
    {
      buffer = serialization_uint_uchar(p_arr++, buffer, 0);
    }
  
    //Фіксуємо час з моменту початку аварійного запису
    buffer = serialization_uint_uchar(&time_tmp, buffer, 1);

    //Фіксуємо причину запису
    *buffer = IDENTIFIER_BIT_ARRAY_MIN_FREQUENCY_ACHR;

//    //Помічаємо, що ми на стадії моніторингу максимального фазного струму
//    state_current_monitoring |= (1<<IDENTIFIER_BIT_ARRAY_MIN_FREQUENCY_ACHR);
  }
  else
  {
    //Відбулася незрозуміла ситуація - сюди програма теоретично ніколи не мала б заходити
    fix_undefined_error_dr(carrent_active_functions);
  }
}
/*****************************************************/

/*****************************************************/
//Продовження моніторингу мінімальної частоти 1
/*****************************************************/
inline void continue_monitoring_min_f(unsigned int const time_tmp, unsigned char * const buffer_for_save_dr_record, unsigned char ** const arr_identifiers, unsigned int* const carrent_active_functions)
{
  unsigned char * buffer = arr_identifiers[IDENTIFIER_BIT_ARRAY_MIN_FREQUENCY_ACHR - 1];
  if (
      (buffer >= (buffer_for_save_dr_record + FIRST_INDEX_FIRST_BLOCK_DR)) && 
      ((buffer + SIZE_ARRAY_FIX_MAX_MEASUREMENTS*sizeof(unsigned int) - 1) < (buffer_for_save_dr_record + FIRST_INDEX_FIRST_DATA_DR))  
    )   
  {
    //Перевірка, чи не є зарза фазний струм більший, ніж той що помічений максимальним
    unsigned int tmp_data = 0;
    deserialization_uchar_uint(&buffer[IDM_freq*sizeof(unsigned int)], &tmp_data);
    if (((int)tmp_data) != (-2)) /*Не зафіксовано, що частота була нижче порогу визначеного константою MIN_FREQUENCY - інакше далі моніторити мінімальну частоту немає сенсу*/
    {
      //Перевірка, чи не є зарза досліджувана частота менша, ніж та що помічена мінімальною
      int frequency_int = (int)frequency;
      if (frequency_int >= 0) frequency_int = (int)(frequency*1000);
  
      if(
         (frequency_int != (-1)) && /*Частота не визначена*/
         (frequency_int != (-3)) &&  /*Частота вище порогу визначеного константою MAX_FREQUENCY*/
         (
          (((int)tmp_data) > frequency_int) ||
          (frequency_int == (-2)) /*Частота нижче порогу визначеного константою MIN_FREQUENCY - цю перевірку робимо, бо у попередній момент часу могло крім значення частоти бути ще числа -1 або -3*/
         )   
        )   
      {
        __meas_to_d_meas const * p_link = im_to_idm;
        for(size_t i = 0; i <_SIZE_ARRAY_FIX_MAX_MEASUREMENTS_TMP; ++i)
        {
          buffer = serialization_uint_uchar(&(p_link->arr[p_link->ind]), buffer, 0);
      
          ++p_link;
        }

        //Зафіксовано зріз при найвищому фазовому струмі з моменту початку спостереження за ним
        buffer = serialization_uint_uchar((unsigned int *)(&frequency_int), buffer, 0);
  
        //Фіксуємо час з моменту початку аварійного запису
        buffer = arr_identifiers[IDENTIFIER_BIT_ARRAY_MIN_FREQUENCY_ACHR - 1] + IDM_TM_STAMPE*sizeof(unsigned int);
        buffer = serialization_uint_uchar(&time_tmp, buffer, 1);
      }
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
inline void command_start_saving_record_dr_into_dataflash(unsigned int * const p_saving_record_dr)
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
inline void routine_for_queue_dr(unsigned int * const p_saving_record_dr)
{
  if(
     (state_dr_record == STATE_DR_MAKE_RECORD)            || /*Стоїть умова сформовані дані передати на запис у DataFlsh без повторного початку  нового запису, бо умова завершення запиу    досягнкта, а не стояла умова примусового завершення даного сформованого запису*/
     (state_dr_record == STATE_DR_CUT_RECORD)                /*Стоїть умова сформовані дані передати на запис у DataFlsh   з повторним  початком нового запису, бо умова завершення запиу не досягнкта, а    стояла умова примусового завершення даного сформованого запису*/
    )
  {
    if (++head_queue_dr >= SIZE_QUEUE_DR) head_queue_dr = 0;
    
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

long tstBrrFooSelector = 0;//змінна для тестів і перевірок

/*****************************************************/
//Функція обробки логіки дискретного реєстратора
/*****************************************************/
inline void digital_registrator(unsigned int* carrent_active_functions)
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
  if (((POWER_CTRL->IDR & POWER_CTRL_PIN) != (uint32_t)Bit_RESET)) statePowerDown = STATE_POWER_DOWN_NONE;
  else
  {
    if (timePowerDown < POWER_DOWN_TIME) statePowerDown = STATE_POWER_DOWN_ETAP_BEFORE;
    else
    {
      if (statePowerDown == STATE_POWER_DOWN_ETAP_BEFORE) statePowerDown = STATE_POWER_DOWN_ETAP_CUT;
      else if (statePowerDown == STATE_POWER_DOWN_ETAP_CUT_CONFIRMED) statePowerDown = STATE_POWER_DOWN_ETAP_AFTER;
    }
  }
  /***/
 

  static unsigned int previous_active_functions[N_BIG];
  
  static unsigned int number_items_dr;
  static unsigned int number_changes_into_dr_record;
  static unsigned int time_from_start_record_dr;
  static unsigned int blocking_continue_monitoring_min_U;
  
 DigRegUniqVarsAddreses  drUniqVarsAddreses = {
	&number_items_dr,
	&number_changes_into_dr_record,
	&time_from_start_record_dr,
	&blocking_continue_monitoring_min_U,
	previous_active_functions,
	carrent_active_functions
};
  
  
//  TestStateNameSpaceFooBrrr((DigRegUniqVarsAddreses*)&drUniqVarsAddreses);
  //put_before_info_in_buf
  //CmdPlusTimeStampLogHundler(active_functions);
  static unsigned int const monitoring_phase_signals[N_BIG] =
  {
    MASKA_MONITOTYNG_PHASE_SIGNALES_0,
    MASKA_MONITOTYNG_PHASE_SIGNALES_1,
    MASKA_MONITOTYNG_PHASE_SIGNALES_2,
    MASKA_MONITOTYNG_PHASE_SIGNALES_3,
    MASKA_MONITOTYNG_PHASE_SIGNALES_4,
    MASKA_MONITOTYNG_PHASE_SIGNALES_5,
    MASKA_MONITOTYNG_PHASE_SIGNALES_6,
    MASKA_MONITOTYNG_PHASE_SIGNALES_7,
		MASKA_MONITOTYNG_PHASE_SIGNALES_8

#ifdef MASKA_MONITOTYNG_PHASE_SIGNALES_9
                                     ,
    MASKA_MONITOTYNG_PHASE_SIGNALES_9
#endif
  };
  static unsigned int const monitoring_phase04_signals[N_BIG] =
  {
    MASKA_MONITOTYNG_PHASE04_SIGNALES_0,
    MASKA_MONITOTYNG_PHASE04_SIGNALES_1,
    MASKA_MONITOTYNG_PHASE04_SIGNALES_2,
    MASKA_MONITOTYNG_PHASE04_SIGNALES_3,
    MASKA_MONITOTYNG_PHASE04_SIGNALES_4,
    MASKA_MONITOTYNG_PHASE04_SIGNALES_5,
    MASKA_MONITOTYNG_PHASE04_SIGNALES_6,
    MASKA_MONITOTYNG_PHASE04_SIGNALES_7,
		MASKA_MONITOTYNG_PHASE04_SIGNALES_8
      
#ifdef MASKA_MONITOTYNG_PHASE04_SIGNALES_9
                                        ,
    MASKA_MONITOTYNG_PHASE04_SIGNALES_9
#endif
  };
  static unsigned int const monitoring_3I0_r_signals[N_BIG] =
  {
    MASKA_MONITOTYNG_3I0_R_SIGNALES_0,
    MASKA_MONITOTYNG_3I0_R_SIGNALES_1,
    MASKA_MONITOTYNG_3I0_R_SIGNALES_2,
    MASKA_MONITOTYNG_3I0_R_SIGNALES_3,
    MASKA_MONITOTYNG_3I0_R_SIGNALES_4,
    MASKA_MONITOTYNG_3I0_R_SIGNALES_5,
    MASKA_MONITOTYNG_3I0_R_SIGNALES_6,
    MASKA_MONITOTYNG_3I0_R_SIGNALES_7,
		MASKA_MONITOTYNG_3I0_R_SIGNALES_8
      
#ifdef MASKA_MONITOTYNG_3I0_R_SIGNALES_9
                                        ,
		MASKA_MONITOTYNG_3I0_R_SIGNALES_9
#endif
  };
  static unsigned int const monitoring_znam_signals[N_BIG] =
  {
    MASKA_MONITOTYNG_ZNAM_SIGNALES_0,
    MASKA_MONITOTYNG_ZNAM_SIGNALES_1,
    MASKA_MONITOTYNG_ZNAM_SIGNALES_2,
    MASKA_MONITOTYNG_ZNAM_SIGNALES_3,
    MASKA_MONITOTYNG_ZNAM_SIGNALES_4,
    MASKA_MONITOTYNG_ZNAM_SIGNALES_5,
    MASKA_MONITOTYNG_ZNAM_SIGNALES_6,
    MASKA_MONITOTYNG_ZNAM_SIGNALES_7,
		MASKA_MONITOTYNG_ZNAM_SIGNALES_8
      
#ifdef MASKA_MONITOTYNG_ZNAM_SIGNALES_9
                                        ,
    MASKA_MONITOTYNG_ZNAM_SIGNALES_9
#endif
  };
  static unsigned int const monitoring_3U0_signals[N_BIG] =
  {
    MASKA_MONITOTYNG_3U0_SIGNALES_0,
    MASKA_MONITOTYNG_3U0_SIGNALES_1,
    MASKA_MONITOTYNG_3U0_SIGNALES_2,
    MASKA_MONITOTYNG_3U0_SIGNALES_3,
    MASKA_MONITOTYNG_3U0_SIGNALES_4,
    MASKA_MONITOTYNG_3U0_SIGNALES_5,
    MASKA_MONITOTYNG_3U0_SIGNALES_6,
    MASKA_MONITOTYNG_3U0_SIGNALES_7,
		MASKA_MONITOTYNG_3U0_SIGNALES_8
      
#ifdef MASKA_MONITOTYNG_3U0_SIGNALES_9
                                   ,
    MASKA_MONITOTYNG_3U0_SIGNALES_9
#endif
  };
  static unsigned int const monitoring_Umin_signals[N_BIG] =
  {
    MASKA_MONITOTYNG_UMIN_SIGNALES_0,
    MASKA_MONITOTYNG_UMIN_SIGNALES_1,
    MASKA_MONITOTYNG_UMIN_SIGNALES_2,
    MASKA_MONITOTYNG_UMIN_SIGNALES_3,
    MASKA_MONITOTYNG_UMIN_SIGNALES_4,
    MASKA_MONITOTYNG_UMIN_SIGNALES_5,
    MASKA_MONITOTYNG_UMIN_SIGNALES_6,
    MASKA_MONITOTYNG_UMIN_SIGNALES_7,
    MASKA_MONITOTYNG_UMIN_SIGNALES_8
      
#ifdef MASKA_MONITOTYNG_UMIN_SIGNALES_9
                                    ,
    MASKA_MONITOTYNG_UMIN_SIGNALES_9
#endif
  };
  static unsigned int const monitoring_Umax_signals[N_BIG] =
  {
    MASKA_MONITOTYNG_UMAX_SIGNALES_0,
    MASKA_MONITOTYNG_UMAX_SIGNALES_1,
    MASKA_MONITOTYNG_UMAX_SIGNALES_2,
    MASKA_MONITOTYNG_UMAX_SIGNALES_3,
    MASKA_MONITOTYNG_UMAX_SIGNALES_4,
    MASKA_MONITOTYNG_UMAX_SIGNALES_5,
    MASKA_MONITOTYNG_UMAX_SIGNALES_6,
    MASKA_MONITOTYNG_UMAX_SIGNALES_7,
		MASKA_MONITOTYNG_UMAX_SIGNALES_8
      
#ifdef MASKA_MONITOTYNG_UMAX_SIGNALES_9
                                    ,
    MASKA_MONITOTYNG_UMAX_SIGNALES_9
#endif
  };
  static unsigned int const monitoring_zzp_signals[N_BIG] =
  {
    MASKA_MONITOTYNG_ZOP_SIGNALES_0,
    MASKA_MONITOTYNG_ZOP_SIGNALES_1,
    MASKA_MONITOTYNG_ZOP_SIGNALES_2,
    MASKA_MONITOTYNG_ZOP_SIGNALES_3,
    MASKA_MONITOTYNG_ZOP_SIGNALES_4,
    MASKA_MONITOTYNG_ZOP_SIGNALES_5,
    MASKA_MONITOTYNG_ZOP_SIGNALES_6,
    MASKA_MONITOTYNG_ZOP_SIGNALES_7,
		MASKA_MONITOTYNG_ZOP_SIGNALES_8	
      
#ifdef MASKA_MONITOTYNG_ZOP_SIGNALES_9
                                   ,
    MASKA_MONITOTYNG_ZOP_SIGNALES_9
#endif
  };
  static unsigned int const monitoring_f_min_achr_signals[N_BIG] =
  {
    MASKA_MONITOTYNG_F_MIN_ACHR_SIGNALES_0,
    MASKA_MONITOTYNG_F_MIN_ACHR_SIGNALES_1,
    MASKA_MONITOTYNG_F_MIN_ACHR_SIGNALES_2,
    MASKA_MONITOTYNG_F_MIN_ACHR_SIGNALES_3,
    MASKA_MONITOTYNG_F_MIN_ACHR_SIGNALES_4,
    MASKA_MONITOTYNG_F_MIN_ACHR_SIGNALES_5,
    MASKA_MONITOTYNG_F_MIN_ACHR_SIGNALES_6,
    MASKA_MONITOTYNG_F_MIN_ACHR_SIGNALES_7,
		MASKA_MONITOTYNG_F_MIN_ACHR_SIGNALES_8
      
#ifdef MASKA_MONITOTYNG_F_MIN_ACHR_SIGNALES_9
                                          ,
    MASKA_MONITOTYNG_F_MIN_ACHR_SIGNALES_9
#endif
  };
  
  unsigned char * buffer_for_save_dr_record = queue_dr[head_queue_dr];
  static unsigned int saving_record_dr = false;
  static unsigned char * arr_identifiers[_NUMBER_IDENTIFIER] =
  {
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
  };
  static unsigned char * next_free_array = NULL;

  //Цю перевірку виконуємо тільки у тому випадку, коли іде процес формування нового запису
  if(state_dr_record == STATE_DR_EXECUTING_RECORD)
  {
    //Перевіряємо чи не виникла умова, що зарараз буде перебір фіксації максимальних струмів
    unsigned int temp_value_for_max_min_fix_measurement = (
                                                            number_max_phase_dr      + 
                                                            number_max_phase04_dr    + 
                                                            number_max_ZNAM_dr       + 
                                                            number_max_3I0_dr_1g     +
                                                            number_max_3I0_dr_otherg +
                                                            number_max_3I0_r_dr      +
                                                            number_max_3U0_dr        +
                                                            number_min_U_dr          +
                                                            number_max_U_dr          +
                                                            number_max_ZOP_dr        +
                                                            number_min_f_achr_dr     +
                                                            number_f_chapv_dr    
                                                      );
    if(temp_value_for_max_min_fix_measurement > MAX_NUMBER_FIX_MAX_MEASUREMENTS)
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
      //Перевіряємо чи стоїть умова почати моніторити максимальний фазний струм
      unsigned int comp = false;
      COMPARE_NOT_ZERO_OR(comp, carrent_active_functions, monitoring_phase_signals, N_BIG)
      if(comp)
      {
        if(arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE - 1] == NULL)
        {
          //Є умова почати новий моніторинг максимального фазного струму
          temp_value_for_max_min_fix_measurement++;
        }
      }

      //Перевіряємо чи стоїть умова почати моніторити максимальний фазний струм сторони 0.4кВ
      COMPARE_NOT_ZERO_OR(comp, carrent_active_functions, monitoring_phase04_signals, N_BIG)
      if(comp)
      {
        if(arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE04 - 1] == NULL)
        {
          //Є умова почати новий моніторинг максимального фазного струму сторони 0.4кВ
          temp_value_for_max_min_fix_measurement++;
        }
      }

      //Перевіряємо чи стоїть умова почати моніторити відношення другої гармоніки до першої гармоніки фазного струму
      COMPARE_NOT_ZERO_OR(comp, carrent_active_functions, monitoring_znam_signals, N_BIG)
      if(comp)
      {
        if(arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZNAM - 1] == NULL)
        {
          //Є умова почати новий моніторинг максимального відношення другої гармоніки до першої гармоніки фазного струму
          temp_value_for_max_min_fix_measurement++;
        }
      }

      //Перевіряємо чи стоїть умова почати моніторити максимальний струм 3I0 1г
      comp = false;
      if(!_GET_STATE(current_settings_prt.control_zz, INDEX_ML_CTR_ZZ1_TYPE_BIT))
      {
        COMPARE_NOT_ZERO_OR(comp, carrent_active_functions, monitoring_3I0_1g_signals, N_BIG)
      }
      if(comp)
      {
        if(arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0_1G - 1] == NULL)
        {
          //Є умова почати новий моніторинг максимального струму 3I0 1г
          temp_value_for_max_min_fix_measurement++;
        }
      }

      //Перевіряємо чи стоїть умова почати моніторити максимальний струм 3I0 вищих гармонік
      comp = false;
      if(_GET_STATE(current_settings_prt.control_zz, INDEX_ML_CTR_ZZ1_TYPE_BIT))
      {
        COMPARE_NOT_ZERO_OR(comp, carrent_active_functions, monitoring_3I0_otherg_signals, N_BIG)
      }
      if(comp)
      {
        if(arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0_OTHERG - 1] == NULL)
        {
          //Є умова почати новий моніторинг максимального струму 3I0 вищих гармонік
          temp_value_for_max_min_fix_measurement++;
        }
      }
      
      //Перевіряємо чи стоїть умова почати моніторити максимальний струм 3I0 розрахунковий
      COMPARE_NOT_ZERO_OR(comp, carrent_active_functions, monitoring_3I0_r_signals, N_BIG)
      if(comp)
      {
        if(arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0_r - 1] == NULL)
        {
          //Є умова почати новий моніторинг максимального струму 3I0 розрахункового
          temp_value_for_max_min_fix_measurement++;
        }
      }

      //Перевіряємо чи стоїть умова почати моніторити максимальну напругу 3U0
      COMPARE_NOT_ZERO_OR(comp, carrent_active_functions, monitoring_3U0_signals, N_BIG)
      if(comp)
      {
        if(arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE_3U0 - 1] == NULL)
        {
          //Є умова почати новий моніторинг максимальну напругу 3U0
          temp_value_for_max_min_fix_measurement++;
        }
      }
      
      //Перевіряємо чи стоїть умова почати моніторити мінімальну напругу
      if (
          (blocking_continue_monitoring_min_U == 0) ||
          (_CHECK_SET_BIT(carrent_active_functions, RANG_WORK_BV) != 0)  
         )
      {
        COMPARE_NOT_ZERO_OR(comp, carrent_active_functions, monitoring_Umin_signals, N_BIG)
        if(comp)
        {
          if(arr_identifiers[IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE - 1] == NULL)
          {
            //Є умова почати новий моніторинг мінімальну фазну напругу
            temp_value_for_max_min_fix_measurement++;
          }
        }
      }
      
      //Перевіряємо чи стоїть умова почати моніторити максимальну напругу
      COMPARE_NOT_ZERO_OR(comp, carrent_active_functions, monitoring_Umax_signals, N_BIG)
      if(comp)
      {
        if(arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE - 1] == NULL)
        {
          //Є умова почати новий моніторинг максимальну фазну напругу
          temp_value_for_max_min_fix_measurement++;
        }
      }
      
      //Перевіряємо чи стоїть умова почати моніторити максимальний струм зворотньої послідовності
      COMPARE_NOT_ZERO_OR(comp, carrent_active_functions, monitoring_zzp_signals, N_BIG)
      if(comp)
      {
        if(arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZOP - 1] == NULL)
        {
          //Є умова почати новий моніторинг максимального струму зворотнньої послідовності
          temp_value_for_max_min_fix_measurement++;
        }
      }
    
      //Перевіряємо чи стоїть умова почати моніторити мінімальну частоту для АЧР
      COMPARE_NOT_ZERO_OR(comp, carrent_active_functions, monitoring_f_min_achr_signals, N_BIG)
      if(comp)
      {
        if(arr_identifiers[IDENTIFIER_BIT_ARRAY_MIN_FREQUENCY_ACHR - 1] == NULL)
        {
          //Є умова почати новий моніторинг мінімальну частоту
          temp_value_for_max_min_fix_measurement++;
        }
      }
      
      
      if (
          /*Є умова дозволу подачі команди ЧАПВ*/
          (_CHECK_SET_BIT(carrent_active_functions , RANG_VN_BLOCK_CHAPV) == 0)
          &&
          (
           (
            /*Умова зафіксувати частоту для зовнішнього ЧАПВ*/
            (_CHECK_SET_BIT(previous_active_functions, RANG_ACHR_CHAPV_VID_DV) != 0) &&
            (_CHECK_SET_BIT(carrent_active_functions , RANG_ACHR_CHAPV_VID_DV) == 0)
           )
           ||
           (
            /*Умова зафіксувати частоту від внутрішнього ЧАПВ*/
            (_CHECK_SET_BIT(carrent_active_functions , RANG_RAZR_CHAPV) != 0) 
            &&
            (
             (  
              (_CHECK_SET_BIT(previous_active_functions, RANG_ACHR_CHAPV1  ) != 0) &&
              (_CHECK_SET_BIT(carrent_active_functions , RANG_ACHR_CHAPV1  ) == 0)
             )   
             ||  
             (  
              (_CHECK_SET_BIT(previous_active_functions, RANG_ACHR_CHAPV2  ) != 0) &&
              (_CHECK_SET_BIT(carrent_active_functions , RANG_ACHR_CHAPV2  ) == 0)
             )   
            )           
           ) 
          )
         )   
      {
        //Є умова зафіксувати частоту від ЧАПВ
        temp_value_for_max_min_fix_measurement++;
      }
          
      
      if(temp_value_for_max_min_fix_measurement > MAX_NUMBER_FIX_MAX_MEASUREMENTS)
      {
        //Виникла ситуація, що зарараз буде перебір фіксації максимальних вимірювань
        //Треба примусово завершити текучий запис і почати наступний запис

        buffer_for_save_dr_record[FIRST_INDEX_NUMBER_ITEMS_DR      ] = number_items_dr;
        buffer_for_save_dr_record[FIRST_INDEX_NUMBER_CHANGES_DR    ] = number_changes_into_dr_record        & 0xff;
        buffer_for_save_dr_record[FIRST_INDEX_NUMBER_CHANGES_DR + 1] = (number_changes_into_dr_record >> 8) & 0xff;

        //Перевіряємо чи треба завершити моніторинг максимального фазного струму
        if(arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE - 1] != NULL)
          end_monitoring_min_max_measurement(arr_identifiers, buffer_for_save_dr_record, IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE, carrent_active_functions);

        //Перевіряємо чи треба завершити моніторинг максимального фазного струму сторони 0.4кВ
        if(arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE04 - 1] != NULL)
          end_monitoring_min_max_measurement(arr_identifiers, buffer_for_save_dr_record, IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE04, carrent_active_functions);

        //Перевіряємо чи треба завершити моніторинг максимального відношення другої гармоніки до першої гармоніки фазного струму
        if(arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZNAM - 1] != NULL)
          end_monitoring_min_max_measurement(arr_identifiers, buffer_for_save_dr_record, IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZNAM, carrent_active_functions);

        //Перевіряємо чи треба завершити моніторинг максимального струму 3I0 1г
        if(arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0_1G - 1] != NULL)
          end_monitoring_min_max_measurement(arr_identifiers, buffer_for_save_dr_record, IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0_1G, carrent_active_functions);

        //Перевіряємо чи треба завершити моніторинг максимального струму 3I0 вищих гармонік
        if(arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0_OTHERG - 1] != NULL)
          end_monitoring_min_max_measurement(arr_identifiers, buffer_for_save_dr_record, IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0_OTHERG, carrent_active_functions);

        //Перевіряємо чи треба завершити моніторинг максимального струму 3I0 розрахункового
        if(arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0_r - 1] != NULL)
          end_monitoring_min_max_measurement(arr_identifiers, buffer_for_save_dr_record, IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0_r, carrent_active_functions);

        //Перевіряємо чи треба завершити моніторинг максимальної напруги 3U0
        if(arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE_3U0 - 1] != NULL)
          end_monitoring_min_max_measurement(arr_identifiers, buffer_for_save_dr_record, IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE_3U0, carrent_active_functions);

        //Перевіряємо чи треба завершити моніторинг мінімальної напруги
        if(arr_identifiers[IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE - 1] != NULL)
          end_monitoring_min_max_measurement(arr_identifiers, buffer_for_save_dr_record, IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE, carrent_active_functions);

        //Перевіряємо чи треба завершити моніторинг максимальної напруги
        if(arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE - 1] != NULL)
          end_monitoring_min_max_measurement(arr_identifiers, buffer_for_save_dr_record, IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE, carrent_active_functions);

        //Перевіряємо чи треба завершити моніторинг максимального струму звороної послідовності
        if(arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZOP - 1] != NULL)
          end_monitoring_min_max_measurement(arr_identifiers, buffer_for_save_dr_record, IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZOP, carrent_active_functions);
        
        //Перевіряємо чи треба завершити моніторинг мінімальної частоти для АЧР
        if(arr_identifiers[IDENTIFIER_BIT_ARRAY_MIN_FREQUENCY_ACHR - 1] != NULL)
          end_monitoring_min_max_measurement(arr_identifiers, buffer_for_save_dr_record, IDENTIFIER_BIT_ARRAY_MIN_FREQUENCY_ACHR, carrent_active_functions);
        
        /*
        Ми не первіряємо чи треба завершити моніторинг фіксації частоти для ЧАПВ,
        бо коли фіксується ця частота, то зразу моніторинг припиняється (ми не
        шукаємо мінімальну чи максимальну частоту - а фікчуємо частоту при запуску АПВ
        від ЧАПВ)
        */

        //Дальші дії виконуємо тіьлки у тому випадку, якщо функція end_monitoring_min_max_measurement не зафіксувала помилку і не скинула state_dr_record у STATE_DR_NO_RECORD
        if(state_dr_record != STATE_DR_NO_RECORD)
        {
          //Записуємо кількість зафіксованих максимальних струмів всіх типів
          buffer_for_save_dr_record[FIRST_INDEX_NUMBER_MAX_PHASE_DR     ] = number_max_phase_dr;
          buffer_for_save_dr_record[FIRST_INDEX_NUMBER_MAX_PHASE04_DR   ] = number_max_phase04_dr;
          buffer_for_save_dr_record[FIRST_INDEX_NUMBER_MAX_ZNAM_DR      ] = number_max_ZNAM_dr;
          buffer_for_save_dr_record[FIRST_INDEX_NUMBER_MAX_3I0_1G_DR    ] = number_max_3I0_dr_1g;
          buffer_for_save_dr_record[FIRST_INDEX_NUMBER_MAX_3I0_OTHERG_DR] = number_max_3I0_dr_otherg;
          buffer_for_save_dr_record[FIRST_INDEX_NUMBER_MAX_3I0_r_DR     ] = number_max_3I0_r_dr;
          buffer_for_save_dr_record[FIRST_INDEX_NUMBER_MAX_3U0_DR       ] = number_max_3U0_dr;
          buffer_for_save_dr_record[FIRST_INDEX_NUMBER_MIN_U_DR         ] = number_min_U_dr;
          buffer_for_save_dr_record[FIRST_INDEX_NUMBER_MAX_U_DR         ] = number_max_U_dr;
          buffer_for_save_dr_record[FIRST_INDEX_NUMBER_MAX_ZOP_DR       ] = number_max_ZOP_dr;
          buffer_for_save_dr_record[FIRST_INDEX_NUMBER_MIN_F_ACHR_DR    ] = number_min_f_achr_dr;
          buffer_for_save_dr_record[FIRST_INDEX_NUMBER_F_CHAPV_DR       ] = number_f_chapv_dr;

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
    if (++tail_queue_dr >= SIZE_QUEUE_DR) tail_queue_dr = 0;
    
    move_tail_queue_dr = false;
  }
    
  switch (state_dr_record)
  {
  case STATE_DR_NO_RECORD:
  case STATE_DR_FORCE_START_NEW_RECORD:
    {
      //Попередньо скидаємо невизначену помилку  роботи дискретного реєстратора
      _SET_BIT(clear_diagnostyka, ERROR_DR_UNDEFINED_BIT);
      if((saving_record_dr == false) || (tail_queue_dr != head_queue_dr))
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
          (state_dr_record == STATE_DR_FORCE_START_NEW_RECORD)
         )
      {
        //Є умова запуску дискретного реєстратора
        //global_timers[INDEX_TIMER_DR_WORK] = 0;//_TIMER_0_T(INDEX_TIMER_DR_WORK,current_settings_prt.timeout_prolongation_work_digital_registrator,1,1,0,0);
        //Перевіряємо, чи при початку нового запису ми не втратимо попередню інформацію
        if((saving_record_dr == false) || (tail_queue_dr != head_queue_dr))
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

          for(size_t i = 0; i < sizeof(time_t); i++)  buffer_for_save_dr_record[FIRST_INDEX_DATA_TIME_DR + i] = *((unsigned char*)(&time_dat) + i);
          for(size_t i = 0; i < sizeof(int32_t); i++)  buffer_for_save_dr_record[FIRST_INDEX_DATA_TIME_DR + sizeof(time_t) + i] = *((unsigned char*)(&time_ms) + i);
          
          //Додаткові налаштування при яких було запущено дискретний реєстратор
          unsigned int control_extra_settings_1_tmp = current_settings_prt.control_extra_settings_1 & (MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE) | MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_IB_I04));
          unsigned char *point_to_extra_settings = (unsigned char *)(&control_extra_settings_1_tmp);
          for (unsigned int i = 0; i < sizeof(control_extra_settings_1_tmp); i++)
            buffer_for_save_dr_record[FIRST_INDEX_EXTRA_SETTINGS_DR + i] = *(point_to_extra_settings + i);

           //І'мя комірки
          for(unsigned int i=0; i< MAX_CHAR_IN_NAME_OF_CELL; i++) 
            buffer_for_save_dr_record[FIRST_INDEX_NAME_OF_CELL_DR + i] = current_settings_prt.name_of_cell[i] & 0xff;
          
          //Коефіцієнти трансформації
          {
            unsigned char *ptr_target = buffer_for_save_dr_record + FIRST_INDEX_T0;
            unsigned char *ptr_source = (unsigned char *)(&current_settings_prt.T0);
            for(size_t i = 0; i < sizeof(current_settings_prt.T0); ++i) 
              *ptr_target++ = *ptr_source++;

            ptr_target = buffer_for_save_dr_record + FIRST_INDEX_TC;
            ptr_source = (unsigned char *)(&current_settings_prt.TCurrent);
            for(size_t i = 0; i < sizeof(current_settings_prt.TCurrent); ++i) 
              *ptr_target++ = *ptr_source++;

            ptr_target = buffer_for_save_dr_record + FIRST_INDEX_TC04;
            ptr_source = (unsigned char *)(&current_settings_prt.TCurrent04);
            for(size_t i = 0; i < sizeof(current_settings_prt.TCurrent04); ++i) 
              *ptr_target++ = *ptr_source++;

            ptr_target = buffer_for_save_dr_record + FIRST_INDEX_TV;
            ptr_source = (unsigned char *)(&current_settings_prt.TVoltage);
            for(size_t i = 0; i < sizeof(current_settings_prt.TVoltage); ++i) 
              *ptr_target++ = *ptr_source++;
          }
          

           //Джерела запуску
          for(unsigned int i = 0; i < NUMBER_BYTES_SAMPLE_DR; i++) 
            buffer_for_save_dr_record[FIRST_INDEX_SOURCE_DR + i] = *(((unsigned char*)cur_active_sources) + i);
/*
          //Записуємо попередній cтан сигналів перед аварією
          //Мітка часу попереднього стану сигналів до моменту початку запису
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR +  0] = 0x00;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR +  1] = 0x00;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR +  2] = 0x80;
          
          //Помічаємо скільки часу пройшло з початку запуску запису
          time_from_start_record_dr = 0;
          
          //Помічаємо кількість нових зрізів
          number_items_dr = 1;
		  
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*SD_DR +  0] =  time_from_start_record_dr        & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*SD_DR +  1] = (time_from_start_record_dr >> 8 ) & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*SD_DR +  2] = (time_from_start_record_dr >> 16) & 0xff;
          
          for (size_t i = 0; i < NUMBER_BYTES_SAMPLE_DR; ++i)
          {
            size_t offset = i >> 2;
            size_t shift = 8*(i & ((1u << 2) - 1));
            
            buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR                        + 3 + i] = (previous_active_functions[offset] >> shift) & 0xff;
		  
           buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*SD_DR + 3 + i] = (carrent_active_functions[offset] >> shift) & 0xff;;
          }
          //Нулем позначаємо у цій позиції кількість змін
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + 3 + NUMBER_BYTES_SAMPLE_DR + 0] = 0;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + 3 + NUMBER_BYTES_SAMPLE_DR + 1] = 0;
		  
          //Вираховуємо кількість змін сигналів
          number_changes_into_dr_record = 0;
          unsigned int number_changes_into_current_item;
          _NUMBER_CHANGES_INTO_UNSIGNED_INT_ARRAY(previous_active_functions, carrent_active_functions, N_BIG, number_changes_into_current_item);
          number_changes_into_dr_record += number_changes_into_current_item;
		  
          //Кількість змін сигналів у порівнянні із попереднім станом
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*SD_DR + 3 + NUMBER_BYTES_SAMPLE_DR + 0] = number_changes_into_current_item & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*SD_DR + 3 + NUMBER_BYTES_SAMPLE_DR + 1] = (number_changes_into_current_item >> 8) & 0xff;*/
          put_before_info_in_buf_from_queue(buffer_for_save_dr_record, &drUniqVarsAddreses);
          //put_before_info_in_buf(&drUniqVarsAddreses);FillBeforeBufinDirectOrder(&drUniqVarsAddreses);
          //Переводимо режим роботи із дискретним реєстратором у стан "Іде процес запису реєстратора"
          state_dr_record = STATE_DR_EXECUTING_RECORD;
          //Скидаємо кількість фіксацій максимальних струмів/напруг
          number_max_phase_dr = 0;
          number_max_phase04_dr = 0;
          number_max_ZNAM_dr = 0;
          number_max_3I0_dr_1g = 0;
          number_max_3I0_dr_otherg = 0;
          number_max_3I0_r_dr = 0;
          number_max_3U0_dr = 0;
          number_min_U_dr = 0;
          number_max_U_dr = 0;
          number_max_ZOP_dr = 0;
          number_min_f_achr_dr = 0;
          number_f_chapv_dr = 0;
          for (size_t i = 0; i < _NUMBER_IDENTIFIER; ++i) arr_identifiers[i] = NULL;
          next_free_array = (buffer_for_save_dr_record + FIRST_INDEX_FIRST_BLOCK_DR);
          
          
          //Перевіряємо чи стоїть умова моніторити максимальний фазний струм
          COMPARE_NOT_ZERO_OR(comp, carrent_active_functions, monitoring_phase_signals, N_BIG)
          if(comp)
          {
            start_monitoring_max_phase_current(time_from_start_record_dr, buffer_for_save_dr_record, arr_identifiers, &next_free_array, carrent_active_functions);
          }

          //Перевіряємо чи стоїть умова моніторити максимальний фазний струм сторони 0.4кВ
          COMPARE_NOT_ZERO_OR(comp, carrent_active_functions, monitoring_phase04_signals, N_BIG)
          if(comp)
          {
            start_monitoring_max_phase04_current(time_from_start_record_dr, buffer_for_save_dr_record, arr_identifiers, &next_free_array, carrent_active_functions);
          }

          //Перевіряємо чи стоїть умова моніторити відношення другої гармоніки до першої гармоніки фазного струму
          COMPARE_NOT_ZERO_OR(comp, carrent_active_functions, monitoring_znam_signals, N_BIG)
          if(comp)
          {
            start_monitoring_max_ZNAM(time_from_start_record_dr, buffer_for_save_dr_record, arr_identifiers, &next_free_array, carrent_active_functions);
          }

					//Перевіряємо чи стоїть умова моніторити максимальний струм 3I0
          monitoring_max_3I0(time_from_start_record_dr, buffer_for_save_dr_record, arr_identifiers, &next_free_array, carrent_active_functions);

          //Перевіряємо чи стоїть умова моніторити максимальну напругу 3U0
          COMPARE_NOT_ZERO_OR(comp, carrent_active_functions, monitoring_3U0_signals, N_BIG)
          if(comp)
          {
            start_monitoring_max_3U0(time_from_start_record_dr, buffer_for_save_dr_record, arr_identifiers, &next_free_array, carrent_active_functions);
          }

          //Перевіряємо чи стоїть умова моніторити максимальний струм 3I0 розрахунковий
          COMPARE_NOT_ZERO_OR(comp, carrent_active_functions, monitoring_3I0_r_signals, N_BIG)
          if(comp)
          {
            start_monitoring_max_3I0_r(time_from_start_record_dr, buffer_for_save_dr_record, arr_identifiers, &next_free_array, carrent_active_functions);
          }

          //Знімаємо блокування моніторингу мінімальної напруги
          blocking_continue_monitoring_min_U = 0;
          //Перевіряємо чи стоїть умова моніторити мінімальну напругу
          COMPARE_NOT_ZERO_OR(comp, carrent_active_functions, monitoring_Umin_signals, N_BIG)
          if(comp)
          {
            start_monitoring_min_U(time_from_start_record_dr, buffer_for_save_dr_record, arr_identifiers, &next_free_array, carrent_active_functions);
          }
          //Перевіряємо, чи стоїть умова припинити моніторинг мінімальної напруги після вимкнення вимикача
          if(arr_identifiers[IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE - 1] != NULL)
          {
            if (_CHECK_SET_BIT(carrent_active_functions, RANG_WORK_BO) != 0)
            {
              blocking_continue_monitoring_min_U = 0xff;
              end_monitoring_min_max_measurement(arr_identifiers, buffer_for_save_dr_record, IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE, carrent_active_functions);
            }
          }

          //Перевіряємо чи стоїть умова моніторити максимальну напругу
          COMPARE_NOT_ZERO_OR(comp, carrent_active_functions, monitoring_Umax_signals, N_BIG)
          if(comp)
          {
            start_monitoring_max_U(time_from_start_record_dr, buffer_for_save_dr_record, arr_identifiers, &next_free_array, carrent_active_functions);
          }

          //Перевіряємо чи стоїть умова моніторити максимальний струм зворотньої послідовності
          COMPARE_NOT_ZERO_OR(comp, carrent_active_functions, monitoring_zzp_signals, N_BIG)
          if(comp)
          {
            start_monitoring_max_ZOP(time_from_start_record_dr, buffer_for_save_dr_record, arr_identifiers, &next_free_array, carrent_active_functions);
          }
      
          //Перевіряємо чи стоїть умова моніторити мінімальну частоту
          COMPARE_NOT_ZERO_OR(comp, carrent_active_functions, monitoring_f_min_achr_signals, N_BIG)
          if(comp)
          {
            start_monitoring_min_f(time_from_start_record_dr, buffer_for_save_dr_record, arr_identifiers, &next_free_array, carrent_active_functions);
          }

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
      else state_dr_record = STATE_DR_NO_RECORD;
      
      break;
    }
  case STATE_DR_EXECUTING_RECORD:
    {
      if (
          (drDateTimeState == AVAR_DATE_TIME_NONE) &&
          (realDateTime != 0)  
         )
      {
        drDateTimeState = AVAT_DATE_TIMR_FIX_LEVEL2;
        for(size_t i = 0; i < sizeof(time_t); i++)  buffer_for_save_dr_record[FIRST_INDEX_DATA_TIME_DR + i] = *((unsigned char*)(&time_dat) + i);
        for(size_t i = 0; i < sizeof(int32_t); i++)  buffer_for_save_dr_record[FIRST_INDEX_DATA_TIME_DR + sizeof(time_t) + i] = *((unsigned char*)(&time_ms) + i);
      }

      //Збільшуємо час з початку запуску запису
      time_from_start_record_dr += DELTA_TIME_FOR_TIMERS;

      //Включно до цього часу іде процес запису

      //Контроль-фіксація максимальних аналогових сигналів
      
      //Перевіряємо чи стоїть умова моніторити максимальний фазний струм
      unsigned int comp = false;
      COMPARE_NOT_ZERO_OR(comp, carrent_active_functions, monitoring_phase_signals, N_BIG)
      if(comp)
      {
        if(arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE - 1] != NULL)
          continue_monitoring_max_phase_current(time_from_start_record_dr, buffer_for_save_dr_record, arr_identifiers, carrent_active_functions);
        else
          start_monitoring_max_phase_current(time_from_start_record_dr, buffer_for_save_dr_record, arr_identifiers, &next_free_array, carrent_active_functions);
      }
      else
      {
        if(arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE - 1] != NULL)
          end_monitoring_min_max_measurement(arr_identifiers, buffer_for_save_dr_record, IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE, carrent_active_functions);
      }

      //Перевіряємо чи стоїть умова моніторити максимальний фазний струм сторони 0.4кВ
      COMPARE_NOT_ZERO_OR(comp, carrent_active_functions, monitoring_phase04_signals, N_BIG)
      if(comp)
      {
        if(arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE04 - 1] != NULL)
          continue_monitoring_max_phase04_current(time_from_start_record_dr, buffer_for_save_dr_record, arr_identifiers, carrent_active_functions);
        else
          start_monitoring_max_phase04_current(time_from_start_record_dr, buffer_for_save_dr_record, arr_identifiers, &next_free_array, carrent_active_functions);
      }
      else
      {
        if(arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE04 - 1] != NULL)
          end_monitoring_min_max_measurement(arr_identifiers, buffer_for_save_dr_record, IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE04, carrent_active_functions);
      }

      //Перевіряємо чи стоїть умова моніторити відношення другої гармоніки до першої гармоніки фазного струму
      COMPARE_NOT_ZERO_OR(comp, carrent_active_functions, monitoring_znam_signals, N_BIG)
      if(comp)
      {
        if(arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZNAM - 1] != NULL)
          continue_monitoring_max_ZNAM(time_from_start_record_dr, buffer_for_save_dr_record, arr_identifiers, carrent_active_functions);
        else
	        start_monitoring_max_ZNAM(time_from_start_record_dr, buffer_for_save_dr_record, arr_identifiers, &next_free_array, carrent_active_functions);
      }
      else
      {
        if(arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZNAM - 1] != NULL)
          end_monitoring_min_max_measurement(arr_identifiers, buffer_for_save_dr_record, IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZNAM, carrent_active_functions);
      }
			
      //Перевіряємо чи стоїть умова моніторити максимальний струм 3I0 (перша і вищі гармоніки; аналіз старту, продовження чи закінчення  - все виконується всередині функції)
      monitoring_max_3I0(time_from_start_record_dr, buffer_for_save_dr_record, arr_identifiers, &next_free_array, carrent_active_functions);
      
      //Перевіряємо чи стоїть умова моніторити максимальний струм 3I0-1
      COMPARE_NOT_ZERO_OR(comp, carrent_active_functions, monitoring_3I0_r_signals, N_BIG)
      if(comp)
      {
        if(arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0_r - 1] != NULL)
          continue_monitoring_max_3I0_r(time_from_start_record_dr, buffer_for_save_dr_record, arr_identifiers, carrent_active_functions);
        else
          start_monitoring_max_3I0_r(time_from_start_record_dr, buffer_for_save_dr_record, arr_identifiers, &next_free_array, carrent_active_functions);
      }
      else
      {
        if(arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0_r - 1] != NULL)
          end_monitoring_min_max_measurement(arr_identifiers, buffer_for_save_dr_record, IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0_r, carrent_active_functions);
      }

      //Перевіряємо чи стоїть умова моніторити максимальну напругу 3U0
      COMPARE_NOT_ZERO_OR(comp, carrent_active_functions, monitoring_3U0_signals, N_BIG)
      if(comp)
      {
        if(arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE_3U0 - 1] != NULL)
          continue_monitoring_max_3U0(time_from_start_record_dr, buffer_for_save_dr_record, arr_identifiers, carrent_active_functions);
        else
          start_monitoring_max_3U0(time_from_start_record_dr, buffer_for_save_dr_record, arr_identifiers, &next_free_array, carrent_active_functions);
      }
      else
      {
        if(arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE_3U0 - 1] != NULL)
          end_monitoring_min_max_measurement(arr_identifiers, buffer_for_save_dr_record, IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE_3U0, carrent_active_functions);
      }
      
      //Перевіряємо чи стоїть умова відновити моніторинг мінімальної напруги
      if (_CHECK_SET_BIT(carrent_active_functions, RANG_WORK_BV) != 0)
      {
        //Знімаємо блокування моніторингу мінімальної напруги
        blocking_continue_monitoring_min_U = 0;
      }
      //Перевіряємо чи стоїть умова моніторити мінімальну напругу
      if (blocking_continue_monitoring_min_U == 0)
      {
        COMPARE_NOT_ZERO_OR(comp, carrent_active_functions, monitoring_Umin_signals, N_BIG)
        if(comp)
        {
          if(arr_identifiers[IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE - 1] != NULL)
            continue_monitoring_min_U(time_from_start_record_dr, buffer_for_save_dr_record, arr_identifiers, carrent_active_functions);
          else
            start_monitoring_min_U(time_from_start_record_dr, buffer_for_save_dr_record, arr_identifiers, &next_free_array, carrent_active_functions);
        }
        else
        {
          if(arr_identifiers[IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE - 1] != NULL)
            end_monitoring_min_max_measurement(arr_identifiers, buffer_for_save_dr_record, IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE, carrent_active_functions);
        }
      }
      //Перевіряємо, чи стоїть умова припинити моніторинг мінімальної напруги після вимкнення вимикача
      if(arr_identifiers[IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE - 1] != NULL)
      {
        if (_CHECK_SET_BIT(carrent_active_functions, RANG_WORK_BO) != 0)
        {
          blocking_continue_monitoring_min_U = 0xff;
          end_monitoring_min_max_measurement(arr_identifiers, buffer_for_save_dr_record, IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE, carrent_active_functions);
        }
      }
      
      //Перевіряємо чи стоїть умова моніторити максимальну напругу
      COMPARE_NOT_ZERO_OR(comp, carrent_active_functions, monitoring_Umax_signals, N_BIG)
      if(comp)
      {
        if(arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE - 1] != NULL)
          continue_monitoring_max_U(time_from_start_record_dr, buffer_for_save_dr_record, arr_identifiers, carrent_active_functions);
        else
          start_monitoring_max_U(time_from_start_record_dr, buffer_for_save_dr_record, arr_identifiers, &next_free_array, carrent_active_functions);
      }
      else
      {
        if(arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE - 1] != NULL)
          end_monitoring_min_max_measurement(arr_identifiers, buffer_for_save_dr_record, IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE, carrent_active_functions);
      }
      
      //Перевіряємо чи стоїть умова моніторити максимальний струм зворотньої послідовності
      COMPARE_NOT_ZERO_OR(comp, carrent_active_functions, monitoring_zzp_signals, N_BIG)
      if(comp)
      {
        if(arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZOP - 1] != NULL)
          continue_monitoring_max_ZOP(time_from_start_record_dr, buffer_for_save_dr_record, arr_identifiers, carrent_active_functions);
        else
          start_monitoring_max_ZOP(time_from_start_record_dr, buffer_for_save_dr_record, arr_identifiers, &next_free_array, carrent_active_functions);
      }
      else
      {
        if(arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZOP - 1] != NULL)
          end_monitoring_min_max_measurement(arr_identifiers, buffer_for_save_dr_record, IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZOP, carrent_active_functions);
      }
      
      //Перевіряємо чи стоїть умова моніторити мінімальну частоту для АЧР
      COMPARE_NOT_ZERO_OR(comp, carrent_active_functions, monitoring_f_min_achr_signals, N_BIG)
      if(comp)
      {
        if(arr_identifiers[IDENTIFIER_BIT_ARRAY_MIN_FREQUENCY_ACHR - 1] != NULL)
          continue_monitoring_min_f(time_from_start_record_dr, buffer_for_save_dr_record, arr_identifiers, carrent_active_functions);
        else
          start_monitoring_min_f(time_from_start_record_dr, buffer_for_save_dr_record, arr_identifiers, &next_free_array, carrent_active_functions);
      }
      else
      {
        if(arr_identifiers[IDENTIFIER_BIT_ARRAY_MIN_FREQUENCY_ACHR - 1] != NULL)
          end_monitoring_min_max_measurement(arr_identifiers, buffer_for_save_dr_record, IDENTIFIER_BIT_ARRAY_MIN_FREQUENCY_ACHR, carrent_active_functions);
      }
      
      if (
          /*Є умова дозволу подачі команди ЧАПВ*/
          (_CHECK_SET_BIT(carrent_active_functions , RANG_VN_BLOCK_CHAPV) == 0)
          &&
          (
           (
            /*Умова зафіксувати частоту від зовнішнього ЧАПВ*/
            (_CHECK_SET_BIT(previous_active_functions, RANG_ACHR_CHAPV_VID_DV) != 0) &&
            (_CHECK_SET_BIT(carrent_active_functions , RANG_ACHR_CHAPV_VID_DV) == 0)
           )
           ||
           (
            /*Умова зафіксувати частоту від ЧАПВ*/
            (_CHECK_SET_BIT(carrent_active_functions , RANG_RAZR_CHAPV) != 0)
            &&
            (
             (
              (_CHECK_SET_BIT(previous_active_functions, RANG_ACHR_CHAPV1  ) != 0) &&
              (_CHECK_SET_BIT(carrent_active_functions , RANG_ACHR_CHAPV1  ) == 0)
             )   
             ||  
             (
              (_CHECK_SET_BIT(previous_active_functions, RANG_ACHR_CHAPV2  ) != 0) &&
              (_CHECK_SET_BIT(carrent_active_functions , RANG_ACHR_CHAPV2  ) == 0)
             )   
            ) 
           )
          ) 
         )   
      {
        unsigned char * buffer = next_free_array;
        if (
            (arr_identifiers[IDENTIFIER_BIT_ARRAY_FREQUENCY_CHAPV - 1] == NULL) &&
            (buffer >= (buffer_for_save_dr_record + FIRST_INDEX_FIRST_BLOCK_DR)) && 
            ((buffer + SIZE_ARRAY_FIX_MAX_MEASUREMENTS*sizeof(unsigned int) - 1) < (buffer_for_save_dr_record + FIRST_INDEX_FIRST_DATA_DR))  
           )   
        {
          //Збільшуємо кількість фіксованих значень частоти від ЧАПВ
          ++number_f_chapv_dr;
          arr_identifiers[IDENTIFIER_BIT_ARRAY_FREQUENCY_CHAPV - 1] = buffer;
          next_free_array += SIZE_ARRAY_FIX_MAX_MEASUREMENTS*sizeof(unsigned int);

          //Помічаємо, що будем виходити з того, що зараз значення тільки починають моніторитися, тому приймаємо їх за найбільші
          __meas_to_d_meas const * p_link = im_to_idm;
          for(size_t i = 0; i <_SIZE_ARRAY_FIX_MAX_MEASUREMENTS_TMP; ++i)
          {
            buffer = serialization_uint_uchar(&(p_link->arr[p_link->ind]), buffer, 0);
      
            ++p_link;
          }
  
          //Помічаємо, що будем виходити з того, що зараз значення тільки починають моніторитися, тому приймаємо їх за найбільші
          int frequency_int = (int)frequency;
          if (frequency_int >= 0) frequency_int = (int)(frequency*1000);
          buffer = serialization_uint_uchar((unsigned int *)(&frequency_int), buffer, 0);

          unsigned int const * p_arr = vmp_start_val;
          for (size_t i = 0; i < 2; ++i)
          {
            buffer = serialization_uint_uchar(p_arr++, buffer, 0);
          }
  
          //Фіксуємо час з моменту початку аварійного запису
          buffer = serialization_uint_uchar(&time_from_start_record_dr, buffer, 1);

          //Фіксуємо причину запису
          *buffer = IDENTIFIER_BIT_ARRAY_FREQUENCY_CHAPV;

//          //Помічаємо, що ми фіксуємо чатоту від ЧАПВ
//          state_current_monitoring |= (1<<IDENTIFIER_BIT_ARRAY_FREQUENCY_CHAPV);
            
          //Зразу цю подію фіксуємо у масив, який формує поточний запис
          arr_identifiers[IDENTIFIER_BIT_ARRAY_FREQUENCY_CHAPV - 1] = NULL;
        }
        else
        {
          //Відбулася незрозуміла ситуація - сюди програма теоретично ніколи не мала б заходити
          fix_undefined_error_dr(carrent_active_functions);
        }
      }
      
      //Дальші дії виконуємо тіьлки у тому випадку, якщо функція end_monitoring_min_max_measurement не зафіксувала помилку і не скинула state_dr_record у STATE_DR_NO_RECORD
      if(state_dr_record != STATE_DR_NO_RECORD)
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
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*SD_DR +  0] =  time_from_start_record_dr        & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*SD_DR +  1] = (time_from_start_record_dr >> 8 ) & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*SD_DR +  2] = (time_from_start_record_dr >> 16) & 0xff;

          for (size_t i = 0; i < NUMBER_BYTES_SAMPLE_DR; ++i)
          {
            size_t offset = i >> 2;
            size_t shift = 8*(i & ((1u << 2) - 1));
             
           buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*SD_DR + 3 + i] = (carrent_active_functions[offset] >> shift) & 0xff;
          }
          //Кількість змін сигналів у порівнянні із попереднім станом
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*SD_DR + 3 + NUMBER_BYTES_SAMPLE_DR + 0] = number_changes_into_current_item & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*SD_DR + 3 + NUMBER_BYTES_SAMPLE_DR + 1] = (number_changes_into_current_item >> 8) & 0xff;
        }
        
        //Перевіряємо, чи стоїть умова завершення запису
        if (
            (state_dr_record == STATE_DR_MAKE_RECORD) ||
            (time_from_start_record_dr >= (MAX_TIME_OFFSET_FROM_START - DELTA_TIME_FOR_TIMERS + 1))  ||
            ((number_items_dr + 1)     >= MAX_EVENTS_IN_ONE_RECORD  ) || 
            (statePowerDown == STATE_POWER_DOWN_ETAP_CUT)  
           )
        {
          //Підтверджуємо примусовий запис у енергонезалежну пам'ять через пропадання живлення
          if (statePowerDown == STATE_POWER_DOWN_ETAP_CUT) statePowerDown = STATE_POWER_DOWN_ETAP_CUT_CONFIRMED;
          
          //Немає умови продовження запису, або є умова завершення запису - завершуємо формування запису і подаємо команду на запис
          buffer_for_save_dr_record[FIRST_INDEX_NUMBER_ITEMS_DR      ] = number_items_dr;
          buffer_for_save_dr_record[FIRST_INDEX_NUMBER_CHANGES_DR    ] =  number_changes_into_dr_record       & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_NUMBER_CHANGES_DR + 1] = (number_changes_into_dr_record >> 8) & 0xff;

          //Перевіряємо чи треба завершити моніторинг максимального фазного струму
          if(arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE - 1] != NULL)
            end_monitoring_min_max_measurement(arr_identifiers, buffer_for_save_dr_record, IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE, carrent_active_functions);

          //Перевіряємо чи треба завершити моніторинг максимального фазного струму сторони 0.4кВ
          if(arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE04 - 1] != NULL)
            end_monitoring_min_max_measurement(arr_identifiers, buffer_for_save_dr_record, IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE04, carrent_active_functions);

          //Перевіряємо чи треба завершити моніторинг максимального відношення другої гармоніки до першої гармоніки фазного струму
          if(arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZNAM - 1] != NULL)
            end_monitoring_min_max_measurement(arr_identifiers, buffer_for_save_dr_record, IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZNAM, carrent_active_functions);

          //Перевіряємо чи треба завершити моніторинг максимального струму 3I0 по першій гармоніці
          if(arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0_1G - 1] != NULL)
            end_monitoring_min_max_measurement(arr_identifiers, buffer_for_save_dr_record, IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0_1G, carrent_active_functions);

          //Перевіряємо чи треба завершити моніторинг максимального струму 3I0 по вищих гармоніках
          if(arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0_OTHERG - 1] != NULL)
            end_monitoring_min_max_measurement(arr_identifiers, buffer_for_save_dr_record, IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0_OTHERG, carrent_active_functions);

          //Перевіряємо чи треба завершити моніторинг максимальний струм розрахунковий
          if(arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0_r - 1] != NULL)
            end_monitoring_min_max_measurement(arr_identifiers, buffer_for_save_dr_record, IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0_r, carrent_active_functions);

          //Перевіряємо чи треба завершити моніторинг максимальну напругу 3U0
          if(arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE_3U0 - 1] != NULL)
            end_monitoring_min_max_measurement(arr_identifiers, buffer_for_save_dr_record, IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE_3U0, carrent_active_functions);

          //Перевіряємо чи треба завершити моніторинг мінімальну напругу
          if(arr_identifiers[IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE - 1] != NULL)
            end_monitoring_min_max_measurement(arr_identifiers, buffer_for_save_dr_record, IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE, carrent_active_functions);

          //Перевіряємо чи треба завершити моніторинг максимальну напругу
          if(arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE - 1] != NULL)
            end_monitoring_min_max_measurement(arr_identifiers, buffer_for_save_dr_record, IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE, carrent_active_functions);

          //Перевіряємо чи треба завершити моніторинг максимального струму послідовності
          if(arr_identifiers[IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZOP - 1] != NULL)
            end_monitoring_min_max_measurement(arr_identifiers, buffer_for_save_dr_record, IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZOP, carrent_active_functions);
        
          //Перевіряємо чи треба завершити моніторинг мінімальну частоту для АЧР
          if(arr_identifiers[IDENTIFIER_BIT_ARRAY_MIN_FREQUENCY_ACHR - 1] != NULL)
            end_monitoring_min_max_measurement(arr_identifiers, buffer_for_save_dr_record, IDENTIFIER_BIT_ARRAY_MIN_FREQUENCY_ACHR, carrent_active_functions);

          /*
          Перевірку необхідності запису фіксація вимірювань при роботі ЧАПВ здійснювати
          не потрібно, тому що зараз іде перевірка чи треба примусово зупинити робити формування
          поточного запису. Якщо була ситуація заауску АПВ від ЧАПВ, то ця подія мала б зафіксованою бути
          у місці програми, де аналізувалися блоки інтегральних величин
          */

          //Дальші дії виконуємо тіьлки у тому випадку, якщо функція end_monitoring_min_max_measurement не зафіксувала помилку і не скинула state_dr_record у STATE_DR_NO_RECORD
          if(state_dr_record != STATE_DR_NO_RECORD)
          {
            //Записуємо кількість зафіксованих максимальних вимірювань всіх типів
            buffer_for_save_dr_record[FIRST_INDEX_NUMBER_MAX_PHASE_DR     ] = number_max_phase_dr;
            buffer_for_save_dr_record[FIRST_INDEX_NUMBER_MAX_PHASE04_DR   ] = number_max_phase04_dr;
            buffer_for_save_dr_record[FIRST_INDEX_NUMBER_MAX_ZNAM_DR      ] = number_max_ZNAM_dr;
            buffer_for_save_dr_record[FIRST_INDEX_NUMBER_MAX_3I0_1G_DR    ] = number_max_3I0_dr_1g;
            buffer_for_save_dr_record[FIRST_INDEX_NUMBER_MAX_3I0_OTHERG_DR] = number_max_3I0_dr_otherg;
            buffer_for_save_dr_record[FIRST_INDEX_NUMBER_MAX_3I0_r_DR     ] = number_max_3I0_r_dr;
            buffer_for_save_dr_record[FIRST_INDEX_NUMBER_MAX_3U0_DR       ] = number_max_3U0_dr;
            buffer_for_save_dr_record[FIRST_INDEX_NUMBER_MIN_U_DR         ] = number_min_U_dr;
            buffer_for_save_dr_record[FIRST_INDEX_NUMBER_MAX_U_DR         ] = number_max_U_dr;
            buffer_for_save_dr_record[FIRST_INDEX_NUMBER_MAX_ZOP_DR       ] = number_max_ZOP_dr;
            buffer_for_save_dr_record[FIRST_INDEX_NUMBER_MIN_F_ACHR_DR    ] = number_min_f_achr_dr;
            buffer_for_save_dr_record[FIRST_INDEX_NUMBER_F_CHAPV_DR       ] = number_f_chapv_dr;

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
  for (unsigned int i = 0; i < N_BIG; i++) previous_active_functions[i] = carrent_active_functions[i];
  /*********************/
}
/*****************************************************/

/*****************************************************/
//Функція обробки логіки дискретного реєстратора
/*****************************************************/
inline void analog_registrator(unsigned int* carrent_active_functions)
{
  static unsigned int prev_active_sources[N_BIG];
  unsigned int cur_active_sources[N_BIG];
  for(size_t i = 0; i < N_BIG; ++i) cur_active_sources[i] = carrent_active_functions[i] & current_settings_prt.ranguvannja_analog_registrator[i];

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
    if(comp) 
    {
      //Перша умова розблокування можливості початку нового запису виконана
      forbidden_new_record_ar_mode_0 = 0;
    }
    else if ((current_settings_prt.control_ar & MASKA_FOR_BIT(INDEX_ML_CTR_AR_AVAR_STATE)) == 0)
    {
      //Попередній запис повністю записаний у DataFlash, але ще деякі джерела активації не деакттивувалися
      unsigned int diff_active_sources[N_BIG];
      for(size_t i = 0; i < N_BIG; ++i) diff_active_sources[i] = prev_active_sources[i] ^ cur_active_sources[i];

      COMPARE_NOT_ZERO_OR(comp, diff_active_sources, cur_active_sources, N_BIG)
      if(comp) 
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
      (state_ar_record_fatfs == STATE_AR_BLOCK_FATFS)
     )   
  {
    if (global_timers[INDEX_TIMER_FULL_AR_RECORD] >= MAX_TIME_FULL_AR_RECORD) _SET_BIT(set_diagnostyka, ERROR_AR_TEMPORARY_BUSY_BIT);

    state_ar_record_prt = STATE_AR_BLOCK_PRT;
    global_timers[INDEX_TIMER_POSTFAULT] = -1;
    global_timers[INDEX_TIMER_FULL_AR_RECORD] = -1;
  }
  
  if (
      (arDateTimeState == AVAR_DATE_TIME_NONE) &&
      (state_ar_record_fatfs != STATE_AR_NONE_FATFS) &&
      (state_ar_record_fatfs != STATE_AR_BLOCK_FATFS) &&
      (realDateTime != 0)  
     )
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
           (comp)
           &&  
           (forbidden_new_record_ar_mode_0 == 0) /*при попередній роботі ан.реєстротора (якщо така була) вже всі джерела активації були зняті і зароз вони знову виникли*/ 
          )
          ||
          (start_ar != 0) 
         )    
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
          
            prefault_number_periods_tmp = ((POWER_CTRL->IDR & POWER_CTRL_PIN) != (uint32_t)Bit_RESET) ? current_settings_prt.prefault_number_periods : AR_TAIL_MIN_NUMBER_PERIOD;
            global_timers[INDEX_TIMER_FULL_AR_RECORD] =  20*prefault_number_periods_tmp; //Запускаємо таймер цілого запису  з врахуванням що буде доданий доаварійний масив

          
            //Записуємо мітку початку запису
            header_ar.label_start_record = LABEL_START_RECORD_AR;
            //Записуємо час початку запису
            arDateTimeState = (realDateTime != 0) ? AVAT_DATE_TIMR_FIX_LEVEL2 : AVAR_DATE_TIME_NONE;
            header_ar.time_dat = time_dat;
            header_ar.time_ms = time_ms;

            //Коефіцієнт трансформації T0
            header_ar.T0 = current_settings_prt.T0;
            //Коефіцієнт трансформації TT
            header_ar.TCurrent = current_settings_prt.TCurrent;
            //Коефіцієнт трансформації TT сторони 0.4кВ
            header_ar.TCurrent04 = current_settings_prt.TCurrent04;
            //Коефіцієнт трансформації TН
            header_ar.TVoltage = current_settings_prt.TVoltage;
            
            //Додаткові налаштування при яких було запущено аналоговий реєстратор
            header_ar.control_extra_settings_1 = current_settings_prt.control_extra_settings_1 & (MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_IB_I04) | (MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE)));

            //Час доаварійного масиву
            header_ar.prefault_number_periods = prefault_number_periods_tmp;

            //І'мя комірки
            for(unsigned int i=0; i<MAX_CHAR_IN_NAME_OF_CELL; i++)
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
          (comp)
          || 
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
          (comp)
         )
      {
        //Повертаємося до аварійного процесу
        state_ar_record_prt = STATE_AR_AVAR_PRT;
        global_timers[INDEX_TIMER_POSTFAULT] = -1; //Зупиняємо таймер післяаварійного процесу
      }
      else if (global_timers[INDEX_TIMER_POSTFAULT] >= (int)(20*current_settings_prt.postfault_number_periods))
      {
        //Завершився післяаварійний процес
        global_timers[INDEX_TIMER_POSTFAULT] = -1; //Зупиняємо таймер післяаварійного процесу
        state_ar_record_prt = STATE_AR_NONE_PRT;
        
        if (
            /*перевірку на те, що режим "Власнеаварійний процес" увімкнутий не треба, бо при умові активних джерел ми б попали у попередню умову де з післяаваріного процесу йде поворот до аварійного процесу*/
            (comp)
           )
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
          (state_ar_record_m == STATE_AR_NONE_M)
         )
      {
        state_ar_record_prt = STATE_AR_NONE_PRT;
       
        NOT_ZERO_OR(comp, cur_active_sources, N_BIG)
        if(comp)   
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
      (state_ar_record_prt == STATE_AR_POSTAVAR_PRT)
     )
  {
    _SET_BIT(carrent_active_functions, RANG_WORK_A_REJESTRATOR);
    if (truncPrefault)
    {
      truncPrefault = false;
      int trunc = 20*(prefault_number_periods_tmp - AR_TAIL_MIN_NUMBER_PERIOD);
      if (
          (trunc > 0) && //перестраховка, бо завжди б так мало бути у цій ситуації
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

  for (size_t i = 0; i != N_BIG; ++i) prev_active_sources[i] = cur_active_sources[i];
  
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
    COMMAND_PERMITTED_FROM_KP_SIGNALES_2, 
    COMMAND_PERMITTED_FROM_KP_SIGNALES_3 
  };
#endif
	
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
  /***
  Зафіксувати чи є активними сигнали блокування Вх.GOOSE блоків і Вх.MMS блоків
  ***/
enum WRP_GOOSE_BIT_POS { 
    GS_BLOCK_ORD_NUM_00,//0
    GS_BLOCK_ORD_NUM_01,//1
    GS_BLOCK_ORD_NUM_02,//2
    GS_BLOCK_ORD_NUM_03,//3
    GS_BLOCK_ORD_NUM_04,//4
    GS_BLOCK_ORD_NUM_05,//5
    GS_BLOCK_ORD_NUM_06,//6
    GS_BLOCK_ORD_NUM_07,//7
    GS_BLOCK_ORD_NUM_08,//8
    GS_BLOCK_ORD_NUM_09,//9
    GS_BLOCK_ORD_NUM_10,//10
    GS_BLOCK_ORD_NUM_11,//11
    GS_BLOCK_ORD_NUM_12,//12
    GS_BLOCK_ORD_NUM_13,//13
    GS_BLOCK_ORD_NUM_14,//14
    GS_BLOCK_ORD_NUM_15,//15

TOTAL_GOOSE_BLOCK
//
};
enum WRP_MMS_BIT_POS { 
    MMS_BLOCK_ORD_NUM_00,//0
    MMS_BLOCK_ORD_NUM_01,//1
    MMS_BLOCK_ORD_NUM_02,//2
    MMS_BLOCK_ORD_NUM_03,//3
TOTAL_MMS_BLOCK
//
};
enum WRP_LAN_BIT_POS { 
    LAN_BLOCK_ORD_NUM_00,//0
    LAN_BLOCK_ORD_NUM_01,//1
    LAN_BLOCK_ORD_NUM_02,//2
    LAN_BLOCK_ORD_NUM_03,//3
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
  
typedef union mms_block_Unn{ 
    unsigned char arCh[4];
    unsigned short arSh[2];
    unsigned long Mms_Vl;
    struct{
        struct {
            unsigned int in1 : 1; //0
            unsigned int in2 : 1; //1
            unsigned int in3 : 1; //2
            unsigned int in4 : 1; //3
            unsigned int in5 : 1; //4 
            unsigned int in6 : 1; //5 
            unsigned int in7 : 1; //6 
            unsigned int in8 : 1; //7    
        } in_bool; //
    unsigned char Blk;
    struct {
        unsigned int out1 : 1; //0
        unsigned int out2 : 1; //1
        unsigned int out3 : 1; //2
        unsigned int out4 : 1; //3
        unsigned int out5 : 1; //4 
        unsigned int out6 : 1; //5 
        unsigned int out7 : 1; //6 
        unsigned int out8 : 1; //7    
    } out_bool; //unsigned char Out;
    unsigned char Res;
   }Mms_Hld;
    
} MmsBlock_stt; 
typedef union goose_block_Unn{ 
   unsigned char arCh[4];
   unsigned short arSh[2];
   unsigned long GS_Vl;
   struct{
        struct {
            unsigned int in1 : 1; //0
            unsigned int in2 : 1; //1
            unsigned int in3 : 1; //2
            unsigned int in4 : 1; //3
            unsigned int in5 : 1; //4 
            unsigned int in6 : 1; //5 
            unsigned int in7 : 1; //6 
            unsigned int in8 : 1; //7    
        } in_bool; //
    unsigned char Blk;
    struct {
        unsigned int out1 : 1; //0
        unsigned int out2 : 1; //1
        unsigned int out3 : 1; //2
        unsigned int out4 : 1; //3
        unsigned int out5 : 1; //4 
        unsigned int out6 : 1; //5 
        unsigned int out7 : 1; //6 
        unsigned int out8 : 1; //7    
    } out_bool; //unsigned char Out;
    unsigned char Res;
   }GS_Hld;
    
} GsBlock_stt; //
typedef struct tag_GsBlkParam
{
    union {
        unsigned char   u8Ar [4];
        unsigned short  u16Ar [2];
        unsigned long   ulV;
        struct {
        unsigned char OrdNumGsBlk;
        //unsigned char Res;
        } GsBlkHdr;
    }unnV1[N_IN_GOOSE];
    //void* pAddrGsBlk[N_IN_GOOSE];
    char ch_amount_blk_src;
    char ch_amount_active_src;
    short sh_amount_gs_active_src;//use whith arrOrdNumsGsSignal
}GsBlkParamDsc;
typedef struct tag_MmsBlkParam
{
    union {
        unsigned char   u8Ar [4];
        unsigned short  u16Ar [2];
        unsigned long   ulV;
        struct {
        unsigned char OrdNumMmsBlk;
        //unsigned char Res;
        } MmsBlkHdr;
    }unnV1[N_IN_MMS];
    //void* pAddrGsBlk[N_IN_MMS];
    char ch_amount_blk_src;
    char ch_amount_active_src;
    short sh_amount_mms_active_src;//use whith arrOrdNumsGsSignal
}MmsBlkParamDsc;
typedef struct tag_LanBlkParam
{
    union {
        unsigned char   u8Ar [4];
        unsigned short  u16Ar [2];
        unsigned long   ulV;
        struct {
        unsigned char OrdNumLanBlk;
        //unsigned char Res;
        } LanBlkHdr;
    }unnV1[N_OUT_LAN];
    //void* pAddrGsBlk[N_OUT_LAN];

}LanBlkParamDsc;

static GsBlock_stt  arrGsBlk [N_IN_GOOSE];
static MmsBlock_stt arrMmsBlk[N_IN_MMS  ];
//static LanBlock_stt arrLanBlk[N_OUT_LAN ];

static GsBlkParamDsc hldGsBlkParam;
static MmsBlkParamDsc hldMmsBlkParam;
static LanBlkParamDsc hldLanBlkParam = {
    LAN_BLOCK_ORD_NUM_00,0,0,0,
    LAN_BLOCK_ORD_NUM_01,0,0,0,
    LAN_BLOCK_ORD_NUM_02,0,0,0,
    LAN_BLOCK_ORD_NUM_03,0,0,0
};

static short arrOrdNumsGsSignal [N_IN_GOOSE*N_IN_GOOSE_MMS_OUT];
static short arrOrdNumsMmsSignal[N_IN_MMS  *N_IN_GOOSE_MMS_OUT];



struct{
    char  ch_while_breaker;
    char ch_amount_blk_src,ch_amount_mms_blk_src;
    char ch_amount_active_src,ch_amount_mms_active_src;
    char ch_amount_GsSignal,ch_amount_MmsSignal;
    __SETTINGS *p_current_settings_prt;
    unsigned int *p_active_functions;

}sLV;
    sLV.p_active_functions = &active_functions[0];
  //Check GOOSE Blk Cmd
  //RANG_BLOCK_IN_GOOSE1
//=====================================================================================================
//''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
//     GOOSE BLOCK             
//....................................................................................................
//=====================================================================================================
    do{
     sLV.ch_amount_active_src = sLV.ch_amount_blk_src = sLV.ch_while_breaker = 0;//! optimize then
     sLV.ch_amount_GsSignal =  sLV.ch_amount_MmsSignal = 0;
     register long i,lV;
     //register void* pv;
     register unsigned int *r_p_active_functions;
// ----------------    -------------------------
//    i = (RANG_BLOCK_IN_GOOSE1>>5);               <- old code
//    lV = sLV.p_active_functions[i];//whole word  <- old code
//    sLV.wrp.lVl = lV;                            <- old code
    //pv = (void*) sLV.p_active_functions;
    r_p_active_functions = sLV.p_active_functions;
    
    if(_CHECK_SET_BIT(((unsigned int*)r_p_active_functions), RANG_BLOCK_IN_GOOSE1) != 0){
    //clr block
        i = GS_BLOCK_ORD_NUM_15 - sLV.ch_amount_blk_src;
        hldGsBlkParam.unnV1[i].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_00;
        sLV.ch_amount_blk_src++;
		
		arrGsBlk[GS_BLOCK_ORD_NUM_00].GS_Hld.Blk = 1;
		
    }else{
        lV = sLV.ch_amount_active_src;
        hldGsBlkParam.unnV1[lV].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_00;
        sLV.ch_amount_active_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_00].GS_Hld.Blk = 0;
		
    }
	
    if(_CHECK_SET_BIT( ((unsigned int*)r_p_active_functions), (RANG_BLOCK_IN_GOOSE1 + 1)) != 0){
    //clr block
        i = GS_BLOCK_ORD_NUM_15 - sLV.ch_amount_blk_src;
        hldGsBlkParam.unnV1[i].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_01;
        sLV.ch_amount_blk_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_01].GS_Hld.Blk = 1;
    }else{
        //Insert
        lV = sLV.ch_amount_active_src;
        hldGsBlkParam.unnV1[lV].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_01;
        sLV.ch_amount_active_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_01].GS_Hld.Blk = 0;
    }
    if(_CHECK_SET_BIT( ((unsigned int*)r_p_active_functions), (RANG_BLOCK_IN_GOOSE1 + 2)) != 0){
    //clr block
        i = GS_BLOCK_ORD_NUM_15 - sLV.ch_amount_blk_src;
        hldGsBlkParam.unnV1[i].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_02;
        sLV.ch_amount_blk_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_02].GS_Hld.Blk = 1;
    }else{
        lV = sLV.ch_amount_active_src;
        hldGsBlkParam.unnV1[lV].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_02;
        sLV.ch_amount_active_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_02].GS_Hld.Blk = 0;
    }
    if(_CHECK_SET_BIT( ((unsigned int*)r_p_active_functions), (RANG_BLOCK_IN_GOOSE1 + 3)) != 0){
    //clr block
        i = GS_BLOCK_ORD_NUM_15 - sLV.ch_amount_blk_src;
        hldGsBlkParam.unnV1[i].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_03;
        sLV.ch_amount_blk_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_03].GS_Hld.Blk = 1;
    }else{
        lV = sLV.ch_amount_active_src;
        hldGsBlkParam.unnV1[lV].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_03;
        sLV.ch_amount_active_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_03].GS_Hld.Blk = 0;
    }
    if(_CHECK_SET_BIT( ((unsigned int*)r_p_active_functions), (RANG_BLOCK_IN_GOOSE1 + 4)) != 0){
    //clr block
        i = GS_BLOCK_ORD_NUM_15 - sLV.ch_amount_blk_src;
        hldGsBlkParam.unnV1[i].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_04;
        sLV.ch_amount_blk_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_04].GS_Hld.Blk = 1;
    }else{
        lV = sLV.ch_amount_active_src;
        hldGsBlkParam.unnV1[lV].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_04;
        sLV.ch_amount_active_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_04].GS_Hld.Blk = 0;
    }
    if(_CHECK_SET_BIT( ((unsigned int*)r_p_active_functions), (RANG_BLOCK_IN_GOOSE1 + 5)) != 0){
    //clr block
        i = GS_BLOCK_ORD_NUM_15 - sLV.ch_amount_blk_src;
        hldGsBlkParam.unnV1[i].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_05;
        sLV.ch_amount_blk_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_05].GS_Hld.Blk = 1;
    }else{
        lV = sLV.ch_amount_active_src;
        hldGsBlkParam.unnV1[lV].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_05;
        sLV.ch_amount_active_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_05].GS_Hld.Blk = 0;
    }
    if(_CHECK_SET_BIT( ((unsigned int*)r_p_active_functions), (RANG_BLOCK_IN_GOOSE1 + 6)) != 0){
    //clr block
        i = GS_BLOCK_ORD_NUM_15 - sLV.ch_amount_blk_src;
        hldGsBlkParam.unnV1[i].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_06;
        sLV.ch_amount_blk_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_06].GS_Hld.Blk = 1;
    }else{
        lV = sLV.ch_amount_active_src;
        hldGsBlkParam.unnV1[lV].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_06;
        sLV.ch_amount_active_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_06].GS_Hld.Blk = 0;
    }
    if(_CHECK_SET_BIT( ((unsigned int*)r_p_active_functions), (RANG_BLOCK_IN_GOOSE1 + 7)) != 0){
    //clr block
        i = GS_BLOCK_ORD_NUM_15 - sLV.ch_amount_blk_src;
        hldGsBlkParam.unnV1[i].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_07;
        sLV.ch_amount_blk_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_07].GS_Hld.Blk = 1;
    }else{
        lV = sLV.ch_amount_active_src;
        hldGsBlkParam.unnV1[lV].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_07;
        sLV.ch_amount_active_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_07].GS_Hld.Blk = 0;
    }
    if(_CHECK_SET_BIT( ((unsigned int*)r_p_active_functions), (RANG_BLOCK_IN_GOOSE1 + 8)) != 0){
    //clr block
                i = GS_BLOCK_ORD_NUM_15 - sLV.ch_amount_blk_src;
        hldGsBlkParam.unnV1[i].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_08;
        sLV.ch_amount_blk_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_08].GS_Hld.Blk = 1;
    }else{
        lV = sLV.ch_amount_active_src;
        hldGsBlkParam.unnV1[lV].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_08;
        sLV.ch_amount_active_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_08].GS_Hld.Blk = 0;
    }
    if(_CHECK_SET_BIT( ((unsigned int*)r_p_active_functions), (RANG_BLOCK_IN_GOOSE1 + 9)) != 0){
    //clr block
        i = GS_BLOCK_ORD_NUM_15 - sLV.ch_amount_blk_src;
        hldGsBlkParam.unnV1[i].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_09;
        sLV.ch_amount_blk_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_09].GS_Hld.Blk = 1;
    }else{
        lV = sLV.ch_amount_active_src;
        hldGsBlkParam.unnV1[lV].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_09;
        sLV.ch_amount_active_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_09].GS_Hld.Blk = 0;
    }
    if(_CHECK_SET_BIT( ((unsigned int*)r_p_active_functions), (RANG_BLOCK_IN_GOOSE1 + 10)) != 0){
    //clr block
        i = GS_BLOCK_ORD_NUM_15 - sLV.ch_amount_blk_src;
        hldGsBlkParam.unnV1[i].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_10;
        sLV.ch_amount_blk_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_10].GS_Hld.Blk = 1;
    }else{
        lV = sLV.ch_amount_active_src;
        hldGsBlkParam.unnV1[lV].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_10;
        sLV.ch_amount_active_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_10].GS_Hld.Blk = 0;
    }
    if(_CHECK_SET_BIT( ((unsigned int*)r_p_active_functions), (RANG_BLOCK_IN_GOOSE1 + 11)) != 0){
    //clr block
        i = GS_BLOCK_ORD_NUM_15 - sLV.ch_amount_blk_src;
        hldGsBlkParam.unnV1[i].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_11;
        sLV.ch_amount_blk_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_11].GS_Hld.Blk = 1;
    }else{
        lV = sLV.ch_amount_active_src;
        hldGsBlkParam.unnV1[lV].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_11;
        sLV.ch_amount_active_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_11].GS_Hld.Blk = 0;
    }
    if(_CHECK_SET_BIT( ((unsigned int*)r_p_active_functions), (RANG_BLOCK_IN_GOOSE1 + 12)) != 0){
    //clr block
        i = GS_BLOCK_ORD_NUM_15 - sLV.ch_amount_blk_src;
        hldGsBlkParam.unnV1[i].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_12;
        sLV.ch_amount_blk_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_12].GS_Hld.Blk = 1;
    }else{
        lV = sLV.ch_amount_active_src;
        hldGsBlkParam.unnV1[lV].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_12;
        sLV.ch_amount_active_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_12].GS_Hld.Blk = 0;
    }
    if(_CHECK_SET_BIT( ((unsigned int*)r_p_active_functions), (RANG_BLOCK_IN_GOOSE1 + 13)) != 0){
    //clr block
        i = GS_BLOCK_ORD_NUM_15 - sLV.ch_amount_blk_src;
        hldGsBlkParam.unnV1[i].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_13;
        sLV.ch_amount_blk_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_13].GS_Hld.Blk = 1;
    }else{
        lV = sLV.ch_amount_active_src;
        hldGsBlkParam.unnV1[lV].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_13;
        sLV.ch_amount_active_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_13].GS_Hld.Blk = 0;
    }
    if(_CHECK_SET_BIT( ((unsigned int*)r_p_active_functions), (RANG_BLOCK_IN_GOOSE1 + 14)) != 0){
    //clr block
        i = GS_BLOCK_ORD_NUM_15 - sLV.ch_amount_blk_src;
        hldGsBlkParam.unnV1[i].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_14;
        sLV.ch_amount_blk_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_14].GS_Hld.Blk = 1;
    }else{
        lV = sLV.ch_amount_active_src;
        hldGsBlkParam.unnV1[lV].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_14;
        sLV.ch_amount_active_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_14].GS_Hld.Blk = 0;
    }
    if(_CHECK_SET_BIT( ((unsigned int*)r_p_active_functions), (RANG_BLOCK_IN_GOOSE1 + 15)) != 0){
    //clr block
        i = GS_BLOCK_ORD_NUM_15 - sLV.ch_amount_blk_src;
        hldGsBlkParam.unnV1[i].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_15;
        sLV.ch_amount_blk_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_15].GS_Hld.Blk = 1;
    }else{
        lV = sLV.ch_amount_active_src;
        hldGsBlkParam.unnV1[lV].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_15;
        sLV.ch_amount_active_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_15].GS_Hld.Blk = 0;
    }
        hldGsBlkParam.ch_amount_blk_src    = sLV.ch_amount_blk_src;
        hldGsBlkParam.ch_amount_active_src = sLV.ch_amount_active_src;


    }while(sLV.ch_while_breaker);
//
//--------------------------------------------------------------------------------------------------------
//````````````````````````````````````````````````````````````````````````````````````````````````````````


//=====================================================================================================
//''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
//             MMS BLOCK     
//....................................................................................................
//=====================================================================================================
do{
    sLV.ch_while_breaker = 0;sLV.ch_amount_mms_blk_src = sLV.ch_amount_mms_active_src = 0;//! optimize then
    register long i,lV;
    register void* pv;
    
     register unsigned int *r_p_active_functions;
// ----------------    -------------------------
    r_p_active_functions = sLV.p_active_functions;
    pv = (void*)&hldMmsBlkParam;
    if(_CHECK_SET_BIT(((unsigned int*)r_p_active_functions), (RANG_BLOCK_IN_MMS1 + ((unsigned int)MMS_BLOCK_ORD_NUM_00))) != 0){
    //clr block
        i = MMS_BLOCK_ORD_NUM_03 - sLV.ch_amount_mms_blk_src;
        ((MmsBlkParamDsc*)pv)->unnV1[i].MmsBlkHdr.OrdNumMmsBlk = MMS_BLOCK_ORD_NUM_00;
        sLV.ch_amount_mms_blk_src++;
		arrMmsBlk[MMS_BLOCK_ORD_NUM_00].Mms_Hld.Blk = 1;
    }else{
        lV = sLV.ch_amount_mms_active_src;
        //hldGsBlkParam.pAddrGsBlk[lV] = (void*)(&arrMmsBlk[GS_BLOCK_ORD_NUM_00]);
        hldMmsBlkParam.unnV1[lV].MmsBlkHdr.OrdNumMmsBlk = MMS_BLOCK_ORD_NUM_00;
		//hldMmsBlkParam.unnV1[MMS_BLOCK_ORD_NUM_00].u8Ar[] = 
		//arrMmsBlk[MMS_BLOCK_ORD_NUM_00].arCh[2] = 0;
        sLV.ch_amount_mms_active_src++;
		arrMmsBlk[MMS_BLOCK_ORD_NUM_00].Mms_Hld.Blk = 0;
    }

    if(_CHECK_SET_BIT(((unsigned int*)r_p_active_functions), (RANG_BLOCK_IN_MMS1+((unsigned int)MMS_BLOCK_ORD_NUM_01))) != 0){
    //clr block
        i = MMS_BLOCK_ORD_NUM_03 - sLV.ch_amount_mms_blk_src;
        hldMmsBlkParam.unnV1[i].MmsBlkHdr.OrdNumMmsBlk = MMS_BLOCK_ORD_NUM_01;
        sLV.ch_amount_mms_blk_src++;
		arrMmsBlk[MMS_BLOCK_ORD_NUM_01].Mms_Hld.Blk = 1;
    }else{
        lV = sLV.ch_amount_mms_active_src;
        //hldGsBlkParam.pAddrGsBlk[lV] = (void*)(&arrMmsBlk[GS_BLOCK_ORD_NUM_00]);
        hldMmsBlkParam.unnV1[lV].MmsBlkHdr.OrdNumMmsBlk = MMS_BLOCK_ORD_NUM_01;
        sLV.ch_amount_mms_active_src++;
		arrMmsBlk[MMS_BLOCK_ORD_NUM_01].Mms_Hld.Blk = 0;
    }

    if(_CHECK_SET_BIT(((unsigned int*)r_p_active_functions), (RANG_BLOCK_IN_MMS1+((unsigned int)MMS_BLOCK_ORD_NUM_02))) != 0){
    //clr block
        i = MMS_BLOCK_ORD_NUM_03 - sLV.ch_amount_mms_blk_src;
        hldMmsBlkParam.unnV1[i].MmsBlkHdr.OrdNumMmsBlk = MMS_BLOCK_ORD_NUM_02;
        sLV.ch_amount_mms_blk_src++;
		arrMmsBlk[MMS_BLOCK_ORD_NUM_02].Mms_Hld.Blk = 1;
    }else{
        lV = sLV.ch_amount_mms_active_src;
        //hldGsBlkParam.pAddrGsBlk[lV] = (void*)(&arrMmsBlk[GS_BLOCK_ORD_NUM_00]);
        hldMmsBlkParam.unnV1[lV].MmsBlkHdr.OrdNumMmsBlk = MMS_BLOCK_ORD_NUM_02;
        sLV.ch_amount_mms_active_src++;
		arrMmsBlk[MMS_BLOCK_ORD_NUM_02].Mms_Hld.Blk = 0;
    }

    if(_CHECK_SET_BIT(((unsigned int*)r_p_active_functions), (RANG_BLOCK_IN_MMS1+((unsigned int)MMS_BLOCK_ORD_NUM_03))) != 0){
    //clr block
        i = MMS_BLOCK_ORD_NUM_03 - sLV.ch_amount_mms_blk_src;
        hldMmsBlkParam.unnV1[i].MmsBlkHdr.OrdNumMmsBlk = MMS_BLOCK_ORD_NUM_03;
        sLV.ch_amount_mms_blk_src++;
		arrMmsBlk[MMS_BLOCK_ORD_NUM_03].Mms_Hld.Blk = 1;
    }else{
        lV = sLV.ch_amount_mms_active_src;
        //hldGsBlkParam.pAddrGsBlk[lV] = (void*)(&arrMmsBlk[GS_BLOCK_ORD_NUM_00]);
        hldMmsBlkParam.unnV1[lV].MmsBlkHdr.OrdNumMmsBlk = MMS_BLOCK_ORD_NUM_03;
        sLV.ch_amount_mms_active_src++;
		arrMmsBlk[MMS_BLOCK_ORD_NUM_03].Mms_Hld.Blk = 0;
    }

    hldMmsBlkParam.ch_amount_blk_src    = sLV.ch_amount_blk_src;
    hldMmsBlkParam.ch_amount_active_src = sLV.ch_amount_active_src;
	if(_CHECK_SET_BIT(((unsigned int*)r_p_active_functions), (RANG_MISCEVE_DYSTANCIJNE )) != 1)
		hldMmsBlkParam.ch_amount_blk_src |= 0x80;



   
}while(sLV.ch_while_breaker);
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
    MASKA_FOR_INPUT_SIGNALS_6, 
    MASKA_FOR_INPUT_SIGNALS_7,
    MASKA_FOR_INPUT_SIGNALS_8
      
#ifdef MASKA_FOR_INPUT_SIGNALS_9
                             ,
    MASKA_FOR_INPUT_SIGNALS_9
#endif
  };
  for (unsigned int i = 0; i < N_BIG; i++) active_functions[i] &= (unsigned int)(~maska_input_signals[i]);
  
  /**************************/
  //Перевірка, чи треба очистити трігерні функції
  /**************************/
  if (reset_trigger_function_from_interface !=0)
  {
    if ((reset_trigger_function_from_interface & (1 << USB_RECUEST)) != 0)
    {
      for (unsigned int i = 0; i < N_BIG; i++) trigger_functions_USB[i] = 0;
      
      information_about_restart_counter  &= (unsigned int)(~(1 << USB_RECUEST));
      information_about_clean_energy     &= (unsigned int)(~(1 << USB_RECUEST));
    }
    if ((reset_trigger_function_from_interface & (1 << RS485_RECUEST)) != 0)
    {
      for (unsigned int i = 0; i < N_BIG; i++) trigger_functions_RS485[i] = 0;
      
      information_about_restart_counter  &= (unsigned int)(~(1 << RS485_RECUEST));
      information_about_clean_energy     &= (unsigned int)(~(1 << RS485_RECUEST));
    }
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
    if ((reset_trigger_function_from_interface & (1 << LAN_RECUEST)) != 0)
    {
      for (unsigned int i = 0; i < N_BIG; i++) trigger_functions_LAN[i] = 0;
      
      information_about_restart_counter  &= (unsigned int)(~(1 << LAN_RECUEST));
      information_about_clean_energy     &= (unsigned int)(~(1 << LAN_RECUEST));
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
      (pressed_buttons != 0)
     )   
  {
    pressed_buttons_tmp = pressed_buttons;
    //Очищаємо натиснуті кнопкb, інформацію про яких ми вже забрали у роботу (аде ще не опрацювали)
    pressed_buttons = 0;
  }
      
  uint32_t pressed_buttons_switcher = pressed_buttons_tmp & buttons_mode_tmp; /*натиснуті в даний момент кнопки-ключі*/
  pressed_buttons_tmp &= (uint32_t)(~buttons_mode_tmp); /*натиснуті в даний момент всі інші типи кнопок*/
      
  //Опрацьвуємо спочатку кнопки-ключі
  uint32_t const fix_active_buttons_tmp = (_CHECK_SET_BIT(active_functions, RANG_AVAR_DEFECT) == 0) ?
    ((fix_active_buttons ^ pressed_buttons_switcher) & buttons_mode_tmp) :
    fix_active_buttons;
    
  if (
      (_CHECK_SET_BIT(active_functions, RANG_AVAR_DEFECT) == 0) &&
      (fix_active_buttons_tmp != fix_active_buttons) 
     )   
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
    uint32_t maska_yellow = ((1 << (i*NUMBER_LED_COLOR + LED_COLOR_RED_BIT)) | (1 << (i*NUMBER_LED_COLOR + LED_COLOR_GREEN_BIT)));
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
    do{
    // ----------------    -------------------------
     
     register long i,lV,j;
     register long lIdxBlk;
     
     register void *pvl;
     
        pvl = (void*)&hldGsBlkParam;
        sLV.ch_amount_active_src = i = N_IN_GOOSE;//hldGsBlkParam.ch_amount_active_src;
        sLV.ch_amount_GsSignal = 0;
        if(i > 0){
            ((GsBlkParamDsc*)pvl)->sh_amount_gs_active_src = lV = lIdxBlk = 0;asm ("nop" ::"r"(lIdxBlk),"r"(lV));
            while(lIdxBlk < N_IN_GOOSE){ //do{
                //--i = ((GsBlkParamDsc*)pvl)-> unnV1[lIdxBlk].GsBlkHdr.OrdNumGsBlk;;
                //--//Check State Inputs                                            ;
                //--lV = arrGsBlk[i].arCh[2];//Outs                                 ;
                i = lIdxBlk;
                lV = arrGsBlk[i].arCh[2];//!!Outs arrGsBlk[i].arCh[0]
                
				
                lV &= ~((unsigned long)(Input_ctrl_In_GOOSE_block[i]));//Clear all changeble bits
                lV |= Input_In_GOOSE_block[i]&Input_ctrl_In_GOOSE_block[i];//Set ones only
                //if(noerror)
                    arrGsBlk[i].arCh[2] = lV;//arrGsBlk[i].arCh[0]
                if(arrGsBlk[i].GS_Hld.Blk == 0){   
					register unsigned long wrpLV;
					wrpLV = arrGsBlk[i].arCh[2];//!!arrGsBlk[i].arCh[2] = arrGsBlk[i].arCh[0]
					j = 0;lV = ((GsBlkParamDsc*)pvl)->sh_amount_gs_active_src;
					while( (wrpLV != 0) ){//&& j < 8
						if((wrpLV & (1<<j)) != 0 ){
							arrOrdNumsGsSignal[lV] = (lIdxBlk<<3)+j;lV++;wrpLV &= ~(1<<j);
						}
						j++;
						
					}
					((GsBlkParamDsc*)pvl)->sh_amount_gs_active_src = lV;
				}
                lIdxBlk++;//  += lV;//! ????
            }//while(lIdxBlk);//(--sLV.ch_amount_active_src) != 0
            sLV.ch_amount_GsSignal = ((GsBlkParamDsc*)pvl)->sh_amount_gs_active_src;
        }
    //Try bield list of outs which sets in ones.
    
    }while(sLV.ch_while_breaker);
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
    do{
    // ----------------    -------------------------
     register long i,lV,j;
     register long lIdxBlk;
     register void *pvl;
        pvl = (void*)&hldMmsBlkParam;
        sLV.ch_amount_active_src = i  = hldMmsBlkParam.ch_amount_active_src;
        if(i > 0){
            ((MmsBlkParamDsc*)pvl)->sh_amount_mms_active_src = lV = lIdxBlk = 0;asm ("nop" ::"r"(lIdxBlk),"r"(lV));
            while(lIdxBlk < N_IN_MMS){ //do{
                //--;
                //--;
                //--;
                i = lIdxBlk;//.i = ((MmsBlkParamDsc*)pvl)-> unnV1[lIdxBlk].MmsBlkHdr.OrdNumMmsBlk;
                //Check State Inputs
                lV = arrMmsBlk[i].arCh[2];//Outs
                lV &= ~((unsigned long)(Input_In_MMS_block[i]));//Clear all changeble bits
                lV |= Input_In_MMS_block[i]&Input_ctrl_In_MMS_block[i];//Set ones only
                //if(noerror)
                    arrMmsBlk[i].arCh[2] = lV;//
                if(arrMmsBlk[i].Mms_Hld.Blk == 0 && (((MmsBlkParamDsc*)pvl)->ch_amount_blk_src&0x80)){ 
                    register unsigned long wrpLV;
        
					wrpLV = lV;//arrMmsBlk[i].arCh[2];
					j = 0;lV = ((MmsBlkParamDsc*)pvl)->sh_amount_mms_active_src;
					while( (wrpLV != 0) ){//&& j < 8
						if((wrpLV & (1<<j)) != 0 ){
							arrOrdNumsMmsSignal[lV] = (lIdxBlk<<3)+j;lV++;wrpLV &= ~(1<<j);
						}
						j++;
					}
                }
                ((MmsBlkParamDsc*)pvl)->sh_amount_mms_active_src = lV;
                lIdxBlk++;//  += lV;//! ????
            }//while(lIdxBlk);//(--sLV.ch_amount_active_src) != 0
            sLV.ch_amount_MmsSignal = ((MmsBlkParamDsc*)pvl)->sh_amount_mms_active_src; 
        }    

    //Try bield list of outs which sets in ones.
    
    }while(sLV.ch_while_breaker);
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
  for(size_t m = 0; m < N_SMALL; ++m) 
	{
		command_from_KP[m] = command_permitted_from_KP[m] & activation_function_from_KP[m] & ctrl_activation_function_from_KP[m];
		activation_function_from_KP[m] = 0;
		ctrl_activation_function_from_KP[m] = 0;
		
		if (command_from_KP[m] != 0)  flagCommandKP = true; 
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
      
  int flag=0;
  for(int m=0; m<N_SMALL; m++) if(activation_function_from_interface[m] != 0) {flag=1; break;}
  if (
      (pressed_buttons_united !=0)
      ||  
      (  
       (mutex_interface == false) &&
       (flag) &&
			 (dostup_z_VR) 
      )
      ||  
      (active_inputs_local !=0)
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
     /* якщо є активація виходів від Вх.GOOSE блоків і Вх.MMS блоків*/   
      || ((sLV.ch_amount_MmsSignal+sLV.ch_amount_GsSignal)>0)
			|| (flagCommandKP)	
#endif
     )   
  {
    unsigned int temp_value_for_activated_function_button_interface[N_SMALL];
    unsigned int temp_value_for_activated_function[N_SMALL];
    for(int m=0; m<N_SMALL; m++) 
    {
      temp_value_for_activated_function_button_interface[m]=0;
      temp_value_for_activated_function[m]=0;
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
    do{
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
        proc_Gs_blk_out((void*)&temp_value_for_activated_function,hldGsBlkParam.sh_amount_gs_active_src,arrOrdNumsGsSignal );    
//        }
    }while(sLV.ch_while_breaker);
    memcpy((void *)active_functions_goose_small_tmp,(const void*)temp_value_for_activated_function, ((N_SMALL)<<2) );
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
    do{
        sLV.ch_while_breaker = 0;
    // ----------------    -------------------------       
//        register long i,lV,j,lCtrMmsSrc;
//         register void *pvlc,*pvll;
//        pvlc = (void*)temp_value_for_activated_function;
//        pvll = (void*)&current_settings_prt.ranguvannja_In_MMS;
//        //sLV.ch_amount_active_src = hldGsBlkParam.ch_amount_active_src;
//        //pvl = (void*)&hldGsBlkParam;((GsBlkParamDsc*)pvl)->
//        lCtrMmsSrc = hldMmsBlkParam.sh_amount_mms_active_src;   
        proc_Mms_blk_out((void*)&temp_value_for_activated_function_button_interface,hldMmsBlkParam.sh_amount_mms_active_src,arrOrdNumsMmsSignal );        
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
    }while(sLV.ch_while_breaker);
    long idx_n_small_arr = 0;
    do{
		if(temp_value_for_activated_function_button_interface[idx_n_small_arr] != 0){
		_SET_BIT(sLV.p_active_functions, RANG_LAN_ACTIVE);
		idx_n_small_arr += N_SMALL;//break;
		}
	
	}while(++idx_n_small_arr < N_SMALL);
	

	
	
    
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
          unsigned int * const point_temp_value_for_activated_function = ((pressed_buttons_tmp & (1 << i)) != 0) ? temp_value_for_activated_function_button_interface : temp_value_for_activated_function;
          for (size_t j = 0; j < N_SMALL; ++j ) 
          {
            point_temp_value_for_activated_function[j] |= current_settings_prt.ranguvannja_buttons[N_SMALL*i + j];
            active_functions_fc_small_tmp[j] |= current_settings_prt.ranguvannja_buttons[N_SMALL*i + j];
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
            temp_value_for_activated_function[j] |= current_settings_prt.ranguvannja_inputs[N_SMALL*i + j];
            active_functions_bi_small_tmp[j] |= current_settings_prt.ranguvannja_inputs[N_SMALL*i + j];
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
			  (dostup_z_VR) 
			 )		
    {
      for (size_t i = 0; i < N_SMALL; ++i)
      {
        if (dystancijnyj) temp_value_for_activated_function_button_interface[i] |= activation_function_from_interface[i];
        activation_function_from_interface[i] = 0;
      }

			if (dystancijnyj)
			{
				if (_GET_STATE(dostup_z_VR, USB_RECUEST)  ) _SET_BIT(active_functions, RANG_USB_ACTIVE);
				if (_GET_STATE(dostup_z_VR, RS485_RECUEST)) _SET_BIT(active_functions, RANG_RS485_ACTIVE);
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
				if (_GET_STATE(dostup_z_VR, LAN_RECUEST)  ) _SET_BIT(active_functions, RANG_LAN_ACTIVE);
#endif
			}
			dostup_z_VR = 0;
    }
		
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
    //Активація з КП
		if((flagCommandKP) && (dystancijnyj))
		{
      for (size_t i = 0; i < N_SMALL; ++i) temp_value_for_activated_function_button_interface[i] |= command_from_KP[i];
      _SET_BIT(active_functions, RANG_LAN_ACTIVE);
		}
#endif
    
    //Обєднуємо активації з кнопок (режим кнопка) + активації з інтерфейсу і кнопок (режим ключ) + кнопки фіксованого функціоналу
    for (size_t i = 0; i < N_SMALL; ++i) temp_value_for_activated_function[i] |= temp_value_for_activated_function_button_interface[i];

    //Опреділювані функції
    for (size_t i = 0; i != NUMBER_DEFINED_FUNCTIONS; ++i)
    {
      size_t index_big = RANG_DF1_IN + 3*i;
      size_t index_small = RANG_SMALL_DF1_IN + 2*i;
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

      index_big = RANG_DF1_R +3*i;
      index_small = RANG_SMALL_DF1_R +2*i;
      active_functions[index_big >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, index_small) != 0) << (index_big & 0x1f);
    }

    //Опреділювані триґери
    for (size_t i = 0; i != NUMBER_DEFINED_TRIGGERS; ++i)
    {
      size_t index_big = RANG_DT1_SET + 3*i;
      size_t index_small = RANG_SMALL_DT1_SET + 2*i;
      active_functions[index_big >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, index_small) != 0) << (index_big & 0x1f);
      active_functions[(index_big + 1) >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, (index_small + 1)) != 0) << ((index_big + 1) & 0x1f);

      index_big = RANG_DT1_RESET +3*i;
      index_small = RANG_SMALL_DT1_RESET +2*i;
      active_functions[index_big >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, index_small) != 0) << (index_big & 0x1f);
    }

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
    {
      size_t word_n_small = RANG_SMALL_BLOCK_IN_GOOSE1 >> 5;
      unsigned int maska_small = (unsigned int)(1 << (RANG_SMALL_BLOCK_IN_GOOSE1 & 0x1f));
      
      size_t word_n_full = RANG_BLOCK_IN_GOOSE1 >> 5;
      unsigned int maska_full = (unsigned int)(1 << (RANG_BLOCK_IN_GOOSE1 & 0x1f));

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
      unsigned int index_big = RANG_LF1 +i;
      unsigned int index_small = RANG_SMALL_LF1 +i;
      active_functions[index_big >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, index_small) != 0) << (index_big & 0x1f);
    }
    
    //Загальні функції (без ОФ-ій і функцій, які можуть блокуватися у місцевому управлінні)
    active_functions[RANG_BLOCK_VKL_VV                      >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_BLOCK_VKL_VV                     ) != 0) << (RANG_BLOCK_VKL_VV                      & 0x1f);
    active_functions[RANG_RESET_LEDS                        >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_RESET_LEDS                       ) != 0) << (RANG_RESET_LEDS                        & 0x1f);
    active_functions[RANG_RESET_RELES                       >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_RESET_RELES                      ) != 0) << (RANG_RESET_RELES                       & 0x1f);
    active_functions[RANG_MISCEVE_DYSTANCIJNE               >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_MISCEVE_DYSTANCIJNE              ) != 0) << (RANG_MISCEVE_DYSTANCIJNE               & 0x1f);
    active_functions[RANG_STATE_VV                          >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_STATE_VV                         ) != 0) << (RANG_STATE_VV                          & 0x1f);
    active_functions[RANG_CTRL_VKL                          >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_CTRL_VKL                         ) != 0) << (RANG_CTRL_VKL                          & 0x1f);
    active_functions[RANG_CTRL_OTKL                         >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_CTRL_OTKL                        ) != 0) << (RANG_CTRL_OTKL                         & 0x1f);
    active_functions[RANG_RESET_BLOCK_READY_TU_VID_ZAHYSTIV >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_RESET_BLOCK_READY_TU_VID_ZAHYSTIV) != 0) << (RANG_RESET_BLOCK_READY_TU_VID_ZAHYSTIV & 0x1f);
    active_functions[RANG_OTKL_VID_ZOVN_ZAHYSTIV            >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_OTKL_VID_ZOVN_ZAHYSTIV     ) != 0) << (RANG_OTKL_VID_ZOVN_ZAHYSTIV            & 0x1f);

    active_inputs_grupa_ustavok |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_1_GRUPA_USTAVOK    ) != 0) << (RANG_SMALL_1_GRUPA_USTAVOK - RANG_SMALL_1_GRUPA_USTAVOK);
    active_inputs_grupa_ustavok |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_2_GRUPA_USTAVOK    ) != 0) << (RANG_SMALL_2_GRUPA_USTAVOK - RANG_SMALL_1_GRUPA_USTAVOK);
    active_inputs_grupa_ustavok |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_3_GRUPA_USTAVOK    ) != 0) << (RANG_SMALL_3_GRUPA_USTAVOK - RANG_SMALL_1_GRUPA_USTAVOK);
    active_inputs_grupa_ustavok |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_4_GRUPA_USTAVOK    ) != 0) << (RANG_SMALL_4_GRUPA_USTAVOK - RANG_SMALL_1_GRUPA_USTAVOK);
      
    //Увімкнення ВВ
    active_functions[RANG_VKL_VV >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_VKL_VV) != 0) << (RANG_VKL_VV & 0x1f);

    //Вимкнення ВВ
    active_functions[RANG_OTKL_VV >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_OTKL_VV) != 0) << (RANG_OTKL_VV & 0x1f);

    //МТЗ
    active_functions[RANG_BLOCK_MTZ1     >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_BLOCK_MTZ1    ) != 0) << (RANG_BLOCK_MTZ1     & 0x1f);
    active_functions[RANG_BLOCK_MTZ2     >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_BLOCK_MTZ2    ) != 0) << (RANG_BLOCK_MTZ2     & 0x1f);
    active_functions[RANG_BLOCK_USK_MTZ2 >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_BLOCK_USK_MTZ2) != 0) << (RANG_BLOCK_USK_MTZ2 & 0x1f);
    active_functions[RANG_BLOCK_MTZ3     >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_BLOCK_MTZ3    ) != 0) << (RANG_BLOCK_MTZ3     & 0x1f);
    active_functions[RANG_BLOCK_MTZ4     >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_BLOCK_MTZ4    ) != 0) << (RANG_BLOCK_MTZ4     & 0x1f);

    //Блок для МТЗ 0.4кВ
    active_functions[RANG_BLOCK_MTZ04_1     >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_BLOCK_MTZ04_1    ) != 0) << (RANG_BLOCK_MTZ04_1     & 0x1f);
    active_functions[RANG_BLOCK_MTZ04_2     >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_BLOCK_MTZ04_2    ) != 0) << (RANG_BLOCK_MTZ04_2     & 0x1f);
    active_functions[RANG_BLOCK_USK_MTZ04_2 >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_BLOCK_USK_MTZ04_2) != 0) << (RANG_BLOCK_USK_MTZ04_2 & 0x1f);

    //ЗНам
    active_functions[RANG_BLOCK_ZNAM >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_BLOCK_ZNAM) != 0) << (RANG_BLOCK_ZNAM & 0x1f);

		//Блок для ЗДЗ
    active_functions[RANG_BLOCK_ZDZ       >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_BLOCK_ZDZ      ) != 0) << (RANG_BLOCK_ZDZ       & 0x1f);
    active_functions[RANG_PUSK_ZDZ_VID_DV >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_PUSK_ZDZ_VID_DV) != 0) << (RANG_PUSK_ZDZ_VID_DV & 0x1f);

    //СЗЗ
    active_functions[RANG_BLOCK_NZZ >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_BLOCK_NZZ) != 0) << (RANG_BLOCK_NZZ & 0x1f);

    //Блок ТЗНП
    active_functions[RANG_BLOCK_TZNP1 >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_BLOCK_TZNP1) != 0) << (RANG_BLOCK_TZNP1 & 0x1f);
    active_functions[RANG_BLOCK_TZNP2 >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_BLOCK_TZNP2) != 0) << (RANG_BLOCK_TZNP2 & 0x1f);
    active_functions[RANG_BLOCK_TZNP3 >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_BLOCK_TZNP3) != 0) << (RANG_BLOCK_TZNP3 & 0x1f);

    //АПВ
    active_functions[RANG_STAT_BLK_APV >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_STAT_BLK_APV) != 0) << (RANG_STAT_BLK_APV & 0x1f);
      
    //ЧАПВ
    active_functions[RANG_ACHR_CHAPV_VID_DV  >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_ACHR_CHAPV_VID_DV ) != 0) << (RANG_ACHR_CHAPV_VID_DV & 0x1f);
    active_functions[RANG_BLOCK_ACHR1        >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_BLOCK_ACHR1      ) != 0) << (RANG_BLOCK_ACHR1       & 0x1f);
    active_functions[RANG_BLOCK_ACHR2        >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_BLOCK_ACHR2      ) != 0) << (RANG_BLOCK_ACHR2       & 0x1f);

    //УРОВ
    active_functions[RANG_BLOCK_UROV >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_BLOCK_UROV) != 0) << (RANG_BLOCK_UROV & 0x1f);
    active_functions[RANG_PUSK_UROV_VID_DV >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_PUSK_UROV_VID_DV) != 0) << (RANG_PUSK_UROV_VID_DV & 0x1f);

    //Блок ЗОП(КОФ)
    active_functions[RANG_BLOCK_ZOP >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_BLOCK_ZOP) != 0) << (RANG_BLOCK_ZOP & 0x1f);

    //Блок для Umin
    active_functions[RANG_BLOCK_UMIN1 >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_BLOCK_UMIN1) != 0) << (RANG_BLOCK_UMIN1 & 0x1f);
    active_functions[RANG_START_UMIN1 >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_START_UMIN1) != 0) << (RANG_START_UMIN1 & 0x1f);
    active_functions[RANG_BLOCK_UMIN2 >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_BLOCK_UMIN2) != 0) << (RANG_BLOCK_UMIN2 & 0x1f);
    active_functions[RANG_START_UMIN2 >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_START_UMIN2) != 0) << (RANG_START_UMIN2 & 0x1f);
      
    //Блок для Umax
    active_functions[RANG_BLOCK_UMAX1 >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_BLOCK_UMAX1) != 0) << (RANG_BLOCK_UMAX1 & 0x1f);
    active_functions[RANG_BLOCK_UMAX2 >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_BLOCK_UMAX2) != 0) << (RANG_BLOCK_UMAX2 & 0x1f);

    //Блокування для УЗ
    for (size_t i = 0; i < NUMBER_UP; i++)
    {
      uint32_t rang_small_block_up = RANG_SMALL_BLOCK_UP1 + i;
      uint32_t rang_block_up = RANG_BLOCK_UP1 + 3*i;
      active_functions[rang_block_up >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, rang_small_block_up) != 0) << (rang_block_up & 0x1f);
    }
  }
  /**************************/
  
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
  
#define N_SOURCE  3
  
#else
  
#define N_SOURCE  2
  
#endif

  for (size_t i = 0; i < N_SOURCE; ++i)
  {
    static unsigned int const command_signales[N_SMALL] = 
    {
      COMMAND_SIGNALES_0, 
      COMMAND_SIGNALES_1, 
      COMMAND_SIGNALES_2 
    };
  
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
    unsigned int const * const ref[N_SOURCE] = 
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
    for (unsigned int *p = temp_activating_functions_small; p < (temp_activating_functions_small + N_SMALL); ++p) *p = 0;
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
    for(int i=0; i<N_BIG; i++) temp_maska_filter_function[i]=0;
    unsigned int temp_activating_functions[N_BIG];
    for(int i=0; i<N_BIG; i++) temp_activating_functions[i]=0;
  
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
      if (_CHECK_SET_BIT ((current_settings_prt.ranguvannja_inputs + N_SMALL*i), RANG_SMALL_STATE_VV) != 0)
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
        if (_CHECK_SET_BIT (current_settings_prt.ranguvannja_In_GOOSE[i][j], RANG_SMALL_STATE_VV) != 0)
        {
          state_vv_dv = true;
          break;
        }
      }
    }
#endif
    
    if (state_vv_dv)
    {
      if (_CHECK_SET_BIT(active_functions, RANG_STATE_VV) !=0) 
      {
        state_leds_ctrl &=  (uint32_t)(~((1 << LED_COLOR_GREEN_BIT) << ((uint32_t)NUMBER_LED_COLOR*(uint32_t)LED_CTRL_O)));
        state_leds_ctrl |=  (uint32_t)(  (1 << LED_COLOR_RED_BIT  ) << ((uint32_t)NUMBER_LED_COLOR*(uint32_t)LED_CTRL_I) );
      }
      else
      {
        state_leds_ctrl &=  (uint32_t)(~((1 << LED_COLOR_RED_BIT  ) << ((uint32_t)NUMBER_LED_COLOR*(uint32_t)LED_CTRL_I)));
        state_leds_ctrl |=  (uint32_t)(  (1 << LED_COLOR_GREEN_BIT) << ((uint32_t)NUMBER_LED_COLOR*(uint32_t)LED_CTRL_O) );
      }
    }
    else
    {
      state_leds_ctrl &=  (uint32_t)(~(((1 << LED_COLOR_GREEN_BIT) << ((uint32_t)NUMBER_LED_COLOR*(uint32_t)LED_CTRL_O)) | ((1 << LED_COLOR_RED_BIT) << ((uint32_t)NUMBER_LED_COLOR*(uint32_t)LED_CTRL_I))));
    }
  }
  /**************************/

  /**************************/
  /*Режим перепрограмування*/
  /**************************/
//#ifndef DEBUG_TEST  
  if ((GPIO_STAFF_REPROGRAM->IDR & GPIO_PIN_STAFF_REPROGRAM) != (uint32_t)Bit_RESET)
//#else
//  if (reprogram)
//#endif
  {
    _SET_BIT(set_diagnostyka, WARNING_REPROGRAM);
    timerWaitReprogram = -1;
  }
  else _SET_BIT(clear_diagnostyka, WARNING_REPROGRAM);
  /**************************/

//  //Діагностика справності раз на період
//  diagnostyca_adc_execution();
  
  /**************************/
  //Сигнал "Несправность Общая"
  /**************************/
  if (
      (_CHECK_SET_BIT(active_functions, RANG_RESET_LEDS ) != 0) ||
      (_CHECK_SET_BIT(active_functions, RANG_RESET_RELES) != 0)
     )
  {
    _SET_BIT(clear_diagnostyka, ERROR_DR_LOSS_INFORMATION_BIT);
    _SET_BIT(clear_diagnostyka, ERROR_PR_ERR_LOSS_INFORMATION_BIT);
  }
        
  unsigned int diagnostyka_tmp[N_DIAGN];
  for (size_t i = 0; i < N_DIAGN; i ++)
  {
    diagnostyka_tmp[i] = diagnostyka[i];

    diagnostyka_tmp[i] &= (unsigned int)(~clear_diagnostyka[i]); 
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
    if (not_null) break;
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
      not_null |= ((diagnostyka_tmp[i] & maska_avar_error[i])  != 0);
      if (not_null) break;
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
      if (i == (NUMBER_GROUP_USTAVOK - 1)) _SET_BIT(active_functions, RANG_INVERS_DV_GRUPA_USTAVOK);
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
  	  if (not_null) break;
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
    MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_6,
    MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_7,
    MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_8
      
#ifdef MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_9
                                          ,
    MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_9
#endif
  };
  unsigned int comp = false;
  COMPARE_NOT_ZERO_OR(comp, active_functions, maska_signals_for_lock_group, N_BIG)
  if (comp)
  {
    //Іде блокування груп уставок - група уставок залишається тою, яка вибрана попередньо
    _SET_BIT(active_functions, RANG_BLK_GRUP_USTAVOK_VID_ZACHYSTIV);

    unsigned int number_group_stp_tmp = 0;
    if(( _CHECK_SET_BIT(active_functions, RANG_1_GRUPA_USTAVOK) ) !=0 ) number_group_stp_tmp++;
    if(( _CHECK_SET_BIT(active_functions, RANG_2_GRUPA_USTAVOK) ) !=0 ) number_group_stp_tmp++;
    if(( _CHECK_SET_BIT(active_functions, RANG_3_GRUPA_USTAVOK) ) !=0 ) number_group_stp_tmp++;
    if(( _CHECK_SET_BIT(active_functions, RANG_4_GRUPA_USTAVOK) ) !=0 ) number_group_stp_tmp++;
    
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
  if     (( _CHECK_SET_BIT(active_functions, RANG_1_GRUPA_USTAVOK) ) !=0 ) number_group_stp = 0;
  else if(( _CHECK_SET_BIT(active_functions, RANG_2_GRUPA_USTAVOK) ) !=0 ) number_group_stp = 1;
  else if(( _CHECK_SET_BIT(active_functions, RANG_3_GRUPA_USTAVOK) ) !=0 ) number_group_stp = 2;
  else if(( _CHECK_SET_BIT(active_functions, RANG_4_GRUPA_USTAVOK) ) !=0 ) number_group_stp = 3;
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
  if(semaphore_measure_values_low1 == 0)
  {
    for (unsigned int i = 0; i < _NUMBER_IM; i++) 
    {
      measurement_high[bank_measurement_high_tmp][i] = measurement_middle[i] = measurement[i];
    }
    frequency_middle = frequency;
    for (unsigned int i = 0; i < MAX_NUMBER_INDEXES_RESISTANCE; i++) resistance_middle[i] = resistance[i];
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
    //Контроль привода ВВ
    /**************************/
    control_VV(active_functions);
    /**************************/

    /**************************/
    //ЗНам
    /**************************/
    if ((current_settings_prt.configuration & (1 << ZNAM_BIT_CONFIGURATION)) != 0)
    {
      znam_handler(active_functions, number_group_stp);
    }
    else
    {
      //Очищуємо сигнали, які не можуть бути у даній конфігурації
      static const unsigned int maska_znam_signals[N_BIG] = 
      {
        MASKA_ZNAM_SIGNALS_0,
        MASKA_ZNAM_SIGNALS_1,
        MASKA_ZNAM_SIGNALS_2, 
        MASKA_ZNAM_SIGNALS_3, 
        MASKA_ZNAM_SIGNALS_4, 
        MASKA_ZNAM_SIGNALS_5,
        MASKA_ZNAM_SIGNALS_6, 
        MASKA_ZNAM_SIGNALS_7,
				MASKA_ZNAM_SIGNALS_8
          
#ifdef MASKA_ZNAM_SIGNALS_9
                           ,
        MASKA_ZNAM_SIGNALS_9
#endif
      };
      for (size_t i = 0; i < N_BIG; ++i) active_functions[i] &= (unsigned int)(~maska_znam_signals[i]);
      for (int *p = (global_timers + _INDEX_ZNAM_BEGIN); p <= (global_timers + _INDEX_ZNAM_END); ++p) *p = -1;
			
			for (size_t i = 0; i < NUMBER_I2G_CANALES; ++i)
			{
				i1g_bilshe_porogu[0] = 0;
				i2g_bilshe_porogu[0] = 0;
			}
    }
    /**************************/

    /**************************/
    //МCЗ
    /**************************/
    if ((current_settings_prt.configuration & (1 << MTZ_BIT_CONFIGURATION)) != 0)
    {
      ocp_handler(active_functions, number_group_stp);
    }
    else
    {
      //Очищуємо сигнали, які не можуть бути у даній конфігурації
      static const unsigned int maska_mtz_signals[N_BIG] = 
      {
        MASKA_MTZ_SIGNALS_0,
        MASKA_MTZ_SIGNALS_1,
        MASKA_MTZ_SIGNALS_2, 
        MASKA_MTZ_SIGNALS_3, 
        MASKA_MTZ_SIGNALS_4, 
        MASKA_MTZ_SIGNALS_5,
        MASKA_MTZ_SIGNALS_6, 
        MASKA_MTZ_SIGNALS_7,
				MASKA_MTZ_SIGNALS_8
          
#ifdef MASKA_MTZ_SIGNALS_9
                           ,
        MASKA_MTZ_SIGNALS_9
#endif
      };
      for (size_t i = 0; i < N_BIG; ++i) active_functions[i] &= (unsigned int)(~maska_mtz_signals[i]);
      for (int *p = (global_timers + _INDEX_MTZ_BEGIN); p <= (global_timers + _INDEX_MTZ_END); ++p) *p = -1;
			
			ocp_general_bits = 0;
			static_ocp_select_tmp_bits = 0;
			static_ocp_tmp_bits = 0;
			static_ocp_fw_tmp_bits = 0;
			static_ocp_bw_tmp_bits = 0;
			static_ocp_by_U_tmp_bits = 0;
//			static_ocp_dep_tmp_bits = 0;
			static_ocp_dep_rez_bits = 0;
    }
    /**************************/
    
    /**************************/
    //МСЗ04
    /**************************/
    if ((current_settings_prt.configuration & (1 << MTZ04_BIT_CONFIGURATION)) != 0)
    {
      ocp04_handler(active_functions, number_group_stp);
    }
    else
    {
      //Очищуємо сигнали, які не можуть бути у даній конфігурації
      static const unsigned int maska_mtz04_signals[N_BIG] = 
      {
        MASKA_MTZ04_SIGNALS_0, 
        MASKA_MTZ04_SIGNALS_1, 
        MASKA_MTZ04_SIGNALS_2,
        MASKA_MTZ04_SIGNALS_3, 
        MASKA_MTZ04_SIGNALS_4, 
        MASKA_MTZ04_SIGNALS_5, 
        MASKA_MTZ04_SIGNALS_6, 
        MASKA_MTZ04_SIGNALS_7,
				MASKA_MTZ04_SIGNALS_8
          
#ifdef MASKA_MTZ04_SIGNALS_9
                             ,
        MASKA_MTZ04_SIGNALS_9
#endif
      };
      for (size_t i = 0; i < N_BIG; ++i) active_functions[i] &= (unsigned int)(~maska_mtz04_signals[i]);
      for (int *p = (global_timers + _INDEX_MTZ04_BEGIN); p <= (global_timers + _INDEX_MTZ04_END); ++p) *p = -1;
			
			ocp04_general_bits = 0;
			static_ocp04_tmp_bits = 0;
//			static_ocp04_dep_tmp_bits = 0;
			static_ocp04_dep_rez_bits = 0;
    }
    /**************************/

    /**************************/
    //ЗЗ
    /**************************/
    if ((current_settings_prt.configuration & (1 << ZZ_BIT_CONFIGURATION)) != 0)
    {
      zz_handler(active_functions, number_group_stp);
    }
    else
    {
      //Очищуємо сигнали, які не можуть бути у даній конфігурації
      static const unsigned int maska_nzz_signals[N_BIG] = 
      {
        MASKA_NZZ_SIGNALS_0, 
        MASKA_NZZ_SIGNALS_1, 
        MASKA_NZZ_SIGNALS_2,
        MASKA_NZZ_SIGNALS_3, 
        MASKA_NZZ_SIGNALS_4, 
        MASKA_NZZ_SIGNALS_5, 
        MASKA_NZZ_SIGNALS_6, 
        MASKA_NZZ_SIGNALS_7,
				MASKA_NZZ_SIGNALS_8
          
#ifdef MASKA_NZZ_SIGNALS_9
                           ,
        MASKA_NZZ_SIGNALS_9
#endif
      };
      for (size_t i = 0; i < N_BIG; ++i) active_functions[i] &= (unsigned int)(~maska_nzz_signals[i]);
      for (int *p = (global_timers + _INDEX_NZZ_BEGIN); p <= (global_timers + _INDEX_NZZ_END); ++p) *p = -1;
    }
    /**************************/

    /**************************/
    //ТЗНП
    /**************************/
    if ((current_settings_prt.configuration & (1 << TZNP_BIT_CONFIGURATION)) != 0)
    {
      tznp_handler(active_functions, number_group_stp);
    }
    else
    {
      //Очищуємо сигнали, які не можуть бути у даній конфігурації
      static const unsigned int maska_tznp_signals[N_BIG] = 
      {
        MASKA_TZNP_SIGNALS_0, 
        MASKA_TZNP_SIGNALS_1, 
        MASKA_TZNP_SIGNALS_2,
        MASKA_TZNP_SIGNALS_3, 
        MASKA_TZNP_SIGNALS_4, 
        MASKA_TZNP_SIGNALS_5, 
        MASKA_TZNP_SIGNALS_6, 
        MASKA_TZNP_SIGNALS_7,
				MASKA_TZNP_SIGNALS_8
          
#ifdef MASKA_TZNP_SIGNALS_9
                            ,
        MASKA_TZNP_SIGNALS_9
#endif
      };
      for (size_t i = 0; i < N_BIG; ++i) active_functions[i] &= (unsigned int)(~maska_tznp_signals[i]);
      for (int *p = (global_timers + _INDEX_SZNP_BEGIN); p <= (global_timers + _INDEX_SZNP_END); ++p) *p = -1;
    }
    /**************************/
  
    /**************************/
    //ЗОП(КОФ)
    /**************************/
    if ((current_settings_prt.configuration & (1 << ZOP_BIT_CONFIGURATION)) != 0)
    {
      zop_handler(active_functions, number_group_stp);
    }
    else
    {
      //Очищуємо сигнали, які не можуть бути у даній конфігурації
      static const unsigned int maska_zop_signals[N_BIG] = 
      {
        MASKA_ZOP_SIGNALS_0, 
        MASKA_ZOP_SIGNALS_1, 
        MASKA_ZOP_SIGNALS_2,
        MASKA_ZOP_SIGNALS_3, 
        MASKA_ZOP_SIGNALS_4, 
        MASKA_ZOP_SIGNALS_5, 
        MASKA_ZOP_SIGNALS_6, 
        MASKA_ZOP_SIGNALS_7,
				MASKA_ZOP_SIGNALS_8
          
#ifdef MASKA_ZOP_SIGNALS_9
                           ,
        MASKA_ZOP_SIGNALS_9
#endif
      };
      for (size_t i = 0; i < N_BIG; ++i) active_functions[i] &= (unsigned int)(~maska_zop_signals[i]);
      for (int *p = (global_timers + _INDEX_ZZP_BEGIN); p <= (global_timers + _INDEX_ZZP_END); ++p) *p = -1;
    }
    /**************************/
    
    if ((current_settings_prt.configuration & (1 << UMIN_BIT_CONFIGURATION)) != 0) 
    {
      /**************************/
      //ЗНМИН1
      /**************************/
      umin1_handler(active_functions, number_group_stp);
      /**************************/
      
      /**************************/
      //ЗНМИН2
      /**************************/
      umin2_handler(active_functions, number_group_stp);
      /**************************/
    } 
    else 
    {
      //Очищуємо сигнали, які не можуть бути у даній конфігурації
      static const unsigned int maska_umin_signals[N_BIG] = 
      {
        MASKA_UMIN_SIGNALS_0, 
        MASKA_UMIN_SIGNALS_1, 
        MASKA_UMIN_SIGNALS_2,
        MASKA_UMIN_SIGNALS_3, 
        MASKA_UMIN_SIGNALS_4, 
        MASKA_UMIN_SIGNALS_5, 
        MASKA_UMIN_SIGNALS_6, 
        MASKA_UMIN_SIGNALS_7,
				MASKA_UMIN_SIGNALS_8
          
#ifdef MASKA_UMIN_SIGNALS_9
                            ,
        MASKA_UMIN_SIGNALS_9
#endif
      };
      for (unsigned int i = 0; i < N_BIG; i++) active_functions[i] &= (unsigned int)(~maska_umin_signals[i]);
      for (int *p = (global_timers + _INDEX_UMIN_BEGIN); p <= (global_timers + _INDEX_UMIN_END); ++p) *p = -1;
    }
    
    if ((current_settings_prt.configuration & (1 << UMAX_BIT_CONFIGURATION)) != 0) 
    {
      /**************************/
      //ЗНМАКС1
      /**************************/
      umax1_handler(active_functions, number_group_stp);
      /**************************/
      
      /**************************/
      //ЗНМАКС2
      /**************************/
      umax2_handler(active_functions, number_group_stp);
      /**************************/
    } 
    else 
    {
      //Очищуємо сигнали, які не можуть бути у даній конфігурації
      static const unsigned int maska_umax_signals[N_BIG] = 
      {
        MASKA_UMAX_SIGNALS_0, 
        MASKA_UMAX_SIGNALS_1, 
        MASKA_UMAX_SIGNALS_2,
        MASKA_UMAX_SIGNALS_3, 
        MASKA_UMAX_SIGNALS_4, 
        MASKA_UMAX_SIGNALS_5, 
        MASKA_UMAX_SIGNALS_6, 
        MASKA_UMAX_SIGNALS_7,
				MASKA_UMAX_SIGNALS_8
          
#ifdef MASKA_UMAX_SIGNALS_9
                            ,
        MASKA_UMAX_SIGNALS_9
#endif
      };
      for (size_t i = 0; i < N_BIG; ++i) active_functions[i] &= (unsigned int)(~maska_umax_signals[i]);
      for (int *p = (global_timers + _INDEX_UMAX_BEGIN); p <= (global_timers + _INDEX_UMAX_END); ++p) *p = -1;
    }
    
    if ((current_settings_prt.configuration & (1 << ACHR_CHAPV_BIT_CONFIGURATION)) != 0)
    {
      /**************************/
      //АЧР ЧАПВ
      /**************************/
      achr_chapv_handler(active_functions, number_group_stp);
      /**************************/
    } 
    else 
    {
      //Очищуємо сигнали, які не можуть бути у даній конфігурації
      static const unsigned int maska_achr_chapv_signals[N_BIG] = 
      {
        MASKA_ACHR_CHAPV_SIGNALS_0, 
        MASKA_ACHR_CHAPV_SIGNALS_1, 
        MASKA_ACHR_CHAPV_SIGNALS_2,
        MASKA_ACHR_CHAPV_SIGNALS_3, 
        MASKA_ACHR_CHAPV_SIGNALS_4, 
        MASKA_ACHR_CHAPV_SIGNALS_5, 
        MASKA_ACHR_CHAPV_SIGNALS_6, 
        MASKA_ACHR_CHAPV_SIGNALS_7,
				MASKA_ACHR_CHAPV_SIGNALS_8
          
#ifdef MASKA_ACHR_CHAPV_SIGNALS_9
                                  ,
        MASKA_ACHR_CHAPV_SIGNALS_9
#endif
      };
      for (size_t i = 0; i < N_BIG; ++i) active_functions[i] &= (unsigned int)(~maska_achr_chapv_signals[i]);
      for (int *p = (global_timers + _INDEX_ACHR_CHAPV_BEGIN); p <= (global_timers + _INDEX_ACHR_CHAPV_END); ++p) *p = -1;


      previous_state_po_achr_chapv_uaf1 = 0;
      previous_state_po_achr_chapv_ubf1 = 0;
      previous_state_po_achr_chapv_ucf1 = 0;
      previous_states_CHAPV1 = 0;
      trigger_CHAPV1 = 0;
      previous_states_CHAPV2= 0;
      trigger_CHAPV2= 0;
    }
    
    /**************************/
    //ЗДЗ
    /**************************/
    if ((current_settings_prt.configuration & (1 << ZDZ_BIT_CONFIGURATION)) != 0)
    {
      zdz_handler(active_functions, number_group_stp);
    }
    else
    {
#if   (                                 \
       (MODYFIKACIA_VERSII_PZ == 0) ||  \
       (MODYFIKACIA_VERSII_PZ == 3) ||  \
       (MODYFIKACIA_VERSII_PZ == 4) ||  \
       (MODYFIKACIA_VERSII_PZ == 6) ||  \
       (MODYFIKACIA_VERSII_PZ == 7) ||  \
       (MODYFIKACIA_VERSII_PZ == 10)||  \
       (MODYFIKACIA_VERSII_PZ == 13)||  \
       (MODYFIKACIA_VERSII_PZ == 14)||  \
       (MODYFIKACIA_VERSII_PZ == 17)||  \
       (MODYFIKACIA_VERSII_PZ == 17)    \
      )   
      //Вимикаємо можливий режим тестування оптоканалу
       test_OVD = 0;
       zdz_ovd_porig = current_settings_prt.zdz_ovd_porig + 1;
      if (zdz_ovd_diagnostyka)
      {
        if (zdz_ovd_diagnostyka & (1u << 0)) _SET_BIT(clear_diagnostyka, TEST_OVD1);
        if (zdz_ovd_diagnostyka & (1u << 1)) _SET_BIT(clear_diagnostyka, TEST_OVD2);
        if (zdz_ovd_diagnostyka & (1u << 2)) _SET_BIT(clear_diagnostyka, TEST_OVD3);

#if (                                   \
       (MODYFIKACIA_VERSII_PZ != 7)&&   \
       (MODYFIKACIA_VERSII_PZ != 17)    \
    )
        if (zdz_ovd_diagnostyka & (1u << 3)) _SET_BIT(clear_diagnostyka, TEST_OVD4);
#endif        

        zdz_ovd_diagnostyka = 0;
      }
      delta_time_test = PERIOD_ZDZ_TEST;
#endif
      
      //Очищуємо сигнали, які не можуть бути у даній конфігурації
      static const unsigned int maska_zdz_signals[N_BIG] = 
      {
        MASKA_ZDZ_SIGNALS_0, 
        MASKA_ZDZ_SIGNALS_1, 
        MASKA_ZDZ_SIGNALS_2,
        MASKA_ZDZ_SIGNALS_3, 
        MASKA_ZDZ_SIGNALS_4, 
        MASKA_ZDZ_SIGNALS_5, 
        MASKA_ZDZ_SIGNALS_6, 
        MASKA_ZDZ_SIGNALS_7,
				MASKA_ZDZ_SIGNALS_8
          
#ifdef MASKA_ZDZ_SIGNALS_9
                           ,
        MASKA_ZDZ_SIGNALS_9
#endif
      };
      for (size_t i = 0; i < N_BIG; ++i) active_functions[i] &= (unsigned int)(~maska_zdz_signals[i]);
      for (int *p = (global_timers + _INDEX_ZDZ_BEGIN); p <= (global_timers + _INDEX_ZDZ_END); ++p) *p = -1;
    }
    /**************************/

    /**************************/
    //УРОВ
    /**************************/
    if ((current_settings_prt.configuration & (1 << UROV_BIT_CONFIGURATION)) != 0)
    {
      urov_handler(active_functions, number_group_stp);
    }
    else
    {
      //Очищуємо сигнали, які не можуть бути у даній конфігурації
      static const unsigned int maska_urov_signals[N_BIG] = 
      {
        MASKA_UROV_SIGNALS_0, 
        MASKA_UROV_SIGNALS_1, 
        MASKA_UROV_SIGNALS_2,
        MASKA_UROV_SIGNALS_3, 
        MASKA_UROV_SIGNALS_4, 
        MASKA_UROV_SIGNALS_5, 
        MASKA_UROV_SIGNALS_6, 
        MASKA_UROV_SIGNALS_7,
				MASKA_UROV_SIGNALS_8
          
#ifdef MASKA_UROV_SIGNALS_9
                            ,
        MASKA_UROV_SIGNALS_9
#endif
      };
      for (size_t i = 0; i < N_BIG; ++i) active_functions[i] &= (unsigned int)(~maska_urov_signals[i]);
      for (int *p = (global_timers + _INDEX_PRVV_BEGIN); p <= (global_timers + _INDEX_PRVV_END); ++p) *p = -1;
    }
    /**************************/

    /**************************/
    //АПВ
    /**************************/
    if ((current_settings_prt.configuration & (1 << APV_BIT_CONFIGURATION)) != 0)
    {
      apv_handler(active_functions, number_group_stp);
    }
    else
    {
      //Очищуємо сигнали, які не можуть бути у даній конфігурації
      static const unsigned int maska_apv_signals[N_BIG] = 
      {
        MASKA_APV_SIGNALS_0, 
        MASKA_APV_SIGNALS_1, 
        MASKA_APV_SIGNALS_2,
        MASKA_APV_SIGNALS_3, 
        MASKA_APV_SIGNALS_4, 
        MASKA_APV_SIGNALS_5, 
        MASKA_APV_SIGNALS_6, 
        MASKA_APV_SIGNALS_7,
				MASKA_APV_SIGNALS_8
          
#ifdef MASKA_APV_SIGNALS_9
                           ,
        MASKA_APV_SIGNALS_9
#endif
      };
      for (size_t i = 0; i < N_BIG; ++i) active_functions[i] &= (unsigned int)(~maska_apv_signals[i]);
      for (int *p = (global_timers + _INDEX_APV_BEGIN); p <= (global_timers + _INDEX_APV_END); ++p) *p = -1;

      static_logic_APV_0 = 0;
      previous_states_APV_0 = 0;
      trigger_APV_0 = 0;
    }
    /**************************/

    if ((current_settings_prt.configuration & (1 << UP_BIT_CONFIGURATION)) != 0) 
    {
      up_handler(active_functions, number_group_stp);
    } 
    else 
    {
      //Очищуємо сигнали, які не можуть бути у даній конфігурації
      static const unsigned int maska_up_signals[N_BIG] = 
      {
        MASKA_UP_SIGNALS_0, 
        MASKA_UP_SIGNALS_1, 
        MASKA_UP_SIGNALS_2,
        MASKA_UP_SIGNALS_3, 
        MASKA_UP_SIGNALS_4, 
        MASKA_UP_SIGNALS_5, 
        MASKA_UP_SIGNALS_6, 
        MASKA_UP_SIGNALS_7,
				MASKA_UP_SIGNALS_8
          
#ifdef MASKA_UP_SIGNALS_9
                          ,
        MASKA_UP_SIGNALS_9
#endif
      };
      for (size_t i = 0; i < N_BIG; ++i) active_functions[i] &= (unsigned int)(~maska_up_signals[i]);
      for (int *p = (global_timers + _INDEX_UP_BEGIN); p <= (global_timers + _INDEX_UP_END); ++p) *p = -1;
    }
    
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
          if (comp) break;
          
          //Пошук, чи вже був поточний стан зафіксований у попередніх ітераціях
          for (int i = (iteration - 2); ((repeat_state == false) && (i >= 0)); --i)
          {
            COMPARE_AND(repeat_state, active_functions_tmp[i], active_functions, N_BIG)
          }            
        }
        if (repeat_state) break;
        
        for (size_t i = 0; i != N_BIG; ++i) active_functions_tmp[iteration][i] = active_functions[i];

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
          (iteration >= current_settings_prt.number_iteration_el)
         )
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
        MASKA_EL_SIGNALS_6, 
        MASKA_EL_SIGNALS_7,
        MASKA_EL_SIGNALS_8
          
#ifdef MASKA_EL_SIGNALS_9                          
                          ,
        MASKA_EL_SIGNALS_9
#endif
      };
      for (size_t i = 0; i < N_BIG; ++i) active_functions[i] &= (unsigned int)(~maska_el_signals[i]);
      
      //Скидаємо всі таймери, які відповідають за розширену логіку
      for(int *p = (global_timers + INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START); p <= (global_timers + INDEX_TIMER_DF_WORK_START + NUMBER_DEFINED_FUNCTIONS - 1); ++p) *p = -1;
      
      static_logic_df = 0;
    }
    /**************************/

    /**************************/
    //Включення-Відключення
    /**************************/
    on_off_handler(active_functions);
    /**************************/
    
    /**************************/
    //ВМП (завжди пісяля логіки Блоку вимикання)
    /**************************/
    vmp_handler(active_functions);
    /**************************/

    /**************************/
    //Готовность к ТУ
    /**************************/
    ready_tu(active_functions);
    /**************************/

    /**************************/
    //Ресурс вимикача (завжди пісяля логіки Блоку вимикання)
    /**************************/
    resurs_vymykacha_handler(active_functions);
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
      MASKA_INFO_SIGNALES_6,
      MASKA_INFO_SIGNALES_7,
			MASKA_INFO_SIGNALES_8
        
#ifdef MASKA_INFO_SIGNALES_9
                           ,
      MASKA_INFO_SIGNALES_9
#endif
    };
    
    for (size_t i = 0; i != N_BIG; ++i) active_functions[i] &= maska_info_signals[i];

    //Скидаємо всі таймери, які присутні у лозіці
    for (int *p = (global_timers + _INDEX_TIMER_LOGIC_BEGIN); p != (global_timers + _MAX_NUMBER_GLOBAL_TIMERS_WITHOUT_REGS); ++p) *p = -1;
    
    //Деактивовуємо всі реле
    state_outputs = 0;
#ifdef NUMBER_DS
    ds = 0;
#endif
    
    //Переводимо у початковий стан деякі глобальні змінні
			
		ocp_general_bits = 0;
		static_ocp_select_tmp_bits = 0;
		static_ocp_tmp_bits = 0;
		static_ocp_fw_tmp_bits = 0;
		static_ocp_bw_tmp_bits = 0;
		static_ocp_by_U_tmp_bits = 0;
//		static_ocp_dep_tmp_bits = 0;
		static_ocp_dep_rez_bits = 0;
			
		ocp04_general_bits = 0;
		static_ocp04_tmp_bits = 0;
//		static_ocp04_dep_tmp_bits = 0;
		static_ocp04_dep_rez_bits = 0;
			
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
do{
    sLV.ch_while_breaker = 0;//! optimize then
    
    register void* pvl;
    
     register unsigned int *r_p_active_functions;
// ----------------    -------------------------
    r_p_active_functions = sLV.p_active_functions;
    pvl = (void*)&hldLanBlkParam;//hldMmsBlkParam;
    if(_CHECK_SET_BIT(((unsigned int*)r_p_active_functions), (RANG_BLOCK_OUT_LAN1 + ((unsigned int)LAN_BLOCK_ORD_NUM_00))) != 0){
    //clr block
        Output_Out_LAN_block[LAN_BLOCK_ORD_NUM_00] = 0;
        //
    }else{
        proc_Lan_blk_out(&current_settings_prt.ranguvannja_Out_LAN[0][0][0],r_p_active_functions,(void*)&(((GsBlkParamDsc*)pvl)-> unnV1[LAN_BLOCK_ORD_NUM_00].ulV));
    }

    if(_CHECK_SET_BIT(((unsigned int*)r_p_active_functions), (RANG_BLOCK_OUT_LAN1+((unsigned int)LAN_BLOCK_ORD_NUM_01))) != 0){
    //clr block
        Output_Out_LAN_block[LAN_BLOCK_ORD_NUM_01] = 0; 
    }else{
        proc_Lan_blk_out(&current_settings_prt.ranguvannja_Out_LAN[0][0][0],r_p_active_functions,(void*)&(((GsBlkParamDsc*)pvl)-> unnV1[LAN_BLOCK_ORD_NUM_01].ulV));
    }

    if(_CHECK_SET_BIT(((unsigned int*)r_p_active_functions), (RANG_BLOCK_OUT_LAN1+((unsigned int)LAN_BLOCK_ORD_NUM_02))) != 0){
    //clr block
        Output_Out_LAN_block[LAN_BLOCK_ORD_NUM_02] = 0;
    }else{
        proc_Lan_blk_out(&current_settings_prt.ranguvannja_Out_LAN[0][0][0],r_p_active_functions,(void*)&(((GsBlkParamDsc*)pvl)-> unnV1[LAN_BLOCK_ORD_NUM_02].ulV));
    }

    if(_CHECK_SET_BIT(((unsigned int*)r_p_active_functions), (RANG_BLOCK_OUT_LAN1+((unsigned int)LAN_BLOCK_ORD_NUM_03))) != 0){
    //clr block
        Output_Out_LAN_block[LAN_BLOCK_ORD_NUM_03] = 0;
    }else{
        proc_Lan_blk_out(&current_settings_prt.ranguvannja_Out_LAN[0][0][0],r_p_active_functions,(void*)&(((GsBlkParamDsc*)pvl)-> unnV1[LAN_BLOCK_ORD_NUM_03].ulV));
    }





   
}while(sLV.ch_while_breaker);
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
      MASKA_TRIGGER_SIGNALES_6,
      MASKA_TRIGGER_SIGNALES_7,
      MASKA_TRIGGER_SIGNALES_8
      
#ifdef MASKA_TRIGGER_SIGNALES_9
                            ,
      MASKA_TRIGGER_SIGNALES_9
#endif
    };
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
    trigger_functions_USB[i]   |= temp_data;
    trigger_functions_RS485[i] |= temp_data;
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
    trigger_functions_LAN[i]   |= temp_data;
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
  if (_CHECK_SET_BIT(active_functions, RANG_RESET_RELES) !=0)
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
    
      for (unsigned int j = 0; j < N_BIG; j++) temp_array_of_outputs[j] = current_settings_prt.ranguvannja_outputs[N_BIG*i + j] & active_functions[j];

      //Сигнал "Аварійна несправність" працює у інверсному режимі: замикає реле на якому зранжована у випадку, коли даний сигнал не активинй
      if(_CHECK_SET_BIT((current_settings_prt.ranguvannja_outputs + N_BIG*i), RANG_AVAR_DEFECT) !=0)
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
      if(_CHECK_SET_BIT((current_settings_prt.ranguvannja_outputs + N_BIG*i), RANG_DEFECT) !=0)
      {
        //Сигнал "Загальна несправність"  справді зранжовано на даний вихід
        if (_CHECK_SET_BIT(temp_array_of_outputs, RANG_DEFECT) ==0)
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
      flag=0;
      for(int m=0; m<N_BIG; m++) if(temp_array_of_outputs[m] != 0) {flag=1; break;}
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
          if (_CHECK_SET_BIT((current_settings_prt.ranguvannja_outputs + N_BIG*i), RANG_WORK_BV) ==0)
          {
            //На дане реле не заводиться сигнал БВ (блок включення)
          
            //Відмічаємо, що даний вихід - ЗАМКНУТИЙ
            state_outputs |= (1 << i);
          }
          else
          {
            //На дане реле заводиться сигнал БВ (блок включення)
          
            //Відмічаємо, що даний вихід - ЗАМКНУТИЙ тільки тоді, коли функція БВ активна зараз
            if (_CHECK_SET_BIT(active_functions, RANG_WORK_BV) !=0)
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
    
        for (unsigned int j = 0; j < N_BIG; j++) temp_array_of_outputs[j] = current_settings_prt.ranguvannja_outputs[N_BIG*i + j] & active_functions[j];

        //Сигнал "Аварійна несправність" працює у інверсному режимі: замикає реле на якому зранжована у випадку, коли даний сигнал не активинй
        if(_CHECK_SET_BIT((current_settings_prt.ranguvannja_outputs + N_BIG*i), RANG_AVAR_DEFECT) !=0)
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
        if(_CHECK_SET_BIT((current_settings_prt.ranguvannja_outputs + N_BIG*i), RANG_DEFECT) !=0)
        {
          //Сигнал "Загальна несправність"  справді зранжовано на даний вихід
          if (_CHECK_SET_BIT(temp_array_of_outputs, RANG_DEFECT) ==0)
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
          if (_CHECK_SET_BIT((current_settings_prt.ranguvannja_outputs + N_BIG*i), RANG_WORK_BV) == 0)
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
    else ds = 0;
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
      ((state_outputs  & current_settings_prt.type_of_output) != state_signal_outputs)
     )   
  {
    state_signal_outputs = state_outputs  & current_settings_prt.type_of_output;
    //Виставляємо повідомлення про те, що в EEPROM треба записати нові значення сигнальних виходів і тригерних світлоіндикаторів
    _SET_BIT(control_spi1_taskes, TASK_START_WRITE_STATE_LEDS_OUTPUTS_EEPROM_BIT);
  }
  
  //Стан виходу з уразуванням імпульсного режиму роботи сигнальних виходів
  unsigned int output_signal_modif = (current_settings_prt.type_of_output_modif & current_settings_prt.type_of_output);
  state_outputs_raw = ( state_outputs & ((unsigned int)(~output_signal_modif)) ) | ((state_outputs & output_signal_modif)*output_timer_prt_signal_output_mode_2);

//Оновлюємо поріг чутливості для тих випаків, коли на цей регістр більше нічого не заводиться   
#if (                                   \
     (MODYFIKACIA_VERSII_PZ == 0) ||    \
     (MODYFIKACIA_VERSII_PZ == 3) ||    \
     (MODYFIKACIA_VERSII_PZ == 4) ||    \
     (MODYFIKACIA_VERSII_PZ == 6) ||    \
     (MODYFIKACIA_VERSII_PZ == 10)||    \
     (MODYFIKACIA_VERSII_PZ == 13)||    \
     (MODYFIKACIA_VERSII_PZ == 14)||    \
     (MODYFIKACIA_VERSII_PZ == 14)      \
    )   
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD25_DD27_DD28_DD30) = ((zdz_ovd_porig & 0xf) << 8) | (test_OVD << 12);
#endif
  
#if (                                     \
       (MODYFIKACIA_VERSII_PZ != 7) &&    \
       (MODYFIKACIA_VERSII_PZ != 17)&&    \
       (MODYFIKACIA_VERSII_PZ != 8) &&    \
       (MODYFIKACIA_VERSII_PZ != 18)      \
      )   
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37) = state_outputs_raw;
#elif (                                   \
       (MODYFIKACIA_VERSII_PZ != 8) &&    \
       (MODYFIKACIA_VERSII_PZ != 18)      \
      )  
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37) = 
    (state_outputs_raw & ((1u << 9) - 1)) |
    (((zdz_ovd_porig & 0xf) << 9) | ((test_OVD != 0) << 13)) | 
    (((state_outputs_raw >> 9) & ((1u << 2) - 1)) << 14);
#endif
  
#if (                                   \
     (MODYFIKACIA_VERSII_PZ == 5) ||    \
     (MODYFIKACIA_VERSII_PZ == 15)      \
    )  

  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD25_DD27_DD28_DD30) = (state_outputs_raw >> 16)<< 8;

#else

#ifdef NUMBER_DS
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD25_DD27_DD28_DD30) = 
#if (                                   \
     (MODYFIKACIA_VERSII_PZ == 7) ||    \
     (MODYFIKACIA_VERSII_PZ == 17)      \
    )  

          (((state_outputs_raw >> 11) & ((1u << 1) - 1)) << 1) |
  
#endif
    (ds & (MASKA_FOR_BIT(NUMBER_DS) - 1)) << 8;
#endif
  
#endif

#if (                                   \
     (MODYFIKACIA_VERSII_PZ == 8) ||    \
     (MODYFIKACIA_VERSII_PZ == 18)      \
    )  

  #define A_01_ONB  0               // 1  
  #define A_02_ONB  (A_01_ONB  + 1) // 2
  #define E_01_ONB  (A_02_ONB  + 1) // 3
  #define E_02_ONB  (E_01_ONB  + 1) // 4
  #define E_03_ONB  (E_02_ONB  + 1) // 5
  #define E_04_ONB  (E_03_ONB  + 1) // 6
  #define E_05_ONB  (E_04_ONB  + 1) // 7
  #define E_06_ONB  (E_05_ONB  + 1) // 8
  #define E_07_ONB  (E_06_ONB  + 1) // 9
  #define E_08_ONB  (E_07_ONB  + 1) // 10
  #define E_09_ONB  (E_08_ONB  + 1) // 11
  #define E_10_ONB  (E_09_ONB  + 1) // 12
  #define E_11_ONB  (E_10_ONB  + 1) // 13
  #define E_12_ONB  (E_11_ONB  + 1) // 14
  #define E_13_ONB  (E_12_ONB  + 1) // 15
  #define E_14_ONB  (E_13_ONB  + 1) // 16
  #define E_15_ONB  (E_14_ONB  + 1) // 17
  #define E_16_ONB  (E_15_ONB  + 1) // 18
  #define E_17_ONB  (E_16_ONB  + 1) // 19
  #define E_18_ONB  (E_17_ONB  + 1) // 20
  #define G_01_ONB  (E_18_ONB  + 1) // 21
  #define G_02_ONB  (G_01_ONB  + 1) // 22
  #define G_03_ONB  (G_02_ONB  + 1) // 23
  #define G_04_ONB  (G_03_ONB  + 1) // 24
  
  
  
  
  #define REL3 (3-1)
  #define REL5 (5-1)
  #define REL4 (4-1)
  #define REL6 (6-1)
  
  #define REL16 (16-1)
  #define CHDO2  (2 -1)
  #define CHDO3  (3 -1)



  #define REL3_REL4_REL5_REL6_BIT_VAL 0x3C  // 0x3C <-  0011 1100
  #define REL_3_4_5_6_SLOT_E_OUT_1_9_WR_VAL   ( (1 << REL3)|(1 << REL5)|(1 << REL4)|(0 << REL6)|(1<<REL16) )
  #define REL_3456_1010_SLOT_E_SELECT_FOR_WR  ( (1 << REL3)|(1 << REL5)|(0 << REL4)|(0 << REL6)|(1<<REL16) )
  #define REL_3_4_5_6_SLOT_E_OUT_10_18_WR_VAL ( (1 << REL3)|(1 << REL5)|(0 << REL4)|(1 << REL6)|(1<<REL16) )

state_outputs_raw |= state_outputs_raw_dbg;

   unsigned long u32_rele_val  = state_outputs_raw ;//<- state out Rele
  //``````````````````````````````````````````````````````````````````
  //...........БП3
  //...........Двих.А.1      1
  //...........Двих.А.2      2
  //-REL1 REL2 - на блок хивлення
  //``````````````````````````````````````````````````````````````````
   unsigned char ch_REL1_REL2_VAL =   (u32_rele_val&(1 << A_01_ONB))
                                    | (u32_rele_val&(1 << A_02_ONB)) ;//А.1,А.2, 
  
   volatile unsigned char  ch_REL7_REL8_VAL =   ( ((u32_rele_val&(1<<E_01_ONB))>>E_01_ONB) << 6)
                                |    ( ((u32_rele_val&(1<<E_02_ONB))>>E_02_ONB) << 7);//E1 E2 out 
                          
  
  volatile unsigned char ch_REL_3_4_5_6_WR_VAL = 0;
  
  extern volatile unsigned char  chGbl__CDHO1_CDHO7__W_VAL;
  extern volatile unsigned char  chGbl__REL1_REL8__W_VAL;
  //?chGbl__REL1_REL8__W_VAL &= 3;chGbl__REL1_REL8__W_VAL |= ch_REL1_REL2_VAL;
  //define DBG_MODE
  //Set CHDO
  unsigned char  chLocal__CDHO1_CDHO7__W_VAL = ( ~( ( 1 << CHDO2)|( 1 << CHDO3) ) )&chGbl__CDHO1_CDHO7__W_VAL ;
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD39_DD40_DD47) = chLocal__CDHO1_CDHO7__W_VAL
                                                                | ( 1 << CHDO2) | ( 0 << CHDO3);
  #ifdef DBG_MODE 
  #else
  unsigned short Rel_1__2_E1_E2__E3_E9_SLOT_E_WR_Val = ch_REL1_REL2_VAL | ch_REL7_REL8_VAL| ( ((u32_rele_val>>E_03_ONB)&0x7f) << 8 );
  //..=:>>_Установити на виходах регістрів БВ3 дані, потрібні для запису в виходи 1 – 9;<<-..
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37) = Rel_1__2_E1_E2__E3_E9_SLOT_E_WR_Val|(1<<REL16); 
  //..^
  _DELAY_ABOUT_40NS()
  //..=:>>_Установити REL4 в «0», REL6 в «0»;<<-..
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37) = Rel_1__2_E1_E2__E3_E9_SLOT_E_WR_Val|(1<<REL16); 
  //..^
  #endif

  #ifdef DBG_MODE 

  #else
  //..=:>>_Установити REL3, REL5 в «1»<<-..
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37) = 
  REL_3456_1010_SLOT_E_SELECT_FOR_WR | Rel_1__2_E1_E2__E3_E9_SLOT_E_WR_Val | (1<<REL16);
  //..^
  _DELAY_ABOUT_40NS()
  #endif
   
  #ifdef DBG_MODE 

  #else
  //..=:>>_Установити REL4 в «1» (записати дані у виходи 1-9)<<-..
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37) = 
  REL_3_4_5_6_SLOT_E_OUT_1_9_WR_VAL | Rel_1__2_E1_E2__E3_E9_SLOT_E_WR_Val | (1<<REL16);
  //..^
  #endif
  //

   
  #ifdef DBG_MODE 
  #else
  //..=:>>_Установити REL4 в «0»;<<-..
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37) = 
  REL_3456_1010_SLOT_E_SELECT_FOR_WR | Rel_1__2_E1_E2__E3_E9_SLOT_E_WR_Val | (1<<REL16);
  //..^
  #endif

  ch_REL7_REL8_VAL = (((u32_rele_val&(1<<E_10_ONB))>>E_10_ONB)<<6)|(((u32_rele_val&(1<<E_11_ONB))>>E_11_ONB)<<7);//E10 E11 out 

  #ifdef DBG_MODE
  #else
  //..=:>>_Установити на виходах регістрів БВ3 дані, потрібні для запису в виходи 10– 18;<<-..
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37) = 
  ch_REL1_REL2_VAL | ch_REL7_REL8_VAL | ( ((u32_rele_val>>E_12_ONB)&0x7f) << 8 ) | (1<<REL16);
  //..^
  #endif
  
  _DELAY_ABOUT_40NS()
  
  #ifdef DBG_MODE

  #else
  //..=:>>_Установити  REL6 в «1» (записати дані у виходи 10-18);<<-..
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37) = 
  REL_3_4_5_6_SLOT_E_OUT_10_18_WR_VAL | ch_REL1_REL2_VAL | ch_REL7_REL8_VAL | ( ((u32_rele_val>>E_12_ONB)&0x7f) << 8 ) | (1<<REL16);
  //..^
  #endif
  #ifdef DBG_MODE

  #else
  //..=:>>_Установити REL6 в «0»<<-..
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37) = 
  REL_3456_1010_SLOT_E_SELECT_FOR_WR | ch_REL1_REL2_VAL | ch_REL7_REL8_VAL | ( ((u32_rele_val>>E_12_ONB)&0x7f) << 8 ) | (1<<REL16);
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
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37) = (unsigned short)(arU32[2] | ch_REL1_REL2_VAL| ch_REL7_REL8_VAL)|(1<<REL16);
  #else
  //..=:>>_Установити REL3, REL5 в «0».<<-..
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37) = //CLEAR REL_3_4_5_6_SLOT_E_OUT ESTABLISH DATA
   ch_REL1_REL2_VAL | ch_REL7_REL8_VAL | ( ((u32_rele_val>>E_12_ONB)&0x7f) << 8 )|(1<<REL16);
  //..^
  #endif
  //БДВВ6 
  //AOUT1-AOUT8 (DD28) 
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD25_DD27_DD28_DD30) = ((u32_rele_val>>G_01_ONB)&0xf) << 8;//
  chGbl__REL1_REL8__W_VAL = ch_REL1_REL2_VAL ;//
//////////////////////////////////////////////////////////////////////////////////////////
  //?chGbl__REL1_REL8__W_VAL &= 3;chGbl__REL1_REL8__W_VAL |= ch_REL1_REL2_VAL;
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37) = chGbl__REL1_REL8__W_VAL|(1<<REL16);//Restore REL1_REL8?
  
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD39_DD40_DD47) = chGbl__CDHO1_CDHO7__W_VAL;//Restore CDHO

///////////////////////////////////////////////////////////////////////////////////////////

    

  
  
#endif
  TIM_PRT_write_tick = TIM2->CNT;
  /**************************/

  /**************************/
  //Вивід інформації на світлодіоди
  /**************************/
  //Спочатку перевіряємо, чи не активовувалвся команда "Сблос индикации" - і якщо так, то попередньо скидаємо всю індикацію
  if (_CHECK_SET_BIT(active_functions, RANG_RESET_LEDS) !=0)
  {
    state_leds = 0;
  }
  
  //Визначаємо, які світлоіндикатори зараз мають бути активними
  for (unsigned int i = 0; i < NUMBER_LEDS; i++)
  {
    //У тимчасовий масив копіюємо ранжування світлоіндикатора, який індексується інедексом "i"
    unsigned int temp_array_of_leds[N_BIG];
    
    //Перевіряємо, чи є співпадіння між ранжованими функціями на цьому світлоіндикаторі і функціями, які зараз є активними - умова активації виходу
    for (unsigned int j = 0; j < N_BIG; j++ ) temp_array_of_leds[j] = current_settings_prt.ranguvannja_leds[N_BIG*i + j] & active_functions[j];

    flag=0;
    for(int m=0; m<N_BIG; m++) if(temp_array_of_leds[m] != 0) {flag=1; break;}
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
      ((state_leds  & current_settings_prt.type_of_led) != state_trigger_leds)
     )   
  {
    state_trigger_leds = state_leds  & current_settings_prt.type_of_led;
    //Виставляємо повідомлення про те, що в EEPROM треба записати нові значення сигнальних виходів і тригерних світлоіндикаторів
    _SET_BIT(control_spi1_taskes, TASK_START_WRITE_STATE_LEDS_OUTPUTS_EEPROM_BIT);
  }
  
  //Сервісні світлоіндикатори
  /*Run_Error*/
  if (_CHECK_SET_BIT(active_functions, RANG_AVAR_DEFECT) == 0) state_leds_ctrl |=  (1 << LED_COLOR_GREEN_BIT) << ((uint32_t)NUMBER_LED_COLOR*(uint32_t)LED_CTRL_R_E);
  else state_leds_ctrl &=  (uint32_t)(~((1 << LED_COLOR_GREEN_BIT) << ((uint32_t)NUMBER_LED_COLOR*(uint32_t)LED_CTRL_R_E)));
  if (
      (_CHECK_SET_BIT(active_functions, RANG_DEFECT     ) != 0) ||
      (_CHECK_SET_BIT(active_functions, RANG_AVAR_DEFECT) != 0)
     ) state_leds_ctrl |=  (1 << LED_COLOR_RED_BIT) << ((uint32_t)NUMBER_LED_COLOR*(uint32_t)LED_CTRL_R_E);
  else state_leds_ctrl &=  (uint32_t)(~((1 << LED_COLOR_RED_BIT) << ((uint32_t)NUMBER_LED_COLOR*(uint32_t)LED_CTRL_R_E)));
  
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
      
      state_leds_tmp = (((state_leds_auto >>  0) & 0x1) << 0) |
                       (((state_leds_auto >>  2) & 0x1) << 1) |
                       (((state_leds_auto >>  4) & 0x1) << 2) |
                       (((state_leds_auto >>  6) & 0x1) << 3) |
                       (((state_leds_auto >>  8) & 0x1) << 4) |
                       (((state_leds_auto >> 10) & 0x1) << 5) |
                       (((state_leds_auto >> 12) & 0x1) << 6) |
                       (((state_leds_auto >> 14) & 0x1) << 7);
      break;
    }
  case 1:
    {
      uint32_t state_leds_auto = state_leds_lock[bank_lock_tmp1];
      uint32_t state_leds_ctrl_auto = state_leds_ctrl_lock[bank_lock_tmp1];
      
      state_leds_tmp = (((state_leds_auto >>  1) & 0x1) << 0) |
                       (((state_leds_auto >>  3) & 0x1) << 1) |
                       (((state_leds_auto >>  5) & 0x1) << 2) |
                       (((state_leds_auto >>  7) & 0x1) << 3) |
                       (((state_leds_auto >>  9) & 0x1) << 4) |
                       (((state_leds_auto >> 11) & 0x1) << 5) |
                       (((state_leds_auto >> 13) & 0x1) << 6) |
                       ((((state_leds_ctrl_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)LED_CTRL_O)) & (1 << LED_COLOR_RED_BIT)) != 0) << 7);
      break;
    }
  case 2:
    {
      uint32_t state_leds_auto = state_leds_lock[bank_lock_tmp1];
      uint32_t state_leds_ctrl_auto = state_leds_ctrl_lock[bank_lock_tmp1];
      uint32_t state_leds_Fx0_auto = state_leds_Fx_lock[bank_lock_tmp1][0];

      state_leds_tmp = ((((state_leds_ctrl_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)LED_CTRL_R_E  )) & (1 << LED_COLOR_RED_BIT)) != 0) << 0) |

                       (((state_leds_auto >> 15) & 0x1) << 1) |
                       (((state_leds_auto >> 16) & 0x1) << 2) |
      
                       ((((state_leds_Fx0_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)(1 - 1))) & (1 << LED_COLOR_RED_BIT)) != 0) << 3) | 
                       ((((state_leds_Fx0_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)(2 - 1))) & (1 << LED_COLOR_RED_BIT)) != 0) << 4) | 
                       ((((state_leds_Fx0_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)(3 - 1))) & (1 << LED_COLOR_RED_BIT)) != 0) << 5) | 
                       ((((state_leds_Fx0_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)(4 - 1))) & (1 << LED_COLOR_RED_BIT)) != 0) << 6) | 
                       ((((state_leds_Fx0_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)(5 - 1))) & (1 << LED_COLOR_RED_BIT)) != 0) << 7);
      break;
    }
  case 3:
    {
      uint32_t state_leds_ctrl_auto = state_leds_ctrl_lock[bank_lock_tmp1];
      uint32_t state_leds_Fx0_auto = state_leds_Fx_lock[bank_lock_tmp1][0];

      state_leds_tmp = ((((state_leds_ctrl_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)LED_CTRL_R_E  )) & (1 << LED_COLOR_GREEN_BIT)) != 0) << 0) |

                       ((((state_leds_Fx0_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)(1 - 1))) & (1 << LED_COLOR_GREEN_BIT)) != 0) << 3) | 
                       ((((state_leds_Fx0_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)(2 - 1))) & (1 << LED_COLOR_GREEN_BIT)) != 0) << 4) | 
                       ((((state_leds_Fx0_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)(3 - 1))) & (1 << LED_COLOR_GREEN_BIT)) != 0) << 5) | 
                       ((((state_leds_Fx0_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)(4 - 1))) & (1 << LED_COLOR_GREEN_BIT)) != 0) << 6) | 
                       ((((state_leds_Fx0_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)(5 - 1))) & (1 << LED_COLOR_GREEN_BIT)) != 0) << 7);

      break;
    }
  case 4:
    {
      uint32_t state_leds_ctrl_auto = state_leds_ctrl_lock[bank_lock_tmp1];
      uint32_t state_leds_Fx0_auto = state_leds_Fx_lock[bank_lock_tmp1][0];
      uint32_t state_leds_Fx1_auto = state_leds_Fx_lock[bank_lock_tmp1][1];

      state_leds_tmp = ((((state_leds_Fx0_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)(6 - 1))) & (1 << LED_COLOR_RED_BIT)) != 0) << 0) |
                       ((((state_leds_Fx1_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)(1 - 1))) & (1 << LED_COLOR_RED_BIT)) != 0) << 1) |
                       ((((state_leds_Fx1_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)(2 - 1))) & (1 << LED_COLOR_RED_BIT)) != 0) << 2) |
                       ((((state_leds_Fx1_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)(3 - 1))) & (1 << LED_COLOR_RED_BIT)) != 0) << 3) |
                       ((((state_leds_Fx1_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)(4 - 1))) & (1 << LED_COLOR_RED_BIT)) != 0) << 4) |
                       ((((state_leds_Fx1_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)(5 - 1))) & (1 << LED_COLOR_RED_BIT)) != 0) << 5) |
                       ((((state_leds_Fx1_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)(6 - 1))) & (1 << LED_COLOR_RED_BIT)) != 0) << 6) |
                       ((((state_leds_ctrl_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)LED_CTRL_I)) & (1 << LED_COLOR_RED_BIT)) != 0) << 7);
      break;
    }
  case 5:
    {
      uint32_t state_leds_ctrl_auto = state_leds_ctrl_lock[bank_lock_tmp1];
      uint32_t state_leds_Fx0_auto = state_leds_Fx_lock[bank_lock_tmp1][0];
      uint32_t state_leds_Fx1_auto = state_leds_Fx_lock[bank_lock_tmp1][1];

      state_leds_tmp = ((((state_leds_Fx0_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)(6 - 1))) & (1 << LED_COLOR_GREEN_BIT)) != 0) << 0) |
                       ((((state_leds_Fx1_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)(1 - 1))) & (1 << LED_COLOR_GREEN_BIT)) != 0) << 1) |
                       ((((state_leds_Fx1_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)(2 - 1))) & (1 << LED_COLOR_GREEN_BIT)) != 0) << 2) |
                       ((((state_leds_Fx1_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)(3 - 1))) & (1 << LED_COLOR_GREEN_BIT)) != 0) << 3) |
                       ((((state_leds_Fx1_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)(4 - 1))) & (1 << LED_COLOR_GREEN_BIT)) != 0) << 4) |
                       ((((state_leds_Fx1_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)(5 - 1))) & (1 << LED_COLOR_GREEN_BIT)) != 0) << 5) |
                       ((((state_leds_Fx1_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)(6 - 1))) & (1 << LED_COLOR_GREEN_BIT)) != 0) << 6) |
                       ((((state_leds_ctrl_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)LED_CTRL_I)) & (1 << LED_COLOR_GREEN_BIT)) != 0) << 7);
      break;
    }
  case 6:
    {
      uint32_t state_leds_ctrl_auto = state_leds_ctrl_lock[bank_lock_tmp1];

      state_leds_tmp = ((((state_leds_ctrl_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)LED_CTRL_O)) & (1 << LED_COLOR_GREEN_BIT)) != 0) << 7);
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
    TIM2->SR = (uint16_t)((~(uint32_t)TIM_IT_CC1) & 0xffff);
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
    if ( time_ms >= 1000) 
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
      
      if (copying_time_to_RTC == 1) copying_time_to_RTC = 0;
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
        if (llabs(diff_s*1000 + diff_ms) > 2000)
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

    if (koef_resurs_changed == CHANGED_ETAP_ENDED)
    {
      //Коефіцієнти для підрахунку ресурсу вимикача
      K_resurs_prt = K_resurs;

      //Помічаємо, що зміни прийняті системою захистів
      koef_resurs_changed = CHANGED_ETAP_NONE;
    }
    /***********************************************************/

    /***********************************************************/
    //Перевіряємо необхідність очистки дискретного реєстраторів
    /***********************************************************/
    if (
        ((clean_rejestrators & CLEAN_DR) != 0)
        &&  
        (
         (control_tasks_dataflash & (
                                     TASK_MAMORY_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_DR           | 
                                     TASK_MAMORY_READ_DATAFLASH_FOR_DR_USB                              |
                                     TASK_MAMORY_READ_DATAFLASH_FOR_DR_RS485                            |
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                                     TASK_MAMORY_READ_DATAFLASH_FOR_DR_LAN                              |
#endif  
                                     TASK_MAMORY_READ_DATAFLASH_FOR_DR_MENU                             |
                                     TASK_MAMORY_READ_DATAFLASH_FOR_DR_MENU_SHORT
                                    )
         ) == 0
        )
       )   
    {
      //Виставлено каманда очистити дискретного реєстратор

      //Виставляємо команду запису цієї структури у EEPROM
      _SET_BIT(control_spi1_taskes, TASK_START_WRITE_INFO_REJESTRATOR_DR_EEPROM_BIT);

      //Очищаємо структуру інформації по дискретному реєстраторі
      info_rejestrator_dr.next_address = MIN_ADDRESS_DR_AREA;
      info_rejestrator_dr.saving_execution = 0;
      info_rejestrator_dr.number_records = 0;

      //Помічаємо, що номер запису не вибраний
      number_record_of_dr_for_menu  = 0xffff;
      number_record_of_dr_for_USB   = 0xffff;
      number_record_of_dr_for_RS485 = 0xffff;
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
      number_record_of_dr_for_LAN = 0xffff;
#endif  

      //Знімаємо команду очистки дискретного реєстратора
      clean_rejestrators &= (unsigned int)(~CLEAN_DR);
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
#pragma diag_suppress=Pa082 
    uint64_t TIM_PRT_delta_write_read;
    if (TIM_PRT_read_tick < TIM_PRT_write_tick)
      TIM_PRT_delta_write_read = (uint64_t)TIM_PRT_read_tick + 0x100000000ull - (uint64_t)TIM_PRT_write_tick;
    else TIM_PRT_delta_write_read = TIM_PRT_read_tick - TIM_PRT_write_tick;
    if (TIM_PRT_delta_write_read > (TIM2_MIN_PERIOD_WRITE_READ + 1))
    {
      unsigned int control_state_outputs =    
#if (                                     \
       (MODYFIKACIA_VERSII_PZ != 7) &&    \
       (MODYFIKACIA_VERSII_PZ != 17)      \
      )   
        ((~((unsigned int)(_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37)))) & ((1u << 16) - 1))
#else
         ( (~((unsigned int)(_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37))))        & ((1u << 9) - 1))       |
        ((((~((unsigned int)(_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37)))) >> 14) & ((1u << 2) - 1)) << 9)
#endif

#if (                                   \
     (MODYFIKACIA_VERSII_PZ == 5) ||    \
     (MODYFIKACIA_VERSII_PZ == 15)      \
    )                                   
        | (((~((unsigned int)(_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD25_DD27_DD28_DD30) >> 8))) & 0xf) << 16)
#elif (                                   \
       (MODYFIKACIA_VERSII_PZ == 7) ||    \
       (MODYFIKACIA_VERSII_PZ == 17)      \
      )                                   
        | ((((~((unsigned int)(_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD25_DD27_DD28_DD30)))) >> 1) & ((1u << 1) - 1)) << 11)

#endif
      ;
      
#if (                                   \
     (MODYFIKACIA_VERSII_PZ == 8) ||    \
     (MODYFIKACIA_VERSII_PZ == 18)      \
    )  
  extern volatile unsigned char  chGbl__CDHO1_CDHO7__W_VAL;
  extern volatile unsigned char  chGbl__REL1_REL8__W_VAL;
  //Set CHDO 
   unsigned char  chLocal__CDHO1_CDHO7__W_VAL = ( ~( ( 1 << CHDO2)|( 1 << CHDO3) ) )&chGbl__CDHO1_CDHO7__W_VAL; 
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD39_DD40_DD47) = chLocal__CDHO1_CDHO7__W_VAL;//0-now
  //..=:>>_Установити REL3, REL4, REL5, REL6 в «0»;<<-..
    #define REL_3_4_5_6_SLOT_E_IN_1_9_RD_VAL ( ( 0 << REL3)| ( 1 << REL5)| ( 0 << REL4)| ( 0 << REL6) )
//#define COMENT
//???#ifndef COMENT
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37) = (chGbl__REL1_REL8__W_VAL&3)//CLEAR REL_3_4_5_6_SLOT_E_OUT ESTABLISH DATA
   |(1<<REL16);
  //..^
  unsigned char ch_REL1_REL2_R_VAL = (unsigned char)(_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37));//регістр контролю REL1-REL8 (DD34)
   unsigned long u32_ctrl_rele_val = ( ~( (unsigned long)ch_REL1_REL2_R_VAL ) )&0x3;//А.1 А.2 
  //..=:>>_Установити, REL5 в «1»;//Select Slot E
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37) = (chGbl__REL1_REL8__W_VAL&3)|REL_3_4_5_6_SLOT_E_IN_1_9_RD_VAL|(1<<REL16);
  //..^
  _DELAY_ABOUT_40NS()  
  
  //?u32_ctrl_rele_val |= ( ~( (unsigned long)(_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37)) ) )&0x3;//А.1 А.2 
    //? asm volatile(
    //?   "bkpt 1"
    //?   );
  //Регістр контролю ДВ AOUT1-AOUT8 (кола AIN1-AIN8)   Регістр контролю ДВAOUT1-AOUT8(DD30)(кола AIN1-AIN8)
  unsigned long u32_ctrl_G_01_G_04 = 
  ( ( ((unsigned long)(_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD25_DD27_DD28_DD30))) )& 0xf00)>>8;
  u32_ctrl_G_01_G_04 = ( ~( (unsigned long)u32_ctrl_G_01_G_04 ) )&0xf;//..
  #ifdef DBG_MODE
  #else
  unsigned long u32_ctrl_E_01_E_16 = (_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD33_DD36))& 0xffff;
_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD39_DD40_DD47) = chLocal__CDHO1_CDHO7__W_VAL;
  unsigned long u32_ctrl_E_16_E_18 = 
  (
    (unsigned short)( _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37))
    &((1<<2) | (1<<3))//C_R3  C_R4 
  ) >> 2;
  #endif
  u32_ctrl_rele_val |= (u32_ctrl_E_01_E_16 << 2) 
    | (u32_ctrl_E_16_E_18 << (16+2)) 
    | (u32_ctrl_G_01_G_04 << (16+2+2) );

  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37) = chGbl__REL1_REL8__W_VAL|(1<<REL16);//Restore REL1_REL8?
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD39_DD40_DD47) = chGbl__CDHO1_CDHO7__W_VAL;//Restore CDHO
//???#endif
///////////////////////////////////////////////////////////////////////////////////////////
  control_state_outputs = u32_ctrl_rele_val;//^state_outputs_rawunsigned int control_state_outputs = 
#endif
     
#ifdef NUMBER_DS
      unsigned int control_ds =  (~((unsigned int)(_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD25_DD27_DD28_DD30) >> 8))) & (MASKA_FOR_BIT(NUMBER_DS) - 1);
#endif

      static uint32_t error_rele[NUMBER_OUTPUTS];
      if (control_state_outputs != state_outputs_raw)
      {
        for (size_t index = 0; index < NUMBER_SIMPLE_OUTPUTS; ++index)
        {
          uint32_t maska = 1 << index;
        
          if ((control_state_outputs & maska) != (state_outputs_raw & maska))
          {
            if (error_rele[index] < 3) ++error_rele[index];
            if (error_rele[index] >= 3 ) _SET_BIT(set_diagnostyka, (ERROR_DIGITAL_OUTPUT_1_BIT + index));
          }
          else error_rele[index] = 0;
        }
      }
      else
      {
        for (size_t index = 0; index < NUMBER_SIMPLE_OUTPUTS; ++index) error_rele[index] = 0;
      }

#ifdef NUMBER_DS
      if (control_ds != ds)  
      {
        for (size_t index = 0; index < NUMBER_DS; ++index)
        {
          uint32_t maska = 1 << index;
        
          if ((control_ds & maska) != (ds & maska))
          {
            if (error_rele[NUMBER_SIMPLE_OUTPUTS + index] < 3) ++error_rele[NUMBER_SIMPLE_OUTPUTS + index];
            if (error_rele[NUMBER_SIMPLE_OUTPUTS + index] >= 3 ) _SET_BIT(set_diagnostyka, (ERROR_DS_OUTPUT_BIT + index));
          }
          else error_rele[NUMBER_SIMPLE_OUTPUTS + index] = 0;
        }
      }
      else
      {
        for (size_t index = 0; index < NUMBER_DS; ++index) error_rele[NUMBER_SIMPLE_OUTPUTS + index] = 0;
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
    while (DMA_StreamCANAL1_MO_Tx->NDTR != 0);

//    uint32_t tick_for_Canal_1_tmp = tick_for_Canal_1;
    uint32_t tick_for_Canal_1_tmp = TIM2->CNT;
    uint64_t delta_tmp = 0;
    do
    {
      uint32_t tick_tmp = TIM2->CNT;
      delta_tmp = (tick_tmp < tick_for_Canal_1_tmp) ? ((uint64_t)tick_tmp + 0x100000000llu - (uint64_t)tick_for_Canal_1_tmp) : (tick_tmp - tick_for_Canal_1_tmp);
    }
    while(delta_tmp < 2);
    /***/

    if (
        (_CHECK_SET_BIT(    diagnostyka, WARNING_REPROGRAM) == 0) &&
        (_CHECK_SET_BIT(set_diagnostyka, WARNING_REPROGRAM) == 0)
       )   
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
            (_CHECK_SET_BIT(control_spi1_taskes, TASK_WRITING_TRG_FUNC_EEPROM_BIT    ) == 0) &&
            (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_READ_TRG_FUNC_EEPROM_BIT ) == 0) &&
            (_CHECK_SET_BIT(control_spi1_taskes, TASK_READING_TRG_FUNC_EEPROM_BIT    ) == 0)
           ) 
        {
          //На даний моммент не іде читання-запис триґерної інформації, тому можна здійснити копіювання
          fix_active_buttons_ctrl = fix_active_buttons;
          for (unsigned int i = 0; i < N_BIG; i++) trigger_active_functions_ctrl[i] = trigger_active_functions[i];
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
            (_CHECK_SET_BIT(control_spi1_taskes, TASK_WRITING_INFO_REJESTRATOR_AR_EEPROM_BIT    ) == 0) &&
            (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_READ_INFO_REJESTRATOR_AR_EEPROM_BIT ) == 0) &&
            (_CHECK_SET_BIT(control_spi1_taskes, TASK_READING_INFO_REJESTRATOR_AR_EEPROM_BIT    ) == 0)
           ) 
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
            (_CHECK_SET_BIT(control_spi1_taskes, TASK_WRITING_INFO_REJESTRATOR_DR_EEPROM_BIT    ) == 0) &&
            (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_READ_INFO_REJESTRATOR_DR_EEPROM_BIT ) == 0) &&
            (_CHECK_SET_BIT(control_spi1_taskes, TASK_READING_INFO_REJESTRATOR_DR_EEPROM_BIT    ) == 0)
           ) 
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
            (_CHECK_SET_BIT(control_spi1_taskes, TASK_WRITING_RESURS_EEPROM_BIT    ) == 0) &&
            (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_READ_RESURS_EEPROM_BIT ) == 0) &&
            (_CHECK_SET_BIT(control_spi1_taskes, TASK_READING_RESURS_EEPROM_BIT    ) == 0)
           ) 
        {
          //На даний моммент не іде читання-запис ресурсу вимикача, тому можна здійснити копіювання
          resurs_vymykacha_ctrl = resurs_vymykacha;          
          resurs_vidkljuchennja_ctrl = resurs_vidkljuchennja;
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
        delta_time = (uint64_t)capture_new + 0x100000000ull - (uint64_t)current_tick;
      else delta_time = capture_new - current_tick;

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
              delta_tick = (uint64_t)current_tick + 0x100000000ull - (uint64_t)previous_tick;
            else delta_tick = current_tick - previous_tick;
              
            delta = delta_tick + 1;
          }
          else if (delta == TIM2_CCR1_VAL)
            delta = 1; /*Намагаємося, щоб нове переивання запустилося як омога скоріше*/
          else
          {
            //Теоретично цього ніколи не мало б бути
            total_error_sw_fixed();
          }
          TIM2->CCR1 = (capture_new = (TIM2->CNT +  delta));
          previous_tick = current_tick;
          repeat = 0xff;
        }
      }
    }
    while (repeat != 0);
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
    if( durLogicCur > (3000) ){
    //.  while(1);
    //.      //asm volatile(
    //.      //  "bkpt 1"
    //.      //);
    _SET_BIT(set_diagnostyka, TEST_OVD3);
    }
    if (durLogicMax < delta_tick) durLogicMax = delta_tick;
    if (durLogicMin > delta_tick) durLogicMin = delta_tick;
    
    static uint64_t periodLogicMax = 0;
    static uint64_t periodLogicMin = 0x100000000ull;
    static uint32_t periodLogicReset = 1; /*щоб першу точку неправильно гарантовано пропустити*/
    
    static uint32_t prev_start_tick;
    uint64_t const period_tick = (start_tick > prev_start_tick) ? (start_tick - prev_start_tick) : (0x100000000ull + start_tick - prev_start_tick);
    prev_start_tick = start_tick;
    
    periodLogicCur = period_tick;
    if (periodLogicMax < period_tick) periodLogicMax = period_tick;
    if (periodLogicMin > period_tick) periodLogicMin = period_tick;
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
void setpoints_selecting(unsigned int *p_active_functions, unsigned int act_inp_gr_ustavok) {
  unsigned int grupa_ustavok = 0;
  act_inp_gr_ustavok &= 0xf;
  if (current_settings_prt.grupa_ustavok < SETPOINT_GRUPA_USTAVOK_MIN ||
      current_settings_prt.grupa_ustavok > SETPOINT_GRUPA_USTAVOK_MAX) {
    //Неопределенная ошибка
    while (1);
  } else {
    grupa_ustavok = 1 << (current_settings_prt.grupa_ustavok - 1);
  }
  
  if (_CHECK_SET_BIT(p_active_functions, RANG_INVERS_DV_GRUPA_USTAVOK)) _SET_STATE(act_inp_gr_ustavok, 4);
  
  _AND2(grupa_ustavok, 0, act_inp_gr_ustavok, 4, grupa_ustavok, 4);
  _AND2(grupa_ustavok, 1, act_inp_gr_ustavok, 4, grupa_ustavok, 5);
  _AND2(grupa_ustavok, 2, act_inp_gr_ustavok, 4, grupa_ustavok, 6);
  _AND2(grupa_ustavok, 3, act_inp_gr_ustavok, 4, grupa_ustavok, 7);
  
  unsigned int tmp = gr_ustavok_tmp;
  do {
    _AND4(act_inp_gr_ustavok, 0, gr_ustavok_tmp, 1, gr_ustavok_tmp, 2, gr_ustavok_tmp, 3, act_inp_gr_ustavok, 5);
    _INVERTOR(act_inp_gr_ustavok, 5, tmp, 0);
    
    _AND4(tmp, 0, act_inp_gr_ustavok, 1, tmp, 2, tmp, 3, act_inp_gr_ustavok, 6);
    _INVERTOR(act_inp_gr_ustavok, 6, tmp, 1);
    
    _AND4(tmp, 0, tmp, 1, act_inp_gr_ustavok, 2, tmp, 3, act_inp_gr_ustavok, 7);
    _INVERTOR(act_inp_gr_ustavok, 7, tmp, 2);
    
    _AND4(tmp, 0, tmp, 1, tmp, 2, act_inp_gr_ustavok, 3, act_inp_gr_ustavok, 8);
    _INVERTOR(act_inp_gr_ustavok, 8, tmp, 3);
    if (tmp == gr_ustavok_tmp) {
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
void proc_Gs_blk_out(void* pv,unsigned long lCtrGsSrc,short* p_arrOrdNumsGsSignal ){

    // ----------------    -------------------------       
        register unsigned long i,lV,j;
         
         register void *pvll;

        pvll = (void*)&current_settings_prt.ranguvannja_In_GOOSE;//
        i = j = lV = 0;

        asm ("nop" ::"r"(i),"r"(j),"r"(lV));
        while(lCtrGsSrc){
            lCtrGsSrc--;
            lV = p_arrOrdNumsGsSignal[lCtrGsSrc];
            i = lV>>3;j = lV - (i<<3);

            lV = i*N_IN_GOOSE_MMS_OUT*N_SMALL +j*N_SMALL;
            for(register unsigned long k = 0; k < N_SMALL; k++){
                ((unsigned long*)pv)[k] |= ((unsigned long*)pvll+lV) [k];
            }
            
        }

}
void proc_Mms_blk_out(void* pv,unsigned long lCtrMmsSrc,short* p_arrOrdNumsMmsSignal ){

    // ----------------    -------------------------       
        register unsigned long i,lV,j;
         
         register void *pvll;

        pvll = (void*)&current_settings_prt.ranguvannja_In_MMS;//
        i = j = lV = 0;

        asm ("nop" ::"r"(i),"r"(j),"r"(lV));
        while(lCtrMmsSrc){
            lCtrMmsSrc--;
            lV = p_arrOrdNumsMmsSignal[lCtrMmsSrc];
            i = lV>>3;j = lV - (i<<3);

            lV = i*N_IN_GOOSE_MMS_OUT*N_SMALL +j*N_SMALL;
            for(register unsigned long k = 0; k < N_SMALL; k++){
                ((unsigned long*)pv)[k] |= ((unsigned long*)pvll+lV) [k];
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

void proc_Lan_blk_out(unsigned short *p_rang_Out_LAN,unsigned int *p_active_functions, void *pLanDsc){
    register unsigned long rU_V,rU_bit,rU_out, rU_Idx;
   
    unsigned long IdxBlk,l_O;
// ----------------    -------------------------
    IdxBlk = *((unsigned char*)pLanDsc);l_O = 0;//sLV.IdxBlk = LAN_BLOCK_ORD_NUM_00;    
    for(rU_out = 0; rU_out < 8; rU_out++){
        rU_bit = rU_V = 0;
        while( rU_bit < MAX_FUNCTIONS_IN_OUT_LAN ){
            //
            rU_V = (IdxBlk*N_OUT_LAN_IN*MAX_FUNCTIONS_IN_OUT_LAN) + (rU_out*MAX_FUNCTIONS_IN_OUT_LAN) + rU_bit;//find index
            rU_Idx = p_rang_Out_LAN[rU_V];//find index
            if(rU_Idx>0){
                rU_Idx--;
                //Create 32 bit mask
                rU_V = p_active_functions[rU_Idx >> 5] & ( (unsigned int)( 1 << (rU_Idx & 0x1f)));
                if(rU_V != 0){
                    ;//Set Val
                    rU_bit = MAX_FUNCTIONS_IN_OUT_LAN+1;
                }
            }
            rU_bit++;
        }
        rU_Idx = IdxBlk;//sLV.IdxBlk;
        if(rU_bit == (MAX_FUNCTIONS_IN_OUT_LAN+2) ){ //Activate Out
            //((unsigned char*)pLanDsc+1)[rU_Idx] |= 1<< rU_out;
            l_O  |= 1<< rU_out;
        }
    
    }   
    Output_Out_LAN_block[IdxBlk] = l_O;
}

#endif
/*****************************************************/

#ifdef DEBUG_TEST
#warning "TEST VARIABLES IS PRESENT"
#endif

