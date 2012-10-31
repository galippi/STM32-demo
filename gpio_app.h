#ifndef _GPIO_APP_H_
#define _GPIO_APP_H_

#include <stdint.h>

#include "gpio.h"

#if TARGET_ECU == TARGET_ECU_STM32F0DISCOVERY
/* port definition for STM32F0-discovery board */
#define LED3_PORT GPIOC
#define LED3_PIN_NUM 9
#define LED4_PORT GPIOC
#define LED4_PIN_NUM 8

#elif TARGET_ECU == TARGET_ECU_STM32F4DISCOVERY
/* port definition for STM32F0-discovery board */
#define LED3_PORT GPIOD
#define LED3_PIN_NUM 13
#define LED4_PORT GPIOD
#define LED4_PIN_NUM 12
#define LED5_PORT GPIOD
#define LED5_PIN_NUM 14
#define LED6_PORT GPIOD
#define LED6_PIN_NUM 15

#else
#error "Error: TARGET_ECU is invalid!"
#endif

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

#define LEDx_Init(x) \
  static inline void LED##x##_Init(void)\
  { \
    GPIO_PortInit_Out(LED##x##_PORT, LED##x##_PIN_NUM); \
  }

#define LEDx_Set(x) \
  static inline void LED##x##_Set(uint8_t val) \
  { \
    if (val) \
    { \
      LED##x##_PORT->ODR |= (1 << LED##x##_PIN_NUM); \
    }else \
    { \
      LED##x##_PORT->ODR &= ~(1 << LED##x##_PIN_NUM); \
    } \
  }

#define LEDx_Get(x) \
  static inline uint16_t LED##x##_Get(void) \
  { \
    return (LED##x##_PORT->ODR & (1 << LED##x##_PIN_NUM)); \
  }

LEDx_Init(3)
LEDx_Set(3)
LEDx_Get(3)

LEDx_Init(4)
LEDx_Set(4)
LEDx_Get(4)

#ifdef LED5_PORT
LEDx_Init(5)
LEDx_Set(5)
LEDx_Get(5)
#else
#define LED5_Init() {/* do nothing */}
#define LED5_Set(x) {/* do nothing */}
#endif

#ifdef LED6_PORT
LEDx_Init(6)
LEDx_Set(6)
LEDx_Get(6)
#else
#define LED6_Init() {/* do nothing */}
#define LED6_Set(x) {/* do nothing */}
#endif

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
