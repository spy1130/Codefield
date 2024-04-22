# include "bsp_init.h"
# include "bsp_led.h"
# include "delay.h"

void main()
{
	unsigned char i,j;//为循环变量做准备
	
	Cls_Peripheral();//关闭外设
	
	while(1)
	{
		for(i=0;i<9;i++)//控制亮度等级
		{
			for(j=0;j<50;j++)//控制当前亮度等级的循环次数
			{
   			 Led_Disp(0xff);//全亮
			   Delay(i+1);//亮的时间
				 Led_Disp(0x00);//全灭
			   Delay(9-i);//灭的时间						
			}
		}
	}
}	











