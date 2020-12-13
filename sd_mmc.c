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
  uint32_t size;
  uint32_t size2;
  t_SD_MMC_State state;
  uint8_t initRequest;
  uint8_t ncr;
  uint8_t version;
  uint8_t hc;
  uint8_t ocrValid;
  uint8_t csdValid;
  uint8_t cidValid;
  uint8_t CID[16];
  uint8_t CSD[17];
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
  //for(int32_t i = 0; i < 32; i+=8)
  for(int32_t i = 24; i >= 0; i-=8)
    SPI2_Tx_u8((data >> i) & 0xFF);
  SPI2_Tx_u8((crc << 1) | 0x01);
  uint8_t response;
  for(uint32_t i = 0; i < 64; i++)
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
  uint8_t response = SD_MMC_Command(SD_MMC_CMD55, 0x00000000, 0x32, NULL, 0);
  if (response != 0x01)
    return 0xFF;
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
      //if (SD_MMC_Command(SD_MMC_CMD0, 0x00000000, 0x95, NULL, 0) != 0x01)
      if (SD_MMC_Command(SD_MMC_CMD0, 0x00000000, 0x4A, NULL, 0) != 0x01)
        sd_mmc_data.initRequest = 1;
      else
      {
          {
              uint8_t if_cond[4];
              if (SD_MMC_Command(SD_MMC_CMD8, 0x000001AA, 0x43, if_cond, sizeof(if_cond)) != 0x01)
              { // MMC
                  uint8_t resetOk = 0;
                  for (uint8_t i = 0; i < 160; i++)
                  {
                      uint8_t response;
                      if ((response = SD_MMC_ACmd(SD_MMC_CMD41, 0x00000000, 0xF9, NULL, 0)) != 0xFF)
                      { // SDC
                          if (response == 0)
                          { // SDC ver.1
                              resetOk = 1,
                              sd_mmc_data.version = 2;
                              break;
                          }else
                          {
                          }
                      }
                  }
                  if (resetOk == 0)
                  {
                      for (uint8_t i = 0; i < 160; i++)
                      {
                          uint8_t response;
                          if ((response = SD_MMC_Command(SD_MMC_CMD1, 0x00000000, 0xFF, NULL, 0)) == 0x00)
                          { // MMC ver.3
                              resetOk = 1,
                              sd_mmc_data.version = 1;
                              if (SD_MMC_Command(SD_MMC_CMD58, 0x00000000, 0x25, sd_mmc_data.OCR, sizeof(sd_mmc_data.OCR)) != 0x00)
                              {
                                sd_mmc_data.initRequest = 1;
                                resetOk = 0;
                              }else
                              {
                                sd_mmc_data.ocrValid = 1;
                              }
                              break;
                          }
                      }
                  }
                  if (resetOk == 0)
                  {
                      sd_mmc_data.initRequest = 1;
                  }
              }else
              {
                if ((if_cond[3] == 0xaa) && (if_cond[2] ==0x01)) /* echo pattern and supply voltage is correct */
                {
                    uint8_t resetOk = 0;
                    for (uint32_t i = 0; i < 16000; i++)
                    {
                        uint8_t response;
                        if ((response = SD_MMC_ACmd(SD_MMC_CMD41, 0x40000000, 0xF9, NULL, 0)) != 0xFF)
                        {
                            if (response == 0)
                            { // SDC ver.2
                                resetOk = 1;
                                break;
                            }
                        }
                    }
                    if (resetOk)
                    {
                        if (SD_MMC_Command(SD_MMC_CMD58, 0x00000000, 0x25, sd_mmc_data.OCR, sizeof(sd_mmc_data.OCR)) != 0x00)
                        {
                          sd_mmc_data.initRequest = 1;
                          resetOk = 0;
                        }else
                        {
                          sd_mmc_data.ocrValid = 1;
                          if ((sd_mmc_data.OCR[0] & 0x80) == 0)
                          { /* card not in power-up state, ccs bit invalid */
                            sd_mmc_data.initRequest = 1;
                          }else
                          {
                            if (sd_mmc_data.OCR[0] & 0x40)
                            {
                              sd_mmc_data.hc = 1;
                              sd_mmc_data.version = 4;
                            }else
                            {
                              sd_mmc_data.hc = 0;
                              sd_mmc_data.version = 3;
                              SD_MMC_Command(SD_MMC_SET_BLOCKLEN, 0x00000200, 0xFF, NULL, 0); // set the block size to 512 bytes
                            }
                          }
                        }
                    }
                }else
                {
                  sd_mmc_data.initRequest = 1;
                }
              }
          }
          if (sd_mmc_data.initRequest == 0)
          {
              if (SD_MMC_Command(SD_MMC_CMD9, 0x00000000, 0xAF, sd_mmc_data.CSD, sizeof(sd_mmc_data.CSD)) != 0x00)
              {
                sd_mmc_data.csdValid = 0;
                sd_mmc_data.size = 0;
              }else
              {
                sd_mmc_data.csdValid = 1;
                if (sd_mmc_data.CSD[0] & 0xC0)
                { // V2 or higher device
                  // It is a 22-bit number in bit position 69:48.
                  uint32_t c_size = (((uint32_t)sd_mmc_data.CSD[7] & 0x3F) << 16) |
                                    ((uint32_t)sd_mmc_data.CSD[8] << 8) |
                                    ((uint32_t)sd_mmc_data.CSD[9]);
                  sd_mmc_data.size = ((uint32_t)(c_size + 1) * 1024u);
                }else
                { // V1 or lower device
                  // It is a 12-bit number in bit position 73:62
                  uint32_t c_size = ((uint32_t)(sd_mmc_data.CSD[6] & 0x03) << 10) |
                                    ((uint32_t)(sd_mmc_data.CSD[7]) << 2) |
                                               (sd_mmc_data.CSD[8] >> 6);
                  // C_SIZE_MULT: It is a 3-bit number in bit position 49:47.
                  uint32_t c_size_mult = ( (sd_mmc_data.CSD[ 9] << 1) |
                                           (sd_mmc_data.CSD[10] >> 7) ) & 0x07;
                  //  BLOCK_LEN field from the response. It is a 4-bit number in bit position 83:80.
                  uint32_t block_len = sd_mmc_data.CSD[5] & 0x0F;
                  block_len = 1 << (block_len - 9); //-9 because we report the size in sectors of 512 bytes each
                  sd_mmc_data.size = ((uint32_t)(c_size + 1) * ((uint32_t)1 << (c_size_mult + 2)) * block_len) - 1;
                }
                {
                    // It is a 12-bit number in bit position 73:62
                    uint32_t c_size = (((uint32_t)sd_mmc_data.CSD[6] & 0x03) << 10) |
                                      (((uint32_t)sd_mmc_data.CSD[7]       ) <<  2) |
                                      (           sd_mmc_data.CSD[8]         >>  6);
                    // C_SIZE_MULT: It is a 3-bit number in bit position 49:47.
                    uint32_t c_size_mult = ( (sd_mmc_data.CSD[ 9] << 1) |
                                             (sd_mmc_data.CSD[10] >> 7) ) & 0x07;
                    c_size_mult = ((uint32_t)1 << (c_size_mult + 2));
                    //  BLOCK_LEN field from the response. It is a 4-bit number in bit position 83:80.
                    uint32_t block_len = sd_mmc_data.CSD[5] & 0x0F;
                    //block_len = 1 << (block_len - 9); //-9 because we report the size in sectors of 512 bytes each
                    block_len = 1ul << block_len;
                    uint32_t size = ((c_size + 1) * c_size_mult * block_len) - 1;
                    sd_mmc_data.size2 = size;
                }
              }
              if (SD_MMC_Command(SD_MMC_CMD10, 0x00000000, 0xFF, sd_mmc_data.CID, sizeof(sd_mmc_data.CID)) != 0x00)
              {
                sd_mmc_data.cidValid = 0;
              }else
              {
                sd_mmc_data.cidValid = 1;
              }
              //SD_MMC_Command(SD_MMC_CRC_ON_OFF, 0x00000000, 0x25, NULL, 0); /* turn CRC off */
              //SD_MMC_Command(SD_MMC_SET_BLOCKLEN, 512, 0xFF, NULL, 0);
              SD_MMC_Spi_Fast();
              sd_mmc_data.state = e_SD_MMC_Ready;
          }
      }
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
      for (uint8_t i = 0; i < 16; i++)
      {

      }
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
