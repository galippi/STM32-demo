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

#define SchedPreTask_Disable() { \
}

#define SchedPreTask_Enable() { \
}

#define SchedPreTask_TaskStart(func) { \
  func(); \
}
#define SchedPreTask_ErrorTaskOverrun(i) { \
  CAT_Error(CAT_TaskOverrun_1ms + i); \
}

#endif /* _SCHEDULER_PREEMPTIVE_CONF_H_ */
