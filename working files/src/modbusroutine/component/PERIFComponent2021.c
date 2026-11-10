
#include "header.h"
#include "variables_external_m.h"

//начальный файл в карте памяти
#define BEGIN_FILE_REGISTER FILEPERIF_BEGCOMPONENT2021
//конечный файл в карте памяти
#define END_FILE_REGISTER FILEPERIF_ENDCOMPONENT2021

int privatePERIFGetPerimetr(int fileReg);

int getFileRAIRegister20(int recordNumber, int registerNumber, int recordLen);

int getPERIFComponentRegister20(int fileNumber, int recordNumber, int registerNumber, int recordLen);
int setPERIFComponentRegister21(int fileNumber, int recordNumber, int recordLen, unsigned short *dataPacket);
int postPERIFComponentAction21(int fileNumber, int recordNumber, int recordLen, unsigned short *dataPacket);

int getFileTRANSRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileTRANSRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileTRANSRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

int getFileELMFTRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileELMFTRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileELMFTRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

int getFileELDTRRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileELDTRRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileELDTRRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

int getFileELANDRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileELANDRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileELANDRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

int getFileELORRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileELORRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileELORRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

int getFileELXORRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileELXORRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileELXORRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

int getFileELNOTRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileELNOTRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileELNOTRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

int getFileIECGOOSERegister20(int recordNumber, int registerNumber, int recordLen);
int setFileIECGOOSERegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileIECGOOSERegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

int getFileIECMMSRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileIECMMSRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileIECMMSRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

int getFileIECLANRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileIECLANRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileIECLANRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

int getFileELPFRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileELPFRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileELPFRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

int getFilePKV1Register20(int recordNumber, int registerNumber, int recordLen);
int setFilePKV1Register21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFilePKV1Register21(int recordNumber, int recordLen, unsigned short *dataPacket);

int getFilePKV2Register20(int recordNumber, int registerNumber, int recordLen);
int setFilePKV2Register21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFilePKV2Register21(int recordNumber, int recordLen, unsigned short *dataPacket);

int getFilePKV3Register20(int recordNumber, int registerNumber, int recordLen);
int setFilePKV3Register21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFilePKV3Register21(int recordNumber, int recordLen, unsigned short *dataPacket);

int getFilePKV4Register20(int recordNumber, int registerNumber, int recordLen);
int setFilePKV4Register21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFilePKV4Register21(int recordNumber, int recordLen, unsigned short *dataPacket);

int getFilePKV5Register20(int recordNumber, int registerNumber, int recordLen);
int setFilePKV5Register21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFilePKV5Register21(int recordNumber, int recordLen, unsigned short *dataPacket);

int getFilePKV6Register20(int recordNumber, int registerNumber, int recordLen);
int setFilePKV6Register21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFilePKV6Register21(int recordNumber, int recordLen, unsigned short *dataPacket);

int getFileREGANLRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileREGANLRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileREGANLRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

int getFileANLCONF1Register20(int recordNumber, int registerNumber, int recordLen);
int getFileANLDATA1Register20(int recordNumber, int registerNumber, int recordLen);
int getFileANLDATA2Register20(int recordNumber, int registerNumber, int recordLen);
int getFileANLDATA3Register20(int recordNumber, int registerNumber, int recordLen);
int getFileANLDATA4Register20(int recordNumber, int registerNumber, int recordLen);
int getFileANLDATA5Register20(int recordNumber, int registerNumber, int recordLen);
int getFileANLDATA6Register20(int recordNumber, int registerNumber, int recordLen);
int getFileANLDATA7Register20(int recordNumber, int registerNumber, int recordLen);
int getFileANLDATA8Register20(int recordNumber, int registerNumber, int recordLen);

int getFileREGDISRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileREGDISRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileREGDISRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

int getFileDRCONF1Register20(int recordNumber, int registerNumber, int recordLen);
int getFileDRDATA1Register20(int recordNumber, int registerNumber, int recordLen);
int getFileDRDATA2Register20(int recordNumber, int registerNumber, int recordLen);
int getFileDSREGRegister20(int recordNumber, int registerNumber, int recordLen);

int getFileRPNRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileRPNRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileRPNRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

int getFileZSXRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileZSXRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileZSXRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

int getFileZNXRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileZNXRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileZNXRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

int getFileBRPRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileBRPRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileBRPRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

//int getFileRAIRegister20(int recordNumber, int registerNumber, int recordLen);

