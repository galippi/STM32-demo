#include <stdint.h>
#include <string.h>

#include "gpio_app.h"
#include "adc.h"
#include "adc_app.h"
#include "timer.h"
#include "util.h"
#include "system_conf.h"
#include "timer_conf.h"
#include "timer_app.h"
#include "uart.h"
#include "scheduler_preemptive.h"
#include "tasks.h"
#include "FaultHandler.h"

#include "main.h"

#define VDD 3.0 /* Volt */

GPIO_TypeDef * const gpioa = GPIOA;
GPIO_TypeDef * const gpiob = GPIOB;
GPIO_TypeDef * const gpioc = GPIOC;
RCC_TypeDef * const rcc = RCC;
SysTick_Type * const systick = SysTick;
ADC_TypeDef * const adc1 = ADC1;
TIM_TypeDef * const tim3 = TIM3;
USART_TypeDef * const uart2 = USART2;
DMA_TypeDef * const dma1 = DMA1;
//DMA_Channel_TypeDef * const dma1_4 = DMA1_Channel4;

uint32_t StartUpCounter;

static void PLL_Init(void)
{
#if !defined(TARGET_ECU)
#error "Error: TARGET ECU is not defined!"
#elif TARGET_ECU == TARGET_ECU_STM32F0DISCOVERY
  /* Enable HSE - high speed external oscillator */
  RCC->CR |= (uint32_t)(RCC_CR_HSEON | RCC_CR_HSEBYP);

  StartUpCounter = 0;
  /* Wait till HSE is ready and if Time out is reached exit */
  while(((RCC->CR & RCC_CR_HSERDY) == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT))
  {
    StartUpCounter++;
  }
  if ((RCC->CR & RCC_CR_HSERDY))
  {
    /* Enable Prefetch Buffer and set Flash Latency */
    FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY;

    /* HCLK = SYSCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;

    /* PCLK = HCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE_DIV1;


    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL));
    RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_PREDIV1 | RCC_CFGR_PLLXTPRE_PREDIV1 | RCC_CFGR_PLLMULL6); /* PLL configuration = HSE * 6 = 48 MHz */
    //RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_PREDIV1 | RCC_CFGR_PLLXTPRE_PREDIV1_Div2 | RCC_CFGR_PLLMULL2); /* PLL configuration = HSE * 2 = 16 MHz */

    /* Enable PLL */
    RCC->CR |= RCC_CR_PLLON;

    /* Wait till PLL is ready */
    while((RCC->CR & RCC_CR_PLLRDY) == 0)
    {
    }

    /* Select PLL as system clock source */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;

    /* Wait till PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)RCC_CFGR_SWS_PLL)
    {
    }
  }
  else
  { /* If HSE fails to start-up, the application will have wrong clock
         configuration. User can add here some code to deal with this error */
  }
#elif TARGET_ECU == TARGET_ECU_STM32F103C8_ARDUINO
  /* Enable HSE - high speed external oscillator */
  RCC->CR |= (uint32_t)(RCC_CR_HSEON);

  StartUpCounter = 0;
  /* Wait till HSE is ready and if Time out is reached exit */
  while(((RCC->CR & RCC_CR_HSERDY) == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT))
  {
    StartUpCounter++;
  }
  if ((RCC->CR & RCC_CR_HSERDY))
  {
    /* Enable Prefetch Buffer and set Flash Latency */
    FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY;

    /* HCLK = SYSCLK = 48MHz*/
    RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;

    /* PCLK = HCLK / 2 = 24 MHz */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2;


    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL));
    RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL12); /* PLL configuration = HSE / 2 * 12 = 48 MHz */

    /* Enable PLL */
    RCC->CR |= RCC_CR_PLLON;

    /* Wait till PLL is ready */
    while((RCC->CR & RCC_CR_PLLRDY) == 0)
    {
    }

    /* Select PLL as system clock source */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;

    /* Wait till PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)RCC_CFGR_SWS_PLL)
    {
    }
  }
  else
  { /* If HSE fails to start-up, the application will have wrong clock
         configuration. User can add here some code to deal with this error */
  }
#else
#error CPU type is not yet implemented!
#endif
}

int main(void)
{
  PLL_Init();
  SysTick_Init();
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
  Button1_Init();
  ADC_HandlerInit();
  UART2_Init();
  Task_Init();
  SchedulerPre_Init();
  TIM3_Init();
  TIM3_CCR1_Set(TIM3_Cnt_Get() + TIM3_FREQ); /* set the first scheduler interrupt to 1ms */
  NVIC->ISER[29/32] = NVIC->ISER[29/32] | (1 << (29%32)); /* enable TIM3 interrupt */
  NVIC->IP[29] = 128; /* set TIM3 interrupt priority to medium */

  while (1)
  {
    ADC_Handler();
    //TIM3_UIF_PollHandler();
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
/* INTERRUPT */ void TIM2_ISR(void)
{
  if (TIM2_SR_CC3IF_Get())
  {
    TIM2_SR_CC3IF_Reset();
    TIM2_CC3IF_Callback();
  }else
  {
    CAT_Error(CAT_InvalidISR, (SCB->ICSR & 0x1FF));
  }
}

/* INTERRUPT */ void TIM3_ISR(void)
{
  if (TIM3_SR_UIF_Get())
  {
    TIM3_SR_UIF_Reset();
    TIM3_UIF_Callback();
  }else
  {
    CAT_Error(CAT_InvalidISR, (SCB->ICSR & 0x1FF));
  }
}

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

#if       (__CORTEX_M >= 0x03)
//#error _CORTEX_M >= 3
static __INLINE uint32_t  __get_BASEPRI2(void)
{
  register uint32_t __regBasePri         __ASM("basepri");
  return(__regBasePri);
}
#else
#error _CORTEX_M < 3
#endif

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
