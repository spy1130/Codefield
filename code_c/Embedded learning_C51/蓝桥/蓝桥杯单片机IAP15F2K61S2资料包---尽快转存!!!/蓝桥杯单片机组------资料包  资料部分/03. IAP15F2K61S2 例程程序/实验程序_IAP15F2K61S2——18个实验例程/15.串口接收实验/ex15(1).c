/*******************************************************************************  
* 文件名称：串口通讯实验
* 实验目的：1.掌握51单片机串口工作模式及相关寄存器配置方法
*           2.了解51单片机波特率的计算方法
*           3.掌握单片机串口接收中断服务函数的设计方法
* 程序说明：1.通过USB连接线连接PC机，在计算机硬件管理器中查看串口号
*           2.打开串口调试工具软件，并将串口通讯波特率设定为2400
*			3.将跳线J13配置为MM模式
*			4.输入1-8字符，控制8个LED指示灯状态
* 硬件说明：IAP15F2K61S2@11.0592MHz
* 日期版本：2012-9-5/V1.0a 
*******************************************************************************/

#include "reg52.h"  //定义51单片机特殊功能寄存器
#include "absacc.h"

#define BAUD	     2400  		//波特率
#define SYSTEMCLOCK  11059200L  //系统时钟频率

sfr AUXR  = 0x8e; 

bit rxc = 0;
char rx = '1';
void uart_sendstring(unsigned char *str);

//主函数
void main(void)
{ 
    SCON = 0x50;                

    AUXR = 0x40;                //1T
    TMOD = 0x00;                //
    TL1 = (65536 - (SYSTEMCLOCK/4/BAUD));   //
    TH1 = (65536 - (SYSTEMCLOCK/4/BAUD))>>8;
    TR1 = 1;  
	ES = 1;
	EA = 1;
	
    while(1)
    {
		if(rxc == 1){
			rxc = 0;
			switch(rx){
				case '1':
					XBYTE[0x8000] = 0xFE;
					break;
				case '2':
					XBYTE[0x8000] = 0xFD;
					break;
				case '3':
					XBYTE[0x8000] = 0xFB;
					break;
				case '4':
					XBYTE[0x8000] = 0xF7;
					break;
				case '5':
					XBYTE[0x8000] = 0xEF;
					break;
				case '6':
					XBYTE[0x8000] = 0xDF;
					break;
				case '7':
					XBYTE[0x8000] = 0xBF;
					break;
				case '8':
					XBYTE[0x8000] = 0x7F;
					break;
				default:
					uart_sendstring("error\r\n");	//输入其他数据返回error
					break;
			}
			ES = 1;
		}
    }
}

//串口中断服务函数
void isr_uart(void) interrupt 4
{
    if(RI)
    {
        RI = 0;  //清除接收标志位 
        rx = SBUF;
		ES = 0;
		rxc = 1;
	}
}

//通过串口发送字符串
void uart_sendstring(unsigned char *str)
{
    unsigned char *p;
    
    p = str;
    while(*p != '\0')
    {
        SBUF = *p;
		while(TI == 0);  //等待发送标志位置位
		TI = 0;
        p++;
    }
}