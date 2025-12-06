//inline void main_protection(void)


typedef union net_in_cmd_area_Unn{ 
   
   struct {
      unsigned int out1 : 1; //0
      unsigned int out2 : 1; //1
      unsigned int out3 : 1; //2
      unsigned int out4 : 1; //3
      unsigned int out5 : 1; //4 
      unsigned int out6 : 1; //5 
      unsigned int out7 : 1; //6 
      unsigned int out8 : 1; //7    
    } bool_val;
    unsigned char U8_Vl;//long lVl;
} NetIn_stt; //
typedef union goose_block_Unn{ 
   unsigned char arCh[4];
   unsigned short arSh[2];
   unsigned long GS_Vl;
   struct{
        struct {
            unsigned int in1 : 1; //0
            unsigned int in2 : 1; //1
            unsigned int in3 : 1; //2
            unsigned int in4 : 1; //3
            unsigned int in5 : 1; //4 
            unsigned int in6 : 1; //5 
            unsigned int in7 : 1; //6 
            unsigned int in8 : 1; //7    
        } in_bool; //
    unsigned char Blk;
    struct {
        unsigned int out1 : 1; //0
        unsigned int out2 : 1; //1
        unsigned int out3 : 1; //2
        unsigned int out4 : 1; //3
        unsigned int out5 : 1; //4 
        unsigned int out6 : 1; //5 
        unsigned int out7 : 1; //6 
        unsigned int out8 : 1; //7    
    } out_bool; //unsigned char Out;
    unsigned char Res;
   }GS_Hld;
    
} GsBlock_stt; //
/*

typedef union mms_block_Unn{ 
   struct {
      unsigned int out1 : 1; //0
      unsigned int out2 : 1; //1
      unsigned int out3 : 1; //2
      unsigned int out4 : 1; //3
      unsigned int out5 : 1; //4 
      unsigned int out6 : 1; //5 
      unsigned int out7 : 1; //6 
      unsigned int out8 : 1; //7    
    } bool_val;
    unsigned char U8_Vl;//long lVl;
} MmsBlock_stt; //
*/
  typedef union mms_block_Unn{ 
    unsigned char arCh[4];
    unsigned short arSh[2];
    unsigned long Mms_Vl;
    struct{
        struct {
            unsigned int in1 : 1; //0
            unsigned int in2 : 1; //1
            unsigned int in3 : 1; //2
            unsigned int in4 : 1; //3
            unsigned int in5 : 1; //4 
            unsigned int in6 : 1; //5 
            unsigned int in7 : 1; //6 
            unsigned int in8 : 1; //7    
        } in_bool; //
    unsigned char Blk;
    struct {
        unsigned int out1 : 1; //0
        unsigned int out2 : 1; //1
        unsigned int out3 : 1; //2
        unsigned int out4 : 1; //3
        unsigned int out5 : 1; //4 
        unsigned int out6 : 1; //5 
        unsigned int out7 : 1; //6 
        unsigned int out8 : 1; //7    
    } out_bool; //unsigned char Out;
    unsigned char Res;
   }Mms_Hld;
    
} MmBlock_stt;
register unsigned long rU;
struct{
    char  ch_while_breaker;
    __SETTINGS *p_current_settings_prt;

    
}sLV;

        do{
            ((MmsBlkParamDsc*)pvl)->sh_amount_mms_active_src = lIdxBlk = 0;
			//--;
            //--;
            //--;
            i = ((MmsBlkParamDsc*)pvl)-> unnV1[lIdxBlk].MmsBlkHdr.OrdNumMmsBlk;
            //Check State Inputs
            lV = arrMmsBlk[i].arCh[2];//Outs
            lV &= ~((unsigned long)(Input_ctrl_In_MMS_block[i]));//Clear all changeble bits
            lV |= Input_In_MMS_block[i]&Input_ctrl_In_MMS_block[i];//Set ones only
            //if(noerror)
                arrMmsBlk[i].arCh[2] = lV;//
                register unsigned long wrpLV;
    
            wrpLV = arrMmsBlk[i].arCh[2];
            j = 0;lV = ((MmsBlkParamDsc*)pvl)->sh_amount_mms_active_src;
            while( (wrpLV != 0) && j < 8){
                if((wrpLV & (1<<j)) != 0 ){
                    arrOrdNumsMmsSignal[lV] = lIdxBlk+j;lV++;
                }
                j++;
                
            }
            ((MmsBlkParamDsc*)pvl)->sh_amount_mms_active_src = lV;

        }while((--sLV.ch_amount_active_src) != 0);


