#include "header.h"

//начальный регистр в карте памяти
#define BEGIN_ADR_REGISTER 2380
//конечный регистр в карте памяти
#define END_ADR_REGISTER 2443

#define REGISTERS_VV 32

int privateVVBigGetReg2(int adrReg);

int getVVBigModbusRegister(int);//получить содержимое регистра
int getVVBigModbusBit(int);//получить содержимое бита
int setVVBigModbusRegister(int, int);// регистра
int setVVBigModbusBit(int, int);// бита

int  postVVBigWriteAction(void);//action после записи

SRAM1 COMPONENT_OBJ *vvbigcomponent;

/**************************************/
//подготовка компонента VV
/**************************************/
void constructorVVBigComponent(COMPONENT_OBJ *vvbigcomp)
{
  vvbigcomponent = vvbigcomp;

  vvbigcomponent->getModbusRegister = getVVBigModbusRegister;//получить содержимое регистра
  vvbigcomponent->getModbusBit      = getVVBigModbusBit;//получить содержимое бита
  vvbigcomponent->setModbusRegister = setVVBigModbusRegister;// регистра
  vvbigcomponent->setModbusBit      = setVVBigModbusBit;// бита

  vvbigcomponent->postWriteAction = postVVBigWriteAction;//action после записи
}//prepareDVinConfig

int getVVBigModbusRegister(int adrReg)
{
  //получить содержимое регистра
  if(privateVVBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  int offset = adrReg-BEGIN_ADR_REGISTER;
//поиск активного бита
  unsigned int *ranguvannja_vv = &current_settings_interfaces.ranguvannja_on_cb[0]; //Ранжування прямих
  if((offset/REGISTERS_VV)==1)
       ranguvannja_vv = &current_settings_interfaces.ranguvannja_off_cb[0]; //Ранжування інверсних 

  int bit = getSequenceN_BIGIndexActiveBit(offset%REGISTERS_VV, ranguvannja_vv);//индекс активного бита
  if(bit!=-1)
    {
      int adr = decoderN_BIGACMD(bit);
      return adr<0? 0: adr+getACMDSmallBeginAdr();//декодировщик индекса бита в адрес modbus  для rele
    }//if(bit!=N_BIG*32)
  return 0;
}//getDOUTBigModbusRegister(int adrReg)
int getVVBigModbusBit(int x)
{
  //получить содержимое регистра
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setVVBigModbusRegister(int adrReg, int dataReg)
{
  //записать содержимое регистра
  if(privateVVBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(vvbigcomponent->isActiveActualData)
    {
      edition_settings = current_settings_interfaces;//делаем копию
    }//if(uprbigcomponent->isActiveActualData)
  superClearActiveActualData();

  //валидация //Работа БО //Работа БB
  if(adrReg>=(BEGIN_ADR_REGISTER+REGISTERS_VV))
  {
   //ранжир ВВ откл
   if(dataReg==50532) return MARKER_ERRORPERIMETR;//Работа БО
   if(dataReg==50595) return MARKER_ERRORPERIMETR;//Отключение от защит
  }
  else {
   //ранжир ВВ вкл
   if(dataReg==50533) return MARKER_ERRORPERIMETR;//Работа БВ
  }

  superSetOperativMarker(vvbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//записать в буфер

  return validN_BIGACMD(dataReg);
}//getDOUTBigModbusRegister(int adrReg)
int setVVBigModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //получить содержимое регистра
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

int postVVBigWriteAction(void) {
extern int upravlSchematic;//флаг Rang
//action после записи
  int beginAdr = vvbigcomponent->operativMarker[0];
  if(beginAdr<0) return 0;//не было записи
  int endAdr   = vvbigcomponent->operativMarker[1];
  int countAdr = endAdr-beginAdr+1;
  if(endAdr<0) countAdr = 1;
  int offsetTempWriteArray = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//найти смещение TempWriteArray

  unsigned int tmp[N_BIG];//для сохр ранжирования
  int idxObjOld=-1;//индекс суб обекта old
  //убрать старое
  for(int i=0; i<countAdr; i++)
    {
      int offset = beginAdr-BEGIN_ADR_REGISTER+i;

      unsigned int *ranguvannja_vv = &edition_settings.ranguvannja_on_cb[0]; //Ранжуванн
      if((offset/REGISTERS_VV)==1)
           ranguvannja_vv = &edition_settings.ranguvannja_off_cb[0]; //Ранжування 

      if((offset/REGISTERS_VV)!=idxObjOld) {
       idxObjOld = offset/REGISTERS_VV;
       for(int dx=0; dx<N_BIG; dx++) tmp[dx]=ranguvannja_vv[dx];//сохр старое ранж
      }//if

      int bitOld = getSequenceN_BIGIndexActiveBit(offset%REGISTERS_VV, tmp);//индекс активного бита
      if(bitOld!=-1) {
        ranguvannja_vv[bitOld/REGISTERS_VV] &= ~(1<<(bitOld%REGISTERS_VV));
      }//if
    }//for
  //добавить новое
  for(int i=0; i<countAdr; i++)
    {
      unsigned int adr = (unsigned short)tempWriteArray[offsetTempWriteArray+i];//новое значение
      if(adr==0) continue;
      int offset = beginAdr-BEGIN_ADR_REGISTER+i;

      unsigned int *ranguvannja_vv = &edition_settings.ranguvannja_on_cb[0]; //Ранжуванн
      if((offset/REGISTERS_VV)==1)
           ranguvannja_vv = &edition_settings.ranguvannja_off_cb[0]; //Ранжування 

      int bit = encoderN_BIGACMD(adr-getACMDSmallBeginAdr()); //кодировщик адреса modbus в индекс бита для реле
      if(bit!=-1) {
        ranguvannja_vv[bit/REGISTERS_VV] |= (1<<(bit%REGISTERS_VV));
      }//if
    }//for

      //ВАЛИДАЦИЯ УСПЕШНА - УСТАНОВКА
     upravlSchematic = 1;//флаг Rang

  return 0;
}//

int privateVVBigGetReg2(int adrReg)
{
  //проверить внешний периметр
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)
