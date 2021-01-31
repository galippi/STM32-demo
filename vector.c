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

#include "vector.h"

t_func_ptr const ISR_VectorTable[] =
{
  (const t_func_ptr) &SP_INIT, /* stack top address */
  reset,             /* 1 Reset */
  ExceptionHandler_0,/* 2 NMI */
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
  ISR_Invalid,       /* 15 SysTick */
  ISR_Invalid,       /* 16 */
  ISR_Invalid,       /* 17 */
  ISR_Invalid,       /* 18 */
  ISR_Invalid,       /* 19 */
  ISR_Invalid,       /* 20 */
  ISR_Invalid,       /* 21 */
  ISR_Invalid,       /* 22 External Interrupt(0) */
  ISR_Invalid,       /* 23 External Interrupt(1) */
  ISR_Invalid,       /* 24 External Interrupt(2) */
  ISR_Invalid,       /* 25 External Interrupt(3) */
  ISR_Invalid,       /* 26 External Interrupt(4) */
  ISR_Invalid,       /*  DMA1 Ch1 */
  ISR_Invalid,       /*  DMA1 Ch2 */
  ISR_Invalid,       /*  DMA1 Ch3 */
  ISR_Invalid,       /*  DMA1 Ch4 */
  UART1_RxDma_ISR,   /*  DMA1 Ch5 */
  ISR_Invalid,       /*  DMA1 Ch6 */
  ISR_Invalid,       /*  DMA1 Ch7 */
  ISR_Invalid,       /*  ADC */
  ISR_Invalid,       /*  CAN1 Tx */
  ISR_Invalid,       /*  CAN1 Rx0 */
  ISR_Invalid,       /*  CAN1 Rx1 */
  ISR_Invalid,       /*  CAN1 SCE   */
  ISR_Invalid,       /*  EXTI9_5 */
  ISR_Invalid,       /*  TIM1 */
  ISR_Invalid,       /*  TIM1 */
  ISR_Invalid,       /*  TIM1 */
  ISR_Invalid,       /*  TIM1 */
  TIM2_ISR,          /*  TIM2 */
  TIM3_ISR,          /*  TIM3 */
  ISR_Invalid,       /*  TIM4 */
  ISR_Invalid,       /*  I2C1 */
  ISR_Invalid,       /*  I2C1 */
  ISR_Invalid,       /*  I2C2 */
  ISR_Invalid,       /*  I2C2 */
  ISR_Invalid,       /*  SPI1 */
  ISR_Invalid,       /*  SPI2 */
  ISR_Invalid,       /*  USART1 */
  ISR_Invalid,       /*  USART2 */
  ISR_Invalid,       /*  USART3 */
  ISR_Invalid,       /*  EXTI15_10 */
  ISR_Invalid,       /*  RTC_Alarm */
  ISR_Invalid,       /*  OTG_FS_WKUP */
  ISR_Invalid,       /*  TIM8 */
  ISR_Invalid,       /*  TIM8 */
  ISR_Invalid,       /*  TIM8 */
  ISR_Invalid,       /*  TIM8 */
  ISR_Invalid,       /*  DMA1 */
  ISR_Invalid,       /*  FSMC */
  ISR_Invalid,       /*  SDIO */
  ISR_Invalid,       /*  TIM5 */
  ISR_Invalid,       /*  SPI3 */
  ISR_Invalid,       /*  UART4 */
  ISR_Invalid,       /*  UART5 */
  ISR_Invalid,       /*  TIM6 */
  ISR_Invalid,       /*  TIM7 */
  ISR_Invalid,       /*  DMA2 */
  ISR_Invalid,       /*  DMA2 */
  ISR_Invalid,       /*  DMA2 */
  ISR_Invalid,       /*  DMA2 */
  ISR_Invalid,       /*  DMA2 */
  ISR_Invalid,       /*  ETH */
  ISR_Invalid,       /*  ETH */
  ISR_Invalid,       /*  CAN2 */
  ISR_Invalid,       /*  CAN2 */
  ISR_Invalid,       /*  CAN2 */
  ISR_Invalid,       /*  CAN2 */
  ISR_Invalid,       /*  OTG_FS*/
  ISR_Invalid,       /*  DMA2 */
  ISR_Invalid,       /*  DMA2 */
  ISR_Invalid,       /*  DMA2 */
  ISR_Invalid,       /*  USART6 */
  ISR_Invalid,       /*  I2C3 */
  ISR_Invalid,       /*  I2C3 */
  ISR_Invalid,       /*  OTG_HS */
  ISR_Invalid,       /*  OTG_HS */
  ISR_Invalid,       /*  OTG_HS */
  ISR_Invalid,       /*  OTG_HS */
  ISR_Invalid,       /*  DCMI */
  ISR_Invalid,       /*  CRYP */
  ISR_Invalid,       /*  HASH_RNG */
  ISR_Invalid,       /*  FPU */
  ISR_Invalid,       /*  */
};
