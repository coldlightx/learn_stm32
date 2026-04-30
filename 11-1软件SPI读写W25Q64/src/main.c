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
    OLED_ShowHexNum(1, 1, data, 8);
    while (1) {
    }

    return 0;
}
