#ifndef __CONST_MENU_UP__
#define __CONST_MENU_UP__

#define EKRAN_SETPOINT_UP_GROUP1   (EKRAN_SETPOINT_UMAX_GROUP4 + 1)
#define EKRAN_SETPOINT_UP_GROUP2   (EKRAN_SETPOINT_UP_GROUP1 + 1)
#define EKRAN_SETPOINT_UP_GROUP3   (EKRAN_SETPOINT_UP_GROUP2 + 1)
#define EKRAN_SETPOINT_UP_GROUP4   (EKRAN_SETPOINT_UP_GROUP3 + 1)
#define EKRAN_TIMEOUT_UP_GROUP1    (EKRAN_TIMEOUT_UMAX_GROUP4 + 1)
#define EKRAN_TIMEOUT_UP_GROUP2    (EKRAN_TIMEOUT_UP_GROUP1  + 1)
#define EKRAN_TIMEOUT_UP_GROUP3    (EKRAN_TIMEOUT_UP_GROUP2  + 1)
#define EKRAN_TIMEOUT_UP_GROUP4    (EKRAN_TIMEOUT_UP_GROUP3  + 1)
#define EKRAN_CONTROL_UP           (EKRAN_CONTROL_UMAX  + 1)

enum _index_ml_stp_UP
{
  INDEX_ML_STP_UP = 0,
  INDEX_ML_STP_UP_KP,
  
  MAX_ROW_FOR_SETPOINT_UP
};

#define COL_SETPOINT_UP_I_BEGIN      4
#define COL_SETPOINT_UP_I_COMMA      7
#define COL_SETPOINT_UP_I_END        9

#define COL_SETPOINT_UP_3I0_BEGIN    4
#define COL_SETPOINT_UP_3I0_COMMA    5
#define COL_SETPOINT_UP_3I0_END      8

#define COL_SETPOINT_UP_U_BEGIN      4
#define COL_SETPOINT_UP_U_COMMA      7
#define COL_SETPOINT_UP_U_END        8

#define COL_SETPOINT_UP_PQ_BEGIN     3
#define COL_SETPOINT_UP_PQ_COMMA     7
#define COL_SETPOINT_UP_PQ_END       10

#define COL_SETPOINT_UP_S_BEGIN      2
#define COL_SETPOINT_UP_S_COMMA      6
#define COL_SETPOINT_UP_S_END        9

#define COL_SETPOINT_UP_KP_BEGIN     6
#define COL_SETPOINT_UP_KP_COMMA     7
#define COL_SETPOINT_UP_KP_END       9

enum _index_ml_tmo_UP
{
  INDEX_ML_TMOUP = 0,
  
  MAX_ROW_FOR_TIMEOUT_UP
};

#define COL_TMO_UP_BEGIN             4
#define COL_TMO_UP_COMMA             7
#define COL_TMO_UP_END               9

enum _control_type_UP
{
  _UP_CTRL_MIN = 0,
  
  UP_CTRL_Ia_Ib_Ic = _UP_CTRL_MIN,
  UP_CTRL_Ia,
  UP_CTRL_Ib,
  UP_CTRL_Ic,
  UP_CTRL_I1,
  UP_CTRL_I2,
  UP_CTRL_I04,
  UP_CTRL_3I0_r,
  UP_CTRL_3I0,
  UP_CTRL_3I0_others,
  UP_CTRL_Ua_Ub_Uc,
  UP_CTRL_Uab_Ubc_Uca,
  UP_CTRL_Ua,
  UP_CTRL_Uab,
  UP_CTRL_Ub,
  UP_CTRL_Ubc,
  UP_CTRL_Uc,
  UP_CTRL_Uca,
  UP_CTRL_U1,
  UP_CTRL_U2,
  UP_CTRL_3U0,
  UP_CTRL_P,
  UP_CTRL_Q,
  UP_CTRL_S,
  
  _UP_CTRL_MAX = UP_CTRL_S,
  
