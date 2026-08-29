#ifndef __GLOBAL_VARIABLES_H
#define __GLOBAL_VARIABLES_H

//Вимірювальна система
unsigned int semaphore_adc_irq;
unsigned int adc_DATA_VAL_read;
unsigned int adc_TEST_VAL_read;
unsigned int status_adc_read_work;
const unsigned int input_adc[NUMBER_INPUTs_ADCs][2] = {
  {1, 0x8370},
  {1, 0x8770},
  {1, 0x8b70},
  {1, 0x8f70},
  {1, 0x9370},
  {1, 0x9770},
  {1, 0x9b70},
  {1, 0x9f70},
  {1, 0xa370},
  {1, 0xa770},
  {1, 0xab70},
  {1, 0xaf70},
  {1, 0xb370},
  {1, 0xb770},
  {1, 0xbb70},
  {1, 0xbf70},
  {2, 0x8010},
  {2, 0x8410}};
EXTENDED_OUTPUT_DATA output_adc[NUMBER_INPUTs_ADCs];

uint32_t tick_output_adc_p;
unsigned int command_word_adc, command_word_adc_work, active_index_command_word_adc;
unsigned int state_reading_ADCs = STATE_READING_ADCs_NONE;
unsigned int channel_request, channel_answer;

uint32_t step_timer_adc = TIM5_CCR1_2_VAL;
uint32_t penultimate_tick_DATA_VAL, previous_tick_DATA_VAL;

VYBORKA_XY perechid_cherez_nul[MAX_INDEX_PhK][2];
unsigned int fix_perechid_cherez_nul[MAX_INDEX_PhK];
POPEREDNJY_PERECHID poperednij_perechid;

unsigned int maska_canaliv_fapch;
float frequency = -1;
float frequency_middle = -1;
unsigned int tick_period = (TIM5_CCR1_2_VAL * NUMBER_POINT), tick_period_work = (TIM5_CCR1_2_VAL * NUMBER_POINT);
unsigned int tick_c, tick_c_work;

float freq_arr[N_F_AVER];
size_t index_freq_arr;
float sum_freq_arr;

float frequency_min = 50, frequency_max = 50;
unsigned int command_restart_monitoring_frequency;

EXTENDED_SAMPLE ADCs_data_raw[NUMBER_ANALOG_CANALES];
int ADCs_data[NUMBER_ANALOG_CANALES];
unsigned long long sqr_current_data_3I0[NUMBER_POINT];

unsigned int changed_ustuvannja = CHANGED_ETAP_NONE;
unsigned char crc_ustuvannja;
unsigned int ustuvannja_meas[NUMBER_ANALOG_CANALES] = {
  DEFAULT_USTUVANNJA_VALUE,
  DEFAULT_USTUVANNJA_VALUE,
  DEFAULT_USTUVANNJA_VALUE,
  DEFAULT_USTUVANNJA_VALUE,
  DEFAULT_USTUVANNJA_VALUE,
  DEFAULT_USTUVANNJA_VALUE,
  DEFAULT_USTUVANNJA_VALUE};
unsigned int ustuvannja[NUMBER_ANALOG_CANALES] = {
  DEFAULT_USTUVANNJA_VALUE,
  DEFAULT_USTUVANNJA_VALUE,
  DEFAULT_USTUVANNJA_VALUE,
  DEFAULT_USTUVANNJA_VALUE,
  DEFAULT_USTUVANNJA_VALUE,
  DEFAULT_USTUVANNJA_VALUE,
  DEFAULT_USTUVANNJA_VALUE};

const float sin_data_f[NUMBER_POINT] = {
  0.000000000000000000000000000000f,
  0.195090322016128000000000000000f,
  0.382683432365090000000000000000f,
  0.555570233019602000000000000000f,
  0.707106781186547000000000000000f,
  0.831469612302545000000000000000f,
  0.923879532511287000000000000000f,
  0.980785280403230000000000000000f,
  1.000000000000000000000000000000f,
  0.980785280403230000000000000000f,
  0.923879532511287000000000000000f,
  0.831469612302545000000000000000f,
  0.707106781186548000000000000000f,
  0.555570233019602000000000000000f,
  0.382683432365090000000000000000f,
  0.195090322016129000000000000000f,
  0.000000000000000122514845490862f,
  -0.195090322016128000000000000000f,
  -0.382683432365090000000000000000f,
  -0.555570233019602000000000000000f,
  -0.707106781186547000000000000000f,
  -0.831469612302545000000000000000f,
  -0.923879532511287000000000000000f,
  -0.980785280403230000000000000000f,
  -1.000000000000000000000000000000f,
  -0.980785280403230000000000000000f,
  -0.923879532511287000000000000000f,
  -0.831469612302545000000000000000f,
  -0.707106781186548000000000000000f,
  -0.555570233019602000000000000000f,
  -0.382683432365090000000000000000f,
  -0.195090322016129000000000000000f};

const float cos_data_f[NUMBER_POINT] = {
  1.000000000000000000000000000000f,
  0.980785280403230000000000000000f,
  0.923879532511287000000000000000f,
  0.831469612302545000000000000000f,
  0.707106781186548000000000000000f,
  0.555570233019602000000000000000f,
  0.382683432365090000000000000000f,
  0.195090322016129000000000000000f,
  0.000000000000000122514845490862f,
  -0.195090322016128000000000000000f,
  -0.382683432365090000000000000000f,
  -0.555570233019602000000000000000f,
  -0.707106781186547000000000000000f,
  -0.831469612302545000000000000000f,
  -0.923879532511287000000000000000f,
  -0.980785280403230000000000000000f,
  -1.000000000000000000000000000000f,
  -0.980785280403230000000000000000f,
  -0.923879532511287000000000000000f,
  -0.831469612302545000000000000000f,
  -0.707106781186548000000000000000f,
  -0.555570233019602000000000000000f,
  -0.382683432365090000000000000000f,
  -0.195090322016129000000000000000f,
  0.000000000000000000000000000000f,
  0.195090322016128000000000000000f,
  0.382683432365090000000000000000f,
  0.555570233019602000000000000000f,
  0.707106781186547000000000000000f,
  0.831469612302545000000000000000f,
  0.923879532511287000000000000000f,
  0.980785280403230000000000000000f};

unsigned int index_sin_cos_array;
unsigned int index_data_sin_cos_array;
int data_sin[NUMBER_POINT * NUMBER_ANALOG_CANALES];
int data_cos[NUMBER_POINT * NUMBER_ANALOG_CANALES];
int ortogonal_irq[2 * NUMBER_ANALOG_CANALES];
int ortogonal[2 * NUMBER_ANALOG_CANALES][2];
unsigned int bank_ortogonal;
unsigned int semaphore_measure_values_low;

