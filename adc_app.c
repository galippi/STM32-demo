#include "util.h"
#include "adc.h"
#include "gpio.h"
#include "debug.h"

#include "adc_app.h"

uint16_t ADC_values[ADC_Ch_Num];

int16_t Temperature = -32768;
int16_t Temperature_raw = -32768;

static uint8_t adc_idx;
void ADC_HandlerInit(void)
{
  adc_idx = 0;
  GPIO_PortInit_Analog(GPIOB, 0);
  GPIO_PortInit_Analog(GPIOB, 1);
  ADC_Init();
  ADC_Start();
}

uint16_t ADC_ctr;

void ADC_Handler(void)
{
  if (ADC_GetStatus())
  {
    ADC_values[adc_idx] = ADC_Get();
    adc_idx ++;
    if (adc_idx >= ADC_VALUES_NUM)
    {
      adc_idx = 0;
      uint32_t v_ref_factor;
      #define V_REF_FACTOR_SCALE 16
      if (ADC_values[ADC_Vref] > 1000)
      {
        v_ref_factor = DivU32_U32U32(*VREFINT_CAL << V_REF_FACTOR_SCALE, ADC_values[ADC_Vref]);
      }else
      {
        v_ref_factor = 1 << V_REF_FACTOR_SCALE;
      }
      uint16_t v_sensor = (ADC_values[ADC_TemperatureSensor] * v_ref_factor) >> V_REF_FACTOR_SCALE;
      //Temperature = (((ADC_values[ADC_TemperatureSensor] - (int16_t)*TS_CAL1) * (int16_t)(110 - 30)) / (*TS_CAL2 - *TS_CAL1)) + 30;
      //Temperature = (int8_t)DivI32_I32I32(((v_sensor - (int16_t)*TS_CAL1) * (int16_t)(110 - 30)), (*TS_CAL2 - *TS_CAL1)) + 30;
      Temperature = (int16_t)DivI32_I32I32(((v_sensor - (int32_t)*TS_CAL1) * ((int32_t)(110 - 30) * TEMP_SCALE)), (*TS_CAL2 - *TS_CAL1)) + (30 * TEMP_SCALE);
      Temperature_raw = (int16_t)DivI32_I32I32(((ADC_values[ADC_TemperatureSensor] - (int32_t)*TS_CAL1) * ((int32_t)(110 - 30) * TEMP_SCALE)), (*TS_CAL2 - *TS_CAL1)) + (30 * TEMP_SCALE);
#if TARGET_ECU == TARGET_ECU_STM32F0DISCOVERY
      DebugOut_Set_u16(0, 0, ADC_values[ADC_IN4_DAC]);
      DebugOut_Set_u16(0, 2, ADC_values[ADC_IN5_Ub]);
      DebugOut_Set_u16(0, 4, ADC_values[ADC_IN6_Ib]);
#elif (CPU_TYPE == CPU_TYPE_STM32F1)
      DebugOut_Set_u16(0,  0, ADC_values[ADC_PA0]);
      DebugOut_Set_u16(0,  2, ADC_values[ADC_PA1]);
      DebugOut_Set_u16(0,  4, ADC_values[ADC_PB0]);
      //DebugOut_Set_u16(0,  8, ADC_values[ADC_PB1]);
      //DebugOut_Set_u16(0, 10, ADC_values[ADC_Vref]);
#endif
      DebugOut_Set_u16(0, 6, ADC_values[ADC_TemperatureSensor]);
    }
    //ADC_Start();
    ADC_ctr++;
  }
}
