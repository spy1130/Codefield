/*
  ����˵��: IIC������������
  �������: Keil uVision 4.10 
  Ӳ������: CT107��Ƭ���ۺ�ʵѵƽ̨ 8051��12MHz
  ��    ��: 2011-8-9
*/

#include "iic.h"


#include "intrins.h"

#define DELAY_TIME 5

#define Photo_Res_Channel 0x41
#define Adj_Res_Channel 0x43


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





unsigned char Ad_Read(unsigned char addr)
{
	unsigned char temp;
	IIC_Start();
	IIC_SendByte(0x90);
	IIC_WaitAck();
	IIC_SendByte(addr);
	IIC_WaitAck();
	IIC_Start();
	IIC_SendByte(0x91);
	IIC_WaitAck();	
	temp = IIC_RecByte();
	IIC_SendAck(1);
	IIC_Stop();
	return temp;
}

void Da_Write(unsigned char dat)
{
	IIC_Start();
	IIC_SendByte(0x90);
	IIC_WaitAck();
	IIC_SendByte(0x41);
	IIC_WaitAck();
	IIC_SendByte(dat);
	IIC_WaitAck();	
 	IIC_Stop();	
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