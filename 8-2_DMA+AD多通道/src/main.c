#include <stm32f10x.h>
#include "system/Delay.h"
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

    while (1) {
        OLED_ShowNum(1, 5, AD_DATA[0], 4);
        OLED_ShowNum(2, 5, AD_DATA[1], 4);
        OLED_ShowNum(3, 5, AD_DATA[2], 4);
        OLED_ShowNum(4, 5, AD_DATA[3], 4);
    }

    return 0;
}
