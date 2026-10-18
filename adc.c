#include "dma.h"
#include "adc.h"
#include "adc_conf.h"
#include "timer_app.h"

const uint16_t * const TS_CAL__30 = (uint16_t*)0x08FFF814; /* ADC value of temp. sensor at  30C */
const uint16_t * const TS_CAL_140 = (uint16_t*)0x08FFF818; /* ADC value of temp. sensor at 140C */
const uint16_t * const VREFINT_CAL = (uint16_t*)0x08FFF810; /* ADC value of reference voltage at 30C */

uint8_t ADC_calibration = 64; /* calibration factor - default value -> no calibration */

void ADC_Init(void)
{
  RCC->AHB2ENR |= RCC_AHB2ENR_ADC12EN; /* enable the ADC1 */

#if !(ADC_CR_INIT & ADC_CR_ADVREGEN)
#error ADC_CR_ADVREGEN is not set!
#endif

  if (ADC1->CR & ADC_CR_DEEPPWD) {
      ADC1->CR = ADC1->CR & ~ADC_CR_DEEPPWD;
      //while((ADC1->ISR & ADC_ISR_) == 0)
      //    ;
  }

  if (!(ADC1->CR & ADC_CR_ADVREGEN)) {
      ADC1->CR = ADC1->CR | ADC_CR_ADVREGEN;
      wait_us(10);
      while((ADC1->ISR & ADC_ISR_LDORDY) == 0)
          ;
  }

  //RCC->CR2 |= RCC_CR2_HSI14ON; /* enable ADC-HSI14 oscillator */
  //while (!(RCC->CR2 & RCC_CR2_HSI14RDY)) { /*wait to HSI14 staring */ }
  //RCC->CFGR3 &= ~(RCC_CFGR3_ADCSW); /* ADC clock source to HSI14 */
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
    ADC1->CR |= ADC_CR_ADCAL;
    while (ADC1->CR & ADC_CR_ADCAL)
    { /* wait the end of the calibration */ }
    //ADC_calibration = (uint8_t)(ADC_Get() & 0x003F);
  }
  ADC1->CR = ADC_CR_INIT & ~ADC_CR_ADCAL;
  ADC1->CFGR1 = ADC_CFGR1_INIT;
  //ADC1->CFGR2 = (ADC1->CFGR2 & ~(ADC_CFGR2_JITOFFDIV4 | ADC_CFGR2_JITOFFDIV2)) | ADC_CFGR2_INIT;
  ADC1->SMPR1 = ADC_SMPR1_INIT;
  ADC1->SMPR2 = ADC_SMPR2_INIT;

  ADC12_COMMON->CCR = ADCC_CCR_INIT;
  ADC1->SQR1 = ADC1_SQR1_INIT;
  ADC1->SQR2 = ADC1_SQR2_INIT;
  ADC1->SQR3 = ADC1_SQR3_INIT;
  ADC1->SQR4 = ADC1_SQR4_INIT;
  ADC1->PCSEL = ADC1_PCSEL_INIT;
#if 0 // STM32C542 only
  SBS->PMCR = (SBS->PMCR & ~0x0F) | ADC1_SBS_PMCR_INIT;
#endif

#ifdef ADC1_DMA_CMAR
  {
    extern char _compile_check[((ADC_CFGR1_INIT & ADC_CFGR1_DMAEN) == 0) ? -1 : 1]; /* ADC1_DMA_CMAR is set, but the DMA is not configured for ADC */
    (void)_compile_check;
  }

  DMA1_Channel_ADC1->CCR &= ~DMA_CCR_EN;
  DMA1_Channel_ADC1->CPAR = (uint32_t)&(ADC1->DR);
  DMA1_Channel_ADC1->CCR = \
                       0 /* MEM2MEM */ | \
                       DMA_CCR_PL_1 /* priority high */ | \
                       DMA_CCR_MINC /* no-per-inc, mem-inc */ | \
                       DMA_CCR_CIRC /* circular mode */ | \
                       DMA_CCR_MSIZE_0 /* Memory size 16 bits */ | \
                       DMA_CCR_PSIZE_0 /* Peripheral size 16 bits */ | \
                       0 /* per2mem - DMA_CCR1_DIR */ | \
                       0;
  DMA1_Channel_ADC1->CMAR = ADC1_DMA_CMAR;
  DMA1_Channel_ADC1->CNDTR = ADC1_DMA_CNDTR;
  DMAMUX1[DMA1_ChIdx_ADC1].CCR = 5;
  DMA1_Channel_ADC1->CCR |= DMA_CCR_EN; /* channel is enabled */
#else
  {
    //extern char _compile_check[((ADC_CFGR1_INIT & 0x01) != 0) ? -1 : 1]; /* ADC-DMA is configured, but the DMA is not configured (ADC1_DMA_CMAR) */
  }
#endif
}
