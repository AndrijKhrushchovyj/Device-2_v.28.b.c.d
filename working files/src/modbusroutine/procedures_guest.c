#include "header.h"

int configAnalogRegistrator(int offsetRegister, int recordNumber, int recordLen);
int dataAnalogRegistrator(int offsetRegister, int recordNumber, int recordLen);
int configDiskretRegistrator(int offsetRegister, int recordNumber, int recordLen);
int dataDiskretRegistrator(int offsetRegister, int recordNumber, int recordLen);
int recordNumberCase0Case1(int offsetRegister, int recordNumber, int recordLen, int registrator);
int recordNumberCaseDiskret(int subObj, int offsetRegister);
int recordNumberCaseOther(int subObj, int offsetRegister, int recordLen, int registrator);
int superReader20(int offsetRegister, int fileNumber, int recordNumber, int recordLen);
int openRegistrator(int number_file);

int openRegistratorRDS(void);//Регистратор дискретных сигналов
int dataRegistratorRDS(int offsetRegister, int recordNumber, int recordLen);//Регистратор дискретных сигналов

#define DISKRET_TOTAL NUMBER_TOTAL_SIGNAL_FOR_RANG
#define DISKRET_REGISTRATOR 0
#define ANALOG_REGISTRATOR  1

//Ідентитифікатор каналу - 16 ASCII символів
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
static const char idetyficator_rang[MAX_NAMBER_LANGUAGE][NUMBER_TOTAL_SIGNAL_FOR_RANG + (1 - N_IN_GOOSE)  + (1 - N_IN_MMS) + (1 - N_OUT_LAN) + (3 - NUMBER_UP_SIGNAL_FOR_RANG)][16] =
#else
static const char idetyficator_rang[MAX_NAMBER_LANGUAGE][NUMBER_TOTAL_SIGNAL_FOR_RANG - NUMBER_UP_SIGNAL_FOR_RANG + 3][16] =
#endif
{
  {NAME_RANG_RU},
  {NAME_RANG_UA},
  {NAME_RANG_EN},
  {NAME_RANG_KZ}
};

/***********************************************************************************/
//открыть данные регистратора AR DR
/***********************************************************************************/
int openRegistrator(int number_file)
{
  if(number_file==100) return openRegistratorRDS();//Регистратор дискретных сигналов
//ОБЩЕЕ AR DR
  if (
    (
      (number_file >= 4) &&
      (
        ((pointInterface == USB_RECUEST  ) && ((number_record_of_ar_for_USB   == -1) || (id_ar_record_for_USB[0]   == '\0'))) ||
        ((pointInterface == RS485_RECUEST) && ((number_record_of_ar_for_RS485 == -1) || (id_ar_record_for_RS485[0] == '\0')))
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
        ||
        ((pointInterface == LAN_RECUEST) && ((number_record_of_ar_for_LAN == -1) || (id_ar_record_for_LAN[0] == '\0')))
#endif
      )
    )
    ||
    (
      (number_file >= 1) &&
      (number_file <= 2) &&
      (
        ((pointInterface == USB_RECUEST  ) && (number_record_of_dr_for_USB   == 0xffff)) ||
        ((pointInterface == RS485_RECUEST  ) && (number_record_of_dr_for_RS485 == 0xffff))
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
        ||
        ((pointInterface == LAN_RECUEST  ) && (number_record_of_dr_for_LAN == 0xffff))
#endif
      )
    )
  )
    {
      //Невірний номер файлу, або не подано команди вичитування відповідного запису
      return -1;
    }
  else if (
    (number_file >= 4) &&
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
      //error = ERROR_SLAVE_DEVICE_BUSY;
      return -2;
      /*
      Тут ми не перевіряємо умову на виставлений біт TASK_MAMORY_READ_DATAFLASH_FOR_AR_INTERFACE,
      бо, оскільки, масиви є великі і зразу весь запис прочитати не можливо, то
      програмне забеспечення побудоване на принципі докачки. Тобто формується відповідь з даних, які зараз
      зчитуються з мікросхеми dataFlash
      */
    }
  else if (
    (number_file >= 1) &&
    (number_file <= 2) &&
    (
      (
        ((pointInterface == USB_RECUEST  ) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_DR_USB  ) != 0)) ||
        ((pointInterface == RS485_RECUEST  ) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_DR_RS485) != 0))
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
        ||
        ((pointInterface == LAN_RECUEST  ) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_DR_LAN) != 0))
#endif
      )
      ||
      ((clean_rejestrators & CLEAN_DR) != 0)
    )
  )
    {
      //Зараз іде зчитування для інтерфейсу запису дискретного реєстратора, тому ця операція є тимчасово недоступною
      //error = ERROR_SLAVE_DEVICE_BUSY;
      return -2;
    }//if

//ТОЛЬКО DR
  if(number_file<3)//
    {
      while (
        ((pointInterface == USB_RECUEST  ) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_DR_USB  ) != 0)) ||
        ((pointInterface == RS485_RECUEST  ) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_DR_RS485) != 0))
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
        ||
        ((pointInterface == LAN_RECUEST  ) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_DR_LAN) != 0))
#endif
      )
        {
          //Якщ очасом буде спрацьовувати Watchdog, то тут треба буде поставити функцію роботи з ним
          if (
            (control_spi1_taskes[0] != 0) ||
            (control_spi1_taskes[1] != 0) ||
            (state_execution_spi1 > 0)
          )
            {
              mutex_spi1 = true;
              if (driver_spi_df[number_chip_dataflsh_exchange].state_execution == TRANSACTION_EXECUTING_NONE)
                {
                  main_routines_for_spi1();
                }
              mutex_spi1 = false;
            }
        }//while
    }//else
//ТОЛЬКО AR
  else
    {
    }//if(number_file<5)

  return 0;
}//openRegistrator(int number_file)

