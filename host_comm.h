#ifndef _HOST_COMM_H_
#define _HOST_COMM_H_

#if 0

#define hostInit() /* UART1_Init(9600, 1) */
#define hostTx(dataPtr, len) /* UART1_TX_Queue(dataPtr, len) */
#define hostRx(dataPtr, bufLen) 0 /* UART1_RX(dataPtr, bufLen) */

#elif 1

#include "uart.h"

#define hostInit() UART1_Init(9600, 1)
#define hostTx(dataPtr, len) UART1_TX_Queue(dataPtr, len)
#define hostRx(dataPtr, bufLen) UART1_RX(dataPtr, bufLen)

#else

#define hostInit() /* UART1_Init(9600, 1) */
#define hostTx(dataPtr, len) /* UART1_TX_Queue(dataPtr, len) */
#define hostRx(dataPtr, bufLen) /* UART1_RX(dataPtr, bufLen) */

#endif

#endif /* _HOST_COMM_H_ */
