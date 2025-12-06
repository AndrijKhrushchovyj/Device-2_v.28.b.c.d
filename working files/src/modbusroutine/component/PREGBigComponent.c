#include "header.h"

//начальный регистр в карте памяти
#define BEGIN_ADR_REGISTER 61800
//конечный регистр в карте памяти
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
#define END_ADR_REGISTER 61824
#else
#define END_ADR_REGISTER 61822
#endif

int privatePREGBigGetReg2(int adrReg);

int getPREGBigModbusRegister(int);//получить содержимое регистра
int getPREGBigModbusBit(int);//получить содержимое бита
int setPREGBigModbusRegister(int, int);//получить содержимое регистра
int setPREGBigModbusBit(int, int);//получить содержимое бита

int  postPREGBigWriteAction(void);//action после записи

SRAM1 COMPONENT_OBJ *pregbigcomponent;

/**************************************/
//подготовка компонента REGistrator prog
/**************************************/
void constructorPREGBigComponent(COMPONENT_OBJ *pregbigcomp)
{
  pregbigcomponent = pregbigcomp;

  pregbigcomponent->getModbusRegister = getPREGBigModbusRegister;//получить содержимое регистра
  pregbigcomponent->getModbusBit      = getPREGBigModbusBit;//получить содержимое бита
  pregbigcomponent->setModbusRegister = setPREGBigModbusRegister;//получить содержимое регистра
  pregbigcomponent->setModbusBit      = setPREGBigModbusBit;//получить содержимое бита

  pregbigcomponent->postWriteAction = postPREGBigWriteAction;//action после записи
}//prepareDVinConfig

int getPREGBigModbusRegister(int adrReg)
{
  //получить содержимое регистра
  if(privatePREGBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  int offset = adrReg-BEGIN_ADR_REGISTER;
  switch(offset)
    {
    case 0://Очистить регистратор программных ошибок
      return MARKER_ERRORPERIMETR;
    case 1://Номер аварии доступной для чтения
      if(pointInterface==USB_RECUEST)//метка интерфейса 0-USB 1-RS485 2-LAN
        return (number_record_of_pr_err_into_USB) &0xFFFF;
      if(pointInterface==RS485_RECUEST)//метка интерфейса 0-USB 1-RS485 2-LAN
        return (number_record_of_pr_err_into_RS485) &0xFFFF;
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
      if(pointInterface==LAN_RECUEST) return (number_record_of_pr_err_into_LAN) &0xFFFF;
#endif  
      //Теоретично сюди ніколи не мала б програма зайти
      total_error_sw_fixed();
    case 2://Количество событий
      return (info_rejestrator_pr_err.number_records) &0xFFFF;
    case 3://Окно отображения страницы
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
    case 23:
    case 24:
#endif      
      if (
        ((pointInterface==USB_RECUEST) && (number_record_of_pr_err_into_USB   == 0xffff)) 
        ||
        ((pointInterface==RS485_RECUEST) && (number_record_of_pr_err_into_RS485 == 0xffff))
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)        
        ||
        ((pointInterface==LAN_RECUEST) && (number_record_of_pr_err_into_LAN == 0xffff))
#endif
      ) return MARKER_ERRORPERIMETR;
      
      if (
        ((clean_rejestrators & CLEAN_PR_ERR) != 0) ||
        (
          ((pointInterface==USB_RECUEST  ) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_USB  ) != 0)) 
          ||
          ((pointInterface==RS485_RECUEST  ) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_RS485) != 0))
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)        
          ||
          ((pointInterface==LAN_RECUEST  ) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_LAN) != 0))
