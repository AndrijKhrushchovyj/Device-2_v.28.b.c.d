#include "header.h"

/***********************************************************************************/
//Подача команди нва передачу даних через інтерфейс RS-485
/***********************************************************************************/
void start_transmint_data_via_RS_485(unsigned int count)
{
  //Перевіряємо, чи можна передавати дані
  unsigned int current_time_tim2 = TIM2->CNT;
  uint64_t delta_time;

  //Вираховуємо час, протягом якого підготовлювалася відповідь
  if (
      (current_time_tim2 > time_last_receive_byte_RS485) ||
      (
       (current_time_tim2 == time_last_receive_byte_RS485) &&
       (mark_current_tick_RS_485 != 0)  
      ) 
     )   
    delta_time = current_time_tim2 - time_last_receive_byte_RS485;
  else 
    delta_time = current_time_tim2 + 0x100000000ull - time_last_receive_byte_RS485;

  static uint64_t max_reaction_time_rs_485 = 0;
  static uint64_t min_reaction_time_rs_485 = 0x100000000ull;
  static uint32_t reset_period_rs_485;
  if (reset_period_rs_485 != 0)
  {
    max_reaction_time_rs_485 = 0;
    min_reaction_time_rs_485 = 0x100000000ull;
    
    reset_period_rs_485 = 0;
  }
  if (max_reaction_time_rs_485 < delta_time) max_reaction_time_rs_485 = delta_time;
  if (min_reaction_time_rs_485 > delta_time) min_reaction_time_rs_485 = delta_time;
  
  if (delta_time < MAX_TIMEOUT_PACKET_485)
  {
    //Можна відповідати на запит

    //Переводимо мікросхкму на передачу
    GPIO_485DE->BSRRL = GPIO_PIN_485DE;

    //Скидаємо біт, що символізує, що опстанній байт переданий
    USART_ClearFlag(USART_RS485, USART_FLAG_TC);

    //Дозволяємо генерацію переривання, коли  буде передано всі даних по DMA1 через канал DMA1_ChannelRS485_TX
    DMA_StreamRS485_Tx->CR |= DMA_IT_TC;

    //Зупиняємо потік DMA якщо він запущений
    if ((DMA_StreamRS485_Tx->CR & (uint32_t)DMA_SxCR_EN) !=0) DMA_StreamRS485_Tx->CR &= ~(uint32_t)DMA_SxCR_EN;
    DMA_StreamRS485_Tx->NDTR = count;
    //Дозволяємо передачу через DMA
    if ((USART_RS485->CR3 & USART_DMAReq_Tx) == 0) USART_RS485->CR3 |= USART_DMAReq_Tx;

    //Очищаємо прапореці, що сигналізує про завершення передачі даних для DMA1 по потоку RS-485_TX
    DMA_ClearFlag(DMA_StreamRS485_Tx, DMA_FLAG_TCRS485_Tx | DMA_FLAG_HTRS485_Tx | DMA_FLAG_TEIRS485_Tx | DMA_FLAG_DMEIRS485_Tx | DMA_FLAG_FEIRS485_Tx);
    //Запускаємо передачу
    DMA_StreamRS485_Tx->CR |= (uint32_t)DMA_SxCR_EN;
  }
  else
  {
    //На запит відповідати не можна, бо відповідь готувался більший час ніж відведений час формування відповіді - Треба приступити до нового моніыторингу лінії RS-485
    restart_monitoring_RS485();
  }
}
/***********************************************************************************/
