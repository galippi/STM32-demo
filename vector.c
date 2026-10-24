/******************************************************************************
 *
 * \file
 * \ingroup RESET
 * \brief   Interrupt vector table
 *
 *
 * \details Fill me
 *
 ******************************************************************************/
/** \addtogroup RESET
 *  @{
 ******************************************************************************/
#include <stdint.h>

#include "reset.h"
#include "main.h"
#include "uart.h"
#include "util.h"
#include "SysClock_conf.h"

#ifdef f_USBCLK_Hz
#include "usbd_conf.h"
#else
#define USB_LP_IRQHandler ISR_Invalid
#endif

#include "vector.h"

t_func_ptr const ISR_VectorTable[] =
{
  (const t_func_ptr) &SP_INIT, /* stack top address */
  reset,             /* 1 Reset */
  NMI_Handler,       /* 2 NMI */
  ExceptionHandler_1,/* 3 HardFault */
  ExceptionHandler_2,/* 4 MemManage */
  ExceptionHandler_3,/* 5 BusFault */
  ExceptionHandler_4,/* 6 UsageFault */
  ISR_Invalid,       /* 7 RESERVED */
  ISR_Invalid,       /* 8 RESERVED */
  ISR_Invalid,       /* 9 RESERVED*/
  ISR_Invalid,       /* 10 RESERVED */
  SVC_Handler,       /* 11 SVCall */
  ISR_Invalid,       /* 12 Debug Monitor */
  ISR_Invalid,       /* 13 RESERVED */
  PendSV_Handler,    /* 14 PendSV */
  SysTick_Isr,       /* 15 SysTick */
  ISR_Invalid,       /* 16 WWDG */
  ISR_Invalid,       /* 17 PVD / PVM */
  ISR_Invalid,       /* 18 RTC */
  ISR_Invalid,       /* 19 TAMP */
  ISR_Invalid,       /* 20 RAMCFG */
  ISR_Invalid,       /* 19 FLASH */
  ISR_Invalid,       /* 20 RCC */
  ISR_Invalid,       /* 21 EXTI0 */
  ISR_Invalid,       /* 21 EXTI1 */
  ISR_Invalid,       /* 22 EXTI2 */
  ISR_Invalid,       /* 22 EXTI3 */
  ISR_Invalid,       /* 23 EXTI4 */
  ISR_Invalid,       /* 23 EXTI5 */
  ISR_Invalid,       /* 23 EXTI6 */
  ISR_Invalid,       /* 23 EXTI7 */
  ISR_Invalid,       /* 23 EXTI8 */
  ISR_Invalid,       /* 23 EXTI9 */
  ISR_Invalid,       /* 23 EXTI10 */
  ISR_Invalid,       /* 23 EXTI11 */
  ISR_Invalid,       /* 23 EXTI12 */
  ISR_Invalid,       /* 23 EXTI3 */
  ISR_Invalid,       /* 23 EXTI14 */
  ISR_Invalid,       /* 23 EXTI15 */
  ISR_Invalid,       /* 25 DMA1_Channel0 */
  ISR_Invalid,       /* 25 DMA1_Channel1 */
  ISR_Invalid,       /* 26 DMA1_Channel2 */
  ISR_Invalid,       /* 26 DMA1_Channel3 */
  ISR_Invalid,       /* 27 DMA1_Channel4 */
  ISR_Invalid,       /* 27 DMA1_Channel5 */
  ISR_Invalid,       /* 27 DMA1_Channel6 */
  ISR_Invalid,       /* 27 DMA1_Channel7 */
  ISR_Invalid,       /* xx IWDG */
  ISR_Invalid,       /* 28 ADC1 */
  ISR_Invalid,       /* 28 ADC2 */
  ISR_Invalid,       /* 37 FDCAN1_IT0 */
  ISR_Invalid,       /* 38 FDCAN1_IT1 */
  ISR_Invalid,       /* 29 TIM1_BRK_TERR_IERR */
  ISR_Invalid,       /* 30 TIM1_UP */
  ISR_Invalid,       /* 29 TIM1_TRG_COM_DIR_IDX */
  ISR_Invalid,       /* 30 TIM1_CC */
  ISR_Invalid,       /* 31 TIM2 */
  ISR_Invalid,       /* 32 TIM5 */
  ISR_Invalid,       /* 33 TIM6 */
  ISR_Invalid,       /* 34 TIM7 */
  ISR_Invalid,       /* 39 I2C1_EV */
  ISR_Invalid,       /* 39 I2C1_ER */
  ISR_Invalid,       /* 40 I3C1_EV */
  ISR_Invalid,       /* 40 I3C1_ER */
  ISR_Invalid,       /* 41 SPI1 */
  ISR_Invalid,       /* 42 SPI2 */
  ISR_Invalid,       /* 42 SPI3 */
  ISR_Invalid,       /* 43 USART1 */
  Usart2Isr,         /* 44 USART2 */
  ISR_Invalid,       /* 45 USART3 */
  ISR_Invalid,       /* 45 USART4 */
  ISR_Invalid,       /* 45 USART5 */
  ISR_Invalid,       /* 43 LPUART1 */
  ISR_Invalid,       /* 36 LP_TIM1 */
  ISR_Invalid,       /* 35 TIM12 */
  ISR_Invalid,       /* 36 TIM15 */
  ISR_Invalid,       /* 37 TIM16 */
  ISR_Invalid,       /* 38 TIM17 */
  USB_LP_IRQHandler, /* 24 UCPD1 / UCPD2 / USB */
  ISR_Invalid,       /* 39 CRS */
  ISR_Invalid,       /* 40 RNG */
  ISR_Invalid,       /* 46 FPU */
  ISR_Invalid,       /* 46 ICACHE */
  ISR_Invalid,       /* 46 CORDIC */
  ISR_Invalid,       /* 47 AES */
  ISR_Invalid,       /* 46 HASH */
  ISR_Invalid,       /* 46 I2C2_EV */
  ISR_Invalid,       /* 46 I2C2_ER */
  ISR_Invalid,       /* 46 TIM8_BRK_TERR_IERR */
  ISR_Invalid,       /* 46 TIM8_UP */
  ISR_Invalid,       /* 46 TIM8_TRG_COM_DIR_IDX */
  ISR_Invalid,       /* 46 TIM8_CC */
  ISR_Invalid,       /* 46 COMP1 */
  ISR_Invalid,       /* 46 DAC1 */
  ISR_Invalid,       /* 25 DMA2_Channel0 */
  ISR_Invalid,       /* 25 DMA2_Channel1 */
  ISR_Invalid,       /* 26 DMA2_Channel2 */
  ISR_Invalid,       /* 26 DMA2_Channel3 */
  ISR_Invalid,       /* 27 DMA2_Channel4 */
  ISR_Invalid,       /* 27 DMA2_Channel5 */
  ISR_Invalid,       /* 27 DMA2_Channel6 */
  ISR_Invalid,       /* 27 DMA2_Channel7 */
  ISR_Invalid,       /* 37 FDCAN2_IT0 */
  ISR_Invalid,       /* 38 FDCAN2_IT1 */
  ISR_Invalid,       /* 46 COMP2 */
  ISR_Invalid,       /* 46 TIM3 */
  ISR_Invalid,       /* 46 TIM4 */
  ISR_Invalid,       /* 46 XSPI1 */
  ISR_Invalid,       /* 46 SAES */
  ISR_Invalid,       /* 46 PKA */
  ISR_Invalid,       /* 46 ETH1 */
  ISR_Invalid,       /* 46 ETH1_WKUP */
  ISR_Invalid,       /* 46 USART6 */
  ISR_Invalid,       /* 46 USART7 */
  ISR_Invalid,       /* 46 ADC3 */
};

COMP_CHECK_ARRAY(ISR_VectorTable, 1 + 15 + 99)
