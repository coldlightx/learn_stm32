#include <stm32f10x.h>
#include "hardware/OLED.h"
#include "hardware/button.h"
#include "system/MyStore.h"

int main(int argc, char const *argv[])
{
    OLED_Init();
    button_init();
    MyStore_init();

    OLED_ShowString(1, 1, "Flag:");
    OLED_ShowHexNum(1, 6, MyStore_get_key(), 4);
    OLED_ShowString(2, 1, "Data:");

    while (1) {
        int key = button_get_key();

        if (key == 1) {
            MyStore_cache[0]++;
            MyStore_cache[1] += 2;
            MyStore_cache[2] += 3;
            MyStore_cache[3] += 4;
            MyStore_save();
        } else if (key == 2) {
            MyStore_clear();
        }

        OLED_ShowHexNum(3, 1, MyStore_cache[0], 4);
        OLED_ShowHexNum(3, 6, MyStore_cache[1], 4);
        OLED_ShowHexNum(4, 1, MyStore_cache[2], 4);
        OLED_ShowHexNum(4, 6, MyStore_cache[3], 4);
    }

    return 0;
}
