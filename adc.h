#ifndef _ADC_H_
#define _ADC_H_

#include "controller.h"

extern const uint16_t *TS_CAL1; /* ADC value of temp. sensor at 30C */
extern const uint16_t *TS_CAL2; /* ADC value of temp. sensor at 110C */
extern const uint16_t *VREFINT_CAL; /* ADC value of reference voltage at 30C */

extern uint8_t ADC_calibration; /* calibration factor */

void ADC_Init(void);

static inline void ADC_Start(void)
{
#if TARGET_ECU == TARGET_ECU_STM32F0DISCOVERY
  ADC1->CR |= ADC_CR_ADSTART;
#endif
}

static inline uint16_t ADC_Get(void)
{
  //return ((ADC1->DR * ADC_calibration) >> 6);
  return ADC1->DR;
}

static inline char ADC_GetStatus(void)
{
#if TARGET_ECU == TARGET_ECU_STM32F0DISCOVERY
  return (ADC1->ISR & ADC_ISR_EOC) != 0; /* give back the state of the end of conversion flag */
#else
  return 0;
#endif
}

#endif /* _ADC_H_ */
