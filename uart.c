#include "system_conf.h"
#include "gpio.h"
#include "dma.h"
#include "bitfield_lib.h"

#include "uart.h"

#if (CPU_TYPE == CPU_TYPE_STM32F1)
#if PCLK1 == 24000
//#define USART2_BRR ((F_SYSTEM * 1000l * (8 * (2 - USART2_OVER8))) /  9600)
//#define USART2_BRR (1000 << 4) /* testing clock generation of USART */
#define USART2_BRR_9600   ((24000000/ 9600) + 4) /* testing clock generation of USART */
#define USART2_BRR_19200  ((24000000/ 19200) + 2) /* testing clock generation of USART */
#define USART2_BRR_38400  ((24000000/ 38400) + 0) /* testing clock generation of USART */
#define USART2_BRR_115200 ((24000000/115200) + 0) /* testing clock generation of USART */
#else
#error Settings must be adapted for system configuration!
#endif
#endif

void UART2_Init(uint32_t BaudRate)
{
#if (CPU_TYPE == CPU_TYPE_STM32F0)
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
#error "USART2_BRR must be adapted for system frequency!"
#endif
  USART2->CR2 = 0; /* 1 stop bit */
  USART2->CR3 = 0x80; /* DMA is enabled for the channel */
  /* configuring DMA for USART2-TX */
  DMA_Init();
  DMA1_Channel4->CPAR = (uint32_t)&(USART2->TDR);
  DMA1_Channel4->CCR = DMA_CCR_DIR | DMA_CCR_MINC; /* mem2per, no-circ, no-per-inc, mem-inc, psize=8, memsize=8,ch-prio=low, no-mem2mem */
  USART2->CR1 |= USART_CR1_UE; /* USART2 is enabled */
#endif /* CPU_TYPE == CPU_TYPE_STM32F0 */
#if (CPU_TYPE == CPU_TYPE_STM32F1)
  GPIO_PortInit_AFOut(GPIOA, 2); /* GPIOA_AFRL.2 = 0001 -> PA2 USART2_TX */
  GPIO_PortInit_AFOut(GPIOA, 3); /* GPIOA_AFRL.3 = 0001 -> PA3 USART2_RX */
  BitfieldSet(AFIO->MAPR, 3, 1, 0); /* AFIO_MAPR_USART2_REMAP */
  /* enable the USART2, if it was not enabled */
  if (!(RCC->APB1ENR & RCC_APB1ENR_USART2EN))
  {
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
  }
  USART2->CR1 = USART_CR1_UE; /* USART2 is enabled */
  if (BaudRate == 9600)
  {
    USART2->BRR = USART2_BRR_9600;
  }else
  if (BaudRate == 19200)
  {
    USART2->BRR = USART2_BRR_19200;
  }else
  if (BaudRate == 38400)
  {
    USART2->BRR = USART2_BRR_38400;
  }else
  if (BaudRate == 115200)
  {
    USART2->BRR = USART2_BRR_115200;
  }
  else
  { /* wrong baud rate is specified -> error */
    USART2->BRR = USART2_BRR_9600;
  }
  USART2->CR2 = 0; /* 1 stop bit */
#if UART2_DMA == 0
  USART2->CR3 = 0x00; /* DMA is disabled (???) for the channel */
#else
  USART2->CR3 = 0x80; /* DMA is enabled for the channel */
  /* configuring DMA for USART2-TX */
  DMA_Init();
  DMA1_Channel7->CPAR = (uint32_t)&(USART2->DR);
  DMA1_Channel7->CCR = DMA_CCR1_DIR | DMA_CCR1_MINC; /* mem2per, no-circ, no-per-inc, mem-inc, psize=8, memsize=8,ch-prio=low, no-mem2mem */
#endif
  USART2->CR1 |= (USART_CR1_TE | USART_CR1_RE); /* TX/RX are enabled */
#endif /* CPU_TYPE == CPU_TYPE_STM32F0 */
}

#if UART2_DMA == 0
const uint8_t *UART2_TxData = 0;
uint8_t UART2_TxLen = 0;
#endif
uint8_t Rx_buffer[256];
uint8_t rxIdx;
uint32_t Uart2PollCtr;
void UART2_Poll(void)
{
  Uart2PollCtr++;
#if UART2_DMA == 0
  if (UART2_RXNE_Get())
  {
    Rx_buffer[rxIdx++] = USART2->DR;
  }
  if ((UART2_TxLen != 0) && (UART2_TXE_Get() != 0))
  {
    USART2->DR = *UART2_TxData;
    UART2_TxData++;
    UART2_TxLen--;
  }
#endif
}
