#ifndef __CONST_GLOBAL__
#define __CONST_GLOBAL__

#define NUMBER_INPUTS 16
#define NUMBER_SIMPLE_OUTPUTS 16
#define NUMBER_OUTPUTS (NUMBER_SIMPLE_OUTPUTS)

#define NUMBER_LEDS 17

#define NUMBER_UP 8

#define N_IN_GOOSE 16
#define N_IN_MMS 4
#define N_OUT_LAN 4

#define N_IN_GOOSE_MMS_OUT 8
#define N_OUT_LAN_IN 8

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
  RANG_SMALL_RESET_LEDS = _RANG_SMALL_GENERAL_BEGIN,
  RANG_SMALL_RESET_RELES,
  RANG_SMALL_MISCEVE_DYSTANCIJNE,
  RANG_SMALL_1_GRUPA_USTAVOK,
  RANG_SMALL_2_GRUPA_USTAVOK,
  RANG_SMALL_3_GRUPA_USTAVOK,
  RANG_SMALL_4_GRUPA_USTAVOK,
  RANG_SMALL_RESET_BLOCK_READY_TU_VID_ZAHYSTIV,

  RANG_SMALL_BLOCK_IN_GOOSE1,

  RANG_SMALL_BLOCK_IN_MMS1 = (RANG_SMALL_BLOCK_IN_GOOSE1 + N_IN_GOOSE),

  RANG_SMALL_BLOCK_OUT_LAN1 = (RANG_SMALL_BLOCK_IN_MMS1 + N_IN_MMS),

  _RANG_SMALL_RPN_BEGIN = (RANG_SMALL_BLOCK_OUT_LAN1 + N_OUT_LAN),
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
  RANG_RESET_LEDS = _RANG_GENERAL_BEGIN,
  RANG_RESET_RELES,
  RANG_MISCEVE_DYSTANCIJNE,
  RANG_DI_ACTIVE,
  RANG_FK_ACTIVE,
  RANG_LAN_ACTIVE,
  RANG_GOOSE_ACTIVE,
  RANG_USB_ACTIVE,
  RANG_RS485_ACTIVE,
  RANG_DEFECT,
  RANG_AVAR_DEFECT,
  RANG_WORK_A_REJESTRATOR,
  RANG_WORK_D_REJESTRATOR,
  RANG_1_GRUPA_USTAVOK,
  RANG_2_GRUPA_USTAVOK,
  RANG_3_GRUPA_USTAVOK,
  RANG_4_GRUPA_USTAVOK,
  RANG_INVERS_DV_GRUPA_USTAVOK,
  RANG_BLK_GRUP_USTAVOK_VID_ZACHYSTIV,
  RANG_RESET_BLOCK_READY_TU_VID_ZAHYSTIV,
  RANG_READY_TU,
  RANG_SETTINGS_CHANGED,

  RANG_BLOCK_IN_GOOSE1,

  RANG_BLOCK_IN_MMS1 = (RANG_BLOCK_IN_GOOSE1 + N_IN_GOOSE),

  RANG_BLOCK_OUT_LAN1 = (RANG_BLOCK_IN_MMS1 + N_IN_MMS),

  _RANG_RPN_BEGIN = (RANG_BLOCK_OUT_LAN1 + N_OUT_LAN),
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
  | (1u << (RANG_SMALL_RESET_BLOCK_READY_TU_VID_ZAHYSTIV - 0))\
)

  #define COMMAND_SIGNALES_1 (                                    \
    (1u << (RANG_SMALL_CLEAR_BLK_RPN - 32))                       \
  | (1u << (RANG_SMALL_UBAVYTY_FROM_KB_OR_UPPER_LEVEL_RPN - 32))  \
  | (1u << (RANG_SMALL_PRYBAVYTY_FROM_KB_OR_UPPER_LEVEL_RPN - 32))\
  | (1u << (RANG_SMALL_DF1_IN - 32))                              \
  | (1u << (RANG_SMALL_DF1_R - 32))                           \
  | (1u << (RANG_SMALL_DF2_IN - 32))                          \
)

#define COMMAND_SIGNALES_2 (                                  \
    (1u << (RANG_SMALL_DF2_R - 64))                           \
  | (1u << (RANG_SMALL_DF3_IN - 64))                          \
  | (1u << (RANG_SMALL_DF3_R - 64))                           \
  | (1u << (RANG_SMALL_DF4_IN - 64))                          \
  | (1u << (RANG_SMALL_DF4_R - 64))                           \
  | (1u << (RANG_SMALL_DF5_IN - 64))                          \
  | (1u << (RANG_SMALL_DF5_R - 64))                           \
  | (1u << (RANG_SMALL_DF6_IN - 64))                          \
  | (1u << (RANG_SMALL_DF6_R - 64))                           \
  | (1u << (RANG_SMALL_DF7_IN - 64))                          \
  | (1u << (RANG_SMALL_DF7_R - 64))                           \
  | (1u << (RANG_SMALL_DF8_IN - 64))                          \
  | (1u << (RANG_SMALL_DF8_R - 64))                           \
  | (1u << (RANG_SMALL_DT1_SET - 64))                         \
  | (1u << (RANG_SMALL_DT1_RESET - 64))                       \
  | (1u << (RANG_SMALL_DT2_SET - 64))                         \
  | (1u << (RANG_SMALL_DT2_RESET - 64))                       \
  | (1u << (RANG_SMALL_DT3_SET - 64))                         \
  | (1u << (RANG_SMALL_DT3_RESET - 64))                       \
  | (1u << (RANG_SMALL_DT4_SET - 64))                         \
  | (1u << (RANG_SMALL_DT4_RESET - 64))                       \
  | (1u << (RANG_SMALL_LF1 - 64))                             \
  | (1u << (RANG_SMALL_LF2 - 64))                             \
  | (1u << (RANG_SMALL_LF3 - 64))                             \
  | (1u << (RANG_SMALL_LF4 - 64))                             \
  | (1u << (RANG_SMALL_LF5 - 64))                             \
  | (1u << (RANG_SMALL_LF6 - 64))                             \
  | (1u << (RANG_SMALL_LF7 - 64))                             \
  | (1u << (RANG_SMALL_LF8 - 64))                             \
  | (1u << (RANG_SMALL_LF9 - 64))                             \
  | (1u << (RANG_SMALL_LF10 - 64))                            \
  | (1u << (RANG_SMALL_LF11 - 64))                            \
)

#define COMMAND_SIGNALES_3 (                                  \
    (1u << (RANG_SMALL_LF12 - 96))                            \
  | (1u << (RANG_SMALL_LF13 - 96))                            \
  | (1u << (RANG_SMALL_LF14 - 96))                            \
  | (1u << (RANG_SMALL_LF15 - 96))                            \
  | (1u << (RANG_SMALL_LF16 - 96))                            \
)
/*****************************************/

/*****************************************/
//Макска команд, які можуть активовуватися з КП безпосередньо
/*****************************************/
#define COMMAND_PERMITTED_FROM_KP_SIGNALES_0 (  \
   (1u << (RANG_SMALL_RESET_LEDS - 0))          \
 | (1u << (RANG_SMALL_RESET_RELES - 0))         \
)

#define COMMAND_PERMITTED_FROM_KP_SIGNALES_1 0
#define COMMAND_PERMITTED_FROM_KP_SIGNALES_2 0
#define COMMAND_PERMITTED_FROM_KP_SIGNALES_3 0
/*****************************************/

/*****************************************/
//Макски сигналів, які можна зранжувати на функціональні кнопки у Режимі Кнопка
/*****************************************/
#define MASKA_BUTTON_MODE_0_SIGNALS_0 (                        \
    (1u << (RANG_SMALL_RESET_LEDS - 0))                        \
  | (1u << (RANG_SMALL_RESET_RELES - 0))                       \
  | (1u << (RANG_SMALL_RESET_BLOCK_READY_TU_VID_ZAHYSTIV - 0)) \
)

#define MASKA_BUTTON_MODE_0_SIGNALS_1 (                           \
    (1u << (RANG_SMALL_CLEAR_BLK_RPN - 32))                       \
  | (1u << (RANG_SMALL_UBAVYTY_FROM_KB_OR_UPPER_LEVEL_RPN - 32))  \
  | (1u << (RANG_SMALL_PRYBAVYTY_FROM_KB_OR_UPPER_LEVEL_RPN - 32))\
  | (1u << (RANG_SMALL_DF1_IN - 32))                              \
  | (1u << (RANG_SMALL_DF1_R - 32))                               \
  | (1u << (RANG_SMALL_DF2_IN - 32))                              \
)

