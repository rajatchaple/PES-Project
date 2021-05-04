/******************************************************************************************************
 * @file test.h :
 * @brief    : This file contains defines includes and function prototypes for test.c
 *
 * @author 	 : Rajat Chaple (rajat.chaple@colorado.edu)
 * @date 	 : May 3, 2021
 *
 * @resource :
 *******************************************************************************************************/
#ifndef TEST_H_
#define TEST_H_

/*------------------------------------------------------------------------------------------------------
 * @brief :This function is a test mode for Goods sense device
 * 			It tests:
 * 			1) LEDs by sequence
 * 			2) Accelerometer sw interface and hardware
 *
 * 			User shall note LEDs sequence R_G_B_Y and LED indicator for tilts
 * 			This function is path of no return.. Reset is required without TEST_SWITCH pressed to enter
 * 			App Mode
 *
 * @param : none
 *
 * @return : none
 ------------------------------------------------------------------------------------------------------*/
void test_app(void);
#endif /* TEST_H_ */
