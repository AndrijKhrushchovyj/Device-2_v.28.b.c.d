#include "header.h"

/*****************************************************/
//Формуємо екран година-час
/*****************************************************/
void make_ekran_time(void)
{
  unsigned char name_string[MAX_ROW_FOR_EKRAN_TIME][MAX_COL_LCD] = 
  {
    "   XX-XX-20XX   ",
    "    XX:XX:XX    ",
    "                ",
    "      X XX      "
  };
  
  static const unsigned char calibrating[MAX_NAMBER_LANGUAGE][MAX_COL_LCD] = 
  {
    "   Калибровка   ",
    "  Калібрування  ",
    "  Calibration   ",
    "   Калибровка   "
  };
  
  int index_language = index_language_in_array(current_settings.language);
  for(int index_1 = 0; index_1 < MAX_COL_LCD; index_1++)
    name_string[2][index_1] = calibrating[index_language][index_1];
  
  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  
  /******************************************/
  //Заповнюємо поля відповідними цифрами
  /******************************************/
  if (current_ekran.edition == 0)
  {
    time_t time_dat_tmp;
    if (save_time_dat_l == 3) time_dat_tmp = time_dat_save_l;
    else
    { 
      copying_time_dat = 1;
      time_dat_tmp = time_dat_copy;
      copying_time_dat = 0;
    }

    struct tm *p;
    p = localtime(&time_dat_tmp);
    int field;
    
    //День
    field = p->tm_mday;
    name_string[ROW_Y_][COL_DY1] = (field / 10) + 0x30;
    name_string[ROW_Y_][COL_DY2] = (field % 10) + 0x30;

    //Місяць
    field = p->tm_mon + 1;
    name_string[ROW_Y_][COL_MY1] = (field / 10) + 0x30;
    name_string[ROW_Y_][COL_MY2] = (field % 10) + 0x30;

    //Рік
    field = p->tm_year - 100;
    name_string[ROW_Y_][COL_SY1] = (field / 10) + 0x30;
    name_string[ROW_Y_][COL_SY2] = (field % 10) + 0x30;

    //Година
    field = p->tm_hour;
    name_string[ROW_T_][COL_HT1] = (field / 10) + 0x30;
    name_string[ROW_T_][COL_HT2] = (field % 10) + 0x30;

    //Хвилини
    field = p->tm_min;
    name_string[ROW_T_][COL_MT1] = (field / 10) + 0x30;
    name_string[ROW_T_][COL_MT2] = (field % 10) + 0x30;

    //Секунди
    field = p->tm_sec;
    name_string[ROW_T_][COL_ST1] = (field / 10) + 0x30;
    name_string[ROW_T_][COL_ST2] = (field % 10) + 0x30;

    //Калібровка
    if((calibration & (1<<5)) == 0) name_string[ROW_K_][COL_SK1] = '-';
    else name_string[ROW_K_][COL_SK1] = '+';
    
    name_string[ROW_K_][COL_VK2] =((calibration & 0x1f) % 10) + 0x30;
    name_string[ROW_K_][COL_VK1] =((calibration & 0x1f) / 10) + 0x30;
  }
  else
  {
    //День
    name_string[ROW_Y_][COL_DY1] = (time_edit[4] >>  4) + 0x30;
    name_string[ROW_Y_][COL_DY2] = (time_edit[4] & 0xf) + 0x30;

    //Місяць
    name_string[ROW_Y_][COL_MY1] = (time_edit[5] >>  4) + 0x30;
    name_string[ROW_Y_][COL_MY2] = (time_edit[5] & 0xf) + 0x30;

    //Рік
    name_string[ROW_Y_][COL_SY1] = (time_edit[6] >>  4) + 0x30;
    name_string[ROW_Y_][COL_SY2] = (time_edit[6] & 0xf) + 0x30;

    //Година
    name_string[ROW_T_][COL_HT1] = (time_edit[3] >>  4) + 0x30;
    name_string[ROW_T_][COL_HT2] = (time_edit[3] & 0xf) + 0x30;

    //Хвилини
    name_string[ROW_T_][COL_MT1] = (time_edit[2] >>  4) + 0x30;
    name_string[ROW_T_][COL_MT2] = (time_edit[2] & 0xf) + 0x30;

    //Секунди
    name_string[ROW_T_][COL_ST1] = (time_edit[1] >>  4) + 0x30;
    name_string[ROW_T_][COL_ST2] = (time_edit[1] & 0xf) + 0x30;

    //Калібровка
    if((calibration_edit & (1<<5)) == 0) name_string[ROW_K_][COL_SK1] = '-';
    else name_string[ROW_K_][COL_SK1] = '+';

    name_string[ROW_K_][COL_VK2] =((calibration_edit & 0x1f) % 10) + 0x30;
    name_string[ROW_K_][COL_VK1] =((calibration_edit & 0x1f) / 10) + 0x30;
  }

  /******************************************/
  
  index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  
  //Копіюємо  рядки у робочий екран
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    //Наступні рядки треба перевірити, чи їх требе відображати
    if (index_of_ekran < MAX_ROW_FOR_EKRAN_TIME)
      for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_of_ekran][j];
    else
      for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';

    index_of_ekran++;
  }

  //Відображення курору по вертикалі
  current_ekran.position_cursor_y = position_temp & (MAX_ROW_LCD - 1);
  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//Редагування часу з системи меню
