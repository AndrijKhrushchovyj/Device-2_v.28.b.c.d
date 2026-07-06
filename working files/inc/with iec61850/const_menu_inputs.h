#ifndef __CONSTANTS_INPUTS__
#define __CONSTANTS_INPUTS__

#define EKRAN_LIST_INPUTS_FOR_RANGUVANNJA (EKRAN_LN_FOR_IEC61850_RANG + 1)

#define EKRAN_RANGUVANNJA_INPUT_1 (EKRAN_TRANSFORMATOR_INFO + 1)
#define EKRAN_RANGUVANNJA_INPUT_2 (EKRAN_RANGUVANNJA_INPUT_1 + 1)
#define EKRAN_RANGUVANNJA_INPUT_3 (EKRAN_RANGUVANNJA_INPUT_2 + 1)
#define EKRAN_RANGUVANNJA_INPUT_4 (EKRAN_RANGUVANNJA_INPUT_3 + 1)
#define EKRAN_RANGUVANNJA_INPUT_5 (EKRAN_RANGUVANNJA_INPUT_4 + 1)
#define EKRAN_RANGUVANNJA_INPUT_6 (EKRAN_RANGUVANNJA_INPUT_5 + 1)
#define EKRAN_RANGUVANNJA_INPUT_7 (EKRAN_RANGUVANNJA_INPUT_6 + 1)
#define EKRAN_RANGUVANNJA_INPUT_8 (EKRAN_RANGUVANNJA_INPUT_7 + 1)

#if (                              \
  (MODYFIKACIA_VERSII_PZ == 8) ||  \
  (MODYFIKACIA_VERSII_PZ == 10) || \
  (MODYFIKACIA_VERSII_PZ == 11) || \
  (MODYFIKACIA_VERSII_PZ == 13) || \
  (MODYFIKACIA_VERSII_PZ == 15) || \
  (MODYFIKACIA_VERSII_PZ == 16) || \
  (MODYFIKACIA_VERSII_PZ == 17) || \
  (MODYFIKACIA_VERSII_PZ == 18) || \
  (MODYFIKACIA_VERSII_PZ == 33))

#define EKRAN_RANGUVANNJA_INPUT_9 (EKRAN_RANGUVANNJA_INPUT_8 + 1)
#define EKRAN_RANGUVANNJA_INPUT_10 (EKRAN_RANGUVANNJA_INPUT_9 + 1)
#define EKRAN_RANGUVANNJA_INPUT_11 (EKRAN_RANGUVANNJA_INPUT_10 + 1)
#define EKRAN_RANGUVANNJA_INPUT_12 (EKRAN_RANGUVANNJA_INPUT_11 + 1)

#if (                              \
  (MODYFIKACIA_VERSII_PZ == 8) ||  \
  (MODYFIKACIA_VERSII_PZ == 10) || \
  (MODYFIKACIA_VERSII_PZ == 11) || \
  (MODYFIKACIA_VERSII_PZ == 13) || \
  (MODYFIKACIA_VERSII_PZ == 15) || \
  (MODYFIKACIA_VERSII_PZ == 17) || \
  (MODYFIKACIA_VERSII_PZ == 18) || \
  (MODYFIKACIA_VERSII_PZ == 33))

#define EKRAN_RANGUVANNJA_INPUT_13 (EKRAN_RANGUVANNJA_INPUT_12 + 1)

#if (                              \
  (MODYFIKACIA_VERSII_PZ == 8) ||  \
  (MODYFIKACIA_VERSII_PZ == 10) || \
  (MODYFIKACIA_VERSII_PZ == 11) || \
  (MODYFIKACIA_VERSII_PZ == 13) || \
  (MODYFIKACIA_VERSII_PZ == 15) || \
  (MODYFIKACIA_VERSII_PZ == 18) || \
  (MODYFIKACIA_VERSII_PZ == 33))

#define EKRAN_RANGUVANNJA_INPUT_14 (EKRAN_RANGUVANNJA_INPUT_13 + 1)
#define EKRAN_RANGUVANNJA_INPUT_15 (EKRAN_RANGUVANNJA_INPUT_14 + 1)
#define EKRAN_RANGUVANNJA_INPUT_16 (EKRAN_RANGUVANNJA_INPUT_15 + 1)

