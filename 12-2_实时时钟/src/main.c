#include <stm32f10x.h>
#include "hardware/OLED.h"
#include "system/RTC.h"

int main(int argc, char const *argv[])
{
    OLED_Init();
    MyRTC_init();

    OLED_ShowString(1, 1, "Date:xxxx-xx-xx");
    OLED_ShowString(2, 1, "Time:  xx:xx:xx");
    OLED_ShowString(3, 1, "CNT: ");
    OLED_ShowString(4, 1, "DIV: ");

    Time time_data;

    // time_data.tm_year = 2026;
    // time_data.tm_mon  = 6;
    // time_data.tm_mday = 7;
    // time_data.tm_hour = 23;
    // time_data.tm_min  = 59;
    // time_data.tm_sec  = 55;

    // MyRTC_set_time(&time_data);

    while (1) {
        MyRTC_get_time(&time_data);
        OLED_ShowNum(1, 6, time_data.tm_year, 4);
        OLED_ShowNum(1, 11, time_data.tm_mon, 2);
        OLED_ShowNum(1, 14, time_data.tm_mday, 2);
        OLED_ShowNum(2, 8, time_data.tm_hour, 2);
        OLED_ShowNum(2, 11, time_data.tm_min, 2);
        OLED_ShowNum(2, 14, time_data.tm_sec, 2);
        OLED_ShowNum(3, 6, RTC_GetCounter(), 10);
        OLED_ShowNum(4, 6, RTC_GetDivider(), 10);
    }

    return 0;
}
