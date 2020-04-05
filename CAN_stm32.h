#ifndef _CAN_STM32_H_
#define _CAN_STM32_H_ 1

#include <stdint.h>

typedef enum
{
  CAN_Filter_MASK_16bit,
  CAN_Filter_ID_16bit,
  CAN_Filter_MASK_32bit,
  CAN_Filter_ID_32bit,
}t_CAN_FilterMode;

typedef struct
{
  uint32_t id;
  union
  {
    uint32_t data32[2];
    uint8_t  data8[8];
  }data;
  uint8_t dlc;
}CAN_msg;

void CAN_STM32_init(uint32_t baud);
void CAN_STM32_setFilter(CAN_TypeDef *can, uint8_t filterIdx, t_CAN_FilterMode mode, uint32_t id, uint32_t mask);
uint32_t CAN_STM32_tx(const CAN_msg *msg);
uint32_t CAN_STM32_rx(CAN_msg *msg);

#endif
