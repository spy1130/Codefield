//BUZZ-0X40;Realy-0x10;Y4-0X80;Y5-0XAO;����|������&
#include <Led.h>

void Led_Disp(unsigned char addr,enable)
{
	static unsigned char temp=0x00;
	static unsigned char temp_old=0xff;//ȫ��
	
	if(enable)
		temp |=0x01<<addr;
	else
		temp &=~(0x01<<addr);
	if(temp !=temp_old)
	{
		P0 =~temp;
		P2=P2&0X1F |0X80;//Y4
		P2&=0X1F;
		temp_old=temp;
	}
	
}
//BUZZ��RealyҪ�ߵ�ƽ���뾭��ULN2003A���ɵ͵�ƽ�����͵�ƽ����
void Beep(unsigned char enable)
{
	static unsigned char temp=0x00;
	static unsigned char temp_old=0xff;//ȫ��
	
	if(enable)
		temp |=0x04;
	else
		temp &=~0x04;
	if(temp !=temp_old)
	{
		P0 =temp;
		P2=P2&0X1F |0Xa0;//Y5
		P2&=0X1F;
		temp_old=temp;
	}
	
}
void Relay(unsigned char enable)
{
	static unsigned char temp=0x00;
	static unsigned char temp_old=0xff;//ȫ��
	
	if(enable)
		temp |=0x10;
	else
		temp &=~0x10;
	if(temp !=temp_old)
	{
		P0 =temp;
		P2=P2&0X1F |0Xa0;//Y5
		P2&=0X1F;
		temp_old=temp;
	}
	
}