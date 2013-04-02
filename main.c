#include <stdint.h>
#include <string.h>

#include "gpio_app.h"
#include "adc.h"
#include "adc_app.h"
#include "dac.h"
#include "timer.h"
#include "util.h"
#include "system_conf.h"
#include "timer_conf.h"
#include "timer_app.h"
#include "uart.h"
#include "scheduler_preemptive.h"
#include "vector.h"

#include "tasks.h"

#include "main.h"

//#include "stm32f0xx_rcc.h"

#define VDD 3.0 /* Volt */

GPIO_TypeDef * const gpioa = GPIOA;
GPIO_TypeDef * const gpiob = GPIOB;
GPIO_TypeDef * const gpioc = GPIOC;
RCC_TypeDef * const rcc = RCC;
SysTick_Type * const systick = SysTick;
DAC_TypeDef * const dac = DAC;
ADC_TypeDef * const adc1 = ADC1;
TIM_TypeDef * const tim3 = TIM3;
USART_TypeDef * const uart2 = USART2;
DMA_TypeDef * const dma1 = DMA1;
//DMA_Channel_TypeDef * const dma1_4 = DMA1_Channel4;
SCB_Type * const scb = SCB;
NVIC_Type * const nvic = NVIC;

uint32_t StartUpCounter;

static void PLL_Init(void)
{
#if !defined(TARGET_ECU)
#error "Error: TARGET ECU is not defined!"
#elif TARGET_ECU == TARGET_ECU_STM32F0DISCOVERY
  /* Enable HSE - high speed external input */
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
#elif TARGET_ECU == TARGET_ECU_STM32F4DISCOVERY
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
    /* Select HSI as system clock source */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    /* Disable PLL */
    RCC->CR &= ~(RCC_CR_PLLON);

    /* Configure Flash prefetch, Instruction cache, Data cache and wait state */
    FLASH->ACR = FLASH_ACR_ICEN |FLASH_ACR_DCEN |FLASH_ACR_LATENCY_5WS;

    /* HCLK = SYSCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;

    /* PCLK2 = HCLK / 2*/
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;

    /* PCLK1 = HCLK / 2*/
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;

    /* Configure the main PLL: VCO_in=(HSE / 4) -> 2MHz, VCO_out=VCO_in*48 -> 96MHz, fgen = (VCO_out/2) -> 48MHz, f_USB = (VCO_out/2) -> 48MHz */
    RCC->PLLCFGR &= (uint32_t)((uint32_t)~(RCC_PLLCFGR_PLLSRC | RCC_PLLCFGR_PLLM | RCC_PLLCFGR_PLLN |
                                                                RCC_PLLCFGR_PLLP | RCC_PLLCFGR_PLLQ));
    RCC->PLLCFGR |= (uint32_t)(RCC_PLLCFGR_PLLSRC_HSE | (RCC_PLLCFGR_PLLM_0 * 4) | (RCC_PLLCFGR_PLLN_0 * 96) | /* VCO configuration = HSE / 4 * 96 = 192 MHz */
                                                        (RCC_PLLCFGR_PLLP_0 * 1) | (RCC_PLLCFGR_PLLQ_0 * 4)); /* PLLP = 1 -> div by 4, PLLQ = 4 -> div by 4*/

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
    /* enable MCO1 - HSE - no div */
    RCC->CFGR |= (uint32_t)(RCC_CFGR_MCO1_PLL | RCC_CFGR_MCO1PRE_4);
#define GPIO_AF_MCO 0
    GPIO_PortInit_AFOut(GPIOA, 8, GPIO_AF_MCO);
  }
  else
  { /* If HSE fails to start-up, the application will have wrong clock
         configuration. User can add here some code to deal with this error */
  }
#endif
}

volatile uint32_t PendSV_Ctr;

uint32_t BASEPRI_st_task[8];
uint8_t BASEPRI_st_task_ctr = 0;

int main(void)
{
  PLL_Init();
  SysTick_Init();
  LED3_Init();
  LED3_Set(0);
  LED3_Set(1);
  LED4_Init();
  LED4_Set(0);
  LED4_Set(1);
  LED5_Init();
  LED5_Set(0);
  LED5_Set(1);
  LED6_Init();
  LED6_Set(0);
  LED6_Set(1);
  PB13_Init();
  Button1_Init();
  DAC_Init();
  DAC_1_Set((uint16_t)(1.1/VDD * 4096));
  DAC_2_Set((uint16_t)(2.2/VDD * 4096));
  ADC_HandlerInit();
  UART2_Init();
  Task_Init();
  TIM3_Init();
  TIM3_CCR1_Set(TIM3_Cnt_Get() + TIM3_FREQ); /* set the first scheduler interrupt to 1ms */

#define VECT_TAB_OFFSET ((uint32_t)(&ISR_VectorTable[0]) & 0x1FFFFF80)
  SCB->VTOR = SRAM_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal SRAM */
  PendSV_Ctr = 0;
  SCB->SHP[8] = 192;
  SCB->SHP[9] = 164;
  SCB->SHP[10] = 253;
  SCB->SHP[11] = 252;
  BASEPRI_st_task_ctr = 0;
  BASEPRI_st_task[BASEPRI_st_task_ctr & 0x07] = __get_BASEPRI();
  BASEPRI_st_task_ctr++;
  {
    uint32_t i;
    for (i = 0; i < 32; i++) NVIC->IP[i] = 250 - (2 * i);
  }

  while (1)
  {
    ADC_Handler();
    TIM3_UIF_PollHandler();
    BASEPRI_st_task[BASEPRI_st_task_ctr & 0x07] = __get_BASEPRI();
    BASEPRI_st_task_ctr++;
    SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;
  }

  return 0;
}

void ExceptionHandler(void)
{
  while(1)
    ; /* endless loop */
}

void ISR_Invalid(void)
{
  while(1)
    ; /* endless loop */
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

uint32_t ICSR_st[8];
uint32_t BASEPRI_st[8];
uint32_t BASEPRI_st2[8];
uint32_t BASEPRI_st3[8];
void PendSV_Handler(void)
{
  uint32_t ctr;
  PendSV_Ctr++;
  ctr = PendSV_Ctr;
  //ICSR_st[ctr % (sizeof(ICSR_st)/sizeof(ICSR_st[0]))] = SCB->ICSR;
  ICSR_st[ctr & 0x07] = SCB->ICSR;
  BASEPRI_st[ctr & 0x07] = __get_BASEPRI();
  if ((ctr & 0x01) == 0)
  {
    SCB->SHP[8] -= 16;
    SCB->SHP[9] -= 16;
    SCB->SHP[10] -= 16;
    SCB->SHP[11] -= 16;
    {
      uint32_t i;
      for (i = 0; i < 32; i++) NVIC->IP[i] -= 8;
    }
    BASEPRI_st2[ctr & 0x07] = __get_BASEPRI();
    SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;
    BASEPRI_st3[ctr & 0x07] = __get_BASEPRI();
    while (ctr == PendSV_Ctr)
      ;
    {
      uint32_t i;
      for (i = 0; i < 32; i++) NVIC->IP[i] += 8;
    }
    SCB->SHP[8] += 16;
    SCB->SHP[9] += 16;
    SCB->SHP[10] += 16;
    SCB->SHP[11] += 16;
  }
}
