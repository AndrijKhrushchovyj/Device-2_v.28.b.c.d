
#include "header.h"
#include "variables_external_m.h"

#define TMP_OUTPUTS (tmp = 0)

//начальный регистр в карте памяти
#define BEGIN_ADR_REGISTER 200
//начальный bit в карте памяти
#define BEGIN_ADR_BIT 20000

//конечный регистр в карте памяти
#define END_ADR_REGISTER 200
//конечный bit в карте памяти
#define END_ADR_BIT 20015


int privateDOUTSmallGetReg2(int adrReg);
int privateDOUTSmallGetBit2(int adrBit);

int getDOUTSmallModbusRegister(int);//получить содержимое регистра
int getDOUTSmallModbusBit(int);//получить содержимое бита
int setDOUTSmallModbusRegister(int, int);//записать регистр
int setDOUTSmallModbusBit(int, int);//записать бит

int  postDOUTSmallWriteAction(void);//action после записи

SRAM1 COMPONENT_OBJ *doutsmallcomponent;

/**************************************/
//подготовка компонента ДВ
/**************************************/
void constructorDOUTSmallComponent(COMPONENT_OBJ *doutcomp)
{
  doutsmallcomponent = doutcomp;

//  doutsmallcomponent->getModbusRegister = getDOUTSmallModbusRegister;//получить содержимое регистра
//  doutsmallcomponent->getModbusBit      = getDOUTSmallModbusBit;//получить содержимое бита
//  doutsmallcomponent->setModbusRegister = setDOUTSmallModbusRegister;// регистра
//  doutsmallcomponent->setModbusBit      = setDOUTSmallModbusBit;// бита

//  doutsmallcomponent->postWriteAction = postDOUTSmallWriteAction;//action после записи
}//constructorDOUTSmallComponent(COMPONENT_OBJ *doutcomp)

int getDOUTSmallModbusRegister(int adrReg) {
  //получить содержимое регистра
  if(privateDOUTSmallGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  int tmp   = state_outputs_raw&((1<<NUMBER_SIMPLE_OUTPUTS)-1);

  return tmp;
}//getDOUTModbusRegister(int adrReg)

int getDOUTSmallModbusBit(int adrBit) {
  //получить содержимое bit
  if(privateDOUTSmallGetBit2(adrBit)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  int tmp = state_outputs_raw&((1<<NUMBER_SIMPLE_OUTPUTS)-1);

  int maska = 1<<((adrBit-BEGIN_ADR_BIT)%16);

  if(tmp&maska) return 1;
  return 0;
}//getDOUTModbusBit(int adrReg)
int setDOUTSmallModbusRegister(int x, int y) {
  UNUSED(x);
  UNUSED(y);
  //записать содержимое регистра
  return MARKER_OUTPERIMETR;
}//getDOUTModbusRegister(int adrReg)
int setDOUTSmallModbusBit(int x, int y) {
  UNUSED(x);
  UNUSED(y);
  //получить содержимое регистра
  return MARKER_OUTPERIMETR;
}//getDOUTModbusRegister(int adrReg)

int postDOUTSmallWriteAction(void) {
//action после записи
  return 0;
}//

int privateDOUTSmallGetReg2(int adrReg)
{
  //проверить внешний периметр
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)

int privateDOUTSmallGetBit2(int adrBit)
{
  //проверить внешний периметр
 return controlPerimetr(adrBit, BEGIN_ADR_BIT, END_ADR_BIT);
}//privateGetReg2(int adrReg)
