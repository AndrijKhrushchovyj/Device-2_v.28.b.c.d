#include "header.h"

int etap_settings_test_frequency = -1;

/**************************************/
//Конфігурація I2C
/**************************************/
void Configure_I2C(I2C_TypeDef *I2Cx)
{
  I2C_InitTypeDef I2C_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  uint16_t current_count_tim4, new_count_tim4;
  unsigned int delta;

  /* Забороняємо I2C  */
  I2C_Cmd(I2Cx, DISABLE);

  /***
  Добиваємося, щоб на SDA встановилася 1
  ***/
  /* Настроюємо I2C піни SCL Output Open-drain */
  GPIO_InitStructure.GPIO_Pin = GPIO_I2C_SCL;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIO_I2C, &GPIO_InitStructure);
  /* Встановлюємо піни SCL у високий рівень*/
  GPIO_SetBits(GPIO_I2C, GPIO_I2C_SCL);
  while (GPIO_ReadOutputDataBit(GPIO_I2C, GPIO_I2C_SCL) == Bit_RESET)
    ;

  /*Настроюємо I2C піни SDA  на ввід*/
  GPIO_InitStructure.GPIO_Pin = GPIO_I2C_SDA;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIO_I2C, &GPIO_InitStructure);

  while (GPIO_ReadInputDataBit(GPIO_I2C, GPIO_I2C_SDA) == RESET)
  {
    /* SCL -> "0" */
    GPIO_ResetBits(GPIO_I2C, GPIO_I2C_SCL);
    current_count_tim4 = ((uint16_t) TIM4->CNT);
    delta = 0;
    while (delta < 4) // <= 4x10 = 40(мкс)
    {
      new_count_tim4 = ((uint16_t) TIM4->CNT);
      if (new_count_tim4 >= current_count_tim4)
        delta = new_count_tim4 - current_count_tim4;
      else
        delta = (0x10000 - current_count_tim4) + new_count_tim4; //0x10000 - це повний період таймера, бо ми настроїли його тактуватиу інтервалі [0; 65535]
    }

    /* SCL -> "1" */
    GPIO_SetBits(GPIO_I2C, GPIO_I2C_SCL);
    while (GPIO_ReadOutputDataBit(GPIO_I2C, GPIO_I2C_SCL) == Bit_RESET)
      ;
    current_count_tim4 = ((uint16_t) TIM4->CNT);
    delta = 0;
    while (delta < 4) // <= 4x10 = 40(мкс)
    {
      new_count_tim4 = ((uint16_t) TIM4->CNT);
      if (new_count_tim4 >= current_count_tim4)
        delta = new_count_tim4 - current_count_tim4;
      else
        delta = (0x10000 - current_count_tim4) + new_count_tim4; //0x10000 - це повний період таймера, бо ми настроїли його тактуватиу інтервалі [0; 65535]
    }
  }
  /*******/

  /***
  Переводимо піна під упавління мікроконтролера
  ***/
  /* Настроюємо I2C пін SDA як Output Open-drain */
  GPIO_InitStructure.GPIO_Pin = GPIO_I2C_SDA;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIO_I2C, &GPIO_InitStructure);
  /* Встановлюємо піни SCL і SDA у високий рівень*/
  GPIO_SetBits(GPIO_I2C, (GPIO_I2C_SCL | GPIO_I2C_SDA));
  while (GPIO_ReadOutputDataBit(GPIO_I2C, GPIO_I2C_SCL) == Bit_RESET)
    ;
  /*******/

  /***
  Симулюємо SOFTWARE RESET для EEPROM: Start + 9-bit + Start + Stop 
  ****/

  /*- Start -*/
  /*SCL = 1 і SCA = 1*/

  /* SCL -> "0" */
  GPIO_ResetBits(GPIO_I2C, GPIO_I2C_SCL);
  current_count_tim4 = ((uint16_t) TIM4->CNT);
  delta = 0;
  while (delta < 2) // <= 2x10 = 20(мкс)
  {
    new_count_tim4 = ((uint16_t) TIM4->CNT);
    if (new_count_tim4 >= current_count_tim4)
      delta = new_count_tim4 - current_count_tim4;
    else
      delta = (0x10000 - current_count_tim4) + new_count_tim4; //0x10000 - це повний період таймера, бо ми настроїли його тактуватиу інтервалі [0; 65535]
  }

  /* SCL -> "1" */
  GPIO_SetBits(GPIO_I2C, GPIO_I2C_SCL);
  while (GPIO_ReadOutputDataBit(GPIO_I2C, GPIO_I2C_SCL) == Bit_RESET)
    ;
  current_count_tim4 = ((uint16_t) TIM4->CNT);
  delta = 0;
  while (delta < 2) // <= 2x10 = 20(мкс)
  {
    new_count_tim4 = ((uint16_t) TIM4->CNT);
    if (new_count_tim4 >= current_count_tim4)
      delta = new_count_tim4 - current_count_tim4;
    else
      delta = (0x10000 - current_count_tim4) + new_count_tim4; //0x10000 - це повний період таймера, бо ми настроїли його тактуватиу інтервалі [0; 65535]
  }

  /* SDA -> "0" */
  GPIO_ResetBits(GPIO_I2C, GPIO_I2C_SDA);
  current_count_tim4 = ((uint16_t) TIM4->CNT);
  delta = 0;
  while (delta < 2) // <= 2x10 = 20(мкс)
  {
    new_count_tim4 = ((uint16_t) TIM4->CNT);
    if (new_count_tim4 >= current_count_tim4)
      delta = new_count_tim4 - current_count_tim4;
    else
      delta = (0x10000 - current_count_tim4) + new_count_tim4; //0x10000 - це повний період таймера, бо ми настроїли його тактуватиу інтервалі [0; 65535]
  }

  /* SCL -> "0" */
  GPIO_ResetBits(GPIO_I2C, GPIO_I2C_SCL);
  current_count_tim4 = ((uint16_t) TIM4->CNT);
  delta = 0;
  while (delta < 2) // <= 2x10 = 20(мкс)
  {
    new_count_tim4 = ((uint16_t) TIM4->CNT);
    if (new_count_tim4 >= current_count_tim4)
      delta = new_count_tim4 - current_count_tim4;
    else
      delta = (0x10000 - current_count_tim4) + new_count_tim4; //0x10000 - це повний період таймера, бо ми настроїли його тактуватиу інтервалі [0; 65535]
  }

  /* SDA -> "1" */
  GPIO_SetBits(GPIO_I2C, GPIO_I2C_SDA);

  /*- 9 циклів -*/
  for (unsigned int i = 0; i < 9; i++)
  {
    /* SCL = "0" */
    current_count_tim4 = ((uint16_t) TIM4->CNT);
    delta = 0;
    while (delta < 2) // <= 2x10 = 20(мкс)
    {
      new_count_tim4 = ((uint16_t) TIM4->CNT);
      if (new_count_tim4 >= current_count_tim4)
        delta = new_count_tim4 - current_count_tim4;
      else
        delta = (0x10000 - current_count_tim4) + new_count_tim4; //0x10000 - це повний період таймера, бо ми настроїли його тактуватиу інтервалі [0; 65535]
    }

    /* SCL -> "1" */
    GPIO_SetBits(GPIO_I2C, GPIO_I2C_SCL);
    while (GPIO_ReadOutputDataBit(GPIO_I2C, GPIO_I2C_SCL) == Bit_RESET)
      ;
    current_count_tim4 = ((uint16_t) TIM4->CNT);
    delta = 0;
    while (delta < 4) // <= 4x10 = 40(мкс)
    {
      new_count_tim4 = ((uint16_t) TIM4->CNT);
      if (new_count_tim4 >= current_count_tim4)
        delta = new_count_tim4 - current_count_tim4;
      else
        delta = (0x10000 - current_count_tim4) + new_count_tim4; //0x10000 - це повний період таймера, бо ми настроїли його тактуватиу інтервалі [0; 65535]
    }

    /* SCL -> "0" */
    GPIO_ResetBits(GPIO_I2C, GPIO_I2C_SCL);
    current_count_tim4 = ((uint16_t) TIM4->CNT);
    delta = 0;
    while (delta < 2) // <= 2x10 = 20(мкс)
    {
      new_count_tim4 = ((uint16_t) TIM4->CNT);
      if (new_count_tim4 >= current_count_tim4)
        delta = new_count_tim4 - current_count_tim4;
      else
        delta = (0x10000 - current_count_tim4) + new_count_tim4; //0x10000 - це повний період таймера, бо ми настроїли його тактуватиу інтервалі [0; 65535]
    }
  }

  /*- Start -*/
  /*SCL = 0 і SCA = 1*/

  current_count_tim4 = ((uint16_t) TIM4->CNT);
  delta = 0;
  while (delta < 2) // <= 2x10 = 20(мкс)
  {
    new_count_tim4 = ((uint16_t) TIM4->CNT);
    if (new_count_tim4 >= current_count_tim4)
      delta = new_count_tim4 - current_count_tim4;
    else
      delta = (0x10000 - current_count_tim4) + new_count_tim4; //0x10000 - це повний період таймера, бо ми настроїли його тактуватиу інтервалі [0; 65535]
  }

  /* SCL -> "1" */
  GPIO_SetBits(GPIO_I2C, GPIO_I2C_SCL);
  while (GPIO_ReadOutputDataBit(GPIO_I2C, GPIO_I2C_SCL) == Bit_RESET)
    ;
  current_count_tim4 = ((uint16_t) TIM4->CNT);
  delta = 0;
  while (delta < 2) // <= 2x10 = 20(мкс)
  {
    new_count_tim4 = ((uint16_t) TIM4->CNT);
    if (new_count_tim4 >= current_count_tim4)
      delta = new_count_tim4 - current_count_tim4;
    else
      delta = (0x10000 - current_count_tim4) + new_count_tim4; //0x10000 - це повний період таймера, бо ми настроїли його тактуватиу інтервалі [0; 65535]
  }

  /* SDA -> "0" */
  GPIO_ResetBits(GPIO_I2C, GPIO_I2C_SDA);
  current_count_tim4 = ((uint16_t) TIM4->CNT);
  delta = 0;
  while (delta < 2) // <= 2x10 = 20(мкс)
  {
    new_count_tim4 = ((uint16_t) TIM4->CNT);
    if (new_count_tim4 >= current_count_tim4)
      delta = new_count_tim4 - current_count_tim4;
    else
      delta = (0x10000 - current_count_tim4) + new_count_tim4; //0x10000 - це повний період таймера, бо ми настроїли його тактуватиу інтервалі [0; 65535]
  }

  /* SCL -> "0" */
  GPIO_ResetBits(GPIO_I2C, GPIO_I2C_SCL);
  current_count_tim4 = ((uint16_t) TIM4->CNT);
  delta = 0;
  while (delta < 2) // <= 2x10 = 20(мкс)
  {
    new_count_tim4 = ((uint16_t) TIM4->CNT);
    if (new_count_tim4 >= current_count_tim4)
      delta = new_count_tim4 - current_count_tim4;
    else
      delta = (0x10000 - current_count_tim4) + new_count_tim4; //0x10000 - це повний період таймера, бо ми настроїли його тактуватиу інтервалі [0; 65535]
  }

  /*- Stop -*/
  /*SCL = 0 і SCA = 0*/

  current_count_tim4 = ((uint16_t) TIM4->CNT);
  delta = 0;
  while (delta < 2) // <= 2x10 = 20(мкс)
  {
    new_count_tim4 = ((uint16_t) TIM4->CNT);
    if (new_count_tim4 >= current_count_tim4)
      delta = new_count_tim4 - current_count_tim4;
    else
      delta = (0x10000 - current_count_tim4) + new_count_tim4; //0x10000 - це повний період таймера, бо ми настроїли його тактуватиу інтервалі [0; 65535]
  }

  /* SCL -> "1" */
  GPIO_SetBits(GPIO_I2C, GPIO_I2C_SCL);
  while (GPIO_ReadOutputDataBit(GPIO_I2C, GPIO_I2C_SCL) == Bit_RESET)
    ;
  current_count_tim4 = ((uint16_t) TIM4->CNT);
  delta = 0;
  while (delta < 2) // <= 2x10 = 20(мкс)
  {
    new_count_tim4 = ((uint16_t) TIM4->CNT);
    if (new_count_tim4 >= current_count_tim4)
      delta = new_count_tim4 - current_count_tim4;
    else
      delta = (0x10000 - current_count_tim4) + new_count_tim4; //0x10000 - це повний період таймера, бо ми настроїли його тактуватиу інтервалі [0; 65535]
  }

  /* SDA -> "1" */
  GPIO_SetBits(GPIO_I2C, GPIO_I2C_SDA);
  current_count_tim4 = ((uint16_t) TIM4->CNT);
  delta = 0;
  while (delta < 2) // <= 2x10 = 20(мкс)
  {
    new_count_tim4 = ((uint16_t) TIM4->CNT);
    if (new_count_tim4 >= current_count_tim4)
      delta = new_count_tim4 - current_count_tim4;
    else
      delta = (0x10000 - current_count_tim4) + new_count_tim4; //0x10000 - це повний період таймера, бо ми настроїли його тактуватиу інтервалі [0; 65535]
  }

  /* SCL -> "0" */
  GPIO_ResetBits(GPIO_I2C, GPIO_I2C_SCL);
  current_count_tim4 = ((uint16_t) TIM4->CNT);
  delta = 0;
  while (delta < 2) // <= 2x10 = 20(мкс)
  {
    new_count_tim4 = ((uint16_t) TIM4->CNT);
    if (new_count_tim4 >= current_count_tim4)
      delta = new_count_tim4 - current_count_tim4;
    else
      delta = (0x10000 - current_count_tim4) + new_count_tim4; //0x10000 - це повний період таймера, бо ми настроїли його тактуватиу інтервалі [0; 65535]
  }

  /* Встановлюємо піни SCL і SDA у високий рівень*/
  GPIO_SetBits(GPIO_I2C, (GPIO_I2C_SCL | GPIO_I2C_SDA));
  while (GPIO_ReadOutputDataBit(GPIO_I2C, GPIO_I2C_SCL) == Bit_RESET)
    ;
  /*******/

  /*
  Повертаємо піни під управління I2C
  */
  //Конфігуруємо піни PB8/I2C1_SCL і PB9/I2C1_SDA
  GPIO_PinAFConfig(GPIO_I2C, GPIO_I2C_SCLSource, GPIO_AF_I2C);
  GPIO_PinAFConfig(GPIO_I2C, GPIO_I2C_SDASource, GPIO_AF_I2C);

  /* Настроюємо I2C піни: SCL і SDA */
  GPIO_InitStructure.GPIO_Pin = GPIO_I2C_SCL | GPIO_I2C_SDA;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIO_I2C, &GPIO_InitStructure);
  /*******/

  /* Скидаємо всі I2C регістри */
  I2C_SoftwareResetCmd(I2Cx, ENABLE);
  I2C_SoftwareResetCmd(I2Cx, DISABLE);

  /* Дозволяємо I2C*/
  I2C_Cmd(I2Cx, ENABLE);

  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = /*EEPROM_ADDRESS*/ RTC_ADDRESS;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = /*(low_speed_i2c == 0 ) ? CLOCKSPEED_1MBIT :*/ CLOCKSPEED;
  I2C_Init(I2Cx, &I2C_InitStructure);

  /* Дозволяємо для I2C генерацію переривань по помилках */
  I2C_ITConfig(I2Cx, I2C_IT_ERR, ENABLE);
}
/**************************************/

