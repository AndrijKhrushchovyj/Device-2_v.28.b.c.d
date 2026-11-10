
#include "header.h"
#include "variables_external_m.h"

//к-во ранжир на одном key
#define REGISTERS_KEY 8

int privateKEYvalidRecordLen(int recordNumber, int recordLen);
int privateKEYSelUstRecordNumber(int recordNumber);
int privateKEYSelRangRecordNumber(int recordNumber);
int getFileKEYRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileKEYRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileKEYRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

/**************************************/
//подготовка компонента KEY
/**************************************/

int privateKEYvalidRecordLen(int recordNumber, int recordLen)
{
//валидация recordNumber
  if(recordNumber == 0) return MARKER_ERRORPERIMETR;
  if((recordNumber-1)/10 > NUMBER_DEFINED_BUTTONS) return MARKER_ERRORPERIMETR;
  if((recordNumber-1)%10 > 1) return MARKER_ERRORPERIMETR;
 //валидация recordLen
  if((recordNumber-1)%10 == 0)if(recordLen > 2) return MARKER_ERRORPERIMETR;
  if((recordNumber-1)%10 == 1)if(recordLen > REGISTERS_KEY+1) return MARKER_ERRORPERIMETR;
  return 0;
}//privateKEYvalidRecordLen

int privateKEYSelUstRecordNumber(int recordNumber)
{
  if((recordNumber-1)%10 == 0) return 1;
  return 0;
}//privateKEYSelUstRegisterNumber
int privateKEYSelRangRecordNumber(int recordNumber)
{
  if((recordNumber-1)%10 == 1) return 1;
  return 0;
}//privateKEYSelRangRegisterNumber

