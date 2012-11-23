/******************************************************************************
 *
 * \file
 * \ingroup RESET
 * \brief   Interrupt vector table
 *
 *
 * \details Fill me
 *
 ******************************************************************************/
/** \addtogroup RESET
 *  @{
 ******************************************************************************/
#include <stdint.h>

#include "vector.h"

extern void reset(void);
extern void ExceptionHandler(void);
extern void ISR_Invalid(void);

extern const uint32_t const *SP_INIT;

const uint32_t const *SP_INIT = 0;

t_func_ptr const ISR_VectorTable[] =
{
  (const t_func_ptr) &SP_INIT, /* stack top address */
  reset,             /* 1 Reset */
  ExceptionHandler,  /* 2 NMI */
  ExceptionHandler,  /* 3 HardFault */
  ExceptionHandler,  /* 4 MemManage */
  ExceptionHandler,  /* 5 BusFault */
  ExceptionHandler,  /* 6 UsageFault */
  ISR_Invalid,       /* 7 RESERVED */
  ISR_Invalid,       /* 8 RESERVED */
  ISR_Invalid,       /* 9 RESERVED*/
  ISR_Invalid,       /* 10 RESERVED */
  ISR_Invalid,       /* 11 SVCall */
  ISR_Invalid,       /* 12 Debug Monitor */
  ISR_Invalid,       /* 13 RESERVED */
  ISR_Invalid,       /* 14 PendSV */
  ISR_Invalid,       /* 15 SysTick */
  ISR_Invalid,       /* 16 External Interrupt(0) */
  ISR_Invalid,       /* 17 External Interrupt(1) */
  ISR_Invalid,       /* 18 External Interrupt(2) */
  ISR_Invalid,       /* 19 ...   */
};
