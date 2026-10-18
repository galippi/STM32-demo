#ifndef _ADC_CONF_H_
#define _ADC_CONF_H_

#include "adc_app.h"

#define ADC_CR_INIT    (ADC_CR_ADCAL | ADC_CR_ADVREGEN | ADC_CR_ADEN)/* ADC calibration and enable it */
//#define ADC_CFGR1_INIT  (ADC_CFGR1_CONT | ADC_CFGR1_DMACFG | ADC_CFGR1_DMAEN)
#define ADC_CFGR1_INIT  (ADC_CFGR1_DISCEN)
#define ADC_CFGR2_INIT  0x00000000
#define ADC_SMPR1_INIT  ADC_SMPR1_CH(0, ADC_SMPR_CH_289) | \
                        ADC_SMPR1_CH(1, ADC_SMPR_CH_289) | \
                        ADC_SMPR1_CH(2, ADC_SMPR_CH_289) | \
                        ADC_SMPR1_CH(3, ADC_SMPR_CH_289) | \
                        ADC_SMPR1_CH(4, ADC_SMPR_CH_289) | \
                        ADC_SMPR1_CH(5, ADC_SMPR_CH_289) | \
                        0
#define ADC_SMPR2_INIT  0x00000000
#define ADC_TR_INIT     0x00000000

#define ADC1_SQR1_INIT (ADC_SQR1_Len(ADC_Ch_Num) | \
                        ADC_SQR1_CH(0, 12) | \
                        ADC_SQR1_CH(1, 13) | \
                        ADC_SQR1_CH(2,  0) | \
                        ADC_SQR1_CH(3,  1) | \
                         0)
#define ADC1_SQR2_INIT ( \
                        ADC_SQR2_CH(4,  6) | \
                        ADC_SQR2_CH(5,  7) | \
                        0)
#define ADC1_SQR3_INIT 0
#define ADC1_SQR4_INIT 0
#define ADC1_PCSEL_INIT 0x0C3 // see above

#define ADCC_CCR_INIT   (ADCC_CCR_TSEN | ADCC_CCR_VREFEN)
#define ADC_IER_INIT    0x00000000

extern uint16_t ADC_values_raw[ADC_Ch_Num];
//#define ADC1_DMA_CMAR ((uint32_t)ADC_values_raw)
//#define ADC1_DMA_CNDTR (ADC_Ch_Num)

#endif /* _ADC_CONF_H_ */
