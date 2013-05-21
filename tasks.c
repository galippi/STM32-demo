#include "gpio_app.h"
#include "debug.h"
#include "dac.h"
#include "adc_app.h"

#include "tasks.h"

#define DEBUG_PORT GPIOD

#define DEBUG_1ms_PORT      DEBUG_PORT
#define DEBUG_1ms_PIN_NUM   0

#define DEBUG_10ms_PORT     DEBUG_PORT
#define DEBUG_10ms_PIN_NUM  2

#define DEBUG_500ms_PORT    DEBUG_PORT
#define DEBUG_500ms_PIN_NUM 6

void Task_Init(void)
{
  DebugOut_Init();
  GPIO_PortInit_Out(DEBUG_1ms_PORT,   DEBUG_1ms_PIN_NUM);
  GPIO_PortInit_Out(DEBUG_10ms_PORT,  DEBUG_10ms_PIN_NUM);
  GPIO_PortInit_Out(DEBUG_500ms_PORT, DEBUG_500ms_PIN_NUM);
}

volatile uint8_t Task_1ms_ctr;

void Task_1ms(void)
{
  GPIO_Set(DEBUG_1ms_PORT, DEBUG_1ms_PIN_NUM, 1);
  Task_1ms_ctr++;
  PB13_Set(!PB13_Get()); /* toggling debug port */
  {
    static uint16_t dac_val = 0;
    if (dac_val >= 0x1000)
    {
      DAC_1_Set(0x1FFF - dac_val);
      DAC_2_Set(dac_val - 0x1000);
    }else
    {
      DAC_1_Set(dac_val);
      DAC_2_Set(0x1000 - 1 - dac_val);
    }
    dac_val = (dac_val + 4) & 0x1FFF; /* 1sec rising / 1sec falling edge */
  }
  GPIO_Set(DEBUG_1ms_PORT, DEBUG_1ms_PIN_NUM, 0);
}

//#include "scheduler_preemptive.h"

volatile uint8_t Task_10ms_ctr;

void Task_10ms(void)
{
  GPIO_Set(DEBUG_10ms_PORT, DEBUG_10ms_PIN_NUM, 1);
  Task_10ms_ctr++;
  DebugOut();
  {
    uint8_t old_u8 = Task_1ms_ctr;
    while (old_u8 == Task_1ms_ctr)
    {
      /* wait the activity of the 1ms task - testing nested/preemptive interrupt */
//      SchedulerPre_TaskTableUpdate(); /* simulating 1ms timer */
    }
  }
  GPIO_Set(DEBUG_10ms_PORT, DEBUG_10ms_PIN_NUM, 0);
}

uint8_t old_u8;;

void Task_500ms(void)
{
  GPIO_Set(DEBUG_500ms_PORT, DEBUG_500ms_PIN_NUM, 1);
  {
    static uint8_t port;
    if ((port == 0) || (port & (port - 1)) || (port > 8))
    { /* invalid port value -> reinit it */
      port = 1;
    }
    LED3_Set(port & 1);
    LED5_Set(port & 2);
    LED6_Set(port & 4);
    LED4_Set(port & 8);
    port = port << 1;
  }
#if 1
  old_u8 = Task_10ms_ctr;
#else
  uint8_t old_u8 = Task_10ms_ctr;
#endif
  while (((uint8_t)(Task_10ms_ctr - old_u8)) < 5)
  { /* wait 50 ms */
    //SchedulerPre_TaskTableUpdate(); /* simulating 1ms timer */
  }
  GPIO_Set(DEBUG_500ms_PORT, DEBUG_500ms_PIN_NUM, 0);
}
