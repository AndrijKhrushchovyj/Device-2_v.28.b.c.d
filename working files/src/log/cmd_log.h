#ifndef LOG_H
#define LOG_H
//#include <time.h>
//#include <stdint.h>
#include "constants.h"
#include "libraries.h"
#include "fatfs.h"
#include "variables_external.h"
#include "functions_external.h"

typedef union
{
  int32_t time_ms;
  unsigned long uLMkTime;
  unsigned char uChMkTime[4];

} UNN_MicroSec;

typedef union
{
  time_t time_dat;
  unsigned long arU32MkTime[sizeof(time_t) >> 2];
  unsigned char arU08MkTime[sizeof(time_t)];

} UNN_UnixTime;

typedef union
{
  unsigned long uLCmd[N_BIG];
  unsigned char uChCmd[(N_BIG << 2)];

} UNN_CmdState;

typedef struct tag_CmdPlusTimeStateElem
{
  UNN_CmdState cmd;
  UNN_UnixTime unix_time;
  UNN_MicroSec mksec;
} CmdPlusTimeStateElem;

#ifndef EXT_SRAM_512
#define AMOUNT_CMD_PLUS_TIME_RECORD 4
#else
#define AMOUNT_CMD_PLUS_TIME_RECORD 400
#endif

#define AMOUNT_CMD_PLUS_TIME_STAMP_RECORD 101 //400
#define FIFTY_YEAR_NUMBER ((unsigned long long) 1000 * 3600 * 365 * 50)

typedef struct tag_CmdPlusTimeHolder
{
  CmdPlusTimeStateElem arrCmdPlusTimeHolder[AMOUNT_CMD_PLUS_TIME_RECORD];
  short shIndexWR;
  short shTotalFixElem;
  unsigned long u32IDModifyIndexWR; //Identificator Last Changes Statistics Reg
  //  int   cmdPlusTimeHolderBusy;
} CmdPlusTimeHolder;
extern CmdPlusTimeHolder holderCmdPlusTime;

typedef struct tag_CmdPlusTimeStampElem
{
  UNN_CmdState cmd;
  UNN_UnixTime unix_time;
  UNN_MicroSec mksec;
} CmdPlusTimeStampElem;

typedef struct tag_AdditionalInfoCmdPlusTimeStamp
{
  unsigned long long ullTimeStamp;
  long lNumAlterSigInElem; // analog number_changes_into_current_item
} AdditionalInfoCmdPlusTimeStamp;

typedef struct tag_CmdPlusTimeStampHolder
{
  CmdPlusTimeStampElem arrCmdPlusTimeStampElem[AMOUNT_CMD_PLUS_TIME_STAMP_RECORD];
  AdditionalInfoCmdPlusTimeStamp arrAdditionalInfoCmdPlusTimeStamp[AMOUNT_CMD_PLUS_TIME_STAMP_RECORD];
  short shIndexWR;
  short shIndexRD;
  short shTotalFixElem;
  short shSumDifersElem;
  short shAmount100msElem;
  unsigned long long uLLDrecTimeStampVal;
} CmdPlusTimeStampHolder;
extern CmdPlusTimeStampHolder holderCmdPlusTimeStamp;

typedef struct tag_dig_reg_uniq_vars
{
  unsigned int *number_items_dr;
  unsigned int *number_changes_into_dr_record;
  unsigned int *time_from_start_record_dr;
  unsigned int *blocking_continue_monitoring_min_U;
  unsigned int *previous_active_functions;
  unsigned int *p_active_functions;
} DigRegUniqVarsAddreses;

#define SIZE_QUUE ((int32_t)(MAX_EVENTS_IN_ONE_RECORD - 1)) //100//102  7
typedef struct Queue_tag
{
  unsigned char size; //front, rear
  unsigned char capacity;
  unsigned char existent_elem;
  unsigned char amount_elem;
  uint8_t arTimeLab[SIZE_QUUE];
} QueueChangeMarker;

extern short GetAmountFixElem(void);

void CleanCmdPlusTimeLog(void);
void ChekCurrentStateCmd(unsigned int *p_active_functions);
void CmdPlusTimeLogHundler(unsigned int *p_active_functions);
extern void GetCmdPlusTimeLogElem(unsigned int *p_elem, long lIdx);
extern void GetDateTimeLogElem(unsigned int *p_elem, long lIdx);
extern void GetMsLogElem(unsigned int *p_elem, long lIdx);

extern void CmdPlusTimeStampLogHundler(unsigned int *p_active_functions);
extern void put_before_info_in_buf(unsigned char *const, DigRegUniqVarsAddreses *ptDRecUniqVarsAddreses);
extern void put_before_info_in_buf_from_queue(unsigned char *const, DigRegUniqVarsAddreses *ptDRecUniqVarsAddreses);

extern void TestStateNameSpaceFooBrrr(DigRegUniqVarsAddreses *ptDRecUniqVarsAddreses);
extern void PuCmdinRawBuf(unsigned int *p_active_functions);
extern void FillBeforeBufinDirectOrder(unsigned char *const, DigRegUniqVarsAddreses *ptDRUVAd);
extern void *FillBeforeBufReverseOrder(unsigned char *const, DigRegUniqVarsAddreses *ptDRUVAd);
extern void InithldQueueChangeMarker(void);
extern int enQueue(int value);
extern int deQueue(void);
int getFromRear(int distance);
int getFromFront(int distance);
//~~~ SREC  Part for change
//==============+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//--------------  15:49 2021-10-28  !!!!!!!!!!!!!!      --------------
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef struct tag_Date_holder
{
  long int mday;
  long int mon;
  long int year;
} DateHolder;
typedef struct tag_one_date_elements_dsc
{
  DateHolder dateHolder;
  short amountElemForThisDate;
  short numFierstElemForThisDate; //or Index 1st elem  in arrCmdPlusTimeHolder
} OneDateElmsDsc;

typedef struct tag_DifferDatePlRefTagsInfoDsc
{
  OneDateElmsDsc arrDatePlRefTags[AMOUNT_CMD_PLUS_TIME_RECORD];

  short amountDifferentOneDateElms;
  short indexBaseDatein_arrCmdPlusTimeHolder;

} DiffInfoDsc;
extern DiffInfoDsc hldDiffInfo;
void FillDiffInfo(void);
//..................................................................................
//``````````````````````````````````````````````````````````````````````````````````
extern void FillTwoLineElemInfoForCurrEkrIndexPos(long ekrIndexPos);
extern void Fill2LnScrElemCalcInfo(void *pv2LnCalcInfo, long ekrIndexPos); //?void FillTwoLineElemInfoForCurrEkrIndexPos1(long ekrIndexPos);
extern void Put2LineElemInfoUsingEkrIndexPos(void *pv2LnCalcInfo, long ekrIndexPos);
typedef struct tag_TwoLnCalcInfo
{
  short selectorOneDateElm;
  short selectorCmdPlusTimeStateElem;
  short selIdxBitCmdPlusTimeStateElem;
  long calculatedScrIndexPos;
  long calculatedMaxScrIndexPos;
  union Scr2LnCalcInfoEvt
  {
    struct
    {                                          //.TNV - take new Value
      unsigned int selectorOneDateElemTNV : 1; //0 -selectorOneDateElem take new val
      unsigned int b1 : 1;                     //1
      unsigned int bFierstOneDateElem : 1;     //2
      unsigned int bLastOneDateElem : 1;       //3
      unsigned int b4 : 1;                     //4
      unsigned int b5 : 1;                     //5
      unsigned int b6 : 1;                     //6
      unsigned int b7 : 1;                     //7
    } boolEvtScr;
    unsigned char u8EvtScr;
  };
  char activeScr;
} TwoLnCalcInfo;
extern TwoLnCalcInfo scrTwoLnCalcInfo;
extern unsigned int maxAmountTwoLineSttLogElem;
extern char fierstViewAfterEnterKeyPressed;
extern short selectorOneDateElem, fierstSelectedOneDateElem;
extern long ekr2LineIndexPos;

enum TWO_LINE_SCREEN_INDICATOR_WHAT_TYPE_INFO_SHOWED
{
  //_SLOG_VALUE_LIST
  //SR - statistical REG or SLOG - statistical LOGGER
  SHOW_DATE_PLUS_SPACE = 0,
  SHOW_TIME_MS_STATE_NAME_COMAND,
  TOTAL_SHOWING_TYPES_INFORMATION //??

};
typedef struct tag_make_ekran_func_Var
{

  uint8_t *name_string_tmp;
  unsigned char *pvStrBuf;
  long index_of_ekran;
} Func_make_ekran_variables;
long Calc2LineIndexPos_for_selOneDateElem(long idxOneDateElem);
//?long Calc2LineIndexPos_for_selOneDateElem1(long idxOneDateElem);
void Put2LineElemInfoUsingEkrIndexPos(void *pv2LnCalcInfo, long ekrIndexPos); //TwoLnCalcInfo *pTwoLnCalcInfo
long Calc2lineIndexPosUsingBitCmdValuesAttachedToOneDateElem(long idxOneDateElem);
//,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