/*****************************************************/
//Старт  запису буферу по I2C
/*
  Результат
  0 - почався новий обмін
  1 - покищо лінія зайнята
  2 - не запущений обмін через помилки на i2C
*/
/*****************************************************/
unsigned int start_write_buffer_via_I2C(uint32_t device_id, uint32_t WriteAddr, uint8_t *pBuffer, uint32_t number)
{
  unsigned int error = 0; //0 - успішно виконано; 1 - лінія зайнята; 2 - інші помилки
  //Перевіряємо чи у процесі аналізу попередньої транзакції скинуті попередні помилки
  if (type_error_of_exchanging_via_i2c != 0)
  {
    type_error_of_exchanging_via_i2c |= (1 << ERROR_TOTAL_BIT);
    error = 2;
  }
  else
  {
    /*Перевіряємо чи лінія є зайнятою на момент запуску цієї функції*/
    if (I2C_GetFlagStatus(I2C, I2C_FLAG_BUSY))
    {
      if (number_busy_state < MAX_NUMBER_BUSY_STATE)
      {
        error = 1;
        number_busy_state++;
      }
      else
      {
        type_error_of_exchanging_via_i2c |= (1 << ERROR_TOTAL_BIT);
        error = 2;
      }

      /***/
      //Перевірка, чи є непрочитані дані
      /***/
      unsigned int temp_value = 0;
      while (((I2C_GetLastEvent(I2C) & I2C_EVENT_MASTER_BYTE_RECEIVED_NO_BUSY) != 0) && (temp_value < 100))
      {
        I2C_AcknowledgeConfig(I2C, DISABLE);
        I2C_GenerateSTOP(I2C, ENABLE);
        I2C_ReceiveData(I2C);
        temp_value++;
      }
      if (temp_value >= 100)
      {
        type_error_of_exchanging_via_i2c |= (1 << ERROR_TOTAL_BIT);
        error = 2;
      }
      /***/
    }
    else
    {
      //Очищаємо кількість BUSY станів
      number_busy_state = 0;

      //Заповнюємо структуру драйверу обміну через I2C
      driver_i2c.state_execution = 0;
      driver_i2c.device_id = device_id;
      driver_i2c.action = 2;
      driver_i2c.internal_address = WriteAddr;
      driver_i2c.number_bytes = number;
      driver_i2c.point_buffer = pBuffer;

      //Виставляємо генерацію ACK операції читання (хоч при операції запису цей біт не мав би впливати)
      I2C_AcknowledgeConfig(I2C, ENABLE);

      /*Стираємо можливі помилки, які можуть бути виставленими у регістрі статусу*/
      I2C_ClearFlag(I2C, I2C_FLAG_TIMEOUT | I2C_FLAG_OVR | I2C_FLAG_AF | I2C_FLAG_ARLO | I2C_FLAG_BERR);

      /* Дозволяємо для I2C генерацію переривань по подіях */
      I2C_ITConfig(I2C, I2C_IT_EVT, ENABLE);

      /* Посилаємо STRAT*/
      I2C_GenerateSTART(I2C, ENABLE);
    }
  }

  return error;
}
/*****************************************************/

