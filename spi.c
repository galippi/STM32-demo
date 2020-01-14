#include "gpio.h"
#include "spi.h"
#include "spi_conf.h"
#include "controller.h"

#if SPI2_DMA != 0
#include "dma.h"
#endif

void SPI_Init(void)
{
#ifdef SPI2_CR1_INIT
  RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
#ifndef SPI2_SS_INIT
  GPIO_PortInit_AFOut(GPIOB, 12); /* chip-select */
#else /* SPI2_SS_INIT */
  SPI2_SS_INIT();
#endif /* SPI2_SS_INIT */
  GPIO_PortInit_AFOut(GPIOB, 13); /* clock */
  GPIO_PortInit_In(GPIOB, 14); /* MISO */
  GPIO_PortInit_AFOut(GPIOB, 15); /* MOSI */
  SPI2->CR1 = SPI2_CR1_INIT;
  SPI2->CR2 = SPI2_CR2_INIT;
#if SPI2_DMA == 0
  /* DMA is disabled (???) for the channel */
#else /* SPI2_DMA */
  /* SPI2->CR1 |= 0xxx;*/ /* DMA is enabled for the channel */
  /* configuring DMA for SPI2-Tx */
  DMA_Init(DMA1);
  DMA1_Channel5->CPAR = (uint32_t)&(SPI2->DR);
  DMA1_Channel5->CCR = DMA_CCR1_DIR | DMA_CCR1_MINC; /* mem2per, no-circ, no-per-inc, mem-inc, psize=8, memsize=8,ch-prio=low, no-mem2mem */
  /* configuring DMA for SPI2-Rx */
  DMA1_Channel4->CPAR = (uint32_t)&(SPI2->DR);
  DMA1_Channel4->CCR =                DMA_CCR1_MINC; /* per2mem, no-circ, no-per-inc, mem-inc, psize=8, memsize=8, ch-prio=low, no-mem2mem */

  /* DMA is turned on for SPI2 */
  SPI2->CR2 |= SPI_CR2_RXDMAEN | SPI_CR2_TXDMAEN;

#endif /* SPI2_DMA */
#endif /* SPI2_CR1_INIT */
}

#if SPI2_DMA == 0
uint8_t *spi2DataPtr;
uint8_t spi2TxIdx;
uint8_t spi2RxIdx;
uint8_t  spi2Len = 0;
#endif /* SPI2_DMA */
uint8_t spi2Overrun = 0;
uint8_t spi2TxCtr;

void SPI_Poll(void)
{
  #if SPI2_DMA == 0
  uint16_t spi2SR = SPI2->SR;
  if ((spi2SR & SPI_SR_RXNE) && (spi2RxIdx < spi2Len))
  {
    spi2DataPtr[spi2RxIdx++] = SPI2->DR;
    if (spi2RxIdx == spi2Len)
    {
      SPI2_SS_DEACTIVE();
      /* SPI2->CR1 &= ~SPI_CR1_SPE;*/
      spi2Len = 0;
    }
  }
  if ((spi2SR & SPI_SR_TXE) && (spi2TxIdx < spi2Len))
  {
    if (spi2TxIdx == spi2RxIdx)
    { /* send only if the last byte is already captured to compensate the polling speed */
      SPI2->DR = spi2DataPtr[spi2TxIdx++];
    }
  }
  #else /* SPI2_DMA */
  /* if ((DMA1_Channel4->CCR & DMA_CCR1_EN) && (DMA1_Channel4->CNDTR == 0)) */
  if ((DMA1_Channel4->CCR & DMA_CCR1_EN) && ((DMA1->ISR & DMA_ISR_TCIF4) != 0))
  {
    DMA1_Channel5->CCR &= ~DMA_CCR1_EN;
    DMA1_Channel4->CCR &= ~DMA_CCR1_EN;
    SPI2_SS_DEACTIVE();
  }
  #endif /* SPI2_DMA */
}

void SPI2_Tx(uint8_t *data, uint8_t len)
{
  spi2TxCtr++;
#if SPI2_DMA == 0
  if (spi2Len != 0)
  {
    spi2Overrun++;
  }else
  {
    spi2DataPtr = data;
    spi2Len = len;
    spi2TxIdx = 0;
    spi2RxIdx = 0;
    SPI2_SS_ACTIVE();
    SPI2->CR1 |= SPI_CR1_SPE;
    SPI_Poll();
  }
#else /* SPI2_DMA */
  if (DMA1_Channel4->CCR & DMA_CCR1_EN)
  {
    spi2Overrun++;
  }else
  {
    DMA1->IFCR = (DMA_ISR_TCIF4 | DMA_ISR_TCIF5); /* clear DMA transfer complete flag */
    /* start DMA channel for SPI2-Tx */
    DMA1_Channel5->CMAR = (uint32_t)data;
    DMA1_Channel5->CNDTR = len;
    DMA1_Channel5->CCR |= DMA_CCR1_EN;

    /* start DMA channel for SPI2-Rx */
    DMA1_Channel4->CMAR = (uint32_t)data;
    DMA1_Channel4->CNDTR = len;
    DMA1_Channel4->CCR |= DMA_CCR1_EN;
    SPI2_SS_ACTIVE();
    SPI2->CR1 |= SPI_CR1_SPE;
  }
#endif /* SPI2_DMA */
}
