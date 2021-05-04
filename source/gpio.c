/***************************************************************************************
 * @file gpio.c :
 * @brief : This file contains gpio initializations and gpio drive/read functions
 *
 * @author : Rajat Chaple (rajat.chaple@colorado.edu)
 * @date : Feb 21 2021
 *
 ****************************************************************************************/

/*---------Includes-----------*/
#include <stdbool.h>
#include "gpio.h"
#include "gpio_pins.h"
#include "timers.h"

/*----------------------------------------------------------------------------
 * Initializes pins to Inputs and outputs as defined in init_gpio.h
 * (refer gpio.h for more details)
 ----------------------------------------------------------------------------*/
 void init_gpio()
 {

	 //clock enable : Enable clocks for those ports mentioned in gpio_pins[]
	 gpio_clock_enable();

	 int num_of_pin_definitions = sizeof(gpio_pins) / sizeof(gpio_pin_definition_t);

	 //Configuration for each entry in gpio_pins[]
	 for(int i = 0; i < num_of_pin_definitions; i++)
	 {

		 //using pin as GPIO. Settubg up MUX in PCR register
		 gpio_pins[i].port->PCR[gpio_pins[i].pin] &= ~PORT_PCR_MUX_MASK;
		 gpio_pins[i].port->PCR[gpio_pins[i].pin] |= PORT_PCR_MUX(1);	//1 for GPIO


		 /*Clearing data in a register in case of active low
		   If pin is active low, it would SET the output as soon as its direction is changed to output.
		   Reason for this is default value in the register is 0. This needs to be changed to 1 for active low circuitry.
		    e.g. setting direction to Output without setting relevant bit would turn LED on if its Active low*/
		 if((gpio_pins[i].logic == active_low) && (gpio_pins[i].direction == output))
		 {
			 gpio_pins[i].gpio->PSOR |= MASK(gpio_pins[i].pin);
		 }
		 else if((gpio_pins[i].logic == active_high) && (gpio_pins[i].direction == output))
		 {
			 gpio_pins[i].gpio->PCOR |= MASK(gpio_pins[i].pin);
		 }


		 //If pin is to be set up as Input, set up its pull up / down configuration
		 if((gpio_pins[i].direction == input) && (gpio_pins[i].pull_control != NA))
		 {
			 gpio_pins[i].port->PCR[gpio_pins[i].pin] |= (uint32_t)MASK(0);	//setting 0th bit would enable Pull resister (PE)
			 if(gpio_pins[i].pull_control == pull_up_enable)
			 {
				 //set 1st bit in PCR i.e. PS
				 gpio_pins[i].port->PCR[gpio_pins[i].pin] |= (uint32_t)MASK(1);
			 }
			 else if(gpio_pins[i].pull_control == pull_down_enable)
			 {
				 //clear 1st bit in PCR i.e. PS
				 gpio_pins[i].port->PCR[gpio_pins[i].pin] &= (uint32_t)(~MASK(1));
			 }
		 }

		 //Set Gpio direction as Input or Output
		 if(gpio_pins[i].direction == output)
		 {
			 //setting relevant bit to set pin as output
			 gpio_pins[i].gpio->PDDR |= MASK(gpio_pins[i].pin);
		 }
		 else if(gpio_pins[i].direction == input)
		 {
			 //clearing relevant bit to set pin as input
			 gpio_pins[i].gpio->PDDR &= (uint32_t)(~MASK(gpio_pins[i].pin));
		 }


	 }

 }

/*----------------------------------------------------------------------------
 * Sets Led color to passed parameter
 * (refer gpio.h for more details)
 ----------------------------------------------------------------------------*/
