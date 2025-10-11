typedef struct {
    uint8_t uart1_rxCtr;
    uint8_t CAN_txCtr;
    uint8_t CAN_txFull;
}t_LwSlcanDbg;

extern t_LwSlcanDbg dbgLwSlcan;

#if 1
#define DBG_INC(var) do {dbgLwSlcan.var++;}while(0)
#else
#define DBG_INC(var) do {}while(0) /* do nothing */
#endif
