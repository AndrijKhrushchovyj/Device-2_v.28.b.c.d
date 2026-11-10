#ifndef __PROTOTYPS_H
#define __PROTOTYPS_H

int ustFunc000for2021(int fileRecordNumber, int registerNumber, int *multer, int regUst, unsigned int **editValue);

void uprFuncRead000for2021(int fileRecordNumber, int registerNumber, unsigned int *uprMaska, unsigned int **editControl);
void uprFuncGroupRead000for2021(int fileRecordNumber, int registerNumber);
void uprFuncValidWrite000for2021(int fileRecordNumber, int registerNumber, int inOffset, unsigned int *uprMaska, unsigned int **editControl);

int getUprRegister000for20(int fileRecordNumber, int registerNumber);
int getGroupUprRegister000for20(int fileRecordNumber, int registerNumber, int countGroupBit);
int setGroupUprRegister000for21(int fileRecordNumber, int registerNumber, int countGroupBit, int validData);
int setUprRegister000for21(int fileRecordNumber, int registerNumber, int value);
int postUprRegister000for21(int fileRecordNumber, int registerNumber, unsigned int value);
int postGroupUprRegister000for21(int fileRecordNumber, int registerNumber, int countGroupBit, unsigned int value);

int getUstRegister000for20(int fileRecordNumber, int registerNumber);
int setUstRegister000for21(int fileRecordNumber, int registerNumber, unsigned int value);
int postUstRegister000for21(int fileRecordNumber, int registerNumber, unsigned int value);

void global_component_installation(void);
int  superReaderRegister(int);
int  superWriterRegister(int, int);
int  superReaderBit(int);
int  superWriterBit(int, int);
void superPreAction(void);
int  superPostWriteAction(void);
void superSetOperativMarker(COMPONENT_OBJ *component, int adrReg);
void superSetTempWriteArray(int dataReg);
int  superFindTempWriteArrayOffset(int adr);
void superClearActiveActualData(void);
int  controlPerimetr(int adrReg, int begin_adr, int end_adr);

void writeRangN_BIGModbusRegister2021(unsigned int *ranguvannja_d, unsigned short *dataPacket, int countAdr);
int getRangN_BIGModbusRegister2021(unsigned int *ranguvannja_d, int offset );
void writeRangN_SMALLModbusRegister2021(unsigned int *ranguvannja_d, unsigned short *dataPacket, int countAdr);
int getRangN_SMALLModbusRegister2021(unsigned int *ranguvannja_d, int offset );

int decoderN_BIGACMD(int idxBit);
unsigned int encoderN_BIGACMD(int offsetCMD);
int decoderN_SMALLACMD(int idxBit);
unsigned int encoderN_SMALLACMD(int offsetCMD);
int getACMDSmallBeginAdr(void);
int getSequenceN_BIGIndexActiveBit(int propusk, unsigned int *array);
int getSequenceN_SMALLIndexActiveBit(int propusk, unsigned int *array);
int validN_BIGACMD(unsigned short dataReg);
int validN_SMALLACMD(unsigned short dataReg);
int getRangN_BIGModbusRegister(unsigned int *ranguvannja, int countItem, int offset );
void writeRangN_BIGModbusRegister(unsigned int *ranguvannja, int countItem, int beginAdr, 
                                  int countAdr, int begin_adr_register);
int getRangN_SMALLModbusRegister(unsigned int *ranguvannja, int countItem, int offset );
void writeRangN_SMALLModbusRegister(unsigned int *ranguvannja, int countItem, int beginAdr, 
                                  int countAdr, int begin_adr_register);

unsigned short swapByteInShort(unsigned short data);

void constructorIUSmallComponent(COMPONENT_OBJ *);
void constructorACMDSmallComponent(COMPONENT_OBJ *);
void constructorDOUTSmallComponent(COMPONENT_OBJ *);
void constructorDVSmallComponent(COMPONENT_OBJ *);
void constructorKEYSmallComponent(COMPONENT_OBJ *);
void constructorGCMDSmallComponent(COMPONENT_OBJ *);
void constructorPKVBigComponent(COMPONENT_OBJ *);
void constructorREG1BigComponent(COMPONENT_OBJ *);
void constructorREG2BigComponent(COMPONENT_OBJ *);
void constructorPREGBigComponent(COMPONENT_OBJ *);
void constructorRPRSmallComponent(COMPONENT_OBJ *);
void constructorSDISmallComponent(COMPONENT_OBJ *);
void constructorYustBigComponent(COMPONENT_OBJ *);
void constructorAISmallComponent(COMPONENT_OBJ *);
void constructorRAISmallComponent(COMPONENT_OBJ *);
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
void constructorGOOSSmallComponent(COMPONENT_OBJ *);
void constructorMMSSmallComponent(COMPONENT_OBJ *);
void constructorLANSmallComponent(COMPONENT_OBJ *);
#endif

