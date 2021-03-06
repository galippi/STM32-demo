#ifndef _TIMER_H_
#define _TIMER_H_

#include <stdint.h>

#include "controller.h"

void SysTick_Init(void);

static inline uint16_t SysTick_Get(void)
{
  return (~SysTick->VAL) & 0xFFFF;
}

void TIM2_Init(void);

static inline uint32_t TIM2_Cnt_Get(void)
{
  return TIM2->CNT;
}

static inline uint32_t TIM2_SR_CC3IF_Get(void)
{
  return (TIM2->SR & TIM_SR_CC3IF);
}

static inline void TIM2_SR_CC3IF_Reset(void)
{
  TIM2->SR = ~(TIM_SR_CC3IF); /* reset the CC1-InterrupFlag */
}

static inline uint32_t TIM2_CCR3_Get(void)
{
  return TIM2->CCR3;
}

static inline void TIM2_CCR3_Set(uint32_t new_val)
{
  TIM2->CCR3 = new_val;
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

static inline uint32_t TIM3_SR_CC2IF_Get(void)
{
  return (TIM3->SR & TIM_SR_CC2IF);
}

static inline void TIM3_SR_CC2IF_Reset(void)
{
  TIM3->SR = ~(TIM_SR_CC2IF); /* reset the CC1-InterrupFlag */
}

static inline uint16_t TIM3_CCR2_Get(void)
{
  return TIM3->CCR2;
}

static inline void TIM3_CCR2_Set(uint16_t new_val)
{
  TIM3->CCR2 = new_val;
}

static inline uint32_t TIM3_SR_CC3IF_Get(void)
{
  return (TIM3->SR & TIM_SR_CC3IF);
}

static inline void TIM3_SR_CC3IF_Reset(void)
{
  TIM3->SR = ~(TIM_SR_CC3IF); /* reset the CC3-InterrupFlag */
}

static inline void TIM3_SR_CC3OF_Reset(void)
{
  TIM3->SR = ~(TIM_SR_CC3OF); /* reset the CC3-oveflow-InterrupFlag */
}

static inline uint16_t TIM3_CCR3_Get(void)
{
  return TIM3->CCR3;
}

static inline void TIM3_CCR3_Set(uint16_t new_val)
{
  TIM3->CCR3 = new_val;
}

static inline uint32_t TIM3_SR_CC4IF_Get(void)
{
  return (TIM3->SR & TIM_SR_CC4IF);
}

static inline void TIM3_SR_CC4IF_Reset(void)
{
  TIM3->SR = ~(TIM_SR_CC4IF); /* reset the CC4-InterrupFlag */
}

static inline void TIM3_SR_CC4OF_Reset(void)
{
  TIM3->SR = ~(TIM_SR_CC4OF); /* reset the CC3-oveflow-InterrupFlag */
}

static inline uint16_t TIM3_CCR4_Get(void)
{
  return TIM3->CCR4;
}

static inline void TIM3_CCR4_Set(uint16_t new_val)
{
  TIM3->CCR4 = new_val;
}

#endif /* _TIMER_H_ */
