#include "header.h"

/*****************************************************/
//Формуємо екран відображення уставок УРОВ
/*****************************************************/
void make_ekran_setpoint_urov(unsigned int group)
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_SETPOINT_UROV][MAX_COL_LCD] = 
  {
    {
      "      УРОВ      "
    },
    {
      "      ПРВВ      "
    },
    {
      "      CBF       "
    },
    {
      "      УРОВ      "
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
    if (index_of_ekran_tmp < MAX_ROW_FOR_SETPOINT_UROV)
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][index_of_ekran_tmp][j];

        vaga = 1000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для уставки УРОВ
        if (view == true) value = current_settings.setpoint_urov[group]; //у змінну value поміщаємо значення уставки ЗЗ
        else value = edition_settings.setpoint_urov[group];
        first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
      }
      else
      {
        //У парному номері рядку виводимо значення уставки
        for (unsigned int j = 0; j<MAX_COL_LCD; j++)
        {
          if (
              ((j < COL_SETPOINT_UROV_BEGIN) ||  (j > COL_SETPOINT_UROV_END )) &&
              (j != (COL_SETPOINT_UROV_END + 2))  
             )working_ekran[i][j] = ' ';
          else if (j == COL_SETPOINT_UROV_COMMA )working_ekran[i][j] = ',';
          else if (j == (COL_SETPOINT_UROV_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_A];
          else
            calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_UROV_COMMA, view, 0);
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
    current_ekran.position_cursor_x = COL_SETPOINT_UROV_BEGIN;
    int last_position_cursor_x = COL_SETPOINT_UROV_END;

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
//Формуємо екран відображення витримок УРОВ
/*****************************************************/
void make_ekran_timeout_urov(unsigned int group)
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_TIMEOUT_UROV][MAX_COL_LCD] = 
  {
    {
      "     УРОВ 1     ",
      "     УРОВ 2     "
    },
    {
      "     ПРВВ 1     ",
      "     ПРВВ 2     "
    },
    {
      "   CBF IntTr    ",
      "   CBF ExtTr    "
    },
    {
      "     УРОВ 1     ",
      "     УРОВ 2     "
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
    if (index_of_ekran_tmp < MAX_ROW_FOR_TIMEOUT_UROV)
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][index_of_ekran_tmp][j];
        if (index_of_ekran_tmp == INDEX_ML_TMOUROV1)
        {
          vaga = 10000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки 1 Ступені УРОВ
          if (view == true) value = current_settings.timeout_urov_1[group]; //у змінну value поміщаємо значення витримки 1 Ступені УРОВ
          else value = edition_settings.timeout_urov_1[group];
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
        else if (index_of_ekran_tmp == INDEX_ML_TMOUROV2)
        {
          vaga = 10000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки 2 Ступені УРОВ
          if (view == true) value = current_settings.timeout_urov_2[group]; //у змінну value поміщаємо значення витримки 2 Ступені УРОВ
          else value = edition_settings.timeout_urov_2[group];
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
      }
      else
      {
        //У парному номері рядку виводимо значення уставки
        for (unsigned int j = 0; j<MAX_COL_LCD; j++)
        {
          if (index_of_ekran_tmp == INDEX_ML_TMOUROV1)
          {
            if (
                ((j < COL_TMO_UROV_1_BEGIN) ||  (j > COL_TMO_UROV_1_END )) &&
                (j != (COL_TMO_UROV_1_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_UROV_1_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_UROV_1_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_UROV_1_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == INDEX_ML_TMOUROV2)
          {
            if (
                ((j < COL_TMO_UROV_2_BEGIN) ||  (j > COL_TMO_UROV_2_END )) &&
                (j != (COL_TMO_UROV_2_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_UROV_2_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_UROV_2_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_UROV_2_COMMA, view, 0);
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
    if (current_ekran.index_position == INDEX_ML_TMOUROV1)
    {
      current_ekran.position_cursor_x = COL_TMO_UROV_1_BEGIN;
      last_position_cursor_x = COL_TMO_UROV_1_END;
    }
    else if (current_ekran.index_position == INDEX_ML_TMOUROV2)
    {
      current_ekran.position_cursor_x = COL_TMO_UROV_2_BEGIN;
      last_position_cursor_x = COL_TMO_UROV_2_END;
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
//Формуємо екран відображення значення управлінської інформації для УРОВ
/*****************************************************/
void make_ekran_control_urov()
{
  // static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_CONTROL_UROV - NUMBER_UP + 1][MAX_COL_LCD] = 
  // {
  //   {
  //     "      УРОВ      ",
  //     "    от МТЗ 1    ",
  //     "    от МТЗ 2    ",
  //     "    от МТЗ 3    ",
  //     "    от МТЗ 4    ",
  //     "  от МТЗ0.4 1   ",
  //     "  от МТЗ0.4 2   ",
  //     "     от ЗДЗ     ",
  //     "     от 3I0     ",
  //     "     от 3U0     ",
  //     "     от НЗЗ     ",
  //     "   от ТЗНП 1    ",
  //     "   от ТЗНП 2    ",
  //     "   от ТЗНП 3    ",
  //     "   от ЗОП(I)    ",
  //     "   от Umin 1    ",
  //     "   от Umin 2    ",
  //     "   от Umax 1    ",
  //     "   от Umax 2    ",
  //     "    от АЧР 1    ",
  //     "    от АЧР 2    ",
  //     "    от УЗ x     "
  //   },
  //   {
  //     "      ПРВВ      ",
  //     "   від МСЗ 1    ",
  //     "   від МСЗ 2    ",
  //     "   від МСЗ 3    ",
  //     "   від МСЗ 4    ",
  //     "  від МСЗ0.4 1  ",
  //     "  від МСЗ0.4 2  ",
  //     "    від ЗДЗ     ",
  //     "    від 3I0     ",
  //     "    від 3U0     ",
  //     "    від СЗЗ     ",
  //     "   від СЗНП 1   ",
  //     "   від СЗНП 2   ",
  //     "   від СЗНП 3   ",
  //     "   від ЗЗП(I)   ",
  //     "   від Umin 1   ",
  //     "   від Umin 2   ",
  //     "   від Umax 1   ",
  //     "   від Umax 2   ",
  //     "   від АЧР 1    ",
  //     "   від АЧР 2    ",
  //     "    від УЗ x    "
  //   },
  //   {
  //     "      CBF       ",
  //     "   via OCP 1    ",
  //     "   via OCP 2    ",
  //     "   via OCP 3    ",
  //     "   via OCP 4    ",
  //     "  via LVOCP 1   ",
  //     "  via LVOCP 2   ",
  //     "    via AFD     ",
  //     "  via EFP 3I0   ",
  //     "  via EFP 3V0   ",
  //     "   via SDEFP    ",
  //     "   via DEFP 1   ",
  //     "   via DEFP 2   ",
  //     "   via DEFP 3   ",
  //     "   via NSOCP    ",
  //     "   via UVP 1    ",
  //     "   via UVP 2    ",
  //     "   via OVP 1    ",
  //     "   via OVP 2    ",
  //     "   via LFC 1    ",
  //     "   via LFC 2    ",
  //     "   via MFP x    "
  //   },
  //   {
  //     "      УРОВ      ",
  //     "    от МТЗ 1    ",
  //     "    от МТЗ 2    ",
  //     "    от МТЗ 3    ",
  //     "    от МТЗ 4    ",
  //     "  от МТЗ0.4 1   ",
  //     "  от МТЗ0.4 2   ",
  //     "     от ЗДЗ     ",
  //     "     от 3I0     ",
  //     "     от 3U0     ",
  //     "     от НЗЗ     ",
  //     "   от ТЗНП 1    ",
  //     "   от ТЗНП 2    ",
  //     "   от ТЗНП 3    ",
  //     "   от ЗОП(I)    ",
  //     "   от Umin 1    ",
  //     "   от Umin 2    ",
  //     "   от Umax 1    ",
  //     "   от Umax 2    ",
  //     "    от АЧР 1    ",
  //     "    от АЧР 2    ",
  //     "    от УЗ x     "
  //   }
  // };
  // static const uint32_t index_number_UP_prvv[MAX_NAMBER_LANGUAGE] = {10, 11, 11, 10};
  
  // unsigned char name_string_tmp[MAX_ROW_FOR_CONTROL_UROV][MAX_COL_LCD];

  // int index_language = index_language_in_array(current_settings.language);
  // for(int index_1 = 0; index_1 < MAX_ROW_FOR_CONTROL_UROV; index_1++)
  // {
  //   unsigned int index_row = (index_1 < (MAX_ROW_FOR_CONTROL_UROV - NUMBER_UP)) ? index_1 : (MAX_ROW_FOR_CONTROL_UROV - NUMBER_UP);
  //   for(int index_2 = 0; index_2 < MAX_COL_LCD; index_2++)
  //   {
  //     if (
  //         (index_1 >= (MAX_ROW_FOR_CONTROL_UROV - NUMBER_UP)) &&
  //         (index_2 == index_number_UP_prvv[index_language]) 
  //        )
  //     {
  //       name_string_tmp[index_1][index_2] = 0x30 + (index_1 - (MAX_ROW_FOR_CONTROL_UROV - NUMBER_UP) + 1);
  //     }
  //     else name_string_tmp[index_1][index_2] = name_string[index_language][index_row][index_2];
  //   }
  // }
  
  // unsigned int temp_data;
  // if(current_ekran.edition == 0) temp_data = current_settings.control_urov;
  // else temp_data = edition_settings.control_urov;
        
  // /******************************************/
  // //Виключаємо поля, які не треба відображати
  // /******************************************/
  // int additional_current_mtz = 0, additional_current_mtz04 = 0, additional_current_zdz = 0;
  // int additional_current_zz = 0, additional_current_tznp = 0, additional_current_zop = 0;
  // int additional_current_Umin = 0, additional_current_Umax = 0, additional_current_achr = 0;
  // int additional_current_up = 0;
  // int position_temp = current_ekran.index_position;
  // int index_of_ekran;

  // int additional_current = additional_current_mtz  + additional_current_mtz04 + additional_current_zdz  + 
  //                          additional_current_zz   + additional_current_tznp  + additional_current_zop  + 
  //                          additional_current_Umin + additional_current_Umax  + additional_current_achr + 
  //                          additional_current_up;
  // for (int current_index = 0; current_index < MAX_ROW_FOR_CONTROL_UROV; current_index++ )
  // {

  //   if (
  //       (
  //        (
  //         (current_index == INDEX_ML_CTRUROV_STARTED_FROM_MTZ1) ||
  //         (current_index == INDEX_ML_CTRUROV_STARTED_FROM_MTZ2) ||
  //         (current_index == INDEX_ML_CTRUROV_STARTED_FROM_MTZ3) ||
  //         (current_index == INDEX_ML_CTRUROV_STARTED_FROM_MTZ4)
  //        )   
  //        &&
  //        ((current_settings.configuration & (1<<MTZ_BIT_CONFIGURATION)) == 0)
  //       )  
  //       ||
  //       (  
  //        (
  //         (current_index == INDEX_ML_CTRUROV_STARTED_FROM_MTZ04_1) ||
  //         (current_index == INDEX_ML_CTRUROV_STARTED_FROM_MTZ04_2)
  //        )   
  //        &&
  //        ((current_settings.configuration & (1<<MTZ04_BIT_CONFIGURATION)) == 0)
  //       )  
  //       ||
  //       (
  //        (
  //         (current_index == INDEX_ML_CTRUROV_STARTED_FROM_ZDZ)
  //        )   
  //        &&
  //        ((current_settings.configuration & (1<<ZDZ_BIT_CONFIGURATION)) == 0)
  //       )
  //       ||
  //       (
  //        (
  //         (current_index == INDEX_ML_CTRUROV_STARTED_FROM_3I0) ||
  //         (current_index == INDEX_ML_CTRUROV_STARTED_FROM_3U0) ||
  //         (current_index == INDEX_ML_CTRUROV_STARTED_FROM_NZZ)
  //        )   
  //        &&
  //        ((current_settings.configuration & (1<<ZZ_BIT_CONFIGURATION)) == 0)
  //       )
  //       ||
  //       (
  //        (
  //         (current_index == INDEX_ML_CTRUROV_STARTED_FROM_NZZ)
  //        )
  //        &&  
  //        ((current_settings.control_zz & CTR_ZZ1_TYPE) != 0)  
  //       )
  //       ||
  //       (
  //        (
  //         (current_index == INDEX_ML_CTRUROV_STARTED_FROM_TZNP1) ||
  //         (current_index == INDEX_ML_CTRUROV_STARTED_FROM_TZNP2) ||
  //         (current_index == INDEX_ML_CTRUROV_STARTED_FROM_TZNP3)
  //        )   
  //        &&
  //        ((current_settings.configuration & (1<<TZNP_BIT_CONFIGURATION)) == 0)
  //       )  
  //       ||
  //       (
  //        (
  //         (current_index == INDEX_ML_CTRUROV_STARTED_FROM_ZOP1)
  //        )   
  //        &&
  //        ((current_settings.configuration & (1<<ZOP_BIT_CONFIGURATION)) == 0)
  //       )
  //       ||
  //       (
  //        (
  //         (current_index == INDEX_ML_CTRUROV_STARTED_FROM_UMIN1) ||
  //         (current_index == INDEX_ML_CTRUROV_STARTED_FROM_UMIN2)
  //        )   
  //        &&
  //        ((current_settings.configuration & (1<<UMIN_BIT_CONFIGURATION)) == 0)
  //       )  
  //       ||
  //       (
  //        (
  //         (current_index == INDEX_ML_CTRUROV_STARTED_FROM_UMAX1) ||
  //         (current_index == INDEX_ML_CTRUROV_STARTED_FROM_UMAX2)
  //        )   
  //        &&
  //        ((current_settings.configuration & (1<<UMAX_BIT_CONFIGURATION)) == 0)
  //       )  
  //       ||
  //       (
  //        (
  //         (current_index == INDEX_ML_CTRUROV_STARTED_FROM_ACHR1) ||
  //         (current_index == INDEX_ML_CTRUROV_STARTED_FROM_ACHR2)
  //        )   
  //        &&
  //        ((current_settings.configuration & (1<<ACHR_CHAPV_BIT_CONFIGURATION)) == 0)
  //       )
  //       ||
  //       (
  //        (
  //         (current_index >= INDEX_ML_CTRUROV_STARTED_FROM_UP1) &&
  //         (current_index <= INDEX_ML_CTRUROV_STARTED_FROM_UP1 + NUMBER_UP - 1)
  //        )   
  //        &&
  //        ((current_settings.configuration & (1<<UP_BIT_CONFIGURATION)) == 0)
  //       )
  //      )   
  //   {
  //     int i = current_index - additional_current;
  //     unsigned int maska_1, maska_2;
  //     maska_1 = (1 << i) - 1;
  //     maska_2 = (unsigned int)(~maska_1);
    
  //     if ((i+1) <= position_temp) position_temp--;
  //     do
  //     {
  //       for(unsigned int j = 0; j < MAX_COL_LCD; j++)
  //       {
  //         if ((i+1) < (MAX_ROW_FOR_CONTROL_UROV - additional_current)) name_string_tmp[i][j] = name_string_tmp[i + 1][j];
  //         else name_string_tmp[i][j] = ' ';
  //       }
  //       i++;
  //     }
  //     while (i < (MAX_ROW_FOR_CONTROL_UROV -  additional_current));
    
  //     unsigned int temp_data_1 = (temp_data >> 1) & maska_2;
  //     temp_data = (temp_data & maska_1) | temp_data_1;

  //     if (
  //         (current_index == INDEX_ML_CTRUROV_STARTED_FROM_MTZ1) ||
  //         (current_index == INDEX_ML_CTRUROV_STARTED_FROM_MTZ2) ||
  //         (current_index == INDEX_ML_CTRUROV_STARTED_FROM_MTZ3) ||
  //         (current_index == INDEX_ML_CTRUROV_STARTED_FROM_MTZ4)
  //        )   
  //       additional_current_mtz++;

  //     if (
  //         (current_index == INDEX_ML_CTRUROV_STARTED_FROM_MTZ04_1) ||
  //         (current_index == INDEX_ML_CTRUROV_STARTED_FROM_MTZ04_2)
  //        )   
  //       additional_current_mtz04++;

  //     if (
  //         (current_index == INDEX_ML_CTRUROV_STARTED_FROM_ZDZ)
  //        )   
  //       additional_current_zdz++;

  //     if (
  //         (current_index == INDEX_ML_CTRUROV_STARTED_FROM_3I0) ||
  //         (current_index == INDEX_ML_CTRUROV_STARTED_FROM_3U0) ||
  //         (current_index == INDEX_ML_CTRUROV_STARTED_FROM_NZZ)
  //        )   
  //       additional_current_zz++;

  //     if (
  //         (current_index == INDEX_ML_CTRUROV_STARTED_FROM_TZNP1) ||
  //         (current_index == INDEX_ML_CTRUROV_STARTED_FROM_TZNP2) ||
  //         (current_index == INDEX_ML_CTRUROV_STARTED_FROM_TZNP3)
  //        )   
  //       additional_current_tznp++;

  //     if (
  //         (current_index == INDEX_ML_CTRUROV_STARTED_FROM_ZOP1)
  //        )   
  //       additional_current_zop++;

  //     if (
  //         (current_index == INDEX_ML_CTRUROV_STARTED_FROM_UMIN1) ||
  //         (current_index == INDEX_ML_CTRUROV_STARTED_FROM_UMIN2)
  //        )   
  //       additional_current_Umin++;

  //     if (
  //         (current_index == INDEX_ML_CTRUROV_STARTED_FROM_UMAX1) ||
  //         (current_index == INDEX_ML_CTRUROV_STARTED_FROM_UMAX2)
  //        )   
  //       additional_current_Umax++;

  //     if (
  //         (current_index == INDEX_ML_CTRUROV_STARTED_FROM_ACHR1) ||
  //         (current_index == INDEX_ML_CTRUROV_STARTED_FROM_ACHR2)
  //        )   
  //       additional_current_achr++;

  //     if (
  //         (current_index >= INDEX_ML_CTRUROV_STARTED_FROM_UP1) &&
  //         (current_index <= INDEX_ML_CTRUROV_STARTED_FROM_UP1 + NUMBER_UP - 1)
  //        )   
  //       additional_current_up++;
      
  //     additional_current = additional_current_mtz  + additional_current_mtz04 + additional_current_zdz  + 
  //                          additional_current_zz   + additional_current_tznp  + additional_current_zop  + 
  //                          additional_current_Umin + additional_current_Umax  + additional_current_achr +
  //                          additional_current_up;
  //   }
  // }
  // /******************************************/
  
  // //Множення на два величини position_temp потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
  // index_of_ekran = ((position_temp<<1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  
  // for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  // {
  //   int index_of_ekran_tmp = index_of_ekran >> 1;
  //   if (index_of_ekran_tmp < (MAX_ROW_FOR_CONTROL_UROV - additional_current))
  //   {
  //     if ((i & 0x1) == 0)
  //     {
  //       //У непарному номері рядку виводимо заголовок
  //       for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string_tmp[index_of_ekran_tmp][j];
  //     }
  //     else
  //     {
  //       //У парному номері рядку виводимо значення уставки
  //       unsigned int index_ctr = index_of_ekran_tmp;

  //       for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = information_off_on[index_language][(temp_data >> index_ctr) & 0x1][j];
  //       if (position_temp == index_of_ekran_tmp) current_ekran.position_cursor_x = cursor_x_off_on[index_language][(temp_data >> index_ctr) & 0x1];
  //     }
  //   }
  //   else
  //     for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';

  //   index_of_ekran++;
  // }

  // //Відображення курору по вертикалі і курсор завжди має бути у полі із значенням устаки
  // current_ekran.position_cursor_y = ((position_temp<<1) + 1) & (MAX_ROW_LCD - 1);
  // //Курсор видимий
  // current_ekran.cursor_on = 1;
  // //Курсор не мигає
  // if(current_ekran.edition == 0)current_ekran.cursor_blinking_on = 0;
  // else current_ekran.cursor_blinking_on = 1;
  // //Обновити повністю весь екран
  // current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//
/*****************************************************/
/*****************************************************/
