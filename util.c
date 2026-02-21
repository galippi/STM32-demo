#include <string.h> /* necessary for memset */

#include "SysClock_conf.h"
#include "util.h"

t_DivU32 udiv(uint32_t dividend, uint32_t divisor)
{
    t_DivU32 result;
    if (divisor == 0)
    { /* error case -> overflow */
        result.result = 0xFFFFFFFF;
        result.remainder = dividend;
        return result;
    }
    if (dividend < divisor)
    {
        result.result = 0;
        result.remainder = dividend;
        return result;
    }
    if ((divisor & (divisor - 1)) == 0)
    { /* the divisor is power of 2 */
      result.remainder = dividend & (divisor - 1);
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
      result.result = dividend;
      return result;
    }else
    {
      uint32_t resultU32 = 0;
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
          resultU32 = resultU32 + (1 << shift);
          dividend = dividend - divisor;
          if (dividend == 0)
          { /* no remainder -> return immediately */
            result.remainder = 0;
            result.result = resultU32;
            return result;
          }
        }
        shift--;
        divisor = divisor >> 1;
      }
      result.remainder = dividend;
      result.result = resultU32;
      return result;
    }
}

uint32_t DivU32_U32U32(uint32_t dividend, uint32_t divisor)
{
    t_DivU32 result = udiv(dividend, divisor);
    return result.result;
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
  volatile uint32_t wait = t_us * (f_AHB_Hz / 1000000 / 12);
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
