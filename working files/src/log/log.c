

#include "header.h"
#include "cmd_log.h"
#include <string.h>
//.7.4 Реєстратор програмних подій
//.7.4.1 Реєстратор програмних подій повинен фіксувати та зберігати
//.інформацію з прив’язуванням до часу та дати про виниклі під час роботи пристрою
//.несправності та збої з фіксацією типу несправності та збою.
//.7.4.2 Пристрій повинен реєструвати всі перезапуски пристрою з
//.прив’язуванням до часу та дати.
//.7.5 Реєстратор статистики
//.7.5.1 Реєстратор дискретних сигналів повинен зберігати не менше 400
//.останніх записів.
//.45
//.7.5.2 Реєстратор статистики повинен фіксувати всі зміни всіх дискретних
//.сигналів приладу з прив’язуванням до часу та дати.
//.7.5.3 При введенні реєстатора статистики в прилад без модернізації схемно
//.конструктивних рішень допускається збереження реєтратора тільки в оперативній
//.пам’яті.
//.
//.MRZS-F2J1 (3 оптодавачі + 61850)
//.281 сигнал - це 36 байт інформації.
//.
//.8 байт - UnixTime мітка (з точністю до секунди)
//.4 байти - мікросекунди (у нас точність мілісекунд, тому число завжди буде кратне 1000) Хоча можна і 2 байти зробити для мілісекунд
//.
//.36 байт - стан дискретних сигналів.
//.
//.Сума 48 байт.
//.
//.48*400 = 19200
//.
//.400 записів + 1 один стартовий = 401 зрізи
//.401 х 48 = 19248 байт = 19 кБайт
void ChekCurrentStateCmd(unsigned int *p_active_functions){
    UNUSED(p_active_functions);
    
    
}
//.що треба 1)База актуальний стан сигналів
//.2) Постійно робиш XOR, як не 0, то заносиш дані
//.Пересовуєш вказівник вперед. 
//SRAM1 CmdPlusTimeHolder holderCmdPlusTime;
SRAM1  CmdPlusTimeHolder holderCmdPlusTime;

//?UNN_CmdState arRawCmd        [100];uint8_t arAmontChangesinSlice[100];  - old Code
UNN_CmdState arRawCmd        [AMOUNT_CMD_PLUS_TIME_STAMP_RECORD];uint8_t arAmontChangesinSlice[AMOUNT_CMD_PLUS_TIME_STAMP_RECORD]; 
//?SRAM1 UNN_CmdState arChangeStateCmd[100];
//?SRAM1 uint8_t arTimeLab            [100];
//SRAM1 
short shIndexWRRawCmd;
short shIndexRDRawCmd = (AMOUNT_CMD_PLUS_TIME_STAMP_RECORD - 1);//99;<- previous val
//uint32_t* pAFVal

static int clean_started = 0;
// @In the steps below I am using a timestamp of 1203161493 
// @which corresponds to a date/time of 2-15-2008 11:31:33. 
// @I'm ignoring timezones and whatnot, it isn't necessary for this.

/*SRAM1 */ CmdPlusTimeStampHolder holderCmdPlusTimeStamp;
static short cmdPlusTimeHolderBusy = 0;//indicate holderCmdPlusTimeStamp now in use


short GetAmountFixElem(void){
    return holderCmdPlusTime.shTotalFixElem;
}
void CmdPlusTimeLogHundler(unsigned int *p_active_functions){
    register long i;
    //UNUSED(p_active_functions);

    if(holderCmdPlusTimeStamp.uLLDrecTimeStampVal >= (FIFTY_YEAR_NUMBER))
        holderCmdPlusTimeStamp.uLLDrecTimeStampVal = 0;
    else
        holderCmdPlusTimeStamp.uLLDrecTimeStampVal++;
    if(clean_started != 0)
        return;
    if(time_dat < 1203161493 )//&& time_ms == 0 
        return;
        
    i = holderCmdPlusTime.shIndexWR;
    if (i == 0)
        i = AMOUNT_CMD_PLUS_TIME_RECORD - 1;
    else 
        i--;
    unsigned long *pCmd = (unsigned long *)&(holderCmdPlusTime.arrCmdPlusTimeHolder[i].cmd.uLCmd[0]);
    bool b_comp = false;       
    for (size_t _i = 0; ( (b_comp == false) && (_i < (N_BIG)) ); ++_i) 
    {                                                          
        b_comp |= (p_active_functions[_i] != pCmd[_i]);               
    }
    if( b_comp
        //?((p_active_functions[0] != pCmd[0])  )||
        //?((p_active_functions[1] != pCmd[1])  )||
        //?((p_active_functions[2] != pCmd[2])  )||
        //?((p_active_functions[3] != pCmd[3])  )||
        //?((p_active_functions[4] != pCmd[4])  )||
        //?((p_active_functions[5] != pCmd[5])  )||
        //?((p_active_functions[6] != pCmd[6])  )||
        //?((p_active_functions[7] != pCmd[7])  )||
        //?((p_active_functions[8] != pCmd[8])  )
    ){
        //asm volatile(
        //    "bkpt 1"
        //    );
        i = holderCmdPlusTime.shIndexWR;
        pCmd = (unsigned long *)&(holderCmdPlusTime.arrCmdPlusTimeHolder[i].cmd.uLCmd[0]);
        //?pCmd[0] = p_active_functions[0] ;
        //?pCmd[1] = p_active_functions[1] ;
        //?pCmd[2] = p_active_functions[2] ;
        //?pCmd[3] = p_active_functions[3] ;
        //?pCmd[4] = p_active_functions[4] ;
        //?pCmd[5] = p_active_functions[5] ;
        //?pCmd[6] = p_active_functions[6] ;
        //?pCmd[7] = p_active_functions[7] ;
        //?pCmd[8] = p_active_functions[8] ;
        memcpy((void *)pCmd,(const void*)p_active_functions,sizeof(UNN_CmdState));
        //Put Time
        holderCmdPlusTime.arrCmdPlusTimeHolder[i].unix_time.time_dat = time_dat;
        //Put Mksec
        holderCmdPlusTime.arrCmdPlusTimeHolder[i].mksec.time_ms      = time_ms;
        //i = CmdPlusTimeHolder.shIndexWR;
        i++;
        if( i >= AMOUNT_CMD_PLUS_TIME_RECORD){
            holderCmdPlusTime.shIndexWR = 0;
            //asm volatile(
            //"bkpt 1"
            //);
        }
        else
            holderCmdPlusTime.shIndexWR = i;
        if (holderCmdPlusTime.shTotalFixElem < AMOUNT_CMD_PLUS_TIME_RECORD)
            holderCmdPlusTime.shTotalFixElem++;
        if (holderCmdPlusTime.u32IDModifyIndexWR < 0xffffffff)
            holderCmdPlusTime.u32IDModifyIndexWR++;
        else
            holderCmdPlusTime.u32IDModifyIndexWR = 0;
    }    
    
}

void GetCmdPlusTimeLogElem(unsigned int *p_elem, long lIdx){
    register long i;
    //?UNUSED(p_active_functions);
    if(lIdx >= AMOUNT_CMD_PLUS_TIME_RECORD || lIdx < 0)
        lIdx = 0;
    if(clean_started != 0)
        return;
    unsigned long id_change_index_wr = holderCmdPlusTime.u32IDModifyIndexWR;    
    bool data_not_Ok = 0;char u8_iter = 0;
    i = holderCmdPlusTime.shIndexWR-1;
    do{
        //?id_change_index_wr = holderCmdPlusTime.u32IDModifyIndexWR;
        i -= lIdx;
    
        if (i < 0)
            i += AMOUNT_CMD_PLUS_TIME_RECORD;
    //void *memcpy(void *restrict s1, const void *restrict s2, size_t n);
        unsigned long *pU32 = &(holderCmdPlusTime.arrCmdPlusTimeHolder[i].cmd.uLCmd[0]);
        memcpy((void *)p_elem,(const void*)pU32,  sizeof(UNN_CmdState));//for avoid situation when change data while reading

        data_not_Ok = id_change_index_wr != holderCmdPlusTime.u32IDModifyIndexWR;     
        u8_iter++;
        if(u8_iter >= 3)
            return ;//3 Leak of resorses
        if(data_not_Ok){
            id_change_index_wr = holderCmdPlusTime.u32IDModifyIndexWR;
             i = holderCmdPlusTime.shIndexWR-1;
        }
            
    }while (data_not_Ok && (u8_iter<3));

}
//?inline void GetCmdPlusTimeLogElemUseLocal(unsigned int *p_elem, long lIdx){
//?    register long i;
//?    //?UNUSED(p_active_functions);
//?    i = holderCmdPlusTime.shIndexWR-1;
//?    i -= lIdx;
//?    
//?    if (i < 0)
//?        i += AMOUNT_CMD_PLUS_TIME_RECORD;
//?//void *memcpy(void *restrict s1, const void *restrict s2, size_t n);
//? 
//?    memcpy((void *)p_elem,(const void*)&(holderCmdPlusTime.arrCmdPlusTimeHolder[i].cmd.uLCmd[0]),
//?    sizeof(UNN_CmdState));
//?
//?}

void GetDateTimeLogElem(unsigned int *p_elem, long lIdx){
    register long i;
    //?UNUSED(p_active_functions);
    if(lIdx >= AMOUNT_CMD_PLUS_TIME_RECORD || lIdx < 0)
        lIdx = 0;
    if(clean_started != 0)
    return;
    unsigned long id_change_index_wr = holderCmdPlusTime.u32IDModifyIndexWR;    
    bool data_not_Ok = 0;char u8_iter = 0;
    i = holderCmdPlusTime.shIndexWR-1;
    do{
        //?id_change_index_wr = holderCmdPlusTime.u32IDModifyIndexWR;
        
        i -= lIdx;
        
        if (i < 0)
            i += AMOUNT_CMD_PLUS_TIME_RECORD;
//vo    id *memcpy(void *restrict s1, const void *restrict s2, size_t n);
        unsigned long *pU32 = &(holderCmdPlusTime.arrCmdPlusTimeHolder[i].unix_time.arU32MkTime[0]);
        memcpy((void *)p_elem,(const void*)pU32,    sizeof(UNN_UnixTime));
        
        data_not_Ok = id_change_index_wr != holderCmdPlusTime.u32IDModifyIndexWR;     
        u8_iter++;
        if(u8_iter >= 3)
            return ;//3Leak of resorses
        if(data_not_Ok){
            id_change_index_wr = holderCmdPlusTime.u32IDModifyIndexWR;
             i = holderCmdPlusTime.shIndexWR-1;
        }
  
    }while (data_not_Ok && (u8_iter<3));
}


    
void GetMsLogElem(unsigned int *p_elem, long lIdx){
    register long i;
    //?UNUSED(p_active_functions);
    if(lIdx >= AMOUNT_CMD_PLUS_TIME_RECORD || lIdx < 0)
        lIdx = 0;
    if(clean_started != 0)
        return;
    unsigned long id_change_index_wr = holderCmdPlusTime.u32IDModifyIndexWR;    
    bool data_not_Ok = 0;char u8_iter = 0;
    i = holderCmdPlusTime.shIndexWR-1;
    do{
        //?id_change_index_wr = holderCmdPlusTime.u32IDModifyIndexWR;
        
        i -= lIdx;
        
        if (i < 0)
            i += AMOUNT_CMD_PLUS_TIME_RECORD;
    //void *memcpy(void *restrict s1, const void *restrict s2, size_t n);
     
        unsigned long *pU32 = &(holderCmdPlusTime.arrCmdPlusTimeHolder[i].mksec.uLMkTime);
        memcpy((void *)p_elem,(const void*)pU32,    sizeof(UNN_MicroSec));
    
        data_not_Ok = id_change_index_wr != holderCmdPlusTime.u32IDModifyIndexWR;     
        u8_iter++;
        if(u8_iter >= 3)
            return ;//3Leak of resorses
        if(data_not_Ok){
            id_change_index_wr = holderCmdPlusTime.u32IDModifyIndexWR;
             i = holderCmdPlusTime.shIndexWR-1;
        }
    }while (data_not_Ok && (u8_iter<3));

}
unsigned int mnu_array_old[N_BIG], mnu_array_new[N_BIG];//, mnu_array_changing[N_BIG];
 unsigned long u32IDModifyIndexWR_copy;//need for inform that data change and incorrect
