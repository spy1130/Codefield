/*
  ����˵��: ��������������
  �������: Keil uVision 4.10 
  Ӳ������: CT107��Ƭ���ۺ�ʵѵƽ̨ 8051 12MHz
  ��    ��: 2011-8-9
*/

sbit DQ = P1^4;  //�����߽ӿ�

//��������ʱ����
void Delay_OneWire(unsigned int t)  
{
	unsigned char i;
	while(t--){
		for(i=0; i<8; i++);
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

//DS18B20�豸��ʼ��
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






