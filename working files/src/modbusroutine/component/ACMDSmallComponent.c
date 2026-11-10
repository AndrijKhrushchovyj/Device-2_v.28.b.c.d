
#include "header.h"
#include "variables_external_m.h"

//начальный регистр в карте памяти
#define BEGIN_ADR_REGISTER 500
//начальный bit в карте памяти
#define BEGIN_ADR_BIT 25000

//конечный регистр в карте памяти
#define END_ADR_REGISTER 611
//конечный bit в карте памяти
#define END_ADR_BIT 26790

int privateACMDSmallGetReg2(int adrReg);
int privateACMDSmallGetBit2(int adrBit);

int getACMDSmallModbusRegister(int);//получить содержимое регистра
int getACMDSmallModbusBit(int);//получить содержимое бита
int setACMDSmallModbusRegister(int, int);//записать регистр
int setACMDSmallModbusBit(int, int);//записать бит

int  postACMDSmallWriteAction(void);//action после записи

int  cmdFunc000(int inOffset, int *outMaska, int *dvMaska, int actControl);
void loadACMDSmallActualDataBit(int cmdSwitch, int beginOffset, int endOffset);
unsigned int encoderValidN_BIGACMD(int offsetCMD, int *validCMD, int actControl);
unsigned int encoderValidN_SMALLACMD(int offsetCMD, int *validCMD);
int validBazaN_BIGACMD(unsigned short dataReg, int actControl);
int writeACMDSmallActualDataBit(int offset, int dataBit);
int passwordImunitetBitACMDSmallComponent(int adrReg);
int start_ar_function(int actControl, int dataBit);
int clean_rejestrators_function(int actControl, int dataBit);
int activationFunction(int small_cmd, int actControl, int dataBit);
int clear_ar_function(void);
int clear_dr_function(void);
int getACMDSmallLength(void);

void decoderN_BIGACMDArrayLoader(void);
void decoderN_SMALLACMDArrayLoader(void);

SRAM1 unsigned short decoderN_BIGACMDArray[N_BIG*32];//массив декодирования битов N_BIG
SRAM1 unsigned short decoderN_SMALLACMDArray[N_SMALL*32];//массив декодирования битов N_SMALL

#define CLRACT_CONTROL  0