/**************************************/
//регистровый читатель 20-й функции
/**************************************/
int superReader20(int offsetRegister, int fileNumber, int recordNumber, int recordLen)
{
  switch (fileNumber)
    {
    case 1://Конфигурация дискретного регистратора
      return configDiskretRegistrator(offsetRegister, recordNumber, recordLen);

    case 2://Данные дискретного регистратора 1часть
    case 3://Данные дискретного регистратора 2часть
      if (fileNumber == 3) recordNumber += 10000;
      return dataDiskretRegistrator(offsetRegister, recordNumber, recordLen);

    case 4://Конфигурация аналогового регистратора
      return configAnalogRegistrator(offsetRegister, recordNumber, recordLen);

    case 100://Данные Регистратор дискретных сигналов
      return dataRegistratorRDS(offsetRegister, recordNumber, recordLen);

    default:
      {
      //Данные аналогового регистратора 
      if (fileNumber == 0) break;
      if (fileNumber > 5) recordNumber += (fileNumber - 5)*10000;
      return dataAnalogRegistrator(offsetRegister, recordNumber, recordLen);
      }//default
    }//switch

  return MARKER_ERRORPERIMETR;
}//superReader20(int fileNumber, int offsetRegister)

int dataAnalogRegistrator(int offsetRegister, int recordNumber, int recordLen)
{
//данные аналогового регистратора
  unsigned int lenMax = (3 + AR_TOTAL_NUMBER_CANALES);
  if ((unsigned int)recordLen > lenMax) return MARKER_ERRORPERIMETR;//уйти если превышение

  int *point_to_first_number_time_sample = NULL, *point_to_last_number_time_sample = NULL;
  char *point_id_ar_record = NULL;
  int *point_to_max_number_time_sample = NULL;
  unsigned char *point_to_Buffer = NULL;

  if (pointInterface == USB_RECUEST)
    {
      point_id_ar_record = id_ar_record_for_USB;
      point_to_first_number_time_sample = &first_number_time_sample_for_USB;
      point_to_last_number_time_sample  = &last_number_time_sample_for_USB;
      point_to_max_number_time_sample = &max_number_time_sample_USB;
          
      point_to_Buffer = buffer_for_USB_read_record_ar;
    }
  else if (pointInterface == RS485_RECUEST)
    {
      point_id_ar_record = id_ar_record_for_RS485;
      point_to_first_number_time_sample = &first_number_time_sample_for_RS485;
      point_to_last_number_time_sample  = &last_number_time_sample_for_RS485;
      point_to_max_number_time_sample = &max_number_time_sample_RS485;
          
      point_to_Buffer = buffer_for_RS485_read_record_ar;
    }
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
  else if (pointInterface == LAN_RECUEST)
    {
      point_id_ar_record = id_ar_record_for_LAN;
      point_to_first_number_time_sample = &first_number_time_sample_for_LAN;
      point_to_last_number_time_sample  = &last_number_time_sample_for_LAN;
      point_to_max_number_time_sample = &max_number_time_sample_LAN;
          
      point_to_Buffer = buffer_for_LAN_read_record_ar;
    }
#endif  
  else
  {
    //Теоретично цього ніколи не мало б бути
    total_error_sw_fixed();
  }

  if(recordNumber>*point_to_max_number_time_sample) return MARKER_ERRORPERIMETR;//уйти если превышение

  //Перевіряємо чи зчитано заголовок аналогового реєстратора
  if (
    (((int)recordNumber) < (*point_to_first_number_time_sample)) ||
    (((int)recordNumber) > (*point_to_last_number_time_sample ))
  )
    {
      //Запитуваний часовий зріз не є зчитаним
      FIL fil;
      FRESULT res = f_open(&fil, point_id_ar_record, FA_READ);
      if (res == FR_OK) 
      {
        //Виставляємо читання без заголовку запису даного запису з запитуваноого номеру зрізу і дальше, скільки можливо, часових зрізів 
        *point_to_first_number_time_sample = recordNumber;
        int last_number_time_sample_tmp = recordNumber + SIZE_PAGE_DATAFLASH_2/(AR_TOTAL_NUMBER_CANALES*sizeof(short int));
        int max_number_time_sample = *point_to_max_number_time_sample;
            
        if (last_number_time_sample_tmp <= max_number_time_sample)
        {
          *point_to_last_number_time_sample = last_number_time_sample_tmp - 1;//номер останнього часового зрізу ВКЛЮЧНО
        }
        else
        {
          *point_to_last_number_time_sample = max_number_time_sample - 1;
        }
              
        res = f_lseek(&fil, (sizeof(__HEADER_AR) + recordNumber*AR_TOTAL_NUMBER_CANALES*sizeof(short int)));
        if (res == FR_OK)
        {
          unsigned int read_tmp;
          UINT ByteToRead = ((unsigned int)(*point_to_last_number_time_sample) - recordNumber + 1)*AR_TOTAL_NUMBER_CANALES*sizeof(short int);
          res = f_read(&fil, point_to_Buffer, ByteToRead, &read_tmp);
          if ((res != FR_OK) || (read_tmp != ByteToRead)) return MARKER_ERRORPERIMETR/*насправді потібно видати помилку ERROR_SLAVE_DEVICE_FAILURE*/;
        }
        else return MARKER_ERRORPERIMETR/*насправді потібно видати помилку ERROR_SLAVE_DEVICE_FAILURE*/;

        res = f_close(&fil);
        if (res != FR_OK) 
        {
          //Невизначена ситуація, якої ніколи б не мало бути.
          _SET_BIT(set_diagnostyka, ERROR_AR_UNDEFINED_BIT);
          return MARKER_ERRORPERIMETR/*насправді потібно видати помилку ERROR_SLAVE_DEVICE_FAILURE*/;
        }
      }
      else return MARKER_ERRORPERIMETR/*насправді потібно видати помилку ERROR_SLAVE_DEVICE_FAILURE*/;
    }

  //Якщо ми сюди дійшли, то вважаємо що запитувана виборка зчитана і знаходиться у буфері читання аналогового реєстратора для інтерфейсу

  //Визначаємо індекс у буфері читання з якого розміщений потрібний нам часовий зріз
  unsigned int index_time_sample;
  if((*point_to_first_number_time_sample) == -1)
    {
      index_time_sample = sizeof(__HEADER_AR) + recordNumber*
                          AR_TOTAL_NUMBER_CANALES*sizeof(short int);
    }
  else
    {
      index_time_sample = 0 + (recordNumber - (*point_to_first_number_time_sample))*
                          AR_TOTAL_NUMBER_CANALES*sizeof(short int);
    }

  switch(offsetRegister)
    {
    case 0://порядковый номер выборки
      return recordNumber + 1;
    case 1://метка времени (ms)
      return recordNumber & 0xffff;
    case 2://метка времени (ms)
      return recordNumber >> 16;
    }//switch

  unsigned char *point_to_buffer = NULL;
  if (pointInterface == USB_RECUEST)
    point_to_buffer = buffer_for_USB_read_record_ar;
  else if (pointInterface == RS485_RECUEST)
    point_to_buffer = buffer_for_RS485_read_record_ar;
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
  else if (pointInterface == LAN_RECUEST)
    point_to_buffer = buffer_for_LAN_read_record_ar;
#endif
  else
  {
    //Теоретично цього ніколи не мало б бути
    total_error_sw_fixed();
  }

  return (*(point_to_buffer + index_time_sample +2*(offsetRegister-3))) + ((*(point_to_buffer +
         index_time_sample + 1 +2*(offsetRegister-3))) << 8);
}//dataAnalogRegistrator(int offsetRegister, int recordNumber, int recordLen)

