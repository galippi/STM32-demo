/******************************************************************************
 *
 * \file
 * \ingroup UART
 * \brief   Template sourcefile of Template Component.
 *
 * \details Fill me
 *
 ******************************************************************************/
/** \addtogroup UART
 *  @{
 ******************************************************************************/


#ifndef _UART_CONF_H_
#define _UART_CONF_H_

#include "debug.h"
#include "uart_app.h"
#include "queue.h"

#define UART_MODE_NONE   0
#define UART_MODE_POLLED 1
#define UART_MODE_IRQ    2
#define UART_MODE_DMA    3

#define USART1SEL LL_RCC_USART1_CLKSOURCE_PCLK2
#define UART1_MODE UART_MODE_POLLED
#define UART1_DMA 0
#define UART1_TX_QUEUE 1
#define UART1_DMA_RX_BUFFER uart1RxBuffer
#define UART1_OverrunCallback() DBG_INC(UART1_TxOverrun)
#define UART1_RxDma_Update_Overrun() /* do nothing */

typedef uint8_t t_UART1_idx;

#define USART2SEL LL_RCC_USART2_CLKSOURCE_PCLK1
#define USART2_CR2 (0) /* 1 stop bit */
#define UART2_IS_ENABLED 1
#define UART2_TX_MODE UART_MODE_POLLED
#define UART2_RX_MODE UART_MODE_IRQ
#define UART2_DMA 0
extern uint32_t uart2TxOverrun;
#define UART2_TX_Overrun() (uart2TxOverrun++)
extern const tQueue uart2TxQueue;
#define UART2_TX_QUEUE &uart2TxQueue
extern const tQueue uart2RxQueue;
#define UART2_RX_QUEUE &uart2RxQueue
#define UART2_RX_FIFO_OVERRUN(q) /* do nothing yet */

#endif /* _UART_CONF_H_ */
