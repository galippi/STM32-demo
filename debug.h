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

#define TEST_BUFFER_LEN 12 /* 2 byte ID, 8 byte data, 2 byte checksum */
extern uint8_t TestBuffer[TEST_BUFFER_LEN];

void DebugOut_Init(void);
void DebugOut(void);

#endif /* _DEBUG_H_ */