#endif
        )
      ) return MARKER_ERRORPERIMETR;
      if (
        (
          (pointInterface==USB_RECUEST)//метка интерфейса 0-USB 1-RS485 2-LAN
          &&
          (
            (number_record_of_pr_err_into_USB >= info_rejestrator_pr_err.number_records) ||
            (number_record_of_pr_err_into_USB >= MAX_NUMBER_RECORDS_INTO_PR_ERR        )
          )
        )
        ||
        (
          (pointInterface==RS485_RECUEST)//метка интерфейса 0-USB 1-RS485 2-LAN
          &&
          (
            (number_record_of_pr_err_into_RS485 >= info_rejestrator_pr_err.number_records) ||
            (number_record_of_pr_err_into_RS485 >= MAX_NUMBER_RECORDS_INTO_PR_ERR        )
          )
        )
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
        ||
        (
          (pointInterface==LAN_RECUEST)//метка интерфейса 0-USB 1-RS485 2-LAN
          &&
          (
            (number_record_of_pr_err_into_LAN >= info_rejestrator_pr_err.number_records) ||
            (number_record_of_pr_err_into_LAN >= MAX_NUMBER_RECORDS_INTO_PR_ERR        )
          )
        )
#endif          
      )
        {
          //Помічаємо, що номер запису не вибраний
          if (pointInterface==USB_RECUEST) number_record_of_pr_err_into_USB = 0xffff;
          else if (pointInterface==RS485_RECUEST) number_record_of_pr_err_into_RS485 = 0xffff;
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
          else if (pointInterface==LAN_RECUEST) number_record_of_pr_err_into_LAN = 0xffff;
#endif  
          else
          {
            //Теоретично цього ніколи не мало б бути
            total_error_sw_fixed();
          }
          return MARKER_ERRORPERIMETR;
        }//if
      
      else
        {
          unsigned char *point_to_buffer = NULL;
          if (pointInterface==USB_RECUEST) point_to_buffer = buffer_for_USB_read_record_pr_err;
          else if (pointInterface==RS485_RECUEST) point_to_buffer = buffer_for_RS485_read_record_pr_err;
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
          else if (pointInterface==LAN_RECUEST) point_to_buffer = buffer_for_LAN_read_record_pr_err;
#endif  
          else
          {
            //Теоретично цього ніколи не мало б бути
            total_error_sw_fixed();
          }

          if ( (*(point_to_buffer + 0)) != LABEL_START_RECORD_PR_ERR)
            {
              //зафіксовано недостовірні дані
              return MARKER_ERRORPERIMETR;
            }
          else
            {
              time_t time_dat_tmp = 0;
              int32_t time_ms_tmp = 0;
              for(size_t i = 0; i < sizeof(time_t); i++) *((unsigned char*)(&time_dat_tmp) + i) =  point_to_buffer[1 + i];
              for(size_t i = 0; i < sizeof(int32_t); i++) *((unsigned char*)(&time_ms_tmp) + i) = point_to_buffer[1 + sizeof(time_t) + i];
              time_ms_tmp *= 1000; //перевід у мкс
              
              int offset_tmp = offset - 3;
              switch (offset_tmp)//temp_address)
                {
                case 0:
                {
                  return (time_dat_tmp >> 0) & 0xFFFF;
                }
                case 1:
                {
                  return (time_dat_tmp >> 16) &0xFFFF;
                }
                case 2:
                {
                  return (time_dat_tmp >> 32) &0xFFFF;
                }
                case 3:
                {
                  return (time_dat_tmp >> 48) &0xFFFF;
                }
                case 4:
                {
                  return (time_ms_tmp >> 0) &0xFFFF;
                }
                case 5:
                {
                  return (time_ms_tmp >> 16) &0xFFFF;
                }
                case 6:
                case 7:
                case 8:
                case 9:
                case 10:
                case 11:
                case 12:
                case 13:
                case 14:
                case 15:
                case 16:
                case 17:
                case 18:
                case 19:
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                case 20:
                case 21:
#endif
                {
                  return (((*(point_to_buffer + 15 + 2*(offset_tmp - 6)))  << 8) | (*(point_to_buffer + 14 + 2*(offset_tmp - 6)))) &0xFFFF;
                }
                }//switch

            }//else
            }//else
          
          break;//case 3-8

        }//switch


      return 0;
    }//getDOUTBigModbusRegister(int adrReg)
  int getPREGBigModbusBit(int x)
  {
    //получить содержимое регистра
    UNUSED(x);
    return MARKER_OUTPERIMETR;
  }//getDOUTBigModbusRegister(int adrReg)
  int setPREGBigModbusRegister(int adrReg, int dataReg)
  {
    //записать содержимое регистра
    if(privatePREGBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

    superSetOperativMarker(pregbigcomponent, adrReg);
    superSetTempWriteArray(dataReg);//записать в буфер

    int offset = adrReg-BEGIN_ADR_REGISTER;

    switch(offset)
      {
      case 0://Очистить регистратор программных ошибок
        if (
          (current_ekran.current_level == EKRAN_CHANGES_DIAGNOSTICS_PR_ERR)
          ||
          (
            (control_tasks_dataflash & (
               TASK_WRITE_PR_ERR_RECORDS_INTO_DATAFLASH    |
               TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_USB   |
               TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_RS485 |
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
               TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_LAN |
#endif
               TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_MENU |
               TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_MENU_SHORT
             )
            ) != 0
          )
          ||
          ((clean_rejestrators & CLEAN_PR_ERR) != 0)
        ) return MARKER_ERRORPERIMETR;
        return 0;

      case 1://Номер аварии доступной для чтения
      {
        if (
//      Зараз іде зчитування для інтерфейсу запису реєстратора програмних подій,
//      або очистка його, тому ця операція є тимчасово недоступною
          ((clean_rejestrators & CLEAN_PR_ERR) != 0) ||
          (
            ((pointInterface == USB_RECUEST) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_USB  ) != 0)) 
            ||
            ((pointInterface == RS485_RECUEST) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_RS485) != 0))
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
            ||
            ((pointInterface == LAN_RECUEST) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_LAN) != 0))
