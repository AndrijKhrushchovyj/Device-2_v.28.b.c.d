
#include "header.h"

//начальный регистр в карте памяти
#define BEGIN_ADR_REGISTER 10000
//начальный bit в карте памяти
#define BEGIN_ADR_BIT 55000

#ifdef  MODYFIKACIA_VERSII_DS
//конечный регистр в карте памяти
#define END_ADR_REGISTER 10029
//конечный bit в карте памяти
#define END_ADR_BIT 55464

#else
//конечный регистр в карте памяти
#define END_ADR_REGISTER 10028
//конечный bit в карте памяти
#define END_ADR_BIT 55448

#define DS_BIT_CONFIGURATION 0
#endif

#define CLRVALID_DATA  0

int privateUPRBigGetReg2(int adrReg);
int privateUPRBigGetBit2(int adrReg);

int getUPRBigModbusRegister(int);//получить содержимое регистра
int getUPRBigModbusBit(int);//получить содержимое бита
int setUPRBigModbusRegister(int, int);//получить содержимое регистра
int setUPRBigModbusBit(int, int);//получить содержимое бита

int  postUPRBigWriteAction(void);//action после записи

int  uprFunc000(int action, int inOffset, uint32_t *uprMaska, int validData, uint32_t **editControl);
void uprFuncRead000(int inOffset, uint32_t *uprMaska, uint32_t **editControl);
int  uprFuncValidWrite000(int action, int inOffset, uint32_t *uprMaska, uint32_t **editControl);
int  uprFuncValid000(int inOffset, int validData);
int isValidCONFCondition(unsigned int confMaska);
int isValidEXTRACondition(unsigned int extraMaska);
int isValidZZTYPECondition(unsigned int extraMaska);
int specialСheckUPRBigWriteAction(int offset);
int specialСheckLinU(void);

void loadUPRBigActualDataBit(int beginOffset, int endOffset);

SRAM1 COMPONENT_OBJ *uprbigcomponent;

void uprFuncRead000(int inOffset, uint32_t *uprMaska, uint32_t **editControl)
{
  uprFunc000(0, inOffset, uprMaska, CLRVALID_DATA, editControl);
}//uprFuncRead000(int inOffset, uint32_t *uprMaska)
int uprFuncValidWrite000(int action, int inOffset, uint32_t *uprMaska, uint32_t **editControl)
{
  return uprFunc000(action, inOffset, uprMaska, CLRVALID_DATA, editControl);
}//uprFuncRead000(int inOffset, uint32_t *uprMaska)
int uprFuncValid000(int inOffset, int validData)
{
  uint32_t uprMaska=0;
  uint32_t *editControl=NULL;
  return uprFunc000(0, inOffset, &uprMaska, validData, &editControl);
}//uprFuncRead000(int inOffset, uint32_t *uprMaska)

