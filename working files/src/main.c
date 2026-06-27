/* Includes ------------------------------------------------------------------*/
#include "constants.h"
#include "fatfs.h"
#include "libraries.h"
#include "variables_global.h"
#include "functions_global.h"

__test_watchdog testWatchDogTmp =
  {
    .arrTimeout = {-1, -1, -1, -1, -1, -1, -1, -1, -1},
    .timeoutFirst = -1,
    .timeoutLast = -1,
    .l1 = -1,
    .l2 = -1,
    .timeoutInputMax = -1,
    .InputMux_l1 = -1,
    .InputMux_l2 = -1,
    .time_delta_watchdog_output_max = 0,
    .delta = -1};
__test_watchdog testWatchDogCur =
  {
    .arrTimeout = {-1, -1, -1, -1, -1, -1, -1, -1, -1},
    .timeoutFirst = -1,
    .timeoutLast = -1,
    .l1 = -1,
    .l2 = -1,
    .timeoutInputMax = -1,
    .InputMux_l1 = -1,
    .InputMux_l2 = -1,
    .time_delta_watchdog_output_max = 0,
    .delta = -1};
__test_watchdog testWatchDogMax =
  {
    .arrTimeout = {-1, -1, -1, -1, -1, -1, -1, -1, -1},
    .timeoutFirst = -1,
    .timeoutLast = -1,
    .l1 = -1,
    .l2 = -1,
    .timeoutInputMax = -1,
    .InputMux_l1 = -1,
    .InputMux_l2 = -1,
    .time_delta_watchdog_output_max = 0,
    .delta = -1

};

