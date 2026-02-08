#ifndef __CONST_GLOBAL__
#define __CONST_GLOBAL__

#define MAX_NUMBER_LINES_VMP 8

#define NUMBER_ANALOG_CANALES 8

#define NUMBER_INPUTS 16
#define NUMBER_SIMPLE_OUTPUTS 16
#define NUMBER_OUTPUTS (NUMBER_SIMPLE_OUTPUTS)

#define NUMBER_LEDS 17

#define NUMBER_UP 8

//#define NUMBER_DEFINED_ELEMENTS         6
#define NUMBER_DEFINED_FUNCTIONS 8
#define NUMBER_DEFINED_TRIGGERS 4
#define NUMBER_DEFINED_AND 8
#define NUMBER_DEFINED_OR 8
#define NUMBER_DEFINED_XOR 8
#define NUMBER_DEFINED_NOT 16
#define NUMBER_TRANSFER_FUNCTIONS 16
#define NUMBER_LINK_FUNCTIONS 16

#define NUMBER_DEFINED_BUTTONS 6

#define NUMBER_IN_AND 8
#define NUMBER_IN_OR 8

#define NUMBER_GROUP_USTAVOK 4

#define N_INPUT_BOARDS 2
#define N_OUTPUT_BOARDS 3

enum _configuration
{
  RPN_BIT_CONFIGURATION = 0,
  ZSKh_BIT_CONFIGURATION,
  ZNKh_BIT_CONFIGURATION,
  BRP_BIT_CONFIGURATION,
  Umax_BIT_CONFIGURATION,
  Umin_BIT_CONFIGURATION,
  UP_BIT_CONFIGURATION,
  EL_BIT_CONFIGURATION,
  _FIX_NUMBER_PROTECTION,

  TOTAL_NUMBER_PROTECTION = _FIX_NUMBER_PROTECTION
};

/*****************************************/
//Константи для ранжування дискретних входів
/*****************************************/
enum __rang_small
{
  _RANG_SMALL_GENERAL_BEGIN = 0,
  RANG_SMALL_BLOCK_VKL_VV = _RANG_SMALL_GENERAL_BEGIN,
  RANG_SMALL_RESET_LEDS,
  RANG_SMALL_RESET_RELES,
  RANG_SMALL_MISCEVE_DYSTANCIJNE,
  RANG_SMALL_STATE_VV,
  RANG_SMALL_OTKL_VID_ZOVN_ZAHYSTIV,
  RANG_SMALL_OTKL_VV,
  RANG_SMALL_VKL_VV,
  RANG_SMALL_CTRL_OTKL,
  RANG_SMALL_CTRL_VKL,
  RANG_SMALL_1_GRUPA_USTAVOK,
  RANG_SMALL_2_GRUPA_USTAVOK,
  RANG_SMALL_3_GRUPA_USTAVOK,
  RANG_SMALL_4_GRUPA_USTAVOK,
  RANG_SMALL_RESET_BLOCK_READY_TU_VID_ZAHYSTIV,

  _RANG_SMALL_RPN_BEGIN,
  RANG_SMALL_OSNOVNYJ_TN2_RPN = _RANG_SMALL_RPN_BEGIN,
  RANG_SMALL_MRZS_OR_LOCAL_MODE_RPN,
  RANG_SMALL_TM_MODE_VID_DV_RPN,
  RANG_SMALL_AUTO_MODE_FROM_DV_RPN,
  RANG_SMALL_PEREKLYUCHENNYA_RPN,
  RANG_SMALL_UBAVYTY_FROM_DV_RPN,
  RANG_SMALL_PRYBAVYTY_FROM_DV_RPN,
  RANG_SMALL_BLOCK_STRUM_KOMP_RPN,
  RANG_SMALL_ZOVNISHNJE_BLOCKUVANNJA_RPN,
  RANG_SMALL_1_POLOGENNJA_RPN,
  RANG_SMALL_NOMINALNA_POZYCIJA_RPN,
  RANG_SMALL_N_POLOGENNJA_RPN,
  RANG_SMALL_CLEAR_BLK_RPN,
  RANG_SMALL_UBAVYTY_FROM_KB_OR_UPPER_LEVEL_RPN,
  RANG_SMALL_PRYBAVYTY_FROM_KB_OR_UPPER_LEVEL_RPN,

  _RANG_SMALL_ZSKh_BEGIN,
  RANG_SMALL_KONTROL_UBAVYTY_ZSKh = _RANG_SMALL_ZSKh_BEGIN,
  RANG_SMALL_KONTROL_PRYBAVYTY_ZSKh,

  _RANG_SMALL_ZNKh_BEGIN,

  _RANG_SMALL_BRP_BEGIN = _RANG_SMALL_ZNKh_BEGIN,
  RANG_SMALL_BLOCK_BRP = _RANG_SMALL_BRP_BEGIN,

  _RANG_SMALL_UMAX_BEGIN,
  RANG_SMALL_BLOCK_UMAX2 = _RANG_SMALL_UMAX_BEGIN,

  _RANG_SMALL_UMIN_BEGIN,
  RANG_SMALL_BLOCK_UMIN1 = _RANG_SMALL_UMIN_BEGIN,
  RANG_SMALL_BLOCK_UMIN2,

  _RANG_SMALL_UP_BEGIN,
  RANG_SMALL_BLOCK_UP1 = _RANG_SMALL_UP_BEGIN,

  _RANG_SMALL_EL_BEGIN = (RANG_SMALL_BLOCK_UP1 + NUMBER_UP),
  RANG_SMALL_DF1_IN = _RANG_SMALL_EL_BEGIN,
  RANG_SMALL_DF1_R,
  RANG_SMALL_DF2_IN,
  RANG_SMALL_DF2_R,
  RANG_SMALL_DF3_IN,
  RANG_SMALL_DF3_R,
  RANG_SMALL_DF4_IN,
  RANG_SMALL_DF4_R,
  RANG_SMALL_DF5_IN,
  RANG_SMALL_DF5_R,
  RANG_SMALL_DF6_IN,
  RANG_SMALL_DF6_R,
  RANG_SMALL_DF7_IN,
  RANG_SMALL_DF7_R,
  RANG_SMALL_DF8_IN,
  RANG_SMALL_DF8_R,
  RANG_SMALL_DT1_SET,
  RANG_SMALL_DT1_RESET,
  RANG_SMALL_DT2_SET,
  RANG_SMALL_DT2_RESET,
  RANG_SMALL_DT3_SET,
  RANG_SMALL_DT3_RESET,
  RANG_SMALL_DT4_SET,
  RANG_SMALL_DT4_RESET,
  RANG_SMALL_LF1,
  RANG_SMALL_LF2,
  RANG_SMALL_LF3,
  RANG_SMALL_LF4,
  RANG_SMALL_LF5,
  RANG_SMALL_LF6,
  RANG_SMALL_LF7,
  RANG_SMALL_LF8,
  RANG_SMALL_LF9,
  RANG_SMALL_LF10,
  RANG_SMALL_LF11,
  RANG_SMALL_LF12,
  RANG_SMALL_LF13,
  RANG_SMALL_LF14,
  RANG_SMALL_LF15,
  RANG_SMALL_LF16,

  NUMBER_TOTAL_SIGNAL_FOR_RANG_SMALL
};

#define N_SMALL ((NUMBER_TOTAL_SIGNAL_FOR_RANG_SMALL >> 5) + ((NUMBER_TOTAL_SIGNAL_FOR_RANG_SMALL & 0x1f) != 0))

#define NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL (_RANG_SMALL_RPN_BEGIN - _RANG_SMALL_GENERAL_BEGIN)
#define NUMBER_RPN_SIGNAL_FOR_RANG_SMALL (_RANG_SMALL_ZSKh_BEGIN - _RANG_SMALL_RPN_BEGIN)
#define NUMBER_SZKh_SIGNAL_FOR_RANG_SMALL (_RANG_SMALL_ZNKh_BEGIN - _RANG_SMALL_ZSKh_BEGIN)
#define NUMBER_SNKh_SIGNAL_FOR_RANG_SMALL (_RANG_SMALL_BRP_BEGIN - _RANG_SMALL_ZNKh_BEGIN)
#define NUMBER_BRP_SIGNAL_FOR_RANG_SMALL (_RANG_SMALL_UMAX_BEGIN - _RANG_SMALL_BRP_BEGIN)
#define NUMBER_UMAX_SIGNAL_FOR_RANG_SMALL (_RANG_SMALL_UMIN_BEGIN - _RANG_SMALL_UMAX_BEGIN)
#define NUMBER_UMIN_SIGNAL_FOR_RANG_SMALL (_RANG_SMALL_UP_BEGIN - _RANG_SMALL_UMIN_BEGIN)
#define NUMBER_UP_SIGNAL_FOR_RANG_SMALL (_RANG_SMALL_EL_BEGIN - _RANG_SMALL_UP_BEGIN)
#define NUMBER_EL_SIGNAL_FOR_RANG_SMALL (NUMBER_TOTAL_SIGNAL_FOR_RANG_SMALL - _RANG_SMALL_EL_BEGIN)

/*****************************************/

