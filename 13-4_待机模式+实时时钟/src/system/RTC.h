#if !defined(__RTC_H)
#define __RTC_H
#include <time.h>

typedef struct tm Time;

void MyRTC_init();
uint32_t MyRTC_time_to_clock(Time time_);
void MyRTC_set_time(Time *time_);
void MyRTC_get_time(Time *time_);
void MyRTC_set_alarm(time_t time);

#endif // __RTC_H
