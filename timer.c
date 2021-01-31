#include "controller.h"

#include STM32_RCC_HEADER

#include "timer_conf.h"

#include "timer.h"

void SysTick_Init(void)
{
  uint32_t ticks = 65536;
  SysTick->LOAD  = (ticks & SysTick_LOAD_RELOAD_Msk) - 1;      /* set reload register */
  //NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);  /* set Priority for Cortex-M0 System Interrupts */
  SysTick->VAL   = ticks - 1;                                    /* Load the SysTick Counter Value */
  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                   /*SysTick_CTRL_TICKINT_Msk   | */
                   SysTick_CTRL_ENABLE_Msk;                    /* Enable SysTick IRQ and SysTick Timer */
}

void TIM2_Init(void)
{
  RCC->APB1ENR |= RCC_APB1Periph_TIM2;

  TIM2->CR1 = (TIM2->CR1 & 0xFC00) | TIM2_CR1_INIT;
  TIM2->CR2 = (TIM2->CR2 & 0xFF07) | TIM2_CR2_INIT;
  TIM2->ARR = TIM2_ARR_INIT;
  TIM2->PSC = TIM2_PSC_INIT;
  TIM2->EGR = 0x01; /* immediate reload */
  TIM2->CCMR1 = TIM2_CCMR1_INIT;
  TIM2->CCMR2 = TIM2_CCMR2_INIT;
  TIM2->CCER = TIM2_CCER_INIT;
  TIM2->SR = 0x0000; /* clear all IT requests */
  TIM2->DIER = (TIM2->DIER & 0xA0A0) | TIM2_DIER_INIT; /* enable update interrupt */
  TIM2->CR1 |= 1; /* enable timer */
}

void TIM3_Init(void)
{
  if (!(RCC->APB1ENR & RCC_APB1Periph_TIM3))
  {
    RCC->APB1ENR |= RCC_APB1Periph_TIM3;
  }
  TIM3->CR1 = (TIM3->CR1 & 0xFC00) | TIM3_CR1_INIT;
  TIM3->CR2 = (TIM3->CR2 & 0xFF07) | TIM3_CR2_INIT;
  TIM3->ARR = TIM3_ARR_INIT;
  TIM3->PSC = TIM3_PSC_INIT;
  TIM3->EGR = 0x01; /* immediate reload */
  TIM3->CCMR1 = TIM3_CCMR1_INIT;
  TIM3->CCMR2 = TIM3_CCMR2_INIT;
  TIM3->CCER = TIM3_CCER_INIT;
  TIM3->SR = 0x0000; /* clear all IT requests */
  TIM3->DIER = (TIM3->DIER & 0xA0A0) | TIM3_DIER_INIT; /* enable update interrupt */
  TIM3->CR1 |= 1; /* enable timer */
}
