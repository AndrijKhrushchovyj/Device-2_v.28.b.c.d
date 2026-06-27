#include "header.h"

/*****************************************************/
//Формуємо екран конфігурації
/*****************************************************/
void make_ekran_configuration(unsigned int configuration_edit_temp)
{
#define MAX_COL_LCD_PART1 10

  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_EKRAN_CONFIGURATION][MAX_COL_LCD_PART1] =
    {
      {"РПН       ",
       "ЗСХ       ",
       "ЗНХ       ",
       "БРП       ",
       "ЗНмакс    ",
       "ЗНмин     ",
       "УЗ        ",
       "Расш.лог. "},
      {"РПН       ",
       "ЗСХ       ",
       "ЗБХ       ",
       "БРП       ",
       "ЗНмакс    ",
       "ЗНмін     ",
       "УЗ        ",
       "Розш.лог. "},
      {"RPN       ",
       "ZSCH      ",
       "ZNCH      ",
       "BRP       ",
       "Umax      ",
       "Umin      ",
       "FL        ",
       "UD-logic  "},
      {"РПН       ",
       "ЗСХ       ",
       "ЗНХ       ",
       "БРП       ",
       "ЗНмакс    ",
       "ЗНмин     ",
       "УЗ        ",
       "Расш.лог. "}};

  static const unsigned char information_on[MAX_NAMBER_LANGUAGE][MAX_COL_LCD - MAX_COL_LCD_PART1] =
    {
      "Вкл.  ",
      "Увімк.",
      "On    ",
      "Косу. "};
  static const unsigned char information_off[MAX_NAMBER_LANGUAGE][MAX_COL_LCD - MAX_COL_LCD_PART1] =
    {
      "Откл. ",
      "Вимк. ",
      "Off   ",
      "Сљнд. "};

  unsigned char name_string_tmp[MAX_ROW_FOR_EKRAN_CONFIGURATION][MAX_COL_LCD];

  int index_language = index_language_in_array(current_settings.language);
  for (int index_1 = 0; index_1 < MAX_ROW_FOR_EKRAN_CONFIGURATION; index_1++)
  {
    for (int index_2 = 0; index_2 < MAX_COL_LCD_PART1; index_2++)
      name_string_tmp[index_1][index_2] = name_string[index_language][index_1][index_2];
  }

  /*******************************************************/
  //Формуємо значення
  /*******************************************************/
  for (unsigned int i = 0; i < MAX_ROW_FOR_EKRAN_CONFIGURATION; i++)
  {
    unsigned int configuration_temp;

    //Якщо ми не в режимі редагування, то беремо дані із стуктури текучих настройок
    //інакше берем із структури для редагування
    if (current_ekran.edition == 0)
      configuration_temp = current_settings.configuration;
    else
      configuration_temp = configuration_edit_temp;

    if ((configuration_temp & (1 << i)) == 0)
    {
      for (unsigned int j = MAX_COL_LCD_PART1; j < MAX_COL_LCD; j++)
        name_string_tmp[i][j] = information_off[index_language][j - MAX_COL_LCD_PART1];
    }
    else
    {
      for (unsigned int j = MAX_COL_LCD_PART1; j < MAX_COL_LCD; j++)
        name_string_tmp[i][j] = information_on[index_language][j - MAX_COL_LCD_PART1];
    }
  }
  /*******************************************************/

  int position_temp = current_ekran.index_position;
  int index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  //Копіюємо  рядки у робочий екран
  for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
  {
    //Наступні рядки треба перевірити, чи їх требе відображати у текучій коффігурації
    if (index_of_ekran < MAX_ROW_FOR_EKRAN_CONFIGURATION)
      for (unsigned int j = 0; j < MAX_COL_LCD; j++)
        working_ekran[i][j] = name_string_tmp[index_of_ekran][j];
    else
      for (unsigned int j = 0; j < MAX_COL_LCD; j++)
        working_ekran[i][j] = ' ';

    index_of_ekran++;
  }

  //Курсор по горизонталі відображається на першій позиції
  current_ekran.position_cursor_x = (MAX_COL_LCD_PART1 - 1);
  //Відображення курору по вертикалі
  current_ekran.position_cursor_y = position_temp & (MAX_ROW_LCD - 1);
  //Курсор видимий
  current_ekran.cursor_on = 1;
  //Курсор не мигає
  if (current_ekran.edition == 0)
    current_ekran.cursor_blinking_on = 0;
  else
    current_ekran.cursor_blinking_on = 1;

  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;

#undef MAX_COL_LCD_PART1
}
/*****************************************************/

/*****************************************************/
//
/*****************************************************/
/*****************************************************/
