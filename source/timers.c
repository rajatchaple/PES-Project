/*******************************************************************************************************
 * @file timers.c :
 * @brief   : This file contains functionality of timer modules
 * 
 * @author  : Rajat Chaple (rajat.chaple@colorado.edu)
 * @date    : Apr 27, 2021
 *
 * @resource: This file was created by using KL25z Reference manual
 *******************************************************************************************************/

/*------------- Includes -----------*/
#include "MKL25Z4.h"
#include "log.h"
#include "gpio.h"
#include "timers.h"
#include "sysclock.h"
#include "pwr_mgmt.h"
/*------------- Datatypes-----------*/

#define SYSTICK_TIME_RESOLUTION_MS		(1)	//1 msec resolution
#define MILLISECONDS_IN_SECOND			(1000)
#define SYSTICK_REQUIRED_FREQUENCY 		(MILLISECONDS_IN_SECOND / SYSTICK_TIME_RESOLUTION_MS)
#define LOAD_VALUE_SYSTICK 				((SYSCLOCK_FREQUENCY / (SYSTICK_REQUIRED_FREQUENCY*16))-1)	//As counting starts from 0 and clock 3 MHz

/*-------------variables--------------*/


ticktime_t tick_time_since_startup = 0;
ticktime_t tick_time_reset_offset = 0;


/*-------------------------------------------------------------------------------------------------------
 * This function initializes low power timer module
 * (refer timers.h for more details)
 -------------------------------------------------------------------------------------------------------*/
void init_lptmr()
{
	SIM->SCGC5 |= SIM_SCGC5_LPTMR_MASK;
//	SIM->SOPT2 |= SIM_SOPT1_OSC32KSEL(3);	//selects LPO for LPTMR
	LPTMR0->CSR &= ~LPTMR_CSR_TEN_MASK;  //disabling LP timer before configuration
    
    LPTMR0->CMR = LPTMR_CMR_COMPARE(INDICATION_INTERVAL_MS - 1);	//add compare value

    LPTMR0->PSR |= LPTMR_PSR_PCS(1);	//0-MCGIRCLK, 1-LPO, 2_ERCLK32K, 3-OSCERCLK
    LPTMR0->PSR |= LPTMR_PSR_PBYP_MASK;  // LPO feeds directly to LPT;

    LPTMR0->CSR |= LPTMR_CSR_TCF_MASK;  //TODO may be : counter will reset when TCF is set
	

	LPTMR0->CSR |= LPTMR_CSR_TIE_MASK;  //enabling timer interrupt

	//Enabling interrupt in NVIC
	NVIC_SetPriority(LPTMR0_IRQn, 2);
	NVIC_ClearPendingIRQ(LPTMR0_IRQn);
	NVIC_EnableIRQ(LPTMR0_IRQn);


}

/*-------------------------------------------------------------------------------------------------------
 * This function initializes low power timer module
 * (refer timers.h for more details)
 -------------------------------------------------------------------------------------------------------*/
void enable_lptimer0()
{
	LPTMR0->CSR |= LPTMR_CSR_TEN_MASK;  //enabling LP timer

}

/*----------------------------------------------------------------------------
 * Initializes systick timer with 1 msec resolution
 * (refer timers.h for more details)
 ----------------------------------------------------------------------------*/
void init_systick()
{
	//Loading value for time SYSTICK_TIME_MS into Load register
	SysTick->LOAD = LOAD_VALUE_SYSTICK;
	SysTick->VAL = 0;

	//selecting clock source CLKSOURCE = 1(processor clock 48MHz) or CLKSOURCE = 0(external reference clock 3MHz)
	SysTick->CTRL &= ~SysTick_CTRL_CLKSOURCE_Msk;
	//TICKINT 1 will enable counting down to zero exception
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	//ENABLE field enables counter when set to one
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

/*----------------------------------------------------------------------------
 * Interrupt Handler for systick
 * (refer irq.h for more details)
 ----------------------------------------------------------------------------*/
void SysTick_Handler()
{
	//Entering Critical Section
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;

	tick_time_since_startup += SYSTICK_TIME_RESOLUTION_MS;

	//Exiting Critical Section
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

/*----------------------------------------------------------------------------
 * returns time since startup, in milliseconds
 * (refer timers.h for more details)
 ----------------------------------------------------------------------------*/
ticktime_t now()
{
	return tick_time_since_startup;
}

/*----------------------------------------------------------------------------
 * resets timer to 0; doesn't affect now() values
 * (refer timers.h for more details)
 ----------------------------------------------------------------------------*/
void reset_timer()
{
	tick_time_reset_offset = tick_time_since_startup;
}

/*----------------------------------------------------------------------------
 * returns ticks since the last call to reset_timer()
 * (refer timers.h for more details)
 ----------------------------------------------------------------------------*/
ticktime_t get_timer()
{
	return (tick_time_since_startup - tick_time_reset_offset);
}

/*----------------------------------------------------------------------------
 * blocking delay function
 * (refer timers.h for more details)
 ----------------------------------------------------------------------------*/
void delay(ticktime_t time_ms)
{
	static bool ref_time_set = false;
	static ticktime_t ref_time;

	if(ref_time_set == false)
	{
		ref_time = get_timer();
		ref_time_set = true;
	}
	while(get_timer() < (ref_time + time_ms))
	{
		__asm volatile ("nop");
	}
	ref_time_set = false;	//ready for next delay call
}

/*----------------------------------------------------------------------------
 * blocking delay function which puts system into lower power consumption
 * (refer timers.h for more details)
 ----------------------------------------------------------------------------*/
ticktime_t delay_low_power(ticktime_t time_ms)
{
	LPTMR0->CSR &= ~LPTMR_CSR_TEN_MASK;  //disabling LP timer before configuration
	LPTMR0->CMR = LPTMR_CMR_COMPARE(time_ms - 2); //ideally count should be T-1. But 1 ms is covered below to stabilize system

	deep_sleep_lls(WU_LPTIMER0);
	delay(1);	//allow system to stabilize
	return time_ms;
}

/*----------------------------------------------------------------------------
 * IRQ handler for LPTIMER
 ----------------------------------------------------------------------------*/
void LPTMR0_IRQHandler()
{
	//Do nothing
}



