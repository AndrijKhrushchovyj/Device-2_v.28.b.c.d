#ifndef __CONST_MENU_DIAGNOSTYKA__
#define __CONST_MENU_DIAGNOSTYKA__

#define EKRAN_DIAGNOSTYKA (EKRAN_POINT_TIME_RANGUVANNJA + 1)

// clang-format off
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
  ERROR_ANGLE_EEPROM_BIT,
  ERROR_ANGLE_EEPROM_EMPTY_BIT,
  ERROR_ANGLE_EEPROM_COMPARISON_BIT,
  ERROR_ANGLE_EEPROM_CONTROL_BIT,
  ERROR_CALIBRATION_SELSYN,
  ERROR_NUMBER_STEP_NOMINAL,
  ERROR_RESURS_EEPROM_BIT,
  ERROR_RESURS_EEPROM_EMPTY_BIT,
  ERROR_RESURS_EEPROM_COMPARISON_BIT,
  ERROR_RESURS_EEPROM_CONTROL_BIT,

  RTC_BATTERY_LOW_BIT,
  RTC_OSCILLATOR_STOPED_BIT,
  RTC_OSCILLATOR_FAIL_BIT,
  RTC_UPDATING_HALTED_BIT,
  RTC_WORK_FIELD_NOT_SET_BIT,

  ERROR_VREF_ADC_TEST_BIT,
  ERROR_V_K_3_3_ADC_TEST_BIT,
  ERROR_VREF_ADC_TEST_COARSE_BIT,
  ERROR_V_K_3_3_ADC_TEST_COARSE_BIT,
  ERROR_SPI_ADC1_BIT,
  ERROR_SPI_ADC2_BIT,

  ERROR_DIGITAL_OUTPUT_1_BIT,

  ERROR_AR_TEMPORARY_BUSY_BIT = ERROR_DIGITAL_OUTPUT_1_BIT + 20,
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

  ERROR_LOGOMETR_VOLTAGE,

  ERROR_BA_1_FIX,
  ERROR_BA_1_CTLR,
  ERROR_BDVV5_1_FIX,
  ERROR_BDVV5_1_CTLR,
  ERROR_BDVV5_2_FIX,
  ERROR_BDVV5_2_CTLR,
  ERROR_BDVV6_FIX,
  ERROR_BDVV6_CTLR,

  ERROR_FATFS,
  WARNING_REPROGRAM,

  MAX_ROW_FOR_DIAGNOSTYKA
};

#define N_DIAGN ((MAX_ROW_FOR_DIAGNOSTYKA >> 5) + ((MAX_ROW_FOR_DIAGNOSTYKA & 0x1f) != 0))
#define N_DIAGN_BYTES ((MAX_ROW_FOR_DIAGNOSTYKA >> 3) + ((MAX_ROW_FOR_DIAGNOSTYKA & 0x07) != 0))

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
  | (1 << (ERROR_V_K_3_3_ADC_TEST_BIT - 32))                    \
  | (1 << (ERROR_VREF_ADC_TEST_COARSE_BIT - 32))                \
  | (1 << (ERROR_V_K_3_3_ADC_TEST_COARSE_BIT - 32))             \
  | (1 << (ERROR_SPI_ADC1_BIT - 32))                            \
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
)

#define MASKA_AVAR_ERROR_2        (unsigned int)(               \
    (1 << (ERROR_DIGITAL_OUTPUT_1_BIT + 12 - 64))               \
  | (1 << (ERROR_DIGITAL_OUTPUT_1_BIT + 13 - 64))               \
  | (1 << (ERROR_DIGITAL_OUTPUT_1_BIT + 14 - 64))               \
  | (1 << (ERROR_DIGITAL_OUTPUT_1_BIT + 15 - 64))               \
  | (1 << (ERROR_DIGITAL_OUTPUT_1_BIT + 16 - 64))               \
  | (1 << (ERROR_DIGITAL_OUTPUT_1_BIT + 17 - 64))               \
  | (1 << (ERROR_DIGITAL_OUTPUT_1_BIT + 18 - 64))               \
  | (1 << (ERROR_DIGITAL_OUTPUT_1_BIT + 19 - 64))               \
  | (1 << (ERROR_INTERNAL_FLASH_BIT - 64))                      \
  | (1 << (ERROR_BA_1_FIX - 64))                                \
  | (1 << (ERROR_BA_1_CTLR - 64))                               \
  | (1 << (ERROR_BDVV5_1_FIX - 64))                             \
  | (1 << (ERROR_BDVV5_1_CTLR - 64))                            \
)

