#ifndef _SPI_H_
#define _SPI_H_

#include "stdint.h"

#define SPI_MASTER 0
#define SPI_SLAVE  1

#define SPI_NOREMAP 0
#define SPI_REMAP   1

void SPI1_Init(uint8_t slave, uint8_t spiRemap);
void SPI1_Poll(void);
void SPI1_Tx(uint8_t *data, uint8_t len);

void SPI2_Init(void);
void SPI2_Poll(void);
void SPI2_Tx(uint8_t *data, uint8_t len);

#endif /* _SPI_H_ */
