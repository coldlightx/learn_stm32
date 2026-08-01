#if !defined(__MYSPI_H)
#define __MYSPI_H
void MySPI_init();
void MySPI_start();
void MySPI_stop();
uint8_t MySPI_swap_byte(uint8_t byte);
void MySPI_fast_swap(uint8_t *data, uint32_t length);
void MySPI_fast_send(uint8_t *data, uint32_t length);
uint8_t MySPI_fast_receive(uint8_t *data, uint32_t length);
#endif // __MYSPI_H
