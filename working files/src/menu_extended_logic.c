#include "header.h"

/*****************************************************/
//Формуємо екран відображення заголовків розширеної логіки
/*****************************************************/
void make_ekran_extended_logic(void)
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_EXTENDED_LIGIC][MAX_COL_LCD] = 
  {
    {
      " Общие уставки  ",
      " О-функции      ",
      " О-триггеры     ",
      " О-И            ",
      " О-ИЛИ          ",
      " О-Искл.ИЛИ     ",
      " О-НЕ           ",
      " Перед.функции  "
    },
    {
      " Заг.уставки    ",
      " В-функції      ",
      " В-триґери      ",
      " В-І            ",
      " В-АБО          ",
      " В-Викл.АБО     ",
      " В-НЕ           ",
      " Перед.функції  "
    },
    {
      " Gen Set Points ",
      " UD-Functions   ",
      " UD-FF          ",
      " AND            ",
      " OR             ",
      " XOR            ",
      " NOT            ",
      " Transfer func  "
    },
    {
      " Общие уставки  ",
      " О-функции      ",
      " О-триггеры     ",
      " О-И            ",
      " О-ИЛИ          ",
      " О-Искл.ИЛИ     ",
      " О-НЕ           ",
      " Перед.функции  "
    }
  };

  unsigned char name_string_tmp[MAX_ROW_FOR_EXTENDED_LIGIC][MAX_COL_LCD];
  
  int index_language = index_language_in_array(current_settings.language);
  for(int index_1 = 0; index_1 < MAX_ROW_FOR_EXTENDED_LIGIC; index_1++)
  {
    for(int index_2 = 0; index_2 < MAX_COL_LCD; index_2++)
    {
      name_string_tmp[index_1][index_2] = name_string[index_language][index_1][index_2];
    }
  }
  
//  unsigned int additional_current = 0;
  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;

//  /******************************************/
//  //Виключаємо поля, які не треба відображати
//  /******************************************/
//  unsigned int number_defined_logic[NUMBER_DEFINED_ELEMENTS] =
//  {
//    current_settings.number_defined_df,
//    current_settings.number_defined_dt,
//    current_settings.number_defined_and,
//    current_settings.number_defined_or,
//    current_settings.number_defined_xor,
//    current_settings.number_defined_not
//  };
//  
//  for (unsigned int index_defined_element = 0; index_defined_element < NUMBER_DEFINED_ELEMENTS; index_defined_element++)
//  {
//    if (number_defined_logic[index_defined_element] == 0)
//    {
//      unsigned int i = INDEX_OF_DEFINED_FUNCTIONS + index_defined_element - additional_current;
//
//      if ((i+1) <= position_temp) position_temp--;
//      do
//      {
//        for(unsigned int j = 0; j < MAX_COL_LCD; j++)
//        {
//          if ((i+1) < (MAX_ROW_FOR_EXTENDED_LIGIC - additional_current)) name_string_tmp[i][j] = name_string_tmp[i + 1][j];
//          else name_string_tmp[i][j] = ' ';
//        }
//        i++;
//      }
//      while (i < (MAX_ROW_FOR_EXTENDED_LIGIC - additional_current));
//      additional_current++;
//    }
//  }
//  /******************************************/
  
  index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  
  //Копіюємо  рядки у робочий екран
  for (unsigned int i = 0; i< MAX_ROW_LCD; i++)
  {
    if (index_of_ekran < (MAX_ROW_FOR_EXTENDED_LIGIC/* - additional_current*/))
    {
      for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string_tmp[index_of_ekran][j];
    } 
    else
      for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';

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
