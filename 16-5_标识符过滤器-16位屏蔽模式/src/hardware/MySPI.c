#include "stm32f10x.h"

#define MYSPI_RCC_GPIO RCC_APB2Periph_GPIOA
#define MYSPI_RCC_SPI  RCC_APB2Periph_SPI1
#define MYSPI_SPI      SPI1
#define MYSPI_GPIO     GPIOA
#define MYSPI_MOSI     GPIO_Pin_7
#define MYSPI_SCK      GPIO_Pin_5
#define MYSPI_MISO     GPIO_Pin_6
#define MYSPI_CS       GPIO_Pin_4

void MySPI_write_CS(uint8_t bit_value)
{
    GPIO_WriteBit(MYSPI_GPIO, MYSPI_CS, (BitAction)bit_value);
}

void MySPI_init()
{
    RCC_APB2PeriphClockCmd(MYSPI_RCC_SPI, ENABLE);
    RCC_APB2PeriphClockCmd(MYSPI_RCC_GPIO, ENABLE);

    GPIO_InitTypeDef gpio_config;
    gpio_config.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_config.GPIO_Pin   = MYSPI_CS;
    gpio_config.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(MYSPI_GPIO, &gpio_config);

    gpio_config.GPIO_Pin  = MYSPI_MISO;
    gpio_config.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(MYSPI_GPIO, &gpio_config);

    gpio_config.GPIO_Pin  = MYSPI_MOSI | MYSPI_SCK;
    gpio_config.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(MYSPI_GPIO, &gpio_config);

    SPI_InitTypeDef spi_config;
    spi_config.SPI_Mode              = SPI_Mode_Master;
    spi_config.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
    // clock polarity 时钟极性
    spi_config.SPI_CPOL = SPI_CPOL_Low;
    // clock phase 时钟相位
    spi_config.SPI_CPHA          = SPI_CPHA_1Edge;
    spi_config.SPI_CRCPolynomial = 7;
    spi_config.SPI_DataSize      = SPI_DataSize_8b;
    spi_config.SPI_Direction     = SPI_Direction_2Lines_FullDuplex;
    spi_config.SPI_FirstBit      = SPI_FirstBit_MSB;
    spi_config.SPI_NSS           = SPI_NSS_Soft;

    SPI_Init(MYSPI_SPI, &spi_config);
    SPI_Cmd(MYSPI_SPI, ENABLE);
    MySPI_write_CS(1);
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
    while (!SPI_I2S_GetFlagStatus(MYSPI_SPI, SPI_I2S_FLAG_TXE));
    SPI_I2S_SendData(MYSPI_SPI, byte);
    while (!SPI_I2S_GetFlagStatus(MYSPI_SPI, SPI_I2S_FLAG_RXNE));
    return SPI_I2S_ReceiveData(MYSPI_SPI);
}

void MySPI_wait_TXE()
{
    while (SPI_I2S_GetFlagStatus(MYSPI_SPI, SPI_I2S_FLAG_TXE) == RESET);
}

void MySPI_wait_RXNE()
{
    while (!SPI_I2S_GetFlagStatus(MYSPI_SPI, SPI_I2S_FLAG_RXNE));
}

void MySPI_wait_NOT_BUSY()
{
    while (SPI_I2S_GetFlagStatus(MYSPI_SPI, SPI_I2S_FLAG_BSY));
}

void MySPI_send_byte(uint8_t byte)
{
    MySPI_wait_TXE();
    SPI_I2S_SendData(MYSPI_SPI, byte);
}

uint8_t MySPI_receive_byte()
{
    uint8_t byte;
    MySPI_wait_RXNE();
    byte = SPI_I2S_ReceiveData(MYSPI_SPI);
    return byte;
}

void MySPI_fast_swap(uint8_t *data, uint32_t length)
{
    uint32_t i = 0;
    uint32_t j = 0;
    MySPI_send_byte(data[i++]);
    while (i < length) {
        MySPI_send_byte(data[i++]);
        data[j++] = MySPI_receive_byte();
    }
    data[j] = MySPI_receive_byte();
}

void MySPI_fast_send(uint8_t *data, uint32_t length)
{
    for (uint32_t i = 0; i < length; i++) {
        MySPI_send_byte(data[i]);
    }
    MySPI_wait_TXE();
    MySPI_wait_NOT_BUSY();
    MySPI_receive_byte();
}

uint8_t MySPI_fast_receive(uint8_t *data, uint32_t length)
{

    // 说明: 本函数没有用函数包装, 而是采用直接访问寄存器的方式进行编程, 因为
    // 任何形式的函数都有上下文切换开销, 即使是库函数本身, 因为由前置校验, 时钟周期
    // 也在60+, 导致跟不上SPI的时钟周期, 如果采用直接访问寄存器的方式,
    // 可以大幅缩短指令执行周期, 从而能最快跟上4分频的SPI时钟周期(也有些许间断, 不是完全连续)
    // 如果要完全连续, 预计需要采用DMA的方式
    while ((MYSPI_SPI->SR & 0x02) == RESET);
    MYSPI_SPI->DR = 0xFF; // 11个时钟周期, SPI_I2S_SendData()需要61个时钟周期

    uint32_t i = 0;
    for (; i < length - 1; i++) {
        while ((MYSPI_SPI->SR & 0x02) == RESET); // 20个时钟周期
        MYSPI_SPI->DR = 0xFF;                    // 11个时钟周期
        // 在上一个DR写完, 开始传输时, 在4分频下, 只要下面的wait条件小于32个时钟周期, 均可读取
        while ((MYSPI_SPI->SR & 0x01) == RESET);
        data[i] = MYSPI_SPI->DR; // 16个时钟周期
    }

    while ((MYSPI_SPI->SR & 0x01) == RESET);
    data[i] = MYSPI_SPI->DR;

    // 检测是否有溢出错误
    return SPI_I2S_GetFlagStatus(MYSPI_SPI, SPI_I2S_FLAG_OVR);
}
