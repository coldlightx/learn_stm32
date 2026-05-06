#include "stm32f10x.h"

#define MYSPI_RCC_GPIO RCC_APB2Periph_GPIOA
#define MYSPI_GPIO     GPIOA
#define MYSPI_MOSI     GPIO_Pin_7
#define MYSPI_SCK      GPIO_Pin_5
#define MYSPI_MISO     GPIO_Pin_6
#define MYSPI_CS       GPIO_Pin_4

void MySPI_write_CS(uint8_t bit_value)
{
    GPIO_WriteBit(MYSPI_GPIO, MYSPI_CS, (BitAction)bit_value);
}

void MySPI_write_SCK(uint8_t bit_value)
{
    GPIO_WriteBit(MYSPI_GPIO, MYSPI_SCK, (BitAction)bit_value);
}

void MySPI_write_MOSI(uint8_t bit_value)
{
    GPIO_WriteBit(MYSPI_GPIO, MYSPI_MOSI, (BitAction)bit_value);
}

uint8_t MySPI_read_MISO()
{
    return GPIO_ReadInputDataBit(MYSPI_GPIO, MYSPI_MISO);
}

void MySPI_init()
{
    RCC_APB2PeriphClockCmd(MYSPI_RCC_GPIO, ENABLE);

    GPIO_InitTypeDef gpio_config;
    gpio_config.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_config.GPIO_Pin   = MYSPI_MOSI | MYSPI_SCK | MYSPI_CS;
    gpio_config.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(MYSPI_GPIO, &gpio_config);

    gpio_config.GPIO_Pin  = MYSPI_MISO;
    gpio_config.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(MYSPI_GPIO, &gpio_config);

    MySPI_write_CS(1);
    MySPI_write_SCK(0);
}

void MySPI_start()
{
    MySPI_write_CS(0);
}

void MySPI_stop()
{
    MySPI_write_CS(1);
}

uint8_t MySPI_swap_byte(uint8_t byte)
{
    for (int i = 0; i < 8; i++) {
        MySPI_write_SCK(0);
        MySPI_write_MOSI(byte & 0x80);
        byte <<= 1;
        MySPI_write_SCK(1);
        byte |= MySPI_read_MISO();
    }
    return byte;
}