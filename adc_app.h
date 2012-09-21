#ifndef _ADC_APP_H_
#define _ADC_APP_H_

enum e_ADC_values
{
  ADC_IN4_DAC,
  ADC_TemperatureSensor,
  ADC_Vref,
  ADC_VBat,
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
