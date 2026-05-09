#include "header.h"

/*****************************************************/
//Формуємо екран всіх настройок для контролю положення РПН
/*****************************************************/
void make_ekran_chose_settings_type_control_location(void)
{
  const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_CHOSE_SETTINGS_CONTROL_LOCATION][MAX_COL_LCD] =
    {
      {" Уставки        ",
       " Тип контр.пол. ",
       " Калибр.СУП РПН "},
      {" Уставки        ",
       " Тип контр.пол. ",
       " Калібр.СВП РПН "},
      {" Pickups        ",
       " Pos.Ctrl.Type  ",
       "OLTC PSI Calibr."},
      {" Баскару        ",
       " Тип контр.пол. ",
       " Калибр.СУП РПН "}};
  unsigned char name_string_tmp[MAX_ROW_FOR_CHOSE_SETTINGS_CONTROL_LOCATION][MAX_COL_LCD];

  int const index_language = index_language_in_array(current_settings.language);
  for (int index_1 = 0; index_1 < MAX_ROW_FOR_CHOSE_SETTINGS_CONTROL_LOCATION; index_1++)
  {
    for (int index_2 = 0; index_2 < MAX_COL_LCD; index_2++)
      name_string_tmp[index_1][index_2] = name_string[index_language][index_1][index_2];
  }

  unsigned int additional_current = 0;
  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;

  /******************************************/
  //Виключаємо поля, які не треба відображати
  /******************************************/
  //Калібр.СУП РПН
  if (current_settings.type_control_location != 2)
  {
    unsigned int i = INDEX_ML_CALIBRATION_SELSYN - additional_current;

    if ((i + 1) <= position_temp)
      position_temp--;
    do
    {
      for (unsigned int j = 0; j < MAX_COL_LCD; j++)
      {
        if ((i + 1) < MAX_ROW_FOR_CHOSE_SETTINGS_CONTROL_LOCATION)
          name_string_tmp[i][j] = name_string_tmp[i + 1][j];
        else
          name_string_tmp[i][j] = ' ';
      }
      i++;
    } while (i < (MAX_ROW_FOR_CHOSE_SETTINGS_CONTROL_LOCATION - additional_current));
    additional_current++;
  }
  /******************************************/

  index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  //Копіюємо  рядки у робочий екран
  for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
  {
    //Наступні рядки треба перевірити, чи їх требе відображати у текучій кофігурації
    if (index_of_ekran < (MAX_ROW_FOR_CHOSE_SETTINGS_CONTROL_LOCATION - additional_current))
      for (unsigned int j = 0; j < MAX_COL_LCD; j++)
        working_ekran[i][j] = name_string_tmp[index_of_ekran][j];
    else
      for (unsigned int j = 0; j < MAX_COL_LCD; j++)
        working_ekran[i][j] = ' ';

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
//Формуємо екран відображення кількості ступенів РПН
/*****************************************************/
void make_ekran_setpoints_control_location(void)
{
  const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_STP_CONTROL_LOCATION][MAX_COL_LCD] =
    {
      {"Кол.ступеней РПН",
       "  Ном.пол.РПН   ",
       " Текущее пол.РПН"},
      {"Кіл.ступенів РПН",
       "  Ном.поз.РПН   ",
       " Поточна поз.РПН"},
      {" OLTC Pos.Number",
       " OLTC Rated Pos.",
       " OLTC Curr.Pos. "},
      {"Кол.ступеней РПН",
       "  Ном.пол.РПН   ",
       " Текущее пол.РПН"}};
  const unsigned char undefined[MAX_NAMBER_LANGUAGE][MAX_COL_LCD] =
    {
      "    Неопред.    ",
      "   Невизнач.    ",
      "   Undefined    ",
      "    Неопред.    "};
  const unsigned char undefined_word[MAX_NAMBER_LANGUAGE] = {4, 3, 3, 4};
  int const index_language = index_language_in_array(current_settings.language);

  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;

  unsigned int vaga, value = 0, first_symbol;

  //Множення на два величини position_temp потрібне для того, бо наодн позицію ми використовуємо два рядки (назва + значення)
  index_of_ekran = ((position_temp << 1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
  {
    unsigned int index_of_ekran_tmp = index_of_ekran >> 1;
    unsigned int view = ((current_ekran.edition == 0) || (position_temp != index_of_ekran_tmp));

    if (index_of_ekran_tmp < MAX_ROW_FOR_STP_CONTROL_LOCATION)
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок

        for (unsigned int j = 0; j < MAX_COL_LCD; j++)
          working_ekran[i][j] = name_string[index_language][index_of_ekran >> 1][j];

        if (index_of_ekran_tmp == INDEX_ML_STP_TYPE_CTR_LOC_NUMBER_TOTAL)
        {
          vaga = 10; //максимальний ваговий коефіцієнт
          if (view == true)
            value = current_settings.number_steps_rpn; //у змінну value поміщаємо значення кількості ступенів РПН
          else
            value = edition_settings.number_steps_rpn;
        }
        else if (index_of_ekran_tmp == INDEX_ML_STP_TYPE_CTR_LOC_NUMBER_NOMINAL)
        {
          vaga = 10; //максимальний ваговий коефіцієнт
          if (view == true)
            value = current_settings.number_step_nominal_rpn; //у змінну value поміщаємо значення номер номінальної позиції РПН
          else
            value = edition_settings.number_step_nominal_rpn;
        }
        else if (index_of_ekran_tmp == INDEX_ML_STP_TYPE_CTR_LOC_NUMBER_CURRENT)
        {
          vaga = 10; //максимальний ваговий коефіцієнт
          if (view == true)
            value = current_step_logical; //у змінну value поміщаємо текучу позицію РПН
          else
            value = edition_current_step_logical;
        }

        first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
      }
      else
      {
        //У парному номері рядку виводимо значення уставки
        for (unsigned int j = 0; j < MAX_COL_LCD; j++)
        {
          if (
            (index_of_ekran_tmp == INDEX_ML_STP_TYPE_CTR_LOC_NUMBER_CURRENT) &&
            (((int) value) < 0))
          {
            working_ekran[i][j] = undefined[index_language][j];
          }
          else
          {
            if ((j < COL_NUMBER_STEPS_RPN_BEGIN) || (j > COL_NUMBER_STEPS_RPN_END))
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
    int last_position_cursor_x = COL_NUMBER_STEPS_RPN_END;
    if (
      (current_ekran.index_position == INDEX_ML_STP_TYPE_CTR_LOC_NUMBER_CURRENT) &&
      (edition_current_step_logical < 0))
    {
      current_ekran.position_cursor_x = undefined_word[index_language];
    }
    else
    {
      current_ekran.position_cursor_x = COL_NUMBER_STEPS_RPN_BEGIN;
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

  //Курсор видимий, якщо ми у режимі редагування
  // if (current_ekran.edition == 0)
  //   current_ekran.cursor_on = 0;
  // else
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
//Формуємо екран відображення типу контролю положення РПН
/*****************************************************/
void make_ekran_type_control_location(void)
{
  const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_TYPE_CONTROL_LOCATION][MAX_COL_LCD] =
    {
      {" Контр.положения"},
      {" Контроль стану "},
      {"  Pos.Checking  "},
      {" Контр.положения"}};
  int const index_language = index_language_in_array(current_settings.language);

  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;

  //Множення на два величини position_temp потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
  index_of_ekran = ((position_temp << 1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
  {
    unsigned int const index_of_ekran_tmp = index_of_ekran >> 1;
    if (index_of_ekran_tmp < MAX_ROW_FOR_TYPE_CONTROL_LOCATION)
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
        const unsigned char information[MAX_NAMBER_LANGUAGE][MAX_TYPE_CONTROL_LOCATION + 1][MAX_COL_LCD] =
          {
            {"   Логический   ", "    Логометр    ", "    Сельсин     ", "     Ошибка     "},
            {"    Логічний    ", "    Логометр    ", "    Сельсин     ", "    Помилка     "},
            {"     Logic      ", "   Logometer    ", "     Selsyn     ", "     Error      "},
            {"   Логический   ", "    Логометр    ", "    Сельсин     ", "     Ошибка     "}};
        const unsigned int cursor_x[MAX_NAMBER_LANGUAGE][MAX_TYPE_CONTROL_LOCATION + 1] =
          {
            {2, 3, 3, 4},
            {3, 3, 3, 3},
            {4, 2, 4, 4},
            {2, 3, 3, 4}};

        unsigned int temp_data;
        if (current_ekran.edition == 0)
          temp_data = current_settings.type_control_location;
        else
          temp_data = edition_settings.type_control_location;

        if (temp_data < MAX_TYPE_CONTROL_LOCATION)
        {
          for (unsigned int j = 0; j < MAX_COL_LCD; j++)
            working_ekran[i][j] = information[index_language][temp_data][j];
          current_ekran.position_cursor_x = cursor_x[index_language][temp_data];
        }
        else
        {
          for (unsigned int j = 0; j < MAX_COL_LCD; j++)
            working_ekran[i][j] = information[index_language][MAX_TYPE_CONTROL_LOCATION][j];
          current_ekran.position_cursor_x = cursor_x[index_language][MAX_TYPE_CONTROL_LOCATION];
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

  //Курсор видимий, якщо ми у режимі редагування
  if (current_ekran.edition == 0)
    current_ekran.cursor_on = 0;
  else
    current_ekran.cursor_on = 1;

  //Курсор не мигає, якщо ми у режимі редагування
  if (current_ekran.edition == 0)
    current_ekran.cursor_blinking_on = 0;
  else
    current_ekran.cursor_blinking_on = 1;

  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//Формуємо екран калібрування сельинового датчика
/*****************************************************/
void make_ekran_calibration_selsyn(void)
{
  unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_CALIBRATION_SELSYN][MAX_COL_LCD] =
    {
      {" Заф.пол.N 1 РПН",
       " Заф.пол.N?? РПН"},
      {" Заф.пол.N 1 РПН",
       " Заф.пол.N?? РПН"},
      {"OLTC Fix Pos.N 1",
       "OLTC Fix Pos.N??"},
      {" Заф.пол.N 1 РПН",
       " Заф.пол.N?? РПН"}};
  const unsigned int first_index_number[MAX_NAMBER_LANGUAGE] = {10, 10, 14, 10};
  int const index_language = index_language_in_array(current_settings.language);

  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;

  index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  //Копіюємо  рядки у робочий екран
  for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
  {
    //Наступні рядки треба перевірити, чи їх требе відображати у текучій кофігурації
    if (index_of_ekran < MAX_ROW_FOR_CALIBRATION_SELSYN)
    {
      unsigned int first_index_number_1 = first_index_number[index_language];
      for (unsigned int j = 0; j < MAX_COL_LCD; j++)
      {
        if (
          (index_of_ekran == INDEX_ML_FIRST_LOCATION_SELSYN) ||
          (j < first_index_number_1) ||
          (j > (first_index_number_1 + 1)))
          working_ekran[i][j] = name_string[index_language][index_of_ekran][j];
        else
        {
          unsigned int number_steps_rpn_tmp = current_settings.number_steps_rpn;
          if (j == first_index_number_1)
          {
            if (number_steps_rpn_tmp < 10)
            {
              working_ekran[i][j] = ' ';
            }
            else
            {
              working_ekran[i][j] = (number_steps_rpn_tmp / 10) + 0x30;
            }
          }
          else
          {
            if (number_steps_rpn_tmp < 10)
            {
              working_ekran[i][j] = number_steps_rpn_tmp + 0x30;
            }
            else
            {
              working_ekran[i][j] = (number_steps_rpn_tmp % 10) + 0x30;
            }
          }
        }
      }
    }
    else
      for (unsigned int j = 0; j < MAX_COL_LCD; j++)
        working_ekran[i][j] = ' ';

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
