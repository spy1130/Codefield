/*	#   I2C代码片段说明
	1. 	本文件夹中提供的驱动代码供参赛选手完成程序设计参考。
	2. 	参赛选手可以自行编写相关代码或以该代码为基础，根据所选单片机类型、运行速度和试题
		中对单片机时钟频率的要求，进行代码调试和修改。
*/

#include "bsp_iic_2023.h"



//
static void I2C_Delay(unsigned char n)
{
    do
    {
        _nop_();_nop_();_nop_();_nop_();_nop_();
        _nop_();_nop_();_nop_();_nop_();_nop_();
        _nop_();_nop_();_nop_();_nop_();_nop_();		
    }
    while(n--);      	
}

//
void I2CStart(void)
{
    sda = 1;
    scl = 1;
	I2C_Delay(DELAY_TIME);
    sda = 0;
	I2C_Delay(DELAY_TIME);
    scl = 0;    
}

//
void I2CStop(void)
{
    sda = 0;
    scl = 1;
	I2C_Delay(DELAY_TIME);
    sda = 1;
	I2C_Delay(DELAY_TIME);
}

//
void I2CSendByte(unsigned char byt)
{
    unsigned char i;
	
    for(i=0; i<8; i++){
        scl = 0;
		I2C_Delay(DELAY_TIME);
        if(byt & 0x80){
            sda = 1;
        }
        else{
            sda = 0;
        }
		I2C_Delay(DELAY_TIME);
        scl = 1;
        byt <<= 1;
		I2C_Delay(DELAY_TIME);
    }
	
    scl = 0;  
}

//
unsigned char I2CReceiveByte(void)
{
	unsigned char da;
	unsigned char i;
	for(i=0;i<8;i++){   
		scl = 1;
		I2C_Delay(DELAY_TIME);
		da <<= 1;
		if(sda) 
			da |= 0x01;
		scl = 0;
		I2C_Delay(DELAY_TIME);
	}
	return da;    
}

//
unsigned char I2CWaitAck(void)
{
	unsigned char ackbit;
	
    scl = 1;
	I2C_Delay(DELAY_TIME);
    ackbit = sda; 
    scl = 0;
	I2C_Delay(DELAY_TIME);
	
	return ackbit;
}

//
void I2CSendAck(unsigned char ackbit)
{
    scl = 0;
    sda = ackbit; 
	I2C_Delay(DELAY_TIME);
    scl = 1;
	I2C_Delay(DELAY_TIME);
    scl = 0; 
	sda = 1;
	I2C_Delay(DELAY_TIME);
}







//函数名：ADC转换函数
//入口参数：要进行转换的通道控制位
//返回值：ADC转换的数值
//函数功能：对指定的通道进行ADC转换，函数返回转换的数值
unsigned char Pcf8591_Adc(unsigned char channel_num_contrl)
{
	unsigned char temp;
	
	I2CStart();//发送开启信号
	I2CSendByte(0x90);//选择PCF8591芯片，确定写的模式
	I2CWaitAck();//等待PCF8591反馈
	
	I2CSendByte(channel_num_contrl);//确定要转换的通道（顺便，使能DA转换）
	I2CWaitAck();//等待PCF8591反馈	
	
	I2CStart();//发送开启信号
	I2CSendByte(0x91);//选择PCF8591芯片，确定读的模式
	I2CWaitAck();//等待PCF8591反馈	
		
	temp = I2CReceiveByte();//接收数据
	I2CSendAck(1);//选择不应答
	I2CStop();//停止发送
	
	return temp;

}



//函数名：DAC转换函数
//入口参数：要进行转换的数值
//返回值：无
//函数功能：对入口参数要转换的DA数据进行转换
void Pcf8591_Dac(unsigned char trans_dat)
{
	I2CStart();//发送开启信号
	I2CSendByte(0x90);//选择PCF8591芯片，确定写的模式
	I2CWaitAck();//等待PCF8591反馈
	
	I2CSendByte(0x41);//使能DA转换（随便写通道编号，不影响，主要的功能是使能DA）
	I2CWaitAck();//等待PCF8591反馈		

	I2CSendByte(trans_dat);//将待转换的数据发送出去
	I2CWaitAck();//等待PCF8591反馈	
	I2CStop();//停止发送	

}



//函数名：写EEPROM函数
//入口参数：需要写入的字符串，写入的地址(务必为8的倍数)，写入数量
//返回值：无
//函数功能：向EERPOM的某个地址写入字符串中特定数量的字符。
void EEPROM_Write(unsigned char* EEPROM_String, unsigned char addr, unsigned char num)
{
	I2CStart();//发送开启信号
	I2CSendByte(0xA0);//选择EEPROM芯片，确定写的模式
	I2CWaitAck();//等待EEPROM反馈
	
	I2CSendByte(addr);//写入要存储的数据地址
	I2CWaitAck();//等待EEPROM反馈		

	while(num--)
	{
		I2CSendByte(*EEPROM_String++);//将要写入的信息写入
		I2CWaitAck();//等待EEPROM反馈		
		I2C_Delay(200);	
	}
	I2CStop();//停止发送	
}


//函数名：读EEPROM函数
//入口参数：读到的数据需要存储的字符串，读取的地址(务必为8的倍数)，读取的数量
//返回值：无
//函数功能：读取EERPOM的某个地址中的数据，并存放在字符串数组中。
void EEPROM_Read(unsigned char* EEPROM_String, unsigned char addr, unsigned char num)
{
	I2CStart();//发送开启信号
	I2CSendByte(0xA0);//选择EEPROM芯片，确定写的模式
	I2CWaitAck();//等待EEPROM反馈
	
	I2CSendByte(addr);//写入要读取的数据地址
	I2CWaitAck();//等待EEPROM反馈		

	I2CStart();//发送开启信号
	I2CSendByte(0xA1);//选择EEPROM芯片，确定读的模式
	I2CWaitAck();//等待EEPROM反馈	
	
	while(num--)
	{
		*EEPROM_String++ = I2CReceiveByte();//将要写入的信息写入
		if(num) I2CSendAck(0);//发送应答
			else I2CSendAck(1);//不应答
	}
	
	I2CStop();//停止发送	
}