#if (                              \
  (MODYFIKACIA_VERSII_PZ == 8) ||  \
  (MODYFIKACIA_VERSII_PZ == 10) || \
  (MODYFIKACIA_VERSII_PZ == 15) || \
  (MODYFIKACIA_VERSII_PZ == 18))

#define EKRAN_RANGUVANNJA_INPUT_17 (EKRAN_RANGUVANNJA_INPUT_16 + 1)
#define EKRAN_RANGUVANNJA_INPUT_18 (EKRAN_RANGUVANNJA_INPUT_17 + 1)
#define EKRAN_RANGUVANNJA_INPUT_19 (EKRAN_RANGUVANNJA_INPUT_18 + 1)
#define EKRAN_RANGUVANNJA_INPUT_20 (EKRAN_RANGUVANNJA_INPUT_19 + 1)

#if (                             \
  (MODYFIKACIA_VERSII_PZ == 8) || \
  (MODYFIKACIA_VERSII_PZ == 18))

#define EKRAN_RANGUVANNJA_INPUT_21 (EKRAN_RANGUVANNJA_INPUT_20 + 1)
#define EKRAN_RANGUVANNJA_INPUT_22 (EKRAN_RANGUVANNJA_INPUT_21 + 1)
#define EKRAN_RANGUVANNJA_INPUT_23 (EKRAN_RANGUVANNJA_INPUT_22 + 1)
#define EKRAN_RANGUVANNJA_INPUT_24 (EKRAN_RANGUVANNJA_INPUT_23 + 1)
#define EKRAN_RANGUVANNJA_INPUT_25 (EKRAN_RANGUVANNJA_INPUT_24 + 1)
#define EKRAN_RANGUVANNJA_INPUT_26 (EKRAN_RANGUVANNJA_INPUT_25 + 1)
#define EKRAN_RANGUVANNJA_INPUT_27 (EKRAN_RANGUVANNJA_INPUT_26 + 1)
#define EKRAN_RANGUVANNJA_INPUT_28 (EKRAN_RANGUVANNJA_INPUT_27 + 1)
#define EKRAN_RANGUVANNJA_INPUT_29 (EKRAN_RANGUVANNJA_INPUT_28 + 1)
#define EKRAN_RANGUVANNJA_INPUT_30 (EKRAN_RANGUVANNJA_INPUT_29 + 1)
#define EKRAN_RANGUVANNJA_INPUT_31 (EKRAN_RANGUVANNJA_INPUT_30 + 1)
#define EKRAN_RANGUVANNJA_INPUT_32 (EKRAN_RANGUVANNJA_INPUT_31 + 1)

#endif

#endif

#endif

#endif

#endif

#if ( \
  (MODYFIKACIA_VERSII_PZ == 18))
#define EKRAN_RANGUVANNJA_INPUT_LAST EKRAN_RANGUVANNJA_INPUT_32

#elif (                            \
  (MODYFIKACIA_VERSII_PZ == 10) || \
  (MODYFIKACIA_VERSII_PZ == 15))

#define EKRAN_RANGUVANNJA_INPUT_LAST EKRAN_RANGUVANNJA_INPUT_20

#elif (                            \
  (MODYFIKACIA_VERSII_PZ == 11) || \
  (MODYFIKACIA_VERSII_PZ == 13) || \
  (MODYFIKACIA_VERSII_PZ == 33))

#define EKRAN_RANGUVANNJA_INPUT_LAST EKRAN_RANGUVANNJA_INPUT_16

#elif ( \
  (MODYFIKACIA_VERSII_PZ == 17))

#define EKRAN_RANGUVANNJA_INPUT_LAST EKRAN_RANGUVANNJA_INPUT_13

#elif ( \
  (MODYFIKACIA_VERSII_PZ == 16))

#define EKRAN_RANGUVANNJA_INPUT_LAST EKRAN_RANGUVANNJA_INPUT_12

#else

#define EKRAN_RANGUVANNJA_INPUT_LAST EKRAN_RANGUVANNJA_INPUT_8

#endif

