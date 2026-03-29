#include "header.h"
void make_ekran_data_and_time_elem_d_p_a_registrator(unsigned int type_of_registrator);
/*****************************************************/
//Формуємо екран відображення заголовків реєстраторів
/*****************************************************/
void make_ekran_list_registrators(void)
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_LIST_REGISTRATORS][MAX_COL_LCD] =
    {
      {" Аналог.рег-р   ",
       " Дискр. рег-р   ",
       " Архив диагност.",
       " Рег-р стат.    "},
      {" Аналог.реєстр. ",
       " Дискр. реєстр. ",
       " Архів діагност.",
       " Реєс-р стат.   "},
      {" Dst recorder   ",
       " Binary recorder",
       " Diag recorder  ",
       " Stat. recorder "},
      {" Аналог.рег-р   ",
       " Дискр. рег-р   ",
       " Архив диагност.",
       " Рег-р стат.    "}};

  int index_language = index_language_in_array(current_settings.language);

  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;

  index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  //Копіюємо  рядки у робочий екран
  for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
  {
    //Наступні рядки треба перевірити, чи їх требе відображати у текучій коффігурації
    if (index_of_ekran < MAX_ROW_FOR_LIST_REGISTRATORS)
      for (unsigned int j = 0; j < MAX_COL_LCD; j++)
        working_ekran[i][j] = name_string[index_language][index_of_ekran][j];
    else
      for (unsigned int j = 0; j < MAX_COL_LCD; j++)
        working_ekran[i][j] = ' ';

    index_of_ekran++;
  }

  //Курсор по горизонталі відображається на першій позиції
  current_ekran.position_cursor_x = 0;
  //Відображення курору по вертикалі
  current_ekran.position_cursor_y = position_temp & (MAX_ROW_LCD - 1);
  //Курсор видимий
  current_ekran.cursor_on = 1;
  //Курсор не мигає
  current_ekran.cursor_blinking_on = 0;
  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//Формуємо екран відображення записів аналогового реєстратора
