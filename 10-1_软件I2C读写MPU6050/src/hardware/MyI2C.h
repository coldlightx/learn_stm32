#if !defined(__MYI2C_H)
#define __MYI2C_H

void MyI2C_init();
void MyI2C_start();
void MyI2C_stop();
uint8_t MyI2C_send_byte(uint8_t byte);
uint8_t MyI2C_receive_ack();
void MyI2C_write_data_arr(uint8_t device_addr, uint8_t reg_addr_start, uint8_t *arr, uint8_t length);
void MyI2C_write_data_byte(uint8_t device_addr, uint8_t reg_addr, uint8_t data);
void MyI2C_read_data_arr(uint8_t device_addr, uint8_t reg_addr_start, uint8_t *arr, uint8_t length);
uint8_t MyI2C_read_data_byte(uint8_t device_addr, uint8_t reg_addr);

#endif // __MYI2C_H
