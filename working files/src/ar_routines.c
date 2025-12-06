#include "header.h"

/*************************************************************************
Робота з аналоговим реєстратором і FATFs на низькому рівні програми
*************************************************************************/
void ar_routine_with_fatfs(unsigned int before_full_start)
{
  static unsigned int level_ar_writing;
  if (
      (_CHECK_SET_BIT(    diagnostyka, ERROR_FATFS) == 0) &&
      (_CHECK_SET_BIT(set_diagnostyka, ERROR_FATFS) == 0)
     )   
  {
    //Немає проблем з FATFS
    
    /***********************************************************/
    //Перевіряємо необхідність очистки аналогового і дискретного реєстраторів
    /***********************************************************/
    //Аналоговий реєстратор
    if (
        ((clean_rejestrators & CLEAN_AR) != 0 )
        &&  
        (
         (state_ar_record_fatfs == STATE_AR_NONE_FATFS) ||
         (state_ar_record_fatfs == STATE_AR_MEMORY_FULL_FATFS) ||
         (state_ar_record_fatfs == STATE_AR_BLOCK_FATFS)
        )   
        &&  
        (
         (control_tasks_dataflash & (
                                     TASK_MAMORY_PART_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_FS |
                                     TASK_MAMORY_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_FS      |
                                     TASK_MAMORY_READ_DATAFLASH_FOR_FS
                                    )
         ) == 0
        )   
       )
    {
      //Виставлено каманда очистити аналогового реєстратора

      //Виставляємо команду запису цієї структури у EEPROM
      _SET_BIT(control_spi1_taskes, TASK_START_WRITE_INFO_REJESTRATOR_AR_EEPROM_BIT);
   
      //Очищаємо структуру інформації по дискретному реєстраторі
      info_rejestrator_ar.first_number = -1;
      info_rejestrator_ar.last_number = -1;
      _SET_STATE(FATFS_command, FATFS_FORMAT);
      
      //Помічаємо, що номер запису не вибраний
      number_record_of_ar_for_menu = -1;
      number_record_of_ar_for_USB = -1;
      id_ar_record_for_USB[0] = '\0';
      number_record_of_ar_for_RS485 = 0xffff;
      id_ar_record_for_RS485[0] = '\0';
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)            
      number_record_of_ar_for_LAN = 0xffff;
      id_ar_record_for_LAN[0] = '\0';
#endif

      //Знімаємо команду очистки аналогового реєстратора
      clean_rejestrators &= (unsigned int)(~CLEAN_AR);
    }
      
    /***
    Управління режимами роботи цього рівня з Аналоговим реєстратором
    ***/
    if (FATFS_command != 0)
    {
      //Команди для файлової системи
      if (_GET_STATE(FATFS_command, FATFS_FORMAT))
      {
        _CLEAR_STATE(FATFS_command, FATFS_FORMAT);
          
        //формуємо файлову систему
        FRESULT res = f_mkfs (USERPath, 1, 0);
        if (res != FR_OK) _SET_BIT(set_diagnostyka, ERROR_FATFS);
        else
        {
          state_ar_record_fatfs = STATE_AR_NONE_FATFS;
          _SET_BIT(clear_diagnostyka, ERROR_AR_UNDEFINED_BIT);
        }
      }
      else if (_GET_STATE(FATFS_command, FATFS_READ_DATA_FOR_MENU))
      {
        if (current_ekran.current_level == EKRAN_LIST_ANALOG_REGISTRATOR_RECORDS)
        {
          make_ekran_list_records_registrator_ar();
        }
        else _CLEAR_STATE(FATFS_command, FATFS_READ_DATA_FOR_MENU);
      }
    }
      
    if (state_ar_record_fatfs == STATE_AR_WAIT_TO_WRITE_FATFS)
    {
      if (
          (
           (state_ar_record_m == STATE_AR_WORK_M) ||
           (state_ar_record_m == STATE_AR_WORK_STOP_M) /*цей випадок є фактично малоймовірним*/
          )
          &&
          (index_array_ar_tail != index_array_ar_heat) /*вимірювальна система вже розставила мітки "загрузки" і "вигрузки"*/
          &&  
          (
           ((POWER_CTRL->IDR & POWER_CTRL_PIN) != (uint32_t)Bit_RESET) /*На даний момент на вході блоку живлення подається живлення*/ ||
           (timePowerDown <= POWER_DOWN_TIME) /*час протягом якого ще дозволена робота аналогового реєстратора*/
          )

         )
      {
        state_ar_record_fatfs = STATE_AR_WRITE_FATFS;
        level_ar_writing = LEVEL_AR_WRITING_SEL_NUMBER;
      } 
    }
  }
  else state_ar_record_fatfs = STATE_AR_BLOCK_FATFS;
  
  if (state_ar_record_fatfs != STATE_AR_MEMORY_FULL_FATFS) _SET_BIT(clear_diagnostyka, ERROR_AR_MEMORY_FULL_BIT);
  /***/
  
  static int first_number, last_number;
  static FIL fil;
  static unsigned int dirty_data_ar;
  static unsigned int beforeArraySaved;
  
  switch (state_ar_record_fatfs)
  {
  case STATE_AR_WRITE_FATFS:
    {
      switch (level_ar_writing)
      {
      case LEVEL_AR_WRITING_SEL_NUMBER:
        {
          if (
              ((POWER_CTRL->IDR & POWER_CTRL_PIN) == (uint32_t)Bit_RESET) &&
              (timePowerDown > POWER_DOWN_TIME)  
             )
          {
            break;
          }
          else 
          {
            dirty_data_ar = false;
            beforeArraySaved = false;
          }

          
          //Вибираємо ім'я файлу
          first_number = info_rejestrator_ar.first_number;
          last_number = info_rejestrator_ar.last_number;
          if (
              (first_number < 0) &&
              (last_number >= 0)
             )
          {
            //Невизначена ситуація, якої ніколи б не мало бути. Вирішити її може тільки переформатування
            state_ar_record_fatfs = STATE_AR_BLOCK_FATFS;
            _SET_BIT(set_diagnostyka, ERROR_AR_UNDEFINED_BIT);
            break;
          }
          else
          {
            if (first_number < 0)
            {
              last_number = first_number = 0;
            }
            else
            {
              first_number = (first_number + 1) & MASKA_FATFS_NAME;
            }
              
            level_ar_writing = LEVEL_AR_WRITING_OPEN_FILE;
          }
        }
      case LEVEL_AR_WRITING_OPEN_FILE:
        {
          if (
              ((POWER_CTRL->IDR & POWER_CTRL_PIN) == (uint32_t)Bit_RESET) &&
              (timePowerDown > POWER_DOWN_TIME)  
             )
          {
            break;
          }

          //Створюємо файл
          char str[8 + 1 + 3 + 1];
          int n = snprintf(str, (8 + 1), "%d", first_number);
          if (n > 8) n = 8; //Теоретично цього ніколи б не мало бути
          str[n++] = '.';
          str[n++] = 'd';
          str[n++] = 'a';
          str[n++] = 't';
          str[n++] = '\0';

          FRESULT res;
          unsigned int error = 0;
          do
          {
            res = f_open(&fil, str, FA_CREATE_ALWAYS | FA_WRITE);
            if (res == FR_OK) res = f_lseek(&fil, 0);
            else if (
                     (res == FR_DENIED) ||
                     (res == FR_NOT_ENOUGH_CORE)  
                    )
            {
              do
              {
                error = ar_free_space(&first_number, &last_number);
              }
              while (error == 1);
            }
          }
          while ((res != FR_OK) && (error < 2));
          if (before_full_start == false) periodical_operations_communication(true);  

          if (res != FR_OK) 
          {
            //Невизначена ситуація, якої ніколи б не мало бути. Вирішити її може тільки переформатування
            state_ar_record_fatfs = STATE_AR_BLOCK_FATFS;
            _SET_BIT(set_diagnostyka, ERROR_AR_UNDEFINED_BIT);
            break;
          }
          
          level_ar_writing = LEVEL_AR_WRITING_HEADER;
        }
      case LEVEL_AR_WRITING_HEADER:
        {
          if (
              ((POWER_CTRL->IDR & POWER_CTRL_PIN) == (uint32_t)Bit_RESET) &&
              (timePowerDown > POWER_DOWN_TIME)  
             )
          {
            break;
          }
          
          FRESULT res = FR_OK;
          uint8_t *Buffter =  (uint8_t *)(&header_ar);
          size_t ByteToWrite = sizeof(header_ar);
          size_t ByteWritten = ByteToWrite;
          unsigned int error = 0;

          if  (arDateTimeState == AVAT_DATE_TIMR_FIX_LEVEL1) arDateTimeState = AVAT_DATE_TIMR_FIX_LEVEL2;
          
          do
          {
            if (ByteWritten != ByteToWrite)
            {
              do
              {
                error = ar_free_space(&first_number, &last_number);
              }
              while (error == 1);
              
              Buffter += ByteWritten;
              ByteToWrite = ByteToWrite - ByteWritten;
            }
            
            if (error == 0)
            {
              res = f_write(&fil, Buffter, ByteToWrite, &ByteWritten);
              if (before_full_start == false) periodical_operations_communication(true);
            }
          }
          while ((res == FR_OK) && (error < 2) && (ByteToWrite != ByteWritten));
          
          if ((res |= FR_OK) || (ByteToWrite != ByteWritten))
          {
            //Невизначена ситуація, якої ніколи б не мало бути. Вирішити її може тільки переформатування
            state_ar_record_fatfs = STATE_AR_BLOCK_FATFS;
            _SET_BIT(set_diagnostyka, ERROR_AR_UNDEFINED_BIT);
            break;
          }
          
          dirty_data_ar = true;
          level_ar_writing = LEVEL_AR_WRITING_DATA;

          break;
        }

      case LEVEL_AR_REWRITING_DATE_TIME:
        {
          if (
              ((POWER_CTRL->IDR & POWER_CTRL_PIN) == (uint32_t)Bit_RESET) &&
              (timePowerDown > POWER_DOWN_TIME)  
             )
          {
            break;
          }
          
          struct _rewrite_data
          {
            uint8_t * beginAddress;
            size_t size;
          } rewrite_data[2] = 
          {
            {
              (uint8_t *)(&(header_ar.time_dat)),
              sizeof(header_ar.time_dat)
              
            },
            {
              (uint8_t *)(&(header_ar.time_ms)),
              sizeof(header_ar.time_ms)
            }
          };
          
          FRESULT res = FR_OK;
          size_t ByteToWrite = 0;
          size_t ByteWritten = 0;
          uint8_t * baseAddress = (uint8_t *)(&header_ar);
          for (size_t i = 0; ((i < 2) && (res == FR_OK) && (ByteToWrite == ByteWritten)); ++i)
          {
            uint8_t * beginAddress = rewrite_data[i].beginAddress;
            DWORD shift = beginAddress - baseAddress;
            res = f_lseek(&fil, shift);
          
            if (res == FR_OK)
            {
              uint8_t *Buffter =  beginAddress;
              ByteToWrite = rewrite_data[i].size;
          
              res = f_write(&fil, Buffter, ByteToWrite, &ByteWritten);
              if (before_full_start == false) periodical_operations_communication(true);
            }
          }

          if ((res == FR_OK) && (ByteToWrite == ByteWritten)) res = f_lseek(&fil, f_size(&fil));
          else
          {
            //Невизначена ситуація, якої ніколи б не мало бути. Вирішити її може тільки переформатування
            state_ar_record_fatfs = STATE_AR_BLOCK_FATFS;
            _SET_BIT(set_diagnostyka, ERROR_AR_UNDEFINED_BIT);
            break;
          }
          
          arDateTimeState = AVAT_DATE_TIMR_FIX_LEVEL2;
          dirty_data_ar = true;
          level_ar_writing = LEVEL_AR_WRITING_DATA;
        }

      case LEVEL_AR_WRITING_DATA:
        {
          FRESULT res = FR_OK;
          unsigned int error = 0;
          
          unsigned int index_array_ar_tail_tmp = index_array_ar_tail;
          while (
                 (res == FR_OK) &&
                 (error < 2) && 
                 (tail_to_heat == false) &&
                 (state_ar_record_m != STATE_AR_BLOCK_M) && 
                 (
                  ((POWER_CTRL->IDR & POWER_CTRL_PIN) != (uint32_t)Bit_RESET) /*На даний момент на вході блоку живлення подається живлення*/ ||
                  (timePowerDown <= POWER_DOWN_TIME) /*час протягом якого ще дозволена робота аналогового реєстратора*/
                 )
                )   
          {

            if (
                ((POWER_CTRL->IDR & POWER_CTRL_PIN) == (uint32_t)Bit_RESET) &&
                (beforeArraySaved == false) &&  
                (header_ar.prefault_number_periods > AR_TAIL_MIN_NUMBER_PERIOD)
               )   
            {
              res = f_lseek(&fil, 0);
              if (res == FR_OK) 
              {
                res = f_truncate(&fil);
                if (res == FR_OK)
                {
                  truncPrefault = true;
                  header_ar.prefault_number_periods = AR_TAIL_MIN_NUMBER_PERIOD;
                  index_array_ar_tail_tmp = index_array_ar_tail = index_array_tail_min;
                  level_ar_writing = LEVEL_AR_WRITING_HEADER;
                }
              }
                
              break;
            }
            else if  (arDateTimeState == AVAT_DATE_TIMR_FIX_LEVEL1) 
            {
              level_ar_writing = LEVEL_AR_REWRITING_DATE_TIME;
              break;
            }

            unsigned int index_array_ar_heat_tmp = index_array_ar_heat;
          
            /***
            Копіюємо частину підготовлених даних у тимчасовий буфер для запису з допомогою FATFS
            ***/
            static short int bufer_to_write[5*NUMBER_POINT_AR*AR_TOTAL_NUMBER_CANALES]; /*5x0,02(c) = 0,1(c)*/
            short int *p_target = bufer_to_write;
            short int *p_source = &array_ar[index_array_ar_tail_tmp];
            size_t i = 0;
            while(
                  (index_array_ar_tail_tmp != index_array_ar_heat_tmp) &&
                  (
                   (
                    ((POWER_CTRL->IDR & POWER_CTRL_PIN) != (uint32_t)Bit_RESET) &&
                    (i < (1*NUMBER_POINT_AR))
                   )
                   ||  
                   (
                    ((POWER_CTRL->IDR & POWER_CTRL_PIN) == (uint32_t)Bit_RESET) &&
                    (i < (NUMBER_POINT_AR))
                   )
                  )   
                 )
            {
              if ((beforeArraySaved == false) && (index_array_ar_tail_tmp == index_array_tail_min)) beforeArraySaved = true;

              for (size_t j = 0; j< AR_TOTAL_NUMBER_CANALES; j++)
              {
                *p_target++ = *p_source++;
                index_array_ar_tail_tmp++;
              }
              if (index_array_ar_tail_tmp >= SIZE_BUFFER_FOR_AR)
              {
                index_array_ar_tail_tmp = 0;
                p_source = array_ar;
              }
              index_array_ar_tail = index_array_ar_tail_tmp;
              
              i++;
            }
            if (index_array_ar_tail_tmp == index_array_ar_heat_tmp) tail_to_heat = true;
            /***/

            /***
            Підготовка до запису даних
            ***/
            uint8_t *Buffter =  (uint8_t *)bufer_to_write;
            size_t ByteToWrite = (i*AR_TOTAL_NUMBER_CANALES) << 1;
            size_t ByteWritten = ByteToWrite;
            error = 0;
            /***/
            
            /***
            Запис через FATFS
            ***/
            do
            {
              if (ByteWritten != ByteToWrite)
              {
                do
                {
                  error = ar_free_space(&first_number, &last_number);
                }
                while (error == 1);
              
                Buffter += ByteWritten;
                ByteToWrite -= ByteWritten;
              }
            
              if (error == 0) 
              {
                res = f_write(&fil, Buffter, ByteToWrite, &ByteWritten);
                if (res == FR_OK) dirty_data_ar = true;
                
                if (before_full_start == false) periodical_operations_communication(true);
              }
            }
            while (
                   (res == FR_OK) && 
                   (error < 2) && 
                   (state_ar_record_m != STATE_AR_BLOCK_M) &&
                   (ByteToWrite != ByteWritten)
                  );
            /***/
          }
          
          if (
              (res |= FR_OK) ||
              (error == 2)
             ) 
          {
            //Невизначена ситуація, якої ніколи б не мало бути. Вирішити її може тільки переформатування
            state_ar_record_fatfs = STATE_AR_BLOCK_FATFS;
            _SET_BIT(set_diagnostyka, ERROR_AR_UNDEFINED_BIT);
          }

          else if (
                   (level_ar_writing == LEVEL_AR_WRITING_HEADER) ||
                   (level_ar_writing == LEVEL_AR_REWRITING_DATE_TIME)
                  )   
          {
            //нічого не робимо
          }

          else if (error == 3)
          {
            _SET_BIT(set_diagnostyka, ERROR_AR_MEMORY_FULL_BIT);
            state_ar_record_fatfs = STATE_AR_MEMORY_FULL_FATFS;
          }
          else if (
                   (index_array_ar_tail == index_array_ar_heat) || 
                    (state_ar_record_m == STATE_AR_BLOCK_M)
                  )   
          {
            state_ar_record_fatfs = STATE_AR_WAIT_TO_STOP_WRITE_FATFS;
          }
          
          break;
        }
      default:
        {
          //Теоретично цього ніколи не мало б бути
          total_error_sw_fixed();
        }
      }
      
      if (
          (state_ar_record_fatfs == STATE_AR_MEMORY_FULL_FATFS)
          ||
          (
           (state_ar_record_fatfs == STATE_AR_BLOCK_FATFS) &&
           (level_ar_writing > LEVEL_AR_WRITING_OPEN_FILE)  
          )
          ||
          (
           (
            ((POWER_CTRL->IDR & POWER_CTRL_PIN) == (uint32_t)Bit_RESET) &&
            (timePowerDown > POWER_DOWN_TIME)  
           )
           &&
           (dirty_data_ar != false) 
          )
         )   
      {
        FRESULT res;
        if (
            (
             ((POWER_CTRL->IDR & POWER_CTRL_PIN) == (uint32_t)Bit_RESET) &&
             (timePowerDown > POWER_DOWN_TIME)    
            )
            &&

            (dirty_data_ar != false) 
           ) 
        {
          //Скидаємо кешовані дані на диск
          if (f_size(&fil) > sizeof(__HEADER_AR)) res = f_sync(&fil);
          else
          {
            res = f_close(&fil);
            if (res == FR_OK)
            {
              char str[8 + 1 + 3 + 1];
              int n = snprintf(str, (8 + 1), "%d", first_number);
              if (n > 8) n = 8; //Теоретично цього ніколи б не мало бути
              str[n++] = '.';
              str[n++] = 'd';
              str[n++] = 'a';
              str[n++] = 't';
              str[n++] = '\0';
            
              res = f_unlink(str);
              if (
                  (res == FR_OK) ||
                  (res == FR_NO_FILE)  
                 )   
              {
                if (last_number > first_number)
                {
                  first_number -= 1;
                  if (first_number < 0) first_number += NUMBER_FATFS_NAME;
                }
                else if (last_number == first_number)
                {
                  last_number = first_number = -1;
                }
                else
                {
                  first_number -= 1;
                }
              
                level_ar_writing = LEVEL_AR_WRITING_SEL_NUMBER;
                res = FR_OK;
              }
            }
          }

        }
        else
        {
          //Закриваємо файл
          res = f_close(&fil);
        }
        
        if (res != FR_OK)
        {
          //Невизначена ситуація, якої ніколи б не мало бути. Вирішити її може тільки переформатування
          state_ar_record_fatfs = STATE_AR_BLOCK_FATFS;
          _SET_BIT(set_diagnostyka, ERROR_AR_UNDEFINED_BIT);
        }
        else 
        {
          dirty_data_ar = false;
          
          if (
              (info_rejestrator_ar.first_number != first_number) ||
              (info_rejestrator_ar.last_number  != last_number )
             )
          {
            _SET_BIT(control_spi1_taskes, TASK_START_WRITE_INFO_REJESTRATOR_AR_EEPROM_BIT);
            info_rejestrator_ar.first_number = first_number;
            info_rejestrator_ar.last_number = last_number;
          }
        }
      }
      
      if ((POWER_CTRL->IDR & POWER_CTRL_PIN) == (uint32_t)Bit_RESET)
      {
        //Активні операції по Аналоговому реєстратору блокуються
        if (before_full_start == false) periodical_operations_communication(false);
        periodical_operations(false);
      }
      
      break;
    }
  case STATE_AR_STOP_WRITE_FATFS:
    {
      //Закриваємо файл
      FRESULT res = f_close(&fil);
      if (res != FR_OK)
      {
        //Невизначена ситуація, якої ніколи б не мало бути. Вирішити її може тільки переформатування
        state_ar_record_fatfs = STATE_AR_BLOCK_FATFS;
        _SET_BIT(set_diagnostyka, ERROR_AR_UNDEFINED_BIT);
      }
      else
      {
        dirty_data_ar = false;
          
        if (
            (info_rejestrator_ar.first_number != first_number) ||
            (info_rejestrator_ar.last_number  != last_number )
           )
        {
          _SET_BIT(control_spi1_taskes, TASK_START_WRITE_INFO_REJESTRATOR_AR_EEPROM_BIT);
          info_rejestrator_ar.first_number = first_number;
          info_rejestrator_ar.last_number = last_number;
        }
        
        state_ar_record_fatfs = STATE_AR_NONE_FATFS;

        beforeArraySaved = false;
      }
        
      break;
    }
  default:
    {
      //Немає активних операцій по Аналоговому реєстратору
      if (before_full_start == false) periodical_operations_communication(false);
      periodical_operations(true);
      break;
    }
  }
}
/*************************************************************************/

