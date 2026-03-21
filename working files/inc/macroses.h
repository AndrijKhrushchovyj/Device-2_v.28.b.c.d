#ifndef __MACROSES__
#define __MACROSES__

#define UNUSED(x) (void) (x)

#define MASKA_FOR_BIT(_n) (1u << (_n))

#define _CHECK_SET_BIT(_array, _number_bit) \
  (_array[_number_bit >> 5] & (1u << (_number_bit & 0x1f)))

#define _SET_BIT(_array, _number_bit) \
  _array[_number_bit >> 5] |= (1u << (_number_bit & 0x1f))

#define _CLEAR_BIT(_array, _number_bit) \
  _array[_number_bit >> 5] &= (~(1u << (_number_bit & 0x1f)))

//#define _DEVICE_REGISTER(addr,offset)   *(unsigned short int *)(addr + (offset<<1) )
#define _DEVICE_REGISTER_V2(addr, offset) *(volatile uint16_t *) (addr + offset)

#define _NUMBER_CHANGES_INTO_UNSIGNED_INT_ARRAY(_array1, _array2, _number_items, _rezultat) \
  {                                                                                         \
    _rezultat = 0;                                                                          \
    for (unsigned int _i = 0; _i < _number_items; _i++)                                     \
    {                                                                                       \
      unsigned int _temp_value = _array1[_i] ^ _array2[_i];                                 \
      for (unsigned int _j = 0; _j < 32; _j++)                                              \
      {                                                                                     \
        if ((_temp_value & (1 << _j)) != 0)                                                 \
          _rezultat++;                                                                      \
      }                                                                                     \
    }                                                                                       \
  }

#define _CONVERT_SIGNAL_TO_GMM(__input_array, __output_array, __input_index_signal, __output_index_signal) \
  __output_array[__output_index_signal >> 4] |=                                                            \
    ((__input_array[__input_index_signal >> 5] >> (__input_index_signal & 0x1f)) & 0x1) << (__output_index_signal & 0xf)

#define _DELAY_ABOUT_40NS() \
  {                         \
    word_SRAM1 = 1;         \
    word_SRAM1 = 2;         \
    word_SRAM1 = 3;         \
  }

#define _DELAY_ABOUT_10NS() \
  {                         \
    word_SRAM1 = 1;         \
    word_SRAM1 = 2;         \
    word_SRAM1 = 3;         \
  }

//#define _DELAY_ABOUT_40NS()  \
//        {                    \
//          __no_operation();  \
//          __no_operation();  \
//          __no_operation();  \
//          __no_operation();  \
//          __no_operation();  \
//        }
//
//#define _DELAY_ABOUT_10NS()  \
//        {                    \
//          __no_operation();  \
//          __no_operation();  \
//        }

#define _CLEAR_STATE(_output, _output_bit) _output &= (~(1u << (_output_bit)))

#define _SET_STATE(_output, _output_bit) _output |= (1u << (_output_bit))

#define _GET_STATE(_input, _input_bit) ((_input >> (_input_bit)) & 0x1)

#define _INVERTOR(                                     \
  _input, _input_bit,                                  \
  _output, _output_bit)                                \
  {                                                    \
    if (_input & (unsigned int) (1 << _input_bit))     \
      _output &= (unsigned int) (~(1 << _output_bit)); \
    else                                               \
      _output |= (unsigned int) (1 << _output_bit);    \
  }

#define _AND2(                                           \
  _input_1, _input_bit_1,                                \
  _input_2, _input_bit_2,                                \
  _output, _output_bit)                                  \
  {                                                      \
    if (                                                 \
      (_input_1 & (unsigned int) (1 << _input_bit_1)) && \
      (_input_2 & (unsigned int) (1 << _input_bit_2)))   \
      _output |= (unsigned int) (1 << _output_bit);      \
    else                                                 \
      _output &= (unsigned int) (~(1 << _output_bit));   \
  }

