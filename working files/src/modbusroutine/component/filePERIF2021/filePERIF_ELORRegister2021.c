
#include "header.h"
#include "variables_external_m.h"

int privateELORvalidRecordLen(int recordNumber, int recordLen);
int privateELORRangRecordNumber(int recordNumber);
int getFileELORRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileELORRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileELORRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

#define REGISTERS_OR 8

int privateELORvalidRecordLen(int recordNumber, int recordLen)
{
//валидация recordNumber
  if(recordNumber > NUMBER_DEFINED_OR) return MARKER_ERRORPERIMETR;
//валидация recordLen
  if(recordLen > REGISTERS_OR+1) return MARKER_ERRORPERIMETR;
  return 0;
}//privateELORvalidRecordLen

int privateELORRangRecordNumber(int recordNumber)
{
  if(recordNumber > 0) return 1;
  return 0;
}//privateELORRangRecordNumber

////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getFileELORRegister20(int recordNumber, int registerNumber, int recordLen)
{
//валидация recordLen
  if(privateELORvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;
  if(registerNumber == 0) return 0;

  if(privateELORRangRecordNumber(recordNumber))
  {
    //Ранжирование
    return getRangN_BIGModbusRegister2021(&current_settings_interfaces.ranguvannja_d_or[(recordNumber-1)*N_BIG],
                                          registerNumber-1);
  }//if

  return MARKER_ERRORPERIMETR;
}//getFileELORRegister20

////-SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET-
int setFileELORRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
//валидация recordLen
  if(privateELORvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;

  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    if(privateELORRangRecordNumber(recordNumber))
    {
      //проверка на конфиг
      if(!(edition_settings.configuration&(1<<EL_BIT_CONFIGURATION))) return MARKER_ERRORPERIMETR;
      //Ранжирование
      if(validN_BIGACMD(value) != 0) return MARKER_ERRORDIAPAZON;
    }//if

    else return MARKER_ERRORPERIMETR;

  }//for
  return 0;
}//setFileELORRegister21

////-POST--POST--POST--POST--POST--POST--POST--POST--POST--POST--POST-
int postFileELORRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
  extern int upravlSchematic;//Rang

  if(privateELORRangRecordNumber(recordNumber))
  {
    writeRangN_BIGModbusRegister2021(&edition_settings.ranguvannja_d_or[(recordNumber-1)*N_BIG], &dataPacket[1], recordLen-1);
    //ВАЛИДАЦИЯ УСПЕШНА - УСТАНОВКА
    upravlSchematic = 1;//флаг Rang
  }//if

  return 0;
}//postFileELORRegister21
