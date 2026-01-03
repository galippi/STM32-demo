#include <string.h>

#include "queue.h"
#include "FaultHandler.h"
#include "debug.h"
#include "uart.h"
#include "uart_app.h"

QUEUE_CREATE(uart1TxQueue, 128); // Todo: to be corrected!!!

void UART_appInit(USART_TypeDef *dev)
{
    if (dev == USART1) {
        if (queueInit(&uart1TxQueue) != 0) {
            CAT_Error(CAT_InvalidParameter, 0);
        }
    }
}

uint32_t UART1_TX_Queue(const void *data, uint32_t len)
{
    uint32_t written = queueWrite(&uart1TxQueue, data, len);
    if (written != len)
        DBG_INC(uart1TxOverflowCtr);
    if (DMA1_Channel4->CNDTR == 0)
        UART1_TxDma_Update();
    return written;
}

void UART1_TxDma_Update(void)
{
    static tQueueIdx num = 0;
    if (num != 0)
        queueRemoveData(&uart1TxQueue, num);
    num = 32;
    tQueueData *buffer = queueGetDataBuffer(&uart1TxQueue, &num);
    if (buffer != NULL) {
        UART1_TX(buffer, num);
    }
}