//COMPONENT2021_OBJ *perifcomponent2021;

//void constructorPERIFComponent2021(COMPONENT2021_OBJ *perifcomp)
//{
//  perifcomponent2021 = perifcomp;

//  perifcomponent2021->getModbusRegister20func = getPERIFComponentRegister20;//получить содержимое регистра
//  perifcomponent2021->setModbusRegister21func = setPERIFComponentRegister21;//записать содержимое регистра
//  perifcomponent2021->postWriteAction21func = postPERIFComponentAction21;//запись после пакета
//}//constructorPERIFComponent2021

////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getPERIFComponentRegister20(int fileNumber, int recordNumber, int registerNumber, int recordLen)
{
 //получить содержимое регистра
 if(privatePERIFGetPerimetr(fileNumber) != 0) return MARKER_OUTPERIMETR;

 if(isActiveActualData2021)
 {
   edition_settings = current_settings_interfaces;//делаем копию
   isActiveActualData2021 = 0;//action до записи
 }//if(isActiveActualData)

 switch(fileNumber)
 {
  case FILEPERIF_TRANSREGISTER2021:
    return getFileTRANSRegister20(recordNumber, registerNumber, recordLen); 
  case FILEPERIF_ELMFTREGISTER2021:
    return getFileELMFTRegister20(recordNumber, registerNumber, recordLen); 
  case FILEPERIF_ELDTRREGISTER2021:
    return getFileELDTRRegister20(recordNumber, registerNumber, recordLen); 
  case FILEPERIF_ELANDREGISTER2021:
    return getFileELANDRegister20(recordNumber, registerNumber, recordLen); 
  case FILEPERIF_ELORREGISTER2021:
    return getFileELORRegister20(recordNumber, registerNumber, recordLen); 
  case FILEPERIF_ELXORREGISTER2021:
    return getFileELXORRegister20(recordNumber, registerNumber, recordLen); 
  case FILEPERIF_ELNOTREGISTER2021:
    return getFileELNOTRegister20(recordNumber, registerNumber, recordLen); 
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
  case FILEPERIF_IECGOOSEREGISTER2021:
    return getFileIECGOOSERegister20(recordNumber, registerNumber, recordLen); 
  case FILEPERIF_IECMMSREGISTER2021:
    return getFileIECMMSRegister20(recordNumber, registerNumber, recordLen); 
  case FILEPERIF_IECLANREGISTER2021:
    return getFileIECLANRegister20(recordNumber, registerNumber, recordLen); 
#endif
  case FILEPERIF_ELPFREGISTER2021:
    return getFileELPFRegister20(recordNumber, registerNumber, recordLen); 
  case FILEPERIF_PKV1REGISTER2021:
    return getFilePKV1Register20(recordNumber, registerNumber, recordLen); 
  case FILEPERIF_PKV2REGISTER2021:
    return getFilePKV2Register20(recordNumber, registerNumber, recordLen); 
  case FILEPERIF_PKV3REGISTER2021:
    return getFilePKV3Register20(recordNumber, registerNumber, recordLen); 
  case FILEPERIF_PKV4REGISTER2021:
    return getFilePKV4Register20(recordNumber, registerNumber, recordLen); 
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
  case FILEPERIF_PKV5REGISTER2021:
    return getFilePKV5Register20(recordNumber, registerNumber, recordLen); 
#endif
  case FILEPERIF_PKV6REGISTER2021:
    return getFilePKV6Register20(recordNumber, registerNumber, recordLen); 
  case FILEPERIF_REGANLREGISTER2021:
    return getFileREGANLRegister20(recordNumber, registerNumber, recordLen); 
  case FILEPERIF_ANLCONF1REGISTER20:
    return getFileANLCONF1Register20(recordNumber, registerNumber, recordLen); 
  case FILEPERIF_ANLDATA1REGISTER20:
    return getFileANLDATA1Register20(recordNumber, registerNumber, recordLen); 
  case FILEPERIF_ANLDATA2REGISTER20:
    return getFileANLDATA2Register20(recordNumber, registerNumber, recordLen); 
  case FILEPERIF_ANLDATA3REGISTER20:
    return getFileANLDATA3Register20(recordNumber, registerNumber, recordLen); 
  case FILEPERIF_ANLDATA4REGISTER20:
    return getFileANLDATA4Register20(recordNumber, registerNumber, recordLen); 
  case FILEPERIF_ANLDATA5REGISTER20:
    return getFileANLDATA5Register20(recordNumber, registerNumber, recordLen); 
  case FILEPERIF_ANLDATA6REGISTER20:
    return getFileANLDATA6Register20(recordNumber, registerNumber, recordLen); 
  case FILEPERIF_ANLDATA7REGISTER20:
    return getFileANLDATA7Register20(recordNumber, registerNumber, recordLen); 
  case FILEPERIF_ANLDATA8REGISTER20:
    return getFileANLDATA8Register20(recordNumber, registerNumber, recordLen); 
  case FILEPERIF_REGDISREGISTER2021:
    return getFileREGDISRegister20(recordNumber, registerNumber, recordLen); 
  case FILEPERIF_DRCONF1REGISTER20:
    return getFileDRCONF1Register20(recordNumber, registerNumber, recordLen); 
  case FILEPERIF_DRDATA1REGISTER20:
    return getFileDRDATA1Register20(recordNumber, registerNumber, recordLen); 
  case FILEPERIF_DRDATA2REGISTER20:
    return getFileDRDATA2Register20(recordNumber, registerNumber, recordLen); 
  case FILEPERIF_DSREGREGISTER20:
    return getFileDSREGRegister20(recordNumber, registerNumber, recordLen); 

   case FILEPERIF_RPNREGISTER2021:
     return getFileRPNRegister20(recordNumber, registerNumber, recordLen); 
   case FILEPERIF_ZSXREGISTER2021:
     return getFileZSXRegister20(recordNumber, registerNumber, recordLen); 
   case FILEPERIF_ZNXREGISTER2021:
     return getFileZNXRegister20(recordNumber, registerNumber, recordLen); 
   case FILEPERIF_BRPREGISTER2021:
     return getFileBRPRegister20(recordNumber, registerNumber, recordLen); 
 }//switch
 return MARKER_OUTPERIMETR;
}//getPERIFComponentRegister20

