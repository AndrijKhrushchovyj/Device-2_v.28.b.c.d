#ifndef __TYPE_DEFINITION__
#define __TYPE_DEFINITION__

#define SRAM1 _Pragma("location=\"variables_RAM1\"")
#define SRAM1_AR _Pragma("location=\"Analog_Registrator_Buffer\"")

typedef struct
{
  int current_level; //Текучий рівень відображуваного екрану

  int index_position; //Вказує на якій ми зараз є позиції

  int position_cursor_x; //X - координата курсору
  int position_cursor_y; //Y - координата курсору

  unsigned int edition; //0 - вікно зараз не в режимі редагується;
                        //1 - вікно зараз в режимі редагування;
                        //2 - вікно чекає підтвердження редагування
                        //3 - у вікні виведено повідомлення про те, що під час введення даних була зафіксована помилка величиниданих

  unsigned int cursor_on;          //1 - курсор відображається/ 0 - курсор не відображається
  unsigned int cursor_blinking_on; //1 - курсор мигає/ 0 - курсор не мигає

  unsigned int current_action; //Дія яку треба виконати з текучим екраном
                               //0 - Нічого не виконувати
                               //1 - Перемістити курсор, ш/або змінити стан його відображення
                               //2 - Повністю обновити інформацію

} __CURRENT_EKRAN;

typedef struct
{
  int position_cursor_x; //X - координата курсору

  unsigned int cursor_on;          //1 - курсор відображається/ 0 - курсор не відображається
  unsigned int cursor_blinking_on; //1 - курсор мигає/ 0 - курсор не мигає
} __PREVIOUS_STATE_CURSOR;

