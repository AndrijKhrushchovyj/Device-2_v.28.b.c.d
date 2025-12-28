#include "header.h"

/*****************************************************/
//Формуємо екран відображення витримок АПВ
/*****************************************************/
void make_ekran_timeout_apv(unsigned int group)
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_TIMEOUT_APV][MAX_COL_LCD] = 
  {
    {
      "   1 Цикл АПВ   ",
      "   2 Цикл АПВ   ",
      "   3 Цикл АПВ   ",
      "   4 Цикл АПВ   ",
      "   Блк.АПВ 1    ",
      "   Блк.АПВ 2    ",
      "   Блк.АПВ 3    ",
      "   Блк.АПВ 4    ",
      " Блк.АПВ от ВВ  "
    },
    {
      "   1 Цикл АПВ   ",
      "   2 Цикл АПВ   ",
      "   3 Цикл АПВ   ",
      "   4 Цикл АПВ   ",
      "   Блк.АПВ 1    ",
      "   Блк.АПВ 2    ",
      "   Блк.АПВ 3    ",
      "   Блк.АПВ 4    ",
      " Бл.АПВ від ВВ  "
    },
    {
      "   AR Cycle 1   ",
      "   AR Cycle 2   ",
      "   AR Cycle 3   ",
      "   AR Cycle 4   ",
      "  AR Cyc 1 Blk  ",
      "  AR Cyc 2 Blk  ",
      "  AR Cyc 3 Blk  ",
      "  AR Cyc 4 Blk  ",
      "   AR CB Blk    "
    },
    {
      "   1 Цикл АПВ   ",
      "   2 Цикл АПВ   ",
      "   3 Цикл АПВ   ",
      "   4 Цикл АПВ   ",
      "   Блк.АПВ 1    ",
      "   Блк.АПВ 2    ",
      "   Блк.АПВ 3    ",
      "   Блк.АПВ 4    ",
      " Блк.АПВ от ВВ  "
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
    if (index_of_ekran_tmp < MAX_ROW_FOR_TIMEOUT_APV)
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][index_of_ekran_tmp][j];
        if (index_of_ekran_tmp == INDEX_ML_TMOAPV1)
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки 1 Циклу АПВ
          if (view == true) value = current_settings.timeout_apv_1[group]; //у змінну value поміщаємо значення витримки 1 Циклу АПВ
          else value = edition_settings.timeout_apv_1[group];
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
        else if (index_of_ekran_tmp == INDEX_ML_TMOAPV2)
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки 2 Циклу АПВ
          if (view == true) value = current_settings.timeout_apv_2[group]; //у змінну value поміщаємо значення витримки 2 Циклу АПВ
          else value = edition_settings.timeout_apv_2[group];
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
        else if (index_of_ekran_tmp == INDEX_ML_TMOAPV3)
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки 3 Циклу АПВ
          if (view == true) value = current_settings.timeout_apv_3[group]; //у змінну value поміщаємо значення витримки 3 Циклу АПВ
          else value = edition_settings.timeout_apv_3[group];
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
        else if (index_of_ekran_tmp == INDEX_ML_TMOAPV4)
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки 4 Циклу АПВ
          if (view == true) value = current_settings.timeout_apv_4[group]; //у змінну value поміщаємо значення витримки 4 Циклу АПВ
          else value = edition_settings.timeout_apv_4[group];
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
        else if (index_of_ekran_tmp == INDEX_ML_TMOAPV_BLOCK_VID_APV1)
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки АПВ підготовки до повторної роботи
          if (view == true) value = current_settings.timeout_apv_block_vid_apv1[group]; //у змінну value поміщаємо значення витримки АПВ підготовки до повторної роботи
          else value = edition_settings.timeout_apv_block_vid_apv1[group];
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
        else if (index_of_ekran_tmp == INDEX_ML_TMOAPV_BLOCK_VID_APV2)
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки АПВ підготовки до повторної роботи
          if (view == true) value = current_settings.timeout_apv_block_vid_apv2[group]; //у змінну value поміщаємо значення витримки АПВ підготовки до повторної роботи
          else value = edition_settings.timeout_apv_block_vid_apv2[group];
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
        else if (index_of_ekran_tmp == INDEX_ML_TMOAPV_BLOCK_VID_APV3)
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки АПВ підготовки до повторної роботи
          if (view == true) value = current_settings.timeout_apv_block_vid_apv3[group]; //у змінну value поміщаємо значення витримки АПВ підготовки до повторної роботи
          else value = edition_settings.timeout_apv_block_vid_apv3[group];
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
        else if (index_of_ekran_tmp == INDEX_ML_TMOAPV_BLOCK_VID_APV4)
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки АПВ підготовки до повторної роботи
          if (view == true) value = current_settings.timeout_apv_block_vid_apv4[group]; //у змінну value поміщаємо значення витримки АПВ підготовки до повторної роботи
          else value = edition_settings.timeout_apv_block_vid_apv4[group];
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
        else if (index_of_ekran_tmp == INDEX_ML_TMOAPV_BLOCK_VID_VV)
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки АПВ подачі вихідного сигналу
          if (view == true) value = current_settings.timeout_apv_block_vid_VV[group]; //у змінну value поміщаємо значення итримки АПВ подачі вихідного сигналу
          else value = edition_settings.timeout_apv_block_vid_VV[group];
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
      }
      else
      {
        //У парному номері рядку виводимо значення уставки
        for (unsigned int j = 0; j<MAX_COL_LCD; j++)
        {
          if (index_of_ekran_tmp == INDEX_ML_TMOAPV1)
          {
            if (
                ((j < COL_TMO_APV_1_BEGIN) ||  (j > COL_TMO_APV_1_END )) &&
                (j != (COL_TMO_APV_1_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_APV_1_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_APV_1_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_APV_1_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == INDEX_ML_TMOAPV2)
          {
            if (
                ((j < COL_TMO_APV_2_BEGIN) ||  (j > COL_TMO_APV_2_END )) &&
                (j != (COL_TMO_APV_2_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_APV_2_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_APV_2_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_APV_2_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == INDEX_ML_TMOAPV3)
          {
            if (
                ((j < COL_TMO_APV_3_BEGIN) ||  (j > COL_TMO_APV_3_END )) &&
                (j != (COL_TMO_APV_3_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_APV_3_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_APV_3_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_APV_3_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == INDEX_ML_TMOAPV4)
          {
            if (
                ((j < COL_TMO_APV_4_BEGIN) ||  (j > COL_TMO_APV_4_END )) &&
                (j != (COL_TMO_APV_4_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_APV_4_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_APV_4_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_APV_4_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == INDEX_ML_TMOAPV_BLOCK_VID_APV1)
          {
            if (
                ((j < COL_TMO_APV_BLOCK_VID_APV1_BEGIN) ||  (j > COL_TMO_APV_BLOCK_VID_APV1_END )) &&
                (j != (COL_TMO_APV_BLOCK_VID_APV1_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_APV_BLOCK_VID_APV1_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_APV_BLOCK_VID_APV1_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_APV_BLOCK_VID_APV1_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == INDEX_ML_TMOAPV_BLOCK_VID_APV2)
          {
            if (
                ((j < COL_TMO_APV_BLOCK_VID_APV2_BEGIN) ||  (j > COL_TMO_APV_BLOCK_VID_APV2_END )) &&
                (j != (COL_TMO_APV_BLOCK_VID_APV2_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_APV_BLOCK_VID_APV2_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_APV_BLOCK_VID_APV2_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_APV_BLOCK_VID_APV2_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == INDEX_ML_TMOAPV_BLOCK_VID_APV3)
          {
            if (
                ((j < COL_TMO_APV_BLOCK_VID_APV3_BEGIN) ||  (j > COL_TMO_APV_BLOCK_VID_APV3_END )) &&
                (j != (COL_TMO_APV_BLOCK_VID_APV3_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_APV_BLOCK_VID_APV3_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_APV_BLOCK_VID_APV3_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_APV_BLOCK_VID_APV3_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == INDEX_ML_TMOAPV_BLOCK_VID_APV4)
          {
            if (
                ((j < COL_TMO_APV_BLOCK_VID_APV4_BEGIN) ||  (j > COL_TMO_APV_BLOCK_VID_APV4_END )) &&
                (j != (COL_TMO_APV_BLOCK_VID_APV4_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_APV_BLOCK_VID_APV4_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_APV_BLOCK_VID_APV4_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_APV_BLOCK_VID_APV4_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == INDEX_ML_TMOAPV_BLOCK_VID_VV)
          {
            if (
                ((j < COL_TMO_APV_BLOCK_VID_VV_BEGIN) ||  (j > COL_TMO_APV_BLOCK_VID_VV_END )) &&
                (j != (COL_TMO_APV_BLOCK_VID_VV_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_APV_BLOCK_VID_VV_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_APV_BLOCK_VID_VV_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_APV_BLOCK_VID_VV_COMMA, view, 0);
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
    if (current_ekran.index_position == INDEX_ML_TMOAPV1) 
    {
      current_ekran.position_cursor_x = COL_TMO_APV_1_BEGIN;
      last_position_cursor_x = COL_TMO_APV_1_END;
    }
    else if (current_ekran.index_position == INDEX_ML_TMOAPV2) 
    {
      current_ekran.position_cursor_x = COL_TMO_APV_2_BEGIN;
      last_position_cursor_x = COL_TMO_APV_2_END;
    }
    else if (current_ekran.index_position == INDEX_ML_TMOAPV3) 
    {
      current_ekran.position_cursor_x = COL_TMO_APV_3_BEGIN;
      last_position_cursor_x = COL_TMO_APV_3_END;
    }
    else if (current_ekran.index_position == INDEX_ML_TMOAPV4) 
    {
      current_ekran.position_cursor_x = COL_TMO_APV_4_BEGIN;
      last_position_cursor_x = COL_TMO_APV_4_END;
    }
    else if (current_ekran.index_position == INDEX_ML_TMOAPV_BLOCK_VID_APV1) 
    {
      current_ekran.position_cursor_x = COL_TMO_APV_BLOCK_VID_APV1_BEGIN;
      last_position_cursor_x = COL_TMO_APV_BLOCK_VID_APV1_END;
    }
    else if (current_ekran.index_position == INDEX_ML_TMOAPV_BLOCK_VID_APV2) 
    {
      current_ekran.position_cursor_x = COL_TMO_APV_BLOCK_VID_APV2_BEGIN;
      last_position_cursor_x = COL_TMO_APV_BLOCK_VID_APV2_END;
    }
    else if (current_ekran.index_position == INDEX_ML_TMOAPV_BLOCK_VID_APV3) 
    {
      current_ekran.position_cursor_x = COL_TMO_APV_BLOCK_VID_APV3_BEGIN;
      last_position_cursor_x = COL_TMO_APV_BLOCK_VID_APV3_END;
    }
    else if (current_ekran.index_position == INDEX_ML_TMOAPV_BLOCK_VID_APV4) 
    {
      current_ekran.position_cursor_x = COL_TMO_APV_BLOCK_VID_APV4_BEGIN;
      last_position_cursor_x = COL_TMO_APV_BLOCK_VID_APV4_END;
    }
    else if (current_ekran.index_position == INDEX_ML_TMOAPV_BLOCK_VID_VV)
    {
      current_ekran.position_cursor_x = COL_TMO_APV_BLOCK_VID_VV_BEGIN;
      last_position_cursor_x = COL_TMO_APV_BLOCK_VID_VV_END;
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
//Формуємо екран відображення значення управлінської інформації для АПВ
/*****************************************************/
void make_ekran_control_apv()
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_CONTROL_APV][MAX_COL_LCD] = 
  {
    {
      "      АПВ       ",
      "     АПВ 2      ",
      "     АПВ 3      ",
      "     АПВ 4      ",
      " Пуск от МТЗ 1  ",
      " Пуск от МТЗ 2  ",
      " Пуск от МТЗ 3  ",
      " Пуск от МТЗ 4  ",
      "К.пол.ВВ для АПВ",
      "Бл.АПВ от УРОВ 2"
    },
    {
      "      АПВ       ",
      "     АПВ 2      ",
      "     АПВ 3      ",
      "     АПВ 4      ",
      " Пуск від МСЗ 1 ",
      " Пуск від МСЗ 2 ",
      " Пуск від МСЗ 3 ",
      " Пуск від МСЗ 4 ",
      " К.ст.ВВ для АПВ",
      "Бл.АПВ від ПРВВ2"
    },
    {
      "       AR       ",
      "      AR 2      ",
      "      AR 3      ",
      "      AR 4      ",
      "   via OCP 1    ",
      "   via OCP 2    ",
      "   via OCP 3    ",
      "   via OCP 4    ",
      " AR CB State Ctr",
      " AR Blk via CBF "
    },
    {
      "      АПВ       ",
      "     АПВ 2      ",
      "     АПВ 3      ",
      "     АПВ 4      ",
      " Пуск от МТЗ 1  ",
      " Пуск от МТЗ 2  ",
      " Пуск от МТЗ 3  ",
      " Пуск от МТЗ 4  ",
      "К.пол.ВВ для АПВ",
      "Бл.АПВ от УРОВ 2"
    }
  };
  unsigned char name_string_tmp[MAX_ROW_FOR_CONTROL_APV][MAX_COL_LCD];

  int index_language = index_language_in_array(current_settings.language);
  for(int index_1 = 0; index_1 < MAX_ROW_FOR_CONTROL_APV; index_1++)
  {
    for(int index_2 = 0; index_2 < MAX_COL_LCD; index_2++)
      name_string_tmp[index_1][index_2] = name_string[index_language][index_1][index_2];
  }
  
  unsigned int temp_data;
          
  if(current_ekran.edition == 0) temp_data = current_settings.control_apv;
  else temp_data = edition_settings.control_apv;
          
  /******************************************/
  //Виключаємо поля, які не треба відображати
  /******************************************/
  int additional_current = 0;
  int position_temp = current_ekran.index_position;
  int index_of_ekran;
  
  // if ((current_settings.configuration & (1<<MTZ_BIT_CONFIGURATION)) == 0)
  // {
  //   for (int current_index = INDEX_ML_CTRAPV_STARTED_FROM_MTZ1; current_index <= INDEX_ML_CTRAPV_STARTED_FROM_MTZ4; current_index++ )
  //   {
  //     int i = current_index - additional_current;
  //     unsigned int maska_1, maska_2;
  //     maska_1 = (1 << i) - 1;
  //     maska_2 = (unsigned int)(~maska_1);
    
  //     if ((i+1) <= position_temp) position_temp--;
  //     do
  //     {
  //       for(unsigned int j = 0; j<MAX_COL_LCD; j++)
  //       {
  //         if ((i+1) < (MAX_ROW_FOR_CONTROL_APV - additional_current)) name_string_tmp[i][j] = name_string_tmp[i + 1][j];
  //         else name_string_tmp[i][j] = ' ';
  //       }
  //       i++;
  //     }
  //     while (i < (MAX_ROW_FOR_CONTROL_APV - additional_current));
    
  //     unsigned int temp_data_1 = (temp_data >> 1) & maska_2;
  //     temp_data = (temp_data & maska_1) | temp_data_1;

  //     additional_current++;
  //   }
  // }

  // if ((current_settings.configuration & (1<<UROV_BIT_CONFIGURATION)) == 0)
  // {
  //   {
  //     int i = INDEX_ML_CTRAPV_BLK_CTRL_PRVV2 - additional_current;
  //     unsigned int maska_1, maska_2;
  //     maska_1 = (1 << i) - 1;
  //     maska_2 = (unsigned int)(~maska_1);
    
  //     if ((i+1) <= position_temp) position_temp--;
  //     do
  //     {
  //       for(unsigned int j = 0; j<MAX_COL_LCD; j++)
  //       {
  //         if ((i+1) < (MAX_ROW_FOR_CONTROL_APV - additional_current)) name_string_tmp[i][j] = name_string_tmp[i + 1][j];
  //         else name_string_tmp[i][j] = ' ';
  //       }
  //       i++;
  //     }
  //     while (i < (MAX_ROW_FOR_CONTROL_APV - additional_current));
    
  //     unsigned int temp_data_1 = (temp_data >> 1) & maska_2;
  //     temp_data = (temp_data & maska_1) | temp_data_1;

  //     additional_current++;
  //   }
  // }
  /******************************************/

  //Множення на два величини position_temp потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
  index_of_ekran = ((position_temp<<1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;
  
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    int index_of_ekran_tmp = index_of_ekran >> 1;
    if (index_of_ekran_tmp < (MAX_ROW_FOR_CONTROL_APV - additional_current))
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string_tmp[index_of_ekran_tmp][j];
      }
      else
      {
        //У парному номері рядку виводимо значення уставки
        unsigned int index_ctr = index_of_ekran_tmp;

        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = information_off_on[index_language][(temp_data >> index_ctr) & 0x1][j];
        if (position_temp == index_of_ekran_tmp)current_ekran.position_cursor_x = cursor_x_off_on[index_language][(temp_data >> index_ctr) & 0x1];
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
