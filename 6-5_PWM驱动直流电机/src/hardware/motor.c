#include "stm32f10x.h"
#include "hardware/pwm.h"

void motor_init()
{
    pwm_init();
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef gpio_config;
    gpio_config.GPIO_Mode  = GPIO_Mode_Out_PP;
    gpio_config.GPIO_Pin   = GPIO_Pin_8 | GPIO_Pin_9;
    gpio_config.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpio_config);
}

void motor_set_speed(int speed)
{
    if (speed >= 0) {
        // 正转
        GPIO_SetBits(GPIOA, GPIO_Pin_9);
        GPIO_ResetBits(GPIOA, GPIO_Pin_8);
        pwm_set_compare((uint16_t)speed);
    } else if (speed < 0) {
        GPIO_ResetBits(GPIOA, GPIO_Pin_9);
        GPIO_SetBits(GPIOA, GPIO_Pin_8);
        pwm_set_compare((uint16_t)-speed);
    }
}