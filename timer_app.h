#ifndef _TIMER_APP_H_
#define _TIMER_APP_H_

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

#endif /* _TIMER_APP_H_ */
