#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdint.h>
#include <string.h>

#include "controller.h"

#define NUMOF(array) (sizeof(array) / sizeof(array[0]))

#define COMP_CHECK_VAL(vall, valr) \
    extern char __dummy_size_check[(vall) == (valr) ? 1 : -1];

#define COMP_CHECK_SIZE(var, size) COMP_CHECK_VAL(sizeof(var), size)

#define COMP_CHECK_ARRAY(array, length) COMP_CHECK_VAL(NUMOF(array), length)

typedef struct {
    uint32_t result;
    uint32_t remainder;
}t_DivU32;

#if __CORTEX_M == 33
static t_DivU32 udiv(uint32_t dividend, uint32_t divisor) {
    t_DivU32 result;
    result.result = dividend / divisor;
    result.remainder = dividend % divisor;
    return result;
}

static uint32_t DivU32_U32U32(uint32_t dividend, uint32_t divisor);
static int32_t DivI32_I32I32(int32_t dividend, int32_t divisor);

#else
t_DivU32 udiv(uint32_t dividend, uint32_t divisor);

uint32_t DivU32_U32U32(uint32_t dividend, uint32_t divisor);
int32_t DivI32_I32I32(int32_t dividend, int32_t divisor);
#endif

#if 0
static inline void *memset(void *ptr_, int data, size_t size)
{
  uint8_t *ptr = ptr_;
  while(size > 0)
  {
    *ptr = data;
    ptr++;
    size--;
  }
  return ptr_;
}
#else
void *memset(void *ptr_, int data, size_t size);
#endif

void wait_us_rough(uint32_t t_us);

void systemSleepMs(uint32_t ms);

#endif /* _UTIL_H_ */
