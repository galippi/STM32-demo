#include <stdint.h>
#include <string.h>

#include "stm32f0xx.h"
#include "stm32f0xx_gpio.h"

#define VDD 3.0 /* Volt */

int var1;
struct s
{
  int v1;
  short v2;
  char v3;
}s_var;

GPIO_TypeDef * const gpioa = GPIOA;
GPIO_TypeDef * const gpioc = GPIOC;
RCC_TypeDef * const rcc = RCC;
SysTick_Type * const systick = SysTick;
DAC_TypeDef * const dac = DAC;

#define BitfieldSet(var, bit_num, bit_length, val) ((var) = ((var) & (~(((1 << (bit_length)) - 1)) << (bit_num)) | ((val) << (bit_num))))

void CAT_Error(uint8_t code)
{
  while(1)
    ; /* endless loop */
}

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

inline void DAC_Set(uint16_t val)
{
  DAC->DHR12R1 = val;
}

void DAC_Init(void)
{
  /* enable the GPIO-A, if it was not enabled */
  if (!(RCC->APB1ENR & RCC_APB1ENR_DACEN))
  {
    RCC->APB1ENR |= RCC_APB1ENR_DACEN;
  }
  /* enable the GPIO-A, if it was not enabled - DAC is connected to PA.4-5 */
  GPIO_PortInit_Analog(GPIOA, 4);
  DAC->CR = (DAC->CR & 0x303F) | 0x0001;
  DAC_Set((uint16_t)(1.1/VDD * 4096));
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

inline uint8_t Button1_Get(void)
{
  return (BUTTON1_PORT->IDR & (1 << BUTTON1_PIN_NUM)) ? 1 : 0;
}

inline void LED3_Init(void)
{
  GPIO_PortInit_Out(LED3_PORT, LED3_PIN_NUM);
}

inline void LED3_Set(uint8_t val)
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

inline void LED4_Set(uint8_t val)
{
  if (val)
  {
    LED3_PORT->ODR |= (1 << LED4_PIN_NUM);
  }else
  {
    LED3_PORT->ODR &= ~(1 << LED4_PIN_NUM);
  }
}

void SysTick_Init(void)
{
  uint32_t ticks = 65536;
  SysTick->LOAD  = (ticks & SysTick_LOAD_RELOAD_Msk) - 1;      /* set reload register */
  //NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);  /* set Priority for Cortex-M0 System Interrupts */
  SysTick->VAL   = 0;                                          /* Load the SysTick Counter Value */
  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                   SysTick_CTRL_TICKINT_Msk   |
                   SysTick_CTRL_ENABLE_Msk;                    /* Enable SysTick IRQ and SysTick Timer */
}

uint8_t timer[128];
#define SIZE_TIMER (sizeof(timer)/sizeof(timer[0]))
#define MAX_TIMER 255
uint8_t timer_brake;

inline uint16_t SysTick_Get(void)
{
  uint16_t val = SysTick->VAL & 0xFFFF;
  if (!timer_brake)
  {
    uint16_t idx = val + (SIZE_TIMER / 2);
    if (idx < SIZE_TIMER)
    {
      timer[idx] ++;
      if (timer[idx] == MAX_TIMER)
      {
        timer_brake = 1;
      }
    }
  }
  return val;
}

inline void *memset(void *ptr_, int data, size_t size)
{
  uint8_t *ptr = ptr_;
  while(size > 0)
  {
    *ptr = data;
    ptr++;
    size--;
  }
  return ptr_;
}

void main(void)
{
  uint8_t led3 = 0;
  SysTick_Init();
  LED3_Init();
  LED4_Init();
  Button1_Init();
  DAC_Init();
  memset(timer, 0, sizeof(timer));
  timer_brake = 0;
  {
    uint16_t val = 0;
    while(val < 4096)
    {
      DAC_Set(val);
      val = (val << 1) | 1;
    }
  }

  s_var.v1 = 0;
  //while(s_var.v1 < 40)
  {
    {
      LED3_Set(led3);
      led3 = !led3;
      LED4_Set(led3);
      if (Button1_Get())
      {
        s_var.v1++;
      }
    }
  }
  {
    while (1)
    {
#if 0
      SysTick_Get();
#else
      LED3_Set(1);
      while (SysTick_Get() > 32767)
        ;
      LED3_Set(0);
      while (SysTick_Get() <= 32767)
        ;
#endif
    }
  }
}
