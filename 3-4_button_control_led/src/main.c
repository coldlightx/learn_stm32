#include <stm32f10x.h>
#include "system/Delay.h"
#include "hardware/led.h"
#include "hardware/button.h"

int main(int argc, char const *argv[])
{
    led_init();
    button_init();

    while (1) {
        int key = button_get_key();
        switch (key) {
            case 1:
                led1_toggle();
                break;
            case 2:
                led2_toggle();
                break;
            default:
                break;
        }
    }

    return 0;
}
