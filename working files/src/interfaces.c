#include "header.h"

/***********************************************************************************/
//Програма періодичного сканування USB у перериванні
/***********************************************************************************/
void Usb_routines_irq(void)
{
  static int32_t time_local;
  ++time_local;
  static int32_t time_receive;
  
  /***
  Прийом з USB
  ***/
  int ptr_out = from_USB_ptr_out_irq;
  if ((ptr_out != from_USB_ptr_in_irq) && (count_out_previous >= 0))
  {
    int diff = ptr_out - from_USB_ptr_in_irq;
    if (diff < 0) diff += BUFFER_USB_IN;
    count_out += diff;
    from_USB_ptr_in_irq = ptr_out;
    if (count_out > BUFFER_USB)
    {
      count_out = 2*BUFFER_USB;//Це означає, що на цей пакет не треба відповідати
      from_USB_ptr_in = ptr_out;
    }
    
    time_receive = time_local;
  }
  /***/

  /***
  Передача в USB
  ***/
  if (data_usb_transmiting_irq == true)
  {
    data_usb_transmiting_irq = false;
    
    if (USB_Tx_State == 0)
    { 
      uint32_t usb_transmiting_count_tmp = 0;
      static u8 buffer_USB_tmp[BUFFER_USB];  
  
      while (to_USB_ptr_in_irq != to_USB_ptr_out)
      {
        buffer_USB_tmp[usb_transmiting_count_tmp++] = buffer_USB_out[to_USB_ptr_in_irq++];
    
        if (to_USB_ptr_in_irq >= BUFFER_USB_OUT)
        {
          if (to_USB_ptr_in_irq == BUFFER_USB_OUT) to_USB_ptr_in_irq = 0;
          else total_error_sw_fixed();
        }
    
        if (usb_transmiting_count_tmp >= BUFFER_USB)
        {
          if (usb_transmiting_count_tmp == BUFFER_USB) break;
          else total_error_sw_fixed();
        }
      }
  
      if (usb_transmiting_count_tmp != 0)
      {
        if (from_USB_ptr_in_irq == from_USB_ptr_out_irq)
        {
          int32_t delta = time_local - time_receive;
          if (delta < 0) delta += (1u << 31);
          
          static int32_t max_reaction_time_USB = 0;
          static int32_t min_reaction_time_USB = 0x7fffffffu;
          static unsigned int reset_period_USB;
          if (reset_period_USB != 0)
          {
            max_reaction_time_USB = 0;
            min_reaction_time_USB = 0x7fffffffu;
    
            reset_period_USB = 0;
          }
          if (max_reaction_time_USB < delta) max_reaction_time_USB = delta;
          if (min_reaction_time_USB > delta) min_reaction_time_USB = delta;
          
          if (delta < MAX_TIMEOUT_PACKET_USB)
          {
            if (USB_OTG_dev.dev.device_state == USB_OTG_EP0_STATUS_OUT)
            {
              APP_FOPS.pIf_DataTx(buffer_USB_tmp, usb_transmiting_count_tmp);
            }
          }
        }
      }
    }
    else
    {
      to_USB_ptr_in_irq = to_USB_ptr_out;
    }
  }
  /***/
}  
/***********************************************************************************/

