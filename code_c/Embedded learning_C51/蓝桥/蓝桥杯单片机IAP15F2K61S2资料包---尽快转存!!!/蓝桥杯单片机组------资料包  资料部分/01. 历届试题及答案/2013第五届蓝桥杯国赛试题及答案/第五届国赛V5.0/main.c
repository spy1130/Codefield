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
			dsread();	  //ʱ����ʾ����ᶶ�� ��ո�100Ms��50msȥ��ȡ �򲻶�
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
		if(ADD==0x1e) ADD=0x00;	//��������Ĵ洢λ��Ϊ0x1d �ټ�1λ0x1e
	}
	//if((shuju[0]=='A')&&(shuju[1]=='A')&&(shuju[2]=='A')&&(shuju[3]=='S')&&(shuju[4]=='S')&&(shuju[5]=='S'))
	if(yanzheng==1)	 //����һ�� Ȼ��ͣ�� �ٴ������ٴη���
	{
		shuju[0]='0'; //�����������ƻ�ʹ֮��������yanzheng=1���� ֱ���´η���AAASSS
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
		if(s4==1)	 //Ҫʹ�ȷ���һ�� ͣ�� ���ж��ٷ�����д������
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
			if(s4==0) {s4=1;i=0;shuju[0]='S';WADD=0x00;} //ÿ�л�һ�μǵðѽ���shuju���¸�ֵһ�� ���ж���һ��AAASSS
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
//void Timer0Init(void)		//2����@11.0592MHz
//{
//	AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
//	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
//	TL0 = 0x9A;		//���ö�ʱ��ֵ
//	TH0 = 0xA9;		//���ö�ʱ��ֵ
//	TF0 = 0;		//���TF0��־
//	TR0 = 1;		//��ʱ��0��ʼ��ʱ
//}
void Timer0Init(void)		//2����@12.000MHz
{
	AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0x40;		//���ö�ʱ��ֵ
	TH0 = 0xA2;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
}


//void UartInit(void)		//1200bps@11.0592MHz
//{
//	SCON = 0x50;		//8λ����,�ɱ䲨����
//	AUXR |= 0x40;		//��ʱ��1ʱ��ΪFosc,��1T
//	AUXR &= 0xFE;		//����1ѡ��ʱ��1Ϊ�����ʷ�����
//	TMOD &= 0x0F;		//�趨��ʱ��1Ϊ16λ�Զ���װ��ʽ
//	TL1 = 0x00;		//�趨��ʱ��ֵ
//	TH1 = 0xF7;		//�趨��ʱ��ֵ
//	ET1 = 0;		//��ֹ��ʱ��1�ж�
//	TR1 = 1;		//������ʱ��1
//}
 void UartInit(void)		//1200bps@12.000MHz
{
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x40;		//��ʱ��1ʱ��ΪFosc,��1T
	AUXR &= 0xFE;		//����1ѡ��ʱ��1Ϊ�����ʷ�����
	TMOD &= 0x0F;		//�趨��ʱ��1Ϊ16λ�Զ���װ��ʽ
	TL1 = 0x3C;		//�趨��ʱ��ֵ
	TH1 = 0xF6;		//�趨��ʱ��ֵ
	ET1 = 0;		//��ֹ��ʱ��1�ж�
	TR1 = 1;		//������ʱ��1
}


//void display1(uchar yi,uchar er)
//{
//		P2=0XC0;//��λѡ573   U8
//		P0=0X01;//ѡ���һ�������
//		P2=0XFF;//�򿪶�ѡ573   U7
//		P0=tab[yi];
//		Delay1ms();
//		
//		P2=0XC0;//��λѡ573   U8
//		P0=0X02;//ѡ��ڶ��������
//		P2=0XFF;//�򿪶�ѡ573   U7
//		P0=tab[er];
//		Delay1ms();
//}	

//void display2(uchar san,uchar si)
//{
//		P2=0XC0;//��λѡ573   U8
//		P0=0X04;//ѡ������������
//		P2=0XFF;//�򿪶�ѡ573   U7
//		P0=tab[san];
//			Delay1ms();
//		
//		P2=0XC0;//��λѡ573   U8
//		P0=0X08;//ѡ����ĸ������
//		P2=0XFF;//�򿪶�ѡ573   U7
//		P0=tab[si];
//			Delay1ms();
//}

//void display3(uchar wu,uchar liu)
//{
//		P2=0XC0;//��λѡ573   U8
//		P0=0X10;//ѡ���һ�������
//		P2=0XFF;//�򿪶�ѡ573   U7
//		P0=tab[wu];
//			Delay1ms();
//		
//		P2=0XC0;//��λѡ573   U8
//		P0=0X20;//ѡ���һ�������
//		P2=0XFF;//�򿪶�ѡ573   U7
//		P0=tab[liu];
//			Delay1ms();
//}

//void display4(uchar qi,uchar ba)
//{
//		P2=0XC0;//��λѡ573   U8
//		P0=0X40;//ѡ���һ�������
//		P2=0XFF;//�򿪶�ѡ573   U7
//		P0=tab[qi];
//			Delay1ms();
//		
//		P2=0XC0;//��λѡ573   U8
//		P0=0X80;//ѡ���һ�������
//		P2=0XFF;//�򿪶�ѡ573   U7
//		P0=tab[ba];
//			Delay1ms();

//		P2=0XC0;//��λѡ573   U8
//		P0=0X80;//ѡ���һ�������
//		P2=0XFF;//�򿪶�ѡ573   U7
//		P0=0xff;
//			Delay1ms();
//}
//void DSLED(uint date,uint wei,uchar dp)
//{
//	P2 &= 0x1f;
//	P2 |= 0xe0; 			 //�ر����������y7c  ��ѡ
//	P0 = 0xff;	
//	
//	P2 &= 0x1f;	//��P2�ڸ���λ����Ա�ʹ��������																	  
//	P0 = ledwdata[wei];  //ѡͨ�ڼ����������ʾ
//	P2 |= 0xc0;  //λѡ
//				 
//	if(wei == dp)		//����ʾ��λ����dp��ͬʱ��Ӧ��λ��ʾС����
//	{
//		P2 &= 0x1f;	//��P2�ڸ���λ����Ա�ʹ�������� �ر�y4c,y5c,y6c,y7c	
//		P0 = tab[date]&0x7f;   //����  &0x7f�ڶ�Ӧ��λ��ʾС����
//		P2 |= 0xe0;  //��ѡ
//		P2 &= 0x1f;	//�ر�y4c,y5c,y6c,y7c
//	}
//	else		  //����ʾ��λ����dp��ͬʱ����ʾС����
//	{
//		P2 &= 0x1f;	//��P2�ڸ���λ����Ա�ʹ�������� �ر�y4c,y5c,y6c,y7c	
//		P0 = tab[date];   //���� 
//		P2 |= 0xe0;  //��ѡ
//		P2 &= 0x1f;	//�ر�y4c,y5c,y6c,y7c
//	}
//}
//
////dp ȡֵ0~7 С����λ��
//void Display(uchar dp)
//{
//	uchar dspcom = 0;	//��ʾλ�� �ڼ����������ʾ
//
//	for(dspcom=0;dspcom<8;dspcom++)
//	{
// 		DSLED(dsbuff[dspcom],dspcom,dp);   //�������������
//		Delay1ms();
//	}
//}