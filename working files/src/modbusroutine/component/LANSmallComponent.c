
#include "header.h"
#include "variables_external_m.h"

//начальный регистр в карте памяти
#define BEGIN_ADR_REGISTER 466
//начальный bit в карте памяти
#define BEGIN_ADR_BIT 24256

//конечный регистр в карте памяти
#define END_ADR_REGISTER 467
//конечный bit в карте памяти
#define END_ADR_BIT 24287

int privateLANSmallGetReg2(int adrReg);
int privateLANSmallGetBit2(int adrBit);

int getLANSmallModbusRegister(int);//получить содержимое регистра
int getLANSmallModbusBit(int);//получить содержимое бита
int setLANSmallModbusRegister(int, int);//записать регистр
int setLANSmallModbusBit(int, int);//записать бит

int  postLANSmallWriteAction(void);//action после записи

SRAM1 COMPONENT_OBJ *lansmallcomponent;

/**************************************/
//подготовка компонента LANSmall
/**************************************/
void constructorLANSmallComponent(COMPONENT_OBJ *lancomp)
{
  lansmallcomponent = lancomp;

//  lansmallcomponent->getModbusRegister = getLANSmallModbusRegister;//получить содержимое регистра
//  lansmallcomponent->getModbusBit      = getLANSmallModbusBit;//получить содержимое бита
//  lansmallcomponent->setModbusRegister = setLANSmallModbusRegister;// регистра
//  lansmallcomponent->setModbusBit      = setLANSmallModbusBit;// бита

//  lansmallcomponent->postWriteAction = postLANSmallWriteAction;//action после записи
}//constructorLANSmallComponent(COMPONENT_OBJ *DVcomp)

int getLANSmallModbusRegister(int adrReg) {
  //получить содержимое регистра
//  unsigned char arr_lan_block_outs [N_OUT_LAN];
  if(privateLANSmallGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

//   GetArrayLanBlockOuts(arr_lan_block_outs);

  unsigned short temp = 0;//((unsigned short *)arr_lan_block_outs)[adrReg-BEGIN_ADR_REGISTER];
  return temp & 0xFFFF;
}//getDVModbusRegister(int adrReg)
int getLANSmallModbusBit(int adrBit) {
  //получить содержимое bit
//  unsigned char arr_lan_block_outs [N_OUT_LAN];
  if(privateLANSmallGetBit2(adrBit)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

//  GetArrayLanBlockOuts(arr_lan_block_outs);

  unsigned short tmp = 0;//((unsigned short *)arr_lan_block_outs)[(adrBit-BEGIN_ADR_BIT)/16];
  int maska = 1<<((adrBit-BEGIN_ADR_BIT)%16);
  if(tmp&maska) return 1;
  return 0;
}//getDVModbusBit(int adrReg)
int setLANSmallModbusRegister(int x, int y) {
  UNUSED(x);
  UNUSED(y);
  //записать содержимое регистра
  return MARKER_OUTPERIMETR;
}//getDVModbusRegister(int adrReg)
int setLANSmallModbusBit(int x, int y) {
  UNUSED(x);
  UNUSED(y);
  //получить содержимое регистра
  return MARKER_OUTPERIMETR;
}//getDVModbusRegister(int adrReg)

int postLANSmallWriteAction(void) {
//action после записи
  return 0;
}//

int privateLANSmallGetReg2(int adrReg)
{
  //проверить внешний периметр
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)

int privateLANSmallGetBit2(int adrBit)
{
  //проверить внешний периметр
 return controlPerimetr(adrBit, BEGIN_ADR_BIT, END_ADR_BIT);
}//privateGetReg2(int adrReg)
