#include "header.h"

/*****************************************************/
//Вираховуваня символу і поміщення його в робочий екран
/*****************************************************/
void calc_symbol_and_put_into_working_ekran(unsigned char *point_in_working_ekran, void *point_value, void *point_vaga, unsigned int *point_first_symbol, unsigned int current_position_x, unsigned int position_comma, unsigned int view, unsigned int v_32_64)
{
  unsigned int temp_data;
  if (v_32_64 == 0)
  {
    temp_data = (*((unsigned int *) point_value)) / (*((unsigned int *) point_vaga)); //виділяємо число, яке треба перетворити у символ і помістити у дану позицію екрану
    *((unsigned int *) point_value) %= *((unsigned int *) point_vaga);                //вираховуємо число без символа, який ми зараз будемо виводити на екран
    *((unsigned int *) point_vaga) /= 10;                                             //зменшуємо ваговий коефіцієнт в 10 разів
  }
  else
  {
    temp_data = (*((unsigned long long *) point_value)) / (*((unsigned long long *) point_vaga)); //виділяємо число, яке треба перетворити у символ і помістити у дану позицію екрану
    *((unsigned long long *) point_value) %= *((unsigned long long *) point_vaga);                //вираховуємо число без символа, який ми зараз будемо виводити на екран
    *((unsigned long long *) point_vaga) /= 10;                                                   //зменшуємо ваговий коефіцієнт в 10 разів
  }

  if (view == false)
    *point_in_working_ekran = temp_data + 0x30;
  else
  {
    //У випадку, якщо ми не у режимі редагування, то нулі перед комою (за винятком останнього, якщо такий є) приховуємо
    if ((temp_data != 0) || ((*point_first_symbol) != 0))
    {
      *point_in_working_ekran = temp_data + 0x30;
      if ((*point_first_symbol) == 0)
        *point_first_symbol = 1;
    }
    else
    {
      if (current_position_x < (position_comma - 1))
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
//Повернення номеру індексу першої позиції курсору привідображення слова "   Необмежено   "
/*****************************************************/
static int first_position_for_unlimited_word(void)
{
  const unsigned char unlimited_word[MAX_NAMBER_LANGUAGE] = {2, 3, 3, 2};
  int index_language = index_language_in_array(current_settings.language);
  return unlimited_word[index_language];
}
/*****************************************************/

/*****************************************************/
//Формуємо екран відображення уставок РПН
/*****************************************************/
void make_ekran_setpoint_rpn(unsigned int group)
{
  static const unsigned char name_string_withoutznam[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_SETPOINT_RPN][MAX_COL_LCD] =
    {
      {" Напряж.основное",
       "  Ширина зоны   ",
       "Макс.кол.перекл.",
       " Напряж.вспомог.",
       "Токовая компенс."},
      {" Напруга основна",
       "  Ширина зони   ",
       "Макс.кіл.перемик",
       " Напруга допом. ",
       " Струмова комп. "},
      {"  Main Voltage  ",
       "Width of Dead Z.",
       " Max.switching N",
       "  Aux.Voltage   ",
       "   Curr.Comp.   "},
      {" Напряж.основное",
       "  Ширина зоны   ",
       "Макс.кол.перекл.",
       " Напряж.вспомог.",
       "Токовая компенс."}};

  static const unsigned char unlimited[MAX_NAMBER_LANGUAGE][MAX_COL_LCD] =
    {
      "  Неограничено  ",
      "   Необмежено   ",
      "   Unlimited    ",
      "  Неограничено  "};

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
    if (index_of_ekran_tmp < (unsigned int) (MAX_ROW_FOR_SETPOINT_RPN))
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок

        for (unsigned int j = 0; j < MAX_COL_LCD; j++)
          working_ekran[i][j] = name_string_withoutznam[index_language][index_of_ekran_tmp][j];

        if (index_of_ekran_tmp == INDEX_ML_STPRPN_OSN)
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду
          if (view == true)
            value = current_settings.setpoint_rpn_osn[group]; //у змінну value поміщаємо значення уставки
          else
            value = edition_settings.setpoint_rpn_osn[group];
        }
        else if (index_of_ekran_tmp == INDEX_ML_STPRPN_ZONE)
        {
          vaga = 100; //максимальний ваговий коефіцієнт для вилілення старшого розряду
          if (view == true)
            value = current_settings.setpoint_rpn_zony[group]; //у змінну value поміщаємо значення уставки
          else
            value = edition_settings.setpoint_rpn_zony[group];
        }
        else if (index_of_ekran_tmp == INDEX_ML_STPRPN_MAX_PER)
        {
          vaga = 10; //максимальний ваговий коефіцієнт для вилілення старшого розряду
          if (view == true)
            value = current_settings.setpoint_rpn_per[group]; //у змінну value поміщаємо значення уставки
          else
            value = edition_settings.setpoint_rpn_per[group];
        }
        else if (index_of_ekran_tmp == INDEX_ML_STPRPN_DOD)
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду
          if (view == true)
            value = current_settings.setpoint_rpn_dod[group]; //у змінну value поміщаємо значення уставки
          else
            value = edition_settings.setpoint_rpn_dod[group];
        }
        else if (index_of_ekran_tmp == INDEX_ML_STPRPN_K)
        {
          vaga = 100; //максимальний ваговий коефіцієнт для вилілення старшого розряду
          if (view == true)
            value = current_settings.setpoint_rpn_K[group]; //у змінну value поміщаємо значення уставки
          else
            value = edition_settings.setpoint_rpn_K[group];
        }

        first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
      }
      else
      {
        //У парному номері рядку виводимо значення уставки
        for (unsigned int j = 0; j < MAX_COL_LCD; j++)
        {
          if (index_of_ekran_tmp == INDEX_ML_STPRPN_OSN)
          {
            if (
              ((j < COL_SETPOINT_RPN_OSN_BEGIN) || (j > COL_SETPOINT_RPN_OSN_END)) &&
              (j != (COL_SETPOINT_RPN_OSN_END + 2)))
              working_ekran[i][j] = ' ';
            else if (j == COL_SETPOINT_RPN_OSN_COMMA)
              working_ekran[i][j] = ',';
            else if (j == (COL_SETPOINT_RPN_OSN_END + 2))
              working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_V];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_RPN_OSN_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == INDEX_ML_STPRPN_ZONE)
          {
            if ((value == SETPOINT_RPN_PER_UNLIMITED) && (current_ekran.edition == 0))
            {
              working_ekran[i][j] = unlimited[index_language][j];
            }
            else
            {
              if (
                ((j < COL_SETPOINT_RPN_ZONE_BEGIN) || (j > COL_SETPOINT_RPN_ZONE_END)) &&
                (j != (COL_SETPOINT_RPN_ZONE_END + 2)))
                working_ekran[i][j] = ' ';
              else if (j == COL_SETPOINT_RPN_ZONE_COMMA)
                working_ekran[i][j] = ',';
              else if (j == (COL_SETPOINT_RPN_ZONE_END + 2))
                working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_PERCENT];
              else
                calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_RPN_ZONE_COMMA, view, 0);
            }
          }
          else if (index_of_ekran_tmp == INDEX_ML_STPRPN_MAX_PER)
          {
            if ((j < COL_SETPOINT_RPN_MAX_PER_BEGIN) || (j > COL_SETPOINT_RPN_MAX_PER_END))
              working_ekran[i][j] = ' ';
            else
              calc_int_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, view);
          }
          else if (index_of_ekran_tmp == INDEX_ML_STPRPN_DOD)
          {
            if (
              ((j < COL_SETPOINT_RPN_DOD_BEGIN) || (j > COL_SETPOINT_RPN_DOD_END)) &&
              (j != (COL_SETPOINT_RPN_DOD_END + 2)))
              working_ekran[i][j] = ' ';
            else if (j == COL_SETPOINT_RPN_DOD_COMMA)
              working_ekran[i][j] = ',';
            else if (j == (COL_SETPOINT_RPN_DOD_END + 2))
              working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_V];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_RPN_DOD_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == INDEX_ML_STPRPN_K)
          {
            if ((j < COL_SETPOINT_RPN_K_BEGIN) || (j > COL_SETPOINT_RPN_K_END))
              working_ekran[i][j] = ' ';
            else if (j == COL_SETPOINT_RPN_K_COMMA)
              working_ekran[i][j] = ',';
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_RPN_K_COMMA, view, 0);
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
    if (current_ekran.index_position == INDEX_ML_STPRPN_OSN)
    {
      current_ekran.position_cursor_x = COL_SETPOINT_RPN_OSN_BEGIN;
      last_position_cursor_x = COL_SETPOINT_RPN_OSN_END;
    }
    else if (current_ekran.index_position == INDEX_ML_STPRPN_ZONE)
    {
      current_ekran.position_cursor_x = COL_SETPOINT_RPN_ZONE_BEGIN;
      last_position_cursor_x = COL_SETPOINT_RPN_ZONE_END;
    }
    else if (current_ekran.index_position == INDEX_ML_STPRPN_MAX_PER)
    {
      if ((value >= SETPOINT_RPN_PER_MIN) && (value <= SETPOINT_RPN_PER_MAX))
      {
        current_ekran.position_cursor_x = COL_SETPOINT_RPN_MAX_PER_BEGIN;
        last_position_cursor_x = COL_SETPOINT_RPN_MAX_PER_END;
      }
      else
      {
        current_ekran.position_cursor_x = first_position_for_unlimited_word();
      }
    }
    else if (current_ekran.index_position == INDEX_ML_STPRPN_DOD)
    {
      current_ekran.position_cursor_x = COL_SETPOINT_RPN_DOD_BEGIN;
      last_position_cursor_x = COL_SETPOINT_RPN_DOD_END;
    }
    else if (current_ekran.index_position == INDEX_ML_STPRPN_K)
    {
      current_ekran.position_cursor_x = COL_SETPOINT_RPN_K_BEGIN;
      last_position_cursor_x = COL_SETPOINT_RPN_K_END;
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
//Формуємо екран відображення витримок РПН
/*****************************************************/
void make_ekran_timeout_rpn(unsigned int group)
{
  static const unsigned char name_string_withoutznam[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_TIMEOUT_RPN][MAX_COL_LCD] =
    {
      {" Время Уб./Приб.",
       "Уб./Приб. Ускор.",
       "Вр.Ускор.ЗНмакс1",
       " Время перекл.  ",
       " Время неиспр.  "},
      {" Час Зн./Підв.  ",
       "Зн./Підв. Приск.",
       " Час пр.ЗНмакс1 ",
       "  Час перемик.  ",
       "   Час неспр.   "},
      {" Lower/Raise T. ",
       " Acc.Lower/Raise",
       " Acc.OVP1 Time  ",
       " Switching Time ",
       "   Fault Time   "},
      {" Время Уб./Приб.",
       "Уб./Приб. Ускор.",
       "Вр.Ускор.ЗНмакс1",
       " Время перекл.  ",
       " Время неиспр.  "}};

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
    if (index_of_ekran_tmp < (unsigned int) (MAX_ROW_FOR_TIMEOUT_RPN))
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j < MAX_COL_LCD; j++)
          working_ekran[i][j] = name_string_withoutznam[index_language][index_of_ekran_tmp][j];

        if (index_of_ekran_tmp == INDEX_ML_TMORPN_UB_PRYB)
        {
          vaga = 100; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (view == true)
            value = current_settings.timeout_rpn_ub_pryb[group]; //у змінну value поміщаємо значення витримки
          else
            value = edition_settings.timeout_rpn_ub_pryb[group];

          value /= 1000; //Відображаємо тільки цілу частину, а витримка у пам'яті зберігається з точністю до мілісекунд
        }
        else if (index_of_ekran_tmp == INDEX_ML_TMORPN_UB_PRYB_PRYSK)
        {
          vaga = 100; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (view == true)
            value = current_settings.timeout_rpn_ub_pryb_prysk[group]; //у змінну value поміщаємо значення витримки
          else
            value = edition_settings.timeout_rpn_ub_pryb_prysk[group];

          value /= 1000; //Відображаємо тільки цілу частину, а витримка у пам'яті зберігається з точністю до мілісекунд
        }
        else if (index_of_ekran_tmp == INDEX_ML_TMORPN_PRYSK_VID_UMAX)
        {
          vaga = 10000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (view == true)
            value = current_settings.timeout_rpn_prysk_vid_Umax[group]; //у змінну value поміщаємо значення витримки
          else
            value = edition_settings.timeout_rpn_prysk_vid_Umax[group];
        }
        else if (index_of_ekran_tmp == INDEX_ML_TMORPN_PEREKL)
        {
          vaga = 10000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (view == true)
            value = current_settings.timeout_rpn_perekl[group]; //у змінну value поміщаємо значення витримки
          else
            value = edition_settings.timeout_rpn_perekl[group];
        }
        else if (index_of_ekran_tmp == INDEX_ML_TMORPN_NESPR)
        {
          vaga = 10000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (view == true)
            value = current_settings.timeout_rpn_nespr[group]; //у змінну value поміщаємо значення витримки
          else
            value = edition_settings.timeout_rpn_nespr[group];
        }

        first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
      }
      else
      {
        //У парному номері рядку виводимо значення уставки
        for (unsigned int j = 0; j < MAX_COL_LCD; j++)
        {
          if (index_of_ekran_tmp == INDEX_ML_TMORPN_UB_PRYB)
          {
            if (
              ((j < COL_TMO_RPN_UB_PRYB_BEGIN) || (j > COL_TMO_RPN_UB_PRYB_END)) &&
              (j != (COL_TMO_RPN_UB_PRYB_END + 2)))
              working_ekran[i][j] = ' ';
            else if (j == (COL_TMO_RPN_UB_PRYB_END + 2))
              working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_int_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, view);
          }
          else if (index_of_ekran_tmp == INDEX_ML_TMORPN_UB_PRYB_PRYSK)
          {
            if (
              ((j < COL_TMO_RPN_UB_PRYB_PRYSK_BEGIN) || (j > COL_TMO_RPN_UB_PRYB_PRYSK_END)) &&
              (j != (COL_TMO_RPN_UB_PRYB_PRYSK_END + 2)))
              working_ekran[i][j] = ' ';
            else if (j == (COL_TMO_RPN_UB_PRYB_PRYSK_END + 2))
              working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_int_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, view);
          }
          else if (index_of_ekran_tmp == INDEX_ML_TMORPN_PRYSK_VID_UMAX)
          {
            if (
              ((j < COL_TMO_RPN_PRYSK_VID_UMAX_BEGIN) || (j > COL_TMO_RPN_PRYSK_VID_UMAX_END)) &&
              (j != (COL_TMO_RPN_PRYSK_VID_UMAX_END + 2)))
              working_ekran[i][j] = ' ';
            else if (j == COL_TMO_RPN_PRYSK_VID_UMAX_COMMA)
              working_ekran[i][j] = ',';
            else if (j == (COL_TMO_RPN_PRYSK_VID_UMAX_END + 2))
              working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_RPN_PRYSK_VID_UMAX_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == INDEX_ML_TMORPN_PEREKL)
          {
            if (
              ((j < COL_TMO_RPN_PEREKL_BEGIN) || (j > COL_TMO_RPN_PEREKL_END)) &&
              (j != (COL_TMO_RPN_PEREKL_END + 2)))
              working_ekran[i][j] = ' ';
            else if (j == COL_TMO_RPN_PEREKL_COMMA)
              working_ekran[i][j] = ',';
            else if (j == (COL_TMO_RPN_PEREKL_END + 2))
              working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_RPN_PEREKL_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == INDEX_ML_TMORPN_NESPR)
          {
            if (
              ((j < COL_TMO_RPN_NESPR_BEGIN) || (j > COL_TMO_RPN_NESPR_END)) &&
              (j != (COL_TMO_RPN_NESPR_END + 2)))
              working_ekran[i][j] = ' ';
            else if (j == COL_TMO_RPN_NESPR_COMMA)
              working_ekran[i][j] = ',';
            else if (j == (COL_TMO_RPN_NESPR_END + 2))
              working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_RPN_NESPR_COMMA, view, 0);
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
    if (current_ekran.index_position == INDEX_ML_TMORPN_UB_PRYB)
    {
      current_ekran.position_cursor_x = COL_TMO_RPN_UB_PRYB_BEGIN;
      last_position_cursor_x = COL_TMO_RPN_UB_PRYB_END;
    }
    else if (current_ekran.index_position == INDEX_ML_TMORPN_UB_PRYB_PRYSK)
    {
      current_ekran.position_cursor_x = COL_TMO_RPN_UB_PRYB_PRYSK_BEGIN;
      last_position_cursor_x = COL_TMO_RPN_UB_PRYB_PRYSK_END;
    }
    else if (current_ekran.index_position == INDEX_ML_TMORPN_PRYSK_VID_UMAX)
    {
      current_ekran.position_cursor_x = COL_TMO_RPN_PRYSK_VID_UMAX_BEGIN;
      last_position_cursor_x = COL_TMO_RPN_PRYSK_VID_UMAX_END;
    }
    else if (current_ekran.index_position == INDEX_ML_TMORPN_PEREKL)
    {
      current_ekran.position_cursor_x = COL_TMO_RPN_PEREKL_BEGIN;
      last_position_cursor_x = COL_TMO_RPN_PEREKL_END;
    }
    else if (current_ekran.index_position == INDEX_ML_TMORPN_NESPR)
    {
      current_ekran.position_cursor_x = COL_TMO_RPN_NESPR_BEGIN;
      last_position_cursor_x = COL_TMO_RPN_NESPR_END;
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
//Формуємо екран відображення значення управлінської інформації для РПН
/*****************************************************/
void make_ekran_control_rpn()
{
  const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_CONTROL_RPN][MAX_COL_LCD] =
    {
      {" Трансформатор  ",
       "      Режим     ",
       "     Режим ТМ   ",
       " Доп.контр.по U ",
       "Токовая компенс.",
       "      РПН       ",
       " 2-я гр.уставок ",
       "  Основной ТН2  ",
       "   Разр.Тест    "},
      {" Трансформатор  ",
       "      Режим     ",
       "     Режим ТМ   ",
       " Дод.контр.по U ",
       " Струмова комп. ",
       "      РПН       ",
       " 2-а гр.уставок ",
       "  Основний ТН2  ",
       "   Дозв.Тест    "},
      {"  Transformer   ",
       "      Mode      ",
       "    TM Mode     ",
       "Ad.ctrl.of Aux.V",
       " Current Comp.  ",
       "      OLTC      ",
       " Pick-up Set 2  ",
       "  VT2 is Main   ",
       "  Permit Test   "},
      {" Трансформатор  ",
       "      Режим     ",
       "     Режим ТМ   ",
       " Доп.контр.по U ",
       " Токовая комп.  ",
       "      РПН       ",
       " 2-я гр.уставок ",
       "  Основной ТН2  ",
       "   Разр.Тест    "},
    };
  const unsigned char information[MAX_ROW_FOR_CONTROL_RPN][MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] =
    {
      {{"     2 обм.     ", "     3 обм.     "},
       {"     2 обм.     ", "     3 обм.     "},
       {"   2 Windings   ", "   3 Windings   "},
       {"     2 обм.     ", "     3 обм.     "}},
      {{"   Импульсный   ", "  Непрерывный   "},
       {"   Імпульсний   ", "  Безперервний  "},
       {"    Impulse     ", "   Continuous   "},
       {"   Импульсный   ", "  Непрерывный   "}},
      {{"     Откл.      ", "      Вкл.      "},
       {"     Вимк.      ", "     Ввімк.     "},
       {"      Off       ", "       On       "},
       {"     Косу.      ", "     Сљнд.      "}},
      {{"     Откл.      ", "      Вкл.      "},
       {"     Вимк.      ", "     Ввімк.     "},
       {"      Off       ", "       On       "},
       {"     Косу.      ", "     Сљнд.      "}},
      {{"     Откл.      ", "      Вкл.      "},
       {"     Вимк.      ", "     Ввімк.     "},
       {"      Off       ", "       On       "},
       {"     Косу.      ", "     Сљнд.      "}},
      {{"     Откл.      ", "      Вкл.      "},
       {"     Вимк.      ", "     Ввімк.     "},
       {"      Off       ", "       On       "},
       {"     Косу.      ", "     Сљнд.      "}},
      {{"     Откл.      ", "      Вкл.      "},
       {"     Вимк.      ", "     Ввімк.     "},
       {"      Off       ", "       On       "},
       {"     Косу.      ", "     Сљнд.      "}},
      {{"     Откл.      ", "      Вкл.      "},
       {"     Вимк.      ", "     Ввімк.     "},
       {"      Off       ", "       On       "},
       {"     Косу.      ", "     Сљнд.      "}},
      {{"     Откл.      ", "      Вкл.      "},
       {"     Вимк.      ", "     Ввімк.     "},
       {"      Off       ", "       On       "},
       {"     Косу.      ", "     Сљнд.      "}}};
  const unsigned int cursor_x[MAX_ROW_FOR_CONTROL_RPN][MAX_NAMBER_LANGUAGE][2] =
    {
      {{4, 4},
       {4, 4},
       {2, 2},
       {4, 4}},
      {{2, 1},
       {2, 1},
       {3, 2},
       {2, 1}},
      {{4, 5},
       {4, 4},
       {5, 6},
       {4, 4}},
      {{4, 5},
       {4, 4},
       {5, 6},
       {4, 4}},
      {{4, 5},
       {4, 4},
       {5, 6},
       {4, 4}},
      {{4, 5},
       {4, 4},
       {5, 6},
       {4, 4}},
      {{4, 5},
       {4, 4},
       {5, 6},
       {4, 4}},
      {{4, 5},
       {4, 4},
       {5, 6},
       {4, 4}},
      {{4, 5},
       {4, 4},
       {5, 6},
       {4, 4}}};

  unsigned char name_string_tmp[MAX_ROW_FOR_CONTROL_RPN][MAX_COL_LCD];
  unsigned char information_tmp[MAX_ROW_FOR_CONTROL_RPN][2][MAX_COL_LCD];
  unsigned int cursor_x_tmp[MAX_ROW_FOR_CONTROL_RPN][2];

  int const index_language = index_language_in_array(current_settings.language);

  for (int index_1 = 0; index_1 < MAX_ROW_FOR_CONTROL_RPN; index_1++)
  {
    for (int index_2 = 0; index_2 < MAX_COL_LCD; index_2++)
    {
      name_string_tmp[index_1][index_2] = name_string[index_language][index_1][index_2];

      for (int index_3 = 0; index_3 < 2; index_3++)
        information_tmp[index_1][index_3][index_2] = information[index_1][index_language][index_3][index_2];
    }

    for (int index_3 = 0; index_3 < 2; index_3++)
      cursor_x_tmp[index_1][index_3] = cursor_x[index_1][index_language][index_3];
  }

  unsigned int additional_current = 0;
  unsigned int position_temp = current_ekran.index_position;

  unsigned int temp_data;
  if (current_ekran.edition == 0)
    temp_data = current_settings.control_rpn;
  else
    temp_data = edition_settings.control_rpn;

  if ((temp_data & (1u << INDEX_ML_CTRRPN_TRANSF)) == 0)
  {
    /*************************************************************/
    //У випадку, якщо вибрано двообмотковий трансформатор, то відобпраження можливості встановлення основним ТН2 прибираємо
    /*************************************************************/
    while (additional_current < 2)
    {
      //Першою фільтруємо настройку з більшим індексом, щоб не мати порблем з формуванням маски, коли вже попердньо перша фільтрація проведена
      unsigned int index_deleted_feild;

      if (additional_current == 0)
        index_deleted_feild = INDEX_ML_CTRRPN_OSNOVNYJ_TN2;
      else
        index_deleted_feild = INDEX_ML_CTRRPN_DOD_KONTUR;

      /*************************************************************/
      //Відкидаємо ім'я даного поля і зміщаємо біти
      /*************************************************************/

      //Формуємо маску біт, які не треба переміщати при переміщенні імен полів
      unsigned int maska = 0;
      for (unsigned int j = 0; j < index_deleted_feild; j++)
        maska |= (1 << j);

      /***/
      //Зміщуємо біти стану реанжування функцій разом із їх назвами
      /***/
      unsigned int new_temp_data_1, new_temp_data_2;

      new_temp_data_1 = temp_data & maska;

      new_temp_data_2 = temp_data & ((unsigned int) (~maska));
      new_temp_data_2 = new_temp_data_2 >> 1;
      new_temp_data_2 &= (unsigned int) (~maska);

      temp_data = new_temp_data_1 | new_temp_data_2;
      /***/
      for (unsigned int j = index_deleted_feild; j < (MAX_ROW_FOR_CONTROL_RPN - additional_current); j++)
      {
        if ((j + 1) < (MAX_ROW_FOR_CONTROL_RPN - additional_current))
        {
          for (unsigned int k = 0; k < MAX_COL_LCD; k++)
          {
            name_string_tmp[j][k] = name_string_tmp[j + 1][k];

            for (unsigned int l = 0; l < 2; l++)
              information_tmp[j][l][k] = information_tmp[j + 1][l][k];
          }

          for (unsigned int l = 0; l < 2; l++)
            cursor_x_tmp[j][l] = cursor_x_tmp[j + 1][l];
        }
        else
        {
          for (unsigned int k = 0; k < MAX_COL_LCD; k++)
          {
            name_string_tmp[j][k] = ' ';

            for (unsigned int l = 0; l < 2; l++)
              information_tmp[j][l][k] = ' ';
          }

          for (unsigned int l = 0; l < 2; l++)
            cursor_x_tmp[j][l] = 0;
        }
      }
      if (current_ekran.index_position >= ((int) index_deleted_feild))
        position_temp--;
      additional_current++;
      /*************************************************************/
    }
  }

  unsigned int index_of_ekran;

  //Множення на два величини position_temp потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
  index_of_ekran = ((position_temp << 1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
  {
    unsigned int index_of_ekran_tmp = index_of_ekran >> 1;
    if (index_of_ekran_tmp < (unsigned int) (MAX_ROW_FOR_CONTROL_RPN))
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j < MAX_COL_LCD; j++)
          working_ekran[i][j] = name_string_tmp[index_of_ekran_tmp][j];
      }
      else
      {
        //У парному номері рядку виводимо значення

        for (unsigned int j = 0; j < MAX_COL_LCD; j++)
          working_ekran[i][j] = information_tmp[index_of_ekran_tmp][(temp_data >> index_of_ekran_tmp) & 0x1][j];
        current_ekran.position_cursor_x = cursor_x_tmp[index_of_ekran_tmp][(temp_data >> index_of_ekran_tmp) & 0x1];
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
