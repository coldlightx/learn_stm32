#include "stm32f10x.h"
#include "system/MyFlash.h"

#define MY_STORE_LENGTH 511
#define MY_STORE_KEY    0xAABB
uint16_t MyStore_cache[MY_STORE_LENGTH] = {0};

__IO uint16_t *flash_p = (__IO uint16_t *)0x0800FC00;

void MyStore_save()
{
    MyFlash_erase_page((uint32_t)flash_p);
    MyFlash_program_halfword_arr((uint32_t)flash_p, MyStore_cache, MY_STORE_LENGTH);
    MyFlash_program_halfword((uint32_t)(flash_p + MY_STORE_LENGTH), MY_STORE_KEY);
}

void MyStore_load_data()
{
    for (uint16_t i = 0; i < MY_STORE_LENGTH; i++) {
        MyStore_cache[i] = flash_p[i];
    }
}

void MyStore_init()
{
    if (flash_p[MY_STORE_LENGTH] != MY_STORE_KEY) {
        MyStore_save();
    } else {
        MyStore_load_data();
    }
}

void MyStore_clear()
{
    for (int i = 0; i < MY_STORE_LENGTH; i++) {
        flash_p[i] = MyStore_cache[i] = 0;
    }
}

uint16_t MyStore_get_key()
{
    return flash_p[MY_STORE_LENGTH];
}