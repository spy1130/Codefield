/* 第二届 创意智造·奠基未来 单片机设计与开发大赛 专用模板 */
 
/* 头文件声明区 */
#include <REGX52.H>//单片机寄存器专用头文件
#include <Key.h>//按键底层驱动专用头文件
#include <Seg.h>//数码管底层驱动专用头文件

/* 变量声明区 */
unsigned char Key_Val,Key_Down,Key_Old;//按键专用变量
unsigned char Key_Slow_Down;//按键减速专用变量
unsigned char Seg_Buf[6] = {10,10,10,10,10,10};//数码管显示数据存放数组
unsigned char Seg_Pos;//数码管扫描专用变量
unsigned int Seg_Slow_Down;//数码管减速专用变量
unsigned char mord;//显示功能标志位 0-运行状态界面 1-流转时间设置界面 2-数据显示界面
unsigned char zt[6];//运行状态数据存放数组
unsigned char zt_flag;//运行状态运行标志位 0-关闭 1-开启
unsigned char ms_flag=1;//运行模式标志位
unsigned int jg=4;//流转时间间隔设置辅助变量
unsigned char sz[6];//流转时间设置界面数据存放数组
unsigned char sz_zz;//流转时间设置界面指针
unsigned int time_800ms;//800ms闪烁变量
unsigned char sz_ss;//流转时间设置界面闪烁专用变量 
unsigned char led=0xfe;//P1端口专用变量
unsigned int jg_pro;//流转时间间隔设置专用变量
unsigned int jg_time;//流转时间计时专用变量
unsigned char led1[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};//运行模式1,2数据存放数组
unsigned char led1_zz;//运行模式1,2数据存放数组指针
unsigned char led3[]={0x7e,0xbd,0xdb,0xe7};//运行模式3,4数据存放数组
unsigned char led3_zz;//运行模式3,4数据存放数组指针
bit ms_flag_pro;
unsigned char xs[6]={13,14,12,1,10,10};

/* 键盘处理函数 */
void Key_Proc()
{
	if(Key_Slow_Down) return;
	Key_Slow_Down = 1;//键盘减速程序

	Key_Val = Key_Read();//实时读取键码值
	Key_Down = Key_Val & (Key_Old ^ Key_Val);//捕捉按键下降沿
	Key_Old = Key_Val;//辅助扫描变量
	
	if(zt[0]==5)//进入数据显示界面
	{
		if(Key_Old==4)
		{
			mord=2;
		}
	}
	
	switch(Key_Down)
	{
		case 7://启动/停止
			zt_flag=!zt_flag;
		break;
		case 6://设置
			if(zt[0]==5)
			{
				if(mord==0)//进入设置界面
				{
					mord=1;
				}
				if(mord==1)
				{
					sz_zz++;
					if(sz_zz==3)//保存设置数据并返回运行状态界面
					{	
						mord=0;
						ms_flag_pro=1;
						sz_zz=0;
					}
				}
			}
		break;
		case 4://加
			if(mord==1)
			{
				if(sz_zz==1)//加模式编号
				{
					ms_flag++;
					if(ms_flag==5)ms_flag=1;
				}
				if(sz_zz==2)//加流转间隔
				{
					jg++;
					if(jg==13)jg=4;
					if(jg>=10)sz[2]=jg/10;
				}
			}
		break;
		case 5://减
			if(mord==1)
			{
				if(sz_zz==1)//减模式编号
				{
					ms_flag--;
					if(ms_flag==0)ms_flag=4;
				}
				if(sz_zz==2)//减流转间隔
				{
					jg--;
					if(jg==3)jg=12;
					if(jg>=10)sz[2]=jg/10;
				}
			}
		break;
	}
}

