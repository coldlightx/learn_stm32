#include "stm32f10x.h"
#include "hardware/pwm.h"

void servo_init()
{
    pwm_init();
}

void servo_set_angle(float angle)
{
    pwm_set_compare((uint16_t)(angle / 180 * 2000 + 500));
}