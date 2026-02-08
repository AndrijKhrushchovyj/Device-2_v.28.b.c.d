#include "header.h"

/*****************************************************/
//Формуємо екран вибору ДВ/ДВих/Св для ранжування
/* 
-------------------------------------------------------
Вхідний параметр
0  - входи
1  - виходи
2  - світодіоди
3  - опреділювальні функції
4  - функціональні кнопки
5  - опреділювальні триґери
6  - визначувані "І"
7  - визначувані "АБО"
8  - визначувані "Викл.АБО"
9  - визначувані "НЕ"
10 - Передавальні функції
11 - Вхідний GOOSE блок
12 - Вхідний MMS блок
13 - Вихідний мережевий блок
-------------------------------------------------------
*/
/*****************************************************/
void make_ekran_chose_of_list_for_ranguvannja(__id_input_output type_of_window)
{
  static const unsigned char information[MAX_NAMBER_LANGUAGE][_MAX_ID_INPUT_OUPUT][MAX_COL_LCD] =
    {
      {" ДВх.           ",
       " ДВых.          ",
       " Св             ",
       " О-функция      ",
       " F              ",
       " О-триггер      ",
       " О-И            ",
       " О-ИЛИ          ",
       " О-Искл.ИЛИ     ",
       " О-НЕ           ",
       " Пер.ф.         "
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
       ,
       " Вх.GOOSE Блок  ",
       " Вх.MMS Блок    ",
       " ВМБ            "
#endif
      },
      {" ДВх.           ",
       " ДВих.          ",
       " Св             ",
       " В-функція      ",
       " F              ",
       " В-триґер       ",
       " В-І            ",
       " В-АБО          ",
       " В-Викл.АБО     ",
       " В-НЕ           ",
       " Пер.ф.         "
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
       ,
       " Вх.GOOSE Блок  ",
       " Вх.MMS Блок    ",
       " ВМБ            "
#endif
      },
      {" BI.            ",
       " BO.            ",
       " LED            ",
       " UD-Func        ",
       " F              ",
       " UD-FF          ",
       " AND            ",
       " OR             ",
       " XOR            ",
       " NOT            ",
       " Transfer func  "
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
       ,
       " GSERECEIV      ",
       " MMSRECEIV      ",
       " NETSEND        "
#endif
      },
      {" ДВх.           ",
       " ДВых.          ",
       " Св             ",
       " О-функция      ",
       " F              ",
       " О-триггер      ",
       " О-И            ",
       " О-ИЛИ          ",
       " О-Искл.ИЛИ     ",
       " О-НЕ           ",
       " Пер.ф.         "
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
       ,
       " Вх.GOOSE Блок  ",
       " Вх.MMS Блок    ",
       " ВМБ            "
#endif
      }};

  static const unsigned int first_index_number[MAX_NAMBER_LANGUAGE][_MAX_ID_INPUT_OUPUT] =
    {
      {5, 6, 3, 11, 2, 11, 5, 7, 12, 6, 7
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
       ,
       14, 13, 5
#endif
      },
      {5, 6, 3, 11, 2, 10, 5, 7, 12, 6, 7
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
       ,
       14, 13, 5
#endif
      },
      {4, 4, 4, 9, 2, 7, 5, 4, 5, 5, 14
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
       ,
       11, 11, 9
#endif
      },
      {5, 6, 3, 11, 2, 11, 5, 7, 12, 6, 7
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
       ,
       14, 12, 5
#endif
      }};

  const unsigned int max_row[_MAX_ID_INPUT_OUPUT] =
  {
    MAX_ROW_LIST_INPUTS_FOR_RANGUVANNJA,
    MAX_ROW_LIST_OUTPUTS_FOR_RANGUVANNJA,
    MAX_ROW_LIST_LEDS_FOR_RANGUVANNJA,
    MAX_ROW_FOR_LIST_DF,
    MAX_ROW_LIST_BUTTONS_FOR_RANGUVANNJA,
    MAX_ROW_FOR_LIST_DT,
    MAX_ROW_FOR_LIST_D_AND,
    MAX_ROW_FOR_LIST_D_OR,
    MAX_ROW_FOR_LIST_D_XOR,
    MAX_ROW_FOR_LIST_D_NOT,
    MAX_ROW_FOR_LIST_TF

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
    ,
    N_IN_GOOSE,
    N_IN_MMS,
    N_OUT_LAN
#endif
  };
  int index_language = index_language_in_array(current_settings.language);
  unsigned int first_index_number_1 = first_index_number[index_language][type_of_window];

  unsigned int const position_temp = current_ekran.index_position;
  unsigned int index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  //Формуємо рядки  у робочий екран
  for (size_t i = 0; i < MAX_ROW_LCD; ++i)
  {
    //Наступні рядки треба перевірити, чи їх требе відображати у текучій кофігурації
    if (index_of_ekran < max_row[type_of_window])
    {
      unsigned int number = index_of_ekran + 1;

#ifdef NUMBER_DS
      if ((type_of_window == ID_OUTPUT) && (number > NUMBER_SIMPLE_OUTPUTS))
      {
        static unsigned char const ds_name[MAX_NAMBER_LANGUAGE][MAX_COL_LCD] =
          {
            " ДШ             ",
            " ДШ             ",
            " DSh            ",
            " ДШ             "};
        for (size_t j = 0; j != MAX_COL_LCD; ++j)
        {
          working_ekran[i][j] = ds_name[index_language][j];
        }
      }
      else
#endif
      {
        int tmp_1 = -1, tmp_2 = -1;

        if (type_of_window == ID_INPUT)
        {
          for (size_t j = 0; j < N_INPUT_BOARDS; j++)
          {
            if (number <= input_boards[j][0])
            {
              tmp_1 = input_boards[j][1];
              tmp_2 = (j == 0) ? number : number - input_boards[j - 1][0];

              break;
            }
          }
        }
        else if (type_of_window == ID_OUTPUT)
        {
          for (size_t j = 0; j < N_OUTPUT_BOARDS; j++)
          {
            if (number <= output_boards[j][0])
            {
              tmp_1 = output_boards[j][1];
              tmp_2 = (j == 0) ? number : number - output_boards[j - 1][0];

              break;
            }
          }
        }
        else
        {
          tmp_1 = (number / 10);
          tmp_2 = number - tmp_1 * 10;
        }

        for (size_t j = 0; j < MAX_COL_LCD; ++j)
        {
          if (
            (type_of_window == ID_LED) &&
            (number > (NUMBER_LEDS - 2)) &&
            (j >= first_index_number_1) &&
            (j < (first_index_number_1 + 6)))
          {
            char const leds_fix[2][6] =
              {
                "-Start",
                "-Trip "};
            working_ekran[i][j] = leds_fix[(number == NUMBER_LEDS)][j - first_index_number_1];
          }
          else if ((j < first_index_number_1) || (j >= (first_index_number_1 + 4)))
            working_ekran[i][j] = information[index_language][type_of_window][j];
          else
          {
            if (j == (first_index_number_1 + 0))
            {
              if (tmp_1 < 0)
                working_ekran[i][j] = '?';
              else if (tmp_1 > 0)
              {
                if (
                  (type_of_window != ID_INPUT) &&
                  (type_of_window != ID_OUTPUT))
                {
                  working_ekran[i][j] = tmp_1 + 0x30;
                }
                else
                {
                  working_ekran[i][j] = tmp_1 + 0x40;
                }
              }
            }
            else if (
              (j == (first_index_number_1 + 1)) &&
              ((type_of_window == ID_INPUT) ||
               (type_of_window == ID_OUTPUT)))
            {
              working_ekran[i][j] = '.';
            }
            else if (
              (j == (first_index_number_1 + 1))
              /*попередня перевірка вже перевірила, що якщо (j == (first_index_number_1 + 1)), то type_of_window != ID_INPUT і ID_OUTPUT*/
              ||
              ((j == (first_index_number_1 + 2)) &&
               ((type_of_window == ID_INPUT) ||
                (type_of_window == ID_OUTPUT))))
            {
              if (
                (tmp_1 > 0) ||
                (tmp_1 < 0))
              {
                if (tmp_2 < 0)
                  working_ekran[i][j] = '?';
                //else working_ekran[i][j] = tmp_2 + 0x30;

                else if ((tmp_2 > 0) && (tmp_2 < 10))
                {
                  working_ekran[i][j++] = tmp_2 + 0x30;
                  working_ekran[i][j] = ' ';
                }
                else
                {
                  if (
                    (type_of_window != ID_INPUT) &&
                    (type_of_window != ID_OUTPUT))
                  {
                    working_ekran[i][j] = tmp_2 + 0x30;
                  }
                  else
                  {
                    working_ekran[i][j++] = (tmp_2 / 10) + 0x30;
                    working_ekran[i][j] = (tmp_2 % 10) + 0x30;
                  }
                }
              }
              else
              {
                if (tmp_2 < 0)
                  working_ekran[i][j] = '?';
                else
                {
                  working_ekran[i][j - 1] = tmp_2 + 0x30;
                  working_ekran[i][j] = ' ';
                }
              }
            }
            else
              working_ekran[i][j] = ' ';
          }
        }
      }
    }
    else
    {
      for (unsigned int j = 0; j < MAX_COL_LCD; j++)
        working_ekran[i][j] = ' ';
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
//Формуємо екран відображення зранжованих сигналів на вибраний вхід
/*****************************************************/
void make_ekran_set_function_in_bi(unsigned int number_ekran, unsigned int type_ekran, uint32_t temp_state[]
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                                   ,
                                   int type_LN, int n_LN
#endif
)
{
#define NUMBER_ROW_FOR_NOTHING_INFORMATION 2

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][NUMBER_ROW_FOR_NOTHING_INFORMATION + NUMBER_TOTAL_SIGNAL_FOR_RANG_SMALL + (1 - N_IN_GOOSE) + (1 - N_IN_MMS) + (1 - N_OUT_LAN) + (1 - NUMBER_UP_SIGNAL_FOR_RANG_SMALL)][MAX_COL_LCD] =
#else
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][NUMBER_ROW_FOR_NOTHING_INFORMATION + NUMBER_TOTAL_SIGNAL_FOR_RANG_SMALL + (1 - NUMBER_UP_SIGNAL_FOR_RANG_SMALL)][MAX_COL_LCD] =
#endif
    {
      {"      Нет       ",
       "  ранжирования  ",
       NAME_RANG_SMALL_RU},
      {"      Нема      ",
       "   ранжування   ",
       NAME_RANG_SMALL_UA},
      {"       No       ",
       "  signal conf   ",
       NAME_RANG_SMALL_EN},
      {"      Нет       ",
       "  ранжирования  ",
       NAME_RANG_SMALL_KZ}};

  static const uint32_t index_number_UP_small[MAX_NAMBER_LANGUAGE][1] =
    {
      {11},
      {11},
      {6},
      {11}};

  unsigned char name_string_tmp[NUMBER_ROW_FOR_NOTHING_INFORMATION + NUMBER_TOTAL_SIGNAL_FOR_RANG_SMALL][MAX_COL_LCD];

  int index_language = index_language_in_array(current_settings.language);
  for (size_t index_1 = 0; index_1 < (NUMBER_ROW_FOR_NOTHING_INFORMATION + NUMBER_TOTAL_SIGNAL_FOR_RANG_SMALL); index_1++)
  {
    size_t index_row;
    if (index_1 < (NUMBER_ROW_FOR_NOTHING_INFORMATION + NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL))
    {
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
      if (index_1 < (NUMBER_ROW_FOR_NOTHING_INFORMATION + NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL - (N_IN_GOOSE + N_IN_MMS + N_OUT_LAN)))
      {
        index_row = index_1;
      }
      else if (index_1 < (NUMBER_ROW_FOR_NOTHING_INFORMATION + NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL - (N_IN_MMS + N_OUT_LAN)))
      {
        index_row = NUMBER_ROW_FOR_NOTHING_INFORMATION + RANG_SMALL_BLOCK_IN_GOOSE1 + ((index_1 - (NUMBER_ROW_FOR_NOTHING_INFORMATION + NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL - (N_IN_GOOSE + N_IN_MMS + N_OUT_LAN))) % 1);
      }
      else if (index_1 < (NUMBER_ROW_FOR_NOTHING_INFORMATION + NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL - (N_OUT_LAN)))
      {
        index_row = NUMBER_ROW_FOR_NOTHING_INFORMATION + RANG_SMALL_BLOCK_IN_MMS1 + (1 - N_IN_GOOSE) + ((index_1 - (NUMBER_ROW_FOR_NOTHING_INFORMATION + NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL - (N_IN_MMS + N_OUT_LAN))) % 1);
      }
      else
      {
        index_row = NUMBER_ROW_FOR_NOTHING_INFORMATION + RANG_SMALL_BLOCK_OUT_LAN1 + (1 - N_IN_GOOSE) + (1 - N_IN_MMS) + ((index_1 - (NUMBER_ROW_FOR_NOTHING_INFORMATION + NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL - (N_OUT_LAN))) % 1);
      }
#else
      index_row = index_1;
#endif
    }
    else if (index_1 < (NUMBER_ROW_FOR_NOTHING_INFORMATION + RANG_SMALL_BLOCK_UP1))
    {
      index_row = index_1
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                  + 1 - N_IN_GOOSE + 1 - N_IN_MMS + 1 - N_OUT_LAN
#endif
        ;
    }
    else if (index_1 < (NUMBER_ROW_FOR_NOTHING_INFORMATION + RANG_SMALL_BLOCK_UP1 + NUMBER_UP_SIGNAL_FOR_RANG_SMALL))
    {
      index_row = NUMBER_ROW_FOR_NOTHING_INFORMATION + RANG_SMALL_BLOCK_UP1
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                  + 1 - N_IN_GOOSE + 1 - N_IN_MMS + 1 - N_OUT_LAN
#endif
                  + ((index_1 - (NUMBER_ROW_FOR_NOTHING_INFORMATION + RANG_SMALL_BLOCK_UP1)) % 1);
    }
    else
    {
      index_row = index_1
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                  + 1 - N_IN_GOOSE + 1 - N_IN_MMS + 1 - N_OUT_LAN
#endif
                  + 1 - NUMBER_UP_SIGNAL_FOR_RANG_SMALL;
    }

    for (size_t index_2 = 0; index_2 < MAX_COL_LCD; index_2++)
    {
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
      if (
        (index_1 >= (NUMBER_ROW_FOR_NOTHING_INFORMATION + NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL - (N_IN_GOOSE + N_IN_MMS + N_OUT_LAN))) &&
        (index_1 < (NUMBER_ROW_FOR_NOTHING_INFORMATION + NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL - (N_IN_MMS + N_OUT_LAN))) &&
        (index_2 >= index_n_In_GOOSE[index_language][(index_1 - (NUMBER_ROW_FOR_NOTHING_INFORMATION + RANG_SMALL_BLOCK_IN_GOOSE1)) % 1]) &&
        (index_2 <= (index_n_In_GOOSE[index_language][(index_1 - (NUMBER_ROW_FOR_NOTHING_INFORMATION + RANG_SMALL_BLOCK_IN_GOOSE1)) % 1] + 1)))
      {
        unsigned int n = index_1 - (NUMBER_ROW_FOR_NOTHING_INFORMATION + RANG_SMALL_BLOCK_IN_GOOSE1);
        if ((n + 1) < 10)
        {
          if (index_2 == index_n_In_GOOSE[index_language][n % 1])
            name_string_tmp[index_1][index_2] = 0x30 + (n + 1);
          else
            name_string_tmp[index_1][index_2] = ' ';
        }
        else
        {
          if (index_2 == index_n_In_GOOSE[index_language][n % 1])
            name_string_tmp[index_1][index_2] = 0x30 + (n / 1 + 1) / 10;
          else
            name_string_tmp[index_1][index_2] = 0x30 + (n / 1 + 1) % 10;
        }
      }
      else if (
        (index_1 >= (NUMBER_ROW_FOR_NOTHING_INFORMATION + NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL - (N_IN_MMS + N_OUT_LAN))) &&
        (index_1 < (NUMBER_ROW_FOR_NOTHING_INFORMATION + NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL - (N_OUT_LAN))) &&
        (index_2 == index_n_In_MMS[index_language][(index_1 - (NUMBER_ROW_FOR_NOTHING_INFORMATION + RANG_SMALL_BLOCK_IN_MMS1)) % 1]))
      {
        name_string_tmp[index_1][index_2] = 0x30 + ((index_1 - (NUMBER_ROW_FOR_NOTHING_INFORMATION + RANG_SMALL_BLOCK_IN_MMS1)) / 1 + 1);
      }
      else if (
        (index_1 >= (NUMBER_ROW_FOR_NOTHING_INFORMATION + NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL - (N_OUT_LAN))) &&
        (index_1 < (NUMBER_ROW_FOR_NOTHING_INFORMATION + NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL)) &&
        (index_2 == index_n_Out_LAN[index_language][(index_1 - (NUMBER_ROW_FOR_NOTHING_INFORMATION + RANG_SMALL_BLOCK_OUT_LAN1)) % 1]))
      {
        name_string_tmp[index_1][index_2] = 0x30 + ((index_1 - (NUMBER_ROW_FOR_NOTHING_INFORMATION + RANG_SMALL_BLOCK_OUT_LAN1)) / 1 + 1);
      }
      else
#endif
      {
        if (
          (index_1 >= (NUMBER_ROW_FOR_NOTHING_INFORMATION + RANG_SMALL_BLOCK_UP1)) &&
          (index_1 < (NUMBER_ROW_FOR_NOTHING_INFORMATION + RANG_SMALL_BLOCK_UP1 + NUMBER_UP_SIGNAL_FOR_RANG_SMALL)) &&
          (index_2 == index_number_UP_small[index_language][(index_1 - (NUMBER_ROW_FOR_NOTHING_INFORMATION + RANG_SMALL_BLOCK_UP1)) % 1]))
        {
          name_string_tmp[index_1][index_2] = 0x30 + ((index_1 - (NUMBER_ROW_FOR_NOTHING_INFORMATION + RANG_SMALL_BLOCK_UP1)) / 1 + 1);
        }
        else
          name_string_tmp[index_1][index_2] = name_string[index_language][index_row][index_2];
      }
    }
  }

  unsigned int state_viewing_input[N_SMALL];
  for (size_t i = 0; i < N_SMALL; i++)
    state_viewing_input[i] = temp_state[i];
  if (current_ekran.edition == 0)
  {
    //Випадок, коли ми продивляємося зранжовані функції на вході
    unsigned int comp = true;
    for (size_t i = 0; ((comp == true) && (i < N_SMALL)); ++i)
    {
      comp &= (state_viewing_input[i] == 0);
    }
    if (comp)
    {
      //Це означає, що на даний вхід нічого не відранжовано

      //Текучу позицію в сипску переводимо на сам початок
      current_ekran.index_position = 0;
      position_in_current_level_menu[number_ekran] = 0;

      //Копіюємо  рядки у робочий екран
      for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
      {
        //Копіюємо в робочий екран інформацію, що нічого не відранжовано
        if (i < NUMBER_ROW_FOR_NOTHING_INFORMATION)
          for (unsigned int j = 0; j < MAX_COL_LCD; j++)
            working_ekran[i][j] = name_string_tmp[i][j];
        else
          for (unsigned int j = 0; j < MAX_COL_LCD; j++)
            working_ekran[i][j] = ' ';
      }

      //Відображення курору по вертикалі
      current_ekran.position_cursor_y = 0;
      //Курсор невидимий
      current_ekran.cursor_on = 0;
    }
    else
    {
      /************************************************************/
      //Формуємо список із функцій, які реально відранжовані
      /************************************************************/
      unsigned int position_temp = current_ekran.index_position;
      unsigned int index_of_ekran;
      unsigned int i = 0, offset = 0;
      unsigned int state_current_bit;

      while ((i + offset) < NUMBER_TOTAL_SIGNAL_FOR_RANG_SMALL)
      {
        state_current_bit = state_viewing_input[(i + offset) >> 5] & (1 << ((i + offset) & 0x1f));

        if (state_current_bit == 0)
        {
          for (unsigned int j = i; j < (NUMBER_TOTAL_SIGNAL_FOR_RANG_SMALL - offset); j++)
          {
            if ((j + 1) < (NUMBER_TOTAL_SIGNAL_FOR_RANG_SMALL - offset))
            {
              for (unsigned int k = 0; k < MAX_COL_LCD; k++)
                name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION][k] = name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION + 1][k];
            }
            else
            {
              for (unsigned int k = 0; k < MAX_COL_LCD; k++)
                name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION][k] = ' ';
            }
          }
          if (current_ekran.index_position >= ((int) (i + offset)))
            position_temp--;
          offset++;

          //Робота з Watchdog
          watchdog_routine(UNITED_BITS_WATCHDOG, 29);
        }
        else
          i++;
      }
      /************************************************************/

      index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

      //Копіюємо  рядки у робочий екран
      for (i = 0; i < MAX_ROW_LCD; i++)
      {
        //Наступні рядки треба перевірити, чи їх требе відображати у текучій коффігурації
        if (index_of_ekran < NUMBER_TOTAL_SIGNAL_FOR_RANG_SMALL)
        {
          for (unsigned int j = 0; j < MAX_COL_LCD; j++)
            working_ekran[i][j] = name_string_tmp[index_of_ekran + NUMBER_ROW_FOR_NOTHING_INFORMATION][j];

          //Підтягуємо назву, щоб не було спереді багато пробілів
          unsigned int iteration = 0;
          while (
            (working_ekran[i][0] == ' ') &&
            (working_ekran[i][1] == ' ') &&
            (iteration < (MAX_COL_LCD - 1 - 1)))
          {
            for (unsigned int j = 1; j < MAX_COL_LCD; j++)
            {
              if ((j + 1) < MAX_COL_LCD)
                working_ekran[i][j] = working_ekran[i][j + 1];
              else
                working_ekran[i][j] = ' ';
            }
            iteration++;
          }
        }
        else
          for (unsigned int j = 0; j < MAX_COL_LCD; j++)
            working_ekran[i][j] = ' ';

        index_of_ekran++;
      }
      //Відображення курору по вертикалі
      current_ekran.position_cursor_y = position_temp & (MAX_ROW_LCD - 1);
      //Курсор видимий
      current_ekran.cursor_on = 1;
    }

    //Курсор по горизонталі відображається на першій позиції
    current_ekran.position_cursor_x = 0;
    //Курсор не мигає
    current_ekran.cursor_blinking_on = 0;
  }
  else
  {
    unsigned int position_temp = current_ekran.index_position;
    unsigned int index_of_ekran;
    unsigned int i, offset = 0;
    static const int min_max_number[_FIX_NUMBER_PROTECTION][2] =
      {
        {(NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL),
         (NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL + NUMBER_RPN_SIGNAL_FOR_RANG_SMALL - 1)},
        {(NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL + NUMBER_RPN_SIGNAL_FOR_RANG_SMALL),
         (NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL + NUMBER_RPN_SIGNAL_FOR_RANG_SMALL + NUMBER_SZKh_SIGNAL_FOR_RANG_SMALL - 1)},
        {-1, -1},
        {(NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL + NUMBER_RPN_SIGNAL_FOR_RANG_SMALL + NUMBER_SZKh_SIGNAL_FOR_RANG_SMALL + NUMBER_SNKh_SIGNAL_FOR_RANG_SMALL),
         (NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL + NUMBER_RPN_SIGNAL_FOR_RANG_SMALL + NUMBER_SZKh_SIGNAL_FOR_RANG_SMALL + NUMBER_SNKh_SIGNAL_FOR_RANG_SMALL + NUMBER_BRP_SIGNAL_FOR_RANG_SMALL - 1)},
        {(NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL + NUMBER_RPN_SIGNAL_FOR_RANG_SMALL + NUMBER_SZKh_SIGNAL_FOR_RANG_SMALL + NUMBER_SNKh_SIGNAL_FOR_RANG_SMALL + NUMBER_BRP_SIGNAL_FOR_RANG_SMALL),
         (NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL + NUMBER_RPN_SIGNAL_FOR_RANG_SMALL + NUMBER_SZKh_SIGNAL_FOR_RANG_SMALL + NUMBER_SNKh_SIGNAL_FOR_RANG_SMALL + NUMBER_BRP_SIGNAL_FOR_RANG_SMALL + NUMBER_UMAX_SIGNAL_FOR_RANG_SMALL - 1)},
        {(NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL + NUMBER_RPN_SIGNAL_FOR_RANG_SMALL + NUMBER_SZKh_SIGNAL_FOR_RANG_SMALL + NUMBER_SNKh_SIGNAL_FOR_RANG_SMALL + NUMBER_BRP_SIGNAL_FOR_RANG_SMALL + NUMBER_UMAX_SIGNAL_FOR_RANG_SMALL),
         (NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL + NUMBER_RPN_SIGNAL_FOR_RANG_SMALL + NUMBER_SZKh_SIGNAL_FOR_RANG_SMALL + NUMBER_SNKh_SIGNAL_FOR_RANG_SMALL + NUMBER_BRP_SIGNAL_FOR_RANG_SMALL + NUMBER_UMAX_SIGNAL_FOR_RANG_SMALL + NUMBER_UMIN_SIGNAL_FOR_RANG_SMALL - 1)},
        {(NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL + NUMBER_RPN_SIGNAL_FOR_RANG_SMALL + NUMBER_SZKh_SIGNAL_FOR_RANG_SMALL + NUMBER_SNKh_SIGNAL_FOR_RANG_SMALL + NUMBER_BRP_SIGNAL_FOR_RANG_SMALL + NUMBER_UMAX_SIGNAL_FOR_RANG_SMALL + NUMBER_UMIN_SIGNAL_FOR_RANG_SMALL),
         (NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL + NUMBER_RPN_SIGNAL_FOR_RANG_SMALL + NUMBER_SZKh_SIGNAL_FOR_RANG_SMALL + NUMBER_SNKh_SIGNAL_FOR_RANG_SMALL + NUMBER_BRP_SIGNAL_FOR_RANG_SMALL + NUMBER_UMAX_SIGNAL_FOR_RANG_SMALL + NUMBER_UMIN_SIGNAL_FOR_RANG_SMALL + NUMBER_UP_SIGNAL_FOR_RANG_SMALL - 1)},
        {(NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL + NUMBER_RPN_SIGNAL_FOR_RANG_SMALL + NUMBER_SZKh_SIGNAL_FOR_RANG_SMALL + NUMBER_SNKh_SIGNAL_FOR_RANG_SMALL + NUMBER_BRP_SIGNAL_FOR_RANG_SMALL + NUMBER_UMAX_SIGNAL_FOR_RANG_SMALL + NUMBER_UMIN_SIGNAL_FOR_RANG_SMALL + NUMBER_UP_SIGNAL_FOR_RANG_SMALL),
         (NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL + NUMBER_RPN_SIGNAL_FOR_RANG_SMALL + NUMBER_SZKh_SIGNAL_FOR_RANG_SMALL + NUMBER_SNKh_SIGNAL_FOR_RANG_SMALL + NUMBER_BRP_SIGNAL_FOR_RANG_SMALL + NUMBER_UMAX_SIGNAL_FOR_RANG_SMALL + NUMBER_UMIN_SIGNAL_FOR_RANG_SMALL + NUMBER_UP_SIGNAL_FOR_RANG_SMALL + NUMBER_EL_SIGNAL_FOR_RANG_SMALL - 1)}};

    /*************************************************************/
    //Фільтруємо сигнали, яких у даній конфігурації неприсутні
    /*************************************************************/
    if (
      (type_ekran == INDEX_VIEWING_INPUT) ||
      (type_ekran == INDEX_VIEWING_BUTTON)
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
      ||
      (type_ekran == INDEX_VIEWING_IEC61850_RANG)
#endif
    )
    {
      /*************************************************************/
      //У випадку, якщо відображення здійснюється вікна функціональних кнопок, Вх.GOOSE блоіків, або  Вх.MMS блоіків
      /*************************************************************/
      const uint32_t *p_array = NULL;
      if (type_ekran == INDEX_VIEWING_INPUT)
        p_array = binary_input_signals;
      else if (type_ekran == INDEX_VIEWING_BUTTON)
        p_array = buttons_mode[(current_settings.buttons_mode >> (number_ekran - EKRAN_RANGUVANNJA_BUTTON_1)) & 0x1];
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
      else if (type_ekran == INDEX_VIEWING_IEC61850_RANG)
      {
        if ((type_LN == INDEX_TYPE_IEC61850_IN_GOOSE) || (type_LN == INDEX_TYPE_IEC61850_IN_MMS))
          p_array = rang_iec61850_blocks[type_LN];
        else
          total_error_sw_fixed();
      }
#endif
      else
        total_error_sw_fixed();

      for (unsigned int index_deleted_function = 0; index_deleted_function < NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL; index_deleted_function++)
      {
        if (
          (_CHECK_SET_BIT(p_array, index_deleted_function) == 0)
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
          ||
          ((type_LN == INDEX_TYPE_IEC61850_IN_GOOSE) && (index_deleted_function == (RANG_SMALL_BLOCK_IN_GOOSE1 + n_LN))) ||
          ((type_LN == INDEX_TYPE_IEC61850_IN_MMS) && (index_deleted_function == (RANG_SMALL_BLOCK_IN_MMS1 + n_LN)))
#endif
        )
        {
          /*************************************************************/
          //Відкидаємо ім'я даної функції і зміщаємо біти
          /*************************************************************/

          //Формуємо маску біт, які не треба переміщати при переміщенні імен полів
          unsigned int maska[N_SMALL];
          for (size_t j = 0; j < N_SMALL; ++j)
            maska[j] = 0;
          for (unsigned int j = 0; j < (index_deleted_function - offset); j++)
            _SET_BIT(maska, j);

          /***/
          //Зміщуємо біти стану реанжування функцій разом із їх назвами
          /***/
          unsigned int new_temp_data_1[N_SMALL], new_temp_data_2[N_SMALL];

          for (unsigned int k = 0; k < N_SMALL; k++)
          {
            new_temp_data_1[k] = state_viewing_input[k] & maska[k];

            new_temp_data_2[k] = state_viewing_input[k] & (~maska[k]);
          }

          for (unsigned int k = 0; k < (N_SMALL - 1); k++)
          {
            new_temp_data_2[k] = ((new_temp_data_2[k] >> 1) | ((new_temp_data_2[k + 1] & 0x1) << 31)) & (~maska[k]);
          }
          new_temp_data_2[N_SMALL - 1] = (new_temp_data_2[N_SMALL - 1] >> 1) & (~maska[N_SMALL - 1]);

          for (unsigned int k = 0; k < N_SMALL; k++)
          {
            state_viewing_input[k] = new_temp_data_1[k] | new_temp_data_2[k];
          }
          /***/
          for (unsigned int j = (index_deleted_function - offset); j < (NUMBER_TOTAL_SIGNAL_FOR_RANG_SMALL - offset); j++)
          {
            if ((j + 1) < (NUMBER_TOTAL_SIGNAL_FOR_RANG_SMALL - offset))
            {
              for (unsigned int k = 0; k < MAX_COL_LCD; k++)
                name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION][k] = name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION + 1][k];
            }
            else
            {
              for (unsigned int k = 0; k < MAX_COL_LCD; k++)
                name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION][k] = ' ';
            }
          }
          if (current_ekran.index_position >= ((int) index_deleted_function))
            position_temp--;
          offset++;
          //Робота з Watchdog
          watchdog_routine(UNITED_BITS_WATCHDOG, 30);
          /*************************************************************/
        }
      }
      /*************************************************************/
    }

    //Функції загального призначення пропускаємо (вони знаходяться у початку списку), тому починаємо з першого захисту
    int index_in_list = NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL;

    for (i = 0; i < _FIX_NUMBER_PROTECTION; i++)
    {

      if ((current_settings.configuration & (1 << i)) != 0)
      {
        //Захист присутнійсть, тому функції фільтрувати не потрібно - переводимо індекс на наступні функції
        //Додаємо кількість функцій до поточного індексу, якщо для поточного захисту реально були присутні функції,
        //бо інкаше ми вже знаходимося на індексі наступного захисту
        if (min_max_number[i][0] >= 0)
        {
          if (
            (type_ekran == INDEX_VIEWING_INPUT) ||
            (type_ekran == INDEX_VIEWING_BUTTON)
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
            ||
            (type_ekran == INDEX_VIEWING_IEC61850_RANG)
#endif
          )
          {
            /*
            Випадок коли деякі сигнали треба відфільтрувати
            */
            const uint32_t *p_array = NULL;
            if (type_ekran == INDEX_VIEWING_INPUT)
              p_array = binary_input_signals;
            else if (type_ekran == INDEX_VIEWING_BUTTON)
              p_array = buttons_mode[(current_settings.buttons_mode >> (number_ekran - EKRAN_RANGUVANNJA_BUTTON_1)) & 0x1];
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
            else if (type_ekran == INDEX_VIEWING_IEC61850_RANG)
            {
              if ((type_LN == INDEX_TYPE_IEC61850_IN_GOOSE) || (type_LN == INDEX_TYPE_IEC61850_IN_MMS))
                p_array = rang_iec61850_blocks[type_LN];
              else
                total_error_sw_fixed();
            }
#endif
            else
              total_error_sw_fixed();

            //Відкидати імена функцій і зміщати біти треба тільки у тому випадку, якщо функції пристні у списку для ранжування для даного захисту
            //Формуємо маску біт, які не треба переміщати при переміщенні імен полів
            unsigned int maska[N_SMALL];
            for (size_t j1 = 0; j1 < N_SMALL; ++j1)
              maska[j1] = 0;
            unsigned int j1;
            for (j1 = 0; j1 < (min_max_number[i][0] - offset); j1++)
              _SET_BIT(maska, j1);

            //Відкидаємо назви функцій із списку, які є зайвими
            while (index_in_list <= min_max_number[i][1])
            {
              if (_CHECK_SET_BIT(p_array, index_in_list) == 0)
              {
                /***/
                //Зміщуємо біти стану реанжування функцій разом із їх назвами
                /***/
                unsigned int new_temp_data_1[N_SMALL], new_temp_data_2[N_SMALL];

                for (unsigned int k = 0; k < N_SMALL; k++)
                {
                  new_temp_data_1[k] = state_viewing_input[k] & maska[k];

                  new_temp_data_2[k] = state_viewing_input[k] & (~maska[k]);
                }

                for (unsigned int k = 0; k < (N_SMALL - 1); k++)
                {
                  new_temp_data_2[k] = ((new_temp_data_2[k] >> 1) | ((new_temp_data_2[k + 1] & 0x1) << 31)) & (~maska[k]);
                }
                new_temp_data_2[N_SMALL - 1] = (new_temp_data_2[N_SMALL - 1] >> 1) & (~maska[N_SMALL - 1]);

                for (unsigned int k = 0; k < N_SMALL; k++)
                {
                  state_viewing_input[k] = new_temp_data_1[k] | new_temp_data_2[k];
                }
                /***/
                for (unsigned int j = (index_in_list - offset); j < (NUMBER_TOTAL_SIGNAL_FOR_RANG_SMALL - offset); j++)
                {
                  if ((j + 1) < (NUMBER_TOTAL_SIGNAL_FOR_RANG_SMALL - offset))
                  {
                    for (unsigned int k = 0; k < MAX_COL_LCD; k++)
                      name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION][k] = name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION + 1][k];
                  }
                  else
                  {
                    for (unsigned int k = 0; k < MAX_COL_LCD; k++)
                      name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION][k] = ' ';
                  }
                }
                if (current_ekran.index_position >= index_in_list)
                  position_temp--;

                offset++;
                //Робота з Watchdog
                watchdog_routine(UNITED_BITS_WATCHDOG, 31);
              }
              else
              {
                _SET_BIT(maska, j1);
                j1++;
              }

              index_in_list++;
            }
          }
          else
            index_in_list += ((min_max_number[i][1] - min_max_number[i][0]) + 1);
        }
      }
      else if (min_max_number[i][0] >= 0)
      {
        //Відкидати імена функцій і зміщати біти треба тільки у тому випадку, якщо функції пристні у списку для ранжування для даного захисту
        //Формуємо маску біт, які не треба переміщати при переміщенні імен полів
        unsigned int maska[N_SMALL];
        for (size_t j = 0; j < N_SMALL; ++j)
          maska[j] = 0;
        for (unsigned int j = 0; j < (min_max_number[i][0] - offset); j++)
          _SET_BIT(maska, j);

        //Відкидаємо назви функцій із списку, які є зайвими
        while (index_in_list <= min_max_number[i][1])
        {
          /***/
          //Зміщуємо біти стану реанжування функцій разом із їх назвами
          /***/
          unsigned int new_temp_data_1[N_SMALL], new_temp_data_2[N_SMALL];

          for (unsigned int k = 0; k < N_SMALL; k++)
          {
            new_temp_data_1[k] = state_viewing_input[k] & maska[k];

            new_temp_data_2[k] = state_viewing_input[k] & (~maska[k]);
          }

          for (unsigned int k = 0; k < (N_SMALL - 1); k++)
          {
            new_temp_data_2[k] = ((new_temp_data_2[k] >> 1) | ((new_temp_data_2[k + 1] & 0x1) << 31)) & (~maska[k]);
          }
          new_temp_data_2[N_SMALL - 1] = (new_temp_data_2[N_SMALL - 1] >> 1) & (~maska[N_SMALL - 1]);

          for (unsigned int k = 0; k < N_SMALL; k++)
          {
            state_viewing_input[k] = new_temp_data_1[k] | new_temp_data_2[k];
          }
          /***/
          for (unsigned int j = (index_in_list - offset); j < (NUMBER_TOTAL_SIGNAL_FOR_RANG_SMALL - offset); j++)
          {
            if ((j + 1) < (NUMBER_TOTAL_SIGNAL_FOR_RANG_SMALL - offset))
            {
              for (unsigned int k = 0; k < MAX_COL_LCD; k++)
                name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION][k] = name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION + 1][k];
            }
            else
            {
              for (unsigned int k = 0; k < MAX_COL_LCD; k++)
                name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION][k] = ' ';
            }
          }
          if (current_ekran.index_position >= index_in_list)
            position_temp--;

          offset++;
          index_in_list++;
          //Робота з Watchdog
          watchdog_routine(UNITED_BITS_WATCHDOG, 32);
        }
      }

      //Робота з Watchdog
      watchdog_routine(UNITED_BITS_WATCHDOG, 33);
    }
    /*************************************************************/

    //Множення на два величини position_temp потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
    index_of_ekran = ((position_temp << 1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

    for (i = 0; i < MAX_ROW_LCD; i++)
    {
      if (index_of_ekran < ((NUMBER_TOTAL_SIGNAL_FOR_RANG_SMALL - offset) << 1)) //Множення на два константи  NUMBER_TOTAL_SIGNAL_FOR_RANG_SMALL потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
      {
        if ((i & 0x1) == 0)
        {
          //У непарному номері рядку виводимо заголовок
          for (unsigned int j = 0; j < MAX_COL_LCD; j++)
            working_ekran[i][j] = name_string_tmp[(index_of_ekran >> 1) + NUMBER_ROW_FOR_NOTHING_INFORMATION][j];
        }
        else
        {
          //У парному номері рядку виводимо стан функції
          static unsigned char const information[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] =
            {
              {"      ОТКЛ      ", "      ВКЛ       "},
              {"      ВИМК      ", "     УВІМК      "},
              {"      OFF       ", "       ON       "},
              {"      СЉНД      ", "      КОСУ      "}};
          unsigned int index_bit = index_of_ekran >> 1;

          for (unsigned int j = 0; j < MAX_COL_LCD; j++)
            working_ekran[i][j] = information[index_language][((state_viewing_input[index_bit >> 5] & (1 << (index_bit & 0x1f))) != 0)][j];
        }
      }
      else
        for (unsigned int j = 0; j < MAX_COL_LCD; j++)
          working_ekran[i][j] = ' ';

      index_of_ekran++;
    }

    static const unsigned int cursor_x[MAX_NAMBER_LANGUAGE][2] =
      {
        {5, 5},
        {5, 4},
        {5, 6},
        {5, 5}};

    //Відображення курору по вертикалі і курсор завжди має бути у полі із значенням
    current_ekran.position_cursor_x = cursor_x[index_language][((state_viewing_input[position_temp >> 5] & (1 << (position_temp & 0x1f))) != 0)];
    current_ekran.position_cursor_y = ((position_temp << 1) + 1) & (MAX_ROW_LCD - 1);

    //Курсор мигає
    current_ekran.cursor_blinking_on = 1;
    //Режим відображення у режимі редагування
  }

  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;

#undef NUMBER_ROW_FOR_NOTHING_INFORMATION
}
/*****************************************************/

