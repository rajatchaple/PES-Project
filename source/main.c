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

/*--------- Defines ---------*/
#include <stdio.h>
#include "log.h"
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "init.h"
#include "accelerometer.h"
#include "gpio.h"
/*-------------------------------------------------------------------------------------------------------
 * application entry point
 ------------------------------------------------------------------------------------------------------*/
int main(void) {

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    LOG("Goods Sense device starting...\r\n");

    init_system();
    init_gpio();
    init_accelerometer();


    int i = 800000;
    set_led(CYAN);
	while(i--)
	{};
	set_led(OFF);

    while(1)
    {
//    	if(calculate_accelerometer_tilt(NUM_OF_SAMPLES_PER_READING, THRESHOLD_FOR_TILT))
//    	{
//    		set_led(YELLOW);
//    	}
//    	else
//    	{
//    		set_led(OFF);
//    	}
    	__asm volatile ("nop");
    }


    return 0 ;
}