unsigned int vref_moment_value[NUMBER_POINT];
unsigned int vref_averange_sum = VREF_NORMAL_VALUE * NUMBER_POINT;
volatile unsigned int vref_averange = VREF_NORMAL_VALUE;
unsigned int v_k_3_3_moment_value[NUMBER_POINT];
unsigned int v_k_3_3_averange_sum = V_K_3_3_NORMAL_VALUE * NUMBER_POINT;
unsigned int v_k_3_3_averange = V_K_3_3_NORMAL_VALUE;
int index_array_of_one_value = 0;
//Змінна відповідає за признак завершення перших 32-ти зчитувань з АЦП
unsigned int completion_of_first_period = 0;

int adc2_channel0 = 0; //Напруга на логометрі
int adc2_channel0_moment_value[NUMBER_POINT];
int adc2_channel0_averange_sum = 0;
int adc2_channel0_averange_irq = 0;
int adc2_channel0_averange[2] = {0, 0};
int adc2_channel0_averange_prt = 0;
int adc2_channel0_averange_low = 0;
int adc2_channel1 = 0; //Напруга з виході логометра
int adc2_channel1_moment_value[NUMBER_POINT];
int adc2_channel1_averange_sum = 0;
int adc2_channel1_averange_irq = 0;
int adc2_channel1_averange[2] = {0, 0};
int adc2_channel1_averange_prt = 0;
int adc2_channel1_averange_low = 0;
unsigned int adc2_read_after_start = false;

unsigned int number_inputs_for_fix_one_second;
unsigned int number_inputs_for_fix_one_period;
unsigned int measurement[_NUMBER_IM];
unsigned int measurement_high[2][_NUMBER_IM], bank_measurement_high = 0;
unsigned int measurement_middle[_NUMBER_IM];
SRAM1 unsigned int measurement_low[_NUMBER_IM];

int angle_UP1P2_UC1C2;
int angle_UP2P3_UC1C2;
unsigned int sum_phi_begin = 0;
unsigned int sum_phi_end = 360;
unsigned char crc_angle;

const unsigned int index_converter[NUMBER_ANALOG_CANALES] = {FULL_ORT_UAB_TN1, FULL_ORT_UAB_TN2, FULL_ORT_UP1P2, FULL_ORT_UP2P3, FULL_ORT_UC1C2, FULL_ORT_IA_1, FULL_ORT_IA_2};
int ortogonal_calc[2 * FULL_ORT_MAX];
int ortogonal_calc_low[2 * FULL_ORT_MAX];
int phi_angle[2][FULL_ORT_MAX];
uint32_t bank_for_calc_phi_angle, state_calc_phi_angle;
int base_index_for_angle = -1;

//Конвертація "короткої" таблиці ранжування у "довгу"
const uint32_t small_big_rang[NUMBER_TOTAL_SIGNAL_FOR_RANG_SMALL] =
  {
    RANG_RESET_LEDS,
    RANG_RESET_RELES,
    RANG_MISCEVE_DYSTANCIJNE,
    RANG_1_GRUPA_USTAVOK,
    RANG_2_GRUPA_USTAVOK,
    RANG_3_GRUPA_USTAVOK,
    RANG_4_GRUPA_USTAVOK,
    RANG_BCD_0_BIT,
    RANG_BCD_1_BIT,
    RANG_BCD_2_BIT,
    RANG_BCD_3_BIT,
    RANG_BCD_4_BIT,
    RANG_BCD_5_BIT,

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
    RANG_BLOCK_IN_GOOSE1 + 0,
    RANG_BLOCK_IN_GOOSE1 + 1,
    RANG_BLOCK_IN_GOOSE1 + 2,
    RANG_BLOCK_IN_GOOSE1 + 3,
    RANG_BLOCK_IN_GOOSE1 + 4,
    RANG_BLOCK_IN_GOOSE1 + 5,
    RANG_BLOCK_IN_GOOSE1 + 6,
    RANG_BLOCK_IN_GOOSE1 + 7,
    RANG_BLOCK_IN_GOOSE1 + 8,
    RANG_BLOCK_IN_GOOSE1 + 9,
    RANG_BLOCK_IN_GOOSE1 + 10,
    RANG_BLOCK_IN_GOOSE1 + 11,
    RANG_BLOCK_IN_GOOSE1 + 12,
    RANG_BLOCK_IN_GOOSE1 + 13,
    RANG_BLOCK_IN_GOOSE1 + 14,
    RANG_BLOCK_IN_GOOSE1 + 15,

    RANG_BLOCK_IN_MMS1 + 0,
    RANG_BLOCK_IN_MMS1 + 1,
    RANG_BLOCK_IN_MMS1 + 2,
    RANG_BLOCK_IN_MMS1 + 3,

    RANG_BLOCK_OUT_LAN1 + 0,
    RANG_BLOCK_OUT_LAN1 + 1,
    RANG_BLOCK_OUT_LAN1 + 2,
    RANG_BLOCK_OUT_LAN1 + 3,

#endif

    RANG_OSNOVNYJ_TN2_RPN,
    RANG_MRZS_OR_LOCAL_MODE_RPN,
    RANG_TM_MODE_VID_DV_RPN,
    RANG_AUTO_MODE_FROM_DV_RPN,
    RANG_PEREKLYUCHENNYA_RPN,
    RANG_UBAVYTY_FROM_DV_RPN,
    RANG_PRYBAVYTY_FROM_DV_RPN,
    RANG_BLOCK_STRUM_KOMP_RPN,
    RANG_ZOVNISHNJE_BLOCKUVANNJA_RPN,
    RANG_1_POLOGENNJA_RPN,
    RANG_NOMINALNA_POZYCIJA_RPN,
    RANG_N_POLOGENNJA_RPN,
    RANG_CLEAR_BLK_RPN,
    RANG_UBAVYTY_FROM_KB_OR_UPPER_LEVEL_RPN,
    RANG_PRYBAVYTY_FROM_KB_OR_UPPER_LEVEL_RPN,

    RANG_KONTROL_UBAVYTY_ZSKh,
    RANG_KONTROL_PRYBAVYTY_ZSKh,

    RANG_BLOCK_BRP,

    RANG_BLOCK_UMAX2,

    RANG_BLOCK_UMIN1,
    RANG_BLOCK_UMIN2,

    (RANG_BLOCK_UP1 + 3 * 0),
    (RANG_BLOCK_UP1 + 3 * 1),
    (RANG_BLOCK_UP1 + 3 * 2),
    (RANG_BLOCK_UP1 + 3 * 3),
    (RANG_BLOCK_UP1 + 3 * 4),
    (RANG_BLOCK_UP1 + 3 * 5),
    (RANG_BLOCK_UP1 + 3 * 6),
    (RANG_BLOCK_UP1 + 3 * 7),

    RANG_DF1_IN,
    RANG_DF1_R,
    RANG_DF2_IN,
    RANG_DF2_R,
    RANG_DF3_IN,
    RANG_DF3_R,
    RANG_DF4_IN,
    RANG_DF4_R,
    RANG_DF5_IN,
    RANG_DF5_R,
    RANG_DF6_IN,
    RANG_DF6_R,
    RANG_DF7_IN,
    RANG_DF7_R,
    RANG_DF8_IN,
    RANG_DF8_R,
    RANG_DT1_SET,
    RANG_DT1_RESET,
    RANG_DT2_SET,
    RANG_DT2_RESET,
    RANG_DT3_SET,
    RANG_DT3_RESET,
    RANG_DT4_SET,
    RANG_DT4_RESET,
    RANG_LF1,
    RANG_LF2,
    RANG_LF3,
    RANG_LF4,
    RANG_LF5,
    RANG_LF6,
    RANG_LF7,
    RANG_LF8,
    RANG_LF9,
    RANG_LF10,
    RANG_LF11,
    RANG_LF12,
    RANG_LF13,
    RANG_LF14,
    RANG_LF15,
    RANG_LF16

};

