#ifndef _GPIO_H_
#define _GPIO_H_

#include "controller.h"

#define GPIO_MODER_IN  0
#define GPIO_MODER_OUT 1
#define GPIO_MODER_AF  2
#define GPIO_MODER_AN  3

#define GPIO_OSPEEDR_VERY_LOW_SPEED  0
#define GPIO_OSPEEDR_LOW_SPEED       1
#define GPIO_OSPEEDR_HIGH_SPEED      2
#define GPIO_OSPEEDR_VERY_HIGH_SPEED 3

#define GPIO_OTYPER_PP 0
#define GPIO_OTYPER_OD 1

#define GPIO_PUPDR_NOPULL   0
#define GPIO_PUPDR_PULLUP   1
#define GPIO_PUPDR_PULLDOWN 2

/**
 *
 */
void GPIO_PortInit(GPIO_TypeDef * const gpio, uint8_t portnum, uint8_t ospeed, uint8_t otyper, uint8_t moder, uint8_t pupdr, uint8_t AFR_val);
void GPIO_PortInit_Out(GPIO_TypeDef * const gpio, uint8_t portnum);
void GPIO_PortInit_OC(GPIO_TypeDef * const gpio, uint8_t portnum);

#if (CPU_TYPE != CPU_TYPE_STM32F1)
void GPIO_PortInit_AFOut(GPIO_TypeDef * const gpio, uint8_t portnum, uint8_t AFR_val);
#else
void GPIO_PortInit_AFOut(GPIO_TypeDef * const gpio, uint8_t portnum);
#endif
void GPIO_PortInit_In(GPIO_TypeDef * const gpio, uint8_t portnum);
void GPIO_PortInit_Analog(GPIO_TypeDef * const gpio, uint8_t portnum);

static inline void GPIO_Set(GPIO_TypeDef *port, uint32_t pin_num, uint32_t val)
{
#if 0
  if (val)
  {
    port->ODR |= (1 << pin_num);
  }else
  {
    port->ODR &= ~(1 << pin_num);
  }
#else
  if (val)
  {
    port->BSRR = (1 << pin_num);
  }else
  {
    port->BRR = (1 << pin_num);
  }
#endif
}

static inline uint8_t GPIO_GetI(GPIO_TypeDef *port, uint32_t pin)
{
  return ((port->IDR >> pin) & 1);
}

static inline uint8_t GPIO_GetO(GPIO_TypeDef *port, uint32_t pin)
{
  return ((port->ODR >> pin) & 1);
}

#endif /* _GPIO_H_ */
