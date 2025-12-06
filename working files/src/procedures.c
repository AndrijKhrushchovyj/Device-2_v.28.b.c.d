#include "header.h"

/*****************************************************/
//Функція обновлення змінних при зміні конфігурації
/*****************************************************/
unsigned int action_after_changing_of_configuration(unsigned int new_configuration, __SETTINGS *target_label)
{
  unsigned int error_window = 0;
  
  /************************/
  //Спершу перевіряємо чи не знаходимося зараз ми у такому вікні, яке забороняє змінювати текучу конфігурацію
  /************************/
  //Перевірка МТЗ
  if ((new_configuration & ( 1<< MTZ_BIT_CONFIGURATION )) == 0)
  {
    if(
       (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_MTZ)
       || 
       (
        (current_ekran.current_level >= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_MTZ) &&
        (current_ekran.current_level <= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP4_MTZ) 
       )  
       ||  
       (
        (current_ekran.current_level >= EKRAN_SETPOINT_MTZ_GROUP1) &&
        (current_ekran.current_level <= EKRAN_SETPOINT_MTZ_GROUP4)
       )
       ||  
       (
        (current_ekran.current_level >= EKRAN_TIMEOUT_MTZ_GROUP1) &&
        (current_ekran.current_level <= EKRAN_TIMEOUT_MTZ_GROUP4)
       )
       ||  
       (current_ekran.current_level == EKRAN_CONTROL_MTZ)
      )
      error_window |= (1 << MTZ_BIT_CONFIGURATION );
  }
  //Перевірка МТЗ 0.4кВ
  if ((new_configuration & ( 1<< MTZ04_BIT_CONFIGURATION )) == 0)
  {
    if(
       (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_MTZ04)
       || 
       (
        (current_ekran.current_level >= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_MTZ04) &&
        (current_ekran.current_level <= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP4_MTZ04) 
       )  
       ||  
       (
        (current_ekran.current_level >= EKRAN_SETPOINT_MTZ04_GROUP1) &&
        (current_ekran.current_level <= EKRAN_SETPOINT_MTZ04_GROUP4)
       )
       ||  
       (
        (current_ekran.current_level >= EKRAN_TIMEOUT_MTZ04_GROUP1) &&
        (current_ekran.current_level <= EKRAN_TIMEOUT_MTZ04_GROUP4)
       )
       ||  
       (current_ekran.current_level == EKRAN_CONTROL_MTZ04)
      )
      error_window |= (1 << MTZ04_BIT_CONFIGURATION );
  }
  //Перевірка ЗНам
  if ((new_configuration & ( 1<< ZNAM_BIT_CONFIGURATION )) == 0)
  {
    if(
       (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_ZNAM)
       || 
       (
        (current_ekran.current_level >= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_ZNAM) &&
        (current_ekran.current_level <= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP4_ZNAM) 
       )  
       ||  
       (
        (current_ekran.current_level >= EKRAN_SETPOINT_ZNAM_GROUP1) &&
        (current_ekran.current_level <= EKRAN_SETPOINT_ZNAM_GROUP4)
       )
       ||  
       (
        (current_ekran.current_level >= EKRAN_TIMEOUT_ZNAM_GROUP1) &&
        (current_ekran.current_level <= EKRAN_TIMEOUT_ZNAM_GROUP4)
       )
       ||  
       (current_ekran.current_level == EKRAN_CONTROL_ZNAM)
      )
      error_window |= (1 << ZNAM_BIT_CONFIGURATION );
  }
  //Перевірка ЗДЗ
  if ((new_configuration & (1<<ZDZ_BIT_CONFIGURATION)) == 0)
  {
    if(
       (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_ZDZ) 
       || 
       (
        (current_ekran.current_level >= EKRAN_CHOOSE_TIMEOUT_GROUP1_ZDZ) &&
        (current_ekran.current_level <= EKRAN_CHOOSE_TIMEOUT_GROUP4_ZDZ) 
       )  
       ||
       (
        (current_ekran.current_level >= EKRAN_TIMEOUT_ZDZ_GROUP1) &&
        (current_ekran.current_level <= EKRAN_TIMEOUT_ZDZ_GROUP4)
       )
       ||  
       (current_ekran.current_level == EKRAN_CONTROL_ZDZ        )
      )
      error_window |= (1 << ZDZ_BIT_CONFIGURATION );
  }
  //Перевірка ЗЗ
  if ((new_configuration & (1<<ZZ_BIT_CONFIGURATION)) == 0)
  {
    if(
       (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_ZZ)
       || 
       (
        (current_ekran.current_level >= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_ZZ) &&
        (current_ekran.current_level <= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP4_ZZ) 
       )  
       ||
       (
        (current_ekran.current_level >= EKRAN_SETPOINT_ZZ_GROUP1) &&
        (current_ekran.current_level <= EKRAN_SETPOINT_ZZ_GROUP4)
       )
       ||  
       (
        (current_ekran.current_level >= EKRAN_TIMEOUT_ZZ_GROUP1) &&
        (current_ekran.current_level <= EKRAN_TIMEOUT_ZZ_GROUP4)
       )
       ||  
       (current_ekran.current_level == EKRAN_CONTROL_ZZ        )
      )
      error_window |= (1 << ZZ_BIT_CONFIGURATION );
  }
  //Перевірка ТЗНП
  if ((new_configuration & (1<<TZNP_BIT_CONFIGURATION)) == 0)
  {
    if(
       (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_TZNP)
       || 
       (
        (current_ekran.current_level >= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_TZNP) &&
        (current_ekran.current_level <= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP4_TZNP) 
       )  
       ||
       (
        (current_ekran.current_level >= EKRAN_SETPOINT_TZNP_GROUP1) &&
        (current_ekran.current_level <= EKRAN_SETPOINT_TZNP_GROUP4)
       )
       ||  
       (
        (current_ekran.current_level >= EKRAN_TIMEOUT_TZNP_GROUP1) &&
        (current_ekran.current_level <= EKRAN_TIMEOUT_TZNP_GROUP4)
       )
       ||  
       (current_ekran.current_level == EKRAN_CONTROL_TZNP        )
      )
      error_window |= (1 << TZNP_BIT_CONFIGURATION );
  }
  //Перевірка АПВ
  if ((new_configuration & (1<<APV_BIT_CONFIGURATION)) == 0)
  {
    if(
       (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_APV)
       || 
       (
        (current_ekran.current_level >= EKRAN_CHOOSE_TIMEOUT_GROUP1_APV) &&
        (current_ekran.current_level <= EKRAN_CHOOSE_TIMEOUT_GROUP4_APV) 
       )  
       ||  
       (
        (current_ekran.current_level >= EKRAN_TIMEOUT_APV_GROUP1) &&
        (current_ekran.current_level <= EKRAN_TIMEOUT_APV_GROUP4)
       )
       ||  
       (current_ekran.current_level == EKRAN_CONTROL_APV        )
      )
      error_window |= (1 << APV_BIT_CONFIGURATION );
  }
  //Перевірка АЧР-ЧАПВ
  if ((new_configuration & (1<<ACHR_CHAPV_BIT_CONFIGURATION)) == 0)
  {
    if(
       (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_ACHR_CHAPV)
       || 
       (
        (current_ekran.current_level >= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_ACHR_CHAPV) &&
        (current_ekran.current_level <= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP4_ACHR_CHAPV) 
       )  
       ||
       (
        (current_ekran.current_level >= EKRAN_SETPOINT_ACHR_CHAPV_GROUP1) &&
        (current_ekran.current_level <= EKRAN_SETPOINT_ACHR_CHAPV_GROUP4)
       )
       ||  
       (
        (current_ekran.current_level >= EKRAN_TIMEOUT_ACHR_CHAPV_GROUP1) &&
        (current_ekran.current_level <= EKRAN_TIMEOUT_ACHR_CHAPV_GROUP4)
       )
       ||  
       (current_ekran.current_level == EKRAN_CONTROL_ACHR_CHAPV        )
      )
      error_window |= (1 << ACHR_CHAPV_BIT_CONFIGURATION );
  }
  //Перевірка УРОВ
  if ((new_configuration & (1<<UROV_BIT_CONFIGURATION)) == 0)
  {
    if(
       (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_UROV)
       || 
       (
        (current_ekran.current_level >= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_UROV) &&
        (current_ekran.current_level <= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP4_UROV) 
       )  
       ||
       (
        (current_ekran.current_level >= EKRAN_SETPOINT_UROV_GROUP1) &&
        (current_ekran.current_level <= EKRAN_SETPOINT_UROV_GROUP4)
       )
       ||  
       (
        (current_ekran.current_level >= EKRAN_TIMEOUT_UROV_GROUP1) &&
        (current_ekran.current_level <= EKRAN_TIMEOUT_UROV_GROUP4)
       )
       ||  
       (current_ekran.current_level == EKRAN_CONTROL_UROV        )
      )
      error_window |= (1 << UROV_BIT_CONFIGURATION );
  }
  //Перевірка ЗОП(КОФ)
  if ((new_configuration & (1<<ZOP_BIT_CONFIGURATION)) == 0)
  {
    if(
       (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_ZOP)
       || 
       (
        (current_ekran.current_level >= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_ZOP) &&
        (current_ekran.current_level <= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP4_ZOP) 
       )  
       ||
       (
        (current_ekran.current_level >= EKRAN_SETPOINT_ZOP_GROUP1) &&
        (current_ekran.current_level <= EKRAN_SETPOINT_ZOP_GROUP4)
       )
       ||  
       (
        (current_ekran.current_level >= EKRAN_TIMEOUT_ZOP_GROUP1) &&
        (current_ekran.current_level <= EKRAN_TIMEOUT_ZOP_GROUP4)
       )
       ||  
       (current_ekran.current_level == EKRAN_CONTROL_ZOP        )
      )
      error_window |= (1 << ZOP_BIT_CONFIGURATION );
  }
  //Перевірка Umin
  if ((new_configuration & (1<<UMIN_BIT_CONFIGURATION)) == 0)
  {
    if(
       (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_UMIN)
       || 
       (
        (current_ekran.current_level >= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_UMIN) &&
        (current_ekran.current_level <= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP4_UMIN) 
       )  
       ||
       (
        (current_ekran.current_level >= EKRAN_SETPOINT_UMIN_GROUP1) &&
        (current_ekran.current_level <= EKRAN_SETPOINT_UMIN_GROUP4)
       )
       ||  
       (
        (current_ekran.current_level >= EKRAN_TIMEOUT_UMIN_GROUP1) &&
        (current_ekran.current_level <= EKRAN_TIMEOUT_UMIN_GROUP4)
       )
       ||  
       (current_ekran.current_level == EKRAN_CONTROL_UMIN        )
      )
      error_window |= (1 << UMIN_BIT_CONFIGURATION );
  }
  //Перевірка Umax
  if ((new_configuration & (1<<UMAX_BIT_CONFIGURATION)) == 0)
  {
    if(
       (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_UMAX)
       || 
       (
        (current_ekran.current_level >= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_UMAX) &&
        (current_ekran.current_level <= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP4_UMAX) 
       )  
       ||
       (
        (current_ekran.current_level >= EKRAN_SETPOINT_UMAX_GROUP1) &&
        (current_ekran.current_level <= EKRAN_SETPOINT_UMAX_GROUP4)
       )
       ||  
       (
        (current_ekran.current_level >= EKRAN_TIMEOUT_UMAX_GROUP1) &&
        (current_ekran.current_level <= EKRAN_TIMEOUT_UMAX_GROUP4)
       )
       ||  
       (current_ekran.current_level == EKRAN_CONTROL_UMAX        )
      )
      error_window |= (1 << UMAX_BIT_CONFIGURATION );
  }
  //Перевірка Універсальний Пристрій
  if ((new_configuration & (1<<UP_BIT_CONFIGURATION)) == 0)
  {
    if(
       (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_UP)
       || 
       (
        (current_ekran.current_level >= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_UP) &&
        (current_ekran.current_level <= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP4_UP) 
       )  
       ||
       (
        (current_ekran.current_level >= EKRAN_SETPOINT_UP_GROUP1) &&
        (current_ekran.current_level <= EKRAN_SETPOINT_UP_GROUP4)
       )
       ||  
       (
        (current_ekran.current_level >= EKRAN_TIMEOUT_UP_GROUP1) &&
        (current_ekran.current_level <= EKRAN_TIMEOUT_UP_GROUP4)
       )
       ||  
       (current_ekran.current_level == EKRAN_CONTROL_UP        )
      )
      error_window |= (1 << UP_BIT_CONFIGURATION );
  }
  //Перевірка "Визначення місця пошкодження"
  if ((new_configuration & (1<<VMP_BIT_CONFIGURATION)) == 0)
  {
    if(
       (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_VMP)
       || 
       (
        (current_ekran.current_level >= EKRAN_SETPOINT_VMP_FORWARD) &&
        (current_ekran.current_level <= EKRAN_SETPOINT_VMP_BACKWARD) 
       )  
       ||  
       (current_ekran.current_level == EKRAN_CONTROL_VMP              )
      )
      error_window |= (1 << VMP_BIT_CONFIGURATION );
  }
  //Перевірка "Розширеної логіки"
  if ((new_configuration & (1<<EL_BIT_CONFIGURATION)) == 0)
  {
    if(
       (current_ekran.current_level >= EKRAN_EXTENDED_LIGIC) &&
       (current_ekran.current_level <= EKRAN_RANGUVANNJA_D_NOT16)  
      )
      error_window |= (1 << EL_BIT_CONFIGURATION );
  }
  /************************/
  
  if (error_window == 0)
  {
    //Вводимо нову конфігурацю у цільову структуру
    target_label->configuration = new_configuration;
    
    unsigned int maska[N_SMALL]; 
    for(size_t i = 0; i < N_SMALL; ++i) maska[i] = 0;
    unsigned int maska_1[N_BIG];
    for(size_t i = 0; i < N_BIG; ++i) maska_1[i] = 0;
  
    //Перевіряємо, чи МТЗ зараз знято з конфігурації
    if ((target_label->configuration & (1<<MTZ_BIT_CONFIGURATION)) == 0)
    {
      //Виводим ступені МТЗ
      target_label->control_mtz &= (unsigned int)(~(
																										MASKA_FOR_BIT(N_BIT_CTRMTZ_1) | 
																										MASKA_FOR_BIT(N_BIT_CTRMTZ_2) | 
																										MASKA_FOR_BIT(N_BIT_CTRMTZ_3) | 
																										MASKA_FOR_BIT(N_BIT_CTRMTZ_4)
																									 )
																								 );
   
      //Виводим ступені МТЗ з АПВ
      target_label->control_apv &= (unsigned int)(~(CTR_APV_STARTED_FROM_MTZ1 | CTR_APV_STARTED_FROM_MTZ2 | CTR_APV_STARTED_FROM_MTZ3  | CTR_APV_STARTED_FROM_MTZ4));

      //Вимикаємо контроль ЗДЗ за струмом
      int32_t ctrl_zdz_type_tmp = target_label->ctrl_zdz_type;
      if (
          (ctrl_zdz_type_tmp == ZDZ_CTRL_I      ) ||
          (ctrl_zdz_type_tmp == ZDZ_CTRL_I_OR_U ) ||
          (ctrl_zdz_type_tmp == ZDZ_CTRL_I_AND_U)
         )
        target_label->ctrl_zdz_type = ZDZ_CTRL_NONE;
      //Виводим ступені МТЗ з ЗДЗ
      target_label->control_zdz &= (unsigned int)(~(
                                                    MASKA_FOR_BIT(CTR_ZDZ_STARTED_FROM_MTZ1_BIT - (_CTR_ZDZ_PART_III - _CTR_ZDZ_PART_II)) | 
                                                    MASKA_FOR_BIT(CTR_ZDZ_STARTED_FROM_MTZ2_BIT - (_CTR_ZDZ_PART_III - _CTR_ZDZ_PART_II)) | 
                                                    MASKA_FOR_BIT(CTR_ZDZ_STARTED_FROM_MTZ3_BIT - (_CTR_ZDZ_PART_III - _CTR_ZDZ_PART_II)) | 
                                                    MASKA_FOR_BIT(CTR_ZDZ_STARTED_FROM_MTZ4_BIT - (_CTR_ZDZ_PART_III - _CTR_ZDZ_PART_II))
                                                   ));
      
      //Виводим ступені МТЗ з УРОВ
      target_label->control_urov &= (unsigned int)(~(
                                                     MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_MTZ1) | 
                                                     MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_MTZ2) | 
                                                     MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_MTZ3) | 
                                                     MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_MTZ4)
                                                    )
                                                  );
      
      //Формуємо маски функцій МТЗ
      for (unsigned int i = 0; i < N_SMALL; i++ ) maska[i] = 0;
      for (int i = 0; i < NUMBER_MTZ_SIGNAL_FOR_RANG_SMALL; i++)
        _SET_BIT(
                 maska, 
                 (
                  NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL + 
                  i
                 )
                );

      for (unsigned int i = 0; i < N_BIG; i++ ) maska_1[i] = 0;
      for (int i = 0; i < NUMBER_MTZ_SIGNAL_FOR_RANG; i++)
        _SET_BIT(
                 maska_1, 
                 (
                  NUMBER_GENERAL_SIGNAL_FOR_RANG + 
                  i
                 )
                );
      
      //Знімаємо всі функції для ранжування входів, які відповідають за МТЗ
      for (int i = 0; i < NUMBER_DEFINED_BUTTONS; i++)
      {
        for(size_t m = 0; m < N_SMALL; ++m) target_label->ranguvannja_buttons[N_SMALL*i + m] &= ~maska[m];
      }

      //Знімаємо всі функції для ранжування входів, які відповідають за МТЗ
      for (int i = 0; i < NUMBER_INPUTS; i++)
      {
        for(size_t m = 0; m < N_SMALL; ++m) target_label->ranguvannja_inputs[N_SMALL*i+m] &= ~maska[m];
      }

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
      //Знімаємо всі функції для ранжування виходів Вх.GOOSE блоку
      for (size_t i = 0; i < N_IN_GOOSE; i++)
      {
        for (size_t j = 0; j < N_IN_GOOSE_MMS_OUT; j++)
        {
          for(size_t m = 0; m < N_SMALL; ++m) target_label->ranguvannja_In_GOOSE[i][j][N_SMALL*i+m] &= ~maska[m];  
        }
      }
      
      //Знімаємо всі функції для ранжування виходів Вх.MMS блоку
      for (size_t i = 0; i < N_IN_MMS; i++)
      {
        for (size_t j = 0; j < N_IN_GOOSE_MMS_OUT; j++)
        {
          for(size_t m = 0; m < N_SMALL; ++m) target_label->ranguvannja_In_MMS[i][j][N_SMALL*i+m] &= ~maska[m];
        }
      }
      
      //Знімаємо всі функції для ранжування входів Вихідного Мережевого Блоку
      for (size_t i = 0; i < N_OUT_LAN; i++)
      {
        for (size_t j = 0; j < N_OUT_LAN_IN; j++)
        {
          intptr_t additional_current = 0;
          for (intptr_t k = 0; k < MAX_FUNCTIONS_IN_OUT_LAN; k++)
          {
            unsigned int  value = target_label->ranguvannja_Out_LAN[i][j][k - additional_current];
            
            if (value == 0) break;
            else if ((value >= (RANG_BLOCK_MTZ1 + 1)) && (value <= (RANG_NCN_MTZ + 1)))
            {
              for (intptr_t l = (k - additional_current); l < (MAX_FUNCTIONS_IN_OUT_LAN - additional_current); l++)
              {
                if ((l + 1) < (MAX_FUNCTIONS_IN_OUT_LAN - additional_current)) 
                {
                  target_label->ranguvannja_Out_LAN[i][j][l] = target_label->ranguvannja_Out_LAN[i][j][l + 1];
                }
                else target_label->ranguvannja_Out_LAN[i][j][l] = 0;
              }
              additional_current++;
            }
          }
        }
      }