uint32_t TIM_PRT_write_tick;

//Світлоіндикатори
unsigned int state_leds;
unsigned int state_trigger_leds;
uint32_t state_leds_Fx[2];
uint32_t state_leds_ctrl;

unsigned int active_inputs_lock_guard;
unsigned int active_inputs_lock_guard_prt;
unsigned int active_inputs_prt; //"вхід активний" - відповідає встановленому біту (1); "вхід пасивний" - відповідає скинутому біту (0)
unsigned int active_inputs_low; //"вхід активний" - відповідає встановленому біту (1); "вхід пасивний" - відповідає скинутому біту (0)
unsigned int state_outputs;
unsigned int state_outputs_raw, state_outputs_raw_dbg;
unsigned int state_signal_outputs;

unsigned int active_functions[N_BIG];
unsigned int trigger_active_functions[N_BIG], trigger_active_functions_ctrl[N_BIG];
unsigned char crc_trg_func, crc_trg_func_ctrl;
unsigned int trigger_functions_USB[N_BIG];
unsigned int trigger_functions_RS485[N_BIG];
unsigned int copying_active_functions;
unsigned int active_functions_copy[N_BIG];
unsigned int active_functions_for_lower_moduls[N_BIG];
unsigned int copying_active_functions_for_lower_moduls;
unsigned int mutex_buttons;
unsigned int pressed_buttons;
unsigned int fix_active_buttons, fix_active_buttons_ctrl;
unsigned int mutex_interface;
unsigned int activation_function_from_interface[N_SMALL];
unsigned int dostup_z_VR;
unsigned int reset_trigger_function_from_interface;
unsigned int diagnostyka_before[N_DIAGN];
volatile unsigned int diagnostyka[N_DIAGN];
unsigned int set_diagnostyka[N_DIAGN];
unsigned int clear_diagnostyka[N_DIAGN];

uint32_t board_register;

int global_timers[_MAX_NUMBER_GLOBAL_TIMERS]; //Масив глобальних таймерів
unsigned int timer_prt_signal_output_mode_2;
unsigned int output_timer_prt_signal_output_mode_2;
unsigned int static_logic_df;

unsigned int trigger_ZNKh_0 = 0;
unsigned int trigger_C_inputs_ZNKh_0 = 0;

unsigned int trigger_ZSKh_0 = 0;
unsigned int trigger_C_inputs_ZSKh_0 = 0;

unsigned int trigger_TM_0 = 0, trigger_TM_0_for_save = 0, trigger_TM_0_for_save_ctrl;
unsigned int previous_states_TM_0 = 0;

unsigned int trigger_RPN_0 = 0;
unsigned int previous_states_RPN_0 = 0;
unsigned int exchange_RPN_0 = 0;
unsigned int count_RPN_1 = 0;
int timer_rpn_ub_pryb_low = -1;
int timer_rpn_ub_pryb_prysk_low = -1;
int timer_rpn_prysk_vid_Umax_low = -1;
unsigned int znyzyty_bulo_pered_perecluchennjam = false;
unsigned int pidvyshchyty_bulo_pered_perecluchennjam = false;

unsigned int trigger_ctrl_pol_0 = 0;
unsigned int trigger_C_inputs_ctrl_pol_0 = 0;

_Bool previous_states_ready_tu;
_Bool trigger_ready_tu;

unsigned int start_restart = 0xff;

unsigned int number_group_stp_low;

int current_step_logical = NUMBER_STEPS_RPN_UNDEFINED, edition_current_step_logical;
int current_step = NUMBER_STEPS_RPN_UNDEFINED;

SRAM1 unsigned char working_ekran[MAX_ROW_LCD][MAX_COL_LCD];
SRAM1 unsigned int rewrite_ekran_once_more;

SRAM1 volatile unsigned int new_state_keyboard;
SRAM1 unsigned int new_state_keyboard_for_db;
SRAM1 unsigned char time_set_keyboard[NUMBER_KEY_KEYBOARD];

SRAM1 unsigned int time_rewrite; //Час який пройшов після останнього обновлення
SRAM1 unsigned int reinit_LCD;

SRAM1 __CURRENT_EKRAN current_ekran;
SRAM1 int position_in_current_level_menu[MAX_LEVEL_MENU];       //Масив у якому збкрігається індекс текучоїпозиції
SRAM1 int previous_level_in_current_level_menu[MAX_LEVEL_MENU]; //Масив у якому збкрігається занчення попередніх екранів для даного рівня меню
const uint32_t binary_input_signals[N_SMALL] =
  {
    MASKA_INPUT_SIGNALS_0,
    MASKA_INPUT_SIGNALS_1,
    MASKA_INPUT_SIGNALS_2
#ifdef MASKA_INPUT_SIGNALS_3
    ,
    MASKA_INPUT_SIGNALS_3
#endif
};
const uint32_t buttons_mode[NUMBER_BUTTON_MODE][N_SMALL] =
  {
    {MASKA_BUTTON_MODE_0_SIGNALS_0,
     MASKA_BUTTON_MODE_0_SIGNALS_1,
     MASKA_BUTTON_MODE_0_SIGNALS_2
#ifdef MASKA_BUTTON_MODE_0_SIGNALS_3
     ,
     MASKA_BUTTON_MODE_0_SIGNALS_3
#endif
    },
    {MASKA_BUTTON_MODE_1_SIGNALS_0,
     MASKA_BUTTON_MODE_1_SIGNALS_1,
     MASKA_BUTTON_MODE_1_SIGNALS_2
#ifdef MASKA_BUTTON_MODE_1_SIGNALS_3
     ,
     MASKA_BUTTON_MODE_1_SIGNALS_3
#endif
    }};

const uint32_t output_boards[N_OUTPUT_BOARDS][2] =
  {
    {2, 1},
#if (                             \
  (MODYFIKACIA_VERSII_PZ == 8) || \
  (MODYFIKACIA_VERSII_PZ == 18))
    {20, 5}
#else
    {9, 4}
#endif
#if (                              \
  (MODYFIKACIA_VERSII_PZ == 0) ||  \
  (MODYFIKACIA_VERSII_PZ == 1) ||  \
  (MODYFIKACIA_VERSII_PZ == 3) ||  \
  (MODYFIKACIA_VERSII_PZ == 5) ||  \
  (MODYFIKACIA_VERSII_PZ == 10) || \
  (MODYFIKACIA_VERSII_PZ == 11) || \
  (MODYFIKACIA_VERSII_PZ == 13) || \
  (MODYFIKACIA_VERSII_PZ == 15) || \
  (MODYFIKACIA_VERSII_PZ == 23) || \
  (MODYFIKACIA_VERSII_PZ == 33))
    ,
    {16, 5}

