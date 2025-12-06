#include "header.h"

/*****************************************************/
//Вираховуваня символу і поміщення його в робочий екран
/*****************************************************/
void calc_symbol_and_put_into_working_ekran(unsigned char* point_in_working_ekran, void* point_value, void* point_vaga, unsigned int* point_first_symbol, unsigned int current_position_x, unsigned int position_comma, unsigned int view, unsigned int v_32_64)
{
  unsigned int temp_data;
  if (v_32_64 == 0)
  {
    temp_data = (*((unsigned int*)point_value)) / (*((unsigned int*)point_vaga)); //виділяємо число, яке треба перетворити у символ і помістити у дану позицію екрану
    *((unsigned int*)point_value) %= *((unsigned int*)point_vaga); //вираховуємо число без символа, який ми зараз будемо виводити на екран
    *((unsigned int*)point_vaga) /=10; //зменшуємо ваговий коефіцієнт в 10 разів
  }
  else
  {
    temp_data = (*((unsigned long long*)point_value)) / (*((unsigned long long*)point_vaga)); //виділяємо число, яке треба перетворити у символ і помістити у дану позицію екрану
    *((unsigned long long*)point_value) %= *((unsigned long long*)point_vaga); //вираховуємо число без символа, який ми зараз будемо виводити на екран
    *((unsigned long long*)point_vaga) /=10; //зменшуємо ваговий коефіцієнт в 10 разів
  }
  
  if (view == false) *point_in_working_ekran = temp_data + 0x30;
  else
  {
    //У випадку, якщо ми не у режимі редагування, то нулі перед комою (за винятком останнього, якщо такий є) приховуємо
    if ((temp_data !=0) || ((*point_first_symbol) != 0))
    {
      *point_in_working_ekran = temp_data + 0x30;
      if ((*point_first_symbol) == 0) *point_first_symbol = 1;
    }
    else
    {
      if (current_position_x < (position_comma - 1) ) *point_in_working_ekran = ' ';
      else
      {
        *point_in_working_ekran = temp_data + 0x30;
        if ((*point_first_symbol) == 0) *point_first_symbol = 1;
      }
    }
  }
}
/*****************************************************/

