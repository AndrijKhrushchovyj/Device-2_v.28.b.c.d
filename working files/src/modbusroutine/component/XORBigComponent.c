#include "header.h"

//начальный регистр в карте пам€ти
#define BEGIN_ADR_REGISTER 2572
//конечный регистр в карте пам€ти
#define END_ADR_REGISTER 2587

#define REGISTERS_XOR 2

int privateXORBigGetReg2(int adrReg);

int getXORBigModbusRegister(int);//получить содержимое регистра
int getXORBigModbusBit(int);//получить содержимое бита
int setXORBigModbusRegister(int, int);//получить содержимое регистра
int setXORBigModbusBit(int, int);//получить содержимое бита

int  postXORBigWriteAction(void);//action после записи

SRAM1 COMPONENT_OBJ *xorbigcomponent;

/**************************************/
//подготовка компонента XOR
/**************************************/
void constructorXORBigComponent(COMPONENT_OBJ *xorbigcomp)
{
  xorbigcomponent = xorbigcomp;

  xorbigcomponent->getModbusRegister = getXORBigModbusRegister;//получить содержимое регистра
  xorbigcomponent->getModbusBit      = getXORBigModbusBit;//получить содержимое бита
  xorbigcomponent->setModbusRegister = setXORBigModbusRegister;//получить содержимое регистра
  xorbigcomponent->setModbusBit      = setXORBigModbusBit;//получить содержимое бита

  xorbigcomponent->postWriteAction = postXORBigWriteAction;//action после записи
}//prepareDVinConfig

int getXORBigModbusRegister(int adrReg)
{
  //получить содержимое регистра
  if(privateXORBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  return getRangN_BIGModbusRegister(&current_settings_interfaces.ranguvannja_d_xor[0], REGISTERS_XOR, adrReg-BEGIN_ADR_REGISTER );
}//getDOUTBigModbusRegister(int adrReg)
int getXORBigModbusBit(int x)
{
  //получить содержимое регистра
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setXORBigModbusRegister(int adrReg, int dataReg)
{
  //записать содержимое регистра
  if(privateXORBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(xorbigcomponent->isActiveActualData)
    {
      edition_settings = current_settings_interfaces;//делаем копию
    }//if(uprbigcomponent->isActiveActualData)
  superClearActiveActualData();

  superSetOperativMarker(xorbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//записать в буфер

  //проверка на конфиг
  if(!(edition_settings.configuration&(1<<EL_BIT_CONFIGURATION))) return MARKER_ERRORPERIMETR;
  //проверка на допустимость
  return validN_BIGACMD(dataReg);
}//getDOUTBigModbusRegister(int adrReg)
int setXORBigModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //записать содержимое bit
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

int postXORBigWriteAction(void) {
extern int upravlSchematic;//флаг Rang
//action после записи
  int beginAdr = xorbigcomponent->operativMarker[0];
  if(beginAdr<0) return 0;//не было записи
  int endAdr   = xorbigcomponent->operativMarker[1];
  int countAdr = endAdr-beginAdr+1;
  if(endAdr<0) countAdr = 1;

  writeRangN_BIGModbusRegister(&edition_settings.ranguvannja_d_xor[0], REGISTERS_XOR, beginAdr,
                                countAdr, BEGIN_ADR_REGISTER);

      //¬јЋ»ƒј÷»я ”—ѕ≈ЎЌј - ”—“јЌќ¬ ј
     upravlSchematic = 1;//флаг Rang

  return 0;
}//

int privateXORBigGetReg2(int adrReg)
{
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateYustBigGetReg2(int adrReg)