uint32_t ranguvannja_Out_LAN [N_OUT_LAN][ N_OUT_LAN_IN][ MAX_FUNCTIONS_IN_OUT_LAN];
     
enum __rang_output_led_df_reg {
#if (MODYFIKACIA_VERSII_PZ >= 10)
//=====================================================================================================
//''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
//                  
//....................................................................................................
//=====================================================================================================
do{
    sLV.ch_while_breaker = 0;//! optimize then
    //register long lV;
	
	register unsigned long rU_V,rU_bit,rU_out, rU_Idx;
    //register void  *pvl;
    
     register unsigned int *r_p_active_functions, *prU;
// ----------------    -------------------------
    r_p_active_functions = sLV.p_active_functions;
    pvl = (void*)&hldLanBlkParam;
	//pv  = (void*)&ranguvannja_Out_LAN;
	prU  = (unsigned int*)&ranguvannja_Out_LAN;
// ----------------    -------------------------
	sLV.IdxBlk = LAN_BLOCK_ORD_NUM_00;
	
	for(rU_out = 0; rU_out < 8; rU_out++){
		rU_bit = rU_V = 0;
		while( rU_bit < MAX_FUNCTIONS_IN_OUT_LAN ){
			rU_Idx = prU[sLV.IdxBlk + rU_out + rU_bit];//find index
			if(rU_Idx>0){
				rU_Idx--;
				//Create 32 bit mask
				rU_V = r_p_active_functions[rU_Idx >> 5] & ( (unsigned int)( 1 << (rU_Idx & 0x1f)));
				if(rU_V != 0){
					;//Set Val
					rU_bit = MAX_FUNCTIONS_IN_OUT_LAN+2;
				}
			}
			rU_bit++;
		}
		rU_Idx = sLV.IdxBlk;
		if(rU_bit == (MAX_FUNCTIONS_IN_OUT_LAN+2) ){ //Activate Out
			Output_Out_LAN_block[rU_Idx] |= 1<< rU_out
		}else{
			Output_Out_LAN_block[rU_Idx] &= ~(1<< rU_out);	//Clr Out
		}
		
	
	}
	
ERROR_CPU_RECEIVING_CANAL_1
ERROR_CPU_RECEIVED_PACKET_CANAL_1
ERROR_CPU_ANSWER_CANAL_1	
	
	
   
}while(sLV.ch_while_breaker);
//
//--------------------------------------------------------------------------------------------------------
//````````````````````````````````````````````````````````````````````````````````````````````````````````

#define _CHECK_SET_BIT(_array, _number_bit)                                     \
    (_array[_number_bit >> 5] & ( (unsigned int)( 1 << (_number_bit & 0x1f)) ) ) 

//=====================================================================================================
//''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
//                  
//....................................................................................................
//=====================================================================================================
do{
    sLV.ch_while_breaker = 0;
// ----------------    -------------------------
//   if(_CHECK_SET_BIT(p_active_functions, RANG_OP_PR_DZ2) != 0)
//                p1.bool_vars.or2__i_1 = 1;
//active_functions 
//register unsigned int *p_af = sLV.p_active_functions;
((GsBlkParamDsc*)pvl)-> unnV1[lIdxBlk].GsBlkHdr.OrdNumGsBlk;
   
}while(sLV.ch_while_breaker);
//
//--------------------------------------------------------------------------------------------------------
//````````````````````````````````````````````````````````````````````````````````````````````````````````



//=====================================================================================================
//''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
//                  
//....................................................................................................
//=====================================================================================================
do{
// ----------------    -------------------------
  
    
}while();
//
//--------------------------------------------------------------------------------------------------------
//````````````````````````````````````````````````````````````````````````````````````````````````````````




            
//=====================================================================================================
//''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
//                  
//....................................................................................................
//=====================================================================================================
void iec_handler(unsigned int *p_active_functions, unsigned int number_group_stp){
// ----------------    -------------------------
  UNUSED(p_active_functions);
  UNUSED(number_group_stp);
    
}
//
//--------------------------------------------------------------------------------------------------------
//````````````````````````````````````````````````````````````````````````````````````````````````````````
Mecroses for Set bit in Reg 
#define _SET_BIT_IN_REG(_reg, _number_bit)                                           \
     _asm volatile("orr %0,%0,#_number_bit" :"=r"(_reg) :"+r"(_reg): "cc");

#define _CLEAR_BIT_IN_REG(_reg, _number_bit)                                         \
     _asm volatile("bic %0,%0,#_number_bit" :"=r"(_reg) :"+r"(_reg): "cc");
#define _SET_BIT_IN_REG(_reg, _number_bit)                                           \
  asm ("orrs r0,r0,r1" :"+r"(_reg) :"r"(_reg): "cc");	

