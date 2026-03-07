#ifndef __GLOBAL_FUNCTIONS_H
#define __GLOBAL_FUNCTIONS_H

#include "stm32f2xx_it.h"

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
#if defined(__ICCARM__) /*!< IAR Compiler */
#pragma data_alignment = 4
#endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */

__ALIGN_BEGIN USB_OTG_CORE_HANDLE USB_OTG_dev __ALIGN_END;

void MX_FATFS_Init(void);

void Configure_I2C(I2C_TypeDef *);
void FSMC_SRAM_Init(void);

int main(void);
void ar_routine_with_fatfs(unsigned int);
unsigned int ar_free_space(int *, int *);
void periodical_operations(unsigned int);
void periodical_operations_communication(unsigned int);
void global_vareiables_installation(void);
void start_settings_peripherals(void);
void start_tim4_canal2_for_interrupt_1mc(void);
void start_tim4_canal3_for_interrupt_10mkc(void);
void min_settings(__SETTINGS *);
void error_reading_with_eeprom(void);

void Interrupts_Config(void);

void lcd_init(void);
unsigned int wait_lcd_ready(void);
unsigned int clear_lcd(void);
unsigned int mode_viewing(unsigned int, unsigned int, unsigned int);
unsigned int hd44780_puts(unsigned char *, unsigned int);
unsigned int hd44780_test(unsigned char *, unsigned int);
unsigned int Win1251toHd44780(unsigned int);
unsigned int read_data_from_lcd(unsigned char *);
unsigned int write_data_to_lcd(unsigned char);
unsigned int write_command_to_lcd(unsigned char);
unsigned int hd44780_gotoxy(unsigned char, unsigned char);
unsigned int hd44780_puts_xy(unsigned char, unsigned char, unsigned char *);
void view_whole_ekran(void);
void check_state_key(GPIO_TypeDef *, uint16_t, unsigned int);
int index_language_in_array(int);

void inc_or_dec_value(unsigned int *, unsigned int);
unsigned int bcd_to_int(unsigned char);

