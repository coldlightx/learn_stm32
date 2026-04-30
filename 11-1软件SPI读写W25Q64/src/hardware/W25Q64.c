#include "stm32f10x.h"
#include "MySPI_software.h"
#include "W25Q64.h"

void W25Q64_init()
{
    MySPI_init();
}

void W25Q64_write_enable()
{
    MySPI_start();
    MySPI_swap_byte(W25Q64_WRITE_ENABLE);
    MySPI_stop();
}

uint8_t W25Q64_read_status_reg1()
{
    uint8_t data;
    MySPI_start();
    MySPI_swap_byte(W25Q64_READ_STATUS_REG1);
    data = MySPI_swap_byte(W25Q64_EMPTY_DATA);
    MySPI_stop();
    return data;
}

void W25Q64_wait_device_busy()
{
    long timeout = 1000000;
    MySPI_start();
    MySPI_swap_byte(W25Q64_READ_STATUS_REG1);
    while (MySPI_swap_byte(W25Q64_EMPTY_DATA) & 0x01 && timeout-- > 0);
    MySPI_stop();
}

void W25Q64_send_address(uint32_t address)
{
    MySPI_swap_byte(address >> 16);
    MySPI_swap_byte(address >> 8);
    MySPI_swap_byte(address);
}

void W25Q64_page_program(uint32_t address, uint8_t *data, uint8_t length)
{

    W25Q64_wait_device_busy();
    W25Q64_write_enable();
    MySPI_start();
    MySPI_swap_byte(W25Q64_PAGE_PROGRAM);
    W25Q64_send_address(address);
    for (int i = 0; i < length; i++) {
        MySPI_swap_byte(data[i]);
    }
    MySPI_stop();
}

void W25Q64_sector_erase(uint32_t address)
{
    W25Q64_wait_device_busy();
    W25Q64_write_enable();
    MySPI_start();
    MySPI_swap_byte(W25Q64_SECTOR_ERASE);
    W25Q64_send_address(address);
    MySPI_stop();
}

uint32_t W25Q64_read_id()
{
    W25Q64_wait_device_busy();
    uint32_t data = 0;
    MySPI_start();
    MySPI_swap_byte(W25Q64_JEDEC_ID);

    data |= MySPI_swap_byte(W25Q64_EMPTY_DATA);
    data <<= 8;
    data |= MySPI_swap_byte(W25Q64_EMPTY_DATA);
    data <<= 8;
    data |= MySPI_swap_byte(W25Q64_EMPTY_DATA);

    MySPI_stop();
    return data;
}

void W25Q64_read_data(uint32_t address, uint8_t *data, uint32_t length)
{
    W25Q64_wait_device_busy();
    MySPI_start();
    MySPI_swap_byte(W25Q64_READ_DATA);
    W25Q64_send_address(address);

    for (uint32_t i = 0; i < length; i++) {
        data[i] = MySPI_swap_byte(W25Q64_EMPTY_DATA);
    }
    MySPI_stop();
}
