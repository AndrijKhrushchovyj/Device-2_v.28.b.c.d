#ifndef __EXTERNAL_FUNCTIONS_H
#define __EXTERNAL_FUNCTIONS_H

extern USB_OTG_CORE_HANDLE USB_OTG_dev;
extern uint32_t USBD_OTG_ISR_Handler(USB_OTG_CORE_HANDLE *pdev);

#ifdef USB_OTG_HS_DEDICATED_EP1_ENABLED
extern uint32_t USBD_OTG_EP1IN_ISR_Handler(USB_OTG_CORE_HANDLE *pdev);
extern uint32_t USBD_OTG_EP1OUT_ISR_Handler(USB_OTG_CORE_HANDLE *pdev);
#endif

extern CDC_IF_Prop_TypeDef APP_FOPS;

extern void MX_FATFS_Init(void);

extern void Configure_I2C(I2C_TypeDef *);
extern void FSMC_SRAM_Init(void);

extern int main(void);
extern void ar_routine_with_fatfs(unsigned int);
extern unsigned int ar_free_space(int *, int *);
extern void periodical_operations(unsigned int);
extern void periodical_operations_communication(unsigned int);
extern void global_vareiables_installation(void);
extern void start_settings_peripherals(void);
extern void start_tim4_canal2_for_interrupt_1mc(void);
extern void start_tim4_canal3_for_interrupt_10mkc(void);
extern void min_settings(__SETTINGS *);
extern void error_reading_with_eeprom(void);

extern void Interrupts_Config(void);

extern void lcd_init(void);
extern unsigned int wait_lcd_ready(void);
extern unsigned int clear_lcd(void);
extern unsigned int mode_viewing(unsigned int, unsigned int, unsigned int);
extern unsigned int hd44780_puts(unsigned char *, unsigned int);
extern unsigned int hd44780_test(unsigned char *, unsigned int);
extern unsigned int Win1251toHd44780(unsigned int);
extern unsigned int read_data_from_lcd(unsigned char *);
extern unsigned int write_data_to_lcd(unsigned char);
extern unsigned int write_command_to_lcd(unsigned char);
extern unsigned int hd44780_gotoxy(unsigned char, unsigned char);
extern unsigned int hd44780_puts_xy(unsigned char, unsigned char, unsigned char *);
extern void view_whole_ekran(void);
extern void check_state_key(GPIO_TypeDef *, uint16_t, unsigned int);
extern int index_language_in_array(int);

extern void inc_or_dec_value(unsigned int *, unsigned int);
extern unsigned int bcd_to_int(unsigned char);

