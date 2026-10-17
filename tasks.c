#include <string.h>
#include <stdint.h>

#include "timer.h"

#if 0

#include "SysClock.h"
#include "debug.h"
#include "scheduler_preemptive.h"
#include "version.h"
#include "u32_to_hexstring.h"
#include "adc_app.h"
#include "dma.h"

#ifdef f_USBCLK_Hz
#include "usbd_conf.h"
#endif

#endif

#include "gpio_app.h"
#include "FaultHandler.h"
#include "uart.h"
#include "queue.h"
#include "u32_to_hexstring.h"

#include "tasks.h"

#define LEDR_PORT    GPIOB
#define LEDR_PIN_NUM 14

#define LEDY_PORT    GPIOB
#define LEDY_PIN_NUM 13

#define LEDG_PORT    GPIOB
#define LEDG_PIN_NUM 12

LEDx_Init(R)
LEDx_Set(R)
//LEDx_Get(R)

LEDx_Init(Y)
LEDx_Set(Y)
//LEDx_Get(Y)

LEDx_Init(G)
LEDx_Set(G)
//LEDx_Get(G)

uint8_t uart1RxBuffer[128];
//uint8_t uart1TxBuffer[128];

QUEUE_CREATE(uart2TxQueue, 128);
QUEUE_CREATE(uart2RxQueue, 128);

void Task_Init(void)
{
    UART1_Init(9600, 0);

    queueInit(&uart2TxQueue);
    queueInit(&uart2RxQueue);
    UART2_Init(1200, 0);

    LEDR_Init();
    LEDY_Init();
    LEDG_Init();

    TIM2_Init();

    TIM12_Init();

    GPIO_PortInit_Out(GPIOC, 14);
    GPIO_Set(GPIOC, 14, 1);
    GPIO_Set(GPIOC, 14, 0);
    GPIO_Set(GPIOC, 14, 1);
    GPIO_Set(GPIOC, 14, 0);

    GPIO_PortInit_AFOut(GPIOC, 14, 2); /* PC14 TIM12.CH1 */

#if 0
    DMA_Init(DMA1);


    TIM3_Init();

    ADC_HandlerInit();

    #ifdef f_USBCLK_Hz
    USB_task_init();
    #endif
#endif
}

QUEUE_CREATE(uart1TxQueue, 128);
char uart1TxOverflowCtr1;

static void UART_TX(const void *ptr, uint8_t len)
{
#if 0
    uint32_t written = queueWrite(&uart1TxQueue, ptr, len);
    if (written != len)
        uart1TxOverflowCtr1++;
#else
    static char usart1AbrfSentState = 0;
    if (usart1AbrfSentState == 0)
    {
        if (USART1->ISR & USART_ISR_ABRF_Msk)
        {
            usart1AbrfSentState = 1;
            uint8_t msg[] = {'\n', '\r', 'A', '?', '?', '\n', '\r'};
            msg[3] = ((USART1->ISR & USART_ISR_ABRF_Msk) ? '1' : '0');
            msg[4] = ((USART1->ISR & USART_ISR_ABRE_Msk) ? '1' : '0');
            UART_TX(msg, sizeof(msg));
        }else{
            uint8_t c = 'U';
            UART1_TX_Queue(&c, 1);
        }
    }else{
        UART1_TX_Queue(ptr, len);
    }
#endif
}

uint8_t UART1_TxOverrun;

void Task_Bgnd(void)
{
#if UART1_DMA == 0
    uint8_t data;
    char len = queueRead(&uart1TxQueue, &data, 1);
    if (len)
    {
        UART1_TX(&data, 1);
    }
#endif
    UART2_POLL();
}

static uint32_t halTick_1ms;

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
  halTick_1ms++;
}

extern uint32_t HAL_GetTick(void);

uint32_t HAL_GetTick(void)
{
    return halTick_1ms;
}

extern void HAL_Delay(uint32_t delay);

