
#include "header.h"
#include "variables_external_m.h"

int privateValidatorUstUP(int regUst, int item, int lowHiReg, int *multer);
int privateUPvalidRecordLen(int recordNumber, int recordLen);
int privateUPSelUstRecordNumber(int recordNumber);
int privateUPSelCountGroupBitRecordNumber(int recordNumber);
int getFileUPRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileUPRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileUPRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

int privateUPSelUstRecordNumber(int recordNumber)
{
  if((recordNumber == 2)     ||
      (recordNumber == 102)     ||
      (recordNumber == 202)     ||
      (recordNumber == 302)     ||
      (recordNumber == 402)     ||
      (recordNumber == 502)     ||
      (recordNumber == 602)     ||
      (recordNumber == 702)
    ) return 1;
  if((recordNumber >= 5 && recordNumber <= 7)     ||
      (recordNumber >= 105 && recordNumber <= 107) ||
      (recordNumber >= 205 && recordNumber <= 207) ||
      (recordNumber >= 305 && recordNumber <= 307) ||
      (recordNumber >= 405 && recordNumber <= 407) ||
      (recordNumber >= 505 && recordNumber <= 507) ||
      (recordNumber >= 605 && recordNumber <= 607) ||
      (recordNumber >= 705 && recordNumber <= 707)) return 1;
  return 0;
}//privateUPvalidRecordNumber

int privateUPvalidRecordLen(int recordNumber, int recordLen)
{
//валидация recordLen
  switch(recordNumber)
  {
  case 0:
  case 1:
  case 101:
  case 201:
  case 301:
  case 401:
  case 501:
  case 601:
  case 701:
  case 2:
  case 102:
  case 202:
  case 302:
  case 402:
  case 502:
  case 602:
  case 702:
    if(recordLen>2) return MARKER_ERRORPERIMETR;
    break;
  case 5:
  case 105:
  case 205:
  case 305:
  case 405:
  case 505:
  case 605:
  case 705:
    if(recordLen>9) return MARKER_ERRORPERIMETR;
    break;
  default:
    if(recordLen>5) return MARKER_ERRORPERIMETR;
  }//switch
  return 0;
}//privateUPvalidRecordLen

