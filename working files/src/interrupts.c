#include "header.h"

#ifdef _TEST_DURATION
uint64_t durTIM5_C1Cur = 0;
uint64_t durTIM5_C2Cur = 0;
#endif

#define BACKLIGHTING_ON 1
#define BACKLIGHTING_OFF 300

uint32_t time_backlighting = BACKLIGHTING_ON * 100;

/*****************************************************/
//Перевірка на помилки у процесі транзакції черз I2C
/*****************************************************/
inline ErrorStatus check_errors_i2c(void)
{
  if (I2C_GetFlagStatus(I2C, I2C_FLAG_TIMEOUT | I2C_FLAG_OVR | I2C_FLAG_AF | I2C_FLAG_ARLO | I2C_FLAG_BERR))
  {
    I2C_ClearFlag(I2C, I2C_FLAG_TIMEOUT | I2C_FLAG_OVR | I2C_FLAG_AF | I2C_FLAG_ARLO | I2C_FLAG_BERR);
    return ERROR;
  }
  else
    return SUCCESS;
}
/*****************************************************/

/*****************************************************/
//Перевірка стану кнопки
/*****************************************************/
inline void check_state_key(GPIO_TypeDef *GPIOx, uint16_t mask_bit, unsigned int number_bit)
{
  if ((time_set_keyboard[number_bit] == 0) && ((new_state_keyboard & (1 << number_bit)) == 0))
  {
    //Кнопка до цього моменту не була зафіксована, що вона натиснута
    if (GPIO_ReadInputDataBit(GPIOx, mask_bit) == RESET)
      time_set_keyboard[number_bit]++;
  }
  else if ((time_set_keyboard[number_bit] > 0) && (time_set_keyboard[number_bit] < DEREVIACIA))
  {
    //Перевіряємо чи пройшов час очікування, і якщо стан кнопки підтверджується, то виставляємо повідомлення. що кнопка натиснута, алеще не оброблена
    if (++time_set_keyboard[number_bit] == DEREVIACIA)
    {
      if (GPIO_ReadInputDataBit(GPIOx, mask_bit) == RESET)
        new_state_keyboard |= 1 << number_bit;
      else
        time_set_keyboard[number_bit] = 0;
    }
  }
  else
  {
    if (time_set_keyboard[number_bit] != 0)
    {
      //Перевіряємо чи кнопка вже відтиснута
      if (GPIO_ReadInputDataBit(GPIOx, mask_bit) != RESET)
        time_set_keyboard[number_bit] = 0;
    }
  }
}
/*****************************************************/

/*****************************************************/
//Переривання від I2C
/*****************************************************/
void I2C_EV_IRQHandler(void)
{
#ifdef SYSTEM_VIEWER_ENABLE
  SEGGER_SYSVIEW_RecordEnterISR();
#endif

  switch (I2C_GetLastEvent(I2C))
  {
    case I2C_EVENT_MASTER_MODE_SELECT: /* EV5 */
      {
        if ((driver_i2c.action == 0) || (driver_i2c.action == 2))
        {
          /* Посилаємо EEPROM адресу для запису */
          I2C->DR = (uint8_t)(driver_i2c.device_id & ((uint8_t)(~I2C_OAR1_ADD0)));
        }
        else
        {
          /* Посилаємо EEPROM адресу для читання */
          I2C->DR = (uint8_t)(driver_i2c.device_id | I2C_OAR1_ADD0);
        }
        break;
      }
    case I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED: /* EV6 */
      {
        //Зупиняємо генерацію переривань від подій
        I2C->CR2 &= (uint16_t)((~(uint32_t) I2C_IT_EVT) & 0xffff);

        //Помилки не зафіксовані
        unsigned int number_transmit_with_i2c = 0;
        if (driver_i2c.action == 0)
        {
          //Підготовлюємо буфер для передачі в мікросхему для запису (адреса)
          //        if (driver_i2c.device_id == EEPROM_ADDRESS)
          //        {
          //          //Внутрішня адреса для EEPROM становить 2 байти
          //          Temporaty_I2C_Buffer[0] = ((driver_i2c.internal_address & 0xFF00) >> 8);
          //          Temporaty_I2C_Buffer[1] = (driver_i2c.internal_address & 0x00FF);
          //          number_transmit_with_i2c = 2;
          //        }
          //        else
          {
            //Внутрішня адреса для RTC становить 1 байт
            Temporaty_I2C_Buffer[0] = (driver_i2c.internal_address & 0x00FF);
            number_transmit_with_i2c = 1;
          }
        }
        else if (driver_i2c.action == 2)
        {
          //Підготовлюємо буфер для передачі в мікросхему для запису (адреса + корисні дані)
          //        if (driver_i2c.device_id == EEPROM_ADDRESS)
          //        {
          //          //Запис корисних байт іде разом із двома байтами внутрішньої адреси для EEPROM
          //          Temporaty_I2C_Buffer[0] = ((driver_i2c.internal_address & 0xFF00) >> 8);
          //          Temporaty_I2C_Buffer[1] = (driver_i2c.internal_address & 0x00FF);
          //          for (unsigned int i= 0; i < driver_i2c.number_bytes; i++)
          //          {
          //            Temporaty_I2C_Buffer[2 + i] = *(driver_i2c.point_buffer + i);
          //          }
          //          number_transmit_with_i2c = 2 + driver_i2c.number_bytes;
          //        }
          //        else
          {
            //Запис корисних байт іде разом із одним байтами внутрішньої адреси для RTC
            Temporaty_I2C_Buffer[0] = (driver_i2c.internal_address & 0x00FF);
            for (unsigned int i = 0; i < driver_i2c.number_bytes; i++)
            {
              Temporaty_I2C_Buffer[1 + i] = *(driver_i2c.point_buffer + i);
            }
            number_transmit_with_i2c = 1 + driver_i2c.number_bytes;
          }
        }

        //Зупиняємо потік DMA якщо він запущений
        if ((DMA_StreamI2C_Tx->CR & (uint32_t) DMA_SxCR_EN) != 0)
          DMA_StreamI2C_Tx->CR &= ~(uint32_t) DMA_SxCR_EN;
        DMA_StreamI2C_Tx->M0AR = (uint32_t) Temporaty_I2C_Buffer;
        DMA_StreamI2C_Tx->NDTR = number_transmit_with_i2c;

        /* Set Last bit to have a NACK on the last received byte */
        I2C->CR2 |= I2C_CR2_LAST;

        //Очищаємо всі події по DMA_FLAG_GLI2C_Tx
        DMA_ClearFlag(DMA_StreamI2C_Tx, DMA_FLAG_TCI2C_Tx | DMA_FLAG_HTI2C_Tx | DMA_FLAG_TEII2C_Tx | DMA_FLAG_DMEII2C_Tx | DMA_FLAG_FEII2C_Tx);

        //Дозволяємо генерацію переривань від потоку DMA1_ChannelI2C_TX
        DMA_StreamI2C_Tx->CR |= DMA_IT_TC;

        /* Дозволяємо для I2C передачу по DMA */
        I2C->CR2 |= I2C_CR2_DMAEN;

        /* Дозволяємо DMA_StreamI2C_Tx */
        DMA_StreamI2C_Tx->CR |= (uint32_t) DMA_SxCR_EN;

        break;
      }
    case I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED:
      {
        //Зупиняємо генерацію переривань від подій
        I2C->CR2 &= (uint16_t)((~(uint32_t) I2C_IT_EVT) & 0xffff);

        if (driver_i2c.number_bytes != 1)
        {
          //Якщо читання буде проводитися більше ніж один байт, то читання здійснюємо через DMA_FLAG_GLI2C_Rx

          //Зупиняємо потік DMA якщо він запущений
          if ((DMA_StreamI2C_Rx->CR & (uint32_t) DMA_SxCR_EN) != 0)
            DMA_StreamI2C_Rx->CR &= ~(uint32_t) DMA_SxCR_EN;
          DMA_StreamI2C_Rx->M0AR = (uint32_t) driver_i2c.point_buffer;
          DMA_StreamI2C_Rx->NDTR = driver_i2c.number_bytes;

          //Очищаємо всі події по DMA_FLAG_GLI2C_Rx
          DMA_ClearFlag(DMA_StreamI2C_Rx, DMA_FLAG_TCI2C_Rx | DMA_FLAG_HTI2C_Rx | DMA_FLAG_TEII2C_Rx | DMA_FLAG_DMEII2C_Rx | DMA_FLAG_FEII2C_Rx);

          //Дозволяэмо генерацыю переривань від потоку DMA1_ChannelI2C_RX
          DMA_StreamI2C_Rx->CR |= DMA_IT_TC;

          /* Дозволяємо для I2C передачу по DMA */
          I2C->CR2 |= I2C_CR2_DMAEN;

          /* Дозволяємо DMA1_ChannelI2C_RX */
          DMA_StreamI2C_Rx->CR |= (uint32_t) DMA_SxCR_EN;
        }
        else
        {
          //Читання буде проходити тільки одного байту без DMA_FLAG_GLI2C_Rx

          I2C->CR1 &= (uint16_t)(~I2C_CR1_ACK);
          I2C->CR1 |= I2C_CR1_STOP;

          /* Дозволяємо для I2C генерацію переривань по подіях і прийятому новому байту*/
          I2C->CR2 |= I2C_IT_EVT | I2C_IT_BUF;
        }

        break;
      }
    case I2C_EVENT_MASTER_BYTE_RECEIVED_NO_BUSY:
      {
        /* Забороняємо для I2C генерацію переривань по подіях і прийятому новому байту*/
        I2C->CR2 &= (uint16_t) ~(I2C_IT_EVT | I2C_IT_BUF);
        *driver_i2c.point_buffer = (uint8_t) I2C->DR;

        //Повідомляємо, що читання завершився
        driver_i2c.state_execution = 1;
        break;
      }
    default:
      {
        break;
      }
  }

#ifdef SYSTEM_VIEWER_ENABLE
  SEGGER_SYSVIEW_RecordExitISR();
#endif
}
/*****************************************************/

/*****************************************************/
//Генерація переривань від помилки під час обміну по I2C
/*****************************************************/
void I2C_ER_IRQHandler(void)
{
#ifdef SYSTEM_VIEWER_ENABLE
  SEGGER_SYSVIEW_RecordEnterISR();
#endif

  I2C->CR1 |= I2C_CR1_STOP;

  //Зупиняємо потоки DMA якщо вони запущені
  if ((DMA_StreamI2C_Tx->CR & (uint32_t) DMA_SxCR_EN) != 0)
    DMA_StreamI2C_Tx->CR &= ~(uint32_t) DMA_SxCR_EN;
  if ((DMA_StreamI2C_Rx->CR & (uint32_t) DMA_SxCR_EN) != 0)
    DMA_StreamI2C_Rx->CR &= ~(uint32_t) DMA_SxCR_EN;

  check_errors_i2c();

  type_error_of_exchanging_via_i2c |= (1 << ERROR_FIX_ERRORS_BIT);
  driver_i2c.state_execution = 2;

#ifdef SYSTEM_VIEWER_ENABLE
  SEGGER_SYSVIEW_RecordExitISR();
#endif
}
/*****************************************************/

/*****************************************************/
//Перереривання від завершення передачі DMA_StreamI2C_Tx
/*****************************************************/
void DMA_StreamI2C_Tx_IRQHandler(void)
{
#ifdef SYSTEM_VIEWER_ENABLE
  SEGGER_SYSVIEW_RecordEnterISR();
#endif

  //Забороняємо генерацію переривань від потоку DMA_StreamI2C_Tx
  DMA_StreamI2C_Tx->CR &= ~DMA_IT_TC;

  //Зупиняємо потік DMA
  DMA_StreamI2C_Tx->CR &= ~(uint32_t) DMA_SxCR_EN;
  DMA_StreamI2C_Tx->NDTR = 0;

  /* Забороняємо для I2C передачу по DMA */
  I2C->CR2 &= (uint16_t)(~I2C_CR2_DMAEN);

  /* Очищаємо прапореці, що сигналізує про завершення передачі даних для DMA1 по потоку I2C_TX */
  DMA_ClearFlag(DMA_StreamI2C_Tx, DMA_FLAG_TCI2C_Tx | DMA_FLAG_HTI2C_Tx | DMA_FLAG_TEII2C_Tx | DMA_FLAG_DMEII2C_Tx | DMA_FLAG_FEII2C_Tx);

  //Чекаємо, поки останній байт успішно переасться
  while (I2C_GetFlagStatus(I2C, I2C_FLAG_BTF) == RESET)
    ;

  if (driver_i2c.action == 0)
  {
    //Переходимо в режим зчитування
    driver_i2c.action = 1;
    /* Посилаємо RESTART умову */
    I2C->CR1 |= I2C_CR1_START;
    /* Дозволяємо для I2C генерацію переривань по подіях */
    I2C->CR2 |= I2C_IT_EVT;
  }
  else if (driver_i2c.action == 2)
  {
    /* Посилаємо STOP умову */
    I2C->CR1 |= I2C_CR1_STOP;
    //Повідомляємо, що запис завершився
    driver_i2c.state_execution = 1;
  }

#ifdef SYSTEM_VIEWER_ENABLE
  SEGGER_SYSVIEW_RecordExitISR();
#endif
}
/*****************************************************/

/*****************************************************/
//Перереривання від завершення передачі DMA_StreamI2C_Rx
/*****************************************************/
void DMA_StreamI2C_Rx_IRQHandler(void)
{
#ifdef SYSTEM_VIEWER_ENABLE
  SEGGER_SYSVIEW_RecordEnterISR();
#endif

  //Забороняємо генерацію переривань від потоку DMA_StreamI2C_Rx
  DMA_StreamI2C_Rx->CR &= ~DMA_IT_TC;

  //Зупиняємо потік DMA
  DMA_StreamI2C_Rx->CR &= ~(uint32_t) DMA_SxCR_EN;
  DMA_StreamI2C_Rx->NDTR = 0;

  /* Забороняємо для I2C передачу по DMA */
  I2C->CR2 &= (uint16_t)(~I2C_CR2_DMAEN);

  /* Очищаємо прапореці, що сигналізує про завершення передачі даних для DMA1 по каналу I2C_TX */
  DMA_ClearFlag(DMA_StreamI2C_Rx, DMA_FLAG_TCI2C_Rx | DMA_FLAG_HTI2C_Rx | DMA_FLAG_TEII2C_Rx | DMA_FLAG_DMEII2C_Rx | DMA_FLAG_FEII2C_Rx);

  /* Посилаємо STOP умову */
  I2C->CR1 |= I2C_CR1_STOP;
  //Повідомляємо, що читання завершився
  driver_i2c.state_execution = 1;

#ifdef SYSTEM_VIEWER_ENABLE
  SEGGER_SYSVIEW_RecordExitISR();
#endif
}
/*****************************************************/

