/*******************************************************************************************************
 * @file pwr_mgmt.c :
 * @brief   : This file contains 
 * 
 * @author  : Rajat Chaple (rajat.chaple@colorado.edu)
 * @date    : May 1, 2021
 *
 * @resource: 
 *******************************************************************************************************/

/*------------- Includes -----------*/
#include <stdbool.h>
#include "MKL25Z4.h"
#include "gpio.h"
#include "timers.h"
#include "i2c.h"

/*--------------Defines-------------*/

/*------------- Datatypes-----------*/


static bool timed_wakeup_for_indication = false;
static uint16_t timer_for_tilt_measurement = 0;
/*-------------------------------------------------------------------------------------------------------
 * This function initializes power management_module
 * (refer pwr_mgmt.h for more details)
 -------------------------------------------------------------------------------------------------------*/
void init_pwr_mgmt()
{
#if 1
//	SMC->PMPROT |= SMC_PMPROT_ALLS_MASK;


	//Enablig module for LLWU
	LLWU->ME |= LLWU_ME_WUME0_MASK;	//using LPTMR0 as LLWU

	//Enabling interrupt in NVIC
//	NVIC_SetPriority(LLWU_IRQn, 2);
//	NVIC_ClearPendingIRQ(LLWU_IRQn);
	NVIC_EnableIRQ(LLWU_IRQn);
#endif

	//Enablig module for LLWU
//		LLWU->ME |= LLWU_ME_WUME0_MASK;	//using LPTMR0 as LLWU


//	    NVIC_EnableIRQ(LLW_IRQn);

//	    volatile unsigned int dummyread;
//
//	      /* The PMPROT register allows the MCU to enter the LLS modes.*/
//	      SMC->PMPROT = SMC_PMPROT_ALLS_MASK;
//
//	      /* Set the STOPM field to 0b011 for LLS mode */
//	      SMC->PMCTRL &= ~SMC_PMCTRL_STOPM_MASK;
//	      SMC->PMCTRL |=  SMC_PMCTRL_STOPM(0x3);
//
//	      /*wait for write to complete to SMC before stopping core */
//	      dummyread = SMC->PMCTRL;
//	      dummyread++;
//
//	      /* Set the SLEEPDEEP bit to enable deep sleep mode (STOP) */
//	      SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
//
//	      __WFI();
}

/*-------------------------------------------------------------------------------------------------------
 * This function
 * (refer pwr_mgmt.h for more details)
 -------------------------------------------------------------------------------------------------------*/
void run_to_lls_pwr_mode()
{
	volatile unsigned int dummyread;
	SMC->PMPROT = SMC_PMPROT_ALLS_MASK;	//allow deep sleep upto Low leakage stop

	//disabling accelerometer
	uint8_t data = 0x00;
		i2c_write(0x1D, 0x2A, &data,  1);

//	LPTMR0->CSR |= LPTMR_CSR_TCF_MASK;
	//selecting LLS mode
	SMC->PMCTRL &= ~SMC_PMCTRL_STOPM_MASK;
	SMC->PMCTRL |= SMC_PMCTRL_STOPM(3);

	dummyread = SMC->PMCTRL;
	dummyread++;

	LPTMR0->CSR |= LPTMR_CSR_TEN_MASK;

//	SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk; //
	SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;	//deep sleep
	dummyread = SMC->PMCTRL;
		dummyread++;
		dummyread = SMC->PMCTRL;
	__WFI();

	dummyread = SMC->PMCTRL;

	dummyread = SMC->PMCTRL;
	dummyread = SMC->PMCTRL;
}

/*-------------------------------------------------------------------------------------------------------
 * This function is an IRQ handler for Low-Leakage wakeup
 * (refer pwr_mgmt.h for more details)
 -------------------------------------------------------------------------------------------------------*/
void LLWU_IRQHandler()
{
	//clearing flag (for internal modules internal module's flag neads to be cleared)
	LPTMR0->CSR &= ~LPTMR_CSR_TEN_MASK;  //disabling timer interrupt
	LPTMR0->CSR |= LPTMR_CSR_TCF_MASK;

	timer_for_tilt_measurement += INDICATION_INTERVAL_MS;
	timed_wakeup_for_indication = true;
}

/*-------------------------------------------------------------------------------------------------------
 * This function returns system's status if it transitioned from lls (deep sleep) to run mode
 * (refer pwr_mgmt.h for more details)
 -------------------------------------------------------------------------------------------------------*/
bool is_wakeup_for_indication()
{	//entering critical section as this variables value is read by external module
	uint32_t mask_state = __get_PRIMASK();
	__disable_irq();

	bool ret = timed_wakeup_for_indication;
	timed_wakeup_for_indication = false;

	__set_PRIMASK(mask_state);
	return ret;
}

/*-------------------------------------------------------------------------------------------------------
 * This function returns system's status if it transitioned from lls (deep sleep) to run mode
 * (refer pwr_mgmt.h for more details)
 -------------------------------------------------------------------------------------------------------*/
bool is_wakeup_for_tilt_measurement()
{	//entering critical section as this variables value is read by external module
	bool ret = false;

	//Entering critical section
	uint32_t mask_state = __get_PRIMASK();
	__disable_irq();

	if(timer_for_tilt_measurement >= TILT_MEASUREMENT_INTERVAL)
	{
		timer_for_tilt_measurement = 0;
		ret = true;
	}

	__set_PRIMASK(mask_state);

	//enabling accelerometer
	uint8_t data = 0x03;	//REG_CTRL1 : setting active mode, 8 bit samples and 800 Hz ODR (Output data rate)
	i2c_write(0x1D, 0x2A, &data,  1);


	return ret;
}

