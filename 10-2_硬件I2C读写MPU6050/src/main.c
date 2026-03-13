#include <stm32f10x.h>
#include "system/Delay.h"
#include "hardware/OLED.h"
#include "hardware/MPU6050.h"
#include "hardware/MPU6050_REG.h"

int main(int argc, char const *argv[])
{
    OLED_Init();
    MPU6050_init();
    MPU6050_DATA data = {0};

    float temp = 0;
    OLED_ShowString(1, 1, "ID:");
    OLED_ShowHexNum(1, 4, MPU6050_get_id(), 2);
    OLED_ShowString(1, 7, "T:");
    while (1) {
        MPU6050_get_sensor_data(&data);
        OLED_ShowSignedNum(2, 1, data.ACCEL_XOUT, 6);
        OLED_ShowSignedNum(2, 9, data.GYRO_XOUT, 6);
        OLED_ShowSignedNum(3, 1, data.ACCEL_YOUT, 6);
        OLED_ShowSignedNum(3, 9, data.GYRO_YOUT, 6);
        OLED_ShowSignedNum(4, 1, data.ACCEL_ZOUT, 6);
        OLED_ShowSignedNum(4, 9, data.GYRO_ZOUT, 6);
        temp = 35 + (data.TEMP_OUT + 521) / 340.;
        OLED_ShowFloat(1, 9, temp, 7, 2);
        Delay_ms(200);
    }

    return 0;
}