SRAM1 COMPONENT_OBJ *acmdsmallcomponent;
int cmdFunc000(int inOffset, int *outMaska, int *dvMaska, int actControl)
{
  int isValid = 1;
  (*outMaska) = -1;

  switch(inOffset)//адр команд карты памяти modbus
  {
  case 0:
    (*outMaska) = RANG_1_GRUPA_USTAVOK;
    (*dvMaska)  = RANG_SMALL_1_GRUPA_USTAVOK;
    break;
  case 1:
    (*outMaska) = RANG_2_GRUPA_USTAVOK;
    (*dvMaska)  = RANG_SMALL_2_GRUPA_USTAVOK;
    break;
  case 2:
    (*outMaska) = RANG_3_GRUPA_USTAVOK;
    (*dvMaska)  = RANG_SMALL_3_GRUPA_USTAVOK;
    break;
  case 3:
    (*outMaska) = RANG_4_GRUPA_USTAVOK;
    (*dvMaska)  = RANG_SMALL_4_GRUPA_USTAVOK;
    break;

#define  CMD_ITEM_HOLES3 if(item>3 && item<368) continue;

//  count_bit = 3;
#define UMIN_CONFIGURATION_BEGIN 368
  case UMIN_CONFIGURATION_BEGIN:
    (*outMaska) = RANG_BLOCK_UMIN1;
    (*dvMaska) = RANG_SMALL_BLOCK_UMIN1;
    break;

//  count_bit = 3;
  case 371:
    (*outMaska) = RANG_UMIN1;
//        (*dvMaska) =
    break;
  case 372:
    (*outMaska) = RANG_PO_UMIN1;
//        (*dvMaska) =
    break;

//  count_bit = 3;
  case 384:
    (*outMaska) = RANG_BLOCK_UMIN2;
    (*dvMaska)  = RANG_SMALL_BLOCK_UMIN2;
    break;

//  count_bit = 3;
  case 387:
    (*outMaska) = RANG_UMIN2;
//        (*dvMaska) =
    break;
#define UMIN_CONFIGURATION_END 388
  case UMIN_CONFIGURATION_END:
    (*outMaska) = RANG_PO_UMIN2;
//        (*dvMaska) =
    break;

#define  CMD_ITEM_HOLES388 if(item>388 && item<433) continue;

//  count_bit = 3;
#define UMAX_CONFIGURATION_BEGIN 433
  case UMAX_CONFIGURATION_BEGIN:
    (*outMaska) = RANG_PO_UMAX1;
//        (*dvMaska) =
    break;

//  count_bit = 3;
  case 435:
    (*outMaska) = RANG_PRYSK_UBAVYTY_VID_UMAX1;
//        (*dvMaska) =
    break;
  case 436:
    (*outMaska) = RANG_BLOCK_PRYBAVYTY_VID_UMAX1;
//        (*dvMaska) =
    break;

//  count_bit = 3;
  case 448:
    (*outMaska) = RANG_BLOCK_UMAX2;
    (*dvMaska)  = RANG_SMALL_BLOCK_UMAX2;
    break;
  case 449:
    (*outMaska) = RANG_PO_UMAX2;
//        (*dvMaska) =
    break;
#define UMAX_CONFIGURATION_END 450
  case UMAX_CONFIGURATION_END:
    (*outMaska) = RANG_UMAX2;
//        (*dvMaska) =
    break;

#define  CMD_ITEM_HOLES450 if(item>450 && item<896) continue;

//IF ВСТАВКА 896-919

#define  CMD_ITEM_HOLES919 if(item>919 && item<1296) continue;

//  count_bit = 8;
  case 1296:
    (*outMaska) = RANG_DEFECT;
//        (*dvMaska) =
    break;
  case 1297:
    (*outMaska) = RANG_AVAR_DEFECT;
//        (*dvMaska) =
    break;
#define  IMUNITET_BITACMD1298 1298
  case IMUNITET_BITACMD1298:
    (*outMaska) = RANG_RESET_LEDS;
    (*dvMaska) = RANG_SMALL_RESET_LEDS;
    break;
#define  IMUNITET_BITACMD1299 1299
  case IMUNITET_BITACMD1299:
    (*outMaska) = RANG_RESET_RELES;
    (*dvMaska) = RANG_SMALL_RESET_RELES;
    break;

#define  IMUNITET_BITACMD1300 1300

//  count_bit = 12;
  case 1301:
    (*outMaska) = RANG_SETTINGS_CHANGED;
//        (*dvMaska) =
    break;

#define  IMUNITET_BITACMD1302 1302

#define PASSWORD_SETCMD 1303

  case 1304:
    (*outMaska) = RANG_MISCEVE_DYSTANCIJNE;
    (*dvMaska) = RANG_SMALL_MISCEVE_DYSTANCIJNE;
    break;

  case 1305:
    (*outMaska) = RANG_WORK_A_REJESTRATOR;
//        (*dvMaska) =
    break;

//  count_bit = 1;
  case 1308:
    (*outMaska) = RANG_WORK_D_REJESTRATOR;
//        (*dvMaska) =
    break;

//  count_bit = 5;
  case 1318:
    (*outMaska) = RANG_OSNOVNYJ_TN2_RPN;
//        (*dvMaska) =
    break;

//  count_bit = 5;
  case 1327:
    (*outMaska) = RANG_BLK_GRUP_USTAVOK_VID_ZACHYSTIV;
//        (*dvMaska) =
    break;
  case 1328:
    (*outMaska) = RANG_INVERS_DV_GRUPA_USTAVOK;
//        (*dvMaska) =
    break;

  case 1335:
    (*outMaska) = RANG_DI_ACTIVE;
//        (*dvMaska) =
    break;
  case 1336:
    (*outMaska) = RANG_FK_ACTIVE;
//        (*dvMaska) =
    break;

  case 1339:
    (*outMaska) = RANG_USB_ACTIVE;
//        (*dvMaska) =
    break;
  case 1340:
    (*outMaska) = RANG_RS485_ACTIVE;
//        (*dvMaska) =
    break;

//  count_bit = 8;
#define EL_CONFIGURATION_BEGIN 1360
  case EL_CONFIGURATION_BEGIN:
    (*outMaska) = RANG_DF1_IN;
    (*dvMaska) = RANG_SMALL_DF1_IN;
    if(actControl==(1+SOURCEMARKER_MFT)) isValid = 0;
    break;
  case 1361:
    (*outMaska) = RANG_DF2_IN;
    (*dvMaska) = RANG_SMALL_DF2_IN;
    if(actControl==(2+SOURCEMARKER_MFT)) isValid = 0;
    break;
  case 1362:
    (*outMaska) = RANG_DF3_IN;
    (*dvMaska) = RANG_SMALL_DF3_IN;
    if(actControl==(3+SOURCEMARKER_MFT)) isValid = 0;
    break;
  case 1363:
    (*outMaska) = RANG_DF4_IN;
    (*dvMaska) = RANG_SMALL_DF4_IN;
    if(actControl==(4+SOURCEMARKER_MFT)) isValid = 0;
    break;
  case 1364:
    (*outMaska) = RANG_DF5_IN;
    (*dvMaska) = RANG_SMALL_DF5_IN;
    if(actControl==(5+SOURCEMARKER_MFT)) isValid = 0;
    break;
  case 1365:
    (*outMaska) = RANG_DF6_IN;
    (*dvMaska) = RANG_SMALL_DF6_IN;
    if(actControl==(6+SOURCEMARKER_MFT)) isValid = 0;
    break;
  case 1366:
    (*outMaska) = RANG_DF7_IN;
    (*dvMaska) = RANG_SMALL_DF7_IN;
    if(actControl==(7+SOURCEMARKER_MFT)) isValid = 0;
    break;
  case 1367:
    (*outMaska) = RANG_DF8_IN;
    (*dvMaska) = RANG_SMALL_DF8_IN;
    if(actControl==(8+SOURCEMARKER_MFT)) isValid = 0;
    break;

//  count_bit = 8;
  case 1376:
    (*outMaska) = RANG_DF1_R;
    (*dvMaska) = RANG_SMALL_DF1_R;
    if(actControl==(1+SOURCEMARKER_MFT)) isValid = 0;
    break;
  case 1377:
    (*outMaska) = RANG_DF2_R;
    (*dvMaska) = RANG_SMALL_DF2_R;
    if(actControl==(2+SOURCEMARKER_MFT)) isValid = 0;
    break;
  case 1378:
    (*outMaska) = RANG_DF3_R;
    (*dvMaska) = RANG_SMALL_DF3_R;
    if(actControl==(3+SOURCEMARKER_MFT)) isValid = 0;
    break;
  case 1379:
    (*outMaska) = RANG_DF4_R;
    (*dvMaska) = RANG_SMALL_DF4_R;
    if(actControl==(4+SOURCEMARKER_MFT)) isValid = 0;
    break;
  case 1380:
    (*outMaska) = RANG_DF5_R;
    (*dvMaska) = RANG_SMALL_DF5_R;
    if(actControl==(5+SOURCEMARKER_MFT)) isValid = 0;
    break;
  case 1381:
    (*outMaska) = RANG_DF6_R;
    (*dvMaska) = RANG_SMALL_DF6_R;
    if(actControl==(6+SOURCEMARKER_MFT)) isValid = 0;
    break;
  case 1382:
    (*outMaska) = RANG_DF7_R;
    (*dvMaska) = RANG_SMALL_DF7_R;
    if(actControl==(7+SOURCEMARKER_MFT)) isValid = 0;
    break;
  case 1383:
    (*outMaska) = RANG_DF8_R;
    (*dvMaska) = RANG_SMALL_DF8_R;
    if(actControl==(8+SOURCEMARKER_MFT)) isValid = 0;
    break;

//  count_bit = 8;
  case 1392:
    (*outMaska) = RANG_DF1_OUT;
//        (*dvMaska) =
    if(actControl==(1+SOURCEMARKER_MFT)) isValid = 0;
    break;
  case 1393:
    (*outMaska) = RANG_DF2_OUT;
//        (*dvMaska) =
    if(actControl==(2+SOURCEMARKER_MFT)) isValid = 0;
    break;
  case 1394:
    (*outMaska) = RANG_DF3_OUT;
//        (*dvMaska) =
    if(actControl==(3+SOURCEMARKER_MFT)) isValid = 0;
    break;
  case 1395:
    (*outMaska) = RANG_DF4_OUT;
//        (*dvMaska) =
    if(actControl==(4+SOURCEMARKER_MFT)) isValid = 0;
    break;
  case 1396:
    (*outMaska) = RANG_DF5_OUT;
//        (*dvMaska) =
    if(actControl==(5+SOURCEMARKER_MFT)) isValid = 0;
    break;
  case 1397:
    (*outMaska) = RANG_DF6_OUT;
//        (*dvMaska) =
    if(actControl==(6+SOURCEMARKER_MFT)) isValid = 0;
    break;
  case 1398:
    (*outMaska) = RANG_DF7_OUT;
//        (*dvMaska) =
    if(actControl==(7+SOURCEMARKER_MFT)) isValid = 0;
    break;
  case 1399:
    (*outMaska) = RANG_DF8_OUT;
//        (*dvMaska) =
    if(actControl==(8+SOURCEMARKER_MFT)) isValid = 0;
    break;

//  count_bit = 12;
  case 1408:
    (*outMaska) = RANG_DT1_SET;
    (*dvMaska) = RANG_SMALL_DT1_SET;
    if(actControl==(1+SOURCEMARKER_DTR)) isValid = 0;
    break;
  case 1409:
    (*outMaska) = RANG_DT2_SET;
    (*dvMaska) = RANG_SMALL_DT2_SET;
    if(actControl==(2+SOURCEMARKER_DTR)) isValid = 0;
    break;
  case 1410:
    (*outMaska) = RANG_DT3_SET;
    (*dvMaska) = RANG_SMALL_DT3_SET;
    if(actControl==(3+SOURCEMARKER_DTR)) isValid = 0;
    break;
  case 1411:
    (*outMaska) = RANG_DT4_SET;
    (*dvMaska) = RANG_SMALL_DT4_SET;
    if(actControl==(4+SOURCEMARKER_DTR)) isValid = 0;
    break;

//  count_bit = 12;
  case 1424:
    (*outMaska) = RANG_DT1_RESET;
    (*dvMaska) = RANG_SMALL_DT1_RESET;
    if(actControl==(1+SOURCEMARKER_DTR)) isValid = 0;
    break;
  case 1425:
    (*outMaska) = RANG_DT2_RESET;
    (*dvMaska) = RANG_SMALL_DT2_RESET;
    if(actControl==(2+SOURCEMARKER_DTR)) isValid = 0;
    break;
  case 1426:
    (*outMaska) = RANG_DT3_RESET;
    (*dvMaska) = RANG_SMALL_DT3_RESET;
    if(actControl==(3+SOURCEMARKER_DTR)) isValid = 0;
    break;
  case 1427:
    (*outMaska) = RANG_DT4_RESET;
    (*dvMaska) = RANG_SMALL_DT4_RESET;
    if(actControl==(4+SOURCEMARKER_DTR)) isValid = 0;
    break;

//  count_bit = 12;
  case 1440:
    (*outMaska) = RANG_DT1_OUT;
//        (*dvMaska) =
    if(actControl==(1+SOURCEMARKER_DTR)) isValid = 0;
    break;
  case 1441:
    (*outMaska) = RANG_DT2_OUT;
//        (*dvMaska) =
    if(actControl==(2+SOURCEMARKER_DTR)) isValid = 0;
    break;
  case 1442:
    (*outMaska) = RANG_DT3_OUT;
//        (*dvMaska) =
    if(actControl==(3+SOURCEMARKER_DTR)) isValid = 0;
    break;
  case 1443:
    (*outMaska) = RANG_DT4_OUT;
//        (*dvMaska) =
    if(actControl==(4+SOURCEMARKER_DTR)) isValid = 0;
    break;

//  count_bit = 40;
  case 1456:
    (*outMaska) = RANG_D_AND1;
//        (*dvMaska) =
    break;
  case 1457:
    (*outMaska) = RANG_D_AND2;
//        (*dvMaska) =
    break;
  case 1458:
    (*outMaska) = RANG_D_AND3;
//        (*dvMaska) =
    break;
  case 1459:
    (*outMaska) = RANG_D_AND4;
//        (*dvMaska) =
    break;
  case 1460:
    (*outMaska) = RANG_D_AND5;
//        (*dvMaska) =
    break;
  case 1461:
    (*outMaska) = RANG_D_AND6;
//        (*dvMaska) =
    break;
  case 1462:
    (*outMaska) = RANG_D_AND7;
//        (*dvMaska) =
    break;
  case 1463:
    (*outMaska) = RANG_D_AND8;
//        (*dvMaska) =
    break;

//  count_bit = 40;
  case 1472:
    (*outMaska) = RANG_D_OR1;
//        (*dvMaska) =
    break;
  case 1473:
    (*outMaska) = RANG_D_OR2;
//        (*dvMaska) =
    break;
  case 1474:
    (*outMaska) = RANG_D_OR3;
//        (*dvMaska) =
    break;
  case 1475:
    (*outMaska) = RANG_D_OR4;
//        (*dvMaska) =
    break;
  case 1476:
    (*outMaska) = RANG_D_OR5;
//        (*dvMaska) =
    break;
  case 1477:
    (*outMaska) = RANG_D_OR6;
//        (*dvMaska) =
    break;
  case 1478:
    (*outMaska) = RANG_D_OR7;
//        (*dvMaska) =
    break;
  case 1479:
    (*outMaska) = RANG_D_OR8;
//        (*dvMaska) =
    break;

//  count_bit = 40;
  case 1488:
    (*outMaska) = RANG_D_NOT1;
//        (*dvMaska) =
    break;
  case 1489:
    (*outMaska) = RANG_D_NOT2;
//        (*dvMaska) =
    break;
  case 1490:
    (*outMaska) = RANG_D_NOT3;
//        (*dvMaska) =
    break;
  case 1491:
    (*outMaska) = RANG_D_NOT4;
//        (*dvMaska) =
    break;
  case 1492:
    (*outMaska) = RANG_D_NOT5;
//        (*dvMaska) =
    break;
  case 1493:
    (*outMaska) = RANG_D_NOT6;
//        (*dvMaska) =
    break;
  case 1494:
    (*outMaska) = RANG_D_NOT7;
//        (*dvMaska) =
    break;
  case 1495:
    (*outMaska) = RANG_D_NOT8;
//        (*dvMaska) =
    break;
  case 1496:
    (*outMaska) = RANG_D_NOT9;
//        (*dvMaska) =
    break;
  case 1497:
    (*outMaska) = RANG_D_NOT10;
//        (*dvMaska) =
    break;
  case 1498:
    (*outMaska) = RANG_D_NOT11;
//        (*dvMaska) =
    break;
  case 1499:
    (*outMaska) = RANG_D_NOT12;
//        (*dvMaska) =
    break;
  case 1500:
    (*outMaska) = RANG_D_NOT13;
//        (*dvMaska) =
    break;
  case 1501:
    (*outMaska) = RANG_D_NOT14;
//        (*dvMaska) =
    break;
  case 1502:
    (*outMaska) = RANG_D_NOT15;
//        (*dvMaska) =
    break;
  case 1503:
    (*outMaska) = RANG_D_NOT16;
//        (*dvMaska) =
    break;

//  count_bit = 40;
  case 1504:
    (*outMaska) = RANG_D_XOR1;
//        (*dvMaska) =
    break;
  case 1505:
    (*outMaska) = RANG_D_XOR2;
//        (*dvMaska) =
    break;
  case 1506:
    (*outMaska) = RANG_D_XOR3;
//        (*dvMaska) =
    break;
  case 1507:
    (*outMaska) = RANG_D_XOR4;
//        (*dvMaska) =
    break;
  case 1508:
    (*outMaska) = RANG_D_XOR5;
//        (*dvMaska) =
    break;
  case 1509:
    (*outMaska) = RANG_D_XOR6;
//        (*dvMaska) =
    break;
  case 1510:
    (*outMaska) = RANG_D_XOR7;
//        (*dvMaska) =
    break;
#define EL_CONFIGURATION_END 1511
  case EL_CONFIGURATION_END:
    (*outMaska) = RANG_D_XOR8;
//        (*dvMaska) =
    break;

//  count_bit = 3;
#define LF_CONFIGURATION_BEGIN 1568
  case LF_CONFIGURATION_BEGIN:
    (*outMaska) = RANG_LF1;
    (*dvMaska) = RANG_SMALL_LF1;
    break;
  case 1569:
    (*outMaska) = RANG_LF2;
    (*dvMaska) = RANG_SMALL_LF2;
    break;
  case 1570:
    (*outMaska) = RANG_LF3;
    (*dvMaska) = RANG_SMALL_LF3;
    break;
  case 1571:
    (*outMaska) = RANG_LF4;
    (*dvMaska) = RANG_SMALL_LF4;
    break;
  case 1572:
    (*outMaska) = RANG_LF5;
    (*dvMaska) = RANG_SMALL_LF5;
    break;
  case 1573:
    (*outMaska) = RANG_LF6;
    (*dvMaska) = RANG_SMALL_LF6;
    break;
  case 1574:
    (*outMaska) = RANG_LF7;
    (*dvMaska) = RANG_SMALL_LF7;
    break;
  case 1575:
    (*outMaska) = RANG_LF8;
    (*dvMaska) = RANG_SMALL_LF8;
    break;
  case 1576:
    (*outMaska) = RANG_LF9;
    (*dvMaska) = RANG_SMALL_LF9;
    break;
  case 1577:
    (*outMaska) = RANG_LF10;
    (*dvMaska) = RANG_SMALL_LF10;
    break;
  case 1578:
    (*outMaska) = RANG_LF11;
    (*dvMaska) = RANG_SMALL_LF11;
    break;
  case 1579:
    (*outMaska) = RANG_LF12;
    (*dvMaska) = RANG_SMALL_LF12;
    break;
  case 1580:
    (*outMaska) = RANG_LF13;
    (*dvMaska) = RANG_SMALL_LF13;
    break;
  case 1581:
    (*outMaska) = RANG_LF14;
    (*dvMaska) = RANG_SMALL_LF14;
    break;
  case 1582:
    (*outMaska) = RANG_LF15;
    (*dvMaska) = RANG_SMALL_LF15;
    break;
#define LF_CONFIGURATION_END 1583
  case LF_CONFIGURATION_END:
    (*outMaska) = RANG_LF16;
    (*dvMaska) = RANG_SMALL_LF16;
    break;

#define  CMD_ITEM_HOLES1583 if(item>1583 && item<1728) continue;

#define RPN_CONFIGURATION_BEGIN 1728
  case RPN_CONFIGURATION_BEGIN:
    (*outMaska) = RANG_BLOCKUVANNJA_RPN;
//    (*dvMaska) = ;
    break;
#define  BITACMD1729 1729
  case BITACMD1729:
    (*outMaska) = RANG_CLEAR_BLK_RPN;
    (*dvMaska)  = RANG_SMALL_CLEAR_BLK_RPN;
    break;
  case 1730:
    (*outMaska) = RANG_BLOCK_STRUM_KOMP_RPN;
    (*dvMaska) = RANG_SMALL_BLOCK_STRUM_KOMP_RPN;
    break;
  case 1731:
    (*outMaska) = RANG_PEREKLYUCHENNYA_RPN;
    (*dvMaska)  = RANG_SMALL_PEREKLYUCHENNYA_RPN;
    break;
  case 1732:
    (*outMaska) = RANG_1_POLOGENNJA_RPN;
    (*dvMaska)  = RANG_SMALL_1_POLOGENNJA_RPN;
    break;
  case 1733:
    (*outMaska) = RANG_NOMINALNA_POZYCIJA_RPN;
    (*dvMaska)  = RANG_SMALL_NOMINALNA_POZYCIJA_RPN;
    break;
  case 1734:
    (*outMaska) = RANG_N_POLOGENNJA_RPN;
    (*dvMaska)  = RANG_SMALL_N_POLOGENNJA_RPN;
    break;
  case 1735:
    (*outMaska) = RANG_NESPRAVNIST_RPN;
//    (*dvMaska) = ;
    break;
  case 1736:
    (*outMaska) = RANG_RESET_DEFECT_RPN;
//    (*dvMaska) = ;
    break;
  case 1737:
    (*outMaska) = RANG_ZOVNISHNJE_BLOCKUVANNJA_RPN;
    (*dvMaska)  = RANG_SMALL_ZOVNISHNJE_BLOCKUVANNJA_RPN;
    break;
  case 1738:
    (*outMaska) = RANG_PO1_U_OSN_RPN;
//    (*dvMaska) = ;
    break;
  case 1739:
    (*outMaska) = RANG_PO1_U_OSN_KOMP_RPN;
//    (*dvMaska) = ;
    break;
  case 1740:
    (*outMaska) = RANG_PO1_U_VSPOM_RPN;
//    (*dvMaska) = ;
    break;
  case 1741:
    (*outMaska) = RANG_PO2_U_OSN_RPN;
//    (*dvMaska) = ;
    break;
  case 1742:
    (*outMaska) = RANG_PO2_U_OSN_KOMP_RPN;
//    (*dvMaska) = ;
    break;
  case 1743:
    (*outMaska) = RANG_PO3_U_OSN_RPN;
//    (*dvMaska) = ;
    break;
  case 1744:
    (*outMaska) = RANG_PO3_U_VSPOM_RPN;
//    (*dvMaska) = ;
    break;
  case 1745:
    (*outMaska) = RANG_PO4_U_OSN_RPN;
//    (*dvMaska) = ;
    break;
  case 1746:
    (*outMaska) = RANG_UBAVYTY_1_RPN;
//    (*dvMaska) = ;
    break;
  case 1747:
    (*outMaska) = RANG_UBAVYTY_RPN;
//    (*dvMaska) = ;
    break;
  case 1748:
    (*outMaska) = RANG_UBAVYTY_T_RPN;
//    (*dvMaska) = ;
    break;
  case 1749:
    (*outMaska) = RANG_UBAVYTY_DI_RPN;
//    (*dvMaska) = ;
    break;
  case 1750:
    (*outMaska) = RANG_UBAVYTY_R_RPN;
//    (*dvMaska) = ;
    break;
  case 1751:
    (*outMaska) = RANG_UBAVYTY_FROM_KB_RPN;
//    (*dvMaska) = ;
    break;
#define  BITACMD1752 1752
  case BITACMD1752:
    (*outMaska) = RANG_UBAVYTY_FROM_UPPER_LEVEL_RPN;
//    (*dvMaska) = ;
    break;
  case 1753:
    (*outMaska) = RANG_UBAVYTY_FROM_DV_RPN;
    (*dvMaska)  = RANG_SMALL_UBAVYTY_FROM_DV_RPN;
    break;
  case 1754:
    (*outMaska) = RANG_PRYBAVYTY_1_RPN;
//    (*dvMaska) = ;
    break;
  case 1755:
    (*outMaska) = RANG_PRYBAVYTY_RPN;
//    (*dvMaska) = ;
    break;
  case 1756:
    (*outMaska) = RANG_PRYBAVYTY_T_RPN;
//    (*dvMaska) = ;
    break;
  case 1757:
    (*outMaska) = RANG_PRYBAVYTY_DI_RPN;
//    (*dvMaska) = ;
    break;
  case 1758:
    (*outMaska) = RANG_PRYBAVYTY_R_RPN;
//    (*dvMaska) = ;
    break;
  case 1759:
    (*outMaska) = RANG_PRYBAVYTY_FROM_KB_RPN;
//    (*dvMaska) = ;
    break;
#define  BITACMD1760 1760
  case BITACMD1760:
    (*outMaska) = RANG_PRYBAVYTY_FROM_UPPER_LEVEL_RPN;
//    (*dvMaska) = ;
    break;
  case 1761:
    (*outMaska) = RANG_PRYBAVYTY_FROM_DV_RPN;
    (*dvMaska)  = RANG_SMALL_PRYBAVYTY_FROM_DV_RPN;
    break;
  case 1762:
    (*outMaska) = RANG_DOZVIL_UBAVYTY_RPN;
//    (*dvMaska) = ;
    break;
  case 1763:
    (*outMaska) = RANG_DOZVIL_AVTO_UBAVYTY_RPN;
//    (*dvMaska) = ;
    break;
  case 1764:
    (*outMaska) = RANG_DOZVIL_PRYBAVYTY_RPN;
//    (*dvMaska) = ;
    break;
  case 1765:
    (*outMaska) = RANG_DOZVIL_AVTO_PRYBAVYTY_RPN;
//    (*dvMaska) = ;
    break;
  case 1766:
    (*outMaska) = RANG_ZASTRJAVANNJA_RPN;
//    (*dvMaska) = ;
    break;
  case 1767:
    (*outMaska) = RANG_N_PER_RIVNE_N_PER_USTAVKY_RPN;
//    (*dvMaska) = ;
    break;
  case 1768:
    (*outMaska) = RANG_PRYVID_NE_PISHOV_RPN;
//    (*dvMaska) = ;
    break;
  case 1769:
    (*outMaska) = RANG_VYJCHE_ZONY_RPN;
//    (*dvMaska) = ;
    break;
  case 1770:
    (*outMaska) = RANG_NYJCHE_ZONY_RPN;
//    (*dvMaska) = ;
    break;

#define  BITACMD1772 1772

  case 1773:
    (*outMaska) = RANG_AUTO_MODE_RPN;
//    (*dvMaska) = ;
    break;
  case 1774:
    (*outMaska) = RANG_MRZS_OR_LOCAL_MODE_RPN;
    (*dvMaska)  = RANG_SMALL_MRZS_OR_LOCAL_MODE_RPN;
    break;
  case 1775:
    (*outMaska) = RANG_REMOTE_MODE_RPN;
//    (*dvMaska) = ;
    break;
#define  BITACMD1776 1776
  case BITACMD1776:
    (*outMaska) = RANG_REMOTE_MODE_FROM_UPPER_LEVEL_RPN;
//    (*dvMaska) = ;
    break;
#define  BITACMD1777 1777
  case BITACMD1777:
    (*outMaska) = RANG_AUTO_MODE_FROM_UPPER_LEVEL_RPN;
//    (*dvMaska) = ;
    break;
  case 1778:
    (*outMaska) = RANG_TM_MODE_VID_DV_RPN;
    (*dvMaska) = RANG_SMALL_TM_MODE_VID_DV_RPN;
    break;
  case 1779:
    (*outMaska) = RANG_AUTO_MODE_FROM_DV_RPN;
    (*dvMaska)  = RANG_SMALL_AUTO_MODE_FROM_DV_RPN;
    break;
#define RPN_CONFIGURATION_END 1780
  case RPN_CONFIGURATION_END:
    (*outMaska) = RANG_TM_MODE_RPN;
    (*dvMaska)  = RANG_SMALL_TM_MODE_VID_DV_RPN;
    break;

#define ZSX_CONFIGURATION_BEGIN 1781
  case ZSX_CONFIGURATION_BEGIN:
    (*outMaska) = RANG_KONTROL_UBAVYTY_ZSKh;
    (*dvMaska)  = RANG_SMALL_KONTROL_UBAVYTY_ZSKh;
    break;
  case 1782:
    (*outMaska) = RANG_KONTROL_PRYBAVYTY_ZSKh;
    (*dvMaska)  = RANG_SMALL_KONTROL_PRYBAVYTY_ZSKh;
    break;
#define ZSX_CONFIGURATION_END 1783
  case ZSX_CONFIGURATION_END:
    (*outMaska) = RANG_ZSKh;
//    (*dvMaska) = ;
    break;

#define ZNX_CONFIGURATION_BEGIN 1785
  case ZNX_CONFIGURATION_BEGIN:
    (*outMaska) = RANG_ZNKh;
//    (*dvMaska) = ;
    break;
  case 1786:
    (*outMaska) = RANG_ZNKh_IR;
//    (*dvMaska) = ;
    break;
#define ZNX_CONFIGURATION_END 1787
  case ZNX_CONFIGURATION_END:
    (*outMaska) = RANG_ZNKh_BR;
//    (*dvMaska) = ;
    break;

#define BRP_CONFIGURATION_BEGIN 1789
  case 1789:
    (*outMaska) = RANG_BLOCK_BRP;
    (*dvMaska)  = RANG_SMALL_BLOCK_BRP;
    break;
#define BRP_CONFIGURATION_END 1790
  case BRP_CONFIGURATION_END:
    (*outMaska) = RANG_BRP;
//    (*dvMaska) = ;
    break;
  }//switch

  if((*outMaska)!=-1)
  {
    if(inOffset>=LF_CONFIGURATION_BEGIN && inOffset<=LF_CONFIGURATION_END)
      if(!(edition_settings.configuration&(1<<EL_BIT_CONFIGURATION))) isValid = 0;

    if(inOffset>=RPN_CONFIGURATION_BEGIN && inOffset<=RPN_CONFIGURATION_END)
      if(!(edition_settings.configuration&(1<<RPN_BIT_CONFIGURATION))) isValid = 0;
    if(inOffset>=ZSX_CONFIGURATION_BEGIN && inOffset<=ZSX_CONFIGURATION_END)
      if(!(edition_settings.configuration&(1<<ZSKh_BIT_CONFIGURATION))) isValid = 0;
    if(inOffset>=ZNX_CONFIGURATION_BEGIN && inOffset<=ZNX_CONFIGURATION_END)
      if(!(edition_settings.configuration&(1<<ZNKh_BIT_CONFIGURATION))) isValid = 0;
    if(inOffset>=BRP_CONFIGURATION_BEGIN && inOffset<=BRP_CONFIGURATION_END)
      if(!(edition_settings.configuration&(1<<BRP_BIT_CONFIGURATION))) isValid = 0;
    if(inOffset>=UMIN_CONFIGURATION_BEGIN && inOffset<=UMIN_CONFIGURATION_END)
      if(!(edition_settings.configuration&(1<<Umin_BIT_CONFIGURATION))) isValid = 0;
    if(inOffset>=UMAX_CONFIGURATION_BEGIN && inOffset<=UMAX_CONFIGURATION_END)
      if(!(edition_settings.configuration&(1<<Umax_BIT_CONFIGURATION))) isValid = 0;
    if(inOffset>=EL_CONFIGURATION_BEGIN && inOffset<=EL_CONFIGURATION_END)
      if(!(edition_settings.configuration&(1<<EL_BIT_CONFIGURATION))) isValid = 0;
  }//if((*outMaska)!=-1)
//IF ВСТАВКА 896-919
  if(inOffset>=896 && inOffset<920)
  {
    if(!(edition_settings.configuration&(1<<UP_BIT_CONFIGURATION))) isValid = 0;
    int inupOffset = inOffset-896;
    int subObj = inupOffset/3;
    int offsetObj = inupOffset%3;

    switch(offsetObj)
    {
    case 0:
      (*outMaska) = RANG_BLOCK_UP1 + 3*subObj;
      (*dvMaska)  = RANG_SMALL_BLOCK_UP1 + 1*subObj;
      break;
    case 1:
      (*outMaska) = RANG_PO_UP1 + 3*subObj;
//        (*dvMaska) =
      break;
    case 2:
      (*outMaska) = RANG_UP1 + 3*subObj;
//        (*dvMaska) =
      break;
    }//switch
  }//if

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
  if(inOffset>=1520 && inOffset<1536)
  {
    int ingoosOffset = inOffset-1520;
    (*outMaska) = RANG_BLOCK_IN_GOOSE1 + ingoosOffset;
    (*dvMaska)  = RANG_SMALL_BLOCK_IN_GOOSE1 + ingoosOffset;
  }//if(inOffset>=1520 && inOffset<1536)
  else if(inOffset>=1536 && inOffset<1540)
  {
    int inmmsOffset = inOffset-1536;
    (*outMaska) = RANG_BLOCK_IN_MMS1 + inmmsOffset;
    (*dvMaska)  = RANG_SMALL_BLOCK_IN_MMS1 + inmmsOffset;
  }//if(inOffset>=1536 && inOffset<1540)
  else if(inOffset>=1552 && inOffset<1556)
  {
    int inlanOffset = inOffset-1552;
    (*outMaska) = RANG_BLOCK_OUT_LAN1 + inlanOffset;
    (*dvMaska)  = RANG_SMALL_BLOCK_OUT_LAN1 + inlanOffset;
  }//if(inOffset>=1552 && inOffset<1556)
#endif

  return isValid;
}//cmdFunc000(int inOffset, int *outMaska, int *dvMaska)