/*****************************************************/
//Формуємо екран відображення зранжованих сигналів на вибраний вихід-світлоіндикаторів-о-функцій-реєстраторів
/*****************************************************/
void make_ekran_set_function_in_output_led_df_dt_reg(unsigned int number_ekran, unsigned int type_ekran, uint32_t temp_state[])
{
#define NUMBER_ROW_FOR_NOTHING_INFORMATION 2

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][NUMBER_ROW_FOR_NOTHING_INFORMATION + NUMBER_TOTAL_SIGNAL_FOR_RANG + (1 - N_IN_GOOSE) + (1 - N_IN_MMS) + (1 - N_OUT_LAN) + (3 - NUMBER_UP_SIGNAL_FOR_RANG)][MAX_COL_LCD] =
#else
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][NUMBER_ROW_FOR_NOTHING_INFORMATION + NUMBER_TOTAL_SIGNAL_FOR_RANG + (3 - NUMBER_UP_SIGNAL_FOR_RANG)][MAX_COL_LCD] =
#endif
    {
      {"      Нет       ",
       "  ранжирования  ",
       NAME_RANG_RU},
      {"      Нема      ",
       "   ранжування   ",
       NAME_RANG_UA},
      {"       No       ",
       "  signal conf   ",
       NAME_RANG_EN},
      {"      Нет       ",
       "  ранжирования  ",
       NAME_RANG_KZ}};
  unsigned char name_string_tmp[NUMBER_ROW_FOR_NOTHING_INFORMATION + NUMBER_TOTAL_SIGNAL_FOR_RANG][MAX_COL_LCD];

  int index_language = index_language_in_array(current_settings.language);
  for (int index_1 = 0; index_1 < (NUMBER_ROW_FOR_NOTHING_INFORMATION + NUMBER_TOTAL_SIGNAL_FOR_RANG); index_1++)
  {
    size_t index_row;
    if (index_1 < (NUMBER_ROW_FOR_NOTHING_INFORMATION + NUMBER_GENERAL_SIGNAL_FOR_RANG))
    {
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
      if (index_1 < (NUMBER_ROW_FOR_NOTHING_INFORMATION + NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_IN_GOOSE + N_IN_MMS + N_OUT_LAN)))
      {
        index_row = index_1;
      }
      else if (index_1 < (NUMBER_ROW_FOR_NOTHING_INFORMATION + NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_IN_MMS + N_OUT_LAN)))
      {
        index_row = NUMBER_ROW_FOR_NOTHING_INFORMATION + RANG_BLOCK_IN_GOOSE1 + ((index_1 - (NUMBER_ROW_FOR_NOTHING_INFORMATION + NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_IN_GOOSE + N_IN_MMS + N_OUT_LAN))) % 1);
      }
      else if (index_1 < (NUMBER_ROW_FOR_NOTHING_INFORMATION + NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_OUT_LAN)))
      {
        index_row = NUMBER_ROW_FOR_NOTHING_INFORMATION + RANG_BLOCK_IN_MMS1 + (1 - N_IN_GOOSE) + ((index_1 - (NUMBER_ROW_FOR_NOTHING_INFORMATION + NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_IN_MMS + N_OUT_LAN))) % 1);
      }
      else
      {
        index_row = NUMBER_ROW_FOR_NOTHING_INFORMATION + RANG_BLOCK_OUT_LAN1 + (1 - N_IN_GOOSE) + (1 - N_IN_MMS) + ((index_1 - (NUMBER_ROW_FOR_NOTHING_INFORMATION + NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_OUT_LAN))) % 1);
      }
