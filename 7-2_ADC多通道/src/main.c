#include <stm32f10x.h>
#include "hardware/OLED.h"
#include "hardware/ADC.h"

int main(int argc, char const *argv[])
{
    OLED_Init();
    ADC_init();

    OLED_ShowString(1, 1, "AD0:0000");
    OLED_ShowString(2, 1, "AD1:0000");
    OLED_ShowString(3, 1, "AD2:0000");
    OLED_ShowString(4, 1, "AD3:0000");

    ADC_DATA adc_data;

    while (1) {
        ADC_get_value(&adc_data);
        OLED_ShowNum(1, 5, adc_data.channel0, 4);
        OLED_ShowNum(2, 5, adc_data.channel1, 4);
        OLED_ShowNum(3, 5, adc_data.channel2, 4);
        OLED_ShowNum(4, 5, adc_data.channel3, 4);
    }

    return 0;
}
