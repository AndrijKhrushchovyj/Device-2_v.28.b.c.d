
#include "header.h"
#include "variables_external_m.h"

int privateZNMINvalidRecordLen(int recordNumber, int recordLen);
int privateZNMINSelUstRecordNumber(int recordNumber);
int privateZNMINSelCountGroupBitRecordNumber(int recordNumber);
int getFileZNMINRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileZNMINRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileZNMINRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

int privateZNMINSelUstRecordNumber(int recordNumber)
{
  switch(recordNumber)
  {
   case 5:
   case 6:
   case 105:
   case 106:
    return 1;
  }//switch
  return 0;
}//privateZNMINvalidRecordNumber

int privateZNMINvalidRecordLen(int recordNumber, int recordLen)
{
//валидация recordLen
  switch(recordNumber)
  {
  case 0:
  case 1:
  case 101:
    if(recordLen>2) return MARKER_ERRORPERIMETR;
    break;
  default:
    if(recordLen>5) return MARKER_ERRORPERIMETR;
  }//switch
  return 0;
}//privateZNMINvalidRecordLen

int privateZNMINSelCountGroupBitRecordNumber(int recordNumber)
{
  if((recordNumber == 1) || (recordNumber == 101))
    return 1;//к-сть Group біт
  return -1;
}//privateZNMINSelCountGroupBitRecordNumber
////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getFileZNMINRegister20(int recordNumber, int registerNumber, int recordLen)
{
//валидация recordLen
  if(privateZNMINvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;
  if(registerNumber == 0) return 0;

  int  countGroupBit = privateZNMINSelCountGroupBitRecordNumber(recordNumber);//к-сть Group біт

  if(recordNumber == 0) return GET_UPR_REGISTER000FOR20(FILEPROT_ZNMINREGISTER2021);

  else if(countGroupBit > 0)
  {
    return GET_GROUPUPR_REGISTER000FOR20(FILEPROT_ZNMINREGISTER2021);
  }//if(countGroupBit>0)
  else if(privateZNMINSelUstRecordNumber(recordNumber))
  {
    return GET_UST_REGISTER000FOR20(FILEPROT_ZNMINREGISTER2021);
  }

  return MARKER_ERRORPERIMETR;
}//getFileZNMINRegister20

////-SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET-
int setFileZNMINRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
//валидация recordLen
  if(privateZNMINvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;

  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);
    int  countGroupBit = privateZNMINSelCountGroupBitRecordNumber(recordNumber);//к-сть Group біт

    if(recordNumber == 0)
    {
      if(SET_UPR_REGISTER000FOR21(FILEPROT_ZNMINREGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
    }//if

    else if(countGroupBit > 0)
    {
      if(SET_GROUPUPR_REGISTER000FOR21(FILEPROT_ZNMINREGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
    }//if(countGroupBit>0)

    else if(privateZNMINSelUstRecordNumber(recordNumber))
    {
      if(SET_UST_REGISTER000FOR21(FILEPROT_ZNMINREGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
    }//if
    else return MARKER_ERRORPERIMETR;

  }//for
  return 0;
}//setFileZNMINRegister21

////-POST--POST--POST--POST--POST--POST--POST--POST--POST--POST--POST-
int postFileZNMINRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    int  countGroupBit = privateZNMINSelCountGroupBitRecordNumber(recordNumber);//к-сть Group біт

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    if(recordNumber == 0)
    {
      int errorV = POST_UPR_REGISTER000FOR21(FILEPROT_ZNMINREGISTER2021);
      if(errorV == 1) continue;
      if(errorV != 0) return errorV;
    }//if

    else if(countGroupBit>0)
    {
      int errorV = POST_GROUPUPR_REGISTER000FOR21(FILEPROT_ZNMINREGISTER2021);
      if(errorV != 0) return errorV;
    }//if(countGroupBit>0)

    else if(privateZNMINSelUstRecordNumber(recordNumber))
    {
      int errorV = POST_UST_REGISTER000FOR21(FILEPROT_ZNMINREGISTER2021);
      if(errorV == 1) continue;
    }//if
  }//for

  return 0;
}//postFileZNMINRegister21
