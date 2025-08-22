#include "stm32f10x.h"

#define BUZZER_RCC  RCC_APB2Periph_GPIOA
#define BUZZER_GPIO GPIOA
#define BUZZER_PIN  GPIO_Pin_8

void buzzer_init()
{
    RCC_APB2PeriphClockCmd(BUZZER_RCC, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin   = BUZZER_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BUZZER_GPIO, &GPIO_InitStructure);
    GPIO_SetBits(BUZZER_GPIO, BUZZER_PIN);
}

void buzzer_on()
{
    GPIO_ResetBits(BUZZER_GPIO, BUZZER_PIN);
}

void buzzer_off()
{
    GPIO_SetBits(BUZZER_GPIO, BUZZER_PIN);
}