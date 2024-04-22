#include <Led.h>

void Led_Disp(unsigned char addr,enable)
{
	static unsigned char temp = 0x00;
	static unsigned char temp_old = 0xff;
	
	if(enable)
		temp |= 0x01 << addr;
	else
		temp &= ~(0x01 << addr);
	
	if(temp != temp_old)
	{
		P0 = ~temp;//点亮对应Led
		P2 = P2 & 0x1f | 0x80;//选中Led通道
		P2 &= 0x1f;//关闭通道		
		temp_old = temp;//避免出现电流声
	}
}