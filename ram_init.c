/******************************************************************************
 *
 * \file
 * \ingroup RAM_INIT
 * \brief   Template sourcefile of Template Component.
 *
 * \details Fill me
 *
 ******************************************************************************/
/** \addtogroup RAM_INIT
 *  @{
 ******************************************************************************/

#include <stdint.h>

#include "ram_init.h"

extern uint32_t __bss_start;
extern uint32_t __bss_end;
extern uint32_t __data_start;
extern uint32_t __data_end;
extern const uint32_t __data_load;

//int var1 = 5;

//char var2 = 3;

void RAM_Init(void)
{
  { /* filling up the not initialized RAM with 0 */
    uint32_t *ptr = &__bss_start;
    while (ptr < &__bss_end)
    {
      *ptr = 0;
      ptr++;
    }
  }
  { /* filling up the initialized RAM with the init values */
    uint32_t const *src = &__data_load;
    uint32_t *dst = &__data_start;
    while (dst < &__data_end)
    {
      *dst = *src;
      dst++;
      src++;
    }
  }
}
