#if !defined(__MYSPI_H)
#define __MYSPI_H
void MySPI_init();
void MySPI_start();
void MySPI_stop();
uint8_t MySPI_swap_byte(uint8_t byte);
#endif // __MYSPI_H
