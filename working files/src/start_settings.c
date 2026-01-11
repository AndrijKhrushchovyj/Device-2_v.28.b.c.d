#include "header.h"

/**************************************/
//Тестування зовнішньої SRAM
/**************************************/
inline void test_external_SRAM(void)
{
  //Визнапчаємо кількість двохбайтих слів
  unsigned int size_SRAM_word = (((unsigned int) __section_size("variables_RAM1") /*&__ICFEDIT_region_RAM1_size__*/) + 1) >> 1;

  //Визначаємо вказівник на початок зовнішньої оперативної пам'яті
  unsigned short int *point = ((unsigned short int *) __section_begin("variables_RAM1") /*&__ICFEDIT_region_RAM1_start__*/);

  //Заповнюємо кожну комірку зовнішьої оперативної пам'яті її адресою
  for (unsigned int i = 0; i < size_SRAM_word; i++)
    *point++ = (unsigned short int) (i & 0xffff);

  //Перевіряємо зчитуванням, чи у всіх комірках прописані ті числа, які ми попередньо записали
  unsigned int error = 0, i = 0;
  point = ((unsigned short int *) __section_begin("variables_RAM1") /*&__ICFEDIT_region_RAM1_start__*/);
  while ((i < size_SRAM_word) && (error == 0))
  {
    if ((*point) == ((unsigned short int) (i & 0xffff)))
    {
      //Тест даної комірки пройшов вдало
      i++;
      *point++ = 0;
    }
    else
    {
      //Тест даної комірки не пройшов вдало

      error = 0xff;
      //Виставляємо повідомлення про помилку тесту зовнішьої оперативної пам'яті
      _SET_BIT(set_diagnostyka, ERROR_EXTERNAL_SRAM_BIT);
    }
  }

#ifdef EXT_SRAM_512
  if (error == 0)
  {
    point = (unsigned short int *) (&__ICFEDIT_region_RAM1_start__);
    size_t size = (size_t)(&__ICFEDIT_region_RAM1_size__) + 1;
    uint16_t *pointTmp = point + (size >> 2); /*раз ділимо, щоб обтимати середину пам'яті, а другий раз ділю, бо  вказівник вказує на двобайтне число*/
    *point = 0x0;
    *pointTmp = 0xaa55;
    if (*point != 0x0)
    {
      error = 0xff;
      //Виставляємо повідомлення про помилку тесту зовнішьої оперативної пам'яті
      _SET_BIT(set_diagnostyka, ERROR_EXTERNAL_SRAM_BIT);
    }
    else
      *pointTmp = 0;
  }
#endif
}
/**************************************/

/**************************************/
//Стартова ініціалізація змінниз
/**************************************/
void global_vareiables_installation(void)
{
  /**************************/
  //Вимірювальна система
  /**************************/

  for (size_t i = 0; i < NUMBER_ANALOG_CANALES; i++)
  {
    for (unsigned int j = 0; j < MAIN_FREQUENCY; j++)
      vref_adc_moment_value_1s[i][j] = VREF_NORMAL_VALUE;
  }

  for (unsigned int i = 0; i < MAX_INDEX_DATA_FOR_OSCYLOGRAPH; i++)
  {
    data_for_oscylograph[i].state_ar_record = STATE_AR_NONE_M;
  }

  sector_1[0] = (int) (AMPLITUDA_SECTOR * /*cos*/ arm_cos_f32(/*(double)*/ (PI * ((float) (0 + SECTOR1 - POPRAVKA_NZZ)) / 180.0f)));
  sector_1[1] = (int) (AMPLITUDA_SECTOR * /*sin*/ arm_sin_f32(/*(double)*/ (PI * ((float) (0 + SECTOR1 - POPRAVKA_NZZ)) / 180.0f)));
  sector_1[2] = (int) (AMPLITUDA_SECTOR * /*cos*/ arm_cos_f32(/*(double)*/ (PI * ((float) (180 - SECTOR1 - POPRAVKA_NZZ)) / 180.0f)));
  sector_1[3] = (int) (AMPLITUDA_SECTOR * /*sin*/ arm_sin_f32(/*(double)*/ (PI * ((float) (180 - SECTOR1 - POPRAVKA_NZZ)) / 180.0f)));
  sector_1[4] = (int) (AMPLITUDA_SECTOR * /*cos*/ arm_cos_f32(/*(double)*/ (PI * ((float) (180 + SECTOR1 - POPRAVKA_NZZ)) / 180.0f)));
  sector_1[5] = (int) (AMPLITUDA_SECTOR * /*sin*/ arm_sin_f32(/*(double)*/ (PI * ((float) (180 + SECTOR1 - POPRAVKA_NZZ)) / 180.0f)));
  sector_1[6] = (int) (AMPLITUDA_SECTOR * /*cos*/ arm_cos_f32(/*(double)*/ (PI * ((float) (360 - SECTOR1 - POPRAVKA_NZZ)) / 180.0f)));
  sector_1[7] = (int) (AMPLITUDA_SECTOR * /*sin*/ arm_sin_f32(/*(double)*/ (PI * ((float) (360 - SECTOR1 - POPRAVKA_NZZ)) / 180.0f)));

  sector_2[0] = (int) (AMPLITUDA_SECTOR * /*cos*/ arm_cos_f32(/*(double)*/ (PI * ((float) (0 + SECTOR2 - POPRAVKA_NZZ)) / 180.0f)));
  sector_2[1] = (int) (AMPLITUDA_SECTOR * /*sin*/ arm_sin_f32(/*(double)*/ (PI * ((float) (0 + SECTOR2 - POPRAVKA_NZZ)) / 180.0f)));
  sector_2[2] = (int) (AMPLITUDA_SECTOR * /*cos*/ arm_cos_f32(/*(double)*/ (PI * ((float) (180 - SECTOR2 - POPRAVKA_NZZ)) / 180.0f)));
  sector_2[3] = (int) (AMPLITUDA_SECTOR * /*sin*/ arm_sin_f32(/*(double)*/ (PI * ((float) (180 - SECTOR2 - POPRAVKA_NZZ)) / 180.0f)));
  sector_2[4] = (int) (AMPLITUDA_SECTOR * /*cos*/ arm_cos_f32(/*(double)*/ (PI * ((float) (180 + SECTOR2 - POPRAVKA_NZZ)) / 180.0f)));
  sector_2[5] = (int) (AMPLITUDA_SECTOR * /*sin*/ arm_sin_f32(/*(double)*/ (PI * ((float) (180 + SECTOR2 - POPRAVKA_NZZ)) / 180.0f)));
  sector_2[6] = (int) (AMPLITUDA_SECTOR * /*cos*/ arm_cos_f32(/*(double)*/ (PI * ((float) (360 - SECTOR2 - POPRAVKA_NZZ)) / 180.0f)));
  sector_2[7] = (int) (AMPLITUDA_SECTOR * /*sin*/ arm_sin_f32(/*(double)*/ (PI * ((float) (360 - SECTOR2 - POPRAVKA_NZZ)) / 180.0f)));

  sector_1_mtz_tznp[0] = (int) (AMPLITUDA_SECTOR * /*cos*/ arm_cos_f32(/*(double)*/ (PI * ((float) (90 - SECTOR1_MTZ_TZNP - POPRAVKA_MTZ_TZNP)) / 180.0f)));
  sector_1_mtz_tznp[1] = (int) (AMPLITUDA_SECTOR * /*sin*/ arm_sin_f32(/*(double)*/ (PI * ((float) (90 - SECTOR1_MTZ_TZNP - POPRAVKA_MTZ_TZNP)) / 180.0f)));
  sector_1_mtz_tznp[2] = (int) (AMPLITUDA_SECTOR * /*cos*/ arm_cos_f32(/*(double)*/ (PI * ((float) (90 + SECTOR1_MTZ_TZNP - POPRAVKA_MTZ_TZNP)) / 180.0f)));
  sector_1_mtz_tznp[3] = (int) (AMPLITUDA_SECTOR * /*sin*/ arm_sin_f32(/*(double)*/ (PI * ((float) (90 + SECTOR1_MTZ_TZNP - POPRAVKA_MTZ_TZNP)) / 180.0f)));
  sector_1_mtz_tznp[4] = (int) (AMPLITUDA_SECTOR * /*cos*/ arm_cos_f32(/*(double)*/ (PI * ((float) (-90 - SECTOR1_MTZ_TZNP - POPRAVKA_MTZ_TZNP)) / 180.0f)));
  sector_1_mtz_tznp[5] = (int) (AMPLITUDA_SECTOR * /*sin*/ arm_sin_f32(/*(double)*/ (PI * ((float) (-90 - SECTOR1_MTZ_TZNP - POPRAVKA_MTZ_TZNP)) / 180.0f)));
  sector_1_mtz_tznp[6] = (int) (AMPLITUDA_SECTOR * /*cos*/ arm_cos_f32(/*(double)*/ (PI * ((float) (-90 + SECTOR1_MTZ_TZNP - POPRAVKA_MTZ_TZNP)) / 180.0f)));
  sector_1_mtz_tznp[7] = (int) (AMPLITUDA_SECTOR * /*sin*/ arm_sin_f32(/*(double)*/ (PI * ((float) (-90 + SECTOR1_MTZ_TZNP - POPRAVKA_MTZ_TZNP)) / 180.0f)));

  sector_2_mtz_tznp[0] = (int) (AMPLITUDA_SECTOR * /*cos*/ arm_cos_f32(/*(double)*/ (PI * ((float) (90 - SECTOR2_MTZ_TZNP - POPRAVKA_MTZ_TZNP)) / 180.0f)));
  sector_2_mtz_tznp[1] = (int) (AMPLITUDA_SECTOR * /*sin*/ arm_sin_f32(/*(double)*/ (PI * ((float) (90 - SECTOR2_MTZ_TZNP - POPRAVKA_MTZ_TZNP)) / 180.0f)));
  sector_2_mtz_tznp[2] = (int) (AMPLITUDA_SECTOR * /*cos*/ arm_cos_f32(/*(double)*/ (PI * ((float) (90 + SECTOR2_MTZ_TZNP - POPRAVKA_MTZ_TZNP)) / 180.0f)));
  sector_2_mtz_tznp[3] = (int) (AMPLITUDA_SECTOR * /*sin*/ arm_sin_f32(/*(double)*/ (PI * ((float) (90 + SECTOR2_MTZ_TZNP - POPRAVKA_MTZ_TZNP)) / 180.0f)));
  sector_2_mtz_tznp[4] = (int) (AMPLITUDA_SECTOR * /*cos*/ arm_cos_f32(/*(double)*/ (PI * ((float) (-90 - SECTOR2_MTZ_TZNP - POPRAVKA_MTZ_TZNP)) / 180.0f)));
  sector_2_mtz_tznp[5] = (int) (AMPLITUDA_SECTOR * /*sin*/ arm_sin_f32(/*(double)*/ (PI * ((float) (-90 - SECTOR2_MTZ_TZNP - POPRAVKA_MTZ_TZNP)) / 180.0f)));
  sector_2_mtz_tznp[6] = (int) (AMPLITUDA_SECTOR * /*cos*/ arm_cos_f32(/*(double)*/ (PI * ((float) (-90 + SECTOR2_MTZ_TZNP - POPRAVKA_MTZ_TZNP)) / 180.0f)));
  sector_2_mtz_tznp[7] = (int) (AMPLITUDA_SECTOR * /*sin*/ arm_sin_f32(/*(double)*/ (PI * ((float) (-90 + SECTOR2_MTZ_TZNP - POPRAVKA_MTZ_TZNP)) / 180.0f)));
  /**************************/

  /**************************/
  //Ініціалізація глобальних таймерів
  /**************************/
  for (int *p = global_timers; p != (global_timers + _MAX_NUMBER_GLOBAL_TIMERS); ++p)
    *p = -1;
  /**************************/

  /**************************/
  //Змінні, які потрібні для дискретного реєстратора
  /**************************/
  for (size_t row = 0; row < SIZE_QUEUE_DR; row++)
  {
    unsigned char *buffer_for_save_dr_record = queue_dr[row];
    for (size_t i = 0; i < SIZE_BUFFER_FOR_DR_RECORD; ++i)
    {
      *buffer_for_save_dr_record++ = 0xff;
    }
  }
  /**************************/

  for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
  {
    for (unsigned int j = 0; j < MAX_COL_LCD; j++)
      working_ekran[i][j] = ' ';
  }

  for (unsigned int i = 0; i < MAX_LEVEL_MENU; i++)
  {
    if ((i == EKRAN_LEVEL_PASSWORD) ||
        (i == EKRAN_LEVEL_SET_NEW_PASSWORD1) ||
        (i == EKRAN_LEVEL_SET_NEW_PASSWORD2) ||
        (i == EKRAN_LEVEL_SET_NEW_PASSWORD3))
      position_in_current_level_menu[i] = INDEX_LINE_NUMBER_1_FOR_LEVEL_PASSWORD;
    else
      position_in_current_level_menu[i] = 0;
    previous_level_in_current_level_menu[i] = -1;
  }

  //Визначення початкового стану екрану
  current_ekran.current_level = EKRAN_MAIN;
  current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
  current_ekran.position_cursor_y = current_ekran.index_position;

  /**************************/
  //Ініціалізація структури усправління драйвером I2C
  /**************************/
  //Стан обміну
  driver_i2c.state_execution = -1;
  //Ідентифікатор пристрою, з яким ведетьсяобмін
  driver_i2c.device_id = -1;
  //Код операції
  driver_i2c.action = -1;
  //Значення решти полів поки не грає ролі
  /**************************/

  //Modbus-RTU
  type_of_settings_changed = 0;

  number_bits_rs_485_waiting = 0;

  // //Ініціалізація Ігоревих глобальних змінних
  // pointInterface = 0;

  // extern int sizeOutputPacket;
  // extern  unsigned char *inputPacket;
  // extern  int *received_count;
  // extern  int globalcntBit;//к-во бит
  // extern  int globalcntReg;//к-во регистров
  // extern  int globalbeginAdrReg;//адрес нач регистра
  // extern  int globalbeginAdrBit;//адрес нач бит
  // extern  int upravlSetting;//флаг Setting
  // extern  int upravlSchematic;//флаг Shematic

  // sizeOutputPacket = 0;
  // globalcntBit  = 0;//к-во бит
  // globalcntReg  = 0;//к-во регистров
  // globalbeginAdrReg  = 0;//адрес нач регистра
  // globalbeginAdrBit  = 0;//адрес нач бит
  // upravlSetting=0;//флаг Setting
  // upravlSchematic=0;//флаг Shematic
}
/**************************************/

