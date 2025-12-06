#include "header.h"

/*****************************************************/
//Формуємо екран відображення вибору ранжування БУвімк./БВимк.
/*****************************************************/
void make_ekran_choose_CBOn_CBOff(void)
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_RANG_SWITCH][MAX_COL_LCD] = 
  {
    {
      " БО             ",
      " БВ             "
    },
    {
      " БВимк.         ",
      " БУвімк.        "
    },
    {
      " Open CB Unit   ",
      " Close CB Unit  "
    },
    {
      " БО             ",
      " БВ             "
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
    if (index_of_ekran < MAX_ROW_FOR_LIST_OF_REGISTRATORS)
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
//Формуємо екран відображення уставок вимикача
/*****************************************************/
void make_ekran_setpoint_switch()
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_SETPOINT_SWITCH][MAX_COL_LCD] = 
  {
    {
      "     I ном.     ",
      " Р.к.ст.(I ном.)",
      "    Iот.ном.    ",
      "Р.к.ст.(Iот.ном)",
      "Нач.знач.ресурса",
      "Критичный ресурс",
      "Нач.знач.к.откл."
    },
    {
      "     I ном.     ",
      " Р.к.ст.(I ном.)",
      "    Iв.ном.     ",
      "Р.к.ст.(Iв.ном.)",
      "Поч.знач.ресурсу",
      "Критичний ресурс",
      "Поч.знач.к.вимк."
    },
    {
      "      CB Ir     ",
      "   CB Res Ir    ",
      "   CB Open Ir   ",
      " CB Res Open Ir ",
      " CB Res Balance ",
      "  CB Crit Res   ",
      "CB Comut balance"
    },
    {
      "     I ном.     ",
      " Р.к.ст.(I ном.)",
      "    Iот.ном.    ",
      "Р.к.ст.(Iот.ном)",
      "Нач.знач.ресурса",
      "Критичный ресурс",
      "Нач.знач.к.откл."
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
    if (index_of_ekran_tmp < MAX_ROW_FOR_SETPOINT_SWITCH)
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][index_of_ekran_tmp][j];
        if (index_of_ekran_tmp == INDEX_ML_STPInom)
        {
          vaga = 10000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для уставки номінального струму
          if (view == true) value = current_settings.setpoint_Inom; //у змінну value поміщаємо значення уставки номінального струму
          else value = edition_settings.setpoint_Inom;
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
        else if (index_of_ekran_tmp == INDEX_ML_STPRKS_Inom)
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду ресурсу комунікаційнійої стійкості при номінальному струмі
          if (view == true) value = current_settings.setpoint_r_kom_st_Inom; //у змінну value поміщаємо значення уставки ресурсу комунікаційнійої стійкості при номінальному струмі
          else value = edition_settings.setpoint_r_kom_st_Inom;
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
        else if (index_of_ekran_tmp == INDEX_ML_STPMInom_vymk)
        {
          vaga = 10000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для уставки номінального струму вимкнення
          if (view == true) value = current_settings.setpoint_Inom_vymk; //у змінну value поміщаємо значення уставки номінального струму вимкнення
          else value = edition_settings.setpoint_Inom_vymk;
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
        else if (index_of_ekran_tmp == INDEX_ML_STPRKS_Inom_vymk)
        {
          vaga = 100; //максимальний ваговий коефіцієнт для вилілення старшого розряду ресурсу комунікаційнійої стійкості при номінальному струмі вимкнення
          if (view == true) value = current_settings.setpoint_r_kom_st_Inom_vymk; //у змінну value поміщаємо значення уставки ресурсу комунікаційнійої стійкості при номінальному струмі вимкнення
          else value = edition_settings.setpoint_r_kom_st_Inom_vymk;
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
        else if (index_of_ekran_tmp == INDEX_ML_STPPOCHATKOVYJ_RESURS)
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду початкового ресурсу вимикача
          if (view == true) value = current_settings.setpoint_pochatkovyj_resurs; //у змінну value поміщаємо значення уставки початкового ресурсу вимикача
          else value = edition_settings.setpoint_pochatkovyj_resurs;
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
        else if (index_of_ekran_tmp == INDEX_ML_STPKRYTYCHNYJ_RESURS)
        {
          vaga = 10000; //максимальний ваговий коефіцієнт для вилілення старшого розряду критичного ресурсу вимикача
          if (view == true) value = current_settings.setpoint_krytychnyj_resurs; //у змінну value поміщаємо значення уставки критичного ресурсу вимикача
          else value = edition_settings.setpoint_krytychnyj_resurs;
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
        else if (index_of_ekran_tmp == INDEX_ML_STPPOCHATKOVA_K_VYMK)
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду ресурсу комунікаційнійої стійкості при номінальному струмі
          if (view == true) value = current_settings.setpoint_pochatkova_k_vymk; //у змінну value поміщаємо значення уставки ресурсу комунікаційнійої стійкості при номінальному струмі
          else value = edition_settings.setpoint_pochatkova_k_vymk;
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
      }
      else
      {
        //У парному номері рядку виводимо значення уставки
        for (unsigned int j = 0; j<MAX_COL_LCD; j++)
        {
          if (index_of_ekran_tmp == INDEX_ML_STPInom)
          {
            if (
                ((j < COL_SETPOINT_Inom_BEGIN) ||  (j > COL_SETPOINT_Inom_END )) &&
                (j != (COL_SETPOINT_Inom_END + 2))
               )working_ekran[i][j] = ' ';
            else if (j == (COL_SETPOINT_Inom_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_A]; 
            else
              calc_int_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, view);
          }
          else if (index_of_ekran_tmp == INDEX_ML_STPRKS_Inom)
          {
            if ((j < COL_SETPOINT_RKS_Inom_BEGIN) ||  (j > COL_SETPOINT_RKS_Inom_END ))
              working_ekran[i][j] = ' ';
            else
              calc_int_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, view);
          }
          else if (index_of_ekran_tmp == INDEX_ML_STPMInom_vymk)
          {
            if (
                ((j < COL_SETPOINT_Inom_vymk_BEGIN) ||  (j > COL_SETPOINT_Inom_vymk_END )) &&
                (
                 !((j >= (COL_SETPOINT_Inom_vymk_END + 2)) && (j <= (COL_SETPOINT_Inom_vymk_END + 3)))
                )    
               )working_ekran[i][j] = ' ';
            else if (j == COL_SETPOINT_Inom_vymk_COMMA )working_ekran[i][j] = ',';
            else if ((j >= (COL_SETPOINT_Inom_vymk_END + 2)) && (j <= (COL_SETPOINT_Inom_vymk_END + 3))) 
            {
              static const unsigned char kA[MAX_NAMBER_LANGUAGE][2] = {"кА", "кА", "kA", "кА"};
              
              working_ekran[i][j] = kA[index_language][j - (COL_SETPOINT_Inom_vymk_END + 2)];
            }
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_Inom_vymk_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == INDEX_ML_STPRKS_Inom_vymk)
          {
            if ((j < COL_SETPOINT_RKS_Inom_vymk_BEGIN) ||  (j > COL_SETPOINT_RKS_Inom_vymk_END ))
              working_ekran[i][j] = ' ';
            else
              calc_int_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, view);
          }
          else if (index_of_ekran_tmp == INDEX_ML_STPPOCHATKOVYJ_RESURS)
          {
            if ((j < COL_SETPOINT_POCHATKOVYJ_RESURS_BEGIN) ||  (j > COL_SETPOINT_POCHATKOVYJ_RESURS_END ))
              working_ekran[i][j] = ' ';
            else
              calc_int_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, view);
          }
          else if (index_of_ekran_tmp == INDEX_ML_STPKRYTYCHNYJ_RESURS)
          {
            if ((j < COL_SETPOINT_KRYTYCHNYJ_RESURS_BEGIN) ||  (j > COL_SETPOINT_KRYTYCHNYJ_RESURS_END ))
              working_ekran[i][j] = ' ';
            else
              calc_int_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, view);
          }
          else if (index_of_ekran_tmp == INDEX_ML_STPPOCHATKOVA_K_VYMK)
          {
            if ((j < COL_SETPOINT_POCHATKOVA_K_VYMK_BEGIN) ||  (j > COL_SETPOINT_POCHATKOVA_K_VYMK_END ))
              working_ekran[i][j] = ' ';
            else
              calc_int_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, view);
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
    if (current_ekran.index_position == INDEX_ML_STPInom)
    {
      current_ekran.position_cursor_x = COL_SETPOINT_Inom_BEGIN;
      last_position_cursor_x = COL_SETPOINT_Inom_END;
    }
    else if (current_ekran.index_position == INDEX_ML_STPRKS_Inom) 
    {
      current_ekran.position_cursor_x = COL_SETPOINT_RKS_Inom_BEGIN;
      last_position_cursor_x = COL_SETPOINT_RKS_Inom_END;
    }
    else if (current_ekran.index_position == INDEX_ML_STPMInom_vymk) 
    {
      current_ekran.position_cursor_x = COL_SETPOINT_Inom_vymk_BEGIN;
      last_position_cursor_x = COL_SETPOINT_Inom_vymk_END;
    }
    else if (current_ekran.index_position == INDEX_ML_STPRKS_Inom_vymk) 
    {
      current_ekran.position_cursor_x = COL_SETPOINT_RKS_Inom_vymk_BEGIN;
      last_position_cursor_x = COL_SETPOINT_RKS_Inom_vymk_END;
    }
    else if (current_ekran.index_position == INDEX_ML_STPPOCHATKOVYJ_RESURS) 
    {
      current_ekran.position_cursor_x = COL_SETPOINT_POCHATKOVYJ_RESURS_BEGIN;
      last_position_cursor_x = COL_SETPOINT_POCHATKOVYJ_RESURS_END;
    }
    else if (current_ekran.index_position == INDEX_ML_STPKRYTYCHNYJ_RESURS) 
    {
      current_ekran.position_cursor_x = COL_SETPOINT_KRYTYCHNYJ_RESURS_BEGIN;
      last_position_cursor_x = COL_SETPOINT_KRYTYCHNYJ_RESURS_END;
    }
    else if (current_ekran.index_position == INDEX_ML_STPPOCHATKOVA_K_VYMK) 
    {
      current_ekran.position_cursor_x = COL_SETPOINT_POCHATKOVA_K_VYMK_BEGIN;
      last_position_cursor_x = COL_SETPOINT_POCHATKOVA_K_VYMK_END;
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
//Формуємо екран відображення витримок Виключателя
/*****************************************************/
void make_ekran_timeout_switch()
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_TIMEOUT_SWITCH][MAX_COL_LCD] = 
  {
    {
      "     T вкл.     ",
      "    T откл.     ",
      " T удл.блк.вкл. ",
      "  T Привода ВВ  "
    },
    {
      "    T увімк.    ",
      "    T вимк.     ",
      "T под.блк.увімк.",
      "  T Приводу ВВ  "
    },
    {
      "    Close T     ",
      "     Open T     ",
      " Cl CB Blk Len  ",
      "CB Actuator Time"
    },
    {
      "     T вкл.     ",
      "    T откл.     ",
      " T удл.блк.вкл. ",
      "  T Привода ВВ  "
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
    if (index_of_ekran_tmp < MAX_ROW_FOR_TIMEOUT_SWITCH)
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][index_of_ekran_tmp][j];
        if (index_of_ekran_tmp == INDEX_ML_TMOON)
        {
          vaga = 1000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки T вкл.
          if (view == true) value = current_settings.timeout_swch_on; //у змінну value поміщаємо значення витримки T вкл.
          else value = edition_settings.timeout_swch_on;
        }
        else if (index_of_ekran_tmp == INDEX_ML_TMOOFF)
        {
          vaga = 1000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки T відкл.
          if (view == true) value = current_settings.timeout_swch_off; //у змінну value поміщаємо значення витримки T відкл.
          else value = edition_settings.timeout_swch_off;
        }
        else  if (index_of_ekran_tmp == INDEX_ML_TMOUDL_BLK_ON)
        {
          vaga = 10000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки T "удлинение сигнала блокировки включения"
          if (view == true) value = current_settings.timeout_swch_udl_blk_on; //у змінну value поміщаємо значення итримки T "удлинение сигнала блокировки включения"
          else value = edition_settings.timeout_swch_udl_blk_on;
        }
        else  if (index_of_ekran_tmp == INDEX_ML_TMOPRYVODA_VV)
        {
          vaga = 1000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки T "Привода ВВ"
          if (view == true) value = current_settings.timeout_pryvoda_VV; //у змінну value поміщаємо значення итримки T "Привода ВВ"
          else value = edition_settings.timeout_pryvoda_VV;
        }
        first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
      }
      else
      {
        //У парному номері рядку виводимо значення уставки
        for (unsigned int j = 0; j<MAX_COL_LCD; j++)
        {
          if (index_of_ekran_tmp == INDEX_ML_TMOON)
          {
            if (
                ((j < COL_TMO_SWCH_ON_BEGIN) ||  (j > COL_TMO_SWCH_ON_END )) &&
                (j != (COL_TMO_SWCH_ON_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_SWCH_ON_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_SWCH_ON_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_SWCH_ON_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == INDEX_ML_TMOOFF)
          {
            if (
                ((j < COL_TMO_SWCH_OFF_BEGIN) ||  (j > COL_TMO_SWCH_OFF_END )) &&
                (j != (COL_TMO_SWCH_OFF_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_SWCH_OFF_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_SWCH_OFF_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_SWCH_OFF_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == INDEX_ML_TMOUDL_BLK_ON)
          {
            if (
                ((j < COL_TMO_SWCH_UDL_BLK_ON_BEGIN) ||  (j > COL_TMO_SWCH_UDL_BLK_ON_END )) &&
                (j != (COL_TMO_SWCH_UDL_BLK_ON_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_SWCH_UDL_BLK_ON_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_SWCH_UDL_BLK_ON_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_SWCH_UDL_BLK_ON_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == INDEX_ML_TMOPRYVODA_VV)
          {
            if (
                ((j < COL_TMO_PRYVODA_VV_BEGIN) ||  (j > COL_TMO_PRYVODA_VV_END )) &&
                (j != (COL_TMO_PRYVODA_VV_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_PRYVODA_VV_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_PRYVODA_VV_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j,COL_TMO_PRYVODA_VV_COMMA, view, 0);
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
    if (current_ekran.index_position == INDEX_ML_TMOON)
    {
      current_ekran.position_cursor_x = COL_TMO_SWCH_ON_BEGIN;
      last_position_cursor_x = COL_TMO_SWCH_ON_END;
    }
    else if (current_ekran.index_position == INDEX_ML_TMOOFF)
    {
      current_ekran.position_cursor_x = COL_TMO_SWCH_OFF_BEGIN;
      last_position_cursor_x = COL_TMO_SWCH_OFF_END;
    }
    else if (current_ekran.index_position == INDEX_ML_TMOUDL_BLK_ON)
    {
      current_ekran.position_cursor_x = COL_TMO_SWCH_UDL_BLK_ON_BEGIN;
      last_position_cursor_x = COL_TMO_SWCH_UDL_BLK_ON_END;
    }
    else if (current_ekran.index_position == INDEX_ML_TMOPRYVODA_VV)
    {
      current_ekran.position_cursor_x = COL_TMO_PRYVODA_VV_BEGIN;
      last_position_cursor_x = COL_TMO_PRYVODA_VV_END;
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
//Формуємо екран відображення значення управлінської інформації для вимикача
/*****************************************************/
void make_ekran_control_switch()
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_CONTROL_SWITCH][MAX_COL_LCD] = 
  {
    {
      "  Контроль ВВ   ",
      "   Ресурс ВВ    "
    },
    {
      "  Контроль ВВ   ",
      "   Ресурс ВВ    "
    },
    {
      "   CB Control   ",
      "  CB Resource   "
    },
    {
      "  Контроль ВВ   ",
      "   Ресурс ВВ    "
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
    if (index_of_ekran_tmp < MAX_ROW_FOR_CONTROL_SWITCH)
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

        unsigned int temp_data;
          
        if(current_ekran.edition == 0) temp_data = current_settings.control_switch;
        else temp_data = edition_settings.control_switch;
          
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = information_off_on[index_language][(temp_data >> index_ctr) & 0x1][j];
        if (position_temp == index_of_ekran_tmp) current_ekran.position_cursor_x = cursor_x_off_on[index_language][(temp_data >> index_ctr) & 0x1];
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
