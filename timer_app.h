#ifndef _TIMER_APP_H_
#define _TIMER_APP_H_

#include "gpio_app.h"

#include "timer.h"
#include "timer_conf.h"

static inline void TIM3_UIF_PollHandler(void)
{
  if (TIM3_SR_UIF_Get())
  {
    TIM3_SR_UIF_Reset();
    TIM3_UIF_Callback();
  }
}

extern uint8_t tim3_cc3_ctr;

static inline void TIM3_CC3IF_Callback(void)
{
  tim3_cc3_ctr++;
}

extern uint8_t tim3_cc4_ctr;
static inline void TIM3_CC4IF_Callback(void)
{
  tim3_cc4_ctr++;
}

static inline uint16_t getTimer_us(void)
{
  return TIM14_Cnt_Get();
}

/**
 * Waiting specified microsec
 * @param time - waiting time in us
 */
static inline void wait_us(uint32_t time)
{
  uint16_t t_start = getTimer_us();
  while (time > 16384)
  {
    wait_us(16384);
    time -= 16384;
    t_start += 16384;
  }
  while (((getTimer_us() - t_start) & 0xFFFF) < time)
  { /* wait - do nothing */
  }
}

#ifndef TIM4_CC1IF_Callback
#define TIM4_CC1IF_Callback() /* do nothing */
#endif

#ifndef TIM4_CC2IF_Callback
#define TIM4_CC2IF_Callback() /* do nothing */
#endif

#ifndef TIM4_CC3IF_Callback
#define TIM4_CC3IF_Callback() /* do nothing */
#endif

#ifndef TIM4_CC4IF_Callback
#define TIM4_CC4IF_Callback() /* do nothing */
#endif

uint16_t tim14_uif_ctr;

static inline void TIM14_UIF_Callback(void)
{
  tim14_uif_ctr++;
}

#endif /* _TIMER_APP_H_ */