#if (                             \
  (MODYFIKACIA_VERSII_PZ == 5) || \
  (MODYFIKACIA_VERSII_PZ == 15))
    ,
    {20, 7}
#endif

#elif (                           \
  (MODYFIKACIA_VERSII_PZ == 6) || \
  (MODYFIKACIA_VERSII_PZ == 26))
    ,
    {13, 5}

#elif (                           \
  (MODYFIKACIA_VERSII_PZ == 7) || \
  (MODYFIKACIA_VERSII_PZ == 17))
    ,
    {12, 5}

#elif (                           \
  (MODYFIKACIA_VERSII_PZ == 8) || \
  (MODYFIKACIA_VERSII_PZ == 18))
    ,
    {24, 7}
#endif
};

const uint32_t input_boards[N_INPUT_BOARDS][2] =
  {
#if (                             \
  (MODYFIKACIA_VERSII_PZ == 8) || \
  (MODYFIKACIA_VERSII_PZ == 18))
    {28, 4}
#else
    {8, 4}
#endif
#if (                              \
  (MODYFIKACIA_VERSII_PZ == 0) ||  \
  (MODYFIKACIA_VERSII_PZ == 1) ||  \
  (MODYFIKACIA_VERSII_PZ == 3) ||  \
  (MODYFIKACIA_VERSII_PZ == 5) ||  \
  (MODYFIKACIA_VERSII_PZ == 10) || \
  (MODYFIKACIA_VERSII_PZ == 11) || \
  (MODYFIKACIA_VERSII_PZ == 13) || \
  (MODYFIKACIA_VERSII_PZ == 15) || \
  (MODYFIKACIA_VERSII_PZ == 23) || \
  (MODYFIKACIA_VERSII_PZ == 33))
    ,
    {16, 5}

#if (                              \
  (MODYFIKACIA_VERSII_PZ == 0) ||  \
  (MODYFIKACIA_VERSII_PZ == 5) ||  \
  (MODYFIKACIA_VERSII_PZ == 10) || \
  (MODYFIKACIA_VERSII_PZ == 15))
    ,
    {20, 7}
#endif

#elif (                           \
  (MODYFIKACIA_VERSII_PZ == 6) || \
  (MODYFIKACIA_VERSII_PZ == 26))
    ,
    {12, 5}

#elif (                           \
  (MODYFIKACIA_VERSII_PZ == 7) || \
  (MODYFIKACIA_VERSII_PZ == 17))
    ,
    {13, 5}

#elif (                           \
  (MODYFIKACIA_VERSII_PZ == 8) || \
  (MODYFIKACIA_VERSII_PZ == 18))
    ,
    {32, 7}
#endif
};

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)

const uint32_t index_n_In_GOOSE[MAX_NAMBER_LANGUAGE][1] =
  {
    {14},
    {14},
    {10},
    {14}};

const uint32_t index_n_In_MMS[MAX_NAMBER_LANGUAGE][1] =
  {
    {13},
    {13},
    {10},
    {13}};

const uint32_t index_n_Out_LAN[MAX_NAMBER_LANGUAGE][1] =
  {
    {11},
    {11},
    {9},
    {11}};

SRAM1 unsigned int edit_rang_Out_LAN[N_BIG];

const uint32_t rang_iec61850_blocks[2][N_SMALL] =
  {
    {MASKA_IN_GOOSE_SIGNALS_0,
     MASKA_IN_GOOSE_SIGNALS_1,
     MASKA_IN_GOOSE_SIGNALS_2,
     MASKA_IN_GOOSE_SIGNALS_3},
    {MASKA_IN_MMS_SIGNALS_0,
     MASKA_IN_MMS_SIGNALS_1,
     MASKA_IN_MMS_SIGNALS_2,
     MASKA_IN_MMS_SIGNALS_3}};

#endif

const uint32_t index_number_UP[MAX_NAMBER_LANGUAGE][3] =
  {
    {11, 10, 9},
    {11, 10, 9},
    {6, 6, 9},
    {11, 10, 9}};

unsigned int periodical_tasks_TEST_SETTINGS;
unsigned int periodical_tasks_TEST_USTUVANNJA;
unsigned int periodical_tasks_TEST_TRG_FUNC;
unsigned int periodical_tasks_TEST_TRG_FUNC_LOCK;
unsigned int periodical_tasks_TEST_INFO_REJESTRATOR_AR;
unsigned int periodical_tasks_TEST_INFO_REJESTRATOR_AR_LOCK;
unsigned int periodical_tasks_TEST_INFO_REJESTRATOR_DR;
unsigned int periodical_tasks_TEST_INFO_REJESTRATOR_DR_LOCK;
unsigned int periodical_tasks_TEST_INFO_REJESTRATOR_PR_ERR;
unsigned int periodical_tasks_TEST_INFO_REJESTRATOR_PR_ERR_LOCK;
unsigned int periodical_tasks_CALC_INTEGRAL_MEASUREMENTS;
unsigned int periodical_tasks_TEST_ANGLE;
unsigned int periodical_tasks_TEST_RESURS;
unsigned int periodical_tasks_TEST_RESURS_LOCK;
unsigned int periodical_tasks_TEST_FLASH_MEMORY;

const unsigned char odynyci_vymirjuvannja[MAX_NAMBER_LANGUAGE][NUMBER_ODYNYCI_VYMIRJUVANNJA] =
  {
    {'А', 'В', '%', 'с'},
    {'А', 'В', '%', 'с'},
    {'A', 'V', '%', 's'},
    {'А', 'В', '%', 'с'}};
unsigned char const Hz[MAX_NAMBER_LANGUAGE][2] = {"Гц", "Гц", "Hz", "Гц"};
const unsigned char ms[MAX_NAMBER_LANGUAGE][2] = {"мс", "мс", "ms", "мс"};

