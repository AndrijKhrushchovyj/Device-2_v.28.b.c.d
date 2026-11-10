
#include "header.h"
#include "variables_external_m.h"

int privateELXORvalidRecordLen(int recordNumber, int recordLen);
int privateELXORRangRecordNumber(int recordNumber);
int getFileELXORRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileELXORRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileELXORRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

#define REGISTERS_XOR 2

int privateELXORvalidRecordLen(int recordNumber, int recordLen)
{
//валидация recordNumber
  if(recordNumber > NUMBER_DEFINED_XOR) return MARKER_ERRORPERIMETR;
//валидация recordLen
  if(recordLen > REGISTERS_XOR+1) return MARKER_ERRORPERIMETR;
  return 0;
}//privateELXORvalidRecordLen

int privateELXORRangRecordNumber(int recordNumber)
{
  if(recordNumber > 0) return 1;
  return 0;
}//privateELXORRangRecordNumber

////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getFileELXORRegister20(int recordNumber, int registerNumber, int recordLen)
{
//валидация recordLen
  if(privateELXORvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;
  if(registerNumber == 0) return 0;

  if(privateELXORRangRecordNumber(recordNumber))
  {
    //Ранжирование
    return getRangN_BIGModbusRegister2021(&current_settings_interfaces.ranguvannja_d_xor[(recordNumber-1)*N_BIG],
                                          registerNumber-1);
  }//if

  return MARKER_ERRORPERIMETR;
}//getFileELXORRegister20

////-SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET-
int setFileELXORRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
//валидация recordLen
  if(privateELXORvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;

  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    if(privateELXORRangRecordNumber(recordNumber))
    {
      //проверка на конфиг
      if(!(edition_settings.configuration&(1<<EL_BIT_CONFIGURATION))) return MARKER_ERRORPERIMETR;
      //Ранжирование
      if(validN_BIGACMD(value) != 0) return MARKER_ERRORDIAPAZON;
    }//if

    else return MARKER_ERRORPERIMETR;

  }//for
  return 0;
}//setFileELXORRegister21

////-POST--POST--POST--POST--POST--POST--POST--POST--POST--POST--POST-
int postFileELXORRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
  extern int upravlSchematic;//Rang

  if(privateELXORRangRecordNumber(recordNumber))
  {
    writeRangN_BIGModbusRegister2021(&edition_settings.ranguvannja_d_xor[(recordNumber-1)*N_BIG], &dataPacket[1], recordLen-1);
    //ВАЛИДАЦИЯ УСПЕШНА - УСТАНОВКА
    upravlSchematic = 1;//флаг Rang
  }//if

  return 0;
}//postFileELXORRegister21
