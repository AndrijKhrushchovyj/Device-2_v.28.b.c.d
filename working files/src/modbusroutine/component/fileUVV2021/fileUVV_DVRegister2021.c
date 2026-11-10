
#include "header.h"
#include "variables_external_m.h"

//к-во ранжир на одном dv
#define REGISTERS_DV 8

int privateDVvalidRecordLen(int recordNumber, int recordLen);
int privateDVSelRangRecordNumber(int recordNumber);
int privateDVSelUstRecordNumber(int recordNumber, int registerNumber);
int privateDVSelUprRecordNumber(int recordNumber, int registerNumber);
int getFileDVRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileDVRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileDVRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

/**************************************/
//подготовка компонента DV
/**************************************/

int privateDVvalidRecordLen(int recordNumber, int recordLen)
{
 //валидация recordNumber
  if(recordNumber == 0) return MARKER_ERRORPERIMETR;
  if((recordNumber-1)/10 > NUMBER_INPUTS) return MARKER_ERRORPERIMETR;
  if((recordNumber-1)%10 > 1) return MARKER_ERRORPERIMETR;
 //валидация recordLen
  if((recordNumber-1)%10 == 0)if(recordLen > 4) return MARKER_ERRORPERIMETR;
  if((recordNumber-1)%10 == 1)if(recordLen > REGISTERS_DV+1) return MARKER_ERRORPERIMETR;
  return 0;
}//privateDVvalidRecordLen

int privateDVSelUprRecordNumber(int recordNumber, int registerNumber)
{
  if((recordNumber-1)%10 == 0) if(registerNumber < 3)return 1;
  return 0;
}//privateDVSelUprRegisterNumber
int privateDVSelUstRecordNumber(int recordNumber, int registerNumber)
{
  if((recordNumber-1)%10 == 0) if(registerNumber == 3) return 1;
  return 0;
}//privateDVSelUstRegisterNumber
int privateDVSelRangRecordNumber(int recordNumber)
{
  if((recordNumber-1)%10 == 1) return 1;
  return 0;
}//privateDVSelRangRegisterNumber

////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getFileDVRegister20(int recordNumber, int registerNumber, int recordLen)
{
 //валидация recordLen recordNumber
 if(privateDVvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;
 if(registerNumber == 0) return 0;

 if(privateDVSelUprRecordNumber(recordNumber, registerNumber))
 {
  //Тип ДВх. Вид ДВх
   return GET_UPR_REGISTER000FOR20(FILEUVV_DVREGISTER2021); 
 }//if
 else if(privateDVSelUstRecordNumber(recordNumber, registerNumber))
 {
  //Допуск ДВх
   return GET_UST_REGISTER000FOR20(FILEUVV_DVREGISTER2021); 
 }//if
 else if(privateDVSelRangRecordNumber(recordNumber))
 {
  //Ранжирование ДВх.
  int item = (recordNumber-1)/10;
  return getRangN_SMALLModbusRegister2021(&current_settings_interfaces.ranguvannja_inputs[item*N_SMALL],
                                         registerNumber-1);
 }//if

 return MARKER_ERRORPERIMETR;
}//getFileDVRegister20

////-SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET-
int setFileDVRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
 //валидация recordLen recordNumber
 if(privateDVvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;

 for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
 {
   if(registerNumber == 0) continue;
  
   uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);
   if(privateDVSelUprRecordNumber(recordNumber, registerNumber))
   {
     //логические проверки
     if(value > 1) return MARKER_ERRORDIAPAZON;
   }//if
   else if(privateDVSelRangRecordNumber(recordNumber))
   {
     //Ранжирование1 ДВх.
     if(validN_SMALLACMD(value) != 0) return MARKER_ERRORDIAPAZON;
   }//if
   else if(privateDVSelUstRecordNumber(recordNumber, registerNumber))
   {
     return 0;
   }//if
   else return MARKER_ERRORPERIMETR;
 }//for
 return 0;
}//setFileDVRegister21

////-POST--POST--POST--POST--POST--POST--POST--POST--POST--POST--POST-
int postFileDVRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
  extern int upravlSetting;//флаг Setting
  extern int upravlSchematic;//флаг Rang
 for(int registerNumber=0; registerNumber<recordLen; registerNumber++)//номера регистров
 {
   if(registerNumber == 0) continue;
   uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

   if(privateDVSelUprRecordNumber(recordNumber, registerNumber))
   {
    //Тип ДВх.01 
     int errorV = POST_UPR_REGISTER000FOR21(FILEUVV_DVREGISTER2021);

     if(errorV == 1) continue;
     if(errorV != 0) return errorV;
   }//if
   else if(privateDVSelUstRecordNumber(recordNumber, registerNumber))
   {
    //Допуск ДВх
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
////ОСОБАЯ ПРОВЕРКА
    int item = (recordNumber-1)/10;
    if(value > KOEF_DOPUSK_DV_MAX) return ERROR_VALID2;
    if(!((edition_settings.type_of_input_signal)&(1<<item)))
    {
      //только для постоянки
//      if((value%2)!=0) return ERROR_VALID2;//всегда четные
    }//if
    else
    {
      //только для переменки
      if(value < KOEF_DOPUSK_DV_ZMIN_MIN) return ERROR_VALID2;//не меньше 10 мс
      if((value%10) != 0) return ERROR_VALID2;//кратні 10
    }
    upravlSetting = 1;//флаг Setting
    edition_settings.dopusk_dv[item] = value;
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

   }//if
 }//for

 if(privateDVSelRangRecordNumber(recordNumber))
 {
   int item = (recordNumber-1)/10;
   writeRangN_SMALLModbusRegister2021(&edition_settings.ranguvannja_inputs[item*N_SMALL], &dataPacket[1], recordLen-1);
   //ВАЛИДАЦИЯ УСПЕШНА - УСТАНОВКА
   upravlSchematic = 1;//флаг Rang
 }//if

 return 0;
}//postFileDVRegister21