/*******************************************************************************/
//Робота з Watchdog
/*******************************************************************************/
//#pragma optimize = none
void watchdog_routine(unsigned int maska, unsigned int const label)
{
  static int problemPresent = 0;

  time_1_watchdog_input = time_2_watchdog_input;
  time_2_watchdog_input = TIM4->CNT;
  unsigned int delta_time;
  if (time_2_watchdog_input >= time_1_watchdog_input)
    delta_time = time_2_watchdog_input - time_1_watchdog_input;
  else
    delta_time = time_2_watchdog_input + 0xffff - time_1_watchdog_input;
  time_delta_watchdog_input = delta_time * 10;

  if (time_delta_watchdog_input > 56000)
  { //56000

    //.asm volatile(
    //.  "bkpt 1"
    //.);
    // _SET_BIT(set_diagnostyka, TEST_OVD2); //ERROR_OVER_FLOW_EXTERNAL_WATCHDOG_IN
  }

  for (size_t i = 0; i < 8; ++i)
  {
    if ((testWatchDogTmp.arrTimeout[i] >= 0) && ((control_word_of_watchdog & (1u << i)) == 0))
    {
      total_error_sw_fixed();
    }
    else if ((testWatchDogTmp.arrTimeout[i] < 0) && ((control_word_of_watchdog & (1u << i)) != 0))
    {
      testWatchDogTmp.arrTimeout[i] = ((time_2_watchdog_input >= time_2_watchdog_output) ? (time_2_watchdog_input - time_2_watchdog_output) : (time_2_watchdog_input + 0x10000 - time_2_watchdog_output)) * 10;
    }
  }
  testWatchDogTmp.arrTimeout[8] = ((time_2_watchdog_input >= time_2_watchdog_output) ? (time_2_watchdog_input - time_2_watchdog_output) : (time_2_watchdog_input + 0x10000 - time_2_watchdog_output)) * 10;
  if (testWatchDogTmp.timeoutFirst < 0)
    testWatchDogTmp.timeoutFirst = time_delta_watchdog_input;
  testWatchDogTmp.timeoutLast = time_delta_watchdog_input;
  testWatchDogTmp.l1 = testWatchDogTmp.l2;
  testWatchDogTmp.l2 = label;
  if (testWatchDogTmp.timeoutInputMax < (int32_t) time_delta_watchdog_input)
  {
    testWatchDogTmp.timeoutInputMax = time_delta_watchdog_input;
    testWatchDogTmp.InputMux_l1 = testWatchDogTmp.l1;
    testWatchDogTmp.InputMux_l2 = testWatchDogTmp.l2;
  }

  //Робота з watchdogs з контролем всіх інших систем
  if ((control_word_of_watchdog & maska) == maska)
  {
    //Змінюємо стан біту зовнішнього Watchdog на протилежний
    if (test_watchdogs != CMD_TEST_EXTERNAL_WATCHDOG)
    {
      GPIO_WriteBit(
        GPIO_EXTERNAL_WATCHDOG,
        GPIO_PIN_EXTERNAL_WATCHDOG,
        (BitAction)(1 - GPIO_ReadOutputDataBit(GPIO_EXTERNAL_WATCHDOG, GPIO_PIN_EXTERNAL_WATCHDOG)));

      time_1_watchdog_output = time_2_watchdog_output;
      time_2_watchdog_output = TIM4->CNT;
      if (time_2_watchdog_output >= time_1_watchdog_output)
        delta_time = time_2_watchdog_output - time_1_watchdog_output;
      else
        delta_time = time_2_watchdog_output + 0xffff - time_1_watchdog_output;
      time_delta_watchdog_output = delta_time * 10;

      if (time_delta_watchdog_output > 61000)
      {
        problemPresent |= 0x1;

        //.asm volatile(
        //.  "bkpt 1"
        //.);
        // _SET_BIT(set_diagnostyka, TEST_OVD3);
        //.if(time_delta_watchdog_output > 91000){
        //. asm volatile(
        //.   "bkpt 1"
        //.);
        //.}
      }
    }

    testWatchDogCur = testWatchDogTmp;
    testWatchDogCur.time_delta_watchdog_output_max = time_delta_watchdog_output;

    int32_t delta = ((time_2_watchdog_output >= time_2_watchdog_input) ? (time_2_watchdog_output - time_2_watchdog_input) : (time_2_watchdog_output + 0x10000 - time_2_watchdog_input)) * 10;
    if (testWatchDogCur.delta < delta)
      testWatchDogCur.delta = delta;

    for (size_t i = 0; i < 8; ++i)
    {
      testWatchDogTmp.arrTimeout[i] = -1;
    }
    testWatchDogTmp.timeoutFirst = -1;
    testWatchDogTmp.timeoutLast = -1;
    testWatchDogTmp.timeoutInputMax = -1;
    testWatchDogTmp.InputMux_l1 = -1;
    testWatchDogTmp.InputMux_l2 = -1;
    testWatchDogTmp.delta = -1;

    control_word_of_watchdog = 0;
  }
#ifdef DEBUG_TEST
//  else
//  {
//    unsigned int time_1_watchdog_output_tmp = time_2_watchdog_output;
//    unsigned int time_2_watchdog_output_tmp = TIM4->CNT;
//    if (time_2_watchdog_output_tmp >= time_1_watchdog_output_tmp) delta_time = time_2_watchdog_output_tmp - time_1_watchdog_output_tmp;
//    else delta_time = time_2_watchdog_output_tmp + 0xffff - time_1_watchdog_output_tmp;
//    unsigned int time_delta_watchdog_output_tmp = delta_time* 10;
//
//    if (time_delta_watchdog_output_tmp > 100000)
//    {
//      while(time_delta_watchdog_output_tmp != 0);
//    }
//  }
#endif

  if (restart_timing_watchdog == 0)
  {
    if (time_delta_watchdog_input < time_delta_watchdog_input_min)
      time_delta_watchdog_input_min = time_delta_watchdog_input;
    if (time_delta_watchdog_input > time_delta_watchdog_input_max)
      time_delta_watchdog_input_max = time_delta_watchdog_input;

    if (time_delta_watchdog_output < time_delta_watchdog_output_min)
      time_delta_watchdog_output_min = time_delta_watchdog_output;
    if (time_delta_watchdog_output > time_delta_watchdog_output_max)
    {
      time_delta_watchdog_output_max = time_delta_watchdog_output;

      if ((problemPresent & 0x3) != 0x3)
        testWatchDogMax = testWatchDogCur;
      if ((problemPresent & 0x3) == 0x1)
        problemPresent |= 0x2;
    }

    //    while (time_delta_watchdog_output > 100000);
  }
  else
  {
    restart_timing_watchdog = 0;

    time_delta_watchdog_input = 0;
    time_delta_watchdog_input_min = 0xffff * 10;
    time_delta_watchdog_input_max = 0;

    time_delta_watchdog_output = 0;
    time_delta_watchdog_output_min = 0xffff * 10;
    time_delta_watchdog_output_max = 0;
  }
}
/*******************************************************************************/