/*****************************************************/
void make_ekran_list_records_registrator_ar()
{
  int index_language = index_language_in_array(current_settings.language);

  unsigned int position_temp = current_ekran.index_position;

  unsigned int number_records;
  unsigned int first_number = (info_rejestrator_ar.first_number < 0) ? 0 : (info_rejestrator_ar.first_number + 1);
  unsigned int last_number = (info_rejestrator_ar.last_number < 0) ? 0 : (info_rejestrator_ar.last_number + 1);

  if (first_number == 0)
    number_records = 0;
  else if (first_number >= last_number)
    number_records = first_number - last_number + 1;
  else
    number_records = NUMBER_FATFS_NAME - last_number + first_number + 1;

  unsigned int error_reading = false;
  if (number_records == 0)
  {
    //Немає записів
    static const unsigned char information[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] =
      {
        {"      Нет       ",
         "    записей     "},
        {"     Нема       ",
         "    записів     "},
        {"    Records     ",
         "   are absent   "},
        {"      Нет       ",
         "    записей     "}};

    unsigned char const(*const pArr)[MAX_COL_LCD] = information[index_language];
    for (size_t i = 0; i != MAX_ROW_LCD; ++i)
    {
      for (size_t j = 0; j != MAX_COL_LCD; j++)
        working_ekran[i][j] = (i < 2) ? pArr[i][j] : ' ';
    }

    //Курсор не видимий
    current_ekran.cursor_on = 0;
    position_temp = 0;
  }
  else if (
    (state_ar_record_fatfs != STATE_AR_NONE_FATFS) &&
    (state_ar_record_fatfs != STATE_AR_MEMORY_FULL_FATFS) &&
    (state_ar_record_fatfs != STATE_AR_BLOCK_FATFS))
  {
    //Читання тимчасово  недоступне

    static const unsigned char information[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] =
      {
        {" Процесс чтения ",
         "  не завершен   "},
        {" Процес читання ",
         " не завершений  "},
        {"    Reading     ",
         "is not completed"},
        {" Процесс чтения ",
         "  не завершен   "}};

    unsigned char const(*const pArr)[MAX_COL_LCD] = information[index_language];
    for (size_t i = 0; i != MAX_ROW_LCD; ++i)
    {
      for (size_t j = 0; j != MAX_COL_LCD; j++)
        working_ekran[i][j] = (i < 2) ? pArr[i][j] : ' ';
    }

    //Курсор не видимий
    current_ekran.cursor_on = 0;
    position_temp = 0;
  }
  else if (current_ekran.index_position < ((int) number_records))
  {
    static unsigned char const template_string[2][MAX_COL_LCD] =
      {
        "   XX-XX-20XX   ",
        "  XX:XX:XX.XXX  "};

    //Множення на два величини position_temp потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
    unsigned int index_of_ekran = ((position_temp << 1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

    unsigned int k = 0;
    unsigned int index_of_ekran_tmp = index_of_ekran >> 1;
    while (((index_of_ekran_tmp + k) < number_records) && (k < (MAX_ROW_FOR_LIST_REGISTRATORS_RECORDS >> 1)))
    {
      unsigned int n_record = index_of_ekran_tmp + k;
      time_t time_dat_tmp = 0;
      int32_t time_ms_tmp = 0;

      int record_name = info_rejestrator_ar.first_number - n_record;
      if (record_name < 0)
        record_name += NUMBER_FATFS_NAME;

      char str[8 + 1 + 3 + 1];
      int n = snprintf(str, (8 + 1), "%d", record_name);
      if (n > 8)
        n = 8; //Теоретично цього ніколи б не мало бути
      str[n++] = '.';
      str[n++] = 'd';
      str[n++] = 'a';
      str[n++] = 't';
      str[n++] = '\0';

      FIL fil;
      FRESULT res = f_open(&fil, str, FA_READ);
      if (res == FR_OK)
      {
        res = f_lseek(&fil, 0);
        if (res == FR_OK)
        {
          unsigned int read_tmp;
          res = f_read(&fil, buffer_for_manu_read_record, sizeof(__HEADER_AR), &read_tmp);
          if (
            (res == FR_OK) &&
            (read_tmp == sizeof(__HEADER_AR))

          )
          {
            if (buffer_for_manu_read_record[0] == LABEL_START_RECORD_AR)
            {
              __HEADER_AR header_ar_tmp;
              /*
              У перших байтах зчитаного буферу є заголовок аналоговог ореєстратора.
              Для зручності на цю адресу ставим структуру заголовку аналогового реєстратора
              щоб легше було можливість читати поля
              */
              header_ar_tmp = *((__HEADER_AR *) buffer_for_manu_read_record);
              time_dat_tmp = header_ar_tmp.time_dat;
              time_ms_tmp = header_ar_tmp.time_ms;
            }
            else
            {
              error_reading = true;

              res = f_close(&fil);
              if (res != FR_OK)
              {
                //Невизначена ситуація, якої ніколи б не мало бути.
                _SET_BIT(set_diagnostyka, ERROR_AR_UNDEFINED_BIT);
              }
              break;
            }
          }
          else
          {
            //Невизначена ситуація, якої ніколи б не мало бути.
            _SET_BIT(set_diagnostyka, ERROR_AR_UNDEFINED_BIT);
            error_reading = true;
            res = f_close(&fil);
            break;
          }
        }
        else
        {
          //Невизначена ситуація, якої ніколи б не мало бути.
          _SET_BIT(set_diagnostyka, ERROR_AR_UNDEFINED_BIT);
          error_reading = true;
          res = f_close(&fil);
          break;
        }

        res = f_close(&fil);
        if (res != FR_OK)
        {
          //Невизначена ситуація, якої ніколи б не мало бути.
          _SET_BIT(set_diagnostyka, ERROR_AR_UNDEFINED_BIT);
        }
      }
      else
      {
        //Невизначена ситуація, якої ніколи б не мало бути.
        _SET_BIT(set_diagnostyka, ERROR_AR_UNDEFINED_BIT);
        error_reading = true;
        break;
      }

      //Копіюємо  рядки у робочий екран
      for (size_t i = 0; i != 2; ++i)
      {
        for (size_t j = 0; j != MAX_COL_LCD; ++j)
          working_ekran[2 * k + i][j] = template_string[i][j];
      }
      if (time_dat_tmp != 0)
      {
        struct tm *p = localtime(&time_dat_tmp);
        int field;

        //День
        field = p->tm_mday;
        working_ekran[2 * k + ROW_R_Y_][COL_DY1_R] = (field / 10) + 0x30;
        working_ekran[2 * k + ROW_R_Y_][COL_DY2_R] = (field % 10) + 0x30;

        //Місяць
        field = p->tm_mon + 1;
        working_ekran[2 * k + ROW_R_Y_][COL_MY1_R] = (field / 10) + 0x30;
        working_ekran[2 * k + ROW_R_Y_][COL_MY2_R] = (field % 10) + 0x30;

        //Рік
        field = p->tm_year - 100;
        working_ekran[2 * k + ROW_R_Y_][COL_SY1_R] = (field / 10) + 0x30;
        working_ekran[2 * k + ROW_R_Y_][COL_SY2_R] = (field % 10) + 0x30;

        //Година
        field = p->tm_hour;
        working_ekran[2 * k + ROW_R_T_][COL_HT1_R] = (field / 10) + 0x30;
        working_ekran[2 * k + ROW_R_T_][COL_HT2_R] = (field % 10) + 0x30;

        //Хвилини
        field = p->tm_min;
        working_ekran[2 * k + ROW_R_T_][COL_MT1_R] = (field / 10) + 0x30;
        working_ekran[2 * k + ROW_R_T_][COL_MT2_R] = (field % 10) + 0x30;

        //Секунди
        field = p->tm_sec;
        working_ekran[2 * k + ROW_R_T_][COL_ST1_R] = (field / 10) + 0x30;
        working_ekran[2 * k + ROW_R_T_][COL_ST2_R] = (field % 10) + 0x30;

        //Тисячні секунд
        field = time_ms_tmp;
        working_ekran[2 * k + ROW_R_T_][COL_HST1_R] = (field / 100) + 0x30;
        field %= 100;

        working_ekran[2 * k + ROW_R_T_][COL_HST2_R] = (field / 10) + 0x30;
        field %= 10;

        working_ekran[2 * k + ROW_R_T_][COL_HST3_R] = field + 0x30;
      }

      ++k;
    }

    if (error_reading == false)
    {
      for (size_t i = 2 * k; i < MAX_ROW_FOR_LIST_REGISTRATORS_RECORDS; ++i)
      {
        for (size_t j = 0; j != MAX_COL_LCD; ++j)
          working_ekran[i][j] = ' ';
      }

      //Курсор видимий
      current_ekran.cursor_on = 1;
    }

    _CLEAR_STATE(FATFS_command, FATFS_READ_DATA_FOR_MENU);
  }

  if (error_reading)
  {
    //Зафіксована помилкова ситуація

    static const unsigned char information[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] =
      {
        {"     Ошибка     ",
         "  отображения   "},
        {"    Помилка     ",
         "  відображення  "},
        {"    Display     ",
         "     error      "},
        {"     Ошибка     ",
         "  отображения   "}};

    unsigned char const(*const pArr)[MAX_COL_LCD] = information[index_language];
    for (size_t i = 0; i != MAX_ROW_LCD; ++i)
    {
      for (size_t j = 0; j != MAX_COL_LCD; j++)
        working_ekran[i][j] = (i < 2) ? pArr[i][j] : ' ';
    }

    //Курсор не видимий
    current_ekran.cursor_on = 0;
    position_temp = 0;
  }

  //Курсор по горизонталі відображається на першій позиції
  current_ekran.position_cursor_x = 0;
  //Відображення курору по вертикалі
  current_ekran.position_cursor_y = ((position_temp << 1) + 1) & (MAX_ROW_LCD - 1);
  //Курсор не мигає
  current_ekran.cursor_blinking_on = 0;
  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//Формуємо екран відображення записів дискретного реєстратора
/*****************************************************/
void make_ekran_list_records_registrator(unsigned int type_registrator)
{
  int index_language = index_language_in_array(current_settings.language);

  unsigned int position_temp = current_ekran.index_position;

  unsigned int number_records;
  if (type_registrator == INDEX_ML_DIGITAL_REGISTRATOR_INFO)
    number_records = info_rejestrator_dr.number_records;
  else if (type_registrator == INDEX_ML_PROGRAM_ERROE_REGISTRATOR_INFO)
    number_records = info_rejestrator_pr_err.number_records;
  else
    number_records = holderCmdPlusTime.shTotalFixElem; //info_rejestrator_pr_err.number_records;

  unsigned int error_reading = false;
  if (number_records == 0)
  {
    //Немає записів
    static const unsigned char information[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] =
      {
        {"      Нет       ",
         "    записей     "},
        {"     Нема       ",
         "    записів     "},
        {"    Records     ",
         "   are absent   "},
        {"      Нет       ",
         "    записей     "}};

    unsigned char const(*const pArr)[MAX_COL_LCD] = information[index_language];
    for (size_t i = 0; i != MAX_ROW_LCD; ++i)
    {
      for (size_t j = 0; j != MAX_COL_LCD; j++)
        working_ekran[i][j] = (i < 2) ? pArr[i][j] : ' ';
    }

    //Курсор не видимий
    current_ekran.cursor_on = 0;
    position_temp = 0;
  }
  else if (current_ekran.index_position < ((int) number_records))
  {
    static unsigned char const template_string[2][MAX_COL_LCD] =
      {
        "   XX-XX-20XX   ",
        "  XX:XX:XX.XXX  "};

    //Множення на два величини position_temp потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
    unsigned int index_of_ekran = ((position_temp << 1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

    unsigned int k = 0;
    unsigned int index_of_ekran_tmp = index_of_ekran >> 1;
    while (((index_of_ekran_tmp + k) < number_records) && (k < (MAX_ROW_FOR_LIST_REGISTRATORS_RECORDS >> 1)))
    {
      time_t time_dat_tmp = 0;
      int32_t time_ms_tmp = 0;

      if (type_registrator == INDEX_ML_DIGITAL_REGISTRATOR_INFO)
      {
        number_record_of_dr_for_menu = index_of_ekran_tmp + k;
        _SET_STATE(control_tasks_dataflash, TASK_MAMORY_READ_DATAFLASH_FOR_DR_MENU_SHORT_BIT);
        while (_GET_STATE(control_tasks_dataflash, TASK_MAMORY_READ_DATAFLASH_FOR_DR_MENU_SHORT_BIT))
        {
          periodical_operations(false);
        }

        if (buffer_for_manu_read_record[0] == LABEL_START_RECORD_DR)
        {
          for (size_t i = 0; i < sizeof(time_t); i++)
            *((unsigned char *) (&time_dat_tmp) + i) = buffer_for_manu_read_record[1 + i];
          for (size_t i = 0; i < sizeof(int32_t); i++)
            *((unsigned char *) (&time_ms_tmp) + i) = buffer_for_manu_read_record[1 + sizeof(time_t) + i];
        }
        else
        {
          error_reading = true;
          break;
        }
      }
      else if (type_registrator == INDEX_ML_PROGRAM_ERROE_REGISTRATOR_INFO)
      {
        number_record_of_pr_err_into_menu = index_of_ekran_tmp + k;
        _SET_STATE(control_tasks_dataflash, TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_MENU_SHORT_BIT);
        while (_GET_STATE(control_tasks_dataflash, TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_MENU_SHORT_BIT))
        {
          periodical_operations(false);
        }

        if (buffer_for_manu_read_record[0] == LABEL_START_RECORD_PR_ERR)
        {
          for (size_t i = 0; i < sizeof(time_t); i++)
            *((unsigned char *) (&time_dat_tmp) + i) = buffer_for_manu_read_record[1 + i];
          for (size_t i = 0; i < sizeof(int32_t); i++)
            *((unsigned char *) (&time_ms_tmp) + i) = buffer_for_manu_read_record[1 + sizeof(time_t) + i];
        }
        else
        {
          error_reading = true;
          break;
        }
      }

      //Копіюємо  рядки у робочий екран
      for (size_t i = 0; i != 2; ++i)
      {
        for (size_t j = 0; j != MAX_COL_LCD; ++j)
          working_ekran[2 * k + i][j] = template_string[i][j];
      }
      if (time_dat_tmp != 0)
      {
        struct tm *p = localtime(&time_dat_tmp);
        int field;

        //День
        field = p->tm_mday;
        working_ekran[2 * k + ROW_R_Y_][COL_DY1_R] = (field / 10) + 0x30;
        working_ekran[2 * k + ROW_R_Y_][COL_DY2_R] = (field % 10) + 0x30;

        //Місяць
        field = p->tm_mon + 1;
        working_ekran[2 * k + ROW_R_Y_][COL_MY1_R] = (field / 10) + 0x30;
        working_ekran[2 * k + ROW_R_Y_][COL_MY2_R] = (field % 10) + 0x30;

        //Рік
        field = p->tm_year - 100;
        working_ekran[2 * k + ROW_R_Y_][COL_SY1_R] = (field / 10) + 0x30;
        working_ekran[2 * k + ROW_R_Y_][COL_SY2_R] = (field % 10) + 0x30;

        //Година
        field = p->tm_hour;
        working_ekran[2 * k + ROW_R_T_][COL_HT1_R] = (field / 10) + 0x30;
        working_ekran[2 * k + ROW_R_T_][COL_HT2_R] = (field % 10) + 0x30;

        //Хвилини
        field = p->tm_min;
        working_ekran[2 * k + ROW_R_T_][COL_MT1_R] = (field / 10) + 0x30;
        working_ekran[2 * k + ROW_R_T_][COL_MT2_R] = (field % 10) + 0x30;

        //Секунди
        field = p->tm_sec;
        working_ekran[2 * k + ROW_R_T_][COL_ST1_R] = (field / 10) + 0x30;
        working_ekran[2 * k + ROW_R_T_][COL_ST2_R] = (field % 10) + 0x30;

        //Тисячні секунд
        field = time_ms_tmp;
        working_ekran[2 * k + ROW_R_T_][COL_HST1_R] = (field / 100) + 0x30;
        field %= 100;

        working_ekran[2 * k + ROW_R_T_][COL_HST2_R] = (field / 10) + 0x30;
        field %= 10;

        working_ekran[2 * k + ROW_R_T_][COL_HST3_R] = field + 0x30;
      }

      ++k;
    }

    if (error_reading == false)
    {
      for (size_t i = 2 * k; i < MAX_ROW_FOR_LIST_REGISTRATORS_RECORDS; ++i)
      {
        for (size_t j = 0; j != MAX_COL_LCD; ++j)
          working_ekran[i][j] = ' ';
      }

      //Курсор видимий
      current_ekran.cursor_on = 1;
    }
  }

  if (error_reading)
  {
    //Зафіксована помилкова ситуація

    static const unsigned char information[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] =
      {
        {"     Ошибка     ",
         "  отображения   "},
        {"    Помилка     ",
         "  відображення  "},
        {"    Display     ",
         "     error      "},
        {"     Ошибка     ",
         "  отображения   "}};

    unsigned char const(*const pArr)[MAX_COL_LCD] = information[index_language];
    for (size_t i = 0; i != MAX_ROW_LCD; ++i)
    {
      for (size_t j = 0; j != MAX_COL_LCD; j++)
        working_ekran[i][j] = (i < 2) ? pArr[i][j] : ' ';
    }

    //Курсор не видимий
    current_ekran.cursor_on = 0;
    position_temp = 0;
  }

  //Курсор по горизонталі відображається на першій позиції
  current_ekran.position_cursor_x = 0;
  //Відображення курору по вертикалі
  current_ekran.position_cursor_y = ((position_temp << 1) + 1) & (MAX_ROW_LCD - 1);
  //Курсор не мигає
  current_ekran.cursor_blinking_on = 0;
  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//Формуємо екран відображення заголовків груп для запису дискретного реєстратора
/*****************************************************/
void make_ekran_list_titles_for_record_of_digital_registrator(void)
{
  int index_language = index_language_in_array(current_settings.language);

  if ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_DR_MENU) == 0)
  {
    //Процес зчитування даних з DataFlash вже закінчився
    static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_TITLES_DIGITAL_REGISTRATOR][MAX_COL_LCD] =
      {
        {" Изм.дискр.сигн.",
         " Макс.Напр.осн. ",
         " Макс.Напр.всп. ",
         " Мин.Напр.осн.  ",
         " Макс.Ток осн.  "},
        {" Зм.дискр.сигн. ",
         " Макс.Напр.осн. ",
         " Макс.Напр.дод. ",
         " Мін.Напр.осн.  ",
         " Макс.Струм осн."},
        {" Bin S Changes  ",
         " Max Main Volt. ",
         " Max Aux.Volt.  ",
         " Min Main Volt. ",
         " Max Main Curr. "},
        {" Изм.дискр.сигн.",
         " Макс.Напр.осн. ",
         " Макс.Напр.всп. ",
         " Мин.Напр.осн.  ",
         " Макс.Ток осн.  "}};

    unsigned int position = current_ekran.index_position;
    unsigned int position_temp = position;
    unsigned int shift[MAX_ROW_FOR_TITLES_DIGITAL_REGISTRATOR];
    for (unsigned int *pShift = shift; pShift < (shift + MAX_ROW_FOR_TITLES_DIGITAL_REGISTRATOR); ++pShift)
      *pShift = 0;

    {
      unsigned int *pShift = shift + INDEX_ML_TITLE_DR_CHANGES_SIGNALS + 1;
      unsigned int val_shift = 0;
      for (size_t i = (INDEX_ML_TITLE_DR_CHANGES_SIGNALS + 1); i < MAX_ROW_FOR_TITLES_DIGITAL_REGISTRATOR; ++i)
      {
        if (
          ((i == INDEX_ML_TITLE_DR_MAX_U_BASE) && (buffer_for_manu_read_record[FIRST_INDEX_NUMBER_MAX_U_BASE_DR] == 0)) ||
          ((i == INDEX_ML_TITLE_DR_MAX_U_SECOND) && (buffer_for_manu_read_record[FIRST_INDEX_NUMBER_MAX_U_SECOND_DR] == 0)) ||
          ((i == INDEX_ML_TITLE_DR_MIN_U_BASE) && (buffer_for_manu_read_record[FIRST_INDEX_NUMBER_MIN_U_BASE_DR] == 0)) ||
          ((i == INDEX_ML_TITLE_DR_MAX_I_BASE) && (buffer_for_manu_read_record[FIRST_INDEX_NUMBER_MAX_I_BASE_DR] == 0)))
        {
          if (i < position)
          {
            --position_temp;
          }
          *pShift = ++val_shift;
        }
        else
        {
          *(pShift++) = val_shift;
        }
      }
      while (pShift < (shift + MAX_ROW_FOR_TITLES_DIGITAL_REGISTRATOR))
        *(pShift++) = val_shift;
    }

    unsigned int index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

    //Копіюємо  рядки у робочий екран
    for (size_t i = 0; i < MAX_ROW_LCD; ++i)
    {
      unsigned int const real_index = index_of_ekran + shift[index_of_ekran];
      //Наступні рядки треба перевірити, чи їх требе відображати у текучій коффігурації
      if (real_index < MAX_ROW_FOR_TITLES_DIGITAL_REGISTRATOR)
        for (size_t j = 0; j < MAX_COL_LCD; ++j)
          working_ekran[i][j] = name_string[index_language][real_index][j];
      else
        for (size_t j = 0; j < MAX_COL_LCD; ++j)
          working_ekran[i][j] = ' ';

      index_of_ekran++;
    }

    //Курсор видимий
    current_ekran.cursor_on = 1;
    //Відображення курору по вертикалі
    current_ekran.position_cursor_y = position_temp & (MAX_ROW_LCD - 1);

    //Позначаємо, що більше цей екран перерисовувати не треба
    rewrite_ekran_once_more = 0;
  }
  else
  {
    //Процес зчитування даних з DataFlash ще не закінчився
    static unsigned char const name_string[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] =
      {
        {" Процесс чтения ",
         "  не завершен   "},
        {" Процес читання ",
         " не завершений  "},
        {"    Reading     ",
         "is not completed"},
        {" Процесс чтения ",
         "  не завершен   "}};

    //Копіюємо  рядки у робочий екран
    for (size_t i = 0; i < MAX_ROW_LCD; ++i)
    {
      //Наступні рядки треба перевірити, чи їх требе відображати у текучій коффігурації
      if (i < 2)
        for (size_t j = 0; j < MAX_COL_LCD; ++j)
          working_ekran[i][j] = name_string[index_language][i][j];
      else
        for (size_t j = 0; j < MAX_COL_LCD; ++j)
          working_ekran[i][j] = ' ';
    }

    //Курсор не видимий
    current_ekran.cursor_on = 0;
    //Відображення курору по вертикалі
    current_ekran.position_cursor_y = 0;

    //Позначаємо, що цей екран треба перерисувати
    rewrite_ekran_once_more = 1;
  }

  //Курсор по горизонталі відображається на першій позиції
  current_ekran.position_cursor_x = 0;
  //Курсор не мигає
  current_ekran.cursor_blinking_on = 0;
  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//Формуємо екран відображення дати і часу запису реєстраторів
/*
0 - дискретний реєстратор
1 - реєстратор програмних подій
2 - аналоговий реєстратор
*/
/*****************************************************/
void make_ekran_data_and_time_of_records_registrator(unsigned int type_of_registrator)
{
  int index_language = index_language_in_array(current_settings.language);

  if (type_of_registrator == 3)
  {
    //Пеший байт сходиться із міткою початку запису - вважаємо, що у буфері достовірні дані
    unsigned char name_string[MAX_ROW_FOR_EKRAN_DATA_LABEL][MAX_COL_LCD] =
      {
        "   XX-XX-20XX   ",
        "  XX:XX:XX.XXX  "};

    unsigned int position_temp = current_ekran.index_position;
    unsigned int index_of_ekran;

    index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

    /******************************************/
    //Заповнюємо поля відповідними цифрами
    /******************************************/
    time_t time_dat_tmp = 0;
    int32_t time_ms_tmp = 0;

    GetDateTimeLogElem((unsigned int *) &time_dat_tmp, number_record_of_stt_cmd_into_menu);
    GetMsLogElem((unsigned int *) &time_ms_tmp, number_record_of_stt_cmd_into_menu);
    //long i = holderCmdPlusTime.shIndexWR;
    //if (i == 0)
    //    i = AMOUNT_CMD_PLUS_TIME_RECORD;
    //else
    //i--;

    //..time_dat_tmp = holderCmdPlusTime.arrCmdPlusTimeHolder[i].unix_time.time_dat;//header_ar_tmp.time_dat;
    //..time_ms_tmp  = holderCmdPlusTime.arrCmdPlusTimeHolder[i].mksec.time_ms;//header_ar_tmp.time_ms;

    if (time_dat_tmp != 0)
    {
      struct tm *p = localtime(&time_dat_tmp);
      int field;

      //День
      field = p->tm_mday;
      name_string[ROW_R_Y_][COL_DY1_R] = (field / 10) + 0x30;
      name_string[ROW_R_Y_][COL_DY2_R] = (field % 10) + 0x30;

      //Місяць
      field = p->tm_mon + 1;
      name_string[ROW_R_Y_][COL_MY1_R] = (field / 10) + 0x30;
      name_string[ROW_R_Y_][COL_MY2_R] = (field % 10) + 0x30;

      //Рік
      field = p->tm_year - 100;
      name_string[ROW_R_Y_][COL_SY1_R] = (field / 10) + 0x30;
      name_string[ROW_R_Y_][COL_SY2_R] = (field % 10) + 0x30;

      //Година
      field = p->tm_hour;
      name_string[ROW_R_T_][COL_HT1_R] = (field / 10) + 0x30;
      name_string[ROW_R_T_][COL_HT2_R] = (field % 10) + 0x30;

      //Хвилини
      field = p->tm_min;
      name_string[ROW_R_T_][COL_MT1_R] = (field / 10) + 0x30;
      name_string[ROW_R_T_][COL_MT2_R] = (field % 10) + 0x30;

      //Секунди
      field = p->tm_sec;
      name_string[ROW_R_T_][COL_ST1_R] = (field / 10) + 0x30;
      name_string[ROW_R_T_][COL_ST2_R] = (field % 10) + 0x30;

      //Тисячні секунд
      field = time_ms_tmp;
      name_string[ROW_R_T_][COL_HST1_R] = (field / 100) + 0x30;
      field %= 100;

      name_string[ROW_R_T_][COL_HST2_R] = (field / 10) + 0x30;
      field %= 10;

      name_string[ROW_R_T_][COL_HST3_R] = field + 0x30;
    }

    //Копіюємо  рядки у робочий екран
    for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
    {
      //Наступні рядки треба перевірити, чи їх требе відображати у текучій коффігурації
      if (index_of_ekran < MAX_ROW_FOR_EKRAN_DATA_LABEL)
        for (unsigned int j = 0; j < MAX_COL_LCD; j++)
          working_ekran[i][j] = name_string[index_of_ekran][j];
      else
        for (unsigned int j = 0; j < MAX_COL_LCD; j++)
          working_ekran[i][j] = ' ';

      index_of_ekran++;
    }

    //Відображення курору по вертикалі
    current_ekran.position_cursor_y = position_temp & (MAX_ROW_LCD - 1);
  }
  else
  {
    //Пеший байт не сходиться із міткою початку запису - робимо висновок, що у біфері не достовірні дані
    static const unsigned char name_string[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] =
      {
        {" Недостоверные  ",
         "     данные     "},
        {"  Недостовірні  ",
         "      дані      "},
        {"    Invalid     ",
         "      data      "},
        {" Недостоверные  ",
         "     данные     "}};

    //Копіюємо  рядки у робочий екран
    for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
    {
      //Наступні рядки треба перевірити, чи їх требе відображати у текучій коффігурації
      if (i < 2)
        for (unsigned int j = 0; j < MAX_COL_LCD; j++)
          working_ekran[i][j] = name_string[index_language][i][j];
      else
        for (unsigned int j = 0; j < MAX_COL_LCD; j++)
          working_ekran[i][j] = ' ';
    }

    //Відображення курору по вертикалі
    current_ekran.position_cursor_y = 0;
  }

  //Курсор не видимий
  current_ekran.cursor_on = 0;

  //Курсор по горизонталі відображається на першій позиції
  current_ekran.position_cursor_x = 0;
  //Курсор не мигає
  current_ekran.cursor_blinking_on = 0;
  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//Формуємо екран відображення діючих значень при фіксації максимальних/мінімальних вимірювань запису дискретного реєстратора
/*****************************************************/
void make_ekran_title_analog_value_records_digital_registrator(void)
{
  unsigned char name_string[MAX_ROW_FOR_TITLE_EKRAN_ANALOG_VALUES_DR][MAX_COL_LCD];
  for (int index_1 = 0; index_1 < MAX_ROW_LCD; index_1++)
  {
    for (int index_2 = 0; index_2 < MAX_COL_LCD; index_2++)
      name_string[index_1][index_2] = ' ';
  }
  int index_language = index_language_in_array(current_settings.language);

  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  unsigned int number_info_records;

  number_info_records = buffer_for_manu_read_record[FIRST_INDEX_NUMBER_MAX_U_BASE_DR + type_view_max_values_dr - IDENTIFIER_BIT_ARRAY_MAX_U_BASE];

  index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  index_cell_into_array_for_integral_values_dr = -1; //Помічаємо, що ми покищо не знайшли індекс комірки, який відповідає першому блоку, який визначений курсором
  if (number_info_records == 0)
  {
    static const unsigned char information[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] =
      {
        {"      Нет       ",
         "    записей     "},
        {"     Нема       ",
         "    записів     "},
        {"    Records     ",
         "   are absent   "},
        {"      Нет       ",
         "    записей     "}};

    for (int index_1 = 0; index_1 < 2; index_1++)
    {
      for (int index_2 = 0; index_2 < MAX_COL_LCD; index_2++)
        name_string[index_1][index_2] = information[index_language][index_1][index_2];
    }

    //Курсор не видимий
    current_ekran.cursor_on = 0;
  }
  else if (current_ekran.index_position < ((int) number_info_records))
  {
    unsigned int k = 0;
    while (((index_of_ekran + k) < number_info_records) && (k < MAX_ROW_FOR_TITLE_EKRAN_ANALOG_VALUES_DR))
    {
      //Шукаємо блок, який відповідає заданому типу струму
      unsigned int number_finded_this_type_of_current = 0;
      unsigned int index_cell_into_array = (FIRST_INDEX_FIRST_BLOCK_DR + (sizeof(unsigned int) * SIZE_ARRAY_FIX_MAX_MEASUREMENTS - 1)); //Індекс комірки пам'яті встановлюємо на байт першого блоку, який відображає тип струму по якому йшов моніторинг
      while ((number_finded_this_type_of_current != (index_of_ekran + k + 1)) && (index_cell_into_array < FIRST_INDEX_FIRST_DATA_DR))
      {
        if (buffer_for_manu_read_record[index_cell_into_array] == type_view_max_values_dr)
          number_finded_this_type_of_current++;
        if (number_finded_this_type_of_current != (index_of_ekran + k + 1))
          index_cell_into_array += (sizeof(unsigned int) * SIZE_ARRAY_FIX_MAX_MEASUREMENTS);
      }

      unsigned char information[MAX_COL_LCD] = "                ";
      static const unsigned char ms[MAX_NAMBER_LANGUAGE][3] =
        {
          "мс.",
          "мс.",
          "ms ",
          "мс."};
      if ((number_finded_this_type_of_current == (index_of_ekran + k + 1)) && (index_cell_into_array < FIRST_INDEX_FIRST_DATA_DR))
      {
        //Ми знайшли потрібний блок
        //Якщо у цьому рядку ще і знаходиться курсор, то треба запам'ятати індекс першого елементу найденого блоку
        if (position_temp == (index_of_ekran + k))
          index_cell_into_array_for_integral_values_dr = index_cell_into_array - (sizeof(unsigned int) * SIZE_ARRAY_FIX_MAX_MEASUREMENTS - 1);

        //Визначаємо час зрізу
        index_cell_into_array -= 3; //Переводимо індекс на початок часової мітки
        unsigned char sring_of_time[8] = "        ";
        unsigned int time_of_slice = buffer_for_manu_read_record[index_cell_into_array] + (buffer_for_manu_read_record[index_cell_into_array + 1] << 8) + (buffer_for_manu_read_record[index_cell_into_array + 2] << 16);

        //Конвертуємо цей час у рядок
        unsigned int ost, local_index = 0;
        while ((time_of_slice >= 10) && (local_index < (8 - 1)))
        {
          ost = time_of_slice % 10;
          sring_of_time[7 - local_index] = ost + 0x30;
          time_of_slice /= 10;
          local_index++;
        }
        sring_of_time[7 - local_index] = time_of_slice + 0x30;

        //Копіюємо цей рідок часу у наш робочий рядок
        local_index = 0;
        while ((sring_of_time[local_index] == ' ') && (local_index < 8))
          local_index++;
        unsigned int local_index1 = 1;
        while (local_index < 8)
          information[local_index1++] = sring_of_time[local_index++];
        information[local_index1++] = ' ';
        information[local_index1++] = ms[index_language][0];
        information[local_index1++] = ms[index_language][1];
        information[local_index1++] = ms[index_language][2];
      }
      else
      {
        //теоретично ми сюди б не мали ніколи заходити, але якщо ми сюди зайшли, то виводимо сигналізацію про помилку на екран
        static const unsigned char error_meas[MAX_NAMBER_LANGUAGE][MAX_COL_LCD] =
          {
            " Ошибка         ",
            " Помилка        ",
            " Error          ",
            " Ошибка         "};
        for (unsigned int i = 0; i < MAX_COL_LCD; i++)
          information[i] = error_meas[index_language][i];
      }

      for (unsigned int i = 0; i < MAX_COL_LCD; i++)
        name_string[k][i] = information[i];
      k++;
    }

    //Курсор видимий
    current_ekran.cursor_on = 1;
  }
  else
  {
    //Зафіксована помилкова ситуація
    static const unsigned char information[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] =
      {
        {"     Ошибка     ",
         "  отображения   "},
        {"    Помилка     ",
         "  відображення  "},
        {"    Display     ",
         "     error      "},
        {"     Ошибка     ",
         "  отображения   "}};

    for (int index_1 = 0; index_1 < 2; index_1++)
    {
      for (int index_2 = 0; index_2 < MAX_COL_LCD; index_2++)
        name_string[index_1][index_2] = information[index_language][index_1][index_2];
    }

    //Курсор не видимий
    current_ekran.cursor_on = 0;
  }

  //Копіюємо  рядки у робочий екран
  for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
  {
    for (unsigned int j = 0; j < MAX_COL_LCD; j++)
      working_ekran[i][j] = name_string[i][j];
  }

  //Курсор по горизонталі відображається на першій позиції
  current_ekran.position_cursor_x = 0;
  //Відображення курору по вертикалі
  current_ekran.position_cursor_y = position_temp & (MAX_ROW_LCD - 1);
  //Курсор не мигає
  current_ekran.cursor_blinking_on = 0;
  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//Формуємо екран відображення діючих значень при фіксації при фіксації конкретного типу моніторингу
/*****************************************************/
void make_ekran_analog_value_records_digital_registrator(void)
{
  int const index_language = index_language_in_array(current_settings.language);

  if (buffer_for_manu_read_record[FIRST_INDEX_START_START_RECORD_DR] == LABEL_START_RECORD_DR)
  {
    //Пеший байт сходиться із міткою початку запису - вважаємо, що у буфері достовірні дані
    static unsigned char const name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_EKRAN_ANALOG_VALUES_DR][MAX_COL_LCD] =
      {{" Uab-1=         ",
        " Ia -1=         ",
        " Uab-2=         ",
        " Ia -2=         ",
        " f  =           ",
        " Полож.         "},
       {" Uab-1=         ",
        " Ia -1=         ",
        " Uab-2=         ",
        " Ia -2=         ",
        " f  =           ",
        "Позиц.=         "},
       {" Vab-1=         ",
        " Ia -1=         ",
        " Vab-2=         ",
        " Ia -2=         ",
        " f  =           ",
        "Полож.=         "},
       {" Uab-1=         ",
        " Ia -1=         ",
        " Uab-2=         ",
        " Ia -2=         ",
        " f  =           ",
        "Полож.=         "}};

    unsigned char name_string_tmp[MAX_ROW_FOR_EKRAN_ANALOG_VALUES_DR][MAX_COL_LCD];
    for (size_t i = 0; (i < MAX_ROW_FOR_EKRAN_ANALOG_VALUES_DR); ++i)
    {
      unsigned char const *ptr_source = name_string[index_language][i];
      unsigned char *ptr_target = name_string_tmp[i];
      for (size_t j = 0; j < MAX_COL_LCD; j++)
        *ptr_target++ = *ptr_source++;
    }

    /******************************************
    Заповнюємо масив відповідними числами, які є у буфері читання запису дискретного реєстратора
    ******************************************/
    unsigned int temp_measurement[SIZE_ARRAY_FIX_MAX_MEASUREMENTS];
    unsigned char *point_unsigned_char = (unsigned char *) (buffer_for_manu_read_record + index_cell_into_array_for_integral_values_dr);
    for (unsigned int i = 0; i < SIZE_ARRAY_FIX_MAX_MEASUREMENTS; ++i)
    {
      temp_measurement[i] = 0;
      for (size_t j = 0; j < 4; ++j)
      {
        temp_measurement[i] |= (*point_unsigned_char++) << (8 * i);
      }
    }

    int additional_current = 0;
    int position_temp = current_ekran.index_position;

    unsigned int control_for_dr;
    control_for_dr = temp_measurement[IDM_CONTROL];
    /******************************************/

    if ((control_for_dr & (1 << 0)) == 0)
    {
      //2-обмотковий трансформатор

      for (unsigned int k = 0; k < 2; k++)
      {
        int i = 0;

        switch (k)
        {
          case 0:
            {
              i = INDEX_ML_DR_U2 - additional_current;
              break;
            }
          case 1:
            {
              i = INDEX_ML_DR_I2 - additional_current;
              break;
            }
          default:
            {
              //Теоретично цього ніколи не мало б бути
              total_error_sw_fixed();
            }
        }

        if ((i + 1) <= position_temp)
          position_temp--;
        do
        {
          for (unsigned int j = 0; j < MAX_COL_LCD; j++)
          {
            if ((i + 1) < MAX_ROW_FOR_EKRAN_ANALOG_VALUES_DR)
            {
              name_string_tmp[i][j] = name_string_tmp[i + 1][j];
              temp_measurement[i] = temp_measurement[i + 1];
            }
            else
            {
              name_string_tmp[i][j] = ' ';
              temp_measurement[i] = 0;
            }
          }
          i++;
        } while (i < (MAX_ROW_FOR_EKRAN_ANALOG_VALUES_DR - additional_current));
        additional_current++;
      }
    }
    else
    {
      //3-обмотковий трансформатор
      if ((control_for_dr & (1 << 1)) == 0)
      {
        //Основний трансформатор є ТН1
        name_string_tmp[INDEX_ML_DR_U1][0] = '*';
        name_string_tmp[INDEX_ML_DR_I1][0] = '*';
      }
      else
      {
        //Основний трансформатор є ТН2
        name_string_tmp[INDEX_ML_DR_U2][0] = '*';
        name_string_tmp[INDEX_ML_DR_I2][0] = '*';
      }
    }

    for (int i = 0; i < (MAX_ROW_FOR_EKRAN_ANALOG_VALUES_DR - additional_current); i++)
    {
      if (i < (IDM_freq - additional_current))
      {
        //Струми і напруги

        unsigned int const start_number_digit_after_point = 3;
        convert_and_insert_char_for_measurement(start_number_digit_after_point, temp_measurement[i], 1, 1, name_string_tmp[i], 7);

        if ((i == INDEX_ML_DR_U1) || (i == INDEX_ML_DR_U2))
          name_string_tmp[i][MAX_COL_LCD - 1] = odynyci_vymirjuvannja[index_language][INDEX_V];
        else
          name_string_tmp[i][MAX_COL_LCD - 1] = odynyci_vymirjuvannja[index_language][INDEX_A];
      }
      else if (i == (IDM_freq - additional_current))
      {
        //Частота
        if ((int) temp_measurement[i] < 0)
        {
          if ((int) temp_measurement[i] == (-2 * 1000))
          {
            /*Частота нижче порогу визначеного константою MIN_FREQUENCY*/
            name_string_tmp[i][4] = '<';
            temp_measurement[i] = MIN_FREQUENCY * 1000;
          }
          if ((int) temp_measurement[i] == (-3 * 1000))
          {
            /*Частота вище порогу визначеного константою MAX_FREQUENCY*/
            name_string_tmp[i][4] = '>';
            temp_measurement[i] = MAX_FREQUENCY * 1000;
          }
        }
        convert_and_insert_char_for_frequency(temp_measurement[i], name_string_tmp[i]);
      }
      else if (i == (IDM_CONTROL - additional_current))
      {
        int current_step_tmp = (int) temp_measurement[i];
        if (current_step_tmp < 0)
        {
          name_string_tmp[i][8] = '?';
        }
        else
        {
          unsigned int value = current_step_tmp;
          unsigned int vaga = 10, first_symbol = 0, shift = 0;
          for (unsigned int j = 8; j <= 9; j++)
          {
            calc_int_symbol_and_put_into_working_ekran((name_string_tmp[i] + j - shift), &value, &vaga, &first_symbol, true);

            if (name_string_tmp[i][8] == ' ')
              shift++;
          }
        }
        /********************************/
      }
    }

    unsigned int index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

    //Копіюємо  рядки у робочий екран
    for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
    {
      //Наступні рядки треба перевірити, чи їх требе відображати у текучій коффігурації
      if (((int) index_of_ekran) < (MAX_ROW_FOR_EKRAN_ANALOG_VALUES_DR - additional_current))
        for (unsigned int j = 0; j < MAX_COL_LCD; j++)
          working_ekran[i][j] = name_string_tmp[index_of_ekran][j];
      else
        for (unsigned int j = 0; j < MAX_COL_LCD; j++)
          working_ekran[i][j] = ' ';

      index_of_ekran++;
    }

    //Відображення курору по вертикалі
    current_ekran.position_cursor_y = position_temp & (MAX_ROW_LCD - 1);
    //Курсор видимий
    current_ekran.cursor_on = 1;
  }
  else
  {
    //Пеший байт не сходиться із міткою початку запису - робимо висновок, що у біфері не достовірні дані
    static const unsigned char name_string[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] =
      {
        {" Недостоверные  ",
         "     данные     "},
        {"  Недостовірні  ",
         "      дані      "},
        {"    Invalid     ",
         "      data      "},
        {" Недостоверные  ",
         "     данные     "}};

    //Копіюємо  рядки у робочий екран
    for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
    {
      //Наступні рядки треба перевірити, чи їх требе відображати у текучій коффігурації
      if (i < 2)
        for (unsigned int j = 0; j < MAX_COL_LCD; j++)
          working_ekran[i][j] = name_string[index_language][i][j];
      else
        for (unsigned int j = 0; j < MAX_COL_LCD; j++)
          working_ekran[i][j] = ' ';
    }

    //Відображення курору по вертикалі
    current_ekran.position_cursor_y = 0;
    //Курсор не видимий
    current_ekran.cursor_on = 0;
  }

  //Курсор по горизонталі відображається на першій позиції
  current_ekran.position_cursor_x = 0;
  //Курсор не мигає
  current_ekran.cursor_blinking_on = 0;
  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//Формуємо екран відображення змін дискретних сигналів з запису дискретного реєстратора
/*****************************************************/
void make_ekran_changing_signals_digital_registrator(void)
{
  int index_language = index_language_in_array(current_settings.language);

  if (buffer_for_manu_read_record[FIRST_INDEX_START_START_RECORD_DR] == LABEL_START_RECORD_DR)
  {
    //Пеший байт сходиться із міткою початку запису - вважаємо, що у буфері достовірні дані
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
    static const unsigned char name_string[MAX_NAMBER_LANGUAGE][NUMBER_TOTAL_SIGNAL_FOR_RANG + (1 - N_IN_GOOSE) + (1 - N_IN_MMS) + (1 - N_OUT_LAN) + (3 - NUMBER_UP_SIGNAL_FOR_RANG)][MAX_COL_LCD] =
#else
    static const unsigned char name_string[MAX_NAMBER_LANGUAGE][NUMBER_TOTAL_SIGNAL_FOR_RANG + (3 - NUMBER_UP_SIGNAL_FOR_RANG)][MAX_COL_LCD] =
#endif
      {
        {NAME_RANG_RU},
        {NAME_RANG_UA},
        {NAME_RANG_EN},
        {NAME_RANG_KZ}};

    uint8_t name_string_tmp[NUMBER_TOTAL_SIGNAL_FOR_RANG][MAX_COL_LCD];
    for (int index_1 = 0; index_1 < NUMBER_TOTAL_SIGNAL_FOR_RANG; index_1++)
    {
      size_t index_row;
      if (index_1 < NUMBER_GENERAL_SIGNAL_FOR_RANG)
      {
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
        if (index_1 < (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_IN_GOOSE + N_IN_MMS + N_OUT_LAN)))
        {
          index_row = index_1;
        }
        else if (index_1 < (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_IN_MMS + N_OUT_LAN)))
        {
          index_row = RANG_BLOCK_IN_GOOSE1 + ((index_1 - (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_IN_GOOSE + N_IN_MMS + N_OUT_LAN))) % 1);
        }
        else if (index_1 < (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_OUT_LAN)))
        {
          index_row = RANG_BLOCK_IN_MMS1 + (1 - N_IN_GOOSE) + ((index_1 - (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_IN_MMS + N_OUT_LAN))) % 1);
        }
        else
        {
          index_row = RANG_BLOCK_OUT_LAN1 + (1 - N_IN_GOOSE) + (1 - N_IN_MMS) + ((index_1 - (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_OUT_LAN))) % 1);
        }
#else
        index_row = index_1;
#endif
      }
      else if (index_1 < RANG_BLOCK_UP1)
      {
        index_row = index_1
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                    + 1 - N_IN_GOOSE + 1 - N_IN_MMS + 1 - N_OUT_LAN
#endif
          ;
      }
      else if (index_1 < (RANG_BLOCK_UP1 + NUMBER_UP_SIGNAL_FOR_RANG))
      {
        index_row = RANG_BLOCK_UP1
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                    + 1 - N_IN_GOOSE + 1 - N_IN_MMS + 1 - N_OUT_LAN
#endif
                    + ((index_1 - RANG_BLOCK_UP1) % 3);
      }
      else
      {
        index_row = index_1
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                    + 1 - N_IN_GOOSE + 1 - N_IN_MMS + 1 - N_OUT_LAN
#endif
                    + 3 - NUMBER_UP_SIGNAL_FOR_RANG;
      }

      for (size_t index_2 = 0; index_2 < MAX_COL_LCD; index_2++)
      {
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
        if (
          (index_1 >= (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_IN_GOOSE + N_IN_MMS + N_OUT_LAN))) &&
          (index_1 < (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_IN_MMS + N_OUT_LAN))) &&
          (index_2 >= index_n_In_GOOSE[index_language][(index_1 - RANG_BLOCK_IN_GOOSE1) % 1]) &&
          (index_2 <= (index_n_In_GOOSE[index_language][(index_1 - RANG_BLOCK_IN_GOOSE1) % 1] + 1)))
        {
          unsigned int n = index_1 - RANG_BLOCK_IN_GOOSE1;
          if ((n + 1) < 10)
          {
            if (index_2 == index_n_In_GOOSE[index_language][n % 1])
              name_string_tmp[index_1][index_2] = 0x30 + (n + 1);
            else
              name_string_tmp[index_1][index_2] = ' ';
          }
          else
          {
            if (index_2 == index_n_In_GOOSE[index_language][n % 1])
              name_string_tmp[index_1][index_2] = 0x30 + (n / 1 + 1) / 10;
            else
              name_string_tmp[index_1][index_2] = 0x30 + (n / 1 + 1) % 10;
          }
        }
        else if (
          (index_1 >= (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_IN_MMS + N_OUT_LAN))) &&
          (index_1 < (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_OUT_LAN))) &&
          (index_2 == index_n_In_MMS[index_language][(index_1 - RANG_BLOCK_IN_MMS1) % 1]))
        {
          name_string_tmp[index_1][index_2] = 0x30 + ((index_1 - RANG_BLOCK_IN_MMS1) / 1 + 1);
        }
        else if (
          (index_1 >= (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_OUT_LAN))) &&
          (index_1 < (NUMBER_GENERAL_SIGNAL_FOR_RANG)) &&
          (index_2 == index_n_Out_LAN[index_language][(index_1 - RANG_BLOCK_OUT_LAN1) % 1]))
        {
          name_string_tmp[index_1][index_2] = 0x30 + ((index_1 - RANG_BLOCK_OUT_LAN1) / 1 + 1);
        }
        else
