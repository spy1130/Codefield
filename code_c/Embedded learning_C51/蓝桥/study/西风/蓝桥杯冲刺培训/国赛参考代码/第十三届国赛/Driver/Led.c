#include <Led.h>

static unsigned char temp = 0x00;
static unsigned char temp_old = 0xff;
	
void Led_Disp(unsigned char addr,enable)
{
	static unsigned char temp_0 = 0x00;
	static unsigned char temp_old_0 = 0xff;
	if(enable)
		temp_0 |= 0x01 << addr;
	else
		temp_0 &= ~(0x01 << addr);
	if(temp_0 != temp_old_0)
	{
		P0 = ~temp_0;
		P2 = P2 & 0x1f | 0x80;
		P2 &= 0x1f;
		temp_old_0 = temp_0;
	}
}

void Beep(unsigned char flag)
{
	if(flag)
		temp |= 0x40;
	else
		temp &= ~0x40;
	if(temp != temp_old)
	{
		P0 = temp;
		P2 = P2 & 0x1f | 0xa0;
		P2 &= 0x1f;
		temp_old = temp;		
	}
}

void Relay(unsigned char flag)
{
	if(flag)
		temp |= 0x10;
	else
		temp &= ~0x10;
	if(temp != temp_old)
	{
		P0 = temp;
		P2 = P2 & 0x1f | 0xa0;
		P2 &= 0x1f;
		temp_old = temp;		
	}	
}

void Pulse(unsigned char flag)
{
	if(flag)
		temp |= 0x20;
	else
		temp &= ~0x20;
	if(temp != temp_old)
	{
		P0 = temp;
		P2 = P2 & 0x1f | 0xa0;
		P2 &= 0x1f;
		temp_old = temp;		
	}	
}