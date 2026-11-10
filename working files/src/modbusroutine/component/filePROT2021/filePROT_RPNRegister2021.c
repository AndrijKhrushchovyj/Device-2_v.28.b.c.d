
#include "header.h"
#include "variables_external_m.h"

int privateRPNvalidRecordLen(int recordNumber, int recordLen);
int privateRPNSelUstRecordNumber(int recordNumber);
int privateRPNSelCountGroupBitRecordNumber(int recordNumber);
int getFileRPNRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileRPNRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileRPNRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

int privateRPNSelUstRecordNumber(int recordNumber)
{
  if(recordNumber >= 7 && recordNumber <= 14) return 1;
  if(recordNumber >= 20 && recordNumber <= 23) return 1;

  return 0;
}//privateRPNvalidRecordNumber

int privateRPNvalidRecordLen(int recordNumber, int recordLen)
{
//валидация recordLen
  switch(recordNumber)
  {
  case 0:
  case 1:
  case 20: case 21: case 22: case 23:
    if(recordLen>2) return MARKER_ERRORPERIMETR;
    break;
  default:
    if(recordLen>5) return MARKER_ERRORPERIMETR;
  }//switch
  return 0;
}//privateRPNvalidRecordLen

int privateRPNSelCountGroupBitRecordNumber(int recordNumber)
{
  if(recordNumber == 1)
    return 9;//к-сть Group біт
  return -1;
}//privateRPNSelCountGroupBitRecordNumber
////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getFileRPNRegister20(int recordNumber, int registerNumber, int recordLen)
{
//валидация recordLen
  if(privateRPNvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;
  if(registerNumber == 0) return 0;

  int  countGroupBit = privateRPNSelCountGroupBitRecordNumber(recordNumber);//к-сть Group біт

  if(recordNumber == 0) return GET_UPR_REGISTER000FOR20(FILEPERIF_RPNREGISTER2021);

  else if(countGroupBit > 0)
  {
    return GET_GROUPUPR_REGISTER000FOR20(FILEPERIF_RPNREGISTER2021);
  }//if(countGroupBit>0)
  else if(privateRPNSelUstRecordNumber(recordNumber))
  {
    return GET_UST_REGISTER000FOR20(FILEPERIF_RPNREGISTER2021);
  }

  return MARKER_ERRORPERIMETR;
}//getFileRPNRegister20

////-SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET-
int setFileRPNRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
//валидация recordLen
  if(privateRPNvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;

  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);
    int  countGroupBit = privateRPNSelCountGroupBitRecordNumber(recordNumber);//к-сть Group біт

    if(recordNumber == 0)
    {
      if(SET_UPR_REGISTER000FOR21(FILEPERIF_RPNREGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
    }//if

    else if(countGroupBit > 0)
    {
      if(SET_GROUPUPR_REGISTER000FOR21(FILEPERIF_RPNREGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
    }//if(countGroupBit>0)

    else if(privateRPNSelUstRecordNumber(recordNumber))
    {
      if(SET_UST_REGISTER000FOR21(FILEPERIF_RPNREGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
    }//if
    else return MARKER_ERRORPERIMETR;

  }//for
  return 0;
}//setFileRPNRegister21

////-POST--POST--POST--POST--POST--POST--POST--POST--POST--POST--POST-
int postFileRPNRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    int  countGroupBit = privateRPNSelCountGroupBitRecordNumber(recordNumber);//к-сть Group біт

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    if(recordNumber == 0)
    {
      int errorV = POST_UPR_REGISTER000FOR21(FILEPERIF_RPNREGISTER2021);
      if(errorV == 1) continue;
      if(errorV != 0) return errorV;
    }//if

    else if(countGroupBit>0)
    {
      int errorV = POST_GROUPUPR_REGISTER000FOR21(FILEPERIF_RPNREGISTER2021);
      if(errorV != 0) return errorV;
    }//if(countGroupBit>0)

    else if(privateRPNSelUstRecordNumber(recordNumber))
    {
      int errorV = POST_UST_REGISTER000FOR21(FILEPERIF_RPNREGISTER2021);
      if(errorV == 1) continue;
    }//if
  }//for


  return 0;
}//postFileRPNRegister21
