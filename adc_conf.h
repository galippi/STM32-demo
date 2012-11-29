#ifndef _ADC_CONF_H_
#define _ADC_CONF_H_

#if TARGET_ECU == TARGET_ECU_STM32F0DISCOVERY
/* port definition for STM32F0-discovery board */
#define ADC_CR_INIT     (ADC_CR_ADCAL | ADC_CR_ADEN)/* ADC calibration and enable it */
#define ADC_CFGR1_INIT  (ADC_CFGR1_DISCEN)
#define ADC_CFGR2_INIT  0x00000000
#define ADC_SMPR_INIT   0x00000007
#define ADC_TR_INIT     0x00000000
/* enable ADC channels: DAC1-out, VBat, temperature sensor, Vref */
#define ADC_CHSELR_INIT (ADC_CHSELR_CHSEL4 |ADC_CHSELR_CHSEL5|ADC_CHSELR_CHSEL6| ADC_CHSELR_CHSEL16 | ADC_CHSELR_CHSEL17 | ADC_CHSELR_CHSEL18)
#define ADC_CCR_INIT    (ADC_CCR_VBATEN | ADC_CCR_TSEN | ADC_CCR_VREFEN)
#define ADC_IER_INIT    0x00000000
#elif TARGET_ECU == TARGET_ECU_STM32F4DISCOVERY
/* port definition for STM32F4-discovery board */
#define ADC_CR1_INIT    ((0) /* 12 bit resolution */ | \
                         (ADC_CR1_DISCNUM_0 | ADC_CR1_DISCNUM_1 | ADC_CR1_DISCNUM_2) /* 8 channels Discontinuous mode */ | \
                         ADC_CR1_SCAN /* Scan mode on regular channels enabled */ | \
                        0)
#define ADC_CR2_INIT    ((0) /* right alignment */ | \
                         ADC_CR2_ADON /* enable ADC */ | \
                        0)
#define ADC_CCR_INIT    (ADC_CCR_VBATE | ADC_CCR_TSVREFE) | \
                        (ADC_CCR_ADCPRE_0 | ADC_CCR_ADCPRE_1); /* ADC clock freq to PCLK2/8 */ \
                        (ADC_CCR_DELAY_0 | ADC_CCR_DELAY_1 | ADC_CCR_DELAY_2 | ADC_CCR_DELAY_3) /* maximum delay between 2 sampling */
#else
#error "Error: TARGET_ECU is invalid!"
#endif

#endif /* _ADC_CONF_H_ */
