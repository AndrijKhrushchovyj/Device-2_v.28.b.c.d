#ifndef __CONST_GLOBAL__
#define __CONST_GLOBAL__

#define MAX_NUMBER_LINES_VMP            8

#define NUMBER_ANALOG_CANALES           8

#define NUMBER_INPUTS                   8
#define NUMBER_SIMPLE_OUTPUTS           9
#define NUMBER_DS                       1
#define NUMBER_OUTPUTS                  (NUMBER_SIMPLE_OUTPUTS + NUMBER_DS)

#define NUMBER_LEDS                     17

#define NUMBER_UP                       8

#define N_IN_GOOSE                      16
#define N_IN_MMS                        4
#define N_OUT_LAN                       4

#define N_IN_GOOSE_MMS_OUT              8
#define N_OUT_LAN_IN                    8

//#define NUMBER_DEFINED_ELEMENTS         6
#define NUMBER_DEFINED_FUNCTIONS        8
#define NUMBER_DEFINED_TRIGGERS         4
#define NUMBER_DEFINED_AND              8
#define NUMBER_DEFINED_OR               8
#define NUMBER_DEFINED_XOR              8
#define NUMBER_DEFINED_NOT              16
#define NUMBER_TRANSFER_FUNCTIONS       16
#define NUMBER_LINK_FUNCTIONS           16

#define NUMBER_DEFINED_BUTTONS          6

#define NUMBER_IN_AND                   8
#define NUMBER_IN_OR                    8

#define NUMBER_GROUP_USTAVOK            4

#define N_INPUT_BOARDS          1
#define N_OUTPUT_BOARDS         2

enum _configuration 
{
MTZ_BIT_CONFIGURATION = 0,
MTZ04_BIT_CONFIGURATION,
ZNAM_BIT_CONFIGURATION,
ZDZ_BIT_CONFIGURATION,
ZZ_BIT_CONFIGURATION,
TZNP_BIT_CONFIGURATION,
APV_BIT_CONFIGURATION,
ACHR_CHAPV_BIT_CONFIGURATION,
UROV_BIT_CONFIGURATION,
ZOP_BIT_CONFIGURATION,
UMIN_BIT_CONFIGURATION,
UMAX_BIT_CONFIGURATION,
UP_BIT_CONFIGURATION,
VMP_BIT_CONFIGURATION,
EL_BIT_CONFIGURATION,
  _FIX_NUMBER_PROTECTION,
DS_BIT_CONFIGURATION = _FIX_NUMBER_PROTECTION,

TOTAL_NUMBER_PROTECTION
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

RANG_SMALL_BLOCK_IN_GOOSE1,

RANG_SMALL_BLOCK_IN_MMS1 = (RANG_SMALL_BLOCK_IN_GOOSE1 + N_IN_GOOSE),

RANG_SMALL_BLOCK_OUT_LAN1 = (RANG_SMALL_BLOCK_IN_MMS1 + N_IN_MMS),

  _RANG_SMALL_MSZ_BEGIN = (RANG_SMALL_BLOCK_OUT_LAN1 + N_OUT_LAN),
RANG_SMALL_BLOCK_MTZ1 = _RANG_SMALL_MSZ_BEGIN,
RANG_SMALL_BLOCK_MTZ2,
RANG_SMALL_BLOCK_USK_MTZ2,
RANG_SMALL_BLOCK_MTZ3,
RANG_SMALL_BLOCK_MTZ4,

  _RANG_SMALL_MSZ04_BEGIN,
RANG_SMALL_BLOCK_MTZ04_1 = _RANG_SMALL_MSZ04_BEGIN,
RANG_SMALL_BLOCK_MTZ04_2,
RANG_SMALL_BLOCK_USK_MTZ04_2,

  _RANG_SMALL_ZNAM_BEGIN,
RANG_SMALL_BLOCK_ZNAM = _RANG_SMALL_ZNAM_BEGIN,

  _RANG_SMALL_ZDZ_BEGIN,
RANG_SMALL_BLOCK_ZDZ = _RANG_SMALL_ZDZ_BEGIN,
RANG_SMALL_PUSK_ZDZ_VID_DV,

  _RANG_SMALL_SZZ_BEGIN,
RANG_SMALL_BLOCK_NZZ = _RANG_SMALL_SZZ_BEGIN,

  _RANG_SMALL_SZNP_BEGIN,
RANG_SMALL_BLOCK_TZNP1 = _RANG_SMALL_SZNP_BEGIN,
RANG_SMALL_BLOCK_TZNP2,
RANG_SMALL_BLOCK_TZNP3,

  _RANG_SMALL_APV_BEGIN,
RANG_SMALL_STAT_BLK_APV = _RANG_SMALL_APV_BEGIN,

  _RANG_SMALL_ACHR_CHAPV_BEGIN,
RANG_SMALL_ACHR_CHAPV_VID_DV = _RANG_SMALL_ACHR_CHAPV_BEGIN,
RANG_SMALL_BLOCK_ACHR1,
RANG_SMALL_BLOCK_ACHR2,

  _RANG_SMALL_PRVV_BEGIN,
RANG_SMALL_BLOCK_UROV = _RANG_SMALL_PRVV_BEGIN,
RANG_SMALL_PUSK_UROV_VID_DV,

  _RANG_SMALL_ZZP_BEGIN,
RANG_SMALL_BLOCK_ZOP = _RANG_SMALL_ZZP_BEGIN,

  _RANG_SMALL_UMIN_BEGIN,
RANG_SMALL_BLOCK_UMIN1 = _RANG_SMALL_UMIN_BEGIN,
RANG_SMALL_START_UMIN1,
RANG_SMALL_BLOCK_UMIN2,
RANG_SMALL_START_UMIN2,

  _RANG_SMALL_UMAX_BEGIN,
RANG_SMALL_BLOCK_UMAX1 = _RANG_SMALL_UMAX_BEGIN,
RANG_SMALL_BLOCK_UMAX2,

  _RANG_SMALL_UP_BEGIN,
RANG_SMALL_BLOCK_UP1 = _RANG_SMALL_UP_BEGIN,

  _RANG_SMALL_VMP_BEGIN = (RANG_SMALL_BLOCK_UP1 + NUMBER_UP),
  
  _RANG_SMALL_EL_BEGIN = _RANG_SMALL_VMP_BEGIN,
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

#define NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL     (_RANG_SMALL_MSZ_BEGIN              - _RANG_SMALL_GENERAL_BEGIN   )
#define NUMBER_MTZ_SIGNAL_FOR_RANG_SMALL         (_RANG_SMALL_MSZ04_BEGIN            - _RANG_SMALL_MSZ_BEGIN       )
#define NUMBER_MTZ04_SIGNAL_FOR_RANG_SMALL       (_RANG_SMALL_ZNAM_BEGIN             - _RANG_SMALL_MSZ04_BEGIN     )
#define NUMBER_ZNAM_SIGNAL_FOR_RANG_SMALL        (_RANG_SMALL_ZDZ_BEGIN              - _RANG_SMALL_ZNAM_BEGIN      )
#define NUMBER_ZDZ_SIGNAL_FOR_RANG_SMALL         (_RANG_SMALL_SZZ_BEGIN              - _RANG_SMALL_ZDZ_BEGIN       )
#define NUMBER_ZZ_SIGNAL_FOR_RANG_SMALL          (_RANG_SMALL_SZNP_BEGIN             - _RANG_SMALL_SZZ_BEGIN       )
#define NUMBER_TZNP_SIGNAL_FOR_RANG_SMALL        (_RANG_SMALL_APV_BEGIN              - _RANG_SMALL_SZNP_BEGIN      )
#define NUMBER_APV_SIGNAL_FOR_RANG_SMALL         (_RANG_SMALL_ACHR_CHAPV_BEGIN       - _RANG_SMALL_APV_BEGIN       )
#define NUMBER_ACHR_CHAPV_SIGNAL_FOR_RANG_SMALL  (_RANG_SMALL_PRVV_BEGIN             - _RANG_SMALL_ACHR_CHAPV_BEGIN)
#define NUMBER_UROV_SIGNAL_FOR_RANG_SMALL        (_RANG_SMALL_ZZP_BEGIN              - _RANG_SMALL_PRVV_BEGIN      )
#define NUMBER_ZOP_SIGNAL_FOR_RANG_SMALL         (_RANG_SMALL_UMIN_BEGIN             - _RANG_SMALL_ZZP_BEGIN       )
#define NUMBER_UMIN_SIGNAL_FOR_RANG_SMALL        (_RANG_SMALL_UMAX_BEGIN             - _RANG_SMALL_UMIN_BEGIN      )
#define NUMBER_UMAX_SIGNAL_FOR_RANG_SMALL        (_RANG_SMALL_UP_BEGIN               - _RANG_SMALL_UMAX_BEGIN      )
#define NUMBER_UP_SIGNAL_FOR_RANG_SMALL          (_RANG_SMALL_VMP_BEGIN              - _RANG_SMALL_UP_BEGIN        )
#define NUMBER_VMP_SIGNAL_FOR_RANG_SMALL         (_RANG_SMALL_EL_BEGIN               - _RANG_SMALL_VMP_BEGIN       )
#define NUMBER_EL_SIGNAL_FOR_RANG_SMALL          (NUMBER_TOTAL_SIGNAL_FOR_RANG_SMALL - _RANG_SMALL_EL_BEGIN        )

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
RANG_PEREVYSHCHENNJA_Inom_VYMK,
RANG_KRYTYCHNYJ_RESURS_VYMYKACHA,
RANG_VYCHERPANYJ_RESURS_VYMYKACHA,
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

RANG_BLOCK_IN_GOOSE1,

RANG_BLOCK_IN_MMS1 = (RANG_BLOCK_IN_GOOSE1 + N_IN_GOOSE),

RANG_BLOCK_OUT_LAN1 = (RANG_BLOCK_IN_MMS1 + N_IN_MMS),

  _RANG_MSZ_BEGIN = (RANG_BLOCK_OUT_LAN1 + N_OUT_LAN),
RANG_BLOCK_MTZ1 = _RANG_MSZ_BEGIN,
RANG_BLOCK_MTZ2,
RANG_BLOCK_USK_MTZ2,
RANG_BLOCK_MTZ3,
RANG_BLOCK_MTZ4,
RANG_SECTOR_VPERED_MTZN1,
RANG_SECTOR_NAZAD_MTZN1,
RANG_PO_MTZ1,
RANG_PO_MTZN1_VPERED,
RANG_PO_MTZN1_NAZAD,
RANG_PO_U_MTZPN1,
RANG_PO_MTZPN1,
RANG_MTZ1,
RANG_SECTOR_VPERED_MTZN2,
RANG_SECTOR_NAZAD_MTZN2,
RANG_PO_MTZ2,
RANG_PO_MTZN2_VPERED,
RANG_PO_MTZN2_NAZAD,
RANG_PO_U_MTZPN2,
RANG_PO_MTZPN2,
RANG_MTZ2,
RANG_SECTOR_VPERED_MTZN3,
RANG_SECTOR_NAZAD_MTZN3,
RANG_PO_MTZ3,
RANG_PO_MTZN3_VPERED,
RANG_PO_MTZN3_NAZAD,
RANG_PO_U_MTZPN3,
RANG_PO_MTZPN3,
RANG_MTZ3,
RANG_SECTOR_VPERED_MTZN4,
RANG_SECTOR_NAZAD_MTZN4,
RANG_PO_MTZ4,
RANG_PO_MTZN4_VPERED,
RANG_PO_MTZN4_NAZAD,
RANG_PO_U_MTZPN4,
RANG_PO_MTZPN4,
RANG_MTZ4,
RANG_PO_BLOCK_U_MTZN,
RANG_NCN_MTZ,

  _RANG_MSZ04_BEGIN,
RANG_BLOCK_MTZ04_1 = _RANG_MSZ04_BEGIN,
RANG_BLOCK_MTZ04_2,
RANG_BLOCK_USK_MTZ04_2,
RANG_PO_MTZ04_1,
RANG_MTZ04_1,
RANG_PO_MTZ04_2,
RANG_MTZ04_2,

  _RANG_ZNAM_BEGIN,
RANG_BLOCK_ZNAM = _RANG_ZNAM_BEGIN,
RANG_PO_ZNAM,
RANG_ZNAM,


  _RANG_ZDZ_BEGIN,
RANG_BLOCK_ZDZ = _RANG_ZDZ_BEGIN,
RANG_PUSK_ZDZ_VID_DV,
RANG_LIGHT_ZDZ_FROM_DV,
RANG_PO_ZDZ,
RANG_ZDZ,

  _RANG_SZZ_BEGIN,
RANG_BLOCK_NZZ = _RANG_SZZ_BEGIN,
RANG_PO_NZZ,
RANG_NZZ,
RANG_PO_3I0,
RANG_3I0,
RANG_PO_3U0,
RANG_3U0,
RANG_SECTOR_NZZ,