int configAnalogRegistrator(int offsetRegister, int recordNumber, int recordLen)
{
//Конфигурация аналогового регистратора

  switch(recordNumber)
    {
    case 0://Имя станции recordNumber
    case 1://Количество каналов recordNumber
      return recordNumberCase0Case1(offsetRegister, recordNumber, recordLen, ANALOG_REGISTRATOR);
    }//switch

//АНАЛОГОВЫЕ КАНАЛЫ
  if(recordNumber>=2 && recordNumber<=(2+AR_NUMBER_ANALOG_CANALES-1))
    {
      if(recordLen>27) return MARKER_ERRORPERIMETR;
      int subObj = recordNumber-2;//индекс канала
      __HEADER_AR *header_ar_tmp = NULL;

      if (pointInterface==USB_RECUEST)//метка интерфейса 0-USB 1-RS485
        header_ar_tmp = (__HEADER_AR*)buffer_for_USB_read_record_ar;
      else if (pointInterface==RS485_RECUEST)
        header_ar_tmp = (__HEADER_AR*)buffer_for_RS485_read_record_ar;
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
      else if (pointInterface == LAN_RECUEST)
        header_ar_tmp = (__HEADER_AR*)buffer_for_LAN_read_record_ar;
#endif
      else
      {
        //Теоретично цього ніколи не мало б бути
        total_error_sw_fixed();
      }

      char idetyficator[AR_NUMBER_ANALOG_CANALES][16] =
      {
        "3I0             ",
        "Ia              ",
        "                ",
        "Ic              ",
        "                ",
        "                ",
        "                ",
        "3U0             ",
        "  f             "
      };
      static const char idetyficator_current[2][16] =
      {
        "Ib              ",
        "I 0.4kV         "
      };

      unsigned int phase_line = header_ar_tmp->control_extra_settings_1 & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE);
      unsigned int Ib_I04 = header_ar_tmp->control_extra_settings_1 & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_IB_I04);
      for (unsigned int k = 0; k < 16; k++) idetyficator[2][k] = idetyficator_current[Ib_I04 != 0][k];

      if (phase_line == 0)
        {
          static const char idetyficator_phase[3][16] =
          {
            "Ua              ",
            "Ub              ",
            "Uc              "
          };
          char *point_to_changed_name = idetyficator[I_Ua + 0];
          for (unsigned int j = 0; j < 3; j++)
            {
              for (unsigned int k = 0; k < 16; k++) *(point_to_changed_name + j*16 + k) = idetyficator_phase[j][k];
            }
        }
      else
        {
          static const char idetyficator_line[3][16] =
          {
            "Uab             ",
            "Ubc             ",
            "Uca             "
          };
          char *point_to_changed_name = idetyficator[I_Ua + 0];
          for (unsigned int j = 0; j < 3; j++)
            {
              for (unsigned int k = 0; k < 16; k++) *(point_to_changed_name + j*16 + k) = idetyficator_line[j][k];
            }
        }

      switch(offsetRegister)
        {
        case 0://Порядковый номер аналогового канала offsetRegister
          return 1+subObj;
        case 1://Идентификатор канала0 offsetRegister
          return idetyficator[subObj][0] | (idetyficator[subObj][1]<<8);
        case 2://Идентификатор канала1 offsetRegister
          return idetyficator[subObj][2] | (idetyficator[subObj][3]<<8);
        case 3://Идентификатор канала2 offsetRegister
          return idetyficator[subObj][4] | (idetyficator[subObj][5]<<8);
        case 4://Идентификатор канала3 offsetRegister
          return idetyficator[subObj][6] | (idetyficator[subObj][7]<<8);
        case 5://Идентификатор канала4 offsetRegister
          return idetyficator[subObj][8] | (idetyficator[subObj][9]<<8);
        case 6://Идентификатор канала5 offsetRegister
          return idetyficator[subObj][10] | (idetyficator[subObj][11]<<8);
        case 7://Идентификатор канала6 offsetRegister
          return idetyficator[subObj][12] | (idetyficator[subObj][13]<<8);
        case 8://Идентификатор канала7 offsetRegister
          return idetyficator[subObj][14] | (idetyficator[subObj][15]<<8);
        case 9://Фаза канала offsetRegister
        {
          //Фаза каналу - 2 ASCII символів
          char phase[AR_NUMBER_ANALOG_CANALES][2] =
          {
            "I0",
            "A ",
            "  ",
            "C ",
            "  ",
            "  ",
            "  ",
            "U0",
            "  "
          };

          {
            static const char phase_c[2][2] =
            {
              "B ",
              "04"
            };
            for (unsigned int k = 0; k < 2; k++) phase[I_Ib_I04][k] = phase_c[Ib_I04 != 0][k];
          }

          if (phase_line == 0)
            {
              static const char phase_p[3][2] =
              {
                "A ",
                "B ",
                "C "
              };
              char *point_to_changed_name = phase[I_Ua + 0];
              for (unsigned int j = 0; j < 3; j++)
                {
                  for (unsigned int k = 0; k < 2; k++) *(point_to_changed_name + j*2 + k) = phase_p[j][k];
                }
            }
          else
            {
              static const char pase_l[3][2] =
              {
                "AB",
                "BC",
                "CA"
              };
              char *point_to_changed_name = phase[I_Ua + 0];
              for (unsigned int j = 0; j < 3; j++)
                {
                  for (unsigned int k = 0; k < 2; k++) *(point_to_changed_name + j*2 + k) = pase_l[j][k];
                }
            }

          return phase[subObj][0] | (phase[subObj][1]<<8);
        }//case 9
        case 10://Наблюдаемый компонент цепи0 offsetRegister
        case 11://Наблюдаемый компонент цепи1 offsetRegister
        case 12://Наблюдаемый компонент цепи2 offsetRegister
        case 13://Наблюдаемый компонент цепи3 offsetRegister
        case 14://Наблюдаемый компонент цепи4 offsetRegister
        case 15://Наблюдаемый компонент цепи5 offsetRegister
        case 16://Наблюдаемый компонент цепи6 offsetRegister
        case 17://Наблюдаемый компонент цепи7 offsetRegister
        {
          //Спостережний елемент в колі - 16 ASCII символів - нічого тут не передаємо, тому у це поле поміщаємо ' '
          return ((' ')<<8) | (' ');
        }//case 10-17
        case 18://Единицы измерения offsetRegister
        {
          //Одиниці вимірювання - 2 ASCII символів
          unsigned int const amp  = ('A'|(' '<<8));
          unsigned int const volt = ('V'|(' '<<8));
          unsigned int const Hz_string = ('H'|('z'<<8));
          if (subObj <= I_Ic) return amp;
          else if (subObj <= I_3U0) return volt;
          return Hz_string;
        }//case 18
        case 19://Коэффициент канала offsetRegister
        {
          //Коефіцієнт каналу
          if      (subObj == I_3I0) return (1000*MNOGNYK_3I0) >> (VAGA_DILENNJA_3I0 + 8);
          else if (subObj <= I_Ic ) return (1000*MNOGNYK_I)   >> (VAGA_DILENNJA_I   + 4);
          else if (subObj <= I_3U0) return (1000*MNOGNYK_U)   >> (VAGA_DILENNJA_U   + 4);
          else return (10000); //щоб при ділення на 1000000 отримати 0,01
        }//case 19
        case 20://Добавочное смещение offsetRegister
          //Додаткове зміщення
          return 0;
        case 21://Сдвиг времени канала (µs) offsetRegister
          //Зсув у часі
          return 0; //Час зсуву відпосно початку зчитування
        case 22://Нижний предел значений offsetRegister
          //Нижня гнаниця
          return -32767;
        case 23://Верхний предел значений offsetRegister
          //Верхня границя
          return 32767;
        case 24://Первичный коэф. трансф. offsetRegister
        {
          //Первинний коефіцієнт трансформації
          if (subObj == I_3I0)
            {
              return  header_ar_tmp->T0;
            }
          else if (subObj <= I_Ic )
            {
              if (
                (subObj != I_Ib_I04 ) || (Ib_I04 == 0)
              )
                {
                  return   header_ar_tmp->TCurrent;
                }
              else
                {
                  return header_ar_tmp->TCurrent04;
                }
            }
          else if (subObj <= I_3U0 )
            {
              return header_ar_tmp->TVoltage;
            }
          else 
            {
              return 1;
            }
        }//case 24
        case 25://Вторичный коэф. трансф. offsetRegister
          //Вторинний коефіцієнт трансформації
          return 1;
        case 26://Первичный или вторичный идентификатор offsetRegister
          //Дані будуть передаватися з вторинної обмотки
          return 'S';
        }//switch
      return MARKER_ERRORPERIMETR;
    }//if(recordNumber>=2 && recordNumber<=(2+AR_NUMBER_ANALOG_CANALES))

