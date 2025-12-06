#ifndef __CONSTANTS_INPUTS__
#define __CONSTANTS_INPUTS__

#define EKRAN_LIST_INPUTS_FOR_RANGUVANNJA        (EKRAN_LN_FOR_IEC61850_RANG + 1)

#define EKRAN_RANGUVANNJA_INPUT_1                (EKRAN_TRANSFORMATOR_INFO_CONTROL + 1)
#define EKRAN_RANGUVANNJA_INPUT_2                (EKRAN_RANGUVANNJA_INPUT_1 + 1)
#define EKRAN_RANGUVANNJA_INPUT_3                (EKRAN_RANGUVANNJA_INPUT_2 + 1)
#define EKRAN_RANGUVANNJA_INPUT_4                (EKRAN_RANGUVANNJA_INPUT_3 + 1)
#define EKRAN_RANGUVANNJA_INPUT_5                (EKRAN_RANGUVANNJA_INPUT_4 + 1)
#define EKRAN_RANGUVANNJA_INPUT_6                (EKRAN_RANGUVANNJA_INPUT_5 + 1)
#define EKRAN_RANGUVANNJA_INPUT_7                (EKRAN_RANGUVANNJA_INPUT_6 + 1)
#define EKRAN_RANGUVANNJA_INPUT_8                (EKRAN_RANGUVANNJA_INPUT_7 + 1)

#if (                                 \
     (MODYFIKACIA_VERSII_PZ == 8) ||  \
     (MODYFIKACIA_VERSII_PZ == 10) || \
     (MODYFIKACIA_VERSII_PZ == 11) || \
     (MODYFIKACIA_VERSII_PZ == 13) || \
     (MODYFIKACIA_VERSII_PZ == 15) || \
     (MODYFIKACIA_VERSII_PZ == 16) || \
     (MODYFIKACIA_VERSII_PZ == 17) || \
     (MODYFIKACIA_VERSII_PZ == 18) || \
     (MODYFIKACIA_VERSII_PZ == 33)    \
    )

#define EKRAN_RANGUVANNJA_INPUT_9                (EKRAN_RANGUVANNJA_INPUT_8 + 1)
#define EKRAN_RANGUVANNJA_INPUT_10               (EKRAN_RANGUVANNJA_INPUT_9 + 1)
#define EKRAN_RANGUVANNJA_INPUT_11               (EKRAN_RANGUVANNJA_INPUT_10 + 1)
#define EKRAN_RANGUVANNJA_INPUT_12               (EKRAN_RANGUVANNJA_INPUT_11 + 1)

#if (                                 \
     (MODYFIKACIA_VERSII_PZ == 8)  || \
     (MODYFIKACIA_VERSII_PZ == 10) || \
     (MODYFIKACIA_VERSII_PZ == 11) || \
     (MODYFIKACIA_VERSII_PZ == 13) || \
     (MODYFIKACIA_VERSII_PZ == 15) || \
     (MODYFIKACIA_VERSII_PZ == 17) || \
     (MODYFIKACIA_VERSII_PZ == 18) || \
     (MODYFIKACIA_VERSII_PZ == 33)    \
    )

#define EKRAN_RANGUVANNJA_INPUT_13               (EKRAN_RANGUVANNJA_INPUT_12 + 1)

#if (                                 \
     (MODYFIKACIA_VERSII_PZ == 8)  || \
     (MODYFIKACIA_VERSII_PZ == 10) || \
     (MODYFIKACIA_VERSII_PZ == 11) || \
     (MODYFIKACIA_VERSII_PZ == 13) || \
     (MODYFIKACIA_VERSII_PZ == 15) || \
     (MODYFIKACIA_VERSII_PZ == 18) ||   \
     (MODYFIKACIA_VERSII_PZ == 33)    \
    )

#define EKRAN_RANGUVANNJA_INPUT_14               (EKRAN_RANGUVANNJA_INPUT_13 + 1)
#define EKRAN_RANGUVANNJA_INPUT_15               (EKRAN_RANGUVANNJA_INPUT_14 + 1)
#define EKRAN_RANGUVANNJA_INPUT_16               (EKRAN_RANGUVANNJA_INPUT_15 + 1)

