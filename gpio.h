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

#endif /* _GPIO_H_ */
