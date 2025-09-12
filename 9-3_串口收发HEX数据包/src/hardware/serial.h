#if !defined(__SERIAL_H)
#define __SERIAL_H

void serial_init();
void serial_send_byte(uint8_t byte);
void serial_send_array(uint8_t *arr, unsigned int length);
void serial_send_string(char *str);
void serial_send_number(int num);
void serial_printf(char *format, ...);
uint8_t serial_get_rx_flag();
void serial_get_rx_data(uint8_t *arr);
void serial_clear_rx_flag();
void serial_send_hex_data(uint8_t *data);

typedef enum {
    WATING_FOR_START,
    RECEIVING_DATA,
    RECEIVE_COMPLETE
} SerialStatus;

#endif // __SERIAL_H
