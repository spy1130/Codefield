/*******************************************************************************  
* �ļ����ƣ��������ʵ��
* ʵ��Ŀ�ģ�1.���վ������ɨ��Ļ���ԭ��
*           2.���հ���ɨ�輰���������Ļ���ԭ��
* ����˵����1.ʹ�ó���ǰ��������J2����ΪKBDģʽ
*           2.ת�Ӱ���ʹ��P4.4��P4.2����P3.6 P3.7
* Ӳ��������IAP15F2K61S2@11.0592MHz
* ���ڰ汾��2012-9-4/V1.0a 
*******************************************************************************/

#include "reg52.h"  //����51��Ƭ�����⹦�ܼĴ���
#include "absacc.h"

sfr AUXR = 0x8E;
sfr P4   = 0xC0;	//P4
sbit P42 = P4^2;
sbit P44 = P4^4;
                           //  0    1    2    3    4    5    6    7    8    9   Ϩ��
code unsigned char tab[] = { 0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xFF};
unsigned char dspbuf[8] = {10,10,10,10,10,10,10,10};  //��ʾ������
unsigned char dspcom = 0;

bit key_re;
unsigned char key_press;
unsigned char key_value;

bit key_flag = 0;
unsigned char intr = 0;

unsigned char read_keyboard(void);
void display();

//������
void main(void)
{ 
    unsigned char key_temp = 0xff;
    
	AUXR |= 0x80;	//1Tģʽ��IAP15F2K61S2��Ƭ�����⹦�ܼĴ���
	
	TMOD &= 0xF0;
	TL0 = 0xCD;	
	TH0 = 0xD4;	
	TF0 = 0;	
	TR0 = 1;
	ET0 = 1;
	EA = 1;
    
    while(1)
    {
        if(key_flag)
        {
            key_flag = 0;
			key_temp = read_keyboard();
			if(key_temp != 0xFF){
				dspbuf[7] = key_temp%10;
				dspbuf[6] = key_temp/10;
			}
        } 
        
    }
}

//��ʱ���жϷ�����
void isr_timer_0(void)  interrupt 1  //Ĭ���ж����ȼ� 1
{     
	display();
	if(++intr == 10)  //1msִ��һ��
	{
        intr = 0;
		key_flag = 1;  //10ms����ɨ���־λ��1
    }
}

//��ȡ������̼�ֵ:ת�Ӱ���ʹ��P42��P44����8051����
//˳���е�P36��P37����
unsigned char read_keyboard(void)
{
    static unsigned char col;
    
	P3 = 0xf0; P42 = 1; P44 = 1;

    if((P3 != 0xf0)||(P42 != 0)||(P44 != 0)) //�а�������
        key_press++;
	else
		key_press = 0;  //����
    
    if(key_press == 3)
    {
		key_press = 0;
		key_re = 1;
		
		if(P44 == 0)			col = 1;
		if(P42 == 0)			col = 2;
		if((P3 & 0x20) == 0)	col = 3;
		if((P3 & 0x10) == 0)	col = 4;
        
        P3 = 0x0F; P42 = 0; P44 = 0;

		if((P3&0x01) == 0)	key_value = (col-1);
		if((P3&0x02) == 0)	key_value = (col+3);
		if((P3&0x04) == 0)	key_value = (col+7);
		if((P3&0x08) == 0)	key_value = (col+11);
    }
    
	//�������μ�⵽���������£����Ҹð����Ѿ��ͷ�
	P3 = 0x0f; P42 = 0; P44 = 0;
	
    if(((key_re == 1) && (P3 == 0x0f))&&(P42 == 0)&&(P44 == 0))
    {
        key_re = 0;
        return key_value;
    }
    
    return 0xff;  //�ް������»򱻰��µİ���δ���ͷ� 
}

//��ʾ����
void display(void)
{   
	P0 = 0xff;
	P2 = ((P2&0x1f)|0xE0); 
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