#define MASKA_AVAR_ERROR_3        (unsigned int)(               \
    (1 << (ERROR_BDVV5_2_FIX - 96))                             \
  | (1 << (ERROR_BDVV5_2_CTLR - 96))                            \
  | (1 << (ERROR_BDVV6_FIX - 96))                               \
  | (1 << (ERROR_BDVV6_CTLR - 96))                              \
  | (1 << (WARNING_REPROGRAM - 96))                             \
)
// clang-format on

#define NAME_DIAGN_RU   \
  " Ош.I2C         ",   \
    " Ош.настроек    ", \
    " Настроек нет   ", \
    " Ош.зап.настр.  ", \
    " Ош.контр.настр.", \
    " Ош.типа настр. ", \
    " Ош.юстирования ", \
    " Юстирования нет", \
    " Ош.зап.юстир.  ", \
    " Ош.контр.юстир.", \
    " Ош.юстир.набора", \
    " Ош.воcст.тр.св ", \
    " Ош.воcст.с.вых ", \
    "Инф.вых./св.нет ", \
    " Ош.зап.вых./св.", \
    " Ош.Ин.ЭнНезСЛ  ", \
    " Ин.ЭнНезСЛ Нет ", \
    " Ош.З.Ин.ЭнНезСЛ", \
    " Ош.К.Ин.ЭнНезСЛ", \
    " Ош.инф.ан.рег. ", \
    " Инф.ан.рег.нет ", \
    "Ош.зап.и.ан.рег.", \
    "Ош.контр.ан.рег.", \
    " Ош.инф.д.рег.  ", \
    " Инф.д.рег.нет  ", \
    " Ош.зап.и.д.рег.", \
    " Ош.контр.д.рег.", \
    "Ош.инф.рег.пр.с.", \
    "Инф.рег.пр.с.нет", \
    "Ош.зап.и.р.пр.с.", \
    "Ош.контр.р.пр.с.", \
    " Ош.инф.угл.с.  ", \
    " Инф.угл.с.нет  ", \
    " Ош.зап.угл.с.  ", \
    " Ош.контр.угл.с.", \
    "  Ош.калибр.с.  ", \
    " Ош.номинал.пол.", \
    " Ош.инф.сч.рес. ", \
    " Инф.сч.рес.нет ", \
    " Ош.зап.сч.рес. ", \
    "Ош.контр.сч.рес.", \
    " Батарея разряж.", \
    "Осцилятор остан.", \
    "Отказ Осцилятора", \
    " Ост.обновл.RTC ", \
    " Не уст.поля RTC", \
    " Тест VREF АЦП  ", \
    " Тест VDD АЦП   ", \
    "Тест VREF АЦП гр", \
    " Тест VDD АЦП гр", \
    " Ош.SPI АЦП1    ", \
    " Ош.SPI АЦП2    ", \
    " Ош.вых.реле ?.?", \
    " Ош.вых.реле ?.?", \
    " Ош.вых.реле ?.?", \
    " Ош.вых.реле ?.?", \
    " Ош.вых.реле ?.?", \
    " Ош.вых.реле ?.?", \
    " Ош.вых.реле ?.?", \
    " Ош.вых.реле ?.?", \
    " Ош.вых.реле ?.?", \
    " Ош.вых.реле ?.?", \
    " Ош.вых.реле ?.?", \
    " Ош.вых.реле ?.?", \
    " Ош.вых.реле ?.?", \
    " Ош.вых.реле ?.?", \
    " Ош.вых.реле ?.?", \
    " Ош.вых.реле ?.?", \
    " Ош.вых.реле ?.?", \
    " Ош.вых.реле ?.?", \
    " Ош.вых.реле ?.?", \
    " Ош.вых.реле ?.?", \
    "Ан.рег.вр.занят.", \
    " Пер.буф.aн.рег.", \
    " П.aн.рег.исч.  ", \
    "Неопр.ош.ан.рег.", \
    " Д.рег.вр.занят.", \
    " Неопр.ош.д.рег.", \
    " Потеря д.д.рег.", \
    "Переп.буф.пр.ош.", \
    " Потеря д.пр.ош.", \
    " Старт устр.    ", \
    " Рестарт устр.  ", \
    "Пр.Рестарт устр.", \
    " Останов.устр.  ", \
    " Пропад.питания ", \
    " Отказ ЖКИ      ", \
    " Ош.SPI_EDF     ", \
    " Ош.внешней SRAM", \
    " Ош.внутр.FLASH ", \
    " Ош.выб.гр.уст. ", \
    " Ош.напр.логом. ", \
    " БА1 от.        ", \
    " БА1 п.         ", \
    " БДВВ5_1 от.    ", \
    " БДВВ5_1 п.     ", \
    " БДВВ5_2 от.    ", \
    " БДВВ5_2 п.     ", \
    " БДВВ6 от.      ", \
    " БДВВ6 п.       ", \
    " Ош.Ф.С.        ", \
    "Режим перепрогр."

