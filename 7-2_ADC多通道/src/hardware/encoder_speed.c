#include "stm32f10x.h"

void encoder_init()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef gpio_config;
    gpio_config.GPIO_Pin   = GPIO_Pin_6 | GPIO_Pin_7;
    gpio_config.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    gpio_config.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpio_config);

    TIM_TimeBaseInitTypeDef tim_config;
    TIM_TimeBaseStructInit(&tim_config);

    tim_config.TIM_ClockDivision = TIM_CKD_DIV1;
    tim_config.TIM_CounterMode   = TIM_CounterMode_Up;
    tim_config.TIM_Period        = 65536 - 1;
    tim_config.TIM_Prescaler     = 1 - 1;
    TIM_TimeBaseInit(TIM3, &tim_config);

    TIM_ICInitTypeDef ic_config;
    TIM_ICStructInit(&ic_config);

    ic_config.TIM_Channel  = TIM_Channel_1;
    ic_config.TIM_ICFilter = 0x0F;
    TIM_ICInit(TIM3, &ic_config);
    ic_config.TIM_Channel = TIM_Channel_2;
    TIM_ICInit(TIM3, &ic_config);

    TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
    TIM_Cmd(TIM3, ENABLE);
}

int16_t encoder_get_position()
{
    return TIM_GetCounter(TIM3);
}

void encoder_reset_position()
{
    TIM_SetCounter(TIM3, 0);
}
