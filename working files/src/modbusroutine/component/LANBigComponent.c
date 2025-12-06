#include "header.h"
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)

//начальный регистр в карте памяти
#define BEGIN_ADR_REGISTER 3916
//конечный регистр в карте памяти
#define END_ADR_REGISTER 4171

//к-во регистров внутри одной части LAN
#define REGISTERS_LAN 8
//к-во частей внутри одного LAN
#define PART_REGISTERS_LAN 8

int privateLANBigGetReg2(int adrReg);

int getLANBigModbusRegister(int);//получить содержимое регистра
int getLANBigModbusBit(int);//получить содержимое бита
int setLANBigModbusRegister(int, int);//получить содержимое регистра
int setLANBigModbusBit(int, int);//получить содержимое бита

int  postLANBigWriteAction(void);//action после записи

SRAM1 COMPONENT_OBJ *lanbigcomponent;

/**************************************/
//подготовка компонента LAN
/**************************************/
void constructorLANBigComponent(COMPONENT_OBJ *lanbigcomp)
{
  lanbigcomponent = lanbigcomp;

  lanbigcomponent->getModbusRegister = getLANBigModbusRegister;//получить содержимое регистра
  lanbigcomponent->getModbusBit      = getLANBigModbusBit;//получить содержимое бита
  lanbigcomponent->setModbusRegister = setLANBigModbusRegister;//получить содержимое регистра
  lanbigcomponent->setModbusBit      = setLANBigModbusBit;//получить содержимое бита

  lanbigcomponent->postWriteAction = postLANBigWriteAction;//action после записи
}//

int getLANBigModbusRegister(int adrReg)
{
  //получить содержимое регистра
  if(privateLANBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  int offset = adrReg-BEGIN_ADR_REGISTER;
  int idxObj = offset/(REGISTERS_LAN*PART_REGISTERS_LAN);

//поиск активного бита
  uint16_t *ranguvannja_lan = &current_settings_interfaces.ranguvannja_Out_LAN[idxObj]
                                    [(offset%(REGISTERS_LAN*PART_REGISTERS_LAN))/REGISTERS_LAN][0]; //Ранжування 

  int bitInOut  = ranguvannja_lan[offset%REGISTERS_LAN];
  int adr = -1;

  if(bitInOut!=0)
    {
      adr = decoderN_BIGACMD(bitInOut-1);
    }//if(bitInOut!=0)

  return adr<0? 0: adr+getACMDSmallBeginAdr();//декодировщик индекса бита в адрес modbus  для rele
}//getLANBigModbusRegister(int adrReg)
int getLANBigModbusBit(int x)
{
  //получить содержимое регистра
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getLANBigModbusBit(int adrReg)
int setLANBigModbusRegister(int adrReg, int dataReg)
{
  //записать содержимое регистра
  if(privateLANBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(lanbigcomponent->isActiveActualData)
    {
      edition_settings = current_settings_interfaces;//делаем копию
    }//if(uprbigcomponent->isActiveActualData)
  superClearActiveActualData();

  superSetOperativMarker(lanbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//записать в буфер

//  if(dataReg<0) return 0;
  return validN_BIGACMD(dataReg);
}//getDOUTBigModbusRegister(int adrReg)
int setLANBigModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //получить содержимое регистра
  return MARKER_OUTPERIMETR;
}//setLANBigModbusBit

int postLANBigWriteAction(void) {
extern int upravlSchematic;//флаг Rang
//action после записи
  int beginAdr = lanbigcomponent->operativMarker[0];
  if(beginAdr<0) return 0;//не было записи
  int endAdr   = lanbigcomponent->operativMarker[1];
  int countAdr = endAdr-beginAdr+1;
  if(endAdr<0) countAdr = 1;
  int offsetTempWriteArray = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//найти смещение TempWriteArray

  //добавить новое
  for(int i=0; i<countAdr; i++)
    {
      unsigned int adr = (unsigned short)tempWriteArray[offsetTempWriteArray+i];//новое значение
      int offset = beginAdr-BEGIN_ADR_REGISTER+i;

      int idxObj = offset/(REGISTERS_LAN*PART_REGISTERS_LAN);
      //поиск активного бита
      uint16_t *ranguvannja_lan = &edition_settings.ranguvannja_Out_LAN[idxObj]
                                    [(offset%(REGISTERS_LAN*PART_REGISTERS_LAN))/REGISTERS_LAN][0]; //Ранжування 

      ranguvannja_lan[offset%REGISTERS_LAN] = (encoderN_BIGACMD(adr-getACMDSmallBeginAdr())+1);//кодировщик адреса modbus в индекс бита для реле
    }//for

      //ВАЛИДАЦИЯ УСПЕШНА - УСТАНОВКА
  upravlSchematic = 1;//флаг Rang

  return 0;
}//

int privateLANBigGetReg2(int adrReg)
{
  //проверить внешний периметр
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateLANBigGetReg2(int adrReg)
#endif