////-SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET-

int setPERIFComponentRegister21(int fileNumber, int recordNumber, int recordLen, unsigned short *dataPacket)
{
 if(privatePERIFGetPerimetr(fileNumber) != 0) return MARKER_OUTPERIMETR;

 if(isActiveActualData2021)
 {
   edition_settings = current_settings_interfaces;//делаем копию
   isActiveActualData2021 = 0;//action до записи
 }//if(isActiveActualData)

  switch(fileNumber)
  {
   case FILEPERIF_TRANSREGISTER2021:
     return setFileTRANSRegister21(recordNumber, recordLen, dataPacket); 
   case FILEPERIF_ELMFTREGISTER2021:
     return setFileELMFTRegister21(recordNumber, recordLen, dataPacket); 
   case FILEPERIF_ELDTRREGISTER2021:
     return setFileELDTRRegister21(recordNumber, recordLen, dataPacket); 
   case FILEPERIF_ELANDREGISTER2021:
     return setFileELANDRegister21(recordNumber, recordLen, dataPacket); 
   case FILEPERIF_ELORREGISTER2021:
     return setFileELORRegister21(recordNumber, recordLen, dataPacket); 
   case FILEPERIF_ELXORREGISTER2021:
     return setFileELXORRegister21(recordNumber, recordLen, dataPacket); 
   case FILEPERIF_ELNOTREGISTER2021:
     return setFileELNOTRegister21(recordNumber, recordLen, dataPacket); 
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
   case FILEPERIF_IECGOOSEREGISTER2021:
     return setFileIECGOOSERegister21(recordNumber, recordLen, dataPacket); 
   case FILEPERIF_IECMMSREGISTER2021:
     return setFileIECMMSRegister21(recordNumber, recordLen, dataPacket); 
   case FILEPERIF_IECLANREGISTER2021:
     return setFileIECLANRegister21(recordNumber, recordLen, dataPacket); 
#endif
   case FILEPERIF_ELPFREGISTER2021:
     return setFileELPFRegister21(recordNumber, recordLen, dataPacket); 
   case FILEPERIF_PKV1REGISTER2021:
     return setFilePKV1Register21(recordNumber, recordLen, dataPacket); 
   case FILEPERIF_PKV2REGISTER2021:
     return setFilePKV2Register21(recordNumber, recordLen, dataPacket); 
   case FILEPERIF_PKV3REGISTER2021:
     return setFilePKV3Register21(recordNumber, recordLen, dataPacket); 
   case FILEPERIF_PKV4REGISTER2021:
     return setFilePKV4Register21(recordNumber, recordLen, dataPacket); 
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
   case FILEPERIF_PKV5REGISTER2021:
     return setFilePKV5Register21(recordNumber, recordLen, dataPacket); 
#endif
   case FILEPERIF_PKV6REGISTER2021:
     return setFilePKV6Register21(recordNumber, recordLen, dataPacket); 
   case FILEPERIF_REGANLREGISTER2021:
     return setFileREGANLRegister21(recordNumber, recordLen, dataPacket); 
   case FILEPERIF_REGDISREGISTER2021:
     return setFileREGDISRegister21(recordNumber, recordLen, dataPacket); 

   case FILEPERIF_RPNREGISTER2021:
     return setFileRPNRegister21(recordNumber, recordLen, dataPacket); 
   case FILEPERIF_ZSXREGISTER2021:
     return setFileZSXRegister21(recordNumber, recordLen, dataPacket); 
   case FILEPERIF_ZNXREGISTER2021:
     return setFileZNXRegister21(recordNumber, recordLen, dataPacket); 
   case FILEPERIF_BRPREGISTER2021:
     return setFileBRPRegister21(recordNumber, recordLen, dataPacket); 
  }//switch(fileNumber)

 return MARKER_ERRORPERIMETR;
}//setPERIFComponentRegister21

