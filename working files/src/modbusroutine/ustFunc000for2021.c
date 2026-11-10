
#include "header.h"
#include "variables_external_m.h"

/**************************************/
//База данных уставок
/**************************************/
//int privateArmCosF32(float temp_value_f);
//int privateArmSinF32(float temp_value_f);

int postUstRegister000for21(int fileRecordNumber, int registerNumber, unsigned int value)
{
  extern int upravlSetting;//флаг Setting
  int  multer = 1;
  uint32_t *editValue = NULL;
  ustFunc000for2021(fileRecordNumber, registerNumber, &multer, 0, &editValue);
  if(editValue == NULL) return 1;

  (*editValue) = value*multer;

  //ustFunc000for2021(-fileRecordNumber, registerNumber, &multer, 0, &editValue);//actControl=1

  upravlSetting = 1;//флаг Setting
  return 0;
}//postUstRegister21

int getUstRegister000for20(int fileRecordNumber, int registerNumber)
{
  int  multer = 1;
  uint32_t *editValue = NULL;
  ustFunc000for2021(fileRecordNumber, registerNumber, &multer, 0, &editValue);
  if(editValue == NULL) return 0;
  return ((*editValue)/multer) &0xFFFF;
}//getUstRegister20
int setUstRegister000for21(int fileRecordNumber, int registerNumber, unsigned int value)
{
  int  multer = 1;
  uint32_t *editValue = NULL;
  if(!ustFunc000for2021(fileRecordNumber, registerNumber, &multer, value, &editValue)) return MARKER_ERRORDIAPAZON;
  return 0;
}//setUstRegister21

