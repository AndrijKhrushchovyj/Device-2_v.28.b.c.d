#ifndef __CONST_MENU_MEASURMENT__
#define __CONST_MENU_MEASURMENT__

#define EKRAN_MEASURMENT                (EKRAN_COFIGURATION + 1)
#define EKRAN_MEASURMENT_CURRENT        (EKRAN_MEASURMENT + 1)
#define EKRAN_MEASURMENT_VOLTAGE_TYPE   (EKRAN_MEASURMENT_CURRENT + 1)
#define EKRAN_MEASURMENT_VOLTAGE_PHASE  (EKRAN_MEASURMENT_VOLTAGE_TYPE + 1)
#define EKRAN_MEASURMENT_VOLTAGE_LINE   (EKRAN_MEASURMENT_VOLTAGE_PHASE + 1)
#define EKRAN_MEASURMENT_FREQUENCY      (EKRAN_MEASURMENT_VOLTAGE_LINE + 1)
#define EKRAN_MEASURMENT_ANGLE          (EKRAN_MEASURMENT_FREQUENCY + 1)
#define EKRAN_MEASURMENT_POWER          (EKRAN_MEASURMENT_ANGLE + 1)
#define EKRAN_MEASURMENT_RESISTANCE     (EKRAN_MEASURMENT_POWER + 1)
#define EKRAN_ENERGY                    (EKRAN_MEASURMENT_RESISTANCE + 1)


#define MAX_ROW_FOR_MEASURMENT                  6
#define MAX_ROW_FOR_MEASURMENT_VOLTAGE_TYPE     2
#define MAX_ROW_FOR_EKRAN_ENERGY                MAX_NUMBER_INDEXES_ENERGY

#define INDEX_ML_MEASURMENT_CURRENT     0
#define INDEX_ML_MEASURMENT_VOLTAGE     1
#define INDEX_ML_MEASURMENT_FREQUENCY   2
#define INDEX_ML_MEASURMENT_ANGLE       3
#define INDEX_ML_MEASURMENT_POWER       4
#define INDEX_ML_MEASURMENT_RESISTANCE  5


#define INDEX_ML_MEASURMENT_VOLTAGE_PHASE       0
#define INDEX_ML_MEASURMENT_VOLTAGE_LINE        1

#define MAX_ROW_FOR_MEASURMENT_CURRENT          (4 + 3 + 2 + 1 + 3)
#define MAX_ROW_FOR_MEASURMENT_VOLTAGE_PHASE    (3 + 1 + 2)
#define MAX_ROW_FOR_MEASURMENT_VOLTAGE_LINE     3
#define MAX_ROW_FOR_MEASURMENT_FREQUENCY        1
#define MAX_ROW_FOR_MEASURMENT_ANGLE            FULL_ORT_MAX
#define MAX_ROW_FOR_MEASURMENT_POWER            4
#define MAX_ROW_FOR_MEASURMENT_RESISTANCE       MAX_NUMBER_INDEXES_RESISTANCE

#define INDEX_ML_3I0_i                  0
#define INDEX_ML_3I0                    1
#define INDEX_ML_3I0_other_g            2
#define INDEX_ML_3I0_r                  3
#define INDEX_ML_Ia                     4
#define INDEX_ML_Ib                     5
#define INDEX_ML_Ic                     6
#define INDEX_ML_I2                     7
#define INDEX_ML_I1                     8

#define INDEX_ML_Ua                     0
#define INDEX_ML_Ub                     1
#define INDEX_ML_Uc                     2
#define INDEX_ML_Uab_TN2                3

#define INDEX_ML_F_val_1                0
#define INDEX_ML_F_val_2                1

#define INDEX_ML_P                      0
#define INDEX_ML_Q                      1
#define INDEX_ML_S                      2
#define INDEX_ML_COS_PHI                3

#endif