//ДИСКРЕТНЫЕ КАНАЛЫ
  if(recordNumber>=((2+AR_NUMBER_ANALOG_CANALES)) && recordNumber<=(((2+AR_NUMBER_ANALOG_CANALES)) +DISKRET_TOTAL)-1)
    {
      if(recordLen>19) return MARKER_ERRORPERIMETR;
      int subObj = recordNumber-((2+AR_NUMBER_ANALOG_CANALES));//индекс канала
      return recordNumberCaseDiskret(subObj, offsetRegister);

    }//if(recordNumber>=(2+AR_NUMBER_ANALOG_CANALES +1) && recordNumber<=(2+AR_NUMBER_ANALOG_CANALES +1 +100))

//ОСТАЛЬНЫЕ
  if(recordNumber>=(((2+AR_NUMBER_ANALOG_CANALES)) +DISKRET_TOTAL) && recordNumber<=((((2+AR_NUMBER_ANALOG_CANALES)) +DISKRET_TOTAL) +7)-1)
    {
      int subObj = recordNumber-(((2+AR_NUMBER_ANALOG_CANALES)) +DISKRET_TOTAL);//индекс записи
      return recordNumberCaseOther(subObj, offsetRegister, recordLen, ANALOG_REGISTRATOR);

    }//if(recordNumber>=(2+AR_NUMBER_ANALOG_CANALES +1 +DISKRET_TOTAL +1) && recordNumber<=(2+AR_NUMBER_ANALOG_CANALES +1 +DISKRET_TOTAL +1 +7))

  return MARKER_ERRORPERIMETR;
}//configAnalogRegistrator(int offsetRegister, int recordNumber, int recordLen)

int configDiskretRegistrator(int offsetRegister, int recordNumber, int recordLen)
{
//Конфигурация дискретного регистратора
  switch(recordNumber)
    {
    case 0://Имя станции recordNumber
    case 1://Количество каналов recordNumber
      return recordNumberCase0Case1(offsetRegister, recordNumber, recordLen, DISKRET_REGISTRATOR);
    }//switch

//ДИСКРЕТНЫЕ КАНАЛЫ
  if(recordNumber>=2 && recordNumber<=(2+DISKRET_TOTAL)-1)
    {
      if(recordLen>19) return MARKER_ERRORPERIMETR;
      int subObj = recordNumber-2;//индекс канала
      return recordNumberCaseDiskret(subObj, offsetRegister);

    }//if(recordNumber>=2 && recordNumber<=(2+DISKRET_TOTAL)-1)

//ОСТАЛЬНЫЕ
  if(recordNumber>=((2+DISKRET_TOTAL)) && recordNumber<=(((2+DISKRET_TOTAL)) +7)-1)
    {
      int subObj = recordNumber-((2+DISKRET_TOTAL));//индекс записи
      return recordNumberCaseOther(subObj, offsetRegister, recordLen, DISKRET_REGISTRATOR);

    }//if(recordNumber>=((2+DISKRET_TOTAL)) && recordNumber<=(((2+DISKRET_TOTAL)) +7)-1)

  return MARKER_ERRORPERIMETR;
}//configDiskretRegistrator(int offsetRegister, int recordNumber, int recordLen)

