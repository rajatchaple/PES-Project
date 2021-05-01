/******************************************************************************************************
 * @file timers.h :
 * @brief    : This file contains defines includes and function prototypes for timers.c
 *
 * @author 	 : Rajat Chaple (rajat.chaple@colorado.edu)
 * @date 	 : Apr 27, 2021
 *
 * @resource :
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

/*------------------------------------------------------------------------------------------------------
 * @brief :This function initializes low power timer module
 *
 * @param : none
 *
 * @return : none
 ------------------------------------------------------------------------------------------------------*/
void init_lptmr(void);
#endif /* TIMERS_H_ */
