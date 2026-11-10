
//начальный регистр в карте памяти
#define BEGIN_ADR_REGISTER 1000
//конечный регистр в карте памяти
#define END_ADR_REGISTER 1637

#include "header.h"
#include "variables_external_m.h"

int getAISmallModbusRegister(int);//получить содержимое регистра
int getAISmallModbusBit(int);//получить содержимое бита
int setAISmallModbusRegister(int, int);//получить содержимое регистра
int setAISmallModbusBit(int, int);//получить содержимое бита

int  postAISmallWriteAction(void);//action после записи
void loadAISmallActualData(void);

int privateAISmallGetReg2(int adrReg);

int getAISmallModbusRegister_guest(int offset);

SRAM1 COMPONENT_OBJ *aismallcomponent;

/**************************************/
//компонент измерений
/**************************************/
void constructorAISmallComponent(COMPONENT_OBJ *aismallcomp)
{
  aismallcomponent = aismallcomp;

//  aismallcomponent->getModbusRegister = getAISmallModbusRegister;//получить содержимое регистра
//  aismallcomponent->getModbusBit      = getAISmallModbusBit;//получить содержимое бита
//  aismallcomponent->setModbusRegister = setAISmallModbusRegister;//получить содержимое регистра
//  aismallcomponent->setModbusBit      = setAISmallModbusBit;//получить содержимое бита

//  aismallcomponent->postWriteAction = postAISmallWriteAction;//action после записи
}//constructorIUSmallComponent(COMPONENT_OBJ *iucomp)

int getAISmallModbusRegister(int adrReg)
{
  //получить содержимое регистра
  if(privateAISmallGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  return getAISmallModbusRegister_guest(adrReg-BEGIN_ADR_REGISTER);
}//getDVModbusRegister(int adrReg)
int getAISmallModbusBit(int x) {
  //получить содержимое bit
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getIUModbusBit(int )
int setAISmallModbusRegister(int x, int y) {
  UNUSED(x);
  UNUSED(y);
  //записать содержимое регистра
  return MARKER_OUTPERIMETR;
}//setIUModbusRegister(int, int)
int setAISmallModbusBit(int x, int y) {
  UNUSED(x);
  UNUSED(y);
  //записать содержимое bit
  return MARKER_OUTPERIMETR;
}//setIUModbusBit(int, int )

int postAISmallWriteAction(void) {
//action после записи
 return 0;
}//

int privateAISmallGetReg2(int adrReg)
{
  //проверить внешний периметр
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)

