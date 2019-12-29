#ifndef _PLL_CONF_H_
#define _PLL_CONF_H_

#define f_HSE_Hz 8000000
#define f_LSE_Hz 32768
#define f_PLL_Hz 48000000
#define f_AHB_Hz 48000000
#define f_APB1_Hz 24000000 /* PCLK1 */
#define f_APB2_Hz 48000000 /* PCLK2 */
#define f_TIMXCLK_Hz 48000000
#define f_TIM1CLK_Hz 48000000
#define f_ADCCLK_Hz 12000000
#define f_USBCLK_Hz 48000000
#define f_RTCCLK_Hz 32768

#define RCC_CR_INIT (RCC_CR_HSEBYP | RCC_CR_HSEON | RCC_CR_PLLON)
#define PLLXTPRE_REG 0
#define PLLSRC   1
#define PLLMUL_VAL 6
#define SWS 2 /* PLL */
#define HPRE_REG  0
#define PPRE1_REG 4 /* APB1 prescaler */
#define PPRE2_REG 0 /* APB2 prescaler */
#define ADCPRE_REG 1
#define USBPRE_REG 1

#define LATENCY_VAL 1
#define PRFTBE_REG 1
#define HLFCYA_REG 0
#define FLASH_ACR (FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY)

#endif /* _PLL_CONF_H_ */
