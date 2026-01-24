#include "header.h"

/*****************************************************/
//Формуємо екран відображення витримок ЗНХ
/*****************************************************/
void make_ekran_timeout_znkh(unsigned int group)
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_TIMEOUT_ZNKh][MAX_COL_LCD] =
    {
      {"Время перекл.РПН"},
      {" Час перемик.РПН"},
      {" OLTC Switch.T. "},
      {"Время перекл.РПН"}};
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
    if (index_of_ekran_tmp < MAX_ROW_FOR_TIMEOUT_ZNKh)
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j < MAX_COL_LCD; j++)
          working_ekran[i][j] = name_string[index_language][index_of_ekran_tmp][j];
        vaga = 10000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
        if (view == true)
          value = current_settings.timeout_znkh_perekl[group]; //у змінну value поміщаємо значення витримки
        else
          value = edition_settings.timeout_znkh_perekl[group];
        first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
      }
      else
      {
        //У парному номері рядку виводимо значення уставки
        for (unsigned int j = 0; j < MAX_COL_LCD; j++)
        {
          if (
            ((j < COL_TMO_ZNKh_PEREKL_BEGIN) || (j > COL_TMO_ZNKh_PEREKL_END)) &&
            (j != (COL_TMO_ZNKh_PEREKL_END + 2)))
            working_ekran[i][j] = ' ';
          else if (j == COL_TMO_ZNKh_PEREKL_COMMA)
            working_ekran[i][j] = ',';
          else if (j == (COL_TMO_ZNKh_PEREKL_END + 2))
            working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
          else
            calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_ZNKh_PEREKL_COMMA, view, 0);
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
    current_ekran.position_cursor_x = COL_TMO_ZNKh_PEREKL_BEGIN;
    int last_position_cursor_x = COL_TMO_ZNKh_PEREKL_END;

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
//Формуємо екран відображення значення управлінської інформації для ЗНХ
/*****************************************************/
void make_ekran_control_znkh()
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_CONTROL_ZNKh][MAX_COL_LCD] =
    {
      {"      ЗНХ       "},
      {"      ЗБХ       "},
      {"      CMP       "},
      {"      ЗНХ       "}};

  int const index_language = index_language_in_array(current_settings.language);

  unsigned int position_temp = current_ekran.index_position;
  //Множення на два величини position_temp потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
  unsigned int index_of_ekran = ((position_temp << 1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
  {
    int index_of_ekran_tmp = index_of_ekran >> 1;
    if (index_of_ekran_tmp < MAX_ROW_FOR_CONTROL_ZNKh) //Множення на два константи MAX_ROW_FOR_CONTROL_ZNKh потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j < MAX_COL_LCD; j++)
          working_ekran[i][j] = name_string[index_language][index_of_ekran_tmp][j];
      }
      else
      {
        //У парному номері рядку виводимо значення уставки
        unsigned int const index_ctr = index_of_ekran_tmp;

        unsigned int const temp_data = (current_ekran.edition == 0) ? current_settings.control_znkh : edition_settings.control_znkh;

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
