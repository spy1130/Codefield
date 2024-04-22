#include "bsp_iic.h"

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
unsigned char Pcf8591_Adc(unsigned char channel_num_contrl)
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
void Pcf8591_Dac(unsigned char trans_dat)
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



//函数名：写EEPROM函数
//入口参数：需要写入的字符串，写入的地址(务必为8的倍数)，写入数量
//返回值：无
//函数功能：向EERPOM的某个地址写入字符串中特定数量的字符。
void EEPROM_Write(unsigned char* EEPROM_String, unsigned char addr, unsigned char num)
{
	IIC_Start();//发送开启信号
	IIC_SendByte(0xA0);//选择EEPROM芯片，确定写的模式
	IIC_WaitAck();//等待EEPROM反馈
	
	IIC_SendByte(addr);//写入要存储的数据地址
	IIC_WaitAck();//等待EEPROM反馈		

	while(num--)
	{
		IIC_SendByte(*EEPROM_String++);//将要写入的信息写入
		IIC_WaitAck();//等待EEPROM反馈		
		IIC_Delay(200);	
	}
	IIC_Stop();//停止发送	
}


//函数名：读EEPROM函数
//入口参数：读到的数据需要存储的字符串，读取的地址(务必为8的倍数)，读取的数量
//返回值：无
//函数功能：读取EERPOM的某个地址中的数据，并存放在字符串数组中。
void EEPROM_Read(unsigned char* EEPROM_String, unsigned char addr, unsigned char num)
{
	IIC_Start();//发送开启信号
	IIC_SendByte(0xA0);//选择EEPROM芯片，确定写的模式
	IIC_WaitAck();//等待EEPROM反馈
	
	IIC_SendByte(addr);//写入要读取的数据地址
	IIC_WaitAck();//等待EEPROM反馈		

	IIC_Start();//发送开启信号
	IIC_SendByte(0xA1);//选择EEPROM芯片，确定读的模式
	IIC_WaitAck();//等待EEPROM反馈	
	
	while(num--)
	{
		*EEPROM_String++ = IIC_RecByte();//将要写入的信息写入
		if(num) IIC_SendAck(0);//发送应答
			else IIC_SendAck(1);//不应答
	}
	
	IIC_Stop();//停止发送	
}



