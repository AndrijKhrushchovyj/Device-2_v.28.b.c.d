#include "header.h"

/*****************************************************/
//Формуємо екран відображення заголовків настроювання комунікації
/*****************************************************/
void make_ekran_chose_communication_parameters(void)
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_CHOSE_COMMUNICATION_PARAMETERS][MAX_COL_LCD] = 
  {
    {
      " Имя ячейки     ",
      " Настр.RS-485   "
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                        ,
      " Ethernet       "
#endif
    },
    {
      " Ім'я комірки   ",
      " Налашт.RS-485  "
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                        ,
      " Ethernet       "
#endif
    },
    {
      " Bay name       ",
      " RS-485 Settings"
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                        ,
      " Ethernet       "
#endif
    },
    {
      " Имя ячейки     ",
      " Настр.RS-485   "
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                        ,
      " Ethernet       "
#endif
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
    if (index_of_ekran < MAX_ROW_FOR_CHOSE_COMMUNICATION_PARAMETERS)
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
//Формуємо екран відображення імені ячейки
/*****************************************************/
void make_ekran_name_of_cell(void)
{
  //Можливо треба буде додати  нові символи при зміні мови цією функцією
  index_language_in_array(current_settings.language);
  
  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  
  index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;
  
  //Копіюємо  рядки у робочий екран
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    //Наступні рядки треба перевірити, чи їх требе відображати у текучій кофігурації
    if (index_of_ekran < MAX_ROW_FOR_VIEW_NAME_OF_CELL)
      for (unsigned int j = 0; j<MAX_COL_LCD; j++)
      {
        if (((index_of_ekran<<POWER_MAX_COL_LCD) + j) < MAX_CHAR_IN_NAME_OF_CELL) working_ekran[i][j] = current_settings.name_of_cell[(index_of_ekran<<POWER_MAX_COL_LCD) + j];
        else working_ekran[i][j] = ' ';
      }
    else
      for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';

    index_of_ekran++;
  }

  //Курсор по горизонталі відображається на першій позиції
  current_ekran.position_cursor_x = 0;
  //Відображення курору по вертикалі
  current_ekran.position_cursor_y = position_temp & (MAX_ROW_LCD - 1);
  //Курсор  не видимий
  current_ekran.cursor_on = 0;
  //Курсор не мигає
  current_ekran.cursor_blinking_on = 0;
  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//Формуємо екран відображення адреси для комунікації з верхнім рівнем
/*****************************************************/
void make_ekran_address()
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_ADDRESS][MAX_COL_LCD] = 
  {
    "     Адрес      ",
    "     Адреса     ",
    "    Address     ",
    "     Адрес      "
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
    if (index_of_ekran_tmp < MAX_ROW_FOR_ADDRESS)
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][index_of_ekran_tmp][j];
        vaga = 100; //максимальний ваговий коефіцієнт для адреси
        if (view == true) value = current_settings.address; //у змінну value поміщаємо значення адреси
        else value = edition_settings.address;
        first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
      }
      else
      {
        //У парному номері рядку виводимо значення уставки
        for (unsigned int j = 0; j<MAX_COL_LCD; j++)
        {
          if ((j < COL_ADDRESS_BEGIN) ||  (j > COL_ADDRESS_END ))working_ekran[i][j] = ' ';
          else
            calc_int_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, view);
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
    current_ekran.position_cursor_x = COL_ADDRESS_BEGIN;
    int last_position_cursor_x = COL_ADDRESS_END;

    //Підтягуємо курсор до першого символу
    while (((working_ekran[current_ekran.position_cursor_y][current_ekran.position_cursor_x + 1]) == ' ') && 
           (current_ekran.position_cursor_x < (last_position_cursor_x -1))) current_ekran.position_cursor_x++;

    //Курсор ставимо так, щоб він був перед числом
    if (((working_ekran[current_ekran.position_cursor_y][current_ekran.position_cursor_x]) != ' ') && 
        (current_ekran.position_cursor_x > 0)) current_ekran.position_cursor_x--;
  }
  //Курсор видимий, якщо ми у режимі редагування
  if (current_ekran.edition == 0) current_ekran.cursor_on = 0;
  else current_ekran.cursor_on = 1;
  
  //Курсор не мигає
  if(current_ekran.edition == 0)current_ekran.cursor_blinking_on = 0;
  else current_ekran.cursor_blinking_on = 1;
  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//Формуємо екран відображення заголовків настроювання комунікації
