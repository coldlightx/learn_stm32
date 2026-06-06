#include <stm32f10x.h>
#include "hardware/OLED.h"
#include "hardware/BKP.h"
#include "hardware/button.h"

int main(int argc, char const *argv[])
{
    OLED_Init();
    BKP_init();
    button_init();

    uint16_t write_data[] = {0x1234, 0x5678};
    uint16_t read_data[2];

    OLED_ShowString(1, 1, "W:");
    OLED_ShowString(2, 1, "R:");

    while (1) {
        if (button_get_key() == 1) {
            write_data[0]++;
            write_data[1]++;
            BKP_WriteBackupRegister(BKP_DR1, write_data[0]);
            BKP_WriteBackupRegister(BKP_DR2, write_data[1]);
            OLED_ShowHexNum(1, 3, write_data[0], 4);
            OLED_ShowHexNum(1, 8, write_data[1], 4);
        }

        read_data[0] = BKP_ReadBackupRegister(BKP_DR1);
        read_data[1] = BKP_ReadBackupRegister(BKP_DR2);

        OLED_ShowHexNum(2, 3, read_data[0], 4);
        OLED_ShowHexNum(2, 8, read_data[1], 4);
    }

    return 0;
}
