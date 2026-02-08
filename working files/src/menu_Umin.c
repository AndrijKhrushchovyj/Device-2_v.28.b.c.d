#include "header.h"

/*****************************************************/
//Формуємо екран відображення уставок Umin
/*****************************************************/
void make_ekran_setpoint_Umin(unsigned int group)
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_SETPOINT_UMIN][MAX_COL_LCD] =
    {
      {" Уставка ЗНмин1 ",
       " Уставка ЗНмин2 "},
      {" Уставка ЗНмін1 ",
       " Уставка ЗНмін2 "},
      {"  UVP1 Pickup   ",
       "  UVP2 Pickup   "},
      {" Уставка ЗНмин1 ",
       " Уставка ЗНмин2 "}};
  int const index_language = index_language_in_array(current_settings.language);

  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  unsigned int vaga, value, first_symbol;

  //Множення на два величини position_temp потрібне для того, бо наодн позицію ми використовуємо два рядки (назва + значення)
  index_of_ekran = ((position_temp << 1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
  {
    unsigned int index_of_ekran_tmp = index_of_ekran >> 1;
    unsigned int view = ((current_ekran.edition == 0) || (position_temp != index_of_ekran_tmp));
    if (index_of_ekran_tmp < MAX_ROW_FOR_SETPOINT_UMIN)
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j < MAX_COL_LCD; j++)
          working_ekran[i][j] = name_string[index_language][index_of_ekran_tmp][j];
        if (index_of_ekran_tmp == INDEX_ML_STPUmin_Umin1)
        {
          vaga = 100000 / 1000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для уставки "Уставка ЗНмін1"
          if (view == true)
            value = current_settings.setpoint_Umin1[group]; //у змінну value поміщаємо значення уставки Umin1
          else
            value = edition_settings.setpoint_Umin1[group];

          value /= 1000; //Відображаємо тільки ціоу частину, а витримка у пам'яті зберігається з точністю до мілісекунд
        }
        else if (index_of_ekran_tmp == INDEX_ML_STPUmin_Umin2)
        {
          vaga = 100000 / 1000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для уставки "Уставка ЗНмін2"
          if (view == true)
            value = current_settings.setpoint_Umin2[group]; //у змінну value поміщаємо значення уставки блокування Umin1 по струму
          else
            value = edition_settings.setpoint_Umin2[group];

          value /= 1000; //Відображаємо тільки ціоу частину, а витримка у пам'яті зберігається з точністю до мілісекунд
        }
        first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
      }
      else
      {
        //У парному номері рядку виводимо значення уставки
        for (unsigned int j = 0; j < MAX_COL_LCD; j++)
        {
          if (index_of_ekran_tmp == INDEX_ML_STPUmin_Umin1)
          {
            if (
              ((j < COL_SETPOINT_Umin_Umin1_BEGIN) || (j > COL_SETPOINT_Umin_Umin1_END)) &&
              (j != (COL_SETPOINT_Umin_Umin1_END + 2)))
              working_ekran[i][j] = ' ';
            else if (j == (COL_SETPOINT_Umin_Umin1_END + 2))
              working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_V];
            else
              calc_int_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, view);
          }
          else if (index_of_ekran_tmp == INDEX_ML_STPUmin_Umin2)
          {
            if (
              ((j < COL_SETPOINT_Umin_Umin2_BEGIN) || (j > COL_SETPOINT_Umin_Umin2_END)) &&
              (j != (COL_SETPOINT_Umin_Umin2_END + 2)))
              working_ekran[i][j] = ' ';
            else if (j == (COL_SETPOINT_Umin_Umin2_END + 2))
              working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_V];
            else
              calc_int_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, view);
          }
        }
      }
    }
    else
      for (unsigned int j = 0; j < MAX_COL_LCD; j++)
        working_ekran[i][j] = ' ';

    index_of_ekran++;
  }

  //Відображення курору по вертикалі і курсор завжди має бути у полі із значенням устаки
  current_ekran.position_cursor_y = ((position_temp << 1) + 1) & (MAX_ROW_LCD - 1);
  //Курсор по горизонталі відображається на першому символі у випадку, коли ми не в режимі редагування, інакше позиція буде визначена у функцї main_manu_function
  if (current_ekran.edition == 0)
  {
    int last_position_cursor_x = MAX_COL_LCD;
    if (current_ekran.index_position == INDEX_ML_STPUmin_Umin1)
    {
      current_ekran.position_cursor_x = COL_SETPOINT_Umin_Umin1_BEGIN;
      last_position_cursor_x = COL_SETPOINT_Umin_Umin1_END;
    }
    else if (current_ekran.index_position == INDEX_ML_STPUmin_Umin2)
    {
      current_ekran.position_cursor_x = COL_SETPOINT_Umin_Umin2_BEGIN;
      last_position_cursor_x = COL_SETPOINT_Umin_Umin2_END;
    }

    //Підтягуємо курсор до першого символу
    while (((working_ekran[current_ekran.position_cursor_y][current_ekran.position_cursor_x + 1]) == ' ') &&
           (current_ekran.position_cursor_x < (last_position_cursor_x - 1)))
      current_ekran.position_cursor_x++;

    //Курсор ставимо так, щоб він був перед числом
    if (((working_ekran[current_ekran.position_cursor_y][current_ekran.position_cursor_x]) != ' ') &&
        (current_ekran.position_cursor_x > 0))
      current_ekran.position_cursor_x--;
  }
  //Курсор видимий
  current_ekran.cursor_on = 1;
  //Курсор не мигає
  if (current_ekran.edition == 0)
    current_ekran.cursor_blinking_on = 0;
  else
    current_ekran.cursor_blinking_on = 1;
  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//Формуємо екран відображення витримок Umin
