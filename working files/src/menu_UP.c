#include "header.h"

typedef struct _vd_stp
{
  uint32_t n;
  uint32_t index_arr;
  uint32_t view;
  int sign;
  unsigned int begin;
  unsigned int comma;
  unsigned int end;
  unsigned int u_begin;
  unsigned int u_end;
  const unsigned char *p_unit;
} __vd_stp;

typedef struct _vd_tmo
{
  int sign;
  unsigned int begin;
  unsigned int comma;
  unsigned int end;
  unsigned int u_begin;
  unsigned int u_end;
  const unsigned char *p_unit;
} __vd_tmo;

/*****************************************************/
//Формуємо екран відображення уставок Універсального захисту
/*****************************************************/
void make_ekran_setpoint_UP(unsigned int group)
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_SETPOINT_UP][MAX_COL_LCD] =
    {
      {"      УЗ x      ",
       "   УЗ x возвр.  ",
       "    КВ УЗ x     "},
      {"      УЗ x      ",
       "    УЗ x пов.   ",
       "    КП УЗ x     "},
      {"     MFP x      ",
       "   MFP x Rel    ",
       "    MFP x RR    "},
      {"      УЗ x      ",
       "   УЗ x возвр.  ",
       "    КВ УЗ x     "}};
  static const uint32_t index_number[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_SETPOINT_UP] =
    {
      {9, 6, 10},
      {9, 7, 10},
      {9, 7, 8},
      {9, 6, 10}};

  int index_language = index_language_in_array(current_settings.language);
  unsigned int position_temp = current_ekran.index_position;

  __vd_stp vd[NUMBER_UP * MAX_ROW_FOR_SETPOINT_UP];
  uint32_t vaga_arr[NUMBER_UP * MAX_ROW_FOR_SETPOINT_UP];
  int32_t *p_value[NUMBER_UP * MAX_ROW_FOR_SETPOINT_UP];

  size_t pos = 0;
  size_t rem_before = 0;
  for (size_t i = 0; i < (NUMBER_UP * MAX_ROW_FOR_SETPOINT_UP); i++)
  {
    unsigned int view = ((current_ekran.edition == 0) || (position_temp != i));

    uint32_t _n_UP = i / MAX_ROW_FOR_SETPOINT_UP;
    uint32_t const index = i - _n_UP * MAX_ROW_FOR_SETPOINT_UP;

    uint32_t in_canal = current_settings.ctrl_UP_input[_n_UP];

    if (
      ((in_canal < UP_CTRL_F) && (index != INDEX_ML_STP_UP_POV)) ||
      ((in_canal == UP_CTRL_F) && (index != INDEX_ML_STP_UP_KP)))
    {
      vd[pos].n = _n_UP;
      vd[pos].index_arr = index;
      vd[pos].view = view;

      vd[pos].sign = -1;

      if (
        (index == INDEX_ML_STP_UP) ||
        (index == INDEX_ML_STP_UP_POV))
      {
        switch (in_canal)
        {
          case UP_CTRL_Ia_TN1:
          case UP_CTRL_Ia_TN2:
            {
              vd[pos].begin = COL_SETPOINT_UP_I_BEGIN;
              vd[pos].comma = COL_SETPOINT_UP_I_COMMA;
              vd[pos].end = COL_SETPOINT_UP_I_END;
              vd[pos].u_begin = COL_SETPOINT_UP_I_END + 2;
              vd[pos].u_end = COL_SETPOINT_UP_I_END + 2 + 1 - 1;
              vd[pos].p_unit = &odynyci_vymirjuvannja[index_language][INDEX_A];

              vaga_arr[i] = 100000;

              break;
            }
          case UP_CTRL_Uab_TN1:
          case UP_CTRL_Uab_TN2:
            {
              vd[pos].begin = COL_SETPOINT_UP_U_BEGIN;
              vd[pos].comma = COL_SETPOINT_UP_U_COMMA;
              vd[pos].end = COL_SETPOINT_UP_U_END;
              vd[pos].u_begin = COL_SETPOINT_UP_U_END + 2;
              vd[pos].u_end = COL_SETPOINT_UP_U_END + 2 + 1 - 1;
              vd[pos].p_unit = &odynyci_vymirjuvannja[index_language][INDEX_V];

              vaga_arr[i] = 100000;

              break;
            }
          case UP_CTRL_F:
            {
              vd[pos].begin = COL_STP_UP_F_BEGIN;
              vd[pos].comma = COL_STP_UP_F_COMMA;
              vd[pos].end = COL_STP_UP_F_END;
              vd[pos].u_begin = COL_STP_UP_F_END + 2;
              vd[pos].u_end = COL_STP_UP_F_END + 2 + 2 - 1;
              vd[pos].p_unit = Hz[index_language];

              vaga_arr[pos] = 1000;

              break;
            }
          default:
            {
              //Теоретично цього ніколи не мало б бути
              total_error_sw_fixed();

              break;
            }
        }

        if (view == true)
          p_value[i] = current_settings.setpoint_UP[_n_UP][index]; //у змінну value поміщаємо значення уставки
        else
          p_value[i] = edition_settings.setpoint_UP[_n_UP][index];
      }
      else if (index == INDEX_ML_STP_UP_KP)
      {
        vd[pos].begin = COL_SETPOINT_UP_KP_BEGIN;
        vd[pos].comma = COL_SETPOINT_UP_KP_COMMA;
        vd[pos].end = COL_SETPOINT_UP_KP_END;
        vd[pos].u_begin = COL_SETPOINT_UP_KP_END + 2;
        vd[pos].u_end = COL_SETPOINT_UP_KP_END + 2 + 1 - 1;
        vd[pos].p_unit = NULL;

        vaga_arr[i] = 100;

        if (view == true)
          p_value[i] = (int32_t *) current_settings.setpoint_UP_KP[_n_UP]; //у змінну value поміщаємо значення уставки
        else
          p_value[i] = (int32_t *) edition_settings.setpoint_UP_KP[_n_UP];
      }

      ++pos;
    }
    else
    {
      if (i < position_temp)
        ++rem_before;
    }
  }
  position_temp -= rem_before;

  unsigned int vaga, value, first_symbol;

  //Множення на два величини position_temp потрібне для того, бо наодн позицію ми використовуємо два рядки (назва + значення)
  unsigned int index_of_ekran = ((position_temp << 1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
  {
    unsigned int index_of_ekran_tmp = index_of_ekran >> 1;
    if (index_of_ekran_tmp < pos)
    {
      if ((i & 0x1) == 0)
      {
        uint32_t _n_UP = vd[index_of_ekran_tmp].n;
        uint32_t _n_index = vd[index_of_ekran_tmp].index_arr;
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j < MAX_COL_LCD; j++)
        {
          working_ekran[i][j] = (j != index_number[index_language][_n_index]) ? name_string[index_language][_n_index][j] : (_n_UP + 1 + 0x30);
        }
        vaga = vaga_arr[index_of_ekran_tmp];
        value = abs(*(p_value[index_of_ekran_tmp] + group));

        first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
      }
      else
      {
        //У парному номері рядку виводимо значення уставки
        unsigned int const view = vd[index_of_ekran_tmp].view;
        for (unsigned int j = 0; j < MAX_COL_LCD; j++)
        {
          if (j == vd[index_of_ekran_tmp].sign)
          {
            working_ekran[i][j] = (*(p_value[index_of_ekran_tmp] + group) < 0) ? '-' : ' ';
          }
          else if (
            ((j < vd[index_of_ekran_tmp].begin) || (j > vd[index_of_ekran_tmp].end)) &&
            ((vd[index_of_ekran_tmp].p_unit == NULL) ||
             (!((j >= (vd[index_of_ekran_tmp].end + 2)) && (j <= (vd[index_of_ekran_tmp].end + 2 + vd[index_of_ekran_tmp].u_end - vd[index_of_ekran_tmp].u_begin))))))
            working_ekran[i][j] = ' ';
          else if (j == vd[index_of_ekran_tmp].comma)
            working_ekran[i][j] = ','; /*якщо коми не потрібно, то треба щоб comma була меншою за begin і тоді до ціє частини коду програма не дійде*/
          else if (
            (vd[index_of_ekran_tmp].p_unit != NULL) &&
            ((j >= (vd[index_of_ekran_tmp].end + 2)) && (j <= (vd[index_of_ekran_tmp].end + 2 + vd[index_of_ekran_tmp].u_end - vd[index_of_ekran_tmp].u_begin))))
            working_ekran[i][j] = vd[index_of_ekran_tmp].p_unit[j - vd[index_of_ekran_tmp].u_begin];
          else
            calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, vd[index_of_ekran_tmp].comma, view, 0);
        }

        if (
          (vd[index_of_ekran_tmp].sign >= 0) &&
          (view == true))
        {
          //Підтягуємо знак до першої цифри
          for (int32_t j = vd[index_of_ekran_tmp].sign; j < (int32_t) MAX_COL_LCD; j++)
          {
            if (((working_ekran[i][j + 1]) == ' ') && (j < ((int32_t) vd[index_of_ekran_tmp].end - 1)))
            {
              working_ekran[i][j + 1] = working_ekran[i][j];
              working_ekran[i][j] = ' ';
              vd[index_of_ekran_tmp].sign++;
            }
            else
              break;
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
    if (vd[current_ekran.index_position].sign < 0)
      current_ekran.position_cursor_x = vd[current_ekran.index_position].begin;
    else
      current_ekran.position_cursor_x = vd[current_ekran.index_position].sign;
    last_position_cursor_x = vd[current_ekran.index_position].end;

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
//Формуємо екран відображення витримок Універсального захисту
/*****************************************************/
void make_ekran_timeout_UP(unsigned int group)
{
  static const uint8_t name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_TIMEOUT_UP][MAX_COL_LCD] =
    {
      {"      УЗ x      "},
      {"      УЗ x      "},
      {"     MFP x      "},
      {"      УЗ x      "}};
  static const uint32_t index_number[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_TIMEOUT_UP] =
    {
      {9},
      {9},
      {9},
      {9}};

  int index_language = index_language_in_array(current_settings.language);

  const __vd_tmo vd[MAX_ROW_FOR_TIMEOUT_UP] =
    {
      {-1, COL_TMO_UP_BEGIN, COL_TMO_UP_COMMA, COL_TMO_UP_END, COL_TMO_UP_END + 2, COL_TMO_UP_END + 2, &odynyci_vymirjuvannja[index_language][INDEX_SECOND]}};
  static const uint32_t vaga_arr[MAX_ROW_FOR_TIMEOUT_UP] =
    {
      100000};
  static int32_t(*const p_value_current[MAX_ROW_FOR_TIMEOUT_UP])[NUMBER_GROUP_USTAVOK] =
    {
      current_settings.timeout_UP};
  static int32_t(*const p_value_edit[MAX_ROW_FOR_TIMEOUT_UP])[NUMBER_GROUP_USTAVOK] =
    {
      edition_settings.timeout_UP};

  unsigned int position_temp = current_ekran.index_position;

  int index_of_ekran;
  unsigned int vaga, value, first_symbol;

  //Множення на два величини position_temp потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
  index_of_ekran = ((position_temp << 1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
  {
    if (index_of_ekran < ((NUMBER_UP * MAX_ROW_FOR_TIMEOUT_UP) << 1)) //Множення на два константи потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
    {
      unsigned int index_of_ekran_tmp = index_of_ekran >> 1;
      uint32_t _n_index = index_of_ekran_tmp % MAX_ROW_FOR_TIMEOUT_UP;
      unsigned int view = ((current_ekran.edition == 0) || (position_temp != index_of_ekran_tmp));
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        uint32_t _n_UP = index_of_ekran_tmp / MAX_ROW_FOR_TIMEOUT_UP;
        for (unsigned int j = 0; j < MAX_COL_LCD; j++)
        {
          working_ekran[i][j] = (j != index_number[index_language][_n_index]) ? name_string[index_language][_n_index][j] : (_n_UP + 1 + 0x30);
        }

        vaga = vaga_arr[_n_index];
        if (view == true)
          value = p_value_current[_n_index][_n_UP][group]; //у змінну value поміщаємо значення витримки
        else
          value = p_value_edit[_n_index][_n_UP][group];

        first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
      }
      else
      {
        //У парному номері рядку виводимо значення уставки
        for (unsigned int j = 0; j < MAX_COL_LCD; j++)
        {
          if (
            ((j < vd[_n_index].begin) || (j > vd[_n_index].end)) &&
            ((vd[_n_index].p_unit == NULL) ||
             (!((j >= (vd[_n_index].end + 2)) && (j <= (vd[_n_index].end + 2 + vd[_n_index].u_end - vd[_n_index].u_begin))))))
            working_ekran[i][j] = ' ';
          else if (j == vd[_n_index].comma)
            working_ekran[i][j] = ','; /*якщо коми не потрібно, то треба щоб comma була меншою за begin і тоді до ціє частини коду програма не дійде*/
          else if (
            (vd[_n_index].p_unit != NULL) &&
            ((j >= (vd[_n_index].end + 2)) && (j <= (vd[_n_index].end + 2 + vd[_n_index].u_end - vd[_n_index].u_begin))))
            working_ekran[i][j] = vd[_n_index].p_unit[j - vd[_n_index].u_begin];
          else
            calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, vd[_n_index].comma, view, 0);
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
    uint32_t _n_index = current_ekran.index_position % MAX_ROW_FOR_TIMEOUT_UP;

    int last_position_cursor_x = MAX_COL_LCD;
    current_ekran.position_cursor_x = vd[_n_index].begin;
    last_position_cursor_x = vd[_n_index].end;

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
//Формуємо екран відображення значення управлінської інформації для Універсального Захисту
/*****************************************************/
void make_ekran_control_UP()
{
  static const uint8_t name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_CONTROL_UP][MAX_COL_LCD] =
    {
      {"   Вход УЗ x    ",
       "      УЗ x      ",
       " >/< для УЗ x   "},
      {"   Вхід УЗ x    ",
       "      УЗ x      ",
       " >/< для УЗ x   "},
      {"    MFP x AI    ",
       "     MFP x      ",
       " >/< for MFP x  "},
      {"   Вход УЗ x    ",
       "      УЗ x      ",
       " >/< для УЗ x   "}};
  static const uint32_t index_number[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_CONTROL_UP] =
    {
      {11, 9, 12},
      {11, 9, 12},
      {11, 9, 12}};

  static uint8_t const information_2[MAX_NAMBER_LANGUAGE][_UP_CTRL_NUMBER][MAX_COL_LCD] =
    {
      {"     Ia ТН1     ",
       "     Ia ТН2     ",
       "    Uab ТН1     ",
       "    Uab ТН2     ",
       "       f        "},
      {"     Ia ТН1     ",
       "     Ia ТН2     ",
       "    Uab ТН1     ",
       "    Uab ТН2     ",
       "       f        "},
      {"     Ia TN1     ",
       "     Ia TN2     ",
       "    Vab TN1     ",
       "    Vab TN2     ",
       "       f        "},
      {"     Ia ТН1     ",
       "     Ia ТН2     ",
       "    Uab ТН1     ",
       "    Uab ТН2     ",
       "       f        "}};
  static const uint32_t cursor_x_2[_UP_CTRL_NUMBER] = {4, 4, 3, 3, 6};

  static const uint8_t information_4[2][MAX_COL_LCD] =
    {
      "       >        ",
      "       <        "};
  static const uint32_t cursor_x_4 = 6;

  int const index_language = index_language_in_array(current_settings.language);

  __SETTINGS *point = (current_ekran.edition == 0) ? &current_settings : &edition_settings;

  __ctrl_info ctrl_info[NUMBER_UP * MAX_ROW_FOR_CONTROL_UP];
  for (size_t i = 0; i < (NUMBER_UP * MAX_ROW_FOR_CONTROL_UP); i++)
  {
    switch (i)
    {
      case 0:
      case 3:
      case 6:
      case 9:
      case 12:
      case 15:
      case 18:
      case 21:
        {
          uint32_t index = point->ctrl_UP_input[i / MAX_ROW_FOR_CONTROL_UP];
          ctrl_info[i].information = information_2[index_language][index];
          ctrl_info[i].cursor_x = cursor_x_2[index];
          break;
        }
      case 1:
      case 4:
      case 7:
      case 10:
      case 13:
      case 16:
      case 19:
      case 22:
        {
          uint32_t index = (point->control_UP >> ((i / MAX_ROW_FOR_CONTROL_UP) * (_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_STATE_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I))) & 0x1;
          ctrl_info[i].information = information_off_on[index_language][index];
          ctrl_info[i].cursor_x = cursor_x_off_on[index_language][index];
          break;
        }
      case 2:
      case 5:
      case 8:
      case 11:
      case 14:
      case 17:
      case 20:
      case 23:
        {
          uint32_t index = (point->control_UP >> ((i / MAX_ROW_FOR_CONTROL_UP) * (_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_MORE_LESS_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I))) & 0x1;
          ctrl_info[i].information = information_4[index];
          ctrl_info[i].cursor_x = cursor_x_4;
          break;
        }
      default:
        {
          //Теоретично цього ніколи не мало б бути
          total_error_sw_fixed();
        }
    }
  }

  uint8_t name_string_tmp[NUMBER_UP * MAX_ROW_FOR_CONTROL_UP][MAX_COL_LCD];

  for (size_t index_1 = 0; index_1 < (NUMBER_UP * MAX_ROW_FOR_CONTROL_UP); index_1++)
  {
    uint32_t index_1_tmp = index_1 % MAX_ROW_FOR_CONTROL_UP;
    uint32_t index_number_tmp = index_number[index_language][index_1_tmp];
    for (size_t index_2 = 0; index_2 < MAX_COL_LCD; index_2++)
    {
      name_string_tmp[index_1][index_2] = (index_2 != index_number_tmp) ? name_string[index_language][index_1_tmp][index_2] : ((index_1 / MAX_ROW_FOR_CONTROL_UP) + 1 + 0x30);
    }
  }

  int32_t position_temp = current_ekran.index_position;

  //Множення на два величини position_temp потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
  int32_t index_of_ekran = ((position_temp << 1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  for (size_t i = 0; i < MAX_ROW_LCD; i++)
  {
    int32_t index_of_ekran_tmp = index_of_ekran >> 1;
    if (index_of_ekran_tmp < (NUMBER_UP * MAX_ROW_FOR_CONTROL_UP)) //Множення на два константи потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (size_t j = 0; j < MAX_COL_LCD; j++)
          working_ekran[i][j] = name_string_tmp[index_of_ekran_tmp][j];
      }
      else
      {
        //У парному номері рядку виводимо значення уставки

        for (size_t j = 0; j < MAX_COL_LCD; j++)
          working_ekran[i][j] = ctrl_info[index_of_ekran_tmp].information[j];
        if (position_temp == index_of_ekran_tmp)
          current_ekran.position_cursor_x = ctrl_info[index_of_ekran_tmp].cursor_x;
      }
    }
    else
      for (size_t j = 0; j < MAX_COL_LCD; j++)
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
