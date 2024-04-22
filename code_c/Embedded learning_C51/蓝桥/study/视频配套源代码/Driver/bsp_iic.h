#include "STC15F2K60S2.H"
#include "intrins.h"

#define DELAY_TIME 5

#define Photo_Res_Channel 0x41
#define Adj_Res_Channel 0x43

void IIC_Start(void); 
void IIC_Stop(void);  
bit IIC_WaitAck(void);  
void IIC_SendAck(bit ackbit); 
void IIC_SendByte(unsigned char byt); 
unsigned char IIC_RecByte(void); 

//函数名：ADC转换函数
//入口参数：要进行转换的通道控制位
//返回值：ADC转换的数值
//函数功能：对指定的通道进行ADC转换，函数返回转换的数值
unsigned char Pcf8591_Adc(unsigned char channel_num_contrl);


//函数名：DAC转换函数
//入口参数：要进行转换的数值
//返回值：无
//函数功能：对入口参数要转换的DA数据进行转换
void Pcf8591_Dac(unsigned char trans_dat);

//函数名：写EEPROM函数
//入口参数：需要写入的字符串，写入的地址(务必为8的倍数)，写入数量
//返回值：无
//函数功能：向EERPOM的某个地址写入字符串中特定数量的字符。
void EEPROM_Write(unsigned char* EEPROM_String, unsigned char addr, unsigned char num);


//函数名：读EEPROM函数
//入口参数：读到的数据需要存储的字符串，读取的地址(务必为8的倍数)，读取的数量
//返回值：无
//函数功能：读取EERPOM的某个地址中的数据，并存放在字符串数组中。
void EEPROM_Read(unsigned char* EEPROM_String, unsigned char addr, unsigned char num);