/**************************************/
//Стартова ініціалізація периферії процесора
/**************************************/
void start_settings_peripherals(void)
{
  /**********************/
  //Настроювання мікроконтролера: ядра + периферії
  /**********************/
  Set_System();
  /**********************/

  /**********************/
  //Настроюємо DBG модуль
  /**********************/
  DBGMCU_APB1PeriphConfig(DBGMCU_TIM2_STOP | DBGMCU_TIM5_STOP | DBGMCU_TIM4_STOP | DBGMCU_I2C1_SMBUS_TIMEOUT, ENABLE);
  /**********************/

  /**********************/
  //Настроювання переривань
  /**********************/
  Interrupts_Config();
  /**********************/

  /**********************/
  //Настроювання зовнішню шину
  /**********************/
  FSMC_SRAM_Init();

  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37) = 0;
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
  (MODYFIKACIA_VERSII_PZ == 23) || \
  (MODYFIKACIA_VERSII_PZ == 24) || \
  (MODYFIKACIA_VERSII_PZ == 26) || \
  (MODYFIKACIA_VERSII_PZ == 33) || \
  (MODYFIKACIA_VERSII_PZ == 34))
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD25_DD27_DD28_DD30) = 0;
#endif
  /**********************/

  /**********************/
  //Настроювання пінів процесора
  /**********************/
  GPIO_InitTypeDef GPIO_InitStructure;

  /**************/
  //Піни на вивід
  /**************/
  /* Пін зовнішнього Watchdog, як Output push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_PIN_EXTERNAL_WATCHDOG;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIO_EXTERNAL_WATCHDOG, &GPIO_InitStructure);
  /* Знімаємо пін зовнішнього Watchdog */
  GPIO_SetBits(GPIO_EXTERNAL_WATCHDOG, GPIO_PIN_EXTERNAL_WATCHDOG);

  /*
  Виконуємо стартову ініціалізацію змінних? а потім скидаємо Watchdog
  Після цього продовжуємо настройку периферії
  
  Це потрібно, щоб уникнути перезапуску приладу
  */
  /**********************/
  //Тестуємо зовнішню SRAM
  /**********************/
  test_external_SRAM();
  /**********************/

  //Змінюємо стан біту зовнішнього Watchdog на протилежний
  GPIO_WriteBit(
    GPIO_EXTERNAL_WATCHDOG,
    GPIO_PIN_EXTERNAL_WATCHDOG,
    (BitAction)(1 - GPIO_ReadOutputDataBit(GPIO_EXTERNAL_WATCHDOG, GPIO_PIN_EXTERNAL_WATCHDOG)));

  /**********************/
  //Стартова ініціалізація змінних у зовнішіній і у внітрішній SRAM
  /**********************/
  global_vareiables_installation();
  /**********************/

  //Змінюємо стан біту зовнішнього Watchdog на протилежний
  GPIO_WriteBit(
    GPIO_EXTERNAL_WATCHDOG,
    GPIO_PIN_EXTERNAL_WATCHDOG,
    (BitAction)(1 - GPIO_ReadOutputDataBit(GPIO_EXTERNAL_WATCHDOG, GPIO_PIN_EXTERNAL_WATCHDOG)));
  /*****/

  /*
  Продовжуємо виконувати ініціалізацію периферії  
  */

  /* Конфігурація піну CON-L, як Output push-pull */
  GPIO_InitStructure.GPIO_Pin = CON_L_PIN;
  GPIO_Init(CON_L, &GPIO_InitStructure);
  /* Скидаємо пін CON-L, щоб покищо управляти свтоіндикаторами не можна було*/
  GPIO_ResetBits(CON_L, CON_L_PIN);

  /* Конфігурація піну CON-OUTPUTS, як Output push-pull */
  GPIO_InitStructure.GPIO_Pin = CON_1_OUTPUTS_PIN;
  GPIO_Init(CON_OUTPUTS, &GPIO_InitStructure);
  /* Виставляємо пін CON-OUTPUTS-1*/
  GPIO_SetBits(CON_OUTPUTS, CON_1_OUTPUTS_PIN);

  GPIO_InitStructure.GPIO_Pin = CON_2_OUTPUTS_PIN;
  GPIO_Init(CON_OUTPUTS, &GPIO_InitStructure);
  /* Знімаємо пін CON-OUTPUTS-2*/
  GPIO_ResetBits(CON_OUTPUTS, CON_2_OUTPUTS_PIN);

  GPIO_InitStructure.GPIO_Pin = CON_3_OUTPUTS_PIN;
  GPIO_Init(CON_OUTPUTS, &GPIO_InitStructure);
  /* Виставляємо пін CON-OUTPUTS-3*/
  GPIO_SetBits(CON_OUTPUTS, CON_3_OUTPUTS_PIN);
#if (                             \
  (MODYFIKACIA_VERSII_PZ == 8) || \
  (MODYFIKACIA_VERSII_PZ == 18))
  //`-
  //..  *	Записати «0» в регістри дискретних виходів
  //..  REL1-REL8 (DD31), REL9-REL16 (DD35), REL17-REL23
  //..  (DD25), AOUT1-AOUT8 (DD28), CHDO1-CHDO7 (DD39);
  //..  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37) = 0;Line == 233
  //.  ._DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD25_DD27_DD28_DD30) = 0;Line == 243

  //..  *	Для дозволу роботи дискретних виходів REL1-REL8
  //..  (DD31), REL9-REL16 (DD35), REL17-REL23 (DD25),
  //..  AOUT1-AOUT8 (DD28), CHDO1-CHDO7 (DD39)  блоку
  //..  БВ3
  //..  встановити сигнали CON1, CON2, CON3 в стан в
  //..  відповідності з таблицею 3.28 ТЗ (ст.42), при
  //..  цьому
  //..  на виходах регістрів буде «0»;
  //^-
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37) = 0;      //Установити REL4 в «0», REL6 в «0»;
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37) = 0x14;   //Установити REL3, REL5 в «1»;
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37) = 0x1c;   //Установити REL4 в «1» (по  перепаду «0» - «1» записується «0» у виходи 1-9);
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37) = 0x14;   //Установити REL4 в «0»;
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37) = 0x34;   //Установити  REL6 в «1» (по  перепаду «0» - «1» записується «0» у виходи 10-18);
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37) = 0x14;   //Установити REL6 в «0»;
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37) = 0;      //Установити REL3, REL5 в «0»;
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37) = 0x8000; //Установити REL16 в «1»
  chGbl__REL1_REL8__W_VAL = 0;
  chGbl__CDHO1_CDHO7__W_VAL = 0;
#endif
  /***
  Виконуємо контроль приєднання плат (CON тут вже встановлений)
  ***/
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD39_DD40_DD47) = 0;
  uint32_t board_register_tmp = board_register = _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD39_DD40_DD47);
#if (                              \
  (MODYFIKACIA_VERSII_PZ == 0) ||  \
  (MODYFIKACIA_VERSII_PZ == 3) ||  \
  (MODYFIKACIA_VERSII_PZ == 5) ||  \
  (MODYFIKACIA_VERSII_PZ == 6) ||  \
  (MODYFIKACIA_VERSII_PZ == 7) ||  \
  (MODYFIKACIA_VERSII_PZ == 8) ||  \
  (MODYFIKACIA_VERSII_PZ == 23) || \
  (MODYFIKACIA_VERSII_PZ == 26))
  if ((board_register_tmp & 0x17) != 0x17)
#elif ( \
  (MODYFIKACIA_VERSII_PZ == 1))
  if ((board_register_tmp & 0x07) != 0x07)
#elif ( \
  (MODYFIKACIA_VERSII_PZ == 2))
  if ((board_register_tmp & 0x03) != 0x03)
#elif (                           \
  (MODYFIKACIA_VERSII_PZ == 4) || \
  (MODYFIKACIA_VERSII_PZ == 24))
  if ((board_register_tmp & 0x13) != 0x13)
#elif (                            \
  (MODYFIKACIA_VERSII_PZ == 10) || \
  (MODYFIKACIA_VERSII_PZ == 13) || \
  (MODYFIKACIA_VERSII_PZ == 15) || \
  (MODYFIKACIA_VERSII_PZ == 17) || \
  (MODYFIKACIA_VERSII_PZ == 18) || \
  (MODYFIKACIA_VERSII_PZ == 33))
  if ((board_register_tmp & 0x1F) != 0x1F)
#elif ( \
  (MODYFIKACIA_VERSII_PZ == 11))
  if ((board_register_tmp & 0x0F) != 0x0F)
#elif ( \
  (MODYFIKACIA_VERSII_PZ == 12))
  if ((board_register_tmp & 0x0B) != 0x0B)
#elif (                            \
  (MODYFIKACIA_VERSII_PZ == 14) || \
  (MODYFIKACIA_VERSII_PZ == 34))
  if ((board_register_tmp & 0x1B) != 0x1B)
#else
#error "UDEFINED MODIFIKACIA"
#endif
  {

    if ((board_register_tmp & 0x01) != 0x1)
      _SET_BIT(set_diagnostyka, ERROR_BA_1_FIX);
#if ( \
  (MODYFIKACIA_VERSII_PZ == 18) || (MODYFIKACIA_VERSII_PZ == 8))
    if ((board_register_tmp & 0x02) != 0x2)
      _SET_BIT(set_diagnostyka, ERROR_BDV2_FIX);
#else
    if ((board_register_tmp & 0x02) != 0x2)
      _SET_BIT(set_diagnostyka, ERROR_BDVV5_1_FIX);
#endif

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
    if ((board_register_tmp & 0x08) != 0x8)
      _SET_BIT(set_diagnostyka, ERROR_CB_FIX);
#endif

#if (                              \
  (MODYFIKACIA_VERSII_PZ == 0) ||  \
  (MODYFIKACIA_VERSII_PZ == 1) ||  \
  (MODYFIKACIA_VERSII_PZ == 3) ||  \
  (MODYFIKACIA_VERSII_PZ == 5) ||  \
  (MODYFIKACIA_VERSII_PZ == 6) ||  \
  (MODYFIKACIA_VERSII_PZ == 8) ||  \
  (MODYFIKACIA_VERSII_PZ == 10) || \
  (MODYFIKACIA_VERSII_PZ == 11) || \
  (MODYFIKACIA_VERSII_PZ == 13) || \
  (MODYFIKACIA_VERSII_PZ == 15) || \
  (MODYFIKACIA_VERSII_PZ == 18) || \
  (MODYFIKACIA_VERSII_PZ == 23) || \
  (MODYFIKACIA_VERSII_PZ == 26) || \
  (MODYFIKACIA_VERSII_PZ == 33))

#if ( \
  (MODYFIKACIA_VERSII_PZ == 18) || (MODYFIKACIA_VERSII_PZ == 8))
    if ((board_register_tmp & 0x04) != 0x4)
      _SET_BIT(set_diagnostyka, ERROR_BDV5_FIX);
#else

    if ((board_register_tmp & 0x04) != 0x4)
      _SET_BIT(set_diagnostyka, ERROR_BDVV5_2_FIX);
#endif
//#elif
//#elif (                                \
//       (MODYFIKACIA_VERSII_PZ == 6) || \
//       (MODYFIKACIA_VERSII_PZ == 26)   \
//      )
//    if ((board_register_tmp &  0x04) !=  0x4) _SET_BIT(set_diagnostyka, ERROR_BDVV9_FIX);
#elif (                           \
  (MODYFIKACIA_VERSII_PZ == 7) || \
  (MODYFIKACIA_VERSII_PZ == 17))
    if ((board_register_tmp & 0x04) != 0x4)
      _SET_BIT(set_diagnostyka, ERROR_BDVV_DZ_FIX);
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
  (MODYFIKACIA_VERSII_PZ == 23) || \
  (MODYFIKACIA_VERSII_PZ == 24) || \
  (MODYFIKACIA_VERSII_PZ == 26) || \
  (MODYFIKACIA_VERSII_PZ == 33) || \
  (MODYFIKACIA_VERSII_PZ == 34))
    if ((board_register_tmp & 0x010) != 0x10)

#if (                             \
  (MODYFIKACIA_VERSII_PZ == 0) || \
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
  (MODYFIKACIA_VERSII_PZ == 23) || \
  (MODYFIKACIA_VERSII_PZ == 24) || \
  (MODYFIKACIA_VERSII_PZ == 26) || \
  (MODYFIKACIA_VERSII_PZ == 17) || \
  (MODYFIKACIA_VERSII_PZ == 33) || \
  (MODYFIKACIA_VERSII_PZ == 34))
      _SET_BIT(set_diagnostyka, ERROR_BDSH_FIX);
#else
#error "UDEFINED MODIFIKACIA"
#endif

#endif
  }

  if ((board_register_tmp & 0x01) == 0x01)
  {
    _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD39_DD40_DD47) = 0x1;
    if ((_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD39_DD40_DD47) >> 8) != 0x11)
      _SET_BIT(set_diagnostyka, ERROR_BA_1_CTLR);
  }
#if ( \
  (MODYFIKACIA_VERSII_PZ == 18) || (MODYFIKACIA_VERSII_PZ == 8))
  if ((board_register_tmp & 0x02) == 0x02)
  {
    _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD39_DD40_DD47) = 0x2; //БДВ2_D
    unsigned long u32_R = _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD33_DD36);
    if (u32_R == 0)
      u32_R++;
    if ((_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD33_DD36) >> 8) != 0x59)
      _SET_BIT(set_diagnostyka, ERROR_BDV2_CTLR);
  }
#else
  if ((board_register_tmp & 0x02) == 0x02)
  {
    _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD39_DD40_DD47) = 0x2;
    if ((_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD33_DD36) & 0xff) != 0x25)
      _SET_BIT(set_diagnostyka, ERROR_BDVV5_1_CTLR);
  }
#endif

#if (                              \
  (MODYFIKACIA_VERSII_PZ == 0) ||  \
  (MODYFIKACIA_VERSII_PZ == 1) ||  \
  (MODYFIKACIA_VERSII_PZ == 3) ||  \
  (MODYFIKACIA_VERSII_PZ == 6) ||  \
  (MODYFIKACIA_VERSII_PZ == 10) || \
  (MODYFIKACIA_VERSII_PZ == 11) || \
  (MODYFIKACIA_VERSII_PZ == 13) || \
  (MODYFIKACIA_VERSII_PZ == 23) || \
  (MODYFIKACIA_VERSII_PZ == 26) || \
  (MODYFIKACIA_VERSII_PZ == 33))
  if ((board_register_tmp & 0x04) == 0x04)
  {
    _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD39_DD40_DD47) = 0x4;
    if ((_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD33_DD36) >> 8) != 0x37)
      _SET_BIT(set_diagnostyka, ERROR_BDVV5_2_CTLR);
  }
