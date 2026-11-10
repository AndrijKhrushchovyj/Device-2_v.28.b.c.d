
#include "header.h"
#include "variables_external_m.h"

int privateELNOTvalidRecordLen(int recordNumber, int recordLen);
int privateELNOTRangRecordNumber(int recordNumber);
int getFileELNOTRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileELNOTRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileELNOTRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

#define REGISTERS_NOT 1

int privateELNOTvalidRecordLen(int recordNumber, int recordLen)
{
//валидация recordNumber
  if(recordNumber > NUMBER_DEFINED_NOT) return MARKER_ERRORPERIMETR;
//валидация recordLen
  if(recordLen > REGISTERS_NOT+1) return MARKER_ERRORPERIMETR;
  return 0;
}//privateELNOTvalidRecordLen

int privateELNOTRangRecordNumber(int recordNumber)
{
  if(recordNumber > 0) return 1;
  return 0;
}//privateELNOTRangRecordNumber

////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getFileELNOTRegister20(int recordNumber, int registerNumber, int recordLen)
{
//валидация recordLen
  if(privateELNOTvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;
  if(registerNumber == 0) return 0;

  if(privateELNOTRangRecordNumber(recordNumber))
  {
    //Ранжирование
    return getRangN_BIGModbusRegister2021(&current_settings_interfaces.ranguvannja_d_not[(recordNumber-1)*N_BIG],
                                          registerNumber-1);
  }//if

  return MARKER_ERRORPERIMETR;
}//getFileELNOTRegister20

////-SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET-
int setFileELNOTRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
//валидация recordLen
  if(privateELNOTvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;

  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    if(privateELNOTRangRecordNumber(recordNumber))
    {
      //проверка на конфиг
      if(!(edition_settings.configuration&(1<<EL_BIT_CONFIGURATION))) return MARKER_ERRORPERIMETR;
      //Ранжирование
      if(validN_BIGACMD(value) != 0) return MARKER_ERRORDIAPAZON;
    }//if

    else return MARKER_ERRORPERIMETR;

  }//for
  return 0;
}//setFileELNOTRegister21

////-POST--POST--POST--POST--POST--POST--POST--POST--POST--POST--POST-
int postFileELNOTRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
  extern int upravlSchematic;//Rang

  if(privateELNOTRangRecordNumber(recordNumber))
  {
    writeRangN_BIGModbusRegister2021(&edition_settings.ranguvannja_d_not[(recordNumber-1)*N_BIG], &dataPacket[1], recordLen-1);
    //ВАЛИДАЦИЯ УСПЕШНА - УСТАНОВКА
    upravlSchematic = 1;//флаг Rang
  }//if

  return 0;
}//postFileELNOTRegister21
