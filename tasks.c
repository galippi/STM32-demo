#include <string.h>

#include "gpio_app.h"
#include "debug.h"
#include "adc_app.h"
#include "FaultHandler.h"
#include "uart.h"
#include "scheduler_preemptive.h"
#include "timer_app.h"
#include "can_app.h"
#include "LwSlcan.h"
#include "LwSlcan_conf.h"

#include "tasks.h"

uint8_t uart1RxBuffer[64];
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
    UART1_Init(115200, 1);
    //UART1_Init(38400, 1);
    //UART1_Init(1200, 1);
    GPIO_PortInit_In(   GPIOB,  8); /* CAN1-Rx */
    GPIO_PortInit_AFOut(GPIOB,  9); /* CAN1-Tx */
    AFIO->MAPR = (AFIO->MAPR & ~AFIO_MAPR_CAN_REMAP) | AFIO_MAPR_CAN_REMAP_REMAP2;
    //can1_init();
    slcan_init();
}

void Task_1ms(void)
{
    //if (ADC_values[ADC_IN5_Ub] > (uint32_t)(0.7 * 4095/3.3))
    UART1_TX_Restart();
}

void Task_10ms(void)
{
    ADC_Handler_10ms();
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
#if 0
    {
        static char c = '0';
        UART1_TX(&c, 1);
        //wait_us(15000);
        c++;
        if (c > 'z')
            c = '0';
    }
#endif
}

uint8_t UART1_TxOverrun; // only for debugging
t_LwSlcanDbg dbgLwSlcan;

void Task_Bgrd(void)
{
    ADC_Handler();
    slcan_handler();
}