//#pragma optimize=none
int ustFunc000for2021(int fileRecordNumber, int registerNumber, int *multer, int regUst, uint32_t **editValue)
{
  int diapazon = 1;

  (*multer) = 10;

  int fileNumber       = fileRecordNumber&0xFFFF0000;//номер файла
  int recordNumber     = fileRecordNumber&0x0000FFFF;//номер запису

  int grupa_ustavok = registerNumber - 1;

  switch(fileNumber)
  {
  case (FILENUMBER(FILEUVV_DVREGISTER2021)):
  {
    int item = (recordNumber-1)/10;
    (*editValue) = (uint32_t*)&edition_settings.dopusk_dv[item];
    (*multer) = 1;
  }//case (FILENUMBER(FILEUVV_DVREGISTER2021))
  break;

////MFT+MFT+MFT+MFT+MFT+MFT+MFT+MFT+MFT+MFT+MFT+MFT+MFT+MFT+MFT+MFT+MFT+MFT+MFT+MFT+
  case (FILENUMBER(FILEPERIF_ELMFTREGISTER2021)):
  {
    int item = (recordNumber/5);
    switch(registerNumber)
    {
    case 2:
      (*editValue) = (uint32_t*)&edition_settings.timeout_pause_df[item];
      if(regUst<TIMEOUT_DF_PAUSE_MIN/10 || regUst>TIMEOUT_DF_PAUSE_MAX/10) diapazon=0;
      break;
    case 3:
      (*editValue) = (uint32_t*)&edition_settings.timeout_work_df[item];
      if(regUst<TIMEOUT_DF_WORK_MIN/10 || regUst>TIMEOUT_DF_WORK_MAX/10) diapazon=0;
      break;
    }//switch
  }//FILENUMBER(FILEPERIF_MFTREGISTER2021
  break;
////MFT-MFT-MFT-MFT-MFT-MFT-MFT-MFT-MFT-MFT-MFT-MFT-MFT-MFT-MFT-MFT-MFT-MFT-MFT-MFT-
////UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+
  case (FILENUMBER(FILEPROT_UPREGISTER2021)):
  {
    int item = recordNumber/100;
    switch(recordNumber%100)
    {
    case 6://6 106 206 306 406 506 606 706
      (*editValue) = (uint32_t*)&edition_settings.timeout_UP[item][grupa_ustavok];
      if(regUst<TIMEOUT_UP_MIN/10 || regUst>TIMEOUT_UP_MAX/10) diapazon=0;
      break;
    case 7://7 107 207 307 407 507 607 707
      (*multer) = 1;
      (*editValue) = (uint32_t*)&edition_settings.setpoint_UP_KP[item][grupa_ustavok];

      if(edition_settings.control_UP& (1<<(2+3*item)))//(1<<(2+3*item)))//more_less_control
      {
        if(regUst<SETPOINT_UP_KP_LESS_MIN || regUst>SETPOINT_UP_KP_LESS_MAX) diapazon=0;
      }//if(edition_settings.control_UP& (1<<(2+3*item)))//more_less_control
      else  if(regUst<SETPOINT_UP_KP_MORE_MIN || regUst>SETPOINT_UP_KP_MORE_MAX) diapazon=0;
      break;
    }//switch(recordNumber%100)
  }//FILENUMBER(FILEPROT_UPREGISTER2021
  break;
////UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-

  }//switch

  if((*editValue) == NULL)
    switch(fileRecordNumber)
    {
    case (FILENUMBER(FILEPROT_COMMREGISTER2021)+RECORDNUMBER(1)):
      (*editValue) = (uint32_t*)&edition_settings.grupa_ustavok;
      (*multer) = 1;
      if(regUst<1 || regUst>4) diapazon=0;
      break;

////ZNMIN+ZNMIN+ZNMIN+ZNMIN+ZNMIN+ZNMIN+ZNMIN+ZNMIN+ZNMIN+ZNMIN+
    case (FILENUMBER(FILEPROT_ZNMINREGISTER2021)+RECORDNUMBER(5)):
      (*multer) = 100;
      (*editValue) = (uint32_t*)&edition_settings.setpoint_Umin1[grupa_ustavok];
      if(regUst<SETPOINT_UMIN1_MIN/100 || regUst>SETPOINT_UMIN1_MAX/100) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_ZNMINREGISTER2021)+RECORDNUMBER(6)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_Umin1[grupa_ustavok];
      if(regUst<TIMEOUT_UMIN1_MIN/10 || regUst>TIMEOUT_UMIN1_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_ZNMINREGISTER2021)+RECORDNUMBER(105)):
      (*multer) = 100;
      (*editValue) = (uint32_t*)&edition_settings.setpoint_Umin2[grupa_ustavok];
      if(regUst<SETPOINT_UMIN2_MIN/100 || regUst>SETPOINT_UMIN2_MAX/100) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_ZNMINREGISTER2021)+RECORDNUMBER(106)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_Umin2[grupa_ustavok];
      if(regUst<TIMEOUT_UMIN2_MIN/10 || regUst>TIMEOUT_UMIN2_MAX/10) diapazon=0;
      break;
////ZNMIN-ZNMIN-ZNMIN-ZNMIN-ZNMIN-ZNMIN-ZNMIN-ZNMIN-ZNMIN-ZNMIN-
////ZNMAX+ZNMAX+ZNMAX+ZNMAX+ZNMAX+ZNMAX+ZNMAX+ZNMAX+ZNMAX+ZNMAX+
    case (FILENUMBER(FILEPROT_ZNMAXREGISTER2021)+RECORDNUMBER(5)):
      (*multer) = 100;
      (*editValue) = (uint32_t*)&edition_settings.setpoint_Umax1[grupa_ustavok];
      if(regUst<SETPOINT_UMAX1_MIN/100 || regUst>SETPOINT_UMAX1_MAX/100) diapazon=0;
      break;
/*      
    case (FILENUMBER(FILEPROT_ZNMAXREGISTER2021)+RECORDNUMBER(6)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_Umax1[grupa_ustavok];
      if(regUst<TIMEOUT_UMAX1_MIN/10 || regUst>TIMEOUT_UMAX1_MAX/10) diapazon=0;
      break;
*/
    case (FILENUMBER(FILEPROT_ZNMAXREGISTER2021)+RECORDNUMBER(105)):
      (*multer) = 100;
      (*editValue) = (uint32_t*)&edition_settings.setpoint_Umax2[grupa_ustavok];
      if(regUst<SETPOINT_UMAX2_MIN/100 || regUst>SETPOINT_UMAX2_MAX/100) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_ZNMAXREGISTER2021)+RECORDNUMBER(106)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_Umax2[grupa_ustavok];
      if(regUst<TIMEOUT_UMAX2_MIN/10 || regUst>TIMEOUT_UMAX2_MAX/10) diapazon=0;
      break;
