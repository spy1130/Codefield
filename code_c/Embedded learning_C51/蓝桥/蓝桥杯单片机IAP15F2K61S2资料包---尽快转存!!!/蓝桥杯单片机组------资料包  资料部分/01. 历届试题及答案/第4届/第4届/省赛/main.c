#include "reg52.h"
#include "DS1302.h"
#include "LED.h"
#include "BUZZ.h"
#include "IIC.h"


sbit S7 = P3^0;
sbit S6 = P3^1;
sbit S5 = P3^2;
sbit S4 = P3^3;


bit S7_Flag = 0;
bit S6_Flag = 0;
bit S5_Flag = 0;
bit S4_Flag = 0;

bit Work_Flag = 0;  //0为自动工作状态，1为手动工作状态
bit DISPLAY_Flag = 0;  // 0为时钟页面， 1为阀度调整页面
bit BUZZ_Flag = 1;    // 0可以报警，1不报警

sbit LED1 = P0^0;
sbit LED2 = P0^1;

void Close_Buzz();
void Delay5ms();
void KEY_Press();

void main()
{
	int SHIDU = 50;        //shidu为AD转换器测出来的温度，SHIDU为EEPROM中设置的湿度阈值
	unsigned char shu;
	unsigned char yi,er,san,si,wu;
	unsigned char ii = 0;
	int shidu;
	Close_Buzz();
	Init_DS1302();
	Write_EEPROM(50);
	while(1)
	{
		KEY_Press();
		shu = PCF();
		shidu = shu*5/255*99/5;//
		SHIDU = Read_EEPROM();
		
		if (Work_Flag == 0) //自动工作状态,L1
		{
			P2 = 0x80;
			P0 = 0xFE;
			P2 = 0x00;	
			P0 = 0x00;
			
			Close_B();
			if (shidu < SHIDU)
			{
				Oppen_R();
			}
			else 
			{
				Close_R();
			}	
		}
		else   //手动工作状态,L2
		{
			P2 = 0x80;
			P0 = 0xFD;
			P2 = 0x00;
			P0 = 0x00;
		}
		
		if (S7_Flag == 1)
		{
			S7_Flag = 0;
			if (Work_Flag == 0)
			{
				Work_Flag = 1;
			}
			else
			{
				Work_Flag = 0;
			}
		}
		
		if ((S6_Flag == 1) && (Work_Flag == 0)) 
		{
			S6_Flag = 0;
			ii++;
			if(ii == 1)
			{
				DISPLAY_Flag = ~DISPLAY_Flag;
			}
			else
			{
				ii = 0;
				DISPLAY_Flag = ~DISPLAY_Flag;
			}
		}
		else if ((S6_Flag == 1) && (Work_Flag == 1))
		{
			S6_Flag = 0;
			BUZZ_Flag = ~BUZZ_Flag;		
		}
		
		if ((S5_Flag == 1) && (Work_Flag == 0) && (DISPLAY_Flag == 1))
		{
			S5_Flag = 0;
			SHIDU++;
			Write_EEPROM(SHIDU);
		}
		
		if ((S5_Flag == 1) && (Work_Flag == 1))
		{
			S5_Flag = 0;
			Oppen_R();
		}
		
		if ((S4_Flag == 1) && (Work_Flag == 0) && (DISPLAY_Flag == 1))
		{
			S4_Flag = 0;
			SHIDU--;
			Write_EEPROM(SHIDU);
		}
		
		if ((S4_Flag == 1) && (Work_Flag == 1))
		{
			S4_Flag = 0;
			Close_R();
		}			

		

		if ((shidu < SHIDU) && (BUZZ_Flag == 0) && (Work_Flag == 1))
		{
			Oppen_B();
		}
		
		if ((shidu >= SHIDU) || (BUZZ_Flag == 1) && (Work_Flag == 1))
		{
			Close_B();
		}	
		
		if (DISPLAY_Flag == 0)
		{
			Read_DS1302();		
			yi = Display_time[0]>>4;
			er = Display_time[0]&0x0F;
			san = 11;
			si = Display_time[1]>>4;
			wu = Display_time[1]&0x0F;
			DS1(yi, er, san, si, wu);		
			
			yi = shidu/10;
			er = shidu%10;
			DS2(10, yi, er);
		}
		else
		{
			SHIDU = Read_EEPROM();
			DS1(11, 11, 10, 10, 10);
			DS2(10, SHIDU/10, SHIDU%10);
		}		
	}
}


void Close_Buzz()
{
	P2 = 0xA0;
	P0 = 0x00;
	P2 = 0x00;
	

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

void KEY_Press()
{
	if (S7 == 0)
	{
		Delay5ms();
		if (S7 == 0)	
		{
			S7_Flag = 1;
		}
		while(S7 == 0);
	}
	
	if (S6 == 0)
	{
		Delay5ms();
		if (S6 == 0)	
		{
			S6_Flag = 1;
		}
		while(S6 == 0);
	}
	
	if (S5 == 0)
	{
		Delay5ms();
		if (S5 == 0)	
		{
			S5_Flag = 1;
		}
		while(S5 == 0);
	}
	
	if (S4 == 0)
	{
		Delay5ms();
		if (S4 == 0)	
		{
			S4_Flag = 1;
		}
		while(S4 == 0);
	}
}
