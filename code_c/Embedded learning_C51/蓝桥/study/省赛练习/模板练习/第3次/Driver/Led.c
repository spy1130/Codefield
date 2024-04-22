#include <Led.h>

void Led_Disp(u8 addr,enable)
{
	static u8 temp=0x00;
	static u8 temp_old=0xff;
	
	if(enable)
		temp |=0x01<<addr;
	else
		temp &=~(0x01<<addr);
	if(temp!=temp_old)
	{
		P0=~temp;
		P2=P2&0X1F|0X80;//y4
		P2 &=0X1F;
		
		temp_old=temp;
	}
		
}

void Beep(u8 enable)
{
	static u8 temp=0x00;
	static u8 temp_old=0xff;
	
	if(enable)
		temp |=0x40;
	else
		temp &=~(0x40);
	if(temp!=temp_old)
	{
		P0=temp;
		P2=P2&0X1F|0Xa0;//y5
		P2 &=0X1F;
		
		temp_old=temp;
	}
		
}

void Relay(u8 enable)
{
	static u8 temp=0x00;
	static u8 temp_old=0xff;
	
	if(enable)
		temp |=0x10;
	else
		temp &=~(0x10);
	if(temp!=temp_old)
	{
		P0=temp;
		P2=P2&0X1F|0Xa0;//y5
		P2 &=0X1F;
		
		temp_old=temp;
	}
		
}