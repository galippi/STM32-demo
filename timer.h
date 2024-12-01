#ifndef _TIMER_H_
#define _TIMER_H_

#include <stdint.h>

#include "controller.h"
#include "bitfield_lib.h"

void SysTick_Init(void);

static inline uint16_t SysTick_Get(void)
{
  return (~SysTick->VAL) & 0xFFFF;
}

static inline void TIMx_CCRy_Set(TIM_TypeDef *tim, uint32_t chIdx, uint16_t val)
{
    *((&(tim->CCR1)) + (chIdx * 2)) = val;
}

static inline uint16_t TIMx_CCRy_Get(TIM_TypeDef *tim, uint32_t chIdx)
{
    return *((&(tim->CCR1)) + (chIdx * 2));
}

// 0        - OC1CE: clear is not enabled
//  110     - PWM mode 1
//     1    - OC1PE: preload is enabled
//      0   - OC1FE: fast disabled
//       00 - CC1S: channel is output
#define PWM1_MODE_VAL 0x68

static inline void TIMx_CCRy_PWM1_Set(TIM_TypeDef *tim, uint32_t chIdx)
{
    uint8_t shiftValue = (chIdx & 0x01) * 8;
    if (chIdx < 2)
    {
        BitfieldSet(tim->CCMR1, shiftValue, 8, PWM1_MODE_VAL);
    }else
    {
        BitfieldSet(tim->CCMR2, shiftValue, 8, PWM1_MODE_VAL);
    }
    BitfieldSet(tim->CCER, chIdx * 4, 2, 0x01);
}

void TIM14_Init(void);

static inline uint32_t TIM14_SR_UIF_Get(void)
{
  return (TIM14->SR & TIM_SR_UIF);
}

static inline void TIM14_SR_UIF_Reset(void)
{
  TIM14->SR = ~(TIM_SR_UIF); /* reset the UpdateInterrupFlag */
}

static inline uint32_t TIM14_Cnt_Get(void)
{
  return TIM14->CNT;
}

static inline uint32_t TIM14_SR_CC1IF_Get(void)
{
  return (TIM14->SR & TIM_SR_CC1IF);
}

static inline void TIM14_SR_CC1IF_Reset(void)
{
  TIM14->SR = ~(TIM_SR_CC1IF); /* reset the CC1-InterrupFlag */
}

static inline uint32_t TIM14_CCR1_Get(void)
{
  return TIM14->CCR1;
}

static inline void TIM14_CCR1_Set(uint32_t new_val)
{
  TIM14->CCR1 = new_val;
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

void TIM16_Init(void);

static inline uint32_t TIM16_SR_UIF_Get(void)
{
  return (TIM16->SR & TIM_SR_UIF);
}

static inline void TIM16_SR_UIF_Reset(void)
{
  TIM16->SR = ~(TIM_SR_UIF); /* reset the UpdateInterrupFlag */
}

static inline uint16_t TIM16_Cnt_Get(void)
{
  return TIM16->CNT;
}

static inline uint32_t TIM16_SR_CC1IF_Get(void)
{
  return (TIM16->SR & TIM_SR_CC1IF);
}

static inline void TIM16_SR_CC1IF_Reset(void)
{
  TIM16->SR = ~(TIM_SR_CC1IF); /* reset the CC1-InterrupFlag */
}

static inline uint16_t TIM16_CCR1_Get(void)
{
  return TIM16->CCR1;
}

static inline void TIM16_CCR1_Set(uint16_t new_val)
{
  TIM16->CCR1 = new_val;
}

static inline uint32_t TIM16_SR_CC2IF_Get(void)
{
  return (TIM16->SR & TIM_SR_CC2IF);
}

static inline void TIM16_SR_CC2IF_Reset(void)
{
  TIM16->SR = ~(TIM_SR_CC2IF); /* reset the CC1-InterrupFlag */
}

static inline uint16_t TIM16_CCR2_Get(void)
{
  return TIM16->CCR2;
}

static inline void TIM16_CCR2_Set(uint16_t new_val)
{
  TIM16->CCR2 = new_val;
}

static inline uint32_t TIM16_SR_CC3IF_Get(void)
{
  return (TIM16->SR & TIM_SR_CC3IF);
}

static inline void TIM16_SR_CC3IF_Reset(void)
{
  TIM16->SR = ~(TIM_SR_CC3IF); /* reset the CC3-InterrupFlag */
}

static inline void TIM16_SR_CC3OF_Reset(void)
{
  TIM16->SR = ~(TIM_SR_CC3OF); /* reset the CC3-oveflow-InterrupFlag */
}

static inline uint16_t TIM16_CCR3_Get(void)
{
  return TIM16->CCR3;
}

static inline void TIM16_CCR3_Set(uint16_t new_val)
{
  TIM16->CCR3 = new_val;
}

static inline uint32_t TIM16_SR_CC4IF_Get(void)
{
  return (TIM16->SR & TIM_SR_CC4IF);
}

static inline void TIM16_SR_CC4IF_Reset(void)
{
  TIM16->SR = ~(TIM_SR_CC4IF); /* reset the CC4-InterrupFlag */
}

static inline void TIM16_SR_CC4OF_Reset(void)
{
  TIM16->SR = ~(TIM_SR_CC4OF); /* reset the CC3-oveflow-InterrupFlag */
}

static inline uint16_t TIM16_CCR4_Get(void)
{
  return TIM16->CCR4;
}

static inline void TIM16_CCR4_Set(uint16_t new_val)
{
  TIM16->CCR4 = new_val;
}

#endif /* _TIMER_H_ */
