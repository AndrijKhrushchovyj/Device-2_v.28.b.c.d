
#include "header.h"
#include "variables_external_m.h"

//начальный файл в карте памяти
#define BEGIN_FILE_REGISTER FILEPROT_BEGCOMPONENT2021
//конечный файл в карте памяти
#define END_FILE_REGISTER FILEPROT_ENDCOMPONENT2021

int privatePROTGetPerimetr(int fileReg);

//COMPONENT2021_OBJ *protcomponent2021;

int getPROTComponentRegister20(int fileNumber, int recordNumber, int registerNumber, int recordLen);
int setPROTComponentRegister21(int fileNumber, int recordNumber, int recordLen, unsigned short *dataPacket);
int postPROTComponentAction21(int fileNumber, int recordNumber, int recordLen, unsigned short *dataPacket);

int getFileCOMMRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileCOMMRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileCOMMRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

int getFileZNMINRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileZNMINRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileZNMINRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

int getFileZNMAXRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileZNMAXRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileZNMAXRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

int getFileRPNRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileRPNRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileRPNRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

int getFileZSXRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileZSXNRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileZSXRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

int getFileZNXRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileZNXNRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileZNXRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

int getFileBRPRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileBRPRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileBRPRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

int getFileUPRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileUPRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileUPRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

//void constructorPROTComponent2021(COMPONENT2021_OBJ *protcomp)
//{
//  protcomponent2021 = protcomp;

//  protcomponent2021->getModbusRegister20func = getPROTComponentRegister20;//получить содержимое регистра
//  protcomponent2021->setModbusRegister21func = setPROTComponentRegister21;//записать содержимое регистра
//  protcomponent2021->postWriteAction21func = postPROTComponentAction21;//запись после пакета
//}//constructorPROTComponent2021

////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getPROTComponentRegister20(int fileNumber, int recordNumber, int registerNumber, int recordLen)
{
 //получить содержимое регистра
 if(privatePROTGetPerimetr(fileNumber) != 0) return MARKER_OUTPERIMETR;

 if(isActiveActualData2021)
 {
   edition_settings = current_settings_interfaces;//делаем копию
   isActiveActualData2021 = 0;//action до записи
 }//if(isActiveActualData)

 switch(fileNumber)
 {
  case FILEPROT_COMMREGISTER2021:
    return getFileCOMMRegister20(recordNumber, registerNumber, recordLen); 
   case FILEPROT_ZNMINREGISTER2021:
     return getFileZNMINRegister20(recordNumber, registerNumber, recordLen); 
   case FILEPROT_ZNMAXREGISTER2021:
     return getFileZNMAXRegister20(recordNumber, registerNumber, recordLen); 
   case FILEPROT_UPREGISTER2021:
     return getFileUPRegister20(recordNumber, registerNumber, recordLen); 
 }//switch
 return MARKER_OUTPERIMETR;
}//getPROTComponentRegister20

////-SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET-

int setPROTComponentRegister21(int fileNumber, int recordNumber, int recordLen, unsigned short *dataPacket)
{
 if(privatePROTGetPerimetr(fileNumber) != 0) return MARKER_OUTPERIMETR;

 if(isActiveActualData2021)
 {
   edition_settings = current_settings_interfaces;//делаем копию
   isActiveActualData2021 = 0;//action до записи
 }//if(isActiveActualData)

  switch(fileNumber)
  {
   case FILEPROT_COMMREGISTER2021:
     return setFileCOMMRegister21(recordNumber, recordLen, dataPacket); 
   case FILEPROT_UPREGISTER2021:
     return setFileUPRegister21(recordNumber, recordLen, dataPacket); 
   case FILEPROT_ZNMINREGISTER2021:
     return setFileZNMINRegister21(recordNumber, recordLen, dataPacket); 
   case FILEPROT_ZNMAXREGISTER2021:
     return setFileZNMAXRegister21(recordNumber, recordLen, dataPacket); 
  }//switch(fileNumber)

 return MARKER_ERRORPERIMETR;
}//setPROTComponentRegister21

////-POST--POST--POST--POST--POST--POST--POST--POST--POST--POST--POST-

int postPROTComponentAction21(int fileNumber, int recordNumber, int recordLen, unsigned short *dataPacket)
{
 if(privatePROTGetPerimetr(fileNumber) != 0) return MARKER_OUTPERIMETR;

 switch(fileNumber)
 {
   case FILEPROT_COMMREGISTER2021:
     return postFileCOMMRegister21(recordNumber, recordLen, dataPacket); 
   case FILEPROT_UPREGISTER2021:
     return postFileUPRegister21(recordNumber, recordLen, dataPacket); 
   case FILEPROT_ZNMINREGISTER2021:
     return postFileZNMINRegister21(recordNumber, recordLen, dataPacket); 
   case FILEPROT_ZNMAXREGISTER2021:
     return postFileZNMAXRegister21(recordNumber, recordLen, dataPacket); 
 }//switch(fileNumber)

 return MARKER_ERRORPERIMETR;
}//postPROTComponentAction21

int privatePROTGetPerimetr(int fileNumber)
{
  //проверить внешний периметр
 return controlPerimetr(fileNumber, BEGIN_FILE_REGISTER, END_FILE_REGISTER);
}//privatePROTGetPerimetr(int fileNumber)
