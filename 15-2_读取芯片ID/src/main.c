#include <stm32f10x.h>
#include "hardware/OLED.h"
#include "hardware/button.h"
#include "system/MyStore.h"

int main(int argc, char const *argv[])
{
    OLED_Init();

    OLED_ShowString(1, 1, "FLASH_SIZE:");
    OLED_ShowHexNum(1, 12, *((__IO uint16_t *)0x1FFFF7E0), 4);
    OLED_ShowString(2, 1, "U_ID:");
    OLED_ShowHexNum(2, 6, *((__IO uint16_t *)0x1FFFF7E8), 4);
    OLED_ShowHexNum(2, 11, *((__IO uint16_t *)(0x1FFFF7E8 + 0x02)), 4);
    OLED_ShowHexNum(3, 1, *((__IO uint32_t *)(0x1FFFF7E8 + 0x04)), 8);
    OLED_ShowHexNum(4, 1, *((__IO uint32_t *)(0x1FFFF7E8 + 0x08)), 8);

    return 0;
}