#endif
        {
          if (
            (index_1 >= RANG_BLOCK_UP1) &&
            (index_1 < (RANG_BLOCK_UP1 + NUMBER_UP_SIGNAL_FOR_RANG)) &&
            (index_2 == index_number_UP[index_language][(index_1 - RANG_BLOCK_UP1) % 3]))
          {
            name_string_tmp[index_1][index_2] = 0x30 + ((index_1 - RANG_BLOCK_UP1) / 3 + 1);
          }
          else
            name_string_tmp[index_1][index_2] = name_string[index_language][index_row][index_2];
        }
      }
    }

    unsigned int max_number_changers_in_record = buffer_for_manu_read_record[FIRST_INDEX_NUMBER_CHANGES_DR] | (buffer_for_manu_read_record[FIRST_INDEX_NUMBER_CHANGES_DR + 1] << 8);
    unsigned int position_temp;
    unsigned int index_of_ekran;
    unsigned int array_old[N_BIG], array_new[N_BIG], array_changing[N_BIG];

    //Перевіряємо, чи ми не вийшли за границі
    if (current_ekran.index_position < 0)
      current_ekran.index_position = max_number_changers_in_record - 1;
    else if (current_ekran.index_position >= ((int) max_number_changers_in_record))
      current_ekran.index_position = 0;
    current_ekran.index_position = (current_ekran.index_position >> (POWER_MAX_ROW_LCD - 1)) << (POWER_MAX_ROW_LCD - 1);
    position_in_current_level_menu[EKRAN_CHANGES_SIGNALS_DR] = current_ekran.index_position;

    position_temp = current_ekran.index_position;
    index_of_ekran = position_temp & (unsigned int) (~((1 << (POWER_MAX_ROW_LCD >> 1)) - 1));

    //Копіюємо  рядки у робочий екран
    for (unsigned int i = 0; i < (MAX_ROW_LCD >> 1); i++)
    {
      //Наступні рядки треба перевірити, чи їх требе відображати у текучій кофігурації
      if (index_of_ekran < max_number_changers_in_record)
      {
        //Шукаємо масиви зрізів: попередній до вибраної зміни і у момент даної зміни
        unsigned int index_of_the_slice = 0; //починаємо з першого зрізу
        unsigned int current_number_changes = 0;
        while (
          ((current_number_changes + buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + SD_DR * (1 + index_of_the_slice) + (SD_DR - 2)] + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + SD_DR * (1 + index_of_the_slice) + (SD_DR - 1)] << 8)) < (index_of_ekran + 1)) &&
          (index_of_the_slice < MAX_EVENTS_IN_ONE_RECORD))
        {
          current_number_changes += (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + SD_DR * (1 + index_of_the_slice) + (SD_DR - 2)] + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + SD_DR * (1 + index_of_the_slice) + (SD_DR - 1)] << 8));
          index_of_the_slice++;
        }

        for (size_t j = 0; j < (N_BIG - ((NUMBER_TOTAL_SIGNAL_FOR_RANG % 32) != 0)); ++j)
        {
          array_new[j] = buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + SD_DR * (1 + index_of_the_slice) + 3 + 4 * j] + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + SD_DR * (1 + index_of_the_slice) + 4 + 4 * j] << 8) + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + SD_DR * (1 + index_of_the_slice) + 5 + 4 * j] << 16) + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + SD_DR * (1 + index_of_the_slice) + 6 + 4 * j] << 24);

          array_old[j] = buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + SD_DR * (1 + index_of_the_slice - 1) + 3 + 4 * j] + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + SD_DR * (1 + index_of_the_slice - 1) + 4 + 4 * j] << 8) + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + SD_DR * (1 + index_of_the_slice - 1) + 5 + 4 * j] << 16) + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + SD_DR * (1 + index_of_the_slice - 1) + 6 + 4 * j] << 24);
        }
        if ((NUMBER_TOTAL_SIGNAL_FOR_RANG % 32) != 0)
        {
          array_new[N_BIG - 1] = 0;
          array_old[N_BIG - 1] = 0;
          for (intptr_t j = 0; j < (NUMBER_BYTES_SAMPLE_DR - 4 * (NUMBER_TOTAL_SIGNAL_FOR_RANG / 32)); ++j)
          {
            array_new[N_BIG - 1] += (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + SD_DR * (1 + index_of_the_slice) + (3 + j) + 4 * (N_BIG - 1)] << (8 * j));

            array_old[N_BIG - 1] += (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + SD_DR * (1 + index_of_the_slice - 1) + (3 + j) + 4 * (N_BIG - 1)] << (8 * j));
          }
        }

        //Визначаємо, які сигнали змінилися
        for (unsigned int j = 0; j < N_BIG; j++)
          array_changing[j] = array_new[j] ^ array_old[j];

        //Шукаємо функцію, яку треба зараз відобразити
        int index_of_function_in_the_slice = 0; //починаємо з першого зрізу
        do
        {
          if ((array_changing[index_of_function_in_the_slice >> 5] & (1 << (index_of_function_in_the_slice & ((1 << 5) - 1)))) != 0)
            current_number_changes++;
          if (current_number_changes < (index_of_ekran + 1))
            index_of_function_in_the_slice++;
        } while (
          (current_number_changes < (index_of_ekran + 1)) &&
          (index_of_function_in_the_slice < NUMBER_TOTAL_SIGNAL_FOR_RANG));

        if ((index_of_the_slice < MAX_EVENTS_IN_ONE_RECORD) && (index_of_function_in_the_slice < NUMBER_TOTAL_SIGNAL_FOR_RANG))
        {
          for (unsigned int k = 0; k < 2; k++)
          {
            if (((i << 1) + k) < MAX_ROW_LCD)
            {
              if (k == 0)
              {
                //У першому рядку відображаємо назву сигналу, який змінився
                for (unsigned int j = 0; j < MAX_COL_LCD; j++)
                  working_ekran[(i << 1) + k][j] = name_string_tmp[index_of_function_in_the_slice][j];
              }
              else
              {
                //У другому рядку відображаємо час зміни і яке значення прийняв зараз сигнал
                unsigned char second_row[MAX_COL_LCD] = "                ";
                unsigned char sring_of_time[8] = "        ";
                static const unsigned char ms[MAX_NAMBER_LANGUAGE][3] =
                  {
                    "мс.",
                    "мс.",
                    "ms ",
                    "мс."};
                static const unsigned char passive_active[MAX_NAMBER_LANGUAGE][2][5] =
                  {
                    {"Пасс.", "Акт. "},
                    {"Пас. ", "Акт. "},
                    {"Pass ", "Act  "},
                    {"Пасс.", "Акт. "}};
                unsigned int time_of_slice = buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + SD_DR * (1 + index_of_the_slice) + 0] + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + SD_DR * (1 + index_of_the_slice) + 1] << 8) + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + SD_DR * (1 + index_of_the_slice) + 2] << 16);
                long sign = (long) (time_of_slice & (1 << 23)) >> 23;
                if (sign != 0)
                {
                  time_of_slice |= 0xff000000;
                  time_of_slice *= (unsigned int) (-1);
                }

                //Конвертуємо цей час у рядок
                unsigned int ost, local_index = 0;
                while ((time_of_slice >= 10) && (local_index < (8 - 1 - 1)))
                {
                  ost = time_of_slice % 10;
                  sring_of_time[7 - local_index] = ost + 0x30;
                  time_of_slice /= 10;
                  local_index++;
                }
                sring_of_time[7 - local_index] = time_of_slice + 0x30;
                if (sign != 0)
                  sring_of_time[6 - local_index] = '-';

                //Копіюємо цей рідок часу у наш робочий рядок
                local_index = 0;
                while ((sring_of_time[local_index] == ' ') && (local_index < 8))
                  local_index++;
                unsigned int local_index1 = 1;
                while (local_index < 8)
                  second_row[local_index1++] = sring_of_time[local_index++];
                if (local_index1 < 8)
                  second_row[local_index1++] = ' ';
                second_row[local_index1++] = ms[index_language][0];
                second_row[local_index1++] = ms[index_language][1];
                second_row[local_index1++] = ms[index_language][2];

                //Визначаємо стан даного дискретного сигналу
                if ((array_new[index_of_function_in_the_slice >> 5] & (1 << (index_of_function_in_the_slice & ((1 << 5) - 1)))) != 0)
                {
                  //Сигнал перейшов у активний стан
                  for (local_index1 = 11; local_index1 < MAX_COL_LCD; local_index1++)
                    second_row[local_index1] = passive_active[index_language][1][local_index1 - 11];
                }
                else
                {
                  //Сигнал перейшов у пасивний стан
                  for (local_index1 = 11; local_index1 < MAX_COL_LCD; local_index1++)
                    second_row[local_index1] = passive_active[index_language][0][local_index1 - 11];
                }

                for (unsigned int j = 0; j < MAX_COL_LCD; j++)
                  working_ekran[(i << 1) + k][j] = second_row[j];
              }
            }
          }
        }
        else
        {
          unsigned char const error_string[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] =
            {
              {" Неопределенная ",
               " ошибка отображ."},
              {"  Невизначена   ",
               " помилка відобр."},
              {"   Undefined    ",
               " display error  "},
              {" Неопределенная ",
               " ошибка отображ."}};
          for (unsigned int k = 0; k < 2; k++)
          {
            if (((i << 1) + k) < MAX_ROW_LCD)
              for (unsigned int j = 0; j < MAX_COL_LCD; j++)
                working_ekran[(i << 1) + k][j] = error_string[index_language][k][j];
          }
        }
      }
      else
      {
        for (unsigned int k = 0; k < 2; k++)
        {
          if (((i << 1) + k) < MAX_ROW_LCD)
            for (unsigned int j = 0; j < MAX_COL_LCD; j++)
              working_ekran[(i << 1) + k][j] = ' ';
        }
      }

      index_of_ekran++;
    }

    //Відображення курору по вертикалі
    current_ekran.position_cursor_y = (position_temp << 1) & (MAX_ROW_LCD - 1);
    //Курсор невидимий
    current_ekran.cursor_on = 0;
  }
  else
  {
    //Пеший байт не сходиться із міткою початку запису - робимо висновок, що у біфері не достовірні дані
    static const unsigned char name_string[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] =
      {
        {" Недостоверные  ",
         "     данные     "},
        {"  Недостовірні  ",
         "      дані      "},
        {"    Invalid     ",
         "      data      "},
        {" Недостоверные  ",
         "     данные     "}};

    //Копіюємо  рядки у робочий екран
    for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
    {
      //Наступні рядки треба перевірити, чи їх требе відображати у текучій коффігурації
      if (i < 2)
        for (unsigned int j = 0; j < MAX_COL_LCD; j++)
          working_ekran[i][j] = name_string[index_language][i][j];
      else
        for (unsigned int j = 0; j < MAX_COL_LCD; j++)
          working_ekran[i][j] = ' ';
    }

    //Відображення курору по вертикалі
    current_ekran.position_cursor_y = 0;
    //Курсор не видимий
    current_ekran.cursor_on = 0;

    current_ekran.index_position = 0;
  }

  //Курсор по горизонталі відображається на першій позиції
  current_ekran.position_cursor_x = 0;
  //Курсор не мигає
  current_ekran.cursor_blinking_on = 0;
  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//Формуємо екран відображення змін діагностики у записі реєстратора програмних подій
