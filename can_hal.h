#include "can_stm32.h"

static inline void can1_init(uint32_t baud)
{
  CAN_STM32_init(baud);
  CAN_STM32_setFilter(CAN1,  0, CAN_Filter_MASK_32bit, 0x00000000, 0x00000000); // all messages are accepted
  //CAN_STM32_setFilter(CAN1,  1, CAN_Filter_MASK_32bit, 0x000001F0, 0x800007FF);
}

#define can1_deinit() CAN_STM32_deinit()
