#ifndef _TIMER_CONF_H_
#define _TIMER_CONF_H_

#include "system_conf.h"
#include "scheduler_preemptive.h"

#define TIM3_CR1_INIT 0x00 /* up counter mode */
#define TIM3_CR2_INIT 0x0
#define TIM3_FREQ 1000 /* kHz */
#define TIM3_PSC_INIT  ((F_SYSTEM / TIM3_FREQ) - 1) /* prescaler of TIM3 -> 1MHz */
#define TIM3_ARR_INIT 1000 /* 1MHz / 1000 -> 1ms timebase */
#define TIM3_CCER_INIT 0x01 /* enable CC1 out high */
#define TIM3_CCMR1_INIT 0x0000
#define TIM3_CCMR2_INIT 0x0000

static inline void TIM3_UIF_Callback(void)
{ /* call back function of TIM3 UIF - counter underflow */
  Scheduler();
}

#endif /* _TIMER_CONF_H_ */