/*****************************************************/
//Діагностика необхідно-приєднаних плат
/*****************************************************/
inline void diagnostyka_boards(void)
{
  unsigned int diagnostyka_tmp[N_DIAGN];
  for (size_t i = 0; i < N_DIAGN; i++)
  {
    diagnostyka_tmp[i] = diagnostyka[i];

    diagnostyka_tmp[i] &= (unsigned int) (~clear_diagnostyka[i]);
    diagnostyka_tmp[i] |= set_diagnostyka[i];
  }

  uint32_t board_register_tmp = _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD39_DD40_DD47);
  uint32_t board_register_diff = board_register_tmp ^ board_register;
  board_register = board_register_tmp;

  if ((board_register_diff & 0x01) && ((board_register_tmp & 0x01) != 0x1))
    _SET_BIT(set_diagnostyka, ERROR_BA_1_FIX);
  else if ((board_register_diff & 0x01) || _CHECK_SET_BIT(diagnostyka_tmp, ERROR_BA_1_CTLR))
  {
    _SET_BIT(clear_diagnostyka, ERROR_BA_1_FIX);
    _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD39_DD40_DD47) = 0x1;
    if ((_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD39_DD40_DD47) >> 8) != 0x11)
      _SET_BIT(set_diagnostyka, ERROR_BA_1_CTLR);
    else
      _SET_BIT(clear_diagnostyka, ERROR_BA_1_CTLR);
    _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD39_DD40_DD47) = 0x0;
  }
#if (                             \
  (MODYFIKACIA_VERSII_PZ == 8) && \
  (MODYFIKACIA_VERSII_PZ == 18))
  if ((board_register_diff & 0x02) && ((board_register_tmp & 0x02) != 0x2))
    _SET_BIT(set_diagnostyka, ERROR_BDV2_FIX);
  else if ((board_register_diff & 0x02) || _CHECK_SET_BIT(diagnostyka_tmp, ERROR_BDV2_CTLR))
  {
    _SET_BIT(clear_diagnostyka, ERROR_BDV2_FIX);
    _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD39_DD40_DD47) = 0x2; //БДВ2_D

    if ((_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD33_DD36) >> 8) != 0x59)
      _SET_BIT(set_diagnostyka, ERROR_BDV2_CTLR);
    else
      _SET_BIT(clear_diagnostyka, ERROR_BDVV5_1_CTLR);
    _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD39_DD40_DD47) = 0x0; //Clear BDV2_CTLR
  }
#else
  if ((board_register_diff & 0x02) && ((board_register_tmp & 0x02) != 0x2))
    _SET_BIT(set_diagnostyka, ERROR_BDVV5_1_FIX);
  else if ((board_register_diff & 0x02) || _CHECK_SET_BIT(diagnostyka_tmp, ERROR_BDVV5_1_CTLR))
  {
    _SET_BIT(clear_diagnostyka, ERROR_BDVV5_1_FIX);
    _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD39_DD40_DD47) = 0x2;
    if ((_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD33_DD36) & 0xff) != 0x25)
      _SET_BIT(set_diagnostyka, ERROR_BDVV5_1_CTLR);
    else
      _SET_BIT(clear_diagnostyka, ERROR_BDVV5_1_CTLR);
    _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD39_DD40_DD47) = 0x0;
  }
#endif

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
  if ((board_register_diff & 0x08) && ((board_register_tmp & 0x08) != 0x8))
    _SET_BIT(set_diagnostyka, ERROR_CB_FIX);
  else if (board_register_diff & 0x08)
  {
    _SET_BIT(clear_diagnostyka, ERROR_CB_FIX);
  }
#endif

#if (                              \
  (MODYFIKACIA_VERSII_PZ == 0) ||  \
  (MODYFIKACIA_VERSII_PZ == 1) ||  \
  (MODYFIKACIA_VERSII_PZ == 3) ||  \
  (MODYFIKACIA_VERSII_PZ == 5) ||  \
  (MODYFIKACIA_VERSII_PZ == 6) ||  \
  (MODYFIKACIA_VERSII_PZ == 10) || \
  (MODYFIKACIA_VERSII_PZ == 11) || \
  (MODYFIKACIA_VERSII_PZ == 13) || \
  (MODYFIKACIA_VERSII_PZ == 15) || \
  (MODYFIKACIA_VERSII_PZ == 15) || \
  (MODYFIKACIA_VERSII_PZ == 23) || \
  (MODYFIKACIA_VERSII_PZ == 26) || \
  (MODYFIKACIA_VERSII_PZ == 33))
  if ((board_register_diff & 0x04) && ((board_register_tmp & 0x04) != 0x4))
    _SET_BIT(set_diagnostyka, ERROR_BDVV5_2_FIX);
  else if ((board_register_diff & 0x04) || _CHECK_SET_BIT(diagnostyka_tmp, ERROR_BDVV5_2_CTLR))
  {
    _SET_BIT(clear_diagnostyka, ERROR_BDVV5_2_FIX);
    _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD39_DD40_DD47) = 0x4;
    if ((_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD33_DD36) >> 8) != 0x37)
      _SET_BIT(set_diagnostyka, ERROR_BDVV5_2_CTLR);
    else
      _SET_BIT(clear_diagnostyka, ERROR_BDVV5_2_CTLR);
    _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD39_DD40_DD47) = 0x0;
  }
#elif (                           \
  (MODYFIKACIA_VERSII_PZ == 8) || \
  (MODYFIKACIA_VERSII_PZ == 18))
  if ((board_register_diff & 0x04) && ((board_register_tmp & 0x04) != 0x4))
    _SET_BIT(set_diagnostyka, ERROR_BDV5_FIX);
  else if ((board_register_diff & 0x04) || _CHECK_SET_BIT(diagnostyka_tmp, ERROR_BDV5_CTLR))
  {
    _SET_BIT(clear_diagnostyka, ERROR_BDVV5_2_FIX);
    _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD39_DD40_DD47) = 0x4;
    if ((_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD33_DD36) & 0xff) != 0x6D)
      _SET_BIT(set_diagnostyka, ERROR_BDV5_CTLR);
    else
      _SET_BIT(clear_diagnostyka, ERROR_BDVV5_2_CTLR);
    _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD39_DD40_DD47) = 0x0; //CLr
  }
//#elif (                                \
//       (MODYFIKACIA_VERSII_PZ == 6) || \
//       (MODYFIKACIA_VERSII_PZ == 26)   \
//      )
//      if ((board_register_diff & 0x04) && ((board_register_tmp & 0x04) !=  0x4)) _SET_BIT(set_diagnostyka, ERROR_BDVV9_FIX);
//      else if ((board_register_diff & 0x04) || _CHECK_SET_BIT(diagnostyka_tmp, ERROR_BDVV9_CTLR))
//      {
//        _SET_BIT(clear_diagnostyka, ERROR_BDVV9_FIX);
//        _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD39_DD40_DD47) = 0x4;
//        if ((_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD33_DD36) >> 8) != 0x29)  _SET_BIT(set_diagnostyka, ERROR_BDVV9_CTLR);
//        else _SET_BIT(clear_diagnostyka, ERROR_BDVV9_CTLR);
//        _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD39_DD40_DD47) = 0x0;
//      }
#elif (                           \
  (MODYFIKACIA_VERSII_PZ == 7) || \
  (MODYFIKACIA_VERSII_PZ == 17))
  if ((board_register_diff & 0x04) && ((board_register_tmp & 0x04) != 0x4))
    _SET_BIT(set_diagnostyka, ERROR_BDVV_DZ_FIX);
  else if ((board_register_diff & 0x04) || _CHECK_SET_BIT(diagnostyka_tmp, ERROR_BDVV_DZ_CTLR))
  {
    _SET_BIT(clear_diagnostyka, ERROR_BDVV_DZ_FIX);
    _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD39_DD40_DD47) = 0x4;
    if ((_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD33_DD36) >> 8) != 0xA7)
      _SET_BIT(set_diagnostyka, ERROR_BDVV_DZ_CTLR);
    else
      _SET_BIT(clear_diagnostyka, ERROR_BDVV_DZ_CTLR);
    _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD39_DD40_DD47) = 0x0;
  }
#endif

#if (                              \
  (MODYFIKACIA_VERSII_PZ == 0) ||  \
  (MODYFIKACIA_VERSII_PZ == 3) ||  \
  (MODYFIKACIA_VERSII_PZ == 4) ||  \
  (MODYFIKACIA_VERSII_PZ == 5) ||  \
  (MODYFIKACIA_VERSII_PZ == 6) ||  \
  (MODYFIKACIA_VERSII_PZ == 7) ||  \
  (MODYFIKACIA_VERSII_PZ == 8) ||  \
  (MODYFIKACIA_VERSII_PZ == 10) || \
  (MODYFIKACIA_VERSII_PZ == 13) || \
  (MODYFIKACIA_VERSII_PZ == 14) || \
  (MODYFIKACIA_VERSII_PZ == 15) || \
  (MODYFIKACIA_VERSII_PZ == 17) || \
  (MODYFIKACIA_VERSII_PZ == 18) || \
  (MODYFIKACIA_VERSII_PZ == 15) || \
  (MODYFIKACIA_VERSII_PZ == 23) || \
  (MODYFIKACIA_VERSII_PZ == 24) || \
  (MODYFIKACIA_VERSII_PZ == 26) || \
  (MODYFIKACIA_VERSII_PZ == 33) || \
  (MODYFIKACIA_VERSII_PZ == 34))
  if ((board_register_diff & 0x10) && ((board_register_tmp & 0x010) != 0x10))

#if (                              \
  (MODYFIKACIA_VERSII_PZ == 0) ||  \
  (MODYFIKACIA_VERSII_PZ == 10) || \
  (MODYFIKACIA_VERSII_PZ == 10))
    _SET_BIT(set_diagnostyka, ERROR_BDV_DZ_FIX);
#elif (                            \
  (MODYFIKACIA_VERSII_PZ == 5) ||  \
  (MODYFIKACIA_VERSII_PZ == 8) ||  \
  (MODYFIKACIA_VERSII_PZ == 15) || \
  (MODYFIKACIA_VERSII_PZ == 18))
    _SET_BIT(set_diagnostyka, ERROR_BDVV6_FIX);
#elif (                            \
  (MODYFIKACIA_VERSII_PZ == 3) ||  \
  (MODYFIKACIA_VERSII_PZ == 4) ||  \
  (MODYFIKACIA_VERSII_PZ == 6) ||  \
  (MODYFIKACIA_VERSII_PZ == 13) || \
  (MODYFIKACIA_VERSII_PZ == 14))
    _SET_BIT(set_diagnostyka, ERROR_BDZ_FIX);
#elif (                            \
  (MODYFIKACIA_VERSII_PZ == 7) ||  \
  (MODYFIKACIA_VERSII_PZ == 17) || \
  (MODYFIKACIA_VERSII_PZ == 23) || \
  (MODYFIKACIA_VERSII_PZ == 24) || \
  (MODYFIKACIA_VERSII_PZ == 26) || \
  (MODYFIKACIA_VERSII_PZ == 33) || \
  (MODYFIKACIA_VERSII_PZ == 34))
    _SET_BIT(set_diagnostyka, ERROR_BDSH_FIX);
#else
#error "UDEFINED MODIFIKACIA"
#endif
  else if (
    (board_register_diff & 0x10)
#if (                              \
  (MODYFIKACIA_VERSII_PZ == 0) ||  \
  (MODYFIKACIA_VERSII_PZ == 10) || \
  (MODYFIKACIA_VERSII_PZ == 10))
    || _CHECK_SET_BIT(diagnostyka_tmp, ERROR_BDV_DZ_CTLR))
  {
    _SET_BIT(clear_diagnostyka, ERROR_BDV_DZ_FIX);
#elif (                            \
  (MODYFIKACIA_VERSII_PZ == 5) ||  \
  (MODYFIKACIA_VERSII_PZ == 8) ||  \
  (MODYFIKACIA_VERSII_PZ == 15) || \
  (MODYFIKACIA_VERSII_PZ == 18))
    || _CHECK_SET_BIT(diagnostyka_tmp, ERROR_BDVV6_CTLR))
  {
    _SET_BIT(clear_diagnostyka, ERROR_BDVV6_FIX);
#elif (                            \
  (MODYFIKACIA_VERSII_PZ == 3) ||  \
  (MODYFIKACIA_VERSII_PZ == 4) ||  \
  (MODYFIKACIA_VERSII_PZ == 6) ||  \
  (MODYFIKACIA_VERSII_PZ == 13) || \
  (MODYFIKACIA_VERSII_PZ == 14))
    || _CHECK_SET_BIT(diagnostyka_tmp, ERROR_BDZ_CTLR))
  {
    _SET_BIT(clear_diagnostyka, ERROR_BDZ_FIX);
#elif (                            \
  (MODYFIKACIA_VERSII_PZ == 7) ||  \
  (MODYFIKACIA_VERSII_PZ == 17) || \
  (MODYFIKACIA_VERSII_PZ == 23) || \
  (MODYFIKACIA_VERSII_PZ == 24) || \
  (MODYFIKACIA_VERSII_PZ == 26) || \
  (MODYFIKACIA_VERSII_PZ == 33) || \
  (MODYFIKACIA_VERSII_PZ == 34))
    || _CHECK_SET_BIT(diagnostyka_tmp, ERROR_BDSH_CTLR))
  {
    _SET_BIT(clear_diagnostyka, ERROR_BDSH_FIX);
#else
    )
    {
#error "UDEFINED MODIFIKACIA"
#endif

    _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD39_DD40_DD47) = 0x10;

#if (                              \
  (MODYFIKACIA_VERSII_PZ == 0) ||  \
  (MODYFIKACIA_VERSII_PZ == 10) || \
  (MODYFIKACIA_VERSII_PZ == 10))
    if ((_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD26_DD29) >> 8) != 0x14)
      _SET_BIT(set_diagnostyka, ERROR_BDV_DZ_CTLR);
    else
      _SET_BIT(clear_diagnostyka, ERROR_BDV_DZ_CTLR);
#elif (                            \
  (MODYFIKACIA_VERSII_PZ == 5) ||  \
  (MODYFIKACIA_VERSII_PZ == 8) ||  \
  (MODYFIKACIA_VERSII_PZ == 15) || \
  (MODYFIKACIA_VERSII_PZ == 18))
    if ((_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD26_DD29) >> 8) != 0x12)
      _SET_BIT(set_diagnostyka, ERROR_BDVV6_CTLR);
    else
      _SET_BIT(clear_diagnostyka, ERROR_BDVV6_CTLR);
