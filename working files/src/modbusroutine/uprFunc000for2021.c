
#include "header.h"
#include "variables_external_m.h"

#define CLRVALID_DATA  0

extern int upravlSetting;//флаг Setting
extern int upravlSchematic;//флаг Rang

int uprFunc000for2021(int actControl, int fileRecordNumber, int registerNumber, uint32_t *uprMaska, int validData, uint32_t **editControl);
int isValidCONFCondition(unsigned int confMaska);

/**************************************/
//База данных управления
/**************************************/

void uprFuncRead000for2021(int fileRecordNumber, int registerNumber, uint32_t *uprMaska, uint32_t **editControl)
{
  uprFunc000for2021(0, fileRecordNumber, registerNumber, uprMaska, CLRVALID_DATA, editControl);
}//uprFuncRead000for2021
void uprFuncValidWrite000for2021(int action, int fileRecordNumber, int registerNumber, uint32_t *uprMaska, uint32_t **editControl)
{
  uprFunc000for2021(action, fileRecordNumber, registerNumber, uprMaska, CLRVALID_DATA, editControl);
}//uprFuncValidWrite000for2021

int setUprRegister000for21(int fileRecordNumber, int registerNumber, int dataReg)
{
  uint32_t uprMaska = 0;
  uint32_t *editControl = NULL;
  if(!(dataReg == 0 || dataReg == 1)) return MARKER_ERRORDIAPAZON;
  if(dataReg) //валидация на включение
    if(!uprFunc000for2021(0, fileRecordNumber, registerNumber, &uprMaska, CLRVALID_DATA, &editControl)) return MARKER_ERRORDIAPAZON;
  return 0;
}//setUprRegister21

int setGroupUprRegister000for21(int fileRecordNumber, int registerNumber, int countGroupBit, int dataReg)
{
  uint32_t uprMaska = 0;
  uint32_t *editControl = NULL;

  if(dataReg&((uint32_t)(~((1<<countGroupBit)-1)))) return MARKER_ERRORDIAPAZON;

  for(int item=0; item<countGroupBit; item++)
  {
    if(dataReg&(1<<item)) //валидация на включение
      if(!uprFunc000for2021(0, fileRecordNumber, (item<<16)//registerFractNumber дробова частина номер регістру
                            +registerNumber, &uprMaska, CLRVALID_DATA, &editControl)) return MARKER_ERRORDIAPAZON;
  }//for

  return 0;
}//setGroupUprRegister21

int getUprRegister000for20(int fileRecordNumber, int registerNumber)
{
  uint32_t uprMaska = 0;
  uint32_t *editControl = NULL;
  uprFuncRead000for2021(fileRecordNumber, registerNumber, &uprMaska, &editControl);
  if(editControl != NULL) //читать значение
    if((*editControl) & (1<<uprMaska)) return 1;
  return 0;
}//getUprRegister20

int getGroupUprRegister000for20(int fileRecordNumber, int registerNumber, int countGroupBit)
{
  unsigned short tempReadShort = 0;
  for(int item=0; item<countGroupBit; item++)
  {
    int value = 0;

    uint32_t uprMaska = 0;
    uint32_t *editControl = NULL;
    uprFuncRead000for2021(fileRecordNumber, (item<<16)//registerFractNumber дробова частина номер регістру
                          +registerNumber, &uprMaska, &editControl);
    if(editControl == NULL) value = 0;
    else
    {
      //читать значение
      if(uprMaska == 0xFFFFFFFF) value = 0;
      else
      {
      if(uprMaska >= 32) uprMaska -= 32;
      value = (*editControl)&(1<<uprMaska);
      }
    }//else

    int tsdata = 0;
    if(value) tsdata=1;
    tempReadShort |= tsdata<<item;
  }//for
  return tempReadShort;
}//getGroupUprRegister20

