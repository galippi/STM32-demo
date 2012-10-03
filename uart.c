#include "system_conf.h"
#include "gpio.h"

#include "uart.h"

void UART2_Init(void)
{
  GPIO_PortInit_AFOut(GPIOA, 2, 1); /* GPIOA_AFRL.2 = 0001 -> PA2 USART2_TX */
  GPIO_PortInit_AFOut(GPIOA, 3, 1); /* GPIOA_AFRL.3 = 0001 -> PA3 USART2_RX */
  /* enable the USART2, if it was not enabled */
  if (!(RCC->APB1ENR & RCC_APB1ENR_USART2EN))
  {
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
  }
  USART2->CR1 = USART_CR1_TE | USART_CR1_RE; /* TX/RX are enabled */
#if F_SYSTEM == 48000
  USART2->BRR = 192; /* 48MHz / 250kBaud */
#else
#error "USART2_BRR must be adapted for system frquency!"
#endif
  USART2->CR2 = 0; /* 1 stop bit */
  USART2->CR3 = 0; /* default value */
  USART2->CR1 |= USART_CR1_UE; /* USART2 is enabled */
}