#elif (                            \
  (MODYFIKACIA_VERSII_PZ == 3) ||  \
  (MODYFIKACIA_VERSII_PZ == 4) ||  \
  (MODYFIKACIA_VERSII_PZ == 6) ||  \
  (MODYFIKACIA_VERSII_PZ == 13) || \
  (MODYFIKACIA_VERSII_PZ == 14))
    if ((_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD26_DD29) >> 8) != 0x18)
      _SET_BIT(set_diagnostyka, ERROR_BDZ_CTLR);
    else
      _SET_BIT(clear_diagnostyka, ERROR_BDZ_CTLR);
#elif (                            \
  (MODYFIKACIA_VERSII_PZ == 7) ||  \
  (MODYFIKACIA_VERSII_PZ == 17) || \
  (MODYFIKACIA_VERSII_PZ == 23) || \
  (MODYFIKACIA_VERSII_PZ == 24) || \
  (MODYFIKACIA_VERSII_PZ == 26) || \
  (MODYFIKACIA_VERSII_PZ == 33) || \
  (MODYFIKACIA_VERSII_PZ == 34))
    if ((_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD26_DD29) >> 8) != 0x13)
      _SET_BIT(set_diagnostyka, ERROR_BDSH_CTLR);
    else
      _SET_BIT(clear_diagnostyka, ERROR_BDSH_CTLR);
#else
#error "UDEFINED MODIFIKACIA"
#endif

    _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD39_DD40_DD47) = 0x0;
  }
#endif
}
/*****************************************************/

/*****************************************************/
//Модуль обробки дискретних входів
/*****************************************************/
inline void input_scan(void)
{
  static unsigned int state_inputs; //"є сигнал " - відповідає встановленому біту (1); "немає сигналу" - відповідає скинутому біту (0)

  /***************************/
  //У цій частині знімаємо стани входів процесора (пінів), які відповідають за дискретні входи
  /*
  -----------------------------
  значення поміщається у відпорвідні біти змінної state_inputs_into_pin
    "є     сигнал " - відповідає встановленому біту (1)
    "немає сигналу" - відповідає скинутому     біту (0)
  -----------------------------
  */

#if (                              \
  (MODYFIKACIA_VERSII_PZ != 18) && \
  (MODYFIKACIA_VERSII_PZ != 8))
#pragma diag_suppress = Pa082
  unsigned int const state_inputs_into_pin = (_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD33_DD36) & 0xffff)

#if (                             \
  (MODYFIKACIA_VERSII_PZ == 0) || \
  (MODYFIKACIA_VERSII_PZ == 10))
                                             | (((_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD26_DD29) >> 8) & 0xf) << 16)
#elif (                           \
  (MODYFIKACIA_VERSII_PZ == 5) || \
  (MODYFIKACIA_VERSII_PZ == 15))
                                             | (((_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD26_DD29) >> 12) & 0xf) << 16)
#endif
    ;

#endif

#if (                             \
  (MODYFIKACIA_VERSII_PZ == 8) || \
  (MODYFIKACIA_VERSII_PZ == 18))
  uint32_t register_in = 0, pin_slot_d = 0;
  extern volatile unsigned char chGbl__CDHO1_CDHO7__W_VAL;
  extern volatile unsigned char chGbl__REL1_REL8__W_VAL;
  //Set CHDO
  //?( ~( ( 1 << CHDO2)|( 1 << CHDO3) ) )
#define COMENT
  //~#ifdef COMENT
  unsigned char chLocal__CDHO1_CDHO7__W_VAL = (~((1 << 1) | (1 << 2))) & chGbl__CDHO1_CDHO7__W_VAL;
  //1#ifndef COMENT
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD39_DD40_DD47) = chLocal__CDHO1_CDHO7__W_VAL; //0-now
                                                                                              //1  #endif
                                                                                              //..=:>>_Установити REL3, REL4, REL5, REL6 в «0»;<<-..

  //3#ifndef COMENT
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37) = (chGbl__REL1_REL8__W_VAL & 3) //CLEAR REL_3_4_5_6_SLOT_E_OUT ESTABLISH DATA
                                                                      | 0x8000;                     //8000 == (1<<REL16)

  //3  #endif
  _DELAY_ABOUT_40NS()
  //..^
  //..=:>>_Установити, RELX в «1»;//Select Slot D |REL_3_4_5_6_SLOT_D_IN__RD_VAL ==         0x8000 == (1<<REL16)
  //??#ifndef COMENT
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37) = (chGbl__REL1_REL8__W_VAL & 3) | 0x8004;
  //??  #endif
  //..^
  _DELAY_ABOUT_40NS()

  //Select slot D
  //..chGbl__REL1_REL8__W_VAL &= ~( 0x14);//Clean REL3 REL5
  //..unsigned char  chLocal__CDHO1_CDHO7__W_VAL = chGbl__CDHO1_CDHO7__W_VAL;
  //..chLocal__CDHO1_CDHO7__W_VAL&= ~( (1<<1)|(1<<2) );//Clean CHDO2 CHDO3
  //..chGbl__REL1_REL8__W_VAL |= 4;//Select Slot D

  //Read slot D
  //?#ifndef COMENT
  register_in = _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD33_DD36);
  //?  #endif
  pin_slot_d |= ((register_in & (1 << (1 - 1))) >> (1 - 1)) << (1 - 1);  //.D.1
  pin_slot_d |= ((register_in & (1 << (2 - 1))) >> (2 - 1)) << (3 - 1);  //D.3
  pin_slot_d |= ((register_in & (1 << (3 - 1))) >> (3 - 1)) << (5 - 1);  //D.5
  pin_slot_d |= ((register_in & (1 << (4 - 1))) >> (4 - 1)) << (7 - 1);  //D.7
  pin_slot_d |= ((register_in & (1 << (5 - 1))) >> (5 - 1)) << (9 - 1);  //D.9
  pin_slot_d |= ((register_in & (1 << (6 - 1))) >> (6 - 1)) << (11 - 1); //D.11
  pin_slot_d |= ((register_in & (1 << (7 - 1))) >> (7 - 1)) << (13 - 1); //D.13

  pin_slot_d |= ((register_in & (1 << (9 - 1))) >> (9 - 1)) << (15 - 1);   //D.15
  pin_slot_d |= ((register_in & (1 << (10 - 1))) >> (10 - 1)) << (17 - 1); //D.17
  pin_slot_d |= ((register_in & (1 << (11 - 1))) >> (11 - 1)) << (19 - 1); //D.19
  pin_slot_d |= ((register_in & (1 << (12 - 1))) >> (12 - 1)) << (21 - 1); //D.21
  pin_slot_d |= ((register_in & (1 << (13 - 1))) >> (13 - 1)) << (23 - 1); //D.23
  pin_slot_d |= ((register_in & (1 << (14 - 1))) >> (14 - 1)) << (25 - 1); //D.25
  pin_slot_d |= ((register_in & (1 << (15 - 1))) >> (15 - 1)) << (27 - 1); //D.27
  //}
  //while( lock != 0 );
  //?#ifndef COMENT
  register_in = _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37);
  //?  #endif

  pin_slot_d |= ((register_in & (1 << (3 - 1))) >> (3 - 1)) << (2 - 1);  //2
  pin_slot_d |= ((register_in & (1 << (4 - 1))) >> (4 - 1)) << (4 - 1);  //4
  pin_slot_d |= ((register_in & (1 << (5 - 1))) >> (5 - 1)) << (6 - 1);  //6
  pin_slot_d |= ((register_in & (1 << (6 - 1))) >> (6 - 1)) << (8 - 1);  //8
  pin_slot_d |= ((register_in & (1 << (7 - 1))) >> (7 - 1)) << (10 - 1); //10
  pin_slot_d |= ((register_in & (1 << (8 - 1))) >> (8 - 1)) << (12 - 1); //12

  pin_slot_d |= ((register_in & (1 << (9 - 1))) >> (9 - 1)) << (14 - 1);   //14
  pin_slot_d |= ((register_in & (1 << (10 - 1))) >> (10 - 1)) << (16 - 1); //16
  pin_slot_d |= ((register_in & (1 << (11 - 1))) >> (11 - 1)) << (18 - 1); //18
  pin_slot_d |= ((register_in & (1 << (12 - 1))) >> (12 - 1)) << (20 - 1); //20
  pin_slot_d |= ((register_in & (1 << (13 - 1))) >> (13 - 1)) << (22 - 1); //22
  pin_slot_d |= ((register_in & (1 << (14 - 1))) >> (14 - 1)) << (24 - 1); //24
  pin_slot_d |= ((register_in & (1 << (15 - 1))) >> (15 - 1)) << (26 - 1); //26
  pin_slot_d |= ((register_in & (1 << (16 - 1))) >> (16 - 1)) << (28 - 1); //28

  //..=:>>_Установити REL3, REL4, REL5, REL6 в «0»;<<-..
  //?#ifndef COMENT
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37) = (chGbl__REL1_REL8__W_VAL & 3) //CLEAR REL_3_4_5_6_SLOT_E_OUT ESTABLISH DATA
                                                                      | 0x8000;                     //8000 == (1<<REL16)
                                                                                                    //?  #endif
  _DELAY_ABOUT_40NS()
  //..^
  //2#ifndef COMENT
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37) = chGbl__REL1_REL8__W_VAL | 0x8000; //8000 == (1<<REL16);//Restore REL1_REL8?
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD39_DD40_DD47) = chGbl__CDHO1_CDHO7__W_VAL;             //Restore CDHO

  //2  #endif

  unsigned int const state_inputs_into_pin = pin_slot_d
                                             //?#ifndef COMENT
                                             | (((_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD26_DD29) >> 12) & 0xf) << 28); //bdvv6 4 bits
  //?  #else
  //?    ;
  //?  #endif
  //~ #endif

#endif

  /***************************/
  static unsigned int state_inputs_into_pin_prev;
  unsigned int const state_inputs_into_pin_changed = state_inputs_into_pin_prev ^ state_inputs_into_pin;

  static unsigned int state_inputs_into_pin_trigger; //У цій змінній зберігається попередній стан піна, у випадку коли ми перевіряємо .чи утримається цей стан до кінця тактування таймера допуску
  static unsigned int AC_signal[NUMBER_INPUTS][2];

  /***************************/
  //Обробка таймерів допуску дискретних входів
  /***************************/
#define DELTA_TIME_FOR_TIMERS_MEAS 625
#define PORIG_AC_SIGNAL 5000
  for (size_t i = 0; i < NUMBER_INPUTS; ++i)
  {
    unsigned int const maska = 1u << i;

    unsigned int(*const p_AC_signal)[2] = &AC_signal[i];
    if ((type_of_input_signal_prt & maska) != 0)
    {
      //Випадок змінного сигналу

      if ((state_inputs_into_pin & maska) == 0)
      {
        //Фіксується стан "0"
        if ((state_inputs_into_pin_changed & maska) != 0)
        {
          //Відбувся перехід "1"->"0"
          (*p_AC_signal)[0] = (*p_AC_signal)[1];
          (*p_AC_signal)[1] = 0;
        }
      }
      else
      {
        //Фіксується стан "1"
        if ((state_inputs_into_pin_changed & maska) == 0)
        {
          //Не відбувався перехід "0"->"1"
          if ((*p_AC_signal)[1] < (0xffffffff - DELTA_TIME_FOR_TIMERS_MEAS))
          {
            (*p_AC_signal)[1] += DELTA_TIME_FOR_TIMERS_MEAS;
          }
        }
      }
    }
    else
    {
      //Випадок постійного сигналу

      (*p_AC_signal)[0] = 0;
      (*p_AC_signal)[1] = 0;
    }

    int const max_value_timer = dopusk_dv_prt[i] * 1000; /*у мікросекундах*/

    if (global_timers[INDEX_TIMER_INPUT_START + i] < 0)
    {
      //Провірка на зміну стану дискретного входу здійснюється тільки тоді, коли величина таймере є від'ємною
      //Що означає що таймер не запущений у попередній момент

      //Перевіряємо чи таеперішній стан входу відповідає попердньомук стану
      if ((state_inputs_into_pin & maska) != (state_inputs & maska))
      {
        //Якщо стан входу змінився, то виконуємо дії по зміні стану

        //1-дія: Запям'ятовуємо текучий стан входу
        state_inputs_into_pin_trigger &= ~maska;
        state_inputs_into_pin_trigger |= (state_inputs_into_pin & maska);

        //2-дія: Запускаємо таймер допуску дискретного входу
        global_timers[INDEX_TIMER_INPUT_START + i] = 0;
      }
    }
    else
    {
      //Якщо таймер запущений виконуємо дії з таймером
      if (global_timers[INDEX_TIMER_INPUT_START + i] < max_value_timer)
      {
        //Якщо таймер ще не дійшов до свого макисмуму, то просто збільшуємо його величину
        global_timers[INDEX_TIMER_INPUT_START + i] += DELTA_TIME_FOR_TIMERS_MEAS;

        //У випадку, якщо тип сигналу на вхід подається змінний
        //і ми перевіряємо чи не відбувся перехід "є сигнал"->"немає сигналу"
        //то поява сигналу під час тактування таймера допуску означає, що сигнал на вході є -
        //а це означає, що треба зупинити nаймер, бо переходу "є сигнал"->"немає сигналу" на протязі тактування таймеру не зафіксовано
        if ((type_of_input_signal_prt & maska) != 0)
        {
          if ((state_inputs_into_pin_trigger & maska) == 0)
          {
            if (
              ((state_inputs_into_pin & maska) != 0) &&
              ((*p_AC_signal)[1] >= (PORIG_AC_SIGNAL - DELTA_TIME_FOR_TIMERS_MEAS)))
            {
              global_timers[INDEX_TIMER_INPUT_START + i] = -1;
            }
          }
        }
      }
    }

    //Якщо величина таймера допуска знаходиться у свому максимальному значенні, то перевіряємо, чи фактично змінився стано входу
    if (global_timers[INDEX_TIMER_INPUT_START + i] >= max_value_timer)
    {
      //Перевіряємо, чи стан піна змінився у порівнянні із станом, який був на момент запуску таймера допуску дискретного входу
      unsigned int const state_1 = state_inputs_into_pin_trigger & maska;
      unsigned int const state_2 = state_inputs_into_pin & maska;

      if (
        (state_1 == state_2) &&
        ((state_2 == 0) ||
         ((type_of_input_signal_prt & maska) == 0) ||
         ((*p_AC_signal)[0] >= PORIG_AC_SIGNAL)))
      {
        //Якщо два стани співпадають, то ми вважаємо, що відбулася зіна стану дискретного входу і формуємо новий стан входу
        //При цьому враховуємо що для прявого    входу 1 - це активний вхід, а 0 - це пасивний вхід
        //                        для інверсного входу 0 - це активний вхід, а 1 - це пасивний вхід
        state_inputs &= ~maska;
        state_inputs |= state_2;
      }

      //У будь-якому випадк, чи змінився стан входу, чи ні, а оскілька таймер допуску дотактував до кінця, то скидаємотаймер у висхідне від'ємне занчення
      global_timers[INDEX_TIMER_INPUT_START + i] = -1;
    }
  }