/*****************************************/
//Константи для ранжування дискретних виходів-свтодіодів-О-функцій-реєстраторів
/*****************************************/
enum __rang_output_led_df_reg
{
  _RANG_GENERAL_BEGIN = 0,
  RANG_BLOCK_VKL_VV = _RANG_GENERAL_BEGIN,
  RANG_RESET_LEDS,
  RANG_RESET_RELES,
  RANG_MISCEVE_DYSTANCIJNE,
  RANG_STATE_VV,
  RANG_OTKL_VID_ZOVN_ZAHYSTIV,
  RANG_OTKL_VV,
  RANG_VKL_VV,
  RANG_CTRL_OTKL,
  RANG_CTRL_VKL,
  RANG_PRYVID_VV,
  RANG_DI_ACTIVE,
  RANG_FK_ACTIVE,
  RANG_USB_ACTIVE,
  RANG_RS485_ACTIVE,
  RANG_DEFECT,
  RANG_AVAR_DEFECT,
  RANG_WORK_A_REJESTRATOR,
  RANG_WORK_D_REJESTRATOR,
  RANG_VIDKL_VID_ZAKHYSTIV,
  RANG_WORK_BO,
  RANG_WORK_BV,
  RANG_1_GRUPA_USTAVOK,
  RANG_2_GRUPA_USTAVOK,
  RANG_3_GRUPA_USTAVOK,
  RANG_4_GRUPA_USTAVOK,
  RANG_INVERS_DV_GRUPA_USTAVOK,
  RANG_BLK_GRUP_USTAVOK_VID_ZACHYSTIV,
  RANG_RESET_BLOCK_READY_TU_VID_ZAHYSTIV,
  RANG_READY_TU,
  RANG_SETTINGS_CHANGED,

  _RANG_RPN_BEGIN,
  RANG_OSNOVNYJ_TN2_RPN = _RANG_RPN_BEGIN,
  RANG_MRZS_OR_LOCAL_MODE_RPN,
  RANG_TM_MODE_VID_DV_RPN,
  RANG_TM_MODE_RPN,
  RANG_AUTO_MODE_FROM_DV_RPN,
  RANG_REMOTE_MODE_FROM_UPPER_LEVEL_RPN,
  RANG_REMOTE_MODE_RPN,
  RANG_AUTO_MODE_FROM_UPPER_LEVEL_RPN,
  RANG_AUTO_MODE_RPN,
  RANG_PEREKLYUCHENNYA_RPN,
  RANG_UBAVYTY_FROM_UPPER_LEVEL_RPN,
  RANG_UBAVYTY_FROM_KB_RPN,
  RANG_UBAVYTY_FROM_DV_RPN,
  RANG_UBAVYTY_R_RPN,
  RANG_UBAVYTY_T_RPN,
  RANG_UBAVYTY_DI_RPN,
  RANG_UBAVYTY_1_RPN,
  RANG_UBAVYTY_RPN,
  RANG_PRYBAVYTY_FROM_UPPER_LEVEL_RPN,
  RANG_PRYBAVYTY_FROM_KB_RPN,
  RANG_PRYBAVYTY_FROM_DV_RPN,
  RANG_PRYBAVYTY_R_RPN,
  RANG_PRYBAVYTY_T_RPN,
  RANG_PRYBAVYTY_DI_RPN,
  RANG_PRYBAVYTY_1_RPN,
  RANG_PRYBAVYTY_RPN,
  RANG_BLOCK_STRUM_KOMP_RPN,
  RANG_ZOVNISHNJE_BLOCKUVANNJA_RPN,
  RANG_1_POLOGENNJA_RPN,
  RANG_NOMINALNA_POZYCIJA_RPN,
  RANG_N_POLOGENNJA_RPN,
  RANG_CLEAR_BLK_RPN,
  RANG_UBAVYTY_FROM_KB_OR_UPPER_LEVEL_RPN,
  RANG_PRYBAVYTY_FROM_KB_OR_UPPER_LEVEL_RPN,
  RANG_BLOCKUVANNJA_RPN,
  RANG_RESET_DEFECT_RPN,
  RANG_PO1_U_OSN_RPN,
  RANG_PO1_U_OSN_KOMP_RPN,
  RANG_VYJCHE_ZONY_RPN,
  RANG_PO1_U_VSPOM_RPN,
  RANG_PO2_U_OSN_RPN,
  RANG_PO2_U_OSN_KOMP_RPN,
  RANG_NYJCHE_ZONY_RPN,
  RANG_PO3_U_OSN_RPN,
  RANG_PO3_U_VSPOM_RPN,
  RANG_PO4_U_OSN_RPN,
  RANG_DOZVIL_UBAVYTY_RPN,
  RANG_DOZVIL_AVTO_UBAVYTY_RPN,
  RANG_DOZVIL_PRYBAVYTY_RPN,
  RANG_DOZVIL_AVTO_PRYBAVYTY_RPN,
  RANG_ZASTRJAVANNJA_RPN,
  RANG_PRYVID_NE_PISHOV_RPN,
  RANG_N_PER_RIVNE_N_PER_USTAVKY_RPN,
  RANG_NESPRAVNIST_RPN,

  _RANG_ZSKh_BEGIN,
  RANG_KONTROL_UBAVYTY_ZSKh = _RANG_ZSKh_BEGIN,
  RANG_KONTROL_PRYBAVYTY_ZSKh,
  RANG_ZSKh,

  _RANG_ZNKh_BEGIN,
  RANG_ZNKh_IR = _RANG_ZNKh_BEGIN,
  RANG_ZNKh_BR,
  RANG_ZNKh,

  _RANG_BRP_BEGIN,
  RANG_BLOCK_BRP = _RANG_BRP_BEGIN,
  RANG_BRP,

  _RANG_UMAX_BEGIN,
  RANG_BLOCK_UMAX2 = _RANG_UMAX_BEGIN,
  RANG_PO_UMAX1,
  RANG_PRYSK_UBAVYTY_VID_UMAX1,
  RANG_BLOCK_PRYBAVYTY_VID_UMAX1,
  RANG_PO_UMAX2,
  RANG_UMAX2,

  _RANG_UMIN_BEGIN,
  RANG_BLOCK_UMIN1 = _RANG_UMIN_BEGIN,
  RANG_BLOCK_UMIN2,
  RANG_PO_UMIN1,
  RANG_UMIN1,
  RANG_PO_UMIN2,
  RANG_UMIN2,

  _RANG_UP_BEGIN,
  RANG_BLOCK_UP1 = _RANG_UP_BEGIN,
  RANG_PO_UP1,
  RANG_UP1,

  _RANG_EL_BEGIN = (RANG_BLOCK_UP1 + 3 * NUMBER_UP),
  RANG_DF1_IN = _RANG_EL_BEGIN,
  RANG_DF1_R,
  RANG_DF1_OUT,
  RANG_DF2_IN,
  RANG_DF2_R,
  RANG_DF2_OUT,
  RANG_DF3_IN,
  RANG_DF3_R,
  RANG_DF3_OUT,
  RANG_DF4_IN,
  RANG_DF4_R,
  RANG_DF4_OUT,
  RANG_DF5_IN,
  RANG_DF5_R,
  RANG_DF5_OUT,
  RANG_DF6_IN,
  RANG_DF6_R,
  RANG_DF6_OUT,
  RANG_DF7_IN,
  RANG_DF7_R,
  RANG_DF7_OUT,
  RANG_DF8_IN,
  RANG_DF8_R,
  RANG_DF8_OUT,
  RANG_DT1_SET,
  RANG_DT1_RESET,
  RANG_DT1_OUT,
  RANG_DT2_SET,
  RANG_DT2_RESET,
  RANG_DT2_OUT,
  RANG_DT3_SET,
  RANG_DT3_RESET,
  RANG_DT3_OUT,
  RANG_DT4_SET,
  RANG_DT4_RESET,
  RANG_DT4_OUT,
  RANG_D_AND1,
  RANG_D_AND2,
  RANG_D_AND3,
  RANG_D_AND4,
  RANG_D_AND5,
  RANG_D_AND6,
  RANG_D_AND7,
  RANG_D_AND8,
  RANG_D_OR1,
  RANG_D_OR2,
  RANG_D_OR3,
  RANG_D_OR4,
  RANG_D_OR5,
  RANG_D_OR6,
  RANG_D_OR7,
  RANG_D_OR8,
  RANG_D_XOR1,
  RANG_D_XOR2,
  RANG_D_XOR3,
  RANG_D_XOR4,
  RANG_D_XOR5,
  RANG_D_XOR6,
  RANG_D_XOR7,
  RANG_D_XOR8,
  RANG_D_NOT1,
  RANG_D_NOT2,
  RANG_D_NOT3,
  RANG_D_NOT4,
  RANG_D_NOT5,
  RANG_D_NOT6,
  RANG_D_NOT7,
  RANG_D_NOT8,
  RANG_D_NOT9,
  RANG_D_NOT10,
  RANG_D_NOT11,
  RANG_D_NOT12,
  RANG_D_NOT13,
  RANG_D_NOT14,
  RANG_D_NOT15,
  RANG_D_NOT16,
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
  RANG_LF16,
  RANG_ERROR_CONF_EL,

  NUMBER_TOTAL_SIGNAL_FOR_RANG
};

#define N_BIG ((NUMBER_TOTAL_SIGNAL_FOR_RANG >> 5) + ((NUMBER_TOTAL_SIGNAL_FOR_RANG & 0x1f) != 0))

