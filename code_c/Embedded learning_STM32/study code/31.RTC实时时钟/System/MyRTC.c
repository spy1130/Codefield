#include "stm32f10x.h"                  // Device header
#include <time.h>

uint16_t MyRTC_Time[] = {2024, 2, 1, 16, 0, 0};

void MyRTC_SetTime(void);
//RTC不需要使能，配置好自动开启
void MyRTC_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);//开启PWR时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);//开启BKP时钟
	
	PWR_BackupAccessCmd(ENABLE);//使能对BKP和RTC的访问
	
	if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)//判断BKP是否断电，后面是随便给的数
	{
		BKP_DeInit();
		//开启LSE时钟，并等待LSE时钟启动完成
		RCC_LSEConfig(RCC_LSE_ON);//启动外部LES晶振
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);//等待LSE打开标志位
		//选择RTCCLK时钟源
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);//选择L S E低速 时钟源 外部 
		RCC_RTCCLKCmd(ENABLE);//使能时钟
		//调用两个等待的函数，防止时钟不同步出现bug
		RTC_WaitForSynchro();//等待同步
		RTC_WaitForLastTask();//等待上一次操作完成
		//配置预分频器
		RTC_SetPrescaler(32768-1);//LSE：32.768KHz,分频后就是1hz，这里已经自动配置CNF位，使RTC进入配置模式，并且最后退出
		RTC_WaitForLastTask();//再写入后等待操作完成
		//设置时间
		MyRTC_SetTime();
		
		BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);//更新DR1内存为标志位，说明初始化过，并且备用电源没断电
	}
	else
	{ //防止意外
		RTC_WaitForSynchro();
		RTC_WaitForLastTask();
	}
}



void MyRTC_SetTime(void)
{
	time_t time_cnt;
	struct tm time_date;
	
	time_date.tm_year = MyRTC_Time[0] - 1900;
	time_date.tm_mon = MyRTC_Time[1] - 1;
	time_date.tm_mday = MyRTC_Time[2];
	time_date.tm_hour = MyRTC_Time[3];
	time_date.tm_min = MyRTC_Time[4];
	time_date.tm_sec = MyRTC_Time[5];
	
	time_cnt = mktime(&time_date) - 8 * 60 * 60;//日期时间转换成秒数，减去8h，变成伦敦时间，传入Unix时间戳计算机变量
	
	RTC_SetCounter(time_cnt);//用于设定初始时间
	RTC_WaitForLastTask();//再写入后等待操作完成
}

void MyRTC_ReadTime(void)
{
	time_t time_cnt;
	struct tm time_date;
	
	time_cnt = RTC_GetCounter() + 8 * 60 * 60;//读取时间戳+8h，变成北京时区，传出Unix时间戳计算机变量
	
	time_date = *localtime(&time_cnt);//转换成年月日格式，传出结构体
	
	MyRTC_Time[0] = time_date.tm_year + 1900;
	MyRTC_Time[1] = time_date.tm_mon + 1;
	MyRTC_Time[2] = time_date.tm_mday;
	MyRTC_Time[3] = time_date.tm_hour;
	MyRTC_Time[4] = time_date.tm_min;
	MyRTC_Time[5] = time_date.tm_sec;
}

