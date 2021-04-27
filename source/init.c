/*******************************************************************************************************
 * @file init.c :
 * @brief   : This file contains initialization functions for modules.
 * 				TODO : List modules to be initialized
 * 
 * @author  : Rajat Chaple (rajat.chaple@colorado.edu)
 * @date    : Apr 25, 2021
 *
 * @resource: Embedded sytems fundamentals with ARM Cortex M... Ch 8, by Alexander Dean
 *******************************************************************************************************/

/*------------- Includes -----------*/


/*--------------Defines-------------*/


/*------------- Datatypes-----------*/


/*-------------------------------------------------------------------------------------------------------
 * This function initializes all the modules in this project
 -------------------------------------------------------------------------------------------------------*/
void init_system()
{
    init_i2c();
    //init_tpm();
}
