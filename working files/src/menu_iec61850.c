#include "header.h"

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)

/*****************************************************/
//Формуємо екран відображення заголовків настроювання фізичного рівня RS-485
/*****************************************************/
void make_ekran_type_IEC61850_nodes(void)
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_TYPE_IEC61850_NODES][MAX_COL_LCD] = 
  {
    {
      " Вх.GOOSE блок  ",
      " Вх.MMS блок    ",
      " ВСБ            "
    },
    {
      " Вх.GOOSE блок  ",
      " Вх.MMS блок    ",
      " ВМБ            "
    },
    {
      " In.GOOSE block ",
      " In.MMS block   ",
      " Out.LAN block  "
    },
    {
      " Вх.GOOSE блок  ",
      " Вх.MMS блок    ",
      " ВСБ            "
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
    if (index_of_ekran < MAX_ROW_FOR_TYPE_IEC61850_NODES)
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
//Формуємо екран відображення входів/виходів блоків для IEC 61850
/*
 out_in:
 0 - виходи
 1 - входи

 total_number - повна кількість виходів/входів по даному елементу
*/
/*****************************************************/
void make_ekran_list_in_out_for_iec61850(unsigned int out_in, size_t total_number)
{
  static const uint8_t name_string_out[MAX_NAMBER_LANGUAGE][MAX_COL_LCD] = 
  {
      " Вых.           ",
      " Вих.           ",
      " Out.           ",
      " Вых.           "
  };
  static const unsigned int first_index_number_out[MAX_NAMBER_LANGUAGE] = {5, 5, 5, 5};

  static const uint8_t name_string_in[MAX_NAMBER_LANGUAGE][MAX_COL_LCD] = 
  {
      " Вх.            ",
      " Вх.            ",
      " In.            ",
      " Вх.            "
  };
  static const unsigned int first_index_number_in[MAX_NAMBER_LANGUAGE] = {4, 4, 4, 4};
  
  static const uint8_t (* const name_string[2])[MAX_COL_LCD] = {name_string_out, name_string_in};
  static const unsigned int (* const first_index_number[2])  = {first_index_number_out, first_index_number_in};
    
  int index_language = index_language_in_array(current_settings.language);
  unsigned int first_index_number_1 = first_index_number[out_in][index_language];
  
  unsigned int position_temp = current_ekran.index_position;
  size_t index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;
  
  //Копіюємо  рядки у робочий екран
  for (unsigned int i=0; i < MAX_ROW_LCD; i++)
  {
    unsigned int number = index_of_ekran + 1;
    
    //Наступні рядки треба перевірити, чи їх требе відображати у текучій кофігурації
    if (index_of_ekran < total_number)
    {
      for (unsigned int j = 0; j < MAX_COL_LCD; j++)
      {
        if (j != first_index_number_1)
           working_ekran[i][j] = name_string[out_in][index_language][j];
        else
        {
          if (j == (first_index_number_1 + 0))
          {
            if (number <= 0) working_ekran[i][j] = '?';
            else if (number > 0 ) 
            {
              working_ekran[i][j] = number + 0x30;
            }
          }
        }
      }
    }
    else
    {
      for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';
    }

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

#endif
