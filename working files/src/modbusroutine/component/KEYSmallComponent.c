
#include "header.h"
#include "variables_external_m.h"

//начальный регистр в карте памяти
#define BEGIN_ADR_REGISTER 395
//начальный bit в карте памяти
#define BEGIN_ADR_BIT 23120

//конечный регистр в карте памяти
#define END_ADR_REGISTER 395
//конечный bit в карте памяти
#define END_ADR_BIT 23125

int privateKEYSmallGetReg2(int adrReg);
int privateKEYSmallGetBit2(int adrBit);

int getKEYSmallModbusRegister(int);//получить содержимое регистра
int getKEYSmallModbusBit(int);//получить содержимое бита
int setKEYSmallModbusRegister(int, int);//записать регистр
int setKEYSmallModbusBit(int, int);//записать бит

int  postKEYSmallWriteAction(void);//action после записи

SRAM1 COMPONENT_OBJ *keysmallcomponent;

/**************************************/
//подготовка компонента KEY
/**************************************/
void constructorKEYSmallComponent(COMPONENT_OBJ *keycomp)
{
  keysmallcomponent = keycomp;

//  keysmallcomponent->getModbusRegister = getKEYSmallModbusRegister;//получить содержимое регистра
//  keysmallcomponent->getModbusBit      = getKEYSmallModbusBit;//получить содержимое бита
//  keysmallcomponent->setModbusRegister = setKEYSmallModbusRegister;// регистра
//  keysmallcomponent->setModbusBit      = setKEYSmallModbusBit;// бита

//  keysmallcomponent->postWriteAction = postKEYSmallWriteAction;//action после записи
}//constructorKEYSmallComponent(COMPONENT_OBJ *KEYcomp)

int getKEYSmallModbusRegister(int adrReg) {
  //получить содержимое регистра
  if(privateKEYSmallGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  int tmp   = 0;//state_FK & ((1<<NUMBER_DEFINED_BUTTONS)-1);

  switch(adrReg-BEGIN_ADR_REGISTER)
  {
   case 0:  tmp &= 0xFFFF; break;
   default: tmp = 0;
  }//switch

  return tmp;
}//getKEYModbusRegister(int adrReg)
int getKEYSmallModbusBit(int adrBit) {
  //получить содержимое bit
  if(privateKEYSmallGetBit2(adrBit)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  int tmp   = 0;//state_FK & ((1<<NUMBER_DEFINED_BUTTONS)-1);

  int offset = adrBit-BEGIN_ADR_BIT;
  switch(offset/16)
  {
   case 0:  tmp &= 0xFFFF; break;
   default: tmp = 0;
  }//switch

  int maska = 1<<((adrBit-BEGIN_ADR_BIT)%16);
  if(tmp&maska) return 1;
  return 0;
}//getKEYModbusBit(int adrReg)
int setKEYSmallModbusRegister(int x, int y) {
  UNUSED(x);
  UNUSED(y);
  //записать содержимое регистра
  return MARKER_OUTPERIMETR;
}//getKEYModbusRegister(int adrReg)
int setKEYSmallModbusBit(int x, int y) {
  UNUSED(x);
  UNUSED(y);
  //получить содержимое регистра
  return MARKER_OUTPERIMETR;
}//getKEYModbusRegister(int adrReg)

int postKEYSmallWriteAction(void) {
//action после записи
  return 0;
}//

int privateKEYSmallGetReg2(int adrReg)
{
  //проверить внешний периметр
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)

int privateKEYSmallGetBit2(int adrBit)
{
  //проверить внешний периметр
 return controlPerimetr(adrBit, BEGIN_ADR_BIT, END_ADR_BIT);
}//privateGetReg2(int adrReg)
