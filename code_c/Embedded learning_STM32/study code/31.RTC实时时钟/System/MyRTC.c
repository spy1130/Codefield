#include "stm32f10x.h"                  // Device header
#include <time.h>

uint16_t MyRTC_Time[] = {2024, 2, 1, 16, 0, 0};

void MyRTC_SetTime(void);
//RTC����Ҫʹ�ܣ����ú��Զ�����
void MyRTC_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);//����PWRʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);//����BKPʱ��
	
	PWR_BackupAccessCmd(ENABLE);//ʹ�ܶ�BKP��RTC�ķ���
	
	if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)//�ж�BKP�Ƿ�ϵ磬��������������
	{
		BKP_DeInit();
		//����LSEʱ�ӣ����ȴ�LSEʱ���������
		RCC_LSEConfig(RCC_LSE_ON);//�����ⲿLES����
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);//�ȴ�LSE�򿪱�־λ
		//ѡ��RTCCLKʱ��Դ
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);//ѡ��L S E���� ʱ��Դ �ⲿ 
		RCC_RTCCLKCmd(ENABLE);//ʹ��ʱ��
		//���������ȴ��ĺ�������ֹʱ�Ӳ�ͬ������bug
		RTC_WaitForSynchro();//�ȴ�ͬ��
		RTC_WaitForLastTask();//�ȴ���һ�β������
		//����Ԥ��Ƶ��
		RTC_SetPrescaler(32768-1);//LSE��32.768KHz,��Ƶ�����1hz�������Ѿ��Զ�����CNFλ��ʹRTC��������ģʽ����������˳�
		RTC_WaitForLastTask();//��д���ȴ��������
		//����ʱ��
		MyRTC_SetTime();
		
		BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);//����DR1�ڴ�Ϊ��־λ��˵����ʼ���������ұ��õ�Դû�ϵ�
	}
	else
	{ //��ֹ����
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
	
	time_cnt = mktime(&time_date) - 8 * 60 * 60;//����ʱ��ת������������ȥ8h������׶�ʱ�䣬����Unixʱ������������
	
	RTC_SetCounter(time_cnt);//�����趨��ʼʱ��
	RTC_WaitForLastTask();//��д���ȴ��������
}

void MyRTC_ReadTime(void)
{
	time_t time_cnt;
	struct tm time_date;
	
	time_cnt = RTC_GetCounter() + 8 * 60 * 60;//��ȡʱ���+8h����ɱ���ʱ��������Unixʱ������������
	
	time_date = *localtime(&time_cnt);//ת���������ո�ʽ�������ṹ��
	
	MyRTC_Time[0] = time_date.tm_year + 1900;
	MyRTC_Time[1] = time_date.tm_mon + 1;
	MyRTC_Time[2] = time_date.tm_mday;
	MyRTC_Time[3] = time_date.tm_hour;
	MyRTC_Time[4] = time_date.tm_min;
	MyRTC_Time[5] = time_date.tm_sec;
}

