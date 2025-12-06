
#include "header.h"

//начальный регистр в карте памяти
#define BEGIN_ADR_REGISTER 415
//конечный регистр в карте памяти
#define END_ADR_REGISTER 775

#define MMEASUREMENTS_DR_WIDTH                35
#define MM_ADDRESS_FIRST_MEASUREMENTS_DR       BEGIN_ADR_REGISTER

#define MDR_OFFSET_MEASUREMENT_UA_1            0
#define MDR_OFFSET_MEASUREMENT_UB_1            1
#define MDR_OFFSET_MEASUREMENT_UC_1            2
#define MDR_OFFSET_MEASUREMENT_IA_1            3
#define MDR_OFFSET_MEASUREMENT_IB_1            4
#define MDR_OFFSET_MEASUREMENT_IC_1            5
#define MDR_OFFSET_MEASUREMENT_I04_1           6
#define MDR_OFFSET_MEASUREMENT_UAB_1           7
#define MDR_OFFSET_MEASUREMENT_UBC_1           8
#define MDR_OFFSET_MEASUREMENT_UCA_1           9
#define MDR_OFFSET_MEASUREMENT_3U0_1           10
#define MDR_OFFSET_MEASUREMENT_3I0             11
#define MDR_OFFSET_MEASUREMENT_3I0_r           12
#define MDR_OFFSET_MEASUREMENT_3I0_HIGH_GARM   13
#define MDR_OFFSET_MEASUREMENT_I1              14
#define MDR_OFFSET_MEASUREMENT_I2              15
#define MDR_OFFSET_MEASUREMENT_IAH2            16
#define MDR_OFFSET_MEASUREMENT_IBH2            17
#define MDR_OFFSET_MEASUREMENT_ICH2            18

#define MDR_OFFSET_FREQUENCY_1                 19

//#define DR_OFFSET_MEASUREMENT_UAB_2           19
//#define DR_OFFSET_MEASUREMENT_UBC_2           20

#define MDR_OFFSET_MEASUREMENT_U1              28
#define MDR_OFFSET_MEASUREMENT_U2              29

//#define DR_OFFSET_FREQUENCY_2                 23

#define MDR_OFFSET_VMP                         32

#define MDR_OFFSET_LABEL_TIME_LSW              33
#define MDR_OFFSET_LABEL_TIME_MSW              34
#define MDR_OFFSET_LABEL_PROTECT               35

#define MBLOCK_PROTECTION_MTZ                  1
#define MBLOCK_PROTECTION_3I0                  2
#define MBLOCK_PROTECTION_3U0                  3
#define MBLOCK_PROTECTION_UMIN                 4
#define MBLOCK_PROTECTION_UMAX                 5
#define MBLOCK_PROTECTION_ZOP                  6

#define MBLOCK_PROTECTION_FREQUENCY_1_ACHR     9
#define MBLOCK_PROTECTION_FREQUENCY_1_CAPV     10
#define MBLOCK_PROTECTION_FREQUENCY_2_ACHR     11
#define MBLOCK_PROTECTION_FREQUENCY_2_CAPV     12
#define MBLOCK_PROTECTION_MTZ04                13
#define MBLOCK_PROTECTION_ZNAM                 14

int getRAISmallModbusRegister(int);//получить содержимое регистра
int getRAISmallModbusBit(int);//получить содержимое бита
int setRAISmallModbusRegister(int, int);//получить содержимое регистра
int setRAISmallModbusBit(int, int);//получить содержимое бита

int  postRAISmallWriteAction(void);//action после записи

int privateRAISmallGetReg2(int adrReg);

SRAM1 COMPONENT_OBJ *raismallcomponent;

/**************************************/
//компонент измерений в момент аварии
/**************************************/
void constructorRAISmallComponent(COMPONENT_OBJ *raismallcomp)
{
  raismallcomponent = raismallcomp;

  raismallcomponent->getModbusRegister = getRAISmallModbusRegister;//получить содержимое регистра
  raismallcomponent->getModbusBit      = getRAISmallModbusBit;//получить содержимое бита
  raismallcomponent->setModbusRegister = setRAISmallModbusRegister;//получить содержимое регистра
  raismallcomponent->setModbusBit      = setRAISmallModbusBit;//получить содержимое бита

  raismallcomponent->postWriteAction = postRAISmallWriteAction;//action после записи
}//constructorIUSmallComponent(COMPONENT_OBJ *iucomp)