typedef struct
{
  //Тип пристрою
  unsigned int device_id;

  //Дискретні входи
  unsigned int type_of_input;            //Тип дискретного входу 0 - прямий, 1 - інверсний
  unsigned int type_of_input_signal;     //Тип сигналу дискретного входу 0 - постійний , 1 - змінний
  unsigned int dopusk_dv[NUMBER_INPUTS]; //Допуски ДВ

  unsigned int ranguvannja_inputs[N_SMALL * NUMBER_INPUTS]; //Ранжування дискретних входів
  unsigned int ranguvannja_outputs[N_BIG * NUMBER_OUTPUTS]; //Ранжування дискретних вхиодів
  unsigned int ranguvannja_leds[N_BIG * NUMBER_LEDS];       //Ранжування свіотіндикаторів

  unsigned int ranguvannja_analog_registrator[N_BIG];  //Ранжування аналогового реєстратора
  unsigned int ranguvannja_digital_registrator[N_BIG]; //Ранжування дискретного реєстратора

  int timeout_prolongation_work_digital_registrator; //Час продолвження роботи  дискретного реєстратора

  //Тип дискретних виходів
  unsigned int type_of_output; //Тип дискретних виходів
                               //0 - Командний
                               //1 - Сигналький (постійний або імпульсний)

  unsigned int type_of_output_modif; //Тип дискретних сигнальних виходів
                                     //0 - Сигналький (постійний)
                                     //1 - Сигналький (імпульсний)

  //Тип світлоіндикаторів
  unsigned int type_of_led; //Тип світлоіндикаторі
                            //0 - Нормальний
                            //1 - Тригерний

  unsigned int number_iteration_el; //Максимальна кількість ітерацій для розширеної логіки
                                    //  unsigned int number_defined_df;                                               //Кількість визначуваних функцій у конфігурації приладу
                                    //  unsigned int number_defined_dt;                                               //Кількість визначуваних тригерів у конфігурації приладу
                                    //  unsigned int number_defined_and;                                              //Кількість визначуваних "І" у конфігурації приладу
                                    //  unsigned int number_defined_or;                                               //Кількість визначуваних "АБО" у конфігурації приладу
                                    //  unsigned int number_defined_xor;                                              //Кількість визначуваних "Викл.ВБО" у конфігурації приладу
                                    //  unsigned int number_defined_not;                                              //Кількість визначуваних "НЕ" у конфігурації приладу

  //Опреділювальні функції
  unsigned int type_df;                                                       //Тип опреділювальної функції
                                                                              //0 - Пряма
                                                                              //1 - Зворотня
  int timeout_pause_df[NUMBER_DEFINED_FUNCTIONS];                             //Час затримки спрацюваня опреділювальної функції
  int timeout_work_df[NUMBER_DEFINED_FUNCTIONS];                              //Час роботи опреділювальної функції
  unsigned int ranguvannja_df_source_plus[N_BIG * NUMBER_DEFINED_FUNCTIONS];  //Ранжування прямих команд опреділювальниї функцій
  unsigned int ranguvannja_df_source_minus[N_BIG * NUMBER_DEFINED_FUNCTIONS]; //Ранжування інверсних команд опреділювальниї функцій
  unsigned int ranguvannja_df_source_blk[N_BIG * NUMBER_DEFINED_FUNCTIONS];   //Ранжування команд блокування опреділювальниї функцій
  unsigned int ranguvannja_df_source_reset[N_BIG * NUMBER_DEFINED_FUNCTIONS]; //Ранжування команд скидання таймерів опреділювальниї функцій

  unsigned int ranguvannja_set_dt_source_plus[N_BIG * NUMBER_DEFINED_TRIGGERS];    //Ранжування прямих команд встановлення опреділювальних триґерів
  unsigned int ranguvannja_set_dt_source_minus[N_BIG * NUMBER_DEFINED_TRIGGERS];   //Ранжування інверсних команд встановлення опреділювальних триґерів
  unsigned int ranguvannja_reset_dt_source_plus[N_BIG * NUMBER_DEFINED_TRIGGERS];  //Ранжування прямих команд скидання опреділювальних триґерів
  unsigned int ranguvannja_reset_dt_source_minus[N_BIG * NUMBER_DEFINED_TRIGGERS]; //Ранжування інверсних команд скидання опреділювальних триґерів

  unsigned int ranguvannja_d_and[N_BIG * NUMBER_DEFINED_AND]; //Ранжування команд джерел визначуваних "І"
  unsigned int ranguvannja_d_or[N_BIG * NUMBER_DEFINED_OR];   //Ранжування команд джерел визначуваних "АБО"
  unsigned int ranguvannja_d_xor[N_BIG * NUMBER_DEFINED_XOR]; //Ранжування команд джерел визначуваних "Викл.АБО"
  unsigned int ranguvannja_d_not[N_BIG * NUMBER_DEFINED_NOT]; //Ранжування команд джерел визначуваних "НЕ"

  uint32_t ranguvannja_tf[NUMBER_TRANSFER_FUNCTIONS]; //Ранжування передавальних функцій

  uint32_t buttons_mode;                                              //Режими роботи ФК
                                                                      //0 - Звичайна кнопка
                                                                      //1 - Двопозиційний ключ
  unsigned int ranguvannja_buttons[N_SMALL * NUMBER_DEFINED_BUTTONS]; //Ранжування опреділюваних кнопок

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
  uint32_t ranguvannja_In_GOOSE[N_IN_GOOSE][N_IN_GOOSE_MMS_OUT][N_SMALL];          //Ранжування виходів Вхідних GOOSE блоків
  uint32_t ranguvannja_In_MMS[N_IN_MMS][N_IN_GOOSE_MMS_OUT][N_SMALL];              //Ранжування виходів Вхідних MMS блоків
  uint16_t ranguvannja_Out_LAN[N_OUT_LAN][N_OUT_LAN_IN][MAX_FUNCTIONS_IN_OUT_LAN]; //Ранжування входів  Вихідних мережевих блоків
#endif

  unsigned int configuration; //Конфігурація приладу

  unsigned int grupa_ustavok; //Група уставок
                              // 1 - Група 1
                              // 2 - Група 2
                              // 3 - Група 3
                              // 4 - Група 4

  //РПН
  unsigned int setpoint_rpn_osn[NUMBER_GROUP_USTAVOK];  //Уставка РПН: "Напруга основна" (В)
  unsigned int setpoint_rpn_zony[NUMBER_GROUP_USTAVOK]; //Уставка РПН: "Ширина зони" (xx,xx %)- з точністю до сотих
  unsigned int setpoint_rpn_per[NUMBER_GROUP_USTAVOK];  //Уставка РПН: "Максимальна кількісьл переключень"
  unsigned int setpoint_rpn_dod[NUMBER_GROUP_USTAVOK];  //Уставка РПН: "Напруга допом." (В)
  unsigned int setpoint_rpn_K[NUMBER_GROUP_USTAVOK];    //Уставка РПН: "Струмова компенсація"

  int timeout_rpn_ub_pryb[NUMBER_GROUP_USTAVOK];        //Витримка РПН: "Час Зн./Підв."
  int timeout_rpn_ub_pryb_prysk[NUMBER_GROUP_USTAVOK];  //Витримка РПН: "Зн./Підв. Приск."
  int timeout_rpn_prysk_vid_Umax[NUMBER_GROUP_USTAVOK]; //Витримка РПН: "Час пр.ЗНмакс1"
  int timeout_rpn_perekl[NUMBER_GROUP_USTAVOK];         //Витримка РПН: "Час перемик."
  int timeout_rpn_nespr[NUMBER_GROUP_USTAVOK];          //Витримка РПН: "Час неспр."

  unsigned int control_rpn; //Поле для управління РПН

  //ЗСХ
  unsigned int control_zskh; //Поле для управління ЗСХ

  //ЗНХ
  int32_t timeout_znkh_perekl[NUMBER_GROUP_USTAVOK]; //Витримка ЗНХ: "Час перемик.РПН"
  unsigned int control_znkh;                         //Поле для управління ЗНХ

  //БРП
  unsigned int setpoint_brp_I_blk[NUMBER_GROUP_USTAVOK]; //Уставка БРП: "Струм блокування"
  unsigned int control_brp;                              //Поле для управління БРП

  //Umin
  unsigned int setpoint_Umin1[NUMBER_GROUP_USTAVOK]; //Уставка ЗНмін: "Уставка ЗНмін1"
  unsigned int setpoint_Umin2[NUMBER_GROUP_USTAVOK]; //Уставка ЗНмін: "Уставка ЗНмін2"

  int timeout_Umin1[NUMBER_GROUP_USTAVOK]; //Витримка  Umin1
  int timeout_Umin2[NUMBER_GROUP_USTAVOK]; //Витримка  Umin2
  unsigned int control_Umin;               //Поле для управління Umin

  //Umax
  unsigned int setpoint_Umax1[NUMBER_GROUP_USTAVOK]; //Уставка ЗНмакс: "Уставка ЗНмакс1"
  unsigned int setpoint_Umax2[NUMBER_GROUP_USTAVOK]; //Уставка ЗНмакс: "Уставка ЗНмакс2"

  int timeout_Umax2[NUMBER_GROUP_USTAVOK]; //Витримка ЗНмакс: "Витримка ЗНмакс2"

  unsigned int control_Umax; //Поле для управління ЗНмак

  //Універсальний захист
  int32_t setpoint_UP[NUMBER_UP][1][NUMBER_GROUP_USTAVOK];     //Уставка для всіх ступенів
  uint32_t setpoint_UP_KP[NUMBER_UP][1][NUMBER_GROUP_USTAVOK]; //Уставка для "Коефіцієнта повернення" всіх ступенів
  int32_t timeout_UP[NUMBER_UP][1][NUMBER_GROUP_USTAVOK];      //Витримка для Універсального захисту
  uint32_t control_UP;                                         //Поля для управління
  int32_t ctrl_UP_input[NUMBER_UP];                            //Вибір входу Універсального захисту

  unsigned int number_steps_rpn;        //Кількість ступенів РПН
  unsigned int number_step_nominal_rpn; //Номер номінального положення РПН
  //Тип контролю положення РПН
  int type_control_location; // 0 - "Логічний"
                             // 1 - "Логометр"
                             // 2 - "Сельсин"

  unsigned int TCurrent1; //Коефіцієнт трансформації для трансформатора струмів 1
  unsigned int TCurrent2; //Коефіцієнт трансформації для трансформатора струмів 2
  unsigned int TVoltage1; //Коефіцієнт трансформації для трансформатора напруг  1
  unsigned int TVoltage2; //Коефіцієнт трансформації для трансформатора напруг  2

  unsigned int password1;                                     //Пароль для редагування з меню
  unsigned int password2;                                     //Пароль для очистки лічилчників енегії і ресурсу вимикача
  unsigned int password3;                                     //Пароль для переходу у режим програмування
  unsigned int timeout_deactivation_password_interface_USB;   //Час деактивації паролю для редагування з інтерфейсу USB
  unsigned int password_interface_USB;                        //Пароль для редагування з інтерфейсу USB
  unsigned int timeout_deactivation_password_interface_RS485; //Час деактивації паролю для редагування з інтерфейсу RS485
  unsigned int password_interface_RS485;                      //Пароль для редагування з інтерфейсу RS485

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
  unsigned int timeout_deactivation_password_interface_LAN; //Час деактивації паролю для редагування з інтерфейсу LAN
  unsigned int password_interface_LAN;                      //Пароль для редагування з інтерфейсу LAN
#endif

  unsigned int timeout_idle_new_settings;

  //Аналоговий реєстратор
  unsigned int prefault_number_periods;  //Час доаварійного масиву (кількість періодів промислової частоти)
  unsigned int postfault_number_periods; //Час післяарійного масиву (кількість періодів промислової частоти)
  unsigned int diskretnt_number_periods; //Дискретність виборок (кількість періодів промислової частоти)
  unsigned int control_ar;               //Поля для управління аналоговим реєстратором

  //Комунікація
  unsigned int name_of_cell[MAX_CHAR_IN_NAME_OF_CELL];                                                            //І'мя ячейки
  unsigned short int user_register[(M_ADDRESS_LAST_USER_REGISTER_DATA - M_ADDRESS_FIRST_USER_REGISTER_DATA) + 1]; //Регістри користувача
  unsigned int address;                                                                                           //Адреса
  int speed_RS485;                                                                                                //швидкість обміну
                                                                                                                  // 0 - 9600
                                                                                                                  // 1 - 14400
                                                                                                                  // 2 - 19200
                                                                                                                  // 3 - 28800
                                                                                                                  // 4 - 38400
                                                                                                                  // 5 - 57600
                                                                                                                  // 6 - 115200
  int pare_bit_RS485;                                                                                             //паритет
                                                                                                                  // 0 - NONE
                                                                                                                  // 1 - ODD
                                                                                                                  // 2 - EVEN
  int number_stop_bit_RS485;                                                                                      //кількість стоп-біт
                                                                                                                  // 0 - 1 stop-bit
                                                                                                                  // 1 - 2 stop-bits
  unsigned int time_out_1_RS485;                                                                                  //time-out наступного символу = X/10 символу

  int language; //мова меню  0= змінна мов не підтримується; 1=RU; 2=UA; 3=EN; 4=KZ; 5=др.

  unsigned int control_extra_settings_1; //Поле для додаткових налаштувань

  int32_t time_zone;     //Часова зона
  uint32_t dst;          //Перехід на літній час
  uint32_t dst_on_rule;  //Правило переходу на Літній час
  uint32_t dst_off_rule; //Правило переходу на стандартний час

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
  //IP4
  uint16_t IP4[4];     //XXX.XXX.XXX.XXX Можна б було обійтися типом в один байт, але для редагування може виходити число 999, тому я вибрав двобайтний тип
  uint32_t mask;       //XX
  uint16_t gateway[4]; //XXX.XXX.XXX.XXX Можна б було обійтися типом в один байт, але для редагування може виходити число 999, тому я вибрав двобайтний тип

  uint16_t IP_time_server[4]; //XXX.XXX.XXX.XXX Можна б було обійтися типом в один байт, але для редагування може виходити число 999, тому я вибрав двобайтний тип
  uint32_t port_time_server;
  uint32_t period_sync;

#endif

  time_t time_setpoints;          //Час останніх змін уставок-витримок-управління
  unsigned char source_setpoints; //0 - мінімальні параметри
                                  //1 - клавіатура
                                  //2 - USB
                                  //3 - RS-485
                                  //4 - Ethernet

  time_t time_ranguvannja;          //Час останніх змін ранжування
  unsigned char source_ranguvannja; //0 - мінімальні параметри
                                    //1 - клавіатура
                                    //2 - USB
                                    //3 - RS-485
                                    //4 - Ethernet
} __SETTINGS;