  asm ("orrs %0,%0,=3" :"+r"(_reg) :"r"(_reg),"r"(_number_bit): "cc");  
"+r"(_reg)
//=====================================================================================================
    //''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
    //    state_outs_copy = state_outs;
    //    state_outs_copy &= Input_ctrl_In_GOOSE_block;//First clear changed bits
    //    j = state_outs^Input_In_GOOSE_block;//Select changed position
    //    l = j"Select changed position" & Input_In_GOOSE_block;//Select Ones in bits
    //    state_outs_copy |= l&Input_In_GOOSE_block;//Move changed ones; <-have Error
    //....................................................................................................
    //=====================================================================================================
    do{
    // ----------------    -------------------------
     
     register long i,lV,j;
     register long lIdxBlk;
     
     register void *pvl;
     
        pvl = (void*)&hldGsBlkParam;
        sLV.ch_amount_active_src = i = hldGsBlkParam.ch_amount_active_src;
        sLV.ch_amount_GsSignal = 0;
        if(i > 0){
            ((GsBlkParamDsc*)pvl)->sh_amount_gs_active_src = lV = lIdxBlk = 0;asm ("nop" ::"r"(lIdxBlk),"r"(lV));
            while(lIdxBlk < N_IN_GOOSE){ //do{
                //--;
                //--;
                //--;
                i = ((GsBlkParamDsc*)pvl)-> unnV1[lIdxBlk].GsBlkHdr.OrdNumGsBlk;
                //Check State Inputs
                lV = arrGsBlk[i].arCh[2];//Outs
                lV &= ~((unsigned long)(Input_ctrl_In_GOOSE_block[i]));//Clear all changeble bits
                lV |= Input_In_GOOSE_block[i]&Input_ctrl_In_GOOSE_block[i];//Set ones only
                //if(noerror)
                    arrGsBlk[i].arCh[2] = lV;//
                    register unsigned long wrpLV;
        
                wrpLV = arrGsBlk[i].arCh[2];
                j = 0;lV = ((GsBlkParamDsc*)pvl)->sh_amount_gs_active_src;
                while( (wrpLV != 0) ){//&& j < 8
                    if((wrpLV & (1<<j)) != 0 ){
                        arrOrdNumsGsSignal[lV] = (lIdxBlk<<3)+j;lV++;wrpLV &= ~(1<<j);
                    }
                    j++;
                    
                }
                ((GsBlkParamDsc*)pvl)->sh_amount_gs_active_src = lV;
                lIdxBlk++;//  += lV;//! ????
            }//while(lIdxBlk);//(--sLV.ch_amount_active_src) != 0
            sLV.ch_amount_GsSignal = ((GsBlkParamDsc*)pvl)->sh_amount_gs_active_src;
        }
    //Try bield list of outs which sets in ones.
    
    }while(sLV.ch_while_breaker);
    //
    //--------------------------------------------------------------------------------------------------------
    //````````````````````````````````````````````````````````````````````````````````````````````````````````

void proc_Gs_blk_out(void* pv,unsigned long lCtrGsSrc,short* p_arrOrdNumsGsSignal ){

    // ----------------    -------------------------       
        register unsigned long i,lV,j;
         
         register void *pvll;
        //pvlc = (void*)temp_value_for_activated_function;//ui32
        pvll = (void*)&current_settings_prt.ranguvannja_In_GOOSE;//uint32_t
        i = j = lV = 0;
        //sLV.ch_amount_active_src = hldGsBlkParam.ch_amount_active_src;
        //pvl = (void*)&hldGsBlkParam;((GsBlkParamDsc*)pvl)->
        //lCtrGsSrc = hldGsBlkParam.sh_amount_gs_active_src;   
        asm ("nop" ::"r"(i),"r"(j),"r"(lV));
        while(lCtrGsSrc){
            lCtrGsSrc--;
            lV = p_arrOrdNumsGsSignal[lCtrGsSrc];
            i = lV>>3;j = lV - (i<<3);
            
            
            //temp_value_for_activated_function[N_SMALL];//;ranguvannja_In_GOOSE[i][j][0]
            //lV = i*N_IN_GOOSE_MMS_OUT*N_SMALL*sizeof(long) +j*N_SMALL*sizeof(long);
            lV = i*N_IN_GOOSE_MMS_OUT*N_SMALL +j*N_SMALL;
            for(register unsigned long k = 0; k < N_SMALL; k++){
                ((unsigned long*)pv)[k] |= ((unsigned long*)pvll+lV) [k];
            }
            
        }

}
void proc_Lan_blk_out(unsigned short *p_rang_Out_LAN,unsigned int *p_active_functions, void *pLanDsc){
    register unsigned long rU_V,rU_bit,rU_out, rU_Idx;
//    union {
//        unsigned char   u8Ar [4];
//        unsigned short  u16Ar [2];
//        unsigned long   ulV;
//  }Unn;   
    unsigned long IdxBlk,l_O;
// ----------------    -------------------------
    IdxBlk = *((unsigned char*)pLanDsc);l_O = 0;//sLV.IdxBlk = LAN_BLOCK_ORD_NUM_00;    
    for(rU_out = 0; rU_out < 8; rU_out++){
        rU_bit = rU_V = 0;
        while( rU_bit < MAX_FUNCTIONS_IN_OUT_LAN ){
            //rU_Idx = prU[sLV.IdxBlk + rU_out + rU_bit];//find index
            rU_V = (IdxBlk*N_OUT_LAN_IN*MAX_FUNCTIONS_IN_OUT_LAN) + (rU_out*MAX_FUNCTIONS_IN_OUT_LAN) + rU_bit;//find index
            rU_Idx = p_rang_Out_LAN[rU_V];//find index
            if(rU_Idx>0){
                rU_Idx--;
                //Create 32 bit mask
                rU_V = p_active_functions[rU_Idx >> 5] & ( (unsigned int)( 1 << (rU_Idx & 0x1f)));
                if(rU_V != 0){
                    ;//Set Val
                    rU_bit = MAX_FUNCTIONS_IN_OUT_LAN+1;
                }
            }
            rU_bit++;
        }
        rU_Idx = IdxBlk;//sLV.IdxBlk;
        if(rU_bit == (MAX_FUNCTIONS_IN_OUT_LAN+2) ){ //Activate Out
            //((unsigned char*)pLanDsc+1)[rU_Idx] |= 1<< rU_out;
            l_O  |= 1<< rU_out;
        }//else{
            //((unsigned char*)pLanDsc+1)[rU_Idx] &= ~(1<< rU_out); //Clr Out
            //Output_Out_LAN_block[rU_Idx] &= ~(1<< rU_out);  //Clr Out
        //}
        
    
    }   
    Output_Out_LAN_block[IdxBlk] = l_O;
}


