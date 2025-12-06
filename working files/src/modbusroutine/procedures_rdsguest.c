#include "header.h"

int openRegistratorRDS(void);//Регистратор дискретных сигналов
int dataRegistratorRDS(int offsetRegister, int recordNumber, int recordLen);
int activeFunctionRDS(int offsetActiveFunc, int recordNumber);
void loadACMDSmallActualDataBit(int cmdSwitch, int beginOffset, int endOffset);

int openRegistratorRDS(void)
{
//валидация Регистратор дискретных сигналов
  return 0;//есть валидация
}//openRegistratorRDS

int dataRegistratorRDS(int offsetRegister, int recordNumber, int recordLen)
{
//данные Регистратор дискретных сигналов
      if(recordLen>47) return MARKER_ERRORPERIMETR;
      short cntRecord = GetAmountFixElem();
      if(recordNumber >= cntRecord) return MARKER_ERRORPERIMETR;

//  time_t time_dat_tmp = 0;
//  int32_t time_ms_tmp = 0;
  if(recordNumber_old != recordNumber)
  {
    long err = GetDateTimeLogElemPlWnum((unsigned int *)&time_dat_rds, recordNumber, ulWorkNumber_rds);//0xffffffff);
    if(err) return MARKER_ERRORPERIMETR;
    uint32_t* tmp;
    tmp = &((uint32_t*)&time_dat_rds)[2];
    ulWorkNumber_rds = (unsigned long)(*tmp);
  
    err = GetMsLogElemPlWnum      ((unsigned int *)&time_ms_rds , recordNumber, ulWorkNumber_rds);//0xffffffff);
    if(err) return MARKER_ERRORPERIMETR;
    tmp = &((uint32_t*)&time_ms_rds)[1];
    ulWorkNumber_rds = (unsigned long)(*tmp);
  }//if
      
  switch(offsetRegister)
    {
     case 0://Время, Unix-время
     case 1:
     case 2:
     case 3:
       {
       uint16_t* tmp[4];
       tmp[0] = &((uint16_t*)&time_dat_rds)[0];
       tmp[1] = &((uint16_t*)&time_dat_rds)[1];
       tmp[2] = &((uint16_t*)&time_dat_rds)[2];
       tmp[3] = &((uint16_t*)&time_dat_rds)[3];
       return (*(tmp[offsetRegister]));
       }
     case 4:
     case 5:
       {
       uint16_t* tmp[2];
       tmp[0] = &((uint16_t*)&time_ms_rds)[0];
       tmp[1] = &((uint16_t*)&time_ms_rds)[1];
       return *(tmp[offsetRegister-4]);
       }
    }//switch

  return activeFunctionRDS(offsetRegister-6, recordNumber);//Активные функции RDS
}//dataRegistratorRDS

int activeFunctionRDS(int offsetActiveFunc, int recordNumber)
{
//Активные функции RDS
  if(offsetActiveFunc>41) return MARKER_ERRORPERIMETR;

  if(recordNumber_old != recordNumber)
  {
   long err = GetCmdPlusTimeLogElemPlWnum(rds_functions, recordNumber, ulWorkNumber_rds);//0xffffffff);
   if(err) return MARKER_ERRORPERIMETR;
   uint32_t tmp;
   tmp = rds_functions[N_BIG];
   ulWorkNumber_rds = (unsigned long)tmp;

   recordNumber_old = recordNumber;
  }//if

  int cntReg = 1;
  int beginOffset = offsetActiveFunc*16;
  int endOffset   = beginOffset +cntReg*16;

  loadACMDSmallActualDataBit(2, beginOffset, endOffset); //RDSData

  return tempReadArray[offsetActiveFunc];
}//activeFunctionRDS(int offsetActiveFunc, int recordNumber)

