#include <stm32f10x.h>
#include "hardware/OLED.h"
#include "hardware/ADC.h"

int main(int argc, char const *argv[])
{
    float voltage = 0;
    uint16_t adc_value;

    OLED_Init();
    ADC_init();

    OLED_ShowString(1, 1, "ADCValue:00000");
    OLED_ShowString(2, 1, "Voltage:0.00V");

    while (1) {
        adc_value = ADC_get_value();
        voltage   = adc_value * 3.3 / 4095;
        OLED_ShowNum(1, 10, ADC_get_value(), 5);
        OLED_ShowNum(2, 9, voltage, 1);
        OLED_ShowNum(2, 11, ((int)(voltage * 100)) % 100, 2);
    }

    return 0;
}
