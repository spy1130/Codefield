#include "onewire.h"
#include <STC15F2K60S2.H>


sbit DQ = P1^4;  

//�������ڲ���ʱ����
void Delay_OneWire(unsigned int t)  
{
	t *= 12;
	while(t--);
}

//������д����
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

//�����߶�����
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

//DS18B20��ʼ��
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

/*---------------------------*/
float rd_temperature()
{
	unsigned char low,high;
	init_ds18b20();
	Write_DS18B20(0xcc);//����ROM
	Write_DS18B20(0x44);//�����¶�ת��
	init_ds18b20();
	Write_DS18B20(0xcc);//����ROM
	Write_DS18B20(0xbe);//��ȡ�¶�
	low = Read_DS18B20();//����λ
	high = Read_DS18B20();//����λ
	return ((high<<8) | low) / 16.0;//*���ȳ�0.0625
}