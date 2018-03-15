#ifndef _ADC_CONF_H_
#define _ADC_CONF_H_

#include "adc_app.h"

#if (CPU_TYPE == CPU_TYPE_STM32F0)
#define ADC_CR_INIT     (ADC_CR_ADCAL | ADC_CR_ADEN)/* ADC calibration and enable it */
#define ADC_CFGR1_INIT  (ADC_CFGR1_DISCEN)
#define ADC_CFGR2_INIT  0x00000000
#define ADC_SMPR_INIT   0x00000007
#define ADC_TR_INIT     0x00000000
/* enable ADC channels: DAC1-out, VBat, temperature sensor, Vref */
#define ADC_CHSELR_INIT (ADC_CHSELR_CHSEL4 |ADC_CHSELR_CHSEL5|ADC_CHSELR_CHSEL6| ADC_CHSELR_CHSEL16 | ADC_CHSELR_CHSEL17 | ADC_CHSELR_CHSEL18)
#define ADC_CCR_INIT    (ADC_CCR_VBATEN | ADC_CCR_TSEN | ADC_CCR_VREFEN)
#define ADC_IER_INIT    0x00000000

#elif (CPU_TYPE == CPU_TYPE_STM32F1)
#define ADC_CR1_INIT ( /* single channel, end of conversion interrupt is disabled */ \
    ADC_CR1_SCAN | \
    0 )
#define ADC_CR2_INIT \
          ADC_CR2_ADON | \
          ADC_CR2_CONT | \
          ADC_CR2_CAL | \
          ADC_CR2_ALIGN_LEFT | \
          ADC_CR2_TSVREFE | /* enable temperature and Vref channels */ \
          ADC_CR2_EXTSEL | /* SWSTART is selected */ \
          ADC_CR2_DMA | /* DMA is configured */ \
          0
#if 0
		  ADC_CR2_CONT | \
		  ADC_CR2_DMA | \

#endif

#define ADC_SMPR2_INIT (\
          ADC_SMPR2_CH(0, ADC_SMPR_CH_239_5) | \
          ADC_SMPR2_CH(1, ADC_SMPR_CH_239_5) | \
          ADC_SMPR2_CH(2, ADC_SMPR_CH_239_5) | \
          ADC_SMPR2_CH(3, ADC_SMPR_CH_239_5) | \
          ADC_SMPR2_CH(4, ADC_SMPR_CH_239_5) | \
          ADC_SMPR2_CH(5, ADC_SMPR_CH_239_5) | \
          ADC_SMPR2_CH(6, ADC_SMPR_CH_239_5) | \
          ADC_SMPR2_CH(7, ADC_SMPR_CH_239_5) | \
          ADC_SMPR2_CH(8, ADC_SMPR_CH_239_5) | \
          ADC_SMPR2_CH(9, ADC_SMPR_CH_239_5) )

#define ADC_SMPR1_INIT (\
          ADC_SMPR1_CH(10, ADC_SMPR_CH_239_5) | \
          ADC_SMPR1_CH(11, ADC_SMPR_CH_239_5) | \
          ADC_SMPR1_CH(12, ADC_SMPR_CH_239_5) | \
          ADC_SMPR1_CH(13, ADC_SMPR_CH_239_5) | \
          ADC_SMPR1_CH(14, ADC_SMPR_CH_239_5) | \
          ADC_SMPR1_CH(15, ADC_SMPR_CH_239_5) | \
          ADC_SMPR1_CH(16, ADC_SMPR_CH_239_5) | \
          ADC_SMPR1_CH(17, ADC_SMPR_CH_239_5) )



#define ADC_SQR2_INIT (\
                       0)
#if 1
#define ADC_SQR1_INIT (ADC_SQR1_Len(6))
#define ADC_SQR3_INIT (\
                       ADC_SQR3_CH(0,  0) | \
                       ADC_SQR3_CH(1,  1) | \
                       ADC_SQR3_CH(2,  8) | \
                       ADC_SQR3_CH(3,  9) | \
                       ADC_SQR3_CH(4, 16) | \
                       ADC_SQR3_CH(5, 17) | \
                       0)
#else
#define ADC_SQR1_INIT (ADC_SQR1_Len(2))
#define ADC_SQR3_INIT (\
                       ADC_SQR3_CH(0,  8) | \
                       ADC_SQR3_CH(1,  8) | \
                       0)

#endif

#define ADC1_DMA_CMAR ((uint32_t)ADC_values)
#define ADC1_DMA_CNDTR (ADC_Ch_Num)

#endif

#endif /* _ADC_CONF_H_ */
