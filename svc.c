/*
 * svc.c
 *
 *  Created on: 2020. szept. 29.
 *      Author: liptak
 */

#include <stdint.h>

#include "controller.h"
#include "FaultHandler.h"

#include "svc.h"

extern void SVCHandler_main(uint32_t * svc_args);

/* INTERRUPT */ void SVC_Handler(void)
{
//    "IMPORT SVCHandler_main\n\t"
    __ASM volatile (
    "TST lr, #4\n\t"
    "ITE EQ\n\t"
    "MRSEQ r0, MSP\n\t"
    "MRSNE r0, PSP\n\t"
    "B SVCHandler_main\n\t");
}

void SVCHandler_main(uint32_t * svc_args)
{
    uint8_t svc_number;
    /*
    * Stack contains:
    * r0, r1, r2, r3, r12, r14, LR (the return address) and xPSR
    * First argument (r0) is svc_args[0]
    */
    svc_number = ((uint8_t *)svc_args[6])[-2];
    switch(svc_number)
    {
        case 0: /* Handle SVC 00 - return back to thread mode */
            //__ASM("ADD sp,sp,#(1*4)"); /* pop the pushed stack */
          #if 1
            /* remove one 8-register exception frame */
            __ASM("ADD sp,sp,#(8*4)");
          #else
            /* remove one 8-register exception frame */
            /* plus the "aligner" from the stack */
            __ASM("ADD sp,sp,#(9*4)");
          #endif
            /* return to the preempted task */
            __ASM("BX lr"); /* exception-return to the scheduler */
        break;
        case 1: /* Handle SVC 01 - lock semaphore */
        {
            uint8_t *ptr = (uint8_t *)(svc_args[0]);
            if (*ptr == 0)
            {
                uint8_t locker = (uint8_t)(svc_args[1] & 0xFF);
                *ptr = locker;
            }
        }
        break;
        case 2: /* Handle SVC 02 - exclusive add word - 32 bits */
        {
            uint32_t *ptr = (uint32_t *)(svc_args[0]);
            uint32_t data = svc_args[1];
            *ptr = *ptr + data;
        }
        break;
        case 3: /* Handle SVC 03 - exclusive add half word - 16 bits */
        {
            uint16_t *ptr = (uint16_t *)(svc_args[0]);
            uint16_t data = (uint16_t)svc_args[1];
            *ptr = *ptr + data;
        }
        break;
        case 4: /* Handle SVC 04 - exclusive add byte - 8 bits */
        {
            uint8_t *ptr = (uint8_t *)(svc_args[0]);
            uint8_t data = (uint8_t)svc_args[1];
            *ptr = *ptr + data;
        }
        break;
        case 5: /* Handle SVC 05 - exclusive add byte - 8 bits */
        {
            __enable_irq();
        }
        break;
        case 6: /* Handle SVC 06 - exclusive add byte - 8 bits */
        {
            __disable_irq();
        }
        break;
        default: /* Unknown SVC */
            CAT_Error(CAT_InvalidISR, (SCB->ICSR & 0x1FF));
        break;
    }
}

void svc_ret(void)
{
  // ???????????????????????????????
  //__ASM("CPSIE i"); /* enable interrupts to allow SVCall exception*/
  /*
   * FPU handling
   */
  __ASM("SVC #0");
}

int8_t lockSemaphore(uint8_t *semaphorePtr, uint8_t locker)
{
    __ASM("SVC #01");
    if (*semaphorePtr == locker)
        return 1;
    else
        return 0;
}

void atomicAddU32(uint32_t *address, uint32_t data)
{
    (void)address;
    (void)data;
    __ASM("SVC #02");
}

void atomicAddU16(uint16_t *address, uint16_t data)
{
    (void)address;
    (void)data;
    __ASM("SVC #03");
}

void atomicAddU8(uint8_t *address, uint8_t data)
{
    (void)address;
    (void)data;
    __ASM("SVC #04");
}

void enableInterrupt(void)
{
    __ASM("SVC #05");
}

void disableInterrupt(void)
{
    __ASM("SVC #06");
}
