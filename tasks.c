#include <string.h>

#include "gpio_app.h"
#include "debug.h"
#include "adc_app.h"
//#include "spi.h"
#include "FaultHandler.h"
#include "uart.h"
#include "u32_to_hexstring/u32_to_hexstring.h"
#include "scheduler_preemptive.h"
#include "timer_app.h"
#include "spi.h"
#include "can_app.h"

#include "tasks.h"

uint8_t uart1RxBuffer[16];
uint8_t uart1TxBuffer[128];

void Task_Init(void)
{
  LED3_Init();
  LED3_Set(0);
  Button1_Init();
  ADC_HandlerInit();
  //UART2_Init();
  //SPI_Init();
  //DebugOut_Init();
  UART1_Init(38400, 1);
  SPI2_Init();
  GPIO_PortInit_In(   GPIOB,  8); /* CAN1-Rx */
  GPIO_PortInit_AFOut(GPIOB,  9); /* CAN1-Tx */
  AFIO->MAPR = (AFIO->MAPR & ~AFIO_MAPR_CAN_REMAP) | AFIO_MAPR_CAN_REMAP_REMAP2;
  can1_init();
}

void Task_1ms(void)
{
  //if (ADC_values[ADC_IN5_Ub] > (uint32_t)(0.7 * 4095/3.3))
}

uint8_t spi2Buf[12];
uint8_t CAN_rxCtr;
uint8_t CAN_txCtr;
uint8_t CAN_txFull;
void Task_10ms(void)
{
  //DebugOut();
  ADC_Handler_10ms();
  {
    static uint8_t SD_state;
    static uint8_t NCR;
    switch (SD_state)
    {
      case 0:
      {
        static const uint8_t spiData[sizeof(spi2Buf)] = { 0x40, 0x00, 0x00, 0x00, 0x00, 0x95, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
        memcpy(spi2Buf, spiData, sizeof(spi2Buf));
        SPI2_Tx(spi2Buf, sizeof(spi2Buf));
        SD_state++;
        break;
      }
      case 1:
      {
        for(uint32_t i = 6; i < sizeof(spi2Buf); i++)
        {
          if (spi2Buf[i] == 0x01)
          {
            NCR = i - 5;
          }
        }
        static const uint8_t spiData[sizeof(spi2Buf)] = { 0x48, 0x00, 0x00, 0x01, 0xAA, 0x87, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
        memcpy(spi2Buf, spiData, sizeof(spi2Buf));
        SPI2_Tx(spi2Buf, 6 + NCR);
        SD_state++;
        break;
      }
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
		  timer--;
	  }
  }
  {
    CAN_msg msgTx = { .id = 0x8000F200, .dlc = 8};
    msgTx.data.data32[0] = 0x11224466;
    msgTx.data.data32[1] = 0xFEDCBA98;
    if (CAN_STM32_tx(&msgTx))
      CAN_txCtr++;
    else
      CAN_txFull++;
  }
  {
	{
	  CAN_msg msgTx = { .id = 0x100, .dlc = 8};
	  msgTx.data.data32[0] = 0x01234567;
	  msgTx.data.data32[1] = 0xFEDCBA98;
	  if (CAN_STM32_tx(&msgTx))
		CAN_txCtr++;
	  else
		CAN_txFull++;
	}
  }

  {
      CAN_msg msg;
      while (CAN_STM32_rx(&msg))
      {
        CAN_rxCtr++;
        {
          CAN_msg msgTx = { .id = 0x101, .dlc = 8};
          msgTx.data.data32[0] = msg.id;
          msgTx.data.data8[4] = msg.dlc;
          msgTx.data.data8[5] = msg.data.data8[0];
          msgTx.data.data8[6] = msg.data.data8[1];
          msgTx.data.data8[7] = msg.data.data8[2];
          if (CAN_STM32_tx(&msgTx))
            CAN_txCtr++;
          else
            CAN_txFull++;
        }
      }
  }

  //wait_us(4500);
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
        static uint8_t usbDemoLine[] = "Periodic message ctr=xx xx   xx xx xx xx xx xx xx xxxx xx xx xx xx xx xx\r\n";
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

        //U32_to_HexString((char*)usbDemoLine + 64, 2, spi1_isrCtr, '0');
        U32_to_HexString((char*)usbDemoLine + 67, 2, spi2_isrCtr, '0');
        U32_to_HexString((char*)usbDemoLine + 70, 2, spi2_errCtr, '0');

        UART1_TX(usbDemoLine, sizeof(usbDemoLine)-1);
        msgCtr++;
    }
}

void Task_Bgrd(void)
{
  ADC_Handler();
  SPI1_Poll();
  SPI2_Poll();
}
