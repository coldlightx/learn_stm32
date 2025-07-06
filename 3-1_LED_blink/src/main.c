#include <stm32f10x.h>
#include "Delay.h"

int main(void)
{

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef gpio_config;

    gpio_config.GPIO_Pin   = GPIO_Pin_0;
    gpio_config.GPIO_Mode  = GPIO_Mode_Out_PP;
    gpio_config.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpio_config);

    int delay_time = 500;

    while (1)
    {
        GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET);
        Delay_ms(delay_time);
        GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET);
        Delay_ms(delay_time);
    }

    return 0;
}