#define NUMBER_GENERAL_SIGNAL_FOR_RANG (_RANG_RPN_BEGIN - _RANG_GENERAL_BEGIN)
#define NUMBER_RPN_SIGNAL_FOR_RANG (_RANG_ZSKh_BEGIN - _RANG_RPN_BEGIN)
#define NUMBER_SZKh_SIGNAL_FOR_RANG (_RANG_ZNKh_BEGIN - _RANG_ZSKh_BEGIN)
#define NUMBER_SNKh_SIGNAL_FOR_RANG (_RANG_BRP_BEGIN - _RANG_ZNKh_BEGIN)
#define NUMBER_BRP_SIGNAL_FOR_RANG (_RANG_UMAX_BEGIN - _RANG_BRP_BEGIN)
#define NUMBER_UMAX_SIGNAL_FOR_RANG (_RANG_UMIN_BEGIN - _RANG_UMAX_BEGIN)
#define NUMBER_UMIN_SIGNAL_FOR_RANG (_RANG_UP_BEGIN - _RANG_UMIN_BEGIN)
#define NUMBER_UP_SIGNAL_FOR_RANG (_RANG_EL_BEGIN - _RANG_UP_BEGIN)
#define NUMBER_EL_SIGNAL_FOR_RANG (NUMBER_TOTAL_SIGNAL_FOR_RANG - _RANG_EL_BEGIN)

/*****************************************/

// clang-format off
/*****************************************/
//Макска команд
/*****************************************/
#define COMMAND_SIGNALES_0 (                                  \
    (1u << (RANG_SMALL_RESET_LEDS - 0))                       \
  | (1u << (RANG_SMALL_RESET_RELES - 0))                      \
  | (1u << (RANG_SMALL_OTKL_VV - 0))                          \
  | (1u << (RANG_SMALL_VKL_VV - 0))                           \
  | (1u << (RANG_SMALL_RESET_BLOCK_READY_TU_VID_ZAHYSTIV - 0))\
  | (1u << (RANG_SMALL_CLEAR_BLK_RPN - 0))                    \
  | (1u << (RANG_SMALL_UBAVYTY_FROM_KB_OR_UPPER_LEVEL_RPN - 0))\
  | (1u << (RANG_SMALL_PRYBAVYTY_FROM_KB_OR_UPPER_LEVEL_RPN - 0))\
)

  #define COMMAND_SIGNALES_1 (                                \
    (1u << (RANG_SMALL_DF1_IN - 32))                          \
  | (1u << (RANG_SMALL_DF1_R - 32))                           \
  | (1u << (RANG_SMALL_DF2_IN - 32))                          \
  | (1u << (RANG_SMALL_DF2_R - 32))                           \
  | (1u << (RANG_SMALL_DF3_IN - 32))                          \
  | (1u << (RANG_SMALL_DF3_R - 32))                           \
  | (1u << (RANG_SMALL_DF4_IN - 32))                          \
  | (1u << (RANG_SMALL_DF4_R - 32))                           \
  | (1u << (RANG_SMALL_DF5_IN - 32))                          \
  | (1u << (RANG_SMALL_DF5_R - 32))                           \
  | (1u << (RANG_SMALL_DF6_IN - 32))                          \
  | (1u << (RANG_SMALL_DF6_R - 32))                           \
  | (1u << (RANG_SMALL_DF7_IN - 32))                          \
  | (1u << (RANG_SMALL_DF7_R - 32))                           \
  | (1u << (RANG_SMALL_DF8_IN - 32))                          \
  | (1u << (RANG_SMALL_DF8_R - 32))                           \
  | (1u << (RANG_SMALL_DT1_SET - 32))                         \
  | (1u << (RANG_SMALL_DT1_RESET - 32))                       \
  | (1u << (RANG_SMALL_DT2_SET - 32))                         \
  | (1u << (RANG_SMALL_DT2_RESET - 32))                       \
  | (1u << (RANG_SMALL_DT3_SET - 32))                         \
  | (1u << (RANG_SMALL_DT3_RESET - 32))                       \
  | (1u << (RANG_SMALL_DT4_SET - 32))                         \
  | (1u << (RANG_SMALL_DT4_RESET - 32))                       \
  | (1u << (RANG_SMALL_LF1 - 32))                             \
  | (1u << (RANG_SMALL_LF2 - 32))                             \
  | (1u << (RANG_SMALL_LF3 - 32))                             \
  | (1u << (RANG_SMALL_LF4 - 32))                             \
)

#define COMMAND_SIGNALES_2 (                                  \
    (1u << (RANG_SMALL_LF5 - 64))                             \
  | (1u << (RANG_SMALL_LF6 - 64))                             \
  | (1u << (RANG_SMALL_LF7 - 64))                             \
  | (1u << (RANG_SMALL_LF8 - 64))                             \
  | (1u << (RANG_SMALL_LF9 - 64))                             \
  | (1u << (RANG_SMALL_LF10 - 64))                            \
  | (1u << (RANG_SMALL_LF11 - 64))                            \
  | (1u << (RANG_SMALL_LF12 - 64))                            \
  | (1u << (RANG_SMALL_LF13 - 64))                            \
  | (1u << (RANG_SMALL_LF14 - 64))                            \
  | (1u << (RANG_SMALL_LF15 - 64))                            \
  | (1u << (RANG_SMALL_LF16 - 64))                            \
)
/*****************************************/

/*****************************************/
//Макски сигналів, які можна зранжувати на функціональні кнопки у Режимі Кнопка
/*****************************************/
#define MASKA_BUTTON_MODE_0_SIGNALS_0 (                        \
    (1u << (RANG_SMALL_RESET_LEDS - 0))                        \
  | (1u << (RANG_SMALL_RESET_RELES - 0))                       \
  | (1u << (RANG_SMALL_RESET_BLOCK_READY_TU_VID_ZAHYSTIV - 0)) \
  | (1u << (RANG_SMALL_CLEAR_BLK_RPN - 0))                    \
  | (1u << (RANG_SMALL_UBAVYTY_FROM_KB_OR_UPPER_LEVEL_RPN - 0))\
  | (1u << (RANG_SMALL_PRYBAVYTY_FROM_KB_OR_UPPER_LEVEL_RPN - 0))\
)

#define MASKA_BUTTON_MODE_0_SIGNALS_1 (                        \
    (1u << (RANG_SMALL_DF1_IN - 32))                           \
  | (1u << (RANG_SMALL_DF1_R - 32))                            \
  | (1u << (RANG_SMALL_DF2_IN - 32))                           \
  | (1u << (RANG_SMALL_DF2_R - 32))                            \
  | (1u << (RANG_SMALL_DF3_IN -32))                            \
  | (1u << (RANG_SMALL_DF3_R - 32))                            \
  | (1u << (RANG_SMALL_DF4_IN - 32))                           \
  | (1u << (RANG_SMALL_DF4_R - 32))                            \
  | (1u << (RANG_SMALL_DF5_IN - 32))                           \
  | (1u << (RANG_SMALL_DF5_R - 32))                            \
  | (1u << (RANG_SMALL_DF6_IN - 32))                           \
  | (1u << (RANG_SMALL_DF6_R - 32))                            \
  | (1u << (RANG_SMALL_DF7_IN - 32))                           \
  | (1u << (RANG_SMALL_DF7_R - 32))                            \
  | (1u << (RANG_SMALL_DF8_IN - 32))                           \
  | (1u << (RANG_SMALL_DF8_R - 32))                            \
  | (1u << (RANG_SMALL_DT1_SET - 32))                          \
  | (1u << (RANG_SMALL_DT1_RESET - 32))                        \
  | (1u << (RANG_SMALL_DT2_SET - 32))                          \
  | (1u << (RANG_SMALL_DT2_RESET - 32))                        \
  | (1u << (RANG_SMALL_DT3_SET - 32))                          \
  | (1u << (RANG_SMALL_DT3_RESET - 32))                        \
  | (1u << (RANG_SMALL_DT4_SET - 32))                          \
  | (1u << (RANG_SMALL_DT4_RESET - 32))                        \
  | (1u << (RANG_SMALL_LF1 - 32))                              \
  | (1u << (RANG_SMALL_LF2 - 32))                              \
  | (1u << (RANG_SMALL_LF3 - 32))                              \
  | (1u << (RANG_SMALL_LF4 - 32))                              \
)

#define MASKA_BUTTON_MODE_0_SIGNALS_2 (                        \
    (1u << (RANG_SMALL_LF5 - 64))                              \
  | (1u << (RANG_SMALL_LF6 - 64))                              \
  | (1u << (RANG_SMALL_LF7 - 64))                              \
  | (1u << (RANG_SMALL_LF8 - 64))                              \
  | (1u << (RANG_SMALL_LF9 - 64))                              \
  | (1u << (RANG_SMALL_LF10 - 64))                             \
  | (1u << (RANG_SMALL_LF11 - 64))                             \
  | (1u << (RANG_SMALL_LF12 - 64))                             \
  | (1u << (RANG_SMALL_LF13 - 64))                             \
  | (1u << (RANG_SMALL_LF14 - 64))                             \
  | (1u << (RANG_SMALL_LF15 - 64))                             \
  | (1u << (RANG_SMALL_LF16 - 64))                             \
)
/*****************************************/

