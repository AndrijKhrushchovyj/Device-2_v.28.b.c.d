//начальный регистр в карте памяти
#define BEGIN_ADR_REGISTER 13000
//конечный регистр в карте памяти
#define END_ADR_REGISTER 13369

// источники запуска регистратора
#define REGISTERS_REG 32
//сработавшие источники запуска регистратора
#define REGISTERS_ACTIONREG 32

#include "header.h"

int privateREGBigGetReg2(int adrReg);

int getREGBigModbusRegister(int);//получить содержимое регистра
int getREGBigModbusBit(int);//получить содержимое бита
int setREGBigModbusRegister(int, int);//получить содержимое регистра
int setREGBigModbusBit(int, int);//получить содержимое бита

int  postREGBigWriteAction(void);//action после записи
int passwordImunitetRegREGBigComponent(int adrReg);

SRAM1 COMPONENT_OBJ *regbigcomponent;

/**************************************/
//подготовка компонента REGistrator
/**************************************/
void constructorREGBigComponent(COMPONENT_OBJ *regbigcomp)
{
  regbigcomponent = regbigcomp;

  regbigcomponent->getModbusRegister = getREGBigModbusRegister;//получить содержимое регистра
  regbigcomponent->getModbusBit      = getREGBigModbusBit;//получить содержимое бита
  regbigcomponent->setModbusRegister = setREGBigModbusRegister;//получить содержимое регистра
  regbigcomponent->setModbusBit      = setREGBigModbusBit;//получить содержимое бита

  regbigcomponent->postWriteAction = postREGBigWriteAction;//action после записи
}//prepareDVinConfig

int getREGBigModbusRegister(int adrReg)
{
  //получить содержимое регистра
  if(privateREGBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  int offset = adrReg-BEGIN_ADR_REGISTER;
//Ранжирование источников запуска аналогового регистратора
  if(offset<32) return getRangN_BIGModbusRegister(&current_settings_interfaces.ranguvannja_analog_registrator[0],
                                                   REGISTERS_REG, offset );
//Сработавшие источники запуска текущего аналогового регистратора
  if(offset>=37&&offset<69) 
  {
   //отсекатель аналоговый
   if(
      (
       !(
         ((pointInterface == USB_RECUEST  ) && ((number_record_of_ar_for_USB   != -1) && (id_ar_record_for_USB[0]   != '\0'))) ||
         ((pointInterface == RS485_RECUEST) && ((number_record_of_ar_for_RS485 != -1) && (id_ar_record_for_RS485[0] != '\0')))
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
         ||
         ((pointInterface == LAN_RECUEST) && ((number_record_of_ar_for_LAN != -1) && (id_ar_record_for_LAN[0] != '\0')))
#endif
        )               
      )
      ||
      (
       (
        (state_ar_record_fatfs != STATE_AR_NONE_FATFS) &&
        (state_ar_record_fatfs != STATE_AR_MEMORY_FULL_FATFS) &&
        (state_ar_record_fatfs != STATE_AR_BLOCK_FATFS)
       )
       ||
       ((clean_rejestrators & CLEAN_AR) != 0)  
       ||  
       (
        (control_tasks_dataflash & (
                                    TASK_MAMORY_PART_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_FS |
                                    TASK_MAMORY_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_FS
                                   )
        ) != 0
       )
      )
     )
    {
      //Зараз іде операція запису/стирання для аналоговго реєстратора, яка може тривати довго (післяаварійний масив становить 20 с), тому читання аналогового реєстратора є тимчасово недоступне
      return MARKER_ERRORPERIMETR;
    }

      //Можна читати дані
      int *point_to_first_number_time_sample = NULL, *point_to_last_number_time_sample = NULL;
      char *point_id_ar_record = NULL;
      int *point_to_max_number_time_sample = NULL;
      unsigned char *point_to_buffer = NULL;

      if (pointInterface == USB_RECUEST)
      {
        point_id_ar_record = id_ar_record_for_USB;
        point_to_first_number_time_sample = &first_number_time_sample_for_USB;
        point_to_last_number_time_sample  = &last_number_time_sample_for_USB;
        point_to_max_number_time_sample = &max_number_time_sample_USB;
          
        point_to_buffer = buffer_for_USB_read_record_ar;
      }
      else if (pointInterface == RS485_RECUEST)
      {
        point_id_ar_record = id_ar_record_for_RS485;
        point_to_first_number_time_sample = &first_number_time_sample_for_RS485;
        point_to_last_number_time_sample  = &last_number_time_sample_for_RS485;
        point_to_max_number_time_sample = &max_number_time_sample_RS485;
        
        point_to_buffer = buffer_for_RS485_read_record_ar;
      }
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
      else if (pointInterface == LAN_RECUEST)
      {
        point_id_ar_record = id_ar_record_for_LAN;
        point_to_first_number_time_sample = &first_number_time_sample_for_LAN;
        point_to_last_number_time_sample  = &last_number_time_sample_for_LAN;
        point_to_max_number_time_sample = &max_number_time_sample_LAN;
        
        point_to_buffer = buffer_for_LAN_read_record_ar;
      }
#endif
      else
      {
        //Теоретично цього ніколи не мало б бути
        total_error_sw_fixed();
      }

      if ((*point_to_first_number_time_sample) != -1)
      {
        //Зараз не виконувалося зчитування заголовку аналогового реєстрата
              
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
              res = f_read(&fil, point_to_buffer, ByteToRead, &read_tmp);
              if ((res != FR_OK) || (read_tmp != ByteToRead)) return MARKER_ERRORPERIMETR;
            }
            else return MARKER_ERRORPERIMETR;
          }
          else return MARKER_ERRORPERIMETR;

          res = f_close(&fil);
          if (res != FR_OK) 
          {
            //Невизначена ситуація, якої ніколи б не мало бути.
            _SET_BIT(set_diagnostyka, ERROR_AR_UNDEFINED_BIT);
            return MARKER_ERRORPERIMETR;
          }
        }
        else return MARKER_ERRORPERIMETR;
      }

      //Якщо ми сюди дійшли, то вважаємо що заголовок аналогового реєстратора вже зчитаний
      __HEADER_AR *p_header_ar = (__HEADER_AR*)point_to_buffer;
      return getRangN_BIGModbusRegister(p_header_ar->cur_active_sources, REGISTERS_ACTIONREG, offset-37 );
  }//if(offset>=37&&offset<69) 

