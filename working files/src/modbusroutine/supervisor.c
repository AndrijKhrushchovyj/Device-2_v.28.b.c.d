
#include "header.h"

int inputPacketParser(void);
void inputPacketParserUSB(void);
void inputPacketParserRS485(void);
unsigned short int  AddCRC(unsigned char inpbyte, unsigned short int oldCRC);
int Error_modbus_m(unsigned int address, unsigned int function, unsigned int error, unsigned char *output_data);

int outputFunc20PacketEncoder(int adrUnit, int *fileNumber, int *recordNumber, int *recordLen, int cntitem);
int outputFunc16PacketEncoder(int adrUnit, int adrReg, int cntReg);
int outputFunc15PacketEncoder(int adrUnit, int adrBit, int cntBit);
int outputFunc6PacketEncoder(int adrUnit, int adrReg, int dataReg);
int outputFunc5PacketEncoder(int adrUnit, int adrBit, int dataBit);
int outputFunc3PacketEncoder(int adrUnit, int adrReg, int cntReg);
int outputFunc1PacketEncoder(int adrUnit, int adrReg, int cntReg);
int passwordImunitetReg(int adrReg);
int passwordImunitetBit(int adrBit);

SRAM1 unsigned char  *outputPacket;
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
SRAM1 unsigned char  outputPacket_TCP[300];
#endif
SRAM1 unsigned char  outputPacket_USB[300];
SRAM1 unsigned char  outputPacket_RS485[300];

SRAM1 int sizeOutputPacket;
SRAM1 unsigned char *inputPacket;
SRAM1 int *received_count;
SRAM1 int globalcntBit;//к-во бит
SRAM1 int globalcntReg;//к-во регистров
SRAM1 int globalbeginAdrReg;//адрес нач регистра
SRAM1 int globalbeginAdrBit;//адрес нач бит
SRAM1 int upravlSetting;//флаг Setting
SRAM1 int upravlSchematic;//флаг Shematic

