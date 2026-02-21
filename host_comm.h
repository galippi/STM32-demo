#ifndef _HOST_COMM_H_
#define _HOST_COMM_H_

#if 0

#define hostInit() /* UART1_Init(9600, 1) */
#define hostTx(dataPtr, len) /* UART1_TX_Queue(dataPtr, len) */
#define hostRx(dataPtr, bufLen) 0 /* UART1_RX(dataPtr, bufLen) */

#elif 0

#include "uart.h"

#define hostInit() UART1_Init(9600, 1)
#define hostTx(dataPtr, len) UART1_TX_Queue(dataPtr, len)
#define hostRx(dataPtr, bufLen) UART1_RX(dataPtr, bufLen)

#else

#include "usb_device.h"
#define __STM32F103xB_H
#define __STM32F1XX_H
#include "usbd_cdc_if.h"

//#include "usbd_conf.h"

#define hostInit()   MX_USB_DEVICE_Init()
#define hostTx(dataPtr, len) usbTx(dataPtr, len)
#define hostRx(dataPtr, bufLen) usbRx(dataPtr, bufLen)
#define hostUpdate() usb_10ms()

#endif

#endif /* _HOST_COMM_H_ */
