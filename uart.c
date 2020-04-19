#include "system_conf.h"
#include "gpio.h"
#include "dma.h"
#include "bitfield_lib.h"
#include "SysClock_conf.h"

#include "uart.h"

void UART1_Init(uint32_t baudRate, uint8_t uartRemap)
{
  if (uartRemap == 0)
  {
    GPIO_PortInit_AFOut(GPIOA,  9); /* PA2 USART1_TX */
    GPIO_PortInit_In(GPIOA, 10); /* PA3 USART2_RX */
    BitfieldSet(AFIO->MAPR, 2, 1, 0); /* AFIO_MAPR_USART1_REMAP */
  }else
  {
    GPIO_PortInit_AFOut(GPIOB,  6); /* PA2 USART1_TX */
    GPIO_PortInit_In(GPIOB,  7); /* PA3 USART2_RX */
    BitfieldSet(AFIO->MAPR, 2, 1, 1); /* AFIO_MAPR_USART1_REMAP */
  }
  RCC->APB2ENR |= RCC_APB2ENR_USART1EN;   /* enable the USART1 */
  USART1->CR1 = USART_CR1_TE | USART_CR1_RE; /* TX/RX are enabled */
  USART1->BRR = ((((f_APB2_Hz) / 1) + (baudRate / 2)) / baudRate);
  USART1->CR2 = 0; /* 1 stop bit */
#if UART1_DMA == 0
  USART1->CR3 = 0x00; /* DMA is disabled (???) for the channel */
#else
  USART1->CR3 = 0xC0; /* DMA is enabled for the channel */
  /* configuring DMA for USART1-TX */
  DMA_Init(DMA1);
  DMA1_Channel4->CCR &= ~DMA_CCR1_EN;
  DMA1_Channel4->CPAR = (uint32_t)&(USART1->DR);
  DMA1_Channel4->CCR = DMA_CCR1_DIR | DMA_CCR1_MINC; /* mem2per, no-circ, no-per-inc, mem-inc, psize=8, memsize=8,ch-prio=low, no-mem2mem */
  /* configuring DMA for USART1-RX */
  DMA1_Channel5->CCR &= ~DMA_CCR1_EN;
  DMA1_Channel5->CPAR = (uint32_t)&(USART1->DR);
  DMA1_Channel5->CCR = DMA_CCR1_MINC | DMA_CCR1_CIRC; /* per2mem, circ, no-per-inc, mem-inc, psize=8, memsize=8, ch-prio=low, no-mem2mem */
  DMA1_Channel5->CMAR = (uint32_t)UART1_DMA_RX_BUFFER;
  DMA1_Channel5->CNDTR = sizeof(UART1_DMA_RX_BUFFER);
  DMA1_Channel5->CCR |= DMA_CCR1_EN;
#endif
  USART1->CR1 |= USART_CR1_UE; /* USART1 is enabled */
}

#if (CPU_TYPE == CPU_TYPE_STM32F1)
#if (f_PLL_Hz == 48000000) && (f_APB1_Hz == 24000000)
//#define USART2_BRR ((F_SYSTEM * 1000l * (8 * (2 - USART2_OVER8))) /  9600)
//#define USART2_BRR (1000 << 4) /* testing clock generation of USART */
#define USART2_BRR_9600   ((f_APB1_Hz/  9600) + 4) /* testing clock generation of USART */
#define USART2_BRR_19200  ((f_APB1_Hz/ 19200) + 2) /* testing clock generation of USART */
#define USART2_BRR_38400  ((f_APB1_Hz/ 38400) + 0) /* testing clock generation of USART */
#define USART2_BRR_115200 ((f_APB1_Hz/115200) + 0) /* testing clock generation of USART */
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
  GPIO_PortInit_In(GPIOA, 3); /* GPIOA_AFRL.3 = 0001 -> PA3 USART2_RX */
  BitfieldSet(AFIO->MAPR, 3, 1, 0); /* AFIO_MAPR_USART2_REMAP */
  /* enable the USART2, if it was not enabled */
  if (!(RCC->APB1ENR & RCC_APB1ENR_USART2EN))
  {
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
  }
  USART2->CR1 = USART_CR1_TE | USART_CR1_RE; /* TX/RX are enabled */
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
  USART2->CR3 = 0xC0; /* DMA is enabled for the channel */
  /* configuring DMA for USART2-TX */
  DMA_Init(DMA1);
  DMA1_Channel7->CPAR = (uint32_t)&(USART2->DR);
  DMA1_Channel7->CCR = DMA_CCR1_DIR | DMA_CCR1_MINC; /* mem2per, no-circ, no-per-inc, mem-inc, psize=8, memsize=8,ch-prio=low, no-mem2mem */
  /* configuring DMA for USART2-RX */
  DMA1_Channel6->CCR &= ~DMA_CCR1_EN;
  DMA1_Channel6->CPAR = (uint32_t)&(USART2->DR);
  DMA1_Channel6->CCR = DMA_CCR1_MINC | DMA_CCR1_CIRC; /* per2mem, circ, no-per-inc, mem-inc, psize=8, memsize=8, ch-prio=low, no-mem2mem */
  DMA1_Channel6->CMAR = (uint32_t)Rx_buffer;
  DMA1_Channel6->CNDTR = sizeof(Rx_buffer);
  DMA1_Channel6->CCR |= DMA_CCR1_EN;
#endif
  USART2->CR1 |= USART_CR1_UE; /* USART2 is enabled */
#endif /* CPU_TYPE == CPU_TYPE_STM32F0 */
}

#if UART2_DMA == 0
const uint8_t *UART2_TxData = 0;
uint8_t UART2_TxLen = 0;
#endif
uint8_t Rx_buffer[256];
uint8_t rxIdx;
void UART2_Poll(void)
{
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
#else
  rxIdx = sizeof(Rx_buffer) - DMA1_Channel6->CNDTR;
#endif
}
