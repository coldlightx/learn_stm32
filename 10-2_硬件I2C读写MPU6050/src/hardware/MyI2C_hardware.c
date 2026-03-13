#include "stm32f10x.h"
#include "system/Delay.h"

#define RCC_PERIPH_I2C  RCC_APB1Periph_I2C1
#define RCC_PERIPH_GPIO RCC_APB2Periph_GPIOB
#define I2C_PERIPH      I2C1
#define I2C_GPIO_PIN    GPIO_Pin_6 | GPIO_Pin_7

void MyI2C_init()
{
    // RCC_APB2PeriphClockCmd(RCC_PORT, ENABLE);
    // GPIO_InitTypeDef gpio_config;
    // gpio_config.GPIO_Mode  = GPIO_Mode_Out_OD;
    // gpio_config.GPIO_Pin   = SCL | SDA;
    // gpio_config.GPIO_Speed = GPIO_Speed_50MHz;
    // GPIO_Init(PORT, &gpio_config);
    // GPIO_SetBits(PORT, SCL | SDA);
    RCC_APB1PeriphClockCmd(RCC_PERIPH_I2C, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_PERIPH_GPIO, ENABLE);
    GPIO_InitTypeDef gpio_config;
    gpio_config.GPIO_Mode  = GPIO_Mode_AF_OD;
    gpio_config.GPIO_Pin   = I2C_GPIO_PIN;
    gpio_config.GPIO_Speed = GPIO_Speed_50MHz;

    I2C_InitTypeDef i2c_config;

    i2c_config.I2C_Ack                 = I2C_Ack_Enable;
    i2c_config.I2C_ClockSpeed          = 100 000;
    i2c_config.I2C_DutyCycle           = I2C_DutyCycle_2;
    i2c_config.I2C_Mode                = I2C_Mode_I2C;
    i2c_config.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    i2c_config.I2C_OwnAddress1         = 0x10;
    I2C_Init(I2C_PERIPH, &i2c_config);
    I2C_Cmd(I2C_PERIPH, ENABLE);
}

// void MyI2C_sda_write(uint8_t value)
// {
//     GPIO_WriteBit(PORT, SDA, (BitAction)value);
//     Delay_us(10);
// }

// uint8_t MyI2C_sda_read()
// {
//     return GPIO_ReadInputDataBit(PORT, SDA);
// }

// void MyI2C_scl_write(uint8_t value)
// {
//     GPIO_WriteBit(PORT, SCL, (BitAction)value);
//     Delay_us(10);
// }

int MyI2C_wait_event(uint32_t I2C_EVENT)
{
    long timeout = 100000;
    while (I2C_CheckEvent(I2C_PERIPH, I2C_EVENT) != SUCCESS) {
        if (timeout-- < 0)
            return 0;
    }
    return 1;
}

void MyI2C_start()
{
    I2C_GenerateSTART(I2C_PERIPH, ENABLE);
    MyI2C_wait_event(I2C_EVENT_MASTER_MODE_SELECT);
}

void MyI2C_stop()
{
    MyI2C_sda_write(0);
    MyI2C_scl_write(1);
    MyI2C_sda_write(1);
}

uint8_t MyI2C_send_addr(uint8_t addr, uint8_t i2c_direction)
{
    I2C_Send7bitAddress(I2C_PERIPH, addr, i2c_direction);

    uint32_t event;

    if (i2c_direction == I2C_Direction_Transmitter)
        event = I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED;
    else
        event = I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED;

    return MyI2C_wait_event(event);
}

void MyI2C_send_byte(uint8_t byte)
{
    // 当地址发送结束后, 是否会产生一个EV8事件?
    MyI2C_wait_event(I2C_EVENT_MASTER_BYTE_TRANSMITTING);
    I2C_SendData(I2C_PERIPH, byte);
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
    MyI2C_send_addr(device_addr, I2C_Direction_Transmitter);
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

void MyI2C_read_data_uint16(uint8_t device_addr, uint8_t reg_addr_start, uint16_t *arr, uint8_t length)
{
    MyI2C_start();
    MyI2C_send_byte(device_addr);
    MyI2C_send_byte(reg_addr_start);
    MyI2C_start();
    MyI2C_send_byte(device_addr | 0x01);

    uint8_t need_continue = 1;

    for (int i = 0; i < length; i++) {
        arr[i] = 0;
        arr[i] = MyI2C_receive_byte(need_continue) << 8;
        if (i >= length - 1)
            need_continue = 0;
        arr[i] |= MyI2C_receive_byte(need_continue);
    }
    MyI2C_stop();
}