long GetNumberChangingInLogElem( long lIdx){
    register long i;
    //?UNUSED(p_active_functions);           Errorneus code as this
    //?i = holderCmdPlusTime.shIndexWR-1;    ariphmetic make in lowfunc
    //?i -= lIdx;
    //?
    //?if (i < 0)
    //?    i += AMOUNT_CMD_PLUS_TIME_RECORD;
    if(lIdx >= AMOUNT_CMD_PLUS_TIME_RECORD || lIdx < 0)
        lIdx = 0;
    if(clean_started != 0)
        return 0;//!@_is correct? May be better return error
    unsigned int array_changing[N_BIG];//^^!@ array_old[N_BIG], array_new[N_BIG], 
    //^^!@GetCmdPlusTimeLogElem(array_new ,lIdx);
    //^^!@ memcpy((void *)mnu_array_new,(const void*)array_new,N_BIG*sizeof(unsigned int));
    //i--;                                     Errorneus code as this
    //if (i < 0)                               ariphmetic make in lowfunc
    //    i += AMOUNT_CMD_PLUS_TIME_RECORD;
    //^^!@lIdx++;//Next elem on menu older
    //^^!@if (lIdx >= AMOUNT_CMD_PLUS_TIME_RECORD)
    //^^!@    lIdx -= AMOUNT_CMD_PLUS_TIME_RECORD;
    //^^!@GetCmdPlusTimeLogElem(array_old ,lIdx);
    //^^!@ memcpy((void *)mnu_array_old,(const void*)array_old,N_BIG*sizeof(unsigned int));
     for (unsigned int j = 0; j < N_BIG; j++)
         //?array_changing[j] = array_new[j] ^ array_old[j];
         array_changing[j] = mnu_array_new[j] ^ mnu_array_old[j];
long u32Count = 0;
    for (unsigned int j = 0; j < N_BIG; j++){
        for(i = 0; i < 32; i++)
            if((array_changing[j] &(1<<i)) != 0)
                u32Count++;
        
    }
     //memcpy((void *)mnu_array_changing,(const void*)array_changing,N_BIG*sizeof(unsigned int));        
return u32Count;

}
//?long GetNumberChangingInLogElemUseLocal( long lIdx){
//?    register long i;
//?    //?UNUSED(p_active_functions);           Errorneus code as this
//?    //?i = holderCmdPlusTime.shIndexWR-1;    ariphmetic make in lowfunc
//?    //?i -= lIdx;
//?    //?
//?    //?if (i < 0)
//?    //?    i += AMOUNT_CMD_PLUS_TIME_RECORD;
//?    
//?    unsigned int array_old[N_BIG], array_new[N_BIG], array_changing[N_BIG];
//?    GetCmdPlusTimeLogElemUseLocal(array_new ,lIdx);
//?    //i--;                                     Errorneus code as this
//?    //if (i < 0)                               ariphmetic make in lowfunc
//?    //    i += AMOUNT_CMD_PLUS_TIME_RECORD;
//?    lIdx++;//Next elem on menu older
//?    if (lIdx >= AMOUNT_CMD_PLUS_TIME_RECORD)
//?        lIdx -= AMOUNT_CMD_PLUS_TIME_RECORD;
//?    GetCmdPlusTimeLogElemUseLocal(array_old ,lIdx);
//?    
//?     for (unsigned int j = 0; j < N_BIG; j++)
//?         array_changing[j] = array_new[j] ^ array_old[j];
//?long u32Count = 0;
//?    for (unsigned int j = 0; j < N_BIG; j++){    
//?        for(i = 0; i < 32; i++)
//?            if((array_changing[j] &(1<<i)) != 0)
//?                u32Count++;
//?        
//?    }
//?            
//?return u32Count;
//?
//?}


void CleanCmdPlusTimeLog(void){
    
    if((clean_rejestrators& CLEAN_SR_ERR) != 0){

        if(clean_started == 0)//first block new req
            clean_started++;
 
        if( (cmdPlusTimeHolderBusy == 0) 
            && (clean_rejestrators& CLEAN_SR_ERR)){//check 1)Busy == 0 means all leave resorce 2)check  &CLEAN_SR_ERR still need clear becouse other int can call & clear resorce
            CmdPlusTimeHolder *ph = &holderCmdPlusTime;
            //void *memset(void *s, int c, size_t n);
            memset((void *)&(ph->arrCmdPlusTimeHolder[AMOUNT_CMD_PLUS_TIME_RECORD-1]), (int) 0, sizeof(CmdPlusTimeStateElem) );
            //?memset((void *)&(ph->arrCmdPlusTimeHolder[0]), (int) 0, sizeof(CmdPlusTimeStateElem) );
            ph ->shIndexWR           = 0;
            ph ->shTotalFixElem      = 0;
            ph ->u32IDModifyIndexWR  = 0;
            clean_rejestrators &= (unsigned int)(~CLEAN_SR_ERR);
            clean_started = 0; 
        } 
          
       
    }

}

void CmdPlusTimeStampLogHundler(unsigned int *p_active_functions){

#ifdef __DBG_CODE_  
//Записуємо попередній cтан сигналів перед аварією

#endif    
    register long i;
    register CmdPlusTimeStampHolder *phld  = &holderCmdPlusTimeStamp;
    //UNUSED(p_active_functions);
    
//?    if(time_dat < 1203161493 )//&& time_ms == 0 
//?        return;
    i = holderCmdPlusTimeStamp.shIndexWR;
    if (i == 0)
        i = AMOUNT_CMD_PLUS_TIME_STAMP_RECORD;
    
        i--;
    
    unsigned long *pCmd = (unsigned long *)&(holderCmdPlusTimeStamp.arrCmdPlusTimeStampElem[i].cmd.uLCmd[0]);    
    bool b_comp = false;       
    for (size_t _i = 0; ( (b_comp == false) && (_i < (N_BIG)) ); ++_i) 
    {                                                          
        b_comp |= (p_active_functions[_i] != pCmd[_i]);               
    }
    if( b_comp
        //?((p_active_functions[0] != pCmd[0])  )||
        //?((p_active_functions[1] != pCmd[1])  )||
        //?((p_active_functions[2] != pCmd[2])  )||
        //?((p_active_functions[3] != pCmd[3])  )||
        //?((p_active_functions[4] != pCmd[4])  )||
        //?((p_active_functions[5] != pCmd[5])  )||
        //?((p_active_functions[6] != pCmd[6])  )||
        //?((p_active_functions[7] != pCmd[7])  )||
        //?((p_active_functions[8] != pCmd[8])  )
    ){

        long long lLCurrentDifer  = (phld->arrAdditionalInfoCmdPlusTimeStamp[i].ullTimeStamp);
        if( (phld->uLLDrecTimeStampVal) < (unsigned long long)lLCurrentDifer ){
            lLCurrentDifer = (FIFTY_YEAR_NUMBER - lLCurrentDifer) + (phld->uLLDrecTimeStampVal);
        }
        else{
            lLCurrentDifer = (phld->uLLDrecTimeStampVal) - lLCurrentDifer;
        }
               //asm volatile(
               //"bkpt 1"
               //);
        if(lLCurrentDifer < 100){
            phld->shSumDifersElem += lLCurrentDifer;
            long u,k;
            u = phld->shIndexRD;
            do{
                k = phld->uLLDrecTimeStampVal -
                    (phld->arrAdditionalInfoCmdPlusTimeStamp[u].ullTimeStamp);
                if(k <= 100)    
                    break;
                if( ++u >= AMOUNT_CMD_PLUS_TIME_STAMP_RECORD)   
                        u = 0;  
            }while ( k > 100);
            
            phld->shIndexRD = u;
            phld->shAmount100msElem++;//12:12 2021-07-16 mov from line @!@phld->shAmount100msElem++@@ 23 line down
            if(phld->shSumDifersElem > 100){
                long IdxTimeStamp,j = 0;
                long s = 0;
                if( i < (phld->shAmount100msElem) ){//Idx last elem in 100 ms range
                    j = phld->shAmount100msElem - i;
                    IdxTimeStamp = AMOUNT_CMD_PLUS_TIME_STAMP_RECORD - j;
                }else
                    IdxTimeStamp = i - phld->shAmount100msElem;
                
                //Delete Sum Val which less lLCurrentDifer
                do{
                    s = phld->uLLDrecTimeStampVal -
                    (phld->arrAdditionalInfoCmdPlusTimeStamp[IdxTimeStamp].ullTimeStamp);
                    
                    phld->shAmount100msElem--;
                    if( ++IdxTimeStamp >= AMOUNT_CMD_PLUS_TIME_STAMP_RECORD)   
                        IdxTimeStamp = 0;                                    
                }while( (s >= 100)&& (phld->shAmount100msElem > 0) );
                phld->shSumDifersElem = s;
                
            }
            //else
            //    phld->shAmount100msElem++; @!@phld->shAmount100msElem++@@
        }
        else{
            phld->shAmount100msElem = 0;
            phld->shSumDifersElem = 0;
            phld->shIndexRD = phld->shIndexWR;//..phld->shIndexRD = i;
        }
        //Вираховуємо кількість змін сигналів
          unsigned int number_changes_into_current_item;
        _NUMBER_CHANGES_INTO_UNSIGNED_INT_ARRAY(pCmd, p_active_functions, N_BIG, number_changes_into_current_item);
        
        i = holderCmdPlusTimeStamp.shIndexWR;
        pCmd = (unsigned long *)&(holderCmdPlusTimeStamp.arrCmdPlusTimeStampElem[i].cmd.uLCmd[0]);
        //pCmd[0] = p_active_functions[0] ;
        //pCmd[1] = p_active_functions[1] ;
        //pCmd[2] = p_active_functions[2] ;
        //pCmd[3] = p_active_functions[3] ;
        //pCmd[4] = p_active_functions[4] ;
        //pCmd[5] = p_active_functions[5] ;
        //pCmd[6] = p_active_functions[6] ;
        //pCmd[7] = p_active_functions[7] ;
        //pCmd[8] = p_active_functions[8] ;
        memcpy((void *)pCmd,(const void*)p_active_functions,sizeof(UNN_CmdState));
        //Put Time
        phld->arrCmdPlusTimeStampElem[i].unix_time.time_dat = time_dat;
        //Put Mksec
        phld->arrCmdPlusTimeStampElem[i].mksec.time_ms      = time_ms;
        //i = CmdPlusTimeHolder.shIndexWR;
        (phld->arrAdditionalInfoCmdPlusTimeStamp[i].ullTimeStamp) = (phld->uLLDrecTimeStampVal);
        (phld->arrAdditionalInfoCmdPlusTimeStamp[i].lNumAlterSigInElem) = number_changes_into_current_item; 
        i++;
        if( i >= AMOUNT_CMD_PLUS_TIME_STAMP_RECORD)
            phld->shIndexWR = 0;
        else
            phld->shIndexWR = i;
        if (phld->shTotalFixElem < AMOUNT_CMD_PLUS_TIME_STAMP_RECORD)
            phld->shTotalFixElem++;
        
        //.if( phld->shAmount100msElem == 0 )
        //.    phld->shAmount100msElem += 1;
    }
    
}

