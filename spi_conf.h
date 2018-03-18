#ifndef _SPI_CONF_H_
#define _SPI_CONF_H_

//#include "spi.h"

#define SPI2_CR1_INIT ( \
                        0 /* SPI_CR1_CPHA */ | \
                        0 /* SPI_CR1_CPOL */ | \
                        SPI_CR1_MSTR /* SPI_CR1_MSTR */ | \
                        (7 << 3) /* baud rate */ | \
                        0 /* SPI_CR1_SPE */ | \
                        0 /* SPI_CR1_LSBFIRST */ | \
                        0 /* SPI_CR1_SSI */ | \
                        0 /* SPI_CR1_SSM - hardware SS */ | \
                        0 /* SPI_CR1_RXONLY */ | \
                        0 /* SPI_CR1_DFF - 8-bit */ | \
                        0 /* SPI_CR1_CRCNEXT */ | \
                        0 /* SPI_CR1_CRCEN */ | \
                        0 /* SPI_CR1_BIDIOE */ | \
                        0 /* SPI_CR1_BIDIMODE - 2-line mode */ | \
                        0 )

#define SPI2_CR2_INIT ( \
                        0 /* SPI_CR2_RXDMAEN */ | \
                        0 /* SPI_CR2_TXDMAEN */ | \
                        SPI_CR2_SSOE /* SPI_CR2_SSOE */ | \
                        0 /* SPI_CR2_ERRIE */ | \
                        0 /* SPI_CR2_RXNEIE */ | \
                        0 /* SPI_CR2_TXEIE */ | \
                        0 )

#endif /* _SPI_CONF_H_ */