/*************************************************************************
Періодичні низькопріоритетні задачі
*************************************************************************/
// void decoderN_BIGACMDArrayLoader(void);
// void decoderN_SMALLACMDArrayLoader(void);
void periodical_operations(unsigned int full_actions)
{
  watchdog_routine(UNITED_BITS_WATCHDOG, 9);

  //Обмін через SPI_1
  if (
    (control_spi1_taskes[0] != 0) ||
    (control_spi1_taskes[1] != 0) ||
    (state_execution_spi1 > 0))
  {
    mutex_spi1 = true;
    if (driver_spi_df[number_chip_dataflsh_exchange].state_execution == TRANSACTION_EXECUTING_NONE)
    {
      main_routines_for_spi1();
    }
    mutex_spi1 = false;
  }

  //Обміну через I2C
  if (
    (control_i2c_taskes[0] != 0) ||
    (driver_i2c.state_execution > 0) ||
    (save_time_dat_l == 2) ||
    (save_time_dat_h == 2))
    main_routines_for_i2c();

  /*****/
  //Періодичний розрахунок діючих значень
  /*****/
  if (periodical_tasks_CALC_INTEGRAL_MEASUREMENTS != 0)
  {
    angle_determination();

    periodical_tasks_CALC_INTEGRAL_MEASUREMENTS = false;
  }
  /*****/

  watchdog_routine(UNITED_BITS_WATCHDOG, 62);

  //Обробка дій системи меню
  if ((reinit_LCD) && (full_actions == true))
  {
    reinit_LCD = false;
    lcd_init();
    //new_state_keyboard |= (1u <<BIT_REWRITE);
    //Обновити повністю весь екран
    current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
    view_whole_ekran();
  }

  static unsigned int lock_menu;
  if (lock_menu == false)
  {
    lock_menu = true;

    main_manu_function();
    //Обновляємо інформацію на екрані
    view_whole_ekran();

    lock_menu = false;
  }

  //Робота з Watchdog
  watchdog_routine(UNITED_BITS_WATCHDOG, 10);
  // decoderN_BIGACMDArrayLoader();
  // decoderN_SMALLACMDArrayLoader();

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
  /*******************/
  //Управління Каналом 2 міжпроцесорного обміну між БАв і комунікаційною платою
  /*******************/
  low_routine_for_KP();
  /*******************/
#endif

  if (watchdog_l2)
  {
    //Теоретично цього ніколи не мало б бути
    total_error_sw_fixed();
  }
  CleanCmdPlusTimeLog();
  /*******************/
  //Контроль достовірності важливих даних
  /*******************/
  /*
  Я перевіряю поставлені задачі через
  if ()
  else if()
  else if()
  ...
  else if ()
  Щоб за один оберт виконувалася тільки одна перевірка, тобто щоб в одному оберті
  не було надто довга затримка на фонову перевірку, хоч і важливу.
  */
  if (full_actions == true)
  {
    if (periodical_tasks_TEST_SETTINGS != 0)
    {
      //Стоїть у черзі активна задача самоконтролю таблиці настройок
      if ((state_spi1_task & STATE_SETTINGS_EEPROM_GOOD) != 0)
      {
        //Перевірку здійснюємо тільки тоді, коли таблиця настройок була успішно прочитана
        if (
          (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_WRITE_SETTINGS_EEPROM_BIT) == 0) &&
          (_CHECK_SET_BIT(control_spi1_taskes, TASK_WRITING_SETTINGS_EEPROM_BIT) == 0) &&
          (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_READ_SETTINGS_EEPROM_BIT) == 0) &&
          (_CHECK_SET_BIT(control_spi1_taskes, TASK_READING_SETTINGS_EEPROM_BIT) == 0) &&
          (changed_settings == CHANGED_ETAP_NONE))
        {
          //На даний моммент не іде читання-запис таблиці настройок, тому можна здійснити контроль достовірності
          control_settings();

          //Скидаємо активну задачу самоконтролю таблиці настройок
          periodical_tasks_TEST_SETTINGS = false;
        }
      }
      else
      {
        //Скидаємо активну задачу самоконтролю таблиці настройок, бо не було її успішне зчитування
        periodical_tasks_TEST_SETTINGS = false;
      }
    }
    else if (periodical_tasks_TEST_USTUVANNJA != 0)
    {
      //Стоїть у черзі активна задача самоконтролю юстування (і щоб не ускладнювати задачу і серійного номеру пристрою)
      if ((state_spi1_task & STATE_USTUVANNJA_EEPROM_GOOD) != 0)
      {
        //Перевірку здійснюємо тільки тоді, коли юстування було успішно прочитане
        if (
          (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_WRITE_USTUVANNJA_EEPROM_BIT) == 0) &&
          (_CHECK_SET_BIT(control_spi1_taskes, TASK_WRITING_USTUVANNJA_EEPROM_BIT) == 0) &&
          (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_READ_USTUVANNJA_EEPROM_BIT) == 0) &&
          (_CHECK_SET_BIT(control_spi1_taskes, TASK_READING_USTUVANNJA_EEPROM_BIT) == 0) &&
          (changed_ustuvannja == CHANGED_ETAP_NONE))
        {
          //На даний моммент не іде читання-запис юстування, тому можна здійснити контроль достовірності
          control_ustuvannja();

          //Скидаємо активну задачу самоконтролю юстування
          periodical_tasks_TEST_USTUVANNJA = false;
        }
      }
      else
      {
        //Скидаємо активну задачу самоконтролю таблиці настройок, бо не було її успішне зчитування
        periodical_tasks_TEST_USTUVANNJA = false;
      }
    }
    else if (periodical_tasks_TEST_TRG_FUNC_LOCK != 0)
    {
      //Стоїть у черзі активна задача самоконтролю по резервній копії для триґерної інформації
      //Виконуємо її
      control_trg_func();

      //Скидаємо активну задачу самоконтролю по резервній копії для триґерної інформації
      periodical_tasks_TEST_TRG_FUNC_LOCK = false;
    }
    else if (periodical_tasks_TEST_INFO_REJESTRATOR_AR_LOCK != 0)
    {
      //Стоїть у черзі активна задача самоконтролю по резервній копії для аналогового реєстратора
      //Виконуємо її
      unsigned int result;
      result = control_info_ar_rejestrator(&info_rejestrator_ar_ctrl, crc_info_rejestrator_ar_ctrl);

      if (result == 1)
      {
        //Контроль достовірності реєстратора пройшов успішно

        //Скидаємо повідомлення у слові діагностики
        _SET_BIT(clear_diagnostyka, ERROR_INFO_REJESTRATOR_AR_CONTROL_BIT);
      }
      else
      {
        //Контроль достовірності реєстратора не пройшов

        //Виствляємо повідомлення у слові діагностики
        _SET_BIT(set_diagnostyka, ERROR_INFO_REJESTRATOR_AR_CONTROL_BIT);
      }

      //Скидаємо активну задачу самоконтролю по резервній копії для аналогового реєстратора
      periodical_tasks_TEST_INFO_REJESTRATOR_AR_LOCK = false;
    }
    else if (periodical_tasks_TEST_INFO_REJESTRATOR_DR_LOCK != 0)
    {
      //Стоїть у черзі активна задача самоконтролю по резервній копії для дискретного реєстратора
      //Виконуємо її
      unsigned int result;
      result = control_info_rejestrator(&info_rejestrator_dr_ctrl, crc_info_rejestrator_dr_ctrl);

      if (result == 1)
      {
        //Контроль достовірності реєстратора пройшов успішно

        //Скидаємо повідомлення у слові діагностики
        _SET_BIT(clear_diagnostyka, ERROR_INFO_REJESTRATOR_DR_CONTROL_BIT);
      }
      else
      {
        //Контроль достовірності реєстратора не пройшов

        //Виствляємо повідомлення у слові діагностики
        _SET_BIT(set_diagnostyka, ERROR_INFO_REJESTRATOR_DR_CONTROL_BIT);
      }

      //Скидаємо активну задачу самоконтролю по резервній копії для аналогового реєстратора
      periodical_tasks_TEST_INFO_REJESTRATOR_DR_LOCK = false;
    }
    else if (periodical_tasks_TEST_INFO_REJESTRATOR_PR_ERR_LOCK != 0)
    {
      //Стоїть у черзі активна задача самоконтролю по резервній копії для реєстратора програмних подій
      //Виконуємо її
      unsigned int result;
      result = control_info_rejestrator(&info_rejestrator_pr_err_ctrl, crc_info_rejestrator_pr_err_ctrl);

      if (result == 1)
      {
        //Контроль достовірності реєстратора пройшов успішно

        //Скидаємо повідомлення у слові діагностики
        _SET_BIT(clear_diagnostyka, ERROR_INFO_REJESTRATOR_PR_ERR_CONTROL_BIT);
      }
      else
      {
        //Контроль достовірності реєстратора не пройшов

        //Виствляємо повідомлення у слові діагностики
        _SET_BIT(set_diagnostyka, ERROR_INFO_REJESTRATOR_PR_ERR_CONTROL_BIT);
      }

      //Скидаємо активну задачу самоконтролю по резервній копії для аналогового реєстратора
      periodical_tasks_TEST_INFO_REJESTRATOR_PR_ERR_LOCK = false;
    }
    else if (periodical_tasks_TEST_ANGLE != 0)
    {
      //Стоїть у черзі активна задача самоконтролю юстування (і щоб не ускладнювати задачу і серійного номеру пристрою)
      if (
        ((state_spi1_task & STATE_ANGLE_EEPROM_GOOD) != 0) &&
        (current_settings_prt.type_control_location == 2))
      {
        //Перевірку здійснюємо тільки тоді, коли юстування було успішно прочитане
        if (
          (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_WRITE_ANGLE_EEPROM_BIT) == 0) &&
          (_CHECK_SET_BIT(control_spi1_taskes, TASK_WRITING_ANGLE_EEPROM_BIT) == 0) &&
          (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_READ_ANGLE_EEPROM_BIT) == 0) &&
          (_CHECK_SET_BIT(control_spi1_taskes, TASK_READING_ANGLE_EEPROM_BIT) == 0))
        {
          //На даний моммент не іде читання-запис юстування, тому можна здійснити контроль достовірності
          control_angle();

          //Скидаємо активну задачу самоконтролю юстування
          periodical_tasks_TEST_ANGLE = false;
        }
      }
      else
      {
        //Скидаємо активну задачу самоконтролю
        periodical_tasks_TEST_ANGLE = false;
      }
    }
    else if (periodical_tasks_TEST_RESURS_LOCK != 0)
    {
      //Стоїть у черзі активна задача самоконтролю по резервній копії для ресурсу лічильника
      //Виконуємо її
      control_resurs();

      //Скидаємо активну задачу самоконтролю по резервній копії для аналогового реєстратора
      periodical_tasks_TEST_RESURS_LOCK = false;
    }
  }

  /*******************/

  //Підрахунок вільного ресуру процесор-програма
  if (resurs_temp < 0xfffffffe)
    resurs_temp++;

  //Робота з Watchdog
  watchdog_routine(UNITED_BITS_WATCHDOG, 11);
}
/*************************************************************************/

