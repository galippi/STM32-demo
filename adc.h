#ifndef _ADC_H_
#define _ADC_H_

#include "controller.h"

#if (CPU_TYPE == CPU_TYPE_STM32F1)

#define ADC_CR2_ALIGN_LEFT 0
#define ADC_CR2_ALIGN_RIGHT ADC_CR2_ALIGN

/* ADC sample time in ADC cycles */
#define ADC_SMPR_CH___1_5 0
#define ADC_SMPR_CH___7_5 1
#define ADC_SMPR_CH__13_5 2
#define ADC_SMPR_CH__28_5 3
#define ADC_SMPR_CH__41_5 4
#define ADC_SMPR_CH__55_5 5
#define ADC_SMPR_CH__71_5 6
#define ADC_SMPR_CH_239_5 7

#define ADC_SMPR1_CH(ch, SampleTime) ((SampleTime) << (((ch) - 10) * 3))
#define ADC_SMPR2_CH(ch, SampleTime) ((SampleTime) << ( (ch)       * 3))

#define ADC_SQR1_CH(idx, channel) ((channel) << (((idx) - 12) * 5))
#define ADC_SQR1_Len(len) (((len) - 1) << 20)
#define ADC_SQR2_CH(idx, channel) ((channel) << (((idx) -  6) * 5))
#define ADC_SQR3_CH(idx, channel) ((channel) << ( (idx)       * 5))

#endif

extern const uint16_t * const TS_CAL1; /* ADC value of temp. sensor at 30C */
extern const uint16_t * const TS_CAL2; /* ADC value of temp. sensor at 110C */
extern const uint16_t * const VREFINT_CAL; /* ADC value of reference voltage at 30C */

extern uint8_t ADC_calibration; /* calibration factor */

void ADC_Init(void);

static inline void ADC_Start(void)
{
#if TARGET_ECU == TARGET_ECU_STM32F0DISCOVERY
  ADC1->CR |= ADC_CR_ADSTART;
#elif (CPU_TYPE == CPU_TYPE_STM32F1)
  ADC1->CR2 |= (ADC_CR2_SWSTART | ADC_CR2_EXTTRIG);
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
#elif (CPU_TYPE == CPU_TYPE_STM32F1)
  return (ADC1->SR & ADC_SR_EOC) != 0; /* give back the state of the end of conversion flag */
#else
  return 0;
#endif
}

#endif /* _ADC_H_ */