#else
      index_row = index_1;
#endif
    }
    else if (index_1 < (NUMBER_ROW_FOR_NOTHING_INFORMATION + RANG_BLOCK_UP1))
    {
      index_row = index_1
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                  + 1 - N_IN_GOOSE + 1 - N_IN_MMS + 1 - N_OUT_LAN
#endif
        ;
    }
    else if (index_1 < (NUMBER_ROW_FOR_NOTHING_INFORMATION + RANG_BLOCK_UP1 + NUMBER_UP_SIGNAL_FOR_RANG))
    {
      index_row = NUMBER_ROW_FOR_NOTHING_INFORMATION + RANG_BLOCK_UP1
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                  + 1 - N_IN_GOOSE + 1 - N_IN_MMS + 1 - N_OUT_LAN
#endif
                  + ((index_1 - (NUMBER_ROW_FOR_NOTHING_INFORMATION + RANG_BLOCK_UP1)) % 3);
    }
    else
    {
      index_row = index_1
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                  + 1 - N_IN_GOOSE + 1 - N_IN_MMS + 1 - N_OUT_LAN
#endif
                  + 3 - NUMBER_UP_SIGNAL_FOR_RANG;
    }

    for (size_t index_2 = 0; index_2 < MAX_COL_LCD; index_2++)
    {
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
      if (
        (index_1 >= (NUMBER_ROW_FOR_NOTHING_INFORMATION + NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_IN_GOOSE + N_IN_MMS + N_OUT_LAN))) &&
        (index_1 < (NUMBER_ROW_FOR_NOTHING_INFORMATION + NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_IN_MMS + N_OUT_LAN))) &&
        (index_2 >= index_n_In_GOOSE[index_language][(index_1 - (NUMBER_ROW_FOR_NOTHING_INFORMATION + RANG_BLOCK_IN_GOOSE1)) % 1]) &&
        (index_2 <= (index_n_In_GOOSE[index_language][(index_1 - (NUMBER_ROW_FOR_NOTHING_INFORMATION + RANG_BLOCK_IN_GOOSE1)) % 1] + 1)))
      {
        unsigned int n = index_1 - (NUMBER_ROW_FOR_NOTHING_INFORMATION + RANG_BLOCK_IN_GOOSE1);
        if ((n + 1) < 10)
        {
          if (index_2 == index_n_In_GOOSE[index_language][n % 1])
            name_string_tmp[index_1][index_2] = 0x30 + (n + 1);
          else
            name_string_tmp[index_1][index_2] = ' ';
        }
        else
        {
          if (index_2 == index_n_In_GOOSE[index_language][n % 1])
            name_string_tmp[index_1][index_2] = 0x30 + (n / 1 + 1) / 10;
          else
            name_string_tmp[index_1][index_2] = 0x30 + (n / 1 + 1) % 10;
        }
      }
      else if (
        (index_1 >= (NUMBER_ROW_FOR_NOTHING_INFORMATION + NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_IN_MMS + N_OUT_LAN))) &&
        (index_1 < (NUMBER_ROW_FOR_NOTHING_INFORMATION + NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_OUT_LAN))) &&
        (index_2 == index_n_In_MMS[index_language][(index_1 - (NUMBER_ROW_FOR_NOTHING_INFORMATION + RANG_BLOCK_IN_MMS1)) % 1]))
      {
        name_string_tmp[index_1][index_2] = 0x30 + ((index_1 - (NUMBER_ROW_FOR_NOTHING_INFORMATION + RANG_BLOCK_IN_MMS1)) / 1 + 1);
      }
      else if (
        (index_1 >= (NUMBER_ROW_FOR_NOTHING_INFORMATION + NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_OUT_LAN))) &&
        (index_1 < (NUMBER_ROW_FOR_NOTHING_INFORMATION + NUMBER_GENERAL_SIGNAL_FOR_RANG)) &&
        (index_2 == index_n_Out_LAN[index_language][(index_1 - (NUMBER_ROW_FOR_NOTHING_INFORMATION + RANG_BLOCK_OUT_LAN1)) % 1]))
      {
        name_string_tmp[index_1][index_2] = 0x30 + ((index_1 - (NUMBER_ROW_FOR_NOTHING_INFORMATION + RANG_BLOCK_OUT_LAN1)) / 1 + 1);
      }
      else
