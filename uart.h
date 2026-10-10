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

static inline char UART_RX_Char(USART_TypeDef *uart)
{
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
void UART2_Poll(void);

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

static inline char UART2_TX_Char(char d) {
    if (UART2_TXE_Get() != 0)
    {
        USART2->TDR = d;
        return 0;
    }else
        return 1;
}

// User functions shall be implemented by the user application
extern void UART_appInit(USART_TypeDef *dev);

#endif /* _UART_H_ */
