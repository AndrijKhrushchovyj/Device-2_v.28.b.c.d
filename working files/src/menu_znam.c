#include "header.h"

/*****************************************************/
//Формуємо екран відображення уставок ЗНам
/*****************************************************/
void make_ekran_setpoint_znam(unsigned int group)
{
 static unsigned char const name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_SETPOINT_ZNAM][MAX_COL_LCD] = 
  {
    {
      "      ЗНам      "
    },
    {
      "      ЗНам      "
    },
    {
      "      InrP      "
    },
    {
      "      ЗНам      "
    }
  };
  static unsigned char const symbols[MAX_NAMBER_LANGUAGE][3] = {"ед.", "од.", "u  ", "ед."};
  int index_language = index_language_in_array(current_settings.language);
  
  unsigned int position_temp = current_ekran.index_position;
  unsigned int vaga, value, first_symbol;
  
  //Множення на два величини position_temp потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
  unsigned int index_of_ekran = ((position_temp<<1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;
  
  for (size_t i = 0; i < MAX_ROW_LCD; ++i)
  {
    unsigned int index_of_ekran_tmp = index_of_ekran >> 1;
    unsigned int view = ((current_ekran.edition == 0) || (position_temp != index_of_ekran_tmp));
    //Наступні рядки треба перевірити, чи їх требе відображати у текучій коффігурації
    if (index_of_ekran_tmp < MAX_ROW_FOR_SETPOINT_ZNAM)
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (size_t j = 0; j < MAX_COL_LCD; ++j) working_ekran[i][j] = name_string[index_language][index_of_ekran_tmp][j];

        if (index_of_ekran_tmp == INDEX_ML_STP_ZNAM)
        {
          vaga = 1000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для уставки ЗНам
          if (view == true) value = current_settings.setpoint_znam[group]; //у змінну value поміщаємо значення уставки ЗНам
          else value = edition_settings.setpoint_znam[group];
        }
        first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
      }
      else
      {
        //У парному номері рядку виводимо значення уставки
        for (size_t j = 0; j < MAX_COL_LCD; ++j)
        {
          if (index_of_ekran_tmp == INDEX_ML_STP_ZNAM)
          {
            if (
                ((j < COL_SETPOINT_ZNAM_BEGIN) ||  (j > COL_SETPOINT_ZNAM_END )) &&
                (
                 !((j >= (COL_SETPOINT_ZNAM_END + 2)) && (j <= (COL_SETPOINT_ZNAM_END + 4)))
                )    
               )working_ekran[i][j] = ' ';
            else if (j == COL_SETPOINT_ZNAM_COMMA )working_ekran[i][j] = ',';
            else if ((j >= (COL_SETPOINT_ZNAM_END + 2)) && (j <= (COL_SETPOINT_ZNAM_END + 4)))
              working_ekran[i][j] = symbols[index_language][j - (COL_SETPOINT_ZNAM_END + 2)];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_ZNAM_COMMA, view, 0);
          }
        }
      }
    }
    else
      for (size_t j = 0; j<MAX_COL_LCD; ++j) working_ekran[i][j] = ' ';

    index_of_ekran++;
  }

  //Відображення курору по вертикалі і курсор завжди має бути у полі із значенням устаки
  current_ekran.position_cursor_y = ((position_temp<<1) + 1) & (MAX_ROW_LCD - 1);
  //Курсор по горизонталі відображається на першому символі у випадку, коли ми не в режимі редагування, інакше позиція буде визначена у функцї main_manu_function
  if (current_ekran.edition == 0)
  {
    int last_position_cursor_x = MAX_COL_LCD;
    if (current_ekran.index_position == INDEX_ML_STP_ZNAM)
    {
      current_ekran.position_cursor_x = COL_SETPOINT_ZNAM_BEGIN;
      last_position_cursor_x = COL_SETPOINT_ZNAM_END;
    }

    //Підтягуємо курсор до першого символу
    while (((working_ekran[current_ekran.position_cursor_y][current_ekran.position_cursor_x + 1]) == ' ') && 
           (current_ekran.position_cursor_x < (last_position_cursor_x -1))) current_ekran.position_cursor_x++;
    
    //Курсор ставимо так, щоб він був перед числом
    if (((working_ekran[current_ekran.position_cursor_y][current_ekran.position_cursor_x]) != ' ') && 
        (current_ekran.position_cursor_x > 0)) current_ekran.position_cursor_x--;
  }
  //Курсор видимий
  current_ekran.cursor_on = 1;
  //Курсор не мигає
  if(current_ekran.edition == 0)current_ekran.cursor_blinking_on = 0;
  else current_ekran.cursor_blinking_on = 1;
  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//Формуємо екран відображення витримок ЗНам