int recordNumberCase0Case1(int offsetRegister, int recordNumber, int recordLen, int registrator)
{
  //Якщо ми сюди дійшли, то вважаємо що заголовок аналогового реєстратора вже зчитаний
  __HEADER_AR *header_ar_tmp = NULL;

  if (pointInterface==USB_RECUEST)//метка интерфейса 0-USB 1-RS485
    header_ar_tmp = (__HEADER_AR*)buffer_for_USB_read_record_ar;
  else if (pointInterface==RS485_RECUEST)
    header_ar_tmp = (__HEADER_AR*)buffer_for_RS485_read_record_ar;
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
  else if (pointInterface == LAN_RECUEST)
        header_ar_tmp = (__HEADER_AR*)buffer_for_LAN_read_record_ar;
#endif
  else
  {
    //Теоретично цього ніколи не мало б бути
    total_error_sw_fixed();
  }
  switch(recordNumber)
    {
    case 0://Имя станции recordNumber
    {

      if(recordLen>10) return MARKER_ERRORPERIMETR;
      if(registrator==ANALOG_REGISTRATOR)
        {
          switch(offsetRegister)
            {
            case 0://Имя станции offsetRegister
              return header_ar_tmp->name_of_cell[ 0] | (header_ar_tmp->name_of_cell[ 1]<<8);
            case 1://Имя станции offsetRegister
              return header_ar_tmp->name_of_cell[ 2] | (header_ar_tmp->name_of_cell[ 3]<<8);
            case 2://Имя станции offsetRegister
              return header_ar_tmp->name_of_cell[ 4] | (header_ar_tmp->name_of_cell[ 5]<<8);
            case 3://Имя станции offsetRegister
              return header_ar_tmp->name_of_cell[ 6] | (header_ar_tmp->name_of_cell[ 7]<<8);
            case 4://Имя станции offsetRegister
              return header_ar_tmp->name_of_cell[ 8] | (header_ar_tmp->name_of_cell[ 9]<<8);
            case 5://Имя станции offsetRegister
              return header_ar_tmp->name_of_cell[ 10] | (header_ar_tmp->name_of_cell[ 11]<<8);
            case 6://Имя станции offsetRegister
              return header_ar_tmp->name_of_cell[ 12] | (header_ar_tmp->name_of_cell[ 13]<<8);
            case 7://Имя станции offsetRegister
              return header_ar_tmp->name_of_cell[ 14] | (header_ar_tmp->name_of_cell[ 15]<<8);
            case 8://Идентиф. номер устройства offsetRegister
              //Серійний номер пристрою завжди один і той самий (встановлюється на заводі-виготовнику), тому я його не включив для запису у DataFlash у складі заголовку аналогового реєстратора
              return serial_number_dev & 0xffff;
            case 9://Год редакции стандарта (1999) offsetRegister
              return 1999;

            }//switch
        }//if(registrator==ANALOG_REGISTRATOR)
      else
        {
          //if(registrator==DISKRET_REGISTRATOR)
          unsigned char *point_to_buffer = NULL;
          if (pointInterface==USB_RECUEST)//метка интерфейса 0-USB 1-RS485
            point_to_buffer = buffer_for_USB_read_record_dr;
          else if (pointInterface==RS485_RECUEST) point_to_buffer = buffer_for_RS485_read_record_dr;
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
          else if (pointInterface == LAN_RECUEST) point_to_buffer = buffer_for_LAN_read_record_dr;
#endif
          else
          {
            //Теоретично цього ніколи не мало б бути
            total_error_sw_fixed();
          }

          if (offsetRegister < 8)
            {
              unsigned int index_cell;

              index_cell =  (offsetRegister - 0)<<1;
              return (*(point_to_buffer + FIRST_INDEX_NAME_OF_CELL_DR + index_cell)) | ((*(point_to_buffer + FIRST_INDEX_NAME_OF_CELL_DR + index_cell + 1))<<8);
            }
          else if (offsetRegister == 8)
            {
              //Серійний номер пристрою завжди один і той самий (встановлюється на заводі-виготовнику), тому я його не включив для запису у DataFlash у складі заголовку аналогового реєстратора
              return serial_number_dev & 0xffff;
            }
          else
            {
              return 1999;
            }
        }//else if(registrator==DISKRET_REGISTRATOR)
    }//case 0
    break;

    case 1://Количество каналов recordNumber
    {
      if(recordLen>3) return MARKER_ERRORPERIMETR;
      if(registrator==ANALOG_REGISTRATOR)
        {
          switch(offsetRegister)
            {
            case 0://Количество каналов всего offsetRegister
              return AR_NUMBER_ANALOG_CANALES + NUMBER_TOTAL_SIGNAL_FOR_RANG;
            case 1://Количество аналоговых каналов offsetRegister
              return AR_NUMBER_ANALOG_CANALES;
            case 2://Количество дискретных каналов offsetRegister
              return NUMBER_TOTAL_SIGNAL_FOR_RANG;
            }//switch
          return MARKER_ERRORPERIMETR;
        }// if(registrator==ANALOG_REGISTRATOR)
      else
        {
          //if(registrator==DISKRET_REGISTRATOR)
          switch(offsetRegister)
            {
            case 0://Количество каналов всего offsetRegister
              return   0 + NUMBER_TOTAL_SIGNAL_FOR_RANG;
            case 1://Количество аналоговых каналов offsetRegister
              return 0;
            case 2://Количество дискретных каналов offsetRegister
              return NUMBER_TOTAL_SIGNAL_FOR_RANG;
            }//switch

        }//else
    }
    break;

    }//switch
  return MARKER_ERRORPERIMETR;
}//recordNumberCase0Case1(int offsetRegister, int recordNumber, int recordLen)

