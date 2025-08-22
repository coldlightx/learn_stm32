#include "stm32f10x.h"

void IC_init()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef gpio_config;
    gpio_config.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    gpio_config.GPIO_Pin   = GPIO_Pin_6;
    gpio_config.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpio_config);

    TIM_TimeBaseInitTypeDef time_base_config;
    time_base_config.TIM_ClockDivision     = TIM_CKD_DIV1;
    time_base_config.TIM_CounterMode       = TIM_CounterMode_Up;
    time_base_config.TIM_Period            = 65536 - 1;
    time_base_config.TIM_Prescaler         = 72 - 1;
    time_base_config.TIM_RepetitionCounter = 0;

    TIM_TimeBaseInit(TIM3, &time_base_config);

    TIM_ICInitTypeDef ic_config;
    ic_config.TIM_Channel     = TIM_Channel_1;
    ic_config.TIM_ICFilter    = 0x00;
    ic_config.TIM_ICPolarity  = TIM_ICPolarity_Rising;
    ic_config.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    ic_config.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInit(TIM3, &ic_config);

    // 配置从触发模式
    TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1);
    TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);

    TIM_Cmd(TIM3, ENABLE);
}

unsigned long IC_get_frequency()
{
    unsigned int capture = TIM_GetCapture1(TIM3);
    return 1000000 / (capture - 1);
}