
#include "header.h"
#include "variables_external_m.h"

int privatePKV2validRecordLen(int recordNumber, int recordLen);
int privatePKV2SelUstRecordNumber(int recordNumber);
int getFilePKV2Register20(int recordNumber, int registerNumber, int recordLen);
int setFilePKV2Register21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFilePKV2Register21(int recordNumber, int recordLen, unsigned short *dataPacket);

int privatePKV2SelUstRecordNumber(int recordNumber)
{
  if((recordNumber >= 1) && (recordNumber <= 3)) return 1;
  return 0;
}//privatePKV2SelUstRecordNumber

int privatePKV2validRecordLen(int recordNumber, int recordLen)
{
  UNUSED(recordNumber);
//валидация recordLen
  if(recordLen > 2) return MARKER_ERRORPERIMETR;
  return 0;
}//privatePKV2validRecordLen

////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getFilePKV2Register20(int recordNumber, int registerNumber, int recordLen)
{
//валидация recordLen
  if(privatePKV2validRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;
  if(registerNumber == 0) return 0;

  if(privatePKV2SelUstRecordNumber(recordNumber))
  {
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if(recordNumber == 1) return MARKER_ERRORPERIMETR;//Установка пароля
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    return GET_UST_REGISTER000FOR20(FILEPERIF_PKV2REGISTER2021);
  }//if(privatePKV2SelUstRecordNumber(recordNumber))

  return MARKER_ERRORPERIMETR;
}//getFilePKV2Register20

////-SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET-
int setFilePKV2Register21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
//валидация recordLen
  if(privatePKV2validRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;

  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    if(privatePKV2SelUstRecordNumber(recordNumber))
    {
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      if(recordNumber == 1) return 0;//Установка пароля
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      if(SET_UST_REGISTER000FOR21(FILEPERIF_PKV2REGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
    }//if

    else return MARKER_ERRORPERIMETR;

  }//for
  return 0;
}//setFilePKV2Register21

////-POST--POST--POST--POST--POST--POST--POST--POST--POST--POST--POST-
int postFilePKV2Register21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
  extern int upravlSetting;//флаг Setting
  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    if(privatePKV2SelUstRecordNumber(recordNumber))
    {
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      if(recordNumber == 1) 
      {
       //Установка пароля
        int passwordS = -1;
        switch(pointInterface)//метка интерфейса 0-USB 1-RS485
        {
         case USB_RECUEST:
          passwordS = password_set_USB;//Пароль установлен
         break;
         case RS485_RECUEST:
          passwordS = password_set_RS485;//Пароль установлен
         break;
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
         case LAN_RECUEST:
          passwordS = password_set_LAN;//Пароль установлен
         break;
#endif
        }//switch
       if(passwordS == 0) //пароль снят
       {
         uint32_t *editValue = NULL;
  
         switch(pointInterface)//метка интерфейса 0-USB 1-RS485
         {
          case USB_RECUEST:
            editValue = &edition_settings.password_interface_USB;
          break;
          case RS485_RECUEST:
           editValue = &edition_settings.password_interface_RS485;
          break;
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
          case LAN_RECUEST:
           editValue = &edition_settings.password_interface_LAN;
          break;
#endif
         }//switch
         if((*editValue) == 0) return 0;//спроба повторного зняття паролю
         //записать новый пароль
         *editValue = value;
         upravlSetting = 1;//флаг Setting
  
         if(value != 0)
         {
            //активир флажок пароля
            switch(pointInterface)//метка интерфейса 0-USB 1-RS485
            {
             case USB_RECUEST:
              password_set_USB = 1;//Пароль установлен
             break;
             case RS485_RECUEST:
              password_set_RS485 = 1;//Пароль установлен
             break;
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
             case LAN_RECUEST:
              password_set_LAN = 1;//Пароль установлен
             break;
#endif
            }//switch
         }//if(value != 0)
       }//if(passwordS == 0) //пароль снят
       else return MARKER_ERRORDIAPAZON;//спроба запису при незнятому паролi

       return 0;
      }//if(recordNumber == 1) 
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      int errorV = POST_UST_REGISTER000FOR21(FILEPERIF_PKV2REGISTER2021);
      if(errorV == 1) continue;
    }//if

  }//for

  return 0;
}//postFilePKV2Register21
