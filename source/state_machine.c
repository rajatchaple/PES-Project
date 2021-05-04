/*******************************************************************************************************
 * @file statemachine.c :
 * @brief   : This file contains state machines for Goods Sense device.
 * 
 * @author  : Rajat Chaple (rajat.chaple@colorado.edu)
 * @date    : May 4, 2021
 *
 *******************************************************************************************************/

/*------------- Includes -----------*/
#include "pwr_mgmt.h"
#include "accelerometer.h"
#include "gpio.h"
#include "timers.h"

/*------------- Variables-----------*/

enum accelerometer_phases{
	CALIBRATION,
	INDICATION,
	TILT_MEASUREMENT
}accel_phase = CALIBRATION;

led_pattern_t post_calib_led_pattern[4]={
	{BLUE,				  	150},
	{OFF_ONBOARD_TRICOLOR,	50},
	{CYAN,					80},
	{OFF_ONBOARD_TRICOLOR,	0}
};

static ticktime_t tilt_measurement_timer = 0;
/*-------------------------------------------------------------------------------------------------------
 * This function is a state machine for goods sense device
 * 		It features:
 * 			Calibration of accelerometer
 * 			Tilt measurement and
 * 			Indications
 * (refer statemachine.h for more details)
 -------------------------------------------------------------------------------------------------------*/
void tilt_measurement_state_machine()
{
	static uint8_t num_of_tilts = 0;

	switch(accel_phase)
	{
		case CALIBRATION:
			if(is_wakeup_for_calibration() == true)		//device waits for key-press
			{
				//performing calibration to set reference value of a device
				calc_ref_accelerometer(NUM_OF_SAMPLES_PER_READING, THRESHOLD_FOR_TILT);
				//post-calibration LED sequence
				led_blink_pattern(post_calib_led_pattern, NUM_BLINKS(post_calib_led_pattern));

				accel_phase = INDICATION;
			}
			break;

		case INDICATION:
				if(num_of_tilts == 0)
					set_led(GREEN);		//this means goods was not tilted
				else if(num_of_tilts <= 2)
					set_led(YELLOW);	//this means goods was tilted for less than ~(2*TILT_MEASUREMENT_INTERVAL) WARNING
				else
					set_led(RED);		//this means goods was tilted for more than ~(2*TILT_MEASUREMENT_INTERVAL) ALERT

				delay(1);
				set_led(OFF_ONBOARD_TRICOLOR);

				tilt_measurement_timer += delay_low_power(INDICATION_INTERVAL_MS);
				if(tilt_measurement_timer >= TILT_MEASUREMENT_INTERVAL)
				{
					accel_phase = TILT_MEASUREMENT;
					tilt_measurement_timer = 0;
				}
				break;

		case TILT_MEASUREMENT:
				if(calculate_accelerometer_tilt(NUM_OF_SAMPLES_PER_READING, THRESHOLD_FOR_TILT))
				{
					if(num_of_tilts < 255)	//avoiding crossover to 0
						num_of_tilts++;
				}
				accel_phase = INDICATION;
				break;

		default:
			// Do nothing
			break;
	}
}
