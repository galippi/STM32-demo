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
