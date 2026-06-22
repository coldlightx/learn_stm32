#include <stm32f10x.h>
#include "system/Delay.h"
#include "hardware/OLED.h"
#include "hardware/IR_sensor_counter.h"

int main(int argc, char const *argv[])
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    OLED_Init();
    IR_sensor_counter_init();

    OLED_ShowString(1, 1, "Count:");
    while (1) {
        OLED_ShowNum(1, 7, IR_get_sensor_count(), 5);

        OLED_ShowString(2, 1, "RUNNING");
        Delay_ms(100);
        OLED_ShowString(2, 1, "       ");
        Delay_ms(100);

        PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFI);
        SystemInit();
    }

    return 0;
}
