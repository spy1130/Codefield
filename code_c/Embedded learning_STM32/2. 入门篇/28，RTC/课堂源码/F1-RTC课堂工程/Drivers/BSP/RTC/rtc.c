#include "./BSP/RTC/rtc.h"
#include "./BSP/LED/led.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"


RTC_HandleTypeDef g_rtc_handle; /* RTC���ƾ�� */
_calendar_obj calendar;         /* ʱ��ṹ�� */

/**
 * @brief       RTCд�������SRAM
 * @param       bkrx : �����Ĵ������,��Χ:0~41
                        ��Ӧ RTC_BKP_DR1~RTC_BKP_DR42
 * @param       data : Ҫд�������,16λ����
 * @retval      ��
 */
void rtc_write_bkr(uint32_t bkrx, uint16_t data)
{
    HAL_PWR_EnableBkUpAccess(); /* ȡ��������д���� */
    HAL_RTCEx_BKUPWrite(&g_rtc_handle, bkrx + 1, data);
}

/**
 * @brief       RTC��ȡ������SRAM
 * @param       bkrx : �����Ĵ������,��Χ:0~41
                ��Ӧ RTC_BKP_DR1~RTC_BKP_DR42
 * @retval      ��ȡ����ֵ
 */
uint16_t rtc_read_bkr(uint32_t bkrx)
{
    uint32_t temp = 0;
    temp = HAL_RTCEx_BKUPRead(&g_rtc_handle, bkrx + 1);
    return (uint16_t)temp; /* ���ض�ȡ����ֵ */
}

uint8_t rtc_set_time(uint16_t syear, uint8_t smon, uint8_t sday, uint8_t hour, uint8_t min, uint8_t sec)
{
    uint32_t seccount = 0;
    
    seccount = rtc_date2sec( syear,  smon,  sday,  hour,  min,  sec);

    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_RCC_BKP_CLK_ENABLE();
    HAL_PWR_EnableBkUpAccess();
    
    RTC->CRL |= 1 << 4;
    
    RTC->CNTL = seccount & 0xFFFF;
    RTC->CNTH = seccount >> 16;
    
    RTC->CRL &= ~(1 << 4);
    
    while(!__HAL_RTC_ALARM_GET_FLAG(&g_rtc_handle, RTC_FLAG_RTOFF));
    
    return 0;
}

void rtc_get_time(void)
{
    static uint16_t daycnt = 0;
    uint32_t seccount = 0;
    uint32_t temp = 0;
    uint16_t temp1 = 0;
    const uint8_t month_table[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};   /* ƽ����·����ڱ� */
    
    seccount = RTC->CNTH;
    seccount <<= 16;
    seccount += RTC->CNTL;
    
    temp = seccount / 86400;    /* �õ�����(��������Ӧ��) */

    if (daycnt != temp)         /* ����һ���� */
    {
        daycnt = temp;
        temp1 = 1970;           /* ��1970�꿪ʼ */

        while (temp >= 365)
        {
            if (rtc_is_leap_year(temp1))    /* ������ */
            {
                if (temp >= 366)
                {
                    temp -= 366;    /* ����������� */
                }
                else
                {
                    break;
                }
            }
            else
            {
                temp -= 365;    /* ƽ�� */
            }
            
            temp1++;
        }

        calendar.year = temp1;  /* �õ���� */
        temp1 = 0;

        while (temp >= 28)      /* ������һ���� */
        {
            if (rtc_is_leap_year(calendar.year) && temp1 == 1)  /* �����ǲ�������/2�·� */
            {
                if (temp >= 29)
                {
                    temp -= 29; /* ����������� */
                }
                else
                {
                    break;
                }
            }
            else
            {
                if (temp >= month_table[temp1])
                {
                    temp -= month_table[temp1]; /* ƽ�� */
                }
                else
                {
                    break;
                }
            }

            temp1++;
        }

        calendar.month = temp1 + 1;     /* �õ��·� */
        calendar.date = temp + 1;       /* �õ����� */
    }

    temp = seccount % 86400;            /* �õ������� */
    calendar.hour = temp / 3600;        /* Сʱ */
    calendar.min = (temp % 3600) / 60;  /* ���� */
    calendar.sec = (temp % 3600) % 60;  /* ���� */
    calendar.week = rtc_get_week(calendar.year, calendar.month, calendar.date); /* ��ȡ���� */

}


