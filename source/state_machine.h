/******************************************************************************************************
 * @file state_machine.h :
 * @brief    : This file contains defines includes and function prototypes for state_machine.c
 *
 * @author 	 : Rajat Chaple (rajat.chaple@colorado.edu)
 * @date 	 : May 4, 2021
 *
 * @resource :
 *******************************************************************************************************/
#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_

/*------------------------------------------------------------------------------------------------------
 * @brief : This function is a state machine for goods sense device
 * 			It features:
 *		  			Calibration of accelerometer
 *		  			Tilt measurement and
 *		  			Indications
 *
 * @param : none
 *
 * @return : none
 ------------------------------------------------------------------------------------------------------*/
void tilt_measurement_state_machine(void);

#endif /* STATE_MACHINE_H_ */