int getACMDSmallBeginAdr(void)
{
  return BEGIN_ADR_BIT;
}//getACMDSmallBeginAdr(void)
int getACMDSmallLength(void)
{
  return END_ADR_REGISTER - BEGIN_ADR_REGISTER + 1;
}//getACMDSmallLength(void)
/**************************************/
//компонент Текущие активные и сработавшие функции
/**************************************/
void constructorACMDSmallComponent(COMPONENT_OBJ *acmdcomp)
{
  acmdsmallcomponent = acmdcomp;

//  acmdsmallcomponent->getModbusRegister = getACMDSmallModbusRegister;//получить содержимое регистра
//  acmdsmallcomponent->getModbusBit      = getACMDSmallModbusBit;//получить содержимое бита
//  acmdsmallcomponent->setModbusRegister = setACMDSmallModbusRegister;// регистра
//  acmdsmallcomponent->setModbusBit      = setACMDSmallModbusBit;// бита

//  acmdsmallcomponent->postWriteAction = postACMDSmallWriteAction;//action после записи

}//constructorDOUTSmallComponent(COMPONENT_OBJ *doutcomp)

void decoderN_BIGACMDArrayLoader(void)
{
//декодировщик индекса бита в адрес modbus  для реле
  static int idxBit_decoderN_BIGACMDArrayLoader, idxBit_decoderN_BIGACMDArrayLoader_prev;
  if(idxBit_decoderN_BIGACMDArrayLoader>=N_BIG*32) return;

  int outMaska = -1;
  int dvMaska = -1;
  int iter_diff = 0;
  decoderN_BIGACMDArray[idxBit_decoderN_BIGACMDArrayLoader] = 0xFFFF;
  for(int item=0; item<(END_ADR_BIT-BEGIN_ADR_BIT+1); item++)
  {
    CMD_ITEM_HOLES3 
    CMD_ITEM_HOLES388 
    CMD_ITEM_HOLES450 
    CMD_ITEM_HOLES919 
    CMD_ITEM_HOLES1583 

    cmdFunc000(item, &outMaska, &dvMaska, CLRACT_CONTROL);

    if(outMaska==idxBit_decoderN_BIGACMDArrayLoader)
    {
      decoderN_BIGACMDArray[idxBit_decoderN_BIGACMDArrayLoader] = (unsigned short) item;
      break;
    }//if
    iter_diff = idxBit_decoderN_BIGACMDArrayLoader - idxBit_decoderN_BIGACMDArrayLoader_prev;
    if( iter_diff > 3  ){
      idxBit_decoderN_BIGACMDArrayLoader_prev = idxBit_decoderN_BIGACMDArrayLoader;
//      watchdog_routine(UNITED_BITS_WATCHDOG);
    }
  }//for
  idxBit_decoderN_BIGACMDArrayLoader++;
  
}//decoderN_BIGACMDArrayLoader()