int uprFunc000(int actControl, int inOffset, uint32_t *uprMaska, int validData, uint32_t **editControl)
{
  int isValid = 1;
  (*uprMaska) = 0xFFFFFFFF;
  switch(inOffset)
    {
//  int count_bit = 2;
    case 0:
      (*uprMaska)   = MTZ_BIT_CONFIGURATION;
      (*editControl) = &edition_settings.configuration;
      if(actControl)
        {
         //Функція обновлення змінних при зміні конфігурації
         if(action_after_changing_of_configuration(edition_settings.configuration, &edition_settings)) isValid = 0;
        }//if(actControl)
      break;
#define MTZ_CONFIGURATION_BEGIN 1
    case MTZ_CONFIGURATION_BEGIN:
      (*uprMaska)   = N_BIT_CTRMTZ_1;
      break;
    case 2:
      (*uprMaska)   = N_BIT_CTRMTZ_1_VPERED;
      break;
    case 3:
      (*uprMaska)   = N_BIT_CTRMTZ_1_NAZAD;
      break;
    case 4:
      (*uprMaska)   = N_BIT_CTRMTZ_1_ZNAM;
      if(isValidCONFCondition(ZNAM_BIT_CONFIGURATION)) isValid = 0;
      break;
    case 5:
      (*uprMaska)   = N_BIT_CTRMTZ_1_ZNAM_TYPE;
      if(isValidCONFCondition(ZNAM_BIT_CONFIGURATION)) isValid = 0;
      break;
    case 6:
      (*uprMaska)   = N_BIT_CTRMTZ_2;
      break;
    case 7:
      (*uprMaska)   = N_BIT_CTRMTZ_2_VPERED;
      break;
    case 8:
      (*uprMaska)   = N_BIT_CTRMTZ_2_NAZAD;
      break;
    case 9:
      (*uprMaska)   = N_BIT_CTRMTZ_2_PRYSKORENNJA;
      break;
    case 10:
      (*uprMaska)   = N_BIT_CTRMTZ_2_PRYSKORENA;
      break;
    case 11:
      (*uprMaska)   = N_BIT_CTRMTZ_2_ZNAM;
      if(isValidCONFCondition(ZNAM_BIT_CONFIGURATION)) isValid = 0;
      break;
    case 12:
      (*uprMaska)   = N_BIT_CTRMTZ_2_ZNAM_TYPE;
      if(isValidCONFCondition(ZNAM_BIT_CONFIGURATION)) isValid = 0;
      break;
    case 13:
      (*uprMaska)   = N_BIT_CTRMTZ_3;
      break;
    case 14:
      (*uprMaska)   = N_BIT_CTRMTZ_3_VPERED;
      break;
    case 15:
      (*uprMaska)   = N_BIT_CTRMTZ_3_NAZAD;
      break;

//  count_bit = 3;
    case 18:
      (*uprMaska)   = N_BIT_CTRMTZ_3_ZNAM;
      if(isValidCONFCondition(ZNAM_BIT_CONFIGURATION)) isValid = 0;
      break;
    case 19:
      (*uprMaska)   = N_BIT_CTRMTZ_3_ZNAM_TYPE;
      if(isValidCONFCondition(ZNAM_BIT_CONFIGURATION)) isValid = 0;
      break;
    case 20:
      (*uprMaska)   = N_BIT_CTRMTZ_4;
      break;
    case 21:
      (*uprMaska)   = N_BIT_CTRMTZ_4_VPERED;
      break;
    case 22:
      (*uprMaska)   = N_BIT_CTRMTZ_4_NAZAD;
      break;
    case 23:
      (*uprMaska)   = N_BIT_CTRMTZ_4_ZNAM;
      if(isValidCONFCondition(ZNAM_BIT_CONFIGURATION)) isValid = 0;
      break;
    case 24:
      (*uprMaska)   = N_BIT_CTRMTZ_4_ZNAM_TYPE;
      if(isValidCONFCondition(ZNAM_BIT_CONFIGURATION)) isValid = 0;
      break;
#define MTZ_CONFIGURATION_END 25
    case MTZ_CONFIGURATION_END:
      (*uprMaska)   = N_BIT_CTRMTZ_NESPR_KIL_NAPR;
      break;

//  count_bit = 3;
    case 32:
      (*uprMaska)   = MTZ04_BIT_CONFIGURATION;
      (*editControl) = &edition_settings.configuration;
      if(actControl)
        {
          if(validData)if(!(edition_settings.control_extra_settings_1&(1<<INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_IB_I04))) isValid = 0;
          //Функція обновлення змінних при зміні конфігурації
          if(action_after_changing_of_configuration(edition_settings.configuration, &edition_settings)) isValid = 0;
        }//if(actControl)
      break;
#define MTZ04_CONFIGURATION_BEGIN 33
    case 33:
      (*uprMaska)   = N_BIT_CTRMTZ04_1;
      break;
    case 34:
      (*uprMaska)   = N_BIT_CTRMTZ04_2;
      break;
    case 35:
      (*uprMaska)   = N_BIT_CTRMTZ04_2_PRYSKORENNJA;
      break;
#define MTZ04_CONFIGURATION_END 36
    case 36:
      (*uprMaska)   = N_BIT_CTRMTZ04_2_PRYSKORENA;
      break;

//  count_bit = 3;
    case 48:
      (*uprMaska)   = UP_BIT_CONFIGURATION;
      (*editControl) = &edition_settings.configuration;
      if(actControl)
        {
         //Функція обновлення змінних при зміні конфігурації
         if(action_after_changing_of_configuration(edition_settings.configuration, &edition_settings)) isValid = 0;
        }//if(actControl)
      break;

//IF ВСТАВКА 49-72

//  count_bit = 3;
    case 80:
      (*uprMaska)   = ZNAM_BIT_CONFIGURATION;
      (*editControl) = &edition_settings.configuration;
      if(actControl)
        {
         //Функція обновлення змінних при зміні конфігурації
         if(action_after_changing_of_configuration(edition_settings.configuration, &edition_settings)) isValid = 0;
        }//if(actControl)
      break;
#define ZNAM_CONFIGURATION_BEGIN 81
#define ZNAM_CONFIGURATION_END ZNAM_CONFIGURATION_BEGIN
    case ZNAM_CONFIGURATION_BEGIN:
      (*uprMaska)   = CTR_ZNAM_STATE_BIT;
      break;

//  count_bit = 3;
    case 96:
      (*uprMaska)   = ZDZ_BIT_CONFIGURATION;
      (*editControl) = &edition_settings.configuration;
      if(actControl)
        {
         //Функція обновлення змінних при зміні конфігурації
         if(action_after_changing_of_configuration(edition_settings.configuration, &edition_settings)) isValid = 0;
        }//if(actControl)
      break;
#define ZDZ_CONFIGURATION_BEGIN 97
    case ZDZ_CONFIGURATION_BEGIN:
      (*uprMaska)   = CTR_ZDZ_STATE_BIT;
      break;
    case 98:
      (*uprMaska)   = (CTR_ZDZ_STARTED_FROM_MTZ1_BIT - (_CTR_ZDZ_PART_III - _CTR_ZDZ_PART_II));
      if(isValidCONFCondition(MTZ_BIT_CONFIGURATION)) isValid = 0;
      break;
    case 99:
      (*uprMaska)   = (CTR_ZDZ_STARTED_FROM_MTZ2_BIT - (_CTR_ZDZ_PART_III - _CTR_ZDZ_PART_II));//CTR_ZDZ_STARTED_FROM_MTZ2_BIT;
      if(isValidCONFCondition(MTZ_BIT_CONFIGURATION)) isValid = 0;
      break;
    case 100:
      (*uprMaska)   = (CTR_ZDZ_STARTED_FROM_MTZ3_BIT - (_CTR_ZDZ_PART_III - _CTR_ZDZ_PART_II));//CTR_ZDZ_STARTED_FROM_MTZ3_BIT;
      if(isValidCONFCondition(MTZ_BIT_CONFIGURATION)) isValid = 0;
      break;
    case 101:
      (*uprMaska)   = (CTR_ZDZ_STARTED_FROM_MTZ4_BIT - (_CTR_ZDZ_PART_III - _CTR_ZDZ_PART_II));//CTR_ZDZ_STARTED_FROM_MTZ4_BIT;
      if(isValidCONFCondition(MTZ_BIT_CONFIGURATION)) isValid = 0;
      break;
    case 102:
      (*uprMaska)   = (CTR_ZDZ_STARTED_FROM_UMIN1_BIT - (_CTR_ZDZ_PART_III - _CTR_ZDZ_PART_II));//CTR_ZDZ_STARTED_FROM_UMIN1_BIT;
      if(isValidCONFCondition(UMIN_BIT_CONFIGURATION)) isValid = 0;
      break;
    case 103:
      (*uprMaska)   = (CTR_ZDZ_STARTED_FROM_UMIN2_BIT - (_CTR_ZDZ_PART_III - _CTR_ZDZ_PART_II));//CTR_ZDZ_STARTED_FROM_UMIN2_BIT;
      if(isValidCONFCondition(UMIN_BIT_CONFIGURATION)) isValid = 0;
      break;

//123456
#if (                                   \
       (MODYFIKACIA_VERSII_PZ == 0) ||  \
       (MODYFIKACIA_VERSII_PZ == 3) ||  \
       (MODYFIKACIA_VERSII_PZ == 4) ||  \
       (MODYFIKACIA_VERSII_PZ == 6) ||  \
       (MODYFIKACIA_VERSII_PZ == 7) ||  \
       (MODYFIKACIA_VERSII_PZ == 10)||  \
       (MODYFIKACIA_VERSII_PZ == 13)||  \
       (MODYFIKACIA_VERSII_PZ == 14)||  \
       (MODYFIKACIA_VERSII_PZ == 17)||  \
     (MODYFIKACIA_VERSII_PZ == 14)      \
      )   

    case 104:
      (*uprMaska)   = CTR_ZDZ_OVD1_STATE_BIT;
      break;
    case 105:
      (*uprMaska)   = CTR_ZDZ_OVD2_STATE_BIT;
      break;
#define ZDZ_CONFIGURATION_END 106
    case ZDZ_CONFIGURATION_END:
      (*uprMaska)   = CTR_ZDZ_OVD3_STATE_BIT;
      break;

//123456
#else
#define ZDZ_CONFIGURATION_END 103
#endif

//  count_bit = 2;
    case 112:
      (*uprMaska)   = ZZ_BIT_CONFIGURATION;
      (*editControl) = &edition_settings.configuration;
      if(actControl)
        {
         //Функція обновлення змінних при зміні конфігурації
         if(action_after_changing_of_configuration(edition_settings.configuration, &edition_settings)) isValid = 0;
        }//if(actControl)
      break;
#define ZZ_CONFIGURATION_BEGIN 113
    case ZZ_CONFIGURATION_BEGIN:
      (*uprMaska)   = INDEX_ML_CTR_ZZ1_3I0_STATE;
      break;

//  count_bit = 4;
    case 115:
      (*uprMaska)   = INDEX_ML_CTR_ZZ1_SECTOR;
      break;
    case 116:
      (*uprMaska)   = INDEX_ML_CTR_ZZ1_NZZ_STATE;
      if(isValidZZTYPECondition(INDEX_ML_CTR_ZZ1_TYPE_BIT)) isValid = 0;
      break;
    case 117:
      (*uprMaska)   = INDEX_ML_CTR_ZZ1_3U0_STATE;
      break;
#define ZZ_CONFIGURATION_END 118
    case 118:
      (*uprMaska)   = INDEX_ML_CTR_ZZ1_TYPE_BIT;
      if(actControl)
        {
          //Функція обновлення змінних при зміні типу реле 33/3I0
          action_after_changing_zz1_type(&edition_settings);
        }//if(actControl)
      break;

//  count_bit = 3;
    case 128:
      (*uprMaska)   = TZNP_BIT_CONFIGURATION;
      (*editControl) = &edition_settings.configuration;
      if(actControl)
        {
          if(validData)if((edition_settings.control_extra_settings_1&(1<<INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_IB_I04))) isValid = 0;
          //Функція обновлення змінних при зміні конфігурації
          if(action_after_changing_of_configuration(edition_settings.configuration, &edition_settings)) isValid = 0;
        }//if(actControl)
      break;
#define TZNP_CONFIGURATION_BEGIN 129
    case TZNP_CONFIGURATION_BEGIN:
      (*uprMaska)   = INDEX_ML_CTR_TZNP1;
      break;
    case 130:
      (*uprMaska)   = INDEX_ML_CTR_TZNP1_VPERED;
      break;
    case 131:
      (*uprMaska)   = INDEX_ML_CTR_TZNP1_NAZAD;
      break;
    case 132:
      (*uprMaska)   = INDEX_ML_CTR_TZNP2;
      break;
    case 133:
      (*uprMaska)   = INDEX_ML_CTR_TZNP2_VPERED;
      break;
    case 134:
      (*uprMaska)   = INDEX_ML_CTR_TZNP2_NAZAD;
      break;
    case 135:
      (*uprMaska)   = INDEX_ML_CTR_TZNP3;
      break;
    case 136:
      (*uprMaska)   = INDEX_ML_CTR_TZNP3_VPERED;
      break;
#define TZNP_CONFIGURATION_END 137
    case TZNP_CONFIGURATION_END:
      (*uprMaska)   = INDEX_ML_CTR_TZNP3_NAZAD;
      break;

//  count_bit = 3;
    case 144:
      (*uprMaska)   = ZOP_BIT_CONFIGURATION;
      (*editControl) = &edition_settings.configuration;
      if(actControl)
        {
         //Функція обновлення змінних при зміні конфігурації
         if(action_after_changing_of_configuration(edition_settings.configuration, &edition_settings)) isValid = 0;
        }//if(actControl)
      break;
    case 145:
      (*uprMaska)   = CTR_ZOP_STATE_BIT;
      (*editControl) = &edition_settings.control_zop;
      if(isValidCONFCondition(ZOP_BIT_CONFIGURATION)) isValid = 0;
      break;

//  count_bit = 1;
    case 160:
      (*uprMaska)   = ACHR_CHAPV_BIT_CONFIGURATION;
      (*editControl) = &edition_settings.configuration;
      if(actControl)
        {
         //Функція обновлення змінних при зміні конфігурації
         if(action_after_changing_of_configuration(edition_settings.configuration, &edition_settings)) isValid = 0;
        }//if(actControl)
      break;
#define ACHR_CHAPV_CONFIGURATION_BEGIN 161
    case ACHR_CHAPV_CONFIGURATION_BEGIN:
      (*uprMaska)   = INDEX_CTR_ACHR1;
      break;
    case 162:
      (*uprMaska)   = INDEX_CTR_ACHR2;
      break;
    case 163:
      (*uprMaska)   = INDEX_CTR_CHAPV1;
      break;
    case 164:
      (*uprMaska)   = INDEX_CTR_CHAPV2;
      break;
#define ACHR_CHAPV_CONFIGURATION_END 165
    case ACHR_CHAPV_CONFIGURATION_END:
      (*uprMaska)   = INDEX_CTR_CHAPV_VID_DV;
      break;

//  count_bit = 3;
    case 176:
      (*uprMaska)   = UMIN_BIT_CONFIGURATION;
      (*editControl) = &edition_settings.configuration;
      if(actControl)
        {
         //Функція обновлення змінних при зміні конфігурації
         if(action_after_changing_of_configuration(edition_settings.configuration, &edition_settings)) isValid = 0;
        }//if(actControl)
      break;
#define UMIN_CONFIGURATION_BEGIN 177
    case UMIN_CONFIGURATION_BEGIN:
      (*uprMaska)   = INDEX_CTR_UMIN1;
      break;
    case 178:
      (*uprMaska)   = INDEX_CTR_UMIN2;
      break;
    case 179:
      (*uprMaska)   = INDEX_CTR_PO_UMIN1_OR_AND;
      break;
    case 180:
      (*uprMaska)   = INDEX_CTR_PO_UMIN2_OR_AND;
      break;
    case 181:
      (*uprMaska)   = INDEX_CTR_UMIN1_UBLK;
      break;
    case 182:
      (*uprMaska)   = INDEX_CTR_UMIN2_UBLK;
      break;
    case 183:
      (*uprMaska)   = INDEX_CTR_UMIN1_IBLK;
      break;
#define UMIN_CONFIGURATION_END 184
    case UMIN_CONFIGURATION_END:
      (*uprMaska)   = INDEX_CTR_UMIN2_IBLK;
      break;

//  count_bit = 3;
    case 192:
      (*uprMaska)   = UMAX_BIT_CONFIGURATION;
      (*editControl) = &edition_settings.configuration;
      if(actControl)
        {
         //Функція обновлення змінних при зміні конфігурації
         if(action_after_changing_of_configuration(edition_settings.configuration, &edition_settings)) isValid = 0;
        }//if(actControl)
      break;
#define UMAX_CONFIGURATION_BEGIN 193
    case UMAX_CONFIGURATION_BEGIN:
      (*uprMaska)   = INDEX_CTR_UMAX1;
      break;
    case 194:
      (*uprMaska)   = INDEX_CTR_UMAX2;
      break;
    case 195:
      (*uprMaska)   = INDEX_CTR_PO_UMAX1_OR_AND;
      break;
#define UMAX_CONFIGURATION_END 196
    case UMAX_CONFIGURATION_END:
      (*uprMaska)   = INDEX_CTR_PO_UMAX2_OR_AND;
      break;

//  count_bit = 3;
    case 225:
      (*uprMaska)   = INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE;
      (*editControl) = &edition_settings.control_extra_settings_1;
      if(actControl)
        {
          //Функція обновлення змінних
          if(action_after_changing_extra_settings(edition_settings.control_extra_settings_1, &edition_settings)) isValid = 0;
        }//if(actControl)
      break;

//  count_bit = 3;
    case 227:
      (*uprMaska)   = INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_IB_I04;
      (*editControl) = &edition_settings.control_extra_settings_1;
      if(actControl)
        {
          //Функція обновлення змінних
          if(action_after_changing_extra_settings(edition_settings.control_extra_settings_1, &edition_settings)) isValid = 0;
        }//if(actControl)
      break;
#define MARKER228 228
    case MARKER228:
      (*uprMaska)   = INDEX_ML_CTR_TRANSFORMATOR_PHASE_LINE;
      (*editControl) = &edition_settings.control_transformator;
      break;

//  count_bit = 3;
    case 288:
      (*uprMaska)   = APV_BIT_CONFIGURATION;
      (*editControl) = &edition_settings.configuration;
      if(actControl)
        {
         //Функція обновлення змінних при зміні конфігурації
         if(action_after_changing_of_configuration(edition_settings.configuration, &edition_settings)) isValid = 0;
        }//if(actControl)
      break;
#define APV_CONFIGURATION_BEGIN 289
    case APV_CONFIGURATION_BEGIN:
      (*uprMaska)   = INDEX_ML_CTRAPV_STAGE_1;
      break;
    case 290:
      (*uprMaska)   = INDEX_ML_CTRAPV_STAGE_2;
      break;
    case 291:
      (*uprMaska)   = INDEX_ML_CTRAPV_STAGE_3;
      break;
    case 292:
      (*uprMaska)   = INDEX_ML_CTRAPV_STAGE_4;
      break;
    case 293:
      (*uprMaska)   = INDEX_ML_CTRAPV_STARTED_FROM_MTZ1;
      if(isValidCONFCondition(MTZ_BIT_CONFIGURATION)) isValid = 0;
      break;
    case 294:
      (*uprMaska)   = INDEX_ML_CTRAPV_STARTED_FROM_MTZ2;
      if(isValidCONFCondition(MTZ_BIT_CONFIGURATION)) isValid = 0;
      break;
    case 295:
      (*uprMaska)   = INDEX_ML_CTRAPV_STARTED_FROM_MTZ3;
      if(isValidCONFCondition(MTZ_BIT_CONFIGURATION)) isValid = 0;
      break;
    case 296:
      (*uprMaska)   = INDEX_ML_CTRAPV_STARTED_FROM_MTZ4;
      if(isValidCONFCondition(MTZ_BIT_CONFIGURATION)) isValid = 0;
      break;
//  count_bit = 6;
    case 300:
      (*uprMaska)   = INDEX_ML_CTRAPV_BLK_CTRL_VV;
      break;
#define APV_CONFIGURATION_END 301
    case APV_CONFIGURATION_END:
      (*uprMaska)   = INDEX_ML_CTRAPV_BLK_CTRL_PRVV2;
      if(isValidCONFCondition(UROV_BIT_CONFIGURATION)) isValid = 0;
      break;

//  count_bit = 6;
    case 304:
      (*uprMaska)   = UROV_BIT_CONFIGURATION;
      (*editControl) = &edition_settings.configuration;
      if(actControl)
        {
         //Функція обновлення змінних при зміні конфігурації
         if(action_after_changing_of_configuration(edition_settings.configuration, &edition_settings)) isValid = 0;
        }//if(actControl)
      break;
#define UROV_CONFIGURATION_BEGIN 305
    case UROV_CONFIGURATION_BEGIN:
      (*uprMaska)   = INDEX_ML_CTRUROV_STATE;
      break;
    case 306:
      (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_MTZ1;
      if(isValidCONFCondition(MTZ_BIT_CONFIGURATION)) isValid = 0;
      break;
    case 307:
      (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_MTZ2;
      if(isValidCONFCondition(MTZ_BIT_CONFIGURATION)) isValid = 0;
      break;
    case 308:
      (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_MTZ3;
      if(isValidCONFCondition(MTZ_BIT_CONFIGURATION)) isValid = 0;
      break;
    case 309:
      (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_MTZ4;
      if(isValidCONFCondition(MTZ_BIT_CONFIGURATION)) isValid = 0;
      break;
    case 310:
      (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_MTZ04_1;
      if(isValidCONFCondition(MTZ04_BIT_CONFIGURATION)) isValid = 0;
      break;
    case 311:
      (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_MTZ04_2;
      if(isValidCONFCondition(MTZ04_BIT_CONFIGURATION)) isValid = 0;
      break;
    case 312:
      (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_UMAX1;
      if(isValidCONFCondition(UMAX_BIT_CONFIGURATION)) isValid = 0;
      break;
    case 313:
      (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_UMAX2;
      if(isValidCONFCondition(UMAX_BIT_CONFIGURATION)) isValid = 0;
      break;
    case 314:
      (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_UMIN1;
      if(isValidCONFCondition(UMIN_BIT_CONFIGURATION)) isValid = 0;
      break;
    case 315:
      (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_UMIN2;
      if(isValidCONFCondition(UMIN_BIT_CONFIGURATION)) isValid = 0;
      break;
    case 316:
      (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_ZOP1;
      if(isValidCONFCondition(ZOP_BIT_CONFIGURATION)) isValid = 0;
      break;

//  count_bit = 2;
    case 318:
      (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_NZZ;
      if(isValidCONFCondition(ZZ_BIT_CONFIGURATION)) isValid = 0;
      if(isValidZZTYPECondition(INDEX_ML_CTR_ZZ1_TYPE_BIT)) isValid = 0;
      break;
    case 319:
      (*uprMaska)   =  INDEX_ML_CTRUROV_STARTED_FROM_3I0;
      if(isValidCONFCondition(ZZ_BIT_CONFIGURATION)) isValid = 0;
      break;

//  count_bit = 1;
    case 321:
      (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_3U0;
      if(isValidCONFCondition(ZZ_BIT_CONFIGURATION)) isValid = 0;
      break;
    case 322:
      (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_TZNP1;
      if(isValidCONFCondition(TZNP_BIT_CONFIGURATION)) isValid = 0;
      break;
    case 323:
      (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_TZNP2;
      if(isValidCONFCondition(TZNP_BIT_CONFIGURATION)) isValid = 0;
      break;
    case 324:
      (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_TZNP3;
      if(isValidCONFCondition(TZNP_BIT_CONFIGURATION)) isValid = 0;
      break;
    case 325:
      (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_ZDZ;
      if(isValidCONFCondition(ZDZ_BIT_CONFIGURATION)) isValid = 0;
      break;
    case 326:
      (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_ACHR1;
      if(isValidCONFCondition(ACHR_CHAPV_BIT_CONFIGURATION)) isValid = 0;
      break;
#define UROV_CONFIGURATION_END 327
    case UROV_CONFIGURATION_END:
      (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_ACHR2;
      if(isValidCONFCondition(ACHR_CHAPV_BIT_CONFIGURATION)) isValid = 0;
      break;

//IF ВСТАВКА 329-336

//  count_bit = 1;
    case 368:
      (*uprMaska)   = INDEX_ML_CTRPRYVOD_VV;
      (*editControl) = &edition_settings.control_switch;
      break;

//  count_bit = 1;
//    case 371://Блокировка Вкл. ВВ от ДВ
//      (*uprMaska)   = INDEX_ML_CTREXTRA_SETTINGS_1_BLK_ON_CB_MISCEVE;
//      (*editControl) = &edition_settings.control_extra_settings_1;
//      break;
//    case 372://Блокировка Откл. ВВ от ДВ
//      (*uprMaska)   = INDEX_ML_CTREXTRA_SETTINGS_1_BLK_OFF_CB_MISCEVE;
//      (*editControl) = &edition_settings.control_extra_settings_1;
//      break;
    case 373://Контроль ФК
      (*uprMaska)   = INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_FB_ACTIVATION;
      (*editControl) = &edition_settings.control_extra_settings_1;
      break;
    case 374://Ресурс ВВ
      (*uprMaska)   = INDEX_ML_CTRRESURS_VV;
      (*editControl) = &edition_settings.control_switch;
      break;
    case 375://Разрешение блокировки готовности к ТУ от защит
      (*uprMaska)   = INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_READY_TU;
      (*editControl) = &edition_settings.control_extra_settings_1;
      break;
    case 376://Окно информации об отключении ВВ
      (*uprMaska)   = INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_WINDOW_OFF_CB;
      (*editControl) = &edition_settings.control_extra_settings_1;
      break;

//  count_bit = 1;
    case 384:
      (*uprMaska)   = VMP_BIT_CONFIGURATION;
      (*editControl) = &edition_settings.configuration;
      if(actControl)
        {
         if(action_after_changing_of_configuration(edition_settings.configuration, &edition_settings)) isValid = 0;
        }//if(actControl)
      break;
    case 385:
      (*uprMaska)   = INDEX_CTR_VMP_STATE;
      (*editControl) = &edition_settings.control_vmp;
      if(isValidCONFCondition(VMP_BIT_CONFIGURATION)) isValid = 0;
      break;

//  count_bit = 1;
    case 416:
      (*uprMaska)   = EL_BIT_CONFIGURATION;
      (*editControl) = &edition_settings.configuration;
      if(actControl)
        {
         if(action_after_changing_of_configuration(edition_settings.configuration, &edition_settings)) isValid = 0;
        }//if(actControl)
      break;

//  count_bit = 1;
    case 448:
      (*uprMaska)   = INDEX_ML_CTR_AR_AVAR_STATE;
      (*editControl) = &edition_settings.control_ar;
//      if(actControl)
//        {
//         if(action_after_changing_of_configuration(edition_settings.configuration, &edition_settings)) isValid = 0;
//        }//if(actControl)
      break;

//  count_bit = 3;
    case 464:
      (*uprMaska)   = DS_BIT_CONFIGURATION;
      (*editControl) = &edition_settings.configuration;
      if(actControl)
        {
         //Функція обновлення змінних при зміні конфігурації
         if(action_after_changing_of_configuration(edition_settings.configuration, &edition_settings)) isValid = 0;
        }//if(actControl)
      break;

    }//switch

  if((*uprMaska)!=0xFFFFFFFF)
    {
      if(inOffset>=MTZ_CONFIGURATION_BEGIN && inOffset<=MTZ_CONFIGURATION_END)
        {
          (*editControl) = &edition_settings.control_mtz;
          if(isValidCONFCondition(MTZ_BIT_CONFIGURATION)) isValid = 0;
        }//if

      if(inOffset>=MTZ04_CONFIGURATION_BEGIN && inOffset<=MTZ04_CONFIGURATION_END)
        {
          (*editControl) = &edition_settings.control_mtz04;
          if(isValidCONFCondition(MTZ04_BIT_CONFIGURATION)) isValid = 0;
        }//if

      if(inOffset>=ZDZ_CONFIGURATION_BEGIN && inOffset<=ZDZ_CONFIGURATION_END)
        {
          (*editControl) = &edition_settings.control_zdz;
          if(isValidCONFCondition(ZDZ_BIT_CONFIGURATION)) isValid = 0;
        }//if

      if(inOffset==ZNAM_CONFIGURATION_BEGIN)
        {
          (*editControl) = &edition_settings.control_znam;
          if(isValidCONFCondition(ZNAM_BIT_CONFIGURATION)) isValid = 0;
        }//if

      if(inOffset>=ZZ_CONFIGURATION_BEGIN && inOffset<=ZZ_CONFIGURATION_END)
        {
          (*editControl) = &edition_settings.control_zz;
          if(isValidCONFCondition(ZZ_BIT_CONFIGURATION)) isValid = 0;
        }//if

      if(inOffset>=TZNP_CONFIGURATION_BEGIN && inOffset<=TZNP_CONFIGURATION_END)
        {
          (*editControl) = &edition_settings.control_tznp;
          if(isValidCONFCondition(TZNP_BIT_CONFIGURATION)) isValid = 0;
        }//if

      if(inOffset>=ACHR_CHAPV_CONFIGURATION_BEGIN && inOffset<=ACHR_CHAPV_CONFIGURATION_END)
        {
          (*editControl) = &edition_settings.control_achr_chapv;
          if(isValidCONFCondition(ACHR_CHAPV_BIT_CONFIGURATION)) isValid = 0;
        }//if

      if(inOffset>=UMIN_CONFIGURATION_BEGIN && inOffset<=UMIN_CONFIGURATION_END)
        {
          (*editControl) = &edition_settings.control_Umin;
          if(isValidCONFCondition(UMIN_BIT_CONFIGURATION)) isValid = 0;
        }//if

      if(inOffset>=UMAX_CONFIGURATION_BEGIN && inOffset<=UMAX_CONFIGURATION_END)
        {
          (*editControl) = &edition_settings.control_Umax;
          if(isValidCONFCondition(UMAX_BIT_CONFIGURATION)) isValid = 0;
        }//if

      if(inOffset>=APV_CONFIGURATION_BEGIN && inOffset<=APV_CONFIGURATION_END)
        {
          (*editControl) = &edition_settings.control_apv;
          if(isValidCONFCondition(APV_BIT_CONFIGURATION)) isValid = 0;
        }//if

      if(inOffset>=UROV_CONFIGURATION_BEGIN && inOffset<=UROV_CONFIGURATION_END)
        {
          (*editControl) = &edition_settings.control_urov;
          if(isValidCONFCondition(UROV_BIT_CONFIGURATION)) isValid = 0;
        }//if

    }//if((*uprMaska)!=-1)

  if(inOffset>=49 && inOffset<73)
    {
      (*uprMaska)   = inOffset-49;//maska;
      (*editControl) = &edition_settings.control_UP;
      if(isValidCONFCondition(UP_BIT_CONFIGURATION)) isValid = 0;
      if(actControl)
        {
          //Функція обновлення коефіцієнта повернення після зміни налаштувань УЗ
          change_setpoint_to_ctrl_UP(&edition_settings, edition_settings.control_UP);

        }//if(actControl)
    }//if

  if(inOffset>=329 && inOffset<337)
    {
      int inupOffset = inOffset-329;
      (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_UP1+inupOffset;
      (*editControl) = &edition_settings.control_urov;
      if(isValidCONFCondition(UROV_BIT_CONFIGURATION)) isValid = 0;
      if(isValidCONFCondition(UP_BIT_CONFIGURATION)) isValid = 0;
    }//if

  return isValid;
}//uprFunc000(int inOffset, uint32_t *uprMaska, uint32_t **editControl);

void loadUPRBigActualDataBit(int beginOffset, int endOffset)
{
  //ActualData
  for(int ii=0; ii<(1+(END_ADR_BIT-BEGIN_ADR_BIT+1)/16); ii++) tempReadArray[ii] = 0;
  for(int item=0; item<(END_ADR_BIT-BEGIN_ADR_BIT+1); item++)
    {
      int value =0;
      if(item>=beginOffset && item<endOffset)
        {
          uint32_t uprMaska=0;
          uint32_t *editControl=NULL;
          uprFuncRead000(item, &uprMaska, &editControl);
          if(editControl==NULL) value=0;
          else
            {
              //читать значение
              value = (*editControl)&(1<<uprMaska);
            }//else
        }//if(item>=beginOffset && item<endOffset)

      int tsdata = 0;
      if(value) tsdata=1;
      tempReadArray[item/16] |= tsdata<<(item%16);
    }//for(int item=0; item<568; item++)
}//loadUPRBigActualDataBit(int beginOffset, int endOffset)

/**************************************/
//подготовка компонента UPR
/**************************************/
void constructorUPRBigComponent(COMPONENT_OBJ *uprbigcomp)
{
  uprbigcomponent = uprbigcomp;

  uprbigcomponent->getModbusRegister = getUPRBigModbusRegister;//получить содержимое регистра
  uprbigcomponent->getModbusBit      = getUPRBigModbusBit;//получить содержимое бита
  uprbigcomponent->setModbusRegister = setUPRBigModbusRegister;//получить содержимое регистра
  uprbigcomponent->setModbusBit      = setUPRBigModbusBit;//получить содержимое бита

  uprbigcomponent->postWriteAction = postUPRBigWriteAction;//action после записи
}//prepareDVinConfig

int getUPRBigModbusRegister(int adrReg)
{
  extern int globalcntReg;//к-во  для чтения
  extern int globalbeginAdrReg;//адрес нач
//получить содержимое регистра
  if(privateUPRBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  if(uprbigcomponent->isActiveActualData)
    {
      edition_settings = current_settings_interfaces;//делаем копию
      int begin = globalbeginAdrReg-BEGIN_ADR_REGISTER;
      if(begin<0) globalcntReg += begin;

      int beginOffset = (adrReg-BEGIN_ADR_REGISTER)*16;
      int endOffset   = beginOffset +globalcntReg*16;

      loadUPRBigActualDataBit(beginOffset, endOffset); //ActualData
    }//if(acmdsmallcomponent->isActiveActualData)
  uprbigcomponent->isActiveActualData = 0;

  return tempReadArray[adrReg-BEGIN_ADR_REGISTER];
}//getDOUTBigModbusRegister(int adrReg)
int getUPRBigModbusBit(int adrBit)
{
  //получить содержимое bit
  extern int globalcntBit;//к-во бит для чтения
  extern int globalbeginAdrBit;//адрес нач бит
  //получить содержимое bit
  if(privateUPRBigGetBit2(adrBit)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(uprbigcomponent->isActiveActualData)
    {
      edition_settings = current_settings_interfaces;//делаем копию
      int begin = globalbeginAdrBit-BEGIN_ADR_BIT;
      if(begin<0) globalcntBit += begin;

      int beginOffset = adrBit-BEGIN_ADR_BIT;
      int endOffset   = adrBit-BEGIN_ADR_BIT +globalcntBit;
      loadUPRBigActualDataBit(beginOffset, endOffset); //ActualData
    }//if(acmdsmallcomponent->isActiveActualData)
  uprbigcomponent->isActiveActualData = 0;

  short tmp   = tempReadArray[(adrBit-BEGIN_ADR_BIT)/16];
  short maska = 1<<((adrBit-BEGIN_ADR_BIT)%16);
  if(tmp&maska) return 1;
  return 0;
}//getDOUTBigModbusRegister(int adrReg)
int setUPRBigModbusRegister(int adrReg, int dataReg)
{
  //записать содержимое регистра
  if(privateUPRBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(uprbigcomponent->isActiveActualData)
    {
      edition_settings = current_settings_interfaces;//делаем копию
    }//if(uprbigcomponent->isActiveActualData)
  superClearActiveActualData();

  superSetOperativMarker(uprbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//записать в буфер

  int beginOffset = (adrReg-BEGIN_ADR_REGISTER)*16;
  for(int item=beginOffset; item<(beginOffset+16); item++)
    {
      int temp = 0;
      if(dataReg&(1<<(item-beginOffset))) temp=1;//валидация на включение
      if(temp)
        if(!uprFuncValid000(item, temp)) return MARKER_ERRORDIAPAZON;
    }//for

  return 0;
}//setUPRBigModbusRegister(int adrReg, int dataReg)
int setUPRBigModbusBit(int adrBit, int dataBit)
{
  //записать содержимое bit
  if(privateUPRBigGetBit2(adrBit)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(uprbigcomponent->isActiveActualData)
    {
      edition_settings = current_settings_interfaces;//делаем копию
    }//if(uprbigcomponent->isActiveActualData)
  uprbigcomponent->isActiveActualData = 0;

  superSetOperativMarker(uprbigcomponent, adrBit);
  superSetTempWriteArray(dataBit);//записать в буфер

  int offset = adrBit-BEGIN_ADR_BIT;
  if(dataBit)//валидация на включение
    if(!uprFuncValid000(offset, dataBit)) return MARKER_ERRORDIAPAZON;

  return 0;
}//getDOUTBigModbusRegister(int adrReg)

int postUPRBigWriteAction(void)
{
  extern int upravlSetting;//флаг Setting
  extern int upravlSchematic;//флаг Rang
//action после записи
  int beginAdr = uprbigcomponent->operativMarker[0];
  if(beginAdr<0) return 0;//не было записи
  int endAdr   = uprbigcomponent->operativMarker[1];
  int flag=0;
  int countAdr = endAdr-beginAdr+1;
  if(endAdr<0) countAdr = 1;
  if(beginAdr>=BEGIN_ADR_BIT)
    {
      //работа с битами
      int offsetTempWriteArray = superFindTempWriteArrayOffset(BEGIN_ADR_BIT);//найти смещение TempWriteArray
      for(int i=0; i<countAdr; i++)
        {
          uint32_t uprMaska=0;
          uint32_t *editControl=NULL;
          int offset = i+beginAdr-BEGIN_ADR_BIT;
          uprFuncValidWrite000(0, offset, &uprMaska, &editControl);
          if(editControl==NULL) continue;
          uint32_t value = tempWriteArray[offsetTempWriteArray+i];

          //установка изменений
          (*editControl) &= ~(1<<uprMaska);
          (*editControl) |= (value<<uprMaska);

          //actControl
          if(value)//валидация на включение
                if(!uprFunc000(1, offset, &uprMaska, 1, &editControl)) return ERROR_VALID3;//ошибка валидации
          //ОСОБАЯ СБОРКА
          if((editControl == (uint32_t*)&edition_settings.control_extra_settings_1) ||
              (editControl == (uint32_t*)&edition_settings.configuration) ||
              (editControl == (uint32_t*)&edition_settings.control_UP )
            )
            {
             if(!value)//валидация на отключение
                if(!uprFunc000(1, offset, &uprMaska, 0, &editControl)) return ERROR_VALID3;//ошибка валидации
              upravlSchematic = 1;//флаг Rang
            }//if
          //утверждение изменений
          flag=1;
        }//for
    }//if(beginAdr>=BEGIN_ADR_BIT)
  else
    {
      //работа с регистрами
      int offsetTempWriteArray = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//найти смещение TempWriteArray
      for(int i=0; i<countAdr; i++)
        {
          int offset = i+beginAdr-BEGIN_ADR_REGISTER;
          for(int bit=0; bit<16; bit++)
            {
              uint32_t uprMaska=0;
              uint32_t *editControl=NULL;
              uprFuncValidWrite000(0, offset*16+bit, &uprMaska, &editControl);
              if(editControl==NULL) continue;
              uint32_t value = tempWriteArray[offsetTempWriteArray+i];
              int temp = 0;
              if(value&(1<<bit)) temp = 1;

              //установка изменений
              (*editControl) &= ~(1<<uprMaska);
              (*editControl) |= (temp<<uprMaska);

              if(temp)//валидация на включение
                if(!uprFunc000(1, offset*16+bit, &uprMaska, 1, &editControl)) return ERROR_VALID3;//ошибка валидации
              //ОСОБАЯ СБОРКА
              if((editControl == (uint32_t*)&edition_settings.control_extra_settings_1) ||
                  (editControl == (uint32_t*)&edition_settings.configuration) ||
                  (editControl == (uint32_t*)&edition_settings.control_UP )
                )
                {
                 if(!temp)//валидация на отключение
                if(!uprFunc000(1, offset*16+bit, &uprMaska, 0, &editControl)) return ERROR_VALID3;//ошибка валидации
                  upravlSchematic = 1;//флаг Rang
                }//if
              //утверждение изменений
              flag=1;
            }//for
        }//for
    }//else

//ОСОБАЯ ПРОВЕРКА

  if(beginAdr>=BEGIN_ADR_BIT)
    {
      //работа с битами
      for(int i=0; i<countAdr; i++)
        {
          int offsetBit = i+beginAdr-BEGIN_ADR_BIT;
          if(specialСheckUPRBigWriteAction(offsetBit)) return ERROR_VALID3;//ошибка валидации
        }//for
    }//if(beginAdr>=BEGIN_ADR_BIT)
  else
    {
      //работа с регистрами
      for(int i=0; i<countAdr; i++)
        {
          int offsetReg = i+beginAdr-BEGIN_ADR_REGISTER;
          for(int bit=0; bit<16; bit++)
            {
              if(specialСheckUPRBigWriteAction(offsetReg*16+bit)) return ERROR_VALID3;//ошибка валидации
            }//for
        }//for
    }//else

  if(flag) upravlSetting = 1;//флаг Setting

  return 0;
}//

int specialСheckUPRBigWriteAction(int offset)
{
 if(offset==MARKER228)
 {
   if((edition_settings.control_transformator & (1<<INDEX_ML_CTR_TRANSFORMATOR_PHASE_LINE))==0) //0-фазные
   {
       if((edition_settings.control_extra_settings_1 & (1<<INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE))!=0) //1-линейные
         return 1;//ошибка
   }//if
 }//if(offset==MARKER228)
  return 0;//нет ошибки
}//specialСheckUPRBigWriteAction

int specialСheckLinU(void)
{
  if((edition_settings.control_extra_settings_1 & (1<<INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE))!=0) //1-линейные
         return 1;//линейные
  return 0;
}

int isValidCONFCondition(unsigned int confMaska)
{
  if(!(edition_settings.configuration&(1<<confMaska))) return 1;
  return 0;
}//isValidCONFCondition(unsigned int confControl, unsigned int confMaska, uint32_t uprMaska, int validData)
int isValidEXTRACondition(unsigned int extraMaska)
{
  if(edition_settings.control_extra_settings_1&(1<<extraMaska)) return 1;
  return 0;
}//isValidEXTRACondition(unsigned int extraControl, unsigned int extraMaska, uint32_t uprMaska, int validData)
int isValidZZTYPECondition(unsigned int extraMaska)
{
  if(edition_settings.control_zz&(1<<extraMaska)) return 1;
  return 0;
}//isValidZZTYPECondition(unsigned int extraControl, unsigned int extraMaska, uint32_t uprMaska, int validData)

int privateUPRBigGetReg2(int adrReg)
{
  //проверить внешний периметр
  return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)

int privateUPRBigGetBit2(int adrBit)
{
  //проверить внешний периметр
  return controlPerimetr(adrBit, BEGIN_ADR_BIT, END_ADR_BIT);
}//privateGetReg2(int adrReg)