#elif (                           \
  (MODYFIKACIA_VERSII_PZ == 8) || \
  (MODYFIKACIA_VERSII_PZ == 18))
  if ((board_register_tmp & 0x04) == 0x04)
  {
    _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD39_DD40_DD47) = 0x4;
    unsigned long u32_R = _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD33_DD36);
    if (u32_R == 0)
      u32_R++;
    if ((_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD33_DD36) & 0xff) != 0x6D)
      _SET_BIT(set_diagnostyka, ERROR_BDV5_CTLR);
  }
//#elif (                                \
//       (MODYFIKACIA_VERSII_PZ == 6) || \
//       (MODYFIKACIA_VERSII_PZ == 26)   \
//      )
//  if ((board_register_tmp & 0x04) == 0x04)
//  {
//    _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD39_DD40_DD47) = 0x4;
//    if ((_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD33_DD36) >> 8) != 0x29)  _SET_BIT(set_diagnostyka, ERROR_BDVV9_CTLR);
//  }
#elif (                           \
  (MODYFIKACIA_VERSII_PZ == 7) || \
  (MODYFIKACIA_VERSII_PZ == 17))
  if ((board_register_tmp & 0x04) == 0x04)
  {
    _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD39_DD40_DD47) = 0x4;
    if ((_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD33_DD36) >> 8) != 0xA7)
      _SET_BIT(set_diagnostyka, ERROR_BDVV_DZ_CTLR);
  }
#endif

#if (                              \
  (MODYFIKACIA_VERSII_PZ == 0) ||  \
  (MODYFIKACIA_VERSII_PZ == 3) ||  \
  (MODYFIKACIA_VERSII_PZ == 4) ||  \
  (MODYFIKACIA_VERSII_PZ == 5) ||  \
  (MODYFIKACIA_VERSII_PZ == 6) ||  \
  (MODYFIKACIA_VERSII_PZ == 7) ||  \
  (MODYFIKACIA_VERSII_PZ == 10) || \
  (MODYFIKACIA_VERSII_PZ == 13) || \
  (MODYFIKACIA_VERSII_PZ == 14) || \
  (MODYFIKACIA_VERSII_PZ == 15) || \
  (MODYFIKACIA_VERSII_PZ == 17) || \
  (MODYFIKACIA_VERSII_PZ == 18) || \
  (MODYFIKACIA_VERSII_PZ == 23) || \
  (MODYFIKACIA_VERSII_PZ == 24) || \
  (MODYFIKACIA_VERSII_PZ == 26) || \
  (MODYFIKACIA_VERSII_PZ == 33) || \
  (MODYFIKACIA_VERSII_PZ == 34))
  if ((board_register_tmp & 0x10) == 0x10)
  {
    _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD39_DD40_DD47) = 0x10;
#if (                             \
  (MODYFIKACIA_VERSII_PZ == 0) || \
  (MODYFIKACIA_VERSII_PZ == 10))
    if ((_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD26_DD29) >> 8) != 0x14)
      _SET_BIT(set_diagnostyka, ERROR_BDV_DZ_CTLR);
#elif (                            \
  (MODYFIKACIA_VERSII_PZ == 5) ||  \
  (MODYFIKACIA_VERSII_PZ == 8) ||  \
  (MODYFIKACIA_VERSII_PZ == 15) || \
  (MODYFIKACIA_VERSII_PZ == 18))
    if ((_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD26_DD29) >> 8) != 0x12)
      _SET_BIT(set_diagnostyka, ERROR_BDVV6_CTLR);

    unsigned long u32_R = _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD26_DD29);
    if (u32_R == 0)
      u32_R++;
#elif (                            \
  (MODYFIKACIA_VERSII_PZ == 3) ||  \
  (MODYFIKACIA_VERSII_PZ == 4) ||  \
  (MODYFIKACIA_VERSII_PZ == 6) ||  \
  (MODYFIKACIA_VERSII_PZ == 13) || \
  (MODYFIKACIA_VERSII_PZ == 14))
    if ((_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD26_DD29) >> 8) != 0x18)
      _SET_BIT(set_diagnostyka, ERROR_BDZ_CTLR);
#elif (                            \
  (MODYFIKACIA_VERSII_PZ == 7) ||  \
  (MODYFIKACIA_VERSII_PZ == 23) || \
  (MODYFIKACIA_VERSII_PZ == 24) || \
  (MODYFIKACIA_VERSII_PZ == 26) || \
  (MODYFIKACIA_VERSII_PZ == 17) || \
  (MODYFIKACIA_VERSII_PZ == 33) || \
  (MODYFIKACIA_VERSII_PZ == 34))
    if ((_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD26_DD29) >> 8) != 0x13)
      _SET_BIT(set_diagnostyka, ERROR_BDSH_CTLR);
#else
#error "UDEFINED MODIFIKACIA"
#endif
  }
#endif

  //Вимикаємо вибір всіх плат для подальшого контролю
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD39_DD40_DD47) = 0x0;
#if ( \
  (MODYFIKACIA_VERSII_PZ == 18) || (MODYFIKACIA_VERSII_PZ == 8))
  chGbl__CDHO1_CDHO7__W_VAL = 0x0;
#endif
  /***/

  /* Конфігурація піну LCD-BL, як Output push-pull */
  GPIO_InitStructure.GPIO_Pin = LCD_BL_PIN;
  GPIO_Init(LCD_BL, &GPIO_InitStructure);
  /* Знімаємо пін LCD-BL для того, щоб вимкнути підсвітку LCD*/
  GPIO_ResetBits(LCD_BL, LCD_BL_PIN);

  /* Конфігурація піну LCD-R/W, як Output push-pull */
  GPIO_InitStructure.GPIO_Pin = LCD_RW_PIN;
  GPIO_Init(LCD_RW, &GPIO_InitStructure);
  /* Знімаємо пін LCD-R/W*/
  GPIO_ResetBits(LCD_RW, LCD_RW_PIN);

  /* Конфігурація піну LCD-RS, як Output push-pull */
  GPIO_InitStructure.GPIO_Pin = LCD_RS_PIN;
  GPIO_Init(LCD_RS, &GPIO_InitStructure);
  /* Знімаємо пін LCD-RS*/
  GPIO_ResetBits(LCD_RS, LCD_RS_PIN);

  /*SW_1 клавіатури */
  GPIO_InitStructure.GPIO_Pin = KEYBOARD_SW_1_PIN;
  GPIO_Init(KEYBOARD, &GPIO_InitStructure);
  /* Виставляємо пін SW1*/
  GPIO_SetBits(KEYBOARD, KEYBOARD_SW_1_PIN);

  /*SW_2 клавіатури */
  GPIO_InitStructure.GPIO_Pin = KEYBOARD_SW_2_PIN;
  GPIO_Init(KEYBOARD, &GPIO_InitStructure);
  /* Виставляємо пін SW2*/
  GPIO_SetBits(KEYBOARD, KEYBOARD_SW_2_PIN);

  /*SW_3 клавіатури */
  GPIO_InitStructure.GPIO_Pin = KEYBOARD_SW_3_PIN;
  GPIO_Init(KEYBOARD, &GPIO_InitStructure);
  /*Виставляємо пін SW3*/
  GPIO_SetBits(KEYBOARD, KEYBOARD_SW_3_PIN);

  /*SW_4 клавіатури */
  GPIO_InitStructure.GPIO_Pin = KEYBOARD_SW_4_PIN;
  GPIO_Init(KEYBOARD, &GPIO_InitStructure);
  /*Виставляємо пін SW4*/
  GPIO_SetBits(KEYBOARD, KEYBOARD_SW_4_PIN);

  /*Вибір АЦП */
  GPIO_InitStructure.GPIO_Pin = GPIO_SELECTPin_ADC;
  GPIO_Init(GPIO_SELECT_ADC, &GPIO_InitStructure);
  GPIO_ResetBits(GPIO_SELECT_ADC, GPIO_SELECTPin_ADC);

  /*NSS_ADC каналу SPI_ADC */
  GPIO_InitStructure.GPIO_Pin = GPIO_NSSPin_ADC;
  GPIO_Init(GPIO_SPI_ADC, &GPIO_InitStructure);
  GPIO_SetBits(GPIO_SPI_ADC, GPIO_NSSPin_ADC);

  /*NSS_ADC каналу SPI_EDF */
  GPIO_InitStructure.GPIO_Pin = GPIO_NSSPin_EDF;
  GPIO_Init(GPIO_SPI_EDF, &GPIO_InitStructure);
  GPIO_SetBits(GPIO_SPI_EDF, GPIO_NSSPin_EDF);

  /*GPIO_SPI_EDF_A0 - вибір мікросхеми DataFlash*/
  GPIO_InitStructure.GPIO_Pin = GPIO_SPI_EDF_A0_Pin;
  GPIO_Init(GPIO_SPI_EDF_A0, &GPIO_InitStructure);
  //Вибираємо EEPROM
  GPIO_SetBits(GPIO_SPI_EDF_A0, GPIO_SPI_EDF_A0_Pin);

  /*GPIO_SPI_EDF_A1 - вибір мікросхеми DataFlash*/
  GPIO_InitStructure.GPIO_Pin = GPIO_SPI_EDF_A1_Pin;
  GPIO_Init(GPIO_SPI_EDF_A1, &GPIO_InitStructure);
  //Вибираємо EEPROM
  GPIO_SetBits(GPIO_SPI_EDF_A1, GPIO_SPI_EDF_A1_Pin);

  /* Пін 485DE*/
  GPIO_InitStructure.GPIO_Pin = GPIO_PIN_485DE;
  GPIO_Init(GPIO_485DE, &GPIO_InitStructure);
  /* Знімаємо пін 485DE */
  GPIO_ResetBits(GPIO_485DE, GPIO_PIN_485DE);

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
  /* Canal1_MO_Out1*/
  GPIO_InitStructure.GPIO_Pin = GPIO_PIN_CANAL1_MO_Out1;
  GPIO_Init(GPIO_CANAL1_MO_Out1, &GPIO_InitStructure);
  /* Знімаємо пін Canal1_MO_Out */
  GPIO_ResetBits(GPIO_CANAL1_MO_Out1, GPIO_PIN_CANAL1_MO_Out1);

  /* KP_SOFT_RESET_Out1*/
  GPIO_InitStructure.GPIO_Pin = GPIO_PIN_KP_SOFT_RESET;
  GPIO_Init(GPIO_KP_SOFT_RESET, &GPIO_InitStructure);
  /* Знімаємо пін KP_SOFT_RESET */
  GPIO_ResetBits(GPIO_KP_SOFT_RESET, GPIO_PIN_KP_SOFT_RESET);