/**************************************/
//разбор входного пакета
/**************************************/
int inputPacketParser(void)
{
  upravlSetting=0;//флаг Setting
  upravlSchematic=0;//флаг Shematic

  int adrUnit = inputPacket[0];
  int numFunc = inputPacket[1];//номер ф-ции

  sizeOutputPacket = 0;
  indexTW = 0;//индекс буфера записи
  switch(numFunc)
    {
    case 2:
    case 1:
    {
      globalbeginAdrBit  = (unsigned int)inputPacket[3] +256*(unsigned int)inputPacket[2];
      globalcntBit      = (unsigned int)inputPacket[5] +256*(unsigned int)inputPacket[4];
      if(globalcntBit>1000) return 0;//слишком длинный пакет
      if((*received_count)!=8) return 0;//нарушено к-во

      outputPacket[1] = (unsigned char)numFunc;
      sizeOutputPacket = outputFunc1PacketEncoder(adrUnit, globalbeginAdrBit, globalcntBit);
    }
    break;
    case 4:
    case 3:
    {
      globalbeginAdrReg  = (unsigned int)inputPacket[3] +256*(unsigned int)inputPacket[2];
      globalcntReg      = (unsigned int)inputPacket[5] +256*(unsigned int)inputPacket[4];
      if(globalcntReg>125) return 0;//слишком длинный пакет
      if((*received_count)!=8) return 0;//нарушено к-во

      outputPacket[1] = (unsigned char)numFunc;
      sizeOutputPacket = outputFunc3PacketEncoder(adrUnit, globalbeginAdrReg, globalcntReg);
    }
    break;
    case 5:
    {
      globalbeginAdrBit  = (unsigned int)inputPacket[3] +256*(unsigned int)inputPacket[2];
      int dataBit = (unsigned int)inputPacket[5] +256*(unsigned int)inputPacket[4];
      if((*received_count)!=8) return 0;//нарушено к-во
      globalcntBit      = 1;
      switch(dataBit)
        {
        case 0xFF00:
          dataBit = 1;
          break;
        case 0x0000:
          dataBit = 0;
          break;
        default:
        {
          sizeOutputPacket = Error_modbus_m(adrUnit, // address,
                                            numFunc,//function,
                                            ERROR_ILLEGAL_DATA_VALUE,//error,
                                            outputPacket);//output_data
          dataBit = -1;
        }
        }//switch
      if(dataBit<0) break;
      outputPacket[1] = (unsigned char)numFunc;
      sizeOutputPacket = outputFunc5PacketEncoder(adrUnit, globalbeginAdrBit, dataBit);
    }
    break;
    case 6:
    {
      globalbeginAdrReg  = (unsigned int)inputPacket[3] +256*(unsigned int)inputPacket[2];
      int dataReg = (unsigned int)inputPacket[5] +256*(unsigned int)inputPacket[4];
      if((*received_count)!=8) return 0;//нарушено к-во
      globalcntReg      = 1;
      outputPacket[1] = (unsigned char)numFunc;
      sizeOutputPacket = outputFunc6PacketEncoder(adrUnit, globalbeginAdrReg, dataReg);
    }
    break;
    case 15:
    {
      globalbeginAdrBit  = (unsigned int)inputPacket[3] +256*(unsigned int)inputPacket[2];
      globalcntBit  = (unsigned int)inputPacket[5] +256*(unsigned int)inputPacket[4];
      int cntByte = (unsigned int)inputPacket[6];
      if(globalcntBit>125) return 0;//слишком длинный пакет

      for(int i=0; i<cntByte; i++)
        tempReadArray[i] = (unsigned short)(inputPacket[7+i]);
      outputPacket[1] = (unsigned char)numFunc;
      sizeOutputPacket = outputFunc15PacketEncoder(adrUnit, globalbeginAdrBit, globalcntBit);
    }
    break;
    case 16:
    {
      globalbeginAdrReg  = (unsigned int)inputPacket[3] +256*(unsigned int)inputPacket[2];
      globalcntReg  = (unsigned int)inputPacket[5] +256*(unsigned int)inputPacket[4];
      if(globalcntReg>125) return 0;//слишком длинный пакет

      for(int i=0; i<globalcntReg; i++)
        tempReadArray[i] = (unsigned short)(inputPacket[7+1+(i*2)]) +256*(unsigned short)(inputPacket[7+(i*2)]);
      outputPacket[1] = (unsigned char)numFunc;
      sizeOutputPacket = outputFunc16PacketEncoder(adrUnit, globalbeginAdrReg, globalcntReg);
    }
    break;
    case 20:
    {
      if(*received_count>255) return 0;//слишком длинный пакет
      // int byteCount = inputPacket[2];
      //int refType[40];
      int fileNumber[40];
      int recordNumber[40];
      int recordLen[40];
      int item=0;
      int recordLenSum=0;//к-во запрошенных регистров

      for(item=0; item<40; item++)
        {
          if(*received_count<(5+(item+1)*7)) break;//подсчет количества item
          fileNumber[item]   = (unsigned int)inputPacket[5+item*7] +256*(unsigned int)inputPacket[4+item*7];
          recordNumber[item] = (unsigned int)inputPacket[7+item*7] +256*(unsigned int)inputPacket[6+item*7];
          recordLen[item]    = (unsigned int)inputPacket[9+item*7] +256*(unsigned int)inputPacket[8+item*7];
          recordLenSum += recordLen[item];
          if(recordLenSum>125)//слишком много регистров захотел
            {
              sizeOutputPacket = Error_modbus_m(adrUnit, // address,
                                                inputPacket[1],//function,
                                                2,//error,
                                                outputPacket);//output_data
              item = -1;
              break;
            }//if
        }//for
      if(item<0) break;
      if(item<1) return 0;//неправильный вх пакет

      outputPacket[1] = (unsigned char)numFunc;
      sizeOutputPacket = outputFunc20PacketEncoder(adrUnit, fileNumber, recordNumber, recordLen, item);
      if(sizeOutputPacket==-1)
              sizeOutputPacket = Error_modbus_m(adrUnit, // address,
                                                inputPacket[1],//function,
                                                ERROR_ILLEGAL_DATA_ADDRESS,//error,
                                                outputPacket);//output_data
      if(sizeOutputPacket==-2)
              sizeOutputPacket = Error_modbus_m(adrUnit, // address,
                                                inputPacket[1],//function,
                                                ERROR_SLAVE_DEVICE_BUSY,//error,
                                                outputPacket);//output_data
              break;
    }//case 20:
    break;

    default:
    {
      sizeOutputPacket = Error_modbus_m(adrUnit, // address,
                                        numFunc,//function,
                                        ERROR_ILLEGAL_FUNCTION,//error,
                                        outputPacket);//output_data
    }
    }//switch

  unsigned short CRC_sum = 0xffff;
  for (int index = 0; index < sizeOutputPacket; index++) CRC_sum = AddCRC((*(outputPacket + index)) ,CRC_sum);
  *(outputPacket + sizeOutputPacket)  = CRC_sum & 0xff;
  *(outputPacket + sizeOutputPacket+1)  = CRC_sum >> 8;
  sizeOutputPacket += 2;
  return 1;
}//inputPacketParser

