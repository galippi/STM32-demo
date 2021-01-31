#include <string.h> /* necessary for memset */

#include "SysClock_conf.h"
#include "util.h"

uint32_t DivU32_U32U32(uint32_t dividend, uint32_t divisor)
{
  if (divisor == 0)
  { /* error case -> overflow */
    return 0xFFFFFFFF;
  }
  if (dividend < divisor)
  {
    return 0;
  }
  if ((divisor & (divisor - 1)) == 0)
  { /* the divisor is power of 2 */
    while (divisor != 1)
    {
      if ((divisor & 0xFF) == 0)
      {
        divisor = divisor >> 8;
        dividend = dividend >> 8;
      }else
      {
        divisor = divisor >> 1;
        dividend = dividend >> 1;
      }
    }
    return dividend;
  }else
  {
    uint32_t result = 0;
    int32_t shift = 0;
    if (dividend & 0x80000000)
    { /* the upper bit of the value is set -> take care of it */
      while ((divisor & 0x80000000) == 0)
      {
        divisor = divisor << 1;
        shift++;
      }
    }else
    {
      while (dividend > divisor)
      {
        divisor = divisor << 1;
        shift++;
      }
    }
    while (shift >= 0)
    {
      if (dividend >= divisor)
      {
        result = result + (1 << shift);
        dividend = dividend - divisor;
        if (dividend == 0)
        { /* no remainder -> return immediately */
          return result;
        }
      }
      shift--;
      divisor = divisor >> 1;
    }
    return result;
  }
}

int32_t DivI32_I32I32(int32_t dividend, int32_t divisor)
{
  int32_t sign = 1;
  uint32_t dividend_u;
  uint32_t divisor_u;
  uint32_t result_u;
  if (dividend >= 0)
  {
    dividend_u = (uint32_t) dividend;
  }else
  {
    dividend_u = (uint32_t) -dividend;
    sign = -sign;
  }
  if (divisor >= 0)
  {
    divisor_u = (uint32_t) divisor;
  }else
  {
    divisor_u = (uint32_t) -divisor;
    sign = -sign;
  }
  result_u = DivU32_U32U32(dividend_u, divisor_u);
  if (sign > 0)
  {
    return (int32_t)result_u;
  }else
  {
    return -(int32_t)result_u;
  }
}

void *memset(void *ptr_, int data, size_t size)
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

void wait_us_rough(uint32_t t_us)
{
  volatile uint32_t wait = t_us * (f_AHB_Hz / 1000000 / 2);
  while (wait > 0)
  {
    wait--;
  }
}

int strcmp(const char *str1, const char *str2)
{
    while((*str1 != 0) && (*str1 == *str2))
    {
        str1++;
        str2++;
    }
    return ((*str1 == *str2) ? 0 : ((*str1 < *str2) ? -1 : +1));
}

size_t strlen(const char *str)
{
    size_t len = 0;
    while(*str != 0)
    {
        str++;
        len++;
    }
    return len;
}

char *strcat(char *dst, const char *src)
{
    while(*dst != 0)
    {
        dst++;
    }
    while(*src != 0)
    {
        *dst = *src;
        dst++;
        src++;
    }
    *dst = 0;
    return dst;
}

void systemSleepMs(uint32_t ms)
{
    while(ms > 0)
    {
        wait_us_rough(1000);
        ms--;
    }
}
