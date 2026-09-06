#ifndef __CONST_WATCHDOG__
#define __CONST_WATCHDOG__

#define BIT_FOR_WATCHDOG_MEASURE_START_DATA_VAL_1 0
#define WATCHDOG_MEASURE_START_DATA_VAL_1 (1 << BIT_FOR_WATCHDOG_MEASURE_START_DATA_VAL_1)
#define BIT_FOR_WATCHDOG_MEASURE_START_DATA_VAL_2 1
#define WATCHDOG_MEASURE_START_DATA_VAL_2 (1 << BIT_FOR_WATCHDOG_MEASURE_START_DATA_VAL_2)
#define BIT_FOR_WATCHDOG_MEASURE_START_TEST_VAL 2
#define WATCHDOG_MEASURE_START_TEST_VAL (1 << BIT_FOR_WATCHDOG_MEASURE_START_TEST_VAL)
#define BIT_FOR_WATCHDOG_MEASURE_STOP_DATA_VAL_1 3
#define WATCHDOG_MEASURE_STOP_DATA_VAL_1 (1 << BIT_FOR_WATCHDOG_MEASURE_STOP_DATA_VAL_1)
#define BIT_FOR_WATCHDOG_MEASURE_STOP_DATA_VAL_2 4
#define WATCHDOG_MEASURE_STOP_DATA_VAL_2 (1 << BIT_FOR_WATCHDOG_MEASURE_STOP_DATA_VAL_2)
#define BIT_FOR_WATCHDOG_MEASURE_STOP_TEST_VAL 5
#define WATCHDOG_MEASURE_STOP_TEST_VAL (1 << BIT_FOR_WATCHDOG_MEASURE_STOP_TEST_VAL)
#define BIT_FOR_WATCHDOG_PROTECTION 6
#define WATCHDOG_PROTECTION (1 << BIT_FOR_WATCHDOG_PROTECTION)
#define BIT_FOR_WATCHDOG_KYYBOARD 7
#define WATCHDOG_KYYBOARD (1 << BIT_FOR_WATCHDOG_KYYBOARD)
#define BIT_FOR_WATCHDOG_DATAFLASH 8
#define WATCHDOG_DATAFLASH (1 << BIT_FOR_WATCHDOG_DATAFLASH)
#define BIT_FOR_WATCHDOG_RS485 9
#define WATCHDOG_RS485 (1 << BIT_FOR_WATCHDOG_RS485)

/*****************************************/
//ќб'Їднан≥ вс≥ б≥ти Watchdog у одну константу
/*****************************************/
#define UNITED_BITS_WATCHDOG (unsigned int) (WATCHDOG_MEASURE_START_DATA_VAL_1 | \
                                             WATCHDOG_MEASURE_STOP_DATA_VAL_1 |  \
                                             WATCHDOG_MEASURE_START_DATA_VAL_2 | \
                                             WATCHDOG_MEASURE_STOP_DATA_VAL_2 |  \
                                             WATCHDOG_MEASURE_START_TEST_VAL |   \
                                             WATCHDOG_MEASURE_STOP_TEST_VAL |    \
                                             WATCHDOG_PROTECTION |               \
                                             WATCHDOG_KYYBOARD |                 \
                                             WATCHDOG_DATAFLASH |                \
                                             WATCHDOG_RS485)

#define UNITED_BITS_WATCHDOG_SHORT (unsigned int) (WATCHDOG_MEASURE_START_DATA_VAL_1 | \
                                                   WATCHDOG_MEASURE_STOP_DATA_VAL_1 |  \
                                                   WATCHDOG_MEASURE_START_DATA_VAL_2 | \
                                                   WATCHDOG_MEASURE_STOP_DATA_VAL_2 |  \
                                                   WATCHDOG_MEASURE_START_TEST_VAL |   \
                                                   WATCHDOG_MEASURE_STOP_TEST_VAL |    \
                                                   WATCHDOG_PROTECTION |               \
                                                   WATCHDOG_KYYBOARD |                 \
                                                   WATCHDOG_DATAFLASH)
/*****************************************/

#endif
