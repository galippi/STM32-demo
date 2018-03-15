/******************************************************************************
 *
 * \file
 * \ingroup DMA
 * \brief   Template sourcefile of Template Component.
 *
 * \details Fill me
 *
 ******************************************************************************/
/** \addtogroup DMA
 *  @{
 ******************************************************************************/

#ifndef _DMA_H_
#define _DMA_H_

#include "controller.h"
#include STM32_RCC_HEADER

static inline void DMA_Init(DMA_TypeDef *dma)
{
#if (CPU_TYPE == CPU_TYPE_STM32F0) || (CPU_TYPE == CPU_TYPE_STM32F1)
  if (dma == DMA1)
  {
    RCC->AHBENR |= RCC_AHBENR_DMA1EN; /* enable the DMA */
  }else
  {
    //RCC->AHBENR |= RCC_AHBENR_DMA2EN; /* enable the DMA */
  }
#elif CPU_TYPE == CPU_TYPE_STM32F4
  RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
#else
#error Not implemented processor!
#endif
}

#endif /* _DMA_H_ */
