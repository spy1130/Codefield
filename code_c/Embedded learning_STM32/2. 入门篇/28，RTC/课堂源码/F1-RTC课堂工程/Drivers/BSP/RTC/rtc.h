#ifndef __RTC_H
#define __RTC_H

#include "./SYSTEM/sys/sys.h"


/* ʱ��ṹ��, ������������ʱ�������Ϣ */
typedef struct
{
    uint8_t hour;       /* ʱ */
    uint8_t min;        /* �� */
    uint8_t sec;        /* �� */
    /* ������������ */
    uint16_t year;      /* �� */
    uint8_t  month;     /* �� */
    uint8_t  date;      /* �� */
    uint8_t  week;      /* �� */
} _calendar_obj;

extern _calendar_obj calendar;                      /* ʱ��ṹ�� */


uint8_t rtc_init(void);                             /* ��ʼ��RTC */

uint8_t rtc_get_week(uint16_t year, uint8_t month, uint8_t day);
static uint8_t rtc_is_leap_year(uint16_t year);
static long rtc_date2sec(uint16_t syear, uint8_t smon, uint8_t sday, uint8_t hour, uint8_t min, uint8_t sec);
void rtc_get_time(void);
uint8_t rtc_set_time(uint16_t syear, uint8_t smon, uint8_t sday, uint8_t hour, uint8_t min, uint8_t sec);




#endif

















