#include <stm32f10x.h>
#include "hardware/OLED.h"
#include "hardware/button.h"
#include "hardware/MyCAN.h"

uint32_t TX_ID     = 0x188;
uint8_t TX_LENGTH  = 8;
uint8_t tx_data[8] = {0x12, 0x34, 0x56, 0x78};

int main(int argc, char const *argv[])
{
    OLED_Init();
    button_init();
    MyCAN_init();

    OLED_ShowString(1, 1, "TXID:");
    OLED_ShowString(2, 1, "RXID:");
    OLED_ShowString(3, 1, "DLC:");
    OLED_ShowString(4, 1, "Data:");

    OLED_ShowHexNum(1, 6, TX_ID, 3);

    while (1) {
        if (button_get_key() == 1) {
            tx_data[0]++;
            tx_data[1] += 2;
            tx_data[2] += 3;
            tx_data[3] += 4;
            MyCAN_send_message(TX_ID, TX_LENGTH, tx_data);
        }

        if (MyCAN_message_count()) {
            uint32_t id;
            uint8_t length;
            uint8_t rx_data[8];
            MyCAN_receive_message(&id, &length, rx_data);
            OLED_ShowHexNum(2, 6, id, 3);
            OLED_ShowNum(3, 5, length, 1);
            OLED_ShowHexArray(4, 6, rx_data, 4);
        }
    }

    return 0;
}
