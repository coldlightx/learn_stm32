#include <stm32f10x.h>
#include "system/Delay.h"
#include "hardware/OLED.h"
#include "hardware/pwm.h"

unsigned int timer_counter = 0;

int main(int argc, char const *argv[])
{
    OLED_Init();
    pwm_init();

    unsigned int max_compare = pwm_get_max_compare();
    while (1) {
        for (int i = 0; i <= max_compare; i++) {
            pwm_set_compare(i);
            Delay_ms(10);
        }

        for (int i = 0; i <= max_compare; i++) {
            pwm_set_compare(max_compare - i);
            Delay_ms(10);
        }
    }

    return 0;
}
