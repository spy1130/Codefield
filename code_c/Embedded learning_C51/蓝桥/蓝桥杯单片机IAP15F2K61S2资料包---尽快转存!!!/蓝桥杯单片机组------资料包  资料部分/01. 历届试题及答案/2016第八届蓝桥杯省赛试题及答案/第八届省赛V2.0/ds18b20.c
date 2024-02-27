/*
  程序说明: 单总线驱动程序
  软件环境: Keil uVision 4.10 
  硬件环境: CT107单片机综合实训平台 8051 12MHz
  日    期: 2011-8-9
*/

sbit DQ = P1^4;  //单总线接口

//单总线延时函数
void Delay_OneWire(unsigned int t)  
{
	unsigned char i;
	while(t--){
		for(i=0; i<8; i++);
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
void init_ds18b20(void)
{
  	uchar x = 255;
	DQ = 1;
	DQ = 0;
	Delay_OneWire(80);
	DQ = 1;
	while(DQ && x--);

	Delay_OneWire(20);
}

unsigned char a,b,c;
uchar Read_Temperature(void)
{
	unsigned char i = 0,t = 0,w = 0;
	init_ds18b20();
	Write_DS18B20(0xcc);
	Write_DS18B20(0x44);
	init_ds18b20();
	Write_DS18B20(0xcc);
	Write_DS18B20(0xbe);

	a = Read_DS18B20();
	b = Read_DS18B20();

	c = 0x0f & a;
	a = a >> 4;
	b = b << 4;
	t = a | b;
	return t;
}