typedef struct
{
  //Стан обміну
  int state_execution; //-1 - драйвер готовий до початку нових транзакцій
                       // 0 - відбувається обмін
                       // 1 - обмін завершений без помилок
                       // 2 - обмін завершений з помилками

  //Ідентифікатор пристрою, з яким ведетьсяобмін
  int device_id; //-1 - пристрій не визначений
                 //                              // EEPROM_ADDRESS - EEPROM
                 // RTC_ADDRESS - RTC
  //Код операції
  int action; //-1 - не визначений
              // 0 - запис адреси для подальшого зчитування
              // 1 - зчитування
              // 2 - запис адреси і запис даних

  //Внутрішня адреса по якій буде іти звертання
  unsigned int internal_address;

  //Кількість байт для читання/запису (корисних)
  unsigned int number_bytes;

  //Вказівник на буфер (корисний)
  uint8_t *point_buffer;

} __DRIVER_I2C;

typedef struct
{
  unsigned int next_address;
  unsigned int saving_execution;
  unsigned int number_records;
} __INFO_REJESTRATOR;

typedef struct
{
  int first_number;
  int last_number;
} __INFO_AR_REJESTRATOR;

typedef struct _FIL_FATFS
{
  unsigned int opened;
  FIL fil;

} __FIL_FATFS;

