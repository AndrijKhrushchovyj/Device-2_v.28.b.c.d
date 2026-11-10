
#include "header.h"
#include "variables_external_m.h"

int privateREGANLvalidRecordLen(int recordNumber, int recordLen);
int privateREGANLSelUstRecordNumber(int recordNumber);
int privateREGANLSelRangRecordNumber(int recordNumber);
int getFileREGANLRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileREGANLRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileREGANLRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

#define REGISTERS_REG 32

int privateREGANLSelUstRecordNumber(int recordNumber)
{
  if(recordNumber == 1) return 1;
  return 0;
}//privateREGANLvalidRecordNumber

int privateREGANLvalidRecordLen(int recordNumber, int recordLen)
{
//валидация recordLen
  if(recordNumber == 0) if(recordLen > 2) return MARKER_ERRORPERIMETR;
  if(recordNumber == 1) if(recordLen > 3) return MARKER_ERRORPERIMETR;
  if(recordNumber == 2) if(recordLen > REGISTERS_REG+1) return MARKER_ERRORPERIMETR;
  return 0;
}//privateREGANLvalidRecordLen

int privateREGANLSelRangRecordNumber(int recordNumber)
{
  if(recordNumber == 2) return 1;
  return 0;
}//privateREGANLSelRangRecordNumber

////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getFileREGANLRegister20(int recordNumber, int registerNumber, int recordLen)
{
//валидация recordLen
  if(privateREGANLvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;
  if(registerNumber == 0) return 0;

  if(recordNumber == 0) return GET_UPR_REGISTER000FOR20(FILEPERIF_REGANLREGISTER2021);
  else if(privateREGANLSelUstRecordNumber(recordNumber))
    return GET_UST_REGISTER000FOR20(FILEPERIF_REGANLREGISTER2021);
  else if(privateREGANLSelRangRecordNumber(recordNumber))
  {
    //Ранжирование REGANL
    return getRangN_BIGModbusRegister2021(current_settings_interfaces.ranguvannja_analog_registrator,
                                          registerNumber-1);
  }//if

  return MARKER_ERRORPERIMETR;
}//getFileREGANLRegister20

////-SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET-
int setFileREGANLRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
//валидация recordLen
  if(privateREGANLvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;

  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    if(recordNumber == 0)
    {
      if(SET_UPR_REGISTER000FOR21(FILEPERIF_REGANLREGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
    }//if
    else if(privateREGANLSelUstRecordNumber(recordNumber))
    {
      if(SET_UST_REGISTER000FOR21(FILEPERIF_REGANLREGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
    }//if

    else if(privateREGANLSelRangRecordNumber(recordNumber))
    {
      //Ранжирование REGANL
      if(validN_BIGACMD(value) != 0) return MARKER_ERRORDIAPAZON;
    }//if

    else return MARKER_ERRORPERIMETR;

  }//for
  return 0;
}//setFileREGANLRegister21

////-POST--POST--POST--POST--POST--POST--POST--POST--POST--POST--POST-
int postFileREGANLRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
  extern int upravlSchematic;//Rang
  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    if(recordNumber == 0)
    {
      int errorV = POST_UPR_REGISTER000FOR21(FILEPERIF_REGANLREGISTER2021);
      if(errorV == 1) continue;
      if(errorV != 0) return errorV;
    }//if
    else if(privateREGANLSelUstRecordNumber(recordNumber))
    {
      int errorV = POST_UST_REGISTER000FOR21(FILEPERIF_REGANLREGISTER2021);
      if(errorV == 1) continue;
    }//if

  }//for

  if(privateREGANLSelRangRecordNumber(recordNumber))
  {
    writeRangN_BIGModbusRegister2021(edition_settings.ranguvannja_analog_registrator, &dataPacket[1], recordLen-1);
    //ВАЛИДАЦИЯ УСПЕШНА - УСТАНОВКА
    upravlSchematic = 1;//флаг Rang
  }//if

  return 0;
}//postFileREGANLRegister21
