#ifndef _ADC_APP_H_
#define _ADC_APP_H_

enum e_ADC_values
{
#if TARGET_ECU == TARGET_ECU_STM32F0DISCOVERY
  ADC_IN4_DAC,
  ADC_IN5_Ub,
  ADC_IN6_Ib,
  ADC_TemperatureSensor,
  ADC_Vref,
  ADC_VBat,
#elif (CPU_TYPE == CPU_TYPE_STM32F1)
  ADC_PA0,
  ADC_PA1,
  ADC_PB0,
  ADC_PB1,
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