//extern unsigned int previous_active_functions[N_BIG];
//extern unsigned int number_items_dr; <- Кількість зрізів
//extern unsigned int number_changes_into_dr_record;
//extern unsigned int time_from_start_record_dr;
extern long tstBrrFooSelector;

void TestStateNameSpaceFooBrrr(DigRegUniqVarsAddreses *ptDRecUniqVarsAddreses ) {
    if(tstBrrFooSelector == 0)
        tstBrrFooSelector++;
    else{
        long i,j,k,l;
        i = *ptDRecUniqVarsAddreses->number_items_dr;
        j = *ptDRecUniqVarsAddreses->number_changes_into_dr_record;
        k = *ptDRecUniqVarsAddreses->time_from_start_record_dr;
        l = *ptDRecUniqVarsAddreses->blocking_continue_monitoring_min_U;
        
        tstBrrFooSelector = i+j+k+l;
    }
}

void put_before_info_in_buf (unsigned char * const buffer_for_save_dr_record, DigRegUniqVarsAddreses *ptDRUVAd){

    //register void* pv;
static  int time_before_start_record_dr = 0;

    //Записуємо попередній cтан сигналів перед аварією
    //Вираховуємо кількість змін сигналів

    //Помічаємо скільки часу пройшло з початку запуску запису
    //!@!*(ptDRUVAd->time_from_start_record_dr) = 0;
    //Вираховуємо кількість змін сигналів
    //!@!*(ptDRUVAd->number_changes_into_dr_record) = 0;
    //Помічаємо кількість нових зрізів
   //!@! *(ptDRUVAd->number_items_dr) = 0; 
    //.   asm volatile(
    //.           "bkpt 1"
    //.           );      
    register long i;
    register CmdPlusTimeStampHolder *phld  = &holderCmdPlusTimeStamp;
    char chAmontRecords = 0;//One prev state
//!@!    chAmontRecords += phld->shAmount100msElem ;//1 - add previos elem
    i = phld->shIndexWR;
    if( i > phld->shIndexRD)
        chAmontRecords += i - phld->shIndexRD;
    else 
        chAmontRecords += (AMOUNT_CMD_PLUS_TIME_STAMP_RECORD - phld->shIndexRD)+ phld->shIndexWR;
    
    if(chAmontRecords > 12)
        chAmontRecords = 12;//threeshould or border -- result desigion between I, Taras, MB 
    //i = phld->shIndexWR;
    //i -= phld->shAmount100msElem + 2;//point first elem in 100ms buf;
    //Popint to previos before first elem in 100ms buf; 2 = 1+1 fierst 1 it shIndexWR second  1 it prev position
    //.if( i < 0 ) 
    //.    i += AMOUNT_CMD_PLUS_TIME_STAMP_RECORD;
    
    
    i = phld->shIndexRD;
    
    unsigned long *pCmd;
//D if(*(ptDRUVAd->number_items_dr) > 0 )
//D     *(ptDRUVAd->number_items_dr) += 1;
     *(ptDRUVAd->number_changes_into_dr_record) = 0;
    *(ptDRUVAd->number_items_dr) = 0; 

    long j = i - 1;//point prev 
        if (j < 0)
            j += AMOUNT_CMD_PLUS_TIME_STAMP_RECORD;
    do{
        pCmd = (unsigned long *)&(holderCmdPlusTimeStamp.arrCmdPlusTimeStampElem[j].cmd.uLCmd[0]);
        long idx_data_dr =  FIRST_INDEX_FIRST_DATA_DR + (*(ptDRUVAd->number_items_dr))*SD_DR;
        
        time_before_start_record_dr = phld->uLLDrecTimeStampVal
          - (phld->arrAdditionalInfoCmdPlusTimeStamp[j].ullTimeStamp);
        //.time_before_start_record_dr *= (-1); <- very bad reaction Taras Sys, system can`t view negative numbers
        if(*(ptDRUVAd->number_items_dr) != 0)
            time_before_start_record_dr *= (-1);//1;  
          //unsigned int number_changes_into_current_item;
          //_NUMBER_CHANGES_INTO_UNSIGNED_INT_ARRAY(((unsigned long *)&(holderCmdPlusTimeStamp.arrCmdPlusTimeStampElem[j].cmd.uLCmd[0])), pCmd, N_BIG, number_changes_into_current_item);
          //number_changes_into_dr_record += number_changes_into_current_item;
        if(*(ptDRUVAd->number_items_dr) != 0)
         *(ptDRUVAd->number_changes_into_dr_record) += phld->arrAdditionalInfoCmdPlusTimeStamp[j].lNumAlterSigInElem;
          //Записуємо текучий cтан сигналів
          //Мітка часу
          //buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*38 +  0] =  time_before_start_record_dr        & 0xff;
          //buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*38 +  1] = (time_before_start_record_dr >> 8 ) & 0xff;
          //buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*38 +  2] = (time_before_start_record_dr >> 16) & 0xff;
        if(*(ptDRUVAd->number_items_dr) != 0){
            buffer_for_save_dr_record[idx_data_dr +  0] =  time_before_start_record_dr        & 0xff;
            buffer_for_save_dr_record[idx_data_dr +  1] = (time_before_start_record_dr >> 8 ) & 0xff;
            buffer_for_save_dr_record[idx_data_dr +  2] = (time_before_start_record_dr >> 16) & 0xff;//(3-0)
        }else{
            buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR +  0] = 0xff;
            buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR +  1] = 0xff;
            buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR +  2] = 0xff;
            buffer_for_save_dr_record[FIRST_INDEX_NUMBER_BEFORE_ITEMS_DR] = (time_before_start_record_dr );
        }
          //Текучий стан сигналів
          //.buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*38 +  3] =  pCmd[0]        & 0xff;
          //.buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*38 +  4] = (pCmd[0] >> 8 ) & 0xff;
          //.buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*38 +  5] = (pCmd[0] >> 16) & 0xff;
          
          //.buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*38 + 32] = (pCmd[7] >> 8 ) & 0xff;
          //.buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*38 + 33] = (pCmd[7] >> 16) & 0xff;
          //.buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*38 + 34] = (pCmd[7] >> 24) & 0xff;
          //.buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*38 + 35] =  pCmd[8]        & 0xff;
        memcpy((void*)&buffer_for_save_dr_record[idx_data_dr +  3],(const void*)&pCmd[0],(NUMBER_BYTES_SAMPLE_DR));//.(36-3)                   
          
          //Кількість змін сигналів у порівнянні із попереднім станом
          //.buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*38 + 36] = number_changes_into_current_item & 0xff;
          //.buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*38 + 37] = (number_changes_into_current_item >> 8) & 0xff;
        if(*(ptDRUVAd->number_items_dr) != 0){
            buffer_for_save_dr_record[idx_data_dr + (3 + NUMBER_BYTES_SAMPLE_DR + 0)] = 
            (phld->arrAdditionalInfoCmdPlusTimeStamp[j].lNumAlterSigInElem) & 0xff;
            
            buffer_for_save_dr_record[idx_data_dr + (3 + NUMBER_BYTES_SAMPLE_DR + 1)] = 
            ((phld->arrAdditionalInfoCmdPlusTimeStamp[j].lNumAlterSigInElem)>> 8) & 0xff;
        }
        else{
            buffer_for_save_dr_record[idx_data_dr + (3 + NUMBER_BYTES_SAMPLE_DR + 0)] =  0;
            buffer_for_save_dr_record[idx_data_dr + (3 + NUMBER_BYTES_SAMPLE_DR + 1)] =  0;
        }
        //Збільшуємо на один кількість нових зрізів
        *(ptDRUVAd->number_items_dr) += 1;
        if( ++j >= AMOUNT_CMD_PLUS_TIME_STAMP_RECORD )//if( ++i >= AMOUNT_CMD_PLUS_TIME_STAMP_RECORD ) 
            j -= AMOUNT_CMD_PLUS_TIME_STAMP_RECORD;   //    i -= AMOUNT_CMD_PLUS_TIME_STAMP_RECORD;  
    }while( --chAmontRecords > 0  );
    
    //Записуємо попередній cтан сигналів перед аварією
       long idx_data_dr =  FIRST_INDEX_FIRST_DATA_DR + (*(ptDRUVAd->number_items_dr))*SD_DR;//.?38
//.    //Мітка часу попереднього стану сигналів до моменту початку запису
//.    buffer_for_save_dr_record[idx_data_dr +  0] = 0xff;
//.    buffer_for_save_dr_record[idx_data_dr +  1] = 0xff;
//.    buffer_for_save_dr_record[idx_data_dr +  2] = 0xff;
//.    //Попередній стан  
//.    memcpy((void*)&buffer_for_save_dr_record[idx_data_dr +  3],(const void*)&((ptDRUVAd->previous_active_functions)[0]),(36-3));   
//.    //Нулем позначаємо у цій позиції кількість змін
//.    buffer_for_save_dr_record[idx_data_dr + 36] = 0;
//.    buffer_for_save_dr_record[idx_data_dr + 37] = 0;
//.    *(ptDRUVAd->number_items_dr)++;

    //      //Вираховуємо кількість змін сигналів
    //      number_changes_into_dr_record = 0;
    //      unsigned int number_changes_into_current_item;
    //      _NUMBER_CHANGES_INTO_UNSIGNED_INT_ARRAY(previous_active_functions, p_active_functions, N_BIG, number_changes_into_current_item);
    //      number_changes_into_dr_record += number_changes_into_current_item;
    
    //pCmd = (unsigned long *)(ptDRUVAd->p_active_functions);//unsigned int *pI32 = (ptDRUVAd->previous_active_functions);
    unsigned long *pI32 = (unsigned long *)(ptDRUVAd->p_active_functions);
    *(ptDRUVAd->time_from_start_record_dr) = 0;
    
    unsigned int number_changes_into_current_item;  
    _NUMBER_CHANGES_INTO_UNSIGNED_INT_ARRAY( (pCmd), (pI32), N_BIG, number_changes_into_current_item);
    //Записуємо текучий cтан сигналів
     idx_data_dr =  FIRST_INDEX_FIRST_DATA_DR + (*(ptDRUVAd->number_items_dr))*SD_DR;
    //      //Мітка часу
    //      buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*38 +  0] =  time_from_start_record_dr        & 0xff;
    //      buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*38 +  1] = (time_from_start_record_dr >> 8 ) & 0xff;
    //      buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*38 +  2] = (time_from_start_record_dr >> 16) & 0xff;
    buffer_for_save_dr_record[idx_data_dr +  0] =  *(ptDRUVAd->time_from_start_record_dr)        & 0xff;
    buffer_for_save_dr_record[idx_data_dr +  1] = (*(ptDRUVAd->time_from_start_record_dr) >> 8 ) & 0xff;
    buffer_for_save_dr_record[idx_data_dr +  2] = (*(ptDRUVAd->time_from_start_record_dr) >> 16) & 0xff;//(3-0)

    //      //Текучий стан сигналів
    //      buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*38 +  3] =  p_active_functions[0]        & 0xff;
    //      buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*38 + 35] =  p_active_functions[8]        & 0xff;
    memcpy((void*)&buffer_for_save_dr_record[idx_data_dr +  3],(const void*)&((pI32)[0]),(NUMBER_BYTES_SAMPLE_DR));//?(36-3)
    
    //      
    //      //Кількість змін сигналів у порівнянні із попереднім станом
    //      buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*38 + 36] = number_changes_into_current_item & 0xff;
    //      buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*38 + 37] = (number_changes_into_current_item >> 8) & 0xff;
    buffer_for_save_dr_record[idx_data_dr + (3 + NUMBER_BYTES_SAMPLE_DR + 0)] = number_changes_into_current_item & 0xff;
    buffer_for_save_dr_record[idx_data_dr + (3 + NUMBER_BYTES_SAMPLE_DR + 1)] = (number_changes_into_current_item >> 8) & 0xff;
    //*(ptDRUVAd->number_items_dr) += 1;
    *(ptDRUVAd->number_changes_into_dr_record) += number_changes_into_current_item;// <<= як бути з доаварійним масивом
     //FillBeforeBufinDirectOrder(ptDRUVAd);
} 

 __root  CmdFunctionDepot* pDbgViewCmd = (CmdFunctionDepot*)&holderCmdPlusTimeStamp.arrCmdPlusTimeStampElem[0];
