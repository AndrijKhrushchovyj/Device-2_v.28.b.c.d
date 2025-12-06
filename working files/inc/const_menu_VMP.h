#ifndef __CONST_MENU_VMP__
#define __CONST_MENU_VMP__

#define EKRAN_SETPOINT_VMP_FORWARD                      (EKRAN_CHOOSE_SETTINGS_VMP + 1)
#define EKRAN_SETPOINT_VMP_BACKWARD                     (EKRAN_SETPOINT_VMP_FORWARD + 1)
#define EKRAN_CONTROL_VMP                               (EKRAN_SETPOINT_VMP_BACKWARD + 1)

#define MAX_ROW_FOR_CHOSE_SETTINGS_VMP                  3
#define MAX_ROW_FOR_CONTROL_VMP                         1

#define INDEX_ML_FORWARD_SETTINGS_VMP                   0
#define INDEX_ML_BACKWARD_SETTINGS_VMP                  1
#define INDEX_ML_CONTROL_SETTINGS_VMP                   2

#define INDEX_ML_LINES                                  0
#define INDEX_ML_DOVGYNA                                1
#define INDEX_ML_OPIR                                   2

#define COL_SETPOINT_LINES_BEGIN             7
#define COL_SETPOINT_LINES_END               7

#define COL_SETPOINT_DOVGYNA_BEGIN           3
#define COL_SETPOINT_DOVGYNA_COMMA           5
#define COL_SETPOINT_DOVGYNA_END             8

#define COL_SETPOINT_OPIR_BEGIN              3
#define COL_SETPOINT_OPIR_COMMA              5
#define COL_SETPOINT_OPIR_END                8

#define INDEX_CTR_VMP_STATE       0          
#define CTR_VMP_STATE             (1 << INDEX_CTR_VMP_STATE)          
#define CTR_VMP_MASKA             (CTR_VMP_STATE)


#endif
