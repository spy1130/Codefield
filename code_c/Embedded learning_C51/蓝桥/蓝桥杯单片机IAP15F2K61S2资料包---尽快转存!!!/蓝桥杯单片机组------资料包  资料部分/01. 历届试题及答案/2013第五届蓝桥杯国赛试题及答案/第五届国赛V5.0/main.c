#include <STC15F2K60S2.h>
#include <delay.h>
#include <iic.h>
#include <onewire.h>
#include <ds1302.h>
#include <stdio.h>

#define uchar unsigned char 
#define uint unsigned int

uchar ori_time,fin_time,jc_time;
uint shi_count,fasong_count;
uchar code tab[13]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xbf,0xff,0xc6};
uchar dsbuff[8]={11,11,11,11,11,11,11,11};
uchar discount;
uchar code ledwdata[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80}; 
uchar yi,er,san,si,wu,liu,qi,ba;
pdata uchar s4=0,s5=0,s6=0,s7=0;
uchar ad_count,ad_flag,shidu,tem,ad_count_2,ad_flag_2;
uchar light_strength,light_state;
uchar shi_xian,wen_xian,ting_xian;
uchar led_status,fasong_flag;
pdata char a[25];
char shuju[6];
uchar n,last_light;
uchar jiechu_flag,num;
uchar i=0;
uchar ADD=0x00,WADD=0x00;
uchar yanzheng=0;

void Timer0Init(void);
void keyscan();
void display();
void wendu_xianshi();
void shizhong_xianshi();
void tingzhi_xianshi();
void led(uchar lednum,uchar demand);
void UartInit(void);
void chuangshu();
void sendstring(uchar *p);
void jilu();
void jisuan();
//void display1(uchar yi,uchar er);
//void display2(uchar san,uchar si);
//void display3(uchar wu,uchar liu);
//void display4(uchar qi,uchar ba);
//void Display(uchar dp);
void L_ED(uchar liang);

void allinit()
{
	P2=0x80;P0=0xff;
	P2=0xa0;P0=0x00;				 	
	P2=0xc0;P0=0xff;
	P2=0xe0;P0=0xff;
}

void main()
{
	allinit();
	Timer0Init();
	UartInit();	
	EA=1;
	ET0=1;
	ES=1;
	dswrite();
	while(1)
	{
		keyscan();
		
		tem=wendu();
		if(ad_flag==1)
		{
			shidu=adread(0x03);
			ad_flag=0;
		}
		if(ad_flag_2==1)
		{
			ad_flag_2=0;
			dsread();	  //时钟显示如果会抖动 则空个100Ms或50ms去读取 则不抖
			last_light=light_strength;
			light_strength=adread(0x01);
			light_strength=adread(0x01);
			jisuan();  
		}

		switch(s5)
		{
			case 0: wen_xian=1;shi_xian=0;ting_xian=0;break;
			case 1:	wen_xian=0;shi_xian=1;ting_xian=0;break;
			case 2:	wen_xian=0;shi_xian=0;ting_xian=1;break;
			case 3: s5=0;break;
		}
		if((wen_xian==1)&&(shi_xian==0)&&(ting_xian==0))
		{  
			wendu_xianshi();
		}
		else if((wen_xian==0)&&(shi_xian==1)&&(ting_xian==0))
		{	
			shizhong_xianshi();		
		}
		else if((wen_xian==0)&&(shi_xian==0)&&(ting_xian==1))
		{
			tingzhi_xianshi();		
		}

		if(s4==0)
		{
			//L_ED(1,1);
			if(jiechu_flag==1)
				L_ED(0xfa);
			else L_ED(0xfe);
			chuangshu();	
		}
		else 
		{
			if(jiechu_flag==1)
				L_ED(0xf9);
			else L_ED(0xfd);
			//led(1,0);
			//led(2,1);
			jilu();
		}
//		display1(yi,er);
//		display2(san,si);
//		display3(wu,liu);
//		display4(qi,ba);
//		Display(8);
	}
}
void jilu()
{
	
	uchar r_tem,r_shidu,r_xiaoshi,r_fenzhong,r_miaozhong,r_jctime;
	if((jiechu_flag==1)&&(num==0))
	{
		num=1;
		iicwrite(ADD,tem);Delay2ms();ADD++;
		iicwrite(ADD,shidu);Delay2ms();ADD++;
		iicwrite(ADD,shijian[2]);Delay2ms();ADD++;
		iicwrite(ADD,shijian[1]);Delay2ms();ADD++;
		iicwrite(ADD,shijian[0]);Delay2ms();ADD++;

	}
	else if((num==1)&&(jiechu_flag==0))
	{
		num=0;
		iicwrite(ADD,jc_time);Delay2ms();ADD++;
		if(ADD==0x1e) ADD=0x00;	//计算得最后的存储位置为0x1d 再加1位0x1e
	}
	//if((shuju[0]=='A')&&(shuju[1]=='A')&&(shuju[2]=='A')&&(shuju[3]=='S')&&(shuju[4]=='S')&&(shuju[5]=='S'))
	if(yanzheng==1)	 //发送一段 然后停下 再次输入再次发送
	{
		shuju[0]='0'; //将接受数组破坏使之不再满足yanzheng=1条件 直到下次发送AAASSS
		yanzheng=0;
		for(WADD=0,i=0;i<5;i++)
		{
			r_tem=iicread(WADD);Delay2ms();WADD++;
			r_shidu=iicread(WADD);Delay2ms();WADD++;
			r_xiaoshi=iicread(WADD);Delay2ms();WADD++;
			r_fenzhong=iicread(WADD);Delay2ms();WADD++;
			r_miaozhong=iicread(WADD);Delay2ms();WADD++;
			r_jctime=iicread(WADD);Delay2ms();WADD++;
			sprintf(a,"{%bd-%bd%%}{%bd-%bd-%bd}{%bd}%c",r_tem,r_shidu,r_xiaoshi,r_fenzhong,r_miaozhong,r_jctime,'\n');
			sendstring(a);	
		}

		
	}			  
}
void chuangshu()
{
	if((shuju[0]=='A')&&(shuju[1]=='A')&&(shuju[2]=='A')&&(shuju[3]=='S')&&(shuju[4]=='S')&&(shuju[5]=='S'))
	{
		if(fasong_flag==1)
		{
			fasong_flag=0;
			sprintf(a,"{%bd-%bd%%}{%bd-%bd-%bd}{%bd}%c",tem,shidu,shijian[2],shijian[1],shijian[0],jiechu_flag,'\n');
			sendstring(a);
		
		}
	}	
}
void sendstring(uchar *p)
{
	while(*p)
	{
		SBUF=*p;
		p++;
		while(!TI){};
		TI=0;
//	   	display1(yi,er);
//		display2(san,si);
//		display3(wu,liu);
//		display4(qi,ba);
	}		

}