typedef struct
{
  unsigned int state_execution; //стан виконуваної заразоперації

  unsigned int code_operation; //Код виконуваної операції

} __DRIVER_SPI_DF;

typedef struct
{
  unsigned char label_start_record;
  time_t time_dat;
  int32_t time_ms;
  unsigned int TCurrent1;
  unsigned int TCurrent2;
  unsigned int TVoltage1;
  unsigned int TVoltage2;
  unsigned char name_of_cell[MAX_CHAR_IN_NAME_OF_CELL];
  unsigned int prefault_number_periods;  //Час доаварійного масиву (кількість періодів промислової частоти)
  unsigned int diskretnt_number_periods; //Дискретність виборок (кількість періодів промислової частоти)
  unsigned int cur_active_sources[N_BIG];
} __HEADER_AR;

typedef struct
{
  unsigned char date[3];
  unsigned int count;
} __COUNTER_RESURS;

typedef enum __STATE_READING_ADCs
{
  STATE_READING_ADCs_NONE = 0,
  STATE_READING_WRITE,
  STATE_READING_WRITE_READ,
  STATE_READING_READ
} STATE_READING_ADCs;

typedef enum _full_ort_index __full_ort_index;
typedef enum _index_energy __index_energy;

typedef struct
{
  uint32_t tick;
  int32_t value;

} EXTENDED_OUTPUT_DATA;

