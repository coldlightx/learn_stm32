#include <stm32f10x.h>
#include <stdio.h>
#include "system/Delay.h"
#include "hardware/OLED.h"
#include "hardware/serial.h"

int main(int argc, char const *argv[])
{

    uint8_t data;
    OLED_Init();
    serial_init();
    while (1) {

        if (serial_get_rx_flag()) {
            data = serial_get_data();
            OLED_ShowString(1, 1, "RxData:");
            OLED_ShowHexNum(1, 8, data, 2);
            serial_printf("%X\n", data);
            // printf必须要加\n才能输出
            printf("0x%X\n", data);
        }
    }

    return 0;
}
