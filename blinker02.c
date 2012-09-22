#include <stdint.h>
#include <string.h>

#include "gpio_app.h"
#include "adc.h"
#include "adc_app.h"
#include "dac.h"
#include "util.h"

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
  (void)code;
  while(1)
    ; /* endless loop */
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

int main(void)
{
  uint8_t led3 = 0;
  SysTick_Init();
  LED3_Init();
  LED4_Init();
  Button1_Init();
  DAC_Init();
  DAC_Set((uint16_t)(1.1/VDD * 4096));
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
  return 0;
}