int outputFunc16PacketEncoder(int adrUnit, int adrReg, int cntReg)
{
//выходной кодировщик 16 функции
  int   flag = 1;
  superPreAction();//action до записи
  for(int i=0; i<cntReg; i++)
    {
      uint32_t dataReg = (unsigned short) tempReadArray[i];
      int result = superWriterRegister(adrReg+i, dataReg);
      switch(result)
        {
        case MARKER_OUTPERIMETR:
          result = -1;
          break;
        case MARKER_ERRORPERIMETR:
          result = -2;
          return Error_modbus_m(adrUnit, // address,
                                outputPacket[1],//function,
                                ERROR_ILLEGAL_DATA_ADDRESS,//error,
                                outputPacket);//output_data
        case MARKER_ERRORDIAPAZON:
          result = -3;
          return Error_modbus_m(adrUnit, // address,
                                outputPacket[1],//function,
                                ERROR_ILLEGAL_DATA_VALUE,//error,
                                outputPacket);//output_data
        case MARKER_ERRORPAROL:
          result = -4;
          return Error_modbus_m(adrUnit, // address,
                                outputPacket[1],//function,
                                ERROR_ACKNOWLEDGE,//error,
                                outputPacket);//output_data
        default:
        {
          flag = 0;
        }
        }//switch
    }//for
  if(flag)//незначащие пакеты недопустимы
    return Error_modbus_m(adrUnit, // address,
                          outputPacket[1],//function,
                          ERROR_ILLEGAL_DATA_ADDRESS,//error,
                          outputPacket);//output_data

  int upravlReturn = superPostWriteAction();//action после записи
  if(upravlReturn) return upravlReturn;//сформировать пакет ошибки

  int idxOutputPacket = 0;
//adrUnit
  outputPacket[idxOutputPacket] = (unsigned char)adrUnit;
  idxOutputPacket++;
//numFunc
  idxOutputPacket++;
//MadrReg
  outputPacket[idxOutputPacket] = (unsigned char)((adrReg>>8)&0xFF);
  idxOutputPacket++;
//LadrReg
  outputPacket[idxOutputPacket] = (unsigned char)(adrReg&0xFF);
  idxOutputPacket++;
//MdataReg
  outputPacket[idxOutputPacket] = (unsigned char)((cntReg>>8)&0xFF);
  idxOutputPacket++;
//LdataReg
  outputPacket[idxOutputPacket] = (unsigned char)(cntReg&0xFF);
  idxOutputPacket++;
  return idxOutputPacket;
}//outputFunc16PacketEncoder(int adrUnit, int adrReg, int cntReg)

