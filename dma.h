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
//#include STM32_RCC_HEADER

#define DMA_CTR1_SDW_LOG2_U8  0
#define DMA_CTR1_SDW_LOG2_U16 DMA_CTR1_SDW_LOG2_0
#define DMA_CTR1_SDW_LOG2_U32 DMA_CTR1_SDW_LOG2_1

#define DMA_CTR1_DDW_LOG2_U8  0
#define DMA_CTR1_DDW_LOG2_U16 DMA_CTR1_DDW_LOG2_0
#define DMA_CTR1_DDW_LOG2_U32 DMA_CTR1_DDW_LOG2_1

#if 0
enum { // see it in rm0522 - section 14.3.4
    e_DMA_REQSEL_ADC1,
    e_DMA_REQSEL_USART2_RX = 14,
    e_DMA_REQSEL_USART2_RX = 15,
}e_DMA_REQSEL;
#endif

#if CPU_TYPE == CPU_TYPE_STM32C542
#define LPDMA1_CH_LAST LPDMA1_CH3
#define LPDMA2_CH_LAST LPDMA2_CH3
#else
#error Not yet implemented!
#endif

static inline void DMA_Init(DMA_Channel_TypeDef *dmaCh)
{
    if ((dmaCh >= LPDMA1_CH0) && (dmaCh <= LPDMA1_CH_LAST))
    {
        RCC->AHB1ENR |= RCC_AHB1ENR_LPDMA1EN;
    }else
    if ((dmaCh >= LPDMA2_CH0) && (dmaCh <= LPDMA2_CH_LAST))
    {
        RCC->AHB1ENR |= RCC_AHB1ENR_LPDMA2EN;
    }else
    { /* not valid DMA -> fatal error */
    }
}

#endif /* _DMA_H_ */
