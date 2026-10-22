#ifndef _DMA_CONF_H_
#define _DMA_CONF_H_

#define DMA1_ChIdx_ADC1   0
#define DMA1_Channel_ADC1 LPDMA1_CH0
#define DMA1_Channel0_Isr DmaAdc1_Isr

#define DMA1_ChIdx_USART2_TX   1
#define DMA1_Channel_USART2_TX LPDMA1_CH1

#define DMA1_ChIdx_USART2_RX   2
#define DMA1_Channel_USART2_RX LPDMA1_CH2

#if 0
#define DMA_ADC1      DMA1_Channel1

#define DMA_SPI1_RX   DMA1_Channel2
//#define DMA_USART3_TX DMA1_Channel2

#define DMA_SPI1_TX   DMA1_Channel3
//#define DMA_USART3_RX DMA1_Channel3

#define DMA_USART1_TX DMA1_Channel4
//#define DMA_SPI2_RX   DMA1_Channel4

#define DMA_USART1_RX DMA1_Channel5
//#define DMA_SPI2_TX   DMA1_Channel5

#define DMA_USART2_RX DMA1_Channel6

#define DMA_USART2_TX DMA1_Channel7
#endif

#endif /* _DMA_CONF_H_ */
