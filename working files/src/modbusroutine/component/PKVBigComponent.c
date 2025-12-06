#include "header.h"

//начальный регистр в карте памяти
#define BEGIN_ADR_REGISTER 12000
//конечный регистр в карте памяти
#define END_ADR_REGISTER 12055

int privatePKVBigGetReg2(int adrReg);

int getPKVBigModbusRegister(int);//получить содержимое регистра
int getPKVBigModbusBit(int);//получить содержимое бита
int setPKVBigModbusRegister(int, int);//получить содержимое регистра
int setPKVBigModbusBit(int, int);//получить содержимое бита

int  postPKVBigWriteAction(void);//action после записи
int PKVFunc000(int inOffset, int regPKV, uint32_t **editValue);
int passwordImunitetRegPKVBigComponent(int x);

SRAM1 COMPONENT_OBJ *pkvbigcomponent;
int PKVFunc000(int inOffset, int regPKV, uint32_t **editValue)
{
  int diapazon = 1;

  switch(inOffset)
  {
  case 0://Время активации пароля после простоя
    switch(pointInterface)//метка интерфейса 0-USB 1-RS485
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
    if(regPKV<TIMEOUT_DEACTIVATION_PASSWORD_MIN || regPKV>TIMEOUT_DEACTIVATION_PASSWORD_MAX) diapazon=0;
    break;

#define IMUNITET_PKV1 1
  case IMUNITET_PKV1://Проверка/установка пароля
    switch(pointInterface)//метка интерфейса 0-USB 1-RS485
    {
     case USB_RECUEST:
      (*editValue) = &edition_settings.password_interface_USB;
     break;
     case RS485_RECUEST:
      (*editValue) = &edition_settings.password_interface_RS485;
     break;
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
     case LAN_RECUEST:
      (*editValue) = &edition_settings.password_interface_LAN;
     break;
#endif
    }//switch
    break;

  case 2://Тайм-аут применения изменений
    (*editValue) = &edition_settings.timeout_idle_new_settings;
    if(regPKV<TIMEOUT_NEW_SETTINGS_MIN || regPKV>TIMEOUT_NEW_SETTINGS_MAX) diapazon=0;
    break;

  case 3://Язык пользовательского интерфейса
    (*editValue) = (uint32_t *)&edition_settings.language;
    if(regPKV<VALUE_SETTING_LANGUAGE_MIN || regPKV>VALUE_SETTING_LANGUAGE_MAX) diapazon=0;
    break;

  case 5://Скорость порта связи
    (*editValue) = (uint32_t *)&edition_settings.speed_RS485;
    if(regPKV<VALUE_SPEED_INTERFACE_MIN || regPKV>VALUE_SPEED_INTERFACE_MAX) diapazon=0;
    break;

  case 6://Количество стоп-бит
    (*editValue) = (uint32_t *)&edition_settings.number_stop_bit_RS485;
    if(regPKV<1 || regPKV>MAX_NUMBER_STOP_BITS_INTERFACE) diapazon=0;
    break;

  case 7://Паритет
    (*editValue) = (uint32_t *)&edition_settings.pare_bit_RS485;
    if(regPKV<VALUE_PARE_INTERFACE_MIN || regPKV>VALUE_PARE_INTERFACE_MAX) diapazon=0;
    break;

  case 8://Задержка приёма
    (*editValue) = &edition_settings.time_out_1_RS485;
    if(regPKV<VALUE_TIME_OUT_1_INTERFACE_MIN || regPKV>VALUE_TIME_OUT_1_INTERFACE_MAX) diapazon=0;
    break;

//    case 9://Скорость порта связи 2
//    case 10://Количество стоп-бит 2
//    case 11://Паритет 2
//    case 12://Задержка приёма 2
//      (*editValue) = &stubnull;
//      break;

  case 13://Адрес устройства в сети
    (*editValue) = &edition_settings.address;
    if(regPKV<KOEF_ADDRESS_MIN || regPKV>KOEF_ADDRESS_MAX) diapazon=0;
    break;

  case 38://Часовой пояс
    (*editValue) = (uint32_t*)&edition_settings.time_zone;
    if(regPKV>64000 && regPKV<65536+(TIME_ZONE_MIN*60)) diapazon=0;
    if(regPKV<64000 && regPKV>(TIME_ZONE_MAX*60)) diapazon=0;
    break;
  }//switch

  return diapazon;
} //PKVFunc000(int inOffset, int regPKV, uint32_t **editValue)

