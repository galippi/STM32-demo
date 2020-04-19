#ifndef _TIMER_CONF_H_
#define _TIMER_CONF_H_

#include "SysClock_conf.h"
#include "scheduler_preemptive.h"

static inline void TIM2_CC3IF_Callback(void)
{ /* call back function of TIM2 CC1IF - capture compare event */
}


#define TIM3_CR1_INIT 0x00 /* up counter mode */
#define TIM3_CR2_INIT 0x0
#define TIM3_FREQ 1000 /* Hz */
#define TIM3_PSC_INIT  ((f_TIMXCLK_Hz / (TIM3_FREQ * 1000)) - 1) /* prescaler of TIM3 -> 1MHz */
#define TIM3_ARR_INIT 65535 /* continuous counter */
#define TIM3_CCER_INIT 0x0101 /* enable CC1 out high, CC3/CC4 input */
#define TIM3_CCMR1_INIT 0x0000
#define TIM3_CCMR2_INIT 0x0000
#define TIM3_DIER_INIT 0x0001 /* enable update interrupt */

static inline void TIM3_UIF_Callback(void)
{ /* call back function of TIM3 UIF - counter underflow */
  Scheduler();
}

#endif /* _TIMER_CONF_H_ */
