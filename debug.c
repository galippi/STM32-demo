/******************************************************************************
 *
 * \file
 * \ingroup debug
 * \brief   Declarations for debug modules.
 *
 * \details Usable functions/variables to debugging/measuring the system.
 *
 ******************************************************************************/
/** \addtogroup debug
 *  @{
 ******************************************************************************/

#include "uart.h"

#include "debug.h"

uint8_t DebugId = 1;

uint8_t TestBuffer[TEST_BUFFER_LEN];

void DebugOut_Init(void)
{
  DebugId = 1;

#define TEST_BUFFER_ID 0x3001
  TestBuffer[0] = TEST_BUFFER_ID & 0xFF; /* message ID - 2 bytes */
  TestBuffer[1] = (TEST_BUFFER_ID >> 8 )& 0xFF;
  /* message data 2..(TEST_BUFFER_LEN-3) will be filled by the application */
  {
    TestBuffer[2] = 0x00;
    TestBuffer[3] = 0x0F;
    TestBuffer[4] = 0xF0;
    TestBuffer[5] = 0xAA;
    TestBuffer[6] = 0x55;
    TestBuffer[7] = 0x80;
    TestBuffer[8] = 0x01;
    TestBuffer[9] = 0x00;
  }
  TestBuffer[TEST_BUFFER_LEN - 2] = 0; /* checksum 2 bytes */
  TestBuffer[TEST_BUFFER_LEN - 1] = 0;
}

void DebugOut(void)
{
  UART2_TX(TestBuffer, sizeof(TestBuffer));
}
