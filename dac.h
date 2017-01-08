#ifndef _DAC_H_
#define _DAC_H_

#include <stdint.h>

#include "controller.h"

#if (CPU_TYPE == CPU_TYPE_STM32F1) && (defined(STM32F10X_MD))
#error This CPU does not have DAC!
#endif

extern DAC_TypeDef * const dac;

void DAC_Init(void);

static inline void DAC_Set(uint16_t val)
{
  DAC->DHR12R1 = val;
}

#endif /* _DAC_H_ */