/*****************************************************/
void make_ekran_changing_diagnostics_pr_err_registrator(void)
{
  int index_language = index_language_in_array(current_settings.language);

  while (_GET_STATE(control_tasks_dataflash, TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_MENU_BIT))
  {
    periodical_operations(false);
  }

  if (buffer_for_manu_read_record[0] == LABEL_START_RECORD_PR_ERR)
  {
    //Пеший байт сходиться із міткою початку запису - вважаємо, що у буфері достовірні дані
    static unsigned char const name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_DIAGNOSTYKA][MAX_COL_LCD] =
      {
        {NAME_DIAGN_RU},
        {NAME_DIAGN_UA},
        {NAME_DIAGN_EN},
        {NAME_DIAGN_KZ}};

    unsigned char name_string_tmp[MAX_ROW_FOR_DIAGNOSTYKA][MAX_COL_LCD];

    for (unsigned int index_1 = 0; index_1 < MAX_ROW_FOR_DIAGNOSTYKA; index_1++)
    {
      for (unsigned int index_2 = 0; index_2 < MAX_COL_LCD; index_2++)
        name_string_tmp[index_1][index_2] = name_string[index_language][index_1][index_2];

      if ((index_1 >= ERROR_DIGITAL_OUTPUT_1_BIT) && (index_1 < (ERROR_DIGITAL_OUTPUT_1_BIT + NUMBER_SIMPLE_OUTPUTS)))
      {
        uint32_t value = 0;
        uint32_t index_board = 0, index_number = 0;
        for (size_t index_2 = 0; index_2 < MAX_COL_LCD; index_2++)
        {
          if (name_string_tmp[index_1][index_2] == '?')
          {
            if (value == 0)
            {
              //Це є умовою фіксації слова
              index_board = index_2;
              value = (index_1 - ERROR_DIGITAL_OUTPUT_1_BIT) + 1;
            }
            else
            {
              //Це є умовою фіксації номеру на слоті
              index_number = index_2;
              break;
            }
          }
        }

        int tmp_1 = -1, tmp_2 = -1;
        for (size_t i = 0; i < N_OUTPUT_BOARDS; i++)
        {
          if (value <= output_boards[i][0])
          {
            tmp_1 = output_boards[i][1];
            tmp_2 = (i == 0) ? value : value - output_boards[i - 1][0];

            break;
          }
        }

        name_string_tmp[index_1][index_board] = tmp_1 + 0x40;
        name_string_tmp[index_1][index_number] = tmp_2 + 0x30;
      }
    }

    unsigned int max_number_changers_in_record = buffer_for_manu_read_record[13];
    unsigned int position_temp;
    unsigned int index_of_ekran;
    unsigned int diagnostic_old[N_DIAGN], diagnostic_new[N_DIAGN], diagnostic_changing[N_DIAGN];

    for (size_t i = 0; i < N_DIAGN_BYTES; i++)
    {
      size_t n_word = i >> 2;
      size_t n_byte = i & 0x3;
      if (n_byte == 0)
      {
        diagnostic_old[n_word] = 0;
        diagnostic_new[n_word] = 0;
      }
      diagnostic_old[n_word] |= buffer_for_manu_read_record[14 + i] << (8 * n_byte);
      diagnostic_new[n_word] |= buffer_for_manu_read_record[14 + N_DIAGN_BYTES + i] << (8 * n_byte);
    }

    //Визначаємо, які сигнали змінилися
    for (size_t i = 0; i < N_DIAGN; i++)
      diagnostic_changing[i] = diagnostic_new[i] ^ diagnostic_old[i];

    //Перевіряємо, чи ми не вийшли за границі
    if (current_ekran.index_position < 0)
      current_ekran.index_position = max_number_changers_in_record - 1;
    else if (current_ekran.index_position >= ((int) max_number_changers_in_record))
      current_ekran.index_position = 0;
    position_in_current_level_menu[EKRAN_CHANGES_DIAGNOSTICS_PR_ERR] = current_ekran.index_position;

    position_temp = current_ekran.index_position;
    index_of_ekran = position_temp & (unsigned int) (~((1 << (POWER_MAX_ROW_LCD >> 1)) - 1));

    //Копіюємо  рядки у робочий екран
    for (unsigned int i = 0; i < (MAX_ROW_LCD >> 1); i++)
    {
      //Наступні рядки треба перевірити, чи їх требе відображати у текучій кофігурації
      if (index_of_ekran < max_number_changers_in_record)
      {
        //Шукаємо функцію, яку треба зараз відобразити
        unsigned int current_number_changes = 0;
        unsigned int index_of_diagnostic_in_the_slice = 0; //починаємо з першої функції
        do
        {
          if (_CHECK_SET_BIT(diagnostic_changing, index_of_diagnostic_in_the_slice) != 0)
            current_number_changes++;
          if (current_number_changes < (index_of_ekran + 1))
            index_of_diagnostic_in_the_slice++;
        } while (
          (current_number_changes < (index_of_ekran + 1)) &&
          (index_of_diagnostic_in_the_slice < MAX_ROW_FOR_DIAGNOSTYKA));

        if (index_of_diagnostic_in_the_slice < MAX_ROW_FOR_DIAGNOSTYKA)
        {
          for (unsigned int k = 0; k < 2; k++)
          {
            if (((i << 1) + k) < MAX_ROW_LCD)
            {
              if (k == 0)
              {
                //У першому рядку відображаємо назву діагностики, який змінився
                for (unsigned int j = 0; j < MAX_COL_LCD; j++)
                  working_ekran[(i << 1) + k][j] = name_string_tmp[index_of_diagnostic_in_the_slice][j];
              }
              else
              {
                //У другому рядку відображаємо яке значення прийняла зараз дана діагностика
                unsigned char second_row[MAX_COL_LCD] = "                ";

                static unsigned char const passive_active[MAX_NAMBER_LANGUAGE][2][5] =
                  {
                    {"Пасс.", "Акт. "},
                    {"Пас. ", "Акт. "},
                    {"Pass ", "Act  "},
                    {"Пасс.", "Акт. "}};

                //Визначаємо стан даного дискретного сигналу
                if (_CHECK_SET_BIT(diagnostic_new, index_of_diagnostic_in_the_slice) != 0)
                {
                  //Сигнал перейшов у активний стан
                  for (int j = 0; j < 4; j++)
                    second_row[1 + j] = passive_active[index_language][1][j];
                }
                else
                {
                  //Сигнал перейшов у пасивний стан
                  for (int j = 0; j < 4; j++)
                    second_row[1 + j] = passive_active[index_language][0][j];
                }

                for (unsigned int j = 0; j < MAX_COL_LCD; j++)
                  working_ekran[(i << 1) + k][j] = second_row[j];
              }
            }
          }
        }
        else
        {
          static unsigned char const error_string[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] =
            {
              {" Неопределенная ",
               " ошибка отображ."},
              {"  Невизначена   ",
               " помилка відобр."},
              {"   Undefined    ",
               " display error  "},
              {" Неопределенная ",
               " ошибка отображ."}};
          for (unsigned int k = 0; k < 2; k++)
          {
            if (((i << 1) + k) < MAX_ROW_LCD)
              for (unsigned int j = 0; j < MAX_COL_LCD; j++)
                working_ekran[(i << 1) + k][j] = error_string[index_language][k][j];
          }
        }
      }
      else
      {
        for (unsigned int k = 0; k < 2; k++)
        {
          if (((i << 1) + k) < MAX_ROW_LCD)
            for (unsigned int j = 0; j < MAX_COL_LCD; j++)
              working_ekran[(i << 1) + k][j] = ' ';
        }
      }

      index_of_ekran++;
    }

    //Відображення курору по вертикалі
    current_ekran.position_cursor_y = (position_temp << 1) & (MAX_ROW_LCD - 1);
    //Курсор видимий
    current_ekran.cursor_on = 1;
  }
  else
  {
    //Пеший байт не сходиться із міткою початку запису - робимо висновок, що у біфері не достовірні дані
    static unsigned char const name_string[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] =
      {
        {" Недостоверные  ",
         "     данные     "},
        {"  Недостовірні  ",
         "      дані      "},
        {"    Invalid     ",
         "      data      "},
        {" Недостоверные  ",
         "     данные     "}};

    //Копіюємо  рядки у робочий екран
    for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
    {
      //Наступні рядки треба перевірити, чи їх требе відображати у текучій коффігурації
      if (i < 2)
        for (unsigned int j = 0; j < MAX_COL_LCD; j++)
          working_ekran[i][j] = name_string[index_language][i][j];
      else
        for (unsigned int j = 0; j < MAX_COL_LCD; j++)
          working_ekran[i][j] = ' ';
    }

    //Відображення курору по вертикалі
    current_ekran.position_cursor_y = 0;
    //Курсор не видимий
    current_ekran.cursor_on = 0;

    current_ekran.index_position = 0;
  }

  //Курсор по горизонталі відображається на першій позиції
  current_ekran.position_cursor_x = 0;
  //Курсор не мигає
  current_ekran.cursor_blinking_on = 0;
  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/