#define NAME_DIAGN_UA   \
  " Пом.I2C        ",   \
    " Пом.налаштувань", \
    "Налаштувань нема", \
    " Пом.зап.налашт.", \
    "Пом.контр.налашт", \
    " Пом.типу налашт", \
    " Пом.юстування  ", \
    " Юстування нема ", \
    " Пом.зап.юст.   ", \
    " Пом.контр.юст. ", \
    " Пом.юст.набору ", \
    " Пом.відн.тр.св.", \
    " Пом.відн.с.вих.", \
    "Інф.вих./св.нема", \
    "Пом.зап.вих./св.", \
    " Пом.Ін.ЕнНезСЛ ", \
    " Ін.ЕнНезСЛ Нема", \
    "Пом.З.Ін.ЕнНезСЛ", \
    "Пом.К.Ін.ЕнНезСЛ", \
    " Пом.інф.ан.р.  ", \
    " Інф.ан.р.нема  ", \
    " Пом.зап.і.ан.р.", \
    " Пом.контр.ан.р.", \
    " Пом.інф.д.р.   ", \
    " Інф.д.р.нема   ", \
    " Пом.зап.і.д.р. ", \
    " Пом.контр.д.р. ", \
    " Пом.інф.р.пр.п.", \
    " Інф.р.пр.п.нема", \
    "Пом.зап.і.р.пр.п", \
    "Пом.контр.р.пр.п", \
    " Пом.інф.кyт.с. ", \
    " Інф.кут.с.нема ", \
    " Пом.зап.кут.с. ", \
    "Пом.контр.кут.с.", \
    " Пом.калібр.с.  ", \
    "Пом.номінал.пол.", \
    " Пом.інф.ліч.р. ", \
    " Інф.ліч.р.нема ", \
    " Пом.зап.ліч.р. ", \
    "Пом.контр.ліч.р.", \
    "Батарея розрядж.", \
    " Осцилятор зуп. ", \
    " Відм.Осцилятора", \
    " Зуп.обновл.RTC ", \
    " Не вст.поля RTC", \
    " Тест VREF АЦП  ", \
    " Тест VDD АЦП   ", \
    "Тест VREF АЦП гр", \
    " Тест VDD АЦП гр", \
    " Пом.SPI АЦП1   ", \
    " Пом.SPI АЦП2   ", \
    " Пом.вих.реле?.?", \
    " Пом.вих.реле?.?", \
    " Пом.вих.реле?.?", \
    " Пом.вих.реле?.?", \
    " Пом.вих.реле?.?", \
    " Пом.вих.реле?.?", \
    " Пом.вих.реле?.?", \
    " Пом.вих.реле?.?", \
    " Пом.вих.реле?.?", \
    " Пом.вих.реле?.?", \
    " Пом.вих.реле?.?", \
    " Пом.вих.реле?.?", \
    " Пом.вих.реле?.?", \
    " Пом.вих.реле?.?", \
    " Пом.вих.реле?.?", \
    " Пом.вих.реле?.?", \
    " Пом.вих.реле?.?", \
    " Пом.вих.реле?.?", \
    " Пом.вих.реле?.?", \
    " Пом.вих.реле?.?", \
    "Ан.р.тимч.зайнят", \
    " Переп.буф.aн.р.", \
    " П.aн.рег.вич.  ", \
    "Невизн.пом.ан.р.", \
    "Д.р.тимч.зайнят.", \
    " Невизн.пом.д.р.", \
    " Втрата д.д.р.  ", \
    "Переп.буф.р.пр.п", \
    " Втрата д.р.пр.п", \
    " Старт пристр.  ", \
    " Рестарт пристр.", \
    " Пр.Рестарт пр. ", \
    " Зуп.пристр.    ", \
    " Пропад.живлення", \
    " Відмова РКІ    ", \
    " Пом.SPI_EDF    ", \
    " Пом.зовн.SRAM  ", \
    " Пом.внутр.FLASH", \
    " Пом.виб.гр.уст.", \
    " Пом.напр.логом.", \
    " БА1 від.       ", \
    " БА1 п.         ", \
    " БДВВ5_1 від.   ", \
    " БДВВ5_1 п.     ", \
    " БДВВ5_2 від.   ", \
    " БДВВ5_2 п.     ", \
    " БДВВ6 від.     ", \
    " БДВВ6 п.       ", \
    " Пом.Ф.С.       ", \
    "Режим перепрогр."

