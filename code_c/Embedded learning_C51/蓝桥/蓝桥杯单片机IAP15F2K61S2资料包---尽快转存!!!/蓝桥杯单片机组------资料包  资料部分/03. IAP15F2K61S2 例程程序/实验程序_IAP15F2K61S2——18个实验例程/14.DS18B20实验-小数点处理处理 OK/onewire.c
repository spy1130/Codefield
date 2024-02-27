#include "reg52.h"

sbit DQ = P1^4;

//��������ʱ����

void Delay_OneWire(unsigned int t)  //STC12C5260S2
{
	unsigned char i;
	while(t--){
		for(i=0;i<12;i++);
	}
}


//ͨ����������DS18B20дһ���ֽ�
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

//��DS18B20��ȡһ���ֽ�
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
  	Delay_OneWire(80); // ��ʱ����480us
  	DQ = 1;
  	Delay_OneWire(10);  // 14
  	initflag = DQ;     // initflag����1��ʼ��ʧ��
  	Delay_OneWire(5);
  
  	return initflag;
}

//DS18B20�¶Ȳɼ���������
// unsigned char rd_temperature(void)
// {
//     unsigned char low,high;
//   	char temp;
//   
//   	init_ds18b20();
//   	Write_DS18B20(0xCC);
//   	Write_DS18B20(0x44); //�����¶�ת��
//   	Delay_OneWire(200);

//   	init_ds18b20();
//   	Write_DS18B20(0xCC);
//   	Write_DS18B20(0xBE); //��ȡ�Ĵ���

//   	low = Read_DS18B20(); //���ֽ�
//   	high = Read_DS18B20(); //���ֽ�
// /** ����Ϊ1���϶� */  
//   	temp = high<<4;
//   	temp |= (low>>4);
//   
//   	return temp;
// }

//DS18B20�¶Ȳɼ����򣺸�����
float rd_temperature_f(void)
{
    unsigned int temp;
	float temperature;
    unsigned char low,high;
  
  	init_ds18b20();
  	Write_DS18B20(0xCC);
  	Write_DS18B20(0x44); //�����¶�ת��
  	Delay_OneWire(200);

  	init_ds18b20();
  	Write_DS18B20(0xCC);
  	Write_DS18B20(0xBE); //��ȡ�Ĵ���

  	low = Read_DS18B20(); //���ֽ�
  	high = Read_DS18B20(); //���ֽ�
/** ����Ϊ0.0625���϶� */  
	temp = (high&0x0f);
	temp <<= 8;
	temp |= low;
	temperature = temp*0.0625;
  
  	return temperature;
}

