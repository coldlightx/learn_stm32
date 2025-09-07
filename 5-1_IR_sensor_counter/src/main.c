#include <stm32f10x.h>
#include "hardware/OLED.h"
#include "hardware/IR_sensor_counter.h"

int main(int argc, char const *argv[])
{
    OLED_Init();
    IR_sensor_counter_init();

    OLED_ShowString(1, 1, "Count:");
    while (1) {
        OLED_ShowNum(1, 7, IR_get_sensor_count(), 5);
    }

    return 0;
}
