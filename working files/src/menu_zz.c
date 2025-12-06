#include "header.h"

/*****************************************************/
//Формуємо екран відображення уставок ЗЗ
/*****************************************************/
void make_ekran_setpoint_zz(unsigned int group)
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_SETPOINT_ZZ][MAX_COL_LCD] = 
  {
    {
      "  Уставка 3I0   ",
      "  Уставка 3U0   "
    },
    {
      "  Уставка 3I0   ",
      "  Уставка 3U0   "
    },
    {
      "    SEFP3I0     ",
      "    SEFP3V0     "
    },
    {
      "  Уставка 3I0   ",
      "  Уставка 3U0   "
    }
  };
  int index_language = index_language_in_array(current_settings.language);
  
  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  unsigned int vaga, value, first_symbol;
  
  //Множення на два величини position_temp потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
  index_of_ekran = ((position_temp<<1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;
  
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    unsigned int index_of_ekran_tmp = index_of_ekran >> 1;
    unsigned int view = ((current_ekran.edition == 0) || (position_temp != index_of_ekran_tmp));
    if (index_of_ekran_tmp < MAX_ROW_FOR_SETPOINT_ZZ)
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][index_of_ekran_tmp][j];
        if (index_of_ekran_tmp == INDEX_ML_STPZZ1_3I0)
        {
          vaga = 1000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для уставки ЗЗ
          if (view == true) value = current_settings.setpoint_zz_3I0[group]; //у змінну value поміщаємо значення уставки ЗЗ
          else value = edition_settings.setpoint_zz_3I0[group];
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
        else if (index_of_ekran_tmp == INDEX_ML_STPZZ1_3U0)
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для уставки ЗЗ1/3U0
          if (view == true) value = current_settings.setpoint_zz_3U0[group]; //у змінну value поміщаємо значення уставки ЗЗ1/3U0
          else value = edition_settings.setpoint_zz_3U0[group];
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
      }
      else
      {
        //У парному номері рядку виводимо значення уставки
        for (unsigned int j = 0; j<MAX_COL_LCD; j++)
        {
          if (index_of_ekran_tmp == INDEX_ML_STPZZ1_3I0)
          {
            if (
                ((j < COL_SETPOINT_ZZ1_3I0_BEGIN) ||  (j > COL_SETPOINT_ZZ1_3I0_END )) &&
                (j != (COL_SETPOINT_ZZ1_3I0_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_SETPOINT_ZZ1_3I0_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_SETPOINT_ZZ1_3I0_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_A];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_ZZ1_3I0_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == INDEX_ML_STPZZ1_3U0)
          {
            if (
                ((j < COL_SETPOINT_ZZ1_3U0_BEGIN) ||  (j > COL_SETPOINT_ZZ1_3U0_END )) &&
                (j != (COL_SETPOINT_ZZ1_3U0_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_SETPOINT_ZZ1_3U0_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_SETPOINT_ZZ1_3U0_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_V];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_ZZ1_3U0_COMMA, view, 0);
          }
        }
      }
        
    }
    else
      for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';

    index_of_ekran++;
  }

  //Відображення курсору по вертикалі і курсор завжди має бути у полі із значенням устаки
  current_ekran.position_cursor_y = ((position_temp<<1) + 1) & (MAX_ROW_LCD - 1);
  //Курсор по горизонталі відображається на першому символі у випадку, коли ми не в режимі редагування, інакше позиція буде визначена у функцї main_manu_function
  if (current_ekran.edition == 0)
  {
    int last_position_cursor_x = MAX_COL_LCD;
    if (current_ekran.index_position == INDEX_ML_STPZZ1_3I0)
    {
      current_ekran.position_cursor_x = COL_SETPOINT_ZZ1_3I0_BEGIN;
      last_position_cursor_x = COL_SETPOINT_ZZ1_3I0_END;
    }
    else if (current_ekran.index_position == INDEX_ML_STPZZ1_3U0)
    {
      current_ekran.position_cursor_x = COL_SETPOINT_ZZ1_3U0_BEGIN;
      last_position_cursor_x = COL_SETPOINT_ZZ1_3U0_END;
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
//Формуємо екран відображення витримок ЗЗ
/*****************************************************/
void make_ekran_timeout_zz(unsigned int group)
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_TIMEOUT_ZZ][MAX_COL_LCD] = 
  {
    {
      "  Выдержка 3I0  ",
      "  Выдержка 3U0  ",
      "  Выдержка НЗЗ  "
    },
    {
      "  Витримка 3I0  ",
      "  Витримка 3U0  ",
      "  Витримка СЗЗ  "
    },
    {
      " SEFP3I0 Op Del ",
      " SEFP3V0 Op Del ",
      "  SDEFP Op Del  "
    },
    {
      "  Выдержка 3I0  ",
      "  Выдержка 3U0  ",
      "  Выдержка НЗЗ  "
    }
  };
  int index_language = index_language_in_array(current_settings.language);
  
  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  unsigned int vaga, value, first_symbol;
  
  //Множення на два величини position_temp потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
  index_of_ekran = ((position_temp<<1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;
  
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    unsigned int index_of_ekran_tmp = index_of_ekran >> 1;
    unsigned int view = ((current_ekran.edition == 0) || (position_temp != index_of_ekran_tmp));
    if (
        (
         (index_of_ekran_tmp <= INDEX_ML_TMOZZ1_3U0) || 
         ((current_settings.control_zz & CTR_ZZ1_TYPE) == 0)  
        )   
        &&  
        (index_of_ekran_tmp < MAX_ROW_FOR_TIMEOUT_ZZ)
       )   
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][index_of_ekran_tmp][j];
        
        if (index_of_ekran_tmp == INDEX_ML_TMOZZ1_3I0)
        {
          vaga = 10000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (view == true) value = current_settings.timeout_zz_3I0[group]; //у змінну value поміщаємо значення витримки
          else value = edition_settings.timeout_zz_3I0[group];
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
        else if (index_of_ekran_tmp == INDEX_ML_TMOZZ1_3U0)
        {
          vaga = 10000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (view == true) value = current_settings.timeout_zz_3U0[group]; //у змінну value поміщаємо значення витримки
          else value = edition_settings.timeout_zz_3U0[group];
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
        else if (index_of_ekran_tmp == INDEX_ML_TMOZZ1_NZZ)
        {
          vaga = 10000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (view == true) value = current_settings.timeout_nzz[group]; //у змінну value поміщаємо значення витримки
          else value = edition_settings.timeout_nzz[group];
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
      }
      else
      {
        //У парному номері рядку виводимо значення уставки
        for (unsigned int j = 0; j<MAX_COL_LCD; j++)
        {
          if (index_of_ekran_tmp == INDEX_ML_TMOZZ1_3I0)
          {
            if (
                ((j < COL_TMO_ZZ1_3I0_BEGIN) ||  (j > COL_TMO_ZZ1_3I0_END )) &&
                (j != (COL_TMO_ZZ1_3I0_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_ZZ1_3I0_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_ZZ1_3I0_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_ZZ1_3I0_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == INDEX_ML_TMOZZ1_3U0)
          {
            if (
                ((j < COL_TMO_ZZ1_3U0_BEGIN) ||  (j > COL_TMO_ZZ1_3U0_END )) &&
                (j != (COL_TMO_ZZ1_3U0_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_ZZ1_3U0_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_ZZ1_3U0_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_ZZ1_3U0_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == INDEX_ML_TMOZZ1_NZZ)
          {
            if (
                ((j < COL_TMO_ZZ1_NZZ_BEGIN) ||  (j > COL_TMO_ZZ1_NZZ_END )) &&
                (j != (COL_TMO_ZZ1_NZZ_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_ZZ1_NZZ_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_ZZ1_NZZ_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_ZZ1_NZZ_COMMA, view, 0);
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
    if (current_ekran.index_position == INDEX_ML_TMOZZ1_3I0)
    {
      current_ekran.position_cursor_x = COL_TMO_ZZ1_3I0_BEGIN;
      last_position_cursor_x = COL_TMO_ZZ1_3I0_END;
    }
    else if (current_ekran.index_position == INDEX_ML_TMOZZ1_3U0)
    {
      current_ekran.position_cursor_x = COL_TMO_ZZ1_3U0_BEGIN;
      last_position_cursor_x = COL_TMO_ZZ1_3U0_END;
    }
    else if (current_ekran.index_position == INDEX_ML_TMOZZ1_NZZ)
    {
      current_ekran.position_cursor_x = COL_TMO_ZZ1_NZZ_BEGIN;
      last_position_cursor_x = COL_TMO_ZZ1_NZZ_END;
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
//Формуємо екран відображення значення управлінської інформації для ЗЗ
/*****************************************************/
void make_ekran_control_zz()
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_CONTROL_ZZ][MAX_COL_LCD] = 
  {
    {
      " Защита ЗЗ/3I0  ",
      " Вариант ЗЗ/3I0 ",
      " Защита ЗЗ/3U0  ",
      "   Защита НЗЗ   ",
      "   Сектор НЗЗ   "
    },
    {
      " Захист ЗЗ/3I0  ",
      " Варіант ЗЗ/3I0 ",
      " Захист ЗЗ/3U0  ",
      "   Захист СЗЗ   ",
      "   Сектор СЗЗ   "
    },
    {
      "     SEF3I0     ",
      "  SEFP3I0 Type  ",
      "     EF3U0      ",
      "     SDEFP      ",
      "   SDEFP Sec    "
    },
    {
      " Защита ЗЗ/3I0  ",
      " Вариант ЗЗ/3I0 ",
      " Защита ЗЗ/3U0  ",
      "   Защита НЗЗ   ",
      "   Сектор НЗЗ   "
    }
  };
  unsigned char name_string_tmp[MAX_ROW_FOR_CONTROL_ZZ][MAX_COL_LCD];

  static const unsigned char information[MAX_ROW_FOR_CONTROL_ZZ][MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] = 
  {
    {
      {"     Откл.      ", "      Вкл.      "},
      {"     Вимк.      ", "     Увімк.     "},
      {"      Off       ", "       On       "},
      {"     Сљнд.      ", "     Косу.      "}
    },
    {
      {"     РТЗ50      ", "      УСЗ       "},
      {"     РТЗ50      ", "      УСЗ       "},
      {"       MH       ", "      HiH       "},
      {"     РТЗ50      ", "      УСЗ       "}
    },
    {
      {"     Откл.      ", "      Вкл.      "},
      {"     Вимк.      ", "     Увімк.     "},
      {"      Off       ", "       On       "},
      {"     Сљнд.      ", "     Косу.      "}
    },
    {
      {"     Откл.      ", "      Вкл.      "},
      {"     Вимк.      ", "     Увімк.     "},
      {"      Off       ", "       On       "},
      {"     Сљнд.      ", "     Косу.      "}
    },
    {
      {"    Обратный    ", "     Прямой     "},
      {"   Зворотний    ", "     Прямий     "},
      {"       Bw       ", "       Fw       "},
      {"    Обратный    ", "     Прямой     "}
    }
  };
  unsigned char information_tmp[MAX_ROW_FOR_CONTROL_ZZ][2][MAX_COL_LCD];

  static const unsigned int cursor_x[MAX_ROW_FOR_CONTROL_ZZ][MAX_NAMBER_LANGUAGE][2] = 
  {
    {
      {4, 5},
      {4, 4},
      {5, 6},
      {4, 4}
    },
    {
      {4, 5},
      {4, 5},
      {6, 5},
      {4, 5}
    },
    {
      {4, 5},
      {4, 4},
      {5, 6},
      {4, 4}
    },
    {
      {4, 5},
      {4, 4},
      {5, 6},
      {4, 4}
    },
    {
      {3, 4},
      {2, 4},
      {6, 6},
      {3, 4}
    }
  };
  unsigned int cursor_x_tmp[MAX_ROW_FOR_CONTROL_ZZ][2]; 

  unsigned int temp_data;
  if(current_ekran.edition == 0) temp_data = current_settings.control_zz;
  else temp_data = edition_settings.control_zz;
  
  int index_language = index_language_in_array(current_settings.language);
  for(int index_1 = 0; index_1 < MAX_ROW_FOR_CONTROL_ZZ; index_1++)
  {
    for(int index_2 = 0; index_2 < MAX_COL_LCD; index_2++)
      name_string_tmp[index_1][index_2] = name_string[index_language][index_1][index_2];

    for(int index_2 = 0; index_2 < 2; index_2++)
    {
      for(int index_3 = 0; index_3 < MAX_COL_LCD; index_3++)
      {
        information_tmp[index_1][index_2][index_3] = information[index_1][index_language][index_2][index_3];
      }

      cursor_x_tmp[index_1][index_2] = cursor_x[index_1][index_language][index_2];
    }
  }
  
  /******************************************/
  //Виключаємо поля, які не треба відображати
  /******************************************/
  int additional_current = 0;
  int position_temp = current_ekran.index_position;
  int index_of_ekran;

  for (int current_index = 0; current_index < MAX_ROW_FOR_CONTROL_ZZ; current_index++ )
  {

    if (
        (
         ((current_ekran.edition == 0) && ((current_settings.control_zz & CTR_ZZ1_TYPE) != 0))
         ||  
         ((current_ekran.edition != 0) && ((edition_settings.control_zz & CTR_ZZ1_TYPE) != 0))
        )   
        &&
        (current_index > INDEX_ML_CTR_ZZ1_3U0_STATE)
       )   
    {
      int i = current_index - additional_current;
      unsigned int maska_1, maska_2;
      maska_1 = (1 << i) - 1;
      maska_2 = (unsigned int)(~maska_1);
    
      unsigned int temp_data_1 = (temp_data >> 1) & maska_2;
      temp_data = (temp_data & maska_1) | temp_data_1;
    
      if ((i+1) <= position_temp) position_temp--;
      do
      {
        for(unsigned int j = 0; j < MAX_COL_LCD; j++)
        {
          if ((i+1) < (MAX_ROW_FOR_CONTROL_ZZ - additional_current)) name_string_tmp[i][j] = name_string_tmp[i + 1][j];
          else name_string_tmp[i][j] = ' ';
        }

        for(unsigned int j = 0; j < 2; j++)
        {
          for(unsigned int k = 0; k < MAX_COL_LCD; k++)
          {
            if ((i+1) < (MAX_ROW_FOR_CONTROL_ZZ - additional_current)) information_tmp[i][j][k] = information_tmp[i + 1][j][k];
            else information_tmp[i][j][k] = ' ';
          }

          if ((i+1) < (MAX_ROW_FOR_CONTROL_ZZ - additional_current)) cursor_x_tmp[i][j] = cursor_x_tmp[i + 1][j];
          else cursor_x_tmp[i][j] = 0;
        }

        i++;
      }
      while (i< (MAX_ROW_FOR_CONTROL_ZZ - additional_current));
      additional_current++;
    }
  }
  /******************************************/
  
  //Множення на два величини position_temp потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
  index_of_ekran = ((position_temp<<1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    int index_of_ekran_tmp = index_of_ekran >> 1;
    if (index_of_ekran_tmp < (MAX_ROW_FOR_CONTROL_ZZ - additional_current))
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string_tmp[index_of_ekran_tmp][j];
      }
      else
      {
        //У парному номері рядку виводимо стан
        
        unsigned int index_ctr = index_of_ekran_tmp;

        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = information_tmp[index_ctr][(temp_data >> index_ctr) & 0x1][j];
        if (position_temp == index_of_ekran_tmp) current_ekran.position_cursor_x = cursor_x_tmp[index_ctr][(temp_data >> index_ctr) & 0x1];
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
