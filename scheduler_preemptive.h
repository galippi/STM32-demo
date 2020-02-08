#ifndef _SCHEDULER_PREEMPTIVE_H_
#define _SCHEDULER_PREEMPTIVE_H_

extern uint8_t CPU_load;

void SchedulerPre_Init(void);
void SchedulerPre_TaskTableUpdate(void);
void SchedulerPre_TaskManagement(void);

#define SchedulerPre_IsNewTaskStarted() (1)

#endif /* _SCHEDULER_PREEMPTIVE_H_ */