SRAM1  CmdFunctionDepot arViewCmd;// = holderCmdPlusTimeStamp.arrCmdPlusTimeStampElem[0];




long GetCmdPlusTimeLogElemPlWnum(unsigned int *p_elem, long lIdx,unsigned long ulWorkNumber){
    register long i,d;
    struct {
        char error_code;
    }sLV;
    //?UNUSED(p_active_functions);
    if(clean_started != 0)
        return SLOG_DATA_BLOCKED_CLEAN_FUNC;

    if(lIdx >= AMOUNT_CMD_PLUS_TIME_RECORD || lIdx < 0)
        return SLOG_INVALID_IDX;//Invalid  lIdx
    sLV.error_code = SLOG_OK;
    cmdPlusTimeHolderBusy++;
    unsigned long id_change_index_wr = holderCmdPlusTime.u32IDModifyIndexWR;    
    bool data_not_Ok = 0;char u8_iter = 0;
    i = holderCmdPlusTime.shIndexWR-1;
    do{
        //?id_change_index_wr = holderCmdPlusTime.u32IDModifyIndexWR;
        
        i -= lIdx;
        if (i < 0)
            i += AMOUNT_CMD_PLUS_TIME_RECORD;
    
        if(ulWorkNumber != 0xffffffff){
            if(ulWorkNumber > id_change_index_wr){
                d = (0xffffffff - ulWorkNumber) + id_change_index_wr;//d = ulWorkNumber - id_change_index_wr;
                if(d >= AMOUNT_CMD_PLUS_TIME_RECORD){    
                    sLV.error_code = SLOG_INVALID_L_WNUM;
                    cmdPlusTimeHolderBusy--;return sLV.error_code;
                }
                i -= AMOUNT_CMD_PLUS_TIME_RECORD - d;//lWorkNumber + holderCmdPlusTime.lIDModifyIndexWR;
            }else if(ulWorkNumber <= id_change_index_wr){
                d = id_change_index_wr - ulWorkNumber;
                if(d >= AMOUNT_CMD_PLUS_TIME_RECORD){   
                    sLV.error_code = SLOG_INVALID_L_WNUM;
                    cmdPlusTimeHolderBusy--;return sLV.error_code;
                }//return 2;Invalid  lWorkNumber
                i -= d;
            }           
        }
        if (i <= (-AMOUNT_CMD_PLUS_TIME_RECORD)){//return 3;Invalid  sum lWorkNumber& lIdx
               
                    sLV.error_code = SLOG_INVALID_SUM_IDX_PL_WNUM;
                    cmdPlusTimeHolderBusy--;return sLV.error_code;
                
        }else if (i < 0)
                 i += AMOUNT_CMD_PLUS_TIME_RECORD;
        
        unsigned long *pU32 = &(holderCmdPlusTime.arrCmdPlusTimeHolder[i].cmd.uLCmd[0]);
        memcpy((void *)p_elem,(const void*)pU32,  sizeof(UNN_CmdState));//for avoid situation when change data while reading
        
        data_not_Ok = id_change_index_wr != holderCmdPlusTime.u32IDModifyIndexWR;     
        u8_iter++;
        if(u8_iter >= 3){
            sLV.error_code = SLOG_LACK_COMPUTE_RESORCES;
            cmdPlusTimeHolderBusy--;return sLV.error_code;
        }    //return 3;Leak of resorses
        if(data_not_Ok){
            id_change_index_wr = holderCmdPlusTime.u32IDModifyIndexWR;
             i = holderCmdPlusTime.shIndexWR-1;
        }
    }while ( (data_not_Ok == 1) && (u8_iter < 3) );
    //if(id_change_index_wr != ulWorkNumber)
    //  ERROR? Now data exaxtly differ compare to start reading
    p_elem[sizeof(UNN_CmdState)>>2] = id_change_index_wr;//holderCmdPlusTime.u32IDModifyIndexWR;

    cmdPlusTimeHolderBusy--;
    return sLV.error_code;//0 - normal ret - OK value;
}

long GetDateTimeLogElemPlWnum(unsigned int *p_elem, long lIdx,unsigned long ulWorkNumber){
    register long i,d;
    //?UNUSED(p_active_functions);
    if(clean_started != 0)
        return SLOG_DATA_BLOCKED_CLEAN_FUNC;
    if(lIdx >= AMOUNT_CMD_PLUS_TIME_RECORD || lIdx < 0)
        return SLOG_INVALID_IDX;//Invalid  lIdx
    cmdPlusTimeHolderBusy++;
    unsigned long id_change_index_wr = holderCmdPlusTime.u32IDModifyIndexWR;    
    bool data_not_Ok = 0;char u8_iter = 0;
    i = holderCmdPlusTime.shIndexWR-1;
    
    do{
        i -= lIdx;
        if (i < 0)
            i += AMOUNT_CMD_PLUS_TIME_RECORD;
    
    
        if(ulWorkNumber != 0xffffffff){
            if(ulWorkNumber > id_change_index_wr){
                d = (0xffffffff - ulWorkNumber) + id_change_index_wr;
                if(d >= AMOUNT_CMD_PLUS_TIME_RECORD){
                    cmdPlusTimeHolderBusy--;
                    return SLOG_INVALID_L_WNUM;
                }
                i -= AMOUNT_CMD_PLUS_TIME_RECORD - d;//lWorkNumber + holderCmdPlusTime.lIDModifyIndexWR;
            }else if(ulWorkNumber <= id_change_index_wr){
                d = id_change_index_wr - ulWorkNumber;
                if(d >= AMOUNT_CMD_PLUS_TIME_RECORD){
                    cmdPlusTimeHolderBusy--;
                    return SLOG_INVALID_L_WNUM;
                }
                i -= d;
            }           
        }    
        if (i <= (-AMOUNT_CMD_PLUS_TIME_RECORD)){
            cmdPlusTimeHolderBusy--;
            return SLOG_INVALID_SUM_IDX_PL_WNUM;
        }
        else if (i < 0)
                 i += AMOUNT_CMD_PLUS_TIME_RECORD;    
    
        unsigned long *pU32 = &(holderCmdPlusTime.arrCmdPlusTimeHolder[i].unix_time.arU32MkTime[0]);
        memcpy((void *)p_elem,(const void*)pU32,    sizeof(UNN_UnixTime));
        data_not_Ok = id_change_index_wr != holderCmdPlusTime.u32IDModifyIndexWR;
        u8_iter++;
        if(u8_iter >= 3){
            cmdPlusTimeHolderBusy--;
            return SLOG_LACK_COMPUTE_RESORCES;
        }
        if(data_not_Ok){
            id_change_index_wr = holderCmdPlusTime.u32IDModifyIndexWR;
             i = holderCmdPlusTime.shIndexWR-1;
        }
   
    }while (data_not_Ok && (u8_iter<3));
    //if(id_change_index_wr != ulWorkNumber)
    //  ERROR? Now data exaxtly differ compare to start reading
    p_elem[sizeof(UNN_UnixTime)>>2] = id_change_index_wr;
    cmdPlusTimeHolderBusy--;

    return SLOG_OK;
}


    
long GetMsLogElemPlWnum(unsigned int *p_elem, long lIdx,unsigned long ulWorkNumber){
    register long i,d;
    //?UNUSED(p_active_functions);
    if(clean_started != 0)
        return SLOG_DATA_BLOCKED_CLEAN_FUNC;
    if(lIdx > AMOUNT_CMD_PLUS_TIME_RECORD || lIdx < 0)
        return SLOG_INVALID_IDX;//Invalid  lIdx
    cmdPlusTimeHolderBusy++;
    unsigned long id_change_index_wr = holderCmdPlusTime.u32IDModifyIndexWR;    
    bool data_not_Ok = 0;char u8_iter = 0;
    i = holderCmdPlusTime.shIndexWR-1;
    do{
    //?id_change_index_wr = holderCmdPlusTime.u32IDModifyIndexWR;
        i -= lIdx;
        if (i < 0)
            i += AMOUNT_CMD_PLUS_TIME_RECORD;
    
        if(ulWorkNumber != 0xffffffff){
            if(ulWorkNumber > id_change_index_wr){
                d = (0xffffffff - ulWorkNumber) + id_change_index_wr;//d = ulWorkNumber - id_change_index_wr;
                if(d >= AMOUNT_CMD_PLUS_TIME_RECORD){
                    cmdPlusTimeHolderBusy--;
                    return SLOG_INVALID_L_WNUM;
                }

                i -= AMOUNT_CMD_PLUS_TIME_RECORD - d;//lWorkNumber + holderCmdPlusTime.lIDModifyIndexWR;
            }else if(ulWorkNumber <= id_change_index_wr){
                d = id_change_index_wr - ulWorkNumber;
                if(d >= AMOUNT_CMD_PLUS_TIME_RECORD){
                    cmdPlusTimeHolderBusy--;
                    return SLOG_INVALID_L_WNUM;
                }
                    
                i -= d;
            }           
            
        
        }    
        if (i <= (-AMOUNT_CMD_PLUS_TIME_RECORD)){
            cmdPlusTimeHolderBusy--;
            return SLOG_INVALID_SUM_IDX_PL_WNUM;
        }            
        else if (i < 0)
                 i += AMOUNT_CMD_PLUS_TIME_RECORD;
    
 
        unsigned long *pU32 = &(holderCmdPlusTime.arrCmdPlusTimeHolder[i].mksec.uLMkTime);
        memcpy((void *)p_elem,(const void*)pU32,    sizeof(UNN_MicroSec));
  
        data_not_Ok = id_change_index_wr != holderCmdPlusTime.u32IDModifyIndexWR;     
        u8_iter++;
        if(u8_iter >= 3){
            cmdPlusTimeHolderBusy--;
            return SLOG_LACK_COMPUTE_RESORCES;
        }
        if(data_not_Ok){
            id_change_index_wr = holderCmdPlusTime.u32IDModifyIndexWR;
             i = holderCmdPlusTime.shIndexWR-1;
        }
    }while (data_not_Ok && (u8_iter<3));
    p_elem[sizeof(UNN_MicroSec)>>2] = id_change_index_wr;
    cmdPlusTimeHolderBusy--;
    return SLOG_OK;
}

//?typedef struct Queue_tag {
//?    int  size;//front, rear
//?    unsigned capacity;
//?    uint8_t arTimeLab            [100];
//?} QueueChangeMarker;

