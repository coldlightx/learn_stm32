#include <stm32f10x.h>
#include "system/Delay.h"
#include "hardware/OLED.h"
#include "hardware/button.h"

int main(int argc, char const *argv[])
{
    OLED_Init();
    button_init();
    OLED_Clear();

    OLED_ShowString(1, 1, "IWDG TEST");
    if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) == SET) {
        OLED_ShowString(2, 1, "IWDGRST");
        Delay_ms(500);
        OLED_ShowString(2, 1, "       ");
        RCC_ClearFlag();
    } else {
        OLED_ShowString(3, 1, "SYSRST");
        Delay_ms(500);
        OLED_ShowString(3, 1, "      ");
    }

    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    IWDG_SetPrescaler(IWDG_Prescaler_16);
    // 1000ms: 1000/16/(1/40) - 1
    IWDG_SetReload(2499);

    IWDG_ReloadCounter();
    IWDG_Enable();

    while (1) {
        button_get_key();
        IWDG_ReloadCounter();
        OLED_ShowString(4, 13, "FEED");
        Delay_ms(300);
        OLED_ShowString(4, 13, "    ");
        Delay_ms(500);
    }

    return 0;
}