#undef PORIG_AC_SIGNAL
#undef DELTA_TIME_FOR_TIMERS_MEAS
  /***************************/

  state_inputs_into_pin_prev = state_inputs_into_pin;

  /***************************/
  //Визначаємо, який сигнал є зараз активним
  /***************************/
  unsigned int const active_inputs_meas = (state_inputs ^ type_of_input_prt);
  if (active_inputs_lock_guard_prt == 0)
  {
    active_inputs_prt = active_inputs_meas;
  }
  if (active_inputs_lock_guard == 0)
  {
    active_inputs_low = active_inputs_meas;
  }
  /***************************/
}
/*****************************************************/

/*****************************************************/
//Переривання від таймеру TIM5, який обслуговує вимірювальну систему
/*****************************************************/
void TIM5_IRQHandler(void)
{
#ifdef SYSTEM_VIEWER_ENABLE
  SEGGER_SYSVIEW_RecordEnterISR();
#endif

  if (TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET)
  {
    /***********************************************************************************************/
    //Переривання відбулося вік каналу 1, який генерує переривання для початку зчитування даних з АЦП групи 1
    /***********************************************************************************************/
    TIM5->SR = (uint16_t)((~(uint32_t) TIM_IT_CC1) & 0xffff); //TIM5->SR скидується для виходу з переривання
    penultimate_tick_DATA_VAL = previous_tick_DATA_VAL;
    uint32_t current_tick = previous_tick_DATA_VAL = TIM5->CCR1;

#ifdef _TEST_DURATION
    uint32_t const start_tick = TIM2->CNT;
#endif

    /*
    Виставляємо повідослення пор необхідність оцифрувати канали групи 1 
    для розрахунку ортогональних складових
    */
    adc_DATA_VAL_read = true;

    /***********************************************************************************************/
    //Для випадку, коли немає оцифровки "контрольних точок", то цей код буде запускатися з цього місця
    /***********************************************************************************************/
    //Подальші дії виконуємо тільки тоді, коли зараз не іде обмін
    if (
      ((GPIO_SPI_ADC->ODR & GPIO_NSSPin_ADC) != 0) &&
      (semaphore_adc_irq == false))
    {
      /*
      Це є умовою на цьому місці (переривання від таймеру має вищий пріоритет
      ніж перериванні від SPI) не ведеться зчитування значень з АЦП або їх обробка
      */
      control_reading_ADCs();
    }
    /***********************************************************************************************/

    /***********************************************************/
    //Встановлюємо "значення лічильника для наступного переривання"
    /***********************************************************/
    uint32_t capture_new;
    unsigned int delta, step_timer_adc_tmp;
    TIM5->CCR1 = (capture_new = (current_tick + (delta = step_timer_adc_tmp = step_timer_adc)));

    unsigned int repeat;
    unsigned int previous_tick = current_tick;
    do
    {
      repeat = 0;
      current_tick = TIM5->CNT;

      uint32_t delta_time = 0;
      if (capture_new < current_tick)
      {
        uint64_t delta_time_64 = (uint64_t) capture_new + 0x100000000ull - (uint64_t) current_tick;
        delta_time = delta_time_64;
      }
      else
        delta_time = capture_new - current_tick;

      if ((delta_time > delta) || (delta_time == 0))
      {
        if (TIM_GetITStatus(TIM5, TIM_IT_CC1) == RESET)
        {
          if (delta < step_timer_adc_tmp)
          {
            uint32_t delta_tick;
            if (current_tick < previous_tick)
            {
              uint64_t delta_tick_64 = (uint64_t) current_tick + 0x100000000ull - (uint64_t) previous_tick;
              delta_tick = delta_tick_64;
            }
            else
              delta_tick = current_tick - previous_tick;

            delta = delta_tick + 1;
          }
          else if (delta == step_timer_adc_tmp)
            delta = 1; /*Намагаємося, щоб нове переивання запустилося як омога скоріше*/
          else
          {
            //Теоретично цього ніколи не мало б бути
            total_error_sw_fixed();
          }
          TIM5->CCR1 = (capture_new = (TIM5->CNT + delta));
          previous_tick = current_tick;
          repeat = 0xff;
        }
      }
    } while (repeat != 0);
    /***********************************************************/

    /***********************************************************/
    //Виставляємо повідомлення про те, що почато оцифровку групи віимірювань 1 для перетворення Фур'є
    /***********************************************************/
    control_word_of_watchdog |= WATCHDOG_MEASURE_START_DATA_VAL;
    /***********************************************************/

#ifdef _TEST_DURATION
    uint32_t const stop_tick = TIM2->CNT;
    uint64_t const delta_tick = (stop_tick >= start_tick) ? (stop_tick - start_tick) : (0x100000000ull + stop_tick - start_tick);

    static uint64_t durTIM5_C1Max = 0;
    static uint64_t durTIM5_C1Min = 0x100000000ull;
    static uint32_t durTIM5_C1Reset;

    if (durTIM5_C1Reset != 0)
    {
      durTIM5_C1Max = 0;
      durTIM5_C1Min = 0x100000000ull;
      durTIM5_C1Reset = 0;
    }
    durTIM5_C1Cur = delta_tick;
    if (durTIM5_C1Max < delta_tick)
      durTIM5_C1Max = delta_tick;
    if (durTIM5_C1Min > delta_tick)
      durTIM5_C1Min = delta_tick;
#endif
    /***********************************************************************************************/
  }
  if (TIM_GetITStatus(TIM5, TIM_IT_CC2) != RESET)
  {
    /***********************************************************************************************/
    //Переривання відбулося вік каналу 2, який генерує переривання для початку зчитування даних з АЦП груп 1 і 2 (осцилограф і тестові значення контрольних точок)
    /***********************************************************************************************/
    TIM5->SR = (uint16_t)((~(uint32_t) TIM_IT_CC2) & 0xffff); //TIM5->SR скидується для виходу з переривання
    uint32_t current_tick = TIM5->CCR2;

#ifdef _TEST_DURATION
    uint32_t const start_tick = TIM2->CNT;
#endif
    /*
    Виконуємо дії по формування даних для аналогового реєстратора
    */
    if (data_for_oscylograph[head_data_for_oscylograph].DATA_fix == 0)
    {
      //Переповнення не зафіксоване
      _SET_BIT(clear_diagnostyka, ERROR_OSCYLOJRAPH_OVERFLOW);

      //Перевірка на те, чи не потрібно запустити роботу аналогового реєстртора на рівні "Вимірювання"
      if (
        (state_ar_record_fatfs == STATE_AR_WAIT_TO_STOP_WRITE_FATFS) &&
        (state_ar_record_m != STATE_AR_BLOCK_M) &&
        (index_array_ar_tail != index_array_ar_heat))
      {
        state_ar_record_fatfs = STATE_AR_WRITE_FATFS;
      }

      if (
        (
          (state_ar_record_m == STATE_AR_NONE_M) ||
          (state_ar_record_m == STATE_AR_WORK_STOP_M)) &&
        (_CHECK_SET_BIT(active_functions, RANG_WORK_A_REJESTRATOR) != 0))
      {
        state_ar_record_m = STATE_AR_WORK_M;

        if (state_ar_record_fatfs == STATE_AR_NONE_FATFS)
          state_ar_record_fatfs = STATE_AR_WAIT_TO_WRITE_FATFS;
      }
      else if (
        (state_ar_record_m == STATE_AR_WORK_M) &&
        (_CHECK_SET_BIT(active_functions, RANG_WORK_A_REJESTRATOR) == 0))
      {
        state_ar_record_m = STATE_AR_WORK_STOP_M;
      }
      else if (
        (state_ar_record_m == STATE_AR_WORK_STOP_M) &&
        ((state_ar_record_fatfs == STATE_AR_WAIT_TO_STOP_WRITE_FATFS) ||
         (state_ar_record_fatfs == STATE_AR_MEMORY_FULL_FATFS) ||
         (state_ar_record_fatfs == STATE_AR_BLOCK_FATFS)))
      {
        state_ar_record_m = STATE_AR_NONE_M;
        if (state_ar_record_fatfs == STATE_AR_WAIT_TO_STOP_WRITE_FATFS)
          state_ar_record_fatfs = STATE_AR_STOP_WRITE_FATFS;
        else if (state_ar_record_fatfs == STATE_AR_MEMORY_FULL_FATFS)
          state_ar_record_fatfs = STATE_AR_NONE_FATFS;
      }
      else if (
        (state_ar_record_m == STATE_AR_BLOCK_M) &&
        ((state_ar_record_fatfs == STATE_AR_NONE_FATFS) ||
         (state_ar_record_fatfs == STATE_AR_WAIT_TO_WRITE_FATFS) ||
         (state_ar_record_fatfs == STATE_AR_WAIT_TO_STOP_WRITE_FATFS) ||
         (state_ar_record_fatfs == STATE_AR_MEMORY_FULL_FATFS) ||
         (state_ar_record_fatfs == STATE_AR_BLOCK_FATFS)))
      {
        state_ar_record_m = STATE_AR_NONE_M;
        if (state_ar_record_fatfs == STATE_AR_WAIT_TO_STOP_WRITE_FATFS)
          state_ar_record_fatfs = STATE_AR_STOP_WRITE_FATFS;
        else if (
          (state_ar_record_fatfs == STATE_AR_WAIT_TO_WRITE_FATFS) ||
          (state_ar_record_fatfs == STATE_AR_MEMORY_FULL_FATFS))
        {
          state_ar_record_fatfs = STATE_AR_NONE_FATFS;
        }

        _SET_BIT(clear_diagnostyka, ERROR_AR_OVERLOAD_BUFFER_BIT);
      }

      //Мітка часу
      data_for_oscylograph[head_data_for_oscylograph].time_stemp = current_tick;
      data_for_oscylograph[head_data_for_oscylograph].state_ar_record = state_ar_record_m;

      //Активні дискретні сигнали
      unsigned int *label_to_active_functions_target = data_for_oscylograph[head_data_for_oscylograph].active_functions;
      unsigned int *label_to_active_functions;
      if (copying_active_functions == 0)
        label_to_active_functions = (unsigned int *) active_functions;
      else
        label_to_active_functions = (unsigned int *) active_functions_copy;
      for (unsigned int i = 0; i < N_BIG; i++)
        *(label_to_active_functions_target + i) = *(label_to_active_functions + i);

      if (++head_data_for_oscylograph >= MAX_INDEX_DATA_FOR_OSCYLOGRAPH)
        head_data_for_oscylograph = 0;
    }
    else
    {
      //Переповнення зафіксоване
      _SET_BIT(set_diagnostyka, ERROR_OSCYLOJRAPH_OVERFLOW);
    }
    /***/

    /*
       Виставляємо повідослення пор необхідність оцифрувати каналів тестових значень
       */
    adc_TEST_VAL_read = true;

    //Опрацьовуємо дискретні входи
    input_scan();

    //Самодіагностика приєднаних плат
    diagnostyka_boards();

    /***********************************************************/
    //Встановлюємо "значення лічильника для наступного переривання"
    /***********************************************************/
    uint32_t capture_new;
    unsigned int delta;
    TIM5->CCR2 = (capture_new = (current_tick + (delta = TIM5_CCR1_2_VAL)));

    unsigned int repeat;
    unsigned int previous_tick = current_tick;
    do
    {
      repeat = 0;
      current_tick = TIM5->CNT;

      uint32_t delta_time = 0;
      if (capture_new < current_tick)
      {
        uint64_t delta_time_64 = capture_new + 0x100000000 - current_tick;
        delta_time = delta_time_64;
      }
      else
        delta_time = capture_new - current_tick;

      if ((delta_time > delta) || (delta_time == 0))
      {
        if (TIM_GetITStatus(TIM5, TIM_IT_CC2) == RESET)
        {
          if (delta < TIM5_CCR1_2_VAL)
          {
            uint32_t delta_tick;
            if (current_tick < previous_tick)
            {
              uint64_t delta_tick_64 = current_tick + 0x100000000 - previous_tick;
              delta_tick = delta_tick_64;
            }
            else
              delta_tick = current_tick - previous_tick;

            delta = delta_tick + 1;
          }
          else if (delta == TIM5_CCR1_2_VAL)
            delta = 1; /*Намагаємося, щоб нове переивання запустилося як омога скоріше*/
          else
          {
            //Теоретично цього ніколи не мало б бути
            total_error_sw_fixed();
          }
          TIM5->CCR2 = (capture_new = (TIM5->CNT + delta));
          previous_tick = current_tick;
          repeat = 0xff;
        }
      }
    } while (repeat != 0);
    /***********************************************************/

    /***********************************************************/
    //Виставляємо повідомлення про те, що почато оцифровку всіх груп
    /***********************************************************/
    control_word_of_watchdog |= WATCHDOG_MEASURE_START_TEST_VAL;
    /***********************************************************/

#ifdef _TEST_DURATION
    uint32_t const stop_tick = TIM2->CNT;
    uint64_t const delta_tick = (stop_tick >= start_tick) ? (stop_tick - start_tick) : (0x100000000ull + stop_tick - start_tick);

    static uint64_t durTIM5_C2Max = 0;
    static uint64_t durTIM5_C2Min = 0x100000000ull;
    static uint32_t durTIM5_C2Reset;

    if (durTIM5_C2Reset != 0)
    {
      durTIM5_C2Max = 0;
      durTIM5_C2Min = 0x100000000ull;
      durTIM5_C2Reset = 0;
    }
    durTIM5_C2Cur = delta_tick;
    if (durTIM5_C2Max < delta_tick)
      durTIM5_C2Max = delta_tick;
    if (durTIM5_C2Min > delta_tick)
      durTIM5_C2Min = delta_tick;
#endif
    /***********************************************************************************************/
  }

  //  //Подальші дії виконуємо тільки тоді, коли зараз не іде обмін
  //  if (
  //      ((GPIO_SPI_ADC->ODR & GPIO_NSSPin_ADC) != 0) &&
  //      (semaphore_adc_irq == false)
  //     )
  //  {
  //    /*
  //    Це є умовою на цьому місці (переривання від таймеру має вищий пріоритет
  //    ніж перериванні від SPI) не ведеться зчитування значень з АЦП або їх обробка
  //    */
  //    control_reading_ADCs();
  //  }

#ifdef SYSTEM_VIEWER_ENABLE
  SEGGER_SYSVIEW_RecordExitISR();
#endif
}
/*****************************************************/

