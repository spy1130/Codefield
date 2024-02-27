/*******************************************************************************  
* �ļ����ƣ�DS18B20�¶ȴ�����ʵ��-С���㴦��
* ʵ��Ŀ�ģ�1.���ճ��õ��ַ���������
*           2.����DS18B20�¶ȴ������Ĳ�������
* ����˵����1.ͨ���������ʵʱ�¶����ݣ�ʹ�ô��ڵ��Թ��ߵ��Գ���
*           2.����ͨѶ�������趨Ϊ2400
*           3.DS18B20����������DQ�뵥Ƭ��P2.7��������
* Ӳ��������IAP15F2K61S2@11.0592MHz
* ���ڰ汾��2012-9-6/V1.0a 
*******************************************************************************/

#include "reg52.h"  //����51��Ƭ�����⹦�ܼĴ���
#include "onewire.h"  //�����ߺ�����
#include <stdio.h>
#include "intrins.h"
#include "ds18b20.h"

sfr AUXR  = 0x8e;               

#define BAUD	     2400  //������
#define SYSTEMCLOCK  11059200L  //ϵͳʱ��Ƶ��

unsigned char intr;

void uart_sendstring(unsigned char *str);

void delay()		//@11.0592MHz  300ms
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 13;
	j = 156;
	k = 83;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

//������
void main(void)
{ 
    float temperature;
	unsigned char str[40];

    SCON = 0x50;                

    AUXR = 0x40;                //1T
    TMOD = 0x00;                //
    TL1 = (65536 - (SYSTEMCLOCK/4/BAUD));   //
    TH1 = (65536 - (SYSTEMCLOCK/4/BAUD))>>8;
    TR1 = 1;                    
	
    while(1)
    {
		sprintf(str,"%s%6.3f%c%c","temperature:",temperature,'\r','\n');
		
		temperature = temget();  //���¶�         
		uart_sendstring(str);
		delay();
		delay();
    }
}

//ͨ�����ڷ����ַ���
void uart_sendstring(unsigned char *str)
{
    unsigned char *p;
    
    p = str;
    while(*p != '\0')
    {
        SBUF = *p;
		while(TI == 0);  //�ȴ����ͱ�־λ��λ
		TI = 0;
        p++;
    }
}