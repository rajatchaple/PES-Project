/******************************************************************************************************
 * @file accelerometer.h :
 * @brief    : This file contains defines includes and funnction prototypes for accelerometer.c
 *
 * @author 	 : Rajat Chaple (rajat.chaple@colorado.edu)
 * @date 	 : Apr 25, 2021
 *
 * @resource :
 *******************************************************************************************************/
#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_

/*---------------Includes-------------*/
#include "stdint.h"
/*---------------Defines--------------*/
#define THRESHOLD_FOR_TILT			(40)	//mg
#define NUM_OF_SAMPLES_PER_READING	(16)

/*-------------- Datatypes-------------*/

/*------------------------------------------------------------------------------------------------------
 * @brief :This function initializes accelerometer with required configuration.
 *
 * @param : none
 *
 * @return : none
 ------------------------------------------------------------------------------------------------------*/
void init_accelerometer(void);

/*------------------------------------------------------------------------------------------------------
 * @brief :This function reads orientation(static)
 *
 * @param : none
 *
 * @return : returns 0 if read data is valid else -1
 ------------------------------------------------------------------------------------------------------*/
int8_t read_accelerometer_orientation(int8_t*);

/*------------------------------------------------------------------------------------------------------
 * @brief :This function calculates tilt (dynamic)
 *
 * @param : none
 *
 * @return : returns true if tilt
 ------------------------------------------------------------------------------------------------------*/
bool calculate_accelerometer_tilt(int8_t num_of_samples_per_reading, int8_t tilt_threshold);

#endif /* ACCELEROMETER_H_ */