int recordNumberCaseDiskret(int subObj, int offsetRegister)
{
  switch(offsetRegister)
    {
    case 0://Порядковый номер дискретного канала offsetRegister
      return 1+subObj;
    case 1://Идентификатор канала0 offsetRegister
    case 2://Идентификатор канала1 offsetRegister
    case 3://Идентификатор канала2 offsetRegister
    case 4://Идентификатор канала3 offsetRegister
    case 5://Идентификатор канала4 offsetRegister
    case 6://Идентификатор канала5 offsetRegister
    case 7://Идентификатор канала6 offsetRegister
    case 8://Идентификатор канала7 offsetRegister
    {
//      const uint32_t index_number_UP[MAX_NAMBER_LANGUAGE][3] =
//      {
//        {11, 10, 8},
//        {11, 10, 8},
//        { 6,  6, 9},
//        {11, 10, 8}
//      };
//
      int index_language = index_language_in_array(current_settings.language);

      unsigned int index_cell =  (offsetRegister - 1)<<1;
      unsigned int temp_data = 0;
      for (size_t sym = 0; sym < 2; sym++)
        {
          size_t index_row;
          if (subObj < NUMBER_GENERAL_SIGNAL_FOR_RANG) 
          {
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
            if (subObj < (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_IN_GOOSE + N_IN_MMS + N_OUT_LAN))) 
            {
              index_row = subObj;
            }
            else if (subObj < (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_IN_MMS + N_OUT_LAN))) 
            {
              index_row = RANG_BLOCK_IN_GOOSE1 + ((subObj - (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_IN_GOOSE + N_IN_MMS + N_OUT_LAN))) % 1);
            }
            else if (subObj < (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_OUT_LAN))) 
            {
              index_row = RANG_BLOCK_IN_MMS1 + (1 - N_IN_GOOSE) + ((subObj - (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_IN_MMS + N_OUT_LAN))) % 1);
            }
            else
            {
              index_row = RANG_BLOCK_OUT_LAN1 + (1 - N_IN_GOOSE) + (1 - N_IN_MMS) + ((subObj - (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_OUT_LAN))) % 1);
            }
#else
            index_row = subObj;
#endif        
          }
          else if (subObj < RANG_BLOCK_UP1) 
          {
            index_row = subObj
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                         + 1 - N_IN_GOOSE + 1 - N_IN_MMS + 1 - N_OUT_LAN
#endif        
                         ;
          }
          else if (subObj < (RANG_BLOCK_UP1 + NUMBER_UP_SIGNAL_FOR_RANG))
          {
            index_row = RANG_BLOCK_UP1
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                         + 1 - N_IN_GOOSE + 1 - N_IN_MMS + 1 - N_OUT_LAN
#endif
                         + ((subObj - RANG_BLOCK_UP1) % 3);
          }
          else
          {
            index_row = subObj 
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                        + 1 - N_IN_GOOSE + 1 - N_IN_MMS + 1 - N_OUT_LAN
#endif        
                        + 3 - NUMBER_UP_SIGNAL_FOR_RANG;
          }

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
          if (
              (subObj >= (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_IN_GOOSE + N_IN_MMS + N_OUT_LAN)))  &&
              (subObj <  (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_IN_MMS + N_OUT_LAN))) &&
              ((index_cell + sym) >=  index_n_In_GOOSE[index_language][(subObj - RANG_BLOCK_IN_GOOSE1) % 1]) &&
              ((index_cell + sym) <= (index_n_In_GOOSE[index_language][(subObj - RANG_BLOCK_IN_GOOSE1) % 1] + 1)) 
             )   
          {
            unsigned int n = subObj - RANG_BLOCK_IN_GOOSE1;
            if ((n + 1) < 10)
            {
              if ((index_cell + sym) == index_n_In_GOOSE[index_language][n % 1])
                temp_data |= (0x30 + (n + 1)) << (8*sym);
              else
                temp_data |= (' ') << (8*sym);
            }
            else
            {
              if ((index_cell + sym) == index_n_In_GOOSE[index_language][n % 1])
                temp_data |= (0x30 + (n / 1 + 1) / 10) << (8*sym);
              else
                temp_data |= (0x30 + (n / 1 + 1) % 10) << (8*sym);
            }
          }
          else if (
                   (subObj >= (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_IN_MMS + N_OUT_LAN)))  &&
                   (subObj <  (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_OUT_LAN))) &&
                   ((index_cell + sym) == index_n_In_MMS[index_language][(subObj - RANG_BLOCK_IN_MMS1) % 1]) 
                  )   
          {
            temp_data |= (0x30 + ((subObj - RANG_BLOCK_IN_MMS1) / 1 + 1)) << (8*sym);
          }
          else if (
                   (subObj >= (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_OUT_LAN)))  &&
                   (subObj <  (NUMBER_GENERAL_SIGNAL_FOR_RANG)) &&
                   ((index_cell + sym) == index_n_Out_LAN[index_language][(subObj - RANG_BLOCK_OUT_LAN1) % 1]) 
                  )   
          {
            temp_data |= (0x30 + ((subObj - RANG_BLOCK_OUT_LAN1) / 1 + 1)) << (8*sym);
          }
          else 
#endif
          {
            if (
                (subObj >=  RANG_BLOCK_UP1)  &&
                (subObj <  (RANG_BLOCK_UP1 + NUMBER_UP_SIGNAL_FOR_RANG)) &&
                ((index_cell + sym) == index_number_UP[index_language][(subObj - RANG_BLOCK_UP1) % 3]) 
               )   
            {
              temp_data |= (0x30 + ((subObj - RANG_BLOCK_UP1) / 3 + 1)) << (8*sym);
            }
            else temp_data |= (idetyficator_rang[index_language][index_row][(index_cell + sym)]) << (8*sym);
          }
        }
      return temp_data;
    }//case 1-8
    case 9://Фаза канала offsetRegister
      //Фаза каналу - 2 ASCII символів - нічого не передаємо
      return (' '<<8) | ' ';
    case 10://Наблюдаемый компонент цепи0 offsetRegister
    case 11://Наблюдаемый компонент цепи1 offsetRegister
    case 12://Наблюдаемый компонент цепи2 offsetRegister
    case 13://Наблюдаемый компонент цепи3 offsetRegister
    case 14://Наблюдаемый компонент цепи4 offsetRegister
    case 15://Наблюдаемый компонент цепи5 offsetRegister
    case 16://Наблюдаемый компонент цепи6 offsetRegister
    case 17://Наблюдаемый компонент цепи7 offsetRegister
    {
      //Спостережний елемент в колі - 16 ASCII символів- нічого не передаємо
      return (' '<<8) | ' ';
    }//case 10-18
    case 18://Нормальное состояние offsetRegister
      //Нормальний стан
      return 0;
    }//switch
  return MARKER_ERRORPERIMETR;
}//recordNumberCaseDiskret(int subObj, int offsetRegister, int recordLen)