/*************************************************************************
Періодичні низькопріоритетні задачі комунікації
*************************************************************************/
void periodical_operations_communication(unsigned int ar_working)
{
  //Робота з Watchdog
  watchdog_routine(UNITED_BITS_WATCHDOG, 12);

  //Робота з таймером очікування нових змін налаштувань
  if ((timeout_idle_new_settings >= current_settings.timeout_idle_new_settings) && (restart_timeout_idle_new_settings == 0))
  {
    if (_CHECK_SET_BIT(active_functions, RANG_SETTINGS_CHANGED) != 0)
    {
      current_settings_interfaces = current_settings;
      type_of_settings_changed = 0;
      _CLEAR_BIT(active_functions, RANG_SETTINGS_CHANGED);
    }
  }

  static unsigned int selection_interface;
  unsigned int const block_interface = (((POWER_CTRL->IDR & POWER_CTRL_PIN) == (uint32_t) Bit_RESET) && (ar_working != false));

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
  if ((block_interface == false) && (selection_interface == LAN_RECUEST))
  {
    //Обмін по LAN
    if (current_settings.password_interface_LAN)
    {
      unsigned int timeout = current_settings.timeout_deactivation_password_interface_LAN;
      if ((timeout != 0) && (timeout_idle_LAN >= timeout) && ((restart_timeout_interface & (1 << LAN_RECUEST)) == 0))
        password_set_LAN = 1;
    }

    // if (LAN_received_count > 0)
    //   inputPacketParserLAN();
  }
#endif

  if ((block_interface == false) && (selection_interface == USB_RECUEST))
  {
    //Обмін по USB
    if (current_settings.password_interface_USB)
    {
      unsigned int timeout = current_settings.timeout_deactivation_password_interface_USB;
      if ((timeout != 0) && (timeout_idle_USB >= timeout) && ((restart_timeout_interface & (1 << USB_RECUEST)) == 0))
        password_set_USB = 1;
    }
    Usb_routines();
  }

  if ((block_interface == false) && (selection_interface == RS485_RECUEST))
  {
    //Обмін по RS-485
    if (current_settings.password_interface_RS485)
    {
      unsigned int timeout = current_settings.timeout_deactivation_password_interface_RS485;
      if ((timeout != 0) && (timeout_idle_RS485 >= timeout) && ((restart_timeout_interface & (1 << RS485_RECUEST)) == 0))
        password_set_RS485 = 1;
    }
    if (
      (RxBuffer_RS485_count != 0) &&
      (make_reconfiguration_RS_485 == 0) &&
      ((DMA_StreamRS485_Rx->CR & (uint32_t) DMA_SxCR_EN) == 0))
    {
      //Це є умовою, що дані стоять у черзі  на обробку

      //Робота з Watchdog
      watchdog_routine(UNITED_BITS_WATCHDOG, 13);

      // //Обробляємо запит
      // inputPacketParserRS485();

      //Виставляємо, що кількість прийнятих байт рівна 0
      RxBuffer_RS485_count = 0;
    }
    else if (make_reconfiguration_RS_485 != 0)
    {
      //Стоїть умова переконфігурувати RS-485

      //Перевіряємо чи на даний моент не іде передача даних на верхній рівень
      if (GPIO_ReadOutputDataBit(GPIO_485DE, GPIO_PIN_485DE) == Bit_RESET)
      {

        //Переконфігуровуємо USART для RS-485
        USART_RS485_Configure();

        //Відновлюємо моніторинг каналу RS-485
        restart_monitoring_RS485();

        //Знімаємо індикацю про невикану переконфігупацію інтерфейсу RS-485
        make_reconfiguration_RS_485 = 0;
      }
    }
  }

  //Робота з Watchdog
  watchdog_routine(UNITED_BITS_WATCHDOG, 14);

  selection_interface++;
  selection_interface %= MAX_INTERFACES;
}
/*************************************************************************/

