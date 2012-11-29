/******************************************************************************
 *
 * \file
 * \ingroup DAC
 * \brief   Config file of DAC subsystem
 *
 * \details Fill me
 *
 ******************************************************************************/
/** \addtogroup DAC
 *  @{
 ******************************************************************************/

#ifndef _DAC_CONF_H_
#define _DAC_CONF_H_

#include "controller.h"

#define DAC_1_ENABLED 1

#if TARGET_ECU == TARGET_ECU_STM32F4DISCOVERY
#define DAC_2_ENABLED 1
#endif

#endif /* _DAC_CONF_H_ */
