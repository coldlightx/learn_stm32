#include <stm32f10x.h>
#include "system/Delay.h"

void button_init()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

int check_key_pressed(uint16_t GPIO_Pin)
{
    uint8_t bit_value = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin);
    if (bit_value)
        return 0;

    Delay_ms(20);
    while (!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin));
    Delay_ms(20);
    return 1;
}

int check_key1()
{
    return check_key_pressed(GPIO_Pin_9);
}

int check_key2()
{
    return check_key_pressed(GPIO_Pin_6);
}

int button_get_key()
{
    if (check_key1())
        return 1;

    if (check_key2())
        return 2;

    return 0;
}