
#include "header.h"
#include "variables_external_m.h"

int privateREGDISvalidRecordLen(int recordNumber, int recordLen);
int privateREGDISSelUstRecordNumber(int recordNumber);
int privateREGDISSelRangRecordNumber(int recordNumber);
int getFileREGDISRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileREGDISRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileREGDISRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

#define REGISTERS_REG 32

int privateREGDISSelUstRecordNumber(int recordNumber)
{
  if(recordNumber == 1) return 1;
  return 0;
}//privateREGDISvalidRecordNumber

int privateREGDISvalidRecordLen(int recordNumber, int recordLen)
{
//валидация recordLen
  if(recordNumber == 1) if(recordLen > 2) return MARKER_ERRORPERIMETR;
  if(recordNumber == 2) if(recordLen > REGISTERS_REG+1) return MARKER_ERRORPERIMETR;
  return 0;
}//privateREGDISvalidRecordLen

int privateREGDISSelRangRecordNumber(int recordNumber)
{
  if(recordNumber == 2) return 1;
  return 0;
}//privateREGDISSelRangRecordNumber

////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getFileREGDISRegister20(int recordNumber, int registerNumber, int recordLen)
{
//валидация recordLen
  if(privateREGDISvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;
  if(registerNumber == 0) return 0;

  if(privateREGDISSelUstRecordNumber(recordNumber))
    return GET_UST_REGISTER000FOR20(FILEPERIF_REGDISREGISTER2021);
  else if(privateREGDISSelRangRecordNumber(recordNumber))
  {
    //Ранжирование REGDIS
    return getRangN_BIGModbusRegister2021(current_settings_interfaces.ranguvannja_digital_registrator,
                                          registerNumber-1);
  }//if

  return MARKER_ERRORPERIMETR;
}//getFileREGDISRegister20

////-SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET-
int setFileREGDISRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
//валидация recordLen
  if(privateREGDISvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;

  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    if(privateREGDISSelUstRecordNumber(recordNumber))
    {
      if(SET_UST_REGISTER000FOR21(FILEPERIF_REGDISREGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
    }//if

    else if(privateREGDISSelRangRecordNumber(recordNumber))
    {
      //Ранжирование REGDIS
      if(validN_BIGACMD(value) != 0) return MARKER_ERRORDIAPAZON;
    }//if

    else return MARKER_ERRORPERIMETR;

  }//for
  return 0;
}//setFileREGDISRegister21

////-POST--POST--POST--POST--POST--POST--POST--POST--POST--POST--POST-
int postFileREGDISRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
  extern int upravlSchematic;//Rang
  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    if(privateREGDISSelUstRecordNumber(recordNumber))
    {
      int errorV = POST_UST_REGISTER000FOR21(FILEPERIF_REGDISREGISTER2021);
      if(errorV == 1) continue;
    }//if

  }//for

  if(privateREGDISSelRangRecordNumber(recordNumber))
  {
    writeRangN_BIGModbusRegister2021(edition_settings.ranguvannja_digital_registrator, &dataPacket[1], recordLen-1);
    //ВАЛИДАЦИЯ УСПЕШНА - УСТАНОВКА
    upravlSchematic = 1;//флаг Rang
  }//if

  return 0;
}//postFileREGDISRegister21
