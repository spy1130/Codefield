#include "STC15F2K60S2.h"
#include "init.h"
#include "Delay.h"
#include "LED.h"
#include "Beer.h"
#include "Relay.h"



void main()
{
	unsigned char i=0;
	init_close();
	while(1){
	for(i = 0; i < 3; i++)
	{
		LED(0XFF);
		Delay(1000);
		LED(0X00);
		Delay(1000);
	}
	
	for(i =1;i<=8;i++)
	{
		LED(~(0xFF<<i));
		Delay(100);
	}
	Relay(200);
	for(i =1;i<=8;i++)
	{
		LED(0xFF<<i);
		Delay(100);
	}
	Beer(200);
	}
}