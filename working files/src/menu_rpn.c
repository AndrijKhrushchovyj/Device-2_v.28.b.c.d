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
//Формуємо екран відображення уставок МТЗ
/*****************************************************/
void make_ekran_setpoint_mtz(unsigned int group)
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
//Формуємо екран відображення витримок МТЗ
/*****************************************************/
void make_ekran_timeout_mtz(unsigned int group)
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
//Формуємо екран відображення значення управлінської інформації для МТЗ
/*****************************************************/
void make_ekran_control_mtz()
{
  static const unsigned char name_string_withoutznam[MAX_NAMBER_LANGUAGE][WITHOUTZNAM_MAX_ROW_FOR_CONTROL_MTZ][MAX_COL_LCD] =
    {
      {"     МТЗ 1      ",
       "   Тип МТЗ 1    ",
       "   МТЗН 1 Пр.   ",
       "  МТЗН 1 Обр.   ",
       "     МТЗ 2      ",
       "   Тип МТЗ 2    ",
       "   МТЗН 2 Пр.   ",
       "  МТЗН 2 Обр.   ",
       " Ускорение МТЗ 2",
       "Ускоренная МТЗ 2",
       "     МТЗ 3      ",
       "   Тип МТЗ 3    ",
       "   МТЗН 3 Пр.   ",
       "  МТЗН 3 Обр.   ",
       "     МТЗ 4      ",
       "   Тип МТЗ 4    ",
       "   МТЗН 4 Пр.   ",
       "  МТЗН 4 Обр.   ",
       "    НЦН-МТЗ     "},
      {"     МСЗ 1      ",
       "   Тип МСЗ 1    ",
       " МСЗС 1 Прямий  ",
       "  МСЗС 1 Звор.  ",
       "     МСЗ 2      ",
       "   Тип МСЗ2     ",
       " МСЗС 2 Прямий  ",
       "  МСЗС 2 Звор.  ",
       "Прискорення МСЗ2",
       "Прискорений МСЗ2",
       "     МСЗ 3      ",
       "   Тип МСЗ 3    ",
       " МСЗС 3 Прямий  ",
       "  МСЗС 3 Звор.  ",
       "     МСЗ 4      ",
       "   Тип МСЗ 4    ",
       " МСЗС 4 Прямий  ",
       "  МСЗС 4 Звор.  ",
       "    НКН-МСЗ     "},
      {"     OCP 1      ",
       "   OCP 1 Type   ",
       "   DOCP 1 Fw    ",
       "   DOCP 1 Bw    ",
       "     OCP 2      ",
       "   OCP 2 Type   ",
       "   DOCP 2 Fw    ",
       "   DOCP 2 Bw    ",
       " OCP 2 Acc Ena  ",
       "  OCP 2 Acc C   ",
       "     OCP 3      ",
       "   OCP 3 Type   ",
       "   DOCP 3 Fw    ",
       "   DOCP 3 Bw    ",
       "     OCP 4      ",
       "   OCP 4 Type   ",
       "   DOCP 4 Fw    ",
       "   DOCP 4 Bw    ",
       "    OCP VCM     "},
      {"     МТЗ 1      ",
       "   Тип МТЗ 1    ",
       "   МТЗН 1 Пр.   ",
       "  МТЗН 1 Обр.   ",
       "     МТЗ 2      ",
       "   Тип МТЗ 2    ",
       "   МТЗН 2 Пр.   ",
       "  МТЗН 2 Обр.   ",
       " Ускорение МТЗ 2",
       "Ускоренная МТЗ 2",
       "     МТЗ 3      ",
       "   Тип МТЗ 3    ",
       "   МТЗН 3 Пр.   ",
       "  МТЗН 3 Обр.   ",
       "     МТЗ 4      ",
       "   Тип МТЗ 4    ",
       "   МТЗН 4 Пр.   ",
       "  МТЗН 4 Обр.   ",
       "    НЦН-МТЗ     "}};

  int index_language = index_language_in_array(current_settings.language);

  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;

  //Множення на два величини position_temp потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
  index_of_ekran = ((position_temp << 1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
  {
    unsigned int index_of_ekran_tmp = index_of_ekran >> 1;
    if (index_of_ekran_tmp < (unsigned int) (WITHOUTZNAM_MAX_ROW_FOR_CONTROL_MTZ))
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j < MAX_COL_LCD; j++)
          working_ekran[i][j] = name_string_withoutznam[index_language][index_of_ekran_tmp][j];
      }
      else
      {
        //У парному номері рядку виводимо значення уставки
        unsigned int index_ctr = index_of_ekran_tmp;

        __SETTINGS *point;
        if (current_ekran.edition == 0)
          point = &current_settings;
        else
          point = &edition_settings;

        if (
          (index_ctr == WITHOUTZNAM_INDEX_ML_CTRMTZ_1_TYPE) ||
          (index_ctr == WITHOUTZNAM_INDEX_ML_CTRMTZ_2_TYPE) ||
          (index_ctr == WITHOUTZNAM_INDEX_ML_CTRMTZ_3_TYPE) ||
          (index_ctr == WITHOUTZNAM_INDEX_ML_CTRMTZ_4_TYPE))
        {
        }
        else
        {
          unsigned int temp_data = point->control_mtz;
          unsigned int n_bit = 0;

          //Виділяємо номер біту
          if (index_ctr == WITHOUTZNAM_INDEX_ML_CTRMTZ_1)
            n_bit = N_BIT_CTRMTZ_1;
          else if (index_ctr == WITHOUTZNAM_INDEX_ML_CTRMTZ_1_VPERED)
            n_bit = N_BIT_CTRMTZ_1_VPERED;
          else if (index_ctr == WITHOUTZNAM_INDEX_ML_CTRMTZ_1_NAZAD)
            n_bit = N_BIT_CTRMTZ_1_NAZAD;
          //            else if (index_ctr == WITHOUTZNAM_INDEX_ML_CTRMTZ_1_ZNAM        ) n_bit = N_BIT_CTRMTZ_1_ZNAM;
          else if (index_ctr == WITHOUTZNAM_INDEX_ML_CTRMTZ_2)
            n_bit = N_BIT_CTRMTZ_2;
          else if (index_ctr == WITHOUTZNAM_INDEX_ML_CTRMTZ_2_VPERED)
            n_bit = N_BIT_CTRMTZ_2_VPERED;
          else if (index_ctr == WITHOUTZNAM_INDEX_ML_CTRMTZ_2_NAZAD)
            n_bit = N_BIT_CTRMTZ_2_NAZAD;
          else if (index_ctr == WITHOUTZNAM_INDEX_ML_CTRMTZ_2_PRYSKORENNJA)
            n_bit = N_BIT_CTRMTZ_2_PRYSKORENNJA;
          else if (index_ctr == WITHOUTZNAM_INDEX_ML_CTRMTZ_2_PRYSKORENA)
            n_bit = N_BIT_CTRMTZ_2_PRYSKORENA;
          //            else if (index_ctr == WITHOUTZNAM_INDEX_ML_CTRMTZ_2_ZNAM        ) n_bit = N_BIT_CTRMTZ_2_ZNAM;
          else if (index_ctr == WITHOUTZNAM_INDEX_ML_CTRMTZ_3)
            n_bit = N_BIT_CTRMTZ_3;
          else if (index_ctr == WITHOUTZNAM_INDEX_ML_CTRMTZ_3_VPERED)
            n_bit = N_BIT_CTRMTZ_3_VPERED;
          else if (index_ctr == WITHOUTZNAM_INDEX_ML_CTRMTZ_3_NAZAD)
            n_bit = N_BIT_CTRMTZ_3_NAZAD;
          //            else if (index_ctr == WITHOUTZNAM_INDEX_ML_CTRMTZ_3_ZNAM        ) n_bit = N_BIT_CTRMTZ_3_ZNAM;
          else if (index_ctr == WITHOUTZNAM_INDEX_ML_CTRMTZ_4)
            n_bit = N_BIT_CTRMTZ_4;
          else if (index_ctr == WITHOUTZNAM_INDEX_ML_CTRMTZ_4_VPERED)
            n_bit = N_BIT_CTRMTZ_4_VPERED;
          else if (index_ctr == WITHOUTZNAM_INDEX_ML_CTRMTZ_4_NAZAD)
            n_bit = N_BIT_CTRMTZ_4_NAZAD;
          //            else if (index_ctr == WITHOUTZNAM_INDEX_ML_CTRMTZ_4_ZNAM        ) n_bit = N_BIT_CTRMTZ_4_ZNAM;
          else if (index_ctr == WITHOUTZNAM_INDEX_ML_CTRMTZ_NESPR_KIL_NAPR)
            n_bit = N_BIT_CTRMTZ_NESPR_KIL_NAPR;

          for (unsigned int j = 0; j < MAX_COL_LCD; j++)
            working_ekran[i][j] = information_off_on[index_language][(temp_data >> n_bit) & 0x1][j];
          if (position_temp == index_of_ekran_tmp)
            current_ekran.position_cursor_x = cursor_x_off_on[index_language][(temp_data >> n_bit) & 0x1];
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