void decoderN_SMALLACMDArrayLoader(void)
{
//декодировщик индекса бита в адрес modbus  для ДВ
  static int idxBit_decoderN_SMALLACMDArrayLoader,idxBit_decoderN_SMALLACMDArrayLoader_prev = 0;
  if(idxBit_decoderN_SMALLACMDArrayLoader>=N_SMALL*32) return;

  int outMaska=-1;
  int dvMaska=-1;
  int iter_diff = 0;

  decoderN_SMALLACMDArray[idxBit_decoderN_SMALLACMDArrayLoader] = 0xFFFF;

  for(int item=0; item<(END_ADR_BIT-BEGIN_ADR_BIT+1); item++)
  {
    CMD_ITEM_HOLES3 
    CMD_ITEM_HOLES388 
    CMD_ITEM_HOLES450 
    CMD_ITEM_HOLES919 
    CMD_ITEM_HOLES1583 

    cmdFunc000(item, &outMaska, &dvMaska, CLRACT_CONTROL);
    if(dvMaska==idxBit_decoderN_SMALLACMDArrayLoader)
    {
      decoderN_SMALLACMDArray[idxBit_decoderN_SMALLACMDArrayLoader] = (unsigned short) item;
      break;
    }//if
    iter_diff = idxBit_decoderN_SMALLACMDArrayLoader - idxBit_decoderN_SMALLACMDArrayLoader_prev;
    if( iter_diff > 4  ){
      idxBit_decoderN_SMALLACMDArrayLoader_prev = idxBit_decoderN_SMALLACMDArrayLoader;
//      watchdog_routine(UNITED_BITS_WATCHDOG);
    }
  }//for
  idxBit_decoderN_SMALLACMDArrayLoader++;
}//decoderN_SMALLACMDArrayLoader()

