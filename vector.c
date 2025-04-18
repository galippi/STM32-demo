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
//#include "uart.h"
#include "util.h"

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
  ISR_Invalid,       /* 16 WWDG */
  ISR_Invalid,       /* 17 PVD / PVM */
  ISR_Invalid,       /* 18 RTC / TAMP */
  ISR_Invalid,       /* 19 FLASH */
  ISR_Invalid,       /* 20 RCC / CRS */
  ISR_Invalid,       /* 21 EXTI0_1 */
  ISR_Invalid,       /* 22 EXTI2_3 */
  ISR_Invalid,       /* 23 EXTI4_15 */
  USB_LP_IRQHandler, /* 24 UCPD1 / UCPD2 / USB */
  ISR_Invalid,       /* 25 DMA1_Channel1 */
  ISR_Invalid,       /* 26 DMA1_Channel2_3 */
  ISR_Invalid,       /* 27 DMA1_Channel4_5_6_7 / DMA2_Channel1_2_3_4_5 */
  ISR_Invalid,       /* 28 ADC / COMP */
  ISR_Invalid,       /* 29 TIM1_BRK_UP_TRG_COM */
  ISR_Invalid,       /* 30 TIM1_CC */
  ISR_Invalid,       /* 31 TIM2 */
  ISR_Invalid,       /* 32 TIM3_4 */
  ISR_Invalid,       /* 33 TIM6 / DAC / LPTIM1 */
  ISR_Invalid,       /* 34 TIM7 / LPTIM2 */
  TIM14_ISR,         /* 35 TIM14 */
  ISR_Invalid,       /* 36 TIM15 */
  ISR_Invalid,       /* 37 TIM16 / FDCAN_IT0 */
  ISR_Invalid,       /* 38 TIM17 / FDCAN_IT1 */
  ISR_Invalid,       /* 39 I2C1 */
  ISR_Invalid,       /* 40 I2C2_3 */
  ISR_Invalid,       /* 41 SPI1 */
  ISR_Invalid,       /* 42 SPI2_3 */
  ISR_Invalid,       /* 43 USART1 */
  ISR_Invalid,       /* 44 USART2 / LPUART2 */
  ISR_Invalid,       /* 45 USART3_4_5_6 / LPUART1 */
  ISR_Invalid,       /* 46 CEC */
  ISR_Invalid,       /* 47 AES / RNG */
};

COMP_CHECK_ARRAY(ISR_VectorTable, 48)
