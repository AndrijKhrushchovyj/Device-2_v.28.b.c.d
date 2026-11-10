
#include "header.h"
#include "variables_external_m.h"

int privateTRANSvalidRecordLen(int recordNumber, int recordLen);
int privateTRANSSelUstRecordNumber(int recordNumber);
int getFileTRANSRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileTRANSRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileTRANSRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

int privateTRANSSelUstRecordNumber(int recordNumber)
{
  switch(recordNumber)
  {
  case 1:
  case 2:
  case 4:
  case 5:
    return 1;
  }//switch
  return 0;
}//privateTRANSvalidRecordNumber

int privateTRANSvalidRecordLen(int recordNumber, int recordLen)
{
  UNUSED(recordNumber);
//валидация recordLen
  if(recordLen > 2) return MARKER_ERRORPERIMETR;
  return 0;
}//privateTRANSvalidRecordLen

////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getFileTRANSRegister20(int recordNumber, int registerNumber, int recordLen)
{
//валидация recordLen
  if(privateTRANSvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;
  if(registerNumber == 0) return 0;

  if(privateTRANSSelUstRecordNumber(recordNumber))
    return GET_UST_REGISTER000FOR20(FILEPERIF_TRANSREGISTER2021);

  return MARKER_ERRORPERIMETR;
}//getFileTRANSRegister20

////-SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET-
int setFileTRANSRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
//валидация recordLen
  if(privateTRANSvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;

  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    if(privateTRANSSelUstRecordNumber(recordNumber))
    {
      if(SET_UST_REGISTER000FOR21(FILEPERIF_TRANSREGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
    }//if

    else return MARKER_ERRORPERIMETR;

  }//for
  return 0;
}//setFileTRANSRegister21

////-POST--POST--POST--POST--POST--POST--POST--POST--POST--POST--POST-
int postFileTRANSRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    if(privateTRANSSelUstRecordNumber(recordNumber))
    {
      int errorV = POST_UST_REGISTER000FOR21(FILEPERIF_TRANSREGISTER2021);
      if(errorV == 1) continue;
    }//if

  }//for

  return 0;
}//postFileTRANSRegister21
