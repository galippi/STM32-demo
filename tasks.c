#include <string.h>

#include "gpio_app.h"
#include "debug.h"
#include "adc_app.h"
//#include "spi.h"
#include "FaultHandler.h"
//#include "uart.h"
//#include "uart_app.h"
#include "host_comm.h"
#include "u32_to_hexstring/u32_to_hexstring.h"
#include "scheduler_preemptive.h"
#include "pwm.h"
#include "dht11.h"
#include "timer_app.h"
#include "version.h"
#include "battery.h"

#include "usb_device.h"
#define __STM32F103xB_H
#define __STM32F1XX_H
#include "usbd_cdc_if.h"

#include "tasks.h"

uint8_t uart1RxBuffer[128];
//uint8_t uart1TxBuffer[128];

void Task_Init(void)
{
  TIM2_Init();
  PWM_Init(TIM2, 1);
  //GPIO_PortInit_AFOut(GPIOA, 1); /* PA1 PWM2/2 */
  // BitfieldSet(AFIO->MAPR, 2, 1, 0); /* no remap is needed */
  PWM_Set(TIM2, 1, 0);

  // DHT11
  TIM4_Init();
  dht11_init();
  battery_init();

  MX_USB_DEVICE_Init();
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

void DHT_ResultDebug(uint16_t resultCtr, uint16_t resultChecksumCtr)
{
    char dhtData[] = "DHTxxxxxxxxxxxx\r";
    t_DHT11_Result dhtResult = dht_getResult();
    U32_to_HexString(dhtData + 3, 4, dhtResult.temperature, '0');
    U32_to_HexString(dhtData + 7, 4, dhtResult.humidity, '0');
    U32_to_HexString(dhtData + 11, 2, resultCtr, '0');
    U32_to_HexString(dhtData + 13, 2, resultChecksumCtr, '0');
    hostTx(dhtData, sizeof(dhtData)-1);
}

void Task_10ms(void)
{
  //DebugOut();
  //ADC_Handler_10ms();
    {
        static uint8_t dhtCtr = 0;
        if (dhtCtr == 210) {
            dht11_request();
            dhtCtr = 0;
        }else
            dhtCtr++;
        dht11_run();
    }
  battery_10ms();
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

uint16_t usbItCtr;
uint8_t taskOverrunCtr[5];
uint8_t UART1_TxOverrun;
uint8_t tim3_cc3_ctr;
uint8_t tim3_cc4_ctr;
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
    battery_100ms();
    {
        static uint8_t msgCtr;
        static uint8_t usbDemoLine[] = "Periodic message ctr=xx xx   xx xx xx xx xx xx xx xx xxx xxx xxx xxx xxx xxx *        q\r\n";
        U32_to_HexString((char*)usbDemoLine + 21, 2, msgCtr, '0');
        U32_to_HexString((char*)usbDemoLine + 24, 2, UART1_TxOverrun, '0');
        UART1_RxNum = DMA1_Channel5->CNDTR;
        U32_to_HexString((char*)usbDemoLine + 29, 2, UART1_RxNum, '0');
        U32_to_HexString((char*)usbDemoLine + 32, 2, CPU_load, '0');

        U32_to_HexString((char*)usbDemoLine + 35, 2, SchedPreTask_GetTaskLoad(0), '0');
        U32_to_HexString((char*)usbDemoLine + 38, 2, SchedPreTask_GetTaskLoad(1), '0');
        U32_to_HexString((char*)usbDemoLine + 41, 2, SchedPreTask_GetTaskLoad(2), '0');

        U32_to_HexString((char*)usbDemoLine + 44, 2, SchedPreTask_GetTaskLoadMax(0), '0');
        U32_to_HexString((char*)usbDemoLine + 47, 2, SchedPreTask_GetTaskLoadMax(1), '0');
        U32_to_HexString((char*)usbDemoLine + 50, 2, SchedPreTask_GetTaskLoadMax(2), '0');

        U32_to_HexString((char*)usbDemoLine + 53, 3, ADC_values[0], '0');
        U32_to_HexString((char*)usbDemoLine + 57, 3, ADC_values[1], '0');
        U32_to_HexString((char*)usbDemoLine + 61, 3, ADC_values[2], '0');
        U32_to_HexString((char*)usbDemoLine + 65, 3, ADC_values[3], '0');
        U32_to_HexString((char*)usbDemoLine + 69, 3, ADC_values[4], '0');
        U32_to_HexString((char*)usbDemoLine + 73, 3, ADC_values[5], '0');

        U32_to_HexString((char*)usbDemoLine + 77, 2, tim3_cc3_ctr, '0');

        //ESP8266_send(0, sizeof(usbDemoLine)-1, usbDemoLine);
        //UART1_TX_Queue(usbDemoLine, sizeof(usbDemoLine)-1);
        msgCtr++;
    }
#if 0
    ADC_Handler_10ms();
#else
#endif

    {
        static uint8_t msgCtr;
        uint8_t usbDemoLine[] = "Periodic message ctr=xx xxxx xx xx xx xx xx xx xx xxxx\r\n";
        U32_to_HexString((char*)usbDemoLine + 21, 2, msgCtr, '0');
        U32_to_HexString((char*)usbDemoLine + 24, 4, usbItCtr, '0');
        U32_to_HexString((char*)usbDemoLine + 29, 2, taskOverrunCtr[0], '0');
        U32_to_HexString((char*)usbDemoLine + 32, 2, taskOverrunCtr[1], '0');
        U32_to_HexString((char*)usbDemoLine + 35, 2, taskOverrunCtr[2], '0');
        U32_to_HexString((char*)usbDemoLine + 38, 2, taskOverrunCtr[3], '0');
        U32_to_HexString((char*)usbDemoLine + 41, 2, taskOverrunCtr[4], '0');

        U32_to_HexString((char*)usbDemoLine + 44, 2, tim3_cc3_ctr, '0');
        U32_to_HexString((char*)usbDemoLine + 47, 2, tim3_cc4_ctr, '0');

        //U32_to_HexString((char*)usbDemoLine + 50, 4, encoder, '0');

        CDC_Transmit_FS(usbDemoLine, sizeof(usbDemoLine) - 1);
        msgCtr++;
    }

    //while(1);
}
