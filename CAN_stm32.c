#include "datatype.h"
#include "bitfield_lib.h"

#include "STM32_registers.h"

#include "pll.h"

#include "can_conf.h"

#include "can_stm32.h"

#if (CAN_TSEG1 < 1) || (CAN_TSEG1 > 16)
#error CAN_TSEG1 is wrongly set!
#endif

#if (CAN_TSEG2 < 1) || (CAN_TSEG2 > 8)
#error CAN_TSEG2 is wrongly set!
#endif

#if (CAN_SJW < 1) || (CAN_SJW > 4)
#error CAN_SJW is wrongly set!
#endif

void CAN_STM32_init(uint32_t baud)
{
  RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;
  CAN1->MCR = (CAN_MCR_NART | CAN_MCR_INRQ);       /* init mode, disable auto. retransmission */
  /* set BTR register so that sample point is at about 72% bit time from bit start */
  uint16_t brp  = (PLL_APB1_FREQ_HZ) / ((CAN_TSEG1 + CAN_TSEG2 + 1) * baud);
  CAN1->BTR &= ~(CAN_BTR_SILM | CAN_BTR_LBKM | CAN_BTR_BRP | CAN_BTR_TS1 | CAN_BTR_TS2 | CAN_BTR_SJW);
  CAN1->BTR |=  (CAN_BTR_INIT | ((CAN_SJW - 1)* CAN_BTR_SJW_0) | ((CAN_TSEG2 - 1) * CAN_BTR_TS2_0) | ((CAN_TSEG1 - 1) * CAN_BTR_TS1_0) | ((brp-1) * CAN_BTR_BRP_0));
  CAN1->MCR &= ~CAN_MCR_INRQ;                      /* normal operating mode, reset INRQ */
}

void CAN_STM32_setFilter(CAN_TypeDef *can, uint8_t filterIdx, t_CAN_FilterMode mode, uint32_t id, uint32_t mask)
{
  can->FMR  |=  CAN_FMR_FINIT;      // set Initialisation mode for filter banks
  id = (id   << 3) | ((id >> 29) & 0x04); // converting id to STM32-CAN format
  mask = (mask << 3) | ((mask >> 29) & 0x04); // converting mask to STM32-CAN format
  switch(mode)
  {
    case CAN_Filter_MASK_16bit:
    {
      BitReset(can->FA1R, filterIdx / 2); // deactivate filter
      BitReset(can->FS1R, filterIdx);     // set 16-bit scale configuration
      BitReset(can->FM1R, filterIdx);     // set 16-bit identifier mask mode
      uint32_t data = (id & 0xFFFF) | ((mask << 16) & 0xFFFF0000);
      if (!(filterIdx & 0x01))
      {
        can->sFilterRegister[filterIdx / 2].FR1 = data;
      }else
      {
        can->sFilterRegister[filterIdx / 2].FR2 = data;
      }
      BitReset(can->FFA1R, filterIdx / 2); // assign filter to FIFO 0
      BitSet(can->FA1R, filterIdx / 2);    // activate filter
      break;
    }
    case CAN_Filter_ID_16bit:
    {
      BitReset(can->FA1R, filterIdx / 4); // deactivate filter
      BitReset(can->FS1R, filterIdx);     // set 16-bit scale configuration
      BitSet(can->FM1R, filterIdx);       // set 16-bit identifier list mode
      volatile uint32_t *dataPtr;
      if (!(filterIdx & 0x02))
      {
        dataPtr = &(can->sFilterRegister[filterIdx / 4].FR1);
      }else
      {
        dataPtr = &(can->sFilterRegister[filterIdx / 4].FR2);
      }
      uint32_t data = *dataPtr;
      if (!(filterIdx & 0x01))
      {
        data = (data & 0xFFFF0000) | (id & 0xFFFF);
      }else
      {
        data = (data & 0x0000FFFF) | ((id & 0xFFFF) << 16);
      }
      *dataPtr = data;
      BitReset(can->FFA1R, filterIdx / 4); // assign filter to FIFO 0
      BitSet(can->FA1R, filterIdx / 4);    // activate filter
      break;
    }
    case CAN_Filter_MASK_32bit:
      BitReset(can->FA1R, filterIdx); // deactivate filter
      BitSet(can->FS1R, filterIdx);   // set 32-bit scale configuration
      BitReset(can->FM1R, filterIdx); // set 32-bit identifier mask mode
      can->sFilterRegister[filterIdx].FR1 = id;   //  32-bit identifier
      can->sFilterRegister[filterIdx].FR2 = mask; //  32-bit mask
      BitReset(can->FFA1R, filterIdx); // assign filter to FIFO 0
      BitSet(can->FA1R, filterIdx);    // activate filter
      break;
    case CAN_Filter_ID_32bit:
      BitReset(can->FA1R, filterIdx / 2); // deactivate filter
      BitSet(can->FS1R, filterIdx / 2);   // set 32-bit scale configuration
      BitSet(can->FM1R, filterIdx / 2);   // set 2 32-bit identifier list mode
      if (!(filterIdx & 0x01))
      {
        can->sFilterRegister[filterIdx / 2].FR1 = id; //  32-bit identifier
      }else
      {
        can->sFilterRegister[filterIdx / 2].FR2 = id; //  32-bit identifier
      }
      BitReset(can->FFA1R, filterIdx / 2); // assign filter to FIFO 0
      BitSet(can->FA1R, filterIdx / 2);    // activate filter
      break;
    default: // error handling
      break;
  }
  can->FMR &= ~CAN_FMR_FINIT; // reset Initialisation mode for filter banks
}

