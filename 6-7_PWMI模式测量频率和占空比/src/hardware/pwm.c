#include "stm32f10x.h"

#define CCR_MAX 100

void pwm_init()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    // 配置GPIO
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef gpio_config;
    gpio_config.GPIO_Mode  = GPIO_Mode_AF_PP; // 复用推挽输出
    gpio_config.GPIO_Pin   = GPIO_Pin_0;
    gpio_config.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpio_config);

    TIM_InternalClockConfig(TIM2);

    TIM_TimeBaseInitTypeDef timer_base_config;
    timer_base_config.TIM_ClockDivision     = TIM_CKD_DIV1;
    timer_base_config.TIM_CounterMode       = TIM_CounterMode_Up;
    timer_base_config.TIM_Period            = CCR_MAX - 1;
    timer_base_config.TIM_Prescaler         = 720 - 1;
    timer_base_config.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &timer_base_config);
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);

    TIM_OCInitTypeDef tim_oc_config;

    TIM_OCStructInit(&tim_oc_config);
    tim_oc_config.TIM_OCMode      = TIM_OCMode_PWM1;
    tim_oc_config.TIM_OCPolarity  = TIM_OCPolarity_High;
    tim_oc_config.TIM_OutputState = TIM_OutputState_Enable;
    tim_oc_config.TIM_Pulse       = 0;
    TIM_OC1Init(TIM2, &tim_oc_config);

    // 定时器运行使能
    TIM_Cmd(TIM2, ENABLE);
}

void pwm_set_compare(uint16_t compare)
{
    TIM_SetCompare1(TIM2, compare);
}

unsigned int pwm_get_max_compare()
{
    return CCR_MAX;
}

void pwm_set_prescaler(uint16_t prescaler)
{
    TIM_PrescalerConfig(TIM2, prescaler, TIM_PSCReloadMode_Update);
}

void pwm_set_frequency(unsigned int freq_khz)
{
    uint16_t prescaler = 720000 / (freq_khz * 1000);
    pwm_set_prescaler(prescaler);
}

void pwm_set_duty(unsigned int duty)
{
    pwm_set_compare(duty);
}
