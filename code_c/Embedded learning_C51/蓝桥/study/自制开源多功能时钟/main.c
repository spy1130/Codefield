/*进行分模块实现*/
# include "init.h"
# include "timer.h"
# include "bsp_seg.h"
# include "bsp_key.h"
# include "bsp_led.h"
# include "stdio.h"
# include "ds1302.h"
# include "onewire.h"
//-----------全局变量的声明------------//
/*数码管专用*/
unsigned char Seg_String[9];
unsigned char Seg_Buf[8];
unsigned char pos=0;
/*减速专用*/
unsigned int Seg_Dly;//数码管专用
unsigned char Key_Dly;//按键专用
/*按键专用*/
unsigned char Key_Old;
/*计时专用*/
unsigned int ulms;
unsigned char Sec;
/*led专用*/
unsigned char ucled;
/*ds1302时钟模块*/
unsigned char PrRtc[3]={15,23,45};
unsigned char W_Rtc_ads[3]={0x84,0x82,0x80};//秒,分，时
/*模式切换标志位*/
unsigned char Seg_Mode_Change=0;
/**/
void Led_Proc()
{
	switch(Seg_Mode_Change)
	{
		case 1:ucled = 0x3f;break;
		case 2:ucled = 0xcf;break;
		case 3:ucled = 0xf3;break;
		case 4:ucled = 0xfc;break;
		default :ucled = 0xff;
	}
}
void Key_Proc()//按键处理函数,按键显示
{
	unsigned char Key_Val;
	unsigned char Key_Down;
	if(Key_Dly)return;
	Key_Dly = 1;
//	Key_Val = Scan_Key();//矩阵键盘
	Key_Val = Alone_Key();//独立按键
	Key_Down = Key_Val & (Key_Old ^ Key_Val);
	Key_Old = Key_Val;
	if(Key_Down)
	{
		switch(Key_Old)
		{
			case 1: Seg_Mode_Change=1;break;
			case 2: Seg_Mode_Change=2;break;
			case 3: Seg_Mode_Change=3;break;
			case 4: Seg_Mode_Change=4;break;
		}
	}
}
void Seg_Proc()//显示处理函数
{
	if(Seg_Dly)return;
	Seg_Dly = 1;
	Read_Rtc(PrRtc);
	switch(Seg_Mode_Change)
	{
		case 1:sprintf(Seg_String,"%02d-%02d-%02d",(unsigned int)PrRtc[0],(unsigned int)PrRtc[1],(unsigned int)PrRtc[2]);break;
		
		case 2:sprintf(Seg_String,"-----%03d",(unsigned int)Sec);break;
		
		case 3:sprintf(Seg_String,"%09.4f",Read_Tm()/16.0);break;//改进：如果温度为正前面显示0，为负显示-。
	}
	Seg_Tran(Seg_String,Seg_Buf);
}






void main()
{
	Cls_Peripheral();
	Timer0Init();
	EA=1;
	Set_Rtc(PrRtc);
	while(1)
	{
		Key_Proc();
		Seg_Proc();
		Led_Proc();
	}
}

void tm1_slr()	interrupt 1
{
	if(++ulms == 1000){Sec++;ulms=0;}
	if(++Key_Dly == 10) {Key_Dly = 0;}
	if(++Seg_Dly == 500){Seg_Dly = 0;}
	
	
	Led_Init(ucled);
	Seg_Disp(Seg_Buf,pos);
	if(++pos==8)   pos=0;
}