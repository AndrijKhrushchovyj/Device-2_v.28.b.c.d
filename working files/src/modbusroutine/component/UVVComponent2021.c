
#include "header.h"
#include "variables_external_m.h"

//начальный файл в карте памяти
#define BEGIN_FILE_REGISTER FILEUVV_BEGCOMPONENT2021
//конечный файл в карте памяти
#define END_FILE_REGISTER FILEUVV_ENDCOMPONENT2021

int privateUVVGetPerimetr(int fileReg);

int getUVVComponentRegister20(int fileNumber, int recordNumber, int registerNumber, int recordLen);
int setUVVComponentRegister21(int fileNumber, int recordNumber, int recordLen, unsigned short *dataPacket);
int postUVVComponentAction21(int fileNumber, int recordNumber, int recordLen, unsigned short *dataPacket);

int getFileDOUTRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileDOUTRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileDOUTRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

int getFileDVRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileDVRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileDVRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

int getFileSDIRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileSDIRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileSDIRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

int getFileKEYRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileKEYRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileKEYRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

int getFileRPRRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileRPRRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileRPRRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

//COMPONENT2021_OBJ *uvvcomponent2021;

//void constructorUVVComponent2021(COMPONENT2021_OBJ *uvvcomp)
//{
//  uvvcomponent2021 = uvvcomp;

//  uvvcomponent2021->getModbusRegister20func = getUVVComponentRegister20;//получить содержимое регистра
//  uvvcomponent2021->setModbusRegister21func = setUVVComponentRegister21;//записать содержимое регистра
//  uvvcomponent2021->postWriteAction21func = postUVVComponentAction21;//запись после пакета
//}//constructorUVVComponent2021

////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getUVVComponentRegister20(int fileNumber, int recordNumber, int registerNumber, int recordLen)
{
 //получить содержимое регистра
 if(privateUVVGetPerimetr(fileNumber) != 0) return MARKER_OUTPERIMETR;

 if(isActiveActualData2021)
 {
   edition_settings = current_settings_interfaces;//делаем копию
   isActiveActualData2021 = 0;//action до записи
 }//if(isActiveActualData)

 switch(fileNumber)
 {
  case FILEUVV_DOUTREGISTER2021:
    return getFileDOUTRegister20(recordNumber, registerNumber, recordLen); 
  case FILEUVV_DVREGISTER2021:
    return getFileDVRegister20(recordNumber, registerNumber, recordLen); 
  case FILEUVV_SDIREGISTER2021:
    return getFileSDIRegister20(recordNumber, registerNumber, recordLen); 
  case FILEUVV_KEYREGISTER2021:
    return getFileKEYRegister20(recordNumber, registerNumber, recordLen); 
  case FILEUVV_RPRREGISTER2021:
    return getFileRPRRegister20(recordNumber, registerNumber, recordLen); 
 }//switch
 return MARKER_OUTPERIMETR;
}//getUVVComponentRegister20

////-SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET-

int setUVVComponentRegister21(int fileNumber, int recordNumber, int recordLen, unsigned short *dataPacket)
{
 if(privateUVVGetPerimetr(fileNumber) != 0) return MARKER_OUTPERIMETR;

 if(isActiveActualData2021)
 {
   edition_settings = current_settings_interfaces;//делаем копию
   isActiveActualData2021 = 0;//action до записи
 }//if(isActiveActualData)

  switch(fileNumber)
  {
   case FILEUVV_DOUTREGISTER2021:
     return setFileDOUTRegister21(recordNumber, recordLen, dataPacket); 
   case FILEUVV_DVREGISTER2021:
     return setFileDVRegister21(recordNumber, recordLen, dataPacket); 
   case FILEUVV_SDIREGISTER2021:
     return setFileSDIRegister21(recordNumber, recordLen, dataPacket); 
   case FILEUVV_KEYREGISTER2021:
     return setFileKEYRegister21(recordNumber, recordLen, dataPacket); 
   case FILEUVV_RPRREGISTER2021:
     return setFileRPRRegister21(recordNumber, recordLen, dataPacket); 
  }//switch(fileNumber)

 return MARKER_ERRORPERIMETR;
}//setUVVComponentRegister21

////-POST--POST--POST--POST--POST--POST--POST--POST--POST--POST--POST-

int postUVVComponentAction21(int fileNumber, int recordNumber, int recordLen, unsigned short *dataPacket)
{
 if(privateUVVGetPerimetr(fileNumber) != 0) return MARKER_OUTPERIMETR;

 switch(fileNumber)
 {
   case FILEUVV_DOUTREGISTER2021:
     return postFileDOUTRegister21(recordNumber, recordLen, dataPacket); 
   case FILEUVV_DVREGISTER2021:
     return postFileDVRegister21(recordNumber, recordLen, dataPacket); 
   case FILEUVV_SDIREGISTER2021:
     return postFileSDIRegister21(recordNumber, recordLen, dataPacket); 
   case FILEUVV_KEYREGISTER2021:
     return postFileKEYRegister21(recordNumber, recordLen, dataPacket); 
   case FILEUVV_RPRREGISTER2021:
     return postFileRPRRegister21(recordNumber, recordLen, dataPacket); 
 }//switch(fileNumber)

 return MARKER_ERRORPERIMETR;
}//postUVVComponentAction21

int privateUVVGetPerimetr(int fileNumber)
{
  //проверить внешний периметр
 return controlPerimetr(fileNumber, BEGIN_FILE_REGISTER, END_FILE_REGISTER);
}//privateUVVGetPerimetr(int fileNumber)
