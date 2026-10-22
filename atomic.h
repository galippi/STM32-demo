#ifndef _ATOMIC_H_
#define _ATOMIC_H_

static uint8_t atomicSetU8(volatile uint8_t *var, uint8_t newVal)
{
  if (__LDREXB(var) == newVal) {
      __CLREX();
  }else
  if (__STREXB(newVal, var) == 0)
      return 1; // succesful set
  return 0; // not succesful
}

#endif
