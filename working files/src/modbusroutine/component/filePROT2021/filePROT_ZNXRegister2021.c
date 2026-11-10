
#include "header.h"
#include "variables_external_m.h"

int privateZNXvalidRecordLen(int recordNumber, int recordLen);
int privateZNXSelUstRecordNumber(int recordNumber);
int privateZNXSelCountGroupBitRecordNumber(int recordNumber);
int getFileZNXRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileZNXRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileZNXRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

int privateZNXSelUstRecordNumber(int recordNumber)
{
  switch(recordNumber)
  {
   case 5:
     return 1;
  }//switch
  return 0;
}//privateZNXvalidRecordNumber

int privateZNXvalidRecordLen(int recordNumber, int recordLen)
{
//валидация recordLen
  switch(recordNumber)
  {
  case 0:
  case 1:
    if(recordLen>2) return MARKER_ERRORPERIMETR;
    break;
  default:
    if(recordLen>5) return MARKER_ERRORPERIMETR;
  }//switch
  return 0;
}//privateZNXvalidRecordLen

int privateZNXSelCountGroupBitRecordNumber(int recordNumber)
{
  if(recordNumber == 1)
    return 1;//к-сть Group біт
  return -1;
}//privateZNXSelCountGroupBitRecordNumber
////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getFileZNXRegister20(int recordNumber, int registerNumber, int recordLen)
{
//валидация recordLen
  if(privateZNXvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;
  if(registerNumber == 0) return 0;

  int  countGroupBit = privateZNXSelCountGroupBitRecordNumber(recordNumber);//к-сть Group біт

  if(recordNumber == 0) return GET_UPR_REGISTER000FOR20(FILEPERIF_ZNXREGISTER2021);

  else if(countGroupBit > 0)
  {
    return GET_GROUPUPR_REGISTER000FOR20(FILEPERIF_ZNXREGISTER2021);
  }//if(countGroupBit>0)
  else if(privateZNXSelUstRecordNumber(recordNumber))
  {
    return GET_UST_REGISTER000FOR20(FILEPERIF_ZNXREGISTER2021);
  }

  return MARKER_ERRORPERIMETR;
}//getFileZNXRegister20

////-SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET-
int setFileZNXRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
//валидация recordLen
  if(privateZNXvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;

  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);
    int  countGroupBit = privateZNXSelCountGroupBitRecordNumber(recordNumber);//к-сть Group біт

    if(recordNumber == 0)
    {
      if(SET_UPR_REGISTER000FOR21(FILEPERIF_ZNXREGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
    }//if

    else if(countGroupBit > 0)
    {
      if(SET_GROUPUPR_REGISTER000FOR21(FILEPERIF_ZNXREGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
    }//if(countGroupBit>0)

    else if(privateZNXSelUstRecordNumber(recordNumber))
    {
      if(SET_UST_REGISTER000FOR21(FILEPERIF_ZNXREGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
    }//if
    else return MARKER_ERRORPERIMETR;

  }//for
  return 0;
}//setFileZNXRegister21

////-POST--POST--POST--POST--POST--POST--POST--POST--POST--POST--POST-
int postFileZNXRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    int  countGroupBit = privateZNXSelCountGroupBitRecordNumber(recordNumber);//к-сть Group біт

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    if(recordNumber == 0)
    {
      int errorV = POST_UPR_REGISTER000FOR21(FILEPERIF_ZNXREGISTER2021);
      if(errorV == 1) continue;
      if(errorV != 0) return errorV;
    }//if

    else if(countGroupBit>0)
    {
      int errorV = POST_GROUPUPR_REGISTER000FOR21(FILEPERIF_ZNXREGISTER2021);
      if(errorV != 0) return errorV;
    }//if(countGroupBit>0)

    else if(privateZNXSelUstRecordNumber(recordNumber))
    {
      int errorV = POST_UST_REGISTER000FOR21(FILEPERIF_ZNXREGISTER2021);
      if(errorV == 1) continue;
    }//if
  }//for


  return 0;
}//postFileZNXRegister21
