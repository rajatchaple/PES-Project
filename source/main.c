/*******************************************************************************************************
 * @file main.c :
 * @brief   : Application Entry point for projct Goods-sense.
 * 			  This project is developed to sense condition of goods while transporting. THe project
 * 			  project include implementation on accelerometer and low power operation.
 *
 * @author  : Rajat Chaple (rajat.chaple@colorado.edu)
 * @date    : Apr 25, 2021
 *
  *******************************************************************************************************/

/*--------- Includes ---------*/
#include <stdio.h>
#include "log.h"
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "state_machine.h"
#include "gpio.h"
#include "i2c.h"
#include "timers.h"
#include "pwr_mgmt.h"
#include "sysclock.h"
#include "test.h"

/*--------- Variables ---------*/
led_pattern_t boot_led_pattern[]={
	{MAGENTA,				200},
	{OFF_ONBOARD_TRICOLOR,	0}
};

/*-------------------------------------------------------------------------------------------------------
 * This function initializes all the modules in this project
 -------------------------------------------------------------------------------------------------------*/
void init_system()
{
    sysclock_init();
    init_i2c();
    init_lptmr();
    init_gpio();
    init_pwr_mgmt();
    init_systick();
}

/*-------------------------------------------------------------------------------------------------------
 * application entry point
 ------------------------------------------------------------------------------------------------------*/
int main(void) {

    init_system();	//initializes required modules

    //Boot LED sequence
    led_blink_pattern(boot_led_pattern, NUM_BLINKS(boot_led_pattern));


	if(read_test_switch() == true)	//mode is changed to Test mode (reset required to return to application mode)
	{
		set_led(YELLOW_EXTERNAL_LED1);
		/*REDIRECT TO TEST MODE*/
		test_app();
	}
	else
	{
		set_led(OFF_EXTERNAL_LED1);
	}

	/*APPLICATION MODE*/
	deep_sleep_lls(WU_EXT_PIN_PTD4_FILT);	//sleep into lls mode (deep sleep) to conserve battery

    while(1)
    {
    	//perform tilt measurement and indicate its status
    	tilt_measurement_state_machine();
    }

    return 0 ;
}
