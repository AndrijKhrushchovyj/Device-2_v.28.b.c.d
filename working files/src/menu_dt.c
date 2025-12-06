#include "header.h"

/*****************************************************/
//Формуємо екран відображення заголовків встановллення-скидання орпеділюваних триґерів
/*****************************************************/
void make_ekran_set_reset_trigger(void)
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_LIST_SET_RESET_DT][MAX_COL_LCD] = 
  {
    {
      " Установка...   ",
      " Сброс...       "
    },
    {
      " Встановлення...",
      " Скидання ...   "
    },
    {
      " Setting...     ",
      " Resetting...   "
    },
    {
      " Установка...   ",
      " Сброс...       "
    }
  };
  int index_language = index_language_in_array(current_settings.language);
  
  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  
  
  index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  
  //Копіюємо  рядки у робочий екран
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    //Наступні рядки треба перевірити, чи їх требе відображати у текучій коффігурації
    if (index_of_ekran < MAX_ROW_FOR_LIST_SET_RESET_DT)
    {
      for (unsigned int j = 0; j<MAX_COL_LCD; j++) 
      {
        working_ekran[i][j] = name_string[index_language][index_of_ekran][j];
      }
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
//Формуємо екран відображення типів джерел ранжування орпеділюваних триґерів
/*****************************************************/
void make_ekran_list_type_source_dt(void)
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_LIST_TYPE_SOURCE_DT][MAX_COL_LCD] = 
  {
    {
      " Источники прям.",
      " Источники обр. "
    },
    {
      " Джерела прям.  ",
      " Джерела звор.  "
    },
    {
      " Direct Inputs  ",
      " Inverse Inputs "
    },
    {
      " Источники прям.",
      " Источники обр. "
    }
  };
  int index_language = index_language_in_array(current_settings.language);
  
  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  
  
  index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  
  //Копіюємо  рядки у робочий екран
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    //Наступні рядки треба перевірити, чи їх требе відображати у текучій коффігурації
    if (index_of_ekran < MAX_ROW_LIST_TYPE_SOURCE_DT)
      for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][index_of_ekran][j];
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
