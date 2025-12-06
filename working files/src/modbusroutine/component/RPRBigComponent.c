
#include "header.h"

//начальный регистр в карте памяти
#define BEGIN_ADR_REGISTER 1016
//конечный регистр в карте памяти
#define END_ADR_REGISTER 1099

int privateRPRBigGetReg2(int adrReg);

int getRPRBigModbusRegister(int);//получить содержимое регистра
int getRPRBigModbusBit(int);//получить содержимое бита
int setRPRBigModbusRegister(int, int);//получить содержимое регистра
int setRPRBigModbusBit(int, int);//получить содержимое бита

int  postRPRBigWriteAction(void);//action после записи
int getRPRSmallModbusBeginAdrRegister(void);
int getRPRSmallModbusEndAdrRegister(void);

SRAM1 COMPONENT_OBJ *rprbigcomponent;

/**************************************/
//подготовка компонента ранжирование пользовательских регистров
/**************************************/
void constructorRPRBigComponent(COMPONENT_OBJ *rprbigcomp)
{
  rprbigcomponent = rprbigcomp;

  rprbigcomponent->getModbusRegister = getRPRBigModbusRegister;//получить содержимое регистра
  rprbigcomponent->getModbusBit      = getRPRBigModbusBit;//получить содержимое бита
  rprbigcomponent->setModbusRegister = setRPRBigModbusRegister;//получить содержимое регистра
  rprbigcomponent->setModbusBit      = setRPRBigModbusBit;//получить содержимое бита

  rprbigcomponent->postWriteAction = postRPRBigWriteAction;//action после записи
}//prepareDVinConfig

int getRPRBigModbusRegister(int adrReg)
{
  //получить содержимое регистра
  if(privateRPRBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  return (current_settings_interfaces.user_register[adrReg-BEGIN_ADR_REGISTER]) &0xFFFF;
}//getDOUTBigModbusRegister(int adrReg)
int getRPRBigModbusBit(int x)
{
  //получить содержимое регистра
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setRPRBigModbusRegister(int adrReg, int dataReg)
{
  //записать содержимое регистра
  if(privateRPRBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(rprbigcomponent->isActiveActualData)
    {
      edition_settings = current_settings_interfaces;//делаем копию
    }//if(rprbigcomponent->isActiveActualData)
  superClearActiveActualData();

  superSetOperativMarker(rprbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//записать в буфер
//нельзя записывать адреса small компонента
  if(dataReg>=getRPRSmallModbusBeginAdrRegister() &&
      (dataReg<(getRPRSmallModbusEndAdrRegister()+1))) return MARKER_ERRORDIAPAZON;

  return 0;
}//getDOUTBigModbusRegister(int adrReg)
int setRPRBigModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //получить содержимое регистра
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

int postRPRBigWriteAction(void)
{
extern int upravlSetting;//флаг Setting
//action после записи
  int beginAdr = rprbigcomponent->operativMarker[0];
  if(beginAdr<0) return 0;//не было записи
  int endAdr   = rprbigcomponent->operativMarker[1];
  int offsetTempWriteArray = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//найти смещение TempWriteArray
  int countAdr = endAdr-beginAdr+1;
  if(endAdr<0) countAdr = 1;

  for(int i=0; i<countAdr; i++) {
    edition_settings.user_register[i+beginAdr-BEGIN_ADR_REGISTER] = (unsigned short)tempWriteArray[offsetTempWriteArray+i];//новое значение
  }//for

  //ВАЛИДАЦИЯ УСПЕШНА - УСТАНОВКА
     upravlSetting = 2;//флаг Setting
     type_of_settings_changed |= (1 << USER_REGISTRY_CHANGED_BIT);

  return 0;
}//

int privateRPRBigGetReg2(int adrReg)
{
  //проверить внешний периметр
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)