int getRangN_BIGModbusRegister2021(unsigned int *ranguvannja_d, int offset )
{
//поиск активного бита
  int bit = getSequenceN_BIGIndexActiveBit(offset, ranguvannja_d);//индекс активного бита
  if(bit!=-1)
  {
    int adr = decoderN_BIGACMD(bit);
    return adr<0? 0: adr+getACMDSmallBeginAdr();//декодировщик индекса бита в адрес modbus
  }//if(bit!=-1)
  return 0;
}//getRangN_BIGModbusRegister2021(unsigned int *ranguvannja_d, int offset )

void writeRangN_BIGModbusRegister2021(unsigned int *ranguvannja_d, unsigned short *dataPacket, int countAdr)
{
  unsigned int tmp[N_BIG];
  int idxObjOld = -1;
  //убрать старое
  for(int i=0; i<countAdr; i++)
  {
    if(idxObjOld != 0)
    {
      idxObjOld = 0;
      for(int dx=0; dx<N_BIG; dx++) tmp[dx]=ranguvannja_d[dx];
    }//if

    int bitOld = getSequenceN_BIGIndexActiveBit(i, tmp);
    if(bitOld != -1) {
      ranguvannja_d[bitOld/32] &= ~(1<<(bitOld%32));
    }//if
  }//for

  //добавить новое
  for(int i=0; i<countAdr; i++)
  {
    unsigned int adr = (unsigned short) swapByteInShort(dataPacket[i]);//новое значение
    if(adr == 0) continue;
    int bit = encoderN_BIGACMD(adr-getACMDSmallBeginAdr()); //кодировщик адреса modbus в индекс бита для реле
    if(bit != -1)
    {
      ranguvannja_d[bit/32] |= (1<<(bit%32));
    }//if
  }//for
}//writeRangN_BIGModbusRegister2021

int getRangN_SMALLModbusRegister2021(unsigned int *ranguvannja_d, int offset )
{
//поиск активного бита
  int bit = getSequenceN_SMALLIndexActiveBit(offset, ranguvannja_d);//индекс активного бита
  if(bit!=-1)
  {
    int adr = decoderN_SMALLACMD(bit);
    return adr<0? 0: adr+getACMDSmallBeginAdr();//декодировщик индекса бита в адрес modbus
  }//if(bit!=-1)
  return 0;
}//getRangN_SMALLModbusRegister2021(unsigned int *ranguvannja_d, int offset )

void writeRangN_SMALLModbusRegister2021(unsigned int *ranguvannja_d, unsigned short *dataPacket, int countAdr)
{
  unsigned int tmp[N_SMALL];
  int idxObjOld = -1;
  //убрать старое
  for(int i=0; i<countAdr; i++)
  {
    if(idxObjOld != 0)
    {
      idxObjOld = 0;
      for(int dx=0; dx<N_SMALL; dx++) tmp[dx]=ranguvannja_d[dx];
    }//if

    int bitOld = getSequenceN_SMALLIndexActiveBit(i, tmp);
    if(bitOld != -1) {
      ranguvannja_d[bitOld/32] &= ~(1<<(bitOld%32));
    }//if
  }//for

  //добавить новое
  for(int i=0; i<countAdr; i++)
  {
    unsigned int adr = (unsigned short) swapByteInShort(dataPacket[i]);//новое значение
    if(adr == 0) continue;
    int bit = encoderN_SMALLACMD(adr-getACMDSmallBeginAdr()); //кодировщик адреса modbus в индекс бита для реле
    if(bit != -1)
    {
      ranguvannja_d[bit/32] |= (1<<(bit%32));
    }//if
  }//for
}//writeRangN_SMALLModbusRegister2021
#pragma optimize=medium//..medium 
void loadACMDSmallActualDataBit(int cmdSwitch, int beginOffset, int endOffset)
{
  //ActualData
  for(int ii=0; ii<(1+(END_ADR_BIT-BEGIN_ADR_BIT+1)/16); ii++) tempReadArray[ii] = 0;
  for(int item=0; item<(END_ADR_BIT-BEGIN_ADR_BIT+1); item++)
  {
    int value =0;
    int tsdata = 0;
    if(item>=beginOffset && item<endOffset)
    {
      switch(item)
      {
      case PASSWORD_SETCMD://Пароль установлен
      {
        switch(pointInterface)//метка интерфейса 0-USB 1-RS485
        {
        case USB_RECUEST:
          if(cmdSwitch==0) //ACMD
            value = password_set_USB;//Пароль установлен
          break;
        case RS485_RECUEST:
          if(cmdSwitch==0) //ACMD
            value = password_set_RS485;//Пароль установлен
          break;
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
        case LAN_RECUEST:
          if(cmdSwitch==0) //ACMD
            value = password_set_LAN;//Пароль установлен
          break;
#endif
        }//switch(pointInterface)
        goto m1;
      }//case PASSWORD_SETCMD://Пароль установлен
      }//switch(item)
      value = encoderN_BIGACMD(item);//кодировщик адреса modbus в индекс бита для реле
      if(value==-1) value=0;
      else {
        //читать значение команд
        if(cmdSwitch==0) {
          //ACMD
          value = active_functions[value/32] & (1<<(value%32));
        }//if(cmdSwitch==0)
        else if(cmdSwitch==1) {
          //GCMD
          if(pointInterface==USB_RECUEST)//метка интерфейса 0-USB 1-RS485
          {
            value = trigger_functions_USB[value/32] & (1<<(value%32));
          }//if
          else if(pointInterface==RS485_RECUEST)
          {
            value = trigger_functions_RS485[value/32] & (1<<(value%32));
          }//else
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
          else if(pointInterface==LAN_RECUEST)
          {
            value = trigger_functions_LAN[value/32] & (1<<(value%32));
          }//else
#endif
          else
          {
            //Теоретично цього ніколи не мало б бути
            total_error_sw_fixed();
          }
        }//if(cmdSwitch==1)
        else if(cmdSwitch==2) {
          //RDS
          value = rds_functions[value/32] & (1<<(value%32));
        }//if(cmdSwitch==2)

      }
    }//if(item>=beginOffset && item<endOffset)
m1:
    if(value) tsdata=1;
    tempReadArray[item/16] |= tsdata<<(item%16);
//    if( (item&(1<<9)) && ( (item&(~(1<<9))) == 0 ) ){
//      watchdog_routine(UNITED_BITS_WATCHDOG);
//    }
  }//for(int item=0; item<568; item++)
}//loadACMDSmallActualDataBit(int cmdSwitch, int beginOffset, int endOffset)

