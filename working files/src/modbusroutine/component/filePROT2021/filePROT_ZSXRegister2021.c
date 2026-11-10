
#include "header.h"
#include "variables_external_m.h"

int privateZSXvalidRecordLen(int recordNumber, int recordLen);
int privateZSXSelCountGroupBitRecordNumber(int recordNumber);
int getFileZSXRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileZSXRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileZSXRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

int privateZSXvalidRecordLen(int recordNumber, int recordLen)
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
}//privateZSXvalidRecordLen

int privateZSXSelCountGroupBitRecordNumber(int recordNumber)
{
  if(recordNumber == 1)
    return 1;//к-сть Group біт
  return -1;
}//privateZSXSelCountGroupBitRecordNumber
////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getFileZSXRegister20(int recordNumber, int registerNumber, int recordLen)
{
//валидация recordLen
  if(privateZSXvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;
  if(registerNumber == 0) return 0;

  int  countGroupBit = privateZSXSelCountGroupBitRecordNumber(recordNumber);//к-сть Group біт

  if(recordNumber == 0) return GET_UPR_REGISTER000FOR20(FILEPERIF_ZSXREGISTER2021);

  else if(countGroupBit > 0)
  {
    return GET_GROUPUPR_REGISTER000FOR20(FILEPERIF_ZSXREGISTER2021);
  }//if(countGroupBit>0)

  return MARKER_ERRORPERIMETR;
}//getFileZSXRegister20

////-SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET-
int setFileZSXRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
//валидация recordLen
  if(privateZSXvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;

  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);
    int  countGroupBit = privateZSXSelCountGroupBitRecordNumber(recordNumber);//к-сть Group біт

    if(recordNumber == 0)
    {
      if(SET_UPR_REGISTER000FOR21(FILEPERIF_ZSXREGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
    }//if

    else if(countGroupBit > 0)
    {
      if(SET_GROUPUPR_REGISTER000FOR21(FILEPERIF_ZSXREGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
    }//if(countGroupBit>0)

    else return MARKER_ERRORPERIMETR;

  }//for
  return 0;
}//setFileZSXRegister21

////-POST--POST--POST--POST--POST--POST--POST--POST--POST--POST--POST-
int postFileZSXRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    int  countGroupBit = privateZSXSelCountGroupBitRecordNumber(recordNumber);//к-сть Group біт

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    if(recordNumber == 0)
    {
      int errorV = POST_UPR_REGISTER000FOR21(FILEPERIF_ZSXREGISTER2021);
      if(errorV == 1) continue;
      if(errorV != 0) return errorV;
    }//if

    else if(countGroupBit>0)
    {
      int errorV = POST_GROUPUPR_REGISTER000FOR21(FILEPERIF_ZSXREGISTER2021);
      if(errorV != 0) return errorV;
    }//if(countGroupBit>0)
  }//for


  return 0;
}//postFileZSXRegister21
