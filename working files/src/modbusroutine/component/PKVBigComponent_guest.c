//начальный регистр в карте памяти
#define BEGIN_ADR_REGISTER 3000

#include "header.h"
#include "variables_external_m.h"

int privatePKVBigGetReg2(int adrReg);
int getPKVBigModbusRegister_guest(int adrReg);
int postPKVBigWriteAction(void);
int postPKVBigWriteAction_guest(int countAdr, int beginAdr);
int passwordImunitetRegPKVBigComponent(int x);
int PKVFunc000(int inOffset, int regPKV, uint32_t **editValue);

int PKVFunc000(int inOffset, int regPKV, uint32_t **editValue)
{
  int diapazon = 1;

  switch(inOffset)
  {
#define IMUNITET_PKV1 6
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

#define IMUNITET_REG10 10
  case IMUNITET_REG10://Текущий аналоговый регистратор
  {
    if (
      ((clean_rejestrators & CLEAN_AR) != 0) ||
      (
        (state_ar_record_fatfs != STATE_AR_NONE_FATFS) &&
        (state_ar_record_fatfs != STATE_AR_MEMORY_FULL_FATFS) &&
        (state_ar_record_fatfs != STATE_AR_BLOCK_FATFS)
      )
    )
    {
      diapazon = 1;
      break;
    }//if

    unsigned int first_number = (info_rejestrator_ar.first_number < 0) ? 0 : (info_rejestrator_ar.first_number + 1);
    unsigned int last_number  = (info_rejestrator_ar.last_number  < 0) ? 0 : (info_rejestrator_ar.last_number + 1);

    unsigned int number_records;
    if (first_number == 0) number_records = 0;
    else if (first_number >= last_number) number_records = first_number - last_number + 1;
    else number_records = NUMBER_FATFS_NAME - last_number + first_number + 1;

    if(! ((unsigned int)regPKV < number_records))
    {
      diapazon = 1;
      break;
    }//if

  }
  break;

#define IMUNITET_REG11 11
  case IMUNITET_REG11://Текущий дискретный регистратор
    if (
      ((clean_rejestrators & CLEAN_DR) != 0) ||
      (
        ((pointInterface == USB_RECUEST  ) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_DR_USB  ) != 0)) ||
        ((pointInterface == RS485_RECUEST  ) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_DR_RS485) != 0))
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
        ||
        ((pointInterface == LAN_RECUEST  ) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_DR_LAN) != 0))
#endif
      )
    )
    {
      diapazon = 1;
      break;
    }//if

    if(! ((unsigned int)regPKV < info_rejestrator_dr.number_records))
    {
      diapazon = 1;
      break;
    }//if

    if(regPKV > MAX_NUMBER_RECORDS_INTO_DR)
    {
      diapazon = 1;
      break;
    }//if

    break;

//Дискретність аналогового реєстратора
#define IMUNITET_REG12 12

  }//switch

  return diapazon;
} //PKVFunc000(int inOffset, int regPKV, uint32_t **editValue)

int getPKVBigModbusRegister_guest(int adrReg)
{
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

  int offset = adrReg-BEGIN_ADR_REGISTER;

  switch(offset)
  {
  case 0:
  case 1:
  case 2:
  case 3:
  {
    uint16_t* tmp[4];
    tmp[0] = &((uint16_t*)&time_dat_tmp)[0];
    tmp[1] = &((uint16_t*)&time_dat_tmp)[1];
    tmp[2] = &((uint16_t*)&time_dat_tmp)[2];
    tmp[3] = &((uint16_t*)&time_dat_tmp)[3];
    return (*(tmp[offset-0]));
  }
  case 4:
  case 5:
  {
    uint16_t* tmp[2];
    tmp[0] = &((uint16_t*)&time_ms_tmp)[0];
    tmp[1] = &((uint16_t*)&time_ms_tmp)[1];
    return *(tmp[offset-4]);
  }

  case IMUNITET_PKV1://Проверка/установка пароля
    return MARKER_ERRORPERIMETR;

  case IMUNITET_REG10://Текущий аналоговый регистратор
    if(pointInterface==USB_RECUEST)//метка интерфейса 0-USB 1-RS485 2-LAN
      return (number_record_of_ar_for_USB) &0xFFFF;
    else if(pointInterface==RS485_RECUEST)
      return (number_record_of_ar_for_RS485) &0xFFFF;
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
    else if(pointInterface==LAN_RECUEST)
      return (number_record_of_ar_for_LAN) &0xFFFF;
#endif
    //Теоретично сюди ніколи не мала б програма зайти
    else total_error_sw_fixed();

  case IMUNITET_REG11://Текущий дискретный регистратор
    if(pointInterface==USB_RECUEST)//метка интерфейса 0-USB 1-RS485
      return (number_record_of_dr_for_USB) &0xFFFF;
    else if(pointInterface==RS485_RECUEST)
      return (number_record_of_dr_for_RS485) &0xFFFF;
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
    else if(pointInterface==LAN_RECUEST)
      return (number_record_of_dr_for_LAN) &0xFFFF;
#endif
    //Теоретично сюди ніколи не мала б програма зайти
    else total_error_sw_fixed();

  case IMUNITET_REG12://Дискретність аналогового реєстратора
    edition_settings = current_settings_interfaces;//изменения
    return edition_settings.diskretnt_number_periods / 20;

  }//switch

  return 0;
}//getPKVBigModbusRegister_guest(int adrReg)

