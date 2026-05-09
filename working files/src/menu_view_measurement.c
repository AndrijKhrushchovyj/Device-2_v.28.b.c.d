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
      {" Измерения ТН1  ",
       " Измерения ТН2  ",
       " Логометр       ",
       " Сельсин        ",
       " Частоты        "},
      {" Вимірювання ТН1",
       " Вимірювання ТН2",
       " Логометр       ",
       " Сельсин        ",
       " Частоти        "},
      {" VT1 Measure    ",
       " VT2 Measure    ",
       " Logometer      ",
       " Selsyn         ",
       " Frequencies    "},
      {" Измерения ТН1  ",
       " Измерения ТН2  ",
       " Логометр       ",
       " Сельсин        ",
       " Частоты        "}};
  unsigned char name_string_tmp[MAX_ROW_FOR_MEASURMENT][MAX_COL_LCD];

  int const index_language = index_language_in_array(current_settings.language);
  for (int index_1 = 0; index_1 < MAX_ROW_FOR_MEASURMENT; index_1++)
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
    unsigned int i = INDEX_ML_MEASURMENT_2 - additional_current;

    if ((i + 1) <= position_temp)
      position_temp--;
    do
    {
      for (unsigned int j = 0; j < MAX_COL_LCD; j++)
      {
        if ((i + 1) < MAX_ROW_FOR_MEASURMENT)
          name_string_tmp[i][j] = name_string_tmp[i + 1][j];
        else
          name_string_tmp[i][j] = ' ';
      }
      i++;
    } while (i < (MAX_ROW_FOR_MEASURMENT - additional_current));
    additional_current++;
  }

  //Логометр
  if (current_settings.type_control_location != 1)
  {
    unsigned int i = INDEX_ML_MEASURMENT_LOGOMETR - additional_current;

    if ((i + 1) <= position_temp)
      position_temp--;
    do
    {
      for (unsigned int j = 0; j < MAX_COL_LCD; j++)
      {
        if ((i + 1) < MAX_ROW_FOR_MEASURMENT)
          name_string_tmp[i][j] = name_string_tmp[i + 1][j];
        else
          name_string_tmp[i][j] = ' ';
      }
      i++;
    } while (i < (MAX_ROW_FOR_MEASURMENT - additional_current));
    additional_current++;
  }

  //Сельсин
  if (current_settings.type_control_location != 2)
  {
    unsigned int i = INDEX_ML_MEASURMENT_SELSYN - additional_current;

    if ((i + 1) <= position_temp)
      position_temp--;
    do
    {
      for (unsigned int j = 0; j < MAX_COL_LCD; j++)
      {
        if ((i + 1) < MAX_ROW_FOR_MEASURMENT)
          name_string_tmp[i][j] = name_string_tmp[i + 1][j];
        else
          name_string_tmp[i][j] = ' ';
      }
      i++;
    } while (i < (MAX_ROW_FOR_MEASURMENT - additional_current));
    additional_current++;
  }
  /******************************************/

  index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  //Копіюємо  рядки у робочий екран
  for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
  {
    //Наступні рядки треба перевірити, чи їх требе відображати у текучій коффігурації
    if (index_of_ekran < (MAX_ROW_FOR_MEASURMENT - additional_current))
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
//Формуємо екран відображення списків вимірювань
/*****************************************************/
void make_ekran_measuremet_for_selsyn(void)
{
  const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_MEASURMENT_FOR_SELSYN][MAX_COL_LCD] =
    {
      {" Напряжения     ",
       " Углы           "},
      {" Напруги        ",
       " Кути           "},
      {" Voltages       ",
       " Angles         "},
      {" Напряжения     ",
       " Углы           "}};
  int index_language = index_language_in_array(current_settings.language);

  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;

  index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  //Копіюємо  рядки у робочий екран
  for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
  {
    //Наступні рядки треба перевірити, чи їх требе відображати у текучій коффігурації
    if (index_of_ekran < MAX_ROW_FOR_MEASURMENT_FOR_SELSYN)
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
//Формуємо екран відображення струмів і напруг для ТН1 або ТН2
/*****************************************************/
void make_ekran_current_voltage(unsigned int tn1_tn2, unsigned int pervynna_vtorynna)
{
  unsigned char name_string[MAX_ROW_FOR_MEASURMENT_1_AND_2][MAX_COL_LCD] =
    {
      " Ia-  =         ",
      " Uab- =         "};
  unsigned int measurement_tmp[MAX_ROW_FOR_MEASURMENT_1_AND_2];
  unsigned int TCurrent1, TVoltage1;

  //Виврдимо номер ТН і відповідні вимірювання
  name_string[0][4] = (tn1_tn2 + 1) + 0x30;
  name_string[1][5] = (tn1_tn2 + 1) + 0x30;
  if (tn1_tn2 == 0)
  {
    measurement_tmp[0] = measurement[IM_IA_1];
    measurement_tmp[1] = measurement[IM_UAB_TN1];

    TCurrent1 = current_settings.TCurrent1;
    TVoltage1 = current_settings.TVoltage1;
  }
  else
  {
    measurement_tmp[0] = measurement[IM_IA_2];
    measurement_tmp[1] = measurement[IM_UAB_TN2];

    TCurrent1 = current_settings.TCurrent2;
    TVoltage1 = current_settings.TVoltage2;
  }

  int const index_language = index_language_in_array(current_settings.language);
  //Виврдимо одиниці вимірювань
  name_string[0][MAX_COL_LCD - 1] = odynyci_vymirjuvannja[index_language][INDEX_A];
  name_string[1][MAX_COL_LCD - 1] = odynyci_vymirjuvannja[index_language][INDEX_V];

  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;

  index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  //Копіюємо  рядки у робочий екран
  for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
  {
    /********************************/
    //Вводимо вимірювальні значення
    /********************************/
    if (index_of_ekran < MAX_ROW_FOR_MEASURMENT_1_AND_2)
    {
      unsigned int start_number_digit_after_point = 3;

      if (pervynna_vtorynna == 0)
      {
        convert_and_insert_char_for_measurement(start_number_digit_after_point, measurement_tmp[index_of_ekran], 1, 1, name_string[index_of_ekran], 7);
      }
      else if (index_of_ekran == INDEX_ML_I)
      {
        //Струм Ia
        convert_and_insert_char_for_measurement(start_number_digit_after_point, measurement_tmp[index_of_ekran], TCurrent1, 1, name_string[index_of_ekran], 7);
      }
      else
      {
        //Напруга Uab
        convert_and_insert_char_for_measurement(start_number_digit_after_point, measurement_tmp[index_of_ekran], TVoltage1, 1, name_string[index_of_ekran], 7);
      }

      for (unsigned int j = 0; j < MAX_COL_LCD; j++)
        working_ekran[i][j] = name_string[index_of_ekran][j];
    }
    else
      for (unsigned int j = 0; j < MAX_COL_LCD; j++)
        working_ekran[i][j] = ' ';
    /********************************/

    index_of_ekran++;
  }

  //Курсор по горизонталі відображається на першій позиції
  current_ekran.position_cursor_x = 0;
  //Відображення курору по вертикалі
  current_ekran.position_cursor_y = position_temp & (MAX_ROW_LCD - 1);
  //Курсор видимий
  current_ekran.cursor_on = 0;
  //Курсор не мигає
  current_ekran.cursor_blinking_on = 0;
  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//Формуємо екран відображення напруг для сельсина
/*****************************************************/
void make_ekran_voltage_for_selsyn(void)
{
  unsigned char name_string[MAX_ROW_FOR_MEASURMENT_VOLTAGE_SELSYN][MAX_COL_LCD] =
    {
      " Uc1c2=         ",
      " Up1p2=         ",
      " Up2p3=         "};
  unsigned int measurement_tmp[MAX_ROW_FOR_MEASURMENT_VOLTAGE_SELSYN];

  //Вибираємо відповідні вимірювання
  measurement_tmp[0] = measurement[IM_UC1C2];
  measurement_tmp[1] = measurement[IM_UP1P2];
  measurement_tmp[2] = measurement[IM_UP2P3];

  int const index_language = index_language_in_array(current_settings.language);

  //Виврдимо одиниці вимірювань
  name_string[0][MAX_COL_LCD - 1] =
    name_string[1][MAX_COL_LCD - 1] =
      name_string[2][MAX_COL_LCD - 1] = odynyci_vymirjuvannja[index_language][INDEX_V];

  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;

  index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  //Копіюємо  рядки у робочий екран
  for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
  {
    /********************************/
    //Вводимо вимірювальні значення
    /********************************/
    if (index_of_ekran < MAX_ROW_FOR_MEASURMENT_VOLTAGE_SELSYN)
    {
      unsigned int start_number_digit_after_point = 3;

      convert_and_insert_char_for_measurement(start_number_digit_after_point, measurement_tmp[index_of_ekran], 1, 1, name_string[index_of_ekran], 7);

      for (unsigned int j = 0; j < MAX_COL_LCD; j++)
        working_ekran[i][j] = name_string[index_of_ekran][j];
    }
    else
      for (unsigned int j = 0; j < MAX_COL_LCD; j++)
        working_ekran[i][j] = ' ';
    /********************************/

    index_of_ekran++;
  }

  //Курсор по горизонталі відображається на першій позиції
  current_ekran.position_cursor_x = 0;
  //Відображення курору по вертикалі
  current_ekran.position_cursor_y = position_temp & (MAX_ROW_LCD - 1);
  //Курсор видимий
  current_ekran.cursor_on = 0;
  //Курсор не мигає
  current_ekran.cursor_blinking_on = 0;
  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//Формуємо екран відображення кутів для сельсина
/*****************************************************/
void make_ekran_angle_for_selsyn(void)
{
  unsigned char name_string[MAX_ROW_FOR_MEASURMENT_ANGLE_SELSYN][MAX_COL_LCD] =
    {
      " Phi1 =         ",
      " Phi2 =         "};

  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;

  index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  //Копіюємо  рядки у робочий екран
  for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
  {
    /********************************/
    //Вводимо вимірювальні значення
    /********************************/
    if (index_of_ekran < MAX_ROW_FOR_MEASURMENT_ANGLE_SELSYN)
    {
      unsigned int meas_tmp_1, meas_tmp_2 = measurement[I_UC1C2], angle;

      if (index_of_ekran == INDEX_ML_ANGLE1)
      {
        meas_tmp_1 = measurement[I_UP1P2];
        angle = angle_UP1P2_UC1C2;
      }
      else
      {
        meas_tmp_1 = measurement[I_UP2P3];
        angle = angle_UP2P3_UC1C2;
      }

#define FIRST_POSITION_OF_NUMBER 8
      if (
        (meas_tmp_1 > PORIG_CHUTLYVOSTI_DETECTORA_KUTA) &&
        (meas_tmp_2 > PORIG_CHUTLYVOSTI_DETECTORA_KUTA))
      {
        //Розраховуємо кут
        int vaga = 100, first_symbol = 0, position = FIRST_POSITION_OF_NUMBER; /*позиція з якої поинається виведення значення - найзначуще число*/
        ;

        while (vaga > 0)
        {
          int temp_data;
          temp_data = angle / vaga; //виділяємо число, яке треба перетворити у символ і помістити у дану позицію екрану
          angle %= vaga;            //вираховуємо число без символа, який ми зараз будемо виводити на екран
          vaga /= 10;               //зменшуємо ваговий коефіцієнт в 10 разів

          //Нулі перед найстаршим значущим числом приховуємо
          if ((temp_data != 0) || (first_symbol != 0))
          {
            name_string[index_of_ekran][position] = temp_data + 0x30;
            if (first_symbol == 0)
              first_symbol = 1;
          }
          else
          {
            //Нуль виводимо тільки у тому випадку, якщо це є символ одиниць числа (текуча вага числа рівна 1)
            if (vaga >= 1)
              name_string[index_of_ekran][position] = ' ';
            else
            {
              name_string[index_of_ekran][position] = temp_data + 0x30;
              if (first_symbol == 0)
                first_symbol = 1;
            }
          }
          position++;
        }
        name_string[index_of_ekran][position] = '°';
      }
      else
      {
        const unsigned char undefined[MAX_NAMBER_LANGUAGE][MAX_COL_LCD - FIRST_POSITION_OF_NUMBER] =
          {
            "Неопред.",
            "Невизнач",
            "Undef.  ",
            "Неопред."};
        int index_language = index_language_in_array(current_settings.language);

        for (int j = 0; j < (MAX_COL_LCD - FIRST_POSITION_OF_NUMBER); j++)
          name_string[index_of_ekran][FIRST_POSITION_OF_NUMBER + j] = undefined[index_language][j];
      }
#undef FIRST_POSITION_OF_NUMBER

      for (unsigned int j = 0; j < MAX_COL_LCD; j++)
        working_ekran[i][j] = name_string[index_of_ekran][j];
    }
    else
      for (unsigned int j = 0; j < MAX_COL_LCD; j++)
        working_ekran[i][j] = ' ';
    /********************************/

    index_of_ekran++;
  }

  //Курсор по горизонталі відображається на першій позиції
  current_ekran.position_cursor_x = 0;
  //Відображення курору по вертикалі
  current_ekran.position_cursor_y = position_temp & (MAX_ROW_LCD - 1);
  //Курсор видимий
  current_ekran.cursor_on = 0;
  //Курсор не мигає
  current_ekran.cursor_blinking_on = 0;
  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//Формуємо екран відображення напруг з логометра
/*****************************************************/
void make_ekran_logomentr_voltage(void)
{
  const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_VOLTAGE_LOGOMETR][MAX_COL_LCD] =
    {
      {"Uлог.0=        В",
       "Uлог.1=        В"},
      {"Uлог.0=        В",
       "Uлог.1=        В"},
      {" BZ U =        V",
       " BO U =        V"},
      {"Uлог.0=        В",
       "Uлог.1=        В"}};
  int index_language = index_language_in_array(current_settings.language);

  unsigned char name_string_tmp[MAX_ROW_FOR_VOLTAGE_LOGOMETR][MAX_COL_LCD];
  for (unsigned int i = 0; i < MAX_ROW_FOR_VOLTAGE_LOGOMETR; i++)
  {
    for (unsigned int j = 0; j < MAX_COL_LCD; j++)
      name_string_tmp[i][j] = name_string[index_language][i][j];
  }

  //Виводимо відповідні вимірювання
  int measurement_tmp[MAX_ROW_FOR_VOLTAGE_LOGOMETR];
  //#ifdef DEBUG_TEST
  //  /***/
  //  //Тільки для відладки
  //  /***/
  //  measurement_tmp[0] = temp_adc2_channel0_global_values;
  //  measurement_tmp[1] = temp_adc2_channel1_global_values;
  //  /***/
  //#else
  semaphore_measure_values_low = 1;
  measurement_tmp[0] = adc2_channel0_averange_low;
  measurement_tmp[1] = adc2_channel1_averange_low;
  semaphore_measure_values_low = 0;
  //#endif
  /*
  Переводимо це число у мВ
   10000 (мВ) -  4095
  -10000 (мВ) - -4095
  
  Тоді ціна поділуи одної одиниці АЦП 10000/4095(мВ)
  */
  measurement_tmp[0] = measurement_tmp[0] * 10000 / 4095;
  measurement_tmp[1] = measurement_tmp[1] * 10000 / 4095;

  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;

  index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  //Копіюємо  рядки у робочий екран
  for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
  {
    /********************************/
    //Вводимо вимірювальні значення
    /********************************/
    if (index_of_ekran < MAX_ROW_FOR_VOLTAGE_LOGOMETR)
    {
      int meas_tmp = measurement_tmp[index_of_ekran];

#define FIRST_POSITION_OF_NUMBER 7
      unsigned int position = FIRST_POSITION_OF_NUMBER; /*позиція з якої поинається виведення значення*/
      ;
      if (meas_tmp < 0)
      {
        name_string_tmp[index_of_ekran][position] = '-';
        meas_tmp *= -1;
      }
      position++;

      if (meas_tmp <= 99999)
      {
        //Розраховуємо кут
        unsigned int vaga = 10000, first_symbol = 0;

        while (vaga > 0)
        {
          int temp_data;
          temp_data = meas_tmp / vaga; //виділяємо число, яке треба перетворити у символ і помістити у дану позицію екрану
          meas_tmp %= vaga;            //вираховуємо число без символа, який ми зараз будемо виводити на екран
          vaga /= 10;                  //зменшуємо ваговий коефіцієнт в 10 разів

          //Нулі перед найстаршим значущим числом приховуємо
          if ((temp_data != 0) || (first_symbol != 0))
          {
            name_string_tmp[index_of_ekran][position] = temp_data + 0x30;
            if (first_symbol == 0)
              first_symbol = 1;
          }
          else
          {
            //Нуль виводимо тільки у тому випадку, якщо це є символ одиниць цілої частини числа (текуча вага числа рівна 1000)
            if (vaga >= 1000)
              name_string_tmp[index_of_ekran][position] = ' ';
            else
            {
              name_string_tmp[index_of_ekran][position] = temp_data + 0x30;
              if (first_symbol == 0)
                first_symbol = 1;
            }
          }

          if (vaga == 100)
          {
            //Переходимо на дробову частину
            name_string_tmp[index_of_ekran][++position] = '.';
          }

          position++;
        }
      }
      else
      {
        const unsigned char undefined[MAX_COL_LCD - FIRST_POSITION_OF_NUMBER - 1 - 1 - 1] = "??.???";

        for (int j = 0; j < (MAX_COL_LCD - FIRST_POSITION_OF_NUMBER - 1 - 1 - 1); j++)
          name_string_tmp[index_of_ekran][FIRST_POSITION_OF_NUMBER + 1 + j] = undefined[j];
      }
#undef FIRST_POSITION_OF_NUMBER

      for (unsigned int j = 0; j < MAX_COL_LCD; j++)
        working_ekran[i][j] = name_string_tmp[index_of_ekran][j];
    }
    else
      for (unsigned int j = 0; j < MAX_COL_LCD; j++)
        working_ekran[i][j] = ' ';
    /********************************/

    index_of_ekran++;
  }

  //Курсор по горизонталі відображається на першій позиції
  current_ekran.position_cursor_x = 0;
  //Відображення курору по вертикалі
  current_ekran.position_cursor_y = position_temp & (MAX_ROW_LCD - 1);
  //Курсор видимий
  current_ekran.cursor_on = 0;
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
  semaphore_measure_values_low = 1;
  int measurement_fequency = (int) (frequency_middle * 1000);
  semaphore_measure_values_low = 0;

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
//
/*****************************************************/
/*****************************************************/
