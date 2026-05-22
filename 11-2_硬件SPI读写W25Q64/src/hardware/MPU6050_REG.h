#if !defined(__MPU6050_REG_H)
#define __MPU6050_REG_H
#include "cmsis_compiler.h"

#define SMPLRT_DIV             0x19
#define CONFIG                 0x1A
#define GYRO_CONFIG            0x1B
#define ACCEL_CONFIG           0x1C
#define MPU6050_DATA_REG_START 0x3B
#define PWR_MGMT_1             0x6B
#define PWR_MGMT_2             0x6C
#define WHO_AM_I               0x75

typedef struct {
    int16_t ACCEL_XOUT;
    int16_t ACCEL_YOUT;
    int16_t ACCEL_ZOUT;
    int16_t TEMP_OUT;
    int16_t GYRO_XOUT;
    int16_t GYRO_YOUT;
    int16_t GYRO_ZOUT;
} MPU6050_DATA;

#define MPU6050_DATA_LENGTH 14

#endif // __MPU6050_REG_H
