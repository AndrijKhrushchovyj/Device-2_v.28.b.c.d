
#include "header.h"

//начальный регистр в карте памяти
#define BEGIN_ADR_REGISTER 232
//начальный bit в карте памяти
#define BEGIN_ADR_BIT 232

//конечный регистр в карте памяти
#define END_ADR_REGISTER 233
//конечный bit в карте памяти
#define END_ADR_BIT 248

int privateSDISmallGetReg2(int adrReg);
int privateSDISmallGetBit2(int adrBit);

int getSDISmallModbusRegister(int);//получить содержимое регистра
int getSDISmallModbusBit(int);//получить содержимое бита
int setSDISmallModbusRegister(int, int);//записать регистр
int setSDISmallModbusBit(int, int);//записать бит

int  postSDISmallWriteAction(void);//action после записи

SRAM1 COMPONENT_OBJ *sdismallcomponent;

/**************************************/
//подготовка компонента ДВ
/**************************************/
void constructorSDISmallComponent(COMPONENT_OBJ *sdicomp)
{
  sdismallcomponent = sdicomp;

  sdismallcomponent->getModbusRegister = getSDISmallModbusRegister;//получить содержимое регистра
  sdismallcomponent->getModbusBit      = getSDISmallModbusBit;//получить содержимое бита
  sdismallcomponent->setModbusRegister = setSDISmallModbusRegister;// регистра
  sdismallcomponent->setModbusBit      = setSDISmallModbusBit;// бита

  sdismallcomponent->postWriteAction = postSDISmallWriteAction;//action после записи
}//constructorSDISmallComponent(COMPONENT_OBJ *SDIcomp)

int getSDISmallModbusRegister(int adrReg) {
  //получить содержимое регистра
  if(privateSDISmallGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  switch(adrReg-BEGIN_ADR_REGISTER)
  {
    case 0:
    return (state_leds) &0xFFFF;
    case 1:
    return (state_leds>>16)&0xFFFF;
  }//switch

  return 0;
}//getSDIModbusRegister(int adrReg)
int getSDISmallModbusBit(int adrBit) {
  //получить содержимое bit
  if(privateSDISmallGetBit2(adrBit)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  //state_leds = 0x10001;
  short tmp = state_leds &0xFFFF;
  if((adrBit-BEGIN_ADR_BIT)>=16) tmp = (state_leds>>16)&0xFFFF;
  short maska = 1<<((adrBit-BEGIN_ADR_BIT)%16);
  if(tmp&maska) return 1;
  return 0;
}//getSDIModbusBit(int adrReg)
int setSDISmallModbusRegister(int x, int y) {
  UNUSED(x);
  UNUSED(y);
  //записать содержимое регистра
  return MARKER_OUTPERIMETR;
}//getSDIModbusRegister(int adrReg)
int setSDISmallModbusBit(int x, int y) {
  UNUSED(x);
  UNUSED(y);
  //получить содержимое регистра
  return MARKER_OUTPERIMETR;
}//getSDIModbusRegister(int adrReg)

int postSDISmallWriteAction(void) {
//action после записи
  return 0;
}//

int privateSDISmallGetReg2(int adrReg)
{
  //проверить внешний периметр
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)

int privateSDISmallGetBit2(int adrBit)
{
  //проверить внешний периметр
 return controlPerimetr(adrBit, BEGIN_ADR_BIT, END_ADR_BIT);
}//privateGetReg2(int adrReg)
