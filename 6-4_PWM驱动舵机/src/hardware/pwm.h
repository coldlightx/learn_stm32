#if !defined(__PWM_H)
#define __PWM_H

void pwm_init();
void pwm_set_compare(uint16_t compare);
unsigned int pwm_get_max_compare();

#endif // __PWM_H
