#ifndef _FAULTHANDLER_H_
#define _FAULTHANDLER_H_

#include <stdint.h>

typedef enum {
  CAT_Exception_0,
  CAT_Exception_1,
  CAT_Exception_2,
  CAT_Exception_3,
  CAT_Exception_4,
  CAT_InvalidISR,
  CAT_SchedulerOverrun,
  CAT_TaskOverrun_1ms,
  CAT_TaskOverrun_10ms,
  CAT_TaskOverrun_500ms,
  CAT_InvalidParameter,
}t_CAT_ErrorCode;

void CAT_Error(t_CAT_ErrorCode Code, uint32_t SubCode);

#endif /* _FAULTHANDLER_H_ */
