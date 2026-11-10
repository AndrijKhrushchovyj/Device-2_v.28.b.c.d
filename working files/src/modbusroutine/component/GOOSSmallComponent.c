
//начальный регистр в карте памяти
#define BEGIN_ADR_REGISTER 400
//начальный bit в карте памяти
#define BEGIN_ADR_BIT 23200

//конечный регистр в карте памяти
#define END_ADR_REGISTER 407
//конечный bit в карте памяти
#define END_ADR_BIT 23327

#include "header.h"
#include "variables_external_m.h"

int privateGOOSSmallGetReg2(int adrReg);
int privateGOOSSmallGetBit2(int adrBit);

int getGOOSSmallModbusRegister(int);//получить содержимое регистра
int getGOOSSmallModbusBit(int);//получить содержимое бита
int setGOOSSmallModbusRegister(int, int);//записать регистр
int setGOOSSmallModbusBit(int, int);//записать бит

int  postGOOSSmallWriteAction(void);//action после записи

SRAM1 COMPONENT_OBJ *goossmallcomponent;

/**************************************/
//подготовка компонента GOOSSmall
/**************************************/
void constructorGOOSSmallComponent(COMPONENT_OBJ *gooscomp)
{
  goossmallcomponent = gooscomp;

//  goossmallcomponent->getModbusRegister = getGOOSSmallModbusRegister;//получить содержимое регистра
//  goossmallcomponent->getModbusBit      = getGOOSSmallModbusBit;//получить содержимое бита
//  goossmallcomponent->setModbusRegister = setGOOSSmallModbusRegister;// регистра
//  goossmallcomponent->setModbusBit      = setGOOSSmallModbusBit;// бита

//  goossmallcomponent->postWriteAction = postGOOSSmallWriteAction;//action после записи
}//constructorGOOSSmallComponent(COMPONENT_OBJ *DVcomp)

int getGOOSSmallModbusRegister(int adrReg) {
  //получить содержимое регистра
//  unsigned char arr_goose_block_Outs [N_IN_GOOSE];
  if(privateGOOSSmallGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

//  GetArrayGooseBlockOuts(arr_goose_block_Outs);

  unsigned short temp = 0;//((unsigned short *)arr_goose_block_Outs)[adrReg-BEGIN_ADR_REGISTER];
  return temp & 0xFFFF;
}//getDVModbusRegister(int adrReg)
int getGOOSSmallModbusBit(int adrBit) {
  //получить содержимое bit
//  unsigned char arr_goose_block_Outs [N_IN_GOOSE];
  if(privateGOOSSmallGetBit2(adrBit)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

//  GetArrayGooseBlockOuts(arr_goose_block_Outs);

  unsigned short tmp = 0;//((unsigned short *)arr_goose_block_Outs)[(adrBit-BEGIN_ADR_BIT)/16];
  int maska = 1<<((adrBit-BEGIN_ADR_BIT)%16);
  if(tmp&maska) return 1;
  return 0;
}//getDVModbusBit(int adrReg)
int setGOOSSmallModbusRegister(int x, int y) {
  UNUSED(x);
  UNUSED(y);
  //записать содержимое регистра
  return MARKER_OUTPERIMETR;
}//getDVModbusRegister(int adrReg)
int setGOOSSmallModbusBit(int x, int y) {
  UNUSED(x);
  UNUSED(y);
  //получить содержимое регистра
  return MARKER_OUTPERIMETR;
}//getDVModbusRegister(int adrReg)

int postGOOSSmallWriteAction(void) {
//action после записи
  return 0;
}//

int privateGOOSSmallGetReg2(int adrReg)
{
  //проверить внешний периметр
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)

int privateGOOSSmallGetBit2(int adrBit)
{
  //проверить внешний периметр
 return controlPerimetr(adrBit, BEGIN_ADR_BIT, END_ADR_BIT);
}//privateGetReg2(int adrReg)