  _RANG_SZNP_BEGIN,
RANG_BLOCK_TZNP1 = _RANG_SZNP_BEGIN,
RANG_SECTOR_TZNP1_VPERED,
RANG_SECTOR_TZNP1_NAZAD,
RANG_PO_3I0_TZNP1_VPERED,
RANG_PO_3I0_TZNP1_NAZAD,
RANG_PO_3U0_TZNP1_VPERED,
RANG_PO_3U0_TZNP1_NAZAD,
RANG_PO_TZNP1_VPERED,
RANG_PO_TZNP1_NAZAD,
RANG_TZNP1,
RANG_BLOCK_TZNP2,
RANG_SECTOR_TZNP2_VPERED,
RANG_SECTOR_TZNP2_NAZAD,
RANG_PO_3I0_TZNP2_VPERED,
RANG_PO_3I0_TZNP2_NAZAD,
RANG_PO_3U0_TZNP2_VPERED,
RANG_PO_3U0_TZNP2_NAZAD,
RANG_PO_TZNP2_VPERED,
RANG_PO_TZNP2_NAZAD,
RANG_TZNP2,
RANG_BLOCK_TZNP3,
RANG_SECTOR_TZNP3_VPERED,
RANG_SECTOR_TZNP3_NAZAD,
RANG_PO_3I0_TZNP3_VPERED,
RANG_PO_3I0_TZNP3_NAZAD,
RANG_PO_3U0_TZNP3_VPERED,
RANG_PO_3U0_TZNP3_NAZAD,
RANG_PO_TZNP3_VPERED,
RANG_PO_TZNP3_NAZAD,
RANG_TZNP3,

  _RANG_APV_BEGIN,
RANG_STAT_BLK_APV = _RANG_APV_BEGIN,
RANG_APV1,
RANG_APV2,
RANG_APV3,
RANG_APV4,
RANG_APV_WORK,

  _RANG_ACHR_CHAPV_BEGIN,
RANG_ACHR_CHAPV_VID_DV = _RANG_ACHR_CHAPV_BEGIN,
RANG_BLOCK_ACHR1,
RANG_BLOCK_ACHR2,
RANG_RAZR_CHAPV,
RANG_VN_BLOCK_CHAPV,
RANG_PO_ACHR1,
RANG_PO_CHAPV1,
RANG_ACHR_CHAPV1,
RANG_PO_ACHR2,
RANG_PO_CHAPV2,
RANG_ACHR_CHAPV2,

  _RANG_PRVV_BEGIN,
RANG_BLOCK_UROV = _RANG_PRVV_BEGIN,
RANG_PUSK_UROV_VID_DV,
RANG_PO_UROV,
RANG_UROV1,
RANG_UROV2,

  _RANG_ZZP_BEGIN,
RANG_BLOCK_ZOP = _RANG_ZZP_BEGIN,
RANG_PO_ZOP,
RANG_ZOP,

  _RANG_UMIN_BEGIN,
RANG_BLOCK_UMIN1 = _RANG_UMIN_BEGIN,
RANG_START_UMIN1,
RANG_BLOCK_UMIN2,
RANG_START_UMIN2,
RANG_PO_UMIN1,
RANG_PO_UBLK_UMIN1,
RANG_PO_IBLK_UMIN1,
RANG_UMIN1,
RANG_PO_UMIN2,
RANG_PO_UBLK_UMIN2,
RANG_PO_IBLK_UMIN2,
RANG_UMIN2,

  _RANG_UMAX_BEGIN,
RANG_BLOCK_UMAX1 = _RANG_UMAX_BEGIN,
RANG_PO_UMAX1,
RANG_UMAX1,
RANG_BLOCK_UMAX2,
RANG_PO_UMAX2,
RANG_UMAX2,

  _RANG_UP_BEGIN,
RANG_BLOCK_UP1 = _RANG_UP_BEGIN,
RANG_PO_UP1,
RANG_UP1,

  _RANG_VMP_BEGIN = (RANG_BLOCK_UP1 + 3*NUMBER_UP),

  _RANG_EL_BEGIN = _RANG_VMP_BEGIN,
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

#define NUMBER_GENERAL_SIGNAL_FOR_RANG     (_RANG_MSZ_BEGIN              - _RANG_GENERAL_BEGIN   )
#define NUMBER_MTZ_SIGNAL_FOR_RANG         (_RANG_MSZ04_BEGIN            - _RANG_MSZ_BEGIN       )
#define NUMBER_MTZ04_SIGNAL_FOR_RANG       (_RANG_ZNAM_BEGIN             - _RANG_MSZ04_BEGIN     )
#define NUMBER_ZNAM_SIGNAL_FOR_RANG         (_RANG_ZDZ_BEGIN             - _RANG_ZNAM_BEGIN      )
#define NUMBER_ZDZ_SIGNAL_FOR_RANG         (_RANG_SZZ_BEGIN              - _RANG_ZDZ_BEGIN       )
#define NUMBER_ZZ_SIGNAL_FOR_RANG          (_RANG_SZNP_BEGIN             - _RANG_SZZ_BEGIN       )
#define NUMBER_TZNP_SIGNAL_FOR_RANG        (_RANG_APV_BEGIN              - _RANG_SZNP_BEGIN      )
#define NUMBER_APV_SIGNAL_FOR_RANG         (_RANG_ACHR_CHAPV_BEGIN       - _RANG_APV_BEGIN       )
#define NUMBER_ACHR_CHAPV_SIGNAL_FOR_RANG  (_RANG_PRVV_BEGIN             - _RANG_ACHR_CHAPV_BEGIN)
#define NUMBER_UROV_SIGNAL_FOR_RANG        (_RANG_ZZP_BEGIN              - _RANG_PRVV_BEGIN      )
#define NUMBER_ZOP_SIGNAL_FOR_RANG         (_RANG_UMIN_BEGIN             - _RANG_ZZP_BEGIN       )
#define NUMBER_UMIN_SIGNAL_FOR_RANG        (_RANG_UMAX_BEGIN             - _RANG_UMIN_BEGIN      )
#define NUMBER_UMAX_SIGNAL_FOR_RANG        (_RANG_UP_BEGIN               - _RANG_UMAX_BEGIN      )
#define NUMBER_UP_SIGNAL_FOR_RANG          (_RANG_VMP_BEGIN              - _RANG_UP_BEGIN        )
#define NUMBER_VMP_SIGNAL_FOR_RANG         (_RANG_EL_BEGIN               - _RANG_VMP_BEGIN       )
#define NUMBER_EL_SIGNAL_FOR_RANG          (NUMBER_TOTAL_SIGNAL_FOR_RANG - _RANG_EL_BEGIN        )

/*****************************************/

/*****************************************/
//Макска команд
/*****************************************/
#define COMMAND_SIGNALES_0 (                                  \
    (1u << (RANG_SMALL_RESET_LEDS - 0))                       \
  | (1u << (RANG_SMALL_RESET_RELES - 0))                      \
  | (1u << (RANG_SMALL_OTKL_VV - 0))                          \
  | (1u << (RANG_SMALL_VKL_VV - 0))                           \
  | (1u << (RANG_SMALL_RESET_BLOCK_READY_TU_VID_ZAHYSTIV - 0))\
)

#define COMMAND_SIGNALES_1        0

#define COMMAND_SIGNALES_2 (                                  \
    (1u << (RANG_SMALL_DF1_IN - 64))                          \
  | (1u << (RANG_SMALL_DF1_R - 64))                           \
  | (1u << (RANG_SMALL_DF2_IN - 64))                          \
  | (1u << (RANG_SMALL_DF2_R - 64))                           \
  | (1u << (RANG_SMALL_DF3_IN - 64))                          \
  | (1u << (RANG_SMALL_DF3_R - 64))                           \
  | (1u << (RANG_SMALL_DF4_IN - 64))                          \
  | (1u << (RANG_SMALL_DF4_R - 64))                           \
  | (1u << (RANG_SMALL_DF5_IN - 64))                          \
  | (1u << (RANG_SMALL_DF5_R - 64))                           \
  | (1u << (RANG_SMALL_DF6_IN - 64))                          \
  | (1u << (RANG_SMALL_DF6_R - 64))                           \
  | (1u << (RANG_SMALL_DF7_IN - 64))                          \
)

#define COMMAND_SIGNALES_3 (                                  \
    (1u << (RANG_SMALL_DF7_R - 96))                           \
  | (1u << (RANG_SMALL_DF8_IN - 96))                          \
  | (1u << (RANG_SMALL_DF8_R - 96))                           \
  | (1u << (RANG_SMALL_DT1_SET - 96))                         \
  | (1u << (RANG_SMALL_DT1_RESET - 96))                       \
  | (1u << (RANG_SMALL_DT2_SET - 96))                         \
  | (1u << (RANG_SMALL_DT2_RESET - 96))                       \
  | (1u << (RANG_SMALL_DT3_SET - 96))                         \
  | (1u << (RANG_SMALL_DT3_RESET - 96))                       \
  | (1u << (RANG_SMALL_DT4_SET - 96))                         \
  | (1u << (RANG_SMALL_DT4_RESET - 96))                       \
  | (1u << (RANG_SMALL_LF1 - 96))                             \
  | (1u << (RANG_SMALL_LF2 - 96))                             \
  | (1u << (RANG_SMALL_LF3 - 96))                             \
  | (1u << (RANG_SMALL_LF4 - 96))                             \
  | (1u << (RANG_SMALL_LF5 - 96))                             \
  | (1u << (RANG_SMALL_LF6 - 96))                             \
  | (1u << (RANG_SMALL_LF7 - 96))                             \
  | (1u << (RANG_SMALL_LF8 - 96))                             \
  | (1u << (RANG_SMALL_LF9 - 96))                             \
  | (1u << (RANG_SMALL_LF10 - 96))                            \
  | (1u << (RANG_SMALL_LF11 - 96))                            \
  | (1u << (RANG_SMALL_LF12 - 96))                            \
  | (1u << (RANG_SMALL_LF13 - 96))                            \
  | (1u << (RANG_SMALL_LF14 - 96))                            \
  | (1u << (RANG_SMALL_LF15 - 96))                            \
  | (1u << (RANG_SMALL_LF16 - 96))                            \
)
/*****************************************/

/*****************************************/
//Макска команд, які можуть активовуватися з КП безпосередньо
/*****************************************/
#define COMMAND_PERMITTED_FROM_KP_SIGNALES_0 (                 \
    (1u << (RANG_SMALL_RESET_LEDS - 0))                        \
  | (1u << (RANG_SMALL_RESET_RELES - 0))                       \
  | (1u << (RANG_SMALL_VKL_VV - 0))                            \
  | (1u << (RANG_SMALL_OTKL_VV - 0))                           \
)

#define COMMAND_PERMITTED_FROM_KP_SIGNALES_1        0
#define COMMAND_PERMITTED_FROM_KP_SIGNALES_2        0
#define COMMAND_PERMITTED_FROM_KP_SIGNALES_3        0
/*****************************************/

/*****************************************/
//Макски сигналів, які можна зранжувати на функціональні кнопки у Режимі Кнопка
/*****************************************/
#define MASKA_BUTTON_MODE_0_SIGNALS_0 (                        \
    (1u << (RANG_SMALL_RESET_LEDS - 0))                        \
  | (1u << (RANG_SMALL_RESET_RELES - 0))                       \
  | (1u << (RANG_SMALL_RESET_BLOCK_READY_TU_VID_ZAHYSTIV - 0)) \
)

#define MASKA_BUTTON_MODE_0_SIGNALS_1           0

#define MASKA_BUTTON_MODE_0_SIGNALS_2 (                        \
    (1u << (RANG_SMALL_DF1_IN - 64))                           \
  | (1u << (RANG_SMALL_DF1_R - 64))                            \
  | (1u << (RANG_SMALL_DF2_IN - 64))                           \
  | (1u << (RANG_SMALL_DF2_R - 64))                            \
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
)

#define MASKA_BUTTON_MODE_0_SIGNALS_3 (                        \
    (1u << (RANG_SMALL_DT3_RESET - 96))                        \
  | (1u << (RANG_SMALL_DT4_SET - 96))                          \
  | (1u << (RANG_SMALL_DT4_RESET - 96))                        \
  | (1u << (RANG_SMALL_LF1 - 96))                              \
  | (1u << (RANG_SMALL_LF2 - 96))                              \
  | (1u << (RANG_SMALL_LF3 - 96))                              \
  | (1u << (RANG_SMALL_LF4 - 96))                              \
  | (1u << (RANG_SMALL_LF5 - 96))                              \
  | (1u << (RANG_SMALL_LF6 - 96))                              \
  | (1u << (RANG_SMALL_LF7 - 96))                              \
  | (1u << (RANG_SMALL_LF8 - 96))                              \
  | (1u << (RANG_SMALL_LF9 - 96))                              \
  | (1u << (RANG_SMALL_LF10 - 96))                             \
  | (1u << (RANG_SMALL_LF11 - 96))                             \
  | (1u << (RANG_SMALL_LF12 - 96))                             \
  | (1u << (RANG_SMALL_LF13 - 96))                             \
  | (1u << (RANG_SMALL_LF14 - 96))                             \
  | (1u << (RANG_SMALL_LF15 - 96))                             \
  | (1u << (RANG_SMALL_LF16 - 96))                             \
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
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 +  0 - 0))              \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 +  1 - 0))              \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 +  2 - 0))              \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 +  3 - 0))              \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 +  4 - 0))              \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 +  5 - 0))              \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 +  6 - 0))              \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 +  7 - 0))              \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 +  8 - 0))              \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 +  9 - 0))              \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 10 - 0))              \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 11 - 0))              \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 12 - 0))              \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 13 - 0))              \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 14 - 0))              \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 15 - 0))              \
  | (1u << (RANG_SMALL_BLOCK_IN_MMS1 + 0 - 0))                 \
)