int validN_BIGACMD(unsigned short dataReg)
{
  return validBazaN_BIGACMD(dataReg, CLRACT_CONTROL);
}//validN_BIGACMD(unsigned short dataReg)
int validBazaN_BIGACMD(unsigned short dataReg, int actControl)
{
//расширенная проверка адреса команды for rele
  if(dataReg!=0)
  {
    int validCMD=0;
    if(((int)encoderValidN_BIGACMD(dataReg-getACMDSmallBeginAdr(), &validCMD, actControl))<0) return MARKER_ERRORPERIMETR; //кодировщик адреса modbus в индекс бита для реле
    if(!validCMD) return MARKER_ERRORPERIMETR; //не прошла валидация
  }//if(dataReg!=0)

  return 0;
}//validBazaN_BIGACMD(unsigned short dataReg, int actControl)
int validN_SMALLACMD(unsigned short dataReg)
{
//расширенная проверка адреса команды for dv
  if(dataReg!=0)
  {
    int validCMD=0;
    if(((int)encoderValidN_SMALLACMD(dataReg-getACMDSmallBeginAdr(), &validCMD))<0) return MARKER_ERRORPERIMETR; //кодировщик адреса modbus в индекс бита для реле
    if(!validCMD) return MARKER_ERRORPERIMETR; //не прошла валидация
  }//if(dataReg!=0)

  return 0;
}//validN_SMALLACMD(int dataReg)
unsigned int encoderN_BIGACMD(int offsetCMD)
{
  int validCMD=0;
  return encoderValidN_BIGACMD(offsetCMD, &validCMD, CLRACT_CONTROL);
}//encoderN_BIGACMD(int adrCMD)
unsigned int encoderValidN_BIGACMD(int offsetCMD, int *validCMD, int actControl)
{
//кодировщик адреса modbus в индекс бита для реле
  int outMaska=-1;
  int dvMaska=-1;
  (*validCMD) = cmdFunc000(offsetCMD, &outMaska, &dvMaska, actControl);
  return outMaska;
}//encoderValidN_BIGACMD(int offsetCMD, int *validCMD)
unsigned int encoderN_SMALLACMD(int offsetCMD)
{
  int validCMD=0;
  return encoderValidN_SMALLACMD(offsetCMD, &validCMD);
}//encoderN_SMALLACMD(int adrCMD)
unsigned int encoderValidN_SMALLACMD(int offsetCMD, int *validCMD)
{
//кодировщик адреса modbus в индекс бита для ДВ
  int outMaska=-1;
  int dvMaska=-1;
  (*validCMD) = cmdFunc000(offsetCMD, &outMaska, &dvMaska, CLRACT_CONTROL);
  return dvMaska;
}//encoderValidN_SMALLACMD(int offsetCMD, int *validCMD)

int decoderN_BIGACMD(int idxBit)
{
//декодировщик индекса бита в адрес modbus  для реле
  unsigned short result = decoderN_BIGACMDArray[idxBit];
  if(result==0xFFFF) return -1;
  return result;
}//decoderN_BIGACMD(int idxBit)

int decoderN_SMALLACMD(int idxBit)
{
//декодировщик индекса бита в адрес modbus  для ДВ
  unsigned short result = decoderN_SMALLACMDArray[idxBit];
  if(result==0xFFFF) return -1;
  return result;
}//decoderN_SMALLACMD(int idxBit)

int getSequenceN_BIGIndexActiveBit(int pps, unsigned int *array)
{
//индекс активного бита N_BIG
  int bit=0;
  unsigned int arrayData = 0;
  int propusk = pps;
  for(; bit<N_BIG*32; bit++)
  {
    arrayData = array[bit/32];
    if(arrayData==0) {
      bit+=31;
      continue;
    }
    unsigned int data = arrayData&(1<<(bit%32));
    if(data!=0&&propusk==0) break;
    if(data!=0&&propusk!=0) propusk--;
  }//for
  if(bit!=N_BIG*32) return bit;
  return -1;
}//getSequenceN_BIGIndexActiveBit(int propusk, unsigned int *array)

int getSequenceN_SMALLIndexActiveBit(int pps, unsigned int *array)
{
//индекс активного бита N_SMALL
  int bit=0;
  unsigned int arrayData = 0;
  int propusk = pps;
  for(; bit<N_SMALL*32; bit++)
  {
    arrayData = array[bit/32];
    if(arrayData==0) {
      bit+=31;
      continue;
    }
    unsigned int data = arrayData&(1<<(bit%32));
    if(data!=0&&propusk==0) break;
    if(data!=0&&propusk!=0) propusk--;
  }//for
  if(bit!=N_SMALL*32) return bit;
  return -1;
}//getSequenceN_SMALLIndexActiveBit(int propusk, unsigned int *array)
int getRangN_BIGModbusRegister(unsigned int *ranguvannja, int countItem, int offset )
{
//поиск активного бита
  int subObj = offset/countItem;
  int bit = getSequenceN_BIGIndexActiveBit(offset%countItem, &ranguvannja[N_BIG*subObj]);//индекс активного бита
  if(bit!=-1)
  {
    int adr = decoderN_BIGACMD(bit);
    return adr<0? 0: adr+getACMDSmallBeginAdr();//декодировщик индекса бита в адрес modbus  для rele
  }//if(bit!=-1)
  return 0;
}//getRangN_BIGModbusRegister(unsigned int *ranguvannja, int countItem, int offset )

int getRangN_SMALLModbusRegister(unsigned int *ranguvannja, int countItem, int offset )
{
//поиск активного бита
  int subObj = offset/countItem;
  int bit = getSequenceN_SMALLIndexActiveBit(offset%countItem, &ranguvannja[N_SMALL*subObj]);//индекс активного бита
  if(bit!=-1)
  {
    int adr = decoderN_SMALLACMD(bit);
    return adr<0? 0: adr+getACMDSmallBeginAdr();//декодировщик индекса бита в адрес modbus  для rele
  }//if(bit!=-1)
  return 0;
}//getRangN_SMALLModbusRegister(unsigned int *ranguvannja, int countItem, int offset )
void writeRangN_BIGModbusRegister(unsigned int *ranguvannja, int countItem, int beginAdr,
                                  int countAdr, int begin_adr_register)
{
  int offsetTempWriteArray = superFindTempWriteArrayOffset(begin_adr_register);//найти смещение TempWriteArray
  unsigned int tmp[N_BIG];//для сохр ранжирования
  int idxObjOld=-1;//индекс суб обекта old
  //убрать старое
  for(int i=0; i<countAdr; i++)
  {
    int offset = beginAdr-begin_adr_register+i;

    int idxObj = offset/countItem;//индекс суб обекта
    unsigned int *ranguvannja_d = &ranguvannja[N_BIG*idxObj]; //Ранжування
    if(idxObj!=idxObjOld) {
      idxObjOld = idxObj;
      for(int dx=0; dx<N_BIG; dx++) tmp[dx]=ranguvannja_d[dx];//сохр старое ранж
    }//if

    int bitOld = getSequenceN_BIGIndexActiveBit(offset%countItem, tmp);//индекс активного бита
    if(bitOld!=-1) {
      ranguvannja_d[bitOld/32] &= ~(1<<(bitOld%32));
    }//if
  }//for
  //добавить новое
  for(int i=0; i<countAdr; i++)
  {
    unsigned int adr = (unsigned short)tempWriteArray[offsetTempWriteArray+i];//новое значение
    if(adr==0) continue;
    int offset = beginAdr-begin_adr_register+i;

    int idxObj = offset/countItem;//индекс суб обекта
    unsigned int *ranguvannja_d = &ranguvannja[N_BIG*idxObj]; //Ранжування
    int bit = encoderN_BIGACMD(adr-getACMDSmallBeginAdr()); //кодировщик адреса modbus в индекс бита для реле
    if(bit!=-1) {
      ranguvannja_d[bit/32] |= (1<<(bit%32));
    }//if
  }//for
}//writeRangN_BIGModbusRegister(unsigned int *ranguvannja, int countItem, int beginAdr, int countAdr,
void writeRangN_SMALLModbusRegister(unsigned int *ranguvannja, int countItem, int beginAdr,
                                    int countAdr, int begin_adr_register)
{
  int offsetTempWriteArray = superFindTempWriteArrayOffset(begin_adr_register);//найти смещение TempWriteArray
  unsigned int tmp[N_SMALL];//для сохр ранжирования
  int idxObjOld=-1;//индекс суб обекта old
  //убрать старое
  for(int i=0; i<countAdr; i++)
  {
    int offset = beginAdr-begin_adr_register+i;

    int idxObj = offset/countItem;//индекс суб обекта
    unsigned int *ranguvannja_d = &ranguvannja[N_SMALL*idxObj]; //Ранжування
    if(idxObj!=idxObjOld) {
      idxObjOld = idxObj;
      for(int dx=0; dx<N_SMALL; dx++) tmp[dx]=ranguvannja_d[dx];//сохр старое ранж
    }//if

    int bitOld = getSequenceN_SMALLIndexActiveBit(offset%countItem, tmp);//индекс активного бита
    if(bitOld!=-1) {
      ranguvannja_d[bitOld/32] &= ~(1<<(bitOld%32));
    }//if
  }//for
  //добавить новое
  for(int i=0; i<countAdr; i++)
  {
    unsigned int adr = (unsigned short)tempWriteArray[offsetTempWriteArray+i];//новое значение
    if(adr==0) continue;
    int offset = beginAdr-begin_adr_register+i;

    int idxObj = offset/countItem;//индекс суб обекта
    unsigned int *ranguvannja_d = &ranguvannja[N_SMALL*idxObj]; //Ранжування
    int bit = encoderN_SMALLACMD(adr-getACMDSmallBeginAdr()); //кодировщик адреса modbus в индекс бита для dv
    if(bit!=-1) {
      ranguvannja_d[bit/32] |= (1<<(bit%32));
    }//if
  }//for
}//writeRangN_SMALLModbusRegister(unsigned int *ranguvannja, int countItem, int beginAdr, int countAdr,

