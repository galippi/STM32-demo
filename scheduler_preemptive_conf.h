#ifndef _SCHEDULER_PREEMPTIVE_CONF_H_
#define _SCHEDULER_PREEMPTIVE_CONF_H_

#include "controller.h"
#include "tasks.h"
#include "FaultHandler.h"

#define t_SchedPreTask_Timer uint16_t

#define SCHED_PRE_TASK_LIST \
  SCHED_PRE_TASK_DEF(Task_1ms,     1) \
  SCHED_PRE_TASK_DEF(Task_10ms,   10) \
  SCHED_PRE_TASK_DEF(Task_500ms, 500)

#define SchedPreTask_ActivateTask() { \
  /* activate PendSV handler */ \
  SCB->ICSR = SCB_ICSR_PENDSVSET_Msk; \
}

static inline void TIM14_CC1IF_Callback(void)
{ /* call back function of TIM3 UIF - counter underflow */
  TIM14_CCR1_Set(TIM14_CCR1_Get() + TIM14_1ms); /* set next interrupt to the next 1ms slot */
  if (((TIM14_CCR1_Get() - TIM14_Cnt_Get()) & 0xFFFF) > TIM14_1ms)
    SchedulerPre_LostInterrupt();
  SchedulerPre_TaskTableUpdate();
  SCB->ICSR = SCB_ICSR_PENDSVSET_Msk; /* activate PendSV handler */
}

#define SchedPreTask_Disable() __disable_irq()

#define SchedPreTask_Enable() __enable_irq()

/* atomic function to set the new task status, if it was in the given state */
#if 1
inline static char atomic_check_and_set_u8(uint8_t *var, uint8_t val_old, uint8_t val_new)
{
    char ret;
    SchedPreTask_Disable();
    if (*(var) == (val_old)) {
        *(var) = (val_new);
        ret = 1;
    }else{
        ret = 0;
    }
    SchedPreTask_Enable();
    return ret;
}
#else
#define atomic_check_and_set_u8(var, val_old, val_new) \
    ( (__LDREXB(&var) == val_old) ? (__STREXB(val_new, &var) == 0) : (__CLREX(), 0))
#endif

#define SchedPreTask_TaskStart(func) { \
  func(); \
}
#define SchedPreTask_ErrorTaskOverrun(i) { \
  CAT_Error(CAT_TaskOverrun_1ms + i, 0); \
}

#define SchedulerPre_LostInterruptCallBack() \
  CAT_Error(CAT_SchedLostInterrupt, 0)

#define SchedPreTask_EnableCPULoadMeas 1
#define SchedPreTask_EnableTaskLoadMeas 1
#define SchedPreTask_LoadMeasResolution TIM3_FREQ

#endif /* _SCHEDULER_PREEMPTIVE_CONF_H_ */
