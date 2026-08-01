#include "stm32f10x.h"

uint32_t MyFlash_read_word(uint32_t address)
{
    return *((__IO uint32_t *)address);
}

uint16_t MyFlash_read_halfword(uint32_t address)
{
    return *((__IO uint16_t *)address);
}

uint8_t MyFlash_read_byte(uint32_t address)
{
    return *((__IO uint8_t *)address);
}

void MyFlash_erase_all()
{
    FLASH_Unlock();
    FLASH_EraseAllPages();
    FLASH_Lock();
}

void MyFlash_erase_page(uint32_t address)
{
    FLASH_Unlock();
    FLASH_ErasePage(address);
    FLASH_Lock();
}

void MyFlash_program_word(uint32_t address, uint32_t data)
{
    FLASH_Unlock();
    FLASH_ProgramWord(address, data);
    FLASH_Lock();
}

void MyFlash_program_halfword(uint32_t address, uint16_t data)
{
    FLASH_Unlock();
    FLASH_ProgramHalfWord(address, data);
    FLASH_Lock();
}

void MyFlash_program_halfword_arr(uint32_t address, uint16_t *arr, uint32_t length)
{
    FLASH_Unlock();
    for (uint32_t i = 0; i < length; i++) {
        FLASH_ProgramHalfWord(address + i * 2, arr[i]);
    }
    FLASH_Lock();
}