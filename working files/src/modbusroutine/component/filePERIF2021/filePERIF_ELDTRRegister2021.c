
#include "header.h"
#include "variables_external_m.h"

int privateELDTRvalidRecordLen(int recordNumber, int recordLen);
int privateELDTRSelRang1RecordNumber(int recordNumber);
int privateELDTRSelRang2RecordNumber(int recordNumber);
int privateELDTRSelRang3RecordNumber(int recordNumber);
int privateELDTRSelRang4RecordNumber(int recordNumber);
int getFileELDTRRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileELDTRRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileELDTRRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int validDTRN_BIGACMD(unsigned short value, int recordNumber);
int validBazaN_BIGACMD(unsigned short dataReg, int actControl);

#define REGISTERS_ELDTR 6

int privateELDTRvalidRecordLen(int recordNumber, int recordLen)
{
//валидация recordNumber
  if(recordNumber > 4*NUMBER_DEFINED_TRIGGERS) return MARKER_ERRORPERIMETR;
  if(recordNumber == 0) return MARKER_ERRORPERIMETR;
//валидация recordLen
  if(recordLen > REGISTERS_ELDTR+1) return MARKER_ERRORPERIMETR;
  return 0;
}//privateELDTRvalidRecordLen

int privateELDTRSelRang1RecordNumber(int recordNumber)
{
  if((recordNumber%4) == 1) return 1;
  return 0;
}//privateELDTRSelRang1RecordNumber
int privateELDTRSelRang2RecordNumber(int recordNumber)
{
  if((recordNumber%4) == 2) return 1;
  return 0;
}//privateELDTRSelRang2RecordNumber
int privateELDTRSelRang3RecordNumber(int recordNumber)
{
  if((recordNumber%4) == 3) return 1;
  return 0;
}//privateELDTRSelRang3RecordNumber
int privateELDTRSelRang4RecordNumber(int recordNumber)
{
  if((recordNumber%4) == 0) return 1;
  return 0;
}//privateELDTRSelRang4RecordNumber

////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getFileELDTRRegister20(int recordNumber, int registerNumber, int recordLen)
{
//валидация recordLen
  if(privateELDTRvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;
  if(registerNumber == 0) return 0;

  int item  = ((recordNumber - 1)/4)*N_BIG;
  if(privateELDTRSelRang1RecordNumber(recordNumber))
  {
    //Ранжирование
    return getRangN_BIGModbusRegister2021(&current_settings_interfaces.ranguvannja_set_dt_source_plus[item],
                                          registerNumber-1);
  }//if
  else if(privateELDTRSelRang2RecordNumber(recordNumber))
  {
    //Ранжирование
    return getRangN_BIGModbusRegister2021(&current_settings_interfaces.ranguvannja_set_dt_source_minus[item],
                                          registerNumber-1);
  }//if
  else if(privateELDTRSelRang3RecordNumber(recordNumber))
  {
    //Ранжирование
    return getRangN_BIGModbusRegister2021(&current_settings_interfaces.ranguvannja_reset_dt_source_plus[item],
                                          registerNumber-1);
  }//if
  else if(privateELDTRSelRang4RecordNumber(recordNumber))
  {
    //Ранжирование
    return getRangN_BIGModbusRegister2021(&current_settings_interfaces.ranguvannja_reset_dt_source_minus[item],
                                          registerNumber-1);
  }//if

  return MARKER_ERRORPERIMETR;
}//getFileELDTRRegister20

////-SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET-
int setFileELDTRRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
//валидация recordLen
  if(privateELDTRvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;

  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    if(recordNumber > 0) 
    {
      //проверка на конфиг
      if(!(edition_settings.configuration&(1<<EL_BIT_CONFIGURATION))) return MARKER_ERRORPERIMETR;
      //Ранжирование
      if(validDTRN_BIGACMD(value, recordNumber) != 0) return MARKER_ERRORDIAPAZON;
    }//if

    else return MARKER_ERRORPERIMETR;

  }//for
  return 0;
}//setFileELDTRRegister21

////-POST--POST--POST--POST--POST--POST--POST--POST--POST--POST--POST-
int postFileELDTRRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
  extern int upravlSchematic;//Rang

  int item  = ((recordNumber - 1)/4)*N_BIG;
  if(privateELDTRSelRang1RecordNumber(recordNumber))
  {
    writeRangN_BIGModbusRegister2021(&edition_settings.ranguvannja_set_dt_source_plus[item],
                                     &dataPacket[1], recordLen-1);
    //ВАЛИДАЦИЯ УСПЕШНА - УСТАНОВКА
    upravlSchematic = 1;//флаг Rang
  }//if
  else if(privateELDTRSelRang2RecordNumber(recordNumber))
  {
    writeRangN_BIGModbusRegister2021(&edition_settings.ranguvannja_set_dt_source_minus[item],
                                     &dataPacket[1], recordLen-1);
    //ВАЛИДАЦИЯ УСПЕШНА - УСТАНОВКА
    upravlSchematic = 1;//флаг Rang
  }//if
  else if(privateELDTRSelRang3RecordNumber(recordNumber))
  {
    writeRangN_BIGModbusRegister2021(&edition_settings.ranguvannja_reset_dt_source_plus[item],
                                     &dataPacket[1], recordLen-1);
    //ВАЛИДАЦИЯ УСПЕШНА - УСТАНОВКА
    upravlSchematic = 1;//флаг Rang
  }//if
  else if(privateELDTRSelRang4RecordNumber(recordNumber))
  {
    writeRangN_BIGModbusRegister2021(&edition_settings.ranguvannja_reset_dt_source_minus[item],
                                     &dataPacket[1], recordLen-1);
    //ВАЛИДАЦИЯ УСПЕШНА - УСТАНОВКА
    upravlSchematic = 1;//флаг Rang
  }//if

  return 0;
}//postFileELDTRRegister21

int validDTRN_BIGACMD(unsigned short value, int recordNumber)
{
 int item  = (recordNumber - 1)/4;
 return validBazaN_BIGACMD(value, item + (1+SOURCEMARKER_DTR));
}//validDTRN_BIGACMD(unsigned short value, int recordNumber)
