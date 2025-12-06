
#include "header.h"

//начальный регистр в карте памяти
#define BEGIN_ADR_REGISTER 297
//начальный bit в карте памяти
#define BEGIN_ADR_BIT 50656

//конечный регистр в карте памяти
#define END_ADR_REGISTER 337
//конечный bit в карте памяти
#define END_ADR_BIT 51311
//#endif

int privateGCMDSmallGetReg2(int adrReg);
int privateGCMDSmallGetBit2(int adrBit);

int getGCMDSmallModbusRegister(int);//получить содержимое регистра
int getGCMDSmallModbusBit(int);//получить содержимое бита
int setGCMDSmallModbusRegister(int, int);//записать регистр
int setGCMDSmallModbusBit(int, int);//записать бит

int  postGCMDSmallWriteAction(void);//action после записи
void loadACMDSmallActualDataBit(int cmdSwitch, int beginOffset, int endOffset);

SRAM1 COMPONENT_OBJ *gcmdsmallcomponent;
/**************************************/
//компонент Текущие активные и сработавшие функции
/**************************************/
void constructorGCMDSmallComponent(COMPONENT_OBJ *gcmdcomp)
{
  gcmdsmallcomponent = gcmdcomp;

  gcmdsmallcomponent->getModbusRegister = getGCMDSmallModbusRegister;//получить содержимое регистра
  gcmdsmallcomponent->getModbusBit      = getGCMDSmallModbusBit;//получить содержимое бита
  gcmdsmallcomponent->setModbusRegister = setGCMDSmallModbusRegister;// регистра
  gcmdsmallcomponent->setModbusBit      = setGCMDSmallModbusBit;// бита

  gcmdsmallcomponent->postWriteAction = postGCMDSmallWriteAction;//action после записи
}//constructorGCMDSmallComponent(COMPONENT_OBJ *gcmdcomp)

int getGCMDSmallModbusRegister(int adrReg)
{
  extern int globalcntReg;//к-во бит для чтения
  extern int globalbeginAdrReg;//адрес нач бит
  //получить содержимое регистра
  if(privateGCMDSmallGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(gcmdsmallcomponent->isActiveActualData)
    {
    int begin = globalbeginAdrReg-BEGIN_ADR_REGISTER;
    if(begin<0) globalcntReg += begin;

    int beginOffset = (adrReg-BEGIN_ADR_REGISTER)*16;
    int endOffset   = beginOffset +globalcntReg*16;

      loadACMDSmallActualDataBit(1, beginOffset, endOffset); //ActualData GCMD
    }//if(gcmdsmallcomponent->isActiveActualData)
  gcmdsmallcomponent->isActiveActualData = 0;

  return tempReadArray[adrReg-BEGIN_ADR_REGISTER];
}//getDOUTModbusRegister(int adrReg)
int getGCMDSmallModbusBit(int adrBit)
{
  extern int globalcntBit;//к-во бит для чтения
  extern int globalbeginAdrBit;//адрес нач бит
  //получить содержимое bit
  if(privateGCMDSmallGetBit2(adrBit)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  int beginOffset = adrBit-BEGIN_ADR_BIT;
  if(gcmdsmallcomponent->isActiveActualData)
    {
      int begin = globalbeginAdrBit-BEGIN_ADR_BIT;
    if(begin<0) globalcntBit += begin;

      int endOffset   = beginOffset +globalcntBit;

      loadACMDSmallActualDataBit(1, beginOffset, endOffset); //ActualData GCMD
    }//if(gcmdsmallcomponent->isActiveActualData)
  gcmdsmallcomponent->isActiveActualData = 0;

  short tmp   = tempReadArray[beginOffset/16];
  short maska = 1<<(beginOffset%16);
  if(tmp&maska) return 1;
  return 0;
}//getGCMDSmallModbusBit(int adrBit)
int setGCMDSmallModbusRegister(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //записать содержимое регистра
  return MARKER_OUTPERIMETR;
}//getDOUTModbusRegister(int adrReg)
int setGCMDSmallModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //получить содержимое регистра
  return MARKER_OUTPERIMETR;
}//getDOUTModbusRegister(int adrReg)

int postGCMDSmallWriteAction(void)
{
//action после записи
  return 0;
}//

int privateGCMDSmallGetReg2(int adrReg)
{
  //проверить внешний периметр
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)

int privateGCMDSmallGetBit2(int adrBit)
{
  //проверить внешний периметр
 return controlPerimetr(adrBit, BEGIN_ADR_BIT, END_ADR_BIT);
}//privateGetReg2(int adrReg)
