/*******************************************************************************  
* �ļ����ƣ�DS18B20�¶ȴ�����ʵ��
* ʵ��Ŀ�ģ�1.���յ�����ͨѶ�����ص�͹���ʱ��
*           2.����51��Ƭ��ģ�ⵥ����ʱ��ĳ�����Ʒ���
*           3.����DS18B20�¶ȴ������Ĳ�������
* ����˵����1.ͨ���������ʾʵʱ�¶�����
*           2.DS18B20����������DQ�뵥Ƭ��P1.4��������
*           3.��������Ϊ1���϶�
*           4.�����������������IO��ʽ��д��������J13����ΪIOģʽ
* Ӳ��˵����IAP15F2K61S2@11.0592MHz
* ���ڰ汾��2012-9-5/V1.0a 
*******************************************************************************/

#include "reg52.h"  //����51��Ƭ�����⹦�ܼĴ���
#include "absacc.h"
#include "ds18b20.h"

sfr AUXR = 0x8E; 

unsigned char dspbuf[8] = {10,10,10,10,10,10,10,10};//��ʾ������
unsigned char dspcom = 0;
unsigned char intr;
bit temper_flag = 0;//�¶ȶ�ȡ��־
code unsigned char tab[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,
                            0xff};

void display(void);

void cls_buzz()
{
	P2 = ((P2&0x1f)|0xA0); 
	P0 = 0x00;
	P2 &= 0x1f;
}
void cls_led()
{
	P2 = ((P2&0x1f)|0x80); 
	P0 = 0xFF;
	P2 &= 0x1f;
}

//������
void main(void)
{ 
    unsigned char temperature;
	
	cls_buzz();
	cls_led();
    
	AUXR |= 0x80;
	TMOD &= 0xF0;
	TL0 = 0xCD;	
	TH0 = 0xD4;	
	TF0 = 0;	
	TR0 = 1;
	ET0 = 1;
	EA = 1;
	
    while(1)
    {
        if(temper_flag)
		{
			temper_flag = 0;
			temperature = temget();  //���¶�         
        }
		
		//��ʾ���ݸ���    
		(temperature>=10)?(dspbuf[6] = temperature/10):(dspbuf[6]=10);       
		dspbuf[7] = temperature%10; 
    }
}

//��ʱ���жϷ�����
void isr_timer_0(void)  interrupt 1  //Ĭ���ж����ȼ� 1
{
    display();
	if(++intr == 100)  //1msִ��һ��
	{
        intr = 0;
		temper_flag = 1;  //100ms�¶ȶ�ȡ��־λ��1
    }
}

//��ʾ����
void display(void)
{   
	P2 = ((P2&0x1f)|0xE0); 
	P0 = 0xff;
	P2 &= 0x1f;

	P0 = 1<<dspcom;	
	P2 = ((P2&0x1f)|0xC0); 
	P2 &= 0x1f;
	
	P0 = tab[dspbuf[dspcom]];	
    P2 = ((P2&0x1f)|0xE0); 
	P2 &= 0x1f;
	
    if(++dspcom == 8){
        dspcom = 0;
    }    
}