#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdint.h>
#include <string.h>

#define ARRAY_SIZE(array) (sizeof(array)/sizeof(array[0]))

#define COMPILE_TIME_CHECK(value, expected) \
    extern uint8_t __dummy_size_check__[(value) == (expected) ? 1 : -1];

#define SIZE_CHECK(var, expected) COMPILE_TIME_CHECK(sizeof(var), expected)

#define SIZE_CHECK_ARRAY(array, expected) COMPILE_TIME_CHECK(ARRAY_SIZE(array), expected)

uint32_t DivU32_U32U32(uint32_t dividend, uint32_t divisor);
int32_t DivI32_I32I32(int32_t dividend, int32_t divisor);

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

#endif /* _UTIL_H_ */
