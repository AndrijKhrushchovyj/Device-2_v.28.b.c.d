#ifndef __CONSTANTS__
#define __CONSTANTS__

#define _TEST_DURATION

#include "const_global.h"

#include "const_fsmc.h"

#include "const_lcd_and_keyboard.h"
#include "const_menu_level_password.h"
#include "const_menu_main.h"
#include "const_menu_time.h"
#include "const_menu_configuration.h"
#include "const_menu_measurement.h"
#include "const_menu_chose_settings_protection.h"
#include "const_menu_state_inputs_outputs.h"
#include "const_menu_chose_settings.h"
#include "const_menu_chose_extra_settings.h"
#include "const_menu_chose_grupa_ustavok.h"
#include "const_menu_rpn.h"
#include "const_menu_zskh.h"
#include "const_menu_brp.h"
#include "const_menu_znkh.h"
#include "const_menu_urov.h"
#include "const_menu_zop.h"
#include "const_menu_Umin.h"
#include "const_menu_Umax.h"
#include "const_menu_UP.h"

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
#include "with iec61850/const_menu_inputs.h"
#include "with iec61850/const_menu_IEC61850.h"
#include "with iec61850/const_menu_diagnostyka.h"
#else
#include "without iec61850/const_menu_inputs.h"
#include "without iec61850/const_menu_diagnostyka.h"
#endif

#include "const_menu_outputs.h"
#include "const_menu_leds.h"
#include "const_menu_transformator.h"
#include "const_menu_chose_settings_switcher.h"
#include "const_menu_switch.h"
#include "const_menu_chose_settings_uvv.h"
#include "const_menu_chose_communication_parameters.h"
#include "const_menu_chose_data_time_settings.h"
#include "const_menu_chose_settings_registrators.h"
#include "const_menu_resurs.h"
#include "const_menu_VMP.h"
#include "const_menu_extended_logic.h"
#include "const_menu_general_pickups_el.h"
#include "const_menu_df.h"
#include "const_menu_dt.h"
#include "const_menu_d_and.h"
#include "const_menu_d_or.h"
#include "const_menu_d_xor.h"
#include "const_menu_d_not.h"
#include "const_menu_transfer_functions.h"
#include "const_menu_buttons.h"
#include "const_menu_info.h"
#include "const_menu_point_time_settings.h"
#include "const_menu_registrators.h"
#include "const_menu_setting_language.h"
#include "const_watchdog.h"
#include "const_menu_vidkluchennja.h"
#include "const_menu_reprogram.h"

#include "const_septoints_interval.h"
#include "const_measurement.h"

#include "const_koef_povernennja.h"
#include "const_global_timers.h"

#include "const_eeprom.h"
#include "const_i2c.h"

#include "const_driver_serial_dataflash.h"

#include "const_interfaces.h"

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
#include "const_MO.h"
#include "diagnostyka_mo.h"
#endif

#include "SEGGER_SYSVIEW.h"

#endif
