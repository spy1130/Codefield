/*头文件声明区*/
#include <STC15F2K60S2.H>//单片机寄存器专用头文件
#include <init.H>//初始化底层驱动专用头文件
#include <key.H>//按键底层驱动专用头文件
#include <led.H>//LED底层驱动专用头文件
#include <seg.H>//数码管底层驱动专用头文件
#include <iic.h>//PCF8591底层驱动专用头文件

/*变量声明区*/
unsigned char key_slow_down,seg_slow_down;//减速专用变量
unsigned char key_val,key_down,key_up,key_old;//按键检测专用变量
unsigned char seg_buf[8]={10,10,10,10,10,10,10,10};//数码管显示数据存放数组
unsigned char seg_point[8]={0,0,0,0,0,1,0,0};//小数点显示数据存放数组
unsigned char led_buf[8]={0,0,0,0,0,0,0,0};//LED点亮数据存放数组
unsigned char pos;//数据存放数组专用指针
bit seg_mord;//数码管显示界面标志位 0-电压显示界面 1-电压输出界面
float dat;//测量电压值专用变量
bit dat_flag;//电压值标志位 0-2.00 1-RB2
bit led_flag=1;//LED使能标志位 0-关闭 1-开启
bit seg_flag=1;//数码管使能标志位 0-关闭 1-开启

/*按键处理函数*/
void key_proc()
{
	//按键减速程序
	if(key_slow_down)	return;
	key_slow_down = 1;
	
	//按键检测程序
	key_val = key_read();//实时获取键码值
	key_down = key_val&(key_val^key_old);//捕获按键下降沿
	key_up = ~key_val&(key_val^key_old);//捕获按键上升沿
	key_old = key_val;//辅助扫描按键
	
	//按键调用程序
	switch(key_down)
	{
		case 4:
			seg_mord ^= 1;
		break;
		case 5:
			dat_flag ^= 1;
		break;
		case 6:
			led_flag ^= 1;
		break;
		case 7:
			seg_flag ^= 1;
		break;
	}
}

/*信息处理函数*/
void seg_proc()
{
	//数码管减速程序
	if(seg_slow_down)	return;
	seg_slow_down = 1;
	
	//PCF8591调用程序
	dat = ad_read(0x43)/51.0;//将采集的数据赋给电压值
	
	//数码管显示程序
	if(seg_mord)//处于电压输出界面
	{
		seg_buf[0] = 12;//标识符显示U
		if(dat_flag)
		{
			seg_buf[5] = (unsigned char)dat;
			seg_buf[6] = (unsigned char)(dat*100)/10%10;
			seg_buf[7] = (unsigned char)(dat*100)%10;//数码管显示RB2
		}
		else
		{
			seg_buf[5] = 2;
			seg_buf[6] = seg_buf[7] = 0;//数码管显示2.00
		}
	}
	else//处于电压显示界面
	{
		seg_buf[0] = 11;//标识符显示F
		seg_buf[5] = (unsigned char)dat;
		seg_buf[6] = (unsigned char)(dat*100)/10%10;
		seg_buf[7] = (unsigned char)(dat*100)%10;//数码管显示测量电压值
	}
}

/*其他显示函数*/
void led_proc()
{
	led_buf[0] = seg_mord?0:1;//判断数码管显示界面,确定L1是否点亮
	led_buf[1] = !led_buf[0];//L2状态与L1相反
	led_buf[2] = (dat < 1.5||(dat<3.5&&dat>=2.5))?0:1;//判断输出电压大小,确定L3是否点亮
	led_buf[3] = dat_flag?1:0;//判断输出电压状态,确定L4是否点亮

}

/*定时器0初始化函数*/
void Timer0Init(void)		//1毫秒@12.000MHz
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x18;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0 = 1;//定时器0中断打开
	EA = 1;//总中断打开
}

/*定时器0中断服务函数*/
void timer0_server() interrupt 1
{
	if(++key_slow_down == 10)	key_slow_down = 0;//按键减速专用
	if(++seg_slow_down == 500)	seg_slow_down = 0;//数码管减速专用
	if(++pos == 8)	pos = 0;//数据存放数组扫描专用
	
	//数码管显示专用
	if(seg_flag)
		seg_disp(pos,seg_buf[pos],seg_point[pos]);
	else
		seg_disp(pos,10,0);
	
	//LED点亮专用
	if(led_flag)
		led_disp(pos,led_buf[pos]);
	else
		led_disp(pos,0);
}

/*主函数*/
void main()
{
	system_init();
	Timer0Init();
	while(1)
	{
		key_proc();
		seg_proc();
		led_proc();
	}
}