/* 信息处理函数 */
void Seg_Proc()
{
	unsigned char i;
	if(Seg_Slow_Down) return;
	Seg_Slow_Down = 1;//数码管减速程序
	switch(mord)
	{
		case 0://运行状态界面
			zt[0]=zt_flag?11:5;
			zt[1]=ms_flag;
			if(jg>=10)zt[2]=jg/10;
			else zt[2]=10;
			zt[3]=jg%10;
			zt[4]=0;
			zt[5]=0;
			for(i=0;i<6;i++)Seg_Buf[i]=zt[i];
		break;
		case 1://流转时间设置界面
			sz[0]=12;
			sz[1]=ms_flag;
			if(jg>=10)sz[2]=jg/10;
			else sz[2]=10;
			sz[3]=jg%10;
			sz[4]=0;
			sz[5]=0;
			for(i=0;i<6;i++)Seg_Buf[i]=sz[i];
			if(sz_zz==1)//运行模式编号闪烁
			{
				for(i=0;i<2;i++)Seg_Buf[i]=sz_ss?sz[i]:10;
			}
			if(sz_zz==2)//流转间隔闪烁
			{
				for(i=2;i<6;i++)Seg_Buf[i]=sz_ss?sz[i]:10;
			}
		break;
		case 2:
			for(i=0;i<3;i++)Seg_Buf[i]=xs[i];
			Seg_Buf[3]=ms_flag;
			if(ms_flag==1||ms_flag==2)
			{	
				Seg_Buf[4]=0;
				Seg_Buf[5]=led1_zz+1;
			}
			if(ms_flag==3||ms_flag==4)
			{
				Seg_Buf[4]=led3_zz+1;
				Seg_Buf[5]=3-led3_zz;
			}
		break;
	}
}

/* 其他显示函数 */
void Led_Proc()
{
	jg_pro=jg*100;
	if(zt[0]==11)
	{
		if(ms_flag==1)//模式1
		{
			led=led1[led1_zz];
			if(led1_zz==8)
			{
				if(ms_flag_pro==0)ms_flag=2;
				else led1_zz=0;
			}
		}	
		if(ms_flag==2)//模式2
		{
			led=led1[led1_zz];
			if(led1_zz==255)
			{
				if(ms_flag_pro==0)ms_flag=3;
				else led1_zz=7;
			}
		}
		if(ms_flag==3)//模式3
		{
			led=led3[led3_zz];
			if(led3_zz==4)
			{	
				if(ms_flag_pro==0)ms_flag=4;
				else led3_zz=0;
			}
		}			
		if(ms_flag==4)//模式4
		{
			led=led3[led3_zz];
			if(led3_zz==255)
			{	
				if(ms_flag_pro==0)ms_flag=1;
				else led3_zz=3;
			}
		}
	}
	P1=led;
}

/* 定时器0中断初始化函数 */
void Timer0Init(void)		//1毫秒@12.000MHz
{
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
	TL0 = 0x18;		//设置定时初始值
	TH0 = 0xFC;		//设置定时初始值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0 = 1;        //定时器0中断打开
	EA = 1;         //总中断打开
}

/* 定时器0中断服务函数 */
void Timer0Server() interrupt 1
{
 	TL0 = 0x18;		//设置定时初始值
	TH0 = 0xFC;		//设置定时初始值   
	if(++Key_Slow_Down == 10) Key_Slow_Down = 0;//键盘减速专用
	if(++Seg_Slow_Down == 500) Seg_Slow_Down = 0;//数码管减速专用
	if(++Seg_Pos == 6) Seg_Pos = 0;//数码管显示专用
	Seg_Disp(Seg_Pos,Seg_Buf[Seg_Pos]);
	if(++time_800ms==800)//设置界面闪烁专用
	{
		time_800ms=0;
		sz_ss=!sz_ss;
	}
	if(zt[0]==11)
	{
		if(++jg_time==jg_pro)
		{
			jg_time=0;
			if(ms_flag==1)led1_zz++;
			if(ms_flag==2)led1_zz--;
			if(ms_flag==3)
			{	
				led3_zz++;
				if(led3_zz==4)led3_zz=0;
			}
			if(ms_flag==4)
			{
				led3_zz--;
				if(led3_zz==255)led3_zz=3;
			}
		}
	}
}

/* Main */
void main()
{
	Timer0Init();
	while (1)
	{
		Key_Proc();
		Seg_Proc();
		Led_Proc();
	}
}