//Ранжирование источников запуска дискретного регистратора
  if(offset>=300&&offset<332)
      return getRangN_BIGModbusRegister(&current_settings_interfaces.ranguvannja_digital_registrator[0],
                                                               REGISTERS_REG, offset-300 );
//Сработавшие источники запуска текущего дискретного регистратора
  if(offset>=337&&offset<369) 
  {
   //отсекатель дискретный
          if (
            //Не подано попередньокоманди вичитування відповідного запису дискретного реєстратора
            //pointInterface=0 метка интерфейса 0-USB 1-RS485
            ((pointInterface == USB_RECUEST  ) && (number_record_of_dr_for_USB == 0xffff)) ||
            ((pointInterface == RS485_RECUEST) && (number_record_of_dr_for_RS485 == 0xffff))
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
            ||
            ((pointInterface == LAN_RECUEST) && (number_record_of_dr_for_LAN == 0xffff))
#endif
          ) return MARKER_ERRORPERIMETR;
         if (
              (
               ((pointInterface == USB_RECUEST  ) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_DR_USB  ) != 0)) ||
               ((pointInterface == RS485_RECUEST) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_DR_RS485) != 0))
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
            ||
               ((pointInterface == LAN_RECUEST) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_DR_LAN) != 0))
#endif
              ) 
              ||  
              ((clean_rejestrators & CLEAN_DR) != 0)
            )  
           
    {
     //Зараз іде зчитування для інтерфейсу запису дискретного реєстратора, тому ця операція є тимчасово недоступною
      return MARKER_ERRORPERIMETR;
    }

      //Можна читати дані
      unsigned char *point_to_buffer = NULL;
      if (pointInterface == USB_RECUEST) point_to_buffer = buffer_for_USB_read_record_dr;
      else if (pointInterface == RS485_RECUEST) point_to_buffer = buffer_for_RS485_read_record_dr;
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
      else if (pointInterface == LAN_RECUEST) point_to_buffer = buffer_for_LAN_read_record_dr;
