#ifndef _ADC_CONF_H_
#define _ADC_CONF_H_

#define ADC_CR_INIT     (ADC_CR_ADCAL | ADC_CR_ADEN)/* ADC calibration and enable it */
#define ADC_CFGR1_INIT  (ADC_CFGR1_DISCEN)
#define ADC_CFGR2_INIT  0x00000000
#define ADC_SMPR_INIT   0x00000007
#define ADC_TR_INIT     0x00000000
/* enable ADC channels: DAC1-out, VBat, temperature sensor, Vref */
#define ADC_CHSELR_INIT (ADC_CHSELR_CHSEL4 | ADC_CHSELR_CHSEL16 | ADC_CHSELR_CHSEL17 | ADC_CHSELR_CHSEL18)
#define ADC_CCR_INIT    (ADC_CCR_VBATEN | ADC_CCR_TSEN | ADC_CCR_VREFEN)
#define ADC_IER_INIT    0x00000000

#endif /* _ADC_CONF_H_ */