#endif
      {
        if (
          (index_1 >= (NUMBER_ROW_FOR_NOTHING_INFORMATION + RANG_BLOCK_UP1)) &&
          (index_1 < (NUMBER_ROW_FOR_NOTHING_INFORMATION + RANG_BLOCK_UP1 + NUMBER_UP_SIGNAL_FOR_RANG)) &&
          (index_2 == index_number_UP[index_language][(index_1 - (NUMBER_ROW_FOR_NOTHING_INFORMATION + RANG_BLOCK_UP1)) % 3]))
        {
          name_string_tmp[index_1][index_2] = 0x30 + ((index_1 - (NUMBER_ROW_FOR_NOTHING_INFORMATION + RANG_BLOCK_UP1)) / 3 + 1);
        }
        else
          name_string_tmp[index_1][index_2] = name_string[index_language][index_row][index_2];
      }
    }
  }

  unsigned int state_viewing_input[N_BIG];
  for (size_t i = 0; i < N_BIG; i++)
    state_viewing_input[i] = temp_state[i];

  if (current_ekran.edition == 0)
  {
    //Випадок, коли ми продивляємося зранжовані функції на вході
    int flag = 1;
    for (int m = 0; m < N_BIG; m++)
      if (state_viewing_input[m] != 0)
      {
        flag = 0;
        break;
      }
    if (flag)
    {
      //Це означає, що на даний вхід нічого не відранжовано

      //Текучу позицію в сипску переводимо на сам початок
      current_ekran.index_position = 0;
      position_in_current_level_menu[number_ekran] = 0;

      //Копіюємо  рядки у робочий екран
      for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
      {
        //Копіюємо в робочий екран інформацію, що нічого не відранжовано
        if (i < NUMBER_ROW_FOR_NOTHING_INFORMATION)
          for (unsigned int j = 0; j < MAX_COL_LCD; j++)
            working_ekran[i][j] = name_string_tmp[i][j];
        else
          for (unsigned int j = 0; j < MAX_COL_LCD; j++)
            working_ekran[i][j] = ' ';
      }

      //Відображення курсору по вертикалі
      current_ekran.position_cursor_y = 0;
      //Курсор невидимий
      current_ekran.cursor_on = 0;
    }
    else
    {
      /************************************************************/
      //Формуємо список із функцій, які реально відранжовані
      /************************************************************/
      unsigned int position_temp = current_ekran.index_position;
      unsigned int index_of_ekran;
      unsigned int i = 0, offset = 0;
      unsigned int state_current_bit;

      while ((i + offset) < NUMBER_TOTAL_SIGNAL_FOR_RANG)
      {
        state_current_bit = state_viewing_input[(i + offset) >> 5] & (1 << ((i + offset) & 0x1f));

        if (state_current_bit == 0)
        {
          for (unsigned int j = i; j < (NUMBER_TOTAL_SIGNAL_FOR_RANG - offset); j++)
          {
            if ((j + 1) < (NUMBER_TOTAL_SIGNAL_FOR_RANG - offset))
            {
              for (unsigned int k = 0; k < MAX_COL_LCD; k++)
                name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION][k] = name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION + 1][k];
            }
            else
            {
              for (unsigned int k = 0; k < MAX_COL_LCD; k++)
                name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION][k] = ' ';
            }
          }
          if (current_ekran.index_position >= ((int) (i + offset)))
            position_temp--;
          offset++;

          //Робота з Watchdog
          watchdog_routine(UNITED_BITS_WATCHDOG, 34);
        }
        else
          i++;
      }
      /************************************************************/

      index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

      //Копіюємо  рядки у робочий екран
      for (i = 0; i < MAX_ROW_LCD; i++)
      {
        //Наступні рядки треба перевірити, чи їх требе відображати у текучій кофігурації
        if (index_of_ekran < NUMBER_TOTAL_SIGNAL_FOR_RANG)
        {
          for (unsigned int j = 0; j < MAX_COL_LCD; j++)
            working_ekran[i][j] = name_string_tmp[index_of_ekran + NUMBER_ROW_FOR_NOTHING_INFORMATION][j];

          //Підтягуємо назву, щоб не було спереді багато пробілів
          unsigned int iteration = 0;
          while (
            (working_ekran[i][0] == ' ') &&
            (working_ekran[i][1] == ' ') &&
            (iteration < (MAX_COL_LCD - 1 - 1)))
          {
            for (unsigned int j = 1; j < MAX_COL_LCD; j++)
            {
              if ((j + 1) < MAX_COL_LCD)
                working_ekran[i][j] = working_ekran[i][j + 1];
              else
                working_ekran[i][j] = ' ';
            }
            iteration++;
          }
        }
        else
          for (unsigned int j = 0; j < MAX_COL_LCD; j++)
            working_ekran[i][j] = ' ';

        index_of_ekran++;
      }
      //Відображення курору по вертикалі
      current_ekran.position_cursor_y = position_temp & (MAX_ROW_LCD - 1);
      //Курсор видимий
      current_ekran.cursor_on = 1;
    }

    //Курсор по горизонталі відображається на першій позиції
    current_ekran.position_cursor_x = 0;
    //Курсор не мигає
    current_ekran.cursor_blinking_on = 0;
  }
  else
  {
    unsigned int position_temp = current_ekran.index_position;
    unsigned int index_of_ekran;
    unsigned int i, offset = 0;
    static const int min_max_number[_FIX_NUMBER_PROTECTION][2] =
      {
        {(NUMBER_GENERAL_SIGNAL_FOR_RANG),
         (NUMBER_GENERAL_SIGNAL_FOR_RANG + NUMBER_RPN_SIGNAL_FOR_RANG - 1)},
        {(NUMBER_GENERAL_SIGNAL_FOR_RANG + NUMBER_RPN_SIGNAL_FOR_RANG),
         (NUMBER_GENERAL_SIGNAL_FOR_RANG + NUMBER_RPN_SIGNAL_FOR_RANG + NUMBER_SZKh_SIGNAL_FOR_RANG - 1)},
        {(NUMBER_GENERAL_SIGNAL_FOR_RANG + NUMBER_RPN_SIGNAL_FOR_RANG + NUMBER_SZKh_SIGNAL_FOR_RANG),
         (NUMBER_GENERAL_SIGNAL_FOR_RANG + NUMBER_RPN_SIGNAL_FOR_RANG + NUMBER_SZKh_SIGNAL_FOR_RANG + NUMBER_SNKh_SIGNAL_FOR_RANG - 1)},
        {(NUMBER_GENERAL_SIGNAL_FOR_RANG + NUMBER_RPN_SIGNAL_FOR_RANG + NUMBER_SZKh_SIGNAL_FOR_RANG + NUMBER_SNKh_SIGNAL_FOR_RANG),
         (NUMBER_GENERAL_SIGNAL_FOR_RANG + NUMBER_RPN_SIGNAL_FOR_RANG + NUMBER_SZKh_SIGNAL_FOR_RANG + NUMBER_SNKh_SIGNAL_FOR_RANG + NUMBER_BRP_SIGNAL_FOR_RANG - 1)},
        {(NUMBER_GENERAL_SIGNAL_FOR_RANG + NUMBER_RPN_SIGNAL_FOR_RANG + NUMBER_SZKh_SIGNAL_FOR_RANG + NUMBER_SNKh_SIGNAL_FOR_RANG + NUMBER_BRP_SIGNAL_FOR_RANG),
         (NUMBER_GENERAL_SIGNAL_FOR_RANG + NUMBER_RPN_SIGNAL_FOR_RANG + NUMBER_SZKh_SIGNAL_FOR_RANG + NUMBER_SNKh_SIGNAL_FOR_RANG + NUMBER_BRP_SIGNAL_FOR_RANG + NUMBER_UMAX_SIGNAL_FOR_RANG - 1)},
        {(NUMBER_GENERAL_SIGNAL_FOR_RANG + NUMBER_RPN_SIGNAL_FOR_RANG + NUMBER_SZKh_SIGNAL_FOR_RANG + NUMBER_SNKh_SIGNAL_FOR_RANG + NUMBER_BRP_SIGNAL_FOR_RANG + NUMBER_UMAX_SIGNAL_FOR_RANG),
         (NUMBER_GENERAL_SIGNAL_FOR_RANG + NUMBER_RPN_SIGNAL_FOR_RANG + NUMBER_SZKh_SIGNAL_FOR_RANG + NUMBER_SNKh_SIGNAL_FOR_RANG + NUMBER_BRP_SIGNAL_FOR_RANG + NUMBER_UMAX_SIGNAL_FOR_RANG + NUMBER_UMIN_SIGNAL_FOR_RANG - 1)},
        {(NUMBER_GENERAL_SIGNAL_FOR_RANG + NUMBER_RPN_SIGNAL_FOR_RANG + NUMBER_SZKh_SIGNAL_FOR_RANG + NUMBER_SNKh_SIGNAL_FOR_RANG + NUMBER_BRP_SIGNAL_FOR_RANG + NUMBER_UMAX_SIGNAL_FOR_RANG + NUMBER_UMIN_SIGNAL_FOR_RANG),
         (NUMBER_GENERAL_SIGNAL_FOR_RANG + NUMBER_RPN_SIGNAL_FOR_RANG + NUMBER_SZKh_SIGNAL_FOR_RANG + NUMBER_SNKh_SIGNAL_FOR_RANG + NUMBER_BRP_SIGNAL_FOR_RANG + NUMBER_UMAX_SIGNAL_FOR_RANG + NUMBER_UMIN_SIGNAL_FOR_RANG + NUMBER_UP_SIGNAL_FOR_RANG - 1)},
        {(NUMBER_GENERAL_SIGNAL_FOR_RANG + NUMBER_RPN_SIGNAL_FOR_RANG + NUMBER_SZKh_SIGNAL_FOR_RANG + NUMBER_SNKh_SIGNAL_FOR_RANG + NUMBER_BRP_SIGNAL_FOR_RANG + NUMBER_UMAX_SIGNAL_FOR_RANG + NUMBER_UMIN_SIGNAL_FOR_RANG + NUMBER_UP_SIGNAL_FOR_RANG),
         (NUMBER_GENERAL_SIGNAL_FOR_RANG + NUMBER_RPN_SIGNAL_FOR_RANG + NUMBER_SZKh_SIGNAL_FOR_RANG + NUMBER_SNKh_SIGNAL_FOR_RANG + NUMBER_BRP_SIGNAL_FOR_RANG + NUMBER_UMAX_SIGNAL_FOR_RANG + NUMBER_UMIN_SIGNAL_FOR_RANG + NUMBER_UP_SIGNAL_FOR_RANG + NUMBER_EL_SIGNAL_FOR_RANG - 1)}};

    /*************************************************************/
    //Фільтруємо сигнали, яких у даній конфігурації неприсутні
    /*************************************************************/
    if (
      (type_ekran == INDEX_VIEWING_A_REG) ||
      (type_ekran == INDEX_VIEWING_D_REG) ||
      (type_ekran == INDEX_VIEWING_OFF_CB) ||
      (type_ekran == INDEX_VIEWING_ON_CB))
    {
      /*************************************************************/
      //У випадку, якщо відображення здійснюється вікна аналогового реєстратора чи дискретного реєстратора, то відктдпємо ті функції, які не можуть бути джерелати
      /*************************************************************/

      size_t number = 1;
      for (size_t num = 0; num < number; ++num)
      {
        unsigned int index_deleted_function;

        if (type_ekran == INDEX_VIEWING_A_REG)
          index_deleted_function = RANG_WORK_A_REJESTRATOR;
        else if (type_ekran == INDEX_VIEWING_D_REG)
          index_deleted_function = RANG_WORK_D_REJESTRATOR;
        else if (type_ekran == INDEX_VIEWING_OFF_CB)
        {
          number = 2;

          if (num == 0)
            index_deleted_function = RANG_VIDKL_VID_ZAKHYSTIV;
          else
            index_deleted_function = RANG_WORK_BO;
        }
        else
          index_deleted_function = RANG_WORK_BV;

        /*************************************************************/
        //Відкидаємо ім'я даної функції і зміщаємо біти
        /*************************************************************/

        //Формуємо маску біт, які не треба переміщати при переміщенні імен полів
        unsigned int maska[N_BIG];
        for (int m = 0; m < N_BIG; m++)
          maska[m] = 0;
        for (unsigned int j = 0; j < (index_deleted_function - offset); j++)
          _SET_BIT(maska, j);

        /***/
        //Зміщуємо біти стану реанжування функцій разом із їх назвами
        /***/
        unsigned int new_temp_data_1[N_BIG], new_temp_data_2[N_BIG];

        for (unsigned int k = 0; k < N_BIG; k++)
        {
          new_temp_data_1[k] = state_viewing_input[k] & maska[k];

          new_temp_data_2[k] = state_viewing_input[k] & (~maska[k]);
        }

        for (unsigned int k = 0; k < (N_BIG - 1); k++)
        {
          new_temp_data_2[k] = ((new_temp_data_2[k] >> 1) | ((new_temp_data_2[k + 1] & 0x1) << 31)) & (~maska[k]);
        }
        new_temp_data_2[N_BIG - 1] = (new_temp_data_2[N_BIG - 1] >> 1) & (~maska[N_BIG - 1]);

        for (unsigned int k = 0; k < N_BIG; k++)
        {
          state_viewing_input[k] = new_temp_data_1[k] | new_temp_data_2[k];
        }
        /***/
        for (unsigned int j = (index_deleted_function - offset); j < (NUMBER_TOTAL_SIGNAL_FOR_RANG - offset); j++)
        {
          if ((j + 1) < (NUMBER_TOTAL_SIGNAL_FOR_RANG - offset))
          {
            for (unsigned int k = 0; k < MAX_COL_LCD; k++)
              name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION][k] = name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION + 1][k];
          }
          else
          {
            for (unsigned int k = 0; k < MAX_COL_LCD; k++)
              name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION][k] = ' ';
          }
        }
        if (current_ekran.index_position >= ((int) index_deleted_function))
          position_temp--;
        offset++;
        //Робота з Watchdog
        watchdog_routine(UNITED_BITS_WATCHDOG, 35);
      }
      /*************************************************************/

      /*************************************************************/
    }

    //Функції загального призначення пропускаємо (вони знаходяться у початку списку), тому починаємо з першого записту
    int index_in_list = NUMBER_GENERAL_SIGNAL_FOR_RANG;

    for (i = 0; i < _FIX_NUMBER_PROTECTION; i++)
    {

      if ((current_settings.configuration & (1 << i)) != 0)
      {
        //Захист присутнійсть, тому функції фільтрувати не потрібно - переводимо індекс на наступні функції
        //Додаємо кількість функцій до поточного індексу, якщо для поточного захисту реально були присутні функції,
        //бо інкаше ми вже знаходимося на індексі наступного захисту
        if (min_max_number[i][0] >= 0)
        {
          if (i == EL_BIT_CONFIGURATION)
          {
            /*
            Випадок коли деякі сигнали розширеної логіки треба відфільтрувати
            */

            //Відкидати імена функцій і зміщати біти треба тільки у тому випадку, якщо функції пристні у списку для ранжування для даного захисту
            //Формуємо маску біт, які не треба переміщати при переміщенні імен полів
            unsigned int maska[N_BIG];
            for (int m = 0; m < N_BIG; m++)
              maska[m] = 0;
            unsigned int j1;
            for (j1 = 0; j1 < (min_max_number[i][0] - offset); j1++)
              _SET_BIT(maska, j1);

            //Відкидаємо назви функцій із списку, які є зайвими
            while (index_in_list <= min_max_number[i][1])
            {
              if (
                (
                  /*
                   (
                    (index_in_list >= (int)(RANG_DF1_IN + 2*current_settings.number_defined_df)) &&
                    (index_in_list <= RANG_DF8_OUT)
                   )
                   ||
                   */
                  (
                    (type_ekran == INDEX_VIEWING_DF) &&
                    ((index_in_list == (RANG_DF1_IN + 3 /*кількість сигналів*/ * ((number_ekran - EKRAN_RANGUVANNJA_DF1_PLUS) / MAX_ROW_LIST_TYPE_SOURCE_DF /*кількість типів вікон*/))) ||
                     (index_in_list == (RANG_DF1_R + 3 /*кількість сигналів*/ * ((number_ekran - EKRAN_RANGUVANNJA_DF1_PLUS) / MAX_ROW_LIST_TYPE_SOURCE_DF /*кількість типів вікон*/))) ||
                     (index_in_list == (RANG_DF1_OUT + 3 /*кількість сигналів*/ * ((number_ekran - EKRAN_RANGUVANNJA_DF1_PLUS) / MAX_ROW_LIST_TYPE_SOURCE_DF /*кількість типів вікон*/)))))) ||
                (
                  /*
                   (
                    (index_in_list >= (int)(RANG_DT1_SET + 3*current_settings.number_defined_dt)) &&
                    (index_in_list <= RANG_DT4_OUT)
                   )
                   ||
                   */
                  (
                    (type_ekran == INDEX_VIEWING_DT) &&
                    ((index_in_list == (RANG_DT1_SET + 3 /*кількість сигналів*/ * ((number_ekran - EKRAN_RANGUVANNJA_SET_DT1_PLUS) / 4 /*кількість типів вікон*/))) ||
                     (index_in_list == (RANG_DT1_RESET + 3 /*кількість сигналів*/ * ((number_ekran - EKRAN_RANGUVANNJA_SET_DT1_PLUS) / 4 /*кількість типів вікон*/))) ||
                     (index_in_list == (RANG_DT1_OUT + 3 /*кількість сигналів*/ * ((number_ekran - EKRAN_RANGUVANNJA_SET_DT1_PLUS) / 4 /*кількість типів вікон*/)))))) ||
                (
                  /*
                   (
                    (index_in_list >= (int)(RANG_D_AND1 + current_settings.number_defined_and)) &&
                    (index_in_list <= RANG_D_AND8)
                   )   
                   ||
                   */
                  (
                    (type_ekran == INDEX_VIEWING_D_AND) &&
                    (index_in_list == (RANG_D_AND1 + (number_ekran - EKRAN_RANGUVANNJA_D_AND1))))) ||
                (
                  /*
                   (
                    (index_in_list >= (int)(RANG_D_OR1 + current_settings.number_defined_or)) &&
                    (index_in_list <= RANG_D_OR8)
                   )   
                   ||
                   */
                  (
                    (type_ekran == INDEX_VIEWING_D_OR) &&
                    (index_in_list == (RANG_D_OR1 + (number_ekran - EKRAN_RANGUVANNJA_D_OR1))))) ||
                (
                  /*
                   (
                    (index_in_list >= (int)(RANG_D_XOR1 + current_settings.number_defined_xor)) &&
                    (index_in_list <= RANG_D_XOR8)
                   )   
                   ||
                   */
                  (
                    (type_ekran == INDEX_VIEWING_D_XOR) &&
                    (index_in_list == (RANG_D_XOR1 + (number_ekran - EKRAN_RANGUVANNJA_D_XOR1))))) ||
                (
                  /*
                   (
                    (index_in_list >= (int)(RANG_D_NOT1 + current_settings.number_defined_not)) &&
                    (index_in_list <= RANG_D_NOT16)
                   )   
                   ||
                   */
                  (
                    (type_ekran == INDEX_VIEWING_D_NOT) &&
                    (index_in_list == (RANG_D_NOT1 + (number_ekran - EKRAN_RANGUVANNJA_D_NOT1))))))
              {
                /***/
                //Зміщуємо біти стану реанжування функцій разом із їх назвами
                /***/
                unsigned int new_temp_data_1[N_BIG], new_temp_data_2[N_BIG];

                for (unsigned int k = 0; k < N_BIG; k++)
                {
                  new_temp_data_1[k] = state_viewing_input[k] & maska[k];

                  new_temp_data_2[k] = state_viewing_input[k] & (~maska[k]);
                }

                for (unsigned int k = 0; k < (N_BIG - 1); k++)
                {
                  new_temp_data_2[k] = ((new_temp_data_2[k] >> 1) | ((new_temp_data_2[k + 1] & 0x1) << 31)) & (~maska[k]);
                }
                new_temp_data_2[N_BIG - 1] = (new_temp_data_2[N_BIG - 1] >> 1) & (~maska[N_BIG - 1]);

                for (unsigned int k = 0; k < N_BIG; k++)
                {
                  state_viewing_input[k] = new_temp_data_1[k] | new_temp_data_2[k];
                }
                /***/
                for (unsigned int j = (index_in_list - offset); j < (NUMBER_TOTAL_SIGNAL_FOR_RANG - offset); j++)
                {
                  if ((j + 1) < (NUMBER_TOTAL_SIGNAL_FOR_RANG - offset))
                  {
                    for (unsigned int k = 0; k < MAX_COL_LCD; k++)
                      name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION][k] = name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION + 1][k];
                  }
                  else
                  {
                    for (unsigned int k = 0; k < MAX_COL_LCD; k++)
                      name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION][k] = ' ';
                  }
                }
                if (current_ekran.index_position >= index_in_list)
                  position_temp--;

                offset++;
                //Робота з Watchdog
                watchdog_routine(UNITED_BITS_WATCHDOG, 37);
              }
              else
              {
                _SET_BIT(maska, j1);
                j1++;
              }

              index_in_list++;
            }
          }
          else
            index_in_list += ((min_max_number[i][1] - min_max_number[i][0]) + 1);
        }
      }
      else if (min_max_number[i][0] >= 0)
      {
        //Відкидати імена функцій і зміщати біти треба тільки у тому випадку, якщо функції пристні у списку для ранжування для даного захисту
        //Формуємо маску біт, які не треба переміщати при переміщенні імен полів
        unsigned int maska[N_BIG];
        for (int m = 0; m < N_BIG; m++)
          maska[m] = 0;
        for (unsigned int j = 0; j < (min_max_number[i][0] - offset); j++)
          _SET_BIT(maska, j);

        //Відкидаємо назви функцій із списку, які є зайвими
        while (index_in_list <= min_max_number[i][1])
        {
          /***/
          //Зміщуємо біти стану реанжування функцій разом із їх назвами
          /***/
          unsigned int new_temp_data_1[N_BIG], new_temp_data_2[N_BIG];

          for (unsigned int k = 0; k < N_BIG; k++)
          {
            new_temp_data_1[k] = state_viewing_input[k] & maska[k];

            new_temp_data_2[k] = state_viewing_input[k] & (~maska[k]);
          }

          for (unsigned int k = 0; k < (N_BIG - 1); k++)
          {
            new_temp_data_2[k] = ((new_temp_data_2[k] >> 1) | ((new_temp_data_2[k + 1] & 0x1) << 31)) & (~maska[k]);
          }
          new_temp_data_2[N_BIG - 1] = (new_temp_data_2[N_BIG - 1] >> 1) & (~maska[N_BIG - 1]);

          for (unsigned int k = 0; k < N_BIG; k++)
          {
            state_viewing_input[k] = new_temp_data_1[k] | new_temp_data_2[k];
          }
          /***/
          for (unsigned int j = (index_in_list - offset); j < (NUMBER_TOTAL_SIGNAL_FOR_RANG - offset); j++)
          {
            if ((j + 1) < (NUMBER_TOTAL_SIGNAL_FOR_RANG - offset))
            {
              for (unsigned int k = 0; k < MAX_COL_LCD; k++)
                name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION][k] = name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION + 1][k];
            }
            else
            {
              for (unsigned int k = 0; k < MAX_COL_LCD; k++)
                name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION][k] = ' ';
            }
          }
          if (current_ekran.index_position >= index_in_list)
            position_temp--;

          offset++;
          index_in_list++;
          //Робота з Watchdog
          watchdog_routine(UNITED_BITS_WATCHDOG, 38);
        }
      }

      //Робота з Watchdog
      watchdog_routine(UNITED_BITS_WATCHDOG, 39);
    }
    /*************************************************************/

    //Множення на два величини position_temp потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
    index_of_ekran = ((position_temp << 1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

    for (i = 0; i < MAX_ROW_LCD; i++)
    {
      if (index_of_ekran < ((NUMBER_TOTAL_SIGNAL_FOR_RANG - offset) << 1)) //Множення на два константи  NUMBER_TOTAL_SIGNAL_FOR_RANG потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
      {
        if ((i & 0x1) == 0)
        {
          //У непарному номері рядку виводимо заголовок
          for (unsigned int j = 0; j < MAX_COL_LCD; j++)
            working_ekran[i][j] = name_string_tmp[(index_of_ekran >> 1) + NUMBER_ROW_FOR_NOTHING_INFORMATION][j];
        }
        else
        {
          //У парному номері рядку виводимо стан функції
          static unsigned char const information[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] =
            {
              {"      ОТКЛ      ", "      ВКЛ       "},
              {"      ВИМК      ", "     УВІМК      "},
              {"      OFF       ", "       ON       "},
              {"      СЉНД      ", "      КОСУ      "}};
          unsigned int index_bit = index_of_ekran >> 1;

          for (unsigned int j = 0; j < MAX_COL_LCD; j++)
            working_ekran[i][j] = information[index_language][((state_viewing_input[index_bit >> 5] & (1 << (index_bit & 0x1f))) != 0)][j];
        }
      }
      else
        for (unsigned int j = 0; j < MAX_COL_LCD; j++)
          working_ekran[i][j] = ' ';

      index_of_ekran++;
    }

    static const unsigned int cursor_x[MAX_NAMBER_LANGUAGE][2] =
      {
        {5, 5},
        {5, 4},
        {5, 6},
        {5, 5}};

    //Відображення курору по вертикалі і курсор завжди має бути у полі із значенням устаки
    current_ekran.position_cursor_x = cursor_x[index_language][((state_viewing_input[position_temp >> 5] & (1 << (position_temp & 0x1f))) != 0)];
    current_ekran.position_cursor_y = ((position_temp << 1) + 1) & (MAX_ROW_LCD - 1);

    //Курсор мигає
    current_ekran.cursor_blinking_on = 1;
    //Режим відображення у режимі редагування
  }

  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;

#undef NUMBER_ROW_FOR_NOTHING_INFORMATION
}
/*****************************************************/

