#include <stdint.h>
#include <string.h>

#include "SysClock.h"
#include "gpio_app.h"
//#include "adc.h"
//#include "adc_app.h"
#include "timer.h"
#include "util.h"
#include "system_conf.h"
#include "timer_conf.h"
#include "timer_app.h"
//#include "scheduler_preemptive.h"
#include "tasks.h"
#include "FaultHandler.h"
#include "vector.h"

#include "main.h"

#define VDD 3.0 /* Volt */

#define DBG_PORTS \
	DBG_PORT(GPIO_TypeDef, gpioa, GPIOA) \
	DBG_PORT(GPIO_TypeDef, gpiob, GPIOB) \
	DBG_PORT(GPIO_TypeDef, gpioc, GPIOC) \
	DBG_PORT(RCC_TypeDef, rcc, RCC) \
	DBG_PORT(SysTick_Type, systick, SysTick) \
	DBG_PORT(ADC_TypeDef, adc1, ADC1) \
	DBG_PORT(TIM_TypeDef, tim3, TIM3) \
    DBG_PORT(TIM_TypeDef, tim2, TIM14) \
	DBG_PORT(SCB_Type, scb, SCB) \
	DBG_PORT(NVIC_Type, nvic, NVIC) \
	DBG_PORT(DMA_TypeDef, dma1, DMA1) \
    DBG_PORT(DMA_Channel_TypeDef, dma1_4, DMA1_Channel4) \
    DBG_PORT(DMA_Channel_TypeDef, dma1_5, DMA1_Channel5) \
	DBG_PORT(USART_TypeDef, uart1, USART1) \
  /* no more peripheries */

#undef DBG_PORT
#define DBG_PORT(type, field, val) type * const field;

typedef struct {
    DBG_PORTS
}t_DBG_Ports;

#undef DBG_PORT
#define DBG_PORT(type, field, val) val,

t_DBG_Ports dbg_ports = {
	DBG_PORTS
};

/** \brief  Set Stack Pointer

    This function assigns the given value to the current Stack Pointer (SP).

    \param [in]    topOfStack  Process Stack Pointer value to set
 */
__attribute__( ( always_inline ) ) static __INLINE void __set_SP(uint32_t topOfStack)
{
  __ASM volatile ("MOV sp, %0\n" : : "r" (topOfStack) );
}
/** \brief  Get PC

    This function returns the current value of the PC register.

    \return               PC register value
 */
__attribute__( ( always_inline ) ) static __INLINE uint32_t __get_PC(void)
{
  uint32_t result;

  __ASM volatile ("MOV %0, pc" : "=r" (result) );
  return(result);
}

void RAM_StartCheck(void)
{
#define RAM_START_ADDR 0x20000000
#define RAM_END_ADDR   0x20001FFF
#define VECTOR_START_ADDR ((uint32_t *)RAM_START_ADDR)
#define NUM_VECTOR_NUMBER 32
#define VECTOR_END_ADDR (VECTOR_START_ADDR + NUM_VECTOR_NUMBER)
  uint32_t *ptr;
  uint32_t pc = __get_PC();
  if ((pc >= RAM_START_ADDR) && (pc < RAM_END_ADDR))
  { /* the program runs already in RAM */
    return;
  }
  for (ptr = VECTOR_START_ADDR; ptr < VECTOR_END_ADDR; ptr++)
  {
    if ((*ptr < RAM_START_ADDR) || (*ptr >= RAM_END_ADDR))
    {
      break;
    }
  }
  if (ptr == VECTOR_END_ADDR)
  { /* all vectors valid RAM-pointers -> start the RAM-function */
    __set_SP(*(uint32_t*)RAM_START_ADDR);
    typedef void (*t_func_ptr)(void);
    t_func_ptr RAM_main = (t_func_ptr)*(((uint32_t*)RAM_START_ADDR) + 1);
    RAM_main();
  }
}

int main(void)
{
  SCB->VTOR = (uint32_t)&ISR_VectorTable[0];
  SysClock_Init();
  SysTick_Init();

  DBG->APBFZ2 |= DBG_APB_FZ2_DBG_TIM14_STOP; /* stop scheduler timer */

#if 0
  // MCO out is turned on
  GPIO_PortInit_AFOut(GPIOA, 8); /* Set Port A8 to AF */
  RCC->CFGR &= RCC_CFGR_MCO;
  RCC->CFGR |= RCC_CFGR_MCO_PLL; // MCO output is configured to PLL
#endif
  //RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
  //AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE; /* disable JTAG, enable SWD - PA15, PB3, PB4: free; PA13, PA14: in use */
  LED3_Init();
  LED3_Set(0);
  LED3_Set(1);
  LED3_Set(0);
  LED3_Set(1);
  LED3_Set(0);
  LED3_Set(1);
  LED4_Init();
  LED5_Init();
  LED6_Init();
  PB13_Init();
  GPIO_PortInit_Analog(GPIOA, 0);
  GPIO_PortInit_Analog(GPIOA, 1);
  //ADC_HandlerInit();
  //UART2_Init();
  Task_Init();
  //SchedulerPre_Init();
  //TIM3_Init();
  //TIM3_CCR1_Set(TIM3_Cnt_Get() + TIM3_FREQ); /* set the first scheduler interrupt to 1ms */
  //NVIC->ISER[29/32] = NVIC->ISER[29/32] | (1 << (29%32)); /* enable TIM3 interrupt */
  //NVIC->IP[29] = 0x80; /* set TIM3 interrupt priority to medium */
  /* set PENDSV prio to 0xFF */
  SCB->SHP[14-4] = 0xFF; /* it shall be lower than the prio of the scheduler-timer interrupt */
  /* set SVC prio to 0x00 */
  SCB->SHP[11-4] = 0x10;

  while (1)
  {
    //ADC_Handler();
  }

  return 0;
}

