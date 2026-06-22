#include <stm32f10x.h>
#include "system/Delay.h"
#include "hardware/OLED.h"
#include "system/RTC.h"

int main(int argc, char const *argv[])
{
    OLED_Init();
    MyRTC_init();
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    PWR_WakeUpPinCmd(ENABLE);

    OLED_ShowString(1, 1, "CNT:");
    OLED_ShowString(2, 1, "ALR:");
    OLED_ShowString(3, 1, "ALRF:");

    time_t alarm = RTC_GetCounter() + 10;
    MyRTC_set_alarm(alarm);

    while (1) {
        OLED_ShowNum(1, 5, RTC_GetCounter(), 10);
        OLED_ShowNum(2, 5, alarm, 10);
        OLED_ShowNum(3, 6, RTC_GetFlagStatus(RTC_FLAG_ALR), 1);

        OLED_ShowString(4, 1, "RUNNING");
        Delay_ms(200);
        OLED_ShowString(4, 1, "       ");
        Delay_ms(200);

        OLED_ShowString(4, 10, "STANDBY");
        Delay_ms(1000);

        OLED_Clear();

        PWR_EnterSTANDBYMode();
    }

    return 0;
}
