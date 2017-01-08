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
    static uint16_t t_ug;
    t_ug++;
    if (t_ug > 1950)
    {
      DAC_Set(0);
      if (t_ug > 2000)
      {
        t_ug = 0;
      }
    }else
    {
      if (ADC_values[ADC_IN5_Ub] > (uint32_t)(0.7 * 4095/3.3))
      {
        dac_val++;
      }else
      {
        if (dac_val > 0)
        {
          dac_val--;
        }
      }
      DAC_Set(dac_val);
    }
  }
}

void Task_10ms(void)
{
  DebugOut();
  {
	  static uint8_t timer = 200;
	  if (timer == 0)
	  {
		  static uint8_t l3;
		  LED3_Set(l3);
		  l3 = !l3;
		  timer = 200;
	  }else
	  {
		  if ((timer == 50) || (timer == 150))
		  {
		    static uint8_t l4;
        LED4_Set(l4);
        l4 = !l4;
		  }
		  timer--;
	  }
  }
}
