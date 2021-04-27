/***************************************************************************************
 * @file gpio_pins.c :
 * @brief : This file contains all gpio pins needed to be initialized by init_gpio()
 *
 * @author : Rajat Chaple (rajat.chaple@colorado.edu)
 * @date : Feb 21 2021
 *
 ****************************************************************************************/

#ifndef GPIO_PINS_H_
#define GPIO_PINS_H_

#include "MKL25Z4.h"
#include "stdint.h"
#include "gpio.h"

#define RED_LED_PIN 	(18)
#define GREEN_LED_PIN 	(19)
#define BLUE_LED_PIN	 (1)

//Add or Remove entries in the following table to configure or remove GPIOs as Input or Output
gpio_pin_definition_t gpio_pins[] =
{		//GPIO		port		pin		direction	pull up/down		logic
		{GPIOB,		PORTB,		18,		output,			NA,			active_low},	//Red LED
		{GPIOB,		PORTB,		19,		output,			NA,			active_low},	//Green LED
		{GPIOD,		PORTD,		1,		output,			NA,			active_low}		//Blue LED
};


#endif /* GPIO_PINS_H_ */