int postGroupUprRegister000for21(int fileRecordNumber, int registerNumber, int countGroupBit, unsigned int value)
{
  for(int item=0; item<countGroupBit; item++)
  {
    uint32_t uprMaska = 0;
    uint32_t *editControl = NULL;
    uprFuncValidWrite000for2021(0, fileRecordNumber, (item<<16)//registerFractNumber дробова частина номер регістру
                                +registerNumber, &uprMaska, &editControl);
    if(editControl == NULL) continue;
    if(uprMaska == 0xFFFFFFFF) continue;
    if(uprMaska >= 32) uprMaska -= 32;

    int temp = 0;
    if(value & (1<<item)) temp = 1;

    //установка изменений
    (*editControl) &= ~(1<<uprMaska);
    (*editControl) |= (temp<<uprMaska);

    //actControl
    if(temp)//валидация на включение
      uprFuncValidWrite000for2021(1, fileRecordNumber, (item<<16)//registerFractNumber дробова частина номер регістру
                                  +registerNumber, &uprMaska, &editControl);

    //ОСОБАЯ СБОРКА
    if(  (editControl == (uint32_t*)&edition_settings.control_extra_settings_1) ||
         (editControl == (uint32_t*)&edition_settings.configuration) ||
         (editControl == (uint32_t*)&edition_settings.control_UP )
      )
    {
      if(!value)//валидация на отключение
        if(!uprFunc000for2021(1,
                              fileRecordNumber, (item<<16)//registerFractNumber дробова частина номер регістру
                              +registerNumber,
                              &uprMaska, CLRVALID_DATA, &editControl)) return ERROR_VALID3;//ошибка валидации
      upravlSchematic = 1;//флаг Rang
    }//if
  }//for

  upravlSetting = 1;//флаг Setting
  return 0;
}//postGroupUprRegister21

int postUprRegister000for21(int fileRecordNumber, int registerNumber, unsigned int value)
{
  uint32_t uprMaska = 0;
  uint32_t *editControl = NULL;

  uprFuncValidWrite000for2021(0,//action
                              fileRecordNumber, registerNumber,
                              &uprMaska, &editControl);
  if(editControl == NULL) return 1;//continue;

  //установка изменений
  (*editControl) &= ~(1<<uprMaska);
  (*editControl) |= (value<<uprMaska);

  //actControl
  if(value)//валидация на включение
    if(!uprFunc000for2021(1, //action
                          fileRecordNumber, registerNumber,
                          &uprMaska, 1, &editControl)) return ERROR_VALID3;//ошибка валидации

  if(  (editControl == (uint32_t*)&edition_settings.control_extra_settings_1) ||
       (editControl == (uint32_t*)&edition_settings.configuration)      ||
       (editControl == (uint32_t*)&edition_settings.control_UP )
    )
  {
    if(!value)//валидация на отключение
      if(!uprFunc000for2021(1,
                            fileRecordNumber, registerNumber,
                            &uprMaska, CLRVALID_DATA, &editControl)) return ERROR_VALID3;//ошибка валидации
    upravlSchematic = 1;//флаг Rang
  }//if

  upravlSetting = 1;//флаг Setting
  return 0;
}//postUprRegister000for21

int isValidCONFCondition(unsigned int confMaska)
{
  if(!(edition_settings.configuration&(1<<confMaska))) return 1;
  return 0;
}//isValidCONFCondition(unsigned int confControl, unsigned int confMaska, uint32_t uprMaska, int validData)

