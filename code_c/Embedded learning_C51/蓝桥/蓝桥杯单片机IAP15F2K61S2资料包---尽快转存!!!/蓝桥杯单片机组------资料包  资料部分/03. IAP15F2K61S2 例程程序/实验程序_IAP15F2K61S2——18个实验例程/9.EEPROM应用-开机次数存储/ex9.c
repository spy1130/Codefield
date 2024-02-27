/*******************************************************************************  
* �ļ����ƣ����������洢ʵ��,���¸�λ�������������ϵ磬�������ʾ+1
* ʵ��Ŀ�ģ�1.����I2C����ͨѶ�����ص�͹���ʱ��
*           2.����51��Ƭ��ģ��I2C����ʱ��ĳ�����Ʒ���
*           3.����EEPROM�洢�������Լ�AT24C02�Ķ�д��������
* ����˵����ʹ�ó���ʱ����Ҫ��Preprocessor Symbols �м���궨�壺EEPROM_AT24C02
* Ӳ��������IAP15F2K61S2@11.0592MHz
* ���ڰ汾��2012-9-4/V1.0a 
*******************************************************************************/

#include "reg52.h"  //����51��Ƭ�����⹦�ܼĴ���
#include "i2c.h"    //I2C���������� 
#include "absacc.h"

sfr AUXR = 0x8E; 
code unsigned char tab[] = { 0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,
                             0xff
                           };
unsigned char dspbuf[8] = {10,10,10,10,10,10,10,10};  //��ʾ������
unsigned char dspcom = 0;

void display(void);

void cls_buzz()
{
	XBYTE[0xA000] = 0;
}
void cls_led()
{
	XBYTE[0x8000] = 0xFF;
}


void delay()		//10ms @11.0592MHz
{
	unsigned char i, j;

	i = 108;
	j = 145;
	do
	{
		while (--j);
	} while (--i);
}

//������
void main(void)
{ 
    unsigned char reset_cnt;  //���������洢 (���洢ֵ255)  
    
	cls_buzz();cls_led();
	
	AUXR |= 0x80;
	TMOD &= 0xF0;
	TL0 = 0xCD;	
	TH0 = 0xD4;	
	TF0 = 0;	
	TR0 = 1;
	ET0 = 1;

// 	write_eeprom(0x00,0x00);  //EEPROM�д洢��������Ҫ���г�ʼ��
 	
    reset_cnt = read_eeprom(0x00);  //��AT24C02��ַ0x00�ж�ȡ����
    reset_cnt++;
	delay();	//��ʱ10ms
    write_eeprom(0x00,reset_cnt);  //��AT24C02��ַ0x00��д������
	delay();

	EA = 1;	//����д����ɺ󣬿����ж�
	
    //������ʾ����
    (reset_cnt >= 100)?(dspbuf[5] = reset_cnt/100):(dspbuf[5]=10);       
    (reset_cnt >= 10)?(dspbuf[6] = reset_cnt%100/10):(dspbuf[6]=10);       
    dspbuf[7] = reset_cnt%10;       
    
    while(1)
    {
        ;
    }
}

//��ʱ���жϷ�����
void isr_timer_0(void)  interrupt 1  //Ĭ���ж����ȼ� 1
{    
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