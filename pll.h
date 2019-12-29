#ifndef _PLL_H_
#define _PLL_H_

//#include <stdint.h>

#define STM32F10X_MD_VL
#include "stm32f10x.h"

#define f_LSI_Hz 40000
#define f_HSI_Hz 8000000

#include "pll_conf.h"

#if f_PLL_Hz > 72000000
#error f_PLL_Hz is wrongly set!
#endif

#if f_APB1_Hz > 36000000
#error f_APB1_Hz is wrongly set!
#endif

#if f_APB2_Hz > 72000000
#error f_APB2_Hz is wrongly set!
#endif

#if f_USBCLK_Hz != 48000000
#error f_USBCLK_Hz is wrongly set!
#endif

#if PLLSRC != 0
  #if PLLXTPRE_REG != 0
    #define F_PLL_INPUT_Hz (f_HSE_Hz / 2)
  #else
    #define F_PLL_INPUT_Hz (f_HSE_Hz)
  #endif
    #define f_PLL_MAX_HZ 72000000
#else
    #define F_PLL_INPUT_Hz (f_HSI_Hz / 2)
    #define f_PLL_MAX_HZ 64000000
#endif

#if (PLLMUL_VAL < 2) || (PLLMUL_VAL > 16)
#error PLLMUL_VAL is wrongly set!
#endif

#define f_PLL_CALC_Hz (F_PLL_INPUT_Hz * PLLMUL_VAL)
#if f_PLL_CALC_Hz != f_PLL_Hz
#error PLL_CALC_Hz is wrong!
#endif
#if f_PLL_Hz > f_PLL_MAX_HZ
#error f_PLL_Hz is wrongly set!
#endif

#if SWS == 0
  #define f_SYSCLK_Hz f_HSI_Hz
#elif SWS == 1
  #define f_SYSCLK_Hz f_HSE_Hz
#elif SWS == 2
  #define f_SYSCLK_Hz f_PLL_Hz
#else
#error SWS is wrongly set!
#endif

#if HPRE_REG < 0x08
  #define HPRE_VAL 1
#elif HPRE_REG <= 15
  #define HPRE_VAL (2 << (HPRE_REG - 8))
#else
#error HPRE_REG is wrongly set!
#endif

#define f_AHB_CALC_Hz (f_SYSCLK_Hz / HPRE_VAL)
#if f_AHB_CALC_Hz != f_AHB_Hz
#error f_AHB_CALC_Hz is wrong!
#endif

#if PPRE1_REG < 4
  #define PPRE1_VAL 1
#elif PPRE1_REG < 8
  #define PPRE1_VAL (2 << (PPRE1_REG - 4))
#else
#error PPRE1_REG is wrongly set!
#endif

#define f_APB1_CALC_Hz (f_AHB_Hz / PPRE1_VAL)
#if f_APB1_CALC_Hz != f_APB1_Hz
#error f_APB1_CALC_Hz is wrong!
#endif

#if PPRE1_VAL == 1
  #define f_TIMXCLK_CALC_Hz f_APB1_Hz
#else
  #define f_TIMXCLK_CALC_Hz (f_APB1_Hz * 2)
#endif

#if f_TIMXCLK_CALC_Hz != f_TIMXCLK_Hz
#error f_TIMXCLK_CALC_Hz is wrong!
#endif

#if PPRE2_REG < 4
  #define PPRE2_VAL 1
#elif PPRE2_REG < 8
  #define PPRE2_VAL (2 << (PPRE2_REG - 4))
#else
#error PPRE2_REG is wrongly set!
#endif

#define f_APB2_CALC_Hz (f_AHB_Hz / PPRE2_VAL)
#if f_APB2_CALC_Hz != f_APB2_Hz
#error f_APB2_CALC_Hz is wrong!
#endif

#if PPRE2_VAL == 1
  #define f_TIM1CLK_CALC_Hz f_APB2_Hz
#else
  #define f_TIM1CLK_CALC_Hz (f_APB2_Hz * 2)
#endif

#if f_TIM1CLK_CALC_Hz != f_TIM1CLK_Hz
#error f_TIM1CLK_CALC_Hz is wrong!
#endif

#if (ADCPRE_REG >= 0) && (ADCPRE_REG <= 3)
#define ADCPRE_VAL (2 + (ADCPRE_REG * 2))
#else
#error ADCPRE_REG is wrongly set!
#endif

#define f_ADC_Hz (f_APB2_Hz / ADCPRE_VAL)

#if USBPRE_REG == 0
  #define USBPRE_VAL 3 / 2
#elif USBPRE_REG == 1
  #define USBPRE_VAL 1
#else
#error USBPRE_REG is wrongly set!
#endif

#define f_USB_Hz (f_PLL_Hz * USBPRE_VAL)

#define f_FLASH_MIN_Hz ((LATENCY_VAL    ) * 24000000)
#define f_FLASH_MAX_Hz ((LATENCY_VAL + 1) * 24000000)
#if (f_SYSCLK_Hz < f_FLASH_MIN_Hz) || (f_SYSCLK_Hz > f_FLASH_MAX_Hz)
#error LATENCY_VAL is wrongly set!
#endif

#if (HPRE_VAL != 1) && (PRFTBE_REG != 1)
#define PRFTBE_REG is wrongly set!
#endif

#if (HPRE_VAL != 1) && (HLFCYA_REG != 0)
#define HLFCYA_REG is wrongly set!
#endif

#endif /* _PLL_H_ */
