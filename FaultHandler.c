#include "util.h"
#include "uart.h"
#include "timer_conf.h"
#include "timer.h"
#include "u32_to_hexstring.h"

#include "FaultHandler.h"

t_CAT_ErrorCode CAT_Error_Code;
uint32_t CAT_Error_SubCode;

#if 1

static void CAT_SystemInit(void) {

}

static void CAT_SendData(const void *dataPtr, uint32_t len) {
    const uint8_t *ptr = dataPtr;
    while(len != 0) {
        while (UART1_TXE_Get() == 0)
            ; // wait while the UART buffer becomes empty
        {
          USART1->DR = *ptr;
          ptr++;
          len--;
        }
    }
}

static void toHexString(void *dst, uint32_t val, unsigned len) {
    char *ptr = (char*)dst + len - 1;
    while(len > 0) {
        uint8_t valU8 = val & 0xFF;
        if (len & 0x01)
            valU8 = valU8 >> 4;
        val = val >> 8;
        *ptr = toHexDigit(valU8);
        ptr--;
        len--;
    }
}

static void CAT_SendDebugData(void) {
    static char debugData[] = "CATxxxxyyyy\r";
    //toHexString(debugData + 3, CAT_Error_Code, 4);
    U32_to_HexString(debugData + 3, 4, CAT_Error_Code, '0');
    //toHexString(debugData + 7, CAT_Error_SubCode, 4);
    U32_to_HexString(debugData + 7, 4, CAT_Error_SubCode, '0');
    CAT_SendData(debugData, sizeof(debugData) - 1);
}

/**
 * It waits for specified time
 * @param ms waiting time in ms
 */
static void CAT_WaitMs(uint32_t ms) {
    uint16_t t_start = TIM3_Cnt_Get();
    while(ms != 0) {
        while (1) {
            uint16_t t = TIM3_Cnt_Get();
            if (((t - t_start) & 0xFFFF) >= TIM3_1ms) {
                t_start += TIM3_1ms;
                break;
            }
        }
        ms--;
    }
}

#define CAT_StackReinit() /* not yet implemented */

void CAT_Error(t_CAT_ErrorCode Code, uint32_t SubCode)
{
  __disable_irq();
  CAT_Error_Code = Code;
  CAT_Error_SubCode = SubCode;

  CAT_StackReinit();

  CAT_SystemInit();
  while(1) { /* endless loop */
      CAT_SendDebugData();
      systemSleepMs(1000);
      //CAT_WaitMs(1000);
  }
}

#else

#include "controller.h"

void CAT_Error(t_CAT_ErrorCode Code, uint32_t SubCode)
{
    SCB->ICSR = SCB_ICSR_NMIPENDSET_Msk; // generate NMI to disable all interrupts
    CAT_Error_Code = Code;
    CAT_Error_SubCode = SubCode;
    while(1)
      ; /* endless loop */
}

#endif