#define NAME_DIAGN_EN   \
  " I2C Er         ",   \
    " Settings Er    ", \
    " No settings    ", \
    " Sett W Er      ", \
    " Sett Ctl Er    ", \
    " Sett Type Er   ", \
    " Adjust Er      ", \
    " No adjust      ", \
    " Adjust W Er    ", \
    "Adjust Ctl Er   ", \
    " Adjust-Set Er  ", \
    "Latc LED Rest Er", \
    " Latc BO Rest Er", \
    " No BO/LED Inf  ", \
    " BO/LED W Er    ", \
    " Nvol LS Inf Er ", \
    " No Nvol LS Inf ", \
    "Nvol LS Inf W Er", \
    "Nvol LS Inf C Er", \
    " Dst Rec Er     ", \
    " Dst Rec No info", \
    " Dst Rec W Er   ", \
    " Dst Rec Ctr Er ", \
    " Bin Rec Er     ", \
    " Bin Rec No info", \
    " Bin Rec W Er   ", \
    " Bin Rec Ctr Er ", \
    " Diag Rec Er    ", \
    "Diag Rec No info", \
    " Diag Rec W Er  ", \
    "Diag Rec Ctr Er ", \
    " S.Angle Inf.Err", \
    " No S.Angle Inf.", \
    " S.Angle W.Err. ", \
    "S.Angle Ctrl.Err", \
    " Selsyn Cal.Err.", \
    " Rated Pos.Err. ", \
    " CB Res Coun Er ", \
    "CB Res Ctr No In", \
    " CB Res Ctr W Er", \
    " CB R Cnt Ctr Er", \
    " RTC:Battery low", \
    " RTC:Osc stop   ", \
    " RTC:Osc fail   ", \
    " RTC:Halt update", \
    "RTC:No def sett ", \
    " ADC:VREF fail  ", \
    " ADC:VDD fail   ", \
    "ADC:VREF fail R.", \
    " ADC:VDD fail R.", \
    " ADC1 SPI Err.  ", \
    " ADC2 SPI Err.  ", \
    " BO?.? Ctl Er   ", \
    " BO?.? Ctl Er   ", \
    " BO?.? Ctl Er   ", \
    " BO?.? Ctl Er   ", \
    " BO?.? Ctl Er   ", \
    " BO?.? Ctl Er   ", \
    " BO?.? Ctl Er   ", \
    " BO?.? Ctl Er   ", \
    " BO?.? Ctl Er   ", \
    " BO?.? Ctl Er   ", \
    " BO?.? Ctl Er   ", \
    " BO?.? Ctl Er   ", \
    " BO?.? Ctl Er   ", \
    " BO?.? Ctl Er   ", \
    " BO?.? Ctl Er   ", \
    " BO?.? Ctl Er   ", \
    " BO?.? Ctl Er   ", \
    " BO?.? Ctl Er   ", \
    " BO?.? Ctl Er   ", \
    " BO?.? Ctl Er   ", \
    " Dst Rec Busy   ", \
    " Dst Rec Buf Ovf", \
    "Dst Rec Mem Full", \
    "UnDef Dst Rec Er", \
    " Bin Rec busy   ", \
    "UnDef Bin Rec Er", \
    "Bin Rec Dat Lost", \
    "OVF of PER buff ", \
    " PER Dat lost   ", \
    " Device Start   ", \
    " Device Restart ", \
    " Device SFTRST  ", \
    " Device Stop    ", \
    " Power Outage   ", \
    " LCD Fail       ", \
    " EDF SPI Er     ", \
    " Ext SRAM Er    ", \
    " Flash Mem Er   ", \
    " SP Gr Sel Er   ", \
    " Logometer V.Err", \
    " AIU01 abs      ", \
    " AIU01 ver      ", \
    " BIOU05_1 abs   ", \
    " BIOU05_1 ver   ", \
    " BIOU05_2 abs   ", \
    " BIOU05_2 ver   ", \
    " BIOU06 abs     ", \
    " BIOU06 ver     ", \
    " F.S.Error      ", \
    " Reprogram Mode "