/*****************************************************
Вхідні параметри
  0 - натснуто кнопку вниз
  1 - натиснуто кнопку вверх
Вхідні параметри
  Немає
*****************************************************/
void edit_time(unsigned int action)
{
  //Вводимо число у відповідне поле
  if(current_ekran.index_position == ROW_Y_)
  {
    if(current_ekran.position_cursor_x == COL_DY1)
    {
      unsigned int temp_value = time_edit[4] >> 4;
      inc_or_dec_value(&temp_value, action);
      
      time_edit[4] &= 0xf;
      time_edit[4] |= (temp_value << 4);
    }
    else if(current_ekran.position_cursor_x == COL_DY2)
    {
      unsigned int temp_value = time_edit[4] & 0xf;
      inc_or_dec_value(&temp_value, action);

      time_edit[4] &= 0xf0;
      time_edit[4] |= temp_value;
    }
    else if(current_ekran.position_cursor_x == COL_MY1)
    {
      unsigned int temp_value = time_edit[5] >> 4;
      inc_or_dec_value(&temp_value, action);

      time_edit[5] &= 0xf;
      time_edit[5] |= (temp_value << 4);
    }
    else if(current_ekran.position_cursor_x == COL_MY2)
    {
      unsigned int temp_value = time_edit[5] & 0xf;
      inc_or_dec_value(&temp_value, action);

      time_edit[5] &= 0xf0;
      time_edit[5] |= temp_value;
    }
    else if(current_ekran.position_cursor_x == COL_SY1)
    {
      unsigned int temp_value = time_edit[6] >> 4;
      inc_or_dec_value(&temp_value, action);

      time_edit[6] &= 0xf;
      time_edit[6] |= (temp_value << 4);
    }
    else if(current_ekran.position_cursor_x == COL_SY2)
    {
      unsigned int temp_value = time_edit[6] & 0xf;
      inc_or_dec_value(&temp_value, action);

      time_edit[6] &= 0xf0;
      time_edit[6] |= temp_value;
    }
  }
  else if(current_ekran.index_position == ROW_T_)
  {
    if(current_ekran.position_cursor_x == COL_HT1)
    {
      unsigned int temp_value = time_edit[3] >> 4;
      inc_or_dec_value(&temp_value, action);

      time_edit[3] &= 0xf;
      time_edit[3] |= (temp_value << 4);
    }
    else if(current_ekran.position_cursor_x == COL_HT2)
    {
      unsigned int temp_value = time_edit[3] & 0xf;
      inc_or_dec_value(&temp_value, action);

      time_edit[3] &= 0xf0;
      time_edit[3] |= temp_value;
    }
    else if(current_ekran.position_cursor_x == COL_MT1)
    {
      unsigned int temp_value = time_edit[2] >> 4;
      inc_or_dec_value(&temp_value, action);

      time_edit[2] &= 0xf;
      time_edit[2] |= (temp_value << 4);
    }
    else if(current_ekran.position_cursor_x == COL_MT2)
    {
      unsigned int temp_value = time_edit[2] & 0xf;
      inc_or_dec_value(&temp_value, action);

      time_edit[2] &= 0xf0;
      time_edit[2] |= temp_value;
    }
    else if(current_ekran.position_cursor_x == COL_ST1)
    {
      unsigned int temp_value = time_edit[1] >> 4;
      inc_or_dec_value(&temp_value, action);

      time_edit[1] &= 0xf;
      time_edit[1] |= (temp_value << 4);
    }
    else if(current_ekran.position_cursor_x == COL_ST2)
    {
      unsigned int temp_value = time_edit[1] & 0xf;
      inc_or_dec_value(&temp_value, action);

      time_edit[1] &= 0xf0;
      time_edit[1] |= temp_value;
    }
  }
  else if(current_ekran.index_position == ROW_K_)
  {
    if(current_ekran.position_cursor_x == COL_SK1) 
    {
      if ((calibration_edit & 0x20) != 0) calibration_edit &= 0x1f;
      else calibration_edit |= 0x20;
    }
    else if(current_ekran.position_cursor_x == COL_VK1)
    {
      unsigned char temp_1 = calibration_edit & 0x20, temp_2 = calibration_edit & 0x1f;
      unsigned int temp_value = temp_2 / 10;
      inc_or_dec_value(&temp_value, action);

      temp_2 %= 10;
      temp_2 += temp_value*10;
      if (temp_2 > 31) temp_2 = 31;
      calibration_edit = temp_1 | temp_2;
                
    }
    else if(current_ekran.position_cursor_x == COL_VK2)
    {
      unsigned char temp_1 = calibration_edit & 0x20, temp_2 = calibration_edit & 0x1f;
      unsigned int temp_value = temp_2 % 10;
      inc_or_dec_value(&temp_value, action);

      temp_2 = (temp_2 / 10) *10;
      temp_2 += temp_value;
      if (temp_2 > 31) temp_2 = 31;
      calibration_edit = temp_1 | temp_2;
     }
  }
}
/*****************************************************/

