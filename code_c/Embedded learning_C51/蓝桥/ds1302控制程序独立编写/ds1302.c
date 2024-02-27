#include <STC15F2K60S2.H>
#include "intrins.h"
sbit CE = P1^3;
sbit SCL = P1^7;
sbit SDA = P2^3;


//注意其中的时钟计时所用的是BCD码，注意转化方法

//先传地址再传数据，每次只传一个位。
void write_ds1302(unsigned char temp)
{
	unsigned char i;
	for(i=0;i<8;i++)//八个位移位8次
	{
		SCL = 0;//关闭时钟线
		SDA = temp&0x01;//数据传输
		temp>>=1;
		SCL=1;//开启时钟线
	}
}
void write_ds1302_byte(unsigned char address,unsigned char date)
{
	CE = 0; 	_nop_();//复位使能开关
	SCL = 0; 	_nop_();//复位时钟线
	CE = 1;		_nop_();//开启使能
	write_ds1302(address);
	write_ds1302(date);
	CE = 0;
}
unsigned char read_ds1302_byte(unsigned char address)//读传数据上升沿。
{
	unsigned char i,temp=0x00;
	CE = 0; 	_nop_();//复位使能开关
	SCL = 0; 	_nop_();//复位时钟线
	CE = 1;		_nop_();//开启使能
	write_ds1302(address);
	for(i=0;i<8;i++)
	{
		SCL = 0;
		temp>>=1;
		if(SDA)
		temp|=0x80;	
 		SCL=1;
	} 
 	CE=0;	_nop_();
 	SCL=0;	_nop_();
	SCL=1;	_nop_();
	SDA=0;	_nop_();
	SDA=1;	_nop_();
	return temp;
}