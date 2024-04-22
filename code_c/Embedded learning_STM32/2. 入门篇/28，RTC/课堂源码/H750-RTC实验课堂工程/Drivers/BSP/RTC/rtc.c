#include "./BSP/RTC/rtc.h"
#include "./BSP/LED/led.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"

RTC_HandleTypeDef g_rtc_handle;  /* RTC¾ä±ú */

uint8_t rtc_init(void)
{
    RTC_TimeTypeDef rtc_time_init_struct;
    RTC_DateTypeDef rtc_date_init_struct;
    
    g_rtc_handle.Instance = RTC;
    g_rtc_handle.Init.HourFormat = RTC_HOURFORMAT_24;
    g_rtc_handle.Init.AsynchPrediv = 0x7F;
    g_rtc_handle.Init.SynchPrediv = 0xFF;
    g_rtc_handle.Init.OutPut = RTC_OUTPUT_DISABLE;     
    g_rtc_handle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    g_rtc_handle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
    HAL_RTC_Init(&g_rtc_handle);
    
    rtc_time_init_struct.Hours = 20;
    rtc_time_init_struct.Minutes = 20;
    rtc_time_init_struct.Seconds = 20;
    rtc_time_init_struct.TimeFormat = RTC_HOURFORMAT12_AM;
    rtc_time_init_struct.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    rtc_time_init_struct.StoreOperation = RTC_STOREOPERATION_RESET;
    HAL_RTC_SetTime(&g_rtc_handle, &rtc_time_init_struct, RTC_FORMAT_BIN);
    
    rtc_date_init_struct.Year = 8;
    rtc_date_init_struct.Month = 8;
    rtc_date_init_struct.Date = 8;
    rtc_date_init_struct.WeekDay = 5;
    HAL_RTC_SetDate(&g_rtc_handle, &rtc_date_init_struct, RTC_FORMAT_BIN);
}

void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
    RCC_OscInitTypeDef rcc_osc_init_handle;
    RCC_PeriphCLKInitTypeDef rcc_periphclk_init_handle;
    
    __HAL_RCC_RTC_CLK_ENABLE();
    HAL_PWR_EnableBkUpAccess();
    
    rcc_osc_init_handle.OscillatorType = RCC_OSCILLATORTYPE_LSE;
    rcc_osc_init_handle.LSEState = RCC_LSE_ON;
    rcc_osc_init_handle.PLL.PLLState = RCC_PLL_NONE;
    HAL_RCC_OscConfig(&rcc_osc_init_handle);
    
    rcc_periphclk_init_handle.PeriphClockSelection = RCC_PERIPHCLK_RTC;
    rcc_periphclk_init_handle.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
    HAL_RCCEx_PeriphCLKConfig(&rcc_periphclk_init_handle);
    
    __HAL_RCC_RTC_ENABLE();
}

