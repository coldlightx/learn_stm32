#include "stm32f10x.h"

void led_init()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_SetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1);
}

void led_toggle(uint16_t GPIO_Pin)
{
    uint8_t result = GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin);
    GPIO_WriteBit(GPIOA, GPIO_Pin, (BitAction)!result);
}

void led1_toggle()
{
    led_toggle(GPIO_Pin_0);
}

void led2_toggle()
{
    led_toggle(GPIO_Pin_1);
}

void led_on(uint16_t GPIO_Pin)
{
    GPIO_ResetBits(GPIOA, GPIO_Pin);
}

void led1_on()
{
    led_on(GPIO_Pin_0);
}

void led2_on()
{
    led_on(GPIO_Pin_1);
}

void led_off(uint16_t GPIO_Pin)
{
    GPIO_SetBits(GPIOA, GPIO_Pin);
}

void led1_off()
{
    led_off(GPIO_Pin_0);
}

void led2_off()
{
    led_off(GPIO_Pin_1);
}
