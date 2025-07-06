#include <stm32f10x.h>
#include "system/Delay.h"

int main(int argc, char const *argv[])
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef gpio_config;
    gpio_config.GPIO_Pin   = GPIO_Pin_All;
    gpio_config.GPIO_Mode  = GPIO_Mode_Out_PP;
    gpio_config.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpio_config);

    int value      = 1;
    int delay_time = 100;

    while (1) {
        for (int i = 0; i < 8; i++) {
            GPIO_Write(GPIOA, ~(value << i));
            Delay_ms(delay_time);
        }
    }

    return 0;
}
