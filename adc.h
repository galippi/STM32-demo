#ifndef _ADC_H_
#define _ADC_H_

#include "controller.h"

#define ADC_CFGR1_DMNGT_REGULAR      0
#define ADC_CFGR1_DMNGT_DMA_ONE_SHOT (ADC_CFGR1_DMNGT_0)
#define ADC_CFGR1_DMNGT_RESERVED     (ADC_CFGR1_DMNGT_1)
#define ADC_CFGR1_DMNGT_CIRCULAR     (ADC_CFGR1_DMNGT_0 | ADC_CFGR1_DMNGT_1)

#define ADC_CR2_ALIGN_LEFT 0
#define ADC_CR2_ALIGN_RIGHT ADC_CR2_ALIGN

/* ADC sample time in ADC cycles */
#define ADC_SMPR_CH___3 0
#define ADC_SMPR_CH___5 1
#define ADC_SMPR_CH___8 2
#define ADC_SMPR_CH__13 3
#define ADC_SMPR_CH__25 4
#define ADC_SMPR_CH__48 5
#define ADC_SMPR_CH_139 6
#define ADC_SMPR_CH_289 7

#define ADC_SMPR1_CH(ch, SampleTime) ((SampleTime) << (((ch)     ) * 3))
#define ADC_SMPR2_CH(ch, SampleTime) ((SampleTime) << (((ch) - 10) * 3))

#define ADC_SQR1_Len(len) (((len) - 1) << 0)
#define ADC_SQR1_CH(idx, channel) ((channel) << ((((idx) -  0) * 6) + 6))
#define ADC_SQR2_CH(idx, channel) ((channel) <<  (((idx) -  4) * 6))
#define ADC_SQR3_CH(idx, channel) ((channel) <<  (((idx) -  9) * 6))
#define ADC_SQR4_CH(idx, channel) ((channel) <<  (((idx) - 14) * 6))

extern const uint16_t * const TS_CAL__30; /* ADC value of temp. sensor at  30C */
extern const uint16_t * const TS_CAL_140; /* ADC value of temp. sensor at 140C */

#define V_REF_INT      1.217 /* V */
#define V_REF_INT_VDDA 3.300 /* V */
extern const uint16_t * const VREFINT_CAL; /* ADC value of reference voltage at 30C */

extern uint8_t ADC_calibration; /* calibration factor */

void ADC_Init(void);

static inline void ADC_Start(void)
{
  ADC1->CR |= ADC_CR_ADSTART;
}

static inline uint16_t ADC_Get(void)
{
  //return ((ADC1->DR * ADC_calibration) >> 6);
  return ADC1->DR;
}

static inline char ADC_GetStatus(void)
{
  return (ADC1->ISR & ADC_ISR_EOC) != 0; /* give back the state of the end of conversion flag */
}

void DmaAdc1_Isr(void);

#endif /* _ADC_H_ */
