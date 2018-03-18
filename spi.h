#ifndef _SPI_H_
#define _SPI_H_

#include "stdint.h"

void SPI_Init(void);
void SPI_Poll(void);
void SPI2_Tx(uint8_t *data, uint8_t len);

#endif /* _SPI_H_ */