int outputFunc15PacketEncoder(int adrUnit, int adrBit, int cntBit)
{
//выходной кодировщик 16 функции
  short dataBit = 0;
  int   flag = 1;
  superPreAction();//action до записи
  for(int i=0; i<cntBit; i++)
    {
      int idx = i/8;
      dataBit = tempReadArray[idx];
      short maska = 1<<(i%8);
      int result = superWriterBit(adrBit+i, dataBit&maska ? 1:0);
      switch(result)
        {
        case MARKER_OUTPERIMETR:
          result = -1;
          break;
        case MARKER_ERRORPERIMETR:
          result = -2;
          return Error_modbus_m(adrUnit, // address,
                                outputPacket[1],//function,
                                ERROR_ILLEGAL_DATA_ADDRESS,//error,
                                outputPacket);//output_data
        case MARKER_ERRORDIAPAZON:
          result = -3;
          return Error_modbus_m(adrUnit, // address,
                                outputPacket[1],//function,
                                ERROR_ILLEGAL_DATA_VALUE,//error,
                                outputPacket);//output_data
        case MARKER_ERRORPAROL:
          result = -4;
          return Error_modbus_m(adrUnit, // address,
                                outputPacket[1],//function,
                                ERROR_ACKNOWLEDGE,//error,
                                outputPacket);//output_data
        default:
        {
          flag = 0;
        }
        }//switch
    }//for
  if(flag)//незначащие пакеты недопустимы
    return Error_modbus_m(adrUnit, // address,
                          outputPacket[1],//function,
                          ERROR_ILLEGAL_DATA_ADDRESS,//error,
                          outputPacket);//output_data

  int upravlReturn = superPostWriteAction();//action после записи
  if(upravlReturn) return upravlReturn;//сформировать пакет ошибки

  int idxOutputPacket = 0;
//adrUnit
  outputPacket[idxOutputPacket] = (unsigned char)adrUnit;
  idxOutputPacket++;
//numFunc
  idxOutputPacket++;
//MadrBit
  outputPacket[idxOutputPacket] = (unsigned char)((adrBit>>8)&0xFF);
  idxOutputPacket++;
//LadrBit
  outputPacket[idxOutputPacket] = (unsigned char)(adrBit&0xFF);
  idxOutputPacket++;
//McntBit
  outputPacket[idxOutputPacket] = (unsigned char)((cntBit>>8)&0xFF);
  idxOutputPacket++;
//LcntBit
  outputPacket[idxOutputPacket] = (unsigned char)(cntBit&0xFF);
  idxOutputPacket++;
  return idxOutputPacket;
}//outputFunc15PacketEncoder(int adrUnit, int adrBit, int cntBit)

int outputFunc6PacketEncoder(int adrUnit, int adrReg, int dataReg)
{
//выходной кодировщик 6 функции
  superPreAction();//action до записи
  int result = superWriterRegister(adrReg, dataReg);
  switch(result)
    {
    case MARKER_OUTPERIMETR:
      result = -1;
      return Error_modbus_m(adrUnit, // address,
                            outputPacket[1],//function,
                            ERROR_ILLEGAL_DATA_ADDRESS,//error,
                            outputPacket);//output_data
    case MARKER_ERRORPERIMETR:
      result = -2;
      return Error_modbus_m(adrUnit, // address,
                            outputPacket[1],//function,
                            ERROR_ILLEGAL_DATA_ADDRESS,//error,
                            outputPacket);//output_data
    case MARKER_ERRORDIAPAZON:
      result = -3;
      return Error_modbus_m(adrUnit, // address,
                            outputPacket[1],//function,
                            ERROR_ILLEGAL_DATA_VALUE,//error,
                            outputPacket);//output_data
    case MARKER_ERRORPAROL:
      result = -4;
      return Error_modbus_m(adrUnit, // address,
                            outputPacket[1],//function,
                            ERROR_ACKNOWLEDGE,//error,
                            outputPacket);//output_data
    }//switch

  int upravlReturn = superPostWriteAction();//action после записи
  if(upravlReturn) return upravlReturn;//сформировать пакет ошибки

  int idxOutputPacket = 0;
//adrUnit
  outputPacket[idxOutputPacket] = (unsigned char)adrUnit;
  idxOutputPacket++;
//numFunc
  idxOutputPacket++;
//MadrReg
  outputPacket[idxOutputPacket] = (unsigned char)((adrReg>>8)&0xFF);
  idxOutputPacket++;
//LadrReg
  outputPacket[idxOutputPacket] = (unsigned char)(adrReg&0xFF);
  idxOutputPacket++;
//MdataReg
  outputPacket[idxOutputPacket] = (unsigned char)((dataReg>>8)&0xFF);
  idxOutputPacket++;
//LdataReg
  outputPacket[idxOutputPacket] = (unsigned char)(dataReg&0xFF);
  idxOutputPacket++;
  return idxOutputPacket;
}//outputFunc6PacketEncoder(int adrUnit, int adrReg, int dataReg)