const uint32_t max_value_for_tf[1 + _FIX_NUMBER_PROTECTION][MAX_ROW_LIST_SOURCE_TF] =
  {
    {1 + NUMBER_GENERAL_SIGNAL_FOR_RANG,
     1 + NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL},
    {1 + NUMBER_GENERAL_SIGNAL_FOR_RANG + NUMBER_RPN_SIGNAL_FOR_RANG,
     1 + NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL + NUMBER_RPN_SIGNAL_FOR_RANG_SMALL},
    {1 + NUMBER_GENERAL_SIGNAL_FOR_RANG + NUMBER_RPN_SIGNAL_FOR_RANG + NUMBER_SZKh_SIGNAL_FOR_RANG,
     1 + NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL + NUMBER_RPN_SIGNAL_FOR_RANG_SMALL + NUMBER_SZKh_SIGNAL_FOR_RANG_SMALL},
    {1 + NUMBER_GENERAL_SIGNAL_FOR_RANG + NUMBER_RPN_SIGNAL_FOR_RANG + NUMBER_SZKh_SIGNAL_FOR_RANG + NUMBER_SNKh_SIGNAL_FOR_RANG,
     1 + NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL + NUMBER_RPN_SIGNAL_FOR_RANG_SMALL + NUMBER_SZKh_SIGNAL_FOR_RANG_SMALL + NUMBER_SNKh_SIGNAL_FOR_RANG_SMALL},
    {1 + NUMBER_GENERAL_SIGNAL_FOR_RANG + NUMBER_RPN_SIGNAL_FOR_RANG + NUMBER_SZKh_SIGNAL_FOR_RANG + NUMBER_SNKh_SIGNAL_FOR_RANG + NUMBER_BRP_SIGNAL_FOR_RANG,
     1 + NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL + NUMBER_RPN_SIGNAL_FOR_RANG_SMALL + NUMBER_SZKh_SIGNAL_FOR_RANG_SMALL + NUMBER_SNKh_SIGNAL_FOR_RANG_SMALL + NUMBER_BRP_SIGNAL_FOR_RANG_SMALL},
    {1 + NUMBER_GENERAL_SIGNAL_FOR_RANG + NUMBER_RPN_SIGNAL_FOR_RANG + NUMBER_SZKh_SIGNAL_FOR_RANG + NUMBER_SNKh_SIGNAL_FOR_RANG + NUMBER_BRP_SIGNAL_FOR_RANG + NUMBER_UMAX_SIGNAL_FOR_RANG,
     1 + NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL + NUMBER_RPN_SIGNAL_FOR_RANG_SMALL + NUMBER_SZKh_SIGNAL_FOR_RANG_SMALL + NUMBER_SNKh_SIGNAL_FOR_RANG_SMALL + NUMBER_BRP_SIGNAL_FOR_RANG_SMALL + NUMBER_UMAX_SIGNAL_FOR_RANG_SMALL},
    {1 + NUMBER_GENERAL_SIGNAL_FOR_RANG + NUMBER_RPN_SIGNAL_FOR_RANG + NUMBER_SZKh_SIGNAL_FOR_RANG + NUMBER_SNKh_SIGNAL_FOR_RANG + NUMBER_BRP_SIGNAL_FOR_RANG + NUMBER_UMAX_SIGNAL_FOR_RANG + NUMBER_UMIN_SIGNAL_FOR_RANG,
     1 + NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL + NUMBER_RPN_SIGNAL_FOR_RANG_SMALL + NUMBER_SZKh_SIGNAL_FOR_RANG_SMALL + NUMBER_SNKh_SIGNAL_FOR_RANG_SMALL + NUMBER_BRP_SIGNAL_FOR_RANG_SMALL + NUMBER_UMAX_SIGNAL_FOR_RANG_SMALL + NUMBER_UMIN_SIGNAL_FOR_RANG_SMALL},
    {1 + NUMBER_GENERAL_SIGNAL_FOR_RANG + NUMBER_RPN_SIGNAL_FOR_RANG + NUMBER_SZKh_SIGNAL_FOR_RANG + NUMBER_SNKh_SIGNAL_FOR_RANG + NUMBER_BRP_SIGNAL_FOR_RANG + NUMBER_UMAX_SIGNAL_FOR_RANG + NUMBER_UMIN_SIGNAL_FOR_RANG + NUMBER_UP_SIGNAL_FOR_RANG,
     1 + NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL + NUMBER_RPN_SIGNAL_FOR_RANG_SMALL + NUMBER_SZKh_SIGNAL_FOR_RANG_SMALL + NUMBER_SNKh_SIGNAL_FOR_RANG_SMALL + NUMBER_BRP_SIGNAL_FOR_RANG_SMALL + NUMBER_UMAX_SIGNAL_FOR_RANG_SMALL + NUMBER_UMIN_SIGNAL_FOR_RANG_SMALL + NUMBER_UP_SIGNAL_FOR_RANG_SMALL},
    {1 + NUMBER_GENERAL_SIGNAL_FOR_RANG + NUMBER_RPN_SIGNAL_FOR_RANG + NUMBER_SZKh_SIGNAL_FOR_RANG + NUMBER_SNKh_SIGNAL_FOR_RANG + NUMBER_BRP_SIGNAL_FOR_RANG + NUMBER_UMAX_SIGNAL_FOR_RANG + NUMBER_UMIN_SIGNAL_FOR_RANG + NUMBER_UP_SIGNAL_FOR_RANG + NUMBER_EL_SIGNAL_FOR_RANG,
     1 + NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL + NUMBER_RPN_SIGNAL_FOR_RANG_SMALL + NUMBER_SZKh_SIGNAL_FOR_RANG_SMALL + NUMBER_SNKh_SIGNAL_FOR_RANG_SMALL + NUMBER_BRP_SIGNAL_FOR_RANG_SMALL + NUMBER_UMAX_SIGNAL_FOR_RANG_SMALL + NUMBER_UMIN_SIGNAL_FOR_RANG_SMALL + NUMBER_UP_SIGNAL_FOR_RANG_SMALL + NUMBER_EL_SIGNAL_FOR_RANG_SMALL}};

unsigned int realDateTime;

unsigned char calibration;
unsigned char time_edit[7];
unsigned char calibration_edit;
int etap_reset_of_bit = ETAP_CLEAR_OF_NONE;

char getzone_string[2][55];
size_t bank_getzone;
unsigned int lt_or_utc;
clock_t clk_count;
int32_t time_ms, time_ms_copy;
time_t time_dat, time_dat_copy;
unsigned int copying_time_to_RTC;
int32_t time_ms_RTC;
time_t time_dat_RTC;
unsigned int copying_time_dat;
int32_t time_ms_save_l, time_ms_save_h;
time_t time_dat_save_l, time_dat_save_h;
unsigned int save_time_dat_l, save_time_dat_h;

unsigned int changed_settings = CHANGED_ETAP_NONE;
unsigned char crc_settings;
__SETTINGS current_settings_prt;
unsigned int type_of_input_prt;
unsigned int type_of_input_signal_prt;
unsigned int dopusk_dv_prt[NUMBER_INPUTS];

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
SRAM1 __SETTINGS current_settings;
#else
__SETTINGS current_settings;
#endif
SRAM1 __SETTINGS edition_settings, current_settings_interfaces;

//Визначення періодів у хвилину і більше
unsigned int number_seconds;

//Ресурс++
unsigned int restart_resurs_count;
unsigned int resurs_temp;
unsigned int resurs_global;
unsigned int resurs_global_min;
unsigned int resurs_global_max;

unsigned int restart_timing_watchdog;
unsigned int time_1_watchdog_input;
unsigned int time_2_watchdog_input;
unsigned int time_delta_watchdog_input;
unsigned int time_delta_watchdog_input_min = 0xffff * 10;
unsigned int time_delta_watchdog_input_max;

