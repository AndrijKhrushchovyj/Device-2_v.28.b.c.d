#include "header.h"

/*****************************************************/
//Формуємо екран відображення заголовків настроювання для дати і часу
/*****************************************************/
void make_ekran_chose_data_time_settings(void)
{
  static const uint8_t name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_CHOSE_DATA_TIME_SETTINGS][MAX_COL_LCD] = 
  {
    {
      " Часовой пояс   ",
      " Нач.летнего вр.",
      " Кон.летнего вр."
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                        ,
      " Синхронизация  "
#endif
    },
    {
      " Часовий пояс   ",
      " Поч.літнього ч.",
      " Кін.літнього ч."
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                        ,
      " Синхронізація  "
#endif
    },
    {
      " Time zone      ",
      " DST on         ",
      " DST off        "
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                        ,
      " Synchronization"
#endif
    },
    {
      " Часовой пояс   ",
      " Нач.летнего вр.",
      " Кон.летнего вр."
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                        ,
      " Синхронизация  "
#endif
    }
  };

  uint8_t name_string_tmp[MAX_ROW_FOR_CHOSE_DATA_TIME_SETTINGS][MAX_COL_LCD];

  /******************************************/
  //Виключаємо поля, які не треба відображати
  /******************************************/
  int position_temp = current_ekran.index_position;
  int additional_current = 0;

  int index_language = index_language_in_array(current_settings.language);
  for(intptr_t index_1 = 0; index_1 < MAX_ROW_FOR_CHOSE_DATA_TIME_SETTINGS; ++index_1)
  {
    if (
        ((current_settings.dst & MASKA_FOR_BIT(N_BIT_TZ_DST)) == 0) &&
        (
         (index_1 == INDEX_ML_CHDT_DST_ON) ||
         (index_1 == INDEX_ML_CHDT_DST_OFF)
        )   
       )   
    {
      ++additional_current;
      if ((index_1 - additional_current) < position_temp) --position_temp;
      continue;
    }
    
    uint8_t *ptr_target = name_string_tmp[index_1 - additional_current];
    uint8_t const *ptr_source = name_string[index_language][index_1];
    for(size_t index_2 = 0; index_2 < MAX_COL_LCD; ++index_2) *ptr_target++ = *ptr_source++;
  }
  /******************************************/
  
  int index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;
  
  //Копіюємо  рядки у робочий екран
  for (size_t i = 0; i < MAX_ROW_LCD; i++)
  {
    uint8_t *ptr_target = working_ekran[i];
    
    if (index_of_ekran < (MAX_ROW_FOR_CHOSE_DATA_TIME_SETTINGS - additional_current))
    {
      uint8_t *ptr_source = name_string_tmp[index_of_ekran];
      for (size_t j = 0; j < MAX_COL_LCD; ++j) *ptr_target++ = *ptr_source++;
    }
    else
      for (size_t j = 0; j<MAX_COL_LCD; ++j) *ptr_target++ = ' ';

    ++index_of_ekran;
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
//Формуємо екран відображення інформації по часовій зоні і переходу на літній час
/*****************************************************/
void make_ekran_timezone_dst(void)
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_TIMEZONE_SETTINGS][MAX_COL_LCD] = 
  {
    {
      "  Часовой пояс  ",
      "  Летнее время  "
    },
    {
      "  Часовий пояс  ",
      "   Літній час   "
    },
    {
      "   Time zone    ",
      "      DST       "
    },
    {
      "  Часовой пояс  ",
      "  Летнее время  "
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
    unsigned int view = (current_ekran.edition == 0);
    if (index_of_ekran_tmp < MAX_ROW_FOR_TIMEZONE_SETTINGS)
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (size_t j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][index_of_ekran_tmp][j];
      }
      else
      {
        //У парному номері рядку виводимо стан
        __SETTINGS *point_1 = (view == true) ? &current_settings : &edition_settings;
        
        if (index_of_ekran_tmp == INDEX_ML_TIME_ZONE)
        {
          int time_zone = point_1->time_zone;

          for (size_t j = 0; j < MAX_COL_LCD; j++) 
          {
            if (j == 8) 
            {
              if (time_zone >= 0) working_ekran[i][j] = '+';
              else 
              {
                working_ekran[i][j] = '-';
                time_zone *= -1; //робимо, щоб число було додатнім
              }
            }
            else if (j == 9) working_ekran[i][j] = ((time_zone >= 10) ? (time_zone / 10) : time_zone ) + 0x30;
            else if (j == 10) working_ekran[i][j] = (time_zone >= 10) ? ((time_zone % 10) + 0x30) : ' ';
            else 
            {
              static const unsigned char information[MAX_COL_LCD] = "     UTC___     ";
        
              working_ekran[i][j] = information[j];
            }
          }
          if (position_temp == index_of_ekran_tmp) current_ekran.position_cursor_x = 4;
        }
        else if (index_of_ekran_tmp == INDEX_ML_DST)
        {
          unsigned int state = ((point_1->dst & MASKA_FOR_BIT(N_BIT_TZ_DST)) != 0);

          for (size_t j = 0; j < MAX_COL_LCD; j++) working_ekran[i][j] = information_off_on[index_language][state][j];
          if (position_temp == index_of_ekran_tmp) current_ekran.position_cursor_x = cursor_x_off_on[index_language][state];
        }
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
//Формуємо екран відображення інформації по правилах переходу
/*****************************************************/
void make_ekran_dst_rule(unsigned int rule)
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_DST_RULE][MAX_COL_LCD] = 
  {
    {
      "     Месяц      ",
      "  День недели   ",
      "   Пор.номер    ",
      "      Час       "
    },
    {
      "     Місяць     ",
      "   День тижня   ",
      "   Пор.номер    ",
      "     Година     "
    },
    {
      "     Month      ",
      "  Day of week   ",
      "   Seq.number   ",
      "      Hour      "
    },
    {
      "     Месяц      ",
      "  День недели   ",
      "   Пор.номер    ",
      "      Час       "
    }
  };
  int index_language = index_language_in_array(current_settings.language);
  
  unsigned int position_temp = current_ekran.index_position;
  
  //Множення на два величини position_temp потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
  unsigned int index_of_ekran = ((position_temp << 1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;
  
  for (size_t i = 0; i < MAX_ROW_LCD; i++)
  {
    unsigned int index_of_ekran_tmp = index_of_ekran >> 1;
    if (index_of_ekran_tmp < MAX_ROW_FOR_DST_RULE)
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (size_t j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][index_of_ekran_tmp][j];
      }
      else
      {
        //У парному номері рядку виводимо стан
        static unsigned char const monthes[MAX_NAMBER_LANGUAGE][12][MAX_COL_LCD] =
        {
          {
            "     Январь     ",
            "     Февраль    ",
            "      Март      ",
            "     Апрель     ",
            "       Май      ",
            "      Июнь      ",
            "      Июль      ",
            "     Август     ",
            "    Сентябрь    ",
            "     Октябрь    ",
            "     Ноябрь     ",
            "     Декабрь    "
          },
          {
            "     Січень     ",
            "      Лютий     ",
            "    Березень    ",
            "     Квітень    ",
            "     Травень    ",
            "     Червень    ",
            "     Липень     ",
            "     Серпень    ",
            "    Вересень    ",
            "     Жовтень    ",
            "    Листопад    ",
            "     Грудень    "
          },
          {
            "     January    ",
            "    February    ",
            "      March     ",
            "      April     ",
            "       May      ",
            "      June      ",
            "      July      ",
            "     August     ",
            "    September   ",
            "    October     ",
            "    November    ",
            "    December    "
          },
          {
            "     Январь     ",
            "     Февраль    ",
            "      Март      ",
            "     Апрель     ",
            "       Май      ",
            "      Июнь      ",
            "      Июль      ",
            "     Август     ",
            "    Сентябрь    ",
            "     Октябрь    ",
            "     Ноябрь     ",
            "     Декабрь    "
          }
        };

        static unsigned char const day_of_week[MAX_NAMBER_LANGUAGE][7][MAX_COL_LCD] =
        {
          {
            "   Воскресенье  ",
            "   Понедельник  ",
            "     Вторник    ",
            "      Среда     ",
            "     Четверг    ",
            "     Пятница    ",
            "     Суббота    "
          },
          {
            "     Неділя     ",
            "    Понеділок   ",
            "    Вівторок    ",
            "     Середа     ",
            "     Четвер     ",
            "    П'ятниця    ",
            "     Субота     "
          },
          {
            "     Sunday     ",
            "     Monday     ",
            "     Tuesday    ",
            "    Wednesday   ",
            "    Thursday    ",
            "     Friday     ",
            "    Saturday    "
          },
          {
            "   Воскресенье  ",
            "   Понедельник  ",
            "     Вторник    ",
            "      Среда     ",
            "     Четверг    ",
            "     Пятница    ",
            "     Суббота    "
          }
        };
        
        static unsigned char const week_rule[MAX_NAMBER_LANGUAGE][5][MAX_COL_LCD] = 
        {
          {
            "     Первый     ",
            "     Второй     ",
            "     Третий     ",
            "    Четвёртый   ",
            "    Последний   "
          },
          {
            "     Перший     ",
            "     Другий     ",
            "     Третій     ",
            "    Четвертий   ",
            "    Останній    "
          },
          {
            "      First     ",
            "     Second     ",
            "      Third     ",
            "     Fourth     ",
            "      Last      "
          },
          {
            "     Первый     ",
            "     Второй     ",
            "     Третий     ",
            "    Четвёртый   ",
            "    Последний   "
          }
        };
        unsigned char templ_hour_str[MAX_COL_LCD] = "                ";
        
        unsigned char *ptr_target = working_ekran[i];
        unsigned char const *ptr_source = NULL;
        if (index_of_ekran_tmp == INDEX_ML_DST_MM) ptr_source = monthes[index_language][((rule >> POS_MM) & ((1 << SHIFT_MM) - 1)) - 1];
        else if (index_of_ekran_tmp == INDEX_ML_DST_DOW) ptr_source = day_of_week[index_language][(rule >> POS_DOW) & ((1 << SHIFT_DOW) - 1)];
        else if (index_of_ekran_tmp == INDEX_ML_DST_WR) ptr_source = week_rule[index_language][((rule >> POS_WR) & ((1 << SHIFT_WR) - 1)) - 1];
        else if (index_of_ekran_tmp == INDEX_ML_DST_HH)
        {
          unsigned int hour = (rule >> POS_HH) & ((1 << SHIFT_HH) - 1);
          templ_hour_str[MAX_COL_LCD/2 - 1] = hour / 10 + 0x30;
          templ_hour_str[MAX_COL_LCD/2] = hour % 10 + 0x30;
          ptr_source = templ_hour_str;
        }
        else
        {
          //Теоретично цього ніколи не мало б бути
          total_error_sw_fixed();
        }

        for (size_t j = 0; j < MAX_COL_LCD; ++j) *ptr_target++ = *ptr_source++;
        if (
            (index_of_ekran_tmp == INDEX_ML_DST_WR)
            &&
            (
             (index_language == INDEX_LANGUAGE_RU) ||
             (index_language == INDEX_LANGUAGE_UA)
            )   
           )
        {
          unsigned int dow = (rule >> POS_DOW) & ((1 << SHIFT_DOW) - 1);
          
          if ((dow == _Sun) || (dow == _Wed) || (dow == _Fri)  || (dow == _Sat))
          {
            unsigned int dr = (rule >> POS_WR) & ((1 << SHIFT_WR) - 1);
            if ((dr < _N1) || (dr > _NL))
            {
              //Теоретично цього ніколи не мало б бути
              total_error_sw_fixed();
            }
            
            unsigned char *ptr_tmp = working_ekran[i];
            while (*ptr_tmp == ' ') ++ptr_tmp;
            while (*(++ptr_tmp) != ' ');
          
            if ((ptr_tmp - working_ekran[i]) >= 2)
            {
              --ptr_tmp;
              if (index_language == INDEX_LANGUAGE_RU)
              {
                if (dow == _Sun) *(ptr_tmp--) = 'е';
                else *(ptr_tmp--) = 'я';

                if (dow == _Sun)
                {
                  switch (dr)
                  {
                  case _N1:
                  case _N2:
                  case _N4:
                    {
                      *ptr_tmp = 'о';
                      break;
                    }
                  case _N3:
                    {
                      *ptr_tmp = 'ь';
                      break;
                    }
                  case _NL:
                    {
                      *ptr_tmp = 'е';
                      break;
                    }
                  }
                } 
                else 
                {
                  switch (dr)
                  {
                  case _N1:
                  case _N2:
                  case _N4:
                    {
                      *ptr_tmp = 'а';
                      break;
                    }
                  case _N3:
                    {
                      *ptr_tmp = 'ь';
                      break;
                    }
                  case _NL:
                    {
                      *ptr_tmp = 'я';
                      break;
                    }
                  }
                }
              }
              else
              {
                *(ptr_tmp--) = ' ';

                switch (dr)
                {
                case _N1:
                case _N2:
                case _N4:
                  {
                    *ptr_tmp = 'а';
                    break;
                  }
                case _N3:
                case _NL:
                  {
                    *ptr_tmp = 'я';
                    break;
                  }
                }
              }
            }
            else
            {
              //Теоретично цього ніколи не мало б бути
              total_error_sw_fixed();
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

  {
    int position_cursor_x = 0;
    unsigned char const *ptr_target = working_ekran[current_ekran.position_cursor_y];
    if (*ptr_target == ' ')
    {  
      while ((*(ptr_target + position_cursor_x + 1) == ' ') && (position_cursor_x < MAX_COL_LCD)) ++position_cursor_x;
    }
    current_ekran.position_cursor_x = position_cursor_x;
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


#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
/*****************************************************/
//Формуємо екран відображення налаштувань мережевого рівня Ethernet
/*****************************************************/
void make_ekran_settings_synchro()
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_SYNCHRO][MAX_COL_LCD] = 
  {
    {
      "   IP Сервера   ",
      "      Порт      ",
      "     Период     "
    }, 
    {
      "   IP Сервера   ",
      "      Порт      ",
      "     Період     "
    }, 
    {
      "   Server IP    ",
      "      Port      ",
      "     Period     "
    },
    {
      "   IP Сервера   ",
      "      Порт      ",
      "     Период     "
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
    if (index_of_ekran_tmp < MAX_ROW_FOR_SYNCHRO)
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
        if (index_of_ekran_tmp == INDEX_ML_SYN_IPV4)
        {
          working_ekran[i][4] = working_ekran[i][8] = working_ekran[i][12] = '.';

          uint32_t j = COL_IP4_SERVER_BEGIN + 3 - 1;
          for (size_t k1 = 0; k1 < 4; k1++)
          {
            uint32_t val_tmp = point_1->IP_time_server[k1];
            for (size_t k2 = 0; k2 < 3; k2++)
            {
              uint32_t val = val_tmp % 10;
              val_tmp /= 10;
              if (
                  (current_ekran.edition != 0) ||
                  (k2 == 0) ||
                  (val != 0) ||
                  (val_tmp != 0)  
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
        else if (index_of_ekran_tmp == INDEX_ML_SYN_PORT)
        {
          uint32_t port_tmp = point_1->port_time_server;
        
          uint32_t j = COL_PORT_END;
          for (size_t k2 = 0; k2 < 5; k2++)
          {
            uint32_t val = port_tmp % 10;
            port_tmp /= 10;
            if (
                (current_ekran.edition != 0) ||
                (k2 == 0) ||
                (val != 0) ||
                (port_tmp != 0)  
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
        else if (index_of_ekran_tmp == INDEX_ML_SYN_PERIOD)
        {
          uint32_t period_tmp = point_1->period_sync;
        
          uint32_t j = COL_PEDIOD_END;
          for (size_t k2 = 0; k2 < 5; k2++)
          {
            uint32_t val = period_tmp % 10;
            period_tmp /= 10;
            if (
                (current_ekran.edition != 0) ||
                (k2 == 0) ||
                (val != 0) ||
                (period_tmp != 0)  
               )
            {
              working_ekran[i][j--] = string[val];
            }
            else
            {
              working_ekran[i][j--] = ' ';
            }
          }

          static uint8_t const symbols[MAX_NAMBER_LANGUAGE][4+1] = {"мин.", "хв. ", "min.", "мин."};
          uint8_t const *ptr_tmp= symbols[index_language];
          
          j = COL_PEDIOD_END + 2;
          while ((j < MAX_COL_LCD) && (*ptr_tmp != ' ') && (*ptr_tmp != '\0'))  working_ekran[i][j++] = *ptr_tmp++;
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
    if (current_ekran.index_position == INDEX_ML_SYN_IPV4)
    {
      current_ekran.position_cursor_x = COL_IP4_SERVER_BEGIN;
      last_position_cursor_x = COL_IP4_SERVER_END;
    }
    else if (current_ekran.index_position == INDEX_ML_SYN_PORT) 
    {
      current_ekran.position_cursor_x = COL_PORT_BEGIN;
      last_position_cursor_x = COL_PORT_END;
    }
    else if (current_ekran.index_position == INDEX_ML_SYN_PERIOD) 
    {
      current_ekran.position_cursor_x = COL_PERIOD_BEGIN;
      last_position_cursor_x = COL_PEDIOD_END;
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
