#include "pll.h"

uint32_t StartUpCounter;

void PLL_Init(void)
{
#if !defined(TARGET_ECU)
#error "Error: TARGET ECU is not defined!"
#elif TARGET_ECU == TARGET_ECU_STM32F0DISCOVERY
  /* Enable HSE - high speed external oscillator */
  RCC->CR |= (uint32_t)(RCC_CR_HSEON | RCC_CR_HSEBYP);

  StartUpCounter = 0;
  /* Wait till HSE is ready and if Time out is reached exit */
  while(((RCC->CR & RCC_CR_HSERDY) == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT))
  {
    StartUpCounter++;
  }
  if ((RCC->CR & RCC_CR_HSERDY))
  {
    /* Enable Prefetch Buffer and set Flash Latency */
    FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY;

    /* HCLK = SYSCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;

    /* PCLK = HCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE_DIV1;


    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL));
    RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_PREDIV1 | RCC_CFGR_PLLXTPRE_PREDIV1 | RCC_CFGR_PLLMULL6); /* PLL configuration = HSE * 6 = 48 MHz */

    /* Enable PLL */
    RCC->CR |= RCC_CR_PLLON;

    /* Wait till PLL is ready */
    while((RCC->CR & RCC_CR_PLLRDY) == 0)
    {
    }

    /* Select PLL as system clock source */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;

    /* Wait till PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)RCC_CFGR_SWS_PLL)
    {
    }
  }
  else
  { /* If HSE fails to start-up, the application will have wrong clock
         configuration. User can add here some code to deal with this error */
  }
#elif TARGET_ECU == TARGET_ECU_STM32F103C8_ARDUINO
  /* Enable HSE - high speed external oscillator */
  RCC->CR |= (uint32_t)(RCC_CR_HSEON);

  StartUpCounter = 0;
  /* Wait till HSE is ready and if Time out is reached exit */
  while(((RCC->CR & RCC_CR_HSERDY) == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT))
  {
    StartUpCounter++;
  }
  if ((RCC->CR & RCC_CR_HSERDY))
  {
    /* Enable Prefetch Buffer and set Flash Latency */
    FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY_1;

    /* HCLK = SYSCLK = 48MHz*/
    RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;

#if PCLK2 != 48000
#error Wrong PCLK2 is defined!
#endif
    /* PCLK2 = HCLK = 48 MHz */
    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_PPRE2) | RCC_CFGR_PPRE2_DIV1;
    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_ADCPRE) | RCC_CFGR_ADCPRE_DIV4; /* ADC clock is set to 48 (PCLK2) / 4 => 12MHz */
#if PCLK1 != 24000
#error Wrong PCLK1 is defined!
#endif
    /* PCLK1 = HCLK / 2 = 24 MHz */
    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_PPRE1) | (RCC_CFGR_PPRE1_DIV2);

    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL));
    RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_PREDIV1 | RCC_CFGR_PLLXTPRE_PREDIV1_Div2 | RCC_CFGR_PLLMULL12); /* PLL configuration = HSE / 2 * 12 = 48 MHz */

    /* Enable PLL */
    RCC->CR |= RCC_CR_PLLON;

    /* Wait till PLL is ready */
    while((RCC->CR & RCC_CR_PLLRDY) == 0)
    {
    }

    /* Select PLL as system clock source */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;

    /* Wait till PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)RCC_CFGR_SWS_PLL)
    {
    }
  }
  else
  { /* If HSE fails to start-up, the application will have wrong clock
         configuration. User can add here some code to deal with this error */
  }
#else
#error CPU type is not yet implemented!
#endif
}
