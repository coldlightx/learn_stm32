#include "stm32f10x.h"
#include "MyI2C.h"
#include "MPU6050_REG.h"
#include "MPU6050.h"

#define MPU6050_ADDR 0xD0

void MPU6050_init()
{
    MyI2C_init();
    MPU6050_write_byte(PWR_MGMT_1, 0x01);
    MPU6050_write_byte(PWR_MGMT_2, 0x00);
    MPU6050_write_byte(SMPLRT_DIV, 9);
    MPU6050_write_byte(CONFIG, 0x00);
    MPU6050_write_byte(GYRO_CONFIG, 0x18);
    MPU6050_write_byte(ACCEL_CONFIG, 0x18);
}

uint8_t MPU6050_read_byte(uint8_t reg_addr)
{
    return MyI2C_read_data_byte(MPU6050_ADDR, reg_addr);
}

void MPU6050_read_data(uint8_t reg_addr_start, uint8_t *arr, uint8_t length)
{
    MyI2C_read_data_arr(MPU6050_ADDR, reg_addr_start, arr, length);
}

void MPU6050_write_byte(uint8_t reg_addr, uint8_t data)
{
    MyI2C_write_data_byte(MPU6050_ADDR, reg_addr, data);
}

void MPU6050_write_data(uint8_t reg_addr_start, uint8_t *arr, uint8_t length)
{
    MyI2C_write_data_arr(MPU6050_ADDR, reg_addr_start, arr, length);
}

void MPU6050_get_sensor_data(MPU6050_DATA *data)
{
    MPU6050_read_data(MPU6050_DATA_REG_START, (uint8_t *)data, MPU6050_DATA_LENGTH);
}