unsigned int time_1_watchdog_output;
unsigned int time_2_watchdog_output;
unsigned int time_delta_watchdog_output;
unsigned int time_delta_watchdog_output_min = 0xffff * 10;
unsigned int time_delta_watchdog_output_max;

//I2C
unsigned char Temporaty_I2C_Buffer[MAX_NUMBER_REGISTERS_RTC + 2];
unsigned int number_busy_state;
unsigned int type_error_of_exchanging_via_i2c;
//unsigned int low_speed_i2c = 0;
__DRIVER_I2C driver_i2c;
unsigned int control_i2c_taskes[1];
unsigned int state_i2c_task = STATE_FIRST_READING_RTC;
unsigned char read_write_i2c_buffer[SIZE_BUFFER_FOR_I2C];

//Обмін через SPI1
unsigned char TxBuffer_SPI_EDF[SIZE_BUFFER_FOR_EDF];
unsigned char RxBuffer_SPI_EDF[SIZE_BUFFER_FOR_EDF];
int state_execution_spi1 = -1; //-1 - драйвер готовий до початку нових транзакцій; 0 - відбувається обмін; 1 - обмін завершений без помилок
unsigned int control_spi1_taskes[2];
unsigned int comparison_writing; /*очищений біт означає, що іде зчитування у робочий об'єкт, встановлений біт означає що іде порівняння записаної інформації після операції запису*/
unsigned int state_spi1_task;
int etap_eeprom_write_enable = -1;
unsigned int mutex_spi1;

//DataFlash
unsigned int number_bytes_transfer_spi_df;
unsigned int number_bytes_transfer_spi_df_copy;
unsigned char TxBuffer_SPI_DF_copy[SIZE_PAGE_DATAFLASH_MAX + 10];
unsigned int code_operation_copy;
int number_chip_dataflsh_exchange = -1;
__DRIVER_SPI_DF driver_spi_df[NUMBER_DATAFLASH_CHIP] = {
  {TRANSACTION_EXECUTING_NONE, CODE_OPERATION_NONE},
  {TRANSACTION_EXECUTING_NONE, CODE_OPERATION_NONE}};
unsigned int error_into_spi_df; /*Ненульве значення означає, що зафіксована помилка при прийом/передачі через SPI_DF*/
unsigned int dataflash_not_busy;
volatile unsigned int control_tasks_dataflash;
SRAM1 unsigned char buffer_for_manu_read_record[SIZE_BUFFER_FOR_DR_RECORD];
SRAM1 unsigned char buffer_for_USB_read_record_ar[SIZE_PAGE_DATAFLASH_2];
SRAM1 unsigned char buffer_for_RS485_read_record_ar[SIZE_PAGE_DATAFLASH_2];
SRAM1 unsigned char buffer_for_USB_read_record_dr[SIZE_BUFFER_FOR_DR_RECORD];
SRAM1 unsigned char buffer_for_RS485_read_record_dr[SIZE_BUFFER_FOR_DR_RECORD];
SRAM1 unsigned char buffer_for_USB_read_record_pr_err[SIZE_ONE_RECORD_PR_ERR];
SRAM1 unsigned char buffer_for_RS485_read_record_pr_err[SIZE_ONE_RECORD_PR_ERR];

unsigned int what_we_are_reading_from_dataflash_1;

//FATFS
uint32_t FATFS_command;

//Аналоговий реєстратор
unsigned int start_ar;
unsigned char crc_info_rejestrator_ar;
__INFO_AR_REJESTRATOR info_rejestrator_ar;
unsigned char crc_info_rejestrator_ar_ctrl;
__INFO_AR_REJESTRATOR info_rejestrator_ar_ctrl;
unsigned int forbidden_new_record_ar_mode_0 /*= 0*/;
unsigned int state_ar_record_prt = STATE_AR_NONE_PRT, state_ar_record_fatfs = STATE_AR_NONE_FATFS;
unsigned int prev_state_ar_record_prt = STATE_AR_NONE_PRT;
SRAM1_AR short int array_ar[SIZE_BUFFER_FOR_AR];
SRAM1 short int volatile word_SRAM1;
unsigned int index_array_ar_current;
unsigned int index_array_ar_heat;
unsigned int index_array_ar_tail;
unsigned char tail_to_heat, current_to_tail;
int diff_index_heat_tail;  /*ця змінна поки використовується мною тільки для діагностики*/
unsigned int prescaler_ar; //Потрібний для того, щоб з 32 виборок на секунду зробити 16 виборки на секунду
__HEADER_AR header_ar;
unsigned char buffer_for_fs[SIZE_PAGE_DATAFLASH_2];
unsigned int fs_temporary_address;
volatile unsigned int fs_count_to_transfer;
unsigned int etap_writing_part_page_fs_into_dataflash = ETAP_NONE;
int number_record_of_ar_for_menu = -1; //Це число означає, що номер запису не вибраний
int number_record_of_ar_for_USB = -1;  //Це число означає, що номер запису не вибраний
char id_ar_record_for_USB[8 + 1 + 3 + 1];
int max_number_time_sample_USB;
int number_record_of_ar_for_RS485 = -1; //Це число означає, що номер запису не вибраний
int first_number_time_sample_for_USB;   // -1 - заголовок запису ан.р.; 0 - перший часовий зріз доаварійного масиву і т.д.
int last_number_time_sample_for_USB;    // -1 - заголовок запису ан.р.; 0 - перший часовий зріз доаварійного масиву і т.д.
int first_number_time_sample_for_RS485; // -1 - заголовок запису ан.р.; 0 - перший часовий зріз доаварійного масиву і т.д.
int last_number_time_sample_for_RS485;  // -1 - заголовок запису ан.р.; 0 - перший часовий зріз доаварійного масиву і т.д.
char id_ar_record_for_RS485[8 + 1 + 3 + 1];
int max_number_time_sample_RS485;

int32_t timePowerDown = -1;
int32_t timePowerDown_total = -1;
unsigned int truncPrefault;
unsigned int index_array_tail_min;
enum _fix_date_time_avar arDateTimeState = AVAR_DATE_TIME_NONE;

//Дискретний реєстратор
unsigned char crc_info_rejestrator_dr;
__INFO_REJESTRATOR info_rejestrator_dr;
unsigned char crc_info_rejestrator_dr_ctrl;
__INFO_REJESTRATOR info_rejestrator_dr_ctrl;
unsigned int state_dr_record = STATE_DR_NO_RECORD;
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
SRAM1 unsigned char queue_dr[SIZE_QUEUE_DR][SIZE_BUFFER_FOR_DR_RECORD];
#else
unsigned char queue_dr[SIZE_QUEUE_DR][SIZE_BUFFER_FOR_DR_RECORD];
#endif
size_t tail_queue_dr = 0;
unsigned int move_tail_queue_dr = false;

