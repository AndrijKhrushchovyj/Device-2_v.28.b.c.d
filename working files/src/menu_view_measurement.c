#include "header.h"

/*****************************************************/
//Конвертація в рядок і поміщення в масив для відображення на екрані вимірювань
/*****************************************************/
void convert_and_insert_char_for_measurement(unsigned int start_number_digit_after_point, unsigned int temp_meas, unsigned int koef_mul, unsigned int koef_div, unsigned char *name_string, unsigned int start_position)
{
  unsigned long long temp_value_long = ((unsigned long long) temp_meas) * ((unsigned long long) koef_mul) / ((unsigned long long) koef_div);
  unsigned int temp_value = (unsigned int) temp_value_long;
  unsigned int number_digit_after_point = start_number_digit_after_point, error = 0, overflow = 0;

  //Помічаємо, що зараз ми ще не перейшли ні на шкалу КІЛО ні на шкалу МЕГА
  //  *(name_string + start_position + 7) = ' ';

  overflow = ((temp_value_long & 0xffffffff00000000ull) != 0);
  while (
    (
      (overflow != 0) ||
      (temp_value > 999999)) &&
    (error == 0))
  {
    //Ми не вкладаємося у значення з шести цифр і коми

    //Відкидаємо найменш значуще число
    if (overflow != 0)
    {
      temp_value_long /= 10ull;
      overflow = ((temp_value_long & 0xffffffff00000000ull) != 0);
      if (overflow == 0)
        temp_value = temp_value_long;
    }
    else
      temp_value /= 10;

    //Змінюємо позицію десяткової коми і, при потрребі, переходимо на іншу шкалу
    switch (number_digit_after_point)
    {
      case 1:
      case 2:
        {
          //Переходимо з формату 1.5 до 2.4 або з формату 2.4 до 3.3
          number_digit_after_point++;
          break;
        }
      case 3:
        {
          //Переходимо з формату 3.3 до 1.5 із встановленням позначки ' '->'k' або 'k'->'M'
          number_digit_after_point = 1;
          if ((*(name_string + start_position + 7)) == ' ')
          {
            if (current_settings.language == LANGUAGE_EN)
              *(name_string + start_position + 7) = 'k';
            else
              *(name_string + start_position + 7) = 'к';
          }
          else if (
            ((*(name_string + start_position + 7)) == 'к') ||
            ((*(name_string + start_position + 7)) == 'k'))
          {
            *(name_string + start_position + 7) = 'М';
          }
          else if ((*(name_string + start_position + 7)) == 'М')
          {
            if (current_settings.language == LANGUAGE_EN)
              *(name_string + start_position + 7) = 'G';
            else
              *(name_string + start_position + 7) = 'Г';
          }
          else
          {
            /*
          теоретично сюди б програма могла зайти у двох випадках:
          1) перехід з шкали МЕГА на вищу шкалу, що при наший числах це не мало ніколи б бути
          2) у 7-мый позицыъ було незрозумлий символ - і це э помилкова ситуація, яка б також ніколи не мала виникати
          
          У такому разі будем виводити повідомлення ***.*** і символ у 7-мый позицыъ ,який призвів до такої ситуації
          */
            error = 2;
          }
          break;
        }
      default:
        {
          /*
        теоретично сюди програма ніколи б не  мала зайти
        
        У такому разі будем виводити повідомлення XXXXXXX
        */
          error = 1;
        }
    }
  }

  if (error == 0)
  {
    //При приведені числа до відповідного формату і відповідної шкали помилок не було зафіксовано
    unsigned int first_symbol = 0, value, position = start_position; /*позиція з якої поинається виведення значення - найзначуще число*/

    /*
    X?????
    */
    if (temp_value > 99999)
    {
      value = temp_value / 100000;
      temp_value %= 100000;
      *(name_string + position) = value + 0x30;
      first_symbol = 1;
    }
    else
    {
      if (number_digit_after_point == 1)
      {
        *(name_string + position) = '0';
        first_symbol = 1;
      }
      else
        *(name_string + position) = ' ';
    }
    position++;

    if (number_digit_after_point == 1)
    {
      //Десяткова "кома"
      *(name_string + position) = '.';
      position++;
    }

    /*
    *X????
    */
    if (temp_value > 9999)
    {
      value = temp_value / 10000;
      temp_value %= 10000;
      *(name_string + position) = value + 0x30;
      first_symbol = 1;
    }
    else
    {
      if (
        (number_digit_after_point == 2) ||
        (first_symbol != 0))
      {
        *(name_string + position) = '0';
        first_symbol = 1;
      }
      else
        *(name_string + position) = ' ';
    }
    position++;

    if (number_digit_after_point == 2)
    {
      //Десяткова "кома"
      *(name_string + position) = '.';
      position++;
    }

    /*
    **X???
    */
    value = temp_value / 1000;
    temp_value %= 1000;
    *(name_string + position) = value + 0x30;
    position++;

    if (number_digit_after_point == 3)
    {
      //Десяткова "кома"
      *(name_string + position) = '.';
      position++;
    }

    //З цього місця змінна position має обов'язково бути рівною (start_position + 4)
    if (position == (start_position + 4))
    {
      /*
      ***X??
      */
      value = temp_value / 100;
      temp_value %= 100;
      *(name_string + (position++)) = value + 0x30;

      /*
      ****X?
      */
      value = temp_value / 10;
      temp_value %= 10;
      *(name_string + (position++)) = value + 0x30;

      /*
      *****X
      */
      *(name_string + (position++)) = temp_value + 0x30;
    }
    else
    {
      //Помилка, яка б ніколи не мала виникати помічаємо це знаками оклику
      for (unsigned int i = 0; i < 7; i++)
        *(name_string + start_position + i) = '!';
      *(name_string + start_position + 7) = ' ';
    }
  }
  else if (error == 1)
  {
    //Помилка виникла при аналізі попереднього формату
    for (unsigned int i = 0; i < 7; i++)
      *(name_string + start_position + i) = 'X';
    *(name_string + start_position + 7) = ' ';
  }
  else if (error == 2)
  {
    //Помилка виникла при переході на іншу шкалу
    for (unsigned int i = 0; i < 7; i++)
      *(name_string + start_position + i) = '*';
    *(name_string + start_position + 3) = '.';
  }
  else
  {
    //Невизначена помилка
    for (unsigned int i = 0; i < 7; i++)
      *(name_string + start_position + i) = '?';
    *(name_string + start_position + 7) = ' ';
  }
}
/*****************************************************/