#endif
  /**************/

  /* Пін Перепрограмування*/
  GPIO_InitStructure.GPIO_Pin = GPIO_PIN_REPROGRAM;
  GPIO_Init(GPIO_REPROGRAM, &GPIO_InitStructure);
  /* Знімаємо пін Перепрограмування */
  GPIO_ResetBits(GPIO_REPROGRAM, GPIO_PIN_REPROGRAM);

  /* Пін "Штатний режим"*/
  GPIO_InitStructure.GPIO_Pin = GPIO_PIN_STAFF;
  GPIO_Init(GPIO_STAFF, &GPIO_InitStructure);
  /* Знімаємо пін "Штатний режим" */
  GPIO_ResetBits(GPIO_STAFF, GPIO_PIN_STAFF);

  /* Пін для відладки*/
  GPIO_InitStructure.GPIO_Pin = GPIO_PIN_DEBUG;
  GPIO_Init(GPIO_DEBUG, &GPIO_InitStructure);
  /* Знімаємо пін для відладки */
  GPIO_ResetBits(GPIO_DEBUG, GPIO_PIN_DEBUG);

  /**************/
  //Піни на ввід
  /**************/
  /*SW_A клавіатури */
  GPIO_InitStructure.GPIO_Pin = KEYBOARD_SW_A_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(KEYBOARD_SW_A, &GPIO_InitStructure);

  /*SW_B клавіатури */
  GPIO_InitStructure.GPIO_Pin = KEYBOARD_SW_B_PIN;
  GPIO_Init(KEYBOARD_SW_B, &GPIO_InitStructure);

  /*SW_C клавіатури */
  GPIO_InitStructure.GPIO_Pin = KEYBOARD_SW_C_PIN;
  GPIO_Init(KEYBOARD_SW_C, &GPIO_InitStructure);

  /*Контроль живлення */
  GPIO_InitStructure.GPIO_Pin = POWER_CTRL_PIN;
  GPIO_Init(POWER_CTRL, &GPIO_InitStructure);

  /*Контроль Перепрограмування/Штатний режим */
  GPIO_InitStructure.GPIO_Pin = GPIO_PIN_STAFF_REPROGRAM;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(GPIO_STAFF_REPROGRAM, &GPIO_InitStructure);
  /**************/

  /**************/
  //Піни альтернативних функцій
  /**************/
  //Перекидаємо піни PB13/SPI2_SCK, PB14/SPI2_MISO і  PB15/SPI2_MOSI
  GPIO_PinAFConfig(GPIO_SPI_ADC, GPIO_SCKPin_ADCSource, GPIO_AF_SPI_ADC);
  GPIO_PinAFConfig(GPIO_SPI_ADC, GPIO_MISOPin_ADCSource, GPIO_AF_SPI_ADC);
  GPIO_PinAFConfig(GPIO_SPI_ADC, GPIO_MOSIPin_ADCSource, GPIO_AF_SPI_ADC);

  /* Configure SPI_ADC SCK, MISO і MOSI */
  GPIO_InitStructure.GPIO_Pin = GPIO_SCKPin_ADC | GPIO_MISOPin_ADC | GPIO_MOSIPin_ADC;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIO_SPI_ADC, &GPIO_InitStructure);

  //Перекидаємо піни PA5/SPI1_SCK, PA6/SPI1_MISO і  PA7/SPI1_MOSI
  GPIO_PinAFConfig(GPIO_SPI_EDF, GPIO_SCKPin_EDFSource, GPIO_AF_SPI_EDF);
  GPIO_PinAFConfig(GPIO_SPI_EDF, GPIO_MISOPin_EDFSource, GPIO_AF_SPI_EDF);
  GPIO_PinAFConfig(GPIO_SPI_EDF, GPIO_MOSIPin_EDFSource, GPIO_AF_SPI_EDF);

  /* Configure SPI_EDF SCK, MISO і MOSI */
  GPIO_InitStructure.GPIO_Pin = GPIO_SCKPin_EDF | GPIO_MISOPin_EDF | GPIO_MOSIPin_EDF;
  GPIO_Init(GPIO_SPI_EDF, &GPIO_InitStructure);

  //Перекидаємо піни PA2/Tx_RS-485, PA3/Rx_RS-485
  GPIO_PinAFConfig(GPIO_USART_RS485, GPIO_TxPin_RS485Source, GPIO_AF_USART_RS_485);
  GPIO_PinAFConfig(GPIO_USART_RS485, GPIO_RxPin_RS485Source, GPIO_AF_USART_RS_485);

  /* Configure USART_RS485 Rx/Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_TxPin_RS485 | GPIO_RxPin_RS485;
  GPIO_Init(GPIO_USART_RS485, &GPIO_InitStructure);

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
  //Перекидаємо піни PA9/Tx_CANAL1_MO, PA10/Rx_CANAL1_MO
  GPIO_PinAFConfig(GPIO_CANAL1_MO, GPIO_TxPin_CANAL1_MOSource, GPIO_AF_CANAL1_MO);
  GPIO_PinAFConfig(GPIO_CANAL1_MO, GPIO_RxPin_CANAL1_MOSource, GPIO_AF_CANAL1_MO);

  /* Configure CANAL1_MO Rx/Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_TxPin_CANAL1_MO | GPIO_RxPin_CANAL1_MO;
  GPIO_Init(GPIO_CANAL1_MO, &GPIO_InitStructure);

  //Перекидаємо піни PA9/Tx_CANAL2_MO, PA10/Rx_CANAL2_MO
  GPIO_PinAFConfig(GPIO_CANAL2_MO, GPIO_TxPin_CANAL2_MOSource, GPIO_AF_CANAL2_MO);
  GPIO_PinAFConfig(GPIO_CANAL2_MO, GPIO_RxPin_CANAL2_MOSource, GPIO_AF_CANAL2_MO);

  /* Configure CANAL2_MO Rx/Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_TxPin_CANAL2_MO | GPIO_RxPin_CANAL2_MO;
  GPIO_Init(GPIO_CANAL2_MO, &GPIO_InitStructure);
#endif
  /**********************/

  /* Під'єднання EXTI Line0 to POWER_CTRL_PIN піну */
  SYSCFG_EXTILineConfig(EXTI_PortSource_POWER, EXTI_PinSource_POWER);
  /**********************/

  /* Конфігурування EXTI Line0 */
  EXTI_InitTypeDef EXTI_InitStructure;

  EXTI_InitStructure.EXTI_Line = EXTI_Line_POWER;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  /**********************/

  /**********************/
  //Настроювання DMA контролерів
  /**********************/
  DMA_InitTypeDef DMA_InitStructure;

  /* DMA настроюємо для прийому даних по I2C*/
  DMA_DeInit(DMA_StreamI2C_Rx);
  while (DMA_GetCmdStatus(DMA_StreamI2C_Rx) != DISABLE)
    ;

  DMA_InitStructure.DMA_Channel = DMA_ChannelI2C_Rx;
  DMA_InitStructure.DMA_PeripheralBaseAddr = I2C_DR_Address;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) Temporaty_I2C_Buffer;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize = MAX_NUMBER_REGISTERS_RTC + 2;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA_StreamI2C_Rx, &DMA_InitStructure);
  DMA_ClearFlag(DMA_StreamI2C_Rx, DMA_FLAG_TCI2C_Rx | DMA_FLAG_HTI2C_Rx | DMA_FLAG_TEII2C_Rx | DMA_FLAG_DMEII2C_Rx | DMA_FLAG_FEII2C_Rx);

  /* DMA настроюємо для передавання даних по I2C*/
  DMA_DeInit(DMA_StreamI2C_Tx);
  while (DMA_GetCmdStatus(DMA_StreamI2C_Tx) != DISABLE)
    ;

  DMA_InitStructure.DMA_Channel = DMA_ChannelI2C_Tx;
  DMA_InitStructure.DMA_PeripheralBaseAddr = I2C_DR_Address;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) Temporaty_I2C_Buffer;
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
  DMA_Init(DMA_StreamI2C_Tx, &DMA_InitStructure);
  DMA_ClearFlag(DMA_StreamI2C_Tx, DMA_FLAG_TCI2C_Tx | DMA_FLAG_HTI2C_Tx | DMA_FLAG_TEII2C_Tx | DMA_FLAG_DMEII2C_Tx | DMA_FLAG_FEII2C_Tx);

  /* DMA настроюємо для передачі даних по SPI_EDF*/
  DMA_DeInit(DMA_StreamSPI_EDF_Tx);
  while (DMA_GetCmdStatus(DMA_StreamSPI_EDF_Tx) != DISABLE)
    ;

  DMA_InitStructure.DMA_Channel = DMA_ChannelSPI_EDF_Tx;
  DMA_InitStructure.DMA_PeripheralBaseAddr = SPI_EDF_DR_Address;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) TxBuffer_SPI_EDF;
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
  DMA_InitStructure.DMA_BufferSize = SIZE_PAGE_DATAFLASH_MAX + 10;
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh; /*Так як № потоку DMA_StreamSPI_EDF_Tx > за № потоку DMA_StreamSPI_EDF_Rx, то DMA_StreamSPI_EDF_Rx має пріориет над DMA_StreamSPI_EDF_Tx при однаковому програмному пріоритеті*/
  DMA_Init(DMA_StreamSPI_EDF_Tx, &DMA_InitStructure);
  DMA_ClearFlag(DMA_StreamSPI_EDF_Tx, DMA_FLAG_TCSPI_EDF_Tx | DMA_FLAG_HTSPI_EDF_Tx | DMA_FLAG_TEISPI_EDF_Tx | DMA_FLAG_DMEISPI_EDF_Tx | DMA_FLAG_FEISPI_EDF_Tx);

  /* DMA настроюємо для прийому даних по SPI_EDF*/
  DMA_DeInit(DMA_StreamSPI_EDF_Rx);
  while (DMA_GetCmdStatus(DMA_StreamSPI_EDF_Rx) != DISABLE)
    ;

  DMA_InitStructure.DMA_Channel = DMA_ChannelSPI_EDF_Rx;
  DMA_InitStructure.DMA_PeripheralBaseAddr = SPI_EDF_DR_Address;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) RxBuffer_SPI_EDF;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh; /*Так як № потоку DMA_StreamSPI_EDF_Tx > за № потоку DMA_StreamSPI_EDF_Rx, то DMA_StreamSPI_EDF_Rx має пріориет над DMA_StreamSPI_EDF_Tx при однаковому програмному пріоритеті*/
  DMA_Init(DMA_StreamSPI_EDF_Rx, &DMA_InitStructure);
  DMA_ClearFlag(DMA_StreamSPI_EDF_Rx, DMA_FLAG_TCSPI_EDF_Rx | DMA_FLAG_HTSPI_EDF_Rx | DMA_FLAG_TEISPI_EDF_Rx | DMA_FLAG_DMEISPI_EDF_Rx | DMA_FLAG_FEISPI_EDF_Rx);

  /* Прийом по RS-485*/
  DMA_DeInit(DMA_StreamRS485_Rx);
  while (DMA_GetCmdStatus(DMA_StreamRS485_Rx) != DISABLE)
    ;

  DMA_InitStructure.DMA_Channel = DMA_ChannelRS485_Rx;
  DMA_InitStructure.DMA_PeripheralBaseAddr = USARTRS485_DR_Base;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) RxBuffer_RS485;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize = BUFFER_RS485;
  DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
  DMA_Init(DMA_StreamRS485_Rx, &DMA_InitStructure);
  DMA_ClearFlag(DMA_StreamRS485_Rx, DMA_FLAG_TCRS485_Rx | DMA_FLAG_HTRS485_Rx | DMA_FLAG_TEIRS485_Rx | DMA_FLAG_DMEIRS485_Rx | DMA_FLAG_FEIRS485_Rx);

  /* Передача по RS-485*/
  DMA_DeInit(DMA_StreamRS485_Tx);
  while (DMA_GetCmdStatus(DMA_StreamRS485_Tx) != DISABLE)
    ;

  DMA_InitStructure.DMA_Channel = DMA_ChannelRS485_Tx;
  DMA_InitStructure.DMA_PeripheralBaseAddr = USARTRS485_DR_Base;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) TxBuffer_RS485;
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
  DMA_InitStructure.DMA_BufferSize = 0;
  DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
  DMA_Init(DMA_StreamRS485_Tx, &DMA_InitStructure);
  DMA_ClearFlag(DMA_StreamRS485_Tx, DMA_FLAG_TCRS485_Tx | DMA_FLAG_HTRS485_Tx | DMA_FLAG_TEIRS485_Tx | DMA_FLAG_DMEIRS485_Tx | DMA_FLAG_FEIRS485_Tx);
  /**********************/

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
  /* Прийом по CANAL1_MO*/
  DMA_DeInit(DMA_StreamCANAL1_MO_Rx);
  while (DMA_GetCmdStatus(DMA_StreamCANAL1_MO_Rx) != DISABLE)
    ;

  DMA_InitStructure.DMA_Channel = DMA_ChannelCANAL1_MO_Rx;
  DMA_InitStructure.DMA_PeripheralBaseAddr = CANAL1_MO_DR_Base;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) Canal1_MO_Received[Canal1_MO_ReceiveBank];
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize = BUFFER_CANAL1_MO;
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh; /*Так як № потоку DMA_ChannelCANAL1_MO_Tx > за № потоку DMA_ChannelCANAL1_MO_Rx, то DMA_ChannelCANAL1_MO_Rx має пріориет над DMA_ChannelCANAL1_MO_Tx при однаковому програмному пріоритеті*/
  DMA_Init(DMA_StreamCANAL1_MO_Rx, &DMA_InitStructure);
  DMA_ClearFlag(DMA_StreamCANAL1_MO_Rx, DMA_FLAG_TCCANAL1_MO_Rx | DMA_FLAG_HTCANAL1_MO_Rx | DMA_FLAG_TEICANAL1_MO_Rx | DMA_FLAG_DMEICANAL1_MO_Rx | DMA_FLAG_FEICANAL1_MO_Rx);

  /* Передача по CANAL1_MO*/
  DMA_DeInit(DMA_StreamCANAL1_MO_Tx);
  while (DMA_GetCmdStatus(DMA_StreamCANAL1_MO_Tx) != DISABLE)
    ;

  DMA_InitStructure.DMA_Channel = DMA_ChannelCANAL1_MO_Tx;
  DMA_InitStructure.DMA_PeripheralBaseAddr = CANAL1_MO_DR_Base;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) Canal1_MO_Transmit;
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
  DMA_InitStructure.DMA_BufferSize = 0;
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh; /*Так як № потоку DMA_ChannelCANAL1_MO_Tx > за № потоку DMA_ChannelCANAL1_MO_Rx, то DMA_ChannelCANAL1_MO_Rx має пріориет над DMA_ChannelCANAL1_MO_Tx при однаковому програмному пріоритеті*/
  ;
  DMA_Init(DMA_StreamCANAL1_MO_Tx, &DMA_InitStructure);
  DMA_ClearFlag(DMA_StreamCANAL1_MO_Tx, DMA_FLAG_TCCANAL1_MO_Tx | DMA_FLAG_HTCANAL1_MO_Tx | DMA_FLAG_TEICANAL1_MO_Tx | DMA_FLAG_DMEICANAL1_MO_Tx | DMA_FLAG_FEICANAL1_MO_Tx);

  /* Прийом по CANAL2_MO*/
  DMA_DeInit(DMA_StreamCANAL2_MO_Rx);
  while (DMA_GetCmdStatus(DMA_StreamCANAL2_MO_Rx) != DISABLE)
    ;

  DMA_InitStructure.DMA_Channel = DMA_ChannelCANAL2_MO_Rx;
  DMA_InitStructure.DMA_PeripheralBaseAddr = CANAL2_MO_DR_Base;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) Canal2_MO_Received;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize = BUFFER_CANAL2_MO;
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh; /*Так як № потоку DMA_ChannelCANAL2_MO_Tx > за № потоку DMA_ChannelCANAL2_MO_Rx, то DMA_ChannelCANAL2_MO_Rx має пріориет над DMA_ChannelCANAL2_MO_Tx при однаковому програмному пріоритеті*/
  DMA_Init(DMA_StreamCANAL2_MO_Rx, &DMA_InitStructure);
  DMA_ClearFlag(DMA_StreamCANAL2_MO_Rx, DMA_FLAG_TCCANAL2_MO_Rx | DMA_FLAG_HTCANAL2_MO_Rx | DMA_FLAG_TEICANAL2_MO_Rx | DMA_FLAG_DMEICANAL2_MO_Rx | DMA_FLAG_FEICANAL2_MO_Rx);

  /* Передача по CANAL2_MO*/
  DMA_DeInit(DMA_StreamCANAL2_MO_Tx);
  while (DMA_GetCmdStatus(DMA_StreamCANAL2_MO_Tx) != DISABLE)
    ;

  DMA_InitStructure.DMA_Channel = DMA_ChannelCANAL2_MO_Tx;
  DMA_InitStructure.DMA_PeripheralBaseAddr = CANAL2_MO_DR_Base;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) Canal2_MO_Transmit;
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
  DMA_InitStructure.DMA_BufferSize = 0;
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh; /*Так як № потоку DMA_ChannelCANAL2_MO_Tx > за № потоку DMA_ChannelCANAL2_MO_Rx, то DMA_ChannelCANAL2_MO_Rx має пріориет над DMA_ChannelCANAL2_MO_Tx при однаковому програмному пріоритеті*/
  ;
  DMA_Init(DMA_StreamCANAL2_MO_Tx, &DMA_InitStructure);
  DMA_ClearFlag(DMA_StreamCANAL2_MO_Tx, DMA_FLAG_TCCANAL2_MO_Tx | DMA_FLAG_HTCANAL2_MO_Tx | DMA_FLAG_TEICANAL2_MO_Tx | DMA_FLAG_DMEICANAL2_MO_Tx | DMA_FLAG_FEICANAL2_MO_Tx);
  /**********************/
