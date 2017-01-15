#include "gpio_app.h"
#include "debug.h"
#include "adc_app.h"
#include "FaultHandler.h"

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
		  //LED3_Set(l3);
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

typedef enum
{
  e_Pulse_ShortHigh,
  e_Pulse_ShortLow,
  e_Pulse_LongHigh,
  e_Pulse_LongLow,
}e_PulseType;

void Task_500ms(void)
{
	static const e_PulseType pulseConfig[] =
	{
		e_Pulse_LongLow,
		e_Pulse_ShortHigh, e_Pulse_ShortLow,
		e_Pulse_ShortHigh, e_Pulse_ShortLow,
		e_Pulse_ShortHigh, e_Pulse_ShortLow,
		e_Pulse_LongHigh, e_Pulse_ShortLow,
		e_Pulse_LongHigh, e_Pulse_ShortLow,
		e_Pulse_LongHigh, e_Pulse_ShortLow,
		e_Pulse_ShortHigh, e_Pulse_ShortLow,
		e_Pulse_ShortHigh, e_Pulse_ShortLow,
		e_Pulse_ShortHigh, e_Pulse_ShortLow,
	};
	static uint8_t pulseIdx = 0;
	static uint8_t pulseTimer = 0;
	if (pulseTimer == 0)
	{
		if (pulseIdx < ((sizeof(pulseConfig)/sizeof(pulseConfig[0])) - 1))
		{
			pulseIdx++;
		}else
		{
			pulseIdx = 0;
		}
		switch(pulseConfig[pulseIdx])
		{
		case e_Pulse_ShortHigh:
		  LED3_Set(0);
		  pulseTimer = 0;
			break;
		case e_Pulse_ShortLow:
		  LED3_Set(1);
		  pulseTimer = 0;
			break;
		case e_Pulse_LongHigh:
		  LED3_Set(0);
		  pulseTimer = 2;
			break;
		case e_Pulse_LongLow:
		  LED3_Set(1);
		  pulseTimer = 2;
			break;
		default:
                  CAT_Error(CAT_VarInvalidValue, 0);
                  break;
		}
	}else
	{
		pulseTimer--;
	}
}