////-POST--POST--POST--POST--POST--POST--POST--POST--POST--POST--POST-

int postPERIFComponentAction21(int fileNumber, int recordNumber, int recordLen, unsigned short *dataPacket)
{
 if(privatePERIFGetPerimetr(fileNumber) != 0) return MARKER_OUTPERIMETR;

 switch(fileNumber)
 {
   case FILEPERIF_TRANSREGISTER2021:
     return postFileTRANSRegister21(recordNumber, recordLen, dataPacket); 
   case FILEPERIF_ELMFTREGISTER2021:
     return postFileELMFTRegister21(recordNumber, recordLen, dataPacket); 
   case FILEPERIF_ELDTRREGISTER2021:
     return postFileELDTRRegister21(recordNumber, recordLen, dataPacket); 
   case FILEPERIF_ELANDREGISTER2021:
     return postFileELANDRegister21(recordNumber, recordLen, dataPacket); 
   case FILEPERIF_ELORREGISTER2021:
     return postFileELORRegister21(recordNumber, recordLen, dataPacket); 
   case FILEPERIF_ELXORREGISTER2021:
     return postFileELXORRegister21(recordNumber, recordLen, dataPacket); 
   case FILEPERIF_ELNOTREGISTER2021:
     return postFileELNOTRegister21(recordNumber, recordLen, dataPacket); 
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
   case FILEPERIF_IECGOOSEREGISTER2021:
     return postFileIECGOOSERegister21(recordNumber, recordLen, dataPacket); 
   case FILEPERIF_IECMMSREGISTER2021:
     return postFileIECMMSRegister21(recordNumber, recordLen, dataPacket); 
   case FILEPERIF_IECLANREGISTER2021:
     return postFileIECLANRegister21(recordNumber, recordLen, dataPacket); 
#endif
   case FILEPERIF_ELPFREGISTER2021:
     return postFileELPFRegister21(recordNumber, recordLen, dataPacket); 
   case FILEPERIF_PKV1REGISTER2021:
     return postFilePKV1Register21(recordNumber, recordLen, dataPacket); 
   case FILEPERIF_PKV2REGISTER2021:
     return postFilePKV2Register21(recordNumber, recordLen, dataPacket); 
   case FILEPERIF_PKV3REGISTER2021:
     return postFilePKV3Register21(recordNumber, recordLen, dataPacket); 
   case FILEPERIF_PKV4REGISTER2021:
     return postFilePKV4Register21(recordNumber, recordLen, dataPacket); 
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
   case FILEPERIF_PKV5REGISTER2021:
     return postFilePKV5Register21(recordNumber, recordLen, dataPacket); 
#endif
   case FILEPERIF_PKV6REGISTER2021:
     return postFilePKV6Register21(recordNumber, recordLen, dataPacket); 
   case FILEPERIF_REGANLREGISTER2021:
     return postFileREGANLRegister21(recordNumber, recordLen, dataPacket); 
   case FILEPERIF_REGDISREGISTER2021:
     return postFileREGDISRegister21(recordNumber, recordLen, dataPacket); 
 }//switch(fileNumber)

 return MARKER_ERRORPERIMETR;
}//postPERIFComponentAction21

int privatePERIFGetPerimetr(int fileNumber)
{
  //проверить внешний периметр
 return controlPerimetr(fileNumber, BEGIN_FILE_REGISTER, END_FILE_REGISTER);
}//privatePERIFGetPerimetr(int fileNumber)
