#ifndef _PWM_H_
#define _PWM_H_

#include "timer.h"

void PWM_Init(TIM_TypeDef *tim, uint32_t ccIdx);
void PWM_Set(TIM_TypeDef *tim, uint32_t ccIdx, uint32_t value);
void PWM_SetPercent(TIM_TypeDef *tim, uint32_t ccIdx, uint32_t valuePercent);

static inline uint32_t PWM_Get(TIM_TypeDef *tim, uint32_t ccIdx)
{
  return TIMx_CCRy_Get(tim, ccIdx);
}

#endif /* _PWM_H_ */