void UART() interrupt 4
{
	if(RI)
	{
		shuju[n]=SBUF;
		n++;
		RI=0;
		if(n>=6) n=0;
		if(s4==1)	 //要使先发送一段 停下 再判断再发必须写入这里
		{
			if((shuju[0]=='A')&&(shuju[1]=='A')&&(shuju[2]=='A')&&(shuju[3]=='S')&&(shuju[4]=='S')&&(shuju[5]=='S'))
			 	yanzheng=1;
		}	
	}
	
}
void jisuan()
{
//	uint ori_time,fin_time;
	if((light_strength<6)&&(light_state==0)&&(last_light<6))
	{
		light_state=1;
		//led(3,1);
		jiechu_flag=1;
		ori_time=shijian[0];
	}
	else if((light_strength>=6)&&(light_state==1)&&(last_light>=6))
	{
		light_state=0;
		//led(3,0);
		jiechu_flag=0;
		fin_time=shijian[0];
		if(fin_time>=ori_time)
			jc_time=fin_time-ori_time;	
		else
			jc_time=60+fin_time-ori_time;
	}
}
void tingzhi_xianshi()
{

	dsbuff[0]=11;dsbuff[1]=11;dsbuff[2]=11;dsbuff[3]=10;dsbuff[4]=jc_time/1000;
	dsbuff[5]=jc_time%1000/100;dsbuff[6]=jc_time%100/10;dsbuff[7]=jc_time%10; 
//	yi=11;er=11;san=11;si=10;wu=jc_time/1000;
//	liu=jc_time%1000/100;qi=jc_time%100/10;ba=jc_time%10; 	
//		
}
void shizhong_xianshi()
{
	dsbuff[0]=shijian[2]/10;dsbuff[1]=shijian[2]%10;dsbuff[3]=shijian[1]/10;
	dsbuff[4]=shijian[1]%10;dsbuff[6]=shijian[0]/10;dsbuff[7]=shijian[0]%10;
//	yi=shijian[2]/10;er=shijian[2]%10;si=shijian[1]/10;
//	wu=shijian[1]%10;qi=shijian[0]/10;ba=shijian[0]%10;		
}
void wendu_xianshi()
{
//		if(ad_flag==1)
//		{
//			shidu=adread(0x03);
//			shidu=adread(0x03);
//			ad_flag=0;
//		}
		dsbuff[0]=tem/10;dsbuff[1]=tem%10;dsbuff[2]=12;dsbuff[3]=11;dsbuff[4]=11;
		dsbuff[5]=11;dsbuff[6]=shidu/10;dsbuff[7]=shidu%10;	
//		yi=tem/10;er=tem%10;san=12;si=11;wu=11;
//		liu=11;qi=shidu/10;ba=shidu%10;	
}
void time_0() interrupt 1
{
	static uchar s_liang=0;
	display();
	if(++fasong_count==500)
	{
		fasong_count=0;
		fasong_flag=1;
	}
	if(++ad_count==200)
	{
		ad_count=0;
		ad_flag=1;
	}
	if(++ad_count_2==100)
	{
		ad_count_2=0;
		ad_flag_2=1;
	}
	if(shi_xian==1)
	{
		if(++shi_count==500)
		{
			shi_count=0;
			if(s_liang==0)
			{
				s_liang=1;
				dsbuff[2]=10;
				dsbuff[5]=10;
//				san=10;
//				liu=10;
			}
			else 
			{
				s_liang=0;
				dsbuff[2]=11;
				dsbuff[5]=11;
//				san=11;
//				liu=11;
			}
		}
	}
}
void display()
{
	P2=(P2&0x1f)|0xe0;
	P0=0xff;
	P2=P2&0x1f;

	P2=(P2&0x1f)|0xc0;
	P0=1<<discount;
	P2=P2&0x1f;

	P2=(P2&0x1f)|0xe0;
	P0=tab[dsbuff[discount]];
	P2=P2&0x1f;

	if(++discount == 8)
	{
		discount=0;
	}
}
void keyscan()
{
	if(P33==0)
	{
		Delay5ms();
		if(P33==0)
		{
			if(s4==0) {s4=1;i=0;shuju[0]='S';WADD=0x00;} //每切换一次记得把接收shuju重新赋值一下 好判断下一个AAASSS
			else {s4=0;shuju[0]='S';}
		}
		while(!P33);
	}
		
	else if(P32==0)
	{
		Delay5ms();
		if(P32==0)
		{
			s5++;

		}
		while(!P32);
	}
	else if(P31==0)
	{
		Delay5ms();
		if(P31==0)
		{
			s6=1;
		}
		while(!P31);
	}
	else if(P30==0)
	{
		Delay5ms();
		if(P30==0)
		{
			s7=1;
		}
		while(!P30);
	}
}
//void led(uchar lednum,uchar demand)
//{
//	uchar i;
//	lednum--;
//	i=led_status&(0x01<<lednum);
//	if((i==0)&&(demand==1))
//	{
//		EA=0;
//		P2=P2&0x1f;
//		P2=(P2&0x1f)|0x80;
//		P0=lednum;
//		P2=P2&0x1f;
//		led_status=0x01<<lednum;
//		EA=1;
//	}
//	else if((i>0)&&(demand==0))
//	{
//		EA=0;
//		P2=P2&0x1f;
//		P2=(P2&0x1f)|0x80;
//		P0=0xff;
//		P2=P2&0x1f;
//		led_status=0;
//		EA=1;	
//	}
//}
void L_ED(uchar liang)
{
	P2=P2&0x1f;
	P2=(P2&0x1f)|0x80;
	P0=liang;
	P2=P2&0x1f;	
}

