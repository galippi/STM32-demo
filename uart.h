#ifndef _UART_H_
#define _UART_H_

void UART2_Init(void);

static inline uint32_t UART2_TXE_Get(void)
{
  return USART2->ISR & USART_ISR_TXE;
}

static inline void UART2_TX(uint8_t data)
{
  USART2->TDR = data;
}

#endif /* _UART_H_ */
