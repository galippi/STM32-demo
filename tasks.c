#include <string.h>

#include "gpio_app.h"
#include "debug.h"
#include "FaultHandler.h"
#include "scheduler_preemptive.h"
#include "version.h"
#include "timer.h"
#include "uart.h"

#include "tasks.h"

uint8_t uart1RxBuffer[128];
//uint8_t uart1TxBuffer[128];

void Task_Init(void)
{
    UART1_Init(1200, 0);
}

void Task_1ms(void)
{
  /*PB13_Set(!PB13_Get());*/ /* toggling debug port */
  {
    static uint16_t t_ug;
    if (t_ug < 500)
    {
        t_ug++;
    }
  }
}

void Task_10ms(void)
{
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
    {
        static uint8_t c = 32;
        UART1_TX(&c, 1);
        c++;
        if (c > 127)
            c = 32;
    }
    {
        static uint8_t rxIdx = 0;
        uint8_t len = UART1_RX(uart1RxBuffer + rxIdx, sizeof(uart1RxBuffer) - rxIdx);
        if (len > 0) {
            rxIdx = rxIdx + len;
            if (rxIdx >= sizeof(uart1RxBuffer))
                rxIdx  = 0;
        }
    }
}
