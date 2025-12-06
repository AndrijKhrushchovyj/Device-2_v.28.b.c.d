#include "header.h"

/*****************************************************/
//Формуємо екран відображення уставок для ВМП
/*****************************************************/
void make_ekran_setpoint_VMP(int forward_backward)
{
  unsigned char name_string[MAX_NAMBER_LANGUAGE][1 + 2][MAX_COL_LCD] = 
  {
    {
      "  Кол.участков  ",
      "   Длина уч.X   ",
      " У.сопрот.уч.X  "
    },
    {
      " Кільк.ділянок  ",
      "  Довжина д.X   ",
      "   П.опір д.X   "
    },
    {
      " Sections Number",
      "  Sec LengthX   ",
      "Sec ResistivityX"
    },
    {
      "  Кол.участков  ",
      "   Длина уч.X   ",
      " У.сопрот.уч.X  "
    }
  };

  int index_language = index_language_in_array(current_settings.language);
  
  unsigned int max_rows;
  if (current_ekran.edition == 0) max_rows = 1 + 2*current_settings.lines[forward_backward];
  else max_rows = 1 + 2*edition_settings.lines[forward_backward];
  
  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  unsigned int vaga, value, first_symbol;
  
  //Множення на два величини position_temp потрібне для того, бо наодн позицію ми використовуємо два рядки (назва + значення)
  index_of_ekran = ((position_temp<<1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;
  
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    unsigned int index_of_ekran_tmp = index_of_ekran >> 1;
    unsigned int view = ((current_ekran.edition == 0) || (position_temp != index_of_ekran_tmp));
    if (index_of_ekran_tmp < max_rows)
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        
        unsigned int row_to_show;

        if (index_of_ekran_tmp == INDEX_ML_LINES)
        {
          row_to_show = INDEX_ML_LINES;
          
          vaga = 1; //максимальний ваговий коефіцієнт для вилілення старшого розряду
          if (view == true) value = current_settings.lines[forward_backward]; //у змінну value поміщаємо значення уставки
          else value = edition_settings.lines[forward_backward];
        }
        else
        {
          int _tmp = index_of_ekran_tmp - 1;
          int dilanka = _tmp >> 1;

          row_to_show = 1 + (_tmp & 0x1);
          if (row_to_show  == INDEX_ML_DOVGYNA) 
          {

            static const unsigned int position_dovgyna_row[MAX_NAMBER_LANGUAGE] = {12, 12, 12, 12};
            name_string[index_language][row_to_show][position_dovgyna_row[index_language]] = 0x30 + (dilanka + 1);
            
            vaga = 10000; //максимальний ваговий коефіцієнт для вилілення старшого розряду
            if (view == true) value = current_settings.dovgyna[forward_backward][dilanka]; //у змінну value поміщаємо значення уставки
            else value = edition_settings.dovgyna[forward_backward][dilanka];
          }
          else 
          {
            static const unsigned int position_opir_row[MAX_NAMBER_LANGUAGE] = {13, 12, 15, 13};
            name_string[index_language][row_to_show][position_opir_row[index_language]] = 0x30 + (dilanka + 1);
            
            vaga = 10000; //максимальний ваговий коефіцієнт для вилілення старшого розряду
            if (view == true) value = current_settings.opir[forward_backward][dilanka]; //у змінну value поміщаємо значення уставки
            else value = edition_settings.opir[forward_backward][dilanka];
          }
        }
        first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][row_to_show][j];
      }
      else
      {
        //У парному номері рядку виводимо значення уставки
        for (unsigned int j = 0; j<MAX_COL_LCD; j++)
        {
          if (index_of_ekran_tmp == INDEX_ML_LINES)
          {
            if ((j < COL_SETPOINT_LINES_BEGIN) ||  (j > COL_SETPOINT_LINES_END ))working_ekran[i][j] = ' ';
            else
              calc_int_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, view);
          }
          else
          {
            int _tmp = index_of_ekran_tmp - 1;
            if ((1 + (_tmp & 0x1))  == INDEX_ML_DOVGYNA) 
            {
              if (
                  ((j < COL_SETPOINT_DOVGYNA_BEGIN) ||  (j > COL_SETPOINT_DOVGYNA_END )) &&
                  (
                   !((j >= (COL_SETPOINT_DOVGYNA_END + 2)) && (j <= (COL_SETPOINT_DOVGYNA_END + 3)))
                  )
                 )working_ekran[i][j] = ' ';
              else if (j == COL_SETPOINT_DOVGYNA_COMMA )working_ekran[i][j] = ',';
              else if ((j >= (COL_SETPOINT_DOVGYNA_END + 2)) && (j <= (COL_SETPOINT_DOVGYNA_END + 3))) 
              {
                static const unsigned char km[MAX_NAMBER_LANGUAGE][2] = {"км", "км", "km", "км"};

                working_ekran[i][j] = km[index_language][j - (COL_SETPOINT_DOVGYNA_END + 2)]; 
              }
              else
                calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_DOVGYNA_COMMA, view, 0);
            }
            else
            {
              if (
                  ((j < COL_SETPOINT_OPIR_BEGIN) ||  (j > COL_SETPOINT_OPIR_END )) &&
                  (
                   !((j >= (COL_SETPOINT_OPIR_END + 2)) && (j <= (COL_SETPOINT_OPIR_END + 6)))
                  )
                 )working_ekran[i][j] = ' ';
              else if (j == COL_SETPOINT_OPIR_COMMA )working_ekran[i][j] = ',';
              else if ((j >= (COL_SETPOINT_OPIR_END + 2)) && (j <= (COL_SETPOINT_OPIR_END + 6))) 
              {
                static const unsigned char ohm[MAX_NAMBER_LANGUAGE][5] = {"Ом/км", "Ом/км", "Ї/km ", "Ом/км"};

                working_ekran[i][j] = ohm[index_language][j - (COL_SETPOINT_OPIR_END + 2)]; 
              }
              else
                calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_OPIR_COMMA, view, 0);
            }
          }
        }
      }
        
    }
    else
      for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';

    index_of_ekran++;
  }

  //Відображення курору по вертикалі і курсор завжди має бути у полі із значенням устаки
  current_ekran.position_cursor_y = ((position_temp<<1) + 1) & (MAX_ROW_LCD - 1);
  //Курсор по горизонталі відображається на першому символі у випадку, коли ми не в режимі редагування, інакше позиція буде визначена у функцї main_manu_function
  if (current_ekran.edition == 0)
  {
    int last_position_cursor_x = MAX_COL_LCD;
    
    if (current_ekran.index_position == INDEX_ML_LINES)
    {
      current_ekran.position_cursor_x = COL_SETPOINT_LINES_BEGIN;
      last_position_cursor_x =COL_SETPOINT_LINES_END;
    }
    else
    {
      int _tmp = current_ekran.index_position - 1;
      if ((1 + (_tmp & 0x1))  == INDEX_ML_DOVGYNA) 
      {
        current_ekran.position_cursor_x = COL_SETPOINT_DOVGYNA_BEGIN;
        last_position_cursor_x = COL_SETPOINT_DOVGYNA_END;
      }
      else
      {
        current_ekran.position_cursor_x = COL_SETPOINT_OPIR_BEGIN;
        last_position_cursor_x = COL_SETPOINT_OPIR_END;
      }
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
//Формуємо екран відображення значення управлінської інформації для ВМП
/*****************************************************/
void make_ekran_control_VMP()
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_CONTROL_VMP][MAX_COL_LCD] = 
  {
    {
      "      ОМП       "
    },
    {
      "      ВМП       "
    },
    {
      "       FL       "
    },
    {
      "      ОМП       "
    }
  };
  int index_language = index_language_in_array(current_settings.language);
  
  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  
  //Множення на два величини position_temp потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
  index_of_ekran = ((position_temp<<1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;
  
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    unsigned int index_of_ekran_tmp = index_of_ekran >> 1;
    if (index_of_ekran_tmp < MAX_ROW_FOR_CONTROL_VMP)
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][index_of_ekran_tmp][j];
      }
      else
      {
        //У парному номері рядку виводимо значення уставки
        unsigned int index_ctr = index_of_ekran_tmp;

        unsigned int temp_data;
          
        if(current_ekran.edition == 0) temp_data = current_settings.control_vmp;
        else temp_data = edition_settings.control_vmp;
          
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = information_off_on[index_language][(temp_data >> index_ctr) & 0x1][j];
        if (position_temp == index_of_ekran_tmp) current_ekran.position_cursor_x = cursor_x_off_on[index_language][(temp_data >> index_ctr) & 0x1];
      }
    }
    else
      for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';

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
