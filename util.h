#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdint.h>
#include <string.h>

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

#endif /* _UTIL_H_ */
