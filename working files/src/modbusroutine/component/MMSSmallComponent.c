
#include "header.h"
#include "variables_external_m.h"

//начальный регистр в карте памяти
#define BEGIN_ADR_REGISTER 450
//начальный bit в карте памяти
#define BEGIN_ADR_BIT 24000

//конечный регистр в карте памяти
#define END_ADR_REGISTER 451
//конечный bit в карте памяти
#define END_ADR_BIT 24031

int privateMMSSmallGetReg2(int adrReg);
int privateMMSSmallGetBit2(int adrBit);

int getMMSSmallModbusRegister(int);//получить содержимое регистра
int getMMSSmallModbusBit(int);//получить содержимое бита
int setMMSSmallModbusRegister(int, int);//записать регистр
int setMMSSmallModbusBit(int, int);//записать бит

int  postMMSSmallWriteAction(void);//action после записи

SRAM1 COMPONENT_OBJ *mmssmallcomponent;

/**************************************/
//подготовка компонента MMSSmall
/**************************************/
void constructorMMSSmallComponent(COMPONENT_OBJ *mmscomp)
{
  mmssmallcomponent = mmscomp;

//  mmssmallcomponent->getModbusRegister = getMMSSmallModbusRegister;//получить содержимое регистра
//  mmssmallcomponent->getModbusBit      = getMMSSmallModbusBit;//получить содержимое бита
//  mmssmallcomponent->setModbusRegister = setMMSSmallModbusRegister;// регистра
//  mmssmallcomponent->setModbusBit      = setMMSSmallModbusBit;// бита

//  mmssmallcomponent->postWriteAction = postMMSSmallWriteAction;//action после записи
}//constructorMMSSmallComponent(COMPONENT_OBJ *DVcomp)

int getMMSSmallModbusRegister(int adrReg) {
  //получить содержимое регистра
//  unsigned char arr_mms_block_outs [N_IN_MMS];
  if(privateMMSSmallGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

//  GetArrayMmsBlockOuts(arr_mms_block_outs);

  unsigned short temp = 0;//((unsigned short *)arr_mms_block_outs)[adrReg-BEGIN_ADR_REGISTER];
  return temp & 0xFFFF;
}//getMMSSmallModbusRegister(int adrReg)
int getMMSSmallModbusBit(int adrBit) {
  //получить содержимое bit
//  unsigned char arr_mms_block_outs [N_IN_MMS];
  if(privateMMSSmallGetBit2(adrBit)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

//  GetArrayMmsBlockOuts(arr_mms_block_outs);

  unsigned short tmp = 0;//((unsigned short *)arr_mms_block_outs)[(adrBit-BEGIN_ADR_BIT)/16];
  int maska = 1<<((adrBit-BEGIN_ADR_BIT)%16);
  if(tmp&maska) return 1;
  return 0;
}//getDVModbusBit(int adrReg)
int setMMSSmallModbusRegister(int x, int y) {
  UNUSED(x);
  UNUSED(y);
  //записать содержимое регистра
  return MARKER_OUTPERIMETR;
}//getDVModbusRegister(int adrReg)
int setMMSSmallModbusBit(int x, int y) {
  UNUSED(x);
  UNUSED(y);
  //получить содержимое регистра
  return MARKER_OUTPERIMETR;
}//getDVModbusRegister(int adrReg)

int postMMSSmallWriteAction(void) {
//action после записи
  return 0;
}//

int privateMMSSmallGetReg2(int adrReg)
{
  //проверить внешний периметр
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)

int privateMMSSmallGetBit2(int adrBit)
{
  //проверить внешний периметр
 return controlPerimetr(adrBit, BEGIN_ADR_BIT, END_ADR_BIT);
}//privateGetReg2(int adrReg)
