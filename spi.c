#include "gpio.h"
#include "spi.h"
#include "spi_conf.h"
#include "controller.h"

#if (SPI1_DMA != 0) || (SPI2_DMA != 0)
#include "dma.h"
#endif

#if SPI1_DMA == 0
uint8_t *spi1DataPtr;
uint8_t spi1TxIdx;
uint8_t spi1RxIdx;
uint8_t  spi1Len = 0;
#endif /* SPI2_DMA */
uint8_t spi1TxCtr;
uint8_t spi1Overrun;

void SPI1_Init(uint8_t slave, uint8_t spiRemap)
{
#ifdef SPI1_CR1_INIT
  RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
#ifndef SPI1_SS_INIT
  if (spiRemap == 0)
    GPIO_PortInit_AFOut(GPIOA,  4); /* chip-select */
  else
    GPIO_PortInit_AFOut(GPIOA, 15); /* chip-select */
#else /* SPI1_SS_INIT */
  SPI1_SS_INIT();
#endif /* SPI1_SS_INIT */
  if (spiRemap == 0)
  {
    if (slave == 0)
    {
        GPIO_PortInit_AFOut(GPIOA, 5); /* clock */
        GPIO_PortInit_In(GPIOA, 6); /* MISO */
        GPIO_PortInit_AFOut(GPIOA, 7); /* MOSI */
    }else
    {
        GPIO_PortInit_In(GPIOA, 5); /* clock */
        GPIO_PortInit_AFOut(GPIOA, 6); /* MISO */
        GPIO_PortInit_In(GPIOA, 7); /* MOSI */
    }
  }else
  {
    if (slave == 0)
    {
        GPIO_PortInit_AFOut(GPIOB, 3); /* clock */
        GPIO_PortInit_In(GPIOB, 4); /* MISO */
        GPIO_PortInit_AFOut(GPIOB, 5); /* MOSI */
    }else
    {
        GPIO_PortInit_In(GPIOB, 3); /* clock */
        GPIO_PortInit_AFOut(GPIOB, 4); /* MISO */
        GPIO_PortInit_In(GPIOB, 5); /* MOSI */
    }
  }
  SPI1->CR1 = SPI1_CR1_INIT;
  SPI1->CR2 = SPI1_CR2_INIT;
#if SPI1_DMA == 0
  /* DMA is disabled (???) for the channel */
#else /* SPI1_DMA */
  /* SPI1->CR1 |= 0xxx;*/ /* DMA is enabled for the channel */
  /* configuring DMA for SPI1-Tx */
  DMA_Init(DMA1);
  DMA_SPI1_TX->CPAR = (uint32_t)&(SPI1->DR);
  DMA_SPI1_TX->CCR = DMA_CCR1_DIR | DMA_CCR1_MINC; /* mem2per, no-circ, no-per-inc, mem-inc, psize=8, memsize=8,ch-prio=low, no-mem2mem */
  /* configuring DMA for SPI1-Rx */
  DMA_SPI1_RX->CPAR = (uint32_t)&(SPI1->DR);
  DMA_SPI1_RX->CCR =                DMA_CCR1_MINC; /* per2mem, no-circ, no-per-inc, mem-inc, psize=8, memsize=8, ch-prio=low, no-mem2mem */

  /* DMA is turned on for SPI1 */
  SPI1->CR2 |= SPI_CR2_RXDMAEN | SPI_CR2_TXDMAEN;

#endif /* SPI1_DMA */
#endif /* SPI1_CR1_INIT */
}

void SPI1_Poll(void)
{
  #if SPI1_DMA == 0
  uint16_t spi1SR = SPI1->SR;
  if ((spi1SR & SPI_SR_RXNE) && (spi1RxIdx < spi1Len))
  {
    spi1DataPtr[spi1RxIdx++] = SPI1->DR;
    if (spi1RxIdx == spi1Len)
    {
      SPI1_SS_DEACTIVE();
      /* SPI1->CR1 &= ~SPI_CR1_SPE;*/
      spi1Len = 0;
    }
  }
  if ((spi1SR & SPI_SR_TXE) && (spi1TxIdx < spi1Len))
  {
    if (spi1TxIdx == spi1RxIdx)
    { /* send only if the last byte is already captured to compensate the polling speed */
      SPI1->DR = spi1DataPtr[spi1TxIdx++];
    }
  }
  #else /* SPI1_DMA */
  if ((DMA_SPI1_RX->CCR & DMA_CCR1_EN) && ((DMA1->ISR & DMA_ISR_TCIF2) != 0))
  {
    DMA_SPI1_TX->CCR &= ~DMA_CCR1_EN;
    DMA_SPI1_RX->CCR &= ~DMA_CCR1_EN;
    SPI1_SS_DEACTIVE();
  }
  #endif /* SPI1_DMA */
}

void SPI1_Tx(uint8_t *data, uint8_t len)
{
  spi1TxCtr++;
#if SPI1_DMA == 0
  if (spi1Len != 0)
  {
    spi1Overrun++;
  }else
  {
    spi1DataPtr = data;
    spi1Len = len;
    spi1TxIdx = 0;
    spi1RxIdx = 0;
    SPI1_SS_ACTIVE();
    SPI1->CR1 |= SPI_CR1_SPE;
    SPI1_Poll();
  }
#else /* SPI1_DMA */
  if (DMA_SPI1_RX->CCR & DMA_CCR1_EN)
  {
    spi1Overrun++;
  }else
  {
    DMA1->IFCR |= (DMA_ISR_TCIF4 | DMA_ISR_TCIF5); /* clear DMA transfer complete flag */
    /* start DMA channel for SPI1-Tx */
    DMA_SPI1_TX->CMAR = (uint32_t)data;
    DMA_SPI1_TX->CNDTR = len;
    DMA_SPI1_TX->CCR |= DMA_CCR1_EN;

    /* start DMA channel for SPI1-Rx */
    DMA_SPI1_RX->CMAR = (uint32_t)data;
    DMA_SPI1_RX->CNDTR = len;
    DMA_SPI1_RX->CCR |= DMA_CCR1_EN;
    SPI1_SS_ACTIVE();
    SPI1->CR1 |= SPI_CR1_SPE;
  }
#endif /* SPI1_DMA */
}

void SPI2_Init(void)
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
  DMA_SPI2_TX->CPAR = (uint32_t)&(SPI2->DR);
  DMA_SPI2_TX->CCR = DMA_CCR1_DIR | DMA_CCR1_MINC; /* mem2per, no-circ, no-per-inc, mem-inc, psize=8, memsize=8,ch-prio=low, no-mem2mem */
  /* configuring DMA for SPI2-Rx */
  DMA_SPI2_RX->CPAR = (uint32_t)&(SPI2->DR);
  DMA_SPI2_RX->CCR =                DMA_CCR1_MINC; /* per2mem, no-circ, no-per-inc, mem-inc, psize=8, memsize=8, ch-prio=low, no-mem2mem */

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

void SPI2_Poll(void)
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
    SPI2_Poll();
  }
#else /* SPI2_DMA */
  if (DMA1_Channel4->CCR & DMA_CCR1_EN)
  {
    spi2Overrun++;
  }else
  {
    DMA1->IFCR |= (DMA_ISR_TCIF4 | DMA_ISR_TCIF5); /* clear DMA transfer complete flag */
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
