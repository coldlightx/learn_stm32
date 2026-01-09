#include <stm32f10x.h>
#include <stdio.h>
#include <string.h>
#include "system/Delay.h"
#include "hardware/OLED.h"
#include "hardware/MPU6050.h"
#include "hardware/MPU6050_REG.h"

int main(int argc, char const *argv[])
{
    OLED_Init();
    MPU6050_init();
    MPU6050_DATA data;
    // OLED_ShowString(1, 1, "ACC     GYRO");
    OLED_ShowHexNum(1, 1, MPU6050_read_byte(WHO_AM_I), 2);

    while (1) {
        MPU6050_get_sensor_data(&data);
        OLED_ShowBinNum(1, 1, MPU6050_read_byte(ACCEL_CONFIG), 8);
        OLED_ShowBinNum(1, 9, MPU6050_read_byte(0x3C), 8);
        OLED_ShowSignedNum(2, 1, data.ACCEL_XOUT, 6);
        OLED_ShowSignedNum(2, 9, data.GYRO_XOUT, 6);
        OLED_ShowSignedNum(3, 1, data.ACCEL_YOUT, 6);
        OLED_ShowSignedNum(3, 9, data.GYRO_YOUT, 6);
        OLED_ShowSignedNum(4, 1, data.ACCEL_ZOUT, 6);
        OLED_ShowSignedNum(4, 9, data.GYRO_ZOUT, 6);
        Delay_ms(200);
    }

    return 0;
}
