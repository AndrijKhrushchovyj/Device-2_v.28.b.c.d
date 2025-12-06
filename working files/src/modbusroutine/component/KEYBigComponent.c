#include "header.h"

//начальный регистр в карте памяти
#define BEGIN_ADR_REGISTER 2220
//конечный регистр в карте памяти
#define END_ADR_REGISTER 2267

#define REGISTERS_KEY 8

int privateKEYBigGetReg2(int adrReg);

int getKEYBigModbusRegister(int);//получить содержимое регистра
int getKEYBigModbusBit(int);//получить содержимое бита
int setKEYBigModbusRegister(int, int);// регистра
int setKEYBigModbusBit(int, int);// бита

int  postKEYBigWriteAction(void);//action после записи

SRAM1 COMPONENT_OBJ *keybigcomponent;

/**************************************/
//подготовка компонента KEY
/**************************************/
void constructorKEYBigComponent(COMPONENT_OBJ *keybigcomp)
{
  keybigcomponent = keybigcomp;

  keybigcomponent->getModbusRegister = getKEYBigModbusRegister;//получить содержимое регистра
  keybigcomponent->getModbusBit      = getKEYBigModbusBit;//получить содержимое бита
  keybigcomponent->setModbusRegister = setKEYBigModbusRegister;// регистра
  keybigcomponent->setModbusBit      = setKEYBigModbusBit;// бита

  keybigcomponent->postWriteAction = postKEYBigWriteAction;//action после записи
}//prepareDVinConfig

