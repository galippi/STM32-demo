#ifndef _PWM_H_
#define _PWM_H_

#include "timer.h"

void PWM_Init(TIM_TypeDef *tim, uint32_t ccIdx);
void PWM_Set(TIM_TypeDef *tim, uint32_t ccIdx, uint32_t value);
void PWM_SetPercent(TIM_TypeDef *tim, uint32_t ccIdx, uint32_t valuePercent);

#endif /* _PWM_H_ */