#define _AND3(                                           \
  _input_1, _input_bit_1,                                \
  _input_2, _input_bit_2,                                \
  _input_3, _input_bit_3,                                \
  _output, _output_bit)                                  \
  {                                                      \
    if (                                                 \
      (_input_1 & (unsigned int) (1 << _input_bit_1)) && \
      (_input_2 & (unsigned int) (1 << _input_bit_2)) && \
      (_input_3 & (unsigned int) (1 << _input_bit_3)))   \
      _output |= (unsigned int) (1 << _output_bit);      \
    else                                                 \
      _output &= (unsigned int) (~(1 << _output_bit));   \
  }

#define _AND4(                                           \
  _input_1, _input_bit_1,                                \
  _input_2, _input_bit_2,                                \
  _input_3, _input_bit_3,                                \
  _input_4, _input_bit_4,                                \
  _output, _output_bit)                                  \
  {                                                      \
    if (                                                 \
      (_input_1 & (unsigned int) (1 << _input_bit_1)) && \
      (_input_2 & (unsigned int) (1 << _input_bit_2)) && \
      (_input_3 & (unsigned int) (1 << _input_bit_3)) && \
      (_input_4 & (unsigned int) (1 << _input_bit_4)))   \
      _output |= (unsigned int) (1 << _output_bit);      \
    else                                                 \
      _output &= (unsigned int) (~(1 << _output_bit));   \
  }

#define _AND5(                                           \
  _input_1, _input_bit_1,                                \
  _input_2, _input_bit_2,                                \
  _input_3, _input_bit_3,                                \
  _input_4, _input_bit_4,                                \
  _input_5, _input_bit_5,                                \
  _output, _output_bit)                                  \
  {                                                      \
    if (                                                 \
      (_input_1 & (unsigned int) (1 << _input_bit_1)) && \
      (_input_2 & (unsigned int) (1 << _input_bit_2)) && \
      (_input_3 & (unsigned int) (1 << _input_bit_3)) && \
      (_input_4 & (unsigned int) (1 << _input_bit_4)) && \
      (_input_5 & (unsigned int) (1 << _input_bit_5)))   \
      _output |= (unsigned int) (1 << _output_bit);      \
    else                                                 \
      _output &= (unsigned int) (~(1 << _output_bit));   \
  }

#define _AND6(                                           \
  _input_1, _input_bit_1,                                \
  _input_2, _input_bit_2,                                \
  _input_3, _input_bit_3,                                \
  _input_4, _input_bit_4,                                \
  _input_5, _input_bit_5,                                \
  _input_6, _input_bit_6,                                \
  _output, _output_bit)                                  \
  {                                                      \
    if (                                                 \
      (_input_1 & (unsigned int) (1 << _input_bit_1)) && \
      (_input_2 & (unsigned int) (1 << _input_bit_2)) && \
      (_input_3 & (unsigned int) (1 << _input_bit_3)) && \
      (_input_4 & (unsigned int) (1 << _input_bit_4)) && \
      (_input_5 & (unsigned int) (1 << _input_bit_5)) && \
      (_input_6 & (unsigned int) (1 << _input_bit_6)))   \
      _output |= (unsigned int) (1 << _output_bit);      \
    else                                                 \
      _output &= (unsigned int) (~(1 << _output_bit));   \
  }

#define _AND2_INVERTOR(                                  \
  _input_1, _input_bit_1,                                \
  _input_2, _input_bit_2,                                \
  _output, _output_bit)                                  \
  {                                                      \
    if (                                                 \
      (_input_1 & (unsigned int) (1 << _input_bit_1)) && \
      (_input_2 & (unsigned int) (1 << _input_bit_2)))   \
      _output &= (unsigned int) (~(1 << _output_bit));   \
    else                                                 \
      _output |= (unsigned int) (1 << _output_bit);      \
  }

#define _AND3_INVERTOR(                                  \
  _input_1, _input_bit_1,                                \
  _input_2, _input_bit_2,                                \
  _input_3, _input_bit_3,                                \
  _output, _output_bit)                                  \
  {                                                      \
    if (                                                 \
      (_input_1 & (unsigned int) (1 << _input_bit_1)) && \
      (_input_2 & (unsigned int) (1 << _input_bit_2)) && \
      (_input_3 & (unsigned int) (1 << _input_bit_3)))   \
      _output &= (unsigned int) (~(1 << _output_bit));   \
    else                                                 \
      _output |= (unsigned int) (1 << _output_bit);      \
  }

