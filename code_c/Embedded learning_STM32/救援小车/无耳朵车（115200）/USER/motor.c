

#include <stdio.h>

#include "stm32f10x.h"

#include "delay.h"

#include "cfg.h"


/*机械臂所有动作代码*/
/*以超声波和驱动轮所在的一方为前方*/
/*从车体后方看过去机械臂最下面左侧为1号电机，右侧为2号电机，依次往上为3、4、5号电机*/
/*定时器Tim1的四个参数分别代表1、2、3、4号电机，Tim2的第一个参数代表5号电机，其余3个参数无效*/

void catop(void) 
{

	int n1,n2,n3,n5;
	double k=0;
	Tim1_Init(1500,1560,1500,1500);//直立值
	delay_ms(1000);
	Tim2_Init(2100,1600,270,250);//第五个电机开始的位置	2100 
	for(n2=1560;n2<=2070;n2=n2+2)
	{
	  n1=3000-n2;
	  n3=1500-0.55*k;
		Tim1_Init(n1,n2,n3,1400);
		k=k+3;
		delay_ms(10);
	} 
	delay_ms(10);
	
	for(n5=2100;n5<=2540;n5=n5+8)
			{
				Tim2_Init(n5,1600,270,250);
				delay_ms(10);	
			}
	
	delay_ms(1500);
	Tim1_Init(1500,1560,1500,1500);//直立值
	
}

void camid(void)  //抓起中间一个
{
	int n1,n2,n3,n5;
	double k=0;
	Tim1_Init(1500,1560,1500,1500);//直立值
	delay_ms(1000);
	Tim2_Init(2100,1600,270,250);//第五个电机开始的位置	1150  1700
   
	for(n2=1560;n2<=2200;n2=n2+3)
	{
	  n1=3100-n2;
	  n3=1500-0.55*k;
		Tim1_Init(n1,n2,n3,1300);
		k=k+4;
		delay_ms(10);
	} 
	delay_ms(500);
	
	for(n5=2100;n5<=2540;n5=n5+10)
			{
				Tim2_Init(n5,1600,270,250);
				delay_ms(10);	
			}
	
	delay_ms(1000);
	Tim1_Init(1500,1560,1500,1500);//直立值

}

void cabottom(void)  //抓起最下面一个
{
	int n1,n2,n3,n5;
	double k=0;
	Tim1_Init(1500,1560,1500,1500);//直立值
	delay_ms(1000);
	Tim2_Init(2100,1600,270,250);//第五个电机开始的位置	1150 
   	
	for(n2=1560;n2<=2250;n2=n2+2)
	{
	  n1=3000-n2;
	  n3=1500-0.5*k;
		Tim1_Init(n1,n2,n3,1100);
		k=k+4;
		delay_ms(10);
	}
	
	delay_ms(500);
	for(n5=2100;n5<=2540;n5=n5+10)
			{
				Tim2_Init(n5,1600,270,250);
				delay_ms(10);	
			}
	
	delay_ms(500);
	Tim1_Init(1500,1560,1500,1500);//直立值
	
}

void putbottom(void)  //放下底下一个
{

	int n1,n2,n3,n5;
	double k=0;
	Tim1_Init(1500,1560,1500,1500);//直立值
	Tim2_Init(2540,1600,270,250); //第五个电机抓的动作
	delay_ms(1000);
	
  	for(n2=1560;n2<=2250;n2=n2+2)
	{
	  n1=3000-n2;
	  n3=1500-0.5*k;
		Tim1_Init(n1,n2,n3,1100);
		k=k+4;
		delay_ms(10);
	}
	delay_ms(500);
	
	for(n5=2540;n5>=2100;n5=n5-10)
	{
		Tim2_Init(n5,1600,270,250);
		delay_ms(100);	
	}
	delay_ms(500);

	Tim1_Init(1500,1560,1500,1500);//直立值

}

void putbottom1(void)  //放下底下一个
{

	int n1,n2,n3,n5;
	double k=0;
	Tim1_Init(1500,1560,1500,1500);//直立值
	Tim2_Init(2540,1600,270,250); //第五个电机抓的动作
	delay_ms(1000);
	
  	for(n2=1560;n2<=2250;n2=n2+2)
	{
	  n1=3000-n2;
	  n3=1500-0.5*k;
		Tim1_Init(n1,n2,n3,1100);
		k=k+4;
		delay_ms(10);
	}
	delay_ms(500);
	
	for(n5=2540;n5>=2100;n5=n5-10)
	{
		Tim2_Init(n5,1600,270,250);
		delay_ms(100);	
	}
	delay_ms(500);

	Tim1_Init(1500,1560,1500,1500);//直立值

}

void putmid(void)  //放下中间
{
	int n1,n2,n3,n5;
	double k=0;
	Tim1_Init(1500,1560,1500,1500);//直立值
	Tim2_Init(2540,1600,270,250); //第五个电机抓的动作
	delay_ms(1000);
	
   		for(n2=1560;n2<=2200;n2=n2+3)
	{
	  n1=3100-n2;
	  n3=1500-0.55*k;
		Tim1_Init(n1,n2,n3,1300);
		k=k+4;
		delay_ms(10);
	} 
	delay_ms(500);
	
	for(n5=2540;n5>=2100;n5=n5-10)
	{
		Tim2_Init(n5,1600,270,250);
		delay_ms(10);	
	}
	
	delay_ms(600);
	Tim1_Init(1500,1540,1500,1500);//直立值
}

 void puttop()  //放下最上面一个
{
int n1,n2,n3,n5;
	double k=0;
	Tim1_Init(1500,1560,1500,1500);//直立值
	Tim2_Init(2540,1600,270,250); //第五个电机抓的动作
	delay_ms(1000);
	
   		for(n2=1560;n2<=2070;n2=n2+2)
	{
	  n1=3000-n2;
	  n3=1500-0.55*k;
		Tim1_Init(n1,n2,n3,1400);
		k=k+3;
		delay_ms(10);
	} 
	delay_ms(800);
	
	for(n5=2540;n5>=2100;n5=n5-5)
	{
		Tim2_Init(n5,1600,270,250);
		delay_ms(10);	
	}
	
	delay_ms(300);
	Tim1_Init(1500,1560,1500,1500);//直立值
}

void turnmotordown()//第四个电机后转90度，并打开手抓，再合拢防止物料掉落
{
	int n4,n5;
	for(n4=1500;n4>=560;n4=n4-5)
	{
		Tim1_Init(1500,1560,1500,n4);
		delay_ms(10);
	} 
	
		delay_ms(500);
	for(n5=2545;n5>=2100;n5=n5-5)
	{
		Tim2_Init(n5,1600,270,250);
		delay_ms(10);	
	}
	delay_ms(800);
	for(n5=2100;n5<=2545;n5=n5+10)
	{
		Tim2_Init(n5,1600,270,250);
		delay_ms(10);	
	}
	
}

void turnmotorup() //闭合手抓，并机械手恢复直立姿态
{
	int n4,n5;
	for(n5=2540;n5<=2100;n5=n5+5)
	{
		Tim2_Init(n5,1600,270,250);
		delay_ms(10);	
	}
	delay_ms(500);
	for(n4=560;n4<=1500;n4=n4+5)
	{
		Tim1_Init(1500,1560,1500,n4);
		delay_ms(10);
	} 
	delay_ms(500);
}

void holdhand()//保持直立姿态
{
	Tim1_Init(1500,1560,1500,1500);
	Tim2_Init(2540,1600,270,250);	
}

void turnmotor()//转动手抓
{

}

