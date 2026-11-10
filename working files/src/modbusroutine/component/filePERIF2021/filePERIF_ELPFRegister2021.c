
#include "header.h"
#include "variables_external_m.h"

int privateELPFvalidRecordLen(int recordNumber, int recordLen);
int privateELPFRangRecordNumber(int recordNumber);
int getFileELPFRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileELPFRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileELPFRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

#define REGISTERS_ELPF 2

int privateELPFvalidRecordLen(int recordNumber, int recordLen)
{
//валидация recordNumber
  if(recordNumber > NUMBER_TRANSFER_FUNCTIONS*REGISTERS_ELPF) return MARKER_ERRORPERIMETR;
//валидация recordLen
  if(recordLen > REGISTERS_ELPF+1) return MARKER_ERRORPERIMETR;
  return 0;
}//privateELPFvalidRecordLen

int privateELPFRangRecordNumber(int recordNumber)
{
  if(recordNumber > 0) return 1;
  return 0;
}//privateELPFRangRecordNumber

////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getFileELPFRegister20(int recordNumber, int registerNumber, int recordLen)
{
//валидация recordLen
  if(privateELPFvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;
  if(registerNumber == 0) return 0;

  if(privateELPFRangRecordNumber(recordNumber))
  {
    //Ранжирование
    int item = (recordNumber-1);
    int bitInOut  = current_settings_interfaces.ranguvannja_tf[item];
    int bit = -1;
    int adr = -1;
    switch((registerNumber-1) % REGISTERS_ELPF)
    {
    case 0:
      bit = (bitInOut&0xFFFF)-1;
      if(bit!=-1)
      {
        adr = decoderN_BIGACMD(bit);
      }//if(bit!=-1)
      break;
    case 1:
      bit = ((bitInOut>>16)&0xFFFF)-1;
      if(bit!=-1)
      {
        adr = decoderN_SMALLACMD(bit);
      }//if(bit!=-1)
      break;
    }//switch

    return adr<0? 0: adr+getACMDSmallBeginAdr();//декодировщик индекса бита в адрес modbus
  }//if

  return MARKER_ERRORPERIMETR;
}//getFileELPFRegister20

////-SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET-
int setFileELPFRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
//валидация recordLen
  if(privateELPFvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;

  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    if(privateELPFRangRecordNumber(recordNumber))
    {
      //проверка на конфиг
      if(!(edition_settings.configuration&(1<<EL_BIT_CONFIGURATION))) return MARKER_ERRORPERIMETR;

      switch((registerNumber-1) % REGISTERS_ELPF)
      {
      case 0:
        if(!value) return 0;
        return validN_BIGACMD(value);
      case 1:
        if(!value) return 0;
        return validN_SMALLACMD(value);
      }//switch
    }//if

    else return MARKER_ERRORPERIMETR;

  }//for
  return 0;
}//setFileELPFRegister21

////-POST--POST--POST--POST--POST--POST--POST--POST--POST--POST--POST-
int postFileELPFRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
  extern int upravlSchematic;//Rang

  if(privateELPFRangRecordNumber(recordNumber))
  {
    int item = (recordNumber-1);
    int bit = -1;

    for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
    {
      if(registerNumber == 0) continue;

      uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);
      switch((registerNumber-1) % REGISTERS_ELPF)
      {
      case 0:
        if(value == 0) bit = -2;
        else bit = encoderN_BIGACMD(value - getACMDSmallBeginAdr()); //кодировщик адреса modbus в индекс бита для реле

        if(bit != -1) {
          if(bit == -2) bit = -1;
          edition_settings.ranguvannja_tf[item] &= (uint32_t)~0xffff;
          edition_settings.ranguvannja_tf[item] |= (bit+1);
        }//if
        break;

      case 1:
        if(value == 0) bit = -2;
        else bit = encoderN_SMALLACMD(value - getACMDSmallBeginAdr()); //кодировщик адреса modbus в индекс бита для dv

        if(bit != -1) {
          if(bit == -2) bit = -1;
          edition_settings.ranguvannja_tf[item] &= (uint32_t)~(0xffff<<16);
          edition_settings.ranguvannja_tf[item] |= ((bit+1)<<16);//
        }//if
        break;
      }//switch
    }//for
    upravlSchematic = 1;//флаг Rang
  }//if

  return 0;
}//postFileELPFRegister21
