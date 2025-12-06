#ifndef __CONST_MENU_UMAX__
#define __CONST_MENU_UMAX__

#define EKRAN_SETPOINT_UMAX_GROUP1   (EKRAN_SETPOINT_UMIN_GROUP4 + 1)
#define EKRAN_SETPOINT_UMAX_GROUP2   (EKRAN_SETPOINT_UMAX_GROUP1 + 1)
#define EKRAN_SETPOINT_UMAX_GROUP3   (EKRAN_SETPOINT_UMAX_GROUP2 + 1)
#define EKRAN_SETPOINT_UMAX_GROUP4   (EKRAN_SETPOINT_UMAX_GROUP3 + 1)
#define EKRAN_TIMEOUT_UMAX_GROUP1    (EKRAN_TIMEOUT_UMIN_GROUP4 + 1)
#define EKRAN_TIMEOUT_UMAX_GROUP2    (EKRAN_TIMEOUT_UMAX_GROUP1  + 1)
#define EKRAN_TIMEOUT_UMAX_GROUP3    (EKRAN_TIMEOUT_UMAX_GROUP2  + 1)
#define EKRAN_TIMEOUT_UMAX_GROUP4    (EKRAN_TIMEOUT_UMAX_GROUP3  + 1)
#define EKRAN_CONTROL_UMAX           (EKRAN_CONTROL_UMIN  + 1)

#define MAX_ROW_FOR_TIMEOUT_UMAX         2
#define MAX_ROW_FOR_CONTROL_UMAX         4

enum _index_ml_stp_Umax
{
  INDEX_ML_STPUMAX1 = 0,
  INDEX_ML_STPUMAX2,
  INDEX_ML_STP_KP_UMAX,
  
  MAX_ROW_FOR_SETPOINT_UMAX
};

#define INDEX_ML_TMOUMAX1                0
#define INDEX_ML_TMOUMAX2                1

#define COL_SETPOINT_UMAX1_BEGIN         4
#define COL_SETPOINT_UMAX1_COMMA         7
#define COL_SETPOINT_UMAX1_END           8

#define COL_SETPOINT_UMAX2_BEGIN         4
#define COL_SETPOINT_UMAX2_COMMA         7
#define COL_SETPOINT_UMAX2_END           8

#define COL_SETPOINT_KP_UMAX_BEGIN       6
#define COL_SETPOINT_KP_UMAX_COMMA       7
#define COL_SETPOINT_KP_UMAX_END         9

#define COL_TMO_UMAX1_BEGIN              3
#define COL_TMO_UMAX1_COMMA              6
#define COL_TMO_UMAX1_END                8

#define COL_TMO_UMAX2_BEGIN              3
#define COL_TMO_UMAX2_COMMA              6
#define COL_TMO_UMAX2_END                8

#define INDEX_CTR_UMAX1                  0
#define CTR_UMAX1                        (1 << INDEX_CTR_UMAX1)
#define INDEX_CTR_PO_UMAX1_OR_AND        1
#define CTR_PO_UMAX1_OR_AND              (1 << INDEX_CTR_PO_UMAX1_OR_AND)
#define INDEX_CTR_UMAX2                  2
#define CTR_UMAX2                        (1 << INDEX_CTR_UMAX2)
#define INDEX_CTR_PO_UMAX2_OR_AND        3
#define CTR_PO_UMAX2_OR_AND              (1 << INDEX_CTR_PO_UMAX2_OR_AND)

#define CTR_UMAX_MASKA                  (CTR_UMAX1            |\
                                         CTR_PO_UMAX1_OR_AND  |\
                                         CTR_UMAX2            |\
                                         CTR_PO_UMAX2_OR_AND)

#endif
