
#include "header.h"
#include "variables_external_m.h"
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)

int privateIECMMSvalidRecordLen(int recordNumber, int recordLen);
int privateIECMMSRangRecordNumber(int recordNumber);
int getFileIECMMSRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileIECMMSRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileIECMMSRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

#define REGISTERS_MMS 8
//к-во частей внутри одного MMS
#define PART_REGISTERS_MMS 8

int privateIECMMSvalidRecordLen(int recordNumber, int recordLen)
{
//валидация recordNumber
  if(recordNumber > N_IN_MMS*PART_REGISTERS_MMS) return MARKER_ERRORPERIMETR;
//валидация recordLen
  if(recordLen > REGISTERS_MMS+1) return MARKER_ERRORPERIMETR;
  return 0;
}//privateIECMMSvalidRecordLen

int privateIECMMSRangRecordNumber(int recordNumber)
{
  if(recordNumber > 0) return 1;
  return 0;
}//privateIECMMSRangRecordNumber

////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getFileIECMMSRegister20(int recordNumber, int registerNumber, int recordLen)
{
//валидация recordLen
  if(privateIECMMSvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;
  if(registerNumber == 0) return 0;

  if(privateIECMMSRangRecordNumber(recordNumber))
  {
    //Ранжирование
    int item     = (recordNumber - 1)/PART_REGISTERS_MMS;
    int itemPart = (recordNumber - 1)%PART_REGISTERS_MMS;
    return getRangN_SMALLModbusRegister2021(current_settings_interfaces.ranguvannja_In_MMS[item][itemPart],
                                            registerNumber-1);
  }//if

  return MARKER_ERRORPERIMETR;
}//getFileIECMMSRegister20

////-SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET-
int setFileIECMMSRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
//валидация recordLen
  if(privateIECMMSvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;

  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    if(privateIECMMSRangRecordNumber(recordNumber))
    {
      switch((unsigned short)value)
      {
        case 0://standart
        case 26298://RNG_SMALL_RESET_LEDS;
        case 26299://RNG_SMALL_RESET_RELES;

        case 26360://RNG_SMALL_DF1_IN;
        case 26361://RNG_SMALL_DF1_IN;
        case 26362://RNG_SMALL_DF1_IN;
        case 26363://RNG_SMALL_DF1_IN;
        case 26364://RNG_SMALL_DF1_IN;
        case 26365://RNG_SMALL_DF1_IN;
        case 26366://RNG_SMALL_DF1_IN;
        case 26367://RNG_SMALL_DF1_IN;

        case 26376://RNG_SMALL_DF1_RST;
        case 26377://RNG_SMALL_DF1_RST;
        case 26378://RNG_SMALL_DF1_RST;
        case 26379://RNG_SMALL_DF1_RST;
        case 26380://RNG_SMALL_DF1_RST;
        case 26381://RNG_SMALL_DF1_RST;
        case 26382://RNG_SMALL_DF1_RST;
        case 26383://RNG_SMALL_DF1_RST;

        case 26408://RNG_SMALL_DT1_SET;
        case 26409://RNG_SMALL_DT1_SET;
        case 26410://RNG_SMALL_DT1_SET;
        case 26411://RNG_SMALL_DT1_SET;

        case 26424://RNG_DT1_RESET;
        case 26425://RNG_DT1_RESET;
        case 26426://RNG_DT1_RESET;
        case 26427://RNG_DT1_RESET;

        case 26568://RANG_SMALL_U_LF1;
        case 26569://RANG_SMALL_U_LF1;
        case 26570://RANG_SMALL_U_LF1;
        case 26571://RANG_SMALL_U_LF1;
        case 26572://RANG_SMALL_U_LF1;
        case 26573://RANG_SMALL_U_LF1;
        case 26574://RANG_SMALL_U_LF1;
        case 26575://RANG_SMALL_U_LF1;
        case 26576://RANG_SMALL_U_LF1;
        case 26577://RANG_SMALL_U_LF1;
        case 26578://RANG_SMALL_U_LF1;
        case 26579://RANG_SMALL_U_LF1;
        case 26580://RANG_SMALL_U_LF1;
        case 26581://RANG_SMALL_U_LF1;
        case 26582://RANG_SMALL_U_LF1;
        case 26583://RANG_SMALL_U_LF1;

        return 0; //прошла валидация
      }//switch

      return MARKER_ERRORPERIMETR;//не прошла валидация
    }//if

    else return MARKER_ERRORPERIMETR;

  }//for
  return 0;
}//setFileIECMMSRegister21

////-POST--POST--POST--POST--POST--POST--POST--POST--POST--POST--POST-
int postFileIECMMSRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
  extern int upravlSchematic;//Rang

  if(privateIECMMSRangRecordNumber(recordNumber))
  {
    int item     = (recordNumber - 1)/PART_REGISTERS_MMS;
    int itemPart = (recordNumber - 1)%PART_REGISTERS_MMS;
    writeRangN_SMALLModbusRegister2021(edition_settings.ranguvannja_In_MMS[item][itemPart], &dataPacket[1], recordLen-1);
    //ВАЛИДАЦИЯ УСПЕШНА - УСТАНОВКА
    upravlSchematic = 1;//флаг Rang
  }//if

  return 0;
}//postFileIECMMSRegister21
#endif
