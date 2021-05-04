/*******************************************************************************************************
 * @file pwr_mgmt.c :
 * @brief   : This file contains power management modules,i.e., sleep and wakeup routines
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
#include "pwr_mgmt.h"

/*------------- Datatypes-----------*/

static bool wakeup_for_calibration = false;
/*-------------------------------------------------------------------------------------------------------
 * This function initializes power management_module
 * (refer pwr_mgmt.h for more details)
 -------------------------------------------------------------------------------------------------------*/
void init_pwr_mgmt()
{
	SMC->PMPROT = SMC_PMPROT_ALLS_MASK;	//allow deep sleep upto Low leakage stop

	//selecting LLS mode
	SMC->PMCTRL &= ~SMC_PMCTRL_STOPM_MASK;
	SMC->PMCTRL |= SMC_PMCTRL_STOPM(3);

	//Enabling interrupt in NVIC
	NVIC_SetPriority(LLWU_IRQn, 2);
	NVIC_ClearPendingIRQ(LLWU_IRQn);
	NVIC_EnableIRQ(LLWU_IRQn);

}

/*-------------------------------------------------------------------------------------------------------
 * This function
 * (refer pwr_mgmt.h for more details)
 -------------------------------------------------------------------------------------------------------*/
void deep_sleep_lls(allowed_wakeup_source_t allowed_wakeup_source)
{
	volatile unsigned int dummy;

	if(allowed_wakeup_source & WU_EXT_PIN_PTD4_FILT)
	{
		LLWU->PE4 &= ~LLWU_PE4_WUPE14_MASK;	//disabling PD4 as a wakeup pin (falling and rising edge)
		LLWU->FILT1 &= ~LLWU_FILT1_FILTSEL_MASK;
		LLWU->FILT1 |= LLWU_FILT1_FILTSEL(14);
		LLWU->FILT1 |= LLWU_FILT1_FILTE(2);	//enabling on negative edge

		LLWU->FILT1 |= LLWU_FILT1_FILTF_MASK;
	}
	else
	{
		LLWU->FILT1 &= ~LLWU_FILT1_FILTE_MASK; //Disabling digital filter
	}

	if(allowed_wakeup_source & WU_LPTIMER0)	//if wakeup source is LPTIMER
	{
		LLWU->ME |= LLWU_ME_WUME0_MASK;	//using LPTMR0 as LLWU
		LPTMR0->CSR |= LPTMR_CSR_TCF_MASK;
		LPTMR0->CSR |= LPTMR_CSR_TEN_MASK;
	}
	else
	{
		LPTMR0->CSR &= ~LPTMR_CSR_TEN_MASK;
		LLWU->ME &= ~LLWU_ME_WUME0_MASK;	//using LPTMR0 as LLWU
	}

	SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;	//deep sleep
	dummy = SMC->PMCTRL;	//dummy read helps executing instruction prior to going in deep sleep

	__WFI();

	dummy = SMC->PMCTRL;
	(void)dummy;	//for removing unused warning
}

/*-------------------------------------------------------------------------------------------------------
 * This function is an IRQ handler for Low-Leakage wakeup
 * (refer pwr_mgmt.h for more details)
 -------------------------------------------------------------------------------------------------------*/
void LLWU_IRQHandler()
{
	//checking if wake up caused by internal module LPTIMER0
	if(LLWU->F3 & LLWU_F3_MWUF0_MASK)
	{
		//clearing flag (for internal modules internal module's flag neads to be cleared)
		LPTMR0->CSR |= LPTMR_CSR_TCF_MASK;
	}

	//checking if wake up caused by calibration switch
	if(LLWU->FILT1 & LLWU_FILT1_FILTF_MASK)
	{
		LLWU->FILT1 |= LLWU_FILT1_FILTF_MASK;
		wakeup_for_calibration = true;
	}

}

/*-------------------------------------------------------------------------------------------------------
 * This function returns system's status if it transitioned from lls (deep sleep) to run mode for calibration
 * (refer pwr_mgmt.h for more details)
 -------------------------------------------------------------------------------------------------------*/
bool is_wakeup_for_calibration()
{
	//Entering critical section
	uint32_t mask_state = __get_PRIMASK();
	__disable_irq();

	bool ret = wakeup_for_calibration;
	wakeup_for_calibration = false;

	__set_PRIMASK(mask_state);

	return ret;
}
