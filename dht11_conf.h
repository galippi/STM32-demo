#ifndef _DHT11_CONF_H_
#define _DHT11_CONF_H_

#include "timer.h"
#include "gpio.h"

#define DHT_MODE DHT_MODE_22

#define DHT11_DEBUG 1

#define DHT_TimerResolution 0.000001 /* sec */
#define DHT_Time2TimerTick(sec) ((uint32_t)((sec) / (DHT_TimerResolution)))
#define DHT_Time2TimerTickUs(us) DHT_Time2TimerTick((us)*0.000001)
#define DHT_TimeCheck(timer, us, diff) (((timer) >= (DHT_Time2TimerTickUs((us)-(diff))) && ((timer) <= (DHT_Time2TimerTickUs((us)+(diff))))

#define DHT11_run_period_time 0.010
#define DHT11_run_period_time_us (uint32_t)((DHT11_run_period_time) / 0.000001)
#define DHT11_PULSE_LOW_CTR (uint32_t)(0.021 / (DHT11_run_period_time)) /* min 18/20ms */
#define DHT11_PULSE_TIMEOUT (uint32_t)(0.008 / (DHT_TimerResolution)) /* 8000 us <= 80 bit * 100us */
#define DHT11_STANDBY_CNT (uint32_t)(2.5 / (DHT11_run_period_time)) /* 2.5 sec standby after the last measurement */

/* Port-A - pin 6 - DHT11 - data - TIM3 - CH1/CH2 */
//#define DHT11_PORT_IN  GPIOB, 8
#define DHT11_PORT_OUT GPIOA, 6
#define DHT11_PORT_INIT() \
    do { \
        /* GPIO_PortInit_AFOut(DHT11_PORT_OUT, 1); */ \
        GPIO_PortInit(DHT11_PORT_OUT, GPIO_OSPEEDR_LOW_SPEED, GPIO_OTYPER_OD, GPIO_MODER_AF, GPIO_PUPDR_PULLUP, 1); \
        GPIO_Set(DHT11_PORT_OUT, 0); \
    } while(0)

static inline void GPIO_SetToOD(GPIO_TypeDef * const port, uint8_t pin)
{
    BitfieldSet(port->MODER, pin * 2, 2, GPIO_MODER_OUT);
}

static inline void GPIO_SetToAF(GPIO_TypeDef * const port, uint8_t pin)
{
    BitfieldSet(port->MODER, pin * 2, 2, GPIO_MODER_AF);
}

//#define DHT11_OUT_LOW()  GPIO_Set(DHT11_PORT_OUT, 0)
#define DHT11_OUT_LOW() GPIO_SetToOD(DHT11_PORT_OUT)

//#define DHT11_OUT_HIGH() GPIO_Set(DHT11_PORT_OUT, 1)
#define DHT11_OUT_HIGH() GPIO_SetToAF(DHT11_PORT_OUT)

#define DHT11_CC_IRQ_ENABLE() \
    do { \
        TIM3_SR_CC1IF_Reset(); \
        TIM3_SR_CC1OF_Reset(); \
        TIM3_SR_CC2IF_Reset(); \
        TIM3_SR_CC2OF_Reset(); \
        TIM3->DIER |= (TIM_DIER_CC1IE | TIM_DIER_CC2IE); \
        NVIC_ClearPendingIRQ(TIM3_IRQn); \
        NVIC_EnableIRQ(TIM3_IRQn); \
    }while(0)

#define DHT11_get_time() TIM3_Cnt_Get()

#define DHT11_get_time_since_last_irq() \
    ((DHT11_get_time() - dht11.DHT11_lastTimer) & 0xFFFF)

//#define DHT11_WAIT_US(us)
#define DHT11_ANSWER_PULSE_CNT 84
#define DHT11_IRQ_CNT_MAX 128

#define TIM3_CC1IF_Callback() \
    do { \
        uint16_t timer = TIM3_CCR1_Get(); \
        dht11_IRQ_cb(timer); \
    }while(0)

#define TIM3_CC2IF_Callback() \
    do { \
        uint16_t timer = TIM3_CCR2_Get(); \
        dht11_IRQ_cb(timer); \
    }while(0)

#define DHT11_MissingFallingEdgeIRQ_cb() /* do nothing */

#define DHT_ResultTimeoutCtr 3
#define DHT_ResultTimeout_cb() /* do nothing */

extern void DHT_ResultDebug(uint16_t resultCtr, uint16_t resultChecksumCtr);
#define DHT_Result_cb() DHT_ResultDebug(dht11.dbg.resultCtr, dht11.dbg.resultChecksumCtr)

#endif /* _DHT11_CONF_H_ */
