/*******************************************************************************  
* 文件名称：LED移位控制实验
* 实验目的：掌握单片机IO口操作的基本方法
* 程序说明：无
* 硬件环境：IAP15F2K61S2@11.0592MHz
* 日期版本：2012-9-3/V1.0a 
*******************************************************************************/

#include "reg52.h"  //定义51单片机特殊功能寄存器
#include "intrins.h"

//关闭外设
void cls_buzz(void)
{
	P2 = (P2&0x1F|0xA0);
	P0 = 0x00;
	P2 &= 0x1F;
}

//延时函数
void delay(void)
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 9;
	j = 104;
	k = 139;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

//主函数
void main(void)
{
    unsigned char i;
	
	cls_buzz();
    
    while(1)
    {
		for(i=0; i<8; i++)
		{
			P2 = ((P2 &0x1f)|0x80);
			P0 = 0xfe<<i;  //左移i位
			P2 &= 0x1f;
			delay();
		}
    }
}