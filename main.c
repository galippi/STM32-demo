#include <stdint.h>
#include <string.h>

#include "gpio_app.h"
#include "adc.h"
#include "adc_app.h"
#include "dac.h"
#include "timer.h"
#include "util.h"

#include "tasks.h"

#include "stm32f0xx_rcc.h"

#define VDD 3.0 /* Volt */

GPIO_TypeDef * const gpioa = GPIOA;
GPIO_TypeDef * const gpiob = GPIOB;
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

uint8_t timer[128];
#define SIZE_TIMER (sizeof(timer)/sizeof(timer[0]))
#define MAX_TIMER 255
uint8_t timer_brake;

static uint16_t SysTick_App_Get(void)
{
  uint16_t val = SysTick_Get();
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
  PB13_Init();
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
    uint16_t dac_t = SysTick_App_Get();
#define FSYS 8000000
    while (1)
    {
#if 0
      SysTick_App_Get();
#elif 0
      uint16_t dac_t_new = SysTick_App_Get();
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
      dac_val = (SysTick_App_Get() >> 3) & 0x1FFF;
      if (dac_val >= 0x1000)
      {
        DAC_Set(0x1FFF - dac_val);
      }else
      {
        DAC_Set(dac_val);
      }
#else
      LED3_Set(1);
      while (SysTick_App_Get() > 32767)
        ;
      LED3_Set(0);
      while (SysTick_App_Get() <= 32767)
        ;
#endif
      ADC_Handler();
      TIM3_UIF_PollHandler();
    }
  }
  return 0;
}
