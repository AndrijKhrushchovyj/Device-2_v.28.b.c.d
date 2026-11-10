//начальный регистр в карте памяти
#define BEGIN_ADR_REGISTER 1800
//конечный регистр в карте памяти
#define END_ADR_REGISTER 1802

#include "header.h"
#include "variables_external_m.h"

int privateREG2BigGetReg2(int adrReg);

int getREG2BigModbusRegister(int);//получить содержимое регистра
int getREG2BigModbusBit(int);//получить содержимое бита
int setREG2BigModbusRegister(int, int);//получить содержимое регистра
int setREG2BigModbusBit(int, int);//получить содержимое бита

int  postREG2BigWriteAction(void);//action после записи
int passwordImunitetRegREG2BigComponent(int adrReg);

SRAM1 COMPONENT_OBJ *reg2bigcomponent;

/**************************************/
//подготовка компонента REGistrator
/**************************************/
void constructorREG2BigComponent(COMPONENT_OBJ *reg2bigcomp)
{
  reg2bigcomponent = reg2bigcomp;

//  reg2bigcomponent->getModbusRegister = getREG2BigModbusRegister;//получить содержимое регистра
//  reg2bigcomponent->getModbusBit      = getREG2BigModbusBit;//получить содержимое бита
//  reg2bigcomponent->setModbusRegister = setREG2BigModbusRegister;//получить содержимое регистра
//  reg2bigcomponent->setModbusBit      = setREG2BigModbusBit;//получить содержимое бита

//  reg2bigcomponent->postWriteAction = postREG2BigWriteAction;//action после записи
}//constructorREGBigComponent

int getREG2BigModbusRegister(int adrReg)
{
  //получить содержимое регистра
  if(privateREG2BigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  int offset = adrReg-BEGIN_ADR_REGISTER;

  switch(offset)
    {
    case 0://Количество аналоговых регистраторов
      {
        unsigned int first_number = (info_rejestrator_ar.first_number < 0) ? 0 : (info_rejestrator_ar.first_number + 1);
        unsigned int last_number  = (info_rejestrator_ar.last_number  < 0) ? 0 : (info_rejestrator_ar.last_number + 1);
      
        unsigned int number_records;
        if (first_number == 0) number_records = 0;
        else if (first_number >= last_number) number_records = first_number - last_number + 1;
        else number_records = NUMBER_FATFS_NAME - last_number + first_number + 1;
                
        return (number_records) & 0xFFFF;
      }

    case 1://Количество дискретных регистраторов
      return (info_rejestrator_dr.number_records) &0xFFFF;

    case 2://Количество записей rds
      return  GetAmountFixElem()&0xFFFF;
    }//switch

  return 0;
}//getDOUTBigModbusRegister(int adrReg)
int getREG2BigModbusBit(int x)
{
  //получить содержимое регистра
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setREG2BigModbusRegister(int adrReg, int dataReg)
{
  UNUSED(adrReg);
  UNUSED(dataReg);
  //записать содержимое регистра
  return MARKER_OUTPERIMETR;
}//
int setREG2BigModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //получить содержимое регистра
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

int postREG2BigWriteAction(void)
{
  return 0;
}//

int privateREG2BigGetReg2(int adrReg)
{
  //проверить внешний периметр
  return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)