void make_ekran_level_password(unsigned int password, unsigned int view);
void main_manu_function(void);
void make_ekran_main(void);
void make_ekran_time(void);
void edit_time(unsigned int);
void make_ekran_ask_rewrite(void);
unsigned int check_data_for_data_time_menu(void);
void make_ekran_about_error(const unsigned char[][MAX_COL_LCD]);
void make_ekran_about_activation_command(unsigned int, unsigned char[][MAX_NAMBER_LANGUAGE][MAX_COL_LCD]);
void make_ekran_configuration(unsigned int);
void make_ekran_measurement(void);
void make_ekran_measuremet_for_selsyn(void);
void make_ekran_current_voltage(unsigned int, unsigned int);
void make_ekran_voltage_for_selsyn(void);
void make_ekran_angle_for_selsyn(void);
void make_ekran_logomentr_voltage(void);
void make_ekran_frequency(void);
void convert_and_insert_char_for_measurement(unsigned int, unsigned int, unsigned int, unsigned int, unsigned char *, unsigned int);
void convert_and_insert_char_for_frequency(int, unsigned char *);
void make_ekran_chose_settings_any_protection(void);
void make_ekran_chose_settings_protection_with_groups(void);
void make_ekran_chose_setpoint_and_timeout(void);
void make_ekran_chose_timeout_control(void);
void make_ekran_chose_settings_vmp(void);
void make_ekran_chose_setpoint(void);
void make_ekran_chose_timeout(void);
void make_ekran_chose_control(void);
void make_ekran_chose_settings_switcher(void);
void make_ekran_chose_settings(void);
void make_ekran_chose_passwords(void);
void make_ekran_chose_extra_settings(void);
void make_ekran_setpoint_rpn(unsigned int);
void make_ekran_timeout_rpn(unsigned int);
void make_ekran_control_rpn(void);
void make_ekran_setpoint_mtz04(unsigned int);
void make_ekran_timeout_mtz04(unsigned int);
void make_ekran_control_zskh(void);
void make_ekran_setpoint_brp(unsigned int);
void make_ekran_control_brp(void);
void make_ekran_timeout_znkh(unsigned int);
void make_ekran_control_znkh(void);
void calc_symbol_and_put_into_working_ekran(unsigned char *, void *, void *, unsigned int *, unsigned int, unsigned int, unsigned int, unsigned int);
void calc_int_symbol_and_put_into_working_ekran(unsigned char *, unsigned int *, unsigned int *, unsigned int *, unsigned int);
unsigned int edit_setpoint(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
unsigned int check_data_setpoint(unsigned int, unsigned int, unsigned int);
void make_ekran_setpoint_Umin(unsigned int);
void make_ekran_timeout_Umin(unsigned int);
void make_ekran_control_Umin(void);
void make_ekran_setpoint_Umax(unsigned int);
void make_ekran_timeout_Umax(unsigned int);
void make_ekran_control_Umax(void);
void make_ekran_setpoint_UP(unsigned int);
void make_ekran_timeout_UP(unsigned int);
void make_ekran_control_UP(void);
void make_ekran_chose_of_list_for_ranguvannja(__id_input_output);
void make_ekran_transformator(void);
void make_ekran_transformator_control(void);
void make_ekran_set_function_in_bi(unsigned int, unsigned int, unsigned int *
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                                   ,
                                   int, int
#endif
);
void make_ekran_set_function_in_output_led_df_dt_reg(unsigned int, unsigned int, unsigned int *);
void check_current_index_is_presented_in_configuration(unsigned int *, int *, /*EL_FILTER_STRUCT[],*/ int, int, int, int, int, int, int, int, int, int);
void make_ekran_choose_CBOn_CBOff(void);
void make_ekran_setpoint_switch(void);
void make_ekran_timeout_switch(void);
void make_ekran_control_switch(void);
void make_ekran_setpoint_VMP(int);
void make_ekran_control_VMP(void);
void make_ekran_chose_settings_uvv(void);
void make_ekran_dopusk_dv(void);
void make_ekran_type_input_uvv(unsigned int);
void make_ekran_type_output_uvv(void);
void make_ekran_type_led_uvv(void);
void make_ekran_type_button_uvv(void);
void correct_dopusk_dv_when_type_of_input_signal_is_changed(void);
void make_ekran_chose_communication_parameters(void);
void make_ekran_name_of_cell(void);
void make_ekran_address(void);
void make_ekran_chose_setting_rs485(void);
void make_ekran_chose_setting_Ethernet(void);
void make_ekran_phy_layer_rs485(void);
void make_ekran_protocols_rs485(void);
void make_ekran_speed_interface(void);
void make_ekran_pare_interface(void);
void make_ekran_stopbits_interface(void);
void make_ekran_timeout_interface(void);

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
void make_ekran_settings_network_layer_Ethernet(void);
#endif

void make_ekran_chose_data_time_settings(void);
void make_ekran_timezone_dst(void);
void make_ekran_dst_rule(uint32_t);
void make_ekran_chose_registrators(void);
void make_ekran_settings_analog_registrators(void);
void make_ekran_timeout_analog_registrator(void);
void make_ekran_control_ar(void);
void make_ekran_extended_logic(void);
void make_ekran_general_pickups_el(void);
void make_ekran_chose_settings_df(void);
void make_ekran_type_df(void);
void make_ekran_timeout_df(unsigned int);
void make_ekran_list_type_source_df(void);
void make_ekran_set_reset_trigger(void);
void make_ekran_list_type_source_dt(void);
void make_ekran_type_source_tf(void);
void make_ekran_info(void);
void make_ekran_date_time_pz(void);
void make_ekran_chose_item_of_point_time_settings(void);
void make_ekran_time_settings(unsigned int);
void make_ekran_diagnostyka(unsigned int volatile *);
void make_ekran_list_inputs_outputs(void);
void make_ekran_state_inputs_or_outputs(unsigned int);
void make_ekran_list_registrators(void);
void make_ekran_list_records_registrator(unsigned int);
void make_ekran_list_records_registrator_ar(void);
void make_ekran_data_and_time_elem_d_p_a_registrator(unsigned int type_of_registrator);
void make_ekran_list_titles_for_record_of_digital_registrator(void);
void make_ekran_data_and_time_of_records_registrator(unsigned int);
void make_ekran_title_analog_value_records_digital_registrator(void);
void make_ekran_analog_value_records_digital_registrator(void);
void make_ekran_changing_signals_digital_registrator(void);
void make_ekran_changing_diagnostics_pr_err_registrator(void);
void make_ekran_list_titles_for_record_of_state_cmd_registrator(void);
void make_ekran_changing_signals_statistica_registrator(void);
void make_ekran_data_and_time_elem_stt_registrator(unsigned int type_of_registrator);
void make_ekran_timeout_digital_registrator(void);
void make_ekran_settings_digital_registrators(void);

void make_ekran_setting_language(void);
void make_ekran_resurs(void);
void make_ekran_grupa_ustavok(void);
void make_ekran_vidkluchenja(void);

unsigned int action_after_changing_of_configuration(unsigned int, __SETTINGS *);
void action_after_changing_resurs_pickup(__SETTINGS *);
void action_during_changing_button_mode(__SETTINGS *, __SETTINGS *);
void def_pickup_timeout_UP(__SETTINGS *, uint32_t, uint32_t);
void action_after_changing_input_UP(__SETTINGS *, uint32_t, uint32_t);
void action_after_changing_ctrl_UP(__SETTINGS *, uint32_t);
void change_setpoint_to_ctrl_UP(__SETTINGS *const, uint32_t const);
//void action_after_changing_number_el(__SETTINGS *, unsigned int);
void calculate_namber_bit_waiting_for_rs_485(void);
void fix_change_settings(unsigned int, unsigned int);
unsigned int set_new_settings_from_interface(unsigned int);
void restore_trigger_functions(unsigned int *);
void changing_diagnostyka_state(void);
unsigned int count_number_set_bit(unsigned int *, unsigned int);

long GetNumberChangingInLogElem(long lIdx);

void CmdPlusTimeLogHundler(unsigned int *p_active_functions);
void GetCmdPlusTimeLogElem(unsigned int *p_elem, long lIdx);
void GetDateTimeLogElem(unsigned int *p_elem, long lIdx);
void GetMsLogElem(unsigned int *p_elem, long lIdx);
long GetNumberChangingInLogElemUseLocal(long lIdx);
long GetNumberChangingInLogElem(long lIdx);
void CmdPlusTimeStampLogHundler(unsigned int *p_active_functions);

long GetMsLogElemPlWnum(unsigned int *p_elem, long lIdx, unsigned long ulWorkNumber);
long GetDateTimeLogElemPlWnum(unsigned int *p_elem, long lIdx, unsigned long ulWorkNumber);
long GetCmdPlusTimeLogElemPlWnum(unsigned int *p_elem, long lIdx, unsigned long ulWorkNumber);
void CleanCmdPlusTimeLog(void);
void PuCmdinRawBuf(unsigned int *p_active_functions);
void make_ekran_time_ms_c_val_c_name_sr_format_changing_signals(void);
void make_ekran_signals_stable_one_in_data_elem_stt_reg(void);

void make_ekran_data_elem_stt_registrator(void);

extern void CmdPlusTimeStampLogHundler(unsigned int *p_active_functions);
void Usb_routines_irq(void);
void Usb_routines(void);
void USART_RS485_Configure(void);
void restart_monitoring_RS485(void);
void start_transmint_data_via_RS_485(unsigned int);
unsigned short int AddCRC(unsigned char, unsigned short int);

unsigned int start_write_buffer_via_I2C(uint32_t, uint32_t, uint8_t *, uint32_t);
unsigned int start_read_buffer_via_I2C(uint32_t, uint32_t, uint8_t *, uint32_t);
void main_routines_for_i2c(void);
void error_start_i2c(void);

void find_new_ADC_canal_to_read(unsigned int, unsigned int *);
void control_reading_ADCs(void);
void operate_integral_values_ADCs(void);
void Fourier(void);
void fapch(void);
void calc_measurement(unsigned int);
unsigned int sqrt_64(unsigned long long);

void main_routines_for_spi1(void);
void main_function_for_dataflash_resp(int);
void main_function_for_dataflash_req(int);
void start_checking_dataflash(void);
void start_exchange_via_spi(int, unsigned int);
void dataflash_status_read(int);
void dataflash_set_pagesize_256(int);
void dataflash_erase(int);
void dataflash_mamory_page_program_through_buffer(int);
void dataflash_mamory_read(int);
void dataflash_mamory_page_into_buffer(int);
void dataflash_mamory_write_buffer(int);
void dataflash_mamory_buffer_into_memory(int);

void control_settings(void);
void control_ustuvannja(void);
void control_trg_func(void);
unsigned int control_info_rejestrator(__INFO_REJESTRATOR *, unsigned char);
unsigned int control_info_ar_rejestrator(__INFO_AR_REJESTRATOR *, unsigned char);
void control_angle(void);
void control_resurs(void);

void watchdog_routine(unsigned int, unsigned int const);
void total_error_sw_fixed(void);

void setpoints_selecting(unsigned int *, unsigned int);

int str_to_int_DATE_Mmm(void);

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
void make_ekran_type_IEC61850_nodes(void);
void make_ekran_list_in_out_for_iec61850(unsigned int, size_t);
void start_transmint_data_via_CANAL1_MO(void);
void start_receive_data_via_CANAL1_MO(void);
void CANAL2_MO_routine(void);
void low_routine_for_KP(void);
// void inputPacketParserLAN(void);
void make_ekran_settings_synchro(void);
#endif

// extern void inputPacketParserUSB(void);
// extern void inputPacketParserRS485(void);

#if (__VER__ >= 8000000)
extern int _ForceReloadDstRules(void);
#endif

#endif
