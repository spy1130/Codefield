#include <stc15f2k60s2.h>
unsigned char code smgchar[12]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xbf,0xff};
unsigned char smgbuff[8]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
void smg_ds18b20(unsigned char temp,unsigned char THC,unsigned char TLC)
{
	smgbuff[0]=smgchar[temp%10];
	smgbuff[1]=smgchar[temp/10%10];
	smgbuff[2]=smgchar[11];
	smgbuff[3]=smgchar[11];
	smgbuff[4]=smgchar[TLC%10];
	smgbuff[5]=smgchar[TLC/10%10];
	smgbuff[6]=smgchar[THC%10];
	smgbuff[7]=smgchar[THC/10%10];
}
void Time_T0(unsigned char ms)
{
	unsigned int temp;
	TMOD&=0xf0;
	TMOD|=0x00;
	temp=11059200/12/1000;
	temp=65536-temp*ms;
	TH0=temp/256;
	TL0=temp%256;
	EA=1;
	ET0=1;
	TR0=1;
}
void smg_buff()
{
	static unsigned char i,cnt=0x80;
	P0=0x00;
	P2&=0x1f;
	P2|=0xc0;
	P2&=0x1f;
	P0=smgbuff[i];
	P2|=0xe0;
	P2&=0x1f;
	P0=cnt>>i;
	P2|=0xc0;
	P2&=0x1f;
	i++;
	if(i==8)
	i=0;	
}