/*****************************************/
//Макски сигналів, які можна зранжувати на функціональні кнопки у Режимі Ключ
/*****************************************/
#define MASKA_BUTTON_MODE_1_SIGNALS_0 (                        \
    (1u << (RANG_SMALL_BLOCK_VKL_VV - 0))                      \
  | (1u << (RANG_SMALL_MISCEVE_DYSTANCIJNE - 0))               \
  | (1u << (RANG_SMALL_1_GRUPA_USTAVOK - 0))                   \
  | (1u << (RANG_SMALL_2_GRUPA_USTAVOK - 0))                   \
  | (1u << (RANG_SMALL_3_GRUPA_USTAVOK - 0))                   \
  | (1u << (RANG_SMALL_4_GRUPA_USTAVOK - 0))                   \
  | (1u << (RANG_SMALL_OSNOVNYJ_TN2_RPN - 0))                  \
  | (1u << (RANG_SMALL_MRZS_OR_LOCAL_MODE_RPN - 0))            \
  | (1u << (RANG_SMALL_TM_MODE_VID_DV_RPN - 0))                \
  | (1u << (RANG_SMALL_AUTO_MODE_FROM_DV_RPN - 0))             \
  | (1u << (RANG_SMALL_PEREKLYUCHENNYA_RPN - 0))               \
  | (1u << (RANG_SMALL_UBAVYTY_FROM_DV_RPN - 0))               \
  | (1u << (RANG_SMALL_PRYBAVYTY_FROM_DV_RPN - 0))             \
  | (1u << (RANG_SMALL_BLOCK_STRUM_KOMP_RPN - 0))              \
  | (1u << (RANG_SMALL_ZOVNISHNJE_BLOCKUVANNJA_RPN - 0))       \
  | (1u << (RANG_SMALL_1_POLOGENNJA_RPN - 0))                  \
  | (1u << (RANG_SMALL_NOMINALNA_POZYCIJA_RPN - 0))            \
  | (1u << (RANG_SMALL_N_POLOGENNJA_RPN - 0))                  \
  | (1u << (RANG_SMALL_CLEAR_BLK_RPN - 0))                     \
  | (1u << (RANG_SMALL_KONTROL_UBAVYTY_ZSKh - 0))              \
  | (1u << (RANG_SMALL_KONTROL_PRYBAVYTY_ZSKh - 0))            \
)

#define MASKA_BUTTON_MODE_1_SIGNALS_1 (                        \
    (1u << (RANG_SMALL_BLOCK_BRP - 32))                        \
  | (1u << (RANG_SMALL_BLOCK_UMAX2 - 32))                      \
  | (1u << (RANG_SMALL_BLOCK_UMIN1 - 32))                      \
  | (1u << (RANG_SMALL_BLOCK_UMIN2 - 32))                      \
  | (1u << (RANG_SMALL_BLOCK_UP1 + 0 - 32))                    \
  | (1u << (RANG_SMALL_BLOCK_UP1 + 1 - 32))                    \
  | (1u << (RANG_SMALL_BLOCK_UP1 + 2 - 32))                    \
  | (1u << (RANG_SMALL_BLOCK_UP1 + 3 - 32))                    \
  | (1u << (RANG_SMALL_BLOCK_UP1 + 4 - 32))                    \
  | (1u << (RANG_SMALL_BLOCK_UP1 + 5 - 32))                    \
  | (1u << (RANG_SMALL_BLOCK_UP1 + 6 - 32))                    \
  | (1u << (RANG_SMALL_BLOCK_UP1 + 7 - 32))                    \
  | (1u << (RANG_SMALL_DF1_IN - 32))                           \
  | (1u << (RANG_SMALL_DF1_R - 32))                            \
  | (1u << (RANG_SMALL_DF2_IN - 32))                           \
  | (1u << (RANG_SMALL_DF2_R - 32))                            \
  | (1u << (RANG_SMALL_DF3_IN - 32))                           \
  | (1u << (RANG_SMALL_DF3_R - 32))                            \
  | (1u << (RANG_SMALL_DF4_IN - 32))                           \
  | (1u << (RANG_SMALL_DF4_R - 32))                            \
  | (1u << (RANG_SMALL_DF5_IN - 32))                           \
  | (1u << (RANG_SMALL_DF5_R - 32))                            \
  | (1u << (RANG_SMALL_DF6_IN - 32))                           \
  | (1u << (RANG_SMALL_DF6_R - 32))                            \
  | (1u << (RANG_SMALL_DF7_IN - 32))                           \
  | (1u << (RANG_SMALL_DF7_R - 32))                            \
  | (1u << (RANG_SMALL_DF8_IN - 32))                           \
  | (1u << (RANG_SMALL_DF8_R - 32))                            \
  | (1u << (RANG_SMALL_DT1_SET - 32))                          \
  | (1u << (RANG_SMALL_DT1_RESET - 32))                        \
  | (1u << (RANG_SMALL_DT2_SET - 32))                          \
  | (1u << (RANG_SMALL_DT2_RESET - 32))                        \
  | (1u << (RANG_SMALL_DT3_SET - 32))                          \
  | (1u << (RANG_SMALL_DT3_RESET - 32))                        \
  | (1u << (RANG_SMALL_DT4_SET - 32))                          \
  | (1u << (RANG_SMALL_DT4_RESET - 32))                        \
  | (1u << (RANG_SMALL_LF1 - 32))                              \
  | (1u << (RANG_SMALL_LF2 - 32))                              \
  | (1u << (RANG_SMALL_LF3 - 32))                              \
  | (1u << (RANG_SMALL_LF4 - 32))                              \
)

#define MASKA_BUTTON_MODE_1_SIGNALS_2 (                        \
    (1u << (RANG_SMALL_LF5 - 64))                              \
  | (1u << (RANG_SMALL_LF6 - 64))                              \
  | (1u << (RANG_SMALL_LF7 - 64))                              \
  | (1u << (RANG_SMALL_LF8 - 64))                              \
  | (1u << (RANG_SMALL_LF9 - 64))                              \
  | (1u << (RANG_SMALL_LF10 - 64))                             \
  | (1u << (RANG_SMALL_LF11 - 64))                             \
  | (1u << (RANG_SMALL_LF12 - 64))                             \
  | (1u << (RANG_SMALL_LF13 - 64))                             \
  | (1u << (RANG_SMALL_LF14 - 64))                             \
  | (1u << (RANG_SMALL_LF15 - 64))                             \
  | (1u << (RANG_SMALL_LF16 - 64))                             \
)
/*****************************************/

/*****************************************/
//Макски сигналів, які можна зранжувати на дискретні входи
/*****************************************/
#define MASKA_INPUT_SIGNALS_0 (                                \
    (1u << (RANG_SMALL_BLOCK_VKL_VV - 0))                      \
  | (1u << (RANG_SMALL_RESET_LEDS - 0))                        \
  | (1u << (RANG_SMALL_RESET_RELES - 0))                       \
  | (1u << (RANG_SMALL_MISCEVE_DYSTANCIJNE - 0))               \
  | (1u << (RANG_SMALL_STATE_VV - 0))                          \
  | (1u << (RANG_SMALL_OTKL_VID_ZOVN_ZAHYSTIV - 0))            \
  | (1u << (RANG_SMALL_OTKL_VV - 0))                           \
  | (1u << (RANG_SMALL_VKL_VV - 0))                            \
  | (1u << (RANG_SMALL_CTRL_OTKL - 0))                         \
  | (1u << (RANG_SMALL_CTRL_VKL - 0))                          \
  | (1u << (RANG_SMALL_1_GRUPA_USTAVOK - 0))                   \
  | (1u << (RANG_SMALL_2_GRUPA_USTAVOK - 0))                   \
  | (1u << (RANG_SMALL_3_GRUPA_USTAVOK - 0))                   \
  | (1u << (RANG_SMALL_4_GRUPA_USTAVOK - 0))                   \
  | (1u << (RANG_SMALL_RESET_BLOCK_READY_TU_VID_ZAHYSTIV - 0)) \
  | (1u << (RANG_SMALL_OSNOVNYJ_TN2_RPN - 0))                  \
  | (1u << (RANG_SMALL_MRZS_OR_LOCAL_MODE_RPN - 0))            \
  | (1u << (RANG_SMALL_TM_MODE_VID_DV_RPN - 0))                \
  | (1u << (RANG_SMALL_AUTO_MODE_FROM_DV_RPN - 0))             \
  | (1u << (RANG_SMALL_PEREKLYUCHENNYA_RPN - 0))               \
  | (1u << (RANG_SMALL_UBAVYTY_FROM_DV_RPN - 0))               \
  | (1u << (RANG_SMALL_PRYBAVYTY_FROM_DV_RPN - 0))             \
  | (1u << (RANG_SMALL_BLOCK_STRUM_KOMP_RPN - 0))              \
  | (1u << (RANG_SMALL_ZOVNISHNJE_BLOCKUVANNJA_RPN - 0))       \
  | (1u << (RANG_SMALL_1_POLOGENNJA_RPN - 0))                  \
  | (1u << (RANG_SMALL_NOMINALNA_POZYCIJA_RPN - 0))            \
  | (1u << (RANG_SMALL_N_POLOGENNJA_RPN - 0))                  \
  | (1u << (RANG_SMALL_CLEAR_BLK_RPN - 0))                     \
  | (1u << (RANG_SMALL_KONTROL_UBAVYTY_ZSKh - 0))              \
  | (1u << (RANG_SMALL_KONTROL_PRYBAVYTY_ZSKh - 0))            \
)