#define _OR2(                                            \
  _input_1, _input_bit_1,                                \
  _input_2, _input_bit_2,                                \
  _output, _output_bit)                                  \
  {                                                      \
    if (                                                 \
      (_input_1 & (unsigned int) (1 << _input_bit_1)) || \
      (_input_2 & (unsigned int) (1 << _input_bit_2)))   \
      _output |= (unsigned int) (1 << _output_bit);      \
    else                                                 \
      _output &= (unsigned int) (~(1 << _output_bit));   \
  }

#define _OR2_INVERTOR(                                   \
  _input_1, _input_bit_1,                                \
  _input_2, _input_bit_2,                                \
  _output, _output_bit)                                  \
  {                                                      \
    if (                                                 \
      (_input_1 & (unsigned int) (1 << _input_bit_1)) || \
      (_input_2 & (unsigned int) (1 << _input_bit_2)))   \
      _output &= (unsigned int) (~(1 << _output_bit));   \
    else                                                 \
      _output |= (unsigned int) (1 << _output_bit);      \
  }

#define _OR3(                                            \
  _input_1, _input_bit_1,                                \
  _input_2, _input_bit_2,                                \
  _input_3, _input_bit_3,                                \
  _output, _output_bit)                                  \
  {                                                      \
    if (                                                 \
      (_input_1 & (unsigned int) (1 << _input_bit_1)) || \
      (_input_2 & (unsigned int) (1 << _input_bit_2)) || \
      (_input_3 & (unsigned int) (1 << _input_bit_3)))   \
      _output |= (unsigned int) (1 << _output_bit);      \
    else                                                 \
      _output &= (unsigned int) (~(1 << _output_bit));   \
  }

#define _OR3_INVERTOR(                                   \
  _input_1, _input_bit_1,                                \
  _input_2, _input_bit_2,                                \
  _input_3, _input_bit_3,                                \
  _output, _output_bit)                                  \
  {                                                      \
    if (                                                 \
      (_input_1 & (unsigned int) (1 << _input_bit_1)) || \
      (_input_2 & (unsigned int) (1 << _input_bit_2)) || \
      (_input_3 & (unsigned int) (1 << _input_bit_3)))   \
      _output &= (unsigned int) (~(1 << _output_bit));   \
    else                                                 \
      _output |= (unsigned int) (1 << _output_bit);      \
  }

#define _OR4(                                            \
  _input_1, _input_bit_1,                                \
  _input_2, _input_bit_2,                                \
  _input_3, _input_bit_3,                                \
  _input_4, _input_bit_4,                                \
  _output, _output_bit)                                  \
  {                                                      \
    if (                                                 \
      (_input_1 & (unsigned int) (1 << _input_bit_1)) || \
      (_input_2 & (unsigned int) (1 << _input_bit_2)) || \
      (_input_3 & (unsigned int) (1 << _input_bit_3)) || \
      (_input_4 & (unsigned int) (1 << _input_bit_4)))   \
      _output |= (unsigned int) (1 << _output_bit);      \
    else                                                 \
      _output &= (unsigned int) (~(1 << _output_bit));   \
  }

#define _OR4_INVERTOR(                                   \
  _input_1, _input_bit_1,                                \
  _input_2, _input_bit_2,                                \
  _input_3, _input_bit_3,                                \
  _input_4, _input_bit_4,                                \
  _output, _output_bit)                                  \
  {                                                      \
    if (                                                 \
      (_input_1 & (unsigned int) (1 << _input_bit_1)) || \
      (_input_2 & (unsigned int) (1 << _input_bit_2)) || \
      (_input_3 & (unsigned int) (1 << _input_bit_3)) || \
      (_input_4 & (unsigned int) (1 << _input_bit_4)))   \
      _output &= (unsigned int) (~(1 << _output_bit));   \
    else                                                 \
      _output |= (unsigned int) (1 << _output_bit);      \
  }

