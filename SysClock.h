#ifndef _SYSCLOCK_H_
#define _SYSCLOCK_H_

#include "controller.h"

#define f_LSI_Hz 40000
#define f_HSI_Hz 8000000

#include "SysClock_conf.h"

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

#if PPRE1_REG < 4
  #define PPRE1_VAL 1
#elif PPRE1_REG < 8
  #define PPRE1_VAL (2 << (PPRE1_REG - 4))
#else
#error PPRE1_REG is wrongly set!
#endif

#define f_APB1_CALC_Hz (f_AHB_Hz / PPRE1_VAL)

#if PPRE1_VAL == 1
  #define f_TIMXCLK_CALC_Hz f_APB1_Hz
#else
  #define f_TIMXCLK_CALC_Hz (f_APB1_Hz * 2)
#endif

#if PPRE2_REG < 4
  #define PPRE2_VAL 1
#elif PPRE2_REG < 8
  #define PPRE2_VAL (2 << (PPRE2_REG - 4))
#else
#error PPRE2_REG is wrongly set!
#endif

#define f_APB2_CALC_Hz (f_AHB_Hz / PPRE2_VAL)

#if PPRE2_VAL == 1
  #define f_TIM1CLK_CALC_Hz f_APB2_Hz
#else
  #define f_TIM1CLK_CALC_Hz (f_APB2_Hz * 2)
#endif

#if (ADCPRE_REG >= 0) && (ADCPRE_REG <= 3)
#define ADCPRE_VAL (2 + (ADCPRE_REG * 2))
#else
#error ADCPRE_REG is wrongly set!
#endif

#define f_ADC_Hz (f_APB2_Hz / ADCPRE_VAL)

#if USBPRE_REG == 0
  #define USBPRE_VAL 2 / 3
#elif USBPRE_REG == 1
  #define USBPRE_VAL 1
#else
#error USBPRE_REG is wrongly set!
#endif

#define f_USB_Hz (f_PLL_Hz * USBPRE_VAL)

void SysClock_Init(void);

#endif /* _SYSCLOCK_H_ */