typedef union U32_cmd1_Unn
{
  struct
  {
    unsigned int BLOCK_VKL_VV : 1;           //0
    unsigned int RESET_LEDS : 1;             //1
    unsigned int RESET_RELES : 1;            //2
    unsigned int MISCEVE_DYSTANCIJNE : 1;    //3
    unsigned int STATE_VV : 1;               //4
    unsigned int OTKL_VID_ZOVN_ZAHYSTIV : 1; //5
    unsigned int VKL_VV : 1;                 //6
    unsigned int CTRL_VKL : 1;               //7
    unsigned int OTKL_VV : 1;                //8
    unsigned int CTRL_OTKL : 1;              //9
    unsigned int PRYVID_VV : 1;              //10
    unsigned int PEREVYSHCHENNJA_Inom_V : 1; //11
    unsigned int KRYTYCHNYJ_RESURS_VYMY : 1; //12
    unsigned int VYCHERPANYJ_RESURS_VYM : 1; //13
    unsigned int DEFECT : 1;                 //14
    unsigned int AVAR_DEFECT : 1;            //15
    unsigned int WORK_A_REJESTRATOR : 1;     //16
    unsigned int WORK_D_REJESTRATOR : 1;     //17
    unsigned int VIDKL_VID_ZAKHYSTIV : 1;    //18
    unsigned int WORK_BO : 1;                //19
    unsigned int WORK_BV : 1;                //20
    unsigned int l1_GRUPA_USTAVOK : 1;       //21
    unsigned int l2_GRUPA_USTAVOK : 1;       //22
    unsigned int l3_GRUPA_USTAVOK : 1;       //23
    unsigned int l4_GRUPA_USTAVOK : 1;       //24
    unsigned int INVERS_DV_GRUPA_USTAVO : 1; //25
    unsigned int BLK_GRUP_USTAVOK_VID_Z : 1; //26
    unsigned int RESET_BLOCK_READY_TU_V : 1; //27
    unsigned int READY_TU : 1;               //28
    unsigned int SETTINGS_CHANGED : 1;       //29
    unsigned int BLOCK_MTZ1 : 1;             //30
    unsigned int BLOCK_MTZ2 : 1;             //31

  } bool_val;
  unsigned long U32V;
} U32_cmd1_wrp;

typedef union U32_cmd2_Unn
{
  struct
  {
    unsigned int BLOCK_USK_MTZ2 : 1;      //0
    unsigned int BLOCK_MTZ3 : 1;          //1
    unsigned int BLOCK_MTZ4 : 1;          //2
    unsigned int SECTOR_VPERED_MTZN1 : 1; //3
    unsigned int SECTOR_NAZAD_MTZN1 : 1;  //4
    unsigned int PO_MTZ1 : 1;             //5
    unsigned int PO_MTZN1_VPERED : 1;     //6
    unsigned int PO_MTZN1_NAZAD : 1;      //7
    unsigned int PO_U_MTZPN1 : 1;         //8
    unsigned int PO_MTZPN1 : 1;           //9
    unsigned int MTZ1 : 1;                //10
    unsigned int SECTOR_VPERED_MTZN2 : 1; //11
    unsigned int SECTOR_NAZAD_MTZN2 : 1;  //12
    unsigned int PO_MTZ2 : 1;             //13
    unsigned int PO_MTZN2_VPERED : 1;     //14
    unsigned int PO_MTZN2_NAZAD : 1;      //15
    unsigned int PO_U_MTZPN2 : 1;         //16
    unsigned int PO_MTZPN2 : 1;           //17
    unsigned int MTZ2 : 1;                //18
    unsigned int SECTOR_VPERED_MTZN3 : 1; //19
    unsigned int SECTOR_NAZAD_MTZN3 : 1;  //20
    unsigned int PO_MTZ3 : 1;             //21
    unsigned int PO_MTZN3_VPERED : 1;     //22
    unsigned int PO_MTZN3_NAZAD : 1;      //23
    unsigned int PO_U_MTZPN3 : 1;         //24
    unsigned int PO_MTZPN3 : 1;           //25
    unsigned int MTZ3 : 1;                //26
    unsigned int SECTOR_VPERED_MTZN4 : 1; //27
    unsigned int SECTOR_NAZAD_MTZN4 : 1;  //28
    unsigned int PO_MTZ4 : 1;             //29
    unsigned int PO_MTZN4_VPERED : 1;     //30
    unsigned int PO_MTZN4_NAZAD : 1;      //31

  } bool_val;
  unsigned long U32V;
} U32_cmd2_wrp;

typedef union U32_cmd3_Unn
{
  struct
  {
    unsigned int PO_U_MTZPN4 : 1;         //0
    unsigned int PO_MTZPN4 : 1;           //1
    unsigned int MTZ4 : 1;                //2
    unsigned int PO_BLOCK_U_MTZN : 1;     //3
    unsigned int NCN_MTZ : 1;             //4
    unsigned int BLOCK_MTZ04_1 : 1;       //5
    unsigned int BLOCK_MTZ04_2 : 1;       //6
    unsigned int BLOCK_USK_MTZ : 1;       //7
    unsigned int PO_MTZ04_1 : 1;          //8
    unsigned int MTZ04_1 : 1;             //9
    unsigned int PO_MTZ04_2 : 1;          //10
    unsigned int MTZ04_2 : 1;             //11
    unsigned int BLOCK_ZDZ : 1;           //12
    unsigned int PUSK_ZDZ_VID_ : 1;       //13
    unsigned int LIGHT_ZDZ_FRO : 1;       //14
    unsigned int PO_ZDZ : 1;              //15
    unsigned int ZDZ : 1;                 //16
    unsigned int BLOCK_NZZ : 1;           //17
    unsigned int PO_NZZ : 1;              //18
    unsigned int NZZ : 1;                 //19
    unsigned int PO_3I0 : 1;              //20
    unsigned int l3I0 : 1;                //21
    unsigned int lPO_3U0 : 1;             //22
    unsigned int l3U0 : 1;                //23
    unsigned int SECTOR_NZZ : 1;          //24
    unsigned int BLOCK_TZNP1 : 1;         //25
    unsigned int SECTOR_TZNP1_VPERED : 1; //26
    unsigned int SECTOR_TZNP1_NAZAD : 1;  //27
    unsigned int PO_3I0_TZNP1_VPERED : 1; //28
    unsigned int PO_3I0_TZNP1_NAZAD : 1;  //29
    unsigned int PO_3U0_TZNP1_VPERED : 1; //30
    unsigned int PO_3U0_TZNP1_NAZAD : 1;  //31

  } bool_val;
  unsigned long U32V;
} U32_cmd3_wrp;

typedef union U32_cmd4_Unn
{
  struct
  {
    unsigned int PO_TZNP1_VPERED : 1;     //0
    unsigned int PO_TZNP1_NAZAD : 1;      //1
    unsigned int TZNP1 : 1;               //2
    unsigned int BLOCK_TZNP2 : 1;         //3
    unsigned int SECTOR_TZNP2_VPERED : 1; //4
    unsigned int SECTOR_TZNP2_NAZAD : 1;  //5
    unsigned int PO_3I0_TZNP2_VPERED : 1; //6
    unsigned int PO_3I0_TZNP2_NAZAD : 1;  //7
    unsigned int PO_3U0_TZNP2_VPERED : 1; //8
    unsigned int PO_3U0_TZNP2_NAZAD : 1;  //9
    unsigned int PO_TZNP2_VPERED : 1;     //10
    unsigned int PO_TZNP2_NAZAD : 1;      //11
    unsigned int TZNP2 : 1;               //12
    unsigned int BLOCK_TZNP3 : 1;         //13
    unsigned int SECTOR_TZNP3_VPERED : 1; //14
    unsigned int SECTOR_TZNP3_NAZAD : 1;  //15
    unsigned int PO_3I0_TZNP3_VPERED : 1; //16
    unsigned int PO_3I0_TZNP3_NAZAD : 1;  //17
    unsigned int PO_3U0_TZNP3_VPERED : 1; //18
    unsigned int PO_3U0_TZNP3_NAZAD : 1;  //19
    unsigned int PO_TZNP3_VPERED : 1;     //20
    unsigned int PO_TZNP3_NAZAD : 1;      //21
    unsigned int TZNP3 : 1;               //22
    unsigned int STAT_BLK_APV : 1;        //23
    unsigned int APV1 : 1;                //24
    unsigned int APV2 : 1;                //25
    unsigned int APV3 : 1;                //26
    unsigned int APV4 : 1;                //27
    unsigned int APV_WORK : 1;            //28
    unsigned int ACHR_CHAPV_VID_DV : 1;   //29
    unsigned int BLOCK_ACHR1 : 1;         //30
    unsigned int BLOCK_ACHR2 : 1;         //31

  } bool_val;
  unsigned long U32V;
} U32_cmd4_wrp;