/*****************************************************/
void make_ekran_chose_setting_rs485(void)
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_CHOSE_SETTING_RS485][MAX_COL_LCD] = 
  {
    {
      " Физический ур. ",
      " Протокол       "
    },
    {
      " Фізичний рівень",
      " Протокол       "
    },
    {
      " Physical layer ",
      " Protocol       "
    },
    {
      " Физический ур. ",
      " Протокол       "
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
    if (index_of_ekran < MAX_ROW_FOR_CHOSE_SETTING_RS485)
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
//Формуємо екран відображення заголовків настроювання фізичного рівня RS-485
/*****************************************************/
void make_ekran_phy_layer_rs485(void)
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_PHY_LAYER_RS485][MAX_COL_LCD] = 
  {
    {
      " Скорость обмена",
      " Контр.четности ",
      " Колич.стоп-бит ",
      " Конец приёма   "
    },
    {
      " Швидкість обм. ",
      " Контр.парності ",
      "Кільк.стоп-біт  ",
      "Кінець приймання"
    },
    {
      " Baud rate      ",
      " Parity         ",
      " Stop Bits      ",
      "End of Reception"
    },
    {
      " Скорость обмена",
      " Контр.четности ",
      " Колич.стоп-бит ",
      " Конец приёма   "
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
    if (index_of_ekran < MAX_ROW_FOR_PHY_LAYER_RS485)
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
//Формуємо екран відображення заголовків настроювання протоколів для RS-485
/*****************************************************/
void make_ekran_protocols_rs485(void)
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_PROTOCOLS_RS485][MAX_COL_LCD] = 
  {
    " Адрес Modbus   ",
    " Адреса Modbus  ",
    " Modbus Address ",
    " Адрес Modbus   "
  };
  int index_language = index_language_in_array(current_settings.language);
  
  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  
  
  index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  
  //Копіюємо  рядки у робочий екран
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    //Наступні рядки треба перевірити, чи їх требе відображати у текучій коффігурації
    if (index_of_ekran < MAX_ROW_FOR_PROTOCOLS_RS485)
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
//Формуємо екран відображення швидкостіобміну для інтерфейсу
/*****************************************************/
void make_ekran_speed_interface()
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_VIEW_SPEED_INTERFACE][MAX_COL_LCD] = 
  {
    " Скорость обмена",
    " Швидкість обм. ",
    "   Baud rate    ",
    " Скорость обмена"
  };
  int index_language = index_language_in_array(current_settings.language);
  
  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  
  //Множення на два величини position_temp потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
  index_of_ekran = ((position_temp<<1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    unsigned int index_of_ekran_tmp = index_of_ekran >> 1;
    if (index_of_ekran_tmp < MAX_ROW_FOR_VIEW_SPEED_INTERFACE)
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][index_of_ekran_tmp][j];
      }
      else
      {
        //У парному номері рядку виводимо значення уставки
        static const unsigned char information[MAX_NUMBER_SPEEDS_INTERFACE][MAX_COL_LCD] = 
        {
          "      9600      ",
          "     14400      ",
          "     19200      ",
          "     28800      ",
          "     38400      ",
          "     57600      ",
          "     115200     "
        };
        static const unsigned int cursor_x[MAX_NUMBER_SPEEDS_INTERFACE] = {5, 4, 4, 4, 4, 4, 4};

        unsigned int temp_data;
          
        if(current_ekran.edition == 0) temp_data = current_settings.speed_RS485;
        else temp_data = edition_settings.speed_RS485;
          
        if (temp_data < MAX_NUMBER_SPEEDS_INTERFACE)
        {
          for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = information[temp_data][j];
          current_ekran.position_cursor_x = cursor_x[temp_data];
        }
        else
        {
          static const unsigned char information_error[MAX_NAMBER_LANGUAGE][MAX_COL_LCD] = 
          {
            "     Ошибка     ",
            "    Помилка     ",
            "     Error      ",
            "     Ошибка     "
          };
          static const unsigned int cursor_x_error[MAX_NAMBER_LANGUAGE] = {4, 3, 4, 4};

          for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = information_error[index_language][j];
          current_ekran.position_cursor_x = cursor_x_error[index_language];
        }
      }
    }
    else
      for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';

    index_of_ekran++;
  }

  //Відображення курору по вертикалі і курсор завжди має бути у полі із значенням устаки
  current_ekran.position_cursor_y = ((position_temp<<1) + 1) & (MAX_ROW_LCD - 1);

  //Курсор видимий, якщо ми у режимі редагування
  if (current_ekran.edition == 0) current_ekran.cursor_on = 0;
  else current_ekran.cursor_on = 1;

  //Курсор не мигає, якщо ми у режимі редагування
  if(current_ekran.edition == 0)current_ekran.cursor_blinking_on = 0;
  else current_ekran.cursor_blinking_on = 1;

  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//Формуємо екран відображення контролю парності для інтерфейсу
