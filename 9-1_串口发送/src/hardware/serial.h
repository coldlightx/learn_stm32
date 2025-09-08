#if !defined(__SERIAL_H)
#define __SERIAL_H

void serial_init();
void serial_send_byte(uint8_t byte);
void serial_send_array(uint8_t *arr, unsigned int length);
void serial_send_string(char *str);
void serial_send_number(int num);
void serial_printf(char *format, ...);

#endif // __SERIAL_H
