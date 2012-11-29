#include "gpio.h"

#include "dac.h"

#if (defined(DAC_1_ENABLED) || defined(DAC_2_ENABLED))
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
#endif /* DAC_2_ENABLED */
#ifdef DAC_1_ENABLED
  DAC->CR = (DAC->CR & 0xFFFF303F) | 0x00000001;
#endif /* DAC_1_ENABLED */
#ifdef DAC_2_ENABLED
  DAC->CR = (DAC->CR & 0x303FFFFF) | 0x00010000;
#endif /* DAC_2_ENABLED */
}
#endif /* (defined(DAC_1_ENABLED) || defined(DAC_2_ENABLED)) */