#define MASKA_INPUT_SIGNALS_1 (                                \
    (1u << (RANG_SMALL_BLOCK_BRP - 32))                        \
  | (1u << (RANG_SMALL_BLOCK_UMAX2 - 32))                      \
  | (1u << (RANG_SMALL_BLOCK_UMIN1 - 32))                      \
  | (1u << (RANG_SMALL_BLOCK_UMIN2 - 32))                      \
  | (1u << (RANG_SMALL_BLOCK_UP1 + 0 - 32))                    \
  | (1u << (RANG_SMALL_BLOCK_UP1 + 1 - 32))                    \
  | (1u << (RANG_SMALL_BLOCK_UP1 + 2 - 32))                    \
  | (1u << (RANG_SMALL_BLOCK_UP1 + 3 - 32))                    \
  | (1u << (RANG_SMALL_BLOCK_UP1 + 4 - 32))                    \
  | (1u << (RANG_SMALL_BLOCK_UP1 + 5 - 32))                    \
  | (1u << (RANG_SMALL_BLOCK_UP1 + 6 - 32))                    \
  | (1u << (RANG_SMALL_BLOCK_UP1 + 7 - 32))                    \
  | (1u << (RANG_SMALL_DF1_IN - 32))                           \
  | (1u << (RANG_SMALL_DF1_R - 32))                            \
  | (1u << (RANG_SMALL_DF2_IN - 32))                           \
  | (1u << (RANG_SMALL_DF2_R - 32))                            \
  | (1u << (RANG_SMALL_DF3_IN - 32))                           \
  | (1u << (RANG_SMALL_DF3_R - 32))                            \
  | (1u << (RANG_SMALL_DF4_IN - 32))                           \
  | (1u << (RANG_SMALL_DF4_R - 32))                            \
  | (1u << (RANG_SMALL_DF5_IN - 32))                           \
  | (1u << (RANG_SMALL_DF5_R - 32))                            \
  | (1u << (RANG_SMALL_DF6_IN - 32))                           \
  | (1u << (RANG_SMALL_DF6_R - 32))                            \
  | (1u << (RANG_SMALL_DF7_IN - 32))                           \
  | (1u << (RANG_SMALL_DF7_R - 32))                            \
  | (1u << (RANG_SMALL_DF8_IN - 32))                           \
  | (1u << (RANG_SMALL_DF8_R - 32))                            \
  | (1u << (RANG_SMALL_DT1_SET - 32))                          \
  | (1u << (RANG_SMALL_DT1_RESET - 32))                        \
  | (1u << (RANG_SMALL_DT2_SET - 32))                          \
  | (1u << (RANG_SMALL_DT2_RESET - 32))                        \
  | (1u << (RANG_SMALL_DT3_SET - 32))                          \
  | (1u << (RANG_SMALL_DT3_RESET - 32))                        \
  | (1u << (RANG_SMALL_DT4_SET - 32))                          \
  | (1u << (RANG_SMALL_DT4_RESET - 32))                        \
  | (1u << (RANG_SMALL_LF1 - 32))                              \
  | (1u << (RANG_SMALL_LF2 - 32))                              \
  | (1u << (RANG_SMALL_LF3 - 32))                              \
  | (1u << (RANG_SMALL_LF4 - 32))                              \
)

#define MASKA_INPUT_SIGNALS_2 (                                \
    (1u << (RANG_SMALL_LF5 - 64))                              \
  | (1u << (RANG_SMALL_LF6 - 64))                              \
  | (1u << (RANG_SMALL_LF7 - 64))                              \
  | (1u << (RANG_SMALL_LF8 - 64))                              \
  | (1u << (RANG_SMALL_LF9 - 64))                              \
  | (1u << (RANG_SMALL_LF10 - 64))                             \
  | (1u << (RANG_SMALL_LF11 - 64))                             \
  | (1u << (RANG_SMALL_LF12 - 64))                             \
  | (1u << (RANG_SMALL_LF13 - 64))                             \
  | (1u << (RANG_SMALL_LF14 - 64))                             \
  | (1u << (RANG_SMALL_LF15 - 64))                             \
  | (1u << (RANG_SMALL_LF16 - 64))                             \
)
/*****************************************/

/*****************************************/
//Макски сигналів, які активовуються або з д.входів, або з інтирфейсів чи функціональних кнопок
/*****************************************/

#define MASKA_FOR_INPUT_SIGNALS_0 (                              \
    (1u << (RANG_BLOCK_VKL_VV - 0))                              \
  | (1u << (RANG_RESET_LEDS - 0))                                \
  | (1u << (RANG_RESET_RELES - 0))                               \
  | (1u << (RANG_MISCEVE_DYSTANCIJNE - 0))                       \
  | (1u << (RANG_STATE_VV - 0))                                  \
  | (1u << (RANG_OTKL_VID_ZOVN_ZAHYSTIV - 0))                    \
  | (1u << (RANG_OTKL_VV - 0))                                   \
  | (1u << (RANG_VKL_VV - 0))                                    \
  | (1u << (RANG_CTRL_OTKL - 0))                                 \
  | (1u << (RANG_CTRL_VKL - 0))                                  \
  | (1u << (RANG_RESET_BLOCK_READY_TU_VID_ZAHYSTIV - 0))         \
  | (1u << (RANG_OSNOVNYJ_TN2_RPN - 0))                          \
)

#define MASKA_FOR_INPUT_SIGNALS_1 (                              \
    (1u << (RANG_MRZS_OR_LOCAL_MODE_RPN - 32))                   \
  | (1u << (RANG_TM_MODE_VID_DV_RPN - 32))                       \
  | (1u << (RANG_AUTO_MODE_FROM_DV_RPN - 32))                    \
  | (1u << (RANG_PEREKLYUCHENNYA_RPN - 32))                      \
  | (1u << (RANG_UBAVYTY_FROM_DV_RPN - 32))                      \
  | (1u << (RANG_PRYBAVYTY_FROM_DV_RPN - 32))                    \
  | (1u << (RANG_BLOCK_STRUM_KOMP_RPN - 32))                     \
  | (1u << (RANG_ZOVNISHNJE_BLOCKUVANNJA_RPN - 32))              \
  | (1u << (RANG_1_POLOGENNJA_RPN - 32))                         \
  | (1u << (RANG_NOMINALNA_POZYCIJA_RPN - 32))                   \
  | (1u << (RANG_N_POLOGENNJA_RPN - 32))                         \
  | (1u << (RANG_CLEAR_BLK_RPN - 32))                            \
  | (1u << (RANG_UBAVYTY_FROM_KB_OR_UPPER_LEVEL_RPN - 32))       \
)

#define MASKA_FOR_INPUT_SIGNALS_2 (                              \
    (1u << (RANG_PRYBAVYTY_FROM_KB_OR_UPPER_LEVEL_RPN - 64))     \
  | (1u << (RANG_KONTROL_UBAVYTY_ZSKh - 64))                     \
  | (1u << (RANG_KONTROL_PRYBAVYTY_ZSKh - 64))                   \
  | (1u << (RANG_BLOCK_BRP - 64))                                \
  | (1u << (RANG_BLOCK_UMAX2 - 64))                              \
)

#define MASKA_FOR_INPUT_SIGNALS_3 (                             \
    (1u << (RANG_BLOCK_UMIN1 - 96))                             \
  | (1u << (RANG_BLOCK_UMIN2 - 96))                             \
  | (1u << (RANG_BLOCK_UP1 + 3*0 - 96))                         \
  | (1u << (RANG_BLOCK_UP1 + 3*1 - 96))                         \
  | (1u << (RANG_BLOCK_UP1 + 3*2 - 96))                         \
  | (1u << (RANG_BLOCK_UP1 + 3*3 - 96))                         \
  | (1u << (RANG_BLOCK_UP1 + 3*4 - 96))                         \
  | (1u << (RANG_BLOCK_UP1 + 3*5 - 96))                         \
  | (1u << (RANG_BLOCK_UP1 + 3*6 - 96))                         \
  | (1u << (RANG_BLOCK_UP1 + 3*7 - 96))                         \
  | (1u << (RANG_DF1_IN - 96))                                  \
  | (1u << (RANG_DF1_R -  96))                                  \
  | (1u << (RANG_DF2_IN -  96))                                 \
  | (1u << (RANG_DF2_R -  96))                                  \
  | (1u << (RANG_DF3_IN -  96))                                 \
  | (1u << (RANG_DF3_R -  96))                                  \
  | (1u << (RANG_DF4_IN -  96))                                 \
  | (1u << (RANG_DF4_R -  96))                                  \
  | (1u << (RANG_DF5_IN -  96))                                 \
  | (1u << (RANG_DF5_R -  96))                                  \
  | (1u << (RANG_DF6_IN -  96))                                 \
  | (1u << (RANG_DF6_R -  96))                                  \
  | (1u << (RANG_DF7_IN -  96))                                 \
  | (1u << (RANG_DF7_R -  96))                                  \
  | (1u << (RANG_DF8_IN -  96))                                 \
  | (1u << (RANG_DF8_R -  96))                                  \
)

#define MASKA_FOR_INPUT_SIGNALS_4 (                            \
    (1u << (RANG_DT1_SET -  128))                              \
  | (1u << (RANG_DT1_RESET - 128))                             \
  | (1u << (RANG_DT2_SET - 128))                               \
  | (1u << (RANG_DT2_RESET - 128))                             \
  | (1u << (RANG_DT3_SET - 128))                               \
  | (1u << (RANG_DT3_RESET - 128))                             \
  | (1u << (RANG_DT4_SET - 128))                               \
  | (1u << (RANG_DT4_RESET - 128))                             \
)

#define MASKA_FOR_INPUT_SIGNALS_5 (                            \
    (1u << (RANG_LF1 -  160))                                  \
  | (1u << (RANG_LF2 -  160))                                  \
  | (1u << (RANG_LF3 -  160))                                  \
  | (1u << (RANG_LF4 -  160))                                  \
  | (1u << (RANG_LF5 -  160))                                  \
  | (1u << (RANG_LF6 -  160))                                  \
  | (1u << (RANG_LF7 -  160))                                  \
  | (1u << (RANG_LF8 -  160))                                  \
  | (1u << (RANG_LF9 -  160))                                  \
  | (1u << (RANG_LF10 -  160))                                 \
  | (1u << (RANG_LF11 -  160))                                 \
)

