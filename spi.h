#ifndef _SPI_H_
#define _SPI_H_

#include "stdint.h"

#include "spi_conf.h"

#define SPI_MASTER 0
#define SPI_SLAVE  1

#define SPI_NOREMAP 0
#define SPI_REMAP   1

extern uint8_t spi1_isrCtr;

void SPI1_Init(uint8_t slave, uint8_t spiRemap);
void SPI1_Poll(void);
void SPI1_Tx(uint8_t *data, uint8_t len);

#if SPI1_ISR_ENABLE != 0
void SPI1_ISR(void);
#else
#define SPI1_ISR ISR_Invalid
#endif

extern uint8_t spi2_isrCtr;
extern uint8_t spi2_errCtr;

void SPI2_Init(void);
void SPI2_Poll(void);
void SPI2_Tx(uint8_t *data, uint8_t len);
uint8_t SPI2_Tx_u8(uint8_t data);

#if SPI2_ISR_ENABLE != 0
void SPI2_ISR(void);
#else
#define SPI2_ISR ISR_Invalid
#endif


#endif /* _SPI_H_ */