unsigned int part_writing_dr_into_dataflash;
unsigned int number_record_of_dr_for_menu = 0xffff;  //Це число означає, що номер запису не вибраний
unsigned int number_record_of_dr_for_USB = 0xffff;   //Це число означає, що номер запису не вибраний
unsigned int number_record_of_dr_for_RS485 = 0xffff; //Це число означає, що номер запису не вибраний
unsigned int part_reading_dr_from_dataflash_for_menu;
unsigned int part_reading_dr_from_dataflash_for_USB;
unsigned int part_reading_dr_from_dataflash_for_RS485;
//unsigned int state_current_monitoring;
const __meas_to_d_meas im_to_idm[_SIZE_ARRAY_FIX_MAX_MEASUREMENTS_TMP] =
  {
    {measurement, IM_UAB_TN1},
    {measurement, IM_IA_1},
    {measurement, IM_UAB_TN2},
    {measurement, IM_IA_2}};

unsigned int max_U_base;
unsigned int max_U_second;
unsigned int min_U_base;
unsigned int max_I_base;
unsigned int number_max_U_base_dr;
unsigned int number_max_U_second_dr;
unsigned int number_min_U_base_dr;
unsigned int number_max_I_base_dr;
unsigned int type_view_max_values_dr;
int index_cell_into_array_for_integral_values_dr;

enum _fix_date_time_avar drDateTimeState = AVAR_DATE_TIME_NONE;

//Реєстратор програмних помилок
unsigned char crc_info_rejestrator_pr_err;
__INFO_REJESTRATOR info_rejestrator_pr_err;
unsigned char crc_info_rejestrator_pr_err_ctrl;
__INFO_REJESTRATOR info_rejestrator_pr_err_ctrl;
unsigned char buffer_pr_err_records[SIZE_BUFFER_FOR_PR_ERR];
volatile unsigned int head_fifo_buffer_pr_err_records;
unsigned int tail_fifo_buffer_pr_err_records;
unsigned int temporary_block_writing_records_pr_err_into_DataFlash;
unsigned int etap_writing_pr_err_into_dataflash = ETAP_NONE;
unsigned int number_recods_writing_into_dataflash_now;
unsigned int number_record_of_stt_cmd_into_menu = 0;
unsigned int current_number_changes_of_stt_cmd_into_menu = 0;

unsigned int number_record_of_pr_err_into_menu = 0xffff;
unsigned int number_record_of_pr_err_into_USB = 0xffff;
unsigned int number_record_of_pr_err_into_RS485 = 0xffff;

//Очистка інформації по реєстраторах
unsigned int clean_rejestrators = 0;

//Лічильник ресурсу
__COUNTER_RESURS counter_today = {{0, 0, 0}, 0}, counter_previous_day = {{0, 0, 0}, 0};
unsigned int counter_total = 0;
unsigned char crc_resurs;
__COUNTER_RESURS counter_today_ctrl, counter_previous_day_ctrl;
unsigned int counter_total_ctrl;
unsigned char crc_resurs_ctrl;
unsigned int restart_counter = 0, previous_state_perekluchennya = 0, perekluchennya_unrecorded = 0;

//Перепрограмування приладу
unsigned int reprogram_device;

//RS-485
SRAM1 unsigned char TxBuffer_RS485[BUFFER_RS485];
SRAM1 unsigned char RxBuffer_RS485[BUFFER_RS485];
SRAM1 int TxBuffer_RS485_count;
SRAM1 int RxBuffer_RS485_count;
SRAM1 int RxBuffer_RS485_count_previous;
SRAM1 unsigned int time_last_receive_byte_RS485;
SRAM1 unsigned int make_reconfiguration_RS_485;
SRAM1 unsigned int number_bits_rs_485_waiting;
SRAM1 unsigned int mark_current_tick_RS_485;
SRAM1 unsigned int timeout_idle_RS485;

//Для UDP
int count_out /*= 0*/;
int count_out_previous /*= 0*/;
uint16_t previous_count_tim4_USB;
u8 buffer_USB_in[BUFFER_USB_IN];
int from_USB_ptr_in;
int from_USB_ptr_in_irq;
int from_USB_ptr_out_irq;
unsigned char usb_received[BUFFER_USB];
u8 buffer_USB_out[BUFFER_USB_OUT];
int to_USB_ptr_in_irq;
int to_USB_ptr_out;
unsigned char usb_transmiting[BUFFER_USB];
int usb_received_count /*= 0*/;
int usb_transmiting_count /*= 0*/;
unsigned char data_usb_transmiting /*= false*/;
unsigned char data_usb_transmiting_irq;
unsigned int timeout_idle_USB;

//uint32_t USB_Tx_begin;
//uint32_t USB_Tx_end;
//uint8_t USB_Tx_last_buffer[256];
//uint32_t USB_Tx_count;

extern uint8_t USB_Tx_State;

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
//MODBUS-TCP
SRAM1 unsigned char LAN_received[BUFFER_LAN];
int LAN_received_count;
SRAM1 unsigned char LAN_transmiting[BUFFER_LAN];
int LAN_transmiting_count;

SRAM1 unsigned int mark_current_tick_LAN;

SRAM1 unsigned int timeout_idle_LAN;
SRAM1 unsigned int password_set_LAN;

char id_ar_record_for_LAN[8 + 1 + 3 + 1];
unsigned int trigger_functions_LAN[N_BIG];

SRAM1 unsigned char buffer_for_LAN_read_record_dr[SIZE_BUFFER_FOR_DR_RECORD];
unsigned int number_record_of_dr_for_LAN = 0xffff; //Це число означає, що номер запису не вибраний
unsigned int part_reading_dr_from_dataflash_for_LAN;

SRAM1 unsigned char buffer_for_LAN_read_record_pr_err[SIZE_ONE_RECORD_PR_ERR];
unsigned int number_record_of_pr_err_into_LAN = 0xffff;

int max_number_time_sample_LAN;
SRAM1 unsigned char buffer_for_LAN_read_record_ar[SIZE_PAGE_DATAFLASH_2];
int number_record_of_ar_for_LAN = -1; //Це число означає, що номер запису не вибраний
int first_number_time_sample_for_LAN; // -1 - заголовок запису ан.р.; 0 - перший часовий зріз доаварійного масиву і т.д.
int last_number_time_sample_for_LAN;  // -1 - заголовок запису ан.р.; 0 - перший часовий зріз доаварійного масиву і т.д.
#endif

//MODBUS-RTU
SRAM1 unsigned int password_set_USB, password_set_RS485;
SRAM1 unsigned int password_changed;
SRAM1 unsigned int information_about_restart_counter;
SRAM1 unsigned int restart_timeout_interface;
SRAM1 unsigned int timeout_idle_new_settings;
SRAM1 unsigned int restart_timeout_idle_new_settings;
SRAM1 unsigned int type_of_settings_changed;

unsigned int serial_number_dev; //Заводський номер пристрою
SRAM1 unsigned int edit_serial_number_dev;

unsigned int watchdog_l2;
unsigned int control_word_of_watchdog;
unsigned int test_watchdogs /* = 0*/;

/**************************************************************
 * Змінна використовується в функції вибору групи уставок:
 * protections.c --> setpoints_selecting()
 **************************************************************/
