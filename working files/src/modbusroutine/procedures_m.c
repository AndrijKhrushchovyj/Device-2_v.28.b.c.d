#include "header.h"

extern unsigned char  *outputPacket;
extern unsigned char  outputPacket_USB[300];
extern unsigned char  outputPacket_RS485[300];
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
extern unsigned char  outputPacket_TCP[300];
#endif

extern int sizeOutputPacket;
extern unsigned char *inputPacket;
extern int *received_count;
extern int globalcntBit;//к-во бит
extern int globalcntReg;//к-во регистров
extern int globalbeginAdrReg;//адрес нач регистра
extern int globalbeginAdrBit;//адрес нач бит

int superReader20(int offsetRegister, int fileNumber, int recordNumber, int recordLen);
int openRegistrator(int number_file);

int  outputFunc20PacketEncoder(int adrUnit, int *fileNumber, int *recordNumber, int *recordLen, int cntitem);
int  inputPacketParser(void);
void inputPacketParserUSB(void);
void inputPacketParserRS485(void);
unsigned short int  AddCRC(unsigned char inpbyte, unsigned short int oldCRC);
int  Error_modbus_m(unsigned int address, unsigned int function, unsigned int error, unsigned char *output_data);

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
/**************************************/
//разбор входного пакета TCP
/**************************************/
void inputPacketParserLAN(void)
{
//размер префикса TCP  
//#define TCP_PREFIXSIZE 6  
  pointInterface=LAN_RECUEST;//метка интерфейса 0-USB 1-RS485

  received_count = &LAN_received_count;

  outputPacket = outputPacket_TCP;
  inputPacket = LAN_received;
//                +TCP_PREFIXSIZE;//убрать префикс TCP
//  *received_count -= TCP_PREFIXSIZE;//убрать префикс TCP
  
  if((*received_count)<0)
  {
    LAN_received_count = 0;//очистить вход
    return;//что-то пошло не так
  }//if
  
  *received_count += 2;//симитировать CRC
  //Перевірка контрольної суми
//  unsigned short CRC_sum;
//  CRC_sum = 0xffff;
//  for (int index = 0; index < (*received_count-2); index++) CRC_sum = AddCRC(*(inputPacket + index),CRC_sum);
//  if((CRC_sum & 0xff)  != *(inputPacket+*received_count-2)) return;
//  if ((CRC_sum >> 8  ) != *(inputPacket+*received_count-1)) return;

//  if(inputPacket[0]!=current_settings.address) return;

  if(inputPacketParser()==0)
  {
   LAN_received_count = 0;//очистить вход
   return;
  }//if

  sizeOutputPacket -= 2;//убрать CRC

  LAN_transmiting_count = sizeOutputPacket;
//  for (int i = 0; i < TCP_PREFIXSIZE; i++) LAN_transmiting[i] = 0;//добавить префикс TCP
//  LAN_transmiting[1] = 1;//префикс TCP
//  LAN_transmiting[5] = sizeOutputPacket;//префикс TCP
  for (int i = 0; i < LAN_transmiting_count; i++) LAN_transmiting[i/*+ TCP_PREFIXSIZE*/] = outputPacket[i];
//  LAN_transmiting_count += TCP_PREFIXSIZE;//добавить префикс TCP
  LAN_received_count = 0;//очистить вход
  _SET_STATE (queue_mo, STATE_QUEUE_MO_SEND_MODBUS_TCP_RESP);//отправить результат
}//inputPacketParserLAN(void)
#endif  

/**************************************/
//разбор входного пакета USB
/**************************************/
void inputPacketParserUSB(void)
{
  pointInterface=USB_RECUEST;//метка интерфейса 0-USB 1-RS485

  received_count = &usb_received_count;

  inputPacket = usb_received;
  outputPacket = outputPacket_USB;
  //Перевірка контрольної суми
  unsigned short CRC_sum;
  CRC_sum = 0xffff;
  for (int index = 0; index < (*received_count-2); index++) CRC_sum = AddCRC(*(inputPacket + index),CRC_sum);
  if((CRC_sum & 0xff)  != *(inputPacket+*received_count-2)) return;
  if ((CRC_sum >> 8  ) != *(inputPacket+*received_count-1)) return;

  if(inputPacket[0]!=current_settings.address) return;

  if(inputPacketParser()==0) return;

  usb_transmiting_count = sizeOutputPacket;
  for (int i = 0; i < usb_transmiting_count; i++) usb_transmiting[i] = outputPacket[i];
  data_usb_transmiting = true;
}//inputPacketParserUSB(void)

