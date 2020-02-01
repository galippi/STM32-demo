#include <string.h>

#include "gpio_app.h"
#include "debug.h"
#include "adc_app.h"
#include "spi.h"
#include "FaultHandler.h"
#include "bluetooth_hc05.h"
#include "timer.h"
#include "timer_app.h"
#include "usb/usb_device.h"
#include "usb_if.h"
#include "vcom_if.h"
#include "u32_to_hexstring.h"

#include "tasks.h"

void Task_Init(void)
{
  SPI_Init();
  //DebugOut_Init();
  MX_USB_DEVICE_Init();
}

void Task_1ms(void)
{
  /*PB13_Set(!PB13_Get());*/ /* toggling debug port */
  static uint8_t var;
  Debug1_Set(var);
  var = !var;
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
#if 0
  uint16_t t_start = getTimer_us();
  while (((_USB_ReadInterrupts()) != 0) && (((getTimer_us() - t_start) & 0xFFFF) < 600))
  {
    USB_LP_CAN1_RX0_IRQHandler();
  }
#endif
}

void Task_2ms(void)
{
}

void Task_5ms(void)
{
}

void Task_10ms(void)
{
  Debug0_Set(1);
  //DebugOut();
  Bluetooth_Task_10ms();
  ADC_Handler_10ms();
  {
    static const uint8_t spiData[] = { 0x00, 0x39, 0x00, 0x5A};
    static uint8_t spiBuf[sizeof(spiData)];
    memcpy(spiBuf, spiData, sizeof(spiBuf));
    SPI2_Tx(spiBuf, sizeof(spiBuf));
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

  wait_us(1500);
  Debug0_Set(0);
}

uint16_t usbItCtr;
uint8_t taskOverrunCtr[5];

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
        uint8_t usbDemoLine[] = "Periodic message ctr=xx xxxx xx xx xx xx xx\r\n";
        U32_to_HexString((char*)usbDemoLine + 21, 2, msgCtr, '0');
        U32_to_HexString((char*)usbDemoLine + 24, 4, usbItCtr, '0');
        U32_to_HexString((char*)usbDemoLine + 29, 2, taskOverrunCtr[0], '0');
        U32_to_HexString((char*)usbDemoLine + 32, 2, taskOverrunCtr[1], '0');
        U32_to_HexString((char*)usbDemoLine + 35, 2, taskOverrunCtr[2], '0');
        U32_to_HexString((char*)usbDemoLine + 38, 2, taskOverrunCtr[3], '0');
        U32_to_HexString((char*)usbDemoLine + 41, 2, taskOverrunCtr[4], '0');
        CDC_Transmit_FS(usbDemoLine, sizeof(usbDemoLine) - 1);
        msgCtr++;
    }
}
