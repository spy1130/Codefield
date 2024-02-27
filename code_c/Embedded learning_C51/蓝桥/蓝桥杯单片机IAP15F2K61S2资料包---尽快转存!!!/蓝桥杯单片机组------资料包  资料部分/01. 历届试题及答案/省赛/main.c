#include <STC15F2K60S2.h>
#include "ds1302.h"
#include "onewire.h"
#include "DS.h"

sbit S7 = P3^0;    //����ɼ�ʱ������ҳ��
sbit S6 = P3^1;		 //
sbit S5 = P3^2;		 //ȷ�ϲɼ�ʱ�䣬�˳��ɼ�ʱ������ҳ��
sbit S4 = P3^3;    //�л��¶Ȳɼ����ʱ��

bit s7_flag = 0;
bit s6_flag = 0;
bit s5_flag = 0;
bit s4_flag = 0;

bit L1_flag = 0; //0ʱL1����˸�� 1��˸
bit tt3_flag = 0;
bit tt2_flag = 0;

bit f;
unsigned char ChaTime = 0, tempCha = 0; 
unsigned char Dsplaywei = 1; 	
unsigned char tt = 0, tt1 = 0, tt2 = 0, tt3 = 0;
unsigned char Temperature[10];
unsigned char cha[4] = {1, 5, 30, 60};
void Delay5ms();
void Keypress();
void Timer0Init(void);
void Close_BUZZ();
void Timer1Init(void);		//10����@11.0592MHz
void main()
{
   //ѡ���¶ȼ��ʱ����
 //1Ϊ�¶�����ҳ�棬2Ϊʱ��ҳ�棬3Ϊ��ʾҳ��

	unsigned char tmperwei = 0;
	Close_BUZZ();
	Timer0Init();
	Init_DS1302();
	Timer1Init();

	while(1)
	{
		Keypress();

		if ((L1_flag == 1) && (tt3_flag))
		{
			P0 = 0xFF;
			P2 = 0x80;
			if (f == 0)
			{
				P00 = 1;
				f = ~f;
			}
			else
			{
				P00 = 0;	
				f = ~f;				
			}	
			P2 = 0x00;
		}
		
		
		if (s7_flag == 1)  
		{
			s7_flag = 0;
			
			if (Dsplaywei == 3)
			{
				Dsplaywei = 1;
			}		
		}
		
		if (s6_flag == 1)
		{
			s6_flag = 0;
			if (Dsplaywei == 3)
			{
				if (L1_flag == 1)
				{
					L1_flag = 0;
				}
				
				tmperwei++; 
				if (tmperwei == 10)
				{
					tmperwei = 0;
				}
			}
		}
		
		if (s5_flag == 1)
		{
			s5_flag = 0;
			if (Dsplaywei == 1)
			{
				Dsplaywei = 2;	
				ChaTime = tempCha;
				TR0 = 1;
				Temperature[tt2] = rd_temperature();
			}
		}

		if (s4_flag == 1)
		{
			s4_flag = 0;
			
			if (Dsplaywei == 1)
			{
				tempCha++;	
				if (tempCha == 4)
				{
					tempCha = 0;
				}	
			}	
		}	
		
		if (tt2_flag == 1)
		{
			tt2_flag = 0;
			Temperature[tt2] = rd_temperature();
			Dsplaywei = 3;
			L1_flag = 1;
		}
		
		if (Dsplaywei == 1)
		{
			DS1(10,10);
			DS2(10);
			DS3(10,10);
			DS4(11);
			DS5(cha[tempCha]/10,cha[tempCha]%10);
		}
		
		if (Dsplaywei == 2)
		{
			Read_DS1302();
			DS1(DisplayTime[0]>>4,DisplayTime[0]&0x0F);
			if (tt3_flag)
			{
				DS2(11);
				DS4(11);
			}
			else
			{
				DS2(10);
				DS4(10);
			}
			DS3(DisplayTime[1]>>4,DisplayTime[1]&0x0F);	
			DS5(DisplayTime[2]>>4,DisplayTime[2]&0x0F);
		}
		
		if (Dsplaywei == 3)
		{
			DS1(11,0);
			DS2(tmperwei);
			DS3(10,10);
			DS4(11);
			DS5(Temperature[tmperwei]/10,Temperature[tmperwei]%10);
		}		
	}
}

void Close_BUZZ()
{
	P2 = 0xA0;
	P0 = 0x00;
	P2 = 0x80;
	P0 = 0xFF;
	P2 = 0x00;
}

void Delay5ms()		//@11.0592MHz
{
	unsigned char i, j;

	i = 54;
	j = 199;
	do
	{
		while (--j);
	} while (--i);
}

void Keypress()
{
	if (S7 == 0)
	{
		Delay5ms();
		if (S7 == 0)
		{
			s7_flag = 1;
		}
		while(S7 == 0);
	}
	
	if (S6 == 0)
	{
		Delay5ms();
		if (S6 == 0)
		{
			s6_flag = 1;
		}
		while(S6 == 0);
	}
	
	if (S5 == 0)
	{
		Delay5ms();
		if (S5 == 0)
		{
			s5_flag = 1;
		}
		while(S5 == 0);
	}	
		
	if (S4 == 0)
	{
		Delay5ms();
		if (S4 == 0)
		{
			s4_flag = 1;
		}
		while(S4 == 0);
	}
}

void Timer0Init(void)		//10����@11.0592MHz 
{
	AUXR &= 0x7F;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TMOD |= 0x01;		//���ö�ʱ��ģʽ
	TL0 = 0x00;		//���ö�ʱ��ֵ
	TH0 = 0xDC;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	
	ET0 = 1;
	EA = 1;
}

void Timer0interrupt() interrupt 1
{
	if (++tt == 100) //1��
	{
		tt = 0;
		if (++tt1 == cha[ChaTime])
		{
			tt1 = 0;
			tt2_flag = 1;
			tt2++;
			if (tt2 == 9)
			{
				TL0 = 0x00;		//���ö�ʱ��ֵ
				TH0 = 0xDC;		//���ö�ʱ��ֵ
				TR0 = 0;
				tt2 = 0;
			}
			else
			{
				TL0 = 0x00;		//���ö�ʱ��ֵ
				TH0 = 0xDC;		//���ö�ʱ��ֵ				
			}
		}
	}
	TF0 = 0;
}


void Timer1Init(void)		//10����@11.0592MHz
{
	AUXR &= 0xBF;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TL1 = 0x00;		//���ö�ʱ��ֵ
	TH1 = 0xDC;		//���ö�ʱ��ֵ
	TF1 = 0;		//���TF1��־
	TR1 = 1;		//��ʱ��1��ʼ��ʱ
	
	ET1 = 1;
	EA = 1;
}

void Timer1interrupt() interrupt 3
{
	if (++tt3 == 100) //1��
	{

		
		tt3_flag = ~tt3_flag;
		
	}
}

