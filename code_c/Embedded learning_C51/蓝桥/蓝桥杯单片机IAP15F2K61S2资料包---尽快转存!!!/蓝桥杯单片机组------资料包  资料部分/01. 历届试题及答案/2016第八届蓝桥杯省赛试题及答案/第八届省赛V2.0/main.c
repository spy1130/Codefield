#include <reg51.h>
#include <key.c>
#include <xianshi.c>
#include <ds18b20.c>
uchar cp,cp1,cp2,fl;
bit led;
sbit P0_0 = P0^0;
void kongzhi_2003(uchar w,uchar ww)
{
	wr = 0;
	P2 = w;
	P0 = ww;
	P2 = 0x00;
	wr = 1;
}
void led_func(void)
{
	wr = 0;
	P2 = 0x80;
	P0 = 0xff;
	P0_0 = led;
	P2 = 0x00;
	wr = 1;}
void Timer0_isr(void) interrupt 1
{
	TH0 = (65536 - 2000) / 256;
	TL0 = (65536 - 2000) % 256;
	fa_song();
	cp++;
	if(cp>=100) led = ~led;
	if(cp>=250)
	{
		cp=0;
		cp1++;
		if(cp1>=2)
		{
			if(fl==1) cp2++;
			if(cp2>5) {cp2 = 0; fl = 0;} 
			sec++;
			flash = ~flash;
			if(sec>=60)
			{
				sec = 0;
				min++;
				if(min>=60)
				{
					min = 0;
					hour++;
					if(hour>=24) hour=0;
				}
			}
		}
	}

}
void Timer_init(void)
{
	TMOD = 0X01;
	TH0 = (65536 - 2000) / 256;
	TL0 = (65536 - 2000) % 256;
	EA = 1;
	ET0 = 1;
	TR0 = 1;
}
void main()
{
	kongzhi_2003(0xa0,0x00);
	kongzhi_2003(0x80,0xff);
	Timer_init();
	while(1)
	{
		temp = Read_Temperature();
		key_func();
		if((hour == hour_n)&&(min == min_n)&&(sec == sec_n)) fl = 1;
		if(fl == 1) led_func();
		else kongzhi_2003(0x80,0xff);	

	}	
}