#include <stm32f10x.h>
#include <stdio.h>
#include "system/Delay.h"
#include "hardware/OLED.h"
#include "hardware/serial.h"
#include "hardware/button.h"

int main(int argc, char const *argv[])
{
    uint8_t rx_data[4];
    uint8_t tx_data[4] = {1, 2, 3, 4};
    OLED_Init();
    serial_init();
    button_init();

    OLED_ShowString(1, 1, "TxData:");
    OLED_ShowString(3, 1, "RxData:");

    while (1) {
        if (button_get_key() == 1) {
            serial_send_hex_data(tx_data);
            OLED_ShowHexArray(2, 1, tx_data, 4);
            for (int i = 0; i < 4; i++)
                tx_data[i]++;
        }
        if (serial_get_rx_flag()) {
            serial_get_rx_data(rx_data);
            OLED_ShowHexArray(4, 1, rx_data, 4);
            serial_send_array(rx_data, 4);
        }
    }

    return 0;
}
