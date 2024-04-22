#include "init.h"
void init_close()
{	P0 =0XFF;//ledµÍµçÆ½Çý¶¯
	P2 = P2 & 0X1F | 0X80;//LED
	P2 &=0X1F;
	
	P0 =0X00;
	P2 = P2 & 0X1F | 0xA0;
	P2 &=0X1F;
}