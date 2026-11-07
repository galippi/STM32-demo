#include "controller.h"

#include "gpio_app.h"
#include "FaultHandler.h"

typedef struct {
    t_CAT_ErrorCode errorCode;
    uint32_t        errorSubCode;
}t_CAT_store;

t_CAT_store CAT_store;

void CAT_Error(t_CAT_ErrorCode Code, uint32_t SubCode)
{
  __disable_irq();
  LEDR_Set(0); // turn on red led
  CAT_store.errorCode    = Code;
  CAT_store.errorSubCode = SubCode;
  while(1)
    ; /* endless loop */
}
