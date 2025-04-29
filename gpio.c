#include "FaultHandler.h"
#include "bitfield_lib.h"

#include "gpio.h"

static inline void GPIO_PortEnable(GPIO_TypeDef * const gpio)
{
  if (gpio == GPIOA)
  { /* enable the GPIO-A */
      RCC->IOPENR |= RCC_IOPENR_GPIOAEN;
  }else
  if (gpio == GPIOB)
  { /* enable the GPIO-B */
      RCC->IOPENR |= RCC_IOPENR_GPIOBEN;
  }else
  if (gpio == GPIOC)
  { /* enable the GPIO-C */
      RCC->IOPENR |= RCC_IOPENR_GPIOCEN;
  }else
  if (gpio == GPIOD)
  { /* enable the GPIO-D */
      RCC->IOPENR |= RCC_IOPENR_GPIODEN;
  }else
    CAT_Error(CAT_InvalidParameter, 0);
}

/**
 * GPIO_PortInit
 */
void GPIO_PortInit(GPIO_TypeDef * const gpio, uint8_t portnum, uint8_t ospeed, uint8_t otyper, uint8_t moder, uint8_t pupdr, uint8_t AFR_val)
{
  GPIO_PortEnable(gpio);
  BitfieldSet(gpio->OSPEEDR, portnum * 2, 2, ospeed);
  BitfieldSet(gpio->OTYPER, portnum, 1, otyper);
  BitfieldSet(gpio->MODER, portnum * 2, 2, moder);
  BitfieldSet(gpio->PUPDR, portnum * 2, 2, pupdr);
  if (portnum <= 7)
  {
    BitfieldSet(gpio->AFR[0], portnum * 4, 4, AFR_val);
  }else
  {
    BitfieldSet(gpio->AFR[1], (portnum - 8) * 4, 4, AFR_val);
  }
}

void GPIO_PortInit_Out(GPIO_TypeDef * const gpio, uint8_t portnum)
{
  GPIO_PortEnable(gpio);
  //gpio->OSPEEDR = GPIO_Speed_50MHz << (portnum * 2);
  BitfieldSet(gpio->OSPEEDR, portnum * 2, 2, GPIO_OSPEEDR_VERY_HIGH_SPEED);
  //gpio->OTYPER = GPIO_OType_PP << portnum;
  BitfieldSet(gpio->OTYPER, portnum, 1, GPIO_OTYPER_PP);
  //gpio->MODER = GPIO_Mode_OUT << (portnum * 2);
  BitfieldSet(gpio->MODER, portnum * 2, 2, GPIO_MODER_OUT);
  //gpio->PUPDR = GPIO_PuPd_NOPULL << (portnum * 2);
  BitfieldSet(gpio->PUPDR, portnum * 2, 2, GPIO_PUPDR_NOPULL);
}

void GPIO_PortInit_OC(GPIO_TypeDef * const gpio, uint8_t portnum)
{ // the port is set to open-collector/drain output with 2MHz
  GPIO_PortEnable(gpio);
  BitfieldSet(gpio->OSPEEDR, portnum * 2, 2, GPIO_OSPEEDR_VERY_HIGH_SPEED);
  BitfieldSet(gpio->OTYPER, portnum, 1, GPIO_OTYPER_OD);
  BitfieldSet(gpio->MODER, portnum * 2, 2, GPIO_MODER_OUT);
  BitfieldSet(gpio->PUPDR, portnum * 2, 2, GPIO_PUPDR_PULLUP);
}

void GPIO_PortInit_AFOut(GPIO_TypeDef * const gpio, uint8_t portnum, uint8_t AFR_val)
{
  GPIO_PortEnable(gpio);

  //gpio->OSPEEDR = GPIO_Speed_50MHz << (portnum * 2);
  BitfieldSet(gpio->OSPEEDR, portnum * 2, 2, GPIO_OSPEEDR_VERY_HIGH_SPEED);
  //gpio->OTYPER = GPIO_OType_PP << portnum;
  BitfieldSet(gpio->OTYPER, portnum, 1, GPIO_OTYPER_PP);
  //gpio->MODER = GPIO_Mode_OUT << (portnum * 2);
  BitfieldSet(gpio->MODER, portnum * 2, 2, GPIO_MODER_AF);
  //gpio->PUPDR = GPIO_PuPd_NOPULL << (portnum * 2);
  BitfieldSet(gpio->PUPDR, portnum * 2, 2, GPIO_PUPDR_NOPULL);
  if (portnum <= 7)
  {
    BitfieldSet(gpio->AFR[0], portnum * 4, 4, AFR_val);
  }else
  {
    BitfieldSet(gpio->AFR[1], (portnum - 8) * 4, 4, AFR_val);
  }
}

void GPIO_PortInit_In(GPIO_TypeDef * const gpio, uint8_t portnum)
{
  GPIO_PortEnable(gpio);

  //gpio->OSPEEDR = GPIO_Speed_50MHz << (portnum * 2);
  BitfieldSet(gpio->OSPEEDR, portnum * 2, 2, GPIO_OSPEEDR_VERY_HIGH_SPEED);
  //gpio->OTYPER = GPIO_OType_PP << portnum;
  BitfieldSet(gpio->OTYPER, portnum, 1, GPIO_OTYPER_PP);
  //gpio->MODER = GPIO_Mode_OUT << (portnum * 2);
  BitfieldSet(gpio->MODER, portnum * 2, 2, GPIO_MODER_IN);
  //gpio->PUPDR = GPIO_PuPd_NOPULL << (portnum * 2);
  BitfieldSet(gpio->PUPDR, portnum * 2, 2, GPIO_PUPDR_NOPULL);
}

void GPIO_PortInit_Analog(GPIO_TypeDef * const gpio, uint8_t portnum)
{
  GPIO_PortEnable(gpio);

  //gpio->OSPEEDR = GPIO_Speed_50MHz << (portnum * 2);
  BitfieldSet(gpio->OSPEEDR, portnum * 2, 2, GPIO_OSPEEDR_VERY_HIGH_SPEED);
  //gpio->OTYPER = GPIO_OType_PP << portnum;
  BitfieldSet(gpio->OTYPER, portnum, 1, GPIO_OTYPER_PP);
  //gpio->MODER = GPIO_Mode_OUT << (portnum * 2);
  BitfieldSet(gpio->MODER, portnum * 2, 2, GPIO_MODER_AN);
  //gpio->PUPDR = GPIO_PuPd_NOPULL << (portnum * 2);
  BitfieldSet(gpio->PUPDR, portnum * 2, 2, GPIO_PUPDR_NOPULL);
}
