#include "FaultHandler.h"

t_CAT_ErrorCode CAT_Error_Code;
uint32_t CAT_Error_SubCode;

void CAT_Error(t_CAT_ErrorCode Code, uint32_t SubCode)
{
  CAT_Error_Code = Code;
  CAT_Error_SubCode = SubCode;
  while(1)
    ; /* endless loop */
}