#endif
      
      //Знімаємо всі функції для ранжування виходів
      for (int i = 0; i < NUMBER_OUTPUTS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_outputs[N_BIG*i+j] &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування світоіндикаторів
      for (int i = 0; i < NUMBER_LEDS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_leds[N_BIG*i+j] &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування аналогового і дискретного реєстраторів
      for (unsigned int j = 0; j < N_BIG; j++ ) 
      {
        target_label->ranguvannja_analog_registrator[j]  &= ~maska_1[j];
        target_label->ranguvannja_digital_registrator[j] &= ~maska_1[j];
        target_label->ranguvannja_off_cb[j] &= ~maska_1[j];
        target_label->ranguvannja_on_cb[j]  &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування оприділювальних функцій
      for (int i = 0; i < NUMBER_DEFINED_FUNCTIONS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) 
        {
          target_label->ranguvannja_df_source_plus[N_BIG*i+j]  &= ~maska_1[j];
          target_label->ranguvannja_df_source_minus[N_BIG*i+j] &= ~maska_1[j];
          target_label->ranguvannja_df_source_blk[N_BIG*i+j]   &= ~maska_1[j];
          target_label->ranguvannja_df_source_reset[N_BIG*i+j] &= ~maska_1[j];
        }
      }
      //Знімаємо всі функції для ранжування оприділювальних триґерів
      for (int i = 0; i < NUMBER_DEFINED_TRIGGERS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) 
        {
          target_label->ranguvannja_set_dt_source_plus[N_BIG*i+j]    &= ~maska_1[j];
          target_label->ranguvannja_set_dt_source_minus[N_BIG*i+j]   &= ~maska_1[j];
          target_label->ranguvannja_reset_dt_source_plus[N_BIG*i+j]  &= ~maska_1[j];
          target_label->ranguvannja_reset_dt_source_minus[N_BIG*i+j] &= ~maska_1[j];
        }
      }
  
      //Знімаємо всі функції для ранжування визначуваних "І"
      for(unsigned int i = 0; i < NUMBER_DEFINED_AND; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_and[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "АБО"
      for(unsigned int i = 0; i < NUMBER_DEFINED_OR; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_or[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "Викл.АБО"
      for(unsigned int i = 0; i < NUMBER_DEFINED_XOR; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_xor[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "НЕ"
      for(unsigned int i = 0; i < NUMBER_DEFINED_NOT; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_not[N_BIG*i+j] &= ~maska_1[j];
      }
    }

    //Перевіряємо, чи МТЗ 0.4кВ зараз знято з конфігурації
    if ((target_label->configuration & (1<<MTZ04_BIT_CONFIGURATION)) == 0)
    {
      //Виводим ступені МТЗ 0.4кВ
      target_label->control_mtz04 &= (unsigned int)(~(CTR_MTZ04_1 | CTR_MTZ04_2));
   
      //Виводим ступені МТЗ 0.4кВ з УРОВ
      target_label->control_urov &= (unsigned int)(
                                                   ~(
                                                     MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_MTZ04_1) | 
                                                     MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_MTZ04_2)
                                                    )
                                                  );
      
      //Формуємо маки функцій МТЗ 0.4кВ
      for (unsigned int i = 0; i < N_SMALL; i++ ) maska[i] = 0;
      for (int i = 0; i < NUMBER_MTZ04_SIGNAL_FOR_RANG_SMALL; i++)
        _SET_BIT(
                 maska, 
                 (
                  NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL    + 
                  NUMBER_MTZ_SIGNAL_FOR_RANG_SMALL        +
                  i
                 )
                );

      for (unsigned int i = 0; i < N_BIG; i++ ) maska_1[i] = 0;
      for (int i = 0; i < NUMBER_MTZ04_SIGNAL_FOR_RANG; i++)
        _SET_BIT(
                 maska_1, 
                 (
                  NUMBER_GENERAL_SIGNAL_FOR_RANG    + 
                  NUMBER_MTZ_SIGNAL_FOR_RANG        +
                  i
                 )
                );

      //Знімаємо всі функції для ранжування кнопок, які відповідають за МТЗ 0.4кВ
      for (int i = 0; i < NUMBER_DEFINED_BUTTONS; i++)
      {
        for(size_t m = 0; m < N_SMALL; ++m) target_label->ranguvannja_buttons[N_SMALL*i + m] &= ~maska[m];
      }

      //Знімаємо всі функції для ранжування входів, які відповідають за МТЗ 0.4кВ
      for (int i = 0; i < NUMBER_INPUTS; i++)
      {
        for(size_t m = 0; m < N_SMALL; ++m) target_label->ranguvannja_inputs[N_SMALL*i+m] &= ~maska[m];
      }

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
      //Знімаємо всі функції для ранжування виходів Вх.GOOSE блоку
      for (size_t i = 0; i < N_IN_GOOSE; i++)
      {
        for (size_t j = 0; j < N_IN_GOOSE_MMS_OUT; j++)
        {
          for(size_t m = 0; m < N_SMALL; ++m) target_label->ranguvannja_In_GOOSE[i][j][N_SMALL*i+m] &= ~maska[m];
        }
      }
      
      //Знімаємо всі функції для ранжування виходів Вх.MMS блоку
      for (size_t i = 0; i < N_IN_MMS; i++)
      {
        for (size_t j = 0; j < N_IN_GOOSE_MMS_OUT; j++)
        {
          for(size_t m = 0; m < N_SMALL; ++m) target_label->ranguvannja_In_MMS[i][j][N_SMALL*i+m] &= ~maska[m];
        }
      }
      
      //Знімаємо всі функції для ранжування входів Вихідного Мережевого Блоку
      for (size_t i = 0; i < N_OUT_LAN; i++)
      {
        for (size_t j = 0; j < N_OUT_LAN_IN; j++)
        {
          intptr_t additional_current = 0;
          for (intptr_t k = 0; k < MAX_FUNCTIONS_IN_OUT_LAN; k++)
          {
            unsigned int  value = target_label->ranguvannja_Out_LAN[i][j][k - additional_current];
            
            if (value == 0) break;
            else if ((value >= (RANG_BLOCK_MTZ04_1 + 1)) && (value <= (RANG_MTZ04_2 + 1)))
            {
              for (intptr_t l = (k - additional_current); l < (MAX_FUNCTIONS_IN_OUT_LAN - additional_current); l++)
              {
                if ((l + 1) < (MAX_FUNCTIONS_IN_OUT_LAN - additional_current)) 
                {
                  target_label->ranguvannja_Out_LAN[i][j][l] = target_label->ranguvannja_Out_LAN[i][j][l + 1];
                }
                else target_label->ranguvannja_Out_LAN[i][j][l] = 0;
              }
              additional_current++;
            }
          }
        }
      }
#endif

      //Знімаємо всі функції для ранжування виходів
      for (int i = 0; i < NUMBER_OUTPUTS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_outputs[N_BIG*i+j] &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування світоіндикаторів
      for (int i = 0; i < NUMBER_LEDS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_leds[N_BIG*i+j] &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування аналогового і дискретного реєстраторів
      for (unsigned int j = 0; j < N_BIG; j++ ) 
      {
        target_label->ranguvannja_analog_registrator[j]  &= ~maska_1[j];
        target_label->ranguvannja_digital_registrator[j] &= ~maska_1[j];
        target_label->ranguvannja_off_cb[j] &= ~maska_1[j];
        target_label->ranguvannja_on_cb[j]  &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування оприділювальних функцій
      for (int i = 0; i < NUMBER_DEFINED_FUNCTIONS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) 
        {
          target_label->ranguvannja_df_source_plus[N_BIG*i+j]  &= ~maska_1[j];
          target_label->ranguvannja_df_source_minus[N_BIG*i+j] &= ~maska_1[j];
          target_label->ranguvannja_df_source_blk[N_BIG*i+j]   &= ~maska_1[j];
          target_label->ranguvannja_df_source_reset[N_BIG*i+j] &= ~maska_1[j];
        }
      }
      //Знімаємо всі функції для ранжування оприділювальних триґерів
      for (int i = 0; i < NUMBER_DEFINED_TRIGGERS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) 
        {
          target_label->ranguvannja_set_dt_source_plus[N_BIG*i+j]    &= ~maska_1[j];
          target_label->ranguvannja_set_dt_source_minus[N_BIG*i+j]   &= ~maska_1[j];
          target_label->ranguvannja_reset_dt_source_plus[N_BIG*i+j]  &= ~maska_1[j];
          target_label->ranguvannja_reset_dt_source_minus[N_BIG*i+j] &= ~maska_1[j];
        }
      }
  
      //Знімаємо всі функції для ранжування визначуваних "І"
      for(unsigned int i = 0; i < NUMBER_DEFINED_AND; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_and[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "АБО"
      for(unsigned int i = 0; i < NUMBER_DEFINED_OR; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_or[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "Викл.АБО"
      for(unsigned int i = 0; i < NUMBER_DEFINED_XOR; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_xor[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "НЕ"
      for(unsigned int i = 0; i < NUMBER_DEFINED_NOT; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_not[N_BIG*i+j] &= ~maska_1[j];
      }
    }

    //Перевіряємо, чи ЗНам зараз знято з конфігурації
    if ((target_label->configuration & (1<<ZNAM_BIT_CONFIGURATION)) == 0)
    {
      //Виводим ступені ЗНам
      target_label->control_znam &= (~MASKA_FOR_BIT(CTR_ZNAM_STATE_BIT));
   
      //Виводим вплив на МСЗ
      target_label->control_mtz &= (~(
                                      MASKA_FOR_BIT(N_BIT_CTRMTZ_1_ZNAM) |
                                      MASKA_FOR_BIT(N_BIT_CTRMTZ_2_ZNAM) |
                                      MASKA_FOR_BIT(N_BIT_CTRMTZ_3_ZNAM) |
                                      MASKA_FOR_BIT(N_BIT_CTRMTZ_4_ZNAM)
																		)
                                   );
      //Формуємо маки функцій ЗНам
      for (unsigned int i = 0; i < N_SMALL; i++ ) maska[i] = 0;
      for (int i = 0; i < NUMBER_ZNAM_SIGNAL_FOR_RANG_SMALL; i++)
        _SET_BIT(
                 maska, 
                 (
                  NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL    + 
                  NUMBER_MTZ_SIGNAL_FOR_RANG_SMALL        +
                  NUMBER_MTZ04_SIGNAL_FOR_RANG_SMALL      +
                  i
                 )
                );
     
      for (unsigned int i = 0; i < N_BIG; i++ ) maska_1[i] = 0;
      for (int i = 0; i < NUMBER_ZNAM_SIGNAL_FOR_RANG; i++)
        _SET_BIT(
                 maska_1, 
                 (
                  NUMBER_GENERAL_SIGNAL_FOR_RANG    + 
                  NUMBER_MTZ_SIGNAL_FOR_RANG        +
                  NUMBER_MTZ04_SIGNAL_FOR_RANG      +
                  i
                 )
                );

      //Знімаємо всі функції для ранжування кнопок, які відповідають за ЗНам
      for (int i = 0; i < NUMBER_DEFINED_BUTTONS; i++)
      {
        for(size_t m = 0; m < N_SMALL; ++m) target_label->ranguvannja_buttons[N_SMALL*i+m] &= ~maska[m];
      }

      //Знімаємо всі функції для ранжування входів, які відповідають за ЗНам
      for (int i = 0; i < NUMBER_INPUTS; i++)
      {
        for(size_t m = 0; m < N_SMALL; ++m) target_label->ranguvannja_inputs[N_SMALL*i+m] &= ~maska[m];
      }

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
      //Знімаємо всі функції для ранжування виходів Вх.GOOSE блоку
      for (size_t i = 0; i < N_IN_GOOSE; i++)
      {
        for (size_t j = 0; j < N_IN_GOOSE_MMS_OUT; j++)
        {
          for(size_t m = 0; m < N_SMALL; ++m) target_label->ranguvannja_In_GOOSE[i][j][N_SMALL*i+m] &= ~maska[m];
        }
      }
      
      //Знімаємо всі функції для ранжування виходів Вх.MMS блоку
      for (size_t i = 0; i < N_IN_MMS; i++)
      {
        for (size_t j = 0; j < N_IN_GOOSE_MMS_OUT; j++)
        {
          for(size_t m = 0; m < N_SMALL; ++m) target_label->ranguvannja_In_MMS[i][j][N_SMALL*i+m] &= ~maska[m];
        }
      }
      
      //Знімаємо всі функції для ранжування входів Вихідного Мережевого Блоку
      for (size_t i = 0; i < N_OUT_LAN; i++)
      {
        for (size_t j = 0; j < N_OUT_LAN_IN; j++)
        {
          intptr_t additional_current = 0;
          for (intptr_t k = 0; k < MAX_FUNCTIONS_IN_OUT_LAN; k++)
          {
            unsigned int  value = target_label->ranguvannja_Out_LAN[i][j][k - additional_current];
            
            if (value == 0) break;
            else if ((value >= (RANG_BLOCK_ZNAM + 1)) && (value <= (RANG_ZNAM + 1)))
            {
              for (intptr_t l = (k - additional_current); l < (MAX_FUNCTIONS_IN_OUT_LAN - additional_current); l++)
              {
                if ((l + 1) < (MAX_FUNCTIONS_IN_OUT_LAN - additional_current)) 
                {
                  target_label->ranguvannja_Out_LAN[i][j][l] = target_label->ranguvannja_Out_LAN[i][j][l + 1];
                }
                else target_label->ranguvannja_Out_LAN[i][j][l] = 0;
              }
              additional_current++;
            }
          }
        }
      }
#endif

      //Знімаємо всі функції для ранжування виходів
      for (int i = 0; i < NUMBER_OUTPUTS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_outputs[N_BIG*i+j] &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування світоіндикаторів
      for (int i = 0; i < NUMBER_LEDS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_leds[N_BIG*i+j] &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування аналогового і дискретного реєстраторів
      for (unsigned int j = 0; j < N_BIG; j++ ) 
      {
        target_label->ranguvannja_analog_registrator[j]  &= ~maska_1[j];
        target_label->ranguvannja_digital_registrator[j] &= ~maska_1[j];
        target_label->ranguvannja_off_cb[j] &= ~maska_1[j];
        target_label->ranguvannja_on_cb[j]  &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування оприділювальних функцій
      for (int i = 0; i < NUMBER_DEFINED_FUNCTIONS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) 
        {
          target_label->ranguvannja_df_source_plus[N_BIG*i+j]  &= ~maska_1[j];
          target_label->ranguvannja_df_source_minus[N_BIG*i+j] &= ~maska_1[j];
          target_label->ranguvannja_df_source_blk[N_BIG*i+j]   &= ~maska_1[j];
          target_label->ranguvannja_df_source_reset[N_BIG*i+j] &= ~maska_1[j];
        }
      }
      //Знімаємо всі функції для ранжування оприділювальних триґерів
      for (int i = 0; i < NUMBER_DEFINED_TRIGGERS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) 
        {
          target_label->ranguvannja_set_dt_source_plus[N_BIG*i+j]    &= ~maska_1[j];
          target_label->ranguvannja_set_dt_source_minus[N_BIG*i+j]   &= ~maska_1[j];
          target_label->ranguvannja_reset_dt_source_plus[N_BIG*i+j]  &= ~maska_1[j];
          target_label->ranguvannja_reset_dt_source_minus[N_BIG*i+j] &= ~maska_1[j];
        }
      }
  
      //Знімаємо всі функції для ранжування визначуваних "І"
      for(unsigned int i = 0; i < NUMBER_DEFINED_AND; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_and[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "АБО"
      for(unsigned int i = 0; i < NUMBER_DEFINED_OR; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_or[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "Викл.АБО"
      for(unsigned int i = 0; i < NUMBER_DEFINED_XOR; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_xor[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "НЕ"
      for(unsigned int i = 0; i < NUMBER_DEFINED_NOT; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_not[N_BIG*i+j] &= ~maska_1[j];
      }
    }

    //Перевіряємо, чи ЗДЗ зараз знято з конфігурації
    if ((target_label->configuration & (1<<ZDZ_BIT_CONFIGURATION)) == 0)
    {
      //Виводим ЗДЗ
      target_label->control_zdz &= (unsigned int)(~MASKA_FOR_BIT(CTR_ZDZ_STATE_BIT));

      //Виводим захист ЗДЗ з УРОВ
      target_label->control_urov &= (unsigned int)(~MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_ZDZ));
      
      //Формуємо маки функцій ЗДЗ
      for (unsigned int i = 0; i < N_SMALL; i++ ) maska[i] = 0;
      for (int i = 0; i < NUMBER_ZDZ_SIGNAL_FOR_RANG_SMALL; i++)
        _SET_BIT(
                 maska, 
                 (
                  NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL    + 
                  NUMBER_MTZ_SIGNAL_FOR_RANG_SMALL        +
                  NUMBER_MTZ04_SIGNAL_FOR_RANG_SMALL      +
                  NUMBER_ZNAM_SIGNAL_FOR_RANG_SMALL       +
                  i
                 )
                );
     
      for (unsigned int i = 0; i < N_BIG; i++ ) maska_1[i] = 0;
      for (int i = 0; i < NUMBER_ZDZ_SIGNAL_FOR_RANG; i++)
        _SET_BIT(
                 maska_1, 
                 (
                  NUMBER_GENERAL_SIGNAL_FOR_RANG    + 
                  NUMBER_MTZ_SIGNAL_FOR_RANG        +
                  NUMBER_MTZ04_SIGNAL_FOR_RANG      +
                  NUMBER_ZNAM_SIGNAL_FOR_RANG       +
                  i
                 )
                );

      //Знімаємо всі функції для ранжування входів, які відповідають за ЗДЗ
      for (int i = 0; i < NUMBER_DEFINED_BUTTONS; i++)
      {
        for(size_t m = 0; m < N_SMALL; ++m) target_label->ranguvannja_buttons[N_SMALL*i + m] &= ~maska[m];
      }

      //Знімаємо всі функції для ранжування входів, які відповідають за ЗДЗ
      for (int i = 0; i < NUMBER_INPUTS; i++)
      {
        for(size_t m = 0; m < N_SMALL; ++m) target_label->ranguvannja_inputs[N_SMALL*i+m] &= ~maska[m];
      }

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
      //Знімаємо всі функції для ранжування виходів Вх.GOOSE блоку
      for (size_t i = 0; i < N_IN_GOOSE; i++)
      {
        for (size_t j = 0; j < N_IN_GOOSE_MMS_OUT; j++)
        {
          for(size_t m = 0; m < N_SMALL; ++m) target_label->ranguvannja_In_GOOSE[i][j][N_SMALL*i+m] &= ~maska[m];
        }
      }
      
      //Знімаємо всі функції для ранжування виходів Вх.MMS блоку
      for (size_t i = 0; i < N_IN_MMS; i++)
      {
        for (size_t j = 0; j < N_IN_GOOSE_MMS_OUT; j++)
        {
          for(size_t m = 0; m < N_SMALL; ++m) target_label->ranguvannja_In_MMS[i][j][N_SMALL*i+m] &= ~maska[m];
        }
      }
      
      //Знімаємо всі функції для ранжування входів Вихідного Мережевого Блоку
      for (size_t i = 0; i < N_OUT_LAN; i++)
      {
        for (size_t j = 0; j < N_OUT_LAN_IN; j++)
        {
          intptr_t additional_current = 0;
          for (intptr_t k = 0; k < MAX_FUNCTIONS_IN_OUT_LAN; k++)
          {
            unsigned int  value = target_label->ranguvannja_Out_LAN[i][j][k - additional_current];
            
            if (value == 0) break;
            else if ((value >= (RANG_BLOCK_ZDZ + 1)) && (value <= (RANG_ZDZ + 1)))
            {
              for (intptr_t l = (k - additional_current); l < (MAX_FUNCTIONS_IN_OUT_LAN - additional_current); l++)
              {
                if ((l + 1) < (MAX_FUNCTIONS_IN_OUT_LAN - additional_current)) 
                {
                  target_label->ranguvannja_Out_LAN[i][j][l] = target_label->ranguvannja_Out_LAN[i][j][l + 1];
                }
                else target_label->ranguvannja_Out_LAN[i][j][l] = 0;
              }
              additional_current++;
            }
          }
        }
      }
#endif

      //Знімаємо всі функції для ранжування виходів, які відповідають за ЗДЗ
      //Знімаємо всі функції для ранжування виходів
      for (int i = 0; i < NUMBER_OUTPUTS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_outputs[N_BIG*i+j] &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування світоіндикаторів
      for (int i = 0; i < NUMBER_LEDS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_leds[N_BIG*i+j] &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування аналогового і дискретного реєстраторів
      for (unsigned int j = 0; j < N_BIG; j++ ) 
      {
        target_label->ranguvannja_analog_registrator[j]  &= ~maska_1[j];
        target_label->ranguvannja_digital_registrator[j] &= ~maska_1[j];
        target_label->ranguvannja_off_cb[j] &= ~maska_1[j];
        target_label->ranguvannja_on_cb[j]  &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування оприділювальних функцій
      for (int i = 0; i < NUMBER_DEFINED_FUNCTIONS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) 
        {
          target_label->ranguvannja_df_source_plus[N_BIG*i+j]  &= ~maska_1[j];
          target_label->ranguvannja_df_source_minus[N_BIG*i+j] &= ~maska_1[j];
          target_label->ranguvannja_df_source_blk[N_BIG*i+j]   &= ~maska_1[j];
          target_label->ranguvannja_df_source_reset[N_BIG*i+j] &= ~maska_1[j];
        }
      }
      //Знімаємо всі функції для ранжування оприділювальних триґерів
      for (int i = 0; i < NUMBER_DEFINED_TRIGGERS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) 
        {
          target_label->ranguvannja_set_dt_source_plus[N_BIG*i+j]    &= ~maska_1[j];
          target_label->ranguvannja_set_dt_source_minus[N_BIG*i+j]   &= ~maska_1[j];
          target_label->ranguvannja_reset_dt_source_plus[N_BIG*i+j]  &= ~maska_1[j];
          target_label->ranguvannja_reset_dt_source_minus[N_BIG*i+j] &= ~maska_1[j];
        }
      }
  
      //Знімаємо всі функції для ранжування визначуваних "І"
      for(unsigned int i = 0; i < NUMBER_DEFINED_AND; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_and[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "АБО"
      for(unsigned int i = 0; i < NUMBER_DEFINED_OR; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_or[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "Викл.АБО"
      for(unsigned int i = 0; i < NUMBER_DEFINED_XOR; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_xor[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "НЕ"
      for(unsigned int i = 0; i < NUMBER_DEFINED_NOT; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_not[N_BIG*i+j] &= ~maska_1[j];
      }
    }

    //Перевіряємо, чи ЗЗ зараз знято з конфігурації
    if ((target_label->configuration & (1<<ZZ_BIT_CONFIGURATION)) == 0)
    {
      //Виводим ЗЗ
      target_label->control_zz &= (unsigned int)(~(CTR_ZZ1_3I0_STATE | CTR_ZZ1_3U0_STATE | CTR_ZZ1_NZZ_STATE));
   
      //Вимикаємо контроль ЗДЗ за струмом 3I0
      int32_t ctrl_zdz_type_tmp = target_label->ctrl_zdz_type;
      if (
          (ctrl_zdz_type_tmp == ZDZ_CTRL_3I0)
         )
        target_label->ctrl_zdz_type = ZDZ_CTRL_NONE;

      //Виводим захисти 3I0, 3U0 і СЗЗ з УРОВ
      target_label->control_urov &= (unsigned int)(
                                                   ~(
                                                     MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_3I0) | 
                                                     MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_3U0) |
                                                     MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_NZZ)
                                                    )
                                                  );

      //Формуємо маки функцій ЗЗ
      for (unsigned int i = 0; i < N_SMALL; i++ ) maska[i] = 0;
      for (int i = 0; i < NUMBER_ZZ_SIGNAL_FOR_RANG_SMALL; i++)
        _SET_BIT(
                 maska, 
                 (
                  NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL + 
                  NUMBER_MTZ_SIGNAL_FOR_RANG_SMALL     +
                  NUMBER_MTZ04_SIGNAL_FOR_RANG_SMALL   +
                  NUMBER_ZNAM_SIGNAL_FOR_RANG_SMALL    +
                  NUMBER_ZDZ_SIGNAL_FOR_RANG_SMALL     +
                  i
                 )
                );

      for (unsigned int i = 0; i < N_BIG; i++ ) maska_1[i] = 0;
      for (int i = 0; i < NUMBER_ZZ_SIGNAL_FOR_RANG; i++)
        _SET_BIT(
                 maska_1, 
                 (
                  NUMBER_GENERAL_SIGNAL_FOR_RANG + 
                  NUMBER_MTZ_SIGNAL_FOR_RANG     +
                  NUMBER_MTZ04_SIGNAL_FOR_RANG   +
                  NUMBER_ZNAM_SIGNAL_FOR_RANG    +
                  NUMBER_ZDZ_SIGNAL_FOR_RANG     +
                  i
                 )
                );
      
      //Знімаємо всі функції для ранжування входів, які відповідають за ЗЗ
      for (int i = 0; i < NUMBER_DEFINED_BUTTONS; i++)
      {
        for(size_t m = 0; m < N_SMALL; ++m) target_label->ranguvannja_buttons[N_SMALL*i + m] &= ~maska[m];
      }

      //Знімаємо всі функції для ранжування входів, які відповідають за ЗЗ
      for (int i = 0; i < NUMBER_INPUTS; i++)
      {
        for(size_t m = 0; m < N_SMALL; ++m) target_label->ranguvannja_inputs[N_SMALL*i+m] &= ~maska[m];
      }

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
      //Знімаємо всі функції для ранжування виходів Вх.GOOSE блоку
      for (size_t i = 0; i < N_IN_GOOSE; i++)
      {
        for (size_t j = 0; j < N_IN_GOOSE_MMS_OUT; j++)
        {
          for(size_t m = 0; m < N_SMALL; ++m) target_label->ranguvannja_In_GOOSE[i][j][N_SMALL*i+m] &= ~maska[m];
        }
      }
      
      //Знімаємо всі функції для ранжування виходів Вх.MMS блоку
      for (size_t i = 0; i < N_IN_MMS; i++)
      {
        for (size_t j = 0; j < N_IN_GOOSE_MMS_OUT; j++)
        {
          for(size_t m = 0; m < N_SMALL; ++m) target_label->ranguvannja_In_MMS[i][j][N_SMALL*i+m] &= ~maska[m];
        }
      }
      
      //Знімаємо всі функції для ранжування входів Вихідного Мережевого Блоку
      for (size_t i = 0; i < N_OUT_LAN; i++)
      {
        for (size_t j = 0; j < N_OUT_LAN_IN; j++)
        {
          intptr_t additional_current = 0;
          for (intptr_t k = 0; k < MAX_FUNCTIONS_IN_OUT_LAN; k++)
          {
            unsigned int  value = target_label->ranguvannja_Out_LAN[i][j][k - additional_current];
            
            if (value == 0) break;
            else if ((value >= (RANG_BLOCK_NZZ + 1)) && (value <= (RANG_SECTOR_NZZ + 1)))
            {
              for (intptr_t l = (k - additional_current); l < (MAX_FUNCTIONS_IN_OUT_LAN - additional_current); l++)
              {
                if ((l + 1) < (MAX_FUNCTIONS_IN_OUT_LAN - additional_current)) 
                {
                  target_label->ranguvannja_Out_LAN[i][j][l] = target_label->ranguvannja_Out_LAN[i][j][l + 1];
                }
                else target_label->ranguvannja_Out_LAN[i][j][l] = 0;
              }
              additional_current++;
            }
          }
        }
      }
#endif

      //Знімаємо всі функції для ранжування виходів
      for (int i = 0; i < NUMBER_OUTPUTS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_outputs[N_BIG*i+j] &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування світоіндикаторів
      for (int i = 0; i < NUMBER_LEDS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_leds[N_BIG*i+j] &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування аналогового і дискретного реєстраторів
      for (unsigned int j = 0; j < N_BIG; j++ ) 
      {
        target_label->ranguvannja_analog_registrator[j]  &= ~maska_1[j];
        target_label->ranguvannja_digital_registrator[j] &= ~maska_1[j];
        target_label->ranguvannja_off_cb[j] &= ~maska_1[j];
        target_label->ranguvannja_on_cb[j]  &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування оприділювальних функцій
      for (int i = 0; i < NUMBER_DEFINED_FUNCTIONS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) 
        {
          target_label->ranguvannja_df_source_plus[N_BIG*i+j]  &= ~maska_1[j];
          target_label->ranguvannja_df_source_minus[N_BIG*i+j] &= ~maska_1[j];
          target_label->ranguvannja_df_source_blk[N_BIG*i+j]   &= ~maska_1[j];
          target_label->ranguvannja_df_source_reset[N_BIG*i+j] &= ~maska_1[j];
        }
      }
      //Знімаємо всі функції для ранжування оприділювальних триґерів
      for (int i = 0; i < NUMBER_DEFINED_TRIGGERS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) 
        {
          target_label->ranguvannja_set_dt_source_plus[N_BIG*i+j]    &= ~maska_1[j];
          target_label->ranguvannja_set_dt_source_minus[N_BIG*i+j]   &= ~maska_1[j];
          target_label->ranguvannja_reset_dt_source_plus[N_BIG*i+j]  &= ~maska_1[j];
          target_label->ranguvannja_reset_dt_source_minus[N_BIG*i+j] &= ~maska_1[j];
        }
      }
  
      //Знімаємо всі функції для ранжування визначуваних "І"
      for(unsigned int i = 0; i < NUMBER_DEFINED_AND; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_and[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "АБО"
      for(unsigned int i = 0; i < NUMBER_DEFINED_OR; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_or[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "Викл.АБО"
      for(unsigned int i = 0; i < NUMBER_DEFINED_XOR; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_xor[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "НЕ"
      for(unsigned int i = 0; i < NUMBER_DEFINED_NOT; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_not[N_BIG*i+j] &= ~maska_1[j];
      }
    }
  
    //Перевіряємо, чи ТЗНП зараз знято з конфігурації
    if ((target_label->configuration & (1<<TZNP_BIT_CONFIGURATION)) == 0)
    {
      //Виводим ступені ТЗНП
      target_label->control_tznp &= (unsigned int)(~(CTR_TZNP1 | CTR_TZNP2 | CTR_TZNP3));
   
      //Виводим ступені ТЗНП з УРОВ
      target_label->control_urov &= (unsigned int)(
                                                   ~(
                                                     MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_TZNP1) |
                                                     MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_TZNP2) |
                                                     MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_TZNP3)
                                                    )
                                                  );
      
      //Формуємо маки функцій ТЗНП
      for (unsigned int i = 0; i < N_SMALL; i++ ) maska[i] = 0;
      for (int i = 0; i < NUMBER_TZNP_SIGNAL_FOR_RANG_SMALL; i++)
        _SET_BIT(
                 maska, 
                 (
                  NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL + 
                  NUMBER_MTZ_SIGNAL_FOR_RANG_SMALL     +
                  NUMBER_MTZ04_SIGNAL_FOR_RANG_SMALL   +
                  NUMBER_ZNAM_SIGNAL_FOR_RANG_SMALL    +
                  NUMBER_ZDZ_SIGNAL_FOR_RANG_SMALL     +
                  NUMBER_ZZ_SIGNAL_FOR_RANG_SMALL      +
                  i
                 )
                );

      for (unsigned int i = 0; i < N_BIG; i++ ) maska_1[i] = 0;
      for (int i = 0; i < NUMBER_TZNP_SIGNAL_FOR_RANG; i++)
        _SET_BIT(
                 maska_1, 
                 (
                  NUMBER_GENERAL_SIGNAL_FOR_RANG + 
                  NUMBER_MTZ_SIGNAL_FOR_RANG     +
                  NUMBER_MTZ04_SIGNAL_FOR_RANG   +
                  NUMBER_ZNAM_SIGNAL_FOR_RANG    +
                  NUMBER_ZDZ_SIGNAL_FOR_RANG     +
                  NUMBER_ZZ_SIGNAL_FOR_RANG      +
                  i
                 )
                );
      
      //Знімаємо всі функції для ранжування входів, які відповідають за ТЗНП
      for (int i = 0; i < NUMBER_DEFINED_BUTTONS; i++)
      {
        for(size_t m = 0; m < N_SMALL; ++m) target_label->ranguvannja_buttons[N_SMALL*i + m] &= ~maska[m];
      }

      //Знімаємо всі функції для ранжування входів, які відповідають за ТЗНП
      for (int i = 0; i < NUMBER_INPUTS; i++)
      {
        for(size_t m = 0; m < N_SMALL; ++m) target_label->ranguvannja_inputs[N_SMALL*i+m] &= ~maska[m];
      }

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
      //Знімаємо всі функції для ранжування виходів Вх.GOOSE блоку
      for (size_t i = 0; i < N_IN_GOOSE; i++)
      {
        for (size_t j = 0; j < N_IN_GOOSE_MMS_OUT; j++)
        {
          for(size_t m = 0; m < N_SMALL; ++m) target_label->ranguvannja_In_GOOSE[i][j][N_SMALL*i+m] &= ~maska[m];
        }
      }
      
      //Знімаємо всі функції для ранжування виходів Вх.MMS блоку
      for (size_t i = 0; i < N_IN_MMS; i++)
      {
        for (size_t j = 0; j < N_IN_GOOSE_MMS_OUT; j++)
        {
          for(size_t m = 0; m < N_SMALL; ++m) target_label->ranguvannja_In_MMS[i][j][N_SMALL*i+m] &= ~maska[m];
        }
      }
      
      //Знімаємо всі функції для ранжування входів Вихідного Мережевого Блоку
      for (size_t i = 0; i < N_OUT_LAN; i++)
      {
        for (size_t j = 0; j < N_OUT_LAN_IN; j++)
        {
          intptr_t additional_current = 0;
          for (intptr_t k = 0; k < MAX_FUNCTIONS_IN_OUT_LAN; k++)
          {
            unsigned int  value = target_label->ranguvannja_Out_LAN[i][j][k - additional_current];
            
            if (value == 0) break;
            else if ((value >= (RANG_BLOCK_TZNP1 + 1)) && (value <= (RANG_TZNP3 + 1)))
            {
              for (intptr_t l = (k - additional_current); l < (MAX_FUNCTIONS_IN_OUT_LAN - additional_current); l++)
              {
                if ((l + 1) < (MAX_FUNCTIONS_IN_OUT_LAN - additional_current)) 
                {
                  target_label->ranguvannja_Out_LAN[i][j][l] = target_label->ranguvannja_Out_LAN[i][j][l + 1];
                }
                else target_label->ranguvannja_Out_LAN[i][j][l] = 0;
              }
              additional_current++;
            }
          }
        }
      }
#endif

      //Знімаємо всі функції для ранжування виходів
      for (int i = 0; i < NUMBER_OUTPUTS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_outputs[N_BIG*i+j] &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування світоіндикаторів
      for (int i = 0; i < NUMBER_LEDS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_leds[N_BIG*i+j] &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування аналогового і дискретного реєстраторів
      for (unsigned int j = 0; j < N_BIG; j++ ) 
      {
        target_label->ranguvannja_analog_registrator[j]  &= ~maska_1[j];
        target_label->ranguvannja_digital_registrator[j] &= ~maska_1[j];
        target_label->ranguvannja_off_cb[j] &= ~maska_1[j];
        target_label->ranguvannja_on_cb[j]  &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування оприділювальних функцій
      for (int i = 0; i < NUMBER_DEFINED_FUNCTIONS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) 
        {
          target_label->ranguvannja_df_source_plus[N_BIG*i+j]  &= ~maska_1[j];
          target_label->ranguvannja_df_source_minus[N_BIG*i+j] &= ~maska_1[j];
          target_label->ranguvannja_df_source_blk[N_BIG*i+j]   &= ~maska_1[j];
          target_label->ranguvannja_df_source_reset[N_BIG*i+j] &= ~maska_1[j];
        }
      }
      //Знімаємо всі функції для ранжування оприділювальних триґерів
      for (int i = 0; i < NUMBER_DEFINED_TRIGGERS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) 
        {
          target_label->ranguvannja_set_dt_source_plus[N_BIG*i+j]    &= ~maska_1[j];
          target_label->ranguvannja_set_dt_source_minus[N_BIG*i+j]   &= ~maska_1[j];
          target_label->ranguvannja_reset_dt_source_plus[N_BIG*i+j]  &= ~maska_1[j];
          target_label->ranguvannja_reset_dt_source_minus[N_BIG*i+j] &= ~maska_1[j];
        }
      }
  
      //Знімаємо всі функції для ранжування визначуваних "І"
      for(unsigned int i = 0; i < NUMBER_DEFINED_AND; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_and[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "АБО"
      for(unsigned int i = 0; i < NUMBER_DEFINED_OR; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_or[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "Викл.АБО"
      for(unsigned int i = 0; i < NUMBER_DEFINED_XOR; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_xor[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "НЕ"
      for(unsigned int i = 0; i < NUMBER_DEFINED_NOT; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_not[N_BIG*i+j] &= ~maska_1[j];
      }
    }

    //Перевіряємо, чи АПВ зараз знято з конфігурації
    if ((target_label->configuration & (1<<APV_BIT_CONFIGURATION)) == 0)
    {
      //Виводим АПВ
      target_label->control_apv &= (unsigned int)(~(CTR_APV_STAGE_1 | CTR_APV_STAGE_2 | CTR_APV_STAGE_3 | CTR_APV_STAGE_4));
   
      //Формуємо маки функцій АПВ
      for (unsigned int i = 0; i < N_SMALL; i++ ) maska[i] = 0;
      for (int i = 0; i < NUMBER_APV_SIGNAL_FOR_RANG_SMALL; i++)
        _SET_BIT(
                 maska, 
                 (
                  NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL + 
                  NUMBER_MTZ_SIGNAL_FOR_RANG_SMALL     +
                  NUMBER_MTZ04_SIGNAL_FOR_RANG_SMALL   +
                  NUMBER_ZNAM_SIGNAL_FOR_RANG_SMALL    +
                  NUMBER_ZDZ_SIGNAL_FOR_RANG_SMALL     +
                  NUMBER_ZZ_SIGNAL_FOR_RANG_SMALL      +
                  NUMBER_TZNP_SIGNAL_FOR_RANG_SMALL    +
                  i
                 )
                );

      for (unsigned int i = 0; i < N_BIG; i++ ) maska_1[i] = 0;
      for (int i = 0; i < NUMBER_APV_SIGNAL_FOR_RANG; i++)
        _SET_BIT(
                 maska_1, 
                 (
                  NUMBER_GENERAL_SIGNAL_FOR_RANG + 
                  NUMBER_MTZ_SIGNAL_FOR_RANG     +
                  NUMBER_MTZ04_SIGNAL_FOR_RANG   +
                  NUMBER_ZNAM_SIGNAL_FOR_RANG    +
                  NUMBER_ZDZ_SIGNAL_FOR_RANG     +
                  NUMBER_ZZ_SIGNAL_FOR_RANG      +
                  NUMBER_TZNP_SIGNAL_FOR_RANG    +
                  i
                 )
                );
      
      //Знімаємо всі функції для ранжування входів, які відповідають за АПВ
      for (int i = 0; i < NUMBER_DEFINED_BUTTONS; i++)
      {
        for(size_t m = 0; m < N_SMALL; ++m) target_label->ranguvannja_buttons[N_SMALL*i + m] &= ~maska[m];
      }

      //Знімаємо всі функції для ранжування входів, які відповідають за АПВ
      for (int i = 0; i < NUMBER_INPUTS; i++)
      {
        for(size_t m = 0; m < N_SMALL; ++m) target_label->ranguvannja_inputs[N_SMALL*i+m] &= ~maska[m];
      }

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
      //Знімаємо всі функції для ранжування виходів Вх.GOOSE блоку
      for (size_t i = 0; i < N_IN_GOOSE; i++)
      {
        for (size_t j = 0; j < N_IN_GOOSE_MMS_OUT; j++)
        {
          for(size_t m = 0; m < N_SMALL; ++m) target_label->ranguvannja_In_GOOSE[i][j][N_SMALL*i+m] &= ~maska[m];
        }
      }
      
      //Знімаємо всі функції для ранжування виходів Вх.MMS блоку
      for (size_t i = 0; i < N_IN_MMS; i++)
      {
        for (size_t j = 0; j < N_IN_GOOSE_MMS_OUT; j++)
        {
          for(size_t m = 0; m < N_SMALL; ++m) target_label->ranguvannja_In_MMS[i][j][N_SMALL*i+m] &= ~maska[m];
        }
      }
      
      //Знімаємо всі функції для ранжування входів Вихідного Мережевого Блоку
      for (size_t i = 0; i < N_OUT_LAN; i++)
      {
        for (size_t j = 0; j < N_OUT_LAN_IN; j++)
        {
          intptr_t additional_current = 0;
          for (intptr_t k = 0; k < MAX_FUNCTIONS_IN_OUT_LAN; k++)
          {
            unsigned int  value = target_label->ranguvannja_Out_LAN[i][j][k - additional_current];
            
            if (value == 0) break;
            else if ((value >= (RANG_STAT_BLK_APV + 1)) && (value <= (RANG_APV_WORK + 1)))
            {
              for (intptr_t l = (k - additional_current); l < (MAX_FUNCTIONS_IN_OUT_LAN - additional_current); l++)
              {
                if ((l + 1) < (MAX_FUNCTIONS_IN_OUT_LAN - additional_current)) 
                {
                  target_label->ranguvannja_Out_LAN[i][j][l] = target_label->ranguvannja_Out_LAN[i][j][l + 1];
                }
                else target_label->ranguvannja_Out_LAN[i][j][l] = 0;
              }
              additional_current++;
            }
          }
        }
      }
#endif

      //Знімаємо всі функції для ранжування виходів
      for (int i = 0; i < NUMBER_OUTPUTS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_outputs[N_BIG*i+j] &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування світоіндикаторів
      for (int i = 0; i < NUMBER_LEDS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_leds[N_BIG*i+j] &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування аналогового і дискретного реєстраторів
      for (unsigned int j = 0; j < N_BIG; j++ ) 
      {
        target_label->ranguvannja_analog_registrator[j]  &= ~maska_1[j];
        target_label->ranguvannja_digital_registrator[j] &= ~maska_1[j];
        target_label->ranguvannja_off_cb[j] &= ~maska_1[j];
        target_label->ranguvannja_on_cb[j]  &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування оприділювальних функцій
      for (int i = 0; i < NUMBER_DEFINED_FUNCTIONS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) 
        {
          target_label->ranguvannja_df_source_plus[N_BIG*i+j]  &= ~maska_1[j];
          target_label->ranguvannja_df_source_minus[N_BIG*i+j] &= ~maska_1[j];
          target_label->ranguvannja_df_source_blk[N_BIG*i+j]   &= ~maska_1[j];
          target_label->ranguvannja_df_source_reset[N_BIG*i+j] &= ~maska_1[j];
        }
      }
      //Знімаємо всі функції для ранжування оприділювальних триґерів
      for (int i = 0; i < NUMBER_DEFINED_TRIGGERS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) 
        {
          target_label->ranguvannja_set_dt_source_plus[N_BIG*i+j]    &= ~maska_1[j];
          target_label->ranguvannja_set_dt_source_minus[N_BIG*i+j]   &= ~maska_1[j];
          target_label->ranguvannja_reset_dt_source_plus[N_BIG*i+j]  &= ~maska_1[j];
          target_label->ranguvannja_reset_dt_source_minus[N_BIG*i+j] &= ~maska_1[j];
        }
      }
  
      //Знімаємо всі функції для ранжування визначуваних "І"
      for(unsigned int i = 0; i < NUMBER_DEFINED_AND; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_and[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "АБО"
      for(unsigned int i = 0; i < NUMBER_DEFINED_OR; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_or[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "Викл.АБО"
      for(unsigned int i = 0; i < NUMBER_DEFINED_XOR; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_xor[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "НЕ"
      for(unsigned int i = 0; i < NUMBER_DEFINED_NOT; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_not[N_BIG*i+j] &= ~maska_1[j];
      }
    }

    //Перевіряємо, чи АЧР-ЧАПВ зараз знято з конфігурації
    if ((target_label->configuration & (1<<ACHR_CHAPV_BIT_CONFIGURATION)) == 0)
    {
      //Виводим АЧР-ЧАПВ
      target_label->control_achr_chapv &= (unsigned int)(~(CTR_ACHR1 | CTR_CHAPV1 | CTR_ACHR2 | CTR_CHAPV2));

      //Виводим захисти АЧР з УРОВ
      target_label->control_urov &= (unsigned int)(
                                                   ~(
                                                     MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_ACHR1) |
                                                     MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_ACHR2)
                                                    )
                                                  );
      
      //Формуємо маки функцій АЧР-ЧАПВ
      for (unsigned int i = 0; i < N_SMALL; i++ ) maska[i] = 0;
      for (int i = 0; i < NUMBER_ACHR_CHAPV_SIGNAL_FOR_RANG_SMALL; i++)
        _SET_BIT(
                 maska, 
                 (
                  NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL + 
                  NUMBER_MTZ_SIGNAL_FOR_RANG_SMALL     +
                  NUMBER_MTZ04_SIGNAL_FOR_RANG_SMALL   +
                  NUMBER_ZNAM_SIGNAL_FOR_RANG_SMALL    +
                  NUMBER_ZDZ_SIGNAL_FOR_RANG_SMALL     +
                  NUMBER_ZZ_SIGNAL_FOR_RANG_SMALL      +
                  NUMBER_TZNP_SIGNAL_FOR_RANG_SMALL    +
                  NUMBER_APV_SIGNAL_FOR_RANG_SMALL     +
                  i
                 )
                );
     
      for (unsigned int i = 0; i < N_BIG; i++ ) maska_1[i] = 0;
      for (int i = 0; i < NUMBER_ACHR_CHAPV_SIGNAL_FOR_RANG; i++)
        _SET_BIT(
                 maska_1, 
                 (
                  NUMBER_GENERAL_SIGNAL_FOR_RANG + 
                  NUMBER_MTZ_SIGNAL_FOR_RANG     +
                  NUMBER_MTZ04_SIGNAL_FOR_RANG   +
                  NUMBER_ZNAM_SIGNAL_FOR_RANG    +
                  NUMBER_ZDZ_SIGNAL_FOR_RANG     +
                  NUMBER_ZZ_SIGNAL_FOR_RANG      +
                  NUMBER_TZNP_SIGNAL_FOR_RANG    +
                  NUMBER_APV_SIGNAL_FOR_RANG     +
                  i
                 )
                );

      //Знімаємо всі функції для ранжування входів, які відповідають за АЧР-ЧАПВ
      for (int i = 0; i < NUMBER_DEFINED_BUTTONS; i++)
      {
        for(size_t m = 0; m < N_SMALL; ++m) target_label->ranguvannja_buttons[N_SMALL*i + m] &= ~maska[m];
      }

      //Знімаємо всі функції для ранжування входів, які відповідають за АЧР-ЧАПВ
      for (int i = 0; i < NUMBER_INPUTS; i++)
      {
        for(size_t m = 0; m < N_SMALL; ++m) target_label->ranguvannja_inputs[N_SMALL*i+m] &= ~maska[m];
      }

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
      //Знімаємо всі функції для ранжування виходів Вх.GOOSE блоку
      for (size_t i = 0; i < N_IN_GOOSE; i++)
      {
        for (size_t j = 0; j < N_IN_GOOSE_MMS_OUT; j++)
        {
          for(size_t m = 0; m < N_SMALL; ++m) target_label->ranguvannja_In_GOOSE[i][j][N_SMALL*i+m] &= ~maska[m];
        }
      }
      
      //Знімаємо всі функції для ранжування виходів Вх.MMS блоку
      for (size_t i = 0; i < N_IN_MMS; i++)
      {
        for (size_t j = 0; j < N_IN_GOOSE_MMS_OUT; j++)
        {
          for(size_t m = 0; m < N_SMALL; ++m) target_label->ranguvannja_In_MMS[i][j][N_SMALL*i+m] &= ~maska[m];
        }
      }
      
      //Знімаємо всі функції для ранжування входів Вихідного Мережевого Блоку
      for (size_t i = 0; i < N_OUT_LAN; i++)
      {
        for (size_t j = 0; j < N_OUT_LAN_IN; j++)
        {
          intptr_t additional_current = 0;
          for (intptr_t k = 0; k < MAX_FUNCTIONS_IN_OUT_LAN; k++)
          {
            unsigned int  value = target_label->ranguvannja_Out_LAN[i][j][k - additional_current];
            
            if (value == 0) break;
            else if ((value >= (RANG_ACHR_CHAPV_VID_DV + 1)) && (value <= (RANG_ACHR_CHAPV2 + 1)))
            {
              for (intptr_t l = (k - additional_current); l < (MAX_FUNCTIONS_IN_OUT_LAN - additional_current); l++)
              {
                if ((l + 1) < (MAX_FUNCTIONS_IN_OUT_LAN - additional_current)) 
                {
                  target_label->ranguvannja_Out_LAN[i][j][l] = target_label->ranguvannja_Out_LAN[i][j][l + 1];
                }
                else target_label->ranguvannja_Out_LAN[i][j][l] = 0;
              }
              additional_current++;
            }
          }
        }
      }
#endif

      //Знімаємо всі функції для ранжування виходів
      for (int i = 0; i < NUMBER_OUTPUTS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_outputs[N_BIG*i+j] &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування світоіндикаторів
      for (int i = 0; i < NUMBER_LEDS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_leds[N_BIG*i+j] &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування аналогового і дискретного реєстраторів
      for (unsigned int j = 0; j < N_BIG; j++ ) 
      {
        target_label->ranguvannja_analog_registrator[j]  &= ~maska_1[j];
        target_label->ranguvannja_digital_registrator[j] &= ~maska_1[j];
        target_label->ranguvannja_off_cb[j] &= ~maska_1[j];
        target_label->ranguvannja_on_cb[j]  &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування оприділювальних функцій
      for (int i = 0; i < NUMBER_DEFINED_FUNCTIONS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) 
        {
          target_label->ranguvannja_df_source_plus[N_BIG*i+j]  &= ~maska_1[j];
          target_label->ranguvannja_df_source_minus[N_BIG*i+j] &= ~maska_1[j];
          target_label->ranguvannja_df_source_blk[N_BIG*i+j]   &= ~maska_1[j];
          target_label->ranguvannja_df_source_reset[N_BIG*i+j] &= ~maska_1[j];
        }
      }
      //Знімаємо всі функції для ранжування оприділювальних триґерів
      for (int i = 0; i < NUMBER_DEFINED_TRIGGERS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) 
        {
          target_label->ranguvannja_set_dt_source_plus[N_BIG*i+j]    &= ~maska_1[j];
          target_label->ranguvannja_set_dt_source_minus[N_BIG*i+j]   &= ~maska_1[j];
          target_label->ranguvannja_reset_dt_source_plus[N_BIG*i+j]  &= ~maska_1[j];
          target_label->ranguvannja_reset_dt_source_minus[N_BIG*i+j] &= ~maska_1[j];
        }
      }
  
      //Знімаємо всі функції для ранжування визначуваних "І"
      for(unsigned int i = 0; i < NUMBER_DEFINED_AND; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_and[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "АБО"
      for(unsigned int i = 0; i < NUMBER_DEFINED_OR; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_or[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "Викл.АБО"
      for(unsigned int i = 0; i < NUMBER_DEFINED_XOR; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_xor[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "НЕ"
      for(unsigned int i = 0; i < NUMBER_DEFINED_NOT; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_not[N_BIG*i+j] &= ~maska_1[j];
      }
    }

    //Перевіряємо, чи УРОВ зараз знято з конфігурації
    if ((target_label->configuration & (1<<UROV_BIT_CONFIGURATION)) == 0)
    {
      //Виводим УРОВ
      target_label->control_urov &= (unsigned int)(~MASKA_FOR_BIT(INDEX_ML_CTRUROV_STATE));
   
      //Виводим контроль ПРВВ2 з АПВ
      target_label->control_apv &= (unsigned int)(~MASKA_FOR_BIT(INDEX_ML_CTRAPV_BLK_CTRL_PRVV2));
   
        //Формуємо маки функцій УРОВ
      for (unsigned int i = 0; i < N_SMALL; i++ ) maska[i] = 0;
      for (int i = 0; i < NUMBER_UROV_SIGNAL_FOR_RANG_SMALL; i++)
        _SET_BIT(
                 maska, 
                 (
                  NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL    + 
                  NUMBER_MTZ_SIGNAL_FOR_RANG_SMALL        +
                  NUMBER_MTZ04_SIGNAL_FOR_RANG_SMALL      +
                  NUMBER_ZNAM_SIGNAL_FOR_RANG_SMALL       +
                  NUMBER_ZDZ_SIGNAL_FOR_RANG_SMALL        +
                  NUMBER_ZZ_SIGNAL_FOR_RANG_SMALL         +
                  NUMBER_TZNP_SIGNAL_FOR_RANG_SMALL       +
                  NUMBER_APV_SIGNAL_FOR_RANG_SMALL        +
                  NUMBER_ACHR_CHAPV_SIGNAL_FOR_RANG_SMALL +
                  i
                 )
                );
     
      for (unsigned int i = 0; i < N_BIG; i++ ) maska_1[i] = 0;
      for (int i = 0; i < NUMBER_UROV_SIGNAL_FOR_RANG; i++)
        _SET_BIT(
                 maska_1, 
                 (
                  NUMBER_GENERAL_SIGNAL_FOR_RANG    + 
                  NUMBER_MTZ_SIGNAL_FOR_RANG        +
                  NUMBER_MTZ04_SIGNAL_FOR_RANG      +
                  NUMBER_ZNAM_SIGNAL_FOR_RANG       +
                  NUMBER_ZDZ_SIGNAL_FOR_RANG        +
                  NUMBER_ZZ_SIGNAL_FOR_RANG         +
                  NUMBER_TZNP_SIGNAL_FOR_RANG       +
                  NUMBER_APV_SIGNAL_FOR_RANG        +
                  NUMBER_ACHR_CHAPV_SIGNAL_FOR_RANG +
                  i
                 )
                );

      //Знімаємо всі функції для ранжування входів, які відповідають за УРОВ
      for (int i = 0; i < NUMBER_DEFINED_BUTTONS; i++)
      {
        for(size_t m = 0; m < N_SMALL; ++m) target_label->ranguvannja_buttons[N_SMALL*i + m] &= ~maska[m];
      }

      //Знімаємо всі функції для ранжування входів, які відповідають за УРОВ
      for (int i = 0; i < NUMBER_INPUTS; i++)
      {
        for(size_t m = 0; m < N_SMALL; ++m) target_label->ranguvannja_inputs[N_SMALL*i+m] &= ~maska[m];
      }

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
      //Знімаємо всі функції для ранжування виходів Вх.GOOSE блоку
      for (size_t i = 0; i < N_IN_GOOSE; i++)
      {
        for (size_t j = 0; j < N_IN_GOOSE_MMS_OUT; j++)
        {
          for(size_t m = 0; m < N_SMALL; ++m) target_label->ranguvannja_In_GOOSE[i][j][N_SMALL*i+m] &= ~maska[m];
        }
      }
      
      //Знімаємо всі функції для ранжування виходів Вх.MMS блоку
      for (size_t i = 0; i < N_IN_MMS; i++)
      {
        for (size_t j = 0; j < N_IN_GOOSE_MMS_OUT; j++)
        {
          for(size_t m = 0; m < N_SMALL; ++m) target_label->ranguvannja_In_MMS[i][j][N_SMALL*i+m] &= ~maska[m];
        }
      }
      
      //Знімаємо всі функції для ранжування входів Вихідного Мережевого Блоку
      for (size_t i = 0; i < N_OUT_LAN; i++)
      {
        for (size_t j = 0; j < N_OUT_LAN_IN; j++)
        {
          intptr_t additional_current = 0;
          for (intptr_t k = 0; k < MAX_FUNCTIONS_IN_OUT_LAN; k++)
          {
            unsigned int  value = target_label->ranguvannja_Out_LAN[i][j][k - additional_current];
            
            if (value == 0) break;
            else if ((value >= (RANG_BLOCK_UROV + 1)) && (value <= (RANG_UROV2 + 1)))
            {
              for (intptr_t l = (k - additional_current); l < (MAX_FUNCTIONS_IN_OUT_LAN - additional_current); l++)
              {
                if ((l + 1) < (MAX_FUNCTIONS_IN_OUT_LAN - additional_current)) 
                {
                  target_label->ranguvannja_Out_LAN[i][j][l] = target_label->ranguvannja_Out_LAN[i][j][l + 1];
                }
                else target_label->ranguvannja_Out_LAN[i][j][l] = 0;
              }
              additional_current++;
            }
          }
        }
      }
#endif

      //Знімаємо всі функції для ранжування виходів
      for (int i = 0; i < NUMBER_OUTPUTS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_outputs[N_BIG*i+j] &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування світоіндикаторів
      for (int i = 0; i < NUMBER_LEDS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_leds[N_BIG*i+j] &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування аналогового і дискретного реєстраторів
      for (unsigned int j = 0; j < N_BIG; j++ ) 
      {
        target_label->ranguvannja_analog_registrator[j]  &= ~maska_1[j];
        target_label->ranguvannja_digital_registrator[j] &= ~maska_1[j];
        target_label->ranguvannja_off_cb[j] &= ~maska_1[j];
        target_label->ranguvannja_on_cb[j]  &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування оприділювальних функцій
      for (int i = 0; i < NUMBER_DEFINED_FUNCTIONS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) 
        {
          target_label->ranguvannja_df_source_plus[N_BIG*i+j]  &= ~maska_1[j];
          target_label->ranguvannja_df_source_minus[N_BIG*i+j] &= ~maska_1[j];
          target_label->ranguvannja_df_source_blk[N_BIG*i+j]   &= ~maska_1[j];
          target_label->ranguvannja_df_source_reset[N_BIG*i+j] &= ~maska_1[j];
        }
      }
      //Знімаємо всі функції для ранжування оприділювальних триґерів
      for (int i = 0; i < NUMBER_DEFINED_TRIGGERS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) 
        {
          target_label->ranguvannja_set_dt_source_plus[N_BIG*i+j]    &= ~maska_1[j];
          target_label->ranguvannja_set_dt_source_minus[N_BIG*i+j]   &= ~maska_1[j];
          target_label->ranguvannja_reset_dt_source_plus[N_BIG*i+j]  &= ~maska_1[j];
          target_label->ranguvannja_reset_dt_source_minus[N_BIG*i+j] &= ~maska_1[j];
        }
      }
  
      //Знімаємо всі функції для ранжування визначуваних "І"
      for(unsigned int i = 0; i < NUMBER_DEFINED_AND; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_and[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "АБО"
      for(unsigned int i = 0; i < NUMBER_DEFINED_OR; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_or[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "Викл.АБО"
      for(unsigned int i = 0; i < NUMBER_DEFINED_XOR; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_xor[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "НЕ"
      for(unsigned int i = 0; i < NUMBER_DEFINED_NOT; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_not[N_BIG*i+j] &= ~maska_1[j];
      }
    }

    //Перевіряємо, чи ЗОП(КОФ) зараз знято з конфігурації
    if ((target_label->configuration & (1<<ZOP_BIT_CONFIGURATION)) == 0)
    {
      //Виводим ЗОП(КОФ)
      target_label->control_zop &= (unsigned int)(~CTR_ZOP_STATE);
   
      //Виводим захисти ЗОП(КОФ) з УРОВ
      target_label->control_urov &= (unsigned int)(~MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_ZOP1));

      //Формуємо маки функцій ЗОП(КОФ)
      for (unsigned int i = 0; i < N_SMALL; i++ ) maska[i] = 0;
      for (int i = 0; i < NUMBER_ZOP_SIGNAL_FOR_RANG_SMALL; i++)
        _SET_BIT(
                 maska, 
                 (
                  NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL    + 
                  NUMBER_MTZ_SIGNAL_FOR_RANG_SMALL        +
                  NUMBER_MTZ04_SIGNAL_FOR_RANG_SMALL      +
                  NUMBER_ZNAM_SIGNAL_FOR_RANG_SMALL       +
                  NUMBER_ZDZ_SIGNAL_FOR_RANG_SMALL        +
                  NUMBER_ZZ_SIGNAL_FOR_RANG_SMALL         +
                  NUMBER_TZNP_SIGNAL_FOR_RANG_SMALL       +
                  NUMBER_APV_SIGNAL_FOR_RANG_SMALL        +
                  NUMBER_ACHR_CHAPV_SIGNAL_FOR_RANG_SMALL +
                  NUMBER_UROV_SIGNAL_FOR_RANG_SMALL       +
                  i
                 )
                );
     
      for (unsigned int i = 0; i < N_BIG; i++ ) maska_1[i] = 0;
      for (int i = 0; i < NUMBER_ZOP_SIGNAL_FOR_RANG; i++)
        _SET_BIT(
                 maska_1, 
                 (
                  NUMBER_GENERAL_SIGNAL_FOR_RANG    + 
                  NUMBER_MTZ_SIGNAL_FOR_RANG        +
                  NUMBER_MTZ04_SIGNAL_FOR_RANG      +
                  NUMBER_ZNAM_SIGNAL_FOR_RANG       +
                  NUMBER_ZDZ_SIGNAL_FOR_RANG        +
                  NUMBER_ZZ_SIGNAL_FOR_RANG         +
                  NUMBER_TZNP_SIGNAL_FOR_RANG       +
                  NUMBER_APV_SIGNAL_FOR_RANG        +
                  NUMBER_ACHR_CHAPV_SIGNAL_FOR_RANG +
                  NUMBER_UROV_SIGNAL_FOR_RANG       +
                  i
                 )
                );

      //Знімаємо всі функції для ранжування входів, які відповідають за ЗОП(КОФ)
      for (int i = 0; i < NUMBER_DEFINED_BUTTONS; i++)
      {
        for(size_t m = 0; m < N_SMALL; ++m) target_label->ranguvannja_buttons[N_SMALL*i + m] &= ~maska[m];
      }

      //Знімаємо всі функції для ранжування входів, які відповідають за ЗОП(КОФ)
      for (int i = 0; i < NUMBER_INPUTS; i++)
      {
        for(size_t m = 0; m < N_SMALL; ++m) target_label->ranguvannja_inputs[N_SMALL*i+m] &= ~maska[m];
      }

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
      //Знімаємо всі функції для ранжування виходів Вх.GOOSE блоку
      for (size_t i = 0; i < N_IN_GOOSE; i++)
      {
        for (size_t j = 0; j < N_IN_GOOSE_MMS_OUT; j++)
        {
          for(size_t m = 0; m < N_SMALL; ++m) target_label->ranguvannja_In_GOOSE[i][j][N_SMALL*i+m] &= ~maska[m];
        }
      }
      
      //Знімаємо всі функції для ранжування виходів Вх.MMS блоку
      for (size_t i = 0; i < N_IN_MMS; i++)
      {
        for (size_t j = 0; j < N_IN_GOOSE_MMS_OUT; j++)
        {
          for(size_t m = 0; m < N_SMALL; ++m) target_label->ranguvannja_In_MMS[i][j][N_SMALL*i+m] &= ~maska[m];
        }
      }
      
      //Знімаємо всі функції для ранжування входів Вихідного Мережевого Блоку
      for (size_t i = 0; i < N_OUT_LAN; i++)
      {
        for (size_t j = 0; j < N_OUT_LAN_IN; j++)
        {
          intptr_t additional_current = 0;
          for (intptr_t k = 0; k < MAX_FUNCTIONS_IN_OUT_LAN; k++)
          {
            unsigned int  value = target_label->ranguvannja_Out_LAN[i][j][k - additional_current];
            
            if (value == 0) break;
            else if ((value >= (RANG_BLOCK_ZOP + 1)) && (value <= (RANG_ZOP + 1)))
            {
              for (intptr_t l = (k - additional_current); l < (MAX_FUNCTIONS_IN_OUT_LAN - additional_current); l++)
              {
                if ((l + 1) < (MAX_FUNCTIONS_IN_OUT_LAN - additional_current)) 
                {
                  target_label->ranguvannja_Out_LAN[i][j][l] = target_label->ranguvannja_Out_LAN[i][j][l + 1];
                }
                else target_label->ranguvannja_Out_LAN[i][j][l] = 0;
              }
              additional_current++;
            }
          }
        }
      }
#endif

      //Знімаємо всі функції для ранжування виходів
      for (int i = 0; i < NUMBER_OUTPUTS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_outputs[N_BIG*i+j] &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування світоіндикаторів
      for (int i = 0; i < NUMBER_LEDS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_leds[N_BIG*i+j] &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування аналогового і дискретного реєстраторів
      for (unsigned int j = 0; j < N_BIG; j++ ) 
      {
        target_label->ranguvannja_analog_registrator[j]  &= ~maska_1[j];
        target_label->ranguvannja_digital_registrator[j] &= ~maska_1[j];
        target_label->ranguvannja_off_cb[j] &= ~maska_1[j];
        target_label->ranguvannja_on_cb[j]  &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування оприділювальних функцій
      for (int i = 0; i < NUMBER_DEFINED_FUNCTIONS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) 
        {
          target_label->ranguvannja_df_source_plus[N_BIG*i+j]  &= ~maska_1[j];
          target_label->ranguvannja_df_source_minus[N_BIG*i+j] &= ~maska_1[j];
          target_label->ranguvannja_df_source_blk[N_BIG*i+j]   &= ~maska_1[j];
          target_label->ranguvannja_df_source_reset[N_BIG*i+j] &= ~maska_1[j];
        }
      }
      //Знімаємо всі функції для ранжування оприділювальних триґерів
      for (int i = 0; i < NUMBER_DEFINED_TRIGGERS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) 
        {
          target_label->ranguvannja_set_dt_source_plus[N_BIG*i+j]    &= ~maska_1[j];
          target_label->ranguvannja_set_dt_source_minus[N_BIG*i+j]   &= ~maska_1[j];
          target_label->ranguvannja_reset_dt_source_plus[N_BIG*i+j]  &= ~maska_1[j];
          target_label->ranguvannja_reset_dt_source_minus[N_BIG*i+j] &= ~maska_1[j];
        }
      }
  
      //Знімаємо всі функції для ранжування визначуваних "І"
      for(unsigned int i = 0; i < NUMBER_DEFINED_AND; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_and[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "АБО"
      for(unsigned int i = 0; i < NUMBER_DEFINED_OR; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_or[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "Викл.АБО"
      for(unsigned int i = 0; i < NUMBER_DEFINED_XOR; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_xor[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "НЕ"
      for(unsigned int i = 0; i < NUMBER_DEFINED_NOT; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_not[N_BIG*i+j] &= ~maska_1[j];
      }
    }

    //Перевіряємо, чи Umin зараз знято з конфігурації
    if ((target_label->configuration & (1<<UMIN_BIT_CONFIGURATION)) == 0)
    {
      //Виводим ступені Umin
      target_label->control_Umin &= (unsigned int)(~(CTR_UMIN1 | CTR_UMIN2));
   
      //Вимикаємо контроль ЗДЗ за напругою
      int32_t ctrl_zdz_type_tmp = target_label->ctrl_zdz_type;
      if (
          (ctrl_zdz_type_tmp == ZDZ_CTRL_U      ) ||
          (ctrl_zdz_type_tmp == ZDZ_CTRL_I_OR_U ) ||
          (ctrl_zdz_type_tmp == ZDZ_CTRL_I_AND_U)
         )
        target_label->ctrl_zdz_type = ZDZ_CTRL_NONE;
      //Виводим ступені Umin з ЗДЗ
      target_label->control_zdz &= (unsigned int)(~(
                                                    MASKA_FOR_BIT(CTR_ZDZ_STARTED_FROM_UMIN1_BIT - (_CTR_ZDZ_PART_III - _CTR_ZDZ_PART_II)) | 
                                                    MASKA_FOR_BIT(CTR_ZDZ_STARTED_FROM_UMIN2_BIT - (_CTR_ZDZ_PART_III - _CTR_ZDZ_PART_II))
                                                   ));
      
      //Виводим ступені Umin з УРОВ
      target_label->control_urov &= (unsigned int)(
                                                   ~(
                                                     MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_UMIN1) | 
                                                     MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_UMIN2)
                                                    )
                                                  );
      
      //Формуємо маски функцій Umin
      for (unsigned int i = 0; i < N_SMALL; i++ ) maska[i] = 0;
      for (int i = 0; i < NUMBER_UMIN_SIGNAL_FOR_RANG_SMALL; i++)
        _SET_BIT(
                 maska, 
                 (
                  NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL    + 
                  NUMBER_MTZ_SIGNAL_FOR_RANG_SMALL        +
                  NUMBER_MTZ04_SIGNAL_FOR_RANG_SMALL      +
                  NUMBER_ZNAM_SIGNAL_FOR_RANG_SMALL       +
                  NUMBER_ZDZ_SIGNAL_FOR_RANG_SMALL        +
                  NUMBER_ZZ_SIGNAL_FOR_RANG_SMALL         +
                  NUMBER_TZNP_SIGNAL_FOR_RANG_SMALL       +
                  NUMBER_APV_SIGNAL_FOR_RANG_SMALL        +
                  NUMBER_ACHR_CHAPV_SIGNAL_FOR_RANG_SMALL +
                  NUMBER_UROV_SIGNAL_FOR_RANG_SMALL       +
                  NUMBER_ZOP_SIGNAL_FOR_RANG_SMALL        +
                  i
                 )
                );
     
      for (unsigned int i = 0; i < N_BIG; i++ ) maska_1[i] = 0;
      for (int i = 0; i < NUMBER_UMIN_SIGNAL_FOR_RANG; i++)
        _SET_BIT(
                 maska_1, 
                 (
                  NUMBER_GENERAL_SIGNAL_FOR_RANG    + 
                  NUMBER_MTZ_SIGNAL_FOR_RANG        +
                  NUMBER_MTZ04_SIGNAL_FOR_RANG      +
                  NUMBER_ZNAM_SIGNAL_FOR_RANG       +
                  NUMBER_ZDZ_SIGNAL_FOR_RANG        +
                  NUMBER_ZZ_SIGNAL_FOR_RANG         +
                  NUMBER_TZNP_SIGNAL_FOR_RANG       +
                  NUMBER_APV_SIGNAL_FOR_RANG        +
                  NUMBER_ACHR_CHAPV_SIGNAL_FOR_RANG +
                  NUMBER_UROV_SIGNAL_FOR_RANG       +
                  NUMBER_ZOP_SIGNAL_FOR_RANG       +
                  i
                 )
                );

      //Знімаємо всі функції для ранжування входів, які відповідають за Umin
      for (int i = 0; i < NUMBER_DEFINED_BUTTONS; i++)
      {
        for(size_t m = 0; m < N_SMALL; ++m) target_label->ranguvannja_buttons[N_SMALL*i + m] &= ~maska[m];
      }

      //Знімаємо всі функції для ранжування входів, які відповідають за Umin
      for (int i = 0; i < NUMBER_INPUTS; i++)
      {
        for(size_t m = 0; m < N_SMALL; ++m) target_label->ranguvannja_inputs[N_SMALL*i+m] &= ~maska[m];
      }

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
      //Знімаємо всі функції для ранжування виходів Вх.GOOSE блоку
      for (size_t i = 0; i < N_IN_GOOSE; i++)
      {
        for (size_t j = 0; j < N_IN_GOOSE_MMS_OUT; j++)
        {
          for(size_t m = 0; m < N_SMALL; ++m) target_label->ranguvannja_In_GOOSE[i][j][N_SMALL*i+m] &= ~maska[m];
        }
      }
      
      //Знімаємо всі функції для ранжування виходів Вх.MMS блоку
      for (size_t i = 0; i < N_IN_MMS; i++)
      {
        for (size_t j = 0; j < N_IN_GOOSE_MMS_OUT; j++)
        {
          for(size_t m = 0; m < N_SMALL; ++m) target_label->ranguvannja_In_MMS[i][j][N_SMALL*i+m] &= ~maska[m];
        }
      }
      
      //Знімаємо всі функції для ранжування входів Вихідного Мережевого Блоку
      for (size_t i = 0; i < N_OUT_LAN; i++)
      {
        for (size_t j = 0; j < N_OUT_LAN_IN; j++)
        {
          intptr_t additional_current = 0;
          for (intptr_t k = 0; k < MAX_FUNCTIONS_IN_OUT_LAN; k++)
          {
            unsigned int  value = target_label->ranguvannja_Out_LAN[i][j][k - additional_current];
            
            if (value == 0) break;
            else if ((value >= (RANG_BLOCK_UMIN1 + 1)) && (value <= (RANG_UMIN2 + 1)))
            {
              for (intptr_t l = (k - additional_current); l < (MAX_FUNCTIONS_IN_OUT_LAN - additional_current); l++)
              {
                if ((l + 1) < (MAX_FUNCTIONS_IN_OUT_LAN - additional_current)) 
                {
                  target_label->ranguvannja_Out_LAN[i][j][l] = target_label->ranguvannja_Out_LAN[i][j][l + 1];
                }
                else target_label->ranguvannja_Out_LAN[i][j][l] = 0;
              }
              additional_current++;
            }
          }
        }
      }
#endif

      //Знімаємо всі функції для ранжування виходів
      for (int i = 0; i < NUMBER_OUTPUTS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_outputs[N_BIG*i+j] &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування світоіндикаторів
      for (int i = 0; i < NUMBER_LEDS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_leds[N_BIG*i+j] &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування аналогового і дискретного реєстраторів
      for (unsigned int j = 0; j < N_BIG; j++ ) 
      {
        target_label->ranguvannja_analog_registrator[j]  &= ~maska_1[j];
        target_label->ranguvannja_digital_registrator[j] &= ~maska_1[j];
        target_label->ranguvannja_off_cb[j] &= ~maska_1[j];
        target_label->ranguvannja_on_cb[j]  &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування оприділювальних функцій
      for (int i = 0; i < NUMBER_DEFINED_FUNCTIONS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) 
        {
          target_label->ranguvannja_df_source_plus[N_BIG*i+j]  &= ~maska_1[j];
          target_label->ranguvannja_df_source_minus[N_BIG*i+j] &= ~maska_1[j];
          target_label->ranguvannja_df_source_blk[N_BIG*i+j]   &= ~maska_1[j];
          target_label->ranguvannja_df_source_reset[N_BIG*i+j] &= ~maska_1[j];
        }
      }
      //Знімаємо всі функції для ранжування оприділювальних триґерів
      for (int i = 0; i < NUMBER_DEFINED_TRIGGERS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) 
        {
          target_label->ranguvannja_set_dt_source_plus[N_BIG*i+j]    &= ~maska_1[j];
          target_label->ranguvannja_set_dt_source_minus[N_BIG*i+j]   &= ~maska_1[j];
          target_label->ranguvannja_reset_dt_source_plus[N_BIG*i+j]  &= ~maska_1[j];
          target_label->ranguvannja_reset_dt_source_minus[N_BIG*i+j] &= ~maska_1[j];
        }
      }
  
      //Знімаємо всі функції для ранжування визначуваних "І"
      for(unsigned int i = 0; i < NUMBER_DEFINED_AND; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_and[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "АБО"
      for(unsigned int i = 0; i < NUMBER_DEFINED_OR; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_or[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "Викл.АБО"
      for(unsigned int i = 0; i < NUMBER_DEFINED_XOR; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_xor[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "НЕ"
      for(unsigned int i = 0; i < NUMBER_DEFINED_NOT; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_not[N_BIG*i+j] &= ~maska_1[j];
      }
    }

    //Перевіряємо, чи Umax зараз знято з конфігурації
    if ((target_label->configuration & (1<<UMAX_BIT_CONFIGURATION)) == 0)
    {
      //Виводим ступені Umax
      target_label->control_Umax &= (unsigned int)(~(CTR_UMAX1 | CTR_UMAX2));

      //Виводим ступені Umax з УРОВ
      target_label->control_urov &= (unsigned int)(
                                                   ~(
                                                     MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_UMAX1) |
                                                     MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_UMAX2)
                                                    )
                                                  );
   
      //Формуємо маски функцій Umax
      for (unsigned int i = 0; i < N_SMALL; i++ ) maska[i] = 0;
      for (int i = 0; i < NUMBER_UMAX_SIGNAL_FOR_RANG_SMALL; i++)
        _SET_BIT(
                 maska, 
                 (
                  NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL    + 
                  NUMBER_MTZ_SIGNAL_FOR_RANG_SMALL        +
                  NUMBER_MTZ04_SIGNAL_FOR_RANG_SMALL      +
                  NUMBER_ZNAM_SIGNAL_FOR_RANG_SMALL       +
                  NUMBER_ZDZ_SIGNAL_FOR_RANG_SMALL        +
                  NUMBER_ZZ_SIGNAL_FOR_RANG_SMALL         +
                  NUMBER_TZNP_SIGNAL_FOR_RANG_SMALL       +
                  NUMBER_APV_SIGNAL_FOR_RANG_SMALL        +
                  NUMBER_ACHR_CHAPV_SIGNAL_FOR_RANG_SMALL +
                  NUMBER_UROV_SIGNAL_FOR_RANG_SMALL       +
                  NUMBER_ZOP_SIGNAL_FOR_RANG_SMALL        +
                  NUMBER_UMIN_SIGNAL_FOR_RANG_SMALL       +
                  i
                 )
                );
     
      for (unsigned int i = 0; i < N_BIG; i++ ) maska_1[i] = 0;
      for (int i = 0; i < NUMBER_UMAX_SIGNAL_FOR_RANG; i++)
        _SET_BIT(
                 maska_1, 
                 (
                  NUMBER_GENERAL_SIGNAL_FOR_RANG    + 
                  NUMBER_MTZ_SIGNAL_FOR_RANG        +
                  NUMBER_MTZ04_SIGNAL_FOR_RANG      +
                  NUMBER_ZNAM_SIGNAL_FOR_RANG       +
                  NUMBER_ZDZ_SIGNAL_FOR_RANG        +
                  NUMBER_ZZ_SIGNAL_FOR_RANG         +
                  NUMBER_TZNP_SIGNAL_FOR_RANG       +
                  NUMBER_APV_SIGNAL_FOR_RANG        +
                  NUMBER_ACHR_CHAPV_SIGNAL_FOR_RANG +
                  NUMBER_UROV_SIGNAL_FOR_RANG       +
                  NUMBER_ZOP_SIGNAL_FOR_RANG        +
                  NUMBER_UMIN_SIGNAL_FOR_RANG       +
                  i
                 )
                );


      //Знімаємо всі функції для ранжування входів, які відповідають за Umax
      for (int i = 0; i < NUMBER_DEFINED_BUTTONS; i++)
      {
        for(size_t m = 0; m < N_SMALL; ++m) target_label->ranguvannja_buttons[N_SMALL*i + m] &= ~maska[m];
      }

      //Знімаємо всі функції для ранжування входів, які відповідають за Umax
      for (int i = 0; i < NUMBER_INPUTS; i++)
      {
        for(size_t m = 0; m < N_SMALL; ++m) target_label->ranguvannja_inputs[N_SMALL*i+m] &= ~maska[m];
      }

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
      //Знімаємо всі функції для ранжування виходів Вх.GOOSE блоку
      for (size_t i = 0; i < N_IN_GOOSE; i++)
      {
        for (size_t j = 0; j < N_IN_GOOSE_MMS_OUT; j++)
        {
          for(size_t m = 0; m < N_SMALL; ++m) target_label->ranguvannja_In_GOOSE[i][j][N_SMALL*i+m] &= ~maska[m];
        }
      }
      
      //Знімаємо всі функції для ранжування виходів Вх.MMS блоку
      for (size_t i = 0; i < N_IN_MMS; i++)
      {
        for (size_t j = 0; j < N_IN_GOOSE_MMS_OUT; j++)
        {
          for(size_t m = 0; m < N_SMALL; ++m) target_label->ranguvannja_In_MMS[i][j][N_SMALL*i+m] &= ~maska[m];
        }
      }
      
      //Знімаємо всі функції для ранжування входів Вихідного Мережевого Блоку
      for (size_t i = 0; i < N_OUT_LAN; i++)
      {
        for (size_t j = 0; j < N_OUT_LAN_IN; j++)
        {
          intptr_t additional_current = 0;
          for (intptr_t k = 0; k < MAX_FUNCTIONS_IN_OUT_LAN; k++)
          {
            unsigned int  value = target_label->ranguvannja_Out_LAN[i][j][k - additional_current];
            
            if (value == 0) break;
            else if ((value >= (RANG_BLOCK_UMAX1 + 1)) && (value <= (RANG_UMAX2 + 1)))
            {
              for (intptr_t l = (k - additional_current); l < (MAX_FUNCTIONS_IN_OUT_LAN - additional_current); l++)
              {
                if ((l + 1) < (MAX_FUNCTIONS_IN_OUT_LAN - additional_current)) 
                {
                  target_label->ranguvannja_Out_LAN[i][j][l] = target_label->ranguvannja_Out_LAN[i][j][l + 1];
                }
                else target_label->ranguvannja_Out_LAN[i][j][l] = 0;
              }
              additional_current++;
            }
          }
        }
      }
#endif

      //Знімаємо всі функції для ранжування виходів
      for (int i = 0; i < NUMBER_OUTPUTS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_outputs[N_BIG*i+j] &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування світоіндикаторів
      for (int i = 0; i < NUMBER_LEDS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_leds[N_BIG*i+j] &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування аналогового і дискретного реєстраторів
      for (unsigned int j = 0; j < N_BIG; j++ ) 
      {
        target_label->ranguvannja_analog_registrator[j]  &= ~maska_1[j];
        target_label->ranguvannja_digital_registrator[j] &= ~maska_1[j];
        target_label->ranguvannja_off_cb[j] &= ~maska_1[j];
        target_label->ranguvannja_on_cb[j]  &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування оприділювальних функцій
      for (int i = 0; i < NUMBER_DEFINED_FUNCTIONS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) 
        {
          target_label->ranguvannja_df_source_plus[N_BIG*i+j]  &= ~maska_1[j];
          target_label->ranguvannja_df_source_minus[N_BIG*i+j] &= ~maska_1[j];
          target_label->ranguvannja_df_source_blk[N_BIG*i+j]   &= ~maska_1[j];
          target_label->ranguvannja_df_source_reset[N_BIG*i+j] &= ~maska_1[j];
        }
      }
      //Знімаємо всі функції для ранжування оприділювальних триґерів
      for (int i = 0; i < NUMBER_DEFINED_TRIGGERS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) 
        {
          target_label->ranguvannja_set_dt_source_plus[N_BIG*i+j]    &= ~maska_1[j];
          target_label->ranguvannja_set_dt_source_minus[N_BIG*i+j]   &= ~maska_1[j];
          target_label->ranguvannja_reset_dt_source_plus[N_BIG*i+j]  &= ~maska_1[j];
          target_label->ranguvannja_reset_dt_source_minus[N_BIG*i+j] &= ~maska_1[j];
        }
      }
  
      //Знімаємо всі функції для ранжування визначуваних "І"
      for(unsigned int i = 0; i < NUMBER_DEFINED_AND; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_and[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "АБО"
      for(unsigned int i = 0; i < NUMBER_DEFINED_OR; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_or[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "Викл.АБО"
      for(unsigned int i = 0; i < NUMBER_DEFINED_XOR; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_xor[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "НЕ"
      for(unsigned int i = 0; i < NUMBER_DEFINED_NOT; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_not[N_BIG*i+j] &= ~maska_1[j];
      }
    }

    //Перевіряємо, чи Універсальний захист зараз знято з конфігурації
    if ((target_label->configuration & (1<<UP_BIT_CONFIGURATION)) == 0)
    {
      for (size_t i = 0; i < NUMBER_UP; i++)
      {
        //Виводим ступені УЗ
        target_label->control_UP &= (unsigned int)(~MASKA_FOR_BIT(i*(_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_STATE_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I)));

        //Виводим ступені УЗ з УРОВ
        target_label->control_urov &= (unsigned int)(~((MASKA_FOR_BIT(NUMBER_UP) - 1) << INDEX_ML_CTRUROV_STARTED_FROM_UP1));
      }
   
      //Формуємо маски функцій УЗ
      for (unsigned int i = 0; i < N_SMALL; i++ ) maska[i] = 0;
      for (int i = 0; i < NUMBER_UP_SIGNAL_FOR_RANG_SMALL; i++)
        _SET_BIT(
                 maska, 
                 (
                  NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL    + 
                  NUMBER_MTZ_SIGNAL_FOR_RANG_SMALL        +
                  NUMBER_MTZ04_SIGNAL_FOR_RANG_SMALL      +
                  NUMBER_ZNAM_SIGNAL_FOR_RANG_SMALL       +
                  NUMBER_ZDZ_SIGNAL_FOR_RANG_SMALL        +
                  NUMBER_ZZ_SIGNAL_FOR_RANG_SMALL         +
                  NUMBER_TZNP_SIGNAL_FOR_RANG_SMALL       +
                  NUMBER_APV_SIGNAL_FOR_RANG_SMALL        +
                  NUMBER_ACHR_CHAPV_SIGNAL_FOR_RANG_SMALL +
                  NUMBER_UROV_SIGNAL_FOR_RANG_SMALL       +
                  NUMBER_ZOP_SIGNAL_FOR_RANG_SMALL        +
                  NUMBER_UMIN_SIGNAL_FOR_RANG_SMALL       +
                  NUMBER_UMAX_SIGNAL_FOR_RANG_SMALL       +
                  i
                 )
                );
     
      for (unsigned int i = 0; i < N_BIG; i++ ) maska_1[i] = 0;
      for (int i = 0; i < NUMBER_UP_SIGNAL_FOR_RANG; i++)
        _SET_BIT(
                 maska_1, 
                 (
                  NUMBER_GENERAL_SIGNAL_FOR_RANG    + 
                  NUMBER_MTZ_SIGNAL_FOR_RANG        +
                  NUMBER_MTZ04_SIGNAL_FOR_RANG      +
                  NUMBER_ZNAM_SIGNAL_FOR_RANG       +
                  NUMBER_ZDZ_SIGNAL_FOR_RANG        +
                  NUMBER_ZZ_SIGNAL_FOR_RANG         +
                  NUMBER_TZNP_SIGNAL_FOR_RANG       +
                  NUMBER_APV_SIGNAL_FOR_RANG        +
                  NUMBER_ACHR_CHAPV_SIGNAL_FOR_RANG +
                  NUMBER_UROV_SIGNAL_FOR_RANG       +
                  NUMBER_ZOP_SIGNAL_FOR_RANG        +
                  NUMBER_UMIN_SIGNAL_FOR_RANG       +
                  NUMBER_UMAX_SIGNAL_FOR_RANG       +
                  i
                 )
                );


      //Знімаємо всі функції для ранжування входів, які відповідають за УЗ
      for (int i = 0; i < NUMBER_DEFINED_BUTTONS; i++)
      {
        for(size_t m = 0; m < N_SMALL; ++m) target_label->ranguvannja_buttons[N_SMALL*i + m] &= ~maska[m];
      }

      //Знімаємо всі функції для ранжування входів, які відповідають за УЗ
      for (int i = 0; i < NUMBER_INPUTS; i++)
      {
        for(size_t m = 0; m < N_SMALL; ++m) target_label->ranguvannja_inputs[N_SMALL*i+m] &= ~maska[m];
      }

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
      //Знімаємо всі функції для ранжування виходів Вх.GOOSE блоку
      for (size_t i = 0; i < N_IN_GOOSE; i++)
      {
        for (size_t j = 0; j < N_IN_GOOSE_MMS_OUT; j++)
        {
          for(size_t m = 0; m < N_SMALL; ++m) target_label->ranguvannja_In_GOOSE[i][j][N_SMALL*i+m] &= ~maska[m];
        }
      }
      
      //Знімаємо всі функції для ранжування виходів Вх.MMS блоку
      for (size_t i = 0; i < N_IN_MMS; i++)
      {
        for (size_t j = 0; j < N_IN_GOOSE_MMS_OUT; j++)
        {
          for(size_t m = 0; m < N_SMALL; ++m) target_label->ranguvannja_In_MMS[i][j][N_SMALL*i+m] &= ~maska[m];
        }
      }
      
      //Знімаємо всі функції для ранжування входів Вихідного Мережевого Блоку
      for (size_t i = 0; i < N_OUT_LAN; i++)
      {
        for (size_t j = 0; j < N_OUT_LAN_IN; j++)
        {
          intptr_t additional_current = 0;
          for (intptr_t k = 0; k < MAX_FUNCTIONS_IN_OUT_LAN; k++)
          {
            unsigned int  value = target_label->ranguvannja_Out_LAN[i][j][k - additional_current];
            
            if (value == 0) break;
            else if ((value >= (RANG_BLOCK_UP1 + 1)) && (value <= ((RANG_BLOCK_UP1 + 3*NUMBER_UP - 1) + 1)))
            {
              for (intptr_t l = (k - additional_current); l < (MAX_FUNCTIONS_IN_OUT_LAN - additional_current); l++)
              {
                if ((l + 1) < (MAX_FUNCTIONS_IN_OUT_LAN - additional_current)) 
                {
                  target_label->ranguvannja_Out_LAN[i][j][l] = target_label->ranguvannja_Out_LAN[i][j][l + 1];
                }
                else target_label->ranguvannja_Out_LAN[i][j][l] = 0;
              }
              additional_current++;
            }
          }
        }
      }
#endif

      //Знімаємо всі функції для ранжування виходів
      for (int i = 0; i < NUMBER_OUTPUTS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_outputs[N_BIG*i+j] &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування світоіндикаторів
      for (int i = 0; i < NUMBER_LEDS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_leds[N_BIG*i+j] &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування аналогового і дискретного реєстраторів
      for (unsigned int j = 0; j < N_BIG; j++ ) 
      {
        target_label->ranguvannja_analog_registrator[j]  &= ~maska_1[j];
        target_label->ranguvannja_digital_registrator[j] &= ~maska_1[j];
        target_label->ranguvannja_off_cb[j] &= ~maska_1[j];
        target_label->ranguvannja_on_cb[j]  &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування оприділювальних функцій
      for (int i = 0; i < NUMBER_DEFINED_FUNCTIONS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) 
        {
          target_label->ranguvannja_df_source_plus[N_BIG*i+j]  &= ~maska_1[j];
          target_label->ranguvannja_df_source_minus[N_BIG*i+j] &= ~maska_1[j];
          target_label->ranguvannja_df_source_blk[N_BIG*i+j]   &= ~maska_1[j];
          target_label->ranguvannja_df_source_reset[N_BIG*i+j] &= ~maska_1[j];
        }
      }
      //Знімаємо всі функції для ранжування оприділювальних триґерів
      for (int i = 0; i < NUMBER_DEFINED_TRIGGERS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) 
        {
          target_label->ranguvannja_set_dt_source_plus[N_BIG*i+j]    &= ~maska_1[j];
          target_label->ranguvannja_set_dt_source_minus[N_BIG*i+j]   &= ~maska_1[j];
          target_label->ranguvannja_reset_dt_source_plus[N_BIG*i+j]  &= ~maska_1[j];
          target_label->ranguvannja_reset_dt_source_minus[N_BIG*i+j] &= ~maska_1[j];
        }
      }
  
      //Знімаємо всі функції для ранжування визначуваних "І"
      for(unsigned int i = 0; i < NUMBER_DEFINED_AND; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_and[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "АБО"
      for(unsigned int i = 0; i < NUMBER_DEFINED_OR; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_or[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "Викл.АБО"
      for(unsigned int i = 0; i < NUMBER_DEFINED_XOR; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_xor[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "НЕ"
      for(unsigned int i = 0; i < NUMBER_DEFINED_NOT; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_not[N_BIG*i+j] &= ~maska_1[j];
      }
    }

    //Перевіряємо, чи "Визначення місця пошкодження" зараз знято з конфігурації
    if ((target_label->configuration & (1<<VMP_BIT_CONFIGURATION)) == 0)
    {
      //Виводим ВМП
      target_label->control_vmp &= (unsigned int)(~CTR_VMP_STATE);
    }

    //"Розширена логіка"
    if ((target_label->configuration & (1<<EL_BIT_CONFIGURATION)) == 0)
    {
      //Видаляємо всі сигнали розгиреної логіки
      
      //Формуємо маски функцій Розширеної логіки
      for (unsigned int i = 0; i < N_SMALL; i++ ) maska[i] = 0;
      for (int i = 0; i < NUMBER_EL_SIGNAL_FOR_RANG_SMALL; i++)
        _SET_BIT(
                 maska, 
                 (
                  NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL    + 
                  NUMBER_MTZ_SIGNAL_FOR_RANG_SMALL        +
                  NUMBER_MTZ04_SIGNAL_FOR_RANG_SMALL      +
                  NUMBER_ZNAM_SIGNAL_FOR_RANG_SMALL       +
                  NUMBER_ZDZ_SIGNAL_FOR_RANG_SMALL        +
                  NUMBER_ZZ_SIGNAL_FOR_RANG_SMALL         +
                  NUMBER_TZNP_SIGNAL_FOR_RANG_SMALL       +
                  NUMBER_APV_SIGNAL_FOR_RANG_SMALL        +
                  NUMBER_ACHR_CHAPV_SIGNAL_FOR_RANG_SMALL +
                  NUMBER_UROV_SIGNAL_FOR_RANG_SMALL       +
                  NUMBER_ZOP_SIGNAL_FOR_RANG_SMALL        +
                  NUMBER_UMIN_SIGNAL_FOR_RANG_SMALL       +
                  NUMBER_UMAX_SIGNAL_FOR_RANG_SMALL       +
                  NUMBER_UP_SIGNAL_FOR_RANG_SMALL         +
                  NUMBER_VMP_SIGNAL_FOR_RANG_SMALL        +
                  i
                 )
                );
     
      for (unsigned int i = 0; i < N_BIG; i++ ) maska_1[i] = 0;
      for (int i = 0; i < NUMBER_EL_SIGNAL_FOR_RANG; i++)
        _SET_BIT(
                 maska_1, 
                 (
                  NUMBER_GENERAL_SIGNAL_FOR_RANG    + 
                  NUMBER_MTZ_SIGNAL_FOR_RANG        +
                  NUMBER_MTZ04_SIGNAL_FOR_RANG      +
                  NUMBER_ZNAM_SIGNAL_FOR_RANG       +
                  NUMBER_ZDZ_SIGNAL_FOR_RANG        +
                  NUMBER_ZZ_SIGNAL_FOR_RANG         +
                  NUMBER_TZNP_SIGNAL_FOR_RANG       +
                  NUMBER_APV_SIGNAL_FOR_RANG        +
                  NUMBER_ACHR_CHAPV_SIGNAL_FOR_RANG +
                  NUMBER_UROV_SIGNAL_FOR_RANG       +
                  NUMBER_ZOP_SIGNAL_FOR_RANG        +
                  NUMBER_UMIN_SIGNAL_FOR_RANG       +
                  NUMBER_UMAX_SIGNAL_FOR_RANG       +
                  NUMBER_UP_SIGNAL_FOR_RANG         +
                  NUMBER_VMP_SIGNAL_FOR_RANG        +
                  i
                 )
                );

      //Знімаємо всі функції для ранжування входів, які відповідають за Розширеної логіки
      for (int i = 0; i < NUMBER_DEFINED_BUTTONS; i++)
      {
        for(size_t m = 0; m < N_SMALL; ++m) target_label->ranguvannja_buttons[N_SMALL*i+m] &= ~maska[m];
      }
    
      //Знімаємо всі функції для ранжування входів, які відповідають за Розширеної логіки
      for (int i = 0; i < NUMBER_INPUTS; i++)
      {
        for(size_t m = 0; m < N_SMALL; ++m) target_label->ranguvannja_inputs[N_SMALL*i+m] &= ~maska[m];
      }

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
      //Знімаємо всі функції для ранжування виходів Вх.GOOSE блоку
      for (size_t i = 0; i < N_IN_GOOSE; i++)
      {
        for (size_t j = 0; j < N_IN_GOOSE_MMS_OUT; j++)
        {
          for(size_t m = 0; m < N_SMALL; ++m) target_label->ranguvannja_In_GOOSE[i][j][N_SMALL*i+m] &= ~maska[m];
        }
      }
      
      //Знімаємо всі функції для ранжування виходів Вх.MMS блоку
      for (size_t i = 0; i < N_IN_MMS; i++)
      {
        for (size_t j = 0; j < N_IN_GOOSE_MMS_OUT; j++)
        {
          for(size_t m = 0; m < N_SMALL; ++m) target_label->ranguvannja_In_MMS[i][j][N_SMALL*i+m] &= ~maska[m];
        }
      }
      
      //Знімаємо всі функції для ранжування входів Вихідного Мережевого Блоку
      for (size_t i = 0; i < N_OUT_LAN; i++)
      {
        for (size_t j = 0; j < N_OUT_LAN_IN; j++)
        {
          intptr_t additional_current = 0;
          for (intptr_t k = 0; k < MAX_FUNCTIONS_IN_OUT_LAN; k++)
          {
            unsigned int  value = target_label->ranguvannja_Out_LAN[i][j][k - additional_current];
            
            if (value == 0) break;
            else if ((value >= (RANG_DF1_IN + 1)) && (value <= (RANG_ERROR_CONF_EL + 1)))
            {
              for (intptr_t l = (k - additional_current); l < (MAX_FUNCTIONS_IN_OUT_LAN - additional_current); l++)
              {
                if ((l + 1) < (MAX_FUNCTIONS_IN_OUT_LAN - additional_current)) 
                {
                  target_label->ranguvannja_Out_LAN[i][j][l] = target_label->ranguvannja_Out_LAN[i][j][l + 1];
                }
                else target_label->ranguvannja_Out_LAN[i][j][l] = 0;
              }
              additional_current++;
            }
          }
        }
      }
#endif

      //Знімаємо всі функції для ранжування виходів
      for (int i = 0; i < NUMBER_OUTPUTS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_outputs[N_BIG*i+j] &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування світоіндикаторів
      for (int i = 0; i < NUMBER_LEDS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_leds[N_BIG*i+j] &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування аналогового і дискретного реєстраторів
      for (unsigned int j = 0; j < N_BIG; j++ ) 
      {
        target_label->ranguvannja_analog_registrator[j]  &= ~maska_1[j];
        target_label->ranguvannja_digital_registrator[j] &= ~maska_1[j];
        target_label->ranguvannja_off_cb[j] &= ~maska_1[j];
        target_label->ranguvannja_on_cb[j]  &= ~maska_1[j];
      }


      //Очищємо всі функції для ранжування оприділювальних функцій
      for (int i = 0; i < NUMBER_DEFINED_FUNCTIONS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) 
        {
          target_label->ranguvannja_df_source_plus[N_BIG*i+j]  = 0;
          target_label->ranguvannja_df_source_minus[N_BIG*i+j] = 0;
          target_label->ranguvannja_df_source_blk[N_BIG*i+j]   = 0;
          target_label->ranguvannja_df_source_reset[N_BIG*i+j] = 0;
        }
      }
      //Очищємо всі функції для ранжування оприділювальних триґерів
      for (int i = 0; i < NUMBER_DEFINED_TRIGGERS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) 
        {
          target_label->ranguvannja_set_dt_source_plus[N_BIG*i+j]    = 0;
          target_label->ranguvannja_set_dt_source_minus[N_BIG*i+j]   = 0;
          target_label->ranguvannja_reset_dt_source_plus[N_BIG*i+j]  = 0;
          target_label->ranguvannja_reset_dt_source_minus[N_BIG*i+j] = 0;
        }
      }
  
      //Очищємо всі функції для ранжування визначуваних "І"
      for(unsigned int i = 0; i < NUMBER_DEFINED_AND; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_and[N_BIG*i+j] = 0;
      }
  
      //Очищємо всі функції для ранжування визначуваних "АБО"
      for(unsigned int i = 0; i < NUMBER_DEFINED_OR; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_or[N_BIG*i+j] = 0;
      }
  
      //Очищємо всі функції для ранжування визначуваних "Викл.АБО"
      for(unsigned int i = 0; i < NUMBER_DEFINED_XOR; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_xor[N_BIG*i+j] = 0;
      }
  
      //Очищємо всі функції для ранжування визначуваних "НЕ"
      for(unsigned int i = 0; i < NUMBER_DEFINED_NOT; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_not[N_BIG*i+j] = 0;
      }

      for (size_t i = 0; i < NUMBER_TRANSFER_FUNCTIONS; i++) target_label->ranguvannja_tf[i] = 0;
    }
    else
    {
      for (size_t i = 0; i < _FIX_NUMBER_PROTECTION; i++)
      {
         if ((target_label->configuration & (1 << i)) == 0)
         {
           for (size_t j = 0; j < NUMBER_TRANSFER_FUNCTIONS; j++)
           {
             uint32_t value = target_label->ranguvannja_tf[j];
             uint32_t value_1 = (value >> (16*0)) & 0xffff;
             if (
                 (value_1 >= max_value_for_tf[1 + i - 1][0]) &&
                 (value_1 <  max_value_for_tf[1 + i    ][0])
                ) 
             {
               value_1 = 0;
             }
         
             uint32_t value_2 = (value >> (16*1)) & 0xffff;
             if (
                 (value_2 >= max_value_for_tf[1 + i - 1][1]) &&
                 (value_2 <  max_value_for_tf[1 + i    ][1])
                ) 
             {
               value_2 = 0;
             }

             target_label->ranguvannja_tf[j] = (value_2 << (16*1)) | (value_1 << (16*0));
           }
         }
      }
    }

#ifdef NUMBER_DS
    //Налаштування для ДШ
    if ((target_label->configuration & (1<<DS_BIT_CONFIGURATION)) == 0)
    {
      //Знімаємо всі функції для ДШ
      for (size_t i = NUMBER_SIMPLE_OUTPUTS; i < NUMBER_OUTPUTS; ++i)
      {
        for (size_t j = 0; j < N_BIG; ++j ) target_label->ranguvannja_outputs[N_BIG*i+j] = 0;
      }
    }
#endif

  }  
  return error_window;
}
/*****************************************************/

/*****************************************************/
//Функція обновлення змінних при зміні типу вхідних напруг
/*****************************************************/
unsigned int action_after_changing_extra_settings(unsigned int new_value, __SETTINGS *target_label)
{
  unsigned int error = 0;
  if ((new_value & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE)) != 0)
  {
    if (current_ekran.current_level == EKRAN_TRANSFORMATOR_INFO_CONTROL) error |= (unsigned int)(1 << 31);
  }
  
  if (error == 0)
  {
    unsigned int new_configuration = target_label->configuration;
    if ((new_value & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_IB_I04)) == 0)
    {
      //Підготовляємо вивід з конфігурації МТЗ 0.4кВ
      new_configuration &= (unsigned int)(~(1 << MTZ04_BIT_CONFIGURATION));
    }
    else
    { 
      //Підготовляємо вивід з конфігурації СЗНП
      new_configuration &= (unsigned int)(~(1 << TZNP_BIT_CONFIGURATION));
    }
  
    //Виконуєм спробу зміни конфігурації по зміні Ib/I0.4 і повертаємо результат цієї спроби
    error |= action_after_changing_of_configuration(new_configuration, target_label);
    if (error == 0) 
    {
      //Вводимо в дію номі значення  поля додаткових налаштувань
      target_label->control_extra_settings_1 = new_value;
    
      for (size_t i = 0; i < NUMBER_UP; i++) 
      {
        if (
            ((target_label->ctrl_UP_input[i] == UP_CTRL_I04  ) && ((new_value & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_IB_I04)) == 0)) ||
            ((target_label->ctrl_UP_input[i] == UP_CTRL_3I0_r) && ((new_value & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_IB_I04)) != 0)) ||
            (
             (  
              (target_label->ctrl_UP_input[i] == UP_CTRL_Ua_Ub_Uc) ||
              (target_label->ctrl_UP_input[i] == UP_CTRL_Ua) ||
              (target_label->ctrl_UP_input[i] == UP_CTRL_Ub) ||
              (target_label->ctrl_UP_input[i] == UP_CTRL_Uc) ||
              (target_label->ctrl_UP_input[i] == UP_CTRL_U1) ||
              (target_label->ctrl_UP_input[i] == UP_CTRL_U2)
             )
             &&
             ((new_value & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE)) != 0)  
            ) 
           )   
        {
          action_after_changing_input_UP(target_label, i, UP_CTRL_Ia_Ib_Ic);
        }
      }
      
      if ((new_value & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE)) != 0)
      {
        target_label->control_transformator |= MASKA_FOR_BIT(INDEX_ML_CTR_TRANSFORMATOR_PHASE_LINE);
      }
    }
  }
  
  return error;
}
/*****************************************************/

/*****************************************************/
//Функція обновлення змінних при зміні типу реле 33/3I0
/*****************************************************/
void action_after_changing_zz1_type(__SETTINGS *target_label)
{
  if ((target_label->control_zz & CTR_ZZ1_TYPE) != 0)
  {
    //Виводим СЗЗ і Сектор СЗЗ
    target_label->control_zz &= (unsigned int)(~(CTR_ZZ1_NZZ_STATE/* | CTR_ZZ1_SECTOR*/));
    
    //Виводим СЗЗ з УРОВ
    target_label->control_urov &= (unsigned int)(~MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_NZZ));
      
    unsigned int maska_1[N_BIG];
    for (unsigned int *p = maska_1; p != (maska_1 + N_BIG); ++p) *p = 0;
    
    _SET_BIT(maska_1, RANG_PO_NZZ);
    _SET_BIT(maska_1, RANG_NZZ);
    _SET_BIT(maska_1, RANG_SECTOR_NZZ);
    
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
      //Знімаємо всі функції для ранжування входів Вихідного Мережевого Блоку
      for (size_t i = 0; i < N_OUT_LAN; i++)
      {
        for (size_t j = 0; j < N_OUT_LAN_IN; j++)
        {
          intptr_t additional_current = 0;
          for (intptr_t k = 0; k < MAX_FUNCTIONS_IN_OUT_LAN; k++)
          {
            unsigned int  value = target_label->ranguvannja_Out_LAN[i][j][k - additional_current];
            
            if (value == 0) break;
            else if (
                     (value == (RANG_PO_NZZ     + 1)) ||
                     (value == (RANG_NZZ        + 1)) ||
                     (value == (RANG_SECTOR_NZZ + 1))
                    )
            {
              for (intptr_t l = (k - additional_current); l < (MAX_FUNCTIONS_IN_OUT_LAN - additional_current); l++)
              {
                if ((l + 1) < (MAX_FUNCTIONS_IN_OUT_LAN - additional_current)) 
                {
                  target_label->ranguvannja_Out_LAN[i][j][l] = target_label->ranguvannja_Out_LAN[i][j][l + 1];
                }
                else target_label->ranguvannja_Out_LAN[i][j][l] = 0;
              }
              additional_current++;
            }
          }
        }
      }
#endif

    //Знімаємо всі функції для ранжування виходів
    for (int i = 0; i < NUMBER_OUTPUTS; i++)
    {
      for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_outputs[N_BIG*i+j] &= ~maska_1[j];
    }
    //Знімаємо всі функції для ранжування світоіндикаторів
    for (int i = 0; i < NUMBER_LEDS; i++)
    {
      for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_leds[N_BIG*i+j] &= ~maska_1[j];
    }
    //Знімаємо всі функції для ранжування аналогового і дискретного реєстраторів
    for (unsigned int j = 0; j < N_BIG; j++ ) 
    {
      target_label->ranguvannja_analog_registrator[j]  &= ~maska_1[j];
      target_label->ranguvannja_digital_registrator[j] &= ~maska_1[j];
      target_label->ranguvannja_off_cb[j] &= ~maska_1[j];
      target_label->ranguvannja_on_cb[j]  &= ~maska_1[j];
    }
    //Знімаємо всі функції для ранжування оприділювальних функцій
    for (int i = 0; i < NUMBER_DEFINED_FUNCTIONS; i++)
    {
      for (unsigned int j = 0; j < N_BIG; j++ ) 
      {
        target_label->ranguvannja_df_source_plus[N_BIG*i+j]     &= ~maska_1[j];
        target_label->ranguvannja_df_source_minus[N_BIG*i+j]    &= ~maska_1[j];
        target_label->ranguvannja_df_source_blk[N_BIG*i+j]      &= ~maska_1[j];
        target_label->ranguvannja_df_source_reset[N_BIG*i+j]  &= ~maska_1[j];
      }
    }
    //Знімаємо всі функції для ранжування оприділювальних триґерів
    for (int i = 0; i < NUMBER_DEFINED_TRIGGERS; i++)
    {
      for (unsigned int j = 0; j < N_BIG; j++ ) 
      {
        target_label->ranguvannja_set_dt_source_plus[N_BIG*i+j]    &= ~maska_1[j];
        target_label->ranguvannja_set_dt_source_minus[N_BIG*i+j]   &= ~maska_1[j];
        target_label->ranguvannja_reset_dt_source_plus[N_BIG*i+j]  &= ~maska_1[j];
        target_label->ranguvannja_reset_dt_source_minus[N_BIG*i+j] &= ~maska_1[j];
      }
    }
  
    //Знімаємо всі функції для ранжування визначуваних "І"
    for(unsigned int i = 0; i < NUMBER_DEFINED_AND; i++)
    {
      for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_and[N_BIG*i+j] &= ~maska_1[j];
    }
  
    //Знімаємо всі функції для ранжування визначуваних "АБО"
    for(unsigned int i = 0; i < NUMBER_DEFINED_OR; i++)
    {
      for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_or[N_BIG*i+j] &= ~maska_1[j];
    }
  
    //Знімаємо всі функції для ранжування визначуваних "Викл.АБО"
    for(unsigned int i = 0; i < NUMBER_DEFINED_XOR; i++)
    {
      for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_xor[N_BIG*i+j] &= ~maska_1[j];
    }
  
    //Знімаємо всі функції для ранжування визначуваних "НЕ"
    for(unsigned int i = 0; i < NUMBER_DEFINED_NOT; i++)
    {
      for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_not[N_BIG*i+j] &= ~maska_1[j];
    }

    for (size_t i = 0; i < NUMBER_TRANSFER_FUNCTIONS; i++)
    {
      uint32_t value = (target_label->ranguvannja_tf[i] >> (16*0)) & 0xffff;
      if (
          (value == (RANG_PO_NZZ     + 1)) ||
          (value == (RANG_NZZ        + 1)) ||
          (value == (RANG_SECTOR_NZZ + 1))
         ) 
      {
        target_label->ranguvannja_tf[i] &= (uint32_t)(~(0xffff << (16*0)));
      }
    }
  }

}
/*****************************************************/

/*****************************************************
Функція обновлення змінних при зміні режиму ФК
*****************************************************/
void action_during_changing_button_mode(__SETTINGS *current_label, __SETTINGS *edit_label)
{
  //Перебираємо всі ФК
  for (size_t number = 0; number < NUMBER_DEFINED_BUTTONS; number++)
  {
    uint32_t mode_current = (current_label->buttons_mode >> number) & 0x1;
    uint32_t mode_edit    = (edit_label->buttons_mode    >> number) & 0x1;
    
    //Дії виконуємо тільки над тими кнопками, у яких режим змінився
    if (mode_current != mode_edit)
    {
      unsigned int state_button[N_SMALL];
      for (unsigned int i = 0; i < N_SMALL; i++)
      {
        state_button[i] = current_label->ranguvannja_buttons[N_SMALL*number + i];
      }
  
      for (unsigned int index_deleted_function = 0; index_deleted_function < NUMBER_TOTAL_SIGNAL_FOR_RANG_SMALL; index_deleted_function++)
      {
        if (_CHECK_SET_BIT(buttons_mode[mode_edit], index_deleted_function) == 0)
        {
          /*************************************************************/
          //Відфільтровуємо дану функцію
          /*************************************************************/
          _CLEAR_BIT(state_button, index_deleted_function);
        }
      }
    
      //Вводимо зміни у ранжування які попередньо підготували
      for (unsigned int i = 0; i < N_SMALL; i++)
      {
        current_label->ranguvannja_buttons[N_SMALL*number + i] = state_button[i];
      }
    }
  }
  
  //Активовуємо нові режими для всіх кнопок
  current_label->buttons_mode = edit_label->buttons_mode;
}
/*****************************************************/

/*****************************************************
Функція встановлення устпавок/витримок УЗ у значення "заводські"
*****************************************************/
void def_pickup_timeout_UP(__SETTINGS *current_label, uint32_t _n_UP, uint32_t group)
{
  uint32_t min = 0;
  switch (current_label->ctrl_UP_input[_n_UP])
  {
  case UP_CTRL_Ia_Ib_Ic:
  case UP_CTRL_Ia:
  case UP_CTRL_Ib:
  case UP_CTRL_Ic:
  case UP_CTRL_I1:
  case UP_CTRL_I2:
  case UP_CTRL_I04:
  case UP_CTRL_3I0_r:
    {
      min = SETPOINT_UP_I_MIN;
      break;
    }
  case UP_CTRL_3I0:
  case UP_CTRL_3I0_others:
    {
      min = SETPOINT_UP_3I0_MIN;
      break;
    }
  case UP_CTRL_Ua_Ub_Uc:
  case UP_CTRL_Uab_Ubc_Uca:
  case UP_CTRL_Ua:
  case UP_CTRL_Uab:
  case UP_CTRL_Ub:
  case UP_CTRL_Ubc:
  case UP_CTRL_Uc:
  case UP_CTRL_Uca:
  case UP_CTRL_U1:
  case UP_CTRL_U2:
  case UP_CTRL_3U0:
    {
      min = SETPOINT_UP_U_MIN;
      break;
    }
  case UP_CTRL_P:
  case UP_CTRL_Q:
  case UP_CTRL_S:
    {
      min = SETPOINT_UP_P_MIN;
      break;
    }
  default:
    {
      //Теоретично цього ніколи не мало б бути
      total_error_sw_fixed();
    }
  }
  
  current_label->setpoint_UP[_n_UP][0][group] = min;
  current_label->setpoint_UP_KP[_n_UP][0][group] = ((current_label->control_UP & MASKA_FOR_BIT(_n_UP*(_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_MORE_LESS_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I))) != 0) ? SETPOINT_UP_KP_LESS_MIN : SETPOINT_UP_KP_MORE_MAX;
  current_label->timeout_UP[_n_UP][0][group] = TIMEOUT_UP_MIN;
}
/*****************************************************/

/*****************************************************
Функція обновлення змінних при зміні аналогового входу УЗ
*****************************************************/
void action_after_changing_input_UP(__SETTINGS *current_label, uint32_t index, uint32_t value)
{
  if (current_label->ctrl_UP_input[index] != value)
  {
    current_label->ctrl_UP_input[index] = value;
    uint32_t ctrl_maska = MASKA_FOR_BIT(index*(_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_STATE_BIT     - (_CTR_UP_PART_II - _CTR_UP_PART_I)) |
                          MASKA_FOR_BIT(index*(_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_OR_AND_BIT    - (_CTR_UP_PART_II - _CTR_UP_PART_I)) |
                          MASKA_FOR_BIT(index*(_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_MORE_LESS_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I));  
    current_label->control_UP &= (uint32_t)(~ctrl_maska);
    
    for (size_t group = 0; group < NUMBER_GROUP_USTAVOK; group++) def_pickup_timeout_UP(current_label, index, group);
  }
}
/*****************************************************/

/*****************************************************
Функція обновлення коефіцієнта повернення після зміни налаштувань УЗ
*****************************************************/
void action_after_changing_ctrl_UP(__SETTINGS *current_label, uint32_t value)
{
  if (current_label->control_UP != value)
  {
    //Відбувається зміна налаштувань
    for (size_t index = 0; index < NUMBER_UP; index++)
    {
      uint32_t maska = MASKA_FOR_BIT(index*(_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_MORE_LESS_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I));
      if (((current_label->control_UP ^ value) & maska) != 0)
      {
        //Відбулася зміна ">" на "<", або "<" на ">"
        uint32_t min, max;
        if ((value & maska) != 0)
        {
          min = SETPOINT_UP_KP_LESS_MIN;
          max = SETPOINT_UP_KP_LESS_MAX;
        }
        else
        {
          min = SETPOINT_UP_KP_MORE_MIN;
          max = SETPOINT_UP_KP_MORE_MAX;
        }
      
        for (size_t group = 0; group < NUMBER_GROUP_USTAVOK; group++)
        {
          current_label->setpoint_UP_KP[index][0][group] = ((value & maska) != 0) ? min : max;
        }
      }
    }
    
    current_label->control_UP = value;
  }
}
/*****************************************************/

/*****************************************************
Функція обновлення узгодження уставок УЗ з його налаштуваннями
*****************************************************/
void change_setpoint_to_ctrl_UP(__SETTINGS * const current_label, uint32_t const value)
{
  for (size_t index = 0; index < NUMBER_UP; ++index)
  {
    uint32_t const maska = MASKA_FOR_BIT(index*(_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_MORE_LESS_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I));
      
    uint32_t min, max;
    if ((value & maska) != 0)
    {
      min = SETPOINT_UP_KP_LESS_MIN;
      max = SETPOINT_UP_KP_LESS_MAX;
    }
    else
    {
      min = SETPOINT_UP_KP_MORE_MIN;
      max = SETPOINT_UP_KP_MORE_MAX;
    }
      
    for (uint32_t *p_setpoint = current_label->setpoint_UP_KP[index][0]; p_setpoint < (current_label->setpoint_UP_KP[index][0] + NUMBER_GROUP_USTAVOK); ++p_setpoint)
    {
      if ((*p_setpoint < min) || (*p_setpoint > max))
      {
        *p_setpoint = ((value & maska) != 0) ? min : max;
      }
    }
  }
}
/*****************************************************/

/*****************************************************/
//Функція, яка визначає кількість біт затримки, який допускається між байтами у RS-485 згідно з визначеними настройками
/*****************************************************/
void calculate_namber_bit_waiting_for_rs_485(void)
{
  unsigned int number_bits_rs_485_waiting_tmp = 10;
  
  //Підраховуємо кількість біт у байті
  if (current_settings.pare_bit_RS485 > 0) number_bits_rs_485_waiting_tmp++;
  if (current_settings.number_stop_bit_RS485 > 0) number_bits_rs_485_waiting_tmp++;
  
  //Врахування швидкості
  unsigned int multiplicator;
  switch (current_settings.speed_RS485)
  {
  case 0:
    {
      multiplicator = 12; //9600
      break;
    }
  case 1:
    {
      multiplicator = 8;  //14400
      break;
    }
  case 2:
    {
      multiplicator = 6;  //19200
      break;
    }
  case 3:
    {
      multiplicator = 4;  //28800
      break;
    }
  case 4:
    {
      multiplicator = 3;  //38400
      break;
    }
  case 5:
    {
      multiplicator = 2;  //57600
      break;
    }
  default:
    {
      multiplicator = 1;  //115200
      break;
    }
  }
  number_bits_rs_485_waiting_tmp *= (multiplicator*current_settings.time_out_1_RS485);
  number_bits_rs_485_waiting_tmp /= 10;
  
  number_bits_rs_485_waiting = number_bits_rs_485_waiting_tmp;
}
/*****************************************************/

/******************************************************
//Функція, яка фіксує зміни у настройках і запускає запис настройок у EEPROM
-----------------------------------------------------
setting_rang: 0 - запис уставок
              1 - запис ранжування
              2 - ігнорувати цей параметр

source:       0 - мінімальні параметри
              1 - клавіатура
              2 - USB
              3 - RS-485
              4 - LAN
---------------------------------------------------
******************************************************/
void fix_change_settings(unsigned int setting_rang, unsigned int source)
{
  if (setting_rang < 2)
  {
    //Записуємо час останніх змін
    time_t time_dat_tmp;
    if (save_time_dat_l == 3) time_dat_tmp = time_dat_save_l;
    else
    { 
      copying_time_dat = 1;
      time_dat_tmp = time_dat_copy;
      copying_time_dat = 0;
    }

    if (/*(source != 4) && */(source != 0))
    {
      time_t *time_target = (setting_rang == 0) ? &current_settings.time_setpoints : &current_settings.time_ranguvannja;
      unsigned char *source_target = (setting_rang == 0) ? &current_settings.source_setpoints : &current_settings.source_ranguvannja;
      
      *time_target = time_dat_tmp;
      *source_target = (unsigned char)(source & 0xff);
    }
    else
    {
      current_settings.time_setpoints = current_settings.time_ranguvannja = time_dat_tmp;
      current_settings.source_setpoints = current_settings.source_ranguvannja = (unsigned char)(source & 0xff);

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
      //Помічаємо, що треба перезапустити КП
      _SET_STATE(queue_mo, STATE_QUEUE_MO_RESTART_KP);
#endif
    }
  }
  
  //Запускаємо запис у EEPROM
  _SET_BIT(control_spi1_taskes, TASK_START_WRITE_SETTINGS_EEPROM_BIT);
  
  //Помічаємо, що таблиця змінилася і її треба буде з системи захистів зкопіювати у таблицю з якою працює система захистів
  changed_settings = CHANGED_ETAP_ENDED;
  if (_CHECK_SET_BIT(active_functions, RANG_SETTINGS_CHANGED) == 0) current_settings_interfaces = current_settings;
}
/*****************************************************/

/*****************************************************/
/*
Активація внесених змін у налаштування через інтерфейси
*/
/*****************************************************/
unsigned int set_new_settings_from_interface(unsigned int source)
{
  unsigned int error = 0;
  
  //Системний час
  time_t time_dat_tmp;
  if (save_time_dat_l == 3) time_dat_tmp = time_dat_save_l;
  else
  { 
    copying_time_dat = 1;
    time_dat_tmp = time_dat_copy;
    copying_time_dat = 0;
  }
  
  if ((type_of_settings_changed & (1 << DEFAULT_SETTINGS_SET_BIT)) != 0)
  {
    current_settings_interfaces.time_setpoints = current_settings_interfaces.time_ranguvannja = time_dat_tmp;
    current_settings_interfaces.source_setpoints = current_settings_interfaces.source_ranguvannja = 0;
  }
  
  if ((type_of_settings_changed & (1 << SETTINGS_DATA_CHANGED_BIT)) != 0)
  {
    current_settings_interfaces.time_setpoints = time_dat_tmp;
    current_settings_interfaces.source_setpoints = source;
  }
  
  if ((type_of_settings_changed & (1 << RANGUVANNJA_DATA_CHANGED_BIT)) != 0)
  {
    current_settings_interfaces.time_ranguvannja = time_dat_tmp;
    current_settings_interfaces.source_ranguvannja = source;
  }
  
  unsigned int reconfiguration_RS_485 = 0, reconfiguration_RS_485_with_reset_usart = 0;
  if (
      (current_settings.speed_RS485 != current_settings_interfaces.speed_RS485) ||
      (current_settings.pare_bit_RS485 != current_settings_interfaces.pare_bit_RS485) ||
      (current_settings.number_stop_bit_RS485 != current_settings_interfaces.number_stop_bit_RS485) ||
      (current_settings.time_out_1_RS485 != current_settings_interfaces.time_out_1_RS485)
     )
  {
    //Помічаємо, що треба переконфігурувати інтерфейс RS-485
    reconfiguration_RS_485 = 1;
    
    if (
        (current_settings.speed_RS485 != current_settings_interfaces.speed_RS485) ||
        (current_settings.pare_bit_RS485 != current_settings_interfaces.pare_bit_RS485) ||
        (current_settings.number_stop_bit_RS485 != current_settings_interfaces.number_stop_bit_RS485)
       )
    {
      //Помічаємо, що треба переконфігурувати USART для інтерфейсу RS-485
      reconfiguration_RS_485_with_reset_usart = 1;
    }
  }
  
  unsigned int reload_DST_Rules = false;
  if (
      (current_settings.time_zone != current_settings_interfaces.time_zone) ||
      (current_settings.dst != current_settings_interfaces.dst) ||
      (current_settings.dst_on_rule != current_settings_interfaces.dst_on_rule) ||
      (current_settings.dst_off_rule != current_settings_interfaces.dst_off_rule)
     )
  {
    reload_DST_Rules = true;
  }

  unsigned int set_password_USB = false;
  if (
      (current_settings.password_interface_USB != current_settings_interfaces.password_interface_USB) &&
      (current_settings_interfaces.password_interface_USB != 0)  
     )   
  {
    set_password_USB = true;
  }
  
  unsigned int set_password_RS485 = false;
  if (
      (current_settings.password_interface_RS485 != current_settings_interfaces.password_interface_RS485) &&
      (current_settings_interfaces.password_interface_RS485 != 0)  
     )   
  {
    set_password_RS485 = true;
  }

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
  unsigned int set_password_LAN = false;
  if (
      (current_settings.password_interface_LAN != current_settings_interfaces.password_interface_LAN) &&
      (current_settings_interfaces.password_interface_LAN != 0)  
     )   
  {
    set_password_LAN = true;
  }
  
  unsigned int resrart_kp = false;
  uint16_t *p_source;
  {
    p_source = current_settings_interfaces.IP4;
    for (uint16_t *p_target = current_settings.IP4; p_target != (current_settings.IP4 + 4); )
    {
      if (*p_target++ != *p_source++) 
      {
        resrart_kp = true;
        break;
      }
    }
  }
  
  if (resrart_kp == false)
  {
    p_source = current_settings_interfaces.gateway;
    for (uint16_t *p_target = current_settings.gateway; p_target != (current_settings.gateway + 4); )
    {
      if (*p_target++ != *p_source++) 
      {
        resrart_kp = true;
        break;
      }
    }
  }
  
  if (resrart_kp == false)
  {
    p_source = current_settings_interfaces.IP_time_server;
    for (uint16_t *p_target = current_settings.IP_time_server; p_target != (current_settings.IP_time_server + 4); )
    {
      if (*p_target++ != *p_source++) 
      {
        resrart_kp = true;
        break;
      }
    }
  }
  
  if (
      (resrart_kp == false) 
      &&
      (  
       (current_settings.mask != current_settings_interfaces.mask) ||
       (current_settings.port_time_server != current_settings_interfaces.port_time_server) ||
       (current_settings.period_sync != current_settings_interfaces.period_sync)
      )  
     )
  {
    resrart_kp = true;
  }
#endif
	
	unsigned int set_koef_resurs = false;
	if (
			(current_settings.setpoint_Inom               != current_settings_interfaces.setpoint_Inom              ) ||
			(current_settings.setpoint_r_kom_st_Inom      != current_settings_interfaces.setpoint_r_kom_st_Inom     ) ||
			(current_settings.setpoint_Inom_vymk          != current_settings_interfaces.setpoint_Inom_vymk         ) ||
			(current_settings.setpoint_r_kom_st_Inom_vymk != current_settings_interfaces.setpoint_r_kom_st_Inom_vymk)
		 )	
	{
		set_koef_resurs = true;
	}
  
  if (error == 0)
  {
    if ((type_of_settings_changed & (1 << DEFAULT_SETTINGS_SET_BIT)) != 0)
    {
      //Переводимо меню у висхідний стан
      for(unsigned int i=0; i<MAX_LEVEL_MENU; i++)
      {
        if (
            (i == EKRAN_LEVEL_PASSWORD) || 
            (i == EKRAN_LEVEL_SET_NEW_PASSWORD1) ||
            (i == EKRAN_LEVEL_SET_NEW_PASSWORD2) ||
            (i == EKRAN_LEVEL_SET_NEW_PASSWORD3)
           )
        {
          position_in_current_level_menu[i] = INDEX_LINE_NUMBER_1_FOR_LEVEL_PASSWORD;
        }
        else  position_in_current_level_menu[i] = 0;
        previous_level_in_current_level_menu[i] = -1;
      }
  
      //Визначення початкового стану екрану
      current_ekran.current_level = EKRAN_MAIN;
      current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
      current_ekran.position_cursor_y = current_ekran.index_position;
      current_ekran.edition = 0;
      current_ekran.cursor_on = 0;
      current_ekran.cursor_blinking_on = 0;  
    }
    
    if (set_password_USB   != false) password_set_USB   = 1;
    if (set_password_RS485 != false) password_set_RS485 = 1;
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
    if (set_password_LAN != false) password_set_LAN = 1;
#endif
    
    //Помічаємо, що поля структури зараз будуть змінені
    changed_settings = CHANGED_ETAP_EXECUTION;
              
    //Копіюємо введені зміни у робочу структуру
    current_settings = current_settings_interfaces;
    if (reconfiguration_RS_485 != 0)
    {
      //Підраховуємо нову величину затримки у бітах, яка допускається між байтами у RS-485 згідно з визначеними настройками
      calculate_namber_bit_waiting_for_rs_485();
      //Виставляємо команду про переконфігурування RS-485
      if (reconfiguration_RS_485_with_reset_usart != 0) make_reconfiguration_RS_485 = 0xff;
    }

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
    if (resrart_kp)
    {
      //Помічаємо, що треба перезапустити КП
      _SET_STATE(queue_mo, STATE_QUEUE_MO_RESTART_KP);
    }
#endif    
    		
		if (set_koef_resurs != 0) make_koef_for_resurs();

    if (reload_DST_Rules)
    {
#if (__VER__ >= 8000000)
      _ForceReloadDstRules();
#endif
    }
    
    fix_change_settings(2, source);

    //Виставляємо признак, що на екрані треба обновити інформацію
    new_state_keyboard |= (1<<BIT_REWRITE);
  }
  
  return error;
}
/*****************************************************/

/******************************************************
Відновлення триґерних функцій
******************************************************/
void restore_trigger_functions(unsigned int *active_functions_point)
{
  active_functions[RANG_DT1_OUT >> 5] |= _CHECK_SET_BIT(active_functions_point, RANG_DT1_OUT);
  active_functions[RANG_DT2_OUT >> 5] |= _CHECK_SET_BIT(active_functions_point, RANG_DT2_OUT);
  active_functions[RANG_DT3_OUT >> 5] |= _CHECK_SET_BIT(active_functions_point, RANG_DT3_OUT);
  active_functions[RANG_DT4_OUT >> 5] |= _CHECK_SET_BIT(active_functions_point, RANG_DT4_OUT);
}
/*****************************************************/

/*****************************************************/
//Функція зміни інформації по діагностиці
/*****************************************************/
void changing_diagnostyka_state(void)
{
  /*****
  Визначаємо новий стан діагностики
  
  Алгоритм:
  - Копіюємо у тимчасові масиви біти, які треба скинути і які треба виставити у 
    діагностиці
  - Після цього копіювання більш пріоритетні ситстеми можуть добавляти (не 
    скидають ніколи!!!) нові біти у глобальні масиви
  - Біти , які ми будуть скидалися/встановлюватися очищаємо з глобальних масивів
    (якщо в процесі цього алгоритму більшпріоритетні системи добавили чи 
    добавлятимуть нові біти, то вони не скинуться, а опрацюються при наступному 
    виклику цієї функції)

  - Спочатку очищаємо біти а потім встановлюємо, бо фіксація події має більший 
    пріоритет за очищення
  *****/
  unsigned int clear_diagnostyka_tmp[N_DIAGN], set_diagnostyka_tmp[N_DIAGN];
  
  for (size_t i = 0; i < N_DIAGN; i++)
  {
    clear_diagnostyka_tmp[i] = clear_diagnostyka[i];
    set_diagnostyka_tmp[i] = set_diagnostyka[i];
  }
    
  for (size_t i = 0; i < N_DIAGN; i++)
  {
    diagnostyka[i] &= (unsigned int)(~clear_diagnostyka_tmp[i]); 
    diagnostyka[i] |= set_diagnostyka_tmp[i]; 
  }

  for (size_t i = 0; i < N_DIAGN; i++)
  {
    clear_diagnostyka[i] &= (unsigned int)(~clear_diagnostyka_tmp[i]);
    set_diagnostyka[i] &= (unsigned int)(~set_diagnostyka_tmp[i]);
  }
  /*****/
  
  //Визначаємо, чи відбулися зміни
  unsigned int value_changes[N_DIAGN], diagnostyka_now[N_DIAGN];
  /*
  Робимо копію тепершньої діагностики, бо ця функція працює на найнижчому пріоритеті,
  тому під час роботи може появитися нові значення, які ми не врахували у цій функції
  */
  for (size_t i = 0; i < N_DIAGN; i++)
  {
    diagnostyka_now[i] = diagnostyka[i];
    value_changes[i] = diagnostyka_before[i] ^ diagnostyka_now[i];
  }
  
  /*
  У реєстраторі програмних подій має реєструватися тільки перехід з пасивного стану у активний
  таких подій як " Старт устр.    " і " Рестарт устр.  "
  тому перехід з активного у пачсивний ми ігноруємо і крім того затираємо біти, які його "засигналізували"
  */
  
  /*****/
  //Подія " Старт устр.    "
  /*****/
  if (_CHECK_SET_BIT(value_changes, EVENT_START_SYSTEM_BIT) != 0)
  {
    //Зафіксовано що подія " Старт устр.    " змінила свій стан
    if (_CHECK_SET_BIT(diagnostyka_now, EVENT_START_SYSTEM_BIT) == 0)
    {
      /*
      Новий стан події " Старт устр.    " є неактивний стан
      Тому робимо так, щоб ця подія не попала у реєстратор програмних подій таким операціями
      - знімаємо встановлений біт про зміну стану діагностики
      - знімаємо повідомлення, що у попередньому стані діагностики ця подія була активною
      - у текучому стані діагностики нічого міняти не треба, бо цей сигнал є неактивним
      */
      _CLEAR_BIT(value_changes, EVENT_START_SYSTEM_BIT);
      _CLEAR_BIT(diagnostyka_before, EVENT_START_SYSTEM_BIT);
    }
  }
  /*****/
  
  /*****/
  //Подія " Рестарт устр.  "
  /*****/
  if (_CHECK_SET_BIT(value_changes, EVENT_RESTART_SYSTEM_BIT) != 0)
  {
    //Зафіксовано що подія " Рестарт устр.  " змінила свій стан
    if (_CHECK_SET_BIT(diagnostyka_now, EVENT_RESTART_SYSTEM_BIT) == 0)
    {
      /*
      Новий стан події " Рестарт устр.  " є неактивний стан
      Тому робимо так, щоб ця подія не попала у реєстратор програмних подій таким операціями
      - знімаємо встановлений біт про зміну стану діагностики
      - знімаємо повідомлення, що у попередньому стані діагностики ця подія була активною
      - у текучому стані діагностики нічого міняти не треба, бо цей сигнал є неактивним
      */
      _CLEAR_BIT(value_changes, EVENT_RESTART_SYSTEM_BIT);
      _CLEAR_BIT(diagnostyka_before, EVENT_RESTART_SYSTEM_BIT);
    }
  }
  /*****/

  /*****/
  //Подія "Пр.Рестарт пр."
  /*****/
  if (_CHECK_SET_BIT(value_changes, EVENT_SOFT_RESTART_SYSTEM_BIT) != 0)
  {
    //Зафіксовано що подія "Пр.Рестарт пр." змінила свій стан
    if (_CHECK_SET_BIT(diagnostyka_now, EVENT_SOFT_RESTART_SYSTEM_BIT) == 0)
    {
      /*
      Новий стан події "Пр.Рестарт пр." є неактивний стан
      Тому робимо так, щоб ця подія не попала у реєстратор програмних подій таким операціями
      - знімаємо встановлений біт про зміну стану діагностики
      - знімаємо повідомлення, що у попередньому стані діагностики ця подія була активною
      - у текучому стані діагностики нічого міняти не треба, бо цей сигнал є неактивним
      */
      _CLEAR_BIT(value_changes, EVENT_SOFT_RESTART_SYSTEM_BIT);
      _CLEAR_BIT(diagnostyka_before, EVENT_SOFT_RESTART_SYSTEM_BIT);
    }
  }
  /*****/

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
  /*****/
  //Подія "Рестарт КП"
  /*****/
  if (_CHECK_SET_BIT(value_changes, EVENT_RESTART_CB_BIT) != 0)
  {
    //Зафіксовано що подія "Пр.Рестарт пр." змінила свій стан
    if (_CHECK_SET_BIT(diagnostyka_now, EVENT_RESTART_CB_BIT) == 0)
    {
      /*
      Новий стан події "Рестарт КП" є неактивний стан
      Тому робимо так, щоб ця подія не попала у реєстратор програмних подій таким операціями
      - знімаємо встановлений біт про зміну стану діагностики
      - знімаємо повідомлення, що у попередньому стані діагностики ця подія була активною
      - у текучому стані діагностики нічого міняти не треба, бо цей сигнал є неактивним
      */
      _CLEAR_BIT(value_changes, EVENT_RESTART_CB_BIT);
      _CLEAR_BIT(diagnostyka_before, EVENT_RESTART_CB_BIT);
    }
  }
  /*****/
#endif

  //Перевіряємо, чи треба виконувати дії поо зміні діагностики
  unsigned int not_null = false;
  for (size_t i = 0; i < N_DIAGN; i++) 
  {
    not_null |= (value_changes[i] != 0);
    if (not_null) break;
  }
  
  if (not_null)
  {
    //Є біти, які треба встановити, або зняти
    
    /*****/
    //При можливості формуємо запис у реєстратор програмних помилок
    /*****/
    if (_CHECK_SET_BIT(diagnostyka, ERROR_PR_ERR_OVERLOAD_BIT) == 0)
    {
      /*
      Новий запис робимо тільки тоді, коли попередньо не було зафіксовано 
      переповнення буферу, інакше чикаємо, поки запис у реєстратор програмних подій
      знімить подію про переповнення буферу
      */
      
      //Визначаємо кількість доступних комірок у буфері для реєстратора програмних подій
      int number_empty_cells;
      unsigned int head = head_fifo_buffer_pr_err_records, tail = tail_fifo_buffer_pr_err_records;
      number_empty_cells = (int)(((unsigned int)tail) - ((unsigned int)head));
      while (number_empty_cells <= 0) number_empty_cells += MAX_NUMBER_RECORDS_PR_ERR_INTO_BUFFER;
      if (number_empty_cells == 1)
      {
        //Це є остання вільна комірка, то помічаємо, що з цим записом відбувається повне заповнення буферу
        _SET_BIT(diagnostyka, ERROR_PR_ERR_OVERLOAD_BIT);
        /*
        відбулася зміна стану діагностики, яку треба врахувати у даному записфі ,тому
        робимо повторноу копію тепершньої діагностики, яка може врахувати і ті зміни які відбулися
        між операціями копіювання стану діагностики на початку цієї функції і
        операцією, як зараз ми будемо виконувати
        */
        _SET_BIT(diagnostyka_now, ERROR_PR_ERR_OVERLOAD_BIT);
        
        //Підраховуємо нову кількість змін в діагностиці
        value_changes[ERROR_PR_ERR_OVERLOAD_BIT >> 5] = diagnostyka_before[ERROR_PR_ERR_OVERLOAD_BIT >> 5] ^ diagnostyka_now[ERROR_PR_ERR_OVERLOAD_BIT >> 5];
      }

      //Вираховуємо кількість змін сигналів
      unsigned int number_changes = 0;
      for(unsigned int i = 0; i < (8*sizeof(value_changes)); i++)
      {
        if (_CHECK_SET_BIT(value_changes, i) != 0) number_changes++;
      }

      if(number_changes != 0)
      {
        /*
        теоретично може бути ситуація, що ми знімали повідомлення про переповнення буферу
        FIFO для записів реєстратора програмних подій  і це була єдина зміна, али при цьому
        є тільки одна вільна комірка, тому ми знову виставили повідомлення про переповнення.
        Тому ми не можемо зняти це повідомлення - тому і не можемо робити нового запису, 
        бо фактично і зміни ніякої нема
        */
        
        //Визначаємо індекс у масиві буферу програмних помилок з якого трбе почати заповнювати дані
        unsigned int index_into_buffer_pr_err = head*SIZE_ONE_RECORD_PR_ERR;
      
        //Помічаємо мітку початку запису
        buffer_pr_err_records[index_into_buffer_pr_err + 0] = LABEL_START_RECORD_PR_ERR;

        //Час фіксації зміни у діагностиці
        if(
           (_CHECK_SET_BIT(diagnostyka, EVENT_START_SYSTEM_BIT       ) == 0) &&
           (_CHECK_SET_BIT(diagnostyka, EVENT_RESTART_SYSTEM_BIT     ) == 0) &&
           (_CHECK_SET_BIT(diagnostyka, EVENT_SOFT_RESTART_SYSTEM_BIT) == 0) &&
           (_CHECK_SET_BIT(diagnostyka, EVENT_STOP_SYSTEM_BIT        ) == 0)
          )
        {
          //Вже відбулося перше зчитуванння часу - тобто системний час у нас є
          copying_time_dat = 1;
          time_t time_dat_tmp = time_dat_copy;
          int32_t time_ms_tmp = time_ms_copy;
          copying_time_dat = 0;
          for(size_t i = 0; i < sizeof(time_t); i++)  buffer_pr_err_records[index_into_buffer_pr_err + 1 + i] = *((unsigned char*)(&time_dat_tmp) + i);
          for(size_t i = 0; i < sizeof(int32_t); i++)  buffer_pr_err_records[index_into_buffer_pr_err + 1 + sizeof(time_t) + i] = *((unsigned char*)(&time_ms_tmp) + i);
        }
        else
        {
          //Ще не відбулося перше зчитуванння часу - тому покищо ці поля записуємо числом 0xff, а потім, коли системний час зчитається, то ми це поле обновимо
          for(size_t i = 0; i < sizeof(time_t); i++)  buffer_pr_err_records[index_into_buffer_pr_err + 1 + i] = 0;
          for(size_t i = 0; i < sizeof(int32_t); i++)  buffer_pr_err_records[index_into_buffer_pr_err + 1 + sizeof(time_t) + i] = 0;
        }

        buffer_pr_err_records[index_into_buffer_pr_err + 13] = number_changes & 0xff;
      
        for (size_t i = 0; i < N_DIAGN_BYTES; i++)
        {
          unsigned int n_word = i >> 2;
          unsigned int shift = 8*(i & 0x3);
          
          //Записуємо попередній стан діагностики
          buffer_pr_err_records[index_into_buffer_pr_err + 14 + i] =  (diagnostyka_before[n_word] >> shift) & 0xff;

          //Записуємо теперішній стан діагностики
          buffer_pr_err_records[index_into_buffer_pr_err + 14 + N_DIAGN_BYTES + i] =  (diagnostyka_now[n_word] >> shift) & 0xff;
        }
        
        /*
        У реєстраторі програмних подій має реєструватися тільки перехід з пасивного стану у активний
        таких події як " Останов.устр.  " і ця подія не має бути активною у текучій діагностиці,
        бо це, фактично, подія, яка мала місце до останнього включення приладу в роботу.
        Тобто це подія "попереднього сеансу роботи"
        тому перехід з пасивного у фіксуємо ми фіксуємо і зразу затираємо цей біт у теперішній діагностіці,
        яка після цього буде зкомійомана у масив. який відповідає за попередній стан діагностики, так і у масив
        діагностики, який відповідає за текучий стан подій діагностики
        */
        if (_CHECK_SET_BIT(value_changes, EVENT_STOP_SYSTEM_BIT) != 0)
        {
          /*
          Cтан події " Останов.устр.  " змінився (допускається нашим програмним забезпеченням
          зміна з пасивного стану у активний стан)
          Тому робимо такі операції
          - знімаємо встановлений біт у масиві, який зараз буде копіюватися у масив попереднього стану
          - знімаємо встановлений біт у масиві, який відповідає за текучий стан подій діагностики
          */
          _CLEAR_BIT(diagnostyka_now, EVENT_STOP_SYSTEM_BIT);
          _CLEAR_BIT(diagnostyka, EVENT_STOP_SYSTEM_BIT);
        }

        //Фіксуємо попередній стан, який ми вже записали і відносно якого будемо визначати нові зміни
        for (size_t i = 0; i < N_DIAGN; i ++) diagnostyka_before[i] = diagnostyka_now[i];

        //Підготовлюємося до запуску запису у реєстратор програмних подій
          unsigned int next_index_into_fifo_buffer = head + 1;
          while(next_index_into_fifo_buffer >= MAX_NUMBER_RECORDS_PR_ERR_INTO_BUFFER) next_index_into_fifo_buffer -= MAX_NUMBER_RECORDS_PR_ERR_INTO_BUFFER;
          
          //Встановлюємо нове значення голови буфера FIFO 
          head_fifo_buffer_pr_err_records = next_index_into_fifo_buffer;
      }
    }
    /*****/
  }
}
/*****************************************************/

/*****************************************************/
//Підрахунок кількості втановлених одиничок у області пам'яті початок якої визначається вказівником source
/*****************************************************/
unsigned int count_number_set_bit(unsigned int* source, unsigned int total_number)
{
  unsigned int count = 0;
  unsigned int offset, shift;
  
  for (unsigned int i = 0; i < total_number; i++)  
  {
    offset = i >> 5; //Це є, фактично, ділення на 32
    shift  = (i & (32 - 1)); //Це є, фактично, визначення остачі від ділення на 32
    if ( ( (*(source + offset))  & ((unsigned int)(1 << shift)) ) != 0 ) count++;
  }

  return count;
}
/*****************************************************/

/*****************************************************/
//Контроль достовірності настройок
/*****************************************************/
void control_settings(void)
{
  unsigned char crc_settings_tmp = 0, temp_value_1, temp_value_2;
  unsigned char  *point_1 = (unsigned char*)(&current_settings); 
  unsigned char  *point_2 = (unsigned char*)(&current_settings_prt); 
  unsigned int i = 0, difference = 0;
  while ((difference == 0) && (i < sizeof(__SETTINGS)))
  {
    temp_value_1 = *(point_1);
    temp_value_2 = *(point_2);
    crc_settings_tmp += temp_value_1;
    if (temp_value_1 != temp_value_2) difference = 0xff;
    point_1++;
    point_2++;
    i++;
  }
  
  if (difference == 0)
  {
    for (size_t k = 0; ((difference == 0) && (k < NUMBER_INPUTS)); ++k ) 
    {
      difference |= (dopusk_dv_prt[k] != current_settings_prt.dopusk_dv[k]);
    }
  }
  
  if (
			(difference == 0) &&
			(crc_settings == crc_settings_tmp) &&
			(settings_prt_Ib_I04 == (current_settings.control_extra_settings_1 & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_IB_I04))) &&
			(T0_prt == current_settings.T0)  &&
			(TCurrent_prt == current_settings.TCurrent) &&
			(type_of_input_prt == current_settings.type_of_input) &&
			(type_of_input_signal_prt == current_settings.type_of_input_signal)
		)
  {
    //Контроль достовірності таблиці настройок пройшов успішно
    
    //Скидаємо повідомлення у слові діагностики
    _SET_BIT(clear_diagnostyka, ERROR_SETTINGS_EEPROM_CONTROL_BIT);
  }
  else
  {
    //Контроль достовірності таблиці настройок не пройшов

    //Виствляємо повідомлення у слові діагностики
    _SET_BIT(set_diagnostyka, ERROR_SETTINGS_EEPROM_CONTROL_BIT);
  }
}
/*****************************************************/

/*****************************************************/
//Контроль достовірності юстування
/*****************************************************/
void control_ustuvannja(void)
{
  unsigned char crc_ustuvannja_tmp = 0, temp_value_1, temp_value_2;
  unsigned char  *point_1; 
  unsigned char  *point_2; 
  unsigned int i = 0, difference = 0;

  /*
  У контрольу суму входить індетифікатор типу набору юстуючих коефіцієнтів,
  який займає розмір типу unsigned int
  */
  unsigned int adjustment_id_tmp = ADJUSTMENT_ID;
  point_1 = (unsigned char*)(&adjustment_id_tmp); 
  while (i < sizeof(unsigned int))
  {
    temp_value_1 = *(point_1);
    crc_ustuvannja_tmp += temp_value_1;
    point_1++;
    i++;
  }
  
  point_1 = (unsigned char*)(&ustuvannja); 
  point_2 = (unsigned char*)(&ustuvannja_meas);
  i = 0;
  while ((difference == 0) && (i < sizeof(ustuvannja)))
  {
    temp_value_1 = *(point_1);
    temp_value_2 = *(point_2);
    crc_ustuvannja_tmp += temp_value_1;
    if (temp_value_1 != temp_value_2) difference = 0xff;
    point_1++;
    point_2++;
    i++;
  }

  point_1 = (unsigned char*)(&phi_ustuvannja); 
  point_2 = (unsigned char*)(&phi_ustuvannja_meas);
  i = 0;
  while ((difference == 0) && (i < sizeof(phi_ustuvannja)))
  {
    temp_value_1 = *(point_1);
    temp_value_2 = *(point_2);
    crc_ustuvannja_tmp += temp_value_1;
    if (temp_value_1 != temp_value_2) difference = 0xff;
    point_1++;
    point_2++;
    i++;
  }

  point_1 = (unsigned char*)(&phi_ustuvannja_sin_cos); 
  point_2 = (unsigned char*)(&phi_ustuvannja_sin_cos_meas);
  i = 0;
  while ((difference == 0) && (i < sizeof(phi_ustuvannja_sin_cos)))
  {
    temp_value_1 = *(point_1);
    temp_value_2 = *(point_2);
    crc_ustuvannja_tmp += temp_value_1;
    if (temp_value_1 != temp_value_2) difference = 0xff;
    point_1++;
    point_2++;
    i++;
  }

  /*
  Не треба контролювати ідентичність серійног номеру, який працює з вимірювально системою,
  чи системою захистів, чи ще якоюсь високопріоритетною і високонадійною від завад системою,
  бо його просто не існує
  */
  point_1 = (unsigned char*)(&serial_number_dev); 
  i = 0;
  while ((difference == 0) && (i < sizeof(serial_number_dev)))
  {
    temp_value_1 = *(point_1);
    crc_ustuvannja_tmp += temp_value_1;
    point_1++;
    i++;
  }
  
  if ((difference == 0) && (crc_ustuvannja == crc_ustuvannja_tmp))
  {
    //Контроль достовірності юстування пройшов успішно
    
    //Скидаємо повідомлення у слові діагностики
    _SET_BIT(clear_diagnostyka, ERROR_USTUVANNJA_EEPROM_CONTROL_BIT);
  }
  else
  {
    //Контроль достовірності юстування не пройшов

    //Виствляємо повідомлення у слові діагностики
    _SET_BIT(set_diagnostyka, ERROR_USTUVANNJA_EEPROM_CONTROL_BIT);
  }
}
/*****************************************************/

/*****************************************************/
//Контроль достовірності триґерної інформації
/*****************************************************/
void control_trg_func(void)
{
  unsigned char crc_trg_func_tmp = 0, temp_value_1;
  unsigned char  *point; 
  unsigned int i;
  
  point = (unsigned char*)(&fix_active_buttons_ctrl);
  i = 0;  
  while (i < sizeof(fix_active_buttons_ctrl))
  {
    temp_value_1 = *(point);
    crc_trg_func_tmp += temp_value_1;
    point++;
    i++;
  }
  
  point = (unsigned char*)(trigger_active_functions_ctrl);
  i = 0;  
  while (i < sizeof(trigger_active_functions_ctrl))
  {
    temp_value_1 = *(point);
    crc_trg_func_tmp += temp_value_1;
    point++;
    i++;
  }
  
  if (crc_trg_func_ctrl == crc_trg_func_tmp)
  {
    //Контроль достовірності юстування пройшов успішно
    
    //Скидаємо повідомлення у слові діагностики
    _SET_BIT(clear_diagnostyka, ERROR_TRG_FUNC_EEPROM_CONTROL_BIT);
  }
  else
  {
    //Контроль достовірності юстування не пройшов

    //Виствляємо повідомлення у слові діагностики
    _SET_BIT(set_diagnostyka, ERROR_TRG_FUNC_EEPROM_CONTROL_BIT);
  }
}
/*****************************************************/

/*****************************************************/
//Контроль достовірності інформації по реєстраторах
/*****************************************************/
unsigned int control_info_rejestrator(__INFO_REJESTRATOR* info_rejestrator_point, unsigned char crc_info_rejestrator)
{
  unsigned int result;
  unsigned char crc_info_rejestrator_tmp = 0, temp_value_1;
  unsigned char  *point = (unsigned char*)(info_rejestrator_point); 
  unsigned int i = 0;
  while (i < sizeof(__INFO_REJESTRATOR))
  {
    temp_value_1 = *(point);
    crc_info_rejestrator_tmp += temp_value_1;
    point++;
    i++;
  }
  
  if (crc_info_rejestrator == crc_info_rejestrator_tmp)
  {
    //Контроль достовірності реєстратора пройшов успішно
    result = 1;    
  }
  else
  {
    //Контроль достовірності реєстратора не пройшов
    result = 0;    
  }
  
  return result;
}
/*****************************************************/

/*****************************************************/
//Контроль достовірності інформації по аналоговому реєстратору
/*****************************************************/
unsigned int control_info_ar_rejestrator(__INFO_AR_REJESTRATOR* info_rejestrator_point, unsigned char crc_info_rejestrator)
{
  unsigned int result;
  unsigned char crc_info_rejestrator_tmp = 0, temp_value_1;
  unsigned char  *point = (unsigned char*)(info_rejestrator_point); 
  unsigned int i = 0;
  while (i < sizeof(__INFO_AR_REJESTRATOR))
  {
    temp_value_1 = *(point);
    crc_info_rejestrator_tmp += temp_value_1;
    point++;
    i++;
  }
  
  if (crc_info_rejestrator == crc_info_rejestrator_tmp)
  {
    //Контроль достовірності реєстратора пройшов успішно
    result = 1;    
  }
  else
  {
    //Контроль достовірності реєстратора не пройшов
    result = 0;    
  }
  
  return result;
}
/*****************************************************/

/*****************************************************/
//Контроль достовірності лічильника ресурсу
/*****************************************************/
void control_resurs(void)
{
  unsigned char crc_resurs_tmp = 0, temp_value;
  unsigned char *point;
  
  point = (unsigned char*)(&resurs_vymykacha_ctrl); 
  for (unsigned int i = 0; i < sizeof(resurs_vymykacha_ctrl); i++)
  {
    temp_value = *(point);
    crc_resurs_tmp += temp_value;
    point++;
  }

  point = (unsigned char*)(&resurs_vidkljuchennja_ctrl); 
  for (unsigned int i = 0; i < sizeof(resurs_vidkljuchennja_ctrl); i++)
  {
    temp_value = *(point);
    crc_resurs_tmp += temp_value;
    point++;
  }

  if (crc_resurs_ctrl == crc_resurs_tmp)
  {
    //Контроль достовірності ресурсу лічильника пройшов успішно
    
    //Скидаємо повідомлення у слові діагностики
    _SET_BIT(clear_diagnostyka, ERROR_RESURS_EEPROM_CONTROL_BIT);
  }
  else
  {
    //Контроль достовірності юстування не пройшов

    //Виствляємо повідомлення у слові діагностики
    _SET_BIT(set_diagnostyka, ERROR_RESURS_EEPROM_CONTROL_BIT);
  }
  
}
/*****************************************************/

/*****************************************************/
//Визначення номеру місяця з константи __DATE__
/*****************************************************/
int str_to_int_DATE_Mmm(void)
{
  static const char month_names[12*3] = "JanFebMarAprMayJunJulAugSepOctNovDec";
  
  int i = 0;
  while(i < 12)
  {
    if (
        (month_names[3*i + 0] == __DATE__[0]) &&
        (month_names[3*i + 1] == __DATE__[1]) &&
        (month_names[3*i + 2] == __DATE__[2])
       )
    {
      break;
    }
    
    i++;
  } 
  
  if (i < 12)
  { 
    i += 1; /*щоб нумерація місяців ішла з "1", а не з "0"*/
    return ((i / 10) << 4) + (i % 10);
  }
  else return 0;
}
/*****************************************************/

/*****************************************************/
//Функція обновлення змінних при зміні типу реле 33/3I0
/*****************************************************/
void action_after_changing_resurs_pickup(__SETTINGS * const target_label)
{
  unsigned int const chastka = target_label->setpoint_r_kom_st_Inom/target_label->setpoint_r_kom_st_Inom_vymk;
  if (check_data_setpoint(target_label->setpoint_pochatkovyj_resurs, (2*chastka), target_label->setpoint_r_kom_st_Inom) != 1)
  {
    target_label->setpoint_pochatkovyj_resurs = 2*chastka;
  }

  if (check_data_setpoint(target_label->setpoint_krytychnyj_resurs, chastka, (2*chastka)) != 1)
  {
    target_label->setpoint_krytychnyj_resurs = chastka;
  }
}
/*****************************************************/

/*****************************************************/
/*****************************************************/
//
/*****************************************************/
