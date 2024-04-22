#include "bsp_iic.h"

//�������Ŷ���
sbit SDA = P2^1;  /* ������ */
sbit SCL = P2^0;  /* ʱ���� */

void IIC_Delay(unsigned char i)
{
    do{_nop_();}
    while(i--);        
}

//������������
void IIC_Start(void)
{
    SDA = 1;
    SCL = 1;
    IIC_Delay(DELAY_TIME);
    SDA = 0;
    IIC_Delay(DELAY_TIME);
    SCL = 0;	
}

//����ֹͣ����
void IIC_Stop(void)
{
    SDA = 0;
    SCL = 1;
    IIC_Delay(DELAY_TIME);
    SDA = 1;
    IIC_Delay(DELAY_TIME);
}

//����Ӧ��
void IIC_SendAck(bit ackbit)
{
    SCL = 0;
    SDA = ackbit;  					// 0��Ӧ��1����Ӧ��
    IIC_Delay(DELAY_TIME);
    SCL = 1;
    IIC_Delay(DELAY_TIME);
    SCL = 0; 
    SDA = 1;
    IIC_Delay(DELAY_TIME);
}

//�ȴ�Ӧ��
bit IIC_WaitAck(void)
{
    bit ackbit;
	
    SCL  = 1;
    IIC_Delay(DELAY_TIME);
    ackbit = SDA;
    SCL = 0;
    IIC_Delay(DELAY_TIME);
    return ackbit;
}

//ͨ��I2C���߷�������
void IIC_SendByte(unsigned char byt)
{
    unsigned char i;

    for(i=0; i<8; i++)
    {
        SCL  = 0;
        IIC_Delay(DELAY_TIME);
        if(byt & 0x80) SDA  = 1;
        else SDA  = 0;
        IIC_Delay(DELAY_TIME);
        SCL = 1;
        byt <<= 1;
        IIC_Delay(DELAY_TIME);
    }
    SCL  = 0;  
}

//��I2C�����Ͻ�������
unsigned char IIC_RecByte(void)
{
    unsigned char i, da;
    for(i=0; i<8; i++)
    {   
    	SCL = 1;
	IIC_Delay(DELAY_TIME);
	da <<= 1;
	if(SDA) da |= 1;
	SCL = 0;
	IIC_Delay(DELAY_TIME);
    }
    return da;    
}


//��������ADCת������
//��ڲ�����Ҫ����ת����ͨ������λ
//����ֵ��ADCת������ֵ
//�������ܣ���ָ����ͨ������ADCת������������ת������ֵ
unsigned char Pcf8591_Adc(unsigned char channel_num_contrl)
{
	unsigned char temp;
	
	IIC_Start();//���Ϳ����ź�
	IIC_SendByte(0x90);//ѡ��PCF8591оƬ��ȷ��д��ģʽ
	IIC_WaitAck();//�ȴ�PCF8591����
	
	IIC_SendByte(channel_num_contrl);//ȷ��Ҫת����ͨ����˳�㣬ʹ��DAת����
	IIC_WaitAck();//�ȴ�PCF8591����	
	
	IIC_Start();//���Ϳ����ź�
	IIC_SendByte(0x91);//ѡ��PCF8591оƬ��ȷ������ģʽ
	IIC_WaitAck();//�ȴ�PCF8591����	
		
	temp = IIC_RecByte();//��������
	IIC_SendAck(1);//ѡ��Ӧ��
	IIC_Stop();//ֹͣ����
	
	return temp;

}



//��������DACת������
//��ڲ�����Ҫ����ת������ֵ
//����ֵ����
//�������ܣ�����ڲ���Ҫת����DA���ݽ���ת��
void Pcf8591_Dac(unsigned char trans_dat)
{
	IIC_Start();//���Ϳ����ź�
	IIC_SendByte(0x90);//ѡ��PCF8591оƬ��ȷ��д��ģʽ
	IIC_WaitAck();//�ȴ�PCF8591����
	
	IIC_SendByte(0x41);//ʹ��DAת�������дͨ����ţ���Ӱ�죬��Ҫ�Ĺ�����ʹ��DA��
	IIC_WaitAck();//�ȴ�PCF8591����		

	IIC_SendByte(trans_dat);//����ת�������ݷ��ͳ�ȥ
	IIC_WaitAck();//�ȴ�PCF8591����	
	IIC_Stop();//ֹͣ����	

}



//��������дEEPROM����
//��ڲ�������Ҫд����ַ�����д��ĵ�ַ(���Ϊ8�ı���)��д������
//����ֵ����
//�������ܣ���EERPOM��ĳ����ַд���ַ������ض��������ַ���
void EEPROM_Write(unsigned char* EEPROM_String, unsigned char addr, unsigned char num)
{
	IIC_Start();//���Ϳ����ź�
	IIC_SendByte(0xA0);//ѡ��EEPROMоƬ��ȷ��д��ģʽ
	IIC_WaitAck();//�ȴ�EEPROM����
	
	IIC_SendByte(addr);//д��Ҫ�洢�����ݵ�ַ
	IIC_WaitAck();//�ȴ�EEPROM����		

	while(num--)
	{
		IIC_SendByte(*EEPROM_String++);//��Ҫд�����Ϣд��
		IIC_WaitAck();//�ȴ�EEPROM����		
		IIC_Delay(200);	
	}
	IIC_Stop();//ֹͣ����	
}


//����������EEPROM����
//��ڲ�����������������Ҫ�洢���ַ�������ȡ�ĵ�ַ(���Ϊ8�ı���)����ȡ������
//����ֵ����
//�������ܣ���ȡEERPOM��ĳ����ַ�е����ݣ���������ַ��������С�
void EEPROM_Read(unsigned char* EEPROM_String, unsigned char addr, unsigned char num)
{
	IIC_Start();//���Ϳ����ź�
	IIC_SendByte(0xA0);//ѡ��EEPROMоƬ��ȷ��д��ģʽ
	IIC_WaitAck();//�ȴ�EEPROM����
	
	IIC_SendByte(addr);//д��Ҫ��ȡ�����ݵ�ַ
	IIC_WaitAck();//�ȴ�EEPROM����		

	IIC_Start();//���Ϳ����ź�
	IIC_SendByte(0xA1);//ѡ��EEPROMоƬ��ȷ������ģʽ
	IIC_WaitAck();//�ȴ�EEPROM����	
	
	while(num--)
	{
		*EEPROM_String++ = IIC_RecByte();//��Ҫд�����Ϣд��
		if(num) IIC_SendAck(0);//����Ӧ��
			else IIC_SendAck(1);//��Ӧ��
	}
	
	IIC_Stop();//ֹͣ����	
}