typedef struct
{
  uint32_t tick;
  int value;
} EXTENDED_SAMPLE;

typedef struct
{
  unsigned int x1;
  int y1;

  unsigned int x2;
  int y2;

} VYBORKA_XY;

typedef struct
{
  unsigned int Uab_TN1_x1;
  int Uab_TN1_y1;
  unsigned int Uab_TN1_x2;
  int Uab_TN1_y2;

  unsigned int Uab_TN2_x1;
  int Uab_TN2_y1;
  unsigned int Uab_TN2_x2;
  int Uab_TN2_y2;
} POPEREDNJY_PERECHID;

//typedef struct
//{
//  unsigned int present;
//  int start_index;
//  int stop_index;
//  int number_per_index;
//  int real_number;
//} EL_FILTER_STRUCT;

typedef enum _getting_data
{
  GET_DATA_FOR_EDITING = 0,
  GET_DATA_IMMEDITATE
} __getting_data;

typedef enum _setting_data
{
  SET_DATA_INTO_EDIT_TABLE = 0,
  SET_DATA_IMMEDITATE
} __settings_data;

typedef struct _vd
{
  int sign;
  unsigned int begin;
  unsigned int comma;
  unsigned int end;
  unsigned int u_begin;
  unsigned int u_end;
  const unsigned char *p_unit;
} __vd;

typedef struct _ctrl_info
{
  const uint8_t *information;
  unsigned int cursor_x;
} __ctrl_info;

typedef enum _index_I_U
{
  INDEX_I = 0,
  INDEX_U,

  _NUMBER_FOR_I_U
} __index_I_U;

typedef enum _id_input_output
{
  ID_INPUT = 0,
  ID_OUTPUT,
  ID_LED,
  ID_DF,
  ID_DB,
  ID_DT,
  ID_AND,
  ID_OR,
  ID_XOR,
  ID_NOT,
  ID_TF,

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)

  ID_IN_GOOSE,
  ID_IN_MMS,
  ID_OUT_LAN,
#endif

  _MAX_ID_INPUT_OUPUT

} __id_input_output;

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)

typedef struct _control_info
{
  unsigned int const *const p_ctrl;
  size_t const n_bit;
} __control_info;

#endif

typedef struct _meas_to_d_meas
{
  unsigned int const *const arr;
  size_t const ind;

} __meas_to_d_meas;

typedef struct _test_watchdog
{
  int32_t arrTimeout[8 + 1];
  int32_t timeoutFirst;
  int32_t timeoutLast;
  int l1;
  int l2;
  int32_t timeoutInputMax;
  int InputMux_l1;
  int InputMux_l2;
  unsigned int time_delta_watchdog_output_max;
  int32_t delta;

} __test_watchdog;

#endif
