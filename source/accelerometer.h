/******************************************************************************************************
 * @file accelerometer.h :
 * @brief    : This file contains defines includes and funnction prototypes for accelerometer.c
 *
 * @author 	 : Rajat Chaple (rajat.chaple@colorado.edu)
 * @date 	 : Apr 25, 2021
 *
 *******************************************************************************************************/
#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_

/*---------------Includes-------------*/
#include <stdint.h>
#include <stdbool.h>
/*---------------Defines--------------*/
#define THRESHOLD_FOR_TILT			(40)	//mg
#define NUM_OF_SAMPLES_PER_READING	(5)

/*------------------------------------------------------------------------------------------------------
 * @brief :This function enables accelerometer by changing it into active mode
 * @param : none
 * @return : none
 ------------------------------------------------------------------------------------------------------*/
void enable_accelerometer(void);

/*------------------------------------------------------------------------------------------------------
 * @brief :This function disables accelerometer by putting it into standby mode
 * @param : none
 * @return : none
 ------------------------------------------------------------------------------------------------------*/
void disable_accelerometer(void);

/*------------------------------------------------------------------------------------------------------
 * @brief :This function reads orientation(static)
 * @param : none
 * @return : returns 0 if read data is valid else -1
 ------------------------------------------------------------------------------------------------------*/
int8_t read_accelerometer_orientation(int8_t*);

/*------------------------------------------------------------------------------------------------------
 * @brief :This function calculates tilt (dynamic)
 * @param : none
 * @return : returns true if tilt
 ------------------------------------------------------------------------------------------------------*/
bool calculate_accelerometer_tilt(int8_t num_of_samples_per_reading, int8_t tilt_threshold);

/*------------------------------------------------------------------------------------------------------
 * @brief :This function calculates reference orientation . To be used for comparison later on.
 * @param : none
 * @return : returns true if tilt
 ------------------------------------------------------------------------------------------------------*/
int8_t calc_ref_accelerometer(int8_t num_of_samples_per_reading, int8_t tilt_threshold);

#endif /* ACCELEROMETER_H_ */
