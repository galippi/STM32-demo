#ifndef _SYSCLOCK_CONF_H_
#define _SYSCLOCK_CONF_H_

#include "FaultHandler.h"

#define HSIDIV_REG 1 // div by 2

#define f_HSE_Hz 4000000
//#define f_LSE_Hz 32768
#define f_VCO_Hz 96000000
#define f_SYSCLK_Hz 8000000
//#define f_HCLK_Hz 8000000
//#define f_PCLK_Hz 8000000
//#define f_TIMPCLK_Hz 8000000
#define f_HCLK_Hz 8000000
#define f_PCLK_Hz 8000000
//#define f_APB2_Hz 72000000 /* PCLK2 */
//#define f_TIMXCLK_Hz 72000000
//#define f_TIM1CLK_Hz 72000000
#define f_ADCCLK_Hz 12000000
#define f_RTCCLK_Hz 32768

#define HSI_ON 1
#define HSI48_ON 1
#define HSE_ON (defined(f_HSE_Hz) ? 1 : 0)
#define HSE_BYP 0

//#define PLL_ON 1
#define PLLM_VAL 4
#define PLLSRC   PLLSRC_HSI16
#define PLLN_VAL 24

#define PLLR_VAL 12

#define SWS RCC_CFGR_SW_PLLRCLK

#define HPRE_REG  0 // AHB prescaler
#define PPRE_REG  0 /* APB prescaler */
#define ADCPRE_REG 2

#define f_USBCLK_Hz 48000000
#define USBPRE_REG 1
#define CLOCK_USBSEL RCC_CCIPR2_USBSEL_HSI48

#define LATENCY_VAL 3
#define PRFTBE_REG 1
#define HLFCYA_REG 0
#define FLASH_ACR (FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY)

#define HSI_STARTUP_TIMEOUT 5000
#define HSI48_STARTUP_TIMEOUT 5000
#define HSE_STARTUP_TIMEOUT 5000

#define HSI_STARTUP_DEBUG(x) /* do nothing */
#define HSE_STARTUP_DEBUG(x) /* do nothing */
#define PLL_STOP_DEBUG(x) /* do nothing */
#define PLL_STARTUP_DEBUG(x) /* do nothing */
#define SYSCLK_SET_DEBUG(x) /* do nothing */


#define HSI_STARTUP_ERROR() CAT_Error(CAT_HSI_Init, 0)
#define HSI48_STARTUP_ERROR(data) CAT_Error(CAT_HSI_Init, 1)
/* If HSE fails to start-up, the application will have wrong clock
   configuration. User can add here some code to deal with this error */
#if 0
#define HSE_STARTUP_ERROR(data) CAT_Error(CAT_HSE_Init, data)
#else
#include "debug.h"
#define HSE_STARTUP_ERROR(data) DBG_SET(clockHseInitError, 1)
#endif

#define SYSTICK_RELOAD_VAL 1000
#define SYSTICK_CTRL_VAL   SysTick_CTRL_CLKSOURCE_HCLK8 | \
                         /*SysTick_CTRL_TICKINT_Msk   | */ \
                           SysTick_CTRL_ENABLE_Msk

#endif /* _SYSCLOCK_CONF_H_ */
