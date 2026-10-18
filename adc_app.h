#ifndef _ADC_APP_H_
#define _ADC_APP_H_

enum e_ADC_values
{
  ADC_IN0,
  ADC_IN1,
  ADC_IN6,
  ADC_IN7,
  ADC_TemperatureSensor,
  ADC_Vref,
  ADC_Ch_Num /* this must be the last one */
};
extern uint16_t ADC_values_raw[ADC_Ch_Num];
#define ADC_VALUES_NUM (sizeof(ADC_values_raw)/sizeof(ADC_values_raw[0]))
#define TEMP_SCALE 10
//extern int8_t Temperature = -128;
extern int16_t Temperature;
extern int16_t Temperature_raw;

void ADC_HandlerInit(void);
void ADC_Handler(void);
void ADC_Handler_10ms(void);

#endif /* _ADC_APP_H_ */
