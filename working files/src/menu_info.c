#include "header.h"

/*****************************************************/
//Формуємо екран відображення інформації по версії програмного забезпечення і версії карти пам'яті
/*****************************************************/
void make_ekran_info()
{
  static unsigned char const name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_INFO][MAX_COL_LCD] = 
  {
    {
      "   Версия ПО    ",
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
      " Версия ПО(КП)  ",
#endif
      "   Версия КП    ",
      " Серийный номер ",
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
      "   MAC-адрес    ",
#endif
      "Режим перепрогр."
    },
    {
      "   Версія ПЗ    ",
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
      " Версія ПЗ(КП)  ",
#endif
      "   Версія КП    ",
      " Серійний номер ",
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
      "   MAC-адреса   ",
#endif
      "Режим перепрогр."
    },
    {
      "  Firmware ver  ",
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
      "Firmware ver(CU)",
#endif
      "ModBus M Map ver",
      " Serial number  ",
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
      "  MAC address   ",
#endif
      " Reprogram Mode "
    },
    {
      "   Версия ПО    ",
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
      " Версия ПО(КП)  ",
#endif
      "   Версия КП    ",
      " Серийный номер ",
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
      "   MAC-адрес    ",
#endif
      "Режим перепрогр."
    }
  };
  int index_language = index_language_in_array(current_settings.language);
  
  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  unsigned char value_str[MAX_COL_LCD];
  
  //Множення на два величини position_temp потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
  index_of_ekran = ((position_temp<<1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;
  int first_char_row1 = 0, last_chat_row1 = MAX_COL_LCD - 1;
  
  for (unsigned int i = 0; i< MAX_ROW_LCD; i++)
  {
    unsigned int index_of_ekran_tmp = index_of_ekran >> 1;
    if (index_of_ekran_tmp < MAX_ROW_FOR_INFO)
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) 
        {
          working_ekran[i][j] = name_string[index_language][index_of_ekran_tmp][j];
          value_str[j] = ' ';
        }
        
        if (
            ((index_of_ekran_tmp) == INDEX_ML_INFO_FIRMWARE)
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
            || 
            ((index_of_ekran_tmp) == INDEX_ML_INFO_FIRMWARE_CB)
#endif
           )   
        {
          uint8_t const version[][4] = 
          {
            {VERSIA_PZ, MODYFIKACIA_VERSII_PZ, ZBIRKA_VERSII_PZ, ZBIRKA_PIDVERSII_PZ}
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
            ,
            {fwKP[0], fwKP[1], fwKP[2], fwKP[3]}
#endif
          };
          unsigned int index_tmp = 0;
          unsigned int const row = index_of_ekran_tmp - INDEX_ML_INFO_FIRMWARE;
          
          for (size_t j = 0; j < 4; ++j)
          {
            uint8_t const val = version[row][j];
            if (val > 9)
            {
              value_str[index_tmp++] = (val / 10) + 0x30;
              value_str[index_tmp++] = (val % 10) + 0x30;
            }
            else value_str[index_tmp++] = val + 0x30;

            if ((j + 1) != 4) value_str[index_tmp++] = '.';
          }

          unsigned int shift = (MAX_COL_LCD - index_tmp) >> 1;
          first_char_row1 = shift;
          last_chat_row1 = first_char_row1 + index_tmp;
          index_tmp--;
          for (int k = index_tmp; k >= 0; k-- ) 
          {
            value_str[k + shift] = value_str[k];
            value_str[k] = ' ';
          }
          
        }
        else if ((index_of_ekran_tmp) == INDEX_ML_INFO_MEMORY_MAP)
        {
#if VERSIA_GMM > 9
          value_str[COL_INFO_BEGIN_2 + 0] = (VERSIA_GMM / 10) + 0x30;
          value_str[COL_INFO_BEGIN_2 + 1] = (VERSIA_GMM % 10) + 0x30;
#else
          value_str[COL_INFO_BEGIN_2 + 0] = ' ';
          value_str[COL_INFO_BEGIN_2 + 1] = VERSIA_GMM + 0x30;
#endif

          value_str[COL_INFO_BEGIN_2 + 2] = '.';
          
#if MODYFIKACIA_VERSII_GMM > 9
          value_str[COL_INFO_BEGIN_2 + 3] = (MODYFIKACIA_VERSII_GMM / 10) + 0x30;
          value_str[COL_INFO_BEGIN_2 + 4] = (MODYFIKACIA_VERSII_GMM % 10) + 0x30;
#else
          value_str[COL_INFO_BEGIN_2 + 3] = MODYFIKACIA_VERSII_GMM + 0x30;
          value_str[COL_INFO_BEGIN_2 + 4] = ' ';
#endif
        }
        else if ((index_of_ekran_tmp) == INDEX_ML_INFO_SERIAL_NUMBER)
        {
          uint32_t value = serial_number_dev;
          uint32_t value_tmp = value;
          uint32_t number_digits = 0;
          do
          {
            number_digits++;
            value_tmp /= 10;
          }
          while(value_tmp != 0);
          
          first_char_row1 = (MAX_COL_LCD - number_digits) >> 1;
          last_chat_row1 = first_char_row1 + number_digits;
          uint32_t index = last_chat_row1 - 1;
          for (uint32_t j = 0; j < number_digits; j++)
          {
            value_tmp = value % 10;
            value_str[index--] = value_tmp + 0x30;
            value /= 10;
          }
        }
        else if ((index_of_ekran>>1) == INDEX_ML_REPROGRAM)
        {
          for (size_t j = 0; j < MAX_COL_LCD; ++j) value_str[j] = ' ';
        }
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
        else if ((index_of_ekran_tmp) == INDEX_ML_INFO_MAC_ADDRESS)
        {
          uint16_t MAC_address_tmp[4] = {0x0080, 0xE1, VERSIA_PZ, serial_number_dev};
          size_t const n_digit_arr[4] ={4, 2, 2, 4};
          size_t const b_index_arr[4] ={4, 7, 9, 14};
          static const uint8_t string[] = "0123456789ABCDEF";
          
          value_str[5] = value_str[10] = ':';
          for (size_t k1 = 0; k1 < 4; ++k1)
          {
            uint32_t index = b_index_arr[k1];
            size_t const n_digit = n_digit_arr[k1];
            for (size_t k2 = 0; k2 < n_digit; ++k2)
            {
              value_str[index--] = string[MAC_address_tmp[k1] & 0xf];
              MAC_address_tmp[k1] >>= 4;
            }
          }
        }
#endif        
      }
      else
      {
        //У парному номері рядку виводимо значення
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = value_str[j];
      }
    }
    else
      for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';

    index_of_ekran++;
  }

  //Відображення курору по вертикалі і курсор завжди має бути у полі із значенням (крім як у останньому радку, де є вибір переходу у режим перепрограмування)
  current_ekran.position_cursor_y = ((position_temp<<1) + ((position_temp != INDEX_ML_REPROGRAM) ? 1 : 0)) & (MAX_ROW_LCD - 1);
  int last_position_cursor_x = MAX_COL_LCD;

  //Курсор по горизонталі відображається на першому символі у випадку, коли ми не в режимі редагування, інакше позиція буде визначена у функцї main_manu_function
  if (
      (current_ekran.index_position == INDEX_ML_INFO_FIRMWARE) ||
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
      (current_ekran.index_position == INDEX_ML_INFO_FIRMWARE_CB) ||
#endif
      (current_ekran.index_position == INDEX_ML_INFO_SERIAL_NUMBER)
     )  
  {
    current_ekran.position_cursor_x = first_char_row1;
    last_position_cursor_x = last_chat_row1;
  }
  else if (current_ekran.index_position == INDEX_ML_INFO_MEMORY_MAP)
  {
    current_ekran.position_cursor_x = COL_INFO_BEGIN_2;
    last_position_cursor_x = COL_END_END_2;
  }
  else 
  {
    current_ekran.position_cursor_x = 0;
    last_position_cursor_x = MAX_COL_LCD;
  }
  
  if (current_ekran.index_position != INDEX_ML_REPROGRAM)
  {
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
//Формуємо екран відображення міти дати і часу прошивки
/*****************************************************/
void make_ekran_date_time_pz(void)
{
  unsigned char name_string[2*MAX_ROW_FOR_DATE_TIME_PZ][MAX_COL_LCD] = 
  {
    "   XX-XX-20XX   ",
    "     XX:XX      "
  };
  
  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  
  index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;
  
  uint8_t const dateTime[][6] = 
  {
    {YEAR_VER, MONTH_VER, DAY_VER, HOUR_VER, MINUTE_VER, SECOND_VER}
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
    ,
    {fwDTKP[0], fwDTKP[1], fwDTKP[2], fwDTKP[3], fwDTKP[4], fwDTKP[5]}
#endif
  };
  unsigned int const row = position_in_current_level_menu[previous_level_in_current_level_menu[EKRAN_DATE_TIME_PZ]] - INDEX_ML_INFO_FIRMWARE;
  

  /******************************************/
  //Заповнюємо поля відповідними цифрами
  /******************************************/
  //День
  name_string[0][3 ] = (dateTime[row][2] >>  4) + 0x30;
  name_string[0][4 ] = (dateTime[row][2] & 0xf) + 0x30;

  //Місяць
  name_string[0][6 ] = (dateTime[row][1] >>  4) + 0x30;
  name_string[0][7 ] = (dateTime[row][1] & 0xf) + 0x30;

  //Рік
  name_string[0][11] = (dateTime[row][0] >>  4) + 0x30;
  name_string[0][12] = (dateTime[row][0] & 0xf) + 0x30;

  //Година
  name_string[1][5 ] = (dateTime[row][3] >>  4) + 0x30;
  name_string[1][6 ] = (dateTime[row][3] & 0xf) + 0x30;

  //Хвилини
  name_string[1][8 ] = (dateTime[row][4] >>  4) + 0x30;
  name_string[1][9 ] = (dateTime[row][4] & 0xf) + 0x30;
  /******************************************/
  
  //Копіюємо  рядки у робочий екран
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    //Наступні рядки треба перевірити, чи їх требе відображати
    if (index_of_ekran < (2*MAX_ROW_FOR_DATE_TIME_PZ))
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
