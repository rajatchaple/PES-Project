/******************************************************************************************************
 * @file i2c.h :
 * @brief    : This file contains defines includes and funnction prototypes for i2c.c
 *
 * @author 	 : Rajat Chaple (rajat.chaple@colorado.edu)
 * @date 	 : Apr 25, 2021
 *
 * @resource :
 *******************************************************************************************************/
#ifndef I2C_H_
#define I2C_H_

/*---------------Includes-------------*/

/*---------------Defines--------------*/
#define MULTIPLE_BYTES
/*-------------- Datatypes-------------*/

/*------------------------------------------------------------------------------------------------------
 * @brief :This function initializes i2c0 on FRDM-KL25
 *
 * @param : none
 *
 * @return : none
 ------------------------------------------------------------------------------------------------------*/
void init_i2c(void);

/*------------------------------------------------------------------------------------------------------
 * @brief :This function initializes i2c0 on FRDM-KL25
 *
 * @param : device address
 * 			register to be accessed
 * 			data
 *
 * @return : none
 ------------------------------------------------------------------------------------------------------*/
#ifndef MULTIPLE_BYTES
void i2c_write_byte(uint8_t device_address, uint8_t reg, uint8_t data);
#else
void i2c_write(uint8_t device_address, uint8_t reg, uint8_t* data,  uint8_t data_count);
#endif
/*------------------------------------------------------------------------------------------------------
 * @brief :This function initializes i2c0 on FRDM-KL25
 *
 * @param : device address
 * 			register to be accessed
 * 			data
 *
 * @return : data byte read
 ------------------------------------------------------------------------------------------------------*/
#ifndef MULTIPLE_BYTES
uint8_t i2c_read_byte(uint8_t device_address, uint8_t reg);
#else
uint8_t i2c_read(uint8_t device_address, uint8_t reg, uint8_t* data, uint8_t data_count);
#endif
#endif /* I2C_H_ */
