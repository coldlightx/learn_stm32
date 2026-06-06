#include "stm32f10x.h"

#define RCC_TIMER_COUNTER_TIM RCC_APB2Periph_TIM1
#define TIMER_COUNTER_TIM     TIM1

void timer_counter_init()
{
    RCC_APB2PeriphClockCmd(RCC_TIMER_COUNTER_TIM, ENABLE);
    TIM_InternalClockConfig(TIMER_COUNTER_TIM);

    TIM_TimeBaseInitTypeDef tim_config;
    tim_config.TIM_ClockDivision     = TIM_CKD_DIV1;
    tim_config.TIM_CounterMode       = TIM_CounterMode_Up;
    tim_config.TIM_Period            = (1 << 16) - 1;
    tim_config.TIM_Prescaler         = 0;
    tim_config.TIM_RepetitionCounter = 0;

    TIM_TimeBaseInit(TIMER_COUNTER_TIM, &tim_config);

    TIM_ClearFlag(TIMER_COUNTER_TIM, TIM_FLAG_Update);
    TIM_Cmd(TIMER_COUNTER_TIM, ENABLE);
}

uint16_t timer_counter_get_value()
{
    return TIM_GetCounter(TIMER_COUNTER_TIM);
}