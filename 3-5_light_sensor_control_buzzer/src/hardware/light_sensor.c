#include <stm32f10x.h>
#include "system/Delay.h"

#define LIGHT_SENSOR_RCC  RCC_APB2Periph_GPIOB
#define LIGHT_SENSOR_GPIO GPIOB
#define LIGHT_SENSOR_PIN  GPIO_Pin_0

void light_sensor_init()
{
    RCC_APB2PeriphClockCmd(LIGHT_SENSOR_RCC, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin   = LIGHT_SENSOR_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LIGHT_SENSOR_GPIO, &GPIO_InitStructure);
}

uint8_t light_sensor_read()
{
    return GPIO_ReadInputDataBit(LIGHT_SENSOR_GPIO, LIGHT_SENSOR_PIN);
}