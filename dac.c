#include "gpio.h"

#include "dac.h"

void DAC_Init(void)
{
  /* enable the GPIO-A, if it was not enabled */
  if (!(RCC->APB1ENR & RCC_APB1ENR_DACEN))
  {
    RCC->APB1ENR |= RCC_APB1ENR_DACEN;
  }
  /* enable the GPIO-A, if it was not enabled - DAC is connected to PA.4-5 */
  GPIO_PortInit_Analog(GPIOA, 4);
#ifdef DAC_2_ENABLED
  GPIO_PortInit_Analog(GPIOA, 5);
#endif
  DAC->CR = (DAC->CR & 0x303F) | 0x0001;
}
