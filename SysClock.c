#include "SysClock.h"

#if f_SYSCLK_Hz > 64000000
#error f_SYSCLK_Hz is wrongly set!
#endif

#if f_HCLK_Hz > 64000000
#error f_HCLK_Hz is wrongly set!
#endif

#if f_PCLK_Hz > 64000000
#error f_PCLK_Hz is wrongly set!
#endif

#if f_TIMPCLK_Hz > 64000000
#error f_PCLK_Hz is wrongly set!
#endif

#if f_APB_Hz > 64000000
#error f_APB_Hz is wrongly set!
#endif

#if defined(f_USBCLK_Hz) && ((f_USBCLK_Hz != 48000000) || (f_USB_Hz != f_USBCLK_Hz))
#error f_USBCLK_Hz is wrongly set!
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

#if defined(f_PLL_Hz)
#if f_PLL_CALC_Hz != f_PLL_Hz
#error PLL_CALC_Hz is wrong!
#endif
#if f_PLL_Hz > f_PLL_MAX_HZ
#error f_PLL_Hz is wrongly set!
#endif
#endif

#if 0
#if f_AHB_CALC_Hz != f_AHB_Hz
#error f_AHB_CALC_Hz is wrong!
#endif

#if f_APB1_CALC_Hz != f_APB1_Hz
#error f_APB1_CALC_Hz is wrong!
#endif

#if f_TIMXCLK_CALC_Hz != f_TIMXCLK_Hz
#error f_TIMXCLK_CALC_Hz is wrong!
#endif

#if f_APB2_CALC_Hz != f_APB2_Hz
#error f_APB2_CALC_Hz is wrong!
#endif

#if f_TIM1CLK_CALC_Hz != f_TIM1CLK_Hz
#error f_TIM1CLK_CALC_Hz is wrong!
#endif

#if f_ADC_Hz > 14000000
#error ADCPRE_REG is wrongly set!
#endif

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

#endif

void SysClock_Init(void)
{
  volatile uint32_t StartUpCounter;

  if ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (RCC_CFGR_SWS_HSISYS))
  {
    /* Enable HSI temporarly - high speed internal oscillator */
    RCC->CR |= (uint32_t)(RCC_CR_HSION);
    StartUpCounter = 0;
    while(((RCC->CR & RCC_CR_HSIRDY) == 0) && (StartUpCounter != HSI_STARTUP_TIMEOUT))
    {
      StartUpCounter++;
    }
    HSI_STARTUP_DEBUG(StartUpCounter);
    if (!(RCC->CR & RCC_CR_HSIRDY))
    {
      HSI_STARTUP_ERROR();
    }
    /* Select HSI as system clock source */
    RCC->CFGR = (RCC->CFGR & (uint32_t)((uint32_t)~(RCC_CFGR_SW))) | (RCC_CFGR_SWS_HSISYS);
    /* Wait till HSI is used as system clock source */
    StartUpCounter = 0;
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (RCC_CFGR_SWS_HSISYS))
    {
      StartUpCounter++;
    }
    SYSCLK_SET_DEBUG(StartUpCounter);
  }
  #if HSE_ON != 0
  /* Enable HSE - high speed external oscillator */
  RCC->CR |= (uint32_t)(RCC_CR_HSEON);

  /* Wait till HSE is ready and if Time out is reached exit */
  StartUpCounter = 0;
  while(((RCC->CR & RCC_CR_HSERDY) == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT))
  {
    StartUpCounter++;
  }
  HSE_STARTUP_DEBUG(StartUpCounter);
  if (!(RCC->CR & RCC_CR_HSERDY))
  {
    HSE_STARTUP_ERROR(0);
  }
  #else /* HSE_ON == 0 */
  /* Stop HSE - high speed external oscillator */
  RCC->CR &= (uint32_t)(~RCC_CR_HSEON);
  #endif /* HSE_ON != 0 */
  {
#if 0
    /* Enable Prefetch Buffer and set Flash Latency */
    FLASH->ACR = ((PRFTBE_REG != 0) ? FLASH_ACR_PRFTBE : 0) | LATENCY_VAL | ((HLFCYA_REG != 0) ? FLASH_ACR_HLFCYA : 0);

    RCC->CFGR = (RCC->CFGR & ~(RCC_CFGR_HPRE | RCC_CFGR_PPRE1 | RCC_CFGR_PPRE2 | RCC_CFGR_ADCPRE))
                           | (RCC_CFGR_HPRE_0 * HPRE_REG)
                           | (RCC_CFGR_PPRE2_0 * PPRE2_REG) /* set PCLK2 / APB2 clock */
                           | (RCC_CFGR_ADCPRE_0 * ADCPRE_REG) /* ADC clock is set */
                           | (RCC_CFGR_PPRE1_0 * PPRE1_REG) /* set PCLK1 */;
#endif

    /* Stop PLL */
    RCC->CR &= ~RCC_CR_PLLON;
    #if PLL_ON != 0
    /* PLL configuration */
    RCC->CFGR = (RCC->CFGR & (~(uint32_t)(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL)))
                           | (RCC_CFGR_PLLSRC * PLLSRC)
                           | (RCC_CFGR_PLLXTPRE * PLLXTPRE_REG)
                           | (RCC_CFGR_PLLMULL3 * (PLLMUL_VAL - 2));

    /* Start PLL */
    RCC->CR |= RCC_CR_PLLON;
    /* Wait till PLL is ready */
    StartUpCounter = 0;
    while((RCC->CR & RCC_CR_PLLRDY) == 0)
    {
      StartUpCounter++;
    }
    PLL_STARTUP_DEBUG(StartUpCounter);
    #endif /* PLL_ON != 0 */

#if (SWS == RCC_CFGR_SWS_HSISYS) && (HSI_ON == 0)
#error HSI / SWS config error!
#endif

#if (SWS == RCC_CFGR_SWS_HSE)
#if !defined(HSE_EXTERNAL_OSC)
    #error HSE / SWS config error!
#endif
    if (!(RCC->CR & RCC_CR_HSERDY))
    {
      HSE_STARTUP_ERROR(1);
    }
#endif

#if (SWS == RCC_CFGR_SWS_PLLRCLK) && (PLL_ON == 0)
#error PLL / SWS config error!
#endif

#if (SWS == RCC_CFGR_SWS_LSI) || (SWS == RCC_CFGR_SWS_LSE)
#error SWS config error!
#endif

    /* Select system clock source */
    RCC->CFGR = (RCC->CFGR & (uint32_t)((uint32_t)~(RCC_CFGR_SW))) | (SWS);

    /* Wait till system clock source is set */
    StartUpCounter = 0;
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (RCC_CFGR_SWS_0 * SWS))
    {
      StartUpCounter++;
    }
    SYSCLK_SET_DEBUG(StartUpCounter);

    #if HSI_ON == 0
    RCC->CR &= (uint32_t)(~RCC_CR_HSION);
    #endif
    #ifdef f_LSE_Hz
      RCC->BDCR |= RCC_BDCR_LSEON;
    #else
      RCC->BDCR &= ~RCC_BDCR_LSEON;
    #endif
  }
}
