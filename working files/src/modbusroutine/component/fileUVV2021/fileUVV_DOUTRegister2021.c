
#include "header.h"
#include "variables_external_m.h"

//к-во ранжир на одном реле
#define REGISTERS_OUTPUTS 16

int privateDOUTvalidRecordLen(int recordNumber, int recordLen);
int privateDOUTSelUprRecordNumber(int recordNumber);
int privateDOUTSelRangRecordNumber(int recordNumber);
int getFileDOUTRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileDOUTRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileDOUTRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

/**************************************/
//подготовка компонента RELE
/**************************************/

int privateDOUTvalidRecordLen(int recordNumber, int recordLen)
{
 //валидация recordNumber
  if(recordNumber == 0) return MARKER_ERRORPERIMETR;
  if((recordNumber-1)/10 > NUMBER_OUTPUTS) return MARKER_ERRORPERIMETR;
  if((recordNumber-1)%10 > 1) return MARKER_ERRORPERIMETR;
 //валидация recordLen
  if((recordNumber-1)%10 == 0)if(recordLen > 3) return MARKER_ERRORPERIMETR;
  if((recordNumber-1)%10 == 1)if(recordLen > REGISTERS_OUTPUTS+1) return MARKER_ERRORPERIMETR;
  return 0;
}//privateDOUTvalidRecordLen

int privateDOUTSelUprRecordNumber(int recordNumber)
{
  if((recordNumber-1)%10 == 0) return 1;
  return 0;
}//privateDOUTSelUstRegisterNumber
int privateDOUTSelRangRecordNumber(int recordNumber)
{
  if((recordNumber-1)%10 == 1) return 1;
  return 0;
}//privateDOUTSelRangRegisterNumber

////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getFileDOUTRegister20(int recordNumber, int registerNumber, int recordLen)
{
 //валидация recordLen recordNumber
 if(privateDOUTvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;
 if(registerNumber == 0) return 0;

 if(privateDOUTSelUprRecordNumber(recordNumber))
 {
  //Тип ДВых. Тип ДВых. (модиф.)
   return GET_UPR_REGISTER000FOR20(FILEUVV_DOUTREGISTER2021); 
 }//if
 else if(privateDOUTSelRangRecordNumber(recordNumber))
 {
  //Ранжирование ДВых.
  int item = (recordNumber-1)/10;
  return getRangN_BIGModbusRegister2021(&current_settings_interfaces.ranguvannja_outputs[item*N_BIG],
                                         registerNumber-1);
 }//if

 return MARKER_ERRORPERIMETR;
}//getFileDOUTRegister20

////-SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET-
int setFileDOUTRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
 //валидация recordLen recordNumber
 if(privateDOUTvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;

 for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
 {
   if(registerNumber == 0) continue;
   uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);
   if(privateDOUTSelUprRecordNumber(recordNumber))
   {
   //логические проверки
   if(value > 1) return MARKER_ERRORDIAPAZON;
   }//if
   else if(privateDOUTSelRangRecordNumber(recordNumber))
   {
    //Ранжирование1 ДВых.
     if(validN_BIGACMD(value) != 0) return MARKER_ERRORDIAPAZON;
   }
   else return MARKER_ERRORPERIMETR;
 }//for
 return 0;
}//setFileDOUTRegister21

////-POST--POST--POST--POST--POST--POST--POST--POST--POST--POST--POST-
int postFileDOUTRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
  extern int upravlSchematic;//флаг Rang

 if(privateDOUTSelUprRecordNumber(recordNumber))
 {
   for(int registerNumber=0; registerNumber<recordLen; registerNumber++)//номера регистров
   {
     if(registerNumber == 0) continue;
     uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);
  
     //Тип ДВых.01 Тип ДВых.01 (модиф.)
     int errorV = POST_UPR_REGISTER000FOR21(FILEUVV_DOUTREGISTER2021);
  
     if(errorV == 1) continue;
     if(errorV != 0) return errorV;
   }//for
 }//if
 else if(privateDOUTSelRangRecordNumber(recordNumber))
 {
   int item = (recordNumber-1)/10;
   writeRangN_BIGModbusRegister2021(&edition_settings.ranguvannja_outputs[item*N_BIG], &dataPacket[1], recordLen-1);
   //ВАЛИДАЦИЯ УСПЕШНА - УСТАНОВКА
   upravlSchematic = 1;//флаг Rang
 }//if
 else return MARKER_ERRORPERIMETR;

 return 0;
}//postFileDOUTRegister21