int recordNumberCaseOther(int subObj, int offsetRegister, int recordLen, int registrator)
{
  int *point_to_first_number_time_sample = NULL, *point_to_last_number_time_sample = NULL;
  char *point_id_ar_record = NULL;
  int *point_to_max_number_time_sample = NULL;
  unsigned char *point_to_Buffer = NULL;
  
  __HEADER_AR *header_ar_tmp = NULL;

  if(registrator == ANALOG_REGISTRATOR)
  {
    if (pointInterface == USB_RECUEST)
    {
      point_id_ar_record = id_ar_record_for_USB;
      point_to_first_number_time_sample = &first_number_time_sample_for_USB;
      point_to_last_number_time_sample  = &last_number_time_sample_for_USB;
      point_to_max_number_time_sample = &max_number_time_sample_USB;
          
      point_to_Buffer = buffer_for_USB_read_record_ar;
    }
    else if (pointInterface == RS485_RECUEST)
    {
      point_id_ar_record = id_ar_record_for_RS485;
      point_to_first_number_time_sample = &first_number_time_sample_for_RS485;
      point_to_last_number_time_sample  = &last_number_time_sample_for_RS485;
      point_to_max_number_time_sample = &max_number_time_sample_RS485;
          
      point_to_Buffer = buffer_for_RS485_read_record_ar;
    }
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
    else if (pointInterface == LAN_RECUEST)
    {
      point_id_ar_record = id_ar_record_for_LAN;
      point_to_first_number_time_sample = &first_number_time_sample_for_LAN;
      point_to_last_number_time_sample  = &last_number_time_sample_for_LAN;
      point_to_max_number_time_sample = &max_number_time_sample_LAN;
          
      point_to_Buffer = buffer_for_LAN_read_record_ar;
    }
#endif
    else
    {
      //Теоретично цього ніколи не мало б бути
      total_error_sw_fixed();
    }
  
    //Перевіряємо чи зчитано заголовок аналогового реєстратора
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
            res = f_read(&fil, point_to_Buffer, ByteToRead, &read_tmp);
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
              
    header_ar_tmp = (__HEADER_AR*)point_to_Buffer;
  }
  else if (registrator == DISKRET_REGISTRATOR)
  {
    if (pointInterface == USB_RECUEST) //метка интерфейса 0-USB 1-RS485
      point_to_Buffer = buffer_for_USB_read_record_dr;
    else if (pointInterface == RS485_RECUEST)
      point_to_Buffer = buffer_for_RS485_read_record_dr;
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
    else if (pointInterface == LAN_RECUEST) point_to_Buffer = buffer_for_LAN_read_record_dr;
#endif
    else
    {
      //Теоретично цього ніколи не мало б бути
      total_error_sw_fixed();
    }
  }
  else return MARKER_ERRORPERIMETR;
    
  switch(subObj)
    {
    case 0://Частота линии (Гц) subObj
      if(recordLen>1) return MARKER_ERRORPERIMETR;
      return 5000;
    case 1://Кол-во частот дискретизации subObj
      if(recordLen>1) return MARKER_ERRORPERIMETR;
      if(registrator==ANALOG_REGISTRATOR) return 1;
      else return 0;
    case 2://Частота выборки1 (Гц) subObj
    {
      if(recordLen>2) return MARKER_ERRORPERIMETR;
      switch(offsetRegister)
        {
        case 0://Частота выборки1 (Гц) offsetRegister
          if(registrator==ANALOG_REGISTRATOR) return ((50*1000)<<VAGA_NUMBER_POINT_AR)>>4; //Чаcтота
          return 0;
        case 1://Последняя выборка на этой частоте offsetRegister
          if(registrator==ANALOG_REGISTRATOR) return *point_to_max_number_time_sample; //Частота виборки
          return *(point_to_Buffer + FIRST_INDEX_NUMBER_ITEMS_DR); //остання виборка на даній чатоті дискретизації рівна останній
        }//switch

      return MARKER_ERRORPERIMETR;
    }//case 2
    case 3://день (Дата и время первых данных) subObj
    case 4://день (Дата и время запуска) subObj
    {
      if(recordLen>7) return MARKER_ERRORPERIMETR;
      switch(offsetRegister)
        {
        case 0://день (Дата и время первых данных) offsetRegister
        case 1://месяц offsetRegister
        case 2://год offsetRegister
        case 3://часы offsetRegister
        case 4://минуты offsetRegister
        case 5://секунды offsetRegister
        case 6://микросекунды offsetRegister
        {
          if(registrator==ANALOG_REGISTRATOR)
            {
              if (header_ar_tmp == NULL) total_error_sw_fixed();
              
              time_t time_dat_tmp = header_ar_tmp->time_dat;
              int32_t time_ms_tmp = header_ar_tmp->time_ms;

              struct tm *p;
              int temp_data;

              int max_time_milliseconds_before = (header_ar_tmp->prefault_number_periods)*20;
              int s, ds_ms;

              if(subObj==4)  goto m1;// return time_avar_analog[offsetRegister];

              //Кількість секунд
              s = max_time_milliseconds_before / 1000;
              //Кількість мілісекунд
              ds_ms = max_time_milliseconds_before - s*1000;
              
              time_ms_tmp -= ds_ms;
              time_dat_tmp -= (time_t)s;

              if (time_ms_tmp < 0) 
              {
                time_ms_tmp += 1000;
                --time_dat_tmp;
              }

m1:
              p = localtime(&time_dat_tmp);
              switch(offsetRegister)
                {
                case 0:
                  {
                    temp_data = p->tm_mday;
                    break;
                  }
                case 1:
                  {
                    temp_data = p->tm_mon + 1;
                    break;
                  }
                case 2:
                  {
                    temp_data = p->tm_year + 1900;
                    break;
                  }
                case 3:
                  {
                    temp_data = p->tm_hour;
                    break;
                  }
                case 4:
                  {
                    temp_data = p->tm_min;
                    break;
                  }
                case 5:
                  {
                    temp_data = p->tm_sec*100 + time_ms_tmp/10;
                    break;
                  }
                case 6:
                  {
                    temp_data = (time_ms_tmp % 10) * 1000;
                    break;
                  }
                default:
                  temp_data = 0;
                }//switch
              return temp_data;
            }//if(registrator==ANALOG_REGISTRATOR)
          else
            {
              //if(registrator==DISKRET_REGISTRATOR)
              //Конвертуємо формат BCD у int
              time_t time_dat_tmp = 0;
              int32_t time_ms_tmp = 0;

              for (unsigned int i = 0; i < sizeof(time_t); i++) *((unsigned char *)(&time_dat_tmp) + i) = point_to_Buffer[FIRST_INDEX_DATA_TIME_DR + i];
              for (unsigned int i = 0; i < sizeof(int32_t); i++) *((unsigned char *)(&time_ms_tmp) + i) = point_to_Buffer[FIRST_INDEX_DATA_TIME_DR + sizeof(time_t) + i];

               if(subObj==3)
               {
                //(Дата и время первых данных)
             //данные Diskret регистратора
               unsigned char *point_to_buffer = NULL;
               if (pointInterface==USB_RECUEST)//метка интерфейса 0-USB 1-RS485
                 point_to_buffer = buffer_for_USB_read_record_dr;
               else if (pointInterface==RS485_RECUEST) point_to_buffer = buffer_for_RS485_read_record_dr;
             #if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
               else if (pointInterface == LAN_RECUEST) point_to_buffer = buffer_for_LAN_read_record_dr;
             #endif
               else
               {
                 //Теоретично цього ніколи не мало б бути
                 total_error_sw_fixed();
               }
               unsigned char time_before_dr = (*(point_to_buffer + FIRST_INDEX_NUMBER_BEFORE_ITEMS_DR));// тривалість доаваріного масиву

                 time_ms_tmp -= (int32_t)time_before_dr;
                 if(time_ms_tmp < 0)
                 {
                   time_ms_tmp += 1000;
                   --time_dat_tmp;
                 }//if
                }//if(subObj==3)
              struct tm *p = localtime(&time_dat_tmp);
              int temp_data;
              switch(offsetRegister)
                {
                case 0:
                  {
                    temp_data = p->tm_mday;
                    break;
                  }
                case 1:
                  {
                    temp_data = p->tm_mon + 1;
                    break;
                  }
                case 2:
                  {
                    temp_data = p->tm_year + 1900;
                    break;
                  }
                case 3:
                  {
                    temp_data = p->tm_hour;
                    break;
                  }
                case 4:
                  {
                    temp_data = p->tm_min;
                    break;
                  }
                case 5:
                  {
                    temp_data = p->tm_sec*100 + time_ms_tmp/10;
                    break;
                  }
                case 6:
                  {
                    temp_data = (time_ms_tmp % 10) * 1000;
                    break;
                  }
                default:
                  temp_data = 0;
                }//switch
              return temp_data;
            } //else
        }//case 0-6
        }//switch
      return MARKER_ERRORPERIMETR;
    }//case 3 4
    case 5://Тип данных subObj
      if(recordLen>1) return MARKER_ERRORPERIMETR;
      //if(registrator==DISKRET_REGISTRATOR) 
      return 'B'; //дані - це бінарні числа
      //return (1000000 >> VAGA_NUMBER_POINT_AR)/50; //Період дискретизації
    case 6://Коэффициент временной метки subObj
      if(recordLen>1) return MARKER_ERRORPERIMETR;
    if(registrator==DISKRET_REGISTRATOR) return 1000;
    return (1000000 >> VAGA_NUMBER_POINT_AR)/50;
    }//switch

  return MARKER_ERRORPERIMETR;
}//