typedef union U32_cmd5_Unn
{
  struct
  {
    unsigned int RAZR_CHAPV : 1;       //0
    unsigned int VN_BLOCK_CHAPV : 1;   //1
    unsigned int PO_ACHR1 : 1;         //2
    unsigned int PO_CHAPV1 : 1;        //3
    unsigned int ACHR_CHAPV1 : 1;      //4
    unsigned int PO_ACHR2 : 1;         //5
    unsigned int PO_CHAPV2 : 1;        //6
    unsigned int ACHR_CHAPV2 : 1;      //7
    unsigned int PUSK_UROV_VID_DV : 1; //8
    unsigned int PO_UROV : 1;          //9
    unsigned int UROV1 : 1;            //10
    unsigned int UROV2 : 1;            //11
    unsigned int BLOCK_ZOP : 1;        //12
    unsigned int PO_ZOP : 1;           //13
    unsigned int ZOP : 1;              //14
    unsigned int BLOCK_UMIN1 : 1;      //15
    unsigned int START_UMIN1 : 1;      //16
    unsigned int BLOCK_UMIN2 : 1;      //17
    unsigned int START_UMIN2 : 1;      //18
    unsigned int PO_UMIN1 : 1;         //19
    unsigned int PO_UBLK_UMIN1 : 1;    //20
    unsigned int PO_IBLK_UMIN1 : 1;    //21
    unsigned int UMIN1 : 1;            //22
    unsigned int PO_UMIN2 : 1;         //23
    unsigned int PO_UBLK_UMIN2 : 1;    //24
    unsigned int PO_IBLK_UMIN2 : 1;    //25
    unsigned int UMIN2 : 1;            //26
    unsigned int BLOCK_UMAX1 : 1;      //27
    unsigned int PO_UMAX1 : 1;         //28
    unsigned int UMAX1 : 1;            //29
    unsigned int BLOCK_UMAX2 : 1;      //30
    unsigned int PO_UMAX2 : 1;         //31

  } bool_val;
  unsigned long U32V;
} U32_cmd5_wrp;

typedef union U32_cmd6_Unn
{
  struct
  {
    unsigned int UMAX2 : 1;     //0
    unsigned int BLOCK_UP1 : 1; //1
    unsigned int PO_UP1 : 1;    //2
    unsigned int UP1 : 1;       //3
    unsigned int BLOCK_UP2 : 1; //4
    unsigned int PO_UP2 : 1;    //5
    unsigned int UP2 : 1;       //6
    unsigned int BLOCK_UP3 : 1; //7
    unsigned int PO_UP3 : 1;    //8
    unsigned int UP3 : 1;       //9
    unsigned int BLOCK_UP4 : 1; //10
    unsigned int PO_UP4 : 1;    //11
    unsigned int UP4 : 1;       //12
    unsigned int BLOCK_UP5 : 1; //13
    unsigned int PO_UP5 : 1;    //14
    unsigned int UP5 : 1;       //15
    unsigned int BLOCK_UP6 : 1; //16
    unsigned int PO_UP6 : 1;    //17
    unsigned int UP6 : 1;       //18
    unsigned int BLOCK_UP7 : 1; //19
    unsigned int PO_UP7 : 1;    //20
    unsigned int UP7 : 1;       //21
    unsigned int BLOCK_UP8 : 1; //22
    unsigned int PO_UP8 : 1;    //23
    unsigned int UP8 : 1;       //24
    unsigned int DF1_IN : 1;    //25
    unsigned int DF1_OUT : 1;   //26
    unsigned int DF2_IN : 1;    //27
    unsigned int DF2_OUT : 1;   //28
    unsigned int DF3_IN : 1;    //29
    unsigned int DF3_OUT : 1;   //30
    unsigned int DF4_IN : 1;    //31

  } bool_val;
  unsigned long U32V;
} U32_cmd6_wrp;

typedef union U32_cmd7_Unn
{
  struct
  {
    unsigned int DF4_OUT : 1;   //0
    unsigned int DF5_IN : 1;    //1
    unsigned int DF5_OUT : 1;   //2
    unsigned int DF6_IN : 1;    //3
    unsigned int DF6_OUT : 1;   //4
    unsigned int DF7_IN : 1;    //5
    unsigned int DF7_OUT : 1;   //6
    unsigned int DF8_IN : 1;    //7
    unsigned int DF8_OUT : 1;   //8
    unsigned int DT1_SET : 1;   //9
    unsigned int DT1_RESET : 1; //10
    unsigned int DT1_OUT : 1;   //11
    unsigned int DT2_SET : 1;   //12
    unsigned int DT2_RESET : 1; //13
    unsigned int DT2_OUT : 1;   //14
    unsigned int DT3_SET : 1;   //15
    unsigned int DT3_RESET : 1; //16
    unsigned int DT3_OUT : 1;   //17
    unsigned int DT4_SET : 1;   //18
    unsigned int DT4_RESET : 1; //19
    unsigned int DT4_OUT : 1;   //20
    unsigned int D_AND1 : 1;    //21
    unsigned int D_AND2 : 1;    //22
    unsigned int D_AND3 : 1;    //23
    unsigned int D_AND4 : 1;    //24
    unsigned int D_AND5 : 1;    //25
    unsigned int D_AND6 : 1;    //26
    unsigned int D_AND7 : 1;    //27
    unsigned int D_AND8 : 1;    //28
    unsigned int D_OR1 : 1;     //29
    unsigned int D_OR2 : 1;     //30
    unsigned int D_OR3 : 1;     //31

  } bool_val;
  unsigned long U32V;
} U32_cmd7_wrp;

typedef union U32_cmd8_Unn
{
  struct
  {
    unsigned int D_OR4 : 1;         //0
    unsigned int D_OR5 : 1;         //1
    unsigned int D_OR6 : 1;         //2
    unsigned int D_OR7 : 1;         //3
    unsigned int D_OR8 : 1;         //4
    unsigned int D_XOR1 : 1;        //5
    unsigned int D_XOR2 : 1;        //6
    unsigned int D_XOR3 : 1;        //7
    unsigned int D_XOR4 : 1;        //8
    unsigned int D_XOR5 : 1;        //9
    unsigned int D_XOR6 : 1;        //10
    unsigned int D_XOR7 : 1;        //11
    unsigned int D_XOR8 : 1;        //12
    unsigned int D_NOT1 : 1;        //13
    unsigned int D_NOT2 : 1;        //14
    unsigned int D_NOT3 : 1;        //15
    unsigned int D_NOT4 : 1;        //16
    unsigned int D_NOT5 : 1;        //17
    unsigned int D_NOT6 : 1;        //18
    unsigned int D_NOT7 : 1;        //19
    unsigned int D_NOT8 : 1;        //20
    unsigned int D_NOT9 : 1;        //21
    unsigned int D_NOT10 : 1;       //22
    unsigned int D_NOT11 : 1;       //23
    unsigned int D_NOT12 : 1;       //24
    unsigned int D_NOT13 : 1;       //25
    unsigned int D_NOT14 : 1;       //26
    unsigned int D_NOT15 : 1;       //27
    unsigned int D_NOT16 : 1;       //28
    unsigned int ERROR_CONF_EL : 1; //29
    unsigned int b30 : 1;           //30
    unsigned int b31 : 1;           //31

  } bool_val;
  unsigned long U32V;
} U32_cmd8_wrp;

typedef struct tag_cmd_bit_depot
{
  U32_cmd1_wrp w1;
  U32_cmd2_wrp w2;
  U32_cmd3_wrp w3;
  U32_cmd4_wrp w4;
  U32_cmd5_wrp w5;
  U32_cmd6_wrp w6;
  U32_cmd7_wrp w7;
  U32_cmd8_wrp w8;
  U32_cmd8_wrp w9;
} CmdFunctionDepot;

extern CmdFunctionDepot *pDbgViewCmd;
enum STATISTICAL_LOG_RETURN_VALUE_LIST
{
  //SR - statistical REG or SLOG - statistical LOGGER
  SLOG_OK = 0,                  //OK Value
  SLOG_INVALID_IDX,             //Error Value - Invalid  lIdx PARAMETER
  SLOG_INVALID_L_WNUM,          //Error Value - Invalid  WorkNumber PARAMETER
  SLOG_INVALID_SUM_IDX_PL_WNUM, //Error Value - Invalid  SUM lIdx+WorkNumber PARAMETER
  SLOG_LACK_COMPUTE_RESORCES,   //INFO  Value - NOT ENOUGH PROCESSOR TIME FOR CALCULATION --
  SLOG_DATA_BLOCKED_CLEAN_FUNC, //INFO  Value now start clening statistical registrator
                                //
  TOTAL_SR_MESAGE_INFO          //

};

