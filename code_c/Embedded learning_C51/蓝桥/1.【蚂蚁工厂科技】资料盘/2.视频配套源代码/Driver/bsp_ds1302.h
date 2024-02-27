# include "STC15F2K60S2.H"
#include <intrins.h>

sbit SCK=P1^7;		
sbit SDA=P2^3;		
sbit RST = P1^3;   // DS1302��λ	

void Write_Ds1302(unsigned char temp);
void Write_Ds1302_Byte( unsigned char address,unsigned char dat );
unsigned char Read_Ds1302_Byte( unsigned char address );

//������������DS1302ʱ���뺯��
//��ڲ���������ʱ�������ݵ�����ָ��
//����ֵ����
//�������ܣ�����DS1302ʱ���뺯��

void Set_Rtc(unsigned char* ucRtc);


//����������ȡDS1302ʱ���뺯��
//��ڲ���������ȡ����ʱ�������ݴ�ŵ������ָ��
//����ֵ����
//�������ܣ���ȡDS1302ʱ���뺯��������ȡ�������ݷŵ�����ָ��ָ���λ��

void Read_Rtc(unsigned char* ucRtc);