/*****************************************************/

/*****************************************************/

//=====================================================================================================
//''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
//    Для відображення елементів дат
//....................................................................................................
//=====================================================================================================

// ----------------    -------------------------
void make_ekran_data_elem_stt_registrator(void)
{
  int index_language = index_language_in_array(current_settings.language);

  DiffInfoDsc *pDiffInfo = &hldDiffInfo;
  DateHolder baseDate;
  // asm volatile(
  //           "bkpt 1"
  //           );
  FillDiffInfo();
  // asm volatile(
  //           "bkpt 1"
  //           );
  //?sLV.data_correct = 0;
  if (current_ekran.index_position != selectorOneDateElem)
    current_ekran.index_position = selectorOneDateElem;
  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  if ((pDiffInfo->amountDifferentOneDateElms > 0)) //?holderCmdPlusTime.shTotalFixElem
  {
    unsigned char name_string[MAX_ROW_FOR_EKRAN_DATA_LABEL][MAX_COL_LCD] =
      {
        "   XX-XX-20XX   ",
      };
    //
    index_of_ekran = ((position_temp) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;
    //

    for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
    {
      if ((index_of_ekran) < (unsigned int) (pDiffInfo->amountDifferentOneDateElms)) //holderCmdPlusTime.shTotalFixElem
      {

        baseDate = pDiffInfo->arrDatePlRefTags[index_of_ekran].dateHolder;
        int field;
        //День
        field = baseDate.mday; //!@p->tm_mday;
        name_string[0][3] = (field / 10) + 0x30;
        name_string[0][4] = (field % 10) + 0x30;

        //Місяць
        field = baseDate.mon + 1; //!@p->tm_mon + 1;
        name_string[0][6] = (field / 10) + 0x30;
        name_string[0][7] = (field % 10) + 0x30;

        //Рік
        field = baseDate.year - 100; //!@p->tm_year - 100;
        name_string[0][11] = (field / 10) + 0x30;
        name_string[0][12] = (field % 10) + 0x30;

        for (unsigned int j = 0; j < MAX_COL_LCD; j++)
          working_ekran[i][j] = name_string[(0)][j]; //
      }
      else
        for (unsigned int j = 0; j < MAX_COL_LCD; j++)
          working_ekran[i][j] = ' ';

      index_of_ekran++;
    }

    //Відображення курору по вертикалі і курсор завжди має бути у полі із значенням
    current_ekran.position_cursor_x = 2;
    current_ekran.position_cursor_y = ((position_temp)) & (MAX_ROW_LCD - 1);
    selectorOneDateElem = position_temp;
    if (fierstViewAfterEnterKeyPressed != 0)
    {
      fierstSelectedOneDateElem = selectorOneDateElem;
    }
    //?//Курсор мигає
    //?current_ekran.cursor_blinking_on = 1;
    //Курсор видимий
    current_ekran.cursor_on = 1;
  }
  else
  {

    //Немає записів
    static const unsigned char information[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] =
      {
        {"      Нет       ",
         "    записей     "},
        {"     Нема       ",
         "    записів     "},
        {"    Records     ",
         "   are absent   "},
        {"      Нет       ",
         "    записей     "}};
    unsigned int position = current_ekran.index_position;
    position_temp = position;

    index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

    //Копіюємо  рядки у робочий екран
    for (size_t i = 0; i < MAX_ROW_LCD; ++i)
    {
      unsigned int const real_index = index_of_ekran;
      //Наступні рядки треба перевірити, чи їх требе відображати у текучій коффігурації
      if (real_index < 2)
        for (size_t j = 0; j < MAX_COL_LCD; ++j)
          working_ekran[i][j] = information[index_language][real_index][j];
      else
        for (size_t j = 0; j < MAX_COL_LCD; ++j)
          working_ekran[i][j] = ' ';

      index_of_ekran++;
    }

    //? //Курсор видимий
    //? current_ekran.cursor_on = 1;
    //? //Відображення курору по вертикалі
    //? current_ekran.position_cursor_y = position_temp & (MAX_ROW_LCD - 1);
    //Відображення курору по вертикалі
    current_ekran.position_cursor_y = 0;
    //Курсор не видимий
    current_ekran.cursor_on = 0;

    current_ekran.index_position = 0;
  }

  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/
//=====================================================================================================
//''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
//  для виведення дати з пробілом
//....................................................................................................
//=====================================================================================================
long Fill2LineDateWhithBlankStr(void *pv);
// ----------------    -------------------------
long Fill2LineDateWhithBlankStr(void *pv)
{
  //
  DateHolder baseDate;
  long i = scrTwoLnCalcInfo.selectorOneDateElm; //((Fun_make_ekran_Vars*)pv)->selectorOneDateElm;
  unsigned char *p2LineStr = ((Func_make_ekran_variables *) pv)->pvStrBuf;
  //  unsigned char name_string[MAX_ROW_FOR_EKRAN_DATA_LABEL][MAX_COL_LCD] =
  //        {
  //          "   XX-XX-20XX   ",
  //        };
  baseDate = hldDiffInfo.arrDatePlRefTags[i].dateHolder;
  int field;
  //День
  field = baseDate.mday; //!@p->tm_mday;
  p2LineStr[3] = (field / 10) + 0x30;
  p2LineStr[4] = (field % 10) + 0x30;

  //Місяць
  field = baseDate.mon + 1;
  ; //!@p->tm_mon + 1;
  p2LineStr[6] = (field / 10) + 0x30;
  p2LineStr[7] = (field % 10) + 0x30;

  //Рік
  field = baseDate.year - 100;
  ; //!@p->tm_year - 100;
  p2LineStr[11] = (field / 10) + 0x30;
  p2LineStr[12] = (field % 10) + 0x30;
  //Fill ret buf with blank
  return 0;
}
//
//--------------------------------------------------------------------------------------------------------
//````````````````````````````````````````````````````````````````````````````````````````````````````````
//=====================================================================================================
//''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
//
//....................................................................................................
//=====================================================================================================
long Fill2LineTimeMsCmdValCmdNameStr(void *pv);
// ----------------    -------------------------
long Fill2LineTimeMsCmdValCmdNameStr(void *pv)
{
  uint8_t *name_string_tmp; //[NUMBER_TOTAL_SIGNAL_FOR_RANG][MAX_COL_LCD];
  long error = 0;
  name_string_tmp = ((Func_make_ekran_variables *) pv)->name_string_tmp;
  unsigned char *p2LineStr = ((Func_make_ekran_variables *) pv)->pvStrBuf;
  //?DiffInfoDsc *pDiffInfo = &hldDiffInfo;
  TwoLnCalcInfo *pTwoLnCalcInfo = &scrTwoLnCalcInfo;
  register unsigned long *pU32;
  static const unsigned char passive_active[MAX_NAMBER_LANGUAGE][2][3] =
    {
      {"OFF", "ON "}, //
      {"OFF", "ON "}, //
      {"OFF", "ON "}, //
      {"OFF", "ON "}  //
    };

  //?. unsigned char time_string[MAX_ROW_FOR_EKRAN_DATA_LABEL][MAX_COL_LCD] =
  //?. {
  //?.   "XX:XX:XX.XXX    ",
  //?.   "   XX-XX-20XX   "//."   XX-XX-20XX   "
  //?. };
  //?unsigned int max_number_changers_in_record = buffer_for_manu_read_record[FIRST_INDEX_NUMBER_CHANGES_DR] | (buffer_for_manu_read_record[FIRST_INDEX_NUMBER_CHANGES_DR + 1]<<8);
  //.unsigned int max_number_changers_in_record = current_number_changes_of_stt_cmd_into_menu;

  //..unsigned int position_temp;
  //..unsigned int index_of_ekran,ms_time;
  unsigned int array_old[N_BIG], array_new[N_BIG], array_changing[N_BIG];
  //.long  idx_record_of_stt_cmd_prev = 0;

  struct
  {
    char compare_res_time_dat, compare_res_time_ms, counter_reading, compare_res;
    char data_correct;
    time_t time_dat_tmp, copy_time_dat_tmp;
    int32_t time_ms_tmp, copy_time_ms_tmp;

  } sLV;
  sLV.data_correct = 0;
  sLV.counter_reading = 3;
  sLV.data_correct = 0;

  register long l = pTwoLnCalcInfo->selectorCmdPlusTimeStateElem;
  do
  {
    //?GetDateTimeLogElem((unsigned int *)&(sLV.time_dat_tmp),pTwoLnCalcInfo->selectorCmdPlusTimeStateElem);//>>1
    //?GetMsLogElem      ((unsigned int *)&(sLV.time_ms_tmp ),pTwoLnCalcInfo->selectorCmdPlusTimeStateElem);//>>1
    pU32 = &(holderCmdPlusTime.arrCmdPlusTimeHolder[l].unix_time.arU32MkTime[0]);
    memcpy((void *) &(sLV.time_dat_tmp), (const void *) pU32, sizeof(UNN_UnixTime));

    pU32 = &(holderCmdPlusTime.arrCmdPlusTimeHolder[l].mksec.uLMkTime);
    memcpy((void *) &(sLV.time_ms_tmp), (const void *) pU32, sizeof(UNN_MicroSec));

    //?register long lIdx = pTwoLnCalcInfo->selectorCmdPlusTimeStateElem;//
    //?GetCmdPlusTimeLogElem(array_new ,lIdx);
    //?lIdx++;//Next elem on menu older
    //?if (lIdx >= AMOUNT_CMD_PLUS_TIME_RECORD)
    //?    lIdx -= AMOUNT_CMD_PLUS_TIME_RECORD;
    //?GetCmdPlusTimeLogElem(array_old ,lIdx);
    pU32 = &(holderCmdPlusTime.arrCmdPlusTimeHolder[l].cmd.uLCmd[0]);
    memcpy((void *) array_new, (const void *) pU32, sizeof(UNN_CmdState));
    long k = l;
    if (l == 0)
      l = AMOUNT_CMD_PLUS_TIME_RECORD - 1;
    else
      l--;
    pU32 = &(holderCmdPlusTime.arrCmdPlusTimeHolder[l].cmd.uLCmd[0]);
    memcpy((void *) array_old, (const void *) pU32, sizeof(UNN_CmdState));

    //?GetDateTimeLogElem((unsigned int *)&(sLV.copy_time_dat_tmp),pTwoLnCalcInfo->selectorCmdPlusTimeStateElem);//>>1
    //?GetMsLogElem      ((unsigned int *)&(sLV.copy_time_ms_tmp ),pTwoLnCalcInfo->selectorCmdPlusTimeStateElem);//1
    pU32 = &(holderCmdPlusTime.arrCmdPlusTimeHolder[k].unix_time.arU32MkTime[0]);
    memcpy((void *) &(sLV.copy_time_dat_tmp), (const void *) pU32, sizeof(UNN_UnixTime));

    pU32 = &(holderCmdPlusTime.arrCmdPlusTimeHolder[k].mksec.uLMkTime);
    memcpy((void *) &(sLV.copy_time_ms_tmp), (const void *) pU32, sizeof(UNN_MicroSec));

    if (sLV.time_dat_tmp == sLV.copy_time_dat_tmp)
    {

      sLV.compare_res_time_dat = 1;
    }
    else
      sLV.compare_res_time_dat = 0;
    if (sLV.time_ms_tmp == sLV.copy_time_ms_tmp)
    {

      sLV.compare_res_time_ms = 1;
    }
    else
      sLV.compare_res_time_ms = 0;
    sLV.compare_res = sLV.compare_res_time_ms + sLV.compare_res_time_dat;
  } while ((--sLV.counter_reading > 0) && (sLV.compare_res != 2));

  if (sLV.compare_res == 2)
  {
    struct tm *p = localtime(&sLV.time_dat_tmp); //?time_dat_tmp
    int field;
    //Година
    field = p->tm_hour;
    p2LineStr[0] = (field / 10) + 0x30;
    p2LineStr[1] = (field % 10) + 0x30;

    //Хвилини
    field = p->tm_min;
    p2LineStr[3] = (field / 10) + 0x30;
    p2LineStr[4] = (field % 10) + 0x30;

    //Секунди
    field = p->tm_sec;
    p2LineStr[6] = (field / 10) + 0x30;
    p2LineStr[7] = (field % 10) + 0x30;

    //Тисячні секунд
    field = sLV.time_ms_tmp; //?time_ms_tmp;
    p2LineStr[9] = (field / 100) + 0x30;
    field %= 100;

    p2LineStr[10] = (field / 10) + 0x30;
    field %= 10;

    p2LineStr[11] = field + 0x30;

    //Шукаємо масиви зрізів: попередній до вибраної зміни і у момент даної зміни
    //?unsigned int index_of_the_slice = 0; //починаємо з першого зрізу
    short current_number_changes = 0;
    //Визначаємо, які сигнали змінилися
    for (unsigned int j = 0; j < N_BIG; j++)
      array_changing[j] = array_new[j] ^ array_old[j];

    //Шукаємо функцію, яку треба зараз відобразити
    int index_of_function_in_the_slice = 0; //починаємо з першого зрізу
    do
    {
      if ((array_changing[index_of_function_in_the_slice >> 5] & (1 << (index_of_function_in_the_slice & ((1 << 5) - 1)))) != 0)
        current_number_changes++;
      if (current_number_changes < (pTwoLnCalcInfo->selIdxBitCmdPlusTimeStateElem + 1))
        index_of_function_in_the_slice++;
    } while (
      (current_number_changes < (pTwoLnCalcInfo->selIdxBitCmdPlusTimeStateElem + 1)) &&
      (index_of_function_in_the_slice < NUMBER_TOTAL_SIGNAL_FOR_RANG));
    int index_language = index_language_in_array(current_settings.language);

    unsigned int local_index1 = 0;
    //Визначаємо стан даного дискретного сигналу
    if ((array_new[index_of_function_in_the_slice >> 5] & (1 << (index_of_function_in_the_slice & ((1 << 5) - 1)))) != 0)
    {
      //Сигнал перейшов у активний стан
      for (local_index1 = 13; local_index1 < MAX_COL_LCD; local_index1++)
        p2LineStr[local_index1] = passive_active[index_language][1][local_index1 - 13];
    }
    else
    {
      //Сигнал перейшов у пасивний стан
      for (local_index1 = 13; local_index1 < MAX_COL_LCD; local_index1++)
        p2LineStr[local_index1] = passive_active[index_language][0][local_index1 - 13];
    }
    //Name Comand
    //У першому рядку відображаємо назву сигналу, який змінився
    //!for (unsigned int j = 0; j<MAX_COL_LCD; j++)
    //!	working_ekran[(i<<1)+k][j] = name_string_tmp[index_of_function_in_the_slice][j];
    for (unsigned int j = 0; j < MAX_COL_LCD; j++)
      p2LineStr[MAX_COL_LCD + j] = name_string_tmp[index_of_function_in_the_slice * MAX_COL_LCD + j];

  } //Інакше вивести невизначену помилку відображення
  else
    error = -4;
  return error;
}
//
//--------------------------------------------------------------------------------------------------------
//````````````````````````````````````````````````````````````````````````````````````````````````````````
//=====================================================================================================
//''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
// make ekran changing signals for stt reg for 2Line Str
// use presentation format DateWhithBlankSt
// TimeMs CmdVal CmdName BlankStr
//....................................................................................................
//=====================================================================================================
void make_ekran_time_ms_c_val_c_name_sr_format_changing_signals(void);
long Fill2LineTimeMsCmdValCmdNameStr(void *pv);
long Fill2LineDateWhithBlankStr(void *pv);
// ----------------    -------------------------
void make_ekran_time_ms_c_val_c_name_sr_format_changing_signals(void)
{
  int index_language = index_language_in_array(current_settings.language);
  // asm volatile(
  //           "bkpt 1"
  //           );
  long errorIndicator = 0;
  if ((hldDiffInfo.amountDifferentOneDateElms > 0))
  { //&& ( < (uint32_t))
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
    static const unsigned char name_string[MAX_NAMBER_LANGUAGE][NUMBER_TOTAL_SIGNAL_FOR_RANG + (1 - N_IN_GOOSE) + (1 - N_IN_MMS) + (1 - N_OUT_LAN) + (3 - NUMBER_UP_SIGNAL_FOR_RANG)][MAX_COL_LCD] =
#else
    static const unsigned char name_string[MAX_NAMBER_LANGUAGE][NUMBER_TOTAL_SIGNAL_FOR_RANG + (3 - NUMBER_UP_SIGNAL_FOR_RANG)][MAX_COL_LCD] =
#endif
      {
        {NAME_RANG_RU},
        {NAME_RANG_UA},
        {NAME_RANG_EN},
        {NAME_RANG_KZ}};
    //?#ifndef cmt3_off
    uint8_t name_string_tmp[NUMBER_TOTAL_SIGNAL_FOR_RANG][MAX_COL_LCD];
    for (int index_1 = 0; index_1 < NUMBER_TOTAL_SIGNAL_FOR_RANG; index_1++)
    {
      size_t index_row;
      if (index_1 < NUMBER_GENERAL_SIGNAL_FOR_RANG)
      {
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
        if (index_1 < (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_IN_GOOSE + N_IN_MMS + N_OUT_LAN)))
        {
          index_row = index_1;
        }
        else if (index_1 < (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_IN_MMS + N_OUT_LAN)))
        {
          index_row = RANG_BLOCK_IN_GOOSE1 + ((index_1 - (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_IN_GOOSE + N_IN_MMS + N_OUT_LAN))) % 1);
        }
        else if (index_1 < (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_OUT_LAN)))
        {
          index_row = RANG_BLOCK_IN_MMS1 + (1 - N_IN_GOOSE) + ((index_1 - (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_IN_MMS + N_OUT_LAN))) % 1);
        }
        else
        {
          index_row = RANG_BLOCK_OUT_LAN1 + (1 - N_IN_GOOSE) + (1 - N_IN_MMS) + ((index_1 - (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_OUT_LAN))) % 1);
        }
