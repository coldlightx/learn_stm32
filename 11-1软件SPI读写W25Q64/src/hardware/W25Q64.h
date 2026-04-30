#if !defined(__W25Q64_H)
#define __W25Q64_H
#define W25Q64_WRITE_ENABLE     0x06
#define W25Q64_READ_STATUS_REG1 0x05
#define W25Q64_PAGE_PROGRAM     0x02
#define W25Q64_SECTOR_ERASE     0x20
#define W25Q64_JEDEC_ID         0x9F
#define W25Q64_READ_DATA        0x03
#define W25Q64_EMPTY_DATA       0xFF

void W25Q64_init();
uint8_t W25Q64_read_status_reg1();
void W25Q64_page_program(uint32_t address, uint8_t *data, uint8_t length);
void W25Q64_sector_erase(uint32_t address);
uint32_t W25Q64_read_id();
void W25Q64_read_data(uint32_t address, uint8_t *data, uint32_t length);
#endif // __W25Q64_H
