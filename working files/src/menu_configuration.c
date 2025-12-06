#include "header.h"

/*****************************************************/
//Формуємо екран конфігурації
/*****************************************************/
void make_ekran_configuration(unsigned int configuration_edit_temp)
{
#define MAX_COL_LCD_PART1 10

  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_EKRAN_CONFIGURATION][MAX_COL_LCD_PART1] = 
  {
    {
     "МТЗ       ",
     "МТЗ 0.4кВ ",
     "ЗНам      ",
     "ЗДЗ       ",
     "          ",
     "ТЗНП      ",
     "АПВ       ",
     "АЧР/ЧАПВ  ",
     "УРОВ      ",
     "ЗОП(I)    ",
     "Umin      ",
     "Umax      ",
     "УЗ        ",
     "ОМП       ",
     "Расш.лог. "
#ifdef NUMBER_DS
                  ,
     "ДШ        "
#endif
    },
    {
     "МСЗ       ",
     "МСЗ 0.4кВ ",
     "ЗНам      ",
     "ЗДЗ       ",
     "          ",
     "СЗНП      ",
     "АПВ       ",
     "АЧР/ЧАПВ  ",
     "ПРВВ      ",
     "ЗЗП(I)    ",
     "Umin      ",
     "Umax      ",
     "УЗ        ",
     "ВМП       ",
     "Розш.лог. "
#ifdef NUMBER_DS
                 ,
     "ДШ        "
#endif
    },
    {
     "OCP       ",
     "LVOCP     ",
     "InrP      ",
     "AFD       ",
     "          ",
     "DEFP      ",
     "AR        ",
     "LFC/FAR   ",
     "CBF       ",
     "NSOCP     ",
     "UVP       ",
     "OVP       ",
     "MFP       ",
     "FL        ",
     "UD-logic  "
#ifdef NUMBER_DS
                 ,
     "DSh       "
#endif
    },
    {
     "МТK       ",
     "МТK 0.4кВ ",
     "ЗНам      ",
     "ЗДЗ       ",
     "          ",
     "ТЗНП      ",
     "АKK       ",
     "АЖЖ/ТАKK  ",
     "СІШРK     ",
     "КKK       ",
     "Umin      ",
     "Umax      ",
     "УЗ        ",
     "ОМП       ",
     "Расш.лог. "
#ifdef NUMBER_DS
                 ,
     "ДШ        "
#endif
    }
  };
  static const unsigned char name_nzz[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD_PART1] = 
  {
    {"НЗЗ       ", "ЗЗ(3I0)   "},
    {"СЗЗ       ", "ЗЗ(3I0)   "},
    {"SDEFP     ", "SEFOCP    "},
    {"НЗЗ       ", "ЗЗ(3I0)   "}
  };

  static const unsigned char information_on[MAX_NAMBER_LANGUAGE][MAX_COL_LCD - MAX_COL_LCD_PART1] = 
  {
    "Вкл.  ",
    "Увімк.",
    "On    ",
    "Косу. "
  };
  static const unsigned char information_off[MAX_NAMBER_LANGUAGE][MAX_COL_LCD - MAX_COL_LCD_PART1] = 
  {
    "Откл. ",
    "Вимк. ",
    "Off   ",
    "Сљнд. "
  };
  
  unsigned char name_string_tmp[MAX_ROW_FOR_EKRAN_CONFIGURATION][MAX_COL_LCD];

  int index_language = index_language_in_array(current_settings.language);
  for(int index_1 = 0; index_1 < MAX_ROW_FOR_EKRAN_CONFIGURATION; index_1++)
  {
    unsigned char *point_target;
    unsigned int nzz_zz = ((current_settings.control_zz & CTR_ZZ1_TYPE) != 0);
    if (index_1 == ZZ_BIT_CONFIGURATION) point_target = (unsigned char *)name_nzz[index_language][nzz_zz];
    else point_target = (unsigned char *)name_string[index_language][index_1];
      
    for(int index_2 = 0; index_2 < MAX_COL_LCD_PART1; index_2++)
      name_string_tmp[index_1][index_2] = *(point_target + index_2);
  }
  
  /*******************************************************/
  //Формуємо значення
  /*******************************************************/
  for(unsigned int i = 0; i< MAX_ROW_FOR_EKRAN_CONFIGURATION; i++)
  {
    unsigned int configuration_temp;
    
    //Якщо ми не в режимі редагування, то беремо дані із стуктури текучих настройок
    //інакше берем із структури для редагування
    if(current_ekran.edition == 0) configuration_temp = current_settings.configuration;
    else configuration_temp = configuration_edit_temp;
    
    if ((configuration_temp & (1<<i)) == 0)
    {
      for (unsigned int j = MAX_COL_LCD_PART1; j < MAX_COL_LCD; j++)
        name_string_tmp[i][j] = information_off[index_language][j - MAX_COL_LCD_PART1];
    }
    else
    {
      for (unsigned int j = MAX_COL_LCD_PART1; j < MAX_COL_LCD; j++)
        name_string_tmp[i][j] = information_on[index_language][j - MAX_COL_LCD_PART1];
    }
  }
  /*******************************************************/
    
  int additional_current = 0;
  int position_temp = current_ekran.index_position;
  int index_of_ekran;
  /******************************************/
  //Виключаємо, які вимірювання не треба відображати
  /******************************************/
  {
    int delete_index;
    if ((current_settings.control_extra_settings_1 & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_IB_I04)) == 0)
      delete_index = MTZ04_BIT_CONFIGURATION;
    else
      delete_index = TZNP_BIT_CONFIGURATION;
    
    int i = delete_index - additional_current;
    if ((i+1) <= position_temp) position_temp--;
    do
    {
      for(unsigned int j = 0; j < MAX_COL_LCD; j++)
      {
        if ((i + 1) < (MAX_ROW_FOR_EKRAN_CONFIGURATION - additional_current)) name_string_tmp[i][j] = name_string_tmp[i + 1][j];
        else name_string_tmp[i][j] = ' ';
      }
      i++;
    }
    while (i< (MAX_ROW_FOR_EKRAN_CONFIGURATION - additional_current));
    additional_current++;
  }
  /******************************************/

  index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;
  
  //Копіюємо  рядки у робочий екран
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    //Наступні рядки треба перевірити, чи їх требе відображати у текучій коффігурації
    if (index_of_ekran < (MAX_ROW_FOR_EKRAN_CONFIGURATION - additional_current))
      for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string_tmp[index_of_ekran][j];
    else
      for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';

    index_of_ekran++;
  }

  //Курсор по горизонталі відображається на першій позиції
  current_ekran.position_cursor_x = (MAX_COL_LCD_PART1 - 1);
  //Відображення курору по вертикалі
  current_ekran.position_cursor_y = position_temp & (MAX_ROW_LCD - 1);
  //Курсор видимий
  current_ekran.cursor_on = 1;
  //Курсор не мигає
  if (current_ekran.edition == 0)   current_ekran.cursor_blinking_on = 0;
  else   current_ekran.cursor_blinking_on = 1;

  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
  
#undef MAX_COL_LCD_PART1  
}
/*****************************************************/

/*****************************************************/
//
/*****************************************************/
/*****************************************************/
