/*	# 	单总线代码片段说明
	1. 	本文件夹中提供的驱动代码供参赛选手完成程序设计参考。
	2. 	参赛选手可以自行编写相关代码或以该代码为基础，根据所选单片机类型、运行速度和试题
		中对单片机时钟频率的要求，进行代码调试和修改。
*/
#include "bsp_onewire_2023.h"
//
void Delay_OneWire(unsigned int t)  
{
	unsigned char i;
	while(t--){
		for(i=0;i<12;i++);
	}
}

//
void Write_DS18B20(unsigned char dat)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		DQ = 0;
		DQ = dat&0x01;
		Delay_OneWire(5);
		DQ = 1;
		dat >>= 1;
	}
	Delay_OneWire(5);
}

//
unsigned char Read_DS18B20(void)
{
	unsigned char i;
	unsigned char dat;
  
	for(i=0;i<8;i++)
	{
		DQ = 0;
		dat >>= 1;
		DQ = 1;
		if(DQ)
		{
			dat |= 0x80;
		}	    
		Delay_OneWire(5);
	}
	return dat;
}

//
bit init_ds18b20(void)
{
  	bit initflag = 0;
  	
  	DQ = 1;
  	Delay_OneWire(12);
  	DQ = 0;
  	Delay_OneWire(80);
  	DQ = 1;
  	Delay_OneWire(10); 
    initflag = DQ;     
  	Delay_OneWire(5);
  
  	return initflag;
}




//函数名：读取温度函数
//入口参数：无
//返回值：温度高低两个字节
//函数功能：完成温度转换，并返回转换之后的温度数据;
unsigned int rd_temperature(void)
{
  unsigned char low,high;//返回的温度数据的高低八位
		
	init_ds18b20();//初始化
	Write_DS18B20(0xcc);//跳过ROM
	Write_DS18B20(0x44);//进行温度转换

	init_ds18b20();//初始化
	Write_DS18B20(0xcc);//跳过ROM
	Write_DS18B20(0xbe);//读取温度
	
	low = Read_DS18B20();//读取低位
	high = Read_DS18B20();//读取高位	
	return (high<<8)|low;
	
}

