
#include "header.h"
#include "variables_external_m.h"

int privatePKV4validRecordLen(int recordNumber, int recordLen);
int privatePKV4SelUstRecordNumber(int recordNumber);
int getFilePKV4Register20(int recordNumber, int registerNumber, int recordLen);
int setFilePKV4Register21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFilePKV4Register21(int recordNumber, int recordLen, unsigned short *dataPacket);

int privatePKV4SelUstRecordNumber(int recordNumber)
{
  if((recordNumber == 2) || (recordNumber == 3)) return 1;
  return 0;
}//privatePKV4SelUstRecordNumber

int privatePKV4validRecordLen(int recordNumber, int recordLen)
{
//валидация recordLen
  if(recordNumber == 2)if(recordLen > 5) return MARKER_ERRORPERIMETR;
  if(recordNumber == 3)if(recordLen > 2) return MARKER_ERRORPERIMETR;
  return 0;
}//privatePKV4validRecordLen

////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getFilePKV4Register20(int recordNumber, int registerNumber, int recordLen)
{
//валидация recordLen
  if(privatePKV4validRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;
  if(registerNumber == 0) return 0;

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  if(recordNumber == 2)
    if(registerNumber == 4)
    {
//Количество стоп-бит
      return (edition_settings.number_stop_bit_RS485 + 1) & 0xFFFF;
    }//if
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if(privatePKV4SelUstRecordNumber(recordNumber))
    return GET_UST_REGISTER000FOR20(FILEPERIF_PKV4REGISTER2021);

  return MARKER_ERRORPERIMETR;
}//getFilePKV4Register20

////-SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET-
int setFilePKV4Register21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
//валидация recordLen
  if(privatePKV4validRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;

  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    if(privatePKV4SelUstRecordNumber(recordNumber))
    {
      if(SET_UST_REGISTER000FOR21(FILEPERIF_PKV4REGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
    }//if

    else return MARKER_ERRORPERIMETR;

  }//for
  return 0;
}//setFilePKV4Register21

////-POST--POST--POST--POST--POST--POST--POST--POST--POST--POST--POST-
int postFilePKV4Register21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
  extern int upravlSetting;
  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    if(privatePKV4SelUstRecordNumber(recordNumber))
    {
////ОСОБАЯ СБОРКА
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      if(recordNumber == 2)
        if(registerNumber == 4)
        {
//Количество стоп-бит
          edition_settings.number_stop_bit_RS485 = value - 1;
          upravlSetting = 1;//флаг Setting
          continue;
        }//if
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      int errorV = POST_UST_REGISTER000FOR21(FILEPERIF_PKV4REGISTER2021);
      if(errorV == 1) continue;
    }//if

  }//for

  return 0;
}//postFilePKV4Register21
