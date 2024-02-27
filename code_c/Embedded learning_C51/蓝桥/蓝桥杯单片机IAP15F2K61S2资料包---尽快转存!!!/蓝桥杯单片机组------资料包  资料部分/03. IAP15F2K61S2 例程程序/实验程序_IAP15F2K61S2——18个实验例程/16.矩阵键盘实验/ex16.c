/*******************************************************************************  
* 文件名称：矩阵键盘实验
* 实验目的：1.掌握矩阵键盘扫描的基本原理
*           2.掌握按键扫描及消除抖动的基本原理
* 程序说明：1.使用程序前，将跳线J2调整为KBD模式
*           2.转接板上使用P4.4和P4.2代替P3.6 P3.7
* 硬件环境：IAP15F2K61S2@11.0592MHz
* 日期版本：2012-9-4/V1.0a 
*******************************************************************************/

#include "reg52.h"  //定义51单片机特殊功能寄存器
#include "absacc.h"

sfr AUXR = 0x8E;
sfr P4   = 0xC0;	//P4
sbit P42 = P4^2;
sbit P44 = P4^4;
                           //  0    1    2    3    4    5    6    7    8    9   熄灭
code unsigned char tab[] = { 0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xFF};
unsigned char dspbuf[8] = {10,10,10,10,10,10,10,10};  //显示缓冲区
unsigned char dspcom = 0;

bit key_re;
unsigned char key_press;
unsigned char key_value;

bit key_flag = 0;
unsigned char intr = 0;

unsigned char read_keyboard(void);
void display();

//主函数
void main(void)
{ 
    unsigned char key_temp = 0xff;
    
	AUXR |= 0x80;	//1T模式，IAP15F2K61S2单片机特殊功能寄存器
	
	TMOD &= 0xF0;
	TL0 = 0xCD;	
	TH0 = 0xD4;	
	TF0 = 0;	
	TR0 = 1;
	ET0 = 1;
	EA = 1;
    
    while(1)
    {
        if(key_flag)
        {
            key_flag = 0;
			key_temp = read_keyboard();
			if(key_temp != 0xFF){
				dspbuf[7] = key_temp%10;
				dspbuf[6] = key_temp/10;
			}
        } 
        
    }
}

//定时器中断服务函数
void isr_timer_0(void)  interrupt 1  //默认中断优先级 1
{     
	display();
	if(++intr == 10)  //1ms执行一次
	{
        intr = 0;
		key_flag = 1;  //10ms按键扫描标志位置1
    }
}

//读取矩阵键盘键值:转接板中使用P42和P44代替8051引脚
//顺序中的P36和P37引脚
unsigned char read_keyboard(void)
{
    static unsigned char col;
    
	P3 = 0xf0; P42 = 1; P44 = 1;

    if((P3 != 0xf0)||(P42 != 0)||(P44 != 0)) //有按键按下
        key_press++;
	else
		key_press = 0;  //抖动
    
    if(key_press == 3)
    {
		key_press = 0;
		key_re = 1;
		
		if(P44 == 0)			col = 1;
		if(P42 == 0)			col = 2;
		if((P3 & 0x20) == 0)	col = 3;
		if((P3 & 0x10) == 0)	col = 4;
        
        P3 = 0x0F; P42 = 0; P44 = 0;

		if((P3&0x01) == 0)	key_value = (col-1);
		if((P3&0x02) == 0)	key_value = (col+3);
		if((P3&0x04) == 0)	key_value = (col+7);
		if((P3&0x08) == 0)	key_value = (col+11);
    }
    
	//连续三次检测到按键被按下，并且该按键已经释放
	P3 = 0x0f; P42 = 0; P44 = 0;
	
    if(((key_re == 1) && (P3 == 0x0f))&&(P42 == 0)&&(P44 == 0))
    {
        key_re = 0;
        return key_value;
    }
    
    return 0xff;  //无按键按下或被按下的按键未被释放 
}

//显示函数
void display(void)
{   
	P0 = 0xff;
	P2 = ((P2&0x1f)|0xE0); 
	P2 &= 0x1f;

	P0 = 1<<dspcom;
	P2 = ((P2&0x1f)|0xC0); 
	P2 &= 0x1f;

	P0 = tab[dspbuf[dspcom]];	
    P2 = ((P2&0x1f)|0xE0); 
	P2 &= 0x1f;
  
    if(++dspcom == 8){
        dspcom = 0;
    }    
}