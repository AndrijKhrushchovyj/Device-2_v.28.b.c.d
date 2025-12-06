//начальный регистр в карте пам€ти
#define BEGIN_ADR_REGISTER 4172
//конечный регистр в карте пам€ти
#define END_ADR_REGISTER 4299

#define REGISTERS_DFRES 16

#include "header.h"

int privateDFRESBigGetReg2(int adrReg);

int getDFRESBigModbusRegister(int);//получить содержимое регистра
int getDFRESBigModbusBit(int);//получить содержимое бита
int setDFRESBigModbusRegister(int, int);// регистр
int setDFRESBigModbusBit(int, int);// бит

int  postDFRESBigWriteAction(void);//action после записи
int validMFTN_BIGACMD(unsigned short dataReg, int actControl);

SRAM1 COMPONENT_OBJ *dfresbigcomponent;

/**************************************/
//подготовка компонента DFRESet
/**************************************/
void constructorDFRESBigComponent(COMPONENT_OBJ *dfresbigcomp)
{
  dfresbigcomponent = dfresbigcomp;

  dfresbigcomponent->getModbusRegister = getDFRESBigModbusRegister;//получить содержимое регистра
  dfresbigcomponent->getModbusBit      = getDFRESBigModbusBit;//получить содержимое бита
  dfresbigcomponent->setModbusRegister = setDFRESBigModbusRegister;//записать регистр
  dfresbigcomponent->setModbusBit      = setDFRESBigModbusBit;//записать бит

  dfresbigcomponent->postWriteAction = postDFRESBigWriteAction;//action после записи
}//prepareDVinConfig

int getDFRESBigModbusRegister(int adrReg)
{
  //получить содержимое регистра
  if(privateDFRESBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  return getRangN_BIGModbusRegister(&current_settings_interfaces.ranguvannja_df_source_reset[0], REGISTERS_DFRES, adrReg-BEGIN_ADR_REGISTER );
}//getDOUTBigModbusRegister(int adrReg)
int getDFRESBigModbusBit(int x)
{
  //получить содержимое бита
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setDFRESBigModbusRegister(int adrReg, int dataReg)
{
  //записать содержимое регистра
  if(privateDFRESBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(dfresbigcomponent->isActiveActualData)
    {
      edition_settings = current_settings_interfaces;//делаем копию
    }//if(uprbigcomponent->isActiveActualData)
  superClearActiveActualData();

  superSetOperativMarker(dfresbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//записать в буфер

  //проверка на конфиг
  if(!(edition_settings.configuration&(1<<EL_BIT_CONFIGURATION))) return MARKER_ERRORPERIMETR;
  //проверка на допустимость
  return validMFTN_BIGACMD(dataReg, (adrReg-BEGIN_ADR_REGISTER)/REGISTERS_DFRES);
}//setDOUTBigModbusRegister(int adrReg)
int setDFRESBigModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //получить содержимое бита
  return MARKER_OUTPERIMETR;
}//setDOUTBigModbusRegister(int adrReg)

int postDFRESBigWriteAction(void) {
extern int upravlSchematic;//флаг Rang
//action после записи
  int beginAdr = dfresbigcomponent->operativMarker[0];
  if(beginAdr<0) return 0;//не было записи
  int endAdr   = dfresbigcomponent->operativMarker[1];
  int countAdr = endAdr-beginAdr+1;
  if(endAdr<0) countAdr = 1;

  writeRangN_BIGModbusRegister(&edition_settings.ranguvannja_df_source_reset[0], REGISTERS_DFRES, beginAdr,
                                countAdr, BEGIN_ADR_REGISTER);

      //¬јЋ»ƒј÷»я ”—ѕ≈ЎЌј - ”—“јЌќ¬ ј
     upravlSchematic = 1;//флаг Rang

  return 0;
}//postDFRESBigWriteAction() 

int privateDFRESBigGetReg2(int adrReg)
{
  //проверить внешний периметр
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)
