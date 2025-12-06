
#include "header.h"

//начальный регистр в карте памяти
#define BEGIN_ADR_REGISTER 61904
//конечный регистр в карте памяти
#define END_ADR_REGISTER 61921

int getQAISmallModbusRegister(int);//получить содержимое регистра
int getQAISmallModbusBit(int);//получить содержимое бита
int setQAISmallModbusRegister(int, int);//получить содержимое регистра
int setQAISmallModbusBit(int, int);//получить содержимое бита

int  postQAISmallWriteAction(void);//action после записи
void loadQAISmallActualData(void);

int privateQAISmallGetReg2(int adrReg);

SRAM1 COMPONENT_OBJ *qaismallcomponent;

/**************************************/
//компонент Измерения по методу суммы квадратов
/**************************************/
void constructorQAISmallComponent(COMPONENT_OBJ *qaismallcomp)
{
  qaismallcomponent = qaismallcomp;

  qaismallcomponent->getModbusRegister = getQAISmallModbusRegister;//получить содержимое регистра
  qaismallcomponent->getModbusBit      = getQAISmallModbusBit;//получить содержимое бита
  qaismallcomponent->setModbusRegister = setQAISmallModbusRegister;//получить содержимое регистра
  qaismallcomponent->setModbusBit      = setQAISmallModbusBit;//получить содержимое бита

  qaismallcomponent->postWriteAction = postQAISmallWriteAction;//action после записи
}//constructorIUSmallComponent(COMPONENT_OBJ *iucomp)

int getQAISmallModbusRegister(int adrReg)
{
  //получить содержимое регистра
  if(privateQAISmallGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
    int offset = adrReg-BEGIN_ADR_REGISTER;
    switch (offset)
    {
    case 3://Ток емкостной 3Io
      {
        return (measurement_low[IM_3I0_i]) &0xFFFF;
      }
    }//switch

    return 0;
}//getDVModbusRegister(int adrReg)
int getQAISmallModbusBit(int x) {
  //получить содержимое bit
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getIUModbusBit(int )
int setQAISmallModbusRegister(int x, int y) {
  UNUSED(x);
  UNUSED(y);
  //записать содержимое регистра
  return MARKER_OUTPERIMETR;
}//setIUModbusRegister(int, int)
int setQAISmallModbusBit(int x, int y) {
  UNUSED(x);
  UNUSED(y);
  //записать содержимое bit
  return MARKER_OUTPERIMETR;
}//setIUModbusBit(int, int )

int postQAISmallWriteAction(void) {
//action после записи
 return 0;
}//

int privateQAISmallGetReg2(int adrReg)
{
  //проверить внешний периметр
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)

