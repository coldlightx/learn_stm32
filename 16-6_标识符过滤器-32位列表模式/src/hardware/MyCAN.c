#include "stm32f10x.h"

#define RCC_GPIO        RCC_APB2Periph_GPIOA
#define RCC_CAN         RCC_APB1Periph_CAN1

#define CAN_GPIO        GPIOA
#define RX_PIN          GPIO_Pin_11
#define TX_PIN          GPIO_Pin_12

#define CAN             CAN1

#define CAN_FILTER_FIFO CAN_Filter_FIFO0
#define CAN_FIFO        CAN_FIFO0

void MyCAN_init()
{
    RCC_APB2PeriphClockCmd(RCC_GPIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_CAN, ENABLE);

    GPIO_InitTypeDef gpio_config;

    gpio_config.GPIO_Pin   = RX_PIN;
    gpio_config.GPIO_Mode  = GPIO_Mode_IPU;
    gpio_config.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(CAN_GPIO, &gpio_config);

    gpio_config.GPIO_Pin   = TX_PIN;
    gpio_config.GPIO_Mode  = GPIO_Mode_AF_PP;
    gpio_config.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(CAN_GPIO, &gpio_config);

    CAN_InitTypeDef can_config;
    can_config.CAN_Mode = CAN_Mode_LoopBack;
    // 36MHZ / 6 / (1 + 2 + 3) = 1MHZ
    can_config.CAN_Prescaler = 6;
    can_config.CAN_BS1       = CAN_BS1_2tq;
    can_config.CAN_BS2       = CAN_BS2_3tq;
    can_config.CAN_SJW       = CAN_SJW_2tq;
    can_config.CAN_NART      = DISABLE;
    can_config.CAN_AWUM      = DISABLE;
    can_config.CAN_TXFP      = DISABLE;
    can_config.CAN_RFLM      = ENABLE;
    can_config.CAN_ABOM      = ENABLE;
    can_config.CAN_TTCM      = DISABLE;

    CAN_Init(CAN, &can_config);

    CAN_FilterInitTypeDef can_filter_config;

    can_filter_config.CAN_FilterNumber = 0;
    can_filter_config.CAN_FilterMode   = CAN_FilterMode_IdList;
    can_filter_config.CAN_FilterScale  = CAN_FilterScale_32bit;

    can_filter_config.CAN_FilterIdHigh     = 0x0123 << 5;
    can_filter_config.CAN_FilterIdLow      = 0x0000;
    uint32_t reg_value                     = 0x12345678 << 3 | 0x04;
    can_filter_config.CAN_FilterMaskIdHigh = reg_value >> 16;
    can_filter_config.CAN_FilterMaskIdLow  = reg_value;

    can_filter_config.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
    can_filter_config.CAN_FilterActivation     = ENABLE;
    CAN_FilterInit(&can_filter_config);
}

void MyCAN_send_message(uint32_t id, uint8_t length, uint8_t *data)
{
    CanTxMsg tx_message;

    tx_message.StdId = id;
    tx_message.ExtId = 0;
    tx_message.IDE   = CAN_Id_Standard;
    tx_message.RTR   = CAN_RTR_Data;
    tx_message.DLC   = length;

    for (uint8_t i = 0; i < length; i++) {
        tx_message.Data[i] = data[i];
    }

    uint8_t mailbox = CAN_Transmit(CAN, &tx_message);

    int counter = 100000;
    while (CAN_TransmitStatus(CAN, mailbox) != CAN_TxStatus_Ok && counter-- > 0);
}

void MyCAN_send_tx_message(CanTxMsg *tx_message)
{
    uint8_t mailbox = CAN_Transmit(CAN, tx_message);

    int counter = 100000;
    while (CAN_TransmitStatus(CAN, mailbox) != CAN_TxStatus_Ok && counter-- > 0);
}

uint8_t MyCAN_message_count()
{
    return CAN_MessagePending(CAN, CAN_FIFO);
}

void MyCAN_receive_message(uint32_t *id, uint8_t *length, uint8_t *data)
{
    CanRxMsg rx_message;
    CAN_Receive(CAN, CAN_FIFO, &rx_message);

    if (rx_message.IDE == CAN_Id_Standard) {
        *id = rx_message.StdId;
    } else {
        *id = rx_message.ExtId;
    }

    if (rx_message.RTR == CAN_RTR_Data) {
        *length = rx_message.DLC;

        for (int i = 0; i < *length; i++) {
            data[i] = rx_message.Data[i];
        }
    }
}

void MyCAN_receive_rx_message(CanRxMsg *rx_message)
{
    CAN_Receive(CAN, CAN_FIFO, rx_message);
}