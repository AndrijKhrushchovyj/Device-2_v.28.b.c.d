#ifndef __CONST_MENU_DIAGNOSTYKA__
#define __CONST_MENU_DIAGNOSTYKA__

#define EKRAN_DIAGNOSTYKA                    (EKRAN_POINT_TIME_RANGUVANNJA + 1)

enum _error_id 
{
ERROR_START_VIA_I2C_BIT = 0,
ERROR_SETTINGS_EEPROM_BIT,
ERROR_SETTINGS_EEPROM_EMPTY_BIT,
ERROR_SETTINGS_EEPROM_COMPARISON_BIT,
ERROR_SETTINGS_EEPROM_CONTROL_BIT,
ERROR_SETTINGS_EEPROM_DEVICE_ID_FAIL_BIT,
ERROR_USTUVANNJA_EEPROM_BIT,
ERROR_USTUVANNJA_EEPROM_EMPTY_BIT,
ERROR_USTUVANNJA_EEPROM_COMPARISON_BIT,
ERROR_USTUVANNJA_EEPROM_CONTROL_BIT,
ERROR_USTUVANNJA_EEPROM_ADJUSTMENT_ID_FAIL_BIT,
ERROR_STATE_LEDS_EEPROM_BIT,
ERROR_STATE_OUTPUTS_EEPROM_BIT,
ERROR_STATE_LEDS_OUTPUTS_EEPROM_EMPTY_BIT,
ERROR_STATE_LEDS_OUTPUTS_EEPROM_COMPARISON_BIT,
ERROR_TRG_FUNC_EEPROM_BIT,
ERROR_TRG_FUNC_EEPROM_EMPTY_BIT,
ERROR_TRG_FUNC_EEPROM_COMPARISON_BIT,
ERROR_TRG_FUNC_EEPROM_CONTROL_BIT,
ERROR_INFO_REJESTRATOR_AR_EEPROM_BIT,
ERROR_INFO_REJESTRATOR_AR_EEPROM_EMPTY_BIT,
ERROR_INFO_REJESTRATOR_AR_COMPARISON_BIT,
ERROR_INFO_REJESTRATOR_AR_CONTROL_BIT,
ERROR_INFO_REJESTRATOR_DR_EEPROM_BIT,
ERROR_INFO_REJESTRATOR_DR_EEPROM_EMPTY_BIT,
ERROR_INFO_REJESTRATOR_DR_COMPARISON_BIT,
ERROR_INFO_REJESTRATOR_DR_CONTROL_BIT,
ERROR_INFO_REJESTRATOR_PR_ERR_EEPROM_BIT,
ERROR_INFO_REJESTRATOR_PR_ERR_EEPROM_EMPTY_BIT,
ERROR_INFO_REJESTRATOR_PR_ERR_COMPARISON_BIT,
ERROR_INFO_REJESTRATOR_PR_ERR_CONTROL_BIT,
ERROR_RESURS_EEPROM_BIT,
ERROR_RESURS_EEPROM_EMPTY_BIT,
ERROR_RESURS_EEPROM_COMPARISON_BIT,
ERROR_RESURS_EEPROM_CONTROL_BIT,
ERROR_ENERGY_EEPROM_BIT,
ERROR_ENERGY_EEPROM_EMPTY_BIT,
ERROR_ENERGY_EEPROM_COMPARISON_BIT,

RTC_BATTERY_LOW_BIT,
RTC_OSCILLATOR_STOPED_BIT,
RTC_OSCILLATOR_FAIL_BIT,
RTC_UPDATING_HALTED_BIT,
RTC_WORK_FIELD_NOT_SET_BIT,

ERROR_VREF_ADC_TEST_BIT,

ERROR_SPI_ADC_BIT,

ERROR_OSCYLOJRAPH_OVERFLOW,

ERROR_DIGITAL_OUTPUT_1_BIT,

ERROR_DS_OUTPUT_BIT = ERROR_DIGITAL_OUTPUT_1_BIT + 24,

ERROR_AR_TEMPORARY_BUSY_BIT = ERROR_DS_OUTPUT_BIT + 1,
ERROR_AR_OVERLOAD_BUFFER_BIT,
ERROR_AR_MEMORY_FULL_BIT,
ERROR_AR_UNDEFINED_BIT,

ERROR_DR_TEMPORARY_BUSY_BIT,
ERROR_DR_UNDEFINED_BIT,
ERROR_DR_LOSS_INFORMATION_BIT,

ERROR_PR_ERR_OVERLOAD_BIT,
ERROR_PR_ERR_LOSS_INFORMATION_BIT,

EVENT_START_SYSTEM_BIT,
EVENT_RESTART_SYSTEM_BIT,
EVENT_SOFT_RESTART_SYSTEM_BIT,
EVENT_STOP_SYSTEM_BIT,
EVENT_DROP_POWER_BIT,

ERROR_LCD_BIT,
ERROR_SPI_EDF_BIT,
ERROR_EXTERNAL_SRAM_BIT,
ERROR_INTERNAL_FLASH_BIT,

ERROR_SELECT_GRUPY_USRAVOK,

LOSE_ENERGY_DATA,

TEST_OVD1,
TEST_OVD2,
TEST_OVD3,
TEST_OVD4,

ERROR_CPU_RECEIVING_CANAL_1,
ERROR_CPU_RECEIVED_PACKET_CANAL_1,
ERROR_CPU_ANSWER_CANAL_1,
ERROR_CPU_NO_ANSWER_CANAL_1,
ERROR_IEC_RECEIVING_CANAL_1,
ERROR_IEC_RECEIVED_PACKET_CANAL_1,
ERROR_IEC_REQUEST_CANAL_1,
ERROR_IEC_NO_ANSWER_CANAL_1,

ERROR_CPU_RECEIVING_CANAL_2,
ERROR_CPU_RECEIVED_PACKET_CANAL_2,
ERROR_CPU_ANSWER_CANAL_2,
ERROR_CPU_NO_ANSWER_CANAL_2,
ERROR_IEC_RECEIVING_CANAL_2,
ERROR_IEC_RECEIVED_PACKET_CANAL_2,
ERROR_IEC_REQUEST_CANAL_2,

ERROR_BA_1_FIX,
ERROR_BA_1_CTLR,
ERROR_BDVV5_1_FIX,
ERROR_BDVV5_1_CTLR,
ERROR_BDVV5_2_FIX,
ERROR_BDVV5_2_CTLR,
ERROR_BDV_DZ_FIX,
ERROR_BDV_DZ_CTLR,
ERROR_BDZ_FIX,
ERROR_BDZ_CTLR,
ERROR_BDVV6_FIX,
ERROR_BDVV6_CTLR,
ERROR_BDSH_FIX,
ERROR_BDSH_CTLR,
ERROR_BDVV9_FIX,
ERROR_BDVV9_CTLR,

ERROR_BDV2_FIX,
ERROR_BDV2_CTLR,
ERROR_BDV5_FIX,
ERROR_BDV5_CTLR,

ERROR_CB_FIX,

ERROR_FATFS,
EVENT_RESTART_CB_BIT,
WARNING_REPROGRAM,

ERROR_BDVV_DZ_FIX,
ERROR_BDVV_DZ_CTLR,

//?ERROR_OVER_FLOW_EXTERNAL_WATCHDOG_IN,
//?ERROR_OVER_FLOW_EXTERNAL_WATCHDOG_OUT,

MAX_ROW_FOR_DIAGNOSTYKA
};

