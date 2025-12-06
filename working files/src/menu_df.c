#include "header.h"

/*****************************************************/
//Формуємо екран відображення заголовків настроювання опреділювальних функцій
/*****************************************************/
void make_ekran_chose_settings_df(void)
{
  static unsigned char const name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_SETTINGS_DF][MAX_COL_LCD] = 
  {
    {
      " Источники      ",
      " Таймеры        ",
      " Тип функции    "
    },
    {
      " Джерела        ",
      " Таймери        ",
      " Тип функції    "
    },
    {
      " Inputs         ",
      " Timers         ",
      " Func Type      "
    },
    {
      " Источники      ",
      " Таймеры        ",
      " Тип функции    "
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
    if (index_of_ekran < MAX_ROW_SETTINGS_DF)
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
//Формуємо екран відображення вікна відображення типу опреділювальної функції
/*****************************************************/
void make_ekran_type_df(void)
{
  static unsigned char const name_string[MAX_NAMBER_LANGUAGE][MAX_COL_LCD] = 
  {
    "  О-функция     ",
    "  В-функція     ",
    "   UD-Func      ",
    "  О-функция     "
  };
  static const unsigned int first_index_number[MAX_NAMBER_LANGUAGE] = {12, 12, 11, 12};
  int index_language = index_language_in_array(current_settings.language);
  unsigned int first_index_number_1 = first_index_number[index_language];
  
  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  
  //Множення на два величини position_temp потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
  index_of_ekran = ((position_temp<<1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;
  
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    unsigned int index_of_ekran_tmp = index_of_ekran >> 1;
    if (index_of_ekran_tmp < NUMBER_DEFINED_FUNCTIONS)
    {
      if ((i & 0x1) == 0)
      {
        unsigned int number = (index_of_ekran >> 1) + 1;
        unsigned int tmp_1 = (number / 10), tmp_2 = number - tmp_1*10;
        
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j<MAX_COL_LCD; j++)
        {
          if ((j < first_index_number_1) || (j > (first_index_number_1 + 1)))
            working_ekran[i][j] = name_string[index_language][j];
          else if (j == first_index_number_1)
          {
            if (tmp_1 > 0 ) working_ekran[i][j] = tmp_1 + 0x30;
          }
          else     
          {
            if (tmp_1 > 0 )
            {
              working_ekran[i][j] = tmp_2 + 0x30;
            }
            else
            {
              working_ekran[i][j - 1] = tmp_2 + 0x30;
              working_ekran[i][j] = ' ';
            }
          }
        }
      }
      else
      {
        //У парному номері рядку виводимо значення 
        unsigned int index_ctr = index_of_ekran_tmp;
        unsigned int temp_data;

        static unsigned char const information[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] = 
        {
          {"     ПРЯМАЯ     ", "    ОБРАТНАЯ    "},
          {"     ПРЯМА      ", "    ЗВОРОТНА    "},
          {"    Forward     ", "    Reverse     "},
          {"     ПРЯМАЯ     ", "    ОБРАТНАЯ    "}
        };
        static const unsigned int cursor_x[MAX_NAMBER_LANGUAGE][2] = 
        {
          {4, 3},
          {4, 3},
          {3, 3},
          {4, 3}
        };
        
        if(current_ekran.edition == 0) temp_data = current_settings.type_df;
        else temp_data = edition_settings.type_df;
                
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = information[index_language][(temp_data >> index_ctr) & 0x1][j];
        if (position_temp == index_of_ekran_tmp) current_ekran.position_cursor_x = cursor_x[index_language][(temp_data >> index_ctr) & 0x1];
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
//Формуємо екран відображення часових параметнів для опреділювальних функцій
/*****************************************************/
void make_ekran_timeout_df(unsigned int number_df)
{
  static unsigned char const name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_TIMEOUT_DF][MAX_COL_LCD] = 
  {
    {
      "  Таймер паузы  ",
      " Таймер работы  "
    },
    {
      "  Таймер паузи  ",
      " Таймер роботи  "
    },
    {
      "  Pause Timer   ",
      " Operation Timer"
    },
    {
      "  Таймер паузы  ",
      " Таймер работы  "
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
    if (index_of_ekran_tmp < MAX_ROW_TIMEOUT_DF)
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][index_of_ekran_tmp][j];
        if (index_of_ekran_tmp == INDEX_ML_TMO_DF_PAUSE)
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для таймера павзи
          if (view == true) value = current_settings.timeout_pause_df[number_df]; //у змінну value поміщаємо значення таймера павзи
          else value = edition_settings.timeout_pause_df[number_df];
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
        else if (index_of_ekran_tmp == INDEX_ML_TMO_DF_WORK)
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для таймера роботи
          if (view == true) value = current_settings.timeout_work_df[number_df]; //у змінну value поміщаємо значення таймера роботи
          else value = edition_settings.timeout_work_df[number_df];
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
      }
      else
      {
        //У парному номері рядку виводимо значення уставки
        for (unsigned int j = 0; j<MAX_COL_LCD; j++)
        {
          if (index_of_ekran_tmp == INDEX_ML_TMO_DF_PAUSE)
          {
            if (
                ((j < COL_TMO_DF_PAUSE_BEGIN) ||  (j > COL_TMO_DF_PAUSE_END )) &&
                (j != (COL_TMO_DF_PAUSE_END + 2))
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_DF_PAUSE_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_DF_PAUSE_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_DF_PAUSE_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == INDEX_ML_TMO_DF_WORK)
          {
            if (
                ((j < COL_TMO_DF_WORK_BEGIN) ||  (j > COL_TMO_DF_WORK_END )) &&
                (j != (COL_TMO_DF_WORK_END + 2))
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_DF_WORK_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_DF_WORK_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_DF_WORK_COMMA, view, 0);
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
    if (current_ekran.index_position == INDEX_ML_TMO_DF_PAUSE)
    {
      current_ekran.position_cursor_x = COL_TMO_DF_PAUSE_BEGIN;
      last_position_cursor_x = COL_TMO_DF_PAUSE_END;
    }
    else if (current_ekran.index_position == INDEX_ML_TMO_DF_WORK) 
    {
      current_ekran.position_cursor_x = COL_TMO_DF_WORK_BEGIN;
      last_position_cursor_x = COL_TMO_DF_WORK_END;
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
//Формуємо екран відображення типів джерел ранжування опреділюваних функцій
/*****************************************************/
void make_ekran_list_type_source_df(void)
{
  static unsigned char const name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_LIST_TYPE_SOURCE_DF][MAX_COL_LCD] = 
  {
    {
      " Источники прям.",
      " Источники обр. ",
      " Источники блок.",
      " Источники cбр. "
    },
    {
      " Джерела прям.  ",
      " Джерела звор.  ",
      " Джерела блок.  ",
      " Джерела ск.    "
    },
    {
      " Direct Inputs  ",
      " Inverse Inputs ",
      " Blk Inputs     ",
      " Reset Inputs   "
    },
    {
      " Источники прям.",
      " Источники обр. ",
      " Источники блок.",
      " Источники cбр. "
    },
  };
  int index_language = index_language_in_array(current_settings.language);
  
  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  
  
  index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  
  //Копіюємо  рядки у робочий екран
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    //Наступні рядки треба перевірити, чи їх требе відображати у текучій коффігурації
    if (index_of_ekran < MAX_ROW_LIST_TYPE_SOURCE_DF)
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
//
/*****************************************************/
/*****************************************************/
