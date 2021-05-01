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
#include "accelerometer.h"
#include "gpio.h"
#include "i2c.h"
#include "timers.h"
#include "pwr_mgmt.h"

/*--------- Defines ---------*/
/*-------------------------------------------------------------------------------------------------------
 * This function initializes all the modules in this project
 -------------------------------------------------------------------------------------------------------*/
void init_system()
{
  	/* Init board hardware. */
//    BOARD_InitBootPins();
    BOARD_InitBootClocks();
//    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
//    BOARD_InitDebugConsole();
#endif
    init_i2c();
    init_lptmr();
    init_gpio();
	init_accelerometer();
    init_pwr_mgmt();
}

/*-------------------------------------------------------------------------------------------------------
 * application entry point
 ------------------------------------------------------------------------------------------------------*/
int main(void) {

	int num_of_tilts = 0;
    bool entered_low_pwr = false;
    LOG("Goods Sense device starting...\r\n");

    init_system();


    volatile int i = 800000;
    set_led(MAGENTA);
	while(i--)
	{
		__asm volatile ("nop");
	};
	set_led(OFF);
//	i = 800000;
//	while(i--)
//		{
//			__asm volatile ("nop");
//		};

	//init accelerometer for calibration (First call to this finds ref value)
	calc_ref_accelerometer(NUM_OF_SAMPLES_PER_READING, THRESHOLD_FOR_TILT);
	entered_low_pwr = true;
	run_to_lls_pwr_mode();

    while(1)
    {
//        if(entered_low_pwr == false)
//        {
//        	calculate_accelerometer_tilt(NUM_OF_SAMPLES_PER_READING, THRESHOLD_FOR_TILT);
//            entered_low_pwr = true;
//            run_to_lls_pwr_mode();
//        }

//        if(llwu_occured == true)
//        {
//            llwu_occured = false;
//
//            i = 1500000;
//            set_led(RED);
//            while(i--)
//            {};
//            set_led(OFF);
//            run_to_lls_pwr_mode();
//
//        }

    	if(is_wakeup_for_tilt_measurement() == true)
    	{
    		if(calculate_accelerometer_tilt(NUM_OF_SAMPLES_PER_READING, THRESHOLD_FOR_TILT))
			{
    			num_of_tilts++;
//    			set_led(CYAN);
			}
//    		else
//    		{
//    			set_led(OFF);
//    		}

    	}

    	if(is_wakeup_for_indication() == true)
    	{
    		if(num_of_tilts)
    			set_led(RED);
    		else
    			set_led(GREEN);

			i = 4000;
			while(i--)
				{
					__asm volatile ("nop");
				};
			set_led(OFF);

			run_to_lls_pwr_mode();

    	}

    }


    return 0 ;
}