void led(uchar lednum,uchar demand)
{
	uchar i;
	lednum--;
	i=led_status&(0x01<<lednum);
	if((i==0)&&(demand==1))
	{
		EA=0;
		P2=P2&0x1f;
		P2=(P2&0x1f)|0x80;
		P0&=~(0x01<<lednum);
		P2=P2&0x1f;
		led_status=0x01<<lednum;
		EA=1;
	}
	else if((i>0)&&(demand==0))
	{
		EA=0;
		P2=P2&0x1f;
		P2=(P2&0x1f)|0x80;
		P0=0xff;
		P2=P2&0x1f;
		led_status=0;
		EA=1;	
	}
}
//void Timer0Init(void)		//2毫秒@11.0592MHz
//{
//	AUXR |= 0x80;		//定时器时钟1T模式
//	TMOD &= 0xF0;		//设置定时器模式
//	TL0 = 0x9A;		//设置定时初值
//	TH0 = 0xA9;		//设置定时初值
//	TF0 = 0;		//清除TF0标志
//	TR0 = 1;		//定时器0开始计时
//}
void Timer0Init(void)		//2毫秒@12.000MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x40;		//设置定时初值
	TH0 = 0xA2;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
}


//void UartInit(void)		//1200bps@11.0592MHz
//{
//	SCON = 0x50;		//8位数据,可变波特率
//	AUXR |= 0x40;		//定时器1时钟为Fosc,即1T
//	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
//	TMOD &= 0x0F;		//设定定时器1为16位自动重装方式
//	TL1 = 0x00;		//设定定时初值
//	TH1 = 0xF7;		//设定定时初值
//	ET1 = 0;		//禁止定时器1中断
//	TR1 = 1;		//启动定时器1
//}
 void UartInit(void)		//1200bps@12.000MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x40;		//定时器1时钟为Fosc,即1T
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设定定时器1为16位自动重装方式
	TL1 = 0x3C;		//设定定时初值
	TH1 = 0xF6;		//设定定时初值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1
}


