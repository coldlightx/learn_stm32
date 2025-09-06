#include <stm32f10x.h>
#include "hardware/OLED.h"
#include "system/Delay.h"
#include "system/MyDMA.h"

uint8_t A[] = {0x02, 0x05, 0x08, 0x0A};
uint8_t B[] = {0, 0, 0, 0};

void OLED_show_array(int row, uint8_t *arr, int length)
{
    for (int i = 0; i < length; i++)
        OLED_ShowHexNum(row, i * 3 + 1, arr[i], 2);
}

int main(int argc, char const *argv[])
{
    OLED_Init();

    MyDMA_Init((uint32_t)A, (uint32_t)B, 4);
    OLED_ShowString(1, 1, "ArrayA");
    OLED_ShowHexNum(1, 8, (uint32_t)A, 8);
    OLED_ShowString(3, 1, "ArrayB");
    OLED_ShowHexNum(3, 8, (uint32_t)B, 8);

    while (1) {
        for (int i = 0; i < 4; i++)
            A[i]++;
        OLED_show_array(2, A, 4);
        OLED_show_array(4, B, 4);
        Delay_ms(1000);
        MyDMA_Transfer(4);
        OLED_show_array(2, A, 4);
        OLED_show_array(4, B, 4);
        Delay_ms(1000);
    }

    return 0;
}