unsigned int gr_ustavok_tmp = 0xf;

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
//Міжпроцесорний обмін
uint8_t Canal1_MO_Transmit[BUFFER_CANAL1_MO];
unsigned int Canal1_MO_ReceiveBank = 0;
uint8_t Canal1_MO_Received[2][BUFFER_CANAL1_MO];
uint32_t confirm_diagnostyka_mo;
uint8_t Canal2_MO_Transmit[BUFFER_CANAL2_MO];
uint8_t Canal2_MO_Received[BUFFER_CANAL2_MO];
unsigned int Canal1, Canal2;
const uint8_t my_address_mo = 0;
uint32_t IEC_board_uncall = 500;
uint32_t IEC_board_address;
uint32_t queue_mo = (1u << STATE_QUEUE_MO_READ_FW_VERSION);
uint32_t queue_mo_irq;
unsigned int restart_KP_irq = 5;
uint32_t state_array_control_state;
uint8_t fwKP[4];
uint8_t fwDTKP[6];

uint8_t Input_In_GOOSE_block[N_IN_GOOSE];
uint8_t Input_ctrl_In_GOOSE_block[N_IN_GOOSE];

uint8_t Input_In_MMS_block[N_IN_MMS];
uint8_t Input_ctrl_In_MMS_block[N_IN_MMS];

uint8_t Output_Out_LAN_block[N_OUT_LAN];

unsigned int activation_function_from_KP[N_SMALL];
unsigned int ctrl_activation_function_from_KP[N_SMALL];

#endif

const unsigned char letters[71][2] =
  {
    /***
Екстра сиволи. які немають кодів ASCII але є у таблиці LCD - замінюю службові коди [0x0-0x1F]
***/
    {MY_ASCII_ARROW_UP, 217},   //Стрілка вверх
    {MY_ASCII_ARROW_DOWN, 218}, //Стрілка вниз
                                /***/
    {176, 223},                 //°
    {178, 73},                  // І
    {179, 105},                 // і
    {192, 65},                  // А
    {193, 160},                 // Б
    {194, 66},                  // В
    {195, 161},                 // Г
    {196, 224},                 // Д
    {197, 69},                  // Е
    {168, 162},                 // Ё
    {198, 163},                 // Ж
    {199, 164},                 // З
    {200, 165},                 // И
    {201, 166},                 // Й
    {202, 75},                  // К
    {203, 167},                 // Л
    {204, 77},                  // М
    {205, 72},                  // Н
    {206, 79},                  // О
    {207, 168},                 // П
    {208, 80},                  // Р
    {209, 67},                  // С
    {210, 84},                  // Т
    {211, 169},                 // У
    {212, 170},                 // Ф
    {213, 88},                  // Х
    {214, 225},                 // Ц
    {215, 171},                 // Ч
    {216, 172},                 // Ш
    {217, 226},                 // Щ
    {218, 173},                 // Ъ
    {220, 98},                  // Ь
    {219, 174},                 // Ы
    {221, 175},                 // Э
    {222, 176},                 // Ю
    {223, 177},                 // Я
    {224, 97},                  // а
    {225, 178},                 // б
    {226, 179},                 // в
    {227, 180},                 // г
    {228, 227},                 // д
    {229, 101},                 // е
    {184, 181},                 // ё
    {230, 182},                 // ж
    {231, 183},                 // з
    {232, 184},                 // и
    {233, 185},                 // й
    {234, 186},                 // к
    {235, 187},                 // л
    {236, 188},                 // м
    {237, 189},                 // н
    {238, 111},                 // о
    {239, 190},                 // п
    {240, 112},                 // р
    {241, 99},                  // с
    {242, 191},                 // т
    {243, 121},                 // у
    {244, 228},                 // Ф
    {245, 120},                 // х
    {246, 229},                 // ц
    {247, 192},                 // ч
    {248, 193},                 // ш
    {249, 230},                 // щ
    {250, 194},                 // ъ
    {252, 196},                 // ь
    {251, 195},                 // ы
    {253, 197},                 // э
    {254, 198},                 // ю
    {255, 199}                  // я
};

const unsigned char extra_letters[13][1 + MAX_NAMBER_LANGUAGE] =
  {
    /*CODE   RU    UA    EN    KZ     */
    {0xAF, 0xFF, 0x00, 0x00, 0xFF}, // Ї або велика біква Омега для англійської розкладки клавіатури
    {0xBF, 0xFF, 0x01, 0xFF, 0xFF}, // ї
    {0xA5, 0xFF, 0x02, 0xFF, 0xFF}, // Ґ
    {0xB4, 0xFF, 0x03, 0xFF, 0xFF}, // ґ
    {0xAA, 0xFF, 0x04, 0xFF, 0xFF}, // Є
    {0xBA, 0xFF, 0x05, 0xFF, 0xFF}, // є
    {0x8A, 0xFF, 0xFF, 0xFF, 0x00}, // Љ - замінний символ з даним кодом для WIN1251
    {0x9A, 0xFF, 0xFF, 0xFF, 0x01}, // љ - замінний символ з даним кодом для WIN1251
    {0xBD, 0xFF, 0xFF, 0xFF, 0x02}, // Ѕ - замінний символ з даним кодом для WIN1251
    {0xBE, 0xFF, 0xFF, 0xFF, 0x03}, // ѕ - замінний символ з даним кодом для WIN1251
    {0x80, 0xFF, 0xFF, 0xFF, 0x04}, // Ђ - замінний символ з даним кодом для WIN1251
    {0x90, 0xFF, 0xFF, 0xFF, 0x05}, // ђ - замінний символ з даним кодом для WIN1251
    {0xA7, 0x00, 0x06, 0x01, 0x06}  // § - замінний грецької букви фі для всіх мов
};

int current_language = LANGUAGE_ABSENT;

const uint8_t information_off_on[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] =
  {
    {"     Откл.      ", "      Вкл.      "},
    {"     Вимк.      ", "     Увімк.     "},
    {"      Off       ", "       On       "},
    {"     Сљнд.      ", "     Косу.      "}};
const uint32_t cursor_x_off_on[MAX_NAMBER_LANGUAGE][2] =
  {
    {4, 5},
    {4, 4},
    {5, 6},
    {4, 4}};
unsigned char chGbl__REL1_REL8__W_VAL;
volatile unsigned char chGbl__CDHO1_CDHO7__W_VAL;

extern unsigned short const __checksum;
extern unsigned int __checksum_begin;
extern unsigned int __checksum_end;

extern unsigned int __ICFEDIT_region_RAM1_start__;
extern unsigned int __ICFEDIT_region_RAM1_size__;

#ifdef DEBUG_TEST

//volatile unsigned int reprogram = 1;

//unsigned int time_1 = 0xffff0000, time_2 = 0xffff0000, max_delay = 0;

//Тимчасові змінні
//unsigned int temp_value_for_debbuging_1 = 0;
//unsigned int temp_value_for_debbuging_2 = 0;
//unsigned int temp_value_for_debbuging_3 = 0;

#endif

#endif
