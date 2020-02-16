#ifndef _SPI_CONF_H_
#define _SPI_CONF_H_

#include "gpio_app.h"

#define SPI1_DMA 0

#define SPI1_CR1_INIT ( \
                        0 /* SPI_CR1_CPHA */ | \
                        0 /* SPI_CR1_CPOL */ | \
                        0 /* SPI_CR1_MSTR */ | \
                        0 /* baud rate */ | \
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

#define SPI1_CR2_INIT ( \
                        0 /* SPI_CR2_RXDMAEN */ | \
                        0 /* SPI_CR2_TXDMAEN */ | \
    0 /* SPI_CR2_SSOE */ | \
                        0 /* SPI_CR2_ERRIE */ | \
                        0 /* SPI_CR2_RXNEIE */ | \
                        0 /* SPI_CR2_TXEIE */ | \
                        0 )

//#define SPI1_SS_INIT()     (PA15_Init(),SPI1_SS_DEACTIVE())
#define SPI1_SS_ACTIVE()   //PA15_Set(0)
#define SPI1_SS_DEACTIVE() //PA15_Set(1)

#define SPI2_DMA 0
#define SPI2_ISR_ENABLE 1

#define SPI2_CR1_INIT ( \
                        0 /* SPI_CR1_CPHA */ | \
                        0 /* SPI_CR1_CPOL */ | \
                        SPI_CR1_MSTR /* SPI_CR1_MSTR */ | \
                        (6 << 3) /* baud rate */ | \
                        0 /* SPI_CR1_SPE */ | \
                        0 /* SPI_CR1_LSBFIRST */ | \
                        SPI_CR1_SSI /* SPI_CR1_SSI */ | \
                        SPI_CR1_SSM /* SPI_CR1_SSM - hardware SS */ | \
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
                        0  /* SPI_CR2_SSOE */ | \
                        SPI_CR2_ERRIE /* SPI_CR2_ERRIE */ | \
                        SPI_CR2_RXNEIE /* SPI_CR2_RXNEIE */ | \
                        SPI_CR2_TXEIE /* SPI_CR2_TXEIE */ | \
                        0 )

#define SPI2_SS_INIT()     (PB12_Init(),SPI2_SS_DEACTIVE())
//#define SPI2_SS_ACTIVE()   PB12_Set(0)
#define SPI2_SS_ACTIVE()   PB12_Set(0)
#define SPI2_SS_DEACTIVE() PB12_Set(1)

#endif /* _SPI_CONF_H_ */
