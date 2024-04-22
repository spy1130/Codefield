#include <ultrasound.h>
#include <intrins.h>

sbit Tx = P1^0;
sbit Rx = P1^1;

void Delay12us()		//@12.000MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 33;
	while (--i);
}


void Ul_Init()
{
	unsigned char i;
	EA = 0;
	for(i=0;i<8;i++)
	{
		Tx = 1;
		Delay12us();
		Tx = 0;
		Delay12us();
	}
	EA = 1;
}

unsigned char Wave_Read()
{
	unsigned int time;
	TMOD &= 0x0F;
	TH1 = TL1 = 0;//复位计数值 等待超声波信号发出
	Ul_Init();
	TR1 = 1;//开始计时
	while((Rx == 1) && (TF1 == 0));//等待接受返回信号或者定时器溢出
	TR1 = 0;//停止计时
	if(TF1 == 0) //定时器没有溢出
	{
		time = TH1 << 8 | TL1;//读取当前时间
		return (time * 0.017);//返回距离值
	}
	else
	{
		TF1 = 0;//清除溢出标志位
		return 0;
	}
}