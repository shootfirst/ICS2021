#include <am.h>
#include <nemu.h>
//*****************************pa2*********************************
// #include <klib.h>
//*****************************pa2*********************************

//*****************************pa2*********************************
static uint64_t system_start_time;
//*****************************pa2*********************************

void __am_timer_init() {
  //*****************************pa2*********************************
  // 获取系统启动时间
  system_start_time = ( (uint64_t)inl(RTC_ADDR + 4) << 32 ) + (uint64_t)inl(RTC_ADDR);
  //*****************************pa2*********************************
}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) {
  //*****************************pa2*********************************
  // uptime->us = 0;
  // 从地址为RTC_ADDR处读取时间
  // 小端序
  uptime->us = ( ( (uint64_t)inl(RTC_ADDR + 4) << 32 ) + (uint64_t)inl(RTC_ADDR) ) - system_start_time;
  // printf("time %d\n", (int)uptime->us);
  //*****************************pa2*********************************
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  rtc->minute = 0;
  rtc->hour   = 0;
  rtc->day    = 0;
  rtc->month  = 0;
  rtc->year   = 1900;
}
