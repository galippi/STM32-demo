#ifndef _GPIO_APP_H_
#define _GPIO_APP_H_

#include <stdint.h>

#include "stm32f0xx.h"

#include "gpio.h"

#define LED3_PORT GPIOC
#define LED3_PIN_NUM 9
#define LED4_PORT GPIOC
#define LED4_PIN_NUM 8
#define BUTTON1_PORT GPIOA
#define BUTTON1_PIN_NUM 0

#define PB13_PORT GPIOB
#define PB13_PIN_NUM 13

static inline void Button1_Init(void)
{
  GPIO_PortInit_In(BUTTON1_PORT, BUTTON1_PIN_NUM);
}

static inline uint8_t Button1_Get(void)
{
  return (BUTTON1_PORT->IDR & (1 << BUTTON1_PIN_NUM)) ? 1 : 0;
}

static inline void LED3_Init(void)
{
  GPIO_PortInit_Out(LED3_PORT, LED3_PIN_NUM);
}

static inline void LED3_Set(uint8_t val)
{
  if (val)
  {
    LED3_PORT->ODR |= (1 << LED3_PIN_NUM);
  }else
  {
    LED3_PORT->ODR &= ~(1 << LED3_PIN_NUM);
  }
}

static inline void LED4_Init(void)
{
  GPIO_PortInit_Out(LED4_PORT, LED4_PIN_NUM);
}

static inline void LED4_Set(uint8_t val)
{
  if (val)
  {
    LED3_PORT->ODR |= (1 << LED4_PIN_NUM);
  }else
  {
    LED3_PORT->ODR &= ~(1 << LED4_PIN_NUM);
  }
}

static inline void PB13_Init(void)
{
  GPIO_PortInit_Out(PB13_PORT, PB13_PIN_NUM);
}

static inline uint16_t PB13_Get(void)
{
  return (PB13_PORT->IDR & (1 << PB13_PIN_NUM)); // does not work
}

static inline void PB13_Set(uint32_t val)
{
  if (val)
  {
    PB13_PORT->ODR |= (1 << PB13_PIN_NUM);
  }else
  {
    PB13_PORT->ODR &= ~(1 << PB13_PIN_NUM);
  }
}

#endif /* _GPIO_APP_H_ */
