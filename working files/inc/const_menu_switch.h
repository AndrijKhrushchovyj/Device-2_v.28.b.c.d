#ifndef __CONST_MENU_SWITCH__
#define __CONST_MENU_SWITCH__

#define EKRAN_SETPOINT_SWITCH          (EKRAN_CHOOSE_SETTINGS_SWITCHER  + 1)
#define EKRAN_TIMEOUT_SWITCH           (EKRAN_SETPOINT_SWITCH  + 1)
#define EKRAN_CONTROL_SWITCH           (EKRAN_TIMEOUT_SWITCH  + 1)
#define EKRAN_CHOOSE_RANG_SWITCH       (EKRAN_CONTROL_SWITCH  + 1)
#define EKRAN_RANGUVANNJA_OFF_CB       (EKRAN_CHOOSE_RANG_SWITCH + 1) 
#define EKRAN_RANGUVANNJA_ON_CB        (EKRAN_RANGUVANNJA_OFF_CB + 1) 

#define MAX_ROW_FOR_SETPOINT_SWITCH       7
#define MAX_ROW_FOR_TIMEOUT_SWITCH        4
#define MAX_ROW_FOR_CONTROL_SWITCH        2
#define MAX_ROW_FOR_RANG_SWITCH           2
#define MAX_ROW_RANGUVANNJA_OFF_CB        NUMBER_TOTAL_SIGNAL_FOR_RANG
#define MAX_ROW_RANGUVANNJA_ON_CB         NUMBER_TOTAL_SIGNAL_FOR_RANG

#define INDEX_ML_STPInom                  0
#define INDEX_ML_STPRKS_Inom              1
#define INDEX_ML_STPMInom_vymk            2
#define INDEX_ML_STPRKS_Inom_vymk         3
#define INDEX_ML_STPPOCHATKOVYJ_RESURS    4
#define INDEX_ML_STPKRYTYCHNYJ_RESURS     5
#define INDEX_ML_STPPOCHATKOVA_K_VYMK     6

#define COL_SETPOINT_Inom_BEGIN                 5
#define COL_SETPOINT_Inom_END                   9

#define COL_SETPOINT_RKS_Inom_BEGIN             5
#define COL_SETPOINT_RKS_Inom_END               10

#define COL_SETPOINT_Inom_vymk_BEGIN            3
#define COL_SETPOINT_Inom_vymk_COMMA            5
#define COL_SETPOINT_Inom_vymk_END              8

#define COL_SETPOINT_RKS_Inom_vymk_BEGIN        6
#define COL_SETPOINT_RKS_Inom_vymk_END          8

#define COL_SETPOINT_POCHATKOVYJ_RESURS_BEGIN   5
#define COL_SETPOINT_POCHATKOVYJ_RESURS_END     10

#define COL_SETPOINT_KRYTYCHNYJ_RESURS_BEGIN    5
#define COL_SETPOINT_KRYTYCHNYJ_RESURS_END      9

#define COL_SETPOINT_POCHATKOVA_K_VYMK_BEGIN    5
#define COL_SETPOINT_POCHATKOVA_K_VYMK_END      10

#define INDEX_ML_TMOON                    0
#define INDEX_ML_TMOOFF                   1
#define INDEX_ML_TMOUDL_BLK_ON            2
#define INDEX_ML_TMOPRYVODA_VV            3

#define COL_TMO_SWCH_ON_BEGIN             5
#define COL_TMO_SWCH_ON_COMMA             6
#define COL_TMO_SWCH_ON_END               8

#define COL_TMO_SWCH_OFF_BEGIN            5
#define COL_TMO_SWCH_OFF_COMMA            6
#define COL_TMO_SWCH_OFF_END              8

#define COL_TMO_SWCH_UDL_BLK_ON_BEGIN     4
#define COL_TMO_SWCH_UDL_BLK_ON_COMMA     6
#define COL_TMO_SWCH_UDL_BLK_ON_END       8

#define COL_TMO_PRYVODA_VV_BEGIN          5
#define COL_TMO_PRYVODA_VV_COMMA          6
#define COL_TMO_PRYVODA_VV_END            8

#define INDEX_ML_CTRPRYVOD_VV             0
#define CTR_PRYVOD_VV                     (1 << INDEX_ML_CTRPRYVOD_VV)          
#define INDEX_ML_CTRRESURS_VV             1
#define CTR_RESURS_VV                     (1 << INDEX_ML_CTRRESURS_VV)          

#define CTR_SWITCH_MASKA                  (                     \
                                            CTR_PRYVOD_VV     | \
                                            CTR_RESURS_VV       \
                                          )

#define INDEX_ML_RANG_OFF_CB              0
#define INDEX_ML_RANG_ON_CB               1

#endif
