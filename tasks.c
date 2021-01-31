#include <string.h>

#include "gpio_app.h"
#include "debug.h"
#include "adc_app.h"
//#include "spi.h"
#include "FaultHandler.h"
//#include "ESP8266.h"
#include "uart.h"
#include "u32_to_hexstring/u32_to_hexstring.h"
#include "scheduler_preemptive.h"
#include "timer_app.h"
#include "pwm.h"

#include "tasks.h"

USART_TypeDef * const uart1 = USART1;
DMA_TypeDef * const dma1 = DMA1;
TIM_TypeDef * const tim2 = TIM2;

uint8_t uart1RxBuffer[128];
uint8_t uart1TxBuffer[128];

void Task_Init(void)
{
  UART1_Init(115200, 1);
  //ESP8266_open();
  TIM2_Init();
  PWM_Init(TIM2, 1);
  GPIO_PortInit_AFOut(GPIOA, 1); /* PA1 PWM2/2 */
  // BitfieldSet(AFIO->MAPR, 2, 1, 0); /* no remap is needed */
  PWM_Set(TIM2, 1, 0);
}

void Task_1ms(void)
{
  /*PB13_Set(!PB13_Get());*/ /* toggling debug port */
  {
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
#if (CPU_TYPE == CPU_TYPE_STM32F0)
      static uint16_t dac_val = 0;
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
#endif
    }
  }
}

uint32_t rxCtr = 0;
uint32_t rxErrorCtr = 0;
uint32_t rxOkCtr = 0;
uint8_t rxLastVal = 'Z';

void Task_10ms(void)
{
  //DebugOut();
  ADC_Handler_10ms();
  {
      uint8_t buf[128];
      uint32_t num = UART1_RX(buf, sizeof(buf));
      for(uint32_t i = 0; i < num; i++)
      {
          rxCtr++;
          uint8_t rxNextVal = rxLastVal;
          if (rxNextVal == 'Z')
              rxNextVal = 'A';
          else
              rxNextVal = rxNextVal + 1;
          if (buf[i] == rxNextVal)
          {
              rxOkCtr++;
          }else
          {
              rxErrorCtr++;
          }
          rxLastVal = buf[i];
      }
  }
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
  {
      static uint16_t pwmVal = 16;
      if (pwmVal < 255)
          pwmVal++;
      else
          pwmVal = 0;
      PWM_Set(TIM2, 1, pwmVal);
  }
}

uint8_t UART1_TxOverrun;
uint8_t tim3_cc3_ctr;
uint8_t tim3_cc4_ctr;
uint16_t encoder;
uint32_t UART1_RxNum;

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
        static uint8_t msgCtr;
        static uint8_t usbDemoLine[] = "Periodic message ctr=xx xx   xx xx xx xx xx xx xx xxxx xx xx xx\r\n";
        U32_to_HexString((char*)usbDemoLine + 21, 2, msgCtr, '0');
        U32_to_HexString((char*)usbDemoLine + 24, 2, UART1_TxOverrun, '0');
        UART1_RxNum = DMA1_Channel5->CNDTR;
        U32_to_HexString((char*)usbDemoLine + 29, 2, UART1_RxNum, '0');
        U32_to_HexString((char*)usbDemoLine + 32, 2, CPU_load, '0');

        U32_to_HexString((char*)usbDemoLine + 35, 2, SchedPreTask_GetTaskLoad(0), '0');
        U32_to_HexString((char*)usbDemoLine + 38, 2, SchedPreTask_GetTaskLoad(1), '0');
        U32_to_HexString((char*)usbDemoLine + 41, 2, SchedPreTask_GetTaskLoad(2), '0');

        U32_to_HexString((char*)usbDemoLine + 44, 2, tim3_cc3_ctr, '0');
        U32_to_HexString((char*)usbDemoLine + 47, 2, tim3_cc4_ctr, '0');

        U32_to_HexString((char*)usbDemoLine + 50, 4, encoder, '0');

        U32_to_HexString((char*)usbDemoLine + 55, 2, SchedPreTask_GetTaskLoadMax(0), '0');
        U32_to_HexString((char*)usbDemoLine + 58, 2, SchedPreTask_GetTaskLoadMax(1), '0');
        U32_to_HexString((char*)usbDemoLine + 61, 2, SchedPreTask_GetTaskLoadMax(2), '0');

        //ESP8266_send(0, sizeof(usbDemoLine)-1, usbDemoLine);
        msgCtr++;
    }
}