#define N_DIAGN                              ((MAX_ROW_FOR_DIAGNOSTYKA >> 5) + ((MAX_ROW_FOR_DIAGNOSTYKA & 0x1f) != 0))
#define N_DIAGN_BYTES                        ((MAX_ROW_FOR_DIAGNOSTYKA >> 3) + ((MAX_ROW_FOR_DIAGNOSTYKA & 0x07) != 0))

#define WORD_0_MASKA_ERRORS_FROM_CANAL_1 0
#define WORD_1_MASKA_ERRORS_FROM_CANAL_1 0

#define WORD_2_MASKA_ERRORS_FROM_CANAL_1 (unsigned int)       \
          (                                                   \
           (1 << (ERROR_CPU_RECEIVING_CANAL_1 - 64))          \
          )   

#define WORD_3_MASKA_ERRORS_FROM_CANAL_1 (unsigned int)       \
          (                                                   \
           (1 << (ERROR_CPU_RECEIVED_PACKET_CANAL_1 - 96))   |\
           (1 << (ERROR_CPU_ANSWER_CANAL_1 - 96))            |\
           (1 << (ERROR_CPU_NO_ANSWER_CANAL_1 - 96))         |\
           (1 << (ERROR_IEC_RECEIVING_CANAL_1 - 96))         |\
           (1 << (ERROR_IEC_RECEIVED_PACKET_CANAL_1 - 96))   |\
           (1 << (ERROR_IEC_REQUEST_CANAL_1 - 96))           |\
           (1 << (ERROR_IEC_NO_ANSWER_CANAL_1 - 96))         |\
           (1 << (ERROR_IEC_RECEIVING_CANAL_2 - 96))         |\
           (1 << (ERROR_IEC_RECEIVED_PACKET_CANAL_2 - 96))   |\
           (1 << (ERROR_IEC_REQUEST_CANAL_2 - 96))            \
          )   
            
#define WORD_4_MASKA_ERRORS_FROM_CANAL_1 0


#define WORD_0_MASKA_RECEIVING_ERRORS_CANAL_2 0
#define WORD_1_MASKA_RECEIVING_ERRORS_CANAL_2 0
#define WORD_2_MASKA_RECEIVING_ERRORS_CANAL_2 0

#define WORD_3_MASKA_RECEIVING_ERRORS_CANAL_2 (unsigned int)  \
          (                                                   \
           (1 << (ERROR_CPU_RECEIVING_CANAL_2 - 96))         |\
           (1 << (ERROR_CPU_RECEIVED_PACKET_CANAL_2 - 96))   |\
           (1 << (ERROR_CPU_ANSWER_CANAL_2 - 96))            |\
           (1 << (ERROR_CPU_NO_ANSWER_CANAL_2 - 96))          \
          ) 

