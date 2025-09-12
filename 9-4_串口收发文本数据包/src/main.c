#include <stm32f10x.h>
#include <stdio.h>
#include <string.h>
#include "system/Delay.h"
#include "hardware/OLED.h"
#include "hardware/serial.h"
#include "hardware/led.h"

void process_command(char *command)
{
    OLED_ShowString(2, 1, "                ");
    OLED_ShowString(4, 1, "                ");
    if (strcmp(command, "LED_ON") == 0) {
        led1_on();
        OLED_ShowString(4, 1, "LED_ON");
        OLED_ShowString(2, 1, "LED_ON_OK");
        serial_send_string("LED_ON_OK");
    } else if (strcmp(command, "LED_OFF") == 0) {
        led1_off();
        OLED_ShowString(4, 1, "LED_OFF");
        OLED_ShowString(2, 1, "LED_OFF_OK");
        serial_send_string("LED_OFF_OK");
    } else if (strcmp(command, "LED_TOGGLE") == 0) {
        led1_toggle();
        OLED_ShowString(4, 1, "LED_TOGGLE");
        OLED_ShowString(2, 1, "LED_TOGGLE_OK");
        serial_send_string("LED_TOGGLE_OK");
    } else {
        OLED_ShowString(2, 1, "ERR_COMMAND");
        serial_send_string("ERR_COMMAND");
    }
}

int main(int argc, char const *argv[])
{
    OLED_Init();
    serial_init();
    led_init();

    OLED_ShowString(1, 1, "TxData:");
    OLED_ShowString(3, 1, "RxCommand:");

    while (1) {
        if (serial_get_rx_flag()) {
            process_command((char *)rx_data);
            serial_clear_rx_flag();
        }
    }

    return 0;
}
