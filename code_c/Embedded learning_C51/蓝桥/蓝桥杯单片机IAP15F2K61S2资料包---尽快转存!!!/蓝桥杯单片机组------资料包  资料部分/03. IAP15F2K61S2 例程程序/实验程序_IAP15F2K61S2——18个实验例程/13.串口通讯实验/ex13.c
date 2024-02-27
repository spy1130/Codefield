/*******************************************************************************  
* �ļ����ƣ�����ͨѶʵ��
* ʵ��Ŀ�ģ�1.����51��Ƭ�����ڹ���ģʽ����ؼĴ������÷���
*           2.�˽�51��Ƭ�������ʵļ��㷽��
* ����˵����1.ͨ��USB����������PC�����ڼ����Ӳ���������в鿴���ں�
*           2.�򿪴��ڵ��Թ����������������ͨѶ�������趨Ϊ2400
* Ӳ��˵����IAP15F2K61S2@11.0592MHz
* ���ڰ汾��2012-9-5/V1.0a 
*******************************************************************************/

#include "reg52.h"  //����51��Ƭ�����⹦�ܼĴ���
#include "intrins.h"

#define BAUD	     2400  //������
#define SYSTEMCLOCK  11059200L  //ϵͳʱ��Ƶ��

sfr AUXR  = 0x8e;               
            
void uart_sendstring(unsigned char *str);
void delay()
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 43;
	j = 6;
	k = 203;
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
    SCON = 0x50;                

    AUXR = 0x40;                //1T
    TMOD = 0x00;                //
    TL1 = (65536 - (SYSTEMCLOCK/4/BAUD));   //
    TH1 = (65536 - (SYSTEMCLOCK/4/BAUD))>>8;
    TR1 = 1;                    
   
    while(1){
		uart_sendstring("hello,world.\r\n");
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