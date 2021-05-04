/******************************************************************************************************
 * @file timers.h :
 * @brief    : This file contains defines includes and function prototypes for timers.c
 *
 * @author 	 : Rajat Chaple (rajat.chaple@colorado.edu)
 * @date 	 : Apr 27, 2021
 *
 * @resource : This file was created by using KL25z Reference manual
 *******************************************************************************************************/
#ifndef TIMERS_H_
#define TIMERS_H_

/*---------------Includes-------------*/

/*---------------Defines--------------*/
//There would be delays accumulating when device is run mode and specially when it is measuring tilt.
//Indication Interval neglects these delays and interval is only in terms of sleep time
#define INDICATION_INTERVAL_MS		(3000)
#define TILT_MEASUREMENT_INTERVAL	(12000)	//shall be multiple of INDICATION_INTERVAL_MS as same LPTIMER0 used

/*-------------- Datatypes-------------*/
typedef uint32_t ticktime_t;

/*------------------------------------------------------------------------------------------------------
 * @brief :This function initializes low power timer module
 *
 * @param : none
 *
 * @return : none
 ------------------------------------------------------------------------------------------------------*/
void init_lptmr(void);

/*------------------------------------------------------------------------------------------------------
 * @brief :This function enables low power timer module
 *
 * @param : none
 *
 * @return : none
 ------------------------------------------------------------------------------------------------------*/
void enable_lptimer0();

/*----------------------------------------------------------------------------
 * Initializing system ticks to provide an interrupt for given time
 *
 * Parameters: None
 * Returns: None
 ----------------------------------------------------------------------------- */
void init_systick(void);

/*----------------------------------------------------------------------------
 * returns time since startup, in 1 milliseond resolution
 *
 * Parameters: None
 * Returns: ticktime in milliseconds
 ----------------------------------------------------------------------------- */
ticktime_t now();

/*----------------------------------------------------------------------------
 * reset timer to 0 without affecting now()
 *
 * Parameters: None
 * Returns: None
 ----------------------------------------------------------------------------- */
void reset_timer();

/*----------------------------------------------------------------------------
 * returns current value since last call to reset_timer()
 *
 * Parameters: None
 * Returns: ticktimes in milliseconds since last reset
 ----------------------------------------------------------------------------- */
ticktime_t get_timer();

/*----------------------------------------------------------------------------
 * blocking delay funcion based on systick
 *
 * Parameters: None
 * Returns: ticktimes in milliseconds since last reset
 ----------------------------------------------------------------------------- */
void delay(ticktime_t);

/*----------------------------------------------------------------------------
 * blocking delay funcion based on lptimer (low power)
 *
 * Parameters: None
 * Returns: ticktimes in milliseconds (delay time)
 ----------------------------------------------------------------------------- */
ticktime_t delay_low_power(ticktime_t time_ms);
#endif /* TIMERS_H_ */
