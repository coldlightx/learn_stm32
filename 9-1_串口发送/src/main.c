#include <stm32f10x.h>
#include <stdio.h>
#include "system/Delay.h"
#include "hardware/OLED.h"
#include "hardware/serial.h"

int main(int argc, char const *argv[])
{
    OLED_Init();
    serial_init();

    serial_send_byte('A');

    uint8_t arr[2] = {0x41, 0x42};
    serial_send_array(arr, 2);
    serial_send_string("\nHello, World!\n");
    serial_send_number(12345);
    printf("\nNumber=%d\n", 666);
    serial_printf("Number=%d\n", 888);

    while (1) {
    }

    return 0;
}
