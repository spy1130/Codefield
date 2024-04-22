/*	# 	DS1302代码片段说明
	1. 	本文件夹中提供的驱动代码供参赛选手完成程序设计参考。
	2. 	参赛选手可以自行编写相关代码或以该代码为基础，根据所选单片机类型、运行速度和试题
		中对单片机时钟频率的要求，进行代码调试和修改。
*/								
#include "bsp_ds1302_2023.h"	
//
void Write_Ds1302(unsigned  char temp) 
{
	unsigned char i;
	for (i=0;i<8;i++)     	
	{ 
		SCK = 0;
		SDA = temp&0x01;
		temp>>=1; 
		SCK=1;
	}
}   

//
void Write_Ds1302_Byte( unsigned char address,unsigned char dat )     
{
 	RST=0;	_nop_();
 	SCK=0;	_nop_();
 	RST=1; 	_nop_();  
 	Write_Ds1302(address);	
 	Write_Ds1302(dat);		
 	RST=0; 
}

//
unsigned char Read_Ds1302_Byte ( unsigned char address )
{
 	unsigned char i,temp=0x00;
 	RST=0;	_nop_();
 	SCK=0;	_nop_();
 	RST=1;	_nop_();
 	Write_Ds1302(address);
 	for (i=0;i<8;i++) 	
 	{		
		SCK=0;
		temp>>=1;	
 		if(SDA)
 		temp|=0x80;	
 		SCK=1;
	} 
 	RST=0;	_nop_();
 	SCK=0;	_nop_();
	SCK=1;	_nop_();
	SDA=0;	_nop_();
	SDA=1;	_nop_();
	return (temp);			
}



//函数名：设置DS1302时分秒函数
//入口参数：包含时分秒数据的数组指针
//返回值：无
//函数功能：设置DS1302时分秒函数

void Set_Rtc(unsigned char* ucRtc)
{
	unsigned char temp;//中间局部变量，存放时分秒
	
	Write_Ds1302_Byte(0x8e, 0);//wp = 0, 允许写操作
	
	temp = (((ucRtc[0]/10)<<4)|(ucRtc[0]%10)) ;//数组的第0个数据，小时，用BCD码的形式存储
	Write_Ds1302_Byte(0x84, temp);//写入到小时的寄存器
	temp = (((ucRtc[1]/10)<<4)|(ucRtc[1]%10)) ;//数组的第1个数据，分钟，用BCD码的形式存储
	Write_Ds1302_Byte(0x82, temp);//写入到分钟的寄存器
	temp = (((ucRtc[2]/10)<<4)|(ucRtc[2]%10)) ;//数组的第2个数据，秒，用BCD码的形式存储
	Write_Ds1302_Byte(0x80, temp);//写入到秒的寄存器

	Write_Ds1302_Byte(0x8e, 0x80);//wp = 1, 不允许写操作
}



//函数名：读取DS1302时分秒函数
//入口参数：将读取到的时分秒数据存放到数组的指针
//返回值：无
//函数功能：读取DS1302时分秒函数，将读取到的数据放到数组指针指向的位置

void Read_Rtc(unsigned char* ucRtc)
{
	unsigned char temp;//中间局部变量，存放时分秒

	temp = Read_Ds1302_Byte (0x85);//读取小时数据
	ucRtc[0] = (((temp>>4)*10) + (temp&0x0F));// 23(BCD) = 0010 0011(B) = 35(D),不能直接赋值，要进行BCD到十进制数据的转换
	temp = Read_Ds1302_Byte (0x83);//读取分钟数据
	ucRtc[1] = (((temp>>4)*10) + (temp&0x0F));// 23(BCD) = 0010 0011(B) = 35(D),不能直接赋值，要进行BCD到十进制数据的转换	
	temp = Read_Ds1302_Byte (0x81);//读取秒数据
	ucRtc[2] = (((temp>>4)*10) + (temp&0x0F));// 23(BCD) = 0010 0011(B) = 35(D),不能直接赋值，要进行BCD到十进制数据的转换	
	
}