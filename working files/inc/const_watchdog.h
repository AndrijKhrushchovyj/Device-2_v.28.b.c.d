#ifndef __CONST_WATCHDOG__
#define __CONST_WATCHDOG__

#define BIT_FOR_WATCHDOG_MEASURE_START_DATA_VAL   0
#define WATCHDOG_MEASURE_START_DATA_VAL           (1 << BIT_FOR_WATCHDOG_MEASURE_START_DATA_VAL)
//#define BIT_FOR_WATCHDOG_MEASURE_START_TEST_VAL   1
//#define WATCHDOG_MEASURE_START_TEST_VAL           (1 << BIT_FOR_WATCHDOG_MEASURE_START_TEST_VAL)
#define BIT_FOR_WATCHDOG_MEASURE_STOP_DATA_VAL    2
#define WATCHDOG_MEASURE_STOP_DATA_VAL            (1 << BIT_FOR_WATCHDOG_MEASURE_STOP_DATA_VAL)
//#define BIT_FOR_WATCHDOG_MEASURE_STOP_TEST_VAL    3
//#define WATCHDOG_MEASURE_STOP_TEST_VAL            (1 << BIT_FOR_WATCHDOG_MEASURE_STOP_TEST_VAL)
#define BIT_FOR_WATCHDOG_PROTECTION               4
#define WATCHDOG_PROTECTION                       (1 << BIT_FOR_WATCHDOG_PROTECTION)
#define BIT_FOR_WATCHDOG_KYYBOARD                 5
#define WATCHDOG_KYYBOARD                         (1 << BIT_FOR_WATCHDOG_KYYBOARD)
#define BIT_FOR_WATCHDOG_DATAFLASH                6
#define WATCHDOG_DATAFLASH                        (1 << BIT_FOR_WATCHDOG_DATAFLASH)
#define BIT_FOR_WATCHDOG_RS485                    7
#define WATCHDOG_RS485                            (1 << BIT_FOR_WATCHDOG_RS485)

/*****************************************/
//ќб'Їднан≥ вс≥ б≥ти Watchdog у одну константу
/*****************************************/
#define UNITED_BITS_WATCHDOG (unsigned int)(\
     WATCHDOG_MEASURE_START_DATA_VAL                      \
   | WATCHDOG_MEASURE_STOP_DATA_VAL                       \
   /*| WATCHDOG_MEASURE_START_TEST_VAL */                     \
   /*| WATCHDOG_MEASURE_STOP_TEST_VAL  */                     \
   | WATCHDOG_PROTECTION                                  \
   | WATCHDOG_KYYBOARD                                    \
   | WATCHDOG_DATAFLASH                                   \
   | WATCHDOG_RS485                                       \
)     

#define UNITED_BITS_WATCHDOG_SHORT (unsigned int)(\
     WATCHDOG_MEASURE_START_DATA_VAL                      \
   | WATCHDOG_MEASURE_STOP_DATA_VAL                       \
   /*| WATCHDOG_MEASURE_START_TEST_VAL */                     \
   /*| WATCHDOG_MEASURE_STOP_TEST_VAL  */                     \
   | WATCHDOG_PROTECTION                                  \
   | WATCHDOG_KYYBOARD                                    \
   | WATCHDOG_DATAFLASH                                   \
)     
/*****************************************/


#endif