/*****************************************************/
void make_ekran_timeout_znam(unsigned int group)
{
  static unsigned char const name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_TIMEOUT_ZNAM][MAX_COL_LCD] = 
  {
    {
      "      ЗНам      "
    },
    {
      "      ЗНам      "
    },
    {
      "      InrP      "
    },
    {
      "      ЗНам      "
    }
  };
  int index_language = index_language_in_array(current_settings.language);
  
  unsigned int position_temp = current_ekran.index_position;
  unsigned int vaga, value, first_symbol;
  
  //Множення на два величини position_temp потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
  unsigned int index_of_ekran = ((position_temp<<1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  
  for (size_t i = 0; i< MAX_ROW_LCD; ++i)
  {
    unsigned int index_of_ekran_tmp = index_of_ekran >> 1;
    unsigned int view = ((current_ekran.edition == 0) || (position_temp != index_of_ekran_tmp));
    if (index_of_ekran_tmp < MAX_ROW_FOR_TIMEOUT_ZNAM)
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (size_t j = 0; j < MAX_COL_LCD; ++j) working_ekran[i][j] = name_string[index_language][index_of_ekran_tmp][j];

        if (index_of_ekran_tmp == INDEX_ML_TMO_ZNAM)
        {
          vaga = 1000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки ЗНам
          if (view == true) value = current_settings.timeout_znam[group]; //у змінну value поміщаємо значення витримки ЗНам
          else value = edition_settings.timeout_znam[group];
        }

        first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
      }
      else
      {
        //У парному номері рядку виводимо значення уставки
        for (unsigned int j = 0; j<MAX_COL_LCD; j++)
        {
          if (index_of_ekran_tmp == INDEX_ML_TMO_ZNAM)
          {
            if (
                ((j < COL_TMO_ZNAM_BEGIN) ||  (j > COL_TMO_ZNAM_END )) &&
                (j != (COL_TMO_ZNAM_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_ZNAM_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_ZNAM_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_ZNAM_COMMA, view, 0);
          }
        }
      }
        
    }
    else
      for (size_t j = 0; j < MAX_COL_LCD; ++j) working_ekran[i][j] = ' ';

    index_of_ekran++;
  }

  //Відображення курору по вертикалі і курсор завжди має бути у полі із значенням устаки
  current_ekran.position_cursor_y = ((position_temp<<1) + 1) & (MAX_ROW_LCD - 1);
  //Курсор по горизонталі відображається на першому символі у випадку, коли ми не в режимі редагування, інакше позиція буде визначена у функцї main_manu_function
  if (current_ekran.edition == 0)
  {
    int last_position_cursor_x = MAX_COL_LCD;
    if (current_ekran.index_position == INDEX_ML_TMO_ZNAM) 
    {
      current_ekran.position_cursor_x = COL_TMO_ZNAM_BEGIN;
      last_position_cursor_x = COL_TMO_ZNAM_END;
    }

    //Підтягуємо курсор до першого символу
    while (((working_ekran[current_ekran.position_cursor_y][current_ekran.position_cursor_x + 1]) == ' ') && 
           (current_ekran.position_cursor_x < (last_position_cursor_x -1))) current_ekran.position_cursor_x++;

    //Курсор ставимо так, щоб він був перед числом
    if (((working_ekran[current_ekran.position_cursor_y][current_ekran.position_cursor_x]) != ' ') && 
        (current_ekran.position_cursor_x > 0)) current_ekran.position_cursor_x--;
  }
  //Курсор видимий
  current_ekran.cursor_on = 1;
  //Курсор не мигає
  if(current_ekran.edition == 0)current_ekran.cursor_blinking_on = 0;
  else current_ekran.cursor_blinking_on = 1;
  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//Формуємо екран відображення значення управлінської інформації для ЗНам
/*****************************************************/
void make_ekran_control_znam()
{
  static unsigned char const name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_CONTROL_ZNAM][MAX_COL_LCD] = 
  {
    {
      "      ЗНам      "
    },
    {
      "      ЗНам      "
    },
    {
      "      InrP      "
    },
    {
      "      ЗНам      "
    }
  };
  int index_language = index_language_in_array(current_settings.language);
  
  unsigned int position_temp = current_ekran.index_position;
  
  //Множення на два величини position_temp потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
  unsigned int index_of_ekran = ((position_temp<<1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  
  for (size_t i = 0; i < MAX_ROW_LCD; ++i)
  {
    unsigned int index_of_ekran_tmp = index_of_ekran >> 1;
    if (index_of_ekran_tmp < MAX_ROW_FOR_CONTROL_ZNAM)
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (size_t j = 0; j < MAX_COL_LCD; ++j) working_ekran[i][j] = name_string[index_language][index_of_ekran_tmp][j];
      }
      else
      {
        //У парному номері рядку виводимо значення уставки
        unsigned int index_ctr = index_of_ekran_tmp;

        unsigned int temp_data;
          
        if(current_ekran.edition == 0) temp_data = current_settings.control_znam;
        else temp_data = edition_settings.control_znam;
          
        for (size_t j = 0; j < MAX_COL_LCD; ++j) working_ekran[i][j] = information_off_on[index_language][(temp_data >> index_ctr) & 0x1][j];
        if (position_temp == index_of_ekran_tmp) current_ekran.position_cursor_x = cursor_x_off_on[index_language][(temp_data >> index_ctr) & 0x1];
      }
    }
    else
      for (size_t j = 0; j < MAX_COL_LCD; ++j) working_ekran[i][j] = ' ';

    index_of_ekran++;
  }

  //Відображення курору по вертикалі і курсор завжди має бути у полі із значенням устаки
  current_ekran.position_cursor_y = ((position_temp<<1) + 1) & (MAX_ROW_LCD - 1);
  //Курсор видимий
  current_ekran.cursor_on = 1;
  //Курсор не мигає
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