/*****************************************************/
//Старт  читання буферу по I2C
/*
  Результат
  0 - почався новий обмін
  1 - покищо лінія зайнята
  2 - не запущений обмін через помилки на i2C
*/
/*****************************************************/
unsigned int start_read_buffer_via_I2C(uint32_t device_id, uint32_t ReadAddr, uint8_t *pBuffer, uint32_t number)
{
  unsigned int error = 0; //0 - успішно виконано; 1 - лінія зайнята; 2 - інші помилки
  //Перевіряємо чи у процесі аналізу попередньої транзакції скинуті попередні помилки
  if (type_error_of_exchanging_via_i2c != 0)
  {
    type_error_of_exchanging_via_i2c |= (1 << ERROR_TOTAL_BIT);
    error = 2;
  }
  else
  {
    /*Перевіряємо чи лінія є зайнятою на момент запуску цієї функції*/
    if (I2C_GetFlagStatus(I2C, I2C_FLAG_BUSY))
    {
      if (number_busy_state < MAX_NUMBER_BUSY_STATE)
      {
        error = 1;
        number_busy_state++;
      }
      else
      {
        type_error_of_exchanging_via_i2c |= (1 << ERROR_TOTAL_BIT);
        error = 2;
      }

      /***/
      //Перевірка, чи є непрочитані дані
      /***/
      unsigned int temp_value = 0;
      while (((I2C_GetLastEvent(I2C) & I2C_EVENT_MASTER_BYTE_RECEIVED_NO_BUSY) != 0) && (temp_value < 100))
      {
        I2C_AcknowledgeConfig(I2C, DISABLE);
        I2C_GenerateSTOP(I2C, ENABLE);
        I2C_ReceiveData(I2C);
        temp_value++;
      }
      if (temp_value >= 100)
      {
        type_error_of_exchanging_via_i2c |= (1 << ERROR_TOTAL_BIT);
        error = 2;
      }
      /***/
    }
    else
    {
      //Очищаємо кількість BUSY станів
      number_busy_state = 0;

      //Заповнюэмо структуру драйверу обміну через I2C
      driver_i2c.state_execution = 0;
      driver_i2c.device_id = device_id;
      driver_i2c.action = 0;
      driver_i2c.internal_address = ReadAddr;
      driver_i2c.number_bytes = number;
      driver_i2c.point_buffer = pBuffer;

      //Виставляємо генерацію ACK операції читання
      I2C_AcknowledgeConfig(I2C, ENABLE);

      /*Стираємо можливі помилки, які можуть бути виставленими у регістрі статусу*/
      I2C_ClearFlag(I2C, I2C_FLAG_TIMEOUT | I2C_FLAG_OVR | I2C_FLAG_AF | I2C_FLAG_ARLO | I2C_FLAG_BERR);

      /* Дозволяємо для I2C генерацію переривань по подіях */
      I2C_ITConfig(I2C, I2C_IT_EVT, ENABLE);

      /* Посилаємо STRAT*/
      I2C_GenerateSTART(I2C, ENABLE);
    }
  }

  return error;
}
/*****************************************************/

