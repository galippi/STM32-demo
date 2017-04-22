
#include "main.h"
#include "ram_init.h"

#include "reset.h"

void reset(void)
{
  RAM_StartCheck();
  RAM_Init();
  main();
}
