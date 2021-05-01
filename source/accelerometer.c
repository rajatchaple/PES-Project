/*******************************************************************************************************
 * @file accelerometer.c :
 * @brief   : This file contains 
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
/*--------------Defines-------------*/
#define MMA_ADDR 					(0x1D)

#define REG_WHO_AM_I 				(0x0D)
#define REG_CTRL1					(0x2A)
#define PL_STATUS					(0x10)
#define PL_CFG						(0x11)
#define REG_XHI						(0x01)


#define WHO_AM_I_DEVICE_ID			(0x1A)

#define ABS(x) 		(((x)<0)? -(x) : (x))

/*------------- Datatypes-----------*/

/*-------------------------------------------------------------------------------------------------------
 * This function initializes accelerometer
 * (refer accelerometer.h for more details)
 -------------------------------------------------------------------------------------------------------*/
void init_accelerometer()
{
	//include this in test mode
//	uint8_t data = 0;
//	i2c_read(MMA_ADDR, REG_WHO_AM_I, &data, sizeof(data));
//	if(data != WHO_AM_I_DEVICE_ID)
//	{
//		set_led(RED);//set RED LED as an error
//		return;
//	}


	data = 0x00;	//REG_CTRL1 : setting standby mode
	i2c_write(MMA_ADDR, REG_CTRL1, &data,  1);


//	//enabling portrait landscape detection
//	data = 0x00;
//	i2c_write(MMA_ADDR, PL_CFG, &data,  1);


	data = 0x03;	//REG_CTRL1 : setting active mode, 8 bit samples and 800 Hz ODR (Output data rate)
	i2c_write(MMA_ADDR, REG_CTRL1, &data,  1);

	//	data = 0x01;	//REG_CTRL1 : setting active mode, 14 bit samples and 800 Hz ODR (Output data rate)
//	i2c_write(MMA_ADDR, REG_CTRL1, &data,  1);

	LOG("I2C communication successful\r\n");
}

/*-------------------------------------------------------------------------------------------------------
 * This function reads accelerometer values
 * (refer accelerometer.h for more details)
 -------------------------------------------------------------------------------------------------------*/
int8_t read_accelerometer_orientation(int8_t* accel)
{
	uint8_t data[3];
//	int8_t accel[3] = {0};
	int16_t out;
	int8_t sign = 1;
	uint16_t fractional_value_bitwise = 500;	//using integer point math instead of floating numbers	//reference AN4076.pdf
//	static bool reference_set = false;
//	static int16_t ref_accel[3] = {0};


	i2c_read(MMA_ADDR, REG_XHI, data, sizeof(data));	//read accelerometer data in X Y and Z axis

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
			accel[i] = (int8_t)((out * 90)/1000);	//converting mg into tilt degrees
			if((accel[i] < -90) || (accel[i] > 90))
				return -1;
		}

	return 0;



//	//considering samples
//	if(reference_set == false)
//	{
//		reference_set = true;
//		ref_accel[0] = accel[0];
//		ref_accel[1] = accel[1];
//		ref_accel[2] = accel[2];
//		LOG("Reference -> ", accel[0], accel[1], accel[2]);
//	}
//
//	static int8_t tilt_status_and_count = 0;
//	static uint8_t num_of_measurements_to_be_taken = 5;
//	LOG("abs val accelX = %d, accelY = %d, accelZ = %d 	 ", (accel[0]), (accel[1]), (accel[2]));
//	LOG("rot : accelX = %d, accelY = %d, accelZ = %d", (accel[0]-ref_accel[0]), (accel[1]-ref_accel[1]), (accel[2]-ref_accel[2]));
//	if(((ABS(ref_accel[0] - accel[0]) > THRESHOLD_FOR_TILT)) ||
//		((ABS(ref_accel[1] - accel[1]) > THRESHOLD_FOR_TILT)) ||
//		((ABS(ref_accel[2] - accel[2]) > THRESHOLD_FOR_TILT)))
//	{
//		if(tilt_status_and_count < 127)
//			tilt_status_and_count++;
//
////		set_led(GREEN);
//	}
//	else
//	{
//		if(tilt_status_and_count > -128)
//			tilt_status_and_count--;
//	//	set_led(OFF);
//	}
//	num_of_measurements_to_be_taken--;
//	if(num_of_measurements_to_be_taken <= 0)	//check if there is tilt in atleast 6 of 8 samples
//	{
//		if(tilt_status_and_count > 4)
//			set_led(YELLOW);
//		else if(tilt_status_and_count < -4)
//			set_led(OFF);
//
//		num_of_measurements_to_be_taken = 5;
//		tilt_status_and_count = 0;
//	}
//	LOG("\tTilt Count : %d\r\n", tilt_status_and_count);