void make_ekran_level_password(unsigned int password, unsigned int view);
extern void main_manu_function(void);
extern void make_ekran_main(void);
extern void make_ekran_time(void);
extern void edit_time(unsigned int);
extern void make_ekran_ask_rewrite(void);
extern unsigned int check_data_for_data_time_menu(void);
extern void make_ekran_about_error(const unsigned char[][MAX_COL_LCD]);
extern void make_ekran_about_activation_command(unsigned int, unsigned char[][MAX_NAMBER_LANGUAGE][MAX_COL_LCD]);
extern void make_ekran_configuration(unsigned int);
extern void make_ekran_measurement(void);
extern void make_ekran_measuremet_for_selsyn(void);
extern void make_ekran_current_voltage(unsigned int, unsigned int);
extern void make_ekran_voltage_for_selsyn(void);
extern void make_ekran_angle_for_selsyn(void);
extern void make_ekran_logomentr_voltage(void);
extern void make_ekran_frequency(void);
extern void convert_and_insert_char_for_measurement(unsigned int, unsigned int, unsigned int, unsigned int, unsigned char *, unsigned int);
extern void convert_and_insert_char_for_frequency(int, unsigned char *);
extern void make_ekran_chose_settings_any_protection(void);
extern void make_ekran_chose_settings_protection_with_groups(void);
extern void make_ekran_chose_setpoint_and_timeout(void);
extern void make_ekran_chose_timeout_control(void);
extern void make_ekran_chose_setpoint(void);
extern void make_ekran_chose_timeout(void);
extern void make_ekran_chose_control(void);
extern void make_ekran_chose_settings_switcher(void);
extern void make_ekran_chose_settings(void);
extern void make_ekran_chose_passwords(void);
extern void make_ekran_chose_extra_settings(void);
extern void make_ekran_setpoint_rpn(unsigned int);
extern void make_ekran_timeout_rpn(unsigned int);
extern void make_ekran_control_rpn(void);
extern void make_ekran_setpoint_mtz04(unsigned int);
extern void make_ekran_timeout_mtz04(unsigned int);
extern void make_ekran_control_zskh(void);
extern void make_ekran_setpoint_brp(unsigned int);
extern void make_ekran_control_brp(void);
extern void make_ekran_timeout_znkh(unsigned int);
extern void make_ekran_control_znkh(void);
extern void calc_symbol_and_put_into_working_ekran(unsigned char *, void *, void *, unsigned int *, unsigned int, unsigned int, unsigned int, unsigned int);
extern void calc_int_symbol_and_put_into_working_ekran(unsigned char *, unsigned int *, unsigned int *, unsigned int *, unsigned int);
extern unsigned int edit_setpoint(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
extern unsigned int check_data_setpoint(unsigned int, unsigned int, unsigned int);
extern void make_ekran_setpoint_Umin(unsigned int);
extern void make_ekran_timeout_Umin(unsigned int);
extern void make_ekran_control_Umin(void);
extern void make_ekran_setpoint_Umax(unsigned int);
extern void make_ekran_timeout_Umax(unsigned int);
extern void make_ekran_control_Umax(void);
extern void make_ekran_setpoint_UP(unsigned int);
extern void make_ekran_timeout_UP(unsigned int);
extern void make_ekran_control_UP(void);
extern void make_ekran_chose_of_list_for_ranguvannja(__id_input_output);
extern void make_ekran_transformator(void);
extern void make_ekran_transformator_control(void);
extern void make_ekran_set_function_in_bi(unsigned int, unsigned int, unsigned int *
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                                          ,
                                          int, int
#endif
);
extern void make_ekran_set_function_in_output_led_df_dt_reg(unsigned int, unsigned int, unsigned int *);
extern void check_current_index_is_presented_in_configuration(unsigned int *, int *, /*EL_FILTER_STRUCT[],*/ int, int, int, int, int, int, int, int, int, int);
extern void make_ekran_choose_CBOn_CBOff(void);
extern void make_ekran_setpoint_switch(void);
extern void make_ekran_timeout_switch(void);
extern void make_ekran_control_switch(void);
extern void make_ekran_chose_settings_uvv(void);
extern void make_ekran_dopusk_dv(void);
extern void make_ekran_type_input_uvv(unsigned int);
extern void make_ekran_type_output_uvv(void);
extern void make_ekran_type_led_uvv(void);
extern void make_ekran_type_button_uvv(void);
extern void correct_dopusk_dv_when_type_of_input_signal_is_changed(void);
extern void make_ekran_chose_communication_parameters(void);
extern void make_ekran_name_of_cell(void);
extern void make_ekran_address(void);
extern void make_ekran_chose_setting_rs485(void);
extern void make_ekran_chose_setting_Ethernet(void);
extern void make_ekran_phy_layer_rs485(void);
extern void make_ekran_protocols_rs485(void);
extern void make_ekran_speed_interface(void);
extern void make_ekran_pare_interface(void);
extern void make_ekran_stopbits_interface(void);
extern void make_ekran_timeout_interface(void);

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
extern void make_ekran_settings_network_layer_Ethernet(void);
#endif

extern void make_ekran_chose_data_time_settings(void);
extern void make_ekran_timezone_dst(void);
extern void make_ekran_dst_rule(uint32_t);
extern void make_ekran_chose_registrators(void);
extern void make_ekran_settings_analog_registrators(void);
extern void make_ekran_timeout_analog_registrator(void);
extern void make_ekran_control_ar(void);
extern void make_ekran_extended_logic(void);
extern void make_ekran_general_pickups_el(void);
extern void make_ekran_chose_settings_df(void);
extern void make_ekran_type_df(void);
extern void make_ekran_timeout_df(unsigned int);
extern void make_ekran_list_type_source_df(void);
extern void make_ekran_set_reset_trigger(void);
extern void make_ekran_list_type_source_dt(void);
extern void make_ekran_list_source_tf(void);
extern void make_ekran_info(void);
extern void make_ekran_date_time_pz(void);
extern void make_ekran_chose_item_of_point_time_settings(void);
extern void make_ekran_time_settings(unsigned int);
extern void make_ekran_diagnostyka(unsigned int volatile *);
extern void make_ekran_list_inputs_outputs(void);
extern void make_ekran_state_inputs_or_outputs(unsigned int);
extern void make_ekran_list_registrators(void);
extern void make_ekran_list_records_registrator(unsigned int);
extern void make_ekran_list_records_registrator_ar(void);
void make_ekran_data_and_time_elem_d_p_a_registrator(unsigned int type_of_registrator);
extern void make_ekran_list_titles_for_record_of_digital_registrator(void);
extern void make_ekran_data_and_time_of_records_registrator(unsigned int);
extern void make_ekran_title_analog_value_records_digital_registrator(void);
extern void make_ekran_analog_value_records_digital_registrator(void);
extern void make_ekran_changing_signals_digital_registrator(void);
extern void make_ekran_changing_diagnostics_pr_err_registrator(void);
extern void make_ekran_list_titles_for_record_of_state_cmd_registrator(void);
extern void make_ekran_changing_signals_statistica_registrator(void);
extern void make_ekran_setting_language(void);
extern void make_ekran_regulation(void);
extern void make_ekran_resurs(void);
extern void make_ekran_grupa_ustavok(void);
extern void make_ekran_vidkluchenja(void);
extern void make_ekran_data_and_time_elem_stt_registrator(unsigned int type_of_registrator);
extern void make_ekran_timeout_digital_registrator(void);
extern void make_ekran_settings_digital_registrators(void);

extern unsigned int action_after_changing_of_configuration(unsigned int, __SETTINGS *);
extern void action_after_changing_resurs_pickup(__SETTINGS *);
extern void action_during_changing_button_mode(__SETTINGS *, __SETTINGS *);
extern void def_pickup_timeout_UP(__SETTINGS *, uint32_t, uint32_t);
extern void action_after_changing_input_UP(__SETTINGS *, uint32_t, uint32_t);
extern void action_after_changing_ctrl_UP(__SETTINGS *, uint32_t);
extern void change_setpoint_to_ctrl_UP(__SETTINGS *const, uint32_t const);
//extern void action_after_changing_number_el(__SETTINGS *, unsigned int);
extern void calculate_namber_bit_waiting_for_rs_485(void);
extern void fix_change_settings(unsigned int, unsigned int);
extern unsigned int set_new_settings_from_interface(unsigned int);
extern void restore_trigger_functions(unsigned int *);
extern void changing_diagnostyka_state(void);
extern unsigned int count_number_set_bit(unsigned int *, unsigned int);

extern void set_bit(unsigned int *, unsigned int);
extern void clear_bit(unsigned int *, unsigned int);
extern long GetNumberChangingInLogElem(long lIdx);

extern void CmdPlusTimeLogHundler(unsigned int *p_active_functions);
extern void GetCmdPlusTimeLogElem(unsigned int *p_elem, long lIdx);
extern void GetDateTimeLogElem(unsigned int *p_elem, long lIdx);
extern void GetMsLogElem(unsigned int *p_elem, long lIdx);
extern long GetNumberChangingInLogElemUseLocal(long lIdx);
extern long GetNumberChangingInLogElem(long lIdx);
extern void CmdPlusTimeStampLogHundler(unsigned int *p_active_functions);

extern long GetMsLogElemPlWnum(unsigned int *p_elem, long lIdx, unsigned long ulWorkNumber);
extern long GetDateTimeLogElemPlWnum(unsigned int *p_elem, long lIdx, unsigned long ulWorkNumber);
extern long GetCmdPlusTimeLogElemPlWnum(unsigned int *p_elem, long lIdx, unsigned long ulWorkNumber);
extern void CleanCmdPlusTimeLog(void);
extern void PuCmdinRawBuf(unsigned int *p_active_functions);
extern void make_ekran_signals_stable_one_in_data_elem_stt_reg(void);
extern void make_ekran_time_ms_c_val_c_name_sr_format_changing_signals(void);
extern void make_ekran_data_elem_stt_registrator(void);

extern void Usb_routines_irq(void);
extern void Usb_routines(void);
extern void USART_RS485_Configure(void);
extern void restart_monitoring_RS485(void);
extern void start_transmint_data_via_RS_485(unsigned int);
extern unsigned short int AddCRC(unsigned char, unsigned short int);

extern unsigned int start_write_buffer_via_I2C(uint32_t, uint32_t, uint8_t *, uint32_t);
extern unsigned int start_read_buffer_via_I2C(uint32_t, uint32_t, uint8_t *, uint32_t);
extern void main_routines_for_i2c(void);
extern void error_start_i2c(void);

extern void find_new_ADC_canal_to_read(unsigned int, unsigned int *);
extern void control_reading_ADCs(void);
extern void operate_integral_values_ADCs(void);
extern void Fourier(void);
extern void fapch(void);
extern void calc_measurement(unsigned int);
extern void angle_determination(void);
extern unsigned int sqrt_64(unsigned long long);

extern void main_routines_for_spi1(void);
extern void main_function_for_dataflash_resp(int);
extern void main_function_for_dataflash_req(int);
extern void start_checking_dataflash(void);
extern void start_exchange_via_spi(int, unsigned int);
extern void dataflash_status_read(int);
extern void dataflash_set_pagesize_256(int);
extern void dataflash_erase(int);
extern void dataflash_mamory_page_program_through_buffer(int);
extern void dataflash_mamory_read(int);
extern void dataflash_mamory_page_into_buffer(int);
extern void dataflash_mamory_write_buffer(int);
extern void dataflash_mamory_buffer_into_memory(int);

extern void control_settings(void);
extern void control_ustuvannja(void);
extern void control_trg_func(void);
extern unsigned int control_info_rejestrator(__INFO_REJESTRATOR *, unsigned char);
extern unsigned int control_info_ar_rejestrator(__INFO_AR_REJESTRATOR *, unsigned char);
extern void control_angle(void);
extern void control_resurs(void);

extern void watchdog_routine(unsigned int, unsigned int const);
extern void total_error_sw_fixed(void);

extern void NMI_Handler(void);
extern void HardFault_Handler(void);
extern void MemManage_Handler(void);
extern void BusFault_Handler(void);
extern void UsageFault_Handler(void);
extern void SVC_Handler(void);
extern void DebugMon_Handler(void);
extern void PendSV_Handler(void);
extern void SysTick_Handler(void);
extern void USB_LP_CAN1_RX0_IRQHandler(void);
extern void TIM2_IRQHandler(void);
extern void TIM4_IRQHandler(void);
extern void TIM5_IRQHandler(void);
extern void I2C_EV_IRQHandler(void);
extern void I2C_ER_IRQHandler(void);
extern void DMA_StreamI2C_Tx_IRQHandler(void);
extern void DMA_StreamI2C_Rx_IRQHandler(void);
extern void SPI_ADC_IRQHandler(void);
extern void DMA_StreamSPI_EDF_Rx_IRQHandler(void);
extern void SPI_EDF_IRQHandler(void);
extern void USARTRS485_IRQHandler(void);
extern void DMA_StreamRS485_Tx_IRQHandler(void);
extern void EXITI_POWER_IRQHandler(void);

extern void setpoints_selecting(unsigned int *, unsigned int);

extern int str_to_int_DATE_Mmm(void);

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
extern void make_ekran_type_IEC61850_nodes(void);
extern void make_ekran_list_in_out_for_iec61850(unsigned int, size_t);
extern void start_transmint_data_via_CANAL1_MO(void);
extern void start_receive_data_via_CANAL1_MO(void);
extern void CANAL2_MO_routine(void);
extern void low_routine_for_KP(void);
extern void proc_Gs_blk_out(void *pv, unsigned long lCtrGsSrc, short *p_arrOrdNumsGsSignal);
extern void proc_Mms_blk_out(void *pv, unsigned long lCtrMmsSrc, short *p_arrOrdNumsMmsSignal);
extern void proc_Lan_blk_out(unsigned short *p_rang_Out_LAN, unsigned int *p_active_functions, void *pLanDsc);
// extern void inputPacketParserLAN(void);
void make_ekran_settings_synchro(void);
#endif

// extern void inputPacketParserUSB(void);
// extern void inputPacketParserRS485(void);

#if (__VER__ >= 8000000)
extern int _ForceReloadDstRules(void);
#endif

#endif
