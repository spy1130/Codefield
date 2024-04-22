# include "STC15F2K60S2.H"
#include <intrins.h>

sbit SCK=P1^7;		
sbit SDA=P2^3;		
sbit RST = P1^3;   // DS1302复位	

void Write_Ds1302(unsigned char temp);
void Write_Ds1302_Byte( unsigned char address,unsigned char dat );
unsigned char Read_Ds1302_Byte( unsigned char address );

//函数名：设置DS1302时分秒函数
//入口参数：包含时分秒数据的数组指针
//返回值：无
//函数功能：设置DS1302时分秒函数

void Set_Rtc(unsigned char* ucRtc);


//函数名：读取DS1302时分秒函数
//入口参数：将读取到的时分秒数据存放到数组的指针
//返回值：无
//函数功能：读取DS1302时分秒函数，将读取到的数据放到数组指针指向的位置

void Read_Rtc(unsigned char* ucRtc);