//void constructorUVVComponent2021(COMPONENT2021_OBJ *);
//void constructorPROTComponent2021(COMPONENT2021_OBJ *);
//void constructorPERIFComponent2021(COMPONENT2021_OBJ *);

int getKEYSmallModbusRegister(int);//получить содержимое регистра
int getREG1BigModbusRegister(int);//получить содержимое регистра
int getREG2BigModbusRegister(int);//получить содержимое регистра

int getIUSmallModbusRegister(int);//получить содержимое регистра
int getACMDSmallModbusRegister(int);//получить содержимое регистра
int getACMDSmallModbusBit(int);//получить содержимое бита
int setACMDSmallModbusBit(int, int);//записать бит
int setACMDSmallModbusRegister(int, int);//записать регистра
int postACMDSmallWriteAction(void);//action после записи

int getACMDSmallModbusRegister(int);//получить содержимое регистра
int getACMDSmallModbusBit(int);//получить содержимое бита
int setACMDSmallModbusBit(int, int);//записать бит
int setACMDSmallModbusRegister(int, int);//записать регистра
int postACMDSmallWriteAction(void);//action после записи

int getDOUTSmallModbusRegister(int);//получить содержимое регистра
int getDOUTSmallModbusBit(int);//получить содержимое bit

int getDVSmallModbusRegister(int);//получить содержимое регистра
int getDVSmallModbusBit(int);//получить содержимое bit

int getKEYSmallModbusRegister(int);//получить содержимое регистра
int getKEYSmallModbusBit(int);//получить содержимое bit

int getGCMDSmallModbusRegister(int);//получить содержимое регистра
int getGCMDSmallModbusBit(int);//получить содержимое bit

int getPKVBigModbusRegister(int);//получить содержимое регистра
int setPKVBigModbusRegister(int, int);//записать регистра
int postPKVBigWriteAction(void);//action после записи

int getREGBigModbusRegister(int);//получить содержимое регистра
int setREGBigModbusRegister(int, int);//записать регистра
int postREGBigWriteAction(void);//action после записи

int getPREGBigModbusRegister(int);//получить содержимое регистра
int setPREGBigModbusRegister(int, int);//записать регистра
int postPREGBigWriteAction(void);//action после записи

int getRPRBigModbusRegister(int);//получить содержимое регистра
int setRPRBigModbusRegister(int, int);//записать регистра
int postRPRBigWriteAction(void);//action после записи

int getRPRSmallModbusRegister(int);//получить содержимое регистра

int getYustBigModbusRegister(int);//получить содержимое регистра
int setYustBigModbusRegister(int, int);//записать регистра
int postYustBigWriteAction(void);//action после записи

int getPFBigModbusRegister(int);//получить содержимое регистра
int setPFBigModbusRegister(int, int);//записать регистра
int postPFBigWriteAction(void);//action после записи

int getRAISmallModbusRegister(int);//получить содержимое регистра

int getSDISmallModbusRegister(int);//получить содержимое регистра
int getSDISmallModbusBit(int);//получить содержимое бита

int getAISmallModbusRegister(int);//получить содержимое регистра
int postREG1BigWriteAction(void);//action после записи
int postREG2BigWriteAction(void);//action после записи

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
int getGOOSSmallModbusRegister(int);//получить содержимое регистра
int getGOOSSmallModbusBit(int);//получить содержимое бита
int getMMSSmallModbusRegister(int);//получить содержимое бита
int getLANSmallModbusRegister(int);//получить содержимое бита
int getMMSSmallModbusBit(int);//получить содержимое бита
int getLANSmallModbusBit(int);//получить содержимое бита
#endif

#endif
