#include "stm32f10x.h"

void timer_init()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    // TIM2使用外部时钟
    TIM_InternalClockConfig(TIM2);

    TIM_TimeBaseInitTypeDef timer_base_config;
    timer_base_config.TIM_ClockDivision     = TIM_CKD_DIV1;
    timer_base_config.TIM_CounterMode       = TIM_CounterMode_Up;
    timer_base_config.TIM_Period            = 5000 - 1;
    timer_base_config.TIM_Prescaler         = 7200 - 1;
    timer_base_config.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &timer_base_config);

    TIM_ClearFlag(TIM2, TIM_FLAG_Update);

    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitTypeDef nvic_config;
    nvic_config.NVIC_IRQChannel                   = TIM2_IRQn;
    nvic_config.NVIC_IRQChannelCmd                = ENABLE;
    nvic_config.NVIC_IRQChannelPreemptionPriority = 2;
    nvic_config.NVIC_IRQChannelSubPriority        = 1;
    NVIC_Init(&nvic_config);

    // 定时器运行使能
    TIM_Cmd(TIM2, ENABLE);
}

// void TIM2_IRQHandler()
// {
//     if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) {
//         TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
//     }
// }
