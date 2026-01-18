#include "header.h"

/*****************************************************/
//Формуємо екран відображення уставок МТЗ 0.4кВ
/*****************************************************/
void make_ekran_setpoint_mtz04(unsigned int group)
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_SETPOINT_MTZ04][MAX_COL_LCD] = 
  {
    {
      "  МТЗ 0.4кВ 1   ",
      "  МТЗ 0.4кВ 2   "
    },
    {
      "  МСЗ 0.4кВ 1   ",
      "  МСЗ 0.4кВ 2   "
    },
    {
      "    LVOCP 1     ",
      "    LVOCP 2     "
    },
    {
      "  МТЗ 0.4кВ 1   ",
      "  МТЗ 0.4кВ 2   "
    }
  };
  int index_language = index_language_in_array(current_settings.language);
  
  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  unsigned int vaga, value, first_symbol;
  
  //Множення на два величини position_temp потрібне для того, бо наодн позицію ми використовуємо два рядки (назва + значення)
  index_of_ekran = ((position_temp<<1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;
  
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    unsigned int index_of_ekran_tmp = index_of_ekran >> 1;
    unsigned int view = ((current_ekran.edition == 0) || (position_temp != index_of_ekran_tmp));
    if (index_of_ekran_tmp < MAX_ROW_FOR_SETPOINT_MTZ04)
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][index_of_ekran_tmp][j];
        if (index_of_ekran_tmp == INDEX_ML_STPMTZ04_1)
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду
          if (view == true) value = current_settings.setpoint_mtz04_1[group]; //у змінну value поміщаємо значення уставки
          else value = edition_settings.setpoint_mtz04_1[group];
        }
        else if (index_of_ekran_tmp == INDEX_ML_STPMTZ04_2)
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду
          if (view == true) value = current_settings.setpoint_mtz04_2[group]; //у змінну value поміщаємо значення уставки
          else value = edition_settings.setpoint_mtz04_2[group];
        }
        
        first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
      }
      else
      {
        //У парному номері рядку виводимо значення уставки
        for (unsigned int j = 0; j<MAX_COL_LCD; j++)
        {
          if (index_of_ekran_tmp == INDEX_ML_STPMTZ04_1)
          {
            if (
                ((j < COL_SETPOINT_MTZ04_1_BEGIN) ||  (j > COL_SETPOINT_MTZ04_1_END ))  &&
                (j != (COL_SETPOINT_MTZ04_1_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_SETPOINT_MTZ04_1_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_SETPOINT_MTZ04_1_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_A];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_MTZ04_1_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == INDEX_ML_STPMTZ04_2)
          {
            if (
                ((j < COL_SETPOINT_MTZ04_2_BEGIN) ||  (j > COL_SETPOINT_MTZ04_2_END ))  &&
                (j != (COL_SETPOINT_MTZ04_2_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_SETPOINT_MTZ04_2_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_SETPOINT_MTZ04_2_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_A];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_MTZ04_2_COMMA, view, 0);
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
    if (current_ekran.index_position == INDEX_ML_STPMTZ04_1)
    {
      current_ekran.position_cursor_x = COL_SETPOINT_MTZ04_1_BEGIN;
      last_position_cursor_x = COL_SETPOINT_MTZ04_1_END;
    }
    else if (current_ekran.index_position == INDEX_ML_STPMTZ04_2)
    {
      current_ekran.position_cursor_x = COL_SETPOINT_MTZ04_2_BEGIN;
      last_position_cursor_x = COL_SETPOINT_MTZ04_2_END;
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
//Формуємо екран відображення витримок МТЗ 0.4кВ
/*****************************************************/
void make_ekran_timeout_mtz04(unsigned int group)
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_TIMEOUT_MTZ04][MAX_COL_LCD] = 
  {
    {
      "  МТЗ 0.4кВ 1   ",
      "  МТЗ 0.4кВ 2   ",
      " Уск.МТЗ 0.4кВ 2",
      "В.Ус.МТЗ 0.4кВ 2"
    },
    {
      "  МСЗ 0.4кВ 1   ",
      "  МСЗ 0.4кВ 2   ",
      " Пр.МСЗ 0.4кВ 2 ",
      "В.Пр.МСЗ 0.4кВ 2"
    },
    {
      "    LVOCP 1     ",
      "    LVOCP 2     ",
      " LVOCP 2 AcOpDl ",
      "LVOCP2 Acc Ena T"
    },
    {
      "  МТЗ 0.4кВ 1   ",
      "  МТЗ 0.4кВ 2   ",
      " Уск.МТЗ 0.4кВ 2",
      "В.Ус.МТЗ 0.4кВ 2"
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
    if (index_of_ekran_tmp < MAX_ROW_FOR_TIMEOUT_MTZ04)
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][index_of_ekran_tmp][j];
        if (index_of_ekran_tmp == INDEX_ML_TMOMTZ04_1)
        {
          vaga = 10000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (view == true) value = current_settings.timeout_mtz04_1[group]; //у змінну value поміщаємо значення витримки
          else value = edition_settings.timeout_mtz04_1[group];
        }
        else if (index_of_ekran_tmp == INDEX_ML_TMOMTZ04_2)
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (view == true) value = current_settings.timeout_mtz04_2[group]; //у змінну value поміщаємо значення витримки
          else value = edition_settings.timeout_mtz04_2[group];
        }
        else if (index_of_ekran_tmp == INDEX_ML_TMOMTZ04_2_PR)
        {
          vaga = 1000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (view == true) value = current_settings.timeout_mtz04_2_pr[group]; //у змінну value поміщаємо значення витримки
          else value = edition_settings.timeout_mtz04_2_pr[group];
        }
        else if (index_of_ekran_tmp == INDEX_ML_TMOMTZ04_2_VVID_PR)
        {
          vaga = 1000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (view == true) value = current_settings.timeout_mtz04_2_vvid_pr[group]; //у змінну value поміщаємо значення витримки
          else value = edition_settings.timeout_mtz04_2_vvid_pr[group];
        }

        first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
      }
      else
      {
        //У парному номері рядку виводимо значення уставки
        for (unsigned int j = 0; j<MAX_COL_LCD; j++)
        {
          if (index_of_ekran_tmp == INDEX_ML_TMOMTZ04_1)
          {
            if (
                ((j < COL_TMO_MTZ04_1_BEGIN) ||  (j > COL_TMO_MTZ04_1_END )) &&
                (j != (COL_TMO_MTZ04_1_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_MTZ04_1_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_MTZ04_1_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_MTZ04_1_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == INDEX_ML_TMOMTZ04_2)
          {
            if (
                ((j < COL_TMO_MTZ04_2_BEGIN) ||  (j > COL_TMO_MTZ04_2_END )) &&
                (j != (COL_TMO_MTZ04_2_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_MTZ04_2_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_MTZ04_2_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_MTZ04_2_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == INDEX_ML_TMOMTZ04_2_PR)
          {
            if (
                ((j < COL_TMO_MTZ04_2_PR_BEGIN) ||  (j > COL_TMO_MTZ04_2_PR_END )) &&
                (j != (COL_TMO_MTZ04_2_PR_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_MTZ04_2_PR_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_MTZ04_2_PR_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_MTZ04_2_PR_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == INDEX_ML_TMOMTZ04_2_VVID_PR)
          {
            if (
                ((j < COL_TMO_MTZ04_2_VVID_PR_BEGIN) ||  (j > COL_TMO_MTZ04_2_VVID_PR_END )) &&
                (j != (COL_TMO_MTZ04_2_VVID_PR_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_MTZ04_2_VVID_PR_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_MTZ04_2_VVID_PR_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_MTZ04_2_VVID_PR_COMMA, view, 0);
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
    if (current_ekran.index_position == INDEX_ML_TMOMTZ04_1) 
    {
      current_ekran.position_cursor_x = COL_TMO_MTZ04_1_BEGIN;
      last_position_cursor_x = COL_TMO_MTZ04_1_END;
    }
    else if (current_ekran.index_position == INDEX_ML_TMOMTZ04_2) 
    {
      current_ekran.position_cursor_x = COL_TMO_MTZ04_2_BEGIN;
      last_position_cursor_x = COL_TMO_MTZ04_2_END;
    }
    else if (current_ekran.index_position == INDEX_ML_TMOMTZ04_2_PR) 
    {
      current_ekran.position_cursor_x = COL_TMO_MTZ04_2_PR_BEGIN;
      last_position_cursor_x = COL_TMO_MTZ04_2_PR_END;
    }
    else if (current_ekran.index_position == INDEX_ML_TMOMTZ04_2_VVID_PR) 
    {
      current_ekran.position_cursor_x = COL_TMO_MTZ04_2_VVID_PR_BEGIN;
      last_position_cursor_x = COL_TMO_MTZ04_2_VVID_PR_END;
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
//Формуємо екран відображення значення управлінської інформації для МТЗ 0.4кВ
/*****************************************************/
void make_ekran_control_mtz04()
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_CONTROL_MTZ04][MAX_COL_LCD] = 
  {
    {
      "  МТЗ 0.4кВ 1   ",
      "  МТЗ 0.4кВ 2   ",
      " Тип МТЗ 0.4кВ 2",
      " У-е МТЗ 0.4кВ 2",
      " У-я МТЗ 0.4кВ 2"
    },
    {
      "  МСЗ 0.4кВ 1   ",
      "  МСЗ 0.4кВ 2   ",
      " Тип МСЗ 0.4кВ 2",
      " П-я МСЗ 0.4кВ 2",
      " П-й МСЗ 0.4кВ 2"
    },
    {
      "    LVOCP 1     ",
      "    LVOCP 2     ",
      "  LVOCP 2 Type  ",
      " LVOCP 2 Acc Ena",
      " LVOCP 2 Acc С  "
    },
    {
      "  МТЗ 0.4кВ 1   ",
      "  МТЗ 0.4кВ 2   ",
      " Тип МТЗ 0.4кВ 2",
      " У-е МТЗ 0.4кВ 2",
      " У-я МТЗ 0.4кВ 2"
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
    if (index_of_ekran_tmp < MAX_ROW_FOR_CONTROL_MTZ04)
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

        __SETTINGS *point;
          if(current_ekran.edition == 0) point = &current_settings;
          else point = &edition_settings;

        if (index_ctr == INDEX_ML_CTRMTZ04_2_TYPE)
        {
          int value = point->type_mtz04_2;

          static const unsigned char information[MAX_NAMBER_LANGUAGE][6][MAX_COL_LCD] = 
          {
            {"    Простая     ", "     Ошибка     ", "     Ошибка     ", " Завис.(Тип A)  ", " Завис.(Тип B)  ", " Завис.(Тип C)  "},
            {"    Простий     ", "    Помилка     ", "    Помилка     ", "Залежний (Тип A)", "Залежний (Тип B)", "Залежний (Тип C)"},
            {"    Non Dir     ", "     Error      ", "     Error      ", "  Dep (Type A)  ", "  Dep (Type B)  ", "  Dep (Type C)  "},
            {"    Простая     ", "     Ошибка     ", "     Ошибка     ", " Завис.(Тип A)  ", " Завис.(Тип B)  ", " Завис.(Тип C)  "}
          };
          static const unsigned int cursor_x[MAX_NAMBER_LANGUAGE][6] = 
          {
            {3, 4, 4, 0, 0, 0},
            {3, 3, 3, 0, 0, 0},
            {3, 4, 4, 1, 1, 1},
            {3, 4, 4, 0, 0, 0}
          };
          
          for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = information[index_language][value][j];
          if (position_temp == index_of_ekran_tmp) current_ekran.position_cursor_x = cursor_x[index_language][value];
        }
        else
        {
          unsigned int temp_data = point->control_mtz04;
          unsigned int n_bit = 0;
          
          //Виділяємо номер біту
          if      (index_ctr == INDEX_ML_CTRMTZ04_1             ) n_bit = N_BIT_CTRMTZ04_1;
          else if (index_ctr == INDEX_ML_CTRMTZ04_2             ) n_bit = N_BIT_CTRMTZ04_2;
          else if (index_ctr == INDEX_ML_CTRMTZ04_2_PRYSKORENNJA) n_bit = N_BIT_CTRMTZ04_2_PRYSKORENNJA;
          else if (index_ctr == INDEX_ML_CTRMTZ04_2_PRYSKORENA  ) n_bit = N_BIT_CTRMTZ04_2_PRYSKORENA;
          
          for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = information_off_on[index_language][(temp_data >> n_bit) & 0x1][j];
          if (position_temp == index_of_ekran_tmp) current_ekran.position_cursor_x = cursor_x_off_on[index_language][(temp_data >> n_bit) & 0x1];
        }
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