/*****************************************************/
void make_ekran_timeout_Umin(unsigned int group)
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_TIMEOUT_UMIN][MAX_COL_LCD] =
    {
      {" Выдержка ЗНмин1",
       " Выдержка ЗНмин2"},
      {" Витримка ЗНмін1",
       " Витримка ЗНмін2"},
      {"   UVP1 Delay   ",
       "   UVP2 Delay   "},
      {" Выдержка ЗНмин1",
       " Выдержка ЗНмин2"}};
  int const index_language = index_language_in_array(current_settings.language);

  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  unsigned int vaga, value, first_symbol;

  //Множення на два величини position_temp потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
  index_of_ekran = ((position_temp << 1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
  {
    unsigned int index_of_ekran_tmp = index_of_ekran >> 1;
    unsigned int view = ((current_ekran.edition == 0) || (position_temp != index_of_ekran_tmp));
    if (index_of_ekran_tmp < MAX_ROW_FOR_TIMEOUT_UMIN)
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j < MAX_COL_LCD; j++)
          working_ekran[i][j] = name_string[index_language][index_of_ekran_tmp][j];
        if (index_of_ekran_tmp == INDEX_ML_TMOUmin_Umin1)
        {
          vaga = 10000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки Umin1
          if (view == true)
            value = current_settings.timeout_Umin1[group]; //у змінну value поміщаємо значення витримки Umin1
          else
            value = edition_settings.timeout_Umin1[group];
        }
        else if (index_of_ekran_tmp == INDEX_ML_TMOUmin_Umin2)
        {
          vaga = 10000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки Umin2
          if (view == true)
            value = current_settings.timeout_Umin2[group]; //у змінну value поміщаємо значення витримки Umin2
          else
            value = edition_settings.timeout_Umin2[group];
        }
        first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
      }
      else
      {
        //У парному номері рядку виводимо значення уставки
        for (unsigned int j = 0; j < MAX_COL_LCD; j++)
        {
          if (index_of_ekran_tmp == INDEX_ML_TMOUmin_Umin1)
          {
            if (
              ((j < COL_TMO_Umin_Umin1_BEGIN) || (j > COL_TMO_Umin_Umin1_END)) &&
              (j != (COL_TMO_Umin_Umin1_END + 2)))
              working_ekran[i][j] = ' ';
            else if (j == COL_TMO_Umin_Umin1_COMMA)
              working_ekran[i][j] = ',';
            else if (j == (COL_TMO_Umin_Umin1_END + 2))
              working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_Umin_Umin1_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == INDEX_ML_TMOUmin_Umin2)
          {
            if (
              ((j < COL_TMO_Umin_Umin2_BEGIN) || (j > COL_TMO_Umin_Umin2_END)) &&
              (j != (COL_TMO_Umin_Umin2_END + 2)))
              working_ekran[i][j] = ' ';
            else if (j == COL_TMO_Umin_Umin2_COMMA)
              working_ekran[i][j] = ',';
            else if (j == (COL_TMO_Umin_Umin2_END + 2))
              working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_Umin_Umin2_COMMA, view, 0);
          }
        }
      }
    }
    else
      for (unsigned int j = 0; j < MAX_COL_LCD; j++)
        working_ekran[i][j] = ' ';

    index_of_ekran++;
  }

  //Відображення курору по вертикалі і курсор завжди має бути у полі із значенням устаки
  current_ekran.position_cursor_y = ((position_temp << 1) + 1) & (MAX_ROW_LCD - 1);
  //Курсор по горизонталі відображається на першому символі у випадку, коли ми не в режимі редагування, інакше позиція буде визначена у функцї main_manu_function
  if (current_ekran.edition == 0)
  {
    int last_position_cursor_x = MAX_COL_LCD;
    if (current_ekran.index_position == INDEX_ML_TMOUmin_Umin1)
    {
      current_ekran.position_cursor_x = COL_TMO_Umin_Umin1_BEGIN;
      last_position_cursor_x = COL_TMO_Umin_Umin1_END;
    }
    else if (current_ekran.index_position == INDEX_ML_TMOUmin_Umin2)
    {
      current_ekran.position_cursor_x = COL_TMO_Umin_Umin2_BEGIN;
      last_position_cursor_x = COL_TMO_Umin_Umin2_END;
    }

    //Підтягуємо курсор до першого символу
    while (((working_ekran[current_ekran.position_cursor_y][current_ekran.position_cursor_x + 1]) == ' ') &&
           (current_ekran.position_cursor_x < (last_position_cursor_x - 1)))
      current_ekran.position_cursor_x++;

    //Курсор ставимо так, щоб він був перед числом
    if (((working_ekran[current_ekran.position_cursor_y][current_ekran.position_cursor_x]) != ' ') &&
        (current_ekran.position_cursor_x > 0))
      current_ekran.position_cursor_x--;
  }
  //Курсор видимий
  current_ekran.cursor_on = 1;
  //Курсор не мигає
  if (current_ekran.edition == 0)
    current_ekran.cursor_blinking_on = 0;
  else
    current_ekran.cursor_blinking_on = 1;
  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//Формуємо екран відображення значення управлінської інформації для Umin