uint8_t rtc_init(void)
{
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_RCC_BKP_CLK_ENABLE();
    HAL_PWR_EnableBkUpAccess();
    
    g_rtc_handle.Instance = RTC;
    g_rtc_handle.Init.AsynchPrediv = 32767;
    g_rtc_handle.Init.OutPut = RTC_OUTPUTSOURCE_NONE;
    HAL_RTC_Init(&g_rtc_handle);
    
    if (rtc_read_bkr(0) != 0x8888)
    {
        rtc_set_time(2088, 8, 8, 8, 8, 8);
        rtc_write_bkr(0, 0x8888);
    }
    
    return 0;
}

void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
    __HAL_RCC_RTC_ENABLE();
    
    RCC_OscInitTypeDef rcc_oscinitstruct;
    RCC_PeriphCLKInitTypeDef rcc_periphclkinitstruct;
    
    rcc_oscinitstruct.OscillatorType = RCC_OSCILLATORTYPE_LSE;
    rcc_oscinitstruct.LSEState = RCC_LSE_ON;
    rcc_oscinitstruct.PLL.PLLState = RCC_PLL_NONE;
    HAL_RCC_OscConfig(&rcc_oscinitstruct);
    
    rcc_periphclkinitstruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
    rcc_periphclkinitstruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
    HAL_RCCEx_PeriphCLKConfig(&rcc_periphclkinitstruct);

}

/**
 * @brief       ��������ʱ����ת����������
 *   @note      ��1970��1��1��Ϊ��׼, 1970��1��1��, 0ʱ0��0��, ��ʾ��0����
 *              ����ʾ��2105��, ��Ϊuint32_t����ʾ136���������(����������)!
 *              ������ο�ֻlinux mktime����, ԭ��˵��������:
 *              http://www.openedv.com/thread-63389-1-1.html
 * @param       syear : ���
 * @param       smon  : �·�
 * @param       sday  : ����
 * @param       hour  : Сʱ
 * @param       min   : ����
 * @param       sec   : ����
 * @retval      ת�����������
 */
static long rtc_date2sec(uint16_t syear, uint8_t smon, uint8_t sday, uint8_t hour, uint8_t min, uint8_t sec)
{
    uint32_t Y, M, D, X, T;
    signed char monx = smon; /* ���·�ת���ɴ����ŵ�ֵ, ����������� */

    if (0 >= (monx -= 2))    /* 1..12 -> 11,12,1..10 */
    {
        monx += 12;          /* Puts Feb last since it has leap day */
        syear -= 1;
    }

    Y = (syear - 1) * 365 + syear / 4 - syear / 100 + syear / 400; /* ��ԪԪ��1�����ڵ������� */
    M = 367 * monx / 12 - 30 + 59;
    D = sday - 1;
    X = Y + M + D - 719162;                      /* ��ȥ��ԪԪ�굽1970������� */
    T = ((X * 24 + hour) * 60 + min) * 60 + sec; /* �������� */
    return T;
}

/**
 * @brief       �ж�����Ƿ�������
 *   @note      �·�������:
 *              �·�   1  2  3  4  5  6  7  8  9  10 11 12
 *              ����   31 29 31 30 31 30 31 31 30 31 30 31
 *              ������ 31 28 31 30 31 30 31 31 30 31 30 31
 * @param       year : ���
 * @retval      0, ������; 1, ������;
 */
static uint8_t rtc_is_leap_year(uint16_t year)
{
    /* �������: ��������겻���İ������� */
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief       ��������ʱ����ת����������
 *   @note      ���빫�����ڵõ�����(��ʼʱ��Ϊ: ��Ԫ0��3��1�տ�ʼ, ����������κ�����, �����Ի�ȡ��ȷ������)
 *              ʹ�� ��ķ����ɭ���㹫ʽ ����, ԭ��˵��������:
 *              https://www.cnblogs.com/fengbohello/p/3264300.html
 * @param       syear : ���
 * @param       smon  : �·�
 * @param       sday  : ����
 * @retval      0, ������; 1 ~ 6: ����һ ~ ������
 */
uint8_t rtc_get_week(uint16_t year, uint8_t month, uint8_t day)
{
    uint8_t week = 0;

    if (month < 3)
    {
        month += 12;
        --year;
    }

    week = (day + 1 + 2 * month + 3 * (month + 1) / 5 + year + (year >> 2) - year / 100 + year / 400) % 7;
    return week;
}


