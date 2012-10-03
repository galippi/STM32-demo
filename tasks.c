#include "gpio_app.h"
#include "uart.h"

#include "tasks.h"

void Task_1ms(void)
{
  PB13_Set(!PB13_Get()); /* toggling debug port */
  {
    static uint8_t val;
    UART2_TX(val++);
  }
}