#define MAX_ROW_LIST_INPUTS_FOR_RANGUVANNJA NUMBER_INPUTS
#define MAX_ROW_RANGUVANNJA_INPUT NUMBER_TOTAL_SIGNAL_FOR_RANG_SMALL

#define NAME_RANG_SMALL_RU \
  " Сброс индикации",      \
    "   Сброс реле   ",    \
    " Местн./Дистанц.",    \
    "  Гр.уставок 1  ",    \
    "  Гр.уставок 2  ",    \
    "  Гр.уставок 3  ",    \
    "  Гр.уставок 4  ",    \
    " С.Блк.Гот.к ТУ ",    \
    " Блок.Вх.GOOSExx",    \
    "  Блок.Вх.MMSx  ",    \
    "   Блок.ВСБx    ",    \
    "  Основной ТН2  ",    \
    "  МРЗС/Местный  ",    \
    " Режим ТМ от ДВ ",    \
    "   Режим Авт.   ",    \
    "  Переключение  ",    \
    " Убавить от ДВ  ",    \
    " Прибавить от ДВ",    \
    " Блок.ток.комп. ",    \
    "  Внешн.блок.   ",    \
    " Пол.1 кон.выкл.",    \
    " Номинальн.пол. ",    \
    " Пол.N кон.выкл.",    \
    " Сброс блок.РПН ",    \
    " Убавить от Кл. ",    \
    "Прибавить от Кл.",    \
    " Контр.Убавить  ",    \
    " Контр.Прибавить",    \
    "     Бл.БРП     ",    \
    "   Бл.ЗНмакс2   ",    \
    "   Бл.ЗНмин1    ",    \
    "   Бл.ЗНмин2    ",    \
    "   Блок.УЗ x    ",    \
    " Вх.О-функции 1 ",    \
    " Сбр.О-функции 1",    \
    " Вх.О-функции 2 ",    \
    " Сбр.О-функции 2",    \
    " Вх.О-функции 3 ",    \
    " Сбр.О-функции 3",    \
    " Вх.О-функции 4 ",    \
    " Сбр.О-функции 4",    \
    " Вх.О-функции 5 ",    \
    " Сбр.О-функции 5",    \
    " Вх.О-функции 6 ",    \
    " Сбр.О-функции 6",    \
    " Вх.О-функции 7 ",    \
    " Сбр.О-функции 7",    \
    " Вх.О-функции 8 ",    \
    " Сбр.О-функции 8",    \
    "Уст.О-триггера 1",    \
    "Сбр.О-триггера 1",    \
    "Уст.О-триггера 2",    \
    "Сбр.О-триггера 2",    \
    "Уст.О-триггера 3",    \
    "Сбр.О-триггера 3",    \
    "Уст.О-триггера 4",    \
    "Сбр.О-триггера 4",    \
    "      ФС 1      ",    \
    "      ФС 2      ",    \
    "      ФС 3      ",    \
    "      ФС 4      ",    \
    "      ФС 5      ",    \
    "      ФС 6      ",    \
    "      ФС 7      ",    \
    "      ФС 8      ",    \
    "      ФС 9      ",    \
    "     ФС 10      ",    \
    "     ФС 11      ",    \
    "     ФС 12      ",    \
    "     ФС 13      ",    \
    "     ФС 14      ",    \
    "     ФС 15      ",    \
    "     ФС 16      "