QueueChangeMarker hldQueueChangeMarker;
int front = -1;
int rear  = -1; //front-1;
#pragma optimize=none
void InithldQueueChangeMarker(void){

 hldQueueChangeMarker.size = SIZE_QUUE ;
 hldQueueChangeMarker.existent_elem = 0;hldQueueChangeMarker.amount_elem = 0;
 front = 0;rear = 0;//rear = SIZE_QUUE -1;
    if(front > rear)
        hldQueueChangeMarker.capacity  =  (front - rear) - 1;
    else
        hldQueueChangeMarker.capacity  = (SIZE_QUUE - 1) - rear + front;
}

#pragma optimize=none
int enQueue(int value){
    //   asm volatile(
    //           "bkpt 1"
    //           );
    //long l = 0;
    if( front == -1 && rear == -1){
        InithldQueueChangeMarker();
        //hldQueueChangeMarker.arTimeLab[rear] = value;
        //enQueue(value);deQueue();
    
    
    }
    long l = hldQueueChangeMarker.amount_elem;
    if(++l < hldQueueChangeMarker.size ){
        hldQueueChangeMarker.arTimeLab[front++] = value;
        if( front >= SIZE_QUUE)
            front = 0;
    }
    else{
       //asm volatile(
       //        "bkpt 1"
       //        );
        hldQueueChangeMarker.arTimeLab[front++] = value;
        if( front >= SIZE_QUUE)
            front = 0;
        rear = front + 1;
        if (rear  >= SIZE_QUUE)
            rear = 0;
        l = SIZE_QUUE-1;
    }
    hldQueueChangeMarker.amount_elem  = l;
    return 0;
} 

#pragma optimize=none
int deQueue(void){
    long l = hldQueueChangeMarker.amount_elem;//front must be = rear + amount;
      //asm volatile(
      //        "bkpt 1"
      //        );    
    if(l > 0){
        int value = hldQueueChangeMarker.arTimeLab[rear];
        hldQueueChangeMarker.arTimeLab[rear++] = 0xcf-(l&7);//!@Debug Code
        hldQueueChangeMarker.amount_elem = l - 1;
        if (rear  >= SIZE_QUUE)
            rear = 0;
        return value;
    }
  
return -1;
 
}
#pragma optimize=none
int getFromFront(int distance){
    if( (rear != front-1) && (front > 0) && (rear < SIZE_QUUE-1)){
        int capacity = 0;  int index = 0;
        if(front > rear)
            capacity  =  (front - rear) - 1;
        else
            capacity  = (SIZE_QUUE - 1) - rear + front;   
        if(distance <= capacity){
            if(distance < front)  {
                index = front - distance;
            }
            else{
                index = front - 1;
                index = (SIZE_QUUE-1) + front - distance;
            }
            return hldQueueChangeMarker.arTimeLab[index];
        }
    }
        asm volatile(
               "bkpt 1"
               );   
   return -1;
}
#pragma optimize=none
int getFromRear(int distance){
    if(distance < hldQueueChangeMarker.amount_elem){
        int index = rear + distance;
        if(index > (SIZE_QUUE-1))  {
            index -= (SIZE_QUUE);
        }
        return hldQueueChangeMarker.arTimeLab[index];
    }
 
   return -1;
}

#pragma optimize=none
void PuCmdinRawBuf(unsigned int *p_active_functions){
//!@static int dbg_off, dbg_retval, dbg_dequeue,dbg_enqueue;
    register long i;
    UNUSED(p_active_functions);
    bool b_comp = false; 
    if(hldQueueChangeMarker.size == 0)
        InithldQueueChangeMarker();
    unsigned long *pCmd = (unsigned long *)&arRawCmd[shIndexRDRawCmd];//shIndexRDRawCmd+dbg_off
    for (size_t _i = 0; ( (b_comp == false) && (_i < (N_BIG)) ); ++_i) 
    {                                                          
        b_comp |= (p_active_functions[_i] != pCmd[_i]);               
    }
    i = shIndexWRRawCmd;//i = shIndexWRRawCmd;
    if( b_comp){
               //Вираховуємо кількість змін сигналів
          unsigned int number_changes_into_current_item;
        _NUMBER_CHANGES_INTO_UNSIGNED_INT_ARRAY(pCmd, p_active_functions, N_BIG, number_changes_into_current_item);
        //put in queue 
        if( number_changes_into_current_item > 0 ){ 
          long prev = getFromRear(0);
          if( (prev != (-1)) && (prev == (i+3) ) )//prev > (i+3)
          {
            //asm volatile(
            //   "bkpt 1"
            //   );  
            deQueue();   
          }
            enQueue(i+3);//!@dbg_retval = getFromRear(0);
        }
        arAmontChangesinSlice[i] = number_changes_into_current_item;
    }
    else{
        arAmontChangesinSlice[i] = 0;
        long l = getFromRear(0);
        if( l != (-1) && l == (i+3)){
            deQueue();            
        }
    }
    shIndexRDRawCmd = i;
    memcpy((void *)&arRawCmd[shIndexWRRawCmd],(const void*)p_active_functions,sizeof(UNN_CmdState));    
        i++;
    if( i >= AMOUNT_CMD_PLUS_TIME_STAMP_RECORD)
        shIndexWRRawCmd = 0;
    else
        shIndexWRRawCmd = i;
    
//?    if(dbg_dequeue >0 )
//?		deQueue();
//?	if(dbg_enqueue > 0 )
//?		enQueue(shIndexRDRawCmd+3);

}
/*
typedef signed char int8_t;typedef short int16_t;typedef long int32_t;typedef long long int64_t;
typedef unsigned char uint8_t;typedef unsigned short uint16_t;typedef unsigned long uint32_t;typedef unsigned long long uint64_t;
*/
#define DELTA_TIME_FOR_TIMERS_DR 2
#pragma optimize=none
void put_before_info_in_buf_from_queue (unsigned char * const buffer_for_save_dr_record, DigRegUniqVarsAddreses *ptDRUVAd){
    
static  char fierst_time_before_start_record_dr_selector = 0;
static  int time_before_start_record_dr = 0;

    //Записуємо попередній cтан сигналів перед аварією
    //Вираховуємо кількість змін сигналів
    //Помічаємо скільки часу пройшло з початку запуску запису
    //!@!*(ptDRUVAd->time_from_start_record_dr) = 0;
    //Вираховуємо кількість змін сигналів
    //!@!*(ptDRUVAd->number_changes_into_dr_record) = 0;
    //Помічаємо кількість нових зрізів
   //!@! *(ptDRUVAd->number_items_dr) = 0; 
    //.   asm volatile(
    //.           "bkpt 1"
    //.           );      
    register long j;
    QueueChangeMarker *phld  = &hldQueueChangeMarker;
    uint32_t* pCmd = (uint32_t *)&(arRawCmd[shIndexRDRawCmd]);
    
    j = getFromRear(0);
    j -= 3;
    if(j == shIndexWRRawCmd ){//Max Val <-This is wrong situation as we have lost state cmd
        deQueue();//!--  //@Exclude possible last elem
    }
    char chAmontRecords = phld->amount_elem;
        if( chAmontRecords > (MAX_EVENTS_IN_ONE_RECORD - 1) ){
          asm volatile(
            "bkpt 1"
          );
          do{
              deQueue();
              chAmontRecords = phld->amount_elem;
               
          }while( chAmontRecords > (MAX_EVENTS_IN_ONE_RECORD - 1) );//
        }
        //Now  chAmontRecords = (MAX_EVENTS_IN_ONE_RECORD - 1);
        
    *(ptDRUVAd->number_changes_into_dr_record) = 0;
    *(ptDRUVAd->number_items_dr) = 0;
    time_before_start_record_dr = 0;//!--  //@AddidtionalzZeroed
    if(
        (chAmontRecords == 0) 
        || state_dr_record == STATE_DR_FORCE_START_NEW_RECORD
      )
    {
        buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR +  0] = 0x00;//?0xff;
        buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR +  1] = 0x00;//?0xff;
        buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR +  2] = 0x80;//?0xff;
        buffer_for_save_dr_record[FIRST_INDEX_NUMBER_BEFORE_ITEMS_DR] = (time_before_start_record_dr*(DELTA_TIME_FOR_TIMERS_DR) );
        buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + (3 + NUMBER_BYTES_SAMPLE_DR + 0)] =  0;
        buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + (3 + NUMBER_BYTES_SAMPLE_DR + 1)] =  0;
          //Текучий стан сигналів
        memcpy((void*)&buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR +  3],(const void*)&arRawCmd[shIndexRDRawCmd],(NUMBER_BYTES_SAMPLE_DR));
        //Збільшуємо на один кількість нових зрізів
        *(ptDRUVAd->number_items_dr) += 1;
    }else
    do{
        j = getFromRear(0);
        j -= 3;
        long idx_data_dr =  FIRST_INDEX_FIRST_DATA_DR + (*(ptDRUVAd->number_items_dr))*SD_DR;
        if(*(ptDRUVAd->number_items_dr) == 0){
            if (--j < 0)
            j += AMOUNT_CMD_PLUS_TIME_STAMP_RECORD;//100;
            chAmontRecords++;
            if( j == shIndexWRRawCmd)//j == shIndexRDRawCmd ||
            {
                fierst_time_before_start_record_dr_selector = 1;
               //asm volatile(
               //"bkpt 1"
               //);      
            }
        }
        pCmd =  (uint32_t *)&arRawCmd[j];
        if(j <=  shIndexWRRawCmd)
            time_before_start_record_dr = j;
        else
            time_before_start_record_dr = j - AMOUNT_CMD_PLUS_TIME_STAMP_RECORD;
        time_before_start_record_dr -= shIndexWRRawCmd;
        
         unsigned int number_changes_into_current_item = arAmontChangesinSlice[j];
        if(*(ptDRUVAd->number_items_dr) != 0)
         *(ptDRUVAd->number_changes_into_dr_record) += number_changes_into_current_item;//
          //Записуємо текучий cтан сигналів
          //Мітка часу
        if(*(ptDRUVAd->number_items_dr) != 0){
            buffer_for_save_dr_record[idx_data_dr +  0] =  (time_before_start_record_dr*(DELTA_TIME_FOR_TIMERS_DR))        & 0xff;// time_before_start_record_dr        & 0xff;
            buffer_for_save_dr_record[idx_data_dr +  1] = ((time_before_start_record_dr*(DELTA_TIME_FOR_TIMERS_DR)) >> 8 ) & 0xff;//(time_before_start_record_dr >> 8 ) & 0xff;
            buffer_for_save_dr_record[idx_data_dr +  2] = ((time_before_start_record_dr*(DELTA_TIME_FOR_TIMERS_DR)) >> 16) & 0xff;//(time_before_start_record_dr >> 16) & 0xff;//
        }else{
            buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR +  0] = 0x00;//?0xff;
            buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR +  1] = 0x00;//?0xff;
            buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR +  2] = 0x80;//?0xff;
            if(fierst_time_before_start_record_dr_selector == 0){
                time_before_start_record_dr += 1; time_before_start_record_dr *=-1 ;
                buffer_for_save_dr_record[FIRST_INDEX_NUMBER_BEFORE_ITEMS_DR] = (time_before_start_record_dr*(DELTA_TIME_FOR_TIMERS_DR) );
            }else{
                buffer_for_save_dr_record[FIRST_INDEX_NUMBER_BEFORE_ITEMS_DR] = (AMOUNT_CMD_PLUS_TIME_STAMP_RECORD-1)*(DELTA_TIME_FOR_TIMERS_DR);//AMOUNT_CMD_PLUS_TIME_STAMP_RECORD-1;
                fierst_time_before_start_record_dr_selector = 0;
            }
        }
          //Текучий стан сигналів
        memcpy((void*)&buffer_for_save_dr_record[idx_data_dr +  3],(const void*)&pCmd[0],(NUMBER_BYTES_SAMPLE_DR));

        if(*(ptDRUVAd->number_items_dr) != 0){
            buffer_for_save_dr_record[idx_data_dr + (3 + NUMBER_BYTES_SAMPLE_DR + 0)] = number_changes_into_current_item & 0xff;
            buffer_for_save_dr_record[idx_data_dr + (3 + NUMBER_BYTES_SAMPLE_DR + 1)] = (number_changes_into_current_item >> 8) & 0xff;
            j = deQueue();
        }
        else{
            buffer_for_save_dr_record[idx_data_dr + (3 + NUMBER_BYTES_SAMPLE_DR + 0)] =  0;
            buffer_for_save_dr_record[idx_data_dr + (3 + NUMBER_BYTES_SAMPLE_DR + 1)] =  0;
        }
 
        if( ( *(ptDRUVAd->number_items_dr) +1) < (MAX_EVENTS_IN_ONE_RECORD - 1) ){
        //Збільшуємо на один кількість нових зрізів
          *(ptDRUVAd->number_items_dr) += 1;
        }
        else{
          ;
        }
 
    }while( (--chAmontRecords > 0) && ( j!= (-1)) );
    
    //Записуємо попередній cтан сигналів перед аварією
    //long idx_data_dr =  FIRST_INDEX_FIRST_DATA_DR + (*(ptDRUVAd->number_items_dr))*SD_DR;//
    
    //.asm volatile(
    //.    "bkpt 1"
    //.    ); 
    
    unsigned long *pI32 = (unsigned long *)(ptDRUVAd->p_active_functions);
    *(ptDRUVAd->time_from_start_record_dr) = 0;
    
    unsigned int number_changes_into_current_item;  
    _NUMBER_CHANGES_INTO_UNSIGNED_INT_ARRAY( (pCmd), (pI32), N_BIG, number_changes_into_current_item);
    //Записуємо текучий cтан сигналів
     long idx_data_dr =  FIRST_INDEX_FIRST_DATA_DR + (*(ptDRUVAd->number_items_dr))*SD_DR;
     
    if( idx_data_dr < (long)(SIZE_BUFFER_FOR_DR_RECORD - SD_DR))
    {
      //      //Мітка часу
      buffer_for_save_dr_record[idx_data_dr +  0] = 0;// *(ptDRUVAd->time_from_start_record_dr)        & 0xff;
      buffer_for_save_dr_record[idx_data_dr +  1] = 0;//(*(ptDRUVAd->time_from_start_record_dr) >> 8 ) & 0xff;
      buffer_for_save_dr_record[idx_data_dr +  2] = 0;//(*(ptDRUVAd->time_from_start_record_dr) >> 16) & 0xff;//
      //
      //      //Текучий стан сигналів
      memcpy((void*)&buffer_for_save_dr_record[idx_data_dr +  3],(const void*)&((pI32)[0]),(NUMBER_BYTES_SAMPLE_DR));//
      
      //      
      //      //Кількість змін сигналів у порівнянні із попереднім станом
      buffer_for_save_dr_record[idx_data_dr + (3 + NUMBER_BYTES_SAMPLE_DR + 0)] = number_changes_into_current_item & 0xff;
      buffer_for_save_dr_record[idx_data_dr + (3 + NUMBER_BYTES_SAMPLE_DR + 1)] = (number_changes_into_current_item >> 8) & 0xff;
      //*(ptDRUVAd->number_items_dr) += 1;
      *(ptDRUVAd->number_changes_into_dr_record) += number_changes_into_current_item;// <<= як бути з доаварійним масивом
    }
    else{
      return;//Index out of the scope
    }
     
}

