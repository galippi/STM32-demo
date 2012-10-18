#ifndef _TIMER_H_
#define _TIMER_H_

#include <stdint.h>

#include "controller.h"

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

static inline uint16_t TIM3_Cnt_Get(void)
{
  return TIM3->CNT;
}

static inline uint32_t TIM3_SR_CC1IF_Get(void)
{
  return (TIM3->SR & TIM_SR_CC1IF);
}

static inline void TIM3_SR_CC1IF_Reset(void)
{
  TIM3->SR = ~(TIM_SR_CC1IF); /* reset the CC1-InterrupFlag */
}

static inline uint16_t TIM3_CCR1_Get(void)
{
  return TIM3->CCR1;
}

static inline void TIM3_CCR1_Set(uint16_t new_val)
{
  TIM3->CCR1 = new_val;
}

#endif /* _TIMER_H_ */
