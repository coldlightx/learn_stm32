#if !defined(__LED_H)
#define __LED_H

void led_init();
void led_toggle(uint16_t GPIO_Pin);
void led1_toggle();
void led2_toggle();

#endif // __LED_H
