#include <stdint.h>
#include <string.h>

#include "gpio_app.h"

#include "stm32f0xx_rcc.h"

#define VDD 3.0 /* Volt */

GPIO_TypeDef * const gpioa = GPIOA;
GPIO_TypeDef * const gpioc = GPIOC;
RCC_TypeDef * const rcc = RCC;
SysTick_Type * const systick = SysTick;
DAC_TypeDef * const dac = DAC;
ADC_TypeDef * const adc1 = ADC1;
TIM_TypeDef * const tim3 = TIM3;

void CAT_Error(uint8_t code)
{
  while(1)
    ; /* endless loop */
}

uint32_t DivU32_U32U32(uint32_t dividend, uint32_t divisor)
{
  if (divisor == 0)
  { /* error case -> overflow */
    return 0xFFFFFFFF;
  }
  if (dividend < divisor)
  {
    return 0;
  }
  if ((divisor & (divisor - 1)) == 0)
  { /* the divisor is power of 2 */
    while (divisor != 1)
    {
      if ((divisor & 0xFF) == 0)
      {
        divisor = divisor >> 8;
        dividend = dividend >> 8;
      }else
      {
        divisor = divisor >> 1;
        dividend = dividend >> 1;
      }
    }
    return dividend;
  }else
  {
    uint32_t result = 0;
    int32_t shift = 0;
    if (dividend & 0x80000000)
    { /* the upper bit of the value is set -> take care of it */
      while ((divisor & 0x80000000) == 0)
      {
        divisor = divisor << 1;
        shift++;
      }
    }else
    {
      while (dividend > divisor)
      {
        divisor = divisor << 1;
        shift++;
      }
    }
    while (shift >= 0)
    {
      if (dividend >= divisor)
      {
        result = result + (1 << shift);
        dividend = dividend - divisor;
        if (dividend == 0)
        { /* no remainder -> return immediately */
          return result;
        }
      }
      shift--;
      divisor = divisor >> 1;
    }
    return result;
  }
}

int32_t DivI32_I32I32(int32_t dividend, int32_t divisor)
{
  int32_t sign = 1;
  uint32_t dividend_u;
  uint32_t divisor_u;
  uint32_t result_u;
  if (dividend >= 0)
  {
    dividend_u = (uint32_t) dividend;
  }else
  {
    dividend_u = (uint32_t) -dividend;
    sign = -sign;
  }
  if (divisor >= 0)
  {
    divisor_u = (uint32_t) divisor;
  }else
  {
    divisor_u = (uint32_t) -divisor;
    sign = -sign;
  }
  result_u = DivU32_U32U32(dividend_u, divisor_u);
  if (sign > 0)
  {
    return (int32_t)result_u;
  }else
  {
    return -(int32_t)result_u;
  }
}

inline void DAC_Set(uint16_t val)
{
  DAC->DHR12R1 = val;
}

void DAC_Init(void)
{
  /* enable the GPIO-A, if it was not enabled */
  if (!(RCC->APB1ENR & RCC_APB1ENR_DACEN))
  {
    RCC->APB1ENR |= RCC_APB1ENR_DACEN;
  }
  /* enable the GPIO-A, if it was not enabled - DAC is connected to PA.4-5 */
  GPIO_PortInit_Analog(GPIOA, 4);
  DAC->CR = (DAC->CR & 0x303F) | 0x0001;
  DAC_Set((uint16_t)(1.1/VDD * 4096));
}


const uint16_t *TS_CAL1 = (uint16_t*)0x1FFFF7B8; /* ADC value of temp. sensor at 30C */
const uint16_t *TS_CAL2 = (uint16_t*)0x1FFFF7C2; /* ADC value of temp. sensor at 110C */
const uint16_t *VREFINT_CAL = (uint16_t*)0x1FFFF7BA; /* ADC value of reference voltage at 30C */

#define ADC_CR_INIT     (ADC_CR_ADCAL | ADC_CR_ADEN)/* ADC calibration and enable it */
#define ADC_CFGR1_INIT  (ADC_CFGR1_DISCEN)
#define ADC_CFGR2_INIT  0x00000000
#define ADC_SMPR_INIT   0x00000007
#define ADC_TR_INIT     0x00000000
/* enable ADC channels: DAC1-out, VBat, temperature sensor, Vref */
#define ADC_CHSELR_INIT (ADC_CHSELR_CHSEL4 | ADC_CHSELR_CHSEL16 | ADC_CHSELR_CHSEL17 | ADC_CHSELR_CHSEL18)
#define ADC_CCR_INIT    (ADC_CCR_VBATEN | ADC_CCR_TSEN | ADC_CCR_VREFEN)
#define ADC_IER_INIT    0x00000000