#define MASKA_BUTTON_MODE_1_SIGNALS_1 (                        \
    (1u << (RANG_SMALL_BLOCK_IN_MMS1 + 1 - 32))                \
  | (1u << (RANG_SMALL_BLOCK_IN_MMS1 + 2 - 32))                \
  | (1u << (RANG_SMALL_BLOCK_IN_MMS1 + 3 - 32))                \
  | (1u << (RANG_SMALL_BLOCK_OUT_LAN1 + 0 - 32))               \
  | (1u << (RANG_SMALL_BLOCK_OUT_LAN1 + 1 - 32))               \
  | (1u << (RANG_SMALL_BLOCK_OUT_LAN1 + 2 - 32))               \
  | (1u << (RANG_SMALL_BLOCK_OUT_LAN1 + 3 - 32))               \
  | (1u << (RANG_SMALL_BLOCK_MTZ1 - 32))                       \
  | (1u << (RANG_SMALL_BLOCK_MTZ2 - 32))                       \
  | (1u << (RANG_SMALL_BLOCK_USK_MTZ2 - 32))                   \
  | (1u << (RANG_SMALL_BLOCK_MTZ3 - 32))                       \
  | (1u << (RANG_SMALL_BLOCK_MTZ4 - 32))                       \
  | (1u << (RANG_SMALL_BLOCK_MTZ04_1 - 32))                    \
  | (1u << (RANG_SMALL_BLOCK_MTZ04_2 - 32))                    \
  | (1u << (RANG_SMALL_BLOCK_USK_MTZ04_2 - 32))                \
  | (1u << (RANG_SMALL_BLOCK_ZNAM - 32))                       \
  | (1u << (RANG_SMALL_BLOCK_ZDZ - 32))                        \
  | (1u << (RANG_SMALL_BLOCK_NZZ - 32))                        \
  | (1u << (RANG_SMALL_BLOCK_TZNP1 - 32))                      \
  | (1u << (RANG_SMALL_BLOCK_TZNP2 - 32))                      \
  | (1u << (RANG_SMALL_BLOCK_TZNP3 - 32))                      \
  | (1u << (RANG_SMALL_STAT_BLK_APV - 32))                     \
  | (1u << (RANG_SMALL_BLOCK_ACHR1 - 32))                      \
  | (1u << (RANG_SMALL_BLOCK_ACHR2 - 32))                      \
  | (1u << (RANG_SMALL_BLOCK_UROV - 32))					   \
  | (1u << (RANG_SMALL_BLOCK_ZOP - 32))                        \
  | (1u << (RANG_SMALL_BLOCK_UMIN1 - 32))                      \
  | (1u << (RANG_SMALL_BLOCK_UMIN2 - 32))                      \
)

#define MASKA_BUTTON_MODE_1_SIGNALS_2 (                        \
    (1u << (RANG_SMALL_BLOCK_UMAX1 - 64))                      \
  | (1u << (RANG_SMALL_BLOCK_UMAX2 - 64))                      \
  | (1u << (RANG_SMALL_BLOCK_UP1 + 0 - 64))                    \
  | (1u << (RANG_SMALL_BLOCK_UP1 + 1 - 64))                    \
  | (1u << (RANG_SMALL_BLOCK_UP1 + 2 - 64))                    \
  | (1u << (RANG_SMALL_BLOCK_UP1 + 3 - 64))                    \
  | (1u << (RANG_SMALL_BLOCK_UP1 + 4 - 64))                    \
  | (1u << (RANG_SMALL_BLOCK_UP1 + 5 - 64))                    \
  | (1u << (RANG_SMALL_BLOCK_UP1 + 6 - 64))                    \
  | (1u << (RANG_SMALL_BLOCK_UP1 + 7 - 64))                    \
  | (1u << (RANG_SMALL_DF1_IN - 64))                           \
  | (1u << (RANG_SMALL_DF1_R - 64))                            \
  | (1u << (RANG_SMALL_DF2_IN - 64))                           \
  | (1u << (RANG_SMALL_DF2_R - 64))                            \
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
)

#define MASKA_BUTTON_MODE_1_SIGNALS_3 (                        \
    (1u << (RANG_SMALL_DT3_RESET - 96))                        \
  | (1u << (RANG_SMALL_DT4_SET - 96))                          \
  | (1u << (RANG_SMALL_DT4_RESET - 96))                        \
  | (1u << (RANG_SMALL_LF1 - 96))                              \
  | (1u << (RANG_SMALL_LF2 - 96))                              \
  | (1u << (RANG_SMALL_LF3 - 96))                              \
  | (1u << (RANG_SMALL_LF4 - 96))                              \
  | (1u << (RANG_SMALL_LF5 - 96))                              \
  | (1u << (RANG_SMALL_LF6 - 96))                              \
  | (1u << (RANG_SMALL_LF7 - 96))                              \
  | (1u << (RANG_SMALL_LF8 - 96))                              \
  | (1u << (RANG_SMALL_LF9 - 96))                              \
  | (1u << (RANG_SMALL_LF10 - 96))                             \
  | (1u << (RANG_SMALL_LF11 - 96))                             \
  | (1u << (RANG_SMALL_LF12 - 96))                             \
  | (1u << (RANG_SMALL_LF13 - 96))                             \
  | (1u << (RANG_SMALL_LF14 - 96))                             \
  | (1u << (RANG_SMALL_LF15 - 96))                             \
  | (1u << (RANG_SMALL_LF16 - 96))                             \
)
/*****************************************/

/*****************************************/
//Макски сигналів, які можна зранжувати на виходи Вхідного GOOSE блоку
/*****************************************/
#define MASKA_IN_GOOSE_SIGNALS_0 (                             \
    (1u << (RANG_SMALL_BLOCK_VKL_VV - 0))                      \
  | (1u << (RANG_SMALL_STATE_VV - 0))                          \
  | (1u << (RANG_SMALL_OTKL_VID_ZOVN_ZAHYSTIV - 0))            \
  | (1u << (RANG_SMALL_VKL_VV - 0))                            \
  | (1u << (RANG_SMALL_CTRL_VKL - 0))                          \
  | (1u << (RANG_SMALL_OTKL_VV - 0))                           \
  | (1u << (RANG_SMALL_CTRL_OTKL - 0))                         \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 +  0 - 0))              \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 +  1 - 0))              \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 +  2 - 0))              \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 +  3 - 0))              \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 +  4 - 0))              \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 +  5 - 0))              \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 +  6 - 0))              \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 +  7 - 0))              \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 +  8 - 0))              \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 +  9 - 0))              \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 10 - 0))              \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 11 - 0))              \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 12 - 0))              \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 13 - 0))              \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 14 - 0))              \
  | (1u << (RANG_SMALL_BLOCK_IN_GOOSE1 + 15 - 0))              \
)

#define MASKA_IN_GOOSE_SIGNALS_1 (                             \
    (1u << (RANG_SMALL_BLOCK_OUT_LAN1 + 0 - 32))               \
  | (1u << (RANG_SMALL_BLOCK_OUT_LAN1 + 1 - 32))               \
  | (1u << (RANG_SMALL_BLOCK_OUT_LAN1 + 2 - 32))               \
  | (1u << (RANG_SMALL_BLOCK_OUT_LAN1 + 3 - 32))               \
  | (1u << (RANG_SMALL_BLOCK_MTZ1 - 32))                       \
  | (1u << (RANG_SMALL_BLOCK_MTZ2 - 32))                       \
  | (1u << (RANG_SMALL_BLOCK_USK_MTZ2 - 32))                   \
  | (1u << (RANG_SMALL_BLOCK_MTZ3 - 32))                       \
  | (1u << (RANG_SMALL_BLOCK_MTZ4 - 32))                       \
  | (1u << (RANG_SMALL_BLOCK_MTZ04_1 - 32))                    \
  | (1u << (RANG_SMALL_BLOCK_MTZ04_2 - 32))                    \
  | (1u << (RANG_SMALL_BLOCK_USK_MTZ04_2 - 32))                \
  | (1u << (RANG_SMALL_BLOCK_ZNAM - 32))                       \
  | (1u << (RANG_SMALL_BLOCK_ZDZ - 32))                        \
  | (1u << (RANG_SMALL_PUSK_ZDZ_VID_DV - 32))                  \
  | (1u << (RANG_SMALL_BLOCK_NZZ - 32))                        \
  | (1u << (RANG_SMALL_BLOCK_TZNP1 - 32))                      \
  | (1u << (RANG_SMALL_BLOCK_TZNP2 - 32))                      \
  | (1u << (RANG_SMALL_BLOCK_TZNP3 - 32))                      \
  | (1u << (RANG_SMALL_STAT_BLK_APV - 32))                     \
  | (1u << (RANG_SMALL_ACHR_CHAPV_VID_DV - 32))                \
  | (1u << (RANG_SMALL_BLOCK_ACHR1 - 32))                      \
  | (1u << (RANG_SMALL_BLOCK_ACHR2 - 32))                      \
  | (1u << (RANG_SMALL_BLOCK_UROV - 32))						           \
  | (1u << (RANG_SMALL_PUSK_UROV_VID_DV - 32))                 \
  | (1u << (RANG_SMALL_BLOCK_ZOP - 32))                        \
  | (1u << (RANG_SMALL_BLOCK_UMIN1 - 32))                      \
  | (1u << (RANG_SMALL_START_UMIN1 - 32))                      \
  | (1u << (RANG_SMALL_BLOCK_UMIN2 - 32))                      \
)

#define MASKA_IN_GOOSE_SIGNALS_2 (                             \
    (1u << (RANG_SMALL_START_UMIN2 - 64))                      \
  | (1u << (RANG_SMALL_BLOCK_UMAX1 - 64))                      \
  | (1u << (RANG_SMALL_BLOCK_UMAX2 - 64))                      \
  | (1u << (RANG_SMALL_BLOCK_UP1 + 0 - 64))                    \
  | (1u << (RANG_SMALL_BLOCK_UP1 + 1 - 64))                    \
  | (1u << (RANG_SMALL_BLOCK_UP1 + 2 - 64))                    \
  | (1u << (RANG_SMALL_BLOCK_UP1 + 3 - 64))                    \
  | (1u << (RANG_SMALL_BLOCK_UP1 + 4 - 64))                    \
  | (1u << (RANG_SMALL_BLOCK_UP1 + 5 - 64))                    \
  | (1u << (RANG_SMALL_BLOCK_UP1 + 6 - 64))                    \
  | (1u << (RANG_SMALL_BLOCK_UP1 + 7 - 64))                    \
  | (1u << (RANG_SMALL_DF1_IN - 64))                           \
  | (1u << (RANG_SMALL_DF1_R - 64))                            \
  | (1u << (RANG_SMALL_DF2_IN - 64))                           \
  | (1u << (RANG_SMALL_DF2_R - 64))                            \
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
)


#define MASKA_IN_GOOSE_SIGNALS_3 (                             \
    (1u << (RANG_SMALL_DT3_RESET - 96))                        \
  | (1u << (RANG_SMALL_DT4_SET - 96))                          \
  | (1u << (RANG_SMALL_DT4_RESET - 96))                        \
  | (1u << (RANG_SMALL_LF1 - 96))                              \
  | (1u << (RANG_SMALL_LF2 - 96))                              \
  | (1u << (RANG_SMALL_LF3 - 96))                              \
  | (1u << (RANG_SMALL_LF4 - 96))                              \
  | (1u << (RANG_SMALL_LF5 - 96))                              \
  | (1u << (RANG_SMALL_LF6 - 96))                              \
  | (1u << (RANG_SMALL_LF7 - 96))                              \
  | (1u << (RANG_SMALL_LF8 - 96))                              \
  | (1u << (RANG_SMALL_LF9 - 96))                              \
  | (1u << (RANG_SMALL_LF10 - 96))                             \
  | (1u << (RANG_SMALL_LF11 - 96))                             \
  | (1u << (RANG_SMALL_LF12 - 96))                             \
  | (1u << (RANG_SMALL_LF13 - 96))                             \
  | (1u << (RANG_SMALL_LF14 - 96))                             \
  | (1u << (RANG_SMALL_LF15 - 96))                             \
  | (1u << (RANG_SMALL_LF16 - 96))                             \
)
/*****************************************/

/*****************************************/
//Макски сигналів, які можна зранжувати на виходи Вхідного MMS блоку
/*****************************************/
#define MASKA_IN_MMS_SIGNALS_0 (                               \
    (1u << (RANG_SMALL_RESET_LEDS - 0))                        \
  | (1u << (RANG_SMALL_RESET_RELES - 0))                       \
  | (1u << (RANG_SMALL_VKL_VV - 0))                            \
  | (1u << (RANG_SMALL_OTKL_VV - 0))                           \
  | (1u << (RANG_SMALL_RESET_BLOCK_READY_TU_VID_ZAHYSTIV - 0)) \
)

#define MASKA_IN_MMS_SIGNALS_1           0

#define MASKA_IN_MMS_SIGNALS_2 (                               \
    (1u << (RANG_SMALL_DF1_IN - 64))                           \
  | (1u << (RANG_SMALL_DF1_R - 64))                            \
  | (1u << (RANG_SMALL_DF2_IN - 64))                           \
  | (1u << (RANG_SMALL_DF2_R - 64))                            \
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
)

