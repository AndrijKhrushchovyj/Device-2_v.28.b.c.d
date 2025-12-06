#ifndef __GLOBAL_VARIABLES_M_H
#define __GLOBAL_VARIABLES_M_H

#include "header_m.h"

SRAM1 COMPONENT_OBJ config_array[TOTAL_COMPONENT];
SRAM1 short tempReadArray[130];
SRAM1 short tempWriteArray[MAXIMUMTW];
SRAM1 int   indexTW;//индекс буфера записи
SRAM1 int   pointInterface;//метка интерфейса 0-USB 1-RS485

SRAM1 unsigned int rds_functions[N_BIG+1];//для рег статистики
SRAM1 int recordNumber_old;
SRAM1 time_t time_dat_rds[2];
SRAM1 int32_t time_ms_rds[2];
SRAM1 unsigned long ulWorkNumber_rds;

#endif
