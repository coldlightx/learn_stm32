#include <stm32f10x.h>
#include "hardware/OLED.h"
#include "system/Delay.h"

int main(int argc, char const *argv[])
{
    OLED_Init();

    OLED_ShowString(1, 1, "SYSCLK:");
    OLED_ShowNum(1, 8, SystemCoreClock, 8);

    while (1) {
        OLED_ShowString(2, 1, "RUNNING");
        Delay_ms(500);
        OLED_ShowString(2, 1, "       ");
        Delay_ms(500);
    }

    return 0;
}