/*************************************************************************
Вивільняємо місце з енергонезалежної пам'яті
0 - операція виконана вдало
1 - операція не виконана вдало, але варто спробувати ще...
2 - операція не виконана вдало і повторну стробу робити немає сенсу
3 - існує єдиний файл
*************************************************************************/
unsigned int ar_free_space(int* p_first_number, int* p_last_number)
{
  unsigned int error = 0;
  
  int last_number = *p_last_number;
  if (last_number >= 0)
  {
    int first_number = *p_first_number;
    if (last_number != first_number)
    {
      char str[8 + 1 + 3 + 1];
      int n = snprintf(str, (8 + 1), "%d", last_number);
      if (n > 8) n = 8; //Теоретично цього ніколи б не мало бути
      str[n++] = '.';
      str[n++] = 'd';
      str[n++] = 'a';
      str[n++] = 't';
      str[n++] = '\0';
      
      FRESULT res;
      res = f_unlink(str);
      if (
          (res == FR_OK) ||
          (res == FR_NO_FILE)  
         )   
      {
        last_number = (last_number + 1) & MASKA_FATFS_NAME;
        *p_last_number = last_number;
        if (res == FR_NO_FILE) error = 1;
      }
      else error = 2;
    }
    else  error = 3;
  }
  else error = 2;
  
  return error;
}