int privateUPSelCountGroupBitRecordNumber(int recordNumber)
{
  if((recordNumber == 1)   || (recordNumber == 101) || (recordNumber == 201) || (recordNumber == 301) ||
      (recordNumber == 401) || (recordNumber == 501) || (recordNumber == 601) || (recordNumber == 701))
    return 3;//к-сть Group біт
  return -1;
}//privateUPSelCountGroupBitRecordNumber
////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getFileUPRegister20(int recordNumber, int registerNumber, int recordLen)
{
//валидация recordLen
  if(privateUPvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;
  if(registerNumber == 0) return 0;

  int  countGroupBit = privateUPSelCountGroupBitRecordNumber(recordNumber);//к-сть Group біт

  if(recordNumber == 0) return GET_UPR_REGISTER000FOR20(FILEPROT_UPREGISTER2021);

  else if(countGroupBit > 0)
  {
    return GET_GROUPUPR_REGISTER000FOR20(FILEPROT_UPREGISTER2021);
  }//if(countGroupBit>0)
  else if(privateUPSelUstRecordNumber(recordNumber))
  {
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    int item = recordNumber/100;
    if((recordNumber%100) == 5)//if(editValue == (uint32_t*)&edition_settings.setpoint_UP[item][0][gruppa])
    {
      int grupa_ustavok = (registerNumber - 1)/2;

      uint32_t *editValue = (uint32_t*)&edition_settings.setpoint_UP[item][0][grupa_ustavok];

      int multer = 10;
      privateValidatorUstUP(0, item, (registerNumber-1)&1, &multer);//load multer

      uint32_t temp = ((uint32_t)(*editValue))/multer;
      if((registerNumber-1)&1)//мл регистр
        return temp&0xFFFF;
/*
      else//ct регистр
      {
        switch(edition_settings.ctrl_UP_input[item])
        {
        case UP_CTRL_P:
        case UP_CTRL_Q:
        case UP_CTRL_S:
          //ct регистр
          return (((*editValue)/multer)>>16)&0xFFFF;
        }//switch
        if(temp>60000) return 0xFFFF;
        return (temp>>16)&0xFFFF;
      }//else
*/
    }//if((recordNumber%100) == 5)//if(editValue == (uint32_t*)&edition_settings.setpoint_UP[item][0][gruppa])
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    /*
    В1:0-(Ia+Ib+Ic);1-Ia;2-Ib;3-Ic;4-I1;5-I2;6-I04;7-3I0-1;8-3I0;9-3I0h;10-Ua+Ub+Uc;11-Ua;12-Ub;13-Uc;14-Uab+Ubc+Uca;15-Uab;
    16-Ubc;17-Uca;18-U1;19-U2;20-3U0;21-3U0-1;22-P;23-Q;24-S;25-F;26-Uab_ТВП;27-Ubc_ТВП;28-(Uab+Ubc)ТВП;29-df/dt1;30-df/dt2;
    31-df/dt3;32-df/dt4;33-df/dt5;34-df/dt6;35-df/dt7;36-df/dt8;37-Uосн;38-Iосн;39-Pосн;40-Qосн;41-Sосн;42-Uл;
    В2:100-(Ia+Ib+Ic);101-Ia;102-Ib;103-Ic;104-I1;105-I2;106-I04;107-3I0-1;108-3I0;109-3I0h;110-Ua+Ub+Uc;111-Ua;112-Ub;113-Uc;
    114-Uab+Ubc+Uca;115-Uab;116-Ubc;117-Uca;118-U1;119-U2;120-3U0;121-3U0-1;122-P;123-Q;124-S;125-F;126-Uab_ТВП;
    127-Ubc_ТВП;128-(Uab+Ubc)ТВП;129-df/dt1;130-df/dt2;131-df/dt3;132-df/dt4;133-df/dt5;134-df/dt6;135-df/dt7;136-df/dt8;
    137-Uрез;138-Iрез;139-Pрез;140-Qрез;141-Sрез;142-Uл*/
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if((recordNumber%100) == 2)//(*editValue) = (uint32_t*)&edition_settings.ctrl_UP_input[item];
    {
      uint32_t *editValue = (uint32_t*)&edition_settings.ctrl_UP_input[item];
      switch(*editValue)
      {
      case UP_CTRL_Ia_TN1:
        return 1;//Ia_TN1;
      case UP_CTRL_Ia_TN2:
        return 101;//Ia_TN2;
      case UP_CTRL_Uab_TN1:
        return 15;//Uab_TN1
      case UP_CTRL_Uab_TN2:
        return 115;//Uab_TN2
      case UP_CTRL_F_TN1:
        return 25;//F_TN1
      case UP_CTRL_F_TN2:
        return 125;//F_TN2
      }//switch
      return 0;
    }//if((recordNumber%100) == 2)//(*editValue) = (uint32_t*)&edition_settings.ctrl_UP_input[item];
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    return GET_UST_REGISTER000FOR20(FILEPROT_UPREGISTER2021);
  }

  return MARKER_ERRORPERIMETR;
}//getFileUPRegister20

////-SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET-
int setFileUPRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
//валидация recordLen
  if(privateUPvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;

  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);
    int  countGroupBit = privateUPSelCountGroupBitRecordNumber(recordNumber);//к-сть Group біт

    if(recordNumber == 0)
    {
      if(SET_UPR_REGISTER000FOR21(FILEPROT_UPREGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
    }//if

    else if(countGroupBit > 0)
    {
      if(SET_GROUPUPR_REGISTER000FOR21(FILEPROT_UPREGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
    }//if(countGroupBit>0)

    else if(privateUPSelUstRecordNumber(recordNumber))
    {
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      if((recordNumber%100) == 5)
      {
        int item = recordNumber/100;
        int multer = 10;

        if(!privateValidatorUstUP(value, item, (registerNumber-1)&1, &multer)) return MARKER_ERRORDIAPAZON;
        continue;
      }//if((recordNumber%100) == 5)
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      if((recordNumber%100) == 2)//(*editValue) = (uint32_t*)&edition_settings.ctrl_UP_input[item];
      {
        continue;
      }//if((recordNumber%100) == 2)//(*editValue) = (uint32_t*)&edition_settings.ctrl_UP_input[item];
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      if(SET_UST_REGISTER000FOR21(FILEPROT_UPREGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
    }//if
    else return MARKER_ERRORPERIMETR;

  }//for
  return 0;
}//setFileUPRegister21

////-POST--POST--POST--POST--POST--POST--POST--POST--POST--POST--POST-
int postFileUPRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
  extern int upravlSetting;//флаг Setting
  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    int  countGroupBit = privateUPSelCountGroupBitRecordNumber(recordNumber);//к-сть Group біт

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    if(recordNumber == 0)
    {
      int errorV = POST_UPR_REGISTER000FOR21(FILEPROT_UPREGISTER2021);
      if(errorV == 1) continue;
      if(errorV != 0) return errorV;
    }//if

    else if(countGroupBit>0)
    {
      int errorV = POST_GROUPUPR_REGISTER000FOR21(FILEPROT_UPREGISTER2021);
      if(errorV != 0) return errorV;
    }//if(countGroupBit>0)

    else if(privateUPSelUstRecordNumber(recordNumber))
    {
////ОСОБАЯ СБОРКА
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      if((recordNumber%100) == 5)//if(editValue == (uint32_t*)&edition_settings.setpoint_UP[item][0][grupa_ustavok])
      {
        int grupa_ustavok = (registerNumber - 1)/2;
        int item = recordNumber/100;

        uint32_t *editValue = (uint32_t*)&edition_settings.setpoint_UP[item][0][grupa_ustavok];

        int multer = 10;
        int lowHi = (registerNumber-1)&1;

        privateValidatorUstUP(value, item, lowHi, &multer);//load multer

        value = value*multer;
        if(lowHi)
        {
          //мл регистр
          upravlSetting = 1;//флаг Setting
          switch(edition_settings.ctrl_UP_input[item])
          {
/*
          case UP_CTRL_P:
          case UP_CTRL_Q:
          case UP_CTRL_S:
//          case UP_CTRL_UL:
            (*editValue)  &= 0xffff0000;
            (*editValue)  |= (value & 0xffff);//
            continue;
*/
          default:
           (*editValue) = value;
            continue;
          }//switch
        }//if(offset&1)
        else
        {
          //ст регистр
          switch(edition_settings.ctrl_UP_input[item])
          {
/*
          case UP_CTRL_P:
          case UP_CTRL_Q:
          case UP_CTRL_S:
//          case UP_CTRL_UL:
            (*editValue)  &= 0xffff;
            (*editValue)  |= value<<16;//
            upravlSetting = 1;//флаг Setting
            continue;
*/
          default:
            continue;
          }//switch
        }//else

      }//if((recordNumber%100) == 5) if(editValue == (uint32_t*)&edition_settings.setpoint_UP[item][0][grupa_ustavok])
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      if((recordNumber%100) == 2)//(*editValue) = (uint32_t*)&edition_settings.ctrl_UP_input[item];
      {
        int item = recordNumber/100;
        uint32_t *editValue = (uint32_t*)&edition_settings.ctrl_UP_input[item];
        switch(value)
        {
      case 1://Ia_TN1;
         value = UP_CTRL_Ia_TN1;
          break;
      case 101://Ia_TN2;
         value = UP_CTRL_Ia_TN2;
          break;
      case 15://Uab_TN1
         value = UP_CTRL_Uab_TN1;
          break;
      case 115://Uab_TN2
         value = UP_CTRL_Uab_TN2;
          break;
      case 25://F_TN1
         value = UP_CTRL_F_TN1;
          break;
      case 125://F_TN2
         value = UP_CTRL_F_TN2;
          break;
        default:
          return MARKER_ERRORDIAPAZON;
        }//switch

        (*editValue) = value;

        uint32_t tmp = edition_settings.ctrl_UP_input[item];
        edition_settings.ctrl_UP_input[item] = -1;
        action_after_changing_input_UP(&edition_settings, item, tmp);

        upravlSetting = 1;//флаг Setting
        continue;
      }//if((recordNumber%100) == 2)//(*editValue) = (uint32_t*)&edition_settings.ctrl_UP_input[item];
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      int errorV = POST_UST_REGISTER000FOR21(FILEPROT_UPREGISTER2021);
      if(errorV == 1) continue;
    }//if
  }//for

////ОСОБАЯ ПРОВЕРКА
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
/*
  if((recordNumber%100) == 5)//if(editValue == (uint32_t*)&edition_settings.setpoint_UP[item][0][grupa_ustavok])
  {
    for(int grupa_ustavok=0; grupa_ustavok<4; grupa_ustavok++)
    {
      int item = recordNumber/100;
      int temp1 = edition_settings.setpoint_UP[item][0][grupa_ustavok];

      switch(edition_settings.ctrl_UP_input[item])
      {
      case UP_CTRL_P:
      case UP_CTRL_Q:
        if(temp1<0) temp1 = -temp1;//модуль
        if(temp1<SETPOINT_UP_P_MIN ||
            temp1>SETPOINT_UP_P_MAX
          )
        {
          upravlSetting = 0;//флаг Setting
          return ERROR_VALID2;//ошибка валидации
        }
        break;

      case UP_CTRL_S:
        if(temp1<SETPOINT_UP_P_MIN ||
            temp1>SETPOINT_UP_P_MAX
          )
        {
          upravlSetting = 0;//флаг Setting
          return ERROR_VALID2;//ошибка валидации
        }
        break;
      }//switch
   }//for
  }//if((recordNumber%100) == 5)//if(editValue == (uint32_t*)&edition_settings.setpoint_UP[item][0][grupa_ustavok])
*/
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  return 0;
}//postFileUPRegister21

int privateValidatorUstUP(int regUst, int item, int lowHiReg, int *multer)
{
  switch(edition_settings.ctrl_UP_input[item])
  {
  case UP_CTRL_F_TN1:
  case UP_CTRL_F_TN2:
     (*multer) = 1;
    if(lowHiReg == 0)
    {
      if(regUst != 0) return 0;//нет старшего регистра
    }//if
    else if(regUst<STP_UP_F_MIN || regUst>STP_UP_F_MAX) return 0;
  break;
      case UP_CTRL_Ia_TN1:
      case UP_CTRL_Ia_TN2:
    if(lowHiReg == 0)
    {
      if(regUst != 0) return 0;//нет старшего регистра
    }//if
    else if(regUst<SETPOINT_UP_I_MIN/10 || regUst>SETPOINT_UP_I_MAX/10) return 0;
    break;
/*
      case UP_CTRL_Uab_TN1:
      case UP_CTRL_Uab_TN2:
    (*multer) = 1;
    if(lowHiReg == 0)
    {
      if(regUst != 0) return 0;//нет старшего регистра
    }//if
    else if(regUst<SETPOINT_UP_3I0_MIN || regUst>SETPOINT_UP_3I0_MAX) return 0;
    break;
*/
      case UP_CTRL_Uab_TN1:
      case UP_CTRL_Uab_TN2:
    (*multer) = 100;
    if(lowHiReg == 0)
    {
      if(regUst != 0) return 0;//нет старшего регистра
    }//if
    else if(regUst<SETPOINT_UP_U_MIN/100 || regUst>SETPOINT_UP_U_MAX/100) return 0;
    break;
/*
  case UP_CTRL_P:
  case UP_CTRL_Q:
  case UP_CTRL_S:
    (*multer) = 1;
    break;
*/
  }//switch

  return 1;
}//privateValidatorUstUP

