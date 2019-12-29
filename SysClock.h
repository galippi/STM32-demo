#ifndef _SYSCLOCK_H_
#define _SYSCLOCK_H_

#include "controller.h"

#define f_LSI_Hz 40000
#define f_HSI_Hz 8000000

#include "SysClock_conf.h"

#if f_APB1_Hz > 36000000
#error f_APB1_Hz is wrongly set!
#endif

#if f_APB2_Hz > 72000000
#error f_APB2_Hz is wrongly set!
#endif

#if f_USBCLK_Hz != 48000000
#error f_USBCLK_Hz is wrongly set!
#endif

#ifndef HSI_ON
  #define HSI_ON 0
#endif
#ifndef HSE_ON
  #define HSE_ON 0
#endif
#if !defined(HSE_BYP)
  #define HSE_BYP 0
#endif
#ifndef PLL_ON
  #define PLL_ON 0
#endif

#if (HSE_ON != 0)
  #if (HSE_BYP != 0)
    #define f_HSE_MIN_HZ 0
    #define f_HSE_MAX_HZ 25000000
  #else
    #define f_HSE_MIN_HZ  4000000
    #define f_HSE_MAX_HZ 16000000
  #endif
  #if (f_HSE_Hz < f_HSE_MIN_HZ) || (f_HSE_Hz > f_HSE_MAX_HZ)
    #error f_HSE_Hz or HSEBYP is wrongly set
  #endif
#endif

#if PLLSRC != 0
  #if (HSE_ON == 0)
    #error HSE_ON is worngly set!
  #endif
  #if PLLXTPRE_REG != 0
    #define F_PLL_INPUT_Hz (f_HSE_Hz / 2)
  #else
    #define F_PLL_INPUT_Hz (f_HSE_Hz)
  #endif
    #define f_PLL_MAX_HZ 72000000
#else
    #if HSI_ON != 0
    #error HSI_ON is worngly set!
    #endif
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
#elif SWS == 1 && (HSE_ON != 0)
  #define f_SYSCLK_Hz f_HSE_Hz
#elif SWS == 2 && (PLL_ON != 0)
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

#if f_ADC_Hz > 14000000
#error ADCPRE_REG is wrongly set!
#endif

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

void SysClock_Init(void);

#endif /* _SYSCLOCK_H_ */
