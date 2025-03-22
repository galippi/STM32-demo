#include "FaultHandler.h"
#include "uart.h"
#include "timer_conf.h"
#include "timer.h"

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

static void CAT_SendDebugData(void) {
    static char debugData[] = "CATxxxxyyyy\r";
    CAT_SendData(debugData, sizeof(debugData) - 1);
}

/**
 * It waits for specified time
 * @param ms waiting time in ms
 */
static void CAT_WaitMs(uint32_t ms) {
    uint16_t t_start = TIM3_Cnt_Get();
    while(ms != 0) {
        //TIM3_FREQ
        while (1) {
            uint16_t t = TIM3_Cnt_Get();
            if ((t - t_start) > TIM3_1ms) {
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
      CAT_WaitMs(1000);
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
