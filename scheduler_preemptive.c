#include <stdint.h>

#include "timer_app.h"

#include "scheduler_preemptive_conf.h"

#include "scheduler_preemptive.h"

#if (SchedPreTask_EnableCPULoadMeas)
uint8_t CPU_load = 0xFF;
static volatile uint8_t CPU_loadCntr = 0xFF;
static uint16_t CPU_loadStart;
static uint16_t CPU_loadBusy, CPU_loadFree;
#endif

typedef enum {
  SCHED_PRE_TASK_STATE_IDLE,
  SCHED_PRE_TASK_STATE_READY,
  SCHED_PRE_TASK_STATE_RUNNING
}e_SchedPreTask_State;

typedef struct
{
  e_SchedPreTask_State state;
  t_SchedPreTask_Timer timer;
#if SchedPreTask_EnableTaskLoadMeas
  uint8_t taskLoad;
  uint8_t taskLoadMax;
#endif
}t_SchedPreTask_RAM;

typedef struct
{
  void (*func)(void);
  t_SchedPreTask_Timer T; /* period time - 1 */
#if SchedPreTask_EnableTaskLoadMeas
  uint32_t timerClick;
#endif
}t_SchedPreTask_ROM;

#define SCHED_PRE_TASK_DEF(name, T) TASK_ID_##name,
typedef enum
{
  SCHED_PRE_TASK_LIST
  SchedPreTaskNum
}e_SchedPreTask_ID;
#undef SCHED_PRE_TASK_DEF

#if SchedPreTask_EnableTaskLoadMeas
  #define TIMER_CLICK(T) ,((T) * SchedPreTask_LoadMeasResolution)
#else
  #define TIMER_CLICK(T)
#endif

#define SCHED_PRE_TASK_DEF(name, T) \
  { \
    name, \
    (T - 1) \
	TIMER_CLICK(T) \
  },

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
  uint16_t t = getTimer_us();
  SchedPreTask_Disable(); /* disable IT */
  if (SchedPreTask_EnableCPULoadMeas)
  {
    if (CPU_loadCntr == 0xFF)
    {
        CPU_loadCntr = 0;
        CPU_loadStart = t;
    }else
    if (CPU_loadCntr == 0)
    {
      uint16_t dt = t - CPU_loadStart;
      CPU_loadStart = t;
      uint32_t CPU_loadFree_new = (uint32_t)CPU_loadFree + dt;
      if (CPU_loadFree_new > 65535)
      {
        CPU_loadBusy = CPU_loadBusy / 2;
        CPU_loadFree_new = CPU_loadFree_new / 2;
      }
      CPU_loadFree = (uint16_t)CPU_loadFree_new;
      CPU_load = (uint8_t)((200 * (uint32_t)CPU_loadBusy) / ((uint32_t)CPU_loadFree + (uint32_t)CPU_loadBusy));
    }
    CPU_loadCntr++;
  }
  SchedPreTask_Enable(); /* reenable IT */
  for(i = 0; i < SchedPreTaskNum; i++)
  {
    if (SchedPreTask_RAM[i].state == SCHED_PRE_TASK_STATE_RUNNING)
    { /* task is running -> continue it */
      break;
    }else
    {
      if (atomic_check_and_set_u8(SchedPreTask_RAM[i].state, SCHED_PRE_TASK_STATE_READY, SCHED_PRE_TASK_STATE_RUNNING))
      { /* task is ready to running -> start it */
        SchedPreTask_TaskStart(SchedPreTask_ROM[i].func); /* start the task */
        #if SchedPreTask_EnableTaskLoadMeas
        {
            uint16_t dt = getTimer_us() - t;
            uint8_t taskLoad = (uint8_t)((dt * (uint32_t)256) / SchedPreTask_ROM[i].timerClick);
            SchedPreTask_RAM[i].taskLoad = (uint8_t)((SchedPreTask_RAM[i].taskLoad * (uint16_t)7) + taskLoad) / 8;
            if (taskLoad > SchedPreTask_RAM[i].taskLoadMax)
              SchedPreTask_RAM[i].taskLoadMax = taskLoad;
        }
        #endif
        SchedPreTask_RAM[i].state = SCHED_PRE_TASK_STATE_IDLE;
      }else
      { /* if the lock was not succesful, then the lower prio scheduling is running -> it will start the rest of the tasks */
        break;
      }
    }
  }
  SchedPreTask_Disable(); /* disable IT */
  if (SchedPreTask_EnableCPULoadMeas)
  {
    CPU_loadCntr--;
    if (CPU_loadCntr == 0)
    {
      uint16_t t = getTimer_us();
      uint16_t dt = t - CPU_loadStart;
      uint32_t CPU_loadBusy_new = (uint32_t)CPU_loadBusy + dt;
      CPU_loadBusy += dt;
      if (CPU_loadBusy_new > 65535)
      {
        CPU_loadBusy_new = CPU_loadBusy_new / 2;
        CPU_loadFree = CPU_loadFree / 2;
      }
      CPU_loadBusy = (uint16_t)CPU_loadBusy_new;
      CPU_loadStart = t;
    }
  }
  SchedPreTask_Enable(); /* enable IT */
}

void SchedulerPre_LostInterrupt(void)
{
  SchedulerPre_LostInterruptCallBack();
}

uint8_t SchedPreTask_GetTaskLoad(uint32_t taskIdx)
{
#if SchedPreTask_EnableTaskLoadMeas
  return SchedPreTask_RAM[taskIdx].taskLoad;
#else
  return 0xFF;
#endif
}

uint8_t SchedPreTask_GetTaskLoadMax(uint32_t taskIdx)
{
#if SchedPreTask_EnableTaskLoadMeas
  return SchedPreTask_RAM[taskIdx].taskLoadMax;
#else
  return 0xFF;
#endif
}
