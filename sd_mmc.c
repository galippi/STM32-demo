/*
 * sd_mmc.c
 *
 *  Created on: 2020. máj. 16.
 *      Author: liptakok
 */

#include <stdio.h>
#include <string.h>

#include "spi.h"

#include "sd_mmc.h"

typedef enum
{
  e_SD_MMC_Init,
  e_SD_MMC_InitStep08,
  e_SD_MMC_InitStep1,
  e_SD_MMC_InitStep58,
  e_SD_MMC_InitStep9,
  e_SD_MMC_InitStep10,
  e_SD_MMC_Ready,
}t_SD_MMC_State;

typedef struct
{
  t_SD_MMC_State state;
  uint8_t initRequest;
  uint8_t ncr;
  uint8_t version;
  uint8_t hc;
  uint8_t ocrValid;
  uint8_t csdValid;
  uint8_t cidValid;
  uint8_t CID[16];
  uint8_t CSD[16];
  uint8_t OCR[4];
}t_SD_MMC;

static t_SD_MMC sd_mmc_data;

#define SD_MMC_CS_High() PB12_Set(1)
#define SD_MMC_CS_Low()  PB12_Set(0)

#define SD_MMC_CMD0         (0x40 |  0)
#define SD_MMC_CMD1         (0x40 |  1)
#define SD_MMC_CMD8         (0x40 |  8)
#define SD_MMC_CMD9         (0x40 |  9)
#define SD_MMC_CMD10        (0x40 | 10)
#define SD_MMC_SET_BLOCKLEN (0x40 | 16)
#define SD_MMC_CMD41        (0x40 | 41)
#define SD_MMC_CMD55        (0x40 | 55)
#define SD_MMC_CMD58        (0x40 | 58)
#define SD_MMC_CRC_ON_OFF   (0x40 | 59)

static void SD_MMC_Spi_Slow(void)
{

}

static void SD_MMC_Spi_Fast(void)
{

}

static void SD_MMC_TxDummy(uint8_t dummyCtr)
{
  for(uint32_t i = 0; i < dummyCtr; i++)
    SPI2_Tx_u8(0xFF);
}

static uint8_t SD_MMC_Command(uint8_t cmd, uint32_t data, uint8_t crc, void *responseData, uint32_t responseLength)
{
  SPI2_Tx_u8(0xFF);
  SD_MMC_CS_Low();
  SPI2_Tx_u8(cmd);
  for(uint32_t i = 0; i < sizeof(data); i++)
    SPI2_Tx_u8((data >> (8 * i)) & 0xFF);
  SPI2_Tx_u8(crc);
  uint8_t response;
  for(uint32_t i = 0; i < 16; i++)
  {
    response = SPI2_Tx_u8(0xFF);
    if (response != 0xFF)
      break;
  }
  if ((response != 0xFF) && (responseData != NULL))
  {
    uint8_t *ptr = responseData;
    for(uint32_t i = 0; i < responseLength; i++)
    {
      *ptr = SPI2_Tx_u8(0xFF);
      ptr++;
    }
  }
  if (responseLength != 0xFFFFFFFF)
  {
    SD_MMC_CS_High();
    SPI2_Tx_u8(0xFF);
  }
  return response;
}

static uint8_t SD_MMC_ACmd(uint8_t cmd, uint32_t data, uint8_t crc, void *responseData, uint32_t responseLength)
{
  uint8_t response = SD_MMC_Command(SD_MMC_CMD55, 0x00000000, 0xFF, NULL, 0);
  if (response != 0x05)
    return response;
  return SD_MMC_Command(cmd, data, crc, responseData, responseLength);
}

void SD_MMC_init(void)
{
  sd_mmc_data.initRequest = 1;
  SPI2_Init();
}

