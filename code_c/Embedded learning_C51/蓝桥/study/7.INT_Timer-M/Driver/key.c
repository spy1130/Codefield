#include "STC15F2K60S2.H"
#include "timer.h"
#include "init.h"
#include "stdio.h"
#include "key.h"

//函数名：按键码值读取函数
//入口参数：无
//返回值：按键的码值
//函数功能：返回按键码。

unsigned char Key_Read(void)
{
	unsigned int Key_New;//16位的数值，用于存放P3直接读取的第四位键值
	unsigned char Key_Value;//返回值
	
	P44 = 0; P42 = 1; P35 = 1; P34 = 1;  // 第一列扫描
	Key_New = P3 & 0X0F; // P3 = 0000 1111 ---P37~P30，P30~P33为高电平（准双向口读取I/O口外部电平状态前，需要先将I/O口置为1）
	
	P44 = 1; P42 = 0; P35 = 1; P34 = 1;  // 第二列扫描
	Key_New = (Key_New << 4) | (P3 & 0X0F); //将原来的数值挪到次4位，本次数值放到最低4位，占用了8位
	
	P44 = 1; P42 = 1; P35 = 0; P34 = 1;  // 第三列扫描
	Key_New = (Key_New << 4) | (P3 & 0X0F); //将原来的数值挪到次次4位，本次数值放到最低4位，占用了12位

	P44 = 1; P42 = 1; P35 = 1; P34 = 0;  // 第四列扫描
	Key_New = (Key_New << 4) | (P3 & 0X0F); //将原来的数值挪到次次4位，本次数值放到最低4位，占用了16位
	
	switch(~Key_New)//Key_Value的数值对应按键的编号
	{
		case 0x8000: Key_Value = 4; break;
		case 0x4000: Key_Value = 5; break;
		case 0x2000: Key_Value = 6; break;
		case 0x1000: Key_Value = 7; break;		
	
		case 0x0800: Key_Value = 8; break;
		case 0x0400: Key_Value = 9; break;
		case 0x0200: Key_Value = 10; break;
		case 0x0100: Key_Value = 11; break;		
	
		case 0x0080: Key_Value = 12; break;
		case 0x0040: Key_Value = 13; break;
		case 0x0020: Key_Value = 14; break;
		case 0x0010: Key_Value = 15; break;			
	
		case 0x0008: Key_Value = 16; break;
		case 0x0004: Key_Value = 17; break;
		case 0x0002: Key_Value = 18; break;
		case 0x0001: Key_Value = 19; break;			
	
	}
	
	return Key_Value;
	
}


//函数名：按键码值读取函数-独立按键
//入口参数：无
//返回值：按键的码值
//函数功能：返回按键码。
unsigned char Key_BIN(void)
{
	unsigned char Key_Value;
	
	if(P30 ==0) Key_Value=7 ;
	else if(P31 ==0) Key_Value=6;
	else if(P32 ==0) Key_Value=5;
	else if(P33 ==0) Key_Value=4;
	else Key_Value = 0;
	
	return Key_Value;
}