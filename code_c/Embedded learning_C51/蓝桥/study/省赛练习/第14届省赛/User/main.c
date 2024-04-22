//��Seg���٣�˳����ʱ��������ET0=1,EA=1;Segɨ�裬led+ɨ�裬Key����+����������������
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
u8 Seg_Disp_Mode;//0-ʱ�䣬1-���ԣ�2-����
u8 Disp_Mode;//0-�¶Ȼ��ԣ�1-ʪ�Ȼ��ԣ�2-ʱ�����
unsigned char ucLed[8]={0,0,0,0,0,0,0,0};

unsigned char Key_Val,Key_Old,Key_Down,Key_Up;
unsigned char Key_Slow_Down;
u16 Time_1000ms;//Ƶ����ʱ����
u16 time_2000ms;//�����ӳٺ���
u16 time_3000ms;//�����ӳٺ���
u8 time_100ms;//��˸�ӳٺ���
u16 Freq;//Ƶ��
u8 ucRtc[3]={0x13,0x03,0x05};
float temperature;//�¶�
u8 hum;//ʪ��
float old_temperature;//��һ���¶�
float aver_temperature;//ƽ���¶�
float temp_Max;//�¶����ֵ��ʾ
float old_hum;//��һ��ʪ��
float max_hum;//���ʪ��
float aver_hum;//ƽ��ʪ��
u8 trigger_time[3];//����ʱ��
u8 count=0;//�ɼ�����
float temp_para_Disp=30;//�¶Ȳ�����ʾ
float temp_para_Save=30;//�¶Ȳ�����ʵ
u8 old_light_value;//��ǿ����
bit tri_flag;//������־λ
bit long_press_flag;//������־
bit warn_flag;//�¶ȳ���������־λ
bit Led_star_flag;//��˸��־λ
bit error_flag;//����Ч���ݱ�־λ
bit up_flag;//��ʪ���Ƿ����߱�־λ
//ʪ�ȴ�����
float rd_hum()
{	
	float hum_val;
	if(Freq<200 ||Freq>2000)
	return 0;
	hum_val=(Freq-200)*2/45+10;
	return hum_val;
}
//����ܸ�λ
void Seg_Reset()
{	u8 i;
	for(i=0;i<8;i++)
	{
		Seg_Buf[i]=10;
		Seg_Point[i]=0;
	}

}
//�������
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
	//�ص��
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
			Disp_Mode=0;//���Խ���Ϊ0
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
{	//����0ʱ����
	u8 i,temp_light;
	if(Seg_Slow_Down) return;
	Seg_Slow_Down =1;
	Read_Rtc(ucRtc);
	temp_light=Ad_Read(0x01);//����
	if((old_light_value>50) && (temp_light<50) && (tri_flag==0))
		{
		tri_flag=1;
		if(++count==100)
			count=99;
		}
	else if(tri_flag==1 && time_3000ms>=3000)
		tri_flag=0;
	old_light_value = temp_light;
	if(tri_flag)//�����ɼ�
	{	//������ʪ�Ƚ���
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
		else//��Ч
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
		case 0://ʱ��
		Seg_Buf[2]=Seg_Buf[5]=11;
		for(i=0;i<3;i++)
		{
			Seg_Buf[3*i]=ucRtc[i]/16;
			Seg_Buf[3*i+1]=ucRtc[i]%16;
		}

		break;
		case 1://���Խ���
			if(Disp_Mode==0)//�¶�
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
			else if(Disp_Mode==1)//ʪ��
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
			else if(Disp_Mode==2)//ʱ��
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
		case 2://��������
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
void Timer0_Init(void)		//������@12.000MHz
{
	AUXR &= 0x7F;			//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;			//���ö�ʱ��ģʽ
	TMOD |=0x05;            //��Ƽ���ģʽ
	TL0 = 0;				//���ö�ʱ��ʼֵ
	TH0 = 0;				//���ö�ʱ��ʼֵ
	TF0 = 0;				//���TF0��־
	TR0 = 1;				//��ʱ��0��ʼ��ʱ
}

void Timer1_Init(void)		//1����@12.000MHz
{
	AUXR &= 0xBF;			//��ʱ��ʱ��12Tģʽ
	TMOD &= 0x0F;			//���ö�ʱ��ģʽ
	TL1 = 0x18;				//���ö�ʱ��ʼֵ
	TH1 = 0xFC;				//���ö�ʱ��ʼֵ
	TF1 = 0;				//���TF1��־
	TR1 = 1;				//��ʱ��1��ʼ��ʱ
	ET1=1;					//�ж�
	EA=1;
}

/* Timer0 interrupt routine */
void tm1_isr() interrupt 3
{	if(++Key_Slow_Down == 10) Key_Slow_Down = 0;//���̼���ר��
    if(++Seg_Slow_Down==500) Seg_Slow_Down=0;//����ܼ���ר��
	if(++Seg_Pos==8)Seg_Pos=0;
	Seg_Disp(Seg_Pos,Seg_Buf[Seg_Pos],Seg_Point[Seg_Pos]);
	Led_Disp(Seg_Pos,ucLed[Seg_Pos]);
	if(++Time_1000ms==1000)
	{
		TR0=0;//�رն�ʱ��
		Time_1000ms=0;
		Freq =TH0<<8 |TL0;
		TH0=TL0=0;
		TR0=1;
	}
	if(tri_flag)//һ���������Ϳ�ʼ��ʱ
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