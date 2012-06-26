#include "stm32f0xx.h"
#include "stm32f0xx_gpio.h"
//#include "stdint.h"

int var1;
struct s
{
  int v1;
  short v2;
  char v3;
}s_var;

double fv = 5.0;

GPIO_TypeDef * const gpioc = (GPIO_TypeDef *)GPIOC_BASE;
RCC_TypeDef * const rcc = RCC;

static void func(void)
{
  s_var.v1 += 5;
  //fv = fv - 2.0;
  fv = - 2.0;
}

inline void GPIO_PortInit(GPIO_TypeDef * const gpio, uint8_t portnum)
{
  gpio->OSPEEDR = GPIO_Speed_50MHz << (portnum * 2);
  gpio->OTYPER = GPIO_OType_PP << portnum;
  gpio->MODER = GPIO_Mode_OUT << (portnum * 2);
  gpio->PUPDR = GPIO_PuPd_NOPULL << (portnum * 2);
}

inline void LED3_Init(void)
{
  /* enable the GPIO-C */
  RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
  GPIO_PortInit(GPIOC, 9);
}

void LED4_Init(void)
{
  /* enable the GPIO-C */
  RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
  GPIO_PortInit(GPIOC, 8);
}

void main(void)
{
  #define GPIO_Pin_9_num 9
#if 0
  #define GPIO_OType_PP 0
  #define GPIO_OType_OD 1
#define GPIO_Mode_IN      0
#define GPIO_Mode_OUT     1
#define GPIO_Mode_AF      2
#define GPIO_Mode_Rserved 3
#define GPIO_PuPd_NOPULL 0
#define GPIO_PuPd_PU     1
#define GPIO_PuPd_PD     2
#endif
  RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
  gpioc->OSPEEDR = GPIO_Speed_50MHz << (GPIO_Pin_9_num * 2);
  gpioc->OTYPER = GPIO_OType_PP << GPIO_Pin_9_num;
  gpioc->MODER = GPIO_Mode_OUT << (GPIO_Pin_9_num * 2);
  gpioc->PUPDR = GPIO_PuPd_NOPULL << (GPIO_Pin_9_num * 2);
  var1++;
  s_var.v2 = s_var.v3 +22;
  while(s_var.v1 < 40)
  {
    func();
    gpioc->ODR ^= GPIO_Pin_9;
  }
}
