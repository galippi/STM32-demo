#ifndef _LWSLCAN_CONF_H_
#define _LWSLCAN_CONF_H_

#include "timer_app.h"

typedef struct {
    uint8_t uart1_rxCtr;
    uint8_t CAN_txCtr;
    uint8_t CAN_txFull;
}t_LwSlcanDbg;

extern t_LwSlcanDbg dbgLwSlcan;

#define SLCAN_NEWLINE_STR "\n"
#define SLCAN_NEWLINE_PUT(str_ptr) (*(str_ptr) = '\n', (str_ptr)++, (str_ptr))

#if 1
#define DBG_INC(var) do {dbgLwSlcan.var++;}while(0)
#else
#define DBG_INC(var) do {}while(0) /* do nothing */
#endif

extern uint16_t LwSlcan_1us_ctr;
extern uint16_t LwSlcan_1ms_ctr;

static inline uint16_t get_1ms_ctr(void)
{
    uint16_t t = getTimer_us();
    uint16_t dt = t - LwSlcan_1us_ctr;
    if (dt >= 1000)
    {
        LwSlcan_1ms_ctr++;
        LwSlcan_1us_ctr = LwSlcan_1us_ctr + 1000;
    }
    return LwSlcan_1ms_ctr;
}

#endif /* _LWSLCAN_CONF_H_ */
