#include "stm32f10x.h"

#define SENSOR_RCC_GPIO        RCC_APB2Periph_GPIOA
#define SENSOR_GPIO            GPIOA
#define SENSOR_DATA_PIN        GPIO_Pin_11
#define SENSOR_GPIO_PINSOURCE  GPIO_PinSource11
#define SENSOR_EXTI_LINE       EXTI_Line11

#define SENSOR_NVIC_IRQChannel EXTI15_10_IRQn;

uint16_t sensor_counter = 0;

void IR_sensor_counter_init()
{

    // gpio config
    RCC_APB2PeriphClockCmd(SENSOR_RCC_GPIO, ENABLE);

    GPIO_InitTypeDef gpio_config;
    gpio_config.GPIO_Pin   = SENSOR_DATA_PIN;
    gpio_config.GPIO_Mode  = GPIO_Mode_IPU;
    gpio_config.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SENSOR_GPIO, &gpio_config);

    // AFIO config
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, SENSOR_GPIO_PINSOURCE);

    // EXTI config
    EXTI_InitTypeDef exti_config;
    exti_config.EXTI_Line    = SENSOR_EXTI_LINE;
    exti_config.EXTI_LineCmd = ENABLE;
    exti_config.EXTI_Mode    = EXTI_Mode_Interrupt;
    exti_config.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_Init(&exti_config);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitTypeDef nvic_config;

    nvic_config.NVIC_IRQChannel                   = SENSOR_NVIC_IRQChannel;
    nvic_config.NVIC_IRQChannelCmd                = ENABLE;
    nvic_config.NVIC_IRQChannelPreemptionPriority = 1;
    nvic_config.NVIC_IRQChannelSubPriority        = 1;

    NVIC_Init(&nvic_config);
}

uint16_t IR_get_sensor_count()
{
    return sensor_counter;
}

void EXTI15_10_IRQHandler()
{
    int wait_count = 6;
    if (EXTI_GetITStatus(SENSOR_EXTI_LINE) == SET) {
        // 对射式红外传感器在遮挡时, 有概率存在抖动现象, 需要消抖
        // TODO 通过示波器观察验证
        while (wait_count-- > 0);
        if (!GPIO_ReadInputDataBit(SENSOR_GPIO, SENSOR_DATA_PIN)) sensor_counter++;
        EXTI_ClearITPendingBit(SENSOR_EXTI_LINE);
    }
}