#endif

  /**********************/
  //Настроювання SPI для  АЦП з початковою ініціалізацією АП
  /**********************/
  SPI_InitTypeDef SPI_InitStructure;

  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2; /*30МГц/2 = 15 МГц*/
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI_ADC, &SPI_InitStructure);

  /* Дозволяємо SPI_ADC */
  SPI_Cmd(SPI_ADC, ENABLE);

  //Стартовий запуск АЦП
  //Скидаємо признаки помилок
  if (SPI_I2S_GetFlagStatus(SPI_ADC, SPI_I2S_FLAG_RXNE) == SET)
    SPI_I2S_ReceiveData(SPI_ADC);
  while (SPI_I2S_GetFlagStatus(SPI_ADC, SPI_I2S_FLAG_OVR) == SET)
    SPI_I2S_ReceiveData(SPI_ADC);

  for (unsigned int i = 0; i < NUMBER_ADCs; i++)
  {
    if (i == 0)
      GPIO_SELECT_ADC->BSRRH = GPIO_SELECTPin_ADC;
    else
      GPIO_SELECT_ADC->BSRRL = GPIO_SELECTPin_ADC;

    //Посилаємо перше слово 0xffff
    while (SPI_I2S_GetFlagStatus(SPI_ADC, SPI_I2S_FLAG_TXE) == RESET)
      ;                                            //Очікуємо, поки SPI стане вільним
    tick_output_adc_p = TIM5->CNT;                 //Фіксуємо час початку оцифровки
    GPIO_ResetBits(GPIO_SPI_ADC, GPIO_NSSPin_ADC); //Виставляємо chip_select
    SPI_I2S_SendData(SPI_ADC, 0xFFFF);             //Відправляємо число 0xffff
    while (SPI_I2S_GetFlagStatus(SPI_ADC, SPI_I2S_FLAG_RXNE) == RESET)
      ;                                          //Очікуємо завершення трансакції по прийнятті даних по MISO
    GPIO_SetBits(GPIO_SPI_ADC, GPIO_NSSPin_ADC); //Знімаємо chip_select
    SPI_I2S_ReceiveData(SPI_ADC);                //Читаємо прийняті дані

    //Посилаємо друге слово 0xffff
    while (SPI_I2S_GetFlagStatus(SPI_ADC, SPI_I2S_FLAG_TXE) == RESET)
      ;                                            //Очікуємо, поки SPI стане вільним
    tick_output_adc_p = TIM5->CNT;                 //Фіксуємо час початку оцифровки
    GPIO_ResetBits(GPIO_SPI_ADC, GPIO_NSSPin_ADC); //Виставляємо chip_select
    SPI_I2S_SendData(SPI_ADC, 0xFFFF);             //Відправляємо число 0xffff
    while (SPI_I2S_GetFlagStatus(SPI_ADC, SPI_I2S_FLAG_RXNE) == RESET)
      ;                                          //Очікуємо завершення трансакції по прийнятті даних по MISO
    GPIO_SetBits(GPIO_SPI_ADC, GPIO_NSSPin_ADC); //Знімаємо chip_select
    SPI_I2S_ReceiveData(SPI_ADC);                //Читаємо прийняті дані
  }

  //Дозволяємо переривання від прийнятого байту по SPI
  SPI_I2S_ITConfig(SPI_ADC, SPI_I2S_IT_RXNE, ENABLE);
  /**********************/

  /**********************/
  //Настроювання TIM4 на генерацію переривань кожні 10 мс для клавіатури і 1 мс для низькопріоритетних задач
  /**********************/
  /* Настроюємо TIM4 */
  /* ---------------------------------------------------------------
  TIM4 Настроювання: Output Compare Timing Mode:
  TIM4CLK = (30*2) МГц, Prescaler = 600, TIM4 counter clock = 100 кГц
  CC1 update rate = TIM4 counter clock / CCR1_Val = 100 Hz
  CC2 update rate = TIM4 counter clock / CCR1_Val = 1000 Hz
  --------------------------------------------------------------- */
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;

  TIM_TimeBaseStructure.TIM_Period = 65535;
  TIM_TimeBaseStructure.TIM_Prescaler = (600 - 1);
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

  /* Output Compare Timing Mode настроювання: Канал:1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
  TIM_OCInitStructure.TIM_Pulse = TIM4_CCR1_VAL;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OC1Init(TIM4, &TIM_OCInitStructure);

  TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Disable);

  /* Дозволяємо переривання від каналу 1 таймера 4*/
  TIM_ITConfig(TIM4, TIM_IT_CC1, ENABLE);

  /* Дозволяєм роботу таймера */
  TIM_Cmd(TIM4, ENABLE);
  /**********************/

  /**********************/
  //Настроювання SPI для  DataFlash з початковим читанням , чи мікросхеми переведені у потрібний режим
  /**********************/
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI_EDF, &SPI_InitStructure);

  /* Забороняємо SPI_EDF DMA Tx запити */
  SPI_I2S_DMACmd(SPI_EDF, SPI_I2S_DMAReq_Tx, DISABLE);
  /* Забороняємо SPI_EDF DMA Rx запити */
  SPI_I2S_DMACmd(SPI_EDF, SPI_I2S_DMAReq_Rx, DISABLE);

  //Очищаємо прапореці, що сигналізує про завершення прийом/передачі даних для DMA по потоку DMA_StreamSPI_EDF_Rx і DMA_StreamSPI_EDF_Tx
  DMA_ClearFlag(DMA_StreamSPI_EDF_Rx, DMA_FLAG_TCSPI_EDF_Rx | DMA_FLAG_HTSPI_EDF_Rx | DMA_FLAG_TEISPI_EDF_Rx | DMA_FLAG_DMEISPI_EDF_Rx | DMA_FLAG_FEISPI_EDF_Rx);
  DMA_ClearFlag(DMA_StreamSPI_EDF_Tx, DMA_FLAG_TCSPI_EDF_Tx | DMA_FLAG_HTSPI_EDF_Tx | DMA_FLAG_TEISPI_EDF_Tx | DMA_FLAG_DMEISPI_EDF_Tx | DMA_FLAG_FEISPI_EDF_Tx);

  //Дозволяємо переривання від помилок на SPI_EDF
  SPI_I2S_ITConfig(SPI_EDF, SPI_I2S_IT_ERR, ENABLE);

  /* Дозволяємо SPI_EDF */
  SPI_Cmd(SPI_EDF, ENABLE);
  /**********************/

  /**********************/
  //Читаємо збережені дані енергій з EEPROM
  /**********************/
  comparison_writing &= (unsigned int) (~COMPARISON_WRITING_ENERGY); /*зчитування, а не порівняння*/
  _SET_BIT(control_spi1_taskes, TASK_START_READ_ENERGY_EEPROM_BIT);
  while (
    (control_spi1_taskes[0] != 0) ||
    (control_spi1_taskes[1] != 0) ||
    (state_execution_spi1 > 0))
  {
    //Робота з watchdogs
    watchdog_routine(WATCHDOG_KYYBOARD, 42);

    main_routines_for_spi1();
    changing_diagnostyka_state(); //Підготовлюємо новий потенційно можливий запис для реєстратора програмних подій
  }
  /**********************/

  /**********************/
  //Читаємо збережені дані настройок з EEPROM
  /**********************/
  comparison_writing &= (unsigned int) (~COMPARISON_WRITING_SETTINGS); /*зчитування, а не порівняння*/
  _SET_BIT(control_spi1_taskes, TASK_START_READ_SETTINGS_EEPROM_BIT);
  while (
    (control_spi1_taskes[0] != 0) ||
    (control_spi1_taskes[1] != 0) ||
    (state_execution_spi1 > 0))
  {
    //Робота з watchdogs
    watchdog_routine(WATCHDOG_KYYBOARD, 43);

    main_routines_for_spi1();
    changing_diagnostyka_state(); //Підготовлюємо новий потенційно можливий запис для реєстратора програмних подій
  }
  /**********************/

  /**********************/
  //Читаємо збережені дані юстування з EEPROM
  /**********************/
  comparison_writing &= (unsigned int) (~COMPARISON_WRITING_USTUVANNJA); /*зчитування, а не порівняння*/
  _SET_BIT(control_spi1_taskes, TASK_START_READ_USTUVANNJA_EEPROM_BIT);
  while (
    (control_spi1_taskes[0] != 0) ||
    (control_spi1_taskes[1] != 0) ||
    (state_execution_spi1 > 0))
  {
    //Робота з watchdogs
    watchdog_routine(WATCHDOG_KYYBOARD, 44);

    main_routines_for_spi1();
    changing_diagnostyka_state(); //Підготовлюємо новий потенційно можливий запис для реєстратора програмних подій
  }
  /**********************/

  /**********************/
  //Читаємо збережені дані про сигнальні виходи і тригерні свтлодіоди
  /**********************/
  comparison_writing &= (unsigned int) (~COMPARISON_WRITING_STATE_LEDS_OUTPUTS); /*зчитування, а не порівняння*/
  _SET_BIT(control_spi1_taskes, TASK_START_READ_STATE_LEDS_OUTPUTS_EEPROM_BIT);
  while (
    (control_spi1_taskes[0] != 0) ||
    (control_spi1_taskes[1] != 0) ||
    (state_execution_spi1 > 0))
  {
    //Робота з watchdogs
    watchdog_routine(WATCHDOG_KYYBOARD, 45);

    main_routines_for_spi1();
    changing_diagnostyka_state(); //Підготовлюємо новий потенційно можливий запис для реєстратора програмних подій
  }
  /**********************/

  /**********************/
  //Читаємо збережені дані про тригерну інформацію
  /**********************/
  comparison_writing &= (unsigned int) (~COMPARISON_WRITING_TRG_FUNC); /*зчитування, а не порівняння*/
  _SET_BIT(control_spi1_taskes, TASK_START_READ_TRG_FUNC_EEPROM_BIT);
  while (
    (control_spi1_taskes[0] != 0) ||
    (control_spi1_taskes[1] != 0) ||
    (state_execution_spi1 > 0))
  {
    //Робота з watchdogs
    watchdog_routine(WATCHDOG_KYYBOARD, 46);

    main_routines_for_spi1();
    changing_diagnostyka_state(); //Підготовлюємо новий потенційно можливий запис для реєстратора програмних подій
  }
  /**********************/

  if ((state_spi1_task & STATE_SETTINGS_EEPROM_GOOD) != 0)
  {
    /*
    Оскільки при читанні даних аналогового реєстратора буде аналізуватися
    мкасимальна кількість записів, яка залежить від ширини доаварійного і
    післяаварійного масивів, то це читання треба виконувати тільки, коли відбулося
    успішне зчитування настройок.
    У інікшому разі структура інформації по аналоговому реєстратору має буди
    обнулена - тобто вся інформація по аналоговому реєстратуу буде очищена, якщо
    відбулася втрата інформації по настройках
    */
    /**********************/
    //Читаємо збережені дані аналогового реєстратора з EEPROM
    /**********************/
    comparison_writing &= (unsigned int) (~COMPARISON_WRITING_INFO_REJESTRATOR_AR); /*зчитування, а не порівняння*/
    _SET_BIT(control_spi1_taskes, TASK_START_READ_INFO_REJESTRATOR_AR_EEPROM_BIT);
    while (
      (control_spi1_taskes[0] != 0) ||
      (control_spi1_taskes[1] != 0) ||
      (state_execution_spi1 > 0))
    {
      //Робота з watchdogs
      watchdog_routine(WATCHDOG_KYYBOARD, 47);

      main_routines_for_spi1();
      changing_diagnostyka_state(); //Підготовлюємо новий потенційно можливий запис для реєстратора програмних подій
    }
    /**********************/
  }

  /**********************/
  //Читаємо збережені дані дискретного реєстратора з EEPROM
  /**********************/
  comparison_writing &= (unsigned int) (~COMPARISON_WRITING_INFO_REJESTRATOR_DR); /*зчитування, а не порівняння*/
  _SET_BIT(control_spi1_taskes, TASK_START_READ_INFO_REJESTRATOR_DR_EEPROM_BIT);
  while (
    (control_spi1_taskes[0] != 0) ||
    (control_spi1_taskes[1] != 0) ||
    (state_execution_spi1 > 0))
  {
    //Робота з watchdogs
    watchdog_routine(WATCHDOG_KYYBOARD, 48);

    main_routines_for_spi1();
    changing_diagnostyka_state(); //Підготовлюємо новий потенційно можливий запис для реєстратора програмних подій
  }
  /**********************/

  /**********************/
  //Читаємо збережені дані реєстратора програмних подій з EEPROM
  /**********************/
  comparison_writing &= (unsigned int) (~COMPARISON_WRITING_INFO_REJESTRATOR_PR_ERR); /*зчитування, а не порівняння*/
  _SET_BIT(control_spi1_taskes, TASK_START_READ_INFO_REJESTRATOR_PR_ERR_EEPROM_BIT);
  while (
    (control_spi1_taskes[0] != 0) ||
    (control_spi1_taskes[1] != 0) ||
    (state_execution_spi1 > 0))
  {
    //Робота з watchdogs
    watchdog_routine(WATCHDOG_KYYBOARD, 49);

    main_routines_for_spi1();
    changing_diagnostyka_state(); //Підготовлюємо новий потенційно можливий запис для реєстратора програмних подій
  }
  /**********************/

  /**********************/
  //Читаємо збережені дані ресурсу вимикача
  /**********************/
  comparison_writing &= (unsigned int) (~COMPARISON_WRITING_RESURS); /*зчитування, а не порівняння*/
  _SET_BIT(control_spi1_taskes, TASK_START_READ_RESURS_EEPROM_BIT);
  while (
    (control_spi1_taskes[0] != 0) ||
    (control_spi1_taskes[1] != 0) ||
    (state_execution_spi1 > 0))
  {
    //Робота з watchdogs
    watchdog_routine(WATCHDOG_KYYBOARD, 50);

    main_routines_for_spi1();
    changing_diagnostyka_state(); //Підготовлюємо новий потенційно можливий запис для реєстратора програмних подій
  }
  /**********************/

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
  /**********************/
  //Ініціалізація CANAL1_MO і CANAL2_MO: 6.75Мбіт/с, контроль парності, один стоп біт
  /**********************/
  USART_InitTypeDef USART_InitStructure;

  //Заповняємо структуру
  USART_InitStructure.USART_BaudRate = 7500000;
  USART_InitStructure.USART_WordLength = USART_WordLength_9b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_Even;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  //Виконуємо конфігупацію CANAL1_MO
  USART_OverSampling8Cmd(CANAL1_MO, ENABLE);
  USART_Init(CANAL1_MO, &USART_InitStructure);
  //Виконуємо конфігупацію CANAL2_MO
  USART_OverSampling8Cmd(CANAL2_MO, ENABLE);
  USART_Init(CANAL2_MO, &USART_InitStructure);

  //Дозволяємо передачу через DMA
  CANAL1_MO->CR3 |= USART_DMAReq_Rx;
  CANAL1_MO->CR3 |= USART_DMAReq_Tx;
  /*------------------------------*/
  CANAL2_MO->CR3 |= USART_DMAReq_Rx;
  CANAL2_MO->CR3 |= USART_DMAReq_Tx;

  //  //Дозволяємо переривання від CANAL1_MO при фіксації помикли
  //  USART_ITConfig(CANAL1_MO, USART_IT_IDLE, ENABLE);
  //  USART_ITConfig(CANAL1_MO, USART_IT_LBD, ENABLE);
  //  USART_ITConfig(CANAL1_MO, USART_IT_PE, ENABLE);
  //  USART_ITConfig(CANAL1_MO, USART_IT_ERR, ENABLE);
  //  //Дозволяємо переривання від CANAL2_MO при фіксації помикли
  //  USART_ITConfig(CANAL2_MO, USART_IT_IDLE, ENABLE);
  //  USART_ITConfig(CANAL2_MO, USART_IT_LBD, ENABLE);
  //  USART_ITConfig(CANAL2_MO, USART_IT_PE, ENABLE);
  //  USART_ITConfig(CANAL2_MO, USART_IT_ERR, ENABLE);

  //Дозволяємо CANAL1_MO
  CANAL1_MO->CR1 |= USART_CR1_UE;
  //Дозволяємо CANAL2_MO
  CANAL2_MO->CR1 |= USART_CR1_UE;

  //Запускаэмо канали DMA  на приймання
  DMA_StreamCANAL1_MO_Rx->CR |= (uint32_t) DMA_SxCR_EN;
  /*------------------------------*/
  DMA_StreamCANAL2_MO_Rx->CR |= (uint32_t) DMA_SxCR_EN;
  /**********************/