void HAL_Delay(uint32_t delay)
{
    const uint32_t tStart = HAL_GetTick();
    const uint32_t tEnd = tStart + delay;
    while (HAL_GetTick() < tEnd) {
        // do nothing
    }
}

uint32_t dt;
uint32_t t_last;
uint16_t dt12;
#define T12_SIZE 64
struct {
	uint16_t t0;
	uint16_t t1;
	uint16_t dt;
}dbgT12[T12_SIZE];

void Task_10ms(void)
{
	uint32_t t = TIM2_Cnt_Get();
	if (t_last != 0) {
		uint32_t dtCurrent = t - t_last;
		if (dt == 0)
			dt = dtCurrent;
		else {
#define DT_FILTER 8
			dt = ((dt * (DT_FILTER - 1)) + dtCurrent) / DT_FILTER;
		}
	}
	t_last = t;

	{
		t = TIM2_Cnt_Get();
		uint16_t t12 = TIM12_Cnt_Get();
		while((TIM2_Cnt_Get() - t) < ((1000000 / TIM12_FREQ) / 2)) {
		}
		uint16_t t12New = TIM12_Cnt_Get();
		uint16_t dtCurrent = (t12New << 8) - (t12 << 8);
		dt12 = ((dt12 * (DT_FILTER - 1)) + dtCurrent) / DT_FILTER;
		{
			static uint8_t t12Idx;
			dbgT12[t12Idx].t0 = t12;
			dbgT12[t12Idx].t1 = t12New;
			dbgT12[t12Idx].dt = dtCurrent;
			t12Idx++;
			if (t12Idx == T12_SIZE)
				t12Idx = 0;
		}
	}
	{
		static uint8_t pwm;
		TIMx_CCRy_Set(TIM12, 0, pwm);
		pwm++;
	}
}

typedef enum
{
  e_Pulse_ShortHigh,
  e_Pulse_ShortLow,
  e_Pulse_LongHigh,
  e_Pulse_LongLow,
}e_PulseType;

uint32_t uart2TxOverrun;

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
#if 0
    {
        static uint8_t rxIdx = 0;
        uint8_t len = UART1_RX(uart1RxBuffer + rxIdx, sizeof(uart1RxBuffer) - rxIdx);
        if (len > 0) {
            rxIdx = rxIdx + len;
            if (rxIdx >= sizeof(uart1RxBuffer))
                rxIdx  = 0;
        }
    }
    {
        char adcData[] = "ADCxxxxxxxxxxxxxxxx\r\n";
        U32_to_HexString(adcData +  3, 4, ADC_values_raw[0], '0');
        U32_to_HexString(adcData +  7, 4, ADC_values_raw[1], '0');
        U32_to_HexString(adcData + 11, 4, ADC_values_raw[2], '0');
        U32_to_HexString(adcData + 15, 4, ADC_values_raw[3], '0');
        UART_TX(adcData, sizeof(adcData) - 1);
    }
#endif

    {
        static uint8_t ctr;
        LEDR_Set(ctr & 0x01);
        LEDY_Set(ctr & 0x02);
        LEDG_Set(ctr & 0x04);
        if (ctr < 7)
            ctr++;
        else
            ctr = 0;
    }
#if 1
    //UART2_TX_Char(UART2_RX_Char());
    {
    	char buf[10] = "Demo000**\n";
    	buf[6] = '0' + (uart2TxOverrun & 0x07);
        UART2_TX(buf, sizeof(buf));
    }
    {
        char buf[32] = "Rx:x ";
        uint8_t rxData[12];
        uint8_t len = queueRead(&uart2RxQueue, rxData, sizeof(rxData));
        buf[3] = toHexDigit(len);
        if (len > 0)
            memcpy(buf + 5, rxData, len);
        buf[5 + len] = '\n';
        UART2_TX(buf, 5 + len + 1);
    }
#else
    if (UART2_RXNE_Get())
        UART2_TX_Char(UART2_RX_Char());
    else
        UART2_TX_Char('U');
#endif
}