/*****************************************************/
void make_ekran_pare_interface()
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_VIEW_PARE_INTERFACE][MAX_COL_LCD] = 
  {
    " Контр.четности ",
    " Контр.парності ",
    "     Parity     ",
    " Контр.четности "
  };
  int index_language = index_language_in_array(current_settings.language);
  
  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  
  //Множення на два величини position_temp потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
  index_of_ekran = ((position_temp<<1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    unsigned int index_of_ekran_tmp = index_of_ekran >> 1;
    if (index_of_ekran_tmp < MAX_ROW_FOR_VIEW_PARE_INTERFACE)
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][index_of_ekran_tmp][j];
      }
      else
      {
        static const unsigned char information[MAX_NAMBER_LANGUAGE][MAX_NUMBER_PARE_INTERFACE + 1][MAX_COL_LCD] = 
        {
          {"      Нет       ", "     Нечет.     ", "      Чет.      ", "     Ошибка     "},
          {"      Нема      ", "    Непарн.     ", "      Парн.     ", "    Помилка     "},
          {"      None      ", "      Odd       ", "      Even      ", "     Error      "},
          {"      Нет       ", "     Нечет.     ", "      Чет.      ", "     Ошибка     "}
        };
        static const unsigned int cursor_x[MAX_NAMBER_LANGUAGE][MAX_NUMBER_PARE_INTERFACE + 1] = 
        {
          {5, 4, 5, 4},
          {5, 3, 5, 3},
          {5, 5, 5, 4},
          {5, 4, 5, 4}
        };

        unsigned int temp_data;
          
        if(current_ekran.edition == 0) temp_data = current_settings.pare_bit_RS485;
        else temp_data = edition_settings.pare_bit_RS485;

        if (temp_data < MAX_NUMBER_PARE_INTERFACE)
        {
          for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = information[index_language][temp_data][j];
          current_ekran.position_cursor_x = cursor_x[index_language][temp_data];
        }
        else
        {
          for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = information[index_language][MAX_NUMBER_PARE_INTERFACE][j];
          current_ekran.position_cursor_x = cursor_x[index_language][MAX_NUMBER_PARE_INTERFACE];
        }
      }
    }
    else
      for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';

    index_of_ekran++;
  }

  //Відображення курору по вертикалі і курсор завжди має бути у полі із значенням устаки
  current_ekran.position_cursor_y = ((position_temp<<1) + 1) & (MAX_ROW_LCD - 1);

  //Курсор видимий, якщо ми у режимі редагування
  if (current_ekran.edition == 0) current_ekran.cursor_on = 0;
  else current_ekran.cursor_on = 1;

  //Курсор не мигає, якщо ми у режимі редагування
  if(current_ekran.edition == 0)current_ekran.cursor_blinking_on = 0;
  else current_ekran.cursor_blinking_on = 1;

  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//Формуємо екран відображення stop-bit для інтерфейсу