#define MASKA_BUTTON_MODE_0_SIGNALS_2 (                        \
    (1u << (RANG_SMALL_DF2_R - 64))                            \
  | (1u << (RANG_SMALL_DF3_IN -64))                            \
  | (1u << (RANG_SMALL_DF3_R - 64))                            \
  | (1u << (RANG_SMALL_DF4_IN - 64))                           \
  | (1u << (RANG_SMALL_DF4_R - 64))                            \
  | (1u << (RANG_SMALL_DF5_IN - 64))                           \
  | (1u << (RANG_SMALL_DF5_R - 64))                            \
  | (1u << (RANG_SMALL_DF6_IN - 64))                           \
  | (1u << (RANG_SMALL_DF6_R - 64))                            \
  | (1u << (RANG_SMALL_DF7_IN - 64))                           \
  | (1u << (RANG_SMALL_DF7_R - 64))                            \
  | (1u << (RANG_SMALL_DF8_IN - 64))                           \
  | (1u << (RANG_SMALL_DF8_R - 64))                            \
  | (1u << (RANG_SMALL_DT1_SET - 64))                          \
  | (1u << (RANG_SMALL_DT1_RESET - 64))                        \
  | (1u << (RANG_SMALL_DT2_SET - 64))                          \
  | (1u << (RANG_SMALL_DT2_RESET - 64))                        \
  | (1u << (RANG_SMALL_DT3_SET - 64))                          \
  | (1u << (RANG_SMALL_DT3_RESET - 64))                        \
  | (1u << (RANG_SMALL_DT4_SET - 64))                          \
  | (1u << (RANG_SMALL_DT4_RESET - 64))                        \
  | (1u << (RANG_SMALL_LF1 - 64))                              \
  | (1u << (RANG_SMALL_LF2 - 64))                              \
  | (1u << (RANG_SMALL_LF3 - 64))                              \
  | (1u << (RANG_SMALL_LF4 - 64))                              \
  | (1u << (RANG_SMALL_LF5 - 64))                              \
  | (1u << (RANG_SMALL_LF6 - 64))                              \
  | (1u << (RANG_SMALL_LF7 - 64))                              \
  | (1u << (RANG_SMALL_LF8 - 64))                              \
  | (1u << (RANG_SMALL_LF9 - 64))                              \
  | (1u << (RANG_SMALL_LF10 - 64))                             \
  | (1u << (RANG_SMALL_LF11 - 64))                             \
)

#define MASKA_BUTTON_MODE_0_SIGNALS_3 (                        \
    (1u << (RANG_SMALL_LF12 - 96))                             \
  | (1u << (RANG_SMALL_LF13 - 96))                             \
  | (1u << (RANG_SMALL_LF14 - 96))                             \
  | (1u << (RANG_SMALL_LF15 - 96))                             \
  | (1u << (RANG_SMALL_LF16 - 96))                             \
)
/*****************************************/

/*****************************************/
//Макски сигналів, які можна зранжувати на функціональні кнопки у Режимі Ключ
/*****************************************/
#define MASKA_BUTTON_MODE_1_SIGNALS_0 (                         \
 (1u << (RANG_SMALL_MISCEVE_DYSTANCIJNE - 0))                   \
  | (1u << (RANG_SMALL_1_GRUPA_USTAVOK - 0))                    \
  | (1u << (RANG_SMALL_2_GRUPA_USTAVOK - 0))                    \
  | (1u << (RANG_SMALL_3_GRUPA_USTAVOK - 0))                    \
  | (1u << (RANG_SMALL_4_GRUPA_USTAVOK - 0))                    \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 0 - 0))                \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 1 - 0))                \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 2 - 0))                \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 3 - 0))                \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 4 - 0))                \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 5 - 0))                \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 6 - 0))                \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 7 - 0))                \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 8 - 0))                \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 9 - 0))                \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 10 - 0))               \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 11 - 0))               \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 12 - 0))               \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 13 - 0))               \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 14 - 0))               \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 15 - 0))               \
  | (1u << (RANG_SMALL_BLOCK_IN_MMS1 + 0 - 0))                  \
  | (1u << (RANG_SMALL_BLOCK_IN_MMS1 + 1 - 0))                  \
  | (1u << (RANG_SMALL_BLOCK_IN_MMS1 + 2 - 0))                  \
  | (1u << (RANG_SMALL_BLOCK_IN_MMS1 + 3 - 0))                  \
  | (1u << (RANG_SMALL_BLOCK_OUT_LAN1 + 0 - 0))                 \
  | (1u << (RANG_SMALL_BLOCK_OUT_LAN1 + 1 - 0))                 \
  | (1u << (RANG_SMALL_BLOCK_OUT_LAN1 + 2 - 0))                 \
  | (1u << (RANG_SMALL_BLOCK_OUT_LAN1 + 3 - 0))                 \
)

#define MASKA_BUTTON_MODE_1_SIGNALS_1 (                         \
    (1u << (RANG_SMALL_OSNOVNYJ_TN2_RPN - 32))                  \
  | (1u << (RANG_SMALL_MRZS_OR_LOCAL_MODE_RPN - 32))            \
  | (1u << (RANG_SMALL_TM_MODE_VID_DV_RPN - 32))                \
  | (1u << (RANG_SMALL_AUTO_MODE_FROM_DV_RPN - 32))             \
  | (1u << (RANG_SMALL_PEREKLYUCHENNYA_RPN - 32))               \
  | (1u << (RANG_SMALL_UBAVYTY_FROM_DV_RPN - 32))               \
  | (1u << (RANG_SMALL_PRYBAVYTY_FROM_DV_RPN - 32))             \
  | (1u << (RANG_SMALL_BLOCK_STRUM_KOMP_RPN - 32))              \
  | (1u << (RANG_SMALL_ZOVNISHNJE_BLOCKUVANNJA_RPN - 32))       \
  | (1u << (RANG_SMALL_1_POLOGENNJA_RPN - 32))                  \
  | (1u << (RANG_SMALL_NOMINALNA_POZYCIJA_RPN - 32))            \
  | (1u << (RANG_SMALL_N_POLOGENNJA_RPN - 32))                  \
  | (1u << (RANG_SMALL_CLEAR_BLK_RPN - 32))                     \
  | (1u << (RANG_SMALL_KONTROL_UBAVYTY_ZSKh - 32))              \
  | (1u << (RANG_SMALL_KONTROL_PRYBAVYTY_ZSKh - 32))            \
  | (1u << (RANG_SMALL_BLOCK_BRP - 32))                         \
  | (1u << (RANG_SMALL_BLOCK_UMAX2 - 32))                       \
  | (1u << (RANG_SMALL_BLOCK_UMIN1 - 32))                       \
  | (1u << (RANG_SMALL_BLOCK_UMIN2 - 32))                       \
  | (1u << (RANG_SMALL_BLOCK_UP1 + 0 - 32))                    \
  | (1u << (RANG_SMALL_BLOCK_UP1 + 1 - 32))                     \
  | (1u << (RANG_SMALL_BLOCK_UP1 + 2 - 32))                     \
  | (1u << (RANG_SMALL_BLOCK_UP1 + 3 - 32))                    \
  | (1u << (RANG_SMALL_BLOCK_UP1 + 4 - 32))                    \
  | (1u << (RANG_SMALL_BLOCK_UP1 + 5 - 32))                    \
  | (1u << (RANG_SMALL_BLOCK_UP1 + 6 - 32))                    \
  | (1u << (RANG_SMALL_BLOCK_UP1 + 7 - 32))                    \
  | (1u << (RANG_SMALL_DF1_IN - 32))                           \
  | (1u << (RANG_SMALL_DF1_R - 32))                            \
  | (1u << (RANG_SMALL_DF2_IN - 32))                           \
)

