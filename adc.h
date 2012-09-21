#ifndef _ADC_H_
#define _ADC_H_

#include "stm32f0xx.h"

extern const uint16_t *TS_CAL1; /* ADC value of temp. sensor at 30C */
extern const uint16_t *TS_CAL2; /* ADC value of temp. sensor at 110C */
extern const uint16_t *VREFINT_CAL; /* ADC value of reference voltage at 30C */

void ADC_Init(void);

static inline void ADC_Start(void)
{
  ADC1->CR |= ADC_CR_ADSTART;
}

static inline uint16_t ADC_Get(void)
{
  return ADC1->DR;
}

static inline char ADC_GetStatus(void)
{
  return (ADC1->ISR & ADC_ISR_EOC) != 0; /* give back the state of the end of conversion flag */
}

#endif /* _ADC_H_ */
