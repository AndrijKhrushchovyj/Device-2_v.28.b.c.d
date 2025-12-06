#ifndef __CONST_MESUREMENT__
#define __CONST_MESUREMENT__

#define USTUVANNJA_VAGA 12
#define DEFAULT_USTUVANNJA_VALUE ((1<<USTUVANNJA_VAGA) - 1)
#define AMPLITUDA_SECTOR_VAGA 7
#define AMPLITUDA_SECTOR      (1 << AMPLITUDA_SECTOR_VAGA)
#define AMPLITUDA_FI_VAGA     10
#define AMPLITUDA_FI          (1 << AMPLITUDA_FI_VAGA)

#define SECTOR1   10
#define POPRAVKA_NZZ  0/*-2*/
#define SECTOR2   5

#define SECTOR1_MTZ_TZNP   7
#define POPRAVKA_MTZ_TZNP  0/*3*/
#define SECTOR2_MTZ_TZNP   2

#define MEASUREMENT_TIM_FREQUENCY       60000000 //МГц

#define MAIN_FREQUENCY  50      //Гц
#define MIN_FREQUENCY   45      //Гц
#define MAX_FREQUENCY   55      //Гц

#define MIN_TICK_PERIOD (MEASUREMENT_TIM_FREQUENCY/MAX_FREQUENCY - 1)
#define MAX_TICK_PERIOD (MEASUREMENT_TIM_FREQUENCY/MIN_FREQUENCY + 1)

#define NUMBER_ADCs                     1
#define NUMBER_CANALs_ADC               16
#define NUMBER_INPUTs_ADCs              (NUMBER_ADCs*NUMBER_CANALs_ADC)

#define VAGA_NUMBER_POINT                5
#define NUMBER_POINT                     (1 << VAGA_NUMBER_POINT)

/*
Коефіцієнт, який переводить число з АЦП у мА
          K*2500
Koef = ------------
        R*1*0xFFF
де:
K - коефіцієнт трансформації і він рівний 500 для 3I0, або 2500 для фазних струмів
2500 - максимальне значення напруги у мілівольтах, яке може оцифрувати АЦП (у нас це є 2,5 В = 2500 мВ)
R - резистор, який перетворює трансформований струм у напругу. Зараз стоїть 12,7 Ом
1 - після перетворення струму в напругу як мінімум раз сигнвал проходить через підсилювач на операційному підсилювачі з коефіцієнтом підсилення "1"
0xFFF - максимальне число, яке може видати АЦП

Koef = 24,035457106323248055531520098449 для 3I0 (1538/64 = 24,03125)
Koef = 120,17728553161624027765760049225 для фазних струмів (7691/64 = 120,171875) 

Множення на ці коефіцієнти переводять одиниці АЦП у мА
Оскільки нам треба відображати струм 3I0 з точністю до міліамперів, тоді формула буде
          K*25000
Koef = ------------
        R*1*0xFFF
де:
K - коефіцієнт трансформації і він рівний 500 для 3I0
25000 - максимальне значення напруги у десятих мілівольт, яке може оцифрувати АЦП (у нас це є 2,5 В = 2500,0 мВ)
R - резистор, який перетворює трансформований струм у напругу. Зараз стоїть 12,7 Ом
1 - після перетворення струму в напругу як мінімум раз сигнвал проходить через підсилювач на операційному підсилювачі з коефіцієнтом підсилення "1"
0xFFF - максимальне число, яке може видати АЦП

Koef* = 240,35457106323248055531520098449 для 3I0 при вираженні у десятих міліамперів (1923/8 = 240,375)

Але коли ми проводимо розрахунки з допомогою перетворення Фур'є, то результатом є амплітуда 1-ої  агмоніки (або інших гармонік. якщо розрахунок би йшов по них)
Тому крім цього коефіцієнта треба ще поділити на корінб з двох.
для простоти зробимо узагальнений коефіцієнт, що є результатом Koef_1 = Koef/sqrt(2)

Koef_1 = 16,995634708799561699038686560931 для 3I0 (1088/64 = 17)
Koef_1* = 169,95634708799561699038686560931 для 3I0  для 3I0 при вираженні у десятих міліамперів (170/1 = 170)
Koef_1 = 84,978173543997808495193432804655 для фазних струмів (5439/64 = 84,984375)
*/
#define MNOGNYK_3I0                   1538//1825           
#define VAGA_DILENNJA_3I0             6
//#define MNOGNYK_3I0_D                 1923          
//#define VAGA_DILENNJA_3I0_D           3
//#define MNOGNYK_3I0_DIJUCHE_D         170           
//#define VAGA_DILENNJA_3I0_DIJUCHE_D   0
#define MNOGNYK_3I0_DIJUCHE_D_mA         17           
#define VAGA_DILENNJA_3I0_DIJUCHE_D_mA   0
#define MNOGNYK_3I0_FLOAT               170.0f