//	LOG("\r\n");
}

static bool reference_set = false;
static int8_t ref_accel_orientation[3];
/*------------------------------------------------------------------------------------------------------
 * @brief :This function calculates tilt (dynamic)
 * (refer accelerometer.h for more details)
 ------------------------------------------------------------------------------------------------------*/
bool calculate_accelerometer_tilt(int8_t num_of_samples_per_reading, int8_t tilt_threshold)
{
	int8_t accel_orientation[3];
//	static int8_t ref_accel_orientation[3];
	uint8_t trust_percentage_for_readings = 80;

	int8_t tilt_status_and_count = 0;
	volatile int d = 4000;
	int16_t temp_ref[3] = {0};

	if(reference_set == false)
	{
		return 0;
	}

	for(int i = 0; i < num_of_samples_per_reading; i++)
	{
		read_accelerometer_orientation(accel_orientation);
		//if(reference_set == true)
		{
//			LOG("abs val accelX = %d, accelY = %d, accelZ = %d 	 ", (accel_orientation[0]), (accel_orientation[1]), (accel_orientation[2]));
//			LOG("tilt : accelX = %d, accelY = %d, accelZ = %d\r\n", \
					(accel_orientation[0]-ref_accel_orientation[0]), \
					(accel_orientation[1]-ref_accel_orientation[1]), \
					(accel_orientation[2]-ref_accel_orientation[2]));

			if(((ABS(ref_accel_orientation[0] - accel_orientation[0]) > tilt_threshold))
				|| ((ABS(ref_accel_orientation[1] - accel_orientation[1]) > tilt_threshold))
				|| ((ABS(ref_accel_orientation[2] - accel_orientation[2]) > tilt_threshold)))
			{
				if(tilt_status_and_count < 127)
					tilt_status_and_count++;
			}
			else
			{
				if(tilt_status_and_count > -128)
					tilt_status_and_count--;
			}

		}

		while(d--)
	    	{__asm volatile ("nop");};
	   	d = 4000;
	}


	LOG("\tTilt Count : %d\r\n", tilt_status_and_count);

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

	uint8_t trust_percentage_for_readings = 80;
	int8_t tilt_status_and_count = 0;
	volatile int d = 4000;
	int16_t temp_ref[3] = {0};

	if(reference_set == true)
	{
		return -1;
	}
	for(int i = 0; i < num_of_samples_per_reading; i++)
		{
			read_accelerometer_orientation(accel_orientation);


	//			ref_accel_orientation[0] = accel_orientation[0];
	//			ref_accel_orientation[1] = accel_orientation[1];
	//			ref_accel_orientation[2] = accel_orientation[2];
				temp_ref[0] += accel_orientation[0];
				temp_ref[1] += accel_orientation[1];
				temp_ref[2] += accel_orientation[2];
				if(i == (num_of_samples_per_reading-1))
				{
					ref_accel_orientation[0] = temp_ref[0] / num_of_samples_per_reading;
					ref_accel_orientation[1] = temp_ref[1] / num_of_samples_per_reading;
					ref_accel_orientation[2] = temp_ref[2] / num_of_samples_per_reading;
					reference_set = true;
				}

			while(d--)
				{__asm volatile ("nop");};
			d = 4000;
		}


	return 0;
}