int uprFunc000for2021(int actControl, int fileRecordNumber, int registerNumber, uint32_t *uprMaska, int validData, uint32_t **editControl)
{
//База даних управління
  UNUSED(validData);
  int fileNumber       = fileRecordNumber  & 0xFFFF0000;//номер файлу
  int recordNumber     = fileRecordNumber  & 0x0000FFFF;//номер запису
//  int registerWholeNumber = registerNumber & 0x0000FFFF;//ціла частина номер регістру
  int registerFractNumber = registerNumber & 0xFFFF0000;//дробова частина номер регістру

  int isValid = 1;
  (*uprMaska) = 0xFFFFFFFF;

  switch(fileNumber)
  {
  case (FILENUMBER(FILEUVV_DOUTREGISTER2021)):
    (*uprMaska)   = (recordNumber-1)/10;//idx_bit

    switch(registerNumber)
    {
    case 1:
      (*editControl) = (uint32_t*)&edition_settings.type_of_output;
      break;
    case 2:
      (*editControl) = (uint32_t*)&edition_settings.type_of_output_modif;
      break;
    }//switch(recordOffset)
    break;

  case (FILENUMBER(FILEUVV_DVREGISTER2021)):
    (*uprMaska)   = (recordNumber-1)/10;//idx_bit

    switch(registerNumber)
    {
    case 1:
      (*editControl) = (uint32_t*)&edition_settings.type_of_input_signal;
      break;
    case 2:
      (*editControl) = (uint32_t*)&edition_settings.type_of_input;
      break;
    }//switch(recordOffset)
    break;

  case (FILENUMBER(FILEUVV_SDIREGISTER2021)):
    (*uprMaska)   = (recordNumber-1)/10;//idx_bit

    switch(registerNumber)
    {
    case 1:
      (*editControl) = (uint32_t*)&edition_settings.type_of_led;
      break;
    }//switch(recordOffset)
    break;

#ifdef  MODYFIKACIA_VERSII_DS
  case (FILENUMBER(FILEUVV_DOUTDSREGISTER2021)):
    if(recordNumber != 0) break;
    (*uprMaska)   = DS_BIT_CONFIGURATION;//idx_bit

    switch(registerNumber)
    {
    case 1:
      (*editControl) = (uint32_t*)&edition_settings.configuration;

      if(actControl)
      {
        //Функція обновлення змінних при зміні конфігурації
        if(action_after_changing_of_configuration(edition_settings.configuration, &edition_settings)) isValid = 0;
      }//if(actControl)
      break;
    }//switch(recordOffset)
    break;
#endif
////UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+
  case (FILENUMBER(FILEPROT_UPREGISTER2021)):
  {
    if(recordNumber == 0) break;
    (*editControl) = &edition_settings.control_UP;
    if(isValidCONFCondition(UP_BIT_CONFIGURATION)) isValid = 0;
    (*uprMaska)   = (registerFractNumber>>16)+(recordNumber/100)*3;//maska;
    if(actControl)
    {
      //Функція обновлення коефіцієнта повернення після зміни налаштувань УЗ
      change_setpoint_to_ctrl_UP(&edition_settings, edition_settings.control_UP);

    }//if(actControl)
  }//FILENUMBER(FILEPROT_UPREGISTER2021)
  break;
////UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-
////MFT+MFT+MFT+MFT+MFT+MFT+MFT+MFT+MFT+MFT+MFT+MFT+MFT+MFT+MFT+MFT+MFT+MFT+MFT+MFT+
  case (FILENUMBER(FILEPERIF_ELMFTREGISTER2021)):
  {
    (*uprMaska)   = (recordNumber-1)/5;
    (*editControl) = &edition_settings.type_df;
  }//FILENUMBER(FILEPERIF_MFTREGISTER2021
  break;
////MFT-MFT-MFT-MFT-MFT-MFT-MFT-MFT-MFT-MFT-MFT-MFT-MFT-MFT-MFT-MFT-MFT-MFT-MFT-MFT-

  }//switch(fileOffset)

  if((*uprMaska)==0xFFFFFFFF)
    switch(fileRecordNumber)
    {
////ZNMIN+ZNMIN+ZNMIN+ZNMIN+ZNMIN+ZNMIN+ZNMIN+ZNMIN+ZNMIN+ZNMIN+ZNMIN+
    case (FILENUMBER(FILEPROT_ZNMINREGISTER2021)+RECORDNUMBER(0)):
      (*uprMaska)   = Umin_BIT_CONFIGURATION;
      (*editControl) = &edition_settings.configuration;
      if(actControl)
      {
        //Функція обновлення змінних при зміні конфігурації
        if(action_after_changing_of_configuration(edition_settings.configuration, &edition_settings)) isValid = 0;
      }//if(actControl)
      break;
    case (FILENUMBER(FILEPROT_ZNMINREGISTER2021)+RECORDNUMBER(1)):
      (*editControl) = &edition_settings.control_Umin;
      if(isValidCONFCondition(Umin_BIT_CONFIGURATION)) isValid = 0;
      switch(registerFractNumber)//дробова частина номер регістру
      {
      case 0x00000:
        (*uprMaska)   = INDEX_ML_CTRUmin_STAGE_1;
        break;
      }//switch(registerFractNumber)
      break;
    case (FILENUMBER(FILEPROT_ZNMINREGISTER2021)+RECORDNUMBER(101)):
      (*editControl) = &edition_settings.control_Umin;
      if(isValidCONFCondition(Umin_BIT_CONFIGURATION)) isValid = 0;
      switch(registerFractNumber)//дробова частина номер регістру
      {
      case 0x00000:
        (*uprMaska)   = INDEX_ML_CTRUmin_STAGE_2;
        break;
      }//switch(registerFractNumber)
      break;
////ZNMIN-ZNMIN-ZNMIN-ZNMIN-ZNMIN-ZNMIN-ZNMIN-ZNMIN-ZNMIN-ZNMIN-ZNMIN-ZNMIN-ZNMIN-
////ZNMAX+ZNMAX+ZNMAX+ZNMAX+ZNMAX+ZNMAX+ZNMAX+ZNMAX+ZNMAX+ZNMAX+ZNMAX+ZNMAX+ZNMAX+
    case (FILENUMBER(FILEPROT_ZNMAXREGISTER2021)+RECORDNUMBER(0)):
      (*uprMaska)   = Umax_BIT_CONFIGURATION;
      (*editControl) = &edition_settings.configuration;
      if(actControl)
      {
        //Функція обновлення змінних при зміні конфігурації
        if(action_after_changing_of_configuration(edition_settings.configuration, &edition_settings)) isValid = 0;
      }//if(actControl)
      break;
    case (FILENUMBER(FILEPROT_ZNMAXREGISTER2021)+RECORDNUMBER(1)):
      (*editControl) = &edition_settings.control_Umax;
      if(isValidCONFCondition(Umax_BIT_CONFIGURATION)) isValid = 0;
      switch(registerFractNumber)//дробова частина номер регістру
      {
      case 0x00000:
        (*uprMaska)   = INDEX_ML_CTRUmax_STAGE_1;
        break;
      }//switch(registerFractNumber)
      break;
    case (FILENUMBER(FILEPROT_ZNMAXREGISTER2021)+RECORDNUMBER(101)):
      (*editControl) = &edition_settings.control_Umax;
      if(isValidCONFCondition(Umax_BIT_CONFIGURATION)) isValid = 0;
      switch(registerFractNumber)//дробова частина номер регістру
      {
      case 0x00000:
        (*uprMaska)   = INDEX_ML_CTRUmax_STAGE_2;
        break;
      }//switch(registerFractNumber)
      break;
////ZNMAX-ZNMAX-ZNMAX-ZNMAX-ZNMAX-ZNMAX-ZNMAX-ZNMAX-ZNMAX-ZNMAX-ZNMAX-ZNMAX-
////RPN+RPN+RPN+RPN+RPN+RPN+RPN+RPN+RPN+RPN+RPN+RPN+RPN+RPN+RPN+RPN+RPN+RPN+RPN+
    case (FILENUMBER(FILEPERIF_RPNREGISTER2021)+RECORDNUMBER(0)):
      (*uprMaska)   = RPN_BIT_CONFIGURATION;
      (*editControl) = &edition_settings.configuration;
      if(actControl)
      {
        //Функція обновлення змінних при зміні конфігурації
        if(action_after_changing_of_configuration(edition_settings.configuration, &edition_settings)) isValid = 0;
      }//if(actControl)
      break;
    case (FILENUMBER(FILEPERIF_RPNREGISTER2021)+RECORDNUMBER(1)):
      (*editControl) = &edition_settings.control_rpn;
      if(isValidCONFCondition(RPN_BIT_CONFIGURATION)) isValid = 0;
      switch(registerFractNumber)//дробова частина номер регістру
      {
      case 0x00000:
        (*uprMaska)   = INDEX_ML_CTRRPN_STATE;
        break;
      case 0x10000:
        (*uprMaska)   = INDEX_ML_CTRRPN_TRANSF;
        break;
      case 0x20000:
        (*uprMaska)   = INDEX_ML_CTRRPN_OSNOVNYJ_TN2;
        break;

      case 0x40000:
        (*uprMaska)   = INDEX_ML_CTRRPN_STRUMOVA_COMP;
        break;
      case 0x50000:
        (*uprMaska)   = INDEX_ML_CTRRPN_DOD_KONTUR;
        break;
      case 0x60000:
        (*uprMaska)   = INDEX_ML_CTRRPN_PERMIT_TEST;
        break;
      case 0x70000:
        (*uprMaska)   = INDEX_ML_CTRRPN_TM_MODE;
        break;
      }//switch(registerFractNumber)
      break;
////RPN-RPN-RPN-RPN-RPN-RPN-RPN-RPN-RPN-RPN-RPN-RPN-RPN-RPN-RPN-RPN-RPN-RPN-RPN-RPN-
////ZNX+ZNX+ZNX+ZNX+ZNX+ZNX+ZNX+ZNX+ZNX+ZNX+ZNX+ZNX+ZNX+ZNX+ZNX+ZNX+ZNX+ZNX+ZNX+
    case (FILENUMBER(FILEPERIF_ZNXREGISTER2021)+RECORDNUMBER(0)):
      (*uprMaska)   = ZNKh_BIT_CONFIGURATION;
      (*editControl) = &edition_settings.configuration;
      if(actControl)
      {
        //Функція обновлення змінних при зміні конфігурації
        if(action_after_changing_of_configuration(edition_settings.configuration, &edition_settings)) isValid = 0;
      }//if(actControl)
      break;
    case (FILENUMBER(FILEPERIF_ZNXREGISTER2021)+RECORDNUMBER(1)):
      (*editControl) = &edition_settings.control_znkh;
      if(isValidCONFCondition(ZNKh_BIT_CONFIGURATION)) isValid = 0;
      switch(registerFractNumber)//дробова частина номер регістру
      {
      case 0x00000:
        (*uprMaska)   = INDEX_ML_CTRZNKh_STATE;
        break;
      }//switch(registerFractNumber)
      break;
////ZNX-ZNX-ZNX-ZNX-ZNX-ZNX-ZNX-ZNX-ZNX-ZNX-ZNX-ZNX-ZNX-ZNX-ZNX-ZNX-ZNX-ZNX-ZNX-
////ZSX+ZSX+ZSX+ZSX+ZSX+ZSX+ZSX+ZSX+ZSX+ZSX+ZSX+ZSX+ZSX+ZSX+ZSX+ZSX+ZSX+ZSX+ZSX+
    case (FILENUMBER(FILEPERIF_ZSXREGISTER2021)+RECORDNUMBER(0)):
      (*uprMaska)   = ZSKh_BIT_CONFIGURATION;
      (*editControl) = &edition_settings.configuration;
      if(actControl)
      {
        //Функція обновлення змінних при зміні конфігурації
        if(action_after_changing_of_configuration(edition_settings.configuration, &edition_settings)) isValid = 0;
      }//if(actControl)
      break;
    case (FILENUMBER(FILEPERIF_ZSXREGISTER2021)+RECORDNUMBER(1)):
      (*editControl) = &edition_settings.control_zskh;
      if(isValidCONFCondition(ZSKh_BIT_CONFIGURATION)) isValid = 0;
      switch(registerFractNumber)//дробова частина номер регістру
      {
      case 0x00000:
        (*uprMaska)   = INDEX_ML_CTRZSKh_STATE;
        break;
      }//switch(registerFractNumber)
      break;
////ZSX-ZSX-ZSX-ZSX-ZSX-ZSX-ZSX-ZSX-ZSX-ZSX-ZSX-ZSX-ZSX-ZSX-ZSX-ZSX-ZSX-ZSX-ZSX-
////BRP+BRP+BRP+BRP+BRP+BRP+BRP+BRP+BRP+BRP+BRP+BRP+BRP+BRP+BRP+BRP+BRP+BRP+BRP+
    case (FILENUMBER(FILEPERIF_BRPREGISTER2021)+RECORDNUMBER(0)):
      (*uprMaska)   = BRP_BIT_CONFIGURATION;
      (*editControl) = &edition_settings.configuration;
      if(actControl)
      {
        //Функція обновлення змінних при зміні конфігурації
        if(action_after_changing_of_configuration(edition_settings.configuration, &edition_settings)) isValid = 0;
      }//if(actControl)
      break;
    case (FILENUMBER(FILEPERIF_BRPREGISTER2021)+RECORDNUMBER(1)):
      (*editControl) = &edition_settings.control_brp;
      if(isValidCONFCondition(BRP_BIT_CONFIGURATION)) isValid = 0;
      switch(registerFractNumber)//дробова частина номер регістру
      {
      case 0x00000:
        (*uprMaska)   = INDEX_ML_CTRBRP_STATE;
        break;
      }//switch(registerFractNumber)
      break;
////BRP-BRP-BRP-BRP-BRP-BRP-BRP-BRP-BRP-BRP-BRP-BRP-BRP-BRP-BRP-BRP-BRP-BRP-BRP-
////UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+
    case (FILENUMBER(FILEPROT_UPREGISTER2021)+RECORDNUMBER(0)):
      (*uprMaska)   = UP_BIT_CONFIGURATION;
      (*editControl) = &edition_settings.configuration;
      if(actControl)
      {
        //Функція обновлення змінних при зміні конфігурації
        if(action_after_changing_of_configuration(edition_settings.configuration, &edition_settings)) isValid = 0;
      }//if(actControl)
      break;
////UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-
////ELAND+ELAND+ELAND+ELAND+ELAND+ELAND+ELAND+ELAND+ELAND+ELAND+ELAND+ELAND+ELAND+
    case (FILENUMBER(FILEPERIF_ELANDREGISTER2021)+RECORDNUMBER(0)):
      (*uprMaska)   = EL_BIT_CONFIGURATION;
      (*editControl) = &edition_settings.configuration;
      if(actControl)
      {
        if(action_after_changing_of_configuration(edition_settings.configuration, &edition_settings)) isValid = 0;
      }//if(actControl)
      break;
////ELAND-ELAND-ELAND-ELAND-ELAND-ELAND-ELAND-ELAND-ELAND-ELAND-ELAND-ELAND-ELAND-
////REGANL+REGANL+REGANL+REGANL+REGANL+REGANL+REGANL+REGANL+REGANL+REGANL+REGANL+
    case (FILENUMBER(FILEPERIF_REGANLREGISTER2021)+RECORDNUMBER(0)):
      (*uprMaska)   = INDEX_ML_CTR_AR_AVAR_STATE;
      (*editControl) = &edition_settings.control_ar;
      break;
////REGANL-REGANL-REGANL-REGANL-REGANL-REGANL-REGANL-REGANL-REGANL-REGANL-REGANL-

    }//switch(fileRecordNumber)

  return isValid;
}//uprFunc000for2021
