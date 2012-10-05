#ifndef _UART_H_
#define _UART_H_

#include "stm32f0xx.h"

void UART2_Init(void);

static inline uint32_t UART2_TXE_Get(void)
{
  return USART2->ISR & USART_ISR_TXE;
}

static inline void UART2_TX(const uint8_t *data, uint32_t len)
{
  DMA1_Channel4->CMAR = data;
  DMA1_Channel4->CNDTR = len;
  DMA1_Channel4->CCR |= DMA_CCR_EN;
}

#endif /* _UART_H_ */
