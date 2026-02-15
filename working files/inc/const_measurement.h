#ifndef __CONST_MESUREMENT__
#define __CONST_MESUREMENT__

#define USTUVANNJA_VAGA 12
#define DEFAULT_USTUVANNJA_VALUE ((1 << USTUVANNJA_VAGA) - 1)

#define MEASUREMENT_TIM_FREQUENCY 60000000 //МГц

#define MAIN_FREQUENCY 50 //Гц
#define MIN_FREQUENCY 45  //Гц
#define MAX_FREQUENCY 55  //Гц

#define MIN_TICK_PERIOD (MEASUREMENT_TIM_FREQUENCY / MAX_FREQUENCY - 1)
#define MAX_TICK_PERIOD (MEASUREMENT_TIM_FREQUENCY / MIN_FREQUENCY + 1)

#define NUMBER_ADCs 1
#define NUMBER_CANALs_ADC 16
#define NUMBER_INPUTs_ADCs (NUMBER_ADCs * NUMBER_CANALs_ADC)

#define VAGA_NUMBER_POINT 5
#define NUMBER_POINT (1 << VAGA_NUMBER_POINT)

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
#define MNOGNYK_I 7691
#define VAGA_DILENNJA_I 6

#define MNOGNYK_I_DIJUCHE 5452 //6451
#define VAGA_DILENNJA_I_DIJUCHE 6

/*
Коефіцієнт, який переводить число з АЦП у мB
          Ku*2500*R1
Koef = --------------
        R2*1*0xFFF
де:
Ku - коефіцієнт трансформації і він рівний 1
2500 - максимальне значення напруги у мілівольтах, яке може оцифрувати АЦП (у нас це є 2,5 В = 2500 мВ)
R1 -  резистор, який визначає струм у первинній обмотці трансвформатора струму. Зараз стоїть 75 кОм
R2 - резистор, який перетворює трансформований струм у напругу. Зараз стоїть 499 Ом
1 - після перетворення струму в напругу як мінімум раз сигнвал проходить через підсилювач на операційному підсилювачі з коефіцієнтом підсилення "1"
0xFFF - максимальне число, яке може видати АЦП

Koef = 91,7586087926769289494740396544 (5872/64 = 91,75)

Множення на ці коефіцієнти переводять одиниці АЦП у мВ

Але коли ми проводимо розрахунки з допомогою перетворення Фур'є, то результатом є амплітуда 1-ої  агмоніки (або інших гармонік. якщо розрахунок би йшов по них)
Тому крім цього коефіцієнта треба ще поділити на корінь з двох.
для простоти зробимо узагальнений коефіцієнт, що є результатом Koef_1 = Koef/sqrt(2)

Koef_1 = 64,883134509545420915167731259667 (4152/64 = 64,875)
*/

/*
R2-change to 86,6
*/
#define MNOGNYK_U 1695
#define VAGA_DILENNJA_U 4
#define MNOGNYK_U_DIJUCHE 2405
#define VAGA_DILENNJA_U_DIJUCHE 5

#define MAX_INDEX_DATA_FOR_OSCYLOGRAPH 10

#define GND_NORMAL_VALUE 0x0
#define VREF_NORMAL_VALUE 0x800
#define VDD_NORMAL_VALUE 0xb00

#define NUMBER_I2G_CANALES 3

//Канали
#define VREF1 0
#define C_IA_1_16 1
#define C_IA_1 2
#define UAB_TN2_16 3
#define UAB_TN2 4
#define UAB_TN1_16 5
#define UAB_TN1 6
#define UP1_P2_C_16 7
#define UP1_P2_C 8
#define UP2_P3_C_16 9
#define UP2_P3_C 10
#define UC1_C2_C_16 11
#define UC1_C2_C 12
#define V_K_3_3 13
#define C_IA_2 14
#define C_IA_2_16 15

#define READ_UP1_P2_C ( \
  (1 << UP1_P2_C) |     \
  (1 << UP1_P2_C_16))

#define READ_UP2_P3_C ( \
  (1 << UP2_P3_C) |     \
  (1 << UP2_P3_C_16))

#define READ_UC1_C2_C ( \
  (1 << UC1_C2_C) |     \
  (1 << UC1_C2_C_16))

#define READ_UAB_TN1 ( \
  (1 << UAB_TN1) |     \
  (1 << UAB_TN1_16))

#define READ_UAB_TN2 ( \
  (1 << UAB_TN2) |     \
  (1 << UAB_TN2_16))

#define READ_I (     \
  (1 << C_IA_1) |    \
  (1 << C_IA_1_16) | \
  (1 << C_IA_2) |    \
  (1 << C_IA_2_16))

#define READ_DATA_VAL ( \
  READ_I |              \
  READ_UAB_TN1 |        \
  READ_UAB_TN2 |        \
  READ_UP1_P2_C |       \
  READ_UP2_P3_C |       \
  READ_UC1_C2_C)

#define READ_TEST_VAL ( \
  (1 << VREF1) |        \
  (1 << V_K_3_3))

#define DATA_VAL_READ_BIT 0
#define DATA_VAL_READ (1 << DATA_VAL_READ_BIT)
#define TEST_VAL_READ_BIT 1
#define TEST_VAL_READ (1 << TEST_VAL_READ_BIT)

#define NUMBER_VREF_ADC 1

#define NUMBER_GND_ADC 3

#define NUMBER_VREF_ADC2 1

#define N_VAL_1 0

#define I_UC1C2 0
#define I_UP1P2 1
#define I_IA_2 2
#define I_IA_1 3
#define I_UAB_TN2 4
#define I_UAB_TN1 5
#define I_UP2P3 6

enum _index_meas
{
  IM_IA_1 = 0,
  IM_IA_2,
  IM_UAB_TN1,
  IM_UAB_TN2,
  IM_UP1P2,
  IM_UP2P3,
  IM_UC1C2,

  _NUMBER_IM
};

#define PORIG_FOR_FAPCH 10000

/*****************************************/
//Константи для фазової корекції між ТН1
/*****************************************/
enum _PhK
{
  INDEX_PhK_UAB_TN1 = 0,
  INDEX_PhK_UAB_TN2,

  MAX_INDEX_PhK
};
/*****************************************/

#define PORIG_FOR_FAPCH 10000

enum _full_ort_index
{
  FULL_ORT_UAB_TN1 = 0,
  FULL_ORT_UAB_TN2,
  FULL_ORT_UP1P2,
  FULL_ORT_UP2P3,
  FULL_ORT_UC1C2,
  FULL_ORT_IA_1,
  FULL_ORT_IA_2,

  FULL_ORT_MAX
};

/*****************************************/
//Константи для усереднення частоти і ШЗЧ
/*****************************************/
#define N_F_AVER 4
/*****************************************/

#endif
