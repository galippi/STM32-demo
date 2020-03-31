#include "controller.h"

#include "can_conf.h"

#include "CAN_stm32.h"

#if (CAN_TSEG1 < 1) || (CAN_TSEG1 > 16)
#error CAN_TSEG1 is wrongly set!
#endif

#if (CAN_TSEG2 < 1) || (CAN_TSEG2 > 8)
#error CAN_TSEG2 is wrongly set!
#endif

#if (CAN_SJW < 1) || (CAN_SJW > 4)
#error CAN_SJW is wrongly set!
#endif

void CAN_STM32_init(uint32_t baud, uint8_t remap)
{
  RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;
  if (remap == 0)
  {
      BitfieldSet(AFIO->MAPR, 13, 2, 0); /* AFIO_MAPR_CAN_REMAP */
      GPIO_PortInit_AFOut(GPIOA, 12); /* CAN-TD */
      GPIO_PortInit_In   (GPIOA, 11); /* CAN-RD */
  }else
  if (remap == 2)
  {
      BitfieldSet(AFIO->MAPR, 13, 2, 1); /* AFIO_MAPR_CAN_REMAP */
      GPIO_PortInit_AFOut(GPIOB, 9); /* CAN-TD */
      GPIO_PortInit_In   (GPIOB, 8); /* CAN-RD */
  }else
  {
      BitfieldSet(AFIO->MAPR, 13, 2, 2); /* AFIO_MAPR_CAN_REMAP */
      GPIO_PortInit_AFOut(GPIOD,  0); /* CAN-TD */
      GPIO_PortInit_In   (GPIOD,  1); /* CAN-RD */
  }
  CAN1->MCR = (CAN_MCR_NART | CAN_MCR_INRQ);       /* init mode, disable auto. retransmission */
  /* set BTR register so that sample point is at about 72% bit time from bit start */
  uint16_t brp  = (PCLK1 * 1000) / ((CAN_TSEG1 + CAN_TSEG2 + 1) * baud);
  CAN1->BTR &= ~(CAN_BTR_SILM | CAN_BTR_LBKM | CAN_BTR_BRP | CAN_BTR_TS1 | CAN_BTR_TS2 | CAN_BTR_SJW);
  CAN1->BTR |=  (CAN_BTR_INIT | ((CAN_SJW - 1)* CAN_BTR_SJW_0) | ((CAN_TSEG2 - 1) * CAN_BTR_TS2_0) | ((CAN_TSEG1 - 1) * CAN_BTR_TS1_0) | ((brp-1) * CAN_BTR_BRP_0));
  CAN1->MCR &= ~CAN_MCR_INRQ;                      /* normal operating mode, reset INRQ */
}

uint32_t CAN_STM32_tx(const CAN_msg *msg)
{
  return 0;
}

uint32_t CAN_STM32_rx(CAN_msg *msg)
{
  return 0;
}