#define MASKA_BUTTON_MODE_1_SIGNALS_2 (                        \
    (1u << (RANG_SMALL_DF2_R - 64))                            \
  | (1u << (RANG_SMALL_DF3_IN - 64))                           \
  | (1u << (RANG_SMALL_DF3_R - 64))                            \
  | (1u << (RANG_SMALL_DF4_IN - 64))                           \
  | (1u << (RANG_SMALL_DF4_R - 64))                            \
  | (1u << (RANG_SMALL_DF5_IN - 64))                           \
  | (1u << (RANG_SMALL_DF5_R - 64))                            \
  | (1u << (RANG_SMALL_DF6_IN - 64))                           \
  | (1u << (RANG_SMALL_DF6_R - 64))                            \
  | (1u << (RANG_SMALL_DF7_IN - 64))                           \
  | (1u << (RANG_SMALL_DF7_R - 64))                            \
  | (1u << (RANG_SMALL_DF8_IN - 64))                           \
  | (1u << (RANG_SMALL_DF8_R - 64))                            \
  | (1u << (RANG_SMALL_DT1_SET - 64))                          \
  | (1u << (RANG_SMALL_DT1_RESET - 64))                        \
  | (1u << (RANG_SMALL_DT2_SET - 64))                          \
  | (1u << (RANG_SMALL_DT2_RESET - 64))                        \
  | (1u << (RANG_SMALL_DT3_SET - 64))                          \
  | (1u << (RANG_SMALL_DT3_RESET - 64))                        \
  | (1u << (RANG_SMALL_DT4_SET - 64))                          \
  | (1u << (RANG_SMALL_DT4_RESET - 64))                        \
  | (1u << (RANG_SMALL_LF1 - 64))                              \
  | (1u << (RANG_SMALL_LF2 - 64))                              \
  | (1u << (RANG_SMALL_LF3 - 64))                              \
  | (1u << (RANG_SMALL_LF4 - 64))                              \
  | (1u << (RANG_SMALL_LF5 - 64))                              \
  | (1u << (RANG_SMALL_LF6 - 64))                              \
  | (1u << (RANG_SMALL_LF7 - 64))                              \
  | (1u << (RANG_SMALL_LF8 - 64))                              \
  | (1u << (RANG_SMALL_LF9 - 64))                              \
  | (1u << (RANG_SMALL_LF10 - 64))                             \
  | (1u << (RANG_SMALL_LF11 - 64))                             \
)

#define MASKA_BUTTON_MODE_1_SIGNALS_3 (                        \
    (1u << (RANG_SMALL_LF12 - 96))                             \
  | (1u << (RANG_SMALL_LF13 - 96))                             \
  | (1u << (RANG_SMALL_LF14 - 96))                             \
  | (1u << (RANG_SMALL_LF15 - 96))                             \
  | (1u << (RANG_SMALL_LF16 - 96))                             \
)
/*****************************************/

/*****************************************/
//Макски сигналів, які можна зранжувати на виходи Вхідного GOOSE блоку
/*****************************************/
#define MASKA_IN_GOOSE_SIGNALS_0 (                            \
    (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 0 - 0))              \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 1 - 0))              \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 2 - 0))              \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 3 - 0))              \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 4 - 0))              \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 5 - 0))              \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 6 - 0))              \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 7 - 0))              \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 8 - 0))              \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 9 - 0))              \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 10 - 0))             \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 11 - 0))             \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 12 - 0))             \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 13 - 0))             \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 14 - 0))             \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 15 - 0))             \
  | (1u << (RANG_SMALL_BLOCK_OUT_LAN1 + 0 - 0))               \
  | (1u << (RANG_SMALL_BLOCK_OUT_LAN1 + 1 - 0))               \
  | (1u << (RANG_SMALL_BLOCK_OUT_LAN1 + 2 - 0))               \
  | (1u << (RANG_SMALL_BLOCK_OUT_LAN1 + 3 - 0))               \
)

#define MASKA_IN_GOOSE_SIGNALS_1 (                            \
    (1u << (RANG_SMALL_OSNOVNYJ_TN2_RPN - 32))                \
  | (1u << (RANG_SMALL_MRZS_OR_LOCAL_MODE_RPN - 32))          \
  | (1u << (RANG_SMALL_TM_MODE_VID_DV_RPN - 32))              \
  | (1u << (RANG_SMALL_AUTO_MODE_FROM_DV_RPN - 32))           \
  | (1u << (RANG_SMALL_PEREKLYUCHENNYA_RPN - 32))             \
  | (1u << (RANG_SMALL_UBAVYTY_FROM_DV_RPN - 32))             \
  | (1u << (RANG_SMALL_PRYBAVYTY_FROM_DV_RPN - 32))           \
  | (1u << (RANG_SMALL_BLOCK_STRUM_KOMP_RPN - 32))            \
  | (1u << (RANG_SMALL_ZOVNISHNJE_BLOCKUVANNJA_RPN - 32))     \
  | (1u << (RANG_SMALL_1_POLOGENNJA_RPN - 32))                \
  | (1u << (RANG_SMALL_NOMINALNA_POZYCIJA_RPN - 32))          \
  | (1u << (RANG_SMALL_N_POLOGENNJA_RPN - 32))                \
  | (1u << (RANG_SMALL_CLEAR_BLK_RPN - 32))                   \
  | (1u << (RANG_SMALL_KONTROL_UBAVYTY_ZSKh - 32))            \
  | (1u << (RANG_SMALL_KONTROL_PRYBAVYTY_ZSKh - 32))          \
  | (1u << (RANG_SMALL_BLOCK_BRP - 32))                       \
  | (1u << (RANG_SMALL_BLOCK_UMAX2 - 32))                     \
  | (1u << (RANG_SMALL_BLOCK_UMIN1 - 32))                     \
  | (1u << (RANG_SMALL_BLOCK_UMIN2 - 32))                     \
  | (1u << (RANG_SMALL_BLOCK_UP1 + 0 - 32))                   \
  | (1u << (RANG_SMALL_BLOCK_UP1 + 1 - 32))                   \
  | (1u << (RANG_SMALL_BLOCK_UP1 + 2 - 32))                   \
  | (1u << (RANG_SMALL_BLOCK_UP1 + 3 - 32))                   \
  | (1u << (RANG_SMALL_BLOCK_UP1 + 4 - 32))                   \
  | (1u << (RANG_SMALL_BLOCK_UP1 + 5 - 32))                   \
  | (1u << (RANG_SMALL_BLOCK_UP1 + 6 - 32))                   \
  | (1u << (RANG_SMALL_BLOCK_UP1 + 7 - 32))                   \
  | (1u << (RANG_SMALL_DF1_IN - 32))                          \
  | (1u << (RANG_SMALL_DF1_R - 32))                           \
  | (1u << (RANG_SMALL_DF2_IN - 32))                          \
)

#define MASKA_IN_GOOSE_SIGNALS_2 (                               \
    (1u << (RANG_SMALL_DF2_R - 64))                           \
  | (1u << (RANG_SMALL_DF3_IN - 64))                          \
  | (1u << (RANG_SMALL_DF3_R - 64))                           \
  | (1u << (RANG_SMALL_DF4_IN - 64))                          \
  | (1u << (RANG_SMALL_DF4_R - 64))                           \
  | (1u << (RANG_SMALL_DF5_IN - 64))                          \
  | (1u << (RANG_SMALL_DF5_R - 64))                           \
  | (1u << (RANG_SMALL_DF6_IN - 64))                          \
  | (1u << (RANG_SMALL_DF6_R - 64))                           \
  | (1u << (RANG_SMALL_DF7_IN - 64))                          \
  | (1u << (RANG_SMALL_DF7_R - 64))                           \
  | (1u << (RANG_SMALL_DF8_IN - 64))                          \
  | (1u << (RANG_SMALL_DF8_R - 64))                           \
  | (1u << (RANG_SMALL_DT1_SET - 64))                         \
  | (1u << (RANG_SMALL_DT1_RESET - 64))                       \
  | (1u << (RANG_SMALL_DT2_SET - 64))                         \
  | (1u << (RANG_SMALL_DT2_RESET - 64))                       \
  | (1u << (RANG_SMALL_DT3_SET - 64))                         \
  | (1u << (RANG_SMALL_DT3_RESET - 64))                       \
  | (1u << (RANG_SMALL_DT4_SET - 64))                         \
  | (1u << (RANG_SMALL_DT4_RESET - 64))                       \
  | (1u << (RANG_SMALL_LF1 - 64))                             \
  | (1u << (RANG_SMALL_LF2 - 64))                             \
  | (1u << (RANG_SMALL_LF3 - 64))                             \
  | (1u << (RANG_SMALL_LF4 - 64))                             \
  | (1u << (RANG_SMALL_LF5 - 64))                             \
  | (1u << (RANG_SMALL_LF6 - 64))                             \
  | (1u << (RANG_SMALL_LF7 - 64))                             \
  | (1u << (RANG_SMALL_LF8 - 64))                             \
  | (1u << (RANG_SMALL_LF9 - 64))                             \
  | (1u << (RANG_SMALL_LF10 - 64))                            \
  | (1u << (RANG_SMALL_LF11 - 64))                            \
)

#define MASKA_IN_GOOSE_SIGNALS_3 (                            \
    (1u << (RANG_SMALL_LF12 - 96))                            \
  | (1u << (RANG_SMALL_LF13 - 96))                            \
  | (1u << (RANG_SMALL_LF14 - 96))                            \
  | (1u << (RANG_SMALL_LF15 - 96))                            \
  | (1u << (RANG_SMALL_LF16 - 96))                            \
)
/*****************************************/