#else
        index_row = index_1;
#endif
      }
      else if (index_1 < RANG_BLOCK_UP1)
      {
        index_row = index_1
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                    + 1 - N_IN_GOOSE + 1 - N_IN_MMS + 1 - N_OUT_LAN
#endif
          ;
      }
      else if (index_1 < (RANG_BLOCK_UP1 + NUMBER_UP_SIGNAL_FOR_RANG))
      {
        index_row = RANG_BLOCK_UP1
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                    + 1 - N_IN_GOOSE + 1 - N_IN_MMS + 1 - N_OUT_LAN
#endif
                    + ((index_1 - RANG_BLOCK_UP1) % 3);
      }
      else
      {
        index_row = index_1
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                    + 1 - N_IN_GOOSE + 1 - N_IN_MMS + 1 - N_OUT_LAN
#endif
                    + 3 - NUMBER_UP_SIGNAL_FOR_RANG;
      }

      for (size_t index_2 = 0; index_2 < MAX_COL_LCD; index_2++)
      {
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
        if (
          (index_1 >= (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_IN_GOOSE + N_IN_MMS + N_OUT_LAN))) &&
          (index_1 < (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_IN_MMS + N_OUT_LAN))) &&
          (index_2 >= index_n_In_GOOSE[index_language][(index_1 - RANG_BLOCK_IN_GOOSE1) % 1]) &&
          (index_2 <= (index_n_In_GOOSE[index_language][(index_1 - RANG_BLOCK_IN_GOOSE1) % 1] + 1)))
        {
          unsigned int n = index_1 - RANG_BLOCK_IN_GOOSE1;
          if ((n + 1) < 10)
          {
            if (index_2 == index_n_In_GOOSE[index_language][n % 1])
              name_string_tmp[index_1][index_2] = 0x30 + (n + 1);
            else
              name_string_tmp[index_1][index_2] = ' ';
          }
          else
          {
            if (index_2 == index_n_In_GOOSE[index_language][n % 1])
              name_string_tmp[index_1][index_2] = 0x30 + (n / 1 + 1) / 10;
            else
              name_string_tmp[index_1][index_2] = 0x30 + (n / 1 + 1) % 10;
          }
        }
        else if (
          (index_1 >= (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_IN_MMS + N_OUT_LAN))) &&
          (index_1 < (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_OUT_LAN))) &&
          (index_2 == index_n_In_MMS[index_language][(index_1 - RANG_BLOCK_IN_MMS1) % 1]))
        {
          name_string_tmp[index_1][index_2] = 0x30 + ((index_1 - RANG_BLOCK_IN_MMS1) / 1 + 1);
        }
        else if (
          (index_1 >= (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_OUT_LAN))) &&
          (index_1 < (NUMBER_GENERAL_SIGNAL_FOR_RANG)) &&
          (index_2 == index_n_Out_LAN[index_language][(index_1 - RANG_BLOCK_OUT_LAN1) % 1]))
        {
          name_string_tmp[index_1][index_2] = 0x30 + ((index_1 - RANG_BLOCK_OUT_LAN1) / 1 + 1);
        }
        else