/*****************************************************/
//Формуємо екран відображення уставок МТЗ
/*****************************************************/
void make_ekran_setpoint_mtz(unsigned int group)
{
  static const unsigned char name_string_withznam[MAX_NAMBER_LANGUAGE][WITHZNAM_MAX_ROW_FOR_SETPOINT_MTZ][MAX_COL_LCD] = 
  {
    {
      "     МТЗ 1      ",
      "   МТЗН 1 Пр.   ",
      "  МТЗН 1 Обр.   ",
      " Угол дов.МТЗН 1",
      "    МТЗПН 1     ",
      " Напр.п.МТЗПН 1 ",
      "Загр.МТЗ 1(ЗНам)",
      "     МТЗ 2      ",
      "   МТЗН 2 Пр.   ",
      "  МТЗН 2 Обр.   ",
      " Угол дов.МТЗН 2",
      "    МТЗПН 2     ",
      " Напр.п.МТЗПН 2 ",
      "Загр.МТЗ 2(ЗНам)",
      "     МТЗ 3      ",
      "   МТЗН 3 Пр.   ",
      "  МТЗН 3 Обр.   ",
      " Угол дов.МТЗН 3",
      "    МТЗПН 3     ",
      " Напр.п.МТЗПН 3 ",
      "Загр.МТЗ 3(ЗНам)",
      "     МТЗ 4      ",
      "   МТЗН 4 Пр.   ",
      "  МТЗН 4 Обр.   ",
      " Угол дов.МТЗН 4",
      "    МТЗПН 4     ",
      " Напр.п.МТЗПН 4 ",
      "Загр.МТЗ 4(ЗНам)"
    },
    {
      "     МСЗ 1      ",
      "   МСЗС 1 Пр.   ",
      "   МСЗС 1 Зв.   ",
      " Кут пов.МСЗС 1 ",
      "    МСЗПН 1     ",
      " Напр.п.МСЗПН 1 ",
      "Загр.МСЗ 1(ЗНам)",
      "     МСЗ 2      ",
      "   МСЗС 2 Пр.   ",
      "   МСЗС 2 Зв.   ",
      " Кут пов.МСЗС 2 ",
      "    МСЗПН 2     ",
      " Напр.п.МСЗПН 2 ",
      "Загр.МСЗ 2(ЗНам)",
      "     МСЗ 3      ",
      "   МСЗС 3 Пр.   ",
      "   МСЗС 3 Зв.   ",
      " Кут пов.МСЗС 3 ",
      "    МСЗПН 3     ",
      " Напр.п.МСЗПН 3 ",
      "Загр.МСЗ 3(ЗНам)",
      "     МСЗ 4      ",
      "   МСЗС 4 Пр.   ",
      "   МСЗС 4 Зв.   ",
      " Кут пов.МСЗС 4 ",
      "    МСЗПН 4     ",
      " Напр.п.МСЗПН 4 ",
      "Загр.МСЗ 4(ЗНам)",
    },
    {
      "     OCP 1      ",
      "   DOCP 1 Fw    ",
      "   DOCP 1 Bw    ",
      " DOCP 1 rot ang ",
      "     VOCP 1     ",
      " VOCP 1 V Start ",
      "OCP 1 InrP coars",
      "     OCP 2      ",
      "   DOCP 2 Fw    ",
      "   DOCP 2 Bw    ",
      " DOCP 2 rot ang ",
      "     VOCP 2     ",
      " VOCP 2 V Start ",
      "OCP 2 InrP coars",
      "     OCP 3      ",
      "   DOCP 3 Fw    ",
      "   DOCP 3 Bw    ",
      " DOCP 3 rot ang ",
      "     VOCP 3     ",
      " VOCP 3 V Start ",
      "OCP 3 InrP coars",
      "     OCP 4      ",
      "   DOCP 4 Fw    ",
      "   DOCP 4 Bw    ",
      " DOCP 4 rot ang ",
      "     VOCP 4     ",
      " VOCP 4 V Start ",
      "OCP 4 InrP coars"
    },
    {
      "     МТЗ 1      ",
      "   МТЗН 1 Пр.   ",
      "  МТЗН 1 Обр.   ",
      " Угол дов.МТЗН 1",
      "    МТЗПН 1     ",
      " Напр.п.МТЗПН 1 ",
      "Загр.МТЗ 1(ЗНам)",
      "     МТЗ 2      ",
      "   МТЗН 2 Пр.   ",
      "  МТЗН 2 Обр.   ",
      " Угол дов.МТЗН 2",
      "    МТЗПН 2     ",
      " Напр.п.МТЗПН 2 ",
      "Загр.МТЗ 2(ЗНам)",
      "     МТЗ 3      ",
      "   МТЗН 3 Пр.   ",
      "  МТЗН 3 Обр.   ",
      " Угол дов.МТЗН 3",
      "    МТЗПН 3     ",
      " Напр.п.МТЗПН 3 ",
      "Загр.МТЗ 3(ЗНам)",
      "     МТЗ 4      ",
      "   МТЗН 4 Пр.   ",
      "  МТЗН 4 Обр.   ",
      " Угол дов.МТЗН 4",
      "    МТЗПН 4     ",
      " Напр.п.МТЗПН 4 ",
      "Загр.МТЗ 4(ЗНам)"
    }
  };
  static const unsigned char name_string_withoutznam[MAX_NAMBER_LANGUAGE][WITHOUTZNAM_MAX_ROW_FOR_SETPOINT_MTZ][MAX_COL_LCD] = 
  {
    {
      "     МТЗ 1      ",
      "   МТЗН 1 Пр.   ",
      "  МТЗН 1 Обр.   ",
      " Угол дов.МТЗН 1",
      "    МТЗПН 1     ",
      " Напр.п.МТЗПН 1 ",
      "     МТЗ 2      ",
      "   МТЗН 2 Пр.   ",
      "  МТЗН 2 Обр.   ",
      " Угол дов.МТЗН 2",
      "    МТЗПН 2     ",
      " Напр.п.МТЗПН 2 ",
      "     МТЗ 3      ",
      "   МТЗН 3 Пр.   ",
      "  МТЗН 3 Обр.   ",
      " Угол дов.МТЗН 3",
      "    МТЗПН 3     ",
      " Напр.п.МТЗПН 3 ",
      "     МТЗ 4      ",
      "   МТЗН 4 Пр.   ",
      "  МТЗН 4 Обр.   ",
      " Угол дов.МТЗН 4",
      "    МТЗПН 4     ",
      " Напр.п.МТЗПН 4 "
    },
    {
      "     МСЗ 1      ",
      " МСЗС 1 Прямий  ",
      "  МСЗС 1 Звор.  ",
      " Кут пов.МСЗС 1 ",
      "    МСЗПН 1     ",
      " Напр.п.МСЗПН 1 ",
      "     МСЗ 2      ",
      " МСЗС 2 Прямий  ",
      "  МСЗС 2 Звор.  ",
      " Кут пов.МСЗС 2 ",
      "    МСЗПН 2     ",
      " Напр.п.МСЗПН 2 ",
      "     МСЗ 3      ",
      " МСЗС 3 Прямий  ",
      "  МСЗС 3 Звор.  ",
      " Кут пов.МСЗС 3 ",
      "    МСЗПН 3     ",
      " Напр.п.МСЗПН 3 ",
      "     МСЗ 4      ",
      " МСЗС 4 Прямий  ",
      "  МСЗС 4 Звор.  ",
      " Кут пов.МСЗС 4 ",
      "    МСЗПН 4     ",
      " Напр.п.МСЗПН 4 "
    },
    {
      "     OCP 1      ",
      "   DOCP 1 Fw    ",
      "   DOCP 1 Bw    ",
      " DOCP 1 rot ang ",
      "     VOCP 1     ",
      " VOCP 1 V Start ",
      "     OCP 2      ",
      "   DOCP 2 Fw    ",
      "   DOCP 2 Bw    ",
      " DOCP 2 rot ang ",
      "     VOCP 2     ",
      " VOCP 2 V Start ",
      "     OCP 3      ",
      "   DOCP 3 Fw    ",
      "   DOCP 3 Bw    ",
      " DOCP 3 rot ang ",
      "     VOCP 3     ",
      " VOCP 3 V Start ",
      "     OCP 4      ",
      "   DOCP 4 Fw    ",
      "   DOCP 4 Bw    ",
      " DOCP 4 rot ang ",
      "     VOCP 4     ",
      " VOCP 4 V Start "
    },
    {
      "     МТЗ 1      ",
      "   МТЗН 1 Пр.   ",
      "  МТЗН 1 Обр.   ",
      " Угол дов.МТЗН 1",
      "    МТЗПН 1     ",
      " Напр.п.МТЗПН 1 ",
      "     МТЗ 2      ",
      "   МТЗН 2 Пр.   ",
      "  МТЗН 2 Обр.   ",
      " Угол дов.МТЗН 2",
      "    МТЗПН 2     ",
      " Напр.п.МТЗПН 2 ",
      "     МТЗ 3      ",
      "   МТЗН 3 Пр.   ",
      "  МТЗН 3 Обр.   ",
      " Угол дов.МТЗН 3",
      "    МТЗПН 3     ",
      " Напр.п.МТЗПН 3 ",
      "     МТЗ 4      ",
      "   МТЗН 4 Пр.   ",
      "  МТЗН 4 Обр.   ",
      " Угол дов.МТЗН 4",
      "    МТЗПН 4     ",
      " Напр.п.МТЗПН 4 "
    }
  };

  unsigned int isZNam = current_settings.configuration & (1 << ZNAM_BIT_CONFIGURATION);

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
    if (index_of_ekran_tmp < (unsigned int)(isZNam? WITHZNAM_MAX_ROW_FOR_SETPOINT_MTZ : WITHOUTZNAM_MAX_ROW_FOR_SETPOINT_MTZ))
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок

        if(isZNam)  for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string_withznam[index_language][index_of_ekran_tmp][j];
        else        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string_withoutznam[index_language][index_of_ekran_tmp][j];
        if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_STPMTZ1 : WITHOUTZNAM_INDEX_ML_STPMTZ1))
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду
          if (view == true) value = current_settings.setpoint_mtz_1[group]; //у змінну value поміщаємо значення уставки
          else value = edition_settings.setpoint_mtz_1[group];
        }
        else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_STPMTZ1_N_VPERED : WITHOUTZNAM_INDEX_ML_STPMTZ1_N_VPERED))
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду
          if (view == true) value = current_settings.setpoint_mtz_1_n_vpered[group]; //у змінну value поміщаємо значення уставки
          else value = edition_settings.setpoint_mtz_1_n_vpered[group];
        }
        else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_STPMTZ1_N_NAZAD : WITHOUTZNAM_INDEX_ML_STPMTZ1_N_NAZAD))
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду
          if (view == true) value = current_settings.setpoint_mtz_1_n_nazad[group]; //у змінну value поміщаємо значення уставки
          else value = edition_settings.setpoint_mtz_1_n_nazad[group];
        }
        else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_STPMTZ1_ANGLE : WITHOUTZNAM_INDEX_ML_STPMTZ1_ANGLE))
        {
          vaga = 10; //максимальний ваговий коефіцієнт для вилілення старшого розряду
          if (view == true) value = current_settings.setpoint_mtz_1_angle[group]; //у змінну value поміщаємо значення уставки
          else value = edition_settings.setpoint_mtz_1_angle[group];
        }
        else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_STPMTZ1_PO_NAPRUZI : WITHOUTZNAM_INDEX_ML_STPMTZ1_PO_NAPRUZI))
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду
          if (view == true) value = current_settings.setpoint_mtz_1_po_napruzi[group]; //у змінну value поміщаємо значення уставки
          else value = edition_settings.setpoint_mtz_1_po_napruzi[group];
        }
        else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_STPMTZ1_U : WITHOUTZNAM_INDEX_ML_STPMTZ1_U))
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду
          if (view == true) value = current_settings.setpoint_mtz_1_U[group]; //у змінну value поміщаємо значення уставки
          else value = edition_settings.setpoint_mtz_1_U[group];
        }
        else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_STPMTZ1_K : 100))
        {
          vaga = 10000; //максимальний ваговий коефіцієнт для вилілення старшого розряду
          if (view == true) value = current_settings.setpoint_mtz_1_k_znam[group]; //у змінну value поміщаємо значення уставки
          else value = edition_settings.setpoint_mtz_1_k_znam[group];
        }
        else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_STPMTZ2 : WITHOUTZNAM_INDEX_ML_STPMTZ2))
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду
          if (view == true) value = current_settings.setpoint_mtz_2[group]; //у змінну value поміщаємо значення уставки
          else value = edition_settings.setpoint_mtz_2[group];
        }
        else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_STPMTZ2_N_VPERED : WITHOUTZNAM_INDEX_ML_STPMTZ2_N_VPERED))
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду
          if (view == true) value = current_settings.setpoint_mtz_2_n_vpered[group]; //у змінну value поміщаємо значення уставки
          else value = edition_settings.setpoint_mtz_2_n_vpered[group];
        }
        else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_STPMTZ2_N_NAZAD : WITHOUTZNAM_INDEX_ML_STPMTZ2_N_NAZAD))
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду
          if (view == true) value = current_settings.setpoint_mtz_2_n_nazad[group]; //у змінну value поміщаємо значення уставки
          else value = edition_settings.setpoint_mtz_2_n_nazad[group];
        }
        else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_STPMTZ2_ANGLE : WITHOUTZNAM_INDEX_ML_STPMTZ2_ANGLE))
        {
          vaga = 10; //максимальний ваговий коефіцієнт для вилілення старшого розряду
          if (view == true) value = current_settings.setpoint_mtz_2_angle[group]; //у змінну value поміщаємо значення уставки
          else value = edition_settings.setpoint_mtz_2_angle[group];
        }
        else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_STPMTZ2_PO_NAPRUZI : WITHOUTZNAM_INDEX_ML_STPMTZ2_PO_NAPRUZI))
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду
          if (view == true) value = current_settings.setpoint_mtz_2_po_napruzi[group]; //у змінну value поміщаємо значення уставки
          else value = edition_settings.setpoint_mtz_2_po_napruzi[group];
        }
        else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_STPMTZ2_U : WITHOUTZNAM_INDEX_ML_STPMTZ2_U))
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду
          if (view == true) value = current_settings.setpoint_mtz_2_U[group]; //у змінну value поміщаємо значення уставки
          else value = edition_settings.setpoint_mtz_2_U[group];
        }
        else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_STPMTZ2_K : 100))
        {
          vaga = 10000; //максимальний ваговий коефіцієнт для вилілення старшого розряду
          if (view == true) value = current_settings.setpoint_mtz_2_k_znam[group]; //у змінну value поміщаємо значення уставки
          else value = edition_settings.setpoint_mtz_2_k_znam[group];
        }
        else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_STPMTZ3 : WITHOUTZNAM_INDEX_ML_STPMTZ3))
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду
          if (view == true) value = current_settings.setpoint_mtz_3[group]; //у змінну value поміщаємо значення уставки
          else value = edition_settings.setpoint_mtz_3[group];
        }
        else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_STPMTZ3_N_VPERED : WITHOUTZNAM_INDEX_ML_STPMTZ3_N_VPERED))
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду
          if (view == true) value = current_settings.setpoint_mtz_3_n_vpered[group]; //у змінну value поміщаємо значення уставки
          else value = edition_settings.setpoint_mtz_3_n_vpered[group];
        }
        else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_STPMTZ3_N_NAZAD : WITHOUTZNAM_INDEX_ML_STPMTZ3_N_NAZAD))
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду
          if (view == true) value = current_settings.setpoint_mtz_3_n_nazad[group]; //у змінну value поміщаємо значення уставки
          else value = edition_settings.setpoint_mtz_3_n_nazad[group];
        }
        else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_STPMTZ3_ANGLE : WITHOUTZNAM_INDEX_ML_STPMTZ3_ANGLE))
        {
          vaga = 10; //максимальний ваговий коефіцієнт для вилілення старшого розряду
          if (view == true) value = current_settings.setpoint_mtz_3_angle[group]; //у змінну value поміщаємо значення уставки
          else value = edition_settings.setpoint_mtz_3_angle[group];
        }
        else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_STPMTZ3_PO_NAPRUZI : WITHOUTZNAM_INDEX_ML_STPMTZ3_PO_NAPRUZI))
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду
          if (view == true) value = current_settings.setpoint_mtz_3_po_napruzi[group]; //у змінну value поміщаємо значення уставки
          else value = edition_settings.setpoint_mtz_3_po_napruzi[group];
        }
        else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_STPMTZ3_U : WITHOUTZNAM_INDEX_ML_STPMTZ3_U))
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду
          if (view == true) value = current_settings.setpoint_mtz_3_U[group]; //у змінну value поміщаємо значення уставки
          else value = edition_settings.setpoint_mtz_3_U[group];
        }
        else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_STPMTZ3_K : 100))
        {
          vaga = 10000; //максимальний ваговий коефіцієнт для вилілення старшого розряду
          if (view == true) value = current_settings.setpoint_mtz_3_k_znam[group]; //у змінну value поміщаємо значення уставки
          else value = edition_settings.setpoint_mtz_3_k_znam[group];
        }
        else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_STPMTZ4 : WITHOUTZNAM_INDEX_ML_STPMTZ4))
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду
          if (view == true) value = current_settings.setpoint_mtz_4[group]; //у змінну value поміщаємо значення уставки
          else value = edition_settings.setpoint_mtz_4[group];
        }
        else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_STPMTZ4_N_VPERED : WITHOUTZNAM_INDEX_ML_STPMTZ4_N_VPERED))
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду
          if (view == true) value = current_settings.setpoint_mtz_4_n_vpered[group]; //у змінну value поміщаємо значення уставки
          else value = edition_settings.setpoint_mtz_4_n_vpered[group];
        }
        else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_STPMTZ4_N_NAZAD : WITHOUTZNAM_INDEX_ML_STPMTZ4_N_NAZAD))
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду
          if (view == true) value = current_settings.setpoint_mtz_4_n_nazad[group]; //у змінну value поміщаємо значення уставки
          else value = edition_settings.setpoint_mtz_4_n_nazad[group];
        }
        else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_STPMTZ4_ANGLE : WITHOUTZNAM_INDEX_ML_STPMTZ4_ANGLE))
        {
          vaga = 10; //максимальний ваговий коефіцієнт для вилілення старшого розряду
          if (view == true) value = current_settings.setpoint_mtz_4_angle[group]; //у змінну value поміщаємо значення уставки
          else value = edition_settings.setpoint_mtz_4_angle[group];
        }
        else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_STPMTZ4_PO_NAPRUZI : WITHOUTZNAM_INDEX_ML_STPMTZ4_PO_NAPRUZI))
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду
          if (view == true) value = current_settings.setpoint_mtz_4_po_napruzi[group]; //у змінну value поміщаємо значення уставки
          else value = edition_settings.setpoint_mtz_4_po_napruzi[group];
        }
        else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_STPMTZ4_U : WITHOUTZNAM_INDEX_ML_STPMTZ4_U))
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду
          if (view == true) value = current_settings.setpoint_mtz_4_U[group]; //у змінну value поміщаємо значення уставки
          else value = edition_settings.setpoint_mtz_4_U[group];
        }
        else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_STPMTZ4_K : 100))
        {
          vaga = 10000; //максимальний ваговий коефіцієнт для вилілення старшого розряду
          if (view == true) value = current_settings.setpoint_mtz_4_k_znam[group]; //у змінну value поміщаємо значення уставки
          else value = edition_settings.setpoint_mtz_4_k_znam[group];
        }
        
        first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
      }
      else
      {
        //У парному номері рядку виводимо значення уставки
        for (unsigned int j = 0; j<MAX_COL_LCD; j++)
        {
          if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_STPMTZ1 : WITHOUTZNAM_INDEX_ML_STPMTZ1))
          {
            if (
                ((j < COL_SETPOINT_MTZ_1_BEGIN) ||  (j > COL_SETPOINT_MTZ_1_END ))  &&
                (j != (COL_SETPOINT_MTZ_1_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_SETPOINT_MTZ_1_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_SETPOINT_MTZ_1_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_A];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_MTZ_1_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_STPMTZ1_N_VPERED : WITHOUTZNAM_INDEX_ML_STPMTZ1_N_VPERED))
          {
            if (
                ((j < COL_SETPOINT_MTZ_1_N_VPERED_BEGIN) ||  (j > COL_SETPOINT_MTZ_1_N_VPERED_END ))  &&
                (j != (COL_SETPOINT_MTZ_1_N_VPERED_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_SETPOINT_MTZ_1_N_VPERED_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_SETPOINT_MTZ_1_N_VPERED_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_A];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_MTZ_1_N_VPERED_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_STPMTZ1_N_NAZAD : WITHOUTZNAM_INDEX_ML_STPMTZ1_N_NAZAD))
          {
            if (
                ((j < COL_SETPOINT_MTZ_1_N_NAZAD_BEGIN) ||  (j > COL_SETPOINT_MTZ_1_N_NAZAD_END ))  &&
                (j != (COL_SETPOINT_MTZ_1_N_NAZAD_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_SETPOINT_MTZ_1_N_NAZAD_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_SETPOINT_MTZ_1_N_NAZAD_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_A];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_MTZ_1_N_NAZAD_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_STPMTZ1_ANGLE : WITHOUTZNAM_INDEX_ML_STPMTZ1_ANGLE))
          {
            if (
                ((j < COL_SETPOINT_MTZ_1_ANGLE_BEGIN) ||  (j > COL_SETPOINT_MTZ_1_ANGLE_END ))  &&
                (j != (COL_SETPOINT_MTZ_1_ANGLE_END + 1))  
               )working_ekran[i][j] = ' ';
            else if (j == (COL_SETPOINT_MTZ_1_ANGLE_END + 1)) working_ekran[i][j] = '°';
            else
              calc_int_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, view);
          }
          else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_STPMTZ1_PO_NAPRUZI : WITHOUTZNAM_INDEX_ML_STPMTZ1_PO_NAPRUZI))
          {
            if (
                ((j < COL_SETPOINT_MTZ_1_PO_NAPRUZI_BEGIN) ||  (j > COL_SETPOINT_MTZ_1_PO_NAPRUZI_END ))  &&
                (j != (COL_SETPOINT_MTZ_1_PO_NAPRUZI_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_SETPOINT_MTZ_1_PO_NAPRUZI_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_SETPOINT_MTZ_1_PO_NAPRUZI_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_A];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_MTZ_1_PO_NAPRUZI_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_STPMTZ1_U : WITHOUTZNAM_INDEX_ML_STPMTZ1_U))
          {
            if (
                ((j < COL_SETPOINT_MTZ_1_U_BEGIN) ||  (j > COL_SETPOINT_MTZ_1_U_END ))  &&
                (j != (COL_SETPOINT_MTZ_1_U_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_SETPOINT_MTZ_1_U_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_SETPOINT_MTZ_1_U_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_V];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_MTZ_1_U_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_STPMTZ1_K : 100))
          {
            if (
                ((j < COL_SETPOINT_MTZ_1_K_BEGIN) ||  (j > COL_SETPOINT_MTZ_1_K_END )) 
               )working_ekran[i][j] = ' ';
            else if (j == COL_SETPOINT_MTZ_1_K_COMMA )working_ekran[i][j] = ',';
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_MTZ_1_K_COMMA, view, 0);
          }

          else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_STPMTZ2 : WITHOUTZNAM_INDEX_ML_STPMTZ2))
          {
            if (
                ((j < COL_SETPOINT_MTZ_2_BEGIN) ||  (j > COL_SETPOINT_MTZ_2_END ))  &&
                (j != (COL_SETPOINT_MTZ_2_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_SETPOINT_MTZ_2_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_SETPOINT_MTZ_2_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_A];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_MTZ_2_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_STPMTZ2_N_VPERED : WITHOUTZNAM_INDEX_ML_STPMTZ2_N_VPERED))
          {
            if (
                ((j < COL_SETPOINT_MTZ_2_N_VPERED_BEGIN) ||  (j > COL_SETPOINT_MTZ_2_N_VPERED_END ))  &&
                (j != (COL_SETPOINT_MTZ_2_N_VPERED_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_SETPOINT_MTZ_2_N_VPERED_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_SETPOINT_MTZ_2_N_VPERED_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_A];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_MTZ_2_N_VPERED_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_STPMTZ2_N_NAZAD : WITHOUTZNAM_INDEX_ML_STPMTZ2_N_NAZAD))
          {
            if (
                ((j < COL_SETPOINT_MTZ_2_N_NAZAD_BEGIN) ||  (j > COL_SETPOINT_MTZ_2_N_NAZAD_END ))  &&
                (j != (COL_SETPOINT_MTZ_2_N_NAZAD_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_SETPOINT_MTZ_2_N_NAZAD_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_SETPOINT_MTZ_2_N_NAZAD_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_A];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_MTZ_2_N_NAZAD_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_STPMTZ2_ANGLE : WITHOUTZNAM_INDEX_ML_STPMTZ2_ANGLE))
          {
            if (
                ((j < COL_SETPOINT_MTZ_2_ANGLE_BEGIN) ||  (j > COL_SETPOINT_MTZ_2_ANGLE_END ))  &&
                (j != (COL_SETPOINT_MTZ_2_ANGLE_END + 1))  
               )working_ekran[i][j] = ' ';
            else if (j == (COL_SETPOINT_MTZ_2_ANGLE_END + 1)) working_ekran[i][j] = '°';
            else
              calc_int_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, view);
          }
          else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_STPMTZ2_PO_NAPRUZI : WITHOUTZNAM_INDEX_ML_STPMTZ2_PO_NAPRUZI))
          {
            if (
                ((j < COL_SETPOINT_MTZ_2_PO_NAPRUZI_BEGIN) ||  (j > COL_SETPOINT_MTZ_2_PO_NAPRUZI_END ))  &&
                (j != (COL_SETPOINT_MTZ_2_PO_NAPRUZI_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_SETPOINT_MTZ_2_PO_NAPRUZI_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_SETPOINT_MTZ_2_PO_NAPRUZI_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_A];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_MTZ_2_PO_NAPRUZI_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_STPMTZ2_U : WITHOUTZNAM_INDEX_ML_STPMTZ2_U))
          {
            if (
                ((j < COL_SETPOINT_MTZ_2_U_BEGIN) ||  (j > COL_SETPOINT_MTZ_2_U_END ))  &&
                (j != (COL_SETPOINT_MTZ_2_U_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_SETPOINT_MTZ_2_U_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_SETPOINT_MTZ_2_U_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_V];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_MTZ_2_U_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_STPMTZ2_K : 100))
          {
            if (
                ((j < COL_SETPOINT_MTZ_2_K_BEGIN) ||  (j > COL_SETPOINT_MTZ_2_K_END )) 
               )working_ekran[i][j] = ' ';
            else if (j == COL_SETPOINT_MTZ_2_K_COMMA )working_ekran[i][j] = ',';
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_MTZ_2_K_COMMA, view, 0);
          }

          else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_STPMTZ3 : WITHOUTZNAM_INDEX_ML_STPMTZ3))
          {
            if (
                ((j < COL_SETPOINT_MTZ_3_BEGIN) ||  (j > COL_SETPOINT_MTZ_3_END ))  &&
                (j != (COL_SETPOINT_MTZ_3_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_SETPOINT_MTZ_3_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_SETPOINT_MTZ_3_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_A];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_MTZ_3_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_STPMTZ3_N_VPERED : WITHOUTZNAM_INDEX_ML_STPMTZ3_N_VPERED))
          {
            if (
                ((j < COL_SETPOINT_MTZ_3_N_VPERED_BEGIN) ||  (j > COL_SETPOINT_MTZ_3_N_VPERED_END ))  &&
                (j != (COL_SETPOINT_MTZ_3_N_VPERED_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_SETPOINT_MTZ_3_N_VPERED_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_SETPOINT_MTZ_3_N_VPERED_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_A];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_MTZ_3_N_VPERED_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_STPMTZ3_N_NAZAD : WITHOUTZNAM_INDEX_ML_STPMTZ3_N_NAZAD))
          {
            if (
                ((j < COL_SETPOINT_MTZ_3_N_NAZAD_BEGIN) ||  (j > COL_SETPOINT_MTZ_3_N_NAZAD_END ))  &&
                (j != (COL_SETPOINT_MTZ_3_N_NAZAD_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_SETPOINT_MTZ_3_N_NAZAD_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_SETPOINT_MTZ_3_N_NAZAD_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_A];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_MTZ_3_N_NAZAD_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_STPMTZ3_ANGLE : WITHOUTZNAM_INDEX_ML_STPMTZ3_ANGLE))
          {
            if (
                ((j < COL_SETPOINT_MTZ_3_ANGLE_BEGIN) ||  (j > COL_SETPOINT_MTZ_3_ANGLE_END ))  &&
                (j != (COL_SETPOINT_MTZ_3_ANGLE_END + 1))  
               )working_ekran[i][j] = ' ';
            else if (j == (COL_SETPOINT_MTZ_3_ANGLE_END + 1)) working_ekran[i][j] = '°';
            else
              calc_int_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, view);
          }
          else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_STPMTZ3_PO_NAPRUZI : WITHOUTZNAM_INDEX_ML_STPMTZ3_PO_NAPRUZI))
          {
            if (
                ((j < COL_SETPOINT_MTZ_3_PO_NAPRUZI_BEGIN) ||  (j > COL_SETPOINT_MTZ_3_PO_NAPRUZI_END ))  &&
                (j != (COL_SETPOINT_MTZ_3_PO_NAPRUZI_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_SETPOINT_MTZ_3_PO_NAPRUZI_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_SETPOINT_MTZ_3_PO_NAPRUZI_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_A];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_MTZ_3_PO_NAPRUZI_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_STPMTZ3_U : WITHOUTZNAM_INDEX_ML_STPMTZ3_U))
          {
            if (
                ((j < COL_SETPOINT_MTZ_3_U_BEGIN) ||  (j > COL_SETPOINT_MTZ_3_U_END ))  &&
                (j != (COL_SETPOINT_MTZ_3_U_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_SETPOINT_MTZ_3_U_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_SETPOINT_MTZ_3_U_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_V];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_MTZ_3_U_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_STPMTZ3_K : 100))
          {
            if (
                ((j < COL_SETPOINT_MTZ_3_K_BEGIN) ||  (j > COL_SETPOINT_MTZ_3_K_END )) 
               )working_ekran[i][j] = ' ';
            else if (j == COL_SETPOINT_MTZ_3_K_COMMA )working_ekran[i][j] = ',';
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_MTZ_3_K_COMMA, view, 0);
          }

          else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_STPMTZ4 : WITHOUTZNAM_INDEX_ML_STPMTZ4))
          {
            if (
                ((j < COL_SETPOINT_MTZ_4_BEGIN) ||  (j > COL_SETPOINT_MTZ_4_END ))  &&
                (j != (COL_SETPOINT_MTZ_4_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_SETPOINT_MTZ_4_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_SETPOINT_MTZ_4_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_A];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_MTZ_4_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_STPMTZ4_N_VPERED : WITHOUTZNAM_INDEX_ML_STPMTZ4_N_VPERED))
          {
            if (
                ((j < COL_SETPOINT_MTZ_4_N_VPERED_BEGIN) ||  (j > COL_SETPOINT_MTZ_4_N_VPERED_END ))  &&
                (j != (COL_SETPOINT_MTZ_4_N_VPERED_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_SETPOINT_MTZ_4_N_VPERED_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_SETPOINT_MTZ_4_N_VPERED_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_A];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_MTZ_4_N_VPERED_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_STPMTZ4_N_NAZAD : WITHOUTZNAM_INDEX_ML_STPMTZ4_N_NAZAD))
          {
            if (
                ((j < COL_SETPOINT_MTZ_4_N_NAZAD_BEGIN) ||  (j > COL_SETPOINT_MTZ_4_N_NAZAD_END ))  &&
                (j != (COL_SETPOINT_MTZ_4_N_NAZAD_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_SETPOINT_MTZ_4_N_NAZAD_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_SETPOINT_MTZ_4_N_NAZAD_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_A];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_MTZ_4_N_NAZAD_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_STPMTZ4_ANGLE : WITHOUTZNAM_INDEX_ML_STPMTZ4_ANGLE))
          {
            if (
                ((j < COL_SETPOINT_MTZ_4_ANGLE_BEGIN) ||  (j > COL_SETPOINT_MTZ_4_ANGLE_END ))  &&
                (j != (COL_SETPOINT_MTZ_4_ANGLE_END + 1))  
               )working_ekran[i][j] = ' ';
            else if (j == (COL_SETPOINT_MTZ_4_ANGLE_END + 1)) working_ekran[i][j] = '°';
            else
              calc_int_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, view);
          }
          else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_STPMTZ4_PO_NAPRUZI : WITHOUTZNAM_INDEX_ML_STPMTZ4_PO_NAPRUZI))
          {
            if (
                ((j < COL_SETPOINT_MTZ_4_PO_NAPRUZI_BEGIN) ||  (j > COL_SETPOINT_MTZ_4_PO_NAPRUZI_END ))  &&
                (j != (COL_SETPOINT_MTZ_4_PO_NAPRUZI_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_SETPOINT_MTZ_4_PO_NAPRUZI_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_SETPOINT_MTZ_4_PO_NAPRUZI_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_A];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_MTZ_4_PO_NAPRUZI_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_STPMTZ4_U : WITHOUTZNAM_INDEX_ML_STPMTZ4_U))
          {
            if (
                ((j < COL_SETPOINT_MTZ_4_U_BEGIN) ||  (j > COL_SETPOINT_MTZ_4_U_END ))  &&
                (j != (COL_SETPOINT_MTZ_4_U_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_SETPOINT_MTZ_4_U_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_SETPOINT_MTZ_4_U_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_V];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_MTZ_4_U_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_STPMTZ4_K : 100))
          {
            if (
                ((j < COL_SETPOINT_MTZ_4_K_BEGIN) ||  (j > COL_SETPOINT_MTZ_4_K_END )) 
               )working_ekran[i][j] = ' ';
            else if (j == COL_SETPOINT_MTZ_4_K_COMMA )working_ekran[i][j] = ',';
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_MTZ_4_K_COMMA, view, 0);
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
    if (current_ekran.index_position == (isZNam? WITHZNAM_INDEX_ML_STPMTZ1 : WITHOUTZNAM_INDEX_ML_STPMTZ1))
    {
      current_ekran.position_cursor_x = COL_SETPOINT_MTZ_1_BEGIN;
      last_position_cursor_x = COL_SETPOINT_MTZ_1_END;
    }
    else if (current_ekran.index_position == (isZNam? WITHZNAM_INDEX_ML_STPMTZ1_N_VPERED : WITHOUTZNAM_INDEX_ML_STPMTZ1_N_VPERED))
    {
      current_ekran.position_cursor_x = COL_SETPOINT_MTZ_1_N_VPERED_BEGIN;
      last_position_cursor_x = COL_SETPOINT_MTZ_1_N_VPERED_END;
    }
    else if (current_ekran.index_position == (isZNam? WITHZNAM_INDEX_ML_STPMTZ1_N_NAZAD : WITHOUTZNAM_INDEX_ML_STPMTZ1_N_NAZAD))
    {
      current_ekran.position_cursor_x = COL_SETPOINT_MTZ_1_N_NAZAD_BEGIN;
      last_position_cursor_x = COL_SETPOINT_MTZ_1_N_NAZAD_END;
    }
    else if (current_ekran.index_position == (isZNam? WITHZNAM_INDEX_ML_STPMTZ1_ANGLE : WITHOUTZNAM_INDEX_ML_STPMTZ1_ANGLE))
    {
      current_ekran.position_cursor_x = COL_SETPOINT_MTZ_1_ANGLE_BEGIN;
      last_position_cursor_x = COL_SETPOINT_MTZ_1_ANGLE_END;
    }
    else if (current_ekran.index_position == (isZNam? WITHZNAM_INDEX_ML_STPMTZ1_PO_NAPRUZI : WITHOUTZNAM_INDEX_ML_STPMTZ1_PO_NAPRUZI))
    {
      current_ekran.position_cursor_x = COL_SETPOINT_MTZ_1_PO_NAPRUZI_BEGIN;
      last_position_cursor_x = COL_SETPOINT_MTZ_1_PO_NAPRUZI_END;
    }
    else if (current_ekran.index_position == (isZNam? WITHZNAM_INDEX_ML_STPMTZ1_U : WITHOUTZNAM_INDEX_ML_STPMTZ1_U))
    {
      current_ekran.position_cursor_x = COL_SETPOINT_MTZ_1_U_BEGIN;
      last_position_cursor_x = COL_SETPOINT_MTZ_1_U_END;
    }
    else if (current_ekran.index_position == (isZNam? WITHZNAM_INDEX_ML_STPMTZ1_K : 100))
    {
      current_ekran.position_cursor_x = COL_SETPOINT_MTZ_1_K_BEGIN;
      last_position_cursor_x = COL_SETPOINT_MTZ_1_K_END;
    }

    else if (current_ekran.index_position == (isZNam? WITHZNAM_INDEX_ML_STPMTZ2 : WITHOUTZNAM_INDEX_ML_STPMTZ2))
    {
      current_ekran.position_cursor_x = COL_SETPOINT_MTZ_2_BEGIN;
      last_position_cursor_x = COL_SETPOINT_MTZ_2_END;
    }
    else if (current_ekran.index_position == (isZNam? WITHZNAM_INDEX_ML_STPMTZ2_N_VPERED : WITHOUTZNAM_INDEX_ML_STPMTZ2_N_VPERED))
    {
      current_ekran.position_cursor_x = COL_SETPOINT_MTZ_2_N_VPERED_BEGIN;
      last_position_cursor_x = COL_SETPOINT_MTZ_2_N_VPERED_END;
    }
    else if (current_ekran.index_position == (isZNam? WITHZNAM_INDEX_ML_STPMTZ2_N_NAZAD : WITHOUTZNAM_INDEX_ML_STPMTZ2_N_NAZAD))
    {
      current_ekran.position_cursor_x = COL_SETPOINT_MTZ_2_N_NAZAD_BEGIN;
      last_position_cursor_x = COL_SETPOINT_MTZ_2_N_NAZAD_END;
    }
    else if (current_ekran.index_position == (isZNam? WITHZNAM_INDEX_ML_STPMTZ2_ANGLE : WITHOUTZNAM_INDEX_ML_STPMTZ2_ANGLE))
    {
      current_ekran.position_cursor_x = COL_SETPOINT_MTZ_2_ANGLE_BEGIN;
      last_position_cursor_x = COL_SETPOINT_MTZ_2_ANGLE_END;
    }
    else if (current_ekran.index_position == (isZNam? WITHZNAM_INDEX_ML_STPMTZ2_PO_NAPRUZI : WITHOUTZNAM_INDEX_ML_STPMTZ2_PO_NAPRUZI))
    {
      current_ekran.position_cursor_x = COL_SETPOINT_MTZ_2_PO_NAPRUZI_BEGIN;
      last_position_cursor_x = COL_SETPOINT_MTZ_2_PO_NAPRUZI_END;
    }
    else if (current_ekran.index_position == (isZNam? WITHZNAM_INDEX_ML_STPMTZ2_U : WITHOUTZNAM_INDEX_ML_STPMTZ2_U))
    {
      current_ekran.position_cursor_x = COL_SETPOINT_MTZ_2_U_BEGIN;
      last_position_cursor_x = COL_SETPOINT_MTZ_2_U_END;
    }
    else if (current_ekran.index_position == (isZNam? WITHZNAM_INDEX_ML_STPMTZ2_K : 100))
    {
      current_ekran.position_cursor_x = COL_SETPOINT_MTZ_2_K_BEGIN;
      last_position_cursor_x = COL_SETPOINT_MTZ_2_K_END;
    }

    else if (current_ekran.index_position == (isZNam? WITHZNAM_INDEX_ML_STPMTZ3 : WITHOUTZNAM_INDEX_ML_STPMTZ3))
    {
      current_ekran.position_cursor_x = COL_SETPOINT_MTZ_3_BEGIN;
      last_position_cursor_x = COL_SETPOINT_MTZ_3_END;
    }
    else if (current_ekran.index_position == (isZNam? WITHZNAM_INDEX_ML_STPMTZ3_N_VPERED : WITHOUTZNAM_INDEX_ML_STPMTZ3_N_VPERED))
    {
      current_ekran.position_cursor_x = COL_SETPOINT_MTZ_3_N_VPERED_BEGIN;
      last_position_cursor_x = COL_SETPOINT_MTZ_3_N_VPERED_END;
    }
    else if (current_ekran.index_position == (isZNam? WITHZNAM_INDEX_ML_STPMTZ3_N_NAZAD : WITHOUTZNAM_INDEX_ML_STPMTZ3_N_NAZAD))
    {
      current_ekran.position_cursor_x = COL_SETPOINT_MTZ_3_N_NAZAD_BEGIN;
      last_position_cursor_x = COL_SETPOINT_MTZ_3_N_NAZAD_END;
    }
    else if (current_ekran.index_position == (isZNam? WITHZNAM_INDEX_ML_STPMTZ3_ANGLE : WITHOUTZNAM_INDEX_ML_STPMTZ3_ANGLE))
    {
      current_ekran.position_cursor_x = COL_SETPOINT_MTZ_3_ANGLE_BEGIN;
      last_position_cursor_x = COL_SETPOINT_MTZ_3_ANGLE_END;
    }
    else if (current_ekran.index_position == (isZNam? WITHZNAM_INDEX_ML_STPMTZ3_PO_NAPRUZI : WITHOUTZNAM_INDEX_ML_STPMTZ3_PO_NAPRUZI))
    {
      current_ekran.position_cursor_x = COL_SETPOINT_MTZ_3_PO_NAPRUZI_BEGIN;
      last_position_cursor_x = COL_SETPOINT_MTZ_3_PO_NAPRUZI_END;
    }
    else if (current_ekran.index_position == (isZNam? WITHZNAM_INDEX_ML_STPMTZ3_U : WITHOUTZNAM_INDEX_ML_STPMTZ3_U))
    {
      current_ekran.position_cursor_x = COL_SETPOINT_MTZ_3_U_BEGIN;
      last_position_cursor_x = COL_SETPOINT_MTZ_3_U_END;
    }
    else if (current_ekran.index_position == (isZNam? WITHZNAM_INDEX_ML_STPMTZ3_K : 100))
    {
      current_ekran.position_cursor_x = COL_SETPOINT_MTZ_3_K_BEGIN;
      last_position_cursor_x = COL_SETPOINT_MTZ_3_K_END;
    }

    else if (current_ekran.index_position == (isZNam? WITHZNAM_INDEX_ML_STPMTZ4 : WITHOUTZNAM_INDEX_ML_STPMTZ4))
    {
      current_ekran.position_cursor_x = COL_SETPOINT_MTZ_4_BEGIN;
      last_position_cursor_x = COL_SETPOINT_MTZ_4_END;
    }
    else if (current_ekran.index_position == (isZNam? WITHZNAM_INDEX_ML_STPMTZ4_N_VPERED : WITHOUTZNAM_INDEX_ML_STPMTZ4_N_VPERED))
    {
      current_ekran.position_cursor_x = COL_SETPOINT_MTZ_4_N_VPERED_BEGIN;
      last_position_cursor_x = COL_SETPOINT_MTZ_4_N_VPERED_END;
    }
    else if (current_ekran.index_position == (isZNam? WITHZNAM_INDEX_ML_STPMTZ4_N_NAZAD : WITHOUTZNAM_INDEX_ML_STPMTZ4_N_NAZAD))
    {
      current_ekran.position_cursor_x = COL_SETPOINT_MTZ_4_N_NAZAD_BEGIN;
      last_position_cursor_x = COL_SETPOINT_MTZ_4_N_NAZAD_END;
    }
    else if (current_ekran.index_position == (isZNam? WITHZNAM_INDEX_ML_STPMTZ4_ANGLE : WITHOUTZNAM_INDEX_ML_STPMTZ4_ANGLE))
    {
      current_ekran.position_cursor_x = COL_SETPOINT_MTZ_4_ANGLE_BEGIN;
      last_position_cursor_x = COL_SETPOINT_MTZ_4_ANGLE_END;
    }
    else if (current_ekran.index_position == (isZNam? WITHZNAM_INDEX_ML_STPMTZ4_PO_NAPRUZI : WITHOUTZNAM_INDEX_ML_STPMTZ4_PO_NAPRUZI))
    {
      current_ekran.position_cursor_x = COL_SETPOINT_MTZ_4_PO_NAPRUZI_BEGIN;
      last_position_cursor_x = COL_SETPOINT_MTZ_4_PO_NAPRUZI_END;
    }
    else if (current_ekran.index_position == (isZNam? WITHZNAM_INDEX_ML_STPMTZ4_U : WITHOUTZNAM_INDEX_ML_STPMTZ4_U))
    {
      current_ekran.position_cursor_x = COL_SETPOINT_MTZ_4_U_BEGIN;
      last_position_cursor_x = COL_SETPOINT_MTZ_4_U_END;
    }
    else if (current_ekran.index_position == (isZNam? WITHZNAM_INDEX_ML_STPMTZ4_K : 100))
    {
      current_ekran.position_cursor_x = COL_SETPOINT_MTZ_4_K_BEGIN;
      last_position_cursor_x = COL_SETPOINT_MTZ_4_K_END;
    }
    
    //Підтягуємо курсор до першого символу
    while (((working_ekran[current_ekran.position_cursor_y][current_ekran.position_cursor_x + 1]) == ' ') && 
           (current_ekran.position_cursor_x < (last_position_cursor_x -1))) current_ekran.position_cursor_x++;
    
    //Курсор ставимо так, щоб він був перед числом
    if (((working_ekran[current_ekran.position_cursor_y][current_ekran.position_cursor_x]) != ' ') && 
        (current_ekran.position_cursor_x > 0)) current_ekran.position_cursor_x--;
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

/*****************************************************/
//Формуємо екран відображення витримок МТЗ
/*****************************************************/
void make_ekran_timeout_mtz(unsigned int group)
{
  static const unsigned char name_string_withznam[MAX_NAMBER_LANGUAGE][WITHZNAM_MAX_ROW_FOR_TIMEOUT_MTZ][MAX_COL_LCD] = 
  {
    {
      "     МТЗ 1      ",
      "   МТЗН 1 Пр.   ",
      "  МТЗН 1 Обр.   ",
      "    МТЗПН 1     ",
      "Загр.МТЗ 1(ЗНам)",
      "     МТЗ 2      ",
      "   Уск.МТЗ 2    ",
      "   МТЗН 2 Пр.   ",
      " Уск.МТЗН 2 Пр. ",
      "  МТЗН 2 Обр.   ",
      " Уск.МТЗН 2 Обр.",
      "    МТЗПН 2     ",
      "  Уск.МТЗПН 2   ",
      " Ввод Уск.МТЗ 2 ",
      "Загр.МТЗ 2(ЗНам)",
      "     МТЗ 3      ",
      "   МТЗН 3 Пр.   ",
      "  МТЗН 3 Обр.   ",
      "    МТЗПН 3     ",
      "Загр.МТЗ 3(ЗНам)",
      "     МТЗ 4      ",
      "   МТЗН 4 Пр.   ",
      "  МТЗН 4 Обр.   ",
      "    МТЗПН 4     ",
      "Загр.МТЗ 4(ЗНам)"
    },
    {
      "     МСЗ 1      ",
      "   МСЗС 1 Пр.   ",
      "   МСЗС 1 Зв.   ",
      "    МСЗПН 1     ",
      "Загр.МCЗ 1(ЗНам)",
      "     МСЗ 2      ",
      "  Приск.МСЗ 2   ",
      "   МСЗС 2 Пр.   ",
      "Приск.МСЗС 2 Пр.",
      "   МСЗС 2 Зв.   ",
      "Приск.МСЗС 2 Зв.",
      "     МСЗПН 2    ",
      " Приск.МСЗПН 2  ",
      "Ввід Приск.МСЗ 2",
      "Загр.МCЗ 2(ЗНам)",
      "     МСЗ 3      ",
      "   МСЗС 3 Пр.   ",
      "   МСЗС 3 Зв.   ",
      "    МСЗПН 3     ",
      "Загр.МCЗ 3(ЗНам)",
      "     МСЗ 4      ",
      "   МСЗС 4 Пр.   ",
      "   МСЗС 4 Зв.   ",
      "    МСЗПН 4     ",
      "Загр.МCЗ 4(ЗНам)"
    },
    {
      "     OCP 1      ",
      "   DOCP 1 Fw    ",
      "   DOCP 1 Bw    ",
      "     VOCP 1     ",
      "OCP 1 InrP coars",
      "     OCP 2      ",
      "  OCP 2 AcOpDl  ",
      "   DOCP 2 Fw    ",
      "DOCP 2 Fw AcOpDl",
      "   DOCP 2 Bw    ",
      "DOCP 2 Bw AcOpDl",
      "     VOCP 2     ",
      " VOCP 2 AcOpDl  ",
      "OCP 2 Acc Ena T ",
      "OCP 2 InrP coars",
      "     OCP 3      ",
      "   DOCP 3 Fw    ",
      "   DOCP 3 Bw    ",
      "     VOCP 3     ",
      "OCP 3 InrP coars",
      "     OCP 4      ",
      "   DOCP 4 Fw    ",
      "   DOCP 4 Bw    ",
      "     VOCP 4     ",
      "OCP 4 InrP coars",
    },
    {
      "     МТЗ 1      ",
      "   МТЗН 1 Пр.   ",
      "  МТЗН 1 Обр.   ",
      "    МТЗПН 1     ",
      "Загр.МТЗ 1(ЗНам)",
      "     МТЗ 2      ",
      "   Уск.МТЗ 2    ",
      "   МТЗН 2 Пр.   ",
      " Уск.МТЗН 2 Пр. ",
      "  МТЗН 2 Обр.   ",
      " Уск.МТЗН 2 Обр.",
      "    МТЗПН 2     ",
      "  Уск.МТЗПН 2   ",
      " Ввод Уск.МТЗ 2 ",
      "Загр.МТЗ 2(ЗНам)",
      "     МТЗ 3      ",
      "   МТЗН 3 Пр.   ",
      "  МТЗН 3 Обр.   ",
      "    МТЗПН 3     ",
      "Загр.МТЗ 3(ЗНам)",
      "     МТЗ 4      ",
      "   МТЗН 4 Пр.   ",
      "  МТЗН 4 Обр.   ",
      "    МТЗПН 4     ",
      "Загр.МТЗ 4(ЗНам)"
    }
  };
  static const unsigned char name_string_withoutznam[MAX_NAMBER_LANGUAGE][WITHOUTZNAM_MAX_ROW_FOR_TIMEOUT_MTZ][MAX_COL_LCD] = 
  {
    {
      "     МТЗ 1      ",
      "   МТЗН 1 Пр.   ",
      "  МТЗН 1 Обр.   ",
      "    МТЗПН 1     ",
      "     МТЗ 2      ",
      "   Уск.МТЗ 2    ",
      "   МТЗН 2 Пр.   ",
      " Уск.МТЗН 2 Пр. ",
      "  МТЗН 2 Обр.   ",
      " Уск.МТЗН 2 Обр.",
      "    МТЗПН 2     ",
      "  Уск.МТЗПН 2   ",
      " Ввод Уск.МТЗ 2 ",
      "     МТЗ 3      ",
      "   МТЗН 3 Пр.   ",
      "  МТЗН 3 Обр.   ",
      "    МТЗПН 3     ",
      "     МТЗ 4      ",
      "   МТЗН 4 Пр.   ",
      "  МТЗН 4 Обр.   ",
      "    МТЗПН 4     "
    },
    {
      "     МСЗ 1      ",
      " МСЗС 1 Прямий  ",
      " МСЗС 1 Звор.   ",
      "    МСЗПН 1     ",
      "     МСЗ 2      ",
      "  Приск.МСЗ 2   ",
      " МСЗС 2 Прямий  ",
      "Приск.МСЗС 2 Пр.",
      "  МСЗС 2 Звор.  ",
      "Приск.МСЗС 2 Зв.",
      "     МСЗПН 2    ",
      " Приск.МСЗПН 2  ",
      "Ввід Приск.МСЗ 2",
      "     МСЗ 3      ",
      " МСЗС 3 Прямий  ",
      " МСЗС 3 Звор.   ",
      "    МСЗПН 3     ",
      "     МСЗ 4      ",
      " МСЗС 4 Прямий  ",
      " МСЗС 4 Звор.   ",
      "    МСЗПН 4     "
    },
    {
      "     OCP 1      ",
      "   DOCP 1 Fw    ",
      "   DOCP 1 Bw    ",
      "     VOCP 1     ",
      "     OCP 2      ",
      "  OCP 2 AcOpDl  ",
      "   DOCP 2 Fw    ",
      "DOCP 2 Fw AcOpDl",
      "   DOCP 2 Bw    ",
      "DOCP 2 Bw AcOpDl",
      "     VOCP 2     ",
      " VOCP 2 AcOpDl  ",
      "OCP 2 Acc Ena T ",
      "     OCP 3      ",
      "   DOCP 3 Fw    ",
      "   DOCP 3 Bw    ",
      "     VOCP 3     ",
      "     OCP 4      ",
      "   DOCP 4 Fw    ",
      "   DOCP 4 Bw    ",
      "     VOCP 4     "
    },
    {
      "     МТЗ 1      ",
      "   МТЗН 1 Пр.   ",
      "  МТЗН 1 Обр.   ",
      "    МТЗПН 1     ",
      "     МТЗ 2      ",
      "   Уск.МТЗ 2    ",
      "   МТЗН 2 Пр.   ",
      " Уск.МТЗН 2 Пр. ",
      "  МТЗН 2 Обр.   ",
      " Уск.МТЗН 2 Обр.",
      "    МТЗПН 2     ",
      "  Уск.МТЗПН 2   ",
      " Ввод Уск.МТЗ 2 ",
      "     МТЗ 3      ",
      "   МТЗН 3 Пр.   ",
      "  МТЗН 3 Обр.   ",
      "    МТЗПН 3     ",
      "     МТЗ 4      ",
      "   МТЗН 4 Пр.   ",
      "  МТЗН 4 Обр.   ",
      "    МТЗПН 4     "
    }
  };

  int index_language = index_language_in_array(current_settings.language);
  unsigned int isZNam = current_settings.configuration & (1 << ZNAM_BIT_CONFIGURATION);
 
  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  unsigned int vaga, value, first_symbol;
  
  //Множення на два величини position_temp потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
  index_of_ekran = ((position_temp<<1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    unsigned int index_of_ekran_tmp = index_of_ekran >> 1;
    unsigned int view = ((current_ekran.edition == 0) || (position_temp != index_of_ekran_tmp));
    if (index_of_ekran_tmp < (unsigned int)(isZNam? WITHZNAM_MAX_ROW_FOR_TIMEOUT_MTZ : WITHOUTZNAM_MAX_ROW_FOR_TIMEOUT_MTZ))
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        if(isZNam)  for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string_withznam[index_language][index_of_ekran_tmp][j];
        else        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string_withoutznam[index_language][index_of_ekran_tmp][j];
        if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ1 : WITHOUTZNAM_INDEX_ML_TMOMTZ1))
        {
          vaga = 10000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (view == true) value = current_settings.timeout_mtz_1[group]; //у змінну value поміщаємо значення витримки
          else value = edition_settings.timeout_mtz_1[group];
        }
        else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ1_N_VPERED : WITHOUTZNAM_INDEX_ML_TMOMTZ1_N_VPERED))
        {
          vaga = 10000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (view == true) value = current_settings.timeout_mtz_1_n_vpered[group]; //у змінну value поміщаємо значення витримки
          else value = edition_settings.timeout_mtz_1_n_vpered[group];
        }
        else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ1_N_NAZAD : WITHOUTZNAM_INDEX_ML_TMOMTZ1_N_NAZAD))
        {
          vaga = 10000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (view == true) value = current_settings.timeout_mtz_1_n_nazad[group]; //у змінну value поміщаємо значення витримки
          else value = edition_settings.timeout_mtz_1_n_nazad[group];
        }
        else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ1_PO_NAPRUZI : WITHOUTZNAM_INDEX_ML_TMOMTZ1_PO_NAPRUZI))
        {
          vaga = 10000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (view == true) value = current_settings.timeout_mtz_1_po_napruzi[group]; //у змінну value поміщаємо значення витримки
          else value = edition_settings.timeout_mtz_1_po_napruzi[group];
        }
        else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ1_K : 100))
        {
          vaga = 10000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (view == true) value = current_settings.timeout_mtz_1_k_znam[group]; //у змінну value поміщаємо значення витримки
          else value = edition_settings.timeout_mtz_1_k_znam[group];
        }

        else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ2 : WITHOUTZNAM_INDEX_ML_TMOMTZ2))
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (view == true) value = current_settings.timeout_mtz_2[group]; //у змінну value поміщаємо значення витримки
          else value = edition_settings.timeout_mtz_2[group];
        }
        else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ2_PR : WITHOUTZNAM_INDEX_ML_TMOMTZ2_PR))
        {
          vaga = 1000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (view == true) value = current_settings.timeout_mtz_2_pr[group]; //у змінну value поміщаємо значення витримки
          else value = edition_settings.timeout_mtz_2_pr[group];
        }
        else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ2_N_VPERED : WITHOUTZNAM_INDEX_ML_TMOMTZ2_N_VPERED))
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (view == true) value = current_settings.timeout_mtz_2_n_vpered[group]; //у змінну value поміщаємо значення витримки
          else value = edition_settings.timeout_mtz_2_n_vpered[group];
        }
        else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ2_N_VPERED_PR : WITHOUTZNAM_INDEX_ML_TMOMTZ2_N_VPERED_PR))
        {
          vaga = 1000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (view == true) value = current_settings.timeout_mtz_2_n_vpered_pr[group]; //у змінну value поміщаємо значення витримки
          else value = edition_settings.timeout_mtz_2_n_vpered_pr[group];
        }
        else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ2_N_NAZAD : WITHOUTZNAM_INDEX_ML_TMOMTZ2_N_NAZAD))
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (view == true) value = current_settings.timeout_mtz_2_n_nazad[group]; //у змінну value поміщаємо значення витримки
          else value = edition_settings.timeout_mtz_2_n_nazad[group];
        }
        else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ2_N_NAZAD_PR : WITHOUTZNAM_INDEX_ML_TMOMTZ2_N_NAZAD_PR))
        {
          vaga = 1000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (view == true) value = current_settings.timeout_mtz_2_n_nazad_pr[group]; //у змінну value поміщаємо значення витримки
          else value = edition_settings.timeout_mtz_2_n_nazad_pr[group];
        }
        else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ2_PO_NAPRUZI : WITHOUTZNAM_INDEX_ML_TMOMTZ2_PO_NAPRUZI))
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (view == true) value = current_settings.timeout_mtz_2_po_napruzi[group]; //у змінну value поміщаємо значення витримки
          else value = edition_settings.timeout_mtz_2_po_napruzi[group];
        }
        else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ2_PO_NAPRUZI_PR : WITHOUTZNAM_INDEX_ML_TMOMTZ2_PO_NAPRUZI_PR))
        {
          vaga = 1000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (view == true) value = current_settings.timeout_mtz_2_po_napruzi_pr[group]; //у змінну value поміщаємо значення витримки
          else value = edition_settings.timeout_mtz_2_po_napruzi_pr[group];
        }
        else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ2_VVID_PR : WITHOUTZNAM_INDEX_ML_TMOMTZ2_VVID_PR))
        {
          vaga = 1000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (view == true) value = current_settings.timeout_mtz_2_vvid_pr[group]; //у змінну value поміщаємо значення витримки
          else value = edition_settings.timeout_mtz_2_vvid_pr[group];
        }
        else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ2_K : 100))
        {
          vaga = 10000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (view == true) value = current_settings.timeout_mtz_2_k_znam[group]; //у змінну value поміщаємо значення витримки
          else value = edition_settings.timeout_mtz_2_k_znam[group];
        }

        else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ3 : WITHOUTZNAM_INDEX_ML_TMOMTZ3))
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (view == true) value = current_settings.timeout_mtz_3[group]; //у змінну value поміщаємо значення витримки
          else value = edition_settings.timeout_mtz_3[group];
        }
        else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ3_N_VPERED : WITHOUTZNAM_INDEX_ML_TMOMTZ3_N_VPERED))
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (view == true) value = current_settings.timeout_mtz_3_n_vpered[group]; //у змінну value поміщаємо значення витримки
          else value = edition_settings.timeout_mtz_3_n_vpered[group];
        }
        else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ3_N_NAZAD : WITHOUTZNAM_INDEX_ML_TMOMTZ3_N_NAZAD))
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (view == true) value = current_settings.timeout_mtz_3_n_nazad[group]; //у змінну value поміщаємо значення витримки
          else value = edition_settings.timeout_mtz_3_n_nazad[group];
        }
        else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ3_PO_NAPRUZI : WITHOUTZNAM_INDEX_ML_TMOMTZ3_PO_NAPRUZI))
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (view == true) value = current_settings.timeout_mtz_3_po_napruzi[group]; //у змінну value поміщаємо значення витримки
          else value = edition_settings.timeout_mtz_3_po_napruzi[group];
        }
        else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ3_K : 100))
        {
          vaga = 10000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (view == true) value = current_settings.timeout_mtz_3_k_znam[group]; //у змінну value поміщаємо значення витримки
          else value = edition_settings.timeout_mtz_3_k_znam[group];
        }

        else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ4 : WITHOUTZNAM_INDEX_ML_TMOMTZ4))
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (view == true) value = current_settings.timeout_mtz_4[group]; //у змінну value поміщаємо значення витримки
          else value = edition_settings.timeout_mtz_4[group];
        }
        else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ4_N_VPERED : WITHOUTZNAM_INDEX_ML_TMOMTZ4_N_VPERED))
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (view == true) value = current_settings.timeout_mtz_4_n_vpered[group]; //у змінну value поміщаємо значення витримки
          else value = edition_settings.timeout_mtz_4_n_vpered[group];
        }
        else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ4_N_NAZAD : WITHOUTZNAM_INDEX_ML_TMOMTZ4_N_NAZAD))
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (view == true) value = current_settings.timeout_mtz_4_n_nazad[group]; //у змінну value поміщаємо значення витримки
          else value = edition_settings.timeout_mtz_4_n_nazad[group];
        }
        else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ4_PO_NAPRUZI : WITHOUTZNAM_INDEX_ML_TMOMTZ4_PO_NAPRUZI))
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (view == true) value = current_settings.timeout_mtz_4_po_napruzi[group]; //у змінну value поміщаємо значення витримки
          else value = edition_settings.timeout_mtz_4_po_napruzi[group];
        }
        else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ4_K : 100))
        {
          vaga = 10000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для mtz_4_k_znam
          if (view == true) value = current_settings.timeout_mtz_4_k_znam[group]; //у змінну value поміщаємо значення витримки
          else value = edition_settings.timeout_mtz_4_k_znam[group];
        }

        first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
      }
      else
      {
        //У парному номері рядку виводимо значення уставки
        for (unsigned int j = 0; j<MAX_COL_LCD; j++)
        {
          if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ1 : WITHOUTZNAM_INDEX_ML_TMOMTZ1))
          {
            if (
                ((j < COL_TMO_MTZ_1_BEGIN) ||  (j > COL_TMO_MTZ_1_END )) &&
                (j != (COL_TMO_MTZ_1_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_MTZ_1_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_MTZ_1_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_MTZ_1_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ1_N_VPERED : WITHOUTZNAM_INDEX_ML_TMOMTZ1_N_VPERED))
          {
            if (
                ((j < COL_TMO_MTZ_1_N_VPERED_BEGIN) ||  (j > COL_TMO_MTZ_1_N_VPERED_END )) &&
                (j != (COL_TMO_MTZ_1_N_VPERED_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_MTZ_1_N_VPERED_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_MTZ_1_N_VPERED_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_MTZ_1_N_VPERED_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ1_N_NAZAD : WITHOUTZNAM_INDEX_ML_TMOMTZ1_N_NAZAD))
          {
            if (
                ((j < COL_TMO_MTZ_1_N_NAZAD_BEGIN) ||  (j > COL_TMO_MTZ_1_N_NAZAD_END )) &&
                (j != (COL_TMO_MTZ_1_N_NAZAD_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_MTZ_1_N_NAZAD_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_MTZ_1_N_NAZAD_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_MTZ_1_N_NAZAD_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ1_PO_NAPRUZI : WITHOUTZNAM_INDEX_ML_TMOMTZ1_PO_NAPRUZI))
          {
            if (
                ((j < COL_TMO_MTZ_1_PO_NAPRUZI_BEGIN) ||  (j > COL_TMO_MTZ_1_PO_NAPRUZI_END )) &&
                (j != (COL_TMO_MTZ_1_PO_NAPRUZI_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_MTZ_1_PO_NAPRUZI_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_MTZ_1_PO_NAPRUZI_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_MTZ_1_PO_NAPRUZI_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ1_K : 100))
          {
            if (
                ((j < COL_TMO_MTZ_1_K_BEGIN) ||  (j > COL_TMO_MTZ_1_K_END ))
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_MTZ_1_K_COMMA )working_ekran[i][j] = ',';
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_MTZ_1_K_COMMA, view, 0);
          }

          else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ2 : WITHOUTZNAM_INDEX_ML_TMOMTZ2))
          {
            if (
                ((j < COL_TMO_MTZ_2_BEGIN) ||  (j > COL_TMO_MTZ_2_END )) &&
                (j != (COL_TMO_MTZ_2_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_MTZ_2_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_MTZ_2_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_MTZ_2_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ2_PR : WITHOUTZNAM_INDEX_ML_TMOMTZ2_PR))
          {
            if (
                ((j < COL_TMO_MTZ_2_PR_BEGIN) ||  (j > COL_TMO_MTZ_2_PR_END )) &&
                (j != (COL_TMO_MTZ_2_PR_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_MTZ_2_PR_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_MTZ_2_PR_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_MTZ_2_PR_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ2_N_VPERED : WITHOUTZNAM_INDEX_ML_TMOMTZ2_N_VPERED))
          {
            if (
                ((j < COL_TMO_MTZ_2_N_VPERED_BEGIN) ||  (j > COL_TMO_MTZ_2_N_VPERED_END )) &&
                (j != (COL_TMO_MTZ_2_N_VPERED_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_MTZ_2_N_VPERED_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_MTZ_2_N_VPERED_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_MTZ_2_N_VPERED_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ2_N_VPERED_PR : WITHOUTZNAM_INDEX_ML_TMOMTZ2_N_VPERED_PR))
          {
            if (
                ((j < COL_TMO_MTZ_2_N_VPERED_PR_BEGIN) ||  (j > COL_TMO_MTZ_2_N_VPERED_PR_END )) &&
                (j != (COL_TMO_MTZ_2_N_VPERED_PR_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_MTZ_2_N_VPERED_PR_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_MTZ_2_N_VPERED_PR_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_MTZ_2_N_VPERED_PR_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ2_N_NAZAD : WITHOUTZNAM_INDEX_ML_TMOMTZ2_N_NAZAD))
          {
            if (
                ((j < COL_TMO_MTZ_2_N_NAZAD_BEGIN) ||  (j > COL_TMO_MTZ_2_N_NAZAD_END )) &&
                (j != (COL_TMO_MTZ_2_N_NAZAD_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_MTZ_2_N_NAZAD_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_MTZ_2_N_NAZAD_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_MTZ_2_N_NAZAD_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ2_N_NAZAD_PR : WITHOUTZNAM_INDEX_ML_TMOMTZ2_N_NAZAD_PR))
          {
            if (
                ((j < COL_TMO_MTZ_2_N_NAZAD_PR_BEGIN) ||  (j > COL_TMO_MTZ_2_N_NAZAD_PR_END )) &&
                (j != (COL_TMO_MTZ_2_N_NAZAD_PR_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_MTZ_2_N_NAZAD_PR_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_MTZ_2_N_NAZAD_PR_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_MTZ_2_N_NAZAD_PR_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ2_PO_NAPRUZI : WITHOUTZNAM_INDEX_ML_TMOMTZ2_PO_NAPRUZI))
          {
            if (
                ((j < COL_TMO_MTZ_2_PO_NAPRUZI_BEGIN) ||  (j > COL_TMO_MTZ_2_PO_NAPRUZI_END )) &&
                (j != (COL_TMO_MTZ_2_PO_NAPRUZI_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_MTZ_2_PO_NAPRUZI_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_MTZ_2_PO_NAPRUZI_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_MTZ_2_PO_NAPRUZI_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ2_PO_NAPRUZI_PR : WITHOUTZNAM_INDEX_ML_TMOMTZ2_PO_NAPRUZI_PR))
          {
            if (
                ((j < COL_TMO_MTZ_2_PO_NAPRUZI_PR_BEGIN) ||  (j > COL_TMO_MTZ_2_PO_NAPRUZI_PR_END )) &&
                (j != (COL_TMO_MTZ_2_PO_NAPRUZI_PR_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_MTZ_2_PO_NAPRUZI_PR_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_MTZ_2_PO_NAPRUZI_PR_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_MTZ_2_PO_NAPRUZI_PR_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ2_VVID_PR : WITHOUTZNAM_INDEX_ML_TMOMTZ2_VVID_PR))
          {
            if (
                ((j < COL_TMO_MTZ_2_VVID_PR_BEGIN) ||  (j > COL_TMO_MTZ_2_VVID_PR_END )) &&
                (j != (COL_TMO_MTZ_2_VVID_PR_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_MTZ_2_VVID_PR_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_MTZ_2_VVID_PR_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_MTZ_2_VVID_PR_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ2_K : 100))
          {
            if (
                ((j < COL_TMO_MTZ_2_K_BEGIN) ||  (j > COL_TMO_MTZ_2_K_END ))
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_MTZ_2_K_COMMA )working_ekran[i][j] = ',';
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_MTZ_2_K_COMMA, view, 0);
          }

          else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ3 : WITHOUTZNAM_INDEX_ML_TMOMTZ3))
          {
            if (
                ((j < COL_TMO_MTZ_3_BEGIN) ||  (j > COL_TMO_MTZ_3_END )) &&
                (j != (COL_TMO_MTZ_3_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_MTZ_3_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_MTZ_3_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_MTZ_3_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ3_N_VPERED : WITHOUTZNAM_INDEX_ML_TMOMTZ3_N_VPERED))
          {
            if (
                ((j < COL_TMO_MTZ_3_N_VPERED_BEGIN) ||  (j > COL_TMO_MTZ_3_N_VPERED_END )) &&
                (j != (COL_TMO_MTZ_3_N_VPERED_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_MTZ_3_N_VPERED_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_MTZ_3_N_VPERED_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_MTZ_3_N_VPERED_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ3_N_NAZAD : WITHOUTZNAM_INDEX_ML_TMOMTZ3_N_NAZAD))
          {
            if (
                ((j < COL_TMO_MTZ_3_N_NAZAD_BEGIN) ||  (j > COL_TMO_MTZ_3_N_NAZAD_END )) &&
                (j != (COL_TMO_MTZ_3_N_NAZAD_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_MTZ_3_N_NAZAD_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_MTZ_3_N_NAZAD_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_MTZ_3_N_NAZAD_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ3_PO_NAPRUZI : WITHOUTZNAM_INDEX_ML_TMOMTZ3_PO_NAPRUZI))
          {
            if (
                ((j < COL_TMO_MTZ_3_PO_NAPRUZI_BEGIN) ||  (j > COL_TMO_MTZ_3_PO_NAPRUZI_END )) &&
                (j != (COL_TMO_MTZ_3_PO_NAPRUZI_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_MTZ_3_PO_NAPRUZI_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_MTZ_3_PO_NAPRUZI_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_MTZ_3_PO_NAPRUZI_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ3_K : 100))
          {
            if (
                ((j < COL_TMO_MTZ_3_K_BEGIN) ||  (j > COL_TMO_MTZ_3_K_END ))
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_MTZ_3_K_COMMA )working_ekran[i][j] = ',';
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_MTZ_3_K_COMMA, view, 0);
          }

          else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ4 : WITHOUTZNAM_INDEX_ML_TMOMTZ4))
          {
            if (
                ((j < COL_TMO_MTZ_4_BEGIN) ||  (j > COL_TMO_MTZ_4_END )) &&
                (j != (COL_TMO_MTZ_4_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_MTZ_4_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_MTZ_4_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_MTZ_4_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ4_N_VPERED : WITHOUTZNAM_INDEX_ML_TMOMTZ4_N_VPERED))
          {
            if (
                ((j < COL_TMO_MTZ_4_N_VPERED_BEGIN) ||  (j > COL_TMO_MTZ_4_N_VPERED_END )) &&
                (j != (COL_TMO_MTZ_4_N_VPERED_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_MTZ_4_N_VPERED_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_MTZ_4_N_VPERED_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_MTZ_4_N_VPERED_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ4_N_NAZAD : WITHOUTZNAM_INDEX_ML_TMOMTZ4_N_NAZAD))
          {
            if (
                ((j < COL_TMO_MTZ_4_N_NAZAD_BEGIN) ||  (j > COL_TMO_MTZ_4_N_NAZAD_END )) &&
                (j != (COL_TMO_MTZ_4_N_NAZAD_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_MTZ_4_N_NAZAD_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_MTZ_4_N_NAZAD_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_MTZ_4_N_NAZAD_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ4_PO_NAPRUZI : WITHOUTZNAM_INDEX_ML_TMOMTZ4_PO_NAPRUZI))
          {
            if (
                ((j < COL_TMO_MTZ_4_PO_NAPRUZI_BEGIN) ||  (j > COL_TMO_MTZ_4_PO_NAPRUZI_END )) &&
                (j != (COL_TMO_MTZ_4_PO_NAPRUZI_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_MTZ_4_PO_NAPRUZI_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_MTZ_4_PO_NAPRUZI_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_MTZ_4_PO_NAPRUZI_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ4_K : 100))
          {
            if (
                ((j < COL_TMO_MTZ_4_K_BEGIN) ||  (j > COL_TMO_MTZ_4_K_END ))
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_MTZ_4_K_COMMA )working_ekran[i][j] = ',';
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_MTZ_4_K_COMMA, view, 0);
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
    if (current_ekran.index_position == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ1 : WITHOUTZNAM_INDEX_ML_TMOMTZ1)) 
    {
      current_ekran.position_cursor_x = COL_TMO_MTZ_1_BEGIN;
      last_position_cursor_x = COL_TMO_MTZ_1_END;
    }
    else if (current_ekran.index_position == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ1_N_VPERED : WITHOUTZNAM_INDEX_ML_TMOMTZ1_N_VPERED)) 
    {
      current_ekran.position_cursor_x = COL_TMO_MTZ_1_N_VPERED_BEGIN;
      last_position_cursor_x = COL_TMO_MTZ_1_N_VPERED_END;
    }
    else if (current_ekran.index_position == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ1_N_NAZAD : WITHOUTZNAM_INDEX_ML_TMOMTZ1_N_NAZAD)) 
    {
      current_ekran.position_cursor_x = COL_TMO_MTZ_1_N_NAZAD_BEGIN;
      last_position_cursor_x = COL_TMO_MTZ_1_N_NAZAD_END;
    }
    else if (current_ekran.index_position == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ1_PO_NAPRUZI : WITHOUTZNAM_INDEX_ML_TMOMTZ1_PO_NAPRUZI)) 
    {
      current_ekran.position_cursor_x = COL_TMO_MTZ_1_PO_NAPRUZI_BEGIN;
      last_position_cursor_x = COL_TMO_MTZ_1_PO_NAPRUZI_END;
    }
    else if (current_ekran.index_position == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ1_K : 100)) 
    {
      current_ekran.position_cursor_x = COL_TMO_MTZ_1_K_BEGIN;
      last_position_cursor_x = COL_TMO_MTZ_1_K_END;
    }

    else if (current_ekran.index_position == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ2 : WITHOUTZNAM_INDEX_ML_TMOMTZ2)) 
    {
      current_ekran.position_cursor_x = COL_TMO_MTZ_2_BEGIN;
      last_position_cursor_x = COL_TMO_MTZ_2_END;
    }
    else if (current_ekran.index_position == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ2_PR : WITHOUTZNAM_INDEX_ML_TMOMTZ2_PR)) 
    {
      current_ekran.position_cursor_x = COL_TMO_MTZ_2_PR_BEGIN;
      last_position_cursor_x = COL_TMO_MTZ_2_PR_END;
    }
    else if (current_ekran.index_position == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ2_N_VPERED : WITHOUTZNAM_INDEX_ML_TMOMTZ2_N_VPERED)) 
    {
      current_ekran.position_cursor_x = COL_TMO_MTZ_2_N_VPERED_BEGIN;
      last_position_cursor_x = COL_TMO_MTZ_2_N_VPERED_END;
    }
    else if (current_ekran.index_position == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ2_N_VPERED_PR : WITHOUTZNAM_INDEX_ML_TMOMTZ2_N_VPERED_PR)) 
    {
      current_ekran.position_cursor_x = COL_TMO_MTZ_2_N_VPERED_PR_BEGIN;
      last_position_cursor_x = COL_TMO_MTZ_2_N_VPERED_PR_END;
    }
    else if (current_ekran.index_position == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ2_N_NAZAD : WITHOUTZNAM_INDEX_ML_TMOMTZ2_N_NAZAD)) 
    {
      current_ekran.position_cursor_x = COL_TMO_MTZ_2_N_NAZAD_BEGIN;
      last_position_cursor_x = COL_TMO_MTZ_2_N_NAZAD_END;
    }
    else if (current_ekran.index_position == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ2_N_NAZAD_PR : WITHOUTZNAM_INDEX_ML_TMOMTZ2_N_NAZAD_PR)) 
    {
      current_ekran.position_cursor_x = COL_TMO_MTZ_2_N_NAZAD_PR_BEGIN;
      last_position_cursor_x = COL_TMO_MTZ_2_N_NAZAD_PR_END;
    }
    else if (current_ekran.index_position == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ2_PO_NAPRUZI : WITHOUTZNAM_INDEX_ML_TMOMTZ2_PO_NAPRUZI)) 
    {
      current_ekran.position_cursor_x = COL_TMO_MTZ_2_PO_NAPRUZI_BEGIN;
      last_position_cursor_x = COL_TMO_MTZ_2_PO_NAPRUZI_END;
    }
    else if (current_ekran.index_position == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ2_PO_NAPRUZI_PR : WITHOUTZNAM_INDEX_ML_TMOMTZ2_PO_NAPRUZI_PR)) 
    {
      current_ekran.position_cursor_x = COL_TMO_MTZ_2_PO_NAPRUZI_PR_BEGIN;
      last_position_cursor_x = COL_TMO_MTZ_2_PO_NAPRUZI_PR_END;
    }
    else if (current_ekran.index_position == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ2_VVID_PR : WITHOUTZNAM_INDEX_ML_TMOMTZ2_VVID_PR)) 
    {
      current_ekran.position_cursor_x = COL_TMO_MTZ_2_VVID_PR_BEGIN;
      last_position_cursor_x = COL_TMO_MTZ_2_VVID_PR_END;
    }
    else if (current_ekran.index_position == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ2_K : 100)) 
    {
      current_ekran.position_cursor_x = COL_TMO_MTZ_2_K_BEGIN;
      last_position_cursor_x = COL_TMO_MTZ_2_K_END;
    }

    else if (current_ekran.index_position == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ3 : WITHOUTZNAM_INDEX_ML_TMOMTZ3)) 
    {
      current_ekran.position_cursor_x = COL_TMO_MTZ_3_BEGIN;
      last_position_cursor_x = COL_TMO_MTZ_3_END;
    }
    else if (current_ekran.index_position == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ3_N_VPERED : WITHOUTZNAM_INDEX_ML_TMOMTZ3_N_VPERED)) 
    {
      current_ekran.position_cursor_x = COL_TMO_MTZ_3_N_VPERED_BEGIN;
      last_position_cursor_x = COL_TMO_MTZ_3_N_VPERED_END;
    }
    else if (current_ekran.index_position == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ3_N_NAZAD : WITHOUTZNAM_INDEX_ML_TMOMTZ3_N_NAZAD)) 
    {
      current_ekran.position_cursor_x = COL_TMO_MTZ_3_N_NAZAD_BEGIN;
      last_position_cursor_x = COL_TMO_MTZ_3_N_NAZAD_END;
    }
    else if (current_ekran.index_position == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ3_PO_NAPRUZI : WITHOUTZNAM_INDEX_ML_TMOMTZ3_PO_NAPRUZI)) 
    {
      current_ekran.position_cursor_x = COL_TMO_MTZ_3_PO_NAPRUZI_BEGIN;
      last_position_cursor_x = COL_TMO_MTZ_3_PO_NAPRUZI_END;
    }
    else if (current_ekran.index_position == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ3_K : 100)) 
    {
      current_ekran.position_cursor_x = COL_TMO_MTZ_3_K_BEGIN;
      last_position_cursor_x = COL_TMO_MTZ_3_K_END;
    }

    else if (current_ekran.index_position == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ4 : WITHOUTZNAM_INDEX_ML_TMOMTZ4)) 
    {
      current_ekran.position_cursor_x = COL_TMO_MTZ_4_BEGIN;
      last_position_cursor_x = COL_TMO_MTZ_4_END;
    }
    else if (current_ekran.index_position == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ4_N_VPERED : WITHOUTZNAM_INDEX_ML_TMOMTZ4_N_VPERED)) 
    {
      current_ekran.position_cursor_x = COL_TMO_MTZ_4_N_VPERED_BEGIN;
      last_position_cursor_x = COL_TMO_MTZ_4_N_VPERED_END;
    }
    else if (current_ekran.index_position == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ4_N_NAZAD : WITHOUTZNAM_INDEX_ML_TMOMTZ4_N_NAZAD)) 
    {
      current_ekran.position_cursor_x = COL_TMO_MTZ_4_N_NAZAD_BEGIN;
      last_position_cursor_x = COL_TMO_MTZ_4_N_NAZAD_END;
    }
    else if (current_ekran.index_position == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ4_PO_NAPRUZI : WITHOUTZNAM_INDEX_ML_TMOMTZ4_PO_NAPRUZI)) 
    {
      current_ekran.position_cursor_x = COL_TMO_MTZ_4_PO_NAPRUZI_BEGIN;
      last_position_cursor_x = COL_TMO_MTZ_4_PO_NAPRUZI_END;
    }
    else if (current_ekran.index_position == (isZNam? WITHZNAM_INDEX_ML_TMOMTZ4_K : 100)) 
    {
      current_ekran.position_cursor_x = COL_TMO_MTZ_4_K_BEGIN;
      last_position_cursor_x = COL_TMO_MTZ_4_K_END;
    }

    //Підтягуємо курсор до першого символу
    while (((working_ekran[current_ekran.position_cursor_y][current_ekran.position_cursor_x + 1]) == ' ') && 
           (current_ekran.position_cursor_x < (last_position_cursor_x -1))) current_ekran.position_cursor_x++;

    //Курсор ставимо так, щоб він був перед числом
    if (((working_ekran[current_ekran.position_cursor_y][current_ekran.position_cursor_x]) != ' ') && 
        (current_ekran.position_cursor_x > 0)) current_ekran.position_cursor_x--;
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

/*****************************************************/
//Формуємо екран відображення значення управлінської інформації для МТЗ
/*****************************************************/
void make_ekran_control_mtz()
{
  static const unsigned char name_string_withznam[MAX_NAMBER_LANGUAGE][WITHZNAM_MAX_ROW_FOR_CONTROL_MTZ][MAX_COL_LCD] = 
  {
    {
      "     МТЗ 1      ",
      "   Тип МТЗ 1    ",
      "   МТЗН 1 Пр.   ",
      "  МТЗН 1 Обр.   ",
      "  МТЗ 1 (ЗНам)  ",
      "Вид МТЗ 1 (ЗНам)",
      "     МТЗ 2      ",
      "   Тип МТЗ 2    ",
      "   МТЗН 2 Пр.   ",
      "  МТЗН 2 Обр.   ",
      " Ускорение МТЗ 2",
      "Ускоренная МТЗ 2",
      "  МТЗ 2 (ЗНам)  ",
      "Вид МТЗ 2 (ЗНам)",
      "     МТЗ 3      ",
      "   Тип МТЗ 3    ",
      "   МТЗН 3 Пр.   ",
      "  МТЗН 3 Обр.   ",
      "  МТЗ 3 (ЗНам)  ",
      "Вид МТЗ 3 (ЗНам)",
      "     МТЗ 4      ",
      "   Тип МТЗ 4    ",
      "   МТЗН 4 Пр.   ",
      "  МТЗН 4 Обр.   ",
      "  МТЗ 4 (ЗНам)  ",
      "Вид МТЗ 4 (ЗНам)",
      "    НЦН-МТЗ     "
    },
    {
      "     МСЗ 1      ",
      "   Тип МСЗ 1    ",
      "   МСЗС 1 Пр.   ",
      "   МСЗС 1 Зв.   ",
      "  МCЗ 1 (ЗНам)  ",
      "Вид МCЗ 1 (ЗНам)",
      "     МСЗ 2      ",
      "   Тип МСЗ 2    ",
      " МСЗС 2 Прямий  ",
      "  МСЗС 2 Звор.  ",
      "Прискорення МСЗ2",
      "Прискорений МСЗ2",
      "  МCЗ 2 (ЗНам)  ",
      "Вид МCЗ 2 (ЗНам)",
      "     МСЗ 3      ",
      "   Тип МСЗ 3    ",
      " МСЗС 3 Прямий  ",
      "  МСЗС 3 Звор.  ",
      "  МCЗ 3 (ЗНам)  ",
      "Вид МCЗ 3 (ЗНам)",
      "     МСЗ 4      ",
      "   Тип МСЗ 4    ",
      " МСЗС 4 Прямий  ",
      "  МСЗС 4 Звор.  ",
      "  МCЗ 4 (ЗНам)  ",
      "Вид МCЗ 4 (ЗНам)",
      "    НКН-МСЗ     "
    },
    {
      "     OCP 1      ",
      "   OCP 1 Type   ",
      "   DOCP 1 Fw    ",
      "   DOCP 1 Bw    ",
      "  OCP 1 (InrP)  ",
      "OCP1 Mode (InrP)",
      "     OCP 2      ",
      "   OCP 2 Type   ",
      "   DOCP 2 Fw    ",
      "   DOCP 2 Bw    ",
      " OCP 2 Acc Ena  ",
      "  OCP 2 Acc C   ",
      "  OCP 2 (InrP)  ",
      "OCP2 Mode (InrP)",
      "     OCP 3      ",
      "   OCP 3 Type   ",
      "   DOCP 3 Fw    ",
      "   DOCP 3 Bw    ",
      "  OCP 3 (InrP)  ",
      "OCP3 Mode (InrP)",
      "     OCP 4      ",
      "   OCP 4 Type   ",
      "   DOCP 4 Fw    ",
      "   DOCP 4 Bw    ",
      "  OCP 4 (InrP)  ",
      "OCP4 Mode (InrP)",
      "    OCP VCM     "
    },
    {
      "     МТЗ 1      ",
      "   Тип МТЗ 1    ",
      "   МТЗН 1 Пр.   ",
      "  МТЗН 1 Обр.   ",
      "  МТЗ 1 (ЗНам)  ",
      "Вид МТЗ 1 (ЗНам)",
      "     МТЗ 2      ",
      "   Тип МТЗ 2    ",
      "   МТЗН 2 Пр.   ",
      "  МТЗН 2 Обр.   ",
      " Ускорение МТЗ 2",
      "Ускоренная МТЗ 2",
      "  МТЗ 2 (ЗНам)  ",
      "Вид МТЗ 2 (ЗНам)",
      "     МТЗ 3      ",
      "   Тип МТЗ 3    ",
      "   МТЗН 3 Пр.   ",
      "  МТЗН 3 Обр.   ",
      "  МТЗ 3 (ЗНам)  ",
      "Вид МТЗ 3 (ЗНам)",
      "     МТЗ 4      ",
      "   Тип МТЗ 4    ",
      "   МТЗН 4 Пр.   ",
      "  МТЗН 4 Обр.   ",
      "  МТЗ 4 (ЗНам)  ",
      "Вид МТЗ 4 (ЗНам)",
      "    НЦН-МТЗ     "
    }
  };
  static const unsigned char name_string_withoutznam[MAX_NAMBER_LANGUAGE][WITHOUTZNAM_MAX_ROW_FOR_CONTROL_MTZ][MAX_COL_LCD] = 
  {
    {
      "     МТЗ 1      ",
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
      "    НЦН-МТЗ     "
    },
    {
      "     МСЗ 1      ",
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
      "    НКН-МСЗ     "
    },
    {
      "     OCP 1      ",
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
      "    OCP VCM     "
    },
    {
      "     МТЗ 1      ",
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
      "    НЦН-МТЗ     "
    }
  };

  unsigned int isZNam = current_settings.configuration & (1 << ZNAM_BIT_CONFIGURATION);
  int index_language = index_language_in_array(current_settings.language);
  
  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  
  //Множення на два величини position_temp потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
  index_of_ekran = ((position_temp<<1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    unsigned int index_of_ekran_tmp = index_of_ekran >> 1;
    if (index_of_ekran_tmp < (unsigned int)(isZNam? WITHZNAM_MAX_ROW_FOR_CONTROL_MTZ : WITHOUTZNAM_MAX_ROW_FOR_CONTROL_MTZ))
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        if(isZNam)  for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string_withznam[index_language][index_of_ekran_tmp][j];
        else        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string_withoutznam[index_language][index_of_ekran_tmp][j];
      }
      else
      {
        //У парному номері рядку виводимо значення уставки
        unsigned int index_ctr = index_of_ekran_tmp;

        __SETTINGS *point;
          if(current_ekran.edition == 0) point = &current_settings;
          else point = &edition_settings;

        if (
            (index_ctr == (isZNam? WITHZNAM_INDEX_ML_CTRMTZ_1_TYPE : WITHOUTZNAM_INDEX_ML_CTRMTZ_1_TYPE)) ||
            (index_ctr == (isZNam? WITHZNAM_INDEX_ML_CTRMTZ_2_TYPE : WITHOUTZNAM_INDEX_ML_CTRMTZ_2_TYPE)) ||  
            (index_ctr == (isZNam? WITHZNAM_INDEX_ML_CTRMTZ_3_TYPE : WITHOUTZNAM_INDEX_ML_CTRMTZ_3_TYPE)) || 
            (index_ctr == (isZNam? WITHZNAM_INDEX_ML_CTRMTZ_4_TYPE : WITHOUTZNAM_INDEX_ML_CTRMTZ_4_TYPE))  
           )   
        {
          int value = 0;
          if (index_ctr == (isZNam? WITHZNAM_INDEX_ML_CTRMTZ_1_TYPE : WITHOUTZNAM_INDEX_ML_CTRMTZ_1_TYPE)) value = point->type_mtz1;
          else if (index_ctr == (isZNam? WITHZNAM_INDEX_ML_CTRMTZ_2_TYPE : WITHOUTZNAM_INDEX_ML_CTRMTZ_2_TYPE)) value = point->type_mtz2;
          else if (index_ctr == (isZNam? WITHZNAM_INDEX_ML_CTRMTZ_3_TYPE : WITHOUTZNAM_INDEX_ML_CTRMTZ_3_TYPE)) value = point->type_mtz3;
          else if (index_ctr == (isZNam? WITHZNAM_INDEX_ML_CTRMTZ_4_TYPE : WITHOUTZNAM_INDEX_ML_CTRMTZ_4_TYPE)) value = point->type_mtz4;
          else total_error_sw_fixed();

          static const unsigned char information[MAX_NAMBER_LANGUAGE][8][MAX_COL_LCD] = 
          {
            {"    Простая     ", "  Направленная  ", " С пуском по U  ", " Завис.(Тип A)  ", " Завис.(Тип B)  ", " Завис.(Тип C)  ", " Завис.(РТ-80)  ", " Завис.(РТВ-1)  "},
            {"    Простий     ", "   Спрямований  ", " Із пуском по U ", "Залежний (Тип A)", "Залежний (Тип B)", "Залежний (Тип C)", "Залежний (РТ-80)", "Залежний (РТВ-1)"},
            {"    Non Dir     ", "     Direct     ", "    V Start     ", "  Dep (Type A)  ", "  Dep (Type B)  ", "  Dep (Type C)  ", "  Dep (Type 1)  ", "  Dep (Type 2)  "},
            {"    Простая     ", "  Направленная  ", " С пуском по U  ", " Завис.(Тип A)  ", " Завис.(Тип B)  ", " Завис.(Тип C)  ", " Завис.(РТ-80)  ", " Завис.(РТВ-1)  "}
          };
          static const unsigned int cursor_x[MAX_NAMBER_LANGUAGE][8] = 
          {
            {3, 1, 0, 0, 0, 0, 0, 0},
            {3, 2, 0, 0, 0, 0, 0, 0},
            {3, 4, 3, 1, 1, 1, 1, 1},
            {3, 1, 0, 0, 0, 0, 0, 0}
          };
          
          for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = information[index_language][value][j];
          if (position_temp == index_of_ekran_tmp) current_ekran.position_cursor_x = cursor_x[index_language][value];
        }
        else if (
                 (index_ctr == (isZNam? WITHZNAM_INDEX_ML_CTRMTZ_1_ZNAM_TYPE : 100 )) ||
                 (index_ctr == (isZNam? WITHZNAM_INDEX_ML_CTRMTZ_2_ZNAM_TYPE : 100 )) ||
                 (index_ctr == (isZNam? WITHZNAM_INDEX_ML_CTRMTZ_3_ZNAM_TYPE : 100 )) ||
                 (index_ctr == (isZNam? WITHZNAM_INDEX_ML_CTRMTZ_4_ZNAM_TYPE : 100 ))
                )   
        {

          static unsigned char const information_3[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] = 
          {
            {"  Загрубление   ", "   Блокировка   "},
            {"  Загрублення   ", "   Блокування   "},
            {"   Coarsening   ", "    Blocking    "},
            {"  Загрубление   ", "   Блокировка   "}
          };
          static const unsigned int cursor_x_3[MAX_NAMBER_LANGUAGE][2] = 
          {
           {1, 2},
           {1, 2},
           {2, 3},
           {1, 2}
          };

          unsigned int temp_data = point->control_mtz;
          unsigned int n_bit = 0;
          
          if (index_ctr == WITHZNAM_INDEX_ML_CTRMTZ_1_ZNAM_TYPE ) n_bit = N_BIT_CTRMTZ_1_ZNAM_TYPE;
          else if (index_ctr == WITHZNAM_INDEX_ML_CTRMTZ_2_ZNAM_TYPE ) n_bit = N_BIT_CTRMTZ_2_ZNAM_TYPE;
          else if (index_ctr == WITHZNAM_INDEX_ML_CTRMTZ_3_ZNAM_TYPE ) n_bit = N_BIT_CTRMTZ_3_ZNAM_TYPE;
          else if (index_ctr == WITHZNAM_INDEX_ML_CTRMTZ_4_ZNAM_TYPE ) n_bit = N_BIT_CTRMTZ_4_ZNAM_TYPE;

          unsigned int value = (temp_data >> n_bit) & 0x1;          

          for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = information_3[index_language][value][j];
          if (position_temp == index_of_ekran_tmp) current_ekran.position_cursor_x = cursor_x_3[index_language][value];
        }
        else
        {
          unsigned int temp_data = point->control_mtz;
          unsigned int n_bit = 0;
          
          if(isZNam)
          {
            //Виділяємо номер біту
            if      (index_ctr == WITHZNAM_INDEX_ML_CTRMTZ_1             ) n_bit = N_BIT_CTRMTZ_1;
            else if (index_ctr == WITHZNAM_INDEX_ML_CTRMTZ_1_VPERED      ) n_bit = N_BIT_CTRMTZ_1_VPERED;
            else if (index_ctr == WITHZNAM_INDEX_ML_CTRMTZ_1_NAZAD       ) n_bit = N_BIT_CTRMTZ_1_NAZAD;
            else if (index_ctr == WITHZNAM_INDEX_ML_CTRMTZ_1_ZNAM        ) n_bit = N_BIT_CTRMTZ_1_ZNAM;
            else if (index_ctr == WITHZNAM_INDEX_ML_CTRMTZ_2             ) n_bit = N_BIT_CTRMTZ_2;
            else if (index_ctr == WITHZNAM_INDEX_ML_CTRMTZ_2_VPERED      ) n_bit = N_BIT_CTRMTZ_2_VPERED;
            else if (index_ctr == WITHZNAM_INDEX_ML_CTRMTZ_2_NAZAD       ) n_bit = N_BIT_CTRMTZ_2_NAZAD;
            else if (index_ctr == WITHZNAM_INDEX_ML_CTRMTZ_2_PRYSKORENNJA) n_bit = N_BIT_CTRMTZ_2_PRYSKORENNJA;
            else if (index_ctr == WITHZNAM_INDEX_ML_CTRMTZ_2_PRYSKORENA  ) n_bit = N_BIT_CTRMTZ_2_PRYSKORENA;
            else if (index_ctr == WITHZNAM_INDEX_ML_CTRMTZ_2_ZNAM        ) n_bit = N_BIT_CTRMTZ_2_ZNAM;
            else if (index_ctr == WITHZNAM_INDEX_ML_CTRMTZ_3             ) n_bit = N_BIT_CTRMTZ_3;
            else if (index_ctr == WITHZNAM_INDEX_ML_CTRMTZ_3_VPERED      ) n_bit = N_BIT_CTRMTZ_3_VPERED;
            else if (index_ctr == WITHZNAM_INDEX_ML_CTRMTZ_3_NAZAD       ) n_bit = N_BIT_CTRMTZ_3_NAZAD;
            else if (index_ctr == WITHZNAM_INDEX_ML_CTRMTZ_3_ZNAM        ) n_bit = N_BIT_CTRMTZ_3_ZNAM;
            else if (index_ctr == WITHZNAM_INDEX_ML_CTRMTZ_4             ) n_bit = N_BIT_CTRMTZ_4;
            else if (index_ctr == WITHZNAM_INDEX_ML_CTRMTZ_4_VPERED      ) n_bit = N_BIT_CTRMTZ_4_VPERED;
            else if (index_ctr == WITHZNAM_INDEX_ML_CTRMTZ_4_NAZAD       ) n_bit = N_BIT_CTRMTZ_4_NAZAD;
            else if (index_ctr == WITHZNAM_INDEX_ML_CTRMTZ_4_ZNAM        ) n_bit = N_BIT_CTRMTZ_4_ZNAM;
            else if (index_ctr == WITHZNAM_INDEX_ML_CTRMTZ_NESPR_KIL_NAPR) n_bit = N_BIT_CTRMTZ_NESPR_KIL_NAPR;
          }//if
          else
          {
            //Виділяємо номер біту
            if      (index_ctr == WITHOUTZNAM_INDEX_ML_CTRMTZ_1             ) n_bit = N_BIT_CTRMTZ_1;
            else if (index_ctr == WITHOUTZNAM_INDEX_ML_CTRMTZ_1_VPERED      ) n_bit = N_BIT_CTRMTZ_1_VPERED;
            else if (index_ctr == WITHOUTZNAM_INDEX_ML_CTRMTZ_1_NAZAD       ) n_bit = N_BIT_CTRMTZ_1_NAZAD;
//            else if (index_ctr == WITHOUTZNAM_INDEX_ML_CTRMTZ_1_ZNAM        ) n_bit = N_BIT_CTRMTZ_1_ZNAM;
            else if (index_ctr == WITHOUTZNAM_INDEX_ML_CTRMTZ_2             ) n_bit = N_BIT_CTRMTZ_2;
            else if (index_ctr == WITHOUTZNAM_INDEX_ML_CTRMTZ_2_VPERED      ) n_bit = N_BIT_CTRMTZ_2_VPERED;
            else if (index_ctr == WITHOUTZNAM_INDEX_ML_CTRMTZ_2_NAZAD       ) n_bit = N_BIT_CTRMTZ_2_NAZAD;
            else if (index_ctr == WITHOUTZNAM_INDEX_ML_CTRMTZ_2_PRYSKORENNJA) n_bit = N_BIT_CTRMTZ_2_PRYSKORENNJA;
            else if (index_ctr == WITHOUTZNAM_INDEX_ML_CTRMTZ_2_PRYSKORENA  ) n_bit = N_BIT_CTRMTZ_2_PRYSKORENA;
//            else if (index_ctr == WITHOUTZNAM_INDEX_ML_CTRMTZ_2_ZNAM        ) n_bit = N_BIT_CTRMTZ_2_ZNAM;
            else if (index_ctr == WITHOUTZNAM_INDEX_ML_CTRMTZ_3             ) n_bit = N_BIT_CTRMTZ_3;
            else if (index_ctr == WITHOUTZNAM_INDEX_ML_CTRMTZ_3_VPERED      ) n_bit = N_BIT_CTRMTZ_3_VPERED;
            else if (index_ctr == WITHOUTZNAM_INDEX_ML_CTRMTZ_3_NAZAD       ) n_bit = N_BIT_CTRMTZ_3_NAZAD;
//            else if (index_ctr == WITHOUTZNAM_INDEX_ML_CTRMTZ_3_ZNAM        ) n_bit = N_BIT_CTRMTZ_3_ZNAM;
            else if (index_ctr == WITHOUTZNAM_INDEX_ML_CTRMTZ_4             ) n_bit = N_BIT_CTRMTZ_4;
            else if (index_ctr == WITHOUTZNAM_INDEX_ML_CTRMTZ_4_VPERED      ) n_bit = N_BIT_CTRMTZ_4_VPERED;
            else if (index_ctr == WITHOUTZNAM_INDEX_ML_CTRMTZ_4_NAZAD       ) n_bit = N_BIT_CTRMTZ_4_NAZAD;
//            else if (index_ctr == WITHOUTZNAM_INDEX_ML_CTRMTZ_4_ZNAM        ) n_bit = N_BIT_CTRMTZ_4_ZNAM;
            else if (index_ctr == WITHOUTZNAM_INDEX_ML_CTRMTZ_NESPR_KIL_NAPR) n_bit = N_BIT_CTRMTZ_NESPR_KIL_NAPR;
          }//if
          
          for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = information_off_on[index_language][(temp_data >> n_bit) & 0x1][j];
          if (position_temp == index_of_ekran_tmp) current_ekran.position_cursor_x = cursor_x_off_on[index_language][(temp_data >> n_bit) & 0x1];
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
//
/*****************************************************/
/*****************************************************/
