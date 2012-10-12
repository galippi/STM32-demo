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

#define TEST_BUFFER_ID 0x3000
  TestBuffer[0] =  TEST_BUFFER_ID & 0xFF; /* message ID - 2 bytes */
  TestBuffer[1] = (TEST_BUFFER_ID >> 8 )& 0xFF;
  /* message data 2..(TEST_BUFFER_LEN-3) will be filled by the application */
  TestBuffer[10] =  (TEST_BUFFER_ID+1)       & 0xFF; /* message ID - 2 bytes */
  TestBuffer[11] = ((TEST_BUFFER_ID+1) >> 8 )& 0xFF;
  {
    TestBuffer[12] = 0x00;
    TestBuffer[13] = 0x0F;
    TestBuffer[14] = 0xF0;
    TestBuffer[15] = 0xAA;
    TestBuffer[16] = 0x55;
    TestBuffer[17] = 0x80;
    TestBuffer[18] = 0x01;
    TestBuffer[19] = 0x00;
  }
  TestBuffer[TEST_BUFFER_LEN - 2] = 0; /* checksum 2 bytes */
  TestBuffer[TEST_BUFFER_LEN - 1] = 0;
}

void DebugOut(void)
{
  UART2_TX(TestBuffer, sizeof(TestBuffer));
}