/***********************************************************************************/
//Програма періодичного сканування USB і організації обміну даними
/***********************************************************************************/
void Usb_routines(void)
{
  /***
  Прийом з USB
  ***/
  if (count_out != 0)
  {
    //Фіксуємо значення таймеру TIM4 у якго період тактування 10 мкс
    uint16_t current_count_tim4_USB = ((uint16_t)TIM4->CNT);
    uint16_t delta_USB = 0;//По замовчуванню 0, щоб коли прийде нова порція даних не обраховувати різницю, якщо і так зрозуміло, що вона рівна 0
    
    //Перевіряємо чи зафіксовані прийняті нові дані - і якщо так, то фіксуємо значення таймеру TIM4
    if (count_out_previous != count_out)
    {
      previous_count_tim4_USB = current_count_tim4_USB;
      count_out_previous = count_out;
    }
    else
    {
      //Після отаннього разу коли ми виконували цю частину коду - нової порції інформації не було отримано
      
      //Визначаємо скільки часу не отримуються нові дані по USB
      if (current_count_tim4_USB >= previous_count_tim4_USB) delta_USB = current_count_tim4_USB - previous_count_tim4_USB;
      else delta_USB = (0x10000 - previous_count_tim4_USB) + current_count_tim4_USB; //0x10000 - це повний період таймера, бо ми настроїли його тактуватиу інтервалі [0; 65535]
    }
    
    if (
        (delta_USB > 188) /*1 - відповідає 10 мкс, бо TIM4 настроєний з тактуванням 10 мкс. 188 віщдповідає тоді 1880 мкс. 1880 мк це час 1,5 символа на швидкості 9600 у форматі 1-start + 8-data + pare + 2-stop*/
        &&
        (data_usb_transmiting_irq == false)  
       )   
    {
      //Копіюємо кількість прийнятої інформації
      usb_received_count = count_out;
      count_out_previous = -1; //це від'ємне число каже перериванню, що зараз count_out буже скидатися
      count_out = 0;
      count_out_previous = 0; 
      
      //Копіюємо отримані дані у робочий масив
      if (usb_received_count <= BUFFER_USB)
      {
        for (int i = 0; i < usb_received_count; i++) 
        {
          usb_received[i] = buffer_USB_in[from_USB_ptr_in++];
          if (from_USB_ptr_in >= BUFFER_USB_IN)
          {
            if (from_USB_ptr_in == BUFFER_USB_IN) from_USB_ptr_in = 0;
            else total_error_sw_fixed();
          }
        }
        //Орацювання даних по протоколу MODBUS-RTU прийнятих з USB
        // inputPacketParserUSB();
      }
    }
  }
  /***/
  
  /***
  Передача в USB
  ***/
  if (data_usb_transmiting == true)
  {
    for (int i = 0; i < usb_transmiting_count; ++i)
    {
      buffer_USB_out[to_USB_ptr_out++] = usb_transmiting[i];
      if (to_USB_ptr_out >= BUFFER_USB_OUT)
      {
        if (to_USB_ptr_out == BUFFER_USB_OUT) to_USB_ptr_out = 0;
        else total_error_sw_fixed();
      }
    }

    usb_transmiting_count = 0;
    data_usb_transmiting = false;
    data_usb_transmiting_irq = true;
  }
  /***/
}  
/***********************************************************************************/