#define WORD_4_MASKA_RECEIVING_ERRORS_CANAL_2 0
            
#define MASKA_AVAR_ERROR_0        (unsigned int)(               \
    (1 << (ERROR_SETTINGS_EEPROM_BIT))                          \
  | (1 << (ERROR_SETTINGS_EEPROM_EMPTY_BIT))                    \
  | (1 << (ERROR_SETTINGS_EEPROM_COMPARISON_BIT) )              \
  | (1 << (ERROR_SETTINGS_EEPROM_CONTROL_BIT))                  \
  | (1 << (ERROR_USTUVANNJA_EEPROM_BIT) )                       \
  | (1 << (ERROR_USTUVANNJA_EEPROM_EMPTY_BIT))                  \
  | (1 << (ERROR_USTUVANNJA_EEPROM_COMPARISON_BIT))             \
  | (1 << (ERROR_USTUVANNJA_EEPROM_CONTROL_BIT))                \
  | (1 << (ERROR_USTUVANNJA_EEPROM_ADJUSTMENT_ID_FAIL_BIT))     \
  | (1 << (ERROR_TRG_FUNC_EEPROM_BIT))                          \
  | (1 << (ERROR_TRG_FUNC_EEPROM_EMPTY_BIT))                    \
  | (1 << (ERROR_TRG_FUNC_EEPROM_COMPARISON_BIT))               \
  | (1 << (ERROR_TRG_FUNC_EEPROM_CONTROL_BIT))                  \
)

#define MASKA_AVAR_ERROR_1        (unsigned int)(               \
    (1 << (ERROR_VREF_ADC_TEST_BIT - 32))                       \
  | (1 << (ERROR_SPI_ADC_BIT - 32))                             \
  | (1 << (ERROR_DIGITAL_OUTPUT_1_BIT +  0 - 32))               \
  | (1 << (ERROR_DIGITAL_OUTPUT_1_BIT +  1 - 32))               \
  | (1 << (ERROR_DIGITAL_OUTPUT_1_BIT +  2 - 32))               \
  | (1 << (ERROR_DIGITAL_OUTPUT_1_BIT +  3 - 32))               \
  | (1 << (ERROR_DIGITAL_OUTPUT_1_BIT +  4 - 32))               \
  | (1 << (ERROR_DIGITAL_OUTPUT_1_BIT +  5 - 32))               \
  | (1 << (ERROR_DIGITAL_OUTPUT_1_BIT +  6 - 32))               \
  | (1 << (ERROR_DIGITAL_OUTPUT_1_BIT +  7 - 32))               \
  | (1 << (ERROR_DIGITAL_OUTPUT_1_BIT +  8 - 32))               \
  | (1 << (ERROR_DIGITAL_OUTPUT_1_BIT +  9 - 32))               \
  | (1 << (ERROR_DIGITAL_OUTPUT_1_BIT + 10 - 32))               \
  | (1 << (ERROR_DIGITAL_OUTPUT_1_BIT + 11 - 32))               \
  | (1 << (ERROR_DIGITAL_OUTPUT_1_BIT + 12 - 32))               \
  | (1 << (ERROR_DIGITAL_OUTPUT_1_BIT + 13 - 32))               \
  | (1 << (ERROR_DIGITAL_OUTPUT_1_BIT + 14 - 32))               \
  | (1 << (ERROR_DIGITAL_OUTPUT_1_BIT + 15 - 32))               \
  | (1 << (ERROR_DIGITAL_OUTPUT_1_BIT + 16 - 32))               \
  | (1 << (ERROR_DIGITAL_OUTPUT_1_BIT + 17 - 32))               \
)

#define MASKA_AVAR_ERROR_2        (unsigned int)(               \
    (1 << (ERROR_DIGITAL_OUTPUT_1_BIT + 18 - 64))               \
  | (1 << (ERROR_DIGITAL_OUTPUT_1_BIT + 19 - 64))               \
  | (1 << (ERROR_DIGITAL_OUTPUT_1_BIT + 20 - 64))               \
  | (1 << (ERROR_DIGITAL_OUTPUT_1_BIT + 21 - 64))               \
  | (1 << (ERROR_DIGITAL_OUTPUT_1_BIT + 22 - 64))               \
  | (1 << (ERROR_DIGITAL_OUTPUT_1_BIT + 23 - 64))               \
  | (1 << (ERROR_DS_OUTPUT_BIT - 64))                           \
  | (1 << (ERROR_INTERNAL_FLASH_BIT - 64))                      \
)

