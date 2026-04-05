#include "header.h"

/*****************************************************/
//Формуємо екран відображення стану регулювання
/*****************************************************/
void make_ekran_regulation(void)
{
  int const index_language = index_language_in_array(current_settings.language);

  for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
  {
    //Попередньо очищаємо цілий робочий екран
    for (unsigned int j = 0; j < MAX_COL_LCD; j++)
      working_ekran[i][j] = ' ';
  }

  unsigned int digital_signals[N_BIG];
  int timer_rpn_ub_pryb_tmp;
  int timer_rpn_ub_pryb_prysk_tmp;
  int timer_rpn_prysk_vid_Umax_tmp;
  unsigned int number_group_stp_tmp = 0;

  copying_active_functions_for_lower_moduls = 1; //Помічаємо, що зараз будемо зчитувати значення активних функцій

  for (unsigned int i = 0; i < N_BIG; i++)
  {
    digital_signals[i] = active_functions_for_lower_moduls[i];
  }

  timer_rpn_ub_pryb_tmp = timer_rpn_ub_pryb_low;
  timer_rpn_ub_pryb_prysk_tmp = timer_rpn_ub_pryb_prysk_low;
  timer_rpn_prysk_vid_Umax_tmp = timer_rpn_prysk_vid_Umax_low;

  number_group_stp_tmp = number_group_stp_low;

  copying_active_functions_for_lower_moduls = 0; //Помічаємо, що зчитування значеннь активних функцій завершене

  //Визначаємо номер основного каналу
  unsigned int number_main_canal = 1 + (_CHECK_SET_BIT(digital_signals, RANG_OSNOVNYJ_TN2_RPN) != 0);

  /*******************************/
  //Визначаємо струм-напругу основного і допоміжного каналів
  /*******************************/
  //  unsigned int triple_wound = ((current_settings.control_rpn & MASKA_FOR_BIT(INDEX_ML_CTRRPN_TRANSF)) != 0);
  unsigned int base_faze_current = 0 /*, second_faze_current = 0*/;
  unsigned int base_faze_voltage = 0 /*, second_faze_voltage = 0*/;
  unsigned int TCurrent_base = 1, TVoltage_base = 1;
  switch (number_main_canal)
  {
    case 1:
      {
        base_faze_current = measurement[I_IA_1];
        base_faze_voltage = measurement[I_UAB_TN1];

        TCurrent_base = current_settings.TCurrent1;
        TVoltage_base = current_settings.TVoltage1;

        //      if (triple_wound)
        //      {
        //        second_faze_current = measurement[I_IA_2];
        //        second_faze_voltage = measurement[I_UAB_TN2];
        //      }

        break;
      }
    case 2:
      {
        base_faze_current = measurement[I_IA_2];
        base_faze_voltage = measurement[I_UAB_TN2];

        TCurrent_base = current_settings.TCurrent2;
        TVoltage_base = current_settings.TVoltage2;

        //      if (triple_wound)
        //      {
        //        second_faze_current = measurement[I_IA_1];
        //        second_faze_voltage = measurement[I_UAB_TN1];
        //      }

        break;
      }
    default:
      {
        //Теоретично цього ніколи не мало б бути
        total_error_sw_fixed();
        break;
      }
  }
  /*******************************/

  /*******************************/
  //Спочатку підраховуємо кількість подій, які розглядаються нештатними
  /*******************************/
  int max_number_events = 0;
  if (
    (_CHECK_SET_BIT(digital_signals, RANG_PEREKLYUCHENNYA_RPN) != 0) ||
    (_CHECK_SET_BIT(digital_signals, RANG_UBAVYTY_RPN) != 0) ||
    (_CHECK_SET_BIT(digital_signals, RANG_PRYBAVYTY_RPN) != 0) ||
    (timer_rpn_ub_pryb_tmp >= 0) ||
    (timer_rpn_ub_pryb_prysk_tmp >= 0) ||
    (timer_rpn_prysk_vid_Umax_tmp >= 0))
    max_number_events++;
  if (_CHECK_SET_BIT(digital_signals, RANG_AVAR_DEFECT) != 0)
    max_number_events++;
  if (_CHECK_SET_BIT(digital_signals, RANG_ZNKh) != 0)
    max_number_events++;
  if (_CHECK_SET_BIT(digital_signals, RANG_ZSKh) != 0)
    max_number_events++;
  if (_CHECK_SET_BIT(digital_signals, RANG_ZASTRJAVANNJA_RPN) != 0)
    max_number_events++;
  if (_CHECK_SET_BIT(digital_signals, RANG_PRYVID_NE_PISHOV_RPN) != 0)
    max_number_events++;
  if (_CHECK_SET_BIT(digital_signals, RANG_N_PER_RIVNE_N_PER_USTAVKY_RPN) != 0)
    max_number_events++;
  if (_CHECK_SET_BIT(digital_signals, RANG_1_POLOGENNJA_RPN) != 0)
    max_number_events++;
  if (_CHECK_SET_BIT(digital_signals, RANG_N_POLOGENNJA_RPN) != 0)
    max_number_events++;
  if (_CHECK_SET_BIT(digital_signals, RANG_MRZS_OR_LOCAL_MODE_RPN) == 0)
    max_number_events++;
  if (_CHECK_SET_BIT(digital_signals, RANG_REMOTE_MODE_RPN) != 0)
    max_number_events++;
  if (_CHECK_SET_BIT(digital_signals, RANG_UMIN1) != 0)
    max_number_events++;
  if (_CHECK_SET_BIT(digital_signals, RANG_UMIN2) != 0)
    max_number_events++;
  if (_CHECK_SET_BIT(digital_signals, RANG_UMAX2) != 0)
    max_number_events++;
  if (_CHECK_SET_BIT(digital_signals, RANG_PO4_U_OSN_RPN) != 0)
    max_number_events++;
  if (
    ((current_settings.control_rpn & MASKA_FOR_BIT(INDEX_ML_CTRRPN_DOD_KONTUR)) != 0) &&
    (_CHECK_SET_BIT(digital_signals, RANG_PO1_U_VSPOM_RPN) != 0))
    max_number_events++;

  if (max_number_events == 0)
    max_number_events = 1; /*У штатній роботі просто відобразимо режим роботи або струм навантаження у автоматичному режимі*/

  if (current_ekran.index_position < 0)
    current_ekran.index_position = 0 /*max_number_events - 1*/;
  else if (current_ekran.index_position >= max_number_events)
    current_ekran.index_position = max_number_events - 1 /*0*/;
  /*******************************/

  unsigned int position_temp = current_ekran.index_position;
  unsigned int const min_index = (position_temp / MAX_ROW_LCD) * MAX_ROW_LCD;
  unsigned int const max_index = min_index + (MAX_ROW_LCD - 1);
  unsigned int index_of_ekran = 0;

  if (min_index != 0)
    working_ekran[index_of_ekran][MAX_COL_LCD - 2] = MY_ASCII_ARROW_UP;
  if ((unsigned int) (max_number_events - 1) > max_index)
    working_ekran[index_of_ekran][MAX_COL_LCD - 1] = MY_ASCII_ARROW_DOWN;

  /*****
  Перший рядок
  *****/

  convert_and_insert_char_for_measurement(3, base_faze_voltage, TVoltage_base, 1, working_ekran[index_of_ekran], 0);
  working_ekran[index_of_ekran][8] = odynyci_vymirjuvannja[index_language][INDEX_V];

  working_ekran[index_of_ekran][10] = '/';

  if ((current_step < 0) || (current_step > 99))
  {
    working_ekran[index_of_ekran][11] = '?';
  }
  else
  {
    unsigned int value = current_step;
    unsigned int vaga = 10, first_symbol = 0, shift = 0;
    for (unsigned int i = 11; i <= 12; i++)
    {
      calc_int_symbol_and_put_into_working_ekran((working_ekran[index_of_ekran] + i - shift), &value, &vaga, &first_symbol, true);
      if (working_ekran[index_of_ekran][11] == ' ')
        shift++;
    }
  }
  index_of_ekran++;
  /*****/

  /*****
  Наступні рядки
  *****/
  unsigned int current_event = 0;

  /*****
  Процес регулювання або режим роботи
  *****/
  if (
    (_CHECK_SET_BIT(digital_signals, RANG_PEREKLYUCHENNYA_RPN) != 0) ||
    (_CHECK_SET_BIT(digital_signals, RANG_UBAVYTY_RPN) != 0) ||
    (_CHECK_SET_BIT(digital_signals, RANG_PRYBAVYTY_RPN) != 0) ||
    (timer_rpn_ub_pryb_tmp >= 0) ||
    (timer_rpn_ub_pryb_prysk_tmp >= 0) ||
    (timer_rpn_prysk_vid_Umax_tmp >= 0))
  {
    //Іде порцес регулювання
    if ((current_event >= min_index) && (current_event <= max_index))
    {
      /****
      Рядки, які повідомляють про прцес регулювання зажди відображаються першим
      ****/

      //Ситуацій, які розглядаються, як нештатні не зафіксовано - відображаємо інформацію про штатну роботу
      if (_CHECK_SET_BIT(digital_signals, RANG_PEREKLYUCHENNYA_RPN) != 0)
      {
        //Активний сигнал "Перемикання"

        const unsigned char information[MAX_NAMBER_LANGUAGE][MAX_COL_LCD] =
          {
            " Переключение   ",
            " Перемикання    ",
            "  Switching     ",
            " Переключение   "};
        const unsigned int index_up_down[MAX_NAMBER_LANGUAGE] = {14, 13, 12, 14};

        for (unsigned int i = 0; i < MAX_COL_LCD; i++)
          working_ekran[index_of_ekran][i] = information[index_language][i];

        unsigned int index_up_down_tmp = index_up_down[index_language];
        if (znyzyty_bulo_pered_perecluchennjam)
          working_ekran[index_of_ekran][index_up_down_tmp++] = MY_ASCII_ARROW_DOWN;
        if (pidvyshchyty_bulo_pered_perecluchennjam)
          working_ekran[index_of_ekran][index_up_down_tmp++] = MY_ASCII_ARROW_UP;

        index_of_ekran++;
      }
      else if (_CHECK_SET_BIT(digital_signals, RANG_UBAVYTY_RPN) != 0)
      {
        //Активний сигнал "Убавити"

        const unsigned char information[MAX_NAMBER_LANGUAGE][MAX_COL_LCD] =
          {
            "    Убавить     ",
            "    Знизити     ",
            "     Lower      ",
            "    Убавить     "};
        for (unsigned int i = 0; i < MAX_COL_LCD; i++)
          working_ekran[index_of_ekran][i] = information[index_language][i];
        index_of_ekran++;
      }
      else if (_CHECK_SET_BIT(digital_signals, RANG_PRYBAVYTY_RPN) != 0)
      {
        //Активний сигнал "Підвищити"

        const unsigned char information[MAX_NAMBER_LANGUAGE][MAX_COL_LCD] =
          {
            "   Прибавить    ",
            "   Підвищити    ",
            "     Raise      ",
            "   Прибавить    "};
        for (unsigned int i = 0; i < MAX_COL_LCD; i++)
          working_ekran[index_of_ekran][i] = information[index_language][i];
        index_of_ekran++;
      }
      else if (
        (timer_rpn_ub_pryb_tmp >= 0) ||
        (timer_rpn_ub_pryb_prysk_tmp >= 0) ||
        (timer_rpn_prysk_vid_Umax_tmp >= 0))
      {
        unsigned int mode;
        int delta;

        if (timer_rpn_prysk_vid_Umax_tmp >= 0)
        {
          mode = 3;
          delta = current_settings.timeout_rpn_prysk_vid_Umax[number_group_stp_tmp] - timer_rpn_prysk_vid_Umax_tmp;
        }
        else if (timer_rpn_ub_pryb_prysk_tmp >= 0)
        {
          mode = 2;
          delta = current_settings.timeout_rpn_ub_pryb_prysk[number_group_stp_tmp] - timer_rpn_ub_pryb_prysk_tmp;
        }
        else
        {
          mode = 1;
          delta = current_settings.timeout_rpn_ub_pryb[number_group_stp_tmp] - timer_rpn_ub_pryb_tmp;
        }
        //Нс цікавить тільки ціла частина секунд через який має видатися команда "Знизити"/"Підвищити"
        int delta_tmp = delta / 1000;
        if (delta_tmp * 1000 != delta)
          delta = delta_tmp + 1;
        else
          delta = delta_tmp;

        unsigned int first_position = 0 ;
        if (_CHECK_SET_BIT(digital_signals, RANG_UBAVYTY_1_RPN) != 0)
        {
          //Іде підготовка до видачі команди "Знизити"
          if (mode == 1)
          {
#define INDEX_FIRST_DIGIT 6
            //Витримка без присконення

            const unsigned char information[MAX_NAMBER_LANGUAGE][INDEX_FIRST_DIGIT] =
              {
                "Убав.-",
                "Зниз.-",
                "Lower-",
                "Убав.-"};
            first_position = 1;
            for (unsigned int i = 0; i < INDEX_FIRST_DIGIT; i++)
              working_ekran[index_of_ekran][first_position + i] = information[index_language][i];
            first_position += INDEX_FIRST_DIGIT;
#undef INDEX_FIRST_DIGIT
          }
          else if (mode == 2)
          {
#define INDEX_FIRST_DIGIT 10
            //Витримка без присконення

            const unsigned char information[MAX_NAMBER_LANGUAGE][INDEX_FIRST_DIGIT] =
              {
                "Убав.уск.-",
                "Зниз.пр.- ",
                "Acc.Lower-",
                "Убав.уск.-"};
            first_position = 0;
            for (unsigned int i = 0; i < INDEX_FIRST_DIGIT; i++)
              working_ekran[index_of_ekran][first_position + i] = information[index_language][i];
            if (index_language == INDEX_LANGUAGE_UA)
              first_position += (INDEX_FIRST_DIGIT - 1);
            else
              first_position += INDEX_FIRST_DIGIT;
#undef INDEX_FIRST_DIGIT
          }
          else if (mode == 3)
          {
#define INDEX_FIRST_DIGIT 10
            //Витримка без присконення

            const unsigned char information[MAX_NAMBER_LANGUAGE][INDEX_FIRST_DIGIT] =
              {
                "Убав.(U>)-",
                "Зниз.(U>)-",
                "Lower(U>)-",
                "Убав.(U>)-"};
            first_position = 0;
            for (unsigned int i = 0; i < INDEX_FIRST_DIGIT; i++)
              working_ekran[index_of_ekran][first_position + i] = information[index_language][i];
            first_position += INDEX_FIRST_DIGIT;
#undef INDEX_FIRST_DIGIT
          }
        }
        else
        {
          //Іде підготовка до видачі команди "Підвищити"
          if (mode == 1)
          {
#define INDEX_FIRST_DIGIT 6
            //Витримка без присконення

            const unsigned char information[MAX_NAMBER_LANGUAGE][INDEX_FIRST_DIGIT] =
              {
                "Приб.-",
                "Підв.-",
                "Raise-",
                "Приб.-"};
            first_position = 1;
            for (unsigned int i = 0; i < INDEX_FIRST_DIGIT; i++)
              working_ekran[index_of_ekran][first_position + i] = information[index_language][i];
            first_position += INDEX_FIRST_DIGIT;
#undef INDEX_FIRST_DIGIT
          }
          else if (mode == 2)
          {
#define INDEX_FIRST_DIGIT 10
            //Витримка без присконення

            const unsigned char information[MAX_NAMBER_LANGUAGE][INDEX_FIRST_DIGIT] =
              {
                "Приб.уск.-",
                "Підв.пр.- ",
                "Acc.Raise-",
                "Приб.уск.-"};
            first_position = 0;
            for (unsigned int i = 0; i < INDEX_FIRST_DIGIT; i++)
              working_ekran[index_of_ekran][first_position + i] = information[index_language][i];
            if (index_language == INDEX_LANGUAGE_UA)
              first_position += (INDEX_FIRST_DIGIT - 1);
            else
              first_position += INDEX_FIRST_DIGIT;
#undef INDEX_FIRST_DIGIT
          }
        }

        first_position++;
        unsigned int vaga = 100, first_symbol = 0, shift = 0;
        for (unsigned int i = first_position; i < (first_position + 3); i++)
        {
          calc_int_symbol_and_put_into_working_ekran((working_ekran[index_of_ekran] + i - shift), (unsigned int *) (&delta), &vaga, &first_symbol, true);
          if (working_ekran[index_of_ekran][first_position] == ' ')
            shift++;
        }

        working_ekran[index_of_ekran][(first_position + 4) - shift] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];

        index_of_ekran++;
      }
    }
    current_event++;
  }
  /****/

  /****
  Рядки, які повідомляють про нештатну роботу
  ****/
  if (_CHECK_SET_BIT(digital_signals, RANG_AVAR_DEFECT) != 0)
  {
    if ((current_event >= min_index) && (current_event <= max_index))
    {
      //Аварійна несправність
      const unsigned char information[MAX_NAMBER_LANGUAGE][MAX_COL_LCD] =
        {
          "  Неиспр.АРНТ   ",
          "   Неспр.АРНТ   ",
          " Fault of ATVR  ",
          "  Неиспр.АРНТ   "};
      for (unsigned int i = 0; i < MAX_COL_LCD; i++)
        working_ekran[index_of_ekran][i] = information[index_language][i];

      index_of_ekran++;
    }
    current_event++;
  }
  if (_CHECK_SET_BIT(digital_signals, RANG_ZNKh) != 0)
  {
    if ((current_event >= min_index) && (current_event <= max_index))
    {
      //ЗНХ
      const unsigned char information[MAX_NAMBER_LANGUAGE][MAX_COL_LCD] =
        {
          " Непрерывный ход",
          "Безперервний хід",
          "  Cont.Motion   ",
          " Непрерывный ход"};
      for (unsigned int i = 0; i < MAX_COL_LCD; i++)
        working_ekran[index_of_ekran][i] = information[index_language][i];

      index_of_ekran++;
    }
    current_event++;
  }
  if (_CHECK_SET_BIT(digital_signals, RANG_ZSKh) != 0)
  {
    if ((current_event >= min_index) && (current_event <= max_index))
    {
      //ЗНХ
      const unsigned char information[MAX_NAMBER_LANGUAGE][MAX_COL_LCD] =
        {
          "  Самоход РПН   ",
          "  Самохід РПН   ",
          " OLTC Sp.Motion ",
          "  Самоход РПН   "};
      for (unsigned int i = 0; i < MAX_COL_LCD; i++)
        working_ekran[index_of_ekran][i] = information[index_language][i];

      index_of_ekran++;
    }
    current_event++;
  }
  if (_CHECK_SET_BIT(digital_signals, RANG_ZASTRJAVANNJA_RPN) != 0)
  {
    if ((current_event >= min_index) && (current_event <= max_index))
    {
      //Застрявання РПН
      const unsigned char information[MAX_NAMBER_LANGUAGE][MAX_COL_LCD] =
        {
          " Застревание РПН",
          " Застрявання РПН",
          "Sticking of OLTC",
          " Застревание РПН"};
      for (unsigned int i = 0; i < MAX_COL_LCD; i++)
        working_ekran[index_of_ekran][i] = information[index_language][i];

      index_of_ekran++;
    }
    current_event++;
  }
  if (_CHECK_SET_BIT(digital_signals, RANG_PRYVID_NE_PISHOV_RPN) != 0)
  {
    if ((current_event >= min_index) && (current_event <= max_index))
    {
      //Привід не пішов
      const unsigned char information[MAX_NAMBER_LANGUAGE][MAX_COL_LCD] =
        {
          " Привод не пошел",
          " Привід не пішов",
          "Act.hasn't Moved",
          " Привод не пошел"};
      for (unsigned int i = 0; i < MAX_COL_LCD; i++)
        working_ekran[index_of_ekran][i] = information[index_language][i];

      index_of_ekran++;
    }
    current_event++;
  }
  if (_CHECK_SET_BIT(digital_signals, RANG_N_PER_RIVNE_N_PER_USTAVKY_RPN) != 0)
  {
    if ((current_event >= min_index) && (current_event <= max_index))
    {
      //Перевищення максимальної кількості переключень у одному напрямку
      const unsigned char information[MAX_NAMBER_LANGUAGE][MAX_COL_LCD] =
        {
          " xx переключение",
          " xx перемикання ",
          "  xx Switching  ",
          " xx переключение"};
      const unsigned int first_index_of_number_position_array[MAX_NAMBER_LANGUAGE] = {1, 1, 2, 1};
      unsigned int first_index_of_number_position = first_index_of_number_position_array[index_language];

      for (unsigned int i = 0; i < MAX_COL_LCD; i++)
      {
        if (
          (i < first_index_of_number_position) ||
          (i > (first_index_of_number_position + 1)))
          working_ekran[index_of_ekran][i] = information[index_language][i];
        else
        {
          if (i == first_index_of_number_position)
          {
            if (current_settings.number_steps_rpn < 10)
              working_ekran[index_of_ekran][i] = ' ';
            else
              working_ekran[index_of_ekran][i] = (current_settings_prt.setpoint_rpn_per[number_group_stp_tmp] / 10) + 0x30;
          }
          else
          {
            working_ekran[index_of_ekran][i] = (current_settings_prt.setpoint_rpn_per[number_group_stp_tmp] % 10) + 0x30;
          }
        }
      }
      index_of_ekran++;
    }
    current_event++;
  }
  if (_CHECK_SET_BIT(digital_signals, RANG_1_POLOGENNJA_RPN) != 0)
  {
    if ((current_event >= min_index) && (current_event <= max_index))
    {
      //"Заборона Знизити"
      const unsigned char information[MAX_NAMBER_LANGUAGE][MAX_COL_LCD] =
        {
          " Запрет Убавить ",
          "  Заб.Знизити   ",
          " Disabling Lower",
          " Запрет Убавить "};
      for (unsigned int i = 0; i < MAX_COL_LCD; i++)
        working_ekran[index_of_ekran][i] = information[index_language][i];

      index_of_ekran++;
    }
    current_event++;
  }
  if (_CHECK_SET_BIT(digital_signals, RANG_N_POLOGENNJA_RPN) != 0)
  {
    if ((current_event >= min_index) && (current_event <= max_index))
    {
      //"Заборона Підвищити"
      const unsigned char information[MAX_NAMBER_LANGUAGE][MAX_COL_LCD] =
        {
          "Запрет Прибавить",
          " Заб.Підвищити  ",
          " Disabling Raise",
          "Запрет Прибавить"};
      for (unsigned int i = 0; i < MAX_COL_LCD; i++)
        working_ekran[index_of_ekran][i] = information[index_language][i];

      index_of_ekran++;
    }
    current_event++;
  }
  if (_CHECK_SET_BIT(digital_signals, RANG_MRZS_OR_LOCAL_MODE_RPN) == 0)
  {
    if ((current_event >= min_index) && (current_event <= max_index))
    {
      //Активний сигнал "Місцевий режим"

      const unsigned char information[MAX_NAMBER_LANGUAGE][MAX_COL_LCD] =
        {
          " Режим Местный  ",
          " Режим Місцевий ",
          "   Local Mode   ",
          " Режим Местный  "};
      for (unsigned int i = 0; i < MAX_COL_LCD; i++)
        working_ekran[index_of_ekran][i] = information[index_language][i];

      index_of_ekran++;
    }
    current_event++;
  }
  if (_CHECK_SET_BIT(digital_signals, RANG_REMOTE_MODE_RPN) != 0)
  {
    if ((current_event >= min_index) && (current_event <= max_index))
    {
      //Активний сигнал "Місцевий режим"

      const unsigned char information[MAX_NAMBER_LANGUAGE][MAX_COL_LCD] =
        {
          "  Режим Дист.   ",
          "  Режим Дист.   ",
          "  Remote Mode   ",
          "  Режим Дист.   "};
      for (unsigned int i = 0; i < MAX_COL_LCD; i++)
        working_ekran[index_of_ekran][i] = information[index_language][i];
      index_of_ekran++;
    }
    current_event++;
  }
  if (_CHECK_SET_BIT(digital_signals, RANG_UMIN1) != 0)
  {
    if ((current_event >= min_index) && (current_event <= max_index))
    {
      //ЗНмін1
      const unsigned char information[MAX_NAMBER_LANGUAGE][MAX_COL_LCD] =
        {
          " Защита ЗНмин1  ",
          " Захист ЗНмін1  ",
          " Protection UVP1",
          " Защита ЗНмин1  "};
      for (unsigned int i = 0; i < MAX_COL_LCD; i++)
        working_ekran[index_of_ekran][i] = information[index_language][i];

      index_of_ekran++;
    }
    current_event++;
  }
  if (_CHECK_SET_BIT(digital_signals, RANG_UMIN2) != 0)
  {
    if ((current_event >= min_index) && (current_event <= max_index))
    {
      //ЗНмін2
      const unsigned char information[MAX_NAMBER_LANGUAGE][MAX_COL_LCD] =
        {
          " Защита ЗНмин2  ",
          " Захист ЗНмін2  ",
          " Protection UVP2",
          " Защита ЗНмин2  "};
      for (unsigned int i = 0; i < MAX_COL_LCD; i++)
        working_ekran[index_of_ekran][i] = information[index_language][i];

      index_of_ekran++;
    }
    current_event++;
  }
  if (_CHECK_SET_BIT(digital_signals, RANG_UMAX2) != 0)
  {
    if ((current_event >= min_index) && (current_event <= max_index))
    {
      //ЗНмакс2
      const unsigned char information[MAX_NAMBER_LANGUAGE][MAX_COL_LCD] =
        {
          " Защита ЗНмакс2 ",
          " Захист ЗНмакс2 ",
          " Protection OVP2",
          " Защита ЗНмакс2 "};
      for (unsigned int i = 0; i < MAX_COL_LCD; i++)
        working_ekran[index_of_ekran][i] = information[index_language][i];

      index_of_ekran++;
    }
    current_event++;
  }
  if (_CHECK_SET_BIT(digital_signals, RANG_PO4_U_OSN_RPN) != 0)
  {
    if ((current_event >= min_index) && (current_event <= max_index))
    {
      //Напруга основного каналу менше 0,8Uн
      const unsigned char information[MAX_NAMBER_LANGUAGE][MAX_COL_LCD] =
        {
          " Блок. U<0.8Uн  ",
          " Блок. U<0.8Uн  ",
          " Block.U<0.8Ur  ",
          " Блок. U<0.8Uн  "};
      for (unsigned int i = 0; i < MAX_COL_LCD; i++)
        working_ekran[index_of_ekran][i] = information[index_language][i];

      index_of_ekran++;
    }
    current_event++;
  }
  if (
    ((current_settings.control_rpn & MASKA_FOR_BIT(INDEX_ML_CTRRPN_DOD_KONTUR)) != 0) &&
    (_CHECK_SET_BIT(digital_signals, RANG_PO1_U_VSPOM_RPN) != 0))
  {
    if ((current_event >= min_index) && (current_event <= max_index))
    {
      //Напруга по допоміжному каналу перевишує свою уставку (при активному контролю напруги допоміжного каналу)
      const unsigned char information[MAX_NAMBER_LANGUAGE][MAX_COL_LCD] =
        {
          "ЗНмакс по всп.к.",
          "ЗНмакс за дод.к.",
          "OVP from Aux.Ch.",
          "ЗНмакс по всп.к."};
      for (unsigned int i = 0; i < MAX_COL_LCD; i++)
        working_ekran[index_of_ekran][i] = information[index_language][i];

      index_of_ekran++;
    }
    current_event++;
  }
  /****/

  if (current_event == 0)
  {
    /*
    Це є випадок, коли не йде регулювання і немає повідомлень про помилки - 
    в цьому випадку простовідображаємо струм навантаження, що означає що  "ВСЕ В ПОРЯДКУ"
    */
    if ((current_event >= min_index) && (current_event <= max_index))
    {
      /*
      Ця умова мала б тут завжди справджуватися, бо одну подію ми завжди передвачаємо - це
      або процес регулювання, або коли ні процесу регулювання немає, ні помилок немає - то
      тоді відображаємо просто струм навантаження
      */

      //Регулювання відсутнє - напруга у зоні нечутливості
      unsigned int first_position;
      if (
        (_CHECK_SET_BIT(digital_signals, RANG_BLOCK_STRUM_KOMP_RPN) == 0) &&
        ((current_settings.control_rpn & MASKA_FOR_BIT(INDEX_ML_CTRRPN_STRUMOVA_COMP)) != 0))
      {
#define INDEX_FIRST_DIGIT 7
        //"Працює" струмова компенсація

        const unsigned char information[MAX_NAMBER_LANGUAGE][INDEX_FIRST_DIGIT] =
          {
            "Iкомп.=",
            "Iкомп.=",
            "Icomp.=",
            "Iкомп.="};
        first_position = 0;
        for (unsigned int i = 0; i < INDEX_FIRST_DIGIT; i++)
          working_ekran[index_of_ekran][first_position + i] = information[index_language][i];
        first_position += INDEX_FIRST_DIGIT;
#undef INDEX_FIRST_DIGIT
      }
      else
      {
#define INDEX_FIRST_DIGIT 3
        //"Не працює" струмова компенсація

        const unsigned char information[MAX_NAMBER_LANGUAGE][INDEX_FIRST_DIGIT] =
          {
            "Iн=",
            "Iн=",
            "Iн=",
            "Iн="};
        first_position = 2;
        for (unsigned int i = 0; i < INDEX_FIRST_DIGIT; i++)
          working_ekran[index_of_ekran][first_position + i] = information[index_language][i];
        first_position += INDEX_FIRST_DIGIT;
#undef INDEX_FIRST_DIGIT
      }
      convert_and_insert_char_for_measurement(3, base_faze_current, TCurrent_base, 1, working_ekran[index_of_ekran], first_position);
      working_ekran[index_of_ekran][first_position + 8] = odynyci_vymirjuvannja[index_language][INDEX_A];
      index_of_ekran++;
    }
    current_event++;
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
//
/*****************************************************/
/*****************************************************/