#endif

  /**********************/
  //Настроювання TIM2 на генерацію переривань кожні 1 мс для системи захистів
  /**********************/
  /* Настроюємо TIM2 */
  /* ---------------------------------------------------------------
  TIM2 Настроювання: Output Compare Timing Mode:
  TIM2CLK = 30*2 МГц, Prescaler = 60, TIM2 counter clock = 1 МГц
  CC1 update rate = TIM2 counter clock / CCR1_Val = 1000 Hz
  --------------------------------------------------------------- */
  TIM_TimeBaseStructure.TIM_Period = 0xffffffff;
  TIM_TimeBaseStructure.TIM_Prescaler = (60 - 1);
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  /* Output Compare Timing Mode настроювання: Канал:1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
  TIM_OCInitStructure.TIM_Pulse = TIM2_CCR1_VAL;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OC1Init(TIM2, &TIM_OCInitStructure);

  TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Disable);

  /* Дозволяємо переривання від каналу 1 таймера 2*/
  TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);
  /**********************/

  /**********************/
  //Настроювання TIM5 на генерацію переривань кожні 0,625 мс
  /**********************/
  /* Настроюємо TIM5 */
  /* ---------------------------------------------------------------
  TIM5 Настроювання: Output Compare Timing Mode:
  TIM2CLK = 30*2 МГц, Prescaler = 1, TIM5 counter clock = 60 МГц
  CC1 update rate = TIM5 counter clock / CCR1_Val = 1600 Hz
  --------------------------------------------------------------- */
  TIM_TimeBaseStructure.TIM_Period = 0xffffffff;
  TIM_TimeBaseStructure.TIM_Prescaler = (1 - 1);
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

  /* Output Compare Timing Mode настроювання: Канал:1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
  TIM_OCInitStructure.TIM_Pulse = step_timer_adc;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OC1Init(TIM5, &TIM_OCInitStructure);
  TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Disable);

  TIM_OCInitStructure.TIM_Pulse = TIM5_CCR1_2_VAL;
  TIM_OC2Init(TIM5, &TIM_OCInitStructure);
  TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Disable);

  /* Дозволяємо переривання від каналу 1 таймера 5*/
  TIM_ITConfig(TIM5, TIM_IT_CC1, ENABLE);
  /* Дозволяємо переривання від каналу 2 таймера 5*/
  TIM_ITConfig(TIM5, TIM_IT_CC2, ENABLE);
  /**********************/

  //Робота з watchdogs
  watchdog_routine(WATCHDOG_KYYBOARD, 51);

  //  /**********************/
  //  //Ініціалізація USB
  //  /**********************/
  //  USBD_Init(&USB_OTG_dev,
  //#ifdef USE_USB_OTG_HS
  //            USB_OTG_HS_CORE_ID,
  //#else
  //            USB_OTG_FS_CORE_ID,
  //#endif
  //            &USR_desc,
  //            &USBD_CDC_cb,
  //            &USR_cb);
  //  /**********************/

  //  //Робота з watchdogs
  //  if ((control_word_of_watchdog & WATCHDOG_KYYBOARD) == WATCHDOG_KYYBOARD)
  //  {
  //    //Змінюємо стан біту зовнішнього Watchdog на протилежний
  //    GPIO_WriteBit(
  //                  GPIO_EXTERNAL_WATCHDOG,
  //                  GPIO_PIN_EXTERNAL_WATCHDOG,
  //                  (BitAction)(1 - GPIO_ReadOutputDataBit(GPIO_EXTERNAL_WATCHDOG, GPIO_PIN_EXTERNAL_WATCHDOG))
  //                 );
  //    control_word_of_watchdog =  0;
  //  }
}
/**************************************/

/**************************************/
//Мінімальна конфігурація
/**************************************/
void min_settings(__SETTINGS *target_label)
{
  target_label->device_id = ((VERSIA_PZ << 8) | (MODYFIKACIA_VERSII_PZ));

  target_label->type_of_input = 0x0;
  target_label->type_of_input_signal = 0x0;

  for (unsigned int i = 0; i < NUMBER_INPUTS; i++)
  {
    for (size_t j = 0; j < N_SMALL; ++j)
      target_label->ranguvannja_inputs[N_SMALL * i + j] = 0x0;

    if ((target_label->type_of_input_signal & (1 << i)) != 0)
    {
      target_label->dopusk_dv[i] = KOEF_DOPUSK_DV_ZMIN_MIN;
      if ((target_label->dopusk_dv[i] % 10) != 0)
      {
        target_label->dopusk_dv[i] = (target_label->dopusk_dv[i] / 10) * 10;
      }
    }
    else
      target_label->dopusk_dv[i] = KOEF_DOPUSK_DV_POST_MIN;
  }

  target_label->type_of_output = 0x0;
  target_label->type_of_output_modif = 0x0;
  for (unsigned int i = 0; i < NUMBER_OUTPUTS; i++)
  {
    for (unsigned int j = 0; j < N_BIG; j++)
      target_label->ranguvannja_outputs[N_BIG * i + j] = 0x0;
  }

  target_label->type_of_led = 0x0;
  for (unsigned int i = 0; i < NUMBER_LEDS; i++)
  {
    for (unsigned int j = 0; j < N_BIG; j++)
      target_label->ranguvannja_leds[N_BIG * i + j] = 0x0;
  }

  for (unsigned int j = 0; j < N_BIG; j++)
  {
    target_label->ranguvannja_analog_registrator[j] = 0x0;
    target_label->ranguvannja_digital_registrator[j] = 0x0;

    target_label->ranguvannja_off_cb[j] = 0x0;
    target_label->ranguvannja_on_cb[j] = 0x0;
  }

  target_label->number_iteration_el = NUMBER_ITERATION_EL_MAX;
  //  target_label->number_defined_df = NUMBER_DEFINED_FUNCTIONS_MAX;
  //  target_label->number_defined_dt = NUMBER_DEFINED_TRIGGERS_MAX;
  //  target_label->number_defined_and = NUMBER_DEFINED_AND_MAX;
  //  target_label->number_defined_or = NUMBER_DEFINED_OR_MAX;
  //  target_label->number_defined_xor = NUMBER_DEFINED_XOR_MAX;
  //  target_label->number_defined_not = NUMBER_DEFINED_NOT_MAX;

  target_label->type_df = 0x0;

  for (unsigned int i = 0; i < NUMBER_DEFINED_FUNCTIONS; i++)
  {
    target_label->timeout_pause_df[i] = TIMEOUT_DF_PAUSE_MIN;
    target_label->timeout_work_df[i] = TIMEOUT_DF_WORK_MIN;

    for (unsigned int j = 0; j < N_BIG; j++)
    {
      target_label->ranguvannja_df_source_plus[N_BIG * i + j] = 0x0;
      target_label->ranguvannja_df_source_minus[N_BIG * i + j] = 0x0;
      target_label->ranguvannja_df_source_blk[N_BIG * i + j] = 0x0;
      target_label->ranguvannja_df_source_reset[N_BIG * i + j] = 0x0;
    }
  }

  for (unsigned int i = 0; i < NUMBER_DEFINED_TRIGGERS; i++)
  {
    for (unsigned int j = 0; j < N_BIG; j++)
    {
      target_label->ranguvannja_set_dt_source_plus[N_BIG * i + j] = 0x0;
      target_label->ranguvannja_set_dt_source_minus[N_BIG * i + j] = 0x0;
      target_label->ranguvannja_reset_dt_source_plus[N_BIG * i + j] = 0x0;
      target_label->ranguvannja_reset_dt_source_minus[N_BIG * i + j] = 0x0;
    }
  }

  for (unsigned int i = 0; i < NUMBER_DEFINED_AND; i++)
  {
    for (unsigned int j = 0; j < N_BIG; j++)
      target_label->ranguvannja_d_and[N_BIG * i + j] = 0x0;
  }

  for (unsigned int i = 0; i < NUMBER_DEFINED_OR; i++)
  {
    for (unsigned int j = 0; j < N_BIG; j++)
      target_label->ranguvannja_d_or[N_BIG * i + j] = 0x0;
  }

  for (unsigned int i = 0; i < NUMBER_DEFINED_XOR; i++)
  {
    for (unsigned int j = 0; j < N_BIG; j++)
      target_label->ranguvannja_d_xor[N_BIG * i + j] = 0x0;
  }

  for (unsigned int i = 0; i < NUMBER_DEFINED_NOT; i++)
  {
    for (unsigned int j = 0; j < N_BIG; j++)
      target_label->ranguvannja_d_not[N_BIG * i + j] = 0x0;
  }

  for (unsigned int i = 0; i < NUMBER_TRANSFER_FUNCTIONS; i++)
    target_label->ranguvannja_tf[i] = 0x0;

  target_label->buttons_mode = 0;
  for (unsigned int i = 0; i < NUMBER_DEFINED_BUTTONS; i++)
  {
    for (size_t j = 0; j < N_SMALL; ++j)
      target_label->ranguvannja_buttons[N_SMALL * i + j] = 0x0;
  }

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
  for (size_t i = 0; i < N_IN_GOOSE; i++)
  {
    for (size_t j = 0; j < N_IN_GOOSE_MMS_OUT; j++)
    {
      for (size_t k = 0; k < N_SMALL; k++)
        target_label->ranguvannja_In_GOOSE[i][j][k] = 0;
    }
  }

  for (size_t i = 0; i < N_IN_MMS; i++)
  {
    for (size_t j = 0; j < N_IN_GOOSE_MMS_OUT; j++)
    {
      for (size_t k = 0; k < N_SMALL; k++)
        target_label->ranguvannja_In_MMS[i][j][k] = 0;
    }
  }

  for (size_t i = 0; i < N_OUT_LAN; i++)
  {
    for (size_t j = 0; j < N_OUT_LAN_IN; j++)
    {
      for (size_t k = 0; k < MAX_FUNCTIONS_IN_OUT_LAN; k++)
        target_label->ranguvannja_Out_LAN[i][j][k] = 0;
    }
  }
#endif

  target_label->configuration = 0;

  target_label->ctrl_zdz_type = ZDZ_CTRL_MIN;
#if (                              \
  (MODYFIKACIA_VERSII_PZ == 0) ||  \
  (MODYFIKACIA_VERSII_PZ == 3) ||  \
  (MODYFIKACIA_VERSII_PZ == 4) ||  \
  (MODYFIKACIA_VERSII_PZ == 6) ||  \
  (MODYFIKACIA_VERSII_PZ == 7) ||  \
  (MODYFIKACIA_VERSII_PZ == 10) || \
  (MODYFIKACIA_VERSII_PZ == 13) || \
  (MODYFIKACIA_VERSII_PZ == 14) || \
  (MODYFIKACIA_VERSII_PZ == 17) || \
  (MODYFIKACIA_VERSII_PZ == 14))
  target_label->zdz_ovd_porig = 0;
