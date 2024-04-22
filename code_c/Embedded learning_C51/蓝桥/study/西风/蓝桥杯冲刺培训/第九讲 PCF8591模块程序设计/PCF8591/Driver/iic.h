# include "STC15F2K60S2.H"
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
unsigned char AD_Read(unsigned char channel_num_contrl);


//函数名：DAC转换函数
//入口参数：要进行转换的数值
//返回值：无
//函数功能：对入口参数要转换的DA数据进行转换
void DA_Write(unsigned char trans_dat);
