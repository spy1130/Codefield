#include <stc15f2k60s2.h>
#include <intrins.h>
sbit DQ=P1^4;
void Delay10us(unsigned char us)
{
	unsigned char i;
	while(--us)
	 {
	 	i=24;
	 	while(--i);
		};
}
bit intds18b20()
{
	bit t;
	EA=0;
	DQ=1;
	DQ=0;
	Delay10us(50);
	DQ=1;
	Delay10us(12);
	if(DQ==0)
	t=1;
	while(!DQ);
	EA=1;
	return t;			
}
void DQwritebyte(unsigned char dat)
{
	unsigned char i;
	EA=0;
	for(i=0;i<8;i++)
	{
		DQ=0;
		if(dat&0x01)
		DQ=1;
		else
		DQ=0;
		Delay10us(6);
		DQ=1;
		dat>>=1;			
	}
	EA=1;
}
unsigned char DQreadbyte()
{
	unsigned char i,dat;
	EA=0;
	for(i=0;i<8;i++)
	{
		DQ=0;
		dat>>=1;
		DQ=1;
		Delay10us(2);
		if(DQ)
		dat|=0x80;
		else
		dat&=0x7f;
		Delay10us(4);			
	}
	EA=1;
	return dat;
}
unsigned char read_ds18b20()
{
	unsigned char high,low,temp;
	intds18b20();
	DQwritebyte(0xcc);
	DQwritebyte(0x44);
	Delay10us(50);
	Delay10us(50);
	intds18b20();
	DQwritebyte(0xcc);
	DQwritebyte(0xbe);
	low=DQreadbyte();
	high=DQreadbyte();
	temp=((int)(high<<8)+low)*0.0625;
	return temp;	
}