#endif
      else
      {
        //Теоретично цього ніколи не мало б бути
        total_error_sw_fixed();
      }
      
      //Якщо ми сюди дійшли, то вважаємо що заголовок дискретного реєстратора вже зчитаний
      //Визначаємо, які сигнали запустили реєстратора
      unsigned int *activeNBIG_dr = (unsigned int*)(&point_to_buffer[FIRST_INDEX_SOURCE_DR+0]);//указатель на N_BIG массив активных команд запуска дискр
      return getRangN_BIGModbusRegister(activeNBIG_dr, REGISTERS_ACTIONREG, offset-337 );
  }//if(offset>=337&&offset<369) 

  switch(offset)
    {
    case 32://Время записи аналогового регистратора (доаварийный массив)
      return (current_settings_interfaces.prefault_number_periods) &0xFFFF; //В таблицю настройок записуємо не мілісекунди, а кількість періодів
    case 33://Время записи аналогового регистратора (послеаварый массив)
      return (current_settings_interfaces.postfault_number_periods) &0xFFFF; //В таблицю настройок записуємо не мілісекунди, а кількість періодів
    case 34://Количество аналоговых регистраторов
      {
        unsigned int first_number = (info_rejestrator_ar.first_number < 0) ? 0 : (info_rejestrator_ar.first_number + 1);
        unsigned int last_number  = (info_rejestrator_ar.last_number  < 0) ? 0 : (info_rejestrator_ar.last_number + 1);
      
        unsigned int number_records;
        if (first_number == 0) number_records = 0;
        else if (first_number >= last_number) number_records = first_number - last_number + 1;
        else number_records = NUMBER_FATFS_NAME - last_number + first_number + 1;
                
        return (number_records) & 0xFFFF;
      }
#define IMUNITET_REG35 35
    case IMUNITET_REG35://Текущий аналоговый регистратор
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
    case 36://Очистить аналоговый регистратор
      return MARKER_ERRORPERIMETR;

    case 333://Выдержка доаварийного массива DR
      return (current_settings_interfaces.timeout_prolongation_work_digital_registrator/10) &0xFFFF; 
    case 334://Количество дискретных регистраторов
      return (info_rejestrator_dr.number_records) &0xFFFF;
#define IMUNITET_REG335 335
    case IMUNITET_REG335://Текущий дискретный регистратор
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
    case 336://Очистить дискретный регистратор
      return MARKER_ERRORPERIMETR;
    }//switch

  return 0;
}//getDOUTBigModbusRegister(int adrReg)
int getREGBigModbusBit(int x)
{
  //получить содержимое регистра
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setREGBigModbusRegister(int adrReg, int dataReg)
{
  //записать содержимое регистра
  if(privateREGBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(regbigcomponent->isActiveActualData)
    {
      edition_settings = current_settings_interfaces;//делаем копию
    }//if(uprbigcomponent->isActiveActualData)
  superClearActiveActualData();

  superSetOperativMarker(regbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//записать в буфер

  int offset = adrReg-BEGIN_ADR_REGISTER;
//Ранжирование источников запуска аналогового регистратора
  if(offset<32) return validN_BIGACMD(dataReg);
//Сработавшие источники запуска текущего аналогового регистратора
  if(offset>=37&&offset<69) return MARKER_ERRORDIAPAZON;
//Ранжирование источников запуска аналогового регистратора
  if(offset>=300&&offset<332) return validN_BIGACMD(dataReg);
//Сработавшие источники запуска текущего дискретного регистратора
  if(offset>=337&&offset<369) return MARKER_ERRORDIAPAZON;

  switch(offset)
    {
    case 32://Время записи аналогового регистратора (доаварийный массив)
      if(dataReg<TIMEOUT_PREFAULT_MIN/20 || dataReg>TIMEOUT_PREFAULT_MAX/20) return MARKER_ERRORDIAPAZON;
      break;
    case 33://Время записи аналогового регистратора (послеаварый массив)
      if(dataReg<TIMEOUT_POSTFAULT_MIN/20 || dataReg>TIMEOUT_POSTFAULT_MAX/20) return MARKER_ERRORDIAPAZON;
      break;
    case 34://Количество аналоговых регистраторов
      return MARKER_ERRORDIAPAZON;
    case 35://Текущий аналоговый регистратор
      {
        if (
            ((clean_rejestrators & CLEAN_AR) != 0) ||
            (
             (state_ar_record_fatfs != STATE_AR_NONE_FATFS) &&
             (state_ar_record_fatfs != STATE_AR_MEMORY_FULL_FATFS) &&
             (state_ar_record_fatfs != STATE_AR_BLOCK_FATFS)
            )
          ) return MARKER_ERRORDIAPAZON;

        unsigned int first_number = (info_rejestrator_ar.first_number < 0) ? 0 : (info_rejestrator_ar.first_number + 1);
        unsigned int last_number  = (info_rejestrator_ar.last_number  < 0) ? 0 : (info_rejestrator_ar.last_number + 1);
                
        unsigned int number_records;
        if (first_number == 0) number_records = 0;
        else if (first_number >= last_number) number_records = first_number - last_number + 1;
        else number_records = NUMBER_FATFS_NAME - last_number + first_number + 1;
                
        if(! ((unsigned int)dataReg < number_records)) return MARKER_ERRORDIAPAZON;

      }
      break;
    case 36://Очистить аналоговый регистратор
      if(dataReg!=CMD_WORD_CLEAR_AR) return MARKER_ERRORDIAPAZON;
      break;
    case 333://Выдержка доаварийного массива DR
      if(dataReg<TIMEOUT_DR_ELONGATION_MIN/10 || dataReg>TIMEOUT_DR_ELONGATION_MAX/10) return MARKER_ERRORDIAPAZON;
      break;
    case 334://Количество дискретных регистраторов
      return MARKER_ERRORDIAPAZON;
    case 335://Текущий дискретный регистратор
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
      ) return MARKER_ERRORDIAPAZON;
      if(! ((unsigned int)dataReg < info_rejestrator_dr.number_records)) return MARKER_ERRORDIAPAZON;
      if(dataReg > MAX_NUMBER_RECORDS_INTO_DR) return MARKER_ERRORDIAPAZON;
      break;
    case 336://Очистить дискретный регистратор
      if(dataReg!=CMD_WORD_CLEAR_DR) return MARKER_ERRORDIAPAZON;
      break;
    }//switch

  return 0;
}//getDOUTBigModbusRegister(int adrReg)
int setREGBigModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //получить содержимое регистра
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

int postREGBigWriteAction(void)
{
  extern int upravlSchematic;//флаг Rang
  extern int upravlSetting;//флаг Setting
//action после записи
  int beginAdr = regbigcomponent->operativMarker[0];
  if(beginAdr<0) return 0;//не было записи
  int endAdr   = regbigcomponent->operativMarker[1];
  int countAdr = endAdr-beginAdr+1;
  if(endAdr<0) countAdr = 1;
  int offsetTempWriteArray = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//найти смещение TempWriteArray

  int flag1=1;
  int flag2=1;
  int countReg=countAdr;
  for(int i=0; i<countAdr; i++)
    {
      int offset = beginAdr-BEGIN_ADR_REGISTER+i;
      if(offset<32&&flag1)
        {
          writeRangN_BIGModbusRegister(&edition_settings.ranguvannja_analog_registrator[0], REGISTERS_REG, beginAdr,
                                       countReg, BEGIN_ADR_REGISTER);
          flag1=0;
          upravlSchematic = 1;//флаг Rang
        }//if

      if(offset>=300&&offset<332&&flag2)
        {
          writeRangN_BIGModbusRegister(&edition_settings.ranguvannja_digital_registrator[0], REGISTERS_REG, beginAdr,
                                       countReg, BEGIN_ADR_REGISTER+300);
          flag2=0;
          upravlSchematic = 1;//флаг Rang
        }//if
      switch(offset)
        {
        case 32://Время записи аналогового регистратора (доаварийный массив)
          edition_settings.prefault_number_periods = tempWriteArray[offsetTempWriteArray+i];//В таблицю настройок записуємо не мілісекунди, а кількість періодів
          upravlSetting = 1;//флаг Setting
          break;
        case 33://Время записи аналогового регистратора (послеаварый массив)
          edition_settings.postfault_number_periods = tempWriteArray[offsetTempWriteArray+i];//В таблицю настройок записуємо не мілісекунди, а кількість періодів
          upravlSetting = 1;//флаг Setting
          break;
        case 35://Текущий аналоговый регистратор
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
        break;//case 35
        case 36://Очистить аналоговый регистратор
//ОСОБАЯ ПРОВЕРКА
          if (
            (state_ar_record_m           != STATE_AR_NONE_M )
            ||  
            (state_ar_record_prt         != STATE_AR_NONE_PRT )
            ||  
            (
             (control_tasks_dataflash & (
                                         TASK_MAMORY_PART_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_FS |
                                         TASK_MAMORY_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_FS      |
                                         TASK_MAMORY_READ_DATAFLASH_FOR_FS 
                                        )
             ) != 0
            )
            ||
            ((clean_rejestrators & CLEAN_AR) != 0)
          ) return ERROR_VALID2;//ошибка валидации

          clean_rejestrators |= CLEAN_AR;
          break;

        case 333://Время записи аналогового регистратора (доаварийный массив)
          edition_settings.timeout_prolongation_work_digital_registrator = ((unsigned short)tempWriteArray[offsetTempWriteArray+i])*10;
          upravlSetting = 1;//флаг Setting
          break;
        case 335://Текущий дискретный регистратор

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

          break;
        case 336://Очистить дискретный регистратор
//ОСОБАЯ ПРОВЕРКА
          if (
            (current_ekran.current_level == EKRAN_TITLES_DIGITAL_REGISTRATOR)
            ||
            (current_ekran.current_level == EKRAN_CHANGES_SIGNALS_DR        )
            ||
            (current_ekran.current_level == EKRAN_TITLE_MAX_VALUES          )
            ||
            (current_ekran.current_level == EKRAN_MAX_VALUES                )
            ||
            (
              (control_tasks_dataflash & (
                 TASK_MAMORY_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_DR       |
                 TASK_MAMORY_READ_DATAFLASH_FOR_DR_USB                          |
                 TASK_MAMORY_READ_DATAFLASH_FOR_DR_RS485                        |
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                 TASK_MAMORY_READ_DATAFLASH_FOR_DR_LAN                          |
#endif  
                 TASK_MAMORY_READ_DATAFLASH_FOR_DR_MENU                         |
                 TASK_MAMORY_READ_DATAFLASH_FOR_DR_MENU_SHORT
               )
              ) != 0
            )
            ||
            ((clean_rejestrators & CLEAN_DR) != 0)
          ) return ERROR_VALID2;//ошибка валидации

          //Помічаємо, що треба очистити дискретного реєстратора
          clean_rejestrators |= CLEAN_DR;
          break;
        }//switch
    }//for

  return 0;
}//

int privateREGBigGetReg2(int adrReg)
{
  //проверить внешний периметр
  return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)

int passwordImunitetRegREGBigComponent(int adrReg)
{
  //имунитетные к паролю адреса регистров 0 - есть имунитет
  if(privateREGBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  switch(adrReg)
  {
  case BEGIN_ADR_REGISTER+IMUNITET_REG35://Текущий аналоговый регистратор
  case BEGIN_ADR_REGISTER+IMUNITET_REG335://Текущий дискретный регистратор
    return 0;//есть имунитет
  }//switch
  return MARKER_OUTPERIMETR;
}//passwordImunitetRegREGBigComponent(int adrReg)
