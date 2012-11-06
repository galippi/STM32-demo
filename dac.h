#ifndef _DAC_H_
#define _DAC_H_

#include <stdint.h>

#include "controller.h"

void DAC_Init(void);

static inline void DAC_1_Set(uint16_t val)
{
  DAC->DHR12R1 = val;
}

#if CPU_TYPE == CPU_TYPE_STM32F4
static inline void DAC_2_Set(uint16_t val)
{
  DAC->DHR12R2 = val;
}
#endif

#endif /* _DAC_H_ */
