#ifndef _TIMER_CONF_H_
#define _TIMER_CONF_H_

#define TIM3_CR1_INIT 0x10 /* down counter mode */
#define TIM3_CR2_INIT 0x0
#define TIM3_PSC_INIT  7 /* 8MHz / (7+1) -> 1MHz */
#define TIM3_ARR_INIT 1000 /* 1MHz / 1000 -> 1ms timebase */
#define TIM3_CCER_INIT 0x01 /* enable CC1 out high */
#define TIM3_CCMR1_INIT 0x0000
#define TIM3_CCMR2_INIT 0x0000

#endif /* _TIMER_CONF_H_ */
