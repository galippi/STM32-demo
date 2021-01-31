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

#include "uart_app.h"

#define UART1_DMA 1
#define UART1_DMA_RX_BUFFER uart1RxBuffer
extern uint8_t UART1_TxOverrun;
#define UART1_OverrunCallback() (UART1_TxOverrun++)
typedef uint8_t t_UART1_idx;

#define UART2_DMA 1

#endif /* _UART_CONF_H_ */
