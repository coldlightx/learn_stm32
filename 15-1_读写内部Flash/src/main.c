#include <stm32f10x.h>
#include "hardware/OLED.h"
#include "hardware/button.h"
#include "system/MyFlash.h"

int main(int argc, char const *argv[])
{
    OLED_Init();

    OLED_ShowHexNum(1, 1, MyFlash_read_word(0x08000000), 8);
    OLED_ShowHexNum(2, 1, MyFlash_read_halfword(0x08000000), 4);
    OLED_ShowHexNum(3, 1, MyFlash_read_byte(0x08000000), 2);

    while (1) {
        int key = button_get_key();

        if (key == 1) {
            // MyFlash_erase_page(0x08000000);
        } else if (key == 2) {
        }
    }

    return 0;
}