////ZNMAX-ZNMAX-ZNMAX-ZNMAX-ZNMAX-ZNMAX-ZNMAX-ZNMAX-ZNMAX-ZNMAX-
////TRANS+TRANS+TRANS+TRANS+TRANS+TRANS+TRANS+TRANS+TRANS+TRANS+TRANS+TRANS+TRANS+
    case (FILENUMBER(FILEPERIF_TRANSREGISTER2021)+RECORDNUMBER(1)):
      (*multer) = 1;
      (*editValue) = (uint32_t*)&edition_settings.TVoltage1;
      if(regUst<KOEF_TN_MIN || regUst>KOEF_TN_MAX) diapazon=0;
      break;
    case (FILENUMBER(FILEPERIF_TRANSREGISTER2021)+RECORDNUMBER(2)):
      (*multer) = 1;
      (*editValue) = (uint32_t*)&edition_settings.TCurrent1;
      if(regUst<KOEF_TT_MIN || regUst>KOEF_TT_MAX) diapazon=0;
      break;
    case (FILENUMBER(FILEPERIF_TRANSREGISTER2021)+RECORDNUMBER(4)):
      (*multer) = 1;
      (*editValue) = (uint32_t*)&edition_settings.TVoltage2;
      if(regUst<KOEF_TN_MIN || regUst>KOEF_TN_MAX) diapazon=0;
      break;
    case (FILENUMBER(FILEPERIF_TRANSREGISTER2021)+RECORDNUMBER(5)):
      (*multer) = 1;
      (*editValue) = (uint32_t*)&edition_settings.TCurrent2;
      if(regUst<KOEF_TT_MIN || regUst>KOEF_TT_MAX) diapazon=0;
      break;