/**************************************/
//разбор входного пакета RS485
/**************************************/
void inputPacketParserRS485(void)
{
  pointInterface=RS485_RECUEST;//метка интерфейса 0-USB 1-RS485

  inputPacket = RxBuffer_RS485;

  received_count = &RxBuffer_RS485_count;
  outputPacket = outputPacket_RS485;

  //Перевірка контрольної суми
  unsigned short CRC_sum;
  CRC_sum = 0xffff;
  for (int index = 0; index < (*received_count-2); index++) CRC_sum = AddCRC(*(inputPacket + index),CRC_sum);
  if((CRC_sum & 0xff)  != *(inputPacket+*received_count-2) ||//) return;
      (CRC_sum >> 8  ) != *(inputPacket+*received_count-1))
    {
      /***
      12345
      Причина рестарту (не співпала контрольна сума)
      ***/
      //reason_of_restart_RS485 |= (1 << 5);
      /***/

      restart_monitoring_RS485();
      return;
    }

  if(!(inputPacket[0]==current_settings.address))
    {
      /***
      12345
      Причина рестарту (не співпала адреса 2)
      ***/
      //reason_of_restart_RS485 |= (1 << 6);
      /***/

      restart_monitoring_RS485();
      return;
    }

  if(inputPacketParser()==0)
    {
      /***
      12345
      Причина рестарту (запит пакету відповіді завеликого розміру)
      ***/
      // reason_of_restart_RS485 |= (1 << 7);
      /***/

      restart_monitoring_RS485();
      return;
    }

  TxBuffer_RS485_count = sizeOutputPacket;
  for (int i = 0; i < TxBuffer_RS485_count; i++) TxBuffer_RS485[i] = outputPacket[i];
  start_transmint_data_via_RS_485(TxBuffer_RS485_count);
}//inputPacketParserRS485(void)

int outputFunc20PacketEncoder(int adrUnit, int *fileNumberArray, int *recordNumberArray, int *recordLenArray, int cntitem)
{
//выходной кодировщик 20 функции
  short dataRegister[130];
  int   idxDataRegister = 0;
  int idxOutputPacket = 0;
  unsigned int respDataLength = 0;
//adrUnit
  outputPacket[idxOutputPacket] = (unsigned char)adrUnit;
  idxOutputPacket++;
//numFunc
  idxOutputPacket++;
//Resp. Data length
  idxOutputPacket++;

  ulWorkNumber_rds = 0xffffffff;
  for(int item=0; item<cntitem; item++)  //блоки fileNumber, recordNumber, recordLen
    {
     int tmp1 = openRegistrator(fileNumberArray[item]);//открыть данные регистратора AR DR
     if (tmp1!=0) return tmp1;

//File resp. length
      respDataLength += outputPacket[idxOutputPacket] = (unsigned char)((1+recordLenArray[item]*2)&0xFF);
      idxOutputPacket++;
//Ref. Type
      outputPacket[idxOutputPacket] = 6;
      idxOutputPacket++;

      recordNumber_old = -1;

      for(; idxDataRegister<recordLenArray[item]; idxDataRegister++)
        {
          int result = superReader20(idxDataRegister, fileNumberArray[item], recordNumberArray[item], recordLenArray[item]);

          switch(result)
            {
            case MARKER_ERRORPERIMETR:
              return -1;
            default:
            {

              dataRegister[idxDataRegister] = (short) result;
              //        flag = 0;
            }
            }//switch
        }//for

      for(int i=0; i<idxDataRegister; i++)
        {
//Mdata
          outputPacket[idxOutputPacket] = (unsigned char)((dataRegister[i]>>8)&0xFF);
          idxOutputPacket++;
//Ldata
          outputPacket[idxOutputPacket] = (unsigned char)(dataRegister[i]&0xFF);
          idxOutputPacket++;
        }//for(int i=0; i<idxDataRegister; i++)
      idxDataRegister=0;
    }//for(int item=0; item<cntitem; item++)

//Resp. Data length
  outputPacket[2] = respDataLength+cntitem;
  return idxOutputPacket;
}//outputFunc20PacketEncoder(int adrUnit, int *fileNumber, int *recordNumber, int *recordLen, int cntitem)

/***********************************************************************************/
//Функція відповіді по протоколу Modbus-RTU на помилку
/***********************************************************************************/
int Error_modbus_m(unsigned int address, unsigned int function, unsigned int error, unsigned char *output_data)
{
  output_data[0] = address & 0xff;
  output_data[1] = 0x80 | (function & 0xff);
  output_data[2] = error & 0xff;
  return 3;
}

/***********************************************************************************/
//Додавання числа до CRC суми
/***********************************************************************************/
unsigned short AddCRC(unsigned char inpbyte, unsigned short oldCRC)
{
  unsigned short v,temp;
  int i;
  unsigned char bt,m;

  bt = inpbyte;
  v  = oldCRC;

  for (i=0; i<8; i++)
    {
      temp = v;
      v = v >> 1;
      m = bt >> i;
      if (((temp ^m) & 1)== 1) v = v ^ 0xa001;
    }
  return v;
}