/***********************************************************************************/
//Настройка/перенастройка USART для RS-485
/***********************************************************************************/
void USART_RS485_Configure(void)
{
  USART_InitTypeDef USART_InitStructure;

  unsigned int speed_frame_RS485;
  switch (current_settings.speed_RS485)
  {
  case 0:
    {
      speed_frame_RS485 = 9600;
      break;
    }
  case 1:
    {
      speed_frame_RS485 = 14400;
      break;
    }
  case 2:
    {
      speed_frame_RS485 = 19200;
      break;
    }
  case 3:
    {
      speed_frame_RS485 = 28800;
      break;
    }
  case 4:
    {
      speed_frame_RS485 = 38400;
      break;
    }
  case 5:
    {
      speed_frame_RS485 = 57600;
      break;
    }
  default:
    {
      speed_frame_RS485 = 115200;
      break;
    }
  }
  
  unsigned int pare_RS485;
  switch (current_settings.pare_bit_RS485)
  {
  case 0:
    {
      pare_RS485 = USART_Parity_No;
      break;
    }
  case 1:
    {
      pare_RS485 = USART_Parity_Odd;
      break;
    }
  default:
    {
      pare_RS485 = USART_Parity_Even;
      break;
    }
  }

  //Переводимо мікросхкму на прийом
  GPIO_485DE->BSRRH = GPIO_PIN_485DE;

  //Забороняємо генерацію переривань
  USART_ITConfig(USART_RS485, USART_IT_TC, DISABLE);
  USART_ITConfig(USART_RS485, USART_IT_IDLE, DISABLE);
  USART_ITConfig(USART_RS485, USART_IT_LBD, DISABLE);
  USART_ITConfig(USART_RS485, USART_IT_PE, DISABLE);
  USART_ITConfig(USART_RS485, USART_IT_ERR, DISABLE);

  //Зупиняємо потік DMA_Tx якщо він запущений
  if ((DMA_StreamRS485_Tx->CR & (uint32_t)DMA_SxCR_EN) !=0) DMA_StreamRS485_Tx->CR &= ~(uint32_t)DMA_SxCR_EN;
  DMA_StreamRS485_Tx->NDTR = 0;
  //Забороняємо передачу через DMA, якщо він дозволений
  if ((USART_RS485->CR3 & USART_DMAReq_Tx) != 0) USART_RS485->CR3 &= (uint16_t)((~(uint32_t)USART_DMAReq_Tx) & 0xffff);
  // Очищаємо прапореці, що сигналізує про завершення передачі даних для DMA1 по потоку TX 
  DMA_ClearFlag(DMA_StreamRS485_Tx, DMA_FLAG_TCRS485_Tx | DMA_FLAG_HTRS485_Tx | DMA_FLAG_TEIRS485_Tx | DMA_FLAG_DMEIRS485_Tx | DMA_FLAG_FEIRS485_Tx);

  //Зупиняємо потік DMA_Rx якщо він запущений
  if ((DMA_StreamRS485_Rx->CR & (uint32_t)DMA_SxCR_EN) !=0) DMA_StreamRS485_Rx->CR &= ~(uint32_t)DMA_SxCR_EN;
  DMA_StreamRS485_Rx->NDTR = BUFFER_RS485;
  //Забороняємо передачу через DMA, якщо він дозволений
  if ((USART_RS485->CR3 & USART_DMAReq_Rx) != 0) USART_RS485->CR3 &= (uint16_t)((~(uint32_t)USART_DMAReq_Rx) & 0xffff);
  // Очищаємо прапореці, що сигналізує про завершення передачі даних для DMA1 по потоку RX 
  DMA_ClearFlag(DMA_StreamRS485_Rx, DMA_FLAG_TCRS485_Rx | DMA_FLAG_HTRS485_Rx | DMA_FLAG_TEIRS485_Rx | DMA_FLAG_DMEIRS485_Rx | DMA_FLAG_FEIRS485_Rx);
  
  //Скидаємо кількість прийнятих байт
  RxBuffer_RS485_count = 0;

  //Зумпиняємо USART
  USART_RS485->CR1 &= (uint16_t)(~USART_CR1_UE);

  //Заповняємо структуру
  USART_InitStructure.USART_BaudRate = speed_frame_RS485;
  if (pare_RS485 == USART_Parity_No) USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  else USART_InitStructure.USART_WordLength = USART_WordLength_9b;
  if (current_settings.number_stop_bit_RS485 == 0) USART_InitStructure.USART_StopBits = USART_StopBits_1;
  else USART_InitStructure.USART_StopBits = USART_StopBits_2; 
  USART_InitStructure.USART_Parity = pare_RS485;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  //Виконуємо конфігупацію USART_RS485
  USART_Init(USART_RS485, &USART_InitStructure);

  //Дозволяємо передачу через DMA
  USART_RS485->CR3 |= USART_DMAReq_Rx;
  USART_RS485->CR3 |= USART_DMAReq_Tx;

  //Дозволяємо переривання від USART для RS-485 при фіксації помикли
  USART_ITConfig(USART_RS485, USART_IT_IDLE, ENABLE);
  USART_ITConfig(USART_RS485, USART_IT_LBD, ENABLE);
  USART_ITConfig(USART_RS485, USART_IT_PE, ENABLE);
  USART_ITConfig(USART_RS485, USART_IT_ERR, ENABLE);

  //Дозволяємо USART_RS485
  USART_RS485->CR1 |= USART_CR1_UE;
}
/***********************************************************************************/

/***********************************************************************************/
//Відновлення моніторингу каналу RS-485
/***********************************************************************************/
void restart_monitoring_RS485(void)
{
  //Скидуємо всі можливі помилки
  USART_RS485->SR;
  (unsigned short int)(USART_RS485->DR & (uint16_t)0x01FF);
      
  if ((DMA_StreamRS485_Rx->CR & (uint32_t)DMA_SxCR_EN) !=0) DMA_StreamRS485_Rx->CR &= ~(uint32_t)DMA_SxCR_EN;

  // Очищаємо прапореці, що сигналізує про завершення передачі даних для DMA1 по каналу RS-485_RX 
  DMA_ClearFlag(DMA_StreamRS485_Rx, DMA_FLAG_TCRS485_Rx | DMA_FLAG_HTRS485_Rx | DMA_FLAG_TEIRS485_Rx | DMA_FLAG_DMEIRS485_Rx | DMA_FLAG_FEIRS485_Rx);

  DMA_StreamRS485_Rx->NDTR = BUFFER_RS485;
  RxBuffer_RS485_count_previous = 0;
  RxBuffer_RS485_count = 0;
  //Запускаємо очікування прийому
  DMA_StreamRS485_Rx->CR |= (uint32_t)DMA_SxCR_EN;
}
/***********************************************************************************/

/***********************************************************************************/
//
/***********************************************************************************/
/***********************************************************************************/