int outputFunc5PacketEncoder(int adrUnit, int adrBit, int dataBit)
{
//выходной кодировщик 5 функции
  superPreAction();//action до записи
  int result = superWriterBit(adrBit, dataBit);
  switch(result)
    {
    case MARKER_OUTPERIMETR:
      result = -1;
      return Error_modbus_m(adrUnit, // address,
                            outputPacket[1],//function,
                            ERROR_ILLEGAL_DATA_ADDRESS,//error,
                            outputPacket);//output_data
    case MARKER_ERRORPERIMETR:
      result = -2;
      return Error_modbus_m(adrUnit, // address,
                            outputPacket[1],//function,
                            ERROR_ILLEGAL_DATA_ADDRESS,//error,
                            outputPacket);//output_data
    case MARKER_ERRORDIAPAZON:
      result = -3;
      return Error_modbus_m(adrUnit, // address,
                            outputPacket[1],//function,
                            ERROR_ILLEGAL_DATA_VALUE,//error,
                            outputPacket);//output_data
    case MARKER_ERRORPAROL:
      result = -4;
      return Error_modbus_m(adrUnit, // address,
                            outputPacket[1],//function,
                            ERROR_ACKNOWLEDGE,//error,
                            outputPacket);//output_data
    }//switch

  int upravlReturn = superPostWriteAction();//action после записи
  if(upravlReturn) return upravlReturn;//сформировать пакет ошибки

  int idxOutputPacket = 0;
//adrUnit
  outputPacket[idxOutputPacket] = (unsigned char)adrUnit;
  idxOutputPacket++;
//numFunc
  idxOutputPacket++;
//MadrBit
  outputPacket[idxOutputPacket] = (unsigned char)((adrBit>>8)&0xFF);
  idxOutputPacket++;
//LadrBit
  outputPacket[idxOutputPacket] = (unsigned char)(adrBit&0xFF);
  idxOutputPacket++;
//MdataReg
  outputPacket[idxOutputPacket] = (unsigned char)(0xFF);
  if(dataBit==0) outputPacket[idxOutputPacket] = (unsigned char)(0x00);
  idxOutputPacket++;
//LdataReg
  outputPacket[idxOutputPacket] = (unsigned char)(0x0);
  idxOutputPacket++;
  return idxOutputPacket;
}//outputFunc5PacketEncoder(int adrUnit, int adrBit)

int outputFunc3PacketEncoder(int adrUnit, int adrReg, int cntReg)
{
//выходной кодировщик 3 функции
  short dataRegister[130];
  int   idxDataRegister = 0;
  int   flag = 1;
  superPreAction();//action до чтения
  for(; idxDataRegister<cntReg; idxDataRegister++)
    {
      int result = superReaderRegister(adrReg+ idxDataRegister);
      switch(result)
        {
        case MARKER_OUTPERIMETR:
          dataRegister[idxDataRegister] = -1;
          dataRegister[idxDataRegister] = 0;//(short) result;
          break;
        case MARKER_ERRORPERIMETR://ошибка периметра
          dataRegister[idxDataRegister] = -2;
          return Error_modbus_m(adrUnit, // address,
                                outputPacket[1],//function,
                                ERROR_ILLEGAL_DATA_ADDRESS,//error,
                                outputPacket);//output_data
        default:
        {
          dataRegister[idxDataRegister] = (short) result;
          flag = 0;
        }
        }//switch
    }//for
  if(flag)//незначащие пакеты недопустимы
    return Error_modbus_m(adrUnit, // address,
                          outputPacket[1],//function,
                          ERROR_ILLEGAL_DATA_ADDRESS,//error,
                          outputPacket);//output_data

  int idxOutputPacket = 0;
//adrUnit
  outputPacket[idxOutputPacket] = (unsigned char)adrUnit;
  idxOutputPacket++;
//numFunc
  idxOutputPacket++;
//cnt
  outputPacket[idxOutputPacket] = (unsigned char)cntReg*2;
  idxOutputPacket++;
//data
  for(int i=0; i<idxDataRegister; i++)
    {
//Mdata
      outputPacket[idxOutputPacket] = (unsigned char)((dataRegister[i]>>8)&0xFF);
      idxOutputPacket++;
//Ldata
      outputPacket[idxOutputPacket] = (unsigned char)(dataRegister[i]&0xFF);
      idxOutputPacket++;
    }//for
  return idxOutputPacket;
}//outputFunc3PacketEncoder(int adrUnit, int adrReg, int cntReg)