void ADC_Init(void)
{
  if (!(RCC->APB2ENR & RCC_APB2Periph_ADC1))
  {
    RCC->APB2ENR |= RCC_APB2Periph_ADC1;
  }
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

inline void ADC_Start(void)
{
  ADC1->CR |= ADC_CR_ADSTART;
}

inline uint16_t ADC_Get(void)
{
  return ADC1->DR;
}

inline char ADC_GetStatus(void)
{
  return (ADC1->ISR & ADC_ISR_EOC) != 0; /* give back the state of the end of conversion flag */
}

enum e_ADC_values
{
  ADC_IN4_DAC,
  ADC_TemperatureSensor,
  ADC_Vref,
  ADC_VBat,
  ADC_Ch_Num /* this must be the last one */
};
uint16_t ADC_values[ADC_Ch_Num];
#define ADC_VALUES_NUM (sizeof(ADC_values)/sizeof(ADC_values[0]))
#define TEMP_SCALE 10
//int8_t Temperature = -128;
int16_t Temperature = -32768;
int16_t Temperature_raw = -32768;

void ADC_HandlerInit(void)
{
  ADC_Init();
  ADC_Start();
}

void ADC_Handler(void)
{
  static uint8_t adc_idx = 0;
  if (ADC_GetStatus())
  {
    ADC_values[adc_idx] = ADC_Get();
    adc_idx ++;
    if (adc_idx >= ADC_VALUES_NUM)
    {
      adc_idx = 0;
      uint32_t v_ref_factor;
      #define V_REF_FACTOR_SCALE 16
      if (ADC_values[ADC_Vref] > 1000)
      {
        v_ref_factor = DivU32_U32U32(*VREFINT_CAL << V_REF_FACTOR_SCALE, ADC_values[ADC_Vref]);
      }else
      {
        v_ref_factor = 1 << V_REF_FACTOR_SCALE;
      }
      uint16_t v_sensor = (ADC_values[ADC_TemperatureSensor] * v_ref_factor) >> V_REF_FACTOR_SCALE;
      //Temperature = (((ADC_values[ADC_TemperatureSensor] - (int16_t)*TS_CAL1) * (int16_t)(110 - 30)) / (*TS_CAL2 - *TS_CAL1)) + 30;
      //Temperature = (int8_t)DivI32_I32I32(((v_sensor - (int16_t)*TS_CAL1) * (int16_t)(110 - 30)), (*TS_CAL2 - *TS_CAL1)) + 30;
      Temperature = (int16_t)DivI32_I32I32(((v_sensor - (int32_t)*TS_CAL1) * ((int32_t)(110 - 30) * TEMP_SCALE)), (*TS_CAL2 - *TS_CAL1)) + (30 * TEMP_SCALE);
      Temperature_raw = (int16_t)DivI32_I32I32(((ADC_values[ADC_TemperatureSensor] - (int32_t)*TS_CAL1) * ((int32_t)(110 - 30) * TEMP_SCALE)), (*TS_CAL2 - *TS_CAL1)) + (30 * TEMP_SCALE);
    }
    ADC_Start();
  }
}

#define TIM3_CR1_INIT 0x10 /* down counter mode */
#define TIM3_CR2_INIT 0x0
#define TIM3_PSC_INIT  8 /* 8MHz / 8 -> 1MHz */
#define TIM3_ARR_INIT 1000 /* 1MHz / 1000 -> 1ms timebase */
#define TIM3_CCER_INIT 0x01 /* enable CC1 out high */
#define TIM3_CCMR1_INIT 0x0000
#define TIM3_CCMR2_INIT 0x0000

void TIM3_Init(void)
{
  if (!(RCC->APB1ENR & RCC_APB1Periph_TIM3))
  {
    RCC->APB1ENR |= RCC_APB1Periph_TIM3;
  }
  TIM3->CR1 = (TIM3->CR1 & 0xFC00) | TIM3_CR1_INIT;
  TIM3->CR2 = (TIM3->CR2 & 0xFF07) | TIM3_CR2_INIT;
  TIM3->ARR = TIM3_ARR_INIT;
  TIM3->PSC = TIM3_PSC_INIT;
  TIM3->EGR = 0x01; /* immediate reload */
  TIM3->CCER = TIM3_CCER_INIT;
  TIM3->CCMR1 = TIM3_CCMR1_INIT;
  TIM3->CCMR2 = TIM3_CCMR2_INIT;
  TIM3->SR = 0x0000; /* clear all IT requests */
  TIM3->CR1 |= 1; /* enable timer */
}

inline uint32_t TIM3_SR_UIF_Get(void)
{
  return (TIM3->SR & TIM_SR_UIF);
}

inline void TIM3_SR_UIF_Reset(void)
{
  TIM3->SR = ~(TIM_SR_UIF); /* reset the UpdateInterrupFlag */
}

void Task_1ms(void)
{

}

void Scheduler(void)
{
  Task_1ms();
}

inline void TIM3_UIF_Callback(void)
{ /* call back function of TIM3 UIF - counter underflow */
  Scheduler();
}

void TIM3_UIF_PollHandler(void)
{
  if (TIM3_SR_UIF_Get())
  {
    TIM3_SR_UIF_Reset();
    TIM3_UIF_Callback();
  }
}

void SysTick_Init(void)
{
  uint32_t ticks = 65536;
  SysTick->LOAD  = (ticks & SysTick_LOAD_RELOAD_Msk) - 1;      /* set reload register */
  //NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);  /* set Priority for Cortex-M0 System Interrupts */
  SysTick->VAL   = ticks - 1;                                    /* Load the SysTick Counter Value */
  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                   /*SysTick_CTRL_TICKINT_Msk   | */
                   SysTick_CTRL_ENABLE_Msk;                    /* Enable SysTick IRQ and SysTick Timer */
}

uint8_t timer[128];
#define SIZE_TIMER (sizeof(timer)/sizeof(timer[0]))
#define MAX_TIMER 255
uint8_t timer_brake;

/* inline */ uint16_t SysTick_Get(void)
{
  uint16_t val = (~SysTick->VAL) & 0xFFFF;
  if (!timer_brake)
  {
    uint16_t idx = val + (SIZE_TIMER / 2);
    if (idx < SIZE_TIMER)
    {
      timer[idx] ++;
      if (timer[idx] == MAX_TIMER)
      {
        timer_brake = 1;
      }
    }
  }
  return val;
}

inline void *memset(void *ptr_, int data, size_t size)
{
  uint8_t *ptr = ptr_;
  while(size > 0)
  {
    *ptr = data;
    ptr++;
    size--;
  }
  return ptr_;
}

void main(void)
{
  uint8_t led3 = 0;
  SysTick_Init();
  LED3_Init();
  LED4_Init();
  Button1_Init();
  DAC_Init();
  ADC_HandlerInit();
  memset(timer, 0, sizeof(timer));
  timer_brake = 0;
  {
    uint16_t val = 0;
    while(val < 4096)
    {
      DAC_Set(val);
      ADC_Handler();
      while (!ADC_GetStatus())
      {/* wait the end of the conversion */}
      val = (val << 1) | 1;
    }
  }
  TIM3_Init();

  while (!Button1_Get())
  {
    LED3_Set(led3);
    led3 = !led3;
  }
  LED4_Set(1);
  {
    uint8_t state = 0;
    uint16_t dac_val = 0;
    DAC_Set(dac_val);
    uint16_t dac_t = SysTick_Get();
#define FSYS 8000000
    while (1)
    {
#if 0
      SysTick_Get();
#elif 0
      uint16_t dac_t_new = SysTick_Get();
      uint16_t dt = dac_t_new - dac_t;
      while ((dt = dac_t_new - dac_t) > (FSYS / 1000))
      {
        dac_t += (FSYS / 1000);
      }
      if (dt > ((FSYS / 1000) / 2))
      {
        dac_val = 4095 - ((4095 * (uint32_t)dt) / ((FSYS / 1000) / 2));
      }else
      {
        dac_val = (4095 * (uint32_t)dt) / ((FSYS / 1000) / 2);
      }
      DAC_Set(dac_val);
#elif 1
      dac_val = (SysTick_Get() >> 3) & 0x1FFF;
      if (dac_val >= 0x1000)
      {
        DAC_Set(0x1FFF - dac_val);
      }else
      {
        DAC_Set(dac_val);
      }
#else
      LED3_Set(1);
      while (SysTick_Get() > 32767)
        ;
      LED3_Set(0);
      while (SysTick_Get() <= 32767)
        ;
#endif
      ADC_Handler();
      TIM3_UIF_PollHandler();
    }
  }
}
