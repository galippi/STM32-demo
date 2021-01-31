#ifndef _ESP8266_CONF_H_
#define _ESP8266_CONF_H_

#include <stdint.h>

#include "uart.h"
#include "util.h"

#define ESP8266_RX_BUFFER_SIZE 128

static inline void ESP8266_io_write(int len, const void *data)
{
    UART1_TX((const uint8_t*)data, (uint32_t)len);
}

static inline int ESP8266_io_read(int size, void *data)
{
    return (int)UART1_RX((uint8_t*)data, (uint32_t)size);
}

#define ESP8266_io_close close

#define dprintf(...) /* do nothing */

#endif /* _ESP8266_CONF_H_ */
