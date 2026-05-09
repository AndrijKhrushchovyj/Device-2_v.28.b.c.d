#include "header.h"

/*****************************************************/
//Вираховуваня цілого символу і поміщення його в робочий екран
/*****************************************************/
void calc_int_symbol_and_put_into_working_ekran(unsigned char *point_in_working_ekran, unsigned int *point_value, unsigned int *point_vaga, unsigned int *point_first_symbol, unsigned int view)
{
  unsigned int temp_data;
  temp_data = (*point_value) / (*point_vaga); //виділяємо число, яке треба перетворити у символ і помістити у дану позицію екрану
  *point_value %= *(point_vaga);              //вираховуємо число без символа, який ми зараз будемо виводити на екран
  *point_vaga /= 10;                          //зменшуємо ваговий коефіцієнт в 10 разів
  if (view == false)
    *point_in_working_ekran = temp_data + 0x30;
  else
  {
    //У випадку, якщо ми не у режимі редагування, то нулі перед найстаршим значущим числом приховуємо
    if ((temp_data != 0) || ((*point_first_symbol) != 0))
    {
      *point_in_working_ekran = temp_data + 0x30;
      if ((*point_first_symbol) == 0)
        *point_first_symbol = 1;
    }
    else
    {
      //Нуль виводимо тільки у тому випадку, якщо це є символ одиниць числа (текуча вага числа рівна 1)
      if ((*point_vaga) >= 1)
        *point_in_working_ekran = ' ';
      else
      {
        *point_in_working_ekran = temp_data + 0x30;
        if ((*point_first_symbol) == 0)
          *point_first_symbol = 1;
      }
    }
  }
}
/*****************************************************/

