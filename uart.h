#ifndef _UART_H_
#define _UART_H_

#include "controller.h"

void UART2_Init(void);

static inline uint32_t UART2_TXE_Get(void)
{
#if TARGET_ECU == TARGET_ECU_STM32F0DISCOVERY
  return USART2->ISR & USART_ISR_TXE;
#else
  return 0;
#endif
}

static inline void UART2_TX(const uint8_t *data, uint32_t len)
{
#if TARGET_ECU == TARGET_ECU_STM32F0DISCOVERY
  DMA1_Channel4->CCR &= ~DMA_CCR_EN;
  DMA1_Channel4->CMAR = (uint32_t)data;
  DMA1_Channel4->CNDTR = len;
  DMA1_Channel4->CCR |= DMA_CCR_EN;
#else
  (void)data;
  (void)len;
#endif
}

#endif /* _UART_H_ */
