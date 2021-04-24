/***************************************************************************************
 * @file log.h :
 * @brief : include this file whenever you want to enable logging over UART or Semihost
 *
 * @author : Rajat Chaple (rajat.chaple@colorado.edu)
 * @date : Feb 21 2021
 *
 ****************************************************************************************/

#ifndef LOG_H_
#define LOG_H_

//Build configuration manages this #def
#include "fsl_debug_console.h"
#ifdef DEBUG
#define LOG PRINTF
#else
#define LOG(...)
#endif


#endif /* LOG_H_ */