#define MASKA_IN_MMS_SIGNALS_3 (                               \
    (1u << (RANG_SMALL_DT3_RESET - 96))                        \
  | (1u << (RANG_SMALL_DT4_SET - 96))                          \
  | (1u << (RANG_SMALL_DT4_RESET - 96))                        \
  | (1u << (RANG_SMALL_LF1 - 96))                              \
  | (1u << (RANG_SMALL_LF2 - 96))                              \
  | (1u << (RANG_SMALL_LF3 - 96))                              \
  | (1u << (RANG_SMALL_LF4 - 96))                              \
  | (1u << (RANG_SMALL_LF5 - 96))                              \
  | (1u << (RANG_SMALL_LF6 - 96))                              \
  | (1u << (RANG_SMALL_LF7 - 96))                              \
  | (1u << (RANG_SMALL_LF8 - 96))                              \
  | (1u << (RANG_SMALL_LF9 - 96))                              \
  | (1u << (RANG_SMALL_LF10 - 96))                             \
  | (1u << (RANG_SMALL_LF11 - 96))                             \
  | (1u << (RANG_SMALL_LF12 - 96))                             \
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
)

#define MASKA_FOR_INPUT_SIGNALS_1 (                              \
    (1u << (RANG_RESET_BLOCK_READY_TU_VID_ZAHYSTIV - 32))        \
  | (1u << (RANG_BLOCK_IN_GOOSE1 +  0 - 32))                     \
  | (1u << (RANG_BLOCK_IN_GOOSE1 +  1 - 32))                     \
  | (1u << (RANG_BLOCK_IN_GOOSE1 +  2 - 32))                     \
  | (1u << (RANG_BLOCK_IN_GOOSE1 +  3 - 32))                     \
  | (1u << (RANG_BLOCK_IN_GOOSE1 +  4 - 32))                     \
  | (1u << (RANG_BLOCK_IN_GOOSE1 +  5 - 32))                     \
  | (1u << (RANG_BLOCK_IN_GOOSE1 +  6 - 32))                     \
  | (1u << (RANG_BLOCK_IN_GOOSE1 +  7 - 32))                     \
  | (1u << (RANG_BLOCK_IN_GOOSE1 +  8 - 32))                     \
  | (1u << (RANG_BLOCK_IN_GOOSE1 +  9 - 32))                     \
  | (1u << (RANG_BLOCK_IN_GOOSE1 + 10 - 32))                     \
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
  | (1u << (RANG_BLOCK_MTZ1 - 32))                               \
  | (1u << (RANG_BLOCK_MTZ2 - 32))                               \
  | (1u << (RANG_BLOCK_USK_MTZ2 - 32))                           \
  | (1u << (RANG_BLOCK_MTZ3 - 32))                               \
)

#define MASKA_FOR_INPUT_SIGNALS_2 (                            \
    (1u << (RANG_BLOCK_MTZ4 - 64))                             \
)
    
#define MASKA_FOR_INPUT_SIGNALS_3 (                             \
    (1u << (RANG_BLOCK_MTZ04_1 - 96))                           \
  | (1u << (RANG_BLOCK_MTZ04_2 - 96))                           \
  | (1u << (RANG_BLOCK_USK_MTZ04_2 - 96))                       \
  | (1u << (RANG_BLOCK_ZNAM - 96))                              \
  | (1u << (RANG_BLOCK_ZDZ - 96))                               \
  | (1u << (RANG_PUSK_ZDZ_VID_DV - 96))                         \
  | (1u << (RANG_BLOCK_NZZ - 96))                               \
  | (1u << (RANG_BLOCK_TZNP1 - 96))                             \
)
    
#define MASKA_FOR_INPUT_SIGNALS_4 (                            \
    (1u << (RANG_BLOCK_TZNP2 - 128))                           \
  | (1u << (RANG_BLOCK_TZNP3 - 128))                           \
  | (1u << (RANG_STAT_BLK_APV - 128))                          \
  | (1u << (RANG_ACHR_CHAPV_VID_DV - 128))                     \
  | (1u << (RANG_BLOCK_ACHR1 - 128))                           \
)
    
#define MASKA_FOR_INPUT_SIGNALS_5 (                            \
    (1u << (RANG_BLOCK_ACHR2 - 160))                           \
  | (1u << (RANG_BLOCK_UROV - 160))                      	     \
  | (1u << (RANG_PUSK_UROV_VID_DV - 160))                      \
  | (1u << (RANG_BLOCK_ZOP - 160))                             \
  | (1u << (RANG_BLOCK_UMIN1 - 160))                           \
  | (1u << (RANG_START_UMIN1 - 160))                           \
  | (1u << (RANG_BLOCK_UMIN2 - 160))                           \
  | (1u << (RANG_START_UMIN2 - 160))                           \
  | (1u << (RANG_BLOCK_UMAX1 - 160))                           \
)

#define MASKA_FOR_INPUT_SIGNALS_6 (                            \
    (1u << (RANG_BLOCK_UMAX2 - 192))                           \
  | (1u << (RANG_BLOCK_UP1 + 3*0 - 192))                       \
  | (1u << (RANG_BLOCK_UP1 + 3*1 - 192))                       \
  | (1u << (RANG_BLOCK_UP1 + 3*2 - 192))                       \
  | (1u << (RANG_BLOCK_UP1 + 3*3 - 192))                       \
  | (1u << (RANG_BLOCK_UP1 + 3*4 - 192))                       \
  | (1u << (RANG_BLOCK_UP1 + 3*5 - 192))                       \
  | (1u << (RANG_BLOCK_UP1 + 3*6 - 192))                       \
  | (1u << (RANG_BLOCK_UP1 + 3*7 - 192))                       \
  | (1u << (RANG_DF1_IN - 192))                                \
  | (1u << (RANG_DF1_R - 192))                                 \
  | (1u << (RANG_DF2_IN - 192))                                \
  | (1u << (RANG_DF2_R - 192))                                 \
)

#define MASKA_FOR_INPUT_SIGNALS_7 (                            \
    (1u << (RANG_DF3_IN - 224))                                \
  | (1u << (RANG_DF3_R - 224))                                 \
  | (1u << (RANG_DF4_IN - 224))                                \
  | (1u << (RANG_DF4_R - 224))                                 \
  | (1u << (RANG_DF5_IN - 224))                                \
  | (1u << (RANG_DF5_R - 224))                                 \
  | (1u << (RANG_DF6_IN - 224))                                \
  | (1u << (RANG_DF6_R - 224))                                 \
  | (1u << (RANG_DF7_IN - 224))                                \
  | (1u << (RANG_DF7_R - 224))                                 \
  | (1u << (RANG_DF8_IN - 224))                                \
  | (1u << (RANG_DF8_R - 224))                                 \
  | (1u << (RANG_DT1_SET - 224))                               \
  | (1u << (RANG_DT1_RESET - 224))                             \
  | (1u << (RANG_DT2_SET - 224))                               \
  | (1u << (RANG_DT2_RESET - 224))                             \
  | (1u << (RANG_DT3_SET - 224))                               \
  | (1u << (RANG_DT3_RESET - 224))                             \
  | (1u << (RANG_DT4_SET - 224))                               \
  | (1u << (RANG_DT4_RESET - 224))                             \
)

#define MASKA_FOR_INPUT_SIGNALS_8     0

#define MASKA_FOR_INPUT_SIGNALS_9 (                            \
    (1u << (RANG_LF1 - 288))                                   \
  | (1u << (RANG_LF2 - 288))                                   \
  | (1u << (RANG_LF3 - 288))                                   \
  | (1u << (RANG_LF4 - 288))                                   \
  | (1u << (RANG_LF5 - 288))                                   \
  | (1u << (RANG_LF6 - 288))                                   \
  | (1u << (RANG_LF7 - 288))                                   \
  | (1u << (RANG_LF8 - 288))                                   \
  | (1u << (RANG_LF9 - 288))                                   \
  | (1u << (RANG_LF10 - 288))                                  \
  | (1u << (RANG_LF11 - 288))                                  \
  | (1u << (RANG_LF12 - 288))                                  \
  | (1u << (RANG_LF13 - 288))                                  \
  | (1u << (RANG_LF14 - 288))                                  \
  | (1u << (RANG_LF15 - 288))                                  \
  | (1u << (RANG_LF16 - 288))                                  \
)
/*****************************************/

/*****************************************/
//Макски всіх сигналів МСЗ
/*****************************************/
#define MASKA_MTZ_SIGNALS_0                  0

#define MASKA_MTZ_SIGNALS_1 (                                  \
     (1u << (RANG_BLOCK_MTZ1 - 32))                            \
   | (1u << (RANG_BLOCK_MTZ2 - 32))                            \
   | (1u << (RANG_BLOCK_USK_MTZ2 - 32))                        \
   | (1u << (RANG_BLOCK_MTZ3 - 32))                            \
)

#define MASKA_MTZ_SIGNALS_2 (                                  \
     (1u << (RANG_BLOCK_MTZ4 - 64))                            \
   | (1u << (RANG_SECTOR_VPERED_MTZN1 - 64))                   \
   | (1u << (RANG_SECTOR_NAZAD_MTZN1 - 64))                    \
   | (1u << (RANG_PO_MTZ1 - 64))                               \
   | (1u << (RANG_PO_MTZN1_VPERED - 64))                       \
   | (1u << (RANG_PO_MTZN1_NAZAD - 64))                        \
   | (1u << (RANG_PO_U_MTZPN1 - 64))                           \
   | (1u << (RANG_PO_MTZPN1 - 64))                             \
   | (1u << (RANG_MTZ1 - 64))                                  \
   | (1u << (RANG_SECTOR_VPERED_MTZN2 - 64))                   \
   | (1u << (RANG_SECTOR_NAZAD_MTZN2 - 64))                    \
   | (1u << (RANG_PO_MTZ2 - 64))                               \
   | (1u << (RANG_PO_MTZN2_VPERED - 64))                       \
   | (1u << (RANG_PO_MTZN2_NAZAD - 64))                        \
   | (1u << (RANG_PO_U_MTZPN2 - 64))                           \
   | (1u << (RANG_PO_MTZPN2 - 64))                             \
   | (1u << (RANG_MTZ2 - 64))                                  \
   | (1u << (RANG_SECTOR_VPERED_MTZN3 - 64))                   \
   | (1u << (RANG_SECTOR_NAZAD_MTZN3 - 64))                    \
   | (1u << (RANG_PO_MTZ3 - 64))                               \
   | (1u << (RANG_PO_MTZN3_VPERED - 64))                       \
   | (1u << (RANG_PO_MTZN3_NAZAD - 64))                        \
   | (1u << (RANG_PO_U_MTZPN3 - 64))                           \
   | (1u << (RANG_PO_MTZPN3 - 64))                             \
   | (1u << (RANG_MTZ3 - 64))                                  \
   | (1u << (RANG_SECTOR_VPERED_MTZN4 - 64))                   \
   | (1u << (RANG_SECTOR_NAZAD_MTZN4 - 64))                    \
   | (1u << (RANG_PO_MTZ4 - 64))                               \
   | (1u << (RANG_PO_MTZN4_VPERED - 64))                       \
   | (1u << (RANG_PO_MTZN4_NAZAD - 64))                        \
   | (1u << (RANG_PO_U_MTZPN4 - 64))                           \
   | (1u << (RANG_PO_MTZPN4 - 64))                             \
)

#define MASKA_MTZ_SIGNALS_3 (                                  \
     (1u << (RANG_MTZ4 - 96))                                  \
   | (1u << (RANG_PO_BLOCK_U_MTZN - 96))                       \
   | (1u << (RANG_NCN_MTZ - 96))                               \
)

#define MASKA_MTZ_SIGNALS_4                  0
#define MASKA_MTZ_SIGNALS_5                  0
#define MASKA_MTZ_SIGNALS_6                  0
#define MASKA_MTZ_SIGNALS_7                  0
#define MASKA_MTZ_SIGNALS_8                  0
#define MASKA_MTZ_SIGNALS_9                  0
/*****************************************/
     
/*****************************************/
//Макски всіх сигналів МСЗ 0.4кВ
/*****************************************/
#define MASKA_MTZ04_SIGNALS_0                0
#define MASKA_MTZ04_SIGNALS_1                0
#define MASKA_MTZ04_SIGNALS_2                0

#define MASKA_MTZ04_SIGNALS_3 (                                \
     (1u << (RANG_BLOCK_MTZ04_1 - 96))                         \
   | (1u << (RANG_BLOCK_MTZ04_2 - 96))                         \
   | (1u << (RANG_BLOCK_USK_MTZ04_2 - 96))                     \
   | (1u << (RANG_PO_MTZ04_1 - 96))                            \
   | (1u << (RANG_MTZ04_1 - 96))                               \
   | (1u << (RANG_PO_MTZ04_2 - 96))                            \
   | (1u << (RANG_MTZ04_2 - 96))                               \
)

#define MASKA_MTZ04_SIGNALS_4                0
#define MASKA_MTZ04_SIGNALS_5                0
#define MASKA_MTZ04_SIGNALS_6                0
#define MASKA_MTZ04_SIGNALS_7                0
#define MASKA_MTZ04_SIGNALS_8                0
#define MASKA_MTZ04_SIGNALS_9                0
/*****************************************/
     
