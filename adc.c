#include "dma.h"
#include "adc.h"
#include "adc_conf.h"

#include STM32_RCC_HEADER

//#include CONTROLLER_BASE_NAME#_rcc_h
//#include CONTROLLER_BASE_NAME"_rcc.h"

const uint16_t * const TS_CAL1 = (uint16_t*)0x1FFFF7B8; /* ADC value of temp. sensor at 30C */
const uint16_t * const TS_CAL2 = (uint16_t*)0x1FFFF7C2; /* ADC value of temp. sensor at 110C */
const uint16_t * const VREFINT_CAL = (uint16_t*)0x1FFFF7BA; /* ADC value of reference voltage at 30C */

uint8_t ADC_calibration = 64; /* calibration factor - default value -> no calibration */

void ADC_Init(void)
{
#if TARGET_ECU == TARGET_ECU_STM32F0DISCOVERY
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
#elif (CPU_TYPE == CPU_TYPE_STM32F1)
  /* ADC clock source: PCLK2 */
  if (!(RCC->APB2ENR & RCC_APB2Periph_ADC1))
  { /* enable the ADC1 */
    RCC->APB2ENR |= RCC_APB2Periph_ADC1;
  }
  /* stop last conversions */
  if (ADC1->CR2 & ADC_CR2_ADON)
  {
	  ADC1->CR2 = 0; /* stop ADC */
    while (0)
    { /* wait the end of the running conversion */ }
  }
  if ((ADC_CR2_INIT) & ADC_CR2_CAL)
  {
    ADC1->CR2 = ADC_CR2_CAL | ADC_CR2_ADON;
    while (ADC1->CR2 & ADC_CR2_CAL)
    { /* wait the end of the calibration */ }
    ADC_calibration = (uint8_t)(ADC_Get() & 0x003F);
  }
  ADC1->CR1 = ADC_CR1_INIT;
  ADC1->CR2 = (ADC_CR2_INIT) & (~(ADC_CR2_CAL));
  ADC1->SMPR1 = ADC_SMPR1_INIT;
  ADC1->SMPR2 = ADC_SMPR2_INIT;
  ADC1->SQR1 = ADC_SQR1_INIT;
  ADC1->SQR2 = ADC_SQR2_INIT;
  ADC1->SQR3 = ADC_SQR3_INIT;

  DMA_Init(DMA1);
  DMA1_Channel1->CCR &= ~DMA_CCR1_EN;
  DMA1_Channel1->CPAR = (uint32_t)&(ADC1->DR);
  DMA1_Channel1->CCR = \
                       0 /* MEM2MEM */ | \
                       DMA_CCR1_PL_1 /* priority high */ | \
                       DMA_CCR1_MINC /* no-per-inc, mem-inc */ | \
                       DMA_CCR1_CIRC /* circular mode */ | \
                       DMA_CCR1_MSIZE_0 /* Memory size 16 bits */ | \
                       DMA_CCR1_PSIZE_0 /* Peripheral size 16 bits */ | \
                       0 /* per2mem - DMA_CCR1_DIR */ | \
                       0;
  DMA1_Channel1->CMAR = ADC1_DMA_CMAR;
  DMA1_Channel1->CNDTR = ADC1_DMA_CNDTR;
  DMA1_Channel1->CCR |= DMA_CCR1_EN; /* channel is enabled */
#else
#error Not supported target!
#endif
}
