#include <stm32f10x.h>
#include "hardware/OLED.h"
#include "system/Timer.h"
#include "hardware/encoder_speed.h"

int last_position = 0;
int speed         = 0;

int main(int argc, char const *argv[])
{
    OLED_Init();
    timer_init();
    encoder_init();
    OLED_ShowString(1, 1, "Position:");
    OLED_ShowString(2, 1, "Speed:");
    while (1) {
        OLED_ShowSignedNum(1, 10, encoder_get_position(), 5);
        OLED_ShowSignedNum(2, 10, speed, 5);
    }

    return 0;
}

void TIM2_IRQHandler()
{
    // 0.5s
    int current_position;
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) {
        current_position = encoder_get_position();
        speed            = (current_position - last_position) * 2;
        last_position    = current_position;
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}