/*****************************************/
//Макски всіх сигналів ЗНам
/*****************************************/
#define MASKA_ZNAM_SIGNALS_0                0
#define MASKA_ZNAM_SIGNALS_1                0
#define MASKA_ZNAM_SIGNALS_2                0

#define MASKA_ZNAM_SIGNALS_3 (                            \
     (1u << (RANG_BLOCK_ZNAM - 96))                       \
   | (1u << (RANG_PO_ZNAM - 96))                          \
   | (1u << (RANG_ZNAM - 96))                             \
)

#define MASKA_ZNAM_SIGNALS_4                0
#define MASKA_ZNAM_SIGNALS_5                0
#define MASKA_ZNAM_SIGNALS_6                0
#define MASKA_ZNAM_SIGNALS_7                0
#define MASKA_ZNAM_SIGNALS_8                0
#define MASKA_ZNAM_SIGNALS_9                0
/*****************************************/
     
/*****************************************/
//Макски всіх сигналів ЗДЗ
/*****************************************/
#define MASKA_ZDZ_SIGNALS_0                  0
#define MASKA_ZDZ_SIGNALS_1                  0
#define MASKA_ZDZ_SIGNALS_2                  0

#define MASKA_ZDZ_SIGNALS_3 (                                  \
     (1u << (RANG_BLOCK_ZDZ - 96))                             \
   | (1u << (RANG_PUSK_ZDZ_VID_DV - 96))                       \
   | (1u << (RANG_LIGHT_ZDZ_FROM_DV - 96))                     \
   | (1u << (RANG_PO_ZDZ - 96))                                \
   | (1u << (RANG_ZDZ - 96))                                   \
)

#define MASKA_ZDZ_SIGNALS_4                  0
#define MASKA_ZDZ_SIGNALS_5                  0
#define MASKA_ZDZ_SIGNALS_6                  0
#define MASKA_ZDZ_SIGNALS_7                  0
#define MASKA_ZDZ_SIGNALS_8                  0
#define MASKA_ZDZ_SIGNALS_9                  0
/*****************************************/
     
/*****************************************/
//Макски всіх сигналів СЗЗ
/*****************************************/
#define MASKA_NZZ_SIGNALS_0                  0
#define MASKA_NZZ_SIGNALS_1                  0
#define MASKA_NZZ_SIGNALS_2                  0

#define MASKA_NZZ_SIGNALS_3 (                                  \
     (1u << (RANG_BLOCK_NZZ - 96))                             \
   | (1u << (RANG_PO_NZZ - 96))                                \
   | (1u << (RANG_NZZ - 96))                                   \
   | (1u << (RANG_PO_3I0 - 96))                                \
   | (1u << (RANG_3I0 - 96))                                   \
   | (1u << (RANG_PO_3U0 - 96))                                \
   | (1u << (RANG_3U0 - 96))                                   \
   | (1u << (RANG_SECTOR_NZZ - 96))                            \
)

#define MASKA_NZZ_SIGNALS_4                  0
#define MASKA_NZZ_SIGNALS_5                  0
#define MASKA_NZZ_SIGNALS_6                  0
#define MASKA_NZZ_SIGNALS_7                  0
#define MASKA_NZZ_SIGNALS_8                  0
#define MASKA_NZZ_SIGNALS_9                  0
/*****************************************/
     
/*****************************************/
//Макски всіх сигналів СЗНП
/*****************************************/
#define MASKA_TZNP_SIGNALS_0                 0
#define MASKA_TZNP_SIGNALS_1                 0
#define MASKA_TZNP_SIGNALS_2                 0
     
#define MASKA_TZNP_SIGNALS_3 (                                 \
     (1u << (RANG_BLOCK_TZNP1 - 96))                           \
   | (1u << (RANG_SECTOR_TZNP1_VPERED - 96))                   \
   | (1u << (RANG_SECTOR_TZNP1_NAZAD - 96))                    \
   | (1u << (RANG_PO_3I0_TZNP1_VPERED - 96))                   \
   | (1u << (RANG_PO_3I0_TZNP1_NAZAD - 96))                    \
   | (1u << (RANG_PO_3U0_TZNP1_VPERED - 96))                   \
)

#define MASKA_TZNP_SIGNALS_4 (                                  \
     (1u << (RANG_PO_3U0_TZNP1_NAZAD - 128))                    \
   | (1u << (RANG_PO_TZNP1_VPERED - 128))                       \
   | (1u << (RANG_PO_TZNP1_NAZAD - 128))                        \
   | (1u << (RANG_TZNP1 - 128))                                 \
   | (1u << (RANG_BLOCK_TZNP2 - 128))                           \
   | (1u << (RANG_SECTOR_TZNP2_VPERED - 128))                   \
   | (1u << (RANG_SECTOR_TZNP2_NAZAD - 128))                    \
   | (1u << (RANG_PO_3I0_TZNP2_VPERED - 128))                   \
   | (1u << (RANG_PO_3I0_TZNP2_NAZAD - 128))                    \
   | (1u << (RANG_PO_3U0_TZNP2_VPERED - 128))                   \
   | (1u << (RANG_PO_3U0_TZNP2_NAZAD - 128))                    \
   | (1u << (RANG_PO_TZNP2_VPERED - 128))                       \
   | (1u << (RANG_PO_TZNP2_NAZAD - 128))                        \
   | (1u << (RANG_TZNP2 - 128))                                 \
   | (1u << (RANG_BLOCK_TZNP3 - 128))                           \
   | (1u << (RANG_SECTOR_TZNP3_VPERED - 128))                   \
   | (1u << (RANG_SECTOR_TZNP3_NAZAD - 128))                    \
   | (1u << (RANG_PO_3I0_TZNP3_VPERED - 128))                   \
   | (1u << (RANG_PO_3I0_TZNP3_NAZAD - 128))                    \
   | (1u << (RANG_PO_3U0_TZNP3_VPERED - 128))                   \
   | (1u << (RANG_PO_3U0_TZNP3_NAZAD - 128))                    \
   | (1u << (RANG_PO_TZNP3_VPERED - 128))                       \
   | (1u << (RANG_PO_TZNP3_NAZAD - 128))                        \
   | (1u << (RANG_TZNP3 - 128))                                 \
)

#define MASKA_TZNP_SIGNALS_5                 0
#define MASKA_TZNP_SIGNALS_6                 0
#define MASKA_TZNP_SIGNALS_7                 0
#define MASKA_TZNP_SIGNALS_8                 0
#define MASKA_TZNP_SIGNALS_9                 0
/*****************************************/
     
/*****************************************/
//Макски всіх сигналів АПВ
/*****************************************/
#define MASKA_APV_SIGNALS_0                  0
#define MASKA_APV_SIGNALS_1                  0
#define MASKA_APV_SIGNALS_2                  0
#define MASKA_APV_SIGNALS_3                  0
     
#define MASKA_APV_SIGNALS_4 (                                  \
     (1u << (RANG_STAT_BLK_APV - 128))                         \
   | (1u << (RANG_APV1 - 128))                                 \
   | (1u << (RANG_APV2 - 128))                                 \
   | (1u << (RANG_APV3 - 128))                                 \
   | (1u << (RANG_APV4 - 128))                                 \
   | (1u << (RANG_APV_WORK - 128))                             \
)

#define MASKA_APV_SIGNALS_5                  0
#define MASKA_APV_SIGNALS_6                  0
#define MASKA_APV_SIGNALS_7                  0
#define MASKA_APV_SIGNALS_8                  0
#define MASKA_APV_SIGNALS_9                  0
/*****************************************/

/*****************************************/
//Макски всіх сигналів АЧР/ЧАПВ
/*****************************************/
#define MASKA_ACHR_CHAPV_SIGNALS_0           0
#define MASKA_ACHR_CHAPV_SIGNALS_1           0
#define MASKA_ACHR_CHAPV_SIGNALS_2           0
#define MASKA_ACHR_CHAPV_SIGNALS_3           0

#define MASKA_ACHR_CHAPV_SIGNALS_4 (                           \
     (1u << (RANG_ACHR_CHAPV_VID_DV - 128))                    \
   | (1u << (RANG_BLOCK_ACHR1 - 128))                          \
)

#define MASKA_ACHR_CHAPV_SIGNALS_5 (                           \
     (1u << (RANG_BLOCK_ACHR2 - 160))                          \
   | (1u << (RANG_RAZR_CHAPV - 160))                           \
   | (1u << (RANG_VN_BLOCK_CHAPV - 160))                       \
   | (1u << (RANG_PO_ACHR1 - 160))                             \
   | (1u << (RANG_PO_CHAPV1 - 160))                            \
   | (1u << (RANG_ACHR_CHAPV1 - 160))                          \
   | (1u << (RANG_PO_ACHR2 - 160))                             \
   | (1u << (RANG_PO_CHAPV2 - 160))                            \
   | (1u << (RANG_ACHR_CHAPV2 - 160))                          \
)

#define MASKA_ACHR_CHAPV_SIGNALS_6           0
#define MASKA_ACHR_CHAPV_SIGNALS_7           0
#define MASKA_ACHR_CHAPV_SIGNALS_8           0
#define MASKA_ACHR_CHAPV_SIGNALS_9           0
/*****************************************/
     
/*****************************************/
//Макски всіх сигналів ПРВВ
/*****************************************/
#define MASKA_UROV_SIGNALS_0                 0
#define MASKA_UROV_SIGNALS_1                 0
#define MASKA_UROV_SIGNALS_2                 0
#define MASKA_UROV_SIGNALS_3                 0
#define MASKA_UROV_SIGNALS_4                 0

#define MASKA_UROV_SIGNALS_5 (                                 \
     (1u << (RANG_BLOCK_UROV - 160))                      	   \
   | (1u << (RANG_PUSK_UROV_VID_DV - 160))                     \
   | (1u << (RANG_PO_UROV - 160))                              \
   | (1u << (RANG_UROV1 - 160))                                \
   | (1u << (RANG_UROV2 - 160))                                \
)

#define MASKA_UROV_SIGNALS_6                 0
#define MASKA_UROV_SIGNALS_7                 0
#define MASKA_UROV_SIGNALS_8                 0
#define MASKA_UROV_SIGNALS_9                 0
/*****************************************/

/*****************************************/
//Макски всіх сигналів ЗЗП
/*****************************************/
#define MASKA_ZOP_SIGNALS_0                  0
#define MASKA_ZOP_SIGNALS_1                  0
#define MASKA_ZOP_SIGNALS_2                  0
#define MASKA_ZOP_SIGNALS_3                  0
#define MASKA_ZOP_SIGNALS_4                  0

#define MASKA_ZOP_SIGNALS_5 (                                  \
     (1u << (RANG_BLOCK_ZOP - 160))                            \
   | (1u << (RANG_PO_ZOP - 160))                               \
   | (1u << (RANG_ZOP - 160))                                  \
)

#define MASKA_ZOP_SIGNALS_6                  0
#define MASKA_ZOP_SIGNALS_7                  0
#define MASKA_ZOP_SIGNALS_8                  0
#define MASKA_ZOP_SIGNALS_9                  0
/*****************************************/

/*****************************************/
//Макски всіх сигналів ЗНмін
/*****************************************/
#define MASKA_UMIN_SIGNALS_0                 0
#define MASKA_UMIN_SIGNALS_1                 0
#define MASKA_UMIN_SIGNALS_2                 0
#define MASKA_UMIN_SIGNALS_3                 0
#define MASKA_UMIN_SIGNALS_4                 0
     
#define MASKA_UMIN_SIGNALS_5 (                                 \
     (1u << (RANG_BLOCK_UMIN1 - 160))                          \
   | (1u << (RANG_START_UMIN1 - 160))                          \
   | (1u << (RANG_BLOCK_UMIN2 - 160))                          \
   | (1u << (RANG_START_UMIN2 - 160))                          \
   | (1u << (RANG_PO_UMIN1 - 160))                             \
   | (1u << (RANG_PO_UBLK_UMIN1 - 160))                        \
   | (1u << (RANG_PO_IBLK_UMIN1 - 160))                        \
   | (1u << (RANG_UMIN1 - 160))                                \
   | (1u << (RANG_PO_UMIN2 - 160))                             \
   | (1u << (RANG_PO_UBLK_UMIN2 - 160))                        \
   | (1u << (RANG_PO_IBLK_UMIN2 - 160))                        \
   | (1u << (RANG_UMIN2 - 160))                                \
)

#define MASKA_UMIN_SIGNALS_6                 0
#define MASKA_UMIN_SIGNALS_7                 0
#define MASKA_UMIN_SIGNALS_8                 0
#define MASKA_UMIN_SIGNALS_9                 0
/*****************************************/

/*****************************************/
//Макски всіх сигналів ЗНмакс
/*****************************************/
#define MASKA_UMAX_SIGNALS_0                 0
#define MASKA_UMAX_SIGNALS_1                 0
#define MASKA_UMAX_SIGNALS_2                 0
#define MASKA_UMAX_SIGNALS_3                 0
#define MASKA_UMAX_SIGNALS_4                 0

