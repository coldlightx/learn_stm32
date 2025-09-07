#include <stm32f10x.h>
#include "hardware/OLED.h"
#include "hardware/rotate_encoder.h"

int main(int argc, char const *argv[])
{
    OLED_Init();
    rotate_encoder_init();

    OLED_ShowString(1, 1, "Number:");
    while (1) {
        OLED_ShowSignedNum(1, 8, rotate_encoder_get_count(), 6);
    }

    return 0;
}
