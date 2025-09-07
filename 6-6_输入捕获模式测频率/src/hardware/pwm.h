#if !defined(__PWM_H)
#define __PWM_H

void pwm_init();
void pwm_set_compare(uint16_t compare);
unsigned int pwm_get_max_compare();
void pwm_set_frequency(unsigned int freq_khz);
void pwm_set_duty(unsigned int duty);

#endif // __PWM_H
