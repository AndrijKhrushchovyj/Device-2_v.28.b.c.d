
#include "const_global.h"
#include "variables_external_m.h"

/**************************************/
//Стартовая инициализация компонентов
/**************************************/
void global_component_installation(void) {
  constructorIUSmallComponent(&(config_array[IUSMALLCOMPONENT]));//подготовка компонента iu
  constructorUSTBigComponent(&(config_array[USTBIGCOMPONENT]));//подготовка компонента ust
  constructorACMDSmallComponent(&(config_array[ACMDSMALLCOMPONENT]));//подготовка компонента acmd
  constructorUPRBigComponent(&(config_array[UPRBIGCOMPONENT]));//подготовка компонента upr
  constructorANDBigComponent(&(config_array[ANDBIGCOMPONENT]));//подготовка компонента and
  constructorDOUTBigComponent(&(config_array[DOUTBIGCOMPONENT]));//подготовка компонента rele
  constructorDOUTSmallComponent(&(config_array[DOUTSMALLCOMPONENT]));//подготовка компонента dout
  constructorDTRBigComponent(&(config_array[DTRBIGCOMPONENT]));//подготовка компонента trg
  constructorDVBigComponent(&(config_array[DVBIGCOMPONENT]));//подготовка компонента dv
  constructorDVSmallComponent(&(config_array[DVSMALLCOMPONENT]));//подготовка компонента dv
  constructorGCMDSmallComponent(&(config_array[GCMDSMALLCOMPONENT]));//подготовка компонента gcmd
  constructorKEYBigComponent(&(config_array[KEYBIGCOMPONENT]));//подготовка компонента key
  constructorMFTBigComponent(&(config_array[MFTBIGCOMPONENT]));//подготовка компонента mft
  constructorNOTBigComponent(&(config_array[NOTBIGCOMPONENT]));//подготовка компонента not
  constructorORBigComponent(&(config_array[ORBIGCOMPONENT]));//подготовка компонента or
  constructorPKVBigComponent(&(config_array[PKVBIGCOMPONENT]));//подготовка компонента pkv
  constructorRAISmallComponent(&(config_array[RAISMALLCOMPONENT]));//подготовка компонента raismall
  constructorREGBigComponent(&(config_array[REGBIGCOMPONENT]));//подготовка компонента reg
  constructorPREGBigComponent(&(config_array[PREGBIGCOMPONENT]));//подготовка компонента reg
  constructorRPRBigComponent(&(config_array[RPRBIGCOMPONENT]));//подготовка компонента prpr
  constructorRPRSmallComponent(&(config_array[RPRSMALLCOMPONENT]));//подготовка компонента rpr
  constructorSDIBigComponent(&(config_array[SDIBIGCOMPONENT]));//подготовка компонента sdi
  constructorSDISmallComponent(&(config_array[SDISMALLCOMPONENT]));//подготовка компонента sdi
  constructorVVBigComponent(&(config_array[VVBIGCOMPONENT]));//подготовка компонента vv
  constructorXORBigComponent(&(config_array[XORBIGCOMPONENT]));//подготовка компонента xor
  constructorYustBigComponent(&(config_array[YUSTBIGCOMPONENT]));//подготовка компонента yust
  constructorAISmallComponent(&(config_array[AISMALLCOMPONENT]));//подготовка компонента aismall
  constructorQAISmallComponent(&(config_array[QAISMALLCOMPONENT]));//подготовка компонента qaismall
  constructorPFBigComponent(&(config_array[PFBIGCOMPONENT]));//подготовка компонента pf
  constructorRDSBigComponent(&(config_array[RDSBIGCOMPONENT]));//подготовка компонента rds
  constructorDFRESBigComponent(&(config_array[DFRESBIGCOMPONENT]));//подготовка компонента dfres
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
  constructorGOOSBigComponent(&(config_array[GOOSBIGCOMPONENT]));//подготовка компонента goos
  constructorMMSBigComponent(&(config_array[MMSBIGCOMPONENT]));//подготовка компонента mms
  constructorLANBigComponent(&(config_array[LANBIGCOMPONENT]));//подготовка компонента lan
#endif
#ifdef  MODYFIKACIA_VERSII_DS
  constructorDOUTBigDSComponent(&(config_array[DOUTBIGDSCOMPONENT]));//подготовка компонента DS
#endif
#if (                                \
     (MODYFIKACIA_VERSII_PZ == 8) || \
     (MODYFIKACIA_VERSII_PZ == 18)   \
    )
  constructorDVOUTBigComponent(&(config_array[DVOUTBIGCOMPONENT]));//подготовка компонента dvrele
#endif
}//global_vareiables_installation_bo(void)

