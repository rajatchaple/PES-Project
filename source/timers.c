/*******************************************************************************************************
 * @file timers.c :
 * @brief   : This file contains functionality of timer modules
 * 
 * @author  : Rajat Chaple (rajat.chaple@colorado.edu)
 * @date    : Apr 27, 2021
 *
 * @resource: 
 *******************************************************************************************************/

/*------------- Includes -----------*/
#include "MKL25Z4.h"
#include "log.h"
#include "gpio.h"
#include "timers.h"
/*--------------Defines-------------*/
#define LLWU_FROM_LLS
/*------------- Datatypes-----------*/

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

//    LPTMR0->CNR|= 0;	//needs not be set it seems

//    LPTMR0->CNR|= 400;	//needs not be set it seems

    LPTMR0->PSR |= LPTMR_PSR_PCS(1);	//0-MCGIRCLK, 1-LPO, 2_ERCLK32K, 3-OSCERCLK
    LPTMR0->PSR |= LPTMR_PSR_PBYP_MASK;  // LPO feeds directly to LPT;

    LPTMR0->CSR |= LPTMR_CSR_TCF_MASK;  //TODO may be : counter will reset when TCF is set
	

//#ifndef LLWU_FROM_LLS
	LPTMR0->CSR |= LPTMR_CSR_TIE_MASK;  //enabling timer interrupt

	//Enabling interrupt in NVIC
	NVIC_SetPriority(LPTMR0_IRQn, 2);
	NVIC_ClearPendingIRQ(LPTMR0_IRQn);
	NVIC_EnableIRQ(LPTMR0_IRQn);
//#endif

	LPTMR0->CSR |= LPTMR_CSR_TEN_MASK;  //enabling LP timer


}

bool led_status = false;
void LPTMR0_IRQHandler()
{
//	LPTMR0->CSR |= LPTMR_CSR_TCF_MASK;
	//LPTMR0->CSR = ( LPTMR_CSR_TEN_MASK | LPTMR_CSR_TIE_MASK | LPTMR_CSR_TCF_MASK);
#if 0
	if(led_status == false)
	{
		set_led(BLUE);
		led_status = true;
	}
	else
	{
		set_led(OFF);
		led_status = false;
	}
#endif

}


