#include <stdio.h>
#include <stdarg.h>
#include "stm32f10x.h"
#include "serial.h"

uint8_t rx_data[256];
uint8_t rx_flag;
uint8_t rx_data_index = 0;

void serial_init()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef gpio_config;
    gpio_config.GPIO_Mode  = GPIO_Mode_AF_PP;
    gpio_config.GPIO_Pin   = GPIO_Pin_9;
    gpio_config.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpio_config);

    gpio_config.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    gpio_config.GPIO_Pin   = GPIO_Pin_10;
    gpio_config.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpio_config);

    USART_InitTypeDef usart_config;
    usart_config.USART_BaudRate            = 115200;
    usart_config.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    usart_config.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;
    usart_config.USART_Parity              = USART_Parity_No;
    usart_config.USART_StopBits            = USART_StopBits_1;
    usart_config.USART_WordLength          = USART_WordLength_8b;
    USART_Init(USART1, &usart_config);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    NVIC_InitTypeDef nvic_config;
    nvic_config.NVIC_IRQChannel                   = USART1_IRQn;
    nvic_config.NVIC_IRQChannelCmd                = ENABLE;
    nvic_config.NVIC_IRQChannelPreemptionPriority = 1;
    nvic_config.NVIC_IRQChannelSubPriority        = 1;
    NVIC_Init(&nvic_config);

    USART_Cmd(USART1, ENABLE);
}

void serial_send_byte(uint8_t byte)
{
    USART_SendData(USART1, byte);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) != SET);
}

void serial_send_array(uint8_t *arr, unsigned int length)
{
    for (unsigned int i = 0; i < length; i++) {
        serial_send_byte(arr[i]);
    }
}

void serial_send_string(char *str)
{
    for (int i = 0; str[i] != '\0'; i++) {
        serial_send_byte(str[i]);
    }
}

void serial_send_number(int num)
{
    char s[20];
    sprintf(s, "%d", num);
    serial_send_string(s);
}

int _write(int file, char *ptr, int len)
{
    for (int i = 0; i < len; i++)
        serial_send_byte(ptr[i]);

    return len;
}

void serial_printf(char *format, ...)
{
    va_list args;
    va_start(args, format);
    char s[200];
    vsprintf(s, format, args);
    va_end(args);
    serial_send_string(s);
}

void USART1_IRQHandler()
{
    static SerialStatus status = WATING_FOR_START;
    uint8_t data;

    if (USART_GetITStatus(USART1, USART_IT_RXNE)) {
        data = USART_ReceiveData(USART1);
        switch (status) {
            case WATING_FOR_START:
                // 接收到开始标志位并且rx_flag为0(rx_data已被读取)
                if (data == '@' && rx_flag == 0) {
                    status        = RECEIVING_DATA;
                    rx_data_index = 0;
                }
                break;

            case RECEIVING_DATA:
                if (data == '\r') {
                    status                 = RECEIVE_COMPLETE;
                    rx_flag                = 1;
                    rx_data[rx_data_index] = '\0';
                    break;
                }
                rx_data[rx_data_index++] = data;
                break;

            case RECEIVE_COMPLETE:
                if (data == '\n') {
                    status = WATING_FOR_START;
                }
                break;
            default:
                break;
        }

        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}

uint8_t serial_get_rx_flag()
{
    return rx_flag;
}

void serial_get_rx_data(uint8_t *arr)
{
    for (int i = 0; i < 4; i++)
        arr[i] = rx_data[i];
    serial_clear_rx_flag();
}

void serial_clear_rx_flag()
{
    rx_flag = 0;
}

void serial_send_hex_data(uint8_t *data)
{
    serial_send_byte(0xFF);
    serial_send_array(data, 4);
    serial_send_byte(0xFE);
}