/*****************************************/
//Макски сигналів, які можна зранжувати на виходи Вхідного MMS блоку
/*****************************************/
#define MASKA_IN_MMS_SIGNALS_0 (                                \
  (1u << (RANG_SMALL_RESET_LEDS - 0))                           \
  | (1u << (RANG_SMALL_RESET_RELES - 0))                        \
  | (1u << (RANG_SMALL_RESET_BLOCK_READY_TU_VID_ZAHYSTIV - 0))  \
)

#define MASKA_IN_MMS_SIGNALS_1 (                                \
  (1u << (RANG_SMALL_DF1_IN - 32))                              \
  | (1u << (RANG_SMALL_DF1_R - 32))                             \
  | (1u << (RANG_SMALL_DF2_IN - 32))                            \
)

#define MASKA_IN_MMS_SIGNALS_2 (                                \
    (1u << (RANG_SMALL_DF2_R - 64))                             \
  | (1u << (RANG_SMALL_DF3_IN - 64))                            \
  | (1u << (RANG_SMALL_DF3_R - 64))                             \
  | (1u << (RANG_SMALL_DF4_IN - 64))                            \
  | (1u << (RANG_SMALL_DF4_R - 64))                             \
  | (1u << (RANG_SMALL_DF5_IN - 64))                            \
  | (1u << (RANG_SMALL_DF5_R - 64))                             \
  | (1u << (RANG_SMALL_DF6_IN - 64))                            \
  | (1u << (RANG_SMALL_DF6_R - 64))                             \
  | (1u << (RANG_SMALL_DF7_IN - 64))                            \
  | (1u << (RANG_SMALL_DF7_R - 64))                             \
  | (1u << (RANG_SMALL_DF8_IN - 64))                            \
  | (1u << (RANG_SMALL_DF8_R - 64))                             \
  | (1u << (RANG_SMALL_DT1_SET - 64))                           \
  | (1u << (RANG_SMALL_DT1_RESET - 64))                         \
  | (1u << (RANG_SMALL_DT2_SET - 64))                           \
  | (1u << (RANG_SMALL_DT2_RESET - 64))                         \
  | (1u << (RANG_SMALL_DT3_SET - 64))                           \
  | (1u << (RANG_SMALL_DT3_RESET - 64))                         \
  | (1u << (RANG_SMALL_DT4_SET - 64))                           \
  | (1u << (RANG_SMALL_DT4_RESET - 64))                         \
  | (1u << (RANG_SMALL_LF1 - 64))                               \
  | (1u << (RANG_SMALL_LF2 - 64))                               \
  | (1u << (RANG_SMALL_LF3 - 64))                               \
  | (1u << (RANG_SMALL_LF4 - 64))                               \
  | (1u << (RANG_SMALL_LF5 - 64))                               \
  | (1u << (RANG_SMALL_LF6 - 64))                               \
  | (1u << (RANG_SMALL_LF7 - 64))                               \
  | (1u << (RANG_SMALL_LF8 - 64))                               \
  | (1u << (RANG_SMALL_LF9 - 64))                               \
  | (1u << (RANG_SMALL_LF10 - 64))                              \
  | (1u << (RANG_SMALL_LF11 - 64))                              \
)

#define MASKA_IN_MMS_SIGNALS_3 (                                \
    (1u << (RANG_SMALL_LF12 - 96))                              \
  | (1u << (RANG_SMALL_LF13 - 96))                              \
  | (1u << (RANG_SMALL_LF14 - 96))                              \
  | (1u << (RANG_SMALL_LF15 - 96))                              \
  | (1u << (RANG_SMALL_LF16 - 96))                              \
)
/*****************************************/

/*****************************************/
//Макски сигналів, які можна зранжувати на дискретні входи
/*****************************************/
#define MASKA_INPUT_SIGNALS_0 (                                \
    (1u << (RANG_SMALL_RESET_LEDS - 0))                        \
  | (1u << (RANG_SMALL_RESET_RELES - 0))                       \
  | (1u << (RANG_SMALL_MISCEVE_DYSTANCIJNE - 0))               \
  | (1u << (RANG_SMALL_1_GRUPA_USTAVOK - 0))                   \
  | (1u << (RANG_SMALL_2_GRUPA_USTAVOK - 0))                   \
  | (1u << (RANG_SMALL_3_GRUPA_USTAVOK - 0))                   \
  | (1u << (RANG_SMALL_4_GRUPA_USTAVOK - 0))                   \
  | (1u << (RANG_SMALL_RESET_BLOCK_READY_TU_VID_ZAHYSTIV - 0)) \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 0 - 0))                \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 1 - 0))                \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 2 - 0))                \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 3 - 0))                \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 4 - 0))                \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 5 - 0))                \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 6 - 0))                \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 7 - 0))                \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 8 - 0))                \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 9 - 0))                \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 10 - 0))               \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 11 - 0))               \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 12 - 0))               \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 13 - 0))               \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 14 - 0))               \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 15 - 0))               \
  | (1u << (RANG_SMALL_BLOCK_IN_MMS1 + 0 - 0))                  \
  | (1u << (RANG_SMALL_BLOCK_IN_MMS1 + 1 - 0))                  \
  | (1u << (RANG_SMALL_BLOCK_IN_MMS1 + 2 - 0))                  \
  | (1u << (RANG_SMALL_BLOCK_IN_MMS1 + 3 - 0))                  \
  | (1u << (RANG_SMALL_BLOCK_OUT_LAN1 + 0 - 0))                 \
  | (1u << (RANG_SMALL_BLOCK_OUT_LAN1 + 1 - 0))                 \
  | (1u << (RANG_SMALL_BLOCK_OUT_LAN1 + 2 - 0))                 \
  | (1u << (RANG_SMALL_BLOCK_OUT_LAN1 + 3 - 0))                 \
)

#define MASKA_INPUT_SIGNALS_1 (                                \
    (1u << (RANG_SMALL_OSNOVNYJ_TN2_RPN - 32))                 \
  | (1u << (RANG_SMALL_MRZS_OR_LOCAL_MODE_RPN - 32))           \
  | (1u << (RANG_SMALL_TM_MODE_VID_DV_RPN - 32))               \
  | (1u << (RANG_SMALL_AUTO_MODE_FROM_DV_RPN - 32))            \
  | (1u << (RANG_SMALL_PEREKLYUCHENNYA_RPN - 32))              \
  | (1u << (RANG_SMALL_UBAVYTY_FROM_DV_RPN - 32))              \
  | (1u << (RANG_SMALL_PRYBAVYTY_FROM_DV_RPN - 32))            \
  | (1u << (RANG_SMALL_BLOCK_STRUM_KOMP_RPN - 32))             \
  | (1u << (RANG_SMALL_ZOVNISHNJE_BLOCKUVANNJA_RPN - 32))      \
  | (1u << (RANG_SMALL_1_POLOGENNJA_RPN - 32))                 \
  | (1u << (RANG_SMALL_NOMINALNA_POZYCIJA_RPN - 32))           \
  | (1u << (RANG_SMALL_N_POLOGENNJA_RPN - 32))                 \
  | (1u << (RANG_SMALL_CLEAR_BLK_RPN - 32))                    \
  | (1u << (RANG_SMALL_KONTROL_UBAVYTY_ZSKh - 32))             \
  | (1u << (RANG_SMALL_KONTROL_PRYBAVYTY_ZSKh - 32))           \
  | (1u << (RANG_SMALL_BLOCK_BRP - 32))                        \
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
)

#define MASKA_INPUT_SIGNALS_2 (                                \
    (1u << (RANG_SMALL_DF2_R - 64))                            \
  | (1u << (RANG_SMALL_DF3_IN - 64))                           \
  | (1u << (RANG_SMALL_DF3_R - 64))                            \
  | (1u << (RANG_SMALL_DF4_IN - 64))                           \
  | (1u << (RANG_SMALL_DF4_R - 64))                            \
  | (1u << (RANG_SMALL_DF5_IN - 64))                           \
  | (1u << (RANG_SMALL_DF5_R - 64))                            \
  | (1u << (RANG_SMALL_DF6_IN - 64))                           \
  | (1u << (RANG_SMALL_DF6_R - 64))                            \
  | (1u << (RANG_SMALL_DF7_IN - 64))                           \
  | (1u << (RANG_SMALL_DF7_R - 64))                            \
  | (1u << (RANG_SMALL_DF8_IN - 64))                           \
  | (1u << (RANG_SMALL_DF8_R - 64))                            \
  | (1u << (RANG_SMALL_DT1_SET - 64))                          \
  | (1u << (RANG_SMALL_DT1_RESET - 64))                        \
  | (1u << (RANG_SMALL_DT2_SET - 64))                          \
  | (1u << (RANG_SMALL_DT2_RESET - 64))                        \
  | (1u << (RANG_SMALL_DT3_SET - 64))                          \
  | (1u << (RANG_SMALL_DT3_RESET - 64))                        \
  | (1u << (RANG_SMALL_DT4_SET - 64))                          \
  | (1u << (RANG_SMALL_DT4_RESET - 64))                        \
  | (1u << (RANG_SMALL_LF1 - 64))                              \
  | (1u << (RANG_SMALL_LF2 - 64))                              \
  | (1u << (RANG_SMALL_LF3 - 64))                              \
  | (1u << (RANG_SMALL_LF4 - 64))                              \
  | (1u << (RANG_SMALL_LF5 - 64))                              \
  | (1u << (RANG_SMALL_LF6 - 64))                              \
  | (1u << (RANG_SMALL_LF7 - 64))                              \
  | (1u << (RANG_SMALL_LF8 - 64))                              \
  | (1u << (RANG_SMALL_LF9 - 64))                              \
  | (1u << (RANG_SMALL_LF10 - 64))                             \
  | (1u << (RANG_SMALL_LF11 - 64))                             \
)

