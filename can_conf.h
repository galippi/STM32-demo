#ifndef _CAN_CONF_H_
#define _CAN_CONF_H_ 1

#define CAN_TSEG1  8
#define CAN_TSEG2  3
#define CAN_SJW    1
#define CAN_BTR_INIT (CAN_BTR_SILM | CAN_BTR_LBKM) // Loopback, Silent Mode (self-test)
//#define CAN_BTR_INIT 0 // normal mode

#define CAN_MCR_INIT CAN_MCR_ABOM

#endif /* _CAN_CONF_H_ */
