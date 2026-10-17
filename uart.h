#ifndef _UART_H_
#define _UART_H_

#include "controller.h"
#include "uart_conf.h"
#include "dma_conf.h"

#define RCC_CCIPR_USART1SEL_PCLK   0
#define RCC_CCIPR_USART1SEL_SYSCLK 1
#define RCC_CCIPR_USART1SEL_HSI16  2
#define RCC_CCIPR_USART1SEL_LSE    3

static inline uint32_t UART_RXNE_Get(USART_TypeDef *uart)
{
  return uart->ISR & USART_ISR_RXNE;
}

static inline uint32_t UART_ORE_Get(USART_TypeDef *uart)
{
  return uart->ISR & USART_ISR_ORE;
}

static inline char UART_RX_Char(USART_TypeDef *uart)
{
    if (UART_ORE_Get(uart)) {
        //if (uart->ISR & USART_ISR_EOBF)
        //    uart->ICR = USART_ICR_EOBCF;
        uart->ICR = USART_ICR_ORECF;
        return 'O';
    }else
    if (UART_RXNE_Get(uart))
        return uart->RDR & 0xFF;
    else
        return '?';
}

static inline uint32_t UART_TXE_Get(USART_TypeDef *uart)
{
  return uart->ISR & USART_ISR_TXE;
}

static inline char UART_TX_Char(USART_TypeDef *uart, char d) {
    if (UART_TXE_Get(uart) != 0)
    {
        uart->TDR = d;
        return 0;
    }else
        return 1;
}

void UART1_Init(uint32_t baudRate, uint8_t uartRemap);
void UART1_Poll(void);

static inline void UART1_TX(const uint8_t *data, uint32_t len)
{
#if UART1_DMA != 0
  if (DMA1_Channel4->CNDTR != 0)
  {
    UART1_OverrunCallback();
  }else
  { /* resetting Transfer Complete flag */
    //DMA1->IFCR = DMA_ISR_TCIF7;
  }
  DMA1_Channel_USART1_TX->CCR &= ~DMA_CCR_EN;
  DMA1_Channel_USART1_TX->CMAR = (uint32_t)data;
  DMA1_Channel_USART1_TX->CNDTR = len;
  DMA1_Channel_USART1_TX->CCR |= DMA_CCR_EN;
#else
  do {
      while((USART1->ISR & USART_ISR_TXE_TXFNF) == 0)
          ;
      USART1->TDR = *data;
      data++;
      len--;
  }while (len != 0);
#endif
}

#if UART1_DMA != 0
void UART1_TxDma_ISR(void);
void UART1_RxDma_ISR(void);
#endif

uint32_t UART1_RX(uint8_t *data, uint32_t len);

static inline uint32_t UART1_TXE_Get(void)
{
  return UART_TXE_Get(USART1);
}

static inline uint32_t UART1_RXNE_Get(void)
{
  return UART_RXNE_Get(USART1);
}

static inline char UART1_RX_Char(void)
{
    return UART_RX_Char(USART1);
}

void UART2_Init(uint32_t BaudRate, uint8_t uartRemap);

static inline uint32_t UART2_TXE_Get(void)
{
  return UART_TXE_Get(USART2);
}

static inline uint32_t UART2_RXNE_Get(void)
{
  return UART_RXNE_Get(USART2);
}

static inline char UART2_RX_Char(void)
{
    return UART_RX_Char(USART2);
}

static inline char UART2_TX_Char(char d) {
    if (UART2_TXE_Get() != 0)
    {
        USART2->TDR = d;
        return 0;
    }else
        return 1;
}

#if (UART2_TX_MODE == UART_MODE_POLLED) && defined(UART2_TX_QUEUE)
static void UART2_TX_poll(void)
{
	tQueueIdx num = 8; // number to copy data in one step
	tQueueData *data = queueGetDataBuffer(UART2_TX_QUEUE, &num);
	tQueueIdx processed = 0;
	while (num != 0) {
		if (UART2_TX_Char(*data) == 0) {
			data++;
			processed++;
			num--;
			if (num == 0)
				break;
		}
	}
	if (processed != 0)
		queueRemoveData(UART2_TX_QUEUE, processed);
}
#define UART2_TX_POLL() UART2_TX_poll()
#endif

static inline void UART2_TX(const void *dataP, uint32_t len)
{
#if (UART2_TX_MODE == UART_MODE_POLLED)
#if defined(UART2_TX_QUEUE)
	const uint8_t *data = dataP;
	if (queueGetNum(UART2_TX_QUEUE) == 0) {
		while (UART2_TX_Char(*data) == 0) {
			data++;
			len--;
			if (len == 0)
				break;
		}
	}
    if (len != 0) {
        if (queueWrite(UART2_TX_QUEUE, data, len) != len)
            UART2_TX_Overrun();
    }
#else
	const uint8_t *data = dataP;
	while (len != 0) {
		if (UART2_TX_Char(*data))
			UART2_TX_Overrun();
		data++;
		len--;
	}
#endif
#else
#error Not yet implemented!
#endif
}

#if (UART2_RX_MODE == UART_MODE_POLLED) && defined(UART2_RX_QUEUE)

static void UART2_RX_poll(void)
{
	while (queueIsFull(UART2_RX_QUEUE) == 0) {
		if (UART_ORE_Get(USART2)) {
			USART2->ICR = USART_ICR_ORECF;
			UART2_RX_FIFO_OVERRUN(UART2_RX_QUEUE);
		}
		if (UART_RXNE_Get(USART2) != 0) {
			char c = USART2->RDR & 0xFF;
			queuePut(UART2_RX_QUEUE, c);
		}else
			return;
	}
}
#define UART2_RX_POLL() UART2_RX_poll()
#endif

#if defined(UART2_TX_POLL) || defined(UART2_RX_POLL)
void UART2_Poll(void);
#define UART2_POLL() UART2_Poll()
#endif

// User functions shall be implemented by the user application
extern void UART_appInit(USART_TypeDef *dev);

#endif /* _UART_H_ */
