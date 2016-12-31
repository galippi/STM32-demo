#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include "system_conf.h"

#define CPU_TYPE_STM32F0 0
#define CPU_TYPE_STM32F1 1
#define CPU_TYPE_STM32F4 2

#if !defined(TARGET_ECU)
#error "Error: TARGET ECU is not defined!"
#endif /* !defined(TARGET_ECU) */
#if TARGET_ECU == TARGET_ECU_STM32F0DISCOVERY

#define CPU_TYPE CPU_TYPE_STM32F0

#include "stm32f0xx.h"
#define CONTROLLER_BASE_NAME stm32f0xx
#define STM32_RCC_HEADER "stm32f0xx_rcc.h"
#define STM32_GPIO_HEADER "stm32f0xx_gpio.h"

#elif TARGET_ECU == TARGET_ECU_STM32F4DISCOVERY

#define CPU_TYPE CPU_TYPE_STM32F4

#include "stm32f4xx.h"
#define CONTROLLER_BASE_NAME "stm32f4xx"
#define STM32_RCC_HEADER "stm32f4xx_rcc.h"
#define STM32_ADC_HEADER "stm32f4xx_adc.h"
#define STM32_GPIO_HEADER "stm32f4xx_gpio.h"

#endif

//#define STM32_HEADER(ext) CONTROLLER_BASE_NAME##ext
//#define RCC_HEADER CONTROLLER_BASE_NAME ## "_rcc.h"
//#define RCC_HEADER_ STM32_HEADER("_rcc.h")
//#define RCC_HEADER STM32_HEADER("_rcc.h")
//#define RCC_HEADER RCC_HEADER_

#endif /* _CONTROLLER_H_ */
