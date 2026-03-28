#include "header.h"

static __CURRENT_EKRAN current_ekran_reserv_repr;
static unsigned char working_ekran_reserv_repr[MAX_ROW_LCD][MAX_COL_LCD];

/*****************************************************/
//Функція меню з якої здійснюються всі інші операції
/*****************************************************/
void main_manu_function(void)
{
  //Перевіряємо чи не треба відобразити інформацію про режим перпрограмування або спрацювання захистів
  if (
    (
      (_GET_STATE(reprogram_device, UNSUPPORT_REPROGRAM)) ||
      (_CHECK_SET_BIT(diagnostyka, WARNING_REPROGRAM) != 0)) &&
    (current_ekran.current_level != EKRAN_UNSUPPORT_REPROGRAM) &&
    (current_ekran.current_level != EKRAN_REPROGRAM_ACTIVE) &&
    (new_state_keyboard == 0))
  {
    //Запам'ятовуємо поперердній стан меню
    current_ekran_reserv_repr = current_ekran;
    //Запам'ятовуємо попередній екран (бо діалогові вікна не обновлюються при виконанні команди REWRITE)
    for (size_t i = 0; i < MAX_ROW_LCD; ++i)
    {
      for (size_t j = 0; j < MAX_COL_LCD; ++j)
        working_ekran_reserv_repr[i][j] = working_ekran[i][j];
    }

    //Переходимо на меню відображення повідомлення, що ми у режимі репрограмування
    current_ekran.current_level = (_CHECK_SET_BIT(diagnostyka, WARNING_REPROGRAM) != 0) ? EKRAN_REPROGRAM_ACTIVE : EKRAN_UNSUPPORT_REPROGRAM;
    current_ekran.index_position = -1 /*position_in_current_level_menu[current_ekran.current_level]*/;
    current_ekran.edition = 0;
    current_ekran.cursor_on = 0;
    current_ekran.cursor_blinking_on = 0;

    //Виставляємо команду на обновлекння нового екрану, а всі попередні натискування відміняємо
    new_state_keyboard = (1u << BIT_REWRITE);
  }
  else if (
    (
      ((current_ekran.current_level == EKRAN_REPROGRAM_ACTIVE) && (_CHECK_SET_BIT(diagnostyka, WARNING_REPROGRAM) == 0)) ||
      ((current_ekran.current_level == EKRAN_UNSUPPORT_REPROGRAM) && (!_GET_STATE(reprogram_device, UNSUPPORT_REPROGRAM)))) &&
    (new_state_keyboard == 0))
  {
    //Переходимо у попереднє меню
    current_ekran = current_ekran_reserv_repr;

    //Відображаємо попередній екран (бо діалогові вікна не обновлюються при виконанні команди REWRITE)
    for (size_t i = 0; i < MAX_ROW_LCD; ++i)
    {
      for (size_t j = 0; j < MAX_COL_LCD; ++j)
        working_ekran[i][j] = working_ekran_reserv_repr[i][j];
    }
    //Обновити повністю весь екран
    current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
    time_rewrite = MAX_TIME_REWRITE_EKRAN;

    new_state_keyboard = (1u << BIT_REWRITE); //Ця команда буде виконана після поперднього відновлення стану екрану, який був до переходу у вікно відображення інформації про програмування або вікно, яке говорить, що цей режим не підтримується
    return;                                   //цим виходом я перериваю зараз виконання цієї функції, щоб спочатку булоа виведена попередня інформація, к потім вже відпрацюав біт BIT_REWRITE
  }

  //Перевіряємо чи якась кнопка натиснута
  if (new_state_keyboard != 0)
  {
    static unsigned int pervynna_vtorynna; //Зміна для відображенні або значень із первинної обмоки, або із вторинної обмотки

    //Аналізуємо в якому ми зараз робочому екрані і виконуємо відповідні дії
    switch (current_ekran.current_level)
    {
        /******************************************************************************************************************************************/
      case EKRAN_LEVEL_PASSWORD:
      case EKRAN_LEVEL_PASSWORD_HARD:
      case EKRAN_LEVEL_PASSWORD_REPROGRAM:
        {
          //Зміння для фіксації стану курсору з попреднього рівня меню
          static __PREVIOUS_STATE_CURSOR previous_state_cursor;
          //Змінні для фіксації введеного паролю
          static unsigned int new_password;
          static unsigned int number_symbols;

          //Очищаємо всі біти краім упралінських
          new_state_keyboard &= (1u << BIT_KEY_ENTER) |
                                (1u << BIT_KEY_ESC) |
                                (1u << BIT_KEY_UP) |
                                (1u << BIT_KEY_DOWN) |
                                (1u << BIT_KEY_RIGHT) |
                                (1u << BIT_KEY_LEFT) |
                                (1u << BIT_REWRITE);

          if (new_state_keyboard != 0)
          {
            //Пріоритет стоїть на обновлені екрану
            if ((new_state_keyboard & (1u << BIT_REWRITE)) != 0)
            {
              previous_state_cursor.position_cursor_x = current_ekran.position_cursor_x;
              previous_state_cursor.cursor_on = current_ekran.cursor_on;
              previous_state_cursor.cursor_blinking_on = current_ekran.cursor_blinking_on;
              current_ekran.cursor_on = 1;
              current_ekran.cursor_blinking_on = 1;
              current_ekran.position_cursor_x = COL_NEW_PASSWORD_BEGIN;
              position_in_current_level_menu[current_ekran.current_level] = 1;
              //Скидаємо новий пароль у нуль і скидаємо кількість введених символів
              new_password = 0;
              number_symbols = 0;

              //Формуємо екран рівня password
              make_ekran_level_password(new_password, 0);
              //Очищаємо біт обновлення екрану
              new_state_keyboard &= ~(1u << BIT_REWRITE);
            }
            else
            {
              if (new_state_keyboard == (1u << BIT_KEY_ENTER))
              {
                //Натиснута кнопка ENTER
                unsigned int password = 0;
                if (current_ekran.current_level == EKRAN_LEVEL_PASSWORD)
                  password = current_settings.password1;
                else if (current_ekran.current_level == EKRAN_LEVEL_PASSWORD_HARD)
                  password = current_settings.password2;
                else if (current_ekran.current_level == EKRAN_LEVEL_PASSWORD_REPROGRAM)
                  password = current_settings.password3;
                else
                {
                  //Теоретично цього ніколи не мало б бути
                  total_error_sw_fixed();
                }

                if (new_password == password)
                {
                  //Пароль зійшовся
                  //Переходимо у попереднє меню у режимі редагування
                  current_ekran.current_level = previous_level_in_current_level_menu[current_ekran.current_level];
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 1;
                  current_ekran.position_cursor_x = previous_state_cursor.position_cursor_x;
                  current_ekran.cursor_on = previous_state_cursor.cursor_on;
                  current_ekran.cursor_blinking_on = previous_state_cursor.cursor_blinking_on;
                }
                else
                {
                  //Пароль не зійшовся
                  //Переходимо у попереднє меню і анульовуємо процес редагування
                  unsigned int repeat = false;
                  do
                  {
                    current_ekran.current_level = previous_level_in_current_level_menu[current_ekran.current_level];
                    current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                    current_ekran.edition = 0;
                    if (repeat == false)
                    {
                      current_ekran.position_cursor_x = previous_state_cursor.position_cursor_x;
                      current_ekran.cursor_on = previous_state_cursor.cursor_on;
                      current_ekran.cursor_blinking_on = previous_state_cursor.cursor_blinking_on;
                    }
                    else
                      break;

                    if (current_ekran.current_level == EKRAN_REPROGRAM)
                      repeat = true;
                  } while (repeat);
                }

                //Очистити сигналізацію, що натиснута кнопка
                new_state_keyboard &= ~(1u << BIT_KEY_ENTER);
                //Виставляємо біт обновлення екрану
                new_state_keyboard |= (1u << BIT_REWRITE);
              }
              else if (new_state_keyboard == (1u << BIT_KEY_ESC))
              {
                //Натиснута кнопка ESC

                //Переходимо у попереднє меню і анульовуємо процес редагування
                unsigned int repeat = false;
                do
                {
                  current_ekran.current_level = previous_level_in_current_level_menu[current_ekran.current_level];
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                  if (repeat == false)
                  {
                    current_ekran.position_cursor_x = previous_state_cursor.position_cursor_x;
                    current_ekran.cursor_on = previous_state_cursor.cursor_on;
                    current_ekran.cursor_blinking_on = previous_state_cursor.cursor_blinking_on;
                  }
                  else
                    break;

                  if (current_ekran.current_level == EKRAN_REPROGRAM)
                    repeat = true;
                } while (repeat);

                //Очистити сигналізацію, що натиснута кнопка
                new_state_keyboard &= ~(1u << BIT_KEY_ESC);
                //Виставляємо біт обновлення екрану
                new_state_keyboard |= (1u << BIT_REWRITE);
              }
              else if (new_state_keyboard == (1 << BIT_KEY_UP))
              {
                //Натиснута кнопка UP
                new_password = new_password * 10 + 1;
                number_symbols++;
                current_ekran.position_cursor_x++;

                //Формуємо екран рівня password
                make_ekran_level_password(new_password, 1);
                //Очистити сигналізацію, що натиснута кнопка
                new_state_keyboard &= (unsigned int) (~(1 << BIT_KEY_UP));
                //Перевіряємо чи не натиснуто максимальну кількість символів для паролю (4) і якщо це так, то автоматично приймаємо його
                if (number_symbols >= 4)
                  new_state_keyboard |= (1u << BIT_KEY_ENTER);
              }
              else if (new_state_keyboard == (1u << BIT_KEY_DOWN))
              {
                //Натиснута кнопка DOWN
                new_password = new_password * 10 + 3;
                number_symbols++;
                current_ekran.position_cursor_x++;

                //Формуємо екран рівня password
                make_ekran_level_password(new_password, 1);
                //Очистити сигналізацію, що натиснута кнопка
                new_state_keyboard &= ~(1u << BIT_KEY_DOWN);
                //Перевіряємо чи не натиснуто максимальну кількість символів для паролю (4) і якщо це так, то автоматично приймаємо його
                if (number_symbols >= 4)
                  new_state_keyboard |= (1u << BIT_KEY_ENTER);
              }
              else if (new_state_keyboard == (1u << BIT_KEY_RIGHT))
              {
                //Натиснута кнопка RIGHT
                new_password = new_password * 10 + 2;
                number_symbols++;
                current_ekran.position_cursor_x++;

                //Формуємо екран рівня password
                make_ekran_level_password(new_password, 1);
                //Очистити сигналізацію, що натиснута кнопка
                new_state_keyboard &= ~(1u << BIT_KEY_RIGHT);
                //Перевіряємо чи не натиснуто максимальну кількість символів для паролю (4) і якщо це так, то автоматично приймаємо його
                if (number_symbols >= 4)
                  new_state_keyboard |= (1u << BIT_KEY_ENTER);
              }
              else if (new_state_keyboard == (1 << BIT_KEY_LEFT))
              {
                //Натиснута кнопка LEFT
                new_password = new_password * 10 + 4;
                number_symbols++;
                current_ekran.position_cursor_x++;

                //Формуємо екран рівня password
                make_ekran_level_password(new_password, 1);
                //Очистити сигналізацію, що натиснута кнопка
                new_state_keyboard &= (unsigned int) (~(1 << BIT_KEY_LEFT));
                //Перевіряємо чи не натиснуто максимальну кількість символів для паролю (4) і якщо це так, то автоматично приймаємо його
                if (number_symbols >= 4)
                  new_state_keyboard |= (1u << BIT_KEY_ENTER);
              }
              else
              {
                //Натиснуто зразу декілька кнопок - це є невизначена ситуація, тому скидаємо сигналізацію про натиснуті кнопки і чекаємо знову
                unsigned int temp_data = new_state_keyboard;
                new_state_keyboard &= ~temp_data;
              }
            }
          }
          break;
        }
        /******************************************************************************************************************************************/

        /******************************************************************************************************************************************/
      case EKRAN_LEVEL_SET_NEW_PASSWORD1:
      case EKRAN_LEVEL_SET_NEW_PASSWORD2:
      case EKRAN_LEVEL_SET_NEW_PASSWORD3:
        {
          //Змінні для фіксації введеного паролю
          static unsigned int new_setting_password;
          static int number_symbols_new_setting_password;

          //Очищаємо всі біти краім упралінських
          unsigned int maska_keyboard_bits = (1u << BIT_KEY_ENTER) |
                                             (1u << BIT_KEY_ESC) |
                                             (1u << BIT_REWRITE);

          if (current_ekran.edition == 1)
            maska_keyboard_bits |= (1u << BIT_KEY_RIGHT) | (1u << BIT_KEY_LEFT) | (1u << BIT_KEY_UP) | (1u << BIT_KEY_DOWN);

          new_state_keyboard &= maska_keyboard_bits;

          if (new_state_keyboard != 0)
          {
            unsigned int *p_password = NULL;
            if (current_ekran.current_level == EKRAN_LEVEL_SET_NEW_PASSWORD1)
              p_password = &current_settings.password1;
            else if (current_ekran.current_level == EKRAN_LEVEL_SET_NEW_PASSWORD2)
              p_password = &current_settings.password2;
            else if (current_ekran.current_level == EKRAN_LEVEL_SET_NEW_PASSWORD3)
              p_password = &current_settings.password3;
            else
            {
              //Теоретично цього ніколи не мало б бути
              total_error_sw_fixed();
            }

            //Пріоритет стоїть на обновлені екрану
            if ((new_state_keyboard & (1u << BIT_REWRITE)) != 0)
            {
              if (current_ekran.edition == 1)
              {
                current_ekran.cursor_on = 1;
                current_ekran.cursor_blinking_on = 1;
                current_ekran.position_cursor_x = COL_NEW_PASSWORD_BEGIN;
                position_in_current_level_menu[current_ekran.current_level] = 1;
                //Встановлюємо початкове значення нового паролю і скидаємо кількість введених символів
                new_setting_password = *p_password;
                number_symbols_new_setting_password = 0;

                unsigned int temp_value = new_setting_password;
                while (temp_value != 0)
                {
                  number_symbols_new_setting_password++;
                  temp_value /= 10;
                }
                if (number_symbols_new_setting_password == 0)
                  number_symbols_new_setting_password = 1; //Це випадок коли current_settings.password1 = 0, тоді кількість символів рівна 0, бо число є "0"

                //Формуємо екран рівня password
                make_ekran_level_password(new_setting_password, 1);
                //Очищаємо біт обновлення екрану
                new_state_keyboard &= ~(1u << BIT_REWRITE);
              }
              else
              {
                //Переходимо у попереднє меню і анульовуємо процес редагування
                current_ekran.current_level = previous_level_in_current_level_menu[current_ekran.current_level];
                current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                current_ekran.edition = 0;

                //Біт, який сигналізує про обновлекння екрану не скидаємто, бо його ще раз треба перерисувати
              }
            }
            else
            {
              if (new_state_keyboard == (1u << BIT_KEY_ENTER))
              {
                //Натиснута кнопка ENTER
                if (current_ekran.edition == 1)
                {
                  //Виходимо з режиму редагування
                  if (current_settings.password1 == new_setting_password)
                  {
                    //Переходимо на попередній рівень
                    current_ekran.edition = 0;
                    current_ekran.current_level = previous_level_in_current_level_menu[current_ekran.current_level];
                    current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  }
                  else
                    current_ekran.edition = 2;

                  current_ekran.cursor_on = 0;
                  current_ekran.cursor_blinking_on = 0;
                }
                else if (current_ekran.edition == 2)
                {
                  //Помічаємо, що поле структури зараз буде змінене
                  changed_settings = CHANGED_ETAP_EXECUTION;

                  //Вводимо нове значення у дію
                  *p_password = new_setting_password;

                  //Не формуємо запис у таблиці настройок про зміну конфігурації, бо її як такої не було. Просто змінився пароль доступу з меню
                  //але ініціюємо запис у EEPROM настройок
                  //Запускаємо запис у EEPROM
                  if (_CHECK_SET_BIT(active_functions, RANG_SETTINGS_CHANGED) == 0)
                    current_settings_interfaces = current_settings;
                  _SET_BIT(control_spi1_taskes, TASK_START_WRITE_SETTINGS_EEPROM_BIT);

                  //Помічаємо, що таблиця змінилася і її треба буде з системи захистів зкопіювати у таблицю з якою працює система захистів (хоч ця операція і є зайвою, бо не було змін тих полів, які використовуються системою захистів, але це я зробив для універсальності, щоб завжди дві таблиці були ідентичні)
                  changed_settings = CHANGED_ETAP_ENDED;

                  //Вихід у режимі редагування
                  current_ekran.edition = 0;
                  //Переходимо на попередній рівень
                  current_ekran.current_level = previous_level_in_current_level_menu[current_ekran.current_level];
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.cursor_on = 0;
                  current_ekran.cursor_blinking_on = 0;
                }
                else if (current_ekran.edition == 3)
                {
                  //Вихід у режимі редагування
                  current_ekran.edition = 0;
                  //Переходимо на попередній рівень
                  current_ekran.current_level = previous_level_in_current_level_menu[current_ekran.current_level];
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.cursor_on = 0;
                  current_ekran.cursor_blinking_on = 0;
                }
                else
                {
                  //По ідеї сюди програма ніколи б не мала дійти і виставляється повідомлення про помилку
                  current_ekran.edition = 3;
                }

                if (current_ekran.edition == 2)
                  make_ekran_ask_rewrite();
                else if (current_ekran.edition == 0)
                {
                  //Виставляємо біт обновлення екрану
                  new_state_keyboard |= (1u << BIT_REWRITE);
                }
                else if (current_ekran.edition == 3)
                {
                  static unsigned char const information_about_error[MAX_NAMBER_LANGUAGE][MAX_COL_LCD] =
                    {
                      " Неопред.ошибка ",
                      " Невизн.помилка ",
                      " Undefined error",
                      " Неопред.ошибка "};
                  current_ekran.cursor_on = 0;
                  current_ekran.cursor_blinking_on = 0;
                  make_ekran_about_error(information_about_error);
                }

                //Очистити сигналізацію, що натиснута кнопка
                new_state_keyboard &= ~(1u << BIT_KEY_ENTER);
              }
              else if (new_state_keyboard == (1u << BIT_KEY_ESC))
              {
                //Натиснута кнопка ESC

                //Переходимо у попереднє меню і анульовуємо процес редагування
                current_ekran.current_level = previous_level_in_current_level_menu[current_ekran.current_level];
                current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                current_ekran.edition = 0;

                //Очистити сигналізацію, що натиснута кнопка
                new_state_keyboard &= ~(1u << BIT_KEY_ESC);
                //Виставляємо біт обновлення екрану
                new_state_keyboard |= (1u << BIT_REWRITE);
              }
              else if (new_state_keyboard == (1 << BIT_KEY_UP))
              {
                //Натиснута кнопка UP
                unsigned int vaga = 1, temp_value, ostacha, vyshchi_rozrjady;
                int edit_rozrjad;

                for (int i = (current_ekran.position_cursor_x - 5 + 1); i < number_symbols_new_setting_password; i++)
                  vaga *= 10;
                ostacha = new_setting_password % vaga;
                temp_value = (new_setting_password / vaga);
                edit_rozrjad = temp_value % 10;
                vyshchi_rozrjady = temp_value / 10;
                if (++edit_rozrjad > 4)
                {
                  if (ostacha == 0)
                  {
                    edit_rozrjad = 0;
                    if (number_symbols_new_setting_password > 1)
                    {
                      number_symbols_new_setting_password--;
                      current_ekran.position_cursor_x--;
                      new_setting_password = vyshchi_rozrjady * vaga;
                    }
                    else
                    {
                      if (current_ekran.current_level != EKRAN_LEVEL_SET_NEW_PASSWORD1)
                        edit_rozrjad = 1;
                      new_setting_password = vyshchi_rozrjady * vaga * 10 + edit_rozrjad * vaga;
                    }
                  }
                  else
                  {
                    edit_rozrjad = 1;
                    new_setting_password = (vyshchi_rozrjady * vaga * 10) + edit_rozrjad * vaga + ostacha;
                  }
                }
                else
                  new_setting_password = (vyshchi_rozrjady * vaga * 10) + edit_rozrjad * vaga + ostacha;
                //Формуємо екран рівня password
                make_ekran_level_password(new_setting_password, 1);
                //Очистити сигналізацію, що натиснута кнопка
                new_state_keyboard &= (unsigned int) (~(1 << BIT_KEY_UP));
              }
              else if (new_state_keyboard == (1u << BIT_KEY_DOWN))
              {
                //Натиснута кнопка DOWN
                unsigned int vaga = 1, temp_value, ostacha, vyshchi_rozrjady;
                int edit_rozrjad;
                for (int i = (current_ekran.position_cursor_x - 5 + 1); i < number_symbols_new_setting_password; i++)
                  vaga *= 10;
                ostacha = new_setting_password % vaga;
                temp_value = (new_setting_password / vaga);
                edit_rozrjad = temp_value % 10;
                vyshchi_rozrjady = temp_value / 10;
                edit_rozrjad--;
                if (edit_rozrjad < 0)
                {
                  edit_rozrjad = 4;
                  new_setting_password = (vyshchi_rozrjady * vaga * 10) + edit_rozrjad * vaga + ostacha;
                }
                else if (edit_rozrjad == 0)
                {
                  if ((vyshchi_rozrjady != 0) || (ostacha != 0))
                  {
                    if (ostacha == 0)
                    {
                      edit_rozrjad = 0;
                      if (number_symbols_new_setting_password > 1)
                      {
                        number_symbols_new_setting_password--;
                        current_ekran.position_cursor_x--;
                        new_setting_password = vyshchi_rozrjady * vaga;
                      }
                      else
                      {
                        if (current_ekran.current_level != EKRAN_LEVEL_SET_NEW_PASSWORD1)
                          edit_rozrjad = 4;
                        new_setting_password = vyshchi_rozrjady * vaga * 10 + edit_rozrjad * vaga;
                      }
                    }
                    else
                    {
                      edit_rozrjad = 4;
                      new_setting_password = (vyshchi_rozrjady * vaga * 10) + edit_rozrjad * vaga + ostacha;
                    }
                  }
                  else
                  {
                    if (current_ekran.current_level != EKRAN_LEVEL_SET_NEW_PASSWORD1)
                      new_setting_password = 4;
                    else
                      new_setting_password = 0;
                  }
                }
                else
                  new_setting_password = (vyshchi_rozrjady * vaga * 10) + edit_rozrjad * vaga + ostacha;

                //Формуємо екран рівня password
                make_ekran_level_password(new_setting_password, 1);
                //Очистити сигналізацію, що натиснута кнопка
                new_state_keyboard &= ~(1u << BIT_KEY_DOWN);
              }
              else if (new_state_keyboard == (1u << BIT_KEY_RIGHT))
              {
                //Натиснута кнопка RIGHT
                if (new_setting_password != 0)
                {
                  if (++current_ekran.position_cursor_x > COL_NEW_PASSWORD_END)
                    current_ekran.position_cursor_x = COL_NEW_PASSWORD_BEGIN;
                  if (number_symbols_new_setting_password < (current_ekran.position_cursor_x - COL_NEW_PASSWORD_BEGIN + 1))
                  {
                    new_setting_password = new_setting_password * 10 + 1;
                    number_symbols_new_setting_password++;
                  }
                }

                //Формуємо екран рівня password
                make_ekran_level_password(new_setting_password, 1);
                //Очистити сигналізацію, що натиснута кнопка
                new_state_keyboard &= ~(1u << BIT_KEY_RIGHT);
              }
              else if (new_state_keyboard == (1 << BIT_KEY_LEFT))
              {
                //Натиснута кнопка LEFT
                if (new_setting_password != 0)
                {
                  if (--current_ekran.position_cursor_x < COL_NEW_PASSWORD_BEGIN)
                    current_ekran.position_cursor_x = COL_NEW_PASSWORD_BEGIN + number_symbols_new_setting_password - 1;
                }

                //Формуємо екран рівня password
                make_ekran_level_password(new_setting_password, 1);
                //Очистити сигналізацію, що натиснута кнопка
                new_state_keyboard &= (unsigned int) (~(1 << BIT_KEY_LEFT));
              }
              else
              {
                //Натиснуто зразу декілька кнопок - це є невизначена ситуація, тому скидаємо сигналізацію про натиснуті кнопки і чекаємо знову
                unsigned int temp_data = new_state_keyboard;
                new_state_keyboard &= ~temp_data;
              }
            }
          }
          break;
        }
        /******************************************************************************************************************************************/

        /******************************************************************************************************************************************/
      case EKRAN_MAIN:
        {
          //Очищаємо всі біти краім упралінських
          new_state_keyboard &= (1u << BIT_KEY_ENTER) |
                                (1u << BIT_KEY_UP) |
                                (1u << BIT_KEY_DOWN) |
                                (1u << BIT_REWRITE);
          //Дальше виконуємо дії, якщо натиснута кнопка на яку треба реагівати, або стоїть команда обновити екран
          if (new_state_keyboard != 0)
          {
            //Пріоритет стоїть на обновлені екрану
            if ((new_state_keyboard & (1u << BIT_REWRITE)) != 0)
            {
              do
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_EKRAN_MAIN)
                  current_ekran.index_position = 0;

                if ((current_ekran.index_position == INDEX_ML1_RPN) && ((current_settings.configuration & (1 << RPN_BIT_CONFIGURATION)) == 0))
                  current_ekran.index_position++;
                if ((current_ekran.index_position == INDEX_ML1_ZSKh) && ((current_settings.configuration & (1 << ZSKh_BIT_CONFIGURATION)) == 0))
                  current_ekran.index_position++;
                if ((current_ekran.index_position == INDEX_ML1_ZNKh) && ((current_settings.configuration & (1 << ZNKh_BIT_CONFIGURATION)) == 0))
                  current_ekran.index_position++;
                if ((current_ekran.index_position == INDEX_ML1_BRP) && ((current_settings.configuration & (1 << BRP_BIT_CONFIGURATION)) == 0))
                  current_ekran.index_position++;
                if ((current_ekran.index_position == INDEX_ML1_Umax) && ((current_settings.configuration & (1 << Umax_BIT_CONFIGURATION)) == 0))
                  current_ekran.index_position++;
                if ((current_ekran.index_position == INDEX_ML1_Umin) && ((current_settings.configuration & (1 << Umin_BIT_CONFIGURATION)) == 0))
                  current_ekran.index_position++;
                if ((current_ekran.index_position == INDEX_ML1_UP) && ((current_settings.configuration & (1 << UP_BIT_CONFIGURATION)) == 0))
                  current_ekran.index_position++;
              } while (current_ekran.index_position >= MAX_ROW_FOR_EKRAN_MAIN);
              position_in_current_level_menu[EKRAN_MAIN] = current_ekran.index_position;

              //Формуємо екран рівня головного меню
              make_ekran_main();
              //Очищаємо біт обновлення екрану
              new_state_keyboard &= ~(1u << BIT_REWRITE);
            }
            else
            {
              if (new_state_keyboard == (1u << BIT_KEY_ENTER))
              {
                int temp_current_level = current_ekran.current_level;

                //Натиснута кнопка ENTER
                if (current_ekran.index_position == INDEX_ML1_TIME)
                {
                  //Запам'ятовуємо поперердній екран
                  //Переходимо на меню часу-калібровки
                  time_rewrite = 0;
                  current_ekran.current_level = EKRAN_TIME;
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                  current_ekran.cursor_on = 0;
                  current_ekran.cursor_blinking_on = 0;

                  /**************************************************/
                  //Курсор має бути на першому символі рядка з даними
                  /**************************************************/
                  if (current_ekran.index_position == ROW_Y_)
                  {
                    current_ekran.position_cursor_x = COL_DY1;
                  }
                  else if (current_ekran.index_position == ROW_T_)
                  {
                    current_ekran.index_position = ROW_Y_;
                    current_ekran.position_cursor_x = COL_DY1;
                  }
                  else if (current_ekran.index_position == ROW_N_)
                  {
                    current_ekran.index_position = ROW_K_;
                    current_ekran.position_cursor_x = COL_SK1;
                  }
                  else if (current_ekran.index_position == ROW_K_)
                  {
                    current_ekran.position_cursor_x = COL_SK1;
                  }
                  /**************************************************/
                }
                else if (current_ekran.index_position == INDEX_ML1_MEASURMENTS)
                {
                  //Переходимо на меню вибору відображення асписку типів налогових вимірювань
                  current_ekran.current_level = EKRAN_MEASURMENT;
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                  current_ekran.cursor_on = 1;
                  current_ekran.cursor_blinking_on = 0;
                }
                else if (current_ekran.index_position == INDEX_ML1_INPUTS_OUTPUTS)
                {
                  //Переходимо на меню вибору відображення списку вибору входів-виходів для відображення їх миттєвого стану
                  current_ekran.current_level = EKRAN_LIST_INPUTS_OUTPUTS;
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                }
                else if (current_ekran.index_position == INDEX_ML1_REGISTRATORS)
                {
                  //Переходимо на меню вибору відображення списку реєстраторів
                  current_ekran.current_level = EKRAN_LIST_REGISTRATORS;
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                }
                else if (current_ekran.index_position == INDEX_ML1_LANGUAGE)
                {
                  //Переходимо на меню вибору мови відображення меню
                  current_ekran.current_level = EKRAN_VIEW_SETTING_LANGUAGE;
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                }
                else if (current_ekran.index_position == INDEX_ML1_SETTINGS)
                {
                  //Переходимо на меню настройок
                  current_ekran.current_level = EKRAN_CHOSE_SETTINGS;
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                  current_ekran.cursor_on = 1;
                  current_ekran.cursor_blinking_on = 0;
                }
                else if (current_ekran.index_position == INDEX_ML1_DIAGNOSTYKA)
                {
                  //Переходимо на меню настройок
                  current_ekran.current_level = EKRAN_DIAGNOSTYKA;
                  //Для того, щоб при першому входженні завжди список починався із першої помилки обнуляємо цю позицію
                  position_in_current_level_menu[current_ekran.current_level] = 0;
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                }
                else if (current_ekran.index_position == INDEX_ML1_KONF)
                {
                  //Переходимо на меню конфігурації
                  current_ekran.current_level = EKRAN_COFIGURATION;
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                  current_ekran.cursor_on = 1;
                  current_ekran.cursor_blinking_on = 0;
                }
                else if (
                  (current_ekran.index_position == INDEX_ML1_RPN) ||
                  (current_ekran.index_position == INDEX_ML1_ZSKh) ||
                  (current_ekran.index_position == INDEX_ML1_ZNKh) ||
                  (current_ekran.index_position == INDEX_ML1_BRP) ||
                  (current_ekran.index_position == INDEX_ML1_Umax) ||
                  (current_ekran.index_position == INDEX_ML1_Umin) ||
                  (current_ekran.index_position == INDEX_ML1_UP))
                {
                  //Переходимо на меню РПН
                  if (current_ekran.index_position == INDEX_ML1_RPN)
                    current_ekran.current_level = EKRAN_CHOOSE_SETTINGS_RPN;
                  //Переходимо на меню ЗСХ
                  else if (current_ekran.index_position == INDEX_ML1_ZSKh)
                    current_ekran.current_level = EKRAN_CHOOSE_SETTINGS_ZSKH;
                  //Переходимо на меню БРП
                  else if (current_ekran.index_position == INDEX_ML1_ZNKh)
                    current_ekran.current_level = EKRAN_CHOOSE_SETTINGS_BRP;
                  //Переходимо на меню ЗНХ
                  else if (current_ekran.index_position == INDEX_ML1_BRP)
                    current_ekran.current_level = EKRAN_CHOOSE_SETTINGS_ZNKh;
                  //Переходимо на меню ЗЗ
                  else if (current_ekran.index_position == INDEX_ML1_Umax)
                    current_ekran.current_level = EKRAN_CHOOSE_SETTINGS_UMAX;
                  //Переходимо на меню СЗНП
                  else if (current_ekran.index_position == INDEX_ML1_Umin)
                    current_ekran.current_level = EKRAN_CHOOSE_SETTINGS_UMIN;
                  //Переходимо на меню Універсальний захист
                  else if (current_ekran.index_position == INDEX_ML1_UP)
                    current_ekran.current_level = EKRAN_CHOOSE_SETTINGS_UP;

                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                  current_ekran.cursor_on = 1;
                  current_ekran.cursor_blinking_on = 0;
                }
                //У разі, якщо текучий екран змінився, то запам'ятовуємо екран в який требе буде повернутися
                if (temp_current_level != current_ekran.current_level)
                  previous_level_in_current_level_menu[current_ekran.current_level] = temp_current_level;

                //Виставляємо команду на обновлекння нового екрану
                new_state_keyboard |= (1u << BIT_REWRITE);

                //Очистити сигналізацію, що натиснута кнопка
                new_state_keyboard &= ~(1u << BIT_KEY_ENTER);
              }
              else if (new_state_keyboard == (1u << BIT_KEY_UP))
              {
                //Натиснута кнопка UP
                current_ekran.index_position--;
                do
                {
                  if (current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_FOR_EKRAN_MAIN - 1;

                  if ((current_ekran.index_position == INDEX_ML1_UP) && ((current_settings.configuration & (1 << UP_BIT_CONFIGURATION)) == 0))
                    current_ekran.index_position--;
                  if ((current_ekran.index_position == INDEX_ML1_Umin) && ((current_settings.configuration & (1 << Umin_BIT_CONFIGURATION)) == 0))
                    current_ekran.index_position--;
                  if ((current_ekran.index_position == INDEX_ML1_Umax) && ((current_settings.configuration & (1 << Umax_BIT_CONFIGURATION)) == 0))
                    current_ekran.index_position--;
                  if ((current_ekran.index_position == INDEX_ML1_BRP) && ((current_settings.configuration & (1 << BRP_BIT_CONFIGURATION)) == 0))
                    current_ekran.index_position--;
                  if ((current_ekran.index_position == INDEX_ML1_ZNKh) && ((current_settings.configuration & (1 << ZNKh_BIT_CONFIGURATION)) == 0))
                    current_ekran.index_position--;
                  if ((current_ekran.index_position == INDEX_ML1_ZSKh) && ((current_settings.configuration & (1 << ZSKh_BIT_CONFIGURATION)) == 0))
                    current_ekran.index_position--;
                  if ((current_ekran.index_position == INDEX_ML1_RPN) && ((current_settings.configuration & (1 << RPN_BIT_CONFIGURATION)) == 0))
                    current_ekran.index_position--;

                } while (current_ekran.index_position < 0);
                position_in_current_level_menu[EKRAN_MAIN] = current_ekran.index_position;

                //Формуємо екран рівня головного меню
                make_ekran_main();
                //Очистити сигналізацію, що натиснута кнопка
                new_state_keyboard &= ~(1u << BIT_KEY_UP);
              }
              else if (new_state_keyboard == (1u << BIT_KEY_DOWN))
              {
                //Натиснута кнопка DOWN
                //current_ekran.index_position вказує номер у повному списку полів
                current_ekran.index_position++;
                do
                {
                  if (current_ekran.index_position >= MAX_ROW_FOR_EKRAN_MAIN)
                    current_ekran.index_position = 0;

                  if ((current_ekran.index_position == INDEX_ML1_RPN) && ((current_settings.configuration & (1 << RPN_BIT_CONFIGURATION)) == 0))
                    current_ekran.index_position++;
                  if ((current_ekran.index_position == INDEX_ML1_ZSKh) && ((current_settings.configuration & (1 << ZSKh_BIT_CONFIGURATION)) == 0))
                    current_ekran.index_position++;
                  if ((current_ekran.index_position == INDEX_ML1_ZNKh) && ((current_settings.configuration & (1 << ZNKh_BIT_CONFIGURATION)) == 0))
                    current_ekran.index_position++;
                  if ((current_ekran.index_position == INDEX_ML1_BRP) && ((current_settings.configuration & (1 << BRP_BIT_CONFIGURATION)) == 0))
                    current_ekran.index_position++;
                  if ((current_ekran.index_position == INDEX_ML1_Umax) && ((current_settings.configuration & (1 << Umax_BIT_CONFIGURATION)) == 0))
                    current_ekran.index_position++;
                  if ((current_ekran.index_position == INDEX_ML1_Umin) && ((current_settings.configuration & (1 << Umin_BIT_CONFIGURATION)) == 0))
                    current_ekran.index_position++;
                  if ((current_ekran.index_position == INDEX_ML1_UP) && ((current_settings.configuration & (1 << UP_BIT_CONFIGURATION)) == 0))
                    current_ekran.index_position++;

                } while (current_ekran.index_position >= MAX_ROW_FOR_EKRAN_MAIN);
                position_in_current_level_menu[EKRAN_MAIN] = current_ekran.index_position;

                //Формуємо екран рівня головного меню
                make_ekran_main();
                //Очистити сигналізацію, що натиснута кнопка
                new_state_keyboard &= ~(1u << BIT_KEY_DOWN);
              }
              else
              {
                //Натиснуто зразу декілька кнопок - це є невизначена ситуація, тому скидаємо сигналізацію про натиснуті кнопки і чекаємо знову
                unsigned int temp_data = new_state_keyboard;
                new_state_keyboard &= ~temp_data;
              }
            }
          }
          break;
        }
        /******************************************************************************************************************************************/

        /******************************************************************************************************************************************/
      case EKRAN_TIME:
        {
          //Очищаємо всі біти краім упралінський
          unsigned int maska_keyboard_bits = (1u << BIT_KEY_ENTER) |
                                             (1u << BIT_KEY_ESC) |
                                             (1u << BIT_REWRITE);

          if (current_ekran.edition == 1)
            maska_keyboard_bits |= (1u << BIT_KEY_RIGHT) | (1u << BIT_KEY_LEFT) | (1u << BIT_KEY_UP) | (1u << BIT_KEY_DOWN);
          else if (current_ekran.edition == 0)
            maska_keyboard_bits |= (1u << BIT_KEY_UP) | (1u << BIT_KEY_DOWN);

          new_state_keyboard &= maska_keyboard_bits;
          //Дальше виконуємо дії, якщо натиснута кнопка на яку треба реагівати, або стоїть команда обновити екран
          if (new_state_keyboard != 0)
          {
            //Пріоритет стоїть на обновлені екрану
            if ((new_state_keyboard & (1u << BIT_REWRITE)) != 0)
            {
              if (current_ekran.edition == 0)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_EKRAN_TIME)
                  current_ekran.index_position = 0;
                /**************************************************/
                //Курсор має бути на першому символі рядка з даними
                /**************************************************/
                if (current_ekran.index_position == ROW_Y_)
                {
                  current_ekran.position_cursor_x = COL_DY1;
                }
                else if (current_ekran.index_position == ROW_T_)
                {
                  current_ekran.index_position = ROW_Y_;
                  current_ekran.position_cursor_x = COL_DY1;
                }
                else if (current_ekran.index_position == ROW_N_)
                {
                  current_ekran.index_position = ROW_K_;
                  current_ekran.position_cursor_x = COL_SK1;
                }
                else if (current_ekran.index_position == ROW_K_)
                {
                  current_ekran.position_cursor_x = COL_SK1;
                }
                /**************************************************/
                position_in_current_level_menu[EKRAN_TIME] = current_ekran.index_position;

                current_ekran.cursor_on = 0;
                current_ekran.cursor_blinking_on = 0;
              }

              //Формуємо екран години-часу
              make_ekran_time();
              //Очищаємо біт обновлення екрану
              new_state_keyboard &= ~(1u << BIT_REWRITE);
            }
            else
            {
              if (new_state_keyboard == (1u << BIT_KEY_ENTER))
              {
                static int tm_isdst;
                //Натиснута кнопка ENTER
                if (current_ekran.edition == 0)
                {
                  //Копіюємо текчий масив часу у масив для редагування
                  copying_time_dat = 1;
                  time_t time_dat_tmp = time_dat_copy;
                  int32_t time_ms_tmp = time_ms_copy;
                  copying_time_dat = 0;
                  struct tm *p;
                  p = localtime(&time_dat_tmp);

                  time_edit[0] = INT_TO_BCD(time_ms_tmp / 10);
                  time_edit[1] = INT_TO_BCD(p->tm_sec) & 0x7F;
                  time_edit[2] = INT_TO_BCD(p->tm_min) & 0x7F;
                  time_edit[3] = INT_TO_BCD(p->tm_hour) & 0x3F;
                  time_edit[4] = INT_TO_BCD(p->tm_mday) & 0x3F;
                  time_edit[5] = INT_TO_BCD(p->tm_mon + 1) & 0x1F;
                  time_edit[6] = INT_TO_BCD(p->tm_year - 100) & 0xFF;

                  calibration_edit = calibration;

                  //Підготовка до режиму редагування - включаємо мигаючий курсор
                  current_ekran.cursor_on = 1;
                  current_ekran.cursor_blinking_on = 1;
                  if (current_settings.password1 != 0)
                  {
                    //Переходимо на меню запиту паролю
                    current_ekran.current_level = EKRAN_LEVEL_PASSWORD;
                    previous_level_in_current_level_menu[current_ekran.current_level] = EKRAN_TIME;
                    current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  }
                  else
                  {
                    //Переходимо у режим редагування
                    current_ekran.edition = 1;
                  }
                }
                else if (current_ekran.edition == 1)
                {
                  //Перевіряємо чи якісь зміни відбулися
                  unsigned int found_changes = 0, i = 0;

                  copying_time_dat = 1;
                  time_t time_dat_tmp = time_dat_copy;
                  int32_t time_ms_tmp = time_ms_copy;
                  copying_time_dat = 0;
                  struct tm *p;
                  p = localtime(&time_dat_tmp);
                  tm_isdst = p->tm_isdst;

                  while ((i < 7) && (found_changes == 0))
                  {
                    switch (i)
                    {
                      case 0:
                        {
                          if (INT_TO_BCD(time_ms_tmp / 10) != time_edit[0])
                            found_changes = 1;
                          break;
                        }
                      case 1:
                        {
                          if ((INT_TO_BCD(p->tm_sec) & 0x7F) != time_edit[1])
                            found_changes = 1;
                          break;
                        }
                      case 2:
                        {
                          if ((INT_TO_BCD(p->tm_min) & 0x7F) != time_edit[2])
                            found_changes = 1;
                          break;
                        }
                      case 3:
                        {
                          if ((INT_TO_BCD(p->tm_hour) & 0x3F) != time_edit[3])
                            found_changes = 1;
                          break;
                        }
                      case 4:
                        {
                          if ((INT_TO_BCD(p->tm_mday) & 0x3F) != time_edit[4])
                            found_changes = 1;
                          break;
                        }
                      case 5:
                        {
                          if ((INT_TO_BCD(p->tm_mon + 1) & 0x1F) != time_edit[5])
                            found_changes = 1;
                          break;
                        }
                      case 6:
                        {
                          if (INT_TO_BCD(p->tm_year - 100) != time_edit[6])
                            found_changes = 1;
                          break;
                        }
                      default:
                        break;
                    }
                    i++;
                  }
                  if (found_changes == 0)
                  {
                    if (calibration != calibration_edit)
                      found_changes = 1;
                  }

                  //Виходимо з режиму редагування
                  if (found_changes == 0)
                    current_ekran.edition = 0;
                  else
                    current_ekran.edition = 2;

                  current_ekran.cursor_on = 0;
                  current_ekran.cursor_blinking_on = 0;
                }
                else if (current_ekran.edition == 2)
                {
                  //Перевіряємо достовірність даних
                  if (check_data_for_data_time_menu() == 1)
                  {
                    //Дані достовірні

                    time_ms_save_l = 0;

                    struct tm orig;
                    unsigned int tmp_reg = time_edit[1];
                    orig.tm_sec = 10 * (tmp_reg >> 4) + (tmp_reg & 0xf);

                    tmp_reg = time_edit[2];
                    orig.tm_min = 10 * (tmp_reg >> 4) + (tmp_reg & 0xf);

                    tmp_reg = time_edit[3];
                    orig.tm_hour = 10 * (tmp_reg >> 4) + (tmp_reg & 0xf);

                    tmp_reg = time_edit[4];
                    orig.tm_mday = 10 * (tmp_reg >> 4) + (tmp_reg & 0xf);

                    tmp_reg = time_edit[5];
                    orig.tm_mon = 10 * (tmp_reg >> 4) + (tmp_reg & 0xf) - 1;

                    tmp_reg = time_edit[6];
                    orig.tm_year = 10 * (tmp_reg >> 4) + (tmp_reg & 0xf) + 100;

                    orig.tm_wday = 0;
                    orig.tm_yday = 0;
                    orig.tm_isdst = (current_settings.dst & MASKA_FOR_BIT(N_BIT_TZ_DST)) ? tm_isdst : 0;
                    //Робота з Watchdog
                    watchdog_routine(UNITED_BITS_WATCHDOG, 40);
                    time_dat_save_l = mktime(&orig);
                    if (current_settings.dst & MASKA_FOR_BIT(N_BIT_TZ_DST))
                    {
                      struct tm *p_tmp = localtime(&time_dat_save_l);
                      if (tm_isdst != p_tmp->tm_isdst)
                      {
                        orig.tm_isdst = p_tmp->tm_isdst;
                        //Робота з Watchdog
                        watchdog_routine(UNITED_BITS_WATCHDOG, 41);
                        time_dat_save_l = mktime(&orig);
                      }
                    }
                    save_time_dat_l = 3;

                    calibration = calibration_edit;
                    current_ekran.edition = 0;
                  }
                  else
                  {
                    //Дані недостовірні - повідомляємо про це
                    current_ekran.edition = 3;
                  }

                  //Виходимо з режиму редагування
                  current_ekran.cursor_on = 0;
                  current_ekran.cursor_blinking_on = 0;
                }
                else if (current_ekran.edition == 3)
                {
                  //Вихід у режимі редагування
                  current_ekran.edition = 0;
                  current_ekran.cursor_on = 0;
                  current_ekran.cursor_blinking_on = 0;
                }

                if (current_ekran.edition == 2)
                  make_ekran_ask_rewrite();
                else if (current_ekran.edition == 3)
                {
                  static unsigned char const information_about_error[MAX_NAMBER_LANGUAGE][MAX_COL_LCD] =
                    {
                      " Неверные данные",
                      "  Невірні дані  ",
                      "   False data   ",
                      " Неверные данные"};
                  make_ekran_about_error(information_about_error);
                }
                else
                  //Виставляємо біт обновлення екрану
                  new_state_keyboard |= (1u << BIT_REWRITE);

                //Очистити сигналізацію, що натиснута кнопка
                new_state_keyboard &= ~(1u << BIT_KEY_ENTER);
              }
              else if (new_state_keyboard == (1u << BIT_KEY_ESC))
              {
                //Натиснута кнопка ESC

                if (current_ekran.edition == 0)
                {
                  //Вихід у режимі спостерігання
                  //Переходимо у попереднє меню
                  current_ekran.current_level = previous_level_in_current_level_menu[current_ekran.current_level];
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                }
                else
                {
                  //Вихід у режимі редагування без введення змін
                  current_ekran.edition = 0;
                  current_ekran.cursor_on = 0;
                  current_ekran.cursor_blinking_on = 0;
                }

                //Виставляємо команду на обновлекння нового екрану
                new_state_keyboard |= (1u << BIT_REWRITE);

                //Очистити сигналізацію, що натиснута кнопка
                new_state_keyboard &= ~(1u << BIT_KEY_ESC);
              }
              else if (new_state_keyboard == (1u << BIT_KEY_UP))
              {
                //Натиснута кнопка UP
                if (current_ekran.edition == 0)
                {
                  //Переміщення у режимі спостерігання
                  current_ekran.index_position -= MAX_ROW_LCD;
                  if (current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_FOR_EKRAN_TIME - MAX_ROW_LCD;
                  /**************************************************/
                  //Курсор має бути на першому символі рядка з даними
                  /**************************************************/
                  if (current_ekran.index_position == ROW_Y_)
                  {
                    current_ekran.position_cursor_x = COL_DY1;
                  }
                  else if (current_ekran.index_position == ROW_T_)
                  {
                    current_ekran.index_position = ROW_Y_;
                    current_ekran.position_cursor_x = COL_DY1;
                  }
                  else if (current_ekran.index_position == ROW_N_)
                  {
                    current_ekran.index_position = ROW_K_;
                    current_ekran.position_cursor_x = COL_SK1;
                  }
                  else if (current_ekran.index_position == ROW_K_)
                  {
                    current_ekran.position_cursor_x = COL_SK1;
                  }
                  /**************************************************/
                  position_in_current_level_menu[EKRAN_TIME] = current_ekran.index_position;
                }
                else
                {
                  //Редагування числа
                  edit_time(1);
                }

                //Формуємо екран рівня 2
                make_ekran_time();
                //Очистити сигналізацію, що натиснута кнопка
                new_state_keyboard &= ~(1u << BIT_KEY_UP);
              }
              else if (new_state_keyboard == (1u << BIT_KEY_DOWN))
              {
                //Натиснута кнопка DOWN
                if (current_ekran.edition == 0)
                {
                  //Переміщення у режимі спостерігання
                  current_ekran.index_position += MAX_ROW_LCD;
                  if (current_ekran.index_position >= MAX_ROW_FOR_EKRAN_TIME)
                    current_ekran.index_position = 0;
                  /**************************************************/
                  //Курсор має бути на першому символі рядка з даними
                  /**************************************************/
                  if (current_ekran.index_position == ROW_Y_)
                  {
                    current_ekran.position_cursor_x = COL_DY1;
                  }
                  else if (current_ekran.index_position == ROW_T_)
                  {
                    current_ekran.index_position = ROW_Y_;
                    current_ekran.position_cursor_x = COL_DY1;
                  }
                  else if (current_ekran.index_position == ROW_N_)
                  {
                    current_ekran.index_position = ROW_K_;
                    current_ekran.position_cursor_x = COL_SK1;
                  }
                  else if (current_ekran.index_position == ROW_K_)
                  {
                    current_ekran.position_cursor_x = COL_SK1;
                  }
                  /**************************************************/
                  position_in_current_level_menu[EKRAN_TIME] = current_ekran.index_position;
                }
                else
                {
                  //Редагування числа
                  edit_time(0);
                }

                //Формуємо екран години-часу
                make_ekran_time();
                //Очистити сигналізацію, що натиснута кнопка
                new_state_keyboard &= ~(1u << BIT_KEY_DOWN);
              }
              else if (new_state_keyboard == (1u << BIT_KEY_RIGHT))
              {
                if (current_ekran.index_position == ROW_Y_)
                {
                  if (current_ekran.position_cursor_x == COL_DY1)
                    current_ekran.position_cursor_x = COL_DY2;
                  else if (current_ekran.position_cursor_x == COL_DY2)
                    current_ekran.position_cursor_x = COL_MY1;
                  else if (current_ekran.position_cursor_x == COL_MY1)
                    current_ekran.position_cursor_x = COL_MY2;
                  else if (current_ekran.position_cursor_x == COL_MY2)
                    current_ekran.position_cursor_x = COL_SY1;
                  else if (current_ekran.position_cursor_x == COL_SY1)
                    current_ekran.position_cursor_x = COL_SY2;
                  else if (current_ekran.position_cursor_x == COL_SY2)
                  {
                    current_ekran.index_position = ROW_T_;
                    current_ekran.position_cursor_x = COL_HT1;
                  }
                }
                else if (current_ekran.index_position == ROW_T_)
                {
                  if (current_ekran.position_cursor_x == COL_HT1)
                    current_ekran.position_cursor_x = COL_HT2;
                  else if (current_ekran.position_cursor_x == COL_HT2)
                    current_ekran.position_cursor_x = COL_MT1;
                  else if (current_ekran.position_cursor_x == COL_MT1)
                    current_ekran.position_cursor_x = COL_MT2;
                  else if (current_ekran.position_cursor_x == COL_MT2)
                    current_ekran.position_cursor_x = COL_ST1;
                  else if (current_ekran.position_cursor_x == COL_ST1)
                    current_ekran.position_cursor_x = COL_ST2;
                  else if (current_ekran.position_cursor_x == COL_ST2)
                  {
                    current_ekran.index_position = ROW_K_;
                    current_ekran.position_cursor_x = COL_SK1;
                  }
                }
                else if (current_ekran.index_position == ROW_K_)
                {
                  if (current_ekran.position_cursor_x == COL_SK1)
                    current_ekran.position_cursor_x = COL_VK1;
                  else if (current_ekran.position_cursor_x == COL_VK1)
                    current_ekran.position_cursor_x = COL_VK2;
                  else if (current_ekran.position_cursor_x == COL_VK2)
                  {
                    current_ekran.index_position = ROW_Y_;
                    current_ekran.position_cursor_x = COL_DY1;
                  }
                }
                //Формуємо екран години-часу
                make_ekran_time();
                //Очистити сигналізацію, що натиснута кнопка
                new_state_keyboard &= ~(1u << BIT_KEY_RIGHT);
              }
              else if (new_state_keyboard == (1u << BIT_KEY_LEFT))
              {
                if (current_ekran.index_position == ROW_Y_)
                {
                  if (current_ekran.position_cursor_x == COL_SY2)
                    current_ekran.position_cursor_x = COL_SY1;
                  else if (current_ekran.position_cursor_x == COL_SY1)
                    current_ekran.position_cursor_x = COL_MY2;
                  else if (current_ekran.position_cursor_x == COL_MY2)
                    current_ekran.position_cursor_x = COL_MY1;
                  else if (current_ekran.position_cursor_x == COL_MY1)
                    current_ekran.position_cursor_x = COL_DY2;
                  else if (current_ekran.position_cursor_x == COL_DY2)
                    current_ekran.position_cursor_x = COL_DY1;
                  else if (current_ekran.position_cursor_x == COL_DY1)
                  {
                    current_ekran.index_position = ROW_K_;
                    current_ekran.position_cursor_x = COL_VK2;
                  }
                }
                else if (current_ekran.index_position == ROW_T_)
                {
                  if (current_ekran.position_cursor_x == COL_ST2)
                    current_ekran.position_cursor_x = COL_ST1;
                  else if (current_ekran.position_cursor_x == COL_ST1)
                    current_ekran.position_cursor_x = COL_MT2;
                  else if (current_ekran.position_cursor_x == COL_MT2)
                    current_ekran.position_cursor_x = COL_MT1;
                  else if (current_ekran.position_cursor_x == COL_MT1)
                    current_ekran.position_cursor_x = COL_HT2;
                  else if (current_ekran.position_cursor_x == COL_HT2)
                    current_ekran.position_cursor_x = COL_HT1;
                  else if (current_ekran.position_cursor_x == COL_HT1)
                  {
                    current_ekran.index_position = ROW_Y_;
                    current_ekran.position_cursor_x = COL_SY2;
                  }
                }
                else if (current_ekran.index_position == ROW_K_)
                {
                  if (current_ekran.position_cursor_x == COL_VK2)
                    current_ekran.position_cursor_x = COL_VK1;
                  else if (current_ekran.position_cursor_x == COL_VK1)
                    current_ekran.position_cursor_x = COL_SK1;
                  else if (current_ekran.position_cursor_x == COL_SK1)
                  {
                    current_ekran.index_position = ROW_T_;
                    current_ekran.position_cursor_x = COL_ST2;
                  }
                }
                //Формуємо екран години-часу
                make_ekran_time();
                //Очистити сигналізацію, що натиснута кнопка
                new_state_keyboard &= ~(1u << BIT_KEY_LEFT);
              }
              else
              {
                //Натиснуто зразу декілька кнопок - це є невизначена ситуація, тому скидаємо сигналізацію про натиснуті кнопки і чекаємо знову
                unsigned int temp_data = new_state_keyboard;
                new_state_keyboard &= ~temp_data;
              }
            }
          }
          break;
        }
        /******************************************************************************************************************************************/

        /******************************************************************************************************************************************/
      case EKRAN_COFIGURATION:
        {
          //Змінна для редагування конфігурації
          static unsigned int configuration_edit;

          //Очищаємо всі біти краім упралінський
          unsigned int maska_keyboard_bits = (1u << BIT_KEY_ENTER) |
                                             (1u << BIT_KEY_ESC) |
                                             (1u << BIT_REWRITE);

          if (current_ekran.edition == 1)
            maska_keyboard_bits |= (1u << BIT_KEY_RIGHT) | (1u << BIT_KEY_LEFT) | (1u << BIT_KEY_UP) | (1u << BIT_KEY_DOWN);
          else if (current_ekran.edition == 0)
            maska_keyboard_bits |= (1u << BIT_KEY_UP) | (1u << BIT_KEY_DOWN);

          new_state_keyboard &= maska_keyboard_bits;
          //Дальше виконуємо дії, якщо натиснута кнопка на яку треба реагівати, або стоїть команда обновити екран
          if (new_state_keyboard != 0)
          {
            //Пріоритет стоїть на обновлені екрану
            if ((new_state_keyboard & (1u << BIT_REWRITE)) != 0)
            {
              if (current_ekran.index_position >= MAX_ROW_FOR_EKRAN_CONFIGURATION)
                current_ekran.index_position = 0;

              position_in_current_level_menu[EKRAN_COFIGURATION] = current_ekran.index_position;
              //Формуємо екран конфігурації
              make_ekran_configuration(configuration_edit);
              //Очищаємо біт обновлення екрану
              new_state_keyboard &= ~(1u << BIT_REWRITE);
            }
            else
            {
              if (new_state_keyboard == (1u << BIT_KEY_ENTER))
              {
                //Натиснута кнопка ENTER
                if (current_ekran.edition == 0)
                {
                  //Копіюємо настройки у структуру для редагування
                  configuration_edit = current_settings.configuration;

                  //Підготовка до режиму редагування - включаємо мигаючий курсор
                  current_ekran.cursor_on = 1;
                  current_ekran.cursor_blinking_on = 1;
                  if (current_settings.password1 != 0)
                  {
                    //Переходимо на меню запиту паролю
                    current_ekran.current_level = EKRAN_LEVEL_PASSWORD;
                    previous_level_in_current_level_menu[current_ekran.current_level] = EKRAN_COFIGURATION;
                    current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  }
                  else
                  {
                    //Переходимо у режим редагування
                    current_ekran.edition = 1;
                  }
                }
                else if (current_ekran.edition == 1)
                {
                  //Перевіряємо чи якісь зміни відбулися
                  if (configuration_edit == current_settings.configuration)
                    current_ekran.edition = 0;
                  else
                    current_ekran.edition = 2;

                  current_ekran.cursor_on = 0;
                  current_ekran.cursor_blinking_on = 0;
                }
                else if (current_ekran.edition == 2)
                {
                  //Помічаємо, що поле структури зараз буде змінене
                  changed_settings = CHANGED_ETAP_EXECUTION;

                  //Обновляємо значення
                  action_after_changing_of_configuration(configuration_edit, &current_settings);
                  //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                  fix_change_settings(0, 1);
                  current_ekran.edition = 0;

                  //Виходимо з режиму редагування
                  current_ekran.cursor_on = 1;
                  current_ekran.cursor_blinking_on = 0;
                }

                if (current_ekran.edition == 2)
                  make_ekran_ask_rewrite();
                else
                  //Виставляємо біт обновлення екрану
                  new_state_keyboard |= (1u << BIT_REWRITE);

                //Очистити сигналізацію, що натиснута кнопка
                new_state_keyboard &= ~(1u << BIT_KEY_ENTER);
              }
              else if (new_state_keyboard == (1u << BIT_KEY_ESC))
              {
                //Натиснута кнопка ESC

                if (current_ekran.edition == 0)
                {
                  //Вихід у режимі спостерігання
                  //Переходимо у попереднє меню
                  current_ekran.current_level = previous_level_in_current_level_menu[current_ekran.current_level];
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                }
                else
                {
                  //Вихід у режимі редагування без введення змін
                  current_ekran.cursor_on = 1;
                  current_ekran.cursor_blinking_on = 0;
                }
                current_ekran.edition = 0;

                //Виставляємо команду на обновлекння нового екрану
                new_state_keyboard |= (1u << BIT_REWRITE);
                //Очистити сигналізацію, що натиснута кнопка
                new_state_keyboard &= ~(1u << BIT_KEY_ESC);
              }
              else if (new_state_keyboard == (1u << BIT_KEY_UP))
              {
                //Натиснута кнопка UP

                if (--current_ekran.index_position < 0)
                  current_ekran.index_position = MAX_ROW_FOR_EKRAN_CONFIGURATION - 1;

                position_in_current_level_menu[EKRAN_COFIGURATION] = current_ekran.index_position;
                //Формуємо екран конфігурації
                make_ekran_configuration(configuration_edit);
                //Очистити сигналізацію, що натиснута кнопка
                new_state_keyboard &= ~(1u << BIT_KEY_UP);
              }
              else if (new_state_keyboard == (1u << BIT_KEY_DOWN))
              {
                //Натиснута кнопка DOWN

                if (++current_ekran.index_position >= MAX_ROW_FOR_EKRAN_CONFIGURATION)
                  current_ekran.index_position = 0;

                position_in_current_level_menu[EKRAN_COFIGURATION] = current_ekran.index_position;
                //Формуємо екран конфігурації
                make_ekran_configuration(configuration_edit);
                //Очистити сигналізацію, що натиснута кнопка
                new_state_keyboard &= ~(1u << BIT_KEY_DOWN);
              }
              else if ((new_state_keyboard == (1u << BIT_KEY_LEFT)) || (new_state_keyboard == (1u << BIT_KEY_RIGHT)))
              {
                //Натиснута кнопка LEFT або RIGHT

                configuration_edit ^= (1 << current_ekran.index_position);

                //Формуємо екран конфігурації
                make_ekran_configuration(configuration_edit);
                //Очистити сигналізацію, що натиснута кнопка
                new_state_keyboard &= ~((1u << BIT_KEY_LEFT) | (1u << BIT_KEY_RIGHT));
              }
              else
              {
                //Натиснуто зразу декілька кнопок - це є невизначена ситуація, тому скидаємо сигналізацію про натиснуті кнопки і чекаємо знову
                unsigned int temp_data = new_state_keyboard;
                new_state_keyboard &= ~temp_data;
              }
            }
          }
          break;
        }
        /******************************************************************************************************************************************/

        /****************************************************************************************************************************************/
      case EKRAN_MEASURMENT:
      case EKRAN_MEASURMENT_1:
      case EKRAN_MEASURMENT_2:
      case EKRAN_MEASURMENT_SELSYN:
      case EKRAN_VOLTAGE_SELSYN:
      case EKRAN_ANGLE_SELSYN:
      case EKRAN_MEASURMENT_FREQUENCY:
      case EKRAN_CHOOSE_SETTINGS_RPN:
      case EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_RPN:
      case EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP2_RPN:
      case EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP3_RPN:
      case EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP4_RPN:
      case EKRAN_CHOOSE_SETTINGS_ZSKH:
      case EKRAN_CHOOSE_SETTINGS_BRP:
      case EKRAN_CHOOSE_SETPOINT_GROUP1_BRP:
      case EKRAN_CHOOSE_SETPOINT_GROUP2_BRP:
      case EKRAN_CHOOSE_SETPOINT_GROUP3_BRP:
      case EKRAN_CHOOSE_SETPOINT_GROUP4_BRP:
      case EKRAN_CHOOSE_SETTINGS_ZNKh:
      case EKRAN_CHOOSE_TIMEOUT_GROUP1_ZNKh:
      case EKRAN_CHOOSE_TIMEOUT_GROUP2_ZNKh:
      case EKRAN_CHOOSE_TIMEOUT_GROUP3_ZNKh:
      case EKRAN_CHOOSE_TIMEOUT_GROUP4_ZNKh:
      case EKRAN_CHOOSE_SETTINGS_UMIN:
      case EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_UMIN:
      case EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP2_UMIN:
      case EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP3_UMIN:
      case EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP4_UMIN:
      case EKRAN_CHOOSE_SETTINGS_UMAX:
      case EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_UMAX:
      case EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP2_UMAX:
      case EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP3_UMAX:
      case EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP4_UMAX:
      case EKRAN_CHOOSE_SETTINGS_UP:
      case EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_UP:
      case EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP2_UP:
      case EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP3_UP:
      case EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP4_UP:
      case EKRAN_CHOSE_SETTINGS:
      case EKRAN_LEVEL_CHOOSE_PASSWORDS:
      case EKRAN_LIST_INPUTS_FOR_RANGUVANNJA:
      case EKRAN_LIST_OUTPUTS_FOR_RANGUVANNJA:
      case EKRAN_LIST_LEDS_FOR_RANGUVANNJA:
      case EKRAN_CHOOSE_SETTINGS_SWITCHER:
      case EKRAN_CHOOSE_RANG_SWITCH:
      case EKRAN_CHOOSE_SETTINGS_UVV:
      case EKRAN_CHOSE_COMMUNICATION_PARAMETERS:
      case EKRAN_VIEW_NAME_OF_CELL:
      case EKRAN_CHOSE_SETTING_RS485:
      case EKRAN_PHY_LAYER_RS485:
      case EKRAN_PROTOCOL_RS485:
      case EKRAN_CHOSE_DATA_TIME:

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
      case EKRAN_LIST_TYPE_IEC61850_NODES:
      case EKRAN_LIST_IN_GOOSE:
      case EKRAN_LIST_IN_MMS:
      case EKRAN_LIST_OUT_LAN:
      case EKRAN_IN_GOOSE1:
      case EKRAN_IN_GOOSE2:
      case EKRAN_IN_GOOSE3:
      case EKRAN_IN_GOOSE4:
      case EKRAN_IN_GOOSE5:
      case EKRAN_IN_GOOSE6:
      case EKRAN_IN_GOOSE7:
      case EKRAN_IN_GOOSE8:
      case EKRAN_IN_GOOSE9:
      case EKRAN_IN_GOOSE10:
      case EKRAN_IN_GOOSE11:
      case EKRAN_IN_GOOSE12:
      case EKRAN_IN_GOOSE13:
      case EKRAN_IN_GOOSE14:
      case EKRAN_IN_GOOSE15:
      case EKRAN_IN_GOOSE16:
      case EKRAN_IN_MMS1:
      case EKRAN_IN_MMS2:
      case EKRAN_IN_MMS3:
      case EKRAN_IN_MMS4:
      case EKRAN_OUT_LAN1:
      case EKRAN_OUT_LAN2:
      case EKRAN_OUT_LAN3:
      case EKRAN_OUT_LAN4:
      case EKRAN_CHOSE_SETTING_ETHERNET:
#endif

      case EKRAN_VIEW_LIST_OF_REGISTRATORS:
      case EKRAN_VIEW_SETTINGS_OF_DIGITAL_REGISTRATORS:
      case EKRAN_VIEW_SETTINGS_OF_ANALOG_REGISTRATORS:
      case EKRAN_EXTENDED_LIGIC:
      case EKRAN_LIST_SETTINGS_FOR_DF:
      case EKRAN_LIST_DF_FOR_RANGUVANNJA:
      case EKRAN_LIST_DF_FOR_TIMEOUT_SETTINGS:
      case (EKRAN_LIST_TYPE_SOURCE_DF1 + 0):
      case (EKRAN_LIST_TYPE_SOURCE_DF1 + 1):
      case (EKRAN_LIST_TYPE_SOURCE_DF1 + 2):
      case (EKRAN_LIST_TYPE_SOURCE_DF1 + 3):
      case (EKRAN_LIST_TYPE_SOURCE_DF1 + 4):
      case (EKRAN_LIST_TYPE_SOURCE_DF1 + 5):
      case (EKRAN_LIST_TYPE_SOURCE_DF1 + 6):
      case (EKRAN_LIST_TYPE_SOURCE_DF1 + 7):
      case EKRAN_LIST_DT:
      case (EKRAN_SET_RESET_DT1 + 0):
      case (EKRAN_SET_RESET_DT1 + 1):
      case (EKRAN_SET_RESET_DT1 + 2):
      case (EKRAN_SET_RESET_DT1 + 3):
      case (EKRAN_SET_DT1 + 2 * 0):
      case (EKRAN_RESET_DT1 + 2 * 0):
      case (EKRAN_SET_DT1 + 2 * 1):
      case (EKRAN_RESET_DT1 + 2 * 1):
      case (EKRAN_SET_DT1 + 2 * 2):
      case (EKRAN_RESET_DT1 + 2 * 2):
      case (EKRAN_SET_DT1 + 2 * 3):
      case (EKRAN_RESET_DT1 + 2 * 3):
      case EKRAN_LIST_D_AND:
      case EKRAN_LIST_D_OR:
      case EKRAN_LIST_D_XOR:
      case EKRAN_LIST_D_NOT:
      case EKRAN_LIST_TF_FOR_RANGUVANNJA:
      case EKRAN_LIST_BUTTONS_FOR_RANGUVANNJA:
      case EKRAN_INFO:
      case EKRAN_DATE_TIME_PZ:
      case EKRAN_POINT_TIME_SETTINGS:
      case EKRAN_POINT_TIME_SETPOINT:
      case EKRAN_POINT_TIME_RANGUVANNJA:
      case EKRAN_DIAGNOSTYKA:
      case EKRAN_LIST_INPUTS_OUTPUTS:
      case EKRAN_STATE_INPUTS:
      case EKRAN_STATE_OUTPUTS:
      case EKRAN_LIST_REGISTRATORS:
      case EKRAN_LIST_ANALOG_REGISTRATOR_RECORDS:
      case EKRAN_LIST_DIGITAL_REGISTRATOR_RECORDS:
      case EKRAN_LIST_REGISTRATOR_PROGRAM_ERROR_RECORDS:
      case EKRAN_LIST_STATE_CMD_REGISTRATOR_RECORDS:
      case EKRAN_TITLES_DIGITAL_REGISTRATOR:
      case EKRAN_CHANGES_SIGNALS_DR:
      case EKRAN_TITLE_MAX_VALUES:
      case EKRAN_MAX_VALUES:
      case EKRAN_CHANGES_DIAGNOSTICS_PR_ERR:
      case EKRAN_TITLES_STATE_CMD_REGISTRATOR:
      case EKRAN_DATA_LABEL_STATE_CMD:
      case EKRAN_STATE_CMD_REG:
        {
          //Очищаємо всі біти краім упралінських
          new_state_keyboard &= (1u << BIT_KEY_ENTER) |
                                (1u << BIT_KEY_ESC) |
                                (1u << BIT_KEY_UP) |
                                (1u << BIT_KEY_DOWN) |
                                (1u << BIT_REWRITE);
          //Дальше виконуємо дії, якщо натиснута кнопка на яку треба реагівати, або стоїть команда обновити екран
          if (new_state_keyboard != 0)
          {
            //Пріоритет стоїть на обновлені екрану
            if ((new_state_keyboard & (1u << BIT_REWRITE)) != 0)
            {
              if (current_ekran.current_level == EKRAN_MEASURMENT)
              {
                do
                {
                  if (current_ekran.index_position >= MAX_ROW_FOR_MEASURMENT)
                    current_ekran.index_position = 0;

                  if (
                    (current_ekran.index_position == INDEX_ML_MEASURMENT_2) &&
                    ((current_settings.control_rpn & MASKA_FOR_BIT(INDEX_ML_CTRRPN_TRANSF)) == 0))
                    current_ekran.index_position++;

                  if (
                    (current_ekran.index_position == INDEX_ML_MEASURMENT_LOGOMETR) &&
                    ((current_settings.type_control_location != 1)))
                    current_ekran.index_position++;

                  if (
                    (current_ekran.index_position == INDEX_ML_MEASURMENT_SELSYN) &&
                    ((current_settings.type_control_location != 2)))
                    current_ekran.index_position++;
                } while (current_ekran.index_position >= MAX_ROW_FOR_MEASURMENT);

                position_in_current_level_menu[EKRAN_MEASURMENT] = current_ekran.index_position;
                //Формуємо екран списку вимірювання
                make_ekran_measurement();
              }
              else if (current_ekran.current_level == EKRAN_MEASURMENT_SELSYN)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_MEASURMENT_FOR_SELSYN)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_MEASURMENT_SELSYN] = current_ekran.index_position;
                //Формуємо екран списку вимірювання для сельсина
                make_ekran_measuremet_for_selsyn();
              }
              else if (
                (current_ekran.current_level == EKRAN_MEASURMENT_1) ||
                (current_ekran.current_level == EKRAN_MEASURMENT_2))
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_MEASURMENT_1_AND_2)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;
                //Формуємо екран вимірювання струмів і напруг ТН1 або ТН2
                make_ekran_current_voltage((current_ekran.current_level == EKRAN_MEASURMENT_1) ? 0 : 1, pervynna_vtorynna);
              }
              else if (current_ekran.current_level == EKRAN_VOLTAGE_SELSYN)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_MEASURMENT_VOLTAGE_SELSYN)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_VOLTAGE_SELSYN] = current_ekran.index_position;
                //Формуємо екран вимірювання напруг для сельсина
                make_ekran_voltage_for_selsyn();
              }
              else if (current_ekran.current_level == EKRAN_ANGLE_SELSYN)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_MEASURMENT_ANGLE_SELSYN)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_ANGLE_SELSYN] = current_ekran.index_position;
                //Формуємо екран вимірювання кутів для сельсина
                make_ekran_angle_for_selsyn();
              }
              else if (current_ekran.current_level == EKRAN_LOGOMETR)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_VOLTAGE_LOGOMETR)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_LOGOMETR] = current_ekran.index_position;
                //Формуємо екран вимірювання напруг логометра
                make_ekran_logomentr_voltage();
              }
              else if (current_ekran.current_level == EKRAN_MEASURMENT_FREQUENCY)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_MEASURMENT_FREQUENCY)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_MEASURMENT_FREQUENCY] = current_ekran.index_position;
                //Формуємо екран відображення частоти
                make_ekran_frequency();
              }
              else if (
                (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_RPN) ||
                (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_BRP) ||
                (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_ZNKh) ||
                (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_UMIN) ||
                (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_UMAX) ||
                (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_UP))
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_CHOSE_SETTINGS_PROTECTION_WITH_TWO_GROUP)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;

                //Формуємо екран група1-...-групаN-настройки для всіх захистів де використовуються групи уставок
                make_ekran_chose_settings_protection_with_groups();
              }
              else if (
                (
                  (current_ekran.current_level >= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_RPN) &&
                  (current_ekran.current_level <= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP4_RPN)) ||
                ((current_ekran.current_level >= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_UMIN) &&
                 (current_ekran.current_level <= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP4_UMIN)) ||
                ((current_ekran.current_level >= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_UMAX) &&
                 (current_ekran.current_level <= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP4_UMAX)) ||
                ((current_ekran.current_level >= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_UP) &&
                 (current_ekran.current_level <= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP4_UP)))
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_CHOSE_SETPOINTS_TIMEOUTS)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;

                //Формуємо екран уставки-витримкидля захистів у яких є дві групи уставок
                make_ekran_chose_setpoint_and_timeout();
              }
              else if ((current_ekran.current_level >= EKRAN_CHOOSE_SETPOINT_GROUP1_BRP) &&
                       (current_ekran.current_level <= EKRAN_CHOOSE_SETPOINT_GROUP4_BRP))
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_CHOSE_SETTINGS_PROTECTION_WITH_SETPOINTS)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;

                //Формуємо екран управління для захистів у яких є багато груп уставок
                make_ekran_chose_setpoint();
              }
              else if (
                (current_ekran.current_level >= EKRAN_CHOOSE_TIMEOUT_GROUP1_ZNKh) &&
                (current_ekran.current_level <= EKRAN_CHOOSE_TIMEOUT_GROUP4_ZNKh))
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_CHOSE_SETTINGS_PROTECTION_WITH_TIMEOUT)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;

                //Формуємо екран витримок для захистів у яких є багато груп уставок
                make_ekran_chose_timeout();
              }
              else if (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_ZSKH)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_CHOSE_SETTINGS_PROTECTION_WITH_CONTROL)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;

                //Формуємо екран відображення заголовків настроювання де тільки є управління
                make_ekran_chose_control();
              }
              else if (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_SWITCHER)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_CHOSE_SETTINGS_SWITCHER)
                  current_ekran.index_position = 0;

                position_in_current_level_menu[EKRAN_CHOOSE_SETTINGS_SWITCHER] = current_ekran.index_position;

                //Формуємо екран уставки-витримки-настройки-ранжування вимикача
                make_ekran_chose_settings_switcher();
              }
              else if (current_ekran.current_level == EKRAN_CHOOSE_RANG_SWITCH)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_RANG_SWITCH)
                  current_ekran.index_position = 0;

                position_in_current_level_menu[EKRAN_CHOOSE_RANG_SWITCH] = current_ekran.index_position;

                //Формуємо екран БВимк./БУвімк.
                make_ekran_choose_CBOn_CBOff();
              }
              else if (current_ekran.current_level == EKRAN_CHOSE_SETTINGS)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_CHOSE_SETTINGS)
                  current_ekran.index_position = 0;
                while (
                  ((current_settings.configuration & (1 << EL_BIT_CONFIGURATION)) == 0) &&
                  (current_ekran.index_position == INDEX_OF_EXTENDED_LOGIC))
                {
                  if (++current_ekran.index_position >= MAX_ROW_FOR_CHOSE_SETTINGS)
                    current_ekran.index_position = 0;
                }

                position_in_current_level_menu[EKRAN_CHOSE_SETTINGS] = current_ekran.index_position;

                //Формуємо екран заголовків настроювання
                make_ekran_chose_settings();
              }
              else if (current_ekran.current_level == EKRAN_LEVEL_CHOOSE_PASSWORDS)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_CHOOSE_PASSWORDS)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_LEVEL_CHOOSE_PASSWORDS] = current_ekran.index_position;

                //Формуємо екран заголовків паролів
                make_ekran_chose_passwords();
              }
              else if (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_UVV)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_CHOSE_SETTINGS_UVV)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_CHOOSE_SETTINGS_UVV] = current_ekran.index_position;
                //Формуємо екран вибору настройок УВВ
                make_ekran_chose_settings_uvv();
              }
              else if (current_ekran.current_level == EKRAN_LIST_INPUTS_FOR_RANGUVANNJA)
              {
                if (current_ekran.index_position >= MAX_ROW_LIST_INPUTS_FOR_RANGUVANNJA)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_LIST_INPUTS_FOR_RANGUVANNJA] = current_ekran.index_position;

                //Формуємо екран заголовків дискретних входів для ранжування
                make_ekran_chose_of_list_for_ranguvannja(ID_INPUT);
              }
              else if (current_ekran.current_level == EKRAN_LIST_OUTPUTS_FOR_RANGUVANNJA)
              {
                if (current_ekran.index_position >= MAX_ROW_LIST_OUTPUTS_FOR_RANGUVANNJA)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_LIST_OUTPUTS_FOR_RANGUVANNJA] = current_ekran.index_position;

                //Формуємо екран заголовків дискретних виходів для ранжування
                make_ekran_chose_of_list_for_ranguvannja(ID_OUTPUT);
              }
              else if (current_ekran.current_level == EKRAN_LIST_LEDS_FOR_RANGUVANNJA)
              {
                if (current_ekran.index_position >= MAX_ROW_LIST_LEDS_FOR_RANGUVANNJA)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_LIST_LEDS_FOR_RANGUVANNJA] = current_ekran.index_position;

                //Формуємо екран заголовків світоіндикаторів для ранжування
                make_ekran_chose_of_list_for_ranguvannja(ID_LED);
              }
              else if (current_ekran.current_level == EKRAN_CHOSE_COMMUNICATION_PARAMETERS)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_CHOSE_COMMUNICATION_PARAMETERS)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_CHOSE_COMMUNICATION_PARAMETERS] = current_ekran.index_position;
                //Формуємо екран вибору настройок парамеирів комунікації
                make_ekran_chose_communication_parameters();
              }
              else if (current_ekran.current_level == EKRAN_VIEW_NAME_OF_CELL)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_VIEW_NAME_OF_CELL)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_VIEW_NAME_OF_CELL] = current_ekran.index_position;
                //Формуємо екран відображення імені ячейки
                make_ekran_name_of_cell();
              }
              else if (current_ekran.current_level == EKRAN_CHOSE_SETTING_RS485)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_CHOSE_SETTING_RS485)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_CHOSE_SETTING_RS485] = current_ekran.index_position;
                //Формуємо екран відображення списку настройок для інтерфейсу RS-485
                make_ekran_chose_setting_rs485();
              }
              else if (current_ekran.current_level == EKRAN_PHY_LAYER_RS485)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_PHY_LAYER_RS485)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_PHY_LAYER_RS485] = current_ekran.index_position;
                //Формуємо екран відображення списку настройок фізичного рівня для інтерфейсу RS-485
                make_ekran_phy_layer_rs485();
              }
              else if (current_ekran.current_level == EKRAN_PROTOCOL_RS485)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_PROTOCOLS_RS485)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_PROTOCOL_RS485] = current_ekran.index_position;
                //Формуємо екран відображення списку налаштувань протоколу RS-485
                make_ekran_protocols_rs485();
              }
              else if (current_ekran.current_level == EKRAN_CHOSE_DATA_TIME)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_CHOSE_DATA_TIME_SETTINGS)
                  current_ekran.index_position = 0;
                if ((current_settings.dst & MASKA_FOR_BIT(N_BIT_TZ_DST)) == 0)
                {
                  while (
                    (current_ekran.index_position == INDEX_ML_CHDT_DST_ON) ||
                    (current_ekran.index_position == INDEX_ML_CHDT_DST_OFF))
                  {
                    if (++current_ekran.index_position >= MAX_ROW_FOR_CHOSE_DATA_TIME_SETTINGS)
                      current_ekran.index_position = 0;
                  }
                }

                position_in_current_level_menu[EKRAN_CHOSE_DATA_TIME] = current_ekran.index_position;
                //Формуємо екран вибору налаштувань дати і часу
                make_ekran_chose_data_time_settings();
              }
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
              else if (current_ekran.current_level == EKRAN_LIST_TYPE_IEC61850_NODES)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_TYPE_IEC61850_NODES)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_LIST_TYPE_IEC61850_NODES] = current_ekran.index_position;

                //Формуємо екран заголовків для типів логічних вузлів для IEC 61850
                make_ekran_type_IEC61850_nodes();
              }
              else if (current_ekran.current_level == EKRAN_LIST_IN_GOOSE)
              {
                if (current_ekran.index_position >= N_IN_GOOSE)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_LIST_IN_GOOSE] = current_ekran.index_position;

                //Формуємо екран заголовків вхідних GOOSE блоків
                make_ekran_chose_of_list_for_ranguvannja(ID_IN_GOOSE);
              }
              else if (current_ekran.current_level == EKRAN_LIST_IN_MMS)
              {
                if (current_ekran.index_position >= N_IN_MMS)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_LIST_IN_MMS] = current_ekran.index_position;

                //Формуємо екран заголовків вхідних MMS блоків
                make_ekran_chose_of_list_for_ranguvannja(ID_IN_MMS);
              }
              else if (current_ekran.current_level == EKRAN_LIST_OUT_LAN)
              {
                if (current_ekran.index_position >= N_OUT_LAN)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_LIST_OUT_LAN] = current_ekran.index_position;

                //Формуємо екран заголовків вихідний мережевий блок
                make_ekran_chose_of_list_for_ranguvannja(ID_OUT_LAN);
              }
              else if (
                ((current_ekran.current_level >= EKRAN_IN_GOOSE1) && (current_ekran.current_level <= EKRAN_IN_GOOSE16)) ||
                ((current_ekran.current_level >= EKRAN_IN_MMS1) && (current_ekran.current_level <= EKRAN_IN_MMS4)))
              {
                if (current_ekran.index_position >= N_IN_GOOSE_MMS_OUT)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;
                //Формуємо екран заголовків виходів
                make_ekran_list_in_out_for_iec61850(0, N_IN_GOOSE_MMS_OUT);
              }
              else if ((current_ekran.current_level >= EKRAN_OUT_LAN1) && (current_ekran.current_level <= EKRAN_OUT_LAN4))
              {
                if (current_ekran.index_position >= N_OUT_LAN_IN)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;
                //Формуємо екран заголовків виходів
                make_ekran_list_in_out_for_iec61850(1, N_OUT_LAN_IN);
              }
              else if (current_ekran.current_level == EKRAN_CHOSE_SETTING_ETHERNET)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_CHOSE_SETTING_ETHERNET)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_CHOSE_SETTING_ETHERNET] = current_ekran.index_position;
                //Формуємо екран відображення списку настройок для інтерфейсу Ethernet
                make_ekran_chose_setting_Ethernet();
              }
#endif
              else if (current_ekran.current_level == EKRAN_VIEW_LIST_OF_REGISTRATORS)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_LIST_OF_REGISTRATORS)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_VIEW_LIST_OF_REGISTRATORS] = current_ekran.index_position;
                //Формуємо екран вибору реєстратора
                make_ekran_chose_registrators();
              }
              else if (current_ekran.current_level == EKRAN_VIEW_SETTINGS_OF_ANALOG_REGISTRATORS)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_SETTINGS_OF_ANALOG_REGISTRATOR)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_VIEW_SETTINGS_OF_ANALOG_REGISTRATORS] = current_ekran.index_position;
                //Формуємо екран вибору ностройок аналогового реєстратора
                make_ekran_settings_analog_registrators();
              }
              else if (current_ekran.current_level == EKRAN_VIEW_SETTINGS_OF_DIGITAL_REGISTRATORS)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_SETTINGS_OF_DIGITAL_REGISTRATOR)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_VIEW_SETTINGS_OF_DIGITAL_REGISTRATORS] = current_ekran.index_position;
                //Формуємо екран вибору ностройок аналогового реєстратора
                make_ekran_settings_digital_registrators();
              }
              else if (current_ekran.current_level == EKRAN_EXTENDED_LIGIC)
              {
                do
                {
                  if (current_ekran.index_position >= MAX_ROW_FOR_EXTENDED_LIGIC)
                    current_ekran.index_position = 0;

                  //                if ((current_ekran.index_position == INDEX_OF_DEFINED_FUNCTIONS) && (current_settings.number_defined_df  == 0))
                  //                  current_ekran.index_position++;
                  //                if ((current_ekran.index_position == INDEX_OF_DEFINED_TRIGGERS) && (current_settings.number_defined_dt  == 0))
                  //                  current_ekran.index_position++;
                  //                if ((current_ekran.index_position == INDEX_OF_DEFINED_AND) && (current_settings.number_defined_and  == 0))
                  //                  current_ekran.index_position++;
                  //                if ((current_ekran.index_position == INDEX_OF_DEFINED_OR) && (current_settings.number_defined_or  == 0))
                  //                  current_ekran.index_position++;
                  //                if ((current_ekran.index_position == INDEX_OF_DEFINED_XOR) && (current_settings.number_defined_xor  == 0))
                  //                  current_ekran.index_position++;
                  //                if ((current_ekran.index_position == INDEX_OF_DEFINED_NOT) && (current_settings.number_defined_not  == 0))
                  //                  current_ekran.index_position++;
                } while (current_ekran.index_position >= MAX_ROW_FOR_EXTENDED_LIGIC);

                position_in_current_level_menu[EKRAN_EXTENDED_LIGIC] = current_ekran.index_position;
                //Формуємо екран вибору налаштувань розширеної логіки
                make_ekran_extended_logic();
              }
              else if (current_ekran.current_level == EKRAN_LIST_SETTINGS_FOR_DF)
              {
                if (current_ekran.index_position >= MAX_ROW_SETTINGS_DF)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_LIST_SETTINGS_FOR_DF] = current_ekran.index_position;
                //Формуємо екран відображення списку настройок для опреділювальних функцій
                make_ekran_chose_settings_df();
              }
              else if ((current_ekran.current_level == EKRAN_LIST_DF_FOR_RANGUVANNJA) || (current_ekran.current_level == EKRAN_LIST_DF_FOR_TIMEOUT_SETTINGS))
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_LIST_DF)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;

                //Формуємо екран заголовків опреділювальних функцій
                make_ekran_chose_of_list_for_ranguvannja(ID_DF);
              }
              else if ((current_ekran.current_level >= EKRAN_LIST_TYPE_SOURCE_DF1) && (current_ekran.current_level <= (EKRAN_LIST_TYPE_SOURCE_DF1 + NUMBER_DEFINED_FUNCTIONS - 1)))
              {
                if (current_ekran.index_position >= MAX_ROW_LIST_TYPE_SOURCE_DF)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;

                //Формуємо екран заголовків типів джерел опреділювальних функцій
                make_ekran_list_type_source_df();
              }
              else if (current_ekran.current_level == EKRAN_LIST_DT)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_LIST_DT)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_LIST_DT] = current_ekran.index_position;
                //Формуємо екран заголовків опреділювальних триґерів
                make_ekran_chose_of_list_for_ranguvannja(ID_DT);
              }
              else if ((current_ekran.current_level >= EKRAN_SET_RESET_DT1) && (current_ekran.current_level <= (EKRAN_SET_RESET_DT1 + NUMBER_DEFINED_TRIGGERS - 1)))
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_LIST_SET_RESET_DT)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;
                //Формуємо екран заголовків активація-деактивація опреділювальних триґерів
                make_ekran_set_reset_trigger();
              }
              else if ((current_ekran.current_level >= EKRAN_SET_DT1) && (current_ekran.current_level <= (EKRAN_SET_DT1 + 2 * NUMBER_DEFINED_TRIGGERS - 1)))
              {
                if (current_ekran.index_position >= MAX_ROW_LIST_TYPE_SOURCE_DT)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;
                //Формуємо екран заголовків типів джерел опреділювальних триґерів
                make_ekran_list_type_source_dt();
              }
              else if (current_ekran.current_level == EKRAN_LIST_D_AND)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_LIST_D_AND)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_LIST_D_AND] = current_ekran.index_position;
                //Формуємо екран заголовків визначуваних "І"
                make_ekran_chose_of_list_for_ranguvannja(ID_AND);
              }
              else if (current_ekran.current_level == EKRAN_LIST_D_OR)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_LIST_D_OR)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_LIST_D_OR] = current_ekran.index_position;
                //Формуємо екран заголовків визначуваних "АБО"
                make_ekran_chose_of_list_for_ranguvannja(ID_OR);
              }
              else if (current_ekran.current_level == EKRAN_LIST_D_XOR)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_LIST_D_XOR)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_LIST_D_XOR] = current_ekran.index_position;
                //Формуємо екран заголовків визначуваних "Викл.АБО"
                make_ekran_chose_of_list_for_ranguvannja(ID_XOR);
              }
              else if (current_ekran.current_level == EKRAN_LIST_D_NOT)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_LIST_D_NOT)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_LIST_D_NOT] = current_ekran.index_position;
                //Формуємо екран заголовків визначуваних "НЕ"
                make_ekran_chose_of_list_for_ranguvannja(ID_NOT);
              }
              else if (current_ekran.current_level == EKRAN_LIST_TF_FOR_RANGUVANNJA)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_LIST_TF)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;

                //Формуємо екран заголовків передавальних функцій
                make_ekran_chose_of_list_for_ranguvannja(ID_TF);
              }
              else if (current_ekran.current_level == EKRAN_LIST_BUTTONS_FOR_RANGUVANNJA)
              {
                if (current_ekran.index_position >= MAX_ROW_LIST_BUTTONS_FOR_RANGUVANNJA)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_LIST_BUTTONS_FOR_RANGUVANNJA] = current_ekran.index_position;

                //Формуємо екран заголовків функціональних кнопок для ранжування
                make_ekran_chose_of_list_for_ranguvannja(ID_DB);
              }
              else if (current_ekran.current_level == EKRAN_INFO)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_INFO)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_INFO] = current_ekran.index_position;
                //Формуємо екран інформації
                make_ekran_info();
              }
              else if (current_ekran.current_level == EKRAN_DATE_TIME_PZ)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_DATE_TIME_PZ)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_DATE_TIME_PZ] = current_ekran.index_position;
                //Формуємо екран відображення міти дати і часу прошивки
                make_ekran_date_time_pz();
              }
              else if (current_ekran.current_level == EKRAN_POINT_TIME_SETTINGS)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_POINT_TIME_SETTINGS)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_POINT_TIME_SETTINGS] = current_ekran.index_position;
                //Формуємо екран вибору міток настроювання
                make_ekran_chose_item_of_point_time_settings();
              }
              else if (current_ekran.current_level == EKRAN_POINT_TIME_SETPOINT)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_POINT_TIME_SETPOINT_RANGUVANNJA)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_POINT_TIME_SETPOINT] = current_ekran.index_position;
                //Формуємо екран відображення міти останніх змін у уставках-витримках-управлінчькій інформації
                make_ekran_time_settings(0);
              }
              else if (current_ekran.current_level == EKRAN_POINT_TIME_RANGUVANNJA)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_POINT_TIME_SETPOINT_RANGUVANNJA)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_POINT_TIME_RANGUVANNJA] = current_ekran.index_position;
                //Формуємо екран відображення міти останніх змін у ранжування
                make_ekran_time_settings(1);
              }
              else if (current_ekran.current_level == EKRAN_DIAGNOSTYKA)
              {
                if (current_ekran.index_position >= ((int) MAX_ROW_FOR_DIAGNOSTYKA))
                  current_ekran.index_position = 0;

                unsigned int diagnostyka_tmp[N_DIAGN];
                for (size_t i = 0; i < N_DIAGN; i++)
                  diagnostyka_tmp[i] = diagnostyka[i];

                unsigned int not_null = false;
                for (size_t i = 0; i < N_DIAGN; i++)
                {
                  not_null |= (diagnostyka_tmp[i] != 0);
                  if (not_null)
                    break;
                }

                if (not_null)
                {
                  while (_CHECK_SET_BIT(diagnostyka_tmp, current_ekran.index_position) == 0)
                  {
                    current_ekran.index_position++;
                    if (current_ekran.index_position >= ((int) MAX_ROW_FOR_DIAGNOSTYKA))
                      current_ekran.index_position = 0;
                  }
                }
                else
                {
                  current_ekran.index_position = 0;
                }

                position_in_current_level_menu[EKRAN_DIAGNOSTYKA] = current_ekran.index_position;
                //Формуємо екран діагностики
                make_ekran_diagnostyka(diagnostyka_tmp);
              }
              else if (current_ekran.current_level == EKRAN_LIST_INPUTS_OUTPUTS)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_LIST_INPUTS_OUTPUTS)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_LIST_INPUTS_OUTPUTS] = current_ekran.index_position;
                //Формуємо екран вибору станів входів-виходів
                make_ekran_list_inputs_outputs();
              }
              else if ((current_ekran.current_level == EKRAN_STATE_INPUTS) || (current_ekran.current_level == EKRAN_STATE_OUTPUTS))
              {
                if (current_ekran.current_level == EKRAN_STATE_INPUTS)
                {
                  if (current_ekran.index_position >= NUMBER_INPUTS)
                    current_ekran.index_position = 0;
                }
                else
                {
                  if (current_ekran.index_position >= (NUMBER_SIMPLE_OUTPUTS
#ifdef NUMBER_DS
                                                       + ((current_settings.configuration & (1 << DS_BIT_CONFIGURATION)) != 0) * NUMBER_DS
#endif
                                                       ))
                    current_ekran.index_position = 0;
                }

                position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;
                //Формуємо екран станів входів або виходыв
                make_ekran_state_inputs_or_outputs(current_ekran.current_level - EKRAN_STATE_INPUTS);
              }
              else if (current_ekran.current_level == EKRAN_LIST_REGISTRATORS)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_LIST_REGISTRATORS)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_LIST_REGISTRATORS] = current_ekran.index_position;
                //Формуємо екран вибору відображення реєстраторів
                make_ekran_list_registrators();
              }
              else if (
                (current_ekran.current_level == EKRAN_LIST_ANALOG_REGISTRATOR_RECORDS) ||
                (current_ekran.current_level == EKRAN_LIST_DIGITAL_REGISTRATOR_RECORDS) ||
                (current_ekran.current_level == EKRAN_LIST_REGISTRATOR_PROGRAM_ERROR_RECORDS)
                //?||(current_ekran.current_level == EKRAN_LIST_STATE_CMD_REGISTRATOR_RECORDS)
              )
              {
                unsigned int number_records = 0;
                unsigned int type_registrator = 0;

                if (current_ekran.current_level == EKRAN_LIST_ANALOG_REGISTRATOR_RECORDS)
                {
                  unsigned int first_number = (info_rejestrator_ar.first_number < 0) ? 0 : (info_rejestrator_ar.first_number + 1);
                  unsigned int last_number = (info_rejestrator_ar.last_number < 0) ? 0 : (info_rejestrator_ar.last_number + 1);

                  if (first_number == 0)
                    number_records = 0;
                  else if (first_number >= last_number)
                    number_records = first_number - last_number + 1;
                  else
                    number_records = NUMBER_FATFS_NAME - last_number + first_number + 1;

                  type_registrator = INDEX_ML_ANALOG_REGISTRATOR_INFO;
                }
                else if (current_ekran.current_level == EKRAN_LIST_DIGITAL_REGISTRATOR_RECORDS)
                {
                  number_records = info_rejestrator_dr.number_records;
                  type_registrator = INDEX_ML_DIGITAL_REGISTRATOR_INFO;
                }
                else if (current_ekran.current_level == EKRAN_LIST_REGISTRATOR_PROGRAM_ERROR_RECORDS)
                {
                  number_records = info_rejestrator_pr_err.number_records;
                  type_registrator = INDEX_ML_PROGRAM_ERROE_REGISTRATOR_INFO;
                }
                //?else
                //?{
                //?  number_records = holderCmdPlusTime.shTotalFixElem;//info_rejestrator_pr_err.number_records;
                //?  type_registrator = INDEX_ML_STATE_CMD_REGISTRATOR_INFO;
                //?}
                else
                {
                  total_error_sw_fixed();
                }

                if (current_ekran.index_position >= ((int) number_records))
                  current_ekran.index_position = 0;
                position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;

                //Формуємо екран відображення записів
                //make_ekran_data_and_time_elem_d_p_a_registrator(type_registrator);
                if (type_registrator == INDEX_ML_ANALOG_REGISTRATOR_INFO)
                  _SET_STATE(FATFS_command, FATFS_READ_DATA_FOR_MENU);
                else
                  make_ekran_list_records_registrator(type_registrator);
              }
              else if (current_ekran.current_level == EKRAN_LIST_STATE_CMD_REGISTRATOR_RECORDS)
              {
                if (current_ekran.index_position >= holderCmdPlusTime.shTotalFixElem)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_LIST_STATE_CMD_REGISTRATOR_RECORDS] = current_ekran.index_position;
                //?//Формуємо екран відображення дат і часу
                //?make_ekran_data_and_time_elem_stt_registrator(0);
                //Формуємо екран  відображення дати
                make_ekran_data_elem_stt_registrator();
              }
              else if (current_ekran.current_level == EKRAN_TITLES_DIGITAL_REGISTRATOR)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_TITLES_DIGITAL_REGISTRATOR)
                  current_ekran.index_position = 0;
                while (
                  ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_DR_MENU) == 0) &&
                  (current_ekran.index_position > INDEX_ML_TITLE_DR_CHANGES_SIGNALS) &&
                  (((current_ekran.index_position == INDEX_ML_TITLE_DR_MIN_U) && (buffer_for_manu_read_record[FIRST_INDEX_NUMBER_MIN_U_DR] == 0)) ||
                   ((current_ekran.index_position == INDEX_ML_TITLE_DR_MAX_U) && (buffer_for_manu_read_record[FIRST_INDEX_NUMBER_MAX_U_DR] == 0))))
                {
                  if (++current_ekran.index_position >= MAX_ROW_FOR_TITLES_DIGITAL_REGISTRATOR)
                    current_ekran.index_position = 0;
                }
                position_in_current_level_menu[EKRAN_TITLES_DIGITAL_REGISTRATOR] = current_ekran.index_position;
                //Формуємо екран відображення заголовків груп для дискретного реєстратора
                make_ekran_list_titles_for_record_of_digital_registrator();
              }
              else if (current_ekran.current_level == EKRAN_CHANGES_SIGNALS_DR)
              {
                //Формуємо екран відображення змін сигналів - записаних у дискретному реєстраторі
                make_ekran_changing_signals_digital_registrator();
              }
              else if (current_ekran.current_level == EKRAN_TITLE_MAX_VALUES)
              {
                unsigned int number_records = buffer_for_manu_read_record[FIRST_INDEX_NUMBER_MIN_U_DR + type_view_max_values_dr - IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE];

                if (current_ekran.index_position >= ((int) number_records))
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_TITLE_MAX_VALUES] = current_ekran.index_position;

                //Формуємо екран відображення міток часу записів міксації максимальних струмів
                make_ekran_title_analog_value_records_digital_registrator();
              }
              else if (current_ekran.current_level == EKRAN_MAX_VALUES)
              {
                unsigned char *point_unsigned_char = (unsigned char *) (buffer_for_manu_read_record + index_cell_into_array_for_integral_values_dr);
                unsigned int *point_unsigned_int = (unsigned int *) point_unsigned_char;
                unsigned int control_for_dr = *(point_unsigned_int + 5);

                do
                {
                  if (current_ekran.index_position >= MAX_ROW_FOR_EKRAN_ANALOG_VALUES_DR)
                    current_ekran.index_position = 0;

                  while (
                    (buffer_for_manu_read_record[FIRST_INDEX_START_START_RECORD_DR] == LABEL_START_RECORD_DR) && /*якщо ця умова не виконується, то у функції make_ekran_analog_value_records_digital_registrator ми перейдемо на відображення інформації про недоступні дані, а не будемо виконувати фільтрацію*/
                    ((control_for_dr & (1 << 0)) == 0) &&                                                        /*моніторинг вівся, коли був вибраний 2-обмотковий трансформатор*/
                    (current_ekran.index_position >= INDEX_ML_DR_U2) &&
                    (current_ekran.index_position <= INDEX_ML_DR_I2))
                    current_ekran.index_position++;

                } while (current_ekran.index_position >= MAX_ROW_FOR_EKRAN_ANALOG_VALUES_DR);

                //Формуємо екран відображення аналогових значень з запису дискретного реєстратора
                make_ekran_analog_value_records_digital_registrator();
              }
              else if (current_ekran.current_level == EKRAN_TITLES_STATE_CMD_REGISTRATOR)
              {
                //!if(current_ekran.index_position >= MAX_ROW_FOR_TITLES_PR_ERR_REGISTRATOR) current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_TITLES_STATE_CMD_REGISTRATOR] = current_ekran.index_position;
                //?  //Формуємо екран відображення заголовків груп для реєстратора програмних подій
                //?  make_ekran_list_titles_for_record_of_state_cmd_registrator();//make_ekran_list_titles_for_record_of_pr_err_registrator();
                make_ekran_time_ms_c_val_c_name_sr_format_changing_signals();
              }
              else if (current_ekran.current_level == EKRAN_DATA_LABEL_STATE_CMD)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_EKRAN_DATA_LABEL)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_DATA_LABEL_STATE_CMD] = current_ekran.index_position;
                //?//Формуємо екран відображення мітки часу прочитаного запису реєстратора
                //?make_ekran_data_and_time_of_records_registrator(3);
                make_ekran_signals_stable_one_in_data_elem_stt_reg();
              }
              else if (current_ekran.current_level == EKRAN_CHANGES_DIAGNOSTICS_PR_ERR)
              {
                //Формуємо екран відображення змін діагностик - записаних у реєстраторі програмних подій
                make_ekran_changing_diagnostics_pr_err_registrator();
              }
              //?else if (current_ekran.current_level == EKRAN_STATE_CMD_REG)
              //?{
              //?  //Формуємо екран відображення змін діагностик - записаних у реєстраторі програмних подій
              //?  make_ekran_changing_signals_statistica_registrator();
              //?
              //?
              //?}

              //Очищаємо біт обновлення екрану
              new_state_keyboard &= ~(1u << BIT_REWRITE);
            }
            else
            {
              if (new_state_keyboard == (1u << BIT_KEY_ENTER))
              {
                //Натиснута кнопка ENTER
                int temp_current_level = current_ekran.current_level;

                if (current_ekran.current_level == EKRAN_MEASURMENT)
                {
                  //Ми у вікні відображення списку вимірювань
                  if (current_ekran.index_position == INDEX_ML_MEASURMENT_1)
                  {
                    //Переходимо на меню вибору відображення вимірювань ТН1
                    current_ekran.current_level = EKRAN_MEASURMENT_1;

                    //Спочатку відображаємо інформацію на вторинній обмотці трансворматора - значення , які поступають на аналогові входи приладу
                    pervynna_vtorynna = 0;
                  }
                  else if (current_ekran.index_position == INDEX_ML_MEASURMENT_2)
                  {
                    //Переходимо на меню вибору відображення вимірювань ТН2
                    current_ekran.current_level = EKRAN_MEASURMENT_2;

                    //Спочатку відображаємо інформацію на вторинній обмотці трансворматора - значення , які поступають на аналогові входи приладу
                    pervynna_vtorynna = 0;
                  }
                  else if (current_ekran.index_position == INDEX_ML_MEASURMENT_LOGOMETR)
                  {
                    //Переходимо на меню вибору відображення вимірювань напруг для логометра
                    current_ekran.current_level = EKRAN_LOGOMETR;
                  }
                  else if (current_ekran.index_position == INDEX_ML_MEASURMENT_SELSYN)
                  {
                    //Переходимо на меню вибору відображення списку вимірювань для сельсина
                    current_ekran.current_level = EKRAN_MEASURMENT_SELSYN;
                  }
                  else if (current_ekran.index_position == INDEX_ML_MEASURMENT_FREQUENCY)
                  {
                    //Переходимо на меню вибору відображення списку вимірювань для сельсина
                    current_ekran.current_level = EKRAN_MEASURMENT_FREQUENCY;
                  }

                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                  current_ekran.cursor_on = 1;
                  current_ekran.cursor_blinking_on = 0;
                }
                else if (
                  (current_ekran.current_level == EKRAN_MEASURMENT_1) ||
                  (current_ekran.current_level == EKRAN_MEASURMENT_2))
                {
                  //Ми у вікні відображення або струмів, або напруг

                  //Переключаємося між відображенням для первинної обмотки і вторинної обмотки
                  pervynna_vtorynna ^= 0x1;
                }
                else if (current_ekran.current_level == EKRAN_MEASURMENT_SELSYN)
                {
                  //Ми у вікні відображення списку вимірювань для сельсина
                  if (current_ekran.index_position == INDEX_ML_MEASURMENT_VOLTAGE)
                  {
                    //Переходимо на меню вибору відображення списку вимірювань напруг для сельсина
                    current_ekran.current_level = EKRAN_VOLTAGE_SELSYN;
                  }
                  else if (current_ekran.index_position == INDEX_ML_MEASURMENT_ANGLE)
                  {
                    //Переходимо на меню вибору відображення списку вимірювань кутів для сельсина
                    current_ekran.current_level = EKRAN_ANGLE_SELSYN;
                  }
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                  current_ekran.cursor_on = 1;
                  current_ekran.cursor_blinking_on = 0;
                }
                else if (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_RPN)
                {
                  //Натисну кнопка Enter у вікні вибору група1-...-групаN-настройки РПН
                  if (
                    (current_ekran.index_position >= INDEX_ML_GROUP1) &&
                    (current_ekran.index_position <= INDEX_ML_GROUP4))
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення атбраної групи уставок для РПН
                    current_ekran.current_level = EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_RPN + (current_ekran.index_position - INDEX_ML_GROUP1);
                  }
                  else if (current_ekran.index_position == INDEX_ML_CONTROL_WITH_GROUPS)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення управлінської інформації для РПН
                    current_ekran.current_level = EKRAN_CONTROL_RPN;
                  }
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                  current_ekran.cursor_on = 1;
                  current_ekran.cursor_blinking_on = 0;
                }
                else if (
                  (current_ekran.current_level >= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_RPN) &&
                  (current_ekran.current_level <= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP4_RPN))
                {
                  //Натисну кнопка Enter у вікні вибору уставок-витримок РПН
                  if (current_ekran.index_position == INDEX_ML_SETPOINT)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення уставок для РПН
                    current_ekran.current_level = EKRAN_SETPOINT_RPN_GROUP1 + (current_ekran.current_level - EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_RPN);
                  }
                  else if (current_ekran.index_position == INDEX_ML_TIMEOUT)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення витримок для РПН
                    current_ekran.current_level = EKRAN_TIMEOUT_RPN_GROUP1 + (current_ekran.current_level - EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_RPN);
                  }
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                }
                else if (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_ZSKH)
                {
                  if (current_ekran.index_position == INDEX_ML_WITH_CONTROL_CONTROL)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення управлінської інформації для ЗСХ
                    current_ekran.current_level = EKRAN_CONTROL_SZKh;
                  }
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                  current_ekran.cursor_on = 1;
                  current_ekran.cursor_blinking_on = 0;
                }
                else if (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_BRP)
                {
                  //Натисну кнопка Enter у вікні вибору група1-...-групаN-настройки БРП
                  if (
                    (current_ekran.index_position >= INDEX_ML_GROUP1) &&
                    (current_ekran.index_position <= INDEX_ML_GROUP4))
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення атбраної групи уставок для БРП
                    current_ekran.current_level = EKRAN_CHOOSE_SETPOINT_GROUP1_BRP + (current_ekran.index_position - INDEX_ML_GROUP1);
                  }
                  else if (current_ekran.index_position == INDEX_ML_CONTROL_WITH_GROUPS)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення управлінської інформації для БРП
                    current_ekran.current_level = EKRAN_CONTROL_BRP;
                  }
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                  current_ekran.cursor_on = 1;
                  current_ekran.cursor_blinking_on = 0;
                }
                else if (
                  (current_ekran.current_level >= EKRAN_CHOOSE_SETPOINT_GROUP1_BRP) &&
                  (current_ekran.current_level <= EKRAN_CHOOSE_SETPOINT_GROUP4_BRP))
                {
                  //Натисну кнопка Enter у вікні вибору уставок-витримок БРП
                  if (current_ekran.index_position == INDEX_ML_WITH_SETPOINT_SETPOINT)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення уставок для БРП
                    current_ekran.current_level = EKRAN_SETPOINT_BRP_GROUP1 + (current_ekran.current_level - EKRAN_CHOOSE_SETPOINT_GROUP1_BRP);
                  }

                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                }
                else if (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_ZNKh)
                {
                  //Натисну кнопка Enter у вікні вибору група1-...-групаN-настройки ЗНХ
                  if (
                    (current_ekran.index_position >= INDEX_ML_GROUP1) &&
                    (current_ekran.index_position <= INDEX_ML_GROUP4))
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення обраної групи уставок для ЗНХ
                    current_ekran.current_level = EKRAN_CHOOSE_TIMEOUT_GROUP1_ZNKh + (current_ekran.index_position - INDEX_ML_GROUP1);
                  }
                  else if (current_ekran.index_position == INDEX_ML_CONTROL_WITH_GROUPS)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення управлінської інформації для ЗНХ
                    current_ekran.current_level = EKRAN_CONTROL_ZNKh;
                  }
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                  current_ekran.cursor_on = 1;
                  current_ekran.cursor_blinking_on = 0;
                }
                else if (
                  (current_ekran.current_level >= EKRAN_CHOOSE_TIMEOUT_GROUP1_ZNKh) &&
                  (current_ekran.current_level <= EKRAN_CHOOSE_TIMEOUT_GROUP4_ZNKh))
                {
                  //Натисну кнопка Enter у вікні вибору витримок ЗНХ
                  if (current_ekran.index_position == INDEX_ML_WITH_TIMEOUT_TIMEOUT)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення витримок для ЗНХ
                    current_ekran.current_level = EKRAN_TIMEOUT_ZNKh_GROUP1 + (current_ekran.current_level - EKRAN_CHOOSE_TIMEOUT_GROUP1_ZNKh);
                  }
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                }
                else if (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_UMIN)
                {
                  //Натисну кнопка Enter у вікні вибору група1-...-групаN-настройки Umin
                  if (
                    (current_ekran.index_position >= INDEX_ML_GROUP1) &&
                    (current_ekran.index_position <= INDEX_ML_GROUP4))
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення обраної групи уставок для Umin
                    current_ekran.current_level = EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_UMIN + (current_ekran.index_position - INDEX_ML_GROUP1);
                  }
                  else if (current_ekran.index_position == INDEX_ML_CONTROL_WITH_GROUPS)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення управлінської інформації для Umin
                    current_ekran.current_level = EKRAN_CONTROL_UMIN;
                  }
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                  current_ekran.cursor_on = 1;
                  current_ekran.cursor_blinking_on = 0;
                }
                else if (
                  (current_ekran.current_level >= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_UMIN) &&
                  (current_ekran.current_level <= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP4_UMIN))
                {
                  //Натисну кнопка Enter у вікні вибору уставок-витримок Umin
                  if (current_ekran.index_position == INDEX_ML_SETPOINT)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення уставок для Umin
                    current_ekran.current_level = EKRAN_SETPOINT_UMIN_GROUP1 + (current_ekran.current_level - EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_UMIN);
                  }
                  else if (current_ekran.index_position == INDEX_ML_TIMEOUT)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення витримок для Umin
                    current_ekran.current_level = EKRAN_TIMEOUT_UMIN_GROUP1 + (current_ekran.current_level - EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_UMIN);
                  }
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                }
                else if (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_UMAX)
                {
                  //Натисну кнопка Enter у вікні вибору група1-...-групаN-настройки Umax
                  if (
                    (current_ekran.index_position >= INDEX_ML_GROUP1) &&
                    (current_ekran.index_position <= INDEX_ML_GROUP4))
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення обраної групи уставок для Umax
                    current_ekran.current_level = EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_UMAX + (current_ekran.index_position - INDEX_ML_GROUP1);
                  }
                  else if (current_ekran.index_position == INDEX_ML_CONTROL_WITH_GROUPS)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення управлінської інформації для Umax
                    current_ekran.current_level = EKRAN_CONTROL_UMAX;
                  }
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                  current_ekran.cursor_on = 1;
                  current_ekran.cursor_blinking_on = 0;
                }
                else if (
                  (current_ekran.current_level >= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_UMAX) &&
                  (current_ekran.current_level <= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP4_UMAX))
                {
                  //Натисну кнопка Enter у вікні вибору уставок-витримок Umax
                  if (current_ekran.index_position == INDEX_ML_SETPOINT)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення уставок для Umax
                    current_ekran.current_level = EKRAN_SETPOINT_UMAX_GROUP1 + (current_ekran.current_level - EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_UMAX);
                  }
                  else if (current_ekran.index_position == INDEX_ML_TIMEOUT)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення витримок для Umax
                    current_ekran.current_level = EKRAN_TIMEOUT_UMAX_GROUP1 + (current_ekran.current_level - EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_UMAX);
                  }
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                }
                else if (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_UP)
                {
                  //Натисну кнопка Enter у вікні вибору група1-...-групаN-настройки УЗ
                  if (
                    (current_ekran.index_position >= INDEX_ML_GROUP1) &&
                    (current_ekran.index_position <= INDEX_ML_GROUP4))
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення обраної групи уставок для УЗ
                    current_ekran.current_level = EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_UP + (current_ekran.index_position - INDEX_ML_GROUP1);
                  }
                  else if (current_ekran.index_position == INDEX_ML_CONTROL_WITH_GROUPS)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення управлінської інформації для УЗ
                    current_ekran.current_level = EKRAN_CONTROL_UP;
                  }
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                  current_ekran.cursor_on = 1;
                  current_ekran.cursor_blinking_on = 0;
                }
                else if (
                  (current_ekran.current_level >= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_UP) &&
                  (current_ekran.current_level <= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP4_UP))
                {
                  //Натисну кнопка Enter у вікні вибору уставок-витримок УЗ
                  if (current_ekran.index_position == INDEX_ML_SETPOINT)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення уставок для УЗ
                    current_ekran.current_level = EKRAN_SETPOINT_UP_GROUP1 + (current_ekran.current_level - EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_UP);
                  }
                  else if (current_ekran.index_position == INDEX_ML_TIMEOUT)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення витримок для УЗ
                    current_ekran.current_level = EKRAN_TIMEOUT_UP_GROUP1 + (current_ekran.current_level - EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_UP);
                  }
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                }
                else if (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_SWITCHER)
                {
                  //Натисну кнопка Enter у вікні вибору настройок виключателя
                  if (current_ekran.index_position == INDEX_ML_SETPOINT_SWITCHER)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення уставок для вимикача
                    current_ekran.current_level = EKRAN_SETPOINT_SWITCH;
                  }
                  else if (current_ekran.index_position == INDEX_ML_TIMEOUT_SWITCHER)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення витримок для вимикача
                    current_ekran.current_level = EKRAN_TIMEOUT_SWITCH;
                  }
                  else if (current_ekran.index_position == INDEX_ML_CONTROL_SWITCHER)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення управлінської інформації для вимикача
                    current_ekran.current_level = EKRAN_CONTROL_SWITCH;
                  }
                  else if (current_ekran.index_position == INDEX_ML_RANG_SWITCHER)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення ранжування управління вимикачем
                    current_ekran.current_level = EKRAN_CHOOSE_RANG_SWITCH;
                  }
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                  current_ekran.cursor_on = 1;
                  current_ekran.cursor_blinking_on = 0;
                }
                else if (current_ekran.current_level == EKRAN_CHOOSE_RANG_SWITCH)
                {
                  //Натисну кнопка Enter у вікні вибору Бвимк./БУвімк.
                  if (current_ekran.index_position == INDEX_ML_RANG_OFF_CB)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення ранжування ББвимк.
                    current_ekran.current_level = EKRAN_RANGUVANNJA_OFF_CB;
                    //Для того, щоб при першому входженні завжди список починався із першої ранжованої функції обнуляємо цю позицію
                    position_in_current_level_menu[current_ekran.current_level] = 0;
                  }
                  else if (current_ekran.index_position == INDEX_ML_RANG_ON_CB)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення ранжування БУвімк.
                    current_ekran.current_level = EKRAN_RANGUVANNJA_ON_CB;
                    //Для того, щоб при першому входженні завжди список починався із першої ранжованої функції обнуляємо цю позицію
                    position_in_current_level_menu[current_ekran.current_level] = 0;
                  }
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                  current_ekran.cursor_on = 1;
                  current_ekran.cursor_blinking_on = 0;
                }
                else if (current_ekran.current_level == EKRAN_CHOSE_SETTINGS)
                {
                  //Натисну кнопка Enter у вікні вибору настройок
                  if (current_ekran.index_position == INDEX_OF_INFO)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення інформації по версії програмного забезпечення і версії карти пам'яті
                    current_ekran.current_level = EKRAN_INFO;
                  }
                  else if (current_ekran.index_position == INDEX_OF_SETTINGS_POINT)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення вибору міток настройок
                    current_ekran.current_level = EKRAN_POINT_TIME_SETTINGS;
                  }
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                  else if (current_ekran.index_position == INDEX_OF_IEC61850)
                  {
                    current_ekran.current_level = EKRAN_LIST_TYPE_IEC61850_NODES;
                  }
#endif
                  else if (current_ekran.index_position == INDEX_OF_INPUTS_RANGUVANNJA)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення списку дискретних входів
                    current_ekran.current_level = EKRAN_LIST_INPUTS_FOR_RANGUVANNJA;
                  }
                  else if (current_ekran.index_position == INDEX_OF_OUTPUTS_RANGUVANNJA)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення списку дискретних виходів
                    current_ekran.current_level = EKRAN_LIST_OUTPUTS_FOR_RANGUVANNJA;
                  }
                  else if (current_ekran.index_position == INDEX_OF_LEDS_RANGUVANNJA)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення списку дискретних світодіодів
                    current_ekran.current_level = EKRAN_LIST_LEDS_FOR_RANGUVANNJA;
                  }
                  else if (current_ekran.index_position == INDEX_OF_SWITCHER)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення вибору настройок виключателя
                    current_ekran.current_level = EKRAN_CHOOSE_SETTINGS_SWITCHER;
                  }
                  else if (current_ekran.index_position == INDEX_OF_TRANSFORMATOR)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення настройок трансорматора
                    current_ekran.current_level = EKRAN_TRANSFORMATOR_INFO;
                  }
                  else if (current_ekran.index_position == INDEX_OF_CONTROL_INPUTS_OUTPUTS)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення настройок УВВ
                    current_ekran.current_level = EKRAN_CHOOSE_SETTINGS_UVV;
                  }
                  else if (current_ekran.index_position == INDEX_OF_COMMUNICATION)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення настройок комунікації
                    current_ekran.current_level = EKRAN_CHOSE_COMMUNICATION_PARAMETERS;
                  }
                  else if (current_ekran.index_position == INDEX_OF_DATA_AND_TIME)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення налаштувань дати і часу
                    current_ekran.current_level = EKRAN_CHOSE_DATA_TIME;
                  }
                  else if (current_ekran.index_position == INDEX_OF_REGISTRATORS)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення списку наявних реєстраторів
                    current_ekran.current_level = EKRAN_VIEW_LIST_OF_REGISTRATORS;
                  }
                  else if (current_ekran.index_position == INDEX_OF_EXTENDED_LOGIC)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення списку розширеної логіки
                    current_ekran.current_level = EKRAN_EXTENDED_LIGIC;
                  }
                  else if (current_ekran.index_position == INDEX_OF_DEFINED_BUTTONS)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення списку настроювання функціональних кнопок
                    current_ekran.current_level = EKRAN_LIST_BUTTONS_FOR_RANGUVANNJA;
                  }
                  else if (current_ekran.index_position == INDEX_OF_GRUPA_USTAVOK)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню вибору груп уставок
                    current_ekran.current_level = EKRAN_VIEW_GRUPA_USTAVOK;
                  }
                  else if (current_ekran.index_position == INDEX_OF_EXTRA_SETTINGS)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню додаткових нлаштувань
                    current_ekran.current_level = EKRAN_CHOSE_EXTRA_SETTINGS;
                  }
                  else if (current_ekran.index_position == INDEX_OF_LIST_PASSWORDS)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню списку паролів
                    current_ekran.current_level = EKRAN_LEVEL_CHOOSE_PASSWORDS;
                  }
                  //                else if(current_ekran.index_position == INDEX_OF_PASSWORD)
                  //                {
                  //                  //Запам'ятовуємо поперердній екран
                  //                  //Переходимо на меню зміни паролю
                  //                  current_ekran.current_level = EKRAN_LEVEL_SET_NEW_PASSWORD;
                  //                }
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                  //                if (current_ekran.current_level != EKRAN_LEVEL_SET_NEW_PASSWORD) current_ekran.edition = 0;
                  //                else current_ekran.edition = 1;
                }
                else if (current_ekran.current_level == EKRAN_LEVEL_CHOOSE_PASSWORDS)
                {
                  //Натисну кнопка Enter у вікні вибору паролів
                  if (current_ekran.index_position == INDEX_OF_PASSWORD1)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню зміни паролю
                    current_ekran.current_level = EKRAN_LEVEL_SET_NEW_PASSWORD1;
                  }
                  else if (current_ekran.index_position == INDEX_OF_PASSWORD2)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню зміни паролю
                    current_ekran.current_level = EKRAN_LEVEL_SET_NEW_PASSWORD2;
                  }
                  else
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню зміни паролю
                    current_ekran.current_level = EKRAN_LEVEL_SET_NEW_PASSWORD3;
                  }

                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 1;
                }
                else if (current_ekran.current_level == EKRAN_LIST_INPUTS_FOR_RANGUVANNJA)
                {
                  //Запам'ятовуємо поперердній екран
                  //Переходимо на меню відображення списку ранжованих функцій
                  current_ekran.current_level = EKRAN_RANGUVANNJA_INPUT_1 + current_ekran.index_position;
                  //Для того, щоб при першому входженні завжди список починався із першої ранжованої функції обнуляємо цю позицію
                  position_in_current_level_menu[current_ekran.current_level] = 0;
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                }
                else if (current_ekran.current_level == EKRAN_LIST_OUTPUTS_FOR_RANGUVANNJA)
                {
                  //Запам'ятовуємо поперердній екран
                  //Переходимо на меню відображення списку ранжованих функцій
                  current_ekran.current_level = EKRAN_RANGUVANNJA_OUTPUT_1 + current_ekran.index_position;
                  //Для того, щоб при першому входженні завжди список починався із першої ранжованої функції обнуляємо цю позицію
                  position_in_current_level_menu[current_ekran.current_level] = 0;
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                }
                else if (current_ekran.current_level == EKRAN_LIST_LEDS_FOR_RANGUVANNJA)
                {
                  //Запам'ятовуємо поперердній екран
                  //Переходимо на меню відображення списку ранжованих функцій
                  current_ekran.current_level = EKRAN_RANGUVANNJA_LED_1 + current_ekran.index_position;
                  //Для того, щоб при першому входженні завжди список починався із першої ранжованої функції обнуляємо цю позицію
                  position_in_current_level_menu[current_ekran.current_level] = 0;
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                }
                else if (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_UVV)
                {
                  //Натисну кнопка Enter у вікні вибору вікна настройок УВВ
                  if (current_ekran.index_position == INDEX_ML_UVV_DOPUSKT)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення списку дискретних входів
                    current_ekran.current_level = EKRAN_DOPUSK_DV_UVV;
                  }
                  else if (current_ekran.index_position == INDEX_ML_UVV_TYPE_INPUT)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення списку дискретних входів
                    current_ekran.current_level = EKRAN_TYPE_INPUT_UVV;
                  }
                  else if (current_ekran.index_position == INDEX_ML_UVV_TYPE_INPUT_SIGNAL)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення списку дискретних входів
                    current_ekran.current_level = EKRAN_TYPE_INPUT_SIGNAL_UVV;
                  }
                  else if (current_ekran.index_position == INDEX_ML_UVV_TYPE_OUTPUT)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення типу виходів
                    current_ekran.current_level = EKRAN_TYPE_OUTPUT_UVV;
                  }
                  else if (current_ekran.index_position == INDEX_ML_UVV_TYPE_LED)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення типу свівтлоіндикаторів
                    current_ekran.current_level = EKRAN_TYPE_LED_UVV;
                  }
                  else if (current_ekran.index_position == INDEX_ML_UVV_TYPE_DB)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення типу ФК
                    current_ekran.current_level = EKRAN_TYPE_BUTTON_UVV;
                  }
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                }
                else if (current_ekran.current_level == EKRAN_CHOSE_COMMUNICATION_PARAMETERS)
                {
                  //Натисну кнопка Enter у вікні вибору вікна настройок комунікації
                  if (current_ekran.index_position == INDEX_ML_CHCP_NAME)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення імені ячейки
                    current_ekran.current_level = EKRAN_VIEW_NAME_OF_CELL;
                  }
                  else if (current_ekran.index_position == INDEX_ML_CHCP_SETTING_RS485)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення списку нашаштувань RS-485
                    current_ekran.current_level = EKRAN_CHOSE_SETTING_RS485;
                  }
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                  else if (current_ekran.index_position == INDEX_ML_CHCP_SETTING_ETHERNET)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення списку нашаштувань Ethernet
                    current_ekran.current_level = EKRAN_CHOSE_SETTING_ETHERNET;
                  }
#endif
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                }
                else if (current_ekran.current_level == EKRAN_CHOSE_SETTING_RS485)
                {
                  //Натисну кнопка Enter у вікні вибору вікна настройок RS-485
                  if (current_ekran.index_position == INDEX_ML_PHY_LAYER_RS485)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення налаштувань фізичного рівня RS-485
                    current_ekran.current_level = EKRAN_PHY_LAYER_RS485;
                  }
                  else if (current_ekran.index_position == INDEX_ML_PROTOCOL_RS485)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення протоколів RS-485
                    current_ekran.current_level = EKRAN_PROTOCOL_RS485;
                  }
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                }
                else if (current_ekran.current_level == EKRAN_PHY_LAYER_RS485)
                {
                  //Натисну кнопка Enter у вікні вибору вікна настройок фізичного рівня RS-485
                  if (current_ekran.index_position == INDEX_ML_CHSRS485_SPEED)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення швидкості обміну
                    current_ekran.current_level = EKRAN_VIEW_SPEED_RS485;
                  }
                  else if (current_ekran.index_position == INDEX_ML_CHSRS485_PARE_BIT)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення інформації по бітупаритету
                    current_ekran.current_level = EKRAN_VIEW_PARE_RS485;
                  }
                  else if (current_ekran.index_position == INDEX_ML_CHSRS485_NUMBER_STOP_BIT)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення інфрпмації по стоп-біту
                    current_ekran.current_level = EKRAN_VIEW_STOP_BITS_RS485;
                  }
                  else if (current_ekran.index_position == INDEX_ML_CHSRS485_TIMEOUT)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення інфрпмації по time-out наступного символу
                    current_ekran.current_level = EKRAN_VIEW_TIMEOUT_RS485;
                  }
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                }
                else if (current_ekran.current_level == EKRAN_PROTOCOL_RS485)
                {
                  //Натисну кнопка Enter у вікні вибору налаштувань протоколу RS-485
                  if (current_ekran.index_position == INDEX_ML_ADDRESS)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення мережевої адреси
                    current_ekran.current_level = EKRAN_ADDRESS_RS485;
                  }
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                }
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                else if (current_ekran.current_level == EKRAN_LIST_TYPE_IEC61850_NODES)
                {
                  //Натисну кнопка Enter у вікні вибору вікна настройок УВВ
                  if (current_ekran.index_position == INDEX_TYPE_IEC61850_IN_GOOSE)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення списку вхідних GOOSE блоків
                    current_ekran.current_level = EKRAN_LIST_IN_GOOSE;
                  }
                  else if (current_ekran.index_position == INDEX_TYPE_IEC61850_IN_MMS)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення списку вхідних MMS блоків
                    current_ekran.current_level = EKRAN_LIST_IN_MMS;
                  }
                  else if (current_ekran.index_position == INDEX_TYPE_IEC61850_OUT_LAN)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення списку вихідних мережевих блоків
                    current_ekran.current_level = EKRAN_LIST_OUT_LAN;
                  }
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                }
                else if (current_ekran.current_level == EKRAN_LIST_IN_GOOSE)
                {
                  //Запам'ятовуємо поперердній екран
                  //Переходимо на меню відображення вибору списку виходів Вх.GOOSE блоку
                  current_ekran.current_level = EKRAN_IN_GOOSE1 + current_ekran.index_position;
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                }
                else if (current_ekran.current_level == EKRAN_LIST_IN_MMS)
                {
                  //Запам'ятовуємо поперердній екран
                  //Переходимо на меню відображення вибору  списку виходів Вх.MMS блоку
                  current_ekran.current_level = EKRAN_IN_MMS1 + current_ekran.index_position;
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                }
                else if (current_ekran.current_level == EKRAN_LIST_OUT_LAN)
                {
                  //Запам'ятовуємо поперердній екран
                  //Переходимо на меню відображення вибору  списку входів Вих. мережевого блоку
                  current_ekran.current_level = EKRAN_OUT_LAN1 + current_ekran.index_position;
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                }
                else if (
                  ((current_ekran.current_level >= EKRAN_IN_GOOSE1) && (current_ekran.current_level <= EKRAN_IN_GOOSE16)) ||
                  ((current_ekran.current_level >= EKRAN_IN_MMS1) && (current_ekran.current_level <= EKRAN_IN_MMS4)))
                {
                  //Запам'ятовуємо поперердній екран
                  //Переходимо на меню відображення списку ранжованих функцій (малий список)
                  current_ekran.current_level = EKRAN_LN_FOR_IEC61850_RANG_SMALL;
                  //Для того, щоб при першому входженні завжди список починався із першої ранжованої функції обнуляємо цю позицію
                  position_in_current_level_menu[EKRAN_LN_FOR_IEC61850_RANG_SMALL] = 0;
                  current_ekran.index_position = position_in_current_level_menu[EKRAN_LN_FOR_IEC61850_RANG_SMALL];
                  current_ekran.edition = 0;
                }
                else if ((current_ekran.current_level >= EKRAN_OUT_LAN1) && (current_ekran.current_level <= EKRAN_OUT_LAN4))
                {
                  //Запам'ятовуємо поперердній екран
                  //Переходимо на меню відображення списку ранжованих функцій (великий список)
                  current_ekran.current_level = EKRAN_LN_FOR_IEC61850_RANG;
                  //Для того, щоб при першому входженні завжди список починався із першої ранжованої функції обнуляємо цю позицію
                  position_in_current_level_menu[EKRAN_LN_FOR_IEC61850_RANG] = 0;
                  current_ekran.index_position = position_in_current_level_menu[EKRAN_LN_FOR_IEC61850_RANG];
                  current_ekran.edition = 0;
                }
                else if (current_ekran.current_level == EKRAN_CHOSE_SETTING_ETHERNET)
                {
                  //Натисну кнопка Enter у вікні вибору налаштувань Ethernet
                  if (current_ekran.index_position == INDEX_ML_NETWORK_LAYER_ETHERNET)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення мережевих налаштувань
                    current_ekran.current_level = EKRAN_SETTING_NETWORK_LAYER_ETHERNET;
                  }
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                }
#endif
                else if (current_ekran.current_level == EKRAN_CHOSE_DATA_TIME)
                {
                  //Натисну кнопка Enter у вікні вибору вікна настройок налаштувань дл дати і часу
                  if (current_ekran.index_position == INDEX_ML_CHDT_TIME_ZONE)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення налаштувань часової зони і правил переходу на літній час
                    current_ekran.current_level = EKRAN_TIME_ZONE;
                  }
                  else if (
                    (current_ekran.index_position == INDEX_ML_CHDT_DST_ON) ||
                    (current_ekran.index_position == INDEX_ML_CHDT_DST_OFF))
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення правил переходу між літнім часом і стандартним часом
                    current_ekran.current_level = EKRAN_DST_RULE;
                  }
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                  else if (current_ekran.index_position == INDEX_ML_CHDT_SYNCHRO)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення наштувань для синхронізації по мережі
                    current_ekran.current_level = EKRAN_SYNCHRO;
                  }
#endif
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                }
                else if (current_ekran.current_level == EKRAN_VIEW_LIST_OF_REGISTRATORS)
                {
                  if (current_ekran.index_position == INDEX_ML_DIGITAL_REGISTRATOR)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення списку ранжованих функцій для дискретного реєстратора
                    current_ekran.current_level = EKRAN_VIEW_SETTINGS_OF_DIGITAL_REGISTRATORS; //!@EKRAN_RANGUVANNJA_DIGITAL_REGISTRATOR;
                    //Для того, щоб при першому входженні завжди список починався із першої ранжованої функції обнуляємо цю позицію
                    //!@position_in_current_level_menu[current_ekran.current_level] = 0;
                  }
                  else if (current_ekran.index_position == INDEX_ML_ANALOG_REGISTRATOR)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення списку настройок для аналогового реєстратора
                    current_ekran.current_level = EKRAN_VIEW_SETTINGS_OF_ANALOG_REGISTRATORS;
                  }
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                }
                else if (current_ekran.current_level == EKRAN_VIEW_SETTINGS_OF_ANALOG_REGISTRATORS)
                {
                  if (current_ekran.index_position == INDEX_ML_SOURCE_ANALOG_REGISTRATOR)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення списку ранжованих функцій для аналогового реєстратора
                    current_ekran.current_level = EKRAN_RANGUVANNJA_ANALOG_REGISTRATOR;
                    //Для того, щоб при першому входженні завжди список починався із першої ранжованої функції обнуляємо цю позицію
                    position_in_current_level_menu[current_ekran.current_level] = 0;
                  }
                  else if (current_ekran.index_position == INDEX_ML_TIMEOUT_ANALOG_REGISTRATOR)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення витримок для аналогового реєстратора
                    current_ekran.current_level = EKRAN_TIMEOUT_ANALOG_REGISTRATOR;
                  }
                  else if (current_ekran.index_position == INDEX_ML_CONTROL_ANALOG_REGISTRATOR)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення управління для аналогового реєстратора
                    current_ekran.current_level = EKRAN_CONTROL_AR;
                  }
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                }
                else if (current_ekran.current_level == EKRAN_VIEW_SETTINGS_OF_DIGITAL_REGISTRATORS)
                {
                  if (current_ekran.index_position == INDEX_ML_SOURCE_DIGITAL_REGISTRATOR)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення списку ранжованих функцій для аналогового реєстратора
                    current_ekran.current_level = EKRAN_RANGUVANNJA_DIGITAL_REGISTRATOR;
                    //Для того, щоб при першому входженні завжди список починався із першої ранжованої функції обнуляємо цю позицію
                    position_in_current_level_menu[current_ekran.current_level] = 0;
                  }
                  else if (current_ekran.index_position == INDEX_ML_TIMEOUT_DIGITAL_REGISTRATOR)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення витримок для аналогового реєстратора
                    current_ekran.current_level = EKRAN_TIMEOUT_DIGITAL_REGISTRATOR;
                  }
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                }
                else if (current_ekran.current_level == EKRAN_EXTENDED_LIGIC)
                {
                  if (current_ekran.index_position == INDEX_OF_GENERAL_PICKUP_EL)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення загальних уставок для розширеної логіки
                    current_ekran.current_level = EKRAN_GENERAL_PICKUPS_EL;
                  }
                  else if (current_ekran.index_position == INDEX_OF_DEFINED_FUNCTIONS)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення списку настроювання опреділювальних функцій
                    current_ekran.current_level = EKRAN_LIST_SETTINGS_FOR_DF;
                  }
                  else if (current_ekran.index_position == INDEX_OF_DEFINED_TRIGGERS)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення списку опреділювальних триґерів
                    current_ekran.current_level = EKRAN_LIST_DT;
                  }
                  else if (current_ekran.index_position == INDEX_OF_DEFINED_AND)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення списку визначуваних "І"
                    current_ekran.current_level = EKRAN_LIST_D_AND;
                  }
                  else if (current_ekran.index_position == INDEX_OF_DEFINED_OR)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення списку визначуваних "АБО"
                    current_ekran.current_level = EKRAN_LIST_D_OR;
                  }
                  else if (current_ekran.index_position == INDEX_OF_DEFINED_XOR)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення списку визначуваних "Викл.АБО"
                    current_ekran.current_level = EKRAN_LIST_D_XOR;
                  }
                  else if (current_ekran.index_position == INDEX_OF_DEFINED_NOT)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення списку визначуваних "НЕ"
                    current_ekran.current_level = EKRAN_LIST_D_NOT;
                  }
                  else if (current_ekran.index_position == INDEX_OF_TRANSFER_FUNCTIONS)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення списку визначуваних "НЕ"
                    current_ekran.current_level = EKRAN_LIST_TF_FOR_RANGUVANNJA;
                  }
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                }
                else if (current_ekran.current_level == EKRAN_LIST_SETTINGS_FOR_DF)
                {
                  //Натисну кнопка Enter у вікні вибору вікна настройок опреділювальнтої функції
                  if (current_ekran.index_position == INDEX_ML_SETTING_DF_SOURCE)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення списку оперділювальних функцій для їх ранжування
                    current_ekran.current_level = EKRAN_LIST_DF_FOR_RANGUVANNJA;
                  }
                  else if (current_ekran.index_position == INDEX_ML_SETTING_DF_TIMEOUT)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення списку оперділювальних функцій для їх параметрування часу павзи і роботи
                    current_ekran.current_level = EKRAN_LIST_DF_FOR_TIMEOUT_SETTINGS;
                  }
                  else if (current_ekran.index_position == INDEX_ML_SETTING_DF_TYPE)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення типу опреділювальних функцій
                    current_ekran.current_level = EKRAN_LIST_TYPE_DF;
                  }
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                }
                else if (current_ekran.current_level == EKRAN_LIST_DF_FOR_TIMEOUT_SETTINGS)
                {
                  //Запам'ятовуємо поперердній екран
                  //Переходимо на меню відображення списку ранжованих функцій
                  current_ekran.current_level = EKRAN_TIMEOUT_DF1 + current_ekran.index_position;
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                }
                else if (current_ekran.current_level == EKRAN_LIST_DF_FOR_RANGUVANNJA)
                {
                  //Запам'ятовуємо поперердній екран
                  //Переходимо на меню відображення вибору типу джерел для ранжування опреділюваної функції
                  current_ekran.current_level = EKRAN_LIST_TYPE_SOURCE_DF1 + current_ekran.index_position;
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                }
                else if ((current_ekran.current_level >= EKRAN_LIST_TYPE_SOURCE_DF1) && (current_ekran.current_level <= (EKRAN_LIST_TYPE_SOURCE_DF1 + NUMBER_DEFINED_FUNCTIONS - 1)))
                {
                  //Запам'ятовуємо поперердній екран
                  //Переходимо на меню відображення списку ранжованих функцій
                  current_ekran.current_level = EKRAN_RANGUVANNJA_DF1_PLUS + MAX_ROW_LIST_TYPE_SOURCE_DF * (current_ekran.current_level - EKRAN_LIST_TYPE_SOURCE_DF1) + current_ekran.index_position;
                  //Для того, щоб при першому входженні завжди список починався із першої ранжованої функції обнуляємо цю позицію
                  position_in_current_level_menu[current_ekran.current_level] = 0;
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                }
                else if (current_ekran.current_level == EKRAN_LIST_DT)
                {
                  //Запам'ятовуємо поперердній екран
                  //Переходимо на меню відображення заголовків активація-деактивація опреділювальних триґерів
                  current_ekran.current_level = EKRAN_SET_RESET_DT1 + current_ekran.index_position;
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                }
                else if ((current_ekran.current_level >= EKRAN_SET_RESET_DT1) && (current_ekran.current_level <= (EKRAN_SET_RESET_DT1 + NUMBER_DEFINED_TRIGGERS - 1)))
                {
                  //Запам'ятовуємо поперердній екран
                  //Переходимо на меню відображення заголовків джерел активації-деактивації опреділювальних триґерів
                  current_ekran.current_level = EKRAN_SET_DT1 + 2 * (current_ekran.current_level - EKRAN_SET_RESET_DT1) + current_ekran.index_position;
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                }
                else if ((current_ekran.current_level >= EKRAN_SET_DT1) && (current_ekran.current_level <= (EKRAN_SET_DT1 + 2 * NUMBER_DEFINED_TRIGGERS - 1)))
                {
                  //Запам'ятовуємо поперердній екран
                  //Переходимо на меню відображення списку ранжованих функцій
                  current_ekran.current_level = EKRAN_RANGUVANNJA_SET_DT1_PLUS + 2 * (current_ekran.current_level - EKRAN_SET_DT1) + current_ekran.index_position;
                  //Для того, щоб при першому входженні завжди список починався із першої ранжованої функції обнуляємо цю позицію
                  position_in_current_level_menu[current_ekran.current_level] = 0;
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                }
                else if (current_ekran.current_level == EKRAN_LIST_D_AND)
                {
                  //Запам'ятовуємо поперердній екран
                  //Переходимо на меню відображення списку ранжованих функцій
                  current_ekran.current_level = EKRAN_RANGUVANNJA_D_AND1 + current_ekran.index_position;
                  //Для того, щоб при першому входженні завжди список починався із першої ранжованої функції обнуляємо цю позицію
                  position_in_current_level_menu[current_ekran.current_level] = 0;
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                }
                else if (current_ekran.current_level == EKRAN_LIST_D_OR)
                {
                  //Запам'ятовуємо поперердній екран
                  //Переходимо на меню відображення списку ранжованих функцій
                  current_ekran.current_level = EKRAN_RANGUVANNJA_D_OR1 + current_ekran.index_position;
                  //Для того, щоб при першому входженні завжди список починався із першої ранжованої функції обнуляємо цю позицію
                  position_in_current_level_menu[current_ekran.current_level] = 0;
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                }
                else if (current_ekran.current_level == EKRAN_LIST_D_XOR)
                {
                  //Запам'ятовуємо поперердній екран
                  //Переходимо на меню відображення списку ранжованих функцій
                  current_ekran.current_level = EKRAN_RANGUVANNJA_D_XOR1 + current_ekran.index_position;
                  //Для того, щоб при першому входженні завжди список починався із першої ранжованої функції обнуляємо цю позицію
                  position_in_current_level_menu[current_ekran.current_level] = 0;
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                }
                else if (current_ekran.current_level == EKRAN_LIST_D_NOT)
                {
                  //Запам'ятовуємо поперердній екран
                  //Переходимо на меню відображення списку ранжованих функцій
                  current_ekran.current_level = EKRAN_RANGUVANNJA_D_NOT1 + current_ekran.index_position;
                  //Для того, щоб при першому входженні завжди список починався із першої ранжованої функції обнуляємо цю позицію
                  position_in_current_level_menu[current_ekran.current_level] = 0;
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                }
                else if (current_ekran.current_level == EKRAN_LIST_TF_FOR_RANGUVANNJA)
                {
                  //Запам'ятовуємо поперердній екран
                  //Переходимо на меню відображення вибору типу джерел для ранжування передавальної функції
                  current_ekran.current_level = EKRAN_LIST_SOURCE_TF1 + current_ekran.index_position;
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                }
                else if (current_ekran.current_level == EKRAN_LIST_BUTTONS_FOR_RANGUVANNJA)
                {
                  //Запам'ятовуємо поперердній екран
                  //Переходимо на меню відображення списку ранжованих функцій
                  current_ekran.current_level = EKRAN_RANGUVANNJA_BUTTON_1 + current_ekran.index_position;
                  //Для того, щоб при першому входженні завжди список починався із першої ранжованої функції обнуляємо цю позицію
                  position_in_current_level_menu[current_ekran.current_level] = 0;
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                }
                else if (current_ekran.current_level == EKRAN_INFO)
                {
                  //Натисну кнопка Enter у вікні інформації по версії прошивки і карти пам'яті
                  if (
                    (current_ekran.index_position == INDEX_ML_INFO_FIRMWARE)
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                    ||
                    (current_ekran.index_position == INDEX_ML_INFO_FIRMWARE_CB)
#endif
                  )
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення кдати і часу створення прошивки
                    current_ekran.current_level = EKRAN_DATE_TIME_PZ;

                    current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                    current_ekran.edition = 0;
                  }
                  else if (current_ekran.index_position == INDEX_ML_REPROGRAM)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню активації режиму перепрограмування
                    current_ekran.current_level = EKRAN_REPROGRAM;

                    current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                    current_ekran.edition = 0;

                    //Щоб зразу перейти на запит паролю
                    previous_level_in_current_level_menu[current_ekran.current_level] = temp_current_level;
                    temp_current_level = current_ekran.current_level;
                    current_ekran.current_level = EKRAN_LEVEL_PASSWORD_REPROGRAM;
                    current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  }
                }
                else if (current_ekran.current_level == EKRAN_POINT_TIME_SETTINGS)
                {
                  //Натисну кнопка Enter у вікні вибору міток останніх настройок
                  if (current_ekran.index_position == INDEX_ML_TIME_SETPOINT)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення часу останніх настройк уставок-витримок-управлінської інформації
                    current_ekran.current_level = EKRAN_POINT_TIME_SETPOINT;
                  }
                  else if (current_ekran.index_position == INDEX_ML_TIME_RANGUVANNJA)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення часу останніх настройк ранжування
                    current_ekran.current_level = EKRAN_POINT_TIME_RANGUVANNJA;
                  }
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                }
                else if (current_ekran.current_level == EKRAN_LIST_INPUTS_OUTPUTS)
                {
                  //Натисну кнопка Enter у вікні вибору станів входів-виходів
                  if (current_ekran.index_position == INDEX_ML_STATE_INPUTS)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення станів входів
                    current_ekran.current_level = EKRAN_STATE_INPUTS;
                  }
                  else
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення станів виходів
                    current_ekran.current_level = EKRAN_STATE_OUTPUTS;
                  }
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                }
                else if (current_ekran.current_level == EKRAN_LIST_REGISTRATORS)
                {
                  //Натисну кнопка Enter у вікні вибору реєстраторів
                  if (current_ekran.index_position == INDEX_ML_ANALOG_REGISTRATOR_INFO)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення аналогового реєстратора
                    current_ekran.current_level = EKRAN_LIST_ANALOG_REGISTRATOR_RECORDS;
                  }
                  else if (current_ekran.index_position == INDEX_ML_DIGITAL_REGISTRATOR_INFO)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення дискретного реєстратора
                    current_ekran.current_level = EKRAN_LIST_DIGITAL_REGISTRATOR_RECORDS;
                  }
                  else if (current_ekran.index_position == INDEX_ML_PROGRAM_ERROE_REGISTRATOR_INFO)
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення реєстратора програмних помилок
                    current_ekran.current_level = EKRAN_LIST_REGISTRATOR_PROGRAM_ERROR_RECORDS;
                  }
                  else
                  {
                    //Запам'ятовуємо поперердній екран
                    //Переходимо на меню відображення реєстратора програмних помилок
                    current_ekran.current_level = EKRAN_LIST_STATE_CMD_REGISTRATOR_RECORDS;
                  }
                  current_ekran.index_position = 0; //При відкриванні цих вікон з старших розділів меню завжди треба попадати на найновіший запис
                  current_ekran.edition = 0;
                }
                else if (
                  (current_ekran.current_level == EKRAN_LIST_DIGITAL_REGISTRATOR_RECORDS) &&
                  (info_rejestrator_dr.number_records > 0) &&
                  ((clean_rejestrators & CLEAN_DR) == 0))
                {
                  //Натисну кнопка Enter у вікні вибору запису дискретного реєстратора і реально є записи для відображення

                  //Запам'ятовуємо, який номер запису дискретного реєстратори ми намагаємося продивитися
                  number_record_of_dr_for_menu = current_ekran.index_position;

                  //Виставляємо першу частину запису
                  part_reading_dr_from_dataflash_for_menu = 0;
                  //Подаємо команду зчитати дані у бувер пам'яті
                  mutex_spi1 = true;
                  control_tasks_dataflash |= TASK_MAMORY_READ_DATAFLASH_FOR_DR_MENU;
                  mutex_spi1 = false;

                  //Виставляємо повідомлення, що поки дані не будуть зчитані, то екран треба перерисовувати кожну секунду
                  rewrite_ekran_once_more = 1;
                  //Виставляємо новий екран, який треба відобразити на РКІ
                  current_ekran.current_level = EKRAN_TITLES_DIGITAL_REGISTRATOR;
                  current_ekran.index_position = 0; //При відкриванні цих вікон з старших розділів меню завжди треба попадати на найновіший запис
                  current_ekran.edition = 0;
                }
                else if (current_ekran.current_level == EKRAN_TITLES_DIGITAL_REGISTRATOR)
                {
                  //Натисну кнопка Enter у вікні вибору груп інформації по запису дискретного реєстра
                  if (rewrite_ekran_once_more == 0)
                  {
                    if (current_ekran.index_position == INDEX_ML_TITLE_DR_CHANGES_SIGNALS)
                    {
                      //Переходимо на меню відображення зафіксованих змін сигналів у запису дискретного реєстратора
                      current_ekran.current_level = EKRAN_CHANGES_SIGNALS_DR;
                    }
                    else
                    {
                      //Переходимо на меню відображення зафіксованих аналоговихзначень у запису дискретного реєстратора
                      current_ekran.current_level = EKRAN_TITLE_MAX_VALUES;
                      type_view_max_values_dr = (current_ekran.index_position - INDEX_ML_TITLE_DR_MIN_U) + IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE;
                    }
                    current_ekran.index_position = 0;
                    current_ekran.edition = 0;
                  }
                }
                else if (current_ekran.current_level == EKRAN_TITLE_MAX_VALUES)
                {
                  //Натиснута кнопка Enter у вікні списку зафіксованих максимальних струмів
                  if ((index_cell_into_array_for_integral_values_dr >= FIRST_INDEX_FIRST_BLOCK_DR) && (index_cell_into_array_for_integral_values_dr <= ((int) (FIRST_INDEX_FIRST_DATA_DR - sizeof(unsigned int) * SIZE_ARRAY_FIX_MAX_MEASUREMENTS))))
                  {
                    //Переходимо у нове вікно тільки у тому випадку, якщо ми попередньо зафіксували з якого місця розміщається блок, який визначений курсором
                    current_ekran.current_level = EKRAN_MAX_VALUES;

                    //Спочатку відображаємо інформацію на вторинній обмотці трансворматора - значення , які поступають на аналогові входи приладу
                    pervynna_vtorynna = 0;

                    current_ekran.index_position = 0; //Завжди починаэмо з першого вікна
                    current_ekran.edition = 0;
                  }
                }
                else if (
                  (current_ekran.current_level == EKRAN_LIST_REGISTRATOR_PROGRAM_ERROR_RECORDS) &&
                  (info_rejestrator_pr_err.number_records > 0) &&
                  ((clean_rejestrators & CLEAN_PR_ERR) == 0))
                {
                  //Натисну кнопка Enter у вікні вибору запису реєстратора програмних подій і реально є записи для відображення

                  //Запам'ятовуємо, який номер запису реєстратора програмних подій ми намагаємося продивитися
                  number_record_of_pr_err_into_menu = current_ekran.index_position;

                  //Подаємо команду зчитати дані у бувер пам'яті
                  mutex_spi1 = true;
                  control_tasks_dataflash |= TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_MENU;
                  mutex_spi1 = false;

                  //Виставляємо новий екран, який треба відобразити на РКІ
                  current_ekran.current_level = EKRAN_CHANGES_DIAGNOSTICS_PR_ERR;
                  current_ekran.index_position = 0; //При відкриванні цих вікон з старших розділів меню завжди треба попадати на найновіший запис
                  current_ekran.edition = 0;
                }
                else if (
                  (current_ekran.current_level == EKRAN_LIST_STATE_CMD_REGISTRATOR_RECORDS) && (holderCmdPlusTime.shTotalFixElem > 0) //(&&info_rejestrator_pr_err.number_records > 0) &&
                                                                                                                                      //((clean_rejestrators & CLEAN_PR_ERR) == 0)
                )
                {
                  //Натисну кнопка Enter у вікні вибору запису реєстратора статистики і реально є записи для відображення

                  //Запам'ятовуємо, який номер запису реєстратора програмних подій ми намагаємося продивитися
                  number_record_of_stt_cmd_into_menu = current_ekran.index_position; //number_record_of_pr_err_into_menu = current_ekran.index_position;
                  current_number_changes_of_stt_cmd_into_menu = GetNumberChangingInLogElem(current_ekran.index_position);
                  //Подаємо команду зчитати дані у бувер пам'яті
                  //.control_tasks_dataflash |= TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_MENU;

                  //Виставляємо повідомлення, що поки дані не будуть зчитані, то екран треба перерисовувати кожну секунду
                  //.rewrite_ekran_once_more = 1;
                  //Виставляємо новий екран, який треба відобразити на РКІ
                  current_ekran.current_level = EKRAN_TITLES_STATE_CMD_REGISTRATOR; //.current_ekran.current_level = EKRAN_STATE_CMD_REG;. prev code

                  ekr2LineIndexPos = current_ekran.index_position; //?
                  if (selectorOneDateElem > 0)
                    current_ekran.index_position = Calc2lineIndexPosUsingBitCmdValuesAttachedToOneDateElem(selectorOneDateElem);
                  else
                  {
                    current_ekran.index_position = 0; //При відкриванні цих вікон з старших розділів меню завжди треба попадати на найновіший запис
                  }

                  current_ekran.edition = 0;
                }

                else if (
                  (current_ekran.current_level == EKRAN_TITLES_STATE_CMD_REGISTRATOR) && (holderCmdPlusTime.shTotalFixElem > 0) //
                                                                                                                                //((clean_rejestrators & CLEAN_PR_ERR) == 0)
                )
                {
                  //Натисну кнопка Enter у вікні вибору запису реєстратора статистики і реально є записи для відображення
                  //Виставляємо новий екран, який треба відобразити на РКІ
                  current_ekran.current_level = EKRAN_DATA_LABEL_STATE_CMD; //

                  current_ekran.index_position = 0;
                  current_ekran.edition = 0;
                }

                //У разі, якщо текучий екран змінився, то запам'ятовуємо екран в який требе буде повернутися
                if (temp_current_level != current_ekran.current_level)
                {
                  previous_level_in_current_level_menu[current_ekran.current_level] = temp_current_level;

                  if (
                    ((current_ekran.current_level == EKRAN_LEVEL_SET_NEW_PASSWORD1) && (current_settings.password1 != 0)) ||
                    (current_ekran.current_level == EKRAN_LEVEL_SET_NEW_PASSWORD2) ||
                    (current_ekran.current_level == EKRAN_LEVEL_SET_NEW_PASSWORD3))
                  {
                    //У випавдку, якщо ми намагаємося перейти у вікно зміни паролю, то спочатку треба запитати старий пароль доступу, якщо він встановлений
                    if (current_ekran.current_level == EKRAN_LEVEL_SET_NEW_PASSWORD1)
                    {
                      current_ekran.current_level = EKRAN_LEVEL_PASSWORD;
                      previous_level_in_current_level_menu[current_ekran.current_level] = EKRAN_LEVEL_SET_NEW_PASSWORD1;
                    }
                    else if (current_ekran.current_level == EKRAN_LEVEL_SET_NEW_PASSWORD2)
                    {
                      current_ekran.current_level = EKRAN_LEVEL_PASSWORD_HARD;
                      previous_level_in_current_level_menu[current_ekran.current_level] = EKRAN_LEVEL_SET_NEW_PASSWORD2;
                    }
                    else if (current_ekran.current_level == EKRAN_LEVEL_SET_NEW_PASSWORD3)
                    {
                      current_ekran.current_level = EKRAN_LEVEL_PASSWORD_REPROGRAM;
                      previous_level_in_current_level_menu[current_ekran.current_level] = EKRAN_LEVEL_SET_NEW_PASSWORD3;
                    }
                    else
                    {
                      //Теоретично цього ніколи не мало б бути
                      total_error_sw_fixed();
                    }
                    current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                    current_ekran.cursor_on = 1;
                    current_ekran.cursor_blinking_on = 1;
                  }
                }

                //Очистити сигналізацію, що натиснута кнопка
                new_state_keyboard &= ~(1u << BIT_KEY_ENTER);
                //Виставляємо команду на обновлекння нового екрану
                new_state_keyboard |= (1u << BIT_REWRITE);
              }
              else if (new_state_keyboard == (1u << BIT_KEY_ESC))
              {
                //Переходимо у попереднє меню
                current_ekran.current_level = previous_level_in_current_level_menu[current_ekran.current_level];
                current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                current_ekran.edition = 0;

                //Виставляємо команду на обновлекння нового екрану
                new_state_keyboard |= (1u << BIT_REWRITE);
                //Очистити сигналізацію, що натиснута кнопка
                new_state_keyboard &= ~(1u << BIT_KEY_ESC);
              }
              else if (new_state_keyboard == (1u << BIT_KEY_UP))
              {
                //Натиснута кнопка UP
                if (current_ekran.current_level == EKRAN_MEASURMENT)
                {
                  current_ekran.index_position--;
                  do
                  {
                    if (current_ekran.index_position < 0)
                      current_ekran.index_position = MAX_ROW_FOR_MEASURMENT - 1;

                    if (
                      (current_ekran.index_position == INDEX_ML_MEASURMENT_SELSYN) &&
                      ((current_settings.type_control_location != 2)))
                      current_ekran.index_position--;

                    if (
                      (current_ekran.index_position == INDEX_ML_MEASURMENT_LOGOMETR) &&
                      ((current_settings.type_control_location != 1)))
                      current_ekran.index_position--;

                    if (
                      (current_ekran.index_position == INDEX_ML_MEASURMENT_2) &&
                      ((current_settings.control_rpn & MASKA_FOR_BIT(INDEX_ML_CTRRPN_TRANSF)) == 0))
                      current_ekran.index_position--;
                  } while (current_ekran.index_position < 0);

                  position_in_current_level_menu[EKRAN_MEASURMENT] = current_ekran.index_position;
                  //Формуємо екран списку вимірювання
                  make_ekran_measurement();
                }
                else if (current_ekran.current_level == EKRAN_MEASURMENT_SELSYN)
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_FOR_MEASURMENT_FOR_SELSYN - 1;
                  position_in_current_level_menu[EKRAN_MEASURMENT_SELSYN] = current_ekran.index_position;
                  //Формуємо екран списку вимірювання для сельсина
                  make_ekran_measuremet_for_selsyn();
                }
                else if (
                  (current_ekran.current_level == EKRAN_MEASURMENT_1) ||
                  (current_ekran.current_level == EKRAN_MEASURMENT_2))
                {
                  current_ekran.index_position = current_ekran.index_position - MAX_ROW_LCD;
                  if (current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_FOR_MEASURMENT_1_AND_2 - 1;
                  current_ekran.index_position = (current_ekran.index_position >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;
                  position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;
                  //Формуємо екран вимірювання струмів і напруг ТН1 або ТН2
                  make_ekran_current_voltage((current_ekran.current_level == EKRAN_MEASURMENT_1) ? 0 : 1, pervynna_vtorynna);
                }
                else if (current_ekran.current_level == EKRAN_VOLTAGE_SELSYN)
                {
                  current_ekran.index_position = current_ekran.index_position - MAX_ROW_LCD;
                  if (current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_FOR_MEASURMENT_VOLTAGE_SELSYN - 1;
                  current_ekran.index_position = (current_ekran.index_position >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;
                  position_in_current_level_menu[EKRAN_VOLTAGE_SELSYN] = current_ekran.index_position;
                  //Формуємо екран вимірювання напруг для сельсина
                  make_ekran_voltage_for_selsyn();
                }
                else if (current_ekran.current_level == EKRAN_ANGLE_SELSYN)
                {
                  current_ekran.index_position = current_ekran.index_position - MAX_ROW_LCD;
                  if (current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_FOR_MEASURMENT_ANGLE_SELSYN - 1;
                  current_ekran.index_position = (current_ekran.index_position >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;
                  position_in_current_level_menu[EKRAN_ANGLE_SELSYN] = current_ekran.index_position;
                  //Формуємо екран вимірювання кутів для сельсина
                  make_ekran_angle_for_selsyn();
                }
                else if (current_ekran.current_level == EKRAN_LOGOMETR)
                {
                  current_ekran.index_position = current_ekran.index_position - MAX_ROW_LCD;
                  if (current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_FOR_VOLTAGE_LOGOMETR - 1;
                  current_ekran.index_position = (current_ekran.index_position >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;
                  position_in_current_level_menu[EKRAN_LOGOMETR] = current_ekran.index_position;
                  //Формуємо екран вимірювання напруг логометра
                  make_ekran_logomentr_voltage();
                }
                else if (current_ekran.current_level == EKRAN_MEASURMENT_FREQUENCY)
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_FOR_MEASURMENT_FREQUENCY - 1;
                  position_in_current_level_menu[EKRAN_MEASURMENT_FREQUENCY] = current_ekran.index_position;
                  //Формуємо екран вимірювання частот вікна
                  make_ekran_frequency();
                }
                else if (
                  (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_RPN) ||
                  (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_BRP) ||
                  (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_ZNKh) ||
                  (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_UMIN) ||
                  (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_UMAX) ||
                  (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_UP))
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_FOR_CHOSE_SETTINGS_PROTECTION_WITH_TWO_GROUP - 1;
                  position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;
                  //Формуємо екран група1-...-групаN-настройки для всіх захистів де використовуються групи уставок
                  make_ekran_chose_settings_protection_with_groups();
                }
                else if (
                  (
                    (current_ekran.current_level >= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_RPN) &&
                    (current_ekran.current_level <= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP4_RPN)) ||
                  ((current_ekran.current_level >= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_UMIN) &&
                   (current_ekran.current_level <= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP4_UMIN)) ||
                  ((current_ekran.current_level >= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_UMAX) &&
                   (current_ekran.current_level <= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP4_UMAX)) ||
                  ((current_ekran.current_level >= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_UP) &&
                   (current_ekran.current_level <= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP4_UP)))
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_FOR_CHOSE_SETPOINTS_TIMEOUTS - 1;

                  position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;

                  //Формуємо екран уставки-витримки для захистів у яких є дві групи уставок
                  make_ekran_chose_setpoint_and_timeout();
                }
                else if ((current_ekran.current_level >= EKRAN_CHOOSE_SETPOINT_GROUP1_BRP) &&
                         (current_ekran.current_level <= EKRAN_CHOOSE_SETPOINT_GROUP4_BRP))
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_FOR_CHOSE_SETTINGS_PROTECTION_WITH_SETPOINTS - 1;

                  position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;

                  //Формуємо екран управління для захистів у яких є багато груп уставок
                  make_ekran_chose_setpoint();
                }
                else if (
                  (current_ekran.current_level >= EKRAN_CHOOSE_TIMEOUT_GROUP1_ZNKh) &&
                  (current_ekran.current_level <= EKRAN_CHOOSE_TIMEOUT_GROUP4_ZNKh))
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_FOR_CHOSE_SETTINGS_PROTECTION_WITH_TIMEOUT - 1;

                  position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;

                  //Формуємо екран витримки для захистів у яких є багато груп уставок
                  make_ekran_chose_timeout();
                }
                else if (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_ZSKH)
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_FOR_CHOSE_SETTINGS_PROTECTION_WITH_CONTROL - 1;
                  position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;

                  //Формуємо екран відображення заголовків настроювання де тільки є управління
                  make_ekran_chose_control();
                }
                else if (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_SWITCHER)
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_FOR_CHOSE_SETTINGS_SWITCHER - 1;

                  position_in_current_level_menu[EKRAN_CHOOSE_SETTINGS_SWITCHER] = current_ekran.index_position;

                  //Формуємо екран уставки-витримки-настройки-ранжування вимикача
                  make_ekran_chose_settings_switcher();
                }
                else if (current_ekran.current_level == EKRAN_CHOOSE_RANG_SWITCH)
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_FOR_RANG_SWITCH - 1;

                  position_in_current_level_menu[EKRAN_CHOOSE_RANG_SWITCH] = current_ekran.index_position;

                  //Формуємо екран БВимк./БУвімк.
                  make_ekran_choose_CBOn_CBOff();
                }
                else if (current_ekran.current_level == EKRAN_CHOSE_SETTINGS)
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_FOR_CHOSE_SETTINGS - 1;
                  while (
                    ((current_settings.configuration & (1 << EL_BIT_CONFIGURATION)) == 0) &&
                    (current_ekran.index_position == INDEX_OF_EXTENDED_LOGIC))
                  {
                    if (--current_ekran.index_position < 0)
                      current_ekran.index_position = MAX_ROW_FOR_CHOSE_SETTINGS - 1;
                  }

                  position_in_current_level_menu[EKRAN_CHOSE_SETTINGS] = current_ekran.index_position;
                  //Формуємо екран заголовків настроювання
                  make_ekran_chose_settings();
                }
                else if (current_ekran.current_level == EKRAN_LEVEL_CHOOSE_PASSWORDS)
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_FOR_CHOOSE_PASSWORDS - 1;
                  position_in_current_level_menu[EKRAN_LEVEL_CHOOSE_PASSWORDS] = current_ekran.index_position;
                  //Формуємо екран заголовків паролів
                  make_ekran_chose_passwords();
                }
                else if (current_ekran.current_level == EKRAN_LIST_INPUTS_FOR_RANGUVANNJA)
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_LIST_INPUTS_FOR_RANGUVANNJA - 1;
                  position_in_current_level_menu[EKRAN_LIST_INPUTS_FOR_RANGUVANNJA] = current_ekran.index_position;
                  //Формуємо екран заголовків дискретних входів для ранжування
                  make_ekran_chose_of_list_for_ranguvannja(ID_INPUT);
                }
                else if (current_ekran.current_level == EKRAN_LIST_OUTPUTS_FOR_RANGUVANNJA)
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_LIST_OUTPUTS_FOR_RANGUVANNJA - 1;
                  position_in_current_level_menu[EKRAN_LIST_OUTPUTS_FOR_RANGUVANNJA] = current_ekran.index_position;
                  //Формуємо екран заголовків дискретних виходів для ранжування
                  make_ekran_chose_of_list_for_ranguvannja(ID_OUTPUT);
                }
                else if (current_ekran.current_level == EKRAN_LIST_LEDS_FOR_RANGUVANNJA)
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_LIST_LEDS_FOR_RANGUVANNJA - 1;
                  position_in_current_level_menu[EKRAN_LIST_LEDS_FOR_RANGUVANNJA] = current_ekran.index_position;
                  //Формуємо екран заголовків світоіндикаторів для ранжування
                  make_ekran_chose_of_list_for_ranguvannja(ID_LED);
                }
                else if (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_UVV)
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_FOR_CHOSE_SETTINGS_UVV - 1;
                  position_in_current_level_menu[EKRAN_CHOOSE_SETTINGS_UVV] = current_ekran.index_position;
                  //Формуємо екран вибору настройок УВВ
                  make_ekran_chose_settings_uvv();
                }
                else if (current_ekran.current_level == EKRAN_CHOSE_COMMUNICATION_PARAMETERS)
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_FOR_CHOSE_COMMUNICATION_PARAMETERS - 1;
                  position_in_current_level_menu[EKRAN_CHOSE_COMMUNICATION_PARAMETERS] = current_ekran.index_position;
                  //Формуємо екран вибору настройок парамеирів комунікації
                  make_ekran_chose_communication_parameters();
                }
                else if (current_ekran.current_level == EKRAN_VIEW_NAME_OF_CELL)
                {
                  current_ekran.index_position -= MAX_ROW_LCD;
                  if (current_ekran.index_position < 0)
                    current_ekran.index_position = (MAX_ROW_FOR_VIEW_NAME_OF_CELL - 1) & (~(MAX_ROW_LCD - 1));
                  position_in_current_level_menu[EKRAN_VIEW_NAME_OF_CELL] = current_ekran.index_position;
                  //Формуємо екран відображення імені ячейки
                  make_ekran_name_of_cell();
                }
                else if (current_ekran.current_level == EKRAN_CHOSE_SETTING_RS485)
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_FOR_CHOSE_SETTING_RS485 - 1;
                  position_in_current_level_menu[EKRAN_CHOSE_SETTING_RS485] = current_ekran.index_position;
                  //Формуємо екран відображення списку настройок для інтерфейсу RS-485
                  make_ekran_chose_setting_rs485();
                }
                else if (current_ekran.current_level == EKRAN_PHY_LAYER_RS485)
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_FOR_PHY_LAYER_RS485 - 1;
                  position_in_current_level_menu[EKRAN_PHY_LAYER_RS485] = current_ekran.index_position;
                  //Формуємо екран відображення списку настройок фізичного рівня для інтерфейсу RS-485
                  make_ekran_phy_layer_rs485();
                }
                else if (current_ekran.current_level == EKRAN_PROTOCOLS_RS485)
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_FOR_PROTOCOLS_RS485 - 1;
                  position_in_current_level_menu[EKRAN_PROTOCOLS_RS485] = current_ekran.index_position;
                  //Формуємо екран відображення списку налаштувань протоколу RS-485
                  make_ekran_protocols_rs485();
                }
                else if (current_ekran.current_level == EKRAN_CHOSE_DATA_TIME)
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_FOR_CHOSE_DATA_TIME_SETTINGS - 1;
                  if ((current_settings.dst & MASKA_FOR_BIT(N_BIT_TZ_DST)) == 0)
                  {
                    while (
                      (current_ekran.index_position == INDEX_ML_CHDT_DST_ON) ||
                      (current_ekran.index_position == INDEX_ML_CHDT_DST_OFF))
                    {
                      if (--current_ekran.index_position < 0)
                        current_ekran.index_position = MAX_ROW_FOR_CHOSE_DATA_TIME_SETTINGS - 1;
                    }
                  }

                  position_in_current_level_menu[EKRAN_CHOSE_DATA_TIME] = current_ekran.index_position;
                  //Формуємо екран вибору налаштувань дати і часу
                  make_ekran_chose_data_time_settings();
                }
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                else if (current_ekran.current_level == EKRAN_LIST_TYPE_IEC61850_NODES)
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_FOR_TYPE_IEC61850_NODES - 1;
                  position_in_current_level_menu[EKRAN_LIST_TYPE_IEC61850_NODES] = current_ekran.index_position;

                  //Формуємо екран заголовків для типів логічних вузлів для IEC 61850
                  make_ekran_type_IEC61850_nodes();
                }
                else if (current_ekran.current_level == EKRAN_LIST_IN_GOOSE)
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = N_IN_GOOSE - 1;
                  position_in_current_level_menu[EKRAN_LIST_IN_GOOSE] = current_ekran.index_position;

                  //Формуємо екран заголовків вхідних GOOSE блоків
                  make_ekran_chose_of_list_for_ranguvannja(ID_IN_GOOSE);
                }
                else if (current_ekran.current_level == EKRAN_LIST_IN_MMS)
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = N_IN_MMS - 1;
                  position_in_current_level_menu[EKRAN_LIST_IN_MMS] = current_ekran.index_position;

                  //Формуємо екран заголовків вхідних MMS блоків
                  make_ekran_chose_of_list_for_ranguvannja(ID_IN_MMS);
                }
                else if (current_ekran.current_level == EKRAN_LIST_OUT_LAN)
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = N_OUT_LAN - 1;
                  position_in_current_level_menu[EKRAN_LIST_OUT_LAN] = current_ekran.index_position;

                  //Формуємо екран заголовків вихідний мережевий блок
                  make_ekran_chose_of_list_for_ranguvannja(ID_OUT_LAN);
                }
                else if (
                  ((current_ekran.current_level >= EKRAN_IN_GOOSE1) && (current_ekran.current_level <= EKRAN_IN_GOOSE16)) ||
                  ((current_ekran.current_level >= EKRAN_IN_MMS1) && (current_ekran.current_level <= EKRAN_IN_MMS4)))
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = N_IN_GOOSE_MMS_OUT - 1;
                  position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;
                  //Формуємо екран заголовків виходів
                  make_ekran_list_in_out_for_iec61850(0, N_IN_GOOSE_MMS_OUT);
                }
                else if ((current_ekran.current_level >= EKRAN_OUT_LAN1) && (current_ekran.current_level <= EKRAN_OUT_LAN4))
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = N_OUT_LAN_IN - 1;
                  position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;
                  //Формуємо екран заголовків виходів
                  make_ekran_list_in_out_for_iec61850(1, N_OUT_LAN_IN);
                }
                else if (current_ekran.current_level == EKRAN_CHOSE_SETTING_ETHERNET)
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_FOR_CHOSE_SETTING_ETHERNET - 1;
                  position_in_current_level_menu[EKRAN_CHOSE_SETTING_ETHERNET] = current_ekran.index_position;
                  //Формуємо екран відображення списку настройок для інтерфейсу Ethernet
                  make_ekran_chose_setting_Ethernet();
                }
#endif
                else if (current_ekran.current_level == EKRAN_VIEW_LIST_OF_REGISTRATORS)
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_FOR_LIST_OF_REGISTRATORS - 1;
                  position_in_current_level_menu[EKRAN_VIEW_LIST_OF_REGISTRATORS] = current_ekran.index_position;
                  //Формуємо екран вибору реєстратора
                  make_ekran_chose_registrators();
                }
                else if (current_ekran.current_level == EKRAN_VIEW_SETTINGS_OF_DIGITAL_REGISTRATORS)
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_FOR_SETTINGS_OF_DIGITAL_REGISTRATOR - 1;
                  position_in_current_level_menu[EKRAN_VIEW_SETTINGS_OF_DIGITAL_REGISTRATORS] = current_ekran.index_position;
                  //Формуємо екран вибору настройок аналогового реєстратора
                  make_ekran_settings_digital_registrators();
                }
                else if (current_ekran.current_level == EKRAN_VIEW_SETTINGS_OF_ANALOG_REGISTRATORS)
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_FOR_SETTINGS_OF_ANALOG_REGISTRATOR - 1;
                  position_in_current_level_menu[EKRAN_VIEW_SETTINGS_OF_ANALOG_REGISTRATORS] = current_ekran.index_position;
                  //Формуємо екран вибору настройок аналогового реєстратора
                  make_ekran_settings_analog_registrators();
                }
                else if (current_ekran.current_level == EKRAN_EXTENDED_LIGIC)
                {
                  current_ekran.index_position--;
                  do
                  {
                    if (current_ekran.index_position < 0)
                      current_ekran.index_position = MAX_ROW_FOR_EXTENDED_LIGIC - 1;

                    //                  if ((current_ekran.index_position == INDEX_OF_DEFINED_NOT) && (current_settings.number_defined_not  == 0))
                    //                    current_ekran.index_position--;
                    //                  if ((current_ekran.index_position == INDEX_OF_DEFINED_XOR) && (current_settings.number_defined_xor  == 0))
                    //                    current_ekran.index_position--;
                    //                  if ((current_ekran.index_position == INDEX_OF_DEFINED_OR) && (current_settings.number_defined_or  == 0))
                    //                    current_ekran.index_position--;
                    //                  if ((current_ekran.index_position == INDEX_OF_DEFINED_AND) && (current_settings.number_defined_and  == 0))
                    //                    current_ekran.index_position--;
                    //                  if ((current_ekran.index_position == INDEX_OF_DEFINED_TRIGGERS) && (current_settings.number_defined_dt  == 0))
                    //                    current_ekran.index_position--;
                    //                  if ((current_ekran.index_position == INDEX_OF_DEFINED_FUNCTIONS) && (current_settings.number_defined_df  == 0))
                    //                    current_ekran.index_position--;
                  } while (current_ekran.index_position < 0);

                  position_in_current_level_menu[EKRAN_EXTENDED_LIGIC] = current_ekran.index_position;
                  //Формуємо екран вибору налаштувань розширеної логіки
                  make_ekran_extended_logic();
                }
                else if (current_ekran.current_level == EKRAN_LIST_SETTINGS_FOR_DF)
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_SETTINGS_DF - 1;
                  position_in_current_level_menu[EKRAN_LIST_SETTINGS_FOR_DF] = current_ekran.index_position;
                  //Формуємо екран відображення списку настройок для опреділювальних функцій
                  make_ekran_chose_settings_df();
                }
                else if ((current_ekran.current_level == EKRAN_LIST_DF_FOR_RANGUVANNJA) || (current_ekran.current_level == EKRAN_LIST_DF_FOR_TIMEOUT_SETTINGS))
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_FOR_LIST_DF - 1;
                  position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;
                  //Формуємо екран заголовків опреділювальних функцій
                  make_ekran_chose_of_list_for_ranguvannja(ID_DF);
                }
                else if ((current_ekran.current_level >= EKRAN_LIST_TYPE_SOURCE_DF1) && (current_ekran.current_level <= (EKRAN_LIST_TYPE_SOURCE_DF1 + NUMBER_DEFINED_FUNCTIONS - 1)))
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_LIST_TYPE_SOURCE_DF - 1;
                  position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;
                  //Формуємо екран заголовків типів джерел опреділювальних функцій
                  make_ekran_list_type_source_df();
                }
                else if (current_ekran.current_level == EKRAN_LIST_DT)
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_FOR_LIST_DT - 1;
                  position_in_current_level_menu[EKRAN_LIST_DT] = current_ekran.index_position;
                  //Формуємо екран заголовків опреділювальних триґерів
                  make_ekran_chose_of_list_for_ranguvannja(ID_DT);
                }
                else if ((current_ekran.current_level >= EKRAN_SET_RESET_DT1) && (current_ekran.current_level <= (EKRAN_SET_RESET_DT1 + NUMBER_DEFINED_TRIGGERS - 1)))
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_FOR_LIST_SET_RESET_DT - 1;
                  position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;
                  //Формуємо екран заголовків активація-деактивація опреділювальних триґерів
                  make_ekran_set_reset_trigger();
                }
                else if ((current_ekran.current_level >= EKRAN_SET_DT1) && (current_ekran.current_level <= (EKRAN_SET_DT1 + 2 * NUMBER_DEFINED_TRIGGERS - 1)))
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_LIST_TYPE_SOURCE_DT - 1;
                  position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;
                  //Формуємо екран заголовків типів джерел опреділювальних триґерів
                  make_ekran_list_type_source_dt();
                }
                else if (current_ekran.current_level == EKRAN_LIST_D_AND)
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_FOR_LIST_D_AND - 1;
                  position_in_current_level_menu[EKRAN_LIST_D_AND] = current_ekran.index_position;
                  //Формуємо екран заголовків визначуваних "І"
                  make_ekran_chose_of_list_for_ranguvannja(ID_AND);
                }
                else if (current_ekran.current_level == EKRAN_LIST_D_OR)
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_FOR_LIST_D_OR - 1;
                  position_in_current_level_menu[EKRAN_LIST_D_OR] = current_ekran.index_position;
                  //Формуємо екран заголовків визначуваних "АБО"
                  make_ekran_chose_of_list_for_ranguvannja(ID_OR);
                }
                else if (current_ekran.current_level == EKRAN_LIST_D_XOR)
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_FOR_LIST_D_XOR - 1;
                  position_in_current_level_menu[EKRAN_LIST_D_XOR] = current_ekran.index_position;
                  //Формуємо екран заголовків визначуваних "Викл.АБО"
                  make_ekran_chose_of_list_for_ranguvannja(ID_XOR);
                }
                else if (current_ekran.current_level == EKRAN_LIST_D_NOT)
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_FOR_LIST_D_NOT - 1;
                  position_in_current_level_menu[EKRAN_LIST_D_NOT] = current_ekran.index_position;
                  //Формуємо екран заголовків визначуваних "НЕ"
                  make_ekran_chose_of_list_for_ranguvannja(ID_NOT);
                }
                else if (current_ekran.current_level == EKRAN_LIST_TF_FOR_RANGUVANNJA)
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_FOR_LIST_TF - 1;
                  position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;
                  //Формуємо екран заголовків передавальних функцій
                  make_ekran_chose_of_list_for_ranguvannja(ID_TF);
                }
                else if (current_ekran.current_level == EKRAN_LIST_BUTTONS_FOR_RANGUVANNJA)
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_LIST_BUTTONS_FOR_RANGUVANNJA - 1;
                  position_in_current_level_menu[EKRAN_LIST_BUTTONS_FOR_RANGUVANNJA] = current_ekran.index_position;
                  //Формуємо екран заголовків функціональних кнопок для ранжування
                  make_ekran_chose_of_list_for_ranguvannja(ID_DB);
                }
                else if (current_ekran.current_level == EKRAN_INFO)
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_FOR_INFO - 1;
                  position_in_current_level_menu[EKRAN_INFO] = current_ekran.index_position;
                  //Формуємо екран інформації
                  make_ekran_info();
                }
                else if (current_ekran.current_level == EKRAN_DATE_TIME_PZ)
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_FOR_DATE_TIME_PZ - 1;
                  position_in_current_level_menu[EKRAN_DATE_TIME_PZ] = current_ekran.index_position;
                  //Формуємо екран відображення міти дати і часу прошивки
                  make_ekran_date_time_pz();
                }
                else if (current_ekran.current_level == EKRAN_POINT_TIME_SETTINGS)
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_FOR_POINT_TIME_SETTINGS - 1;
                  position_in_current_level_menu[EKRAN_POINT_TIME_SETTINGS] = current_ekran.index_position;
                  //Формуємо екран вибору міток настроювання
                  make_ekran_chose_item_of_point_time_settings();
                }
                else if (current_ekran.current_level == EKRAN_POINT_TIME_SETPOINT)
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_FOR_POINT_TIME_SETPOINT_RANGUVANNJA - 1;
                  position_in_current_level_menu[EKRAN_POINT_TIME_SETPOINT] = current_ekran.index_position;
                  //Формуємо екран відображення міти останніх змін у уставках-витримках-управлінчькій інформації
                  make_ekran_time_settings(0);
                }
                else if (current_ekran.current_level == EKRAN_POINT_TIME_RANGUVANNJA)
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_FOR_POINT_TIME_SETPOINT_RANGUVANNJA - 1;
                  position_in_current_level_menu[EKRAN_POINT_TIME_RANGUVANNJA] = current_ekran.index_position;
                  //Формуємо екран відображення міти останніх змін у ранжування
                  make_ekran_time_settings(1);
                }
                else if (current_ekran.current_level == EKRAN_DIAGNOSTYKA)
                {
                  unsigned int diagnostyka_tmp[N_DIAGN];
                  for (size_t i = 0; i < N_DIAGN; i++)
                    diagnostyka_tmp[i] = diagnostyka[i];

                  unsigned int not_null = false;
                  for (size_t i = 0; i < N_DIAGN; i++)
                  {
                    not_null |= (diagnostyka_tmp[i] != 0);
                    if (not_null)
                      break;
                  }

                  if (not_null)
                  {
                    if (--current_ekran.index_position < 0)
                      current_ekran.index_position = MAX_ROW_FOR_DIAGNOSTYKA - 1;
                    while (_CHECK_SET_BIT(diagnostyka_tmp, current_ekran.index_position) == 0)
                    {
                      current_ekran.index_position--;
                      if (current_ekran.index_position < 0)
                        current_ekran.index_position = MAX_ROW_FOR_DIAGNOSTYKA - 1;
                    }
                  }
                  else
                  {
                    current_ekran.index_position = 0;
                  }

                  position_in_current_level_menu[EKRAN_DIAGNOSTYKA] = current_ekran.index_position;
                  //Формуємо екран діагностики
                  make_ekran_diagnostyka(diagnostyka_tmp);
                }
                else if (current_ekran.current_level == EKRAN_LIST_INPUTS_OUTPUTS)
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_FOR_LIST_INPUTS_OUTPUTS - 1;
                  position_in_current_level_menu[EKRAN_LIST_INPUTS_OUTPUTS] = current_ekran.index_position;
                  //Формуємо екран вибору станів входів-виходів
                  make_ekran_list_inputs_outputs();
                }
                else if ((current_ekran.current_level == EKRAN_STATE_INPUTS) || (current_ekran.current_level == EKRAN_STATE_OUTPUTS))
                {
                  if (current_ekran.current_level == EKRAN_STATE_INPUTS)
                  {
                    if (--current_ekran.index_position < 0)
                      current_ekran.index_position = NUMBER_INPUTS - 1;
                  }
                  else
                  {
                    if (--current_ekran.index_position < 0)
                      current_ekran.index_position = NUMBER_SIMPLE_OUTPUTS
#ifdef NUMBER_DS
                                                     + ((current_settings.configuration & (1 << DS_BIT_CONFIGURATION)) != 0) * NUMBER_DS
#endif
                                                     - 1;
                  }

                  position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;
                  //Формуємо екран станів входів або виходыв
                  make_ekran_state_inputs_or_outputs(current_ekran.current_level - EKRAN_STATE_INPUTS);
                }
                else if (current_ekran.current_level == EKRAN_LIST_REGISTRATORS)
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_FOR_LIST_REGISTRATORS - 1;
                  position_in_current_level_menu[EKRAN_LIST_REGISTRATORS] = current_ekran.index_position;
                  //Формуємо екран вибору реєстраторів
                  make_ekran_list_registrators();
                }
                else if (
                  (current_ekran.current_level == EKRAN_LIST_ANALOG_REGISTRATOR_RECORDS) ||
                  (current_ekran.current_level == EKRAN_LIST_DIGITAL_REGISTRATOR_RECORDS) ||
                  (current_ekran.current_level == EKRAN_LIST_REGISTRATOR_PROGRAM_ERROR_RECORDS)
                  //||(current_ekran.current_level == EKRAN_LIST_STATE_CMD_REGISTRATOR_RECORDS)
                )
                {
                  unsigned int number_records = 0;
                  unsigned int type_registrator = 0;

                  if (current_ekran.current_level == EKRAN_LIST_ANALOG_REGISTRATOR_RECORDS)
                  {
                    unsigned int first_number = (info_rejestrator_ar.first_number < 0) ? 0 : (info_rejestrator_ar.first_number + 1);
                    unsigned int last_number = (info_rejestrator_ar.last_number < 0) ? 0 : (info_rejestrator_ar.last_number + 1);

                    if (first_number == 0)
                      number_records = 0;
                    else if (first_number >= last_number)
                      number_records = first_number - last_number + 1;
                    else
                      number_records = NUMBER_FATFS_NAME - last_number + first_number + 1;

                    type_registrator = INDEX_ML_ANALOG_REGISTRATOR_INFO;
                  }
                  else if (current_ekran.current_level == EKRAN_LIST_DIGITAL_REGISTRATOR_RECORDS)
                  {
                    number_records = info_rejestrator_dr.number_records;
                    type_registrator = INDEX_ML_DIGITAL_REGISTRATOR_INFO;
                  }
                  else if (current_ekran.current_level == EKRAN_LIST_REGISTRATOR_PROGRAM_ERROR_RECORDS)
                  {
                    number_records = info_rejestrator_pr_err.number_records;
                    type_registrator = INDEX_ML_PROGRAM_ERROE_REGISTRATOR_INFO;
                  }
                  //?else
                  //?{
                  //?  number_records = holderCmdPlusTime.shTotalFixElem;
                  //?  type_registrator = INDEX_ML_STATE_CMD_REGISTRATOR_INFO;
                  //?}
                  else
                  {
                    total_error_sw_fixed();
                  }

                  --current_ekran.index_position;
                  if ((current_ekran.index_position < 0) || (current_ekran.index_position >= ((int) number_records)))
                  {
                    if (number_records == 0)
                      current_ekran.index_position = 0;
                    else
                      current_ekran.index_position = number_records - 1;
                  }
                  position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;

                  //Формуємо екран відображення записів
                  if (type_registrator == INDEX_ML_ANALOG_REGISTRATOR_INFO)
                    _SET_STATE(FATFS_command, FATFS_READ_DATA_FOR_MENU);
                  else
                    make_ekran_list_records_registrator(type_registrator);
                }
                else if (current_ekran.current_level == EKRAN_TITLES_DIGITAL_REGISTRATOR)
                {
                  do
                  {
                    if (--current_ekran.index_position < 0)
                      current_ekran.index_position = MAX_ROW_FOR_TITLES_DIGITAL_REGISTRATOR - 1;
                  } while (
                    ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_DR_MENU) == 0) &&
                    (current_ekran.index_position > INDEX_ML_TITLE_DR_CHANGES_SIGNALS) &&
                    (((current_ekran.index_position == INDEX_ML_TITLE_DR_MIN_U) && (buffer_for_manu_read_record[FIRST_INDEX_NUMBER_MIN_U_DR] == 0)) ||
                     ((current_ekran.index_position == INDEX_ML_TITLE_DR_MAX_U) && (buffer_for_manu_read_record[FIRST_INDEX_NUMBER_MAX_U_DR] == 0))));
                  position_in_current_level_menu[EKRAN_TITLES_DIGITAL_REGISTRATOR] = current_ekran.index_position;
                  //Формуємо екран відображення заголовків груп для дискретного реєстратора
                  make_ekran_list_titles_for_record_of_digital_registrator();
                }
                else if (current_ekran.current_level == EKRAN_CHANGES_SIGNALS_DR)
                {
                  current_ekran.index_position -= (MAX_ROW_LCD >> 1);
                  //Формуємо екран відображення змін сигналів - записаних у дискретному реєстраторі
                  make_ekran_changing_signals_digital_registrator();
                }
                else if (current_ekran.current_level == EKRAN_TITLE_MAX_VALUES)
                {
                  unsigned char *point_unsigned_char = (unsigned char *) (buffer_for_manu_read_record + index_cell_into_array_for_integral_values_dr);
                  unsigned int *point_unsigned_int = (unsigned int *) point_unsigned_char;
                  unsigned int control_for_dr = *(point_unsigned_int + 5);

                  current_ekran.index_position--;
                  do
                  {
                    if (current_ekran.index_position < 0)
                      current_ekran.index_position = MAX_ROW_FOR_EKRAN_ANALOG_VALUES_DR - 1;

                    while (
                      (buffer_for_manu_read_record[FIRST_INDEX_START_START_RECORD_DR] == LABEL_START_RECORD_DR) && /*якщо ця умова не виконується, то у функції make_ekran_analog_value_records_digital_registrator ми перейдемо на відображення інформації про недоступні дані, а не будемо виконувати фільтрацію*/
                      ((control_for_dr & (1 << 0)) == 0) &&                                                        /*моніторинг вівся, коли був вибраний 2-обмотковий трансформатор*/
                      (current_ekran.index_position >= INDEX_ML_DR_U2) &&
                      (current_ekran.index_position <= INDEX_ML_DR_I2))
                      current_ekran.index_position--;

                  } while (current_ekran.index_position < 0);

                  //Формуємо екран відображення аналогових значень з запису дискретного реєстратора
                  make_ekran_analog_value_records_digital_registrator();
                }
                else if (current_ekran.current_level == EKRAN_TITLES_STATE_CMD_REGISTRATOR)
                {
                  //?if(--current_ekran.index_position < 0) current_ekran.index_position = MAX_ROW_FOR_TITLES_PR_ERR_REGISTRATOR - 1;
                  //?position_in_current_level_menu[EKRAN_TITLES_STATE_CMD_REGISTRATOR] = current_ekran.index_position;
                  //?//Формуємо екран відображення заголовків груп для дискретного реєстратора
                  //?make_ekran_list_titles_for_record_of_state_cmd_registrator();
                  current_ekran.index_position--;
                  make_ekran_time_ms_c_val_c_name_sr_format_changing_signals();
                }
                else if (current_ekran.current_level == EKRAN_LIST_STATE_CMD_REGISTRATOR_RECORDS)
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = holderCmdPlusTime.shTotalFixElem - 1;
                  position_in_current_level_menu[EKRAN_LIST_STATE_CMD_REGISTRATOR_RECORDS] = current_ekran.index_position;
                  //Формуємо екран відображення дат і часу
                  make_ekran_data_elem_stt_registrator(); //?make_ekran_data_and_time_elem_stt_registrator(0);
                }
                else if (current_ekran.current_level == EKRAN_CHANGES_DIAGNOSTICS_PR_ERR)
                {
                  current_ekran.index_position--;
                  //Формуємо екран відображення змін діагностик - записаних у реєстраторі програмних подій
                  make_ekran_changing_diagnostics_pr_err_registrator();
                }
                //?else if (current_ekran.current_level == EKRAN_STATE_CMD_REG)
                //?{
                //?  current_ekran.index_position -= (MAX_ROW_LCD >> 1);
                //?  //Формуємо екран відображення змін сигналів - записаних у дискретному реєстраторі
                //?  make_ekran_changing_signals_statistica_registrator();
                //?}

                else if (current_ekran.current_level == EKRAN_DATA_LABEL_STATE_CMD)
                {
                  current_ekran.index_position -= (MAX_ROW_LCD);
                  //asm volatile(
                  //     "bkpt 1"
                  // );

                  //Формуємо екран відображення змін сигналів - записаних у  реєстраторі
                  make_ekran_signals_stable_one_in_data_elem_stt_reg();
                }
                //Очистити сигналізацію, що натиснута кнопка
                new_state_keyboard &= ~(1u << BIT_KEY_UP);
              }
              else if (new_state_keyboard == (1u << BIT_KEY_DOWN))
              {
                //Натиснута кнопка DOWN
                if (current_ekran.current_level == EKRAN_MEASURMENT)
                {
                  current_ekran.index_position++;
                  do
                  {
                    if (current_ekran.index_position >= MAX_ROW_FOR_MEASURMENT)
                      current_ekran.index_position = 0;

                    if (
                      (current_ekran.index_position == INDEX_ML_MEASURMENT_2) &&
                      ((current_settings.control_rpn & MASKA_FOR_BIT(INDEX_ML_CTRRPN_TRANSF)) == 0))
                      current_ekran.index_position++;

                    if (
                      (current_ekran.index_position == INDEX_ML_MEASURMENT_LOGOMETR) &&
                      ((current_settings.type_control_location != 1)))
                      current_ekran.index_position++;

                    if (
                      (current_ekran.index_position == INDEX_ML_MEASURMENT_SELSYN) &&
                      ((current_settings.type_control_location != 2)))
                      current_ekran.index_position++;
                  } while (current_ekran.index_position >= MAX_ROW_FOR_MEASURMENT);

                  position_in_current_level_menu[EKRAN_MEASURMENT] = current_ekran.index_position;
                  //Формуємо екран списку вимірювання
                  make_ekran_measurement();
                }
                else if (current_ekran.current_level == EKRAN_MEASURMENT_SELSYN)
                {
                  if (++current_ekran.index_position >= MAX_ROW_FOR_MEASURMENT_FOR_SELSYN)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[EKRAN_MEASURMENT_SELSYN] = current_ekran.index_position;
                  //Формуємо екран списку вимірювання для сельсина
                  make_ekran_measuremet_for_selsyn();
                }
                else if (
                  (current_ekran.current_level == EKRAN_MEASURMENT_1) ||
                  (current_ekran.current_level == EKRAN_MEASURMENT_2))
                {
                  current_ekran.index_position = current_ekran.index_position + MAX_ROW_LCD;
                  if (current_ekran.index_position >= MAX_ROW_FOR_MEASURMENT_1_AND_2)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;
                  current_ekran.index_position = (current_ekran.index_position >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;
                  //Формуємо екран вимірювання струмів і напруг ТН1 або ТН2
                  make_ekran_current_voltage((current_ekran.current_level == EKRAN_MEASURMENT_1) ? 0 : 1, pervynna_vtorynna);
                }
                else if (current_ekran.current_level == EKRAN_VOLTAGE_SELSYN)
                {
                  current_ekran.index_position = current_ekran.index_position + MAX_ROW_LCD;
                  if (current_ekran.index_position >= MAX_ROW_FOR_MEASURMENT_VOLTAGE_SELSYN)
                    current_ekran.index_position = 0;
                  current_ekran.index_position = (current_ekran.index_position >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;
                  position_in_current_level_menu[EKRAN_VOLTAGE_SELSYN] = current_ekran.index_position;
                  //Формуємо екран вимірювання напруг для сельсина
                  make_ekran_voltage_for_selsyn();
                }
                else if (current_ekran.current_level == EKRAN_ANGLE_SELSYN)
                {
                  current_ekran.index_position = current_ekran.index_position + MAX_ROW_LCD;
                  if (current_ekran.index_position >= MAX_ROW_FOR_MEASURMENT_ANGLE_SELSYN)
                    current_ekran.index_position = 0;
                  current_ekran.index_position = (current_ekran.index_position >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;
                  position_in_current_level_menu[EKRAN_ANGLE_SELSYN] = current_ekran.index_position;
                  //Формуємо екран вимірювання кутів для сельсина
                  make_ekran_angle_for_selsyn();
                }
                else if (current_ekran.current_level == EKRAN_LOGOMETR)
                {
                  current_ekran.index_position = current_ekran.index_position + MAX_ROW_LCD;
                  if (current_ekran.index_position >= MAX_ROW_FOR_VOLTAGE_LOGOMETR)
                    current_ekran.index_position = 0;
                  current_ekran.index_position = (current_ekran.index_position >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;
                  position_in_current_level_menu[EKRAN_LOGOMETR] = current_ekran.index_position;
                  //Формуємо екран вимірювання напруг логометра
                  make_ekran_logomentr_voltage();
                }
                else if (current_ekran.current_level == EKRAN_MEASURMENT_FREQUENCY)
                {
                  if (++current_ekran.index_position >= MAX_ROW_FOR_MEASURMENT_FREQUENCY)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[EKRAN_MEASURMENT_FREQUENCY] = current_ekran.index_position;
                  //Формуємо екран вимірювання частот вікна
                  make_ekran_frequency();
                }
                else if (
                  (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_RPN) ||
                  (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_BRP) ||
                  (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_ZNKh) ||
                  (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_UMIN) ||
                  (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_UMAX) ||
                  (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_UP))
                {
                  if (++current_ekran.index_position >= MAX_ROW_FOR_CHOSE_SETTINGS_PROTECTION_WITH_TWO_GROUP)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;

                  //Формуємо екран група1-...-групаN-настройки для всіх захистів де використовуються групи уставок
                  make_ekran_chose_settings_protection_with_groups();
                }
                else if (
                  (
                    (current_ekran.current_level >= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_RPN) &&
                    (current_ekran.current_level <= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP4_RPN)) ||
                  ((current_ekran.current_level >= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_UMIN) &&
                   (current_ekran.current_level <= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP4_UMIN)) ||
                  ((current_ekran.current_level >= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_UMAX) &&
                   (current_ekran.current_level <= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP4_UMAX)) ||
                  ((current_ekran.current_level >= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_UP) &&
                   (current_ekran.current_level <= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP4_UP)))
                {
                  if (++current_ekran.index_position >= MAX_ROW_FOR_CHOSE_SETPOINTS_TIMEOUTS)
                    current_ekran.index_position = 0;

                  position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;

                  //Формуємо екран уставки-витримки для захистів у яких є дві групи уставок
                  make_ekran_chose_setpoint_and_timeout();
                }
                else if ((current_ekran.current_level >= EKRAN_CHOOSE_SETPOINT_GROUP1_BRP) &&
                         (current_ekran.current_level <= EKRAN_CHOOSE_SETPOINT_GROUP4_BRP))
                {
                  if (++current_ekran.index_position >= MAX_ROW_FOR_CHOSE_SETTINGS_PROTECTION_WITH_SETPOINTS)
                    current_ekran.index_position = 0;

                  position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;

                  //Формуємо екран управління для захистів у яких є багато груп уставок
                  make_ekran_chose_setpoint();
                }
                else if (
                  (current_ekran.current_level >= EKRAN_CHOOSE_TIMEOUT_GROUP1_ZNKh) &&
                  (current_ekran.current_level <= EKRAN_CHOOSE_TIMEOUT_GROUP4_ZNKh))
                {
                  if (++current_ekran.index_position >= MAX_ROW_FOR_CHOSE_SETTINGS_PROTECTION_WITH_TIMEOUT)
                    current_ekran.index_position = 0;

                  position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;

                  //Формуємо екран витримки для захистів у яких є багато груп уставок
                  make_ekran_chose_timeout();
                }
                else if (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_ZSKH)
                {
                  if (++current_ekran.index_position >= MAX_ROW_FOR_CHOSE_SETTINGS_PROTECTION_WITH_CONTROL)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;

                  //Формуємо екран відображення заголовків настроювання де тільки є управління
                  make_ekran_chose_control();
                }
                else if (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_SWITCHER)
                {
                  if (++current_ekran.index_position >= MAX_ROW_FOR_CHOSE_SETTINGS_SWITCHER)
                    current_ekran.index_position = 0;

                  position_in_current_level_menu[EKRAN_CHOOSE_SETTINGS_SWITCHER] = current_ekran.index_position;

                  //Формуємо екран уставки-витримки-настройки-ранжування вимикача
                  make_ekran_chose_settings_switcher();
                }
                else if (current_ekran.current_level == EKRAN_CHOOSE_RANG_SWITCH)
                {
                  if (++current_ekran.index_position >= MAX_ROW_FOR_RANG_SWITCH)
                    current_ekran.index_position = 0;

                  position_in_current_level_menu[EKRAN_CHOOSE_RANG_SWITCH] = current_ekran.index_position;

                  //Формуємо екран БВимк./БУвімк.
                  make_ekran_choose_CBOn_CBOff();
                }
                else if (current_ekran.current_level == EKRAN_CHOSE_SETTINGS)
                {
                  //Натиснута кнопка DOWN
                  if (++current_ekran.index_position >= MAX_ROW_FOR_CHOSE_SETTINGS)
                    current_ekran.index_position = 0;
                  while (
                    ((current_settings.configuration & (1 << EL_BIT_CONFIGURATION)) == 0) &&
                    (current_ekran.index_position == INDEX_OF_EXTENDED_LOGIC))
                  {
                    if (++current_ekran.index_position >= MAX_ROW_FOR_CHOSE_SETTINGS)
                      current_ekran.index_position = 0;
                  }

                  position_in_current_level_menu[EKRAN_CHOSE_SETTINGS] = current_ekran.index_position;

                  //Формуємо екран заголовків настроювання
                  make_ekran_chose_settings();
                }
                else if (current_ekran.current_level == EKRAN_LEVEL_CHOOSE_PASSWORDS)
                {
                  //Натиснута кнопка DOWN
                  if (++current_ekran.index_position >= MAX_ROW_FOR_CHOOSE_PASSWORDS)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[EKRAN_LEVEL_CHOOSE_PASSWORDS] = current_ekran.index_position;

                  //Формуємо екран заголовків паролів
                  make_ekran_chose_passwords();
                }
                else if (current_ekran.current_level == EKRAN_LIST_INPUTS_FOR_RANGUVANNJA)
                {
                  //Натиснута кнопка DOWN
                  if (++current_ekran.index_position >= MAX_ROW_LIST_INPUTS_FOR_RANGUVANNJA)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[EKRAN_LIST_INPUTS_FOR_RANGUVANNJA] = current_ekran.index_position;
                  //Формуємо екран заголовків дискретних входів для ранжування
                  make_ekran_chose_of_list_for_ranguvannja(ID_INPUT);
                }
                else if (current_ekran.current_level == EKRAN_LIST_OUTPUTS_FOR_RANGUVANNJA)
                {
                  //Натиснута кнопка DOWN
                  if (++current_ekran.index_position >= MAX_ROW_LIST_OUTPUTS_FOR_RANGUVANNJA)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[EKRAN_LIST_OUTPUTS_FOR_RANGUVANNJA] = current_ekran.index_position;
                  //Формуємо екран заголовків дискретних виходів для ранжування
                  make_ekran_chose_of_list_for_ranguvannja(ID_OUTPUT);
                }
                else if (current_ekran.current_level == EKRAN_LIST_LEDS_FOR_RANGUVANNJA)
                {
                  //Натиснута кнопка DOWN
                  if (++current_ekran.index_position >= MAX_ROW_LIST_LEDS_FOR_RANGUVANNJA)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[EKRAN_LIST_LEDS_FOR_RANGUVANNJA] = current_ekran.index_position;
                  //Формуємо екран заголовків світоіндикаторів для ранжування
                  make_ekran_chose_of_list_for_ranguvannja(ID_LED);
                }
                else if (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_UVV)
                {
                  if (++current_ekran.index_position >= MAX_ROW_FOR_CHOSE_SETTINGS_UVV)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[EKRAN_CHOOSE_SETTINGS_UVV] = current_ekran.index_position;
                  //Формуємо екран вибору настройок УВВ
                  make_ekran_chose_settings_uvv();
                }
                else if (current_ekran.current_level == EKRAN_CHOSE_COMMUNICATION_PARAMETERS)
                {
                  if (++current_ekran.index_position >= MAX_ROW_FOR_CHOSE_COMMUNICATION_PARAMETERS)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[EKRAN_CHOSE_COMMUNICATION_PARAMETERS] = current_ekran.index_position;
                  //Формуємо екран вибору настройок парамеирів комунікації
                  make_ekran_chose_communication_parameters();
                }
                else if (current_ekran.current_level == EKRAN_VIEW_NAME_OF_CELL)
                {
                  current_ekran.index_position += MAX_ROW_LCD;
                  if (current_ekran.index_position >= MAX_ROW_FOR_VIEW_NAME_OF_CELL)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[EKRAN_VIEW_NAME_OF_CELL] = current_ekran.index_position;
                  //Формуємо екран відображення імені ячейки
                  make_ekran_name_of_cell();
                }
                else if (current_ekran.current_level == EKRAN_CHOSE_SETTING_RS485)
                {
                  if (++current_ekran.index_position >= MAX_ROW_FOR_CHOSE_SETTING_RS485)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[EKRAN_CHOSE_SETTING_RS485] = current_ekran.index_position;
                  //Формуємо екран відображення списку настройок для інтерфейсу RS-485
                  make_ekran_chose_setting_rs485();
                }
                else if (current_ekran.current_level == EKRAN_PHY_LAYER_RS485)
                {
                  if (++current_ekran.index_position >= MAX_ROW_FOR_PHY_LAYER_RS485)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[EKRAN_PHY_LAYER_RS485] = current_ekran.index_position;
                  //Формуємо екран відображення списку настройок фізичного рівня для інтерфейсу RS-485
                  make_ekran_phy_layer_rs485();
                }
                else if (current_ekran.current_level == EKRAN_PROTOCOLS_RS485)
                {
                  if (++current_ekran.index_position >= MAX_ROW_FOR_PROTOCOLS_RS485)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[EKRAN_PROTOCOLS_RS485] = current_ekran.index_position;
                  //Формуємо екран відображення списку настройок фізичного рівня для інтерфейсу RS-485
                  make_ekran_protocols_rs485();
                }
                else if (current_ekran.current_level == EKRAN_CHOSE_DATA_TIME)
                {
                  if (++current_ekran.index_position >= MAX_ROW_FOR_CHOSE_DATA_TIME_SETTINGS)
                    current_ekran.index_position = 0;
                  if ((current_settings.dst & MASKA_FOR_BIT(N_BIT_TZ_DST)) == 0)
                  {
                    while (
                      (current_ekran.index_position == INDEX_ML_CHDT_DST_ON) ||
                      (current_ekran.index_position == INDEX_ML_CHDT_DST_OFF))
                    {
                      if (++current_ekran.index_position >= MAX_ROW_FOR_CHOSE_DATA_TIME_SETTINGS)
                        current_ekran.index_position = 0;
                    }
                  }

                  position_in_current_level_menu[EKRAN_CHOSE_DATA_TIME] = current_ekran.index_position;
                  //Формуємо екран вибору налаштувань дати і часу
                  make_ekran_chose_data_time_settings();
                }
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                else if (current_ekran.current_level == EKRAN_LIST_TYPE_IEC61850_NODES)
                {
                  if (++current_ekran.index_position >= MAX_ROW_FOR_TYPE_IEC61850_NODES)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[EKRAN_LIST_TYPE_IEC61850_NODES] = current_ekran.index_position;

                  //Формуємо екран заголовків для типів логічних вузлів для IEC 61850
                  make_ekran_type_IEC61850_nodes();
                }
                else if (current_ekran.current_level == EKRAN_LIST_IN_GOOSE)
                {
                  if (++current_ekran.index_position >= N_IN_GOOSE)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[EKRAN_LIST_IN_GOOSE] = current_ekran.index_position;

                  //Формуємо екран заголовків вхідних GOOSE блоків
                  make_ekran_chose_of_list_for_ranguvannja(ID_IN_GOOSE);
                }
                else if (current_ekran.current_level == EKRAN_LIST_IN_MMS)
                {
                  if (++current_ekran.index_position >= N_IN_MMS)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[EKRAN_LIST_IN_MMS] = current_ekran.index_position;

                  //Формуємо екран заголовків вхідних MMS блоків
                  make_ekran_chose_of_list_for_ranguvannja(ID_IN_MMS);
                }
                else if (current_ekran.current_level == EKRAN_LIST_OUT_LAN)
                {
                  if (++current_ekran.index_position >= N_OUT_LAN)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[EKRAN_LIST_OUT_LAN] = current_ekran.index_position;

                  //Формуємо екран заголовків вихідний мережевий блок
                  make_ekran_chose_of_list_for_ranguvannja(ID_OUT_LAN);
                }
                else if (
                  ((current_ekran.current_level >= EKRAN_IN_GOOSE1) && (current_ekran.current_level <= EKRAN_IN_GOOSE16)) ||
                  ((current_ekran.current_level >= EKRAN_IN_MMS1) && (current_ekran.current_level <= EKRAN_IN_MMS4)))
                {
                  if (++current_ekran.index_position >= N_IN_GOOSE_MMS_OUT)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;
                  //Формуємо екран заголовків виходів
                  make_ekran_list_in_out_for_iec61850(0, N_IN_GOOSE_MMS_OUT);
                }
                else if ((current_ekran.current_level >= EKRAN_OUT_LAN1) && (current_ekran.current_level <= EKRAN_OUT_LAN4))
                {
                  if (++current_ekran.index_position >= N_OUT_LAN_IN)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;
                  //Формуємо екран заголовків виходів
                  make_ekran_list_in_out_for_iec61850(1, N_OUT_LAN_IN);
                }
                else if (current_ekran.current_level == EKRAN_CHOSE_SETTING_ETHERNET)
                {
                  if (++current_ekran.index_position >= MAX_ROW_FOR_CHOSE_SETTING_ETHERNET)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[EKRAN_CHOSE_SETTING_ETHERNET] = current_ekran.index_position;
                  //Формуємо екран відображення списку настройок для інтерфейсу Ethernet
                  make_ekran_chose_setting_Ethernet();
                }
#endif
                else if (current_ekran.current_level == EKRAN_VIEW_LIST_OF_REGISTRATORS)
                {
                  if (++current_ekran.index_position >= MAX_ROW_FOR_LIST_OF_REGISTRATORS)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[EKRAN_VIEW_LIST_OF_REGISTRATORS] = current_ekran.index_position;
                  //Формуємо екран вибору реєстратора
                  make_ekran_chose_registrators();
                }
                else if (current_ekran.current_level == EKRAN_VIEW_SETTINGS_OF_DIGITAL_REGISTRATORS)
                {
                  if (++current_ekran.index_position >= MAX_ROW_FOR_SETTINGS_OF_DIGITAL_REGISTRATOR)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[EKRAN_VIEW_SETTINGS_OF_DIGITAL_REGISTRATORS] = current_ekran.index_position;
                  //Формуємо екран вибору настройок аналогового реєстратора
                  make_ekran_settings_digital_registrators();
                }
                else if (current_ekran.current_level == EKRAN_VIEW_SETTINGS_OF_ANALOG_REGISTRATORS)
                {
                  if (++current_ekran.index_position >= MAX_ROW_FOR_SETTINGS_OF_ANALOG_REGISTRATOR)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[EKRAN_VIEW_SETTINGS_OF_ANALOG_REGISTRATORS] = current_ekran.index_position;
                  //Формуємо екран вибору настройок аналогового реєстратора
                  make_ekran_settings_analog_registrators();
                }
                else if (current_ekran.current_level == EKRAN_EXTENDED_LIGIC)
                {
                  current_ekran.index_position++;
                  do
                  {
                    if (current_ekran.index_position >= MAX_ROW_FOR_EXTENDED_LIGIC)
                      current_ekran.index_position = 0;

                    //                  if ((current_ekran.index_position == INDEX_OF_DEFINED_FUNCTIONS) && (current_settings.number_defined_df == 0))
                    //                    current_ekran.index_position++;
                    //                  if ((current_ekran.index_position == INDEX_OF_DEFINED_TRIGGERS) && (current_settings.number_defined_dt == 0))
                    //                    current_ekran.index_position++;
                    //                  if ((current_ekran.index_position == INDEX_OF_DEFINED_AND) && (current_settings.number_defined_and == 0))
                    //                    current_ekran.index_position++;
                    //                  if ((current_ekran.index_position == INDEX_OF_DEFINED_OR) && (current_settings.number_defined_or == 0))
                    //                    current_ekran.index_position++;
                    //                  if ((current_ekran.index_position == INDEX_OF_DEFINED_XOR) && (current_settings.number_defined_xor == 0))
                    //                    current_ekran.index_position++;
                    //                  if ((current_ekran.index_position == INDEX_OF_DEFINED_NOT) && (current_settings.number_defined_not == 0))
                    //                    current_ekran.index_position++;
                  } while (current_ekran.index_position >= MAX_ROW_FOR_EXTENDED_LIGIC);

                  position_in_current_level_menu[EKRAN_EXTENDED_LIGIC] = current_ekran.index_position;
                  //Формуємо екран вибору налаштувань розширеної логіки
                  make_ekran_extended_logic();
                }
                else if (current_ekran.current_level == EKRAN_LIST_SETTINGS_FOR_DF)
                {
                  if (++current_ekran.index_position >= MAX_ROW_SETTINGS_DF)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[EKRAN_LIST_SETTINGS_FOR_DF] = current_ekran.index_position;
                  //Формуємо екран відображення списку настройок для опреділювальних функцій
                  make_ekran_chose_settings_df();
                }
                else if ((current_ekran.current_level == EKRAN_LIST_DF_FOR_RANGUVANNJA) || (current_ekran.current_level == EKRAN_LIST_DF_FOR_TIMEOUT_SETTINGS))
                {
                  //Натиснута кнопка DOWN
                  if (++current_ekran.index_position >= MAX_ROW_FOR_LIST_DF)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;
                  //Формуємо екран заголовків опреділювальних функцій
                  make_ekran_chose_of_list_for_ranguvannja(ID_DF);
                }
                else if ((current_ekran.current_level >= EKRAN_LIST_TYPE_SOURCE_DF1) && (current_ekran.current_level <= (EKRAN_LIST_TYPE_SOURCE_DF1 + NUMBER_DEFINED_FUNCTIONS - 1)))
                {
                  //Натиснута кнопка DOWN
                  if (++current_ekran.index_position >= MAX_ROW_LIST_TYPE_SOURCE_DF)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;
                  //Формуємо екран заголовків типів джерел опреділювальних функцій
                  make_ekran_list_type_source_df();
                }
                else if (current_ekran.current_level == EKRAN_LIST_DT)
                {
                  if (++current_ekran.index_position >= MAX_ROW_FOR_LIST_DT)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[EKRAN_LIST_DT] = current_ekran.index_position;
                  //Формуємо екран заголовків опреділювальних триґерів
                  make_ekran_chose_of_list_for_ranguvannja(ID_DT);
                }
                else if ((current_ekran.current_level >= EKRAN_SET_RESET_DT1) && (current_ekran.current_level <= (EKRAN_SET_RESET_DT1 + NUMBER_DEFINED_TRIGGERS - 1)))
                {
                  if (++current_ekran.index_position >= MAX_ROW_FOR_LIST_SET_RESET_DT)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;
                  //Формуємо екран заголовків активація-деактивація опреділювальних триґерів
                  make_ekran_set_reset_trigger();
                }
                else if ((current_ekran.current_level >= EKRAN_SET_DT1) && (current_ekran.current_level <= (EKRAN_SET_DT1 + 2 * NUMBER_DEFINED_TRIGGERS - 1)))
                {
                  if (++current_ekran.index_position >= MAX_ROW_LIST_TYPE_SOURCE_DT)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;
                  //Формуємо екран заголовків типів джерел опреділювальних триґерів
                  make_ekran_list_type_source_dt();
                }
                else if (current_ekran.current_level == EKRAN_LIST_D_AND)
                {
                  if (++current_ekran.index_position >= MAX_ROW_FOR_LIST_D_AND)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[EKRAN_LIST_D_AND] = current_ekran.index_position;
                  //Формуємо екран заголовків визначуваних "І"
                  make_ekran_chose_of_list_for_ranguvannja(ID_AND);
                }
                else if (current_ekran.current_level == EKRAN_LIST_D_OR)
                {
                  if (++current_ekran.index_position >= MAX_ROW_FOR_LIST_D_OR)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[EKRAN_LIST_D_OR] = current_ekran.index_position;
                  //Формуємо екран заголовків визначуваних "АБО"
                  make_ekran_chose_of_list_for_ranguvannja(ID_OR);
                }
                else if (current_ekran.current_level == EKRAN_LIST_D_XOR)
                {
                  if (++current_ekran.index_position >= MAX_ROW_FOR_LIST_D_XOR)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[EKRAN_LIST_D_XOR] = current_ekran.index_position;
                  //Формуємо екран заголовків визначуваних "Викл.АБО"
                  make_ekran_chose_of_list_for_ranguvannja(ID_XOR);
                }
                else if (current_ekran.current_level == EKRAN_LIST_D_NOT)
                {
                  if (++current_ekran.index_position >= MAX_ROW_FOR_LIST_D_NOT)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[EKRAN_LIST_D_NOT] = current_ekran.index_position;
                  //Формуємо екран заголовків визначуваних "НЕ"
                  make_ekran_chose_of_list_for_ranguvannja(ID_NOT);
                }
                else if (current_ekran.current_level == EKRAN_LIST_TF_FOR_RANGUVANNJA)
                {
                  if (++current_ekran.index_position >= MAX_ROW_FOR_LIST_TF)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;
                  //Формуємо екран заголовків передавальних функцій
                  make_ekran_chose_of_list_for_ranguvannja(ID_TF);
                }
                else if (current_ekran.current_level == EKRAN_LIST_BUTTONS_FOR_RANGUVANNJA)
                {
                  //Натиснута кнопка DOWN
                  if (++current_ekran.index_position >= MAX_ROW_LIST_BUTTONS_FOR_RANGUVANNJA)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[EKRAN_LIST_BUTTONS_FOR_RANGUVANNJA] = current_ekran.index_position;
                  //Формуємо екран заголовків функціональних кнопок для ранжування
                  make_ekran_chose_of_list_for_ranguvannja(ID_DB);
                }
                else if (current_ekran.current_level == EKRAN_INFO)
                {
                  if (++current_ekran.index_position >= MAX_ROW_FOR_INFO)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[EKRAN_INFO] = current_ekran.index_position;
                  //Формуємо екран інформації
                  make_ekran_info();
                }
                else if (current_ekran.current_level == EKRAN_DATE_TIME_PZ)
                {
                  if (++current_ekran.index_position >= MAX_ROW_FOR_DATE_TIME_PZ)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[EKRAN_DATE_TIME_PZ] = current_ekran.index_position;
                  //Формуємо екран відображення міти дати і часу прошивки
                  make_ekran_date_time_pz();
                }
                else if (current_ekran.current_level == EKRAN_POINT_TIME_SETTINGS)
                {
                  if (++current_ekran.index_position >= MAX_ROW_FOR_POINT_TIME_SETTINGS)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[EKRAN_POINT_TIME_SETTINGS] = current_ekran.index_position;
                  //Формуємо екран вибору міток настроювання
                  make_ekran_chose_item_of_point_time_settings();
                }
                else if (current_ekran.current_level == EKRAN_POINT_TIME_SETPOINT)
                {
                  if (++current_ekran.index_position >= MAX_ROW_FOR_POINT_TIME_SETPOINT_RANGUVANNJA)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[EKRAN_POINT_TIME_SETPOINT] = current_ekran.index_position;
                  //Формуємо екран відображення міти останніх змін у уставках-витримках-управлінчькій інформації
                  make_ekran_time_settings(0);
                }
                else if (current_ekran.current_level == EKRAN_POINT_TIME_RANGUVANNJA)
                {
                  if (++current_ekran.index_position >= MAX_ROW_FOR_POINT_TIME_SETPOINT_RANGUVANNJA)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[EKRAN_POINT_TIME_RANGUVANNJA] = current_ekran.index_position;
                  //Формуємо екран відображення міти останніх змін у ронжуваннях
                  make_ekran_time_settings(1);
                }
                else if (current_ekran.current_level == EKRAN_DIAGNOSTYKA)
                {
                  unsigned int diagnostyka_tmp[N_DIAGN];
                  for (size_t i = 0; i < N_DIAGN; i++)
                    diagnostyka_tmp[i] = diagnostyka[i];

                  unsigned int not_null = false;
                  for (size_t i = 0; i < N_DIAGN; i++)
                  {
                    not_null |= (diagnostyka_tmp[i] != 0);
                    if (not_null)
                      break;
                  }

                  if (not_null)
                  {
                    if (++current_ekran.index_position >= ((int) MAX_ROW_FOR_DIAGNOSTYKA))
                      current_ekran.index_position = 0;
                    while (_CHECK_SET_BIT(diagnostyka_tmp, current_ekran.index_position) == 0)
                    {
                      current_ekran.index_position++;
                      if (current_ekran.index_position >= ((int) MAX_ROW_FOR_DIAGNOSTYKA))
                        current_ekran.index_position = 0;
                    }
                  }
                  else
                  {
                    current_ekran.index_position = 0;
                  }

                  position_in_current_level_menu[EKRAN_DIAGNOSTYKA] = current_ekran.index_position;
                  //Формуємо екран діагностики
                  make_ekran_diagnostyka(diagnostyka_tmp);
                }
                else if (current_ekran.current_level == EKRAN_LIST_INPUTS_OUTPUTS)
                {
                  if (++current_ekran.index_position >= MAX_ROW_FOR_LIST_INPUTS_OUTPUTS)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[EKRAN_LIST_INPUTS_OUTPUTS] = current_ekran.index_position;
                  //Формуємо екран вибору станів входів-виходів
                  make_ekran_list_inputs_outputs();
                }
                else if ((current_ekran.current_level == EKRAN_STATE_INPUTS) || (current_ekran.current_level == EKRAN_STATE_OUTPUTS))
                {
                  if (current_ekran.current_level == EKRAN_STATE_INPUTS)
                  {
                    if (++current_ekran.index_position >= NUMBER_INPUTS)
                      current_ekran.index_position = 0;
                  }
                  else
                  {
                    if (++current_ekran.index_position >= (NUMBER_SIMPLE_OUTPUTS
#ifdef NUMBER_DS
                                                           + ((current_settings.configuration & (1 << DS_BIT_CONFIGURATION)) != 0) * NUMBER_DS
#endif
                                                           ))
                      current_ekran.index_position = 0;
                  }

                  position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;
                  //Формуємо екран станів входів або виходыв
                  make_ekran_state_inputs_or_outputs(current_ekran.current_level - EKRAN_STATE_INPUTS);
                }
                else if (current_ekran.current_level == EKRAN_LIST_REGISTRATORS)
                {
                  if (++current_ekran.index_position >= MAX_ROW_FOR_LIST_REGISTRATORS)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[EKRAN_LIST_REGISTRATORS] = current_ekran.index_position;
                  //Формуємо екран вибору реєстраторів
                  make_ekran_list_registrators();
                }
                else if (
                  (current_ekran.current_level == EKRAN_LIST_ANALOG_REGISTRATOR_RECORDS) ||
                  (current_ekran.current_level == EKRAN_LIST_DIGITAL_REGISTRATOR_RECORDS) ||
                  (current_ekran.current_level == EKRAN_LIST_REGISTRATOR_PROGRAM_ERROR_RECORDS)
                  //||(current_ekran.current_level == EKRAN_LIST_STATE_CMD_REGISTRATOR_RECORDS)
                )
                {
                  unsigned int number_records = 0;
                  unsigned int type_registrator = 0;

                  if (current_ekran.current_level == EKRAN_LIST_ANALOG_REGISTRATOR_RECORDS)
                  {
                    unsigned int first_number = (info_rejestrator_ar.first_number < 0) ? 0 : (info_rejestrator_ar.first_number + 1);
                    unsigned int last_number = (info_rejestrator_ar.last_number < 0) ? 0 : (info_rejestrator_ar.last_number + 1);

                    if (first_number == 0)
                      number_records = 0;
                    else if (first_number >= last_number)
                      number_records = first_number - last_number + 1;
                    else
                      number_records = NUMBER_FATFS_NAME - last_number + first_number + 1;

                    type_registrator = INDEX_ML_ANALOG_REGISTRATOR_INFO;
                  }
                  else if (current_ekran.current_level == EKRAN_LIST_DIGITAL_REGISTRATOR_RECORDS)
                  {
                    number_records = info_rejestrator_dr.number_records;
                    type_registrator = INDEX_ML_DIGITAL_REGISTRATOR_INFO;
                  }
                  else if (current_ekran.current_level == EKRAN_LIST_REGISTRATOR_PROGRAM_ERROR_RECORDS)
                  {
                    number_records = info_rejestrator_pr_err.number_records;
                    type_registrator = INDEX_ML_PROGRAM_ERROE_REGISTRATOR_INFO;
                  }
                  //else
                  //{
                  //  number_records = holderCmdPlusTime.shTotalFixElem;
                  //  type_registrator = INDEX_ML_STATE_CMD_REGISTRATOR_INFO;
                  //}
                  else
                  {
                    total_error_sw_fixed();
                  }

                  if (++current_ekran.index_position >= ((int) number_records))
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;
                  //Формуємо екран відображення записів
                  if (type_registrator == INDEX_ML_ANALOG_REGISTRATOR_INFO)
                    _SET_STATE(FATFS_command, FATFS_READ_DATA_FOR_MENU);
                  else
                    make_ekran_list_records_registrator(type_registrator);
                }
                else if (current_ekran.current_level == EKRAN_TITLES_DIGITAL_REGISTRATOR)
                {
                  do
                  {
                    if (++current_ekran.index_position >= MAX_ROW_FOR_TITLES_DIGITAL_REGISTRATOR)
                      current_ekran.index_position = 0;
                  } while (
                    ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_DR_MENU) == 0) &&
                    (current_ekran.index_position > INDEX_ML_TITLE_DR_CHANGES_SIGNALS) &&
                    (((current_ekran.index_position == INDEX_ML_TITLE_DR_MIN_U) && (buffer_for_manu_read_record[FIRST_INDEX_NUMBER_MIN_U_DR] == 0)) ||
                     ((current_ekran.index_position == INDEX_ML_TITLE_DR_MAX_U) && (buffer_for_manu_read_record[FIRST_INDEX_NUMBER_MAX_U_DR] == 0))));
                  position_in_current_level_menu[EKRAN_TITLES_DIGITAL_REGISTRATOR] = current_ekran.index_position;
                  //Формуємо екран відображення заголовків груп для дискретного реєстратора
                  make_ekran_list_titles_for_record_of_digital_registrator();
                }
                else if (current_ekran.current_level == EKRAN_CHANGES_SIGNALS_DR)
                {
                  current_ekran.index_position += (MAX_ROW_LCD >> 1);
                  //Формуємо екран відображення змін сигналів - записаних у дискретному реєстраторі
                  make_ekran_changing_signals_digital_registrator();
                }
                else if (current_ekran.current_level == EKRAN_TITLE_MAX_VALUES)
                {
                  unsigned int number_records = buffer_for_manu_read_record[FIRST_INDEX_NUMBER_MIN_U_DR + type_view_max_values_dr - IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE];

                  if (++current_ekran.index_position >= ((int) number_records))
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[EKRAN_TITLE_MAX_VALUES] = current_ekran.index_position;

                  //Формуємо екран відображення міток часу записів міксації максимальних струмів
                  make_ekran_title_analog_value_records_digital_registrator();
                }
                else if (current_ekran.current_level == EKRAN_MAX_VALUES)
                {
                  unsigned char *point_unsigned_char = (unsigned char *) (buffer_for_manu_read_record + index_cell_into_array_for_integral_values_dr);
                  unsigned int *point_unsigned_int = (unsigned int *) point_unsigned_char;
                  unsigned int control_for_dr = *(point_unsigned_int + 5);

                  current_ekran.index_position++;
                  do
                  {
                    if (current_ekran.index_position >= MAX_ROW_FOR_EKRAN_ANALOG_VALUES_DR)
                      current_ekran.index_position = 0;

                    while (
                      (buffer_for_manu_read_record[FIRST_INDEX_START_START_RECORD_DR] == LABEL_START_RECORD_DR) && /*якщо ця умова не виконується, то у функції make_ekran_analog_value_records_digital_registrator ми перейдемо на відображення інформації про недоступні дані, а не будемо виконувати фільтрацію*/
                      ((control_for_dr & (1 << 0)) == 0) &&                                                        /*моніторинг вівся, коли був вибраний 2-обмотковий трансформатор*/
                      (current_ekran.index_position >= INDEX_ML_DR_U2) &&
                      (current_ekran.index_position <= INDEX_ML_DR_I2))
                      current_ekran.index_position++;

                  } while (current_ekran.index_position >= MAX_ROW_FOR_EKRAN_ANALOG_VALUES_DR);

                  //Формуємо екран відображення аналогових значень з запису дискретного реєстратора
                  make_ekran_analog_value_records_digital_registrator();
                }
                else if (current_ekran.current_level == EKRAN_LIST_STATE_CMD_REGISTRATOR_RECORDS)
                {
                  if (++current_ekran.index_position >= holderCmdPlusTime.shTotalFixElem)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[EKRAN_LIST_STATE_CMD_REGISTRATOR_RECORDS] = current_ekran.index_position;
                  //Формуємо екран відображення дат і часу
                  //?make_ekran_data_and_time_elem_stt_registrator(0);
                  make_ekran_data_elem_stt_registrator();
                }
                else if (current_ekran.current_level == EKRAN_CHANGES_DIAGNOSTICS_PR_ERR)
                {
                  current_ekran.index_position++;
                  //Формуємо екран відображення змін діагностик - записаних у реєстраторі програмних подій
                  make_ekran_changing_diagnostics_pr_err_registrator();
                }
                else if (current_ekran.current_level == EKRAN_DATA_LABEL_STATE_CMD)
                {
                  //?if(++current_ekran.index_position >= MAX_ROW_FOR_EKRAN_DATA_LABEL) current_ekran.index_position =  0;
                  //?position_in_current_level_menu[EKRAN_DATA_LABEL_STATE_CMD] = current_ekran.index_position;
                  //?//Формуємо екран відображення мітки часу прочитаного запису аналогового реєстратора
                  //?make_ekran_data_and_time_of_records_registrator(2);
                  current_ekran.index_position += (MAX_ROW_LCD);
                  make_ekran_signals_stable_one_in_data_elem_stt_reg();
                }
                //?else if (current_ekran.current_level == EKRAN_STATE_CMD_REG)
                //?{
                //?  current_ekran.index_position += (MAX_ROW_LCD >> 1);
                //?  //Формуємо екран відображення змін сигналів - записаних у дискретному реєстраторі
                //?  make_ekran_changing_signals_statistica_registrator();
                //?}

                else if (current_ekran.current_level == EKRAN_TITLES_STATE_CMD_REGISTRATOR)
                {
                  current_ekran.index_position++; //+= (MAX_ROW_LCD >> 1);

                  make_ekran_time_ms_c_val_c_name_sr_format_changing_signals();
                }

                //Очистити сигналізацію, що натиснута кнопка
                new_state_keyboard &= ~(1u << BIT_KEY_DOWN);
              }
              else
              {
                //Натиснуто зразу декілька кнопок - це є невизначена ситуація, тому скидаємо сигналізацію про натиснуті кнопки і чекаємо знову
                unsigned int temp_data = new_state_keyboard;
                new_state_keyboard &= ~temp_data;
              }
            }
          }
          break;
        }
        /****************************************************************************************************************************************/

        /******************************************************************************************************************************************/
      case EKRAN_SETPOINT_RPN_GROUP1:
      case EKRAN_SETPOINT_RPN_GROUP2:
      case EKRAN_SETPOINT_RPN_GROUP3:
      case EKRAN_SETPOINT_RPN_GROUP4:
      case EKRAN_TIMEOUT_RPN_GROUP1:
      case EKRAN_TIMEOUT_RPN_GROUP2:
      case EKRAN_TIMEOUT_RPN_GROUP3:
      case EKRAN_TIMEOUT_RPN_GROUP4:
      case EKRAN_CONTROL_RPN:
      case EKRAN_CONTROL_SZKh:
      case EKRAN_SETPOINT_BRP_GROUP1:
      case EKRAN_SETPOINT_BRP_GROUP2:
      case EKRAN_SETPOINT_BRP_GROUP3:
      case EKRAN_SETPOINT_BRP_GROUP4:
      case EKRAN_CONTROL_BRP:
      case EKRAN_TIMEOUT_ZNKh_GROUP1:
      case EKRAN_TIMEOUT_ZNKh_GROUP2:
      case EKRAN_TIMEOUT_ZNKh_GROUP3:
      case EKRAN_TIMEOUT_ZNKh_GROUP4:
      case EKRAN_CONTROL_ZNKh:
      case EKRAN_SETPOINT_UMIN_GROUP1:
      case EKRAN_SETPOINT_UMIN_GROUP2:
      case EKRAN_SETPOINT_UMIN_GROUP3:
      case EKRAN_SETPOINT_UMIN_GROUP4:
      case EKRAN_TIMEOUT_UMIN_GROUP1:
      case EKRAN_TIMEOUT_UMIN_GROUP2:
      case EKRAN_TIMEOUT_UMIN_GROUP3:
      case EKRAN_TIMEOUT_UMIN_GROUP4:
      case EKRAN_CONTROL_UMIN:
      case EKRAN_SETPOINT_UMAX_GROUP1:
      case EKRAN_SETPOINT_UMAX_GROUP2:
      case EKRAN_SETPOINT_UMAX_GROUP3:
      case EKRAN_SETPOINT_UMAX_GROUP4:
      case EKRAN_TIMEOUT_UMAX_GROUP1:
      case EKRAN_TIMEOUT_UMAX_GROUP2:
      case EKRAN_TIMEOUT_UMAX_GROUP3:
      case EKRAN_TIMEOUT_UMAX_GROUP4:
      case EKRAN_CONTROL_UMAX:
      case EKRAN_SETPOINT_UP_GROUP1:
      case EKRAN_SETPOINT_UP_GROUP2:
      case EKRAN_SETPOINT_UP_GROUP3:
      case EKRAN_SETPOINT_UP_GROUP4:
      case EKRAN_TIMEOUT_UP_GROUP1:
      case EKRAN_TIMEOUT_UP_GROUP2:
      case EKRAN_TIMEOUT_UP_GROUP3:
      case EKRAN_TIMEOUT_UP_GROUP4:
      case EKRAN_CONTROL_UP:
      case EKRAN_TRANSFORMATOR_INFO:
      case EKRAN_SETPOINT_SWITCH:
      case EKRAN_TIMEOUT_SWITCH:
      case EKRAN_CONTROL_SWITCH:
      case EKRAN_DOPUSK_DV_UVV:
      case EKRAN_TYPE_INPUT_UVV:
      case EKRAN_TYPE_INPUT_SIGNAL_UVV:
      case EKRAN_TYPE_OUTPUT_UVV:
      case EKRAN_TYPE_LED_UVV:
      case EKRAN_TYPE_BUTTON_UVV:
      case EKRAN_ADDRESS_RS485:
      case EKRAN_VIEW_SPEED_RS485:
      case EKRAN_VIEW_PARE_RS485:
      case EKRAN_VIEW_STOP_BITS_RS485:
      case EKRAN_VIEW_TIMEOUT_RS485:

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
      case EKRAN_SETTING_NETWORK_LAYER_ETHERNET:
      case EKRAN_SYNCHRO:
#endif

      case EKRAN_TIME_ZONE:
      case EKRAN_DST_RULE:
      case EKRAN_GENERAL_PICKUPS_EL:
      case EKRAN_LIST_TYPE_DF:
      case (EKRAN_TIMEOUT_DF1 + 0):
      case (EKRAN_TIMEOUT_DF1 + 1):
      case (EKRAN_TIMEOUT_DF1 + 2):
      case (EKRAN_TIMEOUT_DF1 + 3):
      case (EKRAN_TIMEOUT_DF1 + 4):
      case (EKRAN_TIMEOUT_DF1 + 5):
      case (EKRAN_TIMEOUT_DF1 + 6):
      case (EKRAN_TIMEOUT_DF1 + 7):
      case EKRAN_TIMEOUT_ANALOG_REGISTRATOR:
      case EKRAN_TIMEOUT_DIGITAL_REGISTRATOR:
      case EKRAN_CONTROL_AR:
      case EKRAN_VIEW_SETTING_LANGUAGE:
      case EKRAN_CHOSE_EXTRA_SETTINGS:
      case EKRAN_VIEW_GRUPA_USTAVOK:
      case (EKRAN_LIST_SOURCE_TF1 + 0):
      case (EKRAN_LIST_SOURCE_TF1 + 1):
      case (EKRAN_LIST_SOURCE_TF1 + 2):
      case (EKRAN_LIST_SOURCE_TF1 + 3):
      case (EKRAN_LIST_SOURCE_TF1 + 4):
      case (EKRAN_LIST_SOURCE_TF1 + 5):
      case (EKRAN_LIST_SOURCE_TF1 + 6):
      case (EKRAN_LIST_SOURCE_TF1 + 7):
      case (EKRAN_LIST_SOURCE_TF1 + 8):
      case (EKRAN_LIST_SOURCE_TF1 + 9):
      case (EKRAN_LIST_SOURCE_TF1 + 10):
      case (EKRAN_LIST_SOURCE_TF1 + 11):
      case (EKRAN_LIST_SOURCE_TF1 + 12):
      case (EKRAN_LIST_SOURCE_TF1 + 13):
      case (EKRAN_LIST_SOURCE_TF1 + 14):
      case (EKRAN_LIST_SOURCE_TF1 + 15):
        {
          //Очищаємо всі біти краім упралінських
          unsigned int maska_keyboard_bits = (1u << BIT_KEY_ENTER) |
                                             (1u << BIT_KEY_ESC) |
                                             (1u << BIT_REWRITE);

          if (current_ekran.edition == 1)
            maska_keyboard_bits |= (1u << BIT_KEY_RIGHT) | (1u << BIT_KEY_LEFT) | (1u << BIT_KEY_UP) | (1u << BIT_KEY_DOWN);
          else if (current_ekran.edition == 0)
            maska_keyboard_bits |= (1u << BIT_KEY_UP) | (1u << BIT_KEY_DOWN);

          new_state_keyboard &= maska_keyboard_bits;
          //Дальше виконуємо дії, якщо натиснута кнопка на яку треба реагівати, або стоїть команда обновити екран
          if (new_state_keyboard != 0)
          {
            //Пріоритет стоїть на обновлені екрану
            if ((new_state_keyboard & (1u << BIT_REWRITE)) != 0)
            {

              if (
                (current_ekran.current_level >= EKRAN_SETPOINT_RPN_GROUP1) &&
                (current_ekran.current_level <= EKRAN_SETPOINT_RPN_GROUP4))
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_SETPOINT_RPN)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;

                //Формуємо екран уставок РПН
                int group = (current_ekran.current_level - EKRAN_SETPOINT_RPN_GROUP1);
                make_ekran_setpoint_rpn(group);
              }
              else if (
                (current_ekran.current_level >= EKRAN_TIMEOUT_RPN_GROUP1) &&
                (current_ekran.current_level <= EKRAN_TIMEOUT_RPN_GROUP4))
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_TIMEOUT_RPN)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;

                //Формуємо екран витримок РПН
                int group = (current_ekran.current_level - EKRAN_TIMEOUT_RPN_GROUP1);
                make_ekran_timeout_rpn(group);
              }
              else if (current_ekran.current_level == EKRAN_CONTROL_RPN)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_CONTROL_RPN)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_CONTROL_RPN] = current_ekran.index_position;

                //Формуємо екран управлінської інформації для РПН
                make_ekran_control_rpn();
              }
              else if (current_ekran.current_level == EKRAN_CONTROL_SZKh)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_CONTROL_SZKh)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_CONTROL_SZKh] = current_ekran.index_position;

                //Формуємо екран управлінської інформації для ЗСХ
                make_ekran_control_zskh();
              }
              else if (
                (current_ekran.current_level >= EKRAN_SETPOINT_BRP_GROUP1) &&
                (current_ekran.current_level <= EKRAN_SETPOINT_BRP_GROUP4))
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_SETPOINT_BRP)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;

                //Формуємо екран уставок БРП
                int group = (current_ekran.current_level - EKRAN_SETPOINT_BRP_GROUP1);
                make_ekran_setpoint_brp(group);
              }
              else if (current_ekran.current_level == EKRAN_CONTROL_BRP)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_CONTROL_BRP)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_CONTROL_BRP] = current_ekran.index_position;

                //Формуємо екран управлінської інформації для БРП
                make_ekran_control_brp();
              }
              else if (
                (current_ekran.current_level >= EKRAN_TIMEOUT_ZNKh_GROUP1) &&
                (current_ekran.current_level <= EKRAN_TIMEOUT_ZNKh_GROUP4))
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_TIMEOUT_ZNKh)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;

                //Формуємо екран витримок ЗНХ
                int group = (current_ekran.current_level - EKRAN_TIMEOUT_ZNKh_GROUP1);
                make_ekran_timeout_znkh(group);
              }
              else if (current_ekran.current_level == EKRAN_CONTROL_ZNKh)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_CONTROL_ZNKh)
                  current_ekran.index_position = 0;

                position_in_current_level_menu[EKRAN_CONTROL_ZNKh] = current_ekran.index_position;

                //Формуємо екран управлінської інформації для ЗНХ
                make_ekran_control_znkh();
              }
              else if (
                (current_ekran.current_level >= EKRAN_SETPOINT_UMIN_GROUP1) &&
                (current_ekran.current_level <= EKRAN_SETPOINT_UMIN_GROUP4))
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_SETPOINT_UMIN)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;

                //Формуємо екран уставок Umin
                int group = (current_ekran.current_level - EKRAN_SETPOINT_UMIN_GROUP1);
                make_ekran_setpoint_Umin(group);
              }
              else if (
                (current_ekran.current_level >= EKRAN_TIMEOUT_UMIN_GROUP1) &&
                (current_ekran.current_level <= EKRAN_TIMEOUT_UMIN_GROUP4))
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_TIMEOUT_UMIN)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;

                //Формуємо екран витримок Umin
                int group = (current_ekran.current_level - EKRAN_TIMEOUT_UMIN_GROUP1);
                make_ekran_timeout_Umin(group);
              }
              else if (current_ekran.current_level == EKRAN_CONTROL_UMIN)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_CONTROL_UMIN)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_CONTROL_UMIN] = current_ekran.index_position;
                //Формуємо екран управлінської інформації для Umin
                make_ekran_control_Umin();
              }
              else if (
                (current_ekran.current_level >= EKRAN_SETPOINT_UMAX_GROUP1) &&
                (current_ekran.current_level <= EKRAN_SETPOINT_UMAX_GROUP4))
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_SETPOINT_Umax)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;

                //Формуємо екран уставок Umax
                int group = (current_ekran.current_level - EKRAN_SETPOINT_UMAX_GROUP1);
                make_ekran_setpoint_Umax(group);
              }
              else if (
                (current_ekran.current_level >= EKRAN_TIMEOUT_UMAX_GROUP1) &&
                (current_ekran.current_level <= EKRAN_TIMEOUT_UMAX_GROUP4))
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_TIMEOUT_UMAX)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;

                //Формуємо екран витримок Umax
                int group = (current_ekran.current_level - EKRAN_TIMEOUT_UMAX_GROUP1);
                make_ekran_timeout_Umax(group);
              }
              else if (current_ekran.current_level == EKRAN_CONTROL_UMAX)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_CONTROL_UMAX)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_CONTROL_UMAX] = current_ekran.index_position;
                //Формуємо екран управлінської інформації для Umax
                make_ekran_control_Umax();
              }
              else if (
                (current_ekran.current_level >= EKRAN_SETPOINT_UP_GROUP1) &&
                (current_ekran.current_level <= EKRAN_SETPOINT_UP_GROUP4))
              {
                if (current_ekran.index_position >= (NUMBER_UP * MAX_ROW_FOR_SETPOINT_UP))
                  current_ekran.index_position = 0;
                position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;

                //Формуємо екран уставок УЗ
                int group = (current_ekran.current_level - EKRAN_SETPOINT_UP_GROUP1);
                make_ekran_setpoint_UP(group);
              }
              else if (
                (current_ekran.current_level >= EKRAN_TIMEOUT_UP_GROUP1) &&
                (current_ekran.current_level <= EKRAN_TIMEOUT_UP_GROUP4))
              {
                if (current_ekran.index_position >= (NUMBER_UP * MAX_ROW_FOR_TIMEOUT_UP))
                  current_ekran.index_position = 0;
                position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;

                //Формуємо екран витримок УЗ
                int group = (current_ekran.current_level - EKRAN_TIMEOUT_UP_GROUP1);
                make_ekran_timeout_UP(group);
              }
              else if (current_ekran.current_level == EKRAN_CONTROL_UP)
              {
                if (current_ekran.index_position >= (NUMBER_UP * MAX_ROW_FOR_CONTROL_UP))
                  current_ekran.index_position = 0;
                __SETTINGS *point = (current_ekran.edition == 0) ? &current_settings : &edition_settings;
                uint32_t ctrl_UP_input = point->ctrl_UP_input[current_ekran.index_position / (_CTR_UP_NEXT_BIT - _CTR_UP_PART_I)];
                while (
                  ((current_ekran.index_position % (_CTR_UP_NEXT_BIT - _CTR_UP_PART_I)) == CTR_UP_OR_AND_BIT) &&
                  (ctrl_UP_input != UP_CTRL_Ia_Ib_Ic) &&
                  (ctrl_UP_input != UP_CTRL_Ua_Ub_Uc) &&
                  (ctrl_UP_input != UP_CTRL_Uab_Ubc_Uca))
                {
                  current_ekran.index_position++;
                  if (current_ekran.index_position >= MAX_ROW_FOR_CONTROL_UP)
                    current_ekran.index_position = 0;

                  ctrl_UP_input = point->ctrl_UP_input[current_ekran.index_position / (_CTR_UP_NEXT_BIT - _CTR_UP_PART_I)];
                }

                position_in_current_level_menu[EKRAN_CONTROL_UP] = current_ekran.index_position;

                //Формуємо екран управлінської інформації для УЗ
                make_ekran_control_UP();
              }
              else if (current_ekran.current_level == EKRAN_TRANSFORMATOR_INFO)
              {
                do
                {
                  if (current_ekran.index_position >= MAX_ROW_FOR_TRANSFORMATOR_INFO)
                    current_ekran.index_position = 0;

                  while (
                    (
                      (current_ekran.index_position == INDEX_ML_TT2) ||
                      (current_ekran.index_position == INDEX_ML_TN2)) &&
                    ((current_settings.control_rpn & MASKA_FOR_BIT(INDEX_ML_CTRRPN_TRANSF)) == 0))
                    current_ekran.index_position++;
                } while (current_ekran.index_position >= MAX_ROW_FOR_TRANSFORMATOR_INFO);

                position_in_current_level_menu[EKRAN_TRANSFORMATOR_INFO] = current_ekran.index_position;
                //Формуємо екран інфтрмації по трансформаторах
                make_ekran_transformator();
              }
              else if (current_ekran.current_level == EKRAN_SETPOINT_SWITCH)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_SETPOINT_SWITCH)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_SETPOINT_SWITCH] = current_ekran.index_position;
                //Формуємо екран уставок виключателя
                make_ekran_setpoint_switch();
              }
              else if (current_ekran.current_level == EKRAN_TIMEOUT_SWITCH)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_TIMEOUT_SWITCH)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_TIMEOUT_SWITCH] = current_ekran.index_position;
                //Формуємо екран витримок виключателя
                make_ekran_timeout_switch();
              }
              else if (current_ekran.current_level == EKRAN_CONTROL_SWITCH)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_CONTROL_SWITCH)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_CONTROL_SWITCH] = current_ekran.index_position;
                //Формуємо екран управлінської інформації для вимикача
                make_ekran_control_switch();
              }
              else if (current_ekran.current_level == EKRAN_DOPUSK_DV_UVV)
              {
                if (current_ekran.index_position >= NUMBER_INPUTS)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_DOPUSK_DV_UVV] = current_ekran.index_position;
                //Формуємо екран інфтрмації по допусках ДВ
                make_ekran_dopusk_dv();
              }
              else if ((current_ekran.current_level == EKRAN_TYPE_INPUT_UVV) || (current_ekran.current_level == EKRAN_TYPE_INPUT_SIGNAL_UVV))
              {
                if (current_ekran.index_position >= NUMBER_INPUTS)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;
                //Формуємо екран управлінської інформації для УВВ
                if (current_ekran.current_level == EKRAN_TYPE_INPUT_UVV)
                  make_ekran_type_input_uvv(0);
                else
                  make_ekran_type_input_uvv(1);
              }
              else if (current_ekran.current_level == EKRAN_TYPE_OUTPUT_UVV)
              {
                if (current_ekran.index_position >= NUMBER_SIMPLE_OUTPUTS)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_TYPE_OUTPUT_UVV] = current_ekran.index_position;
                //Формуємо екран типу виходу
                make_ekran_type_output_uvv();
              }
              else if (current_ekran.current_level == EKRAN_TYPE_LED_UVV)
              {
                if (current_ekran.index_position >= NUMBER_LEDS)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_TYPE_LED_UVV] = current_ekran.index_position;
                //Формуємо екран типу світилоіндикаторів
                make_ekran_type_led_uvv();
              }
              else if (current_ekran.current_level == EKRAN_TYPE_BUTTON_UVV)
              {
                if (current_ekran.index_position >= NUMBER_DEFINED_BUTTONS)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_TYPE_BUTTON_UVV] = current_ekran.index_position;
                //Формуємо екран типу ФК
                make_ekran_type_button_uvv();
              }
              else if (current_ekran.current_level == EKRAN_ADDRESS_RS485)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_ADDRESS)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_ADDRESS_RS485] = current_ekran.index_position;
                //Формуємо екран інфтрмації по комунікаційній адресі
                make_ekran_address();
              }
              else if (current_ekran.current_level == EKRAN_VIEW_SPEED_RS485)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_VIEW_SPEED_INTERFACE)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_VIEW_SPEED_RS485] = current_ekran.index_position;
                //Формуємо екран інформації по швидкості обміну
                make_ekran_speed_interface();
              }
              else if (current_ekran.current_level == EKRAN_VIEW_PARE_RS485)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_VIEW_PARE_INTERFACE)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_VIEW_PARE_RS485] = current_ekran.index_position;
                //Формуємо екран інформації по контролю парності
                make_ekran_pare_interface();
              }
              else if (current_ekran.current_level == EKRAN_VIEW_STOP_BITS_RS485)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_VIEW_STOP_BITS_INTERFACE)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_VIEW_STOP_BITS_RS485] = current_ekran.index_position;
                //Формуємо екран інформації по кількості стопових біт
                make_ekran_stopbits_interface();
              }
              else if (current_ekran.current_level == EKRAN_VIEW_TIMEOUT_RS485)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_VIEW_TIMEOUT_INTERFACE)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_VIEW_TIMEOUT_RS485] = current_ekran.index_position;
                //Формуємо екран інформації по time-out наступного символу
                make_ekran_timeout_interface();
              }
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
              else if (current_ekran.current_level == EKRAN_SETTING_NETWORK_LAYER_ETHERNET)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_SETTING_NETWORK_LAYER_ETHERNET)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_SETTING_NETWORK_LAYER_ETHERNET] = current_ekran.index_position;
                //Формуємо екран інфтрмації по налаштуваннях мережевого рівня Ethernet
                make_ekran_settings_network_layer_Ethernet();
              }
              else if (current_ekran.current_level == EKRAN_SYNCHRO)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_SYNCHRO)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_SYNCHRO] = current_ekran.index_position;
                //Формуємо екран інфтрмації по налаштуваннях сервера для синхронізації
                make_ekran_settings_synchro();
              }
#endif
              else if (current_ekran.current_level == EKRAN_TIME_ZONE)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_TIMEZONE_SETTINGS)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_TIME_ZONE] = current_ekran.index_position;
                //Формуємо екран
                make_ekran_timezone_dst();
              }
              else if (current_ekran.current_level == EKRAN_DST_RULE)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_DST_RULE)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_DST_RULE] = current_ekran.index_position;

                __SETTINGS *p_settings = (current_ekran.edition == 0) ? &current_settings : &edition_settings;
                uint32_t rule = (position_in_current_level_menu[previous_level_in_current_level_menu[EKRAN_DST_RULE]] == INDEX_ML_CHDT_DST_ON) ? p_settings->dst_on_rule : p_settings->dst_off_rule;
                //Формуємо екран
                make_ekran_dst_rule(rule);
              }
              else if (current_ekran.current_level == EKRAN_GENERAL_PICKUPS_EL)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_GENERAL_PICKUPS_EL)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_GENERAL_PICKUPS_EL] = current_ekran.index_position;
                //Формуємо екран відображення загальних витримок для розширеної логіки
                make_ekran_general_pickups_el();
              }
              else if (current_ekran.current_level == EKRAN_LIST_TYPE_DF)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_TYPE_DF)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_LIST_TYPE_DF] = current_ekran.index_position;
                //Формуємо екран відображення типу опреділювальної функції
                make_ekran_type_df();
              }
              else if ((current_ekran.current_level >= EKRAN_TIMEOUT_DF1) && (current_ekran.current_level <= (EKRAN_TIMEOUT_DF1 + NUMBER_DEFINED_FUNCTIONS - 1)))
              {
                if (current_ekran.index_position >= MAX_ROW_TIMEOUT_DF)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;
                //Формуємо екран таймерів опреділюваних функцій
                make_ekran_timeout_df(current_ekran.current_level - EKRAN_TIMEOUT_DF1);
              }
              else if ((current_ekran.current_level >= EKRAN_LIST_SOURCE_TF1) && (current_ekran.current_level <= (EKRAN_LIST_SOURCE_TF1 + NUMBER_TRANSFER_FUNCTIONS - 1)))
              {
                if (current_ekran.index_position >= MAX_ROW_LIST_SOURCE_TF)
                  current_ekran.index_position = 0;
                //Формуємо екран
                make_ekran_list_source_tf();
              }
              else if (current_ekran.current_level == EKRAN_TIMEOUT_DIGITAL_REGISTRATOR)
              {
                if (current_ekran.index_position >= MAX_ROW_RANGUVANNJA_DIGITAL_REGISTRATOR)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_TIMEOUT_DIGITAL_REGISTRATOR] = current_ekran.index_position;
                //Формуємо екран витримок аналогового реєстратора
                make_ekran_timeout_digital_registrator();
              }
              else if (current_ekran.current_level == EKRAN_TIMEOUT_ANALOG_REGISTRATOR)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_TIMEOUT_ANALOG_REGISTRATOR)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_TIMEOUT_ANALOG_REGISTRATOR] = current_ekran.index_position;
                //Формуємо екран витримок аналогового реєстратора
                make_ekran_timeout_analog_registrator();
              }
              else if (current_ekran.current_level == EKRAN_CONTROL_AR)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_CONTROL_AR)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_CONTROL_AR] = current_ekran.index_position;
                //Формуємо екран управління аналогового реєстратора
                make_ekran_control_ar();
              }
              else if (current_ekran.current_level == EKRAN_VIEW_SETTING_LANGUAGE)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_VIEW_SETTING_LANGUAGE)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_VIEW_SETTING_LANGUAGE] = current_ekran.index_position;
                //Формуємо екран інформації по мові меню
                make_ekran_setting_language();
              }
              else if (current_ekran.current_level == EKRAN_CHOSE_EXTRA_SETTINGS)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_CHOSE_EXTRA_SETTINGS)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_CHOSE_EXTRA_SETTINGS] = current_ekran.index_position;
                //Формуємо екран відображення додаткових налаштувань
                make_ekran_chose_extra_settings();
              }
              else if (current_ekran.current_level == EKRAN_VIEW_GRUPA_USTAVOK)
              {
                if (current_ekran.index_position >= MAX_ROW_FOR_VIEW_GRUPA_USTAVOK)
                  current_ekran.index_position = 0;
                position_in_current_level_menu[EKRAN_VIEW_GRUPA_USTAVOK] = current_ekran.index_position;
                //Формуємо екран інформації по групах уставок
                make_ekran_grupa_ustavok();
              }

              //Очищаємо біт обновлення екрану
              new_state_keyboard &= ~(1u << BIT_REWRITE);
            }
            else
            {
              if (new_state_keyboard == (1u << BIT_KEY_ENTER))
              {
                //Натиснута кнопка ENTER
                if (current_ekran.edition == 0)
                {
                  int temp_current_level = current_ekran.current_level;

                  //Копіюємо текчі настройки у структуру для редагування
                  if (
                    (current_ekran.current_level >= EKRAN_SETPOINT_RPN_GROUP1) &&
                    (current_ekran.current_level <= EKRAN_SETPOINT_RPN_GROUP4))
                  {
                    int group = (current_ekran.current_level - EKRAN_SETPOINT_RPN_GROUP1);

                    if (current_ekran.index_position == INDEX_ML_STPRPN_OSN)
                    {
                      edition_settings.setpoint_rpn_osn[group] = current_settings.setpoint_rpn_osn[group];
                      current_ekran.position_cursor_x = COL_SETPOINT_RPN_OSN_BEGIN;
                    }
                    else if (current_ekran.index_position == INDEX_ML_STPRPN_ZONE)
                    {
                      edition_settings.setpoint_rpn_zony[group] = current_settings.setpoint_rpn_zony[group];
                      current_ekran.position_cursor_x = COL_SETPOINT_RPN_ZONE_BEGIN;
                    }
                    else if (current_ekran.index_position == INDEX_ML_STPRPN_MAX_PER)
                    {
                      edition_settings.setpoint_rpn_per[group] = current_settings.setpoint_rpn_per[group];
                      current_ekran.position_cursor_x = COL_SETPOINT_RPN_MAX_PER_BEGIN;
                    }
                    else if (current_ekran.index_position == INDEX_ML_STPRPN_DOD)
                    {
                      edition_settings.setpoint_rpn_dod[group] = current_settings.setpoint_rpn_dod[group];
                      current_ekran.position_cursor_x = COL_SETPOINT_RPN_DOD_BEGIN;
                    }
                    else if (current_ekran.index_position == INDEX_ML_STPRPN_K)
                    {
                      edition_settings.setpoint_rpn_K[group] = current_settings.setpoint_rpn_K[group];
                      current_ekran.position_cursor_x = COL_SETPOINT_RPN_K_BEGIN;
                    }
                  }
                  else if (
                    (current_ekran.current_level >= EKRAN_TIMEOUT_RPN_GROUP1) &&
                    (current_ekran.current_level <= EKRAN_TIMEOUT_RPN_GROUP4))
                  {
                    int group = (current_ekran.current_level - EKRAN_TIMEOUT_RPN_GROUP1);

                    if (current_ekran.index_position == INDEX_ML_TMORPN_UB_PRYB)
                    {
                      edition_settings.timeout_rpn_ub_pryb[group] = current_settings.timeout_rpn_ub_pryb[group];
                      current_ekran.position_cursor_x = COL_TMO_RPN_UB_PRYB_BEGIN;
                    }
                    else if (current_ekran.index_position == INDEX_ML_TMORPN_UB_PRYB_PRYSK)
                    {
                      edition_settings.timeout_rpn_ub_pryb_prysk[group] = current_settings.timeout_rpn_ub_pryb_prysk[group];
                      current_ekran.position_cursor_x = COL_TMO_RPN_UB_PRYB_PRYSK_BEGIN;
                    }
                    else if (current_ekran.index_position == INDEX_ML_TMORPN_PRYSK_VID_UMAX)
                    {
                      edition_settings.timeout_rpn_prysk_vid_Umax[group] = current_settings.timeout_rpn_prysk_vid_Umax[group];
                      current_ekran.position_cursor_x = COL_TMO_RPN_PRYSK_VID_UMAX_BEGIN;
                    }
                    else if (current_ekran.index_position == INDEX_ML_TMORPN_PEREKL)
                    {
                      edition_settings.timeout_rpn_perekl[group] = current_settings.timeout_rpn_perekl[group];
                      current_ekran.position_cursor_x = COL_TMO_RPN_PEREKL_BEGIN;
                    }
                    else if (current_ekran.index_position == INDEX_ML_TMORPN_NESPR)
                    {
                      edition_settings.timeout_rpn_nespr[group] = current_settings.timeout_rpn_nespr[group];
                      current_ekran.position_cursor_x = COL_TMO_RPN_NESPR_BEGIN;
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_CONTROL_RPN)
                  {
                    edition_settings.control_rpn = current_settings.control_rpn;
                  }
                  else if (current_ekran.current_level == EKRAN_CONTROL_SZKh)
                  {
                    edition_settings.control_zskh = current_settings.control_zskh;
                  }
                  else if (
                    (current_ekran.current_level >= EKRAN_SETPOINT_BRP_GROUP1) &&
                    (current_ekran.current_level <= EKRAN_SETPOINT_BRP_GROUP4))
                  {
                    int group = (current_ekran.current_level - EKRAN_SETPOINT_BRP_GROUP1);

                    if (current_ekran.index_position == INDEX_ML_STPBRP_I_BLK)
                    {
                      edition_settings.setpoint_brp_I_blk[group] = current_settings.setpoint_brp_I_blk[group];
                      current_ekran.position_cursor_x = COL_SETPOINT_BRP_I_BLK_BEGIN;
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_CONTROL_BRP)
                  {
                    edition_settings.control_brp = current_settings.control_brp;
                  }
                  else if (
                    (current_ekran.current_level >= EKRAN_TIMEOUT_ZNKh_GROUP1) &&
                    (current_ekran.current_level <= EKRAN_TIMEOUT_ZNKh_GROUP4))
                  {
                    int group = (current_ekran.current_level - EKRAN_TIMEOUT_ZNKh_GROUP1);

                    if (current_ekran.index_position == INDEX_ML_TMOZNKh_PEREKL)
                    {
                      edition_settings.timeout_znkh_perekl[group] = current_settings.timeout_znkh_perekl[group];
                      current_ekran.position_cursor_x = COL_TMO_ZNKh_PEREKL_BEGIN;
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_CONTROL_ZNKh)
                  {
                    edition_settings.control_znkh = current_settings.control_znkh;
                  }
                  else if (
                    (current_ekran.current_level >= EKRAN_SETPOINT_UMIN_GROUP1) &&
                    (current_ekran.current_level <= EKRAN_SETPOINT_UMIN_GROUP4))
                  {
                    int group = (current_ekran.current_level - EKRAN_SETPOINT_UMIN_GROUP1);

                    if (current_ekran.index_position == INDEX_ML_STPUmin_Umin1)
                    {
                      edition_settings.setpoint_Umin1[group] = current_settings.setpoint_Umin1[group];
                      current_ekran.position_cursor_x = COL_SETPOINT_Umin_Umin1_BEGIN;
                    }
                    else if (current_ekran.index_position == INDEX_ML_STPUmin_Umin2)
                    {
                      edition_settings.setpoint_Umin2[group] = current_settings.setpoint_Umin2[group];
                      current_ekran.position_cursor_x = COL_SETPOINT_Umin_Umin2_BEGIN;
                    }
                  }
                  else if (
                    (current_ekran.current_level >= EKRAN_TIMEOUT_UMIN_GROUP1) &&
                    (current_ekran.current_level <= EKRAN_TIMEOUT_UMIN_GROUP4))
                  {
                    int group = (current_ekran.current_level - EKRAN_TIMEOUT_UMIN_GROUP1);

                    if (current_ekran.index_position == INDEX_ML_TMOUmin_Umin1)
                    {
                      edition_settings.timeout_Umin1[group] = current_settings.timeout_Umin1[group];
                      current_ekran.position_cursor_x = COL_TMO_Umin_Umin1_BEGIN;
                    }
                    else if (current_ekran.index_position == INDEX_ML_TMOUmin_Umin2)
                    {
                      edition_settings.timeout_Umin2[group] = current_settings.timeout_Umin2[group];
                      current_ekran.position_cursor_x = COL_TMO_Umin_Umin2_BEGIN;
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_CONTROL_UMIN)
                  {
                    edition_settings.control_Umin = current_settings.control_Umin;
                  }
                  else if (
                    (current_ekran.current_level >= EKRAN_SETPOINT_UMAX_GROUP1) &&
                    (current_ekran.current_level <= EKRAN_SETPOINT_UMAX_GROUP4))
                  {
                    int group = (current_ekran.current_level - EKRAN_SETPOINT_UMAX_GROUP1);

                    if (current_ekran.index_position == INDEX_ML_STPUmax_Umax1)
                    {
                      edition_settings.setpoint_Umax1[group] = current_settings.setpoint_Umax1[group];
                      current_ekran.position_cursor_x = COL_SETPOINT_Umax_Umax1_BEGIN;
                    }
                    else if (current_ekran.index_position == INDEX_ML_STPUmax_Umax2)
                    {
                      edition_settings.setpoint_Umax2[group] = current_settings.setpoint_Umax2[group];
                      current_ekran.position_cursor_x = COL_SETPOINT_Umax_Umax2_BEGIN;
                    }
                  }
                  else if (
                    (current_ekran.current_level >= EKRAN_TIMEOUT_UMAX_GROUP1) &&
                    (current_ekran.current_level <= EKRAN_TIMEOUT_UMAX_GROUP4))
                  {
                    int group = (current_ekran.current_level - EKRAN_TIMEOUT_UMAX_GROUP1);

                    if (current_ekran.index_position == INDEX_ML_TMOUmax_Umax2)
                    {
                      edition_settings.timeout_Umax2[group] = current_settings.timeout_Umax2[group];
                      current_ekran.position_cursor_x = COL_TMO_Umax_Umax2_BEGIN;
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_CONTROL_UMAX)
                  {
                    edition_settings.control_Umax = current_settings.control_Umax;
                  }
                  else if (
                    (current_ekran.current_level >= EKRAN_SETPOINT_UP_GROUP1) &&
                    (current_ekran.current_level <= EKRAN_SETPOINT_UP_GROUP4))
                  {
                    int group = (current_ekran.current_level - EKRAN_SETPOINT_UP_GROUP1);

                    uint32_t _n_UP = current_ekran.index_position / MAX_ROW_FOR_SETPOINT_UP;
                    uint32_t _n_index = current_ekran.index_position % MAX_ROW_FOR_SETPOINT_UP;
                    if (_n_index == INDEX_ML_STP_UP)
                    {
                      edition_settings.setpoint_UP[_n_UP][0][group] = current_settings.setpoint_UP[_n_UP][0][group];
                      switch (current_settings.ctrl_UP_input[_n_UP])
                      {
                        case UP_CTRL_Ia_Ib_Ic:
                        case UP_CTRL_Ia:
                        case UP_CTRL_Ib:
                        case UP_CTRL_Ic:
                        case UP_CTRL_I1:
                        case UP_CTRL_I2:
                        case UP_CTRL_I04:
                        case UP_CTRL_3I0_r:
                          {
                            current_ekran.position_cursor_x = COL_SETPOINT_UP_I_BEGIN;
                            break;
                          }
                        case UP_CTRL_3I0:
                        case UP_CTRL_3I0_others:
                          {
                            current_ekran.position_cursor_x = COL_SETPOINT_UP_3I0_BEGIN;
                            break;
                          }
                        case UP_CTRL_Ua_Ub_Uc:
                        case UP_CTRL_Uab_Ubc_Uca:
                        case UP_CTRL_Ua:
                        case UP_CTRL_Uab:
                        case UP_CTRL_Ub:
                        case UP_CTRL_Ubc:
                        case UP_CTRL_Uc:
                        case UP_CTRL_Uca:
                        case UP_CTRL_U1:
                        case UP_CTRL_U2:
                        case UP_CTRL_3U0:
                          {
                            current_ekran.position_cursor_x = COL_SETPOINT_UP_U_BEGIN;
                            break;
                          }
                        case UP_CTRL_P:
                        case UP_CTRL_Q:
                          {
                            current_ekran.position_cursor_x = COL_SETPOINT_UP_PQ_BEGIN - 1;
                            break;
                          }
                        case UP_CTRL_S:
                          {
                            current_ekran.position_cursor_x = COL_SETPOINT_UP_S_BEGIN;
                            break;
                          }
                        default:
                          {
                            //Теоретично цього ніколи не мало б бути
                            total_error_sw_fixed();
                          }
                      }
                    }
                    else if (_n_index == INDEX_ML_STP_UP_KP)
                    {
                      edition_settings.setpoint_UP_KP[_n_UP][0][group] = current_settings.setpoint_UP_KP[_n_UP][0][group];
                      current_ekran.position_cursor_x = COL_SETPOINT_UP_KP_BEGIN;
                    }
                  }
                  else if (
                    (current_ekran.current_level >= EKRAN_TIMEOUT_UP_GROUP1) &&
                    (current_ekran.current_level <= EKRAN_TIMEOUT_UP_GROUP4))
                  {
                    int group = (current_ekran.current_level - EKRAN_TIMEOUT_UP_GROUP1);

                    uint32_t _n_UP = current_ekran.index_position / MAX_ROW_FOR_TIMEOUT_UP;
                    uint32_t _n_index = current_ekran.index_position % MAX_ROW_FOR_TIMEOUT_UP;
                    if (_n_index == INDEX_ML_TMOUP)
                    {
                      edition_settings.timeout_UP[_n_UP][0][group] = current_settings.timeout_UP[_n_UP][0][group];
                      current_ekran.position_cursor_x = COL_TMO_UP_BEGIN;
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_CONTROL_UP)
                  {
                    edition_settings.control_UP = current_settings.control_UP;
                    for (size_t i = 0; i < NUMBER_UP; i++)
                      edition_settings.ctrl_UP_input[i] = current_settings.ctrl_UP_input[i];
                  }
                  else if (current_ekran.current_level == EKRAN_TRANSFORMATOR_INFO)
                  {
                    if (current_ekran.index_position == INDEX_ML_TT1)
                    {
                      edition_settings.TCurrent1 = current_settings.TCurrent1;
                      current_ekran.position_cursor_x = COL_TT_BEGIN;
                    }
                    else if (current_ekran.index_position == INDEX_ML_TT2)
                    {
                      edition_settings.TCurrent2 = current_settings.TCurrent2;
                      current_ekran.position_cursor_x = COL_TT_BEGIN;
                    }
                    else if (current_ekran.index_position == INDEX_ML_TN1)
                    {
                      edition_settings.TVoltage1 = current_settings.TVoltage1;
                      current_ekran.position_cursor_x = COL_TN_BEGIN;
                    }
                    else
                    {
                      edition_settings.TVoltage2 = current_settings.TVoltage2;
                      current_ekran.position_cursor_x = COL_TN_BEGIN;
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_SETPOINT_SWITCH)
                  {
                    if (current_ekran.index_position == INDEX_ML_STPInom)
                    {
                      edition_settings.setpoint_Inom = current_settings.setpoint_Inom;
                      current_ekran.position_cursor_x = COL_TMO_SWCH_ON_BEGIN;
                    }
                    else if (current_ekran.index_position == INDEX_ML_STPRKS_Inom)
                    {
                      edition_settings.setpoint_r_kom_st_Inom = current_settings.setpoint_r_kom_st_Inom;
                      current_ekran.position_cursor_x = COL_SETPOINT_RKS_Inom_BEGIN;
                    }
                    else if (current_ekran.index_position == INDEX_ML_STPMInom_vymk)
                    {
                      edition_settings.setpoint_Inom_vymk = current_settings.setpoint_Inom_vymk;
                      current_ekran.position_cursor_x = COL_SETPOINT_Inom_vymk_BEGIN;
                    }
                    else if (current_ekran.index_position == INDEX_ML_STPRKS_Inom_vymk)
                    {
                      edition_settings.setpoint_r_kom_st_Inom_vymk = current_settings.setpoint_r_kom_st_Inom_vymk;
                      current_ekran.position_cursor_x = COL_SETPOINT_RKS_Inom_vymk_BEGIN;
                    }
                    else if (current_ekran.index_position == INDEX_ML_STPPOCHATKOVYJ_RESURS)
                    {
                      edition_settings.setpoint_pochatkovyj_resurs = current_settings.setpoint_pochatkovyj_resurs;
                      current_ekran.position_cursor_x = COL_SETPOINT_POCHATKOVYJ_RESURS_BEGIN;
                    }
                    else if (current_ekran.index_position == INDEX_ML_STPKRYTYCHNYJ_RESURS)
                    {
                      edition_settings.setpoint_krytychnyj_resurs = current_settings.setpoint_krytychnyj_resurs;
                      current_ekran.position_cursor_x = COL_SETPOINT_KRYTYCHNYJ_RESURS_BEGIN;
                    }
                    else if (current_ekran.index_position == INDEX_ML_STPPOCHATKOVA_K_VYMK)
                    {
                      edition_settings.setpoint_pochatkova_k_vymk = current_settings.setpoint_pochatkova_k_vymk;
                      current_ekran.position_cursor_x = COL_SETPOINT_POCHATKOVA_K_VYMK_BEGIN;
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_TIMEOUT_SWITCH)
                  {
                    if (current_ekran.index_position == INDEX_ML_TMOON)
                    {
                      edition_settings.timeout_swch_on = current_settings.timeout_swch_on;
                      current_ekran.position_cursor_x = COL_TMO_SWCH_ON_BEGIN;
                    }
                    else if (current_ekran.index_position == INDEX_ML_TMOOFF)
                    {
                      edition_settings.timeout_swch_off = current_settings.timeout_swch_off;
                      current_ekran.position_cursor_x = COL_TMO_SWCH_OFF_BEGIN;
                    }
                    else if (current_ekran.index_position == INDEX_ML_TMOUDL_BLK_ON)
                    {
                      edition_settings.timeout_swch_udl_blk_on = current_settings.timeout_swch_udl_blk_on;
                      current_ekran.position_cursor_x = COL_TMO_SWCH_UDL_BLK_ON_BEGIN;
                    }
                    else if (current_ekran.index_position == INDEX_ML_TMOPRYVODA_VV)
                    {
                      edition_settings.timeout_pryvoda_VV = current_settings.timeout_pryvoda_VV;
                      current_ekran.position_cursor_x = COL_TMO_PRYVODA_VV_BEGIN;
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_CONTROL_SWITCH)
                  {
                    edition_settings.control_switch = current_settings.control_switch;
                  }
                  else if (current_ekran.current_level == EKRAN_DOPUSK_DV_UVV)
                  {
                    edition_settings.dopusk_dv[current_ekran.index_position] = current_settings.dopusk_dv[current_ekran.index_position];
                    current_ekran.position_cursor_x = COL_DOPUSK_DV_BEGIN;
                  }
                  else if (current_ekran.current_level == EKRAN_TYPE_INPUT_UVV)
                  {
                    edition_settings.type_of_input = current_settings.type_of_input;
                  }
                  else if (current_ekran.current_level == EKRAN_TYPE_INPUT_SIGNAL_UVV)
                  {
                    edition_settings.type_of_input_signal = current_settings.type_of_input_signal;
                  }
                  else if (current_ekran.current_level == EKRAN_TYPE_OUTPUT_UVV)
                  {
                    edition_settings.type_of_output = current_settings.type_of_output;
                    edition_settings.type_of_output_modif = current_settings.type_of_output_modif;
                  }
                  else if (current_ekran.current_level == EKRAN_TYPE_LED_UVV)
                  {
                    edition_settings.type_of_led = current_settings.type_of_led;
                  }
                  else if (current_ekran.current_level == EKRAN_TYPE_BUTTON_UVV)
                  {
                    edition_settings.buttons_mode = current_settings.buttons_mode;
                  }
                  else if (current_ekran.current_level == EKRAN_ADDRESS_RS485)
                  {
                    edition_settings.address = current_settings.address;
                    current_ekran.position_cursor_x = COL_ADDRESS_BEGIN;
                  }
                  else if (current_ekran.current_level == EKRAN_VIEW_SPEED_RS485)
                  {
                    edition_settings.speed_RS485 = current_settings.speed_RS485;
                  }
                  else if (current_ekran.current_level == EKRAN_VIEW_PARE_RS485)
                  {
                    edition_settings.pare_bit_RS485 = current_settings.pare_bit_RS485;
                  }
                  else if (current_ekran.current_level == EKRAN_VIEW_STOP_BITS_RS485)
                  {
                    edition_settings.number_stop_bit_RS485 = current_settings.number_stop_bit_RS485;
                  }
                  else if (current_ekran.current_level == EKRAN_VIEW_TIMEOUT_RS485)
                  {
                    edition_settings.time_out_1_RS485 = current_settings.time_out_1_RS485;
                    current_ekran.position_cursor_x = COL_TIMEOUT_INTERFACE_BEGIN;
                  }
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                  else if (current_ekran.current_level == EKRAN_SETTING_NETWORK_LAYER_ETHERNET)
                  {
                    if (
                      (current_ekran.index_position == INDEX_ML_NL_IPV4) ||
                      (current_ekran.index_position == INDEX_ML_NL_GATEWAY))
                    {
                      uint16_t *point_target, *point_source;
                      if (current_ekran.index_position == INDEX_ML_NL_IPV4)
                      {
                        point_target = edition_settings.IP4;
                        point_source = current_settings.IP4;
                      }
                      else
                      {
                        point_target = edition_settings.gateway;
                        point_source = current_settings.gateway;
                      }
                      for (size_t i = 0; i < 4; i++)
                        point_target[i] = point_source[i];
                      current_ekran.position_cursor_x = COL_IP4_GATEWAY_BEGIN;
                    }
                    else if (current_ekran.index_position == INDEX_ML_NL_MASK)
                    {
                      edition_settings.mask = current_settings.mask;
                      current_ekran.position_cursor_x = COL_MASK_BEGIN;
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_SYNCHRO)
                  {
                    if (current_ekran.index_position == INDEX_ML_SYN_IPV4)
                    {
                      for (size_t i = 0; i < 4; i++)
                        edition_settings.IP_time_server[i] = current_settings.IP_time_server[i];
                      current_ekran.position_cursor_x = COL_IP4_SERVER_BEGIN;
                    }
                    else if (current_ekran.index_position == INDEX_ML_SYN_PORT)
                    {
                      edition_settings.port_time_server = current_settings.port_time_server;
                      current_ekran.position_cursor_x = COL_PORT_BEGIN;
                    }
                    else if (current_ekran.index_position == INDEX_ML_SYN_PERIOD)
                    {
                      edition_settings.period_sync = current_settings.period_sync;
                      current_ekran.position_cursor_x = COL_PERIOD_BEGIN;
                    }
                  }
#endif
                  else if (current_ekran.current_level == EKRAN_TIME_ZONE)
                  {
                    edition_settings.time_zone = current_settings.time_zone;
                    edition_settings.dst = current_settings.dst;
                  }
                  else if (current_ekran.current_level == EKRAN_DST_RULE)
                  {
                    if (position_in_current_level_menu[previous_level_in_current_level_menu[EKRAN_DST_RULE]] == INDEX_ML_CHDT_DST_ON)
                    {
                      edition_settings.dst_on_rule = current_settings.dst_on_rule;
                    }
                    else
                    {
                      edition_settings.dst_off_rule = current_settings.dst_off_rule;
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_GENERAL_PICKUPS_EL)
                  {
                    if (current_ekran.index_position == INDEX_ML_NUMBER_INERATION)
                    {
                      edition_settings.number_iteration_el = current_settings.number_iteration_el;
                      current_ekran.position_cursor_x = COL_NUMBER_INERATION_BEGIN;
                    }
                    //                  else if (current_ekran.index_position == INDEX_ML_NUMBER_DEFINED_FUNCTIONS)
                    //                  {
                    //                    edition_settings.number_defined_df = current_settings.number_defined_df;
                    //                    current_ekran.position_cursor_x = COL_NUMBER_DEFINED_FUNCTIONS_BEGIN;
                    //                  }
                    //                  else if (current_ekran.index_position == INDEX_ML_NUMBER_DEFINED_TRIGGERS)
                    //                  {
                    //                    edition_settings.number_defined_dt = current_settings.number_defined_dt;
                    //                    current_ekran.position_cursor_x = COL_NUMBER_DEFINED_TRIGGERS_BEGIN;
                    //                  }
                    //                  else if (current_ekran.index_position == INDEX_ML_NUMBER_DEFINED_AND)
                    //                  {
                    //                    edition_settings.number_defined_and = current_settings.number_defined_and;
                    //                    current_ekran.position_cursor_x = COL_NUMBER_DEFINED_AND_BEGIN;
                    //                  }
                    //                  else if (current_ekran.index_position == INDEX_ML_NUMBER_DEFINED_OR)
                    //                  {
                    //                    edition_settings.number_defined_or = current_settings.number_defined_or;
                    //                    current_ekran.position_cursor_x = COL_NUMBER_DEFINED_OR_BEGIN;
                    //                  }
                    //                  else if (current_ekran.index_position == INDEX_ML_NUMBER_DEFINED_XOR)
                    //                  {
                    //                    edition_settings.number_defined_xor = current_settings.number_defined_xor;
                    //                    current_ekran.position_cursor_x = COL_NUMBER_DEFINED_XOR_BEGIN;
                    //                  }
                    //                  else if (current_ekran.index_position == INDEX_ML_NUMBER_DEFINED_NOT)
                    //                  {
                    //                    edition_settings.number_defined_not = current_settings.number_defined_not;
                    //                    current_ekran.position_cursor_x = COL_NUMBER_DEFINED_NOT_BEGIN;
                    //                  }
                  }
                  else if (current_ekran.current_level == EKRAN_LIST_TYPE_DF)
                  {
                    edition_settings.type_df = current_settings.type_df;
                  }
                  else if ((current_ekran.current_level >= EKRAN_TIMEOUT_DF1) && (current_ekran.current_level <= (EKRAN_TIMEOUT_DF1 + NUMBER_DEFINED_FUNCTIONS - 1)))
                  {
                    if (current_ekran.index_position == INDEX_ML_TMO_DF_PAUSE)
                    {
                      edition_settings.timeout_pause_df[current_ekran.current_level - EKRAN_TIMEOUT_DF1] =
                        current_settings.timeout_pause_df[current_ekran.current_level - EKRAN_TIMEOUT_DF1];
                      current_ekran.position_cursor_x = COL_TMO_DF_PAUSE_BEGIN;
                    }
                    else
                    {
                      edition_settings.timeout_work_df[current_ekran.current_level - EKRAN_TIMEOUT_DF1] =
                        current_settings.timeout_work_df[current_ekran.current_level - EKRAN_TIMEOUT_DF1];
                      current_ekran.position_cursor_x = COL_TMO_DF_PAUSE_BEGIN;
                    }
                  }
                  else if ((current_ekran.current_level >= EKRAN_LIST_SOURCE_TF1) && (current_ekran.current_level <= EKRAN_LIST_SOURCE_TF1 + NUMBER_TRANSFER_FUNCTIONS - 1))
                  {
                    edition_settings.ranguvannja_tf[current_ekran.current_level - EKRAN_LIST_SOURCE_TF1] = current_settings.ranguvannja_tf[current_ekran.current_level - EKRAN_LIST_SOURCE_TF1];
                  }
                  else if (current_ekran.current_level == EKRAN_CONTROL_AR)
                  {
                    edition_settings.control_ar = current_settings.control_ar;
                  }
                  else if (current_ekran.current_level == EKRAN_TIMEOUT_DIGITAL_REGISTRATOR)
                  {
                    if (current_ekran.index_position == INDEX_ML_TMO_ELONGATION)
                    {
                      edition_settings.timeout_prolongation_work_digital_registrator = current_settings.timeout_prolongation_work_digital_registrator;
                      current_ekran.position_cursor_x = COL_TMO_ELONGATION_BEGIN;
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_TIMEOUT_ANALOG_REGISTRATOR)
                  {
                    if (current_ekran.index_position == INDEX_ML_TMOPREFAULT)
                    {
                      edition_settings.prefault_number_periods = current_settings.prefault_number_periods;
                      current_ekran.position_cursor_x = COL_TMO_PREFAULT_BEGIN;
                    }
                    else
                    {
                      edition_settings.postfault_number_periods = current_settings.postfault_number_periods;
                      current_ekran.position_cursor_x = COL_TMO_POSTFAULT_BEGIN;
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_VIEW_SETTING_LANGUAGE)
                  {
                    edition_settings.language = current_settings.language;
                  }
                  else if (current_ekran.current_level == EKRAN_CHOSE_EXTRA_SETTINGS)
                  {
                    edition_settings.control_extra_settings_1 = current_settings.control_extra_settings_1;
                  }
                  else if (current_ekran.current_level == EKRAN_VIEW_GRUPA_USTAVOK)
                  {
                    edition_settings.grupa_ustavok = current_settings.grupa_ustavok;
                  }

                  //Підготовка до режиму редагування - включаємо мигаючий курсор
                  current_ekran.cursor_on = 1;
                  current_ekran.cursor_blinking_on = 1;
                  if (current_settings.password1 != 0)
                  {
                    //Переходимо на меню запиту паролю
                    current_ekran.current_level = EKRAN_LEVEL_PASSWORD;
                    previous_level_in_current_level_menu[current_ekran.current_level] = temp_current_level;
                    current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  }
                  else
                  {
                    //Переходимо у режим редагування
                    current_ekran.edition = 1;
                  }
                }
                else if (current_ekran.edition == 1)
                {
                  //Перевіряємо чи якісь зміни відбулися
                  unsigned int found_changes = 0;

                  if (
                    (current_ekran.current_level >= EKRAN_SETPOINT_RPN_GROUP1) &&
                    (current_ekran.current_level <= EKRAN_SETPOINT_RPN_GROUP4))
                  {
                    int group = (current_ekran.current_level - EKRAN_SETPOINT_RPN_GROUP1);

                    if (current_ekran.index_position == INDEX_ML_STPRPN_OSN)
                    {
                      if (edition_settings.setpoint_rpn_osn[group] != current_settings.setpoint_rpn_osn[group])
                        found_changes = 1;
                    }
                    else if (current_ekran.index_position == INDEX_ML_STPRPN_ZONE)
                    {
                      if (edition_settings.setpoint_rpn_zony[group] != current_settings.setpoint_rpn_zony[group])
                        found_changes = 1;
                    }
                    else if (current_ekran.index_position == INDEX_ML_STPRPN_MAX_PER)
                    {
                      if (edition_settings.setpoint_rpn_per[group] != current_settings.setpoint_rpn_per[group])
                        found_changes = 1;
                    }
                    else if (current_ekran.index_position == INDEX_ML_STPRPN_DOD)
                    {
                      if (edition_settings.setpoint_rpn_dod[group] != current_settings.setpoint_rpn_dod[group])
                        found_changes = 1;
                    }
                    else if (current_ekran.index_position == INDEX_ML_STPRPN_K)
                    {
                      if (edition_settings.setpoint_rpn_K[group] != current_settings.setpoint_rpn_K[group])
                        found_changes = 1;
                    }
                  }
                  else if (
                    (current_ekran.current_level >= EKRAN_TIMEOUT_RPN_GROUP1) &&
                    (current_ekran.current_level <= EKRAN_TIMEOUT_RPN_GROUP4))
                  {
                    int group = (current_ekran.current_level - EKRAN_TIMEOUT_RPN_GROUP1);

                    if (current_ekran.index_position == INDEX_ML_TMORPN_UB_PRYB)
                    {
                      if (edition_settings.timeout_rpn_ub_pryb[group] != current_settings.timeout_rpn_ub_pryb[group])
                        found_changes = 1;
                    }
                    else if (current_ekran.index_position == INDEX_ML_TMORPN_UB_PRYB_PRYSK)
                    {
                      if (edition_settings.timeout_rpn_ub_pryb_prysk[group] != current_settings.timeout_rpn_ub_pryb_prysk[group])
                        found_changes = 1;
                    }
                    else if (current_ekran.index_position == INDEX_ML_TMORPN_PRYSK_VID_UMAX)
                    {
                      if (edition_settings.timeout_rpn_prysk_vid_Umax[group] != current_settings.timeout_rpn_prysk_vid_Umax[group])
                        found_changes = 1;
                    }
                    else if (current_ekran.index_position == INDEX_ML_TMORPN_PEREKL)
                    {
                      if (edition_settings.timeout_rpn_perekl[group] != current_settings.timeout_rpn_perekl[group])
                        found_changes = 1;
                    }
                    else if (current_ekran.index_position == INDEX_ML_TMORPN_NESPR)
                    {
                      if (edition_settings.timeout_rpn_nespr[group] != current_settings.timeout_rpn_nespr[group])
                        found_changes = 1;
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_CONTROL_RPN)
                  {
                    if (edition_settings.control_rpn != current_settings.control_rpn)
                      found_changes = 1;
                  }
                  else if (current_ekran.current_level == EKRAN_CONTROL_SZKh)
                  {
                    if (edition_settings.control_zskh != current_settings.control_zskh)
                      found_changes = 1;
                  }

                  else if (
                    (current_ekran.current_level >= EKRAN_SETPOINT_BRP_GROUP1) &&
                    (current_ekran.current_level <= EKRAN_SETPOINT_BRP_GROUP4))
                  {
                    int group = (current_ekran.current_level - EKRAN_SETPOINT_BRP_GROUP1);

                    if (current_ekran.index_position == INDEX_ML_STPBRP_I_BLK)
                    {
                      if (edition_settings.setpoint_brp_I_blk[group] != current_settings.setpoint_brp_I_blk[group])
                        found_changes = 1;
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_CONTROL_BRP)
                  {
                    if (edition_settings.control_brp != current_settings.control_brp)
                      found_changes = 1;
                  }
                  else if (
                    (current_ekran.current_level >= EKRAN_TIMEOUT_ZNKh_GROUP1) &&
                    (current_ekran.current_level <= EKRAN_TIMEOUT_ZNKh_GROUP4))
                  {
                    int group = (current_ekran.current_level - EKRAN_TIMEOUT_ZNKh_GROUP1);

                    if (current_ekran.index_position == INDEX_ML_TMOZNKh_PEREKL)
                    {
                      if (edition_settings.timeout_znkh_perekl[group] != current_settings.timeout_znkh_perekl[group])
                        found_changes = 1;
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_CONTROL_ZNKh)
                  {
                    if (edition_settings.control_znkh != current_settings.control_znkh)
                      found_changes = 1;
                  }
                  else if (
                    (current_ekran.current_level >= EKRAN_SETPOINT_UMIN_GROUP1) &&
                    (current_ekran.current_level <= EKRAN_SETPOINT_UMIN_GROUP4))
                  {
                    int group = (current_ekran.current_level - EKRAN_SETPOINT_UMIN_GROUP1);

                    if (current_ekran.index_position == INDEX_ML_STPUmin_Umin1)
                    {
                      if (edition_settings.setpoint_Umin1[group] != current_settings.setpoint_Umin1[group])
                        found_changes = 1;
                    }
                    else if (current_ekran.index_position == INDEX_ML_STPUmin_Umin2)
                    {
                      if (edition_settings.setpoint_Umin2[group] != current_settings.setpoint_Umin2[group])
                        found_changes = 1;
                    }
                  }
                  else if (
                    (current_ekran.current_level >= EKRAN_TIMEOUT_UMIN_GROUP1) &&
                    (current_ekran.current_level <= EKRAN_TIMEOUT_UMIN_GROUP4))
                  {
                    int group = (current_ekran.current_level - EKRAN_TIMEOUT_UMIN_GROUP1);

                    if (current_ekran.index_position == INDEX_ML_TMOUmin_Umin1)
                    {
                      if (edition_settings.timeout_Umin1[group] != current_settings.timeout_Umin1[group])
                        found_changes = 1;
                    }
                    else if (current_ekran.index_position == INDEX_ML_TMOUmin_Umin2)
                    {
                      if (edition_settings.timeout_Umin2[group] != current_settings.timeout_Umin2[group])
                        found_changes = 1;
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_CONTROL_UMIN)
                  {
                    if (edition_settings.control_Umin != current_settings.control_Umin)
                      found_changes = 1;
                  }
                  else if (
                    (current_ekran.current_level >= EKRAN_SETPOINT_UMAX_GROUP1) &&
                    (current_ekran.current_level <= EKRAN_SETPOINT_UMAX_GROUP4))
                  {
                    int group = (current_ekran.current_level - EKRAN_SETPOINT_UMAX_GROUP1);

                    if (current_ekran.index_position == INDEX_ML_STPUmax_Umax1)
                    {
                      if (edition_settings.setpoint_Umax1[group] != current_settings.setpoint_Umax1[group])
                        found_changes = 1;
                    }
                    else if (current_ekran.index_position == INDEX_ML_STPUmax_Umax2)
                    {
                      if (edition_settings.setpoint_Umax2[group] != current_settings.setpoint_Umax2[group])
                        found_changes = 1;
                    }
                  }
                  else if (
                    (current_ekran.current_level >= EKRAN_TIMEOUT_UMAX_GROUP1) &&
                    (current_ekran.current_level <= EKRAN_TIMEOUT_UMAX_GROUP4))
                  {
                    int group = (current_ekran.current_level - EKRAN_TIMEOUT_UMAX_GROUP1);

                    if (current_ekran.index_position == INDEX_ML_TMOUmax_Umax2)
                    {
                      if (edition_settings.timeout_Umax2[group] != current_settings.timeout_Umax2[group])
                        found_changes = 1;
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_CONTROL_UMAX)
                  {
                    if (edition_settings.control_Umax != current_settings.control_Umax)
                      found_changes = 1;
                  }
                  else if (
                    (current_ekran.current_level >= EKRAN_SETPOINT_UP_GROUP1) &&
                    (current_ekran.current_level <= EKRAN_SETPOINT_UP_GROUP4))
                  {
                    int group = (current_ekran.current_level - EKRAN_SETPOINT_UP_GROUP1);

                    uint32_t _n_UP = current_ekran.index_position / MAX_ROW_FOR_SETPOINT_UP;
                    uint32_t _n_index = current_ekran.index_position % MAX_ROW_FOR_SETPOINT_UP;
                    if (_n_index == INDEX_ML_STP_UP)
                    {
                      if (edition_settings.setpoint_UP[_n_UP][0][group] != current_settings.setpoint_UP[_n_UP][0][group])
                        found_changes = 1;
                    }
                    else if (_n_index == INDEX_ML_STP_UP_KP)
                    {
                      if (edition_settings.setpoint_UP_KP[_n_UP][0][group] != current_settings.setpoint_UP_KP[_n_UP][0][group])
                        found_changes = 1;
                    }
                  }
                  else if (
                    (current_ekran.current_level >= EKRAN_TIMEOUT_UP_GROUP1) &&
                    (current_ekran.current_level <= EKRAN_TIMEOUT_UP_GROUP4))
                  {
                    int group = (current_ekran.current_level - EKRAN_TIMEOUT_UP_GROUP1);

                    uint32_t _n_UP = current_ekran.index_position / MAX_ROW_FOR_TIMEOUT_UP;
                    uint32_t _n_index = current_ekran.index_position % MAX_ROW_FOR_TIMEOUT_UP;
                    if (_n_index == INDEX_ML_TMOUP)
                    {
                      if (edition_settings.timeout_UP[_n_UP][0][group] != current_settings.timeout_UP[_n_UP][0][group])
                        found_changes = 1;
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_CONTROL_UP)
                  {
                    if (edition_settings.control_UP != current_settings.control_UP)
                      found_changes = 1;
                    else
                    {
                      for (size_t i = 0; ((i < NUMBER_UP) && (found_changes == 0)); i++)
                      {
                        if (edition_settings.ctrl_UP_input[i] != current_settings.ctrl_UP_input[i])
                          found_changes = 1;
                      }
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_TRANSFORMATOR_INFO)
                  {
                    if (current_ekran.index_position == INDEX_ML_TT1)
                    {
                      if (edition_settings.TCurrent1 != current_settings.TCurrent1)
                        found_changes = 1;
                    }
                    else if (current_ekran.index_position == INDEX_ML_TT2)
                    {
                      if (edition_settings.TCurrent2 != current_settings.TCurrent2)
                        found_changes = 1;
                    }
                    else if (current_ekran.index_position == INDEX_ML_TN1)
                    {
                      if (edition_settings.TVoltage1 != current_settings.TVoltage1)
                        found_changes = 1;
                    }
                    else
                    {
                      if (edition_settings.TVoltage2 != current_settings.TVoltage2)
                        found_changes = 1;
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_SETPOINT_SWITCH)
                  {
                    if (current_ekran.index_position == INDEX_ML_STPInom)
                    {
                      if (edition_settings.setpoint_Inom != current_settings.setpoint_Inom)
                        found_changes = 1;
                    }
                    else if (current_ekran.index_position == INDEX_ML_STPRKS_Inom)
                    {
                      if (edition_settings.setpoint_r_kom_st_Inom != current_settings.setpoint_r_kom_st_Inom)
                        found_changes = 1;
                    }
                    else if (current_ekran.index_position == INDEX_ML_STPMInom_vymk)
                    {
                      if (edition_settings.setpoint_Inom_vymk != current_settings.setpoint_Inom_vymk)
                        found_changes = 1;
                    }
                    else if (current_ekran.index_position == INDEX_ML_STPRKS_Inom_vymk)
                    {
                      if (edition_settings.setpoint_r_kom_st_Inom_vymk != current_settings.setpoint_r_kom_st_Inom_vymk)
                        found_changes = 1;
                    }
                    else if (current_ekran.index_position == INDEX_ML_STPPOCHATKOVYJ_RESURS)
                    {
                      if (edition_settings.setpoint_pochatkovyj_resurs != current_settings.setpoint_pochatkovyj_resurs)
                        found_changes = 1;
                    }
                    else if (current_ekran.index_position == INDEX_ML_STPKRYTYCHNYJ_RESURS)
                    {
                      if (edition_settings.setpoint_krytychnyj_resurs != current_settings.setpoint_krytychnyj_resurs)
                        found_changes = 1;
                    }
                    else if (current_ekran.index_position == INDEX_ML_STPPOCHATKOVA_K_VYMK)
                    {
                      if (edition_settings.setpoint_pochatkova_k_vymk != current_settings.setpoint_pochatkova_k_vymk)
                        found_changes = 1;
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_TIMEOUT_SWITCH)
                  {
                    if (current_ekran.index_position == INDEX_ML_TMOON)
                    {
                      if (edition_settings.timeout_swch_on != current_settings.timeout_swch_on)
                        found_changes = 1;
                    }
                    else if (current_ekran.index_position == INDEX_ML_TMOOFF)
                    {
                      if (edition_settings.timeout_swch_off != current_settings.timeout_swch_off)
                        found_changes = 1;
                    }
                    else if (current_ekran.index_position == INDEX_ML_TMOUDL_BLK_ON)
                    {
                      if (edition_settings.timeout_swch_udl_blk_on != current_settings.timeout_swch_udl_blk_on)
                        found_changes = 1;
                    }
                    else if (current_ekran.index_position == INDEX_ML_TMOPRYVODA_VV)
                    {
                      if (edition_settings.timeout_pryvoda_VV != current_settings.timeout_pryvoda_VV)
                        found_changes = 1;
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_CONTROL_SWITCH)
                  {
                    if (edition_settings.control_switch != current_settings.control_switch)
                      found_changes = 1;
                  }
                  else if (current_ekran.current_level == EKRAN_DOPUSK_DV_UVV)
                  {
                    if (edition_settings.dopusk_dv[current_ekran.index_position] != current_settings.dopusk_dv[current_ekran.index_position])
                      found_changes = 1;
                  }
                  else if (current_ekran.current_level == EKRAN_TYPE_INPUT_UVV)
                  {
                    if (edition_settings.type_of_input != current_settings.type_of_input)
                      found_changes = 1;
                  }
                  else if (current_ekran.current_level == EKRAN_TYPE_INPUT_SIGNAL_UVV)
                  {
                    if (edition_settings.type_of_input_signal != current_settings.type_of_input_signal)
                      found_changes = 1;
                  }
                  else if (current_ekran.current_level == EKRAN_TYPE_OUTPUT_UVV)
                  {
                    if (
                      (edition_settings.type_of_output != current_settings.type_of_output) ||
                      (edition_settings.type_of_output_modif != current_settings.type_of_output_modif))
                      found_changes = 1;
                  }
                  else if (current_ekran.current_level == EKRAN_TYPE_LED_UVV)
                  {
                    if (edition_settings.type_of_led != current_settings.type_of_led)
                      found_changes = 1;
                  }
                  else if (current_ekran.current_level == EKRAN_TYPE_BUTTON_UVV)
                  {
                    if (edition_settings.buttons_mode != current_settings.buttons_mode)
                      found_changes = 1;
                  }
                  else if (current_ekran.current_level == EKRAN_ADDRESS_RS485)
                  {
                    unsigned int edit_value = edition_settings.address;
                    if (edit_value != current_settings.address)
                      found_changes = 1;
                  }
                  else if (current_ekran.current_level == EKRAN_VIEW_SPEED_RS485)
                  {
                    if (edition_settings.speed_RS485 != current_settings.speed_RS485)
                      found_changes = 1;
                  }
                  else if (current_ekran.current_level == EKRAN_VIEW_PARE_RS485)
                  {
                    if (edition_settings.pare_bit_RS485 != current_settings.pare_bit_RS485)
                      found_changes = 1;
                  }
                  else if (current_ekran.current_level == EKRAN_VIEW_STOP_BITS_RS485)
                  {
                    if (edition_settings.number_stop_bit_RS485 != current_settings.number_stop_bit_RS485)
                      found_changes = 1;
                  }
                  else if (current_ekran.current_level == EKRAN_VIEW_TIMEOUT_RS485)
                  {
                    if (edition_settings.time_out_1_RS485 != current_settings.time_out_1_RS485)
                      found_changes = 1;
                  }
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                  else if (current_ekran.current_level == EKRAN_SETTING_NETWORK_LAYER_ETHERNET)
                  {
                    if (
                      (current_ekran.index_position == INDEX_ML_NL_IPV4) ||
                      (current_ekran.index_position == INDEX_ML_NL_GATEWAY))
                    {
                      uint16_t *point_target, *point_source;
                      if (current_ekran.index_position == INDEX_ML_NL_IPV4)
                      {
                        point_target = edition_settings.IP4;
                        point_source = current_settings.IP4;
                      }
                      else
                      {
                        point_target = edition_settings.gateway;
                        point_source = current_settings.gateway;
                      }
                      for (size_t i = 0; i < 4; i++)
                      {
                        if (point_target[i] != point_source[i])
                        {
                          found_changes = 1;
                          break;
                        }
                      }
                    }
                    else if (current_ekran.index_position == INDEX_ML_NL_MASK)
                    {
                      if (edition_settings.mask != current_settings.mask)
                        found_changes = 1;
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_SYNCHRO)
                  {
                    if (current_ekran.index_position == INDEX_ML_SYN_IPV4)
                    {
                      for (size_t i = 0; i < 4; i++)
                      {
                        if (edition_settings.IP_time_server[i] != current_settings.IP_time_server[i])
                        {
                          found_changes = 1;
                          break;
                        }
                      }
                    }
                    else if (current_ekran.index_position == INDEX_ML_SYN_PORT)
                    {
                      if (edition_settings.port_time_server != current_settings.port_time_server)
                        found_changes = 1;
                    }
                    else if (current_ekran.index_position == INDEX_ML_SYN_PERIOD)
                    {
                      if (edition_settings.period_sync != current_settings.period_sync)
                        found_changes = 1;
                    }
                  }
#endif
                  else if (current_ekran.current_level == EKRAN_TIME_ZONE)
                  {
                    if (
                      (edition_settings.time_zone != current_settings.time_zone) ||
                      (edition_settings.dst != current_settings.dst))
                      found_changes = 1;
                  }
                  else if (current_ekran.current_level == EKRAN_DST_RULE)
                  {
                    if (position_in_current_level_menu[previous_level_in_current_level_menu[EKRAN_DST_RULE]] == INDEX_ML_CHDT_DST_ON)
                    {
                      if (edition_settings.dst_on_rule != current_settings.dst_on_rule)
                        found_changes = 1;
                    }
                    else
                    {
                      if (edition_settings.dst_off_rule != current_settings.dst_off_rule)
                        found_changes = 1;
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_GENERAL_PICKUPS_EL)
                  {
                    if (current_ekran.index_position == INDEX_ML_NUMBER_INERATION)
                    {
                      if (edition_settings.number_iteration_el != current_settings.number_iteration_el)
                        found_changes = 1;
                    }
                    //                  else if (current_ekran.index_position == INDEX_ML_NUMBER_DEFINED_FUNCTIONS)
                    //                  {
                    //                    if (edition_settings.number_defined_df != current_settings.number_defined_df) found_changes = 1;
                    //                  }
                    //                  else if (current_ekran.index_position == INDEX_ML_NUMBER_DEFINED_TRIGGERS)
                    //                  {
                    //                    if (edition_settings.number_defined_dt != current_settings.number_defined_dt) found_changes = 1;
                    //                  }
                    //                  else if (current_ekran.index_position == INDEX_ML_NUMBER_DEFINED_AND)
                    //                  {
                    //                    if (edition_settings.number_defined_and != current_settings.number_defined_and) found_changes = 1;
                    //                  }
                    //                  else if (current_ekran.index_position == INDEX_ML_NUMBER_DEFINED_OR)
                    //                  {
                    //                    if (edition_settings.number_defined_or != current_settings.number_defined_or) found_changes = 1;
                    //                  }
                    //                  else if (current_ekran.index_position == INDEX_ML_NUMBER_DEFINED_XOR)
                    //                  {
                    //                    if (edition_settings.number_defined_xor != current_settings.number_defined_xor) found_changes = 1;
                    //                  }
                    //                  else if (current_ekran.index_position == INDEX_ML_NUMBER_DEFINED_NOT)
                    //                  {
                    //                    if (edition_settings.number_defined_not != current_settings.number_defined_not) found_changes = 1;
                    //                  }
                  }
                  else if (current_ekran.current_level == EKRAN_LIST_TYPE_DF)
                  {
                    if (edition_settings.type_df != current_settings.type_df)
                      found_changes = 1;
                  }
                  else if ((current_ekran.current_level >= EKRAN_TIMEOUT_DF1) && (current_ekran.current_level <= (EKRAN_TIMEOUT_DF1 + NUMBER_DEFINED_FUNCTIONS - 1)))
                  {
                    if (current_ekran.index_position == INDEX_ML_TMO_DF_PAUSE)
                    {
                      if (edition_settings.timeout_pause_df[current_ekran.current_level - EKRAN_TIMEOUT_DF1] !=
                          current_settings.timeout_pause_df[current_ekran.current_level - EKRAN_TIMEOUT_DF1])
                        found_changes = 1;
                    }
                    else
                    {
                      if (edition_settings.timeout_work_df[current_ekran.current_level - EKRAN_TIMEOUT_DF1] !=
                          current_settings.timeout_work_df[current_ekran.current_level - EKRAN_TIMEOUT_DF1])
                        found_changes = 1;
                    }
                  }
                  else if ((current_ekran.current_level >= EKRAN_LIST_SOURCE_TF1) && (current_ekran.current_level <= (EKRAN_LIST_SOURCE_TF1 + NUMBER_TRANSFER_FUNCTIONS - 1)))
                  {
                    if (edition_settings.ranguvannja_tf[current_ekran.current_level - EKRAN_LIST_SOURCE_TF1] != current_settings.ranguvannja_tf[current_ekran.current_level - EKRAN_LIST_SOURCE_TF1])
                      found_changes = 1;
                  }
                  else if (current_ekran.current_level == EKRAN_TIMEOUT_DIGITAL_REGISTRATOR)
                  {
                    if (current_ekran.index_position == INDEX_ML_TMO_ELONGATION)
                    {
                      if (edition_settings.timeout_prolongation_work_digital_registrator != current_settings.timeout_prolongation_work_digital_registrator)
                        found_changes = 1;
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_TIMEOUT_ANALOG_REGISTRATOR)
                  {
                    if (current_ekran.index_position == INDEX_ML_TMOPREFAULT)
                    {
                      if (edition_settings.prefault_number_periods != current_settings.prefault_number_periods)
                        found_changes = 1;
                    }
                    else
                    {
                      if (edition_settings.postfault_number_periods != current_settings.postfault_number_periods)
                        found_changes = 1;
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_CONTROL_AR)
                  {
                    if (edition_settings.control_ar != current_settings.control_ar)
                      found_changes = 1;
                  }
                  else if (current_ekran.current_level == EKRAN_VIEW_SETTING_LANGUAGE)
                  {
                    if (edition_settings.language != current_settings.language)
                      found_changes = 1;
                  }
                  else if (current_ekran.current_level == EKRAN_CHOSE_EXTRA_SETTINGS)
                  {
                    if (edition_settings.control_extra_settings_1 != current_settings.control_extra_settings_1)
                      found_changes = 1;
                  }
                  else if (current_ekran.current_level == EKRAN_VIEW_GRUPA_USTAVOK)
                  {
                    if (edition_settings.grupa_ustavok != current_settings.grupa_ustavok)
                      found_changes = 1;
                  }

                  //Виходимо з режиму редагування
                  if (found_changes == 0)
                    current_ekran.edition = 0;
                  else
                    current_ekran.edition = 2;

                  current_ekran.cursor_on = 0;
                  current_ekran.cursor_blinking_on = 0;
                }
                else if (current_ekran.edition == 2)
                {
                  //Попередньо виставляємо повідомлення, що дані не достовірні
                  current_ekran.edition = 3;
                  //Перевіряємо достовірність даних
                  if (
                    (current_ekran.current_level >= EKRAN_SETPOINT_RPN_GROUP1) &&
                    (current_ekran.current_level <= EKRAN_SETPOINT_RPN_GROUP4))
                  {
                    int group = (current_ekran.current_level - EKRAN_SETPOINT_RPN_GROUP1);

                    if (current_ekran.index_position == INDEX_ML_STPRPN_OSN)
                    {
                      if (check_data_setpoint(edition_settings.setpoint_rpn_osn[group], SETPOINT_RPN_OSN_MIN, SETPOINT_RPN_OSN_MAX) == 1)
                      {
                        if (edition_settings.setpoint_rpn_osn[group] != current_settings.setpoint_rpn_osn[group])
                        {
                          //Помічаємо, що поле структури зараз буде змінене
                          changed_settings = CHANGED_ETAP_EXECUTION;

                          current_settings.setpoint_rpn_osn[group] = edition_settings.setpoint_rpn_osn[group];
                          //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                          fix_change_settings(0, 1);
                        }
                        //Виходимо з режиму редагування
                        current_ekran.edition = 0;
                      }
                    }
                    else if (current_ekran.index_position == INDEX_ML_STPRPN_ZONE)
                    {
                      if (check_data_setpoint(edition_settings.setpoint_rpn_zony[group], SETPOINT_RPN_ZONY_MIN, SETPOINT_RPN_ZONY_MAX) == 1)
                      {
                        if (edition_settings.setpoint_rpn_zony[group] != current_settings.setpoint_rpn_zony[group])
                        {
                          //Помічаємо, що поле структури зараз буде змінене
                          changed_settings = CHANGED_ETAP_EXECUTION;

                          current_settings.setpoint_rpn_zony[group] = edition_settings.setpoint_rpn_zony[group];
                          //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                          fix_change_settings(0, 1);
                        }
                        //Виходимо з режиму редагування
                        current_ekran.edition = 0;
                      }
                    }
                    else if (current_ekran.index_position == INDEX_ML_STPRPN_MAX_PER)
                    {
                      unsigned int value = edition_settings.setpoint_rpn_per[group];
                      if (
                        ((value >= SETPOINT_RPN_PER_MIN) && (value <= SETPOINT_RPN_PER_MAX)) ||
                        (value == SETPOINT_RPN_PER_UNLIMITED))
                      {
                        if (value != current_settings.setpoint_rpn_per[group])
                        {
                          //Помічаємо, що поле структури зараз буде змінене
                          changed_settings = CHANGED_ETAP_EXECUTION;

                          current_settings.setpoint_rpn_per[group] = value;
                          //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                          fix_change_settings(0, 1);
                        }
                        //Виходимо з режиму редагування
                        current_ekran.edition = 0;
                      }
                    }
                    else if (current_ekran.index_position == INDEX_ML_STPRPN_DOD)
                    {
                      if (check_data_setpoint(edition_settings.setpoint_rpn_dod[group], SETPOINT_RPN_DOD_MIN, SETPOINT_RPN_DOD_MAX) == 1)
                      {
                        if (edition_settings.setpoint_rpn_dod[group] != current_settings.setpoint_rpn_dod[group])
                        {
                          //Помічаємо, що поле структури зараз буде змінене
                          changed_settings = CHANGED_ETAP_EXECUTION;

                          current_settings.setpoint_rpn_dod[group] = edition_settings.setpoint_rpn_dod[group];
                          //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                          fix_change_settings(0, 1);
                        }
                        //Виходимо з режиму редагування
                        current_ekran.edition = 0;
                      }
                    }
                    else if (current_ekran.index_position == INDEX_ML_STPRPN_K)
                    {
                      if (check_data_setpoint(edition_settings.setpoint_rpn_K[group], SETPOINT_RPN_K_MIN, SETPOINT_RPN_K_MAX) == 1)
                      {
                        if (edition_settings.setpoint_rpn_K[group] != current_settings.setpoint_rpn_K[group])
                        {
                          //Помічаємо, що поле структури зараз буде змінене
                          changed_settings = CHANGED_ETAP_EXECUTION;

                          current_settings.setpoint_rpn_K[group] = edition_settings.setpoint_rpn_K[group];
                          //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                          fix_change_settings(0, 1);
                        }
                        //Виходимо з режиму редагування
                        current_ekran.edition = 0;
                      }
                    }
                  }
                  else if (
                    (current_ekran.current_level >= EKRAN_TIMEOUT_RPN_GROUP1) &&
                    (current_ekran.current_level <= EKRAN_TIMEOUT_RPN_GROUP4))
                  {
                    int group = (current_ekran.current_level - EKRAN_TIMEOUT_RPN_GROUP1);

                    if (current_ekran.index_position == INDEX_ML_TMORPN_UB_PRYB)
                    {
                      if (check_data_setpoint(edition_settings.timeout_rpn_ub_pryb[group], TIMEOUT_RPN_UB_PRYB_MIN, TIMEOUT_RPN_UB_PRYB_MAX) == 1)
                      {
                        if (edition_settings.timeout_rpn_ub_pryb[group] != current_settings.timeout_rpn_ub_pryb[group])
                        {
                          //Помічаємо, що поле структури зараз буде змінене
                          changed_settings = CHANGED_ETAP_EXECUTION;

                          current_settings.timeout_rpn_ub_pryb[group] = edition_settings.timeout_rpn_ub_pryb[group];
                          //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                          fix_change_settings(0, 1);
                        }
                        //Виходимо з режиму редагування
                        current_ekran.edition = 0;
                      }
                    }
                    else if (current_ekran.index_position == INDEX_ML_TMORPN_UB_PRYB_PRYSK)
                    {
                      if (check_data_setpoint(edition_settings.timeout_rpn_ub_pryb_prysk[group], TIMEOUT_RPN_UB_PRYB_PRYSK_MIN, TIMEOUT_RPN_UB_PRYB_PRYSK_MAX) == 1)
                      {
                        if (edition_settings.timeout_rpn_ub_pryb_prysk[group] != current_settings.timeout_rpn_ub_pryb_prysk[group])
                        {
                          //Помічаємо, що поле структури зараз буде змінене
                          changed_settings = CHANGED_ETAP_EXECUTION;

                          current_settings.timeout_rpn_ub_pryb_prysk[group] = edition_settings.timeout_rpn_ub_pryb_prysk[group];
                          //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                          fix_change_settings(0, 1);
                        }
                        //Виходимо з режиму редагування
                        current_ekran.edition = 0;
                      }
                    }
                    else if (current_ekran.index_position == INDEX_ML_TMORPN_PRYSK_VID_UMAX)
                    {
                      if (check_data_setpoint(edition_settings.timeout_rpn_prysk_vid_Umax[group], TIMEOUT_RPN_UB_PRYSK_VID_UMAX_MIN, TIMEOUT_RPN_UB_PRYSK_VID_UMAX_MAX) == 1)
                      {
                        if (edition_settings.timeout_rpn_prysk_vid_Umax[group] != current_settings.timeout_rpn_prysk_vid_Umax[group])
                        {
                          //Помічаємо, що поле структури зараз буде змінене
                          changed_settings = CHANGED_ETAP_EXECUTION;

                          current_settings.timeout_rpn_prysk_vid_Umax[group] = edition_settings.timeout_rpn_prysk_vid_Umax[group];
                          //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                          fix_change_settings(0, 1);
                        }
                        //Виходимо з режиму редагування
                        current_ekran.edition = 0;
                      }
                    }
                    else if (current_ekran.index_position == INDEX_ML_TMORPN_PEREKL)
                    {
                      if (check_data_setpoint(edition_settings.timeout_rpn_perekl[group], TIMEOUT_RPN_PEREKL_MIN, TIMEOUT_RPN_PEREKL_MAX) == 1)
                      {
                        if (edition_settings.timeout_rpn_perekl[group] != current_settings.timeout_rpn_perekl[group])
                        {
                          //Помічаємо, що поле структури зараз буде змінене
                          changed_settings = CHANGED_ETAP_EXECUTION;

                          current_settings.timeout_rpn_perekl[group] = edition_settings.timeout_rpn_perekl[group];
                          //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                          fix_change_settings(0, 1);
                        }
                        //Виходимо з режиму редагування
                        current_ekran.edition = 0;
                      }
                    }
                    else if (current_ekran.index_position == INDEX_ML_TMORPN_NESPR)
                    {
                      if (check_data_setpoint(edition_settings.timeout_rpn_nespr[group], TIMEOUT_RPN_NESPR_MIN, TIMEOUT_RPN_NESPR_MAX) == 1)
                      {
                        if (edition_settings.timeout_rpn_nespr[group] != current_settings.timeout_rpn_nespr[group])
                        {
                          //Помічаємо, що поле структури зараз буде змінене
                          changed_settings = CHANGED_ETAP_EXECUTION;

                          current_settings.timeout_rpn_nespr[group] = edition_settings.timeout_rpn_nespr[group];
                          //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                          fix_change_settings(0, 1);
                        }
                        //Виходимо з режиму редагування
                        current_ekran.edition = 0;
                      }
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_CONTROL_RPN)
                  {
                    if ((edition_settings.control_rpn & ((unsigned int) (~CTR_RPN_MASKA))) == 0)
                    {
                      if (edition_settings.control_rpn != current_settings.control_rpn)
                      {
                        //Помічаємо, що поле структури зараз буде змінене
                        changed_settings = CHANGED_ETAP_EXECUTION;

                        current_settings.control_rpn = edition_settings.control_rpn;
                        //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                        fix_change_settings(0, 1);
                      }
                      //Виходимо з режиму редагування
                      current_ekran.edition = 0;
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_CONTROL_SZKh)
                  {
                    if ((edition_settings.control_zskh & ((unsigned int) (~CTR_MTZ04_MASKA))) == 0)
                    {
                      if (edition_settings.control_zskh != current_settings.control_zskh)
                      {
                        //Помічаємо, що поле структури зараз буде змінене
                        changed_settings = CHANGED_ETAP_EXECUTION;

                        current_settings.control_zskh = edition_settings.control_zskh;
                        //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                        fix_change_settings(0, 1);
                      }
                      //Виходимо з режиму редагування
                      current_ekran.edition = 0;
                    }
                  }
                  else if (
                    (current_ekran.current_level >= EKRAN_SETPOINT_BRP_GROUP1) &&
                    (current_ekran.current_level <= EKRAN_SETPOINT_BRP_GROUP4))
                  {
                    int group = (current_ekran.current_level - EKRAN_SETPOINT_BRP_GROUP1);

                    if (current_ekran.index_position == INDEX_ML_STPBRP_I_BLK)
                    {
                      if (check_data_setpoint(edition_settings.setpoint_brp_I_blk[group], SETPOINT_BRP_I_BLK_MIN, SETPOINT_BRP_I_BLK_MAX) == 1)
                      {
                        if (edition_settings.setpoint_brp_I_blk[group] != current_settings.setpoint_brp_I_blk[group])
                        {
                          //Помічаємо, що поле структури зараз буде змінене
                          changed_settings = CHANGED_ETAP_EXECUTION;

                          current_settings.setpoint_brp_I_blk[group] = edition_settings.setpoint_brp_I_blk[group];
                          //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                          fix_change_settings(0, 1);
                        }
                        //Виходимо з режиму редагування
                        current_ekran.edition = 0;
                      }
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_CONTROL_BRP)
                  {
                    if ((edition_settings.control_brp & ((unsigned int) (~CTR_BRP_MASKA))) == 0)
                    {
                      if (edition_settings.control_brp != current_settings.control_brp)
                      {
                        //Помічаємо, що поле структури зараз буде змінене
                        changed_settings = CHANGED_ETAP_EXECUTION;

                        current_settings.control_brp = edition_settings.control_brp;
                        //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                        fix_change_settings(0, 1);
                      }
                      //Виходимо з режиму редагування
                      current_ekran.edition = 0;
                    }
                  }
                  else if (
                    (current_ekran.current_level >= EKRAN_TIMEOUT_ZNKh_GROUP1) &&
                    (current_ekran.current_level <= EKRAN_TIMEOUT_ZNKh_GROUP4))
                  {
                    int group = (current_ekran.current_level - EKRAN_TIMEOUT_ZNKh_GROUP1);

                    if (current_ekran.index_position == INDEX_ML_TMOZNKh_PEREKL)
                    {
                      if (check_data_setpoint(edition_settings.timeout_znkh_perekl[group], TIMEOUT_ZNKh_PEREKL_MIN, TIMEOUT_ZNKh_PEREKL_MAX) == 1)
                      {
                        if (edition_settings.timeout_znkh_perekl[group] != current_settings.timeout_znkh_perekl[group])
                        {
                          //Помічаємо, що поле структури зараз буде змінене
                          changed_settings = CHANGED_ETAP_EXECUTION;

                          current_settings.timeout_znkh_perekl[group] = edition_settings.timeout_znkh_perekl[group];
                          //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                          fix_change_settings(0, 1);
                        }
                        //Виходимо з режиму редагування
                        current_ekran.edition = 0;
                      }
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_CONTROL_ZNKh)
                  {
                    if ((edition_settings.control_znkh & ((unsigned int) (~CTR_ZNKh_MASKA))) == 0)
                    {
                      if (edition_settings.control_znkh != current_settings.control_znkh)
                      {
                        //Помічаємо, що поле структури зараз буде змінене
                        changed_settings = CHANGED_ETAP_EXECUTION;

                        current_settings.control_znkh = edition_settings.control_znkh;

                        //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                        fix_change_settings(0, 1);
                      }
                      //Виходимо з режиму редагування
                      current_ekran.edition = 0;
                    }
                  }
                  else if (
                    (current_ekran.current_level >= EKRAN_SETPOINT_UMIN_GROUP1) &&
                    (current_ekran.current_level <= EKRAN_SETPOINT_UMIN_GROUP4))
                  {
                    int group = (current_ekran.current_level - EKRAN_SETPOINT_UMIN_GROUP1);

                    if (current_ekran.index_position == INDEX_ML_STPUmin_Umin1)
                    {
                      if (check_data_setpoint(edition_settings.setpoint_Umin1[group], SETPOINT_UMIN1_MIN, SETPOINT_UMIN1_MAX) == 1)
                      {
                        if (edition_settings.setpoint_Umin1[group] != current_settings.setpoint_Umin1[group])
                        {
                          //Помічаємо, що поле структури зараз буде змінене
                          changed_settings = CHANGED_ETAP_EXECUTION;

                          current_settings.setpoint_Umin1[group] = edition_settings.setpoint_Umin1[group];
                          //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                          fix_change_settings(0, 1);
                        }
                        //Виходимо з режиму редагування
                        current_ekran.edition = 0;
                      }
                    }
                    else if (current_ekran.index_position == INDEX_ML_STPUmin_Umin2)
                    {
                      if (check_data_setpoint(edition_settings.setpoint_Umin2[group], SETPOINT_UMIN2_MIN, SETPOINT_UMIN2_MAX) == 1)
                      {
                        if (edition_settings.setpoint_Umin2[group] != current_settings.setpoint_Umin2[group])
                        {
                          //Помічаємо, що поле структури зараз буде змінене
                          changed_settings = CHANGED_ETAP_EXECUTION;

                          current_settings.setpoint_Umin2[group] = edition_settings.setpoint_Umin2[group];
                          //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                          fix_change_settings(0, 1);
                        }
                        //Виходимо з режиму редагування
                        current_ekran.edition = 0;
                      }
                    }
                  }
                  else if (
                    (current_ekran.current_level >= EKRAN_TIMEOUT_UMIN_GROUP1) &&
                    (current_ekran.current_level <= EKRAN_TIMEOUT_UMIN_GROUP4))
                  {
                    int group = (current_ekran.current_level - EKRAN_TIMEOUT_UMIN_GROUP1);

                    if (current_ekran.index_position == INDEX_ML_TMOUmin_Umin1)
                    {
                      if (check_data_setpoint(edition_settings.timeout_Umin1[group], TIMEOUT_UMIN1_MIN, TIMEOUT_UMIN1_MAX) == 1)
                      {
                        if (edition_settings.timeout_Umin1[group] != current_settings.timeout_Umin1[group])
                        {
                          //Помічаємо, що поле структури зараз буде змінене
                          changed_settings = CHANGED_ETAP_EXECUTION;

                          current_settings.timeout_Umin1[group] = edition_settings.timeout_Umin1[group];
                          //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                          fix_change_settings(0, 1);
                        }
                        //Виходимо з режиму редагування
                        current_ekran.edition = 0;
                      }
                    }
                    else if (current_ekran.index_position == INDEX_ML_TMOUmin_Umin2)
                    {
                      if (check_data_setpoint(edition_settings.timeout_Umin2[group], TIMEOUT_UMIN2_MIN, TIMEOUT_UMIN2_MAX) == 1)
                      {
                        if (edition_settings.timeout_Umin2[group] != current_settings.timeout_Umin2[group])
                        {
                          //Помічаємо, що поле структури зараз буде змінене
                          changed_settings = CHANGED_ETAP_EXECUTION;

                          current_settings.timeout_Umin2[group] = edition_settings.timeout_Umin2[group];
                          //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                          fix_change_settings(0, 1);
                        }
                        //Виходимо з режиму редагування
                        current_ekran.edition = 0;
                      }
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_CONTROL_UMIN)
                  {
                    if ((edition_settings.control_Umin & ((unsigned int) (~CTR_UMIN_MASKA))) == 0)
                    {
                      if (edition_settings.control_Umin != current_settings.control_Umin)
                      {
                        //Помічаємо, що поле структури зараз буде змінене
                        changed_settings = CHANGED_ETAP_EXECUTION;

                        current_settings.control_Umin = edition_settings.control_Umin;
                        //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                        fix_change_settings(0, 1);
                      }
                      //Виходимо з режиму редагування
                      current_ekran.edition = 0;
                    }
                  }
                  else if (
                    (current_ekran.current_level >= EKRAN_SETPOINT_UMAX_GROUP1) &&
                    (current_ekran.current_level <= EKRAN_SETPOINT_UMAX_GROUP4))
                  {
                    int group = (current_ekran.current_level - EKRAN_SETPOINT_UMAX_GROUP1);

                    if (current_ekran.index_position == INDEX_ML_STPUmax_Umax1)
                    {
                      if (check_data_setpoint(edition_settings.setpoint_Umax1[group], SETPOINT_UMAX1_MIN, SETPOINT_UMAX1_MAX) == 1)
                      {
                        if (edition_settings.setpoint_Umax1[group] != current_settings.setpoint_Umax1[group])
                        {
                          //Помічаємо, що поле структури зараз буде змінене
                          changed_settings = CHANGED_ETAP_EXECUTION;

                          current_settings.setpoint_Umax1[group] = edition_settings.setpoint_Umax1[group];
                          //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                          fix_change_settings(0, 1);
                        }
                        //Виходимо з режиму редагування
                        current_ekran.edition = 0;
                      }
                    }
                    else if (current_ekran.index_position == INDEX_ML_STPUmax_Umax2)
                    {
                      if (check_data_setpoint(edition_settings.setpoint_Umax2[group], SETPOINT_UMAX2_MIN, SETPOINT_UMAX2_MAX) == 1)
                      {
                        if (edition_settings.setpoint_Umax2[group] != current_settings.setpoint_Umax2[group])
                        {
                          //Помічаємо, що поле структури зараз буде змінене
                          changed_settings = CHANGED_ETAP_EXECUTION;

                          current_settings.setpoint_Umax2[group] = edition_settings.setpoint_Umax2[group];
                          //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                          fix_change_settings(0, 1);
                        }
                        //Виходимо з режиму редагування
                        current_ekran.edition = 0;
                      }
                    }
                  }
                  else if (
                    (current_ekran.current_level >= EKRAN_TIMEOUT_UMAX_GROUP1) &&
                    (current_ekran.current_level <= EKRAN_TIMEOUT_UMAX_GROUP4))
                  {
                    int group = (current_ekran.current_level - EKRAN_TIMEOUT_UMAX_GROUP1);

                    if (current_ekran.index_position == INDEX_ML_TMOUmax_Umax2)
                    {
                      if (check_data_setpoint(edition_settings.timeout_Umax2[group], TIMEOUT_UMAX2_MIN, TIMEOUT_UMAX2_MAX) == 1)
                      {
                        if (edition_settings.timeout_Umax2[group] != current_settings.timeout_Umax2[group])
                        {
                          //Помічаємо, що поле структури зараз буде змінене
                          changed_settings = CHANGED_ETAP_EXECUTION;

                          current_settings.timeout_Umax2[group] = edition_settings.timeout_Umax2[group];
                          //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                          fix_change_settings(0, 1);
                        }
                        //Виходимо з режиму редагування
                        current_ekran.edition = 0;
                      }
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_CONTROL_UMAX)
                  {
                    if ((edition_settings.control_Umax & ((unsigned int) (~CTR_UMAX_MASKA))) == 0)
                    {
                      if (edition_settings.control_Umax != current_settings.control_Umax)
                      {
                        //Помічаємо, що поле структури зараз буде змінене
                        changed_settings = CHANGED_ETAP_EXECUTION;

                        current_settings.control_Umax = edition_settings.control_Umax;
                        //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                        fix_change_settings(0, 1);
                      }
                      //Виходимо з режиму редагування
                      current_ekran.edition = 0;
                    }
                  }
                  else if (
                    (current_ekran.current_level >= EKRAN_SETPOINT_UP_GROUP1) &&
                    (current_ekran.current_level <= EKRAN_SETPOINT_UP_GROUP4))
                  {
                    int group = (current_ekran.current_level - EKRAN_SETPOINT_UP_GROUP1);

                    uint32_t _n_UP = current_ekran.index_position / MAX_ROW_FOR_SETPOINT_UP;
                    uint32_t _n_index = current_ekran.index_position % MAX_ROW_FOR_SETPOINT_UP;
                    if (_n_index == INDEX_ML_STP_UP)
                    {
                      uint32_t min = 0, max = 0, PQ = false;
                      switch (current_settings.ctrl_UP_input[_n_UP])
                      {
                        case UP_CTRL_Ia_Ib_Ic:
                        case UP_CTRL_Ia:
                        case UP_CTRL_Ib:
                        case UP_CTRL_Ic:
                        case UP_CTRL_I1:
                        case UP_CTRL_I2:
                        case UP_CTRL_I04:
                        case UP_CTRL_3I0_r:
                          {
                            min = SETPOINT_UP_I_MIN;
                            max = SETPOINT_UP_I_MAX;
                            break;
                          }
                        case UP_CTRL_3I0:
                        case UP_CTRL_3I0_others:
                          {
                            min = SETPOINT_UP_3I0_MIN;
                            max = SETPOINT_UP_3I0_MAX;
                            break;
                          }
                        case UP_CTRL_Ua_Ub_Uc:
                        case UP_CTRL_Uab_Ubc_Uca:
                        case UP_CTRL_Ua:
                        case UP_CTRL_Uab:
                        case UP_CTRL_Ub:
                        case UP_CTRL_Ubc:
                        case UP_CTRL_Uc:
                        case UP_CTRL_Uca:
                        case UP_CTRL_U1:
                        case UP_CTRL_U2:
                        case UP_CTRL_3U0:
                          {
                            min = SETPOINT_UP_U_MIN;
                            max = SETPOINT_UP_U_MAX;
                            break;
                          }
                        case UP_CTRL_P:
                        case UP_CTRL_Q:
                          {
                            PQ = true;
                            min = SETPOINT_UP_P_MIN;
                            max = SETPOINT_UP_P_MAX;
                            break;
                          }
                        case UP_CTRL_S:
                          {
                            min = SETPOINT_UP_P_MIN;
                            max = SETPOINT_UP_P_MAX;
                            break;
                          }
                        default:
                          {
                            //Теоретично цього ніколи не мало б бути
                            total_error_sw_fixed();
                          }
                      }
                      if (
                        (
                          (PQ == false) &&
                          (check_data_setpoint(edition_settings.setpoint_UP[_n_UP][0][group], min, max) == 1)) ||
                        ((PQ == true) &&
                         (check_data_setpoint(abs(edition_settings.setpoint_UP[_n_UP][0][group]), min, max) == 1)))
                      {
                        if (edition_settings.setpoint_UP[_n_UP][0][group] != current_settings.setpoint_UP[_n_UP][0][group])
                        {
                          //Помічаємо, що поле структури зараз буде змінене
                          changed_settings = CHANGED_ETAP_EXECUTION;

                          current_settings.setpoint_UP[_n_UP][0][group] = edition_settings.setpoint_UP[_n_UP][0][group];
                          //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                          fix_change_settings(0, 1);
                        }
                        //Виходимо з режиму редагування
                        current_ekran.edition = 0;
                      }
                    }
                    else if (_n_index == INDEX_ML_STP_UP_KP)
                    {
                      uint32_t maska = MASKA_FOR_BIT(_n_UP * (_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_MORE_LESS_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I));
                      uint32_t min, max;
                      if ((current_settings.control_UP & maska) != 0)
                      {
                        min = SETPOINT_UP_KP_LESS_MIN;
                        max = SETPOINT_UP_KP_LESS_MAX;
                      }
                      else
                      {
                        min = SETPOINT_UP_KP_MORE_MIN;
                        max = SETPOINT_UP_KP_MORE_MAX;
                      }

                      if (check_data_setpoint(edition_settings.setpoint_UP_KP[_n_UP][0][group], min, max) == 1)
                      {
                        if (edition_settings.setpoint_UP_KP[_n_UP][0][group] != current_settings.setpoint_UP_KP[_n_UP][0][group])
                        {
                          //Помічаємо, що поле структури зараз буде змінене
                          changed_settings = CHANGED_ETAP_EXECUTION;

                          current_settings.setpoint_UP_KP[_n_UP][0][group] = edition_settings.setpoint_UP_KP[_n_UP][0][group];
                          //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                          fix_change_settings(0, 1);
                        }
                        //Виходимо з режиму редагування
                        current_ekran.edition = 0;
                      }
                    }
                  }
                  else if (
                    (current_ekran.current_level >= EKRAN_TIMEOUT_UP_GROUP1) &&
                    (current_ekran.current_level <= EKRAN_TIMEOUT_UP_GROUP4))
                  {
                    int group = (current_ekran.current_level - EKRAN_TIMEOUT_UP_GROUP1);

                    uint32_t _n_UP = current_ekran.index_position / MAX_ROW_FOR_TIMEOUT_UP;
                    uint32_t _n_index = current_ekran.index_position % MAX_ROW_FOR_TIMEOUT_UP;
                    if (_n_index == INDEX_ML_TMOUP)
                    {
                      if (check_data_setpoint(edition_settings.timeout_UP[_n_UP][0][group], TIMEOUT_UP_MIN, TIMEOUT_UP_MAX) == 1)
                      {
                        if (edition_settings.timeout_UP[_n_UP][0][group] != current_settings.timeout_UP[_n_UP][0][group])
                        {
                          //Помічаємо, що поле структури зараз буде змінене
                          changed_settings = CHANGED_ETAP_EXECUTION;

                          current_settings.timeout_UP[_n_UP][0][group] = edition_settings.timeout_UP[_n_UP][0][group];
                          //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                          fix_change_settings(0, 1);
                        }
                        //Виходимо з режиму редагування
                        current_ekran.edition = 0;
                      }
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_CONTROL_UP)
                  {
                    if (
                      ((edition_settings.control_UP & ((unsigned int) (~CTR_UP_MASKA))) == 0) &&
                      (edition_settings.ctrl_UP_input[0] < _UP_CTRL_NUMBER) &&
                      (edition_settings.ctrl_UP_input[1] < _UP_CTRL_NUMBER) &&
                      (edition_settings.ctrl_UP_input[2] < _UP_CTRL_NUMBER) &&
                      (edition_settings.ctrl_UP_input[3] < _UP_CTRL_NUMBER) &&
                      (edition_settings.ctrl_UP_input[4] < _UP_CTRL_NUMBER) &&
                      (edition_settings.ctrl_UP_input[5] < _UP_CTRL_NUMBER) &&
                      (edition_settings.ctrl_UP_input[6] < _UP_CTRL_NUMBER) &&
                      (edition_settings.ctrl_UP_input[7] < _UP_CTRL_NUMBER))
                    {
                      if (
                        (edition_settings.control_UP != current_settings.control_UP) ||
                        (edition_settings.ctrl_UP_input[0] != current_settings.ctrl_UP_input[0]) ||
                        (edition_settings.ctrl_UP_input[1] != current_settings.ctrl_UP_input[1]) ||
                        (edition_settings.ctrl_UP_input[2] != current_settings.ctrl_UP_input[2]) ||
                        (edition_settings.ctrl_UP_input[3] != current_settings.ctrl_UP_input[3]) ||
                        (edition_settings.ctrl_UP_input[4] != current_settings.ctrl_UP_input[4]) ||
                        (edition_settings.ctrl_UP_input[5] != current_settings.ctrl_UP_input[5]) ||
                        (edition_settings.ctrl_UP_input[6] != current_settings.ctrl_UP_input[6]) ||
                        (edition_settings.ctrl_UP_input[7] != current_settings.ctrl_UP_input[7]))
                      {
                        //Помічаємо, що поле структури зараз буде змінене
                        changed_settings = CHANGED_ETAP_EXECUTION;

                        for (size_t i = 0; i < NUMBER_UP; i++)
                        {
                          action_after_changing_input_UP(&current_settings, i, edition_settings.ctrl_UP_input[i]);
                        }
                        action_after_changing_ctrl_UP(&current_settings, edition_settings.control_UP);
                        //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                        fix_change_settings(0, 1);
                      }
                      //Виходимо з режиму редагування
                      current_ekran.edition = 0;
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_TRANSFORMATOR_INFO)
                  {
                    if (current_ekran.index_position == INDEX_ML_TT1)
                    {
                      if (check_data_setpoint(edition_settings.TCurrent1, KOEF_TT_MIN, KOEF_TT_MAX) == 1)
                      {
                        if (edition_settings.TCurrent1 != current_settings.TCurrent1)
                        {
                          //Помічаємо, що поле структури зараз буде змінене
                          changed_settings = CHANGED_ETAP_EXECUTION;

                          current_settings.TCurrent1 = edition_settings.TCurrent1;
                          //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                          fix_change_settings(0, 1);
                        }
                        //Виходимо з режиму редагування
                        current_ekran.edition = 0;
                      }
                    }
                    else if (current_ekran.index_position == INDEX_ML_TT2)
                    {
                      if (check_data_setpoint(edition_settings.TCurrent2, KOEF_TT_MIN, KOEF_TT_MAX) == 1)
                      {
                        if (edition_settings.TCurrent2 != current_settings.TCurrent2)
                        {
                          //Помічаємо, що поле структури зараз буде змінене
                          changed_settings = CHANGED_ETAP_EXECUTION;

                          current_settings.TCurrent2 = edition_settings.TCurrent2;
                          //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                          fix_change_settings(0, 1);
                        }
                        //Виходимо з режиму редагування
                        current_ekran.edition = 0;
                      }
                    }
                    else if (current_ekran.index_position == INDEX_ML_TN1)
                    {
                      if (check_data_setpoint(edition_settings.TVoltage1, KOEF_TN_MIN, KOEF_TN_MAX) == 1)
                      {
                        if (edition_settings.TVoltage1 != current_settings.TVoltage1)
                        {
                          //Помічаємо, що поле структури зараз буде змінене
                          changed_settings = CHANGED_ETAP_EXECUTION;

                          current_settings.TVoltage1 = edition_settings.TVoltage1;
                          //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                          fix_change_settings(0, 1);
                        }
                        //Виходимо з режиму редагування
                        current_ekran.edition = 0;
                      }
                    }
                    else
                    {
                      if (check_data_setpoint(edition_settings.TVoltage2, KOEF_TN_MIN, KOEF_TN_MAX) == 1)
                      {
                        if (edition_settings.TVoltage2 != current_settings.TVoltage2)
                        {
                          //Помічаємо, що поле структури зараз буде змінене
                          changed_settings = CHANGED_ETAP_EXECUTION;

                          current_settings.TVoltage2 = edition_settings.TVoltage2;
                          //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                          fix_change_settings(0, 1);
                        }
                        //Виходимо з режиму редагування
                        current_ekran.edition = 0;
                      }
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_SETPOINT_SWITCH)
                  {
                    if (current_ekran.index_position == INDEX_ML_STPInom)
                    {
                      if (check_data_setpoint(edition_settings.setpoint_Inom, SETPOINT_Inom_MIN, SETPOINT_Inom_MAX) == 1)
                      {
                        if (edition_settings.setpoint_Inom != current_settings.setpoint_Inom)
                        {
                          //Помічаємо, що поле структури зараз буде змінене
                          changed_settings = CHANGED_ETAP_EXECUTION;

                          current_settings.setpoint_Inom = edition_settings.setpoint_Inom;

                          //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                          fix_change_settings(0, 1);
                        }
                        //Виходимо з режиму редагування
                        current_ekran.edition = 0;
                      }
                    }
                    else if (current_ekran.index_position == INDEX_ML_STPRKS_Inom)
                    {
                      if (check_data_setpoint(edition_settings.setpoint_r_kom_st_Inom, SETPOINT_RKS_Inom_MIN, SETPOINT_RKS_Inom_MAX) == 1)
                      {
                        if (edition_settings.setpoint_r_kom_st_Inom != current_settings.setpoint_r_kom_st_Inom)
                        {
                          //Помічаємо, що поле структури зараз буде змінене
                          changed_settings = CHANGED_ETAP_EXECUTION;

                          current_settings.setpoint_r_kom_st_Inom = edition_settings.setpoint_r_kom_st_Inom;
                          //Проводимо потенйційну корекцію початкового і критичного значення ресурсу
                          action_after_changing_resurs_pickup(&current_settings);

                          //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                          fix_change_settings(0, 1);
                        }
                        //Виходимо з режиму редагування
                        current_ekran.edition = 0;
                      }
                    }
                    else if (current_ekran.index_position == INDEX_ML_STPMInom_vymk)
                    {
                      if (check_data_setpoint(edition_settings.setpoint_Inom_vymk, SETPOINT_Inom_vymk_MIN, SETPOINT_Inom_vymk_MAX) == 1)
                      {
                        if (edition_settings.setpoint_Inom_vymk != current_settings.setpoint_Inom_vymk)
                        {
                          //Помічаємо, що поле структури зараз буде змінене
                          changed_settings = CHANGED_ETAP_EXECUTION;

                          current_settings.setpoint_Inom_vymk = edition_settings.setpoint_Inom_vymk;

                          //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                          fix_change_settings(0, 1);
                        }
                        //Виходимо з режиму редагування
                        current_ekran.edition = 0;
                      }
                    }
                    else if (current_ekran.index_position == INDEX_ML_STPRKS_Inom_vymk)
                    {
                      if (check_data_setpoint(edition_settings.setpoint_r_kom_st_Inom_vymk, SETPOINT_RKS_Inom_vymk_MIN, SETPOINT_RKS_Inom_vymk_MAX) == 1)
                      {
                        if (edition_settings.setpoint_r_kom_st_Inom_vymk != current_settings.setpoint_r_kom_st_Inom_vymk)
                        {
                          //Помічаємо, що поле структури зараз буде змінене
                          changed_settings = CHANGED_ETAP_EXECUTION;

                          current_settings.setpoint_r_kom_st_Inom_vymk = edition_settings.setpoint_r_kom_st_Inom_vymk;
                          //Проводимо потенйційну корекцію початкового і критичного значення ресурсу
                          action_after_changing_resurs_pickup(&current_settings);

                          //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                          fix_change_settings(0, 1);
                        }
                        //Виходимо з режиму редагування
                        current_ekran.edition = 0;
                      }
                    }
                    else if (current_ekran.index_position == INDEX_ML_STPPOCHATKOVYJ_RESURS)
                    {
                      if (check_data_setpoint(edition_settings.setpoint_pochatkovyj_resurs, (2 * (current_settings.setpoint_r_kom_st_Inom / current_settings.setpoint_r_kom_st_Inom_vymk)), current_settings.setpoint_r_kom_st_Inom) == 1)
                      {
                        if (edition_settings.setpoint_pochatkovyj_resurs != current_settings.setpoint_pochatkovyj_resurs)
                        {
                          //Помічаємо, що поле структури зараз буде змінене
                          changed_settings = CHANGED_ETAP_EXECUTION;

                          current_settings.setpoint_pochatkovyj_resurs = edition_settings.setpoint_pochatkovyj_resurs;
                          //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                          fix_change_settings(0, 1);
                        }
                        //Виходимо з режиму редагування
                        current_ekran.edition = 0;
                      }
                    }
                    else if (current_ekran.index_position == INDEX_ML_STPKRYTYCHNYJ_RESURS)
                    {
                      unsigned int chastka = current_settings.setpoint_r_kom_st_Inom / current_settings.setpoint_r_kom_st_Inom_vymk;
                      if (check_data_setpoint(edition_settings.setpoint_krytychnyj_resurs, chastka, (2 * chastka)) == 1)
                      {
                        if (edition_settings.setpoint_krytychnyj_resurs != current_settings.setpoint_krytychnyj_resurs)
                        {
                          //Помічаємо, що поле структури зараз буде змінене
                          changed_settings = CHANGED_ETAP_EXECUTION;

                          current_settings.setpoint_krytychnyj_resurs = edition_settings.setpoint_krytychnyj_resurs;
                          //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                          fix_change_settings(0, 1);
                        }
                        //Виходимо з режиму редагування
                        current_ekran.edition = 0;
                      }
                    }
                    else if (current_ekran.index_position == INDEX_ML_STPPOCHATKOVA_K_VYMK)
                    {
                      if (check_data_setpoint(edition_settings.setpoint_pochatkova_k_vymk, 0, current_settings.setpoint_r_kom_st_Inom) == 1)
                      {
                        if (edition_settings.setpoint_pochatkova_k_vymk != current_settings.setpoint_pochatkova_k_vymk)
                        {
                          //Помічаємо, що поле структури зараз буде змінене
                          changed_settings = CHANGED_ETAP_EXECUTION;

                          current_settings.setpoint_pochatkova_k_vymk = edition_settings.setpoint_pochatkova_k_vymk;
                          //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                          fix_change_settings(0, 1);
                        }
                        //Виходимо з режиму редагування
                        current_ekran.edition = 0;
                      }
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_TIMEOUT_SWITCH)
                  {
                    if (current_ekran.index_position == INDEX_ML_TMOON)
                    {
                      if (check_data_setpoint(edition_settings.timeout_swch_on, TIMEOUT_SWCH_ON_MIN, TIMEOUT_SWCH_ON_MAX) == 1)
                      {
                        if (edition_settings.timeout_swch_on != current_settings.timeout_swch_on)
                        {
                          //Помічаємо, що поле структури зараз буде змінене
                          changed_settings = CHANGED_ETAP_EXECUTION;

                          current_settings.timeout_swch_on = edition_settings.timeout_swch_on;
                          //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                          fix_change_settings(0, 1);
                        }
                        //Виходимо з режиму редагування
                        current_ekran.edition = 0;
                      }
                    }
                    else if (current_ekran.index_position == INDEX_ML_TMOOFF)
                    {
                      if (check_data_setpoint(edition_settings.timeout_swch_off, TIMEOUT_SWCH_OFF_MIN, TIMEOUT_SWCH_OFF_MAX) == 1)
                      {
                        if (edition_settings.timeout_swch_off != current_settings.timeout_swch_off)
                        {
                          //Помічаємо, що поле структури зараз буде змінене
                          changed_settings = CHANGED_ETAP_EXECUTION;

                          current_settings.timeout_swch_off = edition_settings.timeout_swch_off;
                          //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                          fix_change_settings(0, 1);
                        }
                        //Виходимо з режиму редагування
                        current_ekran.edition = 0;
                      }
                    }
                    else if (current_ekran.index_position == INDEX_ML_TMOUDL_BLK_ON)
                    {
                      if (check_data_setpoint(edition_settings.timeout_swch_udl_blk_on, TIMEOUT_SWCH_UDL_BLK_ON_MIN, TIMEOUT_SWCH_UDL_BLK_ON_MAX) == 1)
                      {
                        if (edition_settings.timeout_swch_udl_blk_on != current_settings.timeout_swch_udl_blk_on)
                        {
                          //Помічаємо, що поле структури зараз буде змінене
                          changed_settings = CHANGED_ETAP_EXECUTION;

                          current_settings.timeout_swch_udl_blk_on = edition_settings.timeout_swch_udl_blk_on;
                          //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                          fix_change_settings(0, 1);
                        }
                        //Виходимо з режиму редагування
                        current_ekran.edition = 0;
                      }
                    }
                    else if (current_ekran.index_position == INDEX_ML_TMOPRYVODA_VV)
                    {
                      if (check_data_setpoint(edition_settings.timeout_pryvoda_VV, TIMEOUT_PRYVODA_VV_MIN, TIMEOUT_PRYVODA_VV_MAX) == 1)
                      {
                        if (edition_settings.timeout_pryvoda_VV != current_settings.timeout_pryvoda_VV)
                        {
                          //Помічаємо, що поле структури зараз буде змінене
                          changed_settings = CHANGED_ETAP_EXECUTION;

                          current_settings.timeout_pryvoda_VV = edition_settings.timeout_pryvoda_VV;
                          //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                          fix_change_settings(0, 1);
                        }
                        //Виходимо з режиму редагування
                        current_ekran.edition = 0;
                      }
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_CONTROL_SWITCH)
                  {
                    if ((edition_settings.control_switch & ((unsigned int) (~CTR_SWITCH_MASKA))) == 0)
                    {
                      if (edition_settings.control_switch != current_settings.control_switch)
                      {
                        //Помічаємо, що поле структури зараз буде змінене
                        changed_settings = CHANGED_ETAP_EXECUTION;

                        current_settings.control_switch = edition_settings.control_switch;
                        //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                        fix_change_settings(0, 1);
                      }
                      //Виходимо з режиму редагування
                      current_ekran.edition = 0;
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_DOPUSK_DV_UVV)
                  {
                    unsigned int min_value;
                    if ((current_settings.type_of_input_signal & (1 << current_ekran.index_position)) == 0)
                      min_value = KOEF_DOPUSK_DV_POST_MIN;
                    else
                      min_value = KOEF_DOPUSK_DV_ZMIN_MIN;
                    if (check_data_setpoint(edition_settings.dopusk_dv[current_ekran.index_position], min_value, KOEF_DOPUSK_DV_MAX) == 1)
                    {
                      if (edition_settings.dopusk_dv[current_ekran.index_position] != current_settings.dopusk_dv[current_ekran.index_position])
                      {
                        //Помічаємо, що поле структури зараз буде змінене
                        changed_settings = CHANGED_ETAP_EXECUTION;

                        current_settings.dopusk_dv[current_ekran.index_position] = edition_settings.dopusk_dv[current_ekran.index_position];
                        //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                        fix_change_settings(0, 1);
                      }
                      //Виходимо з режиму редагування
                      current_ekran.edition = 0;
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_TYPE_INPUT_UVV)
                  {
                    if ((edition_settings.type_of_input & ((unsigned int) (~((1ull << NUMBER_INPUTS) - 1)))) == 0)
                    {
                      if (edition_settings.type_of_input != current_settings.type_of_input)
                      {
                        //Помічаємо, що поле структури зараз буде змінене
                        changed_settings = CHANGED_ETAP_EXECUTION;

                        current_settings.type_of_input = edition_settings.type_of_input;
                        //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                        fix_change_settings(0, 1);
                      }
                      //Виходимо з режиму редагування
                      current_ekran.edition = 0;
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_TYPE_INPUT_SIGNAL_UVV)
                  {
                    if ((edition_settings.type_of_input_signal & ((unsigned int) (~((1ull << NUMBER_INPUTS) - 1)))) == 0)
                    {
                      if (edition_settings.type_of_input_signal != current_settings.type_of_input_signal)
                      {
                        //Помічаємо, що поле структури зараз буде змінене
                        changed_settings = CHANGED_ETAP_EXECUTION;

                        correct_dopusk_dv_when_type_of_input_signal_is_changed();
                        current_settings.type_of_input_signal = edition_settings.type_of_input_signal;
                        //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                        fix_change_settings(0, 1);
                      }
                      //Виходимо з режиму редагування
                      current_ekran.edition = 0;
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_TYPE_OUTPUT_UVV)
                  {
                    if (
                      ((edition_settings.type_of_output & (~((1u << NUMBER_SIMPLE_OUTPUTS) - 1))) == 0) &&
                      ((edition_settings.type_of_output_modif & (~((1u << NUMBER_SIMPLE_OUTPUTS) - 1))) == 0))
                    {
                      if (
                        (edition_settings.type_of_output != current_settings.type_of_output) ||
                        (edition_settings.type_of_output_modif != current_settings.type_of_output_modif))
                      {
                        //Помічаємо, що поле структури зараз буде змінене
                        changed_settings = CHANGED_ETAP_EXECUTION;

                        current_settings.type_of_output = edition_settings.type_of_output;
                        current_settings.type_of_output_modif = edition_settings.type_of_output_modif;
                        //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                        fix_change_settings(0, 1);
                      }
                      //Виходимо з режиму редагування
                      current_ekran.edition = 0;
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_TYPE_LED_UVV)
                  {
                    if ((edition_settings.type_of_led & ((unsigned int) (~((1 << NUMBER_LEDS) - 1)))) == 0)
                    {
                      if (edition_settings.type_of_led != current_settings.type_of_led)
                      {
                        //Помічаємо, що поле структури зараз буде змінене
                        changed_settings = CHANGED_ETAP_EXECUTION;

                        current_settings.type_of_led = edition_settings.type_of_led;
                        //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                        fix_change_settings(0, 1);
                      }
                      //Виходимо з режиму редагування
                      current_ekran.edition = 0;
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_TYPE_BUTTON_UVV)
                  {
                    if ((edition_settings.buttons_mode & ((unsigned int) (~((1 << NUMBER_DEFINED_BUTTONS) - 1)))) == 0)
                    {
                      if (edition_settings.buttons_mode != current_settings.buttons_mode)
                      {
                        //Помічаємо, що поле структури зараз буде змінене
                        changed_settings = CHANGED_ETAP_EXECUTION;

                        //Зміна режиму відбудеться у цій функції рахом з можливими змінами у ранжуванні ФК
                        action_during_changing_button_mode(&current_settings, &edition_settings);

                        //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                        fix_change_settings(0, 1);
                      }
                      //Виходимо з режиму редагування
                      current_ekran.edition = 0;
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_ADDRESS_RS485)
                  {
                    if (check_data_setpoint(edition_settings.address, KOEF_ADDRESS_MIN, KOEF_ADDRESS_MAX) == 1)
                    {
                      unsigned int edit_value = edition_settings.address;
                      if (edit_value != current_settings.address)
                      {
                        //Помічаємо, що поле структури зараз буде змінене
                        changed_settings = CHANGED_ETAP_EXECUTION;

                        current_settings.address = edit_value;
                        //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                        fix_change_settings(0, 1);
                      }
                      //Виходимо з режиму редагування
                      current_ekran.edition = 0;
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_VIEW_SPEED_RS485)
                  {
                    if (check_data_setpoint(edition_settings.speed_RS485, VALUE_SPEED_INTERFACE_MIN, VALUE_SPEED_INTERFACE_MAX) == 1)
                    {
                      if (edition_settings.speed_RS485 != current_settings.speed_RS485)
                      {
                        //Помічаємо, що поле структури зараз буде змінене
                        changed_settings = CHANGED_ETAP_EXECUTION;

                        current_settings.speed_RS485 = edition_settings.speed_RS485;
                        //Підраховуємо нову величину затримки у бітах, яка допускається між байтами у RS-485 згідно з визначеними настройками
                        calculate_namber_bit_waiting_for_rs_485();
                        //Виставляємо команду про переконфігурування RS-485
                        make_reconfiguration_RS_485 = 0xff;
                        //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                        fix_change_settings(0, 1);
                      }
                      //Виходимо з режиму редагування
                      current_ekran.edition = 0;
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_VIEW_PARE_RS485)
                  {
                    if (check_data_setpoint(edition_settings.pare_bit_RS485, VALUE_PARE_INTERFACE_MIN, VALUE_PARE_INTERFACE_MAX) == 1)
                    {
                      if (edition_settings.pare_bit_RS485 != current_settings.pare_bit_RS485)
                      {
                        //Помічаємо, що поле структури зараз буде змінене
                        changed_settings = CHANGED_ETAP_EXECUTION;

                        current_settings.pare_bit_RS485 = edition_settings.pare_bit_RS485;
                        //Підраховуємо нову величину затримки у бітах, яка допускається між байтами у RS-485 згідно з визначеними настройками
                        calculate_namber_bit_waiting_for_rs_485();
                        //Виставляємо команду про переконфігурування RS-485
                        make_reconfiguration_RS_485 = 0xff;
                        //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                        fix_change_settings(0, 1);
                      }
                      //Виходимо з режиму редагування
                      current_ekran.edition = 0;
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_VIEW_STOP_BITS_RS485)
                  {
                    if (check_data_setpoint(edition_settings.number_stop_bit_RS485, VALUE_STOP_BITS_INTERFACE_MIN, VALUE_STOP_BITS_INTERFACE_MAX) == 1)
                    {
                      if (edition_settings.number_stop_bit_RS485 != current_settings.number_stop_bit_RS485)
                      {
                        //Помічаємо, що поле структури зараз буде змінене
                        changed_settings = CHANGED_ETAP_EXECUTION;

                        current_settings.number_stop_bit_RS485 = edition_settings.number_stop_bit_RS485;
                        //Підраховуємо нову величину затримки у бітах, яка допускається між байтами у RS-485 згідно з визначеними настройками
                        calculate_namber_bit_waiting_for_rs_485();
                        //Виставляємо команду про переконфігурування RS-485
                        make_reconfiguration_RS_485 = 0xff;
                        //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                        fix_change_settings(0, 1);
                      }
                      //Виходимо з режиму редагування
                      current_ekran.edition = 0;
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_VIEW_TIMEOUT_RS485)
                  {
                    if (check_data_setpoint(edition_settings.time_out_1_RS485, VALUE_TIME_OUT_1_INTERFACE_MIN, VALUE_TIME_OUT_1_INTERFACE_MAX) == 1)
                    {
                      if (edition_settings.time_out_1_RS485 != current_settings.time_out_1_RS485)
                      {
                        //Помічаємо, що поле структури зараз буде змінене
                        changed_settings = CHANGED_ETAP_EXECUTION;

                        current_settings.time_out_1_RS485 = edition_settings.time_out_1_RS485;
                        //Підраховуємо нову величину затримки у бітах, яка допускається між байтами у RS-485 згідно з визначеними настройками
                        calculate_namber_bit_waiting_for_rs_485();
                        //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                        fix_change_settings(0, 1);
                      }
                      //Виходимо з режиму редагування
                      current_ekran.edition = 0;
                    }
                  }
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                  else if (current_ekran.current_level == EKRAN_SETTING_NETWORK_LAYER_ETHERNET)
                  {
                    if (
                      (current_ekran.index_position == INDEX_ML_NL_IPV4) ||
                      (current_ekran.index_position == INDEX_ML_NL_GATEWAY))
                    {
                      uint16_t *point_target, *point_source;
                      if (current_ekran.index_position == INDEX_ML_NL_IPV4)
                      {
                        point_target = edition_settings.IP4;
                        point_source = current_settings.IP4;
                      }
                      else
                      {
                        point_target = edition_settings.gateway;
                        point_source = current_settings.gateway;
                      }

                      enum comp
                      {
                        EQUAL,
                        OUT_OF_RANGE,
                        IN_RANGE
                      } comparation = EQUAL;
                      for (size_t i = 0; i < 4; i++)
                      {
                        if (point_target[i] != point_source[i])
                        {
                          comparation = IN_RANGE;
                          if ((point_target[i] >> 8) != 0)
                          {
                            comparation = OUT_OF_RANGE;
                            break;
                          }
                        }
                      }

                      if (comparation != OUT_OF_RANGE)
                      {
                        if (comparation == IN_RANGE)
                        {
                          //Помічаємо, що поле структури зараз буде змінене
                          changed_settings = CHANGED_ETAP_EXECUTION;

                          for (size_t i = 0; i < 4; i++)
                            point_source[i] = point_target[i];
                          //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                          fix_change_settings(0, 1);

                          //Помічаємо, що треба перезапустити КП
                          _SET_STATE(queue_mo, STATE_QUEUE_MO_RESTART_KP);
                        }
                        //Виходимо з режиму редагування
                        current_ekran.edition = 0;
                      }
                    }
                    else if (current_ekran.index_position == INDEX_ML_NL_MASK)
                    {
                      if (check_data_setpoint(edition_settings.mask, NETWORK_MASK_MIN, NETWORK_MASK_MAX) == 1)
                      {
                        if (edition_settings.mask != current_settings.mask)
                        {
                          //Помічаємо, що поле структури зараз буде змінене
                          changed_settings = CHANGED_ETAP_EXECUTION;

                          current_settings.mask = edition_settings.mask;
                          //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                          fix_change_settings(0, 1);

                          //Помічаємо, що треба перезапустити КП
                          _SET_STATE(queue_mo, STATE_QUEUE_MO_RESTART_KP);
                        }
                        //Виходимо з режиму редагування
                        current_ekran.edition = 0;
                      }
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_SYNCHRO)
                  {
                    if (current_ekran.index_position == INDEX_ML_SYN_IPV4)
                    {
                      enum comp
                      {
                        EQUAL,
                        OUT_OF_RANGE,
                        IN_RANGE
                      } comparation = EQUAL;
                      for (size_t i = 0; i < 4; i++)
                      {
                        if (edition_settings.IP_time_server[i] != current_settings.IP_time_server[i])
                        {
                          comparation = IN_RANGE;
                          if ((edition_settings.IP_time_server[i] >> 8) != 0)
                          {
                            comparation = OUT_OF_RANGE;
                            break;
                          }
                        }
                      }

                      if (comparation != OUT_OF_RANGE)
                      {
                        if (comparation == IN_RANGE)
                        {
                          //Помічаємо, що поле структури зараз буде змінене
                          changed_settings = CHANGED_ETAP_EXECUTION;

                          for (size_t i = 0; i < 4; i++)
                            current_settings.IP_time_server[i] = edition_settings.IP_time_server[i];
                          //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                          fix_change_settings(0, 1);

                          //Помічаємо, що треба перезапустити КП
                          _SET_STATE(queue_mo, STATE_QUEUE_MO_RESTART_KP);
                        }
                        //Виходимо з режиму редагування
                        current_ekran.edition = 0;
                      }
                    }
                    else if (current_ekran.index_position == INDEX_ML_SYN_PORT)
                    {
                      if (check_data_setpoint(edition_settings.port_time_server, NETWORK_PORT_MIN, NETWORK_PORT_MAX) == 1)
                      {
                        if (edition_settings.port_time_server != current_settings.port_time_server)
                        {
                          //Помічаємо, що поле структури зараз буде змінене
                          changed_settings = CHANGED_ETAP_EXECUTION;

                          current_settings.port_time_server = edition_settings.port_time_server;
                          //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                          fix_change_settings(0, 1);

                          //Помічаємо, що треба перезапустити КП
                          _SET_STATE(queue_mo, STATE_QUEUE_MO_RESTART_KP);
                        }
                        //Виходимо з режиму редагування
                        current_ekran.edition = 0;
                      }
                    }
                    else if (current_ekran.index_position == INDEX_ML_SYN_PERIOD)
                    {
                      if (check_data_setpoint(edition_settings.period_sync, NETWORK_PERIOD_SYNC_MIN, NETWORK_PERIOD_SYNC_MAX) == 1)
                      {
                        if (edition_settings.period_sync != current_settings.period_sync)
                        {
                          //Помічаємо, що поле структури зараз буде змінене
                          changed_settings = CHANGED_ETAP_EXECUTION;

                          current_settings.period_sync = edition_settings.period_sync;
                          //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                          fix_change_settings(0, 1);

                          //Помічаємо, що треба перезапустити КП
                          _SET_STATE(queue_mo, STATE_QUEUE_MO_RESTART_KP);
                        }
                        //Виходимо з режиму редагування
                        current_ekran.edition = 0;
                      }
                    }
                  }
#endif
                  else if (current_ekran.current_level == EKRAN_TIME_ZONE)
                  {
                    if (
                      ((edition_settings.time_zone >= TIME_ZONE_MIN) || (edition_settings.time_zone <= TIME_ZONE_MAX)) &&
                      ((edition_settings.dst & ((unsigned int) (~CTR_TZ_MASKA))) == 0))
                    {
                      if (
                        (edition_settings.time_zone != current_settings.time_zone) ||
                        (edition_settings.dst != current_settings.dst))
                      {
                        //Помічаємо, що поле структури зараз буде змінене
                        changed_settings = CHANGED_ETAP_EXECUTION;

                        current_settings.time_zone = edition_settings.time_zone;
                        current_settings.dst = edition_settings.dst;
#if (__VER__ >= 8000000)
                        _ForceReloadDstRules();
#endif

                        //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                        fix_change_settings(0, 1);
                      }
                      //Виходимо з режиму редагування
                      current_ekran.edition = 0;
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_DST_RULE)
                  {
                    uint32_t *p_rule_target = (position_in_current_level_menu[previous_level_in_current_level_menu[EKRAN_DST_RULE]] == INDEX_ML_CHDT_DST_ON) ? &edition_settings.dst_on_rule : &edition_settings.dst_off_rule;
                    int mounth = (*p_rule_target >> POS_MM) & ((1 << SHIFT_MM) - 1);
                    int dow = (*p_rule_target >> POS_DOW) & ((1 << SHIFT_DOW) - 1);
                    int wr = (*p_rule_target >> POS_WR) & ((1 << SHIFT_WR) - 1);
                    int hour = (*p_rule_target >> POS_HH) & ((1 << SHIFT_HH) - 1);

                    if (
                      (
                        (mounth >= DST_RULE_MM_MIN) && (mounth <= DST_RULE_MM_MAX)) &&
                      ((dow >= DST_RULE_DOW_MIN) && (dow <= DST_RULE_DOW_MAX)) &&
                      ((wr >= DST_RULE_WR_MIN) && (wr <= DST_RULE_WR_MAX)) &&
                      ((hour >= DST_RULE_HH_MIN) && (hour <= DST_RULE_HH_MAX)))
                    {
                      uint32_t *p_rule_source = (position_in_current_level_menu[previous_level_in_current_level_menu[EKRAN_DST_RULE]] == INDEX_ML_CHDT_DST_ON) ? &current_settings.dst_on_rule : &current_settings.dst_off_rule;
                      if (*p_rule_target != *p_rule_source)
                      {
                        //Помічаємо, що поле структури зараз буде змінене
                        changed_settings = CHANGED_ETAP_EXECUTION;

                        *p_rule_source = *p_rule_target;
#if (__VER__ >= 8000000)
                        _ForceReloadDstRules();
#endif

                        //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                        fix_change_settings(0, 1);
                      }
                      //Виходимо з режиму редагування
                      current_ekran.edition = 0;
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_GENERAL_PICKUPS_EL)
                  {
                    if (current_ekran.index_position == INDEX_ML_NUMBER_INERATION)
                    {
                      if (check_data_setpoint(edition_settings.number_iteration_el, NUMBER_ITERATION_EL_MIN, NUMBER_ITERATION_EL_MAX) == 1)
                      {
                        if (edition_settings.number_iteration_el != current_settings.number_iteration_el)
                        {
                          //Помічаємо, що поле структури зараз буде змінене
                          changed_settings = CHANGED_ETAP_EXECUTION;

                          current_settings.number_iteration_el = edition_settings.number_iteration_el;
                          //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                          fix_change_settings(0, 1);
                        }
                        //Виходимо з режиму редагування
                        current_ekran.edition = 0;
                      }
                    }
                    //                  else if (current_ekran.index_position == INDEX_ML_NUMBER_DEFINED_FUNCTIONS)
                    //                  {
                    //                    if (check_data_setpoint(edition_settings.number_defined_df, NUMBER_DEFINED_FUNCTIONS_MIN, NUMBER_DEFINED_FUNCTIONS_MAX) == 1)
                    //                    {
                    //                      if (edition_settings.number_defined_df != current_settings.number_defined_df)
                    //                      {
                    //                        //Помічаємо, що поле структури зараз буде змінене
                    //                        changed_settings = CHANGED_ETAP_EXECUTION;
                    //
                    //                        current_settings.number_defined_df = edition_settings.number_defined_df;
                    //                        //Обновляємо значення
                    //                        action_after_changing_number_el(&current_settings, 0);
                    //
                    //                        //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                    //                        fix_change_settings(0, 1);
                    //                      }
                    //                      //Виходимо з режиму редагування
                    //                      current_ekran.edition = 0;
                    //                    }
                    //                  }
                    //                  else if (current_ekran.index_position == INDEX_ML_NUMBER_DEFINED_TRIGGERS)
                    //                  {
                    //                    if (check_data_setpoint(edition_settings.number_defined_dt, NUMBER_DEFINED_TRIGGERS_MIN, NUMBER_DEFINED_TRIGGERS_MAX) == 1)
                    //                    {
                    //                      if (edition_settings.number_defined_dt != current_settings.number_defined_dt)
                    //                      {
                    //                        //Помічаємо, що поле структури зараз буде змінене
                    //                        changed_settings = CHANGED_ETAP_EXECUTION;
                    //
                    //                        current_settings.number_defined_dt = edition_settings.number_defined_dt;
                    //                        //Обновляємо значення
                    //                        action_after_changing_number_el(&current_settings, 1);
                    //
                    //                        //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                    //                        fix_change_settings(0, 1);
                    //                      }
                    //                      //Виходимо з режиму редагування
                    //                      current_ekran.edition = 0;
                    //                    }
                    //                  }
                    //                  else if (current_ekran.index_position == INDEX_ML_NUMBER_DEFINED_AND)
                    //                  {
                    //                    if (check_data_setpoint(edition_settings.number_defined_and, NUMBER_DEFINED_AND_MIN, NUMBER_DEFINED_AND_MAX) == 1)
                    //                    {
                    //                      if (edition_settings.number_defined_and != current_settings.number_defined_and)
                    //                      {
                    //                        //Помічаємо, що поле структури зараз буде змінене
                    //                        changed_settings = CHANGED_ETAP_EXECUTION;
                    //
                    //                        current_settings.number_defined_and = edition_settings.number_defined_and;
                    //                        //Обновляємо значення
                    //                        action_after_changing_number_el(&current_settings, 2);
                    //
                    //                        //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                    //                        fix_change_settings(0, 1);
                    //                      }
                    //                      //Виходимо з режиму редагування
                    //                      current_ekran.edition = 0;
                    //                    }
                    //                  }
                    //                  else if (current_ekran.index_position == INDEX_ML_NUMBER_DEFINED_OR)
                    //                  {
                    //                    if (check_data_setpoint(edition_settings.number_defined_or, NUMBER_DEFINED_OR_MIN, NUMBER_DEFINED_OR_MAX) == 1)
                    //                    {
                    //                      if (edition_settings.number_defined_or != current_settings.number_defined_or)
                    //                      {
                    //                        //Помічаємо, що поле структури зараз буде змінене
                    //                        changed_settings = CHANGED_ETAP_EXECUTION;
                    //
                    //                        current_settings.number_defined_or = edition_settings.number_defined_or;
                    //                        //Обновляємо значення
                    //                        action_after_changing_number_el(&current_settings, 3);
                    //
                    //                        //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                    //                        fix_change_settings(0, 1);
                    //                      }
                    //                      //Виходимо з режиму редагування
                    //                      current_ekran.edition = 0;
                    //                    }
                    //                  }
                    //                  else if (current_ekran.index_position == INDEX_ML_NUMBER_DEFINED_XOR)
                    //                  {
                    //                    if (check_data_setpoint(edition_settings.number_defined_xor, NUMBER_DEFINED_XOR_MIN, NUMBER_DEFINED_XOR_MAX) == 1)
                    //                    {
                    //                      if (edition_settings.number_defined_xor != current_settings.number_defined_xor)
                    //                      {
                    //                        //Помічаємо, що поле структури зараз буде змінене
                    //                        changed_settings = CHANGED_ETAP_EXECUTION;
                    //
                    //                        current_settings.number_defined_xor = edition_settings.number_defined_xor;
                    //                        //Обновляємо значення
                    //                        action_after_changing_number_el(&current_settings, 4);
                    //
                    //                        //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                    //                        fix_change_settings(0, 1);
                    //                      }
                    //                      //Виходимо з режиму редагування
                    //                      current_ekran.edition = 0;
                    //                    }
                    //                  }
                    //                  else if (current_ekran.index_position == INDEX_ML_NUMBER_DEFINED_NOT)
                    //                  {
                    //                    if (check_data_setpoint(edition_settings.number_defined_not, NUMBER_DEFINED_NOT_MIN, NUMBER_DEFINED_NOT_MAX) == 1)
                    //                    {
                    //                      if (edition_settings.number_defined_not != current_settings.number_defined_not)
                    //                      {
                    //                        //Помічаємо, що поле структури зараз буде змінене
                    //                        changed_settings = CHANGED_ETAP_EXECUTION;
                    //
                    //                        current_settings.number_defined_not = edition_settings.number_defined_not;
                    //                        //Обновляємо значення
                    //                        action_after_changing_number_el(&current_settings, 5);
                    //
                    //                        //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                    //                        fix_change_settings(0, 1);
                    //                      }
                    //                      //Виходимо з режиму редагування
                    //                      current_ekran.edition = 0;
                    //                    }
                    //                  }
                  }
                  else if (current_ekran.current_level == EKRAN_LIST_TYPE_DF)
                  {
                    if ((edition_settings.type_df & (~((1u << NUMBER_DEFINED_FUNCTIONS) - 1))) == 0)
                    {
                      if (edition_settings.type_df != current_settings.type_df)
                      {
                        //Помічаємо, що поле структури зараз буде змінене
                        changed_settings = CHANGED_ETAP_EXECUTION;

                        current_settings.type_df = edition_settings.type_df;
                        //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                        fix_change_settings(0, 1);
                      }
                      //Виходимо з режиму редагування
                      current_ekran.edition = 0;
                    }
                  }
                  else if ((current_ekran.current_level >= EKRAN_TIMEOUT_DF1) && (current_ekran.current_level <= (EKRAN_TIMEOUT_DF1 + NUMBER_DEFINED_FUNCTIONS - 1)))
                  {
                    if (current_ekran.index_position == INDEX_ML_TMO_DF_PAUSE)
                    {
                      if (check_data_setpoint(edition_settings.timeout_pause_df[current_ekran.current_level - EKRAN_TIMEOUT_DF1], TIMEOUT_DF_PAUSE_MIN, TIMEOUT_DF_PAUSE_MAX) == 1)
                      {
                        if (edition_settings.timeout_pause_df[current_ekran.current_level - EKRAN_TIMEOUT_DF1] !=
                            current_settings.timeout_pause_df[current_ekran.current_level - EKRAN_TIMEOUT_DF1])
                        {
                          //Помічаємо, що поле структури зараз буде змінене
                          changed_settings = CHANGED_ETAP_EXECUTION;

                          current_settings.timeout_pause_df[current_ekran.current_level - EKRAN_TIMEOUT_DF1] =
                            edition_settings.timeout_pause_df[current_ekran.current_level - EKRAN_TIMEOUT_DF1];
                          //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                          fix_change_settings(0, 1);
                        }
                        //Виходимо з режиму редагування
                        current_ekran.edition = 0;
                      }
                    }
                    else
                    {
                      if (check_data_setpoint(edition_settings.timeout_work_df[current_ekran.current_level - EKRAN_TIMEOUT_DF1], TIMEOUT_DF_WORK_MIN, TIMEOUT_DF_WORK_MAX) == 1)
                      {
                        if (edition_settings.timeout_work_df[current_ekran.current_level - EKRAN_TIMEOUT_DF1] !=
                            current_settings.timeout_work_df[current_ekran.current_level - EKRAN_TIMEOUT_DF1])
                        {
                          //Помічаємо, що поле структури зараз буде змінене
                          changed_settings = CHANGED_ETAP_EXECUTION;

                          current_settings.timeout_work_df[current_ekran.current_level - EKRAN_TIMEOUT_DF1] =
                            edition_settings.timeout_work_df[current_ekran.current_level - EKRAN_TIMEOUT_DF1];
                          //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                          fix_change_settings(0, 1);
                        }
                        //Виходимо з режиму редагування
                        current_ekran.edition = 0;
                      }
                    }
                  }
                  else if ((current_ekran.current_level >= EKRAN_LIST_SOURCE_TF1) && (current_ekran.current_level <= EKRAN_LIST_SOURCE_TF1 + NUMBER_TRANSFER_FUNCTIONS - 1))
                  {
                    uint32_t value_after = edition_settings.ranguvannja_tf[current_ekran.current_level - EKRAN_LIST_SOURCE_TF1];
                    if (
                      ((value_after & 0xffff) < (1 + NUMBER_TOTAL_SIGNAL_FOR_RANG)) &&
                      (((value_after >> 16) & 0xffff) < (1 + NUMBER_TOTAL_SIGNAL_FOR_RANG_SMALL)))
                    {
                      uint32_t value_before = current_settings.ranguvannja_tf[current_ekran.current_level - EKRAN_LIST_SOURCE_TF1];
                      if (value_after != value_before)
                      {
                        //Помічаємо, що поле структури зараз буде змінене
                        changed_settings = CHANGED_ETAP_EXECUTION;

                        current_settings.ranguvannja_tf[current_ekran.current_level - EKRAN_LIST_SOURCE_TF1] =
                          edition_settings.ranguvannja_tf[current_ekran.current_level - EKRAN_LIST_SOURCE_TF1];
                        //Формуємо запис у таблиці настройок про зміну ранжування і ініціюємо запис у EEPROM нових настройок
                        fix_change_settings(1, 1);
                      }
                      //Виходимо з режиму редагування
                      current_ekran.edition = 0;
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_TIMEOUT_DIGITAL_REGISTRATOR)
                  {
                    if (current_ekran.index_position == INDEX_ML_TMO_ELONGATION)
                    {
                      if (check_data_setpoint((edition_settings.timeout_prolongation_work_digital_registrator), TIMEOUT_DR_ELONGATION_MIN, TIMEOUT_DR_ELONGATION_MAX) == 1)
                      {
                        if (edition_settings.timeout_prolongation_work_digital_registrator != current_settings.timeout_prolongation_work_digital_registrator)
                        {
                          //Помічаємо, що поле структури зараз буде змінене
                          changed_settings = CHANGED_ETAP_EXECUTION;

                          current_settings.timeout_prolongation_work_digital_registrator = edition_settings.timeout_prolongation_work_digital_registrator;

                          //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                          fix_change_settings(0, 1);
                        }
                        //Виходимо з режиму редагування
                        current_ekran.edition = 0;
                      }
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_TIMEOUT_ANALOG_REGISTRATOR)
                  {
                    if (current_ekran.index_position == INDEX_ML_TMOPREFAULT)
                    {
                      if (check_data_setpoint((edition_settings.prefault_number_periods * 20), TIMEOUT_PREFAULT_MIN, TIMEOUT_PREFAULT_MAX) == 1)
                      {
                        if (edition_settings.prefault_number_periods != current_settings.prefault_number_periods)
                        {
                          //Помічаємо, що поле структури зараз буде змінене
                          changed_settings = CHANGED_ETAP_EXECUTION;

                          current_settings.prefault_number_periods = edition_settings.prefault_number_periods;

                          //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                          fix_change_settings(0, 1);
                        }
                        //Виходимо з режиму редагування
                        current_ekran.edition = 0;
                      }
                    }
                    else
                    {
                      if (check_data_setpoint((edition_settings.postfault_number_periods * 20), TIMEOUT_POSTFAULT_MIN, TIMEOUT_POSTFAULT_MAX) == 1)
                      {
                        if (edition_settings.postfault_number_periods != current_settings.postfault_number_periods)
                        {
                          //Помічаємо, що поле структури зараз буде змінене
                          changed_settings = CHANGED_ETAP_EXECUTION;

                          current_settings.postfault_number_periods = edition_settings.postfault_number_periods;

                          //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                          fix_change_settings(0, 1);
                        }
                        //Виходимо з режиму редагування
                        current_ekran.edition = 0;
                      }
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_CONTROL_AR)
                  {
                    if ((edition_settings.control_ar & ((unsigned int) (~CTR_AR_MASKA))) == 0)
                    {
                      if (edition_settings.control_ar != current_settings.control_ar)
                      {
                        //Помічаємо, що поле структури зараз буде змінене
                        changed_settings = CHANGED_ETAP_EXECUTION;

                        current_settings.control_ar = edition_settings.control_ar;
                        //Формуємо запис у таблиці настройок про зміну і ініціюємо запис у EEPROM нових настройок
                        fix_change_settings(0, 1);
                      }
                      //Виходимо з режиму редагування
                      current_ekran.edition = 0;
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_VIEW_SETTING_LANGUAGE)
                  {
                    if (check_data_setpoint(edition_settings.language, VALUE_SETTING_LANGUAGE_MIN, VALUE_SETTING_LANGUAGE_MAX) == 1)
                    {
                      if (edition_settings.language != current_settings.language)
                      {
                        //Помічаємо, що поле структури зараз буде змінене
                        changed_settings = CHANGED_ETAP_EXECUTION;

                        current_settings.language = edition_settings.language;
                        //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                        fix_change_settings(0, 1);
                      }
                      //Виходимо з режиму редагування
                      current_ekran.edition = 0;
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_CHOSE_EXTRA_SETTINGS)
                  {
                    if ((edition_settings.control_extra_settings_1 & ((unsigned int) (~CTR_EXTRA_SETTINGS_1_MASKA))) == 0)
                    {
                      if (edition_settings.control_extra_settings_1 != current_settings.control_extra_settings_1)
                      {
                        //Помічаємо, що поле структури зараз буде змінене
                        changed_settings = CHANGED_ETAP_EXECUTION;

                        //Обновляємо значення
                        // action_after_changing_extra_settings(edition_settings.control_extra_settings_1, &current_settings);

                        //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                        fix_change_settings(0, 1);
                      }
                      //Виходимо з режиму редагування
                      current_ekran.edition = 0;
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_VIEW_GRUPA_USTAVOK)
                  {
                    if (check_data_setpoint(edition_settings.grupa_ustavok, SETPOINT_GRUPA_USTAVOK_MIN, SETPOINT_GRUPA_USTAVOK_MAX) == 1)
                    {
                      if (edition_settings.grupa_ustavok != current_settings.grupa_ustavok)
                      {
                        //Помічаємо, що поле структури зараз буде змінене
                        changed_settings = CHANGED_ETAP_EXECUTION;

                        current_settings.grupa_ustavok = edition_settings.grupa_ustavok;
                        //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                        fix_change_settings(0, 1);
                      }
                      //Виходимо з режиму редагування
                      current_ekran.edition = 0;
                    }
                  }
                }
                else if (current_ekran.edition == 3)
                {
                  //Вихід у режимі редагування
                  current_ekran.edition = 0;
                }

                if (current_ekran.edition == 2)
                  make_ekran_ask_rewrite();
                else if (current_ekran.edition == 3)
                {
                  static unsigned char const information_about_error1[MAX_NAMBER_LANGUAGE][MAX_COL_LCD] =
                    {
                      " Вых.за диапазон",
                      " Вих.за діапазон",
                      "  Out of Limits ",
                      "Вых.за диапазон "};

                  static unsigned char const(*point_to_information_about_error)[MAX_COL_LCD] = information_about_error1;
                  make_ekran_about_error(point_to_information_about_error);
                }
                else
                  //Виставляємо біт обновлення екрану
                  new_state_keyboard |= (1u << BIT_REWRITE);

                //Очистити сигналізацію, що натиснута кнопка
                new_state_keyboard &= ~(1u << BIT_KEY_ENTER);
              }
              else if (new_state_keyboard == (1u << BIT_KEY_ESC))
              {
                if (current_ekran.edition == 0)
                {
                  //Вихід у режимі спостерігання
                  //Переходимо у попереднє меню
                  current_ekran.current_level = previous_level_in_current_level_menu[current_ekran.current_level];
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                }
                else
                {
                  //Вихід у режимі редагування без введення змін
                  current_ekran.edition = 0;
                }

                //Виставляємо команду на обновлекння нового екрану
                new_state_keyboard |= (1u << BIT_REWRITE);
                //Очистити сигналізацію, що натиснута кнопка
                new_state_keyboard &= ~(1u << BIT_KEY_ESC);
              }
              else if (new_state_keyboard == (1u << BIT_KEY_UP))
              {
                //Натиснута кнопка UP
                if (
                  (current_ekran.current_level >= EKRAN_SETPOINT_RPN_GROUP1) &&
                  (current_ekran.current_level <= EKRAN_SETPOINT_RPN_GROUP4))
                {
                  int group = (current_ekran.current_level - EKRAN_SETPOINT_RPN_GROUP1);

                  if (current_ekran.edition == 0)
                  {
                    if (--current_ekran.index_position < 0)
                      current_ekran.index_position = MAX_ROW_FOR_SETPOINT_RPN - 1;
                    position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;
                  }
                  else
                  {
                    //Редагування числа
                    if (current_ekran.index_position == INDEX_ML_STPRPN_OSN)
                      edition_settings.setpoint_rpn_osn[group] = edit_setpoint(1, edition_settings.setpoint_rpn_osn[group], 1, COL_SETPOINT_RPN_OSN_COMMA, COL_SETPOINT_RPN_OSN_END, 100);
                    else if (current_ekran.index_position == INDEX_ML_STPRPN_ZONE)
                      edition_settings.setpoint_rpn_zony[group] = edit_setpoint(1, edition_settings.setpoint_rpn_zony[group], 1, COL_SETPOINT_RPN_ZONE_COMMA, COL_SETPOINT_RPN_ZONE_END, 10);
                    else if (current_ekran.index_position == INDEX_ML_STPRPN_MAX_PER)
                      edition_settings.setpoint_rpn_per[group] = edit_setpoint(1, edition_settings.setpoint_rpn_per[group], 0, 0, COL_SETPOINT_RPN_MAX_PER_END, 1);
                    else if (current_ekran.index_position == INDEX_ML_STPRPN_DOD)
                      edition_settings.setpoint_rpn_dod[group] = edit_setpoint(1, edition_settings.setpoint_rpn_dod[group], 1, COL_SETPOINT_RPN_DOD_COMMA, COL_SETPOINT_RPN_DOD_END, 100);
                    else if (current_ekran.index_position == INDEX_ML_STPRPN_K)
                      edition_settings.setpoint_rpn_K[group] = edit_setpoint(1, edition_settings.setpoint_rpn_K[group], 1, COL_SETPOINT_RPN_K_COMMA, COL_SETPOINT_RPN_K_END, 1);
                  }
                  //Формуємо екран уставок РПН
                  make_ekran_setpoint_rpn(group);
                }
                else if (
                  (current_ekran.current_level >= EKRAN_TIMEOUT_RPN_GROUP1) &&
                  (current_ekran.current_level <= EKRAN_TIMEOUT_RPN_GROUP4))
                {
                  int group = (current_ekran.current_level - EKRAN_TIMEOUT_RPN_GROUP1);

                  if (current_ekran.edition == 0)
                  {
                    if (--current_ekran.index_position < 0)
                      current_ekran.index_position = MAX_ROW_FOR_TIMEOUT_RPN - 1;
                    position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;
                  }
                  else
                  {
                    //Редагування числа
                    if (current_ekran.index_position == INDEX_ML_TMORPN_UB_PRYB)
                      edition_settings.timeout_rpn_ub_pryb[group] = edit_setpoint(1, edition_settings.timeout_rpn_ub_pryb[group], 0, 0, COL_TMO_RPN_UB_PRYB_END, 1000);
                    else if (current_ekran.index_position == INDEX_ML_TMORPN_UB_PRYB_PRYSK)
                      edition_settings.timeout_rpn_ub_pryb_prysk[group] = edit_setpoint(1, edition_settings.timeout_rpn_ub_pryb_prysk[group], 0, 0, COL_TMO_RPN_UB_PRYB_PRYSK_END, 1000);
                    else if (current_ekran.index_position == INDEX_ML_TMORPN_PRYSK_VID_UMAX)
                      edition_settings.timeout_rpn_prysk_vid_Umax[group] = edit_setpoint(1, edition_settings.timeout_rpn_prysk_vid_Umax[group], 1, COL_TMO_RPN_PRYSK_VID_UMAX_COMMA, COL_TMO_RPN_PRYSK_VID_UMAX_END, 100);
                    else if (current_ekran.index_position == INDEX_ML_TMORPN_PEREKL)
                      edition_settings.timeout_rpn_perekl[group] = edit_setpoint(1, edition_settings.timeout_rpn_perekl[group], 1, COL_TMO_RPN_PEREKL_COMMA, COL_TMO_RPN_PEREKL_END, 10);
                    else if (current_ekran.index_position == INDEX_ML_TMORPN_NESPR)
                      edition_settings.timeout_rpn_nespr[group] = edit_setpoint(1, edition_settings.timeout_rpn_nespr[group], 1, COL_TMO_RPN_NESPR_COMMA, COL_TMO_RPN_NESPR_END, 10);
                  }
                  //Формуємо екран витримок РПН
                  make_ekran_timeout_rpn(group);
                }
                else if (current_ekran.current_level == EKRAN_CONTROL_RPN)
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_FOR_CONTROL_RPN - 1;
                  position_in_current_level_menu[EKRAN_CONTROL_RPN] = current_ekran.index_position;
                  //Формуємо екран управлінської інформації для РПН
                  make_ekran_control_rpn();
                }
                else if (current_ekran.current_level == EKRAN_CONTROL_SZKh)
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_FOR_CONTROL_SZKh - 1;
                  position_in_current_level_menu[EKRAN_CONTROL_SZKh] = current_ekran.index_position;
                  //Формуємо екран управлінської інформації для ЗСХ
                  make_ekran_control_zskh();
                }
                else if (
                  (current_ekran.current_level >= EKRAN_SETPOINT_BRP_GROUP1) &&
                  (current_ekran.current_level <= EKRAN_SETPOINT_BRP_GROUP4))
                {
                  int group = (current_ekran.current_level - EKRAN_SETPOINT_BRP_GROUP1);

                  if (current_ekran.edition == 0)
                  {
                    if (--current_ekran.index_position < 0)
                      current_ekran.index_position = MAX_ROW_FOR_SETPOINT_BRP - 1;
                    position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;
                  }
                  else
                  {
                    //Редагування числа
                    if (current_ekran.index_position == INDEX_ML_STPBRP_I_BLK)
                      edition_settings.setpoint_brp_I_blk[group] = edit_setpoint(1, edition_settings.setpoint_brp_I_blk[group], 1, COL_SETPOINT_BRP_I_BLK_COMMA, COL_SETPOINT_BRP_I_BLK_END, 1);
                  }
                  //Формуємо екран уставок БРП
                  make_ekran_setpoint_brp(group);
                }
                else if (current_ekran.current_level == EKRAN_CONTROL_BRP)
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_FOR_CONTROL_BRP - 1;
                  position_in_current_level_menu[EKRAN_CONTROL_BRP] = current_ekran.index_position;
                  //Формуємо екран управлінської інформації для БРП
                  make_ekran_control_brp();
                }
                else if (
                  (current_ekran.current_level >= EKRAN_TIMEOUT_ZNKh_GROUP1) &&
                  (current_ekran.current_level <= EKRAN_TIMEOUT_ZNKh_GROUP4))
                {
                  int group = (current_ekran.current_level - EKRAN_TIMEOUT_ZNKh_GROUP1);

                  if (current_ekran.edition == 0)
                  {
                    if (--current_ekran.index_position < 0)
                      current_ekran.index_position = MAX_ROW_FOR_TIMEOUT_ZNKh - 1;
                    position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;
                  }
                  else
                  {
                    //Редагування числа
                    if (current_ekran.index_position == INDEX_ML_TMOZNKh_PEREKL)
                    {
                      unsigned int temp_setpoint = edition_settings.timeout_znkh_perekl[group];
                      if ((temp_setpoint % 2) != 0)
                        temp_setpoint &= (~1u); //Це випадок коли прошивка змінилася з 2мс  циклом де допуск був встановлений непарнмй допуск
                      do
                      {
                        //Величину витримки зменшуємо почергого на 1 мс (ф-ція edit_setpoint збільшує/зменшує з кроком кратних 1, 10 і т.д.), щоб отримати крок 2 мс
                        temp_setpoint = edit_setpoint(1, temp_setpoint, 1, COL_TMO_ZNKh_PEREKL_COMMA, COL_TMO_ZNKh_PEREKL_END, 1);
                      } while ((temp_setpoint % 2) != 0);
                      edition_settings.timeout_znkh_perekl[group] = temp_setpoint;
                    }
                  }
                  //Формуємо екран витримок ЗНХ
                  make_ekran_timeout_znkh(group);
                }
                else if (current_ekran.current_level == EKRAN_CONTROL_ZNKh)
                {
                  current_ekran.index_position--;

                  if (current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_FOR_CONTROL_ZNKh - 1;

                  position_in_current_level_menu[EKRAN_CONTROL_ZNKh] = current_ekran.index_position;

                  //Формуємо екран управлінської інформації для ЗНХ
                  make_ekran_control_znkh();
                }
                else if (
                  (current_ekran.current_level >= EKRAN_SETPOINT_UMIN_GROUP1) &&
                  (current_ekran.current_level <= EKRAN_SETPOINT_UMIN_GROUP4))
                {
                  int group = (current_ekran.current_level - EKRAN_SETPOINT_UMIN_GROUP1);

                  if (current_ekran.edition == 0)
                  {
                    if (--current_ekran.index_position < 0)
                      current_ekran.index_position = MAX_ROW_FOR_SETPOINT_UMIN - 1;
                    position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;
                  }
                  else
                  {
                    //Редагування числа
                    if (current_ekran.index_position == INDEX_ML_STPUmin_Umin1)
                      edition_settings.setpoint_Umin1[group] = edit_setpoint(1, edition_settings.setpoint_Umin1[group], 0, 0, COL_SETPOINT_Umin_Umin1_END, 1000);
                    else if (current_ekran.index_position == INDEX_ML_STPUmin_Umin2)
                      edition_settings.setpoint_Umin2[group] = edit_setpoint(1, edition_settings.setpoint_Umin2[group], 0, 0, COL_SETPOINT_Umin_Umin2_END, 1000);
                  }
                  //Формуємо екран уставок Umin
                  make_ekran_setpoint_Umin(group);
                }
                else if (
                  (current_ekran.current_level >= EKRAN_TIMEOUT_UMIN_GROUP1) &&
                  (current_ekran.current_level <= EKRAN_TIMEOUT_UMIN_GROUP4))
                {
                  int group = (current_ekran.current_level - EKRAN_TIMEOUT_UMIN_GROUP1);

                  if (current_ekran.edition == 0)
                  {
                    if (--current_ekran.index_position < 0)
                      current_ekran.index_position = MAX_ROW_FOR_TIMEOUT_UMIN - 1;
                    position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;
                  }
                  else
                  {
                    //Редагування числа
                    if (current_ekran.index_position == INDEX_ML_TMOUmin_Umin1)
                      edition_settings.timeout_Umin1[group] = edit_setpoint(1, edition_settings.timeout_Umin1[group], 1, COL_TMO_Umin_Umin1_COMMA, COL_TMO_Umin_Umin1_END, 100);
                    else if (current_ekran.index_position == INDEX_ML_TMOUmin_Umin2)
                      edition_settings.timeout_Umin2[group] = edit_setpoint(1, edition_settings.timeout_Umin2[group], 1, COL_TMO_Umin_Umin2_COMMA, COL_TMO_Umin_Umin2_END, 100);
                  }
                  //Формуємо екран витримок Umin
                  make_ekran_timeout_Umin(group);
                }
                else if (current_ekran.current_level == EKRAN_CONTROL_UMIN)
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_FOR_CONTROL_UMIN - 1;
                  position_in_current_level_menu[EKRAN_CONTROL_UMIN] = current_ekran.index_position;
                  //Формуємо екран управлінської інформації для Umin
                  make_ekran_control_Umin();
                }
                else if (
                  (current_ekran.current_level >= EKRAN_SETPOINT_UMAX_GROUP1) &&
                  (current_ekran.current_level <= EKRAN_SETPOINT_UMAX_GROUP4))
                {
                  int group = (current_ekran.current_level - EKRAN_SETPOINT_UMAX_GROUP1);

                  if (current_ekran.edition == 0)
                  {
                    if (--current_ekran.index_position < 0)
                      current_ekran.index_position = MAX_ROW_FOR_SETPOINT_Umax - 1;
                    position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;
                  }
                  else
                  {
                    //Редагування числа
                    if (current_ekran.index_position == INDEX_ML_STPUmax_Umax1)
                      edition_settings.setpoint_Umax1[group] = edit_setpoint(1, edition_settings.setpoint_Umax1[group], 0, 0, COL_SETPOINT_Umax_Umax1_END, 1000);
                    else if (current_ekran.index_position == INDEX_ML_STPUmax_Umax2)
                      edition_settings.setpoint_Umax2[group] = edit_setpoint(1, edition_settings.setpoint_Umax2[group], 0, 0, COL_SETPOINT_Umax_Umax2_END, 1000);
                  }
                  //Формуємо екран уставок Umax
                  make_ekran_setpoint_Umax(group);
                }
                else if (
                  (current_ekran.current_level >= EKRAN_TIMEOUT_UMAX_GROUP1) &&
                  (current_ekran.current_level <= EKRAN_TIMEOUT_UMAX_GROUP4))
                {
                  int group = (current_ekran.current_level - EKRAN_TIMEOUT_UMAX_GROUP1);

                  if (current_ekran.edition == 0)
                  {
                    if (--current_ekran.index_position < 0)
                      current_ekran.index_position = MAX_ROW_FOR_TIMEOUT_UMAX - 1;
                    position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;
                  }
                  else
                  {
                    //Редагування числа
                    if (current_ekran.index_position == INDEX_ML_TMOUmax_Umax2)
                      edition_settings.timeout_Umax2[group] = edit_setpoint(1, edition_settings.timeout_Umax2[group], 1, COL_TMO_Umax_Umax2_COMMA, COL_TMO_Umax_Umax2_END, 100);
                  }
                  //Формуємо екран витримок Umax
                  make_ekran_timeout_Umax(group);
                }
                else if (current_ekran.current_level == EKRAN_CONTROL_UMAX)
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_FOR_CONTROL_UMAX - 1;
                  position_in_current_level_menu[EKRAN_CONTROL_UMAX] = current_ekran.index_position;
                  //Формуємо екран управлінської інформації для Umax
                  make_ekran_control_Umax();
                }
                else if (
                  (current_ekran.current_level >= EKRAN_SETPOINT_UP_GROUP1) &&
                  (current_ekran.current_level <= EKRAN_SETPOINT_UP_GROUP4))
                {
                  int group = (current_ekran.current_level - EKRAN_SETPOINT_UP_GROUP1);

                  if (current_ekran.edition == 0)
                  {
                    if (--current_ekran.index_position < 0)
                      current_ekran.index_position = NUMBER_UP * MAX_ROW_FOR_SETPOINT_UP - 1;
                    position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;
                  }
                  else
                  {
                    //Редагування числа
                    uint32_t _n_UP = current_ekran.index_position / MAX_ROW_FOR_SETPOINT_UP;
                    uint32_t _n_index = current_ekran.index_position % MAX_ROW_FOR_SETPOINT_UP;

                    if (_n_index == INDEX_ML_STP_UP)
                    {
                      uint32_t comma = 0, end = 0, min_step = 0;
                      int32_t sign_before = 1, sign_after = 1;
                      switch (current_settings.ctrl_UP_input[_n_UP])
                      {
                        case UP_CTRL_Ia_Ib_Ic:
                        case UP_CTRL_Ia:
                        case UP_CTRL_Ib:
                        case UP_CTRL_Ic:
                        case UP_CTRL_I1:
                        case UP_CTRL_I2:
                        case UP_CTRL_I04:
                        case UP_CTRL_3I0_r:
                          {
                            comma = COL_SETPOINT_UP_I_COMMA;
                            end = COL_SETPOINT_UP_I_END;
                            min_step = 10;
                            break;
                          }
                        case UP_CTRL_3I0:
                        case UP_CTRL_3I0_others:
                          {
                            comma = COL_SETPOINT_UP_3I0_COMMA;
                            end = COL_SETPOINT_UP_3I0_END;
                            min_step = 1;
                            break;
                          }
                        case UP_CTRL_Ua_Ub_Uc:
                        case UP_CTRL_Uab_Ubc_Uca:
                        case UP_CTRL_Ua:
                        case UP_CTRL_Uab:
                        case UP_CTRL_Ub:
                        case UP_CTRL_Ubc:
                        case UP_CTRL_Uc:
                        case UP_CTRL_Uca:
                        case UP_CTRL_U1:
                        case UP_CTRL_U2:
                        case UP_CTRL_3U0:
                          {
                            comma = COL_SETPOINT_UP_U_COMMA;
                            end = COL_SETPOINT_UP_U_END;
                            min_step = 100;
                            break;
                          }
                        case UP_CTRL_P:
                        case UP_CTRL_Q:
                          {
                            if (edition_settings.setpoint_UP[_n_UP][0][group] < 0)
                              sign_before = -1;
                            if (current_ekran.position_cursor_x == (COL_SETPOINT_UP_PQ_BEGIN - 1))
                            {
                              sign_after = -sign_before;
                            }
                            else
                            {
                              sign_after = sign_before;
                              comma = COL_SETPOINT_UP_PQ_COMMA;
                              end = COL_SETPOINT_UP_PQ_END;
                              min_step = 1;
                            }
                            break;
                          }
                        case UP_CTRL_S:
                          {
                            comma = COL_SETPOINT_UP_S_COMMA;
                            end = COL_SETPOINT_UP_S_END;
                            min_step = 1;
                            break;
                          }
                        default:
                          {
                            //Теоретично цього ніколи не мало б бути
                            total_error_sw_fixed();
                          }
                      }

                      if (sign_before == sign_after)
                        edition_settings.setpoint_UP[_n_UP][0][group] = sign_after * edit_setpoint(1, abs(edition_settings.setpoint_UP[_n_UP][0][group]), 1, comma, end, min_step);
                      else
                        edition_settings.setpoint_UP[_n_UP][0][group] = sign_after * abs(edition_settings.setpoint_UP[_n_UP][0][group]);
                    }
                    else if (_n_index == INDEX_ML_STP_UP_KP)
                      edition_settings.setpoint_UP_KP[_n_UP][0][group] = edit_setpoint(1, edition_settings.setpoint_UP_KP[_n_UP][0][group], 1, COL_SETPOINT_UP_KP_COMMA, COL_SETPOINT_UP_KP_END, 1);
                  }
                  //Формуємо екран уставок УЗ
                  make_ekran_setpoint_UP(group);
                }
                else if (
                  (current_ekran.current_level >= EKRAN_TIMEOUT_UP_GROUP1) &&
                  (current_ekran.current_level <= EKRAN_TIMEOUT_UP_GROUP4))
                {
                  int group = (current_ekran.current_level - EKRAN_TIMEOUT_UP_GROUP1);

                  if (current_ekran.edition == 0)
                  {
                    if (--current_ekran.index_position < 0)
                      current_ekran.index_position = NUMBER_UP * MAX_ROW_FOR_TIMEOUT_UP - 1;
                    position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;
                  }
                  else
                  {
                    //Редагування числа
                    uint32_t _n_UP = current_ekran.index_position / MAX_ROW_FOR_TIMEOUT_UP;
                    uint32_t _n_index = current_ekran.index_position % MAX_ROW_FOR_TIMEOUT_UP;

                    if (_n_index == INDEX_ML_TMOUP)
                      edition_settings.timeout_UP[_n_UP][0][group] = edit_setpoint(1, edition_settings.timeout_UP[_n_UP][0][group], 1, COL_TMO_UP_COMMA, COL_TMO_UP_END, 10);
                  }
                  //Формуємо екран витримок УЗ
                  make_ekran_timeout_UP(group);
                }
                else if (current_ekran.current_level == EKRAN_CONTROL_UP)
                {
                  current_ekran.index_position--;
                  if (current_ekran.index_position < 0)
                    current_ekran.index_position = (NUMBER_UP * MAX_ROW_FOR_CONTROL_UP) - 1;

                  __SETTINGS *point = (current_ekran.edition == 0) ? &current_settings : &edition_settings;
                  uint32_t ctrl_UP_input = point->ctrl_UP_input[current_ekran.index_position / (_CTR_UP_NEXT_BIT - _CTR_UP_PART_I)];
                  while (
                    ((current_ekran.index_position % (_CTR_UP_NEXT_BIT - _CTR_UP_PART_I)) == CTR_UP_OR_AND_BIT) &&
                    (ctrl_UP_input != UP_CTRL_Ia_Ib_Ic) &&
                    (ctrl_UP_input != UP_CTRL_Ua_Ub_Uc) &&
                    (ctrl_UP_input != UP_CTRL_Uab_Ubc_Uca))
                  {
                    current_ekran.index_position--;
                    if (current_ekran.index_position < 0)
                      current_ekran.index_position = (NUMBER_UP * MAX_ROW_FOR_CONTROL_UP) - 1;

                    ctrl_UP_input = point->ctrl_UP_input[current_ekran.index_position / (_CTR_UP_NEXT_BIT - _CTR_UP_PART_I)];
                  }

                  position_in_current_level_menu[EKRAN_CONTROL_UP] = current_ekran.index_position;

                  //Формуємо екран управлінської інформації для УЗ
                  make_ekran_control_UP();
                }
                else if (current_ekran.current_level == EKRAN_TRANSFORMATOR_INFO)
                {
                  if (current_ekran.edition == 0)
                  {
                    current_ekran.index_position--;
                    do
                    {
                      if (current_ekran.index_position < 0)
                        current_ekran.index_position = MAX_ROW_FOR_TRANSFORMATOR_INFO - 1;

                      while (
                        (
                          (current_ekran.index_position == INDEX_ML_TT2) ||
                          (current_ekran.index_position == INDEX_ML_TN2)) &&
                        ((current_settings.control_rpn & MASKA_FOR_BIT(INDEX_ML_CTRRPN_TRANSF)) == 0))
                        current_ekran.index_position--;
                    } while (current_ekran.index_position < 0);

                    position_in_current_level_menu[EKRAN_TRANSFORMATOR_INFO] = current_ekran.index_position;
                  }
                  else
                  {
                    //Редагування числа
                    if (current_ekran.index_position == INDEX_ML_TT1)
                      edition_settings.TCurrent1 = edit_setpoint(1, edition_settings.TCurrent1, 0, 0, COL_TT_END, 1);
                    else if (current_ekran.index_position == INDEX_ML_TT2)
                      edition_settings.TCurrent2 = edit_setpoint(1, edition_settings.TCurrent2, 0, 0, COL_TT_END, 1);
                    else if (current_ekran.index_position == INDEX_ML_TN1)
                      edition_settings.TVoltage1 = edit_setpoint(1, edition_settings.TVoltage1, 0, 0, COL_TN_END, 1);
                    else
                      edition_settings.TVoltage2 = edit_setpoint(1, edition_settings.TVoltage2, 0, 0, COL_TN_END, 1);
                  }
                  //Формуємо екран інфтрмації по трансформаторах
                  make_ekran_transformator();
                }
                else if (current_ekran.current_level == EKRAN_SETPOINT_SWITCH)
                {
                  if (current_ekran.edition == 0)
                  {
                    if (--current_ekran.index_position < 0)
                      current_ekran.index_position = MAX_ROW_FOR_SETPOINT_SWITCH - 1;
                    position_in_current_level_menu[EKRAN_SETPOINT_SWITCH] = current_ekran.index_position;
                  }
                  else
                  {
                    //Редагування числа
                    if (current_ekran.index_position == INDEX_ML_STPInom)
                      edition_settings.setpoint_Inom = edit_setpoint(1, edition_settings.setpoint_Inom, 0, 0, COL_SETPOINT_Inom_END, 1);
                    if (current_ekran.index_position == INDEX_ML_STPRKS_Inom)
                      edition_settings.setpoint_r_kom_st_Inom = edit_setpoint(1, edition_settings.setpoint_r_kom_st_Inom, 0, 0, COL_SETPOINT_RKS_Inom_END, 1);
                    else if (current_ekran.index_position == INDEX_ML_STPMInom_vymk)
                      edition_settings.setpoint_Inom_vymk = edit_setpoint(1, edition_settings.setpoint_Inom_vymk, 1, COL_SETPOINT_Inom_vymk_COMMA, COL_SETPOINT_Inom_vymk_END, 1);
                    else if (current_ekran.index_position == INDEX_ML_STPRKS_Inom_vymk)
                      edition_settings.setpoint_r_kom_st_Inom_vymk = edit_setpoint(1, edition_settings.setpoint_r_kom_st_Inom_vymk, 0, 0, COL_SETPOINT_RKS_Inom_vymk_END, 1);
                    else if (current_ekran.index_position == INDEX_ML_STPPOCHATKOVYJ_RESURS)
                      edition_settings.setpoint_pochatkovyj_resurs = edit_setpoint(1, edition_settings.setpoint_pochatkovyj_resurs, 0, 0, COL_SETPOINT_POCHATKOVYJ_RESURS_END, 1);
                    else if (current_ekran.index_position == INDEX_ML_STPKRYTYCHNYJ_RESURS)
                      edition_settings.setpoint_krytychnyj_resurs = edit_setpoint(1, edition_settings.setpoint_krytychnyj_resurs, 0, 0, COL_SETPOINT_KRYTYCHNYJ_RESURS_END, 1);
                    else if (current_ekran.index_position == INDEX_ML_STPPOCHATKOVA_K_VYMK)
                      edition_settings.setpoint_pochatkova_k_vymk = edit_setpoint(1, edition_settings.setpoint_pochatkova_k_vymk, 0, 0, COL_SETPOINT_POCHATKOVA_K_VYMK_END, 1);
                  }
                  //Формуємо екран уставок виключателя
                  make_ekran_setpoint_switch();
                }
                else if (current_ekran.current_level == EKRAN_TIMEOUT_SWITCH)
                {
                  if (current_ekran.edition == 0)
                  {
                    if (--current_ekran.index_position < 0)
                      current_ekran.index_position = MAX_ROW_FOR_TIMEOUT_SWITCH - 1;
                    position_in_current_level_menu[EKRAN_TIMEOUT_SWITCH] = current_ekran.index_position;
                  }
                  else
                  {
                    //Редагування числа
                    if (current_ekran.index_position == INDEX_ML_TMOON)
                      edition_settings.timeout_swch_on = edit_setpoint(1, edition_settings.timeout_swch_on, 1, COL_TMO_SWCH_ON_COMMA, COL_TMO_SWCH_ON_END, 10);
                    else if (current_ekran.index_position == INDEX_ML_TMOOFF)
                      edition_settings.timeout_swch_off = edit_setpoint(1, edition_settings.timeout_swch_off, 1, COL_TMO_SWCH_OFF_COMMA, COL_TMO_SWCH_OFF_END, 10);
                    else if (current_ekran.index_position == INDEX_ML_TMOUDL_BLK_ON)
                      edition_settings.timeout_swch_udl_blk_on = edit_setpoint(1, edition_settings.timeout_swch_udl_blk_on, 1, COL_TMO_SWCH_UDL_BLK_ON_COMMA, COL_TMO_SWCH_UDL_BLK_ON_END, 10);
                    else if (current_ekran.index_position == INDEX_ML_TMOPRYVODA_VV)
                      edition_settings.timeout_pryvoda_VV = edit_setpoint(1, edition_settings.timeout_pryvoda_VV, 1, COL_TMO_PRYVODA_VV_COMMA, COL_TMO_PRYVODA_VV_END, 10);
                  }
                  //Формуємо екран витримок виключателя
                  make_ekran_timeout_switch();
                }
                else if (current_ekran.current_level == EKRAN_CONTROL_SWITCH)
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_FOR_CONTROL_SWITCH - 1;
                  position_in_current_level_menu[EKRAN_CONTROL_SWITCH] = current_ekran.index_position;
                  //Формуємо екран управлінської інформації для вимикача
                  make_ekran_control_switch();
                }
                else if (current_ekran.current_level == EKRAN_DOPUSK_DV_UVV)
                {
                  if (current_ekran.edition == 0)
                  {
                    if (--current_ekran.index_position < 0)
                      current_ekran.index_position = NUMBER_INPUTS - 1;
                    position_in_current_level_menu[EKRAN_DOPUSK_DV_UVV] = current_ekran.index_position;
                  }
                  else
                  {
                    //Редагування числа
                    edition_settings.dopusk_dv[current_ekran.index_position] =
                      edit_setpoint(1, edition_settings.dopusk_dv[current_ekran.index_position], 0, 0, COL_DOPUSK_DV_END, 1);
                  }
                  //Формуємо екран інфтрмації по допусках ДВ
                  make_ekran_dopusk_dv();
                }
                else if ((current_ekran.current_level == EKRAN_TYPE_INPUT_UVV) || (current_ekran.current_level == EKRAN_TYPE_INPUT_SIGNAL_UVV))
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = NUMBER_INPUTS - 1;
                  position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;
                  //Формуємо екран управлінської інформації для УВВ
                  if (current_ekran.current_level == EKRAN_TYPE_INPUT_UVV)
                    make_ekran_type_input_uvv(0);
                  else
                    make_ekran_type_input_uvv(1);
                }
                else if (current_ekran.current_level == EKRAN_TYPE_OUTPUT_UVV)
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = NUMBER_SIMPLE_OUTPUTS - 1;
                  position_in_current_level_menu[EKRAN_TYPE_OUTPUT_UVV] = current_ekran.index_position;
                  //Формуємо екран типу виходу
                  make_ekran_type_output_uvv();
                }
                else if (current_ekran.current_level == EKRAN_TYPE_LED_UVV)
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = NUMBER_LEDS - 1;
                  position_in_current_level_menu[EKRAN_TYPE_LED_UVV] = current_ekran.index_position;
                  //Формуємо екран типу світлоіндикатора
                  make_ekran_type_led_uvv();
                }
                else if (current_ekran.current_level == EKRAN_TYPE_BUTTON_UVV)
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = NUMBER_DEFINED_BUTTONS - 1;
                  position_in_current_level_menu[EKRAN_TYPE_BUTTON_UVV] = current_ekran.index_position;
                  //Формуємо екран типу ФК
                  make_ekran_type_button_uvv();
                }
                else if (current_ekran.current_level == EKRAN_ADDRESS_RS485)
                {
                  if (current_ekran.edition == 0)
                  {
                    if (--current_ekran.index_position < 0)
                      current_ekran.index_position = MAX_ROW_FOR_ADDRESS - 1;
                    position_in_current_level_menu[EKRAN_ADDRESS_RS485] = current_ekran.index_position;
                  }
                  else
                  {
                    //Редагування числа
                    edition_settings.address = edit_setpoint(1, edition_settings.address, 0, 0, COL_ADDRESS_END, 1);
                  }
                  //Формуємо екран інфтрмації по комунікаційній адресі
                  make_ekran_address();
                }
                else if (current_ekran.current_level == EKRAN_VIEW_SPEED_RS485)
                {
                  if (current_ekran.edition == 0)
                  {
                    if (--current_ekran.index_position < 0)
                      current_ekran.index_position = MAX_ROW_FOR_VIEW_SPEED_INTERFACE - 1;
                    position_in_current_level_menu[EKRAN_VIEW_SPEED_RS485] = current_ekran.index_position;
                  }
                  else
                  {
                    //Редагування числа
                    edition_settings.speed_RS485--;
                    if (edition_settings.speed_RS485 < VALUE_SPEED_INTERFACE_MIN)
                      edition_settings.speed_RS485 = VALUE_SPEED_INTERFACE_MAX;
                  }
                  //Формуємо екран інформації по швидкості обміну
                  make_ekran_speed_interface();
                }
                else if (current_ekran.current_level == EKRAN_VIEW_PARE_RS485)
                {
                  if (current_ekran.edition == 0)
                  {
                    if (--current_ekran.index_position < 0)
                      current_ekran.index_position = MAX_ROW_FOR_VIEW_PARE_INTERFACE - 1;
                    position_in_current_level_menu[EKRAN_VIEW_PARE_RS485] = current_ekran.index_position;
                  }
                  else
                  {
                    //Редагування числа
                    edition_settings.pare_bit_RS485--;
                    if (edition_settings.pare_bit_RS485 < VALUE_PARE_INTERFACE_MIN)
                      edition_settings.pare_bit_RS485 = VALUE_PARE_INTERFACE_MAX;
                  }
                  //Формуємо екран інформації по контролю парності
                  make_ekran_pare_interface();
                }
                else if (current_ekran.current_level == EKRAN_VIEW_STOP_BITS_RS485)
                {
                  if (current_ekran.edition == 0)
                  {
                    if (--current_ekran.index_position < 0)
                      current_ekran.index_position = MAX_ROW_FOR_VIEW_STOP_BITS_INTERFACE - 1;
                    position_in_current_level_menu[EKRAN_VIEW_STOP_BITS_RS485] = current_ekran.index_position;
                  }
                  else
                  {
                    //Редагування числа
                    edition_settings.number_stop_bit_RS485--;
                    if (edition_settings.number_stop_bit_RS485 < VALUE_STOP_BITS_INTERFACE_MIN)
                      edition_settings.number_stop_bit_RS485 = VALUE_STOP_BITS_INTERFACE_MAX;
                  }
                  //Формуємо екран інформації по кількості стопових біт
                  make_ekran_stopbits_interface();
                }
                else if (current_ekran.current_level == EKRAN_VIEW_TIMEOUT_RS485)
                {
                  if (current_ekran.edition == 0)
                  {
                    if (--current_ekran.index_position < 0)
                      current_ekran.index_position = MAX_ROW_FOR_VIEW_TIMEOUT_INTERFACE - 1;
                    position_in_current_level_menu[EKRAN_VIEW_TIMEOUT_RS485] = current_ekran.index_position;
                  }
                  else
                  {
                    //Редагування числа
                    edition_settings.time_out_1_RS485 = edit_setpoint(1, edition_settings.time_out_1_RS485, 1, COL_TIMEOUT_INTERFACE_COMMA, COL_TIMEOUT_INTERFACE_END, 1);
                  }
                  //Формуємо екран інформації по time-out наступного символу
                  make_ekran_timeout_interface();
                }
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                else if (current_ekran.current_level == EKRAN_SETTING_NETWORK_LAYER_ETHERNET)
                {
                  if (current_ekran.edition == 0)
                  {
                    if (--current_ekran.index_position < 0)
                      current_ekran.index_position = MAX_ROW_FOR_SETTING_NETWORK_LAYER_ETHERNET - 1;
                    position_in_current_level_menu[EKRAN_SETTING_NETWORK_LAYER_ETHERNET] = current_ekran.index_position;
                  }
                  else
                  {
                    //Редагування числа
                    if (
                      (current_ekran.index_position == INDEX_ML_NL_IPV4) ||
                      (current_ekran.index_position == INDEX_ML_NL_GATEWAY))
                    {
                      uint16_t *point_target;
                      if (current_ekran.index_position == INDEX_ML_NL_IPV4)
                      {
                        point_target = edition_settings.IP4;
                      }
                      else
                      {
                        point_target = edition_settings.gateway;
                      }

                      size_t index = (current_ekran.position_cursor_x - COL_IP4_GATEWAY_BEGIN) >> 2; /*ділення на 4, бо ХХХ. - це чотири цифри*/
                      point_target[index] = edit_setpoint(1, point_target[index], 0, 0, COL_IP4_GATEWAY_BEGIN + (3 + 1) * (index + 1) - 1 - 1, 1);
                    }
                    else if (current_ekran.index_position == INDEX_ML_NL_MASK)
                    {
                      edition_settings.mask = edit_setpoint(1, edition_settings.mask, 0, 0, COL_MASK_END, 1);
                    }
                  }
                  //Формуємо екран інфтрмації по комунікаційній адресі
                  make_ekran_settings_network_layer_Ethernet();
                }
                else if (current_ekran.current_level == EKRAN_SYNCHRO)
                {
                  if (current_ekran.edition == 0)
                  {
                    if (--current_ekran.index_position < 0)
                      current_ekran.index_position = MAX_ROW_FOR_SYNCHRO - 1;
                    position_in_current_level_menu[EKRAN_SYNCHRO] = current_ekran.index_position;
                  }
                  else
                  {
                    //Редагування числа
                    if (current_ekran.index_position == INDEX_ML_SYN_IPV4)
                    {
                      size_t index = (current_ekran.position_cursor_x - COL_IP4_SERVER_BEGIN) >> 2; /*ділення на 4, бо ХХХ. - це чотири цифри*/
                      edition_settings.IP_time_server[index] = edit_setpoint(1, edition_settings.IP_time_server[index], 0, 0, COL_IP4_SERVER_BEGIN + (3 + 1) * (index + 1) - 1 - 1, 1);
                    }
                    else if (current_ekran.index_position == INDEX_ML_SYN_PORT)
                    {
                      edition_settings.port_time_server = edit_setpoint(1, edition_settings.port_time_server, 0, 0, COL_PORT_END, 1);
                    }
                    else if (current_ekran.index_position == INDEX_ML_SYN_PERIOD)
                    {
                      edition_settings.period_sync = edit_setpoint(1, edition_settings.period_sync, 0, 0, COL_PEDIOD_END, 1);
                    }
                  }
                  //Формуємо екран
                  make_ekran_settings_synchro();
                }
#endif
                else if (current_ekran.current_level == EKRAN_TIME_ZONE)
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_FOR_TIMEZONE_SETTINGS - 1;
                  position_in_current_level_menu[EKRAN_TIME_ZONE] = current_ekran.index_position;
                  //Формуємо екран
                  make_ekran_timezone_dst();
                }
                else if (current_ekran.current_level == EKRAN_DST_RULE)
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_FOR_DST_RULE - 1;
                  position_in_current_level_menu[EKRAN_DST_RULE] = current_ekran.index_position;

                  __SETTINGS *p_settings = (current_ekran.edition == 0) ? &current_settings : &edition_settings;
                  uint32_t rule = (position_in_current_level_menu[previous_level_in_current_level_menu[EKRAN_DST_RULE]] == INDEX_ML_CHDT_DST_ON) ? p_settings->dst_on_rule : p_settings->dst_off_rule;
                  //Формуємо екран
                  make_ekran_dst_rule(rule);
                }
                else if (current_ekran.current_level == EKRAN_GENERAL_PICKUPS_EL)
                {
                  if (current_ekran.edition == 0)
                  {
                    if (--current_ekran.index_position < 0)
                      current_ekran.index_position = MAX_ROW_FOR_GENERAL_PICKUPS_EL - 1;
                    position_in_current_level_menu[EKRAN_GENERAL_PICKUPS_EL] = current_ekran.index_position;
                  }
                  else
                  {
                    //Редагування числа
                    if (current_ekran.index_position == INDEX_ML_NUMBER_INERATION)
                    {
                      edition_settings.number_iteration_el = edit_setpoint(1, edition_settings.number_iteration_el, 0, 0, COL_NUMBER_INERATION_END, 1);
                    }
                    //                  else if (current_ekran.index_position == INDEX_ML_NUMBER_DEFINED_FUNCTIONS)
                    //                  {
                    //                    edition_settings.number_defined_df = edit_setpoint(1, edition_settings.number_defined_df, 0, 0, COL_NUMBER_DEFINED_FUNCTIONS_END, 1);
                    //                  }
                    //                  else if (current_ekran.index_position == INDEX_ML_NUMBER_DEFINED_TRIGGERS)
                    //                  {
                    //                    edition_settings.number_defined_dt = edit_setpoint(1, edition_settings.number_defined_dt, 0, 0, COL_NUMBER_DEFINED_TRIGGERS_END, 1);
                    //                  }
                    //                  else if (current_ekran.index_position == INDEX_ML_NUMBER_DEFINED_AND)
                    //                  {
                    //                    edition_settings.number_defined_and = edit_setpoint(1, edition_settings.number_defined_and, 0, 0, COL_NUMBER_DEFINED_AND_END, 1);
                    //                  }
                    //                  else if (current_ekran.index_position == INDEX_ML_NUMBER_DEFINED_OR)
                    //                  {
                    //                    edition_settings.number_defined_or = edit_setpoint(1, edition_settings.number_defined_or, 0, 0, COL_NUMBER_DEFINED_OR_END, 1);
                    //                  }
                    //                  else if (current_ekran.index_position == INDEX_ML_NUMBER_DEFINED_XOR)
                    //                  {
                    //                    edition_settings.number_defined_xor = edit_setpoint(1, edition_settings.number_defined_xor, 0, 0, COL_NUMBER_DEFINED_XOR_END, 1);
                    //                  }
                    //                  else if (current_ekran.index_position == INDEX_ML_NUMBER_DEFINED_NOT)
                    //                  {
                    //                    edition_settings.number_defined_not = edit_setpoint(1, edition_settings.number_defined_not, 0, 0, COL_NUMBER_DEFINED_NOT_END, 1);
                    //                  }
                  }
                  //Формуємо екран відображення загальних витримок для розширеної логіки
                  make_ekran_general_pickups_el();
                }
                else if (current_ekran.current_level == EKRAN_LIST_TYPE_DF)
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_FOR_TYPE_DF - 1;
                  position_in_current_level_menu[EKRAN_LIST_TYPE_DF] = current_ekran.index_position;
                  //Формуємо екран відображення типу опреділювальної функції
                  make_ekran_type_df();
                }
                else if ((current_ekran.current_level >= EKRAN_TIMEOUT_DF1) && (current_ekran.current_level <= (EKRAN_TIMEOUT_DF1 + NUMBER_DEFINED_FUNCTIONS - 1)))
                {
                  if (current_ekran.edition == 0)
                  {
                    if (--current_ekran.index_position < 0)
                      current_ekran.index_position = MAX_ROW_TIMEOUT_DF - 1;
                    position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;
                  }
                  else
                  {
                    //Редагування числа
                    if (current_ekran.index_position == INDEX_ML_TMO_DF_PAUSE)
                      edition_settings.timeout_pause_df[current_ekran.current_level - EKRAN_TIMEOUT_DF1] =
                        edit_setpoint(1, edition_settings.timeout_pause_df[current_ekran.current_level - EKRAN_TIMEOUT_DF1], 1, COL_TMO_DF_PAUSE_COMMA, COL_TMO_DF_PAUSE_END, 10);
                    else
                      edition_settings.timeout_work_df[current_ekran.current_level - EKRAN_TIMEOUT_DF1] =
                        edit_setpoint(1, edition_settings.timeout_work_df[current_ekran.current_level - EKRAN_TIMEOUT_DF1], 1, COL_TMO_DF_WORK_COMMA, COL_TMO_DF_WORK_END, 10);
                  }
                  //Формуємо екран таймерів опреділюваних функцій
                  make_ekran_timeout_df(current_ekran.current_level - EKRAN_TIMEOUT_DF1);
                }
                else if ((current_ekran.current_level >= EKRAN_LIST_SOURCE_TF1) && (current_ekran.current_level <= (EKRAN_LIST_SOURCE_TF1 + NUMBER_TRANSFER_FUNCTIONS - 1)))
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_LIST_SOURCE_TF - 1;
                  //Формуємо екран
                  make_ekran_list_source_tf();
                }
                else if (current_ekran.current_level == EKRAN_TIMEOUT_DIGITAL_REGISTRATOR)
                {
                  if (current_ekran.edition == 0)
                  {
                    if (--current_ekran.index_position < 0)
                      current_ekran.index_position = MAX_ROW_FOR_TIMEOUT_DIGITAL_REGISTRATOR - 1;
                    position_in_current_level_menu[EKRAN_TIMEOUT_DIGITAL_REGISTRATOR] = current_ekran.index_position;
                  }
                  else
                  {
                    //Редагування числа
                    edition_settings.timeout_prolongation_work_digital_registrator =
                      edit_setpoint(1, edition_settings.timeout_prolongation_work_digital_registrator, 1, COL_TMO_ELONGATION_COMMA, COL_TMO_ELONGATION_END, 10);
                  }
                  //Формуємо екран витримок аналогового реєстратора
                  make_ekran_timeout_digital_registrator();
                }
                else if (current_ekran.current_level == EKRAN_TIMEOUT_ANALOG_REGISTRATOR)
                {
                  if (current_ekran.edition == 0)
                  {
                    if (--current_ekran.index_position < 0)
                      current_ekran.index_position = MAX_ROW_FOR_TIMEOUT_ANALOG_REGISTRATOR - 1;
                    position_in_current_level_menu[EKRAN_TIMEOUT_ANALOG_REGISTRATOR] = current_ekran.index_position;
                  }
                  else
                  {
                    //Редагування числа
                    if (current_ekran.index_position == INDEX_ML_TMOPREFAULT)
                    {
                      unsigned int temp_setpoint = edition_settings.prefault_number_periods * 20;
                      do
                      {
                        //Величину витримки збільшуємо почергого на 10 мс (ф-ція edit_setpoint збільшує/зменшує з крогом кратних 1, 10 і т.д.), щоб отримати крок 20 мс
                        temp_setpoint = edit_setpoint(1, temp_setpoint, 1, COL_TMO_PREFAULT_COMMA, COL_TMO_PREFAULT_END, 10);
                      } while ((temp_setpoint % 20) != 0);
                      edition_settings.prefault_number_periods = temp_setpoint / 20;
                    }
                    else
                    {
                      unsigned int temp_setpoint = edition_settings.postfault_number_periods * 20;
                      do
                      {
                        //Величину витримки збільшуємо почергого на 10 мс (ф-ція edit_setpoint збільшує/зменшує з крогом кратних 1, 10 і т.д.), щоб отримати крок 20 мс
                        temp_setpoint = edit_setpoint(1, temp_setpoint, 1, COL_TMO_PREFAULT_COMMA, COL_TMO_PREFAULT_END, 10);
                      } while ((temp_setpoint % 20) != 0);
                      edition_settings.postfault_number_periods = temp_setpoint / 20;
                    }
                  }
                  //Формуємо екран витримок аналогового реєстратора
                  make_ekran_timeout_analog_registrator();
                }
                else if (current_ekran.current_level == EKRAN_CONTROL_AR)
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_FOR_CONTROL_AR - 1;
                  position_in_current_level_menu[EKRAN_CONTROL_AR] = current_ekran.index_position;
                  //Формуємо екран відображення
                  make_ekran_control_ar();
                }
                else if (current_ekran.current_level == EKRAN_VIEW_SETTING_LANGUAGE)
                {
                  if (current_ekran.edition == 0)
                  {
                    if (--current_ekran.index_position < 0)
                      current_ekran.index_position = MAX_ROW_FOR_VIEW_SETTING_LANGUAGE - 1;
                    position_in_current_level_menu[EKRAN_VIEW_SETTING_LANGUAGE] = current_ekran.index_position;
                  }
                  else
                  {
                    //Редагування числа
                    edition_settings.language--;
                    if (edition_settings.language < VALUE_SETTING_LANGUAGE_MIN)
                      edition_settings.language = VALUE_SETTING_LANGUAGE_MAX;
                  }
                  //Формуємо екран інформації по мові меню
                  make_ekran_setting_language();
                }
                else if (current_ekran.current_level == EKRAN_CHOSE_EXTRA_SETTINGS)
                {
                  if (--current_ekran.index_position < 0)
                    current_ekran.index_position = MAX_ROW_FOR_CHOSE_EXTRA_SETTINGS - 1;
                  position_in_current_level_menu[EKRAN_CHOSE_EXTRA_SETTINGS] = current_ekran.index_position;
                  //Формуємо екран відображення додаткових налаштувань
                  make_ekran_chose_extra_settings();
                }
                else if (current_ekran.current_level == EKRAN_VIEW_GRUPA_USTAVOK)
                {
                  if (current_ekran.edition == 0)
                  {
                    if (--current_ekran.index_position < 0)
                      current_ekran.index_position = MAX_ROW_FOR_VIEW_GRUPA_USTAVOK - 1;
                    position_in_current_level_menu[EKRAN_VIEW_GRUPA_USTAVOK] = current_ekran.index_position;
                  }
                  else
                  {
                    //Редагування числа
                    edition_settings.grupa_ustavok--;
                    if (edition_settings.grupa_ustavok < SETPOINT_GRUPA_USTAVOK_MIN)
                      edition_settings.grupa_ustavok = SETPOINT_GRUPA_USTAVOK_MAX;
                  }
                  //Формуємо екран інформації по групах уставок
                  make_ekran_grupa_ustavok();
                }

                //Очистити сигналізацію, що натиснута кнопка
                new_state_keyboard &= ~(1u << BIT_KEY_UP);
              }
              else if (new_state_keyboard == (1u << BIT_KEY_DOWN))
              {
                //Натиснута кнопка DOWN
                if (
                  (current_ekran.current_level >= EKRAN_SETPOINT_RPN_GROUP1) &&
                  (current_ekran.current_level <= EKRAN_SETPOINT_RPN_GROUP4))
                {
                  int group = (current_ekran.current_level - EKRAN_SETPOINT_RPN_GROUP1);

                  if (current_ekran.edition == 0)
                  {
                    if (++current_ekran.index_position >= MAX_ROW_FOR_SETPOINT_RPN)
                      current_ekran.index_position = 0;
                    position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;
                  }
                  else
                  {
                    //Редагування числа
                    if (current_ekran.index_position == INDEX_ML_STPRPN_OSN)
                      edition_settings.setpoint_rpn_osn[group] = edit_setpoint(0, edition_settings.setpoint_rpn_osn[group], 1, COL_SETPOINT_RPN_OSN_COMMA, COL_SETPOINT_RPN_OSN_END, 100);
                    else if (current_ekran.index_position == INDEX_ML_STPRPN_ZONE)
                      edition_settings.setpoint_rpn_zony[group] = edit_setpoint(0, edition_settings.setpoint_rpn_zony[group], 1, COL_SETPOINT_RPN_ZONE_COMMA, COL_SETPOINT_RPN_ZONE_END, 10);
                    else if (current_ekran.index_position == INDEX_ML_STPRPN_MAX_PER)
                      edition_settings.setpoint_rpn_per[group] = edit_setpoint(0, edition_settings.setpoint_rpn_per[group], 0, 0, COL_SETPOINT_RPN_MAX_PER_END, 1);
                    else if (current_ekran.index_position == INDEX_ML_STPRPN_DOD)
                      edition_settings.setpoint_rpn_dod[group] = edit_setpoint(0, edition_settings.setpoint_rpn_dod[group], 0, COL_SETPOINT_RPN_DOD_COMMA, COL_SETPOINT_RPN_DOD_END, 100);
                    else if (current_ekran.index_position == INDEX_ML_STPRPN_K)
                      edition_settings.setpoint_rpn_K[group] = edit_setpoint(0, edition_settings.setpoint_rpn_K[group], 1, COL_SETPOINT_RPN_K_COMMA, COL_SETPOINT_RPN_K_END, 1);
                  }
                  //Формуємо екран уставок РПН
                  make_ekran_setpoint_rpn(group);
                }
                else if (
                  (current_ekran.current_level >= EKRAN_TIMEOUT_RPN_GROUP1) &&
                  (current_ekran.current_level <= EKRAN_TIMEOUT_RPN_GROUP4))
                {
                  int group = (current_ekran.current_level - EKRAN_TIMEOUT_RPN_GROUP1);

                  if (current_ekran.edition == 0)
                  {
                    if (++current_ekran.index_position >= MAX_ROW_FOR_TIMEOUT_RPN)
                      current_ekran.index_position = 0;
                    position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;
                  }
                  else
                  {
                    //Редагування числа
                    if (current_ekran.index_position == INDEX_ML_TMORPN_UB_PRYB)
                      edition_settings.timeout_rpn_ub_pryb[group] = edit_setpoint(0, edition_settings.timeout_rpn_ub_pryb[group], 0, 0, COL_TMO_RPN_UB_PRYB_END, 1000);
                    else if (current_ekran.index_position == INDEX_ML_TMORPN_UB_PRYB_PRYSK)
                      edition_settings.timeout_rpn_ub_pryb_prysk[group] = edit_setpoint(0, edition_settings.timeout_rpn_ub_pryb_prysk[group], 0, 0, COL_TMO_RPN_UB_PRYB_PRYSK_END, 1000);
                    else if (current_ekran.index_position == INDEX_ML_TMORPN_PRYSK_VID_UMAX)
                      edition_settings.timeout_rpn_prysk_vid_Umax[group] = edit_setpoint(0, edition_settings.timeout_rpn_prysk_vid_Umax[group], 1, COL_TMO_RPN_PRYSK_VID_UMAX_COMMA, COL_TMO_RPN_PRYSK_VID_UMAX_END, 100);
                    else if (current_ekran.index_position == INDEX_ML_TMORPN_PEREKL)
                      edition_settings.timeout_rpn_perekl[group] = edit_setpoint(0, edition_settings.timeout_rpn_perekl[group], 1, COL_TMO_RPN_PEREKL_COMMA, COL_TMO_RPN_PEREKL_END, 10);
                    else if (current_ekran.index_position == INDEX_ML_TMORPN_NESPR)
                      edition_settings.timeout_rpn_nespr[group] = edit_setpoint(0, edition_settings.timeout_rpn_nespr[group], 1, COL_TMO_RPN_NESPR_COMMA, COL_TMO_RPN_NESPR_END, 10);
                  }
                  //Формуємо екран витримок РПН
                  make_ekran_timeout_rpn(group);
                }
                else if (current_ekran.current_level == EKRAN_CONTROL_RPN)
                {
                  if (++current_ekran.index_position >= MAX_ROW_FOR_CONTROL_RPN)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[EKRAN_CONTROL_RPN] = current_ekran.index_position;
                  //Формуємо екран управлінської інформації для РПН
                  make_ekran_control_rpn();
                }
                else if (current_ekran.current_level == EKRAN_CONTROL_SZKh)
                {
                  if (++current_ekran.index_position >= MAX_ROW_FOR_CONTROL_SZKh)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[EKRAN_CONTROL_SZKh] = current_ekran.index_position;
                  //Формуємо екран управлінської інформації для ЗСХ
                  make_ekran_control_zskh();
                }
                else if (
                  (current_ekran.current_level >= EKRAN_SETPOINT_BRP_GROUP1) &&
                  (current_ekran.current_level <= EKRAN_SETPOINT_BRP_GROUP4))
                {
                  int group = (current_ekran.current_level - EKRAN_SETPOINT_BRP_GROUP1);

                  if (current_ekran.edition == 0)
                  {
                    if (++current_ekran.index_position >= MAX_ROW_FOR_SETPOINT_BRP)
                      current_ekran.index_position = 0;
                    position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;
                  }
                  else
                  {
                    //Редагування числа
                    if (current_ekran.index_position == INDEX_ML_STPBRP_I_BLK)
                      edition_settings.setpoint_brp_I_blk[group] = edit_setpoint(0, edition_settings.setpoint_brp_I_blk[group], 1, COL_SETPOINT_BRP_I_BLK_COMMA, COL_SETPOINT_BRP_I_BLK_END, 1);
                  }
                  //Формуємо екран уставок БРП
                  make_ekran_setpoint_brp(group);
                }
                else if (current_ekran.current_level == EKRAN_CONTROL_BRP)
                {
                  if (++current_ekran.index_position >= MAX_ROW_FOR_CONTROL_BRP)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[EKRAN_CONTROL_BRP] = current_ekran.index_position;
                  //Формуємо екран управлінської інформації для БРП
                  make_ekran_control_brp();
                }
                else if (
                  (current_ekran.current_level >= EKRAN_TIMEOUT_ZNKh_GROUP1) &&
                  (current_ekran.current_level <= EKRAN_TIMEOUT_ZNKh_GROUP4))
                {
                  int group = (current_ekran.current_level - EKRAN_TIMEOUT_ZNKh_GROUP1);

                  if (current_ekran.edition == 0)
                  {
                    if (++current_ekran.index_position >= MAX_ROW_FOR_TIMEOUT_ZNKh)
                      current_ekran.index_position = 0;
                    position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;
                  }
                  else
                  {
                    //Редагування числа
                    if (current_ekran.index_position == INDEX_ML_TMOZNKh_PEREKL)
                    {
                      unsigned int temp_setpoint = edition_settings.timeout_znkh_perekl[group];
                      if ((temp_setpoint % 2) != 0)
                        temp_setpoint &= (~1u); //Це випадок коли прошивка змінилася з 2мс  циклом де допуск був встановлений непарнмй допуск
                      do
                      {
                        //Величину витримки зменшуємо почергого на 1 мс (ф-ція edit_setpoint збільшує/зменшує з кроком кратних 1, 10 і т.д.), щоб отримати крок 2 мс
                        temp_setpoint = edit_setpoint(0, temp_setpoint, 1, COL_TMO_ZNKh_PEREKL_COMMA, COL_TMO_ZNKh_PEREKL_END, 1);
                      } while ((temp_setpoint % 2) != 0);
                      edition_settings.timeout_znkh_perekl[group] = temp_setpoint;
                    }
                  }
                  //Формуємо екран витримок ЗНХ
                  make_ekran_timeout_znkh(group);
                }
                else if (current_ekran.current_level == EKRAN_CONTROL_ZNKh)
                {
                  current_ekran.index_position++;

                  if (current_ekran.index_position >= MAX_ROW_FOR_CONTROL_ZNKh)
                    current_ekran.index_position = 0;
                  // while (
                  //   (
                  //     (
                  //       (current_ekran.index_position == CTR_ZDZ_STARTED_FROM_MTZ1_BIT) ||
                  //       (current_ekran.index_position == CTR_ZDZ_STARTED_FROM_MTZ2_BIT) ||
                  //       (current_ekran.index_position == CTR_ZDZ_STARTED_FROM_MTZ3_BIT) ||
                  //       (current_ekran.index_position == CTR_ZDZ_STARTED_FROM_MTZ4_BIT)) &&
                  //     ((current_settings.configuration & (1 << MTZ_BIT_CONFIGURATION)) == 0)) ||
                  //   ((
                  //      (current_ekran.index_position == CTR_ZDZ_STARTED_FROM_UMIN1_BIT) ||
                  //      (current_ekran.index_position == CTR_ZDZ_STARTED_FROM_UMIN2_BIT)) &&
                  //    ((current_settings.configuration & (1 << UMIN_BIT_CONFIGURATION)) == 0)))
                  // {
                  //   current_ekran.index_position++;
                  //   if (current_ekran.index_position >= MAX_ROW_FOR_CONTROL_ZNKh)
                  //     current_ekran.index_position = 0;
                  // }

                  position_in_current_level_menu[EKRAN_CONTROL_ZNKh] = current_ekran.index_position;

                  //Формуємо екран управлінської інформації для ЗНХ
                  make_ekran_control_znkh();
                }
                else if (
                  (current_ekran.current_level >= EKRAN_SETPOINT_UMIN_GROUP1) &&
                  (current_ekran.current_level <= EKRAN_SETPOINT_UMIN_GROUP4))
                {
                  int group = (current_ekran.current_level - EKRAN_SETPOINT_UMIN_GROUP1);

                  if (current_ekran.edition == 0)
                  {
                    if (++current_ekran.index_position >= MAX_ROW_FOR_SETPOINT_UMIN)
                      current_ekran.index_position = 0;
                    position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;
                  }
                  else
                  {
                    //Редагування числа
                    if (current_ekran.index_position == INDEX_ML_STPUmin_Umin1)
                      edition_settings.setpoint_Umin1[group] = edit_setpoint(0, edition_settings.setpoint_Umin1[group], 0, 0, COL_SETPOINT_Umin_Umin1_END, 1000);
                    else if (current_ekran.index_position == INDEX_ML_STPUmin_Umin2)
                      edition_settings.setpoint_Umin2[group] = edit_setpoint(0, edition_settings.setpoint_Umin2[group], 0, 0, COL_SETPOINT_Umin_Umin2_END, 1000);
                  }
                  //Формуємо екран уставок Umin
                  make_ekran_setpoint_Umin(group);
                }
                else if (
                  (current_ekran.current_level >= EKRAN_TIMEOUT_UMIN_GROUP1) &&
                  (current_ekran.current_level <= EKRAN_TIMEOUT_UMIN_GROUP4))
                {
                  int group = (current_ekran.current_level - EKRAN_TIMEOUT_UMIN_GROUP1);

                  if (current_ekran.edition == 0)
                  {
                    if (++current_ekran.index_position >= MAX_ROW_FOR_TIMEOUT_UMIN)
                      current_ekran.index_position = 0;
                    position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;
                  }
                  else
                  {
                    //Редагування числа
                    if (current_ekran.index_position == INDEX_ML_TMOUmin_Umin1)
                      edition_settings.timeout_Umin1[group] = edit_setpoint(0, edition_settings.timeout_Umin1[group], 1, COL_TMO_Umin_Umin1_COMMA, COL_TMO_Umin_Umin1_END, 100);
                    else if (current_ekran.index_position == INDEX_ML_TMOUmin_Umin2)
                      edition_settings.timeout_Umin2[group] = edit_setpoint(0, edition_settings.timeout_Umin2[group], 1, COL_TMO_Umin_Umin2_COMMA, COL_TMO_Umin_Umin2_END, 100);
                  }
                  //Формуємо екран витримок Umin
                  make_ekran_timeout_Umin(group);
                }
                else if (current_ekran.current_level == EKRAN_CONTROL_UMIN)
                {
                  if (++current_ekran.index_position >= MAX_ROW_FOR_CONTROL_UMIN)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[EKRAN_CONTROL_UMIN] = current_ekran.index_position;
                  //Формуємо екран управлінської інформації для Umin
                  make_ekran_control_Umin();
                }
                else if (
                  (current_ekran.current_level >= EKRAN_SETPOINT_UMAX_GROUP1) &&
                  (current_ekran.current_level <= EKRAN_SETPOINT_UMAX_GROUP4))
                {
                  int group = (current_ekran.current_level - EKRAN_SETPOINT_UMAX_GROUP1);

                  if (current_ekran.edition == 0)
                  {
                    if (++current_ekran.index_position >= MAX_ROW_FOR_SETPOINT_Umax)
                      current_ekran.index_position = 0;
                    position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;
                  }
                  else
                  {
                    //Редагування числа
                    if (current_ekran.index_position == INDEX_ML_STPUmax_Umax1)
                      edition_settings.setpoint_Umax1[group] = edit_setpoint(0, edition_settings.setpoint_Umax1[group], 0, 0, COL_SETPOINT_Umax_Umax1_END, 1000);
                    else if (current_ekran.index_position == INDEX_ML_STPUmax_Umax2)
                      edition_settings.setpoint_Umax2[group] = edit_setpoint(0, edition_settings.setpoint_Umax2[group], 0, 0, COL_SETPOINT_Umax_Umax2_END, 1000);
                  }
                  //Формуємо екран уставок Umax
                  make_ekran_setpoint_Umax(group);
                }
                else if (
                  (current_ekran.current_level >= EKRAN_TIMEOUT_UMAX_GROUP1) &&
                  (current_ekran.current_level <= EKRAN_TIMEOUT_UMAX_GROUP4))
                {
                  int group = (current_ekran.current_level - EKRAN_TIMEOUT_UMAX_GROUP1);

                  if (current_ekran.edition == 0)
                  {
                    if (++current_ekran.index_position >= MAX_ROW_FOR_TIMEOUT_UMAX)
                      current_ekran.index_position = 0;
                    position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;
                  }
                  else
                  {
                    //Редагування числа
                    if (current_ekran.index_position == INDEX_ML_TMOUmax_Umax2)
                      edition_settings.timeout_Umax2[group] = edit_setpoint(0, edition_settings.timeout_Umax2[group], 1, COL_TMO_Umax_Umax2_COMMA, COL_TMO_Umax_Umax2_END, 100);
                  }
                  //Формуємо екран витримок Umax
                  make_ekran_timeout_Umax(group);
                }
                else if (current_ekran.current_level == EKRAN_CONTROL_UMAX)
                {
                  if (++current_ekran.index_position >= MAX_ROW_FOR_CONTROL_UMAX)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[EKRAN_CONTROL_UMAX] = current_ekran.index_position;
                  //Формуємо екран управлінської інформації для Umax
                  make_ekran_control_Umax();
                }
                else if (
                  (current_ekran.current_level >= EKRAN_SETPOINT_UP_GROUP1) &&
                  (current_ekran.current_level <= EKRAN_SETPOINT_UP_GROUP4))
                {
                  int group = (current_ekran.current_level - EKRAN_SETPOINT_UP_GROUP1);

                  if (current_ekran.edition == 0)
                  {
                    if (++current_ekran.index_position >= (NUMBER_UP * MAX_ROW_FOR_SETPOINT_UP))
                      current_ekran.index_position = 0;
                    position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;
                  }
                  else
                  {
                    //Редагування числа
                    uint32_t _n_UP = current_ekran.index_position / MAX_ROW_FOR_SETPOINT_UP;
                    uint32_t _n_index = current_ekran.index_position % MAX_ROW_FOR_SETPOINT_UP;

                    if (_n_index == INDEX_ML_STP_UP)
                    {
                      uint32_t comma = 0, end = 0, min_step = 0;
                      int32_t sign_before = 1, sign_after = 1;
                      switch (current_settings.ctrl_UP_input[_n_UP])
                      {
                        case UP_CTRL_Ia_Ib_Ic:
                        case UP_CTRL_Ia:
                        case UP_CTRL_Ib:
                        case UP_CTRL_Ic:
                        case UP_CTRL_I1:
                        case UP_CTRL_I2:
                        case UP_CTRL_I04:
                        case UP_CTRL_3I0_r:
                          {
                            comma = COL_SETPOINT_UP_I_COMMA;
                            end = COL_SETPOINT_UP_I_END;
                            min_step = 10;
                            break;
                          }
                        case UP_CTRL_3I0:
                        case UP_CTRL_3I0_others:
                          {
                            comma = COL_SETPOINT_UP_3I0_COMMA;
                            end = COL_SETPOINT_UP_3I0_END;
                            min_step = 1;
                            break;
                          }
                        case UP_CTRL_Ua_Ub_Uc:
                        case UP_CTRL_Uab_Ubc_Uca:
                        case UP_CTRL_Ua:
                        case UP_CTRL_Uab:
                        case UP_CTRL_Ub:
                        case UP_CTRL_Ubc:
                        case UP_CTRL_Uc:
                        case UP_CTRL_Uca:
                        case UP_CTRL_U1:
                        case UP_CTRL_U2:
                        case UP_CTRL_3U0:
                          {
                            comma = COL_SETPOINT_UP_U_COMMA;
                            end = COL_SETPOINT_UP_U_END;
                            min_step = 100;
                            break;
                          }
                        case UP_CTRL_P:
                        case UP_CTRL_Q:
                          {
                            if (edition_settings.setpoint_UP[_n_UP][0][group] < 0)
                              sign_before = -1;
                            if (current_ekran.position_cursor_x == (COL_SETPOINT_UP_PQ_BEGIN - 1))
                            {
                              sign_after = -sign_before;
                            }
                            else
                            {
                              sign_after = sign_before;
                              comma = COL_SETPOINT_UP_PQ_COMMA;
                              end = COL_SETPOINT_UP_PQ_END;
                              min_step = 1;
                            }
                            break;
                          }
                        case UP_CTRL_S:
                          {
                            comma = COL_SETPOINT_UP_S_COMMA;
                            end = COL_SETPOINT_UP_S_END;
                            min_step = 1;
                            break;
                          }
                        default:
                          {
                            //Теоретично цього ніколи не мало б бути
                            total_error_sw_fixed();
                          }
                      }

                      if (sign_before == sign_after)
                        edition_settings.setpoint_UP[_n_UP][0][group] = sign_after * edit_setpoint(0, abs(edition_settings.setpoint_UP[_n_UP][0][group]), 1, comma, end, min_step);
                      else
                        edition_settings.setpoint_UP[_n_UP][0][group] = sign_after * abs(edition_settings.setpoint_UP[_n_UP][0][group]);
                    }
                    else if (_n_index == INDEX_ML_STP_UP_KP)
                      edition_settings.setpoint_UP_KP[_n_UP][0][group] = edit_setpoint(0, edition_settings.setpoint_UP_KP[_n_UP][0][group], 1, COL_SETPOINT_UP_KP_COMMA, COL_SETPOINT_UP_KP_END, 1);
                  }
                  //Формуємо екран уставок УЗ
                  make_ekran_setpoint_UP(group);
                }
                else if (
                  (current_ekran.current_level >= EKRAN_TIMEOUT_UP_GROUP1) &&
                  (current_ekran.current_level <= EKRAN_TIMEOUT_UP_GROUP4))
                {
                  int group = (current_ekran.current_level - EKRAN_TIMEOUT_UP_GROUP1);

                  if (current_ekran.edition == 0)
                  {
                    if (++current_ekran.index_position >= (NUMBER_UP * MAX_ROW_FOR_TIMEOUT_UP))
                      current_ekran.index_position = 0;
                    position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;
                  }
                  else
                  {
                    //Редагування числа
                    uint32_t _n_UP = current_ekran.index_position / MAX_ROW_FOR_TIMEOUT_UP;
                    uint32_t _n_index = current_ekran.index_position % MAX_ROW_FOR_TIMEOUT_UP;

                    if (_n_index == INDEX_ML_TMOUP)
                      edition_settings.timeout_UP[_n_UP][0][group] = edit_setpoint(0, edition_settings.timeout_UP[_n_UP][0][group], 1, COL_TMO_UP_COMMA, COL_TMO_UP_END, 10);
                  }
                  //Формуємо екран витримок УЗ
                  make_ekran_timeout_UP(group);
                }
                else if (current_ekran.current_level == EKRAN_CONTROL_UP)
                {
                  current_ekran.index_position++;
                  if (current_ekran.index_position >= (NUMBER_UP * MAX_ROW_FOR_CONTROL_UP))
                    current_ekran.index_position = 0;

                  __SETTINGS *point = (current_ekran.edition == 0) ? &current_settings : &edition_settings;
                  uint32_t ctrl_UP_input = point->ctrl_UP_input[current_ekran.index_position / (_CTR_UP_NEXT_BIT - _CTR_UP_PART_I)];
                  while (
                    ((current_ekran.index_position % (_CTR_UP_NEXT_BIT - _CTR_UP_PART_I)) == CTR_UP_OR_AND_BIT) &&
                    (ctrl_UP_input != UP_CTRL_Ia_Ib_Ic) &&
                    (ctrl_UP_input != UP_CTRL_Ua_Ub_Uc) &&
                    (ctrl_UP_input != UP_CTRL_Uab_Ubc_Uca))
                  {
                    current_ekran.index_position++;
                    if (current_ekran.index_position >= (NUMBER_UP * MAX_ROW_FOR_CONTROL_UP))
                      current_ekran.index_position = 0;

                    ctrl_UP_input = point->ctrl_UP_input[current_ekran.index_position / (_CTR_UP_NEXT_BIT - _CTR_UP_PART_I)];
                  }

                  position_in_current_level_menu[EKRAN_CONTROL_UP] = current_ekran.index_position;

                  //Формуємо екран управлінської інформації для УЗ
                  make_ekran_control_UP();
                }
                else if (current_ekran.current_level == EKRAN_TRANSFORMATOR_INFO)
                {
                  if (current_ekran.edition == 0)
                  {
                    current_ekran.index_position++;
                    do
                    {
                      if (current_ekran.index_position >= MAX_ROW_FOR_TRANSFORMATOR_INFO)
                        current_ekran.index_position = 0;

                      while (
                        (
                          (current_ekran.index_position == INDEX_ML_TT2) ||
                          (current_ekran.index_position == INDEX_ML_TN2)) &&
                        ((current_settings.control_rpn & MASKA_FOR_BIT(INDEX_ML_CTRRPN_TRANSF)) == 0))
                        current_ekran.index_position++;
                    } while (current_ekran.index_position >= MAX_ROW_FOR_TRANSFORMATOR_INFO);

                    position_in_current_level_menu[EKRAN_TRANSFORMATOR_INFO] = current_ekran.index_position;
                  }
                  else
                  {
                    //Редагування числа
                    if (current_ekran.index_position == INDEX_ML_TT1)
                      edition_settings.TCurrent1 = edit_setpoint(0, edition_settings.TCurrent1, 0, 0, COL_TT_END, 1);
                    else if (current_ekran.index_position == INDEX_ML_TT2)
                      edition_settings.TCurrent2 = edit_setpoint(0, edition_settings.TCurrent2, 0, 0, COL_TT_END, 1);
                    else if (current_ekran.index_position == INDEX_ML_TN1)
                      edition_settings.TVoltage1 = edit_setpoint(0, edition_settings.TVoltage1, 0, 0, COL_TN_END, 1);
                    else
                      edition_settings.TVoltage2 = edit_setpoint(0, edition_settings.TVoltage2, 0, 0, COL_TN_END, 1);
                  }
                  //Формуємо екран інфтрмації по трансформаторах
                  make_ekran_transformator();
                }
                else if (current_ekran.current_level == EKRAN_SETPOINT_SWITCH)
                {
                  if (current_ekran.edition == 0)
                  {
                    if (++current_ekran.index_position >= MAX_ROW_FOR_SETPOINT_SWITCH)
                      current_ekran.index_position = 0;
                    position_in_current_level_menu[EKRAN_SETPOINT_SWITCH] = current_ekran.index_position;
                  }
                  else
                  {
                    //Редагування числа
                    if (current_ekran.index_position == INDEX_ML_STPInom)
                      edition_settings.setpoint_Inom = edit_setpoint(0, edition_settings.setpoint_Inom, 0, 0, COL_SETPOINT_Inom_END, 1);
                    if (current_ekran.index_position == INDEX_ML_STPRKS_Inom)
                      edition_settings.setpoint_r_kom_st_Inom = edit_setpoint(0, edition_settings.setpoint_r_kom_st_Inom, 0, 0, COL_SETPOINT_RKS_Inom_END, 1);
                    else if (current_ekran.index_position == INDEX_ML_STPMInom_vymk)
                      edition_settings.setpoint_Inom_vymk = edit_setpoint(0, edition_settings.setpoint_Inom_vymk, 1, COL_SETPOINT_Inom_vymk_COMMA, COL_SETPOINT_Inom_vymk_END, 1);
                    else if (current_ekran.index_position == INDEX_ML_STPRKS_Inom_vymk)
                      edition_settings.setpoint_r_kom_st_Inom_vymk = edit_setpoint(0, edition_settings.setpoint_r_kom_st_Inom_vymk, 0, 0, COL_SETPOINT_RKS_Inom_vymk_END, 1);
                    else if (current_ekran.index_position == INDEX_ML_STPPOCHATKOVYJ_RESURS)
                      edition_settings.setpoint_pochatkovyj_resurs = edit_setpoint(0, edition_settings.setpoint_pochatkovyj_resurs, 0, 0, COL_SETPOINT_POCHATKOVYJ_RESURS_END, 1);
                    else if (current_ekran.index_position == INDEX_ML_STPKRYTYCHNYJ_RESURS)
                      edition_settings.setpoint_krytychnyj_resurs = edit_setpoint(0, edition_settings.setpoint_krytychnyj_resurs, 0, 0, COL_SETPOINT_KRYTYCHNYJ_RESURS_END, 1);
                    else if (current_ekran.index_position == INDEX_ML_STPPOCHATKOVA_K_VYMK)
                      edition_settings.setpoint_pochatkova_k_vymk = edit_setpoint(0, edition_settings.setpoint_pochatkova_k_vymk, 0, 0, COL_SETPOINT_POCHATKOVA_K_VYMK_END, 1);
                  }
                  //Формуємо екран уставок виключателя
                  make_ekran_setpoint_switch();
                }
                else if (current_ekran.current_level == EKRAN_TIMEOUT_SWITCH)
                {
                  if (current_ekran.edition == 0)
                  {
                    if (++current_ekran.index_position >= MAX_ROW_FOR_TIMEOUT_SWITCH)
                      current_ekran.index_position = 0;
                    position_in_current_level_menu[EKRAN_TIMEOUT_SWITCH] = current_ekran.index_position;
                  }
                  else
                  {
                    //Редагування числа
                    if (current_ekran.index_position == INDEX_ML_TMOON)
                      edition_settings.timeout_swch_on = edit_setpoint(0, edition_settings.timeout_swch_on, 1, COL_TMO_SWCH_ON_COMMA, COL_TMO_SWCH_ON_END, 10);
                    else if (current_ekran.index_position == INDEX_ML_TMOOFF)
                      edition_settings.timeout_swch_off = edit_setpoint(0, edition_settings.timeout_swch_off, 1, COL_TMO_SWCH_OFF_COMMA, COL_TMO_SWCH_OFF_END, 10);
                    else if (current_ekran.index_position == INDEX_ML_TMOUDL_BLK_ON)
                      edition_settings.timeout_swch_udl_blk_on = edit_setpoint(0, edition_settings.timeout_swch_udl_blk_on, 1, COL_TMO_SWCH_UDL_BLK_ON_COMMA, COL_TMO_SWCH_UDL_BLK_ON_END, 10);
                    else if (current_ekran.index_position == INDEX_ML_TMOPRYVODA_VV)
                      edition_settings.timeout_pryvoda_VV = edit_setpoint(0, edition_settings.timeout_pryvoda_VV, 1, COL_TMO_PRYVODA_VV_COMMA, COL_TMO_PRYVODA_VV_END, 10);
                  }
                  //Формуємо екран витримок виключателя
                  make_ekran_timeout_switch();
                }
                else if (current_ekran.current_level == EKRAN_CONTROL_SWITCH)
                {
                  if (++current_ekran.index_position >= MAX_ROW_FOR_CONTROL_SWITCH)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[EKRAN_CONTROL_SWITCH] = current_ekran.index_position;
                  //Формуємо екран управлінської інформації для вимикача
                  make_ekran_control_switch();
                }
                else if (current_ekran.current_level == EKRAN_DOPUSK_DV_UVV)
                {
                  if (current_ekran.edition == 0)
                  {
                    if (++current_ekran.index_position >= NUMBER_INPUTS)
                      current_ekran.index_position = 0;
                    position_in_current_level_menu[EKRAN_DOPUSK_DV_UVV] = current_ekran.index_position;
                  }
                  else
                  {
                    //Редагування числа
                    edition_settings.dopusk_dv[current_ekran.index_position] =
                      edit_setpoint(0, edition_settings.dopusk_dv[current_ekran.index_position], 0, 0, COL_DOPUSK_DV_END, 1);
                  }
                  //Формуємо екран інфтрмації по допусках ДВ
                  make_ekran_dopusk_dv();
                }
                else if ((current_ekran.current_level == EKRAN_TYPE_INPUT_UVV) || (current_ekran.current_level == EKRAN_TYPE_INPUT_SIGNAL_UVV))
                {
                  if (++current_ekran.index_position >= NUMBER_INPUTS)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;
                  //Формуємо екран управлінської інформації для УВВ
                  if (current_ekran.current_level == EKRAN_TYPE_INPUT_UVV)
                    make_ekran_type_input_uvv(0);
                  else
                    make_ekran_type_input_uvv(1);
                }
                else if (current_ekran.current_level == EKRAN_TYPE_OUTPUT_UVV)
                {
                  if (++current_ekran.index_position >= NUMBER_SIMPLE_OUTPUTS)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[EKRAN_TYPE_OUTPUT_UVV] = current_ekran.index_position;
                  //Формуємо екран типу виходів
                  make_ekran_type_output_uvv();
                }
                else if (current_ekran.current_level == EKRAN_TYPE_LED_UVV)
                {
                  if (++current_ekran.index_position >= NUMBER_LEDS)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[EKRAN_TYPE_LED_UVV] = current_ekran.index_position;
                  //Формуємо екран типу світлоіндикаторів
                  make_ekran_type_led_uvv();
                }
                else if (current_ekran.current_level == EKRAN_TYPE_BUTTON_UVV)
                {
                  if (++current_ekran.index_position >= NUMBER_DEFINED_BUTTONS)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[EKRAN_TYPE_BUTTON_UVV] = current_ekran.index_position;
                  //Формуємо екран типу ФК
                  make_ekran_type_button_uvv();
                }
                else if (current_ekran.current_level == EKRAN_ADDRESS_RS485)
                {
                  if (current_ekran.edition == 0)
                  {
                    if (++current_ekran.index_position >= MAX_ROW_FOR_ADDRESS)
                      current_ekran.index_position = 0;
                    position_in_current_level_menu[EKRAN_ADDRESS_RS485] = current_ekran.index_position;
                  }
                  else
                  {
                    //Редагування числа
                    edition_settings.address = edit_setpoint(0, edition_settings.address, 0, 0, COL_ADDRESS_END, 1);
                  }
                  //Формуємо екран інфтрмації по комунікаційній адресі
                  make_ekran_address();
                }
                else if (current_ekran.current_level == EKRAN_VIEW_SPEED_RS485)
                {
                  if (current_ekran.edition == 0)
                  {
                    if (++current_ekran.index_position >= MAX_ROW_FOR_VIEW_SPEED_INTERFACE)
                      current_ekran.index_position = 0;
                    position_in_current_level_menu[EKRAN_VIEW_SPEED_RS485] = current_ekran.index_position;
                  }
                  else
                  {
                    //Редагування числа
                    edition_settings.speed_RS485++;
                    if (edition_settings.speed_RS485 > VALUE_SPEED_INTERFACE_MAX)
                      edition_settings.speed_RS485 = VALUE_SPEED_INTERFACE_MIN;
                  }
                  //Формуємо екран інформації по швидкості обміну
                  make_ekran_speed_interface();
                }
                else if (current_ekran.current_level == EKRAN_VIEW_PARE_RS485)
                {
                  if (current_ekran.edition == 0)
                  {
                    if (++current_ekran.index_position >= MAX_ROW_FOR_VIEW_PARE_INTERFACE)
                      current_ekran.index_position = 0;
                    position_in_current_level_menu[EKRAN_VIEW_PARE_RS485] = current_ekran.index_position;
                  }
                  else
                  {
                    //Редагування числа
                    edition_settings.pare_bit_RS485++;
                    if (edition_settings.pare_bit_RS485 > VALUE_PARE_INTERFACE_MAX)
                      edition_settings.pare_bit_RS485 = VALUE_PARE_INTERFACE_MIN;
                  }
                  //Формуємо екран інформації по контролю парності
                  make_ekran_pare_interface();
                }
                else if (current_ekran.current_level == EKRAN_VIEW_STOP_BITS_RS485)
                {
                  if (current_ekran.edition == 0)
                  {
                    if (++current_ekran.index_position >= MAX_ROW_FOR_VIEW_STOP_BITS_INTERFACE)
                      current_ekran.index_position = 0;
                    position_in_current_level_menu[EKRAN_VIEW_STOP_BITS_RS485] = current_ekran.index_position;
                  }
                  else
                  {
                    //Редагування числа
                    edition_settings.number_stop_bit_RS485++;
                    if (edition_settings.number_stop_bit_RS485 > VALUE_STOP_BITS_INTERFACE_MAX)
                      edition_settings.number_stop_bit_RS485 = VALUE_STOP_BITS_INTERFACE_MIN;
                  }
                  //Формуємо екран інформації по кількості стопових біт
                  make_ekran_stopbits_interface();
                }
                else if (current_ekran.current_level == EKRAN_VIEW_TIMEOUT_RS485)
                {
                  if (current_ekran.edition == 0)
                  {
                    if (++current_ekran.index_position >= MAX_ROW_FOR_VIEW_TIMEOUT_INTERFACE)
                      current_ekran.index_position = 0;
                    position_in_current_level_menu[EKRAN_VIEW_TIMEOUT_RS485] = current_ekran.index_position;
                  }
                  else
                  {
                    //Редагування числа
                    edition_settings.time_out_1_RS485 = edit_setpoint(0, edition_settings.time_out_1_RS485, 1, COL_TIMEOUT_INTERFACE_COMMA, COL_TIMEOUT_INTERFACE_END, 1);
                  }
                  //Формуємо екран інформації по time-out наступного символу
                  make_ekran_timeout_interface();
                }
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                else if (current_ekran.current_level == EKRAN_SETTING_NETWORK_LAYER_ETHERNET)
                {
                  if (current_ekran.edition == 0)
                  {
                    if (++current_ekran.index_position >= MAX_ROW_FOR_SETTING_NETWORK_LAYER_ETHERNET)
                      current_ekran.index_position = 0;
                    position_in_current_level_menu[EKRAN_SETTING_NETWORK_LAYER_ETHERNET] = current_ekran.index_position;
                  }
                  else
                  {
                    //Редагування числа
                    if (
                      (current_ekran.index_position == INDEX_ML_NL_IPV4) ||
                      (current_ekran.index_position == INDEX_ML_NL_GATEWAY))
                    {
                      uint16_t *point_target;
                      if (current_ekran.index_position == INDEX_ML_NL_IPV4)
                      {
                        point_target = edition_settings.IP4;
                      }
                      else
                      {
                        point_target = edition_settings.gateway;
                      }

                      size_t index = (current_ekran.position_cursor_x - COL_IP4_GATEWAY_BEGIN) >> 2; /*ділення на 4, бо ХХХ. - це чотири цифри*/
                      point_target[index] = edit_setpoint(0, point_target[index], 0, 0, COL_IP4_GATEWAY_BEGIN + (3 + 1) * (index + 1) - 1 - 1, 1);
                    }
                    else if (current_ekran.index_position == INDEX_ML_NL_MASK)
                    {
                      edition_settings.mask = edit_setpoint(0, edition_settings.mask, 0, 0, COL_MASK_END, 1);
                    }
                  }
                  //Формуємо екран інфтрмації по налаштуваннях мережевого рівня Ethernet
                  make_ekran_settings_network_layer_Ethernet();
                }
                else if (current_ekran.current_level == EKRAN_SYNCHRO)
                {
                  if (current_ekran.edition == 0)
                  {
                    if (++current_ekran.index_position >= MAX_ROW_FOR_SYNCHRO)
                      current_ekran.index_position = 0;
                    position_in_current_level_menu[EKRAN_SYNCHRO] = current_ekran.index_position;
                  }
                  else
                  {
                    //Редагування числа
                    if (current_ekran.index_position == INDEX_ML_SYN_IPV4)
                    {
                      size_t index = (current_ekran.position_cursor_x - COL_IP4_SERVER_BEGIN) >> 2; /*ділення на 4, бо ХХХ. - це чотири цифри*/
                      edition_settings.IP_time_server[index] = edit_setpoint(0, edition_settings.IP_time_server[index], 0, 0, COL_IP4_SERVER_BEGIN + (3 + 1) * (index + 1) - 1 - 1, 1);
                    }
                    else if (current_ekran.index_position == INDEX_ML_SYN_PORT)
                    {
                      edition_settings.port_time_server = edit_setpoint(0, edition_settings.port_time_server, 0, 0, COL_PORT_END, 1);
                    }
                    else if (current_ekran.index_position == INDEX_ML_SYN_PERIOD)
                    {
                      edition_settings.period_sync = edit_setpoint(0, edition_settings.period_sync, 0, 0, COL_PEDIOD_END, 1);
                    }
                  }
                  //Формуємо екран інфтрмації
                  make_ekran_settings_synchro();
                }
#endif
                else if (current_ekran.current_level == EKRAN_TIME_ZONE)
                {
                  if (++current_ekran.index_position >= MAX_ROW_FOR_TIMEZONE_SETTINGS)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[EKRAN_TIME_ZONE] = current_ekran.index_position;
                  //Формуємо екран
                  make_ekran_timezone_dst();
                }
                else if (current_ekran.current_level == EKRAN_DST_RULE)
                {
                  if (++current_ekran.index_position >= MAX_ROW_FOR_DST_RULE)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[EKRAN_DST_RULE] = current_ekran.index_position;

                  __SETTINGS *p_settings = (current_ekran.edition == 0) ? &current_settings : &edition_settings;
                  uint32_t rule = (position_in_current_level_menu[previous_level_in_current_level_menu[EKRAN_DST_RULE]] == INDEX_ML_CHDT_DST_ON) ? p_settings->dst_on_rule : p_settings->dst_off_rule;
                  //Формуємо екран
                  make_ekran_dst_rule(rule);
                }
                else if (current_ekran.current_level == EKRAN_GENERAL_PICKUPS_EL)
                {
                  if (current_ekran.edition == 0)
                  {
                    if (++current_ekran.index_position >= MAX_ROW_FOR_GENERAL_PICKUPS_EL)
                      current_ekran.index_position = 0;
                    position_in_current_level_menu[EKRAN_GENERAL_PICKUPS_EL] = current_ekran.index_position;
                  }
                  else
                  {
                    //Редагування числа
                    if (current_ekran.index_position == INDEX_ML_NUMBER_INERATION)
                    {
                      edition_settings.number_iteration_el = edit_setpoint(0, edition_settings.number_iteration_el, 0, 0, COL_NUMBER_INERATION_END, 1);
                    }
                    //                  else if (current_ekran.index_position == INDEX_ML_NUMBER_DEFINED_FUNCTIONS)
                    //                  {
                    //                    edition_settings.number_defined_df = edit_setpoint(0, edition_settings.number_defined_df, 0, 0, COL_NUMBER_DEFINED_FUNCTIONS_END, 1);
                    //                  }
                    //                  else if (current_ekran.index_position == INDEX_ML_NUMBER_DEFINED_TRIGGERS)
                    //                  {
                    //                    edition_settings.number_defined_dt = edit_setpoint(0, edition_settings.number_defined_dt, 0, 0, COL_NUMBER_DEFINED_TRIGGERS_END, 1);
                    //                  }
                    //                  else if (current_ekran.index_position == INDEX_ML_NUMBER_DEFINED_AND)
                    //                  {
                    //                    edition_settings.number_defined_and = edit_setpoint(0, edition_settings.number_defined_and, 0, 0, COL_NUMBER_DEFINED_AND_END, 1);
                    //                  }
                    //                  else if (current_ekran.index_position == INDEX_ML_NUMBER_DEFINED_OR)
                    //                  {
                    //                    edition_settings.number_defined_or = edit_setpoint(0, edition_settings.number_defined_or, 0, 0, COL_NUMBER_DEFINED_OR_END, 1);
                    //                  }
                    //                  else if (current_ekran.index_position == INDEX_ML_NUMBER_DEFINED_XOR)
                    //                  {
                    //                    edition_settings.number_defined_xor = edit_setpoint(0, edition_settings.number_defined_xor, 0, 0, COL_NUMBER_DEFINED_XOR_END, 1);
                    //                  }
                    //                  else if (current_ekran.index_position == INDEX_ML_NUMBER_DEFINED_NOT)
                    //                  {
                    //                    edition_settings.number_defined_not = edit_setpoint(0, edition_settings.number_defined_not, 0, 0, COL_NUMBER_DEFINED_NOT_END, 1);
                    //                  }
                  }
                  //Формуємо екран відображення загальних витримок для розширеної логіки
                  make_ekran_general_pickups_el();
                }
                else if (current_ekran.current_level == EKRAN_LIST_TYPE_DF)
                {
                  if (++current_ekran.index_position >= MAX_ROW_FOR_TYPE_DF)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[EKRAN_LIST_TYPE_DF] = current_ekran.index_position;
                  //Формуємо екран відображення типу опреділювальної функції
                  make_ekran_type_df();
                }
                else if ((current_ekran.current_level >= EKRAN_TIMEOUT_DF1) && (current_ekran.current_level <= (EKRAN_TIMEOUT_DF1 + NUMBER_DEFINED_FUNCTIONS - 1)))
                {
                  if (current_ekran.edition == 0)
                  {
                    if (++current_ekran.index_position >= MAX_ROW_TIMEOUT_DF)
                      current_ekran.index_position = 0;
                    position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;
                  }
                  else
                  {
                    //Редагування числа
                    if (current_ekran.index_position == INDEX_ML_TMO_DF_PAUSE)
                      edition_settings.timeout_pause_df[current_ekran.current_level - EKRAN_TIMEOUT_DF1] =
                        edit_setpoint(0, edition_settings.timeout_pause_df[current_ekran.current_level - EKRAN_TIMEOUT_DF1], 1, COL_TMO_DF_PAUSE_COMMA, COL_TMO_DF_PAUSE_END, 10);
                    else
                      edition_settings.timeout_work_df[current_ekran.current_level - EKRAN_TIMEOUT_DF1] =
                        edit_setpoint(0, edition_settings.timeout_work_df[current_ekran.current_level - EKRAN_TIMEOUT_DF1], 1, COL_TMO_DF_WORK_COMMA, COL_TMO_DF_WORK_END, 10);
                  }
                  //Формуємо екран таймерів опреділюваних функцій
                  make_ekran_timeout_df(current_ekran.current_level - EKRAN_TIMEOUT_DF1);
                }
                else if ((current_ekran.current_level >= EKRAN_LIST_SOURCE_TF1) && (current_ekran.current_level <= (EKRAN_LIST_SOURCE_TF1 + NUMBER_TRANSFER_FUNCTIONS - 1)))
                {
                  if (++current_ekran.index_position >= MAX_ROW_LIST_SOURCE_TF)
                    current_ekran.index_position = 0;
                  //Формуємо екран
                  make_ekran_list_source_tf();
                }
                else if (current_ekran.current_level == EKRAN_TIMEOUT_DIGITAL_REGISTRATOR)
                {
                  if (current_ekran.edition == 0)
                  {
                    if (++current_ekran.index_position >= MAX_ROW_FOR_TIMEOUT_DIGITAL_REGISTRATOR)
                      current_ekran.index_position = 0;
                    position_in_current_level_menu[EKRAN_TIMEOUT_DIGITAL_REGISTRATOR] = current_ekran.index_position;
                  }
                  else
                  {
                    //Редагування числа
                    edition_settings.timeout_prolongation_work_digital_registrator =
                      edit_setpoint(0, edition_settings.timeout_prolongation_work_digital_registrator, 1, COL_TMO_ELONGATION_COMMA, COL_TMO_ELONGATION_END, 10);
                  }
                  //Формуємо екран витримок аналогового реєстратора
                  make_ekran_timeout_digital_registrator();
                }
                else if (current_ekran.current_level == EKRAN_TIMEOUT_ANALOG_REGISTRATOR)
                {
                  if (current_ekran.edition == 0)
                  {
                    if (++current_ekran.index_position >= MAX_ROW_FOR_TIMEOUT_ANALOG_REGISTRATOR)
                      current_ekran.index_position = 0;
                    position_in_current_level_menu[EKRAN_TIMEOUT_ANALOG_REGISTRATOR] = current_ekran.index_position;
                  }
                  else
                  {
                    //Редагування числа
                    if (current_ekran.index_position == INDEX_ML_TMOPREFAULT)
                    {
                      unsigned int temp_setpoint = edition_settings.prefault_number_periods * 20;
                      do
                      {
                        //Величину витримки зменшуємо почергого на 10 мс (ф-ція edit_setpoint збільшує/зменшує з крогом кратних 1, 10 і т.д.), щоб отримати крок 20 мс
                        temp_setpoint = edit_setpoint(0, temp_setpoint, 1, COL_TMO_PREFAULT_COMMA, COL_TMO_PREFAULT_END, 10);
                      } while ((temp_setpoint % 20) != 0);
                      edition_settings.prefault_number_periods = temp_setpoint / 20;
                    }
                    else
                    {
                      unsigned int temp_setpoint = edition_settings.postfault_number_periods * 20;
                      do
                      {
                        //Величину витримки зменшуємо почергого на 10 мс (ф-ція edit_setpoint збільшує/зменшує з крогом кратних 1, 10 і т.д.), щоб отримати крок 20 мс
                        temp_setpoint = edit_setpoint(0, temp_setpoint, 1, COL_TMO_PREFAULT_COMMA, COL_TMO_PREFAULT_END, 10);
                      } while ((temp_setpoint % 20) != 0);
                      edition_settings.postfault_number_periods = temp_setpoint / 20;
                    }
                  }
                  //Формуємо екран витримок аналогового реєстратора
                  make_ekran_timeout_analog_registrator();
                }
                else if (current_ekran.current_level == EKRAN_CONTROL_AR)
                {
                  if (++current_ekran.index_position >= MAX_ROW_FOR_CONTROL_AR)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[EKRAN_CONTROL_AR] = current_ekran.index_position;
                  //Формуємо екран відображення
                  make_ekran_control_ar();
                }
                else if (current_ekran.current_level == EKRAN_VIEW_SETTING_LANGUAGE)
                {
                  if (current_ekran.edition == 0)
                  {
                    if (++current_ekran.index_position >= MAX_ROW_FOR_VIEW_SETTING_LANGUAGE)
                      current_ekran.index_position = 0;
                    position_in_current_level_menu[EKRAN_VIEW_SETTING_LANGUAGE] = current_ekran.index_position;
                  }
                  else
                  {
                    //Редагування числа
                    edition_settings.language++;
                    if (edition_settings.language > VALUE_SETTING_LANGUAGE_MAX)
                      edition_settings.language = VALUE_SETTING_LANGUAGE_MIN;
                  }
                  //Формуємо екран інформації по мові меню
                  make_ekran_setting_language();
                }
                else if (current_ekran.current_level == EKRAN_CHOSE_EXTRA_SETTINGS)
                {
                  if (++current_ekran.index_position >= MAX_ROW_FOR_CHOSE_EXTRA_SETTINGS)
                    current_ekran.index_position = 0;
                  position_in_current_level_menu[EKRAN_CHOSE_EXTRA_SETTINGS] = current_ekran.index_position;
                  //Формуємо екран відображення додаткових налаштувань
                  make_ekran_chose_extra_settings();
                }
                else if (current_ekran.current_level == EKRAN_VIEW_GRUPA_USTAVOK)
                {
                  if (current_ekran.edition == 0)
                  {
                    if (++current_ekran.index_position >= MAX_ROW_FOR_VIEW_GRUPA_USTAVOK)
                      current_ekran.index_position = 0;
                    position_in_current_level_menu[EKRAN_VIEW_GRUPA_USTAVOK] = current_ekran.index_position;
                  }
                  else
                  {
                    //Редагування числа
                    edition_settings.grupa_ustavok++;
                    if (edition_settings.grupa_ustavok > SETPOINT_GRUPA_USTAVOK_MAX)
                      edition_settings.grupa_ustavok = SETPOINT_GRUPA_USTAVOK_MIN;
                  }
                  //Формуємо екран інформації по групах уставок
                  make_ekran_grupa_ustavok();
                }

                //Очистити сигналізацію, що натиснута кнопка
                new_state_keyboard &= ~(1u << BIT_KEY_DOWN);
              }
              else if (new_state_keyboard == (1u << BIT_KEY_RIGHT))
              {
                current_ekran.position_cursor_x++;
                //Перевіряємо, чи ми не вийшли за межі виділені для значення даної уставки, або позицію коми
                if (
                  (current_ekran.current_level >= EKRAN_SETPOINT_RPN_GROUP1) &&
                  (current_ekran.current_level <= EKRAN_SETPOINT_RPN_GROUP4))
                {
                  if (current_ekran.index_position == INDEX_ML_STPRPN_OSN)
                  {
                    if (current_ekran.position_cursor_x == COL_SETPOINT_RPN_OSN_COMMA)
                      current_ekran.position_cursor_x++;
                    if ((current_ekran.position_cursor_x < COL_SETPOINT_RPN_OSN_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_SETPOINT_RPN_OSN_END))
                      current_ekran.position_cursor_x = COL_SETPOINT_RPN_OSN_BEGIN;
                  }
                  else if (current_ekran.index_position == INDEX_ML_STPRPN_ZONE)
                  {
                    if (current_ekran.position_cursor_x == COL_SETPOINT_RPN_ZONE_COMMA)
                      current_ekran.position_cursor_x++;
                    if ((current_ekran.position_cursor_x < COL_SETPOINT_RPN_ZONE_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_SETPOINT_RPN_ZONE_END))
                      current_ekran.position_cursor_x = COL_SETPOINT_RPN_ZONE_BEGIN;
                  }
                  else if (current_ekran.index_position == INDEX_ML_STPRPN_MAX_PER)
                  {
                    if ((current_ekran.position_cursor_x < COL_SETPOINT_RPN_MAX_PER_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_SETPOINT_RPN_MAX_PER_END))
                      current_ekran.position_cursor_x = COL_SETPOINT_RPN_MAX_PER_BEGIN;
                  }
                  else if (current_ekran.index_position == INDEX_ML_STPRPN_DOD)
                  {
                    if (current_ekran.position_cursor_x == COL_SETPOINT_RPN_DOD_COMMA)
                      current_ekran.position_cursor_x++;
                    if ((current_ekran.position_cursor_x < COL_SETPOINT_RPN_DOD_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_SETPOINT_RPN_DOD_END))
                      current_ekran.position_cursor_x = COL_SETPOINT_RPN_DOD_BEGIN;
                  }
                  else if (current_ekran.index_position == INDEX_ML_STPRPN_K)
                  {
                    if (current_ekran.position_cursor_x == COL_SETPOINT_RPN_K_COMMA)
                      current_ekran.position_cursor_x++;
                    if ((current_ekran.position_cursor_x < COL_SETPOINT_RPN_K_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_SETPOINT_RPN_K_END))
                      current_ekran.position_cursor_x = COL_SETPOINT_RPN_K_BEGIN;
                  }

                  //Формуємо екран уставок РПН
                  int group = (current_ekran.current_level - EKRAN_SETPOINT_RPN_GROUP1);
                  make_ekran_setpoint_rpn(group);
                }
                else if (
                  (current_ekran.current_level >= EKRAN_TIMEOUT_RPN_GROUP1) &&
                  (current_ekran.current_level <= EKRAN_TIMEOUT_RPN_GROUP4))
                {
                  if (current_ekran.index_position == INDEX_ML_TMORPN_UB_PRYB)
                  {
                    if ((current_ekran.position_cursor_x < COL_TMO_RPN_UB_PRYB_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_TMO_RPN_UB_PRYB_END))
                      current_ekran.position_cursor_x = COL_TMO_RPN_UB_PRYB_BEGIN;
                  }
                  else if (current_ekran.index_position == INDEX_ML_TMORPN_UB_PRYB_PRYSK)
                  {
                    if ((current_ekran.position_cursor_x < COL_TMO_RPN_UB_PRYB_PRYSK_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_TMO_RPN_UB_PRYB_PRYSK_END))
                      current_ekran.position_cursor_x = COL_TMO_RPN_UB_PRYB_PRYSK_BEGIN;
                  }
                  else if (current_ekran.index_position == INDEX_ML_TMORPN_PRYSK_VID_UMAX)
                  {
                    if (current_ekran.position_cursor_x == COL_TMO_RPN_PRYSK_VID_UMAX_COMMA)
                      current_ekran.position_cursor_x++;
                    if ((current_ekran.position_cursor_x < COL_TMO_RPN_PRYSK_VID_UMAX_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_TMO_RPN_PRYSK_VID_UMAX_END))
                      current_ekran.position_cursor_x = COL_TMO_RPN_PRYSK_VID_UMAX_BEGIN;
                  }
                  else if (current_ekran.index_position == INDEX_ML_TMORPN_PEREKL)
                  {
                    if (current_ekran.position_cursor_x == COL_TMO_RPN_PEREKL_COMMA)
                      current_ekran.position_cursor_x++;
                    if ((current_ekran.position_cursor_x < COL_TMO_RPN_PEREKL_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_TMO_RPN_PEREKL_END))
                      current_ekran.position_cursor_x = COL_TMO_RPN_PEREKL_BEGIN;
                  }
                  else if (current_ekran.index_position == INDEX_ML_TMORPN_NESPR)
                  {
                    if (current_ekran.position_cursor_x == COL_TMO_RPN_NESPR_COMMA)
                      current_ekran.position_cursor_x++;
                    if ((current_ekran.position_cursor_x < COL_TMO_RPN_NESPR_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_TMO_RPN_NESPR_END))
                      current_ekran.position_cursor_x = COL_TMO_RPN_NESPR_BEGIN;
                  }

                  //Формуємо екран витримок РПН
                  int group = (current_ekran.current_level - EKRAN_TIMEOUT_RPN_GROUP1);
                  make_ekran_timeout_rpn(group);
                }
                else if (current_ekran.current_level == EKRAN_CONTROL_RPN)
                {
                  if (current_ekran.index_position < MAX_ROW_FOR_CONTROL_RPN)
                  {
                    //Міняємо на протилежний відповідний біт для вибраної позиції
                    edition_settings.control_rpn ^= (1u << current_ekran.index_position);
                  }

                  //Формуємо екран управлінської інформації для РПН
                  make_ekran_control_rpn();
                }
                else if (current_ekran.current_level == EKRAN_CONTROL_SZKh)
                {
                  if (current_ekran.index_position < MAX_ROW_FOR_CONTROL_SZKh)
                  {
                    //Міняємо на протилежний відповідний біт для вибраної позиції
                    edition_settings.control_zskh ^= (1u << current_ekran.index_position);
                  }

                  //Формуємо екран управлінської інформації для ЗСХ
                  make_ekran_control_zskh();
                }
                else if (
                  (current_ekran.current_level >= EKRAN_SETPOINT_BRP_GROUP1) &&
                  (current_ekran.current_level <= EKRAN_SETPOINT_BRP_GROUP4))
                {
                  if (current_ekran.index_position == INDEX_ML_STPBRP_I_BLK)
                  {
                    if (current_ekran.position_cursor_x == COL_SETPOINT_BRP_I_BLK_COMMA)
                      current_ekran.position_cursor_x++;
                    if ((current_ekran.position_cursor_x < COL_SETPOINT_BRP_I_BLK_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_SETPOINT_BRP_I_BLK_END))
                      current_ekran.position_cursor_x = COL_SETPOINT_BRP_I_BLK_BEGIN;
                  }

                  //Формуємо екран уставок БРП
                  int group = (current_ekran.current_level - EKRAN_SETPOINT_BRP_GROUP1);
                  make_ekran_setpoint_brp(group);
                }
                else if (current_ekran.current_level == EKRAN_CONTROL_BRP)
                {

                  //Міняємо на протилежний відповідний біт для вибраної позиції
                  edition_settings.control_brp ^= MASKA_FOR_BIT(current_ekran.index_position);

                  //Формуємо екран управлінської інформації для БРП
                  make_ekran_control_brp();
                }
                else if (
                  (current_ekran.current_level >= EKRAN_TIMEOUT_ZNKh_GROUP1) &&
                  (current_ekran.current_level <= EKRAN_TIMEOUT_ZNKh_GROUP4))
                {
                  if (current_ekran.index_position == INDEX_ML_TMOZNKh_PEREKL)
                  {
                    if (current_ekran.position_cursor_x == COL_TMO_ZNKh_PEREKL_COMMA)
                      current_ekran.position_cursor_x++;
                    if ((current_ekran.position_cursor_x < COL_TMO_ZNKh_PEREKL_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_TMO_ZNKh_PEREKL_END))
                      current_ekran.position_cursor_x = COL_TMO_ZNKh_PEREKL_BEGIN;
                  }
                  //Формуємо екран витримок ЗНХ
                  int group = (current_ekran.current_level - EKRAN_TIMEOUT_ZNKh_GROUP1);
                  make_ekran_timeout_znkh(group);
                }
                else if (current_ekran.current_level == EKRAN_CONTROL_ZNKh)
                {
                  if (current_ekran.index_position < MAX_ROW_FOR_CONTROL_ZNKh)
                  {
                    //Міняємо на протилежний відповідний біт для вибраної позиції
                    edition_settings.control_znkh ^= (1u << current_ekran.index_position);
                  }

                  //Формуємо екран управлінської інформації для ЗНХ
                  make_ekran_control_znkh();
                }
                else if (
                  (current_ekran.current_level >= EKRAN_SETPOINT_UMIN_GROUP1) &&
                  (current_ekran.current_level <= EKRAN_SETPOINT_UMIN_GROUP4))
                {
                  if (current_ekran.index_position == INDEX_ML_STPUmin_Umin1)
                  {
                    if ((current_ekran.position_cursor_x < COL_SETPOINT_Umin_Umin1_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_SETPOINT_Umin_Umin1_END))
                      current_ekran.position_cursor_x = COL_SETPOINT_Umin_Umin1_BEGIN;
                  }
                  else if (current_ekran.index_position == INDEX_ML_STPUmin_Umin2)
                  {
                    if ((current_ekran.position_cursor_x < COL_SETPOINT_Umin_Umin2_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_SETPOINT_Umin_Umin2_END))
                      current_ekran.position_cursor_x = COL_SETPOINT_Umin_Umin2_BEGIN;
                  }

                  //Формуємо екран уставок Umin
                  int group = (current_ekran.current_level - EKRAN_SETPOINT_UMIN_GROUP1);
                  make_ekran_setpoint_Umin(group);
                }
                else if (
                  (current_ekran.current_level >= EKRAN_TIMEOUT_UMIN_GROUP1) &&
                  (current_ekran.current_level <= EKRAN_TIMEOUT_UMIN_GROUP4))
                {
                  if (current_ekran.index_position == INDEX_ML_TMOUmin_Umin1)
                  {
                    if (current_ekran.position_cursor_x == COL_TMO_Umin_Umin1_COMMA)
                      current_ekran.position_cursor_x++;
                    if ((current_ekran.position_cursor_x < COL_TMO_Umin_Umin1_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_TMO_Umin_Umin1_END))
                      current_ekran.position_cursor_x = COL_TMO_Umin_Umin1_BEGIN;
                  }
                  else if (current_ekran.index_position == INDEX_ML_TMOUmin_Umin2)
                  {
                    if (current_ekran.position_cursor_x == COL_TMO_Umin_Umin2_COMMA)
                      current_ekran.position_cursor_x++;
                    if ((current_ekran.position_cursor_x < COL_TMO_Umin_Umin2_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_TMO_Umin_Umin2_END))
                      current_ekran.position_cursor_x = COL_TMO_Umin_Umin2_BEGIN;
                  }

                  //Формуємо екран витримок Umin
                  int group = (current_ekran.current_level - EKRAN_TIMEOUT_UMIN_GROUP1);
                  make_ekran_timeout_Umin(group);
                }
                else if (current_ekran.current_level == EKRAN_CONTROL_UMIN)
                {
                  unsigned int maska = 0;

                  //Виділяємо, який біт треба міняти
                  if (current_ekran.index_position == INDEX_ML_CTRUmin_STAGE_1)
                    maska = MASKA_FOR_BIT(INDEX_ML_CTRUmin_STAGE_1);
                  else if (current_ekran.index_position == INDEX_ML_CTRUmin_STAGE_2)
                    maska = MASKA_FOR_BIT(INDEX_ML_CTRUmin_STAGE_2);

                  //Міняємо на протилежний відповідний біт для вибраної позиції
                  edition_settings.control_Umin ^= maska;

                  //Формуємо екран управлінської інформації для Umin
                  make_ekran_control_Umin();
                }
                else if (
                  (current_ekran.current_level >= EKRAN_SETPOINT_UMAX_GROUP1) &&
                  (current_ekran.current_level <= EKRAN_SETPOINT_UMAX_GROUP4))
                {
                  if (current_ekran.index_position == INDEX_ML_STPUmax_Umax1)
                  {
                    if ((current_ekran.position_cursor_x < COL_SETPOINT_Umax_Umax1_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_SETPOINT_Umax_Umax1_END))
                      current_ekran.position_cursor_x = COL_SETPOINT_Umax_Umax1_BEGIN;
                  }
                  else if (current_ekran.index_position == INDEX_ML_STPUmax_Umax2)
                  {
                    if ((current_ekran.position_cursor_x < COL_SETPOINT_Umax_Umax2_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_SETPOINT_Umax_Umax2_END))
                      current_ekran.position_cursor_x = COL_SETPOINT_Umax_Umax2_BEGIN;
                  }

                  //Формуємо екран уставок Umax
                  int group = (current_ekran.current_level - EKRAN_SETPOINT_UMAX_GROUP1);
                  make_ekran_setpoint_Umax(group);
                }
                else if (
                  (current_ekran.current_level >= EKRAN_TIMEOUT_UMAX_GROUP1) &&
                  (current_ekran.current_level <= EKRAN_TIMEOUT_UMAX_GROUP4))
                {
                  if (current_ekran.index_position == INDEX_ML_TMOUmax_Umax2)
                  {
                    if (current_ekran.position_cursor_x == COL_TMO_Umax_Umax2_COMMA)
                      current_ekran.position_cursor_x++;
                    if ((current_ekran.position_cursor_x < COL_TMO_Umax_Umax2_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_TMO_Umax_Umax2_END))
                      current_ekran.position_cursor_x = COL_TMO_Umax_Umax2_BEGIN;
                  }

                  //Формуємо екран витримок Umax
                  int group = (current_ekran.current_level - EKRAN_TIMEOUT_UMAX_GROUP1);
                  make_ekran_timeout_Umax(group);
                }
                else if (current_ekran.current_level == EKRAN_CONTROL_UMAX)
                {
                  unsigned int maska = 0;

                  //Виділяємо, який біт треба міняти
                  if (current_ekran.index_position == INDEX_ML_CTRUmax_STAGE_1)
                    maska = MASKA_FOR_BIT(INDEX_ML_CTRUmax_STAGE_1);
                  else if (current_ekran.index_position == INDEX_ML_CTRUmax_STAGE_2)
                    maska = MASKA_FOR_BIT(INDEX_ML_CTRUmax_STAGE_2);

                  //Міняємо на протилежний відповідний біт для вибраної позиції
                  edition_settings.control_Umax ^= maska;

                  //Формуємо екран управлінської інформації для Umax
                  make_ekran_control_Umax();
                }
                else if (
                  (current_ekran.current_level >= EKRAN_SETPOINT_UP_GROUP1) &&
                  (current_ekran.current_level <= EKRAN_SETPOINT_UP_GROUP4))
                {
                  uint32_t _n_index = current_ekran.index_position % MAX_ROW_FOR_SETPOINT_UP;

                  if (_n_index == INDEX_ML_STP_UP)
                  {
                    uint32_t _n_UP = current_ekran.index_position / MAX_ROW_FOR_SETPOINT_UP;
                    int32_t begin = 0, comma = 0, end = 0;
                    switch (current_settings.ctrl_UP_input[_n_UP])
                    {
                      case UP_CTRL_Ia_Ib_Ic:
                      case UP_CTRL_Ia:
                      case UP_CTRL_Ib:
                      case UP_CTRL_Ic:
                      case UP_CTRL_I1:
                      case UP_CTRL_I2:
                      case UP_CTRL_I04:
                      case UP_CTRL_3I0_r:
                        {
                          begin = COL_SETPOINT_UP_I_BEGIN;
                          comma = COL_SETPOINT_UP_I_COMMA;
                          end = COL_SETPOINT_UP_I_END;
                          break;
                        }
                      case UP_CTRL_3I0:
                      case UP_CTRL_3I0_others:
                        {
                          begin = COL_SETPOINT_UP_3I0_BEGIN;
                          comma = COL_SETPOINT_UP_3I0_COMMA;
                          end = COL_SETPOINT_UP_3I0_END;
                          break;
                        }
                      case UP_CTRL_Ua_Ub_Uc:
                      case UP_CTRL_Uab_Ubc_Uca:
                      case UP_CTRL_Ua:
                      case UP_CTRL_Uab:
                      case UP_CTRL_Ub:
                      case UP_CTRL_Ubc:
                      case UP_CTRL_Uc:
                      case UP_CTRL_Uca:
                      case UP_CTRL_U1:
                      case UP_CTRL_U2:
                      case UP_CTRL_3U0:
                        {
                          begin = COL_SETPOINT_UP_U_BEGIN;
                          comma = COL_SETPOINT_UP_U_COMMA;
                          end = COL_SETPOINT_UP_U_END;
                          break;
                        }
                      case UP_CTRL_P:
                      case UP_CTRL_Q:
                        {
                          begin = COL_SETPOINT_UP_PQ_BEGIN - 1;
                          comma = COL_SETPOINT_UP_PQ_COMMA;
                          end = COL_SETPOINT_UP_PQ_END;
                          break;
                        }
                      case UP_CTRL_S:
                        {
                          begin = COL_SETPOINT_UP_S_BEGIN;
                          comma = COL_SETPOINT_UP_S_COMMA;
                          end = COL_SETPOINT_UP_S_END;
                          break;
                        }
                      default:
                        {
                          //Теоретично цього ніколи не мало б бути
                          total_error_sw_fixed();
                        }
                    }

                    if (current_ekran.position_cursor_x == comma)
                      current_ekran.position_cursor_x++;
                    if ((current_ekran.position_cursor_x < begin) ||
                        (current_ekran.position_cursor_x > end))
                      current_ekran.position_cursor_x = begin;
                  }
                  else if (_n_index == INDEX_ML_STP_UP_KP)
                  {
                    if (current_ekran.position_cursor_x == COL_SETPOINT_UP_KP_COMMA)
                      current_ekran.position_cursor_x++;
                    if ((current_ekran.position_cursor_x < COL_SETPOINT_UP_KP_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_SETPOINT_UP_KP_END))
                      current_ekran.position_cursor_x = COL_SETPOINT_UP_KP_BEGIN;
                  }

                  //Формуємо екран уставок УЗ
                  int group = (current_ekran.current_level - EKRAN_SETPOINT_UP_GROUP1);
                  make_ekran_setpoint_UP(group);
                }
                else if (
                  (current_ekran.current_level >= EKRAN_TIMEOUT_UP_GROUP1) &&
                  (current_ekran.current_level <= EKRAN_TIMEOUT_UP_GROUP4))
                {
                  uint32_t _n_index = current_ekran.index_position % MAX_ROW_FOR_TIMEOUT_UP;

                  if (_n_index == INDEX_ML_TMOUP)
                  {
                    if (current_ekran.position_cursor_x == COL_TMO_UP_COMMA)
                      current_ekran.position_cursor_x++;
                    if ((current_ekran.position_cursor_x < COL_TMO_UP_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_TMO_UP_END))
                      current_ekran.position_cursor_x = COL_TMO_UP_BEGIN;
                  }

                  //Формуємо екран витримок УЗ
                  int group = (current_ekran.current_level - EKRAN_TIMEOUT_UP_GROUP1);
                  make_ekran_timeout_UP(group);
                }
                else if (current_ekran.current_level == EKRAN_CONTROL_UP)
                {
                  uint32_t n_UP = current_ekran.index_position / (_CTR_UP_NEXT_BIT - _CTR_UP_PART_I);
                  uint32_t n_index = current_ekran.index_position % (_CTR_UP_NEXT_BIT - _CTR_UP_PART_I);
                  if (n_index == CTR_UP_CTRL_INPUT)
                  {
                    uint32_t ctrl_maska = MASKA_FOR_BIT(n_UP * (_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_STATE_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I)) |
                                          MASKA_FOR_BIT(n_UP * (_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_OR_AND_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I)) |
                                          MASKA_FOR_BIT(n_UP * (_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_MORE_LESS_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I));
                    edition_settings.control_UP &= (uint32_t)(~ctrl_maska);

                    do
                    {
                      if (++edition_settings.ctrl_UP_input[n_UP] >= _UP_CTRL_NUMBER)
                        edition_settings.ctrl_UP_input[n_UP] = _UP_CTRL_MIN;
                    } while (
                      (edition_settings.ctrl_UP_input[n_UP] == UP_CTRL_Ua_Ub_Uc) ||
                      (edition_settings.ctrl_UP_input[n_UP] == UP_CTRL_Ua) ||
                      (edition_settings.ctrl_UP_input[n_UP] == UP_CTRL_Ub) ||
                      (edition_settings.ctrl_UP_input[n_UP] == UP_CTRL_Uc) ||
                      (edition_settings.ctrl_UP_input[n_UP] == UP_CTRL_U1) ||
                      (edition_settings.ctrl_UP_input[n_UP] == UP_CTRL_U2));
                  }
                  else
                  {
                    unsigned int maska = 0;

                    //Виділяємо, який біт треба міняти
                    if (n_index == CTR_UP_STATE_BIT)
                      maska = MASKA_FOR_BIT(n_UP * (_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_STATE_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I));
                    else if (n_index == CTR_UP_OR_AND_BIT)
                      maska = MASKA_FOR_BIT(n_UP * (_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_OR_AND_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I));
                    else if (n_index == CTR_UP_MORE_LESS_BIT)
                      maska = MASKA_FOR_BIT(n_UP * (_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_MORE_LESS_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I));

                    //Міняємо на протилежний відповідний біт для вибраної позиції
                    edition_settings.control_UP ^= maska;
                  }

                  //Формуємо екран управлінської інформації для УЗ
                  make_ekran_control_UP();
                }
                else if (current_ekran.current_level == EKRAN_TRANSFORMATOR_INFO)
                {
                  if (
                    (current_ekran.index_position == INDEX_ML_TT1) ||
                    (current_ekran.index_position == INDEX_ML_TT2))
                  {
                    if ((current_ekran.position_cursor_x < COL_TT_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_TT_END))
                      current_ekran.position_cursor_x = COL_TT_BEGIN;
                  }
                  else
                  {
                    if ((current_ekran.position_cursor_x < COL_TN_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_TN_END))
                      current_ekran.position_cursor_x = COL_TN_BEGIN;
                  }
                  //Формуємо екран відображення інформації по трансформаторах
                  make_ekran_transformator();
                }
                else if (current_ekran.current_level == EKRAN_SETPOINT_SWITCH)
                {
                  if (current_ekran.index_position == INDEX_ML_STPInom)
                  {
                    if ((current_ekran.position_cursor_x < COL_SETPOINT_Inom_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_SETPOINT_Inom_END))
                      current_ekran.position_cursor_x = COL_SETPOINT_Inom_BEGIN;
                  }
                  else if (current_ekran.index_position == INDEX_ML_STPRKS_Inom)
                  {
                    if ((current_ekran.position_cursor_x < COL_SETPOINT_RKS_Inom_BEGIN) ||
                        (current_ekran.position_cursor_x > (COL_SETPOINT_RKS_Inom_END - 1)))
                      current_ekran.position_cursor_x = COL_SETPOINT_RKS_Inom_BEGIN;
                  }
                  else if (current_ekran.index_position == INDEX_ML_STPMInom_vymk)
                  {
                    if (current_ekran.position_cursor_x == COL_SETPOINT_Inom_vymk_COMMA)
                      current_ekran.position_cursor_x++;
                    if ((current_ekran.position_cursor_x < COL_SETPOINT_Inom_vymk_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_SETPOINT_Inom_vymk_END))
                      current_ekran.position_cursor_x = COL_SETPOINT_Inom_vymk_BEGIN;
                  }
                  else if (current_ekran.index_position == INDEX_ML_STPRKS_Inom_vymk)
                  {
                    if ((current_ekran.position_cursor_x < COL_SETPOINT_RKS_Inom_vymk_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_SETPOINT_RKS_Inom_vymk_END))
                      current_ekran.position_cursor_x = COL_SETPOINT_RKS_Inom_vymk_BEGIN;
                  }
                  else if (current_ekran.index_position == INDEX_ML_STPPOCHATKOVYJ_RESURS)
                  {
                    if ((current_ekran.position_cursor_x < COL_SETPOINT_POCHATKOVYJ_RESURS_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_SETPOINT_POCHATKOVYJ_RESURS_END))
                      current_ekran.position_cursor_x = COL_SETPOINT_POCHATKOVYJ_RESURS_BEGIN;
                  }
                  else if (current_ekran.index_position == INDEX_ML_STPKRYTYCHNYJ_RESURS)
                  {
                    if ((current_ekran.position_cursor_x < COL_SETPOINT_KRYTYCHNYJ_RESURS_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_SETPOINT_KRYTYCHNYJ_RESURS_END))
                      current_ekran.position_cursor_x = COL_SETPOINT_KRYTYCHNYJ_RESURS_BEGIN;
                  }
                  else if (current_ekran.index_position == INDEX_ML_STPPOCHATKOVA_K_VYMK)
                  {
                    if ((current_ekran.position_cursor_x < COL_SETPOINT_POCHATKOVA_K_VYMK_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_SETPOINT_POCHATKOVA_K_VYMK_END))
                      current_ekran.position_cursor_x = COL_SETPOINT_POCHATKOVA_K_VYMK_BEGIN;
                  }
                  //Формуємо екран уставок виключателя
                  make_ekran_setpoint_switch();
                }
                else if (current_ekran.current_level == EKRAN_TIMEOUT_SWITCH)
                {
                  if (current_ekran.index_position == INDEX_ML_TMOON)
                  {
                    if (current_ekran.position_cursor_x == COL_TMO_SWCH_ON_COMMA)
                      current_ekran.position_cursor_x++;
                    if ((current_ekran.position_cursor_x < COL_TMO_SWCH_ON_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_TMO_SWCH_ON_END))
                      current_ekran.position_cursor_x = COL_TMO_SWCH_ON_BEGIN;
                  }
                  else if (current_ekran.index_position == INDEX_ML_TMOOFF)
                  {
                    if (current_ekran.position_cursor_x == COL_TMO_SWCH_OFF_COMMA)
                      current_ekran.position_cursor_x++;
                    if ((current_ekran.position_cursor_x < COL_TMO_SWCH_OFF_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_TMO_SWCH_OFF_END))
                      current_ekran.position_cursor_x = COL_TMO_SWCH_OFF_BEGIN;
                  }
                  else if (current_ekran.index_position == INDEX_ML_TMOUDL_BLK_ON)
                  {
                    if (current_ekran.position_cursor_x == COL_TMO_SWCH_UDL_BLK_ON_COMMA)
                      current_ekran.position_cursor_x++;
                    if ((current_ekran.position_cursor_x < COL_TMO_SWCH_UDL_BLK_ON_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_TMO_SWCH_UDL_BLK_ON_END))
                      current_ekran.position_cursor_x = COL_TMO_SWCH_UDL_BLK_ON_BEGIN;
                  }
                  else if (current_ekran.index_position == INDEX_ML_TMOPRYVODA_VV)
                  {
                    if (current_ekran.position_cursor_x == COL_TMO_PRYVODA_VV_COMMA)
                      current_ekran.position_cursor_x++;
                    if ((current_ekran.position_cursor_x < COL_TMO_PRYVODA_VV_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_TMO_PRYVODA_VV_END))
                      current_ekran.position_cursor_x = COL_TMO_PRYVODA_VV_BEGIN;
                  }
                  //Формуємо екран витримок виключателя
                  make_ekran_timeout_switch();
                }
                else if (current_ekran.current_level == EKRAN_CONTROL_SWITCH)
                {
                  unsigned int maska = 0;

                  //Виділяємо, який біт треба міняти
                  if (current_ekran.index_position == INDEX_ML_CTRPRYVOD_VV)
                    maska = CTR_PRYVOD_VV;
                  else if (current_ekran.index_position == INDEX_ML_CTRRESURS_VV)
                    maska = CTR_RESURS_VV;

                  //Міняємо на протилежний відповідний біт для вибраної позиції
                  edition_settings.control_switch ^= maska;

                  //Формуємо екран управлінської інформації длявимикача
                  make_ekran_control_switch();
                }
                else if (current_ekran.current_level == EKRAN_DOPUSK_DV_UVV)
                {
                  if (
                    (current_ekran.position_cursor_x < COL_DOPUSK_DV_BEGIN) ||
                    (current_ekran.position_cursor_x > COL_DOPUSK_DV_END) ||
                    ((current_ekran.position_cursor_x == COL_DOPUSK_DV_END) &&
                     ((current_settings.type_of_input_signal & (1 << current_ekran.index_position)) != 0)))
                    current_ekran.position_cursor_x = COL_DOPUSK_DV_BEGIN;

                  //Формуємо екран інфтрмації по допусках ДВ
                  make_ekran_dopusk_dv();
                }
                else if ((current_ekran.current_level == EKRAN_TYPE_INPUT_UVV) || (current_ekran.current_level == EKRAN_TYPE_INPUT_SIGNAL_UVV) ||
                         (current_ekran.current_level == EKRAN_TYPE_LED_UVV))
                {
                  unsigned int value = (1 << current_ekran.index_position);

                  //Міняємо на протилежний відповідний біт для вибраної позиції і формуємо екран управлінської інформації для УВВ
                  if (current_ekran.current_level == EKRAN_TYPE_INPUT_UVV)
                  {
                    edition_settings.type_of_input ^= value;
                    make_ekran_type_input_uvv(0);
                  }
                  else if (current_ekran.current_level == EKRAN_TYPE_INPUT_SIGNAL_UVV)
                  {
                    edition_settings.type_of_input_signal ^= value;
                    make_ekran_type_input_uvv(1);
                  }
                  else
                  {
                    edition_settings.type_of_led ^= value;
                    make_ekran_type_led_uvv();
                  }
                }
                else if (current_ekran.current_level == EKRAN_TYPE_OUTPUT_UVV)
                {
                  unsigned int maska = (1 << current_ekran.index_position);

                  int value = ((edition_settings.type_of_output & maska) != 0);
                  if (value == true)
                    value += ((edition_settings.type_of_output_modif & maska) != 0); //тільки у випадку, коли вихід сигнальний
                  if ((++value) >= 3)
                    value = 0;

                  if (value == 0)
                  {
                    edition_settings.type_of_output &= (unsigned int) (~maska);
                    edition_settings.type_of_output_modif &= (unsigned int) (~maska);
                  }
                  else
                  {
                    edition_settings.type_of_output |= maska;
                    if (value == 1)
                      edition_settings.type_of_output_modif &= (unsigned int) (~maska);
                    else
                      edition_settings.type_of_output_modif |= maska;
                  }

                  make_ekran_type_output_uvv();
                }
                else if (current_ekran.current_level == EKRAN_TYPE_BUTTON_UVV)
                {
                  edition_settings.buttons_mode ^= (1 << current_ekran.index_position);
                  make_ekran_type_button_uvv();
                }
                else if (current_ekran.current_level == EKRAN_ADDRESS_RS485)
                {
                  if ((current_ekran.position_cursor_x < COL_ADDRESS_BEGIN) ||
                      (current_ekran.position_cursor_x > COL_ADDRESS_END))
                    current_ekran.position_cursor_x = COL_ADDRESS_BEGIN;
                  //Формуємо екран інфтрмації по комунікаційній адресі
                  make_ekran_address();
                }
                else if (current_ekran.current_level == EKRAN_VIEW_TIMEOUT_RS485)
                {
                  if (current_ekran.position_cursor_x == COL_TIMEOUT_INTERFACE_COMMA)
                    current_ekran.position_cursor_x++;
                  if ((current_ekran.position_cursor_x < COL_TIMEOUT_INTERFACE_BEGIN) ||
                      (current_ekran.position_cursor_x > COL_TIMEOUT_INTERFACE_END))
                    current_ekran.position_cursor_x = COL_TIMEOUT_INTERFACE_BEGIN;
                  //Формуємо екран інформації по time-out наступного символу
                  make_ekran_timeout_interface();
                }
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                else if (current_ekran.current_level == EKRAN_SETTING_NETWORK_LAYER_ETHERNET)
                {
                  if (
                    (current_ekran.index_position == INDEX_ML_NL_IPV4) ||
                    (current_ekran.index_position == INDEX_ML_NL_GATEWAY))
                  {
                    if ((current_ekran.position_cursor_x < COL_IP4_GATEWAY_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_IP4_GATEWAY_END))
                      current_ekran.position_cursor_x = COL_IP4_GATEWAY_BEGIN;

                    size_t index = (current_ekran.position_cursor_x - COL_IP4_GATEWAY_BEGIN) >> 2; /*ділення на 4, бо ХХХ. - це чотири цифри*/
                    if (current_ekran.position_cursor_x == (COL_IP4_GATEWAY_BEGIN + (3 + 1) * (index + 1) - 1))
                      current_ekran.position_cursor_x++;

                    if ((current_ekran.position_cursor_x < COL_IP4_GATEWAY_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_IP4_GATEWAY_END))
                      current_ekran.position_cursor_x = COL_IP4_GATEWAY_BEGIN;
                  }
                  else if (current_ekran.index_position == INDEX_ML_NL_MASK)
                  {
                    if ((current_ekran.position_cursor_x < COL_MASK_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_MASK_END))
                      current_ekran.position_cursor_x = COL_MASK_BEGIN;
                  }
                  //Формуємо екран інфтрмації по налаштуваннях мережевого рівня Ethernet
                  make_ekran_settings_network_layer_Ethernet();
                }
                else if (current_ekran.current_level == EKRAN_SYNCHRO)
                {
                  if (current_ekran.index_position == INDEX_ML_SYN_IPV4)
                  {
                    if ((current_ekran.position_cursor_x < COL_IP4_SERVER_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_IP4_SERVER_END))
                      current_ekran.position_cursor_x = COL_IP4_SERVER_BEGIN;

                    size_t index = (current_ekran.position_cursor_x - COL_IP4_SERVER_BEGIN) >> 2; /*ділення на 4, бо ХХХ. - це чотири цифри*/
                    if (current_ekran.position_cursor_x == (COL_IP4_SERVER_BEGIN + (3 + 1) * (index + 1) - 1))
                      current_ekran.position_cursor_x++;

                    if ((current_ekran.position_cursor_x < COL_IP4_SERVER_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_IP4_SERVER_END))
                      current_ekran.position_cursor_x = COL_IP4_SERVER_BEGIN;
                  }
                  else if (current_ekran.index_position == INDEX_ML_SYN_PORT)
                  {
                    if ((current_ekran.position_cursor_x < COL_PORT_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_PORT_END))
                      current_ekran.position_cursor_x = COL_PORT_BEGIN;
                  }
                  else if (current_ekran.index_position == INDEX_ML_SYN_PERIOD)
                  {
                    if ((current_ekran.position_cursor_x < COL_PERIOD_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_PEDIOD_END))
                      current_ekran.position_cursor_x = COL_PERIOD_BEGIN;
                  }
                  //Формуємо екран інфтрмації
                  make_ekran_settings_synchro();
                }
#endif
                else if (current_ekran.current_level == EKRAN_TIME_ZONE)
                {
                  if (current_ekran.index_position == INDEX_ML_TIME_ZONE)
                  {
                    if (++edition_settings.time_zone > TIME_ZONE_MAX)
                      edition_settings.time_zone = TIME_ZONE_MIN;
                  }
                  else if (current_ekran.index_position == INDEX_ML_DST)
                  {
                    //Міняємо на протилежний відповідний біт для вибраної позиції
                    edition_settings.dst ^= MASKA_FOR_BIT(N_BIT_TZ_DST);
                  }

                  //Формуємо екран
                  make_ekran_timezone_dst();
                }
                else if (current_ekran.current_level == EKRAN_DST_RULE)
                {
                  uint32_t *p_rule_target = (position_in_current_level_menu[previous_level_in_current_level_menu[EKRAN_DST_RULE]] == INDEX_ML_CHDT_DST_ON) ? &edition_settings.dst_on_rule : &edition_settings.dst_off_rule;
                  unsigned int pos = 0, shift = 0;
                  unsigned int min = 0, max = 0;

                  if (current_ekran.index_position == INDEX_ML_DST_MM)
                  {
                    pos = POS_MM;
                    shift = SHIFT_MM;

                    min = DST_RULE_MM_MIN;
                    max = DST_RULE_MM_MAX;
                  }
                  else if (current_ekran.index_position == INDEX_ML_DST_DOW)
                  {
                    pos = POS_DOW;
                    shift = SHIFT_DOW;

                    min = DST_RULE_DOW_MIN;
                    max = DST_RULE_DOW_MAX;
                  }
                  else if (current_ekran.index_position == INDEX_ML_DST_WR)
                  {
                    pos = POS_WR;
                    shift = SHIFT_WR;

                    min = DST_RULE_WR_MIN;
                    max = DST_RULE_WR_MAX;
                  }
                  else if (current_ekran.index_position == INDEX_ML_DST_HH)
                  {
                    pos = POS_HH;
                    shift = SHIFT_HH;

                    min = DST_RULE_HH_MIN;
                    max = DST_RULE_HH_MAX;
                  }
                  else
                  {
                    //Теоретично цього ніколи не мало б бути
                    total_error_sw_fixed();
                  }

                  uint32_t value = (*p_rule_target >> pos) & ((1 << shift) - 1);
                  if (++value > max)
                    value = min;

                  uint32_t bit_maska = ((1u << (pos + shift)) - 1) - ((1u << pos) - 1);
                  *p_rule_target &= ~bit_maska;
                  *p_rule_target |= value << pos;

                  //Формуємо екран
                  make_ekran_dst_rule(*p_rule_target);
                }
                else if (current_ekran.current_level == EKRAN_GENERAL_PICKUPS_EL)
                {
                  if (current_ekran.index_position == INDEX_ML_NUMBER_INERATION)
                  {
                    if ((current_ekran.position_cursor_x < COL_NUMBER_INERATION_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_NUMBER_INERATION_END))
                      current_ekran.position_cursor_x = COL_NUMBER_INERATION_BEGIN;
                  }
                  //                else if(current_ekran.index_position == INDEX_ML_NUMBER_DEFINED_FUNCTIONS)
                  //                {
                  //                  if ((current_ekran.position_cursor_x < COL_NUMBER_DEFINED_FUNCTIONS_BEGIN) ||
                  //                      (current_ekran.position_cursor_x > COL_NUMBER_DEFINED_FUNCTIONS_END))
                  //                    current_ekran.position_cursor_x = COL_NUMBER_DEFINED_FUNCTIONS_BEGIN;
                  //                }
                  //                else if(current_ekran.index_position == INDEX_ML_NUMBER_DEFINED_TRIGGERS)
                  //                {
                  //                  if ((current_ekran.position_cursor_x < COL_NUMBER_DEFINED_TRIGGERS_BEGIN) ||
                  //                      (current_ekran.position_cursor_x > COL_NUMBER_DEFINED_TRIGGERS_END))
                  //                    current_ekran.position_cursor_x = COL_NUMBER_DEFINED_TRIGGERS_BEGIN;
                  //                }
                  //                else if(current_ekran.index_position == INDEX_ML_NUMBER_DEFINED_AND)
                  //                {
                  //                  if ((current_ekran.position_cursor_x < COL_NUMBER_DEFINED_AND_BEGIN) ||
                  //                      (current_ekran.position_cursor_x > COL_NUMBER_DEFINED_AND_END))
                  //                    current_ekran.position_cursor_x = COL_NUMBER_DEFINED_AND_BEGIN;
                  //                }
                  //                else if(current_ekran.index_position == INDEX_ML_NUMBER_DEFINED_OR)
                  //                {
                  //                  if ((current_ekran.position_cursor_x < COL_NUMBER_DEFINED_OR_BEGIN) ||
                  //                      (current_ekran.position_cursor_x > COL_NUMBER_DEFINED_OR_END))
                  //                    current_ekran.position_cursor_x = COL_NUMBER_DEFINED_OR_BEGIN;
                  //                }
                  //                else if(current_ekran.index_position == INDEX_ML_NUMBER_DEFINED_XOR)
                  //                {
                  //                  if ((current_ekran.position_cursor_x < COL_NUMBER_DEFINED_XOR_BEGIN) ||
                  //                      (current_ekran.position_cursor_x > COL_NUMBER_DEFINED_XOR_END))
                  //                    current_ekran.position_cursor_x = COL_NUMBER_DEFINED_XOR_BEGIN;
                  //                }
                  //                else if(current_ekran.index_position == INDEX_ML_NUMBER_DEFINED_NOT)
                  //                {
                  //                  if ((current_ekran.position_cursor_x < COL_NUMBER_DEFINED_NOT_BEGIN) ||
                  //                      (current_ekran.position_cursor_x > COL_NUMBER_DEFINED_NOT_END))
                  //                    current_ekran.position_cursor_x = COL_NUMBER_DEFINED_NOT_BEGIN;
                  //                }

                  //Формуємо екран відображення загальних витримок для розширеної логіки
                  make_ekran_general_pickups_el();
                }
                else if (current_ekran.current_level == EKRAN_LIST_TYPE_DF)
                {
                  unsigned int value = (1 << current_ekran.index_position);

                  //Міняємо на протилежний відповідний біт для вибраної позиції
                  edition_settings.type_df ^= value;

                  //Формуємо екран відображення типу опреділювальної функції
                  make_ekran_type_df();
                }
                else if ((current_ekran.current_level >= EKRAN_TIMEOUT_DF1) && (current_ekran.current_level <= (EKRAN_TIMEOUT_DF1 + NUMBER_DEFINED_FUNCTIONS - 1)))
                {
                  if (current_ekran.index_position == INDEX_ML_TMO_DF_PAUSE)
                  {
                    if (current_ekran.position_cursor_x == COL_TMO_DF_PAUSE_COMMA)
                      current_ekran.position_cursor_x++;
                    if ((current_ekran.position_cursor_x < COL_TMO_DF_PAUSE_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_TMO_DF_PAUSE_END))
                      current_ekran.position_cursor_x = COL_TMO_DF_PAUSE_BEGIN;
                  }
                  else
                  {
                    if (current_ekran.position_cursor_x == COL_TMO_DF_WORK_COMMA)
                      current_ekran.position_cursor_x++;
                    if ((current_ekran.position_cursor_x < COL_TMO_DF_WORK_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_TMO_DF_WORK_END))
                      current_ekran.position_cursor_x = COL_TMO_DF_WORK_BEGIN;
                  }
                  //Формуємо екран таймерів опреділюваних функцій
                  make_ekran_timeout_df(current_ekran.current_level - EKRAN_TIMEOUT_DF1);
                }
                else if ((current_ekran.current_level >= EKRAN_LIST_SOURCE_TF1) && (current_ekran.current_level <= (EKRAN_LIST_SOURCE_TF1 + NUMBER_TRANSFER_FUNCTIONS - 1)))
                {
                  int32_t index_position = current_ekran.index_position;
                  uint32_t value = (edition_settings.ranguvannja_tf[current_ekran.current_level - EKRAN_LIST_SOURCE_TF1] >> (16 * index_position)) & 0xffff;

                  // do
                  {
                    if (++value >= max_value_for_tf[1 + _FIX_NUMBER_PROTECTION - 1][index_position])
                      value = 0;

                    for (size_t i = 0; i < _FIX_NUMBER_PROTECTION; i++)
                    {
                      if (
                        ((current_settings.configuration & (1 << i)) == 0) &&
                        (value >= max_value_for_tf[1 + i - 1][index_position]) &&
                        (value < max_value_for_tf[1 + i][index_position]))
                      {
                        value = (i < (_FIX_NUMBER_PROTECTION - 1)) ? max_value_for_tf[1 + i][index_position] : 0;
                      }
                    }
                  } /* while (
                    (index_position == INDEX_ML_LIST_SOURCE_INPUT_TF) &&
                    ((value == (1 + RANG_PO_NZZ)) || (value == (1 + RANG_NZZ)) || (value == (1 + RANG_SECTOR_NZZ))) &&
                    ((current_settings.control_zz & CTR_ZZ1_TYPE) != 0)); */

                  edition_settings.ranguvannja_tf[current_ekran.current_level - EKRAN_LIST_SOURCE_TF1] &= (uint32_t)(~(0xffff << (16 * index_position)));
                  edition_settings.ranguvannja_tf[current_ekran.current_level - EKRAN_LIST_SOURCE_TF1] |= ((value & 0xffff) << (16 * index_position));

                  //Формуємо екран
                  make_ekran_list_source_tf();
                }
                else if (current_ekran.current_level == EKRAN_TIMEOUT_DIGITAL_REGISTRATOR)
                {

                  if (current_ekran.index_position == INDEX_ML_TMO_ELONGATION)
                  {
                    if (current_ekran.position_cursor_x == COL_TMO_ELONGATION_COMMA)
                      current_ekran.position_cursor_x++;
                    if ((current_ekran.position_cursor_x < COL_TMO_ELONGATION_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_TMO_ELONGATION_END))
                      current_ekran.position_cursor_x = COL_TMO_ELONGATION_BEGIN;
                  }

                  //Формуємо екран витримок аналогового реєстратора
                  make_ekran_timeout_digital_registrator();
                }
                else if (current_ekran.current_level == EKRAN_TIMEOUT_ANALOG_REGISTRATOR)
                {
                  if (current_ekran.index_position == INDEX_ML_TMOPREFAULT)
                  {
                    if (current_ekran.position_cursor_x == COL_TMO_PREFAULT_COMMA)
                      current_ekran.position_cursor_x++;
                    if ((current_ekran.position_cursor_x < COL_TMO_PREFAULT_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_TMO_PREFAULT_END))
                      current_ekran.position_cursor_x = COL_TMO_PREFAULT_BEGIN;
                  }
                  else
                  {
                    if (current_ekran.position_cursor_x == COL_TMO_POSTFAULT_COMMA)
                      current_ekran.position_cursor_x++;
                    if ((current_ekran.position_cursor_x < COL_TMO_POSTFAULT_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_TMO_POSTFAULT_END))
                      current_ekran.position_cursor_x = COL_TMO_POSTFAULT_BEGIN;
                  }
                  //Формуємо екран витримок аналогового реєстратора
                  make_ekran_timeout_analog_registrator();
                }
                else if (current_ekran.current_level == EKRAN_CONTROL_AR)
                {
                  unsigned int value = (1 << current_ekran.index_position);

                  //Міняємо на протилежний відповідний біт для вибраної позиції
                  edition_settings.control_ar ^= value;

                  //Формуємо екран відображення
                  make_ekran_control_ar();
                }
                else if (current_ekran.current_level == EKRAN_CHOSE_EXTRA_SETTINGS)
                {
                  //Виділяємо, який біт треба міняти
                  unsigned int maska = (1 << current_ekran.index_position);

                  //Міняємо на протилежний відповідний біт для вибраної позиції
                  edition_settings.control_extra_settings_1 ^= maska;

                  //Формуємо екран відображення додаткових налаштувань
                  make_ekran_chose_extra_settings();
                }

                //Очистити сигналізацію, що натиснута кнопка
                new_state_keyboard &= ~(1u << BIT_KEY_RIGHT);
              }
              else if (new_state_keyboard == (1u << BIT_KEY_LEFT))
              {
                current_ekran.position_cursor_x--;
                //Перевіряємо, чи ми не вийшли за межі виділені для значення даної уставки, або позицію коми
                if (
                  (current_ekran.current_level >= EKRAN_SETPOINT_RPN_GROUP1) &&
                  (current_ekran.current_level <= EKRAN_SETPOINT_RPN_GROUP4))
                {
                  if (current_ekran.index_position == INDEX_ML_STPRPN_OSN)
                  {
                    if (current_ekran.position_cursor_x == COL_SETPOINT_RPN_OSN_COMMA)
                      current_ekran.position_cursor_x--;
                    if ((current_ekran.position_cursor_x < COL_SETPOINT_RPN_OSN_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_SETPOINT_RPN_OSN_END))
                      current_ekran.position_cursor_x = COL_SETPOINT_RPN_OSN_END;
                  }
                  else if (current_ekran.index_position == INDEX_ML_STPRPN_ZONE)
                  {
                    if (current_ekran.position_cursor_x == COL_SETPOINT_RPN_ZONE_COMMA)
                      current_ekran.position_cursor_x--;
                    if ((current_ekran.position_cursor_x < COL_SETPOINT_RPN_ZONE_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_SETPOINT_RPN_ZONE_END))
                      current_ekran.position_cursor_x = COL_SETPOINT_RPN_ZONE_END;
                  }
                  else if (current_ekran.index_position == INDEX_ML_STPRPN_MAX_PER)
                  {
                    if ((current_ekran.position_cursor_x < COL_SETPOINT_RPN_MAX_PER_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_SETPOINT_RPN_MAX_PER_END))
                      current_ekran.position_cursor_x = COL_SETPOINT_RPN_MAX_PER_END;
                  }
                  else if (current_ekran.index_position == INDEX_ML_STPRPN_DOD)
                  {
                    if (current_ekran.position_cursor_x == COL_SETPOINT_RPN_DOD_COMMA)
                      current_ekran.position_cursor_x--;
                    if ((current_ekran.position_cursor_x < COL_SETPOINT_RPN_DOD_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_SETPOINT_RPN_DOD_END))
                      current_ekran.position_cursor_x = COL_SETPOINT_RPN_DOD_END;
                  }
                  else if (current_ekran.index_position == INDEX_ML_STPRPN_K)
                  {
                    if (current_ekran.position_cursor_x == COL_SETPOINT_RPN_K_COMMA)
                      current_ekran.position_cursor_x--;
                    if ((current_ekran.position_cursor_x < COL_SETPOINT_RPN_K_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_SETPOINT_RPN_K_END))
                      current_ekran.position_cursor_x = COL_SETPOINT_RPN_K_END;
                  }

                  //Формуємо екран уставок РПН
                  int group = (current_ekran.current_level - EKRAN_SETPOINT_RPN_GROUP1);
                  make_ekran_setpoint_rpn(group);
                }
                else if (
                  (current_ekran.current_level >= EKRAN_TIMEOUT_RPN_GROUP1) &&
                  (current_ekran.current_level <= EKRAN_TIMEOUT_RPN_GROUP4))
                {
                  if (current_ekran.index_position == INDEX_ML_TMORPN_UB_PRYB)
                  {
                    if ((current_ekran.position_cursor_x < COL_TMO_RPN_UB_PRYB_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_TMO_RPN_UB_PRYB_END))
                      current_ekran.position_cursor_x = COL_TMO_RPN_UB_PRYB_END;
                  }
                  else if (current_ekran.index_position == INDEX_ML_TMORPN_UB_PRYB_PRYSK)
                  {
                    if ((current_ekran.position_cursor_x < COL_TMO_RPN_UB_PRYB_PRYSK_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_TMO_RPN_UB_PRYB_PRYSK_END))
                      current_ekran.position_cursor_x = COL_TMO_RPN_UB_PRYB_PRYSK_END;
                  }
                  else if (current_ekran.index_position == INDEX_ML_TMORPN_PRYSK_VID_UMAX)
                  {
                    if (current_ekran.position_cursor_x == COL_TMO_RPN_PRYSK_VID_UMAX_COMMA)
                      current_ekran.position_cursor_x--;
                    if ((current_ekran.position_cursor_x < COL_TMO_RPN_PRYSK_VID_UMAX_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_TMO_RPN_PRYSK_VID_UMAX_END))
                      current_ekran.position_cursor_x = COL_TMO_RPN_PRYSK_VID_UMAX_END;
                  }
                  else if (current_ekran.index_position == INDEX_ML_TMORPN_PEREKL)
                  {
                    if (current_ekran.position_cursor_x == COL_TMO_RPN_PEREKL_COMMA)
                      current_ekran.position_cursor_x--;
                    if ((current_ekran.position_cursor_x < COL_TMO_RPN_PEREKL_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_TMO_RPN_PEREKL_END))
                      current_ekran.position_cursor_x = COL_TMO_RPN_PEREKL_END;
                  }

                  else if (current_ekran.index_position == INDEX_ML_TMORPN_NESPR)
                  {
                    if (current_ekran.position_cursor_x == COL_TMO_RPN_NESPR_COMMA)
                      current_ekran.position_cursor_x--;
                    if ((current_ekran.position_cursor_x < COL_TMO_RPN_NESPR_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_TMO_RPN_NESPR_END))
                      current_ekran.position_cursor_x = COL_TMO_RPN_NESPR_END;
                  }

                  //Формуємо екран витримок РПН
                  int group = (current_ekran.current_level - EKRAN_TIMEOUT_RPN_GROUP1);
                  make_ekran_timeout_rpn(group);
                }
                else if (current_ekran.current_level == EKRAN_CONTROL_RPN)
                {
                  if (current_ekran.index_position < MAX_ROW_FOR_CONTROL_RPN)
                  {
                    //Міняємо на протилежний відповідний біт для вибраної позиції
                    edition_settings.control_rpn ^= (1u << current_ekran.index_position);
                  }

                  //Формуємо екран управлінської інформації для РПН
                  make_ekran_control_rpn();
                }
                else if (current_ekran.current_level == EKRAN_CONTROL_SZKh)
                {
                  if (current_ekran.index_position < MAX_ROW_FOR_CONTROL_SZKh)
                  {
                    //Міняємо на протилежний відповідний біт для вибраної позиції
                    edition_settings.control_rpn ^= (1u << current_ekran.index_position);
                  }

                  //Формуємо екран управлінської інформації для ЗСХ
                  make_ekran_control_zskh();
                }
                else if (
                  (current_ekran.current_level >= EKRAN_SETPOINT_BRP_GROUP1) &&
                  (current_ekran.current_level <= EKRAN_SETPOINT_BRP_GROUP4))
                {
                  if (current_ekran.index_position == INDEX_ML_STPBRP_I_BLK)
                  {
                    if (current_ekran.position_cursor_x == COL_SETPOINT_BRP_I_BLK_COMMA)
                      current_ekran.position_cursor_x--;
                    if ((current_ekran.position_cursor_x < COL_SETPOINT_BRP_I_BLK_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_SETPOINT_BRP_I_BLK_END))
                      current_ekran.position_cursor_x = COL_SETPOINT_BRP_I_BLK_END;
                  }

                  //Формуємо екран уставок БРП
                  int group = (current_ekran.current_level - EKRAN_SETPOINT_BRP_GROUP1);
                  make_ekran_setpoint_brp(group);
                }
                else if (current_ekran.current_level == EKRAN_CONTROL_BRP)
                {
                  //Міняємо на протилежний відповідний біт для вибраної позиції
                  edition_settings.control_brp ^= MASKA_FOR_BIT(current_ekran.index_position);

                  //Формуємо екран управлінської інформації для БРП
                  make_ekran_control_brp();
                }
                else if (
                  (current_ekran.current_level >= EKRAN_TIMEOUT_ZNKh_GROUP1) &&
                  (current_ekran.current_level <= EKRAN_TIMEOUT_ZNKh_GROUP4))
                {
                  if (current_ekran.index_position == INDEX_ML_TMOZNKh_PEREKL)
                  {
                    if (current_ekran.position_cursor_x == COL_TMO_ZNKh_PEREKL_COMMA)
                      current_ekran.position_cursor_x--;
                    if ((current_ekran.position_cursor_x < COL_TMO_ZNKh_PEREKL_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_TMO_ZNKh_PEREKL_END))
                      current_ekran.position_cursor_x = COL_TMO_ZNKh_PEREKL_END;
                  }
                  //Формуємо екран витримок ЗНХ
                  int group = (current_ekran.current_level - EKRAN_TIMEOUT_ZNKh_GROUP1);
                  make_ekran_timeout_znkh(group);
                }
                else if (current_ekran.current_level == EKRAN_CONTROL_ZNKh)
                {
                  if (current_ekran.index_position < MAX_ROW_FOR_CONTROL_ZNKh)
                  {
                    //Міняємо на протилежний відповідний біт для вибраної позиції
                    edition_settings.control_znkh ^= (1u << current_ekran.index_position);
                  }

                  //Формуємо екран управлінської інформації для ЗНХ
                  make_ekran_control_znkh();
                }
                else if (
                  (current_ekran.current_level >= EKRAN_SETPOINT_UMIN_GROUP1) &&
                  (current_ekran.current_level <= EKRAN_SETPOINT_UMIN_GROUP4))
                {
                  if (current_ekran.index_position == INDEX_ML_STPUmin_Umin1)
                  {
                    if ((current_ekran.position_cursor_x < COL_SETPOINT_Umin_Umin1_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_SETPOINT_Umin_Umin1_END))
                      current_ekran.position_cursor_x = COL_SETPOINT_Umin_Umin1_END;
                  }
                  else if (current_ekran.index_position == INDEX_ML_STPUmin_Umin2)
                  {
                    if ((current_ekran.position_cursor_x < COL_SETPOINT_Umin_Umin2_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_SETPOINT_Umin_Umin2_END))
                      current_ekran.position_cursor_x = COL_SETPOINT_Umin_Umin2_END;
                  }

                  //Формуємо екран уставок Umin
                  int group = (current_ekran.current_level - EKRAN_SETPOINT_UMIN_GROUP1);
                  make_ekran_setpoint_Umin(group);
                }
                else if (
                  (current_ekran.current_level >= EKRAN_TIMEOUT_UMIN_GROUP1) &&
                  (current_ekran.current_level <= EKRAN_TIMEOUT_UMIN_GROUP4))
                {
                  if (current_ekran.index_position == INDEX_ML_TMOUmin_Umin1)
                  {
                    if (current_ekran.position_cursor_x == COL_TMO_Umin_Umin1_COMMA)
                      current_ekran.position_cursor_x--;
                    if ((current_ekran.position_cursor_x < COL_TMO_Umin_Umin1_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_TMO_Umin_Umin1_END))
                      current_ekran.position_cursor_x = COL_TMO_Umin_Umin1_END;
                  }
                  else if (current_ekran.index_position == INDEX_ML_TMOUmin_Umin2)
                  {
                    if (current_ekran.position_cursor_x == COL_TMO_Umin_Umin2_COMMA)
                      current_ekran.position_cursor_x--;
                    if ((current_ekran.position_cursor_x < COL_TMO_Umin_Umin2_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_TMO_Umin_Umin2_END))
                      current_ekran.position_cursor_x = COL_TMO_Umin_Umin2_END;
                  }

                  //Формуємо екран витримок Umin
                  int group = (current_ekran.current_level - EKRAN_TIMEOUT_UMIN_GROUP1);
                  make_ekran_timeout_Umin(group);
                }
                else if (current_ekran.current_level == EKRAN_CONTROL_UMIN)
                {
                  unsigned int maska = 0;

                  //Виділяємо, який біт треба міняти
                  if (current_ekran.index_position == INDEX_ML_CTRUmin_STAGE_1)
                    maska = MASKA_FOR_BIT(INDEX_ML_CTRUmin_STAGE_1);
                  else if (current_ekran.index_position == INDEX_ML_CTRUmin_STAGE_2)
                    maska = MASKA_FOR_BIT(INDEX_ML_CTRUmin_STAGE_2);

                  //Міняємо на протилежний відповідний біт для вибраної позиції
                  edition_settings.control_Umin ^= maska;

                  //Формуємо екран управлінської інформації для Umin
                  make_ekran_control_Umin();
                }
                else if (
                  (current_ekran.current_level >= EKRAN_SETPOINT_UMAX_GROUP1) &&
                  (current_ekran.current_level <= EKRAN_SETPOINT_UMAX_GROUP4))
                {
                  if (current_ekran.index_position == INDEX_ML_STPUmax_Umax1)
                  {
                    if ((current_ekran.position_cursor_x < COL_SETPOINT_Umax_Umax1_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_SETPOINT_Umax_Umax1_END))
                      current_ekran.position_cursor_x = COL_SETPOINT_Umax_Umax1_END;
                  }
                  else if (current_ekran.index_position == INDEX_ML_STPUmax_Umax2)
                  {
                    if ((current_ekran.position_cursor_x < COL_SETPOINT_Umax_Umax2_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_SETPOINT_Umax_Umax2_END))
                      current_ekran.position_cursor_x = COL_SETPOINT_Umax_Umax2_END;
                  }

                  //Формуємо екран уставок Umax
                  int group = (current_ekran.current_level - EKRAN_SETPOINT_UMAX_GROUP1);
                  make_ekran_setpoint_Umax(group);
                }
                else if (
                  (current_ekran.current_level >= EKRAN_TIMEOUT_UMAX_GROUP1) &&
                  (current_ekran.current_level <= EKRAN_TIMEOUT_UMAX_GROUP4))
                {
                  if (current_ekran.index_position == INDEX_ML_TMOUmax_Umax2)
                  {
                    if (current_ekran.position_cursor_x == COL_TMO_Umax_Umax2_COMMA)
                      current_ekran.position_cursor_x--;
                    if ((current_ekran.position_cursor_x < COL_TMO_Umax_Umax2_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_TMO_Umax_Umax2_END))
                      current_ekran.position_cursor_x = COL_TMO_Umax_Umax2_END;
                  }

                  //Формуємо екран витримок Umax
                  int group = (current_ekran.current_level - EKRAN_TIMEOUT_UMAX_GROUP1);
                  make_ekran_timeout_Umax(group);
                }
                else if (current_ekran.current_level == EKRAN_CONTROL_UMAX)
                {
                  unsigned int maska = 0;

                  //Виділяємо, який біт треба міняти
                  if (current_ekran.index_position == INDEX_ML_CTRUmax_STAGE_1)
                    maska = MASKA_FOR_BIT(INDEX_ML_CTRUmax_STAGE_1);
                  else if (current_ekran.index_position == INDEX_ML_CTRUmax_STAGE_2)
                    maska = MASKA_FOR_BIT(INDEX_ML_CTRUmax_STAGE_2);

                  //Міняємо на протилежний відповідний біт для вибраної позиції
                  edition_settings.control_Umax ^= maska;

                  //Формуємо екран управлінської інформації для Umax
                  make_ekran_control_Umax();
                }
                else if (
                  (current_ekran.current_level >= EKRAN_SETPOINT_UP_GROUP1) &&
                  (current_ekran.current_level <= EKRAN_SETPOINT_UP_GROUP4))
                {
                  uint32_t _n_index = current_ekran.index_position % MAX_ROW_FOR_SETPOINT_UP;

                  if (_n_index == INDEX_ML_STP_UP)
                  {
                    uint32_t _n_UP = current_ekran.index_position / MAX_ROW_FOR_SETPOINT_UP;
                    int32_t begin = 0, comma = 0, end = 0;
                    switch (current_settings.ctrl_UP_input[_n_UP])
                    {
                      case UP_CTRL_Ia_Ib_Ic:
                      case UP_CTRL_Ia:
                      case UP_CTRL_Ib:
                      case UP_CTRL_Ic:
                      case UP_CTRL_I1:
                      case UP_CTRL_I2:
                      case UP_CTRL_I04:
                      case UP_CTRL_3I0_r:
                        {
                          begin = COL_SETPOINT_UP_I_BEGIN;
                          comma = COL_SETPOINT_UP_I_COMMA;
                          end = COL_SETPOINT_UP_I_END;
                          break;
                        }
                      case UP_CTRL_3I0:
                      case UP_CTRL_3I0_others:
                        {
                          begin = COL_SETPOINT_UP_3I0_BEGIN;
                          comma = COL_SETPOINT_UP_3I0_COMMA;
                          end = COL_SETPOINT_UP_3I0_END;
                          break;
                        }
                      case UP_CTRL_Ua_Ub_Uc:
                      case UP_CTRL_Uab_Ubc_Uca:
                      case UP_CTRL_Ua:
                      case UP_CTRL_Uab:
                      case UP_CTRL_Ub:
                      case UP_CTRL_Ubc:
                      case UP_CTRL_Uc:
                      case UP_CTRL_Uca:
                      case UP_CTRL_U1:
                      case UP_CTRL_U2:
                      case UP_CTRL_3U0:
                        {
                          begin = COL_SETPOINT_UP_U_BEGIN;
                          comma = COL_SETPOINT_UP_U_COMMA;
                          end = COL_SETPOINT_UP_U_END;
                          break;
                        }
                      case UP_CTRL_P:
                      case UP_CTRL_Q:
                        {
                          begin = COL_SETPOINT_UP_PQ_BEGIN - 1;
                          comma = COL_SETPOINT_UP_PQ_COMMA;
                          end = COL_SETPOINT_UP_PQ_END;
                          break;
                        }
                      case UP_CTRL_S:
                        {
                          begin = COL_SETPOINT_UP_S_BEGIN;
                          comma = COL_SETPOINT_UP_S_COMMA;
                          end = COL_SETPOINT_UP_S_END;
                          break;
                        }
                      default:
                        {
                          //Теоретично цього ніколи не мало б бути
                          total_error_sw_fixed();
                        }
                    }

                    if (current_ekran.position_cursor_x == comma)
                      current_ekran.position_cursor_x--;
                    if ((current_ekran.position_cursor_x < begin) ||
                        (current_ekran.position_cursor_x > end))
                      current_ekran.position_cursor_x = end;
                  }
                  else if (_n_index == INDEX_ML_STP_UP_KP)
                  {
                    if (current_ekran.position_cursor_x == COL_SETPOINT_UP_KP_COMMA)
                      current_ekran.position_cursor_x--;
                    if ((current_ekran.position_cursor_x < COL_SETPOINT_UP_KP_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_SETPOINT_UP_KP_END))
                      current_ekran.position_cursor_x = COL_SETPOINT_UP_KP_END;
                  }

                  //Формуємо екран уставок УЗ
                  int group = (current_ekran.current_level - EKRAN_SETPOINT_UP_GROUP1);
                  make_ekran_setpoint_UP(group);
                }
                else if (
                  (current_ekran.current_level >= EKRAN_TIMEOUT_UP_GROUP1) &&
                  (current_ekran.current_level <= EKRAN_TIMEOUT_UP_GROUP4))
                {
                  uint32_t _n_index = current_ekran.index_position % MAX_ROW_FOR_TIMEOUT_UP;

                  if (_n_index == INDEX_ML_TMOUP)
                  {
                    if (current_ekran.position_cursor_x == COL_TMO_UP_COMMA)
                      current_ekran.position_cursor_x--;
                    if ((current_ekran.position_cursor_x < COL_TMO_UP_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_TMO_UP_END))
                      current_ekran.position_cursor_x = COL_TMO_UP_END;
                  }

                  //Формуємо екран витримок УЗ
                  int group = (current_ekran.current_level - EKRAN_TIMEOUT_UP_GROUP1);
                  make_ekran_timeout_UP(group);
                }
                else if (current_ekran.current_level == EKRAN_CONTROL_UP)
                {
                  uint32_t n_UP = current_ekran.index_position / (_CTR_UP_NEXT_BIT - _CTR_UP_PART_I);
                  uint32_t n_index = current_ekran.index_position % (_CTR_UP_NEXT_BIT - _CTR_UP_PART_I);
                  if (n_index == CTR_UP_CTRL_INPUT)
                  {
                    uint32_t ctrl_maska = MASKA_FOR_BIT(n_UP * (_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_STATE_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I)) |
                                          MASKA_FOR_BIT(n_UP * (_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_OR_AND_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I)) |
                                          MASKA_FOR_BIT(n_UP * (_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_MORE_LESS_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I));
                    edition_settings.control_UP &= (uint32_t)(~ctrl_maska);

                    do
                    {
                      if (--edition_settings.ctrl_UP_input[n_UP] < 0)
                        edition_settings.ctrl_UP_input[n_UP] = _UP_CTRL_NUMBER - 1;
                    } while (
                      (edition_settings.ctrl_UP_input[n_UP] == UP_CTRL_Ua_Ub_Uc) ||
                      (edition_settings.ctrl_UP_input[n_UP] == UP_CTRL_Ua) ||
                      (edition_settings.ctrl_UP_input[n_UP] == UP_CTRL_Ub) ||
                      (edition_settings.ctrl_UP_input[n_UP] == UP_CTRL_Uc) ||
                      (edition_settings.ctrl_UP_input[n_UP] == UP_CTRL_U1) ||
                      (edition_settings.ctrl_UP_input[n_UP] == UP_CTRL_U2));
                  }
                  else
                  {
                    unsigned int maska = 0;

                    //Виділяємо, який біт треба міняти
                    if (n_index == CTR_UP_STATE_BIT)
                      maska = MASKA_FOR_BIT(n_UP * (_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_STATE_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I));
                    else if (n_index == CTR_UP_OR_AND_BIT)
                      maska = MASKA_FOR_BIT(n_UP * (_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_OR_AND_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I));
                    else if (n_index == CTR_UP_MORE_LESS_BIT)
                      maska = MASKA_FOR_BIT(n_UP * (_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_MORE_LESS_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I));

                    //Міняємо на протилежний відповідний біт для вибраної позиції
                    edition_settings.control_UP ^= maska;
                  }

                  //Формуємо екран управлінської інформації для УЗ
                  make_ekran_control_UP();
                }
                else if (current_ekran.current_level == EKRAN_TRANSFORMATOR_INFO)
                {
                  if (
                    (current_ekran.index_position == INDEX_ML_TT1) ||
                    (current_ekran.index_position == INDEX_ML_TT2))
                  {
                    if ((current_ekran.position_cursor_x < COL_TT_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_TT_END))
                      current_ekran.position_cursor_x = COL_TT_END;
                  }
                  else
                  {
                    if ((current_ekran.position_cursor_x < COL_TN_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_TN_END))
                      current_ekran.position_cursor_x = COL_TN_END;
                  }
                  //Формуємо екран відображення інформації по трансформаторах
                  make_ekran_transformator();
                }
                else if (current_ekran.current_level == EKRAN_SETPOINT_SWITCH)
                {
                  if (current_ekran.index_position == INDEX_ML_STPInom)
                  {
                    if ((current_ekran.position_cursor_x < COL_SETPOINT_Inom_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_SETPOINT_Inom_END))
                      current_ekran.position_cursor_x = COL_SETPOINT_Inom_END;
                  }
                  else if (current_ekran.index_position == INDEX_ML_STPRKS_Inom)
                  {
                    if ((current_ekran.position_cursor_x < COL_SETPOINT_RKS_Inom_BEGIN) ||
                        (current_ekran.position_cursor_x > (COL_SETPOINT_RKS_Inom_END - 1)))
                      current_ekran.position_cursor_x = COL_SETPOINT_RKS_Inom_END - 1;
                  }
                  else if (current_ekran.index_position == INDEX_ML_STPMInom_vymk)
                  {
                    if (current_ekran.position_cursor_x == COL_SETPOINT_Inom_vymk_COMMA)
                      current_ekran.position_cursor_x--;
                    if ((current_ekran.position_cursor_x < COL_SETPOINT_Inom_vymk_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_SETPOINT_Inom_vymk_END))
                      current_ekran.position_cursor_x = COL_SETPOINT_Inom_vymk_END;
                  }
                  else if (current_ekran.index_position == INDEX_ML_STPRKS_Inom_vymk)
                  {
                    if ((current_ekran.position_cursor_x < COL_SETPOINT_RKS_Inom_vymk_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_SETPOINT_RKS_Inom_vymk_END))
                      current_ekran.position_cursor_x = COL_SETPOINT_RKS_Inom_vymk_END;
                  }
                  else if (current_ekran.index_position == INDEX_ML_STPPOCHATKOVYJ_RESURS)
                  {
                    if ((current_ekran.position_cursor_x < COL_SETPOINT_POCHATKOVYJ_RESURS_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_SETPOINT_POCHATKOVYJ_RESURS_END))
                      current_ekran.position_cursor_x = COL_SETPOINT_POCHATKOVYJ_RESURS_END;
                  }
                  else if (current_ekran.index_position == INDEX_ML_STPKRYTYCHNYJ_RESURS)
                  {
                    if ((current_ekran.position_cursor_x < COL_SETPOINT_KRYTYCHNYJ_RESURS_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_SETPOINT_KRYTYCHNYJ_RESURS_END))
                      current_ekran.position_cursor_x = COL_SETPOINT_KRYTYCHNYJ_RESURS_END;
                  }
                  else if (current_ekran.index_position == INDEX_ML_STPPOCHATKOVA_K_VYMK)
                  {
                    if ((current_ekran.position_cursor_x < COL_SETPOINT_POCHATKOVA_K_VYMK_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_SETPOINT_POCHATKOVA_K_VYMK_END))
                      current_ekran.position_cursor_x = COL_SETPOINT_POCHATKOVA_K_VYMK_END;
                  }
                  //Формуємо екран уставок виключателя
                  make_ekran_setpoint_switch();
                }
                else if (current_ekran.current_level == EKRAN_TIMEOUT_SWITCH)
                {
                  if (current_ekran.index_position == INDEX_ML_TMOON)
                  {
                    if (current_ekran.position_cursor_x == COL_TMO_SWCH_ON_COMMA)
                      current_ekran.position_cursor_x--;
                    if ((current_ekran.position_cursor_x < COL_TMO_SWCH_ON_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_TMO_SWCH_ON_END))
                      current_ekran.position_cursor_x = COL_TMO_SWCH_ON_END;
                  }
                  else if (current_ekran.index_position == INDEX_ML_TMOOFF)
                  {
                    if (current_ekran.position_cursor_x == COL_TMO_SWCH_OFF_COMMA)
                      current_ekran.position_cursor_x--;
                    if ((current_ekran.position_cursor_x < COL_TMO_SWCH_OFF_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_TMO_SWCH_OFF_END))
                      current_ekran.position_cursor_x = COL_TMO_SWCH_OFF_END;
                  }
                  else if (current_ekran.index_position == INDEX_ML_TMOUDL_BLK_ON)
                  {
                    if (current_ekran.position_cursor_x == COL_TMO_SWCH_UDL_BLK_ON_COMMA)
                      current_ekran.position_cursor_x--;
                    if ((current_ekran.position_cursor_x < COL_TMO_SWCH_UDL_BLK_ON_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_TMO_SWCH_UDL_BLK_ON_END))
                      current_ekran.position_cursor_x = COL_TMO_SWCH_UDL_BLK_ON_END;
                  }
                  else if (current_ekran.index_position == INDEX_ML_TMOPRYVODA_VV)
                  {
                    if (current_ekran.position_cursor_x == COL_TMO_PRYVODA_VV_COMMA)
                      current_ekran.position_cursor_x--;
                    if ((current_ekran.position_cursor_x < COL_TMO_PRYVODA_VV_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_TMO_PRYVODA_VV_END))
                      current_ekran.position_cursor_x = COL_TMO_PRYVODA_VV_END;
                  }
                  //Формуємо екран витримок виключателя
                  make_ekran_timeout_switch();
                }
                else if (current_ekran.current_level == EKRAN_CONTROL_SWITCH)
                {
                  unsigned int maska = 0;

                  //Виділяємо, який біт треба міняти
                  if (current_ekran.index_position == INDEX_ML_CTRPRYVOD_VV)
                    maska = CTR_PRYVOD_VV;
                  else if (current_ekran.index_position == INDEX_ML_CTRRESURS_VV)
                    maska = CTR_RESURS_VV;

                  //Міняємо на протилежний відповідний біт для вибраної позиції
                  edition_settings.control_switch ^= maska;

                  //Формуємо екран управлінської інформації для вимикача
                  make_ekran_control_switch();
                }
                else if (current_ekran.current_level == EKRAN_DOPUSK_DV_UVV)
                {
                  if (
                    (current_ekran.position_cursor_x < COL_DOPUSK_DV_BEGIN) ||
                    (current_ekran.position_cursor_x > COL_DOPUSK_DV_END) ||
                    ((current_ekran.position_cursor_x == COL_DOPUSK_DV_END) &&
                     ((current_settings.type_of_input_signal & (1 << current_ekran.index_position)) != 0)))
                  {
                    if ((current_settings.type_of_input_signal & (1 << current_ekran.index_position)) != 0)
                      current_ekran.position_cursor_x = (COL_DOPUSK_DV_END - 1);
                    else
                      current_ekran.position_cursor_x = COL_DOPUSK_DV_END;
                  }

                  //Формуємо екран інфтрмації по допусках ДВ
                  make_ekran_dopusk_dv();
                }
                else if ((current_ekran.current_level == EKRAN_TYPE_INPUT_UVV) || (current_ekran.current_level == EKRAN_TYPE_INPUT_SIGNAL_UVV) ||
                         (current_ekran.current_level == EKRAN_TYPE_LED_UVV))
                {
                  unsigned int value = (1 << current_ekran.index_position);

                  //Міняємо на протилежний відповідний біт для вибраної позиції і формуємо екран управлінської інформації для УВВ
                  if (current_ekran.current_level == EKRAN_TYPE_INPUT_UVV)
                  {
                    edition_settings.type_of_input ^= value;
                    make_ekran_type_input_uvv(0);
                  }
                  else if (current_ekran.current_level == EKRAN_TYPE_INPUT_SIGNAL_UVV)
                  {
                    edition_settings.type_of_input_signal ^= value;
                    make_ekran_type_input_uvv(1);
                  }
                  else
                  {
                    edition_settings.type_of_led ^= value;
                    make_ekran_type_led_uvv();
                  }
                }
                else if (current_ekran.current_level == EKRAN_TYPE_OUTPUT_UVV)
                {
                  unsigned int maska = (1 << current_ekran.index_position);

                  int value = ((edition_settings.type_of_output & maska) != 0);
                  if (value == true)
                    value += ((edition_settings.type_of_output_modif & maska) != 0); //тільки у випадку, коли вихід сигнальний
                  if ((--value) < 0)
                    value = 2;

                  if (value == 0)
                  {
                    edition_settings.type_of_output &= (unsigned int) (~maska);
                    edition_settings.type_of_output_modif &= (unsigned int) (~maska);
                  }
                  else
                  {
                    edition_settings.type_of_output |= maska;
                    if (value == 1)
                      edition_settings.type_of_output_modif &= (unsigned int) (~maska);
                    else
                      edition_settings.type_of_output_modif |= maska;
                  }

                  make_ekran_type_output_uvv();
                }
                else if (current_ekran.current_level == EKRAN_TYPE_BUTTON_UVV)
                {
                  edition_settings.buttons_mode ^= (1 << current_ekran.index_position);
                  make_ekran_type_button_uvv();
                }
                else if (current_ekran.current_level == EKRAN_ADDRESS_RS485)
                {
                  if ((current_ekran.position_cursor_x < COL_ADDRESS_BEGIN) ||
                      (current_ekran.position_cursor_x > COL_ADDRESS_END))
                    current_ekran.position_cursor_x = COL_ADDRESS_END;
                  //Формуємо екран інфтрмації по комунікаційній адресі
                  make_ekran_address();
                }
                else if (current_ekran.current_level == EKRAN_VIEW_TIMEOUT_RS485)
                {
                  if (current_ekran.position_cursor_x == COL_TIMEOUT_INTERFACE_COMMA)
                    current_ekran.position_cursor_x--;
                  if ((current_ekran.position_cursor_x < COL_TIMEOUT_INTERFACE_BEGIN) ||
                      (current_ekran.position_cursor_x > COL_TIMEOUT_INTERFACE_END))
                    current_ekran.position_cursor_x = COL_TIMEOUT_INTERFACE_END;
                  //Формуємо екран інформації по time-out наступного символу
                  make_ekran_timeout_interface();
                }
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                else if (current_ekran.current_level == EKRAN_SETTING_NETWORK_LAYER_ETHERNET)
                {
                  if (
                    (current_ekran.index_position == INDEX_ML_NL_IPV4) ||
                    (current_ekran.index_position == INDEX_ML_NL_GATEWAY))
                  {
                    if ((current_ekran.position_cursor_x < COL_IP4_GATEWAY_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_IP4_GATEWAY_END))
                      current_ekran.position_cursor_x = COL_IP4_GATEWAY_END;

                    size_t index = (current_ekran.position_cursor_x - COL_IP4_GATEWAY_BEGIN) >> 2; /*ділення на 4, бо ХХХ. - це чотири цифри*/
                    if (current_ekran.position_cursor_x == (COL_IP4_GATEWAY_BEGIN + (3 + 1) * (index + 1) - 1))
                      current_ekran.position_cursor_x--;

                    if ((current_ekran.position_cursor_x < COL_IP4_GATEWAY_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_IP4_GATEWAY_END))
                      current_ekran.position_cursor_x = COL_IP4_GATEWAY_END;
                  }
                  else if (current_ekran.index_position == INDEX_ML_NL_MASK)
                  {
                    if ((current_ekran.position_cursor_x < COL_MASK_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_MASK_END))
                      current_ekran.position_cursor_x = COL_MASK_END;
                  }
                  //Формуємо екран інфтрмації по налаштуваннях мережевого рівня Ethernet
                  make_ekran_settings_network_layer_Ethernet();
                }
                else if (current_ekran.current_level == EKRAN_SYNCHRO)
                {
                  if (current_ekran.index_position == INDEX_ML_SYN_IPV4)
                  {
                    if ((current_ekran.position_cursor_x < COL_IP4_SERVER_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_IP4_SERVER_END))
                      current_ekran.position_cursor_x = COL_IP4_SERVER_END;

                    size_t index = (current_ekran.position_cursor_x - COL_IP4_SERVER_BEGIN) >> 2; /*ділення на 4, бо ХХХ. - це чотири цифри*/
                    if (current_ekran.position_cursor_x == (COL_IP4_SERVER_BEGIN + (3 + 1) * (index + 1) - 1))
                      current_ekran.position_cursor_x--;

                    if ((current_ekran.position_cursor_x < COL_IP4_SERVER_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_IP4_SERVER_END))
                      current_ekran.position_cursor_x = COL_IP4_SERVER_END;
                  }
                  else if (current_ekran.index_position == INDEX_ML_SYN_PORT)
                  {
                    if ((current_ekran.position_cursor_x < COL_PORT_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_PORT_END))
                      current_ekran.position_cursor_x = COL_PORT_END;
                  }
                  else if (current_ekran.index_position == INDEX_ML_SYN_PERIOD)
                  {
                    if ((current_ekran.position_cursor_x < COL_PERIOD_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_PEDIOD_END))
                      current_ekran.position_cursor_x = COL_PEDIOD_END;
                  }
                  //Формуємо екран інфтрмації
                  make_ekran_settings_synchro();
                }
#endif
                else if (current_ekran.current_level == EKRAN_TIME_ZONE)
                {
                  if (current_ekran.index_position == INDEX_ML_TIME_ZONE)
                  {
                    if (--edition_settings.time_zone < TIME_ZONE_MIN)
                      edition_settings.time_zone = TIME_ZONE_MAX;
                  }
                  else if (current_ekran.index_position == INDEX_ML_DST)
                  {
                    //Міняємо на протилежний відповідний біт для вибраної позиції
                    edition_settings.dst ^= MASKA_FOR_BIT(N_BIT_TZ_DST);
                  }

                  //Формуємо екран
                  make_ekran_timezone_dst();
                }
                else if (current_ekran.current_level == EKRAN_DST_RULE)
                {
                  uint32_t *p_rule_target = (position_in_current_level_menu[previous_level_in_current_level_menu[EKRAN_DST_RULE]] == INDEX_ML_CHDT_DST_ON) ? &edition_settings.dst_on_rule : &edition_settings.dst_off_rule;
                  unsigned int pos = 0, shift = 0;
                  unsigned int min = 0, max = 0;

                  if (current_ekran.index_position == INDEX_ML_DST_MM)
                  {
                    pos = POS_MM;
                    shift = SHIFT_MM;

                    min = DST_RULE_MM_MIN;
                    max = DST_RULE_MM_MAX;
                  }
                  else if (current_ekran.index_position == INDEX_ML_DST_DOW)
                  {
                    pos = POS_DOW;
                    shift = SHIFT_DOW;

                    min = DST_RULE_DOW_MIN;
                    max = DST_RULE_DOW_MAX;
                  }
                  else if (current_ekran.index_position == INDEX_ML_DST_WR)
                  {
                    pos = POS_WR;
                    shift = SHIFT_WR;

                    min = DST_RULE_WR_MIN;
                    max = DST_RULE_WR_MAX;
                  }
                  else if (current_ekran.index_position == INDEX_ML_DST_HH)
                  {
                    pos = POS_HH;
                    shift = SHIFT_HH;

                    min = DST_RULE_HH_MIN;
                    max = DST_RULE_HH_MAX;
                  }
                  else
                  {
                    //Теоретично цього ніколи не мало б бути
                    total_error_sw_fixed();
                  }

                  int32_t value = (*p_rule_target >> pos) & ((1 << shift) - 1);
                  if (--value < (int) min)
                    value = max;

                  uint32_t bit_maska = ((1u << (pos + shift)) - 1) - ((1u << pos) - 1);
                  *p_rule_target &= ~bit_maska;
                  *p_rule_target |= value << pos;

                  //Формуємо екран
                  make_ekran_dst_rule(*p_rule_target);
                }
                else if (current_ekran.current_level == EKRAN_GENERAL_PICKUPS_EL)
                {
                  if (current_ekran.index_position == INDEX_ML_NUMBER_INERATION)
                  {
                    if ((current_ekran.position_cursor_x < COL_NUMBER_INERATION_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_NUMBER_INERATION_END))
                      current_ekran.position_cursor_x = COL_NUMBER_INERATION_END;
                  }
                  //                else if(current_ekran.index_position == INDEX_ML_NUMBER_DEFINED_FUNCTIONS)
                  //                {
                  //                  if ((current_ekran.position_cursor_x < COL_NUMBER_DEFINED_FUNCTIONS_BEGIN) ||
                  //                      (current_ekran.position_cursor_x > COL_NUMBER_DEFINED_FUNCTIONS_END))
                  //                    current_ekran.position_cursor_x = COL_NUMBER_DEFINED_FUNCTIONS_END;
                  //                }
                  //                else if(current_ekran.index_position == INDEX_ML_NUMBER_DEFINED_TRIGGERS)
                  //                {
                  //                  if ((current_ekran.position_cursor_x < COL_NUMBER_DEFINED_TRIGGERS_BEGIN) ||
                  //                      (current_ekran.position_cursor_x > COL_NUMBER_DEFINED_TRIGGERS_END))
                  //                    current_ekran.position_cursor_x = COL_NUMBER_DEFINED_TRIGGERS_END;
                  //                }
                  //                else if(current_ekran.index_position == INDEX_ML_NUMBER_DEFINED_AND)
                  //                {
                  //                  if ((current_ekran.position_cursor_x < COL_NUMBER_DEFINED_AND_BEGIN) ||
                  //                      (current_ekran.position_cursor_x > COL_NUMBER_DEFINED_AND_END))
                  //                    current_ekran.position_cursor_x = COL_NUMBER_DEFINED_AND_END;
                  //                }
                  //                else if(current_ekran.index_position == INDEX_ML_NUMBER_DEFINED_OR)
                  //                {
                  //                  if ((current_ekran.position_cursor_x < COL_NUMBER_DEFINED_OR_BEGIN) ||
                  //                      (current_ekran.position_cursor_x > COL_NUMBER_DEFINED_OR_END))
                  //                    current_ekran.position_cursor_x = COL_NUMBER_DEFINED_OR_END;
                  //                }
                  //                else if(current_ekran.index_position == INDEX_ML_NUMBER_DEFINED_XOR)
                  //                {
                  //                  if ((current_ekran.position_cursor_x < COL_NUMBER_DEFINED_XOR_BEGIN) ||
                  //                      (current_ekran.position_cursor_x > COL_NUMBER_DEFINED_XOR_END))
                  //                    current_ekran.position_cursor_x = COL_NUMBER_DEFINED_XOR_END;
                  //                }
                  //                else if(current_ekran.index_position == INDEX_ML_NUMBER_DEFINED_NOT)
                  //                {
                  //                  if ((current_ekran.position_cursor_x < COL_NUMBER_DEFINED_NOT_BEGIN) ||
                  //                      (current_ekran.position_cursor_x > COL_NUMBER_DEFINED_NOT_END))
                  //                    current_ekran.position_cursor_x = COL_NUMBER_DEFINED_NOT_END;
                  //                }

                  //Формуємо екран відображення загальних витримок для розширеної логіки
                  make_ekran_general_pickups_el();
                }
                else if (current_ekran.current_level == EKRAN_LIST_TYPE_DF)
                {
                  unsigned int value = (1 << current_ekran.index_position);

                  //Міняємо на протилежний відповідний біт для вибраної позиції
                  edition_settings.type_df ^= value;

                  //Формуємо екран відображення типу опреділювальної функції
                  make_ekran_type_df();
                }
                else if ((current_ekran.current_level >= EKRAN_TIMEOUT_DF1) && (current_ekran.current_level <= (EKRAN_TIMEOUT_DF1 + NUMBER_DEFINED_FUNCTIONS - 1)))
                {
                  if (current_ekran.index_position == INDEX_ML_TMO_DF_PAUSE)
                  {
                    if (current_ekran.position_cursor_x == COL_TMO_DF_PAUSE_COMMA)
                      current_ekran.position_cursor_x--;
                    if ((current_ekran.position_cursor_x < COL_TMO_DF_PAUSE_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_TMO_DF_PAUSE_END))
                      current_ekran.position_cursor_x = COL_TMO_DF_PAUSE_END;
                  }
                  else
                  {
                    if (current_ekran.position_cursor_x == COL_TMO_DF_WORK_COMMA)
                      current_ekran.position_cursor_x--;
                    if ((current_ekran.position_cursor_x < COL_TMO_DF_WORK_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_TMO_DF_WORK_END))
                      current_ekran.position_cursor_x = COL_TMO_DF_WORK_END;
                  }
                  //Формуємо екран таймерів опреділюваних функцій
                  make_ekran_timeout_df(current_ekran.current_level - EKRAN_TIMEOUT_DF1);
                }
                else if ((current_ekran.current_level >= EKRAN_LIST_SOURCE_TF1) && (current_ekran.current_level <= (EKRAN_LIST_SOURCE_TF1 + NUMBER_TRANSFER_FUNCTIONS - 1)))
                {
                  int32_t index_position = current_ekran.index_position;
                  int32_t value = (edition_settings.ranguvannja_tf[current_ekran.current_level - EKRAN_LIST_SOURCE_TF1] >> (16 * index_position)) & 0xffff;

                  // do
                  {
                    if (--value < 0)
                      value = max_value_for_tf[1 + _FIX_NUMBER_PROTECTION - 1][index_position] - 1;

                    for (intptr_t i = (_FIX_NUMBER_PROTECTION - 1); i >= 0; i--)
                    {
                      if (
                        ((current_settings.configuration & (1 << i)) == 0) &&
                        ((uint32_t) value >= max_value_for_tf[1 + i - 1][index_position]) &&
                        ((uint32_t) value < max_value_for_tf[1 + i][index_position]))
                      {
                        value = max_value_for_tf[1 + i - 1][index_position] - 1;
                      }
                    }
                  } /* while (
                    (index_position == INDEX_ML_LIST_SOURCE_INPUT_TF) &&
                    ((value == (1 + RANG_PO_NZZ)) || (value == (1 + RANG_NZZ)) || (value == (1 + RANG_SECTOR_NZZ))) &&
                    ((current_settings.control_zz & CTR_ZZ1_TYPE) != 0)); */

                  edition_settings.ranguvannja_tf[current_ekran.current_level - EKRAN_LIST_SOURCE_TF1] &= (uint32_t)(~(0xffff << (16 * index_position)));
                  edition_settings.ranguvannja_tf[current_ekran.current_level - EKRAN_LIST_SOURCE_TF1] |= ((value & 0xffff) << (16 * index_position));

                  //Формуємо екран
                  make_ekran_list_source_tf();
                }
                else if (current_ekran.current_level == EKRAN_TIMEOUT_DIGITAL_REGISTRATOR)
                {
                  if (current_ekran.index_position == INDEX_ML_TMO_ELONGATION)
                  {
                    if (current_ekran.position_cursor_x == COL_TMO_ELONGATION_COMMA)
                      current_ekran.position_cursor_x--;
                    if ((current_ekran.position_cursor_x < COL_TMO_ELONGATION_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_TMO_ELONGATION_END))
                      current_ekran.position_cursor_x = COL_TMO_ELONGATION_END;
                  }

                  //Формуємо екран витримок аналогового реєстратора
                  make_ekran_timeout_digital_registrator();
                }
                else if (current_ekran.current_level == EKRAN_TIMEOUT_ANALOG_REGISTRATOR)
                {
                  if (current_ekran.index_position == INDEX_ML_TMOPREFAULT)
                  {
                    if (current_ekran.position_cursor_x == COL_TMO_PREFAULT_COMMA)
                      current_ekran.position_cursor_x--;
                    if ((current_ekran.position_cursor_x < COL_TMO_PREFAULT_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_TMO_PREFAULT_END))
                      current_ekran.position_cursor_x = COL_TMO_PREFAULT_END;
                  }
                  else
                  {
                    if (current_ekran.position_cursor_x == COL_TMO_POSTFAULT_COMMA)
                      current_ekran.position_cursor_x--;
                    if ((current_ekran.position_cursor_x < COL_TMO_POSTFAULT_BEGIN) ||
                        (current_ekran.position_cursor_x > COL_TMO_POSTFAULT_END))
                      current_ekran.position_cursor_x = COL_TMO_POSTFAULT_END;
                  }
                  //Формуємо екран витримок аналогового реєстратора
                  make_ekran_timeout_analog_registrator();
                }
                else if (current_ekran.current_level == EKRAN_CONTROL_AR)
                {
                  unsigned int value = (1 << current_ekran.index_position);

                  //Міняємо на протилежний відповідний біт для вибраної позиції
                  edition_settings.control_ar ^= value;

                  //Формуємо екран відображення
                  make_ekran_control_ar();
                }
                else if (current_ekran.current_level == EKRAN_CHOSE_EXTRA_SETTINGS)
                {
                  //Виділяємо, який біт треба міняти
                  unsigned int maska = (1 << current_ekran.index_position);

                  //Міняємо на протилежний відповідний біт для вибраної позиції
                  edition_settings.control_extra_settings_1 ^= maska;

                  //Формуємо екран відображення додаткових налаштувань
                  make_ekran_chose_extra_settings();
                }

                //Очистити сигналізацію, що натиснута кнопка
                new_state_keyboard &= ~(1u << BIT_KEY_LEFT);
              }
              else
              {
                //Натиснуто зразу декілька кнопок - це є невизначена ситуація, тому скидаємо сигналізацію про натиснуті кнопки і чекаємо знову
                unsigned int temp_data = new_state_keyboard;
                new_state_keyboard &= ~temp_data;
              }
            }
          }
          break;
        }
        /******************************************************************************************************************************************/

        /******************************************************************************************************************************************/
      case EKRAN_REPROGRAM:
        {
          //Очищаємо всі біти краім упралінських
          unsigned int maska_keyboard_bits = (1u << BIT_KEY_ENTER) |
                                             (1u << BIT_KEY_ESC) |
                                             (1u << BIT_REWRITE);

          new_state_keyboard &= maska_keyboard_bits;
          //Дальше виконуємо дії, якщо натиснута кнопка на яку треба реагувати, або стоїть команда обновити екран
          if (new_state_keyboard != 0)
          {
            //Пріоритет стоїть на обновлені екрану
            if ((new_state_keyboard & (1u << BIT_REWRITE)) != 0)
            {
              if (current_ekran.edition == 0)
              {
                //Теоретично цього ніколи не мало б бути
                total_error_sw_fixed();
              }
              else if ((current_ekran.edition == 1) || (current_ekran.edition == 2))
              {
                current_ekran.cursor_on = 0;
                current_ekran.cursor_blinking_on = 0;
                if (current_ekran.current_level == EKRAN_REPROGRAM)
                {
                  unsigned char information_about_reprogram[1][MAX_NAMBER_LANGUAGE][MAX_COL_LCD] =
                    {
                      "Режим перепрогр.",
                      "Режим перепрогр.",
                      " Reprogram Mode ",
                      "Режим перепрогр."};

                  make_ekran_about_activation_command(0, information_about_reprogram);
                }

                //Переходимо в режим введення значень у дію
                current_ekran.edition = 2;
              }

              //Очищаємо біт обновлення екрану
              new_state_keyboard &= ~(1u << BIT_REWRITE);
            }
            else
            {
              if (new_state_keyboard == (1u << BIT_KEY_ENTER))
              {
                //Натиснута кнопка ENTER
                if (current_ekran.edition == 0)
                {
                  int temp_current_level = current_ekran.current_level;

                  //Переходимо на меню запиту паролю скидання ресурсу вимикача
                  current_ekran.current_level = EKRAN_LEVEL_PASSWORD_HARD;
                  previous_level_in_current_level_menu[current_ekran.current_level] = temp_current_level;
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                }
                else if (current_ekran.edition == 2)
                {
                  if (current_ekran.current_level == EKRAN_REPROGRAM)
                  {
                    //Подаємо команду на перехід у режим перепрограмування
                    _SET_STATE(reprogram_device, REPROGRAM_COMMAND);

                    //Переходимо у попереднє меню
                    current_ekran.current_level = previous_level_in_current_level_menu[current_ekran.current_level];
                    current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  }

                  //Виходимо з режиму редагування
                  current_ekran.edition = 0;
                }

                //Виставляємо біт обновлення екрану
                new_state_keyboard |= (1u << BIT_REWRITE);

                //Очистити сигналізацію, що натиснута кнопка
                new_state_keyboard &= ~(1u << BIT_KEY_ENTER);
              }
              else if (new_state_keyboard == (1u << BIT_KEY_ESC))
              {
                if (
                  (current_ekran.edition == 0) ||
                  (current_ekran.current_level == EKRAN_REPROGRAM))
                {
                  //Вихід у режимі спостерігання
                  //Переходимо у попереднє меню
                  current_ekran.current_level = previous_level_in_current_level_menu[current_ekran.current_level];
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  current_ekran.edition = 0;
                }
                else
                {
                  //Теоретично цього ніколи не мало б бути
                  total_error_sw_fixed();
                }

                //Виставляємо команду на обновлекння нового екрану
                new_state_keyboard |= (1u << BIT_REWRITE);
                //Очистити сигналізацію, що натиснута кнопка
                new_state_keyboard &= ~(1u << BIT_KEY_ESC);
              }
              else
              {
                //Натиснуто зразу декілька кнопок - це є невизначена ситуація, тому скидаємо сигналізацію про натиснуті кнопки і чекаємо знову
                unsigned int temp_data = new_state_keyboard;
                new_state_keyboard &= ~temp_data;
              }
            }
          }
          break;
        }
        /******************************************************************************************************************************************/

        /******************************************************************************************************************************************/
      case EKRAN_REPROGRAM_ACTIVE:
      case EKRAN_UNSUPPORT_REPROGRAM:
        {
          //Очищаємо всі біти краім упралінських
          unsigned int maska_keyboard_bits = (1u << BIT_KEY_ENTER) | (1u << BIT_REWRITE);

          new_state_keyboard &= maska_keyboard_bits;
          //Дальше виконуємо дії, якщо натиснута кнопка на яку треба реагувати, або стоїть команда обновити екран
          if (new_state_keyboard != 0)
          {
            //Пріоритет стоїть на обновлені екрану
            if ((new_state_keyboard & (1u << BIT_REWRITE)) != 0)
            {
              static uint8_t const name_string1[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] =
                {
                  {"Режим перепрогр.", "Режим не поддерж"},
                  {"Режим перепрогр.", "Режим не підтрим"},
                  {" Reprogram Mode ", "Unsupported Mode"},
                  {"Режим перепрогр.", "Режим не поддерж"}};

              static uint8_t const name_string2[MAX_NAMBER_LANGUAGE][MAX_COL_LCD] =
                {
                  "  Выйти: Enter  ",
                  "  Вийти: Enter  ",
                  "   Exit: Enter  ",
                  "  Выйти: Enter  "};

              int index_language = index_language_in_array(current_settings.language);
              uint8_t const *const pArray[2] =
                {
                  name_string1[index_language][current_ekran.current_level == EKRAN_UNSUPPORT_REPROGRAM],
                  name_string2[index_language]};
              for (size_t i = 0; i < MAX_ROW_LCD; ++i)
              {
                //Наступні рядки треба перевірити, чи їх требе відображати у текучій коффігурації
                if (i < 2)
                {
                  for (size_t j = 0; j < MAX_COL_LCD; ++j)
                    working_ekran[i][j] = pArray[i][j];
                }
                else
                  for (size_t j = 0; j < MAX_COL_LCD; ++j)
                    working_ekran[i][j] = ' ';
              }

              current_ekran.cursor_on = 0;
              current_ekran.cursor_blinking_on = 0;
              current_ekran.position_cursor_x = 0;
              current_ekran.position_cursor_y = 0;
              //Обновити повністю весь екран
              current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;

              //Очищаємо біт обновлення екрану
              new_state_keyboard &= ~(1u << BIT_REWRITE);
            }
            else
            {
              if (new_state_keyboard == (1u << BIT_KEY_ENTER))
              {
                //Натиснута кнопка ENTER
                //Подаємо команду на перехід у режим перепрограмування
                if (current_ekran.current_level == EKRAN_REPROGRAM_ACTIVE)
                  _SET_STATE(reprogram_device, STAFF_COMMAND);
                else
                  _CLEAR_STATE(reprogram_device, UNSUPPORT_REPROGRAM);

                //Очистити сигналізацію, що натиснута кнопка
                new_state_keyboard &= ~(1u << BIT_KEY_ENTER);
              }
              else
              {
                //Натиснуто зразу декілька кнопок - це є невизначена ситуація, тому скидаємо сигналізацію про натиснуті кнопки і чекаємо знову
                unsigned int temp_data = new_state_keyboard;
                new_state_keyboard &= ~temp_data;
              }
            }
          }
          break;
        }
        /******************************************************************************************************************************************/

        /******************************************************************************************************************************************/
      case (EKRAN_RANGUVANNJA_INPUT_1 + 0):
      case (EKRAN_RANGUVANNJA_INPUT_1 + 1):
      case (EKRAN_RANGUVANNJA_INPUT_1 + 2):
      case (EKRAN_RANGUVANNJA_INPUT_1 + 3):
      case (EKRAN_RANGUVANNJA_INPUT_1 + 4):
      case (EKRAN_RANGUVANNJA_INPUT_1 + 5):
      case (EKRAN_RANGUVANNJA_INPUT_1 + 6):
      case (EKRAN_RANGUVANNJA_INPUT_1 + 7):

#if (                              \
  (MODYFIKACIA_VERSII_PZ == 0) ||  \
  (MODYFIKACIA_VERSII_PZ == 1) ||  \
  (MODYFIKACIA_VERSII_PZ == 3) ||  \
  (MODYFIKACIA_VERSII_PZ == 5) ||  \
  (MODYFIKACIA_VERSII_PZ == 6) ||  \
  (MODYFIKACIA_VERSII_PZ == 7) ||  \
  (MODYFIKACIA_VERSII_PZ == 8) ||  \
  (MODYFIKACIA_VERSII_PZ == 10) || \
  (MODYFIKACIA_VERSII_PZ == 11) || \
  (MODYFIKACIA_VERSII_PZ == 13) || \
  (MODYFIKACIA_VERSII_PZ == 15) || \
  (MODYFIKACIA_VERSII_PZ == 17) || \
  (MODYFIKACIA_VERSII_PZ == 18) || \
  (MODYFIKACIA_VERSII_PZ == 23) || \
  (MODYFIKACIA_VERSII_PZ == 26) || \
  (MODYFIKACIA_VERSII_PZ == 33))

      case (EKRAN_RANGUVANNJA_INPUT_1 + 8):
      case (EKRAN_RANGUVANNJA_INPUT_1 + 9):
      case (EKRAN_RANGUVANNJA_INPUT_1 + 10):
      case (EKRAN_RANGUVANNJA_INPUT_1 + 11):

#if (                              \
  (MODYFIKACIA_VERSII_PZ == 0) ||  \
  (MODYFIKACIA_VERSII_PZ == 1) ||  \
  (MODYFIKACIA_VERSII_PZ == 3) ||  \
  (MODYFIKACIA_VERSII_PZ == 5) ||  \
  (MODYFIKACIA_VERSII_PZ == 7) ||  \
  (MODYFIKACIA_VERSII_PZ == 8) ||  \
  (MODYFIKACIA_VERSII_PZ == 10) || \
  (MODYFIKACIA_VERSII_PZ == 11) || \
  (MODYFIKACIA_VERSII_PZ == 13) || \
  (MODYFIKACIA_VERSII_PZ == 15) || \
  (MODYFIKACIA_VERSII_PZ == 17) || \
  (MODYFIKACIA_VERSII_PZ == 18) || \
  (MODYFIKACIA_VERSII_PZ == 23) || \
  (MODYFIKACIA_VERSII_PZ == 33))

      case (EKRAN_RANGUVANNJA_INPUT_1 + 12):

#if (                              \
  (MODYFIKACIA_VERSII_PZ == 0) ||  \
  (MODYFIKACIA_VERSII_PZ == 1) ||  \
  (MODYFIKACIA_VERSII_PZ == 3) ||  \
  (MODYFIKACIA_VERSII_PZ == 5) ||  \
  (MODYFIKACIA_VERSII_PZ == 8) ||  \
  (MODYFIKACIA_VERSII_PZ == 10) || \
  (MODYFIKACIA_VERSII_PZ == 11) || \
  (MODYFIKACIA_VERSII_PZ == 13) || \
  (MODYFIKACIA_VERSII_PZ == 15) || \
  (MODYFIKACIA_VERSII_PZ == 18) || \
  (MODYFIKACIA_VERSII_PZ == 23) || \
  (MODYFIKACIA_VERSII_PZ == 33))

      case (EKRAN_RANGUVANNJA_INPUT_1 + 13):
      case (EKRAN_RANGUVANNJA_INPUT_1 + 14):
      case (EKRAN_RANGUVANNJA_INPUT_1 + 15):

#if (                              \
  (MODYFIKACIA_VERSII_PZ == 0) ||  \
  (MODYFIKACIA_VERSII_PZ == 5) ||  \
  (MODYFIKACIA_VERSII_PZ == 8) ||  \
  (MODYFIKACIA_VERSII_PZ == 10) || \
  (MODYFIKACIA_VERSII_PZ == 15) || \
  (MODYFIKACIA_VERSII_PZ == 18))

      case (EKRAN_RANGUVANNJA_INPUT_1 + 16):
      case (EKRAN_RANGUVANNJA_INPUT_1 + 17):
      case (EKRAN_RANGUVANNJA_INPUT_1 + 18):
      case (EKRAN_RANGUVANNJA_INPUT_1 + 19):

#if (                             \
  (MODYFIKACIA_VERSII_PZ == 8) || \
  (MODYFIKACIA_VERSII_PZ == 18))

      case (EKRAN_RANGUVANNJA_INPUT_1 + 20):
      case (EKRAN_RANGUVANNJA_INPUT_1 + 21):
      case (EKRAN_RANGUVANNJA_INPUT_1 + 22):
      case (EKRAN_RANGUVANNJA_INPUT_1 + 23):
      case (EKRAN_RANGUVANNJA_INPUT_1 + 24):
      case (EKRAN_RANGUVANNJA_INPUT_1 + 25):
      case (EKRAN_RANGUVANNJA_INPUT_1 + 26):
      case (EKRAN_RANGUVANNJA_INPUT_1 + 27):
      case (EKRAN_RANGUVANNJA_INPUT_1 + 28):
      case (EKRAN_RANGUVANNJA_INPUT_1 + 29):
      case (EKRAN_RANGUVANNJA_INPUT_1 + 30):
      case (EKRAN_RANGUVANNJA_INPUT_1 + 31):

#endif

#endif

#endif

#endif

#endif

      case (EKRAN_RANGUVANNJA_OUTPUT_1 + 0):
      case (EKRAN_RANGUVANNJA_OUTPUT_1 + 1):
      case (EKRAN_RANGUVANNJA_OUTPUT_1 + 2):
      case (EKRAN_RANGUVANNJA_OUTPUT_1 + 3):
      case (EKRAN_RANGUVANNJA_OUTPUT_1 + 4):
      case (EKRAN_RANGUVANNJA_OUTPUT_1 + 5):
      case (EKRAN_RANGUVANNJA_OUTPUT_1 + 6):
      case (EKRAN_RANGUVANNJA_OUTPUT_1 + 7):
      case (EKRAN_RANGUVANNJA_OUTPUT_1 + 8):

#if (                              \
  (MODYFIKACIA_VERSII_PZ == 0) ||  \
  (MODYFIKACIA_VERSII_PZ == 1) ||  \
  (MODYFIKACIA_VERSII_PZ == 3) ||  \
  (MODYFIKACIA_VERSII_PZ == 5) ||  \
  (MODYFIKACIA_VERSII_PZ == 6) ||  \
  (MODYFIKACIA_VERSII_PZ == 7) ||  \
  (MODYFIKACIA_VERSII_PZ == 8) ||  \
  (MODYFIKACIA_VERSII_PZ == 10) || \
  (MODYFIKACIA_VERSII_PZ == 11) || \
  (MODYFIKACIA_VERSII_PZ == 13) || \
  (MODYFIKACIA_VERSII_PZ == 15) || \
  (MODYFIKACIA_VERSII_PZ == 17) || \
  (MODYFIKACIA_VERSII_PZ == 18) || \
  (MODYFIKACIA_VERSII_PZ == 23) || \
  (MODYFIKACIA_VERSII_PZ == 24) || \
  (MODYFIKACIA_VERSII_PZ == 26) || \
  (MODYFIKACIA_VERSII_PZ == 33) || \
  (MODYFIKACIA_VERSII_PZ == 34))

      case (EKRAN_RANGUVANNJA_OUTPUT_1 + 9):

#if (                              \
  (MODYFIKACIA_VERSII_PZ == 0) ||  \
  (MODYFIKACIA_VERSII_PZ == 1) ||  \
  (MODYFIKACIA_VERSII_PZ == 3) ||  \
  (MODYFIKACIA_VERSII_PZ == 5) ||  \
  (MODYFIKACIA_VERSII_PZ == 6) ||  \
  (MODYFIKACIA_VERSII_PZ == 7) ||  \
  (MODYFIKACIA_VERSII_PZ == 8) ||  \
  (MODYFIKACIA_VERSII_PZ == 10) || \
  (MODYFIKACIA_VERSII_PZ == 11) || \
  (MODYFIKACIA_VERSII_PZ == 13) || \
  (MODYFIKACIA_VERSII_PZ == 15) || \
  (MODYFIKACIA_VERSII_PZ == 17) || \
  (MODYFIKACIA_VERSII_PZ == 18) || \
  (MODYFIKACIA_VERSII_PZ == 23) || \
  (MODYFIKACIA_VERSII_PZ == 26) || \
  (MODYFIKACIA_VERSII_PZ == 33))

      case (EKRAN_RANGUVANNJA_OUTPUT_1 + 10):
      case (EKRAN_RANGUVANNJA_OUTPUT_1 + 11):
      case (EKRAN_RANGUVANNJA_OUTPUT_1 + 12):

#if (                              \
  (MODYFIKACIA_VERSII_PZ == 0) ||  \
  (MODYFIKACIA_VERSII_PZ == 1) ||  \
  (MODYFIKACIA_VERSII_PZ == 3) ||  \
  (MODYFIKACIA_VERSII_PZ == 5) ||  \
  (MODYFIKACIA_VERSII_PZ == 8) ||  \
  (MODYFIKACIA_VERSII_PZ == 10) || \
  (MODYFIKACIA_VERSII_PZ == 11) || \
  (MODYFIKACIA_VERSII_PZ == 13) || \
  (MODYFIKACIA_VERSII_PZ == 15) || \
  (MODYFIKACIA_VERSII_PZ == 18) || \
  (MODYFIKACIA_VERSII_PZ == 23) || \
  (MODYFIKACIA_VERSII_PZ == 26) || \
  (MODYFIKACIA_VERSII_PZ == 33))

      case (EKRAN_RANGUVANNJA_OUTPUT_1 + 13):

#if (                              \
  (MODYFIKACIA_VERSII_PZ == 0) ||  \
  (MODYFIKACIA_VERSII_PZ == 1) ||  \
  (MODYFIKACIA_VERSII_PZ == 3) ||  \
  (MODYFIKACIA_VERSII_PZ == 5) ||  \
  (MODYFIKACIA_VERSII_PZ == 8) ||  \
  (MODYFIKACIA_VERSII_PZ == 10) || \
  (MODYFIKACIA_VERSII_PZ == 11) || \
  (MODYFIKACIA_VERSII_PZ == 13) || \
  (MODYFIKACIA_VERSII_PZ == 15) || \
  (MODYFIKACIA_VERSII_PZ == 18) || \
  (MODYFIKACIA_VERSII_PZ == 23) || \
  (MODYFIKACIA_VERSII_PZ == 33))

      case (EKRAN_RANGUVANNJA_OUTPUT_1 + 14):
      case (EKRAN_RANGUVANNJA_OUTPUT_1 + 15):

#if (                              \
  (MODYFIKACIA_VERSII_PZ == 5) ||  \
  (MODYFIKACIA_VERSII_PZ == 8) ||  \
  (MODYFIKACIA_VERSII_PZ == 15) || \
  (MODYFIKACIA_VERSII_PZ == 18) || \
  (MODYFIKACIA_VERSII_PZ == 23) || \
  (MODYFIKACIA_VERSII_PZ == 33))

      case (EKRAN_RANGUVANNJA_OUTPUT_1 + 16):

#if (                              \
  (MODYFIKACIA_VERSII_PZ == 5) ||  \
  (MODYFIKACIA_VERSII_PZ == 8) ||  \
  (MODYFIKACIA_VERSII_PZ == 15) || \
  (MODYFIKACIA_VERSII_PZ == 18))

      case (EKRAN_RANGUVANNJA_OUTPUT_1 + 17):
      case (EKRAN_RANGUVANNJA_OUTPUT_1 + 18):
      case (EKRAN_RANGUVANNJA_OUTPUT_1 + 19):

#if (                             \
  (MODYFIKACIA_VERSII_PZ == 8) || \
  (MODYFIKACIA_VERSII_PZ == 18))

      case (EKRAN_RANGUVANNJA_OUTPUT_1 + 20):
      case (EKRAN_RANGUVANNJA_OUTPUT_1 + 21):
      case (EKRAN_RANGUVANNJA_OUTPUT_1 + 22):
      case (EKRAN_RANGUVANNJA_OUTPUT_1 + 23):

#endif

#endif

#endif

#endif

#endif

#endif

#endif

      case (EKRAN_RANGUVANNJA_LED_1 + 0):
      case (EKRAN_RANGUVANNJA_LED_1 + 1):
      case (EKRAN_RANGUVANNJA_LED_1 + 2):
      case (EKRAN_RANGUVANNJA_LED_1 + 3):
      case (EKRAN_RANGUVANNJA_LED_1 + 4):
      case (EKRAN_RANGUVANNJA_LED_1 + 5):
      case (EKRAN_RANGUVANNJA_LED_1 + 6):
      case (EKRAN_RANGUVANNJA_LED_1 + 7):
      case (EKRAN_RANGUVANNJA_LED_1 + 8):
      case (EKRAN_RANGUVANNJA_LED_1 + 9):
      case (EKRAN_RANGUVANNJA_LED_1 + 10):
      case (EKRAN_RANGUVANNJA_LED_1 + 11):
      case (EKRAN_RANGUVANNJA_LED_1 + 12):
      case (EKRAN_RANGUVANNJA_LED_1 + 13):
      case (EKRAN_RANGUVANNJA_LED_1 + 14):
      case (EKRAN_RANGUVANNJA_LED_1 + 15):
      case (EKRAN_RANGUVANNJA_LED_1 + 16):
      case EKRAN_RANGUVANNJA_ANALOG_REGISTRATOR:
      case EKRAN_RANGUVANNJA_DIGITAL_REGISTRATOR:
      case EKRAN_RANGUVANNJA_OFF_CB:
      case EKRAN_RANGUVANNJA_ON_CB:
      case (EKRAN_RANGUVANNJA_DF1_PLUS + MAX_ROW_LIST_TYPE_SOURCE_DF * 0):
      case (EKRAN_RANGUVANNJA_DF1_MINUS + MAX_ROW_LIST_TYPE_SOURCE_DF * 0):
      case (EKRAN_RANGUVANNJA_DF1_BLK + MAX_ROW_LIST_TYPE_SOURCE_DF * 0):
      case (EKRAN_RANGUVANNJA_DF1_RESET + MAX_ROW_LIST_TYPE_SOURCE_DF * 0):
      case (EKRAN_RANGUVANNJA_DF1_PLUS + MAX_ROW_LIST_TYPE_SOURCE_DF * 1):
      case (EKRAN_RANGUVANNJA_DF1_MINUS + MAX_ROW_LIST_TYPE_SOURCE_DF * 1):
      case (EKRAN_RANGUVANNJA_DF1_BLK + MAX_ROW_LIST_TYPE_SOURCE_DF * 1):
      case (EKRAN_RANGUVANNJA_DF1_RESET + MAX_ROW_LIST_TYPE_SOURCE_DF * 1):
      case (EKRAN_RANGUVANNJA_DF1_PLUS + MAX_ROW_LIST_TYPE_SOURCE_DF * 2):
      case (EKRAN_RANGUVANNJA_DF1_MINUS + MAX_ROW_LIST_TYPE_SOURCE_DF * 2):
      case (EKRAN_RANGUVANNJA_DF1_BLK + MAX_ROW_LIST_TYPE_SOURCE_DF * 2):
      case (EKRAN_RANGUVANNJA_DF1_RESET + MAX_ROW_LIST_TYPE_SOURCE_DF * 2):
      case (EKRAN_RANGUVANNJA_DF1_PLUS + MAX_ROW_LIST_TYPE_SOURCE_DF * 3):
      case (EKRAN_RANGUVANNJA_DF1_MINUS + MAX_ROW_LIST_TYPE_SOURCE_DF * 3):
      case (EKRAN_RANGUVANNJA_DF1_BLK + MAX_ROW_LIST_TYPE_SOURCE_DF * 3):
      case (EKRAN_RANGUVANNJA_DF1_RESET + MAX_ROW_LIST_TYPE_SOURCE_DF * 3):
      case (EKRAN_RANGUVANNJA_DF1_PLUS + MAX_ROW_LIST_TYPE_SOURCE_DF * 4):
      case (EKRAN_RANGUVANNJA_DF1_MINUS + MAX_ROW_LIST_TYPE_SOURCE_DF * 4):
      case (EKRAN_RANGUVANNJA_DF1_BLK + MAX_ROW_LIST_TYPE_SOURCE_DF * 4):
      case (EKRAN_RANGUVANNJA_DF1_RESET + MAX_ROW_LIST_TYPE_SOURCE_DF * 4):
      case (EKRAN_RANGUVANNJA_DF1_PLUS + MAX_ROW_LIST_TYPE_SOURCE_DF * 5):
      case (EKRAN_RANGUVANNJA_DF1_MINUS + MAX_ROW_LIST_TYPE_SOURCE_DF * 5):
      case (EKRAN_RANGUVANNJA_DF1_BLK + MAX_ROW_LIST_TYPE_SOURCE_DF * 5):
      case (EKRAN_RANGUVANNJA_DF1_RESET + MAX_ROW_LIST_TYPE_SOURCE_DF * 5):
      case (EKRAN_RANGUVANNJA_DF1_PLUS + MAX_ROW_LIST_TYPE_SOURCE_DF * 6):
      case (EKRAN_RANGUVANNJA_DF1_MINUS + MAX_ROW_LIST_TYPE_SOURCE_DF * 6):
      case (EKRAN_RANGUVANNJA_DF1_BLK + MAX_ROW_LIST_TYPE_SOURCE_DF * 6):
      case (EKRAN_RANGUVANNJA_DF1_RESET + MAX_ROW_LIST_TYPE_SOURCE_DF * 6):
      case (EKRAN_RANGUVANNJA_DF1_PLUS + MAX_ROW_LIST_TYPE_SOURCE_DF * 7):
      case (EKRAN_RANGUVANNJA_DF1_MINUS + MAX_ROW_LIST_TYPE_SOURCE_DF * 7):
      case (EKRAN_RANGUVANNJA_DF1_BLK + MAX_ROW_LIST_TYPE_SOURCE_DF * 7):
      case (EKRAN_RANGUVANNJA_DF1_RESET + MAX_ROW_LIST_TYPE_SOURCE_DF * 7):
      case (EKRAN_RANGUVANNJA_SET_DT1_PLUS + 4 * 0):
      case (EKRAN_RANGUVANNJA_SET_DT1_MINUS + 4 * 0):
      case (EKRAN_RANGUVANNJA_RESET_DT1_PLUS + 4 * 0):
      case (EKRAN_RANGUVANNJA_RESET_DT1_MINUS + 4 * 0):
      case (EKRAN_RANGUVANNJA_SET_DT1_PLUS + 4 * 1):
      case (EKRAN_RANGUVANNJA_SET_DT1_MINUS + 4 * 1):
      case (EKRAN_RANGUVANNJA_RESET_DT1_PLUS + 4 * 1):
      case (EKRAN_RANGUVANNJA_RESET_DT1_MINUS + 4 * 1):
      case (EKRAN_RANGUVANNJA_SET_DT1_PLUS + 4 * 2):
      case (EKRAN_RANGUVANNJA_SET_DT1_MINUS + 4 * 2):
      case (EKRAN_RANGUVANNJA_RESET_DT1_PLUS + 4 * 2):
      case (EKRAN_RANGUVANNJA_RESET_DT1_MINUS + 4 * 2):
      case (EKRAN_RANGUVANNJA_SET_DT1_PLUS + 4 * 3):
      case (EKRAN_RANGUVANNJA_SET_DT1_MINUS + 4 * 3):
      case (EKRAN_RANGUVANNJA_RESET_DT1_PLUS + 4 * 3):
      case (EKRAN_RANGUVANNJA_RESET_DT1_MINUS + 4 * 3):
      case (EKRAN_RANGUVANNJA_D_AND1 + 0):
      case (EKRAN_RANGUVANNJA_D_AND1 + 1):
      case (EKRAN_RANGUVANNJA_D_AND1 + 2):
      case (EKRAN_RANGUVANNJA_D_AND1 + 3):
      case (EKRAN_RANGUVANNJA_D_AND1 + 4):
      case (EKRAN_RANGUVANNJA_D_AND1 + 5):
      case (EKRAN_RANGUVANNJA_D_AND1 + 6):
      case (EKRAN_RANGUVANNJA_D_AND1 + 7):
      case (EKRAN_RANGUVANNJA_D_OR1 + 0):
      case (EKRAN_RANGUVANNJA_D_OR1 + 1):
      case (EKRAN_RANGUVANNJA_D_OR1 + 2):
      case (EKRAN_RANGUVANNJA_D_OR1 + 3):
      case (EKRAN_RANGUVANNJA_D_OR1 + 4):
      case (EKRAN_RANGUVANNJA_D_OR1 + 5):
      case (EKRAN_RANGUVANNJA_D_OR1 + 6):
      case (EKRAN_RANGUVANNJA_D_OR1 + 7):
      case (EKRAN_RANGUVANNJA_D_XOR1 + 0):
      case (EKRAN_RANGUVANNJA_D_XOR1 + 1):
      case (EKRAN_RANGUVANNJA_D_XOR1 + 2):
      case (EKRAN_RANGUVANNJA_D_XOR1 + 3):
      case (EKRAN_RANGUVANNJA_D_XOR1 + 4):
      case (EKRAN_RANGUVANNJA_D_XOR1 + 5):
      case (EKRAN_RANGUVANNJA_D_XOR1 + 6):
      case (EKRAN_RANGUVANNJA_D_XOR1 + 7):
      case (EKRAN_RANGUVANNJA_D_NOT1 + 0):
      case (EKRAN_RANGUVANNJA_D_NOT1 + 1):
      case (EKRAN_RANGUVANNJA_D_NOT1 + 2):
      case (EKRAN_RANGUVANNJA_D_NOT1 + 3):
      case (EKRAN_RANGUVANNJA_D_NOT1 + 4):
      case (EKRAN_RANGUVANNJA_D_NOT1 + 5):
      case (EKRAN_RANGUVANNJA_D_NOT1 + 6):
      case (EKRAN_RANGUVANNJA_D_NOT1 + 7):
      case (EKRAN_RANGUVANNJA_D_NOT1 + 8):
      case (EKRAN_RANGUVANNJA_D_NOT1 + 9):
      case (EKRAN_RANGUVANNJA_D_NOT1 + 10):
      case (EKRAN_RANGUVANNJA_D_NOT1 + 11):
      case (EKRAN_RANGUVANNJA_D_NOT1 + 12):
      case (EKRAN_RANGUVANNJA_D_NOT1 + 13):
      case (EKRAN_RANGUVANNJA_D_NOT1 + 14):
      case (EKRAN_RANGUVANNJA_D_NOT1 + 15):
      case (EKRAN_RANGUVANNJA_BUTTON_1 + 0):
      case (EKRAN_RANGUVANNJA_BUTTON_1 + 1):
      case (EKRAN_RANGUVANNJA_BUTTON_1 + 2):
      case (EKRAN_RANGUVANNJA_BUTTON_1 + 3):
      case (EKRAN_RANGUVANNJA_BUTTON_1 + 4):
      case (EKRAN_RANGUVANNJA_BUTTON_1 + 5):

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
      case EKRAN_LN_FOR_IEC61850_RANG_SMALL:
      case EKRAN_LN_FOR_IEC61850_RANG:
#endif

        {
          //Очищаємо всі біти краім упралінських
          unsigned int maska_keyboard_bits = (1u << BIT_KEY_ENTER) | (1u << BIT_KEY_ESC) | (1u << BIT_REWRITE);

          if (current_ekran.edition == 1)
            maska_keyboard_bits |= (1u << BIT_KEY_RIGHT) | (1u << BIT_KEY_LEFT) | (1u << BIT_KEY_UP) | (1u << BIT_KEY_DOWN);
          else if (current_ekran.edition == 0)
            maska_keyboard_bits |= (1u << BIT_KEY_UP) | (1u << BIT_KEY_DOWN);

          new_state_keyboard &= maska_keyboard_bits;
          //Дальше виконуємо дії, якщо натиснута кнопка на яку треба реагівати, або стоїть команда обновити екран
          if (new_state_keyboard != 0)
          {
            //Пріоритет стоїть на обновлені екрану
            if ((new_state_keyboard & (1u << BIT_REWRITE)) != 0)
            {
              if (
                ((current_ekran.current_level >= EKRAN_RANGUVANNJA_BUTTON_1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_BUTTON_1 + NUMBER_DEFINED_BUTTONS - 1))) ||
                ((current_ekran.current_level >= EKRAN_RANGUVANNJA_INPUT_1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_INPUT_1 + NUMBER_INPUTS - 1)))
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                ||
                (current_ekran.current_level == EKRAN_LN_FOR_IEC61850_RANG_SMALL)
#endif
              )
              {
                unsigned int temp_state[N_SMALL];
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                int type_LN = -1;
                int n_LN = -1;
#endif

                if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_BUTTON_1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_BUTTON_1 + NUMBER_DEFINED_BUTTONS - 1)))
                {
                  uint32_t *p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_buttons : edition_settings.ranguvannja_buttons;
                  for (size_t i = 0; i < N_SMALL; i++)
                    temp_state[i] = p_rang[N_SMALL * (current_ekran.current_level - EKRAN_RANGUVANNJA_BUTTON_1) + i];
                }
                else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_INPUT_1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_INPUT_1 + NUMBER_INPUTS - 1)))
                {
                  uint32_t *p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_inputs : edition_settings.ranguvannja_inputs;
                  for (size_t i = 0; i < N_SMALL; i++)
                    temp_state[i] = p_rang[N_SMALL * (current_ekran.current_level - EKRAN_RANGUVANNJA_INPUT_1) + i];
                }
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                else if (current_ekran.current_level == EKRAN_LN_FOR_IEC61850_RANG_SMALL)
                {
                  int prev_ekran = previous_level_in_current_level_menu[EKRAN_LN_FOR_IEC61850_RANG_SMALL];
                  if ((prev_ekran >= EKRAN_IN_GOOSE1) && (prev_ekran <= EKRAN_IN_MMS4))
                  {
                    int n_out_in = position_in_current_level_menu[prev_ekran];

                    prev_ekran = previous_level_in_current_level_menu[prev_ekran];
                    if (
                      (n_out_in >= 0) &&
                      ((prev_ekran >= EKRAN_LIST_IN_GOOSE) || (prev_ekran <= EKRAN_LIST_IN_MMS)))
                    {
                      n_LN = position_in_current_level_menu[prev_ekran];

                      prev_ekran = previous_level_in_current_level_menu[prev_ekran];
                      if (
                        (n_LN >= 0) &&
                        (prev_ekran == EKRAN_LIST_TYPE_IEC61850_NODES))
                      {
                        type_LN = position_in_current_level_menu[prev_ekran];

                        if (
                          (type_LN >= 0) &&
                          (((type_LN == INDEX_TYPE_IEC61850_IN_GOOSE) && (n_LN < N_IN_GOOSE) && (n_out_in < N_IN_GOOSE_MMS_OUT)) ||
                           ((type_LN == INDEX_TYPE_IEC61850_IN_MMS) && (n_LN < N_IN_MMS) && (n_out_in < N_IN_GOOSE_MMS_OUT))))
                        {
                          uint32_t *p_rang = NULL;
                          switch (type_LN)
                          {
                            case 0:
                              {
                                p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_In_GOOSE[n_LN][n_out_in] : edition_settings.ranguvannja_In_GOOSE[n_LN][n_out_in];
                                break;
                              }
                            case 1:
                              {
                                p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_In_MMS[n_LN][n_out_in] : edition_settings.ranguvannja_In_MMS[n_LN][n_out_in];
                                break;
                              }
                            default:
                              {
                                //Теоретично цього ніколи не мало б бути
                                total_error_sw_fixed();
                                break;
                              }
                          }

                          for (size_t i = 0; i < N_SMALL; i++)
                            temp_state[i] = p_rang[i];
                        }
                        else
                          total_error_sw_fixed();
                      }
                      else
                        total_error_sw_fixed();
                    }
                    else
                      total_error_sw_fixed();
                  }
                  else
                    total_error_sw_fixed();
                }
#endif

                if (current_ekran.index_position >= NUMBER_TOTAL_SIGNAL_FOR_RANG_SMALL)
                  current_ekran.index_position = 0;
                if (current_ekran.edition == 0)
                {

                  unsigned int comp = true;
                  for (size_t i = 0; ((comp == true) && (i < N_SMALL)); ++i)
                  {
                    comp &= (temp_state[i] == 0);
                  }
                  if (comp)
                    current_ekran.index_position = 0;
                  else
                  {
                    // (x>>5) аналогічне операції x / 32 - ціла частина від ділення на 32
                    // (x & 0x1f) аналогічне операції x % 32 - остача від ділення на 32
                    while ((temp_state[current_ekran.index_position >> 5] & (1 << (current_ekran.index_position & 0x1f))) == 0)
                    {
                      current_ekran.index_position++;
                      if (current_ekran.index_position >= NUMBER_TOTAL_SIGNAL_FOR_RANG_SMALL)
                        current_ekran.index_position = 0;
                    }
                  }
                }
                else
                {
                  unsigned int found_new_index = 0;
                  // int add_filter[0 + 1] =
                  //   {
                  //     -1 /*признак завершення масиву*/
                  //   };
                  //                EL_FILTER_STRUCT el_filter[NUMBER_DEFINED_ELEMENTS] =
                  //                {
                  //                  {1, RANG_SMALL_DF1_IN , RANG_SMALL_DF8_IN   , 1, current_settings.number_defined_df },
                  //                  {1, RANG_SMALL_DT1_SET, RANG_SMALL_DT4_RESET, 2, current_settings.number_defined_dt },
                  //                  {0, 0                 , 0                   , 1, current_settings.number_defined_and},
                  //                  {0, 0                 , 0                   , 1, current_settings.number_defined_or },
                  //                  {0, 0                 , 0                   , 1, current_settings.number_defined_xor},
                  //                  {0, 0                 , 0                   , 1, current_settings.number_defined_not}
                  //                };

                  //Перевіряємо, чи даний індекс функції присутній у даній конфігурації
                  while (found_new_index == 0)
                  {
                    check_current_index_is_presented_in_configuration(&found_new_index,
                                                                      /* add_filter */ NULL,
                                                                      /*el_filter,*/
                                                                      1,
                                                                      NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL,
                                                                      NUMBER_RPN_SIGNAL_FOR_RANG_SMALL,
                                                                      NUMBER_SZKh_SIGNAL_FOR_RANG_SMALL,
                                                                      NUMBER_SNKh_SIGNAL_FOR_RANG_SMALL,
                                                                      NUMBER_BRP_SIGNAL_FOR_RANG_SMALL,
                                                                      NUMBER_UMAX_SIGNAL_FOR_RANG_SMALL,
                                                                      NUMBER_UMIN_SIGNAL_FOR_RANG_SMALL,
                                                                      NUMBER_UP_SIGNAL_FOR_RANG_SMALL,
                                                                      NUMBER_EL_SIGNAL_FOR_RANG_SMALL);

                    //Перевіряємо режим рооботи функціональної кнопки (якщо іде редагування ФК)
                    if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_BUTTON_1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_BUTTON_1 + NUMBER_DEFINED_BUTTONS - 1)))
                    {
                      uint32_t mode = (current_settings.buttons_mode >> (current_ekran.current_level - EKRAN_RANGUVANNJA_BUTTON_1)) & 0x1;
                      if (_CHECK_SET_BIT(buttons_mode[mode], current_ekran.index_position) == 0)
                      {
                        found_new_index = 0;
                        current_ekran.index_position++;
                      }
                    }
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                    else if (current_ekran.current_level == EKRAN_LN_FOR_IEC61850_RANG_SMALL)
                    {
                      if ((type_LN == INDEX_TYPE_IEC61850_IN_GOOSE) || (type_LN == INDEX_TYPE_IEC61850_IN_MMS))
                      {
                        if (
                          (_CHECK_SET_BIT(rang_iec61850_blocks[type_LN], current_ekran.index_position) == 0) ||
                          ((type_LN == INDEX_TYPE_IEC61850_IN_GOOSE) && (current_ekran.index_position == (RANG_SMALL_BLOCK_IN_GOOSE1 + n_LN))) ||
                          ((type_LN == INDEX_TYPE_IEC61850_IN_MMS) && (current_ekran.index_position == (RANG_SMALL_BLOCK_IN_MMS1 + n_LN))))
                        {
                          found_new_index = 0;
                          current_ekran.index_position++;
                        }
                      }
                      else
                        total_error_sw_fixed();
                    }
#endif

                    //Перевіряємо, чи ми не вийшли за допустиму кількість функцій
                    if (current_ekran.index_position >= NUMBER_TOTAL_SIGNAL_FOR_RANG_SMALL)
                    {
                      found_new_index = 0;
                      current_ekran.index_position = 0;
                    }
                  }
                }
                position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;
                //Формуємо екран відображення зранжованих функцій
                if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_BUTTON_1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_BUTTON_1 + NUMBER_DEFINED_BUTTONS - 1)))
                  make_ekran_set_function_in_bi(current_ekran.current_level, INDEX_VIEWING_BUTTON, temp_state
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                                                ,
                                                type_LN, n_LN
#endif
                  );
                else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_INPUT_1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_INPUT_1 + NUMBER_INPUTS - 1)))
                  make_ekran_set_function_in_bi(current_ekran.current_level, INDEX_VIEWING_INPUT, temp_state
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                                                ,
                                                type_LN, n_LN
#endif
                  );
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                else if (current_ekran.current_level == EKRAN_LN_FOR_IEC61850_RANG_SMALL)
                  make_ekran_set_function_in_bi(current_ekran.current_level, INDEX_VIEWING_IEC61850_RANG, temp_state, type_LN, n_LN);
#endif
              }
              else if (
                ((current_ekran.current_level >= EKRAN_RANGUVANNJA_OUTPUT_1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_OUTPUT_1 + NUMBER_OUTPUTS - 1))) ||
                ((current_ekran.current_level >= EKRAN_RANGUVANNJA_LED_1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_LED_1 + NUMBER_LEDS - 1))) ||
                (current_ekran.current_level == EKRAN_RANGUVANNJA_ANALOG_REGISTRATOR) ||
                (current_ekran.current_level == EKRAN_RANGUVANNJA_DIGITAL_REGISTRATOR) ||
                (current_ekran.current_level == EKRAN_RANGUVANNJA_OFF_CB) ||
                (current_ekran.current_level == EKRAN_RANGUVANNJA_ON_CB) ||
                ((current_ekran.current_level >= EKRAN_RANGUVANNJA_DF1_PLUS) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_DF1_PLUS + MAX_ROW_LIST_TYPE_SOURCE_DF * NUMBER_DEFINED_FUNCTIONS - 1))) ||
                ((current_ekran.current_level >= EKRAN_RANGUVANNJA_SET_DT1_PLUS) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_SET_DT1_PLUS + 4 * NUMBER_DEFINED_TRIGGERS - 1))) ||
                ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_AND1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_AND1 + NUMBER_DEFINED_AND - 1))) ||
                ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_OR1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_OR1 + NUMBER_DEFINED_OR - 1))) ||
                ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_XOR1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_XOR1 + NUMBER_DEFINED_XOR - 1))) ||
                ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_NOT1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_NOT1 + NUMBER_DEFINED_NOT - 1)))
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                ||
                (current_ekran.current_level == EKRAN_LN_FOR_IEC61850_RANG)
#endif
              )
              {
                unsigned int temp_state[N_BIG];

                if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_OUTPUT_1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_OUTPUT_1 + NUMBER_OUTPUTS - 1)))
                {
                  uint32_t *p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_outputs : edition_settings.ranguvannja_outputs;
                  for (size_t i = 0; i < N_BIG; i++)
                    temp_state[i] = p_rang[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_OUTPUT_1) + i];
                }
                else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_LED_1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_LED_1 + NUMBER_LEDS - 1)))
                {
                  uint32_t *p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_leds : edition_settings.ranguvannja_leds;
                  for (size_t i = 0; i < N_BIG; i++)
                    temp_state[i] = p_rang[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_LED_1) + i];
                }
                else if (current_ekran.current_level == EKRAN_RANGUVANNJA_ANALOG_REGISTRATOR)
                {
                  uint32_t *p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_analog_registrator : edition_settings.ranguvannja_analog_registrator;
                  for (size_t i = 0; i < N_BIG; i++)
                    temp_state[i] = p_rang[i];
                }
                else if (current_ekran.current_level == EKRAN_RANGUVANNJA_DIGITAL_REGISTRATOR)
                {
                  uint32_t *p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_digital_registrator : edition_settings.ranguvannja_digital_registrator;
                  for (size_t i = 0; i < N_BIG; i++)
                    temp_state[i] = p_rang[i];
                }
                else if (current_ekran.current_level == EKRAN_RANGUVANNJA_OFF_CB)
                {
                  uint32_t *p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_off_cb : edition_settings.ranguvannja_off_cb;
                  for (size_t i = 0; i < N_BIG; i++)
                    temp_state[i] = p_rang[i];
                }
                else if (current_ekran.current_level == EKRAN_RANGUVANNJA_ON_CB)
                {
                  uint32_t *p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_on_cb : edition_settings.ranguvannja_on_cb;
                  for (size_t i = 0; i < N_BIG; i++)
                    temp_state[i] = p_rang[i];
                }
                else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_DF1_PLUS) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_DF1_PLUS + MAX_ROW_LIST_TYPE_SOURCE_DF * NUMBER_DEFINED_FUNCTIONS - 1)))
                {
                  unsigned int index_in_ekran_list = current_ekran.current_level - EKRAN_RANGUVANNJA_DF1_PLUS;
                  unsigned int type_source = index_in_ekran_list % MAX_ROW_LIST_TYPE_SOURCE_DF;
                  unsigned int index_of_df = index_in_ekran_list / MAX_ROW_LIST_TYPE_SOURCE_DF;

                  uint32_t *p_rang = NULL;
                  switch (type_source)
                  {
                    case INDEX_ML_LIST_TYPE_SOURCE_PLUS_DF:
                      {
                        p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_df_source_plus : edition_settings.ranguvannja_df_source_plus;
                        break;
                      }
                    case INDEX_ML_LIST_TYPE_SOURCE_MINUS_DF:
                      {
                        p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_df_source_minus : edition_settings.ranguvannja_df_source_minus;
                        break;
                      }
                    case INDEX_ML_LIST_TYPE_SOURCE_BLK_DF:
                      {
                        p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_df_source_blk : edition_settings.ranguvannja_df_source_blk;
                        break;
                      }
                    case INDEX_ML_LIST_TYPE_SOURCE_R_DF:
                      {
                        p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_df_source_reset : edition_settings.ranguvannja_df_source_reset;
                        break;
                      }
                    default:
                      {
                        total_error_sw_fixed();
                      }
                  }

                  for (size_t i = 0; i < N_BIG; i++)
                    temp_state[i] = p_rang[N_BIG * index_of_df + i];
                }
                else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_SET_DT1_PLUS) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_SET_DT1_PLUS + 4 * NUMBER_DEFINED_TRIGGERS - 1)))
                {
                  unsigned int index_in_ekran_list = current_ekran.current_level - EKRAN_RANGUVANNJA_SET_DT1_PLUS;
                  unsigned int type_source = index_in_ekran_list % 2;
                  unsigned int type_of_action = (index_in_ekran_list / 2) & 0x1;
                  unsigned int index_of_dt = index_in_ekran_list / 4;

                  uint32_t *p_rang = NULL;
                  switch (type_of_action)
                  {
                    case INDEX_ML_SET_DT:
                      {
                        switch (type_source)
                        {
                          case INDEX_ML_LIST_TYPE_SOURCE_PLUS_DT:
                            {
                              p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_set_dt_source_plus : edition_settings.ranguvannja_set_dt_source_plus;
                              break;
                            }
                          case INDEX_ML_LIST_TYPE_SOURCE_MINUS_DT:
                            {
                              p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_set_dt_source_minus : edition_settings.ranguvannja_set_dt_source_minus;
                              break;
                            }
                          default:
                            {
                              total_error_sw_fixed();
                            }
                        }
                        break;
                      }
                    case INDEX_ML_RESET_DT:
                      {
                        switch (type_source)
                        {
                          case INDEX_ML_LIST_TYPE_SOURCE_PLUS_DT:
                            {
                              p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_reset_dt_source_plus : edition_settings.ranguvannja_reset_dt_source_plus;
                              break;
                            }
                          case INDEX_ML_LIST_TYPE_SOURCE_MINUS_DT:
                            {
                              p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_reset_dt_source_minus : edition_settings.ranguvannja_reset_dt_source_minus;
                              break;
                            }
                          default:
                            {
                              total_error_sw_fixed();
                            }
                        }
                        break;
                      }
                    default:
                      {
                        total_error_sw_fixed();
                      }
                  }

                  for (size_t i = 0; i < N_BIG; i++)
                    temp_state[i] = p_rang[N_BIG * index_of_dt + i];
                }
                else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_AND1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_AND1 + NUMBER_DEFINED_AND - 1)))
                {
                  uint32_t *p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_d_and : edition_settings.ranguvannja_d_and;
                  for (size_t i = 0; i < N_BIG; i++)
                    temp_state[i] = p_rang[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_D_AND1) + i];
                }
                else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_OR1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_OR1 + NUMBER_DEFINED_OR - 1)))
                {
                  uint32_t *p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_d_or : edition_settings.ranguvannja_d_or;
                  for (size_t i = 0; i < N_BIG; i++)
                    temp_state[i] = p_rang[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_D_OR1) + i];
                }
                else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_XOR1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_XOR1 + NUMBER_DEFINED_XOR - 1)))
                {
                  uint32_t *p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_d_xor : edition_settings.ranguvannja_d_xor;
                  for (size_t i = 0; i < N_BIG; i++)
                    temp_state[i] = p_rang[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_D_XOR1) + i];
                }
                else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_NOT1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_NOT1 + NUMBER_DEFINED_NOT - 1)))
                {
                  uint32_t *p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_d_not : edition_settings.ranguvannja_d_not;
                  for (size_t i = 0; i < N_BIG; i++)
                    temp_state[i] = p_rang[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_D_NOT1) + i];
                }
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                else if (current_ekran.current_level == EKRAN_LN_FOR_IEC61850_RANG)
                {
                  int prev_ekran = previous_level_in_current_level_menu[EKRAN_LN_FOR_IEC61850_RANG];
                  if ((prev_ekran >= EKRAN_OUT_LAN1) && (prev_ekran <= EKRAN_OUT_LAN4))
                  {
                    int n_out_in = position_in_current_level_menu[prev_ekran];

                    prev_ekran = previous_level_in_current_level_menu[prev_ekran];
                    if (
                      (n_out_in >= 0) &&
                      (prev_ekran == EKRAN_LIST_OUT_LAN))
                    {
                      int n_LN = position_in_current_level_menu[prev_ekran];

                      if ((n_LN < N_OUT_LAN) && (n_out_in < N_OUT_LAN_IN))
                      {
                        if (current_ekran.edition == 0)
                        {
                          uint16_t *p_rang = current_settings.ranguvannja_Out_LAN[n_LN][n_out_in];

                          for (size_t i = 0; i < N_BIG; i++)
                            temp_state[i] = 0;
                          for (size_t i = 0; i < MAX_FUNCTIONS_IN_OUT_LAN; i++)
                          {
                            uint16_t val = p_rang[i];
                            if (val > 0)
                              _SET_BIT(temp_state, (p_rang[i] - 1));
                            else
                              break;
                          }
                        }
                        else
                        {
                          for (size_t i = 0; i < N_BIG; i++)
                            temp_state[i] = edit_rang_Out_LAN[i];
                        }
                      }
                      else
                        total_error_sw_fixed();
                    }
                    else
                      total_error_sw_fixed();
                  }
                  else
                    total_error_sw_fixed();
                }
#endif

                if (current_ekran.index_position >= NUMBER_TOTAL_SIGNAL_FOR_RANG)
                  current_ekran.index_position = 0;
                if (current_ekran.edition == 0)
                {
                  unsigned int comp = true;
                  for (size_t i = 0; ((comp == true) && (i < N_BIG)); ++i)
                  {
                    comp &= (temp_state[i] == 0);
                  }
                  if (comp)
                    current_ekran.index_position = 0;
                  else
                  {
                    // (x>>5) аналогічне операції x / 32 - ціла частина від ділення на 32
                    // (x & 0x1f) аналогічне операції x % 32 - остача від ділення на 32
                    while ((temp_state[current_ekran.index_position >> 5] & (1 << (current_ekran.index_position & 0x1f))) == 0)
                    {
                      current_ekran.index_position++;
                      if (current_ekran.index_position >= NUMBER_TOTAL_SIGNAL_FOR_RANG)
                        current_ekran.index_position = 0;
                    }
                  }
                }
                else
                {
                  unsigned int found_new_index = 0;
                  // int add_filter[3 + 1] =
                  //   {
                  //     RANG_PO_NZZ,
                  //     RANG_NZZ,
                  //     RANG_SECTOR_NZZ,
                  //     -1 /*признак завершення масиву*/
                  //   };
                  //                EL_FILTER_STRUCT el_filter[NUMBER_DEFINED_ELEMENTS] =
                  //                {
                  //                  {1, RANG_DF1_IN , RANG_DF8_OUT, 2, current_settings.number_defined_df },
                  //                  {1, RANG_DT1_SET, RANG_DT4_OUT, 3, current_settings.number_defined_dt },
                  //                  {1, RANG_D_AND1 , RANG_D_AND8 , 1, current_settings.number_defined_and},
                  //                  {1, RANG_D_OR1  , RANG_D_OR8  , 1, current_settings.number_defined_or },
                  //                  {1, RANG_D_XOR1 , RANG_D_XOR8 , 1, current_settings.number_defined_xor},
                  //                  {1, RANG_D_NOT1 , RANG_D_NOT16, 1, current_settings.number_defined_not}
                  //                };

                  //Перевіряємо, чи даний індекс функції присутній у даній конфігурації
                  while (found_new_index == 0)
                  {
                    check_current_index_is_presented_in_configuration(&found_new_index,
                                                                      /* add_filter */ NULL,
                                                                      /*el_filter,*/
                                                                      1,
                                                                      NUMBER_GENERAL_SIGNAL_FOR_RANG,
                                                                      NUMBER_RPN_SIGNAL_FOR_RANG,
                                                                      NUMBER_SZKh_SIGNAL_FOR_RANG,
                                                                      NUMBER_SNKh_SIGNAL_FOR_RANG,
                                                                      NUMBER_BRP_SIGNAL_FOR_RANG,
                                                                      NUMBER_UMAX_SIGNAL_FOR_RANG,
                                                                      NUMBER_UMIN_SIGNAL_FOR_RANG,
                                                                      NUMBER_UP_SIGNAL_FOR_RANG,
                                                                      NUMBER_EL_SIGNAL_FOR_RANG);

                    //Перевіряємо, чи ми не  на індексі функцій із списку загальних, яку треба викинути для даного типу ранжування
                    if (
                      (current_ekran.current_level == EKRAN_RANGUVANNJA_ANALOG_REGISTRATOR) ||
                      (current_ekran.current_level == EKRAN_RANGUVANNJA_DIGITAL_REGISTRATOR) ||
                      (current_ekran.current_level == EKRAN_RANGUVANNJA_OFF_CB) ||
                      (current_ekran.current_level == EKRAN_RANGUVANNJA_ON_CB))
                    {
                      size_t number = 1;
                      for (size_t num = 0; num < number; ++num)
                      {
                        unsigned int index_deleted_function = 0;

                        if (current_ekran.current_level == EKRAN_RANGUVANNJA_ANALOG_REGISTRATOR)
                          index_deleted_function = RANG_WORK_A_REJESTRATOR;
                        else if (current_ekran.current_level == EKRAN_RANGUVANNJA_DIGITAL_REGISTRATOR)
                          index_deleted_function = RANG_WORK_D_REJESTRATOR;
                        else if (current_ekran.current_level == EKRAN_RANGUVANNJA_OFF_CB)
                        {
                          number = 2;

                          if (num == 0)
                            index_deleted_function = RANG_VIDKL_VID_ZAKHYSTIV;
                          else
                            index_deleted_function = RANG_WORK_BO;
                        }
                        else if (current_ekran.current_level == EKRAN_RANGUVANNJA_ON_CB)
                          index_deleted_function = RANG_WORK_BV;

                        if (index_deleted_function == current_ekran.index_position)
                        {
                          found_new_index = 0;
                          current_ekran.index_position++;
                        }
                      }
                    }
                    else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_DF1_PLUS) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_DF1_PLUS + MAX_ROW_LIST_TYPE_SOURCE_DF * NUMBER_DEFINED_FUNCTIONS - 1)))
                    {
                      unsigned int index_in_ekran_list = current_ekran.current_level - EKRAN_RANGUVANNJA_DF1_PLUS;
                      unsigned int index_of_df = index_in_ekran_list / MAX_ROW_LIST_TYPE_SOURCE_DF;

                      for (unsigned int i = 0; i < 3; i++)
                      {
                        //Першою перевіряємо функцію з меншим номером, щоб за одну операцію циклу можна було переміститися на функцію, яку можна ранжувати
                        unsigned int const index_deleted_function = RANG_DF1_IN + 3 /*кількість сигналів*/ * index_of_df + i;

                        if (index_deleted_function == current_ekran.index_position)
                        {
                          found_new_index = 0;
                          current_ekran.index_position++;
                        }
                      }
                    }
                    else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_SET_DT1_PLUS) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_SET_DT1_PLUS + 4 * NUMBER_DEFINED_TRIGGERS - 1)))
                    {
                      unsigned int index_in_ekran_list = current_ekran.current_level - EKRAN_RANGUVANNJA_SET_DT1_PLUS;
                      unsigned int index_of_dt = index_in_ekran_list / 4;

                      for (unsigned int i = 0; i < 3; i++)
                      {
                        unsigned int index_deleted_function = 0;
                        //Першою перевіряємо функцію з меншим номером, щоб за одну операцію циклу  можна було переміститися на функцію, яку можна ранжувати
                        if (i == 0)
                        {
                          if (index_of_dt == 0)
                            index_deleted_function = RANG_DT1_SET;
                          else if (index_of_dt == 1)
                            index_deleted_function = RANG_DT2_SET;
                          else if (index_of_dt == 2)
                            index_deleted_function = RANG_DT3_SET;
                          else if (index_of_dt == 3)
                            index_deleted_function = RANG_DT4_SET;
                        }
                        else if (i == 1)
                        {
                          if (index_of_dt == 0)
                            index_deleted_function = RANG_DT1_RESET;
                          else if (index_of_dt == 1)
                            index_deleted_function = RANG_DT2_RESET;
                          else if (index_of_dt == 2)
                            index_deleted_function = RANG_DT3_RESET;
                          else if (index_of_dt == 3)
                            index_deleted_function = RANG_DT4_RESET;
                        }
                        else
                        {
                          if (index_of_dt == 0)
                            index_deleted_function = RANG_DT1_OUT;
                          else if (index_of_dt == 1)
                            index_deleted_function = RANG_DT2_OUT;
                          else if (index_of_dt == 2)
                            index_deleted_function = RANG_DT3_OUT;
                          else if (index_of_dt == 3)
                            index_deleted_function = RANG_DT4_OUT;
                        }

                        if (index_deleted_function == current_ekran.index_position)
                        {
                          found_new_index = 0;
                          current_ekran.index_position++;
                        }
                      }
                    }
                    else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_AND1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_AND1 + NUMBER_DEFINED_AND - 1)))
                    {
                      unsigned int index_of_d_and = current_ekran.current_level - EKRAN_RANGUVANNJA_D_AND1;

                      //Першою перевіряємо функцію з меншим номером, щоб за одну операцію циклу  можна було переміститися на функцію, яку можна ранжувати
                      unsigned int index_deleted_function = RANG_D_AND1 + index_of_d_and;

                      if (index_deleted_function == current_ekran.index_position)
                      {
                        found_new_index = 0;
                        current_ekran.index_position++;
                      }
                    }
                    else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_OR1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_OR1 + NUMBER_DEFINED_OR - 1)))
                    {
                      unsigned int index_of_d_or = current_ekran.current_level - EKRAN_RANGUVANNJA_D_OR1;

                      //Першою перевіряємо функцію з меншим номером, щоб за одну операцію циклу  можна було переміститися на функцію, яку можна ранжувати
                      unsigned int index_deleted_function = RANG_D_OR1 + index_of_d_or;

                      if (index_deleted_function == current_ekran.index_position)
                      {
                        found_new_index = 0;
                        current_ekran.index_position++;
                      }
                    }
                    else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_XOR1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_XOR1 + NUMBER_DEFINED_XOR - 1)))
                    {
                      unsigned int index_of_d_xor = current_ekran.current_level - EKRAN_RANGUVANNJA_D_XOR1;

                      //Першою перевіряємо функцію з меншим номером, щоб за одну операцію циклу  можна було переміститися на функцію, яку можна ранжувати
                      unsigned int index_deleted_function = RANG_D_XOR1 + index_of_d_xor;

                      if (index_deleted_function == current_ekran.index_position)
                      {
                        found_new_index = 0;
                        current_ekran.index_position++;
                      }
                    }
                    else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_NOT1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_NOT1 + NUMBER_DEFINED_NOT - 1)))
                    {
                      unsigned int index_of_d_not = current_ekran.current_level - EKRAN_RANGUVANNJA_D_NOT1;

                      //Першою перевіряємо функцію з меншим номером, щоб за одну операцію циклу  можна було переміститися на функцію, яку можна ранжувати
                      unsigned int index_deleted_function = RANG_D_NOT1 + index_of_d_not;

                      if (index_deleted_function == current_ekran.index_position)
                      {
                        found_new_index = 0;
                        current_ekran.index_position++;
                      }
                    }

                    //Перевіряємо, чи ми не вийшли за допустиму кількість функцій
                    if (current_ekran.index_position >= NUMBER_TOTAL_SIGNAL_FOR_RANG)
                    {
                      found_new_index = 0;
                      current_ekran.index_position = 0;
                    }
                  }
                }
                position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;
                //Формуємо екран відображення зранжованих функцій
                if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_OUTPUT_1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_OUTPUT_1 + NUMBER_OUTPUTS - 1)))
                  make_ekran_set_function_in_output_led_df_dt_reg(current_ekran.current_level, INDEX_VIEWING_OUTPUT, temp_state);
                else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_LED_1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_LED_1 + NUMBER_LEDS - 1)))
                  make_ekran_set_function_in_output_led_df_dt_reg(current_ekran.current_level, INDEX_VIEWING_LED, temp_state);
                else if (current_ekran.current_level == EKRAN_RANGUVANNJA_ANALOG_REGISTRATOR)
                  make_ekran_set_function_in_output_led_df_dt_reg(current_ekran.current_level, INDEX_VIEWING_A_REG, temp_state);
                else if (current_ekran.current_level == EKRAN_RANGUVANNJA_DIGITAL_REGISTRATOR)
                  make_ekran_set_function_in_output_led_df_dt_reg(current_ekran.current_level, INDEX_VIEWING_D_REG, temp_state);
                else if (current_ekran.current_level == EKRAN_RANGUVANNJA_OFF_CB)
                  make_ekran_set_function_in_output_led_df_dt_reg(current_ekran.current_level, INDEX_VIEWING_OFF_CB, temp_state);
                else if (current_ekran.current_level == EKRAN_RANGUVANNJA_ON_CB)
                  make_ekran_set_function_in_output_led_df_dt_reg(current_ekran.current_level, INDEX_VIEWING_ON_CB, temp_state);
                else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_DF1_PLUS) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_DF1_PLUS + MAX_ROW_LIST_TYPE_SOURCE_DF * NUMBER_DEFINED_FUNCTIONS - 1)))
                  make_ekran_set_function_in_output_led_df_dt_reg(current_ekran.current_level, INDEX_VIEWING_DF, temp_state);
                else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_SET_DT1_PLUS) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_SET_DT1_PLUS + 4 * NUMBER_DEFINED_TRIGGERS - 1)))
                  make_ekran_set_function_in_output_led_df_dt_reg(current_ekran.current_level, INDEX_VIEWING_DT, temp_state);
                else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_AND1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_AND1 + NUMBER_DEFINED_AND - 1)))
                  make_ekran_set_function_in_output_led_df_dt_reg(current_ekran.current_level, INDEX_VIEWING_D_AND, temp_state);
                else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_OR1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_OR1 + NUMBER_DEFINED_OR - 1)))
                  make_ekran_set_function_in_output_led_df_dt_reg(current_ekran.current_level, INDEX_VIEWING_D_OR, temp_state);
                else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_XOR1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_XOR1 + NUMBER_DEFINED_XOR - 1)))
                  make_ekran_set_function_in_output_led_df_dt_reg(current_ekran.current_level, INDEX_VIEWING_D_XOR, temp_state);
                else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_NOT1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_NOT1 + NUMBER_DEFINED_NOT - 1)))
                  make_ekran_set_function_in_output_led_df_dt_reg(current_ekran.current_level, INDEX_VIEWING_D_NOT, temp_state);
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                else if (current_ekran.current_level == EKRAN_LN_FOR_IEC61850_RANG)
                  make_ekran_set_function_in_output_led_df_dt_reg(current_ekran.current_level, INDEX_VIEWING_IEC61850_RANG, temp_state);
#endif
              }

              //Очищаємо біт обновлення екрану
              new_state_keyboard &= ~(1u << BIT_REWRITE);
            }
            else
            {
              if (new_state_keyboard == (1u << BIT_KEY_ENTER))
              {
                //Натиснута кнопка ENTER
                if (current_ekran.edition == 0)
                {
                  //Копіюємо ранжування у структуру для редагування
                  if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_INPUT_1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_INPUT_1 + NUMBER_INPUTS - 1)))
                  {
                    for (unsigned int i = 0; i < N_SMALL; i++)
                    {
                      edition_settings.ranguvannja_inputs[N_SMALL * (current_ekran.current_level - EKRAN_RANGUVANNJA_INPUT_1) + i] =
                        current_settings.ranguvannja_inputs[N_SMALL * (current_ekran.current_level - EKRAN_RANGUVANNJA_INPUT_1) + i];
                    }
                  }
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_BUTTON_1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_BUTTON_1 + NUMBER_DEFINED_BUTTONS - 1)))
                  {
                    for (unsigned int i = 0; i < N_SMALL; i++)
                    {
                      edition_settings.ranguvannja_buttons[N_SMALL * (current_ekran.current_level - EKRAN_RANGUVANNJA_BUTTON_1) + i] =
                        current_settings.ranguvannja_buttons[N_SMALL * (current_ekran.current_level - EKRAN_RANGUVANNJA_BUTTON_1) + i];
                    }
                  }
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                  else if (current_ekran.current_level == EKRAN_LN_FOR_IEC61850_RANG_SMALL)
                  {
                    int prev_ekran = previous_level_in_current_level_menu[EKRAN_LN_FOR_IEC61850_RANG_SMALL];
                    if ((prev_ekran >= EKRAN_IN_GOOSE1) && (prev_ekran <= EKRAN_IN_MMS4))
                    {
                      int n_out_in = position_in_current_level_menu[prev_ekran];

                      prev_ekran = previous_level_in_current_level_menu[prev_ekran];
                      if (
                        (n_out_in >= 0) &&
                        ((prev_ekran >= EKRAN_LIST_IN_GOOSE) || (prev_ekran <= EKRAN_LIST_IN_MMS)))
                      {
                        int n_LN = position_in_current_level_menu[prev_ekran];

                        prev_ekran = previous_level_in_current_level_menu[prev_ekran];
                        if (
                          (n_LN >= 0) &&
                          (prev_ekran == EKRAN_LIST_TYPE_IEC61850_NODES))
                        {
                          int type_LN = position_in_current_level_menu[prev_ekran];

                          if (
                            (type_LN >= 0) &&
                            (((type_LN == INDEX_TYPE_IEC61850_IN_GOOSE) && (n_LN < N_IN_GOOSE) && (n_out_in < N_IN_GOOSE_MMS_OUT)) ||
                             ((type_LN == INDEX_TYPE_IEC61850_IN_MMS) && (n_LN < N_IN_MMS) && (n_out_in < N_IN_GOOSE_MMS_OUT))))
                          {
                            uint32_t *p_rang = NULL, *p_rang_edit = NULL;
                            switch (type_LN)
                            {
                              case 0:
                                {
                                  p_rang = current_settings.ranguvannja_In_GOOSE[n_LN][n_out_in];
                                  p_rang_edit = edition_settings.ranguvannja_In_GOOSE[n_LN][n_out_in];
                                  break;
                                }
                              case 1:
                                {
                                  p_rang = current_settings.ranguvannja_In_MMS[n_LN][n_out_in];
                                  p_rang_edit = edition_settings.ranguvannja_In_MMS[n_LN][n_out_in];
                                  break;
                                }
                              default:
                                {
                                  //Теоретично цього ніколи не мало б бути
                                  total_error_sw_fixed();
                                  break;
                                }
                            }

                            for (unsigned int i = 0; i < N_SMALL; i++)
                              p_rang_edit[i] = p_rang[i];
                          }
                          else
                            total_error_sw_fixed();
                        }
                        else
                          total_error_sw_fixed();
                      }
                      else
                        total_error_sw_fixed();
                    }
                    else
                      total_error_sw_fixed();
                  }
                  else if (current_ekran.current_level == EKRAN_LN_FOR_IEC61850_RANG)
                  {
                    int prev_ekran = previous_level_in_current_level_menu[EKRAN_LN_FOR_IEC61850_RANG];
                    if ((prev_ekran >= EKRAN_OUT_LAN1) && (prev_ekran <= EKRAN_OUT_LAN4))
                    {
                      int n_out_in = position_in_current_level_menu[prev_ekran];

                      prev_ekran = previous_level_in_current_level_menu[prev_ekran];
                      if (
                        (n_out_in >= 0) &&
                        (prev_ekran == EKRAN_LIST_OUT_LAN))
                      {
                        int n_LN = position_in_current_level_menu[prev_ekran];

                        if ((n_LN < N_OUT_LAN) && (n_out_in < N_OUT_LAN_IN))
                        {
                          for (size_t i = 0; i < N_BIG; i++)
                            edit_rang_Out_LAN[i] = 0;

                          uint16_t *p_rang = current_settings.ranguvannja_Out_LAN[n_LN][n_out_in];
                          for (size_t i = 0; i < MAX_FUNCTIONS_IN_OUT_LAN; i++)
                          {
                            uint16_t val = p_rang[i];
                            if (val > 0)
                              _SET_BIT(edit_rang_Out_LAN, (p_rang[i] - 1));
                            else
                              break;
                          }
                        }
                        else
                          total_error_sw_fixed();
                      }
                      else
                        total_error_sw_fixed();
                    }
                    else
                      total_error_sw_fixed();
                  }
#endif
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_OUTPUT_1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_OUTPUT_1 + NUMBER_OUTPUTS - 1)))
                  {
                    for (unsigned int i = 0; i < N_BIG; i++)
                    {
                      edition_settings.ranguvannja_outputs[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_OUTPUT_1) + i] =
                        current_settings.ranguvannja_outputs[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_OUTPUT_1) + i];
                    }
                  }
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_LED_1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_LED_1 + NUMBER_LEDS - 1)))
                  {
                    for (unsigned int i = 0; i < N_BIG; i++)
                    {
                      edition_settings.ranguvannja_leds[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_LED_1) + i] =
                        current_settings.ranguvannja_leds[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_LED_1) + i];
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_RANGUVANNJA_ANALOG_REGISTRATOR)
                  {
                    for (unsigned int i = 0; i < N_BIG; i++)
                    {
                      edition_settings.ranguvannja_analog_registrator[i] = current_settings.ranguvannja_analog_registrator[i];
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_RANGUVANNJA_DIGITAL_REGISTRATOR)
                  {
                    for (unsigned int i = 0; i < N_BIG; i++)
                    {
                      edition_settings.ranguvannja_digital_registrator[i] = current_settings.ranguvannja_digital_registrator[i];
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_RANGUVANNJA_OFF_CB)
                  {
                    for (unsigned int i = 0; i < N_BIG; i++)
                    {
                      edition_settings.ranguvannja_off_cb[i] = current_settings.ranguvannja_off_cb[i];
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_RANGUVANNJA_ON_CB)
                  {
                    for (unsigned int i = 0; i < N_BIG; i++)
                    {
                      edition_settings.ranguvannja_on_cb[i] = current_settings.ranguvannja_on_cb[i];
                    }
                  }
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_DF1_PLUS) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_DF1_PLUS + MAX_ROW_LIST_TYPE_SOURCE_DF * NUMBER_DEFINED_FUNCTIONS - 1)))
                  {
                    unsigned int index_in_ekran_list = current_ekran.current_level - EKRAN_RANGUVANNJA_DF1_PLUS;
                    unsigned int type_source = index_in_ekran_list % MAX_ROW_LIST_TYPE_SOURCE_DF;
                    unsigned int index_of_df = index_in_ekran_list / MAX_ROW_LIST_TYPE_SOURCE_DF;

                    if (type_source == INDEX_ML_LIST_TYPE_SOURCE_PLUS_DF)
                    {
                      for (unsigned int i = 0; i < N_BIG; i++)
                      {
                        edition_settings.ranguvannja_df_source_plus[N_BIG * index_of_df + i] = current_settings.ranguvannja_df_source_plus[N_BIG * index_of_df + i];
                      }
                    }
                    else if (type_source == INDEX_ML_LIST_TYPE_SOURCE_MINUS_DF)
                    {
                      for (unsigned int i = 0; i < N_BIG; i++)
                      {
                        edition_settings.ranguvannja_df_source_minus[N_BIG * index_of_df + i] = current_settings.ranguvannja_df_source_minus[N_BIG * index_of_df + i];
                      }
                    }
                    else if (type_source == INDEX_ML_LIST_TYPE_SOURCE_BLK_DF)
                    {
                      for (unsigned int i = 0; i < N_BIG; i++)
                      {
                        edition_settings.ranguvannja_df_source_blk[N_BIG * index_of_df + i] = current_settings.ranguvannja_df_source_blk[N_BIG * index_of_df + i];
                      }
                    }
                    else if (type_source == INDEX_ML_LIST_TYPE_SOURCE_R_DF)
                    {
                      for (unsigned int i = 0; i < N_BIG; i++)
                      {
                        edition_settings.ranguvannja_df_source_reset[N_BIG * index_of_df + i] = current_settings.ranguvannja_df_source_reset[N_BIG * index_of_df + i];
                      }
                    }
                    else
                      total_error_sw_fixed();
                  }
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_SET_DT1_PLUS) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_SET_DT1_PLUS + 4 * NUMBER_DEFINED_TRIGGERS - 1)))
                  {
                    unsigned int index_in_ekran_list = current_ekran.current_level - EKRAN_RANGUVANNJA_SET_DT1_PLUS;
                    unsigned int type_source = index_in_ekran_list % 2;
                    unsigned int type_of_action = (index_in_ekran_list / 2) & 0x1;
                    unsigned int index_of_dt = index_in_ekran_list / 4;

                    if (type_of_action == INDEX_ML_SET_DT)
                    {
                      if (type_source == INDEX_ML_LIST_TYPE_SOURCE_PLUS_DT)
                      {
                        for (unsigned int i = 0; i < N_BIG; i++)
                        {
                          edition_settings.ranguvannja_set_dt_source_plus[N_BIG * index_of_dt + i] = current_settings.ranguvannja_set_dt_source_plus[N_BIG * index_of_dt + i];
                        }
                      }
                      else
                      {
                        for (unsigned int i = 0; i < N_BIG; i++)
                        {
                          edition_settings.ranguvannja_set_dt_source_minus[N_BIG * index_of_dt + i] = current_settings.ranguvannja_set_dt_source_minus[N_BIG * index_of_dt + i];
                        }
                      }
                    }
                    else
                    {
                      if (type_source == INDEX_ML_LIST_TYPE_SOURCE_PLUS_DT)
                      {
                        for (unsigned int i = 0; i < N_BIG; i++)
                        {
                          edition_settings.ranguvannja_reset_dt_source_plus[N_BIG * index_of_dt + i] = current_settings.ranguvannja_reset_dt_source_plus[N_BIG * index_of_dt + i];
                        }
                      }
                      else
                      {
                        for (unsigned int i = 0; i < N_BIG; i++)
                        {
                          edition_settings.ranguvannja_reset_dt_source_minus[N_BIG * index_of_dt + i] = current_settings.ranguvannja_reset_dt_source_minus[N_BIG * index_of_dt + i];
                        }
                      }
                    }
                  }
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_AND1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_AND1 + NUMBER_DEFINED_AND - 1)))
                  {
                    for (unsigned int i = 0; i < N_BIG; i++)
                    {
                      edition_settings.ranguvannja_d_and[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_D_AND1) + i] =
                        current_settings.ranguvannja_d_and[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_D_AND1) + i];
                    }
                  }
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_OR1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_OR1 + NUMBER_DEFINED_OR - 1)))
                  {
                    for (unsigned int i = 0; i < N_BIG; i++)
                    {
                      edition_settings.ranguvannja_d_or[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_D_OR1) + i] =
                        current_settings.ranguvannja_d_or[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_D_OR1) + i];
                    }
                  }
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_XOR1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_XOR1 + NUMBER_DEFINED_XOR - 1)))
                  {
                    for (unsigned int i = 0; i < N_BIG; i++)
                    {
                      edition_settings.ranguvannja_d_xor[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_D_XOR1) + i] =
                        current_settings.ranguvannja_d_xor[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_D_XOR1) + i];
                    }
                  }
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_NOT1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_NOT1 + NUMBER_DEFINED_NOT - 1)))
                  {
                    for (unsigned int i = 0; i < N_BIG; i++)
                    {
                      edition_settings.ranguvannja_d_not[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_D_NOT1) + i] =
                        current_settings.ranguvannja_d_not[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_D_NOT1) + i];
                    }
                  }

                  //Підготовка до режиму редагування - включаємо мигаючий курсор
                  int temp_current_level = current_ekran.current_level;
                  current_ekran.cursor_on = 1;
                  current_ekran.cursor_blinking_on = 1;
                  if (current_settings.password1 != 0)
                  {
                    //Переходимо на меню запиту паролю
                    current_ekran.current_level = EKRAN_LEVEL_PASSWORD;
                    previous_level_in_current_level_menu[current_ekran.current_level] = temp_current_level;
                    current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                  }
                  else
                  {
                    //Переходимо у режим редагування
                    current_ekran.edition = 1;
                  }
                }
                else if (current_ekran.edition == 1)
                {
                  //Перевіряємо чи якісь зміни відбулися
                  if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_INPUT_1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_INPUT_1 + NUMBER_INPUTS - 1)))
                  {
                    unsigned int comp = true;
                    for (size_t i = 0; ((comp == true) && (i < N_SMALL)); ++i)
                    {
                      comp &= (edition_settings.ranguvannja_inputs[N_SMALL * (current_ekran.current_level - EKRAN_RANGUVANNJA_INPUT_1) + i] == current_settings.ranguvannja_inputs[N_SMALL * (current_ekran.current_level - EKRAN_RANGUVANNJA_INPUT_1) + i]);
                    }

                    if (comp)
                      current_ekran.edition = 0;
                    else
                      current_ekran.edition = 2;
                  }
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_BUTTON_1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_BUTTON_1 + NUMBER_DEFINED_BUTTONS - 1)))
                  {
                    unsigned int comp = true;
                    for (size_t i = 0; ((comp == true) && (i < N_SMALL)); ++i)
                    {
                      comp &= (edition_settings.ranguvannja_buttons[N_SMALL * (current_ekran.current_level - EKRAN_RANGUVANNJA_BUTTON_1) + i] == current_settings.ranguvannja_buttons[N_SMALL * (current_ekran.current_level - EKRAN_RANGUVANNJA_BUTTON_1) + i]);
                    }

                    if (comp)
                      current_ekran.edition = 0;
                    else
                      current_ekran.edition = 2;
                  }
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                  else if (current_ekran.current_level == EKRAN_LN_FOR_IEC61850_RANG_SMALL)
                  {
                    int prev_ekran = previous_level_in_current_level_menu[EKRAN_LN_FOR_IEC61850_RANG_SMALL];
                    if ((prev_ekran >= EKRAN_IN_GOOSE1) && (prev_ekran <= EKRAN_IN_MMS4))
                    {
                      int n_out_in = position_in_current_level_menu[prev_ekran];

                      prev_ekran = previous_level_in_current_level_menu[prev_ekran];
                      if (
                        (n_out_in >= 0) &&
                        ((prev_ekran >= EKRAN_LIST_IN_GOOSE) || (prev_ekran <= EKRAN_LIST_IN_MMS)))
                      {
                        int n_LN = position_in_current_level_menu[prev_ekran];

                        prev_ekran = previous_level_in_current_level_menu[prev_ekran];
                        if (
                          (n_LN >= 0) &&
                          (prev_ekran == EKRAN_LIST_TYPE_IEC61850_NODES))
                        {
                          int type_LN = position_in_current_level_menu[prev_ekran];

                          if (
                            (type_LN >= 0) &&
                            (((type_LN == INDEX_TYPE_IEC61850_IN_GOOSE) && (n_LN < N_IN_GOOSE) && (n_out_in < N_IN_GOOSE_MMS_OUT)) ||
                             ((type_LN == INDEX_TYPE_IEC61850_IN_MMS) && (n_LN < N_IN_MMS) && (n_out_in < N_IN_GOOSE_MMS_OUT))))
                          {
                            uint32_t *p_rang = NULL, *p_rang_edit = NULL;
                            switch (type_LN)
                            {
                              case 0:
                                {
                                  p_rang = current_settings.ranguvannja_In_GOOSE[n_LN][n_out_in];
                                  p_rang_edit = edition_settings.ranguvannja_In_GOOSE[n_LN][n_out_in];
                                  break;
                                }
                              case 1:
                                {
                                  p_rang = current_settings.ranguvannja_In_MMS[n_LN][n_out_in];
                                  p_rang_edit = edition_settings.ranguvannja_In_MMS[n_LN][n_out_in];
                                  break;
                                }
                              default:
                                {
                                  //Теоретично цього ніколи не мало б бути
                                  total_error_sw_fixed();
                                  break;
                                }
                            }

                            unsigned int comp = true;
                            for (size_t i = 0; ((comp == true) && (i < N_SMALL)); ++i)
                            {
                              comp &= (p_rang_edit[i] == p_rang[i]);
                            }

                            if (comp)
                              current_ekran.edition = 0;
                            else
                              current_ekran.edition = 2;
                          }
                          else
                            total_error_sw_fixed();
                        }
                        else
                          total_error_sw_fixed();
                      }
                      else
                        total_error_sw_fixed();
                    }
                    else
                      total_error_sw_fixed();
                  }
                  else if (current_ekran.current_level == EKRAN_LN_FOR_IEC61850_RANG)
                  {
                    int prev_ekran = previous_level_in_current_level_menu[EKRAN_LN_FOR_IEC61850_RANG];
                    if ((prev_ekran >= EKRAN_OUT_LAN1) && (prev_ekran <= EKRAN_OUT_LAN4))
                    {
                      int n_out_in = position_in_current_level_menu[prev_ekran];

                      prev_ekran = previous_level_in_current_level_menu[prev_ekran];
                      if (
                        (n_out_in >= 0) &&
                        (prev_ekran == EKRAN_LIST_OUT_LAN))
                      {
                        int n_LN = position_in_current_level_menu[prev_ekran];

                        if ((n_LN < N_OUT_LAN) && (n_out_in < N_OUT_LAN_IN))
                        {
                          unsigned int temp_state[N_BIG];
                          for (size_t i = 0; i < N_BIG; i++)
                            temp_state[i] = 0;

                          uint16_t *p_rang = current_settings.ranguvannja_Out_LAN[n_LN][n_out_in];
                          for (size_t i = 0; i < MAX_FUNCTIONS_IN_OUT_LAN; i++)
                          {
                            uint16_t val = p_rang[i];
                            if (val > 0)
                              _SET_BIT(temp_state, (p_rang[i] - 1));
                            else
                              break;
                          }

                          unsigned int comp = true;
                          for (size_t i = 0; ((comp == true) && (i < N_BIG)); ++i)
                          {
                            comp &= (edit_rang_Out_LAN[i] == temp_state[i]);
                          }
                          if (comp == true)
                          {
                            current_ekran.edition = 0;
                          }
                          else
                            current_ekran.edition = 2;
                        }
                        else
                          total_error_sw_fixed();
                      }
                      else
                        total_error_sw_fixed();
                    }
                    else
                      total_error_sw_fixed();
                  }
#endif
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_OUTPUT_1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_OUTPUT_1 + NUMBER_OUTPUTS - 1)))
                  {
                    unsigned int comp = true;
                    for (size_t i = 0; ((comp == true) && (i < N_BIG)); ++i)
                    {
                      comp &= (edition_settings.ranguvannja_outputs[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_OUTPUT_1) + i] == current_settings.ranguvannja_outputs[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_OUTPUT_1) + i]);
                    }
                    if (comp == true)
                      current_ekran.edition = 0;
                    else
                      current_ekran.edition = 2;
                  }
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_LED_1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_LED_1 + NUMBER_LEDS - 1)))
                  {
                    unsigned int comp = true;
                    for (size_t i = 0; ((comp == true) && (i < N_BIG)); ++i)
                    {
                      comp &= (edition_settings.ranguvannja_leds[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_LED_1) + i] == current_settings.ranguvannja_leds[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_LED_1) + i]);
                    }
                    if (comp == true)
                      current_ekran.edition = 0;
                    else
                      current_ekran.edition = 2;
                  }
                  else if (current_ekran.current_level == EKRAN_RANGUVANNJA_ANALOG_REGISTRATOR)
                  {
                    unsigned int comp = true;
                    for (size_t i = 0; ((comp == true) && (i < N_BIG)); ++i)
                    {
                      comp &= (edition_settings.ranguvannja_analog_registrator[i] == current_settings.ranguvannja_analog_registrator[i]);
                    }
                    if (comp == true)
                      current_ekran.edition = 0;
                    else
                      current_ekran.edition = 2;
                  }
                  else if (current_ekran.current_level == EKRAN_RANGUVANNJA_DIGITAL_REGISTRATOR)
                  {
                    unsigned int comp = true;
                    for (size_t i = 0; ((comp == true) && (i < N_BIG)); ++i)
                    {
                      comp &= (edition_settings.ranguvannja_digital_registrator[i] == current_settings.ranguvannja_digital_registrator[i]);
                    }
                    if (comp == true)
                      current_ekran.edition = 0;
                    else
                      current_ekran.edition = 2;
                  }
                  else if (current_ekran.current_level == EKRAN_RANGUVANNJA_OFF_CB)
                  {
                    unsigned int comp = true;
                    for (size_t i = 0; ((comp == true) && (i < N_BIG)); ++i)
                    {
                      comp &= (edition_settings.ranguvannja_off_cb[i] == current_settings.ranguvannja_off_cb[i]);
                    }
                    if (comp == true)
                      current_ekran.edition = 0;
                    else
                      current_ekran.edition = 2;
                  }
                  else if (current_ekran.current_level == EKRAN_RANGUVANNJA_ON_CB)
                  {
                    unsigned int comp = true;
                    for (size_t i = 0; ((comp == true) && (i < N_BIG)); ++i)
                    {
                      comp &= (edition_settings.ranguvannja_on_cb[i] == current_settings.ranguvannja_on_cb[i]);
                    }
                    if (comp == true)
                      current_ekran.edition = 0;
                    else
                      current_ekran.edition = 2;
                  }
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_DF1_PLUS) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_DF1_PLUS + MAX_ROW_LIST_TYPE_SOURCE_DF * NUMBER_DEFINED_FUNCTIONS - 1)))
                  {
                    unsigned int index_in_ekran_list = current_ekran.current_level - EKRAN_RANGUVANNJA_DF1_PLUS;
                    unsigned int type_source = index_in_ekran_list % MAX_ROW_LIST_TYPE_SOURCE_DF;
                    unsigned int index_of_df = index_in_ekran_list / MAX_ROW_LIST_TYPE_SOURCE_DF;

                    if (type_source == INDEX_ML_LIST_TYPE_SOURCE_PLUS_DF)
                    {
                      unsigned int comp = true;
                      for (size_t i = 0; ((comp == true) && (i < N_BIG)); ++i)
                      {
                        comp &= (edition_settings.ranguvannja_df_source_plus[N_BIG * index_of_df + i] == current_settings.ranguvannja_df_source_plus[N_BIG * index_of_df + i]);
                      }
                      if (comp == true)
                        current_ekran.edition = 0;
                      else
                        current_ekran.edition = 2;
                    }
                    else if (type_source == INDEX_ML_LIST_TYPE_SOURCE_MINUS_DF)
                    {
                      unsigned int comp = true;
                      for (size_t i = 0; ((comp == true) && (i < N_BIG)); ++i)
                      {
                        comp &= (edition_settings.ranguvannja_df_source_minus[N_BIG * index_of_df + i] == current_settings.ranguvannja_df_source_minus[N_BIG * index_of_df + i]);
                      }
                      if (comp == true)
                        current_ekran.edition = 0;
                      else
                        current_ekran.edition = 2;
                    }
                    else if (type_source == INDEX_ML_LIST_TYPE_SOURCE_BLK_DF)
                    {
                      unsigned int comp = true;
                      for (size_t i = 0; ((comp == true) && (i < N_BIG)); ++i)
                      {
                        comp &= (edition_settings.ranguvannja_df_source_blk[N_BIG * index_of_df + i] == current_settings.ranguvannja_df_source_blk[N_BIG * index_of_df + i]);
                      }
                      if (comp == true)
                        current_ekran.edition = 0;
                      else
                        current_ekran.edition = 2;
                    }
                    else if (type_source == INDEX_ML_LIST_TYPE_SOURCE_R_DF)
                    {
                      unsigned int comp = true;
                      for (size_t i = 0; ((comp == true) && (i < N_BIG)); ++i)
                      {
                        comp &= (edition_settings.ranguvannja_df_source_reset[N_BIG * index_of_df + i] == current_settings.ranguvannja_df_source_reset[N_BIG * index_of_df + i]);
                      }
                      if (comp == true)
                        current_ekran.edition = 0;
                      else
                        current_ekran.edition = 2;
                    }
                    else
                      total_error_sw_fixed();
                  }
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_SET_DT1_PLUS) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_SET_DT1_PLUS + 4 * NUMBER_DEFINED_TRIGGERS - 1)))
                  {
                    unsigned int index_in_ekran_list = current_ekran.current_level - EKRAN_RANGUVANNJA_SET_DT1_PLUS;
                    unsigned int type_source = index_in_ekran_list % 2;
                    unsigned int type_of_action = (index_in_ekran_list / 2) & 0x1;
                    unsigned int index_of_dt = index_in_ekran_list / 4;

                    if (type_of_action == INDEX_ML_SET_DT)
                    {
                      if (type_source == INDEX_ML_LIST_TYPE_SOURCE_PLUS_DT)
                      {
                        unsigned int comp = true;
                        for (size_t i = 0; ((comp == true) && (i < N_BIG)); ++i)
                        {
                          comp &= (edition_settings.ranguvannja_set_dt_source_plus[N_BIG * index_of_dt + i] == current_settings.ranguvannja_set_dt_source_plus[N_BIG * index_of_dt + i]);
                        }
                        if (comp == true)
                          current_ekran.edition = 0;
                        else
                          current_ekran.edition = 2;
                      }
                      else
                      {
                        unsigned int comp = true;
                        for (size_t i = 0; ((comp == true) && (i < N_BIG)); ++i)
                        {
                          comp &= (edition_settings.ranguvannja_set_dt_source_minus[N_BIG * index_of_dt + i] == current_settings.ranguvannja_set_dt_source_minus[N_BIG * index_of_dt + i]);
                        }
                        if (comp == true)
                          current_ekran.edition = 0;
                        else
                          current_ekran.edition = 2;
                      }
                    }
                    else
                    {
                      if (type_source == INDEX_ML_LIST_TYPE_SOURCE_PLUS_DT)
                      {
                        unsigned int comp = true;
                        for (size_t i = 0; ((comp == true) && (i < N_BIG)); ++i)
                        {
                          comp &= (edition_settings.ranguvannja_reset_dt_source_plus[N_BIG * index_of_dt + i] == current_settings.ranguvannja_reset_dt_source_plus[N_BIG * index_of_dt + i]);
                        }
                        if (comp == true)
                          current_ekran.edition = 0;
                        else
                          current_ekran.edition = 2;
                      }
                      else
                      {
                        unsigned int comp = true;
                        for (size_t i = 0; ((comp == true) && (i < N_BIG)); ++i)
                        {
                          comp &= (edition_settings.ranguvannja_reset_dt_source_minus[N_BIG * index_of_dt + i] == current_settings.ranguvannja_reset_dt_source_minus[N_BIG * index_of_dt + i]);
                        }
                        if (comp == true)
                          current_ekran.edition = 0;
                        else
                          current_ekran.edition = 2;
                      }
                    }
                  }
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_AND1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_AND1 + NUMBER_DEFINED_AND - 1)))
                  {
                    unsigned int comp = true;
                    for (size_t i = 0; ((comp == true) && (i < N_BIG)); ++i)
                    {
                      comp &= (edition_settings.ranguvannja_d_and[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_D_AND1) + i] == current_settings.ranguvannja_d_and[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_D_AND1) + i]);
                    }
                    if (comp == true)
                      current_ekran.edition = 0;
                    else
                      current_ekran.edition = 2;
                  }
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_OR1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_OR1 + NUMBER_DEFINED_OR - 1)))
                  {
                    unsigned int comp = true;
                    for (size_t i = 0; ((comp == true) && (i < N_BIG)); ++i)
                    {
                      comp &= (edition_settings.ranguvannja_d_or[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_D_OR1) + i] == current_settings.ranguvannja_d_or[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_D_OR1) + i]);
                    }
                    if (comp == true)
                      current_ekran.edition = 0;
                    else
                      current_ekran.edition = 2;
                  }
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_XOR1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_XOR1 + NUMBER_DEFINED_XOR - 1)))
                  {
                    unsigned int comp = true;
                    for (size_t i = 0; ((comp == true) && (i < N_BIG)); ++i)
                    {
                      comp &= (edition_settings.ranguvannja_d_xor[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_D_XOR1) + i] == current_settings.ranguvannja_d_xor[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_D_XOR1) + i]);
                    }
                    if (comp == true)
                      current_ekran.edition = 0;
                    else
                      current_ekran.edition = 2;
                  }
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_NOT1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_NOT1 + NUMBER_DEFINED_NOT - 1)))
                  {
                    unsigned int comp = true;
                    for (size_t i = 0; ((comp == true) && (i < N_BIG)); ++i)
                    {
                      comp &= (edition_settings.ranguvannja_d_not[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_D_NOT1) + i] == current_settings.ranguvannja_d_not[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_D_NOT1) + i]);
                    }
                    if (comp == true)
                      current_ekran.edition = 0;
                    else
                      current_ekran.edition = 2;
                  }

                  current_ekran.cursor_on = 0;
                  current_ekran.cursor_blinking_on = 0;
                }
                else if (current_ekran.edition == 2)
                {
                  //Попередньо виставляємо повідомлення, що дані не достовірні
                  current_ekran.edition = 3;
                  //Обновляємо значення
                  if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_INPUT_1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_INPUT_1 + NUMBER_INPUTS - 1)))
                  {
                    unsigned int *point = edition_settings.ranguvannja_inputs;
                    if (count_number_set_bit(
                          (point + N_SMALL * (current_ekran.current_level - EKRAN_RANGUVANNJA_INPUT_1)),
                          NUMBER_TOTAL_SIGNAL_FOR_RANG_SMALL) <= MAX_FUNCTIONS_IN_INPUT)
                    {
                      //Помічаємо, що поле структури зараз буде змінене
                      changed_settings = CHANGED_ETAP_EXECUTION;

                      for (unsigned int i = 0; i < N_SMALL; i++)
                      {
                        current_settings.ranguvannja_inputs[N_SMALL * (current_ekran.current_level - EKRAN_RANGUVANNJA_INPUT_1) + i] =
                          edition_settings.ranguvannja_inputs[N_SMALL * (current_ekran.current_level - EKRAN_RANGUVANNJA_INPUT_1) + i];
                      }

                      //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                      fix_change_settings(1, 1);
                      //Виходимо з режиму редагування
                      current_ekran.edition = 0;
                    }
                  }
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_BUTTON_1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_BUTTON_1 + NUMBER_DEFINED_BUTTONS - 1)))
                  {
                    unsigned int *point = edition_settings.ranguvannja_buttons;
                    if (count_number_set_bit(
                          (point + N_SMALL * (current_ekran.current_level - EKRAN_RANGUVANNJA_BUTTON_1)),
                          NUMBER_TOTAL_SIGNAL_FOR_RANG_SMALL) <= MAX_FUNCTIONS_IN_DB)
                    {
                      //Помічаємо, що поле структури зараз буде змінене
                      changed_settings = CHANGED_ETAP_EXECUTION;

                      for (unsigned int i = 0; i < N_SMALL; i++)
                      {
                        current_settings.ranguvannja_buttons[N_SMALL * (current_ekran.current_level - EKRAN_RANGUVANNJA_BUTTON_1) + i] =
                          edition_settings.ranguvannja_buttons[N_SMALL * (current_ekran.current_level - EKRAN_RANGUVANNJA_BUTTON_1) + i];
                      }
                      //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                      fix_change_settings(1, 1);
                      //Виходимо з режиму редагування
                      current_ekran.edition = 0;
                    }
                  }
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                  else if (current_ekran.current_level == EKRAN_LN_FOR_IEC61850_RANG_SMALL)
                  {
                    int prev_ekran = previous_level_in_current_level_menu[EKRAN_LN_FOR_IEC61850_RANG_SMALL];
                    if ((prev_ekran >= EKRAN_IN_GOOSE1) && (prev_ekran <= EKRAN_IN_MMS4))
                    {
                      int n_out_in = position_in_current_level_menu[prev_ekran];

                      prev_ekran = previous_level_in_current_level_menu[prev_ekran];
                      if (
                        (n_out_in >= 0) &&
                        ((prev_ekran >= EKRAN_LIST_IN_GOOSE) || (prev_ekran <= EKRAN_LIST_IN_MMS)))
                      {
                        int n_LN = position_in_current_level_menu[prev_ekran];

                        prev_ekran = previous_level_in_current_level_menu[prev_ekran];
                        if (
                          (n_LN >= 0) &&
                          (prev_ekran == EKRAN_LIST_TYPE_IEC61850_NODES))
                        {
                          int type_LN = position_in_current_level_menu[prev_ekran];

                          if (
                            (type_LN >= 0) &&
                            (((type_LN == INDEX_TYPE_IEC61850_IN_GOOSE) && (n_LN < N_IN_GOOSE) && (n_out_in < N_IN_GOOSE_MMS_OUT)) ||
                             ((type_LN == INDEX_TYPE_IEC61850_IN_MMS) && (n_LN < N_IN_MMS) && (n_out_in < N_IN_GOOSE_MMS_OUT))))
                          {
                            uint32_t *p_rang = NULL, *p_rang_edit = NULL;
                            uint32_t max_functions = 0;
                            switch (type_LN)
                            {
                              case 0:
                                {
                                  max_functions = MAX_FUNCTIONS_IN_IN_GOOSE;
                                  p_rang = current_settings.ranguvannja_In_GOOSE[n_LN][n_out_in];
                                  p_rang_edit = edition_settings.ranguvannja_In_GOOSE[n_LN][n_out_in];
                                  break;
                                }
                              case 1:
                                {
                                  max_functions = MAX_FUNCTIONS_IN_IN_MMS;
                                  p_rang = current_settings.ranguvannja_In_MMS[n_LN][n_out_in];
                                  p_rang_edit = edition_settings.ranguvannja_In_MMS[n_LN][n_out_in];
                                  break;
                                }
                              default:
                                {
                                  //Теоретично цього ніколи не мало б бути
                                  total_error_sw_fixed();
                                  break;
                                }
                            }

                            if (count_number_set_bit(p_rang_edit, NUMBER_TOTAL_SIGNAL_FOR_RANG_SMALL) <= max_functions)
                            {
                              //Помічаємо, що поле структури зараз буде змінене
                              changed_settings = CHANGED_ETAP_EXECUTION;

                              for (unsigned int i = 0; i < N_SMALL; i++)
                                p_rang[i] = p_rang_edit[i];

                              //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                              fix_change_settings(1, 1);
                              //Виходимо з режиму редагування
                              current_ekran.edition = 0;
                            }
                          }
                          else
                            total_error_sw_fixed();
                        }
                        else
                          total_error_sw_fixed();
                      }
                      else
                        total_error_sw_fixed();
                    }
                    else
                      total_error_sw_fixed();
                  }
                  else if (current_ekran.current_level == EKRAN_LN_FOR_IEC61850_RANG)
                  {
                    int prev_ekran = previous_level_in_current_level_menu[EKRAN_LN_FOR_IEC61850_RANG];
                    if ((prev_ekran >= EKRAN_OUT_LAN1) && (prev_ekran <= EKRAN_OUT_LAN4))
                    {
                      int n_out_in = position_in_current_level_menu[prev_ekran];

                      prev_ekran = previous_level_in_current_level_menu[prev_ekran];
                      if (
                        (n_out_in >= 0) &&
                        (prev_ekran == EKRAN_LIST_OUT_LAN))
                      {
                        int n_LN = position_in_current_level_menu[prev_ekran];

                        prev_ekran = previous_level_in_current_level_menu[prev_ekran];

                        if ((n_LN < N_OUT_LAN) && (n_out_in < N_OUT_LAN_IN))
                        {
                          if (count_number_set_bit(edit_rang_Out_LAN, NUMBER_TOTAL_SIGNAL_FOR_RANG) <= MAX_FUNCTIONS_IN_OUT_LAN)
                          {
                            //Помічаємо, що поле структури зараз буде змінене
                            changed_settings = CHANGED_ETAP_EXECUTION;

                            uint16_t *p_rang = current_settings.ranguvannja_Out_LAN[n_LN][n_out_in];
                            for (size_t i = 0; i < MAX_FUNCTIONS_IN_OUT_LAN; i++)
                              p_rang[i] = 0;

                            unsigned int n_signals = 0;
                            for (size_t i = 0; i < NUMBER_TOTAL_SIGNAL_FOR_RANG; i++)
                            {
                              if (_CHECK_SET_BIT(edit_rang_Out_LAN, i))
                              {
                                p_rang[n_signals++] = i + 1;
                                if (n_signals > MAX_FUNCTIONS_IN_OUT_LAN)
                                  total_error_sw_fixed();
                              }
                            }

                            //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                            fix_change_settings(1, 1);
                            //Виходимо з режиму редагування
                            current_ekran.edition = 0;
                          }
                        }
                        else
                          total_error_sw_fixed();
                      }
                      else
                        total_error_sw_fixed();
                    }
                    else
                      total_error_sw_fixed();
                  }
#endif
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_OUTPUT_1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_OUTPUT_1 + NUMBER_OUTPUTS - 1)))
                  {
                    unsigned int *point = edition_settings.ranguvannja_outputs;
                    if (count_number_set_bit(
                          (point + N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_OUTPUT_1)),
                          NUMBER_TOTAL_SIGNAL_FOR_RANG) <= MAX_FUNCTIONS_IN_OUTPUT)
                    {
                      //Помічаємо, що поле структури зараз буде змінене
                      changed_settings = CHANGED_ETAP_EXECUTION;

                      for (unsigned int i = 0; i < N_BIG; i++)
                      {
                        current_settings.ranguvannja_outputs[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_OUTPUT_1) + i] =
                          edition_settings.ranguvannja_outputs[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_OUTPUT_1) + i];
                      }

                      //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                      fix_change_settings(1, 1);
                      //Виходимо з режиму редагування
                      current_ekran.edition = 0;
                    }
                  }
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_LED_1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_LED_1 + NUMBER_LEDS - 1)))
                  {
                    unsigned int *point = edition_settings.ranguvannja_leds;
                    if (count_number_set_bit(
                          (point + N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_LED_1)),
                          NUMBER_TOTAL_SIGNAL_FOR_RANG) <= MAX_FUNCTIONS_IN_LED)
                    {
                      //Помічаємо, що поле структури зараз буде змінене
                      changed_settings = CHANGED_ETAP_EXECUTION;

                      for (unsigned int i = 0; i < N_BIG; i++)
                      {
                        current_settings.ranguvannja_leds[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_LED_1) + i] =
                          edition_settings.ranguvannja_leds[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_LED_1) + i];
                      }

                      //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                      fix_change_settings(1, 1);
                      //Виходимо з режиму редагування
                      current_ekran.edition = 0;
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_RANGUVANNJA_ANALOG_REGISTRATOR)
                  {
                    unsigned int *point = edition_settings.ranguvannja_analog_registrator;
                    if (count_number_set_bit(
                          point,
                          NUMBER_TOTAL_SIGNAL_FOR_RANG) <= MAX_FUNCTIONS_IN_AREG)
                    {
                      //Помічаємо, що поле структури зараз буде змінене
                      changed_settings = CHANGED_ETAP_EXECUTION;

                      for (unsigned int i = 0; i < N_BIG; i++)
                      {
                        current_settings.ranguvannja_analog_registrator[i] = edition_settings.ranguvannja_analog_registrator[i];
                      }

                      //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                      fix_change_settings(1, 1);
                      //Виходимо з режиму редагування
                      current_ekran.edition = 0;
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_RANGUVANNJA_DIGITAL_REGISTRATOR)
                  {
                    unsigned int *point = edition_settings.ranguvannja_digital_registrator;
                    if (count_number_set_bit(
                          point,
                          NUMBER_TOTAL_SIGNAL_FOR_RANG) <= MAX_FUNCTIONS_IN_DREG)
                    {
                      //Помічаємо, що поле структури зараз буде змінене
                      changed_settings = CHANGED_ETAP_EXECUTION;

                      for (unsigned int i = 0; i < N_BIG; i++)
                      {
                        current_settings.ranguvannja_digital_registrator[i] = edition_settings.ranguvannja_digital_registrator[i];
                      }

                      //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                      fix_change_settings(1, 1);
                      //Виходимо з режиму редагування
                      current_ekran.edition = 0;
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_RANGUVANNJA_OFF_CB)
                  {
                    unsigned int *point = edition_settings.ranguvannja_off_cb;
                    if (count_number_set_bit(
                          point,
                          NUMBER_TOTAL_SIGNAL_FOR_RANG) <= MAX_FUNCTIONS_IN_OFF_CB)
                    {
                      //Помічаємо, що поле структури зараз буде змінене
                      changed_settings = CHANGED_ETAP_EXECUTION;

                      for (unsigned int i = 0; i < N_BIG; i++)
                      {
                        current_settings.ranguvannja_off_cb[i] = edition_settings.ranguvannja_off_cb[i];
                      }

                      //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                      fix_change_settings(1, 1);
                      //Виходимо з режиму редагування
                      current_ekran.edition = 0;
                    }
                  }
                  else if (current_ekran.current_level == EKRAN_RANGUVANNJA_ON_CB)
                  {
                    unsigned int *point = edition_settings.ranguvannja_on_cb;
                    if (count_number_set_bit(
                          point,
                          NUMBER_TOTAL_SIGNAL_FOR_RANG) <= MAX_FUNCTIONS_IN_ON_CB)
                    {
                      //Помічаємо, що поле структури зараз буде змінене
                      changed_settings = CHANGED_ETAP_EXECUTION;

                      for (unsigned int i = 0; i < N_BIG; i++)
                      {
                        current_settings.ranguvannja_on_cb[i] = edition_settings.ranguvannja_on_cb[i];
                      }

                      //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                      fix_change_settings(1, 1);
                      //Виходимо з режиму редагування
                      current_ekran.edition = 0;
                    }
                  }
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_DF1_PLUS) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_DF1_PLUS + MAX_ROW_LIST_TYPE_SOURCE_DF * NUMBER_DEFINED_FUNCTIONS - 1)))
                  {
                    unsigned int index_in_ekran_list = current_ekran.current_level - EKRAN_RANGUVANNJA_DF1_PLUS;
                    unsigned int type_source = index_in_ekran_list % MAX_ROW_LIST_TYPE_SOURCE_DF;
                    unsigned int index_of_df = index_in_ekran_list / MAX_ROW_LIST_TYPE_SOURCE_DF;

                    unsigned int *point = NULL;

                    if (type_source == INDEX_ML_LIST_TYPE_SOURCE_PLUS_DF)
                      point = edition_settings.ranguvannja_df_source_plus;
                    else if (type_source == INDEX_ML_LIST_TYPE_SOURCE_MINUS_DF)
                      point = edition_settings.ranguvannja_df_source_minus;
                    else if (type_source == INDEX_ML_LIST_TYPE_SOURCE_BLK_DF)
                      point = edition_settings.ranguvannja_df_source_blk;
                    else if (type_source == INDEX_ML_LIST_TYPE_SOURCE_R_DF)
                      point = edition_settings.ranguvannja_df_source_reset;
                    else
                      total_error_sw_fixed();

                    if (count_number_set_bit(
                          (point + N_BIG * index_of_df),
                          NUMBER_TOTAL_SIGNAL_FOR_RANG) <= MAX_FUNCTIONS_IN_DF)
                    {
                      //Помічаємо, що поле структури зараз буде змінене
                      changed_settings = CHANGED_ETAP_EXECUTION;

                      if (type_source == INDEX_ML_LIST_TYPE_SOURCE_PLUS_DF)
                      {
                        for (unsigned int i = 0; i < N_BIG; i++)
                        {
                          current_settings.ranguvannja_df_source_plus[N_BIG * index_of_df + i] = edition_settings.ranguvannja_df_source_plus[N_BIG * index_of_df + i];
                        }
                      }
                      else if (type_source == INDEX_ML_LIST_TYPE_SOURCE_MINUS_DF)
                      {
                        for (unsigned int i = 0; i < N_BIG; i++)
                        {
                          current_settings.ranguvannja_df_source_minus[N_BIG * index_of_df + i] = edition_settings.ranguvannja_df_source_minus[N_BIG * index_of_df + i];
                        }
                      }
                      else if (type_source == INDEX_ML_LIST_TYPE_SOURCE_BLK_DF)
                      {
                        for (unsigned int i = 0; i < N_BIG; i++)
                        {
                          current_settings.ranguvannja_df_source_blk[N_BIG * index_of_df + i] = edition_settings.ranguvannja_df_source_blk[N_BIG * index_of_df + i];
                        }
                      }
                      else if (type_source == INDEX_ML_LIST_TYPE_SOURCE_R_DF)
                      {
                        for (unsigned int i = 0; i < N_BIG; i++)
                        {
                          current_settings.ranguvannja_df_source_reset[N_BIG * index_of_df + i] = edition_settings.ranguvannja_df_source_reset[N_BIG * index_of_df + i];
                        }
                      }
                      else
                        total_error_sw_fixed();

                      //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                      fix_change_settings(1, 1);
                      //Виходимо з режиму редагування
                      current_ekran.edition = 0;
                    }
                  }
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_SET_DT1_PLUS) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_SET_DT1_PLUS + 4 * NUMBER_DEFINED_TRIGGERS - 1)))
                  {
                    unsigned int index_in_ekran_list = current_ekran.current_level - EKRAN_RANGUVANNJA_SET_DT1_PLUS;
                    unsigned int type_source = index_in_ekran_list % 2;
                    unsigned int type_of_action = (index_in_ekran_list / 2) & 0x1;
                    unsigned int index_of_dt = index_in_ekran_list / 4;

                    unsigned int *point;

                    if (type_of_action == INDEX_ML_SET_DT)
                    {
                      if (type_source == INDEX_ML_LIST_TYPE_SOURCE_PLUS_DT)
                        point = edition_settings.ranguvannja_set_dt_source_plus;
                      else
                        point = edition_settings.ranguvannja_set_dt_source_minus;
                    }
                    else
                    {
                      if (type_source == INDEX_ML_LIST_TYPE_SOURCE_PLUS_DT)
                        point = edition_settings.ranguvannja_reset_dt_source_plus;
                      else
                        point = edition_settings.ranguvannja_reset_dt_source_minus;
                    }

                    if (count_number_set_bit(
                          (point + N_BIG * index_of_dt),
                          NUMBER_TOTAL_SIGNAL_FOR_RANG) <= MAX_FUNCTIONS_IN_DT)
                    {
                      //Помічаємо, що поле структури зараз буде змінене
                      changed_settings = CHANGED_ETAP_EXECUTION;

                      if (type_of_action == INDEX_ML_SET_DT)
                      {
                        if (type_source == INDEX_ML_LIST_TYPE_SOURCE_PLUS_DT)
                        {
                          for (unsigned int i = 0; i < N_BIG; i++)
                          {
                            current_settings.ranguvannja_set_dt_source_plus[N_BIG * index_of_dt + i] = edition_settings.ranguvannja_set_dt_source_plus[N_BIG * index_of_dt + i];
                          }
                        }
                        else
                        {
                          for (unsigned int i = 0; i < N_BIG; i++)
                          {
                            current_settings.ranguvannja_set_dt_source_minus[N_BIG * index_of_dt + i] = edition_settings.ranguvannja_set_dt_source_minus[N_BIG * index_of_dt + i];
                          }
                        }
                      }
                      else
                      {
                        if (type_source == INDEX_ML_LIST_TYPE_SOURCE_PLUS_DT)
                        {
                          for (unsigned int i = 0; i < N_BIG; i++)
                          {
                            current_settings.ranguvannja_reset_dt_source_plus[N_BIG * index_of_dt + i] = edition_settings.ranguvannja_reset_dt_source_plus[N_BIG * index_of_dt + i];
                          }
                        }
                        else
                        {
                          for (unsigned int i = 0; i < N_BIG; i++)
                          {
                            current_settings.ranguvannja_reset_dt_source_minus[N_BIG * index_of_dt + i] = edition_settings.ranguvannja_reset_dt_source_minus[N_BIG * index_of_dt + i];
                          }
                        }
                      }

                      //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                      fix_change_settings(1, 1);
                      //Виходимо з режиму редагування
                      current_ekran.edition = 0;
                    }
                  }
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_AND1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_AND1 + NUMBER_DEFINED_AND - 1)))
                  {
                    unsigned int *point = edition_settings.ranguvannja_d_and;
                    if (count_number_set_bit(
                          (point + N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_D_AND1)),
                          NUMBER_TOTAL_SIGNAL_FOR_RANG) <= MAX_FUNCTIONS_IN_D_AND)
                    {
                      //Помічаємо, що поле структури зараз буде змінене
                      changed_settings = CHANGED_ETAP_EXECUTION;

                      for (unsigned int i = 0; i < N_BIG; i++)
                      {
                        current_settings.ranguvannja_d_and[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_D_AND1) + i] =
                          edition_settings.ranguvannja_d_and[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_D_AND1) + i];
                      }

                      //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                      fix_change_settings(1, 1);
                      //Виходимо з режиму редагування
                      current_ekran.edition = 0;
                    }
                  }
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_OR1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_OR1 + NUMBER_DEFINED_OR - 1)))
                  {
                    unsigned int *point = edition_settings.ranguvannja_d_or;
                    if (count_number_set_bit(
                          (point + N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_D_OR1)),
                          NUMBER_TOTAL_SIGNAL_FOR_RANG) <= MAX_FUNCTIONS_IN_D_OR)
                    {
                      //Помічаємо, що поле структури зараз буде змінене
                      changed_settings = CHANGED_ETAP_EXECUTION;

                      for (unsigned int i = 0; i < N_BIG; i++)
                      {
                        current_settings.ranguvannja_d_or[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_D_OR1) + i] =
                          edition_settings.ranguvannja_d_or[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_D_OR1) + i];
                      }

                      //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                      fix_change_settings(1, 1);
                      //Виходимо з режиму редагування
                      current_ekran.edition = 0;
                    }
                  }
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_XOR1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_XOR1 + NUMBER_DEFINED_XOR - 1)))
                  {
                    unsigned int *point = edition_settings.ranguvannja_d_xor;
                    if (count_number_set_bit(
                          (point + N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_D_XOR1)),
                          NUMBER_TOTAL_SIGNAL_FOR_RANG) <= MAX_FUNCTIONS_IN_D_XOR)
                    {
                      //Помічаємо, що поле структури зараз буде змінене
                      changed_settings = CHANGED_ETAP_EXECUTION;

                      for (unsigned int i = 0; i < N_BIG; i++)
                      {
                        current_settings.ranguvannja_d_xor[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_D_XOR1) + i] =
                          edition_settings.ranguvannja_d_xor[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_D_XOR1) + i];
                      }

                      //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                      fix_change_settings(1, 1);
                      //Виходимо з режиму редагування
                      current_ekran.edition = 0;
                    }
                  }
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_NOT1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_NOT1 + NUMBER_DEFINED_NOT - 1)))
                  {
                    unsigned int *point = edition_settings.ranguvannja_d_not;
                    if (count_number_set_bit(
                          (point + N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_D_NOT1)),
                          NUMBER_TOTAL_SIGNAL_FOR_RANG) <= MAX_FUNCTIONS_IN_D_NOT)
                    {
                      //Помічаємо, що поле структури зараз буде змінене
                      changed_settings = CHANGED_ETAP_EXECUTION;

                      for (unsigned int i = 0; i < N_BIG; i++)
                      {
                        current_settings.ranguvannja_d_not[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_D_NOT1) + i] =
                          edition_settings.ranguvannja_d_not[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_D_NOT1) + i];
                      }

                      //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
                      fix_change_settings(1, 1);
                      //Виходимо з режиму редагування
                      current_ekran.edition = 0;
                    }
                  }
                }
                else if (current_ekran.edition == 3)
                {
                  //Вихід у режимі редагування
                  current_ekran.edition = 0;
                }

                if (current_ekran.edition == 2)
                  make_ekran_ask_rewrite();
                else if (current_ekran.edition == 3)
                {
                  static unsigned char const information_about_error[MAX_NAMBER_LANGUAGE][MAX_COL_LCD] =
                    {
                      "Прев.макс.кол.ф.",
                      "Переб.макс.кіл.ф",
                      " Max.n.of F.Exc.",
                      "Прев.макс.кол.ф."};
                  make_ekran_about_error(information_about_error);
                }
                else
                  //Виставляємо біт обновлення екрану
                  new_state_keyboard |= (1u << BIT_REWRITE);

                //Очистити сигналізацію, що натиснута кнопка
                new_state_keyboard &= ~(1u << BIT_KEY_ENTER);
              }
              else if (new_state_keyboard == (1u << BIT_KEY_ESC))
              {
                if (current_ekran.edition == 0)
                {
                  //Вихід у режимі спостерігання
                  //Переходимо у попереднє меню
                  current_ekran.current_level = previous_level_in_current_level_menu[current_ekran.current_level];
                  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
                }
                //Інекше переходдимо у режим спостерігання
                current_ekran.edition = 0;

                //Виставляємо команду на обновлекння нового екрану
                new_state_keyboard |= (1u << BIT_REWRITE);
                //Очистити сигналізацію, що натиснута кнопка
                new_state_keyboard &= ~(1u << BIT_KEY_ESC);
              }
              else if (new_state_keyboard == (1u << BIT_KEY_UP))
              {
                //Натиснута кнопка UP
                if (
                  ((current_ekran.current_level >= EKRAN_RANGUVANNJA_BUTTON_1) && (current_ekran.current_level <= EKRAN_RANGUVANNJA_BUTTON_6)) ||
                  ((current_ekran.current_level >= EKRAN_RANGUVANNJA_INPUT_1) && (current_ekran.current_level <= EKRAN_RANGUVANNJA_INPUT_LAST))
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                  ||
                  (current_ekran.current_level == EKRAN_LN_FOR_IEC61850_RANG_SMALL)
#endif
                )
                {
                  unsigned int temp_state[N_SMALL];
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                  int type_LN = -1;
                  int n_LN = -1;
#endif

                  if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_BUTTON_1) && (current_ekran.current_level <= EKRAN_RANGUVANNJA_BUTTON_6))
                  {
                    uint32_t *p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_buttons : edition_settings.ranguvannja_buttons;
                    for (size_t i = 0; i < N_SMALL; i++)
                      temp_state[i] = p_rang[N_SMALL * (current_ekran.current_level - EKRAN_RANGUVANNJA_BUTTON_1) + i];
                  }
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_INPUT_1) && (current_ekran.current_level <= EKRAN_RANGUVANNJA_INPUT_LAST))
                  {
                    uint32_t *p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_inputs : edition_settings.ranguvannja_inputs;
                    for (size_t i = 0; i < N_SMALL; i++)
                      temp_state[i] = p_rang[N_SMALL * (current_ekran.current_level - EKRAN_RANGUVANNJA_INPUT_1) + i];
                  }
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                  else if (current_ekran.current_level == EKRAN_LN_FOR_IEC61850_RANG_SMALL)
                  {
                    int prev_ekran = previous_level_in_current_level_menu[EKRAN_LN_FOR_IEC61850_RANG_SMALL];
                    if ((prev_ekran >= EKRAN_IN_GOOSE1) && (prev_ekran <= EKRAN_IN_MMS4))
                    {
                      int n_out_in = position_in_current_level_menu[prev_ekran];

                      prev_ekran = previous_level_in_current_level_menu[prev_ekran];
                      if (
                        (n_out_in >= 0) &&
                        ((prev_ekran >= EKRAN_LIST_IN_GOOSE) && (prev_ekran <= EKRAN_LIST_OUT_LAN)))
                      {
                        n_LN = position_in_current_level_menu[prev_ekran];

                        prev_ekran = previous_level_in_current_level_menu[prev_ekran];
                        if (
                          (n_LN >= 0) &&
                          (prev_ekran == EKRAN_LIST_TYPE_IEC61850_NODES))
                        {
                          type_LN = position_in_current_level_menu[prev_ekran];

                          if (
                            (type_LN >= 0) &&
                            (((type_LN == INDEX_TYPE_IEC61850_IN_GOOSE) && (n_LN < N_IN_GOOSE) && (n_out_in < N_IN_GOOSE_MMS_OUT)) ||
                             ((type_LN == INDEX_TYPE_IEC61850_IN_MMS) && (n_LN < N_IN_MMS) && (n_out_in < N_IN_GOOSE_MMS_OUT))))
                          {
                            uint32_t *p_rang = NULL;
                            switch (type_LN)
                            {
                              case 0:
                                {
                                  p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_In_GOOSE[n_LN][n_out_in] : edition_settings.ranguvannja_In_GOOSE[n_LN][n_out_in];
                                  break;
                                }
                              case 1:
                                {
                                  p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_In_MMS[n_LN][n_out_in] : edition_settings.ranguvannja_In_MMS[n_LN][n_out_in];
                                  break;
                                }
                              default:
                                {
                                  //Теоретично цього ніколи не мало б бути
                                  total_error_sw_fixed();
                                  break;
                                }
                            }

                            for (size_t i = 0; i < N_SMALL; i++)
                              temp_state[i] = p_rang[i];
                          }
                          else
                            total_error_sw_fixed();
                        }
                        else
                          total_error_sw_fixed();
                      }
                      else
                        total_error_sw_fixed();
                    }
                    else
                      total_error_sw_fixed();
                  }
#endif

                  if (current_ekran.edition == 0)
                  {
                    unsigned int comp = true;
                    for (size_t i = 0; ((comp == true) && (i < N_SMALL)); ++i)
                    {
                      comp &= (temp_state[i] == 0);
                    }

                    if (comp)
                      current_ekran.index_position = 0;
                    else
                    {
                      if (--current_ekran.index_position < 0)
                        current_ekran.index_position = NUMBER_TOTAL_SIGNAL_FOR_RANG_SMALL - 1;
                      // (x>>5) аналогічне операції x / 32 - ціла частина від ділення на 32
                      // (x & 0x1f) аналогічне операції x % 32 - остача від ділення на 32
                      while ((temp_state[current_ekran.index_position >> 5] & (1 << (current_ekran.index_position & 0x1f))) == 0)
                      {
                        current_ekran.index_position--;
                        if (current_ekran.index_position < 0)
                          current_ekran.index_position = NUMBER_TOTAL_SIGNAL_FOR_RANG_SMALL - 1;
                      }
                    }
                  }
                  else
                  {
                    //Редагування
                    unsigned int found_new_index = 0;
                    // int add_filter[0 + 1] =
                    //   {
                    //     -1 /*признак завершення масиву*/
                    //   };
                    //                  EL_FILTER_STRUCT el_filter[NUMBER_DEFINED_ELEMENTS] =
                    //                  {
                    //                    {1, RANG_SMALL_DF1_IN , RANG_SMALL_DF8_IN   , 1, current_settings.number_defined_df },
                    //                    {1, RANG_SMALL_DT1_SET, RANG_SMALL_DT4_RESET, 2, current_settings.number_defined_dt },
                    //                    {0, 0                 , 0                   , 1, current_settings.number_defined_and},
                    //                    {0, 0                 , 0                   , 1, current_settings.number_defined_or },
                    //                    {0, 0                 , 0                   , 1, current_settings.number_defined_xor},
                    //                    {0, 0                 , 0                   , 1, current_settings.number_defined_not}
                    //                  };

                    //Переміщаємося на наступну функцію
                    if (--current_ekran.index_position < 0)
                      current_ekran.index_position = NUMBER_TOTAL_SIGNAL_FOR_RANG_SMALL - 1;
                    //Перевіряємо, чи даний індекс функції присутній у даній конфігурації
                    while (found_new_index == 0)
                    {
                      check_current_index_is_presented_in_configuration(&found_new_index,
                                                                        /* add_filter */ NULL,
                                                                        /*el_filter,*/
                                                                        0,
                                                                        NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL,
                                                                        NUMBER_RPN_SIGNAL_FOR_RANG_SMALL,
                                                                        NUMBER_SZKh_SIGNAL_FOR_RANG_SMALL,
                                                                        NUMBER_SNKh_SIGNAL_FOR_RANG_SMALL,
                                                                        NUMBER_BRP_SIGNAL_FOR_RANG_SMALL,
                                                                        NUMBER_UMAX_SIGNAL_FOR_RANG_SMALL,
                                                                        NUMBER_UMIN_SIGNAL_FOR_RANG_SMALL,
                                                                        NUMBER_UP_SIGNAL_FOR_RANG_SMALL,
                                                                        NUMBER_EL_SIGNAL_FOR_RANG_SMALL);

                      //Перевіряємо режим рооботи функціональної кнопки (якщо іде редагування ФК)
                      if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_BUTTON_1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_BUTTON_1 + NUMBER_DEFINED_BUTTONS - 1)))
                      {
                        uint32_t mode = (current_settings.buttons_mode >> (current_ekran.current_level - EKRAN_RANGUVANNJA_BUTTON_1)) & 0x1;
                        if (_CHECK_SET_BIT(buttons_mode[mode], current_ekran.index_position) == 0)
                        {
                          found_new_index = 0;
                          current_ekran.index_position--;
                        }
                      }
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                      else if (current_ekran.current_level == EKRAN_LN_FOR_IEC61850_RANG_SMALL)
                      {
                        if ((type_LN == INDEX_TYPE_IEC61850_IN_GOOSE) || (type_LN == INDEX_TYPE_IEC61850_IN_MMS))
                        {
                          if (
                            (_CHECK_SET_BIT(rang_iec61850_blocks[type_LN], current_ekran.index_position) == 0) ||
                            ((type_LN == INDEX_TYPE_IEC61850_IN_GOOSE) && (current_ekran.index_position == (RANG_SMALL_BLOCK_IN_GOOSE1 + n_LN))) ||
                            ((type_LN == INDEX_TYPE_IEC61850_IN_MMS) && (current_ekran.index_position == (RANG_SMALL_BLOCK_IN_MMS1 + n_LN))))
                          {
                            found_new_index = 0;
                            current_ekran.index_position--;
                          }
                        }
                        else
                          total_error_sw_fixed();
                      }
#endif

                      //Перевіряємо, чи ми не вийшли за допустиму кількість функцій
                      if (current_ekran.index_position < 0)
                      {
                        found_new_index = 0;
                        current_ekran.index_position = NUMBER_TOTAL_SIGNAL_FOR_RANG_SMALL - 1;
                      }
                    }
                  }
                  position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;

                  //Формуємо екран відображення зранжованих функцій
                  if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_BUTTON_1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_BUTTON_1 + NUMBER_DEFINED_BUTTONS - 1)))
                    make_ekran_set_function_in_bi(current_ekran.current_level, INDEX_VIEWING_BUTTON, temp_state
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                                                  ,
                                                  type_LN, n_LN
#endif
                    );
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_INPUT_1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_INPUT_1 + NUMBER_INPUTS - 1)))
                    make_ekran_set_function_in_bi(current_ekran.current_level, INDEX_VIEWING_INPUT, temp_state
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                                                  ,
                                                  type_LN, n_LN
#endif
                    );

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                  else if (current_ekran.current_level == EKRAN_LN_FOR_IEC61850_RANG_SMALL)
                    make_ekran_set_function_in_bi(current_ekran.current_level, INDEX_VIEWING_IEC61850_RANG, temp_state, type_LN, n_LN);
#endif
                }
                else if (
                  ((current_ekran.current_level >= EKRAN_RANGUVANNJA_OUTPUT_1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_OUTPUT_1 + NUMBER_OUTPUTS - 1))) ||
                  ((current_ekran.current_level >= EKRAN_RANGUVANNJA_LED_1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_LED_1 + NUMBER_LEDS - 1))) ||
                  (current_ekran.current_level == EKRAN_RANGUVANNJA_ANALOG_REGISTRATOR) ||
                  (current_ekran.current_level == EKRAN_RANGUVANNJA_DIGITAL_REGISTRATOR) ||
                  (current_ekran.current_level == EKRAN_RANGUVANNJA_OFF_CB) ||
                  (current_ekran.current_level == EKRAN_RANGUVANNJA_ON_CB) ||
                  ((current_ekran.current_level >= EKRAN_RANGUVANNJA_DF1_PLUS) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_DF1_PLUS + MAX_ROW_LIST_TYPE_SOURCE_DF * NUMBER_DEFINED_FUNCTIONS - 1))) ||
                  ((current_ekran.current_level >= EKRAN_RANGUVANNJA_SET_DT1_PLUS) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_SET_DT1_PLUS + 4 * NUMBER_DEFINED_TRIGGERS - 1))) ||
                  ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_AND1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_AND1 + NUMBER_DEFINED_AND - 1))) ||
                  ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_OR1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_OR1 + NUMBER_DEFINED_OR - 1))) ||
                  ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_XOR1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_XOR1 + NUMBER_DEFINED_XOR - 1))) ||
                  ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_NOT1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_NOT1 + NUMBER_DEFINED_NOT - 1)))
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                  ||
                  (current_ekran.current_level == EKRAN_LN_FOR_IEC61850_RANG)
#endif
                )
                {
                  unsigned int temp_state[N_BIG];

                  if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_OUTPUT_1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_OUTPUT_1 + NUMBER_OUTPUTS - 1)))
                  {
                    uint32_t *p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_outputs : edition_settings.ranguvannja_outputs;
                    for (size_t i = 0; i < N_BIG; i++)
                      temp_state[i] = p_rang[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_OUTPUT_1) + i];
                  }
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_LED_1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_LED_1 + NUMBER_LEDS - 1)))
                  {
                    uint32_t *p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_leds : edition_settings.ranguvannja_leds;
                    for (size_t i = 0; i < N_BIG; i++)
                      temp_state[i] = p_rang[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_LED_1) + i];
                  }
                  else if (current_ekran.current_level == EKRAN_RANGUVANNJA_ANALOG_REGISTRATOR)
                  {
                    uint32_t *p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_analog_registrator : edition_settings.ranguvannja_analog_registrator;
                    for (size_t i = 0; i < N_BIG; i++)
                      temp_state[i] = p_rang[i];
                  }
                  else if (current_ekran.current_level == EKRAN_RANGUVANNJA_DIGITAL_REGISTRATOR)
                  {
                    uint32_t *p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_digital_registrator : edition_settings.ranguvannja_digital_registrator;
                    for (size_t i = 0; i < N_BIG; i++)
                      temp_state[i] = p_rang[i];
                  }
                  else if (current_ekran.current_level == EKRAN_RANGUVANNJA_OFF_CB)
                  {
                    uint32_t *p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_off_cb : edition_settings.ranguvannja_off_cb;
                    for (size_t i = 0; i < N_BIG; i++)
                      temp_state[i] = p_rang[i];
                  }
                  else if (current_ekran.current_level == EKRAN_RANGUVANNJA_ON_CB)
                  {
                    uint32_t *p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_on_cb : edition_settings.ranguvannja_on_cb;
                    for (size_t i = 0; i < N_BIG; i++)
                      temp_state[i] = p_rang[i];
                  }
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_DF1_PLUS) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_DF1_PLUS + MAX_ROW_LIST_TYPE_SOURCE_DF * NUMBER_DEFINED_FUNCTIONS - 1)))
                  {
                    unsigned int index_in_ekran_list = current_ekran.current_level - EKRAN_RANGUVANNJA_DF1_PLUS;
                    unsigned int type_source = index_in_ekran_list % MAX_ROW_LIST_TYPE_SOURCE_DF;
                    unsigned int index_of_df = index_in_ekran_list / MAX_ROW_LIST_TYPE_SOURCE_DF;

                    uint32_t *p_rang = NULL;
                    switch (type_source)
                    {
                      case INDEX_ML_LIST_TYPE_SOURCE_PLUS_DF:
                        {
                          p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_df_source_plus : edition_settings.ranguvannja_df_source_plus;
                          break;
                        }
                      case INDEX_ML_LIST_TYPE_SOURCE_MINUS_DF:
                        {
                          p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_df_source_minus : edition_settings.ranguvannja_df_source_minus;
                          break;
                        }
                      case INDEX_ML_LIST_TYPE_SOURCE_BLK_DF:
                        {
                          p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_df_source_blk : edition_settings.ranguvannja_df_source_blk;
                          break;
                        }
                      case INDEX_ML_LIST_TYPE_SOURCE_R_DF:
                        {
                          p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_df_source_reset : edition_settings.ranguvannja_df_source_reset;
                          break;
                        }
                      default:
                        {
                          total_error_sw_fixed();
                        }
                    }

                    for (size_t i = 0; i < N_BIG; i++)
                      temp_state[i] = p_rang[N_BIG * index_of_df + i];
                  }
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_SET_DT1_PLUS) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_SET_DT1_PLUS + 4 * NUMBER_DEFINED_TRIGGERS - 1)))
                  {
                    unsigned int index_in_ekran_list = current_ekran.current_level - EKRAN_RANGUVANNJA_SET_DT1_PLUS;
                    unsigned int type_source = index_in_ekran_list % 2;
                    unsigned int type_of_action = (index_in_ekran_list / 2) & 0x1;
                    unsigned int index_of_dt = index_in_ekran_list / 4;

                    uint32_t *p_rang = NULL;
                    switch (type_of_action)
                    {
                      case INDEX_ML_SET_DT:
                        {
                          switch (type_source)
                          {
                            case INDEX_ML_LIST_TYPE_SOURCE_PLUS_DT:
                              {
                                p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_set_dt_source_plus : edition_settings.ranguvannja_set_dt_source_plus;
                                break;
                              }
                            case INDEX_ML_LIST_TYPE_SOURCE_MINUS_DT:
                              {
                                p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_set_dt_source_minus : edition_settings.ranguvannja_set_dt_source_minus;
                                break;
                              }
                            default:
                              {
                                total_error_sw_fixed();
                              }
                          }
                          break;
                        }
                      case INDEX_ML_RESET_DT:
                        {
                          switch (type_source)
                          {
                            case INDEX_ML_LIST_TYPE_SOURCE_PLUS_DT:
                              {
                                p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_reset_dt_source_plus : edition_settings.ranguvannja_reset_dt_source_plus;
                                break;
                              }
                            case INDEX_ML_LIST_TYPE_SOURCE_MINUS_DT:
                              {
                                p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_reset_dt_source_minus : edition_settings.ranguvannja_reset_dt_source_minus;
                                break;
                              }
                            default:
                              {
                                total_error_sw_fixed();
                              }
                          }
                          break;
                        }
                      default:
                        {
                          total_error_sw_fixed();
                        }
                    }

                    for (size_t i = 0; i < N_BIG; i++)
                      temp_state[i] = p_rang[N_BIG * index_of_dt + i];
                  }
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_AND1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_AND1 + NUMBER_DEFINED_AND - 1)))
                  {
                    uint32_t *p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_d_and : edition_settings.ranguvannja_d_and;
                    for (size_t i = 0; i < N_BIG; i++)
                      temp_state[i] = p_rang[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_D_AND1) + i];
                  }
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_OR1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_OR1 + NUMBER_DEFINED_OR - 1)))
                  {
                    uint32_t *p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_d_or : edition_settings.ranguvannja_d_or;
                    for (size_t i = 0; i < N_BIG; i++)
                      temp_state[i] = p_rang[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_D_OR1) + i];
                  }
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_XOR1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_XOR1 + NUMBER_DEFINED_XOR - 1)))
                  {
                    uint32_t *p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_d_xor : edition_settings.ranguvannja_d_xor;
                    for (size_t i = 0; i < N_BIG; i++)
                      temp_state[i] = p_rang[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_D_XOR1) + i];
                  }
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_NOT1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_NOT1 + NUMBER_DEFINED_NOT - 1)))
                  {
                    uint32_t *p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_d_not : edition_settings.ranguvannja_d_not;
                    for (size_t i = 0; i < N_BIG; i++)
                      temp_state[i] = p_rang[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_D_NOT1) + i];
                  }
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                  else if (current_ekran.current_level == EKRAN_LN_FOR_IEC61850_RANG)
                  {
                    int prev_ekran = previous_level_in_current_level_menu[EKRAN_LN_FOR_IEC61850_RANG];
                    if ((prev_ekran >= EKRAN_OUT_LAN1) && (prev_ekran <= EKRAN_OUT_LAN4))
                    {
                      int n_out_in = position_in_current_level_menu[prev_ekran];

                      prev_ekran = previous_level_in_current_level_menu[prev_ekran];
                      if (
                        (n_out_in >= 0) &&
                        (prev_ekran == EKRAN_LIST_OUT_LAN))
                      {
                        int n_LN = position_in_current_level_menu[prev_ekran];

                        if ((n_LN < N_OUT_LAN) && (n_out_in < N_OUT_LAN_IN))
                        {
                          if (current_ekran.edition == 0)
                          {
                            uint16_t *p_rang = current_settings.ranguvannja_Out_LAN[n_LN][n_out_in];

                            for (size_t i = 0; i < N_BIG; i++)
                              temp_state[i] = 0;
                            for (size_t i = 0; i < MAX_FUNCTIONS_IN_OUT_LAN; i++)
                            {
                              uint16_t val = p_rang[i];
                              if (val > 0)
                                _SET_BIT(temp_state, (p_rang[i] - 1));
                              else
                                break;
                            }
                          }
                          else
                          {
                            for (size_t i = 0; i < N_BIG; i++)
                              temp_state[i] = edit_rang_Out_LAN[i];
                          }
                        }
                        else
                          total_error_sw_fixed();
                      }
                      else
                        total_error_sw_fixed();
                    }
                    else
                      total_error_sw_fixed();
                  }
#endif

                  if (current_ekran.edition == 0)
                  {
                    unsigned int comp = true;
                    for (size_t i = 0; ((comp == true) && (i < N_BIG)); ++i)
                    {
                      comp &= (temp_state[i] == 0);
                    }
                    if (comp == true)
                      current_ekran.index_position = 0;
                    else
                    {
                      if (--current_ekran.index_position < 0)
                        current_ekran.index_position = NUMBER_TOTAL_SIGNAL_FOR_RANG - 1;
                      // (x>>5) аналогічне операції x / 32 - ціла частина від ділення на 32
                      // (x & 0x1f) аналогічне операції x % 32 - остача від ділення на 32
                      while ((temp_state[current_ekran.index_position >> 5] & (1 << (current_ekran.index_position & 0x1f))) == 0)
                      {
                        current_ekran.index_position--;
                        if (current_ekran.index_position < 0)
                          current_ekran.index_position = NUMBER_TOTAL_SIGNAL_FOR_RANG - 1;
                      }
                    }
                  }
                  else
                  {
                    //Редагування
                    unsigned int found_new_index = 0;
                    // int add_filter[3 + 1] =
                    //   {
                    //     RANG_PO_NZZ,
                    //     RANG_NZZ,
                    //     RANG_SECTOR_NZZ,
                    //     -1 /*признак завершення масиву*/
                    //   };
                    //                  EL_FILTER_STRUCT el_filter[NUMBER_DEFINED_ELEMENTS] =
                    //                  {
                    //                    {1, RANG_DF1_IN , RANG_DF8_OUT, 2, current_settings.number_defined_df },
                    //                    {1, RANG_DT1_SET, RANG_DT4_OUT, 3, current_settings.number_defined_dt },
                    //                    {1, RANG_D_AND1 , RANG_D_AND8 , 1, current_settings.number_defined_and},
                    //                    {1, RANG_D_OR1  , RANG_D_OR8  , 1, current_settings.number_defined_or },
                    //                    {1, RANG_D_XOR1 , RANG_D_XOR8 , 1, current_settings.number_defined_xor},
                    //                    {1, RANG_D_NOT1 , RANG_D_NOT16, 1, current_settings.number_defined_not}
                    //                  };

                    //Переміщаємося на наступну функцію
                    if (--current_ekran.index_position < 0)
                      current_ekran.index_position = NUMBER_TOTAL_SIGNAL_FOR_RANG - 1;
                    //Перевіряємо, чи даний індекс функції присутній у даній конфігурації
                    while (found_new_index == 0)
                    {
                      check_current_index_is_presented_in_configuration(&found_new_index,
                                                                        /* add_filter */ NULL,
                                                                        /*el_filter,*/
                                                                        0,
                                                                        NUMBER_GENERAL_SIGNAL_FOR_RANG,
                                                                        NUMBER_RPN_SIGNAL_FOR_RANG,
                                                                        NUMBER_SZKh_SIGNAL_FOR_RANG,
                                                                        NUMBER_SNKh_SIGNAL_FOR_RANG,
                                                                        NUMBER_BRP_SIGNAL_FOR_RANG,
                                                                        NUMBER_UMAX_SIGNAL_FOR_RANG,
                                                                        NUMBER_UMIN_SIGNAL_FOR_RANG,
                                                                        NUMBER_UP_SIGNAL_FOR_RANG,
                                                                        NUMBER_EL_SIGNAL_FOR_RANG);
                      //Перевіряємо, чи ми не  на індексі функцій із списку загальних, яку треба викинути для даного типу ранжування
                      if (
                        (current_ekran.current_level == EKRAN_RANGUVANNJA_ANALOG_REGISTRATOR) ||
                        (current_ekran.current_level == EKRAN_RANGUVANNJA_DIGITAL_REGISTRATOR) ||
                        (current_ekran.current_level == EKRAN_RANGUVANNJA_OFF_CB) ||
                        (current_ekran.current_level == EKRAN_RANGUVANNJA_ON_CB))
                      {
                        size_t number = 1;
                        for (size_t num = 0; num < number; ++num)
                        {
                          unsigned int index_deleted_function = 0;

                          if (current_ekran.current_level == EKRAN_RANGUVANNJA_ANALOG_REGISTRATOR)
                            index_deleted_function = RANG_WORK_A_REJESTRATOR;
                          else if (current_ekran.current_level == EKRAN_RANGUVANNJA_DIGITAL_REGISTRATOR)
                            index_deleted_function = RANG_WORK_D_REJESTRATOR;
                          else if (current_ekran.current_level == EKRAN_RANGUVANNJA_OFF_CB)
                          {
                            number = 2;

                            if (num == 0)
                              index_deleted_function = RANG_WORK_BO;
                            else
                              index_deleted_function = RANG_VIDKL_VID_ZAKHYSTIV;
                          }
                          else if (current_ekran.current_level == EKRAN_RANGUVANNJA_ON_CB)
                            index_deleted_function = RANG_WORK_BV;

                          if (index_deleted_function == current_ekran.index_position)
                          {
                            found_new_index = 0;
                            current_ekran.index_position--;
                          }
                        }
                      }
                      else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_DF1_PLUS) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_DF1_PLUS + MAX_ROW_LIST_TYPE_SOURCE_DF * NUMBER_DEFINED_FUNCTIONS - 1)))
                      {
                        unsigned int index_in_ekran_list = current_ekran.current_level - EKRAN_RANGUVANNJA_DF1_PLUS;
                        unsigned int index_of_df = index_in_ekran_list / MAX_ROW_LIST_TYPE_SOURCE_DF;

                        for (unsigned int i = 0; i < 3; i++)
                        {
                          //Першою перевіряємо функцію з більшим номером, щоб за одну операцію циклу можна було переміститися на функцію, яку можна ранжувати
                          unsigned int const index_deleted_function = RANG_DF1_OUT + 3 /*кількість сигналів*/ * index_of_df - i;

                          if (index_deleted_function == current_ekran.index_position)
                          {
                            found_new_index = 0;
                            current_ekran.index_position--;
                          }
                        }
                      }
                      else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_SET_DT1_PLUS) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_SET_DT1_PLUS + 4 * NUMBER_DEFINED_TRIGGERS - 1)))
                      {
                        unsigned int index_in_ekran_list = current_ekran.current_level - EKRAN_RANGUVANNJA_SET_DT1_PLUS;
                        unsigned int index_of_dt = index_in_ekran_list / 4;

                        for (unsigned int i = 0; i < 3; i++)
                        {
                          unsigned int index_deleted_function = 0;
                          //Першою перевіряємо функцію з більшим номером, щоб за одну операцію циклу  можна було переміститися на функцію, яку можна ранжувати
                          if (i == 0)
                          {
                            if (index_of_dt == 0)
                              index_deleted_function = RANG_DT1_OUT;
                            else if (index_of_dt == 1)
                              index_deleted_function = RANG_DT2_OUT;
                            else if (index_of_dt == 2)
                              index_deleted_function = RANG_DT3_OUT;
                            else if (index_of_dt == 3)
                              index_deleted_function = RANG_DT4_OUT;
                          }
                          else if (i == 1)
                          {
                            if (index_of_dt == 0)
                              index_deleted_function = RANG_DT1_RESET;
                            else if (index_of_dt == 1)
                              index_deleted_function = RANG_DT2_RESET;
                            else if (index_of_dt == 2)
                              index_deleted_function = RANG_DT3_RESET;
                            else if (index_of_dt == 3)
                              index_deleted_function = RANG_DT4_RESET;
                          }
                          else
                          {
                            if (index_of_dt == 0)
                              index_deleted_function = RANG_DT1_SET;
                            else if (index_of_dt == 1)
                              index_deleted_function = RANG_DT2_SET;
                            else if (index_of_dt == 2)
                              index_deleted_function = RANG_DT3_SET;
                            else if (index_of_dt == 3)
                              index_deleted_function = RANG_DT4_SET;
                          }

                          if (index_deleted_function == current_ekran.index_position)
                          {
                            found_new_index = 0;
                            current_ekran.index_position--;
                          }
                        }
                      }
                      else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_AND1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_AND1 + NUMBER_DEFINED_AND - 1)))
                      {
                        unsigned int index_of_d_and = current_ekran.current_level - EKRAN_RANGUVANNJA_D_AND1;

                        //Першою перевіряємо функцію з меншим номером, щоб за одну операцію циклу  можна було переміститися на функцію, яку можна ранжувати
                        unsigned int index_deleted_function = RANG_D_AND1 + index_of_d_and;

                        if (index_deleted_function == current_ekran.index_position)
                        {
                          found_new_index = 0;
                          current_ekran.index_position--;
                        }
                      }
                      else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_OR1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_OR1 + NUMBER_DEFINED_OR - 1)))
                      {
                        unsigned int index_of_d_or = current_ekran.current_level - EKRAN_RANGUVANNJA_D_OR1;

                        //Першою перевіряємо функцію з меншим номером, щоб за одну операцію циклу  можна було переміститися на функцію, яку можна ранжувати
                        unsigned int index_deleted_function = RANG_D_OR1 + index_of_d_or;

                        if (index_deleted_function == current_ekran.index_position)
                        {
                          found_new_index = 0;
                          current_ekran.index_position--;
                        }
                      }
                      else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_XOR1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_XOR1 + NUMBER_DEFINED_XOR - 1)))
                      {
                        unsigned int index_of_d_xor = current_ekran.current_level - EKRAN_RANGUVANNJA_D_XOR1;

                        //Першою перевіряємо функцію з меншим номером, щоб за одну операцію циклу  можна було переміститися на функцію, яку можна ранжувати
                        unsigned int index_deleted_function = RANG_D_XOR1 + index_of_d_xor;

                        if (index_deleted_function == current_ekran.index_position)
                        {
                          found_new_index = 0;
                          current_ekran.index_position--;
                        }
                      }
                      else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_NOT1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_NOT1 + NUMBER_DEFINED_NOT - 1)))
                      {
                        unsigned int index_of_d_not = current_ekran.current_level - EKRAN_RANGUVANNJA_D_NOT1;

                        //Першою перевіряємо функцію з меншим номером, щоб за одну операцію циклу  можна було переміститися на функцію, яку можна ранжувати
                        unsigned int index_deleted_function = RANG_D_NOT1 + index_of_d_not;

                        if (index_deleted_function == current_ekran.index_position)
                        {
                          found_new_index = 0;
                          current_ekran.index_position--;
                        }
                      }

                      //Перевіряємо, чи ми не вийшли за допустиму кількість функцій
                      if (current_ekran.index_position < 0)
                      {
                        found_new_index = 0;
                        current_ekran.index_position = NUMBER_TOTAL_SIGNAL_FOR_RANG - 1;
                      }
                    }
                  }
                  position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;

                  //Формуємо екран відображення зранжованих функцій
                  if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_OUTPUT_1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_OUTPUT_1 + NUMBER_OUTPUTS - 1)))
                    make_ekran_set_function_in_output_led_df_dt_reg(current_ekran.current_level, INDEX_VIEWING_OUTPUT, temp_state);
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_LED_1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_LED_1 + NUMBER_LEDS - 1)))
                    make_ekran_set_function_in_output_led_df_dt_reg(current_ekran.current_level, INDEX_VIEWING_LED, temp_state);
                  else if (current_ekran.current_level == EKRAN_RANGUVANNJA_ANALOG_REGISTRATOR)
                    make_ekran_set_function_in_output_led_df_dt_reg(current_ekran.current_level, INDEX_VIEWING_A_REG, temp_state);
                  else if (current_ekran.current_level == EKRAN_RANGUVANNJA_DIGITAL_REGISTRATOR)
                    make_ekran_set_function_in_output_led_df_dt_reg(current_ekran.current_level, INDEX_VIEWING_D_REG, temp_state);
                  else if (current_ekran.current_level == EKRAN_RANGUVANNJA_OFF_CB)
                    make_ekran_set_function_in_output_led_df_dt_reg(current_ekran.current_level, INDEX_VIEWING_OFF_CB, temp_state);
                  else if (current_ekran.current_level == EKRAN_RANGUVANNJA_ON_CB)
                    make_ekran_set_function_in_output_led_df_dt_reg(current_ekran.current_level, INDEX_VIEWING_ON_CB, temp_state);
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_DF1_PLUS) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_DF1_PLUS + MAX_ROW_LIST_TYPE_SOURCE_DF * NUMBER_DEFINED_FUNCTIONS - 1)))
                    make_ekran_set_function_in_output_led_df_dt_reg(current_ekran.current_level, INDEX_VIEWING_DF, temp_state);
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_SET_DT1_PLUS) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_SET_DT1_PLUS + 4 * NUMBER_DEFINED_TRIGGERS - 1)))
                    make_ekran_set_function_in_output_led_df_dt_reg(current_ekran.current_level, INDEX_VIEWING_DT, temp_state);
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_AND1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_AND1 + NUMBER_DEFINED_AND - 1)))
                    make_ekran_set_function_in_output_led_df_dt_reg(current_ekran.current_level, INDEX_VIEWING_D_AND, temp_state);
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_OR1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_OR1 + NUMBER_DEFINED_OR - 1)))
                    make_ekran_set_function_in_output_led_df_dt_reg(current_ekran.current_level, INDEX_VIEWING_D_OR, temp_state);
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_XOR1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_XOR1 + NUMBER_DEFINED_XOR - 1)))
                    make_ekran_set_function_in_output_led_df_dt_reg(current_ekran.current_level, INDEX_VIEWING_D_XOR, temp_state);
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_NOT1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_NOT1 + NUMBER_DEFINED_NOT - 1)))
                    make_ekran_set_function_in_output_led_df_dt_reg(current_ekran.current_level, INDEX_VIEWING_D_NOT, temp_state);
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                  else if (current_ekran.current_level == EKRAN_LN_FOR_IEC61850_RANG)
                    make_ekran_set_function_in_output_led_df_dt_reg(current_ekran.current_level, INDEX_VIEWING_IEC61850_RANG, temp_state);
#endif
                }

                //Очистити сигналізацію, що натиснута кнопка
                new_state_keyboard &= ~(1u << BIT_KEY_UP);
              }
              else if (new_state_keyboard == (1u << BIT_KEY_DOWN))
              {
                //Натиснута кнопка DOWN
                if (
                  ((current_ekran.current_level >= EKRAN_RANGUVANNJA_BUTTON_1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_BUTTON_1 + NUMBER_DEFINED_BUTTONS - 1))) ||
                  ((current_ekran.current_level >= EKRAN_RANGUVANNJA_INPUT_1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_INPUT_1 + NUMBER_INPUTS - 1)))
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                  ||
                  (current_ekran.current_level == EKRAN_LN_FOR_IEC61850_RANG_SMALL)
#endif
                )
                {
                  unsigned int temp_state[N_SMALL];
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                  int type_LN = -1;
                  int n_LN = -1;
#endif

                  if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_BUTTON_1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_BUTTON_1 + NUMBER_DEFINED_BUTTONS - 1)))
                  {
                    uint32_t *p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_buttons : edition_settings.ranguvannja_buttons;
                    for (size_t i = 0; i < N_SMALL; i++)
                      temp_state[i] = p_rang[N_SMALL * (current_ekran.current_level - EKRAN_RANGUVANNJA_BUTTON_1) + i];
                  }
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_INPUT_1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_INPUT_1 + NUMBER_INPUTS - 1)))
                  {
                    uint32_t *p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_inputs : edition_settings.ranguvannja_inputs;
                    for (size_t i = 0; i < N_SMALL; i++)
                      temp_state[i] = p_rang[N_SMALL * (current_ekran.current_level - EKRAN_RANGUVANNJA_INPUT_1) + i];
                  }
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                  else if (current_ekran.current_level == EKRAN_LN_FOR_IEC61850_RANG_SMALL)
                  {
                    int prev_ekran = previous_level_in_current_level_menu[EKRAN_LN_FOR_IEC61850_RANG_SMALL];
                    if ((prev_ekran >= EKRAN_IN_GOOSE1) && (prev_ekran <= EKRAN_IN_MMS4))
                    {
                      int n_out_in = position_in_current_level_menu[prev_ekran];

                      prev_ekran = previous_level_in_current_level_menu[prev_ekran];
                      if (
                        (n_out_in >= 0) &&
                        ((prev_ekran >= EKRAN_LIST_IN_GOOSE) || (prev_ekran <= EKRAN_LIST_IN_MMS)))
                      {
                        n_LN = position_in_current_level_menu[prev_ekran];

                        prev_ekran = previous_level_in_current_level_menu[prev_ekran];
                        if (
                          (n_LN >= 0) &&
                          (prev_ekran == EKRAN_LIST_TYPE_IEC61850_NODES))
                        {
                          type_LN = position_in_current_level_menu[prev_ekran];

                          if (
                            (type_LN >= 0) &&
                            (((type_LN == INDEX_TYPE_IEC61850_IN_GOOSE) && (n_LN < N_IN_GOOSE) && (n_out_in < N_IN_GOOSE_MMS_OUT)) ||
                             ((type_LN == INDEX_TYPE_IEC61850_IN_MMS) && (n_LN < N_IN_MMS) && (n_out_in < N_IN_GOOSE_MMS_OUT))))
                          {
                            uint32_t *p_rang = NULL;
                            switch (type_LN)
                            {
                              case 0:
                                {
                                  p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_In_GOOSE[n_LN][n_out_in] : edition_settings.ranguvannja_In_GOOSE[n_LN][n_out_in];
                                  break;
                                }
                              case 1:
                                {
                                  p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_In_MMS[n_LN][n_out_in] : edition_settings.ranguvannja_In_MMS[n_LN][n_out_in];
                                  break;
                                }
                              default:
                                {
                                  //Теоретично цього ніколи не мало б бути
                                  total_error_sw_fixed();
                                  break;
                                }
                            }

                            for (size_t i = 0; i < N_SMALL; i++)
                              temp_state[i] = p_rang[i];
                          }
                          else
                            total_error_sw_fixed();
                        }
                        else
                          total_error_sw_fixed();
                      }
                      else
                        total_error_sw_fixed();
                    }
                    else
                      total_error_sw_fixed();
                  }
#endif

                  if (current_ekran.edition == 0)
                  {
                    unsigned int comp = true;
                    for (size_t i = 0; ((comp == true) && (i < N_SMALL)); ++i)
                    {
                      comp &= (temp_state[i] == 0);
                    }

                    if (comp)
                      current_ekran.index_position = 0;
                    else
                    {
                      if (++current_ekran.index_position >= NUMBER_TOTAL_SIGNAL_FOR_RANG_SMALL)
                        current_ekran.index_position = 0;
                      // (x>>5) аналогічне операції x / 32 - ціла частина від ділення на 32
                      // (x & 0x1f) аналогічне операції x % 32 - остача від ділення на 32
                      while ((temp_state[current_ekran.index_position >> 5] & (1 << (current_ekran.index_position & 0x1f))) == 0)
                      {
                        current_ekran.index_position++;
                        if (current_ekran.index_position >= NUMBER_TOTAL_SIGNAL_FOR_RANG_SMALL)
                          current_ekran.index_position = 0;
                      }
                    }
                  }
                  else
                  {
                    //Редагування
                    unsigned int found_new_index = 0;
                    // int add_filter[0 + 1] =
                    //   {
                    //     -1 /*признак завершення масиву*/
                    //   };
                    //                  EL_FILTER_STRUCT el_filter[NUMBER_DEFINED_ELEMENTS] =
                    //                  {
                    //                    {1, RANG_SMALL_DF1_IN , RANG_SMALL_DF8_IN   , 1, current_settings.number_defined_df },
                    //                    {1, RANG_SMALL_DT1_SET, RANG_SMALL_DT4_RESET, 2, current_settings.number_defined_dt },
                    //                    {0, 0                 , 0                   , 1, current_settings.number_defined_and},
                    //                    {0, 0                 , 0                   , 1, current_settings.number_defined_or },
                    //                    {0, 0                 , 0                   , 1, current_settings.number_defined_xor},
                    //                    {0, 0                 , 0                   , 1, current_settings.number_defined_not}
                    //                  };

                    //Переміщаємося на наступну функцію
                    if (++current_ekran.index_position >= NUMBER_TOTAL_SIGNAL_FOR_RANG_SMALL)
                      current_ekran.index_position = 0;
                    //Перевіряємо, чи даний індекс функції присутній у даній конфігурації
                    while (found_new_index == 0)
                    {
                      check_current_index_is_presented_in_configuration(&found_new_index,
                                                                        /* add_filter */ NULL,
                                                                        /*el_filter,*/
                                                                        1,
                                                                        NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL,
                                                                        NUMBER_RPN_SIGNAL_FOR_RANG_SMALL,
                                                                        NUMBER_SZKh_SIGNAL_FOR_RANG_SMALL,
                                                                        NUMBER_SNKh_SIGNAL_FOR_RANG_SMALL,
                                                                        NUMBER_BRP_SIGNAL_FOR_RANG_SMALL,
                                                                        NUMBER_UMAX_SIGNAL_FOR_RANG_SMALL,
                                                                        NUMBER_UMIN_SIGNAL_FOR_RANG_SMALL,
                                                                        NUMBER_UP_SIGNAL_FOR_RANG_SMALL,
                                                                        NUMBER_EL_SIGNAL_FOR_RANG_SMALL);

                      //Перевіряємо режим рооботи функціональної кнопки (якщо іде редагування ФК)
                      if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_BUTTON_1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_BUTTON_1 + NUMBER_DEFINED_BUTTONS - 1)))
                      {
                        uint32_t mode = (current_settings.buttons_mode >> (current_ekran.current_level - EKRAN_RANGUVANNJA_BUTTON_1)) & 0x1;
                        if (_CHECK_SET_BIT(buttons_mode[mode], current_ekran.index_position) == 0)
                        {
                          found_new_index = 0;
                          current_ekran.index_position++;
                        }
                      }
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                      else if (current_ekran.current_level == EKRAN_LN_FOR_IEC61850_RANG_SMALL)
                      {
                        if ((type_LN == INDEX_TYPE_IEC61850_IN_GOOSE) || (type_LN == INDEX_TYPE_IEC61850_IN_MMS))
                        {
                          if (
                            (_CHECK_SET_BIT(rang_iec61850_blocks[type_LN], current_ekran.index_position) == 0) ||
                            ((type_LN == INDEX_TYPE_IEC61850_IN_GOOSE) && (current_ekran.index_position == (RANG_SMALL_BLOCK_IN_GOOSE1 + n_LN))) ||
                            ((type_LN == INDEX_TYPE_IEC61850_IN_MMS) && (current_ekran.index_position == (RANG_SMALL_BLOCK_IN_MMS1 + n_LN))))
                          {
                            found_new_index = 0;
                            current_ekran.index_position++;
                          }
                        }
                        else
                          total_error_sw_fixed();
                      }
#endif

                      //Перевіряємо, чи ми не вийшли за допустиму кількість функцій
                      if (current_ekran.index_position >= NUMBER_TOTAL_SIGNAL_FOR_RANG_SMALL)
                      {
                        found_new_index = 0;
                        current_ekran.index_position = 0;
                      }
                    }
                  }
                  position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;

                  //Формуємо екран відображення зранжованих функцій
                  if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_BUTTON_1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_BUTTON_1 + NUMBER_DEFINED_BUTTONS - 1)))
                    make_ekran_set_function_in_bi(current_ekran.current_level, INDEX_VIEWING_BUTTON, temp_state
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                                                  ,
                                                  type_LN, n_LN
#endif
                    );
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_INPUT_1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_INPUT_1 + NUMBER_INPUTS - 1)))
                    make_ekran_set_function_in_bi(current_ekran.current_level, INDEX_VIEWING_INPUT, temp_state
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                                                  ,
                                                  type_LN, n_LN
#endif
                    );

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                  else if (current_ekran.current_level == EKRAN_LN_FOR_IEC61850_RANG_SMALL)
                    make_ekran_set_function_in_bi(current_ekran.current_level, INDEX_VIEWING_IEC61850_RANG, temp_state, type_LN, n_LN);
#endif
                }
                else if (
                  ((current_ekran.current_level >= EKRAN_RANGUVANNJA_OUTPUT_1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_OUTPUT_1 + NUMBER_OUTPUTS - 1))) ||
                  ((current_ekran.current_level >= EKRAN_RANGUVANNJA_LED_1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_LED_1 + NUMBER_LEDS - 1))) ||
                  (current_ekran.current_level == EKRAN_RANGUVANNJA_ANALOG_REGISTRATOR) ||
                  (current_ekran.current_level == EKRAN_RANGUVANNJA_DIGITAL_REGISTRATOR) ||
                  (current_ekran.current_level == EKRAN_RANGUVANNJA_OFF_CB) ||
                  (current_ekran.current_level == EKRAN_RANGUVANNJA_ON_CB) ||
                  ((current_ekran.current_level >= EKRAN_RANGUVANNJA_DF1_PLUS) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_DF1_PLUS + MAX_ROW_LIST_TYPE_SOURCE_DF * NUMBER_DEFINED_FUNCTIONS - 1))) ||
                  ((current_ekran.current_level >= EKRAN_RANGUVANNJA_SET_DT1_PLUS) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_SET_DT1_PLUS + 4 * NUMBER_DEFINED_TRIGGERS - 1))) ||
                  ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_AND1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_AND1 + NUMBER_DEFINED_AND - 1))) ||
                  ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_OR1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_OR1 + NUMBER_DEFINED_OR - 1))) ||
                  ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_XOR1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_XOR1 + NUMBER_DEFINED_XOR - 1))) ||
                  ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_NOT1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_NOT1 + NUMBER_DEFINED_NOT - 1)))
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                  ||
                  (current_ekran.current_level == EKRAN_LN_FOR_IEC61850_RANG)
#endif
                )
                {
                  unsigned int temp_state[N_BIG];

                  if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_OUTPUT_1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_OUTPUT_1 + NUMBER_OUTPUTS - 1)))
                  {
                    uint32_t *p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_outputs : edition_settings.ranguvannja_outputs;
                    for (size_t i = 0; i < N_BIG; i++)
                      temp_state[i] = p_rang[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_OUTPUT_1) + i];
                  }
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_LED_1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_LED_1 + NUMBER_LEDS - 1)))
                  {
                    uint32_t *p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_leds : edition_settings.ranguvannja_leds;
                    for (size_t i = 0; i < N_BIG; i++)
                      temp_state[i] = p_rang[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_LED_1) + i];
                  }
                  else if (current_ekran.current_level == EKRAN_RANGUVANNJA_ANALOG_REGISTRATOR)
                  {
                    uint32_t *p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_analog_registrator : edition_settings.ranguvannja_analog_registrator;
                    for (size_t i = 0; i < N_BIG; i++)
                      temp_state[i] = p_rang[i];
                  }
                  else if (current_ekran.current_level == EKRAN_RANGUVANNJA_DIGITAL_REGISTRATOR)
                  {
                    uint32_t *p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_digital_registrator : edition_settings.ranguvannja_digital_registrator;
                    for (size_t i = 0; i < N_BIG; i++)
                      temp_state[i] = p_rang[i];
                  }
                  else if (current_ekran.current_level == EKRAN_RANGUVANNJA_OFF_CB)
                  {
                    uint32_t *p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_off_cb : edition_settings.ranguvannja_off_cb;
                    for (size_t i = 0; i < N_BIG; i++)
                      temp_state[i] = p_rang[i];
                  }
                  else if (current_ekran.current_level == EKRAN_RANGUVANNJA_ON_CB)
                  {
                    uint32_t *p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_on_cb : edition_settings.ranguvannja_on_cb;
                    for (size_t i = 0; i < N_BIG; i++)
                      temp_state[i] = p_rang[i];
                  }
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_DF1_PLUS) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_DF1_PLUS + MAX_ROW_LIST_TYPE_SOURCE_DF * NUMBER_DEFINED_FUNCTIONS - 1)))
                  {
                    unsigned int index_in_ekran_list = current_ekran.current_level - EKRAN_RANGUVANNJA_DF1_PLUS;
                    unsigned int type_source = index_in_ekran_list % MAX_ROW_LIST_TYPE_SOURCE_DF;
                    unsigned int index_of_df = index_in_ekran_list / MAX_ROW_LIST_TYPE_SOURCE_DF;

                    uint32_t *p_rang = NULL;
                    switch (type_source)
                    {
                      case INDEX_ML_LIST_TYPE_SOURCE_PLUS_DF:
                        {
                          p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_df_source_plus : edition_settings.ranguvannja_df_source_plus;
                          break;
                        }
                      case INDEX_ML_LIST_TYPE_SOURCE_MINUS_DF:
                        {
                          p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_df_source_minus : edition_settings.ranguvannja_df_source_minus;
                          break;
                        }
                      case INDEX_ML_LIST_TYPE_SOURCE_BLK_DF:
                        {
                          p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_df_source_blk : edition_settings.ranguvannja_df_source_blk;
                          break;
                        }
                      case INDEX_ML_LIST_TYPE_SOURCE_R_DF:
                        {
                          p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_df_source_reset : edition_settings.ranguvannja_df_source_reset;
                          break;
                        }
                      default:
                        {
                          total_error_sw_fixed();
                        }
                    }

                    for (size_t i = 0; i < N_BIG; i++)
                      temp_state[i] = p_rang[N_BIG * index_of_df + i];
                  }
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_SET_DT1_PLUS) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_SET_DT1_PLUS + 4 * NUMBER_DEFINED_TRIGGERS - 1)))
                  {
                    unsigned int index_in_ekran_list = current_ekran.current_level - EKRAN_RANGUVANNJA_SET_DT1_PLUS;
                    unsigned int type_source = index_in_ekran_list % 2;
                    unsigned int type_of_action = (index_in_ekran_list / 2) & 0x1;
                    unsigned int index_of_dt = index_in_ekran_list / 4;

                    uint32_t *p_rang = NULL;
                    switch (type_of_action)
                    {
                      case INDEX_ML_SET_DT:
                        {
                          switch (type_source)
                          {
                            case INDEX_ML_LIST_TYPE_SOURCE_PLUS_DT:
                              {
                                p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_set_dt_source_plus : edition_settings.ranguvannja_set_dt_source_plus;
                                break;
                              }
                            case INDEX_ML_LIST_TYPE_SOURCE_MINUS_DT:
                              {
                                p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_set_dt_source_minus : edition_settings.ranguvannja_set_dt_source_minus;
                                break;
                              }
                            default:
                              {
                                total_error_sw_fixed();
                              }
                          }
                          break;
                        }
                      case INDEX_ML_RESET_DT:
                        {
                          switch (type_source)
                          {
                            case INDEX_ML_LIST_TYPE_SOURCE_PLUS_DT:
                              {
                                p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_reset_dt_source_plus : edition_settings.ranguvannja_reset_dt_source_plus;
                                break;
                              }
                            case INDEX_ML_LIST_TYPE_SOURCE_MINUS_DT:
                              {
                                p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_reset_dt_source_minus : edition_settings.ranguvannja_reset_dt_source_minus;
                                break;
                              }
                            default:
                              {
                                total_error_sw_fixed();
                              }
                          }
                          break;
                        }
                      default:
                        {
                          total_error_sw_fixed();
                        }
                    }

                    for (size_t i = 0; i < N_BIG; i++)
                      temp_state[i] = p_rang[N_BIG * index_of_dt + i];
                  }
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_AND1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_AND1 + NUMBER_DEFINED_AND - 1)))
                  {
                    uint32_t *p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_d_and : edition_settings.ranguvannja_d_and;
                    for (size_t i = 0; i < N_BIG; i++)
                      temp_state[i] = p_rang[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_D_AND1) + i];
                  }
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_OR1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_OR1 + NUMBER_DEFINED_OR - 1)))
                  {
                    uint32_t *p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_d_or : edition_settings.ranguvannja_d_or;
                    for (size_t i = 0; i < N_BIG; i++)
                      temp_state[i] = p_rang[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_D_OR1) + i];
                  }
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_XOR1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_XOR1 + NUMBER_DEFINED_XOR - 1)))
                  {
                    uint32_t *p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_d_xor : edition_settings.ranguvannja_d_xor;
                    for (size_t i = 0; i < N_BIG; i++)
                      temp_state[i] = p_rang[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_D_XOR1) + i];
                  }
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_NOT1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_NOT1 + NUMBER_DEFINED_NOT - 1)))
                  {
                    uint32_t *p_rang = (current_ekran.edition == 0) ? current_settings.ranguvannja_d_not : edition_settings.ranguvannja_d_not;
                    for (size_t i = 0; i < N_BIG; i++)
                      temp_state[i] = p_rang[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_D_NOT1) + i];
                  }
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                  else if (current_ekran.current_level == EKRAN_LN_FOR_IEC61850_RANG)
                  {
                    int prev_ekran = previous_level_in_current_level_menu[EKRAN_LN_FOR_IEC61850_RANG];
                    if ((prev_ekran >= EKRAN_OUT_LAN1) && (prev_ekran <= EKRAN_OUT_LAN4))
                    {
                      int n_out_in = position_in_current_level_menu[prev_ekran];

                      prev_ekran = previous_level_in_current_level_menu[prev_ekran];
                      if (
                        (n_out_in >= 0) &&
                        (prev_ekran == EKRAN_LIST_OUT_LAN))
                      {
                        int n_LN = position_in_current_level_menu[prev_ekran];

                        if ((n_LN < N_OUT_LAN) && (n_out_in < N_OUT_LAN_IN))
                        {
                          if (current_ekran.edition == 0)
                          {
                            uint16_t *p_rang = current_settings.ranguvannja_Out_LAN[n_LN][n_out_in];

                            for (size_t i = 0; i < N_BIG; i++)
                              temp_state[i] = 0;
                            for (size_t i = 0; i < MAX_FUNCTIONS_IN_OUT_LAN; i++)
                            {
                              uint16_t val = p_rang[i];
                              if (val > 0)
                                _SET_BIT(temp_state, (p_rang[i] - 1));
                              else
                                break;
                            }
                          }
                          else
                          {
                            for (size_t i = 0; i < N_BIG; i++)
                              temp_state[i] = edit_rang_Out_LAN[i];
                          }
                        }
                        else
                          total_error_sw_fixed();
                      }
                      else
                        total_error_sw_fixed();
                    }
                    else
                      total_error_sw_fixed();
                  }
#endif

                  if (current_ekran.edition == 0)
                  {
                    unsigned int comp = true;
                    for (size_t i = 0; ((comp == true) && (i < N_BIG)); ++i)
                    {
                      comp &= (temp_state[i] == 0);
                    }
                    if (comp == true)
                      current_ekran.index_position = 0;
                    else
                    {
                      if (++current_ekran.index_position >= NUMBER_TOTAL_SIGNAL_FOR_RANG)
                        current_ekran.index_position = 0;
                      // (x>>5) аналогічне операції x / 32 - ціла частина від ділення на 32
                      // (x & 0x1f) аналогічне операції x % 32 - остача від ділення на 32
                      while ((temp_state[current_ekran.index_position >> 5] & (1 << (current_ekran.index_position & 0x1f))) == 0)
                      {
                        current_ekran.index_position++;
                        if (current_ekran.index_position >= NUMBER_TOTAL_SIGNAL_FOR_RANG)
                          current_ekran.index_position = 0;
                      }
                    }
                  }
                  else
                  {
                    //Редагування
                    unsigned int found_new_index = 0;
                    // int add_filter[3 + 1] =
                    //   {
                    //     RANG_PO_NZZ,
                    //     RANG_NZZ,
                    //     RANG_SECTOR_NZZ,
                    //     -1 /*признак завершення масиву*/
                    //   };
                    //                  EL_FILTER_STRUCT el_filter[NUMBER_DEFINED_ELEMENTS] =
                    //                  {
                    //                    {1, RANG_DF1_IN , RANG_DF8_OUT, 2, current_settings.number_defined_df },
                    //                    {1, RANG_DT1_SET, RANG_DT4_OUT, 3, current_settings.number_defined_dt },
                    //                    {1, RANG_D_AND1 , RANG_D_AND8 , 1, current_settings.number_defined_and},
                    //                    {1, RANG_D_OR1  , RANG_D_OR8  , 1, current_settings.number_defined_or },
                    //                    {1, RANG_D_XOR1 , RANG_D_XOR8 , 1, current_settings.number_defined_xor},
                    //                    {1, RANG_D_NOT1 , RANG_D_NOT16, 1, current_settings.number_defined_not}
                    //                  };

                    //Переміщаємося на наступну функцію
                    if (++current_ekran.index_position >= NUMBER_TOTAL_SIGNAL_FOR_RANG)
                      current_ekran.index_position = 0;
                    //Перевіряємо, чи даний індекс функції присутній у даній конфігурації
                    while (found_new_index == 0)
                    {
                      check_current_index_is_presented_in_configuration(&found_new_index,
                                                                        /* add_filter */ NULL,
                                                                        /*el_filter,*/
                                                                        1,
                                                                        NUMBER_GENERAL_SIGNAL_FOR_RANG,
                                                                        NUMBER_RPN_SIGNAL_FOR_RANG,
                                                                        NUMBER_SZKh_SIGNAL_FOR_RANG,
                                                                        NUMBER_SNKh_SIGNAL_FOR_RANG,
                                                                        NUMBER_BRP_SIGNAL_FOR_RANG,
                                                                        NUMBER_UMAX_SIGNAL_FOR_RANG,
                                                                        NUMBER_UMIN_SIGNAL_FOR_RANG,
                                                                        NUMBER_UP_SIGNAL_FOR_RANG,
                                                                        NUMBER_EL_SIGNAL_FOR_RANG);

                      //Перевіряємо, чи ми не  на індексі функцій із списку загальних, яку треба викинути для даного типу ранжування
                      if (
                        (current_ekran.current_level == EKRAN_RANGUVANNJA_ANALOG_REGISTRATOR) ||
                        (current_ekran.current_level == EKRAN_RANGUVANNJA_DIGITAL_REGISTRATOR) ||
                        (current_ekran.current_level == EKRAN_RANGUVANNJA_OFF_CB) ||
                        (current_ekran.current_level == EKRAN_RANGUVANNJA_ON_CB))
                      {
                        size_t number = 1;
                        for (size_t num = 0; num < number; ++num)
                        {
                          unsigned int index_deleted_function = 0;

                          if (current_ekran.current_level == EKRAN_RANGUVANNJA_ANALOG_REGISTRATOR)
                            index_deleted_function = RANG_WORK_A_REJESTRATOR;
                          else if (current_ekran.current_level == EKRAN_RANGUVANNJA_DIGITAL_REGISTRATOR)
                            index_deleted_function = RANG_WORK_D_REJESTRATOR;
                          else if (current_ekran.current_level == EKRAN_RANGUVANNJA_OFF_CB)
                          {
                            number = 2;

                            if (num == 0)
                              index_deleted_function = RANG_VIDKL_VID_ZAKHYSTIV;
                            else
                              index_deleted_function = RANG_WORK_BO;
                          }
                          else if (current_ekran.current_level == EKRAN_RANGUVANNJA_ON_CB)
                            index_deleted_function = RANG_WORK_BV;

                          if (index_deleted_function == current_ekran.index_position)
                          {
                            found_new_index = 0;
                            current_ekran.index_position++;
                          }
                        }
                      }
                      else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_DF1_PLUS) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_DF1_PLUS + MAX_ROW_LIST_TYPE_SOURCE_DF * NUMBER_DEFINED_FUNCTIONS - 1)))
                      {
                        unsigned int index_in_ekran_list = current_ekran.current_level - EKRAN_RANGUVANNJA_DF1_PLUS;
                        unsigned int index_of_df = index_in_ekran_list / MAX_ROW_LIST_TYPE_SOURCE_DF;

                        for (unsigned int i = 0; i < 3; i++)
                        {
                          //Першою перевіряємо функцію з меншим номером, щоб за одну операцію циклу можна було переміститися на функцію, яку можна ранжувати
                          unsigned int const index_deleted_function = RANG_DF1_IN + 3 /*кількість сигналів*/ * index_of_df + i;

                          if (index_deleted_function == current_ekran.index_position)
                          {
                            found_new_index = 0;
                            current_ekran.index_position++;
                          }
                        }
                      }
                      else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_SET_DT1_PLUS) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_SET_DT1_PLUS + 4 * NUMBER_DEFINED_TRIGGERS - 1)))
                      {
                        unsigned int index_in_ekran_list = current_ekran.current_level - EKRAN_RANGUVANNJA_SET_DT1_PLUS;
                        unsigned int index_of_dt = index_in_ekran_list / 4;

                        for (unsigned int i = 0; i < 3; i++)
                        {
                          unsigned int index_deleted_function = 0;
                          //Першою перевіряємо функцію з меншим номером, щоб за одну операцію циклу  можна було переміститися на функцію, яку можна ранжувати
                          if (i == 0)
                          {
                            if (index_of_dt == 0)
                              index_deleted_function = RANG_DT1_SET;
                            else if (index_of_dt == 1)
                              index_deleted_function = RANG_DT2_SET;
                            else if (index_of_dt == 2)
                              index_deleted_function = RANG_DT3_SET;
                            else if (index_of_dt == 3)
                              index_deleted_function = RANG_DT4_SET;
                          }
                          else if (i == 1)
                          {
                            if (index_of_dt == 0)
                              index_deleted_function = RANG_DT1_RESET;
                            else if (index_of_dt == 1)
                              index_deleted_function = RANG_DT2_RESET;
                            else if (index_of_dt == 2)
                              index_deleted_function = RANG_DT3_RESET;
                            else if (index_of_dt == 3)
                              index_deleted_function = RANG_DT4_RESET;
                          }
                          else
                          {
                            if (index_of_dt == 0)
                              index_deleted_function = RANG_DT1_OUT;
                            else if (index_of_dt == 1)
                              index_deleted_function = RANG_DT2_OUT;
                            else if (index_of_dt == 2)
                              index_deleted_function = RANG_DT3_OUT;
                            else if (index_of_dt == 3)
                              index_deleted_function = RANG_DT4_OUT;
                          }

                          if (index_deleted_function == current_ekran.index_position)
                          {
                            found_new_index = 0;
                            current_ekran.index_position++;
                          }
                        }
                      }
                      else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_AND1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_AND1 + NUMBER_DEFINED_AND - 1)))
                      {
                        unsigned int index_of_d_and = current_ekran.current_level - EKRAN_RANGUVANNJA_D_AND1;

                        //Першою перевіряємо функцію з меншим номером, щоб за одну операцію циклу  можна було переміститися на функцію, яку можна ранжувати
                        unsigned int index_deleted_function = RANG_D_AND1 + index_of_d_and;

                        if (index_deleted_function == current_ekran.index_position)
                        {
                          found_new_index = 0;
                          current_ekran.index_position++;
                        }
                      }
                      else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_OR1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_OR1 + NUMBER_DEFINED_OR - 1)))
                      {
                        unsigned int index_of_d_or = current_ekran.current_level - EKRAN_RANGUVANNJA_D_OR1;

                        //Першою перевіряємо функцію з меншим номером, щоб за одну операцію циклу  можна було переміститися на функцію, яку можна ранжувати
                        unsigned int index_deleted_function = RANG_D_OR1 + index_of_d_or;

                        if (index_deleted_function == current_ekran.index_position)
                        {
                          found_new_index = 0;
                          current_ekran.index_position++;
                        }
                      }
                      else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_XOR1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_XOR1 + NUMBER_DEFINED_XOR - 1)))
                      {
                        unsigned int index_of_d_xor = current_ekran.current_level - EKRAN_RANGUVANNJA_D_XOR1;

                        //Першою перевіряємо функцію з меншим номером, щоб за одну операцію циклу  можна було переміститися на функцію, яку можна ранжувати
                        unsigned int index_deleted_function = RANG_D_XOR1 + index_of_d_xor;

                        if (index_deleted_function == current_ekran.index_position)
                        {
                          found_new_index = 0;
                          current_ekran.index_position++;
                        }
                      }
                      else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_NOT1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_NOT1 + NUMBER_DEFINED_NOT - 1)))
                      {
                        unsigned int index_of_d_not = current_ekran.current_level - EKRAN_RANGUVANNJA_D_NOT1;

                        //Першою перевіряємо функцію з меншим номером, щоб за одну операцію циклу  можна було переміститися на функцію, яку можна ранжувати
                        unsigned int index_deleted_function = RANG_D_NOT1 + index_of_d_not;

                        if (index_deleted_function == current_ekran.index_position)
                        {
                          found_new_index = 0;
                          current_ekran.index_position++;
                        }
                      }

                      //Перевіряємо, чи ми не вийшли за допустиму кількість функцій
                      if (current_ekran.index_position >= NUMBER_TOTAL_SIGNAL_FOR_RANG)
                      {
                        found_new_index = 0;
                        current_ekran.index_position = 0;
                      }
                    }
                  }
                  position_in_current_level_menu[current_ekran.current_level] = current_ekran.index_position;

                  //Формуємо екран відображення зранжованих функцій
                  if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_OUTPUT_1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_OUTPUT_1 + NUMBER_OUTPUTS - 1)))
                    make_ekran_set_function_in_output_led_df_dt_reg(current_ekran.current_level, INDEX_VIEWING_OUTPUT, temp_state);
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_LED_1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_LED_1 + NUMBER_LEDS - 1)))
                    make_ekran_set_function_in_output_led_df_dt_reg(current_ekran.current_level, INDEX_VIEWING_LED, temp_state);
                  else if (current_ekran.current_level == EKRAN_RANGUVANNJA_ANALOG_REGISTRATOR)
                    make_ekran_set_function_in_output_led_df_dt_reg(current_ekran.current_level, INDEX_VIEWING_A_REG, temp_state);
                  else if (current_ekran.current_level == EKRAN_RANGUVANNJA_DIGITAL_REGISTRATOR)
                    make_ekran_set_function_in_output_led_df_dt_reg(current_ekran.current_level, INDEX_VIEWING_D_REG, temp_state);
                  else if (current_ekran.current_level == EKRAN_RANGUVANNJA_OFF_CB)
                    make_ekran_set_function_in_output_led_df_dt_reg(current_ekran.current_level, INDEX_VIEWING_OFF_CB, temp_state);
                  else if (current_ekran.current_level == EKRAN_RANGUVANNJA_ON_CB)
                    make_ekran_set_function_in_output_led_df_dt_reg(current_ekran.current_level, INDEX_VIEWING_ON_CB, temp_state);
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_DF1_PLUS) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_DF1_PLUS + MAX_ROW_LIST_TYPE_SOURCE_DF * NUMBER_DEFINED_FUNCTIONS - 1)))
                    make_ekran_set_function_in_output_led_df_dt_reg(current_ekran.current_level, INDEX_VIEWING_DF, temp_state);
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_SET_DT1_PLUS) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_SET_DT1_PLUS + 4 * NUMBER_DEFINED_TRIGGERS - 1)))
                    make_ekran_set_function_in_output_led_df_dt_reg(current_ekran.current_level, INDEX_VIEWING_DT, temp_state);
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_AND1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_AND1 + NUMBER_DEFINED_AND - 1)))
                    make_ekran_set_function_in_output_led_df_dt_reg(current_ekran.current_level, INDEX_VIEWING_D_AND, temp_state);
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_OR1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_OR1 + NUMBER_DEFINED_OR - 1)))
                    make_ekran_set_function_in_output_led_df_dt_reg(current_ekran.current_level, INDEX_VIEWING_D_OR, temp_state);
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_XOR1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_XOR1 + NUMBER_DEFINED_XOR - 1)))
                    make_ekran_set_function_in_output_led_df_dt_reg(current_ekran.current_level, INDEX_VIEWING_D_XOR, temp_state);
                  else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_NOT1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_NOT1 + NUMBER_DEFINED_NOT - 1)))
                    make_ekran_set_function_in_output_led_df_dt_reg(current_ekran.current_level, INDEX_VIEWING_D_NOT, temp_state);
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                  else if (current_ekran.current_level == EKRAN_LN_FOR_IEC61850_RANG)
                    make_ekran_set_function_in_output_led_df_dt_reg(current_ekran.current_level, INDEX_VIEWING_IEC61850_RANG, temp_state);
#endif
                }

                //Очистити сигналізацію, що натиснута кнопка
                new_state_keyboard &= ~(1u << BIT_KEY_DOWN);
              }
              else if ((new_state_keyboard == (1u << BIT_KEY_RIGHT)) || (new_state_keyboard == (1u << BIT_KEY_LEFT)))
              {
                //Натиснута кнопка RIGHT або LEFT
                //Міняємо на протилежний відповідний біт для вибраної функції
                if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_BUTTON_1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_BUTTON_1 + NUMBER_DEFINED_BUTTONS - 1)))
                {
                  unsigned int offset, shift;
                  offset = current_ekran.index_position >> 5;        //Це є, фактично, ділення на 32
                  shift = (current_ekran.index_position & (32 - 1)); //Це є, фактично, визначення остачі від ділення на 32

                  uint32_t *p_rang = &edition_settings.ranguvannja_buttons[N_SMALL * (current_ekran.current_level - EKRAN_RANGUVANNJA_BUTTON_1)];
                  *(p_rang + offset) ^= (1 << shift);

                  //Формуємо екран відображення зранжованих функцій
                  make_ekran_set_function_in_bi(current_ekran.current_level, INDEX_VIEWING_BUTTON, p_rang
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                                                ,
                                                -1, -1
#endif
                  );
                }
                else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_INPUT_1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_INPUT_1 + NUMBER_INPUTS - 1)))
                {
                  unsigned int offset, shift;
                  offset = current_ekran.index_position >> 5;        //Це є, фактично, ділення на 32
                  shift = (current_ekran.index_position & (32 - 1)); //Це є, фактично, визначення остачі від ділення на 32

                  uint32_t *p_rang = &edition_settings.ranguvannja_inputs[N_SMALL * (current_ekran.current_level - EKRAN_RANGUVANNJA_INPUT_1)];
                  *(p_rang + offset) ^= (1 << shift);

                  //Формуємо екран відображення зранжованих функцій
                  make_ekran_set_function_in_bi(current_ekran.current_level, INDEX_VIEWING_INPUT, p_rang
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                                                ,
                                                -1, -1
#endif
                  );
                }
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                else if (current_ekran.current_level == EKRAN_LN_FOR_IEC61850_RANG_SMALL)
                {
                  int prev_ekran = previous_level_in_current_level_menu[EKRAN_LN_FOR_IEC61850_RANG_SMALL];
                  if ((prev_ekran >= EKRAN_IN_GOOSE1) && (prev_ekran <= EKRAN_IN_MMS4))
                  {
                    int n_out_in = position_in_current_level_menu[prev_ekran];

                    prev_ekran = previous_level_in_current_level_menu[prev_ekran];
                    if (
                      (n_out_in >= 0) &&
                      ((prev_ekran >= EKRAN_LIST_IN_GOOSE) || (prev_ekran <= EKRAN_LIST_IN_MMS)))
                    {
                      int n_LN = position_in_current_level_menu[prev_ekran];

                      prev_ekran = previous_level_in_current_level_menu[prev_ekran];
                      if (
                        (n_LN >= 0) &&
                        (prev_ekran == EKRAN_LIST_TYPE_IEC61850_NODES))
                      {
                        int type_LN = position_in_current_level_menu[prev_ekran];

                        if (
                          (type_LN >= 0) &&
                          (((type_LN == INDEX_TYPE_IEC61850_IN_GOOSE) && (n_LN < N_IN_GOOSE) && (n_out_in < N_IN_GOOSE_MMS_OUT)) ||
                           ((type_LN == INDEX_TYPE_IEC61850_IN_MMS) && (n_LN < N_IN_MMS) && (n_out_in < N_IN_GOOSE_MMS_OUT))))
                        {
                          uint32_t *p_rang_edit = NULL;
                          switch (type_LN)
                          {
                            case 0:
                              {
                                p_rang_edit = edition_settings.ranguvannja_In_GOOSE[n_LN][n_out_in];
                                break;
                              }
                            case 1:
                              {
                                p_rang_edit = edition_settings.ranguvannja_In_MMS[n_LN][n_out_in];
                                break;
                              }
                            default:
                              {
                                //Теоретично цього ніколи не мало б бути
                                total_error_sw_fixed();
                                break;
                              }
                          }

                          unsigned int offset, shift;
                          offset = current_ekran.index_position >> 5;        //Це є, фактично, ділення на 32
                          shift = (current_ekran.index_position & (32 - 1)); //Це є, фактично, визначення остачі від ділення на 32

                          *(p_rang_edit + offset) ^= (1 << shift);

                          //Формуємо екран відображення зранжованих функцій
                          make_ekran_set_function_in_bi(current_ekran.current_level, INDEX_VIEWING_IEC61850_RANG, p_rang_edit, type_LN, n_LN);
                        }
                        else
                          total_error_sw_fixed();
                      }
                      else
                        total_error_sw_fixed();
                    }
                    else
                      total_error_sw_fixed();
                  }
                  else
                    total_error_sw_fixed();
                }
                else if (current_ekran.current_level == EKRAN_LN_FOR_IEC61850_RANG)
                {
                  unsigned int offset, shift;
                  offset = current_ekran.index_position >> 5;        //Це є, фактично, ділення на 32
                  shift = (current_ekran.index_position & (32 - 1)); //Це є, фактично, визначення остачі від ділення на 32

                  edit_rang_Out_LAN[offset] ^= (1 << shift);

                  make_ekran_set_function_in_output_led_df_dt_reg(current_ekran.current_level, INDEX_VIEWING_IEC61850_RANG, edit_rang_Out_LAN);
                }
#endif
                else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_OUTPUT_1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_OUTPUT_1 + NUMBER_OUTPUTS - 1)))
                {
                  unsigned int offset, shift;
                  offset = current_ekran.index_position >> 5;        //Це є, фактично, ділення на 32
                  shift = (current_ekran.index_position & (32 - 1)); //Це є, фактично, визначення остачі від ділення на 32

                  uint32_t *p_rang = &edition_settings.ranguvannja_outputs[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_OUTPUT_1)];
                  *(p_rang + offset) ^= (1 << shift);

                  //Формуємо екран відображення зранжованих функцій
                  make_ekran_set_function_in_output_led_df_dt_reg(current_ekran.current_level, INDEX_VIEWING_OUTPUT, p_rang);
                }
                else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_LED_1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_LED_1 + NUMBER_LEDS - 1)))
                {
                  unsigned int offset, shift;
                  offset = current_ekran.index_position >> 5;        //Це є, фактично, ділення на 32
                  shift = (current_ekran.index_position & (32 - 1)); //Це є, фактично, визначення остачі від ділення на 32

                  uint32_t *p_rang = &edition_settings.ranguvannja_leds[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_LED_1)];
                  *(p_rang + offset) ^= (1 << shift);

                  //Формуємо екран відображення зранжованих функцій
                  make_ekran_set_function_in_output_led_df_dt_reg(current_ekran.current_level, INDEX_VIEWING_LED, p_rang);
                }
                else if (current_ekran.current_level == EKRAN_RANGUVANNJA_ANALOG_REGISTRATOR)
                {
                  unsigned int offset, shift;
                  offset = current_ekran.index_position >> 5;        //Це є, фактично, ділення на 32
                  shift = (current_ekran.index_position & (32 - 1)); //Це є, фактично, визначення остачі від ділення на 32

                  uint32_t *p_rang = edition_settings.ranguvannja_analog_registrator;
                  *(p_rang + offset) ^= (1 << shift);

                  //Формуємо екран відображення зранжованих функцій
                  make_ekran_set_function_in_output_led_df_dt_reg(current_ekran.current_level, INDEX_VIEWING_A_REG, p_rang);
                }
                else if (current_ekran.current_level == EKRAN_RANGUVANNJA_DIGITAL_REGISTRATOR)
                {
                  unsigned int offset, shift;
                  offset = current_ekran.index_position >> 5;        //Це є, фактично, ділення на 32
                  shift = (current_ekran.index_position & (32 - 1)); //Це є, фактично, визначення остачі від ділення на 32

                  uint32_t *p_rang = edition_settings.ranguvannja_digital_registrator;
                  *(p_rang + offset) ^= (1 << shift);

                  //Формуємо екран відображення зранжованих функцій
                  make_ekran_set_function_in_output_led_df_dt_reg(current_ekran.current_level, INDEX_VIEWING_D_REG, p_rang);
                }
                else if (current_ekran.current_level == EKRAN_RANGUVANNJA_OFF_CB)
                {
                  unsigned int offset, shift;
                  offset = current_ekran.index_position >> 5;        //Це є, фактично, ділення на 32
                  shift = (current_ekran.index_position & (32 - 1)); //Це є, фактично, визначення остачі від ділення на 32

                  uint32_t *p_rang = edition_settings.ranguvannja_off_cb;
                  *(p_rang + offset) ^= (1 << shift);

                  //Формуємо екран відображення зранжованих функцій
                  make_ekran_set_function_in_output_led_df_dt_reg(current_ekran.current_level, INDEX_VIEWING_OFF_CB, p_rang);
                }
                else if (current_ekran.current_level == EKRAN_RANGUVANNJA_ON_CB)
                {
                  unsigned int offset, shift;
                  offset = current_ekran.index_position >> 5;        //Це є, фактично, ділення на 32
                  shift = (current_ekran.index_position & (32 - 1)); //Це є, фактично, визначення остачі від ділення на 32

                  uint32_t *p_rang = edition_settings.ranguvannja_on_cb;
                  *(p_rang + offset) ^= (1 << shift);

                  //Формуємо екран відображення зранжованих функцій
                  make_ekran_set_function_in_output_led_df_dt_reg(current_ekran.current_level, INDEX_VIEWING_ON_CB, p_rang);
                }
                else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_DF1_PLUS) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_DF1_PLUS + MAX_ROW_LIST_TYPE_SOURCE_DF * NUMBER_DEFINED_FUNCTIONS - 1)))
                {
                  unsigned int offset, shift;
                  offset = current_ekran.index_position >> 5;        //Це є, фактично, ділення на 32
                  shift = (current_ekran.index_position & (32 - 1)); //Це є, фактично, визначення остачі від ділення на 32

                  unsigned int index_in_ekran_list = current_ekran.current_level - EKRAN_RANGUVANNJA_DF1_PLUS;
                  unsigned int type_source = index_in_ekran_list % MAX_ROW_LIST_TYPE_SOURCE_DF;
                  unsigned int index_of_df = index_in_ekran_list / MAX_ROW_LIST_TYPE_SOURCE_DF;

                  uint32_t *p_rang = NULL;
                  if (type_source == INDEX_ML_LIST_TYPE_SOURCE_PLUS_DF)
                    p_rang = &edition_settings.ranguvannja_df_source_plus[N_BIG * index_of_df];
                  else if (type_source == INDEX_ML_LIST_TYPE_SOURCE_MINUS_DF)
                    p_rang = &edition_settings.ranguvannja_df_source_minus[N_BIG * index_of_df];
                  else if (type_source == INDEX_ML_LIST_TYPE_SOURCE_BLK_DF)
                    p_rang = &edition_settings.ranguvannja_df_source_blk[N_BIG * index_of_df];
                  else if (type_source == INDEX_ML_LIST_TYPE_SOURCE_R_DF)
                    p_rang = &edition_settings.ranguvannja_df_source_reset[N_BIG * index_of_df];
                  else
                    total_error_sw_fixed();

                  *(p_rang + offset) ^= (1 << shift);

                  //Формуємо екран відображення зранжованих функцій
                  make_ekran_set_function_in_output_led_df_dt_reg(current_ekran.current_level, INDEX_VIEWING_DF, p_rang);
                }
                else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_SET_DT1_PLUS) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_SET_DT1_PLUS + 4 * NUMBER_DEFINED_TRIGGERS - 1)))
                {
                  unsigned int offset, shift;
                  offset = current_ekran.index_position >> 5;        //Це є, фактично, ділення на 32
                  shift = (current_ekran.index_position & (32 - 1)); //Це є, фактично, визначення остачі від ділення на 32

                  unsigned int index_in_ekran_list = current_ekran.current_level - EKRAN_RANGUVANNJA_SET_DT1_PLUS;
                  unsigned int type_source = index_in_ekran_list % 2;
                  unsigned int type_of_action = (index_in_ekran_list / 2) & 0x1;
                  unsigned int index_of_dt = index_in_ekran_list / 4;

                  uint32_t *p_rang;
                  if (type_of_action == INDEX_ML_SET_DT)
                  {
                    if (type_source == INDEX_ML_LIST_TYPE_SOURCE_PLUS_DT)
                      p_rang = &edition_settings.ranguvannja_set_dt_source_plus[N_BIG * index_of_dt];
                    else if (type_source == INDEX_ML_LIST_TYPE_SOURCE_MINUS_DT)
                      p_rang = &edition_settings.ranguvannja_set_dt_source_minus[N_BIG * index_of_dt];
                    else
                      total_error_sw_fixed();
                  }
                  else if (type_of_action == INDEX_ML_RESET_DT)
                  {
                    if (type_source == INDEX_ML_LIST_TYPE_SOURCE_PLUS_DT)
                      p_rang = &edition_settings.ranguvannja_reset_dt_source_plus[N_BIG * index_of_dt];
                    else if (type_source == INDEX_ML_LIST_TYPE_SOURCE_MINUS_DT)
                      p_rang = &edition_settings.ranguvannja_reset_dt_source_minus[N_BIG * index_of_dt];
                    else
                      total_error_sw_fixed();
                  }
                  else
                    total_error_sw_fixed();

                  *(p_rang + offset) ^= (1 << shift);

                  //Формуємо екран відображення зранжованих функцій
                  make_ekran_set_function_in_output_led_df_dt_reg(current_ekran.current_level, INDEX_VIEWING_DT, p_rang);
                }
                else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_AND1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_AND1 + NUMBER_DEFINED_AND - 1)))
                {
                  unsigned int offset, shift;
                  offset = current_ekran.index_position >> 5;        //Це є, фактично, ділення на 32
                  shift = (current_ekran.index_position & (32 - 1)); //Це є, фактично, визначення остачі від ділення на 32

                  uint32_t *p_rang = &edition_settings.ranguvannja_d_and[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_D_AND1)];
                  *(p_rang + offset) ^= (1 << shift);

                  //Формуємо екран відображення зранжованих функцій
                  make_ekran_set_function_in_output_led_df_dt_reg(current_ekran.current_level, INDEX_VIEWING_D_AND, p_rang);
                }
                else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_OR1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_OR1 + NUMBER_DEFINED_OR - 1)))
                {
                  unsigned int offset, shift;
                  offset = current_ekran.index_position >> 5;        //Це є, фактично, ділення на 32
                  shift = (current_ekran.index_position & (32 - 1)); //Це є, фактично, визначення остачі від ділення на 32

                  uint32_t *p_rang = &edition_settings.ranguvannja_d_or[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_D_OR1)];
                  *(p_rang + offset) ^= (1 << shift);

                  //Формуємо екран відображення зранжованих функцій
                  make_ekran_set_function_in_output_led_df_dt_reg(current_ekran.current_level, INDEX_VIEWING_D_OR, p_rang);
                }
                else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_XOR1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_XOR1 + NUMBER_DEFINED_XOR - 1)))
                {
                  unsigned int offset, shift;
                  offset = current_ekran.index_position >> 5;        //Це є, фактично, ділення на 32
                  shift = (current_ekran.index_position & (32 - 1)); //Це є, фактично, визначення остачі від ділення на 32

                  uint32_t *p_rang = &edition_settings.ranguvannja_d_xor[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_D_XOR1)];
                  *(p_rang + offset) ^= (1 << shift);

                  //Формуємо екран відображення зранжованих функцій
                  make_ekran_set_function_in_output_led_df_dt_reg(current_ekran.current_level, INDEX_VIEWING_D_XOR, p_rang);
                }
                else if ((current_ekran.current_level >= EKRAN_RANGUVANNJA_D_NOT1) && (current_ekran.current_level <= (EKRAN_RANGUVANNJA_D_NOT1 + NUMBER_DEFINED_NOT - 1)))
                {
                  unsigned int offset, shift;
                  offset = current_ekran.index_position >> 5;        //Це є, фактично, ділення на 32
                  shift = (current_ekran.index_position & (32 - 1)); //Це є, фактично, визначення остачі від ділення на 32

                  uint32_t *p_rang = &edition_settings.ranguvannja_d_not[N_BIG * (current_ekran.current_level - EKRAN_RANGUVANNJA_D_NOT1)];
                  *(p_rang + offset) ^= (1 << shift);

                  //Формуємо екран відображення зранжованих функцій
                  make_ekran_set_function_in_output_led_df_dt_reg(current_ekran.current_level, INDEX_VIEWING_D_NOT, p_rang);
                }

                //Очистити сигналізацію, що натиснута кнопка
                if (new_state_keyboard == (1u << BIT_KEY_RIGHT))
                  new_state_keyboard &= ~(1u << BIT_KEY_RIGHT);
                else
                  new_state_keyboard &= ~(1u << BIT_KEY_LEFT);
              }
              else
              {
                //Натиснуто зразу декілька кнопок - це є невизначена ситуація, тому скидаємо сигналізацію про натиснуті кнопки і чекаємо знову
                unsigned int temp_data = new_state_keyboard;
                new_state_keyboard &= ~temp_data;
              }
            }
          }
          break;
        }
        /******************************************************************************************************************************************/

        /************************************* BEGIN  EKRAN_LEVEL_ *****************************************************************************************************/
        //    case 0xff:
        //      {
        //        //Очищаємо всі біти краім упралінських
        //        new_state_keyboard &= (1<<BIT_KEY_ENTER)|(1<<BIT_KEY_ESC)|(1<<BIT_KEY_UP)|(1<<BIT_KEY_DOWN)|(1<<BIT_REWRITE);
        //        //Дальше виконуємо дії, якщо натиснута кнопка на яку треба реагівати, або стоїть команда обновити екран
        //        if (new_state_keyboard !=0)
        //        {
        //          //Пріоритет стоїть на обновлені екрану
        //          if((new_state_keyboard & (1<<BIT_REWRITE)) !=0)
        //          {
        //            //Очищаємо біт обновлення екрану
        //            new_state_keyboard &= (unsigned int)(~(1<<BIT_REWRITE));
        //          }
        //          else
        //          {
        //            if (new_state_keyboard == (1<<BIT_KEY_ENTER))
        //            {
        //              //Натиснута кнопка ENTER
        //
        //              //Очистити сигналізацію, що натиснута кнопка
        //              new_state_keyboard &= (unsigned int)(~(1<<BIT_KEY_ENTER));
        //            }
        //            else if (new_state_keyboard == (1<<BIT_KEY_ESC))
        //            {
        //              //Переходимо у попереднє меню
        //              current_ekran.current_level = previous_level_in_current_level_menu[current_ekran.current_level];
        //              current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
        //              current_ekran.edition = 0;
        //
        //              //Виставляємо команду на обновлекння нового екрану
        //              new_state_keyboard |= (1<<BIT_REWRITE);
        //              //Очистити сигналізацію, що натиснута кнопка
        //              new_state_keyboard &= (unsigned int)(~(1<<BIT_KEY_ESC));
        //            }
        //            else if (new_state_keyboard == (1<<BIT_KEY_UP))
        //            {
        //              //Натиснута кнопка UP
        //
        //              //Очистити сигналізацію, що натиснута кнопка
        //              new_state_keyboard &= (unsigned int)(~(1<<BIT_KEY_UP));
        //            }
        //            else if (new_state_keyboard == (1<<BIT_KEY_DOWN))
        //            {
        //              //Натиснута кнопка DOWN
        //
        //              //Очистити сигналізацію, що натиснута кнопка
        //              new_state_keyboard &= (unsigned int)(~(1<<BIT_KEY_DOWN));
        //            }
        //            else if (new_state_keyboard == (1<<BIT_KEY_RIGHT))
        //            {
        //              //Натиснута кнопка RIGHT
        //
        //              //Очистити сигналізацію, що натиснута кнопка
        //              new_state_keyboard &= (unsigned int)(~(1<<BIT_KEY_RIGHT));
        //            }
        //            else if (new_state_keyboard == (1<<BIT_KEY_LEFT))
        //            {
        //              //Натиснута кнопка LEFT
        //
        //              //Очистити сигналізацію, що натиснута кнопка
        //              new_state_keyboard &= (unsigned int)(~(1<<BIT_KEY_LEFT));
        //            }
        //            else
        //            {
        //              //Натиснуто зразу декілька кнопок - це є невизначена ситуація, тому скидаємо сигналізацію про натиснуті кнопки і чекаємо знову
        //              unsigned int temp_data = new_state_keyboard;
        //              new_state_keyboard &= ~temp_data;
        //            }
        //          }
        //        }
        //        break;
        //      }
        /************************************* END  EKRAN_LEVEL_ *****************************************************************************************************/
      default:
        break;
    }
  }
}
/*****************************************************/

/*****************************************************/
//Збільшення або зменшення числа при натисканні кнопки
/*
  Вхідні параменти
  label_value - вказівник на змінюване  число
  inc_dec 
    0 - зменшити на одиницю
    1 - збільшити на одиницю
*/
/*****************************************************/
void inc_or_dec_value(unsigned int *label_value, unsigned int inc_dec)
{
  int temp_value = *label_value;

  if (inc_dec == 0)
    temp_value--;
  else if (inc_dec == 1)
    temp_value++;

  if (temp_value > 9)
    temp_value = 0;
  else if (temp_value < 0)
    temp_value = 9;

  *label_value = temp_value;
}
/*****************************************************/

/*****************************************************/
//Робимо повідомлення про те чи треба щоб зміни набули сили
/*****************************************************/
void make_ekran_ask_rewrite(void)
{
  static unsigned char const name_string[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] =
    {
      {"Ввести изменения",
       "Нет-ESC Да-ENTER"},
      {"  Ввести зміни  ",
       "Ні-ESC Так-ENTER"},
      {" Enter changes  ",
       "No-ESC Yes-ENTER"},
      {"Ввести изменения",
       "Нет-ESC Да-ENTER"}};

  int index_language;

  if (current_ekran.current_level == EKRAN_VIEW_SETTING_LANGUAGE)
    index_language = index_language_in_array(edition_settings.language);
  else
    index_language = index_language_in_array(current_settings.language);

  //Копіюємо  рядки у робочий екран
  for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
  {
    for (unsigned int j = 0; j < MAX_COL_LCD; j++)
      working_ekran[i][j] = (i < 2) ? name_string[index_language][i][j] : ' ';
  }

  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//Робимо повідомлення підтвердження виконання команди
/*****************************************************/
void make_ekran_about_activation_command(unsigned int index, unsigned char information[][MAX_NAMBER_LANGUAGE][MAX_COL_LCD])
{
  static unsigned char const name_string[MAX_NAMBER_LANGUAGE][MAX_COL_LCD] =
    {
      "Нет-ESC Да-ENTER",
      "Ні-ESC Так-ENTER",
      "No-ESC Yes-ENTER",
      "Нет-ESC Да-ENTER"};
  int index_language = index_language_in_array(current_settings.language);

  //Копіюємо  рядки у робочий екран
  for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
  {
    if (i == 0)
    {
      for (unsigned int j = 0; j < MAX_COL_LCD; j++)
        working_ekran[i][j] = information[index][index_language][j];
    }
    else
    {
      for (unsigned int j = 0; j < MAX_COL_LCD; j++)
        working_ekran[i][j] = (i == 1) ? name_string[index_language][j] : ' ';
    }
  }

  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//Формування вікна про помилку
/*****************************************************/
void make_ekran_about_error(const unsigned char information[][MAX_COL_LCD])
{
  static unsigned char const name_string[MAX_NAMBER_LANGUAGE][MAX_COL_LCD] =
    {
      " Выход Esc/Enter",
      " Вихід Esc/Enter",
      " Exit Esc/Enter ",
      " Выход Esc/Enter"};
  int index_language;

  if (current_ekran.current_level == EKRAN_VIEW_SETTING_LANGUAGE)
    index_language = index_language_in_array(edition_settings.language);
  else
    index_language = index_language_in_array(current_settings.language);

  //Копіюємо  рядки у робочий екран
  for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
  {
    if (i == 0)
    {
      for (unsigned int j = 0; j < MAX_COL_LCD; j++)
        working_ekran[i][j] = information[index_language][j];
    }
    else
    {
      for (unsigned int j = 0; j < MAX_COL_LCD; j++)
        working_ekran[i][j] = (i == 1) ? name_string[index_language][j] : ' ';
    }
  }

  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//Редагування величин вводимих значень
/*****************************************************
Вхідні параметри
  0 - натснуто кнопку вниз
  1 - натиснуто кнопку вверх
Вхідні параметри
  Немає
*****************************************************/
unsigned int edit_setpoint(unsigned int action, unsigned int value, unsigned int floating_value, unsigned int comma, unsigned int end, unsigned int min_step)
{
  unsigned int rozrjad, vaga = min_step, temp_value, data_return;
  //Вираховуємо розряд числа на який зараз вказує курсор
  rozrjad = end - current_ekran.position_cursor_x;

  //У разі якщо редаговане число є числом з комою, то враховуємо позицію коми
  if (floating_value != 0)
    if (current_ekran.position_cursor_x < ((int) comma))
      rozrjad--;

  for (unsigned int i = 0; i < rozrjad; i++)
    vaga *= 10;

  //Виділяємо число , яке трба змінити
  temp_value = (value / vaga) % 10;

  //У вхідному числі, який нас цікавить редагований розряд скидаємо в нуль відніманням
  data_return = value - temp_value * vaga;

  //Змінюємо виділений розняд
  inc_or_dec_value(&temp_value, action);

  //Вводимо зміненй розряд у кінцевий результат
  data_return += temp_value * vaga;

  return data_return;
}
/*****************************************************/

/*****************************************************/
//Перевіряємо достовірність даних
/*
Результат
  1 - дані достовірні
  0 - дані недостовірні
*/
/*****************************************************/
unsigned int check_data_setpoint(unsigned int current_value, unsigned int min_value, unsigned int max_value)
{
  if ((current_value < min_value) || (current_value > max_value))
    return 0;

  //Вихід з повідомленням, що уставко в межах допуску
  return 1;
}
/*****************************************************/

/*****************************************************/

/*****************************************************/
//
/*****************************************************/
/*****************************************************/
