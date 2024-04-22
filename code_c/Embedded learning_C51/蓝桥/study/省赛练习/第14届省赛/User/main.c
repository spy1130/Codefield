//先Seg减速，顺带定时器别忘了ET0=1,EA=1;Seg扫描，led+扫描，Key减速+三行消抖，主程序
#include <STC15F2K60S2.H>
#include <init.h>
#include <Led.h>
#include <Key.h>
#include <Seg.h>
#include "ds1302.h"
#include "onewire.h"
#include "iic.h"

#define u8 unsigned char
#define u16 unsigned int
unsigned char Seg_Slow_Down;
unsigned char Seg_Buf[8]={10,10,10,10,10,10,10,10};
unsigned char Seg_Point[8]={0,0,0,0,0,0,0,0};
unsigned char Seg_Pos;
u8 Seg_Disp_Mode;//0-时间，1-回显，2-参数
u8 Disp_Mode;//0-温度回显，1-湿度回显，2-时间回显
unsigned char ucLed[8]={0,0,0,0,0,0,0,0};

unsigned char Key_Val,Key_Old,Key_Down,Key_Up;
unsigned char Key_Slow_Down;
u16 Time_1000ms;//频率延时函数
u16 time_2000ms;//长按延迟函数
u16 time_3000ms;//触发延迟函数
u8 time_100ms;//闪烁延迟函数
u16 Freq;//频率
u8 ucRtc[3]={0x13,0x03,0x05};
float temperature;//温度
u8 hum;//湿度
float old_temperature;//上一次温度
float aver_temperature;//平均温度
float temp_Max;//温度最大值显示
float old_hum;//上一次湿度
float max_hum;//最大湿度
float aver_hum;//平均湿度
u8 trigger_time[3];//触发时间
u8 count=0;//采集次数
float temp_para_Disp=30;//温度参数显示
float temp_para_Save=30;//温度参数真实
u8 old_light_value;//光强参数
bit tri_flag;//触发标志位
bit long_press_flag;//长按标志
bit warn_flag;//温度超过报警标志位
bit Led_star_flag;//闪烁标志位
bit error_flag;//有无效数据标志位
bit up_flag;//温湿度是否都升高标志位
//湿度处理函数
float rd_hum()
{	
	float hum_val;
	if(Freq<200 ||Freq>2000)
	return 0;
	hum_val=(Freq-200)*2/45+10;
	return hum_val;
}
//数码管复位
void Seg_Reset()
{	u8 i;
	for(i=0;i<8;i++)
	{
		Seg_Buf[i]=10;
		Seg_Point[i]=0;
	}

}
//清空数据
void Clear_Data()
{	u8 i;
	Freq=0;
	temperature=0;
	hum=0;
	aver_temperature=0;
	temp_Max=0;
	max_hum=0;
	aver_hum=0;
	count=0;
	temp_para_Disp=temp_para_Save=0;
	for(i=0;i<3;i++){
		trigger_time[i]=0;
	}
}
void Key_Proc()
{
	if(Key_Slow_Down)return;
	Key_Slow_Down =1;
	//重点记
	Key_Val=Key_Read();
	Key_Down=Key_Val &(Key_Old^Key_Val);
	Key_Up=~Key_Val &(Key_Old^Key_Val);
	Key_Old =Key_Val;
	if(Seg_Disp_Mode==1 )
	{
		if(Key_Down==9)
		{
			long_press_flag=1;
		}
		if(Key_Up==9)
		{	if(time_2000ms>=2000)
				Clear_Data();
			long_press_flag=0;
		}
	}
	switch(Key_Down)
	{
		case 4:
			Seg_Reset();
			if(++Seg_Disp_Mode==3)Seg_Disp_Mode=0;
			Disp_Mode=0;//回显界面为0
			if(Seg_Disp_Mode==0)temp_para_Save=temp_para_Disp;
			if(Seg_Disp_Mode==2)temp_para_Disp=temp_para_Save;
		break;
		case 5:
			if(Seg_Disp_Mode==1){
			Seg_Reset();
			if(++Disp_Mode==3)Disp_Mode=0;}
		break;
		case 8:
			if(Seg_Disp_Mode==2){
				temp_para_Disp=(++temp_para_Disp>99)?99:temp_para_Disp;
			}
		break;
		case 9:
			if(Seg_Disp_Mode==2){
				temp_para_Disp=(--temp_para_Disp<0)?0:temp_para_Disp;
			}
		break;
	}
	
}