void FillBeforeBufinDirectOrder(unsigned char * const buffer_for_save_dr_record, DigRegUniqVarsAddreses *ptDRUVAd){
  static  int time_before_start_record_dr = 0;
  static  char prev_time_before_start_record_dr = 0;
  static  int idx_100ms;
  static  int need_scan_in_reverse_order = 0;
  uint32_t number_changes_into_current_item;

    need_scan_in_reverse_order = time_before_start_record_dr = prev_time_before_start_record_dr = 0;
  //Помічаємо скільки часу пройшло з початку запуску запису
  *(ptDRUVAd->time_from_start_record_dr) = 0;
  //Вираховуємо кількість змін сигналів
  *(ptDRUVAd->number_changes_into_dr_record) = 0;
  //Помічаємо кількість нових зрізів
  *(ptDRUVAd->number_items_dr) = 0; 

    int32_t i;
    uint32_t* pU32,*pCmd,*p_last_save;

    asm volatile(
        "bkpt 1"
        );    
    p_last_save = pU32 = (uint32_t *)(ptDRUVAd->p_active_functions);
    
    idx_100ms = i = shIndexWRRawCmd;
    char chAmontRecords = AMOUNT_CMD_PLUS_TIME_STAMP_RECORD;
    pCmd = (uint32_t *)&(arRawCmd[0]);
    do{
        
        long idx_data_dr =  FIRST_INDEX_FIRST_DATA_DR + (*(ptDRUVAd->number_items_dr))*SD_DR;
        //Calc Time
        //time_before_start_record_dr = //uLLDrecTimeStampVal - (arrAdditionalInfoCmdPlusTimeStamp[j].ullTimeStamp);    
        
        if(*(ptDRUVAd->number_items_dr) != 0){
            
            pU32 = (uint32_t *)&(arRawCmd[idx_100ms]);
            pCmd = (uint32_t *)&(arRawCmd[i]);
             _NUMBER_CHANGES_INTO_UNSIGNED_INT_ARRAY( (pCmd), (pU32), N_BIG, number_changes_into_current_item);
             
            if(number_changes_into_current_item > 0) {
                *(ptDRUVAd->number_changes_into_dr_record) += number_changes_into_current_item;
                
                if(idx_100ms <= shIndexWRRawCmd)
                    time_before_start_record_dr = idx_100ms; 
                else
                    time_before_start_record_dr = idx_100ms - AMOUNT_CMD_PLUS_TIME_STAMP_RECORD;
                time_before_start_record_dr -= shIndexWRRawCmd;
                if (prev_time_before_start_record_dr++ == 0){
                    buffer_for_save_dr_record[FIRST_INDEX_NUMBER_BEFORE_ITEMS_DR] = (time_before_start_record_dr );
                }
                //time_before_start_record_dr *= (-1);//1;
                buffer_for_save_dr_record[idx_data_dr +  0] =  time_before_start_record_dr        & 0xff;
                buffer_for_save_dr_record[idx_data_dr +  1] = (time_before_start_record_dr >> 8 ) & 0xff;
                buffer_for_save_dr_record[idx_data_dr +  2] = (time_before_start_record_dr >> 16) & 0xff;//(3-0)
                
                buffer_for_save_dr_record[idx_data_dr + (3 + NUMBER_BYTES_SAMPLE_DR + 0)] = 
                number_changes_into_current_item & 0xff;
                
                buffer_for_save_dr_record[idx_data_dr + (3 + NUMBER_BYTES_SAMPLE_DR + 1)] = 
                (number_changes_into_current_item >> 8) & 0xff;
                //?memcpy((void*)&buffer_for_save_dr_record[idx_data_dr +  3],(const void*)&pCmd[0],(NUMBER_BYTES_SAMPLE_DR));
                memcpy((void*)&buffer_for_save_dr_record[idx_data_dr +  3],(const void*)&pU32[0],(NUMBER_BYTES_SAMPLE_DR));
                 *(ptDRUVAd->number_items_dr) += 1;p_last_save = pU32;
                if (*(ptDRUVAd->number_items_dr) >= MAX_EVENTS_IN_ONE_RECORD - 1){
                    need_scan_in_reverse_order++;
                    time_before_start_record_dr = prev_time_before_start_record_dr = 0;
                        asm volatile(
                        "bkpt 1"
                        );
                }
            } 
            i = idx_100ms;
        }else{
            buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR +  0] = 0x00;//time_before_start_record_dr
            buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR +  1] = 0x00;//time_before_start_record_dr
            buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR +  2] = 0x80;//time_before_start_record_dr
            buffer_for_save_dr_record[FIRST_INDEX_NUMBER_BEFORE_ITEMS_DR] = (time_before_start_record_dr );
            buffer_for_save_dr_record[idx_data_dr + (3 + NUMBER_BYTES_SAMPLE_DR + 0)] =  0;
            buffer_for_save_dr_record[idx_data_dr + (3 + NUMBER_BYTES_SAMPLE_DR + 1)] =  0;
            memcpy((void*)&buffer_for_save_dr_record[idx_data_dr +  3],(const void*)&(arRawCmd[shIndexWRRawCmd]),(NUMBER_BYTES_SAMPLE_DR));
             *(ptDRUVAd->number_items_dr) += 1;
             p_last_save = (uint32_t *)&(arRawCmd[shIndexWRRawCmd]);
        }
        
        
    
        if( ++idx_100ms >= AMOUNT_CMD_PLUS_TIME_STAMP_RECORD )//if( ++i >= AMOUNT_CMD_PLUS_TIME_STAMP_RECORD ) 
            idx_100ms -= AMOUNT_CMD_PLUS_TIME_STAMP_RECORD;   //    i -= AMOUNT_CMD_PLUS_TIME_STAMP_RECORD;  
    
        
        
    }while( --chAmontRecords > 0 && need_scan_in_reverse_order == 0 );
    asm volatile(
        "bkpt 1"
        );  
    if(need_scan_in_reverse_order != 0){
        p_last_save = FillBeforeBufReverseOrder(buffer_for_save_dr_record, ptDRUVAd);
    }
    asm volatile(
        "bkpt 1"
        );  
    long idx_data_dr =  FIRST_INDEX_FIRST_DATA_DR + (*(ptDRUVAd->number_items_dr))*SD_DR;

    unsigned long *pI32 = (unsigned long *)(ptDRUVAd->p_active_functions);
    *(ptDRUVAd->time_from_start_record_dr) = 0;
    //
    //unsigned int number_changes_into_current_item;  
    _NUMBER_CHANGES_INTO_UNSIGNED_INT_ARRAY( (p_last_save), (pI32), N_BIG, number_changes_into_current_item);
    //Записуємо текучий cтан сигналів
     idx_data_dr =  FIRST_INDEX_FIRST_DATA_DR + (*(ptDRUVAd->number_items_dr))*SD_DR;
    //      //Мітка часу
    buffer_for_save_dr_record[idx_data_dr +  0] =  *(ptDRUVAd->time_from_start_record_dr)        & 0xff;
    buffer_for_save_dr_record[idx_data_dr +  1] = (*(ptDRUVAd->time_from_start_record_dr) >> 8 ) & 0xff;
    buffer_for_save_dr_record[idx_data_dr +  2] = (*(ptDRUVAd->time_from_start_record_dr) >> 16) & 0xff;//(3-0)
    //      //Текучий стан сигналів
    memcpy((void*)&buffer_for_save_dr_record[idx_data_dr +  3],(const void*)&((pI32)[0]),(NUMBER_BYTES_SAMPLE_DR));//?(36-3)

    //      //Кількість змін сигналів у порівнянні із попереднім станом
    buffer_for_save_dr_record[idx_data_dr + (3 + NUMBER_BYTES_SAMPLE_DR + 0)] = number_changes_into_current_item & 0xff;
    buffer_for_save_dr_record[idx_data_dr + (3 + NUMBER_BYTES_SAMPLE_DR + 1)] = (number_changes_into_current_item >> 8) & 0xff;

    *(ptDRUVAd->number_changes_into_dr_record) += number_changes_into_current_item;

}

