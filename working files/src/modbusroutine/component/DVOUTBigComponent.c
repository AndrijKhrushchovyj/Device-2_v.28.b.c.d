
#include "header.h"

//начальный регистр в карте пам€ти
#define BEGIN_ADR_REGISTER 4332
#define REGISTERS_DV 8
#define REGISTERS_OUTPUTS 16


#if (                                \
     (MODYFIKACIA_VERSII_PZ == 8) || \
     (MODYFIKACIA_VERSII_PZ == 18)   \
    )
//конечный регистр в карте пам€ти
#define END_ADR_REGISTER 4491
#endif


int privateDVOUTBigGetReg2(int adrReg);

int getDVOUTBigModbusRegister(int);//получить содержимое регистра
int getDVOUTBigModbusBit(int);//получить содержимое бита
int setDVOUTBigModbusRegister(int, int);// регистра
int setDVOUTBigModbusBit(int, int);// бита

int  postDVOUTBigWriteAction(void);//action после записи

SRAM1 COMPONENT_OBJ *dvoutbigcomponent;

/**************************************/
//подготовка компонента dvrele
/**************************************/
void constructorDVOUTBigComponent(COMPONENT_OBJ *dvoutcomp)
{
  dvoutbigcomponent = dvoutcomp;

  dvoutbigcomponent->getModbusRegister = getDVOUTBigModbusRegister;//получить содержимое регистра
  dvoutbigcomponent->getModbusBit      = getDVOUTBigModbusBit;//получить содержимое бита
  dvoutbigcomponent->setModbusRegister = setDVOUTBigModbusRegister;// регистра
  dvoutbigcomponent->setModbusBit      = setDVOUTBigModbusBit;// бита

  dvoutbigcomponent->postWriteAction = postDVOUTBigWriteAction;//action после записи
}//prepareDVinConfig

int getDVOUTBigModbusRegister(int adrReg)
{
  //получить содержимое регистра
  if(privateDVOUTBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
#if (                                \
     (MODYFIKACIA_VERSII_PZ == 8) || \
     (MODYFIKACIA_VERSII_PZ == 18)   \
    )
  if(adrReg >= 4396)
    return getRangN_SMALLModbusRegister(&current_settings_interfaces.ranguvannja_inputs[20*N_SMALL], REGISTERS_DV, adrReg-4396);//+20*REGISTERS_DV );
  return getRangN_BIGModbusRegister(&current_settings_interfaces.ranguvannja_outputs[20*N_BIG], REGISTERS_OUTPUTS, adrReg-4332);//+20*REGISTERS_OUTPUTS );
#else
  return MARKER_OUTPERIMETR;
#endif
}//getDVOUTBigModbusRegister(int adrReg)
int getDVOUTBigModbusBit(int x)
{
  //получить содержимое регистра
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getDVOUTBigModbusRegister(int adrReg)
int setDVOUTBigModbusRegister(int adrReg, int dataReg)
{
  //записать содержимое регистра
  if(privateDVOUTBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(dvoutbigcomponent->isActiveActualData)
    {
      edition_settings = current_settings_interfaces;//делаем копию
    }//if(uprbigcomponent->isActiveActualData)
  superClearActiveActualData();

  superSetOperativMarker(dvoutbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//записать в буфер
  //проверка на допустимость
  return validN_BIGACMD(dataReg);
}//getDVOUTBigModbusRegister(int adrReg)
int setDVOUTBigModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //получить содержимое регистра
  return MARKER_OUTPERIMETR;
}//getDVOUTBigModbusRegister(int adrReg)

int postDVOUTBigWriteAction(void)
{
extern int upravlSchematic;//флаг Rang
//action после записи
  int beginAdr = dvoutbigcomponent->operativMarker[0];
  if(beginAdr<0) return 0;//не было записи
  int endAdr   = dvoutbigcomponent->operativMarker[1];
  int countAdr = endAdr-beginAdr+1;
  if(endAdr<0) countAdr = 1;

#if (                                \
     (MODYFIKACIA_VERSII_PZ == 8) || \
     (MODYFIKACIA_VERSII_PZ == 18)   \
    )
  if(beginAdr >= 4396)
    writeRangN_SMALLModbusRegister(&edition_settings.ranguvannja_inputs[20*N_SMALL], REGISTERS_DV, beginAdr,
                                   countAdr, 4396);
  else writeRangN_BIGModbusRegister(&edition_settings.ranguvannja_outputs[20*N_BIG], REGISTERS_OUTPUTS, beginAdr,
                                   countAdr, 4332);

      //¬јЋ»ƒј÷»я ”—ѕ≈ЎЌј - ”—“јЌќ¬ ј
     upravlSchematic = 1;//флаг Rang
#endif

  return 0;
}//

int privateDVOUTBigGetReg2(int adrReg)
{
  //проверить внешний периметр
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)