////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getFileKEYRegister20(int recordNumber, int registerNumber, int recordLen)
{
//валидация recordLen recordNumber
  if(privateKEYvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;
  if(registerNumber == 0) return 0;

  int item  = (recordNumber - 1)/10;

  if(privateKEYSelUstRecordNumber(recordNumber))
  {
//- - - - - - - - - - - - - - - - - - - - - - - - - - -
    if(edition_settings.buttons_mode & (1<<item)) return 1;
    return 0;
//- - - - - - - - - - - - - - - - - - - - - - - - - - -
  }//if
  else if(privateKEYSelRangRecordNumber(recordNumber))
  {
    //Ранжирование KEY
    return getRangN_SMALLModbusRegister2021(&current_settings_interfaces.ranguvannja_buttons[item*N_SMALL],
                                            registerNumber-1);
  }//if
  return MARKER_ERRORPERIMETR;
}//getFileKEYRegister20

////-SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET-
int setFileKEYRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
//валидация recordLen recordNumber
  if(privateKEYvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;

  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    if(privateKEYSelUstRecordNumber(recordNumber))
    {
//- - - - - - - - - - - - - - - - - - - - - - - - - - -
      //проверкa ust
      if(value > 1) return MARKER_ERRORDIAPAZON;
//- - - - - - - - - - - - - - - - - - - - - - - - - - -
    }//if
    else if(privateKEYSelRangRecordNumber(recordNumber))
    {
      //Ранжирование KEY
      if(validN_SMALLACMD(value) != 0) return MARKER_ERRORDIAPAZON;

        switch((unsigned short)value)
        {
          case 26731://RANG_PEREKLYUCHENNYA_RPN
          case 26753://RANG_UBAVYTY_FROM_DV_RPN
          case 26761://RANG_PRYBAVYTY_FROM_DV_RPN
          case 26732://RANG_1_POLOGENNJA_RPN
          case 26733://RANG_NOMINALNA_POZYCIJA_RPN
          case 26734://RANG_N_POLOGENNJA_RPN;
          case 26781://RANG_KONTROL_UBAVYTY_ZSKh
          case 26782://RANG_KONTROL_PRYBAVYTY_ZSKh

          return MARKER_ERRORPERIMETR; //не прошла валидация
        }//switch

      int item  = (recordNumber - 1)/10;
      uint32_t maska = ((uint32_t)edition_settings.buttons_mode)&(1<<item);
      //проверка на допустимость Режим «Кнопка»
      if(!maska)
      {
        switch((unsigned short)value)
        {
        case 25000://- 1-а гр.уставок    *
        case 25001://- 2-а гр.уставок     *
        case 25002://- 3-а гр.уставок     *
        case 25003://- 4-а гр.уставок     *

        case 25368://RNG_SMALL_U_BLOCK_UMIN_1;
        case 25384://RNG_SMALL_U_BLOCK_UMIN_2;

        case 25448://RNG_SMALL_U_BLOCK_UMAX_2;

        case 25896://- Блок.УЗ1
        case 25899://- Блок.УЗ2
        case 25902://- Блок.УЗ3
        case 25905://- Блок.УЗ4
        case 25908://- Блок.УЗ5
        case 25911://- Блок.УЗ6
        case 25914://- Блок.УЗ7
        case 25917://- Блок.УЗ8

//        case 26298://RNG_SMALL_RESET_LEDS;
//        case 26299://RNG_SMALL_RESET_RELES;
        case 26304://RNG_SMALL_MISCEVE_DYSTANCIJNE;

//        case 26729://RANG_CLEAR_BLK_RPN;
//        case 26752://RANG_UBAVYTY_FROM_UPPER_LEVEL_RPN;
//        case 26760://RANG_PRYBAVYTY_FROM_UPPER_LEVEL_RPN

        case 26773://RANG_AUTO_MODE_RPN
        case 26778://RANG_TM_MODE_VID_DV_RPN

        case 26730://RANG_BLOCK_STRUM_KOMP_RPN

        case 26737://RANG_ZOVNISHNJE_BLOCKUVANNJA_RPN

        case 26789://RANG_BLOCK_BRP

          return MARKER_ERRORPERIMETR; //не прошла валидация
        }//switch
        if((value >= 26520) && (value <= 26555)) return MARKER_ERRORPERIMETR; //не прошла валидация
      }//if(!maska)
      else
        //проверка на допустимость Режим «Ключ
        switch((unsigned short)value)
        {
//        case 25000://- 1-а гр.уставок    *
//        case 25001://- 2-а гр.уставок     *
//        case 25002://- 3-а гр.уставок     *
//        case 25003://- 4-а гр.уставок     *

//        case 25368://RNG_SMALL_U_BLOCK_UMIN_1;
//        case 25384://RNG_SMALL_U_BLOCK_UMIN_2;

//        case 25448://RNG_SMALL_U_BLOCK_UMAX_2;

//        case 25896://- Блок.УЗ1
//        case 25899://- Блок.УЗ2
//        case 25902://- Блок.УЗ3
//        case 25905://- Блок.УЗ4
//        case 25908://- Блок.УЗ5
//        case 25911://- Блок.УЗ6
//        case 25914://- Блок.УЗ7
//        case 25917://- Блок.УЗ8

        case 26298://RNG_SMALL_RESET_LEDS;
        case 26299://RNG_SMALL_RESET_RELES;
//        case 26304://RNG_SMALL_MISCEVE_DYSTANCIJNE;

        case 26729://RANG_CLEAR_BLK_RPN;
        case 26752://RANG_UBAVYTY_FROM_UPPER_LEVEL_RPN;
        case 26760://RANG_PRYBAVYTY_FROM_UPPER_LEVEL_RPN

//        case 26778://RANG_TM_MODE_VID_DV_RPN
//        case 26773://RANG_AUTO_MODE_RPN

//        case 26730://RANG_BLOCK_STRUM_KOMP_RPN

//        case 26737://RANG_ZOVNISHNJE_BLOCKUVANNJA_RPN

//        case 26789://RANG_BLOCK_BRP

          return MARKER_ERRORPERIMETR; //не прошла валидация
        }//switch

    }//if
   else return MARKER_ERRORPERIMETR;
  }//for
  return 0;
}//setFileKEYRegister21

////-POST--POST--POST--POST--POST--POST--POST--POST--POST--POST--POST-
int postFileKEYRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
  extern int upravlSchematic;//флаг Rang
  extern int upravlSetting;//флаг Setting

  int item  = (recordNumber - 1)/10;
  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)//номера регистров
  {
    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);
    if(privateKEYSelUstRecordNumber(recordNumber))
    {
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      if(value) edition_settings.buttons_mode |= (1<<item);
      else  edition_settings.buttons_mode &= ~(1<<item);

      action_during_changing_button_mode(&current_settings_interfaces, &edition_settings);

      for (unsigned int i = 0; i < N_SMALL; i++)
      {
        edition_settings.ranguvannja_buttons[N_SMALL*item + i] =
          current_settings_interfaces.ranguvannja_buttons[N_SMALL*item + i];
      }//for
//- - - - - - - - - - - - - - - - - - - - - - - - - - -
      upravlSetting = 1;//флаг Setting
      upravlSchematic = 1;//флаг Rang
    }//if
  }//for

  if(privateKEYSelRangRecordNumber(recordNumber))
  {
    writeRangN_SMALLModbusRegister2021(&edition_settings.ranguvannja_buttons[item * N_SMALL], &dataPacket[1], recordLen-1);
    //ВАЛИДАЦИЯ УСПЕШНА - УСТАНОВКА
    upravlSchematic = 1;//флаг Rang
  }//if

  return 0;
}//postFileKEYRegister21

