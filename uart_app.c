#include "queue.h"
#include "uart.h"
#include "uart_app.h"

QUEUE_CREATE(uart1TxQueue, 128);

uint8_t uart1TxOverflowCtr;

uint32_t UART1_TX_Queue(const void *data, uint32_t len)
{
    uint32_t written = queueWrite(&uart1TxQueue, data, len);
    if (written != len)
        uart1TxOverflowCtr++;
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
