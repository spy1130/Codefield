#include "ds13022.h"
/********************************************************************/ 
/*单字节写入一字节数据*/

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
		if (dat & 0x01) 	// 等价于if((addr & 0x01) ==1) 
		{
			SDA_SET;		//#define SDA_SET SDA=1 /*电平置高*/
		}
		else 
		{
			SDA_CLR;		//#define SDA_CLR SDA=0 /*电平置低*/
		}		 
		SCK_SET;
		SCK_CLR;		
		dat = dat >> 1; 
	} 
}
/********************************************************************/ 
/*单字节读出一字节数据*/
unsigned char Read_Ds1302_Byte(void) 
{
	unsigned char i, dat=0;	
	for (i=0;i<8;i++)
	{	
		dat = dat >> 1;
		if (SDA_R) 	  //等价于if(SDA_R==1)    #define SDA_R SDA /*电平读取*/	
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
/*向DS1302 单字节写入一字节数据*/
void Ds1302_Single_Byte_Write(unsigned char addr, unsigned char dat)
{ 
   	uchar num;
	RST_CLR;			/*RST脚置低，实现DS1302的初始化*/
	SCK_CLR;			/*SCK脚置低，实现DS1302的初始化*/

	RST_SET;			/*启动DS1302总线,RST=1电平置高 */
	addr = addr & 0xFE;	 
	Write_Ds1302_Byte(addr); /*写入目标地址：addr,保证是写操作,写之前将最低位置零*/
	num=(dat/10<<4)|(dat%10);	
	Write_Ds1302_Byte(num);	 /*写入数据：dat*/
	RST_CLR;				 /*停止DS1302总线*/
	SDA_CLR;
}

/********************************************************************/ 
/*从DS1302单字节读出一字节数据*/
unsigned char Ds1302_Single_Byte_Read(unsigned char addr) 
{ 
	unsigned char temp,temp_l,temp_h;
	RST_CLR;			/*RST脚置低，实现DS1302的初始化*/
	SCK_CLR;			/*SCK脚置低，实现DS1302的初始化*/

	RST_SET;	/*启动DS1302总线,RST=1电平置高 */	
	addr = addr | 0x01;	 
	Write_Ds1302_Byte(addr); /*写入目标地址：addr,保证是读操作,写之前将最低位置高*/
	temp=Read_Ds1302_Byte(); /*从DS1302中读出一个字节的数据*/
	temp_h=temp/16;
	temp_l=temp%16;
	temp=temp_h*10+temp_l;		
	RST_CLR;	/*停止DS1302总线*/
	SDA_CLR;					 	
	return temp;
}