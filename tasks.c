#include "gpio_app.h"

#include "tasks.h"

void UART2_TX(uint8_t data);

void Task_1ms(void)
{
  PB13_Set(!PB13_Get()); /* toggling debug port */
  {
    static uint8_t val;
    UART2_TX(val++);
  }
}
