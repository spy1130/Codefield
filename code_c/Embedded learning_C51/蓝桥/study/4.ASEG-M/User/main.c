#include "STC15F2K60S2.H"
#include "init.h"
#include "Delay.h"
#include "SEG.h"
#include "STDIO.H"
#include "STRING.H"
#include "timer.h"

//ȫ�ֱ���
unsigned char seg_string[10];
unsigned char month=0;
unsigned char seg_buf[18];//��ת�������
unsigned char pos=0;
bit tim1_int_sign =0;//tim1�жϱ�ʶλ
unsigned char len;//�ַ����ĳ���
unsigned int one_ms_trigger_count=0;//ÿһ���봥��һ��,0-65535

void main()
{	init_close();
	tim1_init();
	EA=1;//�����ж�
	while(1){
	//��ʱ��1�ص�������ÿ1ms��������һ��������ÿ1s������ʾ����
	if(tim1_int_sign)
	{
		tim1_int_sign = 0;//���ʶ���жϱ�־�仯����λ�жϱ�־��
		one_ms_trigger_count++;	//�������
	}
			
	if((!(one_ms_trigger_count%1000))&&(one_ms_trigger_count>0))//����1s
	{
		one_ms_trigger_count = 0;
		month +=1;
		if(month>12)
		{
			month =1;
		}
		
	}
	len=sprintf(seg_string,"2018--%02u",(unsigned int)month);//ָ��8λ���
	NumTran(seg_string,seg_buf);
	}
		
}

/* Timer1 interrupt routine */
void tm1_isr() interrupt 3
{
    tim1_int_sign = 1;
	NumShow(seg_buf, pos);
	pos++;
	if(pos == 8)
		pos = 0;
	
}