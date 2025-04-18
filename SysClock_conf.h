#ifndef _SYSCLOCK_CONF_H_
#define _SYSCLOCK_CONF_H_

#include "FaultHandler.h"

#define HSIDIV_REG 1

//#define f_HSE_Hz 8000000
//#define f_LSE_Hz 32768
//#define f_PLL_Hz 48000000
#define f_HCLK_Hz 8000000
#define f_PCLK_Hz 8000000
#define f_TIMPCLK_Hz 8000000
#define f_HCLK_Hz 8000000
#define f_PCLK_Hz 8000000 /* PCLK1 */
//#define f_APB2_Hz 72000000 /* PCLK2 */
//#define f_TIMXCLK_Hz 72000000
//#define f_TIM1CLK_Hz 72000000
#define f_ADCCLK_Hz 12000000
#define f_RTCCLK_Hz 32768

#define HSI_ON 1
//#define HSE_ON 1
#define HSE_BYP 0
//#define PLL_ON 1

#define PLLXTPRE_REG 0
//#define PLLSRC   PLLSRC_HSE
#define PLLMUL_VAL 6
//#define SWS 2 /* PLL */
//#define SWS RCC_CFGR_SW_PLLRCLK

#define AHB_PRESC 1
#define APB_PRESC 1

#define SWS RCC_CFGR_SW_HSISYS

#define HPRE_REG  0
#define PPRE_REG  0 /* APB prescaler */
#define ADCPRE_REG 2

//#define f_USBCLK_Hz 48000000
#define USBPRE_REG 1
#define CLOCK_USBSEL RCC_CCIPR2_USBSEL_PLL

#define LATENCY_VAL 3
#define PRFTBE_REG 1
#define HLFCYA_REG 0
#define FLASH_ACR (FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY)

#define HSI_STARTUP_TIMEOUT 5000
#define HSE_STARTUP_TIMEOUT 5000

#define HSI_STARTUP_DEBUG(x) /* do nothing */
#define HSE_STARTUP_DEBUG(x) /* do nothing */
#define PLL_STARTUP_DEBUG(x) /* do nothing */
#define SYSCLK_SET_DEBUG(x) /* do nothing */


#define HSI_STARTUP_ERROR() CAT_Error(CAT_HSI_Init, 0)
/* If HSE fails to start-up, the application will have wrong clock
   configuration. User can add here some code to deal with this error */
#define HSE_STARTUP_ERROR(data) CAT_Error(CAT_HSE_Init, data)

#endif /* _SYSCLOCK_CONF_H_ */
