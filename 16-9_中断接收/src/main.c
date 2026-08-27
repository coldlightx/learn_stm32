#include <stm32f10x.h>
#include "hardware/OLED.h"
#include "hardware/button.h"
#include "hardware/MyCAN.h"

CanTxMsg tx_message_arr[] = {
    // StdID   ExtID         IDE              RTR       DLC    DATA[8]
    {0x555, 0x00000000, CAN_Id_Standard, CAN_RTR_Data, 4, {0x11, 0x22, 0x33, 0x44}},
    {0x000, 0x12345678, CAN_Id_Extended, CAN_RTR_Data, 4, {0xAA, 0xBB, 0xCC, 0xDD}},
    {0x666, 0x00000000, CAN_Id_Standard, CAN_RTR_Remote, 0, {0}},
    {0x000, 0x12312345, CAN_Id_Extended, CAN_RTR_Remote, 0, {0}}};

void receive_can_message()
{
    CanRxMsg rx_message;
    rx_message = MyCAN_get_message();

    if (rx_message.IDE == CAN_Id_Standard) {
        OLED_ShowString(1, 7, "Std");
        OLED_ShowHexNum(2, 6, rx_message.StdId, 8);

    } else if (rx_message.IDE == CAN_Id_Extended) {
        OLED_ShowString(1, 7, "Ext");
        OLED_ShowHexNum(2, 6, rx_message.ExtId, 8);
    }

    OLED_ShowNum(3, 5, rx_message.DLC, 1);

    if (rx_message.RTR == CAN_RTR_Data) {
        OLED_ShowString(1, 11, "Data  ");
        OLED_ShowHexArray(4, 6, rx_message.Data, rx_message.DLC);
    } else if (rx_message.RTR == CAN_RTR_Remote) {
        OLED_ShowString(1, 11, "Remote");
        OLED_ShowString(4, 6, "           ");
    }
}

int main(int argc, char const *argv[])
{
    OLED_Init();
    button_init();
    MyCAN_init();

    OLED_ShowString(1, 1, "Type:");
    OLED_ShowString(2, 1, "RXID:");
    OLED_ShowString(3, 1, "DLC:");
    OLED_ShowString(4, 1, "Data:");

    uint8_t tx_offset = 0;
    while (1) {
        if (button_get_key() == 1) {
            MyCAN_send_tx_message(tx_message_arr + tx_offset++);

            if (tx_offset >= sizeof(tx_message_arr) / sizeof(CanTxMsg))
                tx_offset = 0;
        }

        if (MyCAN_has_data()) {
            receive_can_message();
        }
    }

    return 0;
}