#include "onewire.h"
#include <STC15F2K60S2.h>

sbit DQ = P1^4;  //单总线接口

//单总线延时函数
void Delay_OneWire(unsigned int t)  //STC89C52RC
{
	unsigned char i;
	while(t--)
	{
		for(i=12;i>10;i++);
	}
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


unsigned char rd_temperature(void)
{
	char temper;
	unsigned char high,low;
	init_ds18b20();
	Write_DS18B20(0xCC);
	Write_DS18B20(0x44);		

	Delay_OneWire(200);
	init_ds18b20();
	Write_DS18B20(0xCC);
	Write_DS18B20(0xBE);	
	low = Read_DS18B20();
	high = Read_DS18B20();
	temper = high<<4;
	temper |= (low>>4);

	
	return temper;
}