/**************************************/
//подготовка компонента PKV
/**************************************/
void constructorPKVBigComponent(COMPONENT_OBJ *pkvbigcomp)
{
  pkvbigcomponent = pkvbigcomp;

  pkvbigcomponent->getModbusRegister = getPKVBigModbusRegister;//получить содержимое регистра
  pkvbigcomponent->getModbusBit      = getPKVBigModbusBit;//получить содержимое бита
  pkvbigcomponent->setModbusRegister = setPKVBigModbusRegister;//получить содержимое регистра
  pkvbigcomponent->setModbusBit      = setPKVBigModbusBit;//получить содержимое бита

  pkvbigcomponent->postWriteAction = postPKVBigWriteAction;//action после записи
}//prepareDVinConfig

int getPKVBigModbusRegister(int adrReg)
{
  //получить содержимое регистра
  if(privatePKVBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(pkvbigcomponent->isActiveActualData)
  {
    edition_settings = current_settings_interfaces;//делаем копию
  }//if(uprbigcomponent->isActiveActualData)
  superClearActiveActualData();

  time_t time_dat_tmp;
  int32_t time_ms_tmp;
  if (save_time_dat_l == 3) 
  {
    time_dat_tmp = time_dat_save_l;
    time_ms_tmp = time_ms_save_l;
  }
  else
  { 
    copying_time_dat = 1;
    time_dat_tmp = time_dat_copy;
    time_ms_tmp = time_ms_copy;
    copying_time_dat = 0;
  }
  //struct tm *p;
  //struct tm *p = localtime(&time_dat_tmp);
//time_dat_tmp = 1581934622;

  uint32_t *editValue=NULL;
  PKVFunc000(adrReg-BEGIN_ADR_REGISTER, 0, &editValue);
  int offset = adrReg-BEGIN_ADR_REGISTER;

  switch(offset)
  {
  case 1://Проверка/установка пароля
    return MARKER_ERRORPERIMETR;

  case 6://Количество стоп-бит
    return (((unsigned short)*editValue)+1) &0xFFFF;

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
    case 16://IP адрес устройства
     return edition_settings.IP4[0]&0xff | (((edition_settings.IP4[1]&0xff)<<8)&0xFF00);
    case 17://IP адрес устройства
     return edition_settings.IP4[2]&0xff | (((edition_settings.IP4[3]&0xff)<<8)&0xFF00);
    case 18://Маска подсети
     return edition_settings.mask&0xff;
    case 19://Шлюз по-умолчанию
     return edition_settings.gateway[0]&0xff | (((edition_settings.gateway[1]&0xff)<<8)&0xFF00);
    case 20://Шлюз по-умолчанию
     return edition_settings.gateway[2]&0xff | (((edition_settings.gateway[3]&0xff)<<8)&0xFF00);
#endif

  case 23://Символ 1 и 2
    return (edition_settings.name_of_cell[0]&0xFF) | ((edition_settings.name_of_cell[1]<<8)&0xFF00);
  case 24://Символ 3 и 4
    return (edition_settings.name_of_cell[2]&0xFF) | ((edition_settings.name_of_cell[3]<<8)&0xFF00);
  case 25://Символ 5 и 6
    return (edition_settings.name_of_cell[4]&0xFF) | ((edition_settings.name_of_cell[5]<<8)&0xFF00);
  case 26://Символ 7 и 8
    return (edition_settings.name_of_cell[6]&0xFF) | ((edition_settings.name_of_cell[7]<<8)&0xFF00);
  case 27://Символ 9 и 10
    return (edition_settings.name_of_cell[8]&0xFF) | ((edition_settings.name_of_cell[9]<<8)&0xFF00);
  case 28://Символ 11 и 12
    return (edition_settings.name_of_cell[10]&0xFF) | ((edition_settings.name_of_cell[11]<<8)&0xFF00);
  case 29://Символ 13 и 14
    return (edition_settings.name_of_cell[12]&0xFF) | ((edition_settings.name_of_cell[13]<<8)&0xFF00);
  case 30://Символ 15 и 16
    return (edition_settings.name_of_cell[14]&0xFF) | ((edition_settings.name_of_cell[15]<<8)&0xFF00);

  case 31:
  case 32:
  case 33:
  case 34:
    {
    uint16_t* tmp[4];
    tmp[0] = &((uint16_t*)&time_dat_tmp)[0];
    tmp[1] = &((uint16_t*)&time_dat_tmp)[1];
    tmp[2] = &((uint16_t*)&time_dat_tmp)[2];
    tmp[3] = &((uint16_t*)&time_dat_tmp)[3];
    return (*(tmp[offset-31]));
    }
  case 35:
  case 36:
    {
    uint16_t* tmp[2];
    tmp[0] = &((uint16_t*)&time_ms_tmp)[0];
    tmp[1] = &((uint16_t*)&time_ms_tmp)[1];
    return *(tmp[offset-35]);
    }

  case 38://Часовой пояс
    return ((*editValue) &0xFFFF)*60;

  case 39://Переход на Зимнее/Летнее время
    return (edition_settings.dst & MASKA_FOR_BIT(N_BIT_TZ_DST)) ? 1 : 0;

  case 40://Месяц перехода на Летнее время
    return (edition_settings.dst_on_rule>>POS_MM)&((int)(pow(2,SHIFT_MM)-1));
  case 41://Неделя месяца перехода на Летнее время
    return (edition_settings.dst_on_rule>>POS_DOW)&((int)(pow(2,SHIFT_DOW)-1));
  case 42://День недели перехода на Летнее время
    return (edition_settings.dst_on_rule>>POS_WR)&((int)(pow(2,SHIFT_WR)-1));
  case 43://Час недели перехода на Летнее время
    return (edition_settings.dst_on_rule>>POS_HH)&((int)(pow(2,SHIFT_HH)-1));
  case 44://Месяц перехода на Зимнее время
    return (edition_settings.dst_off_rule>>POS_MM)&((int)(pow(2,SHIFT_MM)-1));
  case 45://Неделя месяца перехода на Зимнее время
    return (edition_settings.dst_off_rule>>POS_DOW)&((int)(pow(2,SHIFT_DOW)-1));
  case 46://День недели перехода на Зимнее время
    return (edition_settings.dst_off_rule>>POS_WR)&((int)(pow(2,SHIFT_WR)-1));
  case 47://Час недели перехода на Зимнее время
    return (edition_settings.dst_off_rule>>POS_HH)&((int)(pow(2,SHIFT_HH)-1));

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
  case 51://IP адрес сервера NTP 1
     return edition_settings.IP_time_server[0]&0xff | (((edition_settings.IP_time_server[1]&0xff)<<8)&0xFF00);
  case 52://IP адрес сервера NTP 1
     return edition_settings.IP_time_server[2]&0xff | (((edition_settings.IP_time_server[3]&0xff)<<8)&0xFF00);

  case 55://Интервал опроса NTP сервера
    return edition_settings.period_sync&0xFFFF;
#endif
  }//switch

  if(editValue==NULL) return 0;
  return (((unsigned short)*editValue)) &0xFFFF;
}//getDOUTBigModbusRegister(int adrReg)
int getPKVBigModbusBit(int x)
{
  //получить содержимое регистра
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setPKVBigModbusRegister(int adrReg, int dataReg)
{
  //записать содержимое регистра
  if(privatePKVBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(pkvbigcomponent->isActiveActualData)
  {
    edition_settings = current_settings_interfaces;//делаем копию
  }//if(uprbigcomponent->isActiveActualData)
  superClearActiveActualData();

  superSetOperativMarker(pkvbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//записать в буфер

  uint32_t *editValue=NULL;
  if(!PKVFunc000(adrReg-BEGIN_ADR_REGISTER, dataReg, &editValue)) return MARKER_ERRORDIAPAZON;

  return 0;
}//getDOUTBigModbusRegister(int adrReg)
int setPKVBigModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //получить содержимое регистра
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

int postPKVBigWriteAction(void)
{
  extern int upravlSetting;//флаг Setting
//action после записи
  int beginAdr = pkvbigcomponent->operativMarker[0];
  if(beginAdr<0) return 0;//не было записи
  int endAdr   = pkvbigcomponent->operativMarker[1];
  int offsetTempWriteArray = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//найти смещение TempWriteArray
  int countAdr = endAdr-beginAdr+1;
  if(endAdr<0) countAdr = 1;//к-во регистров
  int flag_time_array = 0;
  int flag_ms_array = 0;

//  struct tm *p;
  //запрещенные начальные адреса пакета
  switch(beginAdr)
  {
   case 12032: case 12033: case 12034: case 12036: return ERROR_VALID2;
  }//switch
  //запрещенные конечные адреса пакета
  switch(endAdr)
  {
   case 12031: case 12032: case 12033: case 12035: return ERROR_VALID2;
  }//switch
  unsigned short time_dat_frac[4];//фракции Время (для 64-бит)
  unsigned short time_ms_frac[2];//фракции Время, микросекунды


  for(int i=0; i<countAdr; i++)
  {
    int offset = i+beginAdr-BEGIN_ADR_REGISTER;
    uint32_t *editValue=NULL;
    PKVFunc000(offset, 0, &editValue);
    unsigned short offsetWriteRegister = tempWriteArray[offsetTempWriteArray+i];//новое значение регистра
    switch(offset)  //индекс регистра
    {
    case 0://Время активации пароля после простоя
    case 2://Тайм-аут применения изменений
    case 3://Язык пользовательского интерфейса
    case 5://Скорость порта связи
    case 7://Паритет
    case 8://Задержка приёма
    case 13://Адрес устройства в сети
      *editValue = offsetWriteRegister;
      upravlSetting = 1;//флаг Setting
      break;

    case 1://Проверка/установка пароля
      {
      int passwordS=-1;
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
      unsigned short new_parol = offsetWriteRegister;

      if(passwordS==1) //пароль не снят
      {
        //режим check parol
        if((*editValue) == new_parol)
        {
          //обнулить флажок пароля
          switch(pointInterface)//метка интерфейса 0-USB 1-RS485
          {
           case USB_RECUEST:
            password_set_USB=0;//Пароль установлен
           break;
           case RS485_RECUEST:
            password_set_RS485=0;//Пароль установлен
           break;
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
           case LAN_RECUEST:
            password_set_LAN=0;//Пароль установлен
           break;
#endif
          }//switch
        }//if
        else return ERROR_VALID2;//неправильный пароль
        break;
      }//if(passwordS==1) //пароль не снят
      else
      {//пароль снят
        //режим check parol
        if((*editValue) == new_parol)
        {
          if(new_parol==0) break;//повторная запись нулевого пароля не активирует флаг
          //активир флажок пароля
          switch(pointInterface)//метка интерфейса 0-USB 1-RS485
          {
           case USB_RECUEST:
            password_set_USB=1;//Пароль установлен
           break;
           case RS485_RECUEST:
            password_set_RS485=1;//Пароль установлен
           break;
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
           case LAN_RECUEST:
            password_set_LAN=1;//Пароль установлен
           break;
#endif
          }//switch
        }//if
      }//else
      //записать новый пароль
      *editValue = offsetWriteRegister;
      upravlSetting = 1;//флаг Setting
      } break;

    case 6://Количество стоп-бит
      *editValue = offsetWriteRegister-1;
      upravlSetting = 1;//флаг Setting
      break;

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
    case 16://IP адрес устройства
     edition_settings.IP4[0] = offsetWriteRegister & 0xff;
     edition_settings.IP4[1] = (offsetWriteRegister>>8) & 0xff;
     upravlSetting = 1;//флаг Setting
    break;
    case 17://IP адрес устройства
     edition_settings.IP4[2] = offsetWriteRegister & 0xff;
     edition_settings.IP4[3] = (offsetWriteRegister>>8) & 0xff;
     upravlSetting = 1;//флаг Setting
    break;

    case 18://Маска подсети
     edition_settings.mask = offsetWriteRegister & 0xff;
     upravlSetting = 1;//флаг Setting
    break;

    case 19://Шлюз по-умолчанию
     edition_settings.gateway[0] = offsetWriteRegister & 0xff;
     edition_settings.gateway[1] = (offsetWriteRegister>>8) & 0xff;
     upravlSetting = 1;//флаг Setting
    break;
    case 20://Шлюз по-умолчанию
     edition_settings.gateway[2] = offsetWriteRegister & 0xff;
     edition_settings.gateway[3] = (offsetWriteRegister>>8) & 0xff;
     upravlSetting = 1;//флаг Setting
    break;
#endif
    case 23://Символ 1 и 2
      edition_settings.name_of_cell[0] = offsetWriteRegister;
      edition_settings.name_of_cell[1] = offsetWriteRegister>>8;
      upravlSetting = 1;//флаг Setting
      break;
    case 24://Символ 3 и 4
      edition_settings.name_of_cell[2] = offsetWriteRegister;
      edition_settings.name_of_cell[3] = offsetWriteRegister>>8;
      upravlSetting = 1;//флаг Setting
      break;
    case 25://Символ 5 и 6
      edition_settings.name_of_cell[4] = offsetWriteRegister;
      edition_settings.name_of_cell[5] = offsetWriteRegister>>8;
      upravlSetting = 1;//флаг Setting
      break;
    case 26://Символ 7 и 8
      edition_settings.name_of_cell[6] = offsetWriteRegister;
      edition_settings.name_of_cell[7] = offsetWriteRegister>>8;
      upravlSetting = 1;//флаг Setting
      break;
    case 27://Символ 9 и 10
      edition_settings.name_of_cell[8] = offsetWriteRegister;
      edition_settings.name_of_cell[9] = offsetWriteRegister>>8;
      upravlSetting = 1;//флаг Setting
      break;
    case 28://Символ 11 и 12
      edition_settings.name_of_cell[10] = offsetWriteRegister;
      edition_settings.name_of_cell[11] = offsetWriteRegister>>8;
      upravlSetting = 1;//флаг Setting
      break;
    case 29://Символ 13 и 14
      edition_settings.name_of_cell[12] = offsetWriteRegister;
      edition_settings.name_of_cell[13] = offsetWriteRegister>>8;
      upravlSetting = 1;//флаг Setting
      break;
    case 30://Символ 15 и 16
      edition_settings.name_of_cell[14] = offsetWriteRegister;
      edition_settings.name_of_cell[15] = offsetWriteRegister>>8;
      upravlSetting = 1;//флаг Setting
      break;

    case 31://Время (для 64-бит)
    case 32://Время (для 64-бит)
    case 33://Время (для 64-бит)
    case 34://Время (для 64-бит)
      if(countAdr == 1) return ERROR_VALID2;//к-во регистров
      time_dat_frac[offset-31] = offsetWriteRegister;//фракции Время (для 64-бит)
      flag_time_array = 1;
      break;
    case 35://Время, микросекунды
    case 36://Время, микросекунды
      if(countAdr == 1) return ERROR_VALID2;//к-во регистров
      time_ms_frac[offset-35] = offsetWriteRegister;//фракции Время, микросекунды
      flag_ms_array = 1;
      break;

    case 38://Часовой пояс
      {
      int value = (uint16_t)offsetWriteRegister;
//      if((value%60) >0) return ERROR_VALID2;//проверка кратность 60
      int temp = value;
      if(value>64000) temp = value - 65536;
      if((temp%60) >0) return ERROR_VALID2;//проверка кратность 60

      *editValue = temp/60;
      upravlSetting = 1;//флаг Setting
      }
      break;

   case 39://Переход на Зимнее/Летнее время
     if(offsetWriteRegister>1) return ERROR_VALID2;
     edition_settings.dst &= (uint32_t)(~(1<<N_BIT_TZ_DST));
     edition_settings.dst |= (offsetWriteRegister<<N_BIT_TZ_DST);
     upravlSetting = 1;//флаг Setting
     break;
 
   case 40://Месяц перехода на Летнее время
     if(offsetWriteRegister>DST_RULE_MM_MAX) return ERROR_VALID2;
     if(offsetWriteRegister<DST_RULE_MM_MIN) return ERROR_VALID2;
     edition_settings.dst_on_rule &= (uint32_t)(~(((1 << SHIFT_MM) - 1)<<POS_MM));
     edition_settings.dst_on_rule |= (offsetWriteRegister<<POS_MM);
     upravlSetting = 1;//флаг Setting
     break;
   case 41://Неделя месяца перехода на Летнее время
     if(offsetWriteRegister>DST_RULE_DOW_MAX) return ERROR_VALID2;
//     if(offsetWriteRegister<DST_RULE_DOW_MIN) return ERROR_VALID2;
     edition_settings.dst_on_rule &= (uint32_t)(~(((1 << SHIFT_DOW) - 1)<<POS_DOW));
     edition_settings.dst_on_rule |= (offsetWriteRegister<<POS_DOW);
     upravlSetting = 1;//флаг Setting
     break;
   case 42://Номер дня недели перехода  на Летнее время
     if(offsetWriteRegister>DST_RULE_WR_MAX) return ERROR_VALID2;
     if(offsetWriteRegister<DST_RULE_WR_MIN) return ERROR_VALID2;
     edition_settings.dst_on_rule &= (uint32_t)(~(((1 << SHIFT_WR) - 1)<<POS_WR));
     edition_settings.dst_on_rule |= (offsetWriteRegister<<POS_WR);
     upravlSetting = 1;//флаг Setting
     break;
   case 43://Час недели перехода на Летнее время
     if(offsetWriteRegister>DST_RULE_HH_MAX) return ERROR_VALID2;
     //if(offsetWriteRegister<DST_RULE_HH_MIN) return ERROR_VALID2;
     edition_settings.dst_on_rule &= (uint32_t)(~(((1 << SHIFT_HH) - 1)<<POS_HH));
     edition_settings.dst_on_rule |= (offsetWriteRegister<<POS_HH);
     upravlSetting = 1;//флаг Setting
     break;
   case 44://Месяц перехода на Зимнее время
     if(offsetWriteRegister>DST_RULE_MM_MAX) return ERROR_VALID2;
     if(offsetWriteRegister<DST_RULE_MM_MIN) return ERROR_VALID2;
     edition_settings.dst_off_rule &= (uint32_t)(~(((1 << SHIFT_MM) - 1)<<POS_MM));
     edition_settings.dst_off_rule |= (offsetWriteRegister<<POS_MM);
     upravlSetting = 1;//флаг Setting
     break;
   case 45://Неделя месяца перехода на Зимнее время
     if(offsetWriteRegister>DST_RULE_DOW_MAX) return ERROR_VALID2;
//     if(offsetWriteRegister<DST_RULE_DOW_MIN) return ERROR_VALID2;
     edition_settings.dst_off_rule &= (uint32_t)(~(((1 << SHIFT_DOW) - 1)<<POS_DOW));
     edition_settings.dst_off_rule |= (offsetWriteRegister<<POS_DOW);
     upravlSetting = 1;//флаг Setting
     break;
   case 46://Номер дня недели перехода на Стандартное время
     if(offsetWriteRegister>DST_RULE_WR_MAX) return ERROR_VALID2;
     if(offsetWriteRegister<DST_RULE_WR_MIN) return ERROR_VALID2;
     edition_settings.dst_off_rule &= (uint32_t)(~(((1 << SHIFT_WR) - 1)<<POS_WR));
     edition_settings.dst_off_rule |= (offsetWriteRegister<<POS_WR);
     upravlSetting = 1;//флаг Setting
     break;
   case 47://Час недели перехода на Зимнее время
     if(offsetWriteRegister>DST_RULE_HH_MAX) return ERROR_VALID2;
     //if(offsetWriteRegister<DST_RULE_HH_MIN) return ERROR_VALID2;
     edition_settings.dst_off_rule &= (uint32_t)(~(((1 << SHIFT_HH) - 1)<<POS_HH));
     edition_settings.dst_off_rule |= (offsetWriteRegister<<POS_HH);
     upravlSetting = 1;//флаг Setting
     break;
 
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
   case 51://IP адрес сервера NTP 1
     edition_settings.IP_time_server[0] = offsetWriteRegister & 0xff;
     edition_settings.IP_time_server[1] = (offsetWriteRegister>>8) & 0xff;
     upravlSetting = 1;//флаг Setting
     break;
   case 52://IP адрес сервера NTP 1
     edition_settings.IP_time_server[2] = offsetWriteRegister & 0xff;
     edition_settings.IP_time_server[3] = (offsetWriteRegister>>8) & 0xff;
     upravlSetting = 1;//флаг Setting
     break;
 
   case 55://Интервал опроса NTP сервера
//     if(offsetWriteRegister==0) return ERROR_VALID2;
     edition_settings.period_sync = offsetWriteRegister;
     upravlSetting = 1;//флаг Setting
     break;
#endif

    }//switch
  }//for

  //Сборка новых значений времени
  copying_time_dat = 1;
  time_t time_dat_tmp = time_dat_copy;//time UNIX
  uint32_t time_ms_tmp = time_ms_copy;//milisec
  copying_time_dat = 0;
   uint16_t* tmp1 = &((uint16_t*)&time_dat_tmp)[0];
   uint16_t* tmp2 = &((uint16_t*)&time_dat_tmp)[1];
   uint16_t* tmp3 = &((uint16_t*)&time_dat_tmp)[2];
   uint16_t* tmp4 = &((uint16_t*)&time_dat_tmp)[3];
   uint16_t* tmp5 = &((uint16_t*)&time_ms_tmp)[0];
   uint16_t* tmp6 = &((uint16_t*)&time_ms_tmp)[1];
  if(flag_time_array)
  {
  (*tmp1) = time_dat_frac[0];
  (*tmp2) = time_dat_frac[1];
  (*tmp3) = time_dat_frac[2];
  (*tmp4) = time_dat_frac[3];
  }//if
  if(flag_ms_array)
  {
  (*tmp5) = time_ms_frac[0];
  (*tmp6) = time_ms_frac[1];
  if(time_ms_tmp>1000) return ERROR_VALID2;
  }//if

  if(flag_time_array || flag_ms_array)
  {
    time_dat_save_l = time_dat_tmp;
    time_ms_save_l = time_ms_tmp;
    save_time_dat_l = 3;
  }//if(flag_time_array || flag_ms_array)
  return 0;
}//

int privatePKVBigGetReg2(int adrReg)
{
  //проверить внешний периметр
  return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)

int passwordImunitetRegPKVBigComponent(int adrReg)
{
  //имунитетные к паролю адреса регистров 0 - есть имунитет
  if(privatePKVBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  switch(adrReg)
  {
  case BEGIN_ADR_REGISTER+IMUNITET_PKV1://Проверка/установка пароля
    return 0;//есть имунитет
  }//switch
  return MARKER_OUTPERIMETR;
}//passwordImunitetRegPKVBigComponent(int adrReg)
