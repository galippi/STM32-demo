#include "gpio.h"

#include "dac.h"

DAC_TypeDef * const dac = DAC;

void DAC_Init(void)
{
  /* enable the DAC, if it was not enabled */
  if (!(RCC->APB1ENR & RCC_APB1ENR_DACEN))
  {
    RCC->APB1ENR |= RCC_APB1ENR_DACEN;
  }
  /* Configure GPIOA.4 to analog - DAC is connected to PA.4-5 */
  GPIO_PortInit_Analog(GPIOA, 4);
  DAC->CR = (DAC->CR & 0x303F) | 0x0001;
}
