/******************************************************************************
 *
 * \file
 * \ingroup debug
 * \brief   Prototypes for debug modules.
 *
 * \details Usable functions/variables to debugging/measuring the system.
 *
 ******************************************************************************/
/** \addtogroup debug
 *  @{
 ******************************************************************************/


#ifndef _DEBUG_H_
#define _DEBUG_H_

extern uint8_t DebugId;

//#define TEST_BUFFER_LEN 12 /* 2 byte ID, 8 byte data, 2 byte checksum */
#define TEST_BUFFER_LEN 202 /* 2 byte ID, 8 byte data, 2 byte checksum */
extern uint8_t TestBuffer[TEST_BUFFER_LEN];

void DebugOut_Init(void);
void DebugOut(void);

static inline void DebugOut_Set_u8(uint32_t block, uint32_t idx, uint8_t data)
{
  TestBuffer[block*10+2+idx] = data;
}

static inline void DebugOut_Set_u16(uint32_t block, uint32_t idx, uint16_t data)
{
  TestBuffer[block*10+2+idx+0] = (data     ) & 0xFF;
  TestBuffer[block*10+2+idx+1] = (data >> 8) & 0xFF;
}

typedef struct {
    uint16_t uart1_rxCtr;
    uint8_t uart1TxOverflowCtr;
    uint8_t UART1_TxOverrun;
    uint8_t clockHseInitError;
}t_dbgBuffer;
t_dbgBuffer dbgBuffer;

#define DBG_SET(field, val) dbgBuffer.field = (val)
#define DBG_INC(field) dbgBuffer.field++

#endif /* _DEBUG_H_ */