void set_led(led_color_t color)
{
	if(color == YELLOW_EXTERNAL_LED1)	//for external LED1 yellow
	{
		GPIOA->PSOR |= MASK(YELLOW_LED_PIN);	//Turns external Yellow LED on
	}
	else if(color == OFF_EXTERNAL_LED1)
	{
		GPIOA->PCOR |= MASK(YELLOW_LED_PIN);	//Turns external Yellow LED off
	}
	else	//for onboard tricolor LED
	{
		if(color & 0b001)	//Checking if Blue LED needs to be turned or
		{
			GPIOD->PCOR |= MASK(BLUE_LED_PIN);	//Turns Blue LED on
		}
		else
		{
			GPIOD->PSOR |= MASK(BLUE_LED_PIN);	//Turns Blue LED off
		}

		if(color & 0b010)	//Checking if Green LED needs to be turned or
		{
			GPIOB->PCOR |= MASK(GREEN_LED_PIN);	//Turns Green LED on
		}
		else
		{
			GPIOB->PSOR |= MASK(GREEN_LED_PIN);	//Turns Green LED off
		}

		if(color & 0b100)	//Checking if Red LED needs to be turned or
		{
			GPIOB->PCOR |= MASK(RED_LED_PIN);	//Turns Red LED on
		}
		else
		{
			GPIOB->PSOR |= MASK(RED_LED_PIN);	//Turns Red LED off
		}
	}

}

/*----------------------------------------------------------------------------
 * Executes LED_sequence defined in gpio.h
 * (refer gpio.h for more details)
 ----------------------------------------------------------------------------*/
void led_blink_pattern(led_pattern_t* pattern, uint8_t iterations)
{
	for(int i=0; i<iterations; i++)
	{
		set_led(pattern[i].color);
		delay(pattern[i].delay);
	}
}

/*----------------------------------------------------------------------------
 * Returns test switch status
 * (refer gpio.h for more details)
 ----------------------------------------------------------------------------*/
bool read_test_switch()
{
	return(!(GPIOA->PDIR & MASK(TEST_SWITCH)));	//Active low switch
}

/*----------------------------------------------------------------------------
 * Enable clocks for ports defined in gpio_pins.h
 * (refer gpio.h for more details)
 ----------------------------------------------------------------------------*/
void gpio_clock_enable()
{
	int num_of_pin_definitions = sizeof(gpio_pins) / sizeof(gpio_pin_definition_t);
//For each entry in table gpio_pins[] turning corresponding clock ON
	for(int i = 0; i < num_of_pin_definitions; i++)
	 {
		if((gpio_pins[i].port == PORTA) && !(SIM->SCGC5 & SIM_SCGC5_PORTA_MASK))
			 {
				 SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
			 }
		 else if((gpio_pins[i].port == PORTB) && !(SIM->SCGC5 & SIM_SCGC5_PORTB_MASK))
			 {
				 SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
			 }
		 else if((gpio_pins[i].port == PORTC) && !(SIM->SCGC5 & SIM_SCGC5_PORTC_MASK))
			 {
				 SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
			 }
		 else if((gpio_pins[i].port == PORTD) && !(SIM->SCGC5 & SIM_SCGC5_PORTD_MASK))
			 {
				 SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
			 }
		 else if((gpio_pins[i].port == PORTE) && !(SIM->SCGC5 & SIM_SCGC5_PORTE_MASK))
			 {
				 SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
			 }
	 }

}

/*----------------------------------------------------------------------------
 * Enable clocks for ports defined in gpio_pins.h
 * (refer gpio.h for more details)
 ----------------------------------------------------------------------------*/
void gpio_clock_disable()
{
				 SIM->SCGC5 &= ~SIM_SCGC5_PORTA_MASK;
				 SIM->SCGC5 &= ~SIM_SCGC5_PORTB_MASK;
				 SIM->SCGC5 &= ~SIM_SCGC5_PORTC_MASK;
				 SIM->SCGC5 &= ~SIM_SCGC5_PORTD_MASK;
				 SIM->SCGC5 &= ~SIM_SCGC5_PORTE_MASK;
}

