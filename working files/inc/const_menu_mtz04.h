#ifndef __CONST_MENU_MTZ04__
#define __CONST_MENU_MTZ04__

#define EKRAN_SETPOINT_MTZ04_GROUP1   (EKRAN_SETPOINT_MTZ_GROUP4 + 1)
#define EKRAN_SETPOINT_MTZ04_GROUP2   (EKRAN_SETPOINT_MTZ04_GROUP1 + 1)
#define EKRAN_SETPOINT_MTZ04_GROUP3   (EKRAN_SETPOINT_MTZ04_GROUP2 + 1)
#define EKRAN_SETPOINT_MTZ04_GROUP4   (EKRAN_SETPOINT_MTZ04_GROUP3 + 1)
#define EKRAN_TIMEOUT_MTZ04_GROUP1    (EKRAN_TIMEOUT_MTZ_GROUP4 + 1)
#define EKRAN_TIMEOUT_MTZ04_GROUP2    (EKRAN_TIMEOUT_MTZ04_GROUP1  + 1)
#define EKRAN_TIMEOUT_MTZ04_GROUP3    (EKRAN_TIMEOUT_MTZ04_GROUP2  + 1)
#define EKRAN_TIMEOUT_MTZ04_GROUP4    (EKRAN_TIMEOUT_MTZ04_GROUP3  + 1)
#define EKRAN_CONTROL_MTZ04           (EKRAN_CONTROL_MTZ + 1)

#define MAX_ROW_FOR_SETPOINT_MTZ04        2
#define MAX_ROW_FOR_TIMEOUT_MTZ04         4
#define MAX_ROW_FOR_CONTROL_MTZ04         5

#define INDEX_ML_STPMTZ04_1               0
#define INDEX_ML_STPMTZ04_2               1

#define INDEX_ML_TMOMTZ04_1               0
#define INDEX_ML_TMOMTZ04_2               1
#define INDEX_ML_TMOMTZ04_2_PR            2
#define INDEX_ML_TMOMTZ04_2_VVID_PR       3

#define COL_SETPOINT_MTZ04_1_BEGIN                3
#define COL_SETPOINT_MTZ04_1_COMMA                6
#define COL_SETPOINT_MTZ04_1_END                  8

#define COL_SETPOINT_MTZ04_2_BEGIN                3
#define COL_SETPOINT_MTZ04_2_COMMA                6
#define COL_SETPOINT_MTZ04_2_END                  8

#define COL_TMO_MTZ04_1_BEGIN                     4
#define COL_TMO_MTZ04_1_COMMA                     6
#define COL_TMO_MTZ04_1_END                       8

#define COL_TMO_MTZ04_2_BEGIN                     3
#define COL_TMO_MTZ04_2_COMMA                     6
#define COL_TMO_MTZ04_2_END                       8

#define COL_TMO_MTZ04_2_PR_BEGIN                  5
#define COL_TMO_MTZ04_2_PR_COMMA                  6
#define COL_TMO_MTZ04_2_PR_END                    8

#define COL_TMO_MTZ04_2_VVID_PR_BEGIN             5
#define COL_TMO_MTZ04_2_VVID_PR_COMMA             6
#define COL_TMO_MTZ04_2_VVID_PR_END               8

#define INDEX_ML_CTRMTZ04_1                       0          
#define INDEX_ML_CTRMTZ04_2                       1        
#define INDEX_ML_CTRMTZ04_2_TYPE                  2          
#define INDEX_ML_CTRMTZ04_2_PRYSKORENNJA          3         
#define INDEX_ML_CTRMTZ04_2_PRYSKORENA            4          

#define N_BIT_CTRMTZ04_1                          (INDEX_ML_CTRMTZ04_1 - 0)          
#define CTR_MTZ04_1                               (1 << N_BIT_CTRMTZ04_1)          
#define N_BIT_CTRMTZ04_2                          (INDEX_ML_CTRMTZ04_2 - 0)        
#define CTR_MTZ04_2                               (1 << N_BIT_CTRMTZ04_2)          
#define N_BIT_CTRMTZ04_2_PRYSKORENNJA             (INDEX_ML_CTRMTZ04_2_PRYSKORENNJA - 1)         
#define CTR_MTZ04_2_PRYSKORENNJA                  (1 << N_BIT_CTRMTZ04_2_PRYSKORENNJA)      
#define N_BIT_CTRMTZ04_2_PRYSKORENA               (INDEX_ML_CTRMTZ04_2_PRYSKORENA - 1)          
#define CTR_MTZ04_2_PRYSKORENA                    (1 << N_BIT_CTRMTZ04_2_PRYSKORENA)          

#define CTR_MTZ04_MASKA                   (                                  \
                                           CTR_MTZ04_1                      |\
                                           CTR_MTZ04_2                      |\
                                           CTR_MTZ04_2_PRYSKORENNJA         |\
                                           CTR_MTZ04_2_PRYSKORENA            \
                                          )

enum 
{
  CTR_MTZ04 = 0,
	CTR_MTZ04_PRYSKORENNJA,
	CTR_MTZ04_PRYSKORENA,
  /*************************************/
  MTZ04_CONST_MENU_SETTINGS_LENGTH
};

enum __mtz04_tmr_const 
{
  INDEX_TIMER_MTZ04 = 0,
	
  INDEX_TIMER_MTZ04_DEPENDENT_A,
  INDEX_TIMER_MTZ04_DEPENDENT_B,
  INDEX_TIMER_MTZ04_DEPENDENT_C,
	
	INDEX_TIMER_MTZ04_PR,
	INDEX_TIMER_MTZ04_VVID_PR,
	
  /*************************************/
  NUMBER_LEVEL_OCP04_TMR_CONST
};

enum __mtz04_const 
{
  RANG_BLOCK_MTZ04 = 0,
  RANG_BLOCK_USK_MTZ04,
  RANG_PO_MTZ04,
  RANG_MTZ04,
  /*************************************/
  OCP04_SIGNALS_LENGTH
};

enum __mtz04_levels_const 
{
  OCP04_LEVEL1 = 0,
  OCP04_LEVEL2,
  /*************************************/
  NUMBER_LEVEL_OCP04
};

#endif
