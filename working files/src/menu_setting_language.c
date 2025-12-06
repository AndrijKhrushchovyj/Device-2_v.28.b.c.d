#include "header.h"

/*****************************************************/
//Формуємо екран відображення мови меню
/*****************************************************/
void make_ekran_setting_language()
{
  static const unsigned char name_string[MAX_ROW_FOR_VIEW_SETTING_LANGUAGE][MAX_COL_LCD] = 
  {
    "    Language    "
  };
  
  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  
  //Множення на два величини position_temp потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
  index_of_ekran = ((position_temp<<1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    if (index_of_ekran < (MAX_ROW_FOR_VIEW_SETTING_LANGUAGE<<1))//Множення на два константи MAX_ROW_FOR_VIEW_SETTING_LANGUAGE потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_of_ekran>>1][j];
      }
      else
      {
        //У парному номері рядку виводимо значення уставки
        unsigned char information[MAX_COL_LCD] = "                ";
        unsigned int temp_data;
          
        if(current_ekran.edition == 0) 
        {
          temp_data = current_settings.language;
          index_language_in_array(current_settings.language);
        }
        else
        {
          temp_data = edition_settings.language;
          index_language_in_array(edition_settings.language);
        }
          
        if (temp_data == LANGUAGE_RU)
        {
          information[ 4] = 'Р';
          information[ 5] = 'у';
          information[ 6] = 'с';
          information[ 7] = 'с';
          information[ 8] = 'к';
          information[ 9] = 'и';
          information[10] = 'й';
          current_ekran.position_cursor_x = 3;
        }
        else if (temp_data == LANGUAGE_UA)
        {
          information[ 3] = 'У';
          information[ 4] = 'к';
          information[ 5] = 'р';
          information[ 6] = 'а';
          information[ 7] = 'ї';
          information[ 8] = 'н';
          information[ 9] = 'с';
          information[10] = 'ь';
          information[11] = 'к';
          information[12] = 'а';
          current_ekran.position_cursor_x = 2;
        }
        else if (temp_data == LANGUAGE_EN)
        {
          information[ 4] = 'E';
          information[ 5] = 'n';
          information[ 6] = 'g';
          information[ 7] = 'l';
          information[ 8] = 'i';
          information[ 9] = 's';
          information[10] = 'h';
          current_ekran.position_cursor_x = 3;
        }
        else if (temp_data == LANGUAGE_KZ)
        {
          information[5 ] = 'К';
          information[6 ] = 'а';
          information[7 ] = 'з';
          information[8 ] = 'а';
          information[9 ] = 'к';
          current_ekran.position_cursor_x = 4;
        }
        else
        {
          information[5 ] = 'E';
          information[6 ] = 'r';
          information[7 ] = 'r';
          information[8 ] = 'o';
          information[9 ] = 'r';
          current_ekran.position_cursor_x = 4;
        }
        
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = information[j];
      }
    }
    else
      for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';

    index_of_ekran++;
  }

  //Відображення курору по вертикалі і курсор завжди має бути у полі із значенням устаки
  current_ekran.position_cursor_y = ((position_temp<<1) + 1) & (MAX_ROW_LCD - 1);

  //Курсор видимий, якщо ми у режимі редагування
  if (current_ekran.edition == 0) current_ekran.cursor_on = 0;
  else current_ekran.cursor_on = 1;

  //Курсор не мигає, якщо ми у режимі редагування
  if(current_ekran.edition == 0)current_ekran.cursor_blinking_on = 0;
  else current_ekran.cursor_blinking_on = 1;

  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/


/*****************************************************/
//
/*****************************************************/
/*****************************************************/
