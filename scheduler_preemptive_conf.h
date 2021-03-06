#ifndef _SCHEDULER_PREEMPTIVE_CONF_H_
#define _SCHEDULER_PREEMPTIVE_CONF_H_

#include "controller.h"
#include "tasks.h"
#include "FaultHandler.h"

#define t_SchedPreTask_Timer uint16_t

#define SCHED_PRE_TASK_LIST \
  SCHED_PRE_TASK_DEF(Task_1ms,     1) \
  SCHED_PRE_TASK_DEF(Task_10ms,   10) \
  SCHED_PRE_TASK_DEF(Task_500ms, 100)

#define SchedPreTask_ActivateTask() { \
  /* activate PendSV handler */ \
  SCB->ICSR = SCB_ICSR_PENDSVSET_Msk; \
}

#define SchedPreTask_Disable() { \
}

#define SchedPreTask_Enable() { \
}

/* atomic function to set the new task status, if it was in the given state */
#define atomic_check_and_set_u8(var, val_old, val_new) \
    ( (__LDREXB(&var) == val_old) ? (__STREXB(val_new, &var) == 0) : (__CLREX(), 0))

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