#define _OR5(                                            \
  _input_1, _input_bit_1,                                \
  _input_2, _input_bit_2,                                \
  _input_3, _input_bit_3,                                \
  _input_4, _input_bit_4,                                \
  _input_5, _input_bit_5,                                \
  _output, _output_bit)                                  \
  {                                                      \
    if (                                                 \
      (_input_1 & (unsigned int) (1 << _input_bit_1)) || \
      (_input_2 & (unsigned int) (1 << _input_bit_2)) || \
      (_input_3 & (unsigned int) (1 << _input_bit_3)) || \
      (_input_4 & (unsigned int) (1 << _input_bit_4)) || \
      (_input_5 & (unsigned int) (1 << _input_bit_5)))   \
      _output |= (unsigned int) (1 << _output_bit);      \
    else                                                 \
      _output &= (unsigned int) (~(1 << _output_bit));   \
  }

#define _OR6(                                            \
  _input_1, _input_bit_1,                                \
  _input_2, _input_bit_2,                                \
  _input_3, _input_bit_3,                                \
  _input_4, _input_bit_4,                                \
  _input_5, _input_bit_5,                                \
  _input_6, _input_bit_6,                                \
  _output, _output_bit)                                  \
  {                                                      \
    if (                                                 \
      (_input_1 & (unsigned int) (1 << _input_bit_1)) || \
      (_input_2 & (unsigned int) (1 << _input_bit_2)) || \
      (_input_3 & (unsigned int) (1 << _input_bit_3)) || \
      (_input_4 & (unsigned int) (1 << _input_bit_4)) || \
      (_input_5 & (unsigned int) (1 << _input_bit_5)) || \
      (_input_6 & (unsigned int) (1 << _input_bit_6)))   \
      _output |= (unsigned int) (1 << _output_bit);      \
    else                                                 \
      _output &= (unsigned int) (~(1 << _output_bit));   \
  }

#define _OR6_INVERTOR(                                   \
  _input_1, _input_bit_1,                                \
  _input_2, _input_bit_2,                                \
  _input_3, _input_bit_3,                                \
  _input_4, _input_bit_4,                                \
  _input_5, _input_bit_5,                                \
  _input_6, _input_bit_6,                                \
  _output, _output_bit)                                  \
  {                                                      \
    if (                                                 \
      (_input_1 & (unsigned int) (1 << _input_bit_1)) || \
      (_input_2 & (unsigned int) (1 << _input_bit_2)) || \
      (_input_3 & (unsigned int) (1 << _input_bit_3)) || \
      (_input_4 & (unsigned int) (1 << _input_bit_4)) || \
      (_input_5 & (unsigned int) (1 << _input_bit_5)) || \
      (_input_6 & (unsigned int) (1 << _input_bit_6)))   \
      _output &= (unsigned int) (~(1 << _output_bit));   \
    else                                                 \
      _output |= (unsigned int) (1 << _output_bit);      \
  }

#define _OR8(                                            \
  _input_1, _input_bit_1,                                \
  _input_2, _input_bit_2,                                \
  _input_3, _input_bit_3,                                \
  _input_4, _input_bit_4,                                \
  _input_5, _input_bit_5,                                \
  _input_6, _input_bit_6,                                \
  _input_7, _input_bit_7,                                \
  _input_8, _input_bit_8,                                \
  _output, _output_bit)                                  \
  {                                                      \
    if (                                                 \
      (_input_1 & (unsigned int) (1 << _input_bit_1)) || \
      (_input_2 & (unsigned int) (1 << _input_bit_2)) || \
      (_input_3 & (unsigned int) (1 << _input_bit_3)) || \
      (_input_4 & (unsigned int) (1 << _input_bit_4)) || \
      (_input_5 & (unsigned int) (1 << _input_bit_5)) || \
      (_input_6 & (unsigned int) (1 << _input_bit_6)) || \
      (_input_7 & (unsigned int) (1 << _input_bit_7)) || \
      (_input_8 & (unsigned int) (1 << _input_bit_8)))   \
      _output |= (unsigned int) (1 << _output_bit);      \
    else                                                 \
      _output &= (unsigned int) (~(1 << _output_bit));   \
  }

