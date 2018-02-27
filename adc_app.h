#ifndef _ADC_APP_H_
#define _ADC_APP_H_

enum e_ADC_values
{
#if (CPU_TYPE == CPU_TYPE_STM32F0)
  ADC_IN4_DAC,
  ADC_IN5_Ub,
  ADC_IN6_Ib,
  ADC_TemperatureSensor,
  ADC_Vref,
  ADC_VBat,
#elif (CPU_TYPE == CPU_TYPE_STM32F1)
  ADC_IN0,
  ADC_IN1,
  ADC_IN8,
  ADC_IN9,
  ADC_TemperatureSensor,
  ADC_Vref,
#endif
  ADC_Ch_Num /* this must be the last one */
};
extern uint16_t ADC_values[ADC_Ch_Num];
#define ADC_VALUES_NUM (sizeof(ADC_values)/sizeof(ADC_values[0]))
#define TEMP_SCALE 10
//extern int8_t Temperature = -128;
extern int16_t Temperature;
extern int16_t Temperature_raw;

void ADC_HandlerInit(void);
void ADC_Handler(void);

#endif /* _ADC_APP_H_ */
