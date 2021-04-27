/***************************************************************************************
 * @file gpio.c :
 * @brief : This file contains defines includes and funnction prototypes for gpio.c
 *
 * @author : Rajat Chaple (rajat.chaple@colorado.edu)
 * @date : Feb 21 2021
 *
 ****************************************************************************************/

#ifndef GPIO_H_
#define GPIO_H_

/*---------------Includes-------------*/
#include "MKL25Z4.h"
#include "stdint.h"

/*---------------Defines--------------*/
#define MASK(x)			(1<<x)


/*--------------- Datatypes-------------*/
typedef enum led_color{
	OFF,	//000
	BLUE,	//001
	GREEN,	//010
	CYAN,	//011 -> GREEN+BLUE
	RED,	//100
	MAGENTA,//101 -> RED+BLUE
	YELLOW,	//110
	WHITE	//111 -> WHITE
}led_color_t;

typedef enum pull_control_s{
	NA = -1,
	pull_down_enable = 0,
	pull_up_enable = 1,
}pull_control_t;

typedef enum direction_s{
	input,
	output
}direction_t;

//defines whether external circuit is active low or active high
typedef enum logic_s{
	active_low,
	active_high
}logic_t;

//configuration structure for gpio pin
typedef struct gpio_pin_definition_s{
	GPIO_Type* gpio;
	PORT_Type* port;
	uint8_t pin;
	direction_t direction;	//Input or Output
	pull_control_t pull_control;
	logic_t logic;
}gpio_pin_definition_t;


/*---------------Function prototypes-------------*/
/*------------------------------------------------------------------------------------
 * Initializes gpio pins
 * This function initializes all the pins defined in gpio_pinsh
 * Parameters: None
 *
 * Returns: None
 -------------------------------------------------------------------------------------- */
void init_gpio();

/*------------------------------------------------------------------------------------
 * Sets Led to one of the colors : Off, Blue, Green, Cyan, Red, Magenta, Yellow, White
 *
 * Parameters: led_color
 *
 * Returns: None
 ------------------------------------------------------------------------------------- */
void set_led(led_color_t);

/*------------------------------------------------------------------------------------
 * This function enables clock for those ports defined in gpio_pins.h
 *
 * Parameters: led_color
 *
 * Returns: None
 ------------------------------------------------------------------------------------- */
void gpio_clock_enable(void);
#endif /* GPIO_H_ */
