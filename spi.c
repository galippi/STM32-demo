#include "spi.h"
#include "spi_conf.h"
#include "controller.h"


void SPI_Init(void)
{
#ifdef SPI2_CR1_INIT
  RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
  SPI2->CR1 = SPI2_CR1_INIT;
  SPI2->CR1 = SPI2_CR2_INIT;
#endif
}

uint8_t *spi2DataPtr;
uint8_t  spi2Len = 0;
uint8_t spi2Overrun = 0;

void SPI_Poll(void)
{
  uint8_t spi2Data = 0;
  uint8_t spi2SR = SPI2->SR;
  if (spi2SR & SPI_SR_RXNE)
  {
    *spi2DataPtr = SPI2->DR;
    spi2DataPtr++;
    spi2Data = 1;
  }
  if (spi2SR & SPI_SR_TXE)
  {
    SPI2->DR = *spi2DataPtr;
    spi2Data = 1;
  }
  if (spi2Data)
  {
    spi2Len--;
  }
}

void SPI2_Tx(uint8_t *data, uint8_t len)
{
  if (spi2Len != 0)
  {
    spi2Overrun++;
  }else
  {
    spi2DataPtr = data;
    spi2Len = len;
    SPI2->CR1 |= SPI_CR1_SPE;
    SPI_Poll();
  }
}
