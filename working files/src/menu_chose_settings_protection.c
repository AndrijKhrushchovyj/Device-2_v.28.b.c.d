#include "header.h"

/*****************************************************/
//Формуємо екран відображення заголовків настроювання захистів
/*****************************************************/
void make_ekran_chose_settings_any_protection(void)
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_CHOSE_SETTINGS_ANY_PROTECTION][MAX_COL_LCD] =
    {
      {" Уставки        ",
       " Выдержки       ",
       " Управление     "},
      {" Уставки        ",
       " Витримки       ",
       " Управління     "},
      {" Start Values   ",
       " Operate delays ",
       " Control        "},
      {" Баскару        ",
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
    if (index_of_ekran < MAX_ROW_FOR_CHOSE_SETTINGS_ANY_PROTECTION)
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
//Формуємо екран відображення заголовків настроювання захистів (з двома групами уставок)
/*****************************************************/
void make_ekran_chose_settings_protection_with_groups(void)
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_CHOSE_SETTINGS_PROTECTION_WITH_TWO_GROUP][MAX_COL_LCD] =
    {
      {" Группа 1       ",
       " Группа 2       ",
       " Группа 3       ",
       " Группа 4       ",
       " Управление     "},
      {" Група 1        ",
       " Група 2        ",
       " Група 3        ",
       " Група 4        ",
       " Управління     "},
      {" SetPoints Gr 1 ",
       " SetPoints Gr 2 ",
       " SetPoints Gr 3 ",
       " SetPoints Gr 4 ",
       " Control        "},
      {" Группа 1       ",
       " Группа 2       ",
       " Группа 3       ",
       " Группа 4       ",
       " Баскару        "}};
  int index_language = index_language_in_array(current_settings.language);

  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;

  index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  //Копіюємо  рядки у робочий екран
  for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
  {
    //Наступні рядки треба перевірити, чи їх требе відображати у текучій коффігурації
    if (index_of_ekran < MAX_ROW_FOR_CHOSE_SETTINGS_PROTECTION_WITH_TWO_GROUP)
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
//Формуємо екран відображення уставок і витримок для захистів з двома групами уставок
/*****************************************************/
void make_ekran_chose_setpoint_and_timeout(void)
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_CHOSE_SETPOINTS_TIMEOUTS][MAX_COL_LCD] =
    {
      {" Уставки        ",
       " Выдержки       "},
      {" Уставки        ",
       " Витримки       "},
      {" Start Values   ",
       " Operate delays "},
      {" Кyрулар        ",
       " Беріктік       "}};
  int index_language = index_language_in_array(current_settings.language);

  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;

  index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  //Копіюємо  рядки у робочий екран
  for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
  {
    //Наступні рядки треба перевірити, чи їх требе відображати у текучій коффігурації
    if (index_of_ekran < MAX_ROW_FOR_CHOSE_SETPOINTS_TIMEOUTS)
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
//Формуємо екран відображення уставок для захистів з чотирма групами уставок
/*****************************************************/
void make_ekran_chose_setpoint(void)
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_CHOSE_SETTINGS_PROTECTION_WITH_SETPOINTS][MAX_COL_LCD] =
    {
      {" Уставки        "},
      {" Уставки        "},
      {" Start Values   "},
      {" Кyрулар        "}};
  int const index_language = index_language_in_array(current_settings.language);

  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;

  index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  //Копіюємо  рядки у робочий екран
  for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
  {
    //Наступні рядки треба перевірити, чи їх требе відображати у поточній кофігурації
    if (index_of_ekran < MAX_ROW_FOR_CHOSE_SETTINGS_PROTECTION_WITH_SETPOINTS)
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
//Формуємо екран відображення витримок для захистів з чотирма групами уставок
/*****************************************************/
void make_ekran_chose_timeout(void)
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_CHOSE_SETTINGS_PROTECTION_WITH_TIMEOUT][MAX_COL_LCD] =
    {
      {" Выдержки       "},
      {" Витримки       "},
      {" Operate delays "},
      {" Беріктік       "}};
  int index_language = index_language_in_array(current_settings.language);

  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;

  index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  //Копіюємо  рядки у робочий екран
  for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
  {
    //Наступні рядки треба перевірити, чи їх требе відображати у поточній кофігурації
    if (index_of_ekran < MAX_ROW_FOR_CHOSE_SETTINGS_PROTECTION_WITH_TIMEOUT)
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
//Формуємо екран відображення заголовків настроювання де тільки є управління
/*****************************************************/
void make_ekran_chose_control(void)
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_CHOSE_SETTINGS_PROTECTION_WITH_CONTROL][MAX_COL_LCD] =
    {
      {" Управление     "},
      {" Управління     "},
      {" Control        "},
      {" Баскару        "}};
  int index_language = index_language_in_array(current_settings.language);

  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;

  index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  //Копіюємо  рядки у робочий екран
  for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
  {
    if (index_of_ekran < MAX_ROW_FOR_CHOSE_SETTINGS_PROTECTION_WITH_CONTROL)
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
//Формуємо екран відображення заголовків настроювання де є витримки і управління
/*****************************************************/
void make_ekran_chose_timeout_control(void)
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_CHOSE_SETTINGS_PROTECTION_WITH_TIMEOUT_CONTROL][MAX_COL_LCD] =
    {
      {" Выдержки       ",
       " Управление     "},
      {" Витримки       ",
       " Управління     "},
      {" Operate delays ",
       " Control        "},
      {" Беріктік       ",
       " Баскару        "}};
  int index_language = index_language_in_array(current_settings.language);

  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;

  index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  //Копіюємо  рядки у робочий екран
  for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
  {
    if (index_of_ekran < MAX_ROW_FOR_CHOSE_SETTINGS_PROTECTION_WITH_TIMEOUT_CONTROL)
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
//Формуємо екран відображення заголовків настроювання вимикача
/*****************************************************/
void make_ekran_chose_settings_switcher(void)
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_CHOSE_SETTINGS_SWITCHER][MAX_COL_LCD] =
    {
      {" Уставки        ",
       " Выдержки       ",
       " Управление     ",
       " Ранжирование   "},
      {" Уставки        ",
       " Витримки       ",
       " Управління     ",
       " Ранжування     "},
      {" Start Values   ",
       " Operate delays ",
       " Control        ",
       " Ranging        "},
      {" Баскару        ",
       " Кyрулар        ",
       " Беріктік       ",
       " Ранжирование   "}};
  int index_language = index_language_in_array(current_settings.language);

  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;

  index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  //Копіюємо  рядки у робочий екран
  for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
  {
    //Наступні рядки треба перевірити, чи їх требе відображати у текучій кофігурації
    if (index_of_ekran < MAX_ROW_FOR_CHOSE_SETTINGS_SWITCHER)
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
