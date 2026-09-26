#include "bitfield_lib.h"

#include "SysClock.h"

#if defined(f_PSI_INPUT_Hz) && (((f_PSI_INPUT_Hz) < 2600000) || ((f_PLL_PSI_Hz) > 16000000))
#error PSI input is wrongly configured!
#endif

#if f_SYSCLK_Hz > 150000000
#error f_SYSCLK_Hz is wrongly set!
#endif

#if f_SYSCLK_CALC_Hz != f_SYSCLK_Hz
#error System clock config error!
#endif

//#if f_HCLK_Hz > 64000000
//#error f_HCLK_Hz is wrongly set!
//#endif

#define f_HCLK_CALC_Hz ((f_SYSCLK_Hz) / (AHB_PRESC_VAL))
#if (f_HCLK_CALC_Hz) != (f_HCLK_Hz)
#error f_HCLK_Hz config error!
#endif

//#if f_PCLK_Hz > 64000000
//#error f_PCLK_Hz is wrongly set!
//#endif

#if 0
#if (f_PCLK_CALC_Hz) != (f_PCLK_Hz)
#error f_PCLK_Hz config error!
#endif

//#if f_TIMPCLK_Hz > 64000000
//#error f_PCLK_Hz is wrongly set!
//#endif

//#if f_APB_Hz > 64000000
//#error f_APB_Hz is wrongly set!
//#endif

#if CLOCK_USBSEL == RCC_CCIPR2_USBSEL_HSI48
#if HSI48_ON == 0
#error HSI48 shall be enabled, if the USB is configured to use it!
#endif
#define f_USB_Hz f_HSI48_Hz
#elif CLOCK_USBSEL == RCC_CCIPR2_USBSEL_HSE
#define f_USB_Hz f_HSE_Hz
#elif CLOCK_USBSEL == RCC_CCIPR2_USBSEL_PLL
#define f_USB_Hz f_PLL_Hz
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

#if defined(f_VCO_Hz)
#if f_VCO_CALC_Hz != f_VCO_Hz
#error f_VCO_Hz is wrong!
#endif
#if ((f_VCO_Hz) < 96000000) || ((f_VCO_Hz) > 344000000)
#error f_VCO_Hz is wrongly set!
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

#endif

#ifndef HSI_STARTUP_DEBUG
#define HSI_STARTUP_DEBUG(val) /* do nothing */
#endif

#ifndef HSI48_STARTUP_DEBUG
#define HSI48_STARTUP_DEBUG(val) /* do nothing */
#endif

#ifndef HSE_STARTUP_DEBUG
#define HSE_STARTUP_DEBUG(val) /* do nothing */
#endif

volatile char start_enable = 0;

