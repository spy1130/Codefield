#include "bsp_onewire.h"


////��������ʱ����
void Delay_OneWire(unsigned int t)  
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




//����������ȡ�¶Ⱥ���
//��ڲ�������
//����ֵ���¶ȸߵ������ֽ�
//�������ܣ�����¶�ת����������ת��֮����¶�����;
unsigned int rd_temperature(void)
{
  unsigned char low,high;//���ص��¶����ݵĸߵͰ�λ
		
	init_ds18b20();//��ʼ��
	Write_DS18B20(0xcc);//����ROM
	Write_DS18B20(0x44);//�����¶�ת��

	init_ds18b20();//��ʼ��
	Write_DS18B20(0xcc);//����ROM
	Write_DS18B20(0xbe);//��ȡ�¶�
	
	low = Read_DS18B20();//��ȡ��λ
	high = Read_DS18B20();//��ȡ��λ	
	return (high<<8)|low;
	
}

