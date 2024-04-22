#ifndef __RTC_H
#define __RTC_H

#include "./SYSTEM/sys/sys.h"


/* 时间结构体, 包括年月日周时分秒等信息 */
typedef struct
{
    uint8_t hour;       /* 时 */
    uint8_t min;        /* 分 */
    uint8_t sec;        /* 秒 */
    /* 公历年月日周 */
    uint16_t year;      /* 年 */
    uint8_t  month;     /* 月 */
    uint8_t  date;      /* 日 */
    uint8_t  week;      /* 周 */
} _calendar_obj;

extern _calendar_obj calendar;                      /* 时间结构体 */


uint8_t rtc_init(void);                             /* 初始化RTC */

uint8_t rtc_get_week(uint16_t year, uint8_t month, uint8_t day);
static uint8_t rtc_is_leap_year(uint16_t year);
static long rtc_date2sec(uint16_t syear, uint8_t smon, uint8_t sday, uint8_t hour, uint8_t min, uint8_t sec);
void rtc_get_time(void);
uint8_t rtc_set_time(uint16_t syear, uint8_t smon, uint8_t sday, uint8_t hour, uint8_t min, uint8_t sec);




#endif

















