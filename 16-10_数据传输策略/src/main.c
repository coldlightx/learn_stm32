#include <stm32f10x.h>
#include "hardware/OLED.h"
#include "hardware/button.h"
#include "hardware/MyCAN.h"
#include "system/Timer.h"
#include "system/Delay.h"

typedef enum {
    SENDER_MODE,
    RECEIVER_MODE
} Mode;

Mode RUNNING_MODE = SENDER_MODE;

uint8_t Time_triggered    = 0;
uint8_t Request_triggered = 0;
uint8_t Event_triggered   = 0;

CanTxMsg time_message = {
    .StdId = 0x100,
    .ExtId = 0,
    .IDE   = CAN_Id_Standard,
    .RTR   = CAN_RTR_Data,
    .DLC   = 4,
    .Data  = {0x11, 0x22, 0x33, 0x55},
};

CanTxMsg event_message = {
    .StdId = 0x200,
    .ExtId = 0,
    .IDE   = CAN_Id_Standard,
    .RTR   = CAN_RTR_Data,
    .DLC   = 4,
    .Data  = {0x12, 0x34, 0x56, 0x78},
};

CanTxMsg request_message = {
    .StdId = 0x300,
    .ExtId = 0,
    .IDE   = CAN_Id_Standard,
    .RTR   = CAN_RTR_Data,
    .DLC   = 4,
    .Data  = {0x13, 0x24, 0x57, 0x68},
};

CanTxMsg request_data_message = {
    .StdId = 0x300,
    .ExtId = 0,
    .IDE   = CAN_Id_Standard,
    .RTR   = CAN_RTR_Remote,
    .DLC   = 0,
    .Data  = {},
};

CanTxMsg request_data_message2 = {
    .StdId = 0x3FF,
    .ExtId = 0,
    .IDE   = CAN_Id_Standard,
    .RTR   = CAN_RTR_Data,
    .DLC   = 0,
    .Data  = {},
};

void receive_can_message()
{
    if (!MyCAN_has_data())
        return;

    CanRxMsg message;
    message = MyCAN_get_message();

    if (message.IDE != CAN_Id_Standard)
        return;

    // process standard message

    if ((message.RTR == CAN_RTR_Remote && message.StdId == 0x300) ||
        (message.RTR == CAN_RTR_Data && message.StdId == 0x3FF)) {
        Request_triggered = 1;
        OLED_ShowString(1, 8, "RECV REQ");
        Delay_ms(500);
        OLED_ShowString(1, 8, "        ");
        return;
    }

    if (message.RTR == CAN_RTR_Data) {
        OLED_ShowString(1, 1, "Rx:");

        switch (message.StdId) {
            case 0x100:
                OLED_ShowHexNum(1, 4, message.StdId, 3);
                OLED_ShowHexArray(2, 6, message.Data, message.DLC);
                break;

            case 0x200:
                OLED_ShowHexNum(1, 4, message.StdId, 3);
                OLED_ShowHexArray(3, 6, message.Data, message.DLC);
                break;

            case 0x300:
                OLED_ShowHexNum(1, 4, message.StdId, 3);
                OLED_ShowHexArray(4, 6, message.Data, message.DLC);
                break;
            default:
                break;
        }
    }
}

void send_and_display(CanTxMsg *message, uint8_t display_row, uint8_t increment)
{
    MyCAN_send_tx_message(message);
    OLED_ShowString(1, 1, "Tx:");
    OLED_ShowHexNum(1, 4, message->StdId, 3);
    OLED_ShowHexArray(display_row, 6, message->Data, message->DLC);

    if (increment) {
        for (uint8_t i = 0; i < message->DLC; i++) {
            message->Data[i]++;
        }
    }
}
void send_can_message()
{
    if (Time_triggered) {
        send_and_display(&time_message, 2, 1);
        Time_triggered = 0;
    }

    if (Event_triggered) {
        send_and_display(&event_message, 3, 1);
        Event_triggered = 0;
    }
    if (Request_triggered) {

        Request_triggered = 0;
        send_and_display(&request_message, 4, 1);
    }
}

int main(int argc, char const *argv[])
{
    OLED_Init();
    button_init();
    MyCAN_init();

    if (RUNNING_MODE == SENDER_MODE) {
        timer_init();
    }

    OLED_ShowString(2, 1, "Tim:");
    OLED_ShowString(3, 1, "Trig:");
    OLED_ShowString(4, 1, "Req:");

    while (1) {

        uint8_t key_num = button_get_key();
        receive_can_message();

        if (RUNNING_MODE == SENDER_MODE) {
            if (key_num == 1)
                Event_triggered = 1;
            send_can_message();
        } else if (RUNNING_MODE == RECEIVER_MODE) {
            if (key_num == 1) {
                MyCAN_send_tx_message(&request_data_message);
                OLED_ShowString(1, 8, "SEND REQ");
                Delay_ms(500);
            } else if (key_num == 2) {
                MyCAN_send_tx_message(&request_data_message2);
                OLED_ShowString(1, 8, "SEND REQ");
                Delay_ms(500);
            }
            OLED_ShowString(1, 8, "        ");
        }
    }

    return 0;
}

void TIM2_IRQHandler()
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) {
        Time_triggered = 1;
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}