void* FillBeforeBufReverseOrder(unsigned char * const buffer_for_save_dr_record, DigRegUniqVarsAddreses *ptDRUVAd){

  static  int time_before_start_record_dr = 0;  
  static  int idx_100ms;
  uint32_t number_changes_into_current_item;
  uint32_t amount_changed_elements = MAX_EVENTS_IN_ONE_RECORD - 1;
    asm volatile(
        "bkpt 1"
        );    
    int32_t i;
    uint32_t* pU32,*pCmd,*p_last_save;
    *(ptDRUVAd->number_items_dr) = 0;
    *(ptDRUVAd->number_changes_into_dr_record) = 0;//?
    p_last_save = pU32 = (uint32_t *)(ptDRUVAd->p_active_functions);
    char chAmontRecords = AMOUNT_CMD_PLUS_TIME_STAMP_RECORD ;//- 1
    i = shIndexWRRawCmd;//- 1 current
    //if(i < 0)
    //    i += AMOUNT_CMD_PLUS_TIME_STAMP_RECORD;
    i-- ;//Curr
    if(i < 0)
        i += AMOUNT_CMD_PLUS_TIME_STAMP_RECORD;
    idx_100ms = i;
    i-- ;//prev
    if(i < 0)
        i += AMOUNT_CMD_PLUS_TIME_STAMP_RECORD;
    int32_t j = idx_100ms; idx_100ms = i; i = j;
    //?*(ptDRUVAd->number_items_dr) = MAX_EVENTS_IN_ONE_RECORD - 1;
    do{
        
        long idx_data_dr =  FIRST_INDEX_FIRST_DATA_DR + amount_changed_elements*SD_DR;//(*(ptDRUVAd->number_items_dr))
        //Calc Time
        //time_before_start_record_dr = //uLLDrecTimeStampVal - (arrAdditionalInfoCmdPlusTimeStamp[j].ullTimeStamp);    
                    
        pU32 = (uint32_t *)&(arRawCmd[idx_100ms]);
        pCmd = (uint32_t *)&(arRawCmd[i]);
         _NUMBER_CHANGES_INTO_UNSIGNED_INT_ARRAY( (pU32), (pCmd), N_BIG, number_changes_into_current_item);
        if((number_changes_into_current_item > 0)){
            if(idx_100ms <= shIndexWRRawCmd)
                time_before_start_record_dr = idx_100ms; 
            else
                time_before_start_record_dr = idx_100ms - AMOUNT_CMD_PLUS_TIME_STAMP_RECORD;
            time_before_start_record_dr -= shIndexWRRawCmd; 
            if( amount_changed_elements > 1){
                *(ptDRUVAd->number_changes_into_dr_record) += number_changes_into_current_item;

                //if(idx_100ms <= shIndexWRRawCmd)
                //    time_before_start_record_dr = idx_100ms; 
                //else
                //    time_before_start_record_dr = idx_100ms - AMOUNT_CMD_PLUS_TIME_STAMP_RECORD;
                //time_before_start_record_dr -= shIndexWRRawCmd;

                buffer_for_save_dr_record[idx_data_dr +  0] =  time_before_start_record_dr        & 0xff;
                buffer_for_save_dr_record[idx_data_dr +  1] = (time_before_start_record_dr >> 8 ) & 0xff;
                buffer_for_save_dr_record[idx_data_dr +  2] = (time_before_start_record_dr >> 16) & 0xff;//(3-0)
                
                buffer_for_save_dr_record[idx_data_dr + (3 + NUMBER_BYTES_SAMPLE_DR + 0)] = 
                number_changes_into_current_item & 0xff;
                
                buffer_for_save_dr_record[idx_data_dr + (3 + NUMBER_BYTES_SAMPLE_DR + 1)] = 
                (number_changes_into_current_item >> 8) & 0xff;
                memcpy((void*)&buffer_for_save_dr_record[idx_data_dr +  3],(const void*)pCmd,(NUMBER_BYTES_SAMPLE_DR));
                p_last_save = pCmd;//i = idx_100ms;
                
                *(ptDRUVAd->number_items_dr) += 1;
             i = idx_100ms;
            }else{
                 asm volatile(
                "bkpt 1"
                );  
                buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR +  0] = 0x00;//time_before_start_record_dr
                buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR +  1] = 0x00;//time_before_start_record_dr
                buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR +  2] = 0x80;//time_before_start_record_dr
                buffer_for_save_dr_record[FIRST_INDEX_NUMBER_BEFORE_ITEMS_DR] = (time_before_start_record_dr );//
                buffer_for_save_dr_record[idx_data_dr + (3 + NUMBER_BYTES_SAMPLE_DR + 0)] =  0;
                buffer_for_save_dr_record[idx_data_dr + (3 + NUMBER_BYTES_SAMPLE_DR + 1)] =  0;
                memcpy((void*)&buffer_for_save_dr_record[idx_data_dr +  3],(const void*)&(arRawCmd[idx_100ms]),(NUMBER_BYTES_SAMPLE_DR));
                p_last_save = (uint32_t*)&(arRawCmd[idx_100ms]);
                 *(ptDRUVAd->number_items_dr) += 1;
                break;
            }
                amount_changed_elements--;
        }
        
        
        
        // *(ptDRUVAd->number_items_dr) -= 1;
        if( --idx_100ms < 0 )                                 //++idx_100ms >= AMOUNT_CMD_PLUS_TIME_STAMP_RECORD
            idx_100ms += AMOUNT_CMD_PLUS_TIME_STAMP_RECORD;   // idx_100ms -= AMOUNT_CMD_PLUS_TIME_STAMP_RECORD;   
    
        
        
    }while( --chAmontRecords > 0  );//*(ptDRUVAd->number_items_dr) > 0

        
    

    asm volatile(
        "bkpt 1"
        );  
    return (void*)p_last_save;
}

//=====================================================================================================
//''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
//      This func was design for fill hldDiffInfo field
//....................................................................................................
//=====================================================================================================
SRAM1 DiffInfoDsc hldDiffInfo;
// ----------------    -------------------------

void FillDiffInfo(void){
    //?struct tm tm_time_dat;
    struct tm *restrict tptr;
    time_t selected_as_base_time_dat,next_time_dat;
    DateHolder seltdAsBaseDate,nextDate;
    long i;
    if(holderCmdPlusTime.shTotalFixElem == 0)
        return;
    i = holderCmdPlusTime.shIndexWR;//most old
    if (i == 0)
        i = AMOUNT_CMD_PLUS_TIME_RECORD - 1;
    else 
        i--;
    DiffInfoDsc *pDiffInfo = &hldDiffInfo;
    short IdxArrCmdPlusTimeElem = i;//last writed
    short counter_scaned_elem = 1, SelectorCurrElemArrDatePlRefTags;
    short IdxSelDateElem = i,IdxNextDateElem = 0;
    selected_as_base_time_dat = 
    holderCmdPlusTime.arrCmdPlusTimeHolder[IdxSelDateElem].unix_time.time_dat;
    
    tptr = localtime((const time_t *)&selected_as_base_time_dat);
    seltdAsBaseDate.mday = tptr->tm_mday;
    seltdAsBaseDate.mon  = tptr-> tm_mon;  
    seltdAsBaseDate.year = tptr->tm_year;
    i = IdxArrCmdPlusTimeElem;
     if (i == 0)
         i = AMOUNT_CMD_PLUS_TIME_RECORD - 1;
     else 
         i--;
    IdxArrCmdPlusTimeElem = i;
    IdxNextDateElem = i;
    SelectorCurrElemArrDatePlRefTags = 0;
    pDiffInfo->indexBaseDatein_arrCmdPlusTimeHolder = IdxSelDateElem;   
    pDiffInfo->arrDatePlRefTags[0].amountElemForThisDate = 1;
    pDiffInfo->arrDatePlRefTags[0].numFierstElemForThisDate = IdxSelDateElem;
    pDiffInfo->arrDatePlRefTags[0].dateHolder = seltdAsBaseDate;
     pDiffInfo->amountDifferentOneDateElms = 1;
    if(holderCmdPlusTime.shTotalFixElem > 1)
    //    while(++counter_scaned_elem < holderCmdPlusTime.shTotalFixElem){
    do{
        
        
        next_time_dat = 
        holderCmdPlusTime.arrCmdPlusTimeHolder[IdxArrCmdPlusTimeElem].
        unix_time.time_dat;
        tptr = localtime((const time_t *)&next_time_dat);
        nextDate.mday = tptr->tm_mday;
        nextDate.mon  = tptr-> tm_mon;  
        nextDate.year = tptr->tm_year;
        if(
              (seltdAsBaseDate.mday == nextDate.mday)
            &&(seltdAsBaseDate.mon  == nextDate.mon )
            &&(seltdAsBaseDate.year == nextDate.year)
        )
        {
            pDiffInfo->arrDatePlRefTags[SelectorCurrElemArrDatePlRefTags].amountElemForThisDate +=1;
    
        }
        else{
             pDiffInfo->amountDifferentOneDateElms += 1;
             SelectorCurrElemArrDatePlRefTags++;
            pDiffInfo->arrDatePlRefTags[SelectorCurrElemArrDatePlRefTags].amountElemForThisDate    = 1;
            pDiffInfo->arrDatePlRefTags[SelectorCurrElemArrDatePlRefTags].numFierstElemForThisDate = IdxNextDateElem;
            pDiffInfo->arrDatePlRefTags[SelectorCurrElemArrDatePlRefTags].dateHolder = nextDate;
            seltdAsBaseDate =  nextDate;
        }   
        //i = IdxArrCmdPlusTimeElem;//
        if (IdxArrCmdPlusTimeElem == 0)
            IdxArrCmdPlusTimeElem = AMOUNT_CMD_PLUS_TIME_RECORD - 1;
        else 
            IdxArrCmdPlusTimeElem--;
        //IdxArrCmdPlusTimeElem = i;
        IdxSelDateElem = IdxNextDateElem;
        IdxNextDateElem = IdxArrCmdPlusTimeElem;

    }while(++counter_scaned_elem < holderCmdPlusTime.shTotalFixElem);



}
TwoLnCalcInfo scrTwoLnCalcInfo;
unsigned int maxAmountTwoLineSttLogElem;long ekr2LineIndexPos;
short selectorOneDateElem, fierstSelectedOneDateElem;
char fierstViewAfterEnterKeyPressed = 0;
//=====================================================================================================
//''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
//  This func was design for fill hldDiffInfo field
// void FillTwoLineElemInfoForCurrEkrIndexPos1(long ekrIndexPos) was 1-st edition
// used param TwoLnCalcInfo conv to void* = )(void*) scrTwoLnCalcInfo  
// ekrIndexPos - as usual current_ekran.index_position value inserted
// func fill on addr pv2LnCalcInfo TwoLnCalcInfo calculaned for value ekrIndexPos param
//....................................................................................................
//=====================================================================================================