#endif
          )
        ) return MARKER_ERRORPERIMETR;

        if (
          ((unsigned int)dataReg < info_rejestrator_pr_err.number_records) &&
          ((unsigned int)dataReg < MAX_NUMBER_RECORDS_INTO_PR_ERR        ) //Хоч теоретично ця умова має перекриватися завжди першою умовою
        ) return 0;
      }//case 1
      break;
      }//switch


    return MARKER_ERRORPERIMETR;
  }//getDOUTBigModbusRegister(int adrReg)
  int setPREGBigModbusBit(int x, int y)
  {
    UNUSED(x);
    UNUSED(y);
    //получить содержимое регистра
    return MARKER_OUTPERIMETR;
  }//getDOUTBigModbusRegister(int adrReg)

  int postPREGBigWriteAction(void)
  {
//action после записи
    int beginAdr = pregbigcomponent->operativMarker[0];
    if(beginAdr<0) return 0;//не было записи
    int endAdr   = pregbigcomponent->operativMarker[1];
    int countAdr = endAdr-beginAdr+1;
    if(endAdr<0) countAdr = 1;
    int offsetTempWriteArray = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//найти смещение TempWriteArray

    for(int i=0; i<countAdr; i++)
      {
        int offset = i+beginAdr-BEGIN_ADR_REGISTER;
        switch(offset)
          {
          case 0://Очистить регистратор программных ошибок
            //Помічаємо, що треба очистити реєстратор програмних подій
            clean_rejestrators |= CLEAN_PR_ERR;
            break;
          case 1://Номер аварии доступной для чтения
          {
            if(pointInterface==USB_RECUEST)//метка интерфейса 0-USB 1-RS485
              {
                //Встановлюємо номер запису реєстратора програмних подій для читання через інтерфейс USB
                number_record_of_pr_err_into_USB = (tempWriteArray[offsetTempWriteArray+i]);
                //Подаємо команду читання реєстратора програмних подій для  інтерфейсу USB

                //Подаємо команду зчитати дані у бувер пам'яті для USB
                control_tasks_dataflash |= TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_USB;
              }
            else if(pointInterface==RS485_RECUEST)//if (type_interface == RS485_RECUEST)
              {
                //Встановлюємо номер запису реєстратора програмних подій для читання через інтерфейс RS-485
                number_record_of_pr_err_into_RS485 = (tempWriteArray[offsetTempWriteArray+i]);
                //Подаємо команду читання реєстратора програмних подій для  інтерфейсу RS-485

                //Подаємо команду зчитати дані у бувер пам'яті для RS-485
                control_tasks_dataflash |= TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_RS485;
              }
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
            else if (pointInterface == LAN_RECUEST)
              {
                //Встановлюємо номер запису реєстратора програмних подій для читання через інтерфейс LAN
                number_record_of_pr_err_into_LAN = (tempWriteArray[offsetTempWriteArray+i]);
                //Подаємо команду читання реєстратора програмних подій для  інтерфейсу LAN

                //Подаємо команду зчитати дані у бувер пам'яті для LAN
                control_tasks_dataflash |= TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_LAN;
              }
#endif
            else
              {
                //Теоретично цього ніколи не мало б бути
                total_error_sw_fixed();
              }
          }//case 1
          break;
          }//switch
      }//for(int i=0; i<countAdr; i++)


    return 0;
  }//

  int privatePREGBigGetReg2(int adrReg)
  {
    //проверить внешний периметр
    return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
  }//privateGetReg2(int adrReg)


