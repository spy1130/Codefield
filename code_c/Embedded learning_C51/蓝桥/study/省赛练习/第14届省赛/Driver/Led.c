//BUZZ-0X40;Realy-0x10;Y4-0X80;Y5-0XAO;开是|，关是&
#include <Led.h>

void Led_Disp(unsigned char addr,enable)
{
	static unsigned char temp=0x00;
	static unsigned char temp_old=0xff;//全关
	
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
