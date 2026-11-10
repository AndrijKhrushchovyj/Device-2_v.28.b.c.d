//начальный регистр в карте памяти
#define BEGIN_ADR_REGISTER 3000
//конечный регистр в карте памяти
#define END_ADR_REGISTER 3012

#include "header.h"
#include "variables_external_m.h"

int privatePKVBigGetReg2(int adrReg);
int getPKVBigModbusRegister(int);//получить содержимое регистра
int getPKVBigModbusBit(int);//получить содержимое бита
int setPKVBigModbusRegister(int, int);//получить содержимое регистра
int setPKVBigModbusBit(int, int);//получить содержимое бита

int  postPKVBigWriteAction(void);//action после записи
int PKVFunc000(int inOffset, int regPKV, uint32_t **editValue);
int getPKVBigModbusRegister_guest(int adrReg);
int postPKVBigWriteAction_guest(int countAdr, int beginAdr);

SRAM1 COMPONENT_OBJ *pkvbigcomponent;
/**************************************/
//подготовка компонента PKV
/**************************************/
void constructorPKVBigComponent(COMPONENT_OBJ *pkvbigcomp)
{
  pkvbigcomponent = pkvbigcomp;

//  pkvbigcomponent->getModbusRegister = getPKVBigModbusRegister;//получить содержимое регистра
//  pkvbigcomponent->getModbusBit      = getPKVBigModbusBit;//получить содержимое бита
//  pkvbigcomponent->setModbusRegister = setPKVBigModbusRegister;//получить содержимое регистра
//  pkvbigcomponent->setModbusBit      = setPKVBigModbusBit;//получить содержимое бита

//  pkvbigcomponent->postWriteAction = postPKVBigWriteAction;//action после записи
}//prepareDVinConfig

int getPKVBigModbusRegister(int adrReg)
{
  //получить содержимое регистра
  if(privatePKVBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(pkvbigcomponent->isActiveActualData)
  {
    edition_settings = current_settings_interfaces;//делаем копию
  }//if(uprbigcomponent->isActiveActualData)
  superClearActiveActualData();
  return getPKVBigModbusRegister_guest(adrReg);
}//getDOUTBigModbusRegister(int adrReg)
int getPKVBigModbusBit(int x)
{
  //получить содержимое регистра
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setPKVBigModbusRegister(int adrReg, int dataReg)
{
  //записать содержимое регистра
  if(privatePKVBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(pkvbigcomponent->isActiveActualData)
  {
    edition_settings = current_settings_interfaces;//делаем копию
  }//if(uprbigcomponent->isActiveActualData)
  superClearActiveActualData();

  superSetOperativMarker(pkvbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//записать в буфер

  uint32_t *editValue=NULL;
  if(!PKVFunc000(adrReg-BEGIN_ADR_REGISTER, dataReg, &editValue)) return MARKER_ERRORDIAPAZON;

  return 0;
}//getDOUTBigModbusRegister(int adrReg)
int setPKVBigModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //получить содержимое регистра
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

int postPKVBigWriteAction(void)
{
//action после записи
  int beginAdr = pkvbigcomponent->operativMarker[0];
  if(beginAdr<0) return 0;//не было записи
  int endAdr   = pkvbigcomponent->operativMarker[1];
  int countAdr = endAdr-beginAdr+1;
  if(endAdr<0) countAdr = 1;

//  struct tm *p;
  //запрещенные начальные адреса пакета
  switch(beginAdr)
  {
   case 3001: case 3002: case 3003: case 3005: return ERROR_VALID2;
  }//switch
  //запрещенные конечные адреса пакета
  switch(endAdr)
  {
   case 3000: case 3001: case 3002: case 3004: return ERROR_VALID2;
  }//switch

  return postPKVBigWriteAction_guest(countAdr, beginAdr);
}//

int privatePKVBigGetReg2(int adrReg)
{
  //проверить внешний периметр
  return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)

