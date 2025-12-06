#ifndef __CONST_MENU_UMIN__
#define __CONST_MENU_UMIN__

#define EKRAN_SETPOINT_UMIN_GROUP1   (EKRAN_SETPOINT_ZOP_GROUP4 + 1)
#define EKRAN_SETPOINT_UMIN_GROUP2   (EKRAN_SETPOINT_UMIN_GROUP1 + 1)
#define EKRAN_SETPOINT_UMIN_GROUP3   (EKRAN_SETPOINT_UMIN_GROUP2 + 1)
#define EKRAN_SETPOINT_UMIN_GROUP4   (EKRAN_SETPOINT_UMIN_GROUP3 + 1)
#define EKRAN_TIMEOUT_UMIN_GROUP1    (EKRAN_TIMEOUT_ZOP_GROUP4 + 1)
#define EKRAN_TIMEOUT_UMIN_GROUP2    (EKRAN_TIMEOUT_UMIN_GROUP1  + 1)
#define EKRAN_TIMEOUT_UMIN_GROUP3    (EKRAN_TIMEOUT_UMIN_GROUP2  + 1)
#define EKRAN_TIMEOUT_UMIN_GROUP4    (EKRAN_TIMEOUT_UMIN_GROUP3  + 1)
#define EKRAN_CONTROL_UMIN           (EKRAN_CONTROL_ZOP  + 1)

#define MAX_ROW_FOR_SETPOINT_UMIN        4
#define MAX_ROW_FOR_TIMEOUT_UMIN         2
#define MAX_ROW_FOR_CONTROL_UMIN         8

#define INDEX_ML_STPUMIN1                0
#define INDEX_ML_STPUMIN1_IBLK           1
#define INDEX_ML_STPUMIN2                2
#define INDEX_ML_STPUMIN2_IBLK           3

#define INDEX_ML_TMOUMIN1                0
#define INDEX_ML_TMOUMIN2                1

#define COL_SETPOINT_UMIN1_BEGIN         4
#define COL_SETPOINT_UMIN1_COMMA         7
#define COL_SETPOINT_UMIN1_END           8

#define COL_SETPOINT_UMIN1_IBLK_BEGIN    5
#define COL_SETPOINT_UMIN1_IBLK_COMMA    6
#define COL_SETPOINT_UMIN1_IBLK_END      8

#define COL_SETPOINT_UMIN2_BEGIN         4
#define COL_SETPOINT_UMIN2_COMMA         7
#define COL_SETPOINT_UMIN2_END           8

#define COL_SETPOINT_UMIN2_IBLK_BEGIN    5
#define COL_SETPOINT_UMIN2_IBLK_COMMA    6
#define COL_SETPOINT_UMIN2_IBLK_END      8

#define COL_TMO_UMIN1_BEGIN              4
#define COL_TMO_UMIN1_COMMA              6
#define COL_TMO_UMIN1_END                8

#define COL_TMO_UMIN2_BEGIN              4
#define COL_TMO_UMIN2_COMMA              6
#define COL_TMO_UMIN2_END                8

#define INDEX_CTR_UMIN1                  0          
#define CTR_UMIN1                        (1 << INDEX_CTR_UMIN1)          
#define INDEX_CTR_PO_UMIN1_OR_AND        1          
#define CTR_PO_UMIN1_OR_AND              (1 << INDEX_CTR_PO_UMIN1_OR_AND)          
#define INDEX_CTR_UMIN1_UBLK             2          
#define CTR_UMIN1_UBLK                   (1 << INDEX_CTR_UMIN1_UBLK)          
#define INDEX_CTR_UMIN1_IBLK             3          
#define CTR_UMIN1_IBLK                   (1 << INDEX_CTR_UMIN1_IBLK)          
#define INDEX_CTR_UMIN2                  4          
#define CTR_UMIN2                        (1 << INDEX_CTR_UMIN2)          
#define INDEX_CTR_PO_UMIN2_OR_AND        5          
#define CTR_PO_UMIN2_OR_AND              (1 << INDEX_CTR_PO_UMIN2_OR_AND)          
#define INDEX_CTR_UMIN2_UBLK             6          
#define CTR_UMIN2_UBLK                   (1 << INDEX_CTR_UMIN2_UBLK)          
#define INDEX_CTR_UMIN2_IBLK             7          
#define CTR_UMIN2_IBLK                   (1 << INDEX_CTR_UMIN2_IBLK)          

#define CTR_UMIN_MASKA                  (CTR_UMIN1            |\
                                         CTR_PO_UMIN1_OR_AND  |\
                                         CTR_UMIN1_UBLK       |\
                                         CTR_UMIN1_IBLK       |\
                                         CTR_UMIN2            |\
                                         CTR_PO_UMIN2_OR_AND  |\
                                         CTR_UMIN2_UBLK       |\
                                         CTR_UMIN2_IBLK)

#endif