/*****************************************************/
//Переривання від таймеру TIM4, який обслуговує інтерфейси, клавіатуру і DataFlash
/*****************************************************/
void TIM4_IRQHandler(void)
{
#ifdef SYSTEM_VIEWER_ENABLE
  SEGGER_SYSVIEW_RecordEnterISR();
#endif

  if (TIM_GetITStatus(TIM4, TIM_IT_CC3) != RESET)
  {
    /***********************************************************************************************/
    //Переривання відбулося вік каналу 3, який генерує переривання з періодом, який визначений настройками, для очікування наступного символа по RS-485
    /***********************************************************************************************/
    TIM4->SR = (uint16_t)((~(uint32_t) TIM_IT_CC3) & 0xffff);
    uint16_t current_tick = TIM4->CNT;

    /******/
    //Перевіряємо інтерфейс RS-485 тільки коли запущений DMA для прийому, бо інакше стоїть умова аналізу прийнятого пакету
    /******/
    if ((DMA_StreamRS485_Rx->CR & (uint32_t) DMA_SxCR_EN) != 0)
    {
      RxBuffer_RS485_count = BUFFER_RS485 - (uint16_t)(DMA_StreamRS485_Rx->NDTR);
      current_tick = TIM4->CNT; /*фіксуємо час, коли ми зафіксували кількість прийнятих байт відносно якого будемо визначати новий час генерації переривання*/

      if (RxBuffer_RS485_count != 0)
      {
        if (RxBuffer_RS485_count_previous == RxBuffer_RS485_count)
        {
          //Прийнятий цілий фрейм

          //Зупиняємо потік DMA
          if ((DMA_StreamRS485_Rx->CR & (uint32_t) DMA_SxCR_EN) != 0)
            DMA_StreamRS485_Rx->CR &= ~(uint32_t) DMA_SxCR_EN;

          //Зчитуємо можливі помилки і перевіряємо, чи вони присутні
          unsigned int error_status = USART_RS485->SR & (USART_FLAG_ORE | USART_FLAG_NE | USART_FLAG_FE | USART_FLAG_PE);
          if (error_status == 0)
          {
            //Помилок немає

            //Перевіряємо адресу
            unsigned int global_requect;
            if (
              ((global_requect = (RxBuffer_RS485[0] == BROADCAST_ADDRESS_MODBUS_RTU)) != 0) ||
              (RxBuffer_RS485[0] == current_settings.address))
            {
              //Інакше прийнятий пакет буде стояти в очікуванні на подальшу обробку - якщо не виконувати більше тут ніяких дій (не перезапускати моніторинг)

              if (global_requect == 0)
                time_last_receive_byte_RS485 = TIM2->CNT;

              //Встановлюємо мітку, що на останньому перериванні відбулася фіксація прийняття цілого фрейму
              mark_current_tick_RS_485 = 0xff;
            }
            else
            {
              //Адреса не співпадає - відновити моніторинг лінії
              restart_monitoring_RS485();
            }
          }
          else
          {
            //Зафіксовані помилки  - відновити моніторинг лінії
            restart_monitoring_RS485();
          }
        }
        else
        {
          //Фрейм на стадії прийому
          RxBuffer_RS485_count_previous = RxBuffer_RS485_count;
        }
      }
    }
    /******/

    /***********************************************************/
    //Встановлюємо "значення лічильника для наступного переривання"
    /***********************************************************/
    if (number_bits_rs_485_waiting < 10)
      calculate_namber_bit_waiting_for_rs_485();
    uint16_t /*current_tick = TIM4->CNT,*/ capture_new;
    unsigned int delta, step_tmp;
    TIM4->CCR3 = (capture_new = (current_tick + (delta = step_tmp = (TIM4_CCR3_VAL * number_bits_rs_485_waiting))));

    unsigned int repeat;
    unsigned int previous_tick = current_tick;
    do
    {
      repeat = 0;
      current_tick = TIM4->CNT;

      uint32_t delta_time = 0;
      if (capture_new < current_tick)
        delta_time = capture_new + 0x10000 - current_tick;
      else
        delta_time = capture_new - current_tick;

      if ((delta_time > delta) || (delta_time == 0))
      {
        if (TIM_GetITStatus(TIM4, TIM_IT_CC3) == RESET)
        {
          if (delta < step_tmp)
          {
            uint32_t delta_tick;
            if (current_tick < previous_tick)
              delta_tick = current_tick + 0x10000 - previous_tick;
            else
              delta_tick = current_tick - previous_tick;

            delta = delta_tick + 1;
          }
          else if (delta == step_tmp)
            delta = 1; /*Намагаємося, щоб нове переривання запустилося як омога скоріше*/
          else
          {
            //Теоретично цього ніколи не мало б бути
            total_error_sw_fixed();
          }
          TIM4->CCR3 = (capture_new = (TIM4->CNT + delta));
          previous_tick = current_tick;
          repeat = 0xff;
        }
      }
    } while (repeat != 0);
    /***********************************************************/

    /***********************************************************/
    //Виставляємо повідомлення про те, що канал 3 TIM4 працює, що відповідає за періодичні функції фнтерфейсу RS-485
    /***********************************************************/
    control_word_of_watchdog |= WATCHDOG_RS485;
    /***********************************************************/

    /***********************************************************************************************/
  }
  else if (TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET)
  {
    /***********************************************************************************************/
    //Переривання відбулося вік каналу 1, який генерує переривання кожні 10 мс, для опрацьовування клавіатури
    /***********************************************************************************************/
    TIM4->SR = (uint16_t)((~(uint32_t) TIM_IT_CC1) & 0xffff);
    uint16_t current_tick = TIM4->CCR1;

    /***********************************************************/
    //Опрцювання пінів клавіатури
    /***********************************************************/
    /***************************/
    //Перебираємо всі рядки
    /***************************/
    //Рядок - 1
    GPIO_ResetBits(KEYBOARD, KEYBOARD_SW_1_PIN);
    check_state_key(KEYBOARD_SW_A, KEYBOARD_SW_A_PIN, BIT_KEY_ENTER);
    check_state_key(KEYBOARD_SW_B, KEYBOARD_SW_B_PIN, BIT_KEY_DOWN);
    check_state_key(KEYBOARD_SW_C, KEYBOARD_SW_C_PIN, BIT_KEY_RIGHT);
    check_state_key(KEYBOARD_SW_D, KEYBOARD_SW_D_PIN, BIT_KEY_C);
    GPIO_SetBits(KEYBOARD, KEYBOARD_SW_1_PIN);

    //Робимо невелику затримку
    for (unsigned int delay_local = 0; delay_local < 100; delay_local++)
      __no_operation();

    //Рядок - 2
    GPIO_ResetBits(KEYBOARD, KEYBOARD_SW_2_PIN);
    check_state_key(KEYBOARD_SW_A, KEYBOARD_SW_A_PIN, BIT_KEY_ESC);
    check_state_key(KEYBOARD_SW_B, KEYBOARD_SW_B_PIN, BIT_KEY_LEFT);
    check_state_key(KEYBOARD_SW_C, KEYBOARD_SW_C_PIN, BIT_KEY_UP);
    check_state_key(KEYBOARD_SW_D, KEYBOARD_SW_D_PIN, BIT_KEY_I);
    GPIO_SetBits(KEYBOARD, KEYBOARD_SW_2_PIN);

    //Робимо невелику затримку
    for (unsigned int delay_local = 0; delay_local < 100; delay_local++)
      __no_operation();

    //Рядок - 3
    GPIO_ResetBits(KEYBOARD, KEYBOARD_SW_3_PIN);
    check_state_key(KEYBOARD_SW_A, KEYBOARD_SW_A_PIN, BIT_KEY_1);
    check_state_key(KEYBOARD_SW_B, KEYBOARD_SW_B_PIN, BIT_KEY_2);
    check_state_key(KEYBOARD_SW_C, KEYBOARD_SW_C_PIN, BIT_KEY_3);
    check_state_key(KEYBOARD_SW_D, KEYBOARD_SW_D_PIN, BIT_KEY_O);
    GPIO_SetBits(KEYBOARD, KEYBOARD_SW_3_PIN);

    //Робимо невелику затримку
    for (unsigned int delay_local = 0; delay_local < 100; delay_local++)
      __no_operation();

    //Рядок - 4
    GPIO_ResetBits(KEYBOARD, KEYBOARD_SW_4_PIN);
    check_state_key(KEYBOARD_SW_A, KEYBOARD_SW_A_PIN, BIT_KEY_4);
    check_state_key(KEYBOARD_SW_B, KEYBOARD_SW_B_PIN, BIT_KEY_5);
    check_state_key(KEYBOARD_SW_C, KEYBOARD_SW_C_PIN, BIT_KEY_6);
    GPIO_SetBits(KEYBOARD, KEYBOARD_SW_4_PIN);
    /***************************/

    uint32_t maska_all_keys = (uint32_t)(
      (1 << BIT_KEY_ENTER) |
      (1 << BIT_KEY_DOWN) |
      (1 << BIT_KEY_RIGHT) |
      (1 << BIT_KEY_ESC) |
      (1 << BIT_KEY_LEFT) |
      (1 << BIT_KEY_UP) |
      (1 << BIT_KEY_1) |
      (1 << BIT_KEY_2) |
      (1 << BIT_KEY_3) |
      (1 << BIT_KEY_4) |
      (1 << BIT_KEY_5) |
      (1 << BIT_KEY_6) |
      (1 << BIT_KEY_C) |
      (1 << BIT_KEY_I) |
      (1 << BIT_KEY_O));
    if ((LCD_BL->ODR & LCD_BL_PIN) != (uint32_t) Bit_RESET)
    //?if (1)
    {
      //Підсвітка увімкнута
      if ((new_state_keyboard & maska_all_keys) != 0)
        time_backlighting = BACKLIGHTING_OFF * 100;
      if (
        (time_backlighting > 0) && /*випадок старту з викнутою підсвіткою*/
        (--time_backlighting == 0) /*закінчився час роботи приладу без підсвітки після старту приладу*/
      )
      {
        //Умова вивмкнення підсвітки
        LCD_BL->BSRRH = LCD_BL_PIN;
      }
    }
    else
    {
      //Підсвітка вимкнута
      if (
        ((new_state_keyboard & maska_all_keys) != 0) /*умова увімкнення підсвітки по натискуванні кнопки*/
        ||
        ((time_backlighting > 0) &&                                   /*випадок старту з викнутою підсвіткою*/
         (--time_backlighting == 0) &&                                /*закінчився час роботи приладу без підсвітки після старту приладу*/
         ((POWER_CTRL->IDR & POWER_CTRL_PIN) != (uint32_t) Bit_RESET) /*є оперативне живлення*/
         ))
      {
        //Умова увімкнення підсвітки після старту приладу
        LCD_BL->BSRRL = LCD_BL_PIN;
        time_backlighting = BACKLIGHTING_OFF * 100;

        uint32_t maska_wake_up_keys = (uint32_t)(
          (1 << BIT_KEY_ENTER) |
          (1 << BIT_KEY_DOWN) |
          (1 << BIT_KEY_RIGHT) |
          (1 << BIT_KEY_ESC) |
          (1 << BIT_KEY_LEFT) |
          (1 << BIT_KEY_UP));
        new_state_keyboard &= (unsigned int) (~maska_wake_up_keys);
      }
    }

    /***************************/
    //Обробка алгоритму функціональних кнопок
    /***************************/
    new_state_keyboard_for_db |= (unsigned int) (new_state_keyboard & ((1 << BIT_KEY_1) |
                                                                       (1 << BIT_KEY_2) |
                                                                       (1 << BIT_KEY_3) |
                                                                       (1 << BIT_KEY_4) |
                                                                       (1 << BIT_KEY_5) |
                                                                       (1 << BIT_KEY_6) |
                                                                       (1 << BIT_KEY_C) |
                                                                       (1 << BIT_KEY_I) |
                                                                       (1 << BIT_KEY_O))); /*new_state_keyboard змінюється і потім аналізується у функції main_manu_function, тому для аналізу натискування функціональних кнопок натискування цих кнопок аналізуємо через іншу змінну*/
    new_state_keyboard &= (unsigned int) (~(
      (1 << BIT_KEY_1) |
      (1 << BIT_KEY_2) |
      (1 << BIT_KEY_3) |
      (1 << BIT_KEY_4) |
      (1 << BIT_KEY_5) |
      (1 << BIT_KEY_6) |
      (1 << BIT_KEY_C) |
      (1 << BIT_KEY_I) |
      (1 << BIT_KEY_O)));

    //Перевірка на те, чи вже кнопка відпущена чи ще натиснута
    if (time_set_keyboard[BIT_KEY_1] == 0)
      new_state_keyboard_for_db &= (unsigned int) (~(1 << BIT_KEY_1));
    if (time_set_keyboard[BIT_KEY_2] == 0)
      new_state_keyboard_for_db &= (unsigned int) (~(1 << BIT_KEY_2));
    if (time_set_keyboard[BIT_KEY_3] == 0)
      new_state_keyboard_for_db &= (unsigned int) (~(1 << BIT_KEY_3));
    if (time_set_keyboard[BIT_KEY_4] == 0)
      new_state_keyboard_for_db &= (unsigned int) (~(1 << BIT_KEY_4));
    if (time_set_keyboard[BIT_KEY_5] == 0)
      new_state_keyboard_for_db &= (unsigned int) (~(1 << BIT_KEY_5));
    if (time_set_keyboard[BIT_KEY_6] == 0)
      new_state_keyboard_for_db &= (unsigned int) (~(1 << BIT_KEY_6));
    if (time_set_keyboard[BIT_KEY_C] == 0)
      new_state_keyboard_for_db &= (unsigned int) (~(1 << BIT_KEY_C));
    if (time_set_keyboard[BIT_KEY_I] == 0)
      new_state_keyboard_for_db &= (unsigned int) (~(1 << BIT_KEY_I));
    if (time_set_keyboard[BIT_KEY_O] == 0)
      new_state_keyboard_for_db &= (unsigned int) (~(1 << BIT_KEY_O));

    if ((new_state_keyboard_for_db & ((1 << BIT_KEY_1) | (1 << BIT_KEY_2) | (1 << BIT_KEY_3) | (1 << BIT_KEY_4) | (1 << BIT_KEY_5) | (1 << BIT_KEY_6) | (1 << BIT_KEY_C) | (1 << BIT_KEY_I) | (1 << BIT_KEY_O))) != 0)
    {
      if (
        ((current_settings.control_extra_settings_1 & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_FB_ACTIVATION)) == 0) ||
        ((new_state_keyboard & (1 << BIT_KEY_ENTER)) != 0))
      {
        /**************************
        Натиснути функціональна кнопка підтверджена натискуванням кнопки ENTER  
        Натиснуті кнопки переносимо до спеціальної змінної, щоб їх обслуговувати  у системі захистів
        ***************************/
        mutex_buttons = true;
        pressed_buttons |= (new_state_keyboard_for_db & ((1 << BIT_KEY_1) | (1 << BIT_KEY_2) | (1 << BIT_KEY_3) | (1 << BIT_KEY_4) | (1 << BIT_KEY_5) | (1 << BIT_KEY_6) | (1 << BIT_KEY_C) | (1 << BIT_KEY_I) | (1 << BIT_KEY_O))) >> BIT_KEY_1;
        mutex_buttons = false;
        new_state_keyboard_for_db &= (unsigned int) (~((1 << BIT_KEY_1) | (1 << BIT_KEY_2) | (1 << BIT_KEY_3) | (1 << BIT_KEY_4) | (1 << BIT_KEY_5) | (1 << BIT_KEY_6) | (1 << BIT_KEY_C) | (1 << BIT_KEY_I) | (1 << BIT_KEY_O)));
        /***************************/

        if ((current_settings.control_extra_settings_1 & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_FB_ACTIVATION)) != 0)
        {
          /**************************
          Кнопка ENTER вже відпацювала свою функцію активації кнопки, тому 
          знімаємо повідомлення, що вона була натиснута, щоб не відпацювала ще
          функція відображення нового пункту меню по натискуванні кнопки ENTER
          ***************************/
          new_state_keyboard &= (unsigned int) (~(1 << BIT_KEY_ENTER));
          /***************************/
        }
      }
    }
    /***************************/
    /***********************************************************/

    /***********************************************************/
    //Перевіряємо чи треба обновляти з часом 1 с дані на екрані
    /***********************************************************/
    if (((current_ekran.current_level == EKRAN_TIME) && (current_ekran.edition == 0)) ||
        ((current_ekran.current_level == EKRAN_TITLES_DIGITAL_REGISTRATOR) && (rewrite_ekran_once_more > 0)) ||
        (current_ekran.current_level == EKRAN_LIST_ANALOG_REGISTRATOR_RECORDS) ||
        (current_ekran.current_level == EKRAN_LIST_DIGITAL_REGISTRATOR_RECORDS) ||
        (current_ekran.current_level == EKRAN_LIST_REGISTRATOR_PROGRAM_ERROR_RECORDS) ||
        (current_ekran.current_level == EKRAN_LIST_STATE_CMD_REGISTRATOR_RECORDS) ||
        (current_ekran.current_level == EKRAN_MEASURMENT_2) ||
        (current_ekran.current_level == EKRAN_VOLTAGE_SELSYN) ||
        (current_ekran.current_level == EKRAN_ANGLE_SELSYN) ||
        (current_ekran.current_level == EKRAN_MEASURMENT_FREQUENCY) ||
        (current_ekran.current_level == EKRAN_STATE_INPUTS) ||
        (current_ekran.current_level == EKRAN_STATE_OUTPUTS) ||
        (current_ekran.current_level == EKRAN_DIAGNOSTYKA))
    {
      if (++time_rewrite > MAX_TIME_REWRITE_EKRAN)
      {
        time_rewrite = 0;
        new_state_keyboard |= (1 << BIT_REWRITE);
      }
    }
    /***********************************************************/

    /***********************************************************/
    //Лічильник ресурсу + періодичні операції раз у секунду
    /***********************************************************/
    static unsigned int number_ticks_for_OF_bit_reset;

    if (etap_reset_of_bit == ETAP_CLEAR_OF_ST_START_WAITING_5S)
    {
      etap_reset_of_bit = ETAP_CLEAR_OF_ST_WAITING_5S;
      number_ticks_for_OF_bit_reset = 0;
    }
    else if (etap_reset_of_bit == ETAP_CLEAR_OF_ST_WAITING_5S)
    {
      number_ticks_for_OF_bit_reset++;
      if (number_ticks_for_OF_bit_reset > 500)
        etap_reset_of_bit = ETAP_CLEAR_OF_WRITE_OF_0;
    }

    if (++number_inputs_for_fix_one_second >= 100)
    {

      //Запуск читання часу з RTC тільки піся успішного зчитування настройок
      //При цьому виставляємо біт блокування негайного запуску операції, щоб засинхронізуватися з роботою вимірювальної системи
      if ((state_spi1_task & STATE_SETTINGS_EEPROM_GOOD) != 0)
      {
        _SET_BIT(control_i2c_taskes, TASK_START_READ_RTC_BIT);
        _SET_BIT(control_i2c_taskes, TASK_BLK_OPERATION_BIT);
      }

      //Запусаємо раз у секунду самоконтроль важливих змінних
      periodical_tasks_TEST_SETTINGS =
        periodical_tasks_TEST_USTUVANNJA =
          periodical_tasks_TEST_TRG_FUNC =
            periodical_tasks_TEST_INFO_REJESTRATOR_AR =
              periodical_tasks_TEST_INFO_REJESTRATOR_DR =
                periodical_tasks_TEST_INFO_REJESTRATOR_PR_ERR =
                  periodical_tasks_CALC_INTEGRAL_MEASUREMENTS =
                    periodical_tasks_TEST_ANGLE =
                      periodical_tasks_TEST_RESURS =
                        periodical_tasks_TEST_FLASH_MEMORY = true;

      number_inputs_for_fix_one_second = 0;

      if (++number_seconds >= 60)
      {
        number_seconds = 0;
        if ((POWER_CTRL->IDR & POWER_CTRL_PIN) != (uint32_t) Bit_RESET)
        {
          if (state_ar_record_fatfs != STATE_AR_WRITE_FATFS)
            reinit_LCD = true; /*реініціалізацію індикатора робимо, коли не іде запис Аналогового реєстратора*/
        }
      }

      //Робота з таймером очікування нових змін налаштувань
      if (
        (restart_timeout_idle_new_settings != 0) ||
        (_CHECK_SET_BIT(active_functions, RANG_SETTINGS_CHANGED) == 0))
      {
        timeout_idle_new_settings = 0;
        if (restart_timeout_idle_new_settings != 0)
          restart_timeout_idle_new_settings = false;
      }
      else
      {
        if (timeout_idle_new_settings < (current_settings.timeout_idle_new_settings))
          timeout_idle_new_settings++;
      }

      //Робота з таймерами простою USB
      if ((restart_timeout_interface & (1 << USB_RECUEST)) != 0)
      {
        timeout_idle_USB = 0;
        restart_timeout_interface &= (unsigned int) (~(1 << USB_RECUEST));
      }
      else
      {
        if (timeout_idle_USB < (current_settings.timeout_deactivation_password_interface_USB))
          timeout_idle_USB++;
      }

      //Робота з таймерами простою RS-485
      if ((restart_timeout_interface & (1 << RS485_RECUEST)) != 0)
      {
        timeout_idle_RS485 = 0;
        restart_timeout_interface &= (unsigned int) (~(1 << RS485_RECUEST));
      }
      else
      {
        if (timeout_idle_RS485 < (current_settings.timeout_deactivation_password_interface_RS485))
          timeout_idle_RS485++;
      }

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
      //Робота з таймерами простою LAN
      if ((restart_timeout_interface & (1 << LAN_RECUEST)) != 0)
      {
        timeout_idle_LAN = 0;
        restart_timeout_interface &= (unsigned int) (~(1 << LAN_RECUEST));
      }
      else
      {
        if (timeout_idle_LAN < (current_settings.timeout_deactivation_password_interface_LAN))
          timeout_idle_LAN++;
      }
#endif

      //Ресурс
      if (restart_resurs_count == 0)
      {
        resurs_global = resurs_temp;
        resurs_temp = 0;
        if (resurs_global < resurs_global_min)
          resurs_global_min = resurs_global;
        if (resurs_global > resurs_global_max)
          resurs_global_max = resurs_global;
      }
      else
      {
        restart_resurs_count = 0;

        resurs_global = 0;
        resurs_temp = 0;
        resurs_global_min = 0xffffffff;
        resurs_global_max = 0;
      }
    }
    /***********************************************************/

    /***********************************************************/
    //Виставляємо повідомлення про те, що канал 1 TIM4, що відповідає за періодичні функції клавіатури працює
    /***********************************************************/
    if (watchdog_l2)
    {
      time_2_watchdog_output = time_2_watchdog_input = TIM4->CNT;
      GPIO_WriteBit(
        GPIO_EXTERNAL_WATCHDOG,
        GPIO_PIN_EXTERNAL_WATCHDOG,
        (BitAction)(1 - GPIO_ReadOutputDataBit(GPIO_EXTERNAL_WATCHDOG, GPIO_PIN_EXTERNAL_WATCHDOG)));
    }
    else
      control_word_of_watchdog |= WATCHDOG_KYYBOARD;
    /***********************************************************/

    /***********************************************************/
    //Встановлюємо "значення лічильника для наступного переривання"
    /***********************************************************/
    uint16_t capture_new;
    unsigned int delta;
    TIM4->CCR1 = (capture_new = (current_tick + (delta = TIM4_CCR1_VAL)));

    unsigned int repeat;
    unsigned int previous_tick = current_tick;
    do
    {
      repeat = 0;
      current_tick = TIM4->CNT;

      uint32_t delta_time = 0;
      if (capture_new < current_tick)
        delta_time = capture_new + 0x10000 - current_tick;
      else
        delta_time = capture_new - current_tick;

      if ((delta_time > delta) || (delta_time == 0))
      {
        if (TIM_GetITStatus(TIM4, TIM_IT_CC1) == RESET)
        {
          if (delta < TIM4_CCR1_VAL)
          {
            uint32_t delta_tick;
            if (current_tick < previous_tick)
              delta_tick = current_tick + 0x10000 - previous_tick;
            else
              delta_tick = current_tick - previous_tick;

            delta = delta_tick + 1;
          }
          else if (delta == TIM4_CCR1_VAL)
            delta = 1; /*Намагаємося, щоб нове переивання запустилося як омога скоріше*/
          else
          {
            //Теоретично цього ніколи не мало б бути
            total_error_sw_fixed();
          }
          TIM4->CCR1 = (capture_new = (TIM4->CNT + delta));
          previous_tick = current_tick;
          repeat = 0xff;
        }
      }
    } while (repeat != 0);
    /***********************************************************/

    /***********************************************************************************************/
  }
  else if (TIM_GetITStatus(TIM4, TIM_IT_CC2) != RESET)
  {
    /***********************************************************************************************/
    //Переривання відбулося від каналу 2, який генерує переривання кожні 1 мс, для опраціьовування управління мікросхемами DataFlash і формування нових записів реєстратора програмних подій
    /***********************************************************************************************/
    TIM4->SR = (uint16_t)((~(uint32_t) TIM_IT_CC2) & 0xffff);
    uint16_t current_tick = TIM4->CCR2;

    /***********************************************************/
    //Робота з USB на стиороні переривання
    /***********************************************************/
    Usb_routines_irq();
    /***********************************************************/

    /***********************************************************/
    //Перевіряємо необхідність очистки реєстратора програмних подій
    /***********************************************************/
    if (
      ((clean_rejestrators & CLEAN_PR_ERR) != 0) &&
      ((control_tasks_dataflash & (TASK_WRITE_PR_ERR_RECORDS_INTO_DATAFLASH |
                                   TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_USB |
                                   TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_RS485 |
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                                   TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_LAN |
#endif
                                   TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_MENU |
                                   TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_MENU_SHORT)) == 0))
    {
      //Виставлено каманда очистити реєстратор програмних подій

      //Виставляємо команду запису цієї структури у EEPROM
      _SET_BIT(control_spi1_taskes, TASK_START_WRITE_INFO_REJESTRATOR_PR_ERR_EEPROM_BIT);

      //Очищаємо структуру інформації по реєстраторі програмних помилок
      info_rejestrator_pr_err.next_address = MIN_ADDRESS_PR_ERR_AREA;
      info_rejestrator_pr_err.saving_execution = 0;
      info_rejestrator_pr_err.number_records = 0;

      //Помічаємо, що номер запису не вибраний
      number_record_of_pr_err_into_menu = 0xffff;
      number_record_of_pr_err_into_USB = 0xffff;
      number_record_of_pr_err_into_RS485 = 0xffff;
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
      number_record_of_pr_err_into_LAN = 0xffff;
#endif

      //Знімаємо команду очистки реєстратора програмних подій
      clean_rejestrators &= (unsigned int) (~CLEAN_PR_ERR);
    }
    /***********************************************************/

    /***********************************************************/
    //Періодично запускаємо звертання то мікросхем DataFlash
    /***********************************************************/
    if (
      (mutex_spi1 == false) &&
      (state_execution_spi1 < 0) &&
      (((DMA_StreamSPI_EDF_Tx->CR & (uint32_t) DMA_SxCR_EN) == 0) && ((DMA_StreamSPI_EDF_Rx->CR & (uint32_t) DMA_SxCR_EN) == 0)))
    {
      if ((number_chip_dataflsh_exchange >= INDEX_DATAFLASH_1) && (number_chip_dataflsh_exchange <= INDEX_DATAFLASH_2))
      {
        if (error_into_spi_df == 0)
        {
          //Не було зафіксовано, що при поперпдній операції виникла помилка

          //Перевіряємо, чи є потреба і можливість записувати реєстратор програмних подій
          if (

            /***
              Перший раз вже зчитаний час з моменту перезапуску
              ***/
            (_CHECK_SET_BIT(diagnostyka, EVENT_START_SYSTEM_BIT) == 0) &&
            (_CHECK_SET_BIT(set_diagnostyka, EVENT_START_SYSTEM_BIT) == 0) &&
            (_CHECK_SET_BIT(diagnostyka, EVENT_RESTART_SYSTEM_BIT) == 0) &&
            (_CHECK_SET_BIT(set_diagnostyka, EVENT_RESTART_SYSTEM_BIT) == 0) &&
            (_CHECK_SET_BIT(diagnostyka, EVENT_SOFT_RESTART_SYSTEM_BIT) == 0) &&
            (_CHECK_SET_BIT(set_diagnostyka, EVENT_SOFT_RESTART_SYSTEM_BIT) == 0) &&
            /***/
            ((POWER_CTRL->IDR & POWER_CTRL_PIN) != (uint32_t) Bit_RESET) && /*На даний момент на вході блоку живлення подається живлення*/
            /***/
            (temporary_block_writing_records_pr_err_into_DataFlash == 0) && /*Блокування запису підготовлених записів реєстратора програмних подій у мікросхему DataFlash1*/
            ((control_tasks_dataflash & 0xffff) == 0)                       /*Починаємо запис у реєстратор програмних подій тільки тоді, коли інших задач у черзі немає для запису у DataFlash1*/
          )
          {
            unsigned int head = head_fifo_buffer_pr_err_records, tail = tail_fifo_buffer_pr_err_records;
            if (
              (head != tail) ||
              (_CHECK_SET_BIT(diagnostyka, ERROR_PR_ERR_OVERLOAD_BIT) != 0))
            {
              control_tasks_dataflash |= TASK_WRITE_PR_ERR_RECORDS_INTO_DATAFLASH; //Є нові записи у буфері подій
            }
          }

          //Аналізуємо прийняті дані, якщо такі є і чекають на аналіз
          main_function_for_dataflash_resp(number_chip_dataflsh_exchange);

          if (
            (control_spi1_taskes[0] == 0) &&
            (control_spi1_taskes[1] == 0))
          {
            //Змінюємо номер DataFlash з яким буде іти зараз робота
            if (driver_spi_df[number_chip_dataflsh_exchange].state_execution == TRANSACTION_EXECUTING_NONE)
            {
              //Змінюємо номер мікросхеми, до якої ми будемо звертатися при натупних трансакціях, якщо зараз не запущена ніяка трансакція
              number_chip_dataflsh_exchange = (number_chip_dataflsh_exchange + 1) & (NUMBER_DATAFLASH_CHIP - 1);
            }

            //Робимо запит на нову мікросхему DataFlash, якщо є такий
            main_function_for_dataflash_req(number_chip_dataflsh_exchange);
          }
        }
        else
        {
          //Було зафіксовано, що при поперпдній операції виникла помилка
          if (driver_spi_df[number_chip_dataflsh_exchange].state_execution == TRANSACTION_EXECUTED_WAIT_ANALIZE)
          {
            //Очікується аналіз прийнятих даних з мікросхеми DataFlash у режимі виходу з "збою" обміну
            if (driver_spi_df[number_chip_dataflsh_exchange].code_operation == CODE_OPERATION_STATUS_READ)
            {
              /*
              У режимі відновлення обміну з мікросхемою DataFlash після "збою" в обміні
              можлива ситуація, що ми аналізуємо відповідь на читання пегістру статусу - 
              у всіх інших випадках - це є недопустимий випадок, який свідчить, що порграма
              виконується незаплпнованим шляхом - тому перезапуск
              */

              if ((RxBuffer_SPI_EDF[1] & (1 << 7)) != 0)
                dataflash_not_busy |= (1 << number_chip_dataflsh_exchange);
              else
                dataflash_not_busy &= (unsigned int) (~(1 << number_chip_dataflsh_exchange));

              driver_spi_df[number_chip_dataflsh_exchange].state_execution = TRANSACTION_EXECUTING_NONE;
              driver_spi_df[number_chip_dataflsh_exchange].code_operation = CODE_OPERATION_NONE;
            }
            else
            {
              total_error_sw_fixed();
            }
          }

          /*
          Оскільки номер мікросхеми, з якою здійснюємо обмін, під час виходу з ситуації,
          коли виник збій, не змінюємо, то можна спробувати запустити зразу після
          аналізу принятого пакету новий запит
          */
          if (driver_spi_df[number_chip_dataflsh_exchange].state_execution == TRANSACTION_EXECUTING_NONE)
          {
            //Очікується початок нового обміну з мікросхемою DataFlash
            if ((dataflash_not_busy & (1 << number_chip_dataflsh_exchange)) == 0)
            {
              //Читаємо регітр статусу мікросхеми
              dataflash_status_read(number_chip_dataflsh_exchange);
            }
            else
            {
              //Мікросхема готова до повторного виконання операції, під час якої виникла помилкова ситуація на SPI_EDF

              //Відновлюємо буфер передавання, який був під час помилки
              if ((number_bytes_transfer_spi_df_copy != 0) && (number_bytes_transfer_spi_df_copy < sizeof(TxBuffer_SPI_EDF)))
              {
                number_bytes_transfer_spi_df = number_bytes_transfer_spi_df_copy;
                for (unsigned int i = 0; i < number_bytes_transfer_spi_df; i++)
                  TxBuffer_SPI_EDF[i] = TxBuffer_SPI_DF_copy[i];

                /*
                Знімаємо помітку, що зафіксовано помилку при обміні, бо ми будемо 
                виконувати ту операція, яка виконувалася до моменту фіксації помилки
                */
                error_into_spi_df = 0;

                //Відновлюємо операцію, яка виконувался
                driver_spi_df[number_chip_dataflsh_exchange].code_operation = code_operation_copy;

                //Запускаємо повторно дану операцію
                start_exchange_via_spi(number_chip_dataflsh_exchange, number_bytes_transfer_spi_df);
              }
              else
              {
                //Відбулася невизначена помилка, тому треба піти на перезавантаження
                total_error_sw_fixed();
              }
            }
          }
        }
      }
      else
      {
        //Відбулася невизначена помилка, тому треба піти на перезавантаження
        total_error_sw_fixed();
      }
    }
    /***********************************************************/

    /***********************************************************/
    //Підготовлюємо новий запис для реєстратора програмних подій
    /***********************************************************/
    changing_diagnostyka_state();
    /***********************************************************/

    /***********************************************************/
    //Перевірка на необхідність зроботи резервну копію даних для самоконтролю
    /***********************************************************/
    //Реєстратор програмних подій
    if (periodical_tasks_TEST_INFO_REJESTRATOR_PR_ERR != 0)
    {
      //Стоїть у черзі активна задача зроботи резервні копії даних
      if ((state_spi1_task & STATE_INFO_REJESTRATOR_PR_ERR_EEPROM_GOOD) != 0)
      {
        //Робимо копію тільки тоді, коли структура інформації реєстратора успішно зчитана і сформована контрольна сума
        if (
          (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_WRITE_INFO_REJESTRATOR_PR_ERR_EEPROM_BIT) == 0) &&
          (_CHECK_SET_BIT(control_spi1_taskes, TASK_WRITING_INFO_REJESTRATOR_PR_ERR_EEPROM_BIT) == 0) &&
          (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_READ_INFO_REJESTRATOR_PR_ERR_EEPROM_BIT) == 0) &&
          (_CHECK_SET_BIT(control_spi1_taskes, TASK_READING_INFO_REJESTRATOR_PR_ERR_EEPROM_BIT) == 0))
        {
          //На даний моммент не іде читання-запис структури інформації реєстратора, тому можна здійснити копіювання
          info_rejestrator_pr_err_ctrl = info_rejestrator_pr_err;
          crc_info_rejestrator_pr_err_ctrl = crc_info_rejestrator_pr_err;

          //Скидаємо активну задачу формування резервної копії
          periodical_tasks_TEST_INFO_REJESTRATOR_PR_ERR = false;
          //Виставляємо активну задачу контролю достовірності по резервній копії
          periodical_tasks_TEST_INFO_REJESTRATOR_PR_ERR_LOCK = true;
        }
      }
      else
      {
        //Скидаємо активну задачу формування резервної копії
        periodical_tasks_TEST_INFO_REJESTRATOR_PR_ERR = false;
      }
    }
    /***********************************************************/

    /***********************************************************/
    //Встановлюємо "значення лічильника для наступного переривання"
    /***********************************************************/
    uint16_t /*current_tick = TIM4->CNT,*/ capture_new;
    unsigned int delta;
    TIM4->CCR2 = (capture_new = (current_tick + (delta = TIM4_CCR2_VAL)));

    unsigned int repeat;
    unsigned int previous_tick = current_tick;
    do
    {
      repeat = 0;
      current_tick = TIM4->CNT;

      uint32_t delta_time = 0;
      if (capture_new < current_tick)
        delta_time = capture_new + 0x10000 - current_tick;
      else
        delta_time = capture_new - current_tick;

      if ((delta_time > delta) || (delta_time == 0))
      {
        if (TIM_GetITStatus(TIM4, TIM_IT_CC2) == RESET)
        {
          if (delta < TIM4_CCR2_VAL)
          {
            uint32_t delta_tick;
            if (current_tick < previous_tick)
              delta_tick = current_tick + 0x10000 - previous_tick;
            else
              delta_tick = current_tick - previous_tick;

            delta = delta_tick + 1;
          }
          else if (delta == TIM4_CCR2_VAL)
            delta = 1; /*Намагаємося, щоб нове переивання запустилося як омога скоріше*/
          else
          {
            //Теоретично цього ніколи не мало б бути
            total_error_sw_fixed();
          }
          TIM4->CCR2 = (capture_new = (TIM4->CNT + delta));
          previous_tick = current_tick;
          repeat = 0xff;
        }
      }
    } while (repeat != 0);
    /***/
    /***********************************************************/

    /***********************************************************/
    //Виставляємо повідомлення про те, що канал 2 TIM4, що відповідає за періодичні функції для DataFlash працює
    /***********************************************************/
    control_word_of_watchdog |= WATCHDOG_DATAFLASH;
    /***********************************************************/
    /***********************************************************************************************/
  }

