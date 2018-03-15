#include "gpio.h"
#include "uart.h"

#include "bluetooth_hc05_conf.h"

#include "bluetooth_hc05.h"

static const uint8_t TestData0[4] = {'A', 'T', '\r', '\n'};
static const uint8_t TestData1[13] = "AT+VERSION?\r\n";
//static const uint8_t TestData2[18] = "AT\r\nHello World!\n\r";
static const uint8_t TestData2[10] = "AT+UART?\n\r";
//static const uint8_t TestData2[8] = "AT+UART?";
//static const uint8_t TestData3[19] = "AT+UART=19200,0,0\n\r";
static const uint8_t TestData3[19] = "AT+UART=38400,0,0\n\r";

void Bluetooth_Init(void)
{
  #ifdef HC05_ResetPort
  GPIO_PortInit_Out(HC05_ResetPort, 0); /* Bluetooth - reset */
  GPIO_Set(HC05_ResetPort, HC05_ResetPin, 0); /* set Bluetooth reset to low */
  #endif
  #ifdef HC05_KeyPort
  GPIO_Set(HC05_KeyPort, HC05_KeyPin, 0); /* set Bluetooth key to low */
  GPIO_PortInit_Out(HC05_KeyPort, HC05_KeyPin); /* Bluetooth - key */
  GPIO_Set(HC05_KeyPort, HC05_KeyPin, 0); /* set Bluetooth key to low */
  #endif
  #ifdef HC05_EnablePort
  GPIO_PortInit_Out(HC05_EnablePort, HC05_EnablePin); /* Bluetooth - enable */
  GPIO_Set(HC05_EnablePort, HC05_EnablePin, 1); /* set Bluetooth enable to high */
  #endif
  //UART2_Init(9600); /* initializing UART for HC-05 module */
  //UART2_Init(19200); /* initializing UART for HC-05 module */
  UART2_Init(38400); /* initializing UART for HC-05 module */

  #ifdef HC05_ResetPort
  { /* wait a little to reset HW the module */
    volatile int wait = 10000;
    while (wait > 0)
    {
      wait--;
    }
  }
  GPIO_Set(HC05_ResetPort, HC05_ResetPin, 1); /* set Bluetooth reset to high - start the HW module again */
  #endif

  #ifdef HC05_DebugPort
  GPIO_PortInit_Out(HC05_DebugPort, HC05_DebugPin); /* UART2-debug port */
  #endif
}

static volatile uint8_t dummy_data;

void Bluetooth_Task_10ms(void)
{
  static uint8_t TestDataIdx = 0;
  if (TestDataIdx == 0)
  {
    #ifdef HC05_KeyPort
    GPIO_Set(HC05_KeyPort, HC05_KeyPin, 1); /* set Bluetooth key to high - AT mode */
    #endif
  }else
  if (TestDataIdx == 3)
  {
    UART2_Tx(TestData0, sizeof(TestData0));
  }else
  if (TestDataIdx == 5)
  {
    UART2_Tx(TestData1, sizeof(TestData1));
  }else
  if (TestDataIdx == 7)
  {
    //UART2_Tx(TestData2, sizeof(TestData2));
  }else
  if (TestDataIdx == 10)
  {
    UART2_Tx(TestData3, sizeof(TestData3));
  }else
  if (TestDataIdx == 13)
  {
    UART2_Tx(TestData0, sizeof(TestData0));
  }else
  if (TestDataIdx == 31)
  {
    #ifdef HC05_KeyPort
    GPIO_Set(HC05_KeyPort, HC05_KeyPin, 0); /* set Bluetooth key to low  - normal mode*/
    #endif
    //UART2_Init(115200); /* initializing UART for HC-05 module */
  }else
  if ((TestDataIdx >= 32) && ((TestDataIdx & 0x1F) == 0))
  {
    dummy_data++;
    //UART2_Tx(TestData2, sizeof(TestData2));
  }else
  if ((TestDataIdx >= 32) && ((TestDataIdx & 0x07) == 1))
  {
#if 1
    static uint8_t idx;
    static char string[] = "    0123456789012345678901234567890123456789012345678901234567890123456789\n\r";
    string[0] = (idx / 100) + '0';
    string[1] = ((idx % 100) / 10) + '0';
    string[2] = (idx % 10) + '0';
    idx++;
    UART2_Tx((uint8_t*)string, sizeof(string));
#endif
  }else
  if ((TestDataIdx >= 32) && ((TestDataIdx & 0x07) == 6))
  {
    static const uint8_t buf[4] = { 0xA5, 0x5A, 0xEB, 0xFF};
    #ifdef HC05_DebugPort
    GPIO_Set(HC05_DebugPort, HC05_DebugPin, 0); /* toggle the UART debug pin */
    wait_us(1);
    GPIO_Set(HC05_DebugPort, HC05_DebugPin, 1); /* toggle the UART debug pin */
    #endif
    UART2_Tx(buf, sizeof(buf));
    #ifdef HC05_DebugPort
    GPIO_Set(HC05_DebugPort, HC05_DebugPin, 0); /* toggle the UART debug pin */
    #endif
  }else
  { /* do nothing */
  }
  if (TestDataIdx < 255)
  {
    TestDataIdx++;
  }else
  { /* repeat the "Hello world" sending */
    //TestDataIdx = 32;
    TestDataIdx = 0;
  }
}
