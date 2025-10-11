#ifndef _SCHEDULER_PREEMPTIVE_H_
#define _SCHEDULER_PREEMPTIVE_H_

#include "scheduler_preemptive_conf.h"

extern uint8_t CPU_load; /** Filtered load in percent */

void SchedulerPre_Init(void);
void SchedulerPre_TaskTableUpdate(void);
void SchedulerPre_TaskManagement(void);
void SchedulerPre_LostInterrupt(void);

#define SchedulerPre_IsNewTaskStarted() (1)

uint8_t SchedPreTask_GetTaskLoad(t_SchedPreTaskIdx taskIdx);
uint8_t SchedPreTask_GetTaskLoadMax(t_SchedPreTaskIdx taskIdx);

#define SCHEDULER_PRE_TASK_IDX_NA 255

extern volatile t_SchedPreTaskIdx SchedulerPre_CurrentTaskIdx;

static t_SchedPreTaskIdx SchedulerPre_GetCurrentTaskIdx(void) {
    return SchedulerPre_CurrentTaskIdx;
}

extern volatile t_SchedPreTaskIdx SchedulerPre_AtomicTaskIdx; /** Current index of the task which holds the lock */

static void SchedulerPre_AtomicBegin(void)
{
    if (SchedulerPre_AtomicTaskIdx != SCHEDULER_PRE_TASK_IDX_NA) {
        SchedulerPre_AtomicInvalidCallBack((SchedulerPre_GetCurrentTaskIdx() << 8) | 0);
        // no return to here
    }
    SchedulerPre_AtomicTaskIdx = SchedulerPre_GetCurrentTaskIdx();
}

static void SchedulerPre_AtomicEnd(void)
{
    if (SchedulerPre_AtomicTaskIdx != SchedulerPre_GetCurrentTaskIdx()) {
        SchedulerPre_AtomicInvalidCallBack((SchedulerPre_GetCurrentTaskIdx() << 8) | 1);
        // no return to here
    }
    SchedulerPre_AtomicTaskIdx = SCHEDULER_PRE_TASK_IDX_NA;
    SchedulerPre_TaskManagement();
}

#endif /* _SCHEDULER_PREEMPTIVE_H_ */
