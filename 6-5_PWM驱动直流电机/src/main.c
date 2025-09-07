#include <stm32f10x.h>
#include "hardware/OLED.h"
#include "hardware/button.h"
#include "hardware/motor.h"

float speed = 0;

void operate_for_key(int key)
{
    if (!(key > 0))
        return;
    switch (key) {
        case 1:
            speed -= 20;
            break;
        case 2:
            speed += 20;
            break;
        default:
            break;
    }

    if (speed > 100)
        speed = 100;

    if (speed < -100)
        speed = -100;

    motor_set_speed(speed);
    OLED_ShowSignedNum(1, 7, speed, 3);
}

int main(int argc, char const *argv[])
{
    OLED_Init();
    button_init();
    motor_init();
    OLED_ShowString(1, 1, "Speed:");
    OLED_ShowSignedNum(1, 7, speed, 3);

    while (1) {
        operate_for_key(button_get_key());
    }

    return 0;
}
