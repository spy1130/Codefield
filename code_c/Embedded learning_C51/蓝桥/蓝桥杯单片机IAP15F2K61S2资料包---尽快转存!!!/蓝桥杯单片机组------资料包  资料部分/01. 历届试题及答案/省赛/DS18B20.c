#include "DS18B20.h"


// SKIP ROM [CCh]  CONVERT T [44h] READ SCRATCHPAD [BEh]

sbit DQ = P1^4;

void Delay_OneWire(unsigned int t)  //STC89C52RC
{
	unsigned char i;
	while(t--)
	{
		for(i=12; i>0; i--);
	}
}

//通过单总线向DS18B20写一个字节
void Write_DS18B20_byte(unsigned char dat)
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
unsigned char Read_DS18B20_byte(void)
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

void write_DS18B20(unsigned char dat)
{
	init_ds18b20();
	Write_DS18B20_byte(0xCC);
	Write_DS18B20_byte(dat);
}

int Read_DS18B20()
{
	int temp;
	unsigned char TH,TL;
	init_ds18b20();
	Write_DS18B20_byte(0xCC);
	Write_DS18B20_byte(0xBE);
	TL = Read_DS18B20_byte();
	TH = Read_DS18B20_byte();
	temp = TH&0x0f;
	temp <<= 8;
	temp |= TL;
	temp = temp * 0.0625;
	return temp;
}