#define MNOGNYK_I                     7691           
#define VAGA_DILENNJA_I               6
#define MNOGNYK_I_DIJUCHE             5439//6451          
#define VAGA_DILENNJA_I_DIJUCHE       6
#define MNOGNYK_I_DIJUCHE_DOUBLE       84.984375

#define MNOGNYK_I2G_DIJUCHE           701          
#define VAGA_DILENNJA_I2G_DIJUCHE     3

/*
Коефіцієнт, який переводить число з АЦП у мB
          Ku*2500*R1
Koef = --------------
        R2*1*0xFFF
де:
Ku - коефіцієнт трансформації і він рівний 1
2500 - максимальне значення напруги у мілівольтах, яке може оцифрувати АЦП (у нас це є 2,5 В = 2500 мВ)
R1 -  резистор, який визначає струм у первинній обмотці трансвформатора струму. Зараз стоїть 86.5(75) кОм
R2 - резистор, який перетворює трансформований струм у напругу. Зараз стоїть 499 Ом
1 - після перетворення струму в напругу як мінімум раз сигнвал проходить через підсилювач на операційному підсилювачі з коефіцієнтом підсилення "1"
0xFFF - максимальне число, яке може видати АЦП

Koef = 105.82826214088739138839339240141 (6773/64 = 105.828125)

Множення на ці коефіцієнти переводять одиниці АЦП у мВ

Але коли ми проводимо розрахунки з допомогою перетворення Фур'є, то результатом є амплітуда 1-ої  агмоніки (або інших гармонік. якщо розрахунок би йшов по них)
Тому крім цього коефіцієнта треба ще поділити на корінь з двох.
для простоти зробимо узагальнений коефіцієнт, що є результатом Koef_1 = Koef/sqrt(2)

Koef_1 = 74.831881801009052122160116719483 (4790/64 = 74.84375)
*/
#define MNOGNYK_U                     6773           
#define VAGA_DILENNJA_U               6
#define MNOGNYK_U_DIJUCHE             4790           
#define VAGA_DILENNJA_U_DIJUCHE       6
#define MNOGNYK_U_DIJUCHE_DOUBLE      74.831881801009052122160116719483

/*
При роботі з ортогональними ми працюємо у поділках АЦП і сигналом підсиленим у 16 раз
Щоб отримати мВ(мА) треба результат математичних обчислень на відповідний масштабуючий коефіцієнт
      
При розрахунку діючих значень використовувалися пари (MNOGNYK_U_DIJUCHE; VAGA_DILENNJA_U_DIJUCHE)
і (MNOGNYK_I_DIJUCHE; VAGA_DILENNJA_I_DIJUCHE)
      
Тоді миотримували числа у мВ(мА)
            мВ
При діленні -- ми отримаємо розмірність Ом.
            мА 
      
Ми хочемо отримати число з точністю до мОм. Тоді чисельник треба помножити на 1000

результатом є число 880,67659496230924802353373781945
Так як розрахунки проводяться над числами у форматі long long, то і константу я роблю у форматі long long
*/
#define MNOGNYK_R_FLOAT                 880.67659496230924802353373781945f


#define MAX_INDEX_DATA_FOR_OSCYLOGRAPH  10

#define GND_NORMAL_VALUE                0x0
#define VREF_NORMAL_VALUE               0x800
#define VDD_NORMAL_VALUE                0xb00

#define NUMBER_I2G_CANALES      3


//Канали
#define C_3I0_256               0
#define C_3I0_16                1
#define C_Ic_1                  2
#define C_Ic_16                 3
#define C_Ib_1                  4
#define C_Ib_16                 5
#define C_Ia_1                  6
#define C_Ia_16                 7
#define C_3U0_1                 8
#define C_3U0_16                9
#define C_Uc_1                  10
#define C_Uc_16                 11
#define C_Ua_16                 12
#define C_Ua_1                  13
#define C_Ub_16                 14
#define C_Ub_1                  15

#define READ_3U0 (                    \
                  (1 << C_3U0_1  ) |  \
                  (1 << C_3U0_16 )    \
                 )

#define READ_Ua  (                     \
                  (1 << C_Ua_1   ) |   \
                  (1 << C_Ua_16  )     \
                 )

#define READ_Ub  (                     \
                  (1 << C_Ub_1   ) |   \
                  (1 << C_Ub_16  )     \
                       )

#define READ_Uc  (                     \
                  (1 << C_Uc_1   ) |   \
                  (1 << C_Uc_16  )     \
                 )

