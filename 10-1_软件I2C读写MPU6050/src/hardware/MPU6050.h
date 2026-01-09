#if !defined(__MPU6050_H)
#define __MPU6050_H
#include "MPU6050_REG.h"

void MPU6050_init();
uint8_t MPU6050_read_byte(uint8_t reg_addr);
void MPU6050_read_data(uint8_t reg_addr_start, uint8_t *arr, uint8_t length);
void MPU6050_write_byte(uint8_t reg_addr, uint8_t data);
void MPU6050_write_data(uint8_t reg_addr_start, uint8_t *arr, uint8_t length);
void MPU6050_get_sensor_data(MPU6050_DATA *data);

#endif // __MPU6050_H
