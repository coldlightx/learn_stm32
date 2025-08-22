#include <stm32f10x.h>
#include "system/Delay.h"
#include "hardware/OLED.h"
#include "hardware/pwm.h"
#include "hardware/IC.h"

unsigned int timer_counter = 0;

int main(int argc, char const *argv[])
{
    OLED_Init();
    pwm_init();
    IC_init();

    pwm_set_frequency(1);
    pwm_set_duty(50);

    OLED_ShowString(1, 1, "Freq:000000Hz");

    while (1) {
        OLED_ShowNum(1, 6, IC_get_frequency(), 6);
    }

    return 0;
}
