#include <stm32f10x.h>
#include "hardware/OLED.h"
#include "system/Timer.h"

unsigned int timer_counter = 0;

int main(int argc, char const *argv[])
{
    OLED_Init();
    timer_init();
    OLED_ShowString(1, 1, "Number:");
    OLED_ShowString(2, 1, "Counter:");
    while (1) {
        OLED_ShowNum(1, 8, timer_counter, 6);
        OLED_ShowNum(2, 9, timer_get_counter(), 6);
    }

    return 0;
}

void TIM2_IRQHandler()
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) {
        timer_counter++;
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}