/*****************************************************/
//Конвертація в рядок і поміщення в масив для відображення на екрані частоти
/*****************************************************/
void convert_and_insert_char_for_frequency(int temp_meas_1000, unsigned char *name_string)
{
#define FIRST_POSITION_OF_NUMBER 5

  int temp_value = temp_meas_1000;

  int index_language = index_language_in_array(current_settings.language);
  if ((temp_value >= (MIN_FREQUENCY * 1000)) && (temp_value <= ((MAX_FREQUENCY + 1) * 1000)))
  {
    static const unsigned char Hz[MAX_NAMBER_LANGUAGE][2] = {"Гц", "Гц", "Hz", "Гц"};
    for (unsigned int i = 0; i < 2; i++)
    {
      *(name_string + FIRST_POSITION_OF_NUMBER + 7 + i) = Hz[index_language][i];
    }

    unsigned int position = FIRST_POSITION_OF_NUMBER; /*позиція з якої поинається виведення значення - найзначуще число*/
    int value;

    /*
    X????
    */
    if (temp_value > 9999)
    {
      value = temp_value / 10000;
      temp_value %= 10000;
      *(name_string + position) = value + 0x30;
    }
    else
    {
      *(name_string + position) = ' ';
    }
    position++;

    /*
    *X???
    */
    value = temp_value / 1000;
    temp_value %= 1000;
    *(name_string + position) = value + 0x30;
    position++;
    {
      //Десяткова "кома"
      *(name_string + position) = '.';
      position++;
    }

    //З цього місця змінна position має обов'язково бути рівною (FIRST_POSITION_OF_NUMBER + 3)
    if (position == (FIRST_POSITION_OF_NUMBER + 3))
    {
      /*
      **X??
      */
      value = temp_value / 100;
      temp_value %= 100;
      *(name_string + (position++)) = value + 0x30;

      /*
      ***X?
      */
      value = temp_value / 10;
      temp_value %= 10;
      *(name_string + (position++)) = value + 0x30;

      /*
      ****X
      */
      *(name_string + (position++)) = temp_value + 0x30;
    }
    else
    {
      //Помилка, яка б ніколи не мала виникати помічаємо це знаками оклику
      for (unsigned int i = 0; i < 6; i++)
        *(name_string + FIRST_POSITION_OF_NUMBER + i) = '!';
    }
  }
  else if (temp_meas_1000 < 0)
  {
    static const unsigned char undefined[MAX_NAMBER_LANGUAGE][MAX_COL_LCD - FIRST_POSITION_OF_NUMBER] =
      {
        "Неопред.   ",
        "Невизнач.  ",
        "Undef.     ",
        "Неопред.   "};

    for (int i = 0; i < (MAX_COL_LCD - FIRST_POSITION_OF_NUMBER); i++)
      *(name_string + FIRST_POSITION_OF_NUMBER + i) = undefined[index_language][i];
  }
  else
  {
    //Помилка, яка б ніколи не мала виникати помічаємо це знаками питання
    for (unsigned int i = 0; i < 6; i++)
      *(name_string + FIRST_POSITION_OF_NUMBER + i) = '?';
  }
#undef FIRST_POSITION_OF_NUMBER
}
/*****************************************************/