#define MASKA_AVAR_ERROR_3        (unsigned int)(               \
   (1 << (ERROR_BA_1_FIX - 96))                                 \
  | (1 << (ERROR_BA_1_CTLR - 96))                               \
  | (1 << (ERROR_BDVV5_1_FIX - 96))                             \
  | (1 << (ERROR_BDVV5_1_CTLR - 96))                            \
  | (1 << (ERROR_BDVV5_2_FIX - 96))                             \
  | (1 << (ERROR_BDVV5_2_CTLR - 96))                            \
  | (1 << (ERROR_BDV_DZ_FIX - 96))                              \
  | (1 << (ERROR_BDV_DZ_CTLR - 96))                             \
  | (1 << (ERROR_BDZ_FIX - 96))                                 \
  | (1 << (ERROR_BDZ_CTLR - 96))                                \
  | (1 << (ERROR_BDVV6_FIX - 96))                               \
  | (1 << (ERROR_BDVV6_CTLR - 96))                              \
  | (1 << (ERROR_BDSH_FIX - 96))                                \
  | (1 << (ERROR_BDSH_CTLR - 96))                               \
  | (1 << (ERROR_BDVV9_FIX - 96))                               \
  | (1 << (ERROR_BDVV9_CTLR - 96))                              \
  | (1 << (ERROR_BDV2_FIX - 96))                                \
  | (1 << (ERROR_BDV2_CTLR - 96))                               \
)


#define MASKA_AVAR_ERROR_4        (unsigned int)(               \
    (1 << (ERROR_BDV5_FIX - 128))                               \
  | (1 << (WARNING_REPROGRAM - 128))                            \
  | (1 << (ERROR_CB_FIX - 128))                                 \
  | (1 << (ERROR_BDV5_CTLR - 128))                              \
  | (1 << (ERROR_BDVV_DZ_FIX - 128))                            \
  | (1 << (ERROR_BDVV_DZ_CTLR - 128))                           \
)

#define NAME_DIAGN_RU   \
  " Ош.I2C         ",   \
  " Ош.настроек    ",   \
  " Настроек нет   ",   \
  " Ош.зап.настр.  ",   \
  " Ош.контр.настр.",   \
  " Ош.типа настр. ",   \
  " Ош.юстирования ",   \
  " Юстирования нет",   \
  " Ош.зап.юстир.  ",   \
  " Ош.контр.юстир.",   \
  " Ош.юстир.набора",   \
  " Ош.воcст.тр.св ",   \
  " Ош.воcст.с.вых ",   \
  "Инф.вых./св.нет ",   \
  " Ош.зап.вых./св.",   \
  " Ош.Ин.ЭнНезСЛ  ",   \
  " Ин.ЭнНезСЛ Нет ",   \
  " Ош.З.Ин.ЭнНезСЛ",   \
  " Ош.К.Ин.ЭнНезСЛ",   \
  " Ош.инф.ан.рег. ",   \
  " Инф.ан.рег.нет ",   \
  "Ош.зап.и.ан.рег.",   \
  "Ош.контр.ан.рег.",   \
  " Ош.инф.д.рег.  ",   \
  " Инф.д.рег.нет  ",   \
  " Ош.зап.и.д.рег.",   \
  " Ош.контр.д.рег.",   \
  "Ош.инф.рег.пр.с.",   \
  "Инф.рег.пр.с.нет",   \
  "Ош.зап.и.р.пр.с.",   \
  "Ош.контр.р.пр.с.",   \
  " Ош.инф.сч.рес. ",   \
  " Инф.сч.рес.нет ",   \
  " Ош.зап.сч.рес. ",   \
  "Ош.контр.сч.рес.",   \
  " Ош.к.с.энергий ",   \
  " Энергий нет    ",   \
  " Ош.зап.энергий ",   \
  " Батарея разряж.",   \
  "Осцилятор остан.",   \
  "Отказ Осцилятора",   \
  " Ост.обновл.RTC ",   \
  " Не уст.поля RTC",   \
  " Тест VREF АЦП  ",   \
  " Ош. SPI АЦП    ",   \
  "Переп.буф.ц.осц.",   \
  " Ош.вых.реле ?.?",   \
  " Ош.вых.реле ?.?",   \
  " Ош.вых.реле ?.?",   \
  " Ош.вых.реле ?.?",   \
  " Ош.вых.реле ?.?",   \
  " Ош.вых.реле ?.?",   \
  " Ош.вых.реле ?.?",   \
  " Ош.вых.реле ?.?",   \
  " Ош.вых.реле ?.?",   \
  " Ош.вых.реле ?.?",   \
  " Ош.вых.реле ?.?",   \
  " Ош.вых.реле ?.?",   \
  " Ош.вых.реле ?.?",   \
  " Ош.вых.реле ?.?",   \
  " Ош.вых.реле ?.?",   \
  " Ош.вых.реле ?.?",   \
  " Ош.вых.реле ?.?",   \
  " Ош.вых.реле ?.?",   \
  " Ош.вых.реле ?.?",   \
  " Ош.вых.реле ?.?",   \
  " Ош.вых.реле ?.?",   \
  " Ош.вых.реле ?.?",   \
  " Ош.вых.реле ?.?",   \
  " Ош.вых.реле ?.?",   \
  " Ош.вых.ДШ      ",   \
  "Ан.рег.вр.занят.",   \
  " Пер.буф.aн.рег.",   \
  " П.aн.рег.исч.  ",   \
  "Неопр.ош.ан.рег.",   \
  " Д.рег.вр.занят.",   \
  " Неопр.ош.д.рег.",   \
  " Потеря д.д.рег.",   \
  "Переп.буф.пр.ош.",   \
  " Потеря д.пр.ош.",   \
  " Старт устр.    ",   \
  " Рестарт устр.  ",   \
  "Пр.Рестарт устр.",   \
  " Останов.устр.  ",   \
  " Пропад.питания ",   \
  " Отказ ЖКИ      ",   \
  " Ош.SPI_EDF     ",   \
  " Ош.внешней SRAM",   \
  " Ош.внутр.FLASH ",   \
  " Ош.выб.гр.уст. ",   \
  " Пот.д.энергии  ",   \
  " Тест ОВД1      ",   \
  " Тест ОВД2      ",   \
  " Тест ОВД3      ",   \
  " Тест ОВД4      ",   \
  "ЦП:Ош.приема к.1",   \
  "ЦП:Ош.пакета к.1",   \
  "ЦП:Ош.ответа к.1",   \
  " ЦП:Нет отв.к.1 ",   \
  "КП:Ош.приема к.1",   \
  "КП:Ош.пакета к.1",   \
  " КП:Ош.запр.к.1 ",   \
  " КП:Нет запр.к.1",   \
  "ЦП:Ош.приема к.2",   \
  "ЦП:Ош.пакета к.2",   \
  "ЦП:Ош.ответа к.2",   \
  " ЦП:Нет отв.к.2 ",   \
  "КП:Ош.приема к.2",   \
  "КП:Ош.пакета к.2",   \
  " КП:Ош.запр.к.2 ",   \
  " БА1 от.        ",   \
  " БА1 п.         ",   \
  " БДВВ5_1 от.    ",   \
  " БДВВ5_1 п.     ",   \
  " БДВВ5_2 от.    ",   \
  " БДВВ5_2 п.     ",   \
  " БДВ-ДЗ от.     ",   \
  " БДВ-ДЗ п.      ",   \
  " БДЗ от.        ",   \
  " БДЗ п.         ",   \
  " БДВВ6 от.      ",   \
  " БДВВ6 п.       ",   \
  " БДШ от.        ",   \
  " БДШ п.         ",   \
  " БДВВ9 от.      ",   \
  " БДВВ9 п.       ",   \
  " БДВ2  от.      ",   \
  " БДВ2  п.       ",   \
  " БДВ5  от.      ",   \
  " БДВ5  п.       ",   \
  " КП от.         ",   \
  " Ош.Ф.С.        ",   \
  " Рестарт КП     ",   \
  "Режим перепрогр.",   \
  " БДВВ-ДЗ от.    ",   \
  " БДВВ-ДЗ п.     "