int getRAISmallModbusRegister(int adrReg)
{
  //получить содержимое регистра
  if(privateRAISmallGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if (
    //Не подано попередньокоманди вичитування відповідного запису дискретного реєстратора
    //pointInterface=0 метка интерфейса 0-USB 1-RS485 2-LAN
    ((pointInterface == USB_RECUEST  ) && (number_record_of_dr_for_USB == 0xffff)) 
    ||
    ((pointInterface == RS485_RECUEST) && (number_record_of_dr_for_RS485 == 0xffff))
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
    ||
    ((pointInterface == LAN_RECUEST) && (number_record_of_dr_for_LAN == 0xffff))
#endif      
  ) return MARKER_ERRORPERIMETR;
  if (
    //Зараз іде зчитування для інтерфейсу запису дискретного реєстратора, або очистка його, тому ця операція є тимчасово недоступною
    ((clean_rejestrators & CLEAN_DR) != 0) ||
    (
      ((pointInterface == USB_RECUEST  ) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_DR_USB  ) != 0)) 
      ||
      ((pointInterface == RS485_RECUEST) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_DR_RS485) != 0))
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
      ||
      ((pointInterface == LAN_RECUEST) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_DR_LAN) != 0))
#endif      
    )
  ) return MARKER_ERRORPERIMETR; //Не подано попередньокоманди вичитування відповідного запису дискретного реєстратора

  //Можна читати дані
  int number_block, offset;
  unsigned char *point_to_buffer = NULL;
  number_block = (adrReg - MM_ADDRESS_FIRST_MEASUREMENTS_DR) / MMEASUREMENTS_DR_WIDTH;
  offset = (adrReg - MM_ADDRESS_FIRST_MEASUREMENTS_DR) - number_block*MMEASUREMENTS_DR_WIDTH;
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

  if (!(
        number_block < (
          (*(point_to_buffer + FIRST_INDEX_NUMBER_MAX_PHASE_DR  )) +
          (*(point_to_buffer + FIRST_INDEX_NUMBER_MAX_PHASE04_DR)) +
          (*(point_to_buffer + FIRST_INDEX_NUMBER_MAX_ZNAM_DR   )) +
          (*(point_to_buffer + FIRST_INDEX_NUMBER_MAX_3I0_1G_DR    )) +
          (*(point_to_buffer + FIRST_INDEX_NUMBER_MAX_3I0_OTHERG_DR    )) +
          (*(point_to_buffer + FIRST_INDEX_NUMBER_MAX_3I0_r_DR    )) +
          (*(point_to_buffer + FIRST_INDEX_NUMBER_MAX_3U0_DR    )) +
          (*(point_to_buffer + FIRST_INDEX_NUMBER_MIN_U_DR      )) +
          (*(point_to_buffer + FIRST_INDEX_NUMBER_MAX_U_DR      )) +
          (*(point_to_buffer + FIRST_INDEX_NUMBER_MAX_ZOP_DR    )) +
          (*(point_to_buffer + FIRST_INDEX_NUMBER_MIN_F_ACHR_DR )) +
          (*(point_to_buffer + FIRST_INDEX_NUMBER_F_CHAPV_DR    ))
        )
      )) return 0;//Блок пустой
  unsigned int value, index;
  unsigned int control_extra_settings_1_tmp = *((unsigned int *)(point_to_buffer + FIRST_INDEX_EXTRA_SETTINGS_DR));

  switch (offset)
  {
  case MDR_OFFSET_MEASUREMENT_IA_1:
  {
    index = FIRST_INDEX_FIRST_BLOCK_DR + (number_block*SIZE_ARRAY_FIX_MAX_MEASUREMENTS + 3)*sizeof(unsigned int);
    value = *((unsigned int *)(point_to_buffer + index));

    return (value >> 2) &0xFFFF;
  }
  case MDR_OFFSET_MEASUREMENT_IB_1:
  {
    if ((control_extra_settings_1_tmp & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_IB_I04)) == 0)
    {
      index = FIRST_INDEX_FIRST_BLOCK_DR + (number_block*SIZE_ARRAY_FIX_MAX_MEASUREMENTS + 4)*sizeof(unsigned int);
      value = *((unsigned int *)(point_to_buffer + index));

      return (value >> 2) &0xFFFF;
    }
    return 0;
  }
  case MDR_OFFSET_MEASUREMENT_IC_1:
  {
    index = FIRST_INDEX_FIRST_BLOCK_DR + (number_block*SIZE_ARRAY_FIX_MAX_MEASUREMENTS + 5)*sizeof(unsigned int);
    value = *((unsigned int *)(point_to_buffer + index));

    return (value >> 2) &0xFFFF;
  }
  case MDR_OFFSET_MEASUREMENT_3I0:
  {
    index = FIRST_INDEX_FIRST_BLOCK_DR + (number_block*SIZE_ARRAY_FIX_MAX_MEASUREMENTS + 0)*sizeof(unsigned int);
    value = *((unsigned int *)(point_to_buffer + index));

    return (value) &0xFFFF;
  }
  case MDR_OFFSET_MEASUREMENT_3I0_HIGH_GARM:
  {
    index = FIRST_INDEX_FIRST_BLOCK_DR + (number_block*SIZE_ARRAY_FIX_MAX_MEASUREMENTS + 1)*sizeof(unsigned int);
    value = *((unsigned int *)(point_to_buffer + index));

    return (value) &0xFFFF;
  }
  case MDR_OFFSET_MEASUREMENT_3I0_r:
  {
    index = FIRST_INDEX_FIRST_BLOCK_DR + (number_block*SIZE_ARRAY_FIX_MAX_MEASUREMENTS + 2)*sizeof(unsigned int);
    value = *((unsigned int *)(point_to_buffer + index));

    return (value >> 2) &0xFFFF;
  }
  case MDR_OFFSET_MEASUREMENT_I1:
  {
    index = FIRST_INDEX_FIRST_BLOCK_DR + (number_block*SIZE_ARRAY_FIX_MAX_MEASUREMENTS + 7)*sizeof(unsigned int);
    value = *((unsigned int *)(point_to_buffer + index));

    return (value >> 2) &0xFFFF;
  }
  case MDR_OFFSET_MEASUREMENT_I2:
  {
    index = FIRST_INDEX_FIRST_BLOCK_DR + (number_block*SIZE_ARRAY_FIX_MAX_MEASUREMENTS + 6)*sizeof(unsigned int);
    value = *((unsigned int *)(point_to_buffer + index));

    return (value >> 2) &0xFFFF;
  }
  case MDR_OFFSET_MEASUREMENT_IAH2:
  {
    index = FIRST_INDEX_FIRST_BLOCK_DR + (number_block*SIZE_ARRAY_FIX_MAX_MEASUREMENTS + 25)*sizeof(unsigned int);
    value = *((unsigned int *)(point_to_buffer + index));

    return (value >> 2) &0xFFFF;
  }
  case MDR_OFFSET_MEASUREMENT_IBH2:
  {
    index = FIRST_INDEX_FIRST_BLOCK_DR + (number_block*SIZE_ARRAY_FIX_MAX_MEASUREMENTS + 26)*sizeof(unsigned int);
    value = *((unsigned int *)(point_to_buffer + index));

    return (value >> 2) &0xFFFF;
  }
  case MDR_OFFSET_MEASUREMENT_ICH2:
  {
    index = FIRST_INDEX_FIRST_BLOCK_DR + (number_block*SIZE_ARRAY_FIX_MAX_MEASUREMENTS + 27)*sizeof(unsigned int);
    value = *((unsigned int *)(point_to_buffer + index));

    return (value >> 2) &0xFFFF;
  }
  case MDR_OFFSET_MEASUREMENT_I04_1:
  {
    if ((control_extra_settings_1_tmp & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_IB_I04)) != 0)
    {
      index = FIRST_INDEX_FIRST_BLOCK_DR + (number_block*SIZE_ARRAY_FIX_MAX_MEASUREMENTS + 8)*sizeof(unsigned int);
      value = *((unsigned int *)(point_to_buffer + index));

      return (value >> 2) &0xFFFF;
    }
    return 0;
  }
  case MDR_OFFSET_MEASUREMENT_UAB_1:
  {
    index = FIRST_INDEX_FIRST_BLOCK_DR + (number_block*SIZE_ARRAY_FIX_MAX_MEASUREMENTS + 15)*sizeof(unsigned int);
    value = *((unsigned int *)(point_to_buffer + index));

    return (value >> 3) &0xFFFF;
  }
  case MDR_OFFSET_MEASUREMENT_UBC_1:
  {
    index = FIRST_INDEX_FIRST_BLOCK_DR + (number_block*SIZE_ARRAY_FIX_MAX_MEASUREMENTS + 16)*sizeof(unsigned int);
    value = *((unsigned int *)(point_to_buffer + index));

    return (value >> 3) &0xFFFF;
  }
  case MDR_OFFSET_MEASUREMENT_UCA_1:
  {
    index = FIRST_INDEX_FIRST_BLOCK_DR + (number_block*SIZE_ARRAY_FIX_MAX_MEASUREMENTS + 17)*sizeof(unsigned int);
    value = *((unsigned int *)(point_to_buffer + index));

    return (value >> 3) &0xFFFF;
  }
  case MDR_OFFSET_MEASUREMENT_UA_1:
  {
    if ((control_extra_settings_1_tmp & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE)) == 0)
    {
      index = FIRST_INDEX_FIRST_BLOCK_DR + (number_block*SIZE_ARRAY_FIX_MAX_MEASUREMENTS + 9)*sizeof(unsigned int);
      value = *((unsigned int *)(point_to_buffer + index));

      return (value >> 3) &0xFFFF;
    }
    return 0;
  }
  case MDR_OFFSET_MEASUREMENT_UB_1:
  {
    if ((control_extra_settings_1_tmp & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE)) == 0)
    {
      index = FIRST_INDEX_FIRST_BLOCK_DR + (number_block*SIZE_ARRAY_FIX_MAX_MEASUREMENTS + 10)*sizeof(unsigned int);
      value = *((unsigned int *)(point_to_buffer + index));

      return (value >> 3) &0xFFFF;
    }
    return 0;
  }
  case MDR_OFFSET_MEASUREMENT_UC_1:
  {
    if ((control_extra_settings_1_tmp & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE)) == 0)
    {
      index = FIRST_INDEX_FIRST_BLOCK_DR + (number_block*SIZE_ARRAY_FIX_MAX_MEASUREMENTS + 11)*sizeof(unsigned int);
      value = *((unsigned int *)(point_to_buffer + index));

      return (value >> 3) &0xFFFF;
    }
    return 0;
  }
  case MDR_OFFSET_MEASUREMENT_3U0_1:
  {
    index = FIRST_INDEX_FIRST_BLOCK_DR + (number_block*SIZE_ARRAY_FIX_MAX_MEASUREMENTS + 12)*sizeof(unsigned int);
    value = *((unsigned int *)(point_to_buffer + index));

    return (value >> 3) &0xFFFF;
  }
  case MDR_OFFSET_MEASUREMENT_U1:
  {
    index = FIRST_INDEX_FIRST_BLOCK_DR + (number_block*SIZE_ARRAY_FIX_MAX_MEASUREMENTS + 14)*sizeof(unsigned int);
    value = *((unsigned int *)(point_to_buffer + index));

    return (value >> 3) &0xFFFF;
  }
  case MDR_OFFSET_MEASUREMENT_U2:
  {
    index = FIRST_INDEX_FIRST_BLOCK_DR + (number_block*SIZE_ARRAY_FIX_MAX_MEASUREMENTS + 13)*sizeof(unsigned int);
    value = *((unsigned int *)(point_to_buffer + index));

    return (value >> 3) &0xFFFF;
  }
  case MDR_OFFSET_FREQUENCY_1:
  {
    index = FIRST_INDEX_FIRST_BLOCK_DR + (number_block*SIZE_ARRAY_FIX_MAX_MEASUREMENTS + 18)*sizeof(unsigned int);
    value = *((unsigned int *)(point_to_buffer + index));

    return (value / 10) &0xFFFF;
  }
  case MDR_OFFSET_VMP:
  {
    index = FIRST_INDEX_FIRST_BLOCK_DR + (number_block*SIZE_ARRAY_FIX_MAX_MEASUREMENTS + 25)*sizeof(unsigned int);
    int value_int = *((int *)(point_to_buffer + index));

    if (((unsigned int)value_int) != ((unsigned int)UNDEF_VMP))
    {
      if (abs(value_int) >= 10)
        return (value_int/10) &0xFFFF;
      else
      {
        if (value_int >= 0)
          return 1;
        else
          return (-1)&0x0000FFFF;
      }
    }
    return 0;
  }
  case MDR_OFFSET_LABEL_TIME_LSW:
  case MDR_OFFSET_LABEL_TIME_MSW:
  case MDR_OFFSET_LABEL_PROTECT:
  {
    index = FIRST_INDEX_FIRST_BLOCK_DR + (number_block*SIZE_ARRAY_FIX_MAX_MEASUREMENTS + 27)*sizeof(unsigned int);

    if (offset == MDR_OFFSET_LABEL_TIME_LSW)
    {
      return ((*(point_to_buffer + index + 0)) | ((*(point_to_buffer + index + 1)) << 8 )) &0xFFFF;
    }
    else if (offset ==MDR_OFFSET_LABEL_TIME_MSW)
    {
      return  (*(point_to_buffer + index + 2)) &0xFF;
    }
    else
    {
      value = *(point_to_buffer + index + 3);
      switch (value)
      {
      case IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE:
      {
        return MBLOCK_PROTECTION_MTZ;
      }
      case IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE04:
      {
        return MBLOCK_PROTECTION_MTZ04;
      }
      case IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZNAM:
      {
        return MBLOCK_PROTECTION_ZNAM;
      }

//      case IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0:
//      {
//        return MBLOCK_PROTECTION_3I0;
//      }
      case IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE_3U0:
      {
        return MBLOCK_PROTECTION_3U0;
      }
      case IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE:
      {
        return MBLOCK_PROTECTION_UMIN;
      }
      case IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE:
      {
        return MBLOCK_PROTECTION_UMAX;
      }
      case IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZOP:
      {
        return MBLOCK_PROTECTION_ZOP;
      }
      case IDENTIFIER_BIT_ARRAY_MIN_FREQUENCY_ACHR:
      {
        return MBLOCK_PROTECTION_FREQUENCY_1_ACHR;
      }
      case IDENTIFIER_BIT_ARRAY_FREQUENCY_CHAPV:
      {
        return MBLOCK_PROTECTION_FREQUENCY_1_CAPV;
      }
      default:
      {
        //Якщо немає помилок, то сюди б програма не мала заходити
        return 0;
      }

      }//switch
    }//else

    break;
  }//case

  }//switch
  return 0;
}//getDVModbusRegister(int adrReg)
int getRAISmallModbusBit(int x) {
  //получить содержимое bit
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getIUModbusBit(int )
int setRAISmallModbusRegister(int x, int y) {
  UNUSED(x);
  UNUSED(y);
  //записать содержимое регистра
  return MARKER_OUTPERIMETR;
}//setIUModbusRegister(int, int)
int setRAISmallModbusBit(int x, int y) {
  UNUSED(x);
  UNUSED(y);
  //записать содержимое bit
  return MARKER_OUTPERIMETR;
}//setIUModbusBit(int, int )

int postRAISmallWriteAction(void) {
//action после записи
  return 0;
}//

int privateRAISmallGetReg2(int adrReg)
{
  //проверить внешний периметр
  return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateRAISmallGetReg2(int adrReg)