#define MASKA_UMAX_SIGNALS_5 (                                 \
     (1u << (RANG_BLOCK_UMAX1 - 160))                          \
   | (1u << (RANG_PO_UMAX1 - 160))                             \
   | (1u << (RANG_UMAX1 - 160))                                \
)

#define MASKA_UMAX_SIGNALS_6 (                                 \
     (1u << (RANG_BLOCK_UMAX2 - 192))                          \
   | (1u << (RANG_PO_UMAX2 - 192))                             \
   | (1u << (RANG_UMAX2 - 192))                                \
)

#define MASKA_UMAX_SIGNALS_7                 0
#define MASKA_UMAX_SIGNALS_8                 0
#define MASKA_UMAX_SIGNALS_9                 0
/*****************************************/

/*****************************************/
//Макски всіх сигналів Універсального Захисту
/*****************************************/
#define MASKA_UP_SIGNALS_0                 0
#define MASKA_UP_SIGNALS_1                 0
#define MASKA_UP_SIGNALS_2                 0
#define MASKA_UP_SIGNALS_3                 0
#define MASKA_UP_SIGNALS_4                 0
#define MASKA_UP_SIGNALS_5                 0

#define MASKA_UP_SIGNALS_6 (                                 \
     (1u << (RANG_BLOCK_UP1 + 3*0 - 192))                    \
   | (1u << (RANG_PO_UP1 + 3*0 - 192))                       \
   | (1u << (RANG_UP1 + 3*0 - 192))                          \
   | (1u << (RANG_BLOCK_UP1 + 3*1 - 192))                    \
   | (1u << (RANG_PO_UP1 + 3*1 - 192))                       \
   | (1u << (RANG_UP1 + 3*1 - 192))                          \
   | (1u << (RANG_BLOCK_UP1 + 3*2 - 192))                    \
   | (1u << (RANG_PO_UP1 + 3*2 - 192))                       \
   | (1u << (RANG_UP1 + 3*2 - 192))                          \
   | (1u << (RANG_BLOCK_UP1 + 3*3 - 192))                    \
   | (1u << (RANG_PO_UP1 + 3*3 - 192))                       \
   | (1u << (RANG_UP1 + 3*3 - 192))                          \
   | (1u << (RANG_BLOCK_UP1 + 3*4 - 192))                    \
   | (1u << (RANG_PO_UP1 + 3*4 - 192))                       \
   | (1u << (RANG_UP1 + 3*4 - 192))                          \
   | (1u << (RANG_BLOCK_UP1 + 3*5 - 192))                    \
   | (1u << (RANG_PO_UP1 + 3*5 - 192))                       \
   | (1u << (RANG_UP1 + 3*5 - 192))                          \
   | (1u << (RANG_BLOCK_UP1 + 3*6 - 192))                    \
   | (1u << (RANG_PO_UP1 + 3*6 - 192))                       \
   | (1u << (RANG_UP1 + 3*6 - 192))                          \
   | (1u << (RANG_BLOCK_UP1 + 3*7 - 192))                    \
   | (1u << (RANG_PO_UP1 + 3*7 - 192))                       \
   | (1u << (RANG_UP1 + 3*7 - 192))                          \
)

#define MASKA_UP_SIGNALS_7                 0
#define MASKA_UP_SIGNALS_8                 0
#define MASKA_UP_SIGNALS_9                 0
/*****************************************/

/*****************************************/
//Макски всіх сигналів розширеної логіки
/*****************************************/
#define MASKA_EL_SIGNALS_0                  0
#define MASKA_EL_SIGNALS_1                  0
#define MASKA_EL_SIGNALS_2                  0
#define MASKA_EL_SIGNALS_3                  0
#define MASKA_EL_SIGNALS_4                  0
#define MASKA_EL_SIGNALS_5                  0

#define MASKA_EL_SIGNALS_6 (                                   \
    (1u << (RANG_DF1_IN - 192))                                \
  | (1u << (RANG_DF1_R - 192))                                 \
  | (1u << (RANG_DF1_OUT - 192))                               \
  | (1u << (RANG_DF2_IN - 192))                                \
  | (1u << (RANG_DF2_R - 192))                                 \
)

#define MASKA_EL_SIGNALS_7 (                                   \
    (1u << (RANG_DF2_OUT - 224))                               \
  | (1u << (RANG_DF3_IN - 224))                                \
  | (1u << (RANG_DF3_R - 224))                                 \
  | (1u << (RANG_DF3_OUT - 224))                               \
  | (1u << (RANG_DF4_IN - 224))                                \
  | (1u << (RANG_DF4_R - 224))                                 \
  | (1u << (RANG_DF4_OUT - 224))                               \
  | (1u << (RANG_DF5_IN - 224))                                \
  | (1u << (RANG_DF5_R - 224))                                 \
  | (1u << (RANG_DF5_OUT - 224))                               \
  | (1u << (RANG_DF6_IN - 224))                                \
  | (1u << (RANG_DF6_R - 224))                                 \
  | (1u << (RANG_DF6_OUT - 224))                               \
  | (1u << (RANG_DF7_IN - 224))                                \
  | (1u << (RANG_DF7_R - 224))                                 \
  | (1u << (RANG_DF7_OUT - 224))                               \
  | (1u << (RANG_DF8_IN - 224))                                \
  | (1u << (RANG_DF8_R - 224))                                 \
  | (1u << (RANG_DF8_OUT - 224))                               \
  | (1u << (RANG_DT1_SET - 224))                               \
  | (1u << (RANG_DT1_RESET - 224))                             \
  | (1u << (RANG_DT1_OUT - 224))                               \
  | (1u << (RANG_DT2_SET - 224))                               \
  | (1u << (RANG_DT2_RESET - 224))                             \
  | (1u << (RANG_DT2_OUT - 224))                               \
  | (1u << (RANG_DT3_SET - 224))                               \
  | (1u << (RANG_DT3_RESET - 224))                             \
  | (1u << (RANG_DT3_OUT - 224))                               \
  | (1u << (RANG_DT4_SET - 224))                               \
  | (1u << (RANG_DT4_RESET - 224))                             \
  | (1u << (RANG_DT4_OUT - 224))                               \
  | (1u << (RANG_D_AND1 - 224))                                \
)

#define MASKA_EL_SIGNALS_8 (                                   \
    (1u << (RANG_D_AND2 - 256))                                \
  | (1u << (RANG_D_AND3 - 256))                                \
  | (1u << (RANG_D_AND4 - 256))                                \
  | (1u << (RANG_D_AND5 - 256))                                \
  | (1u << (RANG_D_AND6 - 256))                                \
  | (1u << (RANG_D_AND7 - 256))                                \
  | (1u << (RANG_D_AND8 - 256))                                \
  | (1u << (RANG_D_OR1 - 256))                                 \
  | (1u << (RANG_D_OR2 - 256))                                 \
  | (1u << (RANG_D_OR3 - 256))                                 \
  | (1u << (RANG_D_OR4 - 256))                                 \
  | (1u << (RANG_D_OR5 - 256))                                 \
  | (1u << (RANG_D_OR6 - 256))                                 \
  | (1u << (RANG_D_OR7 - 256))                                 \
  | (1u << (RANG_D_OR8 - 256))                                 \
  | (1u << (RANG_D_XOR1 - 256))                                \
  | (1u << (RANG_D_XOR2 - 256))                                \
  | (1u << (RANG_D_XOR3 - 256))                                \
  | (1u << (RANG_D_XOR4 - 256))                                \
  | (1u << (RANG_D_XOR5 - 256))                                \
  | (1u << (RANG_D_XOR6 - 256))                                \
  | (1u << (RANG_D_XOR7 - 256))                                \
  | (1u << (RANG_D_XOR8 - 256))                                \
  | (1u << (RANG_D_NOT1 - 256))                                \
  | (1u << (RANG_D_NOT2 - 256))                                \
  | (1u << (RANG_D_NOT3 - 256))                                \
  | (1u << (RANG_D_NOT4 - 256))                                \
  | (1u << (RANG_D_NOT5 - 256))                                \
  | (1u << (RANG_D_NOT6 - 256))                                \
  | (1u << (RANG_D_NOT7 - 256))                                \
  | (1u << (RANG_D_NOT8 - 256))                                \
  | (1u << (RANG_D_NOT9 - 256))                                \
)


#define MASKA_EL_SIGNALS_9 (                                   \
    (1u << (RANG_D_NOT10 - 288))                               \
  | (1u << (RANG_D_NOT11 - 288))                               \
  | (1u << (RANG_D_NOT12 - 288))                               \
  | (1u << (RANG_D_NOT13 - 288))                               \
  | (1u << (RANG_D_NOT14 - 288))                               \
  | (1u << (RANG_D_NOT15 - 288))                               \
  | (1u << (RANG_D_NOT16 - 288))                               \
  | (1u << (RANG_LF1 - 288))                                   \
  | (1u << (RANG_LF2 - 288))                                   \
  | (1u << (RANG_LF3 - 288))                                   \
  | (1u << (RANG_LF4 - 288))                                   \
  | (1u << (RANG_LF5 - 288))                                   \
  | (1u << (RANG_LF6 - 288))                                   \
  | (1u << (RANG_LF7 - 288))                                   \
  | (1u << (RANG_LF8 - 288))                                   \
  | (1u << (RANG_LF9 - 288))                                   \
  | (1u << (RANG_LF10 - 288))                                  \
  | (1u << (RANG_LF11 - 288))                                  \
  | (1u << (RANG_LF12 - 288))                                  \
  | (1u << (RANG_LF13 - 288))                                  \
  | (1u << (RANG_LF14 - 288))                                  \
  | (1u << (RANG_LF15 - 288))                                  \
  | (1u << (RANG_LF16 - 288))                                  \
  | (1u << (RANG_ERROR_CONF_EL - 288))                         \
)
/*****************************************/


/*****************************************/
//Макска сигналів, які мають записуватися у енергонезалежну пам'ять
/*****************************************/
#define MASKA_TRIGGER_SIGNALES_0                  0
#define MASKA_TRIGGER_SIGNALES_1                  0
#define MASKA_TRIGGER_SIGNALES_2                  0
#define MASKA_TRIGGER_SIGNALES_3                  0
#define MASKA_TRIGGER_SIGNALES_4                  0
#define MASKA_TRIGGER_SIGNALES_5                  0
#define MASKA_TRIGGER_SIGNALES_6                  0

#define MASKA_TRIGGER_SIGNALES_7 (                             \
     (1u << (RANG_DT1_OUT - 224))                              \
   | (1u << (RANG_DT2_OUT - 224))                              \
   | (1u << (RANG_DT3_OUT - 224))                              \
   | (1u << (RANG_DT4_OUT - 224))                              \
)     

#define MASKA_TRIGGER_SIGNALES_8                  0
#define MASKA_TRIGGER_SIGNALES_9                  0
/*****************************************/

/*****************************************/
//Макска для блокування зміни груп  уставок
/*****************************************/
#define MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_0 (               \
     (1u << (RANG_WORK_BO - 0))                                \
   | (1u << (RANG_WORK_BV - 0))                                \
)

#define MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_1    0

#define MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_2 (               \
     (1u << (RANG_PO_MTZ1 - 64))                               \
   | (1u << (RANG_PO_MTZN1_VPERED - 64))                       \
   | (1u << (RANG_PO_MTZN1_NAZAD - 64))                        \
   | (1u << (RANG_PO_MTZPN1 - 64))                             \
   | (1u << (RANG_MTZ1 - 64))                                  \
   | (1u << (RANG_PO_MTZ2 - 64))                               \
   | (1u << (RANG_PO_MTZN2_VPERED - 64))                       \
   | (1u << (RANG_PO_MTZN2_NAZAD - 64))                        \
   | (1u << (RANG_PO_MTZPN2 - 64))                             \
   | (1u << (RANG_MTZ2 - 64))                                  \
   | (1u << (RANG_PO_MTZ3 - 64))                               \
   | (1u << (RANG_PO_MTZN3_VPERED - 64))                       \
   | (1u << (RANG_PO_MTZN3_NAZAD - 64))                        \
   | (1u << (RANG_PO_MTZPN3 - 64))                             \
   | (1u << (RANG_MTZ3 - 64))                                  \
   | (1u << (RANG_PO_MTZ4 - 64))                               \
   | (1u << (RANG_PO_MTZN4_VPERED - 64))                       \
   | (1u << (RANG_PO_MTZN4_NAZAD - 64))                        \
   | (1u << (RANG_PO_MTZPN4 - 64))                             \
)

#define MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_3 (               \
     (1u << (RANG_MTZ4 - 96))                                  \
   | (1u << (RANG_PO_MTZ04_1 - 96))                            \
   | (1u << (RANG_MTZ04_1 - 96))                               \
   | (1u << (RANG_PO_MTZ04_2 - 96))                            \
   | (1u << (RANG_MTZ04_2 - 96))                               \
   | (1u << (RANG_PO_ZNAM - 96))                               \
   | (1u << (RANG_ZNAM - 96))                                  \
   | (1u << (RANG_PO_ZDZ - 96))                                \
   | (1u << (RANG_ZDZ - 96))                                   \
   | (1u << (RANG_PO_NZZ - 96))                                \
   | (1u << (RANG_NZZ - 96))                                   \
   | (1u << (RANG_PO_3I0 - 96))                                \
   | (1u << (RANG_3I0 - 96))                                   \
   | (1u << (RANG_PO_3U0 - 96))                                \
   | (1u << (RANG_3U0 - 96))                                   \
)

