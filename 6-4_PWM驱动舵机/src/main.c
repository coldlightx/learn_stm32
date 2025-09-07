#include <stm32f10x.h>
#include "system/Delay.h"
#include "hardware/OLED.h"
#include "hardware/pwm.h"
#include "hardware/button.h"
#include "hardware/servo.h"

float angle = 0;

int main(int argc, char const *argv[])
{
    OLED_Init();
    button_init();
    servo_init();
    servo_set_angle(angle);

    OLED_ShowString(1, 1, "Angle:");
    OLED_ShowNum(1, 7, angle, 3);

    while (1) {
        int key = button_get_key();
        switch (key) {
            case 1:
                angle += 30;
                break;
            case 2:
                angle -= 30;
                break;
            default:
                break;
        }

        if (angle < 0)
            angle = 0;

        if (angle > 180)
            angle = 180;

        if (key > 0) {
            servo_set_angle(angle);
            OLED_ShowNum(1, 7, angle, 3);
        }
    }

    return 0;
}