int postPKVBigWriteAction_guest(int countAdr, int beginAdr)
{
  extern int upravlSetting;//флаг Setting
  int offsetTempWriteArray = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//найти смещение TempWriteArray

  int flag_time_array = 0;
  int flag_ms_array = 0;

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
    case 0://Время (для 64-бит)
    case 1://Время (для 64-бит)
    case 2://Время (для 64-бит)
    case 3://Время (для 64-бит)
      if(countAdr == 1) return ERROR_VALID2;//к-во регистров
      time_dat_frac[offset-0] = offsetWriteRegister;//фракции Время (для 64-бит)
      flag_time_array = 1;
      break;
    case 4://Время, микросекунды
    case 5://Время, микросекунды
      if(countAdr == 1) return ERROR_VALID2;//к-во регистров
      time_ms_frac[offset-4] = offsetWriteRegister;//фракции Время, микросекунды
      flag_ms_array = 1;
      break;

    case IMUNITET_PKV1://Проверка/установка пароля
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
      { //пароль снят
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
    }
    break;

    case IMUNITET_REG10://Текущий аналоговый регистратор
    {
      int *point_to_number_record_of_ar = NULL;
      int *point_to_first_number_time_sample= NULL, *point_to_last_number_time_sample = NULL;
      char *point_id_ar_record = NULL;
      int *point_to_max_number_time_sample = NULL;
      unsigned char *point_to_Buffer = NULL;

      if (pointInterface == USB_RECUEST)
      {
        point_to_number_record_of_ar = &number_record_of_ar_for_USB;
        point_id_ar_record = id_ar_record_for_USB;
        point_to_first_number_time_sample = &first_number_time_sample_for_USB;
        point_to_last_number_time_sample = &last_number_time_sample_for_USB;
        point_to_max_number_time_sample = &max_number_time_sample_USB;

        point_to_Buffer = buffer_for_USB_read_record_ar;
      }
      else if (pointInterface == RS485_RECUEST)
      {
        point_to_number_record_of_ar = &number_record_of_ar_for_RS485;
        point_id_ar_record = id_ar_record_for_RS485;
        point_to_first_number_time_sample = &first_number_time_sample_for_RS485;
        point_to_last_number_time_sample = &last_number_time_sample_for_RS485;
        point_to_max_number_time_sample = &max_number_time_sample_RS485;

        point_to_Buffer = buffer_for_RS485_read_record_ar;
      }
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
      else if (pointInterface == LAN_RECUEST)
      {
        point_to_number_record_of_ar = &number_record_of_ar_for_LAN;
        point_id_ar_record = id_ar_record_for_LAN;
        point_to_first_number_time_sample = &first_number_time_sample_for_LAN;
        point_to_last_number_time_sample = &last_number_time_sample_for_LAN;
        point_to_max_number_time_sample = &max_number_time_sample_LAN;

        point_to_Buffer = buffer_for_LAN_read_record_ar;
      }
#endif
      else
      {
        //Теоретично цього ніколи не мало б бути
        total_error_sw_fixed();
      }

      unsigned short int data = tempWriteArray[offsetTempWriteArray+i];

      int record_name = info_rejestrator_ar.first_number - data;
      if (record_name < 0) record_name += NUMBER_FATFS_NAME;

      int n = snprintf(point_id_ar_record, (8 + 1), "%d", record_name);
      if (n > 8) n = 8; //Теоретично цього ніколи б не мало бути
      point_id_ar_record[n++] = '.';
      point_id_ar_record[n++] = 'd';
      point_id_ar_record[n++] = 'a';
      point_id_ar_record[n++] = 't';
      point_id_ar_record[n++] = '\0';

      unsigned int error = 0;
      FIL fil;
      FRESULT res = f_open(&fil, point_id_ar_record, FA_READ);
      if (res == FR_OK)
      {
        res = f_lseek(&fil, 0);
        if (res == FR_OK)
        {
          //Виставляємо читання заголовку запису даного запису і дальше, скільки можливо, часових зрізів
          *point_to_first_number_time_sample = -1;
          int last_number_time_sample_tmp = (SIZE_PAGE_DATAFLASH_2 - sizeof(__HEADER_AR))/(AR_TOTAL_NUMBER_CANALES*sizeof(short int));
          int max_number_time_sample = *point_to_max_number_time_sample = (f_size(&fil) - sizeof(__HEADER_AR))/(AR_TOTAL_NUMBER_CANALES*sizeof(short int));

          if (max_number_time_sample >= 0)
          {
            if (last_number_time_sample_tmp <= max_number_time_sample)
            {
              *point_to_last_number_time_sample = last_number_time_sample_tmp - 1;//номер останнього часового зрізу ВКЛЮЧНО
            }
            else
            {
              *point_to_last_number_time_sample = max_number_time_sample - 1;
            }

            unsigned int read_tmp;
            UINT ByteToRead = sizeof(__HEADER_AR) + ((unsigned int)(*point_to_last_number_time_sample) + 1)*AR_TOTAL_NUMBER_CANALES*sizeof(short int);
            res = f_read(&fil, point_to_Buffer, ByteToRead, &read_tmp);
            if ((res != FR_OK) || (read_tmp != ByteToRead)) error = ERROR_SLAVE_DEVICE_FAILURE;
          }
          else error = ERROR_SLAVE_DEVICE_FAILURE;
        }
        else error = ERROR_SLAVE_DEVICE_FAILURE;

        res = f_close(&fil);
        if (res != FR_OK)
        {
          //Невизначена ситуація, якої ніколи б не мало бути.
          _SET_BIT(set_diagnostyka, ERROR_AR_UNDEFINED_BIT);


          
          error = ERROR_SLAVE_DEVICE_FAILURE;
        }
      }
      else error = ERROR_SLAVE_DEVICE_FAILURE;

      if (error == 0)
      {
        //Встановлюємо номер запису аналогового реєстратора для читання
        *point_to_number_record_of_ar = data;
      }
      else
      {
        //Помічаємо, що немає вибраної аварії для читання
        *point_to_number_record_of_ar = -1;
        point_id_ar_record[0] = '\0';
        return ERROR_VALID2;
      }
    }
    break;//case IMUNITET_REG10

    case IMUNITET_REG11://Текущий дискретный регистратор

      if(pointInterface==USB_RECUEST)//метка интерфейса 0-USB 1-RS485
      {
        number_record_of_dr_for_USB = tempWriteArray[offsetTempWriteArray+i];
        //Подаємо команду читання дискретного реєстратора для інтерфейсу USB

        //Виставляємо першу частину запису
        part_reading_dr_from_dataflash_for_USB = 0;
        //Подаємо команду зчитати дані у бувер пам'яті
        control_tasks_dataflash |= TASK_MAMORY_READ_DATAFLASH_FOR_DR_USB;
      }//if
      else if(pointInterface==RS485_RECUEST)
      {
        number_record_of_dr_for_RS485 = tempWriteArray[offsetTempWriteArray+i];
        //Подаємо команду читання дискретного реєстратора для інтерфейсу RS-485

        //Виставляємо першу частину запису
        part_reading_dr_from_dataflash_for_RS485 = 0;
        //Подаємо команду зчитати дані у бувер пам'яті
        control_tasks_dataflash |= TASK_MAMORY_READ_DATAFLASH_FOR_DR_RS485;
      }
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
      else if(pointInterface==LAN_RECUEST)
      {
        number_record_of_dr_for_LAN = tempWriteArray[offsetTempWriteArray+i];
        //Подаємо команду читання дискретного реєстратора для інтерфейсу LAN

        //Виставляємо першу частину запису
        part_reading_dr_from_dataflash_for_LAN = 0;
        //Подаємо команду зчитати дані у бувер пам'яті
        control_tasks_dataflash |= TASK_MAMORY_READ_DATAFLASH_FOR_DR_LAN;
      }
#endif
      else
      {
        //Теоретично цього ніколи не мало б бути
        total_error_sw_fixed();
      }

      break;//IMUNITET_REG11

  case IMUNITET_REG12://Дискретність аналогового реєстратора
    edition_settings.diskretnt_number_periods = tempWriteArray[offsetTempWriteArray+i] * 20; //Дискретність виборок (кількість періодів промислової частоти)
    if (edition_settings.diskretnt_number_periods >= TIMEOUT_DISKRETN_MIN)
    if (edition_settings.diskretnt_number_periods <= TIMEOUT_DISKRETN_MAX)
    {
    upravlSetting = 1;
    }//if
      break;//IMUNITET_REG12

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
}//postPKVBigWriteAction_guest(int countAdr, int beginAdr)

int passwordImunitetRegPKVBigComponent(int adrReg)
{
  //имунитетные к паролю адреса регистров 0 - есть имунитет
  if(privatePKVBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  switch(adrReg)
  {
  case BEGIN_ADR_REGISTER+IMUNITET_PKV1://Проверка/установка пароля
  case BEGIN_ADR_REGISTER+IMUNITET_REG10://Текущий аналоговый регистратор
  case BEGIN_ADR_REGISTER+IMUNITET_REG11://Текущий дискретный регистратор
  case BEGIN_ADR_REGISTER+IMUNITET_REG12://Дискретність аналогового реєстратора
    return 0;//есть имунитет
  }//switch
  return MARKER_OUTPERIMETR;
}//passwordImunitetRegPKVBigComponent(int adrReg)

