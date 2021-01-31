#ifndef _SCHEDULER_PREEMPTIVE_H_
#define _SCHEDULER_PREEMPTIVE_H_

extern uint8_t CPU_load;

void SchedulerPre_Init(void);
void SchedulerPre_TaskTableUpdate(void);
void SchedulerPre_TaskManagement(void);
void SchedulerPre_LostInterrupt(void);

#define SchedulerPre_IsNewTaskStarted() (1)

uint8_t SchedPreTask_GetTaskLoad(uint32_t taskIdx);
uint8_t SchedPreTask_GetTaskLoadMax(uint32_t taskIdx);

#endif /* _SCHEDULER_PREEMPTIVE_H_ */