#define _XOR_INVERTOR(                                           \
  _input_1, _input_bit_1,                                        \
  _input_2, _input_bit_2,                                        \
  _output, _output_bit)                                          \
  {                                                              \
    if (                                                         \
      ((_input_1 & ((unsigned int) (1 << _input_bit_1))) != 0) ^ \
      ((_input_2 & ((unsigned int) (1 << _input_bit_2))) != 0))  \
      _output &= (unsigned int) (~(1 << _output_bit));           \
    else                                                         \
      _output |= (unsigned int) (1 << _output_bit);              \
  }

#define _TIMER_T_0_LOCK(                                  \
  _index_timer,                                           \
  _max_count,                                             \
  _input, _input_bit,                                     \
  _output, _output_bit)                                   \
  {                                                       \
    if ((_input & (unsigned int) (1 << _input_bit)) == 0) \
      global_timers[_index_timer] = -1;                   \
    else                                                  \
    {                                                     \
      if (global_timers[_index_timer] < 0)                \
        global_timers[_index_timer] = 0;                  \
    }                                                     \
                                                          \
    if (global_timers[_index_timer] >= _max_count)        \
      _output |= (unsigned int) (1 << _output_bit);       \
    else                                                  \
    {                                                     \
      if (global_timers[_index_timer] == -1)              \
        _output &= (unsigned int) (~(1 << _output_bit));  \
    }                                                     \
  }

#define _TIMER_T_0(                                       \
  _index_timer,                                           \
  _max_count,                                             \
  _input, _input_bit,                                     \
  _output, _output_bit)                                   \
  {                                                       \
    if ((_input & (unsigned int) (1 << _input_bit)) == 0) \
      global_timers[_index_timer] = -1;                   \
    else                                                  \
    {                                                     \
      if (global_timers[_index_timer] < 0)                \
        global_timers[_index_timer] = 0;                  \
    }                                                     \
                                                          \
    if (global_timers[_index_timer] >= _max_count)        \
      _output |= (unsigned int) (1 << _output_bit);       \
    else                                                  \
      _output &= (unsigned int) (~(1 << _output_bit));    \
  }

#define _TIMER_0_T(                                       \
  _index_timer,                                           \
  _max_count,                                             \
  _input, _input_bit,                                     \
  _output, _output_bit)                                   \
  {                                                       \
    if ((_input & (unsigned int) (1 << _input_bit)) != 0) \
    {                                                     \
      global_timers[_index_timer] = 0;                    \
    }                                                     \
    else                                                  \
    {                                                     \
      if (global_timers[_index_timer] >= _max_count)      \
        global_timers[_index_timer] = -1;                 \
    }                                                     \
                                                          \
    if (global_timers[_index_timer] >= 0)                 \
      _output |= (unsigned int) (1 << _output_bit);       \
    else                                                  \
      _output &= (unsigned int) (~(1 << _output_bit));    \
  }

#define _TIMER_0_T_R(                                \
  _index_timer,                                      \
  _max_count,                                        \
  _reset, _reset_bit,                                \
  _input, _input_bit,                                \
  _output, _output_bit)                              \
  {                                                  \
    if ((_input & (1u << _input_bit)) != 0)          \
    {                                                \
      global_timers[_index_timer] = 0;               \
    }                                                \
    else                                             \
    {                                                \
      if (global_timers[_index_timer] >= _max_count) \
        global_timers[_index_timer] = -1;            \
    }                                                \
    if (_GET_STATE(_reset, _reset_bit))              \
      global_timers[_index_timer] = -1;              \
                                                     \
    if (global_timers[_index_timer] >= 0)            \
      _output |= (1u << _output_bit);                \
    else                                             \
      _output &= (~(1u << _output_bit));             \
  }

