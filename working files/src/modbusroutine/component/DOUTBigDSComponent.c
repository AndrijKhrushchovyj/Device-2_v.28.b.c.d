
#include "header.h"

#ifdef  MODYFIKACIA_VERSII_DS
//начальный регистр в карте памяти
#define BEGIN_ADR_REGISTER 1404
//конечный регистр в карте памяти
#define END_ADR_REGISTER 1419

#define REGISTERS_OUTPUTS 16

int privateDOUTBigDSGetReg2(int adrReg);

int getDOUTBigDSModbusRegister(int);//получить содержимое регистра
int getDOUTBigDSModbusBit(int);//получить содержимое бита
int setDOUTBigDSModbusRegister(int, int);// регистра
int setDOUTBigDSModbusBit(int, int);// бита

int  postDOUTBigDSWriteAction(void);//action после записи

COMPONENT_OBJ *doutbigdscomponent;

/**************************************/
//подготовка компонента rele ДШ
/**************************************/
void constructorDOUTBigDSComponent(COMPONENT_OBJ *doutcomp)
{
  doutbigdscomponent = doutcomp;

  doutbigdscomponent->getModbusRegister = getDOUTBigDSModbusRegister;//получить содержимое регистра
  doutbigdscomponent->getModbusBit      = getDOUTBigDSModbusBit;//получить содержимое бита
  doutbigdscomponent->setModbusRegister = setDOUTBigDSModbusRegister;// регистра
  doutbigdscomponent->setModbusBit      = setDOUTBigDSModbusBit;// бита

  doutbigdscomponent->postWriteAction = postDOUTBigDSWriteAction;//action после записи
}//prepareDVinConfig

int getDOUTBigDSModbusRegister(int adrReg)
{
  //получить содержимое регистра
  if(privateDOUTBigDSGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  return getRangN_BIGModbusRegister(&current_settings_interfaces.ranguvannja_outputs[(N_BIG*NUMBER_SIMPLE_OUTPUTS)],
                                     REGISTERS_OUTPUTS, adrReg-BEGIN_ADR_REGISTER );
}//getDOUTBigDSModbusRegister(int adrReg)
int getDOUTBigDSModbusBit(int x)
{
  //получить содержимое регистра
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getDOUTBigDSModbusRegister(int adrReg)
int setDOUTBigDSModbusRegister(int adrReg, int dataReg)
{
  //записать содержимое регистра
  if(privateDOUTBigDSGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(doutbigdscomponent->isActiveActualData)
    {
      edition_settings = current_settings_interfaces;//делаем копию
    }//if(uprbigcomponent->isActiveActualData)
  superClearActiveActualData();

  superSetOperativMarker(doutbigdscomponent, adrReg);
  superSetTempWriteArray(dataReg);//записать в буфер

  //проверка на конфиг
  if(!(current_settings_interfaces.configuration & (1 << DS_BIT_CONFIGURATION))) return MARKER_ERRORPERIMETR;//Ранжирование ДШ
  //проверка на допустимость
  return validN_BIGACMD(dataReg);
}//getDOUTBigDSModbusRegister(int adrReg)
int setDOUTBigDSModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //получить содержимое регистра
  return MARKER_OUTPERIMETR;
}//getDOUTBigDSModbusRegister(int adrReg)

int postDOUTBigDSWriteAction(void)
{
extern int upravlSchematic;//флаг Rang
//action после записи
  int beginAdr = doutbigdscomponent->operativMarker[0];
  if(beginAdr<0) return 0;//не было записи
  int endAdr   = doutbigdscomponent->operativMarker[1];
  int countAdr = endAdr-beginAdr+1;
  if(endAdr<0) countAdr = 1;

  writeRangN_BIGModbusRegister(&edition_settings.ranguvannja_outputs[(N_BIG*NUMBER_SIMPLE_OUTPUTS)], REGISTERS_OUTPUTS, beginAdr,
                                countAdr, BEGIN_ADR_REGISTER);

      //ВАЛИДАЦИЯ УСПЕШНА - УСТАНОВКА
     upravlSchematic = 1;//флаг Rang

  return 0;
}//

int privateDOUTBigDSGetReg2(int adrReg)
{
  //проверить внешний периметр
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)
#endif