int outputFunc1PacketEncoder(int adrUnit, int adrBit, int cntBit)
{
//выходной кодировщик 1 функции
  short dataRegister[130];
  for(int i=0; i<128; i++) dataRegister[i]=0;
  int   idxDataBit = 0;
  int   flag = 1;

  superPreAction();//action до чтения
  for(; idxDataBit<cntBit; idxDataBit++)
    {
      int result = superReaderBit(adrBit+ idxDataBit);

      switch(result)
        {
        case MARKER_OUTPERIMETR:
          break;
        case MARKER_ERRORPERIMETR:
//          dataRegister[idxReg] = -2;
          return Error_modbus_m(adrUnit, // address,
                                outputPacket[1],//function,
                                ERROR_ILLEGAL_DATA_ADDRESS,//error,
                                outputPacket);//output_data
        default:
        {
          dataRegister[idxDataBit/8] |= (result<<(idxDataBit%8));
          flag = 0;
        }
        }//switch
    }//for
  if(flag)//незначащие пакеты недопустимы
    return Error_modbus_m(adrUnit, // address,
                          outputPacket[1],//function,
                          ERROR_ILLEGAL_DATA_ADDRESS,//error,
                          outputPacket);//output_data

  int idxOutputPacket = 0;
//adrUnit
  outputPacket[idxOutputPacket] = (unsigned char)adrUnit;
  idxOutputPacket++;
//numFunc
  idxOutputPacket++;

      int cntByte = cntBit/8;
      if(cntBit%8) cntByte++;
      outputPacket[idxOutputPacket] = cntByte;
      idxOutputPacket++;
//data
      for(int i=0; i<cntByte; i++)
        {
//data
          outputPacket[idxOutputPacket] = (unsigned char)(dataRegister[i]&0xFF);
          idxOutputPacket++;
        }//for

  return idxOutputPacket;
}//outputFunc1PacketEncoder(int adrUnit, int adrBit, int cntBit)

/**************************************/
//action до чтения и записи
/**************************************/
void superPreAction(void)
{
  int i=0;
  for(; i<TOTAL_COMPONENT; i++)
    {
      config_array[i].operativMarker[0]=-1;
      config_array[i].operativMarker[1]=-1;
      config_array[i].isActiveActualData=1;
    }//for
}//superPreAction

/**************************************/
//action после записи
/**************************************/
int superPostWriteAction(void)
{
  int i=0;
  for(; i<TOTAL_COMPONENT; i++)
    {
      switch(config_array[i].postWriteAction())

        {
        case ERROR_VALID2:
          return Error_modbus_m(inputPacket[0], // address,
                                inputPacket[1],//function,
                                ERROR_ILLEGAL_DATA_VALUE,//error,
                                outputPacket);//output_data
        case ERROR_VALID3:
          return Error_modbus_m(inputPacket[0], // address,
                                inputPacket[1],//function,
                                ERROR_SLAVE_DEVICE_BUSY,//error,
                                outputPacket);//output_data
        }//switch

    }//for

    //рестарт таймера 12000 Время активации пароля после простоя
    restart_timeout_interface |= (1 << pointInterface);//метка интерфейса 0-USB 1-RS485
    if(!(upravlSetting>0 || upravlSchematic>0)) return 0;
    _SET_BIT(active_functions, RANG_SETTINGS_CHANGED);
    restart_timeout_idle_new_settings = true;
    current_settings_interfaces = edition_settings;//утвердить изменения
    if(upravlSetting==1) type_of_settings_changed |= (1 << SETTINGS_DATA_CHANGED_BIT);
    if(upravlSchematic==1) type_of_settings_changed |= (1 << RANGUVANNJA_DATA_CHANGED_BIT);

  return 0;
}//superPostWriteAction

