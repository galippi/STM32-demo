#ifndef _TIMER_APP_H_
#define _TIMER_APP_H_

#include "timer.h"
#include "timer_conf.h"

static inline void TIM3_UIF_PollHandler(void)
{
  if (TIM3_SR_UIF_Get())
  {
    TIM3_SR_UIF_Reset();
    TIM3_UIF_Callback();
  }
}

static inline void TIM3_CC1IF_Callback(void)
{ /* call back function of TIM3 UIF - counter underflow */
  TIM3_CCR1_Set(TIM3_CCR1_Get() + TIM3_FREQ); /* set next interrupt to the next 1ms slot */
  SchedulerPre_TaskTableUpdate();
  SCB->ICSR = SCB_ICSR_PENDSVSET_Msk; /* activate PendSV handler */
}

static inline void TIM3_CC1IF_PollHandler(void)
{
  if (TIM3_SR_CC1IF_Get())
  {
    TIM3_SR_CC1IF_Reset();
    TIM3_CCR1_Set(TIM3_CCR1_Get() + TIM3_FREQ); /* set next interrupt to the next 1ms slot */
    TIM3_CC1IF_Callback();
  }
}

extern uint8_t tim3_cc3_ctr;
extern uint16_t encoder;
static inline void TIM3_CC3IF_Callback(void)
{
  tim3_cc3_ctr++;
  static uint16_t t_prev;
  uint16_t t = TIM3_CCR3_Get();
  uint16_t dt = t - t_prev;
  t_prev = t;
  if (dt > 200)
  {
	  if (GPIO_GetI(GPIOB, 1))
	  {
	    encoder++;
	  }else
	  {
	    encoder--;
	  }
  }
}

extern uint8_t tim3_cc4_ctr;
static inline void TIM3_CC4IF_Callback(void)
{
  tim3_cc4_ctr++;
}

static inline uint16_t getTimer_us(void)
{
  return TIM3_Cnt_Get();
}

static inline void wait_us(uint32_t time)
{
  uint16_t t_start = getTimer_us();
  while (time > 16384)
  {
    wait_us(16384);
    time -= 16384;
    t_start += 16384;
  }
  while (((getTimer_us() - t_start) & 0xFFFF) < time)
  { /* wait - do nothing */
  }
}

#endif /* _TIMER_APP_H_ */
