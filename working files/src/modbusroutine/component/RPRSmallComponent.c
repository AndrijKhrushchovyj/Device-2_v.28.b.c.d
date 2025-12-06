#include "header.h"

//начальный регистр в карте памяти
#define BEGIN_ADR_REGISTER 16
//конечный регистр в карте памяти
#define END_ADR_REGISTER 99

int privateRPRSmallGetReg2(int adrReg);

int getRPRSmallModbusRegister(int);//получить содержимое регистра
int getRPRSmallModbusBit(int);//получить содержимое бита
int setRPRSmallModbusRegister(int, int);//получить содержимое регистра
int setRPRSmallModbusBit(int, int);//получить содержимое бита

int  postRPRSmallWriteAction(void);//action после записи
int getRPRSmallModbusBeginAdrRegister(void);
int getRPRSmallModbusEndAdrRegister(void);

SRAM1 COMPONENT_OBJ *rprsmallcomponent;

int getRPRSmallModbusBeginAdrRegister(void)
{
  //получить адрес нач регистра
 return BEGIN_ADR_REGISTER;
}//getRPRSmallModbusBeginAdrRegister
int getRPRSmallModbusEndAdrRegister(void)
{
  //получить адрес end регистра
 return END_ADR_REGISTER;
}//getRPRSmallModbusEndAdrRegister

/**************************************/
//подготовка компонента ранжирование пользовательских регистров
/**************************************/
void constructorRPRSmallComponent(COMPONENT_OBJ *rprsmallcomp)
{
  rprsmallcomponent = rprsmallcomp;

  rprsmallcomponent->getModbusRegister = getRPRSmallModbusRegister;//получить содержимое регистра
  rprsmallcomponent->getModbusBit      = getRPRSmallModbusBit;//получить содержимое бита
  rprsmallcomponent->setModbusRegister = setRPRSmallModbusRegister;//получить содержимое регистра
  rprsmallcomponent->setModbusBit      = setRPRSmallModbusBit;//получить содержимое бита

  rprsmallcomponent->postWriteAction = postRPRSmallWriteAction;//action после записи
}//prepareDVinConfig

int getRPRSmallModbusRegister(int adrReg)
{
  //получить содержимое регистра
  if(privateRPRSmallGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  int result = superReaderRegister(current_settings_interfaces.user_register[adrReg-BEGIN_ADR_REGISTER]);
  if(result&0xffff0000) return 0;

  return (result) &0xFFFF;
}//getDOUTModbusRegister(int adrReg)
int getRPRSmallModbusBit(int x)
{
  //получить содержимое bit
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setRPRSmallModbusRegister(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //записать содержимое регистра
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setRPRSmallModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //записать содержимое bit
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

int postRPRSmallWriteAction(void) {
//action после записи
  return 0;
}//

int privateRPRSmallGetReg2(int adrReg)
{
  //проверить внешний периметр
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)
