#ifndef _DAC_H_
#define _DAC_H_

#include <stdint.h>

#include "dac_conf.h"

#if (defined(DAC_1_ENABLED) || defined(DAC_2_ENABLED))
void DAC_Init(void);
#endif /* (defined(DAC_1_ENABLED) || defined(DAC_2_ENABLED)) */

#if defined(DAC_1_ENABLED)
static inline void DAC_1_Set(uint16_t val)
{
  DAC->DHR12R1 = val;
}
#endif /* DAC_1_ENABLED */

#if defined(DAC_2_ENABLED)
static inline void DAC_2_Set(uint16_t val)
{
  DAC->DHR12R2 = val;
}
#endif /* DAC_2_ENABLED */

#endif /* _DAC_H_ */
