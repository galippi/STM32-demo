#include "gpio_app.h"

#include "tasks.h"

void Task_1ms(void)
{
  PB13_Set(!PB13_Get()); /* toggling debug port */
}
