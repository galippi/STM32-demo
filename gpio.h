#ifndef _GPIO_H_
#define _GPIO_H_

#include "controller.h"

void GPIO_PortInit_Out(GPIO_TypeDef * const gpio, uint8_t portnum);
#if (CPU_TYPE != CPU_TYPE_STM32F1)
void GPIO_PortInit_AFOut(GPIO_TypeDef * const gpio, uint8_t portnum, uint8_t AFR_val);
#else
void GPIO_PortInit_AFOut(GPIO_TypeDef * const gpio, uint8_t portnum);
#endif
void GPIO_PortInit_In(GPIO_TypeDef * const gpio, uint8_t portnum);
void GPIO_PortInit_Analog(GPIO_TypeDef * const gpio, uint8_t portnum);

static inline void GPIO_Set(GPIO_TypeDef *port, uint32_t pin_num, uint32_t val)
{
  if (val)
  {
    port->ODR |= (1 << pin_num);
  }else
  {
    port->ODR &= ~(1 << pin_num);
  }
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
