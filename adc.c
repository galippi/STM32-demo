#include "dma.h"
#include "adc.h"
#include "adc_conf.h"
#include "timer_app.h"

const uint16_t * const TS_CAL1 = (uint16_t*)0x1FFF75A8; /* ADC value of temp. sensor at 30C */
const uint16_t * const VREFINT_CAL = (uint16_t*)0x1FFF75AA; /* ADC value of reference voltage at 30C */

uint8_t ADC_calibration = 64; /* calibration factor - default value -> no calibration */

void ADC_Init(void)
{
#if (CPU_TYPE == CPU_TYPE_STM32G0) || (CPU_TYPE == CPU_TYPE_STM32G0B1)
  if (!(RCC->APBENR2 & RCC_APBENR2_ADCEN))
  { /* enable the ADC1 */
    RCC->APBENR2 |= RCC_APBENR2_ADCEN;
  }
#if !(ADC_CR_INIT & ADC_CR_ADVREGEN)
#error ADC_CR_ADVREGEN is not set!
#endif

  if (!(ADC1->CR & ADC_CR_ADVREGEN)) {
      ADC1->CR = ADC1->CR | ADC_CR_ADVREGEN;
      wait_us(20); // wait 20us
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
    ADC1->CR = ADC_CR_ADCAL | ADC_CR_ADEN;
    while (ADC1->CR & ADC_CR_ADCAL)
    { /* wait the end of the calibration */ }
    ADC_calibration = (uint8_t)(ADC_Get() & 0x003F);
    ADC1->CR = ADC_CR_INIT & ~(ADC_CR_ADCAL);
  }else
  {
    ADC1->CR = ADC_CR_INIT;
  }
  ADC1->CFGR1 = ADC_CFGR1_INIT;
  //ADC1->CFGR2 = (ADC1->CFGR2 & ~(ADC_CFGR2_JITOFFDIV4 | ADC_CFGR2_JITOFFDIV2)) | ADC_CFGR2_INIT;
  ADC1->SMPR = ADC_SMPR_INIT;
  //ADC1->TR = (ADC1->TR & 0xF000F000) | ADC_TR_INIT;

  ADC1->ISR = ADC_ISR_CCRDY; // enable CHSELR writting
  ADC1->CHSELR = ADC_CHSELR_INIT;
  while ((ADC1->ISR & ADC_ISR_CCRDY) == 0)
      ;
  ADC1->IER = ADC_IER_INIT;
  ADC->CCR = ADC_CCR_INIT;

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
    extern char _compile_check[((ADC_CFGR1_INIT & ADC_CFGR1_DMAEN) != 0) ? -1 : 1]; /* ADC-DMA is configured, but the DMA is not configured (ADC1_DMA_CMAR) */
  }
#endif

#elif (CPU_TYPE == CPU_TYPE_STM32F1)

#ifdef ADC1_DMA_CMAR
  {
    extern char _compile_check[((ADC_CR2_INIT & ADC_CR2_DMA) == 0) ? -1 : 1]; /* ADC1_DMA_CMAR is set, but the DMA is not configured for ADC */
    (void)_compile_check;
  }
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
  {
    extern char _compile_check[((ADC_CR2_INIT & ADC_CR2_DMA) != 0) ? -1 : 1]; /* ADC-DMA is configured, but the DMA is not configured (ADC1_DMA_CMAR) */
  }
#endif

#else
#error Not supported target!
#endif
}