void SysClock_Init(void)
{
  volatile uint32_t StartUpCounter;

  if (((RCC->CFGR1 & (uint32_t)RCC_CFGR1_SWS) != LL_RCC_SYS_CLKSOURCE_STATUS_HSIDIV3) &&
      ((RCC->CFGR1 & (uint32_t)RCC_CFGR1_SWS) != LL_RCC_SYS_CLKSOURCE_STATUS_HSIS))
  {
    /* Enable HSI temporarly - high speed internal oscillator */
    RCC->CR1 |= (uint32_t)(RCC_CR1_HSISON);
    StartUpCounter = 0;
    while(((RCC->CR1 & RCC_CR1_HSISRDY) == 0) && (StartUpCounter != HSI_STARTUP_TIMEOUT))
    {
      StartUpCounter++;
    }
    HSI_STARTUP_DEBUG(StartUpCounter);
    if (!(RCC->CR1 & RCC_CR1_HSISRDY))
    {
      HSI_STARTUP_ERROR();
    }
    RCC->CR1 = RCC->CR1 | RCC_CR1_HSIDIV3ON_Msk;
    /* Select HSI as system clock source */
    RCC->CFGR1 = (RCC->CFGR1 & (uint32_t)((uint32_t)~(RCC_CFGR1_SW))) | (LL_RCC_SYS_CLKSOURCE_STATUS_HSIDIV3);
    /* Wait till HSI is used as system clock source */
    StartUpCounter = 0;
    while ((RCC->CFGR1 & (uint32_t)RCC_CFGR1_SWS) != (LL_RCC_SYS_CLKSOURCE_STATUS_HSIDIV3))
    {
      StartUpCounter++;
    }
    SYSCLK_SET_DEBUG(StartUpCounter);
  }else{
      //BitfieldSet(RCC->CR,RCC_CR_HSIDIV_Pos, 3,  HSIDIV_REG);
  }

  #if HSE_ON != 0
  /* Enable HSE - high speed external oscillator */
  RCC->CR1 |= (uint32_t)(RCC_CR1_HSEON);

  /* Wait till HSE is ready and if Time out is reached exit */
  StartUpCounter = 0;
  while(((RCC->CR1 & RCC_CR1_HSERDY) == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT))
  {
    StartUpCounter++;
  }
  HSE_STARTUP_DEBUG(StartUpCounter);
  if (!(RCC->CR1 & RCC_CR1_HSERDY))
  {
    HSE_STARTUP_ERROR(0);
  }
  #else /* HSE_ON == 0 */
  /* Stop HSE - high speed external oscillator */
  RCC->CR1 &= (uint32_t)(~RCC_CR1_HSEON);
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

#if 0
    /* Stop PLL */
    RCC->CR &= ~RCC_CR_PLLON;
    StartUpCounter = 0;
    while((RCC->CR & RCC_CR_PLLRDY) != 0)
    {
      StartUpCounter++;
    }
    PLL_STOP_DEBUG(StartUpCounter);

    {
        //while(start_enable == 0)
            ;
    }
    #if PLL_ON != 0
    /* PLL configuration */
    RCC->PLLCFGR = ((PLLM_VAL) - 1) << (RCC_PLLCFGR_PLLM_Pos) |
                   (((PLLSRC) << RCC_PLLCFGR_PLLSRC_Pos) | \
                   ((PLLN_VAL) << RCC_PLLCFGR_PLLN_Pos));

    #if (SWS == 2) // RCC_CFGR_SWS_PLLRCLK
      #if ((f_PLL_RCLK_Hz) > 64000000)
        #error PLLR config failure!
      #endif
    //RCC->PLLCFGR = (RCC->PLLCFGR & ~RCC_PLLCFGR_PLLR_Msk) | (RCC_PLLCFGR_PLLREN | ((PLLR_VAL) << RCC_PLLCFGR_PLLR_Pos));
    RCC->PLLCFGR = (RCC->PLLCFGR & ~RCC_PLLCFGR_PLLR_Msk) | (((PLLR_VAL) - 1) << RCC_PLLCFGR_PLLR_Pos);
    RCC->PLLCFGR = RCC->PLLCFGR | RCC_PLLCFGR_PLLREN;
    #endif

    #if 0
    RCC->CFGR = (RCC->CFGR & (~(uint32_t)(RCC_CFGR_ | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL)))
                           | (RCC_CFGR_PLLSRC * PLLSRC)
                           | (RCC_CFGR_PLLXTPRE * PLLXTPRE_REG)
                           | (RCC_CFGR_PLLMULL3 * (PLLMUL_VAL - 2));

    RCC->CFGR = (RCC->CFGR & (~(uint32_t)(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL)))
                           | (RCC_CFGR_PLLSRC * PLLSRC)
                           | (RCC_CFGR_PLLXTPRE * PLLXTPRE_REG)
                           | (RCC_CFGR_PLLMULL3 * (PLLMUL_VAL - 2));
#endif

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

#endif

#if ((RCC_CFGR2_SW_VAL == LL_RCC_SYS_CLKSOURCE_STATUS_HSIDIV3) || \
     (RCC_CFGR2_SW_VAL == LL_RCC_SYS_CLKSOURCE_STATUS_HSIDIV3)) && \
    (HSI_ON == 0)
#error HSI / SW config error!
#endif

#if 0

#if (RCC_CFGR2_SW_VAL == RCC_CFGR_SWS_HSE)
#if !defined(HSE_EXTERNAL_OSC)
    #error HSE / RCC_CFGR2_SW_VAL config error!
#endif
    if (!(RCC->CR1 & RCC_CR1_HSERDY))
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

#endif

    /* Select system clock source */
    RCC->CFGR1 = (RCC->CFGR1 & (uint32_t)((uint32_t)~(RCC_CFGR1_SW))) | ((RCC_CFGR2_SW_VAL) << RCC_CFGR1_SW_Pos);

    /* Wait till system clock source is set */
    StartUpCounter = 0;
    while ((RCC->CFGR1 & (uint32_t)RCC_CFGR1_SWS) != ((RCC_CFGR2_SW_VAL) << RCC_CFGR1_SWS_Pos))
    {
      StartUpCounter++;
    }
    SYSCLK_SET_DEBUG(StartUpCounter);

    #if HSI_ON == 0
    RCC->CR &= (uint32_t)(~RCC_CR_HSION);
    #endif
    #ifdef f_LSE_Hz
      RCC->RTCCR |= RCC_RTCCR_LSEON;
    #else
      RCC->RTCCR &= ~RCC_RTCCR_LSEON;
    #endif
  }

  //BitfieldSet(RCC->CFGR, RCC_CFGR_HPRE_Pos, 4, HPRE_REG);

#if defined(f_USBCLK_Hz)
  RCC->CCIPR2 = (RCC->CCIPR2 & ~(RCC_CCIPR2_USBSEL_Msk)) | (CLOCK_USBSEL << RCC_CCIPR2_USBSEL_Pos);
  RCC->PLLCFGR |= RCC_PLLCFGR_PLLQEN;
#endif
}

void SysTick_Init(void)
{
  const uint32_t ticks = SYSTICK_RELOAD_VAL;
  SysTick->CTRL = 0; // disable timer
  RCC->CCIPR2 = (RCC->CCIPR2 & ~RCC_CCIPR2_SYSTICKSEL_Msk) | RCC_CCIPR2_SYSTICKSEL_INIT;
  SysTick->LOAD  = (ticks & SysTick_LOAD_RELOAD_Msk) - 1;      /* set reload register */
  //NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);  /* set Priority for Cortex-M0 System Interrupts */
  SysTick->VAL   = ticks - 1;                                    /* Load the SysTick Counter Value */
  SysTick->CTRL  = SYSTICK_CTRL_VAL;
}