////TRANS-TRANS-TRANS-TRANS-TRANS-TRANS-TRANS-TRANS-TRANS-TRANS-TRANS-TRANS-TRANS-
////RPN+RPN+RPN+RPN+RPN+RPN+RPN+RPN+RPN+RPN+RPN+RPN+RPN+RPN+RPN+RPN+RPN+RPN+RPN+
    case (FILENUMBER(FILEPERIF_RPNREGISTER2021)+RECORDNUMBER(7)):
      (*multer) = 1;
      (*editValue) = (uint32_t*)&edition_settings.setpoint_rpn_zony[grupa_ustavok];
      if(regUst<SETPOINT_RPN_ZONY_MIN || regUst>SETPOINT_RPN_ZONY_MAX) diapazon=0;
      break;
    case (FILENUMBER(FILEPERIF_RPNREGISTER2021)+RECORDNUMBER(8)):
      (*multer) = 1;
      (*editValue) = (uint32_t*)&edition_settings.setpoint_rpn_per[grupa_ustavok];
      if(regUst<SETPOINT_RPN_PER_MIN || regUst>SETPOINT_RPN_PER_MAX) diapazon=0;
      break;
    case (FILENUMBER(FILEPERIF_RPNREGISTER2021)+RECORDNUMBER(9)):
      (*multer) = 1;
      (*editValue) = (uint32_t*)&edition_settings.setpoint_rpn_K[grupa_ustavok];
      if(regUst<SETPOINT_RPN_K_MIN || regUst>SETPOINT_RPN_K_MAX) diapazon=0;
      break;
    case (FILENUMBER(FILEPERIF_RPNREGISTER2021)+RECORDNUMBER(10)):
      (*multer) = 1000;
      (*editValue) = (uint32_t*)&edition_settings.timeout_rpn_ub_pryb[grupa_ustavok];
      if(regUst<TIMEOUT_RPN_UB_PRYB_MIN/1000 || regUst>TIMEOUT_RPN_UB_PRYB_MAX/1000) diapazon=0;
      break;
    case (FILENUMBER(FILEPERIF_RPNREGISTER2021)+RECORDNUMBER(11)):
      (*multer) = 1000;
      (*editValue) = (uint32_t*)&edition_settings.timeout_rpn_ub_pryb_prysk[grupa_ustavok];
      if(regUst<TIMEOUT_RPN_UB_PRYB_PRYSK_MIN/1000 || regUst>TIMEOUT_RPN_UB_PRYB_PRYSK_MAX/1000) diapazon=0;
      break;
    case (FILENUMBER(FILEPERIF_RPNREGISTER2021)+RECORDNUMBER(12)):
      (*multer) = 100;
      (*editValue) = (uint32_t*)&edition_settings.timeout_rpn_prysk_vid_Umax[grupa_ustavok];
      if(regUst<TIMEOUT_RPN_UB_PRYSK_VID_UMAX_MIN/100 || regUst>TIMEOUT_RPN_UB_PRYSK_VID_UMAX_MAX/100) diapazon=0;
      break;
    case (FILENUMBER(FILEPERIF_RPNREGISTER2021)+RECORDNUMBER(13)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_rpn_perekl[grupa_ustavok];
      if(regUst<TIMEOUT_RPN_PEREKL_MIN/10 || regUst>TIMEOUT_RPN_PEREKL_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPERIF_RPNREGISTER2021)+RECORDNUMBER(14)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_rpn_nespr[grupa_ustavok];
      if(regUst<TIMEOUT_RPN_NESPR_MIN/10 || regUst>TIMEOUT_RPN_NESPR_MAX/10) diapazon=0;
      break;

    case (FILENUMBER(FILEPERIF_RPNREGISTER2021)+RECORDNUMBER(20)):
      (*multer) = 1;
      (*editValue) = (uint32_t*)&edition_settings.type_control_location;
      if(regUst<0 || regUst>4) diapazon=0;
      break;
    case (FILENUMBER(FILEPERIF_RPNREGISTER2021)+RECORDNUMBER(21)):
      (*multer) = 1;
      (*editValue) = (uint32_t*)&edition_settings.number_steps_rpn;
      if(regUst<NUMBER_STEPS_RPN_MIN || regUst>NUMBER_STEPS_RPN_MAX) diapazon=0;
      break;
    case (FILENUMBER(FILEPERIF_RPNREGISTER2021)+RECORDNUMBER(22)):
      (*multer) = 1;
      (*editValue) = (uint32_t*)&edition_settings.number_steps_rpn;
      if(regUst<NUMBER_STEPS_RPN_MIN || regUst>NUMBER_STEPS_RPN_MAX) diapazon=0;
      break;
    case (FILENUMBER(FILEPERIF_RPNREGISTER2021)+RECORDNUMBER(23)):
      (*multer) = 1;
      (*editValue) = (uint32_t*)&edition_settings.number_steps_rpn;
      if(regUst<NUMBER_STEPS_RPN_MIN || regUst>NUMBER_STEPS_RPN_MAX) diapazon=0;
      break;
////RPN-RPN-RPN-RPN-RPN-RPN-RPN-RPN-RPN-RPN-RPN-RPN-RPN-RPN-RPN-RPN-RPN-RPN-RPN-RPN-
////ZNX+ZNX+ZNX+ZNX+ZNX+ZNX+ZNX+ZNX+ZNX+ZNX+ZNX+ZNX+ZNX+ZNX+ZNX+ZNX+ZNX+ZNX+ZNX+
    case (FILENUMBER(FILEPERIF_ZNXREGISTER2021)+RECORDNUMBER(5)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_znkh_perekl[grupa_ustavok];
      if(regUst<TIMEOUT_ZNKh_PEREKL_MIN/10 || regUst>TIMEOUT_ZNKh_PEREKL_MAX/10) diapazon=0;
      break;
////ZNX-ZNX-ZNX-ZNX-ZNX-ZNX-ZNX-ZNX-ZNX-ZNX-ZNX-ZNX-ZNX-ZNX-ZNX-ZNX-ZNX-ZNX-ZNX-
////BRP+BRP+BRP+BRP+BRP+BRP+BRP+BRP+BRP+BRP+BRP+BRP+BRP+BRP+BRP+BRP+BRP+BRP+BRP+
    case (FILENUMBER(FILEPERIF_BRPREGISTER2021)+RECORDNUMBER(5)):
      (*multer) = 100;
      (*editValue) = (uint32_t*)&edition_settings.setpoint_brp_I_blk[grupa_ustavok];
      if(regUst<SETPOINT_BRP_I_BLK_MIN/100 || regUst>SETPOINT_BRP_I_BLK_MAX/100) diapazon=0;
      break;
////BRP-BRP-BRP-BRP-BRP-BRP-BRP-BRP-BRP-BRP-BRP-BRP-BRP-BRP-BRP-BRP-BRP-BRP-BRP-
////PKV1+PKV1+PKV1+PKV1+PKV1+PKV1+PKV1+PKV1+PKV1+PKV1+PKV1+PKV1+PKV1+PKV1+PKV1+
    case (FILENUMBER(FILEPERIF_PKV1REGISTER2021)+RECORDNUMBER(1)):
      (*multer) = 1;
      //Язык пользовательского интерфейса
      (*editValue) = (uint32_t *)&edition_settings.language;
      if(regUst<VALUE_SETTING_LANGUAGE_MIN || regUst>VALUE_SETTING_LANGUAGE_MAX) diapazon=0;
      break;
////PKV1-PKV1-PKV1-PKV1-PKV1-PKV1-PKV1-PKV1-PKV1-PKV1-PKV1-PKV1-PKV1-PKV1-PKV1-
////PKV2+PKV2+PKV2+PKV2+PKV2+PKV2+PKV2+PKV2+PKV2+PKV2+PKV2+PKV2+PKV2+PKV2+
    case (FILENUMBER(FILEPERIF_PKV2REGISTER2021)+RECORDNUMBER(2)):
      (*multer) = 1;
      switch(pointInterface)//Время активации пароля после простоя
      {
      case USB_RECUEST:
        (*editValue) = &edition_settings.timeout_deactivation_password_interface_USB;
        break;
      case RS485_RECUEST:
        (*editValue) = &edition_settings.timeout_deactivation_password_interface_RS485;
        break;
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
      case LAN_RECUEST:
        (*editValue) = &edition_settings.timeout_deactivation_password_interface_LAN;
        break;
#endif
      }//switch
      break;
    case (FILENUMBER(FILEPERIF_PKV2REGISTER2021)+RECORDNUMBER(3)):
      (*multer) = 1;
      //Тайм-аут применения изменений
      (*editValue) = &edition_settings.timeout_idle_new_settings;
      if(regUst<TIMEOUT_NEW_SETTINGS_MIN || regUst>TIMEOUT_NEW_SETTINGS_MAX) diapazon=0;
      break;
////PKV2-PKV2-PKV2-PKV2-PKV2-PKV2-PKV2-PKV2-PKV2-PKV2-PKV2-PKV2-PKV2-PKV2-
////PKV4+PKV4+PKV4+PKV4+PKV4+PKV4+PKV4+PKV4+PKV4+PKV4+PKV4+PKV4+PKV4+PKV4+
    case (FILENUMBER(FILEPERIF_PKV4REGISTER2021)+RECORDNUMBER(2)):
      switch(registerNumber)
      {
      case 1:
        //Скорость порта связи
        (*multer) = 1;
        (*editValue) = (uint32_t *)&edition_settings.speed_RS485;
        if(regUst<VALUE_SPEED_INTERFACE_MIN || regUst>VALUE_SPEED_INTERFACE_MAX) diapazon=0;
        break;
      case 3://Паритет
        (*multer) = 1;
        (*editValue) = (uint32_t *)&edition_settings.pare_bit_RS485;
        if(regUst<VALUE_PARE_INTERFACE_MIN || regUst>VALUE_PARE_INTERFACE_MAX) diapazon=0;
        break;
      case 4://Количество стоп-бит
        (*multer) = 1;
        (*editValue) = (uint32_t *)&edition_settings.number_stop_bit_RS485;
        if(regUst<1 || regUst>MAX_NUMBER_STOP_BITS_INTERFACE) diapazon=0;
        break;
      }//switch
      break;
    case (FILENUMBER(FILEPERIF_PKV4REGISTER2021)+RECORDNUMBER(3)):
      //Задержка приёма
      (*multer) = 1;
      (*editValue) = &edition_settings.time_out_1_RS485;
      if(regUst<VALUE_TIME_OUT_1_INTERFACE_MIN || regUst>VALUE_TIME_OUT_1_INTERFACE_MAX) diapazon=0;
      break;
////PKV4-PKV4-PKV4-PKV4-PKV4-PKV4-PKV4-PKV4-PKV4-PKV4-PKV4-PKV4-PKV4-PKV4-PKV4-
////PKV6+PKV6+PKV6+PKV6+PKV6+PKV6+PKV6+PKV6+PKV6+PKV6+PKV6+PKV6+PKV6+PKV6+PKV6+
    case (FILENUMBER(FILEPERIF_PKV6REGISTER2021)+RECORDNUMBER(2)):
      //Адрес устройства в сети
      (*multer) = 1;
      (*editValue) = &edition_settings.address;
      if(regUst<KOEF_ADDRESS_MIN || regUst>KOEF_ADDRESS_MAX) diapazon=0;
      break;
////PKV6-PKV6-PKV6-PKV6-PKV6-PKV6-PKV6-PKV6-PKV6-PKV6-PKV6-PKV6-PKV6-PKV6-PKV6-
////REGANL+REGANL+REGANL+REGANL+REGANL+REGANL+REGANL+REGANL+REGANL+REGANL+REGANL+
    case (FILENUMBER(FILEPERIF_REGANLREGISTER2021)+RECORDNUMBER(1)):
      switch(registerNumber)
      {
      case 1:
        (*multer) = 1;
        //Время записи аналогового регистратора (доаварийный массив)
        //В таблицю настройок записуємо не мілісекунди, а кількість періодів
        (*editValue) = (uint32_t*)&edition_settings.prefault_number_periods;
        if(regUst<TIMEOUT_PREFAULT_MIN/20 || regUst>TIMEOUT_PREFAULT_MAX/20)  diapazon=0;
        break;
      case 2:
        (*multer) = 1;
        //Время записи аналогового регистратора (послеаварый массив)
        //В таблицю настройок записуємо не мілісекунди, а кількість періодів
        (*editValue) = (uint32_t*)&edition_settings.postfault_number_periods;
        if(regUst<TIMEOUT_POSTFAULT_MIN/20 || regUst>TIMEOUT_POSTFAULT_MAX/20)  diapazon=0;
        break;
      }//switch
      break;
////REGANL-REGANL-REGANL-REGANL-REGANL-REGANL-REGANL-REGANL-REGANL-REGANL-REGANL-
////REGDIS+REGDIS+REGDIS+REGDIS+REGDIS+REGDIS+REGDIS+REGDIS+REGDIS+REGDIS+REGDIS+
    case (FILENUMBER(FILEPERIF_REGDISREGISTER2021)+RECORDNUMBER(1)):
      //Выдержка доаварийного массива DR
      (*editValue) = (uint32_t*)&edition_settings.timeout_prolongation_work_digital_registrator;
      if(regUst<TIMEOUT_DR_ELONGATION_MIN/10 || regUst>TIMEOUT_DR_ELONGATION_MAX/10) diapazon=0;
      break;
////REGDIS-REGDIS-REGDIS-REGDIS-REGDIS-REGDIS-REGDIS-REGDIS-REGDIS-REGDIS-REGDIS-
    }//switch

  return diapazon;
}//ustFunc000for2021