#if (                                    \
     (MODYFIKACIA_VERSII_PZ == 8)  ||    \
     (MODYFIKACIA_VERSII_PZ == 10) ||    \
     (MODYFIKACIA_VERSII_PZ == 15) ||    \
     (MODYFIKACIA_VERSII_PZ == 18)       \
    )                                   

#define EKRAN_RANGUVANNJA_INPUT_17               (EKRAN_RANGUVANNJA_INPUT_16 + 1)
#define EKRAN_RANGUVANNJA_INPUT_18               (EKRAN_RANGUVANNJA_INPUT_17 + 1)
#define EKRAN_RANGUVANNJA_INPUT_19               (EKRAN_RANGUVANNJA_INPUT_18 + 1)
#define EKRAN_RANGUVANNJA_INPUT_20               (EKRAN_RANGUVANNJA_INPUT_19 + 1)

#if (                                    \
     (MODYFIKACIA_VERSII_PZ == 8)  ||    \
     (MODYFIKACIA_VERSII_PZ == 18)       \
    )                                   

#define EKRAN_RANGUVANNJA_INPUT_21               (EKRAN_RANGUVANNJA_INPUT_20 + 1)
#define EKRAN_RANGUVANNJA_INPUT_22               (EKRAN_RANGUVANNJA_INPUT_21 + 1)
#define EKRAN_RANGUVANNJA_INPUT_23               (EKRAN_RANGUVANNJA_INPUT_22 + 1)
#define EKRAN_RANGUVANNJA_INPUT_24               (EKRAN_RANGUVANNJA_INPUT_23 + 1)
#define EKRAN_RANGUVANNJA_INPUT_25               (EKRAN_RANGUVANNJA_INPUT_24 + 1)
#define EKRAN_RANGUVANNJA_INPUT_26               (EKRAN_RANGUVANNJA_INPUT_25 + 1)
#define EKRAN_RANGUVANNJA_INPUT_27               (EKRAN_RANGUVANNJA_INPUT_26 + 1)
#define EKRAN_RANGUVANNJA_INPUT_28               (EKRAN_RANGUVANNJA_INPUT_27 + 1)
#define EKRAN_RANGUVANNJA_INPUT_29               (EKRAN_RANGUVANNJA_INPUT_28 + 1)
#define EKRAN_RANGUVANNJA_INPUT_30               (EKRAN_RANGUVANNJA_INPUT_29 + 1)
#define EKRAN_RANGUVANNJA_INPUT_31               (EKRAN_RANGUVANNJA_INPUT_30 + 1)
#define EKRAN_RANGUVANNJA_INPUT_32               (EKRAN_RANGUVANNJA_INPUT_31 + 1)

#endif

#endif

#endif

#endif

#endif

#if (                                   \
     (MODYFIKACIA_VERSII_PZ == 18)      \
    )  
#define EKRAN_RANGUVANNJA_INPUT_LAST             EKRAN_RANGUVANNJA_INPUT_32

#elif (                                    \
     (MODYFIKACIA_VERSII_PZ == 10) ||    \
     (MODYFIKACIA_VERSII_PZ == 15)       \
    )                                   

#define EKRAN_RANGUVANNJA_INPUT_LAST             EKRAN_RANGUVANNJA_INPUT_20

#elif (                                 \
       (MODYFIKACIA_VERSII_PZ == 11) || \
       (MODYFIKACIA_VERSII_PZ == 13) || \
       (MODYFIKACIA_VERSII_PZ == 33)    \
      )   

#define EKRAN_RANGUVANNJA_INPUT_LAST             EKRAN_RANGUVANNJA_INPUT_16

#elif (                                  \
       (MODYFIKACIA_VERSII_PZ == 17)     \
      )   

#define EKRAN_RANGUVANNJA_INPUT_LAST             EKRAN_RANGUVANNJA_INPUT_13

#elif (                                  \
       (MODYFIKACIA_VERSII_PZ == 16)     \
      )   