#ifdef SYSTEM_VIEWER_ENABLE
  SEGGER_SYSVIEW_RecordExitISR();
#endif
}
/*****************************************************/

/*****************************************************/
//Переривання від генеррації події про помилку для SPI_EDF
/*****************************************************/
void SPI_EDF_IRQHandler(void)
{
#ifdef SYSTEM_VIEWER_ENABLE
  SEGGER_SYSVIEW_RecordEnterISR();
#endif

  /*
  Відбулася помилкова ситуація - треба повторно виконати попередньо виконувану трансакцію
  */

  /*
  Запам'ятовуємо регістр статусу, який зчитаний зразу після генерації переривання - 
  у ньому має бути зафіксована причина генерації переривання
  Цю змінну будемо використовувати при аналізі помилки
  */
  uint16_t spi_status = SPI_EDF->SR;

  /*Забороняємо генерацію переривань від потоку DMA_StreamSPI_EDF_Rx*/
  DMA_StreamSPI_EDF_Rx->CR &= ~DMA_IT_TC;

  /*
  Можливо зараз ще продовжується робота DMA контролера по прийом/передачі даних,
  але осекільки зафіксовано помилку у процесі обміну, то зупиняємо їх
  */
  DMA_StreamSPI_EDF_Tx->CR &= ~(uint32_t) DMA_SxCR_EN;
  DMA_StreamSPI_EDF_Tx->NDTR = 0;
  DMA_StreamSPI_EDF_Rx->CR &= ~(uint32_t) DMA_SxCR_EN;
  DMA_StreamSPI_EDF_Rx->NDTR = 0;

  /*Тимчасово забороняємо переривання від помилок на SPI_EDF*/
  SPI_I2S_ITConfig(SPI_EDF, SPI_I2S_IT_ERR, DISABLE);

  /*
  Очікуємо поки TXE=1 і BSY=0 - це ознака повногго завершення передачі даних,
  а ознакою завершення прийому дані - це є генерація цього переривання
  */
  while ((SPI_EDF->SR & SPI_I2S_FLAG_TXE) == 0)
    ;
  while ((SPI_EDF->SR & SPI_I2S_FLAG_BSY) != 0)
    ;

  /*Знімаємо Chip_select переводом NSS  у 1*/
  GPIO_SPI_EDF->BSRRL = GPIO_NSSPin_EDF;

  if (
    ((spi_status & SPI_I2S_FLAG_OVR) != 0) ||
    ((SPI_EDF->SR & SPI_I2S_FLAG_OVR) != 0))
  {
    //Подія про переповнення приймача

    do
    {
      //Очищаємо цю подією такою комбінацією читань:
      SPI_EDF->DR;
      SPI_EDF->SR;
    } while ((SPI_EDF->SR & SPI_I2S_FLAG_OVR) != 0);
  }

  if (
    ((spi_status & SPI_FLAG_MODF) != 0) ||
    ((SPI_EDF->SR & SPI_FLAG_MODF) != 0))
  {
    //Подія "Master mode fault" - теоретично нікли не малаб виникати

    do
    {
      //Очищаємо цю подією такою комбінацією:
      SPI_EDF->SR;

      //Забороняємо SPI_EDF
      SPI_Cmd(SPI_EDF, DISABLE);
      //Забороняємо SPI_EDF DMA Tx запити
      SPI_I2S_DMACmd(SPI_EDF, SPI_I2S_DMAReq_Tx, DISABLE);
      //Забороняємо SPI_EDF DMA Rx запити
      SPI_I2S_DMACmd(SPI_EDF, SPI_I2S_DMAReq_Rx, DISABLE);

      //Примусово переводимо SPI_EDF у Slave-режимі
      SPI_InitTypeDef SPI_InitStructure;
      SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
      SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
      SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
      SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
      SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
      SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
      SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
      SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
      SPI_InitStructure.SPI_CRCPolynomial = 7;
      SPI_Init(SPI_EDF, &SPI_InitStructure);
      //Дозволяємо SPI_EDF - у Slave-режимі
      SPI_Cmd(SPI_EDF, ENABLE);

      //Знову забороняємо SPI_EDF для перенастройки у Master-режим
      SPI_Cmd(SPI_EDF, DISABLE);

      //Повертаємо SPI_EDF у Master-режим
      SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
      SPI_Init(SPI_EDF, &SPI_InitStructure);

      //Забороняємо SPI_EDF DMA Tx запити
      SPI_I2S_DMACmd(SPI_EDF, SPI_I2S_DMAReq_Tx, DISABLE);
      //Забороняємо SPI_EDF DMA Rx запити
      SPI_I2S_DMACmd(SPI_EDF, SPI_I2S_DMAReq_Rx, DISABLE);

      //Очищаємо прапореці, що сигналізує про завершення прийом/передачі даних для DMA по потоку DMA_StreamSPI_EDF_Rx і DMA_StreamSPI_EDF_Tx
      DMA_ClearFlag(DMA_StreamSPI_EDF_Rx, DMA_FLAG_TCSPI_EDF_Rx | DMA_FLAG_HTSPI_EDF_Rx | DMA_FLAG_TEISPI_EDF_Rx | DMA_FLAG_DMEISPI_EDF_Rx | DMA_FLAG_FEISPI_EDF_Rx);
      DMA_ClearFlag(DMA_StreamSPI_EDF_Tx, DMA_FLAG_TCSPI_EDF_Tx | DMA_FLAG_HTSPI_EDF_Tx | DMA_FLAG_TEISPI_EDF_Tx | DMA_FLAG_DMEISPI_EDF_Tx | DMA_FLAG_FEISPI_EDF_Tx);

      //Дозволяємо SPI_EDF
      SPI_Cmd(SPI_EDF, ENABLE);
    } while ((SPI_EDF->SR & SPI_FLAG_MODF) != 0);
  }

  if (
    ((spi_status & SPI_FLAG_CRCERR) != 0) ||
    ((SPI_EDF->SR & SPI_FLAG_CRCERR) != 0))
  {
    //Подія "CRC error" - ніколи не мала б виникати у нашоій програмі

    //Відбулася невизначена помилка, тому треба піти на перезавантаження
    total_error_sw_fixed();
  }

  if (state_execution_spi1 >= 0)
  {
    //Відбувався обмін з EEPROM. Помічаємо, що відбулася помилка у процесі виконання попередньої операції прийом-передачі
    state_execution_spi1 = 2;
  }
  else if ((number_chip_dataflsh_exchange >= INDEX_DATAFLASH_1) && (number_chip_dataflsh_exchange <= INDEX_DATAFLASH_2))
  {
    if (driver_spi_df[number_chip_dataflsh_exchange].state_execution != TRANSACTION_EXECUTING_NONE)
    {
      //Помилка виникла в процесі прийом/передачі

      //Фіксуємо операцію, яка виконувалася для подальшої обробки
      unsigned int code_operation_copy_tmp = driver_spi_df[number_chip_dataflsh_exchange].code_operation;

      /*
      Оскільки помилка зафіксована при прийомі, то, впринципі, передавання даних могло закінчитися успішно,
      тому треба помітити, що треба зчитати статус готовності
      */
      dataflash_not_busy &= (unsigned int) (~(1 << number_chip_dataflsh_exchange));

      //Знімаємо сигналізацію, що виконується операція для даної мікросхеми
      driver_spi_df[number_chip_dataflsh_exchange].state_execution = TRANSACTION_EXECUTING_NONE;
      driver_spi_df[number_chip_dataflsh_exchange].code_operation = CODE_OPERATION_NONE;

      if (code_operation_copy_tmp != CODE_OPERATION_STATUS_READ)
      {
        /*
        Виключаємо подпальші дії, якщо попередня операція було читання регістру статусу,
        бо, оскільки ми зняли прапорець готовності даної мікросхеми, то ця операція всеодно
        викличеться першою, а далі підуть всі наступні операції
        */
        if (code_operation_copy_tmp == CODE_OPERATION_STATUS_READ)
        {
          /*Відбулася невизначена помилка (бо не може бути, щоб ішда якась операція, 
          але вона була поміена. що ніякої операції немає), тому треба піти на перезавантаження
          */
          total_error_sw_fixed();
        }

        //Робимо резервну копію кількості байт для передавання і буфер передавання
        if ((number_bytes_transfer_spi_df != 0) && (number_bytes_transfer_spi_df < sizeof(TxBuffer_SPI_EDF)))
        {
          number_bytes_transfer_spi_df_copy = number_bytes_transfer_spi_df;
          for (unsigned int i = 0; i < number_bytes_transfer_spi_df_copy; i++)
            TxBuffer_SPI_DF_copy[i] = TxBuffer_SPI_EDF[i];
        }
        else
        {
          //Відбулася невизначена помилка, тому треба піти на перезавантаження
          total_error_sw_fixed();
        }

        //Запам'ятовуємо, яка виконувалася операція
        code_operation_copy = code_operation_copy_tmp;

        //Помічаємо, що зафіксовано помилку при обміні
        error_into_spi_df = 0xff;
      }
    }
  }
  else
  {
    //Відбулася невизначена помилка, тому треба піти на перезавантаження
    total_error_sw_fixed();
  }

  /*Виставляємо повідомлення про помилку обміну через SPI_EDF*/
  _SET_BIT(set_diagnostyka, ERROR_SPI_EDF_BIT);

  //Дозволяємо переривання від помилок на SPI_EDF
  SPI_I2S_ITConfig(SPI_EDF, SPI_I2S_IT_ERR, ENABLE);

#ifdef SYSTEM_VIEWER_ENABLE
  SEGGER_SYSVIEW_RecordExitISR();
#endif
}
/*****************************************************/