/*****************************************************/
//Перевірка, чи текучий індекс вказує на функцію, яка присутня у даній конфігурації
/*****************************************************/
void check_current_index_is_presented_in_configuration(
  unsigned int *found_new_index_tmp,
  int *add_filter_point,
  /*EL_FILTER_STRUCT el_filter[],*/
  int plus_minus,
  int number_general_function,
  int number_el_function)
{
  UNUSED(add_filter_point);

  if (plus_minus == 1)
  {
    if (current_ekran.index_position < number_general_function)
      *found_new_index_tmp = 1;
    else
    {
      if (current_ekran.index_position < (number_general_function + number_el_function))
      {
        if ((current_settings.configuration & (1 << EL_BIT_CONFIGURATION)) != 0)
          *found_new_index_tmp = 1;
        else
          current_ekran.index_position = number_general_function + number_el_function;
      }
    }
  }
  else
  {
    if (current_ekran.index_position < number_general_function)
      *found_new_index_tmp = 1;
    else
    {
      if (current_ekran.index_position < (number_general_function + number_el_function))
      {
        if ((current_settings.configuration & (1 << EL_BIT_CONFIGURATION)) != 0)
          *found_new_index_tmp = 1;
        else
          current_ekran.index_position = number_general_function - 1;
      }
    }
  }
}
/*****************************************************/

/*****************************************************/
//
/*****************************************************/
/*****************************************************/