#define EKRAN_RANGUVANNJA_INPUT_LAST             EKRAN_RANGUVANNJA_INPUT_12

#else

#define EKRAN_RANGUVANNJA_INPUT_LAST             EKRAN_RANGUVANNJA_INPUT_8

#endif

#define MAX_ROW_LIST_INPUTS_FOR_RANGUVANNJA       NUMBER_INPUTS
#define MAX_ROW_RANGUVANNJA_INPUT                 NUMBER_TOTAL_SIGNAL_FOR_RANG_SMALL

#define NAME_RANG_SMALL_RU              \
  "  Блок.вкл.ВВ   ",   \
  " Сброс индикации",   \
  "   Сброс реле   ",   \
  " Местн./Дистанц.",   \
  "  Положение ВВ  ",   \
  "Откл.от вн.защит",   \
  "    Откл.ВВ     ",   \
  "     Вкл.ВВ     ",   \
  " Контроль Откл. ",   \
  " Контроль Вкл.  ",   \
  "  Гр.уставок 1  ",   \
  "  Гр.уставок 2  ",   \
  "  Гр.уставок 3  ",   \
  "  Гр.уставок 4  ",   \
  " С.блк.Гот.к ТУ ",   \
  " Блок.Вх.GOOSExx",   \
  "  Блок.Вх.MMSx  ",   \
  "   Блок.ВСБx    ",   \
  "   Блок.МТЗ 1   ",   \
  "   Блок.МТЗ 2   ",   \
  " Блок.уск.МТЗ 2 ",   \
  "   Блок.МТЗ 3   ",   \
  "   Блок.МТЗ 4   ",   \
  "Блок.МТЗ 0.4кВ 1",   \
  "Блок.МТЗ 0.4кВ 2",   \
  "Бл.у.МТЗ 0.4кВ 2",   \
  "   Блок.ЗНам    ",   \
  "    Блок.ЗДЗ    ",   \
  " Пуск ЗДЗ от ДВ ",   \
  "    Блок.НЗЗ    ",   \
  "  Блок.ТЗНП 1   ",   \
  "  Блок.ТЗНП 2   ",   \
  "  Блок.ТЗНП 3   ",   \
  " Стат.блок.АПВ  ",   \
  " АЧР/ЧАПВ от ДВ ",   \
  "   Блок.АЧР 1   ",   \
  "   Блок.АЧР 2   ",   \
  "   Блок.УРОВ    ",   \
  " Пуск УРОВ от ДВ",   \
  "  Блок.ЗОП(I)   ",   \
  "  Блок.Umin 1   ",   \
  "  Пуск Umin 1   ",   \
  "  Блок.Umin 2   ",   \
  "  Пуск Umin 2   ",   \
  "  Блок.Umax 1   ",   \
  "  Блок.Umax 2   ",   \
  "   Блок.УЗ x    ",   \
  " Вх.О-функции 1 ",   \
  " Сбр.О-функции 1",   \
  " Вх.О-функции 2 ",   \
  " Сбр.О-функции 2",   \
  " Вх.О-функции 3 ",   \
  " Сбр.О-функции 3",   \
  " Вх.О-функции 4 ",   \
  " Сбр.О-функции 4",   \
  " Вх.О-функции 5 ",   \
  " Сбр.О-функции 5",   \
  " Вх.О-функции 6 ",   \
  " Сбр.О-функции 6",   \
  " Вх.О-функции 7 ",   \
  " Сбр.О-функции 7",   \
  " Вх.О-функции 8 ",   \
  " Сбр.О-функции 8",   \
  "Уст.О-триггера 1",   \
  "Сбр.О-триггера 1",   \
  "Уст.О-триггера 2",   \
  "Сбр.О-триггера 2",   \
  "Уст.О-триггера 3",   \
  "Сбр.О-триггера 3",   \
  "Уст.О-триггера 4",   \
  "Сбр.О-триггера 4",   \
  "      ФС 1      ",   \
  "      ФС 2      ",   \
  "      ФС 3      ",   \
  "      ФС 4      ",   \
  "      ФС 5      ",   \
  "      ФС 6      ",   \
  "      ФС 7      ",   \
  "      ФС 8      ",   \
  "      ФС 9      ",   \
  "     ФС 10      ",   \
  "     ФС 11      ",   \
  "     ФС 12      ",   \
  "     ФС 13      ",   \
  "     ФС 14      ",   \
  "     ФС 15      ",   \
  "     ФС 16      "

