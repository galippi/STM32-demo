#ifndef _USB_IF_H_
#define _USB_IF_H_ 1

void MX_USB_DEVICE_Init(void);

void USB_HP_CAN1_TX_IRQHandler(void);
void USB_LP_CAN1_RX0_IRQHandler(void);
void USBWakeUp_IRQHandler(void);

#endif /* _USB_IF_H_ */
