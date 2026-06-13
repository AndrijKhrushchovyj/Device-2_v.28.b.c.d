#include "header.h"

/*****************************************************/
//Формуємо екран відображення спику реєстраторів
/*****************************************************/
void make_ekran_chose_registrators(void)
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_LIST_OF_REGISTRATORS][MAX_COL_LCD] =
    {
      {" Дискр. рег-р   ",
       " Аналог.рег-р   "},
      {" Дискр. реєстр. ",
       " Аналог.реєстр. "},
      {" Binary recorder",
       "Disturb recorder"},
      {" Дискр. рег-р   ",
       " Аналог.рег-р   "}};
  int index_language = index_language_in_array(current_settings.language);

  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;

  index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  //Копіюємо  рядки у робочий екран
  for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
  {
    //Наступні рядки треба перевірити, чи їх требе відображати у текучій коффігурації
    if (index_of_ekran < MAX_ROW_FOR_LIST_OF_REGISTRATORS)
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
//Формуємо екран відображення спику настройок аналогового реєстратора
/*****************************************************/
void make_ekran_settings_analog_registrators(void)
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_SETTINGS_OF_ANALOG_REGISTRATOR][MAX_COL_LCD] =
    {
      {" Источники      ",
       " Выдержки       ",
       " Управление     "},
      {" Джерела        ",
       " Витримки       ",
       " Управління     "},
      {" Inputs         ",
       " Operate delays ",
       " Control        "},
      {" Источники      ",
       " Кyрулар        ",
       " Беріктік       "}};
  int index_language = index_language_in_array(current_settings.language);

  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;

  index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  //Копіюємо  рядки у робочий екран
  for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
  {
    //Наступні рядки треба перевірити, чи їх требе відображати у текучій коффігурації
    if (index_of_ekran < MAX_ROW_FOR_SETTINGS_OF_ANALOG_REGISTRATOR)
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
//Формуємо екран відображення витримок Виключателя
/*****************************************************/
void make_ekran_timeout_analog_registrator(void)
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_TIMEOUT_ANALOG_REGISTRATOR][MAX_COL_LCD] =
    {
      {"   Предавар.    ",
       "   Послеавар.   ",
       "  Дискретность  "},
      {"   Передавар.   ",
       "   Післяавар.   ",
       "  Дискретність  "},
      {"    PreFault    ",
       "   PostFault    ",
       "   Resolution   "},
      {"   Предавар.    ",
       "   Послеавар.   ",
       "  Дискретность  "}};

  int index_language = index_language_in_array(current_settings.language);

  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  unsigned int vaga, value, first_symbol;

  //Множення на два величини position_temp потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
  index_of_ekran = ((position_temp << 1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
  {
    unsigned int index_of_ekran_tmp = index_of_ekran >> 1;
    unsigned int view = ((current_ekran.edition == 0) || (position_temp != index_of_ekran_tmp));
    if (index_of_ekran_tmp < MAX_ROW_FOR_TIMEOUT_ANALOG_REGISTRATOR)
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j < MAX_COL_LCD; j++)
          working_ekran[i][j] = name_string[index_language][index_of_ekran_tmp][j];
        if (index_of_ekran_tmp == INDEX_ML_TMOPREFAULT)
        {
          vaga = 1000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки "Час доаварійного масиву"
          if (view == true)
            value = current_settings.prefault_number_periods * 20; //у змінну value поміщаємо значення витримки "Час доаварійного масиву" але виражену не у періодах, а у мілісекундах
          else
            value = edition_settings.prefault_number_periods * 20;
        }
        else if (index_of_ekran_tmp == INDEX_ML_TMOPOSTFAULT)
        {
          vaga = 10000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки "Час пісдяаварійного масиву"
          if (view == true)
            value = current_settings.postfault_number_periods * 20; //у змінну value поміщаємо значення витримки "Час післяаварійного масиву" але виражену не у періодах, а у мілісекундах
          else
            value = edition_settings.postfault_number_periods * 20;
        }
        else if (index_of_ekran_tmp == INDEX_ML_TMODISKRETN)
        {
          vaga = 100; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки "Дискретність"
          if (view == true)
            value = current_settings.diskretnt_number_periods * 20; //у змінну value поміщаємо значення витримки "Час післяаварійного масиву" але виражену не у періодах, а у мілісекундах
          else
            value = edition_settings.diskretnt_number_periods * 20;
        }
        first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
      }
      else
      {
        //У парному номері рядку виводимо значення уставки
        for (unsigned int j = 0; j < MAX_COL_LCD; j++)
        {
          if (index_of_ekran_tmp == INDEX_ML_TMOPREFAULT)
          {
            if (
              ((j < COL_TMO_PREFAULT_BEGIN) || (j > COL_TMO_PREFAULT_END)) &&
              (j != (COL_TMO_PREFAULT_END + 2)))
              working_ekran[i][j] = ' ';
            else if (j == COL_TMO_PREFAULT_COMMA)
              working_ekran[i][j] = ',';
            else if (j == (COL_TMO_PREFAULT_END + 2))
              working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_PREFAULT_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == INDEX_ML_TMOPOSTFAULT)
          {
            if (
              ((j < COL_TMO_POSTFAULT_BEGIN) || (j > COL_TMO_POSTFAULT_END)) &&
              (j != (COL_TMO_POSTFAULT_END + 2)))
              working_ekran[i][j] = ' ';
            else if (j == COL_TMO_POSTFAULT_COMMA)
              working_ekran[i][j] = ',';
            else if (j == (COL_TMO_POSTFAULT_END + 2))
              working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_POSTFAULT_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == INDEX_ML_TMODISKRETN)
          {
            if (
              ((j < COL_TMO_DISKRETN_BEGIN) || (j > COL_TMO_DISKRETN_END)) &&
              (!((j >= (COL_TMO_DISKRETN_END + 2)) && (j <= (COL_TMO_DISKRETN_END + 3)))))
              working_ekran[i][j] = ' ';
            else if ((j >= (COL_TMO_DISKRETN_END + 2)) && (j <= (COL_TMO_DISKRETN_END + 3)))
              working_ekran[i][j] = ms[index_language][j - (COL_TMO_DISKRETN_END + 2)];
            else
              calc_int_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, view);
          }
        }
      }
    }
    else
      for (unsigned int j = 0; j < MAX_COL_LCD; j++)
        working_ekran[i][j] = ' ';

    index_of_ekran++;
  }

  //Відображення курору по вертикалі і курсор завжди має бути у полі із значенням устаки
  current_ekran.position_cursor_y = ((position_temp << 1) + 1) & (MAX_ROW_LCD - 1);
  //Курсор по горизонталі відображається на першому символі у випадку, коли ми не в режимі редагування, інакше позиція буде визначена у функцї main_manu_function
  if (current_ekran.edition == 0)
  {
    int last_position_cursor_x = MAX_COL_LCD;
    if (current_ekran.index_position == INDEX_ML_TMOPREFAULT)
    {
      current_ekran.position_cursor_x = COL_TMO_PREFAULT_BEGIN;
      last_position_cursor_x = COL_TMO_PREFAULT_END;
    }
    else if (current_ekran.index_position == INDEX_ML_TMOPOSTFAULT)
    {
      current_ekran.position_cursor_x = COL_TMO_POSTFAULT_BEGIN;
      last_position_cursor_x = COL_TMO_POSTFAULT_END;
    }
    else if (current_ekran.index_position == INDEX_ML_TMODISKRETN)
    {
      current_ekran.position_cursor_x = COL_TMO_DISKRETN_BEGIN;
      last_position_cursor_x = COL_TMO_DISKRETN_END;
    }

    //Підтягуємо курсор до першого символу
    while (((working_ekran[current_ekran.position_cursor_y][current_ekran.position_cursor_x + 1]) == ' ') &&
           (current_ekran.position_cursor_x < (last_position_cursor_x - 1)))
      current_ekran.position_cursor_x++;

    //Курсор ставимо так, щоб він був перед числом
    if (((working_ekran[current_ekran.position_cursor_y][current_ekran.position_cursor_x]) != ' ') &&
        (current_ekran.position_cursor_x > 0))
      current_ekran.position_cursor_x--;
  }
  //Курсор видимий
  current_ekran.cursor_on = 1;
  //Курсор не мигає
  if (current_ekran.edition == 0)
    current_ekran.cursor_blinking_on = 0;
  else
    current_ekran.cursor_blinking_on = 1;
  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//Формуємо екран відображення значення управлінської інформації для  аналогового реєстратора
