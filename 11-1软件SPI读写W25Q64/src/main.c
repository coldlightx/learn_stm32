#include <stm32f10x.h>
#include "system/Delay.h"
#include "hardware/OLED.h"
#include "hardware/W25Q64.h"

int main(int argc, char const *argv[])
{
    OLED_Init();
    W25Q64_init();

    uint32_t data;
    data = W25Q64_read_id();

    OLED_ShowString(1, 1, "MID:");
    OLED_ShowHexNum(1, 5, data >> 16, 2);
    OLED_ShowString(1, 8, "DID:");
    OLED_ShowHexNum(1, 12, data, 4);
    W25Q64_sector_erase(0x010100);
    uint8_t write_data[] = {1, 2, 3, 4};
    //                      01 10 11 100
    //                      10 11 01 111
    //                      00 10 01 100
    // uint8_t write_data[] = {0x02, 0x03, 0x01, 0x07};
    uint8_t read_data[4] = {0};
    W25Q64_page_program(0x010100, write_data, 4);
    W25Q64_read_data(0x010100, read_data, 4);
    OLED_ShowString(2, 1, "W:");
    OLED_ShowHexArray(2, 3, write_data, 4);
    OLED_ShowString(3, 1, "R:");
    OLED_ShowHexArray(3, 3, read_data, 4);
    while (1) {
    }

    return 0;
}