#define MASKA_FOR_INPUT_SIGNALS_6 (                            \
    (1u << (RANG_LF12 -  192))                                 \
  | (1u << (RANG_LF13 -  192))                                 \
  | (1u << (RANG_LF14 -  192))                                 \
  | (1u << (RANG_LF15 -  192))                                 \
  | (1u << (RANG_LF16 - 192))                                  \
)
/*****************************************/

/*****************************************/
//Макски всіх сигналів РПН
/*****************************************/
#define MASKA_RPN_SIGNALS_0 (                                   \
    (1u << (RANG_OSNOVNYJ_TN2_RPN - 0))                         \
)

#define MASKA_RPN_SIGNALS_1 (                                   \
    (1u << (RANG_MRZS_OR_LOCAL_MODE_RPN - 32))                  \
  | (1u << (RANG_TM_MODE_VID_DV_RPN - 32))                      \
  | (1u << (RANG_TM_MODE_RPN - 32))                             \
  | (1u << (RANG_AUTO_MODE_FROM_DV_RPN - 32))                   \
  | (1u << (RANG_REMOTE_MODE_FROM_UPPER_LEVEL_RPN - 32))        \
  | (1u << (RANG_REMOTE_MODE_RPN - 32))                         \
  | (1u << (RANG_AUTO_MODE_FROM_UPPER_LEVEL_RPN - 32))          \
  | (1u << (RANG_AUTO_MODE_RPN - 32))                           \
  | (1u << (RANG_PEREKLYUCHENNYA_RPN - 32))                     \
  | (1u << (RANG_UBAVYTY_FROM_UPPER_LEVEL_RPN - 32))            \
  | (1u << (RANG_UBAVYTY_FROM_KB_RPN - 32))                     \
  | (1u << (RANG_UBAVYTY_FROM_DV_RPN - 32))                     \
  | (1u << (RANG_UBAVYTY_R_RPN - 32))                           \
  | (1u << (RANG_UBAVYTY_T_RPN - 32))                           \
  | (1u << (RANG_UBAVYTY_DI_RPN - 32))                          \
  | (1u << (RANG_UBAVYTY_1_RPN - 32))                           \
  | (1u << (RANG_UBAVYTY_RPN - 32))                             \
  | (1u << (RANG_PRYBAVYTY_FROM_UPPER_LEVEL_RPN - 32))          \
  | (1u << (RANG_PRYBAVYTY_FROM_KB_RPN - 32))                   \
  | (1u << (RANG_PRYBAVYTY_FROM_DV_RPN - 32))                   \
  | (1u << (RANG_PRYBAVYTY_R_RPN - 32))                         \
  | (1u << (RANG_PRYBAVYTY_T_RPN - 32))                         \
  | (1u << (RANG_PRYBAVYTY_DI_RPN - 32))                        \
  | (1u << (RANG_PRYBAVYTY_1_RPN - 32))                         \
  | (1u << (RANG_PRYBAVYTY_RPN - 32))                           \
  | (1u << (RANG_BLOCK_STRUM_KOMP_RPN - 32))                    \
  | (1u << (RANG_ZOVNISHNJE_BLOCKUVANNJA_RPN - 32))             \
  | (1u << (RANG_1_POLOGENNJA_RPN - 32))                        \
  | (1u << (RANG_NOMINALNA_POZYCIJA_RPN - 32))                  \
  | (1u << (RANG_N_POLOGENNJA_RPN - 32))                        \
  | (1u << (RANG_CLEAR_BLK_RPN - 32))                           \
  | (1u << (RANG_UBAVYTY_FROM_KB_OR_UPPER_LEVEL_RPN - 32))      \
)

#define MASKA_RPN_SIGNALS_2 (                                   \
    (1u << (RANG_PRYBAVYTY_FROM_KB_OR_UPPER_LEVEL_RPN - 64))    \
  | (1u << (RANG_BLOCKUVANNJA_RPN - 64))                        \
  | (1u << (RANG_RESET_DEFECT_RPN - 64))                        \
  | (1u << (RANG_PO1_U_OSN_RPN - 64))                           \
  | (1u << (RANG_PO1_U_OSN_KOMP_RPN - 64))                      \
  | (1u << (RANG_VYJCHE_ZONY_RPN - 64))                         \
  | (1u << (RANG_PO1_U_VSPOM_RPN - 64))                         \
  | (1u << (RANG_PO2_U_OSN_RPN - 64))                           \
  | (1u << (RANG_PO2_U_OSN_KOMP_RPN - 64))                      \
  | (1u << (RANG_NYJCHE_ZONY_RPN - 64))                         \
  | (1u << (RANG_PO3_U_OSN_RPN - 64))                           \
  | (1u << (RANG_PO3_U_VSPOM_RPN - 64))                         \
  | (1u << (RANG_PO4_U_OSN_RPN - 64))                           \
  | (1u << (RANG_DOZVIL_UBAVYTY_RPN - 64))                      \
  | (1u << (RANG_DOZVIL_AVTO_UBAVYTY_RPN - 64))                 \
  | (1u << (RANG_DOZVIL_PRYBAVYTY_RPN - 64))                    \
  | (1u << (RANG_DOZVIL_AVTO_PRYBAVYTY_RPN - 64))               \
  | (1u << (RANG_ZASTRJAVANNJA_RPN - 64))                       \
  | (1u << (RANG_PRYVID_NE_PISHOV_RPN - 64))                    \
  | (1u << (RANG_N_PER_RIVNE_N_PER_USTAVKY_RPN - 64))           \
  | (1u << (RANG_NESPRAVNIST_RPN - 64))                         \
)

#define MASKA_RPN_SIGNALS_3    0
#define MASKA_RPN_SIGNALS_4    0
#define MASKA_RPN_SIGNALS_5    0
#define MASKA_RPN_SIGNALS_6    0
/*****************************************/

/*****************************************/
//Макски всіх сигналів ЗСХ
/*****************************************/
#define MASKA_ZSKh_SIGNALS_0    0
#define MASKA_ZSKh_SIGNALS_1    0

#define MASKA_ZSKh_SIGNALS_2 (                                   \
    (1u << (RANG_KONTROL_UBAVYTY_ZSKh - 64))                     \
  | (1u << (RANG_KONTROL_PRYBAVYTY_ZSKh - 64))                   \
  | (1u << (RANG_ZSKh - 64))                                     \
)

#define MASKA_ZSKh_SIGNALS_3    0
#define MASKA_ZSKh_SIGNALS_4    0
#define MASKA_ZSKh_SIGNALS_5    0
#define MASKA_ZSKh_SIGNALS_6    0
/*****************************************/

/*****************************************/
//Макски всіх сигналів ЗСН
/*****************************************/
#define MASKA_ZNKh_SIGNALS_0    0
#define MASKA_ZNKh_SIGNALS_1    0

#define MASKA_ZNKh_SIGNALS_2 (                                   \
    (1u << (RANG_ZNKh_IR - 64))                                  \
  | (1u << (RANG_ZNKh_BR - 64))                                  \
  | (1u << (RANG_ZNKh - 64))                                     \
)

#define MASKA_ZNKh_SIGNALS_3    0
#define MASKA_ZNKh_SIGNALS_4    0
#define MASKA_ZNKh_SIGNALS_5    0
#define MASKA_ZNKh_SIGNALS_6    0
/*****************************************/

/*****************************************/
//Макски всіх сигналів БРП
/*****************************************/
#define MASKA_BRP_SIGNALS_0    0
#define MASKA_BRP_SIGNALS_1    0

#define MASKA_BRP_SIGNALS_2 (                                    \
    (1u << (RANG_BLOCK_BRP - 64))                                \
  | (1u << (RANG_BRP - 64))                                      \
)

#define MASKA_BRP_SIGNALS_3    0
#define MASKA_BRP_SIGNALS_4    0
#define MASKA_BRP_SIGNALS_5    0
#define MASKA_BRP_SIGNALS_6    0
/*****************************************/

/*****************************************/
//Макски всіх сигналів UMAX
/*****************************************/
#define MASKA_UMAX_SIGNALS_0    0
#define MASKA_UMAX_SIGNALS_1    0

#define MASKA_UMAX_SIGNALS_2 (                                   \
    (1u << (RANG_BLOCK_UMAX2 - 64))                              \
  | (1u << (RANG_PO_UMAX1 - 64))                                 \
  | (1u << (RANG_PRYSK_UBAVYTY_VID_UMAX1 - 64))                  \
)

#define MASKA_UMAX_SIGNALS_3 (                                   \
    (1u << (RANG_BLOCK_PRYBAVYTY_VID_UMAX1 - 96))                \
  | (1u << (RANG_PO_UMAX2 - 96))                                 \
  | (1u << (RANG_UMAX2 - 96))                                    \
)

#define MASKA_UMAX_SIGNALS_4    0
#define MASKA_UMAX_SIGNALS_5    0
#define MASKA_UMAX_SIGNALS_6    0
/*****************************************/

/*****************************************/
//Макски всіх сигналів UMIN
/*****************************************/
#define MASKA_UMIN_SIGNALS_0    0
#define MASKA_UMIN_SIGNALS_1    0
#define MASKA_UMIN_SIGNALS_2    0

#define MASKA_UMIN_SIGNALS_3 (                                    \
    (1u << (RANG_BLOCK_UMIN1 - 96))                               \
  | (1u << (RANG_BLOCK_UMIN2 - 96))                               \
  | (1u << (RANG_PO_UMIN1 - 96))                                  \
  | (1u << (RANG_UMIN1 - 96))                                     \
  | (1u << (RANG_PO_UMIN2 - 96))                                  \
  | (1u << (RANG_UMIN2 - 96))                                     \
)

