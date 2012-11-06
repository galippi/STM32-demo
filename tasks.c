#include "gpio_app.h"
#include "debug.h"
#include "dac.h"
#include "adc_app.h"

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
      DAC_1_Set(0x1FFF - dac_val);
    }else
    {
      DAC_1_Set(dac_val);
    }
    dac_val = (dac_val + 4) & 0x1FFF; /* 1sec rising / 1sec falling edge */
  }
  {
    LED3_Set(!LED3_Get());
  }
}

void Task_10ms(void)
{
  LED4_Set(!LED4_Get());
  DebugOut();
  {
    static uint8_t t_500ms = 0;
    if (t_500ms >= 49)
    {
      t_500ms = 0;
      Task_500ms();
    }else
    {
      t_500ms++;
    }
  }
}

void Task_500ms(void)
{
  LED5_Set(!LED5_Get());
}