// ----------------    -------------------------
void Fill2LnScrElemCalcInfo(void* pv2LnCalcInfo ,long ekrIndexPos){

    // asm volatile(
    //           "bkpt 1"
    //           );  
    long calcScrIndexPos = 0,calcMaxScrIndexPos = 0;               //
    short selectorOneDateElm = 0;            //
    short selectorCmdPlusTimeStateElem  = 0;//
    short selIdxBitCmdPlusTimeStateElem = 0;//
    
    DiffInfoDsc *pDiffInfo = &hldDiffInfo;
    //..TwoLnCalcInfo *pTwoLnCalcInfo = &scrTwoLnCalcInfo;
    short counterOneDateElms = 0;
    char confirmContinuewhile = 1;
    selectorCmdPlusTimeStateElem =
    pDiffInfo->arrDatePlRefTags[0].numFierstElemForThisDate;
    
    do{

        if(confirmContinuewhile == 1) {
            short amountCmdPlusTimeElem = pDiffInfo->arrDatePlRefTags[selectorOneDateElm].amountElemForThisDate;
            short idxCmdPlusTimeElem = pDiffInfo->arrDatePlRefTags[selectorOneDateElm].numFierstElemForThisDate;
            
            for(long ctrCmdPlusTimeElem = 0, amountChangesInCmd = 0, confirmContinueFor = 1;//differ type here not allowed
              (ctrCmdPlusTimeElem < amountCmdPlusTimeElem)&& confirmContinueFor == 1; ctrCmdPlusTimeElem++)
            {
                amountChangesInCmd = GetNumberChangingInLogElemUseLocal(idxCmdPlusTimeElem);
                if(amountChangesInCmd+calcScrIndexPos >= (ekrIndexPos+1)){
                    selectorCmdPlusTimeStateElem = idxCmdPlusTimeElem;//!@FixSelector
                    selIdxBitCmdPlusTimeStateElem = ekrIndexPos - calcScrIndexPos;
                    //pTwoLnCalcInfo->activeScr = SHOW_TIME_MS_STATE_NAME_COMAND;
                    confirmContinueFor = 0; calcMaxScrIndexPos = amountChangesInCmd+calcScrIndexPos;
                    confirmContinuewhile = 0;
                    break;
                }
                else{
                    calcScrIndexPos += amountChangesInCmd;
                    if (idxCmdPlusTimeElem == 0)
                        idxCmdPlusTimeElem = AMOUNT_CMD_PLUS_TIME_RECORD - 1;
                    else 
                        idxCmdPlusTimeElem--;
                }
            }
            if((++counterOneDateElms < pDiffInfo->amountDifferentOneDateElms)&& confirmContinuewhile != 0){
            selectorOneDateElm++;
            }
            else
                confirmContinuewhile = 0;
        }
    }while( confirmContinuewhile == 1);


    ((TwoLnCalcInfo *)pv2LnCalcInfo)->selectorOneDateElm             = selectorOneDateElm            ;
    ((TwoLnCalcInfo *)pv2LnCalcInfo)->selectorCmdPlusTimeStateElem  = selectorCmdPlusTimeStateElem ;
    ((TwoLnCalcInfo *)pv2LnCalcInfo)->selIdxBitCmdPlusTimeStateElem = selIdxBitCmdPlusTimeStateElem;
    ((TwoLnCalcInfo *)pv2LnCalcInfo)->calculatedScrIndexPos          = calcScrIndexPos;
    ((TwoLnCalcInfo *)pv2LnCalcInfo)->calculatedMaxScrIndexPos       = calcMaxScrIndexPos;
}
//,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//=====================================================================================================
//''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
//                  
//....................................................................................................
//=====================================================================================================

// ----------------    -------------------------
long Calc2lineIndexPosUsingBitCmdValuesAttachedToOneDateElem(long idxOneDateElem){
    long findedScrIndexPos = 0;
    long amountDifferentOneDateElms = hldDiffInfo.amountDifferentOneDateElms;
    TwoLnCalcInfo *pTwoLnCalcInfo = &scrTwoLnCalcInfo;
    if(idxOneDateElem > amountDifferentOneDateElms)
        return -1;
    do{
        Fill2LnScrElemCalcInfo(pTwoLnCalcInfo,findedScrIndexPos);//FillTwoLineElemInfoForCurrEkrIndexPos1(findedScrIndexPos);
        findedScrIndexPos += pTwoLnCalcInfo->calculatedMaxScrIndexPos;
    }while( ((pTwoLnCalcInfo->selectorOneDateElm) != idxOneDateElem) 
		&& ((pTwoLnCalcInfo->selectorOneDateElm) <= amountDifferentOneDateElms)
	);
    return pTwoLnCalcInfo->calculatedScrIndexPos;//?..+pTwoLnCalcInfo->selIdxBitCmdPlusTimeStateElem;
}
  

        
    
//
//--------------------------------------------------------------------------------------------------------
//````````````````````````````````````````````````````````````````````````````````````````````````````````

//=====================================================================================================
//''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
//      This func was introduce for better robust Do the same as GetNumberChangingInLogElem
//....................................................................................................
//=====================================================================================================
//Late add cjntrol whith date
// ----------------    -------------------------
long GetNumberChangingInLogElemUseLocal( long lIdx){
    register long i;        
    unsigned int array_old[N_BIG], array_new[N_BIG], array_changing[N_BIG];
struct{
        char compare_res_time_dat,compare_res_time_ms,counter_reading,compare_res;
        char data_correct; 
        time_t time_dat_tmp,copy_time_dat_tmp ; 
        int32_t time_ms_tmp,copy_time_ms_tmp ; 
    }sLV; 
    sLV.data_correct = 0;
    sLV.counter_reading = 3;sLV.data_correct = 0;

    do{
        GetMsLogElem      ((unsigned int *)&(sLV.time_ms_tmp ),lIdx);
        GetDateTimeLogElem((unsigned int *)&(sLV.time_dat_tmp),lIdx);    
            //.GetCmdPlusTimeLogElem(array_new ,lIdx);
            //long l = lIdx+1;//Next elem on menu older
            //if (l >= AMOUNT_CMD_PLUS_TIME_RECORD)
            //    l -= AMOUNT_CMD_PLUS_TIME_RECORD;
            //.GetCmdPlusTimeLogElem(array_old ,l);
            unsigned long *pU32 = &(holderCmdPlusTime.arrCmdPlusTimeHolder[lIdx].cmd.uLCmd[0]);
            memcpy((void *)array_new,(const void*)pU32,  sizeof(UNN_CmdState));
            long l = lIdx; 
            if (l == 0)
                 l = AMOUNT_CMD_PLUS_TIME_RECORD - 1;
            else 
                l--;
            pU32 = &(holderCmdPlusTime.arrCmdPlusTimeHolder[l].cmd.uLCmd[0]);
            memcpy((void *)array_old,(const void*)pU32,  sizeof(UNN_CmdState));
            
        GetDateTimeLogElem((unsigned int *)&(sLV.copy_time_dat_tmp),lIdx); 
        GetMsLogElem      ((unsigned int *)&(sLV.copy_time_ms_tmp ),lIdx);
        if(sLV.time_ms_tmp == sLV.copy_time_ms_tmp){
            
            sLV.compare_res_time_ms = 1;
        }
        else 
             sLV.compare_res_time_ms = 0;
        if(sLV.time_dat_tmp == sLV.copy_time_dat_tmp){
            
            sLV.compare_res_time_dat = 1;
        }
        else 
             sLV.compare_res_time_dat = 0;
        sLV.compare_res = sLV.compare_res_time_ms + sLV.compare_res_time_dat;
    }while( (--sLV.counter_reading > 0) && (sLV.compare_res != 2));
        
    //?.unsigned int array_old[N_BIG], array_new[N_BIG], array_changing[N_BIG];
    //?.GetCmdPlusTimeLogElemUseLocal(array_new ,lIdx);
	//?.
    //?.lIdx++;//Next elem on menu older
    //?.if (lIdx >= AMOUNT_CMD_PLUS_TIME_RECORD)
    //?.    lIdx -= AMOUNT_CMD_PLUS_TIME_RECORD;
    //?.GetCmdPlusTimeLogElemUseLocal(array_old ,lIdx);
    if (sLV.compare_res == 2){
             for (unsigned int j = 0; j < N_BIG; j++)
                 array_changing[j] = array_new[j] ^ array_old[j];
        long l32Count = 0;
            for (unsigned int j = 0; j < N_BIG; j++){    
                for(i = 0; i < 32; i++)
                    if((array_changing[j] &(1<<i)) != 0)
                        l32Count++;
                
            }
        return l32Count;
    }
    else
        return -1;
}

//,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""



/*
 __root  const short arrEkransId[] = {
     
    EKRAN_CONTROL_UP                  , 
    //.EKRAN_LIST_TYPE_IEC61850_NODES    ,
    EKRAN_LIST_INPUTS_FOR_RANGUVANNJA ,
    EKRAN_RANGUVANNJA_INPUT_1         ,
    EKRAN_LIST_OUTPUTS_FOR_RANGUVANNJA, 
    EKRAN_RANGUVANNJA_OUTPUT_1        ,
    EKRAN_RANGUVANNJA_OUTPUT_LAST     ,   
     
     
     
    EKRAN_LIST_LEDS_FOR_RANGUVANNJA,
    EKRAN_RANGUVANNJA_LED_1      ,
    
    
    EKRAN_LIST_SETTINGS_FOR_DF          ,
    EKRAN_RANGUVANNJA_DF8_BLK           ,
    
    EKRAN_LIST_DT                      ,
    EKRAN_SET_RESET_DT1                ,
    
    EKRAN_RANGUVANNJA_RESET_DT4_MINUS  ,
    
    
    EKRAN_LIST_D_AND          ,
    EKRAN_RANGUVANNJA_D_AND1  ,
    EKRAN_RANGUVANNJA_D_AND8  ,
    EKRAN_LIST_D_OR          ,
    EKRAN_RANGUVANNJA_D_OR1  ,
    EKRAN_RANGUVANNJA_D_OR8  ,
    EKRAN_LIST_D_XOR         ,
    EKRAN_RANGUVANNJA_D_XOR1 ,
    EKRAN_RANGUVANNJA_D_XOR8 ,
    EKRAN_LIST_D_NOT         ,
    EKRAN_RANGUVANNJA_D_NOT1  ,
    EKRAN_RANGUVANNJA_D_NOT16 ,
    EKRAN_LIST_TF_FOR_RANGUVANNJA,
    EKRAN_LIST_SOURCE_TF1,
    
    
    EKRAN_LIST_BUTTONS_FOR_RANGUVANNJA,
    EKRAN_RANGUVANNJA_BUTTON_1,
    EKRAN_RANGUVANNJA_BUTTON_6,
    
    EKRAN_VIDKLUCHENNJA,
    
    (NUMBER_INPUTS + NUMBER_OUTPUTS + NUMBER_LEDS + 5*NUMBER_DEFINED_FUNCTIONS + 7*NUMBER_DEFINED_TRIGGERS + NUMBER_DEFINED_AND + NUMBER_DEFINED_OR + NUMBER_DEFINED_XOR + NUMBER_DEFINED_NOT + NUMBER_TRANSFER_FUNCTIONS + NUMBER_DEFINED_BUTTONS),
    (N_IN_GOOSE + N_IN_MMS + N_OUT_LAN + NUMBER_INPUTS +
    NUMBER_OUTPUTS + NUMBER_LEDS + 5*NUMBER_DEFINED_FUNCTIONS + 
    7*NUMBER_DEFINED_TRIGGERS + NUMBER_DEFINED_AND + NUMBER_DEFINED_OR + NUMBER_DEFINED_XOR + 
    NUMBER_DEFINED_NOT + NUMBER_TRANSFER_FUNCTIONS + NUMBER_DEFINED_BUTTONS)
    
 };
*/