int getACMDSmallModbusRegister(int adrReg)
{
  extern int globalcntReg;//к-во reg для чтения
  extern int globalbeginAdrReg;//адрес нач reg
  //получить содержимое регистра
  if(privateACMDSmallGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(acmdsmallcomponent->isActiveActualData)
  {
    int begin = globalbeginAdrReg-BEGIN_ADR_REGISTER;
    if(begin<0) globalcntReg += begin;

    int beginOffset = (adrReg-BEGIN_ADR_REGISTER)*16;
    int endOffset   = beginOffset +globalcntReg*16;

    loadACMDSmallActualDataBit(0, beginOffset, endOffset); //ActualData
  }//if(acmdsmallcomponent->isActiveActualData)
  acmdsmallcomponent->isActiveActualData = 0;

  return tempReadArray[adrReg-BEGIN_ADR_REGISTER];
}//getDOUTModbusRegister(int adrReg)
int getACMDSmallModbusBit(int adrBit)
{
  extern int globalcntBit;//к-во бит для чтения
  extern int globalbeginAdrBit;//адрес нач бит
  //получить содержимое bit

  if(privateACMDSmallGetBit2(adrBit)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  int beginOffset = adrBit-BEGIN_ADR_BIT;
  if(acmdsmallcomponent->isActiveActualData)
  {
    int begin = globalbeginAdrBit-BEGIN_ADR_BIT;
    if(begin<0) globalcntBit += begin;

    int endOffset   = beginOffset +globalcntBit;

    loadACMDSmallActualDataBit(0, beginOffset, endOffset); //ActualData
  }//if(acmdsmallcomponent->isActiveActualData)
  acmdsmallcomponent->isActiveActualData = 0;

  short tmp   = tempReadArray[beginOffset/16];
  short maska = 1<<(beginOffset%16);
  if(tmp&maska) return 1;
  return 0;
}//getDOUTModbusBit(int adrReg)
int setACMDSmallModbusRegister(int adrReg, int dataReg)
{
  UNUSED(dataReg);
  if(privateACMDSmallGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  return MARKER_ERRORPERIMETR;
}//getDOUTModbusRegister(int adrReg)
int setACMDSmallModbusBit(int adrBit, int dataBit)
{
  //записать содержимое bit
  if(privateACMDSmallGetBit2(adrBit)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  superSetOperativMarker(acmdsmallcomponent, adrBit);
  superSetTempWriteArray(dataBit);//записать в буфер

  if(adrBit != BEGIN_ADR_BIT+IMUNITET_BITACMD1302)//Активация конфигурации
  {
    if(dataBit == 0) return MARKER_ERRORPERIMETR;
  }//if(adrBit!=50567)
  else return 0;

  if(writeACMDSmallActualDataBit(adrBit-BEGIN_ADR_BIT, 0) != 0) return MARKER_ERRORPERIMETR;
  else
  { //біти з enum _INTERFACE звідки прийшла команда ТУ
    mutex_interface = true;
    switch(pointInterface)//метка интерфейса 0-USB 1-RS485
    {
    case USB_RECUEST:
      dostup_z_VR |= (1<<USB_RECUEST);
      break;
    case RS485_RECUEST:
      dostup_z_VR |= (1<<RS485_RECUEST);
      break;
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
    case LAN_RECUEST:
      dostup_z_VR |= (1<<LAN_RECUEST);
      break;
#endif
    }//switch
    mutex_interface = false;
  }//else

  return 0;
}//setACMDSmallModbusBit(int adrBit, int dataBit)

int writeACMDSmallActualDataBit(int inOffset, int dataBit)
{
  int actControl = 0;
  if(inOffset<0)
  {
    inOffset = -inOffset;
    actControl = 1;
  }//if(offset<0)
  switch(inOffset)
  {
/*    
  case IMUNITET_BITACMD1213://Сигнал про очищення ресурсу лічильників з системи захистів
    if(_CHECK_SET_BIT(active_functions, RANG_MISCEVE_DYSTANCIJNE) != 0) return MARKER_ERRORPERIMETR;
//    if (_CHECK_SET_BIT(active_functions, RANG_READY_TU) == 0)return MARKER_ERRORPERIMETR;
    if(actControl&&dataBit)
    {
      restart_counter = 0xff; //Сигнал про очищення ресурсу лічильників з системи захистів
    }//if(action)
    return 0;
*/
  case IMUNITET_BITACMD1298://Очищення індикації
    return activationFunction(RANG_SMALL_RESET_LEDS, actControl, dataBit);
  case IMUNITET_BITACMD1299://Скидання реле
    return activationFunction(RANG_SMALL_RESET_RELES, actControl, dataBit);

  case IMUNITET_BITACMD1300://Скидання загальних функцій
    if(actControl&&dataBit)
    {
      //Скидання загальних функцій
      switch(pointInterface)//метка интерфейса 0-USB 1-RS485
      {
      case USB_RECUEST:
        reset_trigger_function_from_interface |= (1 << USB_RECUEST);
        break;
      case RS485_RECUEST:
        reset_trigger_function_from_interface |= (1 << RS485_RECUEST);
        break;
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
      case LAN_RECUEST:
        reset_trigger_function_from_interface |= (1 << LAN_RECUEST);
        break;
#endif
      }//switch
    }//if(action)
    return 0;

  case IMUNITET_BITACMD1302://Активизация конфигурации
    if(actControl)
    {
      if(dataBit)
      {
        //Активація внесекних змін
        int typI = 2;
        if(pointInterface==RS485_RECUEST) typI = 3;//метка интерфейса 0-USB 1-RS485
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
        else if(pointInterface==LAN_RECUEST) typI = 4;//метка интерфейса 0-USB 1-RS485
#endif
        if(set_new_settings_from_interface(typI))//2-USB
        {
          type_of_settings_changed = 0;
          _CLEAR_BIT(active_functions, RANG_SETTINGS_CHANGED);
          return ERROR_VALID3;//ошибка
        }//if
      }//if
      else//деактивация
      {
        //Відміна внесекних змін
        current_settings_interfaces = current_settings;
      }
      type_of_settings_changed = 0;
      _CLEAR_BIT(active_functions, RANG_SETTINGS_CHANGED);
    }//if(action)
    return 0;

  case 1305://Запуск аналогового реєстратора
    return start_ar_function(actControl, dataBit); //Запуск аналогового реєстратора
  case 1306://Очистить аналогового регистратор
    return clear_ar_function(); //Очистить аналогового регистратор

  case 1308://Работа Дискретного регистратора
    return 0;//start_ar_function(actControl, dataBit); //Работа Дискретного регистратора
  case 1309://Очистить Дискретного регистратора
    return clear_dr_function(); //Очистить Дискретного регистратора

  case 1360://OF
    return activationFunction(RANG_SMALL_DF1_IN, actControl, dataBit);
  case 1361://OF
    return activationFunction(RANG_SMALL_DF2_IN, actControl, dataBit);
  case 1362://OF
    return activationFunction(RANG_SMALL_DF3_IN, actControl, dataBit);
  case 1363://OF
    return activationFunction(RANG_SMALL_DF4_IN, actControl, dataBit);
  case 1364://OF
    return activationFunction(RANG_SMALL_DF5_IN, actControl, dataBit);
  case 1365://OF
    return activationFunction(RANG_SMALL_DF6_IN, actControl, dataBit);
  case 1366://OF
    return activationFunction(RANG_SMALL_DF7_IN, actControl, dataBit);
  case 1367://OF
    return activationFunction(RANG_SMALL_DF8_IN, actControl, dataBit);

  case 1376://OF
    return activationFunction(RANG_SMALL_DF1_R, actControl, dataBit);
  case 1377://OF
    return activationFunction(RANG_SMALL_DF2_R, actControl, dataBit);
  case 1378://OF
    return activationFunction(RANG_SMALL_DF3_R, actControl, dataBit);
  case 1379://OF
    return activationFunction(RANG_SMALL_DF4_R, actControl, dataBit);
  case 1380://OF
    return activationFunction(RANG_SMALL_DF5_R, actControl, dataBit);
  case 1381://OF
    return activationFunction(RANG_SMALL_DF6_R, actControl, dataBit);
  case 1382://OF
    return activationFunction(RANG_SMALL_DF7_R, actControl, dataBit);
  case 1383://OF
    return activationFunction(RANG_SMALL_DF8_R, actControl, dataBit);

  case 1408://DTR
    return activationFunction(RANG_SMALL_DT1_SET, actControl, dataBit);
  case 1409://DTR
    return activationFunction(RANG_SMALL_DT2_SET, actControl, dataBit);
  case 1410://DTR
    return activationFunction(RANG_SMALL_DT3_SET, actControl, dataBit);
  case 1411://DTR
    return activationFunction(RANG_SMALL_DT4_SET, actControl, dataBit);

  case 1424://DTR
    return activationFunction(RANG_SMALL_DT1_RESET, actControl, dataBit);
  case 1425://DTR
    return activationFunction(RANG_SMALL_DT2_RESET, actControl, dataBit);
  case 1426://DTR
    return activationFunction(RANG_SMALL_DT3_RESET, actControl, dataBit);
  case 1427://DTR
    return activationFunction(RANG_SMALL_DT4_RESET, actControl, dataBit);

  case 1568://Выход ФС 1
    return activationFunction(RANG_SMALL_LF1, actControl, dataBit);
  case 1569://Выход ФС 2
    return activationFunction(RANG_SMALL_LF2, actControl, dataBit);
  case 1570://Выход ФС 3
    return activationFunction(RANG_SMALL_LF3, actControl, dataBit);
  case 1571://Выход ФС 4
    return activationFunction(RANG_SMALL_LF4, actControl, dataBit);
  case 1572://Выход ФС 5
    return activationFunction(RANG_SMALL_LF5, actControl, dataBit);
  case 1573://Выход ФС 6
    return activationFunction(RANG_SMALL_LF6, actControl, dataBit);
  case 1574://Выход ФС 7
    return activationFunction(RANG_SMALL_LF7, actControl, dataBit);
  case 1575://Выход ФС 8
    return activationFunction(RANG_SMALL_LF8, actControl, dataBit);
  case 1576://Выход ФС 9
    return activationFunction(RANG_SMALL_LF9, actControl, dataBit);
  case 1577://Выход ФС 10
    return activationFunction(RANG_SMALL_LF10, actControl, dataBit);
  case 1578://Выход ФС 11
    return activationFunction(RANG_SMALL_LF11, actControl, dataBit);
  case 1579://Выход ФС 12
    return activationFunction(RANG_SMALL_LF12, actControl, dataBit);
  case 1580://Выход ФС 13
    return activationFunction(RANG_SMALL_LF13, actControl, dataBit);
  case 1581://Выход ФС 14
    return activationFunction(RANG_SMALL_LF14, actControl, dataBit);
  case 1582://Выход ФС 15
    return activationFunction(RANG_SMALL_LF15, actControl, dataBit);
  case 1583://Выход ФС 16
    return activationFunction(RANG_SMALL_LF16, actControl, dataBit);

  case BITACMD1729://Скидання блокування РПН
    return 0;

  case BITACMD1752://Знизити від ВР
    return 0;

  case BITACMD1760://Підвищити від ВР
    return 0;

  case BITACMD1772://Скидання загальної кількості перемикань РПН
    return 0;

  case BITACMD1776://Режим керування Дистанційний від ВР
    return 0;

  case BITACMD1777://Режим керування Автоматичний від ВР
    return 0;
  }//switch

  return MARKER_ERRORPERIMETR;
}//writeACMDSmallActualDataBit(int offset)

int activationFunction(int small_cmd, int actControl, int dataBit)
{
  if(_CHECK_SET_BIT(active_functions, RANG_MISCEVE_DYSTANCIJNE) != 0) return MARKER_ERRORPERIMETR;
//    if (_CHECK_SET_BIT(active_functions, RANG_READY_TU) == 0)return MARKER_ERRORPERIMETR;
  if(actControl&&dataBit)
  {
    mutex_interface = true;
    _SET_BIT(activation_function_from_interface, small_cmd);
    mutex_interface = false;
  }//if(action)
  return 0;
}//activationFunction(int small_cmd, int actControl, int dataBit)

int start_ar_function(int actControl, int dataBit)
{
  if(_CHECK_SET_BIT(active_functions, RANG_MISCEVE_DYSTANCIJNE) != 0) return MARKER_ERRORPERIMETR;
//    if (_CHECK_SET_BIT(active_functions, RANG_READY_TU) == 0)return MARKER_ERRORPERIMETR;
  if(actControl&&dataBit)
  {
    start_ar = 0xff; //Запуск аналогового реєстратора
  }//if(action)
  return 0;
}//start_ar_function

int clear_ar_function(void)
{
//Очистить аналоговый регистратор
  if (
/*      
    (state_ar_record_m           != STATE_AR_NONE_M )
    ||
*/
    (state_ar_record_prt         != STATE_AR_NONE_PRT )
    ||
    (
      (control_tasks_dataflash & (
         TASK_MAMORY_PART_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_FS |
         TASK_MAMORY_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_FS      |
         TASK_MAMORY_READ_DATAFLASH_FOR_FS
       )
      ) != 0
    )
    ||
    ((clean_rejestrators & CLEAN_AR) != 0)
  ) return ERROR_VALID2;//ошибка валидации

  clean_rejestrators |= CLEAN_AR;
  return 0;
}//clear_ar_function

int clear_dr_function(void)
{
//Очистить дискретный регистратор
  if (
    (current_ekran.current_level == EKRAN_TITLES_DIGITAL_REGISTRATOR)
    ||
    (current_ekran.current_level == EKRAN_CHANGES_SIGNALS_DR        )
    ||
    (current_ekran.current_level == EKRAN_TITLE_MAX_VALUES          )
    ||
    (current_ekran.current_level == EKRAN_MAX_VALUES                )
    ||
    (
      (control_tasks_dataflash & (
         TASK_MAMORY_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_DR       |
         TASK_MAMORY_READ_DATAFLASH_FOR_DR_USB                          |
         TASK_MAMORY_READ_DATAFLASH_FOR_DR_RS485                        |
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
         TASK_MAMORY_READ_DATAFLASH_FOR_DR_LAN                          |
#endif
         TASK_MAMORY_READ_DATAFLASH_FOR_DR_MENU                         |
         TASK_MAMORY_READ_DATAFLASH_FOR_DR_MENU_SHORT
       )
      ) != 0
    )
    ||
    ((clean_rejestrators & CLEAN_DR) != 0)
  ) return ERROR_VALID2;//ошибка валидации

  //Помічаємо, що треба очистити дискретного реєстратора
  clean_rejestrators |= CLEAN_DR;
  return 0;
}//clear_dr_function

int clean_rejestrators_function(int actControl, int dataBit)
{
//Очистить регистратор статистики
  if(_CHECK_SET_BIT(active_functions, RANG_MISCEVE_DYSTANCIJNE) != 0) return MARKER_ERRORPERIMETR;
//    if (_CHECK_SET_BIT(active_functions, RANG_READY_TU) == 0)return MARKER_ERRORPERIMETR;
  if(actControl&&dataBit)
  {
    clean_rejestrators |= CLEAN_SR_ERR;
  }//if(action)
  return 0;
}//clean_rejestrators_function

int postACMDSmallWriteAction(void)
{
//action после записи
  int beginAdr = acmdsmallcomponent->operativMarker[0];
  if(beginAdr<0) return 0;//не было записи
  int endAdr   = acmdsmallcomponent->operativMarker[1];
  int countAdr = endAdr-beginAdr+1;
  if(endAdr<0) countAdr = 1;

  if(beginAdr>=BEGIN_ADR_BIT)
  {
    //работа с битами
    int offsetTempWriteArray = superFindTempWriteArrayOffset(BEGIN_ADR_BIT);//найти смещение TempWriteArray

    for(int i=0; i<countAdr; i++) {
      int offset = i+beginAdr-BEGIN_ADR_BIT;
      uint32_t value = tempWriteArray[offsetTempWriteArray+i];
      writeACMDSmallActualDataBit(-offset, value);//action
    }//for
  }//if(beginAdr>=BEGIN_ADR_BIT)
  else
  {
    //работа с регистрами
    int offsetTempWriteArray = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//найти смещение TempWriteArray
    for(int i=0; i<countAdr; i++) {
      int offset = i+beginAdr-BEGIN_ADR_REGISTER;
      for(int bit=0; bit<16; bit++)
      {
        uint32_t value = tempWriteArray[offsetTempWriteArray+i];
        int temp = 0;
        if(value&(1<<bit)) temp = 1;
        writeACMDSmallActualDataBit(-(offset*16+bit), temp);//action
      }//for
    }//for
  }//else

  return 0;
}//

int privateACMDSmallGetReg2(int adrReg)
{
  //проверить внешний периметр
  return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)

int privateACMDSmallGetBit2(int adrBit)
{
  //проверить внешний периметр
  return controlPerimetr(adrBit, BEGIN_ADR_BIT, END_ADR_BIT);
}//privateGetReg2(int adrReg)

int passwordImunitetBitACMDSmallComponent(int adrBit)
{
  //имунитетные к паролю адреса bit 0 - есть имунитет
  if(privateACMDSmallGetBit2(adrBit)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  switch(adrBit)
  {
/*    
  case BEGIN_ADR_BIT+IMUNITET_BITACMD1200://Включение ВВ
  case BEGIN_ADR_BIT+IMUNITET_BITACMD1201://Отключение ВВ

  case BEGIN_ADR_BIT+IMUNITET_BITACMD1213://Сброс счетчика ресурса выключателя
*/    
  case BEGIN_ADR_BIT+IMUNITET_BITACMD1298://Сброс индикации
  case BEGIN_ADR_BIT+IMUNITET_BITACMD1299://Сброс реле
  case BEGIN_ADR_BIT+IMUNITET_BITACMD1300://Сброс сработавших функций

    return 0;//есть имунитет
  }//switch
  return MARKER_OUTPERIMETR;
}//passwordImunitetBitACMDSmallComponent(int adrBit)
