#include <stm32f10x.h>
#include "hardware/OLED.h"

int main(int argc, char const *argv[])
{
    OLED_Init();
    OLED_ShowChar(1, 1, 'A');
    OLED_ShowString(1, 3, "Hello,World!");
    OLED_ShowNum(2, 1, 12345, 6);
    OLED_ShowSignedNum(3, 1, 12345, 5);
    OLED_ShowHexNum(3, 8, 0xAA55, 4);
    OLED_ShowBinNum(4, 1, 0xAA55, 16);

    while (1) {
    }

    return 0;
}