#define MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_4 (               \
     (1u << (RANG_PO_TZNP1_VPERED - 128))                      \
   | (1u << (RANG_PO_TZNP1_NAZAD - 128))                       \
   | (1u << (RANG_TZNP1 - 128))                                \
   | (1u << (RANG_PO_TZNP2_VPERED - 128))                      \
   | (1u << (RANG_PO_TZNP2_NAZAD - 128))                       \
   | (1u << (RANG_TZNP2 - 128))                                \
   | (1u << (RANG_PO_TZNP3_VPERED - 128))                      \
   | (1u << (RANG_PO_TZNP3_NAZAD - 128))                       \
   | (1u << (RANG_TZNP3 - 128))                                \
   | (1u << (RANG_APV_WORK - 128))                             \
)

#define MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_5 (               \
     (1u << (RANG_PO_ACHR1 - 160))                             \
   | (1u << (RANG_ACHR_CHAPV1 - 160))                          \
   | (1u << (RANG_PO_ACHR2 - 160))                             \
   | (1u << (RANG_ACHR_CHAPV2 - 160))                          \
   | (1u << (RANG_PO_UROV - 160))                              \
   | (1u << (RANG_UROV1 - 160))                                \
   | (1u << (RANG_UROV2 - 160))                                \
   | (1u << (RANG_PO_ZOP - 160))                               \
   | (1u << (RANG_ZOP - 160))                                  \
   | (1u << (RANG_PO_UMIN1 - 160))                             \
   | (1u << (RANG_UMIN1 - 160))                                \
   | (1u << (RANG_PO_UMIN2 - 160))                             \
   | (1u << (RANG_UMIN2 - 160))                                \
   | (1u << (RANG_PO_UMAX1 - 160))                             \
   | (1u << (RANG_UMAX1 - 160))                                \
)

#define MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_6 (               \
     (1u << (RANG_PO_UMAX2 - 192))                             \
   | (1u << (RANG_UMAX2 - 192))                                \
   | (1u << (RANG_PO_UP1 + 3*0 - 192))                         \
   | (1u << (RANG_UP1 + 3*0 - 192))                            \
   | (1u << (RANG_PO_UP1 + 3*1 - 192))                         \
   | (1u << (RANG_UP1 + 3*1 - 192))                            \
   | (1u << (RANG_PO_UP1 + 3*2 - 192))                         \
   | (1u << (RANG_UP1 + 3*2 - 192))                            \
   | (1u << (RANG_PO_UP1 + 3*3 - 192))                         \
   | (1u << (RANG_UP1 + 3*3 - 192))                            \
   | (1u << (RANG_PO_UP1 + 3*4 - 192))                         \
   | (1u << (RANG_UP1 + 3*4 - 192))                            \
   | (1u << (RANG_PO_UP1 + 3*5 - 192))                         \
   | (1u << (RANG_UP1 + 3*5 - 192))                            \
   | (1u << (RANG_PO_UP1 + 3*6 - 192))                         \
   | (1u << (RANG_UP1 + 3*6 - 192))                            \
   | (1u << (RANG_PO_UP1 + 3*7 - 192))                         \
   | (1u << (RANG_UP1 + 3*7 - 192))                            \
)

#define MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_7  0
#define MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_8  0
#define MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_9  0
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
)     

#define MASKA_INFO_SIGNALES_1 (                                \
     (1u << (RANG_SETTINGS_CHANGED - 32))                      \
)     

#define MASKA_INFO_SIGNALES_2                  0
#define MASKA_INFO_SIGNALES_3                  0
#define MASKA_INFO_SIGNALES_4                  0
#define MASKA_INFO_SIGNALES_5                  0
#define MASKA_INFO_SIGNALES_6                  0
#define MASKA_INFO_SIGNALES_7                  0
#define MASKA_INFO_SIGNALES_8                  0
#define MASKA_INFO_SIGNALES_9                  0
/*****************************************/

/*****************************************/
//Макска моніторингу максимального фазного струму для дискретного реєстратора
/*****************************************/
#define MASKA_MONITOTYNG_PHASE_SIGNALES_0        0
#define MASKA_MONITOTYNG_PHASE_SIGNALES_1        0

#define MASKA_MONITOTYNG_PHASE_SIGNALES_2 (                    \
     (1u << (RANG_PO_MTZ1 - 64))                               \
   | (1u << (RANG_PO_MTZN1_VPERED - 64))                       \
   | (1u << (RANG_PO_MTZN1_NAZAD - 64))                        \
   | (1u << (RANG_PO_MTZPN1 - 64))                             \
   | (1u << (RANG_MTZ1 - 64))                                  \
   | (1u << (RANG_PO_MTZ2 - 64))                               \
   | (1u << (RANG_PO_MTZN2_VPERED - 64))                       \
   | (1u << (RANG_PO_MTZN2_NAZAD - 64))                        \
   | (1u << (RANG_PO_MTZPN2 - 64))                             \
   | (1u << (RANG_MTZ2 - 64))                                  \
   | (1u << (RANG_PO_MTZ3 - 64))                               \
   | (1u << (RANG_PO_MTZN3_VPERED - 64))                       \
   | (1u << (RANG_PO_MTZN3_NAZAD - 64))                        \
   | (1u << (RANG_PO_MTZPN3 - 64))                             \
   | (1u << (RANG_MTZ3 - 64))                                  \
   | (1u << (RANG_PO_MTZ4 - 64))                               \
   | (1u << (RANG_PO_MTZN4_VPERED - 64))                       \
   | (1u << (RANG_PO_MTZN4_NAZAD - 64))                        \
   | (1u << (RANG_PO_MTZPN4 - 64))                             \
)

#define MASKA_MONITOTYNG_PHASE_SIGNALES_3 (                    \
     (1u << (RANG_MTZ4 - 96))                                  \
)

#define MASKA_MONITOTYNG_PHASE_SIGNALES_4        0
#define MASKA_MONITOTYNG_PHASE_SIGNALES_5        0
#define MASKA_MONITOTYNG_PHASE_SIGNALES_6        0
#define MASKA_MONITOTYNG_PHASE_SIGNALES_7        0
#define MASKA_MONITOTYNG_PHASE_SIGNALES_8        0
#define MASKA_MONITOTYNG_PHASE_SIGNALES_9        0
/*****************************************/

/*****************************************/
//Макска моніторингу максимального фазного струму сторони 0.4кВ для дискретного реєстратора
/*****************************************/
#define MASKA_MONITOTYNG_PHASE04_SIGNALES_0        0
#define MASKA_MONITOTYNG_PHASE04_SIGNALES_1        0
#define MASKA_MONITOTYNG_PHASE04_SIGNALES_2        0

#define MASKA_MONITOTYNG_PHASE04_SIGNALES_3 (                  \
     (1u << (RANG_PO_MTZ04_1 - 96))                            \
   | (1u << (RANG_MTZ04_1 - 96))                               \
   | (1u << (RANG_PO_MTZ04_2 - 96))                            \
   | (1u << (RANG_MTZ04_2 - 96))                               \
)

#define MASKA_MONITOTYNG_PHASE04_SIGNALES_4        0
#define MASKA_MONITOTYNG_PHASE04_SIGNALES_5        0
#define MASKA_MONITOTYNG_PHASE04_SIGNALES_6        0
#define MASKA_MONITOTYNG_PHASE04_SIGNALES_7        0
#define MASKA_MONITOTYNG_PHASE04_SIGNALES_8        0
#define MASKA_MONITOTYNG_PHASE04_SIGNALES_9        0
/*****************************************/

/*****************************************/
//Макска моніторингу відношення другої гармоніки до першої гармоніки фазного струму
/*****************************************/
#define MASKA_MONITOTYNG_ZNAM_SIGNALES_0        0
#define MASKA_MONITOTYNG_ZNAM_SIGNALES_1        0
#define MASKA_MONITOTYNG_ZNAM_SIGNALES_2        0

#define MASKA_MONITOTYNG_ZNAM_SIGNALES_3 (                  \
     (1u << (RANG_PO_ZNAM - 96))                            \
   | (1u << (RANG_ZNAM - 96))                               \
)

#define MASKA_MONITOTYNG_ZNAM_SIGNALES_4        0
#define MASKA_MONITOTYNG_ZNAM_SIGNALES_5        0
#define MASKA_MONITOTYNG_ZNAM_SIGNALES_6        0
#define MASKA_MONITOTYNG_ZNAM_SIGNALES_7        0
#define MASKA_MONITOTYNG_ZNAM_SIGNALES_8        0
#define MASKA_MONITOTYNG_ZNAM_SIGNALES_9        0
/*****************************************/

/*****************************************/
//Макска моніторингу максимального струму 3I0 1г
/*****************************************/
#define MASKA_MONITOTYNG_3I0_1G_SIGNALES_0     0
#define MASKA_MONITOTYNG_3I0_1G_SIGNALES_1     0
#define MASKA_MONITOTYNG_3I0_1G_SIGNALES_2     0

#define MASKA_MONITOTYNG_3I0_1G_SIGNALES_3 (                   \
     (1u << (RANG_PO_NZZ - 96))                                \
   | (1u << (RANG_NZZ - 96))                                   \
   | (1u << (RANG_PO_3I0 - 96))                                \
   | (1u << (RANG_3I0 - 96))                                   \
)    
#define MASKA_MONITOTYNG_3I0_1G_SIGNALES_4     0
#define MASKA_MONITOTYNG_3I0_1G_SIGNALES_5     0
#define MASKA_MONITOTYNG_3I0_1G_SIGNALES_6     0
#define MASKA_MONITOTYNG_3I0_1G_SIGNALES_7     0
#define MASKA_MONITOTYNG_3I0_1G_SIGNALES_8     0
#define MASKA_MONITOTYNG_3I0_1G_SIGNALES_9     0
/*****************************************/

/*****************************************/
//Макска моніторингу максимального струму 3I0 вищих гармонік
/*****************************************/
#define MASKA_MONITOTYNG_3I0_OTHERG_SIGNALES_0     0
#define MASKA_MONITOTYNG_3I0_OTHERG_SIGNALES_1     0
#define MASKA_MONITOTYNG_3I0_OTHERG_SIGNALES_2     0

#define MASKA_MONITOTYNG_3I0_OTHERG_SIGNALES_3 (               \
     (1u << (RANG_PO_3I0 - 96))                                \
   | (1u << (RANG_3I0 - 96))                                   \
)    
#define MASKA_MONITOTYNG_3I0_OTHERG_SIGNALES_4     0
#define MASKA_MONITOTYNG_3I0_OTHERG_SIGNALES_5     0
#define MASKA_MONITOTYNG_3I0_OTHERG_SIGNALES_6     0
#define MASKA_MONITOTYNG_3I0_OTHERG_SIGNALES_7     0
#define MASKA_MONITOTYNG_3I0_OTHERG_SIGNALES_8     0
#define MASKA_MONITOTYNG_3I0_OTHERG_SIGNALES_9     0
/*****************************************/

/*****************************************/
//Макска моніторингу максимальної напргуи 3I0 розрахунокве
/*****************************************/
#define MASKA_MONITOTYNG_3I0_R_SIGNALES_0        0
#define MASKA_MONITOTYNG_3I0_R_SIGNALES_1        0
#define MASKA_MONITOTYNG_3I0_R_SIGNALES_2        0
#define MASKA_MONITOTYNG_3I0_R_SIGNALES_3        0

#define MASKA_MONITOTYNG_3I0_R_SIGNALES_4 (                    \
     (1u << (RANG_PO_TZNP1_VPERED - 128))                      \
   | (1u << (RANG_PO_TZNP1_NAZAD - 128))                       \
   | (1u << (RANG_TZNP1 - 128))                                \
   | (1u << (RANG_PO_TZNP2_VPERED - 128))                      \
   | (1u << (RANG_PO_TZNP2_NAZAD - 128))                       \
   | (1u << (RANG_TZNP2 - 128))                                \
   | (1u << (RANG_PO_TZNP3_VPERED - 128))                      \
   | (1u << (RANG_PO_TZNP3_NAZAD - 128))                       \
   | (1u << (RANG_TZNP3 - 128))                                \
)            

#define MASKA_MONITOTYNG_3I0_R_SIGNALES_5        0
#define MASKA_MONITOTYNG_3I0_R_SIGNALES_6        0
#define MASKA_MONITOTYNG_3I0_R_SIGNALES_7        0
#define MASKA_MONITOTYNG_3I0_R_SIGNALES_8        0
#define MASKA_MONITOTYNG_3I0_R_SIGNALES_9        0
/*****************************************/

/*****************************************/
//Макска моніторингу максимальної напргуи 3U0 для дискретного реєстратора
/*****************************************/
#define MASKA_MONITOTYNG_3U0_SIGNALES_0        0
#define MASKA_MONITOTYNG_3U0_SIGNALES_1        0
#define MASKA_MONITOTYNG_3U0_SIGNALES_2        0

#define MASKA_MONITOTYNG_3U0_SIGNALES_3 (                      \
     (1u << (RANG_PO_NZZ - 96))                                \
   | (1u << (RANG_NZZ - 96))                                   \
   | (1u << (RANG_PO_3U0 - 96))                                \
   | (1u << (RANG_3U0 - 96))                                   \
)            

