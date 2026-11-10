
#include "header.h"
#include "variables_external_m.h"

//к-во ранжир на одном sdi
#define REGISTERS_SDI 8

int privateSDIvalidRecordLen(int recordNumber, int recordLen);
int privateSDISelUprRecordNumber(int recordNumber);
int privateSDISelRangRecordNumber(int recordNumber);
int getFileSDIRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileSDIRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileSDIRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

/**************************************/
//подготовка компонента SDI
/**************************************/

int privateSDIvalidRecordLen(int recordNumber, int recordLen)
{
 //валидация recordNumber
  if(recordNumber == 0) return MARKER_ERRORPERIMETR;
  if((recordNumber-1)/10 > NUMBER_LEDS) return MARKER_ERRORPERIMETR;
  if((recordNumber-1)%10 > 1) return MARKER_ERRORPERIMETR;
 //валидация recordLen
  if((recordNumber-1)%10 == 0)if(recordLen > 2) return MARKER_ERRORPERIMETR;
  if((recordNumber-1)%10 == 1)if(recordLen > REGISTERS_SDI+1) return MARKER_ERRORPERIMETR;
  return 0;
}//privateSDIvalidRecordLen

int privateSDISelUprRecordNumber(int recordNumber)
{
  if((recordNumber-1)%10 == 0) return 1;
  return 0;
}//privateSDISelUstRegisterNumber
int privateSDISelRangRecordNumber(int recordNumber)
{
  if((recordNumber-1)%10 == 1) return 1;
  return 0;
}//privateSDISelRangRegisterNumber

////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getFileSDIRegister20(int recordNumber, int registerNumber, int recordLen)
{
 //валидация recordLen recordNumber
 if(privateSDIvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;
 if(registerNumber == 0) return 0;

 if(privateSDISelUprRecordNumber(recordNumber))
 {
  //Тип СДI
   return GET_UPR_REGISTER000FOR20(FILEUVV_SDIREGISTER2021); 
 }//if
 else if(privateSDISelRangRecordNumber(recordNumber))
 {
  //Ранжирование СДI.
  int item = (recordNumber-1)/10;
  return getRangN_BIGModbusRegister2021(&current_settings_interfaces.ranguvannja_leds[item*N_BIG],
                                         registerNumber-1);
 }//if
 return MARKER_ERRORPERIMETR;
}//getFileSDIRegister20

////-SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET-
int setFileSDIRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
 //валидация recordLen recordNumber
 if(privateSDIvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;

 for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
 {
   if(registerNumber == 0) continue;
   uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

   if(privateSDISelUprRecordNumber(recordNumber))
   {
     //логические проверки
     if(value > 1) return MARKER_ERRORDIAPAZON;
   }//if
   else if(privateSDISelRangRecordNumber(recordNumber))
   {
    //Ранжирование1 СДI.
     if(validN_BIGACMD(value) != 0) return MARKER_ERRORDIAPAZON;
   }//if
   else return MARKER_ERRORPERIMETR;
 }//for
 return 0;
}//setFileSDIRegister21

////-POST--POST--POST--POST--POST--POST--POST--POST--POST--POST--POST-
int postFileSDIRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
  extern int upravlSchematic;//флаг Rang
 for(int registerNumber=0; registerNumber<recordLen; registerNumber++)//номера регистров
 {
   if(registerNumber == 0) continue;
   uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

   if(privateSDISelUprRecordNumber(recordNumber))
   {
    //Тип СДI.
     int errorV = POST_UPR_REGISTER000FOR21(FILEUVV_SDIREGISTER2021);

     if(errorV == 1) continue;
     if(errorV != 0) return errorV;
   }//if
 }//for

 if(privateSDISelRangRecordNumber(recordNumber))
 {
   int item = (recordNumber-1)/10;
   writeRangN_BIGModbusRegister2021(&edition_settings.ranguvannja_leds[item*N_BIG], &dataPacket[1], recordLen-1);
   //ВАЛИДАЦИЯ УСПЕШНА - УСТАНОВКА
   upravlSchematic = 1;//флаг Rang
 }//if

 return 0;
}//postFileSDIRegister21
