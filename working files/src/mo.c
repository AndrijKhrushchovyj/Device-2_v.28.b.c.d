#include "header.h"

#ifdef _TEST_DURATION
uint64_t periodPacketCur = 0;
#endif

__control_info const control_info[] =
  {
    {&current_settings_prt.control_rpn, INDEX_ML_CTRRPN_TRANSF},
    {&current_settings_prt.control_rpn, INDEX_ML_CTRRPN_STATE},
    {&current_settings_prt.control_rpn, N_BIT_CTRMTZ_3},
    {&current_settings_prt.control_rpn, N_BIT_CTRMTZ_4},

    {&current_settings_prt.control_zskh, N_BIT_CTRMTZ04_1},
    {&current_settings_prt.control_zskh, N_BIT_CTRMTZ04_2},

    {&current_settings_prt.control_brp, INDEX_ML_CTRBRP_STATE},

    {&current_settings_prt.control_znkh, INDEX_ML_CTRZNKh_STATE},

    {&current_settings_prt.control_zz, INDEX_ML_CTR_ZZ1_3I0_STATE},
    {&current_settings_prt.control_zz, INDEX_ML_CTR_ZZ1_3U0_STATE},
    {&current_settings_prt.control_zz, INDEX_ML_CTR_ZZ1_NZZ_STATE},

    {&current_settings_prt.control_tznp, INDEX_ML_CTR_TZNP1},
    {&current_settings_prt.control_tznp, INDEX_ML_CTR_TZNP2},
    {&current_settings_prt.control_tznp, INDEX_ML_CTR_TZNP3},

    {&current_settings_prt.control_apv, INDEX_ML_CTRAPV_STAGE_1},
    {&current_settings_prt.control_apv, INDEX_ML_CTRAPV_STAGE_2},
    {&current_settings_prt.control_apv, INDEX_ML_CTRAPV_STAGE_3},
    {&current_settings_prt.control_apv, INDEX_ML_CTRAPV_STAGE_4},

    {&current_settings_prt.control_achr_chapv, INDEX_CTR_ACHR1},
    {&current_settings_prt.control_achr_chapv, INDEX_CTR_CHAPV1},
    {&current_settings_prt.control_achr_chapv, INDEX_CTR_ACHR2},
    {&current_settings_prt.control_achr_chapv, INDEX_CTR_CHAPV2},

    {&current_settings_prt.control_urov, INDEX_ML_CTRUROV_STATE},

    {&current_settings_prt.control_zop, CTR_ZOP_STATE_BIT},

    {&current_settings_prt.control_Umin, INDEX_CTR_UMIN1},
    {&current_settings_prt.control_Umin, INDEX_CTR_UMIN2},

    {&current_settings_prt.control_Umax, INDEX_CTR_UMAX1},
    {&current_settings_prt.control_Umax, INDEX_CTR_UMAX2},

    {&current_settings_prt.control_UP, (0 * (_CTR_UP_NEXT_BIT - _CTR_UP_PART_II) + CTR_UP_STATE_BIT - _CTR_UP_PART_II)},
    {&current_settings_prt.control_UP, (1 * (_CTR_UP_NEXT_BIT - _CTR_UP_PART_II) + CTR_UP_STATE_BIT - _CTR_UP_PART_II)},
    {&current_settings_prt.control_UP, (2 * (_CTR_UP_NEXT_BIT - _CTR_UP_PART_II) + CTR_UP_STATE_BIT - _CTR_UP_PART_II)},
    {&current_settings_prt.control_UP, (3 * (_CTR_UP_NEXT_BIT - _CTR_UP_PART_II) + CTR_UP_STATE_BIT - _CTR_UP_PART_II)},
    {&current_settings_prt.control_UP, (4 * (_CTR_UP_NEXT_BIT - _CTR_UP_PART_II) + CTR_UP_STATE_BIT - _CTR_UP_PART_II)},
    {&current_settings_prt.control_UP, (5 * (_CTR_UP_NEXT_BIT - _CTR_UP_PART_II) + CTR_UP_STATE_BIT - _CTR_UP_PART_II)},
    {&current_settings_prt.control_UP, (6 * (_CTR_UP_NEXT_BIT - _CTR_UP_PART_II) + CTR_UP_STATE_BIT - _CTR_UP_PART_II)},
    {&current_settings_prt.control_UP, (7 * (_CTR_UP_NEXT_BIT - _CTR_UP_PART_II) + CTR_UP_STATE_BIT - _CTR_UP_PART_II)},

    {&current_settings_prt.configuration, EL_BIT_CONFIGURATION},

    {&current_settings_prt.control_vmp, INDEX_CTR_VMP_STATE},

    {&current_settings_prt.control_switch, INDEX_ML_CTRPRYVOD_VV},
    {&current_settings_prt.control_switch, INDEX_ML_CTRRESURS_VV},

    {&current_settings_prt.control_ar, INDEX_ML_CTR_AR_AVAR_STATE}

#ifdef NUMBER_DS
    ,
    {&current_settings_prt.configuration, DS_BIT_CONFIGURATION}
#endif
};

#define SIZE_CONTROL_INFO (sizeof(control_info) / sizeof(__control_info))
#define SIZE_BLOCK_CONTROL_INFO ((SIZE_CONTROL_INFO / 8) + ((SIZE_CONTROL_INFO % 8) != 0))

unsigned short dr;

