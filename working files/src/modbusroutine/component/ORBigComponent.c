#include "header.h"

//начальный регистр в карте пам€ти
#define BEGIN_ADR_REGISTER 2508
//конечный регистр в карте пам€ти
#define END_ADR_REGISTER 2571

#define REGISTERS_OR 8

int privateORBigGetReg2(int adrReg);

int getORBigModbusRegister(int);//получить содержимое регистра
int getORBigModbusBit(int);//получить содержимое бита
int setORBigModbusRegister(int, int);//получить содержимое регистра
int setORBigModbusBit(int, int);//получить содержимое бита

int  postORBigWriteAction(void);//action после записи

SRAM1 COMPONENT_OBJ *orbigcomponent;

/**************************************/
//подготовка компонента OR
/**************************************/
void constructorORBigComponent(COMPONENT_OBJ *orbigcomp)
{
  orbigcomponent = orbigcomp;

  orbigcomponent->getModbusRegister = getORBigModbusRegister;//получить содержимое регистра
  orbigcomponent->getModbusBit      = getORBigModbusBit;//получить содержимое бита
  orbigcomponent->setModbusRegister = setORBigModbusRegister;//получить содержимое регистра
  orbigcomponent->setModbusBit      = setORBigModbusBit;//получить содержимое бита

  orbigcomponent->postWriteAction = postORBigWriteAction;//action после записи
}//prepareDVinConfig

int getORBigModbusRegister(int adrReg)
{
  //получить содержимое регистра
  if(privateORBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  return getRangN_BIGModbusRegister(&current_settings_interfaces.ranguvannja_d_or[0], REGISTERS_OR, adrReg-BEGIN_ADR_REGISTER );
}//getDOUTBigModbusRegister(int adrReg)
int getORBigModbusBit(int x)
{
  //получить содержимое регистра
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setORBigModbusRegister(int adrReg, int dataReg)
{
  //записать содержимое регистра
  if(privateORBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(orbigcomponent->isActiveActualData)
    {
      edition_settings = current_settings_interfaces;//делаем копию
    }//if(uprbigcomponent->isActiveActualData)
  superClearActiveActualData();

  superSetOperativMarker(orbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//записать в буфер

  //проверка на конфиг
  if(!(edition_settings.configuration&(1<<EL_BIT_CONFIGURATION))) return MARKER_ERRORPERIMETR;
  //проверка на допустимость
  return validN_BIGACMD(dataReg);
}//getDOUTBigModbusRegister(int adrReg)
int setORBigModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //получить содержимое регистра
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

int postORBigWriteAction(void) {
extern int upravlSchematic;//флаг Rang
//action после записи
  int beginAdr = orbigcomponent->operativMarker[0];
  if(beginAdr<0) return 0;//не было записи
  int endAdr   = orbigcomponent->operativMarker[1];
  int countAdr = endAdr-beginAdr+1;
  if(endAdr<0) countAdr = 1;

  writeRangN_BIGModbusRegister(&edition_settings.ranguvannja_d_or[0], REGISTERS_OR, beginAdr,
                                countAdr, BEGIN_ADR_REGISTER);

      //¬јЋ»ƒј÷»я ”—ѕ≈ЎЌј - ”—“јЌќ¬ ј
     upravlSchematic = 1;//флаг Rang

  return 0;
}//

int privateORBigGetReg2(int adrReg)
{
  //проверить внешний периметр
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)
