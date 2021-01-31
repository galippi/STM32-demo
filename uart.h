#ifndef _UART_H_
#define _UART_H_

#include "controller.h"
#include "uart_conf.h"

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
  DMA1_Channel4->CCR &= ~DMA_CCR1_EN;
  DMA1_Channel4->CMAR = (uint32_t)data;
  DMA1_Channel4->CNDTR = len;
  DMA1_Channel4->CCR |= DMA_CCR1_EN;
#endif
}

void UART1_RxDma_ISR(void);
uint32_t UART1_RX(uint8_t *data, uint32_t len);

void UART2_Init(uint32_t BaudRate);
void UART2_Poll(void);

static inline uint32_t UART2_TXE_Get(void)
{
#if CPU_TYPE == CPU_TYPE_STM32F0
  return USART2->ISR & USART_ISR_TXE;
#elif CPU_TYPE == CPU_TYPE_STM32F1
  return USART2->SR & USART_SR_TXE;
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

#if UART2_DMA == 0
extern const uint8_t *UART2_TxData;
extern uint8_t UART2_TxLen;
#endif
uint8_t UART2_TxOverrun;
static inline void UART2_TX(const uint8_t *data, uint32_t len)
{
#if (CPU_TYPE == CPU_TYPE_STM32F0)
  DMA1_Channel4->CCR &= ~DMA_CCR_EN;
  DMA1_Channel4->CMAR = (uint32_t)data;
  DMA1_Channel4->CNDTR = len;
  DMA1_Channel4->CCR |= DMA_CCR_EN;
#elif (CPU_TYPE == CPU_TYPE_STM32F1)
#if UART2_DMA != 0
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
  if (UART2_TxLen != 0)
  {
    UART2_TxOverrun++;
  }else
  {
    UART2_TxData = data;
    UART2_TxLen = len;
    UART2_Poll();
  }
#endif
#else
  (void)data;
  (void)len;
#endif
}

#endif /* _UART_H_ */
