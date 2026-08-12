#if !defined(__MYFLASH)
#define __MYFLASH

uint32_t MyFlash_read_word(uint32_t address);
uint16_t MyFlash_read_halfword(uint32_t address);
uint8_t MyFlash_read_byte(uint32_t address);
void MyFlash_erase_all();
void MyFlash_erase_page(uint32_t address);
void MyFlash_program_word(uint32_t address, uint32_t data);
void MyFlash_program_halfword(uint32_t address, uint16_t data);
void MyFlash_program_halfword_arr(uint32_t address, uint16_t *arr, uint32_t length);
#endif // __MYFLASH
