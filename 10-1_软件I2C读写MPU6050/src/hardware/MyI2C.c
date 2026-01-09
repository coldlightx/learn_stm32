#include "stm32f10x.h"
#include "system/Delay.h"

#define RCC_PORT RCC_APB2Periph_GPIOB
#define PORT     GPIOB
#define SCL      GPIO_Pin_0
#define SDA      GPIO_Pin_1

void MyI2C_init()
{
    RCC_APB2PeriphClockCmd(RCC_PORT, ENABLE);
    GPIO_InitTypeDef gpio_config;
    gpio_config.GPIO_Mode  = GPIO_Mode_Out_OD;
    gpio_config.GPIO_Pin   = SCL | SDA;
    gpio_config.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PORT, &gpio_config);
    GPIO_SetBits(PORT, SCL | SDA);
}

void MyI2C_sda_write(uint8_t value)
{
    GPIO_WriteBit(PORT, SDA, (BitAction)value);
    Delay_us(10);
}

uint8_t MyI2C_sda_read()
{
    return GPIO_ReadInputDataBit(PORT, SDA);
}

void MyI2C_scl_write(uint8_t value)
{
    GPIO_WriteBit(PORT, SCL, (BitAction)value);
    Delay_us(10);
}

void MyI2C_start()
{
    // 适配重新开始
    MyI2C_sda_write(1);
    MyI2C_scl_write(1);

    MyI2C_sda_write(0);
    MyI2C_scl_write(0);
}

void MyI2C_stop()
{
    MyI2C_scl_write(1);
    MyI2C_sda_write(1);
}

uint8_t MyI2C_receive_ack()
{
    uint8_t ack;
    MyI2C_sda_write(1);
    MyI2C_scl_write(1);
    ack = MyI2C_sda_read();
    MyI2C_scl_write(0);

    return ack;
}

void MyI2C_send_ack(uint8_t ack)
{
    MyI2C_sda_write(ack);
    MyI2C_scl_write(1);
    MyI2C_scl_write(0);
}

uint8_t MyI2C_send_byte(uint8_t byte)
{
    for (int i = 0; i < 8; i++) {
        MyI2C_sda_write(byte & (0x80 >> i));
        MyI2C_scl_write(1);
        MyI2C_scl_write(0);
    }

    return MyI2C_receive_ack();
}

uint8_t MyI2C_receive_byte(uint8_t need_continue)
{
    uint8_t data = 0;
    uint8_t ack  = !need_continue;

    // 释放SDA
    MyI2C_sda_write(1);

    for (int i = 0; i < 8; i++) {
        MyI2C_scl_write(1);
        if (MyI2C_sda_read())
            data |= 0x80 >> i;
        MyI2C_scl_write(0);
    }
    MyI2C_send_ack(ack);
    return data;
}

void MyI2C_write_data_arr(uint8_t device_addr, uint8_t reg_addr_start, uint8_t *arr, uint8_t length)
{
    MyI2C_start();
    MyI2C_send_byte(device_addr);
    MyI2C_send_byte(reg_addr_start);

    for (int i = 0; i < length; i++)
        MyI2C_send_byte(arr[i]);

    MyI2C_stop();
}

void MyI2C_write_data_byte(uint8_t device_addr, uint8_t reg_addr, uint8_t data)
{
    MyI2C_write_data_arr(device_addr, reg_addr, &data, 1);
}

void MyI2C_read_data_arr(uint8_t device_addr, uint8_t reg_addr_start, uint8_t *arr, uint8_t length)
{
    MyI2C_start();
    MyI2C_send_byte(device_addr);
    MyI2C_send_byte(reg_addr_start);
    MyI2C_start();
    MyI2C_send_byte(device_addr | 0x01);

    // 开始接收
    uint8_t need_continue;
    for (int i = 0; i < length; i++) {
        need_continue = i < length - 1;
        arr[i]        = MyI2C_receive_byte(need_continue);
    }
    MyI2C_stop();
}

uint8_t MyI2C_read_data_byte(uint8_t device_addr, uint8_t reg_addr)
{
    uint8_t data;
    MyI2C_read_data_arr(device_addr, reg_addr, &data, 1);
    return data;
}