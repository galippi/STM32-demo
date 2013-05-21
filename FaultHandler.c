#include "FaultHandler.h"

t_CAT_ErrorCode code;

void CAT_Error(t_CAT_ErrorCode _code)
{
  code = _code;
  while(1)
    ; /* endless loop */
}