/*******************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int main(void)
{

  //  /************************************************************/
  //  //Перевірка контрольної суми програми
  //  /************************************************************/
  //  {
  //    unsigned short sum = 0;
  //    unsigned char *point = ((unsigned char *)&__checksum_begin);
  //    for (unsigned int i = ((unsigned int)&__checksum_end -(unsigned int)&__checksum_begin +1); i > 0; i--)
  //      sum += *point++;
  //    if (sum != (unsigned short)__checksum)
  //    {
  //      while(1);
  //    }
  //  }
  //  /************************************************************/

  /************************************************************/
  //Стартова ініціалізація
  /************************************************************/
#ifdef SYSTEM_VIEWER_ENABLE
  SEGGER_SYSVIEW_Conf(); /* Configure and initialize SystemView  */
#endif

  //Виставляємо подію про зупинку пристрою у попередньому сеансі роботи, а час встановиться пізніше, RTC запм'ятовує час пропадання живлення
  _SET_BIT(set_diagnostyka, EVENT_STOP_SYSTEM_BIT);
  changing_diagnostyka_state(); //Підготовлюємо новий запис для реєстратора програмних подій

  //Перевіряємо, що відбулося: запуск приладу, чи перезапуск (перезапуск роботи приладу без зняття оперативного живлення)
  if (RCC_GetFlagStatus(RCC_FLAG_SFTRST) == SET)
  {
    //Виставляємо подію про програмний перезапуск пристрою
    _SET_BIT(set_diagnostyka, EVENT_SOFT_RESTART_SYSTEM_BIT);
  }
  else if (RCC_GetFlagStatus(RCC_FLAG_BORRST /*RCC_FLAG_PORRST*/) != SET)
  {
    //Виставляємо подію про перезапуск пристрою (бо не зафіксовано подію Power-on/Power-down)
    _SET_BIT(set_diagnostyka, EVENT_RESTART_SYSTEM_BIT);
  }
  else
  {
    //Виставляємо подію про запуск пристрою
    _SET_BIT(set_diagnostyka, EVENT_START_SYSTEM_BIT);
  }
  //Очищаємо прапорці
  RCC->CSR |= RCC_CSR_RMVF;
  changing_diagnostyka_state(); //Підготовлюємо новий запис для реєстратора програмних подій

  //Стартова настройка периферії процесора
  start_settings_peripherals();

  //  static const size_t index_global_tmp = EKRAN_UNSUPPORT_REPROGRAM;
  //  position_in_current_level_menu[index_global_tmp] = 0; //Тест, що оптимально вибрано константу MAX_LEVEL_MENU
  //  position_in_current_level_menu[EKRAN_UNSUPPORT_REPROGRAM] = 0; //Тест, що оптимально вибрано константу MAX_LEVEL_MENU
  //#warning "Test size menu array"

  if (
    ((state_spi1_task & STATE_SETTINGS_EEPROM_GOOD) != 0) &&
    ((state_spi1_task & STATE_TRG_FUNC_EEPROM_GOOD) != 0))
  {
    //Випадок, якщо настройки успішно зчитані

    /*******************************************************/
    //Активовуємо величини для вимірювальної системи і системи захистів
    /*******************************************************/
    if (changed_ustuvannja == CHANGED_ETAP_ENDED) /*Це є умова, що нові дані підготовлені для передачі їх у роботу вимірювальною системою (і при цьому зараз дані не змінюються)*/
    {
      for (unsigned int k = 0; k < NUMBER_ANALOG_CANALES; k++)
      {
        //Копіюємо масив юстування у копію цього масиву але з яким працює (читає і змінює) тільки вимірювальна захистема
        ustuvannja_meas[k] = ustuvannja[k];
      }
      //Помічаємо, що зміни прийняті всіма системами
      changed_ustuvannja = CHANGED_ETAP_NONE;
    }

    if (changed_settings == CHANGED_ETAP_ENDED) /*Це є умова, що нові дані підготовлені для передачі їх у роботу системою захистів (і при цьому зараз дані не змінюються)*/
    {
      //Копіюємо таблицю настройок у копію цієї таблиці але з якою працює (читає і змінює) тільки система захистів
      current_settings_prt = current_settings;
      type_of_input_prt = current_settings_prt.type_of_input;
      type_of_input_signal_prt = current_settings_prt.type_of_input_signal;
      for (size_t i = 0; i < NUMBER_INPUTS; ++i)
        dopusk_dv_prt[i] = current_settings_prt.dopusk_dv[i];

      //Помічаємо, що зміни прийняті всіма системами
      changed_settings = CHANGED_ETAP_NONE;
    }
    /*******************************************************/

    //Дозволяєм роботу таймера вимірювальної системи
    TIM_Cmd(TIM5, ENABLE);
    // Дозволяєм роботу таймера системи захистів
    TIM_Cmd(TIM2, ENABLE);

    //    TEST_OUTPUT->BSRRH = TEST_OUTPUT_PIN;

    //Робота з watchdogs
    watchdog_routine(WATCHDOG_KYYBOARD, 15);

    /**********************/
    //Конфігуруємо I2C
    /**********************/
    Configure_I2C(I2C);
    /**********************/

    //Робота з watchdogs
    watchdog_routine(WATCHDOG_KYYBOARD, 16);

    //Виставляємо признак, що требаа прочитати всі регістри RTC, а потім, при потребі відкоректувати його поля
    //При цьому виставляємо біт блокування негайного запуску операції, щоб засинхронізуватися з роботою вимірювальної системи
    _SET_BIT(control_i2c_taskes, TASK_START_READ_RTC_BIT);
    _SET_BIT(control_i2c_taskes, TASK_BLK_OPERATION_BIT);
    //Обмін через I2C
    while (
      (control_i2c_taskes[0] != 0) ||
      (driver_i2c.state_execution > 0))
    {
      main_routines_for_i2c();

      //Робота з watchdogs
      watchdog_routine(WATCHDOG_KYYBOARD, 17);
    }

    //Ініціалізація LCD
    lcd_init();
    changing_diagnostyka_state(); //Підготовлюємо новий потенційно можливий запис для реєстратора програмних подій
  }
  else
  {
    //Випадок, якщо настройки успішно не зчитані, або їх взагалі немає

    //Ініціалізація LCD
    lcd_init();
    changing_diagnostyka_state(); //Підготовлюємо новий потенційно можливий запис для реєстратора програмних подій

    //Якщо настройки не зчитані успішно з EEPROM, то спочатку виводимо на екран повідомлення про це
    while (
      ((state_spi1_task & STATE_SETTINGS_EEPROM_GOOD) == 0) ||
      ((state_spi1_task & STATE_TRG_FUNC_EEPROM_GOOD) == 0))
    {
      error_reading_with_eeprom();
    }

    /*****/
    /*
    Очищаємо структуру інформації по аналоговому реєстраторі оскільки настройки 
    встановлені у мінімальну конфігурацію, а це значить, що, можливо, величини 
    ширин доаварійного і післяаварійного масивів не будуть співпадати (на яких 
    до цього часу працював аналоговий реєстратор і при мінімильній конфігурації)
    */
    /*****/
    //Виставляємо команду запису цієї структури у EEPROM
    /*
    Команду виставляємо скоріше, а потім робимо зміни у полях, які треба змінити,
    бо по вимозі проконтролювати достовірність даних інформації по аналоговому
    реєстратору відбувається копіювання з системи захистів структури
    info_rejestrator_ar у резервну мкопію. Це копіювання блокується у випадку 
    "читання з"/"запису в" EEPROM цієї інформації. Тому виставлення спочатку команди
    запису заблокує копіювання.
    З другої сторони не можливо, щоб почався запис до модифікації, 
    бо запис ініціюється функцією main_routines_for_spi1 - яка виконується на одному і тому ж
    рівні пріоритетності, що і функція main.
    Тобто спочатку треба дійти до виклику функції main_routines_for_spi1, і аж тоді можливе
    виконання команди, яку ми виставили перед зміною даних, яку 
    ми зараз гарантовано зробимо (до виклику функції main_routines_for_spi1)
    */
    _SET_BIT(control_spi1_taskes, TASK_START_WRITE_INFO_REJESTRATOR_AR_EEPROM_BIT);

    info_rejestrator_ar.first_number = -1;
    info_rejestrator_ar.last_number = -1;
    _SET_STATE(FATFS_command, FATFS_FORMAT);
    while (
      (control_spi1_taskes[0] != 0) ||
      (control_spi1_taskes[1] != 0) ||
      (state_execution_spi1 > 0))
    {
      //Робота з watchdogs
      watchdog_routine(WATCHDOG_KYYBOARD, 18);

      main_routines_for_spi1();
    }
    /*****/

    /*******************************************************/
    //Активовуємо величини для вимірювальної системи і системи захистів
    /*******************************************************/
    if (changed_ustuvannja == CHANGED_ETAP_ENDED) /*Це є умова, що нові дані підготовлені для передачі їх у роботу вимірювальною системою (і при цьому зараз дані не змінюються)*/
    {
      for (unsigned int k = 0; k < NUMBER_ANALOG_CANALES; k++)
      {
        //Копіюємо масив юстування у копію цього масиву але з яким працює (читає і змінює) тільки вимірювальна захистема
        ustuvannja_meas[k] = ustuvannja[k];
      }
      //Помічаємо, що зміни прийняті всіма системами
      changed_ustuvannja = CHANGED_ETAP_NONE;
    }

    if (changed_settings == CHANGED_ETAP_ENDED) /*Це є умова, що нові дані підготовлені для передачі їх у роботу системою захистів (і при цьому зараз дані не змінюються)*/
    {
      //Копіюємо таблицю настройок у копію цієї таблиці але з якою працює (читає і змінює) тільки система захистів
      current_settings_prt = current_settings;
      type_of_input_prt = current_settings_prt.type_of_input;
      type_of_input_signal_prt = current_settings_prt.type_of_input_signal;
      for (size_t i = 0; i < NUMBER_INPUTS; ++i)
        dopusk_dv_prt[i] = current_settings_prt.dopusk_dv[i];

      //Помічаємо, що зміни прийняті всіма системами
      changed_settings = CHANGED_ETAP_NONE;
    }
    /*******************************************************/

    //Дозволяєм роботу таймера вимірювальної системи
    TIM_Cmd(TIM5, ENABLE);
    //Дозволяєм роботу таймера системи захистів
    TIM_Cmd(TIM2, ENABLE);

    //Робота з watchdogs
    watchdog_routine(WATCHDOG_KYYBOARD, 19);

    /**********************/
    //Конфігуруємо I2C
    /**********************/
    Configure_I2C(I2C);
    /**********************/

    //Робота з watchdogs
    watchdog_routine(WATCHDOG_KYYBOARD, 20);

    //Виставляємо признак, що требаа прочитати всі регістри RTC, а потім, при потребі відкоректувати його поля
    //При цьому виставляємо біт блокування негайного запуску операції, щоб засинхронізуватися з роботою вимірювальної системи
    _SET_BIT(control_i2c_taskes, TASK_START_READ_RTC_BIT);
    _SET_BIT(control_i2c_taskes, TASK_BLK_OPERATION_BIT);

    //Обмін через I2C
    while (
      (control_i2c_taskes[0] != 0) ||
      (driver_i2c.state_execution > 0))
    {
      main_routines_for_i2c();

      //Робота з watchdogs
      watchdog_routine(WATCHDOG_KYYBOARD, 21);
    }
  }
  changing_diagnostyka_state(); //Підготовлюємо новий потенційно можливий запис для реєстратора програмних подій

  /**********************/
  //Завершальна частина запуску логіки приладу
  /**********************/
  //Робота з watchdogs
  watchdog_routine(WATCHDOG_KYYBOARD, 22);

  //Перевірка параметрування мікросхем DataFlash
  start_checking_dataflash();

  //Запускаємо генерацію переривань кожну кожну 1 мс від каналу 2 таймеру 4 для виконання періодичних низькопріоритетних задач
  start_tim4_canal2_for_interrupt_1mc();

  //  /***/
  //  {
  //    size_t col = 0;
  //    for(size_t i = 0; i < 0x10000; ++i)
  //    {
  //      _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD32_DD38) = ((1 << col) << LED_N_ROW) | ((uint32_t)(0) & ((1 << LED_N_ROW) - 1));
  //      if (++col >= LED_N_COL) col = 0;
  //      for (size_t j = 0; j < 100; ++j) watchdog_routine(WATCHDOG_KYYBOARD, 23);
  //    }
  //  }
  //  /***/
  //Ініціалізація FATFs
  MX_FATFS_Init();

  /**********************/

  /**********************/
  //Ініціалізація компонет Ігоря для Modbus + USB
  /**********************/
  watchdog_l2 = true;
  // global_component_installation();

  USBD_Init(&USB_OTG_dev,
#ifdef USE_USB_OTG_HS
            USB_OTG_HS_CORE_ID,
#else
            USB_OTG_FS_CORE_ID,
#endif
            &USR_desc,
            &USBD_CDC_cb,
            &USR_cb);
  watchdog_l2 = false;

  //Робота з watchdogs
  watchdog_routine(WATCHDOG_KYYBOARD, 25);
  /**********************/

  timeout_idle_new_settings = current_settings.timeout_idle_new_settings;
  //Визначаємо, чи стоїть дозвіл запису через інтерфейси з паролем
  if (current_settings.password_interface_RS485 == 0)
    password_set_RS485 = 0;
  else
    password_set_RS485 = 1;
  timeout_idle_RS485 = current_settings.timeout_deactivation_password_interface_RS485;

  if (current_settings.password_interface_USB == 0)
    password_set_USB = 0;
  else
    password_set_USB = 1;
  timeout_idle_USB = current_settings.timeout_deactivation_password_interface_USB;

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
  if (current_settings.password_interface_LAN == 0)
    password_set_LAN = 0;
  else
    password_set_LAN = 1;
  timeout_idle_LAN = current_settings.timeout_deactivation_password_interface_LAN;
