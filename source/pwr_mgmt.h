/******************************************************************************************************
 * @file pwr_mgmt.h :
 * @brief    : This file contains defines includes and function prototypes for pwr_mgmt.c
 *
 * @author 	 : Rajat Chaple (rajat.chaple@colorado.edu)
 * @date 	 : May 1, 2021
 *
 * @resource :
 *******************************************************************************************************/
#ifndef PWR_MGMT_H_
#define PWR_MGMT_H_

/*---------------Includes-------------*/
#include <stdbool.h>

/*---------------Defines--------------*/
typedef enum allowed_wakeup_source{
	WU_EXT_PIN_PTD4_FILT = 0x01,
	WU_LPTIMER0 = 0x02
}allowed_wakeup_source_t;
/*-------------- Datatypes-------------*/

/*------------------------------------------------------------------------------------------------------
 * @brief :This function initializes pwr management module
 * @param : none
 * @return : none
 ------------------------------------------------------------------------------------------------------*/
void init_pwr_mgmt(void);

/*------------------------------------------------------------------------------------------------------
 * @brief :This function enters system from RUN mode to LLS mode
 *
 * @param : wakeup source : allowed to get back into RUN mode
 *
 * @return : none
 ------------------------------------------------------------------------------------------------------*/
void deep_sleep_lls(allowed_wakeup_source_t);


/*------------------------------------------------------------------------------------------------------
 * @brief :This function tells whether wakeup caused is for system accelerometer calibration
 *
 * @param : none
 *
 * @return : returns true if wakeup caused
 ------------------------------------------------------------------------------------------------------*/
bool is_wakeup_for_calibration(void);

#endif /* PWR_MGMT_H_ */
