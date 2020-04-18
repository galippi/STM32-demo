#ifndef _CAN_APP_H_
#define _CAN_APP_H_ 1

#include "can_stm32.h"

static inline void can1_init(void)
{
  CAN_STM32_init(1000000);
  CAN_STM32_setFilter(CAN1,  0, CAN_Filter_MASK_32bit, 0x00000100, 0x800007FF);
  //CAN_STM32_setFilter(CAN1, 0, CAN_Filter_MASK_32bit, 0x80000000, 0x800000FF);
  //CAN_STM32_setFilter(CAN1, 1, CAN_Filter_MASK_32bit, 0x8000abeb, 0x8000FFFF);
  //CAN_STM32_setFilter(CAN1, 2, CAN_Filter_MASK_32bit, 0x80110000, 0x80FF0000);
  //CAN_STM32_setFilter(CAN1, 3, CAN_Filter_MASK_32bit, 0x00000100, 0x800007FF);
}

#endif /* _CAN_APP_H_ */