#endif

  //Підраховуємо величину затримки у бітах, яка допускається між байтами у RS-485 згідно з визначеними настройками
  calculate_namber_bit_waiting_for_rs_485();
  //Запускаємо генерацію переривань з плаваючим періодом (час очікування наступного символа) від каналу 3 таймеру 4 для RS-485
  start_tim4_canal3_for_interrupt_10mkc();
  //Настроюємо USART на роботу з RS-485
  USART_RS485_Configure();
  //Починаємо моніторинг каналу RS-485
  restart_monitoring_RS485();
  /************************************************************/

  //Виставляємо признак, що на екрані треба обновити стартову інформацію
  new_state_keyboard |= (1u << BIT_REWRITE);

  //Робота з watchdogs
  watchdog_routine(WATCHDOG_KYYBOARD, 26);
  restart_resurs_count = 0xff; /*Ненульове значення перезапускає лічильники*/

  time_2_watchdog_input = time_2_watchdog_output = TIM4->CNT;
  restart_timing_watchdog = 0xff;

  /* Періодичні задачі */
  while (1)
  {
    //Немає активних операцій по Аналоговому реєстратору
    if (periodical_tasks_TEST_FLASH_MEMORY != 0)
    {
      /************************************************************/
      //Перевірка контрольної суми програми
      /************************************************************/
      unsigned short sum = 0;
      unsigned char *point = ((unsigned char *) &__checksum_begin);
      for (unsigned int i = ((unsigned int) &__checksum_end - (unsigned int) &__checksum_begin + 1); i > 0; i--)
      {
        sum += *point++;
        ar_routine_with_fatfs(false);
        watchdog_routine(UNITED_BITS_WATCHDOG, 27);
      }
      if (sum != (unsigned short) __checksum)
        _SET_BIT(set_diagnostyka, ERROR_INTERNAL_FLASH_BIT);
      else
        _SET_BIT(clear_diagnostyka, ERROR_INTERNAL_FLASH_BIT);
      /************************************************************/

      periodical_tasks_TEST_FLASH_MEMORY = false;
    }
    else
    {
      ar_routine_with_fatfs(false);
      watchdog_routine(UNITED_BITS_WATCHDOG, 28);
    }
  }
}
/*******************************************************************************/

/*******************************************************************************/
//Глобальна помилка програмного забеспечення
/*******************************************************************************/
void total_error_sw_fixed(void)
{
  while (1)
    ;
}
/*******************************************************************************/

#ifdef USE_FULL_ASSERT
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(u8 *file, u32 line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

#ifndef EXT_SRAM_512
#warning "Small Statistic Registrator"
#endif

#ifdef DEBUG_TEST
#warning "TEST VARIABLES IS PRESENT"
#endif

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
