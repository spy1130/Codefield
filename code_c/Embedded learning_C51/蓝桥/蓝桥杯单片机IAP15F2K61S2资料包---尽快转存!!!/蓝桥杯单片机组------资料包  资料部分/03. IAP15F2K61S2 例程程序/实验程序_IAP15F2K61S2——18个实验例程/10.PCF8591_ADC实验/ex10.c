/*******************************************************************************  
* �ļ����ƣ�ADCת��ʵ��
* ʵ��Ŀ�ģ�1.����I2C����ͨѶ�����ص�͹���ʱ��
*           2.����51��Ƭ��ģ��I2C����ʱ��ĳ�����Ʒ���
*           3.����PCF8591 ADCоƬ�Ĳ�������
* ����˵����1.ʹ�ó���ǰ�� Preprocessor Symbols �����Ӻ궨��ADC_PCF8591
*           2.���ڵ�λ��Rb2���۲��������ʾ���
* Ӳ��������IAP15F2K61S2@11.0592MHz
* ���ڰ汾��2012-9-5/V1.0a 
*******************************************************************************/

#include "reg52.h"  //����51��Ƭ�����⹦�ܼĴ���
#include "i2c.h"    //I2C���������� 
#include "absacc.h"

sfr AUXR = 0x8E; 	//IAP15F2K61S2��Ƭ�����⹦�ܼĴ���

code unsigned char tab[] = { 0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,
                             0xff
                           };
unsigned char dspbuf[8] = {10,10,10,10,10,10,10,10};  //��ʾ������
unsigned char dspcom = 0;
unsigned char intr;
bit adc_flag;

void display(void);

void cls_buzz()
{
	XBYTE[0xA000] = 0;
}
void cls_led()
{
	XBYTE[0x8000] = 0xFF;
}

//������
void main(void)
{ 
    unsigned char adc_value;  //ADCת������ 
	
	cls_buzz();cls_led();
	P1 &= 0x7f;
		
	AUXR |= 0x80;
	TMOD &= 0xF0;
	TL0 = 0xCD;	
	TH0 = 0xD4;	
	TF0 = 0;	
	TR0 = 1;
	ET0 = 1;
	EA = 1;

	init_pcf8591();  //PCF8591��ʼ��      
    
    while(1)
    {
		if(adc_flag)
		{
			adc_flag = 0;  //���ADCɨ���־λ 
			adc_value = adc_pcf8591();
			//������ʾ����
			(adc_value>= 100)?(dspbuf[5] = adc_value/100):(dspbuf[5] = 10);       
			(adc_value>=10)?(dspbuf[6] = adc_value%100/10):(dspbuf[6] = 10);       
			dspbuf[7] = adc_value%10; 
		}
    }
}

//��ʱ���жϷ�����
void isr_timer_0(void)  interrupt 1  //Ĭ���ж����ȼ� 1
{
    if(++intr == 50)
	{
		intr = 0;
		adc_flag = 1;
	} 
    display();
}

//��ʾ����
void display(void)
{   
	XBYTE[0xE000] = 0xff;  //����
	
	XBYTE[0xC000] = (1<<dspcom);
    XBYTE[0xE000] = tab[dspbuf[dspcom]]; //����
   
    if(++dspcom == 8){
        dspcom = 0;
    }   
}