int dataDiskretRegistrator(int offsetRegister, int recordNumber, int recordLen)
{
//данные Diskret регистратора
  unsigned char *point_to_buffer = NULL;
  if (pointInterface==USB_RECUEST)//метка интерфейса 0-USB 1-RS485
    point_to_buffer = buffer_for_USB_read_record_dr;
  else if (pointInterface==RS485_RECUEST) point_to_buffer = buffer_for_RS485_read_record_dr;
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
  else if (pointInterface == LAN_RECUEST) point_to_buffer = buffer_for_LAN_read_record_dr;
#endif
  else
  {
    //Теоретично цього ніколи не мало б бути
    total_error_sw_fixed();
  }

  unsigned int max_number_two_bytes = (NUMBER_TOTAL_SIGNAL_FOR_RANG >> 4);
  if ((max_number_two_bytes << 4) != NUMBER_TOTAL_SIGNAL_FOR_RANG)  max_number_two_bytes++;

  unsigned int lenMax = (3 + max_number_two_bytes);
  if ((unsigned int)recordLen > lenMax) return MARKER_ERRORPERIMETR;//уйти если превышение

  if(recordNumber>(*(point_to_buffer + FIRST_INDEX_NUMBER_ITEMS_DR))) return MARKER_ERRORPERIMETR;//уйти если превышение

  int const time_before_dr = point_to_buffer[FIRST_INDEX_NUMBER_BEFORE_ITEMS_DR];// тривалість доаваріного масиву
  unsigned int const offset = FIRST_INDEX_FIRST_DATA_DR + (recordNumber + 1)*SD_DR; //бо найперший запис містить попереднє значення (до фіксації запуску роботи дискретного реєстратора)

	int time_marker_dr = (int)(
														 (unsigned int)(point_to_buffer[offset + 0]      ) + 
														 (unsigned int)(point_to_buffer[offset + 1] <<  8) + 
														 (unsigned int)(point_to_buffer[offset + 2] << 16) + 
														               ((point_to_buffer[offset + 2] & 0x80) ? 0xff000000u : 0x0u) 
													  );
  time_marker_dr += time_before_dr;

  switch(offsetRegister)
    {
    case 0://порядковый номер выборки
      return recordNumber + 1;
    case 1://метка времени (ms)
      return time_marker_dr&0xFFFF;//(*(point_to_buffer + offset + 0)) + ((*(point_to_buffer + offset + 1)) << 8 );
    case 2://метка времени (ms)
      return (time_marker_dr>>16)&0xFFFF;//*(point_to_buffer + offset + 2);
    }//switch

  return (*(point_to_buffer + offset + 3 + (offsetRegister - 3)*2)) + ((*(point_to_buffer + offset + 4 + (offsetRegister - 3)*2)) << 8);
}//dataDiskretRegistrator(int offsetRegister, int recordNumber, int recordLen)