/*****************************************************/
//Управління обміном через I2C
/*****************************************************/
void main_routines_for_i2c(void)
{
  static unsigned int temp_value_for_rtc;
  static uint8_t reg_RTC[7];
  static uint8_t copy_register8_RTC;
  static uint8_t temp_register_rtc[2];

  if ((save_time_dat_l == 2) || (save_time_dat_h == 2))
  {
    if (save_time_dat_l == 2)
      save_time_dat_l = 1;
    if (save_time_dat_h == 2)
      save_time_dat_h = 1;

    _SET_BIT(control_i2c_taskes, TASK_START_WRITE_RTC_BIT);
    _SET_BIT(control_i2c_taskes, TASK_BLK_OPERATION_BIT);
  }

  if (driver_i2c.state_execution < 0)
  {
    /*************************************************/
    //Зараз можна ініціювати нову трансакцію через I2C
    /*************************************************/

    if (_CHECK_SET_BIT(control_i2c_taskes, TASK_SET_START_SETTINGS_RTC_BIT) != 0)
    {
      unsigned int rez;

      //Виставляння регістрів RTC у потрібні стартові значення для того, щоб на виході мати 1 Гц
      if (etap_settings_test_frequency == 0)
      {
        read_write_i2c_buffer[0] = temp_register_rtc[0];
        rez = start_write_buffer_via_I2C(RTC_ADDRESS, 0xC, read_write_i2c_buffer, 1);
      }
      else if (etap_settings_test_frequency == 1)
      {
        read_write_i2c_buffer[0] = 0xf0;
        rez = start_write_buffer_via_I2C(RTC_ADDRESS, 0x13, read_write_i2c_buffer, 1);
      }
      else
      {
        read_write_i2c_buffer[0] = temp_register_rtc[1];
        rez = start_write_buffer_via_I2C(RTC_ADDRESS, 0xA, read_write_i2c_buffer, 1);
      }

      //Аналізуємо успішність запуску нового запису
      if (rez > 1)
      {
        error_start_i2c();

        //Покищо просто очищаємо змінну, яка конкретизуєм помилку, у майбутньому її можна буде конкретизувати
        type_error_of_exchanging_via_i2c = 0;
      }
      else if (rez == 0)
        _SET_BIT(clear_diagnostyka, ERROR_START_VIA_I2C_BIT);
    }
    else if ((_CHECK_SET_BIT(control_i2c_taskes, TASK_RESET_OF_RTC_BIT) != 0) || (_CHECK_SET_BIT(control_i2c_taskes, TASK_RESET_ST_RTC_BIT) != 0))
    {
      unsigned int rez;

      if (_CHECK_SET_BIT(control_i2c_taskes, TASK_RESET_OF_RTC_BIT) != 0)
      {
        //Скидання OF-біт в RTC
        if (etap_reset_of_bit == ETAP_CLEAR_OF_ST_STOP)
          read_write_i2c_buffer[0] = reg_RTC[1] | (1 << 7); //ST = 1
        else if (etap_reset_of_bit == ETAP_CLEAR_OF_ST_CLEAR)
          read_write_i2c_buffer[0] = reg_RTC[1] & (~(1 << 7)); //ST = 0
        else
          read_write_i2c_buffer[0] = (unsigned char) (temp_value_for_rtc) & (~(1 << 2)); //OF = 0
      }
      else
      {
        read_write_i2c_buffer[0] = reg_RTC[1] & (~(1 << 7)); //ST = 0
      }
      if ((_CHECK_SET_BIT(control_i2c_taskes, TASK_RESET_OF_RTC_BIT) != 0) && (etap_reset_of_bit != ETAP_CLEAR_OF_ST_STOP) && (etap_reset_of_bit != ETAP_CLEAR_OF_ST_CLEAR))
        rez = start_write_buffer_via_I2C(RTC_ADDRESS, 0xF, read_write_i2c_buffer, 1);
      else
        rez = start_write_buffer_via_I2C(RTC_ADDRESS, 0x1, read_write_i2c_buffer, 1);

      //Аналізуємо успішність запуску нового запису
      if (rez > 1)
      {
        error_start_i2c();

        //Покищо просто очищаємо змінну, яка конкретизуєм помилку, у майбутньому її можна буде конкретизувати
        type_error_of_exchanging_via_i2c = 0;
      }
      else if (rez == 0)
        _SET_BIT(clear_diagnostyka, ERROR_START_VIA_I2C_BIT);
    }
    else if (_CHECK_SET_BIT(control_i2c_taskes, TASK_BLK_OPERATION_BIT) != 0)
    {
      //Зараз треба зачекати з початною опрерацією, тому просто виходимо з цієї підпрограми
    }
    else if (_CHECK_SET_BIT(control_i2c_taskes, TASK_START_WRITE_RTC_BIT) != 0)
    {
      copying_time_dat = 1;
      time_t time_dat_tmp = time_dat_copy + 1; //+1 щоб спробувати записати, коли буде перехід на наступну секунду, або вже як вийде...
      copying_time_dat = 0;
      struct tm *p;
      p = gmtime(&time_dat_tmp);

      //Запускаємо процес запису часу в RTC
      read_write_i2c_buffer[0] = 0;

      int tmp_reg = p->tm_sec;
      read_write_i2c_buffer[1] = INT_TO_BCD(tmp_reg) & 0x7F;

      tmp_reg = p->tm_min;
      read_write_i2c_buffer[2] = INT_TO_BCD(tmp_reg) & 0x7F;

      tmp_reg = p->tm_hour;
      read_write_i2c_buffer[3] = INT_TO_BCD(tmp_reg) & 0x3F;

      tmp_reg = p->tm_wday + 1;
      read_write_i2c_buffer[4] = tmp_reg & 0x7;

      tmp_reg = p->tm_mday;
      read_write_i2c_buffer[5] = INT_TO_BCD(tmp_reg) & 0x3F;

      tmp_reg = p->tm_mon + 1;
      read_write_i2c_buffer[6] = INT_TO_BCD(tmp_reg) & 0x1F;

      tmp_reg = p->tm_year - 100;
      read_write_i2c_buffer[7] = INT_TO_BCD(tmp_reg) & 0xFF;

      read_write_i2c_buffer[8] = (calibration & 0x3f) | (copy_register8_RTC & 0xC0);

      //Визначаємо час, коли десяті і стоі секунди мали  бубти рівними 0
      time_t time_dat_tmp1;
      do
      {
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
        /*******************/
        //Управління Каналом 2 міжпроцесорного обміну між БАв і комунікаційною платою
        /*******************/
        low_routine_for_KP();
        /*******************/
#endif
        copying_time_dat = 1;
        time_dat_tmp1 = time_dat_copy;
        copying_time_dat = 0;

        watchdog_routine(UNITED_BITS_WATCHDOG, 6);
      } while (time_dat_tmp1 < time_dat_tmp);

      unsigned int rez = start_write_buffer_via_I2C(RTC_ADDRESS, START_ADDRESS_TIME_REGISTERS, read_write_i2c_buffer, 9);

      //Аналізуємо успішність запуску нового запису
      if (rez > 1)
      {
        error_start_i2c();

        //Покищо просто очищаємо змінну, яка конкретизуєм помилку, у майбутньому її можна буде конкретизувати
        type_error_of_exchanging_via_i2c = 0;
      }
      else if (rez == 0)
      {
        _SET_BIT(clear_diagnostyka, ERROR_START_VIA_I2C_BIT);

        //При успішнопу запуску читання скидаємо біт запуску читання RTC і виставляємо біт процесу читання RTC
        _SET_BIT(control_i2c_taskes, TASK_WRITING_RTC_BIT);
        _CLEAR_BIT(control_i2c_taskes, TASK_START_WRITE_RTC_BIT);

        if ((save_time_dat_l == 1) || (save_time_dat_h == 1))
        {
          if (save_time_dat_l == 1)
            save_time_dat_l = 0;
          if (save_time_dat_h == 1)
            save_time_dat_h = 0;
        }
      }
    }
    else if (_CHECK_SET_BIT(control_i2c_taskes, TASK_START_READ_RTC_BIT) != 0)
    {
      unsigned int rez;

      //Запускаємо процес читання RTC
      rez = start_read_buffer_via_I2C(RTC_ADDRESS, START_ADDRESS_TIME_REGISTERS, read_write_i2c_buffer, MAX_NUMBER_REGISTERS_RTC);

      //Аналізуємо успішність запуску нового запису
      if (rez > 1)
      {
        error_start_i2c();

        //Покищо просто очищаємо змінну, яка конкретизуєм помилку, у майбутньому її можна буде конкретизувати
        type_error_of_exchanging_via_i2c = 0;
      }
      else if (rez == 0)
      {
        _SET_BIT(clear_diagnostyka, ERROR_START_VIA_I2C_BIT);

        //При успішнопу запуску читання скидаємо біт запуску читання RTC і виставляємо біт процесу читання RTC
        _SET_BIT(control_i2c_taskes, TASK_READING_RTC_BIT);
        _CLEAR_BIT(control_i2c_taskes, TASK_START_READ_RTC_BIT);
      }
    }

    /*************************************************/
  }
  else if (driver_i2c.state_execution == 1)
  {
    /*************************************************/
    //Обмін завершився без помилок
    /*************************************************/
    //Виставляємо повідомлення, що I2C готовий до нової транзакції
    driver_i2c.state_execution = -1;

    if (_CHECK_SET_BIT(control_i2c_taskes, TASK_READING_RTC_BIT) != 0)
    {
      //Аналізуємо прочитані дані
      if (
        ((read_write_i2c_buffer[0xA] & (1 << 6)) == 0) /*SQWE = 0*/ ||
        ((read_write_i2c_buffer[0xC] & (1 << 6)) != 0) /*HT  = 1 */ ||
        ((read_write_i2c_buffer[0x13]) != 0xf0) /*Потрібна частота*/
      )
      {
        //Треба потрібні біти  і слова виставити у робочі значення, щоб на виході отримати 1гц
        temp_register_rtc[0] = read_write_i2c_buffer[0xC] & (unsigned char) (~(1 << 6));
        temp_register_rtc[1] = read_write_i2c_buffer[0xA] | (1 << 6);

        //Повідомлення про цю несправність виставляємо, коли не йде тільки-но перше зчитування
        if ((state_i2c_task & STATE_FIRST_READING_RTC) == 0)
          _SET_BIT(set_diagnostyka, RTC_WORK_FIELD_NOT_SET_BIT);
        else
          _SET_BIT(clear_diagnostyka, RTC_WORK_FIELD_NOT_SET_BIT);

        etap_settings_test_frequency = 0;
        _SET_BIT(control_i2c_taskes, TASK_SET_START_SETTINGS_RTC_BIT);
      }
      else
        _SET_BIT(clear_diagnostyka, RTC_WORK_FIELD_NOT_SET_BIT);

      if ((read_write_i2c_buffer[0x1] & (1 << 7)) != 0) /*ST = 1 */
      {
        //Треба запустиити осцилятор для RTC
        _SET_BIT(control_i2c_taskes, TASK_RESET_ST_RTC_BIT);
      }
      else if ((etap_reset_of_bit < ETAP_CLEAR_OF_ST_STOP) && ((read_write_i2c_buffer[0xf] & (1 << 2)) != 0))
      {
        _SET_BIT(set_diagnostyka, RTC_OSCILLATOR_FAIL_BIT);
        //Треба ST-біт виставити в 1 і негайно скинути його в 0. Почекати через 5 секунд він має скинутися
        if ((state_i2c_task & STATE_FIRST_READING_RTC) == 0)
        {
          etap_reset_of_bit = ETAP_CLEAR_OF_ST_STOP;
          _SET_BIT(control_i2c_taskes, TASK_RESET_OF_RTC_BIT);
        }
        else
          etap_reset_of_bit = ETAP_CLEAR_OF_ST_START_WAITING_5S;
      }
      else if (etap_reset_of_bit == ETAP_CLEAR_OF_WRITE_OF_0)
      {
        //Треба очистити OF-біт
        temp_value_for_rtc = read_write_i2c_buffer[0xf];
        _SET_BIT(control_i2c_taskes, TASK_RESET_OF_RTC_BIT);
      }

      if ((read_write_i2c_buffer[0xC] & (1 << 6)) != 0)
      {
        //Повідомлення про цю несправність виставляємо, коли не йде тільки-но перше зчитування
        if ((state_i2c_task & STATE_FIRST_READING_RTC) == 0)
        {
          _SET_BIT(set_diagnostyka, RTC_UPDATING_HALTED_BIT);
          //..asm volatile(
          //..  "bkpt 1"
          //..);
        }
        else
        {
          _SET_BIT(clear_diagnostyka, RTC_UPDATING_HALTED_BIT);
        }
      }
      else
        _SET_BIT(clear_diagnostyka, RTC_UPDATING_HALTED_BIT);
      if ((read_write_i2c_buffer[0x1] & (1 << 7)) != 0)
        _SET_BIT(set_diagnostyka, RTC_OSCILLATOR_STOPED_BIT);
      else
        _SET_BIT(clear_diagnostyka, RTC_OSCILLATOR_STOPED_BIT);
      if ((read_write_i2c_buffer[0xF] & (1 << 4)) != 0)
        _SET_BIT(set_diagnostyka, RTC_BATTERY_LOW_BIT);
      else
        _SET_BIT(clear_diagnostyka, RTC_BATTERY_LOW_BIT);
      if ((read_write_i2c_buffer[0xF] & (1 << 2)) == 0)
        _SET_BIT(clear_diagnostyka, RTC_OSCILLATOR_FAIL_BIT);

      //Обновлюємо час
      reg_RTC[0] = read_write_i2c_buffer[0] & 0xff;
      reg_RTC[1] = read_write_i2c_buffer[1] & 0x7f;
      reg_RTC[2] = read_write_i2c_buffer[2] & 0x7f;
      reg_RTC[3] = read_write_i2c_buffer[3] & 0x3f;
      reg_RTC[4] = read_write_i2c_buffer[5] & 0x3f;
      reg_RTC[5] = read_write_i2c_buffer[6] & 0x1f;
      reg_RTC[6] = read_write_i2c_buffer[7] & 0xff;
      copy_register8_RTC = read_write_i2c_buffer[8];

      struct tm orig;
      unsigned int tmp_reg = reg_RTC[1];
      orig.tm_sec = 10 * (tmp_reg >> 4) + (tmp_reg & 0xf);

      tmp_reg = reg_RTC[2];
      orig.tm_min = 10 * (tmp_reg >> 4) + (tmp_reg & 0xf);

      tmp_reg = reg_RTC[3];
      orig.tm_hour = 10 * (tmp_reg >> 4) + (tmp_reg & 0xf);

      tmp_reg = reg_RTC[4];
      orig.tm_mday = 10 * (tmp_reg >> 4) + (tmp_reg & 0xf);

      tmp_reg = reg_RTC[5];
      orig.tm_mon = 10 * (tmp_reg >> 4) + (tmp_reg & 0xf) - 1;

      tmp_reg = reg_RTC[6];
      orig.tm_year = 10 * (tmp_reg >> 4) + (tmp_reg & 0xf) + 100;

      orig.tm_wday = 0;
      orig.tm_yday = 0;
      orig.tm_isdst = -1;

      lt_or_utc = true;
#if (__VER__ >= 8000000)
      _ForceReloadDstRules();
#endif
      //Робота з Watchdog
      watchdog_routine(UNITED_BITS_WATCHDOG, 7);
      time_t time_dat_tmp = mktime(&orig);
      lt_or_utc = false;
#if (__VER__ >= 8000000)
      _ForceReloadDstRules();
#endif
      tmp_reg = reg_RTC[0];
      int32_t time_ms_tmp = 100 * (tmp_reg >> 4) + 10 * (tmp_reg & 0xf);

      if ((state_i2c_task & STATE_FIRST_READING_RTC) != 0)
      {
        //Зараз відбувається перше зчитування даних з RTC - скидаємо біт першого читання
        state_i2c_task &= (unsigned int) (~STATE_FIRST_READING_RTC);
      }

      unsigned int realDateTime_tmp = true;
      if (
        (_CHECK_SET_BIT(diagnostyka, EVENT_START_SYSTEM_BIT) != 0) ||
        (_CHECK_SET_BIT(set_diagnostyka, EVENT_START_SYSTEM_BIT) != 0) ||
        (_CHECK_SET_BIT(diagnostyka, EVENT_RESTART_SYSTEM_BIT) != 0) ||
        (_CHECK_SET_BIT(set_diagnostyka, EVENT_RESTART_SYSTEM_BIT) != 0) ||
        (_CHECK_SET_BIT(diagnostyka, EVENT_SOFT_RESTART_SYSTEM_BIT) != 0) ||
        (_CHECK_SET_BIT(set_diagnostyka, EVENT_SOFT_RESTART_SYSTEM_BIT) != 0))
      {
        static unsigned int fixed_power_down_into_RTC;

        //До цього часу ще не зчитано першої реальної часової мітки
        if ((read_write_i2c_buffer[0xC] & (1 << 6)) != 0)
        {
          //Зчитано час, коли пропало живлення з RTC внаслідок виключення пристрою
          realDateTime_tmp = false;
          fixed_power_down_into_RTC = 1; //Виставляємо повідомлення про те, що було зафіксовано пропадання живлення на годиннику RTC до того, як програма стартувала спочатку

          if ((head_fifo_buffer_pr_err_records > 1) && (tail_fifo_buffer_pr_err_records == 0))
          {
            //Це перестраховка, бо завжди у цьому місці має бути як мінімум два записи в буфері FIFO подія про зупинку роботи приладу і старт/рестарт системи, а запис у мікросхему dataFlash ще  є забороненй
            unsigned int local_point_for_time = tail_fifo_buffer_pr_err_records * SIZE_ONE_RECORD_PR_ERR + 1; //Індекс першого числа часу у вже першому записі (перший запис відподає за подію - зупинку роботи приладу) (tail_fifo_buffer_pr_err_records має дорівнювати 0 у ціьому місці програми)
            //Час, який зараз прочитаний з RTC до моменту скидання біту HT вказує на час до пропадання живлення з годинника RTC
            for (unsigned int i = 0; i < sizeof(time_t); i++)
              buffer_pr_err_records[local_point_for_time + i] = *((unsigned char *) (&time_dat_tmp) + i);
            for (unsigned int i = 0; i < sizeof(int32_t); i++)
              buffer_pr_err_records[local_point_for_time + sizeof(time_t) + i] = *((unsigned char *) (&time_ms_tmp) + i);
          }
        }
        else
        {
          //Зчитано першу часову мітку з моменту запуску/перезапуску програми
          unsigned int local_point_for_time;

          if (fixed_power_down_into_RTC == 0)
          {
            //Це означає, що програма не зафікусала факт пропадання живлення на годиннику RTC до моменту запуску/перезапуску програми
            /*
            Тому подію про зупинку пристрою трбе очистити із очікуваних на запис
            шляхом встановлення "хвоста" буферу FIF0  в значення 1
            (індес 0 до моменту розблокування запису в реєстратор програмних подій,
            згідно розробленого програмного забезпечення  - це подія про зупинку роботи приладу,
            а 1 - це наступна подія)
            */
            if ((head_fifo_buffer_pr_err_records > 0) && (tail_fifo_buffer_pr_err_records == 0))
            {
              //Це перестраховка, бо завжди у цьому місці має бути як мінімум два записи в буфері FIFO подія про зупинку роботи приладу і старт/рестарт системи, а запис у мікросхему dataFlash ще  є забороненй
              /*
              Якби перша половина цієї умови (head_fifo_buffer_pr_err_records > 0)
              по незрозумілій причині для мене не виконувалася то тоді ми нічого не міняємо, і тоді 
              head_fifo_buffer_pr_err_records = 0 і tail_fifo_buffer_pr_err_records = 0,
              а це буде означати. що немає нових записів у черзі до запису і тому я думаю. що нічого 
              надзвичайного не мало б статися. Хоч це і неможливо, на мій погляд, і з точки зору ідеології
              розробленого програмного забезпечення.
              
              Якби ця умова по незрозумілій причині для мене не виконувалася повністю.
              То я з нову ж таки не можу зрозуміти, чого така виникла ситтуація, але думю. що тоді ніяких корекцій
              не варта робити, бо хоч це і неможливо, на мій погляд, і з точки зору ідеології
              розробленого програмного забезпечення  - аде я не думаю, що це б привело до неправильної 
              роботи "основної" частини програмного забезпечення
              */
              tail_fifo_buffer_pr_err_records += 1;
              while (tail_fifo_buffer_pr_err_records >= MAX_NUMBER_RECORDS_PR_ERR_INTO_BUFFER)
                tail_fifo_buffer_pr_err_records -= MAX_NUMBER_RECORDS_PR_ERR_INTO_BUFFER;
            }

            local_point_for_time = tail_fifo_buffer_pr_err_records * SIZE_ONE_RECORD_PR_ERR + 1; //Індекс першого числа часу у першому записі, який чекає на запис у DataFlash
          }
          else
          {
            local_point_for_time = (tail_fifo_buffer_pr_err_records + 1) * SIZE_ONE_RECORD_PR_ERR + 1; //Індекс першого числа часу у другому записі, який чекає на запис у DataFlash
          }

          //Встановлюємо часові мітки для тих подій, які мали місце до зчитування першої реальної часової мітки
          /*
          Змінна local_point_for_time зараз вказує на перше число часу другого запису (якщо була фіксація виключення живлення перед стартом),
          або першого запису (якщо пропадання живлення на RTC перед стартом програми не було зафіксовано)
          */

          //Блокуємо початок записування підготовлених записів до моменту гарантованого розставляння всіх часових міток
          temporary_block_writing_records_pr_err_into_DataFlash = 1;
          /*Цикл у циклі з однаковою умовою завершення я зробив для того, що 
          ця частина коду може у будь-який момент часу буде перервана вищепріоритетною
          роботою, яка сформує новий запис (але часову мітку ставить тоді,
          коли зняті події про запуск/перезапуск ). Крім того тут я знімаю в діагностіиці
          подію про запуск/перезапуск програми, що є також умовою блокування старту
          записування у мікросхему DataFlash і, саме головне, встановлення часових міток
          з модуля формування запису.
          Тому, теоретично може бути, що часові мітки жве всі виставлені, але до моменту
          зняття події запуску/перезапуску, сформувався новий запис у якого не буде часової мітки.
          Тому повторна перевірка після зняття заборони встановлення часових міток у модулі 
          формування запису, мала б повторно перейти до довстановлення часових міток
          у такоих записах.
          Аж після успішного повторного підтвердження розставляння часових міток буде розблоковно
          записування записів у мікросхему DataFlash.
          При цьому не треба боятися, що у такий момент, що часові сітки дозволені 
          і з модуля формування запису і дорозставляння часових міток з цієї частини програми
          буде втачена достовірність часової мітки, бо поки ми не вийдемо з цієї
          частини програми, то не буде запущено читання нового значення часу. Тобто
          на цей момент часу не боже бути "свіжішої" часової мітки, ніж та що є зараз. 
          І старшою часовою часовоюміткою може бути тільки мітка вимкнення приладу.
          */
          do
          {
            while (
              (local_point_for_time < (head_fifo_buffer_pr_err_records * SIZE_ONE_RECORD_PR_ERR)) &&
              (local_point_for_time < SIZE_BUFFER_FOR_PR_ERR))
            {
              for (unsigned int i = 0; i < sizeof(time_t); i++)
                buffer_pr_err_records[local_point_for_time + i] = *((unsigned char *) (&time_dat_tmp) + i);
              for (unsigned int i = 0; i < sizeof(int32_t); i++)
                buffer_pr_err_records[local_point_for_time + sizeof(time_t) + i] = *((unsigned char *) (&time_ms_tmp) + i);
              local_point_for_time += SIZE_ONE_RECORD_PR_ERR;
            }

            //Скидаємо біти запуску програми або її перезапуску
            _SET_BIT(clear_diagnostyka, EVENT_START_SYSTEM_BIT);
            _SET_BIT(clear_diagnostyka, EVENT_RESTART_SYSTEM_BIT);
            _SET_BIT(clear_diagnostyka, EVENT_SOFT_RESTART_SYSTEM_BIT);
          } while (
            (local_point_for_time < (head_fifo_buffer_pr_err_records * SIZE_ONE_RECORD_PR_ERR)) &&
            (local_point_for_time < SIZE_BUFFER_FOR_PR_ERR));
          //Розблоковуємо початок записування підготовлених записів, бо я вважаю, що часові мітки я вже гарантовано розставив
          temporary_block_writing_records_pr_err_into_DataFlash = 0;
        }
      }

      //Скидаємо повідомлення про читання системного часу
      _CLEAR_BIT(control_i2c_taskes, TASK_READING_RTC_BIT);

      if (realDateTime_tmp)
      {
        //Обновлюємо час
        //        TEST_OUTPUT->BSRRL = TEST_OUTPUT_PIN;
        copying_time_to_RTC = 2;
        time_dat_RTC = time_dat_tmp;
        time_ms_RTC = time_ms_tmp;
        calibration = copy_register8_RTC & 0x3f;

        copying_time_to_RTC = 1;
      }
      else
      {
        _SET_BIT(control_i2c_taskes, TASK_START_READ_RTC_BIT);
        _SET_BIT(control_i2c_taskes, TASK_BLK_OPERATION_BIT);
      }
    }
    else if (_CHECK_SET_BIT(control_i2c_taskes, TASK_WRITING_RTC_BIT) != 0)
    {
      //Виставляємо команду читання системного часу
      _SET_BIT(control_i2c_taskes, TASK_START_READ_RTC_BIT);
      _SET_BIT(control_i2c_taskes, TASK_BLK_OPERATION_BIT);

      _CLEAR_BIT(control_i2c_taskes, TASK_WRITING_RTC_BIT);
    }
    else if (_CHECK_SET_BIT(control_i2c_taskes, TASK_SET_START_SETTINGS_RTC_BIT) != 0)
    {
      etap_settings_test_frequency++;
      if (etap_settings_test_frequency > 2)
      {
        etap_settings_test_frequency = -1;

        //Повторно запускаємо читання регістрів RTC
        _SET_BIT(control_i2c_taskes, TASK_START_READ_RTC_BIT);
        _SET_BIT(control_i2c_taskes, TASK_BLK_OPERATION_BIT);

        //Скидаємо біт виставляння бітів і полів у потрібні значення, якщо всі поля записані
        _CLEAR_BIT(control_i2c_taskes, TASK_SET_START_SETTINGS_RTC_BIT);
      }
    }
    else if (_CHECK_SET_BIT(control_i2c_taskes, TASK_RESET_ST_RTC_BIT) != 0)
    {
      //Скидаємо біт запуску осцилятора RTC
      _CLEAR_BIT(control_i2c_taskes, TASK_RESET_ST_RTC_BIT);
    }
    else if (_CHECK_SET_BIT(control_i2c_taskes, TASK_RESET_OF_RTC_BIT) != 0)
    {
      etap_reset_of_bit++;
      if (etap_reset_of_bit == ETAP_CLEAR_OF_ST_START_WAITING_5S)
      {
        //Скидаємо запущені операції попереднього  скидання OF-біту, але ще маємо витримати 5с для остаточного скидання OF-біту
        _CLEAR_BIT(control_i2c_taskes, TASK_RESET_OF_RTC_BIT);
      }
      else if (etap_reset_of_bit >= ETAP_CLEAR_OF_WRITE_OF_0)
      {
        //Скидаємо запущені операції скидання OF-біту
        etap_reset_of_bit = ETAP_CLEAR_OF_NONE;
        _CLEAR_BIT(control_i2c_taskes, TASK_RESET_OF_RTC_BIT);
      }
    }
    else
    {
      //Сюди, теоретично, ніколи не мала б дійти
      //У всіх інакших випадках нічого не робимо
    }
    /*************************************************/
  }
  else if (driver_i2c.state_execution == 2)
  {
    /*************************************************/
    //Обмін завершився з помилкою
    /*************************************************/

    //Покищо просто очищаємо змінну, яка конкретизуєм помилку, у майбутньому її можна буде конкретизувати
    type_error_of_exchanging_via_i2c = 0;

    //Виставляємо повідомлення, що I2C готовий до нової транзакції
    driver_i2c.state_execution = -1;

    //Визначаємося з наступними діями
    if (_CHECK_SET_BIT(control_i2c_taskes, TASK_READING_RTC_BIT) != 0)
    {
      //Стоїть умова читання RTC

      //Повторно запускаємо процес читання RTC
      _SET_BIT(control_i2c_taskes, TASK_START_READ_RTC_BIT);
      _SET_BIT(control_i2c_taskes, TASK_BLK_OPERATION_BIT);
      _CLEAR_BIT(control_i2c_taskes, TASK_READING_RTC_BIT);
    }
    else if (_CHECK_SET_BIT(control_i2c_taskes, TASK_WRITING_RTC_BIT) != 0)
    {
      //Стоїть умова запису RTC

      //Повторно запускаємо процес запису RTC
      _SET_BIT(control_i2c_taskes, TASK_START_WRITE_RTC_BIT);
      _SET_BIT(control_i2c_taskes, TASK_BLK_OPERATION_BIT);
      _CLEAR_BIT(control_i2c_taskes, TASK_WRITING_RTC_BIT);
    }
    else if (
      (_CHECK_SET_BIT(control_i2c_taskes, TASK_RESET_OF_RTC_BIT) != 0) ||
      (_CHECK_SET_BIT(control_i2c_taskes, TASK_SET_START_SETTINGS_RTC_BIT) != 0) ||
      (_CHECK_SET_BIT(control_i2c_taskes, TASK_RESET_ST_RTC_BIT) != 0))
    {
      //Повторно запускаємо операцію з того самого місця, яка не вийшла
    }
    else
    {
      //У всіх інакших випадках нічого не робимо
    }
    /*************************************************/
  }
}
/*****************************************************/

/*****************************************************/
//Перенастроювання I2C при періодичному неуспішному запуску трансакції
/*****************************************************/
void error_start_i2c(void)
{
  _SET_BIT(set_diagnostyka, ERROR_START_VIA_I2C_BIT);

  /**************/
  //Переконфігуровуємо I2C
  /**************/
  //Зупиняємо геренацію переривань від I2C
  I2C_ITConfig(I2C, I2C_IT_ERR | I2C_IT_EVT | I2C_IT_BUF, DISABLE);

  //Зупиняємо потоки DMA якщо вони запущені
  if ((DMA_StreamI2C_Tx->CR & (uint32_t) DMA_SxCR_EN) != 0)
    DMA_StreamI2C_Tx->CR &= ~(uint32_t) DMA_SxCR_EN;
  if ((DMA_StreamI2C_Rx->CR & (uint32_t) DMA_SxCR_EN) != 0)
    DMA_StreamI2C_Rx->CR &= ~(uint32_t) DMA_SxCR_EN;

  //Переконфігуровуємо I2C
  Configure_I2C(I2C);
  /**************/
}
/*****************************************************/

/*****************************************************/
//
/*****************************************************/
/*****************************************************/