#define NAME_RANG_SMALL_UA         			\
  " Блок.увімкн.ВВ ",   \
  " Скид.індикації ",   \
  "   Скид.реле    ",   \
  " Місц./Дистанц. ",   \
  "    Стан ВВ     ",   \
  "Вимк.від зовн.з.",   \
  "    Вимк.ВВ     ",   \
  "    Увімк.ВВ    ",   \
  " Контроль Вимк. ",   \
  " Контроль Увімк.",   \
  "  Гр.уставок 1  ",   \
  "  Гр.уставок 2  ",   \
  "  Гр.уставок 3  ",   \
  "  Гр.уставок 4  ",   \
  " С.блк.Гот.до ТУ",   \
  " Блок.Вх.GOOSExx",   \
  "  Блок.Вх.MMSx  ",   \
  "   Блок.ВМБx    ",   \
  "   Блок.МСЗ 1   ",   \
  "   Блок.МСЗ 2   ",   \
  "Блок.приск.МСЗ 2",   \
  "   Блок.МСЗ 3   ",   \
  "   Блок.МСЗ 4   ",   \
  "Блок.МСЗ 0.4кВ 1",   \
  "Блок.МСЗ 0.4кВ 2",   \
  "Бл.п.МСЗ 0.4кВ 2",   \
  "   Блок.ЗНам    ",   \
  "    Блок.ЗДЗ    ",   \
  " Пуск ЗДЗ від ДВ",   \
  "    Блок.СЗЗ    ",   \
  "  Блок.СЗНП 1   ",   \
  "  Блок.СЗНП 2   ",   \
  "  Блок.СЗНП 3   ",   \
  " Стат.блок.АПВ  ",   \
  " АЧР/ЧАПВ від ДВ",   \
  "   Блок.АЧР 1   ",   \
  "   Блок.АЧР 2   ",   \
  "   Блок.ПРВВ    ",   \
  "Пуск ПРВВ від ДВ",   \
  "  Блок.ЗЗП(I)   ",   \
  "  Блок.Umin 1   ",   \
  "  Пуск Umin 1   ",   \
  "  Блок.Umin 2   ",   \
  "  Пуск Umin 2   ",   \
  "  Блок.Umax 1   ",   \
  "  Блок.Umax 2   ",   \
  "   Блок.УЗ x    ",   \
  " Вх.В-функції 1 ",   \
  " Ск.В-функції 1 ",   \
  " Вх.В-функції 2 ",   \
  " Ск.В-функції 2 ",   \
  " Вх.В-функції 3 ",   \
  " Ск.В-функції 3 ",   \
  " Вх.В-функції 4 ",   \
  " Ск.В-функції 4 ",   \
  " Вх.В-функції 5 ",   \
  " Ск.В-функції 5 ",   \
  " Вх.В-функції 6 ",   \
  " Ск.В-функції 6 ",   \
  " Вх.В-функції 7 ",   \
  " Ск.В-функції 7 ",   \
  " Вх.В-функції 8 ",   \
  " Ск.В-функції 8 ",   \
  " Вст.В-триґера 1",   \
  " Ск.В-триґера 1 ",   \
  " Вст.В-триґера 2",   \
  " Ск.В-триґера 2 ",   \
  " Вст.В-триґера 3",   \
  " Ск.В-триґера 3 ",   \
  " Вст.В-триґера 4",   \
  " Ск.В-триґера 4 ",   \
  "      ФЗ 1      ",   \
  "      ФЗ 2      ",   \
  "      ФЗ 3      ",   \
  "      ФЗ 4      ",   \
  "      ФЗ 5      ",   \
  "      ФЗ 6      ",   \
  "      ФЗ 7      ",   \
  "      ФЗ 8      ",   \
  "      ФЗ 9      ",   \
  "     ФЗ 10      ",   \
  "     ФЗ 11      ",   \
  "     ФЗ 12      ",   \
  "     ФЗ 13      ",   \
  "     ФЗ 14      ",   \
  "     ФЗ 15      ",   \
  "     ФЗ 16      "

