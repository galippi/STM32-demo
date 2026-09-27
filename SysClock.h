#ifndef _SYSCLOCK_H_
#define _SYSCLOCK_H_

#include "controller.h"

#include "stm32c5xx_ll_rcc.h"

#define f_LSI_Hz 32000
#define f_HSI_Hz 144000000

#include "SysClock_conf.h"

#ifndef HSI_ON
  #define HSI_ON 0
#endif

#ifndef HSE_ON
  #define HSE_ON 0
#endif
#if (defined(f_HSE_Hz) && (HSE_ON == 0)) || (!defined(f_HSE_Hz) && (HSE_ON != 0))
  #error HSE config error!
#endif

#if (defined(f_HSE_Hz) && ((f_HSE_Hz < 4000000) || (f_HSE_Hz > 50000000)))
  #error HSE value config error!
#endif

#if !defined(HSE_BYP)
  #define HSE_BYP 0
#endif

#ifndef PSI_ON
  #ifdef f_PSI_Hz
    #define PSI_ON 1
    #ifndef PSIREFSRC
    #error PSI is on, but the PSIREFSRC is not defined!
    #endif
    #if PSIREFSRC == LL_RCC_PSISOURCE_HSE
    #define f_PSI_INPUT_CALC_Hz f_HSE_Hz
    #elif PSIREFSRC == LL_RCC_PSISOURCE_LSE
    #define f_PSI_INPUT_CALC_Hz f_LSE_Hz
    #elif PSIREFSRC == LL_RCC_PSISOURCE_HSIDIV18
    #define f_PSI_INPUT_CALC_Hz (f_HSI_Hz / 18)
    #else
    #error PSIREFSRC is wrong!
    #endif
    #if PSISRC == LL_RCC_PSIREF_32768HZ
      #define f_PSI_INPUT_REQ_Hz    32768
    #elif PSISRC == LL_RCC_PSIREF_16MHZ
      #define f_PSI_INPUT_REQ_Hz 16000000
    #else
      #error PSISRC is wrong!
    #endif
    #if (f_PSI_INPUT_CALC_Hz) != (f_PSI_INPUT_REQ_Hz)
    #error PSI configuration error!
    #endif
    #if PISFREQ == LL_RCC_PSIFREQ_100MHZ
    #define f_PSI_OUTPUT_CALC_Hz 100000000
    #elif PISFREQ == LL_RCC_PSIFREQ_144MHZ
    #define f_PSI_OUTPUT_CALC_Hz 144000000
    #elif PISFREQ == LL_RCC_PSIFREQ_166MHZ
    #define f_PSI_OUTPUT_CALC_Hz 160000000
    #else
    #error PSIFREQ is wrongly set!
    #endif
    #if f_PSI_OUTPUT_CALC_Hz != f_PSI_Hz
    #error PSI configuration is wrong!
    #endif
  #else
    #define PSI_ON 0
  #endif
#endif

#if (RCC_CFGR2_SW_VAL == LL_RCC_SYS_CLKSOURCE_HSIDIV3)
  #define f_SYSCLK_CALC_Hz ((f_HSI_Hz) / 3)
#elif (RCC_CFGR2_SW_VAL == LL_RCC_SYS_CLKSOURCE_HSIS)
  #define f_SYSCLK_CALC_Hz F_HSISYS_Hz
#elif RCC_CFGR2_SW_VAL == LL_RCC_SYS_CLKSOURCE_HSE && (HSE_ON != 0)
  #define f_SYSCLK_CALC_Hz f_HSE_Hz
#elif RCC_CFGR2_SW_VAL == LL_RCC_SYS_CLKSOURCE_PSIS && (PSI_ON != 0)
  #define f_SYSCLK_CALC_Hz f_PSI_Hz
#else
#error RCC_CFGR2_SW_VAL is wrongly set!
#endif

#if f_SYSCLK_Hz != f_SYSCLK_CALC_Hz
#error f_SYSCLK_Hz setting error!
#endif

#if HPRE_REG < 0x08
  #define AHB_PRESC_VAL 1
#elif HPRE_REG <= 15
  #define AHB_PRESC_VAL (1 << ((HPRE_REG) - 7))
#else
#error HPRE_REG is wrongly set!
#endif

#if 0

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

#endif

#define PPRE2_VAL 1 /* to be calculated */

#define f_APB2_CALC_Hz ((f_HCLK_Hz() / (PPRE2_VAL))

#if 0

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

#endif

#ifndef f_LSE_USART_Hz
#define f_LSE_USART_Hz 0
#endif

#define f_USART1_Hz (((USART1SEL) == LL_RCC_USART1_CLKSOURCE_PCLK2) ? (f_PCLK2_Hz)  : \
                    (((USART1SEL) == LL_RCC_USART1_CLKSOURCE_PSIK)  ? (f_SYSCLK_Hz) : \
                    (((USART1SEL) == LL_RCC_USART1_CLKSOURCE_HSIK)  ? (f_HSI_Hz)    : (f_LSE_USART_Hz))))

#if 0

#define RCC_CCIPR2_USBSEL_HSI48 0
#define RCC_CCIPR2_USBSEL_HSE   1
#define RCC_CCIPR2_USBSEL_PLL   2

#endif

#define SysTick_CTRL_CLKSOURCE_INTERNAL SysTick_CTRL_CLKSOURCE_Msk /* clock source is unknown */
#define SysTick_CTRL_CLKSOURCE_EXTERNAL 0 /* clock source is selected by RCC_CCIPR2_SYSTICKSEL */

#if (((SYSTICK_CTRL_VAL) & SysTick_CTRL_CLKSOURCE_Msk) != 0)
#define f_CortexSystemTimer_Hz (f_HSI_Hz)
#else
#if RCC_CCIPR2_SYSTICKSEL_INIT == LL_RCC_SYSTICK_CLKSOURCE_HCLKDIV8
#define f_CortexSystemTimer_Hz ((f_HCLK_Hz) / 8)
#elif RCC_CCIPR2_SYSTICKSEL_INIT == LL_RCC_SYSTICK_CLKSOURCE_LSE
#define f_CortexSystemTimer_Hz (f_LSE_Hz)
#elif RCC_CCIPR2_SYSTICKSEL_INIT == LL_RCC_SYSTICK_CLKSOURCE_LSI
#define f_CortexSystemTimer_Hz (f_LSI_Hz)
#else
#error Wrong System timer source config!
#endif
#endif

void SysClock_Init(void);
void SysTick_Init(void);

static inline uint32_t SysTick_Get(void)
{
  return (~SysTick->VAL) & 0xFFFFFF;
}

static inline uint32_t SysTick_GetCountFlag(void)
{
  return SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk;
}

#endif /* _SYSCLOCK_H_ */
