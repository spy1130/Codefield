#include <STC15F2K60S2.H>
#include "ds1302.h"
#include "decoder_138.h"
#include "delay.h"

unsigned char table[11] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90 ,0XBF};
unsigned char pucRtc[3]	= {23,45,6};//时 分 秒
unsigned char write_address[3]	= {0x84,0x82,0x80};//时 分 秒
unsigned char read_address[3]	= {0x85,0x83,0x81};
void dushu(unsigned char*pucRtc);
void SMG_display(unsigned char location,unsigned char num)
{
	control_138decoder(6);
	P0=1<<location;
	control_138decoder(7);
	P0=table[num];
	control_138decoder(0);
	delay_ms(2);
}
void Set_Rtc(unsigned char* pucRtc,unsigned char*write_address)
{
	unsigned char i,temp;
	write_ds1302_byte(0x8e,0);//WP置0关闭写保护
	for(i=0;i<3;i++)
	{
		temp=((pucRtc[i]/10)<<4)+pucRtc[i]%10;
		write_ds1302_byte(write_address[i],temp);
	}
	write_ds1302_byte(0x8e,0x80);//WP置1开启写保护
}
void Read_Rtc(unsigned char*pucRtc,unsigned char*read_address)
{
	unsigned char i,temp;
	for(i=0;i<3;i++)
	{
		temp=read_ds1302_byte(read_address[i]);
		pucRtc[i]=(temp>>4)*10+(temp&0xf);
		dushu(pucRtc);
	}
}
void dushu(unsigned char*pucRtc)
{
	SMG_display(0,pucRtc[0]/10);
	SMG_display(1,pucRtc[0]%10);
	SMG_display(2,10);
	SMG_display(3,pucRtc[1]/10);
	SMG_display(4,pucRtc[1]%10);
	SMG_display(5,10);
	SMG_display(6,pucRtc[2]/10);
	SMG_display(7,pucRtc[2]%10);
}
void main()
{	
	Set_Rtc(pucRtc,write_address);
	while(1)
	{
		Read_Rtc(pucRtc,read_address);
	}
}
