#include <stdint.h>

#include "tasks.h"

#include "scheduler_preemptive.h"

void Scheduler(void)
{
  Task_1ms();
  {
    static uint8_t TaskTimer;
    if (TaskTimer == 9)
    {
      Task_10ms();
  	  {
      	static uint8_t taskTimer_500ms = 1;
      	if (taskTimer_500ms == 0)
    	  {
      		taskTimer_500ms = 50;
    	  	Task_500ms();
    	  }else
    	  {
      		taskTimer_500ms--;
    	  }
  	  }
    }
    if (TaskTimer < 9)
    {
      TaskTimer++;
    }else
    {
      TaskTimer = 0;
    }
  }
}

void SchedulerPre_TaskManagement(void)
{
}
