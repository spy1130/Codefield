/*
  程序说明: 单总线驱动程序
  软件环境: Keil uVision 4.10 
  硬件环境: CT107单片机综合实训平台(外部晶振12MHz) STC89C52RC单片机
  日    期: 2011-8-9
*/
#include "bsp_onewire.h"


//单总线延时函数
void Delay_OneWire(unsigned int t)  //STC89C52RC
{
 	t *= 12;
	while(t--);
}

//通过单总线向DS18B20写一个字节
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

//从DS18B20读取一个字节
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

//DS18B20设备初始化
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