extern unsigned int mnu_array_old[N_BIG], mnu_array_new[N_BIG];
enum ONM_EKRAN_LIST
{
  ONM_EKRAN_SETPOINT_ZOP_GROUP1 = EKRAN_SETPOINT_ZOP_GROUP1,
  ONM_EKRAN_SETPOINT_ZOP_GROUP2 = EKRAN_SETPOINT_ZOP_GROUP2,
  ONM_EKRAN_SETPOINT_ZOP_GROUP3 = EKRAN_SETPOINT_ZOP_GROUP3,
  ONM_EKRAN_SETPOINT_ZOP_GROUP4 = EKRAN_SETPOINT_ZOP_GROUP4,
  ONM_EKRAN_TIMEOUT_ZOP_GROUP1 = EKRAN_TIMEOUT_ZOP_GROUP1,
  ONM_EKRAN_TIMEOUT_ZOP_GROUP2 = EKRAN_TIMEOUT_ZOP_GROUP2,
  ONM_EKRAN_TIMEOUT_ZOP_GROUP3 = EKRAN_TIMEOUT_ZOP_GROUP3,
  ONM_EKRAN_TIMEOUT_ZOP_GROUP4 = EKRAN_TIMEOUT_ZOP_GROUP4,
  ONM_EKRAN_CONTROL_ZOP = EKRAN_CONTROL_ZOP,

  ONM_EKRAN_TIMEOUT_ZDZ_GROUP1 = EKRAN_TIMEOUT_ZNKh_GROUP1,
  ONM_EKRAN_TIMEOUT_ZDZ_GROUP2 = EKRAN_TIMEOUT_ZNKh_GROUP2,
  ONM_EKRAN_TIMEOUT_ZDZ_GROUP3 = EKRAN_TIMEOUT_ZNKh_GROUP3,
  ONM_EKRAN_TIMEOUT_ZDZ_GROUP4 = EKRAN_TIMEOUT_ZNKh_GROUP4,
  ONM_EKRAN_CONTROL_ZDZ = EKRAN_CONTROL_ZNKh,

  ONM_EKRAN_SETPOINT_VMP_FORWARD = EKRAN_SETPOINT_VMP_FORWARD,
  ONM_EKRAN_SETPOINT_VMP_BACKWARD = EKRAN_SETPOINT_VMP_BACKWARD,
  ONM_EKRAN_CONTROL_VMP = EKRAN_CONTROL_VMP,

  ONM_EKRAN_VIDKLUCHENNJA = EKRAN_VIDKLUCHENNJA,

  ONM_EKRAN_SETPOINT_UROV_GROUP1 = EKRAN_SETPOINT_UROV_GROUP1,
  ONM_EKRAN_SETPOINT_UROV_GROUP2 = EKRAN_SETPOINT_UROV_GROUP2,
  ONM_EKRAN_SETPOINT_UROV_GROUP3 = EKRAN_SETPOINT_UROV_GROUP3,
  ONM_EKRAN_SETPOINT_UROV_GROUP4 = EKRAN_SETPOINT_UROV_GROUP4,
  ONM_EKRAN_TIMEOUT_UROV_GROUP1 = EKRAN_TIMEOUT_UROV_GROUP1,
  ONM_EKRAN_TIMEOUT_UROV_GROUP2 = EKRAN_TIMEOUT_UROV_GROUP2,
  ONM_EKRAN_TIMEOUT_UROV_GROUP3 = EKRAN_TIMEOUT_UROV_GROUP3,
  ONM_EKRAN_TIMEOUT_UROV_GROUP4 = EKRAN_TIMEOUT_UROV_GROUP4,
  ONM_EKRAN_CONTROL_UROV = EKRAN_CONTROL_UROV,
  ONM_EKRAN_SETPOINT_UP_GROUP1 = EKRAN_SETPOINT_UP_GROUP1,
  ONM_EKRAN_SETPOINT_UP_GROUP2 = EKRAN_SETPOINT_UP_GROUP2,
  ONM_EKRAN_SETPOINT_UP_GROUP3 = EKRAN_SETPOINT_UP_GROUP3,
  ONM_EKRAN_SETPOINT_UP_GROUP4 = EKRAN_SETPOINT_UP_GROUP4,
  ONM_EKRAN_TIMEOUT_UP_GROUP1 = EKRAN_TIMEOUT_UP_GROUP1,
  ONM_EKRAN_TIMEOUT_UP_GROUP2 = EKRAN_TIMEOUT_UP_GROUP2,
  ONM_EKRAN_TIMEOUT_UP_GROUP3 = EKRAN_TIMEOUT_UP_GROUP3,
  ONM_EKRAN_TIMEOUT_UP_GROUP4 = EKRAN_TIMEOUT_UP_GROUP4,
  ONM_EKRAN_CONTROL_UP = EKRAN_CONTROL_UP,

  ONM_EKRAN_SETPOINT_UMIN_GROUP1 = EKRAN_SETPOINT_UMIN_GROUP1,
  ONM_EKRAN_SETPOINT_UMIN_GROUP2 = EKRAN_SETPOINT_UMIN_GROUP2,
  ONM_EKRAN_SETPOINT_UMIN_GROUP3 = EKRAN_SETPOINT_UMIN_GROUP3,
  ONM_EKRAN_SETPOINT_UMIN_GROUP4 = EKRAN_SETPOINT_UMIN_GROUP4,
  ONM_EKRAN_TIMEOUT_UMIN_GROUP1 = EKRAN_TIMEOUT_UMIN_GROUP1,
  ONM_EKRAN_TIMEOUT_UMIN_GROUP2 = EKRAN_TIMEOUT_UMIN_GROUP2,
  ONM_EKRAN_TIMEOUT_UMIN_GROUP3 = EKRAN_TIMEOUT_UMIN_GROUP3,
  ONM_EKRAN_TIMEOUT_UMIN_GROUP4 = EKRAN_TIMEOUT_UMIN_GROUP4,
  ONM_EKRAN_CONTROL_UMIN = EKRAN_CONTROL_UMIN,

  ONM_EKRAN_SETPOINT_UMAX_GROUP1 = EKRAN_SETPOINT_UMAX_GROUP1,
  ONM_EKRAN_SETPOINT_UMAX_GROUP2 = EKRAN_SETPOINT_UMAX_GROUP2,
  ONM_EKRAN_SETPOINT_UMAX_GROUP3 = EKRAN_SETPOINT_UMAX_GROUP3,
  ONM_EKRAN_SETPOINT_UMAX_GROUP4 = EKRAN_SETPOINT_UMAX_GROUP4,
  ONM_EKRAN_TIMEOUT_UMAX_GROUP1 = EKRAN_TIMEOUT_UMAX_GROUP1,
  ONM_EKRAN_TIMEOUT_UMAX_GROUP2 = EKRAN_TIMEOUT_UMAX_GROUP2,
  ONM_EKRAN_TIMEOUT_UMAX_GROUP3 = EKRAN_TIMEOUT_UMAX_GROUP3,
  ONM_EKRAN_TIMEOUT_UMAX_GROUP4 = EKRAN_TIMEOUT_UMAX_GROUP4,
  ONM_EKRAN_CONTROL_UMAX = EKRAN_CONTROL_UMAX,

  ONM_EKRAN_TRANSFORMATOR_INFO = EKRAN_TRANSFORMATOR_INFO,
  ONM_EKRAN_TRANSFORMATOR_INFO_SETPOINT = EKRAN_TRANSFORMATOR_INFO_SETPOINT,
  ONM_EKRAN_TRANSFORMATOR_INFO_CONTROL = EKRAN_TRANSFORMATOR_INFO_CONTROL,

  ONM_EKRAN_LIST_TF_FOR_RANGUVANNJA = EKRAN_LIST_TF_FOR_RANGUVANNJA,
  ONM_EKRAN_LIST_SOURCE_TF1 = EKRAN_LIST_SOURCE_TF1,

  ONM_EKRAN_TIME = EKRAN_TIME,

  ONM_EKRAN_SETPOINT_SWITCH = EKRAN_SETPOINT_SWITCH,
  ONM_EKRAN_TIMEOUT_SWITCH = EKRAN_TIMEOUT_SWITCH,
  ONM_EKRAN_CONTROL_SWITCH = EKRAN_CONTROL_SWITCH,
  ONM_EKRAN_CHOOSE_RANG_SWITCH = EKRAN_CHOOSE_RANG_SWITCH,
  ONM_EKRAN_RANGUVANNJA_OFF_CB = EKRAN_RANGUVANNJA_OFF_CB,
  ONM_EKRAN_RANGUVANNJA_ON_CB = EKRAN_RANGUVANNJA_ON_CB,

  ONM_EKRAN_LIST_INPUTS_OUTPUTS = EKRAN_LIST_INPUTS_OUTPUTS,
  ONM_EKRAN_STATE_INPUTS = EKRAN_STATE_INPUTS,
  ONM_EKRAN_STATE_OUTPUTS = EKRAN_STATE_OUTPUTS,

