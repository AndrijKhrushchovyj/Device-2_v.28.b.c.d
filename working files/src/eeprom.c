#include "header.h"

/*****************************************************/
//Управління обміном через SPI
/*****************************************************/
void main_routines_for_spi1(void)
{
  //Статична змінна, яка вказує який блок настройок треба записувати у EEPROM
  static unsigned int number_block_settings_write_to_eeprom;
  //Статична змінна, яка вказує який блок юстування треба записувати у EEPROM
  static unsigned int number_block_ustuvannja_write_to_eeprom;
  //Статична змінна, яка вказує який блок станів виходів і світлоіндикаторів треба записувати у EEPROM
  static unsigned int number_block_state_leds_outputs_write_to_eeprom;
  //Статична змінна, яка вказує який блок триґерної інформації треба записувати у EEPROM
  static unsigned int number_block_trg_func_write_to_eeprom;
  //Статична змінна, яка вказує який блок інформації по аналоговому реєстраторі треба записувати у EEPROM
  static unsigned int number_block_info_rejestrator_ar_write_to_eeprom;
  //Статична змінна, яка вказує який блок інформації по дискретному реєстраторі треба записувати у EEPROM
  static unsigned int number_block_info_rejestrator_dr_write_to_eeprom;
  //Статична змінна, яка вказує який блок інформації по реєстраторі програмних подій треба записувати у EEPROM
  static unsigned int number_block_info_rejestrator_pr_err_write_to_eeprom;
  //Статична змінна, яка вказує який блок інформації по ресурсу вимикача треба записувати у EEPROM
  //Статична змінна, яка вказує який блок кутів  треба записувати у EEPROM
  static unsigned int number_block_angle_write_to_eeprom;
  //Статична змінна, яка вказує який блок лічильника кутів треба записувати у EEPROM
  static unsigned int number_block_resurs_write_to_eeprom;

  //Статичні змінні для контролю коректності запису
  static __SETTINGS current_settings_comp;
  static unsigned int ustuvannja_comp[NUMBER_ANALOG_CANALES], serial_number_dev_comp;
  static unsigned int state_trigger_leds_comp, state_signal_outputs_comp;
  static unsigned int fix_active_buttons_comp, trigger_active_functions_comp[N_BIG];
  static __INFO_AR_REJESTRATOR info_rejestrator_ar_comp;
  static __INFO_REJESTRATOR info_rejestrator_dr_comp;
  static __INFO_REJESTRATOR info_rejestrator_pr_err_comp;
  static unsigned int sum_phi_begin_comp, sum_phi_end_comp;
  static __COUNTER_RESURS counter_today_comp, counter_previous_day_comp;
  static unsigned int counter_total_comp;

  if (state_execution_spi1 < 0)
  {
    /*************************************************/
    //Зараз можна ініціювати нову трансакцію через SPI
    /*************************************************/

    if (_CHECK_SET_BIT(control_spi1_taskes, TASK_EEPROM_WRITE_PREPARATION_BIT) != 0)
    {
      unsigned int number = 0;

      //Формуємо буфер для передачі у  EEPROM (біт запису + початкова адреса - інформаційні байти)
      if ((etap_eeprom_write_enable < 0) || ((etap_eeprom_write_enable > 1)))
        etap_eeprom_write_enable = 0;

      if (etap_eeprom_write_enable == 0)
      {
        //Читаємо регістр статусу
        TxBuffer_SPI_EDF[0] = OPCODE_RDSR;
        TxBuffer_SPI_EDF[1] = 0; //Будь-який байт для того, щоб здійснити читання регістру статусу;
        number = 2;
      }
      else if (etap_eeprom_write_enable == 1)
      {
        //Читаємо регістр статусу
        TxBuffer_SPI_EDF[0] = OPCODE_WREN;
        number = 1;
      }

      //Запускаємо процес запису в EEPROM
      start_exchange_via_spi(INDEX_EEPROM, number);
    }
    else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_WRITING_SETTINGS_EEPROM_BIT) != 0)
    {
      //Стоїть умова запису блоку у EEPROM настройок

      int size_to_end;
      unsigned int number, offset_from_start;

      //Визначаємо з якого місця треба почати записувати
      offset_from_start = number_block_settings_write_to_eeprom * SIZE_PAGE_EEPROM;

      //Кількість байт до кінця буферу
      size_to_end = (sizeof(__SETTINGS) + 2) - offset_from_start;

      if (size_to_end > 0)
      {
        TxBuffer_SPI_EDF[0] = OPCODE_WRITE;
        TxBuffer_SPI_EDF[1] = ((START_ADDRESS_SETTINGS_IN_EEPROM + offset_from_start) >> 8) & 0xff; //старша  адреса початку зберігання настройок у EEPROM
        TxBuffer_SPI_EDF[2] = ((START_ADDRESS_SETTINGS_IN_EEPROM + offset_from_start)) & 0xff;      //молодша адреса початку зберігання настройок у EEPROM

        if (size_to_end < SIZE_PAGE_EEPROM)
          number = size_to_end;
        else
          number = SIZE_PAGE_EEPROM;

        if (offset_from_start != 0)
        {
          //Переміщаємо дані для запису до опкоду з адресою початку запису для того, щоб сформувати цілий масив для передачі по DMA
          for (unsigned int i = 0; i < number; i++)
            TxBuffer_SPI_EDF[3 + i] = TxBuffer_SPI_EDF[3 + offset_from_start + i];
        }

        //Запускаємо процес запису в EEPROM
        start_exchange_via_spi(INDEX_EEPROM, (3 + number));
      }
      else
      {
        //Весь масив настройок вже записаний, тому скидаємо умову запису настройок у EEPROM

        //Виставляємо команду контрольного читання для перевідрки достовірності записаної інформації
        comparison_writing |= COMPARISON_WRITING_SETTINGS;
        _SET_BIT(control_spi1_taskes, TASK_START_READ_SETTINGS_EEPROM_BIT);

        //Скидаємо умову запису настройок у EEPROM
        _CLEAR_BIT(control_spi1_taskes, TASK_WRITING_SETTINGS_EEPROM_BIT);
      }
    }
    else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_WRITING_USTUVANNJA_EEPROM_BIT) != 0)
    {
      //Стоїть умова запису блоку у EEPROM юстування

      int size_to_end;
      unsigned int number, offset_from_start;

      //Визначаємо з якого місця треба почати записувати
      offset_from_start = number_block_ustuvannja_write_to_eeprom * SIZE_PAGE_EEPROM;

      //Кількість байт до кінця буферу
      size_to_end = (SIZE_USTUVANNJA + 1) - offset_from_start;

      if (size_to_end > 0)
      {
        TxBuffer_SPI_EDF[0] = OPCODE_WRITE;
        TxBuffer_SPI_EDF[1] = ((START_ADDRESS_USTUVANNJA_IN_EEPROM + offset_from_start) >> 8) & 0xff; //старша  адреса початку зберігання юстування і серійного номеру у EEPROM
        TxBuffer_SPI_EDF[2] = ((START_ADDRESS_USTUVANNJA_IN_EEPROM + offset_from_start)) & 0xff;      //молодша адреса початку зберігання юстування і серійного номеру у EEPROM

        if (size_to_end < SIZE_PAGE_EEPROM)
          number = size_to_end;
        else
          number = SIZE_PAGE_EEPROM;

        if (offset_from_start != 0)
        {
          //Переміщаємо дані для запису до опкоду з адресою початку запису для того, щоб сформувати цілий масив для передачі по DMA
          for (unsigned int i = 0; i < number; i++)
            TxBuffer_SPI_EDF[3 + i] = TxBuffer_SPI_EDF[3 + offset_from_start + i];
        }

        //Запускаємо процес запису в EEPROM
        start_exchange_via_spi(INDEX_EEPROM, (3 + number));
      }
      else
      {
        //Весь масив юстування вже записаний

        //Виставляємо команду контрольного читання для перевідрки достовірності записаної інформації
        comparison_writing |= COMPARISON_WRITING_USTUVANNJA;
        _SET_BIT(control_spi1_taskes, TASK_START_READ_USTUVANNJA_EEPROM_BIT);

        //Скидаємо умову запису юстування у EEPROM
        _CLEAR_BIT(control_spi1_taskes, TASK_WRITING_USTUVANNJA_EEPROM_BIT);
      }
    }
    else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_WRITING_STATE_LEDS_OUTPUTS_EEPROM_BIT) != 0)
    {
      //Стоїть умова запису блоку у EEPROM стану виходів і світлоіндикаторів
      int size_to_end;
      unsigned int number, offset_from_start;

      //Визначаємо з якого місця треба почати записувати
      offset_from_start = number_block_state_leds_outputs_write_to_eeprom * SIZE_PAGE_EEPROM;

      //Кількість байт до кінця буферу
      size_to_end = (2 * (3 + 3)) - offset_from_start;

      if (size_to_end > 0)
      {
        TxBuffer_SPI_EDF[0] = OPCODE_WRITE;
        TxBuffer_SPI_EDF[1] = ((START_ADDRESS_STATE_LEDS_OUTPUTS_IN_EEPROM + offset_from_start) >> 8) & 0xff; //старша  адреса початку зберігання стану виходів і світлоіндикаторів у EEPROM
        TxBuffer_SPI_EDF[2] = ((START_ADDRESS_STATE_LEDS_OUTPUTS_IN_EEPROM + offset_from_start)) & 0xff;      //молодша адреса початку зберігання стану виходів і світлоіндикаторів у EEPROM

        if (size_to_end < SIZE_PAGE_EEPROM)
          number = size_to_end;
        else
          number = SIZE_PAGE_EEPROM;

        if (offset_from_start != 0)
        {
          //Переміщаємо дані для запису до опкоду з адресою початку запису для того, щоб сформувати цілий масив для передачі по DMA
          for (unsigned int i = 0; i < number; i++)
            TxBuffer_SPI_EDF[3 + i] = TxBuffer_SPI_EDF[3 + offset_from_start + i];
        }

        //Запускаємо процес запису в EEPROM
        start_exchange_via_spi(INDEX_EEPROM, (3 + number));
      }
      else
      {
        //Весь масив стану виходів і світлоіндикаторів вже записаний, тому скидаємо умову запису стану виходів і світлоіндикаторів у EEPROM

        //Виставляємо команду контрольного читання для перевідрки достовірності записаної інформації
        comparison_writing |= COMPARISON_WRITING_STATE_LEDS_OUTPUTS;
        _SET_BIT(control_spi1_taskes, TASK_START_READ_STATE_LEDS_OUTPUTS_EEPROM_BIT);

        //Cкидаємо умову запису станів виходів-світлоіндикаторів у EEPROM
        _CLEAR_BIT(control_spi1_taskes, TASK_WRITING_STATE_LEDS_OUTPUTS_EEPROM_BIT);
      }
    }
    else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_WRITING_TRG_FUNC_EEPROM_BIT) != 0)
    {
      //Стоїть умова запису блоку триґерної інформації

      int size_to_end;
      unsigned int number, offset_from_start;

      //Визначаємо з якого місця треба почати записувати
      offset_from_start = number_block_trg_func_write_to_eeprom * SIZE_PAGE_EEPROM;

      //Кількість байт до кінця буферу
      size_to_end = (sizeof(fix_active_buttons) + sizeof(trigger_active_functions) + 1) - offset_from_start;

      if (size_to_end > 0)
      {
        TxBuffer_SPI_EDF[0] = OPCODE_WRITE;
        TxBuffer_SPI_EDF[1] = ((START_ADDRESS_TRG_FUNC + offset_from_start) >> 8) & 0xff; //старша  адреса початку зберігання стану триґерної інформації у EEPROM
        TxBuffer_SPI_EDF[2] = ((START_ADDRESS_TRG_FUNC + offset_from_start)) & 0xff;      //молодша адреса початку зберігання стану триґерної інформації у EEPROM

        if (size_to_end < SIZE_PAGE_EEPROM)
          number = size_to_end;
        else
          number = SIZE_PAGE_EEPROM;

        if (offset_from_start != 0)
        {
          //Переміщаємо дані для запису до опкоду з адресою початку запису для того, щоб сформувати цілий масив для передачі по DMA
          for (unsigned int i = 0; i < number; i++)
            TxBuffer_SPI_EDF[3 + i] = TxBuffer_SPI_EDF[3 + offset_from_start + i];
        }

        //Запускаємо процес запису в EEPROM
        start_exchange_via_spi(INDEX_EEPROM, (3 + number));
      }
      else
      {
        //Весь масив вже записаний

        //Виставляємо команду контрольного читання для перевідрки достовірності записаної інформації
        comparison_writing |= COMPARISON_WRITING_TRG_FUNC;
        _SET_BIT(control_spi1_taskes, TASK_START_READ_TRG_FUNC_EEPROM_BIT);

        //Cкидаємо умову записуу EEPROM
        _CLEAR_BIT(control_spi1_taskes, TASK_WRITING_TRG_FUNC_EEPROM_BIT);
      }
    }
    else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_WRITING_INFO_REJESTRATOR_AR_EEPROM_BIT) != 0)
    {
      //Стоїть умова запису блоку інформації аналогового реєстратора у EEPROM

      int size_to_end;
      unsigned int number, offset_from_start;

      //Визначаємо з якого місця треба почати записувати
      offset_from_start = number_block_info_rejestrator_ar_write_to_eeprom * SIZE_PAGE_EEPROM;

      //Кількість байт до кінця буферу
      size_to_end = (sizeof(info_rejestrator_ar) + 1) - offset_from_start;

      if (size_to_end > 0)
      {
        TxBuffer_SPI_EDF[0] = OPCODE_WRITE;
        TxBuffer_SPI_EDF[1] = ((START_ADDRESS_INFO_REJESTRATORS_AR + offset_from_start) >> 8) & 0xff; //старша  адреса початку зберігання структури дискретного реєстратора у EEPROM
        TxBuffer_SPI_EDF[2] = ((START_ADDRESS_INFO_REJESTRATORS_AR + offset_from_start)) & 0xff;      //молодша адреса початку зберігання структури дискретного реєстратора у EEPROM

        if (size_to_end < SIZE_PAGE_EEPROM)
          number = size_to_end;
        else
          number = SIZE_PAGE_EEPROM;

        if (offset_from_start != 0)
        {
          //Переміщаємо дані для запису до опкоду з адресою початку запису для того, щоб сформувати цілий масив для передачі по DMA
          for (unsigned int i = 0; i < number; i++)
            TxBuffer_SPI_EDF[3 + i] = TxBuffer_SPI_EDF[3 + offset_from_start + i];
        }

        //Запускаємо процес запису в EEPROM
        start_exchange_via_spi(INDEX_EEPROM, (3 + number));
      }
      else
      {
        //Весь масив вже записаний

        //Виставляємо команду контрольного читання для перевідрки достовірності записаної інформації
        comparison_writing |= COMPARISON_WRITING_INFO_REJESTRATOR_AR;
        _SET_BIT(control_spi1_taskes, TASK_START_READ_INFO_REJESTRATOR_AR_EEPROM_BIT);

        //Cкидаємо умову запису інформації аналогового реєстратора у EEPROM
        _CLEAR_BIT(control_spi1_taskes, TASK_WRITING_INFO_REJESTRATOR_AR_EEPROM_BIT);
      }
    }
    else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_WRITING_INFO_REJESTRATOR_DR_EEPROM_BIT) != 0)
    {
      //Стоїть умова запису блоку інформації дискретного реєстратора у EEPROM

      int size_to_end;
      unsigned int number, offset_from_start;

      //Визначаємо з якого місця треба почати записувати
      offset_from_start = number_block_info_rejestrator_dr_write_to_eeprom * SIZE_PAGE_EEPROM;

      //Кількість байт до кінця буферу
      size_to_end = (sizeof(info_rejestrator_dr) + 1) - offset_from_start;

      if (size_to_end > 0)
      {
        TxBuffer_SPI_EDF[0] = OPCODE_WRITE;
        TxBuffer_SPI_EDF[1] = ((START_ADDRESS_INFO_REJESTRATORS_DR + offset_from_start) >> 8) & 0xff; //старша  адреса початку зберігання структури дискретного реєстратора у EEPROM
        TxBuffer_SPI_EDF[2] = ((START_ADDRESS_INFO_REJESTRATORS_DR + offset_from_start)) & 0xff;      //молодша адреса початку зберігання структури дискретного реєстратора у EEPROM

        if (size_to_end < SIZE_PAGE_EEPROM)
          number = size_to_end;
        else
          number = SIZE_PAGE_EEPROM;

        if (offset_from_start != 0)
        {
          //Переміщаємо дані для запису до опкоду з адресою початку запису для того, щоб сформувати цілий масив для передачі по DMA
          for (unsigned int i = 0; i < number; i++)
            TxBuffer_SPI_EDF[3 + i] = TxBuffer_SPI_EDF[3 + offset_from_start + i];
        }

        //Запускаємо процес запису в EEPROM
        start_exchange_via_spi(INDEX_EEPROM, (3 + number));
      }
      else
      {
        //Весь масив вже записаний

        //Виставляємо команду контрольного читання для перевідрки достовірності записаної інформації
        comparison_writing |= COMPARISON_WRITING_INFO_REJESTRATOR_DR;
        _SET_BIT(control_spi1_taskes, TASK_START_READ_INFO_REJESTRATOR_DR_EEPROM_BIT);

        //Cкидаємо умову запису інформації дискретного реєстратора у EEPROM
        _CLEAR_BIT(control_spi1_taskes, TASK_WRITING_INFO_REJESTRATOR_DR_EEPROM_BIT);
      }
    }
    else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_WRITING_INFO_REJESTRATOR_PR_ERR_EEPROM_BIT) != 0)
    {
      //Стоїть умова запису блоку інформації реєстратора програмних подій у EEPROM

      int size_to_end;
      unsigned int number, offset_from_start;

      //Визначаємо з якого місця треба почати записувати
      offset_from_start = number_block_info_rejestrator_pr_err_write_to_eeprom * SIZE_PAGE_EEPROM;

      //Кількість байт до кінця буферу
      size_to_end = (sizeof(info_rejestrator_pr_err) + 1) - offset_from_start;

      if (size_to_end > 0)
      {
        TxBuffer_SPI_EDF[0] = OPCODE_WRITE;
        TxBuffer_SPI_EDF[1] = ((START_ADDRESS_INFO_REJESTRATORS_PR_ERR + offset_from_start) >> 8) & 0xff; //старша  адреса початку зберігання структури реєстратора програмних подій у EEPROM
        TxBuffer_SPI_EDF[2] = ((START_ADDRESS_INFO_REJESTRATORS_PR_ERR + offset_from_start)) & 0xff;      //молодша адреса початку зберігання структури реєстратора програмних подій у EEPROM

        if (size_to_end < SIZE_PAGE_EEPROM)
          number = size_to_end;
        else
          number = SIZE_PAGE_EEPROM;

        if (offset_from_start != 0)
        {
          //Переміщаємо дані для запису до опкоду з адресою початку запису для того, щоб сформувати цілий масив для передачі по DMA
          for (unsigned int i = 0; i < number; i++)
            TxBuffer_SPI_EDF[3 + i] = TxBuffer_SPI_EDF[3 + offset_from_start + i];
        }

        //Запускаємо процес запису в EEPROM
        start_exchange_via_spi(INDEX_EEPROM, (3 + number));
      }
      else
      {
        //Весь масив вже записаний

        //Виставляємо команду контрольного читання для перевідрки достовірності записаної інформації
        comparison_writing |= COMPARISON_WRITING_INFO_REJESTRATOR_PR_ERR;
        _SET_BIT(control_spi1_taskes, TASK_START_READ_INFO_REJESTRATOR_PR_ERR_EEPROM_BIT);

        //Cкидаємо умову запису інформації реєстратора програмних подій у EEPROM
        _CLEAR_BIT(control_spi1_taskes, TASK_WRITING_INFO_REJESTRATOR_PR_ERR_EEPROM_BIT);
      }
    }
    else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_WRITING_ANGLE_EEPROM_BIT) != 0)
    {
      //Стоїть умова запису блоку у EEPROM кутів

      int size_to_end;
      unsigned int number, offset_from_start;

      //Визначаємо з якого місця треба почати записувати
      offset_from_start = number_block_angle_write_to_eeprom * SIZE_PAGE_EEPROM;

      //Кількість байт до кінця буферу
      size_to_end = (2 * sizeof(unsigned int) + 1) - offset_from_start;

      if (size_to_end > 0)
      {
        TxBuffer_SPI_EDF[0] = OPCODE_WRITE;
        TxBuffer_SPI_EDF[1] = ((START_ADDRESS_ANGLE_IN_EEPROM + offset_from_start) >> 8) & 0xff; //старша  адреса початку зберігання кутів у EEPROM
        TxBuffer_SPI_EDF[2] = ((START_ADDRESS_ANGLE_IN_EEPROM + offset_from_start)) & 0xff;      //молодша адреса початку зберігання кутів у EEPROM

        if (size_to_end < SIZE_PAGE_EEPROM)
          number = size_to_end;
        else
          number = SIZE_PAGE_EEPROM;

        if (offset_from_start != 0)
        {
          //Переміщаємо дані для запису до опкоду з адресою початку запису для того, щоб сформувати цілий масив для передачі по DMA
          for (unsigned int i = 0; i < number; i++)
            TxBuffer_SPI_EDF[3 + i] = TxBuffer_SPI_EDF[3 + offset_from_start + i];
        }

        //Запускаємо процес запису в EEPROM
        start_exchange_via_spi(INDEX_EEPROM, (3 + number));
      }
      else
      {
        //Весь масив кутів вже записаний

        //Виставляємо команду контрольного читання для перевідрки достовірності записаної інформації
        comparison_writing |= COMPARISON_WRITING_ANGLE;
        _SET_BIT(control_spi1_taskes, TASK_START_READ_ANGLE_EEPROM_BIT);

        //Скидаємо умову запису кутів у EEPROM
        _CLEAR_BIT(control_spi1_taskes, TASK_WRITING_ANGLE_EEPROM_BIT);
      }
    }
    else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_WRITING_RESURS_EEPROM_BIT) != 0)
    {
      //Стоїть умова запису блоку у EEPROM ресурсу лічильника

      int size_to_end;
      unsigned int number, offset_from_start;

      //Визначаємо з якого місця треба почати записувати
      offset_from_start = number_block_resurs_write_to_eeprom * SIZE_PAGE_EEPROM;

      //Кількість байт до кінця буферу
      size_to_end = (2 * sizeof(__COUNTER_RESURS) + sizeof(unsigned int) + 1) - offset_from_start;

      if (size_to_end > 0)
      {
        TxBuffer_SPI_EDF[0] = OPCODE_WRITE;
        TxBuffer_SPI_EDF[1] = ((START_ADDRESS_RESURS_IN_EEPROM + offset_from_start) >> 8) & 0xff; //старша  адреса початку зберігання лічильників ресурсу у EEPROM
        TxBuffer_SPI_EDF[2] = ((START_ADDRESS_RESURS_IN_EEPROM + offset_from_start)) & 0xff;      //молодша адреса початку зберігання лічильників ресурсу у EEPROM

        if (size_to_end < SIZE_PAGE_EEPROM)
          number = size_to_end;
        else
          number = SIZE_PAGE_EEPROM;

        if (offset_from_start != 0)
        {
          //Переміщаємо дані для запису до опкоду з адресою початку запису для того, щоб сформувати цілий масив для передачі по DMA
          for (unsigned int i = 0; i < number; i++)
            TxBuffer_SPI_EDF[3 + i] = TxBuffer_SPI_EDF[3 + offset_from_start + i];
        }

        //Запускаємо процес запису в EEPROM
        start_exchange_via_spi(INDEX_EEPROM, (3 + number));
      }
      else
      {
        //Весь масив лічильника ресурсу вже записаний

        //Виставляємо команду контрольного читання для перевідрки достовірності записаної інформації
        comparison_writing |= COMPARISON_WRITING_RESURS;
        _SET_BIT(control_spi1_taskes, TASK_START_READ_RESURS_EEPROM_BIT);

        //Скидаємо умову запису кутів у EEPROM
        _CLEAR_BIT(control_spi1_taskes, TASK_WRITING_RESURS_EEPROM_BIT);
      }
    }
    else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_READING_SETTINGS_EEPROM_BIT) != 0)
    {
      //Запускаємо процес читання настройок - формуємо буфер для передавання в EEPROM щоб прочитати настройки
      TxBuffer_SPI_EDF[0] = OPCODE_READ;
      TxBuffer_SPI_EDF[1] = (START_ADDRESS_SETTINGS_IN_EEPROM >> 8) & 0xff; //старша  адреса початку зберігання настройок у EEPROM
      TxBuffer_SPI_EDF[2] = (START_ADDRESS_SETTINGS_IN_EEPROM) &0xff;       //молодша адреса початку зберігання настройок у EEPROM
                                                                            //дальше значення байт не має значення
      start_exchange_via_spi(INDEX_EEPROM, ((sizeof(__SETTINGS) + 2) + 3));
    }
    else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_READING_USTUVANNJA_EEPROM_BIT) != 0)
    {
      //Запускаємо процес читання юстування - формуємо буфер для передавання в EEPROM щоб прочитати юстування і серійний номер
      TxBuffer_SPI_EDF[0] = OPCODE_READ;
      TxBuffer_SPI_EDF[1] = (START_ADDRESS_USTUVANNJA_IN_EEPROM >> 8) & 0xff; //старша  адреса початку зберігання юстування і серійного номеру у EEPROM
      TxBuffer_SPI_EDF[2] = (START_ADDRESS_USTUVANNJA_IN_EEPROM) &0xff;       //молодша адреса початку зберігання юстування і серійного номеру у EEPROM
                                                                              //дальше значення байт не має значення
      start_exchange_via_spi(INDEX_EEPROM, ((SIZE_USTUVANNJA + 1) + 3));
    }
    else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_READING_STATE_LEDS_OUTPUTS_EEPROM_BIT) != 0)
    {
      //Запускаємо процес читання  даних про сигнальні виходи і тригерні свтлодіоди - формуємо буфер для передавання в EEPROM щоб прочитати  дані про сигнальні виходи і тригерні свтлодіоди
      TxBuffer_SPI_EDF[0] = OPCODE_READ;
      TxBuffer_SPI_EDF[1] = (START_ADDRESS_STATE_LEDS_OUTPUTS_IN_EEPROM >> 8) & 0xff; //старша  адреса початку зберігання даних про сигнальні виходи і тригерні свтлодіоди у EEPROM
      TxBuffer_SPI_EDF[2] = (START_ADDRESS_STATE_LEDS_OUTPUTS_IN_EEPROM) &0xff;       //молодша адреса початку зберігання даних про сигнальні виходи і тригерні свтлодіоди у EEPROM
                                                                                      //дальше значення байт не має значення
      start_exchange_via_spi(INDEX_EEPROM, ((2 * (3 + 3)) + 3));
    }
    else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_READING_TRG_FUNC_EEPROM_BIT) != 0)
    {
      //Запускаємо процес читання  даних про триґерній функції - формуємо буфер для передавання в EEPROM щоб прочитати  дані
      TxBuffer_SPI_EDF[0] = OPCODE_READ;
      TxBuffer_SPI_EDF[1] = (START_ADDRESS_TRG_FUNC >> 8) & 0xff; //старша  адреса початку зберігання даних про триґерну функцію у EEPROM
      TxBuffer_SPI_EDF[2] = (START_ADDRESS_TRG_FUNC) &0xff;       //молодша адреса початку зберігання даних про триґерну функцію у EEPROM
                                                                  //дальше значення байт не має значення
      start_exchange_via_spi(INDEX_EEPROM, ((sizeof(fix_active_buttons) + sizeof(trigger_active_functions) + 1) + 3));
    }
    else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_READING_INFO_REJESTRATOR_AR_EEPROM_BIT) != 0)
    {
      //Запускаємо процес читання інформації по дискретному реєстратору - формуємо буфер для передавання в EEPROM щоб прочитати  дані по аналоговому реєстратору
      TxBuffer_SPI_EDF[0] = OPCODE_READ;
      TxBuffer_SPI_EDF[1] = (START_ADDRESS_INFO_REJESTRATORS_AR >> 8) & 0xff; //старша  адреса початку зберігання даних по аналоговому реєстратору у EEPROM
      TxBuffer_SPI_EDF[2] = (START_ADDRESS_INFO_REJESTRATORS_AR) &0xff;       //молодша адреса початку зберігання даних по аналоговому реєстратору у EEPROM
                                                                              //дальше значення байт не має значення
      start_exchange_via_spi(INDEX_EEPROM, ((sizeof(info_rejestrator_ar) + 1) + 3));
    }
    else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_READING_INFO_REJESTRATOR_DR_EEPROM_BIT) != 0)
    {
      //Запускаємо процес читання інформації по дискретному реєстратору - формуємо буфер для передавання в EEPROM щоб прочитати  дані по дискретному реєстратору
      TxBuffer_SPI_EDF[0] = OPCODE_READ;
      TxBuffer_SPI_EDF[1] = (START_ADDRESS_INFO_REJESTRATORS_DR >> 8) & 0xff; //старша  адреса початку зберігання даних по дискретному реєстратору у EEPROM
      TxBuffer_SPI_EDF[2] = (START_ADDRESS_INFO_REJESTRATORS_DR) &0xff;       //молодша адреса початку зберігання даних по дискретному реєстратору у EEPROM
                                                                              //дальше значення байт не має значення
      start_exchange_via_spi(INDEX_EEPROM, ((sizeof(info_rejestrator_dr) + 1) + 3));
    }
    else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_READING_INFO_REJESTRATOR_PR_ERR_EEPROM_BIT) != 0)
    {
      //Запускаємо процес читання інформації по реєстратору програмних подій - формуємо буфер для передавання в EEPROM щоб прочитати  дані по реєстратору програмних подій
      TxBuffer_SPI_EDF[0] = OPCODE_READ;
      TxBuffer_SPI_EDF[1] = (START_ADDRESS_INFO_REJESTRATORS_PR_ERR >> 8) & 0xff; //старша  адреса початку зберігання даних по реєстратору програмних подій у EEPROM
      TxBuffer_SPI_EDF[2] = (START_ADDRESS_INFO_REJESTRATORS_PR_ERR) &0xff;       //молодша адреса початку зберігання даних по реєстратору програмних подій у EEPROM
                                                                                  //дальше значення байт не має значення
      start_exchange_via_spi(INDEX_EEPROM, ((sizeof(info_rejestrator_pr_err) + 1) + 3));
    }
    else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_READING_ANGLE_EEPROM_BIT) != 0)
    {

      TxBuffer_SPI_EDF[0] = OPCODE_READ;
      TxBuffer_SPI_EDF[1] = (START_ADDRESS_ANGLE_IN_EEPROM >> 8) & 0xff; //старша  адреса початку зберігання даних  у EEPROM
      TxBuffer_SPI_EDF[2] = (START_ADDRESS_ANGLE_IN_EEPROM) &0xff;       //молодша адреса початку зберігання даних  у EEPROM
                                                                         //дальше значення байт не має значення
      start_exchange_via_spi(INDEX_EEPROM, ((2 * sizeof(unsigned int) + 1) + 3));
    }
    else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_READING_RESURS_EEPROM_BIT) != 0)
    {

      TxBuffer_SPI_EDF[0] = OPCODE_READ;
      TxBuffer_SPI_EDF[1] = (START_ADDRESS_RESURS_IN_EEPROM >> 8) & 0xff; //старша  адреса початку зберігання даних  у EEPROM
      TxBuffer_SPI_EDF[2] = (START_ADDRESS_RESURS_IN_EEPROM) &0xff;       //молодша адреса початку зберігання даних  у EEPROM
                                                                          //дальше значення байт не має значення
      start_exchange_via_spi(INDEX_EEPROM, ((2 * sizeof(__COUNTER_RESURS) + sizeof(unsigned int) + 1) + 3));
    }
    else if (
      (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_READ_SETTINGS_EEPROM_BIT) != 0) ||
      (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_READ_USTUVANNJA_EEPROM_BIT) != 0) ||
      (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_READ_STATE_LEDS_OUTPUTS_EEPROM_BIT) != 0) ||
      (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_READ_TRG_FUNC_EEPROM_BIT) != 0) ||
      (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_READ_INFO_REJESTRATOR_AR_EEPROM_BIT) != 0) ||
      (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_READ_INFO_REJESTRATOR_DR_EEPROM_BIT) != 0) ||
      (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_READ_INFO_REJESTRATOR_PR_ERR_EEPROM_BIT) != 0) ||
      (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_READ_ANGLE_EEPROM_BIT) != 0) ||
      (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_READ_RESURS_EEPROM_BIT) != 0))
    {
      //Запускаємо процес читання

      //Читаємо регістр статусу тому, що не можна читати EEPROM поки іде процес запису
      TxBuffer_SPI_EDF[0] = OPCODE_RDSR;
      TxBuffer_SPI_EDF[1] = 0; //Будь-який байт для того, щоб здійснити читання регістру статусу;

      //Запускаємо процес запису в EEPROM
      start_exchange_via_spi(INDEX_EEPROM, 2);
    }
    else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_WRITE_SETTINGS_EEPROM_BIT) != 0)
    {
      //Стоїть умова початку нового запису у EEPROM настройок

      //Скидаємо біт запуску нового запису і виставляємо біт запису блоків у EEPROM з бітом встановлення дозволу на запис
      _SET_BIT(control_spi1_taskes, TASK_EEPROM_WRITE_PREPARATION_BIT);
      _SET_BIT(control_spi1_taskes, TASK_WRITING_SETTINGS_EEPROM_BIT);
      _CLEAR_BIT(control_spi1_taskes, TASK_START_WRITE_SETTINGS_EEPROM_BIT);
      /***/

      //Робимо копію записуваної інформації для контролю

      //Готуємо буфер для запису настройок у EEPROM разом з контрольною сумою
      unsigned char crc_eeprom_settings = 0, temp_value;
      unsigned char *point_1 = (unsigned char *) (&current_settings);
      unsigned char *point_2 = (unsigned char *) (&current_settings_comp);
      for (unsigned int i = 0; i < sizeof(__SETTINGS); i++)
      {
        temp_value = *(point_1);
        *(point_2) = temp_value;
        point_1++;
        point_2++;
        TxBuffer_SPI_EDF[3 + i] = temp_value;
        crc_eeprom_settings += temp_value;
      }
      //Добавляємо інвертовану контрольну суму у кінець масиву
      TxBuffer_SPI_EDF[3 + sizeof(__SETTINGS)] = (unsigned char) ((~(unsigned int) crc_eeprom_settings) & 0xff); /*  інвертована контрольна сума*/
      TxBuffer_SPI_EDF[3 + sizeof(__SETTINGS) + 1] = (unsigned char) (crc_eeprom_settings & 0xff);               /*неінвертована контрольна сума*/

      //Виставляємо перший блок настройок запису у EEPROM
      number_block_settings_write_to_eeprom = 0;
    }
    else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_WRITE_USTUVANNJA_EEPROM_BIT) != 0)
    {
      //Стоїть умова початку нового запису у EEPROM юстування

      //Скидаємо біт запуску нового запису і виставляємо біт запису блоків у EEPROM з бітом встановлення дозволу на запис
      _SET_BIT(control_spi1_taskes, TASK_EEPROM_WRITE_PREPARATION_BIT);
      _SET_BIT(control_spi1_taskes, TASK_WRITING_USTUVANNJA_EEPROM_BIT);
      _CLEAR_BIT(control_spi1_taskes, TASK_START_WRITE_USTUVANNJA_EEPROM_BIT);

      //Робимо копію записуваної інформації для контролю

      //Готуємо буфер для запису настройок у EEPROM разом з контрольноюсумою
      unsigned char crc_eeprom_ustuvannja = 0, temp_value;
      unsigned char *point_1;
      unsigned char *point_2;
      unsigned int offset = 3;

      //Додаємо ідентифікатор типу нобору юстуючих коефіцієнтів
      unsigned int adjustment_id_tmp = ADJUSTMENT_ID;
      point_1 = (unsigned char *) (&adjustment_id_tmp);
      for (unsigned int i = 0; i < sizeof(unsigned int); i++)
      {
        temp_value = *(point_1++);
        TxBuffer_SPI_EDF[offset + i] = temp_value;
        crc_eeprom_ustuvannja += temp_value;
      }
      offset += sizeof(unsigned int);

      //Додаємо юстуючі коефіцієнти
      point_1 = (unsigned char *) (&ustuvannja);
      point_2 = (unsigned char *) (&ustuvannja_comp);
      for (unsigned int i = 0; i < sizeof(ustuvannja); i++)
      {
        temp_value = *(point_1);
        *(point_2) = temp_value;
        point_1++;
        point_2++;
        TxBuffer_SPI_EDF[offset + i] = temp_value;
        crc_eeprom_ustuvannja += temp_value;
      }
      offset += sizeof(ustuvannja);

      //Додаємо ще серійний номер пристрою
      point_1 = (unsigned char *) (&serial_number_dev);
      point_2 = (unsigned char *) (&serial_number_dev_comp);
      for (unsigned int i = 0; i < sizeof(serial_number_dev); i++)
      {
        temp_value = *(point_1);
        *(point_2) = temp_value;
        point_1++;
        point_2++;
        TxBuffer_SPI_EDF[offset + i] = temp_value;
        crc_eeprom_ustuvannja += temp_value;
      }
      offset += sizeof(serial_number_dev);

      //Добавляємо інвертовану контрольну суму у кінець масиву
      TxBuffer_SPI_EDF[offset] = (unsigned char) ((~(unsigned int) crc_eeprom_ustuvannja) & 0xff);

      //Виставляємо перший блок юстування запису у EEPROM
      number_block_ustuvannja_write_to_eeprom = 0;
    }
    else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_WRITE_STATE_LEDS_OUTPUTS_EEPROM_BIT) != 0)
    {
      //Стоїть умова початку нового запису у EEPROM стану сигнальних виходів і тригерних світлоігдикаторів

      //Скидаємо біт запуску нового запису і виставляємо біт запису блоків у EEPROM з бітом встановлення дозволу на запис
      _SET_BIT(control_spi1_taskes, TASK_EEPROM_WRITE_PREPARATION_BIT);
      _SET_BIT(control_spi1_taskes, TASK_WRITING_STATE_LEDS_OUTPUTS_EEPROM_BIT);
      _CLEAR_BIT(control_spi1_taskes, TASK_START_WRITE_STATE_LEDS_OUTPUTS_EEPROM_BIT);

      //Готуємо буфер для запису
      unsigned int temp_value = state_trigger_leds;
      unsigned int temp_value_inv = ((unsigned int) (~temp_value)) & ((1 << NUMBER_LEDS) - 1);
      state_trigger_leds_comp = temp_value;
      TxBuffer_SPI_EDF[3 + 0] = (unsigned char) (temp_value & 0xff);
      TxBuffer_SPI_EDF[3 + 1] = (unsigned char) ((temp_value >> 8) & 0xff);
      TxBuffer_SPI_EDF[3 + 2] = (unsigned char) ((temp_value >> 16) & 0xff);
      TxBuffer_SPI_EDF[3 + 3] = (unsigned char) (temp_value_inv & 0xff);
      TxBuffer_SPI_EDF[3 + 4] = (unsigned char) ((temp_value_inv >> 8) & 0xff);
      TxBuffer_SPI_EDF[3 + 5] = (unsigned char) ((temp_value_inv >> 16) & 0xff);

      temp_value = state_signal_outputs;
      temp_value_inv = ((unsigned int) (~temp_value)) & ((1 << NUMBER_SIMPLE_OUTPUTS) - 1);
      state_signal_outputs_comp = temp_value;
      TxBuffer_SPI_EDF[3 + 6] = (unsigned char) (temp_value & 0xff);
      TxBuffer_SPI_EDF[3 + 7] = (unsigned char) ((temp_value >> 8) & 0xff);
      TxBuffer_SPI_EDF[3 + 8] = (unsigned char) ((temp_value >> 16) & 0xff);
      TxBuffer_SPI_EDF[3 + 9] = (unsigned char) (temp_value_inv & 0xff);
      TxBuffer_SPI_EDF[3 + 10] = (unsigned char) ((temp_value_inv >> 8) & 0xff);
      TxBuffer_SPI_EDF[3 + 11] = (unsigned char) ((temp_value_inv >> 16) & 0xff);

      //Виставляємо перший блок стану виходів-світлоіндикаторів запису у EEPROM
      number_block_state_leds_outputs_write_to_eeprom = 0;
    }
    else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_WRITE_TRG_FUNC_EEPROM_BIT) != 0)
    {
      //Стоїть умова початку нового запису у EEPROM по триґерній інформації

      //Скидаємо активну задачу самоконтролю по резервній копії для триґерної інформації, так як зараз ми будемо поля змінювати. які могли бути підготовлені для контролю
      periodical_tasks_TEST_TRG_FUNC_LOCK = false;

      //Скидаємо біт запуску нового запису і виставляємо біт запису блоків у EEPROM з бітом встановлення дозволу на запис
      _SET_BIT(control_spi1_taskes, TASK_EEPROM_WRITE_PREPARATION_BIT);
      _SET_BIT(control_spi1_taskes, TASK_WRITING_TRG_FUNC_EEPROM_BIT);
      _CLEAR_BIT(control_spi1_taskes, TASK_START_WRITE_TRG_FUNC_EEPROM_BIT);

      //Готуємо буфер для запису у EEPROM разом з контрольноюсумою
      unsigned char crc_eeprom_trg_func = 0, temp_value;
      unsigned char *point_1;
      unsigned char *point_2;
      unsigned int offset = 3;

      //Додаємо інформацію по ФК, які працюють у режимі ключа
      point_1 = (unsigned char *) (&fix_active_buttons);
      point_2 = (unsigned char *) (&fix_active_buttons_comp);
      for (unsigned int i = 0; i < sizeof(fix_active_buttons); i++)
      {
        temp_value = *(point_1);
        *(point_2) = temp_value;
        point_1++;
        point_2++;
        TxBuffer_SPI_EDF[offset + i] = temp_value;
        crc_eeprom_trg_func += temp_value;
      }
      offset += sizeof(fix_active_buttons);

      //Додаємо триґерні функції
      point_1 = (unsigned char *) (&trigger_active_functions);
      point_2 = (unsigned char *) (&trigger_active_functions_comp);
      for (unsigned int i = 0; i < sizeof(trigger_active_functions); i++)
      {
        temp_value = *(point_1);
        *(point_2) = temp_value;
        point_1++;
        point_2++;
        TxBuffer_SPI_EDF[offset + i] = temp_value;
        crc_eeprom_trg_func += temp_value;
      }
      offset += sizeof(trigger_active_functions);

      //Добавляємо інвертовану контрольну суму у кінець масиву
      TxBuffer_SPI_EDF[offset] = (unsigned char) ((~(unsigned int) crc_eeprom_trg_func) & 0xff);

      //Виставляємо перший блок запису у EEPROM
      number_block_trg_func_write_to_eeprom = 0;
    }
    else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_WRITE_INFO_REJESTRATOR_AR_EEPROM_BIT) != 0)
    {
      //Стоїть умова початку нового запису у EEPROM по інформації аналогового реєстратора

      //Скидаємо активну задачу самоконтролю по резервній копії, так як зараз ми будемо поля змінювати. які могли бути підготовлені для контролю
      periodical_tasks_TEST_INFO_REJESTRATOR_AR_LOCK = false;

      //Скидаємо біт запуску нового запису і виставляємо біт запису блоків у EEPROM з блокуванням, щоб запуск почався з синхронізацією
      _SET_BIT(control_spi1_taskes, TASK_EEPROM_WRITE_PREPARATION_BIT);
      _SET_BIT(control_spi1_taskes, TASK_WRITING_INFO_REJESTRATOR_AR_EEPROM_BIT);
      _CLEAR_BIT(control_spi1_taskes, TASK_START_WRITE_INFO_REJESTRATOR_AR_EEPROM_BIT);

      //Робимо копію записуваної інформації для контролю

      //Готуємо буфер для запису у EEPROM разом з контрольною сумою
      unsigned char crc_eeprom_info_rejestrator_ar = 0, temp_value;
      unsigned char *point_1 = (unsigned char *) (&info_rejestrator_ar);
      unsigned char *point_2 = (unsigned char *) (&info_rejestrator_ar_comp);
      for (unsigned int i = 0; i < sizeof(__INFO_AR_REJESTRATOR); i++)
      {
        temp_value = *(point_1);
        *(point_2) = temp_value;
        point_1++;
        point_2++;
        TxBuffer_SPI_EDF[3 + i] = temp_value;
        crc_eeprom_info_rejestrator_ar += temp_value;
      }

      //Добавляємо інвертовану контрольну суму у кінець масиву
      TxBuffer_SPI_EDF[3 + sizeof(info_rejestrator_ar)] = (unsigned char) ((~(unsigned int) crc_eeprom_info_rejestrator_ar) & 0xff);

      //Виставляємо перший блок запису у EEPROM
      number_block_info_rejestrator_ar_write_to_eeprom = 0;
    }
    else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_WRITE_INFO_REJESTRATOR_DR_EEPROM_BIT) != 0)
    {
      //Стоїть умова початку нового запису у EEPROM по інформації дискретного реєстратора

      //Скидаємо активну задачу самоконтролю по резервній копії, так як зараз ми будемо поля змінювати. які могли бути підготовлені для контролю
      periodical_tasks_TEST_INFO_REJESTRATOR_DR_LOCK = false;

      //Скидаємо біт запуску нового запису і виставляємо біт запису блоків у EEPROM з бітом встановлення дозволу на запис
      _SET_BIT(control_spi1_taskes, TASK_EEPROM_WRITE_PREPARATION_BIT);
      _SET_BIT(control_spi1_taskes, TASK_WRITING_INFO_REJESTRATOR_DR_EEPROM_BIT);
      _CLEAR_BIT(control_spi1_taskes, TASK_START_WRITE_INFO_REJESTRATOR_DR_EEPROM_BIT);

      //Готуємо буфер для запису у EEPROM разом з контрольною сумою
      unsigned char crc_eeprom_info_rejestrator_dr = 0, temp_value;
      unsigned char *point_1 = (unsigned char *) (&info_rejestrator_dr);
      unsigned char *point_2 = (unsigned char *) (&info_rejestrator_dr_comp);
      for (unsigned int i = 0; i < sizeof(__INFO_REJESTRATOR); i++)
      {
        temp_value = *(point_1);
        *(point_2) = temp_value;
        point_1++;
        point_2++;
        TxBuffer_SPI_EDF[3 + i] = temp_value;
        crc_eeprom_info_rejestrator_dr += temp_value;
      }

      //Добавляємо інвертовану контрольну суму у кінець масиву
      TxBuffer_SPI_EDF[3 + sizeof(info_rejestrator_dr)] = (unsigned char) ((~(unsigned int) crc_eeprom_info_rejestrator_dr) & 0xff);

      //Виставляємо перший блок запису у EEPROM
      number_block_info_rejestrator_dr_write_to_eeprom = 0;
    }
    else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_WRITE_INFO_REJESTRATOR_PR_ERR_EEPROM_BIT) != 0)
    {
      //Стоїть умова початку нового запису у EEPROM по інформації реєстратора програмних подій

      //Скидаємо активну задачу самоконтролю по резервній копії, так як зараз ми будемо поля змінювати. які могли бути підготовлені для контролю
      periodical_tasks_TEST_INFO_REJESTRATOR_PR_ERR_LOCK = false;

      //Скидаємо біт запуску нового запису і виставляємо біт запису блоків у EEPROM з бітом встановлення дозволу на запис
      _SET_BIT(control_spi1_taskes, TASK_EEPROM_WRITE_PREPARATION_BIT);
      _SET_BIT(control_spi1_taskes, TASK_WRITING_INFO_REJESTRATOR_PR_ERR_EEPROM_BIT);
      _CLEAR_BIT(control_spi1_taskes, TASK_START_WRITE_INFO_REJESTRATOR_PR_ERR_EEPROM_BIT);

      //Готуємо буфер для запису у EEPROM разом з контрольною сумою
      unsigned char crc_eeprom_info_rejestrator_pr_err = 0, temp_value;
      unsigned char *point_1 = (unsigned char *) (&info_rejestrator_pr_err);
      unsigned char *point_2 = (unsigned char *) (&info_rejestrator_pr_err_comp);
      for (unsigned int i = 0; i < sizeof(__INFO_REJESTRATOR); i++)
      {
        temp_value = *(point_1);
        *(point_2) = temp_value;
        point_1++;
        point_2++;
        TxBuffer_SPI_EDF[3 + i] = temp_value;
        crc_eeprom_info_rejestrator_pr_err += temp_value;
      }

      //Добавляємо інвертовану контрольну суму у кінець масиву
      TxBuffer_SPI_EDF[3 + sizeof(info_rejestrator_pr_err)] = (unsigned char) ((~(unsigned int) crc_eeprom_info_rejestrator_pr_err) & 0xff);

      //Виставляємо перший блок запису у EEPROM
      number_block_info_rejestrator_pr_err_write_to_eeprom = 0;
    }
    else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_WRITE_ANGLE_EEPROM_BIT) != 0)
    {

      //Стоїть умова початку нового запису у EEPROM кутів для сельсинового контролю

      //Готуємо буфер для запису
      unsigned char crc_eeprom_angle = 0, temp_value;
      unsigned char *point;
      unsigned int offset = 3;

      sum_phi_begin_comp = sum_phi_begin;
      point = (unsigned char *) (&sum_phi_begin);
      for (unsigned int i = 0; i < sizeof(sum_phi_begin); i++)
      {
        temp_value = *(point++);
        TxBuffer_SPI_EDF[offset + i] = temp_value;
        crc_eeprom_angle += temp_value;
      }
      offset += sizeof(sum_phi_begin);

      sum_phi_end_comp = sum_phi_end;
      point = (unsigned char *) (&sum_phi_end);
      for (unsigned int i = 0; i < sizeof(sum_phi_end); i++)
      {
        temp_value = *(point++);
        TxBuffer_SPI_EDF[offset + i] = temp_value;
        crc_eeprom_angle += temp_value;
      }
      offset += sizeof(sum_phi_end);

      TxBuffer_SPI_EDF[offset] = (unsigned char) ((~(unsigned int) crc_eeprom_angle) & 0xff);

      //Скидаємо біт запуску нового запису і виставляємо біт запису блоків у EEPROM з бітом встановлення дозволу на запис
      _SET_BIT(control_spi1_taskes, TASK_EEPROM_WRITE_PREPARATION_BIT);
      _SET_BIT(control_spi1_taskes, TASK_WRITING_ANGLE_EEPROM_BIT);
      _CLEAR_BIT(control_spi1_taskes, TASK_START_WRITE_ANGLE_EEPROM_BIT);

      //Виставляємо перший блок стану виходів-світлоіндикаторів запису у EEPROM
      number_block_angle_write_to_eeprom = 0;
    }
    else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_WRITE_RESURS_EEPROM_BIT) != 0)
    {
      //Стоїть умова початку нового запису у EEPROM кількості переключень для лічильника ресурсу

      //Готуємо буфер для запису
      unsigned char crc_eeprom_resurs = 0, temp_value;
      unsigned char *point;
      unsigned int offset = 3;

      counter_today_comp = counter_today;
      point = (unsigned char *) (&counter_today);
      for (unsigned int i = 0; i < sizeof(counter_today); i++)
      {
        temp_value = *(point++);
        TxBuffer_SPI_EDF[offset + i] = temp_value;
        crc_eeprom_resurs += temp_value;
      }
      offset += sizeof(counter_today);

      counter_previous_day_comp = counter_previous_day;
      point = (unsigned char *) (&counter_previous_day);
      for (unsigned int i = 0; i < sizeof(counter_previous_day); i++)
      {
        temp_value = *(point++);
        TxBuffer_SPI_EDF[offset + i] = temp_value;
        crc_eeprom_resurs += temp_value;
      }
      offset += sizeof(counter_previous_day);

      counter_total_comp = counter_total;
      point = (unsigned char *) (&counter_total);
      for (unsigned int i = 0; i < sizeof(counter_total); i++)
      {
        temp_value = *(point++);
        TxBuffer_SPI_EDF[offset + i] = temp_value;
        crc_eeprom_resurs += temp_value;
      }
      offset += sizeof(counter_total);

      TxBuffer_SPI_EDF[offset] = (unsigned char) ((~(unsigned int) crc_eeprom_resurs) & 0xff);

      //Скидаємо біт запуску нового запису і виставляємо біт запису блоків у EEPROM з бітом встановлення дозволу на запис
      _SET_BIT(control_spi1_taskes, TASK_EEPROM_WRITE_PREPARATION_BIT);
      _SET_BIT(control_spi1_taskes, TASK_WRITING_RESURS_EEPROM_BIT);
      _CLEAR_BIT(control_spi1_taskes, TASK_START_WRITE_RESURS_EEPROM_BIT);

      //Виставляємо перший блок лічильників ресурсу запису у EEPROM
      number_block_resurs_write_to_eeprom = 0;
    }
    /*************************************************/
  }
  else if (state_execution_spi1 == 1)
  {
    /*************************************************/
    //Обмін завершився без помилок
    /*************************************************/
    //Виставляємо повідомлення, що SPI готовий до нової транзакції
    state_execution_spi1 = -1;

    if (_CHECK_SET_BIT(control_spi1_taskes, TASK_EEPROM_WRITE_PREPARATION_BIT) != 0)
    {
      //      /***
      //      Тільки для відладки
      //      ***/
      //      if(_CHECK_SET_BIT(control_spi1_taskes, TASK_WRITING_SETTINGS_EEPROM_BIT))
      //      {
      //        if (etap_eeprom_write_enable == 0)
      //        {
      //          if (iteration == 0) array_status[number_block_settings_write_to_eeprom][0] = RxBuffer_SPI_EDF[1 + 0];
      //          iteration++;
      //
      //          if ((RxBuffer_SPI_EDF[1 + 0] & 0x70) != 0)
      //          {
      //            if (RxBuffer_SPI_EDF[1 + 0] != 0xff)
      //            {
      //              error_spi_eeprom++;
      //            }
      //            else good_spi_eeprom++;
      //          }
      //          else good_spi_eeprom++;
      //        }
      //      }
      //      /**/

      //Цей блок має бути завжди першим, щоб коли іде перевірка на доступність запису не виконувалися помилковоінші задачі
      if (etap_eeprom_write_enable == 0)
      {
        //Прочитано ресістр статусу

        if ((RxBuffer_SPI_EDF[1 + 0] & 0x70) != 0)
        {
          /*
          Біти 4-6 знаходятся у "1" тільки під час внутрішнього процесу зпапису - 
          Треба продовжувати читати регістр статусу
          */
          etap_eeprom_write_enable = 0;
          if (RxBuffer_SPI_EDF[1 + 0] != 0xff)
          {
            //            /*
            //            Під час внутрішнього процесу запису всі біти 0-7 регстру статусу,
            //            згідно документації, мають бути встановлені.
            //            Якщо ні - це питання, яке потребує дослідження, покищо я це просто фіксую
            //            встановлекнням "1" на контрольній точці КТ3
            //            */
            //            GPIO_KT3->BSRR = GPIO_KT3_Pin;
          }
          else
          {
            //            /*
            //            Під час внутрішнього процесу запису всі біти 0-7 регстру статусу,
            //            згідно документації, мають бути встановлені.
            //            Якщо так і є, то я фіксую нормальну роботу встановлекнням "0" на контрольній точці КТ3
            //            */
            //             GPIO_KT3->BRR = GPIO_KT3_Pin;
          }
        }
        else if ((RxBuffer_SPI_EDF[1 + 0] & (1 << 0)) == 0)
        {
          //          /*
          //          Встановлекнням "0" на контрольній точці КТ3 фіксуємо нормальну роботу з
          //          регістром статусу/дозволом запису EEPROM
          //          */
          //          GPIO_KT3->BRR = GPIO_KT3_Pin;

          //Процес запису у EEPROM не відбувається
          if ((RxBuffer_SPI_EDF[1 + 0] & (1 << 1)) != 0)
          {
            //            /***
            //            Тільки для відладки
            //            ***/
            //            if(_CHECK_SET_BIT(control_spi1_taskes, TASK_WRITING_SETTINGS_EEPROM_BIT))
            //            {
            //              array_status[number_block_settings_write_to_eeprom][1] = RxBuffer_SPI_EDF[1 + 0];
            //              array_status[number_block_settings_write_to_eeprom][2] = iteration;
            //              array_status[number_block_settings_write_to_eeprom][3] = error_spi_eeprom;
            //              array_status[number_block_settings_write_to_eeprom][4] = good_spi_eeprom;
            //              iteration = 0;
            //              error_spi_eeprom = 0;
            //              good_spi_eeprom = 0;
            //            }
            //            /**/

            //EEPROM є доступною до запису
            //Скидаємо біт підготовки до запису EEPROM
            _CLEAR_BIT(control_spi1_taskes, TASK_EEPROM_WRITE_PREPARATION_BIT);
            etap_eeprom_write_enable = -1;
          }
          else
          {
            //EEPROM є не доступною до запису - переходимо на етап подачі команди дозволу запису в EEPROM
            etap_eeprom_write_enable = 1;
          }
        }
        else
        {
          //Інакше виконуємо повторно ту саму опрецію, поки в EEPROM не завершится процес запису

          //          /*
          //          Встановлекнням "0" на контрольній точці КТ3 фіксуємо нормальну роботу з
          //          регістром статусу/дозволом запису EEPROM
          //          */
          //          GPIO_KT3->BRR = GPIO_KT3_Pin;
        }
      }
      else if (etap_eeprom_write_enable == 1)
      {
        //        /*
        //        Встановлекнням "0" на контрольній точці КТ3 фіксуємо нормальну роботу з
        //        регістром статусу/дозволом запису EEPROM
        //        */
        //        GPIO_KT3->BRR = GPIO_KT3_Pin;

        //Передано команду дозволу запису - повторно читаємо регістра статусу
        etap_eeprom_write_enable = 0;
        //Біт підготовки до запису EEPROM не скидаємо, бо маємо переконатися, що щдозвіл запису успішно встановився
      }
      else
      {
        //Теоретично програма сюди б не мала ніколи зайти
        //Відбцлася невизначена помилка, тому треба піти на перезавантаження
        total_error_sw_fixed();
      }
    }
    else if (
      (_CHECK_SET_BIT(control_spi1_taskes, TASK_WRITING_SETTINGS_EEPROM_BIT) != 0) ||
      (_CHECK_SET_BIT(control_spi1_taskes, TASK_WRITING_USTUVANNJA_EEPROM_BIT) != 0) ||
      (_CHECK_SET_BIT(control_spi1_taskes, TASK_WRITING_STATE_LEDS_OUTPUTS_EEPROM_BIT) != 0) ||
      (_CHECK_SET_BIT(control_spi1_taskes, TASK_WRITING_TRG_FUNC_EEPROM_BIT) != 0) ||
      (_CHECK_SET_BIT(control_spi1_taskes, TASK_WRITING_INFO_REJESTRATOR_AR_EEPROM_BIT) != 0) ||
      (_CHECK_SET_BIT(control_spi1_taskes, TASK_WRITING_INFO_REJESTRATOR_DR_EEPROM_BIT) != 0) ||
      (_CHECK_SET_BIT(control_spi1_taskes, TASK_WRITING_INFO_REJESTRATOR_PR_ERR_EEPROM_BIT) != 0) ||
      (_CHECK_SET_BIT(control_spi1_taskes, TASK_WRITING_ANGLE_EEPROM_BIT) != 0) ||
      (_CHECK_SET_BIT(control_spi1_taskes, TASK_WRITING_RESURS_EEPROM_BIT) != 0))
    {
      //Стоїть умова запису блоку у EEPROM

      if (_CHECK_SET_BIT(control_spi1_taskes, TASK_WRITING_SETTINGS_EEPROM_BIT) != 0)
      {
        //Виставляємо наступний блок настройок запису у EEPROM
        number_block_settings_write_to_eeprom++;
      }
      else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_WRITING_USTUVANNJA_EEPROM_BIT) != 0)
      {
        //Виставляємо наступний блок юстування запису у EEPROM
        number_block_ustuvannja_write_to_eeprom++;
      }
      else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_WRITING_STATE_LEDS_OUTPUTS_EEPROM_BIT) != 0)
      {
        //Виставляємо що стани виходів-світлоіндикаторів вже записані - треба тільки коректно завершити цю операцію з витримкою часу на саму процедуру запису у мікросхемі
        number_block_state_leds_outputs_write_to_eeprom++;
      }
      else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_WRITING_TRG_FUNC_EEPROM_BIT) != 0)
      {
        //Виставляємо наступний блок триґерної інформаціїзапису у EEPROM
        number_block_trg_func_write_to_eeprom++;
      }
      else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_WRITING_INFO_REJESTRATOR_AR_EEPROM_BIT) != 0)
      {
        //Виставляємо наступний блок інформації по реєстраторах запису у EEPROM
        number_block_info_rejestrator_ar_write_to_eeprom++;
      }
      else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_WRITING_INFO_REJESTRATOR_DR_EEPROM_BIT) != 0)
      {
        //Виставляємо наступний блок інформації по реєстраторах запису у EEPROM
        number_block_info_rejestrator_dr_write_to_eeprom++;
      }
      else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_WRITING_INFO_REJESTRATOR_PR_ERR_EEPROM_BIT) != 0)
      {
        //Виставляємо наступний блок інформації по реєстраторах запису у EEPROM
        number_block_info_rejestrator_pr_err_write_to_eeprom++;
      }
      else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_WRITING_ANGLE_EEPROM_BIT) != 0)
      {
        //Виставляємо що кути для сельсинового контролю вже записані - треба тільки коректно завершити цю операцію з витримкою часу на саму процедуру запису у мікросхемі
        number_block_angle_write_to_eeprom++;
      }
      else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_WRITING_RESURS_EEPROM_BIT) != 0)
      {
        //Виставляємо наступний блок інформації по лічильниках запису у EEPROM
        number_block_resurs_write_to_eeprom++;
      }
      else
      {
        //Сюди програма не мала б теоретично ніколи заходити
        //Якщо сюди дійшла програма, значить відбулася недопустива помилка, тому треба зациклити програму, щоб вона пішла на перезагрузку
        total_error_sw_fixed();
      }

      //Виставляємо біт встановлення дозволу на запис
      _SET_BIT(control_spi1_taskes, TASK_EEPROM_WRITE_PREPARATION_BIT);
    }
    else if (
      (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_READ_USTUVANNJA_EEPROM_BIT) != 0) ||
      (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_READ_SETTINGS_EEPROM_BIT) != 0) ||
      (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_READ_STATE_LEDS_OUTPUTS_EEPROM_BIT) != 0) ||
      (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_READ_TRG_FUNC_EEPROM_BIT) != 0) ||
      (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_READ_INFO_REJESTRATOR_AR_EEPROM_BIT) != 0) ||
      (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_READ_INFO_REJESTRATOR_DR_EEPROM_BIT) != 0) ||
      (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_READ_INFO_REJESTRATOR_PR_ERR_EEPROM_BIT) != 0) ||
      (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_READ_ANGLE_EEPROM_BIT) != 0) ||
      (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_READ_RESURS_EEPROM_BIT) != 0))
    {
      //Прочитано ресістр статусу
      if ((RxBuffer_SPI_EDF[1 + 0] & (1 << 0)) == 0)
      {
        //Процес запису у EEPROM не відбувається
        if (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_READ_USTUVANNJA_EEPROM_BIT) != 0)
        {
          //Скидаємо біт запуску читання юстування і виставляємо біт процесу читання юстування
          _SET_BIT(control_spi1_taskes, TASK_READING_USTUVANNJA_EEPROM_BIT);
          _CLEAR_BIT(control_spi1_taskes, TASK_START_READ_USTUVANNJA_EEPROM_BIT);
        }
        else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_READ_SETTINGS_EEPROM_BIT) != 0)
        {
          //Скидаємо біт запуску читання настройок і виставляємо біт процесу читання настройок
          _SET_BIT(control_spi1_taskes, TASK_READING_SETTINGS_EEPROM_BIT);
          _CLEAR_BIT(control_spi1_taskes, TASK_START_READ_SETTINGS_EEPROM_BIT);
        }
        else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_READ_STATE_LEDS_OUTPUTS_EEPROM_BIT) != 0)
        {
          //Скидаємо біт запуску читання стану тригерних світлодіодів і сигнальних реле і виставляємо біт процесу читання тригерних світлодіодів і сигнальних реле
          _SET_BIT(control_spi1_taskes, TASK_READING_STATE_LEDS_OUTPUTS_EEPROM_BIT);
          _CLEAR_BIT(control_spi1_taskes, TASK_START_READ_STATE_LEDS_OUTPUTS_EEPROM_BIT);
        }
        else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_READ_TRG_FUNC_EEPROM_BIT) != 0)
        {
          //Скидаємо біт запуску читання стану триґерної інформації і виставляємо біт процесу читання триґерної інформації
          _SET_BIT(control_spi1_taskes, TASK_READING_TRG_FUNC_EEPROM_BIT);
          _CLEAR_BIT(control_spi1_taskes, TASK_START_READ_TRG_FUNC_EEPROM_BIT);
        }
        else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_READ_INFO_REJESTRATOR_AR_EEPROM_BIT) != 0)
        {
          //Скидаємо біт запуску читання інформації по аналоговому реєстратору і виставляємо біт процесу читання інформації по дискретному реєстратору
          _SET_BIT(control_spi1_taskes, TASK_READING_INFO_REJESTRATOR_AR_EEPROM_BIT);
          _CLEAR_BIT(control_spi1_taskes, TASK_START_READ_INFO_REJESTRATOR_AR_EEPROM_BIT);
        }
        else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_READ_INFO_REJESTRATOR_DR_EEPROM_BIT) != 0)
        {
          //Скидаємо біт запуску читання інформації по дискретному реєстратору і виставляємо біт процесу читання інформації по дискретному реєстратору
          _SET_BIT(control_spi1_taskes, TASK_READING_INFO_REJESTRATOR_DR_EEPROM_BIT);
          _CLEAR_BIT(control_spi1_taskes, TASK_START_READ_INFO_REJESTRATOR_DR_EEPROM_BIT);
        }
        else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_READ_INFO_REJESTRATOR_PR_ERR_EEPROM_BIT) != 0)
        {
          //Скидаємо біт запуску читання інформації по дискретному реєстратору і виставляємо біт процесу читання інформації по дискретному реєстратору
          _SET_BIT(control_spi1_taskes, TASK_READING_INFO_REJESTRATOR_PR_ERR_EEPROM_BIT);
          _CLEAR_BIT(control_spi1_taskes, TASK_START_READ_INFO_REJESTRATOR_PR_ERR_EEPROM_BIT);
        }
        else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_READ_ANGLE_EEPROM_BIT) != 0)
        {
          //Скидаємо біт запуску читання інформації і виставляємо біт процесу читання інформації
          _SET_BIT(control_spi1_taskes, TASK_READING_ANGLE_EEPROM_BIT);
          _CLEAR_BIT(control_spi1_taskes, TASK_START_READ_ANGLE_EEPROM_BIT);
        }
        else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_READ_RESURS_EEPROM_BIT) != 0)
        {
          //Скидаємо біт запуску читання інформації і виставляємо біт процесу читання інформації
          _SET_BIT(control_spi1_taskes, TASK_READING_RESURS_EEPROM_BIT);
          _CLEAR_BIT(control_spi1_taskes, TASK_START_READ_RESURS_EEPROM_BIT);
        }
        else
        {
          //Сюди програма не мала б теоретично ніколи заходити
          //Якщо сюди дійшла програма, значить відбулася недопустива помилка, тому треба зациклити програму, щоб вона пішла на перезагрузку
          total_error_sw_fixed();
        }
      }
      else
      {
        //Інакше виконуємо повторно ту саму опрецію, поки в EEPROM не завершится процес запису
      }
    }
    else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_READING_SETTINGS_EEPROM_BIT) != 0)
    {
      //Аналізуємо прочитані дані
      //Спочатку аналізуємо, чи прояитаний блок є пустим, чи вже попередньо записаним
      unsigned int empty_block = 1, i = 0;
      static __SETTINGS current_settings_tmp;

      while ((empty_block != 0) && (i < (sizeof(__SETTINGS) + 2)))
      {
        if (RxBuffer_SPI_EDF[3 + i] != 0xff)
          empty_block = 0;
        i++;
      }

      if (empty_block == 0)
      {
        //Помічаємо, що блок настроювання не є пустим
        state_spi1_task &= (unsigned int) (~STATE_SETTINGS_EEPROM_EMPTY);
        //Скидаємо повідомлення у слові діагностики
        _SET_BIT(clear_diagnostyka, ERROR_SETTINGS_EEPROM_EMPTY_BIT);

        //Перевіряємо контрольну суму і переписуємо прочитані дані у структуру управління
        unsigned char crc_eeprom_settings = 0, temp_value;
        unsigned char *point = (unsigned char *) (&current_settings_tmp);
        for (i = 0; i < sizeof(__SETTINGS); i++)
        {
          temp_value = RxBuffer_SPI_EDF[3 + i];
          *(point) = temp_value;
          crc_eeprom_settings += temp_value;
          point++;
        }
        if (
          (RxBuffer_SPI_EDF[3 + sizeof(__SETTINGS)] == ((unsigned char) ((~(unsigned int) crc_eeprom_settings) & 0xff))) &&
          (RxBuffer_SPI_EDF[3 + sizeof(__SETTINGS) + 1] == ((unsigned char) (crc_eeprom_settings & 0xff))))
        {
          //Контролдьна сума сходиться

          //Скидаємо повідомлення у слові діагностики
          _SET_BIT(clear_diagnostyka, ERROR_SETTINGS_EEPROM_BIT);

          if (current_settings_tmp.device_id == ((VERSIA_PZ << 8) | (MODYFIKACIA_VERSII_PZ)))
          {
            //Таблиця настройок відповідає типу даного приладу

            //Зберігаємо контрольну суму (не інвертовану)
            crc_settings = crc_eeprom_settings;

            if ((comparison_writing & COMPARISON_WRITING_SETTINGS) == 0)
            {
              //Виконувалося зчитування настройок у таблицю настройок

              //Помічаємо, що таблиця зараз змінилася і її треба буде з системи захистів зкопіювати у таблицю з якою працює система захистів
              changed_settings = CHANGED_ETAP_EXECUTION;
              //Перекидаємо таблицю настройок з тимчасового масиву у робочу таблицю
              current_settings = current_settings_tmp;
              //Помічаємо, що таблиця змінилася і її треба буде з системи захистів зкопіювати у таблицю з якою працює система захистів
              changed_settings = CHANGED_ETAP_ENDED;
              current_settings_interfaces = current_settings;

              //Розраховуємо розмір одного запису і максимальну кількість записів у аналоговому реєстраторі для даних витримок
              /*
              Читання настройок є зациклене поки не буде зчитано успішно настройок, а це означає, що ця дія мусить виконатися перед тим
              як ми продовжимо запускати інші модулі в роботу
              */
              //Онулюємо доаварійний масив перед стартом захистів
              {
                uint32_t const number_words_slice = AR_TOTAL_NUMBER_CANALES;
                uint32_t total_size = (current_settings.prefault_number_periods / current_settings.diskretnt_number_periods) * number_words_slice;
                int32_t difference = index_array_ar_heat - total_size;
                uint32_t index = (difference >= 0) ? difference : (difference + SIZE_BUFFER_FOR_AR);

                uint32_t m = 0;
                for (size_t l = 0; l < total_size; l++)
                {
                  int32_t data_tmp = (m < AR_NUMBER_ANALOG_CANALES) ? 0x8000 : 0;
                  if (++m >= number_words_slice)
                    m = 0;

                  array_ar[index++] = data_tmp;
                  //                  AR_WRITE(index, data_tmp);
                  if (index >= SIZE_BUFFER_FOR_AR)
                    index = 0; /*Умова мал аб бути ==, але щоб перестахуватися на невизначену помилку я поставив >=*/
                }
              }
#if (                              \
  (MODYFIKACIA_VERSII_PZ == 0) ||  \
  (MODYFIKACIA_VERSII_PZ == 3) ||  \
  (MODYFIKACIA_VERSII_PZ == 4) ||  \
  (MODYFIKACIA_VERSII_PZ == 6) ||  \
  (MODYFIKACIA_VERSII_PZ == 10) || \
  (MODYFIKACIA_VERSII_PZ == 14) || \
  (MODYFIKACIA_VERSII_PZ == 13) || \
  (MODYFIKACIA_VERSII_PZ == 14))
              zdz_ovd_porig = current_settings.zdz_ovd_porig + 1;
              _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD25_DD27_DD28_DD30) = ((zdz_ovd_porig & 0xf) << 8) | (test_OVD << 12);
#elif (                           \
  (MODYFIKACIA_VERSII_PZ == 7) || \
  (MODYFIKACIA_VERSII_PZ == 17))
              _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37) =
                (state_outputs_raw & ((1u << 9) - 1)) |
                ((((current_settings.zdz_ovd_porig + 1) & 0xf) << 9) | ((test_OVD != 0) << 13)) |
                (((state_outputs_raw >> 9) & ((1u << 2) - 1)) << 14);
#endif
            }
            else
            {
              //Виконувалося контроль достовірності записаної інформації у EEPROM з записуваною

              unsigned char *point_to_read = (unsigned char *) (&current_settings_tmp);
              unsigned char *point_to_write = (unsigned char *) (&current_settings_comp);
              unsigned int difference = 0;

              i = 0;
              while ((difference == 0) && (i < sizeof(__SETTINGS)))
              {
                if (*point_to_write != *point_to_read)
                  difference = 0xff;
                else
                {
                  point_to_write++;
                  point_to_read++;
                  i++;
                }
              }
              if (difference == 0)
              {
                //Контроль порівнняння пройшов успішно

                //Скидаємо повідомлення у слові діагностики
                _SET_BIT(clear_diagnostyka, ERROR_SETTINGS_EEPROM_COMPARISON_BIT);
              }
              else
              {
                //Контроль порівнняння зафіксував розбіжності між записаною і записуваною інформацією

                //Виствляємо повідомлення у слові діагностики
                _SET_BIT(set_diagnostyka, ERROR_SETTINGS_EEPROM_COMPARISON_BIT);
              }
            }

            state_spi1_task &= (unsigned int) (~STATE_SETTINGS_EEPROM_FAIL);
            state_spi1_task |= STATE_SETTINGS_EEPROM_GOOD;

            //Скидаємо повідомлення у слові діагностики
            _SET_BIT(clear_diagnostyka, ERROR_SETTINGS_EEPROM_DEVICE_ID_FAIL_BIT);
          }
          else
          {
            //Таблиця настройок не відповідає типу даного приладу

            //Помічаємо, що прочитаний блок настройок є пустим
            state_spi1_task &= (unsigned int) (~STATE_SETTINGS_EEPROM_FAIL);
            state_spi1_task &= (unsigned int) (~STATE_SETTINGS_EEPROM_GOOD);
            state_spi1_task |= STATE_SETTINGS_EEPROM_EMPTY; /*Не відповідність типу настройок це то саме що їх немає взагалі*/

            //Виствляємо повідомлення у слові діагностики
            _SET_BIT(set_diagnostyka, ERROR_SETTINGS_EEPROM_DEVICE_ID_FAIL_BIT);
          }
        }
        else
        {
          //Контрольна сума не сходиться
          state_spi1_task &= (unsigned int) (~STATE_SETTINGS_EEPROM_GOOD);
          state_spi1_task |= STATE_SETTINGS_EEPROM_FAIL;

          //Виствляємо повідомлення у слові діагностики
          _SET_BIT(clear_diagnostyka, ERROR_SETTINGS_EEPROM_DEVICE_ID_FAIL_BIT);
          _SET_BIT(set_diagnostyka, ERROR_SETTINGS_EEPROM_BIT);
        }
      }
      else
      {
        //Помічаємо, що прочитаний блок настройок є пустим
        state_spi1_task &= (unsigned int) (~STATE_SETTINGS_EEPROM_FAIL);
        state_spi1_task &= (unsigned int) (~STATE_SETTINGS_EEPROM_GOOD);
        state_spi1_task |= STATE_SETTINGS_EEPROM_EMPTY;

        //Виствляємо повідомлення у слові діагностики
        _SET_BIT(clear_diagnostyka, ERROR_SETTINGS_EEPROM_BIT);
        _SET_BIT(clear_diagnostyka, ERROR_SETTINGS_EEPROM_DEVICE_ID_FAIL_BIT);
        _SET_BIT(set_diagnostyka, ERROR_SETTINGS_EEPROM_EMPTY_BIT);
      }

      //Знімаємо можливу сигналізацію, що виконувалося порівнняння
      comparison_writing &= (unsigned int) (~COMPARISON_WRITING_SETTINGS);
      //Скидаємо повідомлення про читання даних
      _CLEAR_BIT(control_spi1_taskes, TASK_READING_SETTINGS_EEPROM_BIT);
    }
    else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_READING_USTUVANNJA_EEPROM_BIT) != 0)
    {
      //Аналізуємо прочитані дані
      //Спочатку аналізуємо, чи прояитаний блок є пустим, чи вже попередньо записаним
      unsigned int empty_block = 1, i = 0;
      unsigned int adjustment_id_tmp = 0, ustuvannja_tmp[NUMBER_ANALOG_CANALES], serial_number_dev_tmp = 0;

      while ((empty_block != 0) && (i < (SIZE_USTUVANNJA + 1)))
      {
        if (RxBuffer_SPI_EDF[3 + i] != 0xff)
          empty_block = 0;
        i++;
      }

      if (empty_block == 0)
      {
        //Помічаємо, що блок настроювання не є пустим
        state_spi1_task &= (unsigned int) (~STATE_USTUVANNJA_EEPROM_EMPTY);
        //Скидаємо повідомлення у слові діагностики
        _SET_BIT(clear_diagnostyka, ERROR_USTUVANNJA_EEPROM_EMPTY_BIT);

        //Перевіряємо контрольну суму і переписуємо прочитані дані у масив юстування з серійним номером
        unsigned char crc_eeprom_ustuvannja = 0, temp_value;
        unsigned char *point;
        unsigned int offset = 3;

        point = (unsigned char *) (&adjustment_id_tmp);
        for (i = 0; i < sizeof(unsigned int); i++)
        {
          temp_value = RxBuffer_SPI_EDF[offset + i];
          *(point) = temp_value;
          crc_eeprom_ustuvannja += temp_value;
          point++;
        }
        offset += sizeof(unsigned int);

        point = (unsigned char *) (&ustuvannja_tmp);
        for (i = 0; i < sizeof(ustuvannja_tmp); i++)
        {
          temp_value = RxBuffer_SPI_EDF[offset + i];
          *(point) = temp_value;
          crc_eeprom_ustuvannja += temp_value;
          point++;
        }
        offset += sizeof(ustuvannja_tmp);

        point = (unsigned char *) (&serial_number_dev_tmp);
        for (i = 0; i < sizeof(serial_number_dev_tmp); i++)
        {
          temp_value = RxBuffer_SPI_EDF[offset + i];
          *(point) = temp_value;
          crc_eeprom_ustuvannja += temp_value;
          point++;
        }
        offset += sizeof(serial_number_dev_tmp);

        if (RxBuffer_SPI_EDF[offset] == ((unsigned char) ((~(unsigned int) crc_eeprom_ustuvannja) & 0xff)))
        {
          //Контролдьна сума сходиться

          //Скидаємо повідомлення у слові діагностики
          _SET_BIT(clear_diagnostyka, ERROR_USTUVANNJA_EEPROM_BIT);

          if (adjustment_id_tmp == ADJUSTMENT_ID)
          {
            //Тип набору юстуючих коефіцієнтів відповідає даному програмному забезпеченню

            //Зберігаємо контрольну суму (не інвертовану)
            crc_ustuvannja = crc_eeprom_ustuvannja;

            if ((comparison_writing & COMPARISON_WRITING_USTUVANNJA) == 0)
            {
              //Виконувалося зчитування юстування у масив юстування

              //Помічаємо, що юстування зараз буде змінюватися і їх треба буде з вимірювальної системи зкопіювати у таблицю з якою працює вимірювальна система
              changed_ustuvannja = CHANGED_ETAP_EXECUTION;
              //Перекидаємо масив юстування з тимчасового масиву у робочий масив
              for (unsigned int k = 0; k < NUMBER_ANALOG_CANALES; k++)
              {
                ustuvannja[k] = ustuvannja_tmp[k];
              }
              serial_number_dev = serial_number_dev_tmp;
              //Помічаємо, що юстування змінилася і її треба буде з вимірювальної системи зкопіювати у масив з яким працює вимірювальна система
              changed_ustuvannja = CHANGED_ETAP_ENDED;
            }
            else
            {
              //Виконувалося контроль достовірності записаної інформації у EEPROM з записуваною

              unsigned int difference = 0;

              i = 0;
              while ((difference == 0) && (i < NUMBER_ANALOG_CANALES))
              {
                //Перевірка запису юстуючих коефіцієнтів
                if (
                  (ustuvannja_comp[i] != ustuvannja_tmp[i]))
                {
                  difference = 0xff;
                }
                else
                {
                  i++;
                }
              }
              if (difference == 0)
              {
                //Перевірка запису серійного номеру (якщо до того не зафіксовано вже помилки)
                if (serial_number_dev_comp != serial_number_dev_tmp)
                  difference = 0xff;
              }

              if (difference == 0)
              {
                //Контроль порівнняння пройшов успішно

                //Скидаємо повідомлення у слові діагностики
                _SET_BIT(clear_diagnostyka, ERROR_USTUVANNJA_EEPROM_COMPARISON_BIT);
              }
              else
              {
                //Контроль порівнняння зафіксував розбіжності між записаною і записуваною інформацією

                //Виствляємо повідомлення у слові діагностики
                _SET_BIT(set_diagnostyka, ERROR_USTUVANNJA_EEPROM_COMPARISON_BIT);
              }
            }

            state_spi1_task &= (unsigned int) (~STATE_USTUVANNJA_EEPROM_FAIL);
            state_spi1_task |= STATE_USTUVANNJA_EEPROM_GOOD;

            //Скидаємо повідомлення у слові діагностики
            _SET_BIT(clear_diagnostyka, ERROR_USTUVANNJA_EEPROM_ADJUSTMENT_ID_FAIL_BIT);
          }
          else
          {
            //Тип набору юстуючих коефіцієнтів не відповідає даному програмному забезпеченню

            //Помічаємо, що прочитаний блок юстування є пустим
            state_spi1_task &= (unsigned int) (~STATE_USTUVANNJA_EEPROM_FAIL);
            state_spi1_task &= (unsigned int) (~STATE_USTUVANNJA_EEPROM_GOOD);
            state_spi1_task |= STATE_USTUVANNJA_EEPROM_EMPTY; /*Не відповідність типу юстуючих коефіцієнтів -  це то саме що їх немає взагалі*/

            //Виствляємо повідомлення у слові діагностики
            _SET_BIT(set_diagnostyka, ERROR_USTUVANNJA_EEPROM_ADJUSTMENT_ID_FAIL_BIT);
          }
        }
        else
        {
          //Контрольна сума не сходиться
          state_spi1_task &= (unsigned int) (~STATE_USTUVANNJA_EEPROM_GOOD);
          state_spi1_task |= STATE_USTUVANNJA_EEPROM_FAIL;

          //Виствляємо повідомлення у слові діагностики
          _SET_BIT(clear_diagnostyka, ERROR_USTUVANNJA_EEPROM_ADJUSTMENT_ID_FAIL_BIT);
          _SET_BIT(set_diagnostyka, ERROR_USTUVANNJA_EEPROM_BIT);
        }
      }
      else
      {
        //Помічаємо, що прочитаний блок юстування є пустим
        state_spi1_task &= (unsigned int) (~STATE_USTUVANNJA_EEPROM_FAIL);
        state_spi1_task &= (unsigned int) (~STATE_USTUVANNJA_EEPROM_GOOD);
        state_spi1_task |= STATE_USTUVANNJA_EEPROM_EMPTY;

        //Виствляємо повідомлення у слові діагностики
        _SET_BIT(clear_diagnostyka, ERROR_USTUVANNJA_EEPROM_BIT);
        _SET_BIT(clear_diagnostyka, ERROR_USTUVANNJA_EEPROM_ADJUSTMENT_ID_FAIL_BIT);
        _SET_BIT(set_diagnostyka, ERROR_USTUVANNJA_EEPROM_EMPTY_BIT);
      }

      //Знімаємо можливу сигналізацію, що виконувалося порівнняння
      comparison_writing &= (unsigned int) (~COMPARISON_WRITING_USTUVANNJA);
      //Скидаємо повідомлення про читання даних
      _CLEAR_BIT(control_spi1_taskes, TASK_READING_USTUVANNJA_EEPROM_BIT);
    }
    else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_READING_STATE_LEDS_OUTPUTS_EEPROM_BIT) != 0)
    {
      //Аналізуємо прочитані дані
      //Спочатку аналізуємо, чи прояитаний блок є пустим, чи вже попередньо записаним
      unsigned int empty_block = 1, i = 0;
      unsigned int state_trigger_leds_tmp = 0, state_signal_outputs_tmp = 0;

      while ((empty_block != 0) && (i < (2 * (3 + 3))))
      {
        if (RxBuffer_SPI_EDF[3 + i] != 0xff)
          empty_block = 0;
        i++;
      }

      if (empty_block == 0)
      {
        //Помічаємо, що блок не є пустим
        state_spi1_task &= (unsigned int) (~STATE_STATE_LEDS_OUTPUTS_EEPROM_EMPTY);

        //Скидаємо повідомлення у слові діагностики, що блок пустий
        _SET_BIT(clear_diagnostyka, ERROR_STATE_LEDS_OUTPUTS_EEPROM_EMPTY_BIT);

        //Перевіряємо достовірність стану тригерних індикаторів
        state_trigger_leds_tmp = RxBuffer_SPI_EDF[3 + 0] | (RxBuffer_SPI_EDF[3 + 1] << 8) | (RxBuffer_SPI_EDF[3 + 2] << 16);
        unsigned int value_1 = RxBuffer_SPI_EDF[3 + 3] | (RxBuffer_SPI_EDF[3 + 4] << 8) | (RxBuffer_SPI_EDF[3 + 5] << 16);
        if (state_trigger_leds_tmp == ((unsigned int) ((~value_1) & ((1 << NUMBER_LEDS) - 1))))
        {
          //Контролдь зійшовся

          if ((comparison_writing & COMPARISON_WRITING_STATE_LEDS_OUTPUTS) == 0)
          {
            //Виконувалося зчитування станів тригерних світлоіндикаторів/сигнальних виходів

            //Відновлюємо інформацію по тригерних світлоіндикаторах
            state_leds = state_trigger_leds = state_trigger_leds_tmp;
          }

          state_spi1_task &= (unsigned int) (~STATE_STATE_LEDS_EEPROM_FAIL);
          state_spi1_task |= STATE_STATE_LEDS_EEPROM_GOOD;

          //Скидаємо повідомлення у слові діагностики
          _SET_BIT(clear_diagnostyka, ERROR_STATE_LEDS_EEPROM_BIT);
        }
        else
        {
          //Контролдь не зійшовся
          state_spi1_task &= (unsigned int) (~STATE_STATE_LEDS_EEPROM_GOOD);
          state_spi1_task |= STATE_STATE_LEDS_EEPROM_FAIL;

          //Виствляємо повідомлення у слові діагностики
          _SET_BIT(set_diagnostyka, ERROR_STATE_LEDS_EEPROM_BIT);
        }

        //Перевіряємо достовірність стану сигнальних виходів
        state_signal_outputs_tmp = RxBuffer_SPI_EDF[3 + 6] | (RxBuffer_SPI_EDF[3 + 7] << 8) | (RxBuffer_SPI_EDF[3 + 8] << 16);
        value_1 = RxBuffer_SPI_EDF[3 + 9] | (RxBuffer_SPI_EDF[3 + 10] << 8) | (RxBuffer_SPI_EDF[3 + 11] << 16);
        if (state_signal_outputs_tmp == ((unsigned int) ((~value_1) & ((1 << NUMBER_SIMPLE_OUTPUTS) - 1))))
        {
          //Контролдь зійшовся

          if ((comparison_writing & COMPARISON_WRITING_STATE_LEDS_OUTPUTS) == 0)
          {
            //Виконувалося зчитування станів тригерних світлоіндикаторів/сигнальних виходів

            //Відновлюємо інформацію по сигнальних виходах
            state_outputs_raw = state_outputs = state_signal_outputs = state_signal_outputs_tmp;
#if (                             \
  (MODYFIKACIA_VERSII_PZ == 8) || \
  (MODYFIKACIA_VERSII_PZ == 18))
            chGbl__REL1_REL8__W_VAL &= 0X3;
            chGbl__REL1_REL8__W_VAL |= state_outputs_raw & 3;
#endif
          }

          state_spi1_task &= (unsigned int) (~STATE_STATE_OUTPUTS_EEPROM_FAIL);
          state_spi1_task |= STATE_STATE_OUTPUTS_EEPROM_GOOD;

          //Скидаємо повідомлення у слові діагностики
          _SET_BIT(clear_diagnostyka, ERROR_STATE_OUTPUTS_EEPROM_BIT);
        }
        else
        {
          //Контролдь не зійшовся
          state_spi1_task &= (unsigned int) (~STATE_STATE_OUTPUTS_EEPROM_GOOD);
          state_spi1_task |= STATE_STATE_OUTPUTS_EEPROM_FAIL;

          //Виствляємо повідомлення у слові діагностики
          _SET_BIT(set_diagnostyka, ERROR_STATE_OUTPUTS_EEPROM_BIT);
        }
      }
      else
      {
        //Помічаємо, що прочитаний блок є пустим
        state_spi1_task &= (unsigned int) (~(
          STATE_STATE_LEDS_EEPROM_FAIL |
          STATE_STATE_LEDS_EEPROM_GOOD |
          STATE_STATE_OUTPUTS_EEPROM_FAIL |
          STATE_STATE_OUTPUTS_EEPROM_GOOD));
        state_spi1_task |= STATE_STATE_LEDS_OUTPUTS_EEPROM_EMPTY;

        //Виствляємо повідомлення у слові діагностики
        _SET_BIT(clear_diagnostyka, ERROR_STATE_LEDS_EEPROM_BIT);
        _SET_BIT(clear_diagnostyka, ERROR_STATE_OUTPUTS_EEPROM_BIT);
        _SET_BIT(set_diagnostyka, ERROR_STATE_LEDS_OUTPUTS_EEPROM_EMPTY_BIT);

        if ((comparison_writing & COMPARISON_WRITING_STATE_LEDS_OUTPUTS) == 0)
        {
          /*
          Виставляємо повідомлення про те, що в EEPROM треба записати нові значення
          сигнальних виходів і тригерних світлоіндикаторів тільки тоді, коли ми зчитуємо
          збережені дані для відновлення їх у оперативній пам'яті, а не коли ми проводимо 
          контроль запису.
          Бо для контролю запису нам важливо знати чи успішно записалися дані, які є у 
          оперативній пам'яті і при цьому, навіть, якщо запис відбувся невдало, то,
          оскільки ми працюємо зі змінними з оперативної пам'яті,  які є у нас достовірні,
          бо ми їх якраз і записували, то на роботу до перезавантаження програмного забезперечння 
          збій запису у EEPROM не мав би вплинути
          */
          _SET_BIT(control_spi1_taskes, TASK_START_WRITE_STATE_LEDS_OUTPUTS_EEPROM_BIT);
        }
      }

      if ((comparison_writing & COMPARISON_WRITING_STATE_LEDS_OUTPUTS) == 0)
      {
        //Після виконування зчитування станів тригерних світлоіндикаторів/сигнальних виходів - вводимо їх у МРЗС-05Л

        //Виводимо інформацію по світлоіндикаторах на світлодіоди
        for (size_t col = 0; col < LED_N_COL; col++)
          _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD32_DD38) = ((1 << col) << LED_N_ROW) | ((uint32_t)(~0) & ((1 << LED_N_ROW) - 1));

        //Виставляємо пін CON-L, щоб можна було управляти свтоіндикаторами
        GPIO_SetBits(CON_L, CON_L_PIN);

        //Виводимо інформацію по виходах на піни процесора