uint32_t CAN_STM32_tx(const CAN_msg *msg)
{
  CAN_TypeDef *can = CAN1;
  if ((can->TSR & CAN_TSR_TME0) == 0)
    return 0; // Tx mailbox is full

  if ((msg->id & 0x80000000) == 0)  {          //    Standard ID
      can->sTxMailBox[0].TIR = (msg->id << 21);
  }  else  {                                      // Extended ID
      can->sTxMailBox[0].TIR = (msg->id <<  3) | 0x04;
  }
  //can->sTxMailBox[0].TIR &= ~CAN_TI1R_RTR; // no remote frame support
  can->sTxMailBox[0].TDLR = msg->data.data32[0];
  can->sTxMailBox[0].TDHR = msg->data.data32[1];

  can->sTxMailBox[0].TDTR = (can->sTxMailBox[0].TDTR & ~CAN_TDT0R_DLC) | (msg->dlc & CAN_TDT0R_DLC); // setup message length

  //can->IER |= CAN_IER_TMEIE;                      // enable  TME interrupt
  can->sTxMailBox[0].TIR |=  CAN_TI0R_TXRQ;       // transmit message

  return 1;
}

uint32_t CAN_STM32_rx(CAN_msg *msg)
{
  CAN_TypeDef *can = CAN1;
  if (can->RF0R & CAN_RF0R_FMP0)
  { // is message available in FIFO?
    if ((can->sFIFOMailBox[0].RIR & CAN_RI0R_IDE) == 0) { // Standard ID
      msg->id = (can->sFIFOMailBox[0].RIR >> 21) & 0x000007FF;
    }  else  {                                            // Extended ID
      msg->id = ((can->sFIFOMailBox[0].RIR >> 3) & 0x1FFFFFFF) | 0x80000000;
    }
    msg->dlc = can->sFIFOMailBox[0].RDTR & 0x0F;
    msg->data.data32[0] = (can->sFIFOMailBox[0].RDLR);
    msg->data.data32[1] = (can->sFIFOMailBox[0].RDHR);
    can->RF0R |= CAN_RF0R_RFOM0; // Release FIFO 0
    return 1;
  }else
  {
    return 0;
  }
}