/*****************************************************/
void make_ekran_stopbits_interface()
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_VIEW_STOP_BITS_INTERFACE][MAX_COL_LCD] = 
  {
    " Колич.стоп-бит ",
    "Кільк.стоп-біт  ",
    "   Stop Bits    ",
    " Колич.стоп-бит "
  };
  int index_language = index_language_in_array(current_settings.language);
  
  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  
  //Множення на два величини position_temp потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
  index_of_ekran = ((position_temp<<1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    unsigned int index_of_ekran_tmp = index_of_ekran >> 1;
    if (index_of_ekran_tmp < MAX_ROW_FOR_VIEW_STOP_BITS_INTERFACE)
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][index_of_ekran_tmp][j];
      }
      else
      {
        static const unsigned char information[MAX_NAMBER_LANGUAGE][MAX_NUMBER_STOP_BITS_INTERFACE + 1][MAX_COL_LCD] = 
        {
          {"      Один      ", "      Два       ", "     Ошибка     "},
          {"      Один      ", "      Два       ", "    Помилка     "},
          {"      One       ", "      Two       ", "     Error      "},
          {"      Один      ", "      Два       ", "     Ошибка     "}
        };
        static const unsigned int cursor_x[MAX_NAMBER_LANGUAGE][MAX_NUMBER_STOP_BITS_INTERFACE + 1] = 
        {
          {5, 5, 4},
          {5, 5, 3},
          {5, 5, 4},
          {5, 5, 4}
        };

        unsigned int temp_data;
          
        if(current_ekran.edition == 0) temp_data = current_settings.number_stop_bit_RS485;
        else temp_data = edition_settings.number_stop_bit_RS485;
          
        if (temp_data < MAX_NUMBER_STOP_BITS_INTERFACE)
        {
          for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = information[index_language][temp_data][j];
          current_ekran.position_cursor_x = cursor_x[index_language][temp_data];
        }
        else
        {
          for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = information[index_language][MAX_NUMBER_STOP_BITS_INTERFACE][j];
          current_ekran.position_cursor_x = cursor_x[index_language][MAX_NUMBER_STOP_BITS_INTERFACE];
        }
      }
    }
    else
      for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';

    index_of_ekran++;
  }

  //Відображення курору по вертикалі і курсор завжди має бути у полі із значенням устаки
  current_ekran.position_cursor_y = ((position_temp<<1) + 1) & (MAX_ROW_LCD - 1);

  //Курсор видимий, якщо ми у режимі редагування
  if (current_ekran.edition == 0) current_ekran.cursor_on = 0;
  else current_ekran.cursor_on = 1;

  //Курсор не мигає, якщо ми у режимі редагування
  if(current_ekran.edition == 0)current_ekran.cursor_blinking_on = 0;
  else current_ekran.cursor_blinking_on = 1;

  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//Формуємо екран відображення time-out наступного символу для інтерфейсу
