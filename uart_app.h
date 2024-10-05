#ifndef _UART_APP_H_
#define _UART_APP_H_

extern uint8_t uart1RxBuffer[128];
//extern uint8_t uart1TxBuffer[128];

//extern uint8_t Rx_buffer[256];
//extern uint8_t rxIdx;
extern uint32_t UART1_TX_Queue(const void *data, uint32_t len);
extern void UART1_TxDma_Update(void);

#endif /* _UART_APP_H_ */