void ExceptionHandler_0(void)
{
  CAT_Error(CAT_Exception_0, 0);
}

void ExceptionHandler_1(void)
{
  CAT_Error(CAT_Exception_1, 0);
}

void ExceptionHandler_2(void)
{
  CAT_Error(CAT_Exception_2, 0);
}

void ExceptionHandler_3(void)
{
  CAT_Error(CAT_Exception_3, 0);
}

void ExceptionHandler_4(void)
{
  CAT_Error(CAT_Exception_4, 0);
}

uint32_t tcnt0,tcnt1,tcnt2, ccr3_old, ccr3_new;
/* INTERRUPT */ void TIM14_ISR(void)
{
    if (TIM14_SR_UIF_Get())
    {
      TIM14_SR_UIF_Reset();
      TIM14_UIF_Callback();
    }else
    {
      CAT_Error(CAT_InvalidISR, (SCB->ICSR & 0x1FF) | ((TIM14->SR) << 16));
    }
}

/* INTERRUPT */ void TIM3_ISR(void)
{
  if (TIM3_SR_UIF_Get())
  {
    TIM3_SR_UIF_Reset();
    TIM3_UIF_Callback();
  }else
  if (TIM3_SR_CC1IF_Get())
  {
    TIM3_SR_CC1IF_Reset();
    TIM3_CC1IF_Callback();
  }else
  if (TIM3_SR_CC3IF_Get())
  {
    TIM3_SR_CC3IF_Reset();
    TIM3_SR_CC3OF_Reset();
    //TIM3_CC3IF_Callback();
  }else
   if (TIM3_SR_CC4IF_Get())
  {
    TIM3_SR_CC4IF_Reset();
    TIM3_SR_CC4OF_Reset();
    //TIM3_CC4IF_Callback();
  }else
  {
    CAT_Error(CAT_InvalidISR, (SCB->ICSR & 0x1FF) | ((TIM3->SR) << 16));
  }
}

#if 0
/* INTERRUPT */ void TIM4_ISR(void)
{
  if (TIM4_SR_UIF_Get())
  {
    TIM4_SR_UIF_Reset();
    TIM4_UIF_Callback();
  }else
  if (TIM4_SR_CC1IF_Get())
  {
    TIM4_SR_CC1IF_Reset();
    TIM4_CC1IF_Callback();
  }else
  if (TIM4_SR_CC3IF_Get())
  {
    TIM4_SR_CC3IF_Reset();
    TIM4_SR_CC3OF_Reset();
    TIM4_CC3IF_Callback();
  }else
   if (TIM4_SR_CC4IF_Get())
  {
    TIM4_SR_CC4IF_Reset();
    TIM4_SR_CC4OF_Reset();
    TIM4_CC4IF_Callback();
  }else
  {
    CAT_Error(CAT_InvalidISR, (SCB->ICSR & 0x1FF) | ((TIM4->SR) << 16));
  }
}
#endif

void ISR_Invalid0(void)
{
  CAT_Error(CAT_InvalidISR, 1000 + (SCB->ICSR & 0x1FF));
}

void ISR_Invalid1(void)
{
  CAT_Error(CAT_InvalidISR, 2000 + (SCB->ICSR & 0x1FF));
}

void ISR_Invalid2(void)
{
  CAT_Error(CAT_InvalidISR, 3000 + (SCB->ICSR & 0x1FF));
}

void ISR_Invalid3(void)
{
  CAT_Error(CAT_InvalidISR, 4000 + (SCB->ICSR & 0x1FF));
}

void ISR_Invalid(void)
{
  CAT_Error(CAT_InvalidISR, (SCB->ICSR & 0x1FF));
}

extern void svc_ret(void);
void svc_ret(void)
{
  // ???????????????????????????????
  //__ASM("CPSIE i"); /* enable interrupts to allow SVCall exception*/
  /*
   * FPU handling
   */
  __ASM("SVC #0");
}

#if 0
/* INTERRUPT */ void PendSV_Handler(void)
{
  //PendSV_Ctr++;
  //SchedulerPre_TaskManagement();
  __ASM("MOVS r3,#1");
  __ASM("LSLS r3,r3,#24"); /* r3:=(1 << 24), set the T bit (new xpsr) */
  __ASM("LDR r2,=SchedulerPre_TaskManagement"); /* address of the QK scheduler (new pc) */
  __ASM("LDR r1,=svc_ret"); /* return address after the call (new lr) */
  __ASM("PUSH {r1-r3}"); /* push xpsr,pc,lr */
  __ASM("SUB sp,sp,#(4*4)"); /* don't care for r12,r3,r2,r1 */
  __ASM("PUSH {r0}"); /* push the prio argument (new r0) */
  __ASM("MOVS r0,#0x6");
  __ASM("MVNS r0,r0"); /* r0:=~0x6=0xFFFFFFF9 */
  __ASM("BX r0"); /* exception-return to the scheduler */
}
#else
/* INTERRUPT */ void PendSV_Handler(void)
{
    CAT_Error(CAT_InvalidISR, (SCB->ICSR & 0x1FF));
}

#endif

/* INTERRUPT */ void SVC_Handler(void)
{
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
}
