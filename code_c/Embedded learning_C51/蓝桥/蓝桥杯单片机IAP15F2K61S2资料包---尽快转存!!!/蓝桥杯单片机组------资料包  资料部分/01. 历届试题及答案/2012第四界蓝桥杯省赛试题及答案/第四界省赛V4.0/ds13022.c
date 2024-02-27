#include "ds13022.h"
/********************************************************************/ 
/*���ֽ�д��һ�ֽ�����*/

uchar clk[7]={30,30,8,10,11,2,20};
uchar shijian[7];

void Write_Ds1302_Byte(unsigned char dat);
unsigned char Read_Ds1302_Byte(void);
void Ds1302_Single_Byte_Write(unsigned char addr, unsigned char dat);
unsigned char Ds1302_Single_Byte_Read(unsigned char addr);

void ds_write()
{
	uchar i,wadd;
	wadd=0x80;
	Ds1302_Single_Byte_Write(0x8e,0x00);
	for(i=0;i<7;i++)
	{
		Ds1302_Single_Byte_Write(wadd,clk[i]);
		wadd+=2;
	}
	Ds1302_Single_Byte_Write(0x8e,0x80);
}

void ds_read()
{
	uchar i,wadd;
	wadd=0x81;
	Ds1302_Single_Byte_Write(0x8e,0x00);
	for(i=0;i<7;i++)
	{
		shijian[i]=Ds1302_Single_Byte_Read(wadd);  
		wadd+=2;
	}
	Ds1302_Single_Byte_Write(0x8e,0x80);	
}
void Write_Ds1302_Byte(unsigned char dat) 
{
	unsigned char i;
	SCK = 0;
	for (i=0;i<8;i++) 
	{ 
		if (dat & 0x01) 	// �ȼ���if((addr & 0x01) ==1) 
		{
			SDA_SET;		//#define SDA_SET SDA=1 /*��ƽ�ø�*/
		}
		else 
		{
			SDA_CLR;		//#define SDA_CLR SDA=0 /*��ƽ�õ�*/
		}		 
		SCK_SET;
		SCK_CLR;		
		dat = dat >> 1; 
	} 
}
/********************************************************************/ 
/*���ֽڶ���һ�ֽ�����*/
unsigned char Read_Ds1302_Byte(void) 
{
	unsigned char i, dat=0;	
	for (i=0;i<8;i++)
	{	
		dat = dat >> 1;
		if (SDA_R) 	  //�ȼ���if(SDA_R==1)    #define SDA_R SDA /*��ƽ��ȡ*/	
		{
			dat |= 0x80;
		}
		else 
		{
			dat &= 0x7F;
		}
		SCK_SET;
		SCK_CLR;
	}
	return dat;
}

/********************************************************************/ 
/*��DS1302 ���ֽ�д��һ�ֽ�����*/
void Ds1302_Single_Byte_Write(unsigned char addr, unsigned char dat)
{ 
   	uchar num;
	RST_CLR;			/*RST���õͣ�ʵ��DS1302�ĳ�ʼ��*/
	SCK_CLR;			/*SCK���õͣ�ʵ��DS1302�ĳ�ʼ��*/

	RST_SET;			/*����DS1302����,RST=1��ƽ�ø� */
	addr = addr & 0xFE;	 
	Write_Ds1302_Byte(addr); /*д��Ŀ���ַ��addr,��֤��д����,д֮ǰ�����λ����*/
	num=(dat/10<<4)|(dat%10);	
	Write_Ds1302_Byte(num);	 /*д�����ݣ�dat*/
	RST_CLR;				 /*ֹͣDS1302����*/
	SDA_CLR;
}

/********************************************************************/ 
/*��DS1302���ֽڶ���һ�ֽ�����*/
unsigned char Ds1302_Single_Byte_Read(unsigned char addr) 
{ 
	unsigned char temp,temp_l,temp_h;
	RST_CLR;			/*RST���õͣ�ʵ��DS1302�ĳ�ʼ��*/
	SCK_CLR;			/*SCK���õͣ�ʵ��DS1302�ĳ�ʼ��*/

	RST_SET;	/*����DS1302����,RST=1��ƽ�ø� */	
	addr = addr | 0x01;	 
	Write_Ds1302_Byte(addr); /*д��Ŀ���ַ��addr,��֤�Ƕ�����,д֮ǰ�����λ�ø�*/
	temp=Read_Ds1302_Byte(); /*��DS1302�ж���һ���ֽڵ�����*/
	temp_h=temp/16;
	temp_l=temp%16;
	temp=temp_h*10+temp_l;		
	RST_CLR;	/*ֹͣDS1302����*/
	SDA_CLR;					 	
	return temp;
}