#define MASKA_UMIN_SIGNALS_4    0
#define MASKA_UMIN_SIGNALS_5    0
#define MASKA_UMIN_SIGNALS_6    0
/*****************************************/

/*****************************************/
//Макски всіх сигналів Універсального Захисту
/*****************************************/
#define MASKA_UP_SIGNALS_0    0
#define MASKA_UP_SIGNALS_1    0
#define MASKA_UP_SIGNALS_2    0

#define MASKA_UP_SIGNALS_3 (                                    \
     (1u << (RANG_BLOCK_UP1 + 3*0 - 96))                        \
   | (1u << (RANG_PO_UP1 + 3*0 - 96))                           \
   | (1u << (RANG_UP1 + 3*0 - 96))                              \
   | (1u << (RANG_BLOCK_UP1 + 3*1 - 96))                        \
   | (1u << (RANG_PO_UP1 + 3*1 - 96))                           \
   | (1u << (RANG_UP1 + 3*1 - 96))                              \
   | (1u << (RANG_BLOCK_UP1 + 3*2 - 96))                        \
   | (1u << (RANG_PO_UP1 + 3*2 - 96))                           \
   | (1u << (RANG_UP1 + 3*2 - 96))                              \
   | (1u << (RANG_BLOCK_UP1 + 3*3 - 96))                        \
   | (1u << (RANG_PO_UP1 + 3*3 - 96))                           \
   | (1u << (RANG_UP1 + 3*3 - 96))                              \
   | (1u << (RANG_BLOCK_UP1 + 3*4 - 96))                        \
   | (1u << (RANG_PO_UP1 + 3*4 - 96))                           \
   | (1u << (RANG_UP1 + 3*4 - 96))                              \
   | (1u << (RANG_BLOCK_UP1 + 3*5 - 96))                        \
   | (1u << (RANG_PO_UP1 + 3*5 - 96))                           \
   | (1u << (RANG_UP1 + 3*5 - 96))                              \
   | (1u << (RANG_BLOCK_UP1 + 3*6 - 96))                        \
   | (1u << (RANG_PO_UP1 + 3*6 - 96))                           \
   | (1u << (RANG_UP1 + 3*6 - 96))                              \
   | (1u << (RANG_BLOCK_UP1 + 3*7 - 96))                        \
   | (1u << (RANG_PO_UP1 + 3*7 - 96))                           \
   | (1u << (RANG_UP1 + 3*7 - 96))                              \
)

#define MASKA_UP_SIGNALS_4    0
#define MASKA_UP_SIGNALS_5    0
#define MASKA_UP_SIGNALS_6    0
/*****************************************/

/*****************************************/
//Макски всіх сигналів розширеної логіки
/*****************************************/
#define MASKA_EL_SIGNALS_0    0
#define MASKA_EL_SIGNALS_1    0
#define MASKA_EL_SIGNALS_2    0

#define MASKA_EL_SIGNALS_3 (                                   \
    (1u << (RANG_DF1_IN - 96))                                 \
  | (1u << (RANG_DF1_R - 96))                                  \
  | (1u << (RANG_DF1_OUT - 96))                                \
  | (1u << (RANG_DF2_IN - 96))                                 \
  | (1u << (RANG_DF2_R - 96))                                  \
  | (1u << (RANG_DF2_OUT - 96))                                \
  | (1u << (RANG_DF3_IN - 96))                                 \
  | (1u << (RANG_DF3_R - 96))                                  \
  | (1u << (RANG_DF3_OUT - 96))                                \
  | (1u << (RANG_DF4_IN - 96))                                 \
  | (1u << (RANG_DF4_R - 96))                                  \
  | (1u << (RANG_DF4_OUT - 96))                                \
  | (1u << (RANG_DF5_IN - 96))                                 \
  | (1u << (RANG_DF5_R - 96))                                  \
  | (1u << (RANG_DF5_OUT - 96))                                \
  | (1u << (RANG_DF6_IN - 96))                                 \
  | (1u << (RANG_DF6_R - 96))                                  \
  | (1u << (RANG_DF6_OUT - 96))                                \
  | (1u << (RANG_DF7_IN - 96))                                 \
  | (1u << (RANG_DF7_R - 96))                                  \
  | (1u << (RANG_DF7_OUT - 96))                                \
  | (1u << (RANG_DF8_IN - 96))                                 \
  | (1u << (RANG_DF8_R - 96))                                  \
)

#define MASKA_EL_SIGNALS_4 (                                   \
    (1u << (RANG_DF8_OUT - 128))                               \
  | (1u << (RANG_DT1_SET - 128))                               \
  | (1u << (RANG_DT1_RESET - 128))                             \
  | (1u << (RANG_DT1_OUT - 128))                               \
  | (1u << (RANG_DT2_SET - 128))                               \
  | (1u << (RANG_DT2_RESET - 128))                             \
  | (1u << (RANG_DT2_OUT - 128))                               \
  | (1u << (RANG_DT3_SET - 128))                               \
  | (1u << (RANG_DT3_RESET - 128))                             \
  | (1u << (RANG_DT3_OUT - 128))                               \
  | (1u << (RANG_DT4_SET - 128))                               \
  | (1u << (RANG_DT4_RESET - 128))                             \
  | (1u << (RANG_DT4_OUT - 128))                               \
  | (1u << (RANG_D_AND1 - 128))                                \
  | (1u << (RANG_D_AND2 - 128))                                \
  | (1u << (RANG_D_AND3 - 128))                                \
  | (1u << (RANG_D_AND4 - 128))                                \
  | (1u << (RANG_D_AND5 - 128))                                \
  | (1u << (RANG_D_AND6 - 128))                                \
  | (1u << (RANG_D_AND7 - 128))                                \
  | (1u << (RANG_D_AND8 - 128))                                \
  | (1u << (RANG_D_OR1 - 128))                                 \
  | (1u << (RANG_D_OR2 - 128))                                 \
  | (1u << (RANG_D_OR3 - 128))                                 \
  | (1u << (RANG_D_OR4 - 128))                                 \
  | (1u << (RANG_D_OR5 - 128))                                 \
  | (1u << (RANG_D_OR6 - 128))                                 \
  | (1u << (RANG_D_OR7 - 128))                                 \
  | (1u << (RANG_D_OR8 - 128))                                 \
  | (1u << (RANG_D_XOR1 - 128))                                \
  | (1u << (RANG_D_XOR2 - 128))                                \
  | (1u << (RANG_D_XOR3 - 128))                                \
)

#define MASKA_EL_SIGNALS_5 (                                   \
    (1u << (RANG_D_XOR4 - 160))                                 \
  | (1u << (RANG_D_XOR5 - 160))                                 \
  | (1u << (RANG_D_XOR6 - 160))                                 \
  | (1u << (RANG_D_XOR7 - 160))                                 \
  | (1u << (RANG_D_XOR8 - 160))                                 \
  | (1u << (RANG_D_NOT1 - 160))                                 \
  | (1u << (RANG_D_NOT2 - 160))                                 \
  | (1u << (RANG_D_NOT3 - 160))                                 \
  | (1u << (RANG_D_NOT4 - 160))                                 \
  | (1u << (RANG_D_NOT5 - 160))                                 \
  | (1u << (RANG_D_NOT6 - 160))                                 \
  | (1u << (RANG_D_NOT7 - 160))                                 \
  | (1u << (RANG_D_NOT8 - 160))                                 \
  | (1u << (RANG_D_NOT9 - 160))                                 \
  | (1u << (RANG_D_NOT10 - 160))                                \
  | (1u << (RANG_D_NOT11 - 160))                                \
  | (1u << (RANG_D_NOT12 - 160))                                \
  | (1u << (RANG_D_NOT13 - 160))                                \
  | (1u << (RANG_D_NOT14 - 160))                                \
  | (1u << (RANG_D_NOT15 - 160))                                \
  | (1u << (RANG_D_NOT16 - 160))                                \
  | (1u << (RANG_LF1 - 160))                                    \
  | (1u << (RANG_LF2 - 160))                                    \
  | (1u << (RANG_LF3 - 160))                                    \
  | (1u << (RANG_LF4 - 160))                                    \
  | (1u << (RANG_LF5 - 160))                                    \
  | (1u << (RANG_LF6 - 160))                                    \
  | (1u << (RANG_LF7 - 160))                                    \
  | (1u << (RANG_LF8 - 160))                                    \
  | (1u << (RANG_LF9 - 160))                                    \
  | (1u << (RANG_LF10 - 160))                                   \
  | (1u << (RANG_LF11 - 160))                                   \
)

#define MASKA_EL_SIGNALS_6 (                                   \
    (1u << (RANG_LF12 - 192))                                  \
  | (1u << (RANG_LF13 - 192))                                  \
  | (1u << (RANG_LF14 - 192))                                  \
  | (1u << (RANG_LF15 - 192))                                  \
  | (1u << (RANG_LF16 - 192))                                  \
  | (1u << (RANG_ERROR_CONF_EL - 192))                         \
)

/*****************************************/


/*****************************************/
//Макска сигналів, які мають записуватися у енергонезалежну пам'ять
/*****************************************/
#define MASKA_TRIGGER_SIGNALES_0                  0
#define MASKA_TRIGGER_SIGNALES_1                  0
#define MASKA_TRIGGER_SIGNALES_2                  0
#define MASKA_TRIGGER_SIGNALES_3                  0

