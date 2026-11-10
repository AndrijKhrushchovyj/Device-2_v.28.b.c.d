
#include "header.h"
#include "variables_external_m.h"

int privateELANDvalidRecordLen(int recordNumber, int recordLen);
int privateELANDRangRecordNumber(int recordNumber);
int getFileELANDRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileELANDRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileELANDRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

#define REGISTERS_AND 8

int privateELANDvalidRecordLen(int recordNumber, int recordLen)
{
//валидация recordNumber
  if(recordNumber > NUMBER_DEFINED_AND) return MARKER_ERRORPERIMETR;
//валидация recordLen
  if(recordNumber == 0) if(recordLen > 2) return MARKER_ERRORPERIMETR;
  if(recordNumber > 0)  if(recordLen > REGISTERS_AND+1) return MARKER_ERRORPERIMETR;
  return 0;
}//privateELANDvalidRecordLen

int privateELANDRangRecordNumber(int recordNumber)
{
  if(recordNumber > 0) return 1;
  return 0;
}//privateELANDRangRecordNumber

////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getFileELANDRegister20(int recordNumber, int registerNumber, int recordLen)
{
//валидация recordLen
  if(privateELANDvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;
  if(registerNumber == 0) return 0;

  if(recordNumber == 0) return GET_UPR_REGISTER000FOR20(FILEPERIF_ELANDREGISTER2021);
  else if(privateELANDRangRecordNumber(recordNumber))
  {
    //Ранжирование
    return getRangN_BIGModbusRegister2021(&current_settings_interfaces.ranguvannja_d_and[(recordNumber-1)*N_BIG],
                                          registerNumber-1);
  }//if

  return MARKER_ERRORPERIMETR;
}//getFileELANDRegister20

////-SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET-
int setFileELANDRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
//валидация recordLen
  if(privateELANDvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;

  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    if(recordNumber == 0)
    {
      if(SET_UPR_REGISTER000FOR21(FILEPERIF_ELANDREGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
    }//if
    else if(privateELANDRangRecordNumber(recordNumber))
    {
      //проверка на конфиг
      if(!(edition_settings.configuration&(1<<EL_BIT_CONFIGURATION))) return MARKER_ERRORPERIMETR;
      //Ранжирование
      if(validN_BIGACMD(value) != 0) return MARKER_ERRORDIAPAZON;
    }//if

    else return MARKER_ERRORPERIMETR;

  }//for
  return 0;
}//setFileELANDRegister21

////-POST--POST--POST--POST--POST--POST--POST--POST--POST--POST--POST-
int postFileELANDRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
  extern int upravlSchematic;//Rang
  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    if(recordNumber == 0)
    {
      int errorV = POST_UPR_REGISTER000FOR21(FILEPERIF_ELANDREGISTER2021);
      if(errorV == 1) continue;
      if(errorV != 0) return errorV;
    }//if
  }//for

  if(privateELANDRangRecordNumber(recordNumber))
  {
    writeRangN_BIGModbusRegister2021(&edition_settings.ranguvannja_d_and[(recordNumber-1)*N_BIG], &dataPacket[1], recordLen-1);
    //ВАЛИДАЦИЯ УСПЕШНА - УСТАНОВКА
    upravlSchematic = 1;//флаг Rang
  }//if

  return 0;
}//postFileELANDRegister21