/**************************************/
//регистровый читатель
/**************************************/
int superReaderRegister(int adrReg)
{
  int result=0;
  int i=0;
  for(; i<TOTAL_COMPONENT; i++)
    {
      result = config_array[i].getModbusRegister(adrReg);
      if(!(result==MARKER_OUTPERIMETR)) break;
    }//for
  if(i==TOTAL_COMPONENT) result = MARKER_OUTPERIMETR;
  return result;
}//superReaderRegister()

/**************************************/
//регистровый писатель
/**************************************/
int superWriterRegister(int adrReg, int dataReg)
{
  int result=0;
  int i=0;
  switch(pointInterface)//метка интерфейса 0-USB 1-RS485
  {
   case USB_RECUEST:
      if(passwordImunitetReg(adrReg) && password_set_USB) return MARKER_ERRORPAROL;
   break;
   case RS485_RECUEST:
      if(passwordImunitetReg(adrReg) && password_set_RS485) return MARKER_ERRORPAROL;
   break;
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
   case LAN_RECUEST:
      if(passwordImunitetReg(adrReg) && password_set_LAN) return MARKER_ERRORPAROL;
   break;
#endif
  }//switch

  for(; i<TOTAL_COMPONENT; i++)
    {
      result = config_array[i].setModbusRegister(adrReg, dataReg);
      if(!(result==MARKER_OUTPERIMETR)) break;
    }//for
  if(i==TOTAL_COMPONENT) result = MARKER_OUTPERIMETR;
  return result;
}//superWriterRegister(int adrReg, int dataReg)

/**************************************/
//bit читатель
/**************************************/
int superReaderBit(int adrBit)
{
  int result=0;
  int i=0;
  for(; i<TOTAL_COMPONENT; i++)
    {
      result = config_array[i].getModbusBit(adrBit);
      if(!(result==MARKER_OUTPERIMETR)) break;
    }//for
  if(i==TOTAL_COMPONENT) result = MARKER_OUTPERIMETR;
  return result;
}//superReaderBit()

/**************************************/
//bit писатель
/**************************************/
int superWriterBit(int adrBit, int dataBit)
{
  int result=0;
  int i=0;
  switch(pointInterface)//метка интерфейса 0-USB 1-RS485
  {
   case USB_RECUEST:
      if(passwordImunitetBit(adrBit) && password_set_USB) return MARKER_ERRORPAROL;
   break;
   case RS485_RECUEST:
      if(passwordImunitetBit(adrBit) && password_set_RS485) return MARKER_ERRORPAROL;
   break;
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
   case LAN_RECUEST:
      if(passwordImunitetBit(adrBit) && password_set_LAN) return MARKER_ERRORPAROL;
   break;
#endif
  }//switch

  for(; i<TOTAL_COMPONENT; i++)
    {
      result = config_array[i].setModbusBit(adrBit, dataBit);
      if(!(result==MARKER_OUTPERIMETR)) break;
    }//for
  if(i==TOTAL_COMPONENT) result = MARKER_OUTPERIMETR;
  return result;
}//superReaderRegister()

/**************************************/
//установка оперативных маркеров
/**************************************/
void superSetOperativMarker(COMPONENT_OBJ *component, int adrReg)
{
  if(component->operativMarker[0]<0) component->operativMarker[0] = adrReg;
  else component->operativMarker[1] = adrReg;
}//superSetOperativMarker(COMPONENT_OBJ component, int adrReg)

void superSetTempWriteArray(int dataReg)
{
//записать в буфер
  if(indexTW>=MAXIMUMTW) return;
  tempWriteArray[indexTW] = dataReg;
  indexTW++;//индекс буфера записи
}//superSetTempWriteArray(int dataReg)

int superFindTempWriteArrayOffset(int adr)
{
//найти смещение TempWriteArray
  int iComponentOffset=0;
  int i=0;
  for(; i<TOTAL_COMPONENT; i++)
    {
      if(config_array[i].operativMarker[0]<0) continue;
      if(config_array[i].operativMarker[0]<adr)
        {
          if(config_array[i].operativMarker[1]<0) iComponentOffset++;
          else iComponentOffset+=(config_array[i].operativMarker[1]-config_array[i].operativMarker[0]+1);
        }//if
    }//for
  return iComponentOffset;
}//superFindTempWriteArrayOffset(int adr)