#define MASKA_INPUT_SIGNALS_3 (                                \
   (1u << (RANG_SMALL_LF12 - 96))                             \
  | (1u << (RANG_SMALL_LF13 - 96))                             \
  | (1u << (RANG_SMALL_LF14 - 96))                             \
  | (1u << (RANG_SMALL_LF15 - 96))                             \
  | (1u << (RANG_SMALL_LF16 - 96))                             \
)
/*****************************************/

/*****************************************/
//Макски сигналів, які активовуються або з д.входів, або з інтирфейсів чи функціональних кнопок
/*****************************************/

#define MASKA_FOR_INPUT_SIGNALS_0 (                              \
    (1u << (RANG_RESET_LEDS - 0))                                \
  | (1u << (RANG_RESET_RELES - 0))                               \
  | (1u << (RANG_MISCEVE_DYSTANCIJNE - 0))                       \
  | (1u << (RANG_RESET_BLOCK_READY_TU_VID_ZAHYSTIV - 0))         \
  | (1u << (RANG_BLOCK_IN_GOOSE1 +  0 - 0))                      \
  | (1u << (RANG_BLOCK_IN_GOOSE1 +  1 - 0))                      \
  | (1u << (RANG_BLOCK_IN_GOOSE1 +  2 - 0))                      \
  | (1u << (RANG_BLOCK_IN_GOOSE1 +  3 - 0))                      \
  | (1u << (RANG_BLOCK_IN_GOOSE1 +  4 - 0))                      \
  | (1u << (RANG_BLOCK_IN_GOOSE1 +  5 - 0))                      \
  | (1u << (RANG_BLOCK_IN_GOOSE1 +  6 - 0))                      \
  | (1u << (RANG_BLOCK_IN_GOOSE1 +  7 - 0))                      \
  | (1u << (RANG_BLOCK_IN_GOOSE1 +  8 - 0))                      \
  | (1u << (RANG_BLOCK_IN_GOOSE1 +  9 - 0))                      \
 )

#define MASKA_FOR_INPUT_SIGNALS_1 (                              \
    (1u << (RANG_BLOCK_IN_GOOSE1 + 10 - 32))                     \
  | (1u << (RANG_BLOCK_IN_GOOSE1 + 11 - 32))                     \
  | (1u << (RANG_BLOCK_IN_GOOSE1 + 12 - 32))                     \
  | (1u << (RANG_BLOCK_IN_GOOSE1 + 13 - 32))                     \
  | (1u << (RANG_BLOCK_IN_GOOSE1 + 14 - 32))                     \
  | (1u << (RANG_BLOCK_IN_GOOSE1 + 15 - 32))                     \
  | (1u << (RANG_BLOCK_IN_MMS1 + 0 - 32))                        \
  | (1u << (RANG_BLOCK_IN_MMS1 + 1 - 32))                        \
  | (1u << (RANG_BLOCK_IN_MMS1 + 2 - 32))                        \
  | (1u << (RANG_BLOCK_IN_MMS1 + 3 - 32))                        \
  | (1u << (RANG_BLOCK_OUT_LAN1 + 0 - 32))                       \
  | (1u << (RANG_BLOCK_OUT_LAN1 + 1 - 32))                       \
  | (1u << (RANG_BLOCK_OUT_LAN1 + 2 - 32))                       \
  | (1u << (RANG_BLOCK_OUT_LAN1 + 3 - 32))                       \
  | (1u << (RANG_OSNOVNYJ_TN2_RPN - 32))                          \
  | (1u << (RANG_MRZS_OR_LOCAL_MODE_RPN - 32))                   \
  | (1u << (RANG_TM_MODE_VID_DV_RPN - 32))                       \
  | (1u << (RANG_AUTO_MODE_FROM_DV_RPN - 32))                    \
  | (1u << (RANG_PEREKLYUCHENNYA_RPN - 32))                      \
  | (1u << (RANG_UBAVYTY_FROM_DV_RPN - 32))                      \
)

#define MASKA_FOR_INPUT_SIGNALS_2 (                              \
    (1u << (RANG_PRYBAVYTY_FROM_DV_RPN - 64))                    \
  | (1u << (RANG_BLOCK_STRUM_KOMP_RPN - 64))                     \
  | (1u << (RANG_ZOVNISHNJE_BLOCKUVANNJA_RPN - 64))              \
  | (1u << (RANG_1_POLOGENNJA_RPN - 64))                         \
  | (1u << (RANG_NOMINALNA_POZYCIJA_RPN - 64))                   \
  | (1u << (RANG_N_POLOGENNJA_RPN - 64))                         \
  | (1u << (RANG_CLEAR_BLK_RPN - 64))                            \
  | (1u << (RANG_UBAVYTY_FROM_KB_OR_UPPER_LEVEL_RPN - 64))       \
  | (1u << (RANG_PRYBAVYTY_FROM_KB_OR_UPPER_LEVEL_RPN - 64))     \
)

#define MASKA_FOR_INPUT_SIGNALS_3 (                             \
    (1u << (RANG_KONTROL_UBAVYTY_ZSKh - 96))                     \
  | (1u << (RANG_KONTROL_PRYBAVYTY_ZSKh - 96))                   \
  | (1u << (RANG_BLOCK_BRP - 96))                                \
  | (1u << (RANG_BLOCK_UMAX2 - 96))                              \
  | (1u << (RANG_BLOCK_UMIN1 - 96))                             \
  | (1u << (RANG_BLOCK_UMIN2 - 96))                             \
  | (1u << (RANG_BLOCK_UP1 + 3*0 - 96))                         \
  | (1u << (RANG_BLOCK_UP1 + 3*1 - 96))                         \
  | (1u << (RANG_BLOCK_UP1 + 3*2 - 96))                         \
)

#define MASKA_FOR_INPUT_SIGNALS_4 (                            \
    (1u << (RANG_BLOCK_UP1 + 3*3 - 128))                       \
  | (1u << (RANG_BLOCK_UP1 + 3*4 - 128))                       \
  | (1u << (RANG_BLOCK_UP1 + 3*5 - 128))                       \
  | (1u << (RANG_BLOCK_UP1 + 3*6 - 128))                       \
  | (1u << (RANG_BLOCK_UP1 + 3*7 - 128))                       \
  | (1u << (RANG_DF1_IN - 128))                                \
  | (1u << (RANG_DF1_R -  128))                                \
  | (1u << (RANG_DF2_IN -  128))                               \
  | (1u << (RANG_DF2_R -  128))                                \
  | (1u << (RANG_DF3_IN -  128))                               \
  | (1u << (RANG_DF3_R -  128))                                \
  | (1u << (RANG_DF4_IN -  128))                               \
  | (1u << (RANG_DF4_R -  128))                                \
  | (1u << (RANG_DF5_IN -  128))                               \
  | (1u << (RANG_DF5_R -  128))                                \
  | (1u << (RANG_DF6_IN -  128))                               \
)


#define MASKA_FOR_INPUT_SIGNALS_5 (                            \
    (1u << (RANG_DF6_R -  160))                                \
  | (1u << (RANG_DF7_IN -  160))                               \
  | (1u << (RANG_DF7_R -  160))                                \
  | (1u << (RANG_DF8_IN -  160))                               \
  | (1u << (RANG_DF8_R -  160))                                \
  | (1u << (RANG_DT1_SET -  160))                              \
  | (1u << (RANG_DT1_RESET - 160))                             \
  | (1u << (RANG_DT2_SET - 160))                               \
  | (1u << (RANG_DT2_RESET - 160))                             \
  | (1u << (RANG_DT3_SET - 160))                               \
  | (1u << (RANG_DT3_RESET - 160))                             \
  | (1u << (RANG_DT4_SET - 160))                               \
  | (1u << (RANG_DT4_RESET - 160))                             \
)