/*****************************************************/
void make_ekran_control_ar()
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_CONTROL_AR][MAX_COL_LCD] =
    {
      {" Аварийный проц."},
      {" Аварійний проц."},
      {" Fault process  "},
      {" Аварийный проц."}};
  int index_language = index_language_in_array(current_settings.language);

  int position_temp = current_ekran.index_position;
  int index_of_ekran;

  //Множення на два величини position_temp потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
  index_of_ekran = ((position_temp << 1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
  {
    unsigned int index_of_ekran_tmp = index_of_ekran >> 1;
    if (index_of_ekran_tmp < MAX_ROW_FOR_CONTROL_AR)
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j < MAX_COL_LCD; j++)
          working_ekran[i][j] = name_string[index_language][index_of_ekran_tmp][j];
      }
      else
      {
        //У парному номері рядку виводимо значення уставки
        unsigned int index_ctr = index_of_ekran_tmp;

        unsigned int temp_data;

        if (current_ekran.edition == 0)
          temp_data = current_settings.control_ar;
        else
          temp_data = edition_settings.control_ar;

        for (unsigned int j = 0; j < MAX_COL_LCD; j++)
          working_ekran[i][j] = information_off_on[index_language][(temp_data >> index_ctr) & 0x1][j];
        if (position_temp == index_of_ekran_tmp)
          current_ekran.position_cursor_x = cursor_x_off_on[index_language][(temp_data >> index_ctr) & 0x1];
      }
    }
    else
      for (unsigned int j = 0; j < MAX_COL_LCD; j++)
        working_ekran[i][j] = ' ';

    index_of_ekran++;
  }

  //Відображення курору по вертикалі і курсор завжди має бути у полі із значенням устаки
  current_ekran.position_cursor_y = ((position_temp << 1) + 1) & (MAX_ROW_LCD - 1);
  //Курсор видимий
  current_ekran.cursor_on = 1;
  //Курсор не мигає
  if (current_ekran.edition == 0)
    current_ekran.cursor_blinking_on = 0;
  else
    current_ekran.cursor_blinking_on = 1;
  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//Формуємо екран відображення витримок Bin Rec
