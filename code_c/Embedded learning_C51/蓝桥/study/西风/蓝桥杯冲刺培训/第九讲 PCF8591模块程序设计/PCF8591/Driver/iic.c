/*
  程序说明: IIC总线驱动程序
  软件环境: Keil uVision 4.10 
  硬件环境: CT107单片机综合实训平台 8051，12MHz
  日    期: 2011-8-9
*/

#include "iic.h"

//总线引脚定义
sbit SDA = P2^1;  /* 数据线 */
sbit SCL = P2^0;  /* 时钟线 */

void IIC_Delay(unsigned char i)
{
    do{_nop_();}
    while(i--);        
}

//总线启动条件
void IIC_Start(void)
{
    SDA = 1;
    SCL = 1;
    IIC_Delay(DELAY_TIME);
    SDA = 0;
    IIC_Delay(DELAY_TIME);
    SCL = 0;	
}

//总线停止条件
void IIC_Stop(void)
{
    SDA = 0;
    SCL = 1;
    IIC_Delay(DELAY_TIME);
    SDA = 1;
    IIC_Delay(DELAY_TIME);
}

//发送应答
void IIC_SendAck(bit ackbit)
{
    SCL = 0;
    SDA = ackbit;  					// 0：应答，1：非应答
    IIC_Delay(DELAY_TIME);
    SCL = 1;
    IIC_Delay(DELAY_TIME);
    SCL = 0; 
    SDA = 1;
    IIC_Delay(DELAY_TIME);
}

//等待应答
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

//通过I2C总线发送数据
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

//从I2C总线上接收数据
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





//函数名：ADC转换函数
//入口参数：要进行转换的通道控制位
//返回值：ADC转换的数值
//函数功能：对指定的通道进行ADC转换，函数返回转换的数值
unsigned char AD_Read(unsigned char channel_num_contrl)
{
	unsigned char temp;
	
	IIC_Start();//发送开启信号
	IIC_SendByte(0x90);//选择PCF8591芯片，确定写的模式
	IIC_WaitAck();//等待PCF8591反馈
	
	IIC_SendByte(channel_num_contrl);//确定要转换的通道（顺便，使能DA转换）
	IIC_WaitAck();//等待PCF8591反馈	
	
	IIC_Start();//发送开启信号
	IIC_SendByte(0x91);//选择PCF8591芯片，确定读的模式
	IIC_WaitAck();//等待PCF8591反馈	
		
	temp = IIC_RecByte();//接收数据
	IIC_SendAck(1);//选择不应答
	IIC_Stop();//停止发送
	
	return temp;

}



//函数名：DAC转换函数
//入口参数：要进行转换的数值
//返回值：无
//函数功能：对入口参数要转换的DA数据进行转换
void DA_Write(unsigned char trans_dat)
{
	IIC_Start();//发送开启信号
	IIC_SendByte(0x90);//选择PCF8591芯片，确定写的模式
	IIC_WaitAck();//等待PCF8591反馈
	
	IIC_SendByte(0x41);//使能DA转换（随便写通道编号，不影响，主要的功能是使能DA）
	IIC_WaitAck();//等待PCF8591反馈		

	IIC_SendByte(trans_dat);//将待转换的数据发送出去
	IIC_WaitAck();//等待PCF8591反馈	
	IIC_Stop();//停止发送	

}
