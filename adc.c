#include "stm32f0xx_rcc.h"

#include "adc.h"
#include "adc_conf.h"

const uint16_t *TS_CAL1 = (uint16_t*)0x1FFFF7B8; /* ADC value of temp. sensor at 30C */
const uint16_t *TS_CAL2 = (uint16_t*)0x1FFFF7C2; /* ADC value of temp. sensor at 110C */
const uint16_t *VREFINT_CAL = (uint16_t*)0x1FFFF7BA; /* ADC value of reference voltage at 30C */

void ADC_Init(void)
{
  if (!(RCC->APB2ENR & RCC_APB2Periph_ADC1))
  {
    RCC->APB2ENR |= RCC_APB2Periph_ADC1;
  }
  RCC->CR2 |= RCC_CR2_HSI14ON;
  if (ADC1->CR & ADC_CR_ADEN)
  {
    ADC1->CR |= ADC_CR_ADSTP;
    while (ADC1->CR & ADC_CR_ADSTP)
    { /* wait the end of the running conversion */ }
  }
  if (ADC_CR_INIT & ADC_CR_ADCAL)
  {
    if (ADC1->CR & ADC_CR_ADEN)
    {
      ADC1->CR = ADC_CR_ADDIS;
      while (ADC1->CR & ADC_CR_ADEN)
      { /* wait the end of the deinitialization */ }
    }
    ADC1->CR = ADC_CR_ADCAL;
    while (ADC1->CR & ADC_CR_ADCAL)
    { /* wait the end of the calibration */ }
    ADC1->CR = ADC_CR_INIT & (ADC_CR_ADEN);
  }else
  {
    ADC1->CR = ADC_CR_INIT;
  }
  ADC1->CFGR1 = ADC_CFGR1_INIT;
  ADC1->CFGR2 = (ADC1->CFGR2 & ~(ADC_CFGR2_JITOFFDIV4 | ADC_CFGR2_JITOFFDIV2)) | ADC_CFGR2_INIT;
  ADC1->SMPR = ADC_SMPR_INIT;
  ADC1->TR = (ADC1->TR & 0xF000F000) | ADC_TR_INIT;
  ADC1->CHSELR = ADC_CHSELR_INIT;
  ADC1->IER = ADC_IER_INIT;
  ADC->CCR = ADC_CCR_INIT;
}