/*****************************************************/
//Перереривання від завершення прийому DMA_StreamSPI_EDF_Rx
/*****************************************************/
void DMA_StreamSPI_EDF_Rx_IRQHandler(void)
{
#ifdef SYSTEM_VIEWER_ENABLE
  SEGGER_SYSVIEW_RecordEnterISR();
#endif

  /*
  Очікуємо поки TXE=1 і BSY=0 - це ознака повногго завершення передачі даних,
  а ознакою завершення прийому дані - це є генерація цього переривання
  */
  while ((SPI_EDF->SR & SPI_I2S_FLAG_TXE) == 0)
    ;
  while ((SPI_EDF->SR & SPI_I2S_FLAG_BSY) != 0)
    ;

  //Знімаємо Chip_select переводом NSS  у 1
  GPIO_SPI_EDF->BSRRL = GPIO_NSSPin_EDF;

  //Зупиняємо потоки DMA
  DMA_StreamSPI_EDF_Tx->CR &= ~(uint32_t) DMA_SxCR_EN;
  DMA_StreamSPI_EDF_Tx->NDTR = 0;
  DMA_StreamSPI_EDF_Rx->CR &= ~(uint32_t) DMA_SxCR_EN;
  DMA_StreamSPI_EDF_Rx->NDTR = 0;

  //Забороняємо генерацію переривань від потоку DMA_StreamSPI_EDF_Rx
  DMA_StreamSPI_EDF_Rx->CR &= ~DMA_IT_TC;

  //Очищаємо прапореці, що сигналізує про завершення передачі даних для DMA по потоку DMA_StreamSPI_EDF_Rx
  DMA_ClearFlag(DMA_StreamSPI_EDF_Rx, DMA_FLAG_TCSPI_EDF_Rx | DMA_FLAG_HTSPI_EDF_Rx | DMA_FLAG_TEISPI_EDF_Rx | DMA_FLAG_DMEISPI_EDF_Rx | DMA_FLAG_FEISPI_EDF_Rx);

  //Виставляємо повідомлення, що дані передані і готові до наступного аналізу
  if (state_execution_spi1 == 0)
  {
    //Відбувався обмін з EEPROM. Помічаємо, що він завершився
    state_execution_spi1 = 1;
  }
  else if ((number_chip_dataflsh_exchange == INDEX_DATAFLASH_1) || (number_chip_dataflsh_exchange == INDEX_DATAFLASH_2))
    driver_spi_df[number_chip_dataflsh_exchange].state_execution = TRANSACTION_EXECUTED_WAIT_ANALIZE;
  else
  {
    //Відбулася невизначена помилка, тому треба піти на перезавантаження
    total_error_sw_fixed();
  }

  //Обмін відбувся вдало - скидаємо повідомлення про попередньо можливу помилку обміну через SPI_EDF
  _SET_BIT(clear_diagnostyka, ERROR_SPI_EDF_BIT);

#ifdef SYSTEM_VIEWER_ENABLE
  SEGGER_SYSVIEW_RecordExitISR();
#endif
}
/*****************************************************/

