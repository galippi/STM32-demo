#ifndef _TIMER_H_
#define _TIMER_H_

#include <stdint.h>
#include "stm32f0xx.h"

void SysTick_Init(void);

static inline uint16_t SysTick_Get(void)
{
  return (~SysTick->VAL) & 0xFFFF;
}

void TIM3_Init(void);

static inline uint32_t TIM3_SR_UIF_Get(void)
{
  return (TIM3->SR & TIM_SR_UIF);
}

static inline void TIM3_SR_UIF_Reset(void)
{
  TIM3->SR = ~(TIM_SR_UIF); /* reset the UpdateInterrupFlag */
}

#endif /* _TIMER_H_ */
