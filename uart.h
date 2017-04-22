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

static inline uint32_t UART2_RXNE_Get(void)
{
#if CPU_TYPE == CPU_TYPE_STM32F0
  return USART2->ISR & USART_ISR_RXNE;
#elif CPU_TYPE == CPU_TYPE_STM32F1
  return USART2->SR & USART_SR_RXNE;
#elif CPU_TYPE == CPU_TYPE_STM32F4
  return USART2->SR & USART_SR_RXNE;
#else
  return 0;
#endif
}

uint8_t UART2_TxOverrun;
static inline void UART2_TX(const uint8_t *data, uint32_t len)
{
#if (CPU_TYPE == CPU_TYPE_STM32F0)
  DMA1_Channel4->CCR &= ~DMA_CCR_EN;
  DMA1_Channel4->CMAR = (uint32_t)data;
  DMA1_Channel4->CNDTR = len;
  DMA1_Channel4->CCR |= DMA_CCR_EN;
#elif (CPU_TYPE == CPU_TYPE_STM32F1)
  if (DMA1_Channel7->CNDTR != 0)
  {
    UART2_TxOverrun++;
  }else
  { /* resetting Transfer Complete flag */
    //DMA1->IFCR = DMA_ISR_TCIF7;
  }
  DMA1_Channel7->CCR &= ~DMA_CCR1_EN;
  DMA1_Channel7->CMAR = (uint32_t)data;
  DMA1_Channel7->CNDTR = len;
  DMA1_Channel7->CCR |= DMA_CCR1_EN;
#else
  (void)data;
  (void)len;
#endif
}

#endif /* _UART_H_ */
