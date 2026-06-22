#include <time.h>
#include "stm32f10x.h"
#include "RTC.h"

void MyRTC_init()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);
    PWR_BackupAccessCmd(ENABLE);

    RCC_LSEConfig(RCC_LSE_ON);
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
    RCC_RTCCLKCmd(ENABLE);

    RTC_WaitForSynchro();
    RTC_WaitForLastTask();

    if (BKP_ReadBackupRegister(BKP_DR1) != 0xAA) {
        RTC_SetPrescaler(32768 - 1);
        RTC_WaitForLastTask();

        RTC_SetCounter(1780847995);
        RTC_WaitForLastTask();
        BKP_WriteBackupRegister(BKP_DR1, 0xAA);
    }
}

uint32_t MyRTC_time_to_clock(Time time_)
{
    time_.tm_year -= 1900;
    time_.tm_mon -= 1;

    return mktime(&time_) - 8 * 60 * 60;
}
void MyRTC_set_time(Time *time_)
{
    RTC_SetCounter(MyRTC_time_to_clock(*time_));
    RTC_WaitForLastTask();
}

void MyRTC_set_alarm(time_t time)
{
    RTC_SetAlarm(time);
    RTC_WaitForLastTask();
}

void MyRTC_get_time(Time *time_)
{
    time_t rtc_time;
    rtc_time = RTC_GetCounter() + 8 * 60 * 60;
    *time_   = *localtime(&rtc_time);

    time_->tm_year += 1900;
    time_->tm_mon += 1;
}
