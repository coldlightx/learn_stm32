#if !defined(__ENCODER_SPEED)
#define __ENCODER_SPEED

void encoder_init();
int16_t encoder_get_position();
void encoder_reset_position();

#endif // __ENCODER_SPEED
