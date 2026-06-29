#include <stm32f10x.h>
#include "system/Delay.h"
#include "hardware/OLED.h"
#include "hardware/button.h"

int main(int argc, char const *argv[])
{
    OLED_Init();
    button_init();

    OLED_ShowString(1, 1, "WWDG TEST");
    if (RCC_GetFlagStatus(RCC_FLAG_WWDGRST) == SET) {
        OLED_ShowString(2, 1, "WWDGRST");
        Delay_ms(500);
        OLED_ShowString(2, 1, "       ");
        RCC_ClearFlag();
    } else {
        OLED_ShowString(3, 1, "SYSRST");
        Delay_ms(500);
        OLED_ShowString(3, 1, "      ");
    }

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);

    // 50ms
    // 1/36000 * 4096 * 8 * (T + 1) = 50
    // T = 54

    // window time
    // 30ms
    // 1/36000 * 4096 * 8 * (T - W) = 30
    // T = 54
    // W = 21
    WWDG_SetPrescaler(WWDG_Prescaler_8);
    WWDG_SetWindowValue(0X40 | 21); // 30ms
    WWDG_Enable(0X40 | 54);         // 50ms

    while (1) {
        button_get_key();
        OLED_ShowString(4, 13, "FEED");
        Delay_ms(20);
        OLED_ShowString(4, 13, "    ");
        Delay_ms(10);
        WWDG_SetCounter(0X40 | 54);
    }

    return 0;
}