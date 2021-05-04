/*******************************************************************************************************
 * @file accelerometer.c :
 * @brief   : This file contains accelerometer initializations and measurements
 * 
 * @author  : Rajat Chaple (rajat.chaple@colorado.edu)
 * @date    : Apr 25, 2021
 *
 * @resource: 
 *******************************************************************************************************/

/*------------- Includes -----------*/
#include "log.h"
#include "i2c.h"
#include "gpio.h"
#include "timers.h"
#include "pwr_mgmt.h"
/*--------------Defines-------------*/
#define MMA_ADDR 					(0x1D)

#define REG_WHO_AM_I 				(0x0D)
#define REG_CTRL1					(0x2A)
#define REG_CTRL2					(0x2B)
#define PL_STATUS					(0x10)
#define PL_CFG						(0x11)
#define REG_XHI						(0x01)


#define WHO_AM_I_DEVICE_ID			(0x1A)
#define ABS(x) 		(((x)<0)? -(x) : (x))

/*------------ Variables ------------*/
static bool reference_set = false;
static int8_t ref_accel_orientation[3];

/*-------------------------------------------------------------------------------------------------------
 * This function enables accelerometer by turning it into active mode
 * (refer accelerometer.h for more details)
 -------------------------------------------------------------------------------------------------------*/
void enable_accelerometer()
{
	uint8_t data = 0x03;	//REG_CTRL1 : setting active mode, 8 bit samples and 800 Hz ODR (Output data rate)
	i2c_write(MMA_ADDR, REG_CTRL1, &data,  1);
}

/*-------------------------------------------------------------------------------------------------------
 * This function disables accelerometer by putting it into standby mode
 * (refer accelerometer.h for more details)
 -------------------------------------------------------------------------------------------------------*/
void disable_accelerometer()
{
	uint8_t data = 0x00;	//REG_CTRL1 : setting standby mode
	i2c_write(MMA_ADDR, REG_CTRL1, &data,  1);
}

/*-------------------------------------------------------------------------------------------------------
 * This function reads accelerometer values
 * (refer accelerometer.h for more details)
 -------------------------------------------------------------------------------------------------------*/
int8_t read_accelerometer_orientation(int8_t* accel)
{
	uint8_t data[3];
	int16_t out;
	int8_t sign = 1;
	uint16_t fractional_value_bitwise = 500;	//using integer point math instead of floating numbers	//reference AN4076.pdf

	enable_accelerometer();
	delay(3);	//allow enough time to switch from standby mode to active mode
	i2c_read(MMA_ADDR, REG_XHI, data, sizeof(data));	//read accelerometer data in X Y and Z axis
	disable_accelerometer();
	delay(1);
	for(int i = 0; i<3; i++)
		{
			accel[i] = (int8_t) data[i];

			//converting received data to mg
			if(accel[i] & 0x80)	//sign bit
				sign = -1;
			else
				sign = 1;

			if(accel[i] & 0x40)	//integer bit 
			{
				out = 1000;
			}
			else
			{
				out = 0;
			}

			//calculating fractional part
			for(uint8_t j=0x20; j >= 0x01; j>>=1)
			{
				if(j & accel[i])
				{
					if(sign == 1)
						out += fractional_value_bitwise;
					else
						out -= fractional_value_bitwise;
				}
				fractional_value_bitwise = (fractional_value_bitwise + 1) / 2;
			}
			if(out == 1000 || out == 2000)	//handling condition where out%1000 = 0. (mg range is -2g to 1.9g)
				out = 1000;
			else
				out %= 1000;

			out = sign * out;

			fractional_value_bitwise = 500;	//fractional part related to 5th bit of data
			accel[i] = (int8_t)((out * 90)/1000);	//converting mg into tilt angle degrees
			if((accel[i] < -90) || (accel[i] > 90))
				return -1;
		}

	return 0;
}


/*------------------------------------------------------------------------------------------------------
 * @brief :This function calculates tilt (dynamic)
 * (refer accelerometer.h for more details)
 ------------------------------------------------------------------------------------------------------*/
bool calculate_accelerometer_tilt(int8_t num_of_samples_per_reading, int8_t tilt_threshold)
{

	int8_t accel_orientation[3];
	uint8_t trust_percentage_for_readings = 100;
	int8_t tilt_status_and_count = 0;

	if(reference_set == false)
	{
		return 0;
	}


	for(int i = 0; i < num_of_samples_per_reading; i++)
	{
		read_accelerometer_orientation(accel_orientation);	//read accelerometer reading in angle degrees

		//finding delta tilt/rotation and apply filter
		if(((ABS(ref_accel_orientation[0] - accel_orientation[0]) > tilt_threshold))
			|| ((ABS(ref_accel_orientation[1] - accel_orientation[1]) > tilt_threshold))
			|| ((ABS(ref_accel_orientation[2] - accel_orientation[2]) > tilt_threshold)))
		{
			if(tilt_status_and_count < 127)	//checking if tilt value does not loopover from 127 to -128
				tilt_status_and_count++;
		}
		else
		{
			if(tilt_status_and_count > -128)	//checking if tilt value does not loopover from -128 to 127
				tilt_status_and_count = -num_of_samples_per_reading;
		}


		delay_low_power(50);	//delay with device in low power mode
	}

	if(tilt_status_and_count >= (num_of_samples_per_reading * trust_percentage_for_readings)/100)
		return true;
	else if(tilt_status_and_count <= -(num_of_samples_per_reading * trust_percentage_for_readings)/100)
		return false;

	return 0;
}

/*------------------------------------------------------------------------------------------------------
 * @brief :This function calculates reference position of accelerometer
 * (refer accelerometer.h for more details)
 ------------------------------------------------------------------------------------------------------*/
int8_t calc_ref_accelerometer(int8_t num_of_samples_per_reading, int8_t tilt_threshold)
{
	int8_t accel_orientation[3];
	int16_t temp_ref[3] = {0};

	if(reference_set == true)
	{
		return -1;
	}

	for(int i = 0; i < num_of_samples_per_reading; i++)
		{

			read_accelerometer_orientation(accel_orientation);

			temp_ref[0] += accel_orientation[0];
			temp_ref[1] += accel_orientation[1];
			temp_ref[2] += accel_orientation[2];
			//averaging for num_of_samples_per_reading readings
			if(i == (num_of_samples_per_reading-1))
			{
				ref_accel_orientation[0] = temp_ref[0] / num_of_samples_per_reading;
				ref_accel_orientation[1] = temp_ref[1] / num_of_samples_per_reading;
				ref_accel_orientation[2] = temp_ref[2] / num_of_samples_per_reading;
				reference_set = true;
			}

			delay(30);
		}

	return 0;
}

