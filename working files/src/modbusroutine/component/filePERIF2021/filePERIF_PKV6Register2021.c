
#include "header.h"
#include "variables_external_m.h"

int privatePKV6validRecordLen(int recordNumber, int recordLen);
int privatePKV6SelUstRecordNumber(int recordNumber);
int getFilePKV6Register20(int recordNumber, int registerNumber, int recordLen);
int setFilePKV6Register21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFilePKV6Register21(int recordNumber, int recordLen, unsigned short *dataPacket);

int privatePKV6SelUstRecordNumber(int recordNumber)
{
  if(recordNumber == 2) return 1;
  return 0;
}//privatePKV6SelUstRecordNumber

int privatePKV6validRecordLen(int recordNumber, int recordLen)
{
  UNUSED(recordNumber);
//валидация recordLen
  if(recordLen > 2) return MARKER_ERRORPERIMETR;
  return 0;
}//privatePKV6validRecordLen

////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getFilePKV6Register20(int recordNumber, int registerNumber, int recordLen)
{
//валидация recordLen
  if(privatePKV6validRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;
  if(registerNumber == 0) return 0;

  if(privatePKV6SelUstRecordNumber(recordNumber))
    return GET_UST_REGISTER000FOR20(FILEPERIF_PKV6REGISTER2021);

  return MARKER_ERRORPERIMETR;
}//getFilePKV6Register20

////-SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET-
int setFilePKV6Register21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
//валидация recordLen
  if(privatePKV6validRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;

  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    if(privatePKV6SelUstRecordNumber(recordNumber))
    {
      if(SET_UST_REGISTER000FOR21(FILEPERIF_PKV6REGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
    }//if

    else return MARKER_ERRORPERIMETR;

  }//for
  return 0;
}//setFilePKV6Register21

////-POST--POST--POST--POST--POST--POST--POST--POST--POST--POST--POST-
int postFilePKV6Register21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
  extern int upravlSetting;
  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    if(privatePKV6SelUstRecordNumber(recordNumber))
    {
      int errorV = POST_UST_REGISTER000FOR21(FILEPERIF_PKV6REGISTER2021);
      if(errorV == 1) continue;
    }//if

  }//for

  return 0;
}//postFilePKV6Register21
