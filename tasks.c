#include <string.h>

#include "gpio_app.h"
#include "debug.h"
#include "FaultHandler.h"
#include "scheduler_preemptive.h"
#include "version.h"
#include "timer.h"
#include "uart.h"
#include "u32_to_hexstring.h"
#include "dht11.h"
#include "queue.h"

#ifdef f_USBCLK_Hz
#include "usbd_conf.h"
#endif

#include "tasks.h"

uint8_t uart1RxBuffer[128];
//uint8_t uart1TxBuffer[128];

void Task_Init(void)
{
    UART1_Init(1200, 0);

    TIM3_Init();
    dht11_init();

    #ifdef f_USBCLK_Hz
    USB_task_init();
#endif
}

QUEUE_CREATE(uart1TxQueue, 128);
char uart1TxOverflowCtr1;

static void UART_TX(const void *ptr, uint8_t len)
{
    uint32_t written = queueWrite(&uart1TxQueue, ptr, len);
    if (written != len)
        uart1TxOverflowCtr1++;
}

void Task_Bgnd(void)
{
    uint8_t data;
    char len = queueRead(&uart1TxQueue, &data, 1);
    if (len)
    {
        UART1_TX(&data, 1);
    }
}

void Task_1ms(void)
{
#ifdef f_USBCLK_Hz
  USB_task_1ms();
#endif
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
    UART_TX("\r\n", 2);
    char dhtData[] = "DHTxxxxxxxxxxxx\r\n";
    t_DHT11_Result dhtResult = dht_getResult();
    U32_to_HexString(dhtData + 3, 4, dhtResult.temperature, '0');
    U32_to_HexString(dhtData + 7, 4, dhtResult.humidity, '0');
    U32_to_HexString(dhtData + 11, 2, resultCtr, '0');
    U32_to_HexString(dhtData + 13, 2, resultChecksumCtr, '0');
    //UART1_TX_Queue(dhtData, sizeof(dhtData)-1);
    UART_TX((uint8_t*)dhtData, sizeof(dhtData)-1);
}

void Task_10ms(void)
{
    {
        static uint8_t dhtCtr = 0;
        if (dhtCtr == 210) {
            dht11_request();
            dhtCtr = 0;
        }else
            dhtCtr++;
        dht11_run();
        static uint8_t db = 0; // TODO:
        //GPIO_Set(DHT11_PORT_OUT, db);
        GPIO_Set(GPIOA, 5, db);
        db = 1 - db;
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
    {
        static char usart1AbrfSentState = 0;
        if ((usart1AbrfSentState == 0) && (USART1->ISR & USART_ISR_ABRF_Msk))
        {
            usart1AbrfSentState = 1;
            uint8_t msg[] = {'\n', '\r', 'A', '?', '?', '\n', '\r'};
            msg[3] = ((USART1->ISR & USART_ISR_ABRF_Msk) ? '1' : '0');
            msg[4] = ((USART1->ISR & USART_ISR_ABRE_Msk) ? '1' : '0');
            UART_TX(msg, sizeof(msg));
        }else
        {
            static uint8_t c = 127;
            if (c < 127) {
                UART_TX(&c, 1);
                c++;
            }else{
                c = 0x0D;
                UART_TX(&c, 1);
                c = 0x0A;
                UART_TX(&c, 1);
                c = 32;
            }
        }
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
