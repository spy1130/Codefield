/*
  ����˵��: DS1302��������
  �������: Keil uVision 4.10 
  Ӳ������: CT107��Ƭ���ۺ�ʵѵƽ̨ 8051��12MHz
  ��    ��: 2011-8-9
*/

#include "bsp_ds1302.h"											


void Write_Ds1302(unsigned  char temp) 
{
	unsigned char i;
	for (i=0;i<8;i++)     	
	{ 
		SCK=0;
		SDA=temp&0x01;
		temp>>=1; 
		SCK=1;
	}
}   

void Write_Ds1302_Byte( unsigned char address,unsigned char dat )     
{
 	RST=0;	_nop_();
 	SCK=0;	_nop_();
 	RST=1; 	_nop_();  
 	Write_Ds1302(address);	
 	Write_Ds1302(dat);		
 	RST=0; 
}

unsigned char Read_Ds1302_Byte ( unsigned char address )
{
 	unsigned char i,temp=0x00;
 	RST=0;	_nop_();
 	SCK=0;	_nop_();
 	RST=1;	_nop_();
 	Write_Ds1302(address);
 	for (i=0;i<8;i++) 	
 	{		
		SCK=0;
		temp>>=1;	
 		if(SDA)
 		temp|=0x80;	
 		SCK=1;
	} 
 	RST=0;	_nop_();
 	SCK=0;	_nop_();
	SCK=1;	_nop_();
	SDA=0;	_nop_();
	SDA=1;	_nop_();
	return (temp);			
}


//������������DS1302ʱ���뺯��
//��ڲ���������ʱ�������ݵ�����ָ��
//����ֵ����
//�������ܣ�����DS1302ʱ���뺯��

void Set_Rtc(unsigned char* ucRtc)
{
	unsigned char temp;//�м�ֲ����������ʱ����
	
	Write_Ds1302_Byte(0x8e, 0);//wp = 0, ����д����
	
	temp = (((ucRtc[0]/10)<<4)|(ucRtc[0]%10)) ;//����ĵ�0�����ݣ�Сʱ����BCD�����ʽ�洢
	Write_Ds1302_Byte(0x84, temp);//д�뵽Сʱ�ļĴ���
	temp = (((ucRtc[1]/10)<<4)|(ucRtc[1]%10)) ;//����ĵ�1�����ݣ����ӣ���BCD�����ʽ�洢
	Write_Ds1302_Byte(0x82, temp);//д�뵽���ӵļĴ���
	temp = (((ucRtc[2]/10)<<4)|(ucRtc[2]%10)) ;//����ĵ�2�����ݣ��룬��BCD�����ʽ�洢
	Write_Ds1302_Byte(0x80, temp);//д�뵽��ļĴ���

	Write_Ds1302_Byte(0x8e, 0x80);//wp = 1, ������д����
}



//����������ȡDS1302ʱ���뺯��
//��ڲ���������ȡ����ʱ�������ݴ�ŵ������ָ��
//����ֵ����
//�������ܣ���ȡDS1302ʱ���뺯��������ȡ�������ݷŵ�����ָ��ָ���λ��

void Read_Rtc(unsigned char* ucRtc)
{
	unsigned char temp;//�м�ֲ����������ʱ����

	temp = Read_Ds1302_Byte (0x85);//��ȡСʱ����
	ucRtc[0] = (((temp>>4)*10) + (temp&0x0F));// 23(BCD) = 0010 0011(B) = 35(D),����ֱ�Ӹ�ֵ��Ҫ����BCD��ʮ�������ݵ�ת��
	temp = Read_Ds1302_Byte (0x83);//��ȡ��������
	ucRtc[1] = (((temp>>4)*10) + (temp&0x0F));// 23(BCD) = 0010 0011(B) = 35(D),����ֱ�Ӹ�ֵ��Ҫ����BCD��ʮ�������ݵ�ת��	
	temp = Read_Ds1302_Byte (0x81);//��ȡ������
	ucRtc[2] = (((temp>>4)*10) + (temp&0x0F));// 23(BCD) = 0010 0011(B) = 35(D),����ֱ�Ӹ�ֵ��Ҫ����BCD��ʮ�������ݵ�ת��	
	
}


