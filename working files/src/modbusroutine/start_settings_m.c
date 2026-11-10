
#include "const_global.h"
#include "variables_external_m.h"

/**************************************/
//Стартовая инициализация компонентов
/**************************************/
void global_component_installation(void) {
  constructorIUSmallComponent(&(config_array[IUSMALLCOMPONENT]));//подготовка компонента iu
  constructorACMDSmallComponent(&(config_array[ACMDSMALLCOMPONENT]));//подготовка компонента acmd
  constructorYustBigComponent(&(config_array[YUSTBIGCOMPONENT]));//подготовка компонента yust
  constructorDOUTSmallComponent(&(config_array[DOUTSMALLCOMPONENT]));//подготовка компонента dout
  constructorDVSmallComponent(&(config_array[DVSMALLCOMPONENT]));//подготовка компонента dv
  constructorKEYSmallComponent(&(config_array[KEYSMALLCOMPONENT]));//подготовка компонента key
  constructorGCMDSmallComponent(&(config_array[GCMDSMALLCOMPONENT]));//подготовка компонента gcmd
  constructorRPRSmallComponent(&(config_array[RPRSMALLCOMPONENT]));//подготовка компонента rpr
  constructorSDISmallComponent(&(config_array[SDISMALLCOMPONENT]));//подготовка компонента sdi
  constructorAISmallComponent(&(config_array[AISMALLCOMPONENT]));//подготовка компонента aismall
  constructorPKVBigComponent(&(config_array[PKVBIGCOMPONENT]));//подготовка компонента pkv
  constructorPREGBigComponent(&(config_array[PREGBIGCOMPONENT]));//подготовка компонента preg
  constructorREG1BigComponent(&(config_array[REG1BIGCOMPONENT]));//подготовка компонента reg1
  constructorREG2BigComponent(&(config_array[REG2BIGCOMPONENT]));//подготовка компонента reg2
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
  constructorGOOSSmallComponent(&(config_array[GOOSSMALLCOMPONENT]));//подготовка компонента goos
  constructorMMSSmallComponent(&(config_array[MMSSMALLCOMPONENT]));//подготовка компонента mms
  constructorLANSmallComponent(&(config_array[LANSMALLCOMPONENT]));//подготовка компонента lan
#endif
//  constructorUVVComponent2021(&(config_array2021func[UVVCOMPONENT2021]));//подготовка компонента UVVCOMPONENT2021
//  constructorPROTComponent2021(&(config_array2021func[PROTCOMPONENT2021]));//подготовка компонента PROTCOMPONENT2021
//  constructorPERIFComponent2021(&(config_array2021func[PERIFCOMPONENT2021]));//подготовка компонента PERIFCOMPONENT2021
}//global_vareiables_installation_bo(void)

