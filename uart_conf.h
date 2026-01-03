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

#define USART1SEL RCC_CCIPR_USART1SEL_SYSCLK
#define UART1_DMA 1
#define UART1_TX_QUEUE 1
#define UART1_DMA_RX_BUFFER uart1RxBuffer
#define UART1_OverrunCallback() DBG_INC(UART1_TxOverrun)
#define UART1_RxDma_Update_Overrun() /* do nothing */

typedef uint8_t t_UART1_idx;

#define UART2_DMA 0

#endif /* _UART_CONF_H_ */
