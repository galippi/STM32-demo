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

#include "dma_conf.h"

#include "controller.h"
#include STM32_RCC_HEADER

static inline void DMA_Init(DMA_TypeDef *dma)
{
  if (dma == DMA1)
  {
#if (CPU_TYPE == CPU_TYPE_STM32F0) || (CPU_TYPE == CPU_TYPE_STM32F1)
    RCC->AHBENR |= RCC_AHBENR_DMA1EN; /* enable the DMA */
#elif CPU_TYPE == CPU_TYPE_STM32F4
  RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
#else
#error Not implemented processor!
#endif
#if (defined(RCC_AHBENR_DMA2EN)) || (CPU_TYPE == CPU_TYPE_STM32F4)
  }else
  if (dma == DMA2)
  {
#if defined(RCC_AHBENR_DMA2EN)
    RCC->AHBENR |= RCC_AHBENR_DMA2EN; /* enable the DMA */
#elif CPU_TYPE == CPU_TYPE_STM32F4
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN; /* enable the DMA */
#endif
#endif
  }else
  { /* not valid DMA -> fatal error */
  }
}

#endif /* _DMA_H_ */