/*****************************************************/
void make_ekran_timeout_interface()
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_VIEW_TIMEOUT_INTERFACE][MAX_COL_LCD] = 
  {
    "  Конец приёма  ",
    "Кінець приймання",
    "End of Reception",
    "  Конец приёма  "
  };
  static const unsigned char symbols[MAX_NAMBER_LANGUAGE][5] = {"симв.", "симв.", "symb ", "симв."};
  int index_language = index_language_in_array(current_settings.language);
  
  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  unsigned int vaga, value, first_symbol;
  
  //Множення на два величини position_temp потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
  index_of_ekran = ((position_temp<<1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    //Наступні рядки треба перевірити, чи їх требе відображати у текучій кофігурації
    unsigned int index_of_ekran_tmp = index_of_ekran >> 1;
    unsigned int view = ((current_ekran.edition == 0) || (position_temp != index_of_ekran_tmp));
    if (index_of_ekran_tmp < MAX_ROW_FOR_VIEW_TIMEOUT_INTERFACE)
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][index_of_ekran_tmp][j];

        vaga = 100; //максимальний ваговий коефіцієнт для вилілення старшого розряду
        if (view == true) value = current_settings.time_out_1_RS485; //у змінну value поміщаємо значення time-out наступного символу 
        else value = edition_settings.time_out_1_RS485;
        first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
      }
      else
      {
        //У парному номері рядку виводимо значення уставки
        for (unsigned int j = 0; j<MAX_COL_LCD; j++)
        {
          if (
              ((j < COL_TIMEOUT_INTERFACE_BEGIN) ||  (j > COL_TIMEOUT_INTERFACE_END )) &&
              (
               !((j >= (COL_TIMEOUT_INTERFACE_END + 2)) && (j <= (COL_TIMEOUT_INTERFACE_END + 6)))
              )    
             )working_ekran[i][j] = ' ';
          else if (j == COL_TIMEOUT_INTERFACE_COMMA )working_ekran[i][j] = ',';
          else if ((j >= (COL_TIMEOUT_INTERFACE_END + 2)) && (j <= (COL_TIMEOUT_INTERFACE_END + 6)))
            working_ekran[i][j] = symbols[index_language][j - (COL_TIMEOUT_INTERFACE_END + 2)];
          else
            calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TIMEOUT_INTERFACE_COMMA, view, 0);
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
    current_ekran.position_cursor_x = COL_TIMEOUT_INTERFACE_BEGIN;
    int last_position_cursor_x = COL_TIMEOUT_INTERFACE_END;
    
    //Підтягуємо курсор до першого символу
    while (((working_ekran[current_ekran.position_cursor_y][current_ekran.position_cursor_x + 1]) == ' ') && 
           (current_ekran.position_cursor_x < (last_position_cursor_x -1))) current_ekran.position_cursor_x++;

    //Курсор ставимо так, щоб він був перед числом
    if (((working_ekran[current_ekran.position_cursor_y][current_ekran.position_cursor_x]) != ' ') && 
        (current_ekran.position_cursor_x > 0)) current_ekran.position_cursor_x--;
  }
  //Курсор видимий, якщо ми у режимі редагування
  if (current_ekran.edition == 0) current_ekran.cursor_on = 0;
  else current_ekran.cursor_on = 1;

  //Курсор не мигає
  if(current_ekran.edition == 0)current_ekran.cursor_blinking_on = 0;
  else current_ekran.cursor_blinking_on = 1;
  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//Формуємо екран відображення заголовків настроювання Ethernet