# define NAME_DIAGN_UA  \
  " Пом.I2C        ",   \
  " Пом.налаштувань",   \
  "Налаштувань нема",   \
  " Пом.зап.налашт.",   \
  "Пом.контр.налашт",   \
  " Пом.типу налашт",   \
  " Пом.юстування  ",   \
  " Юстування нема ",   \
  " Пом.зап.юст.   ",   \
  " Пом.контр.юст. ",   \
  " Пом.юст.набору ",   \
  " Пом.відн.тр.св.",   \
  " Пом.відн.с.вих.",   \
  "Інф.вих./св.нема",   \
  "Пом.зап.вих./св.",   \
  " Пом.Ін.ЕнНезСЛ ",   \
  " Ін.ЕнНезСЛ Нема",   \
  "Пом.З.Ін.ЕнНезСЛ",   \
  "Пом.К.Ін.ЕнНезСЛ",   \
  " Пом.інф.ан.р.  ",   \
  " Інф.ан.р.нема  ",   \
  " Пом.зап.і.ан.р.",   \
  " Пом.контр.ан.р.",   \
  " Пом.інф.д.р.   ",   \
  " Інф.д.р.нема   ",   \
  " Пом.зап.і.д.р. ",   \
  " Пом.контр.д.р. ",   \
  " Пом.інф.р.пр.п.",   \
  " Інф.р.пр.п.нема",   \
  "Пом.зап.і.р.пр.п",   \
  "Пом.контр.р.пр.п",   \
  " Пом.інф.ліч.р. ",   \
  " Інф.ліч.р.нема ",   \
  " Пом.зап.ліч.р. ",   \
  "Пом.контр.ліч.р.",   \
  " Пом.к.с.енергій",   \
  " Енергій нема   ",   \
  " Пом.зап.енергій",   \
  "Батарея розрядж.",   \
  " Осцилятор зуп. ",   \
  " Відм.Осцилятора",   \
  " Зуп.обновл.RTC ",   \
  " Не вст.поля RTC",   \
  " Тест VREF АЦП  ",   \
  " Пом.SPI АЦП    ",   \
  "Переп.буф.ц.осц.",   \
  " Пом.вих.реле?.?",   \
  " Пом.вих.реле?.?",   \
  " Пом.вих.реле?.?",   \
  " Пом.вих.реле?.?",   \
  " Пом.вих.реле?.?",   \
  " Пом.вих.реле?.?",   \
  " Пом.вих.реле?.?",   \
  " Пом.вих.реле?.?",   \
  " Пом.вих.реле?.?",   \
  " Пом.вих.реле?.?",   \
  " Пом.вих.реле?.?",   \
  " Пом.вих.реле?.?",   \
  " Пом.вих.реле?.?",   \
  " Пом.вих.реле?.?",   \
  " Пом.вих.реле?.?",   \
  " Пом.вих.реле?.?",   \
  " Пом.вих.реле?.?",   \
  " Пом.вих.реле?.?",   \
  " Пом.вих.реле?.?",   \
  " Пом.вих.реле?.?",   \
  " Пом.вих.реле?.?",   \
  " Пом.вих.реле?.?",   \
  " Пом.вих.реле?.?",   \
  " Пом.вих.реле?.?",   \
  " Пом.вих.ДШ     ",   \
  "Ан.р.тимч.зайнят",   \
  " Переп.буф.aн.р.",   \
  " П.aн.рег.вич.  ",   \
  "Невизн.пом.ан.р.",   \
  "Д.р.тимч.зайнят.",   \
  " Невизн.пом.д.р.",   \
  " Втрата д.д.р.  ",   \
  "Переп.буф.р.пр.п",   \
  " Втрата д.р.пр.п",   \
  " Старт пристр.  ",   \
  " Рестарт пристр.",   \
  " Пр.Рестарт пр. ",   \
  " Зуп.пристр.    ",   \
  " Пропад.живлення",   \
  " Відмова РКІ    ",   \
  " Пом.SPI_EDF    ",   \
  " Пом.зовн.SRAM  ",   \
  " Пом.внутр.FLASH",   \
  " Пом.виб.гр.уст.",   \
  " Втр.д.енергії  ",   \
  " Тест ОВД1      ",   \
  " Тест ОВД2      ",   \
  " Тест ОВД3      ",   \
  " Тест ОВД4      ",   \
  "ЦП:Пом.прийм.к.1",   \
  " ЦП:Пом.п-ту к.1",   \
  " ЦП:Пом.відп.к.1",   \
  " ЦП:Нема в.к.1  ",   \
  "КП:Пом.прийм.к.1",   \
  " КП:Пом.п-ту к.1",   \
  " КП:Пом.зап.к.1 ",   \
  " КП:Нема зап.к.1",   \
  "ЦП:Пом.прийм.к.2",   \
  " ЦП:Пом.п-ту к.2",   \
  " ЦП:Пом.відп.к.2",   \
  " ЦП:Нема в.к.2  ",   \
  "КП:Пом.прийм.к.2",   \
  " КП:Пом.п-ту к.2",   \
  " КП:Пом.зап.к.2 ",   \
  " БА1 від.       ",   \
  " БА1 п.         ",   \
  " БДВВ5_1 від.   ",   \
  " БДВВ5_1 п.     ",   \
  " БДВВ5_2 від.   ",   \
  " БДВВ5_2 п.     ",   \
  " БДВ-ДЗ від.    ",   \
  " БДВ-ДЗ п.      ",   \
  " БДЗ від.       ",   \
  " БДЗ п.         ",   \
  " БДВВ6 від.     ",   \
  " БДВВ6 п.       ",   \
  " БДШ від.       ",   \
  " БДШ п.         ",   \
  " БДВВ9 від.     ",   \
  " БДВВ9 п.       ",   \
  " БДВ2  від.     ",   \
  " БДВ2  п.       ",   \
  " БДВ5  від.     ",   \
  " БДВ5  п.       ",   \
  " КП від.        ",   \
  " Пом.Ф.С.       ",   \
  " Рестарт КП     ",   \
  "Режим перепрогр.",   \
  " БДВВ-ДЗ від.   ",   \
  " БДВВ-ДЗ п.     "

