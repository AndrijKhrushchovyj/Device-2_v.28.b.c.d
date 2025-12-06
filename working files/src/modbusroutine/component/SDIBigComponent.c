#include "header.h"

//начальный регистр в карте памяти
#define BEGIN_ADR_REGISTER 1580
//конечный регистр в карте памяти
//17шт
#define END_ADR_REGISTER 1715

#define REGISTERS_SDI 8

int privateSDIBigGetReg2(int adrReg);

int getSDIBigModbusRegister(int);//получить содержимое регистра
int getSDIBigModbusBit(int);//получить содержимое бита
int setSDIBigModbusRegister(int, int);//получить содержимое регистра
int setSDIBigModbusBit(int, int);//получить содержимое бита

int  postSDIBigWriteAction(void);//action после записи

SRAM1 COMPONENT_OBJ *sdibigcomponent;

/**************************************/
//подготовка компонента светоиндикаторов
/**************************************/
void constructorSDIBigComponent(COMPONENT_OBJ *sdibigcomp)
{
  sdibigcomponent = sdibigcomp;

  sdibigcomponent->getModbusRegister = getSDIBigModbusRegister;//получить содержимое регистра
  sdibigcomponent->getModbusBit      = getSDIBigModbusBit;//получить содержимое бита
  sdibigcomponent->setModbusRegister = setSDIBigModbusRegister;//получить содержимое регистра
  sdibigcomponent->setModbusBit      = setSDIBigModbusBit;//получить содержимое бита

  sdibigcomponent->postWriteAction = postSDIBigWriteAction;//action после записи
}//prepareDVinConfig

int getSDIBigModbusRegister(int adrReg)
{
  //получить содержимое регистра
  if(privateSDIBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  return getRangN_BIGModbusRegister(&current_settings_interfaces.ranguvannja_leds[0], 
                                     REGISTERS_SDI, adrReg-BEGIN_ADR_REGISTER );
}//getSDIBigModbusRegister(int adrReg)
int getSDIBigModbusBit(int x)
{
  //получить содержимое регистра
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getSDIBigModbusBit(int )
int setSDIBigModbusRegister(int adrReg, int dataReg)
{
  //записать содержимое регистра
  if(privateSDIBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(sdibigcomponent->isActiveActualData)
    {
      edition_settings = current_settings_interfaces;//делаем копию
    }//if(uprbigcomponent->isActiveActualData)
  superClearActiveActualData();

  superSetOperativMarker(sdibigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//записать в буфер
  //проверка на допустимость
  return validN_BIGACMD(dataReg);
}//getDOUTBigModbusRegister(int adrReg)
int setSDIBigModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //получить содержимое регистра
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

int postSDIBigWriteAction(void) {
extern int upravlSchematic;//флаг Rang
//action после записи
  int beginAdr = sdibigcomponent->operativMarker[0];
  if(beginAdr<0) return 0;//не было записи
  int endAdr   = sdibigcomponent->operativMarker[1];
  int countAdr = endAdr-beginAdr+1;
  if(endAdr<0) countAdr = 1;

  writeRangN_BIGModbusRegister(&edition_settings.ranguvannja_leds[0], REGISTERS_SDI, beginAdr,
                                countAdr, BEGIN_ADR_REGISTER);

      //ВАЛИДАЦИЯ УСПЕШНА - УСТАНОВКА
     upravlSchematic = 1;//флаг Rang

  return 0;
}//

int privateSDIBigGetReg2(int adrReg)
{
  //проверить внешний периметр
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)
