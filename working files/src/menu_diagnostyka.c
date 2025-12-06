#include "header.h"

/*****************************************************/
//Формуємо екран відображення діагностики
/*****************************************************/
void make_ekran_diagnostyka(unsigned int volatile diagnostyka_tmp[])
{
#define NUMBER_ROW_FOR_NO_ERRORS 2
  
  unsigned int position_temp = current_ekran.index_position;
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][NUMBER_ROW_FOR_NO_ERRORS + MAX_ROW_FOR_DIAGNOSTYKA][MAX_COL_LCD] = 
  {
    {
      "      Нет       ",
      "     ошибок     ",
      NAME_DIAGN_RU
    },
    {
      "     Немає      ",
      "    помилок     ",
      NAME_DIAGN_UA
    },
    {
      "       No       ",
      "     errors     ",
      NAME_DIAGN_EN
    },
    {
      "      Кате      ",
      "      жок       ",
      NAME_DIAGN_KZ
    }
  };

  int index_language = index_language_in_array(current_settings.language);
  
  unsigned int not_null = false;
  for (size_t i = 0; i < N_DIAGN; i++) 
  {
    not_null |= (diagnostyka_tmp[i] != 0);
    if (not_null) break;
  }
              
  if (not_null)
  {
    /************************************************************/
    //Формуємо список із помилок які реально присутні
    /************************************************************/
    unsigned char name_string_tmp[MAX_ROW_FOR_DIAGNOSTYKA][MAX_COL_LCD];

    for(unsigned int index_1 = 0; index_1 < MAX_ROW_FOR_DIAGNOSTYKA; index_1++)
    {
      for(unsigned int index_2 = 0; index_2 < MAX_COL_LCD; index_2++)
        name_string_tmp[index_1][index_2] = name_string[index_language][NUMBER_ROW_FOR_NO_ERRORS + index_1][index_2];
      
      if ((index_1 >= ERROR_DIGITAL_OUTPUT_1_BIT) && (index_1 < (ERROR_DIGITAL_OUTPUT_1_BIT + NUMBER_SIMPLE_OUTPUTS)))
      {
        uint32_t value = 0;
        uint32_t index_board = 0, index_number = 0;
        for(size_t index_2 = 0; index_2 < MAX_COL_LCD; index_2++)
        {
          if (name_string_tmp[index_1][index_2] == '?')
          {
            if (value == 0)
            {
              //Це є умовою фіксації слова
              index_board = index_2;
              value = (index_1 - ERROR_DIGITAL_OUTPUT_1_BIT) + 1;
            }
            else
            {
              //Це є умовою фіксації номеру на слоті
              index_number = index_2;
              break;
            }
          }
        }
        
        int tmp_1 = -1, tmp_2 = -1;
        for (size_t i = 0; i < N_OUTPUT_BOARDS; i++)
        {
          if (value <= output_boards[i][0])
          {
            tmp_1 = output_boards[i][1];
            tmp_2 = (i == 0) ? value : value - output_boards[i - 1][0];
          
            break;
          }
        }
        
        name_string_tmp[index_1][index_board]  = tmp_1 + 0x40;
        name_string_tmp[index_1][index_number] = tmp_2 + 0x30;
      }
    }

    unsigned int index_of_ekran;
    unsigned int i = 0, offset = 0;

    while ((i + offset) < MAX_ROW_FOR_DIAGNOSTYKA)
    {
      if (_CHECK_SET_BIT(diagnostyka_tmp, (i + offset)) == 0)
      {
        for (unsigned int j = i; j < (MAX_ROW_FOR_DIAGNOSTYKA - offset); j++)
        {
          if ((j + 1) < (MAX_ROW_FOR_DIAGNOSTYKA - offset))
          {
            for (unsigned int k = 0; k<MAX_COL_LCD; k++)
              name_string_tmp[j][k] = name_string_tmp[j + 1][k];
          }
          else 
          {
            for (unsigned int k = 0; k<MAX_COL_LCD; k++)
              name_string_tmp[j][k] = ' ';
          }
        }
        if (current_ekran.index_position >= ((int)(i + offset))) position_temp--;
        offset++;
      }
      else i++;
    }
    /************************************************************/

    
    index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;
      
    //Копіюємо  рядки у робочий екран
    for (i=0; i< MAX_ROW_LCD; i++)
    {
      //Наступні рядки треба перевірити, чи їх требе відображати у текучій коффігурації
      if (index_of_ekran < MAX_ROW_FOR_DIAGNOSTYKA)
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string_tmp[index_of_ekran][j];
      else
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';

      index_of_ekran++;
    }

    //Курсор видимий
    current_ekran.cursor_on = 1;
  }
  else
  {
    //Це означає, що ніякої помилки не зафіксовано
     
    //Текучу позицію в сипску переводимо на сам початок
    current_ekran.index_position = 0;
    position_in_current_level_menu[EKRAN_DIAGNOSTYKA] = 0;

    //Копіюємо  рядки у робочий екран
    for (unsigned int i=0; i< MAX_ROW_LCD; i++)
    {
      //Копіюємо в робочий екран інформацію, що нічого не відранжовано
      if (i < 2)
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][i][j];
      else
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';
    }
    //Курсор не видимий
    current_ekran.cursor_on = 0;
  }

  //Курсор по горизонталі відображається на першій позиції
  current_ekran.position_cursor_x = 0;
  //Відображення курору по вертикалі
  current_ekran.position_cursor_y = position_temp & (MAX_ROW_LCD - 1);
  //Курсор не мигає
  current_ekran.cursor_blinking_on = 0;

  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
  
#undef NUMBER_ROW_FOR_NO_ERRORS
}
/*****************************************************/

/*****************************************************/
//
/*****************************************************/
/*****************************************************/