#define NAME_RANG_SMALL_EN         \
  "  CB Close Blk  ",   \
  "   LED Reset    ",   \
  "  Relay Reset   ",   \
  "  Local/Remote  ",   \
  "    CB State    ",   \
  " Ext CB Open Cmd",   \
  "  Open CB Cmd   ",   \
  "  Close CB Cmd  ",   \
  "  Open CB Ctl   ",   \
  " Closing CB Ctl ",   \
  " SetPoints Gr-1 ",   \
  " SetPoints Gr-2 ",   \
  " SetPoints Gr-3 ",   \
  " SetPoints Gr-4 ",   \
  "Rem Ctr R Bl Res",   \
  " GSERECEIVxx Blk",   \
  " MMSRECEIVx Blk ",   \
  "  NETSENDx Blk  ",   \
  "   OCP 1 Blk    ",   \
  "   OCP 2 Blk    ",   \
  " OCP 2 Acc Blk  ",   \
  "   OCP 3 Blk    ",   \
  "   OCP 4 Blk    ",   \
  "  LVOCP 1 Blk   ",   \
  "  LVOCP 2 Blk   ",   \
  " LVOCP 2 acc Blk",   \
  "    InrP Blk    ",   \
  "    AFD Blk     ",   \
  "AFD via BI Start",   \
  "   SDEFP Blk    ",   \
  "   DEFP 1 Blk   ",   \
  "   DEFP 2 Blk   ",   \
  "   DEFP 3 Blk   ",   \
  "     AR Blk     ",   \
  " LFС/FAR v BI St",   \
  "   LFС 1 Blk    ",   \
  "   LFС 2 Blk    ",   \
  "    CBF Blk     ",   \
  " CBF v BI Start ",   \
  "   NSOCP Blk    ",   \
  "   UVP 1 Blk    ",   \
  " UVP 1 vBI Start",   \
  "   UVP 2 Blk    ",   \
  " UVP 2 vBI Start",   \
  "   OVP 1 Blk    ",   \
  "   OVP 2 Blk    ",   \
  "  MFP x Block   ",   \
  "  UD-Func 1 In  ",   \
  " UD-Func 1 Reset",   \
  "  UD-Func 2 In  ",   \
  " UD-Func 2 Reset",   \
  "  UD-Func 3 In  ",   \
  " UD-Func 3 Reset",   \
  "  UD-Func 4 In  ",   \
  " UD-Func 4 Reset",   \
  "  UD-Func 5 In  ",   \
  " UD-Func 5 Reset",   \
  "  UD-Func 6 In  ",   \
  " UD-Func 6 Reset",   \
  "  UD-Func 7 In  ",   \
  " UD-Func 7 Reset",   \
  "  UD-Func 8 In  ",   \
  " UD-Func 8 Reset",   \
  "  UD-FF 1 Set   ",   \
  " UD-FF 1 Reset  ",   \
  "  UD-FF 2 Set   ",   \
  " UD-FF 2 Reset  ",   \
  "  UD-FF 3 Set   ",   \
  " UD-FF 3 Reset  ",   \
  "  UD-FF 4 Set   ",   \
  " UD-FF 4 Reset  ",   \
  "      CF 1      ",   \
  "      CF 2      ",   \
  "      CF 3      ",   \
  "      CF 4      ",   \
  "      CF 5      ",   \
  "      CF 6      ",   \
  "      CF 7      ",   \
  "      CF 8      ",   \
  "      CF 9      ",   \
  "     CF 10      ",   \
  "     CF 11      ",   \
  "     CF 12      ",   \
  "     CF 13      ",   \
  "     CF 14      ",   \
  "     CF 15      ",   \
  "     CF 16      "
        
