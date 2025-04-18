#ifndef _SYSCLOCK_H_
#define _SYSCLOCK_H_

#include "controller.h"

#define f_LSI_Hz 40000
#define f_HSI_Hz 16000000

#include "SysClock_conf.h"

#ifndef HSI_ON
  #define HSI_ON 0
#endif

#define HSIDIV_VAL (1 << HSIDIV_REG)
#define F_HSISYS_Hz ((f_HSI_Hz) / (HSIDIV_VAL))

#ifndef HSE_ON
  #define HSE_ON 0
#endif
#if (defined(f_HSE_Hz) && (HSE_ON == 0)) || (!defined(f_HSE_Hz) && (HSE_ON != 0))
  #error HSE config error!
#endif

#if (defined(f_HSE_Hz) && ((f_HSE_Hz < 4000000) || (f_HSE_Hz > 48000000)))
  #error HSE value config error!
#endif

#if !defined(HSE_BYP)
  #define HSE_BYP 0
#endif

#ifndef PLL_ON
  #ifdef f_PLL_Hz
    #define PLL_ON 1
  #else
    #define PLL_ON 0
  #endif
#endif

#define PLLSRC_NONE 0
//#define PLLSRC_RESERVED 1
#define PLLSRC_HSI16 2
#define PLLSRC_HSE 3

#ifndef PLLSRC
#define PLLSRC PLLSRC_NONE
#endif

#if (PLLSRC == PLLSRC_HSE)
  #if (HSE_ON == 0)
    #error HSE_ON is worngly set!
  #endif
  #define f_PLL_INPUT_Hz (f_HSE_Hz / (PLLM_VAL))
#elif (PLLSRC == PLLSRC_HSI16)
    #if HSI_ON != 1
    #error HSI_ON is worngly set!
    #endif
    #define f_PLL_INPUT_Hz ((f_HSI_Hz) / (PLLM_VAL))
#else
#error The PLLSRC is wrongly set!
#endif

#if ((PLLM_VAL) < 1) || ((PLLM_VAL) > 8)
#error PLLM_VAL is wrongly set!
#endif

#if ((PLLN_VAL) < 8) || ((PLLN_VAL) > 86)
#error PLLN is wrongly set!
#endif

#define f_VCO_CALC_Hz ((f_PLL_INPUT_Hz) * (PLLN_VAL))
#define f_PLL_RCLK_Hz ((f_VCO_CALC_Hz) / (PLLR_VAL))

#define RCC_CFGR_SW_HSISYS  0
#define RCC_CFGR_SW_HSE     1
#define RCC_CFGR_SW_PLLRCLK 2

#if SWS == RCC_CFGR_SW_HSISYS
  #define f_SYSCLK_CALC_Hz F_HSISYS_Hz
#elif SWS == RCC_CFGR_SW_HSE && (HSE_ON != 0)
  #define f_SYSCLK_CALC_Hz f_HSE_Hz
#elif SWS == RCC_CFGR_SW_PLLRCLK && (PLL_ON != 0)
  #define f_SYSCLK_CALC_Hz f_PLL_RCLK_Hz
#else
#error SWS is wrongly set!
#endif

#if HPRE_REG < 0x08
  #define AHB_PRESC_VAL 1
#elif HPRE_REG <= 15
  #define AHB_PRESC_VAL (2 << (HPRE_REG - 8))
#else
#error HPRE_REG is wrongly set!
#endif

#define f_HCLK_CALC_Hz (f_SYSCLK_Hz / AHB_PRESC_VAL)

#if PPRE_REG < 4
  #define APB_PRESC_VAL 1
#elif PPRE_REG < 8
  #define APB_PRESC_VAL (2 << (PPRE_REG - 4))
#else
#error PPRE_REG is wrongly set!
#endif

#define f_PCLK_CALC_Hz (f_HCLK_Hz / APB_PRESC_VAL)

#if APB_PRESC_VAL == 1
  #define f_TIMPCLK_CALC_Hz f_HCLK_Hz
#else
  #define f_TIMPCLK_CALC_Hz (f_HCLK_Hz * 2)
#endif

#define f_APB2_CALC_Hz (f_AHB_Hz / PPRE2_VAL)

#if (ADCPRE_REG >= 0) && (ADCPRE_REG <= 3)
#define ADCPRE_VAL (2 + (ADCPRE_REG * 2))
#else
#error ADCPRE_REG is wrongly set!
#endif

#define f_ADC_Hz (f_APB2_Hz / ADCPRE_VAL)

#ifndef f_LSE_Hz
#define f_LSE_USART_Hz 0
#else
#define f_LSE_USART_Hz f_LSE_Hz
#endif

#define f_USART1_Hz (((USART1SEL) == RCC_CCIPR_USART1SEL_PCLK)   ? (f_PCLK_Hz)   : \
                    (((USART1SEL) == RCC_CCIPR_USART1SEL_SYSCLK) ? (f_SYSCLK_Hz) : \
                    (((USART1SEL) == RCC_CCIPR_USART1SEL_HSI16)  ? (f_HSI_Hz)    : (f_LSE_USART_Hz))))

#define RCC_CCIPR2_USBSEL_HSI48 0
#define RCC_CCIPR2_USBSEL_HSE   1
#define RCC_CCIPR2_USBSEL_PLL   2

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
