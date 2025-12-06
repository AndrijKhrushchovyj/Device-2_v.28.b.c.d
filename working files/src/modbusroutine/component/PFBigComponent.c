#include "header.h"

//начальный регистр в карте пам€ти
#define BEGIN_ADR_REGISTER 2604
//конечный регистр в карте пам€ти
#define END_ADR_REGISTER 2635

#define REGISTERS_PF 2

int privatePFBigGetReg2(int adrReg);

int getPFBigModbusRegister(int);//получить содержимое регистра
int getPFBigModbusBit(int);//получить содержимое бита
int setPFBigModbusRegister(int, int);//получить содержимое регистра
int setPFBigModbusBit(int, int);//получить содержимое бита

int  postPFBigWriteAction(void);//action после записи

SRAM1 COMPONENT_OBJ *pfbigcomponent;

/**************************************/
//подготовка компонента PF
/**************************************/
void constructorPFBigComponent(COMPONENT_OBJ *pfbigcomp)
{
  pfbigcomponent = pfbigcomp;

  pfbigcomponent->getModbusRegister = getPFBigModbusRegister;//получить содержимое регистра
  pfbigcomponent->getModbusBit      = getPFBigModbusBit;//получить содержимое бита
  pfbigcomponent->setModbusRegister = setPFBigModbusRegister;//получить содержимое регистра
  pfbigcomponent->setModbusBit      = setPFBigModbusBit;//получить содержимое бита

  pfbigcomponent->postWriteAction = postPFBigWriteAction;//action после записи
}//prepareDVinConfig

int getPFBigModbusRegister(int adrReg)
{
  //получить содержимое регистра
  if(privatePFBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  int offset = adrReg-BEGIN_ADR_REGISTER;
  int bitInOut  = current_settings_interfaces.ranguvannja_tf[offset/REGISTERS_PF];
  int bit = -1;
  int adr = -1;
  switch(offset%REGISTERS_PF)
  {
   case 0:
     bit = (bitInOut&0xFFFF)-1;
  if(bit!=-1)
    {
      adr = decoderN_BIGACMD(bit);
    }//if(bit!=-1)
   break;
   case 1:
     bit = ((bitInOut>>16)&0xFFFF)-1;
  if(bit!=-1)
    {
      adr = decoderN_SMALLACMD(bit);
    }//if(bit!=-1)
   break;
  }//switch

  return adr<0? 0: adr+getACMDSmallBeginAdr();//декодировщик индекса бита в адрес modbus  дл€ rele
}//getDOUTBigModbusRegister(int adrReg)
int getPFBigModbusBit(int x)
{
  //получить содержимое регистра
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setPFBigModbusRegister(int adrReg, int dataReg)
{
  //записать содержимое регистра
  if(privatePFBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(pfbigcomponent->isActiveActualData)
    {
      edition_settings = current_settings_interfaces;//делаем копию
    }//if(uprbigcomponent->isActiveActualData)
  superClearActiveActualData();

  superSetOperativMarker(pfbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//записать в буфер

  //проверка на конфиг
  if(!(edition_settings.configuration&(1<<EL_BIT_CONFIGURATION))) return MARKER_ERRORPERIMETR;

  int offset = adrReg-BEGIN_ADR_REGISTER;
  switch(offset%REGISTERS_PF)
  {
   case 0:
     if(!dataReg) return 0;
     return validN_BIGACMD(dataReg);
   case 1:
     if(!dataReg) return 0;
     return validN_SMALLACMD(dataReg);
  }//switch

  return 0;
}//getDOUTBigModbusRegister(int adrReg)
int setPFBigModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //получить содержимое регистра
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

int postPFBigWriteAction(void) {
extern int upravlSchematic;//флаг Rang
//action после записи
  int beginAdr = pfbigcomponent->operativMarker[0];
  if(beginAdr<0) return 0;//не было записи
  int endAdr   = pfbigcomponent->operativMarker[1];
  int countAdr = endAdr-beginAdr+1;
  if(endAdr<0) countAdr = 1;
  int offsetTempWriteArray = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//найти смещение TempWriteArray

  //добавить новое
  for(int i=0; i<countAdr; i++)
    {
      unsigned int adr = (unsigned short)tempWriteArray[offsetTempWriteArray+i];//новое значение
      int offset = beginAdr-BEGIN_ADR_REGISTER+i;
      int subObj = offset/REGISTERS_PF;
      int bit = -1;

  switch(offset%REGISTERS_PF)
  {
   case 0:
      if(adr==0) bit = -2;
      else bit = encoderN_BIGACMD(adr-getACMDSmallBeginAdr()); //кодировщик адреса modbus в индекс бита дл€ реле

      if(bit!=-1) {
         if(bit==-2) bit=-1;
         edition_settings.ranguvannja_tf[subObj] &= (uint32_t)~0xffff;
         edition_settings.ranguvannja_tf[subObj] |= (bit+1);
      }//if
   break;

   case 1:
      if(adr==0) bit = -2;
      else bit = encoderN_SMALLACMD(adr-getACMDSmallBeginAdr()); //кодировщик адреса modbus в индекс бита дл€ dv

      if(bit!=-1) {
         if(bit==-2) bit=-1;
         edition_settings.ranguvannja_tf[subObj] &= (uint32_t)~(0xffff<<16);
         edition_settings.ranguvannja_tf[subObj] |= ((bit+1)<<16);//
      }//if
   break;
  }//switch

    }//for

      //¬јЋ»ƒј÷»я ”—ѕ≈ЎЌј - ”—“јЌќ¬ ј
     upravlSchematic = 1;//флаг Rang

  return 0;
}//

int privatePFBigGetReg2(int adrReg)
{
  //проверить внешний периметр
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)
