#include "header.h"

//начальный регистр в карте пам€ти
#define BEGIN_ADR_REGISTER 2444
//конечный регистр в карте пам€ти
#define END_ADR_REGISTER 2507

#define REGISTERS_AND 8

int privateANDBigGetReg2(int adrReg);

int getANDBigModbusRegister(int);//получить содержимое регистра
int getANDBigModbusBit(int);//получить содержимое бита
int setANDBigModbusRegister(int, int);// регистр
int setANDBigModbusBit(int, int);// бит

int  postANDBigWriteAction(void);//action после записи

SRAM1 COMPONENT_OBJ *andbigcomponent;

/**************************************/
//подготовка компонента AND
/**************************************/
void constructorANDBigComponent(COMPONENT_OBJ *andbigcomp)
{
  andbigcomponent = andbigcomp;

  andbigcomponent->getModbusRegister = getANDBigModbusRegister;//получить содержимое регистра
  andbigcomponent->getModbusBit      = getANDBigModbusBit;//получить содержимое бита
  andbigcomponent->setModbusRegister = setANDBigModbusRegister;//записать регистр
  andbigcomponent->setModbusBit      = setANDBigModbusBit;//записать бит

  andbigcomponent->postWriteAction = postANDBigWriteAction;//action после записи
}//prepareDVinConfig

int getANDBigModbusRegister(int adrReg)
{
  //получить содержимое регистра
  if(privateANDBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  return getRangN_BIGModbusRegister(&current_settings_interfaces.ranguvannja_d_and[0], REGISTERS_AND, adrReg-BEGIN_ADR_REGISTER );
}//getDOUTBigModbusRegister(int adrReg)
int getANDBigModbusBit(int x)
{
  //получить содержимое бита
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setANDBigModbusRegister(int adrReg, int dataReg)
{
  //записать содержимое регистра
  if(privateANDBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(andbigcomponent->isActiveActualData)
    {
      edition_settings = current_settings_interfaces;//делаем копию
    }//if(uprbigcomponent->isActiveActualData)
  superClearActiveActualData();

  superSetOperativMarker(andbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//записать в буфер

  //проверка на конфиг
  if(!(edition_settings.configuration&(1<<EL_BIT_CONFIGURATION))) return MARKER_ERRORPERIMETR;
  //проверка на допустимость
  return validN_BIGACMD(dataReg);
}//setDOUTBigModbusRegister(int adrReg)
int setANDBigModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //получить содержимое бита
  return MARKER_OUTPERIMETR;
}//setDOUTBigModbusRegister(int adrReg)

int postANDBigWriteAction(void) {
extern int upravlSchematic;//флаг Rang
//action после записи
  int beginAdr = andbigcomponent->operativMarker[0];
  if(beginAdr<0) return 0;//не было записи
  int endAdr   = andbigcomponent->operativMarker[1];
  int countAdr = endAdr-beginAdr+1;
  if(endAdr<0) countAdr = 1;

  writeRangN_BIGModbusRegister(&edition_settings.ranguvannja_d_and[0], REGISTERS_AND, beginAdr,
                                countAdr, BEGIN_ADR_REGISTER);

      //¬јЋ»ƒј÷»я ”—ѕ≈ЎЌј - ”—“јЌќ¬ ј
     upravlSchematic = 1;//флаг Rang

  return 0;
}//postANDBigWriteAction() 

int privateANDBigGetReg2(int adrReg)
{
  //проверить внешний периметр
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)
