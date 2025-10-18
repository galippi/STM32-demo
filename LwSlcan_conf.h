#ifndef _LWSLCAN_CONF_H_
#define _LWSLCAN_CONF_H_

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

#endif /* _LWSLCAN_CONF_H_ */
