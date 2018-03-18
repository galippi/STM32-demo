#include "util.h"
#include "adc.h"
#include "gpio.h"
#include "uart.h"
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

#if (CPU_TYPE == CPU_TYPE_STM32F0)
      DebugOut_Set_u16(0, 0, ADC_values[ADC_IN4_DAC]);
      DebugOut_Set_u16(0, 2, ADC_values[ADC_IN5_Ub]);
      DebugOut_Set_u16(0, 4, ADC_values[ADC_IN6_Ib]);
#elif (CPU_TYPE == CPU_TYPE_STM32F1)
      DebugOut_Set_u16(0,  0, ADC_values[ADC_IN0]);
      DebugOut_Set_u16(0,  2, ADC_values[ADC_IN1]);
      DebugOut_Set_u16(0,  4, ADC_values[ADC_IN8]);
      DebugOut_Set_u16(0,  8, ADC_values[ADC_IN9]);
      DebugOut_Set_u16(0, 10, ADC_values[ADC_Vref]);
#endif
      DebugOut_Set_u16(0, 6, ADC_values[ADC_TemperatureSensor]);
    }
    //ADC_Start();
    ADC_ctr++;
  }
}

void ADC_Handler_10ms(void)
{
  static uint8_t ctr = 0;
  static char uart2Buffer[32];
  int8_t U32_to_HexString(char *string, int len, uint32_t val, char leadingChar);
  (void)U32_to_HexString(uart2Buffer, 2, ctr, '0');
  uart2Buffer[2] = ' ';
  (void)U32_to_HexString(uart2Buffer +  3, 3, ADC_values[ADC_IN0], ' ');
  uart2Buffer[6] = ' ';
  (void)U32_to_HexString(uart2Buffer +  7, 3, ADC_values[ADC_IN1], ' ');
  uart2Buffer[10] = ' ';
  (void)U32_to_HexString(uart2Buffer + 11, 3, ADC_values[ADC_IN8], ' ');
  uart2Buffer[14] = ' ';
  (void)U32_to_HexString(uart2Buffer + 15, 3, ADC_values[ADC_IN9], ' ');
  uart2Buffer[18] = ' ';
  (void)U32_to_HexString(uart2Buffer + 19, 3, ADC_values[ADC_Vref], ' ');
  uart2Buffer[22] = ' ';
  (void)U32_to_HexString(uart2Buffer + 23, 3, ADC_values[ADC_TemperatureSensor], ' ');
  uart2Buffer[26] = ' ';
  memset(uart2Buffer + 27, ' ', sizeof(uart2Buffer) - 27 - 1);
  uart2Buffer[sizeof(uart2Buffer) - 1] = '\r';
  UART2_TX((uint8_t*)uart2Buffer, sizeof(uart2Buffer));
  ctr++;
}
