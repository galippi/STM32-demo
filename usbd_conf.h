#ifndef _USBD_CONF_H_
#define _USBD_CONF_H_

#include "controller.h"

#include "stm32g0xx_hal_pcd.h"

#include "FaultHandler.h"

#ifndef UNUSED
#define UNUSED(X) (void)X      /* To avoid gcc/g++ warnings */
#endif

#ifndef __PACKED
#define __PACKED __attribute__((__packed__))
#endif

#define USBD_CLASS_BOS_ENABLED 0

/*---------- -----------*/
#define USBD_MAX_NUM_INTERFACES     1
/*---------- -----------*/
#define USBD_MAX_NUM_CONFIGURATION     1
/*---------- -----------*/
#define USBD_MAX_STR_DESC_SIZ     512
/*---------- -----------*/
#define USBD_DEBUG_LEVEL     0
/*---------- -----------*/
#define USBD_SELF_POWERED     1
/*---------- -----------*/
#define MAX_STATIC_ALLOC_SIZE     512

/****************************************/
/* #define for FS and HS identification */
#define DEVICE_FS 		0

/** Alias for memory allocation. */
void *USBD_static_malloc(uint32_t size);
#define USBD_malloc         (uint32_t *)USBD_static_malloc

/** Alias for memory release. */
void USBD_static_free(void *p);
#define USBD_free           USBD_static_free

static inline void Error_Handler(void)
{
  CAT_Error(CAT_USBError, 0);
}

void USB_task_init(void);
void USB_task_1ms(void);

void USB_LP_IRQHandler(void);
void USB_LP_IRQErrorCb(PCD_HandleTypeDef *hpcd);

#endif /* _USBD_CONF_H_ */