/***********************************************************************************
Прийом даних через інтерфейс CANAL1_MO
***********************************************************************************/
void start_receive_data_via_CANAL1_MO()
{
#ifdef _TEST_DURATION
  uint32_t const start_tick = TIM2->CNT;
#endif
  //LCD_BL->BSRRH = LCD_BL_PIN;//
  GPIO_DEBUG->BSRRH = GPIO_PIN_DEBUG;

  uint8_t(*const pCanal1_MO_Received)[BUFFER_CANAL1_MO] = &Canal1_MO_Received[Canal1_MO_ReceiveBank];
  Canal1_MO_ReceiveBank = (Canal1_MO_ReceiveBank + 1) & 0x1;

#ifdef _TEST_DURATION
  static uint64_t periodPacketMax = 0;
  static uint64_t periodPacketMin = 0x100000000ull;
  static uint32_t periodPacketReset = 1; /*щоб першу точку неправильно гарантовано пропустити*/

  static uint64_t period_tick;
  static uint32_t prev_start_tick;
  uint64_t const period_tick_tmp = (start_tick > prev_start_tick) ? (start_tick - prev_start_tick) : (0x100000000ull + start_tick - prev_start_tick);
  prev_start_tick = start_tick;
#endif

  //Попередньо скидаємо повідомплення про помилки, які потім будемо виставляти
  clear_diagnostyka[0] |= WORD_0_MASKA_ERRORS_FROM_CANAL_1;
  clear_diagnostyka[1] |= WORD_1_MASKA_ERRORS_FROM_CANAL_1;
  clear_diagnostyka[2] |= WORD_2_MASKA_ERRORS_FROM_CANAL_1;
  clear_diagnostyka[3] |= WORD_3_MASKA_ERRORS_FROM_CANAL_1;
  clear_diagnostyka[4] |= WORD_4_MASKA_ERRORS_FROM_CANAL_1;

  uint32_t error_status = 0xffffffff;
  int32_t size_packet = 0;

  //Зупиняэмо канал приймання
  if ((DMA_StreamCANAL1_MO_Rx->CR & (uint32_t) DMA_SxCR_EN) != 0)
    DMA_StreamCANAL1_MO_Rx->CR &= ~(uint32_t) DMA_SxCR_EN;

  static unsigned int lock_error_no_answer;
  if (DMA_StreamCANAL1_MO_Rx->NDTR != BUFFER_CANAL1_MO)
  {
    lock_error_no_answer = 0;
    //    do
    //    {
    error_status = CANAL1_MO->SR;
    //    }
    //    while ((error_status & (USART_FLAG_IDLE | USART_FLAG_LBD)) == 0);
    size_packet = BUFFER_CANAL1_MO - (uint16_t)(DMA_StreamCANAL1_MO_Rx->NDTR);

    error_status &= (USART_FLAG_ORE | USART_FLAG_NE | USART_FLAG_FE | USART_FLAG_PE);
  }
  else
  {
    //Не прийняті дані з комунікаційної плати по каналу 1
    if (restart_KP_irq == 0)
    {
      if (IEC_board_uncall == 0)
      {
        if (
          (_CHECK_SET_BIT(diagnostyka, WARNING_REPROGRAM) == 0) &&
          (_CHECK_SET_BIT(set_diagnostyka, WARNING_REPROGRAM) == 0))
        {
          if (++lock_error_no_answer >= COARSENING)
          {
            _SET_BIT(set_diagnostyka, ERROR_CPU_NO_ANSWER_CANAL_1);
            lock_error_no_answer &= ~(1u << 31); // щоб не винекла ситуація переходу з максимального числа до нуля
          }
        }
        else
          lock_error_no_answer = 0;
      }
      else
        IEC_board_uncall--;
    }
  }

  //Скидуємо всі можливі помилки
  CANAL1_MO->SR;
  dr = (unsigned short int) (CANAL1_MO->DR & (uint16_t) 0x01FF);
  CANAL1_MO->SR = (uint16_t)(~(uint32_t) USART_FLAG_LBD);

  DMA_StreamCANAL1_MO_Rx->M0AR = (uint32_t) Canal1_MO_Received[Canal1_MO_ReceiveBank];
  DMA_StreamCANAL1_MO_Rx->NDTR = BUFFER_CANAL1_MO;

  // Очищаємо прапореці, що сигналізує про завершення передачі даних для DMA1 по каналу RS-485_RX
  DMA_ClearFlag(DMA_StreamCANAL1_MO_Rx, DMA_FLAG_TCCANAL1_MO_Rx | DMA_FLAG_HTCANAL1_MO_Rx | DMA_FLAG_TEICANAL1_MO_Rx | DMA_FLAG_DMEICANAL1_MO_Rx | DMA_FLAG_FEICANAL1_MO_Rx);

  //Запускаємо очікування прийому
  DMA_StreamCANAL1_MO_Rx->CR |= (uint32_t) DMA_SxCR_EN;

  if (restart_KP_irq == 0)
  {
    //Перевіряємо чи прийшли дані по каналу CANAL1_MO з комунікаційної плати
    GPIO_CANAL1_MO_Out1->BSRRL = GPIO_PIN_CANAL1_MO_Out1; //Переводимо пін canal1_Out1 в стан "1"
  }
  else
  {
    _CLEAR_STATE(queue_mo, STATE_QUEUE_MO_RESTART_KP);

    if (--restart_KP_irq != 0)
      GPIO_KP_SOFT_RESET->BSRRL = GPIO_PIN_KP_SOFT_RESET; //Подаємо команду на перезапуск комунікаціної плати
    else
    {
      _SET_BIT(clear_diagnostyka, EVENT_RESTART_CB_BIT);
      GPIO_KP_SOFT_RESET->BSRRH = GPIO_PIN_KP_SOFT_RESET; //Знімаємо команду на перезапуск комунікаціної плати

      queue_mo_irq = 0;
      IEC_board_uncall = 200;
    }
  }

  //Прийняті дані з комунікаційної плати по каналу 1
  static unsigned int lock_error_receiving;
  if (
    (error_status == 0) &&
    (size_packet >= 3) &&
    ((*pCanal1_MO_Received)[0] == START_BYTE_MO) &&
    ((*pCanal1_MO_Received)[size_packet - 1] == STOP_BYTE_MO))
  {
    lock_error_receiving = 0;
    //Перевіряємо адресу
    if (
      ((*pCanal1_MO_Received)[1] == BROADCAST_ADDRESS_MO) ||
      ((*pCanal1_MO_Received)[1] == my_address_mo))
    {
      //Перевіряємо контрольну суму
      uint8_t sum = 0;
      for (int32_t i = 0; i < (size_packet - 3); i++)
        sum += (*pCanal1_MO_Received)[1 + i];

      static unsigned int lock_error_received_packet;
      if (sum == (*pCanal1_MO_Received)[size_packet - 2])
      {
        lock_error_received_packet = 0;
        IEC_board_uncall = 0;
        IEC_board_address = (*pCanal1_MO_Received)[2];

        uint32_t index = 4;
        switch ((*pCanal1_MO_Received)[3])
        {
          case SENDING_GOOSE_INFO:
            {
#ifdef _TEST_DURATION
              period_tick = 0;
#endif

              //Черга з КП для обміну між КП і БАВ
              uint32_t IEC_queue_mo = 0;
              uint8_t *point = (uint8_t *) (&IEC_queue_mo);
              for (uint32_t i = 0; ((i < sizeof(IEC_queue_mo)) && (index < BUFFER_CANAL1_MO)); i++)
              {
                *(point++) = (*pCanal1_MO_Received)[index++];
              }

              if (_GET_STATE(IEC_queue_mo, IEC_STATE_QUEUE_MO_ASK_BASIC_SETTINGS))
                _SET_STATE(queue_mo_irq, STATE_QUEUE_MO_ASK_BASIC_SETTINGS);
              else
                _CLEAR_STATE(queue_mo_irq, STATE_QUEUE_MO_ASK_BASIC_SETTINGS);

              if (_GET_STATE(IEC_queue_mo, IEC_STATE_QUEUE_MO_ASK_MAKING_MEMORY_BLOCK))
                _SET_STATE(queue_mo_irq, STATE_QUEUE_MO_ASK_MAKING_MEMORY_BLOCK);
              else
                _CLEAR_STATE(queue_mo_irq, STATE_QUEUE_MO_ASK_MAKING_MEMORY_BLOCK);

              if (_GET_STATE(IEC_queue_mo, IEC_STATE_QUEUE_MO_ASK_SENDING_SETTING_NETWORK_LAYER))
                _SET_STATE(queue_mo_irq, STATE_QUEUE_MO_ASK_SENDING_SETTING_NETWORK_LAYER);
              else
                _CLEAR_STATE(queue_mo_irq, STATE_QUEUE_MO_ASK_SENDING_SETTING_NETWORK_LAYER);

              if (_GET_STATE(IEC_queue_mo, IEC_STATE_QUEUE_MO_NEW_MODBUS_TCP_REQ))
                _SET_STATE(queue_mo_irq, STATE_QUEUE_MO_ASK_NEW_MODBUS_TCP_REQ);
              else
                _CLEAR_STATE(queue_mo_irq, STATE_QUEUE_MO_ASK_NEW_MODBUS_TCP_REQ);

              if (_GET_STATE(IEC_queue_mo, IEC_STATE_QUEUE_MO_TRANSACTION_PROGRESS))
                _SET_STATE(queue_mo_irq, STATE_QUEUE_MO_TRANSACTION_PROGRESS_IN_IEC);
              else
                _CLEAR_STATE(queue_mo_irq, STATE_QUEUE_MO_TRANSACTION_PROGRESS_IN_IEC);

              if (_GET_STATE(IEC_queue_mo, IEC_STATE_QUEUE_MO_REBOOT_REQ))
                _SET_STATE(queue_mo_irq, STATE_QUEUE_MO_RESTART_KP);
              else
                _CLEAR_STATE(queue_mo_irq, STATE_QUEUE_MO_RESTART_KP);

              //Синхронізація часу
              index += sizeof(time_t) + sizeof(int32_t);
              if ((*pCanal1_MO_Received)[index++] != 0)
              {
                //Треба активувати системний час
                int32_t index_tmp = index - 1 - sizeof(time_t) - sizeof(int32_t);
                if (index_tmp > 0)
                {
                  for (size_t i = 0; i < sizeof(time_t); i++)
                    *((uint8_t *) (&time_dat_save_h) + i) = (*pCanal1_MO_Received)[index_tmp++];
                  int32_t time_ms_tmp = 0;
                  for (size_t i = 0; i < sizeof(int32_t); i++)
                    *((uint8_t *) (&time_ms_tmp) + i) = (*pCanal1_MO_Received)[index_tmp++];
                  time_ms_save_h = time_ms_tmp / 1000;
                  save_time_dat_h = 3;
                }
                else
                  total_error_sw_fixed();
              }

              /***
            Оперативні дані
            ***/
              while (
                ((*pCanal1_MO_Received)[index] == START_LABEL_NEW_TM) &&
                (index < BUFFER_CANAL1_MO))
              {
                index++;
                if ((*pCanal1_MO_Received)[index++] == 0) /*для цього приладу створюється єдиний блок*/
                {
                  size_t shift = (*pCanal1_MO_Received)[index + 0] | ((*pCanal1_MO_Received)[index + 1] << 8);
                  size_t size = (*pCanal1_MO_Received)[index + 2] | ((*pCanal1_MO_Received)[index + 3] << 8);
                  index += 4;

                  if ((index + size) < BUFFER_CANAL1_MO)
                  {
                    for (size_t i = 0; i < size; i++)
                    {
                      size_t shift_tmp = shift + i;
                      if (shift_tmp < sizeof(Input_In_GOOSE_block))
                      {
                        *((uint8_t *) (&Input_In_GOOSE_block) + shift_tmp) = (*pCanal1_MO_Received)[index++];
                      }
                      else
                      {
                        shift_tmp -= sizeof(Input_In_GOOSE_block);
                        if (shift_tmp < sizeof(Input_In_MMS_block))
                        {
                          *((uint8_t *) (&Input_In_MMS_block) + shift_tmp) = (*pCanal1_MO_Received)[index++];
                        }
                        else
                        {
                          shift_tmp -= sizeof(Input_In_MMS_block);
                          if (shift_tmp < sizeof(activation_function_from_KP))
                          {
                            *((uint8_t *) (&activation_function_from_KP) + shift_tmp) = (*pCanal1_MO_Received)[index++];
                          }
                          else
                          {
                            shift_tmp -= sizeof(activation_function_from_KP);
                            if (shift_tmp < sizeof(Input_ctrl_In_GOOSE_block))
                            {
                              *((uint8_t *) (&Input_ctrl_In_GOOSE_block) + shift_tmp) = (*pCanal1_MO_Received)[index++];
                            }
                            else
                            {
                              shift_tmp -= sizeof(Input_ctrl_In_GOOSE_block);
                              if (shift_tmp < sizeof(Input_ctrl_In_MMS_block))
                              {
                                *((uint8_t *) (&Input_ctrl_In_MMS_block) + shift_tmp) = (*pCanal1_MO_Received)[index++];
                              }
                              else
                              {
                                shift_tmp -= sizeof(Input_ctrl_In_MMS_block);
                                if (shift_tmp < sizeof(ctrl_activation_function_from_KP))
                                {
                                  *((uint8_t *) (&ctrl_activation_function_from_KP) + shift_tmp) = (*pCanal1_MO_Received)[index++];
                                }
                                else
                                {
                                  total_error_sw_fixed();
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                  else
                    total_error_sw_fixed();
                }
                else
                  total_error_sw_fixed();
              }

              if (
                ((index + 1 + sizeof(confirm_diagnostyka_mo)) < BUFFER_CANAL1_MO) &&
                ((*pCanal1_MO_Received)[index++] == START_LABEL_CONF_DIAGN))
              {
                //Діагностика обміну по Каналу1
                point = (uint8_t *) (&confirm_diagnostyka_mo);
                for (uint32_t i = 0; i < sizeof(confirm_diagnostyka_mo); i++)
                {
                  *(point++) = (*pCanal1_MO_Received)[index++];
                }

                static int block_errors;
                if (
                  (_CHECK_SET_BIT(diagnostyka, WARNING_REPROGRAM) == 0) &&
                  (_CHECK_SET_BIT(set_diagnostyka, WARNING_REPROGRAM) == 0))
                {
                  if (--block_errors < 0)
                  {
                    if (_GET_STATE(confirm_diagnostyka_mo, ERROR_RECEIVING_CANAL_1))
                      _SET_BIT(set_diagnostyka, ERROR_IEC_RECEIVING_CANAL_1);
                    if (_GET_STATE(confirm_diagnostyka_mo, ERROR_RECEIVED_PACKET_CANAL_1))
                      _SET_BIT(set_diagnostyka, ERROR_IEC_RECEIVED_PACKET_CANAL_1);
                    if (_GET_STATE(confirm_diagnostyka_mo, ERROR_REQUEST_CANAL_1))
                      _SET_BIT(set_diagnostyka, ERROR_IEC_REQUEST_CANAL_1);
                    if (_GET_STATE(confirm_diagnostyka_mo, ERROR_NO_ANSWER_CANAL_1))
                      _SET_BIT(set_diagnostyka, ERROR_IEC_NO_ANSWER_CANAL_1);

                    if (_GET_STATE(confirm_diagnostyka_mo, ERROR_RECEIVING_CANAL_2))
                      _SET_BIT(set_diagnostyka, ERROR_IEC_RECEIVING_CANAL_2);
                    if (_GET_STATE(confirm_diagnostyka_mo, ERROR_RECEIVED_PACKET_CANAL_2))
                      _SET_BIT(set_diagnostyka, ERROR_IEC_RECEIVED_PACKET_CANAL_2);
                    if (_GET_STATE(confirm_diagnostyka_mo, ERROR_REQUEST_CANAL_2))
                      _SET_BIT(set_diagnostyka, ERROR_IEC_REQUEST_CANAL_2);

                    block_errors = 0;
                  }
                }
                else
                  block_errors = 2;
              }

              /***/
              break;
            }
          default:
            {
              _SET_BIT(set_diagnostyka, ERROR_CPU_ANSWER_CANAL_1);

              break;
            }
        }
      }
      else
      {
        if (
          (_CHECK_SET_BIT(diagnostyka, WARNING_REPROGRAM) == 0) &&
          (_CHECK_SET_BIT(set_diagnostyka, WARNING_REPROGRAM) == 0))
        {
          if (++lock_error_received_packet >= COARSENING)
          {
            _SET_BIT(set_diagnostyka, ERROR_CPU_RECEIVED_PACKET_CANAL_1);
            lock_error_received_packet &= ~(1u << 31); // щоб не винекла ситуація переходу з максимального числа до нуля
          }
        }
        else
          lock_error_received_packet = 0;
      }
    }
  }
  else if (error_status != 0xffffffff)
  {
    if (
      (_CHECK_SET_BIT(diagnostyka, WARNING_REPROGRAM) == 0) &&
      (_CHECK_SET_BIT(set_diagnostyka, WARNING_REPROGRAM) == 0))
    {
      if (++lock_error_receiving >= COARSENING)
      {
        _SET_BIT(set_diagnostyka, ERROR_CPU_RECEIVING_CANAL_1);
        lock_error_receiving &= ~(1u << 31); // щоб не винекла ситуація переходу з максимального числа до нуля
        //LCD_BL->BSRRL = LCD_BL_PIN;
        GPIO_DEBUG->BSRRL = GPIO_PIN_DEBUG;
      }
    }
    else
      lock_error_receiving = 0;
  }

  GPIO_CANAL1_MO_Out1->BSRRH = GPIO_PIN_CANAL1_MO_Out1; //Переводимо пін canal1_Out1 в стан "0"

#ifdef _TEST_DURATION
  if ((restart_KP_irq == 0) && (IEC_board_uncall == 0))
  {
    period_tick += period_tick_tmp;
    if (periodPacketMax < period_tick)
      periodPacketMax = period_tick;
    if (periodPacketMin > period_tick)
      periodPacketMin = period_tick;
    if (periodPacketReset != 0)
    {
      periodPacketMax = 0;
      periodPacketMin = 0x100000000ull;
      periodPacketReset = 0;
    }
  }
  else
  {
    period_tick = 0;
    periodPacketReset = 1;
  }
  periodPacketCur = period_tick;
#endif
}
/***********************************************************************************/

/***********************************************************************************
Подача команди нва передачу даних через інтерфейс CANAL1_MO
***********************************************************************************/
void start_transmint_data_via_CANAL1_MO(void)
{
  uint8_t sum = 0;
  uint32_t index = 0;

  Canal1_MO_Transmit[index++] = START_BYTE_MO;
  sum += Canal1_MO_Transmit[index++] = BROADCAST_ADDRESS_MO;
  sum += Canal1_MO_Transmit[index++] = my_address_mo;

  sum += Canal1_MO_Transmit[index++] = SENDIND_TM_INFO;

  for (size_t i = 0; i < sizeof(time_t); i++)
    sum += Canal1_MO_Transmit[index++] = *((uint8_t *) (&time_dat) + i);
  int32_t time_ms_tmp = time_ms * 1000;
  for (size_t i = 0; i < sizeof(int32_t); i++)
    sum += Canal1_MO_Transmit[index++] = *((uint8_t *) (&time_ms_tmp) + i);

  //Оперативні дані
  if ((index + 1 + 1 + 2 + 2 + SIZE_SENDING_DATA_TM + SIZE_BLOCK_CONTROL_INFO) < BUFFER_CANAL1_MO)
  {
    sum += Canal1_MO_Transmit[index++] = START_LABEL_NEW_TM;

    sum += Canal1_MO_Transmit[index++] = 0;

    sum += Canal1_MO_Transmit[index++] = 0;
    sum += Canal1_MO_Transmit[index++] = 0;

    sum += Canal1_MO_Transmit[index++] = ((SIZE_SENDING_DATA_TM + SIZE_BLOCK_CONTROL_INFO) & 0xff);
    sum += Canal1_MO_Transmit[index++] = (((SIZE_SENDING_DATA_TM + SIZE_BLOCK_CONTROL_INFO) >> 8) & 0xff);

    //Вимірювання з аналогових давачів
    for (size_t i = 0; i < _NUMBER_IM; ++i)
    {
      static unsigned int const *const p_koef[_NUMBER_IM] =
        {
          &current_settings_prt.T0,
          &current_settings_prt.T0,
          &current_settings_prt.T0,
          &current_settings_prt.TCurrent,
          &current_settings_prt.TCurrent,
          &current_settings_prt.TCurrent,
          &current_settings_prt.TCurrent,
          &current_settings_prt.TCurrent,
          &current_settings_prt.TCurrent,
          &current_settings_prt.TCurrent04,
          &current_settings_prt.TVoltage,
          &current_settings_prt.TVoltage,
          &current_settings_prt.TVoltage,
          &current_settings_prt.TVoltage,
          &current_settings_prt.TVoltage,
          &current_settings_prt.TVoltage,
          &current_settings_prt.TVoltage,
          &current_settings_prt.TVoltage,
          &current_settings_prt.TVoltage,
          &current_settings_prt.TCurrent,
          &current_settings_prt.TCurrent,
          &current_settings_prt.TCurrent};

      float const meas_tmp = ((float) (*p_koef[i])) * ((float) measurement[i]) / 1000.0f;
      uint8_t const *p = (uint8_t const *) (&meas_tmp);
      for (size_t j = 0; j < sizeof(meas_tmp); ++j)
      {
        sum += Canal1_MO_Transmit[index++] = *p++;
      }
    }

    //Частота
    {
      uint8_t const *point = (uint8_t const *) (&frequency);
      for (uint32_t i = 0; i < sizeof(frequency); i++)
      {
        sum += Canal1_MO_Transmit[index++] = *(point++);
      }
    }

    //Вимірювання, які одночасно залежні від коефіцієнта трансормації струму і напруги
    {
      static unsigned int const *const p_koef_U = &current_settings_prt.TVoltage;
      static unsigned int const *const p_koef_I = &current_settings_prt.TCurrent;

      //Опори (Ом)
      float const div_tmp = ((float) (*p_koef_U)) / ((float) (*p_koef_I));
      for (size_t i = 0; i < MAX_NUMBER_INDEXES_RESISTANCE; ++i)
      {
        float const meas_tmp = div_tmp * ((float) resistance[i]);
        uint8_t const *p = (uint8_t const *) (&meas_tmp);
        for (size_t j = 0; j < sizeof(meas_tmp); ++j)
        {
          sum += Canal1_MO_Transmit[index++] = *p++;
        }
      }

      float const mul_tmp = ((float) (*p_koef_U)) * ((float) (*p_koef_I));
      uint32_t bank_for_calc_power_tmp = (state_calc_power == false) ? bank_for_calc_power : ((bank_for_calc_power ^ 0x1) & 0x1);

      //Активнам потужність (Вт)
      {
        float const meas_tmp = mul_tmp * ((float) P[bank_for_calc_power_tmp]) / 1000.0f;
        uint8_t const *p = (uint8_t const *) (&meas_tmp);
        for (size_t j = 0; j < sizeof(meas_tmp); ++j)
        {
          sum += Canal1_MO_Transmit[index++] = *p++;
        }
      }

      //Реактивна потужність (ВАр)
      {
        float const meas_tmp = mul_tmp * ((float) Q[bank_for_calc_power_tmp]) / 1000.0f;
        uint8_t const *p = (uint8_t const *) (&meas_tmp);
        for (size_t j = 0; j < sizeof(meas_tmp); ++j)
        {
          sum += Canal1_MO_Transmit[index++] = *p++;
        }
      }

      //Повна потужність (ВА)
      {
        float const meas_tmp = mul_tmp * ((float) S[bank_for_calc_power_tmp]) / 1000.0f;
        uint8_t const *p = (uint8_t const *) (&meas_tmp);
        for (size_t j = 0; j < sizeof(meas_tmp); ++j)
        {
          sum += Canal1_MO_Transmit[index++] = *p++;
        }
      }

      //Коефіцієнт потужності
      {
        float const meas_tmp = cos_phi_x1000[bank_for_calc_power_tmp] / 1000.0f;
        uint8_t const *p = (uint8_t const *) (&meas_tmp);
        for (size_t j = 0; j < sizeof(meas_tmp); ++j)
        {
          sum += Canal1_MO_Transmit[index++] = *p++;
        }
      }

      //Енергії (кВт*год)
      uint32_t bank_for_calc_energy_tmp = (state_calc_energy == false) ? 0 : 1;
      double const *p_energy = energy[bank_for_calc_energy_tmp];
      for (size_t i = 0; i < MAX_NUMBER_INDEXES_ENERGY; ++i)
      {
        float const meas_tmp = (float) (((double) mul_tmp) * p_energy[i]);
        uint8_t const *p = (uint8_t const *) (&meas_tmp);
        for (size_t j = 0; j < sizeof(meas_tmp); ++j)
        {
          sum += Canal1_MO_Transmit[index++] = *p++;
        }
      }
    }

    //Кут між вимірюванням і базовою напругою (°)
    uint32_t bank_for_calc_phi_angle_tmp = (state_calc_phi_angle == false) ? bank_for_calc_phi_angle : ((bank_for_calc_phi_angle ^ 0x1) & 0x1);

    int const *p_phi_angle = phi_angle[bank_for_calc_phi_angle_tmp];
    for (size_t i = 0; i < FULL_ORT_MAX; ++i)
    {
      float const meas_tmp = ((float) p_phi_angle[i]) / 10.0f;
      uint8_t const *p = (uint8_t const *) (&meas_tmp);
      for (size_t j = 0; j < sizeof(meas_tmp); ++j)
      {
        sum += Canal1_MO_Transmit[index++] = *p++;
      }
    }

    //Ресурс вимикача + кількість вимкнень
    {
      //Ресурс вимикача (у відсотках)
      static unsigned int const *const p_setpoint_r_kom_st_Inom = &current_settings_prt.setpoint_r_kom_st_Inom;
      {
        float const meas_tmp = ((float) resurs_vymykacha) * 100.0f / ((float) (*p_setpoint_r_kom_st_Inom));
        uint8_t const *p = (uint8_t const *) (&meas_tmp);
        for (size_t j = 0; j < sizeof(meas_tmp); ++j)
        {
          sum += Canal1_MO_Transmit[index++] = *p++;
        }
      }

      //Кількість вимкнень
      {
        uint8_t const *p = (uint8_t const *) (&resurs_vidkljuchennja);
        for (uint32_t i = 0; i < sizeof(resurs_vidkljuchennja); i++)
        {
          sum += Canal1_MO_Transmit[index++] = *(p++);
        }
      }
    }

    active_inputs_lock_guard_prt = 0xff;
    unsigned int const active_inputs_local = active_inputs_prt;
    active_inputs_lock_guard_prt = 0x0;
    for (uint32_t i = 0; i < sizeof(active_inputs_local); i++)
    {
      sum += Canal1_MO_Transmit[index++] = *(((uint8_t *) &active_inputs_local) + i);
    }

    for (uint32_t i = 0; i < sizeof(state_outputs); i++)
    {
      sum += Canal1_MO_Transmit[index++] = *(((uint8_t *) &state_outputs) + i);
    }

#ifdef NUMBER_DS
    for (size_t i = 0; i < sizeof(ds); ++i)
    {
      sum += Canal1_MO_Transmit[index++] = *(((uint8_t *) &ds) + i);
    }
#endif

    for (uint32_t i = 0; i < sizeof(state_leds); i++)
    {
      sum += Canal1_MO_Transmit[index++] = *(((uint8_t *) &state_leds) + i);
    }

    for (uint32_t i = 0; i < sizeof(active_functions); i++)
    {
      sum += Canal1_MO_Transmit[index++] = *(((uint8_t *) active_functions) + i);
    }

    for (uint32_t i = 0; i < sizeof(diagnostyka); i++)
    {
      sum += Canal1_MO_Transmit[index++] = *(((uint8_t *) diagnostyka) + i);
    }

    for (uint32_t i = 0; i < sizeof(Output_Out_LAN_block); i++)
    {
      sum += Canal1_MO_Transmit[index++] = *(((uint8_t *) Output_Out_LAN_block) + i);
    }

    {
      uint32_t index_tmp = 0;
      uint8_t data_tmp = 0;
      __control_info const *p_control_info = control_info;
      for (size_t i = 0; i < SIZE_CONTROL_INFO; ++i)
      {
        data_tmp |= ((*(p_control_info->p_ctrl) & MASKA_FOR_BIT(p_control_info->n_bit)) != 0) << index_tmp;
        if (++index_tmp == (sizeof(uint8_t) * 8))
        {
          sum += Canal1_MO_Transmit[index++] = data_tmp;
          index_tmp = 0;
          data_tmp = 0;
        }
        ++p_control_info;
      }
      if (index_tmp != 0)
        sum += Canal1_MO_Transmit[index++] = data_tmp;
    }
  }

  if ((index + 1 + sizeof(confirm_diagnostyka_mo)) < BUFFER_CANAL1_MO)
  {
    sum += Canal1_MO_Transmit[index++] = START_LABEL_CONF_DIAGN;
    uint8_t *point = (uint8_t *) (&confirm_diagnostyka_mo);
    for (uint32_t i = 0; i < sizeof(confirm_diagnostyka_mo); i++)
    {
      sum += Canal1_MO_Transmit[index++] = *(point++);
    }
    confirm_diagnostyka_mo = 0; //Очищаємо підтверджену діагностику каналу обміну епо Куналу 1
  }

  if ((index + 2) >= BUFFER_CANAL1_MO)
    total_error_sw_fixed();

  Canal1_MO_Transmit[index++] = sum;
  Canal1_MO_Transmit[index++] = STOP_BYTE_MO;

  //Скидаємо біт, що символізує, що опстанній байт переданий
  USART_ClearFlag(CANAL1_MO, USART_FLAG_TC);

  //Зупиняємо потік DMA якщо він запущений
  if ((DMA_StreamCANAL1_MO_Tx->CR & (uint32_t) DMA_SxCR_EN) != 0)
    DMA_StreamCANAL1_MO_Tx->CR &= ~(uint32_t) DMA_SxCR_EN;
  DMA_StreamCANAL1_MO_Tx->NDTR = index;
  //Дозволяємо передачу через DMA
  if ((CANAL1_MO->CR3 & USART_DMAReq_Tx) == 0)
    CANAL1_MO->CR3 |= USART_DMAReq_Tx;

  //Очищаємо прапореці, що сигналізує про завершення передачі даних для DMA1 по потоку CANAL1_MO_TX
  DMA_ClearFlag(DMA_StreamCANAL1_MO_Tx, DMA_FLAG_TCCANAL1_MO_Tx | DMA_FLAG_HTCANAL1_MO_Tx | DMA_FLAG_TEICANAL1_MO_Tx | DMA_FLAG_DMEICANAL1_MO_Tx | DMA_FLAG_FEICANAL1_MO_Tx);
  //Запускаємо передачу
  DMA_StreamCANAL1_MO_Tx->CR |= (uint32_t)(DMA_SxCR_EN /* | DMA_IT_TC*/);

  //  for (size_t i = 0; i < index; i++)
  //  {
  //    test_Canal1_MO_Transmit[test][i] = Canal1_MO_Transmit[i];
  //  }
  //  test_index[test] = index;
  //  if (++test >= 5) test = 0;
}
/***********************************************************************************/

/***********************************************************************************
Модуль прийому-передачі даних через інтерфейс CANAL2_MO
***********************************************************************************/
void CANAL2_MO_routine()
{
  typedef enum _CANAL2_MO_states
  {
    CANAL2_MO_FREE = 0,
    CANAL2_MO_SENDING,
    CANAL2_MO_RECEIVING,
    CANAL2_MO_RECEIVED,
    CANAL2_MO_ERROR,
    CANAL2_MO_BREAK_LAST_ACTION
  } __CANAL2_MO_states;

  SRAM1 static unsigned int time_mb_req;

  static unsigned int Modbus_TCP_REQ;
  static unsigned int Modbus_TCP_RESP;
  static unsigned int Modbus_TCP_RESP_REP;
  static unsigned int Modbus_TCP_RESP_CONFIRM;

  static __CANAL2_MO_states CANAL2_MO_state;

  queue_mo |= (queue_mo_irq & STATE_QUEUE_MO_RESTART_KP);
  queue_mo &= (uint32_t)(~QUEUQ_MO_IRQ);
  if (!_GET_STATE(queue_mo, STATE_QUEUE_MO_RESTART_KP))
  {
    //Немає активної команди перезапустити КП
    queue_mo |= queue_mo_irq;
  }
  else
  {
    if (
      (queue_mo == MASKA_FOR_BIT(STATE_QUEUE_MO_RESTART_KP)) &&
      (CANAL2_MO_state == CANAL2_MO_FREE) &&
      (_CHECK_SET_BIT(diagnostyka, WARNING_REPROGRAM) == 0) &&
      (_CHECK_SET_BIT(set_diagnostyka, WARNING_REPROGRAM) == 0))
    {
      _SET_BIT(set_diagnostyka, EVENT_RESTART_CB_BIT);
      restart_KP_irq = 5;
    }
  }

  static uint32_t tick;
  static uint32_t rx_ndtr;
  static uint32_t packet_number;

  uint8_t sum = 0;
  uint32_t index_w = 0;

  static uint32_t delta_max;

  if (
    (CANAL2_MO_state == CANAL2_MO_BREAK_LAST_ACTION) &&
    (_CHECK_SET_BIT(diagnostyka, WARNING_REPROGRAM) == 0) &&
    (_CHECK_SET_BIT(set_diagnostyka, WARNING_REPROGRAM) == 0))
  {
    Canal2_MO_Transmit[index_w++] = START_BYTE_MO;
    sum += Canal2_MO_Transmit[index_w++] = IEC_board_address;
    sum += Canal2_MO_Transmit[index_w++] = my_address_mo;

    sum += Canal2_MO_Transmit[index_w++] = BREAK_LAST_ACTION;

    _SET_STATE(queue_mo, STATE_QUEUE_MO_BREAK_LAST_ACTION_IN_IEC);
  }
  else if (CANAL2_MO_state == CANAL2_MO_FREE)
  {
    //На даний момент не іде передавання даних по Каналу 2
    if (!_GET_STATE(queue_mo, STATE_QUEUE_MO_TRANSACTION_PROGRESS_IN_IEC))
    {
      if (
        (Canal1 == true) &&
        (_CHECK_SET_BIT(diagnostyka, WARNING_REPROGRAM) == 0) &&
        (_CHECK_SET_BIT(set_diagnostyka, WARNING_REPROGRAM) == 0))
      {
        if (_GET_STATE(queue_mo, STATE_QUEUE_MO_ASK_BASIC_SETTINGS))
        {
          Canal2_MO_Transmit[index_w++] = START_BYTE_MO;
          sum += Canal2_MO_Transmit[index_w++] = IEC_board_address;
          sum += Canal2_MO_Transmit[index_w++] = my_address_mo;

          sum += Canal2_MO_Transmit[index_w++] = SENDING_BASIC_SETTINGS;

          sum += Canal2_MO_Transmit[index_w++] = VERSIA_PZ;
          sum += Canal2_MO_Transmit[index_w++] = MODYFIKACIA_VERSII_PZ;
          sum += Canal2_MO_Transmit[index_w++] = ZBIRKA_VERSII_PZ;
          sum += Canal2_MO_Transmit[index_w++] = ZBIRKA_PIDVERSII_PZ;

          sum += Canal2_MO_Transmit[index_w++] = serial_number_dev & 0xff;
          sum += Canal2_MO_Transmit[index_w++] = (serial_number_dev >> 8) & 0xff;

          sum += Canal2_MO_Transmit[index_w++] = current_settings.IP4[0] & 0xff;
          sum += Canal2_MO_Transmit[index_w++] = current_settings.IP4[1] & 0xff;
          sum += Canal2_MO_Transmit[index_w++] = current_settings.IP4[2] & 0xff;
          sum += Canal2_MO_Transmit[index_w++] = current_settings.IP4[3] & 0xff;

          sum += Canal2_MO_Transmit[index_w++] = current_settings.mask;

          sum += Canal2_MO_Transmit[index_w++] = current_settings.gateway[0] & 0xff;
          sum += Canal2_MO_Transmit[index_w++] = current_settings.gateway[1] & 0xff;
          sum += Canal2_MO_Transmit[index_w++] = current_settings.gateway[2] & 0xff;
          sum += Canal2_MO_Transmit[index_w++] = current_settings.gateway[3] & 0xff;

          sum += Canal2_MO_Transmit[index_w++] = current_settings.IP_time_server[0] & 0xff;
          sum += Canal2_MO_Transmit[index_w++] = current_settings.IP_time_server[1] & 0xff;
          sum += Canal2_MO_Transmit[index_w++] = current_settings.IP_time_server[2] & 0xff;
          sum += Canal2_MO_Transmit[index_w++] = current_settings.IP_time_server[3] & 0xff;

          unsigned int port_time_server = current_settings.port_time_server;
          sum += Canal2_MO_Transmit[index_w++] = port_time_server & 0xff;
          sum += Canal2_MO_Transmit[index_w++] = (port_time_server >> 8) & 0xff;

          unsigned int period_sync = current_settings.period_sync;
          sum += Canal2_MO_Transmit[index_w++] = period_sync & 0xff;
          sum += Canal2_MO_Transmit[index_w++] = (period_sync >> 8) & 0xff;
          sum += Canal2_MO_Transmit[index_w++] = (period_sync >> 16) & 0xff;
          sum += Canal2_MO_Transmit[index_w++] = (period_sync >> 24) & 0xff;

          unsigned int period = TIM2_CCR1_VAL * 60 /*Prescaler*/ / 60 /*вхідна частота таймера у МГц*/; /*результат у мкс*/
          sum += Canal2_MO_Transmit[index_w++] = period & 0xff;
          sum += Canal2_MO_Transmit[index_w++] = (period >> 8) & 0xff;

          _SET_STATE(queue_mo, STATE_QUEUE_MO_TRANSMITING_BASIC_SETTINGS);
        }
        else if (_GET_STATE(queue_mo, STATE_QUEUE_MO_ASK_MAKING_MEMORY_BLOCK))
        {

          Canal2_MO_Transmit[index_w++] = START_BYTE_MO;
          sum += Canal2_MO_Transmit[index_w++] = IEC_board_address;
          sum += Canal2_MO_Transmit[index_w++] = my_address_mo;

          sum += Canal2_MO_Transmit[index_w++] = MAKING_NEW_BLOCKS;
          sum += Canal2_MO_Transmit[index_w++] = 1; /*LSB: загальна кількість блоків, які треба створити*/
          sum += Canal2_MO_Transmit[index_w++] = 0; /*MSB: загальна кількість блоків, які треба створити*/

          packet_number = 0;                                                  /*номер пакету*/
          sum += Canal2_MO_Transmit[index_w++] = packet_number & 0xff;        /*LSB: номер пакету*/
          sum += Canal2_MO_Transmit[index_w++] = (packet_number >> 8) & 0xff; /*MSB: номер пакету*/

          sum += Canal2_MO_Transmit[index_w++] = START_LABEL_NEW_BLOCK; /*признак початку інформації по створюваному блоку*/
          sum += Canal2_MO_Transmit[index_w++] = 0;                     /*ID створюваного блоку*/

          sum += Canal2_MO_Transmit[index_w++] = 0; /*Тип логічного вузла для немодульного приладу*/
          sum += Canal2_MO_Transmit[index_w++] = 1; /*Кількість блоків*/

          sum += Canal2_MO_Transmit[index_w++] = (SIZE_SENDING_DATA_TM + SIZE_BLOCK_CONTROL_INFO) & 0xff;        /*LSB: Розмір буферу на прийом оперативних даних у КП з ПЛ*/
          sum += Canal2_MO_Transmit[index_w++] = ((SIZE_SENDING_DATA_TM + SIZE_BLOCK_CONTROL_INFO) >> 8) & 0xff; /*MSB: Розмір буферу на прийом оперативних даних у КП з ПЛ*/

          sum += Canal2_MO_Transmit[index_w++] = SIZE_RECEIVING_DATA_TM & 0xff;        /*LSB: Розмір буферу на передачу оперативних даних з КП у ПЛ*/
          sum += Canal2_MO_Transmit[index_w++] = (SIZE_RECEIVING_DATA_TM >> 8) & 0xff; /*MSB: Розмір буферу на передачу оперативних даних з КП у ПЛ*/

          sum += Canal2_MO_Transmit[index_w++] = SIZE_RECEIVING_ACT_DATA_TM & 0xff;        /*LSB: Розмір буферу на передачу признаків активації оперативних даних з КП у ПЛ*/
          sum += Canal2_MO_Transmit[index_w++] = (SIZE_RECEIVING_ACT_DATA_TM >> 8) & 0xff; /*MSB: Розмір буферу на передачу признаків активації оперативних даних з КП у ПЛ*/

          sum += Canal2_MO_Transmit[index_w++] = SIZE_DESCRIPTION_DATA_TM & 0xff;         /*LSW->LSB: Розмір буферу описової частини оперативних даних*/
          sum += Canal2_MO_Transmit[index_w++] = (SIZE_DESCRIPTION_DATA_TM >> 8) & 0xff;  /*LSW->MSB: Розмір буферу описової частини оперативних даних*/
          sum += Canal2_MO_Transmit[index_w++] = (SIZE_DESCRIPTION_DATA_TM >> 16) & 0xff; /*MSW->LSB: Розмір буферу описової частини оперативних даних*/
          sum += Canal2_MO_Transmit[index_w++] = (SIZE_DESCRIPTION_DATA_TM >> 24) & 0xff; /*MSW->MSB: Розмір буферу описової частини оперативних даних*/

          sum += Canal2_MO_Transmit[index_w++] = false; /*Не буде продовження передавання взагальному*/

          _SET_STATE(queue_mo, STATE_QUEUE_MO_TRANSMITING_MAKING_MEMORY_BLOCK);
        }
        else if (_GET_STATE(queue_mo, STATE_QUEUE_MO_ASK_SENDING_SETTING_NETWORK_LAYER))
        {
          Canal2_MO_Transmit[index_w++] = START_BYTE_MO;
          sum += Canal2_MO_Transmit[index_w++] = IEC_board_address;
          sum += Canal2_MO_Transmit[index_w++] = my_address_mo;

          sum += Canal2_MO_Transmit[index_w++] = REQUEST_SETTINGS_NETWORK_LAYER;

          _SET_STATE(queue_mo, STATE_QUEUE_MO_RECEIVING_SETTING_NETWORK_LAYER);
        }
        else if (_GET_STATE(queue_mo, STATE_QUEUE_MO_ASK_NEW_MODBUS_TCP_REQ))
        {
          Canal2_MO_Transmit[index_w++] = START_BYTE_MO;
          sum += Canal2_MO_Transmit[index_w++] = IEC_board_address;
          sum += Canal2_MO_Transmit[index_w++] = my_address_mo;

          sum += Canal2_MO_Transmit[index_w++] = REQUEST_MODBUS_TCP_REQ;

          _SET_STATE(queue_mo, STATE_QUEUE_MO_RECEIVING_NEW_MODBUS_TCP_REQ);
        }
        else if (_GET_STATE(queue_mo, STATE_QUEUE_MO_SEND_MODBUS_TCP_RESP))
        {
          _CLEAR_STATE(queue_mo, STATE_QUEUE_MO_SEND_MODBUS_TCP_RESP);

          if ((LAN_transmiting_count > 0) && (LAN_transmiting_count < (BUFFER_LAN - 2)))
          {
            //Копіюємо час підготовки відповіді
            unsigned int time_mb_resp = TIM2->CNT;
            uint64_t delta_time;

            //Вираховуємо час, протягом якого підготовлювалася відповідь
            if (
              (time_mb_resp > time_mb_req) ||
              ((time_mb_resp == time_mb_req) &&
               (mark_current_tick_LAN != 0)))
              delta_time = time_mb_resp - time_mb_req;
            else
              delta_time = time_mb_resp + 0x100000000ull - time_mb_req;

            static uint64_t max_period_tcp = 0;
            static uint64_t min_period_tcp = 0x100000000ull;
            static uint32_t reset_period_tcp;
            if (reset_period_tcp != 0)
            {
              max_period_tcp = 0;
              min_period_tcp = 0x100000000ull;

              reset_period_tcp = 0;
            }
            if (max_period_tcp < delta_time)
              max_period_tcp = delta_time;
            if (min_period_tcp > delta_time)
              min_period_tcp = delta_time;

            if (delta_time < MAX_TIMEOUT_PACKET_LAN)
            {
              unsigned int length = LAN_transmiting_count;

              Canal2_MO_Transmit[index_w++] = START_BYTE_MO;
              sum += Canal2_MO_Transmit[index_w++] = IEC_board_address;
              sum += Canal2_MO_Transmit[index_w++] = my_address_mo;

              sum += Canal2_MO_Transmit[index_w++] = SENDIND_MODBUS_TCP_RESP;

              sum += Canal2_MO_Transmit[index_w++] = length;
              for (size_t i = 0; i < length; i++)
                sum += Canal2_MO_Transmit[index_w++] = LAN_transmiting[i];

              _SET_STATE(queue_mo, STATE_QUEUE_MO_TRANSMITING_MODBUS_TCP_RESP);

              ++Modbus_TCP_RESP;
            }
          }
        }
        else if (_GET_STATE(queue_mo, STATE_QUEUE_MO_READ_FW_VERSION))
        {
          _CLEAR_STATE(queue_mo, STATE_QUEUE_MO_READ_FW_VERSION);

          Canal2_MO_Transmit[index_w++] = START_BYTE_MO;
          sum += Canal2_MO_Transmit[index_w++] = IEC_board_address;
          sum += Canal2_MO_Transmit[index_w++] = my_address_mo;

          sum += Canal2_MO_Transmit[index_w++] = REQUEST_FW_VERSION;

          _SET_STATE(queue_mo, STATE_QUEUE_MO_READING_FW_VERSION);
        }
      }
    }
    else
    {
      uint32_t const tick_tmp = TIM2->CNT;
      uint32_t const delta = (tick_tmp >= tick) ? (tick_tmp - tick) : (0x100000000ull - (uint64_t) tick + (uint64_t) tick_tmp);

      if (delta > 1000000)
        CANAL2_MO_state = CANAL2_MO_BREAK_LAST_ACTION;

      if (delta_max < delta)
        delta_max = delta;
    }
  }
  else if (CANAL2_MO_state == CANAL2_MO_SENDING)
  {
    if (DMA_StreamCANAL2_MO_Tx->NDTR == 0)
    {
      CANAL2_MO_state = CANAL2_MO_RECEIVING;
      tick = TIM2->CNT;
      rx_ndtr = DMA_StreamCANAL2_MO_Rx->NDTR;
    }
  }
  else if (CANAL2_MO_state == CANAL2_MO_RECEIVING)
  {
    uint32_t rx_ndtr_tmp = DMA_StreamCANAL2_MO_Rx->NDTR;
    if (rx_ndtr_tmp != rx_ndtr)
    {
      rx_ndtr = rx_ndtr_tmp;
    }
    else
    {
      uint32_t const tick_tmp = TIM2->CNT;
      uint32_t const delta = (tick_tmp >= tick) ? (tick_tmp - tick) : (0x100000000ull - (uint64_t) tick + (uint64_t) tick_tmp);

      if (rx_ndtr == BUFFER_CANAL2_MO)
      {
        //Не прийнято жодного байту
        if (delta > 1000000)
          CANAL2_MO_state = CANAL2_MO_ERROR;

        if (delta_max < delta)
          delta_max = delta;
      }
      else
      {
        //Є прийняті байти
        if (delta > 30)
          CANAL2_MO_state = CANAL2_MO_RECEIVED;
      }

      //Робимо перевірку пакету
      if (
        (CANAL2_MO_state == CANAL2_MO_RECEIVED) ||
        (CANAL2_MO_state == CANAL2_MO_ERROR))
      {
        //Попередньо скидаємо повідомплення про помилки прийому Каналу 2, які потім будемо виставляти
        clear_diagnostyka[0] |= WORD_0_MASKA_RECEIVING_ERRORS_CANAL_2;
        clear_diagnostyka[1] |= WORD_1_MASKA_RECEIVING_ERRORS_CANAL_2;
        clear_diagnostyka[2] |= WORD_2_MASKA_RECEIVING_ERRORS_CANAL_2;
        clear_diagnostyka[3] |= WORD_3_MASKA_RECEIVING_ERRORS_CANAL_2;
        clear_diagnostyka[4] |= WORD_4_MASKA_RECEIVING_ERRORS_CANAL_2;

        int32_t size_packet = BUFFER_CANAL2_MO - rx_ndtr;
        static unsigned int lock_error_no_answer;
        if (size_packet != 0)
        {
          lock_error_no_answer = 0;
          uint32_t error_status = CANAL2_MO->SR & (USART_FLAG_ORE | USART_FLAG_NE | USART_FLAG_FE | USART_FLAG_PE);

          //Прийняті дані з комунікаційної плати по каналу 1
          static unsigned int lock_error_receiving;
          if (
            (error_status == 0) &&
            (size_packet >= 3) &&
            (Canal2_MO_Received[0] == START_BYTE_MO) &&
            (Canal2_MO_Received[size_packet - 1] == STOP_BYTE_MO))
          {
            lock_error_receiving = 0;
            if (Canal2_MO_Received[1] == my_address_mo)
            {
              //Перевіряємо контрольну суму
              sum = 0;
              for (int32_t i = 0; i < (size_packet - 3); i++)
                sum += Canal2_MO_Received[1 + i];

              static unsigned int lock_error_received_packet;
              if (sum == Canal2_MO_Received[size_packet - 2])
              {
                lock_error_received_packet = 0;
                if (
                  !(
                    ((_GET_STATE(queue_mo, STATE_QUEUE_MO_BREAK_LAST_ACTION_IN_IEC)) && (Canal2_MO_Received[3] == CONFIRM_BREAKING_LAST_ACTION)) ||
                    ((_GET_STATE(queue_mo, STATE_QUEUE_MO_TRANSMITING_MAKING_MEMORY_BLOCK)) && (Canal2_MO_Received[3] == ANSWER_FOR_MAKING_NEW_BLOCKS)) ||
                    ((_GET_STATE(queue_mo, STATE_QUEUE_MO_TRANSMITING_BASIC_SETTINGS)) && (Canal2_MO_Received[3] == ANY_CONFIRMATION)) ||
                    ((_GET_STATE(queue_mo, STATE_QUEUE_MO_RECEIVING_SETTING_NETWORK_LAYER)) && ((Canal2_MO_Received[3] == SENDING_SETTINGS_NETWORK_LAYER) ||
                                                                                                (Canal2_MO_Received[3] == ANY_CONFIRMATION))) ||
                    ((_GET_STATE(queue_mo, STATE_QUEUE_MO_RECEIVING_NEW_MODBUS_TCP_REQ)) && ((Canal2_MO_Received[3] == SENDING_MODBUS_TCP_REQ) ||
                                                                                             (Canal2_MO_Received[3] == ANY_CONFIRMATION))) ||
                    ((_GET_STATE(queue_mo, STATE_QUEUE_MO_TRANSMITING_MODBUS_TCP_RESP)) && (Canal2_MO_Received[3] == ANY_CONFIRMATION)) ||
                    ((_GET_STATE(queue_mo, STATE_QUEUE_MO_READING_FW_VERSION)) && (Canal2_MO_Received[3] == SEND_FIRMWARE_VERSION))))
                {
                  _SET_BIT(set_diagnostyka, ERROR_CPU_ANSWER_CANAL_2);
                  CANAL2_MO_state = CANAL2_MO_ERROR;
                }
              }
              else
              {
                if (++lock_error_received_packet >= COARSENING)
                {
                  _SET_BIT(set_diagnostyka, ERROR_CPU_RECEIVED_PACKET_CANAL_2);
                  lock_error_received_packet &= ~(1u << 31); // щоб не винекла ситуація переходу з максимального числа до нуля
                }
              }
            }
          }
          else
          {
            if (++lock_error_receiving >= COARSENING)
            {
              _SET_BIT(set_diagnostyka, ERROR_CPU_RECEIVING_CANAL_2);
              lock_error_receiving &= ~(1u << 31); // щоб не винекла ситуація переходу з максимального числа до нуля
            }
            CANAL2_MO_state = CANAL2_MO_ERROR;
          }
        }
        else
        {
          if (++lock_error_no_answer >= COARSENING)
          {
            _SET_BIT(set_diagnostyka, ERROR_CPU_NO_ANSWER_CANAL_2);
            lock_error_no_answer &= ~(1u << 31); // щоб не винекла ситуація переходу з максимального числа до нуля
          }
          CANAL2_MO_state = CANAL2_MO_ERROR;
        }
      }
    }
  }

  if (
    (CANAL2_MO_state == CANAL2_MO_RECEIVED) ||
    (CANAL2_MO_state == CANAL2_MO_ERROR))
  {
    if (CANAL2_MO_state == CANAL2_MO_RECEIVED)
    {
      sum = 0;

      Canal2_MO_Transmit[index_w++] = START_BYTE_MO;
      sum += Canal2_MO_Transmit[index_w++] = IEC_board_address;
      sum += Canal2_MO_Transmit[index_w++] = my_address_mo;

      if (_GET_STATE(queue_mo, STATE_QUEUE_MO_BREAK_LAST_ACTION_IN_IEC))
      {
        index_w = 0;
        _CLEAR_STATE(queue_mo, STATE_QUEUE_MO_BREAK_LAST_ACTION_IN_IEC);

        CANAL2_MO_state = CANAL2_MO_FREE;
        Canal2 = true;
      }
      else if (
        (_GET_STATE(queue_mo, STATE_QUEUE_MO_TRANSMITING_BASIC_SETTINGS)) ||
        (_GET_STATE(queue_mo, STATE_QUEUE_MO_TRANSMITING_MODBUS_TCP_RESP)))
      {
        index_w = 0;
        if (_GET_STATE(queue_mo, STATE_QUEUE_MO_TRANSMITING_BASIC_SETTINGS))
        {
          _CLEAR_STATE(queue_mo, STATE_QUEUE_MO_TRANSMITING_BASIC_SETTINGS);
          _SET_STATE(queue_mo, STATE_QUEUE_MO_READ_FW_VERSION);
        }
        else
        {
          LAN_transmiting_count = 0; //Це є ознакою того, що пакет-відповідь успішно передано
          _CLEAR_STATE(queue_mo, STATE_QUEUE_MO_TRANSMITING_MODBUS_TCP_RESP);

          ++Modbus_TCP_RESP_CONFIRM;

          unsigned int time_mb_resp2 = TIM2->CNT;
          uint64_t delta_time;

          //Вираховуємо час, протягом якого підготовлювалася відповідь з підтвердженням
          if (
            (time_mb_resp2 > time_mb_req) ||
            ((time_mb_resp2 == time_mb_req) &&
             (mark_current_tick_LAN != 0)))
            delta_time = time_mb_resp2 - time_mb_req;
          else
            delta_time = time_mb_resp2 + 0x100000000ull - time_mb_req;

          static uint64_t max_period_tcp2 = 0;
          static uint64_t min_period_tcp2 = 0x100000000ull;
          static uint32_t reset_period_tcp2;
          if (reset_period_tcp2 != 0)
          {
            max_period_tcp2 = 0;
            min_period_tcp2 = 0x100000000ull;

            reset_period_tcp2 = 0;
          }
          if (max_period_tcp2 < delta_time)
            max_period_tcp2 = delta_time;
          if (min_period_tcp2 > delta_time)
            min_period_tcp2 = delta_time;
        }

        if (Canal2_MO_Received[4] == true)
        {
          CANAL2_MO_state = CANAL2_MO_FREE;
          Canal2 = true;
        }
        else
        {
          CANAL2_MO_state = CANAL2_MO_ERROR;
        }
      }
      else if (_GET_STATE(queue_mo, STATE_QUEUE_MO_TRANSMITING_MAKING_MEMORY_BLOCK))
      {
        index_w = 0;
        state_array_control_state = Canal2_MO_Received[4] | (Canal2_MO_Received[5] << 8);
        _CLEAR_STATE(queue_mo, STATE_QUEUE_MO_TRANSMITING_MAKING_MEMORY_BLOCK);

        if (
          ((state_array_control_state & (MASKA_FOR_BIT(ACS_MADE) | MASKA_FOR_BIT(ACS_ALL_BLOCKS_CREATED))) == (MASKA_FOR_BIT(ACS_MADE) | MASKA_FOR_BIT(ACS_ALL_BLOCKS_CREATED))) &&
          ((Canal2_MO_Received[6] | (Canal2_MO_Received[7] << 8)) == 0))
        {
          CANAL2_MO_state = CANAL2_MO_FREE;
          Canal2 = true;
        }
        else
        {
          CANAL2_MO_state = CANAL2_MO_ERROR;
        }
      }
      else if (_GET_STATE(queue_mo, STATE_QUEUE_MO_RECEIVING_SETTING_NETWORK_LAYER))
      {
        if (Canal2_MO_Received[3] == SENDING_SETTINGS_NETWORK_LAYER)
        {
          //Прийняти налаштування мережевого рівня Ethernet
          uint16_t IPv4_tmp[4] = {Canal2_MO_Received[4], Canal2_MO_Received[5], Canal2_MO_Received[6], Canal2_MO_Received[7]};
          uint32_t mask_tmp = Canal2_MO_Received[8];
          uint16_t gateway_tmp[4] = {Canal2_MO_Received[9], Canal2_MO_Received[10], Canal2_MO_Received[11], Canal2_MO_Received[12]};
          if (
            (current_settings.IP4[0] != IPv4_tmp[0]) ||
            (current_settings.IP4[1] != IPv4_tmp[1]) ||
            (current_settings.IP4[2] != IPv4_tmp[2]) ||
            (current_settings.IP4[3] != IPv4_tmp[3]) ||
            (current_settings.mask != mask_tmp) ||
            (current_settings.gateway[0] != gateway_tmp[0]) ||
            (current_settings.gateway[1] != gateway_tmp[1]) ||
            (current_settings.gateway[2] != gateway_tmp[2]) ||
            (current_settings.gateway[3] != gateway_tmp[3]))
          {
            //Помічаємо, що поле структури зараз буде змінене
            changed_settings = CHANGED_ETAP_EXECUTION;

            current_settings.IP4[0] = IPv4_tmp[0];
            current_settings.IP4[1] = IPv4_tmp[1];
            current_settings.IP4[2] = IPv4_tmp[2];
            current_settings.IP4[3] = IPv4_tmp[3];
            current_settings.mask = mask_tmp;
            current_settings.gateway[0] = gateway_tmp[0];
            current_settings.gateway[1] = gateway_tmp[1];
            current_settings.gateway[2] = gateway_tmp[2];
            current_settings.gateway[3] = gateway_tmp[3];

            //Формуємо запис у таблиці настройок про зміну конфігурації і ініціюємо запис у EEPROM нових настройок
            fix_change_settings(0, 4);

            //Виставляємо признак, що на екрані треба обновити інформацію
            new_state_keyboard |= (1 << BIT_REWRITE);
          }

          //Відправити підтвердження прийняття налаштувань мережевого рівня Ethernet
          sum += Canal2_MO_Transmit[index_w++] = CONFIRM_RECEIVING_SETTINGS_NETWORK_LAYER;
          sum += Canal2_MO_Transmit[index_w++] = true;
        }
        else
        {
          index_w = 0;
          _CLEAR_STATE(queue_mo, STATE_QUEUE_MO_RECEIVING_SETTING_NETWORK_LAYER);

          if (
            (Canal2_MO_Received[3] == ANY_CONFIRMATION) &&
            (Canal2_MO_Received[4] == true))
          {
            CANAL2_MO_state = CANAL2_MO_FREE;
            Canal2 = true;
          }
          else
          {
            CANAL2_MO_state = CANAL2_MO_ERROR;
          }
        }
      }
      else if (_GET_STATE(queue_mo, STATE_QUEUE_MO_RECEIVING_NEW_MODBUS_TCP_REQ))
      {
        if (Canal2_MO_Received[3] == SENDING_MODBUS_TCP_REQ)
        {
          //Прийняти пакет-запит Modbus-TCP з КП
          LAN_received_count = Canal2_MO_Received[4];
          unsigned int answer;
          if ((LAN_received_count > 0) && (LAN_received_count < BUFFER_LAN))
          {
            for (intptr_t i = 0; i < LAN_received_count; i++)
              LAN_received[i] = Canal2_MO_Received[5 + i];
            answer = true;

            //Копіюємо час постановки часу на обробку
            time_mb_req = TIM2->CNT;
            mark_current_tick_LAN = 0xff;

            ++Modbus_TCP_REQ;
          }
          else
          {
            LAN_received_count = 0;
            answer = false;
          }

          //Відправити підтвердження прийняття налаштувань мережевого рівня Ethernet
          sum += Canal2_MO_Transmit[index_w++] = CONFIRM_RECEIVING_MODBUS_TCP_REQ;
          sum += Canal2_MO_Transmit[index_w++] = answer;
        }
        else
        {
          index_w = 0;
          _CLEAR_STATE(queue_mo, STATE_QUEUE_MO_RECEIVING_NEW_MODBUS_TCP_REQ);

          if (
            (Canal2_MO_Received[3] == ANY_CONFIRMATION) &&
            (Canal2_MO_Received[4] == true))
          {
            CANAL2_MO_state = CANAL2_MO_FREE;
            Canal2 = true;
          }
          else
          {
            CANAL2_MO_state = CANAL2_MO_ERROR;
          }
        }
      }
      else if (_GET_STATE(queue_mo, STATE_QUEUE_MO_READING_FW_VERSION))
      {
        index_w = 0;
        _CLEAR_STATE(queue_mo, STATE_QUEUE_MO_READING_FW_VERSION);
        for (size_t i = 0; i < 4; ++i)
          fwKP[i] = Canal2_MO_Received[4 + i];
        for (size_t i = 0; i < 6; ++i)
          fwDTKP[i] = Canal2_MO_Received[14 + i];

        CANAL2_MO_state = CANAL2_MO_FREE;
        Canal2 = true;
      }
    }
    else if (CANAL2_MO_state == CANAL2_MO_ERROR)
    {
      CANAL2_MO_state = CANAL2_MO_BREAK_LAST_ACTION;
      if (_GET_STATE(queue_mo, STATE_QUEUE_MO_TRANSMITING_MODBUS_TCP_RESP))
      {
        _CLEAR_STATE(queue_mo, STATE_QUEUE_MO_TRANSMITING_MODBUS_TCP_RESP);
        _SET_STATE(queue_mo, STATE_QUEUE_MO_SEND_MODBUS_TCP_RESP);

        ++Modbus_TCP_RESP_REP;
      }
    }

    tick = TIM2->CNT; /*стан лічильника коли буда завершена остання трасакція повністю з помилкою або без неї*/
  }

  if (index_w != 0)
  {
    if (
      (_CHECK_SET_BIT(diagnostyka, WARNING_REPROGRAM) == 0) &&
      (_CHECK_SET_BIT(set_diagnostyka, WARNING_REPROGRAM) == 0))
    {
      CANAL2_MO_state = CANAL2_MO_SENDING;

      //Додаємомконтрольну суму і мітку зафершенняпакету
      Canal2_MO_Transmit[index_w++] = sum;
      Canal2_MO_Transmit[index_w++] = STOP_BYTE_MO;

      /*
      Підготовляємо канал до прийняття даних
      */
      //Зупиняємо канал приймання
      if ((DMA_StreamCANAL2_MO_Rx->CR & (uint32_t) DMA_SxCR_EN) != 0)
        DMA_StreamCANAL2_MO_Rx->CR &= ~(uint32_t) DMA_SxCR_EN;

      //Скидуємо всі можливі помилки
      CANAL2_MO->SR;
      dr = (unsigned short int) (CANAL2_MO->DR & (uint16_t) 0x01FF);
      CANAL2_MO->SR = (uint16_t)(~(uint32_t) USART_FLAG_LBD);

      // Очищаємо прапореці, що сигналізує про завершення передачі даних для DMA
      DMA_ClearFlag(DMA_StreamCANAL2_MO_Rx, DMA_FLAG_TCCANAL2_MO_Rx | DMA_FLAG_HTCANAL2_MO_Rx | DMA_FLAG_TEICANAL2_MO_Rx | DMA_FLAG_DMEICANAL2_MO_Rx | DMA_FLAG_FEICANAL2_MO_Rx);

      DMA_StreamCANAL2_MO_Rx->NDTR = BUFFER_CANAL2_MO;
      //Запускаємо очікування прийому
      DMA_StreamCANAL2_MO_Rx->CR |= (uint32_t) DMA_SxCR_EN;
      /***/

      /*
      Починаємо відправляти дані
      */
      //Скидаємо біт, що символізує, що опстанній байт переданий
      USART_ClearFlag(CANAL2_MO, USART_FLAG_TC);

      //Зупиняємо потік DMA якщо він запущений
      if ((DMA_StreamCANAL2_MO_Tx->CR & (uint32_t) DMA_SxCR_EN) != 0)
        DMA_StreamCANAL2_MO_Tx->CR &= ~(uint32_t) DMA_SxCR_EN;
      DMA_StreamCANAL2_MO_Tx->NDTR = index_w;
      //Дозволяємо передачу через DMA
      if ((CANAL2_MO->CR3 & USART_DMAReq_Tx) == 0)
        USART2->CR3 |= USART_DMAReq_Tx;

      //Очищаємо прапореці, що сигналізує про завершення передачі даних для DMA1 по потоку CANAL1_MO_TX
      DMA_ClearFlag(DMA_StreamCANAL2_MO_Tx, DMA_FLAG_TCCANAL2_MO_Tx | DMA_FLAG_HTCANAL2_MO_Tx | DMA_FLAG_TEICANAL2_MO_Tx | DMA_FLAG_DMEICANAL2_MO_Tx | DMA_FLAG_FEICANAL2_MO_Tx);
      //Запускаємо передачу
      DMA_StreamCANAL2_MO_Tx->CR |= (uint32_t) DMA_SxCR_EN;
      /***/
    }
    else
    {
      //Резим перепрограмування - треба всі процеси припинити
      CANAL2_MO_state = CANAL2_MO_BREAK_LAST_ACTION;
      queue_mo = 0;
    }
  }
}
/***********************************************************************************/

/***********************************************************************************
Обробка низькопріотетних задач по міжпроцесорному обміні
***********************************************************************************/
void low_routine_for_KP(void)
{
  if (
    (restart_KP_irq == 0) &&
    (IEC_board_uncall == 0) &&
    (Canal2 == false))
  {
    CANAL2_MO_routine();
  }
  else if ((Canal1 == true) && (Canal2 == true))
    Canal2 = false;
  Canal1 = false;
}
/***********************************************************************************/