void SD_MMC_BgdTask(void)
{
  if (sd_mmc_data.initRequest)
  {
    SD_MMC_CS_High();
    sd_mmc_data.state = e_SD_MMC_Init;
    sd_mmc_data.initRequest = 0;
    sd_mmc_data.ocrValid = 0;
    sd_mmc_data.hc = 0;
  }
  switch(sd_mmc_data.state)
  {
    case e_SD_MMC_Init:
      SD_MMC_Spi_Slow();
      SD_MMC_TxDummy(10);
      if (SD_MMC_Command(SD_MMC_CMD0, 0x00000000, 0x95, NULL, 0) != 0x01)
        sd_mmc_data.initRequest = 1;
      else
        sd_mmc_data.state = e_SD_MMC_InitStep08;
      break;
    case e_SD_MMC_InitStep08:
    {
      uint8_t if_cond[4];
      if (SD_MMC_Command(SD_MMC_CMD8, 0x000001AA, 0x43, if_cond, sizeof(if_cond)) != 0x01)
      {
        sd_mmc_data.version = 1;
        sd_mmc_data.state = e_SD_MMC_InitStep1;
      }else
      {
        if ((if_cond[3] == 0xaa) && (if_cond[2] ==0x01)) /* echo pattern and supply voltage is correct */
        {
          if (SD_MMC_Command(SD_MMC_CMD1, 0x00000000, 0xF9, NULL, 0) != 0x01)
          {
            sd_mmc_data.initRequest = 1;
          }else
          {
            sd_mmc_data.state = e_SD_MMC_InitStep9;
          }
        }
      }
      break;
    }
    case e_SD_MMC_InitStep1:
    {
      if (SD_MMC_Command(SD_MMC_CMD1, 0x00000000, 0xF9, NULL, 0) != 0x01)
      {
        sd_mmc_data.state = e_SD_MMC_InitStep1;
      }else
      {
        sd_mmc_data.state = e_SD_MMC_InitStep58;
      }
      break;
    }
    case e_SD_MMC_InitStep58:
    {
      if (SD_MMC_Command(SD_MMC_CMD58, 0x00000000, 0x25, sd_mmc_data.OCR, sizeof(sd_mmc_data.OCR)) != 0x01)
      {
        sd_mmc_data.ocrValid = 0;
        sd_mmc_data.initRequest = 1;
      }else
      {
        sd_mmc_data.ocrValid = 1;
        if (sd_mmc_data.OCR[0] & 0x80)
        { /* card not in power-up state, ccs bit invalid */
          sd_mmc_data.initRequest = 1;
        }else
        {
          if (sd_mmc_data.OCR[0] & 0x40)
          {
            sd_mmc_data.hc = 1;
          }else
          {
            sd_mmc_data.hc = 0;
          }
        }
        sd_mmc_data.state = e_SD_MMC_InitStep9;
      }
      break;
    }
    case e_SD_MMC_InitStep9:
    {
      uint8_t buffer[48];
      SD_MMC_ACmd(SD_MMC_CMD41, 0x00000000, 0xF9, NULL, 0);
      if (SD_MMC_Command(SD_MMC_CMD9, 0x00000000, 0xAF, buffer, sizeof(buffer)) != 0x01)
      {
        sd_mmc_data.csdValid = 0;
      }else
      {
        memcpy(sd_mmc_data.CSD, &buffer[30], 16);
        sd_mmc_data.csdValid = 1;
      }
      sd_mmc_data.state = e_SD_MMC_InitStep10;
      break;
    }
    case e_SD_MMC_InitStep10:
    {
      uint8_t buffer[48];
      if (SD_MMC_Command(SD_MMC_CMD10, 0x00000000, 0xFF, buffer, sizeof(buffer)) != 0x01)
      {
        sd_mmc_data.cidValid = 0;
      }else
      {
        memcpy(sd_mmc_data.CID, &buffer[30], 16);
        sd_mmc_data.cidValid = 1;
      }
      SD_MMC_Command(SD_MMC_CRC_ON_OFF, 0x00000000, 0x25, NULL, 0); /* turn CRC off */
      SD_MMC_Command(SD_MMC_SET_BLOCKLEN, 512, 0xFF, NULL, 0);
      sd_mmc_data.state = e_SD_MMC_Ready;
      SD_MMC_Spi_Fast();
      break;
    }
    case e_SD_MMC_Ready:
    {
      break;
    }
    default:
      sd_mmc_data.initRequest = 1;
      break;
  }
}