#if (                             \
  (MODYFIKACIA_VERSII_PZ != 7) && \
  (MODYFIKACIA_VERSII_PZ != 17))
        _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37) = state_outputs_raw;
#else
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

#endif

        TIM_PRT_write_tick = TIM2->CNT;
        //        //Виставляємо пін CON-OUTPUTS-1, щоб можна було управляти виходами
        //        GPIO_SetBits(CON_OUTPUTS, CON_1_OUTPUTS_PIN);
        //        //Знімаємо пін CON-OUTPUTS-2, щоб можна було управляти виходамии
        //        GPIO_ResetBits(CON_OUTPUTS, CON_2_OUTPUTS_PIN);
        //        //Виставляємо  пін CON-OUTPUTS-3, щоб можна було управляти виходами
        //        GPIO_SetBits(CON_OUTPUTS, CON_3_OUTPUTS_PIN);
      }
      else
      {
        //Виконувалося контроль достовірності записаної інформації у EEPROM з записуваною

        if (
          (state_spi1_task & (STATE_STATE_LEDS_EEPROM_GOOD | STATE_STATE_OUTPUTS_EEPROM_GOOD)) ==
          (STATE_STATE_LEDS_EEPROM_GOOD | STATE_STATE_OUTPUTS_EEPROM_GOOD))
        {
          //Контроль запису здійснюємо тільки тоді, коли коли блок читання не є пустим і помилок не зафіксовано
          if (
            (state_trigger_leds_comp == state_trigger_leds_tmp) &&
            (state_signal_outputs_comp == state_signal_outputs_tmp))
          {
            //Контроль порівнняння пройшов успішно

            //Скидаємо повідомлення у слові діагностики
            _SET_BIT(clear_diagnostyka, ERROR_STATE_LEDS_OUTPUTS_EEPROM_COMPARISON_BIT);
          }
          else
          {
            //Контроль порівнняння зафіксував розбіжності між записаною і записуваною інформацією

            //Виствляємо повідомлення у слові діагностики
            _SET_BIT(set_diagnostyka, ERROR_STATE_LEDS_OUTPUTS_EEPROM_COMPARISON_BIT);
          }
        }
      }

      //Знімаємо можливу сигналізацію, що виконувалося порівнняння
      comparison_writing &= (unsigned int) (~COMPARISON_WRITING_STATE_LEDS_OUTPUTS);
      //Скидаємо повідомлення про читання даних
      _CLEAR_BIT(control_spi1_taskes, TASK_READING_STATE_LEDS_OUTPUTS_EEPROM_BIT);
    }
    else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_READING_TRG_FUNC_EEPROM_BIT) != 0)
    {
      //Аналізуємо прочитані дані
      //Спочатку аналізуємо, чи прояитаний блок є пустим, чи вже попередньо записаним
      unsigned int empty_block = 1, i = 0;
      unsigned int fix_active_buttons_tmp = 0, trigger_active_functions_tmp[N_BIG];

      while ((empty_block != 0) && (i < (sizeof(fix_active_buttons_tmp) + sizeof(trigger_active_functions_tmp) + 1)))
      {
        if (RxBuffer_SPI_EDF[3 + i] != 0xff)
          empty_block = 0;
        i++;
      }

      if (empty_block == 0)
      {
        //Помічаємо, що блок не є пустим
        state_spi1_task &= (unsigned int) (~STATE_TRG_FUNC_EEPROM_EMPTY);
        //Скидаємо повідомлення у слові діагностики
        _SET_BIT(clear_diagnostyka, ERROR_TRG_FUNC_EEPROM_EMPTY_BIT);

        //Перевіряємо контрольну суму і переписуємо прочитані дані у тимчасові змінні
        unsigned char crc_eeprom_trg_func = 0, temp_value;
        unsigned char *point;
        unsigned int offset = 3;

        point = (unsigned char *) (&fix_active_buttons_tmp);
        for (i = 0; i < sizeof(fix_active_buttons_tmp); i++)
        {
          temp_value = RxBuffer_SPI_EDF[offset + i];
          *(point) = temp_value;
          crc_eeprom_trg_func += temp_value;
          point++;
        }
        offset += sizeof(fix_active_buttons_tmp);

        point = (unsigned char *) (&trigger_active_functions_tmp);
        for (i = 0; i < sizeof(trigger_active_functions_tmp); i++)
        {
          temp_value = RxBuffer_SPI_EDF[offset + i];
          *(point) = temp_value;
          crc_eeprom_trg_func += temp_value;
          point++;
        }
        offset += sizeof(trigger_active_functions_tmp);

        if (RxBuffer_SPI_EDF[offset] == ((unsigned char) ((~(unsigned int) crc_eeprom_trg_func) & 0xff)))
        {
          //Контролдьна сума сходиться

          //Скидаємо повідомлення у слові діагностики
          _SET_BIT(clear_diagnostyka, ERROR_TRG_FUNC_EEPROM_BIT);

          //Зберігаємо контрольну суму (не інвертовану)
          crc_trg_func = crc_eeprom_trg_func;

          if ((comparison_writing & COMPARISON_WRITING_TRG_FUNC) == 0)
          {
            //Виконувалося зчитування триґерної інформації

            //Перекидаємо триґерну інформацію у робочі змінні
            fix_active_buttons = fix_active_buttons_tmp;
            for (unsigned int k = 0; k < N_BIG; k++)
              trigger_active_functions[k] = trigger_active_functions_tmp[k];
            restore_trigger_functions(trigger_active_functions);
          }
          else
          {
            //Виконувалося контроль достовірності записаної інформації у EEPROM з записуваною

            unsigned int difference = 0;

            if (fix_active_buttons_comp != fix_active_buttons_tmp)
              difference = 0xff;
            i = 0;
            while ((difference == 0) && (i < N_BIG))
            {
              //Перевірка запису функцій значення яких зберігається у енергонезалежній пам'яті
              if (trigger_active_functions_comp[i] != trigger_active_functions_tmp[i])
                difference = 0xff;
              else
              {
                i++;
              }
            }

            if (difference == 0)
            {
              //Контроль порівнняння пройшов успішно

              //Скидаємо повідомлення у слові діагностики
              _SET_BIT(clear_diagnostyka, ERROR_TRG_FUNC_EEPROM_COMPARISON_BIT);
            }
            else
            {
              //Контроль порівнняння зафіксував розбіжності між записаною і записуваною інформацією

              //Виствляємо повідомлення у слові діагностики
              _SET_BIT(set_diagnostyka, ERROR_TRG_FUNC_EEPROM_COMPARISON_BIT);
            }
          }

          state_spi1_task &= (unsigned int) (~STATE_TRG_FUNC_EEPROM_FAIL);
          state_spi1_task |= STATE_TRG_FUNC_EEPROM_GOOD;
        }
        else
        {
          //Контрольна сума не сходиться
          state_spi1_task &= (unsigned int) (~STATE_TRG_FUNC_EEPROM_GOOD);
          state_spi1_task |= STATE_TRG_FUNC_EEPROM_FAIL;

          //Виствляємо повідомлення у слові діагностики
          _SET_BIT(set_diagnostyka, ERROR_TRG_FUNC_EEPROM_BIT);
        }
      }
      else
      {
        //Помічаємо, що прочитаний блок є пустим
        state_spi1_task &= (unsigned int) (~STATE_TRG_FUNC_EEPROM_FAIL);
        state_spi1_task &= (unsigned int) (~STATE_TRG_FUNC_EEPROM_GOOD);
        state_spi1_task |= STATE_TRG_FUNC_EEPROM_EMPTY;

        //Виствляємо повідомлення у слові діагностики
        _SET_BIT(clear_diagnostyka, ERROR_TRG_FUNC_EEPROM_BIT);
        _SET_BIT(set_diagnostyka, ERROR_TRG_FUNC_EEPROM_EMPTY_BIT);
      }

      //Знімаємо можливу сигналізацію, що виконувалося порівнняння
      comparison_writing &= (unsigned int) (~COMPARISON_WRITING_TRG_FUNC);
      //Скидаємо повідомлення про читання даних
      _CLEAR_BIT(control_spi1_taskes, TASK_READING_TRG_FUNC_EEPROM_BIT);
    }
    else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_READING_INFO_REJESTRATOR_AR_EEPROM_BIT) != 0)
    {
      //Аналізуємо прочитані дані
      //Спочатку аналізуємо, чи прояитаний блок є пустим, чи вже попередньо записаним
      unsigned int empty_block = 1, i = 0;
      static __INFO_AR_REJESTRATOR info_rejestrator_ar_tmp;

      while ((empty_block != 0) && (i < (sizeof(__INFO_AR_REJESTRATOR) + 1)))
      {
        if (RxBuffer_SPI_EDF[3 + i] != 0xff)
          empty_block = 0;
        i++;
      }

      if (empty_block == 0)
      {
        //Помічаємо, що блок не є пустим
        state_spi1_task &= (unsigned int) (~STATE_INFO_REJESTRATOR_AR_EEPROM_EMPTY);
        //Скидаємо повідомлення у слові діагностики
        _SET_BIT(clear_diagnostyka, ERROR_INFO_REJESTRATOR_AR_EEPROM_EMPTY_BIT);

        //Перевіряємо контрольну суму і переписуємо прочитані дані у структуру
        unsigned char crc_eeprom_info_rejestrator_ar = 0, temp_value;
        unsigned char *point = (unsigned char *) (&info_rejestrator_ar_tmp);
        for (i = 0; i < sizeof(__INFO_AR_REJESTRATOR); i++)
        {
          temp_value = RxBuffer_SPI_EDF[3 + i];
          *(point) = temp_value;
          crc_eeprom_info_rejestrator_ar += temp_value;
          point++;
        }
        if (RxBuffer_SPI_EDF[3 + sizeof(__INFO_AR_REJESTRATOR)] == ((unsigned char) ((~(unsigned int) crc_eeprom_info_rejestrator_ar) & 0xff)))
        {
          //Контролдьна сума сходиться

          //Зберігаємо контрольну суму (не інвертовану)
          crc_info_rejestrator_ar = crc_eeprom_info_rejestrator_ar;

          state_spi1_task &= (unsigned int) (~STATE_INFO_REJESTRATOR_AR_EEPROM_FAIL);
          state_spi1_task |= STATE_INFO_REJESTRATOR_AR_EEPROM_GOOD;

          //Скидаємо повідомлення у слові діагностики
          _SET_BIT(clear_diagnostyka, ERROR_INFO_REJESTRATOR_AR_EEPROM_BIT);

          if ((comparison_writing & COMPARISON_WRITING_INFO_REJESTRATOR_AR) == 0)
          {
            //Виконувалося зчитування інформації по аналоговому реєстратору у робочу структуру

            //Перекидаємо інформації по аналоговому реєстратору з тимчасової структури у робочу структуру
            info_rejestrator_ar = info_rejestrator_ar_tmp;
          }
          else
          {
            //Виконувалося контроль достовірності записаної інформації у EEPROM з записуваною

            unsigned char *point_to_read = (unsigned char *) (&info_rejestrator_ar_tmp);
            unsigned char *point_to_write = (unsigned char *) (&info_rejestrator_ar_comp);
            unsigned int difference = 0;

            i = 0;
            while ((difference == 0) && (i < sizeof(__INFO_AR_REJESTRATOR)))
            {
              if (*point_to_write != *point_to_read)
                difference = 0xff;
              else
              {
                point_to_write++;
                point_to_read++;
                i++;
              }
            }
            if (difference == 0)
            {
              //Контроль порівнняння пройшов успішно

              //Скидаємо повідомлення у слові діагностики
              _SET_BIT(clear_diagnostyka, ERROR_INFO_REJESTRATOR_AR_COMPARISON_BIT);
            }
            else
            {
              //Контроль порівнняння зафіксував розбіжності між записаною і записуваною інформацією

              //Виствляємо повідомлення у слові діагностики
              _SET_BIT(set_diagnostyka, ERROR_INFO_REJESTRATOR_AR_COMPARISON_BIT);
            }
          }
        }
        else
        {
          //Контрольна сума не сходиться
          state_spi1_task &= (unsigned int) (~STATE_INFO_REJESTRATOR_AR_EEPROM_GOOD);
          state_spi1_task |= STATE_INFO_REJESTRATOR_AR_EEPROM_FAIL;

          //Виствляємо повідомлення у слові діагностики
          _SET_BIT(set_diagnostyka, ERROR_INFO_REJESTRATOR_AR_EEPROM_BIT);

          /*
          Виставляємо повідомлення про те, що в EEPROM треба записати нові значення
          структури тільки тоді, коли ми зчитуємо збережені дані для відновлення 
          їх у оперативній пам'яті, а не коли ми проводимо контроль запису.
          Бо для контролю запису нам важливо знати чи успішно записалися дані, які є у 
          оперативній пам'яті і при цьому, навіть, якщо запис відбувся невдало, то,
          оскільки ми працюємо зі змінними з оперативної пам'яті,  які є у нас достовірні,
          бо ми їх якраз і записували, то на роботу до перезавантаження програмного забезперечння 
          збій запису у EEPROM не мав би вплинути
          */
          if ((comparison_writing & COMPARISON_WRITING_INFO_REJESTRATOR_AR) == 0)
          {
            //Виконувалося зчитування інформації по аналоговому реєстратору у робочу структуру

            //Виставляємо команду запису цієї структури у EEPROM
            /*
            Команду виставляємо скоріше, а потім робимо зміни у полях, які треба змінити,
            бо по вимозі проконтролювати достовірність даних інформації по аналоговому
            реєстратору відбувається копіювання з системи захистів структури
            info_rejestrator_ar у резервну мкопію. Це копіювання блокується у випадку 
            "читання з"/"запису в" EEPROM цієї інформації. Тому виставлення спочатку команди
            запису заблокує копіювання.
            З другої сторони не можливо, щоб почався запис до модифікації, 
            бо запис ініціюється функцією main_routines_for_spi1 - в якій ми зараз знаходимося.
            Тобто спочатку треба з цієї функції вийти і при наступних входженнях у цю функцію
            можливе виконання команди яку ми виставили перед зміною даних, яку 
            ми зараз гарантовано зробимо (до виходу з цієї функції)
            */
            _SET_BIT(control_spi1_taskes, TASK_START_WRITE_INFO_REJESTRATOR_AR_EEPROM_BIT);

            //Очищаємо структуру інформації по аналоговому реєстраторі
            info_rejestrator_ar.first_number = -1;
            info_rejestrator_ar.last_number = -1;
            _SET_STATE(FATFS_command, FATFS_FORMAT);
          }
        }
      }
      else
      {
        //Помічаємо, що прочитаний блок є пустим
        state_spi1_task &= (unsigned int) (~(STATE_INFO_REJESTRATOR_AR_EEPROM_FAIL | STATE_INFO_REJESTRATOR_AR_EEPROM_GOOD));
        state_spi1_task |= STATE_INFO_REJESTRATOR_AR_EEPROM_EMPTY;

        //Виствляємо повідомлення у слові діагностики
        _SET_BIT(clear_diagnostyka, ERROR_INFO_REJESTRATOR_AR_EEPROM_BIT);
        _SET_BIT(set_diagnostyka, ERROR_INFO_REJESTRATOR_AR_EEPROM_EMPTY_BIT);

        /*
        Виставляємо повідомлення про те, що в EEPROM треба записати нові значення
        структури тільки тоді, коли ми зчитуємо збережені дані для відновлення 
        їх у оперативній пам'яті, а не коли ми проводимо контроль запису.
        Бо для контролю запису нам важливо знати чи успішно записалися дані, які є у 
        оперативній пам'яті і при цьому, навіть, якщо запис відбувся невдало, то,
        оскільки ми працюємо зі змінними з оперативної пам'яті,  які є у нас достовірні,
        бо ми їх якраз і записували, то на роботу до перезавантаження програмного забезперечння 
        збій запису у EEPROM не мав би вплинути
        */
        if ((comparison_writing & COMPARISON_WRITING_INFO_REJESTRATOR_AR) == 0)
        {
          //Виконувалося зчитування інформації по аналоговому реєстратору у робочу структуру

          //Виставляємо команду запису цієї структури у EEPROM
          /*
          Команду виставляємо скоріше, а потім робимо зміни у полях, які треба змінити,
          бо по вимозі проконтролювати достовірність даних інформації по аналоговому
          реєстратору відбувається копіювання з системи захистів структури
          info_rejestrator_ar у резервну мкопію. Це копіювання блокується у випадку 
          "читання з"/"запису в" EEPROM цієї інформації. Тому виставлення спочатку команди
          запису заблокує копіювання.
          З другої сторони не можливо, щоб почався запис до модифікації, 
          бо запис ініціюється функцією main_routines_for_spi1 - в якій ми зараз знаходимося.
          Тобто спочатку треба з цієї функції вийти і при наступних входженнях у цю функцію
          можливе виконання команди яку ми виставили перед зміною даних, яку 
          ми зараз гарантовано зробимо (до виходу з цієї функції)
          */
          _SET_BIT(control_spi1_taskes, TASK_START_WRITE_INFO_REJESTRATOR_AR_EEPROM_BIT);

          //Очищаємо структуру інформації по аналоговому реєстраторі
          info_rejestrator_ar.first_number = -1;
          info_rejestrator_ar.last_number = -1;
          _SET_STATE(FATFS_command, FATFS_FORMAT);
        }
      }

      //Знімаємо можливу сигналізацію, що виконувалося порівнняння
      comparison_writing &= (unsigned int) (~COMPARISON_WRITING_INFO_REJESTRATOR_AR);
      //Скидаємо повідомлення про читання даних
      _CLEAR_BIT(control_spi1_taskes, TASK_READING_INFO_REJESTRATOR_AR_EEPROM_BIT);
    }
    else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_READING_INFO_REJESTRATOR_DR_EEPROM_BIT) != 0)
    {
      //Аналізуємо прочитані дані
      //Спочатку аналізуємо, чи прояитаний блок є пустим, чи вже попередньо записаним
      unsigned int empty_block = 1, i = 0;
      static __INFO_REJESTRATOR info_rejestrator_dr_tmp;

      while ((empty_block != 0) && (i < (sizeof(__INFO_REJESTRATOR) + 1)))
      {
        if (RxBuffer_SPI_EDF[3 + i] != 0xff)
          empty_block = 0;
        i++;
      }

      if (empty_block == 0)
      {
        //Помічаємо, що блок не є пустим
        state_spi1_task &= (unsigned int) (~STATE_INFO_REJESTRATOR_DR_EEPROM_EMPTY);
        //Скидаємо повідомлення у слові діагностики
        _SET_BIT(clear_diagnostyka, ERROR_INFO_REJESTRATOR_DR_EEPROM_EMPTY_BIT);

        //Перевіряємо контрольну суму і переписуємо прочитані дані у структуру
        unsigned char crc_eeprom_info_rejestrator_dr = 0, temp_value;
        unsigned char *point = (unsigned char *) (&info_rejestrator_dr_tmp);
        for (i = 0; i < sizeof(__INFO_REJESTRATOR); i++)
        {
          temp_value = RxBuffer_SPI_EDF[3 + i];
          *(point) = temp_value;
          crc_eeprom_info_rejestrator_dr += temp_value;
          point++;
        }
        if (RxBuffer_SPI_EDF[3 + sizeof(__INFO_REJESTRATOR)] == ((unsigned char) ((~(unsigned int) crc_eeprom_info_rejestrator_dr) & 0xff)))
        {
          //Контролдьна сума сходиться

          //Зберігаємо контрольну суму (не інвертовану)
          crc_info_rejestrator_dr = crc_eeprom_info_rejestrator_dr;

          state_spi1_task &= (unsigned int) (~STATE_INFO_REJESTRATOR_DR_EEPROM_FAIL);
          state_spi1_task |= STATE_INFO_REJESTRATOR_DR_EEPROM_GOOD;

          //Скидаємо повідомлення у слові діагностики
          _SET_BIT(clear_diagnostyka, ERROR_INFO_REJESTRATOR_DR_EEPROM_BIT);

          if ((comparison_writing & COMPARISON_WRITING_INFO_REJESTRATOR_DR) == 0)
          {
            //Виконувалося зчитування інформації по дискретному реєстратору у робочу структуру

            //Перекидаємо інформації по дискретному реєстратору з тимчасової структури у робочу структуру
            info_rejestrator_dr = info_rejestrator_dr_tmp;

            //Перевіряємо чи всі поляу у своїх допустимих межах
            if (
#if MIN_ADDRESS_DR_AREA != 0
              (info_rejestrator_dr.next_address >= MIN_ADDRESS_DR_AREA) &&
#endif
              (info_rejestrator_dr.next_address <= MAX_ADDRESS_DR_AREA) &&
              (info_rejestrator_dr.number_records <= MAX_NUMBER_RECORDS_INTO_DR))
            {
              //Всі величину мають допустимі значення

              //Перевіряємо, чи у процесі запису останньої аварії не відбувся перезапуск/запуск приладу.
              //Тоді останій запис може бути пошкодженим, якщо вже вся флешка є заповнена
              //Тоді помічаємо, що у нашій флешці на один запис є менше
              if (info_rejestrator_dr.saving_execution != 0)
              {
                //Виставляємо повідомлення про цю подію
                _SET_BIT(set_diagnostyka, ERROR_DR_LOSS_INFORMATION_BIT);

                //Виставляємо команду запису цієї структури у EEPROM
                /*
                Команду виставляємо скоріше, а потім робимо зміни у полях, які треба змінити,
                бо по вимозі проконтролювати достовірність даних інформації по дискреному
                реєстратору відбувається копіювання з системи захистів структури
                info_rejestrator_dr у резервну копію. Це копіювання блокується у випадку 
                "читання з"/"запису в" EEPROM цієї інформації. Тому виставлення спочатку команди
                запису заблокує копіювання.
                З другої сторони не можливо, щоб почався запис до модифікації, 
                бо запис ініціюється функцією main_routines_for_spi1 - в якій ми зараз знаходимося.
                Тобто спочатку треба з цієї функції вийти і при наступних входженнях у цю функцію
                можливе виконання команди яку ми виставили перед зміною даних, яку 
                ми зараз гарантовано зробимо (до виходу з цієї функції)
                */
                _SET_BIT(control_spi1_taskes, TASK_START_WRITE_INFO_REJESTRATOR_DR_EEPROM_BIT);

                info_rejestrator_dr.saving_execution = 0;
                if (info_rejestrator_dr.number_records >= MAX_NUMBER_RECORDS_INTO_DR)
                  info_rejestrator_dr.number_records = (MAX_NUMBER_RECORDS_INTO_DR - 1);
              }
            }
            else
            {
              //Виствляємо повідомлення у слові діагностики
              _SET_BIT(set_diagnostyka, ERROR_INFO_REJESTRATOR_DR_EEPROM_BIT);

              //Виставляємо команду запису цієї структури у EEPROM
              /*
              Команду виставляємо скоріше, а потім робимо зміни у полях, які треба змінити,
              бо по вимозі проконтролювати достовірність даних інформації по дискреному
              реєстратору відбувається копіювання з системи захистів структури
              info_rejestrator_dr у резервну копію. Це копіювання блокується у випадку 
              "читання з"/"запису в" EEPROM цієї інформації. Тому виставлення спочатку команди
              запису заблокує копіювання.
              З другої сторони не можливо, щоб почався запис до модифікації, 
              бо запис ініціюється функцією main_routines_for_spi1 - в якій ми зараз знаходимося.
              Тобто спочатку треба з цієї функції вийти і при наступних входженнях у цю функцію
              можливе виконання команди яку ми виставили перед зміною даних, яку 
              ми зараз гарантовано зробимо (до виходу з цієї функції)
              */
              _SET_BIT(control_spi1_taskes, TASK_START_WRITE_INFO_REJESTRATOR_DR_EEPROM_BIT);

              //Очищаємо структуру інформації по дискретному реєстраторі
              info_rejestrator_dr.next_address = MIN_ADDRESS_DR_AREA;
              info_rejestrator_dr.saving_execution = 0;
              info_rejestrator_dr.number_records = 0;
            }
          }
          else
          {
            //Виконувалося контроль достовірності записаної інформації у EEPROM з записуваною

            unsigned char *point_to_read = (unsigned char *) (&info_rejestrator_dr_tmp);
            unsigned char *point_to_write = (unsigned char *) (&info_rejestrator_dr_comp);
            unsigned int difference = 0;

            i = 0;
            while ((difference == 0) && (i < sizeof(__INFO_REJESTRATOR)))
            {
              if (*point_to_write != *point_to_read)
                difference = 0xff;
              else
              {
                point_to_write++;
                point_to_read++;
                i++;
              }
            }
            if (difference == 0)
            {
              //Контроль порівнняння пройшов успішно

              //Скидаємо повідомлення у слові діагностики
              _SET_BIT(clear_diagnostyka, ERROR_INFO_REJESTRATOR_DR_COMPARISON_BIT);
            }
            else
            {
              //Контроль порівнняння зафіксував розбіжності між записаною і записуваною інформацією

              //Виствляємо повідомлення у слові діагностики
              _SET_BIT(set_diagnostyka, ERROR_INFO_REJESTRATOR_DR_COMPARISON_BIT);
            }
          }
        }
        else
        {
          //Контрольна сума не сходиться
          state_spi1_task &= (unsigned int) (~STATE_INFO_REJESTRATOR_DR_EEPROM_GOOD);
          state_spi1_task |= STATE_INFO_REJESTRATOR_DR_EEPROM_FAIL;

          //Виствляємо повідомлення у слові діагностики
          _SET_BIT(set_diagnostyka, ERROR_INFO_REJESTRATOR_DR_EEPROM_BIT);

          /*
          Виставляємо повідомлення про те, що в EEPROM треба записати нові значення
          структури тільки тоді, коли ми зчитуємо збережені дані для відновлення 
          їх у оперативній пам'яті, а не коли ми проводимо контроль запису.
          Бо для контролю запису нам важливо знати чи успішно записалися дані, які є у 
          оперативній пам'яті і при цьому, навіть, якщо запис відбувся невдало, то,
          оскільки ми працюємо зі змінними з оперативної пам'яті,  які є у нас достовірні,
          бо ми їх якраз і записували, то на роботу до перезавантаження програмного забезперечння 
          збій запису у EEPROM не мав би вплинути
          */
          if ((comparison_writing & COMPARISON_WRITING_INFO_REJESTRATOR_DR) == 0)
          {
            //Виконувалося зчитування інформації по дискретному реєстратору у робочу структуру

            //Виставляємо команду запису цієї структури у EEPROM
            /*
            Команду виставляємо скоріше, а потім робимо зміни у полях, які треба змінити,
            бо по вимозі проконтролювати достовірність даних інформації по дискреному
            реєстратору відбувається копіювання з системи захистів структури
            info_rejestrator_dr у резервну копію. Це копіювання блокується у випадку 
            "читання з"/"запису в" EEPROM цієї інформації. Тому виставлення спочатку команди
            запису заблокує копіювання.
            З другої сторони не можливо, щоб почався запис до модифікації, 
            бо запис ініціюється функцією main_routines_for_spi1 - в якій ми зараз знаходимося.
            Тобто спочатку треба з цієї функції вийти і при наступних входженнях у цю функцію
            можливе виконання команди яку ми виставили перед зміною даних, яку 
            ми зараз гарантовано зробимо (до виходу з цієї функції)
            */
            _SET_BIT(control_spi1_taskes, TASK_START_WRITE_INFO_REJESTRATOR_DR_EEPROM_BIT);

            //Очищаємо структуру інформації по дискретному реєстраторі
            info_rejestrator_dr.next_address = MIN_ADDRESS_DR_AREA;
            info_rejestrator_dr.saving_execution = 0;
            info_rejestrator_dr.number_records = 0;
          }
        }
      }
      else
      {
        //Помічаємо, що прочитаний блок є пустим
        state_spi1_task &= (unsigned int) (~(STATE_INFO_REJESTRATOR_DR_EEPROM_FAIL | STATE_INFO_REJESTRATOR_DR_EEPROM_GOOD));
        state_spi1_task |= STATE_INFO_REJESTRATOR_DR_EEPROM_EMPTY;

        //Виствляємо повідомлення у слові діагностики
        _SET_BIT(clear_diagnostyka, ERROR_INFO_REJESTRATOR_DR_EEPROM_BIT);
        _SET_BIT(set_diagnostyka, ERROR_INFO_REJESTRATOR_DR_EEPROM_EMPTY_BIT);

        /*
        Виставляємо повідомлення про те, що в EEPROM треба записати нові значення
        структури тільки тоді, коли ми зчитуємо збережені дані для відновлення 
        їх у оперативній пам'яті, а не коли ми проводимо контроль запису.
        Бо для контролю запису нам важливо знати чи успішно записалися дані, які є у 
        оперативній пам'яті і при цьому, навіть, якщо запис відбувся невдало, то,
        оскільки ми працюємо зі змінними з оперативної пам'яті,  які є у нас достовірні,
        бо ми їх якраз і записували, то на роботу до перезавантаження програмного забезперечння 
        збій запису у EEPROM не мав би вплинути
        */
        if ((comparison_writing & COMPARISON_WRITING_INFO_REJESTRATOR_DR) == 0)
        {
          //Виконувалося зчитування інформації по дискретному реєстратору у робочу структуру

          //Виставляємо команду запису цієї структури у EEPROM
          /*
          Команду виставляємо скоріше, а потім робимо зміни у полях, які треба змінити,
          бо по вимозі проконтролювати достовірність даних інформації по аналоговому
          реєстратору відбувається копіювання з системи захистів структури
          info_rejestrator_ar у резервну копію. Це копіювання блокується у випадку 
          "читання з"/"запису в" EEPROM цієї інформації. Тому виставлення спочатку команди
          запису заблокує копіювання.
          З другої сторони не можливо, щоб почався запис до модифікації, 
          бо запис ініціюється функцією main_routines_for_spi1 - в якій ми зараз знаходимося.
          Тобто спочатку треба з цієї функції вийти і при наступних входженнях у цю функцію
          можливе виконання команди яку ми виставили перед зміною даних, яку 
          ми зараз гарантовано зробимо (до виходу з цієї функції)
          */
          _SET_BIT(control_spi1_taskes, TASK_START_WRITE_INFO_REJESTRATOR_DR_EEPROM_BIT);

          //Очищаємо структуру інформації по дискретному реєстраторі
          info_rejestrator_dr.next_address = MIN_ADDRESS_DR_AREA;
          info_rejestrator_dr.saving_execution = 0;
          info_rejestrator_dr.number_records = 0;
        }
      }

      //Знімаємо можливу сигналізацію, що виконувалося порівнняння
      comparison_writing &= (unsigned int) (~COMPARISON_WRITING_INFO_REJESTRATOR_DR);
      //Скидаємо повідомлення про читання даних
      _CLEAR_BIT(control_spi1_taskes, TASK_READING_INFO_REJESTRATOR_DR_EEPROM_BIT);
    }
    else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_READING_INFO_REJESTRATOR_PR_ERR_EEPROM_BIT) != 0)
    {
      //Аналізуємо прочитані дані
      //Спочатку аналізуємо, чи прояитаний блок є пустим, чи вже попередньо записаним
      unsigned int empty_block = 1, i = 0;
      static __INFO_REJESTRATOR info_rejestrator_pr_err_tmp;

      while ((empty_block != 0) && (i < (sizeof(__INFO_REJESTRATOR) + 1)))
      {
        if (RxBuffer_SPI_EDF[3 + i] != 0xff)
          empty_block = 0;
        i++;
      }

      if (empty_block == 0)
      {
        //Помічаємо, що блок не є пустим
        state_spi1_task &= (unsigned int) (~STATE_INFO_REJESTRATOR_PR_ERR_EEPROM_EMPTY);
        //Скидаємо повідомлення у слові діагностики
        _SET_BIT(clear_diagnostyka, ERROR_INFO_REJESTRATOR_PR_ERR_EEPROM_EMPTY_BIT);

        //Перевіряємо контрольну суму і переписуємо прочитані дані у структуру
        unsigned char crc_eeprom_info_rejestrator_pr_err = 0, temp_value;
        unsigned char *point = (unsigned char *) (&info_rejestrator_pr_err_tmp);
        for (i = 0; i < sizeof(__INFO_REJESTRATOR); i++)
        {
          temp_value = RxBuffer_SPI_EDF[3 + i];
          *(point) = temp_value;
          crc_eeprom_info_rejestrator_pr_err += temp_value;
          point++;
        }
        if (RxBuffer_SPI_EDF[3 + sizeof(__INFO_REJESTRATOR)] == ((unsigned char) ((~(unsigned int) crc_eeprom_info_rejestrator_pr_err) & 0xff)))
        {
          //Контролдьна сума сходиться

          //Зберігаємо контрольну суму (не інвертовану)
          crc_info_rejestrator_pr_err = crc_eeprom_info_rejestrator_pr_err;

          state_spi1_task &= (unsigned int) (~STATE_INFO_REJESTRATOR_PR_ERR_EEPROM_FAIL);
          state_spi1_task |= STATE_INFO_REJESTRATOR_PR_ERR_EEPROM_GOOD;

          //Скидаємо повідомлення у слові діагностики
          _SET_BIT(clear_diagnostyka, ERROR_INFO_REJESTRATOR_PR_ERR_EEPROM_BIT);

          if ((comparison_writing & COMPARISON_WRITING_INFO_REJESTRATOR_PR_ERR) == 0)
          {
            //Виконувалося зчитування інформації по реєстратору програмних подій у робочу структуру

            //Перекидаємо інформації по реєстратору програмних подій з тимчасової структури у робочу структуру
            info_rejestrator_pr_err = info_rejestrator_pr_err_tmp;

            //Перевіряємо чи всі поляу у своїх допустимих межах
            if (
              (info_rejestrator_pr_err.next_address >= MIN_ADDRESS_PR_ERR_AREA) &&
              (info_rejestrator_pr_err.next_address <= MAX_ADDRESS_PR_ERR_AREA) &&
              (info_rejestrator_pr_err.number_records <= MAX_NUMBER_RECORDS_INTO_PR_ERR))
            {
              //Всі величину мають допустимі значення

              //Перевіряємо, чи у процесі запису останньої аварії не відбувся перезапуск/запуск приладу.
              //Тоді останій запис може бути пошкодженим, якщо вже свя флешка є заповнена
              //Тоді помічаємо, що у нашій флешці на один запис є менше
              //Перевіряємо, чи у процесі запису програмної події не відбувся перезапуск/запуск приладу.
              //Тоді записи у сторінці, яку записували може бути пошкодженими
              if (info_rejestrator_pr_err.saving_execution != 0)
              {
                //Виставляємо повідомлення про цю подію
                _SET_BIT(set_diagnostyka, ERROR_PR_ERR_LOSS_INFORMATION_BIT);

                //Виставляємо команду запису цієї структури у EEPROM
                /*
                Команду виставляємо скоріше, а потім робимо зміни у полях, які треба змінити,
                бо по вимозі проконтролювати достовірність даних інформації по реєстратору
                програмних подій відбувається копіювання з переривання обслуговування DataFlash структури
                info_rejestrator_pr_err у резервну копію. Це копіювання блокується у випадку 
                "читання з"/"запису в" EEPROM цієї інформації. Тому виставлення спочатку команди
                запису заблокує копіювання.
                З другої сторони не можливо, щоб почався запис до модифікації, 
                бо запис ініціюється функцією main_routines_for_spi1 - в якій ми зараз знаходимося.
                Тобто спочатку треба з цієї функції вийти і при наступних входженнях у цю функцію
                можливе виконання команди, яку ми виставили перед зміною даних, яку 
                ми зараз гарантовано зробимо (до виходу з цієї функції)
                */

                unsigned int number_records = info_rejestrator_pr_err.number_records;
                if (number_records < MAX_NUMBER_RECORDS_INTO_PR_ERR)
                {
                  unsigned int cut_record = (info_rejestrator_pr_err.next_address & (SIZE_PAGE_DATAFLASH_1 - 1)) >> VAGA_SIZE_ONE_RECORD_PR_ERR;
                  number_records = (number_records > cut_record) ? (number_records - cut_record) : 0;
                }
                else
                {
                  number_records -= 1u << (VAGA_SIZE_PAGE_DATAFLASH_1 - VAGA_SIZE_ONE_RECORD_PR_ERR);
                }

                _SET_BIT(control_spi1_taskes, TASK_START_WRITE_INFO_REJESTRATOR_PR_ERR_EEPROM_BIT);

                info_rejestrator_pr_err.next_address &= ~(SIZE_PAGE_DATAFLASH_1 - 1);
                info_rejestrator_pr_err.saving_execution = 0;
                info_rejestrator_pr_err.number_records = number_records;
              }
            }
            else
            {
              //Виствляємо повідомлення у слові діагностики
              _SET_BIT(set_diagnostyka, ERROR_INFO_REJESTRATOR_PR_ERR_EEPROM_BIT);

              //Виставляємо команду запису цієї структури у EEPROM
              /*
              Команду виставляємо скоріше, а потім робимо зміни у полях, які треба змінити,
              бо по вимозі проконтролювати достовірність даних інформації по реєстратору
              програмних подій відбувається копіювання з переривання обслуговування DataFlash структури
              info_rejestrator_pr_err у резервну копію. Це копіювання блокується у випадку 
              "читання з"/"запису в" EEPROM цієї інформації. Тому виставлення спочатку команди
              запису заблокує копіювання.
              З другої сторони не можливо, щоб почався запис до модифікації, 
              бо запис ініціюється функцією main_routines_for_spi1 - в якій ми зараз знаходимося.
              Тобто спочатку треба з цієї функції вийти і при наступних входженнях у цю функцію
              можливе виконання команди, яку ми виставили перед зміною даних, яку 
              ми зараз гарантовано зробимо (до виходу з цієї функції)
              */
              _SET_BIT(control_spi1_taskes, TASK_START_WRITE_INFO_REJESTRATOR_PR_ERR_EEPROM_BIT);

              //Очищаємо структуру інформації по реєстраторі програмних подій
              info_rejestrator_pr_err.next_address = MIN_ADDRESS_PR_ERR_AREA;
              info_rejestrator_pr_err.saving_execution = 0;
              info_rejestrator_pr_err.number_records = 0;
            }
          }
          else
          {
            //Виконувалося контроль достовірності записаної інформації у EEPROM з записуваною

            unsigned char *point_to_read = (unsigned char *) (&info_rejestrator_pr_err_tmp);
            unsigned char *point_to_write = (unsigned char *) (&info_rejestrator_pr_err_comp);
            unsigned int difference = 0;

            i = 0;
            while ((difference == 0) && (i < sizeof(__INFO_REJESTRATOR)))
            {
              if (*point_to_write != *point_to_read)
                difference = 0xff;
              else
              {
                point_to_write++;
                point_to_read++;
                i++;
              }
            }
            if (difference == 0)
            {
              //Контроль порівнняння пройшов успішно

              //Скидаємо повідомлення у слові діагностики
              _SET_BIT(clear_diagnostyka, ERROR_INFO_REJESTRATOR_PR_ERR_COMPARISON_BIT);
            }
            else
            {
              //Контроль порівнняння зафіксував розбіжності між записаною і записуваною інформацією

              //Виствляємо повідомлення у слові діагностики
              _SET_BIT(set_diagnostyka, ERROR_INFO_REJESTRATOR_PR_ERR_COMPARISON_BIT);
            }
          }
        }
        else
        {
          //Контрольна сума не сходиться
          state_spi1_task &= (unsigned int) (~STATE_INFO_REJESTRATOR_PR_ERR_EEPROM_GOOD);
          state_spi1_task |= STATE_INFO_REJESTRATOR_PR_ERR_EEPROM_FAIL;

          //Виствляємо повідомлення у слові діагностики
          _SET_BIT(set_diagnostyka, ERROR_INFO_REJESTRATOR_PR_ERR_EEPROM_BIT);

          /*
          Виставляємо повідомлення про те, що в EEPROM треба записати нові значення
          структури тільки тоді, коли ми зчитуємо збережені дані для відновлення 
          їх у оперативній пам'яті, а не коли ми проводимо контроль запису.
          Бо для контролю запису нам важливо знати чи успішно записалися дані, які є у 
          оперативній пам'яті і при цьому, навіть, якщо запис відбувся невдало, то,
          оскільки ми працюємо зі змінними з оперативної пам'яті,  які є у нас достовірні,
          бо ми їх якраз і записували, то на роботу до перезавантаження програмного забезперечння 
          збій запису у EEPROM не мав би вплинути
          */
          if ((comparison_writing & COMPARISON_WRITING_INFO_REJESTRATOR_PR_ERR) == 0)
          {
            //Виконувалося зчитування інформації по дискретному реєстратору у робочу структуру

            //Виставляємо команду запису цієї структури у EEPROM
            /*
            Команду виставляємо скоріше, а потім робимо зміни у полях, які треба змінити,
            бо по вимозі проконтролювати достовірність даних інформації по реєстратору
            програмних подій відбувається копіювання з переривання обслуговування DataFlash структури
            info_rejestrator_pr_err у резервну копію. Це копіювання блокується у випадку 
            "читання з"/"запису в" EEPROM цієї інформації. Тому виставлення спочатку команди
            запису заблокує копіювання.
            З другої сторони не можливо, щоб почався запис до модифікації, 
            бо запис ініціюється функцією main_routines_for_spi1 - в якій ми зараз знаходимося.
            Тобто спочатку треба з цієї функції вийти і при наступних входженнях у цю функцію
            можливе виконання команди, яку ми виставили перед зміною даних, яку 
            ми зараз гарантовано зробимо (до виходу з цієї функції)
            */
            _SET_BIT(control_spi1_taskes, TASK_START_WRITE_INFO_REJESTRATOR_PR_ERR_EEPROM_BIT);

            //Очищаємо структуру інформації по реєстраторі програмних подій
            info_rejestrator_pr_err.next_address = MIN_ADDRESS_PR_ERR_AREA;
            info_rejestrator_pr_err.saving_execution = 0;
            info_rejestrator_pr_err.number_records = 0;
          }
        }
      }
      else
      {
        //Помічаємо, що прочитаний блок є пустим
        state_spi1_task &= (unsigned int) (~(STATE_INFO_REJESTRATOR_PR_ERR_EEPROM_FAIL | STATE_INFO_REJESTRATOR_PR_ERR_EEPROM_GOOD));
        state_spi1_task |= STATE_INFO_REJESTRATOR_PR_ERR_EEPROM_EMPTY;

        //Виствляємо повідомлення у слові діагностики
        _SET_BIT(clear_diagnostyka, ERROR_INFO_REJESTRATOR_PR_ERR_EEPROM_BIT);
        _SET_BIT(set_diagnostyka, ERROR_INFO_REJESTRATOR_PR_ERR_EEPROM_EMPTY_BIT);

        /*
        Виставляємо повідомлення про те, що в EEPROM треба записати нові значення
        структури тільки тоді, коли ми зчитуємо збережені дані для відновлення 
        їх у оперативній пам'яті, а не коли ми проводимо контроль запису.
        Бо для контролю запису нам важливо знати чи успішно записалися дані, які є у 
        оперативній пам'яті і при цьому, навіть, якщо запис відбувся невдало, то,
        оскільки ми працюємо зі змінними з оперативної пам'яті,  які є у нас достовірні,
        бо ми їх якраз і записували, то на роботу до перезавантаження програмного забезперечння 
        збій запису у EEPROM не мав би вплинути
        */
        if ((comparison_writing & COMPARISON_WRITING_INFO_REJESTRATOR_PR_ERR) == 0)
        {
          //Виконувалося зчитування інформації по дискретному реєстратору у робочу структуру

          //Виставляємо команду запису цієї структури у EEPROM
          /*
          Команду виставляємо скоріше, а потім робимо зміни у полях, які треба змінити,
          бо по вимозі проконтролювати достовірність даних інформації по реєстратору
          програмних подій відбувається копіювання з переривання обслуговування DataFlash структури
          info_rejestrator_pr_err у резервну копію. Це копіювання блокується у випадку 
          "читання з"/"запису в" EEPROM цієї інформації. Тому виставлення спочатку команди
          запису заблокує копіювання.
          З другої сторони не можливо, щоб почався запис до модифікації, 
          бо запис ініціюється функцією main_routines_for_spi1 - в якій ми зараз знаходимося.
          Тобто спочатку треба з цієї функції вийти і при наступних входженнях у цю функцію
          можливе виконання команди, яку ми виставили перед зміною даних, яку 
          ми зараз гарантовано зробимо (до виходу з цієї функції)
          */
          _SET_BIT(control_spi1_taskes, TASK_START_WRITE_INFO_REJESTRATOR_PR_ERR_EEPROM_BIT);

          //Очищаємо структуру інформації по реєстраторі програмних подій
          info_rejestrator_pr_err.next_address = MIN_ADDRESS_PR_ERR_AREA;
          info_rejestrator_pr_err.saving_execution = 0;
          info_rejestrator_pr_err.number_records = 0;
        }
      }

      //Знімаємо можливу сигналізацію, що виконувалося порівнняння
      comparison_writing &= (unsigned int) (~COMPARISON_WRITING_INFO_REJESTRATOR_PR_ERR);
      //Скидаємо повідомлення про читання даних
      _CLEAR_BIT(control_spi1_taskes, TASK_READING_INFO_REJESTRATOR_PR_ERR_EEPROM_BIT);
    }
    else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_READING_ANGLE_EEPROM_BIT) != 0)
    {
      //Аналізуємо прочитані дані
      //Спочатку аналізуємо, чи прояитаний блок є пустим, чи вже попередньо записаним
      unsigned int empty_block = 1, i = 0;
      unsigned int sum_phi_begin_tmp = 0, sum_phi_end_tmp = 0;

      while ((empty_block != 0) && (i < (sizeof(sum_phi_begin) + sizeof(sum_phi_end) + 1)))
      {
        if (RxBuffer_SPI_EDF[3 + i] != 0xff)
          empty_block = 0;
        i++;
      }

      if (empty_block == 0)
      {
        //Помічаємо, що блок не є пустим
        state_spi1_task &= (unsigned int) (~STATE_ANGLE_EEPROM_EMPTY);
        //Скидаємо повідомлення у слові діагностики
        _SET_BIT(clear_diagnostyka, ERROR_ANGLE_EEPROM_EMPTY_BIT);

        //Перевіряємо контрольну суму
        unsigned char crc_eeprom_angle = 0, temp_value;
        unsigned char *point;
        unsigned int offset = 3;

        point = (unsigned char *) (&sum_phi_begin_tmp);
        for (i = 0; i < sizeof(sum_phi_begin_tmp); i++)
        {
          temp_value = RxBuffer_SPI_EDF[offset + i];
          *(point) = temp_value;
          crc_eeprom_angle += temp_value;
          point++;
        }
        offset += sizeof(sum_phi_begin_tmp);

        point = (unsigned char *) (&sum_phi_end_tmp);
        for (i = 0; i < sizeof(sum_phi_end_tmp); i++)
        {
          temp_value = RxBuffer_SPI_EDF[offset + i];
          *(point) = temp_value;
          crc_eeprom_angle += temp_value;
          point++;
        }
        offset += sizeof(sum_phi_end_tmp);

        if (RxBuffer_SPI_EDF[offset] == ((unsigned char) ((~(unsigned int) crc_eeprom_angle) & 0xff)))
        {
          //Контролдьна сума сходиться

          //Скидаємо повідомлення у слові діагностики
          _SET_BIT(clear_diagnostyka, ERROR_ANGLE_EEPROM_BIT);

          crc_angle = crc_eeprom_angle;

          if ((comparison_writing & COMPARISON_WRITING_ANGLE) == 0)
          {
            sum_phi_begin = sum_phi_begin_tmp;
            sum_phi_end = sum_phi_end_tmp;
          }
          else
          {
            //Виконувалося контроль достовірності записаної інформації у EEPROM з записуваною
            if ((sum_phi_begin_comp == sum_phi_begin_tmp) && (sum_phi_end_comp == sum_phi_end_tmp))
            {
              //Контроль порівнняння пройшов успішно

              //Скидаємо повідомлення у слові діагностики
              _SET_BIT(clear_diagnostyka, ERROR_ANGLE_EEPROM_COMPARISON_BIT);
            }
            else
            {
              //Контроль порівнняння зафіксував розбіжності між записаною і записуваною інформацією

              //Виствляємо повідомлення у слові діагностики
              _SET_BIT(set_diagnostyka, ERROR_ANGLE_EEPROM_COMPARISON_BIT);
            }
          }

          state_spi1_task &= (unsigned int) (~STATE_ANGLE_EEPROM_FAIL);
          state_spi1_task |= STATE_ANGLE_EEPROM_GOOD;
        }
        else
        {
          //Контрольна сума не сходиться
          state_spi1_task &= (unsigned int) (~STATE_ANGLE_EEPROM_GOOD);
          state_spi1_task |= STATE_ANGLE_EEPROM_FAIL;

          //Виствляємо повідомлення у слові діагностики
          _SET_BIT(set_diagnostyka, ERROR_ANGLE_EEPROM_BIT);
        }
      }
      else
      {
        //Помічаємо, що прочитаний блок є пустим
        state_spi1_task &= (unsigned int) (~STATE_ANGLE_EEPROM_FAIL);
        state_spi1_task &= (unsigned int) (~STATE_ANGLE_EEPROM_GOOD);
        state_spi1_task |= STATE_ANGLE_EEPROM_EMPTY;

        //Виствляємо повідомлення у слові діагностики
        _SET_BIT(clear_diagnostyka, ERROR_ANGLE_EEPROM_BIT);
        _SET_BIT(set_diagnostyka, ERROR_ANGLE_EEPROM_EMPTY_BIT);
      }

      //Знімаємо можливу сигналізацію, що виконувалося порівнняння
      comparison_writing &= (unsigned int) (~COMPARISON_WRITING_ANGLE);
      //Скидаємо повідомлення про читання даних
      _CLEAR_BIT(control_spi1_taskes, TASK_READING_ANGLE_EEPROM_BIT);
    }
    else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_READING_RESURS_EEPROM_BIT) != 0)
    {
      //Аналізуємо прочитані дані
      //Спочатку аналізуємо, чи прояитаний блок є пустим, чи вже попередньо записаним
      unsigned int empty_block = 1, i = 0;
      __COUNTER_RESURS counter_today_tmp = {
        .date = {0, 0, 0},
        .count = 0};
      __COUNTER_RESURS counter_previous_day_tmp = {
        .date = {0, 0, 0},
        .count = 0};
      unsigned int counter_total_tmp = 0;

      while ((empty_block != 0) && (i < (sizeof(counter_today) + sizeof(counter_previous_day) + sizeof(counter_total) + 1)))
      {
        if (RxBuffer_SPI_EDF[3 + i] != 0xff)
          empty_block = 0;
        i++;
      }

      if (empty_block == 0)
      {
        //Помічаємо, що блок не є пустим
        state_spi1_task &= (unsigned int) (~STATE_RESURS_EEPROM_EMPTY);
        //Скидаємо повідомлення у слові діагностики
        _SET_BIT(clear_diagnostyka, ERROR_RESURS_EEPROM_EMPTY_BIT);

        //Перевіряємо контрольну суму
        unsigned char crc_eeprom_resurs = 0, temp_value;
        unsigned char *point;
        unsigned int offset = 3;

        point = (unsigned char *) (&counter_today_tmp);
        for (i = 0; i < sizeof(counter_today_tmp); i++)
        {
          temp_value = RxBuffer_SPI_EDF[offset + i];
          *(point) = temp_value;
          crc_eeprom_resurs += temp_value;
          point++;
        }
        offset += sizeof(counter_today_tmp);

        point = (unsigned char *) (&counter_previous_day_tmp);
        for (i = 0; i < sizeof(counter_previous_day_tmp); i++)
        {
          temp_value = RxBuffer_SPI_EDF[offset + i];
          *(point) = temp_value;
          crc_eeprom_resurs += temp_value;
          point++;
        }
        offset += sizeof(counter_previous_day_tmp);

        point = (unsigned char *) (&counter_total_tmp);
        for (i = 0; i < sizeof(counter_total_tmp); i++)
        {
          temp_value = RxBuffer_SPI_EDF[offset + i];
          *(point) = temp_value;
          crc_eeprom_resurs += temp_value;
          point++;
        }
        offset += sizeof(counter_total_tmp);

        if (RxBuffer_SPI_EDF[offset] == ((unsigned char) ((~(unsigned int) crc_eeprom_resurs) & 0xff)))
        {
          //Контролдьна сума сходиться

          //Скидаємо повідомлення у слові діагностики
          _SET_BIT(clear_diagnostyka, ERROR_RESURS_EEPROM_BIT);

          crc_resurs = crc_eeprom_resurs;

          if ((comparison_writing & COMPARISON_WRITING_RESURS) == 0)
          {
            counter_today = counter_today_tmp;
            counter_previous_day = counter_previous_day_tmp;
            counter_total = counter_total_tmp;
          }
          else
          {
            //Виконувалося контроль достовірності записаної інформації у EEPROM з записуваною
            unsigned int target = 0;
            unsigned int difference = 0;
            while ((target < 3) && (difference == 0))
            {
              unsigned char *point_to_read = NULL;
              unsigned char *point_to_write = NULL;
              unsigned int size_of_target = 0;

              switch (target)
              {
                case 0:
                  {
                    point_to_read = (unsigned char *) (&counter_today_tmp);
                    point_to_write = (unsigned char *) (&counter_today_comp);
                    size_of_target = sizeof(counter_today);
                    break;
                  }
                case 1:
                  {
                    point_to_read = (unsigned char *) (&counter_previous_day_tmp);
                    point_to_write = (unsigned char *) (&counter_previous_day_comp);
                    size_of_target = sizeof(counter_previous_day);
                    break;
                  }
                case 2:
                  {
                    point_to_read = (unsigned char *) (&counter_total_tmp);
                    point_to_write = (unsigned char *) (&counter_total_comp);
                    size_of_target = sizeof(counter_total);
                    break;
                  }
                default:
                  {
                    //Теоретично цього ніколи не мало б бути
                    total_error_sw_fixed();
                  }
              }

              i = 0;
              while ((difference == 0) && (i < size_of_target))
              {
                if (*point_to_write != *point_to_read)
                  difference = 0xff;
                else
                {
                  point_to_write++;
                  point_to_read++;
                  i++;
                }
              }

              if (difference == 0)
                target++;
            }

            if (difference == 0)
            {
              //Контроль порівнняння пройшов успішно

              //Скидаємо повідомлення у слові діагностики
              _SET_BIT(clear_diagnostyka, ERROR_RESURS_EEPROM_COMPARISON_BIT);
            }
            else
            {
              //Контроль порівнняння зафіксував розбіжності між записаною і записуваною інформацією

              //Виствляємо повідомлення у слові діагностики
              _SET_BIT(set_diagnostyka, ERROR_RESURS_EEPROM_COMPARISON_BIT);
            }
          }

          state_spi1_task &= (unsigned int) (~STATE_RESURS_EEPROM_FAIL);
          state_spi1_task |= STATE_RESURS_EEPROM_GOOD;
        }
        else
        {
          //Контрольна сума не сходиться
          state_spi1_task &= (unsigned int) (~STATE_RESURS_EEPROM_GOOD);
          state_spi1_task |= STATE_RESURS_EEPROM_FAIL;

          //Виствляємо повідомлення у слові діагностики
          _SET_BIT(set_diagnostyka, ERROR_RESURS_EEPROM_BIT);
        }
      }
      else
      {
        //Помічаємо, що прочитаний блок є пустим
        state_spi1_task &= (unsigned int) (~STATE_RESURS_EEPROM_FAIL);
        state_spi1_task &= (unsigned int) (~STATE_RESURS_EEPROM_GOOD);
        state_spi1_task |= STATE_RESURS_EEPROM_EMPTY;

        //Виствляємо повідомлення у слові діагностики
        _SET_BIT(clear_diagnostyka, ERROR_RESURS_EEPROM_BIT);
        _SET_BIT(set_diagnostyka, ERROR_RESURS_EEPROM_EMPTY_BIT);
      }

      //Знімаємо можливу сигналізацію, що виконувалося порівнняння
      comparison_writing &= (unsigned int) (~COMPARISON_WRITING_RESURS);
      //Скидаємо повідомлення про читання даних
      _CLEAR_BIT(control_spi1_taskes, TASK_READING_RESURS_EEPROM_BIT);
    }
    else
    {
      //Сюди, теоретично, ніколи не мала б дійти
      //У всіх інакших випадках нічого не робимо
    }
    /*************************************************/
  }
  else if (state_execution_spi1 == 2)
  {
    /*************************************************/
    //Обмін завершився з помилкою
    /*************************************************/

    //Виставляємо повідомлення, що SPI1 готовий до нової транзакції
    state_execution_spi1 = -1;

    //Визначаємося з наступними діями
    if (_CHECK_SET_BIT(control_spi1_taskes, TASK_EEPROM_WRITE_PREPARATION_BIT) != 0)
    {
      //Переходимо на початок цієї процедури
      etap_eeprom_write_enable = -1;
    }
    else if (
      (_CHECK_SET_BIT(control_spi1_taskes, TASK_WRITING_SETTINGS_EEPROM_BIT) != 0) ||
      (_CHECK_SET_BIT(control_spi1_taskes, TASK_WRITING_USTUVANNJA_EEPROM_BIT) != 0) ||
      (_CHECK_SET_BIT(control_spi1_taskes, TASK_WRITING_STATE_LEDS_OUTPUTS_EEPROM_BIT) != 0) ||
      (_CHECK_SET_BIT(control_spi1_taskes, TASK_WRITING_TRG_FUNC_EEPROM_BIT) != 0) ||
      (_CHECK_SET_BIT(control_spi1_taskes, TASK_WRITING_INFO_REJESTRATOR_AR_EEPROM_BIT) != 0) ||
      (_CHECK_SET_BIT(control_spi1_taskes, TASK_WRITING_INFO_REJESTRATOR_DR_EEPROM_BIT) != 0) ||
      (_CHECK_SET_BIT(control_spi1_taskes, TASK_WRITING_INFO_REJESTRATOR_PR_ERR_EEPROM_BIT) != 0) ||
      (_CHECK_SET_BIT(control_spi1_taskes, TASK_WRITING_ANGLE_EEPROM_BIT) != 0) ||
      (_CHECK_SET_BIT(control_spi1_taskes, TASK_WRITING_RESURS_EEPROM_BIT) != 0))
    {
      //Стоїть умова запису блоку у EEPROM
      //Залишаємо номер блоку для запису в EEPROM той самий, бо немає підтвердження, що він записався
      //Виставляємо біт встановлення дозволу на запис
      _SET_BIT(control_spi1_taskes, TASK_EEPROM_WRITE_PREPARATION_BIT);
    }
    else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_READING_SETTINGS_EEPROM_BIT) != 0)
    {
      //Стоїть умова читання блоку у EEPROM настройок

      //Повторно запускаємо процес читання
      _SET_BIT(control_spi1_taskes, TASK_START_READ_SETTINGS_EEPROM_BIT);
      _CLEAR_BIT(control_spi1_taskes, TASK_READING_SETTINGS_EEPROM_BIT);
    }
    else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_READING_USTUVANNJA_EEPROM_BIT) != 0)
    {
      //Стоїть умова читання блоку у EEPROM юстування

      //Повторно запускаємо процес читання
      _SET_BIT(control_spi1_taskes, TASK_START_READ_USTUVANNJA_EEPROM_BIT);
      _CLEAR_BIT(control_spi1_taskes, TASK_READING_USTUVANNJA_EEPROM_BIT);
    }
    else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_READING_STATE_LEDS_OUTPUTS_EEPROM_BIT) != 0)
    {
      //Стоїть умова читання блоку у EEPROM стану тригерних світлоіндикаторів і сигнальних виходів

      //Повторно запускаємо процес читання
      _SET_BIT(control_spi1_taskes, TASK_START_READ_STATE_LEDS_OUTPUTS_EEPROM_BIT);
      _CLEAR_BIT(control_spi1_taskes, TASK_READING_STATE_LEDS_OUTPUTS_EEPROM_BIT);
    }
    else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_READING_TRG_FUNC_EEPROM_BIT) != 0)
    {
      //Стоїть умова читання блоку у EEPROM стану тригерної інформації

      //Повторно запускаємо процес читання
      _SET_BIT(control_spi1_taskes, TASK_START_READ_TRG_FUNC_EEPROM_BIT);
      _CLEAR_BIT(control_spi1_taskes, TASK_READING_TRG_FUNC_EEPROM_BIT);
    }
    else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_READING_INFO_REJESTRATOR_AR_EEPROM_BIT) != 0)
    {
      //Стоїть умова читання блоку у EEPROM по аналоговому реєстраторі

      //Повторно запускаємо процес читання
      _SET_BIT(control_spi1_taskes, TASK_START_READ_INFO_REJESTRATOR_AR_EEPROM_BIT);
      _CLEAR_BIT(control_spi1_taskes, TASK_READING_INFO_REJESTRATOR_AR_EEPROM_BIT);
    }
    else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_READING_INFO_REJESTRATOR_DR_EEPROM_BIT) != 0)
    {
      //Стоїть умова читання блоку у EEPROM по дискретному реєстраторі

      //Повторно запускаємо процес читання
      _SET_BIT(control_spi1_taskes, TASK_START_READ_INFO_REJESTRATOR_DR_EEPROM_BIT);
      _CLEAR_BIT(control_spi1_taskes, TASK_READING_INFO_REJESTRATOR_DR_EEPROM_BIT);
    }
    else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_READING_INFO_REJESTRATOR_PR_ERR_EEPROM_BIT) != 0)
    {
      //Стоїть умова читання блоку у EEPROM по реєстраторі програмнихподій

      //Повторно запускаємо процес читання
      _SET_BIT(control_spi1_taskes, TASK_START_READ_INFO_REJESTRATOR_PR_ERR_EEPROM_BIT);
      _CLEAR_BIT(control_spi1_taskes, TASK_READING_INFO_REJESTRATOR_PR_ERR_EEPROM_BIT);
    }
    else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_READING_ANGLE_EEPROM_BIT) != 0)
    {
      _SET_BIT(control_spi1_taskes, TASK_START_READ_ANGLE_EEPROM_BIT);
      _CLEAR_BIT(control_spi1_taskes, TASK_READING_ANGLE_EEPROM_BIT);
    }
    else if (_CHECK_SET_BIT(control_spi1_taskes, TASK_READING_RESURS_EEPROM_BIT) != 0)
    {
      _SET_BIT(control_spi1_taskes, TASK_START_READ_RESURS_EEPROM_BIT);
      _CLEAR_BIT(control_spi1_taskes, TASK_READING_RESURS_EEPROM_BIT);
    }
    else
    {
      //У всіх інакших випадках нічого не робимо
    }
    /*************************************************/
  }
}
/*****************************************************/