/*****************************************************/
void make_ekran_chose_setting_Ethernet(void)
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_CHOSE_SETTING_ETHERNET][MAX_COL_LCD] = 
  {
    {
      " Сетевой ур.    "/*,
      " Протокол       "*/
    },
    {
      " Мережевий р-нь "/*,
      " Протокол       "*/
    },
    {
      " Network layer  "/*,
      " Protocol       "*/
    },
    {
      " Сетевой ур.    "/*,
      " Протокол       "*/
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
    if (index_of_ekran < MAX_ROW_FOR_CHOSE_SETTING_ETHERNET)
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

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
/*****************************************************/
//Формуємо екран відображення налаштувань мережевого рівня Ethernet
/*****************************************************/
void make_ekran_settings_network_layer_Ethernet()
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_SETTING_NETWORK_LAYER_ETHERNET][MAX_COL_LCD] = 
  {
    {
      "      IPv4      ",
      "     Маска      ",
      "      Шлюз      "
    }, 
    {
      "      IPv4      ",
      "     Маска      ",
      "      Шлюз      "
    }, 
    {
      "      IPv4      ",
      "      Mask      ",
      "    Gateway     "
    },
    {
      "      IPv4      ",
      "     Маска      ",
      "      Шлюз      "
    }  
  };
  int index_language = index_language_in_array(current_settings.language);
  
  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  
  //Множення на два величини position_temp потрібне для того, бо наодн позицію ми використовуємо два рядки (назва + значення)
  index_of_ekran = ((position_temp<<1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    uint32_t index_of_ekran_tmp = index_of_ekran >> 1;
    unsigned int view = ((current_ekran.edition == 0) || (position_temp != index_of_ekran_tmp));
    if (index_of_ekran_tmp < MAX_ROW_FOR_SETTING_NETWORK_LAYER_ETHERNET)
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][index_of_ekran_tmp][j];
      }
      else
      {
        //У парному номері рядку виводимо значення
        static const uint8_t string[] = "0123456789";
        __SETTINGS *point_1 = (view == true) ? &current_settings : &edition_settings;
        
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';
        if (
            (index_of_ekran_tmp == INDEX_ML_NL_IPV4) ||
            (index_of_ekran_tmp == INDEX_ML_NL_GATEWAY)
           )   
        {
          uint16_t *point_2 = NULL;
          if (index_of_ekran_tmp == INDEX_ML_NL_IPV4) point_2 = point_1->IP4;
          else if (index_of_ekran_tmp == INDEX_ML_NL_GATEWAY) point_2 = point_1->gateway;
          else total_error_sw_fixed();
          
          uint16_t array[4] = {point_2[0], point_2[1], point_2[2], point_2[3]};
          
          working_ekran[i][4] = working_ekran[i][8] = working_ekran[i][12] = '.';

          uint32_t j = COL_IP4_GATEWAY_BEGIN + 3 - 1;
          for (size_t k1 = 0; k1 < 4; k1++)
          {
            for (size_t k2 = 0; k2 < 3; k2++)
            {
              uint32_t val = array[k1] % 10;
              array[k1] /= 10;
              if (
                  (current_ekran.edition != 0) ||
                  (k2 == 0) ||
                  (val != 0) ||
                  (array[k1] != 0)  
                 )
              {
                working_ekran[i][j--] = string[val];
              }
              else
              {
                 working_ekran[i][j--] = ' ';
              }
            }
            j += (3 + 1 + 3);
          }
        }
        else if (index_of_ekran_tmp == INDEX_ML_NL_MASK)
        {
          uint32_t maska_tmp = point_1->mask;
        
          uint32_t j = COL_MASK_BEGIN + 2 - 1;
          for (size_t k2 = 0; k2 < 2; k2++)
          {
            uint32_t val = maska_tmp % 10;
            maska_tmp /= 10;
            if (
                (current_ekran.edition != 0) ||
                (k2 == 0) ||
                (val != 0) ||
                (maska_tmp != 0)  
               )
            {
              working_ekran[i][j--] = string[val];
            }
            else
            {
               working_ekran[i][j--] = ' ';
            }
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
    if (
        (current_ekran.index_position == INDEX_ML_NL_IPV4) ||
        (current_ekran.index_position == INDEX_ML_NL_GATEWAY) 
       )   
    {
      current_ekran.position_cursor_x = COL_IP4_GATEWAY_BEGIN;
      last_position_cursor_x = COL_IP4_GATEWAY_END;
    }
    else if (current_ekran.index_position == INDEX_ML_NL_MASK) 
    {
      current_ekran.position_cursor_x = COL_MASK_BEGIN;
      last_position_cursor_x = COL_MASK_END;
    }

    //Підтягуємо курсор до першого символу
    while (((working_ekran[current_ekran.position_cursor_y][current_ekran.position_cursor_x + 1]) == ' ') && 
           (current_ekran.position_cursor_x < (last_position_cursor_x -1))) current_ekran.position_cursor_x++;

    //Курсор ставимо так, щоб він був перед числом
    if (((working_ekran[current_ekran.position_cursor_y][current_ekran.position_cursor_x]) != ' ') && 
        (current_ekran.position_cursor_x > 0)) current_ekran.position_cursor_x--;
  }

  //Курсор видимий, якщо ми у режимі редагування
  current_ekran.cursor_on = 1;
  
  //Курсор не мигає
  if(current_ekran.edition == 0)current_ekran.cursor_blinking_on = 0;
  else current_ekran.cursor_blinking_on = 1;
  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/
#endif

/*****************************************************/
//
/*****************************************************/
/*****************************************************/