/*****************************************************/
//Переривання від USART RS-485
/*****************************************************/
void USARTRS485_IRQHandler(void)
{
#ifdef SYSTEM_VIEWER_ENABLE
  SEGGER_SYSVIEW_RecordEnterISR();
#endif

  if (USART_GetITStatus(USART_RS485, USART_IT_TC) != RESET)
  {
    //Переводимо мікросхкму на прийом
    GPIO_485DE->BSRRH = GPIO_PIN_485DE;

    //Забороняємо генерацію переривань при завершенні передачі байту
    USART_ITConfig(USART_RS485, USART_IT_TC, DISABLE);

    //Скидаємо біт, що символізує, що опстанній байт переданий
    USART_ClearFlag(USART_RS485, USART_FLAG_TC);

    //Відновлюємо моніторинг каналу RS-485 , якщо не стоїть умова реконфігурувати інтерфейс
    if (make_reconfiguration_RS_485 == 0)
      restart_monitoring_RS485();
  }

  if (USART_GetITStatus(USART_RS485, USART_IT_IDLE) != RESET)
  {
    //Зафіксовано IDLE стан
    if ((DMA_StreamRS485_Rx->CR & (uint32_t) DMA_SxCR_EN) != 0)
      DMA_StreamRS485_Rx->CR &= ~(uint32_t) DMA_SxCR_EN;
    USART_RS485->SR;
    (uint16_t)(USART_RS485->DR & (uint16_t) 0x01FF);

    // Очищаємо прапореці, що сигналізує про завершення передачі даних для DMA1 по каналу RS-485_RX
    DMA_ClearFlag(DMA_StreamRS485_Rx, DMA_FLAG_TCRS485_Rx | DMA_FLAG_HTRS485_Rx | DMA_FLAG_TEIRS485_Rx | DMA_FLAG_DMEIRS485_Rx | DMA_FLAG_FEIRS485_Rx);
    uint32_t NDTR_tmp = DMA_StreamRS485_Rx->NDTR;
    DMA_StreamRS485_Rx->NDTR = NDTR_tmp;
    DMA_StreamRS485_Rx->CR |= (uint32_t) DMA_SxCR_EN;
  }

  if (USART_GetITStatus(USART_RS485, USART_IT_LBD) != RESET)
  {
    //Зафіксовано Break стан
    USART_ClearFlag(USART_RS485, USART_FLAG_LBD);
  }

  unsigned int error_status = USART_RS485->SR & (USART_FLAG_ORE | USART_FLAG_NE | USART_FLAG_FE | USART_FLAG_PE);
  if (error_status != 0)
  /*    
  if(
     (USART_GetITStatus(USART_RS485, USART_IT_PE  ) != RESET) ||
     (USART_GetITStatus(USART_RS485, USART_IT_NE  ) != RESET) ||
     (USART_GetITStatus(USART_RS485, USART_IT_FE  ) != RESET) ||
     (USART_GetITStatus(USART_RS485, USART_IT_ORE ) != RESET)
    )
*/
  {
    //Зафіксовано помилку під час прийому - перезапускаємо прийом через DMA
    if ((DMA_StreamRS485_Rx->CR & (uint32_t) DMA_SxCR_EN) != 0)
      DMA_StreamRS485_Rx->CR &= ~(uint32_t) DMA_SxCR_EN;
    USART_RS485->SR;
    (uint16_t)(USART_RS485->DR & (uint16_t) 0x01FF);
    // Очищаємо прапореці, що сигналізує про завершення передачі даних для DMA1 по каналу RS-485_RX
    DMA_ClearFlag(DMA_StreamRS485_Rx, DMA_FLAG_TCRS485_Rx | DMA_FLAG_HTRS485_Rx | DMA_FLAG_TEIRS485_Rx | DMA_FLAG_DMEIRS485_Rx | DMA_FLAG_FEIRS485_Rx);
    DMA_StreamRS485_Rx->NDTR = BUFFER_RS485;
    RxBuffer_RS485_count_previous = 0;
    RxBuffer_RS485_count = 0;
    DMA_StreamRS485_Rx->CR |= (uint32_t) DMA_SxCR_EN;
  }

#ifdef SYSTEM_VIEWER_ENABLE
  SEGGER_SYSVIEW_RecordExitISR();
#endif
}
/*****************************************************/

/*****************************************************/
//Переривання від завершення передачі даних через DMA потоку DMA_StreamRS485_Tx
/*****************************************************/
void DMA_StreamRS485_Tx_IRQHandler(void)
{
#ifdef SYSTEM_VIEWER_ENABLE
  SEGGER_SYSVIEW_RecordEnterISR();
#endif

  //Дозволяємо генерацію переривань при завершенні передачі байту
  USART_ITConfig(USART_RS485, USART_IT_TC, ENABLE);

  //Забороняємо генерацію переривання від DMA_StreamRS485_Tx
  DMA_StreamRS485_Tx->CR &= ~DMA_IT_TC;

  //Зупиняємо потоку DMA
  DMA_StreamRS485_Tx->CR &= ~(uint32_t) DMA_SxCR_EN;
  DMA_StreamRS485_Tx->NDTR = 0;

  //Очищаємо прапореці, що сигналізує про завершення передачі даних для DMA1 по потоку RS-485_TX
  DMA_ClearFlag(DMA_StreamRS485_Tx, DMA_FLAG_TCRS485_Tx | DMA_FLAG_HTRS485_Tx | DMA_FLAG_TEIRS485_Tx | DMA_FLAG_DMEIRS485_Tx | DMA_FLAG_FEIRS485_Tx);

#ifdef SYSTEM_VIEWER_ENABLE
  SEGGER_SYSVIEW_RecordExitISR();
#endif
}
/*****************************************************/

/*****************************************************/
//Переривання від I2C
/*****************************************************/
void EXITI_POWER_IRQHandler(void)
{
#ifdef SYSTEM_VIEWER_ENABLE
  SEGGER_SYSVIEW_RecordEnterISR();
#endif

  if (EXTI_GetITStatus(EXTI_Line_POWER) != RESET)
  {
    /* Clear the EXTI line 0 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line_POWER);

    if ((POWER_CTRL->IDR & POWER_CTRL_PIN) != (uint32_t) Bit_RESET)
    {
      //Живлення появилося на вході блоку живлення

      //Виставляємо повідомлення про цю подію
      _SET_BIT(clear_diagnostyka, EVENT_DROP_POWER_BIT);
      if (state_ar_record_fatfs != STATE_AR_WRITE_FATFS)
        reinit_LCD = true; /*реініціалізацію індикатора робимо, коли не іде запис Аналогового реєстратора*/
    }
    else
    {
      //Живлення пропало на вході блоку живлення

      //Вимикаємо підсвітку
      LCD_BL->BSRRH = LCD_BL_PIN;
      time_backlighting = 0;

      //Виставляємо повідомлення про цю подію
      _SET_BIT(set_diagnostyka, EVENT_DROP_POWER_BIT);
    }
  }

#ifdef SYSTEM_VIEWER_ENABLE
  SEGGER_SYSVIEW_RecordExitISR();
#endif
}
/*****************************************************/

/*****************************************************/
//
/*****************************************************/
/*****************************************************/

#undef BACKLIGHTING_ON
#undef BACKLIGHTING_OFF