void Seg_Proc()
{	//等于0时进入
	u8 i,temp_light;
	if(Seg_Slow_Down) return;
	Seg_Slow_Down =1;
	Read_Rtc(ucRtc);
	temp_light=Ad_Read(0x01);//光敏
	if((old_light_value>50) && (temp_light<50) && (tri_flag==0))
		{
		tri_flag=1;
		if(++count==100)
			count=99;
		}
	else if(tri_flag==1 && time_3000ms>=3000)
		tri_flag=0;
	old_light_value = temp_light;
	if(tri_flag)//触发采集
	{	//特殊温湿度界面
			hum=rd_hum();
			temperature=rd_temperature();
			Seg_Buf[1] = Seg_Buf[2] = 10;
			Seg_Point[6] = 0;
			Seg_Buf[0]=16;//E
			Seg_Buf[3]=(u8)temperature/10;
			Seg_Buf[4]=(u8)temperature%10;
			Seg_Buf[5]=11;
		if(hum==0)
		{	count-=1;
			error_flag=1;
			Seg_Buf[6]=17;//A
			Seg_Buf[7]=17;//A
		}
		else//有效
		{	
			error_flag=0;
			warn_flag=(temperature>temp_para_Save);
			if((count>=2)&&(old_temperature<temperature)&&(old_hum<hum))
				up_flag=1;
			else up_flag=0;
			old_temperature=temperature;
			old_hum=hum;
			Seg_Buf[6]=hum/10;
			Seg_Buf[7]=hum%10;
			temp_Max=(temp_Max>temperature)?temp_Max:temperature;
			max_hum=(max_hum>hum)?max_hum:hum;
			aver_temperature=(aver_temperature*(count-1)+temperature)/(float)count;
			aver_hum=(aver_hum*(count-1)+hum)/(float)count;
			
		}
	}else{
	switch(Seg_Disp_Mode)
	{
		case 0://时间
		Seg_Buf[2]=Seg_Buf[5]=11;
		for(i=0;i<3;i++)
		{
			Seg_Buf[3*i]=ucRtc[i]/16;
			Seg_Buf[3*i+1]=ucRtc[i]%16;
		}

		break;
		case 1://回显界面
			if(Disp_Mode==0)//温度
			{
				Seg_Buf[0]=12;
				if(count !=0){
				Seg_Buf[1]=10;
				Seg_Buf[2]=(u8)temp_Max/10;
				Seg_Buf[3]=(u8)temp_Max%10;
				Seg_Buf[4]=11;
				Seg_Buf[5]=(u8)aver_temperature/10;
				Seg_Buf[6]=(u8)aver_temperature%10;
				Seg_Buf[7]=(u16)(aver_temperature*10)%10;
				Seg_Point[6]=1;
				}
			}
			else if(Disp_Mode==1)//湿度
			{
				Seg_Buf[0]=13;
				if(count !=0){
				Seg_Buf[1]=10;
				Seg_Buf[2]=(u8)max_hum/10;
				Seg_Buf[3]=(u8)max_hum%10;
				Seg_Buf[4]=11;
				Seg_Buf[5]=(u8)aver_hum/10;
				Seg_Buf[6]=(u8)aver_hum%10;
				Seg_Buf[7]=(u16)(aver_hum*10)%10;
				Seg_Point[6]=1;
				}
			}
			else if(Disp_Mode==2)//时间
			{
				Seg_Buf[0]=14;//F
				Seg_Buf[1]=count/10%10;
				Seg_Buf[2]=count%10;
				if(count !=0)
				{
					Seg_Buf[3]=trigger_time[0]/16;
					Seg_Buf[4]=trigger_time[0]%16;
					Seg_Buf[5]=11;
					Seg_Buf[6]=trigger_time[1]/16;
					Seg_Buf[7]=trigger_time[1]%16;
				}
			}
			
		break;
		case 2://参数界面
			Seg_Buf[0]=15;//P
			Seg_Buf[6]=(u8)temp_para_Disp/10;
			Seg_Buf[7]=(u8)temp_para_Disp%10;
		break;
		
	}
	}
}

void Led_Proc()
{	u8 i;
	for(i=0;i<3;i++)
	{
		ucLed[i]=(Seg_Disp_Mode==i);
	}
	ucLed[3]=(warn_flag)?Led_star_flag:0;
	ucLed[4]=error_flag;
	ucLed[5]=up_flag;
}
void Timer0_Init(void)		//计数器@12.000MHz
{
	AUXR &= 0x7F;			//定时器时钟12T模式
	TMOD &= 0xF0;			//设置定时器模式
	TMOD |=0x05;            //设计计数模式
	TL0 = 0;				//设置定时初始值
	TH0 = 0;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
}

void Timer1_Init(void)		//1毫秒@12.000MHz
{
	AUXR &= 0xBF;			//定时器时钟12T模式
	TMOD &= 0x0F;			//设置定时器模式
	TL1 = 0x18;				//设置定时初始值
	TH1 = 0xFC;				//设置定时初始值
	TF1 = 0;				//清除TF1标志
	TR1 = 1;				//定时器1开始计时
	ET1=1;					//中断
	EA=1;
}

/* Timer0 interrupt routine */
void tm1_isr() interrupt 3
{	if(++Key_Slow_Down == 10) Key_Slow_Down = 0;//键盘减速专用
    if(++Seg_Slow_Down==500) Seg_Slow_Down=0;//数码管减速专用
	if(++Seg_Pos==8)Seg_Pos=0;
	Seg_Disp(Seg_Pos,Seg_Buf[Seg_Pos],Seg_Point[Seg_Pos]);
	Led_Disp(Seg_Pos,ucLed[Seg_Pos]);
	if(++Time_1000ms==1000)
	{
		TR0=0;//关闭定时器
		Time_1000ms=0;
		Freq =TH0<<8 |TL0;
		TH0=TL0=0;
		TR0=1;
	}
	if(tri_flag)//一旦触发，就开始计时
	{
	if(++time_3000ms==3000)time_3000ms=3001;
	}
	else
	{
		time_3000ms=0;
	}
	if(long_press_flag==1){
	if(++time_2000ms==2000){
		time_2000ms=0;
	}
	}
	if(warn_flag)
	{
		if(++time_100ms==100){
		time_100ms=0;
		Led_star_flag^=1;
		}
	}
}

void Delay750ms()	//@12.000MHz
{
	unsigned char data i, j, k;


	i = 35;
	j = 51;
	k = 182;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}


void main()
{	
	System_Init();
	Timer0_Init();
	Timer1_Init();
	Set_Rtc(ucRtc);
	rd_temperature();
	Delay750ms();
	
	while(1)
	{
		Seg_Proc();
		Key_Proc();
		Led_Proc();
		
	}
	
}