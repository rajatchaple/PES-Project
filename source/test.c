/*******************************************************************************************************
 * @file test.c :
 * @brief   : This file contains Test functionality of Goods sence device
 * 			To enter device into this test mode user shall press and hold TEST_SWITCH followed by reset
 * 
 * @author  : Rajat Chaple (rajat.chaple@colorado.edu)
 * @date    : May 3, 2021
 *
 *******************************************************************************************************/

/*------------- Includes -----------*/
#include "accelerometer.h"
#include "gpio.h"
#include "timers.h"
/*--------------Defines-------------*/

/*------------- Variables-----------*/
extern led_pattern_t post_calib_led_pattern[4];

led_pattern_t test_led_pattern[]={
	{RED,				  	1000},
	{GREEN,					1000},
	{BLUE,					1000},
	{OFF_ONBOARD_TRICOLOR,	0},
	{OFF_EXTERNAL_LED1,		1000},
	{YELLOW_EXTERNAL_LED1,	0}
};
/*-------------------------------------------------------------------------------------------------------
 * This function tests Goods sense device functionality.
 * (refer test.h for more details)
 -------------------------------------------------------------------------------------------------------*/
void test_app()
{
	//Testing LEDs
	delay(200);
	led_blink_pattern(test_led_pattern, NUM_BLINKS(test_led_pattern));

	//calibrating reference value aka calibration for tilt
	calc_ref_accelerometer(NUM_OF_SAMPLES_PER_READING, THRESHOLD_FOR_TILT);

	//post-calibration LED sequence
	led_blink_pattern(post_calib_led_pattern, NUM_BLINKS(post_calib_led_pattern));

	while(1)
	{
		if(calculate_accelerometer_tilt(NUM_OF_SAMPLES_PER_READING, THRESHOLD_FOR_TILT))	//calculating tilt
		{
			set_led(CYAN);
		}
		else
		{
			set_led(OFF_ONBOARD_TRICOLOR);
		}
	}
}
