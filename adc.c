#include "adc.h"
#include "adc_conf.h"

#include STM32_RCC_HEADER

//#include CONTROLLER_BASE_NAME#_rcc_h
//#include CONTROLLER_BASE_NAME"_rcc.h"

const uint16_t *TS_CAL1 = (uint16_t*)0x1FFFF7B8; /* ADC value of temp. sensor at 30C */
const uint16_t *TS_CAL2 = (uint16_t*)0x1FFFF7C2; /* ADC value of temp. sensor at 110C */
const uint16_t *VREFINT_CAL = (uint16_t*)0x1FFFF7BA; /* ADC value of reference voltage at 30C */

uint8_t ADC_calibration = 64; /* calibration factor - default value -> no calibration */

void ADC_Init(void)
{
#if CPU_TYPE == CPU_TYPE_STM32F0
  if (!(RCC->APB2ENR & RCC_APB2Periph_ADC1))
  { /* enable the ADC1 */
    RCC->APB2ENR |= RCC_APB2Periph_ADC1;
  }
  RCC->CR2 |= RCC_CR2_HSI14ON; /* enable ADC-HSI14 oscillator */
  while (!(RCC->CR2 & RCC_CR2_HSI14RDY)) { /*wait to HSI14 staring */ }
  RCC->CFGR3 &= ~(RCC_CFGR3_ADCSW); /* ADC clock source to HSI14 */
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
    ADC_calibration = (uint8_t)(ADC_Get() & 0x003F);
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
#elif CPU_TYPE == CPU_TYPE_STM32F4
/* Handler for STM32F4 */
  if (!(RCC->APB2ENR & RCC_APB2ENR_ADC1EN))
  { /* enable the ADC1 */
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
  }
  ADC->CCR = (ADC->CCR & 0xFF3C10E0) | ADC_CCR_INIT;
  ADC1->CR1 = (ADC1->CR1 & 0xF83F0000) | ADC1_CR1_INIT;
  ADC1->CR2 = (ADC1->CR2 & 0x8080F0FC) | (ADC1_CR2_INIT & (~ADC_CR2_ADON));
  ADC1->SMPR1 = (ADC1->SMPR1 & 0xF8000000) | ADC1_SMPR1_INIT;
  ADC1->SMPR2 = (ADC1->SMPR2 & 0xF8000000) | ADC1_SMPR2_INIT;
  ADC1->SQR1 = (ADC1->SQR1 & 0xFF000000) | ADC1_SQR1_INIT;
  ADC1->SQR2 = (ADC1->SQR2 & 0xC0000000) | ADC1_SQR2_INIT;
  ADC1->SQR1 = (ADC1->SQR3 & 0xC0000000) | ADC1_SQR3_INIT;
  if (ADC1_CR2_INIT & ADC_CR2_ADON)
  {
    ADC1->CR2 = ADC1->CR2 | ADC_CR2_ADON;
  }
#endif
}