/*****************************************************/
//Перевіряємо достовірність даних
/*
Результат
  1 - дані достовірні
  0 - дані недостовірні
*/
/*****************************************************/
unsigned int check_data_for_data_time_menu(void)
{
  unsigned int temp_data_1, temp_data_2;
  
  /*******/
  //Рік
  /*******/
  temp_data_1 = bcd_to_int(time_edit[6]);
  if( temp_data_1 > 99 ) return 0;

  /*******/
  //Місяць
  /*******/
  temp_data_1 = bcd_to_int(time_edit[5]);
  if( (temp_data_1 == 0) || (temp_data_1 > 12)) return 0;
  
  /*******/
  //День місяця
  /*******/
  temp_data_2 = bcd_to_int(time_edit[4]);
  if((temp_data_2 == 0) || (temp_data_2 > 31)) return 0;
  //Додатков перевірка на місяць у якого є тільки 30 днів
  if (((temp_data_1 == 4) || (temp_data_1 == 6) || (temp_data_1 == 9) || (temp_data_1 == 11)) &&
      (temp_data_2 > 30)) return 0;

  //Додатков перевірка на місяць місяць лютий з врахування року високосного чи не високосного
  if((temp_data_1 == 2) && (temp_data_2 > 29)) return 0;
  else if ((temp_data_1 == 2) && (temp_data_2 == 29))
  {
    //Змінну temp_data_1 визначаємо, як номер року, бо вже надалі номер місяця нам не буде потрібен
    temp_data_1 = bcd_to_int(time_edit[6]);
    if ((temp_data_1 % 4) !=0) return 0;
  }
  /*******/

  /*******/
  //Година
  /*******/
  temp_data_2 = bcd_to_int(time_edit[3]);
  if(temp_data_2 > 23) return 0;
  /*******/

  /*******/
  //Хвилини
  /*******/
  temp_data_2 = bcd_to_int(time_edit[2]);
  if(temp_data_2 > 59) return 0;
  /*******/

  /*******/
  //Секунди
  /*******/
  temp_data_2 = bcd_to_int(time_edit[1]);
  if(temp_data_2 > 59) return 0;
  /*******/

  /*******/
  //Соті секунди
  /*******/
  temp_data_1 = bcd_to_int(time_edit[0]);
  if( temp_data_1 > 99 ) return 0;

  //Вихід з повідомленням, що всі параметри у  межах допуску
  return 1;
}
/*****************************************************/


/*****************************************************/
//Перевід числа з формату BCD в int 
/*****************************************************/
unsigned int bcd_to_int(unsigned char input)
{
  unsigned int output, temp_data;
  
  temp_data = input >> 4;
  output = temp_data*10;
  output +=input & 0xf;
  
  return output;
}
/*****************************************************/


/*****************************************************/
// 
/*****************************************************/
/*****************************************************/
