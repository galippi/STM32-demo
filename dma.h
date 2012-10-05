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

static inline void DMA_Init(void)
{
  RCC->RCC_AHBENR |= RCC_AHBENR_DMA1EN; /* enable the DMA */
}

#endif /* _DMA_H_ */
