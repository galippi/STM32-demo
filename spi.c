#include "gpio.h"
#include "spi.h"
#include "spi_conf.h"
#include "controller.h"


void SPI_Init(void)
{
#ifdef SPI2_CR1_INIT
  RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
  GPIO_PortInit_AFOut(GPIOB, 12); /* chip-select */
  GPIO_PortInit_AFOut(GPIOB, 13); /* clock */
  GPIO_PortInit_In(GPIOB, 14); /* MISO */
  GPIO_PortInit_AFOut(GPIOB, 15); /* MOSI */
  SPI2->CR1 = SPI2_CR1_INIT;
  SPI2->CR2 = SPI2_CR2_INIT;
#endif
}

uint8_t *spi2DataPtr;
uint8_t spi2TxIdx;
uint8_t spi2RxIdx;
uint8_t  spi2Len = 0;
uint8_t spi2Overrun = 0;
uint8_t spi2TxCtr;

void SPI_Poll(void)
{
  uint16_t spi2SR = SPI2->SR;
  if ((spi2SR & SPI_SR_RXNE) && (spi2RxIdx < spi2Len))
  {
    spi2DataPtr[spi2RxIdx++] = SPI2->DR;
    if (spi2RxIdx == spi2Len)
    {
      SPI2->CR1 &= ~SPI_CR1_SPE;
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
}

void SPI2_Tx(uint8_t *data, uint8_t len)
{
  spi2TxCtr++;
  if (spi2Len != 0)
  {
    spi2Overrun++;
  }else
  {
    spi2DataPtr = data;
    spi2Len = len;
    spi2TxIdx = 0;
    spi2RxIdx = 0;
    SPI2->CR1 |= SPI_CR1_SPE;
    SPI_Poll();
  }
}
