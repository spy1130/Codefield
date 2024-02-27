# include "bsp_key.h"

//矩阵键盘
unsigned char Scan_Key()
{
	unsigned char temp;
	unsigned int KeyNum;
	P44 = 0; P42 = 1; P35 = 1; P34 = 1; 
	KeyNum = (KeyNum<<4)| P3&0xf;
	
	P44 = 1; P42 = 0; P35 = 1; P34 = 1; 
	KeyNum = (KeyNum<<4) | P3&0xf;
	
	P44 = 1; P42 = 1; P35 = 0; P34 = 1; 
	KeyNum = (KeyNum<<4) | P3&0xf;
	
	P44 = 1; P42 = 1; P35 = 1; P34 = 0; 
	KeyNum = (KeyNum<<4) | P3&0xf;
	switch(~KeyNum)
	{
		case 0x8000 : temp = 4;break;
		case 0x4000 : temp = 5;break;
		case 0x2000 : temp = 6;break;
		case 0x1000 : temp = 7;break;
		
		case 0x0800 : temp = 8;break;
		case 0x0400 : temp = 9;break;
		case 0x0200 : temp = 10;break;
		case 0x0100 : temp = 11;break;
		
		case 0x0080 : temp = 12;break;
		case 0x0040 : temp = 13;break;
		case 0x0020 : temp = 14;break;
		case 0x0010 : temp = 15;break;
		
		case 0x0008 : temp = 16;break;
		case 0x0004 : temp = 17;break;
		case 0x0002 : temp = 18;break;
		case 0x0001 : temp = 19;break;
	    default: temp = 0;break;
	}
	return temp;
}

unsigned char Alone_Key()
{
	unsigned char temp;//值不确定
	if(P30 == 0)temp = 1;
	else if(P31 == 0)temp = 2;
	else if(P32 == 0)temp = 3;
	else if(P33 == 0)temp = 4;
	return temp;
}