#endif
        {
          if (
            (index_1 >= RANG_BLOCK_UP1) &&
            (index_1 < (RANG_BLOCK_UP1 + NUMBER_UP_SIGNAL_FOR_RANG)) &&
            (index_2 == index_number_UP[index_language][(index_1 - RANG_BLOCK_UP1) % 3]))
          {
            name_string_tmp[index_1][index_2] = 0x30 + ((index_1 - RANG_BLOCK_UP1) / 3 + 1);
          }
          else
            name_string_tmp[index_1][index_2] = name_string[index_language][index_row][index_2];
        }
      }
    }
    //?#endif
    //?#ifndef cmt3_off

    //asm volatile(
    //          "bkpt 1"
    //          );
    Fill2LnScrElemCalcInfo((void *) &scrTwoLnCalcInfo, 0xffff); //?FillTwoLineElemInfoForCurrEkrIndexPos1(0xffff);

    maxAmountTwoLineSttLogElem = scrTwoLnCalcInfo.calculatedScrIndexPos;
    //Перевіряємо, чи ми не вийшли за границі
    if (current_ekran.index_position < 0)
    {
      current_ekran.index_position = maxAmountTwoLineSttLogElem - 1;
      fierstViewAfterEnterKeyPressed = 0;
    }
    else if (current_ekran.index_position >= ((int) maxAmountTwoLineSttLogElem))
    {
      current_ekran.index_position = 0;
      fierstViewAfterEnterKeyPressed = 0;
    }
    //
    position_in_current_level_menu[EKRAN_CHANGES_SIGNALS_DR] = current_ekran.index_position;

    unsigned int position_temp = current_ekran.index_position; //

    unsigned int index_of_ekran = position_temp & (unsigned int) (~((1 << (POWER_MAX_ROW_LCD >> 1)) - 1));
    //  index_of_ekran serve for view info started from  current_ekran.index_position
    //  index_of_ekran всеодно будемо вирівнювати для зрчності поки index_of_ekran Це індекс 2LineElem фактично
    //FillTwoLineElemInfoForCurrEkrIndexPos1(position_temp);//position_temp serveed mainly for debug purposes
    Fill2LnScrElemCalcInfo((void *) &scrTwoLnCalcInfo, position_temp);
    unsigned char two_line_string[MAX_ROW_FOR_EKRAN_DATA_LABEL][MAX_COL_LCD] =
      {
        "XX:XX:XX.XXX    ",
        "   XX-XX-20XX   "};
    unsigned char date_str[MAX_ROW_FOR_EKRAN_DATA_LABEL][MAX_COL_LCD] =
      {
        "   XX-XX-20XX   ",
        "                ",
      };
    Func_make_ekran_variables funcVars = {
      //?&scrTwoLnCalcInfo,
      //?&hldDiffInfo,
      (uint8_t *) name_string_tmp,
      (unsigned char *) two_line_string,
      0};

    struct
    {
      char clrRowScr;
      char data_correct;
      unsigned short scrIndexPos, mnuIndexPos;
      long counter2LineTimeMsNameCmdElm;

    } sLV;

    //.sLV.data_correct = 0; sLV.pTwoLnCalcInfo = &scrTwoLnCalcInfo;sLV.pDiffInfo       = &hldDiffInfo;
    sLV.clrRowScr = 0;
    sLV.counter2LineTimeMsNameCmdElm = 0;
    sLV.mnuIndexPos = position_temp;
    sLV.scrIndexPos = index_of_ekran;
    TwoLnCalcInfo *pTwoLnCalcInfo = &scrTwoLnCalcInfo;
    //TwoLnCalcInfo arTwoLnCalcInfo[2];WhatToDo arWhatToDo[2];
    //for (unsigned int i_row = 0,copy_index_of_ekran = index_of_ekran; i_row< (MAX_ROW_LCD>>1); i_row++)
    //    FillTwoLineElemInfoForCurrEkrIndexPos(copy_index_of_ekran);
    //    arTwoLnCalcInfo[i_row] = scrTwoLnCalcInfo;
    //    copy_index_of_ekran++;
    //}
    long idxInArrDatePlRefTags = pTwoLnCalcInfo->selectorOneDateElm;
    OneDateElmsDsc selectedOneDateElms = hldDiffInfo.arrDatePlRefTags[idxInArrDatePlRefTags];
    if ((selectedOneDateElms.numFierstElemForThisDate == pTwoLnCalcInfo->selectorCmdPlusTimeStateElem) && (pTwoLnCalcInfo->selIdxBitCmdPlusTimeStateElem == 0))
      pTwoLnCalcInfo->boolEvtScr.bFierstOneDateElem = 1; //headList
    else if (((selectedOneDateElms.numFierstElemForThisDate + (selectedOneDateElms.amountElemForThisDate - 1)) == pTwoLnCalcInfo->selectorCmdPlusTimeStateElem) && (pTwoLnCalcInfo->selIdxBitCmdPlusTimeStateElem ==
                                                                                                                                                                    (pTwoLnCalcInfo->calculatedMaxScrIndexPos - pTwoLnCalcInfo->calculatedScrIndexPos - 1)))
      pTwoLnCalcInfo->boolEvtScr.bLastOneDateElem = 1; //check End of List

    //?FillTwoLineElemInfoForCurrEkrIndexPos(sLV.mnuIndexPos);//?position_tempScr2LnCalcInfoEvt.
    if (pTwoLnCalcInfo->boolEvtScr.bFierstOneDateElem != 0)
    { //need SHOW_DATE_PLUS_SPACE
      //put 2line date
      memcpy((void *) &working_ekran[0][0], (const void *) date_str, sizeof(date_str));
      funcVars.pvStrBuf = (unsigned char *) &working_ekran[0][0];
      Fill2LineDateWhithBlankStr((void *) &funcVars);

      funcVars.index_of_ekran = sLV.mnuIndexPos; //index_of_ekran
      funcVars.pvStrBuf = (unsigned char *) &working_ekran[2][0];
      memcpy((void *) &working_ekran[2][0], (const void *) two_line_string, sizeof(two_line_string));
      //Bield 2line time  ms Name
      Fill2LineTimeMsCmdValCmdNameStr((void *) &funcVars);
      position_temp |= 1;
    }
    else if (pTwoLnCalcInfo->boolEvtScr.bLastOneDateElem != 0)
    {
      funcVars.index_of_ekran = sLV.mnuIndexPos; //index_of_ekran;//
      funcVars.pvStrBuf = (unsigned char *) &working_ekran[0][0];
      memcpy((void *) &working_ekran[0][0], (const void *) two_line_string, sizeof(two_line_string));
      //Bield 2line time  ms Name
      Fill2LineTimeMsCmdValCmdNameStr((void *) &funcVars);
      memset((void *) &working_ekran[2][0], 0x20, sizeof(two_line_string));
      position_temp &= (unsigned int) ~1;
    }
    else
      //Копіюємо  рядки у робочий екран
      for (unsigned int i_row = 0; i_row < (MAX_ROW_LCD >> 1); i_row++)
      {
        //Наступні рядки треба перевірити, чи їх требе відображати у текучій кофігурації

        if (sLV.mnuIndexPos < maxAmountTwoLineSttLogElem)
        {
          //FillTwoLineElemInfoForCurrEkrIndexPos1(index_of_ekran);//?position_temp
          Fill2LnScrElemCalcInfo((void *) &scrTwoLnCalcInfo, sLV.mnuIndexPos++);
          {
            funcVars.index_of_ekran = sLV.mnuIndexPos;
            funcVars.pvStrBuf = (unsigned char *) &working_ekran[2 * i_row][0];
            memcpy((void *) &working_ekran[i_row << 1][0], (const void *) two_line_string, sizeof(two_line_string));
            //Bield 2line time  ms Name
            Fill2LineTimeMsCmdValCmdNameStr((void *) &funcVars);
            //?counter2LineTimeMsNameCmdElm++;
          }
        }
        else
        {
          for (unsigned int k = 0; k < 2; k++)
          {
            if (((i_row << 1) + k) < MAX_ROW_LCD)
            {
              for (unsigned int j_col = 0; j_col < MAX_COL_LCD; j_col++)
                working_ekran[(i_row << 1) + k][j_col] = ' ';
            }
          }
        }
        //if(index_of_ekran == position_temp)
        //    selectorOneDateElem  = pTwoLnCalcInfo->selectorOneDateElm;
        //index_of_ekran++;
      }
    selectorOneDateElem = pTwoLnCalcInfo->selectorOneDateElm;
    pTwoLnCalcInfo->u8EvtScr &= ~((1 << 2) | (1 << 3));
    //Відображення курору по вертикалі
    current_ekran.position_cursor_y = (position_temp << 1) & (MAX_ROW_LCD - 1);
    //Курсор видимий
    current_ekran.cursor_on = 1;
  }
  else
    errorIndicator = -2;
  if (errorIndicator < 0)
  {
    //Пеший байт не сходиться із міткою початку запису - робимо висновок, що у біфері не достовірні дані
    static const unsigned char name_string[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] =
      {
        {" Недостоверные  ",
         "     данные     "},
        {"  Недостовірні  ",
         "      дані      "},
        {"    Invalid     ",
         "      data      "},
        {" Недостоверные  ",
         "     данные     "}};

    //Копіюємо  рядки у робочий екран
    for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
    {
      //Наступні рядки треба перевірити, чи їх требе відображати у текучій коффігурації
      if (i < 2)
        for (unsigned int j = 0; j < MAX_COL_LCD; j++)
          working_ekran[i][j] = name_string[index_language][i][j];
      else
        for (unsigned int j = 0; j < MAX_COL_LCD; j++)
          working_ekran[i][j] = ' ';
    }

    //Відображення курору по вертикалі
    current_ekran.position_cursor_y = 0;
    //Курсор не видимий
    current_ekran.cursor_on = 0;

    current_ekran.index_position = 0;
  }

  //Курсор по горизонталі відображається на першій позиції
  current_ekran.position_cursor_x = 0;
  //Курсор не мигає
  current_ekran.cursor_blinking_on = 0;
  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
