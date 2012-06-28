#include "stdint.h"
#include "stm32f0xx.h"
#include "stm32f0xx_gpio.h"

int var1;
struct s
{
  int v1;
  short v2;
  char v3;
}s_var;

double fv = 5.0;

GPIO_TypeDef * const gpioa = GPIOA;
GPIO_TypeDef * const gpioc = GPIOC;
RCC_TypeDef * const rcc = RCC;

static void func(void)
{
  s_var.v1 += 5;
  //fv = fv - 2.0;
  fv = - 2.0;
}

#define BitfieldSet(var, bit_num, bit_length, val) ((var) = ((var) & (~(((1 << (bit_length)) - 1)) << (bit_num)) | ((val) << (bit_num))))

void CAT_Error(uint8_t code)
{
  while(1)
    ; /* endless loop */
}

inline void GPIO_PortInit_Out(GPIO_TypeDef * const gpio, uint8_t portnum)
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

inline void GPIO_PortInit_In(GPIO_TypeDef * const gpio, uint8_t portnum)
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

#define LED3_PORT GPIOC
#define LED3_PIN_NUM 9
#define LED4_PORT GPIOC
#define LED4_PIN_NUM 8
#define BUTTON1_PORT GPIOA
#define BUTTON1_PIN_NUM 0

inline void Button1_Init(void)
{
  GPIO_PortInit_In(BUTTON1_PORT, BUTTON1_PIN_NUM);
}

uint8_t Button1_Get(void)
{
  return (BUTTON1_PORT->IDR & (1 << BUTTON1_PIN_NUM)) ? 1 : 0;
}

inline void LED3_Init(void)
{
  GPIO_PortInit_Out(LED3_PORT, LED3_PIN_NUM);
}

void LED3_Set(uint8_t val)
{
  if (val)
  {
    LED3_PORT->ODR |= (1 << LED3_PIN_NUM);
  }else
  {
    LED3_PORT->ODR &= ~(1 << LED3_PIN_NUM);
  }
}

inline void LED4_Init(void)
{
  GPIO_PortInit_Out(LED4_PORT, LED4_PIN_NUM);
}

void LED4_Set(uint8_t val)
{
  if (val)
  {
    LED3_PORT->ODR |= (1 << LED4_PIN_NUM);
  }else
  {
    LED3_PORT->ODR &= ~(1 << LED4_PIN_NUM);
  }
}

void main(void)
{
  uint8_t led3 = 0;
  LED3_Init();
  LED4_Init();
  Button1_Init();

  var1++;
  s_var.v2 = s_var.v3 +22;
  s_var.v1 = 0;
  while(s_var.v1 < 40)
  {
    func();
    {
      LED3_Set(led3);
      led3 = !led3;
      LED4_Set(led3);
      if (Button1_Get())
      {
        var1++;
      }
    }
  }
}