#define _TIMER_IMPULSE(                                \
  _index_timer,                                        \
  _max_count,                                          \
  _input, _input_bit,                                  \
  _INPUT, _INPUT_BIT,                                  \
  _OUTPUT, _OUTPUT_BIT)                                \
  {                                                    \
    unsigned int _c, _C;                               \
                                                       \
    _c = _GET_STATE(_input, _input_bit);               \
    _C = _GET_STATE(_INPUT, _INPUT_BIT);               \
                                                       \
    if (_c ^ _C)                                       \
    {                                                  \
      if (_C)                                          \
        _input |= (unsigned int) (1 << _input_bit);    \
      else                                             \
        _input &= (unsigned int) (~(1 << _input_bit)); \
                                                       \
      if (_C == 1)                                     \
        global_timers[_index_timer] = 0;               \
    }                                                  \
                                                       \
    if (global_timers[_index_timer] >= _max_count)     \
      global_timers[_index_timer] = -1;                \
                                                       \
    if (global_timers[_index_timer] >= 0)              \
      _OUTPUT |= (unsigned int) (1 << _OUTPUT_BIT);    \
    else                                               \
      _OUTPUT &= (unsigned int) (~(1 << _OUTPUT_BIT)); \
  }

#define _TIMER_IMPULSE_R(                               \
  _index_timer,                                         \
  _max_count,                                           \
  _reset, _reset_bit,                                   \
  _input, _input_bit,                                   \
  _INPUT, _INPUT_BIT,                                   \
  _OUTPUT, _OUTPUT_BIT)                                 \
  {                                                     \
    unsigned int _c, _C;                                \
                                                        \
    _c = _GET_STATE(_input, _input_bit);                \
    _C = _GET_STATE(_INPUT, _INPUT_BIT);                \
                                                        \
    if (_c ^ _C)                                        \
    {                                                   \
      if (_C)                                           \
        _input |= (1u << _input_bit);                   \
      else                                              \
        _input &= ~(1u << _input_bit);                  \
                                                        \
      if (_C == 1)                                      \
        global_timers[_index_timer] = 0;                \
    }                                                   \
                                                        \
    if (_GET_STATE(_reset, _reset_bit))                 \
      global_timers[_index_timer] = -1;                 \
    else if (global_timers[_index_timer] >= _max_count) \
      global_timers[_index_timer] = -1;                 \
                                                        \
    if (global_timers[_index_timer] >= 0)               \
      _OUTPUT |= (1u << _OUTPUT_BIT);                   \
    else                                                \
      _OUTPUT &= ~(1u << _OUTPUT_BIT);                  \
  }

#define _D_TRIGGER(                                        \
  _D,                                                      \
  _SET,                                                    \
  _CLR,                                                    \
  _synchro, _synchro_bit,                                  \
  _SYNCHRO, _SYNCHRO_BIT,                                  \
  _output, _output_bit)                                    \
  {                                                        \
    unsigned int _c, _C, _changing;                        \
                                                           \
    _c = _GET_STATE(_synchro, _synchro_bit);               \
    _C = _GET_STATE(_SYNCHRO, _SYNCHRO_BIT);               \
                                                           \
    if (_changing = (_c ^ _C))                             \
    {                                                      \
      if (_C)                                              \
        _synchro |= (unsigned int) (1 << _synchro_bit);    \
      else                                                 \
        _synchro &= (unsigned int) (~(1 << _synchro_bit)); \
    }                                                      \
                                                           \
    if (_CLR)                                              \
      _output &= (unsigned int) (~(1 << _output_bit));     \
    else if (_SET)                                         \
      _output |= (unsigned int) (1 << _output_bit);        \
    else                                                   \
    {                                                      \
      if (_changing && _C)                                 \
      {                                                    \
        if (_D)                                            \
          _output |= (unsigned int) (1 << _output_bit);    \
        else                                               \
          _output &= (unsigned int) (~(1 << _output_bit)); \
      }                                                    \
    }                                                      \
  }

#define _RS_TRIGGER(                                     \
  _S,                                                    \
  _R,                                                    \
  _output, _output_bit)                                  \
  {                                                      \
    if (_S ^ _R)                                         \
    {                                                    \
      if (_R)                                            \
        _output &= (unsigned int) (~(1 << _output_bit)); \
      else if (_S)                                       \
        _output |= (unsigned int) (1 << _output_bit);    \
    }                                                    \
  }

#define _MULTIPLEXER(                                                   \
  _ENB,                                                                 \
  _C,                                                                   \
  _input_1, _input_bit_1,                                               \
  _input_2, _input_bit_2,                                               \
  _output, _output_bit)                                                 \
  {                                                                     \
    _output &= (unsigned int) (~(1 << _output_bit));                    \
    if (_ENB)                                                           \
    {                                                                   \
      if (_C)                                                           \
        _output |= (_GET_STATE(_input_2, _input_bit_2) << _output_bit); \
      else                                                              \
        _output |= (_GET_STATE(_input_1, _input_bit_1) << _output_bit); \
    }                                                                   \
  }