/*****************************************************/
//Формуємо екран відображення коефіцієнтів трансформації
/*****************************************************/
void make_ekran_transformator()
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_TRANSFORMATOR_INFO][MAX_COL_LCD] =
    {
      {"Коэфф.трансф.ТТ1",
       "Коэфф.трансф.ТТ2",
       "Коэфф.трансф.ТН1",
       "Коэфф.трансф.ТН2"},
      {" Коеф.трансф.ТТ1",
       " Коеф.трансф.ТТ2",
       " Коеф.трансф.ТН1",
       " Коеф.трансф.ТН2"},
      {"   CT Ratio1    ",
       "   CT Ratio2    ",
       "   VT Ratio1    ",
       "   VT Ratio2    "},
      {"Коэфф.трансф.ТТ1",
       "Коэфф.трансф.ТТ2",
       "Коэфф.трансф.ТН1",
       "Коэфф.трансф.ТН2"}};
  unsigned char name_string_tmp[MAX_ROW_FOR_TRANSFORMATOR_INFO][MAX_COL_LCD];

  int const index_language = index_language_in_array(current_settings.language);
  for (int index_1 = 0; index_1 < MAX_ROW_FOR_TRANSFORMATOR_INFO; index_1++)
  {
    for (int index_2 = 0; index_2 < MAX_COL_LCD; index_2++)
      name_string_tmp[index_1][index_2] = name_string[index_language][index_1][index_2];
  }

  int additional_current = 0;
  int position_temp = current_ekran.index_position;
  int shift[MAX_ROW_FOR_TRANSFORMATOR_INFO] = {0};
  /******************************************/
  //Виключаємо поля, які не треба відображати
  /******************************************/
  //"Вимірювання ТН2"
  /*
  Не відображаємо "Вимірювання ТН2" у тому випадку, якщо у конфігурації РПН
  вибрано алу настройка стоїть на 2-обмотковий трансформатор
  
  Інакше відображаємо вимірювання як з ТН1, так і з ТН2 (навіть якщо РПН виведено
  з конфігурації, бо тоді, на мою думку, буде неможливо побачити вибір 2-обмю/3-обм.
  трансформатор і виникне питання .чого в одному випідку ці вимірюванні відображаються,
  а у іншому ні).
  */
  if (
    ((current_settings.configuration & (1u << RPN_BIT_CONFIGURATION)) != 0) &&
    ((current_settings.control_rpn & MASKA_FOR_BIT(INDEX_ML_CTRRPN_TRANSF)) == 0))
  {
    while (additional_current < 2)
    {
      int index_deleted_feild;

      if (additional_current == 0)
        index_deleted_feild = INDEX_ML_TT2;
      else
        index_deleted_feild = INDEX_ML_TN2;

      int i = index_deleted_feild - additional_current;

      if ((i + 1) <= position_temp)
        position_temp--;
      do
      {
        for (unsigned int j = 0; j < MAX_COL_LCD; j++)
        {
          if ((i + 1) < MAX_ROW_FOR_TRANSFORMATOR_INFO)
            name_string_tmp[i][j] = name_string_tmp[i + 1][j];
          else
            name_string_tmp[i][j] = ' ';
        }
        shift[i] += 1;
        i++;
      } while (i < (MAX_ROW_FOR_TRANSFORMATOR_INFO - additional_current));
      additional_current++;
    }
  }

  unsigned int vaga, value, first_symbol;

  //Множення на два величини position_temp потрібне для того, бо наодн позицію ми використовуємо два рядки (назва + значення)
  unsigned int index_of_ekran = ((position_temp << 1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
  {
    int index_of_ekran_tmp = index_of_ekran >> 1;
    unsigned int view = ((current_ekran.edition == 0) || (position_temp != index_of_ekran_tmp));
    if (index_of_ekran_tmp < (MAX_ROW_FOR_TRANSFORMATOR_INFO - additional_current))
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j < MAX_COL_LCD; j++)
          working_ekran[i][j] = name_string_tmp[index_of_ekran_tmp][j];
        if ((index_of_ekran_tmp + shift[index_of_ekran_tmp]) == INDEX_ML_TT1)
        {
          vaga = 100; //максимальний ваговий коефіцієнт для коефіцієнта трансформації TC1
          if (view == true)
            value = current_settings.TCurrent1; //у змінну value поміщаємо значення коефіцієнта трансформації TC1
          else
            value = edition_settings.TCurrent1;
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
        else if ((index_of_ekran_tmp + shift[index_of_ekran_tmp]) == INDEX_ML_TT2)
        {
          vaga = 100; //максимальний ваговий коефіцієнт для коефіцієнта трансформації TC2
          if (view == true)
            value = current_settings.TCurrent2; //у змінну value поміщаємо значення коефіцієнта трансформації TC2
          else
            value = edition_settings.TCurrent2;
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
        else if ((index_of_ekran_tmp + shift[index_of_ekran_tmp]) == INDEX_ML_TN1)
        {
          vaga = 100; //максимальний ваговий коефіцієнт для коефіцієнта трансформації TН1
          if (view == true)
            value = current_settings.TVoltage1; //у змінну value поміщаємо значення коефіцієнта трансформації TН1
          else
            value = edition_settings.TVoltage1;
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
        else if ((index_of_ekran_tmp + shift[index_of_ekran_tmp]) == INDEX_ML_TN2)
        {
          vaga = 100; //максимальний ваговий коефіцієнт для коефіцієнта трансформації TН2
          if (view == true)
            value = current_settings.TVoltage2; //у змінну value поміщаємо значення коефіцієнта трансформації TН2
          else
            value = edition_settings.TVoltage2;
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
      }
      else
      {
        //У парному номері рядку виводимо значення уставки
        for (unsigned int j = 0; j < MAX_COL_LCD; j++)
        {
          if (
            ((index_of_ekran_tmp + shift[index_of_ekran_tmp]) == INDEX_ML_TT1) ||
            ((index_of_ekran_tmp + shift[index_of_ekran_tmp]) == INDEX_ML_TT2))
          {
            if ((j < COL_TT_BEGIN) || (j > COL_TT_END))
              working_ekran[i][j] = ' ';
            else
              calc_int_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, view);
          }
          else
          {
            if ((j < COL_TN_BEGIN) || (j > COL_TN_END))
              working_ekran[i][j] = ' ';
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
    if (
      (current_ekran.index_position == INDEX_ML_TT1) ||
      (current_ekran.index_position == INDEX_ML_TT2))
    {
      current_ekran.position_cursor_x = COL_TT_BEGIN;
      last_position_cursor_x = COL_TT_END;
    }
    else
    {
      current_ekran.position_cursor_x = COL_TN_BEGIN;
      last_position_cursor_x = COL_TN_END;
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
//
/*****************************************************/
/*****************************************************/
