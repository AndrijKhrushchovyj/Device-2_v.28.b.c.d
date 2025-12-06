#include "header.h"

/*****************************************************/
//Формуємо екран відображення списку міток настроювання
/*****************************************************/
void make_ekran_chose_item_of_point_time_settings(void)
{
  static unsigned char const name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_POINT_TIME_SETTINGS][MAX_COL_LCD] = 
  {
    {
      " Метка настроек ",
      " Метка ранжиров."
    },
    {
      " Мітка налашт.  ",
      " Мітка ранжув.  "
    },
    {
      " Settings Mark  ",
      " Sign Conf Mark "
    },
    {
      " Метка настроек ",
      " Метка ранжиров."
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
    if (index_of_ekran < MAX_ROW_FOR_POINT_TIME_SETTINGS)
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
//Формуємо екран мітки часу останніх змін у настроюванні
/*
-------------------------------------------------------
Вхідний прараметр:
kind
      0 - мітка уставок-витримок-управлінської інформації
      1 - мітка ранжування
-------------------------------------------------------
*/
/*****************************************************/
void make_ekran_time_settings(unsigned int kind)
{
  unsigned char name_string[2*MAX_ROW_FOR_POINT_TIME_SETPOINT_RANGUVANNJA][MAX_COL_LCD] = 
  {
    "00-00-2000      ",
    "00:00:00 XXXXXXX"
  };
  
  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  time_t time_target = (kind == 0) ? current_settings.time_setpoints : current_settings.time_ranguvannja;
  unsigned char source_target = (kind == 0) ? current_settings.source_setpoints : current_settings.source_ranguvannja;
  
  index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  /******************************************/
  //Заповнюємо поля відповідними цифрами
  /******************************************/
   if (time_target != 0)
   {
      struct tm *p = localtime(&time_target);
      int field;
      
      //День
      field = p->tm_mday;
      name_string[0][0 ] = (field / 10) + 0x30;
      name_string[0][1 ] = (field % 10) + 0x30;

      //Місяць
      field = p->tm_mon + 1;
      name_string[0][3 ] = (field / 10) + 0x30;
      name_string[0][4 ] = (field % 10) + 0x30;

      //Рік
      field = p->tm_year - 100;
      name_string[0][8 ] = (field / 10) + 0x30;
      name_string[0][9 ] = (field % 10) + 0x30;

      //Година
      field = p->tm_hour;
      name_string[1][0 ] = (field / 10) + 0x30;
      name_string[1][1 ] = (field % 10) + 0x30;

      //Хвилини
      field = p->tm_min;
      name_string[1][3 ] = (field / 10) + 0x30;
      name_string[1][4 ] = (field % 10) + 0x30;

      //Секунди
      field = p->tm_sec;
      name_string[1][6 ] = (field / 10) + 0x30;
      name_string[1][7 ] = (field % 10) + 0x30;
   }

  //Повідомлення про джерело змін
  if(source_target == 0)
  {
    name_string[ROW_T_][MAX_COL_LCD - 7] = ' ';
    name_string[ROW_T_][MAX_COL_LCD - 6] = ' ';
    name_string[ROW_T_][MAX_COL_LCD - 5] = ' ';
    name_string[ROW_T_][MAX_COL_LCD - 4] = ' ';
    name_string[ROW_T_][MAX_COL_LCD - 3] = ' ';
    name_string[ROW_T_][MAX_COL_LCD - 2] = ' ';
    name_string[ROW_T_][MAX_COL_LCD - 1] = '0';
  }
  else if(source_target == 1)
  {
    name_string[ROW_T_][MAX_COL_LCD - 7] = ' ';
    name_string[ROW_T_][MAX_COL_LCD - 6] = ' ';
    name_string[ROW_T_][MAX_COL_LCD - 5] = ' ';
    name_string[ROW_T_][MAX_COL_LCD - 4] = ' ';
    name_string[ROW_T_][MAX_COL_LCD - 3] = ' ';
    name_string[ROW_T_][MAX_COL_LCD - 2] = ' ';
    name_string[ROW_T_][MAX_COL_LCD - 1] = 'K';
  }
  else if(source_target == 2)
  {
    name_string[ROW_T_][MAX_COL_LCD - 7] = ' ';
    name_string[ROW_T_][MAX_COL_LCD - 6] = ' ';
    name_string[ROW_T_][MAX_COL_LCD - 5] = ' ';
    name_string[ROW_T_][MAX_COL_LCD - 4] = ' ';
    name_string[ROW_T_][MAX_COL_LCD - 3] = 'U';
    name_string[ROW_T_][MAX_COL_LCD - 2] = 'S';
    name_string[ROW_T_][MAX_COL_LCD - 1] = 'B';
  }
  else if(source_target == 3)
  {
    name_string[ROW_T_][MAX_COL_LCD - 7] = ' ';
    name_string[ROW_T_][MAX_COL_LCD - 6] = 'R';
    name_string[ROW_T_][MAX_COL_LCD - 5] = 'S';
    name_string[ROW_T_][MAX_COL_LCD - 4] = '-';
    name_string[ROW_T_][MAX_COL_LCD - 3] = '4';
    name_string[ROW_T_][MAX_COL_LCD - 2] = '8';
    name_string[ROW_T_][MAX_COL_LCD - 1] = '5';
  }
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
  else if(source_target == 4)
  {
    name_string[ROW_T_][MAX_COL_LCD - 7] = ' ';
    name_string[ROW_T_][MAX_COL_LCD - 6] = ' ';
    name_string[ROW_T_][MAX_COL_LCD - 5] = ' ';
    name_string[ROW_T_][MAX_COL_LCD - 4] = ' ';
    name_string[ROW_T_][MAX_COL_LCD - 3] = 'L';
    name_string[ROW_T_][MAX_COL_LCD - 2] = 'A';
    name_string[ROW_T_][MAX_COL_LCD - 1] = 'N';
  }
#endif
  else
  {
    //теоретично ми сюди б не мали ніколи заходити, але якщо ми сюди зайшли, то виводимо сигналізацію про помилку на екран
    static unsigned char const error_meas[MAX_NAMBER_LANGUAGE][7] = 
    {
      " Ошибка",
      "Помилка",
      "  Error",
      " Ошибка"
    };
    int index_language = index_language_in_array(current_settings.language);
      
    for (unsigned int i = 0; i < 7; i++) name_string[ROW_T_][9 + i] = error_meas[index_language][i];
  }
  
  /******************************************/
  
  //Копіюємо  рядки у робочий екран
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    //Наступні рядки треба перевірити, чи їх требе відображати
    if (index_of_ekran < (2*MAX_ROW_FOR_POINT_TIME_SETPOINT_RANGUVANNJA))
      for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_of_ekran][j];
    else
      for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';

    index_of_ekran++;
  }

  //Курсор по горизонталі поміщаємо на першій позиції
  current_ekran.position_cursor_x = 0;
  //Відображення курору по вертикалі в 0
  current_ekran.position_cursor_y = 0;
  //Курсор не видимий
  current_ekran.cursor_on = 0;
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