# define NAME_DIAGN_EN  \
  " I2C Er         ",   \
  " Settings Er    ",   \
  " No settings    ",   \
  " Sett W Er      ",   \
  " Sett Ctl Er    ",   \
  " Sett Type Er   ",   \
  " Adjust Er      ",   \
  " No adjust      ",   \
  " Adjust W Er    ",   \
  "Adjust Ctl Er   ",   \
  " Adjust-Set Er  ",   \
  "Latc LED Rest Er",   \
  " Latc BO Rest Er",   \
  " No BO/LED Inf  ",   \
  " BO/LED W Er    ",   \
  " Nvol LS Inf Er ",   \
  " No Nvol LS Inf ",   \
  "Nvol LS Inf W Er",   \
  "Nvol LS Inf C Er",   \
  " Dst Rec Er     ",   \
  " Dst Rec No info",   \
  " Dst Rec W Er   ",   \
  " Dst Rec Ctr Er ",   \
  " Bin Rec Er     ",   \
  " Bin Rec No info",   \
  " Bin Rec W Er   ",   \
  " Bin Rec Ctr Er ",   \
  " Diag Rec Er    ",   \
  "Diag Rec No info",   \
  " Diag Rec W Er  ",   \
  "Diag Rec Ctr Er ",   \
  " CB Res Coun Er ",   \
  "CB Res Ctr No In",   \
  " CB Res Ctr W Er",   \
  " CB R Cnt Ctr Er",   \
  " Energy calc Er ",   \
  " Energy calc Abs",   \
  "Energy calc W Er",   \
  " RTC:Battery low",   \
  " RTC:Osc stop   ",   \
  " RTC:Osc fail   ",   \
  " RTC:Halt update",   \
  "RTC:No def sett ",   \
  " ADC:VREF fail  ",   \
  " ADC SPI Er     ",   \
  "UnAp D Buf Ovrfl",   \
  " BO?.? Ctl Er   ",   \
  " BO?.? Ctl Er   ",   \
  " BO?.? Ctl Er   ",   \
  " BO?.? Ctl Er   ",   \
  " BO?.? Ctl Er   ",   \
  " BO?.? Ctl Er   ",   \
  " BO?.? Ctl Er   ",   \
  " BO?.? Ctl Er   ",   \
  " BO?.? Ctl Er   ",   \
  " BO?.? Ctl Er   ",   \
  " BO?.? Ctl Er   ",   \
  " BO?.? Ctl Er   ",   \
  " BO?.? Ctl Er   ",   \
  " BO?.? Ctl Er   ",   \
  " BO?.? Ctl Er   ",   \
  " BO?.? Ctl Er   ",   \
  " BO?.? Ctl Er   ",   \
  " BO?.? Ctl Er   ",   \
  " BO?.? Ctl Er   ",   \
  " BO?.? Ctl Er   ",   \
  " BO?.? Ctl Er   ",   \
  " BO?.? Ctl Er   ",   \
  " BO?.? Ctl Er   ",   \
  " BO?.? Ctl Er   ",   \
  " DSh outp. err. ",   \
  " Dst Rec Busy   ",   \
  " Dst Rec Buf Ovf",   \
  "Dst Rec Mem Full",   \
  "UnDef Dst Rec Er",   \
  " Bin Rec busy   ",   \
  "UnDef Bin Rec Er",   \
  "Bin Rec Dat Lost",   \
  "OVF of PER buff ",   \
  " PER Dat lost   ",   \
  " Device Start   ",   \
  " Device Restart ",   \
  " Device SFTRST  ",   \
  " Device Stop    ",   \
  " Power Outage   ",   \
  " LCD Fail       ",   \
  " EDF SPI Er     ",   \
  " Ext SRAM Er    ",   \
  " Flash Mem Er   ",   \
  " SP Gr Sel Er   ",   \
  " En Ctr Dat Lost",   \
  " Sensor1 Test   ",   \
  " Sensor2 Test   ",   \
  " Sensor3 Test   ",   \
  " Sensor4 Test   ",   \
  " CPU:Rec Ch1 err",   \
  "CPU:Pack Ch1 err",   \
  "CPU:Send Ch1 err",   \
  " CPU:No resp Ch1",   \
  " CU:Rec  Ch1 err",   \
  " CU:Pack Ch1 err",   \
  " CU:Send Ch1 err",   \
  " CU:No resp Ch1 ",   \
  " CPU:Rec Ch2 err",   \
  "CPU:Pack Ch2 err",   \
  "CPU:Send Ch2 err",   \
  " CPU:No resp Ch2",   \
  " CU:Rec Ch2 err ",   \
  " CU:Pack Ch2 err",   \
  " CU:Send Ch2 err",   \
  " AIU01 abs      ",   \
  " AIU01 ver      ",   \
  " BIOU05_1 abs   ",   \
  " BIOU05_1 ver   ",   \
  " BIOU05_2 abs   ",   \
  " BIOU05_2 ver   ",   \
  " BIASU01 abs    ",   \
  " BIASU01 ver    ",   \
  " ASU abs        ",   \
  " ASU ver        ",   \
  " BIOU06 abs     ",   \
  " BIOU06 ver     ",   \
  " DShU01 abs.    ",   \
  " DShU01 ver.    ",   \
  " BIOU09 abs     ",   \
  " BIOU09 ver     ",   \
  " BIU02  abs     ",   \
  " BIU02  ver     ",   \
  " BOU05  abs     ",   \
  " BOU05  ver     ",   \
  " CU abs.        ",   \
  " F.S.Error      ",   \
  " CU Restart     ",   \
  " Reprogram Mode ",   \
  " BIOASU01 abs   ",   \
  " BIOASU01 ver   "