#define MASKA_FOR_INPUT_SIGNALS_6 (                            \
    (1u << (RANG_LF1 -  192))                                  \
  | (1u << (RANG_LF2 -  192))                                  \
  | (1u << (RANG_LF3 -  192))                                  \
  | (1u << (RANG_LF4 -  192))                                  \
)

#define MASKA_FOR_INPUT_SIGNALS_7 (                            \
    (1u << (RANG_LF5 -  224))                                  \
  | (1u << (RANG_LF6 -  224))                                  \
  | (1u << (RANG_LF7 -  224))                                  \
  | (1u << (RANG_LF8 -  224))                                  \
  | (1u << (RANG_LF9 -  224))                                  \
  | (1u << (RANG_LF10 -  224))                                 \
  | (1u << (RANG_LF11 -  224))                                 \
  | (1u << (RANG_LF12 -  224))                                 \
  | (1u << (RANG_LF13 -  224))                                 \
  | (1u << (RANG_LF14 -  224))                                 \
  | (1u << (RANG_LF15 -  224))                                 \
  | (1u << (RANG_LF16 - 224))                                  \
)
/*****************************************/

/*****************************************/
//Макски всіх сигналів РПН
/*****************************************/
#define MASKA_RPN_SIGNALS_0    0

#define MASKA_RPN_SIGNALS_1 (                                   \
    (1u << (RANG_OSNOVNYJ_TN2_RPN - 32))                        \
  | (1u << (RANG_MRZS_OR_LOCAL_MODE_RPN - 32))                  \
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
)

#define MASKA_RPN_SIGNALS_2 (                                   \
    (1u << (RANG_PRYBAVYTY_FROM_UPPER_LEVEL_RPN - 64))          \
  | (1u << (RANG_PRYBAVYTY_FROM_KB_RPN - 64))                   \
  | (1u << (RANG_PRYBAVYTY_FROM_DV_RPN - 64))                   \
  | (1u << (RANG_PRYBAVYTY_R_RPN - 64))                         \
  | (1u << (RANG_PRYBAVYTY_T_RPN - 64))                         \
  | (1u << (RANG_PRYBAVYTY_DI_RPN - 64))                        \
  | (1u << (RANG_PRYBAVYTY_1_RPN - 64))                         \
  | (1u << (RANG_PRYBAVYTY_RPN - 64))                           \
  | (1u << (RANG_BLOCK_STRUM_KOMP_RPN - 64))                    \
  | (1u << (RANG_ZOVNISHNJE_BLOCKUVANNJA_RPN - 64))             \
  | (1u << (RANG_1_POLOGENNJA_RPN - 64))                        \
  | (1u << (RANG_NOMINALNA_POZYCIJA_RPN - 64))                  \
  | (1u << (RANG_N_POLOGENNJA_RPN - 64))                        \
  | (1u << (RANG_CLEAR_BLK_RPN - 64))                           \
  | (1u << (RANG_UBAVYTY_FROM_KB_OR_UPPER_LEVEL_RPN - 64))      \
  | (1u << (RANG_PRYBAVYTY_FROM_KB_OR_UPPER_LEVEL_RPN - 64))    \
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
)

#define MASKA_RPN_SIGNALS_3 (                                   \
    (1u << (RANG_ZASTRJAVANNJA_RPN - 96))                       \
  | (1u << (RANG_PRYVID_NE_PISHOV_RPN - 96))                    \
  | (1u << (RANG_N_PER_RIVNE_N_PER_USTAVKY_RPN - 96))           \
  | (1u << (RANG_NESPRAVNIST_RPN - 96))                         \
)

#define MASKA_RPN_SIGNALS_4    0
#define MASKA_RPN_SIGNALS_5    0
#define MASKA_RPN_SIGNALS_6    0
#define MASKA_RPN_SIGNALS_7    0
/*****************************************/

/*****************************************/
//Макски всіх сигналів ЗСХ
/*****************************************/
#define MASKA_ZSKh_SIGNALS_0    0
#define MASKA_ZSKh_SIGNALS_1    0
#define MASKA_ZSKh_SIGNALS_2    0

#define MASKA_ZSKh_SIGNALS_3 (                                   \
    (1u << (RANG_KONTROL_UBAVYTY_ZSKh - 96))                     \
  | (1u << (RANG_KONTROL_PRYBAVYTY_ZSKh - 96))                   \
  | (1u << (RANG_ZSKh - 96))                                     \
)

#define MASKA_ZSKh_SIGNALS_4    0
#define MASKA_ZSKh_SIGNALS_5    0
#define MASKA_ZSKh_SIGNALS_6    0
#define MASKA_ZSKh_SIGNALS_7    0
/*****************************************/

/*****************************************/
//Макски всіх сигналів ЗСН
/*****************************************/
#define MASKA_ZNKh_SIGNALS_0    0
#define MASKA_ZNKh_SIGNALS_1    0
#define MASKA_ZNKh_SIGNALS_2    0

#define MASKA_ZNKh_SIGNALS_3 (                                   \
    (1u << (RANG_ZNKh_IR - 96))                                  \
  | (1u << (RANG_ZNKh_BR - 96))                                  \
  | (1u << (RANG_ZNKh - 96))                                     \
)

#define MASKA_ZNKh_SIGNALS_4    0
#define MASKA_ZNKh_SIGNALS_5    0
#define MASKA_ZNKh_SIGNALS_6    0
#define MASKA_ZNKh_SIGNALS_7    0
/*****************************************/

/*****************************************/
//Макски всіх сигналів БРП
/*****************************************/
#define MASKA_BRP_SIGNALS_0    0
#define MASKA_BRP_SIGNALS_1    0
#define MASKA_BRP_SIGNALS_2    0

#define MASKA_BRP_SIGNALS_3 (                                    \
    (1u << (RANG_BLOCK_BRP - 96))                                \
  | (1u << (RANG_BRP - 96))                                      \
)

#define MASKA_BRP_SIGNALS_4    0
#define MASKA_BRP_SIGNALS_5    0
#define MASKA_BRP_SIGNALS_6    0
#define MASKA_BRP_SIGNALS_7    0
/*****************************************/

/*****************************************/
//Макски всіх сигналів UMAX
/*****************************************/
#define MASKA_UMAX_SIGNALS_0    0
#define MASKA_UMAX_SIGNALS_1    0
#define MASKA_UMAX_SIGNALS_2    0

#define MASKA_UMAX_SIGNALS_3 (                                   \
    (1u << (RANG_BLOCK_UMAX2 - 96))                              \
  | (1u << (RANG_PO_UMAX1 - 96))                                 \
  | (1u << (RANG_PRYSK_UBAVYTY_VID_UMAX1 - 96))                  \
  | (1u << (RANG_BLOCK_PRYBAVYTY_VID_UMAX1 - 96))                \
  | (1u << (RANG_PO_UMAX2 - 96))                                 \
  | (1u << (RANG_UMAX2 - 96))                                    \
)

#define MASKA_UMAX_SIGNALS_4    0
#define MASKA_UMAX_SIGNALS_5    0
#define MASKA_UMAX_SIGNALS_6    0
#define MASKA_UMAX_SIGNALS_7    0
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
#define MASKA_UMIN_SIGNALS_7    0
/*****************************************/

/*****************************************/
//Макски всіх сигналів Універсального Захисту
/*****************************************/
#define MASKA_UP_SIGNALS_0 0
#define MASKA_UP_SIGNALS_1 0
#define MASKA_UP_SIGNALS_2 0

#define MASKA_UP_SIGNALS_3 (                                      \
  (1u << (RANG_BLOCK_UP1 + 3 * 0 - 96))                           \
  | (1u << (RANG_PO_UP1 + 3 * 0 - 96))                            \
  | (1u << (RANG_UP1 + 3 * 0 - 96))                               \
  | (1u << (RANG_BLOCK_UP1 + 3 * 1 - 96))                         \
  | (1u << (RANG_PO_UP1 + 3 * 1 - 96))                            \
  | (1u << (RANG_UP1 + 3 * 1 - 96))                               \
  | (1u << (RANG_BLOCK_UP1 + 3 * 2 - 96))                         \
  | (1u << (RANG_PO_UP1 + 3 * 2 - 96))                            \
)