//
//,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

/*****************************************************/
//=====================================================================================================
//''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
//
//....................................................................................................
//=====================================================================================================

// ----------------    -------------------------
void make_ekran_signals_stable_one_in_data_elem_stt_reg(void)
{
  int index_language = index_language_in_array(current_settings.language);
  // asm volatile(
  //           "bkpt 1"
  //           );
  long errorIndicator = 0;

  if ((hldDiffInfo.amountDifferentOneDateElms > 0))
  { //&& ( < (uint32_t))
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
    static const unsigned char name_string[MAX_NAMBER_LANGUAGE][NUMBER_TOTAL_SIGNAL_FOR_RANG + (1 - N_IN_GOOSE) + (1 - N_IN_MMS) + (1 - N_OUT_LAN) + (3 - NUMBER_UP_SIGNAL_FOR_RANG)][MAX_COL_LCD] =
#else
    static const unsigned char name_string[MAX_NAMBER_LANGUAGE][NUMBER_TOTAL_SIGNAL_FOR_RANG + (3 - NUMBER_UP_SIGNAL_FOR_RANG)][MAX_COL_LCD] =
#endif
      {
        {NAME_RANG_RU},
        {NAME_RANG_UA},
        {NAME_RANG_EN},
        {NAME_RANG_KZ}};
    //?#ifndef cmt3_off
    uint8_t name_string_tmp[NUMBER_TOTAL_SIGNAL_FOR_RANG][MAX_COL_LCD];
    for (int index_1 = 0; index_1 < NUMBER_TOTAL_SIGNAL_FOR_RANG; index_1++)
    {
      size_t index_row;
      if (index_1 < NUMBER_GENERAL_SIGNAL_FOR_RANG)
      {
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
        if (index_1 < (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_IN_GOOSE + N_IN_MMS + N_OUT_LAN)))
        {
          index_row = index_1;
        }
        else if (index_1 < (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_IN_MMS + N_OUT_LAN)))
        {
          index_row = RANG_BLOCK_IN_GOOSE1 + ((index_1 - (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_IN_GOOSE + N_IN_MMS + N_OUT_LAN))) % 1);
        }
        else if (index_1 < (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_OUT_LAN)))
        {
          index_row = RANG_BLOCK_IN_MMS1 + (1 - N_IN_GOOSE) + ((index_1 - (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_IN_MMS + N_OUT_LAN))) % 1);
        }
        else
        {
          index_row = RANG_BLOCK_OUT_LAN1 + (1 - N_IN_GOOSE) + (1 - N_IN_MMS) + ((index_1 - (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_OUT_LAN))) % 1);
        }
#else
        index_row = index_1;
#endif
      }
      else if (index_1 < RANG_BLOCK_UP1)
      {
        index_row = index_1
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                    + 1 - N_IN_GOOSE + 1 - N_IN_MMS + 1 - N_OUT_LAN
#endif
          ;
      }
      else if (index_1 < (RANG_BLOCK_UP1 + NUMBER_UP_SIGNAL_FOR_RANG))
      {
        index_row = RANG_BLOCK_UP1
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                    + 1 - N_IN_GOOSE + 1 - N_IN_MMS + 1 - N_OUT_LAN
#endif
                    + ((index_1 - RANG_BLOCK_UP1) % 3);
      }
      else
      {
        index_row = index_1
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                    + 1 - N_IN_GOOSE + 1 - N_IN_MMS + 1 - N_OUT_LAN
#endif
                    + 3 - NUMBER_UP_SIGNAL_FOR_RANG;
      }

      for (size_t index_2 = 0; index_2 < MAX_COL_LCD; index_2++)
      {
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
        if (
          (index_1 >= (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_IN_GOOSE + N_IN_MMS + N_OUT_LAN))) &&
          (index_1 < (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_IN_MMS + N_OUT_LAN))) &&
          (index_2 >= index_n_In_GOOSE[index_language][(index_1 - RANG_BLOCK_IN_GOOSE1) % 1]) &&
          (index_2 <= (index_n_In_GOOSE[index_language][(index_1 - RANG_BLOCK_IN_GOOSE1) % 1] + 1)))
        {
          unsigned int n = index_1 - RANG_BLOCK_IN_GOOSE1;
          if ((n + 1) < 10)
          {
            if (index_2 == index_n_In_GOOSE[index_language][n % 1])
              name_string_tmp[index_1][index_2] = 0x30 + (n + 1);
            else
              name_string_tmp[index_1][index_2] = ' ';
          }
          else
          {
            if (index_2 == index_n_In_GOOSE[index_language][n % 1])
              name_string_tmp[index_1][index_2] = 0x30 + (n / 1 + 1) / 10;
            else
              name_string_tmp[index_1][index_2] = 0x30 + (n / 1 + 1) % 10;
          }
        }
        else if (
          (index_1 >= (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_IN_MMS + N_OUT_LAN))) &&
          (index_1 < (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_OUT_LAN))) &&
          (index_2 == index_n_In_MMS[index_language][(index_1 - RANG_BLOCK_IN_MMS1) % 1]))
        {
          name_string_tmp[index_1][index_2] = 0x30 + ((index_1 - RANG_BLOCK_IN_MMS1) / 1 + 1);
        }
        else if (
          (index_1 >= (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_OUT_LAN))) &&
          (index_1 < (NUMBER_GENERAL_SIGNAL_FOR_RANG)) &&
          (index_2 == index_n_Out_LAN[index_language][(index_1 - RANG_BLOCK_OUT_LAN1) % 1]))
        {
          name_string_tmp[index_1][index_2] = 0x30 + ((index_1 - RANG_BLOCK_OUT_LAN1) / 1 + 1);
        }
        else
#endif
        {
          if (
            (index_1 >= RANG_BLOCK_UP1) &&
            (index_1 < (RANG_BLOCK_UP1 + NUMBER_UP_SIGNAL_FOR_RANG)) &&
            (index_2 == index_number_UP[index_language][(index_1 - RANG_BLOCK_UP1) % 3]))
          {
            name_string_tmp[index_1][index_2] = 0x30 + ((index_1 - RANG_BLOCK_UP1) / 3 + 1);
          }
          else
            name_string_tmp[index_1][index_2] = name_string[index_language][index_row][index_2];
        }
      }
    }
    //?#endif
    //?#ifndef cmt3_off
    register unsigned long *pU32;
    unsigned int array_new[N_BIG];
    //.long  idx_record_of_stt_cmd_prev = 0;
    TwoLnCalcInfo *pTwoLnCalcInfo = &scrTwoLnCalcInfo;
    struct
    {
      char compare_res_time_dat, compare_res_time_ms, counter_reading, compare_res;
      char data_correct;
      time_t time_dat_tmp, copy_time_dat_tmp;
      int32_t time_ms_tmp, copy_time_ms_tmp;

    } sLV;
    sLV.data_correct = 0;
    sLV.counter_reading = 3;
    sLV.data_correct = 0;
    register long l = pTwoLnCalcInfo->selectorCmdPlusTimeStateElem;

    do
    {
      GetDateTimeLogElem((unsigned int *) &(sLV.time_dat_tmp), pTwoLnCalcInfo->selectorCmdPlusTimeStateElem);
      GetMsLogElem((unsigned int *) &(sLV.time_ms_tmp), pTwoLnCalcInfo->selectorCmdPlusTimeStateElem);
      //.pU32 = &(holderCmdPlusTime.arrCmdPlusTimeHolder[l].unix_time.arU32MkTime[0]);
      //.memcpy((void *)&(sLV.time_dat_tmp),(const void*)pU32,    sizeof(UNN_UnixTime));
      //.pU32 = &(holderCmdPlusTime.arrCmdPlusTimeHolder[l].mksec.uLMkTime);
      //.memcpy((void *)&(sLV.time_ms_tmp ),(const void*)pU32,    sizeof(UNN_MicroSec));

      //    register long lIdx = pTwoLnCalcInfo->selectorCmdPlusTimeStateElem;//
      pU32 = &(holderCmdPlusTime.arrCmdPlusTimeHolder[l].cmd.uLCmd[0]);
      memcpy((void *) array_new, (const void *) pU32, sizeof(UNN_CmdState));

      //GetCmdPlusTimeLogElem(array_new ,lIdx);
      //?lIdx++;//Next elem on menu older
      //?if (lIdx >= AMOUNT_CMD_PLUS_TIME_RECORD)
      //?    lIdx -= AMOUNT_CMD_PLUS_TIME_RECORD;
      //?GetCmdPlusTimeLogElem(array_old ,lIdx);
      GetDateTimeLogElem((unsigned int *) &(sLV.copy_time_dat_tmp), pTwoLnCalcInfo->selectorCmdPlusTimeStateElem);
      GetMsLogElem((unsigned int *) &(sLV.copy_time_ms_tmp), pTwoLnCalcInfo->selectorCmdPlusTimeStateElem);
      //.pU32 = &(holderCmdPlusTime.arrCmdPlusTimeHolder[l].unix_time.arU32MkTime[0]);
      //.memcpy((void *)&(sLV.copy_time_dat_tmp),(const void*)pU32,    sizeof(UNN_UnixTime));
      //.pU32 = &(holderCmdPlusTime.arrCmdPlusTimeHolder[l].mksec.uLMkTime);
      //.memcpy((void *)&(sLV.copy_time_ms_tmp ),(const void*)pU32,    sizeof(UNN_MicroSec));

      if (sLV.time_dat_tmp == sLV.copy_time_dat_tmp)
      {

        sLV.compare_res_time_dat = 1;
      }
      else
        sLV.compare_res_time_dat = 0;
      if (sLV.time_ms_tmp == sLV.copy_time_ms_tmp)
      {

        sLV.compare_res_time_ms = 1;
      }
      else
        sLV.compare_res_time_ms = 0;
      sLV.compare_res = sLV.compare_res_time_ms + sLV.compare_res_time_dat;
    } while ((--sLV.counter_reading > 0) && (sLV.compare_res != 2));

    if (sLV.compare_res == 2)
    {
      //asm volatile(
      //          "bkpt 1"
      //          );
      //Перевіряємо, чи ми не вийшли за границі
      long lOrderNumCmd = 0, counterActiveComand = 0;

      do
      {
        if ((array_new[lOrderNumCmd >> 5] & (1 << (lOrderNumCmd & ((1 << 5) - 1)))) != 0)
          counterActiveComand++;

      } while (++lOrderNumCmd < NUMBER_TOTAL_SIGNAL_FOR_RANG);
      unsigned max_number_changers_in_record = counterActiveComand;
      if (current_ekran.index_position < 0)
        current_ekran.index_position = max_number_changers_in_record - 1;
      else if (current_ekran.index_position >= ((int) max_number_changers_in_record))
        current_ekran.index_position = 0;
      current_ekran.index_position = (current_ekran.index_position >> (POWER_MAX_ROW_LCD)) << (POWER_MAX_ROW_LCD);
      position_in_current_level_menu[EKRAN_CHANGES_SIGNALS_DR] = current_ekran.index_position;

      unsigned int position_temp = current_ekran.index_position; //?position_temp = current_ekran.index_position;
      unsigned int index_of_ekran = position_temp & (unsigned int) (~((1 << (POWER_MAX_ROW_LCD)) - 1));

      //Копіюємо  рядки у робочий екран
      for (unsigned int i = 0; i < (MAX_ROW_LCD); i++)
      {
        //Наступні рядки треба перевірити, чи їх требе відображати у текучій кофігурації
        if (index_of_ekran < max_number_changers_in_record)
        {
          //Шукаємо масиви зрізів: попередній до вибраної зміни і у момент даної зміни
          unsigned int current_number_changes = 0;
          //Визначаємо, які сигнали 1
          //Шукаємо функцію, яку треба зараз відобразити
          int index_of_function_in_the_slice = 0; //починаємо з першого зрізу
          do
          {
            if ((array_new[index_of_function_in_the_slice >> 5] & (1 << (index_of_function_in_the_slice & ((1 << 5) - 1)))) != 0)
              current_number_changes++;
            if (current_number_changes < (index_of_ekran + 1))
              index_of_function_in_the_slice++;
          } while (
            (current_number_changes < (index_of_ekran + 1)) &&
            (index_of_function_in_the_slice < NUMBER_TOTAL_SIGNAL_FOR_RANG));
          if (index_of_function_in_the_slice < NUMBER_TOTAL_SIGNAL_FOR_RANG)
          {

            //У першому рядку відображаємо назву сигналу, який змінився
            for (unsigned int j = 0; j < MAX_COL_LCD; j++)
              working_ekran[i][j] = name_string_tmp[index_of_function_in_the_slice][j];

            //#endif
          }
        }
        else
        {
          //for (unsigned int k = 0; k < 2; k++)
          //{
          //  if (((i<<1)+k) < MAX_ROW_LCD)
          //    for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[(i<<1)+k][j] = ' ';
          //}
          for (unsigned int j = 0; j < MAX_COL_LCD; j++)
            working_ekran[i][j] = ' ';
        }

        index_of_ekran++;
      }
      //Відображення курору по вертикалі
      current_ekran.position_cursor_y = (position_temp << 1) & (MAX_ROW_LCD - 1);
      //Курсор невидимий
      current_ekran.cursor_on = 0;
    }
    else
    {
      errorIndicator = -3; //Error
    }
  }
  else
    errorIndicator = -2;
  if (errorIndicator < 0)
  {
    //Пеший байт не сходиться із міткою початку запису - робимо висновок, що у біфері не достовірні дані
    static const unsigned char name_string[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] =
      {
        {" Недостоверные  ",
         "     данные     "},
        {"  Недостовірні  ",
         "      дані      "},
        {"    Invalid     ",
         "      data      "},
        {" Недостоверные  ",
         "     данные     "}};

    //Копіюємо  рядки у робочий екран
    for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
    {
      //Наступні рядки треба перевірити, чи їх требе відображати у текучій коффігурації
      if (i < 2)
        for (unsigned int j = 0; j < MAX_COL_LCD; j++)
          working_ekran[i][j] = name_string[index_language][i][j];
      else
        for (unsigned int j = 0; j < MAX_COL_LCD; j++)
          working_ekran[i][j] = ' ';
    }

    //Відображення курору по вертикалі
    current_ekran.position_cursor_y = 0;
    //Курсор не видимий
    current_ekran.cursor_on = 0;

    current_ekran.index_position = 0;
  }

  //Курсор по горизонталі відображається на першій позиції
  current_ekran.position_cursor_x = 0;
  //Курсор не мигає
  current_ekran.cursor_blinking_on = 0;
  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
//
//--------------------------------------------------------------------------------------------------------
//````````````````````````````````````````````````````````````````````````````````````````````````````````

/*****************************************************/
