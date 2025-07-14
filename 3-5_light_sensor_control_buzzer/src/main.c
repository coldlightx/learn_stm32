#include <stm32f10x.h>
#include "system/Delay.h"
#include "hardware/buzzer.h"
#include "hardware/light_sensor.h"

int main(int argc, char const *argv[])
{
    buzzer_init();
    light_sensor_init();

    while (1) {
        if (light_sensor_read())
            buzzer_on();
        else
            buzzer_off();
    }

    return 0;
}