#define NAME_RANG_SMALL_KZ              \
  "  Блок.вкл.ВВ   ",   \
  " Сброс индикации",   \
  "   Сброс реле   ",   \
  " Местн./Дистанц.",   \
  "  Положение ВВ  ",   \
  "Откл.от вн.защит",   \
  "    Откл.ВВ     ",   \
  "     Вкл.ВВ     ",   \
  " Контроль Откл. ",   \
  " Контроль Вкл.  ",   \
  "  Гр.уставок 1  ",   \
  "  Гр.уставок 2  ",   \
  "  Гр.уставок 3  ",   \
  "  Гр.уставок 4  ",   \
  " С.блк.Гот.к ТУ ",   \
  " Блок.Вх.GOOSExx",   \
  "  Блок.Вх.MMSx  ",   \
  "   Блок.ВСБx    ",   \
  "   Блок.МТЗ 1   ",   \
  "   Блок.МТЗ 2   ",   \
  " Блок.уск.МТЗ 2 ",   \
  "   Блок.МТЗ 3   ",   \
  "   Блок.МТЗ 4   ",   \
  "Блок.МТЗ 0.4кВ 1",   \
  "Блок.МТЗ 0.4кВ 2",   \
  "Бл.у.МТЗ 0.4кВ 2",   \
  "   Блок.ЗНам    ",   \
  "    Блок.ЗДЗ    ",   \
  " Пуск ЗДЗ от ДВ ",   \
  "    Блок.НЗЗ    ",   \
  "  Блок.ТЗНП 1   ",   \
  "  Блок.ТЗНП 2   ",   \
  "  Блок.ТЗНП 3   ",   \
  " Стат.блок.АПВ  ",   \
  " АЧР/ЧАПВ от ДВ ",   \
  "   Блок.АЧР 1   ",   \
  "   Блок.АЧР 2   ",   \
  "   Блок.УРОВ    ",   \
  " Пуск УРОВ от ДВ",   \
  "  Блок.ЗОП(I)   ",   \
  "  Блок.Umin 1   ",   \
  "  Пуск Umin 1   ",   \
  "  Блок.Umin 2   ",   \
  "  Пуск Umin 2   ",   \
  "  Блок.Umax 1   ",   \
  "  Блок.Umax 2   ",   \
  "   Блок.УЗ x    ",   \
  " Вх.О-функции 1 ",   \
  " Сбр.О-функции 1",   \
  " Вх.О-функции 2 ",   \
  " Сбр.О-функции 2",   \
  " Вх.О-функции 3 ",   \
  " Сбр.О-функции 3",   \
  " Вх.О-функции 4 ",   \
  " Сбр.О-функции 4",   \
  " Вх.О-функции 5 ",   \
  " Сбр.О-функции 5",   \
  " Вх.О-функции 6 ",   \
  " Сбр.О-функции 6",   \
  " Вх.О-функции 7 ",   \
  " Сбр.О-функции 7",   \
  " Вх.О-функции 8 ",   \
  " Сбр.О-функции 8",   \
  "Уст.О-триггера 1",   \
  "Сбр.О-триггера 1",   \
  "Уст.О-триггера 2",   \
  "Сбр.О-триггера 2",   \
  "Уст.О-триггера 3",   \
  "Сбр.О-триггера 3",   \
  "Уст.О-триггера 4",   \
  "Сбр.О-триггера 4",   \
  "      ФС 1      ",   \
  "      ФС 2      ",   \
  "      ФС 3      ",   \
  "      ФС 4      ",   \
  "      ФС 5      ",   \
  "      ФС 6      ",   \
  "      ФС 7      ",   \
  "      ФС 8      ",   \
  "      ФС 9      ",   \
  "     ФС 10      ",   \
  "     ФС 11      ",   \
  "     ФС 12      ",   \
  "     ФС 13      ",   \
  "     ФС 14      ",   \
  "     ФС 15      ",   \
  "     ФС 16      "
        
#endif 