  _UP_CTRL_NUMBER
};

enum _index_ml_ctrl_UP
{
  _CTR_UP_PART_I = 0, 
  
  CTR_UP_CTRL_INPUT = _CTR_UP_PART_I,
  
  _CTR_UP_PART_II,
  CTR_UP_STATE_BIT = _CTR_UP_PART_II,
  CTR_UP_OR_AND_BIT,
  CTR_UP_MORE_LESS_BIT,
  
  _CTR_UP_NEXT_BIT,
  MAX_ROW_FOR_CONTROL_UP = _CTR_UP_NEXT_BIT
};

#define CTR_UP_MASKA    (                                                                                                                                                          \
                         MASKA_FOR_BIT(0*(_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_STATE_BIT     - (_CTR_UP_PART_II - _CTR_UP_PART_I))   | \
                         MASKA_FOR_BIT(0*(_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_OR_AND_BIT    - (_CTR_UP_PART_II - _CTR_UP_PART_I))   | \
                         MASKA_FOR_BIT(0*(_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_MORE_LESS_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I))   | \
                         MASKA_FOR_BIT(1*(_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_STATE_BIT     - (_CTR_UP_PART_II - _CTR_UP_PART_I))   | \
                         MASKA_FOR_BIT(1*(_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_OR_AND_BIT    - (_CTR_UP_PART_II - _CTR_UP_PART_I))   | \
                         MASKA_FOR_BIT(1*(_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_MORE_LESS_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I))   | \
                         MASKA_FOR_BIT(2*(_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_STATE_BIT     - (_CTR_UP_PART_II - _CTR_UP_PART_I))   | \
                         MASKA_FOR_BIT(2*(_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_OR_AND_BIT    - (_CTR_UP_PART_II - _CTR_UP_PART_I))   | \
                         MASKA_FOR_BIT(2*(_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_MORE_LESS_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I))   | \
                         MASKA_FOR_BIT(3*(_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_STATE_BIT     - (_CTR_UP_PART_II - _CTR_UP_PART_I))   | \
                         MASKA_FOR_BIT(3*(_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_OR_AND_BIT    - (_CTR_UP_PART_II - _CTR_UP_PART_I))   | \
                         MASKA_FOR_BIT(3*(_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_MORE_LESS_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I))   | \
                         MASKA_FOR_BIT(4*(_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_STATE_BIT     - (_CTR_UP_PART_II - _CTR_UP_PART_I))   | \
                         MASKA_FOR_BIT(4*(_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_OR_AND_BIT    - (_CTR_UP_PART_II - _CTR_UP_PART_I))   | \
                         MASKA_FOR_BIT(4*(_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_MORE_LESS_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I))   | \
                         MASKA_FOR_BIT(5*(_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_STATE_BIT     - (_CTR_UP_PART_II - _CTR_UP_PART_I))   | \
                         MASKA_FOR_BIT(5*(_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_OR_AND_BIT    - (_CTR_UP_PART_II - _CTR_UP_PART_I))   | \
                         MASKA_FOR_BIT(5*(_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_MORE_LESS_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I))   | \
                         MASKA_FOR_BIT(6*(_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_STATE_BIT     - (_CTR_UP_PART_II - _CTR_UP_PART_I))   | \
                         MASKA_FOR_BIT(6*(_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_OR_AND_BIT    - (_CTR_UP_PART_II - _CTR_UP_PART_I))   | \
                         MASKA_FOR_BIT(6*(_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_MORE_LESS_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I))   | \
                         MASKA_FOR_BIT(7*(_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_STATE_BIT     - (_CTR_UP_PART_II - _CTR_UP_PART_I))   | \
                         MASKA_FOR_BIT(7*(_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_OR_AND_BIT    - (_CTR_UP_PART_II - _CTR_UP_PART_I))   | \
                         MASKA_FOR_BIT(7*(_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_MORE_LESS_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I))     \
                        )


#endif
