#include "gpio_app.h"
#include "debug.h"
#include "dac.h"

#include "tasks.h"

void Task_Init(void)
{
  DebugOut_Init();
}

void Task_1ms(void)
{
  PB13_Set(!PB13_Get()); /* toggling debug port */
  {
    static uint16_t dac_val = 0;
    if (dac_val >= 0x1000)
    {
      DAC_Set(0x1FFF - dac_val);
    }else
    {
      DAC_Set(dac_val);
    }
    dac_val = (dac_val + 1) & 0x1FFF; /* 1sec rising / 1sec falling edge */
  }
}

void Task_10ms(void)
{
  DebugOut();
}
