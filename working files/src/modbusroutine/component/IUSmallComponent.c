#include "header.h"

//начальный регистр в карте памяти
#define BEGIN_ADR_REGISTER 0
//конечный регистр в карте памяти
#define END_ADR_REGISTER 15

int getIUSmallModbusRegister(int);//получить содержимое регистра
int getIUSmallModbusBit(int);//получить содержимое бита
int setIUSmallModbusRegister(int, int);//получить содержимое регистра
int setIUSmallModbusBit(int, int);//получить содержимое бита

int postIUSmallWriteAction(void);//action после записи

int privateIUGetReg2(int adrReg);

SRAM1 COMPONENT_OBJ *iucomponent;

/**************************************/
//подготовка компонента идентификация устройства
/**************************************/
void constructorIUSmallComponent(COMPONENT_OBJ *iucomp)
{
  iucomponent = iucomp;

  iucomponent->getModbusRegister = getIUSmallModbusRegister;//получить содержимое регистра
  iucomponent->getModbusBit      = getIUSmallModbusBit;//получить содержимое бита
  iucomponent->setModbusRegister = setIUSmallModbusRegister;//получить содержимое регистра
  iucomponent->setModbusBit      = setIUSmallModbusBit;//получить содержимое бита

  iucomponent->postWriteAction = postIUSmallWriteAction;//action после записи
}//prepareDVinConfig

int getIUSmallModbusRegister(int adrReg)
{
  //получить содержимое регистра
  if(privateIUGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

//  superSetOperativMarker(iucomponent, adrReg);

  int offset = adrReg-BEGIN_ADR_REGISTER;
  switch(offset) {//индекс регистра 
    case 0://MA_SERIAL_NUMBER:
        return serial_number_dev;//serial_number_dev;

    case 1://MA_NAME_CHAR_1_2:
        return ('K' << 8) + 'i';

    case 2://MA_NAME_CHAR_3_4:
        return ('e' << 8) + 'v';

    case 3://MA_NAME_CHAR_5_6:
        return ('p' << 8) + 'r';

    case 4://MA_NAME_CHAR_7_8:
        return ('i' << 8) + 'b';

    case 5://MA_NAME_CHAR_9_10:
        return ('o' << 8) + 'r';

    case 6://MA_NAME_CHAR_11_12:
        return ('M' << 8) + 'R';

    case 7://MA_NAME_CHAR_13_14:
        return ('Z' << 8) + 'S';

    case 8://MA_NAME_CHAR_15_16:
        return ('0' << 8) + '5';

    case 9://MA_NAME_CHAR_17_18:
        return ('L' << 8) + '_';

    case 10://MA_VERSION_SW:
        return (VERSIA_PZ << 8) + MODYFIKACIA_VERSII_PZ;

    case 11://MA_DATA_COMP_1:
        return (YEAR_VER << 8) + MONTH_VER;

    case 12://MA_DATA_COMP_2:
        return (DAY_VER << 8);

    case 13://MA_TIME_COMP:
        return (HOUR_VER << 8) + MINUTE_VER;

    case 14://MA_VERSION_GMM:
        return (VERSIA_GMM << 8) + MODYFIKACIA_VERSII_GMM;

    case 15://MA_ZBIRKA_SW:
        return (ZBIRKA_VERSII_PZ<< 8) + ZBIRKA_PIDVERSII_PZ;
  }//switch

  return 0;//tempReadArray[adrReg-BEGIN_ADR_REGISTER];
}//getDVModbusRegister(int adrReg)

int getIUSmallModbusBit(int x) {
  //получить содержимое bit
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getIUModbusBit(int )
int setIUSmallModbusRegister(int x, int y) {
  UNUSED(x);
  UNUSED(y);
  //записать содержимое регистра
  return MARKER_OUTPERIMETR;
}//setIUModbusRegister(int, int)
int setIUSmallModbusBit(int x, int y) {
  UNUSED(x);
  UNUSED(y);
  //записать содержимое bit
  return MARKER_OUTPERIMETR;
}//setIUModbusBit(int, int )

int postIUSmallWriteAction(void) {
//action после записи
 return 0;
}//

int privateIUGetReg2(int adrReg)
{
  //проверить внешний периметр
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)