#define NAME_RANG_SMALL_UA \
  " Скид.індикації ",      \
    "   Скид.реле    ",    \
    " Місц./Дистанц. ",    \
    "  Гр.уставок 1  ",    \
    "  Гр.уставок 2  ",    \
    "  Гр.уставок 3  ",    \
    "  Гр.уставок 4  ",    \
    " С.блк.Гот.до ТУ",    \
    " Блок.Вх.GOOSExx",    \
    "  Блок.Вх.MMSx  ",    \
    "   Блок.ВМБx    ",    \
    "  Основний ТН2  ",    \
    " МРЗС/Місцевий  ",    \
    " Режим ТМ від ДВ",    \
    "   Режим Авт.   ",    \
    "  Перемикання   ",    \
    " Знизити від ДВ ",    \
    "Підвищити від ДВ",    \
    " Блок.стр.комп. ",    \
    "   Зовн.блок.   ",    \
    "Пол.1 кон.вимик.",    \
    " Номінальна поз.",    \
    "Пол.N кон.вимик.",    \
    "  Ск.блок.РПН   ",    \
    " Знизити від Кл.",    \
    "Підвищити від Кл",    \
    " Контр.Знизити  ",    \
    " Контр.Підвищити",    \
    "     Бл.БРП     ",    \
    "   Бл.ЗНмакс2   ",    \
    "   Бл.ЗНмін1    ",    \
    "   Бл.ЗНмін2    ",    \
    "   Блок.УЗ x    ",    \
    " Вх.В-функції 1 ",    \
    " Ск.В-функції 1 ",    \
    " Вх.В-функції 2 ",    \
    " Ск.В-функції 2 ",    \
    " Вх.В-функції 3 ",    \
    " Ск.В-функції 3 ",    \
    " Вх.В-функції 4 ",    \
    " Ск.В-функції 4 ",    \
    " Вх.В-функції 5 ",    \
    " Ск.В-функції 5 ",    \
    " Вх.В-функції 6 ",    \
    " Ск.В-функції 6 ",    \
    " Вх.В-функції 7 ",    \
    " Ск.В-функції 7 ",    \
    " Вх.В-функції 8 ",    \
    " Ск.В-функції 8 ",    \
    " Вст.В-триґера 1",    \
    " Ск.В-триґера 1 ",    \
    " Вст.В-триґера 2",    \
    " Ск.В-триґера 2 ",    \
    " Вст.В-триґера 3",    \
    " Ск.В-триґера 3 ",    \
    " Вст.В-триґера 4",    \
    " Ск.В-триґера 4 ",    \
    "      ФЗ 1      ",    \
    "      ФЗ 2      ",    \
    "      ФЗ 3      ",    \
    "      ФЗ 4      ",    \
    "      ФЗ 5      ",    \
    "      ФЗ 6      ",    \
    "      ФЗ 7      ",    \
    "      ФЗ 8      ",    \
    "      ФЗ 9      ",    \
    "     ФЗ 10      ",    \
    "     ФЗ 11      ",    \
    "     ФЗ 12      ",    \
    "     ФЗ 13      ",    \
    "     ФЗ 14      ",    \
    "     ФЗ 15      ",    \
    "     ФЗ 16      "

#define NAME_RANG_SMALL_EN \
  "   LED Reset    ",      \
    "  Relay Reset   ",    \
    "  Local/Remote  ",    \
    " SetPoints Gr-1 ",    \
    " SetPoints Gr-2 ",    \
    " SetPoints Gr-3 ",    \
    " SetPoints Gr-4 ",    \
    "Rem Ctr R Bl Res",    \
    " GSERECEIVxx Blk",    \
    " MMSRECEIVx Blk ",    \
    "  NETSENDx Blk  ",    \
    "  VT2 is Main   ",    \
    " MRZS/Local Mode",    \
    " TM Mode from DI",    \
    "   AUTO Mode    ",    \
    "   Switching    ",    \
    " Lower from DI  ",    \
    " Raise from DI  ",    \
    "Blc.of Cur.Comp.",    \
    "  Ext.Blocking  ",    \
    "Pos.1 of End Sw.",    \
    " Rated Position ",    \
    "Pos.N of End Sw.",    \
    " OLTC Blc.Reset ",    \
    " Lower from KB  ",    \
    " Raise from KB  ",    \
    "Control of Raise",    \
    "Control of Lower",    \
    " Ctrl.B.at OC B.",    \
    " OVP2 Blocking  ",    \
    " UVP1 Blocking  ",    \
    " UVP2 Blocking  ",    \
    "  MFP x Block   ",    \
    "  UD-Func 1 In  ",    \
    " UD-Func 1 Reset",    \
    "  UD-Func 2 In  ",    \
    " UD-Func 2 Reset",    \
    "  UD-Func 3 In  ",    \
    " UD-Func 3 Reset",    \
    "  UD-Func 4 In  ",    \
    " UD-Func 4 Reset",    \
    "  UD-Func 5 In  ",    \
    " UD-Func 5 Reset",    \
    "  UD-Func 6 In  ",    \
    " UD-Func 6 Reset",    \
    "  UD-Func 7 In  ",    \
    " UD-Func 7 Reset",    \
    "  UD-Func 8 In  ",    \
    " UD-Func 8 Reset",    \
    "  UD-FF 1 Set   ",    \
    " UD-FF 1 Reset  ",    \
    "  UD-FF 2 Set   ",    \
    " UD-FF 2 Reset  ",    \
    "  UD-FF 3 Set   ",    \
    " UD-FF 3 Reset  ",    \
    "  UD-FF 4 Set   ",    \
    " UD-FF 4 Reset  ",    \
    "      CF 1      ",    \
    "      CF 2      ",    \
    "      CF 3      ",    \
    "      CF 4      ",    \
    "      CF 5      ",    \
    "      CF 6      ",    \
    "      CF 7      ",    \
    "      CF 8      ",    \
    "      CF 9      ",    \
    "     CF 10      ",    \
    "     CF 11      ",    \
    "     CF 12      ",    \
    "     CF 13      ",    \
    "     CF 14      ",    \
    "     CF 15      ",    \
    "     CF 16      "

