
#include "header.h"

//начальный регистр в карте памяти
#define BEGIN_ADR_REGISTER 200
//начальный bit в карте памяти
#define BEGIN_ADR_BIT 200

#if (                                   \
     (MODYFIKACIA_VERSII_PZ == 2) ||    \
     (MODYFIKACIA_VERSII_PZ == 4) ||    \
     (MODYFIKACIA_VERSII_PZ == 12)||    \
     (MODYFIKACIA_VERSII_PZ == 14)||    \
     (MODYFIKACIA_VERSII_PZ == 24)||    \
     (MODYFIKACIA_VERSII_PZ == 34)      \
    )   
//конечный регистр в карте памяти
#define END_ADR_REGISTER 200
//конечный bit в карте памяти
#define END_ADR_BIT 207
#endif

#if (                                   \
     (MODYFIKACIA_VERSII_PZ == 6) ||    \
     (MODYFIKACIA_VERSII_PZ == 26)      \
    )   
//конечный регистр в карте памяти
#define END_ADR_REGISTER 200
//конечный bit в карте памяти
#define END_ADR_BIT 211
#endif

#if (                                   \
     (MODYFIKACIA_VERSII_PZ == 7) ||    \
     (MODYFIKACIA_VERSII_PZ == 17)      \
    )   
//конечный регистр в карте памяти
#define END_ADR_REGISTER 200
//конечный bit в карте памяти
#define END_ADR_BIT 212
#endif

#if (                                   \
     (MODYFIKACIA_VERSII_PZ == 1) ||    \
     (MODYFIKACIA_VERSII_PZ == 3) ||    \
     (MODYFIKACIA_VERSII_PZ == 11) ||    \
     (MODYFIKACIA_VERSII_PZ == 13) ||    \
     (MODYFIKACIA_VERSII_PZ == 23) ||    \
     (MODYFIKACIA_VERSII_PZ == 33)      \
    )   
//конечный регистр в карте памяти
#define END_ADR_REGISTER 200
//конечный bit в карте памяти
#define END_ADR_BIT 215
#endif

#if (                                   \
     (MODYFIKACIA_VERSII_PZ == 0) ||    \
     (MODYFIKACIA_VERSII_PZ == 10)||    \
     (MODYFIKACIA_VERSII_PZ ==  5)||    \
     (MODYFIKACIA_VERSII_PZ == 15)    \
    )                                   
//конечный регистр в карте памяти
#define END_ADR_REGISTER 201
//конечный bit в карте памяти
#define END_ADR_BIT 219
#endif

#if (                                   \
     (MODYFIKACIA_VERSII_PZ ==  8) ||   \
     (MODYFIKACIA_VERSII_PZ == 18)      \
    )                                   
//конечный регистр в карте памяти
#define END_ADR_REGISTER 201
//конечный bit в карте памяти
#define END_ADR_BIT 231
#endif

int privateDVSmallGetReg2(int adrReg);
int privateDVSmallGetBit2(int adrBit);

int getDVSmallModbusRegister(int);//получить содержимое регистра
int getDVSmallModbusBit(int);//получить содержимое бита
int setDVSmallModbusRegister(int, int);//записать регистр
int setDVSmallModbusBit(int, int);//записать бит

int  postDVSmallWriteAction(void);//action после записи

SRAM1 COMPONENT_OBJ *dvsmallcomponent;

/**************************************/
//подготовка компонента ДВ
/**************************************/
void constructorDVSmallComponent(COMPONENT_OBJ *dvcomp)
{
  dvsmallcomponent = dvcomp;

  dvsmallcomponent->getModbusRegister = getDVSmallModbusRegister;//получить содержимое регистра
  dvsmallcomponent->getModbusBit      = getDVSmallModbusBit;//получить содержимое бита
  dvsmallcomponent->setModbusRegister = setDVSmallModbusRegister;// регистра
  dvsmallcomponent->setModbusBit      = setDVSmallModbusBit;// бита

  dvsmallcomponent->postWriteAction = postDVSmallWriteAction;//action после записи
}//constructorDVSmallComponent(COMPONENT_OBJ *DVcomp)

int getDVSmallModbusRegister(int adrReg) {
  //получить содержимое регистра
  if(privateDVSmallGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  active_inputs_lock_guard = 0xff;
  unsigned int const active_inputs_local  = active_inputs_low;
  active_inputs_lock_guard = 0x0;
  
  int tmp   = active_inputs_local & ((1ull <<NUMBER_INPUTS)-1);
  switch(adrReg-BEGIN_ADR_REGISTER)
  {
   case 0: tmp &= 0xFFFF; break;
   case 1: tmp = (tmp>>16)&0xFFFF; break;
   default: tmp = 0;
  }//switch

  return tmp;
}//getDVModbusRegister(int adrReg)
int getDVSmallModbusBit(int adrBit) {
  //получить содержимое bit
  if(privateDVSmallGetBit2(adrBit)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  active_inputs_lock_guard = 0xff;
  unsigned int const active_inputs_local  = active_inputs_low;
  active_inputs_lock_guard = 0x0;

  int tmp   = active_inputs_local & ((1ull <<NUMBER_INPUTS)-1);
  int offset = adrBit-BEGIN_ADR_BIT;
  switch(offset/16)
  {
   case 0: tmp &= 0xFFFF; break;
   case 1: tmp = (tmp>>16)&0xFFFF; break;
   default: tmp = 0;
  }//switch

  int maska = 1<<((adrBit-BEGIN_ADR_BIT)%16);
  if(tmp&maska) return 1;
  return 0;
}//getDVModbusBit(int adrReg)
int setDVSmallModbusRegister(int x, int y) {
  UNUSED(x);
  UNUSED(y);
  //записать содержимое регистра
  return MARKER_OUTPERIMETR;
}//getDVModbusRegister(int adrReg)
int setDVSmallModbusBit(int x, int y) {
  UNUSED(x);
  UNUSED(y);
  //получить содержимое регистра
  return MARKER_OUTPERIMETR;
}//getDVModbusRegister(int adrReg)

int postDVSmallWriteAction(void) {
//action после записи
  return 0;
}//

int privateDVSmallGetReg2(int adrReg)
{
  //проверить внешний периметр
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)

int privateDVSmallGetBit2(int adrBit)
{
  //проверить внешний периметр
 return controlPerimetr(adrBit, BEGIN_ADR_BIT, END_ADR_BIT);
}//privateGetReg2(int adrReg)
