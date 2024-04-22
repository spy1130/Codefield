# include "delay.h"


//函数名：延时毫秒函数
//入口参数：延时的数值，2个字节，16位，0---65535
//返回值：无
//函数功能：延时一定的毫秒数据
void Delay(unsigned int num)
{
	unsigned int i;
	while(num--)
	for(i=0; i<628; i++);
}