#define _COUNTER(                                          \
  _R,                                                      \
  _input, _input_bit,                                      \
  _INPUT, _INPUT_BIT,                                      \
  _count,                                                  \
  _max_count,                                              \
  _output, _output_bit)                                    \
  {                                                        \
    unsigned int _c, _C;                                   \
                                                           \
    _c = _GET_STATE(_input, _input_bit);                   \
    _C = _GET_STATE(_INPUT, _INPUT_BIT);                   \
                                                           \
    if (_c ^ _C)                                           \
    {                                                      \
      if (_C)                                              \
        _input |= (unsigned int) (1 << _input_bit);        \
      else                                                 \
        _input &= (unsigned int) (~(1 << _input_bit));     \
    }                                                      \
                                                           \
    if (_R /* || (_max_count == 0)*/)                      \
    {                                                      \
      _count = 0;                                          \
      _output &= (unsigned int) (~(1 << _output_bit));     \
    }                                                      \
    else                                                   \
    {                                                      \
      if ((_c == 1) && (_C == 0) && (_count < _max_count)) \
        _count++;                                          \
                                                           \
      if (_count >= _max_count)                            \
        _output |= (unsigned int) (1 << _output_bit);      \
      else                                                 \
        _output &= (unsigned int) (~(1 << _output_bit));   \
    }                                                      \
  }

#define AR_WRITE(index, data)                             \
  {                                                       \
    int16_t test_data = index & 0xffff;                   \
    if ((array_ar[index] = test_data) != test_data)       \
      _SET_BIT(set_diagnostyka, ERROR_EXTERNAL_SRAM_BIT); \
                                                          \
    array_ar[index++] = data;                             \
  }

#define INT_TO_BCD(_data) ((((_data) / 10) << 4) | ((_data) % 10))

#define COMPARE_NOT_ZERO_OR(_comp, _p_array, _arr_maska, _n)   \
  {                                                            \
    _comp = false;                                             \
    for (size_t _i = 0; ((_comp == false) && (_i < _n)); ++_i) \
    {                                                          \
      _comp |= ((_p_array[_i] & _arr_maska[_i]) != 0);         \
    }                                                          \
  }

#define COMPARE_OR(_comp, _p_array1, _p_array2, _n)            \
  {                                                            \
    _comp = false;                                             \
    for (size_t _i = 0; ((_comp == false) && (_i < _n)); ++_i) \
    {                                                          \
      _comp |= (_p_array1[_i] != _p_array2[_i]);               \
    }                                                          \
  }

#define NOT_ZERO_OR(_comp, _p_array, _n)                       \
  {                                                            \
    _comp = false;                                             \
    for (size_t _i = 0; ((_comp == false) && (_i < _n)); ++_i) \
    {                                                          \
      _comp |= (_p_array[_i] != 0);                            \
    }                                                          \
  }

#define COMPARE_ZERO_AND(_comp, _p_array, _arr_maska, _n)     \
  {                                                           \
    comp = true;                                              \
    for (size_t _i = 0; ((_comp == true) && (_i < _n)); ++_i) \
    {                                                         \
      _comp &= ((_p_array[_i] & _arr_maska[_i]) == 0);        \
    }                                                         \
  }

#define ZERO_AND(_comp, _p_array, _n)                         \
  {                                                           \
    comp = true;                                              \
    for (size_t _i = 0; ((_comp == true) && (_i < _n)); ++_i) \
    {                                                         \
      _comp &= (_p_array[_i] == 0);                           \
    }                                                         \
  }

#define COMPARE_AND(_comp, _p_array1, _p_array2, _n)          \
  {                                                           \
    _comp = true;                                             \
    for (size_t _i = 0; ((_comp == true) && (_i < _n)); ++_i) \
    {                                                         \
      _comp &= (_p_array1[_i] == _p_array2[_i]);              \
    }                                                         \
  }

#endif