  ONM_EKRAN_RESURS = EKRAN_RESURS,

  ONM_EKRAN_LIST_REGISTRATORS = EKRAN_LIST_REGISTRATORS,
  ONM_EKRAN_LIST_ANALOG_REGISTRATOR_RECORDS = EKRAN_LIST_ANALOG_REGISTRATOR_RECORDS,
  ONM_EKRAN_LIST_DIGITAL_REGISTRATOR_RECORDS = EKRAN_LIST_DIGITAL_REGISTRATOR_RECORDS,

  ONM_EKRAN_LIST_REGISTRATOR_PROGRAM_ERROR_RECORDS = EKRAN_LIST_REGISTRATOR_PROGRAM_ERROR_RECORDS,
  ONM_EKRAN_TITLES_DIGITAL_REGISTRATOR = EKRAN_TITLES_DIGITAL_REGISTRATOR,
  ONM_EKRAN_CHANGES_SIGNALS_DR = EKRAN_CHANGES_SIGNALS_DR,
  ONM_EKRAN_TITLE_MAX_VALUES = EKRAN_TITLE_MAX_VALUES,
  ONM_EKRAN_MAX_VALUES = EKRAN_MAX_VALUES,
  ONM_EKRAN_CHANGES_DIAGNOSTICS_PR_ERR = EKRAN_CHANGES_DIAGNOSTICS_PR_ERR,

  ONM_EKRAN_LIST_STATE_CMD_REGISTRATOR_RECORDS = EKRAN_LIST_STATE_CMD_REGISTRATOR_RECORDS,
  ONM_EKRAN_TITLES_STATE_CMD_REGISTRATOR = EKRAN_TITLES_STATE_CMD_REGISTRATOR,
  ONM_EKRAN_DATA_LABEL_STATE_CMD = EKRAN_DATA_LABEL_STATE_CMD,
  ONM_EKRAN_STATE_CMD_REG = EKRAN_STATE_CMD_REG,

  ONM_EKRAN_POINT_TIME_SETTINGS = EKRAN_POINT_TIME_SETTINGS,
  ONM_EKRAN_POINT_TIME_SETPOINT = EKRAN_POINT_TIME_SETPOINT,
  ONM_EKRAN_POINT_TIME_RANGUVANNJA = EKRAN_POINT_TIME_RANGUVANNJA,

  ONM_EKRAN_CONTROL_MTZ04 = EKRAN_CONTROL_SZKh,

  ONM_EKRAN_SETPOINT_MTZ_GROUP1 = EKRAN_SETPOINT_RPN_GROUP1,
  ONM_EKRAN_SETPOINT_MTZ_GROUP2 = EKRAN_SETPOINT_RPN_GROUP2,
  ONM_EKRAN_SETPOINT_MTZ_GROUP3 = EKRAN_SETPOINT_RPN_GROUP3,
  ONM_EKRAN_SETPOINT_MTZ_GROUP4 = EKRAN_SETPOINT_RPN_GROUP4,
  ONM_EKRAN_TIMEOUT_MTZ_GROUP1 = EKRAN_TIMEOUT_RPN_GROUP1,
  ONM_EKRAN_TIMEOUT_MTZ_GROUP2 = EKRAN_TIMEOUT_RPN_GROUP2,
  ONM_EKRAN_TIMEOUT_MTZ_GROUP3 = EKRAN_TIMEOUT_RPN_GROUP3,
  ONM_EKRAN_TIMEOUT_MTZ_GROUP4 = EKRAN_TIMEOUT_RPN_GROUP4,
  ONM_EKRAN_CONTROL_MTZ = EKRAN_CONTROL_RPN,

  ONM_EKRAN_MEASURMENT = EKRAN_MEASURMENT,
  ONM_EKRAN_MEASURMENT_CURRENT = EKRAN_MEASURMENT_CURRENT,
  ONM_EKRAN_MEASURMENT_VOLTAGE_TYPE = EKRAN_MEASURMENT_VOLTAGE_TYPE,
  ONM_EKRAN_MEASURMENT_VOLTAGE_PHASE = EKRAN_MEASURMENT_VOLTAGE_PHASE,
  ONM_EKRAN_MEASURMENT_VOLTAGE_LINE = EKRAN_MEASURMENT_VOLTAGE_LINE,
  ONM_EKRAN_MEASURMENT_FREQUENCY = EKRAN_MEASURMENT_FREQUENCY,
  ONM_EKRAN_MEASURMENT_ANGLE = EKRAN_MEASURMENT_ANGLE,
  ONM_EKRAN_MEASURMENT_POWER = EKRAN_MEASURMENT_POWER,
  ONM_EKRAN_MEASURMENT_RESISTANCE = EKRAN_MEASURMENT_RESISTANCE,
  ONM_EKRAN_ENERGY = EKRAN_ENERGY,

  ONM_EKRAN_MAIN = EKRAN_MAIN,

  ONM_EKRAN_LEVEL_PASSWORD = EKRAN_LEVEL_PASSWORD,                   //! << = #define EKRAN_LEVEL_PASSWORD          0
  ONM_EKRAN_LEVEL_PASSWORD_HARD = EKRAN_LEVEL_PASSWORD_HARD,         //! << = #define EKRAN_LEVEL_PASSWORD_HARD     (EKRAN_LEVEL_PASSWORD + 1)
  ONM_EKRAN_LEVEL_CHOOSE_PASSWORDS = EKRAN_LEVEL_CHOOSE_PASSWORDS,   //! << = #define EKRAN_LEVEL_CHOOSE_PASSWORDS  (EKRAN_RANGUVANNJA_BUTTON_6 + 1)
  ONM_EKRAN_LEVEL_SET_NEW_PASSWORD1 = EKRAN_LEVEL_SET_NEW_PASSWORD1, //! << = #define EKRAN_LEVEL_SET_NEW_PASSWORD1 (EKRAN_LEVEL_CHOOSE_PASSWORDS + 1)
  ONM_EKRAN_LEVEL_SET_NEW_PASSWORD2 = EKRAN_LEVEL_SET_NEW_PASSWORD2, //! << = #define EKRAN_LEVEL_SET_NEW_PASSWORD2 (EKRAN_LEVEL_SET_NEW_PASSWORD1 + 1)

  ONM_EKRAN_LIST_LEDS_FOR_RANGUVANNJA = EKRAN_LIST_LEDS_FOR_RANGUVANNJA,

  ONM_EKRAN_RANGUVANNJA_LED_1 = EKRAN_RANGUVANNJA_LED_1,
  ONM_EKRAN_RANGUVANNJA_LED_2 = EKRAN_RANGUVANNJA_LED_2,
  ONM_EKRAN_RANGUVANNJA_LED_3 = EKRAN_RANGUVANNJA_LED_3,
  ONM_EKRAN_RANGUVANNJA_LED_4 = EKRAN_RANGUVANNJA_LED_4,
  ONM_EKRAN_RANGUVANNJA_LED_5 = EKRAN_RANGUVANNJA_LED_5,
  ONM_EKRAN_RANGUVANNJA_LED_6 = EKRAN_RANGUVANNJA_LED_6,
  ONM_EKRAN_RANGUVANNJA_LED_7 = EKRAN_RANGUVANNJA_LED_7,
  ONM_EKRAN_RANGUVANNJA_LED_8 = EKRAN_RANGUVANNJA_LED_8,
  ONM_EKRAN_RANGUVANNJA_LED_9 = EKRAN_RANGUVANNJA_LED_9,
  ONM_EKRAN_RANGUVANNJA_LED_10 = EKRAN_RANGUVANNJA_LED_10,
  ONM_EKRAN_RANGUVANNJA_LED_11 = EKRAN_RANGUVANNJA_LED_11,
  ONM_EKRAN_RANGUVANNJA_LED_12 = EKRAN_RANGUVANNJA_LED_12,
  ONM_EKRAN_RANGUVANNJA_LED_13 = EKRAN_RANGUVANNJA_LED_13,
  ONM_EKRAN_RANGUVANNJA_LED_14 = EKRAN_RANGUVANNJA_LED_14,
  ONM_EKRAN_RANGUVANNJA_LED_15 = EKRAN_RANGUVANNJA_LED_15,
  ONM_EKRAN_RANGUVANNJA_LED_16 = EKRAN_RANGUVANNJA_LED_16,
  ONM_EKRAN_RANGUVANNJA_LED_17 = EKRAN_RANGUVANNJA_LED_17,

  ONM_EKRAN_INFO = EKRAN_INFO,
  ONM_EKRAN_DATE_TIME_PZ = EKRAN_DATE_TIME_PZ,
  ONM_EKRAN_GENERAL_PICKUPS_EL = EKRAN_GENERAL_PICKUPS_EL,
  ONM_EKRAN_EXTENDED_LIGIC = EKRAN_EXTENDED_LIGIC,

