#include "FaultHandler.h"
#include "bitfield_lib.h"

#include "gpio.h"

#include STM32_GPIO_HEADER

#if (CPU_TYPE == CPU_TYPE_STM32F0) || (CPU_TYPE == CPU_TYPE_STM32F4)
static inline void GPIO_PortEnable(GPIO_TypeDef * const gpio)
{
  if (gpio == GPIOA)
  { /* enable the GPIO-B */
#if CPU_TYPE == CPU_TYPE_STM32F0
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
#elif CPU_TYPE == CPU_TYPE_STM32F4
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
#endif
  }else
  if (gpio == GPIOB)
  { /* enable the GPIO-B */
#if CPU_TYPE == CPU_TYPE_STM32F0
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
#elif CPU_TYPE == CPU_TYPE_STM32F4
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
#endif
  }else
  if (gpio == GPIOC)
  { /* enable the GPIO-C */
#if CPU_TYPE == CPU_TYPE_STM32F0
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
#elif CPU_TYPE == CPU_TYPE_STM32F4
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
#endif
  }else
  if (gpio == GPIOD)
  { /* enable the GPIO-D */
#if CPU_TYPE == CPU_TYPE_STM32F0
    RCC->AHBENR |= RCC_AHBENR_GPIODEN;
#elif CPU_TYPE == CPU_TYPE_STM32F4
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
#endif
  }else
    CAT_Error(CAT_InvalidParameter, 0);
}

void GPIO_PortInit_Out(GPIO_TypeDef * const gpio, uint8_t portnum)
{
  GPIO_PortEnable(gpio);
  //gpio->OSPEEDR = GPIO_Speed_50MHz << (portnum * 2);
  BitfieldSet(gpio->OSPEEDR, portnum * 2, 2, GPIO_Speed_50MHz);
  //gpio->OTYPER = GPIO_OType_PP << portnum;
  BitfieldSet(gpio->OTYPER, portnum, 1, GPIO_OType_PP);
  //gpio->MODER = GPIO_Mode_OUT << (portnum * 2);
  BitfieldSet(gpio->MODER, portnum * 2, 2, GPIO_Mode_OUT);
  //gpio->PUPDR = GPIO_PuPd_NOPULL << (portnum * 2);
  BitfieldSet(gpio->PUPDR, portnum * 2, 2, GPIO_PuPd_NOPULL);
}

void GPIO_PortInit_AFOut(GPIO_TypeDef * const gpio, uint8_t portnum, uint8_t AFR_val)
{
  GPIO_PortEnable(gpio);

  //gpio->OSPEEDR = GPIO_Speed_50MHz << (portnum * 2);
  BitfieldSet(gpio->OSPEEDR, portnum * 2, 2, GPIO_Speed_50MHz);
  //gpio->OTYPER = GPIO_OType_PP << portnum;
  BitfieldSet(gpio->OTYPER, portnum, 1, GPIO_OType_PP);
  //gpio->MODER = GPIO_Mode_OUT << (portnum * 2);
  BitfieldSet(gpio->MODER, portnum * 2, 2, GPIO_Mode_AF);
  //gpio->PUPDR = GPIO_PuPd_NOPULL << (portnum * 2);
  BitfieldSet(gpio->PUPDR, portnum * 2, 2, GPIO_PuPd_NOPULL);
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
  GPIO_PortEnable(gpio);

  //gpio->OSPEEDR = GPIO_Speed_50MHz << (portnum * 2);
  BitfieldSet(gpio->OSPEEDR, portnum * 2, 2, GPIO_Speed_50MHz);
  //gpio->OTYPER = GPIO_OType_PP << portnum;
  BitfieldSet(gpio->OTYPER, portnum, 1, GPIO_OType_PP);
  //gpio->MODER = GPIO_Mode_OUT << (portnum * 2);
  BitfieldSet(gpio->MODER, portnum * 2, 2, GPIO_Mode_AN);
  //gpio->PUPDR = GPIO_PuPd_NOPULL << (portnum * 2);
  BitfieldSet(gpio->PUPDR, portnum * 2, 2, GPIO_PuPd_NOPULL);
}
#elif (CPU_TYPE == CPU_TYPE_STM32F1)

static inline void GPIO_PortEnable(GPIO_TypeDef * const gpio)
{
  RCC->APB1ENR |= RCC_APB1ENR_PWREN;
  if (gpio == GPIOA)
  { /* enable the GPIO-A */
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
  }else
  if (gpio == GPIOB)
  { /* enable the GPIO-B */
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
  }else
  if (gpio == GPIOC)
  { /* enable the GPIO-C */
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
  }else
  if (gpio == GPIOD)
  { /* enable the GPIO-D */
    RCC->APB2ENR |= RCC_APB2ENR_IOPDEN;
  }else
    CAT_Error(CAT_InvalidParameter, 1);
}

void GPIO_PortInit_Out(GPIO_TypeDef * const gpio, uint8_t portnum)
{
  GPIO_PortEnable(gpio);
  if (portnum < 8)
  {
  	// the port is set to push-pull output with 50MHz
    BitfieldSet(gpio->CRL, portnum * 4, 4, ((GPIO_Mode_Out_PP & 0x0F) | GPIO_Speed_50MHz));
  }else
  {
  	// the port is set to push-pull output with 50MHz
    BitfieldSet(gpio->CRH, (portnum - 8) * 4, 4, ((GPIO_Mode_Out_PP & 0x0F) | GPIO_Speed_50MHz));
  }
}

void GPIO_PortInit_In(GPIO_TypeDef * const gpio, uint8_t portnum)
{
  GPIO_PortEnable(gpio);
  if (portnum < 8)
  {
  	// the port is set to floating input
    BitfieldSet(gpio->CRL, portnum * 4, 4, GPIO_Mode_IN_FLOATING);
  }else
  {
  	// the port is set to floating input
    BitfieldSet(gpio->CRH, (portnum - 8) * 4, 4, GPIO_Mode_IN_FLOATING);
  }
}

void GPIO_PortInit_AFOut(GPIO_TypeDef * const gpio, uint8_t portnum)
{
  GPIO_PortEnable(gpio);
  if (portnum <= 7)
  {
    BitfieldSet(gpio->CRL, portnum * 4, 4, ((GPIO_Mode_AF_PP & 0x0F) | GPIO_Speed_50MHz));
  }else
  {
    BitfieldSet(gpio->CRH, (portnum - 8) * 4, 4, ((GPIO_Mode_AF_PP & 0x0F) | GPIO_Speed_50MHz));
  }
}

#else
#error Not implemented CPU type!
#endif
