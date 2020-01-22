#include <stdint.h>

#include "scheduler_preemptive_conf.h"

#include "scheduler_preemptive.h"

typedef enum {
  SCHED_PRE_TASK_STATE_IDLE,
  SCHED_PRE_TASK_STATE_READY,
  SCHED_PRE_TASK_STATE_RUNNING
}e_SchedPreTask_State;

typedef struct
{
  e_SchedPreTask_State state;
  t_SchedPreTask_Timer timer;
}t_SchedPreTask_RAM;

typedef struct
{
  void (*func)(void);
  t_SchedPreTask_Timer T; /* period time - 1 */
}t_SchedPreTask_ROM;

#define SCHED_PRE_TASK_DEF(name, T) TASK_ID_##name,
typedef enum
{
  SCHED_PRE_TASK_LIST
  SchedPreTaskNum
}e_SchedPreTask_ID;
#undef SCHED_PRE_TASK_DEF

#define SCHED_PRE_TASK_DEF(name, T) {name, (T - 1)},
const t_SchedPreTask_ROM SchedPreTask_ROM[SchedPreTaskNum] =
{
  SCHED_PRE_TASK_LIST
};
#undef SCHED_PRE_TASK_DEF

t_SchedPreTask_RAM SchedPreTask_RAM[SchedPreTaskNum];

void SchedulerPre_Init(void)
{
  uint32_t i;
  for(i = 0; i < SchedPreTaskNum; i++)
  {
    SchedPreTask_RAM[i].timer = SchedPreTask_ROM[i].T;
    SchedPreTask_RAM[i].state = SCHED_PRE_TASK_STATE_IDLE;
  }
}

void SchedulerPre_TaskTableUpdate(void)
{
  uint32_t i;
  SchedPreTask_Disable(); /* disable IT */
  for(i = 0; i < SchedPreTaskNum; i++)
  {
    if (SchedPreTask_RAM[i].timer == 0)
    { /* if the time is elapsed */
      if (SchedPreTask_RAM[i].state == SCHED_PRE_TASK_STATE_IDLE)
      { /* task is waiting -> set it ready to running */
        SchedPreTask_RAM[i].state = SCHED_PRE_TASK_STATE_READY;
      }else
      { /* task is started -> error management */
        SchedPreTask_ErrorTaskOverrun(i);
      }
      /* reschedule the task */
      SchedPreTask_RAM[i].timer = SchedPreTask_ROM[i].T;
    ;
    }else
    { /* update/decrease the task timer */
      SchedPreTask_RAM[i].timer --;
    }
  }
  SchedPreTask_Enable(); /* reenable IT */
  SchedPreTask_ActivateTask();
}

void SchedulerPre_TaskManagement(void)
{
  uint32_t i;
  for(i = 0; i < SchedPreTaskNum; i++)
  {
    if (SchedPreTask_RAM[i].state == SCHED_PRE_TASK_STATE_RUNNING)
    { /* task is running -> continue it */
      return;
    }else
    {
      if (atomic_check_and_set_u8(SchedPreTask_RAM[i].state, SCHED_PRE_TASK_STATE_READY, SCHED_PRE_TASK_STATE_RUNNING))
      { /* task is ready to running -> start it */
        SchedPreTask_TaskStart(SchedPreTask_ROM[i].func); /* start the task */
        SchedPreTask_RAM[i].state = SCHED_PRE_TASK_STATE_IDLE;
      }else
      { /* if the lock was not succesful, then the lower prio scheduling is running -> it will start the rest of the tasks */
        return;
      }
    }
  }
}