int getKEYBigModbusRegister(int adrReg)
{
  //получить содержимое регистра
  if(privateKEYBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  return getRangN_SMALLModbusRegister(&current_settings_interfaces.ranguvannja_buttons[0], REGISTERS_KEY, adrReg-BEGIN_ADR_REGISTER );
}//getDOUTBigModbusRegister(int adrReg)
int getKEYBigModbusBit(int x)
{
  //получить содержимое регистра
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setKEYBigModbusRegister(int adrReg, int dataReg)
{
  //записать содержимое регистра
  if(privateKEYBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(keybigcomponent->isActiveActualData)
    {
      edition_settings = current_settings_interfaces;//делаем копию
    }//if(uprbigcomponent->isActiveActualData)
  superClearActiveActualData();

  superSetOperativMarker(keybigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//записать в буфер
  if(validN_SMALLACMD(dataReg)==MARKER_ERRORPERIMETR) return MARKER_ERRORPERIMETR;

  int offset = adrReg-BEGIN_ADR_REGISTER;
  uint32_t maska = ((uint32_t)edition_settings.buttons_mode)&(1<<((offset/REGISTERS_KEY)*1));
  //проверка на допустимость Режим «Кнопка»
  if(!maska)
  {
    switch((unsigned short)dataReg)
    {
     case 50000://- 1-а гр.уставок    *
     case 50001://- 2-а гр.уставок     *
     case 50002://- 3-а гр.уставок     *
     case 50003://- 4-а гр.уставок     *
     case 50016://- Блок.МСЗ1          *
     case 50032://- Блок.МСЗ2          *
     case 50041://- Блок.приск.МСЗ2    *
     case 50048://- Блок.МСЗ3          *
     case 50064://- Блок.МСЗ4    
     case 50080://- Блок.МСЗ 0.4кВ 1
     case 50083://- Блок.МСЗ 0.4кВ 2
     case 50086://- Бл.п.МСЗ 0.4кВ 2
     case 50112://- Блок.ЗДЗ
     case 50113://- Пуск ЗДЗ від ДВ
     case 50128://- Блок.СЗЗ    
     case 50144://- Блок.TZNP1
     case 50154://- Блок.TZNP2
     case 50164://- Блок.TZNP3
     case 50176://- Блок.ЗЗП(КОФ)  
     case 50192://- Блокировка ЗНмин1
     case 50197://- Блокировка ЗНмин1
     case 50202://- Пуск ЗНмин1
     case 50203://- Пуск ЗНмин2
     case 50208://- Блокировка ЗНмакс1
     case 50211://- Блокировка ЗНмакс2
     case 50288://- Стат.блок.АПВ  
     case 50304://  Пуск ПРВВ від ДВ
     case 50368://- АЧР/ЧАПВ    
     case 50369://- Блок.АЧР1
     case 50370://- Блок.АЧР2
     case 50384://- Блок.УЗ1
     case 50387://- Блок.УЗ2
     case 50390://- Блок.УЗ3
     case 50393://- Блок.УЗ4
     case 50396://- Блок.УЗ5
     case 50399://- Блок.УЗ6
     case 50402://- Блок.УЗ7
     case 50405://- Блок.УЗ8
     case 50528://- Увімк.ВВ      *
     case 50529://- Вимк.ВВ       *
     case 50530://- Блок.увімкн.ВВ  *
     case 50534://- Стан ВВ         *
     case 50536://- Контроль Увімк. *
     case 50537://- Контроль Вимк.  *
//   case 50562://- Сброс индикации
//   case 50563://- Сброс реле
     case 50569://- Місц./Дистанц. *
     case 50576://- Вимк.від зовн.з.* 
//   case 50599://- С.блк.Гот.до ТУ
        return MARKER_ERRORPERIMETR; //не прошла валидация
    }//switch
      if((((unsigned short)dataReg) >= 50608) && (((unsigned short)dataReg) < 50632)) return MARKER_ERRORPERIMETR; //не прошла валидация
      if((((unsigned short)dataReg) >= 50640) && (((unsigned short)dataReg) <= 50643)) return MARKER_ERRORPERIMETR; //не прошла валидация
  }//if(!maska)
  else
  //проверка на допустимость Режим «Ключ
  switch((unsigned short)dataReg)
  {
//   case 50000://- 1-а гр.уставок    *
//   case 50001://- 2-а гр.уставок     *
//   case 50002://- 3-а гр.уставок     *
//   case 50003://- 4-а гр.уставок     *
//   case 50016://- Блок.МСЗ1          *
//   case 50032://- Блок.МСЗ2          *
//   case 50041://- Блок.приск.МСЗ2    *
//   case 50048://- Блок.МСЗ3          *
//   case 50064://- Блок.МСЗ4    
//   case 50080://- Блок.МСЗ 0.4кВ 1
//   case 50083://- Блок.МСЗ 0.4кВ 2
//   case 50086://- Бл.п.МСЗ 0.4кВ 2
//   case 50112://- Блок.ЗДЗ
   case 50113://- Пуск ЗДЗ від ДВ
//   case 50128://- Блок.СЗЗ    
//   case 50144://- Блок.TZNP1
//   case 50154://- Блок.TZNP2
//   case 50164://- Блок.TZNP3
//   case 50176://- Блок.ЗЗП(КОФ)  
//   case 50192://- Блокировка ЗНмин1
//   case 50197://- Блокировка ЗНмин1
   case 50202://- Пуск ЗНмин1
   case 50203://- Пуск ЗНмин2
//   case 50208://- Блокировка ЗНмакс1
//   case 50211://- Блокировка ЗНмакс2
//   case 50288://- Стат.блок.АПВ  
   case 50304://  Пуск ПРВВ від ДВ
   case 50368://- АЧР/ЧАПВ    
//   case 50369://- Блок.АЧР1
//   case 50370://- Блок.АЧР2
//   case 50384://- Блок.УЗ1
//   case 50387://- Блок.УЗ2
//   case 50390://- Блок.УЗ3
//   case 50393://- Блок.УЗ4
//   case 50396://- Блок.УЗ5
//   case 50399://- Блок.УЗ6
//   case 50402://- Блок.УЗ7
//   case 50405://- Блок.УЗ8
   case 50528://- Увімк.ВВ      *
   case 50529://- Вимк.ВВ       *
//   case 50530://- Блок.увімкн.ВВ  *
   case 50534://- Стан ВВ         *
   case 50536://- Контроль Увімк. *
   case 50537://- Контроль Вимк.  *
   case 50562://- Сброс индикации
   case 50563://- Сброс реле
//   case 50569://- Місц./Дистанц. *
   case 50576://- Вимк.від зовн.з.* 
   case 50599://- С.блк.Гот.до ТУ
      return MARKER_ERRORPERIMETR; //не прошла валидация
  }//switch

  return 0;
}//getDOUTBigModbusRegister(int adrReg)
int setKEYBigModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //получить содержимое регистра
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

int postKEYBigWriteAction(void) {
extern int upravlSchematic;//флаг Rang
//action после записи
  int beginAdr = keybigcomponent->operativMarker[0];
  if(beginAdr<0) return 0;//не было записи
  int endAdr   = keybigcomponent->operativMarker[1];
  int countAdr = endAdr-beginAdr+1;
  if(endAdr<0) countAdr = 1;

  writeRangN_SMALLModbusRegister(&edition_settings.ranguvannja_buttons[0], REGISTERS_KEY, beginAdr,
                                countAdr, BEGIN_ADR_REGISTER);

      //ВАЛИДАЦИЯ УСПЕШНА - УСТАНОВКА
     upravlSchematic = 1;//флаг Rang

  return 0;
}//

int privateKEYBigGetReg2(int adrReg)
{
  //проверить внешний периметр
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)