#define MASKA_UP_SIGNALS_4 (                                       \
    (1u << (RANG_UP1 + 3 * 2 - 128))                               \
  | (1u << (RANG_BLOCK_UP1 + 3 * 3 - 128))                         \
  | (1u << (RANG_PO_UP1 + 3 * 3 - 128))                            \
  | (1u << (RANG_UP1 + 3 * 3 - 128))                               \
  | (1u << (RANG_BLOCK_UP1 + 3 * 4 - 128))                         \
  | (1u << (RANG_PO_UP1 + 3 * 4 - 128))                            \
  | (1u << (RANG_UP1 + 3 * 4 - 128))                               \
  | (1u << (RANG_BLOCK_UP1 + 3 * 5 - 128))                         \
  | (1u << (RANG_PO_UP1 + 3 * 5 - 128))                            \
  | (1u << (RANG_UP1 + 3 * 5 - 128))                               \
  | (1u << (RANG_BLOCK_UP1 + 3 * 6 - 128))                         \
  | (1u << (RANG_PO_UP1 + 3 * 6 - 128))                            \
  | (1u << (RANG_UP1 + 3 * 6 - 128))                               \
  | (1u << (RANG_BLOCK_UP1 + 3 * 7 - 128))                         \
  | (1u << (RANG_PO_UP1 + 3 * 7 - 128))                            \
  | (1u << (RANG_UP1 + 3 * 7 - 128))                               \
)

#define MASKA_UP_SIGNALS_5 0
#define MASKA_UP_SIGNALS_6 0
#define MASKA_UP_SIGNALS_7 0
/*****************************************/

/*****************************************/
//Макски всіх сигналів розширеної логіки
/*****************************************/
#define MASKA_EL_SIGNALS_0    0
#define MASKA_EL_SIGNALS_1    0
#define MASKA_EL_SIGNALS_2    0
#define MASKA_EL_SIGNALS_3    0

#define MASKA_EL_SIGNALS_4 (                                   \
    (1u << (RANG_DF1_IN - 128))                                \
  | (1u << (RANG_DF1_R - 128))                                 \
  | (1u << (RANG_DF1_OUT - 128))                               \
  | (1u << (RANG_DF2_IN - 128))                                \
  | (1u << (RANG_DF2_R - 128))                                 \
  | (1u << (RANG_DF2_OUT - 128))                               \
  | (1u << (RANG_DF3_IN - 128))                                \
  | (1u << (RANG_DF3_R - 128))                                 \
  | (1u << (RANG_DF3_OUT - 128))                               \
  | (1u << (RANG_DF4_IN - 128))                                \
  | (1u << (RANG_DF4_R - 128))                                 \
  | (1u << (RANG_DF4_OUT - 128))                               \
  | (1u << (RANG_DF5_IN - 128))                                \
  | (1u << (RANG_DF5_R - 128))                                 \
  | (1u << (RANG_DF5_OUT - 128))                               \
  | (1u << (RANG_DF6_IN - 128))                                \
)

#define MASKA_EL_SIGNALS_5 (                                   \
    (1u << (RANG_DF6_R - 160))                                 \
  | (1u << (RANG_DF6_OUT - 160))                               \
  | (1u << (RANG_DF7_IN - 160))                                \
  | (1u << (RANG_DF7_R - 160))                                 \
  | (1u << (RANG_DF7_OUT - 160))                               \
  | (1u << (RANG_DF8_IN - 160))                                \
  | (1u << (RANG_DF8_R - 160))                                 \
  | (1u << (RANG_DF8_OUT - 160))                               \
  | (1u << (RANG_DT1_SET - 160))                               \
  | (1u << (RANG_DT1_RESET - 160))                             \
  | (1u << (RANG_DT1_OUT - 160))                               \
  | (1u << (RANG_DT2_SET - 160))                               \
  | (1u << (RANG_DT2_RESET - 160))                             \
  | (1u << (RANG_DT2_OUT - 160))                               \
  | (1u << (RANG_DT3_SET - 160))                               \
  | (1u << (RANG_DT3_RESET - 160))                             \
  | (1u << (RANG_DT3_OUT - 160))                               \
  | (1u << (RANG_DT4_SET - 160))                               \
  | (1u << (RANG_DT4_RESET - 160))                             \
  | (1u << (RANG_DT4_OUT - 160))                               \
  | (1u << (RANG_D_AND1 - 160))                                \
  | (1u << (RANG_D_AND2 - 160))                                \
  | (1u << (RANG_D_AND3 - 160))                                \
  | (1u << (RANG_D_AND4 - 160))                                \
  | (1u << (RANG_D_AND5 - 160))                                \
  | (1u << (RANG_D_AND6 - 160))                                \
  | (1u << (RANG_D_AND7 - 160))                                \
  | (1u << (RANG_D_AND8 - 160))                                \
  | (1u << (RANG_D_OR1 - 160))                                 \
  | (1u << (RANG_D_OR2 - 160))                                 \
  | (1u << (RANG_D_OR3 - 160))                                 \
  | (1u << (RANG_D_OR4 - 160))                                 \
)

#define MASKA_EL_SIGNALS_6 (                                   \
    (1u << (RANG_D_OR5 - 192))                                 \
  | (1u << (RANG_D_OR6 - 192))                                 \
  | (1u << (RANG_D_OR7 - 192))                                 \
  | (1u << (RANG_D_OR8 - 192))                                 \
  | (1u << (RANG_D_XOR1 - 192))                                \
  | (1u << (RANG_D_XOR2 - 192))                                \
  | (1u << (RANG_D_XOR3 - 192))                                \
  | (1u << (RANG_D_XOR4 - 192))                                 \
  | (1u << (RANG_D_XOR5 - 192))                                 \
  | (1u << (RANG_D_XOR6 - 192))                                 \
  | (1u << (RANG_D_XOR7 - 192))                                 \
  | (1u << (RANG_D_XOR8 - 192))                                 \
  | (1u << (RANG_D_NOT1 - 192))                                 \
  | (1u << (RANG_D_NOT2 - 192))                                 \
  | (1u << (RANG_D_NOT3 - 192))                                 \
  | (1u << (RANG_D_NOT4 - 192))                                 \
  | (1u << (RANG_D_NOT5 - 192))                                 \
  | (1u << (RANG_D_NOT6 - 192))                                 \
  | (1u << (RANG_D_NOT7 - 192))                                 \
  | (1u << (RANG_D_NOT8 - 192))                                 \
  | (1u << (RANG_D_NOT9 - 192))                                 \
  | (1u << (RANG_D_NOT10 - 192))                                \
  | (1u << (RANG_D_NOT11 - 192))                                \
  | (1u << (RANG_D_NOT12 - 192))                                \
  | (1u << (RANG_D_NOT13 - 192))                                \
  | (1u << (RANG_D_NOT14 - 192))                                \
  | (1u << (RANG_D_NOT15 - 192))                               \
  | (1u << (RANG_D_NOT16 - 192))                               \
  | (1u << (RANG_LF1 - 192))                                   \
  | (1u << (RANG_LF2 - 192))                                   \
  | (1u << (RANG_LF3 - 192))                                   \
  | (1u << (RANG_LF4 - 192))                                   \
)

#define MASKA_EL_SIGNALS_7 (                                   \
    (1u << (RANG_LF5 - 224))                                   \
  | (1u << (RANG_LF6 - 224))                                   \
  | (1u << (RANG_LF7 - 224))                                   \
  | (1u << (RANG_LF8 - 224))                                   \
  | (1u << (RANG_LF9 - 224))                                   \
  | (1u << (RANG_LF10 - 224))                                  \
  | (1u << (RANG_LF11 - 224))                                  \
  | (1u << (RANG_LF12 - 224))                                  \
  | (1u << (RANG_LF13 - 224))                                  \
  | (1u << (RANG_LF14 - 224))                                  \
  | (1u << (RANG_LF15 - 224))                                  \
  | (1u << (RANG_LF16 - 224))                                  \
  | (1u << (RANG_ERROR_CONF_EL - 224))                         \
)
/*****************************************/

/*****************************************/
//Макска сигналів, які мають записуватися у енергонезалежну пам'ять
/*****************************************/
#define MASKA_TRIGGER_SIGNALES_0 0
#define MASKA_TRIGGER_SIGNALES_1 0
#define MASKA_TRIGGER_SIGNALES_2 0
#define MASKA_TRIGGER_SIGNALES_3 0
#define MASKA_TRIGGER_SIGNALES_4 0

#define MASKA_TRIGGER_SIGNALES_5 (                                \
  (1u << (RANG_DT1_OUT - 160))                                    \
  | (1u << (RANG_DT2_OUT - 160))                                  \
  | (1u << (RANG_DT3_OUT - 160))                                  \
  | (1u << (RANG_DT4_OUT - 160))                                  \
)

#define MASKA_TRIGGER_SIGNALES_6 0
#define MASKA_TRIGGER_SIGNALES_7 0
/*****************************************/

/*****************************************/
//Макска для блокування зміни груп  уставок
/*****************************************/
#define MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_0  0
#define MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_1  0

#define MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_2 (              \
     (1u << (RANG_VYJCHE_ZONY_RPN - 64))                      \
   | (1u << (RANG_NYJCHE_ZONY_RPN - 64))                      \
)

