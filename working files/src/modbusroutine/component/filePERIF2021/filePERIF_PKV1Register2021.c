
#include "header.h"
#include "variables_external_m.h"

int privatePKV1validRecordLen(int recordNumber, int recordLen);
int privatePKV1SelUstRecordNumber(int recordNumber);
int getFilePKV1Register20(int recordNumber, int registerNumber, int recordLen);
int setFilePKV1Register21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFilePKV1Register21(int recordNumber, int recordLen, unsigned short *dataPacket);

int privatePKV1SelUstRecordNumber(int recordNumber)
{
  if((recordNumber == 1) || (recordNumber == 4)) return 1;
  return 0;
}//privatePKV1validRecordNumber

int privatePKV1validRecordLen(int recordNumber, int recordLen)
{
//валидация recordLen
  if(recordNumber == 1)
  {
    if(recordLen > 2) return MARKER_ERRORPERIMETR;
  }//if
  else if(recordLen > 17) return MARKER_ERRORPERIMETR;
  return 0;
}//privatePKV1validRecordLen

////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getFilePKV1Register20(int recordNumber, int registerNumber, int recordLen)
{
//валидация recordLen
  if(privatePKV1validRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;
  if(registerNumber == 0) return 0;

  if(privatePKV1SelUstRecordNumber(recordNumber))
  {
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if(recordNumber == 4)//Символ
    {
     int idx = registerNumber - 1;
     return (edition_settings.name_of_cell[idx*2]&0xFF) | ((edition_settings.name_of_cell[idx*2 +1]<<8)&0xFF00);
    }//if(recordNumber == 4)//Символ
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    return GET_UST_REGISTER000FOR20(FILEPERIF_PKV1REGISTER2021);
  }//

  return MARKER_ERRORPERIMETR;
}//getFilePKV1Register20

////-SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET-
int setFilePKV1Register21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
//валидация recordLen
  if(privatePKV1validRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;

  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    if(privatePKV1SelUstRecordNumber(recordNumber))
    {
      if(SET_UST_REGISTER000FOR21(FILEPERIF_PKV1REGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
    }//if

    else return MARKER_ERRORPERIMETR;

  }//for
  return 0;
}//setFilePKV1Register21

////-POST--POST--POST--POST--POST--POST--POST--POST--POST--POST--POST-
int postFilePKV1Register21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
  extern int upravlSetting;
  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    if(privatePKV1SelUstRecordNumber(recordNumber))
    {
////ОСОБАЯ СБОРКА
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      if(recordNumber == 4)//Символ
      {
       int idx = registerNumber - 1;
       edition_settings.name_of_cell[idx*2] = value;
       edition_settings.name_of_cell[idx*2 +1] = value>>8;
       upravlSetting = 1;
       continue;
      }//if(recordNumber == 4)//Символ
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

      int errorV = POST_UST_REGISTER000FOR21(FILEPERIF_PKV1REGISTER2021);
      if(errorV == 1) continue;
    }//if

  }//for

  return 0;
}//postFilePKV1Register21