/*****************************************************/
//Формуємо екран відображення списків вимірювань
/*****************************************************/
void make_ekran_measurement(void)
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_MEASURMENT][MAX_COL_LCD] =
    {
      {" Токи           ",
       " Напряжения     ",
       " Частоты        ",
       " Углы           ",
       " Мощность       "},
      {" Струми         ",
       " Напруги        ",
       " Частоти        ",
       " Кути           ",
       " Потужність     "},
      {" Currents       ",
       " Voltages       ",
       " Frequencies    ",
       " Angles         ",
       " Power          "},
      {" Токи           ",
       " Напряжения     ",
       " Частоты        ",
       " Углы           ",
       " Мощность       "}};
  int index_language = index_language_in_array(current_settings.language);

  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;

  index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  //Копіюємо  рядки у робочий екран
  for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
  {
    //Наступні рядки треба перевірити, чи їх требе відображати у текучій коффігурації
    if (index_of_ekran < MAX_ROW_FOR_MEASURMENT)
      for (unsigned int j = 0; j < MAX_COL_LCD; j++)
        working_ekran[i][j] = name_string[index_language][index_of_ekran][j];
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
//Формуємо екран відображення списків вимірювань
/*****************************************************/
void make_ekran_measurement_voltage_type(void)
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_MEASURMENT_VOLTAGE_TYPE][MAX_COL_LCD] =
    {
      {" Фазные         ",
       " Линейные       "},
      {" Фазні          ",
       " Лінійні        "},
      {" Ph-N           ",
       " Ph-Ph          "},
      {" Фазные         ",
       " Линейные       "}};
  int index_language = index_language_in_array(current_settings.language);

  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;

  index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  //Копіюємо  рядки у робочий екран
  for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
  {
    //Наступні рядки треба перевірити, чи їх требе відображати у текучій коффігурації
    if (index_of_ekran < MAX_ROW_FOR_MEASURMENT_VOLTAGE_TYPE)
      for (unsigned int j = 0; j < MAX_COL_LCD; j++)
        working_ekran[i][j] = name_string[index_language][index_of_ekran][j];
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
//Формуємо екран відображення струмів
/*****************************************************/
void make_ekran_current(unsigned int pervynna_vtorynna)
{

  static unsigned char const name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_MEASURMENT_CURRENT][MAX_COL_LCD] =
    {
      {" 3I0 i=         ",
       " 3I0  =         ",
       " 3I0вг=         ",
       " 3I0-1=         ",
       " Ia   =         ",
       " Ia 2г=         ",
       " Ib   =         ",
       " Ib 2г=         ",
       " Ic   =         ",
       " Ic 2г=         ",
       " I2   =         ",
       " I1   =         ",
       " I0.4 =         "},
      {" 3I0 i=         ",
       " 3I0  =         ",
       " 3I0вг=         ",
       " 3I0-1=         ",
       " Ia   =         ",
       " Ia 2г=         ",
       " Ib   =         ",
       " Ib 2г=         ",
       " Ic   =         ",
       " Ic 2г=         ",
       " I2   =         ",
       " I1   =         ",
       " I0.4 =         "},
      {" 3I0 i=         ",
       " 3I0  =         ",
       " 3I0oh=         ",
       " 3I0-1=         ",
       " Ia   =         ",
       " Ih2a =         ",
       " Ib   =         ",
       " Ih2b =         ",
       " Ic   =         ",
       " Ih2c =         ",
       " I2   =         ",
       " I1   =         ",
       " I0.4 =         "},
      {" 3I0 i=         ",
       " 3I0  =         ",
       " 3I0вг=         ",
       " 3I0-1=         ",
       " Ia   =         ",
       " Ia 2г=         ",
       " Ib   =         ",
       " Ib 2г=         ",
       " Ic   =         ",
       " Ic 2г=         ",
       " I2   =         ",
       " I1   =         ",
       " I0.4 =         "}};
  int const index_language = index_language_in_array(current_settings.language);
  unsigned int index_array[MAX_ROW_FOR_MEASURMENT_CURRENT] =
    {
      IM_3I0_i,
      IM_3I0,
      IM_3I0_other_g,
      IM_3I0_r,
      IM_IA,
      IM2_IA,
      IM_IB,
      IM2_IB,
      IM_IC,
      IM2_IC,
      IM_I2,
      IM_I1,
      IM_I04};

  unsigned char name_string_tmp[MAX_ROW_FOR_MEASURMENT_CURRENT][MAX_COL_LCD];
  for (size_t i = 0; (i < MAX_ROW_FOR_MEASURMENT_CURRENT); ++i)
  {
    unsigned char const *ptr_source = name_string[index_language][i];
    unsigned char *ptr_target = name_string_tmp[i];
    for (size_t j = 0; j < MAX_COL_LCD; j++)
      *ptr_target++ = *ptr_source++;
  }

  //Копіюємо вимірювання які потрібні для відображення
  semaphore_measure_values_low1 = 1;
  for (unsigned int i = 0; i < MAX_ROW_FOR_MEASURMENT_CURRENT; i++)
  {
    unsigned int index_to_copy = index_array[i];
    measurement_low[index_to_copy] = measurement_middle[index_to_copy];
  }
  semaphore_measure_values_low1 = 0;

  for (unsigned int i = 0; i < MAX_ROW_FOR_MEASURMENT_CURRENT; i++)
  {
    name_string_tmp[i][MAX_COL_LCD - 1] = odynyci_vymirjuvannja[index_language][INDEX_A];
    if (
      (index_array[i] == IM_IB) &&
      ((current_settings.control_extra_settings_1 & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_IB_I04)) != 0))
    {
      if (index_language == INDEX_LANGUAGE_EN)
        name_string_tmp[i][4] = 'c';
      else
        name_string_tmp[i][4] = 'р';

      name_string_tmp[i][5] = '.';
    }
  }

  int additional_current = 0;
  int position_temp = current_ekran.index_position;
  int index_of_ekran;
  /******************************************/
  //Виключаємо, які вимірювання не треба відображати
  /******************************************/
  {
    int delete_meas;
    if ((current_settings.control_extra_settings_1 & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_IB_I04)) == 0)
      delete_meas = IM_I04;
    else
      delete_meas = IM_3I0_r;

    int i = 0;
    for (; i < (MAX_ROW_FOR_MEASURMENT_CURRENT - additional_current); ++i)
    {
      if (index_array[i + additional_current] == delete_meas)
        break;
    }
    //    int i = delete_index - additional_current;
    if ((i + 1) <= position_temp)
      position_temp--;
    do
    {
      for (unsigned int j = 0; j < MAX_COL_LCD; j++)
      {
        if ((i + 1) < (MAX_ROW_FOR_MEASURMENT_CURRENT - additional_current))
          name_string_tmp[i][j] = name_string_tmp[i + 1][j];
        else
          name_string_tmp[i][j] = ' ';
      }

      if ((i + 1) < (MAX_ROW_FOR_MEASURMENT_CURRENT - additional_current))
        index_array[i] = index_array[i + 1];
      else
        index_array[i] = 255;

      i++;
    } while (i < (MAX_ROW_FOR_MEASURMENT_CURRENT - additional_current));
    additional_current++;
  }
  /******************************************/

  index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  //Копіюємо  рядки у робочий екран
  for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
  {
    //Наступні рядки треба перевірити, чи їх требе відображати у текучій кофігурації
    if (index_of_ekran < (MAX_ROW_FOR_MEASURMENT_CURRENT - additional_current))
    {
      /********************************/
      //Вводимо вимірювальні значення
      unsigned int index = index_array[index_of_ekran];
      if (index != 255)
      {
        unsigned int start_number_digit_after_point;
        if (
          (index == INDEX_ML_3I0_i) ||
          (index == INDEX_ML_3I0) ||
          (index == INDEX_ML_3I0_other_g))
          start_number_digit_after_point = 2;
        else
          start_number_digit_after_point = 3;

        unsigned int koef_mul = 1;
        if (pervynna_vtorynna != 0)
        {
          if (index < IM_3I0_r)
            koef_mul = current_settings.T0;
          else if (index == IM_I04)
            koef_mul = current_settings.TCurrent04;
          else
            koef_mul = current_settings.TCurrent;
        }
        convert_and_insert_char_for_measurement(start_number_digit_after_point, measurement_low[index], koef_mul, 1, name_string_tmp[index_of_ekran], 7);
      }
      /********************************/

      for (unsigned int j = 0; j < MAX_COL_LCD; j++)
        working_ekran[i][j] = name_string_tmp[index_of_ekran][j];
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
//Формуємо екран відображення фазних напруг
/*****************************************************/
void make_ekran_voltage_phase(unsigned int pervynna_vtorynna)
{
  int const index_language = index_language_in_array(current_settings.language);
  unsigned char name_string[MAX_ROW_FOR_MEASURMENT_VOLTAGE_PHASE][MAX_COL_LCD] =
    {
      "                ",
      "                ",
      "                ",
      "                ",
      "                ",
      "                "};
  unsigned int index_array[MAX_ROW_FOR_MEASURMENT_VOLTAGE_PHASE] = {255, 255, 255, 255, 255, 255};
  unsigned int row = 0;

  if ((current_settings.control_extra_settings_1 & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE)) == 0)
  {
    static const unsigned char name_phase[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_MEASURMENT_VOLTAGE_PHASE - 1][MAX_COL_LCD] =
      {
        {" Ua  =          ",
         " Ub  =          ",
         " Uc  =          ",
         " U2  =          ",
         " U1  =          "},
        {" Ua  =          ",
         " Ub  =          ",
         " Uc  =          ",
         " U2  =          ",
         " U1  =          "},
        {" Va  =          ",
         " Vb  =          ",
         " Vc  =          ",
         " V2  =          ",
         " V1  =          "},
        {" Ua  =          ",
         " Ub  =          ",
         " Uc  =          ",
         " U2  =          ",
         " U1  =          "}};
    static const unsigned int index_array_phase[MAX_ROW_FOR_MEASURMENT_VOLTAGE_PHASE - 1] =
      {
        IM_UA,
        IM_UB,
        IM_UC,
        IM_U2,
        IM_U1};

    for (unsigned int i = 0; i < (MAX_ROW_FOR_MEASURMENT_VOLTAGE_PHASE - 1); i++)
    {
      for (unsigned int j = 0; j < MAX_COL_LCD; j++)
        name_string[row][j] = name_phase[index_language][i][j];
      index_array[row++] = index_array_phase[i];
    }
  }

  static const unsigned char name_3U0[MAX_NAMBER_LANGUAGE][MAX_COL_LCD] =
    {
      " 3U0 =          ",
      " 3U0 =          ",
      " 3V0 =          ",
      " 3U0 =          "};

  for (unsigned int j = 0; j < MAX_COL_LCD; j++)
    name_string[row][j] = name_3U0[index_language][j];
  index_array[row++] = IM_3U0;

  //Копіюємо вимірювання які потрібні для відображення
  semaphore_measure_values_low1 = 1;
  for (unsigned int i = 0; i < row; i++)
  {
    unsigned int index_to_copy = index_array[i];
    measurement_low[index_to_copy] = measurement_middle[index_to_copy];
  }
  semaphore_measure_values_low1 = 0;

  for (unsigned int i = 0; i < row; i++)
  {
    name_string[i][MAX_COL_LCD - 1] = odynyci_vymirjuvannja[index_language][INDEX_V];
  }

  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;

  index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  //Копіюємо  рядки у робочий екран
  for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
  {
    //Наступні рядки треба перевірити, чи їх требе відображати у текучій кофігурації
    if (index_of_ekran < row)
    {
      /********************************/
      //Вводимо вимірювальні значення
      unsigned int start_number_digit_after_point = 3;

      unsigned int index = index_array[index_of_ekran];
      if (index != 255)
      {
        if (pervynna_vtorynna == 0)
          convert_and_insert_char_for_measurement(start_number_digit_after_point, measurement_low[index], 1, 1, name_string[index_of_ekran], 7);
        else
        {
          //Фазні напруги
          convert_and_insert_char_for_measurement(start_number_digit_after_point, measurement_low[index], current_settings.TVoltage, 1, name_string[index_of_ekran], 7);
        }
      }
      /********************************/

      for (unsigned int j = 0; j < MAX_COL_LCD; j++)
        working_ekran[i][j] = name_string[index_of_ekran][j];
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
//Формуємо екран відображення лінійних напруг
/*****************************************************/
void make_ekran_voltage_line(unsigned int pervynna_vtorynna)
{
  static unsigned char const name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_MEASURMENT_VOLTAGE_LINE][MAX_COL_LCD] =
    {
      {" Uab =          ",
       " Ubc =          ",
       " Uca =          "},
      {" Uab =          ",
       " Ubc =          ",
       " Uca =          "},
      {" Vab =          ",
       " Vbc =          ",
       " Vca =          "},
      {" Uab =          ",
       " Ubc =          ",
       " Uca =          "}};
  unsigned int index_array[MAX_ROW_FOR_MEASURMENT_VOLTAGE_LINE] = {IM_UAB, IM_UBC, IM_UCA};
  unsigned int row = 3;

  //Копіюємо вимірювання які потрібні для відображення
  semaphore_measure_values_low1 = 1;
  for (unsigned int i = 0; i < row; i++)
  {
    unsigned int index_to_copy = index_array[i];
    measurement_low[index_to_copy] = measurement_middle[index_to_copy];
  }
  semaphore_measure_values_low1 = 0;

  int const index_language = index_language_in_array(current_settings.language);
  unsigned char name_string_tmp[MAX_ROW_FOR_MEASURMENT_VOLTAGE_LINE][MAX_COL_LCD];
  for (size_t i = 0; (i < MAX_ROW_FOR_MEASURMENT_VOLTAGE_LINE); ++i)
  {
    unsigned char const *ptr_source = name_string[index_language][i];
    unsigned char *ptr_target = name_string_tmp[i];
    for (size_t j = 0; j < MAX_COL_LCD; j++)
      *ptr_target++ = *ptr_source++;
  }

  for (unsigned int i = 0; i < row; i++)
  {
    name_string_tmp[i][MAX_COL_LCD - 1] = odynyci_vymirjuvannja[index_language][INDEX_V];
  }

  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;

  index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  //Копіюємо  рядки у робочий екран
  for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
  {
    //Наступні рядки треба перевірити, чи їх требе відображати у текучій кофігурації
    if (index_of_ekran < row)
    {
      /********************************/
      //Вводимо вимірювальні значення
      unsigned int start_number_digit_after_point = 3;

      unsigned int index = index_array[index_of_ekran];
      if (index != 255)
      {
        if (pervynna_vtorynna == 0)
          convert_and_insert_char_for_measurement(start_number_digit_after_point, measurement_low[index], 1, 1, name_string_tmp[index_of_ekran], 7);
        else
        {
          //Лінійні напруги
          convert_and_insert_char_for_measurement(start_number_digit_after_point, measurement_low[index], current_settings.TVoltage, 1, name_string_tmp[index_of_ekran], 7);
        }
      }
      /********************************/

      for (unsigned int j = 0; j < MAX_COL_LCD; j++)
        working_ekran[i][j] = name_string_tmp[index_of_ekran][j];
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
//Формуємо екран відображення частот
/*****************************************************/
void make_ekran_frequency(void)
{
  unsigned char name_string[MAX_ROW_FOR_MEASURMENT_FREQUENCY][MAX_COL_LCD] =
    {
      " f =            "};
  semaphore_measure_values_low1 = 1;
  int measurement_fequency = (int) (frequency_middle * 1000);
  semaphore_measure_values_low1 = 0;

  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;

  index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  //Копіюємо  рядки у робочий екран
  for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
  {
    //Наступні рядки треба перевірити, чи їх требе відображати у текучій кофігурації
    if (index_of_ekran < MAX_ROW_FOR_MEASURMENT_FREQUENCY)
    {
      /********************************/
      //Вводимо вимірювальні значення
      if (measurement_fequency < 0)
      {
        if (measurement_fequency == (-2 * 1000))
        {
          /*Частота нижче порогу визначеного константою MIN_FREQUENCY*/
          name_string[index_of_ekran][3] = '<';
          measurement_fequency = MIN_FREQUENCY * 1000;
        }
        if (measurement_fequency == (-3 * 1000))
        {
          /*Частота вище порогу визначеного константою MAX_FREQUENCY*/
          name_string[index_of_ekran][3] = '>';
          measurement_fequency = MAX_FREQUENCY * 1000;
        }
      }

      convert_and_insert_char_for_frequency(measurement_fequency, name_string[index_of_ekran]);
      /********************************/

      for (unsigned int j = 0; j < MAX_COL_LCD; j++)
        working_ekran[i][j] = name_string[index_of_ekran][j];
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
//Формуємо екран відображення кутів
/*****************************************************/
void make_ekran_angle(void)
{
  int const index_language = index_language_in_array(current_settings.language);
  if (base_index_for_angle < 0)
  {
    //Неможливо визначити кути
    static const unsigned char information[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] =
      {
        {"  Невозможно    ",
         " определить углы"},
        {"   Неможливо    ",
         " визначити кути "},
        {" Impossibility  ",
         "to define angles"},
        {"  Невозможно    ",
         " определить углы"}};

    for (int index_1 = 0; index_1 < MAX_ROW_LCD; index_1++)
    {
      for (int index_2 = 0; index_2 < MAX_COL_LCD; index_2++)
        working_ekran[index_1][index_2] = (index_1 < 2) ? information[index_language][index_1][index_2] : ' ';
    }

    //Відображення курору по вертикалі
    current_ekran.position_cursor_y = 0;
    //Курсор не видимий
    current_ekran.cursor_on = 0;
  }
  else
  {
    static unsigned char const name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_MEASURMENT_ANGLE][MAX_COL_LCD] =
      {
        {" Ua  -          ",
         " Ub  -          ",
         " Uc  -          ",
         " Uab -          ",
         " Ubc -          ",
         " Uca -          ",
         " 3U0 -          ",
         " Ia  -          ",
         " Ib  -          ",
         " Ic  -          ",
         " 3I0 -          ",
         "3I0-1-          ",
         "I0.4 -          "},
        {" Ua  -          ",
         " Ub  -          ",
         " Uc  -          ",
         " Uab -          ",
         " Ubc -          ",
         " Uca -          ",
         " 3U0 -          ",
         " Ia  -          ",
         " Ib  -          ",
         " Ic  -          ",
         " 3I0 -          ",
         "3I0-1-          ",
         "I0.4 -          "},
        {" Va  -          ",
         " Vb  -          ",
         " Vc  -          ",
         " Vab -          ",
         " Vbc -          ",
         " Vca -          ",
         " 3V0 -          ",
         " Ia  -          ",
         " Ib  -          ",
         " Ic  -          ",
         " 3I0 -          ",
         "3I0-1-          ",
         "I0.4 -          "},
        {" Ua  -          ",
         " Ub  -          ",
         " Uc  -          ",
         " Uab -          ",
         " Ubc -          ",
         " Uca -          ",
         " 3U0 -          ",
         " Ia  -          ",
         " Ib  -          ",
         " Ic  -          ",
         " 3I0 -          ",
         "3I0-1-          ",
         "I0.4 -          "}};
    unsigned char name_string_tmp[MAX_ROW_FOR_MEASURMENT_ANGLE][MAX_COL_LCD];
    for (size_t i = 0; (i < MAX_ROW_FOR_MEASURMENT_ANGLE); ++i)
    {
      unsigned char const *ptr_source = name_string[index_language][i];
      unsigned char *ptr_target = name_string_tmp[i];
      for (size_t j = 0; j < MAX_COL_LCD; j++)
        *ptr_target++ = *ptr_source++;
    }

#define SIZE_UNDEF 6
    static const unsigned char undefined[MAX_NAMBER_LANGUAGE][SIZE_UNDEF] =
      {
        "Неопр.",
        "Нев.  ",
        "Undef.",
        "Неопр."};
    static const unsigned char number_chars_for_undef[MAX_NAMBER_LANGUAGE] = {SIZE_UNDEF, 4, SIZE_UNDEF, SIZE_UNDEF};
#undef SIZE_UNDEF

    int number_charts_for_undef_tmp = number_chars_for_undef[index_language];

    /*************
    Завершуємо формування назв кутів
    *************/
    //    if ((current_settings.control_extra_settings_1 & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_IB_I04)) == 0)
    //    {
    //      if (index_language == INDEX_LANGUAGE_EN) name_string[FULL_ORT_3I0_r][3] = 'c';
    //      else name_string[FULL_ORT_3I0_r][3] = 'р';
    //    }

#define SIZE_NAME_ANALOG_CANAL 5
    for (int index_1 = 0; index_1 < MAX_ROW_FOR_MEASURMENT_ANGLE; index_1++)
    {
      for (int index_2 = 0; index_2 < SIZE_NAME_ANALOG_CANAL; index_2++)
        name_string_tmp[index_1][SIZE_NAME_ANALOG_CANAL + 1 + index_2] = name_string_tmp[base_index_for_angle][index_2];
    }
#undef SIZE_NAME_ANALOG_CANAL
    /*************/

    unsigned int value_index_shift[MAX_ROW_FOR_MEASURMENT_ANGLE] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    unsigned int additional_current = 0;
    unsigned int position_temp = current_ekran.index_position;
    unsigned int index_of_ekran;

    /******************************************/
    //Виключаємо поля, які не треба відображати
    /******************************************/
    if ((current_settings.control_extra_settings_1 & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE)) != 0)
    {
      for (__full_ort_index index_tmp = FULL_ORT_Ua; index_tmp <= FULL_ORT_Uc; index_tmp++)
      {
        unsigned int i = index_tmp - additional_current;
        unsigned int additional_current_new = additional_current + 1;
        if ((i + 1) <= position_temp)
          position_temp--;
        do
        {
          for (unsigned int j = 0; j < MAX_COL_LCD; j++)
          {
            if ((i + 1) < (MAX_ROW_FOR_MEASURMENT_ANGLE - additional_current))
              name_string_tmp[i][j] = name_string_tmp[i + 1][j];
            else
              name_string_tmp[i][j] = ' ';
          }
          value_index_shift[i] = additional_current_new;
          i++;
        } while (i < (MAX_ROW_FOR_MEASURMENT_ANGLE - additional_current));
        additional_current = additional_current_new;
      }
    }

    {
      int delete_index;
      if ((current_settings.control_extra_settings_1 & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_IB_I04)) == 0)
        delete_index = FULL_ORT_I04;
      else
        delete_index = FULL_ORT_3I0_r;

      unsigned int i = delete_index - additional_current;
      unsigned int additional_current_new = additional_current + 1;
      if ((i + 1) <= position_temp)
        position_temp--;
      do
      {
        for (unsigned int j = 0; j < MAX_COL_LCD; j++)
        {
          if ((i + 1) < (MAX_ROW_FOR_MEASURMENT_ANGLE - additional_current))
            name_string_tmp[i][j] = name_string_tmp[i + 1][j];
          else
            name_string_tmp[i][j] = ' ';
        }
        value_index_shift[i] = additional_current_new;
        i++;
      } while (i < (MAX_ROW_FOR_MEASURMENT_ANGLE - additional_current));
      additional_current = additional_current_new;
    }
    /******************************************/

    index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

    //Копіюємо  рядки у робочий екран
    for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
    {
      //Наступні рядки треба перевірити, чи їх требе відображати у текучій кофігурації
      if (index_of_ekran < (MAX_ROW_FOR_MEASURMENT_ANGLE - additional_current))
      {
        int value = phi_angle[bank_for_calc_phi_angle][index_of_ekran + value_index_shift[index_of_ekran]];

#define LAST_POSITION_OF_TITLE 10

        //Видаляємо зайві пробіли і по можливості звільняємо першу позицю по горизонталі для курсору
        int shift = 0;
        for (int index_1 = 1; index_1 <= LAST_POSITION_OF_TITLE; index_1++)
        {
          if (name_string_tmp[index_of_ekran][index_1 - shift] == ' ')
          {
            //Підтягуємо символи, щоб не було багато пробілів
            for (int index_2 = (index_1 - shift); index_2 <= (LAST_POSITION_OF_TITLE - shift); index_2++)
            {
              name_string_tmp[index_of_ekran][index_2] = name_string_tmp[index_of_ekran][index_2 + 1];
            }
            shift++;
          }
        }
        if (
          (name_string_tmp[index_of_ekran][0] != ' ') &&
          (shift > 0) &&
          (((value >= 0 /*0.0°*/) && (value <= 999 /*99.9°*/)) ||
           ((value < 0) &&
            (number_charts_for_undef_tmp < (MAX_COL_LCD - LAST_POSITION_OF_TITLE - 1 - 1)))))
        {
          for (int index_1 = (LAST_POSITION_OF_TITLE - shift); index_1 >= 0; index_1--)
            name_string_tmp[index_of_ekran][index_1 + 1] = name_string_tmp[index_of_ekran][index_1];
          name_string_tmp[index_of_ekran][0] = ' ';
          shift--;
        }

        int position = LAST_POSITION_OF_TITLE - shift + 1;

#undef LAST_POSITION_OF_TITLE

        if (value >= 0)
        {
          int number_charts_for_value = 4; /*число до коми, десяткова кома, десті і знак "°"*/
          if (value > 999 /*99.9°*/)
            number_charts_for_value += 2;
          else if (value > 99 /*9.9°*/)
            number_charts_for_value += 1;

          int free_position = MAX_COL_LCD - position - number_charts_for_value;
          if (free_position >= 3)
          {
            name_string_tmp[index_of_ekran][position] = ' ';
            name_string_tmp[index_of_ekran][position + 1] = '=';
            name_string_tmp[index_of_ekran][position + 2] = ' ';
            position += 3;
          }
          else if (free_position == 2)
          {
            name_string_tmp[index_of_ekran][position] = ' ';
            name_string_tmp[index_of_ekran][position + 1] = '=';
            position += 2;
          }
          else
          {
            name_string_tmp[index_of_ekran][position] = '=';
            position += 1;
          }

          //Кут між лінійною напругою і фазним струмом (з точністю до десятих)
          int vaga = 1000, first_symbol = 0;

          while (vaga > 0)
          {
            int temp_data;
            temp_data = value / vaga; //виділяємо число, яке треба перетворити у символ і помістити у дану позицію екрану
            value %= vaga;            //вираховуємо число без символа, який ми зараз будемо виводити на екран
            vaga /= 10;               //зменшуємо ваговий коефіцієнт в 10 разів

            if (vaga == 0)
              name_string_tmp[index_of_ekran][position++] = '.'; //десяткова кома

            //У випадку, якщо ми не у режимі редагування, то нулі перед найстаршим значущим числом приховуємо
            if ((temp_data != 0) || (first_symbol != 0))
            {
              name_string_tmp[index_of_ekran][position] = temp_data + 0x30;
              if (first_symbol == 0)
                first_symbol = 1;
            }
            else
            {
              //Нуль виводимо тільки у тому випадку, якщо це є символ одиниць числа (текуча вага числа рівна 1)
              if (vaga > 1)
                name_string_tmp[index_of_ekran][position] = ' ';
              else
              {
                name_string_tmp[index_of_ekran][position] = temp_data + 0x30;
                if (first_symbol == 0)
                  first_symbol = 1;
              }
            }
            if (name_string_tmp[index_of_ekran][position] != ' ')
              position++;
          }
          name_string_tmp[index_of_ekran][position] = '°';
        }
        else
        {
          int free_position = MAX_COL_LCD - position - number_charts_for_undef_tmp;
          if (free_position >= 3)
          {
            name_string_tmp[index_of_ekran][position] = ' ';
            name_string_tmp[index_of_ekran][position + 1] = '=';
            name_string_tmp[index_of_ekran][position + 2] = ' ';
            position += 3;
          }
          else if (free_position == 2)
          {
            name_string_tmp[index_of_ekran][position] = ' ';
            name_string_tmp[index_of_ekran][position + 1] = '=';
            position += 2;
          }
          else
          {
            name_string_tmp[index_of_ekran][position] = '=';
            position += 1;
          }

          for (int j = 0; j < number_charts_for_undef_tmp; j++)
            name_string_tmp[index_of_ekran][position + j] = undefined[index_language][j];
        }

        for (unsigned int j = 0; j < MAX_COL_LCD; j++)
          working_ekran[i][j] = name_string_tmp[index_of_ekran][j];
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
  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//Формуємо екран відображення потужностей
/*****************************************************/
void make_ekran_power(unsigned int pervynna_vtorynna)
{

  unsigned char name_string[MAX_ROW_FOR_MEASURMENT_POWER][MAX_COL_LCD] =
    {
      " P =            ",
      " Q =            ",
      " S =            ",
      " cos(§) =       "};
  static const unsigned int index_of_start_position[MAX_ROW_FOR_MEASURMENT_POWER] = {5, 5, 5, 10};

#define SIZE_POWER_DIMENSION 3
  static const unsigned char power_dimension[MAX_ROW_FOR_MEASURMENT_POWER - 1][MAX_NAMBER_LANGUAGE][SIZE_POWER_DIMENSION] =
    {
      {"Вт ", "Вт ", "W  ", "Вт "},
      {"ВАр", "ВАр", "VAr", "ВАр"},
      {"ВА ", "ВА ", "VA ", "ВА "}};

  int index_language = index_language_in_array(current_settings.language);

  for (unsigned int i = 0; i < (MAX_ROW_FOR_MEASURMENT_POWER - 1); i++)
  {
    for (unsigned int j = 0; j < SIZE_POWER_DIMENSION; j++)
      name_string[i][MAX_COL_LCD - SIZE_POWER_DIMENSION + j] = power_dimension[i][index_language][j];
  }
#undef SIZE_POWER_DIMENSION

  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;

  index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  //Копіюємо  рядки у робочий екран
  for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
  {
    //Наступні рядки треба перевірити, чи їх требе відображати у текучій кофігурації
    if (index_of_ekran < MAX_ROW_FOR_MEASURMENT_POWER)
    {
      unsigned int start_position = index_of_start_position[index_of_ekran];
      /********************************/
      //Вводимо вимірювальні значення
      int temp_value = 0;
      switch (index_of_ekran)
      {
        case INDEX_ML_P:
          {
            temp_value = P[bank_for_calc_power];
            break;
          }
        case INDEX_ML_Q:
          {
            temp_value = Q[bank_for_calc_power];
            break;
          }
        case INDEX_ML_S:
          {
            temp_value = S[bank_for_calc_power];
            break;
          }
        case INDEX_ML_COS_PHI:
          {
            temp_value = cos_phi_x1000[bank_for_calc_power];
            break;
          }
        default:
          {
            //Теоретично цього ніколи не мало б бути
            total_error_sw_fixed();
          }
      }

      if (temp_value < 0)
      {
        temp_value = -temp_value;
        name_string[index_of_ekran][start_position - 1] = '-';
      }

      if (index_of_ekran != INDEX_ML_COS_PHI)
      {
        if (pervynna_vtorynna == 0)
          convert_and_insert_char_for_measurement(3, (unsigned int) temp_value, 1, 1, name_string[index_of_ekran], start_position);
        else
          convert_and_insert_char_for_measurement(3, (unsigned int) temp_value, current_settings.TVoltage * current_settings.TCurrent, 1, name_string[index_of_ekran], start_position);
      }
      else
      {
        unsigned int position = start_position;
        if (S[bank_for_calc_power] != 0)
        {
          unsigned int dilnyk = 1000;
          for (unsigned int j = 0; j < 4; j++)
          {
            int value;

            value = temp_value / dilnyk;
            temp_value %= dilnyk;
            name_string[index_of_ekran][position++] = value + 0x30;
            if (j == 0)
            {
              //Десяткова "кома"
              name_string[index_of_ekran][position++] = '.';
            }
            dilnyk /= 10;
          }
        }
        else
        {
#define SIZE_UNDEF 6
          static const unsigned char undefined[MAX_NAMBER_LANGUAGE][SIZE_UNDEF] =
            {
              "Неопр.",
              "Нев.  ",
              "Undef.",
              "Неопр."};

          position -= 1;
          for (unsigned int j = 0; j < SIZE_UNDEF; j++)
            name_string[index_of_ekran][position + j] = undefined[index_language][j];
#undef SIZE_UNDEF
        }
      }

      unsigned int shift = 0;
      while (
        (name_string[index_of_ekran][start_position] == ' ') &&
        ((start_position + shift) < MAX_COL_LCD))
      {
        for (unsigned int j = start_position; j < (MAX_COL_LCD - 1); j++)
          name_string[index_of_ekran][j] = name_string[index_of_ekran][j + 1];
        name_string[index_of_ekran][MAX_COL_LCD - 1] = ' ';
        shift++;
      }
      /********************************/

      for (unsigned int j = 0; j < MAX_COL_LCD; j++)
        working_ekran[i][j] = name_string[index_of_ekran][j];
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
