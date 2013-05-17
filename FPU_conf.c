#include "controller.h"
#include "FPU_conf.h"

void FPU_Init(void)
{
  #if FPU_MODE == FPU_OFF

  /* FPU is turned off at reset -> do nothing */

  #else /* FPU_MODE == FPU_ON */

  SCB->CPACR |= (0xFU << 20); /* Turn FPU on */

  #if FPU_MODE == FPU_ON_APP_ONLY

  /* turn off the automatic FPU state preservation and do not use the lazy stacking */
  FPU->FPCCR &= ~((1U << FPU_FPCCR_ASPEN_Pos) | (1U << FPU_FPCCR_LSPEN_Pos));

  #else /* FPU_MODE == FPU_ON_ALWAYS */

  /* automatic FPU state preservation is on and use the lazy stacking */
  FPU->FPCCR |= (1U << FPU_FPCCR_ASPEN_Pos) | (1U << FPU_FPCCR_LSPEN_Pos);

  #endif /* FPU_MODE == FPU_ON_ALWAYS */

  #endif /* FPU_MODE == FPU_ON */
}
