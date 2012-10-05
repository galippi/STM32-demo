#include "gpio_app.h"
#include "uart.h"

#include "tasks.h"

void Task_Init(void)
{
  DebugOut_Init();
}

void Task_1ms(void)
{
  PB13_Set(!PB13_Get()); /* toggling debug port */
}

void Task_10ms(void)
{
  DebugOut();
}
