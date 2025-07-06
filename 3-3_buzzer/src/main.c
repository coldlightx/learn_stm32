#include <stm32f10x.h>
#include "system/Delay.h"

int main(int argc, char const *argv[])
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef gpio_config;
    gpio_config.GPIO_Pin   = GPIO_Pin_8;
    gpio_config.GPIO_Mode  = GPIO_Mode_Out_PP;
    gpio_config.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpio_config);

    while (1) {
        GPIO_WriteBit(GPIOA, GPIO_Pin_8, Bit_RESET);
        Delay_ms(100);
        GPIO_WriteBit(GPIOA, GPIO_Pin_8, Bit_SET);
        Delay_ms(100);
        GPIO_WriteBit(GPIOA, GPIO_Pin_8, Bit_RESET);
        Delay_ms(100);
        GPIO_WriteBit(GPIOA, GPIO_Pin_8, Bit_SET);
        Delay_ms(700);
    }

    return 0;
}
