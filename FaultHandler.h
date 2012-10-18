#ifndef _FAULTHANDLER_H_
#define _FAULTHANDLER_H_

#include <stdint.h>

static inline void CAT_Error(uint8_t code)
{
  (void)code;
  while(1)
    ; /* endless loop */
}

#endif /* _FAULTHANDLER_H_ */
