#ifndef _DAC_H_
#define _DAC_H_

#include <stdint.h>

#include "stm32f0xx.h"

void DAC_Init(void);

static inline void DAC_Set(uint16_t val)
{
  DAC->DHR12R1 = val;
}

#endif /* _DAC_H_ */
