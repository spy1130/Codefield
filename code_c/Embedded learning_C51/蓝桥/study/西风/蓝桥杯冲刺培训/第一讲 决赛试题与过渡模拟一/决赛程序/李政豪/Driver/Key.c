#include "Key.h"

unsigned char Key_Read()
{
	unsigned char temp = 0;
	P3_0 = 0;P3_1 = 1;P3_2 = 1;P3_3 = 1;
	if(P3_4 == 0) temp = 1;
	if(P3_5 == 0) temp = 2;
	if(P3_6 == 0) temp = 3;
	if(P3_7 == 0) temp = 4;
	P3_0 = 1;P3_1 = 0;P3_2 = 1;P3_3 = 1;
	if(P3_4 == 0) temp = 5;
	if(P3_5 == 0) temp = 6;
	if(P3_6 == 0) temp = 7;
	if(P3_7 == 0) temp = 8;
	P3_0 = 1;P3_1 = 1;P3_2 = 0;P3_3 = 1;
	if(P3_4 == 0) temp = 9;
	if(P3_5 == 0) temp = 10;
	if(P3_6 == 0) temp = 11;
	if(P3_7 == 0) temp = 12;
	P3_0 = 1;P3_1 = 1;P3_2 = 1;P3_3 = 0;
	if(P3_4 == 0) temp = 13;
	if(P3_5 == 0) temp = 14;
	if(P3_6 == 0) temp = 15;
	if(P3_7 == 0) temp = 16;
	return temp;
}