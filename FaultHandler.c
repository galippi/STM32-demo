#include "FaultHandler.h"

void CAT_Error(uint8_t code)
{
  (void)code;
  while(1)
    ; /* endless loop */
}