//void display1(uchar yi,uchar er)
//{
//		P2=0XC0;//打开位选573   U8
//		P0=0X01;//选择第一个数码管
//		P2=0XFF;//打开段选573   U7
//		P0=tab[yi];
//		Delay1ms();
//		
//		P2=0XC0;//打开位选573   U8
//		P0=0X02;//选择第二个数码管
//		P2=0XFF;//打开段选573   U7
//		P0=tab[er];
//		Delay1ms();
//}	

//void display2(uchar san,uchar si)
//{
//		P2=0XC0;//打开位选573   U8
//		P0=0X04;//选择第三个数码管
//		P2=0XFF;//打开段选573   U7
//		P0=tab[san];
//			Delay1ms();
//		
//		P2=0XC0;//打开位选573   U8
//		P0=0X08;//选择第四个数码管
//		P2=0XFF;//打开段选573   U7
//		P0=tab[si];
//			Delay1ms();
//}

//void display3(uchar wu,uchar liu)
//{
//		P2=0XC0;//打开位选573   U8
//		P0=0X10;//选择第一个数码管
//		P2=0XFF;//打开段选573   U7
//		P0=tab[wu];
//			Delay1ms();
//		
//		P2=0XC0;//打开位选573   U8
//		P0=0X20;//选择第一个数码管
//		P2=0XFF;//打开段选573   U7
//		P0=tab[liu];
//			Delay1ms();
//}

//void display4(uchar qi,uchar ba)
//{
//		P2=0XC0;//打开位选573   U8
//		P0=0X40;//选择第一个数码管
//		P2=0XFF;//打开段选573   U7
//		P0=tab[qi];
//			Delay1ms();
//		
//		P2=0XC0;//打开位选573   U8
//		P0=0X80;//选择第一个数码管
//		P2=0XFF;//打开段选573   U7
//		P0=tab[ba];
//			Delay1ms();

//		P2=0XC0;//打开位选573   U8
//		P0=0X80;//选择第一个数码管
//		P2=0XFF;//打开段选573   U7
//		P0=0xff;
//			Delay1ms();
//}
//void DSLED(uint date,uint wei,uchar dp)
//{
//	P2 &= 0x1f;
//	P2 |= 0xe0; 			 //关闭所有数码管y7c  段选
//	P0 = 0xff;	
//	
//	P2 &= 0x1f;	//将P2口高三位清除以便使用译码器																	  
//	P0 = ledwdata[wei];  //选通第几个数码管显示
//	P2 |= 0xc0;  //位选
//				 
//	if(wei == dp)		//当显示的位数与dp相同时对应的位显示小数点
//	{
//		P2 &= 0x1f;	//将P2口高三位清除以便使用译码器 关闭y4c,y5c,y6c,y7c	
//		P0 = tab[date]&0x7f;   //段码  &0x7f在对应的位显示小数点
//		P2 |= 0xe0;  //段选
//		P2 &= 0x1f;	//关闭y4c,y5c,y6c,y7c
//	}
//	else		  //当显示的位数与dp不同时不显示小数点
//	{
//		P2 &= 0x1f;	//将P2口高三位清除以便使用译码器 关闭y4c,y5c,y6c,y7c	
//		P0 = tab[date];   //段码 
//		P2 |= 0xe0;  //段选
//		P2 &= 0x1f;	//关闭y4c,y5c,y6c,y7c
//	}
//}
//
////dp 取值0~7 小数点位置
//void Display(uchar dp)
//{
//	uchar dspcom = 0;	//显示位置 第几个数码管显示
//
//	for(dspcom=0;dspcom<8;dspcom++)
//	{
// 		DSLED(dsbuff[dspcom],dspcom,dp);   //数码管驱动函数
//		Delay1ms();
//	}
//}