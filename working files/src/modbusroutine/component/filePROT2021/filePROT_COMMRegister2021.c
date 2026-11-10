
#include "header.h"
#include "variables_external_m.h"

int privateCOMMvalidRecordLen(int recordNumber, int recordLen);
int getFileCOMMRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileCOMMRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileCOMMRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int privateCOMMSelUstRecordNumber(int recordNumber);

int privateCOMMvalidRecordLen(int recordNumber, int recordLen)
{
//валидация recordLen
  UNUSED(recordNumber);
  if(recordLen > 2) return MARKER_ERRORPERIMETR;
  return 0;
}//privateCOMMvalidRecordLen

int privateCOMMSelUstRecordNumber(int recordNumber)
{
  if(recordNumber == 1) return 1;
  return 0;
}//privateJAPVvalidRecordNumber

////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getFileCOMMRegister20(int recordNumber, int registerNumber, int recordLen)
{
//валидация recordLen
  if(privateCOMMvalidRecordLen(recordNumber, recordLen) == MARKER_ERRORPERIMETR) return MARKER_ERRORPERIMETR;
  if(registerNumber == 0) return 0;

  if(privateCOMMSelUstRecordNumber(recordNumber))
  {
    return GET_UST_REGISTER000FOR20(FILEPROT_COMMREGISTER2021);
  }//if

  return MARKER_ERRORPERIMETR;
}//getFileCOMMRegister20

////-SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET-
int setFileCOMMRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
//валидация recordLen
  if(privateCOMMvalidRecordLen(recordNumber, recordLen) == MARKER_ERRORPERIMETR) return MARKER_ERRORPERIMETR;

  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;
    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    if(privateCOMMSelUstRecordNumber(recordNumber))
    {
      if(SET_UST_REGISTER000FOR21(FILEPROT_COMMREGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
    }//if
    else return MARKER_ERRORPERIMETR;

  }//for
  return 0;
}//setFileCOMMRegister21

////-POST--POST--POST--POST--POST--POST--POST--POST--POST--POST--POST-
int postFileCOMMRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;
    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    if(privateCOMMSelUstRecordNumber(recordNumber))
    {
      int errorV = POST_UST_REGISTER000FOR21(FILEPROT_COMMREGISTER2021);
      if(errorV == 1) continue;
    }//if
  }//for

  return 0;
}//postFileCOMMRegister21