/*****************************************************/
void make_ekran_timeout_digital_registrator(void)
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_TIMEOUT_DIGITAL_REGISTRATOR][MAX_COL_LCD] =
    {
      {//      "   Предавар.    ",
       "   Послеавар.   "},
      {//      "   Передавар.   ",
       "   Післяавар.   "},
      {//      "    PreFault    ",
       "   PostFault    "},
      {//      "   Предавар.    ",
       "   Послеавар.   "}};
  int index_language = index_language_in_array(current_settings.language);

  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  unsigned int vaga, value, first_symbol;

  //Множення на два величини position_temp потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
  index_of_ekran = ((position_temp << 1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
  {
    unsigned int index_of_ekran_tmp = index_of_ekran >> 1;
    unsigned int view = ((current_ekran.edition == 0) || (position_temp != index_of_ekran_tmp));
    if (index_of_ekran_tmp < MAX_ROW_FOR_TIMEOUT_DIGITAL_REGISTRATOR)
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j < MAX_COL_LCD; j++)
          working_ekran[i][j] = name_string[index_language][index_of_ekran_tmp][j];
        //.if (index_of_ekran_tmp == INDEX_ML_TMOPREFAULT)
        //.{
        //.  vaga = 1000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки "Час доаварійного масиву"
        //.  if (view == true) value = current_settings.prefault_number_periods*20; //у змінну value поміщаємо значення витримки "Час доаварійного масиву" але виражену не у періодах, а у мілісекундах
        //.  else value = edition_settings.prefault_number_periods*20;
        //.}
        //.else if (index_of_ekran_tmp == INDEX_ML_TMOPOSTFAULT)
        if (index_of_ekran_tmp == INDEX_ML_TMO_ELONGATION)
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки "Час пісдяаварійного масиву"
          if (view == true)
            value = current_settings.timeout_prolongation_work_digital_registrator; //у змінну value поміщаємо значення витримки "Час післяаварійного масиву" але виражену не у періодах, а у мілісекундах
          else
            value = edition_settings.timeout_prolongation_work_digital_registrator;
        }
        first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
      }
      else
      {
        //У парному номері рядку виводимо значення уставки
        for (unsigned int j = 0; j < MAX_COL_LCD; j++)
        {
          //.if (index_of_ekran_tmp == INDEX_ML_TMOPREFAULT)
          //.{
          //.  if (
          //.      ((j < COL_TMO_PREFAULT_BEGIN) ||  (j > COL_TMO_PREFAULT_END )) &&
          //.      (j != (COL_TMO_PREFAULT_END + 2))
          //.     )working_ekran[i][j] = ' ';
          //.  else if (j == COL_TMO_PREFAULT_COMMA )working_ekran[i][j] = ',';
          //.  else if (j == (COL_TMO_PREFAULT_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
          //.  else
          //.    calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_PREFAULT_COMMA, view, 0);
          //.}
          //.else if (index_of_ekran_tmp == INDEX_ML_TMOPOSTFAULT)
          if (index_of_ekran_tmp == INDEX_ML_TMO_ELONGATION)
          {
            if (
              ((j < COL_TMO_ELONGATION_BEGIN) || (j > COL_TMO_ELONGATION_END)) &&
              (j != (COL_TMO_ELONGATION_END + 2)))
              working_ekran[i][j] = ' ';
            else if (j == COL_TMO_ELONGATION_COMMA)
              working_ekran[i][j] = ',';
            else if (j == (COL_TMO_ELONGATION_END + 2))
              working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_ELONGATION_COMMA, view, 0);
          }
        }
      }
    }
    else
      for (unsigned int j = 0; j < MAX_COL_LCD; j++)
        working_ekran[i][j] = ' ';

    index_of_ekran++;
  }

  //Відображення курору по вертикалі і курсор завжди має бути у полі із значенням устаки
  current_ekran.position_cursor_y = ((position_temp << 1) + 1) & (MAX_ROW_LCD - 1);
  //Курсор по горизонталі відображається на першому символі у випадку, коли ми не в режимі редагування, інакше позиція буде визначена у функцї main_manu_function
  if (current_ekran.edition == 0)
  {
    int last_position_cursor_x = MAX_COL_LCD;
    //.if (current_ekran.index_position == INDEX_ML_TMOPREFAULT)
    //.{
    //.  current_ekran.position_cursor_x = COL_TMO_PREFAULT_BEGIN;
    //.  last_position_cursor_x = COL_TMO_PREFAULT_END;
    //.}
    //.else if (current_ekran.index_position == INDEX_ML_TMOPOSTFAULT)
    if (current_ekran.index_position == INDEX_ML_TMO_ELONGATION)
    {
      current_ekran.position_cursor_x = COL_TMO_ELONGATION_BEGIN;
      last_position_cursor_x = COL_TMO_ELONGATION_END;
    }

    //Підтягуємо курсор до першого символу
    while (((working_ekran[current_ekran.position_cursor_y][current_ekran.position_cursor_x + 1]) == ' ') &&
           (current_ekran.position_cursor_x < (last_position_cursor_x - 1)))
      current_ekran.position_cursor_x++;

    //Курсор ставимо так, щоб він був перед числом
    if (((working_ekran[current_ekran.position_cursor_y][current_ekran.position_cursor_x]) != ' ') &&
        (current_ekran.position_cursor_x > 0))
      current_ekran.position_cursor_x--;
  }
  //Курсор видимий
  current_ekran.cursor_on = 1;
  //Курсор не мигає
  if (current_ekran.edition == 0)
    current_ekran.cursor_blinking_on = 0;
  else
    current_ekran.cursor_blinking_on = 1;
  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//Формуємо екран відображення спику настройок bin реєстратора
/*****************************************************/
void make_ekran_settings_digital_registrators(void)
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_SETTINGS_OF_DIGITAL_REGISTRATOR][MAX_COL_LCD] =
    {
      {" Источники      ",
       " Выдержки       "},
      {" Джерела        ",
       " Витримки       "

      },
      {" Inputs         ",
       " Operate delays "

      },
      {" Источники      ",
       " Кyрулар        "

      }};
  int index_language = index_language_in_array(current_settings.language);

  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;

  index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  //Копіюємо  рядки у робочий екран
  for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
  {
    //Наступні рядки треба перевірити, чи їх требе відображати у текучій коффігурації
    if (index_of_ekran < MAX_ROW_FOR_SETTINGS_OF_DIGITAL_REGISTRATOR)
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
//
/*****************************************************/
/*****************************************************/
