#ifndef __CONST_I2C__
#define __CONST_I2C__

#define I2C_EVENT_MASTER_BYTE_RECEIVED_NO_BUSY            ((uint32_t)0x00000040)  /* RxNE */

#define MAX_NUMBER_BUSY_STATE     10

#define ERROR_TOTAL_BIT           0
#define ERROR_UNEXPECTED_BIT      1
#define ERROR_FIX_ERRORS_BIT      2

//#define CLOCKSPEED_1MBIT      1000000
#define CLOCKSPEED            400000

#define START_ADDRESS_TIME_REGISTERS              0x0
#define MAX_NUMBER_REGISTERS_RTC                  20

#define SIZE_BUFFER_FOR_I2C                  MAX_NUMBER_REGISTERS_RTC


#define TASK_BLK_OPERATION_BIT                                          0      

#define TASK_START_READ_RTC_BIT                                         1      
#define TASK_READING_RTC_BIT                                            2     

#define TASK_START_WRITE_RTC_BIT                                        3      
#define TASK_WRITING_RTC_BIT                                            4      

#define TASK_SET_START_SETTINGS_RTC_BIT                                 5      
#define TASK_RESET_ST_RTC_BIT                                           6      
#define TASK_RESET_OF_RTC_BIT                                           7      

#define STATE_FIRST_READING_RTC_BIT                                     30      
#define STATE_FIRST_READING_RTC                                         (1<<STATE_FIRST_READING_RTC_BIT)    

#define ETAP_CLEAR_OF_NONE                                      -1                                            //-1
#define ETAP_CLEAR_OF_ST_STOP                                   0                                             //0
#define ETAP_CLEAR_OF_ST_CLEAR                                  (ETAP_CLEAR_OF_ST_STOP + 1)                   //1
#define ETAP_CLEAR_OF_ST_START_WAITING_5S                       (ETAP_CLEAR_OF_ST_CLEAR + 1)                  //2
#define ETAP_CLEAR_OF_ST_WAITING_5S                             (ETAP_CLEAR_OF_ST_START_WAITING_5S + 1)       //3
#define ETAP_CLEAR_OF_WRITE_OF_0                                (ETAP_CLEAR_OF_ST_WAITING_5S + 1)             //4

#endif
