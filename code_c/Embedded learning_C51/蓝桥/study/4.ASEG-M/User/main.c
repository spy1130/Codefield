#include "STC15F2K60S2.H"
#include "init.h"
#include "Delay.h"
#include "SEG.h"
#include "STDIO.H"
#include "STRING.H"
#include "timer.h"

//全局变量
unsigned char seg_string[10];
unsigned char month=0;
unsigned char seg_buf[18];//存转换后的数
unsigned char pos=0;
bit tim1_int_sign =0;//tim1中断标识位
unsigned char len;//字符串的长度
unsigned int one_ms_trigger_count=0;//每一毫秒触发一次,0-65535

void main()
{	init_close();
	tim1_init();
	EA=1;//打开总中断
	while(1){
	//定时器1回调函数，每1ms触发，第一个函数，每1s控制显示变量
	if(tim1_int_sign)
	{
		tim1_int_sign = 0;//如果识别到中断标志变化，复位中断标志。
		one_ms_trigger_count++;	//毫秒计数
	}
			
	if((!(one_ms_trigger_count%1000))&&(one_ms_trigger_count>0))//到达1s
	{
		one_ms_trigger_count = 0;
		month +=1;
		if(month>12)
		{
			month =1;
		}
		
	}
	len=sprintf(seg_string,"2018--%02u",(unsigned int)month);//指定8位宽度
	NumTran(seg_string,seg_buf);
	}
		
}

/* Timer1 interrupt routine */
void tm1_isr() interrupt 3
{
    tim1_int_sign = 1;
	NumShow(seg_buf, pos);
	pos++;
	if(pos == 8)
		pos = 0;
	
}