#include "header.h"

/*****************************************************/
//Формуємо екран відображення заголовків настроювання УВВ
/*****************************************************/
void make_ekran_chose_settings_uvv(void)
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_CHOSE_SETTINGS_UVV][MAX_COL_LCD] = 
  {
    {
      " Допуск д.входа ",
      " Вид входа      ",
      " Тип вх.сигнала ",
      " Вид выхода     ",
      " Вид индикатора ",
      " Режим Ф-кнопки "
    },
    {
      " Допуск д.входу ",
      " Вид входу      ",
      " Тип вх.сигналу ",
      " Вид виходу     ",
      " Вид індикатора ",
      " Режим Ф-кнопки "
    },
    {
      " BI Tolerance   ",
      " Input Type     ",
      " BI signal type ",
      " BO Type        ",
      " LED Type       ",
      " UD-Key Mode    "
    },
    {
      " Допуск д.входа ",
      " Вид входа      ",
      " Тип вх.сигнала ",
      " Вид выхода     ",
      " Вид индикатора ",
      " Режим Ф-кнопки "
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
    if (index_of_ekran < MAX_ROW_FOR_CHOSE_SETTINGS_UVV)
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
//Формуємо екран відображення допусків ДВ
/*****************************************************/
void make_ekran_dopusk_dv()
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_COL_LCD] = 
  {
    "    ДВx.        ",
    "    ДВx.        ",
    "     BI.        ",
    "    ДВx.        "
  };
  static const unsigned int first_index_number[MAX_NAMBER_LANGUAGE] = {8, 8, 8, 8};
  static const unsigned char ms[MAX_NAMBER_LANGUAGE][2] = {"мс", "мс", "ms", "мс"};

  int index_language = index_language_in_array(current_settings.language);
  unsigned int first_index_number_1 = first_index_number[index_language];
  
  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  unsigned int vaga, value, first_symbol;
  
  //Множення на два величини position_temp потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
  index_of_ekran = ((position_temp<<1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;
  
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    unsigned int index_of_ekran_tmp = index_of_ekran >> 1;
    unsigned int view = ((current_ekran.edition == 0) || (position_temp != index_of_ekran_tmp));
    if (index_of_ekran_tmp < NUMBER_INPUTS)
    {
      if ((i & 0x1) == 0)
      {
        unsigned int number = index_of_ekran_tmp + 1;
        int tmp_1 = -1, tmp_2 = -1;

        for (size_t j = 0; j < N_INPUT_BOARDS; j++)
        {
          if (number <= input_boards[j][0])
          {
            tmp_1 = input_boards[j][1];
            tmp_2 = (j == 0) ? number : number - input_boards[j - 1][0];
          
            break;
          }
        }
        
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j<MAX_COL_LCD; j++)
        {
          if ((j < first_index_number_1) || (j > (first_index_number_1 + 3)))
            working_ekran[i][j] = name_string[index_language][j];
          else if (j == (first_index_number_1 + 0))
          {
            if (tmp_1 < 0) working_ekran[i][j] = '?';
            else if (tmp_1 > 0 ) working_ekran[i][j] = tmp_1 + 0x40;
          }
          else if (j == (first_index_number_1 + 1)) working_ekran[i][j] = '.';
          else     
          {
            if (tmp_2 < 0) working_ekran[i][j] = '?';
            //?else working_ekran[i][j] = tmp_2 + 0x30;
              else if ( (tmp_2 > 0) && (tmp_2 < 10))
              {
                working_ekran[i][j++] = tmp_2 + 0x30;
                working_ekran[i][j] = ' ';
              }
              else
              {
                  working_ekran[i][j++] = (tmp_2/10) + 0x30; working_ekran[i][j] = (tmp_2%10) + 0x30;
              }
          }
        }
        vaga = 10; //максимальний ваговий коефіцієнт для величини допуску дискретного входу
        if (view == true) value = current_settings.dopusk_dv[index_of_ekran_tmp]; //у змінну value поміщаємо значення допуску дискретного входу
        else value = edition_settings.dopusk_dv[index_of_ekran_tmp];
        first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
      }
      else
      {
        //У парному номері рядку виводимо значення уставки
        for (unsigned int j = 0; j<MAX_COL_LCD; j++)
        {
          if (
              ((j < COL_DOPUSK_DV_BEGIN) ||  (j > COL_DOPUSK_DV_END )) &&
              (
               !((j >= (COL_DOPUSK_DV_END + 2)) && (j <= (COL_DOPUSK_DV_END + 3)))
              )    
             )working_ekran[i][j] = ' ';
          else if ((j >= (COL_DOPUSK_DV_END + 2)) && (j <= (COL_DOPUSK_DV_END + 3))) 
            working_ekran[i][j] = ms[index_language][j - (COL_DOPUSK_DV_END + 2)];
          else
            calc_int_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, view);
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
    current_ekran.position_cursor_x = COL_DOPUSK_DV_BEGIN;
    int last_position_cursor_x = COL_DOPUSK_DV_END;

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
//Формуємо екран відображення значення типу входу для УВВ
/*****************************************************/
void make_ekran_type_input_uvv(unsigned int type_input_or_signal)
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_COL_LCD] = 
  {
    "    ДВx.        ",
    "    ДВx.        ",
    "     BI.        ",
    "    ДВx.        "
  };
  static const unsigned int first_index_number[MAX_NAMBER_LANGUAGE] = {8, 8, 8, 8};
  int index_language = index_language_in_array(current_settings.language);
  unsigned int first_index_number_1 = first_index_number[index_language];
  
  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  
  //Множення на два величини position_temp потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
  index_of_ekran = ((position_temp<<1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    if (index_of_ekran < (NUMBER_INPUTS<<1))//Множення на два константи NUMBER_INPUTS потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
    {
      if ((i & 0x1) == 0)
      {
        unsigned int number = (index_of_ekran >> 1) + 1;
        int tmp_1 = -1, tmp_2 = -1;
        
        for (size_t j = 0; j < N_INPUT_BOARDS; j++)
        {
          if (number <= input_boards[j][0])
          {
            tmp_1 = input_boards[j][1];
            tmp_2 = (j == 0) ? number : number - input_boards[j - 1][0];
          
            break;
          }
        }
        
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j<MAX_COL_LCD; j++)
        {
          if ((j < first_index_number_1) || (j > (first_index_number_1 + 2)))
            working_ekran[i][j] = name_string[index_language][j];
          else if (j == (first_index_number_1 + 0))
          {
            if (tmp_1 < 0) working_ekran[i][j] = '?';
            else if (tmp_1 > 0 ) working_ekran[i][j] = tmp_1 + 0x40;
          }
          else if (j == (first_index_number_1 + 1)) working_ekran[i][j] = '.';
          else     
          {
            if (tmp_2 < 0) working_ekran[i][j] = '?';
            //else working_ekran[i][j] = tmp_2 + 0x30;
              else if ( (tmp_2 > 0) && (tmp_2 < 10))
              {
                working_ekran[i][j++] = tmp_2 + 0x30;
                working_ekran[i][j] = ' ';
              }
              else
              {
                  working_ekran[i][j++] = (tmp_2/10) + 0x30; working_ekran[i][j] = (tmp_2%10) + 0x30;
              }
          }
        }
      }
      else
      {
        //У парному номері рядку виводимо значення 
        unsigned int index_ctr = (index_of_ekran>>1);
        unsigned int temp_data;
          
        if (type_input_or_signal == 0)
        {
          static const unsigned char information[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] = 
          {
            {"     ПРЯМОЙ     ", "   ИНВЕРСНЫЙ    "},
            {"     ПРЯМИЙ     ", "   ІНВЕРСНИЙ    "},
            {"     Direct     ", "    Inverse     "},
            {"     ПРЯМОЙ     ", "   ИНВЕРСНЫЙ    "}
          };
          static const unsigned int cursor_x[MAX_NAMBER_LANGUAGE][2] = 
          {
            {4, 2},
            {4, 2},
            {4, 3},
            {4, 2}
          };

          if(current_ekran.edition == 0) temp_data = current_settings.type_of_input;
          else temp_data = edition_settings.type_of_input;
          
          for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = information[index_language][(temp_data >> index_ctr) & 0x1][j];
          if (position_temp == index_ctr) current_ekran.position_cursor_x = cursor_x[index_language][(temp_data >> index_ctr) & 0x1];
        }
        else
        {
          static const unsigned char information[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] = 
          {
            {"   ПОСТОЯННЫЙ   ", "   ПЕРЕМЕННЫЙ   "},
            {"   ПОСТІЙНИЙ    ", "    ЗМІННИЙ     "},
            {"     Direct     ", "   Alternate    "},
            {"   ПОСТОЯННЫЙ   ", "   ПЕРЕМЕННЫЙ   "}
          };
          static const unsigned int cursor_x[MAX_NAMBER_LANGUAGE][2] = 
          {
            {2, 2},
            {2, 3},
            {4, 2},
            {2, 2}
          };

          if(current_ekran.edition == 0) temp_data = current_settings.type_of_input_signal;
          else temp_data = edition_settings.type_of_input_signal;
        
          for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = information[index_language][(temp_data >> index_ctr) & 0x1][j];
          if (position_temp == index_ctr) current_ekran.position_cursor_x = cursor_x[index_language][(temp_data >> index_ctr) & 0x1];
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
//Корекція допуску ДВ, коли змінюється тип вхідного сигналу
/*****************************************************/
void correct_dopusk_dv_when_type_of_input_signal_is_changed(void)
{
  //Корекція значень таблиці робочих настройок (допусків) з таблиці редагування (типу вхідного сигнал)
  for(unsigned int i = 0; i < NUMBER_INPUTS; i++)
  {
    if ((edition_settings.type_of_input_signal & (1 << i)) !=0)
    {
      if ((current_settings.dopusk_dv[i] % 10) != 0)
      {
        current_settings.dopusk_dv[i] = (current_settings.dopusk_dv[i] / 10)*10;
      }

      if (current_settings.dopusk_dv[i] < KOEF_DOPUSK_DV_ZMIN_MIN)
        current_settings.dopusk_dv[i] = KOEF_DOPUSK_DV_ZMIN_MIN;
    }
  }
}
/*****************************************************/

/*****************************************************/
//Формуємо екран відображення типу виходів
/*****************************************************/
void make_ekran_type_output_uvv(void)
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_COL_LCD] = 
  {
    "    ДВых.       ",
    "    ДВих.       ",
    "     BO.        ",
    "    ДВых.       "
  };
  static const unsigned int first_index_number[MAX_NAMBER_LANGUAGE] = {9, 9, 8, 9};
  int index_language = index_language_in_array(current_settings.language);
  unsigned int first_index_number_1 = first_index_number[index_language];
  
  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  
  //Множення на два величини position_temp потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
  index_of_ekran = ((position_temp<<1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;
  
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    if (index_of_ekran < (NUMBER_SIMPLE_OUTPUTS<<1))//Множення на два константи NUMBER_SIMPLE_OUTPUTS потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
    {
      if ((i & 0x1) == 0)
      {
        unsigned int number = (index_of_ekran >> 1) + 1;
        int tmp_1 = -1, tmp_2 = -1;

        for (size_t j = 0; j < N_OUTPUT_BOARDS; j++)
        {
          if (number <= output_boards[j][0])
          {
            tmp_1 = output_boards[j][1];
            tmp_2 = (j == 0) ? number : number - output_boards[j - 1][0];
          
            break;
          }
        }
        
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j<MAX_COL_LCD; j++)
        {
          if ((j < first_index_number_1) || (j > (first_index_number_1 + 2)))
            working_ekran[i][j] = name_string[index_language][j];
          else if (j == (first_index_number_1 + 0))
          {
            if (tmp_1 < 0) working_ekran[i][j] = '?';
            else if (tmp_1 > 0 ) working_ekran[i][j] = tmp_1 + 0x40;
          }
          else if (j == (first_index_number_1 + 1)) working_ekran[i][j] = '.';
          else     
          {
            if (tmp_2 < 0) working_ekran[i][j] = '?';
            //else working_ekran[i][j] = tmp_2 + 0x30;
              else if ( (tmp_2 > 0) && (tmp_2 < 10))
              {
                working_ekran[i][j++] = tmp_2 + 0x30;
                working_ekran[i][j] = ' ';
              }
              else
              {
                  working_ekran[i][j++] = (tmp_2/10) + 0x30; working_ekran[i][j] = (tmp_2%10) + 0x30;
              }
          }
        }
      }
      else
      {
        //У парному номері рядку виводимо значення 
        unsigned int index_ctr = (index_of_ekran>>1);

        static const unsigned char information[MAX_NAMBER_LANGUAGE][4][MAX_COL_LCD] = 
        {
          {"   КОМАНДНЫЙ    ", "   СИГНАЛЬНЫЙ   ", " СИГНАЛЬНЫЙ ИМП ", "  ОШИБКА ДАНЫХ  "},
          {"   КОМАНДНИЙ    ", "   СИГНАЛЬНИЙ   ", " СИГНАЛЬНИЙ ІМП ", "  ПОМИЛКА ДАНИХ "},
          {"     Follow     ", "    Latched     ", "  Latched-Imp   ", "   Data Error   "},
          {"   КОМАНДНЫЙ    ", "   СИГНАЛЬНЫЙ   ", " СИГНАЛЬНЫЙ ИМП ", "  ОШИБКА ДАНЫХ  "}
        };
        static const unsigned int cursor_x[MAX_NAMBER_LANGUAGE][4] = 
        {
          {2, 2, 0, 1},
          {2, 2, 0, 1},
          {4, 3, 1, 2},
          {2, 2, 0, 1}
        };

        unsigned int *p_temp_data, *p_temp_data_modif;
        if(current_ekran.edition == 0) 
        {
          p_temp_data       = &current_settings.type_of_output;
          p_temp_data_modif = &current_settings.type_of_output_modif;
        }
        else
        {
          p_temp_data       = &edition_settings.type_of_output;
          p_temp_data_modif = &edition_settings.type_of_output_modif;
        }

        unsigned int maska = (1 << index_ctr);
        int value = ((*p_temp_data & maska) != 0);
        if (value == true) value += ((*p_temp_data_modif & maska) != 0); //тільки у випадку, коли вихід сигнальний
        
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = information[index_language][value][j];
        if (position_temp == index_ctr) current_ekran.position_cursor_x = cursor_x[index_language][value];
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
//Формуємо екран відображення типу світлоіндикаторів
/*****************************************************/
void make_ekran_type_led_uvv(void)
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_COL_LCD] = 
  {
    "      Cв.       ",
    "      Cв.       ",
    "      LED       ",
    "      Cв.       "
  };
  static const unsigned int first_index_number[MAX_NAMBER_LANGUAGE] = {9, 9, 9, 9};
  int index_language = index_language_in_array(current_settings.language);
  unsigned int first_index_number_1 = first_index_number[index_language];
  
  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  
  //Множення на два величини position_temp потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
  index_of_ekran = ((position_temp<<1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;
  
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    if (index_of_ekran < (NUMBER_LEDS<<1))//Множення на два константи NUMBER_LEDS потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
    {
      if ((i & 0x1) == 0)
      {
        unsigned int number = (index_of_ekran >> 1) + 1;
        unsigned int tmp_1 = (number / 10), tmp_2 = number - tmp_1*10;
        
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j<MAX_COL_LCD; j++)
        {
          if (number > (NUMBER_LEDS - 2))
          {
            static const unsigned char leds_fix[2][MAX_NAMBER_LANGUAGE][MAX_COL_LCD] = 
            {
              {
                "    Cв-Start    ",
                "    Cв-Start    ",
                "    LED-Start   ",
                "    Cв-Start    "
              },
              {
                "    Cв-Trip     ",
                "    Cв-Trip     ",
                "    LED-Trip    ",
                "    Cв-Trip     "
              }
            };
            working_ekran[i][j] = leds_fix[(number == NUMBER_LEDS)][index_language][j];
          }
          else if ((j < first_index_number_1) || (j > (first_index_number_1 + 1)))
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
        unsigned int index_ctr = (index_of_ekran>>1);
        unsigned int temp_data;

        static const unsigned char information[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] = 
        {
          {"   НОРМАЛЬНЫЙ   ", "   ТРИГГЕРНЫЙ   "},
          {"   НОРМАЛЬНИЙ   ", "   ТРИҐЕРНИЙ    "},
          {"     Follow     ", "    Latched     "},
          {"   НОРМАЛЬНЫЙ   ", "   ТРИГГЕРНЫЙ   "}
        };
        static const unsigned int cursor_x[MAX_NAMBER_LANGUAGE][2] = 
        {
          {2, 2},
          {2, 2},
          {4, 3},
          {2, 2}
        };

        if(current_ekran.edition == 0) temp_data = current_settings.type_of_led;
        else temp_data = edition_settings.type_of_led;
        
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = information[index_language][(temp_data >> index_ctr) & 0x1][j];
        if (position_temp == index_ctr) current_ekran.position_cursor_x = cursor_x[index_language][(temp_data >> index_ctr) & 0x1];
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
//Формуємо екран відображення режиму ФК
/*****************************************************/
void make_ekran_type_button_uvv(void)
{
  static const unsigned char name_string[MAX_COL_LCD] = "       F        ";
  static const unsigned int first_index_number = 8;
  
  int index_language = index_language_in_array(current_settings.language);
  
  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  
  //Множення на два величини position_temp потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
  index_of_ekran = ((position_temp<<1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;
  
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    if (index_of_ekran < (NUMBER_DEFINED_BUTTONS << 1))//Множення на два константи NUMBER_BUTTON потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j<MAX_COL_LCD; j++)
        {
          if (j != first_index_number) working_ekran[i][j] = name_string[j];
          else working_ekran[i][j] = ((index_of_ekran >> 1) + 1) + 0x30;
        }
      }
      else
      {
        //У парному номері рядку виводимо значення 
        unsigned int index_ctr = (index_of_ekran>>1);

        static const unsigned char information[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] = 
        {
          {"     КНОПКА     ", "      КЛЮЧ      "},
          {"     КНОПКА     ", "      КЛЮЧ      "},
          {"      KEY       ", "     SWITCH     "},
          {"     КНОПКА     ", "      КЛЮЧ      "}
        };
        static const unsigned int cursor_x[MAX_NAMBER_LANGUAGE][2] = 
        {
          {4, 5},
          {4, 5},
          {5, 4},
          {4, 5}
        };

        uint32_t *p_temp_data;
        if(current_ekran.edition == 0) 
        {
          p_temp_data = &current_settings.buttons_mode;
        }
        else
        {
          p_temp_data = &edition_settings.buttons_mode;
        }

        int value = (*p_temp_data >> index_ctr) & 0x1;
        
        for (unsigned int j = 0; j < MAX_COL_LCD; j++) working_ekran[i][j] = information[index_language][value][j];
        if (position_temp == index_ctr) current_ekran.position_cursor_x = cursor_x[index_language][value];
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