#define NAME_RANG_SMALL_KZ \
  " Сброс индикации",      \
    "   Сброс реле   ",    \
    " Местн./Дистанц.",    \
    "  Гр.уставок 1  ",    \
    "  Гр.уставок 2  ",    \
    "  Гр.уставок 3  ",    \
    "  Гр.уставок 4  ",    \
    " С.блк.Гот.к ТУ ",    \
    " Блок.Вх.GOOSExx",    \
    "  Блок.Вх.MMSx  ",    \
    "   Блок.ВСБx    ",    \
    "  Основной ТН2  ",    \
    "  МРЗС/Местный  ",    \
    " Режим ТМ від ДВ",    \
    "   Режим Авт.   ",    \
    "  Переключение  ",    \
    " Убавить от ДВ  ",    \
    " Прибавить от ДВ",    \
    " Блок.ток.комп. ",    \
    "  Внешн.блок.   ",    \
    " Пол.1 кон.выкл.",    \
    " Номинальн.пол. ",    \
    " Пол.N кон.выкл.",    \
    " Сброс блок.РПН ",    \
    " Убавить от Кл. ",    \
    "Прибавить от Кл.",    \
    " Контр.Убавить  ",    \
    " Контр.Прибавить",    \
    "     Бл.БРП     ",    \
    "   Бл.ЗНмакс2   ",    \
    "   Бл.ЗНмин1    ",    \
    "   Бл.ЗНмин2    ",    \
    "   Блок.УЗ x    ",    \
    " Вх.О-функции 1 ",    \
    " Сбр.О-функции 1",    \
    " Вх.О-функции 2 ",    \
    " Сбр.О-функции 2",    \
    " Вх.О-функции 3 ",    \
    " Сбр.О-функции 3",    \
    " Вх.О-функции 4 ",    \
    " Сбр.О-функции 4",    \
    " Вх.О-функции 5 ",    \
    " Сбр.О-функции 5",    \
    " Вх.О-функции 6 ",    \
    " Сбр.О-функции 6",    \
    " Вх.О-функции 7 ",    \
    " Сбр.О-функции 7",    \
    " Вх.О-функции 8 ",    \
    " Сбр.О-функции 8",    \
    "Уст.О-триггера 1",    \
    "Сбр.О-триггера 1",    \
    "Уст.О-триггера 2",    \
    "Сбр.О-триггера 2",    \
    "Уст.О-триггера 3",    \
    "Сбр.О-триггера 3",    \
    "Уст.О-триггера 4",    \
    "Сбр.О-триггера 4",    \
    "      ФС 1      ",    \
    "      ФС 2      ",    \
    "      ФС 3      ",    \
    "      ФС 4      ",    \
    "      ФС 5      ",    \
    "      ФС 6      ",    \
    "      ФС 7      ",    \
    "      ФС 8      ",    \
    "      ФС 9      ",    \
    "     ФС 10      ",    \
    "     ФС 11      ",    \
    "     ФС 12      ",    \
    "     ФС 13      ",    \
    "     ФС 14      ",    \
    "     ФС 15      ",    \
    "     ФС 16      "

#endif
