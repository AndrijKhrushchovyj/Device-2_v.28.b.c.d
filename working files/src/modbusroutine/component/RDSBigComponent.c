//начальный регистр в карте памяти
#define BEGIN_ADR_REGISTER 15000
//конечный регистр в карте памяти
#define END_ADR_REGISTER 15001

#include "header.h"
//#include "cmd_log.h"

int privateRDSBigGetReg2(int adrReg);

int getRDSBigModbusRegister(int);//получить содержимое регистра
int getRDSBigModbusBit(int);//получить содержимое бита
int setRDSBigModbusRegister(int, int);//получить содержимое регистра
int setRDSBigModbusBit(int, int);//получить содержимое бита

int  postRDSBigWriteAction(void);//action после записи

SRAM1 COMPONENT_OBJ *rdsbigcomponent;

/**************************************/
//подготовка компонента рег статистики
/**************************************/
void constructorRDSBigComponent(COMPONENT_OBJ *rdsbigcomp)
{
  rdsbigcomponent = rdsbigcomp;

  rdsbigcomponent->getModbusRegister = getRDSBigModbusRegister;//получить содержимое регистра
  rdsbigcomponent->getModbusBit      = getRDSBigModbusBit;//получить содержимое бита
  rdsbigcomponent->setModbusRegister = setRDSBigModbusRegister;//получить содержимое регистра
  rdsbigcomponent->setModbusBit      = setRDSBigModbusBit;//получить содержимое бита

  rdsbigcomponent->postWriteAction = postRDSBigWriteAction;//action после записи
}//prepareDVinConfig

int getRDSBigModbusRegister(int adrReg)
{
  //получить содержимое регистра
  if(privateRDSBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  int offset = adrReg-BEGIN_ADR_REGISTER;

  switch(offset)
  {
    case 0://Очистить регистратор rds
      return MARKER_ERRORPERIMETR;
    case 1://Количество записей rds
      return  GetAmountFixElem()&0xFFFF;
  }//switch

  return 0;
}//getDOUTBigModbusRegister(int adrReg)
int getRDSBigModbusBit(int x)
{
  //получить содержимое регистра
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setRDSBigModbusRegister(int adrReg, int dataReg)
{
  //записать содержимое регистра
  if(privateRDSBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  superSetOperativMarker(rdsbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//записать в буфер

  int offset = adrReg-BEGIN_ADR_REGISTER;
  switch(offset)
  {
    case 0://Очистить аналоговый регистратор
      if(dataReg!=0x1412) return MARKER_ERRORDIAPAZON;
      break;
    case 1://Количество записей rds
      return MARKER_ERRORDIAPAZON;
  }//switch

  return 0;
}//getDOUTBigModbusRegister(int adrReg)
int setRDSBigModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //получить содержимое регистра
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

int postRDSBigWriteAction(void)
{
//action после записи
  int beginAdr = rdsbigcomponent->operativMarker[0];
  if(beginAdr<0) return 0;//не было записи
  int endAdr   = rdsbigcomponent->operativMarker[1];
  int countAdr = endAdr-beginAdr+1;
  if(endAdr<0) countAdr = 1;

  for(int i=0; i<countAdr; i++)
    {
      int offset = beginAdr-BEGIN_ADR_REGISTER+i;
      switch(offset)
      {
        case 0://Очистить регистратор rds
          if(
            ((clean_rejestrators & CLEAN_SR_ERR) != 0)
          ) return ERROR_VALID2;//ошибка валидации

          clean_rejestrators |= CLEAN_SR_ERR;
          break;
      }//switch
    }//for

  return 0;
}//

int privateRDSBigGetReg2(int adrReg)
{
  //проверить внешний периметр
  return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)

