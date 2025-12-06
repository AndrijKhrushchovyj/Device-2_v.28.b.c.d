#include "header.h"

/*****************************************************/
//Формуємо екран відображення повідомдень про відключення від захистів
/*****************************************************/
void make_ekran_vidkluchenja(void)
{
  int const index_language = index_language_in_array(current_settings.language);

  static unsigned char const template_string[2][MAX_COL_LCD] = 
  {
    "   XX-XX-20XX   ",
    "  XX:XX:XX.XXX  "
  };
  
  for (size_t i = 0; i < 2; ++i)
  {
    unsigned char const * ptr_source = template_string[i];
    unsigned char       * ptr_target = working_ekran[i];
    for (unsigned int j = 0; j < MAX_COL_LCD; j++) *ptr_target++ = *ptr_source++;
  }
 
  //очищаємо решту екрану
  for (unsigned int i = 2; i < MAX_ROW_LCD; i++)
  {
    unsigned char * ptr_target = working_ekran[i];
    for (unsigned int j = 0; j < MAX_COL_LCD; j++) *ptr_target++ = ' ';
  }
  
  int32_t index_position_tmp = current_ekran.index_position;
  __info_vymk info_vymk = info_vidkluchennja_vymykachatime[index_position_tmp];

  time_t time_dat_tmp = info_vymk.time_dat;
  if (time_dat_tmp != 0)
  {
    struct tm *p = localtime(&info_vymk.time_dat);
    int field;

    //День
    field = p->tm_mday;
    working_ekran[VIDKL_ROW_Y_][VIDKL_COL_DY1] = (field / 10) + 0x30;
    working_ekran[VIDKL_ROW_Y_][VIDKL_COL_DY2] = (field % 10) + 0x30;

    //Місяць
    field = p->tm_mon + 1;
    working_ekran[VIDKL_ROW_Y_][VIDKL_COL_MY1] = (field / 10) + 0x30;
    working_ekran[VIDKL_ROW_Y_][VIDKL_COL_MY2] = (field % 10) + 0x30;

    //Рік
    field = p->tm_year - 100;
    working_ekran[VIDKL_ROW_Y_][VIDKL_COL_SY1] = (field / 10) + 0x30;
    working_ekran[VIDKL_ROW_Y_][VIDKL_COL_SY2] = (field % 10) + 0x30;

    //Година
    field = p->tm_hour;
    working_ekran[VIDKL_ROW_T_][VIDKL_COL_HT1] = (field / 10) + 0x30;
    working_ekran[VIDKL_ROW_T_][VIDKL_COL_HT2] = (field % 10) + 0x30;

    //Хвилини
    field = p->tm_min;
    working_ekran[VIDKL_ROW_T_][VIDKL_COL_MT1] = (field / 10) + 0x30;
    working_ekran[VIDKL_ROW_T_][VIDKL_COL_MT2] = (field % 10) + 0x30;

    //Секунди
    field = p->tm_sec;
    working_ekran[VIDKL_ROW_T_][VIDKL_COL_ST1] = (field / 10) + 0x30;
    working_ekran[VIDKL_ROW_T_][VIDKL_COL_ST2] = (field % 10) + 0x30;

    //Тисячні секунд
    field = info_vymk.time_ms;
    working_ekran[VIDKL_ROW_T_][VIDKL_COL_HST1] = (field / 100) + 0x30;
    field %= 100;
      
    working_ekran[VIDKL_ROW_T_][VIDKL_COL_HST2] = (field / 10) + 0x30;
    field %= 10;

    working_ekran[VIDKL_ROW_T_][VIDKL_COL_HST3] = field + 0x30;
  }

  static unsigned char const information[MAX_NAMBER_LANGUAGE][VYMKNENNJA_VID_MAX_NUMBER - NUMBER_UP + 1][MAX_COL_LCD] = 
  {
    {
      "     МТЗ 1      ",
      "     МТЗ 2      ",
      "     МТЗ 3      ",
      "     МТЗ 4      ",
      "  МТЗ 0.4кВ 1   ",
      "  МТЗ 0.4кВ 2   ",
      "      ЗДЗ       ",
      "    ЗЗ(3I0)     ",
      "    ЗЗ(3U0)     ",
      "      НЗЗ       ",
      "     ТЗНП 1     ",
      "     ТЗНП 2     ",
      "     ТЗНП 3     ",
      " АЧР/ЧАПВ от ДВ ",
      "   АЧР/ЧАПВ 1   ",
      "   АЧР/ЧАПВ 2   ",
      "     УРОВ 1     ",
      "     УРОВ 2     ",
      "     ЗОП(I)     ",
      "     Umin 1     ",
      "     Umin 2     ",
      "     Umax 1     ",
      "     Umax 2     ",
      "      УЗ x      ",
      "   Вн.Защита    ",
      "     Другие     "
    },
    {
      "     МСЗ 1      ",
      "     МСЗ 2      ",
      "     МСЗ 3      ",
      "     МСЗ 4      ",
      "  МСЗ 0.4кВ 1   ",
      "  МСЗ 0.4кВ 2   ",
      "      ЗДЗ       ",
      "    ЗЗ(3I0)     ",
      "    ЗЗ(3U0)     ",
      "      СЗЗ       ",
      "     СЗНП 1     ",
      "     СЗНП 2     ",
      "     СЗНП 3     ",
      " АЧР/ЧАПВ від ДВ",
      "   АЧР/ЧАПВ 1   ",
      "   АЧР/ЧАПВ 2   ",
      "     ПРВВ 1     ",
      "     ПРВВ 2     ",
      "     ЗЗП(I)     ",
      "     Umin 1     ",
      "     Umin 2     ",
      "     Umax 1     ",
      "     Umax 2     ",
      "      УЗ x      ",
      "    З.Захист    ",
      "      Інші      "
    },
    {
      "     OCP 1      ",
      "     OCP 2      ",
      "     OCP 3      ",
      "     OCP 4      ",
      "    LVOCP 1     ",
      "    LVOCP 2     ",
      "      AFD       ",
      "     SEFOCP     ",
      "     EFOVP      ",
      "     SDEFP      ",
      "     DEFP 1     ",
      "     DEFP 2     ",
      "     DEFP 3     ",
      " LFС/FAR v BI St",
      "   LFС/FAR 1    ",
      "   LFС/FAR 2    ",
      "   CBF In Op    ",
      "   CBF Ex Op    ",
      "     NSOCP      ",
      "     UVP 1      ",
      "     UVP 2      ",
      "     OVP 1      ",
      "     OVP 2      ",
      "     MFP x      ",
      "    Ext Prt     ",
      "     Others     "
    },
    {
      "     МТЗ 1      ",
      "     МТЗ 2      ",
      "     МТЗ 3      ",
      "     МТЗ 4      ",
      "  МТЗ 0.4кВ 1   ",
      "  МТЗ 0.4кВ 2   ",
      "      ЗДЗ       ",
      "    ЗЗ(3I0)     ",
      "    ЗЗ(3U0)     ",
      "      НЗЗ       ",
      "     ТЗНП 1     ",
      "     ТЗНП 2     ",
      "     ТЗНП 3     ",
      " АЧР/ЧАПВ от ДВ ",
      "   АЧР/ЧАПВ 1   ",
      "   АЧР/ЧАПВ 2   ",
      "     УРОВ 1     ",
      "     УРОВ 2     ",
      "     ЗОП(I)     ",
      "     Umin 1     ",
      "     Umin 2     ",
      "     Umax 1     ",
      "     Umax 2     ",
      "      УЗ x      ",
      "   Вн.Защита    ",
      "     Другие     "
    }
  };
  
  unsigned char const *point;
  if ((index_position_tmp < VYMKNENNJA_VID_UP1) || (index_position_tmp >= (VYMKNENNJA_VID_UP1 + NUMBER_UP)))
  {
    if (index_position_tmp < VYMKNENNJA_VID_UP1)
      point = (unsigned char *)information[index_language][index_position_tmp];
    else
      point = (unsigned char *)information[index_language][index_position_tmp - NUMBER_UP + 1];

    for (size_t i = 0; i < MAX_COL_LCD; ++i) working_ekran[VIDKL_ROW_INFO_][i] = *point++;
  }
  else
  {
    point = (unsigned char *)information[index_language][VYMKNENNJA_VID_UP1];

    for (size_t i = 0; i < MAX_COL_LCD; ++i) 
    {
      working_ekran[VIDKL_ROW_INFO_][i] = (i != index_number_UP[index_language][2]) ? *(point + i) : (index_position_tmp - VYMKNENNJA_VID_UP1 + 1 + 0x30);
    }
  }
    
  //Курсор по горизонталі відображається на першій позиції
  current_ekran.position_cursor_x = 0;
  current_ekran.position_cursor_y = 0;
  //Курсор невидимий
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