int controlPerimetr(int adrReg, int begin_adr, int end_adr)
{
  //проверить внешний периметр
  int count_register = end_adr-begin_adr+1;
  if(adrReg>=begin_adr && adrReg<(begin_adr+count_register)) return 0;
  return MARKER_OUTPERIMETR;
}//controlPerimetr(int adrReg, int begin_adr, int end_adr)

void superClearActiveActualData(void)
{
  extern COMPONENT_OBJ *ustbigcomponent;
  extern COMPONENT_OBJ *andbigcomponent;
  extern COMPONENT_OBJ *doutbigcomponent;
  extern COMPONENT_OBJ *dtrbigcomponent;
  extern COMPONENT_OBJ *dvbigcomponent;
  extern COMPONENT_OBJ *keybigcomponent;
  extern COMPONENT_OBJ *mftbigcomponent;
  extern COMPONENT_OBJ *notbigcomponent;
  extern COMPONENT_OBJ *orbigcomponent;
  extern COMPONENT_OBJ *pkvbigcomponent;
  extern COMPONENT_OBJ *regbigcomponent;
  extern COMPONENT_OBJ *rprbigcomponent;
  extern COMPONENT_OBJ *sdibigcomponent;
  extern COMPONENT_OBJ *uprbigcomponent;
  extern COMPONENT_OBJ *vvbigcomponent;
  extern COMPONENT_OBJ *xorbigcomponent;
  extern COMPONENT_OBJ *pfbigcomponent;

  ustbigcomponent->isActiveActualData = 0;
  andbigcomponent->isActiveActualData = 0;
  doutbigcomponent->isActiveActualData = 0;
  dtrbigcomponent->isActiveActualData = 0;
  dvbigcomponent->isActiveActualData = 0;
  keybigcomponent->isActiveActualData = 0;
  mftbigcomponent->isActiveActualData = 0;
  notbigcomponent->isActiveActualData = 0;
  orbigcomponent->isActiveActualData = 0;
  pkvbigcomponent->isActiveActualData = 0;
  regbigcomponent->isActiveActualData = 0;
  rprbigcomponent->isActiveActualData = 0;
  sdibigcomponent->isActiveActualData = 0;
  uprbigcomponent->isActiveActualData = 0;
  vvbigcomponent->isActiveActualData = 0;
  xorbigcomponent->isActiveActualData = 0;
  pfbigcomponent->isActiveActualData = 0;
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
  extern COMPONENT_OBJ *goosbigcomponent;
  extern COMPONENT_OBJ *mmsbigcomponent;
  extern COMPONENT_OBJ *lanbigcomponent;
  goosbigcomponent->isActiveActualData = 0;
  mmsbigcomponent->isActiveActualData = 0;
  lanbigcomponent->isActiveActualData = 0;
#endif
#ifdef  MODYFIKACIA_VERSII_DS
  extern COMPONENT_OBJ *doutbigdscomponent;
  doutbigdscomponent->isActiveActualData = 0;
#endif
#if (                                \
     (MODYFIKACIA_VERSII_PZ == 8) || \
     (MODYFIKACIA_VERSII_PZ == 18)   \
    )
  extern COMPONENT_OBJ *dvoutbigcomponent;
  dvoutbigcomponent->isActiveActualData = 0;
#endif
}//superClearActiveActualData()

int passwordImunitetRegYUSTBigComponent(int adrReg);
int passwordImunitetRegPKVBigComponent(int adrReg);
int passwordImunitetRegREGBigComponent(int adrReg);
int passwordImunitetBitACMDSmallComponent(int adrBit);
int passwordImunitetBit(int adrBit)
{
  if(passwordImunitetBitACMDSmallComponent(adrBit)==MARKER_OUTPERIMETR) return 1;//имунитета нет
  return 0;//имунитет есть 
}//passwordImunitetBit(int adrBit)

int passwordImunitetReg(int adrReg)
{
  if(passwordImunitetRegPKVBigComponent(adrReg)==MARKER_OUTPERIMETR &&
     passwordImunitetRegREGBigComponent(adrReg)==MARKER_OUTPERIMETR &&
     passwordImunitetRegYUSTBigComponent(adrReg)==MARKER_OUTPERIMETR) return 1;//имунитета нет  
     return 0;//имунитет есть 
}//passwordImunitetReg(int adrReg)

