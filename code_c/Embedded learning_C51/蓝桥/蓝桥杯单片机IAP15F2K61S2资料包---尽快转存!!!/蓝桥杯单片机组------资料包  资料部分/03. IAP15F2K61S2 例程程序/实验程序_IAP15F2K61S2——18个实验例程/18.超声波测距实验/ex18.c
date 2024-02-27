/*******************************************************************************  
* �ļ����ƣ����������ʵ��
* ʵ��Ŀ�ģ�1.���ճ��������ԭ��
*           2.����51��Ƭ����ʱ������ģʽ�����÷���
* ����˵���������ʱ��������Ҫ���ݵ�Ƭ����ʱ��Ƶ�ʽ��е���,����������11.0592MHz
* Ӳ��˵����IAP15F2K61S2@11.0592MHz
* ���ڰ汾��2012-9-6/V1.0a 
*******************************************************************************/

#include "reg52.h"  //����51��Ƭ�����⹦�ܼĴ���
#include "intrins.h"
#include "absacc.h"

#define somenop {_nop_();_nop_();_nop_();_nop_();_nop_();\
                 _nop_();_nop_();_nop_();_nop_(); _nop_();}
sbit TX = P1^0;  //��������
sbit RX = P1^1;  //��������

code unsigned char tab[] = { 0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,\
                             0xff};
unsigned char dspbuf[8] = {10,10,10,10,10,10,10,10};  //��ʾ������
unsigned char dspcom = 0;

unsigned int intr = 0;
bit s_flag;
unsigned int t = 0;

void send_wave(void);
void display(void);

void main(void)
{
    unsigned int distance;

    TMOD |= 0x11;  //���ö�ʱ������ģʽ
    TH0 = (65536-2000)/256;
    TL0 = (65536-2000)%256;  
    TH1 = 0;
    TL1 = 0;  
  
    EA = 1;
    ET0 = 1;  //�򿪶�ʱ��0�ж�
    TR0 = 1;  //������ʱ��   

    while(1)
    {
        /** 200�������һ������ */
		if(s_flag)
        {
            s_flag = 0;
            /** �رն�ʱ��0�ж�:���㳬�������͵����ص�ʱ�� */
// 			ET0 = 0;
            send_wave();  //���ͷ����ź�
            TR1 = 1;  //������ʱ
			while((RX == 1) && (TF1 == 0));  //�ȴ��յ�����
			TR1 = 0;  //�رռ�ʱ

			//�������
			if(TF1 == 1)
			{
				TF1 = 0;
				distance = 9999;  //�޷���
			}
			else
			{
				/**  ����ʱ��  */
				t = TH1;
				t <<= 8;
				t |= TL1;
				distance = (unsigned int)(t*0.017);  //�������				
			}
			TH1 = 0;
			TL1 = 0;
        }
		/** ���ݴ���                      */
                     
		dspbuf[5] = distance/100;
		dspbuf[6] = distance%100/10;
		dspbuf[7] = distance%10;        
    }
}

//��ʱ��0�жϷ�����
void isr_timer_0(void)  interrupt 1  //Ĭ���ж����ȼ� 1
{
    TH0 = (65536-2000)/256;
    TL0 = (65536-2000)%256;  //��ʱ������ 
     
	display();  //2msִ��һ��

	if(++intr == 200)
	{
        s_flag = 1;
        intr = 0;
    }
}

//��ʾ����  
void display(void)
{
 	XBYTE[0xE000] = 0xff;  //ȥ����Ӱ

 	XBYTE[0xC000] = (1<<dspcom);
 	XBYTE[0xE000] = tab[dspbuf[dspcom]]; 
	
	if(++dspcom == 8){
 		dspcom = 0;
	}	
}

//TX���ŷ���40KHz�����ź���������������̽ͷ
//ʹ�������ʱע��RC����Ƶ��
void send_wave(void)
{
	unsigned char i = 8;  //����8������
	
	do
	{
		TX = 1;
		somenop;somenop;somenop;somenop;somenop;somenop;
		somenop;somenop;somenop;somenop;		
		TX = 0;
		somenop;somenop;somenop;somenop;somenop;somenop;
		somenop;somenop;somenop;somenop;		
	}
	while(i--);
}
