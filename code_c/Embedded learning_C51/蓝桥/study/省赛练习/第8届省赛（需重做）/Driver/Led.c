#include <Led.h>


void Led_Disp(unsigned char addr,enable)
{
	static unsigned char temp =0x00;
	static unsigned char temp_old =0xff;
	if(enable)
		temp |=0x01<<addr;//不改变其他值加上
	else
		temp &=~(0x01 <<addr);//不改变其他值关掉灯
	if(temp != temp_old)//判断是否改变
	{
		P0 =temp;
		P2 =P2 &0x1f |0xa0;
		P2 &=0x1f;
		temp_old =temp;
	}
	
}