#include "stm32f10x.h"

int counter;

void rotate_encoder_init()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef gpio_config;
    gpio_config.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1;
    gpio_config.GPIO_Mode  = GPIO_Mode_IPU;
    gpio_config.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gpio_config);

    // AFIO config
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);

    // EXTI config
    EXTI_InitTypeDef exti_config;
    exti_config.EXTI_Line    = EXTI_Line0 | EXTI_Line1;
    exti_config.EXTI_LineCmd = ENABLE;
    exti_config.EXTI_Mode    = EXTI_Mode_Interrupt;
    exti_config.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_Init(&exti_config);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitTypeDef nvic_config;
    nvic_config.NVIC_IRQChannel                   = EXTI0_IRQn;
    nvic_config.NVIC_IRQChannelCmd                = ENABLE;
    nvic_config.NVIC_IRQChannelPreemptionPriority = 1;
    nvic_config.NVIC_IRQChannelSubPriority        = 1;
    NVIC_Init(&nvic_config);

    nvic_config.NVIC_IRQChannel            = EXTI1_IRQn;
    nvic_config.NVIC_IRQChannelSubPriority = 2;
    NVIC_Init(&nvic_config);
}

int rotate_encoder_get_count()
{
    return counter;
}

// A口的下降沿中断
void EXTI0_IRQHandler()
{
    // 反转
    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)
        counter--;
    EXTI_ClearITPendingBit(EXTI_Line0);
}

// B口的下降沿中断
void EXTI1_IRQHandler()
{
    // 正转
    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0)
        counter++;
    EXTI_ClearITPendingBit(EXTI_Line1);
}