#define MASKA_MONITOTYNG_3U0_SIGNALES_4        0
#define MASKA_MONITOTYNG_3U0_SIGNALES_5        0
#define MASKA_MONITOTYNG_3U0_SIGNALES_6        0
#define MASKA_MONITOTYNG_3U0_SIGNALES_7        0
#define MASKA_MONITOTYNG_3U0_SIGNALES_8        0
#define MASKA_MONITOTYNG_3U0_SIGNALES_9        0
/*****************************************/

/*****************************************/
//Макска моніторингу Umin для дискретного реєстратора
/*****************************************/
#define MASKA_MONITOTYNG_UMIN_SIGNALES_0        0
#define MASKA_MONITOTYNG_UMIN_SIGNALES_1        0
#define MASKA_MONITOTYNG_UMIN_SIGNALES_2        0
#define MASKA_MONITOTYNG_UMIN_SIGNALES_3        0
#define MASKA_MONITOTYNG_UMIN_SIGNALES_4        0

#define MASKA_MONITOTYNG_UMIN_SIGNALES_5 (                     \
     (1u << (RANG_PO_UMIN1 - 160))                             \
   | (1u << (RANG_UMIN1 - 160))                                \
   | (1u << (RANG_PO_UMIN2 - 160))                             \
   | (1u << (RANG_UMIN2 - 160))                                \
)

#define MASKA_MONITOTYNG_UMIN_SIGNALES_6        0
#define MASKA_MONITOTYNG_UMIN_SIGNALES_7        0
#define MASKA_MONITOTYNG_UMIN_SIGNALES_8        0
#define MASKA_MONITOTYNG_UMIN_SIGNALES_9        0
/*****************************************/

/*****************************************/
//Макска моніторингу Umax для дискретного реєстратора
/*****************************************/
#define MASKA_MONITOTYNG_UMAX_SIGNALES_0        0
#define MASKA_MONITOTYNG_UMAX_SIGNALES_1        0
#define MASKA_MONITOTYNG_UMAX_SIGNALES_2        0
#define MASKA_MONITOTYNG_UMAX_SIGNALES_3        0
#define MASKA_MONITOTYNG_UMAX_SIGNALES_4        0

#define MASKA_MONITOTYNG_UMAX_SIGNALES_5 (                     \
     (1u << (RANG_PO_UMAX1 - 160))                             \
   | (1u << (RANG_UMAX1 - 160))                                \
)

#define MASKA_MONITOTYNG_UMAX_SIGNALES_6 (                     \
     (1u << (RANG_PO_UMAX2 - 192))                             \
   | (1u << (RANG_UMAX2 - 192))                                \
)

#define MASKA_MONITOTYNG_UMAX_SIGNALES_7        0
#define MASKA_MONITOTYNG_UMAX_SIGNALES_8        0
#define MASKA_MONITOTYNG_UMAX_SIGNALES_9        0
/*****************************************/

/*****************************************/
//Макска моніторингу максимального струму зворотньої послідовності для дискретного реєстратора
/*****************************************/
#define MASKA_MONITOTYNG_ZOP_SIGNALES_0                  0
#define MASKA_MONITOTYNG_ZOP_SIGNALES_1                  0
#define MASKA_MONITOTYNG_ZOP_SIGNALES_2                  0
#define MASKA_MONITOTYNG_ZOP_SIGNALES_3                  0
#define MASKA_MONITOTYNG_ZOP_SIGNALES_4                  0

#define MASKA_MONITOTYNG_ZOP_SIGNALES_5 (                      \
     (1u << (RANG_PO_ZOP - 160))                               \
   | (1u << (RANG_ZOP - 160))                                  \
)

#define MASKA_MONITOTYNG_ZOP_SIGNALES_6                  0
#define MASKA_MONITOTYNG_ZOP_SIGNALES_7                  0
#define MASKA_MONITOTYNG_ZOP_SIGNALES_8                  0
#define MASKA_MONITOTYNG_ZOP_SIGNALES_9                  0
/*****************************************/

/*****************************************/
//Макска моніторингу частоти при АЧР для дискретного реєстратора
/*****************************************/
#define MASKA_MONITOTYNG_F_MIN_ACHR_SIGNALES_0   0
#define MASKA_MONITOTYNG_F_MIN_ACHR_SIGNALES_1   0
#define MASKA_MONITOTYNG_F_MIN_ACHR_SIGNALES_2   0
#define MASKA_MONITOTYNG_F_MIN_ACHR_SIGNALES_3   0
#define MASKA_MONITOTYNG_F_MIN_ACHR_SIGNALES_4   0

#define MASKA_MONITOTYNG_F_MIN_ACHR_SIGNALES_5 (                 \
     (1u << (RANG_PO_ACHR1 - 160))                               \
   | (1u << (RANG_ACHR_CHAPV1 - 160))                            \
   | (1u << (RANG_PO_ACHR2 - 160))                               \
   | (1u << (RANG_ACHR_CHAPV2 - 160))                            \
)     

#define MASKA_MONITOTYNG_F_MIN_ACHR_SIGNALES_6   0
#define MASKA_MONITOTYNG_F_MIN_ACHR_SIGNALES_7   0
#define MASKA_MONITOTYNG_F_MIN_ACHR_SIGNALES_8   0
#define MASKA_MONITOTYNG_F_MIN_ACHR_SIGNALES_9   0
/*****************************************/

/*****************************************/
//Макска моніторингу максимального фазного струму для фіксації КЗ
/*****************************************/
#define MASKA_MONITOTYNG_PHASE_SIGNALES_KZ_0        0
#define MASKA_MONITOTYNG_PHASE_SIGNALES_KZ_1        0

#define MASKA_MONITOTYNG_PHASE_SIGNALES_KZ_2 (                 \
     (1u << (RANG_PO_MTZ1 - 64))                               \
   | (1u << (RANG_PO_MTZN1_VPERED - 64))                       \
   | (1u << (RANG_PO_MTZN1_NAZAD - 64))                        \
   | (1u << (RANG_PO_MTZPN1 - 64))                             \
   | (1u << (RANG_PO_MTZ2 - 64))                               \
   | (1u << (RANG_PO_MTZN2_VPERED - 64))                       \
   | (1u << (RANG_PO_MTZN2_NAZAD - 64))                        \
   | (1u << (RANG_PO_MTZPN2 - 64))                             \
   | (1u << (RANG_PO_MTZ3 - 64))                               \
   | (1u << (RANG_PO_MTZN3_VPERED - 64))                       \
   | (1u << (RANG_PO_MTZN3_NAZAD - 64))                        \
   | (1u << (RANG_PO_MTZPN3 - 64))                             \
   | (1u << (RANG_PO_MTZ4 - 64))                               \
   | (1u << (RANG_PO_MTZN4_VPERED - 64))                       \
   | (1u << (RANG_PO_MTZN4_NAZAD - 64))                        \
   | (1u << (RANG_PO_MTZPN4 - 64))                             \
)

#define MASKA_MONITOTYNG_PHASE_SIGNALES_KZ_3        0
#define MASKA_MONITOTYNG_PHASE_SIGNALES_KZ_4        0
#define MASKA_MONITOTYNG_PHASE_SIGNALES_KZ_5        0
#define MASKA_MONITOTYNG_PHASE_SIGNALES_KZ_6        0
#define MASKA_MONITOTYNG_PHASE_SIGNALES_KZ_7        0
#define MASKA_MONITOTYNG_PHASE_SIGNALES_KZ_8        0
#define MASKA_MONITOTYNG_PHASE_SIGNALES_KZ_9        0
/*****************************************/


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
  
}__vymknennja_vid_zakhystiv;
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
#define LED_N_COL       7
#define LED_N_ROW       8
/*****************************************/

/*****************************************/
//Максимальна кількість сигналів (функцій), які можуть бути зранжовані на входи/виходи/світлоіндикатори/д.реєстратор/оф
/*****************************************/
#define VAGA_MAX_FUNCTIONS_IN_INPUT             3
#define MAX_FUNCTIONS_IN_INPUT                  (1 << VAGA_MAX_FUNCTIONS_IN_INPUT)
#define VAGA_MAX_FUNCTIONS_IN_OUTPUT            4
#define MAX_FUNCTIONS_IN_OUTPUT                 (1 << VAGA_MAX_FUNCTIONS_IN_OUTPUT)
#define VAGA_MAX_FUNCTIONS_IN_LED               3
#define MAX_FUNCTIONS_IN_LED                    (1 << VAGA_MAX_FUNCTIONS_IN_LED)
#define VAGA_MAX_FUNCTIONS_IN_DF                4
#define MAX_FUNCTIONS_IN_DF                     (1 << VAGA_MAX_FUNCTIONS_IN_DF)
#define MAX_FUNCTIONS_IN_DT                     6
#define MAX_FUNCTIONS_IN_D_AND                  NUMBER_IN_AND
#define MAX_FUNCTIONS_IN_D_OR                   NUMBER_IN_OR
#define MAX_FUNCTIONS_IN_D_XOR                  2
#define MAX_FUNCTIONS_IN_D_NOT                  1
#define VAGA_MAX_FUNCTIONS_IN_DB                3
#define MAX_FUNCTIONS_IN_DB                     (1 << VAGA_MAX_FUNCTIONS_IN_DB)
#define VAGA_MAX_FUNCTIONS_IN_AREG              5
#define MAX_FUNCTIONS_IN_AREG                   (1 << VAGA_MAX_FUNCTIONS_IN_AREG)
#define VAGA_MAX_FUNCTIONS_IN_DREG              5
#define MAX_FUNCTIONS_IN_DREG                   (1 << VAGA_MAX_FUNCTIONS_IN_DREG)
#define VAGA_MAX_FUNCTIONS_IN_OFF_CB            5
#define MAX_FUNCTIONS_IN_OFF_CB                 (1 << VAGA_MAX_FUNCTIONS_IN_OFF_CB)
#define VAGA_MAX_FUNCTIONS_IN_ON_CB             5
#define MAX_FUNCTIONS_IN_ON_CB                  (1 << VAGA_MAX_FUNCTIONS_IN_ON_CB)
#define VAGA_MAX_FUNCTIONS_IN_IN_GOOSE          3
#define MAX_FUNCTIONS_IN_IN_GOOSE               (1 << VAGA_MAX_FUNCTIONS_IN_IN_GOOSE)
#define VAGA_MAX_FUNCTIONS_IN_IN_MMS            3
#define MAX_FUNCTIONS_IN_IN_MMS                 (1 << VAGA_MAX_FUNCTIONS_IN_IN_MMS)
#define VAGA_MAX_FUNCTIONS_IN_OUT_LAN           3
#define MAX_FUNCTIONS_IN_OUT_LAN                (1 << VAGA_MAX_FUNCTIONS_IN_OUT_LAN)

/*****************************************/

/*****************************************/
//Константи для реєстратора статистики
/*****************************************/
#define EXT_SRAM_512 1
/*****************************************/

/*****************************************/
//Константи для комунікації
/*****************************************/
#define MAX_CHAR_IN_NAME_OF_CELL                              32
/*****************************************/

/*****************************************/
//Константи для фіксації зміни настройок
/*****************************************/
#define CHANGED_ETAP_NONE                                     0
#define CHANGED_ETAP_EXECUTION                                1
#define CHANGED_ETAP_ENDED                                    2
#define CHANGED_ETAP_ENDED_EXTRA_ETAP                         3
/*****************************************/

/*****************************************/
//Константи для ранжування функціональних кнопок
/*****************************************/
enum __odynyci_vymirjuvannja {
INDEX_A = 0,
INDEX_V,
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
#define ADJUSTMENT_3I0_IA_IB_IC_UA_UB_UC_3U0                  4
#define ADJUSTMENT_ID                                         ADJUSTMENT_3I0_IA_IB_IC_UA_UB_UC_3U0
/*****************************************/

/*****************************************/
//Версія програмного забезпечення
/*****************************************/
#define VERSIA_PZ                                             14
#define MODYFIKACIA_VERSII_PZ                                 34
#define ZBIRKA_VERSII_PZ                                      3
#define ZBIRKA_PIDVERSII_PZ                                   0
/*****************************************/

/*****************************************/
//Час компіляції
/*****************************************/

#define YEAR_VER                                              ((__DATE__[9] != ' ') ? (((__DATE__[9] - 0x30) << 4) | (__DATE__[10] - 0x30)) : (__DATE__[10] - 0x30))
#define MONTH_VER                                             str_to_int_DATE_Mmm()
#define DAY_VER                                               ((__DATE__[4] != ' ') ? (((__DATE__[4] - 0x30) << 4) | (__DATE__[5] - 0x30)) : (__DATE__[5] - 0x30))
#define HOUR_VER                                              (((__TIME__[0] - 0x30) << 4) | (__TIME__[1] - 0x30))
#define MINUTE_VER                                            (((__TIME__[3] - 0x30) << 4) | (__TIME__[4] - 0x30))
#define SECOND_VER                                            (((__TIME__[6] - 0x30) << 4) | (__TIME__[7] - 0x30))
/*****************************************/

/*****************************************/
//Версія універсальної карти памяті
/*****************************************/
#define VERSIA_GMM                                             8
#define MODYFIKACIA_VERSII_GMM                                 4
/*****************************************/
#endif
