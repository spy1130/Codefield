#include <Key.h>

unsigned char Key_Read()
{
	unsigned char temp = 0;//局部变量必须初始化数据
	P37 = 0;P36 = 1;P35 = 1;P34 = 1;//仿真没有P4引脚 需要改动
	if(P33 == 0) temp = 4;
	if(P32 == 0) temp = 5;
	if(P31 == 0) temp = 6;
	if(P30 == 0) temp = 7;
	P37 = 1;P36 = 0;P35 = 1;P34 = 1;//仿真没有P4引脚 需要改动
	if(P33 == 0) temp = 8;
	if(P32 == 0) temp = 9;
	if(P31 == 0) temp = 10;
	if(P30 == 0) temp = 11;
	P37 = 1;P36 = 1;P35 = 0;P34 = 1;//仿真没有P4引脚 需要改动
	if(P33 == 0) temp = 12;
	if(P32 == 0) temp = 13;
	if(P31 == 0) temp = 14;
	if(P30 == 0) temp = 15;
	P37 = 1;P36 = 1;P35 = 1;P34 = 0;//仿真没有P4引脚 需要改动
	if(P33 == 0) temp = 16;
	if(P32 == 0) temp = 17;
	if(P31 == 0) temp = 18;
	if(P30 == 0) temp = 19;
	return temp;
}