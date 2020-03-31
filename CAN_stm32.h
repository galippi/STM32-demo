#ifndef _CAN_STM32_H_
#define _CAN_STM32_H_ 1

#include <stdint.h>

typedef struct
{
  uint32_t id;
  uint8_t data[8];
}CAN_msg;

void CAN_STM32_init(void);
uint32_t CAN_STM32_tx(const CAN_msg *msg);
uint32_t CAN_STM32_rx(CAN_msg *msg);

#endif