/*****************************************************/
void make_ekran_control_Umin()
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_CONTROL_UMIN][MAX_COL_LCD] =
    {
      {"    ЗНмин1      ",
       "    ЗНмин2      "},
      {"    ЗНмін1      ",
       "    ЗНмін2      "},
      {"      UVP1      ",
       "      UVP2      "},
      {"    ЗНмин1      ",
       "    ЗНмин2      "}};
  int const index_language = index_language_in_array(current_settings.language);

  int position_temp = current_ekran.index_position;
  int index_of_ekran;

  //Множення на два величини position_temp потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
  index_of_ekran = ((position_temp << 1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
  {
    unsigned int index_of_ekran_tmp = index_of_ekran >> 1;
    if (index_of_ekran_tmp < MAX_ROW_FOR_CONTROL_UMIN)
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j < MAX_COL_LCD; j++)
          working_ekran[i][j] = name_string[index_language][index_of_ekran_tmp][j];
      }
      else
      {
        unsigned int index_ctr = index_of_ekran_tmp;

        unsigned int temp_data;
        if (current_ekran.edition == 0)
          temp_data = current_settings.control_Umin;
        else
          temp_data = edition_settings.control_Umin;

        for (unsigned int j = 0; j < MAX_COL_LCD; j++)
          working_ekran[i][j] = information_off_on[index_language][(temp_data >> index_ctr) & 0x1][j];
        if (position_temp == index_of_ekran_tmp)
          current_ekran.position_cursor_x = cursor_x_off_on[index_language][(temp_data >> index_ctr) & 0x1];
      }
    }
    else
      for (unsigned int j = 0; j < MAX_COL_LCD; j++)
        working_ekran[i][j] = ' ';

    index_of_ekran++;
  }

  //Відображення курору по вертикалі і курсор завжди має бути у полі із значенням устаки
  current_ekran.position_cursor_y = ((position_temp << 1) + 1) & (MAX_ROW_LCD - 1);
  //Курсор видимий
  current_ekran.cursor_on = 1;
  //Курсор не мигає
  if (current_ekran.edition == 0)
    current_ekran.cursor_blinking_on = 0;
  else
    current_ekran.cursor_blinking_on = 1;
  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//
/*****************************************************/
/*****************************************************/
