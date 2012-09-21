#include "stm32f0xx_gpio.h"

#include "bitfield_lib.h"

#include "gpio.h"

void GPIO_PortInit_Out(GPIO_TypeDef * const gpio, uint8_t portnum)
{
  if (gpio == GPIOC)
  {
    /* enable the GPIO-C, if it was not enabled */
    if (!(RCC->AHBENR & RCC_AHBENR_GPIOCEN))
    {
      RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    }
  }else
    CAT_Error(1);

  //gpio->OSPEEDR = GPIO_Speed_50MHz << (portnum * 2);
  BitfieldSet(gpio->OSPEEDR, portnum * 2, 2, GPIO_Speed_50MHz);
  //gpio->OTYPER = GPIO_OType_PP << portnum;
  BitfieldSet(gpio->OTYPER, portnum, 1, GPIO_OType_PP);
  //gpio->MODER = GPIO_Mode_OUT << (portnum * 2);
  BitfieldSet(gpio->MODER, portnum * 2, 2, GPIO_Mode_OUT);
  //gpio->PUPDR = GPIO_PuPd_NOPULL << (portnum * 2);
  BitfieldSet(gpio->PUPDR, portnum * 2, 2, GPIO_PuPd_NOPULL);
}

void GPIO_PortInit_In(GPIO_TypeDef * const gpio, uint8_t portnum)
{
  if (gpio == GPIOA)
  {
    /* enable the GPIO-A, if it was not enabled */
    if (!(RCC->AHBENR & RCC_AHBENR_GPIOAEN))
    {
      RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    }
  }else
    CAT_Error(1);

  //gpio->OSPEEDR = GPIO_Speed_50MHz << (portnum * 2);
  BitfieldSet(gpio->OSPEEDR, portnum * 2, 2, GPIO_Speed_50MHz);
  //gpio->OTYPER = GPIO_OType_PP << portnum;
  BitfieldSet(gpio->OTYPER, portnum, 1, GPIO_OType_PP);
  //gpio->MODER = GPIO_Mode_OUT << (portnum * 2);
  BitfieldSet(gpio->MODER, portnum * 2, 2, GPIO_Mode_IN);
  //gpio->PUPDR = GPIO_PuPd_NOPULL << (portnum * 2);
  BitfieldSet(gpio->PUPDR, portnum * 2, 2, GPIO_PuPd_NOPULL);
}

void GPIO_PortInit_Analog(GPIO_TypeDef * const gpio, uint8_t portnum)
{
  if (gpio == GPIOA)
  {
    /* enable the GPIO-A, if it was not enabled */
    if (!(RCC->AHBENR & RCC_AHBENR_GPIOAEN))
    {
      RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    }
  }else
    CAT_Error(1);

  //gpio->OSPEEDR = GPIO_Speed_50MHz << (portnum * 2);
  BitfieldSet(gpio->OSPEEDR, portnum * 2, 2, GPIO_Speed_50MHz);
  //gpio->OTYPER = GPIO_OType_PP << portnum;
  BitfieldSet(gpio->OTYPER, portnum, 1, GPIO_OType_PP);
  //gpio->MODER = GPIO_Mode_OUT << (portnum * 2);
  BitfieldSet(gpio->MODER, portnum * 2, 2, GPIO_Mode_AN);
  //gpio->PUPDR = GPIO_PuPd_NOPULL << (portnum * 2);
  BitfieldSet(gpio->PUPDR, portnum * 2, 2, GPIO_PuPd_NOPULL);
}