#define MASKA_TRIGGER_SIGNALES_4 (                             \
     (1u << (RANG_DT1_OUT - 128))                              \
   | (1u << (RANG_DT2_OUT - 128))                              \
   | (1u << (RANG_DT3_OUT - 128))                              \
   | (1u << (RANG_DT4_OUT - 128))                              \
)     

#define MASKA_TRIGGER_SIGNALES_5                  0
#define MASKA_TRIGGER_SIGNALES_6                  0
/*****************************************/

/*****************************************/
//Макска для блокування зміни груп  уставок
/*****************************************/
#define MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_0 (               \
     (1u << (RANG_WORK_BO - 0))                                \
   | (1u << (RANG_WORK_BV - 0))                                \
)

#define MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_1  0
#define MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_2  0
#define MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_3  0
#define MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_4  0
#define MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_5  0
#define MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_6  0
/*****************************************/

/*****************************************/
//Макска інформативних сигналів
/*****************************************/

#define MASKA_INFO_SIGNALES_0 (                                \
     (1u << (RANG_STATE_VV - 0))                               \
   | (1u << (RANG_DEFECT - 0))                                 \
   | (1u << (RANG_AVAR_DEFECT - 0))                            \
   | (1u << (RANG_WORK_A_REJESTRATOR - 0))                     \
   | (1u << (RANG_WORK_D_REJESTRATOR - 0))                     \
   | (1u << (RANG_SETTINGS_CHANGED - 0))                       \
)     

#define MASKA_INFO_SIGNALES_1                  0
#define MASKA_INFO_SIGNALES_2                  0
#define MASKA_INFO_SIGNALES_3                  0
#define MASKA_INFO_SIGNALES_4                  0
#define MASKA_INFO_SIGNALES_5                  0
#define MASKA_INFO_SIGNALES_6                  0
/*****************************************/
// clang-format on

/*****************************************/
//Відключення від захистів
/*****************************************/
typedef enum __vymknennja_vid_zakhystiv__
{
  VYMKNENNJA_VID_MTZ1,
  VYMKNENNJA_VID_MTZ2,
  VYMKNENNJA_VID_MTZ3,
  VYMKNENNJA_VID_MTZ4,

  VYMKNENNJA_VID_MTZ04_1,
  VYMKNENNJA_VID_MTZ04_2,

  VYMKNENNJA_VID_ZDZ,

  VYMKNENNJA_VID_3I0,
  VYMKNENNJA_VID_3U0,
  VYMKNENNJA_VID_NZZ,

  VYMKNENNJA_VID_TZNP1,
  VYMKNENNJA_VID_TZNP2,
  VYMKNENNJA_VID_TZNP3,

  VYMKNENNJA_VID_ACHR_CHAPV_VID_DV,
  VYMKNENNJA_VID_ACHR_CHAPV1,
  VYMKNENNJA_VID_ACHR_CHAPV2,

  VYMKNENNJA_VID_UROV1,
  VYMKNENNJA_VID_UROV2,

  VYMKNENNJA_VID_ZOP,

  VYMKNENNJA_VID_UMIN1,
  VYMKNENNJA_VID_UMIN2,

  VYMKNENNJA_VID_UMAX1,
  VYMKNENNJA_VID_UMAX2,

  VYMKNENNJA_VID_UP1,

  VYMKNENNJA_VID_ZOVNISHNIKH_ZAKHYSTIV = VYMKNENNJA_VID_UP1 + NUMBER_UP,

  VYMKNENNJA_VID_INSHYKH_SYGNALIV,

  VYMKNENNJA_VID_MAX_NUMBER

} __vymknennja_vid_zakhystiv;
/*****************************************/

/*****************************************/
//Світлоіндикатори
/*****************************************/
//Нумерація  усправлінських світлоіндикаторів
enum _leds_ctrl
{
  LED_CTRL_R_E = 0,
  LED_CTRL_START,
  LED_CTRL_TRIP,
  LED_CTRL_I,
  LED_CTRL_O
};

enum _leds_colors
{
  LED_COLOR_RED_BIT = 0,
  LED_COLOR_GREEN_BIT,

  NUMBER_LED_COLOR
};

//Кількість управлінських стовпців
#define LED_N_COL 7
#define LED_N_ROW 8
/*****************************************/

/*****************************************/
//Максимальна кількість сигналів (функцій), які можуть бути зранжовані на входи/виходи/світлоіндикатори/д.реєстратор/оф
/*****************************************/
#define VAGA_MAX_FUNCTIONS_IN_INPUT 3
#define MAX_FUNCTIONS_IN_INPUT (1 << VAGA_MAX_FUNCTIONS_IN_INPUT)
#define VAGA_MAX_FUNCTIONS_IN_OUTPUT 4
#define MAX_FUNCTIONS_IN_OUTPUT (1 << VAGA_MAX_FUNCTIONS_IN_OUTPUT)
#define VAGA_MAX_FUNCTIONS_IN_LED 3
#define MAX_FUNCTIONS_IN_LED (1 << VAGA_MAX_FUNCTIONS_IN_LED)
#define VAGA_MAX_FUNCTIONS_IN_DF 4
#define MAX_FUNCTIONS_IN_DF (1 << VAGA_MAX_FUNCTIONS_IN_DF)
#define MAX_FUNCTIONS_IN_DT 6
#define MAX_FUNCTIONS_IN_D_AND NUMBER_IN_AND
#define MAX_FUNCTIONS_IN_D_OR NUMBER_IN_OR
#define MAX_FUNCTIONS_IN_D_XOR 2
#define MAX_FUNCTIONS_IN_D_NOT 1
#define VAGA_MAX_FUNCTIONS_IN_DB 3
#define MAX_FUNCTIONS_IN_DB (1 << VAGA_MAX_FUNCTIONS_IN_DB)
#define VAGA_MAX_FUNCTIONS_IN_AREG 5
#define MAX_FUNCTIONS_IN_AREG (1 << VAGA_MAX_FUNCTIONS_IN_AREG)
#define VAGA_MAX_FUNCTIONS_IN_DREG 5
#define MAX_FUNCTIONS_IN_DREG (1 << VAGA_MAX_FUNCTIONS_IN_DREG)
#define VAGA_MAX_FUNCTIONS_IN_OFF_CB 5
#define MAX_FUNCTIONS_IN_OFF_CB (1 << VAGA_MAX_FUNCTIONS_IN_OFF_CB)
#define VAGA_MAX_FUNCTIONS_IN_ON_CB 5
#define MAX_FUNCTIONS_IN_ON_CB (1 << VAGA_MAX_FUNCTIONS_IN_ON_CB)
/*****************************************/

/*****************************************/
//Константи для реєстратора статистики
/*****************************************/
#define EXT_SRAM_512 1
/*****************************************/

/*****************************************/
//Константи для комунікації
/*****************************************/
#define MAX_CHAR_IN_NAME_OF_CELL 32
/*****************************************/

/*****************************************/
//Константи для фіксації зміни настройок
/*****************************************/
#define CHANGED_ETAP_NONE 0
#define CHANGED_ETAP_EXECUTION 1
#define CHANGED_ETAP_ENDED 2
#define CHANGED_ETAP_ENDED_EXTRA_ETAP 3
/*****************************************/

/*****************************************/
//Константи для ранжування функціональних кнопок
/*****************************************/
enum __odynyci_vymirjuvannja
{
  INDEX_A = 0,
  INDEX_V,
  INDEX_PERCENT,
  INDEX_SECOND,

  NUMBER_ODYNYCI_VYMIRJUVANNJA
};
/*****************************************/

/*****************************************/
//Індетифікатор типу набору юстуючих коефіцієнтів
/*****************************************/
//#define ADJUSTMENT_3I0_IA_IB_IC                             1
//#define ADJUSTMENT_3I0_IA_IC_3U0                            2
//#define ADJUSTMENT_3I0_IA_IB_IC_UA_UB_UC_UABTN2             3
#define ADJUSTMENT_3I0_IA_IB_IC_UA_UB_UC_3U0 4
#define ADJUSTMENT_ID ADJUSTMENT_3I0_IA_IB_IC_UA_UB_UC_3U0
/*****************************************/

/*****************************************/
//Версія програмного забезпечення
/*****************************************/
#define VERSIA_PZ 28
#define MODYFIKACIA_VERSII_PZ 1
#define ZBIRKA_VERSII_PZ 0
#define ZBIRKA_PIDVERSII_PZ 0
/*****************************************/

/*****************************************/
//Час компіляції
/*****************************************/

#define YEAR_VER ((__DATE__[9] != ' ') ? (((__DATE__[9] - 0x30) << 4) | (__DATE__[10] - 0x30)) : (__DATE__[10] - 0x30))
#define MONTH_VER str_to_int_DATE_Mmm()
#define DAY_VER ((__DATE__[4] != ' ') ? (((__DATE__[4] - 0x30) << 4) | (__DATE__[5] - 0x30)) : (__DATE__[5] - 0x30))
#define HOUR_VER (((__TIME__[0] - 0x30) << 4) | (__TIME__[1] - 0x30))
#define MINUTE_VER (((__TIME__[3] - 0x30) << 4) | (__TIME__[4] - 0x30))
#define SECOND_VER (((__TIME__[6] - 0x30) << 4) | (__TIME__[7] - 0x30))
/*****************************************/

/*****************************************/
//Версія універсальної карти памяті
/*****************************************/
#define VERSIA_GMM 8
#define MODYFIKACIA_VERSII_GMM 4
/*****************************************/
#endif