#define MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_3 (              \
     (1u << (RANG_PO_UMAX1 - 96))                             \
   | (1u << (RANG_PRYSK_UBAVYTY_VID_UMAX1 - 96))              \
   | (1u << (RANG_BLOCK_PRYBAVYTY_VID_UMAX1 - 96))            \
   | (1u << (RANG_PO_UMAX2 - 96))                             \
   | (1u << (RANG_UMAX2 - 96))                                \
   | (1u << (RANG_BRP - 96))                                  \
   | (1u << (RANG_PO_UMIN1 - 96))                             \
   | (1u << (RANG_UMIN1 - 96))                                \
   | (1u << (RANG_PO_UMIN2 - 96))                             \
   | (1u << (RANG_UMIN2 - 96))                                \
   | (1u << (RANG_PO_UP1 + 3*0 - 96))                         \
   | (1u << (RANG_UP1 + 3*0 - 96))                            \
   | (1u << (RANG_PO_UP1 + 3*1 - 96))                         \
   | (1u << (RANG_UP1 + 3*1 - 96))                            \
   | (1u << (RANG_PO_UP1 + 3*2 - 96))                         \
)

#define MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_4 (              \
     (1u << (RANG_UP1 + 3*2 - 128))                           \
   | (1u << (RANG_PO_UP1 + 3*3 - 128))                        \
   | (1u << (RANG_UP1 + 3*3 - 128))                           \
   | (1u << (RANG_PO_UP1 + 3*4 - 128))                        \
   | (1u << (RANG_UP1 + 3*4 - 128))                           \
   | (1u << (RANG_PO_UP1 + 3*5 - 128))                        \
   | (1u << (RANG_UP1 + 3*5 - 128))                           \
   | (1u << (RANG_PO_UP1 + 3*6 - 128))                        \
   | (1u << (RANG_UP1 + 3*6 - 128))                           \
   | (1u << (RANG_PO_UP1 + 3*7 - 128))                        \
   | (1u << (RANG_UP1 + 3*7 - 128))                           \
)
#define MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_5  0
#define MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_6  0
#define MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_7  0
/*****************************************/

/*****************************************/
//Макска інформативних сигналів
/*****************************************/

#define MASKA_INFO_SIGNALES_0 (                                   \
    (1u << (RANG_DEFECT - 0))                                     \
  | (1u << (RANG_AVAR_DEFECT - 0))                                \
  | (1u << (RANG_WORK_A_REJESTRATOR - 0))                         \
  | (1u << (RANG_WORK_D_REJESTRATOR - 0))                         \
  | (1u << (RANG_SETTINGS_CHANGED - 0))                           \
)

#define MASKA_INFO_SIGNALES_1 0
#define MASKA_INFO_SIGNALES_2 0
#define MASKA_INFO_SIGNALES_3 0
#define MASKA_INFO_SIGNALES_4 0
#define MASKA_INFO_SIGNALES_5 0
#define MASKA_INFO_SIGNALES_6 0
#define MASKA_INFO_SIGNALES_7 0
/*****************************************/

/*****************************************/
//Макска моніторингу максимальної фазної напруги основного каналу
/*****************************************/
#define MASKA_MONITOTYNG_MAX_U_BASE_SIGNALES_0 0
#define MASKA_MONITOTYNG_MAX_U_BASE_SIGNALES_1 0

#define MASKA_MONITOTYNG_MAX_U_BASE_SIGNALES_2  (               \
       (1u << (RANG_VYJCHE_ZONY_RPN - 64))                      \
     | (1u << (RANG_PO3_U_OSN_RPN - 64))                        \
)     

#define MASKA_MONITOTYNG_MAX_U_BASE_SIGNALES_3  (               \
       (1u << (RANG_PO_UMAX1 - 96))                             \
     | (1u << (RANG_PRYSK_UBAVYTY_VID_UMAX1 - 96))              \
     | (1u << (RANG_BLOCK_PRYBAVYTY_VID_UMAX1 - 96))            \
     | (1u << (RANG_PO_UMAX2 - 96))                             \
     | (1u << (RANG_UMAX2 - 96))                                \
)     

#define MASKA_MONITOTYNG_MAX_U_BASE_SIGNALES_4 0
#define MASKA_MONITOTYNG_MAX_U_BASE_SIGNALES_5 0
#define MASKA_MONITOTYNG_MAX_U_BASE_SIGNALES_6 0
#define MASKA_MONITOTYNG_MAX_U_BASE_SIGNALES_7 0
/*****************************************/

/*****************************************/
//Макска моніторингу максимального фазного струму сторони 0.4кВ для дискретного реєстратора
/*****************************************/
/*****************************************/
//Макска моніторингу максимальної фазної напруги допоміжного каналу
/*****************************************/
#define MASKA_MONITOTYNG_MAX_U_SECOND_SIGNALES_0 0
#define MASKA_MONITOTYNG_MAX_U_SECOND_SIGNALES_1 0

#define MASKA_MONITOTYNG_MAX_U_SECOND_SIGNALES_2 (              \
       (1u << (RANG_PO1_U_VSPOM_RPN - 64))                      \
     | (1u << (RANG_PO3_U_VSPOM_RPN - 64))                      \
)

#define MASKA_MONITOTYNG_MAX_U_SECOND_SIGNALES_3 0
#define MASKA_MONITOTYNG_MAX_U_SECOND_SIGNALES_4 0
#define MASKA_MONITOTYNG_MAX_U_SECOND_SIGNALES_5 0
#define MASKA_MONITOTYNG_MAX_U_SECOND_SIGNALES_6 0
#define MASKA_MONITOTYNG_MAX_U_SECOND_SIGNALES_7 0
/*****************************************/
/*****************************************/

/*****************************************/
//Макска моніторингу відношення другої гармоніки до першої гармоніки фазного струму
/*****************************************/
#define MASKA_MONITOTYNG_MIN_U_BASE_SIGNALES_0  0
#define MASKA_MONITOTYNG_MIN_U_BASE_SIGNALES_1  0

#define MASKA_MONITOTYNG_MIN_U_BASE_SIGNALES_2 (              \
       (1u << (RANG_NYJCHE_ZONY_RPN - 64))                    \
     | (1u << (RANG_PO4_U_OSN_RPN - 64))                      \
)     

#define MASKA_MONITOTYNG_MIN_U_BASE_SIGNALES_3 (              \
       (1u << (RANG_PO_UMIN1 - 96))                           \
     | (1u << (RANG_UMIN1 - 96))                              \
     | (1u << (RANG_PO_UMIN2 - 96))                           \
     | (1u << (RANG_UMIN2 - 96))                              \
)     
#define MASKA_MONITOTYNG_MIN_U_BASE_SIGNALES_4  0
#define MASKA_MONITOTYNG_MIN_U_BASE_SIGNALES_5  0
#define MASKA_MONITOTYNG_MIN_U_BASE_SIGNALES_6  0
#define MASKA_MONITOTYNG_MIN_U_BASE_SIGNALES_7  0
/*****************************************/

/*****************************************/
//Макска моніторингу максимальноого струму основного каналу
/*****************************************/
#define MASKA_MONITOTYNG_MAX_I_BASE_SIGNALES_0  0
#define MASKA_MONITOTYNG_MAX_I_BASE_SIGNALES_1  0
#define MASKA_MONITOTYNG_MAX_I_BASE_SIGNALES_2  0

#define MASKA_MONITOTYNG_MAX_I_BASE_SIGNALES_3 (              \
       (1u << (RANG_BRP - 96))                                \
) 

#define MASKA_MONITOTYNG_MAX_I_BASE_SIGNALES_4  0
#define MASKA_MONITOTYNG_MAX_I_BASE_SIGNALES_5  0
#define MASKA_MONITOTYNG_MAX_I_BASE_SIGNALES_6  0
#define MASKA_MONITOTYNG_MAX_I_BASE_SIGNALES_7  0
/*****************************************/
// clang-format on

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
#define VAGA_MAX_FUNCTIONS_IN_IN_GOOSE 3
#define MAX_FUNCTIONS_IN_IN_GOOSE (1 << VAGA_MAX_FUNCTIONS_IN_IN_GOOSE)
#define VAGA_MAX_FUNCTIONS_IN_IN_MMS 3
#define MAX_FUNCTIONS_IN_IN_MMS (1 << VAGA_MAX_FUNCTIONS_IN_IN_MMS)
#define VAGA_MAX_FUNCTIONS_IN_OUT_LAN 3
#define MAX_FUNCTIONS_IN_OUT_LAN (1 << VAGA_MAX_FUNCTIONS_IN_OUT_LAN)

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
#define MODYFIKACIA_VERSII_PZ 11
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
