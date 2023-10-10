#include "stm32f10x.h"                  // Device header
#include "Delay.h"
//�Բ����˿ڵĿ⺯�����з�װ
void MyI2C_W_SCL(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_10, (BitAction)BitValue);//BitAction��һ��ö�����ͣ���ʾ���λ�Ǹߵ�ƽ���ǵ͵�ƽ

	Delay_us(10);
}

void MyI2C_W_SDA(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_11, (BitAction)BitValue);
	Delay_us(10);
}

uint8_t MyI2C_R_SDA(void)
{
	uint8_t BitValue;
	BitValue = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
	Delay_us(10);
	return BitValue;
}
//���I2Cֻ������GPIO�Ķ�д��������
//��scl��sda����ʼ��Ϊ��©���ģʽ,��ģʽ�Կ�������,����ʱ.������l����ֱ���������ݼĴ������ɡ�
//��scl��SDA��Ϊ�ߵ�ƽ����ʱI2C���ߴ��ڿ���״̬

void MyI2C_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11);
}
//��ʼ����
void MyI2C_Start(void)
{	//ȷ��I2C�����ͷ�
	MyI2C_W_SDA(1);
	MyI2C_W_SCL(1);
	//������SDA��������SCL
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(0);
}
//��ֹ����
void MyI2C_Stop(void)
{	//������SDA,���ͷ�SCL.SDA
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(1);
}

void MyI2C_SendByte(uint8_t Byte)//����һ���ֽ�8λ
{
	uint8_t i;
	for (i = 0; i < 8; i ++)
	{
		MyI2C_W_SDA(Byte & (0x80 >> i));//��λ�����������¸�ֵ��Byte&0x80�൱�ڰ�Byte�����λȡ����
		MyI2C_W_SCL(1);
		MyI2C_W_SCL(0);//ÿ����һλ��ִ���ͷ�SCL������SCL����
	}
}
//����һ���ֽ�8λ
uint8_t MyI2C_ReceiveByte(void)
{
	uint8_t i, Byte = 0x00;//��ʼΪ0
	MyI2C_W_SDA(1);
	for (i = 0; i < 8; i ++)//������SCLΪ�͵�ƽ�������ͷ�SDA���ӻ������ݷŵ�SDA����ʱ�����ͷ�SCL����R_SDA��ȡ
	{
		MyI2C_W_SCL(1);
		if (MyI2C_R_SDA() == 1){Byte |= (0x80 >> i);}//�ж�����Ϊ������ݲ�Ϊ0
		MyI2C_W_SCL(0);
	}
	return Byte;
}

void MyI2C_SendAck(uint8_t AckBit)//��һλ
{
	MyI2C_W_SDA(AckBit);
	MyI2C_W_SCL(1);
	MyI2C_W_SCL(0);
}

uint8_t MyI2C_ReceiveAck(void)//��һλ
	//�ӻ�Ӧ��Ϊ0����Ӧ��Ϊ1
{	//��������ʱ��SCL�͵�ƽ�������ͷ�SDA����ֹ�ӻ����ţ�
	//ͬʱ�ӻ���Ӧ��λ����SDA�ϣ�SCL�ߵ�ƽ��������ȡӦ��λ��SCL�͵�ƽ��������һ��ʱ��Ԫ
	uint8_t AckBit;
	MyI2C_W_SDA(1);
	MyI2C_W_SCL(1);
	AckBit = MyI2C_R_SDA();
	MyI2C_W_SCL(0);
	return AckBit;
}
