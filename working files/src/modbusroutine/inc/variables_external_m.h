#ifndef __GLOBAL_EXTERNAL_M_H
#define __GLOBAL_EXTERNAL_M_H

#include "constants.h"
#include "fatfs.h"
#include "libraries.h"
//#include "variables_global.h"
//#include "functions_global.h"

#include "header_m.h"

extern  COMPONENT_OBJ config_array[];
extern  short tempReadArray[];
extern  short tempWriteArray[];
extern  int   indexTW;//индекс буфера записи
extern  int   pointInterface;//метка интерфейса 0-USB 1-RS485

extern  unsigned int rds_functions[];//для рег статистики
extern  int recordNumber_old;
extern  time_t time_dat_rds;
extern  int32_t time_ms_rds;
extern  unsigned long ulWorkNumber_rds;


//для валидации ранжир источников входов-выходов триггеров
#define SOURCEMARKER_DTR  10
//для валидации ранжир источников входов ОФ
#define SOURCEMARKER_MFT  20

//закоментируйте для боевого применения
//#define TESTZBIRKA_VERSII_PZ



#endif