          active_functions[RANG_DF1_IN >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_DF1_IN) != 0) << (RANG_DF1_IN & 0x1f);
          active_functions[RANG_DF2_IN >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_DF2_IN) != 0) << (RANG_DF2_IN & 0x1f);
          active_functions[RANG_DF3_IN >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_DF3_IN) != 0) << (RANG_DF3_IN & 0x1f);
          active_functions[RANG_DF4_IN >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_DF4_IN) != 0) << (RANG_DF4_IN & 0x1f);
          active_functions[RANG_DF5_IN >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_DF5_IN) != 0) << (RANG_DF5_IN & 0x1f);
          active_functions[RANG_DF6_IN >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_DF6_IN) != 0) << (RANG_DF6_IN & 0x1f);
          active_functions[RANG_DF7_IN >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_DF7_IN) != 0) << (RANG_DF7_IN & 0x1f);
          active_functions[RANG_DF8_IN >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_DF8_IN) != 0) << (RANG_DF8_IN & 0x1f);


/*

Тут буде одна «незручність».
Якщо ми будемо реалізовувати Вх.GOOSE як «віртуальні входи», то, я так розумію. 
треба буде і реалізувати існуючий функціонал, що команди «Ввімк.ВВ» і «Вимк.ВВ» 

можуть блокуватися від входів (поки тільки від дискретних) у місцевому режимі. 
а можуть не блокуватися.
Якщо цей функціонал перенести на Вх.GOOSE, то треба цей алгоритм реалізувати як 
для дискретних входів, а саме через активацію змінних локальних vvimk_VV_vid_DV 

і vymk_VV_vid_DV
GOOSE i MMS
Taras, 12:40
enum __rang_small

У мене діапазон в 16 команд. В загальному випадку він може розміщатись 
0-к-8біт-м.Тобто може бути 2-3 байти блокування і це треба врахувати.
Може бути к або 0 біт,адреса і 8 біт і адреса і м або 0 біт.
Тоб-то спочатку вираховую адресу початкового біту
Припустимо номер біту RANG_BLOCK_IN_GOOSE1.
 Індекс байтової адреси і = RANG_BLOCK_IN_GOOSE1>>3. 
Зміщення біту -- о = RANG_BLOCK_IN_GOOSE1 - 8*і;
Маска -- 

Вихід - Знайти індекс початку і_с = RANG_BLOCK_IN_GOOSE1.Індекс кінця
 і_е = (RANG_BLOCK_IN_GOOSE1+N_IN_GOOSE)>>3.
Скільки треба зчитати -- і_е - і_с;
Зміщення
#define  RANG_BLOCK_IN_GOOSE1_U8_MSK
            (1<<( (  RANG_BLOCK_IN_GOOSE1) - (8*( RANG_BLOCK_IN_GOOSE1 >>3)) )  )

Це початкове зміщення. Відповідно слово потрібно зсунути "вліво" на 
RANG_BLOCK_IN_GOOSE1_U8_MSK
І виділити 16 біт. Тобто потім старші 16 біт можна відкинути.
Якщо знайти 32 зміщення кінця, то відповідно можна вирахувати величину зсуву 
вправо.
#define  RESERV00_DI_EPRF_U32_MSK
             (1<<( ( RANG_BLOCK_IN_GOOSE1+N_IN_GOOSE )
             - (32*(  RANG_BLOCK_IN_GOOSE1+N_IN_GOOSE      >>5)) )  )

Зчитую по байтам в 32 слово. 

Що треба, щоб виділити 16 біт.  
далі копіюю 16 біт в 32 слово по байтам.

register union { 
   struct {
      unsigned int b00:1;//00.
        unsigned int b01:1;//01.
        unsigned int b02:1;//02.
        unsigned int b03:1;//03.
        unsigned int b04:1;//04.
        unsigned int b05:1;//05.
        unsigned int b06:1;//06.
        unsigned int b07:1;//07.
        unsigned int b08:1;//08.
        unsigned int b09:1;//09.
        unsigned int b10:1;//10.
        unsigned int b11:1;//11.
        unsigned int b12:1;//12.
        unsigned int b13:1;//13.
        unsigned int b14:1;//14.
        unsigned int b15:1;//15.
        unsigned int b16:1;//16.
        unsigned int b17:1;//17.
        unsigned int b18:1;//18.
        unsigned int b19:1;//19.
        unsigned int b20:1;//20.
        unsigned int b21:1;//21.
        unsigned int b22:1;//22.
        unsigned int b23:1;//23.
        unsigned int b24:1;//24.
        unsigned int b25:1;//25.
        unsigned int b26:1;//26.
        unsigned int b27:1;//27.
        unsigned int b28:1;//28.
        unsigned int b29:1;//29.
        unsigned int b30:1;//30.
        unsigned int b31:1;//31.

   } bool_vars;
  long lVl;
  unsigned char arCh[4];
  unsigned short arSh[2];
}wrp
*/
	//pv = (void*) sLV.p_active_functions;
	//--r_p_active_functions = sLV.p_active_functions;
	//--if(_CHECK_SET_BIT((unsigned int*)r_p_active_functions, RANG_BLOCK_IN_GOOSE1) != 0){
	//--//clr block
	//--}else{
	//--
	//--}
	//--if(_CHECK_SET_BIT((unsigned int*)r_p_active_functions, (RANG_BLOCK_IN_GOOSE1 + 1)) != 0){
	//--//clr block
	//--}else{
	//--
	//--}
	//--if(_CHECK_SET_BIT((unsigned int*)r_p_active_functions, (RANG_BLOCK_IN_GOOSE1 + 2)) != 0){
	//--//clr block
	//--}else{
	//--
	//--}
	//--if(_CHECK_SET_BIT((unsigned int*)r_p_active_functions, (RANG_BLOCK_IN_GOOSE1 + 3)) != 0){
	//--//clr block
	//--}else{
	//--
	//--}
	//--if(_CHECK_SET_BIT((unsigned int*)r_p_active_functions, (RANG_BLOCK_IN_GOOSE1 + 15)) != 0){
	//--//clr block
	//--}else{
	//--
	//--}
