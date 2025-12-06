#ifndef __CONST_MENU_ACHR_CHAPV__
#define __CONST_MENU_ACHR_CHAPV__


#define EKRAN_SETPOINT_ACHR_CHAPV_GROUP1   (EKRAN_SETPOINT_TZNP_GROUP4 + 1)
#define EKRAN_SETPOINT_ACHR_CHAPV_GROUP2   (EKRAN_SETPOINT_ACHR_CHAPV_GROUP1 + 1)
#define EKRAN_SETPOINT_ACHR_CHAPV_GROUP3   (EKRAN_SETPOINT_ACHR_CHAPV_GROUP2 + 1)
#define EKRAN_SETPOINT_ACHR_CHAPV_GROUP4   (EKRAN_SETPOINT_ACHR_CHAPV_GROUP3 + 1)
#define EKRAN_TIMEOUT_ACHR_CHAPV_GROUP1    (EKRAN_TIMEOUT_APV_GROUP4 + 1)
#define EKRAN_TIMEOUT_ACHR_CHAPV_GROUP2    (EKRAN_TIMEOUT_ACHR_CHAPV_GROUP1 + 1)
#define EKRAN_TIMEOUT_ACHR_CHAPV_GROUP3    (EKRAN_TIMEOUT_ACHR_CHAPV_GROUP2 + 1)
#define EKRAN_TIMEOUT_ACHR_CHAPV_GROUP4    (EKRAN_TIMEOUT_ACHR_CHAPV_GROUP3 + 1)
#define EKRAN_CONTROL_ACHR_CHAPV           (EKRAN_CONTROL_APV + 1)

#define MAX_ROW_FOR_SETPOINT_ACHR_CHAPV     5
#define MAX_ROW_FOR_TIMEOUT_ACHR_CHAPV      4
#define MAX_ROW_FOR_CONTROL_ACHR_CHAPV      5

#define INDEX_ML_STPACHR_CHAPV_UF           0
#define INDEX_ML_STPACHR1_F_RAB             1
#define INDEX_ML_STPCHAPV1_F_RAB            2
#define INDEX_ML_STPACHR2_F_RAB             3
#define INDEX_ML_STPCHAPV2_F_RAB            4

#define INDEX_ML_TMOACHR1                   0
#define INDEX_ML_TMOCHAPV1                  1
#define INDEX_ML_TMOACHR2                   2
#define INDEX_ML_TMOCHAPV2                  3

#define COL_SETPOINT_ACHR_CHAPV_UF_BEGIN    5
#define COL_SETPOINT_ACHR_CHAPV_UF_COMMA    8
#define COL_SETPOINT_ACHR_CHAPV_UF_END      9

#define COL_SETPOINT_ACHR1_F_RAB_BEGIN      4
#define COL_SETPOINT_ACHR1_F_RAB_COMMA      6
#define COL_SETPOINT_ACHR1_F_RAB_END        7

#define COL_SETPOINT_CHAPV1_F_RAB_BEGIN     4
#define COL_SETPOINT_CHAPV1_F_RAB_COMMA     6
#define COL_SETPOINT_CHAPV1_F_RAB_END       7

#define COL_SETPOINT_ACHR2_F_RAB_BEGIN      4
#define COL_SETPOINT_ACHR2_F_RAB_COMMA      6
#define COL_SETPOINT_ACHR2_F_RAB_END        7

#define COL_SETPOINT_CHAPV2_F_RAB_BEGIN     4
#define COL_SETPOINT_CHAPV2_F_RAB_COMMA     6
#define COL_SETPOINT_CHAPV2_F_RAB_END       7

#define COL_TMO_ACHR1_BEGIN                 4
#define COL_TMO_ACHR1_COMMA                 6
#define COL_TMO_ACHR1_END                   8

#define COL_TMO_CHAPV1_BEGIN                4
#define COL_TMO_CHAPV1_COMMA                6
#define COL_TMO_CHAPV1_END                  8

#define COL_TMO_ACHR2_BEGIN                 4
#define COL_TMO_ACHR2_COMMA                 7
#define COL_TMO_ACHR2_END                   9

#define COL_TMO_CHAPV2_BEGIN                4
#define COL_TMO_CHAPV2_COMMA                7
#define COL_TMO_CHAPV2_END                  9

#define INDEX_CTR_CHAPV_VID_DV          0
#define CTR_CHAPV_VID_DV                (1 << INDEX_CTR_CHAPV_VID_DV)
#define INDEX_CTR_ACHR1                 1
#define CTR_ACHR1                       (1 << INDEX_CTR_ACHR1)
#define INDEX_CTR_CHAPV1                2
#define CTR_CHAPV1                      (1 << INDEX_CTR_CHAPV1)
#define INDEX_CTR_ACHR2                 3
#define CTR_ACHR2                       (1 << INDEX_CTR_ACHR2)
#define INDEX_CTR_CHAPV2                4
#define CTR_CHAPV2                      (1 << INDEX_CTR_CHAPV2)

#define CTR_ACHR_CHAPV_MASKA            (                         \
                                         CTR_CHAPV_VID_DV        |\
                                         CTR_ACHR1               |\
                                         CTR_CHAPV1              |\
                                         CTR_ACHR2               |\
                                         CTR_CHAPV2               \
                                        )

#endif