# define NAME_DIAGN_KZ  \
  " Ош.I2C         ",   \
  " Ош.настроек    ",   \
  " Настроек нет   ",   \
  " Ош.зап.настр.  ",   \
  " Ош.контр.настр.",   \
  " Ош.типа настр. ",   \
  " Ош.юстирования ",   \
  " Юстирования нет",   \
  " Ош.зап.юстир.  ",   \
  " Ош.контр.юстир.",   \
  " Ош.юстир.набора",   \
  " Ош.воcст.тр.св ",   \
  " Ош.воcст.с.вых ",   \
  "Инф.вых./св.нет ",   \
  " Ош.зап.вых./св.",   \
  "  Ош.Ин.ЭнНезСЛ ",   \
  " Ин.ЭнНезСЛ Нет ",   \
  " Ош.З.Ин.ЭнНезСЛ",   \
  " Ош.К.Ин.ЭнНезСЛ",   \
  " Ош.инф.ан.рег. ",   \
  " Инф.ан.рег.нет ",   \
  "Ош.зап.и.ан.рег.",   \
  "Ош.контр.ан.рег.",   \
  " Ош.инф.д.рег.  ",   \
  " Инф.д.рег.нет  ",   \
  " Ош.зап.и.д.рег.",   \
  " Ош.контр.д.рег.",   \
  "Ош.инф.рег.пр.с.",   \
  "Инф.рег.пр.с.нет",   \
  "Ош.зап.и.р.пр.с.",   \
  "Ош.контр.р.пр.с.",   \
  " Ош.инф.сч.рес. ",   \
  " Инф.сч.рес.нет ",   \
  " Ош.зап.сч.рес. ",   \
  "Ош.контр.сч.рес.",   \
  " Ош.к.с.энергий ",   \
  " Энергий нет    ",   \
  " Ош.зап.энергий ",   \
  " Батарея разряж.",   \
  "Осцилятор остан.",   \
  "Отказ Осцилятора",   \
  " Ост.обновл.RTC ",   \
  " Не уст.поля RTC",   \
  " Тест VREF АЦП  ",   \
  " Ош.SPI АЦП     ",   \
  "Переп.буф.ц.осц.",   \
  " Ош.вых.реле ?.?",   \
  " Ош.вых.реле ?.?",   \
  " Ош.вых.реле ?.?",   \
  " Ош.вых.реле ?.?",   \
  " Ош.вых.реле ?.?",   \
  " Ош.вых.реле ?.?",   \
  " Ош.вых.реле ?.?",   \
  " Ош.вых.реле ?.?",   \
  " Ош.вых.реле ?.?",   \
  " Ош.вых.реле ?.?",   \
  " Ош.вых.реле ?.?",   \
  " Ош.вых.реле ?.?",   \
  " Ош.вых.реле ?.?",   \
  " Ош.вых.реле ?.?",   \
  " Ош.вых.реле ?.?",   \
  " Ош.вых.реле ?.?",   \
  " Ош.вых.реле ?.?",   \
  " Ош.вых.реле ?.?",   \
  " Ош.вых.реле ?.?",   \
  " Ош.вых.реле ?.?",   \
  " Ош.вых.реле ?.?",   \
  " Ош.вых.реле ?.?",   \
  " Ош.вых.реле ?.?",   \
  " Ош.вых.реле ?.?",   \
  " Ош.вых.ДШ      ",   \
  "Ан.рег.вр.занят.",   \
  " Пер.буф.aн.рег.",   \
  " П.aн.рег.исч.  ",   \
  "Неопр.ош.ан.рег.",   \
  " Д.рег.вр.занят.",   \
  " Неопр.ош.д.рег.",   \
  " Потеря д.д.рег.",   \
  "Переп.буф.пр.ош.",   \
  " Потеря д.пр.ош.",   \
  " Старт устр.    ",   \
  " Рестарт устр.  ",   \
  "Пр.Рестарт устр.",   \
  " Останов.устр.  ",   \
  " Пропад.питания ",   \
  " Отказ ЖКИ      ",   \
  " Ош.SPI_EDF     ",   \
  " Ош.внешней SRAM",   \
  " Ош.внутр.FLASH ",   \
  " Ош.выб.гр.уст. ",   \
  " Пот.д.энергии  ",   \
  " Тест ОВД1      ",   \
  " Тест ОВД2      ",   \
  " Тест ОВД3      ",   \
  " Тест ОВД4      ",   \
  "ЦП:Ош.приема к.1",   \
  "ЦП:Ош.пакета к.1",   \
  "ЦП:Ош.ответа к.1",   \
  " ЦП:Нет отв.к.1 ",   \
  "КП:Ош.приема к.1",   \
  "КП:Ош.пакета к.1",   \
  " КП:Ош.запр.к.1 ",   \
  " КП:Нет запр.к.1",   \
  "ЦП:Ош.приема к.2",   \
  "ЦП:Ош.пакета к.2",   \
  "ЦП:Ош.ответа к.2",   \
  " ЦП:Нет отв.к.2 ",   \
  "КП:Ош.приема к.2",   \
  "КП:Ош.пакета к.2",   \
  " КП:Ош.запр.к.2 ",   \
  " БА1 от.        ",   \
  " БА1 п.         ",   \
  " БДВВ5_1 от.    ",   \
  " БДВВ5_1 п.     ",   \
  " БДВВ5_2 от.    ",   \
  " БДВВ5_2 п.     ",   \
  " БДВ-ДЗ от.     ",   \
  " БДВ-ДЗ п.      ",   \
  " БДЗ от.        ",   \
  " БДЗ п.         ",   \
  " БДВВ6 от.      ",   \
  " БДВВ6 п.       ",   \
  " БДШ от.        ",   \
  " БДШ п.         ",   \
  " БДВВ9 от.      ",   \
  " БДВВ9 п.       ",   \
  " БДВ2  от.      ",   \
  " БДВ2  п.       ",   \
  " БДВ5  от.      ",   \
  " БДВ5  п.       ",   \
  " КП от.         ",   \
  " Ош.Ф.С.        ",   \
  " Рестарт КП     ",   \
  "Режим перепрогр.",   \
  " БДВВ-ДЗ от.    ",   \
  " БДВВ-ДЗ п.     "
    
#endif