  ONM_EKRAN_LIST_DT = EKRAN_LIST_DT,
  ONM_EKRAN_SET_RESET_DT1 = EKRAN_SET_RESET_DT1,
  ONM_EKRAN_SET_RESET_DT2 = EKRAN_SET_RESET_DT2,
  ONM_EKRAN_SET_RESET_DT3 = EKRAN_SET_RESET_DT3,
  ONM_EKRAN_SET_RESET_DT4 = EKRAN_SET_RESET_DT4,
  ONM_EKRAN_SET_DT1 = EKRAN_SET_DT1,
  ONM_EKRAN_RESET_DT1 = EKRAN_RESET_DT1,
  ONM_EKRAN_SET_DT2 = EKRAN_SET_DT2,
  ONM_EKRAN_RESET_DT2 = EKRAN_RESET_DT2,
  ONM_EKRAN_SET_DT3 = EKRAN_SET_DT3,
  ONM_EKRAN_RESET_DT3 = EKRAN_RESET_DT3,
  ONM_EKRAN_SET_DT4 = EKRAN_SET_DT4,
  ONM_EKRAN_RESET_DT4 = EKRAN_RESET_DT4,
  ONM_EKRAN_RANGUVANNJA_SET_DT1_PLUS = EKRAN_RANGUVANNJA_SET_DT1_PLUS,
  ONM_EKRAN_RANGUVANNJA_SET_DT1_MINUS = EKRAN_RANGUVANNJA_SET_DT1_MINUS,
  ONM_EKRAN_RANGUVANNJA_RESET_DT1_PLUS = EKRAN_RANGUVANNJA_RESET_DT1_PLUS,
  ONM_EKRAN_RANGUVANNJA_RESET_DT1_MINUS = EKRAN_RANGUVANNJA_RESET_DT1_MINUS,
  ONM_EKRAN_RANGUVANNJA_SET_DT2_PLUS = EKRAN_RANGUVANNJA_SET_DT2_PLUS,
  ONM_EKRAN_RANGUVANNJA_SET_DT2_MINUS = EKRAN_RANGUVANNJA_SET_DT2_MINUS,
  ONM_EKRAN_RANGUVANNJA_RESET_DT2_PLUS = EKRAN_RANGUVANNJA_RESET_DT2_PLUS,
  ONM_EKRAN_RANGUVANNJA_RESET_DT2_MINUS = EKRAN_RANGUVANNJA_RESET_DT2_MINUS,
  ONM_EKRAN_RANGUVANNJA_SET_DT3_PLUS = EKRAN_RANGUVANNJA_SET_DT3_PLUS,
  ONM_EKRAN_RANGUVANNJA_SET_DT3_MINUS = EKRAN_RANGUVANNJA_SET_DT3_MINUS,
  ONM_EKRAN_RANGUVANNJA_RESET_DT3_PLUS = EKRAN_RANGUVANNJA_RESET_DT3_PLUS,
  ONM_EKRAN_RANGUVANNJA_RESET_DT3_MINUS = EKRAN_RANGUVANNJA_RESET_DT3_MINUS,
  ONM_EKRAN_RANGUVANNJA_SET_DT4_PLUS = EKRAN_RANGUVANNJA_SET_DT4_PLUS,
  ONM_EKRAN_RANGUVANNJA_SET_DT4_MINUS = EKRAN_RANGUVANNJA_SET_DT4_MINUS,
  ONM_EKRAN_RANGUVANNJA_RESET_DT4_PLUS = EKRAN_RANGUVANNJA_RESET_DT4_PLUS,
  ONM_EKRAN_RANGUVANNJA_RESET_DT4_MINUS = EKRAN_RANGUVANNJA_RESET_DT4_MINUS,

  ONM_EKRAN_LIST_SETTINGS_FOR_DF = EKRAN_LIST_SETTINGS_FOR_DF,
  ONM_EKRAN_LIST_TYPE_DF = EKRAN_LIST_TYPE_DF,
  ONM_EKRAN_LIST_DF_FOR_RANGUVANNJA = EKRAN_LIST_DF_FOR_RANGUVANNJA,
  ONM_EKRAN_LIST_DF_FOR_TIMEOUT_SETTINGS = EKRAN_LIST_DF_FOR_TIMEOUT_SETTINGS,
  ONM_EKRAN_TIMEOUT_DF1 = EKRAN_TIMEOUT_DF1,
  ONM_EKRAN_TIMEOUT_DF2 = EKRAN_TIMEOUT_DF2,
  ONM_EKRAN_TIMEOUT_DF3 = EKRAN_TIMEOUT_DF3,
  ONM_EKRAN_TIMEOUT_DF4 = EKRAN_TIMEOUT_DF4,
  ONM_EKRAN_TIMEOUT_DF5 = EKRAN_TIMEOUT_DF5,
  ONM_EKRAN_TIMEOUT_DF6 = EKRAN_TIMEOUT_DF6,
  ONM_EKRAN_TIMEOUT_DF7 = EKRAN_TIMEOUT_DF7,
  ONM_EKRAN_TIMEOUT_DF8 = EKRAN_TIMEOUT_DF8,
  ONM_EKRAN_LIST_TYPE_SOURCE_DF1 = EKRAN_LIST_TYPE_SOURCE_DF1,
  ONM_EKRAN_LIST_TYPE_SOURCE_DF2 = EKRAN_LIST_TYPE_SOURCE_DF2,
  ONM_EKRAN_LIST_TYPE_SOURCE_DF3 = EKRAN_LIST_TYPE_SOURCE_DF3,
  ONM_EKRAN_LIST_TYPE_SOURCE_DF4 = EKRAN_LIST_TYPE_SOURCE_DF4,
  ONM_EKRAN_LIST_TYPE_SOURCE_DF5 = EKRAN_LIST_TYPE_SOURCE_DF5,
  ONM_EKRAN_LIST_TYPE_SOURCE_DF6 = EKRAN_LIST_TYPE_SOURCE_DF6,
  ONM_EKRAN_LIST_TYPE_SOURCE_DF7 = EKRAN_LIST_TYPE_SOURCE_DF7,
  ONM_EKRAN_LIST_TYPE_SOURCE_DF8 = EKRAN_LIST_TYPE_SOURCE_DF8,
  ONM_EKRAN_RANGUVANNJA_DF1_PLUS = EKRAN_RANGUVANNJA_DF1_PLUS,
  ONM_EKRAN_RANGUVANNJA_DF1_MINUS = EKRAN_RANGUVANNJA_DF1_MINUS,
  ONM_EKRAN_RANGUVANNJA_DF1_BLK = EKRAN_RANGUVANNJA_DF1_BLK,
  ONM_EKRAN_RANGUVANNJA_DF2_PLUS = EKRAN_RANGUVANNJA_DF2_PLUS,
  ONM_EKRAN_RANGUVANNJA_DF2_MINUS = EKRAN_RANGUVANNJA_DF2_MINUS,
  ONM_EKRAN_RANGUVANNJA_DF2_BLK = EKRAN_RANGUVANNJA_DF2_BLK,
  ONM_EKRAN_RANGUVANNJA_DF3_PLUS = EKRAN_RANGUVANNJA_DF3_PLUS,
  ONM_EKRAN_RANGUVANNJA_DF3_MINUS = EKRAN_RANGUVANNJA_DF3_MINUS,
  ONM_EKRAN_RANGUVANNJA_DF3_BLK = EKRAN_RANGUVANNJA_DF3_BLK,
  ONM_EKRAN_RANGUVANNJA_DF4_PLUS = EKRAN_RANGUVANNJA_DF4_PLUS,
  ONM_EKRAN_RANGUVANNJA_DF4_MINUS = EKRAN_RANGUVANNJA_DF4_MINUS,
  ONM_EKRAN_RANGUVANNJA_DF4_BLK = EKRAN_RANGUVANNJA_DF4_BLK,
  ONM_EKRAN_RANGUVANNJA_DF5_PLUS = EKRAN_RANGUVANNJA_DF5_PLUS,
  ONM_EKRAN_RANGUVANNJA_DF5_MINUS = EKRAN_RANGUVANNJA_DF5_MINUS,
  ONM_EKRAN_RANGUVANNJA_DF5_BLK = EKRAN_RANGUVANNJA_DF5_BLK,
  ONM_EKRAN_RANGUVANNJA_DF6_PLUS = EKRAN_RANGUVANNJA_DF6_PLUS,
  ONM_EKRAN_RANGUVANNJA_DF6_MINUS = EKRAN_RANGUVANNJA_DF6_MINUS,
  ONM_EKRAN_RANGUVANNJA_DF6_BLK = EKRAN_RANGUVANNJA_DF6_BLK,
  ONM_EKRAN_RANGUVANNJA_DF7_PLUS = EKRAN_RANGUVANNJA_DF7_PLUS,
  ONM_EKRAN_RANGUVANNJA_DF7_MINUS = EKRAN_RANGUVANNJA_DF7_MINUS,
  ONM_EKRAN_RANGUVANNJA_DF7_BLK = EKRAN_RANGUVANNJA_DF7_BLK,
  ONM_EKRAN_RANGUVANNJA_DF8_PLUS = EKRAN_RANGUVANNJA_DF8_PLUS,
  ONM_EKRAN_RANGUVANNJA_DF8_MINUS = EKRAN_RANGUVANNJA_DF8_MINUS,
  ONM_EKRAN_RANGUVANNJA_DF8_BLK = EKRAN_RANGUVANNJA_DF8_BLK,

