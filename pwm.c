#include "pwm.h"

void PWM_Init(TIM_TypeDef *tim, uint32_t ccIdx)
{
  TIMx_CCRy_PWM1_Set(tim, ccIdx);
  PWM_Set(tim, ccIdx, 0);
}

void PWM_Set(TIM_TypeDef *tim, uint32_t ccIdx, uint32_t value)
{
  TIMx_CCRy_Set(tim, ccIdx, value);
}

void PWM_SetPercent(TIM_TypeDef *tim, uint32_t ccIdx, uint32_t valuePercent)
{
  PWM_Set(tim, ccIdx, (tim->ARR * valuePercent) / 1000);
}
