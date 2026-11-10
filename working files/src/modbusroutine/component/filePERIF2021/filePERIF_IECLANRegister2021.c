
#include "header.h"
#include "variables_external_m.h"
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)

int privateIECLANvalidRecordLen(int recordNumber, int recordLen);
int privateIECLANRangRecordNumber(int recordNumber);
int getFileIECLANRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileIECLANRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileIECLANRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

#define REGISTERS_LAN 8
//к-во частей внутри одного LAN
#define PART_REGISTERS_LAN 8

int privateIECLANvalidRecordLen(int recordNumber, int recordLen)
{
//валидация recordNumber
  if(recordNumber > N_OUT_LAN*PART_REGISTERS_LAN) return MARKER_ERRORPERIMETR;
//валидация recordLen
  if(recordLen > REGISTERS_LAN+1) return MARKER_ERRORPERIMETR;
  return 0;
}//privateIECLANvalidRecordLen

int privateIECLANRangRecordNumber(int recordNumber)
{
  if(recordNumber > 0) return 1;
  return 0;
}//privateIECLANRangRecordNumber

////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getFileIECLANRegister20(int recordNumber, int registerNumber, int recordLen)
{
//валидация recordLen
  if(privateIECLANvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;
  if(registerNumber == 0) return 0;

  if(privateIECLANRangRecordNumber(recordNumber))
  {
    //Ранжирование
    int item     = (recordNumber - 1)/PART_REGISTERS_LAN;
    int itemPart = (recordNumber - 1)%PART_REGISTERS_LAN;
//поиск активного бита
    uint16_t *ranguvannja_lan = current_settings_interfaces.ranguvannja_Out_LAN[item]
                                [itemPart]; //Ранжування

    int bitInOut  = ranguvannja_lan[registerNumber-1];//offset%REGISTERS_LAN];
    int adr = -1;

    if(bitInOut!=0)
    {
      adr = decoderN_BIGACMD(bitInOut-1);
    }//if(bitInOut!=0)

    return adr<0? 0: adr+getACMDSmallBeginAdr();//декодировщик индекса бита в адрес modbus  для rele
  }//if

  return MARKER_ERRORPERIMETR;
}//getFileIECLANRegister20

////-SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET-
int setFileIECLANRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
//валидация recordLen
  if(privateIECLANvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;

  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    if(privateIECLANRangRecordNumber(recordNumber))
    {
      if(validN_BIGACMD(value) != 0) return MARKER_ERRORPERIMETR;
    }//if

    else return MARKER_ERRORPERIMETR;

  }//for
  return 0;
}//setFileIECLANRegister21

////-POST--POST--POST--POST--POST--POST--POST--POST--POST--POST--POST-
int postFileIECLANRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
  extern int upravlSchematic;//Rang

  if(privateIECLANRangRecordNumber(recordNumber))
  {
    int item     = (recordNumber - 1)/PART_REGISTERS_LAN;
    int itemPart = (recordNumber - 1)%PART_REGISTERS_LAN;

    //добавить новое
    for(int i=0; i<recordLen-1; i++)
    {
      unsigned int adr = (unsigned short) swapByteInShort(dataPacket[i+1]);//новое значение

      //поиск активного бита
      uint16_t *ranguvannja_lan = edition_settings.ranguvannja_Out_LAN[item]
                                  [itemPart]; //Ранжування

      ranguvannja_lan[i] = (encoderN_BIGACMD(adr-getACMDSmallBeginAdr())+1);//кодировщик адреса modbus в индекс бита для реле;
    }//for
//*/
    //ВАЛИДАЦИЯ УСПЕШНА - УСТАНОВКА
    upravlSchematic = 1;//флаг Rang
  }//if

  return 0;
}//postFileIECLANRegister21
#endif
