#include <STC15F2K60S2.H>
#include "intrins.h"
sbit CE = P1^3;
sbit SCL = P1^7;
sbit SDA = P2^3;


//ע�����е�ʱ�Ӽ�ʱ���õ���BCD�룬ע��ת������

//�ȴ���ַ�ٴ����ݣ�ÿ��ֻ��һ��λ��
void write_ds1302(unsigned char temp)
{
	unsigned char i;
	for(i=0;i<8;i++)//�˸�λ��λ8��
	{
		SCL = 0;//�ر�ʱ����
		SDA = temp&0x01;//���ݴ���
		temp>>=1;
		SCL=1;//����ʱ����
	}
}
void write_ds1302_byte(unsigned char address,unsigned char date)
{
	CE = 0; 	_nop_();//��λʹ�ܿ���
	SCL = 0; 	_nop_();//��λʱ����
	CE = 1;		_nop_();//����ʹ��
	write_ds1302(address);
	write_ds1302(date);
	CE = 0;
}
unsigned char read_ds1302_byte(unsigned char address)//�������������ء�
{
	unsigned char i,temp=0x00;
	CE = 0; 	_nop_();//��λʹ�ܿ���
	SCL = 0; 	_nop_();//��λʱ����
	CE = 1;		_nop_();//����ʹ��
	write_ds1302(address);
	for(i=0;i<8;i++)
	{
		SCL = 0;
		temp>>=1;
		if(SDA)
		temp|=0x80;	
 		SCL=1;
	} 
 	CE=0;	_nop_();
 	SCL=0;	_nop_();
	SCL=1;	_nop_();
	SDA=0;	_nop_();
	SDA=1;	_nop_();
	return temp;
}