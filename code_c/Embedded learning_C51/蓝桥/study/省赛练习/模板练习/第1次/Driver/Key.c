#include <Key.h>

unsigned char Key_Read()
{
	unsigned char temp=0;
	P44 =0;P42=1;P35=1;P34=1;
	if(P30==0) temp=7;//R1
	if(P31==0) temp=6;
	if(P32==0) temp=5;
	if(P33==0) temp=4;
	
	P44 =1;P42=0;P35=1;P34=1;
	if(P30==0) temp=11;//R1
	if(P31==0) temp=10;
	if(P32==0) temp=9;
	if(P33==0) temp=8;
	
	P44 =1;P42=1;P35=0;P34=1;
	if(P30==0) temp=15;//R1
	if(P31==0) temp=14;
	if(P32==0) temp=13;
	if(P33==0) temp=12;
	
	P44 =1;P42=1;P35=1;P34=0;
	if(P30==0) temp=19;//R1
	if(P31==0) temp=18;
	if(P32==0) temp=17;
	if(P33==0) temp=16;
	
	return temp;
}