#endif
  for (size_t i = 0; i < NUMBER_UP; i++)
    target_label->ctrl_UP_input[i] = UP_CTRL_Ia_Ib_Ic;

  target_label->control_mtz = 0;
  target_label->control_mtz04 = 0;
  target_label->control_znam = 0;
  target_label->control_zdz = 0;
  target_label->control_zz = 0;
  target_label->control_tznp = 0;
  target_label->control_apv = 0;
  target_label->control_achr_chapv = 0;
  target_label->control_urov = 0;
  target_label->control_zop = 0;
  target_label->control_Umin = 0;
  target_label->control_Umax = 0;
  target_label->control_UP = 0;

  target_label->grupa_ustavok = SETPOINT_GRUPA_USTAVOK_MIN;

  for (unsigned int i = 0; i < NUMBER_GROUP_USTAVOK; i++)
  {
    target_label->setpoint_rpn_osn[i] = SETPOINT_RPN_OSN_WORK;
    target_label->setpoint_rpn_zony[i] = SETPOINT_RPN_ZONY_WORK;
    target_label->setpoint_rpn_per[i] = SETPOINT_RPN_PER_WORK;
    target_label->setpoint_rpn_dod[i] = SETPOINT_RPN_DOD_WORK;
    target_label->setpoint_rpn_K[i] = SETPOINT_RPN_K_MAX;

    target_label->timeout_mtz_1[i] = TIMEOUT_MTZ1_MIN;
    target_label->timeout_mtz_1_n_vpered[i] = TIMEOUT_MTZ1_N_VPERED_MIN;
    target_label->timeout_mtz_1_n_nazad[i] = TIMEOUT_MTZ1_N_NAZAD_MIN;
    target_label->timeout_mtz_1_po_napruzi[i] = TIMEOUT_MTZ1_PO_NAPRUZI_MIN;
    target_label->timeout_mtz_1_k_znam[i] = TIMEOUT_MTZ1_ZNAM_MIN;

    target_label->timeout_mtz_2[i] = TIMEOUT_MTZ2_MIN;
    target_label->timeout_mtz_2_pr[i] = TIMEOUT_MTZ2_PR_MIN;
    target_label->timeout_mtz_2_n_vpered[i] = TIMEOUT_MTZ2_N_VPERED_MIN;
    target_label->timeout_mtz_2_n_vpered_pr[i] = TIMEOUT_MTZ2_N_VPERED_PR_MIN;
    target_label->timeout_mtz_2_n_nazad[i] = TIMEOUT_MTZ2_N_NAZAD_MIN;
    target_label->timeout_mtz_2_n_nazad_pr[i] = TIMEOUT_MTZ2_N_NAZAD_PR_MIN;
    target_label->timeout_mtz_2_po_napruzi[i] = TIMEOUT_MTZ2_PO_NAPRUZI_MIN;
    target_label->timeout_mtz_2_po_napruzi_pr[i] = TIMEOUT_MTZ2_PO_NAPRUZI_PR_MIN;
    target_label->timeout_mtz_2_vvid_pr[i] = TIMEOUT_MTZ2_VVID_PR_MIN;
    target_label->timeout_mtz_2_k_znam[i] = TIMEOUT_MTZ2_ZNAM_MIN;

    target_label->timeout_mtz_3[i] = TIMEOUT_MTZ3_MIN;
    target_label->timeout_mtz_3_n_vpered[i] = TIMEOUT_MTZ3_N_VPERED_MIN;
    target_label->timeout_mtz_3_n_nazad[i] = TIMEOUT_MTZ3_N_NAZAD_MIN;
    target_label->timeout_mtz_3_po_napruzi[i] = TIMEOUT_MTZ3_PO_NAPRUZI_MIN;
    target_label->timeout_mtz_3_k_znam[i] = TIMEOUT_MTZ3_ZNAM_MIN;

    target_label->timeout_mtz_4[i] = TIMEOUT_MTZ4_MIN;
    target_label->timeout_mtz_4_n_vpered[i] = TIMEOUT_MTZ4_N_VPERED_MIN;
    target_label->timeout_mtz_4_n_nazad[i] = TIMEOUT_MTZ4_N_NAZAD_MIN;
    target_label->timeout_mtz_4_po_napruzi[i] = TIMEOUT_MTZ4_PO_NAPRUZI_MIN;
    target_label->timeout_mtz_4_k_znam[i] = TIMEOUT_MTZ4_ZNAM_MIN;

    target_label->setpoint_mtz04_1[i] = SETPOINT_MTZ04_1_MIN;
    target_label->setpoint_mtz04_2[i] = SETPOINT_MTZ04_2_MIN;

    target_label->timeout_mtz04_1[i] = TIMEOUT_MTZ04_1_MIN;
    target_label->timeout_mtz04_2[i] = TIMEOUT_MTZ04_2_MIN;
    target_label->timeout_mtz04_2_pr[i] = TIMEOUT_MTZ04_2_PR_MIN;
    target_label->timeout_mtz04_2_vvid_pr[i] = TIMEOUT_MTZ04_2_VVID_PR_MIN;

    target_label->setpoint_znam[i] = SETPOINT_ZNAM_MIN;
    target_label->timeout_znam[i] = TIMEOUT_ZNAM_MIN;

    target_label->timeout_zdz[i] = TIMEOUT_ZDZ_MIN;

    target_label->setpoint_zz_3I0[i] = SETPOINT_ZZ_3I0_MIN;
    target_label->setpoint_zz_3U0[i] = SETPOINT_ZZ_3U0_MIN;
    target_label->timeout_zz_3I0[i] = TIMEOUT_ZZ_3I0_MIN;
    target_label->timeout_zz_3U0[i] = TIMEOUT_ZZ_3U0_MIN;
    target_label->timeout_nzz[i] = TIMEOUT_NZZ_MIN;

    target_label->setpoint_tznp_1_3I0_vpered[i] = SETPOINT_TZNP1_3I0_VPERED_MIN;
    target_label->setpoint_tznp_1_3U0_vpered[i] = SETPOINT_TZNP1_3U0_VPERED_MIN;
    target_label->setpoint_tznp_1_3I0_nazad[i] = SETPOINT_TZNP1_3I0_NAZAD_MIN;
    target_label->setpoint_tznp_1_3U0_nazad[i] = SETPOINT_TZNP1_3U0_NAZAD_MIN;

    target_label->setpoint_tznp_1_angle[i] = SETPOINT_TZNP1_ANGLE_MIN;

    target_label->setpoint_tznp_2_3I0_vpered[i] = SETPOINT_TZNP2_3I0_VPERED_MIN;
    target_label->setpoint_tznp_2_3U0_vpered[i] = SETPOINT_TZNP2_3U0_VPERED_MIN;
    target_label->setpoint_tznp_2_3I0_nazad[i] = SETPOINT_TZNP2_3I0_NAZAD_MIN;
    target_label->setpoint_tznp_2_3U0_nazad[i] = SETPOINT_TZNP2_3U0_NAZAD_MIN;

    target_label->setpoint_tznp_2_angle[i] = SETPOINT_TZNP2_ANGLE_MIN;

    target_label->setpoint_tznp_3_3I0_vpered[i] = SETPOINT_TZNP3_3I0_VPERED_MIN;
    target_label->setpoint_tznp_3_3U0_vpered[i] = SETPOINT_TZNP3_3U0_VPERED_MIN;
    target_label->setpoint_tznp_3_3I0_nazad[i] = SETPOINT_TZNP3_3I0_NAZAD_MIN;
    target_label->setpoint_tznp_3_3U0_nazad[i] = SETPOINT_TZNP3_3U0_NAZAD_MIN;

    target_label->setpoint_tznp_3_angle[i] = SETPOINT_TZNP3_ANGLE_MIN;

    target_label->timeout_tznp_1_vpered[i] = TIMEOUT_TZNP1_VPERED_MIN;
    target_label->timeout_tznp_1_nazad[i] = TIMEOUT_TZNP1_NAZAD_MIN;
    target_label->timeout_tznp_2_vpered[i] = TIMEOUT_TZNP2_VPERED_MIN;
    target_label->timeout_tznp_2_nazad[i] = TIMEOUT_TZNP2_NAZAD_MIN;
    target_label->timeout_tznp_3_vpered[i] = TIMEOUT_TZNP3_VPERED_MIN;
    target_label->timeout_tznp_3_nazad[i] = TIMEOUT_TZNP3_NAZAD_MIN;

    target_label->timeout_apv_1[i] = TIMEOUT_APV1_MIN;
    target_label->timeout_apv_2[i] = TIMEOUT_APV2_MIN;
    target_label->timeout_apv_3[i] = TIMEOUT_APV3_MIN;
    target_label->timeout_apv_4[i] = TIMEOUT_APV4_MIN;
    target_label->timeout_apv_block_vid_apv1[i] = TIMEOUT_APV_BLOCK_VID_APV1_MIN;
    target_label->timeout_apv_block_vid_apv2[i] = TIMEOUT_APV_BLOCK_VID_APV2_MIN;
    target_label->timeout_apv_block_vid_apv3[i] = TIMEOUT_APV_BLOCK_VID_APV3_MIN;
    target_label->timeout_apv_block_vid_apv4[i] = TIMEOUT_APV_BLOCK_VID_APV4_MIN;
    target_label->timeout_apv_block_vid_VV[i] = TIMEOUT_APV_BLOCK_VID_VV_MIN;

    target_label->setpoint_achr_chapv_uf[i] = SETPOINT_ACHR_CHAPV_UF_MIN;
    target_label->setpoint_achr1_f_rab[i] = SETPOINT_ACHR1_F_RAB_MIN;
    target_label->setpoint_chapv1_f_rab[i] = MAIN_FREQUENCY * 100 - 30;
    target_label->setpoint_achr2_f_rab[i] = SETPOINT_ACHR2_F_RAB_MIN;
    target_label->setpoint_chapv2_f_rab[i] = MAIN_FREQUENCY * 100 - 30;

    target_label->timeout_achr_1[i] = TIMEOUT_ACHR1_MIN;
    target_label->timeout_chapv_1[i] = TIMEOUT_CHAPV1_MIN;
    target_label->timeout_achr_2[i] = TIMEOUT_ACHR2_MIN;
    target_label->timeout_chapv_2[i] = TIMEOUT_CHAPV2_MIN;

    target_label->setpoint_urov[i] = SETPOINT_UROV_MIN;
    target_label->timeout_urov_1[i] = TIMEOUT_UROV1_MIN;
    target_label->timeout_urov_2[i] = TIMEOUT_UROV2_MIN;

    target_label->setpoint_zop[i] = SETPOINT_ZOP_MIN;
    target_label->timeout_zop[i] = TIMEOUT_ZOP_MIN;

    target_label->setpoint_Umin1[i] = SETPOINT_UMIN1_MIN;
    target_label->setpoint_Umin1_Iblk[i] = SETPOINT_UMIN1_IBLK_MIN;
    target_label->setpoint_Umin2[i] = SETPOINT_UMIN2_MIN;
    target_label->setpoint_Umin2_Iblk[i] = SETPOINT_UMIN2_IBLK_MIN;
    target_label->timeout_Umin1[i] = TIMEOUT_UMIN1_MIN;
    target_label->timeout_Umin2[i] = TIMEOUT_UMIN2_MIN;

    target_label->setpoint_Umax1[i] = SETPOINT_UMAX1_MIN;
    target_label->setpoint_Umax2[i] = SETPOINT_UMAX2_MIN;
    target_label->setpoint_kp_Umax[i] = SETPOINT_KP_UMAX_DEFAULT;
    target_label->timeout_Umax1[i] = TIMEOUT_UMAX1_MIN;
    target_label->timeout_Umax2[i] = TIMEOUT_UMAX2_MIN;

    for (size_t j = 0; j < NUMBER_UP; j++)
    {
      def_pickup_timeout_UP(target_label, j, i);
    }
  }

  target_label->setpoint_Inom = SETPOINT_Inom_MIN;
  target_label->setpoint_r_kom_st_Inom = SETPOINT_RKS_Inom_MIN;
  target_label->setpoint_Inom_vymk = SETPOINT_Inom_vymk_MIN;
  target_label->setpoint_r_kom_st_Inom_vymk = SETPOINT_RKS_Inom_vymk_MIN;

  unsigned int chastka = target_label->setpoint_r_kom_st_Inom / target_label->setpoint_r_kom_st_Inom_vymk;
  target_label->setpoint_pochatkovyj_resurs = 2 * chastka;
  target_label->setpoint_krytychnyj_resurs = chastka;
  target_label->setpoint_pochatkova_k_vymk = 0;

  target_label->timeout_swch_on = TIMEOUT_SWCH_ON_MIN;
  target_label->timeout_swch_off = TIMEOUT_SWCH_OFF_MIN;
  target_label->timeout_swch_udl_blk_on = TIMEOUT_SWCH_UDL_BLK_ON_MIN;
  target_label->timeout_pryvoda_VV = TIMEOUT_PRYVODA_VV_MIN;
  target_label->control_switch = 0;

  for (unsigned int i = 0; i < 2; i++)
  {
    if (i == 0)
      target_label->lines[0] = NUMBER_LINES_FORWARD_MIN;
    else
      target_label->lines[i] = NUMBER_LINES_BACKWARD_MIN;

    for (unsigned int j = 0; j < MAX_NUMBER_LINES_VMP; j++)
    {
      target_label->dovgyna[i][j] = SETPOINT_DOVGYNA_VMP_MIN;
      target_label->opir[i][j] = SETPOINT_OPIR_VMP_MIN;
    }
  }
  target_label->control_vmp = 0;

  target_label->prefault_number_periods = TIMEOUT_PREFAULT_MIN / 20;
  target_label->postfault_number_periods = TIMEOUT_POSTFAULT_MIN / 20;
  target_label->control_ar = 0;

  target_label->timeout_prolongation_work_digital_registrator = TIMEOUT_DR_ELONGATION_MIN;

  target_label->password1 = 0;
  target_label->password2 = 1234;
  target_label->password3 = 4321;
  target_label->timeout_deactivation_password_interface_USB = TIMEOUT_DEACTIVATION_PASSWORD_MIN;
  target_label->password_interface_USB = 0;
  target_label->timeout_deactivation_password_interface_RS485 = TIMEOUT_DEACTIVATION_PASSWORD_MIN;
  target_label->password_interface_RS485 = 0;
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
  target_label->timeout_deactivation_password_interface_LAN = TIMEOUT_DEACTIVATION_PASSWORD_MIN;
  target_label->password_interface_LAN = 0;
#endif

  target_label->timeout_idle_new_settings = TIMEOUT_NEW_SETTINGS_MIN;

  target_label->T0 = KOEF_TO_MIN;
  target_label->TCurrent = KOEF_TT_MIN;
  target_label->TCurrent04 = KOEF_TT04_MIN;
  target_label->TVoltage = KOEF_TN_MIN;
  target_label->control_transformator = MASKA_FOR_BIT(INDEX_ML_CTR_TRANSFORMATOR_PHASE_LINE);

  for (unsigned int i = 0; i < ((M_ADDRESS_LAST_USER_REGISTER_DATA - M_ADDRESS_FIRST_USER_REGISTER_DATA) + 1); i++)
    target_label->user_register[i] = 0;

  for (unsigned int i = 0; i < MAX_CHAR_IN_NAME_OF_CELL; i++)
    target_label->name_of_cell[i] = ' ';
  target_label->name_of_cell[4] = 'N';
  target_label->name_of_cell[5] = 'o';
  target_label->name_of_cell[7] = 'n';
  target_label->name_of_cell[8] = 'a';
  target_label->name_of_cell[9] = 'm';
  target_label->name_of_cell[10] = 'e';

  target_label->address = KOEF_ADDRESS_MIN;
  target_label->speed_RS485 = VALUE_SPEED_INTERFACE_MIN;
  target_label->pare_bit_RS485 = VALUE_PARE_INTERFACE_MAX; //томущо восновному контроль парності має бути в EVEN - а це число відповідає VALUE_PARE_INTERFACE_MAX
  target_label->number_stop_bit_RS485 = VALUE_STOP_BITS_INTERFACE_MIN;
  target_label->time_out_1_RS485 = VALUE_TIME_OUT_1_INTERFACE_MIN;

  target_label->language = LANGUAGE_EN;

  target_label->control_extra_settings_1 = 0;

  target_label->time_zone = 2;
  target_label->dst = MASKA_FOR_BIT(N_BIT_TZ_DST);
  target_label->dst_on_rule = (_Mar << POS_MM) | (_NL << POS_WR) | (_Sun << POS_DOW) | (3 << POS_HH);
  target_label->dst_off_rule = (_Oct << POS_MM) | (_NL << POS_WR) | (_Sun << POS_DOW) | (3 << POS_HH);

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)

  target_label->IP4[0] = 192;
  target_label->IP4[1] = 168;
  target_label->IP4[2] = 0;
  target_label->IP4[3] = 11;

  target_label->mask = NETWORK_MASK_DEFAULT;

  target_label->gateway[0] = 0;
  target_label->gateway[1] = 0;
  target_label->gateway[2] = 0;
  target_label->gateway[3] = 0;

  target_label->IP_time_server[0] = 192;
  target_label->IP_time_server[1] = 168;
  target_label->IP_time_server[2] = 100;
  target_label->IP_time_server[3] = 10;

  target_label->port_time_server = 123;

  target_label->period_sync = 300;

#endif

  target_label->time_setpoints = target_label->time_ranguvannja = 0;
  target_label->source_setpoints = target_label->source_ranguvannja = 0;
}
/**************************************/

