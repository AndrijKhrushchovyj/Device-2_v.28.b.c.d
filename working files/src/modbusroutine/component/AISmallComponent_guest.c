#include "header.h"
#include "variables_external_m.h"

int getAISmallModbusRegister_guest(int offset);

int measurement_low[_NUMBER_IM];

int getAISmallModbusRegister_guest(int offset)
{
  semaphore_measure_values_low = 1;
  //Копіюємо вимірювання
  for (unsigned int i = 0; i < _NUMBER_IM; i++ )
  {
    measurement_low[i] = measurement_middle[i];
  }

  semaphore_measure_values_low = 0;

  switch (offset)
  {

  case 3://IA
  {
    return (measurement_low[IM_IA_1] >> 2) &0xFFFF;
  }

  case 25://UAB_1
  {
    return (measurement_low[IM_UAB_TN1] >> 3) &0xFFFF;
  }

  case 203://Струм Ia2
  {
    return (measurement_low[IM_IA_2] >> 2) &0xFFFF;
  }

  case 225://UAB_2
    return (measurement_low[IM_UAB_TN2] >> 3) &0xFFFF;

  case 620://Напруга Uлог 5.19-5.18
   return 0;
  case 621://Напруга Uлог 5.18-5.17 
   return 0;
  case 622://Напруга Uс1с2
    return (measurement_low[IM_UC1C2] >> 3) &0xFFFF;
  case 623://Напруга Uр1р2
    return (measurement_low[IM_UP1P2] >> 3) &0xFFFF;
  case 624://Напруга Uр2р3
    return (measurement_low[IM_UP2P3] >> 3) &0xFFFF;
  case 625://Кут сельсина
      if (
        (measurement_low[IM_UC1C2] > PORIG_CHUTLYVOSTI_DETECTORA_KUTA))
      {
        return angle_selsyn;
      }//if
   return 0xFFFF;

  case 631://Положення РПН
   return 0;
  case 632://Перемикання РПН за поточну добу
   return 0;
  case 633://Перемикання РПН за поточну добу
   return 0;
  case 634://Перемикання РПН за попередню добу
   return 0;
  case 635://Перемикання РПН за попередню добу
   return 0;
  case 636://Загальна кількість перемикань РПН
   return 0;
  case 637://Загальна кількість перемикань РПН
   return 0;

  }//switch
  return 0;
}//getAISmallModbusRegister_guest(int offset)