  ONM_EKRAN_LIST_D_XOR = EKRAN_LIST_D_XOR,
  ONM_EKRAN_RANGUVANNJA_D_XOR1 = EKRAN_RANGUVANNJA_D_XOR1,
  ONM_EKRAN_RANGUVANNJA_D_XOR2 = EKRAN_RANGUVANNJA_D_XOR2,
  ONM_EKRAN_RANGUVANNJA_D_XOR3 = EKRAN_RANGUVANNJA_D_XOR3,
  ONM_EKRAN_RANGUVANNJA_D_XOR4 = EKRAN_RANGUVANNJA_D_XOR4,
  ONM_EKRAN_RANGUVANNJA_D_XOR5 = EKRAN_RANGUVANNJA_D_XOR5,
  ONM_EKRAN_RANGUVANNJA_D_XOR6 = EKRAN_RANGUVANNJA_D_XOR6,
  ONM_EKRAN_RANGUVANNJA_D_XOR7 = EKRAN_RANGUVANNJA_D_XOR7,
  ONM_EKRAN_RANGUVANNJA_D_XOR8 = EKRAN_RANGUVANNJA_D_XOR8,

  ONM_EKRAN_LIST_D_OR = EKRAN_LIST_D_OR,
  ONM_EKRAN_RANGUVANNJA_D_OR1 = EKRAN_RANGUVANNJA_D_OR1,
  ONM_EKRAN_RANGUVANNJA_D_OR2 = EKRAN_RANGUVANNJA_D_OR2,
  ONM_EKRAN_RANGUVANNJA_D_OR3 = EKRAN_RANGUVANNJA_D_OR3,
  ONM_EKRAN_RANGUVANNJA_D_OR4 = EKRAN_RANGUVANNJA_D_OR4,
  ONM_EKRAN_RANGUVANNJA_D_OR5 = EKRAN_RANGUVANNJA_D_OR5,
  ONM_EKRAN_RANGUVANNJA_D_OR6 = EKRAN_RANGUVANNJA_D_OR6,
  ONM_EKRAN_RANGUVANNJA_D_OR7 = EKRAN_RANGUVANNJA_D_OR7,
  ONM_EKRAN_RANGUVANNJA_D_OR8 = EKRAN_RANGUVANNJA_D_OR8,

  ONM_EKRAN_LIST_D_NOT = EKRAN_LIST_D_NOT,
  ONM_EKRAN_RANGUVANNJA_D_NOT1 = EKRAN_RANGUVANNJA_D_NOT1,
  ONM_EKRAN_RANGUVANNJA_D_NOT2 = EKRAN_RANGUVANNJA_D_NOT2,
  ONM_EKRAN_RANGUVANNJA_D_NOT3 = EKRAN_RANGUVANNJA_D_NOT3,
  ONM_EKRAN_RANGUVANNJA_D_NOT4 = EKRAN_RANGUVANNJA_D_NOT4,
  ONM_EKRAN_RANGUVANNJA_D_NOT5 = EKRAN_RANGUVANNJA_D_NOT5,
  ONM_EKRAN_RANGUVANNJA_D_NOT6 = EKRAN_RANGUVANNJA_D_NOT6,
  ONM_EKRAN_RANGUVANNJA_D_NOT7 = EKRAN_RANGUVANNJA_D_NOT7,
  ONM_EKRAN_RANGUVANNJA_D_NOT8 = EKRAN_RANGUVANNJA_D_NOT8,
  ONM_EKRAN_RANGUVANNJA_D_NOT9 = EKRAN_RANGUVANNJA_D_NOT9,
  ONM_EKRAN_RANGUVANNJA_D_NOT10 = EKRAN_RANGUVANNJA_D_NOT10,
  ONM_EKRAN_RANGUVANNJA_D_NOT11 = EKRAN_RANGUVANNJA_D_NOT11,
  ONM_EKRAN_RANGUVANNJA_D_NOT12 = EKRAN_RANGUVANNJA_D_NOT12,
  ONM_EKRAN_RANGUVANNJA_D_NOT13 = EKRAN_RANGUVANNJA_D_NOT13,
  ONM_EKRAN_RANGUVANNJA_D_NOT14 = EKRAN_RANGUVANNJA_D_NOT14,
  ONM_EKRAN_RANGUVANNJA_D_NOT15 = EKRAN_RANGUVANNJA_D_NOT15,
  ONM_EKRAN_RANGUVANNJA_D_NOT16 = EKRAN_RANGUVANNJA_D_NOT16,

  ONM_EKRAN_LIST_D_AND = EKRAN_LIST_D_AND,
  ONM_EKRAN_RANGUVANNJA_D_AND1 = EKRAN_RANGUVANNJA_D_AND1,
  ONM_EKRAN_RANGUVANNJA_D_AND2 = EKRAN_RANGUVANNJA_D_AND2,
  ONM_EKRAN_RANGUVANNJA_D_AND3 = EKRAN_RANGUVANNJA_D_AND3,
  ONM_EKRAN_RANGUVANNJA_D_AND4 = EKRAN_RANGUVANNJA_D_AND4,
  ONM_EKRAN_RANGUVANNJA_D_AND5 = EKRAN_RANGUVANNJA_D_AND5,
  ONM_EKRAN_RANGUVANNJA_D_AND6 = EKRAN_RANGUVANNJA_D_AND6,
  ONM_EKRAN_RANGUVANNJA_D_AND7 = EKRAN_RANGUVANNJA_D_AND7,
  ONM_EKRAN_RANGUVANNJA_D_AND8 = EKRAN_RANGUVANNJA_D_AND8,

  ONM_EKRAN_COFIGURATION = EKRAN_COFIGURATION,

  ONM_EKRAN_CHOOSE_SETTINGS_UVV = EKRAN_CHOOSE_SETTINGS_UVV,

  ONM_EKRAN_DOPUSK_DV_UVV = EKRAN_DOPUSK_DV_UVV,
  ONM_EKRAN_TYPE_INPUT_UVV = EKRAN_TYPE_INPUT_UVV,
  ONM_EKRAN_TYPE_INPUT_SIGNAL_UVV = EKRAN_TYPE_INPUT_SIGNAL_UVV,
  ONM_EKRAN_TYPE_OUTPUT_UVV = EKRAN_TYPE_OUTPUT_UVV,
  ONM_EKRAN_TYPE_LED_UVV = EKRAN_TYPE_LED_UVV,
  ONM_EKRAN_TYPE_BUTTON_UVV = EKRAN_TYPE_BUTTON_UVV,

  ONM_EKRAN_CHOOSE_SETTINGS_SWITCHER = EKRAN_CHOOSE_SETTINGS_SWITCHER,
  ONM_EKRAN_VIEW_LIST_OF_REGISTRATORS = EKRAN_VIEW_LIST_OF_REGISTRATORS,
  ONM_EKRAN_VIEW_SETTINGS_OF_DIGITAL_REGISTRATORS = EKRAN_VIEW_SETTINGS_OF_DIGITAL_REGISTRATORS,
  ONM_EKRAN_RANGUVANNJA_DIGITAL_REGISTRATOR = EKRAN_RANGUVANNJA_DIGITAL_REGISTRATOR,
  ONM_EKRAN_TIMEOUT_DIGITAL_REGISTRATOR = EKRAN_TIMEOUT_DIGITAL_REGISTRATOR,

  ONM_EKRAN_VIEW_SETTINGS_OF_ANALOG_REGISTRATORS = EKRAN_VIEW_SETTINGS_OF_ANALOG_REGISTRATORS,
  ONM_EKRAN_RANGUVANNJA_ANALOG_REGISTRATOR = EKRAN_RANGUVANNJA_ANALOG_REGISTRATOR,
  ONM_EKRAN_TIMEOUT_ANALOG_REGISTRATOR = EKRAN_TIMEOUT_ANALOG_REGISTRATOR,

  ONM_EKRAN_CONTROL_AR = EKRAN_CONTROL_AR,

