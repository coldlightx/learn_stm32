#if !defined(__ADC_H)
#define __ADC_H

typedef struct {
    uint16_t channel0;
    uint16_t channel1;
    uint16_t channel2;
    uint16_t channel3;
} ADC_DATA;

extern ADC_DATA adc_data;
extern uint16_t AD_DATA[4];

void ADC_init();
void ADC_get_value(ADC_DATA *adc_data);

#endif // __ADC_H