/**************************************/
//Виводимо повідомлення про те, що настройки не зчитані успішно
/**************************************/
void error_reading_with_eeprom()
{
  static unsigned char const string_info[MAX_NAMBER_LANGUAGE][4][MAX_COL_LCD] =
    {
      {"  Настроек нет  ",
       " Ош.к.с.настр.  ",
       " Ин.ЭнНезСЛ Нет ",
       "  Ош.Ин.ЭнНезСЛ "},
      {"Налаштувань нема",
       " Пом.к.с.налашт.",
       " Ін.ЕнНезСЛ Нема",
       " Пом.Ін.ЕнНезСЛ "},
      {"Param are absent",
       "Error Param chks",
       " No Nvol LS Inf ",
       " Nvol LS Inf Er "},
      {"  Настроек нет  ",
       " Ош.к.с.настр.  ",
       " Ин.ЭнНезСЛ Нет ",
       "  Ош.Ин.ЭнНезСЛ "}};
  static unsigned char const string_action[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] =
    {
      {"Мин.парам.:Enter",
       " Сбросить: Enter"},
      {"Мін.парам.:Enter",
       " Скинути: Enter "},
      {"Min.param :Enter",
       "  Reset: Enter  "},
      {"Мин.парам.:Enter",
       " Сбросить: Enter"}};

  int index_language;
  if ((state_spi1_task & STATE_SETTINGS_EEPROM_GOOD) == 0)
    index_language = index_language_in_array(LANGUAGE_ABSENT);
  else
    index_language = index_language_in_array(current_settings.language);

  if (
    ((state_spi1_task & (STATE_SETTINGS_EEPROM_EMPTY | STATE_SETTINGS_EEPROM_FAIL)) != 0) ||
    ((state_spi1_task & (STATE_TRG_FUNC_EEPROM_EMPTY | STATE_TRG_FUNC_EEPROM_FAIL)) != 0))
  {
    //Робота з watchdogs
    watchdog_routine(WATCHDOG_KYYBOARD, 52);

    unsigned int index_info = 0, index_action = 0, information_type = 0;
    if ((state_spi1_task & STATE_SETTINGS_EEPROM_EMPTY) != 0)
    {
      index_info = 0;
      index_action = 0;
      information_type = 1;
    }
    else if ((state_spi1_task & STATE_SETTINGS_EEPROM_FAIL) != 0)
    {
      index_info = 1;
      index_action = 0;
      information_type = 1;
    }
    else if ((state_spi1_task & STATE_TRG_FUNC_EEPROM_EMPTY) != 0)
    {
      index_info = 2;
      index_action = 1;
      information_type = 2;
    }
    else if ((state_spi1_task & STATE_TRG_FUNC_EEPROM_FAIL) != 0)
    {
      index_info = 3;
      index_action = 1;
      information_type = 2;
    }
    else
    {
      //Теоретично цього ніколи не мало б бути
      total_error_sw_fixed();
    }

    //Копіюємо  рядки у робочий екран
    for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
    {
      for (unsigned int j = 0; j < MAX_COL_LCD; j++)
      {
        if (i == 0)
          working_ekran[i][j] = string_info[index_language][index_info][j];
        else if (i == 1)
          working_ekran[i][j] = string_action[index_language][index_action][j];
        else
          working_ekran[i][j] = ' ';
      }
    }

    //Обновити повністю весь екран
    current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;

    //Обновляємо інформацію на екрані
    view_whole_ekran();

    //Чекаємо поки не буде натиснута кнопка ENTER
    while ((new_state_keyboard & (1 << BIT_KEY_ENTER)) == 0)
    {
      //Робота з watchdogs
      watchdog_routine(WATCHDOG_KYYBOARD, 53);
    }

    new_state_keyboard = 0;

    if (information_type == 1)
    {
      //Помічаємо, що таблиця зараз буде змінилася і її треба буде з системи захистів зкопіювати у таблицю з якою працює система захистів
      changed_settings = CHANGED_ETAP_EXECUTION;
      //Заповнюємо мінімальну конфігурацію
      min_settings(&current_settings);
      current_settings_interfaces = current_settings;

      //Записуємо мінімальну конфігурацію
      _SET_BIT(control_spi1_taskes, TASK_START_WRITE_SETTINGS_EEPROM_BIT);

      //Помічаємо, що таблиця змінилася і її треба буде з системи захистів зкопіювати у таблицю з якою працює система захистів
      changed_settings = CHANGED_ETAP_ENDED;
    }
    else if (information_type == 2)
    {
      fix_active_buttons = 0;
      for (unsigned int i = 0; i < N_BIG; i++)
        trigger_active_functions[i] = 0x0;

      //Записуємо очищену триґерну інформацію
      _SET_BIT(control_spi1_taskes, TASK_START_WRITE_TRG_FUNC_EEPROM_BIT);
    }

    //Чекаємо завершення запису
    while (
      (control_spi1_taskes[0] != 0) ||
      (control_spi1_taskes[1] != 0) ||
      (state_execution_spi1 > 0))
    {
      //Робота з watchdogs
      watchdog_routine(WATCHDOG_KYYBOARD, 54);

      main_routines_for_spi1();
      changing_diagnostyka_state(); //Підготовлюємо новий потенційно можливий запис для реєстратора програмних подій
      //Оскільки ще тамер вимірювальної системи не запущений, то цю операцію треба робити тут
    }

    if (information_type == 1)
    {
      //Повтрокно зчитуємо налаштування
      comparison_writing &= (unsigned int) (~COMPARISON_WRITING_SETTINGS); /*зчитування, а не порівняння*/
      _SET_BIT(control_spi1_taskes, TASK_START_READ_SETTINGS_EEPROM_BIT);
    }
    else if (information_type == 2)
    {
      //Повтрокно зчитуємо триґерну інформацію
      comparison_writing &= (unsigned int) (~COMPARISON_WRITING_TRG_FUNC); /*зчитування, а не порівняння*/
      _SET_BIT(control_spi1_taskes, TASK_START_READ_TRG_FUNC_EEPROM_BIT);
    }

    //Чекаємо завершення читання
    while (
      (control_spi1_taskes[0] != 0) ||
      (control_spi1_taskes[1] != 0) ||
      (state_execution_spi1 > 0))
    {
      //Робота з watchdogs
      watchdog_routine(WATCHDOG_KYYBOARD, 55);

      main_routines_for_spi1();
      changing_diagnostyka_state(); //Підготовлюємо новий потенційно можливий запис для реєстратора програмних подій
      //Оскільки ще тамер вимірювальної системи не запущений, то цю операцію треба робити тут
    }
  }
}
/**************************************/

/**************************************/
//стартова перевірка (і при необхідності перенастройка на потрібні параметри) DataFlash
/**************************************/
void start_checking_dataflash(void)
{
  //Читаємо статуми мікросхеми 1
  unsigned char page_size_256, page_size_256_total = 1;
  unsigned int ready_busy;
  number_chip_dataflsh_exchange = INDEX_DATAFLASH_1;

  for (unsigned int i = 0; i < NUMBER_DATAFLASH_CHIP; i++)
  {
    page_size_256 = 1;
    dataflash_status_read(number_chip_dataflsh_exchange);
    while (driver_spi_df[number_chip_dataflsh_exchange].state_execution != TRANSACTION_EXECUTED_WAIT_ANALIZE)
    {
      if (driver_spi_df[number_chip_dataflsh_exchange].state_execution == TRANSACTION_EXECUTING_NONE)
      {
        /*
        ця ситуація могла виникнути тільки в одному випадку - якщо в процесі прийом/передачі
        зафісована була помилка у SPI_EDF, тому повторно виконуємо запуск читання регістру статусу
        */
        dataflash_status_read(number_chip_dataflsh_exchange);
      }

      //Робота з watchdogs
      watchdog_routine(WATCHDOG_KYYBOARD, 56);
    }
    page_size_256 &= RxBuffer_SPI_EDF[1] & (1 << 0);
    ready_busy = RxBuffer_SPI_EDF[1] & (1 << 7);
    driver_spi_df[number_chip_dataflsh_exchange].state_execution = TRANSACTION_EXECUTING_NONE;
    driver_spi_df[number_chip_dataflsh_exchange].code_operation = CODE_OPERATION_NONE;
    if (page_size_256 == 0)
    {
      //Треба подати команду на перевід мікросхеми з розміром сторінки 256 байт

      //Перевіряємо, чи мікросхема зараз є вільною по біту Ready/Busy
      while (ready_busy == 0) /*перший раз біт ready/busy беремо з попередньої операції читання регістру статусу*/
      {
        dataflash_status_read(number_chip_dataflsh_exchange);
        while (driver_spi_df[number_chip_dataflsh_exchange].state_execution != TRANSACTION_EXECUTED_WAIT_ANALIZE)
        {
          if (driver_spi_df[number_chip_dataflsh_exchange].state_execution == TRANSACTION_EXECUTING_NONE)
          {
            /*
            ця ситуація могла виникнути тільки в одному випадку - якщо в процесі прийом/передачі
            зафісована була помилка у SPI_EDF, тому повторно виконуємо запуск читання регістру статусу
            */
            dataflash_status_read(number_chip_dataflsh_exchange);
          }

          //Робота з watchdogs
          watchdog_routine(WATCHDOG_KYYBOARD, 57);
        }
        ready_busy = RxBuffer_SPI_EDF[1] & (1 << 7);
        driver_spi_df[number_chip_dataflsh_exchange].state_execution = TRANSACTION_EXECUTING_NONE;
        driver_spi_df[number_chip_dataflsh_exchange].code_operation = CODE_OPERATION_NONE;
      }

      //Подаємо команду на переналаштування DataFlash
      dataflash_set_pagesize_256(number_chip_dataflsh_exchange);
      while (driver_spi_df[number_chip_dataflsh_exchange].state_execution != TRANSACTION_EXECUTED_WAIT_ANALIZE)
      {
        /*
        Тут я вирішив не аналізувати випадок збою, бо після виконання цієї операції
        треба буде виклю чити і включити прилад (причому скоріше всього це буде на заводі), 
        тому у гіршому випадку мікросхема скаже, що вона ще не переведена на потрібну ширину сторінки
        */

        //Робота з watchdogs
        watchdog_routine(WATCHDOG_KYYBOARD, 58);
      }
      driver_spi_df[number_chip_dataflsh_exchange].state_execution = TRANSACTION_EXECUTING_NONE;
      driver_spi_df[number_chip_dataflsh_exchange].code_operation = CODE_OPERATION_NONE;

      //Очікуємо, поки попредня команда переналаштування мікросхеми виконається
      do
      {
        dataflash_status_read(number_chip_dataflsh_exchange);
        while (driver_spi_df[number_chip_dataflsh_exchange].state_execution != TRANSACTION_EXECUTED_WAIT_ANALIZE)
        {
          if (driver_spi_df[number_chip_dataflsh_exchange].state_execution == TRANSACTION_EXECUTING_NONE)
          {
            /*
            ця ситуація могла виникнути тільки в одному випадку - якщо в процесі прийом/передачі
            зафісована була помилка у SPI_EDF, тому повторно виконуємо запуск читання регістру статусу
            */
            dataflash_status_read(number_chip_dataflsh_exchange);
          }

          //Робота з watchdogs
          watchdog_routine(WATCHDOG_KYYBOARD, 59);
        }
        ready_busy = RxBuffer_SPI_EDF[1] & (1 << 7);
        driver_spi_df[number_chip_dataflsh_exchange].state_execution = TRANSACTION_EXECUTING_NONE;
        driver_spi_df[number_chip_dataflsh_exchange].code_operation = CODE_OPERATION_NONE;
      } while (ready_busy == 0); /*біт ready/busy беремо з попереднії операцій читання регістру статусу, а з нової операції читання регістру статсусу*/
    }

    page_size_256_total &= page_size_256;
    number_chip_dataflsh_exchange = (number_chip_dataflsh_exchange + 1) & (NUMBER_DATAFLASH_CHIP - 1);
  }

  if (error_into_spi_df != 0)
  {
    /*
    Якщо була зафіксована помилка під час переходу на ширину сторінки 256 байт
    (під час читання регістру статусу змінна error_into_spi_df не встановлюється
    у ненульове значення), то робимо примусовий перезапуск з допомогою внутрішнього
    (і зовнішнього, якщо джампер є) watchdog-а
    */
    while (1)
      ;
  }

  if (page_size_256_total == 0)
  {
    static unsigned char const name_string[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] =
      {
        {" Перезапустите  ",
         "   устройство   "},
        {" Перезапустіть  ",
         "    пристрій    "},
        {"     Reset      ",
         "     Device     "},
        {" Перезапустите  ",
         "   устройство   "}};

    int index_language = index_language_in_array(current_settings.language);

    //Робота з watchdogs
    watchdog_routine(WATCHDOG_KYYBOARD, 60);

    //Копіюємо  рядки у робочий екран
    for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
    {
      if (i < 2)
      {
        for (unsigned int j = 0; j < MAX_COL_LCD; j++)
          working_ekran[i][j] = name_string[index_language][i][j];
      }
      else
      {
        for (unsigned int j = 0; j < MAX_COL_LCD; j++)
          working_ekran[i][j] = ' ';
      }
    }

    //Обновити повністю весь екран
    current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;

    //Обновляємо інформацію на екрані
    view_whole_ekran();

    //Чекаємо, поки пристрій буде перезапущений
    while (1)
    {
      //Робота з watchdogs
      watchdog_routine(WATCHDOG_KYYBOARD, 61);
    }
  }
}
/**************************************/

/**************************************/
//Запускаємо канал 2 таймеру 4 на генерацію переривань кожну 1 мс
/**************************************/
void start_tim4_canal2_for_interrupt_1mc(void)
{
  TIM_OCInitTypeDef TIM_OCInitStructure;

  /* Output Compare Timing Mode настроювання: Канал:2 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
  TIM_OCInitStructure.TIM_Pulse = ((uint16_t) TIM4->CNT) + TIM4_CCR2_VAL;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OC2Init(TIM4, &TIM_OCInitStructure);

  TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Disable);

  /* Дозволяємо переривання від каналу 2 таймера 4*/
  TIM_ITConfig(TIM4, TIM_IT_CC2, ENABLE);

  //  //Обновляємо новий час запуску переривання
  //  TIM4->CCR2 = ((uint16_t)TIM4->CNT) + TIM4_CCR2_VAL;
}
/**************************************/

/**************************************/
//Запускаємо канал 3 таймеру 4 на генерацію переривань як мінімум 10 мкс
/**************************************/
void start_tim4_canal3_for_interrupt_10mkc(void)
{
  TIM_OCInitTypeDef TIM_OCInitStructure;

  if (number_bits_rs_485_waiting < 10)
    calculate_namber_bit_waiting_for_rs_485();

  /* Output Compare Timing Mode настроювання: Канал:2 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
  unsigned int delta = TIM4_CCR3_VAL * number_bits_rs_485_waiting;
  TIM_OCInitStructure.TIM_Pulse = ((uint16_t) TIM4->CNT) + delta;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OC3Init(TIM4, &TIM_OCInitStructure);

  TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Disable);

  /* Дозволяємо переривання від каналу 3 таймера 4*/
  TIM_ITConfig(TIM4, TIM_IT_CC3, ENABLE);
}
/**************************************/

/**************************************/
//
/**************************************/
/**************************************/
