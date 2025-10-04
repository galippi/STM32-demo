#include "can_stm32.h"

static inline void can1_init(uint32_t baud)
{
  CAN_STM32_init(baud);
}