#define NAME_DIAGN_KZ   \
  " Ош.I2C         ",   \
    " Ош.настроек    ", \
    " Настроек нет   ", \
    " Ош.зап.настр.  ", \
    " Ош.контр.настр.", \
    " Ош.типа настр. ", \
    " Ош.юстирования ", \
    " Юстирования нет", \
    " Ош.зап.юстир.  ", \
    " Ош.контр.юстир.", \
    " Ош.юстир.набора", \
    " Ош.воcст.тр.св ", \
    " Ош.воcст.с.вых ", \
    "Инф.вых./св.нет ", \
    " Ош.зап.вых./св.", \
    "  Ош.Ин.ЭнНезСЛ ", \
    " Ин.ЭнНезСЛ Нет ", \
    " Ош.З.Ин.ЭнНезСЛ", \
    " Ош.К.Ин.ЭнНезСЛ", \
    " Ош.инф.ан.рег. ", \
    " Инф.ан.рег.нет ", \
    "Ош.зап.и.ан.рег.", \
    "Ош.контр.ан.рег.", \
    " Ош.инф.д.рег.  ", \
    " Инф.д.рег.нет  ", \
    " Ош.зап.и.д.рег.", \
    " Ош.контр.д.рег.", \
    "Ош.инф.рег.пр.с.", \
    "Инф.рег.пр.с.нет", \
    "Ош.зап.и.р.пр.с.", \
    "Ош.контр.р.пр.с.", \
    " Ош.инф.угл.с.  ", \
    " Инф.угл.с.нет  ", \
    " Ош.зап.угл.с.  ", \
    " Ош.контр.угл.с.", \
    "  Ош.калибр.с.  ", \
    " Ош.номинал.пол.", \
    " Ош.инф.сч.рес. ", \
    " Инф.сч.рес.нет ", \
    " Ош.зап.сч.рес. ", \
    "Ош.контр.сч.рес.", \
    " Батарея разряж.", \
    "Осцилятор остан.", \
    "Отказ Осцилятора", \
    " Ост.обновл.RTC ", \
    " Не уст.поля RTC", \
    " Тест VREF АЦП  ", \
    " Тест VDD АЦП   ", \
    "Тест VREF АЦП гр", \
    " Тест VDD АЦП гр", \
    " Ош.SPI АЦП1    ", \
    " Ош.SPI АЦП2    ", \
    " Ош.вых.реле ?.?", \
    " Ош.вых.реле ?.?", \
    " Ош.вых.реле ?.?", \
    " Ош.вых.реле ?.?", \
    " Ош.вых.реле ?.?", \
    " Ош.вых.реле ?.?", \
    " Ош.вых.реле ?.?", \
    " Ош.вых.реле ?.?", \
    " Ош.вых.реле ?.?", \
    " Ош.вых.реле ?.?", \
    " Ош.вых.реле ?.?", \
    " Ош.вых.реле ?.?", \
    " Ош.вых.реле ?.?", \
    " Ош.вых.реле ?.?", \
    " Ош.вых.реле ?.?", \
    " Ош.вых.реле ?.?", \
    " Ош.вых.реле ?.?", \
    " Ош.вых.реле ?.?", \
    " Ош.вых.реле ?.?", \
    " Ош.вых.реле ?.?", \
    "Ан.рег.вр.занят.", \
    " Пер.буф.aн.рег.", \
    " П.aн.рег.исч.  ", \
    "Неопр.ош.ан.рег.", \
    " Д.рег.вр.занят.", \
    " Неопр.ош.д.рег.", \
    " Потеря д.д.рег.", \
    "Переп.буф.пр.ош.", \
    " Потеря д.пр.ош.", \
    " Старт устр.    ", \
    " Рестарт устр.  ", \
    "Пр.Рестарт устр.", \
    " Останов.устр.  ", \
    " Пропад.питания ", \
    " Отказ ЖКИ      ", \
    " Ош.SPI_EDF     ", \
    " Ош.внешней SRAM", \
    " Ош.внутр.FLASH ", \
    " Ош.выб.гр.уст. ", \
    " Ош.напр.логом. ", \
    " БА1 от.        ", \
    " БА1 п.         ", \
    " БДВВ5_1 от.    ", \
    " БДВВ5_1 п.     ", \
    " БДВВ5_2 от.    ", \
    " БДВВ5_2 п.     ", \
    " БДВВ6 от.      ", \
    " БДВВ6 п.       ", \
    " Ош.Ф.С.        ", \
    "Режим перепрогр."

#endif