#define READ_I   (                     \
                  (1 << C_3I0_16 ) |   \
                  (1 << C_3I0_256) |   \
                  (1 << C_Ia_1   ) |   \
                  (1 << C_Ia_16  ) |   \
                  (1 << C_Ib_1   ) |   \
                  (1 << C_Ib_16  ) |   \
                  (1 << C_Ic_1   ) |   \
                  (1 << C_Ic_16  )     \
                 )

#define READ_DATA_VAL  (          \
                        READ_I  | \
                        READ_Ua | \
                        READ_Ub | \
                        READ_Uc | \
                        READ_3U0  \
                      )

//#define READ_TEST_VAL  (                                  \
//                        (1 << C_GND_ADC1_1)             | \
//                        (1 << C_GND_ADC1_2)             | \
//                        (1 << C_GND_ADC1_3)             | \
//                        (1 << C_GND_ADC1_4)             | \
//                        (1 << C_GND_ADC1_5)             | \
//                        (1 << C_VREF_ADC1 )             | \
//                        (1 << C_VDD_ADC1  )             | \
//                        (1 << C_GND_ADC2_1)             | \
//                        (1 << C_GND_ADC2_2)             | \
//                        (1 << C_GND_ADC2_3)             | \
//                        (1 << C_VREF_ADC2_1)            | \
//                      /*(1 << C_VREF_ADC2_2)            |*/\
//                      /*(1 << C_VREF_ADC2_3)            |*/\
//                      /*(1 << C_VREF_ADC2_4)            |*/\
//          (unsigned int)(1 << C_VDD_ADC2  )               \
//                       )

#define DATA_VAL_READ_BIT       0
#define DATA_VAL_READ           (1 << DATA_VAL_READ_BIT)
//#define TEST_VAL_READ_BIT       1
//#define TEST_VAL_READ           (1 << TEST_VAL_READ_BIT)

//#define NUMBER_GND_ADC1         5

//#define NUMBER_GND_ADC2         3

#define NUMBER_VREF_ADC2        1/*4*/

#define N_VAL_1                 0

#define I_3I0         0
#define I_Ia          1
#define I_Ib_I04      2
#define I_Ic          3
#define I_Ua          4
#define I_Ub          5
#define I_Uc          6
#define I_3U0         7

enum _index_meas
{
  IM_3I0_i = 0,
  IM_3I0,
  IM_3I0_other_g,
  IM_3I0_r,
  IM_IA,
  IM_IB,
  IM_IC,
  IM_I2,
  IM_I1,
  IM_I04,
  IM_UA,
  IM_UB,
  IM_UC,
  IM_3U0,
  IM_UAB,
  IM_UBC,
  IM_UCA,
  IM_U2,
  IM_U1,

  IM2_IA,
  IM2_IB,
  IM2_IC,
	
  _NUMBER_IM
};

#define PORIG_FOR_FAPCH 10000

/*****************************************/
//Константи для фазової корекції між ТН1 і ТН2
/*****************************************/
enum _PhK
{
INDEX_PhK_UA = 0,
INDEX_PhK_UB,
INDEX_PhK_UC,

MAX_INDEX_PhK
};
/*****************************************/

enum _full_ort_index
{
FULL_ORT_Ua = 0,
FULL_ORT_Ub,
FULL_ORT_Uc,
FULL_ORT_Uab,
FULL_ORT_Ubc,
FULL_ORT_Uca,
FULL_ORT_3U0,
FULL_ORT_Ia,
FULL_ORT_Ib,
FULL_ORT_Ic,
FULL_ORT_3I0,
FULL_ORT_3I0_r,
FULL_ORT_I04,

FULL_ORT_MAX
};

#define DIV_kWh         3600000000.0
#define PERIOD_SAVE_ENERGY_IN_MINUTES           30 /*хв.*/
enum _index_energy
{
  INDEX_EA_PLUS = 0,
  INDEX_EA_MINUS,
  INDEX_ER_1,
  INDEX_ER_2,
  INDEX_ER_3,
  INDEX_ER_4,
  
  MAX_NUMBER_INDEXES_ENERGY
};

#define UNDEF_PHI                              (1 << 31)
#define UNDEF_RESISTANCE                       (1 << 31)
#define UNDEF_VMP                              (1 << 31)

enum _resistance_index
{
  R_AB = 0,
  X_AB,
  R_BC,
  X_BC,
  R_CA,
  X_CA,
  
  MAX_NUMBER_INDEXES_RESISTANCE,
};

/*****************************************/
//Константи для усереднення частоти і ШЗЧ
/*****************************************/
#define N_F_AVER			4
/*****************************************/


#endif
