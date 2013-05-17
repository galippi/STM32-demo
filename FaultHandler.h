#ifndef _FAULTHANDLER_H_
#define _FAULTHANDLER_H_

//#include <stdint.h>

typedef enum {
  CAT_Exception_0,
  CAT_Exception_1,
  CAT_Exception_2,
  CAT_Exception_3,
  CAT_Exception_4,
  CAT_TaskOverrun_1ms,
  CAT_TaskOverrun_10ms,
  CAT_TaskOverrun_500ms,
}t_CAN_ErrorCode;

static inline void CAT_Error(t_CAN_ErrorCode code)
{
  (void)code;
  while(1)
    ; /* endless loop */
}

#endif /* _FAULTHANDLER_H_ */