  ONM_EKRAN_CHOOSE_SETTINGS_MTZ = EKRAN_CHOOSE_SETTINGS_RPN,
  ONM_EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_MTZ = EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_RPN,
  ONM_EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP2_MTZ = EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP2_RPN,
  ONM_EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP3_MTZ = EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP3_RPN,
  ONM_EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP4_MTZ = EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP4_RPN,
  ONM_EKRAN_CHOOSE_SETTINGS_MTZ04 = EKRAN_CHOOSE_SETTINGS_ZSKH,
  ONM_EKRAN_CHOOSE_SETTINGS_ZDZ = EKRAN_CHOOSE_SETTINGS_ZNKh,
  ONM_EKRAN_CHOOSE_TIMEOUT_GROUP1_ZDZ = EKRAN_CHOOSE_TIMEOUT_GROUP1_ZNKh,
  ONM_EKRAN_CHOOSE_TIMEOUT_GROUP2_ZDZ = EKRAN_CHOOSE_TIMEOUT_GROUP2_ZNKh,
  ONM_EKRAN_CHOOSE_TIMEOUT_GROUP3_ZDZ = EKRAN_CHOOSE_TIMEOUT_GROUP3_ZNKh,
  ONM_EKRAN_CHOOSE_TIMEOUT_GROUP4_ZDZ = EKRAN_CHOOSE_TIMEOUT_GROUP4_ZNKh,
  ONM_EKRAN_CHOOSE_SETTINGS_ACHR_CHAPV = EKRAN_CHOOSE_SETTINGS_ACHR_CHAPV,
  ONM_EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_ACHR_CHAPV = EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_ACHR_CHAPV,
  ONM_EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP2_ACHR_CHAPV = EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP2_ACHR_CHAPV,
  ONM_EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP3_ACHR_CHAPV = EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP3_ACHR_CHAPV,
  ONM_EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP4_ACHR_CHAPV = EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP4_ACHR_CHAPV,
  ONM_EKRAN_CHOOSE_SETTINGS_UROV = EKRAN_CHOOSE_SETTINGS_UROV,
  ONM_EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_UROV = EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_UROV,
  ONM_EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP2_UROV = EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP2_UROV,
  ONM_EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP3_UROV = EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP3_UROV,
  ONM_EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP4_UROV = EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP4_UROV,
  ONM_EKRAN_CHOOSE_SETTINGS_ZOP = EKRAN_CHOOSE_SETTINGS_ZOP,
  ONM_EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_ZOP = EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_ZOP,
  ONM_EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP2_ZOP = EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP2_ZOP,
  ONM_EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP3_ZOP = EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP3_ZOP,
  ONM_EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP4_ZOP = EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP4_ZOP,
  ONM_EKRAN_CHOOSE_SETTINGS_UMIN = EKRAN_CHOOSE_SETTINGS_UMIN,
  ONM_EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_UMIN = EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_UMIN,
  ONM_EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP2_UMIN = EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP2_UMIN,
  ONM_EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP3_UMIN = EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP3_UMIN,
  ONM_EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP4_UMIN = EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP4_UMIN,
  ONM_EKRAN_CHOOSE_SETTINGS_UMAX = EKRAN_CHOOSE_SETTINGS_UMAX,
  ONM_EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_UMAX = EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_UMAX,
  ONM_EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP2_UMAX = EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP2_UMAX,
  ONM_EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP3_UMAX = EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP3_UMAX,
  ONM_EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP4_UMAX = EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP4_UMAX,
  ONM_EKRAN_CHOOSE_SETTINGS_UP = EKRAN_CHOOSE_SETTINGS_UP,
  ONM_EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_UP = EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_UP,
  ONM_EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP2_UP = EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP2_UP,
  ONM_EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP3_UP = EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP3_UP,
  ONM_EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP4_UP = EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP4_UP,
  ONM_EKRAN_CHOOSE_SETTINGS_VMP = EKRAN_CHOOSE_SETTINGS_VMP,

  ONM_EKRAN_CHOSE_SETTINGS = EKRAN_CHOSE_SETTINGS,
  ONM_EKRAN_VIEW_GRUPA_USTAVOK = EKRAN_VIEW_GRUPA_USTAVOK,
  ONM_EKRAN_CHOSE_EXTRA_SETTINGS = EKRAN_CHOSE_EXTRA_SETTINGS,

  ONM_EKRAN_CHOSE_DATA_TIME = EKRAN_CHOSE_DATA_TIME,
  ONM_EKRAN_TIME_ZONE = EKRAN_TIME_ZONE,
  ONM_EKRAN_DST_RULE = EKRAN_DST_RULE,
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
  ONM_EKRAN_SYNCHRO = EKRAN_SYNCHRO,
#endif

  ONM_EKRAN_CHOSE_COMMUNICATION_PARAMETERS = EKRAN_CHOSE_COMMUNICATION_PARAMETERS,
  ONM_EKRAN_VIEW_NAME_OF_CELL = EKRAN_VIEW_NAME_OF_CELL,
  ONM_EKRAN_CHOSE_SETTING_RS485 = EKRAN_CHOSE_SETTING_RS485,
  ONM_EKRAN_PHY_LAYER_RS485 = EKRAN_PHY_LAYER_RS485,
  ONM_EKRAN_PROTOCOL_RS485 = EKRAN_PROTOCOL_RS485,
  ONM_EKRAN_VIEW_SPEED_RS485 = EKRAN_VIEW_SPEED_RS485,
  ONM_EKRAN_VIEW_PARE_RS485 = EKRAN_VIEW_PARE_RS485,
  ONM_EKRAN_VIEW_STOP_BITS_RS485 = EKRAN_VIEW_STOP_BITS_RS485,
  ONM_EKRAN_VIEW_TIMEOUT_RS485 = EKRAN_VIEW_TIMEOUT_RS485,
  ONM_EKRAN_PROTOCOLS_RS485 = EKRAN_PROTOCOLS_RS485,
  ONM_EKRAN_ADDRESS_RS485 = EKRAN_ADDRESS_RS485,
  ONM_EKRAN_CHOSE_SETTING_ETHERNET = EKRAN_CHOSE_SETTING_ETHERNET,
  ONM_EKRAN_SETTING_NETWORK_LAYER_ETHERNET = EKRAN_SETTING_NETWORK_LAYER_ETHERNET,

  ONM_EKRAN_LIST_BUTTONS_FOR_RANGUVANNJA = EKRAN_LIST_BUTTONS_FOR_RANGUVANNJA,

  ONM_EKRAN_RANGUVANNJA_BUTTON_1 = EKRAN_RANGUVANNJA_BUTTON_1,
  ONM_EKRAN_RANGUVANNJA_BUTTON_2 = EKRAN_RANGUVANNJA_BUTTON_2,
  ONM_EKRAN_RANGUVANNJA_BUTTON_3 = EKRAN_RANGUVANNJA_BUTTON_3,
  ONM_EKRAN_RANGUVANNJA_BUTTON_4 = EKRAN_RANGUVANNJA_BUTTON_4,
  ONM_EKRAN_RANGUVANNJA_BUTTON_5 = EKRAN_RANGUVANNJA_BUTTON_5,
  ONM_EKRAN_RANGUVANNJA_BUTTON_6 = EKRAN_RANGUVANNJA_BUTTON_6,

  ONM_EKRAN_SETPOINT_ACHR_CHAPV_GROUP1 = EKRAN_SETPOINT_ACHR_CHAPV_GROUP1,
  ONM_EKRAN_SETPOINT_ACHR_CHAPV_GROUP2 = EKRAN_SETPOINT_ACHR_CHAPV_GROUP2,
  ONM_EKRAN_SETPOINT_ACHR_CHAPV_GROUP3 = EKRAN_SETPOINT_ACHR_CHAPV_GROUP3,
  ONM_EKRAN_SETPOINT_ACHR_CHAPV_GROUP4 = EKRAN_SETPOINT_ACHR_CHAPV_GROUP4,
  ONM_EKRAN_TIMEOUT_ACHR_CHAPV_GROUP1 = EKRAN_TIMEOUT_ACHR_CHAPV_GROUP1,
  ONM_EKRAN_TIMEOUT_ACHR_CHAPV_GROUP2 = EKRAN_TIMEOUT_ACHR_CHAPV_GROUP2,
  ONM_EKRAN_TIMEOUT_ACHR_CHAPV_GROUP3 = EKRAN_TIMEOUT_ACHR_CHAPV_GROUP3,
  ONM_EKRAN_TIMEOUT_ACHR_CHAPV_GROUP4 = EKRAN_TIMEOUT_ACHR_CHAPV_GROUP4,
  ONM_EKRAN_CONTROL_ACHR_CHAPV = EKRAN_CONTROL_ACHR_CHAPV,

  //  ONM_EKRAN_
  //  ONM_EKRAN_
  //  ONM_EKRAN_
  //  ONM_EKRAN_
  //  ONM_EKRAN_
  //  ONM_EKRAN_
  //  ONM_EKRAN_
  //  ONM_EKRAN_
  MAX_AMOUNT_ONM_EKRAN

};

#endif
