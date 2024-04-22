#include "STC15F2K60S2.H"
#include "init.h"
#include "Delay.h"
#include "SEG.h"
#include "STDIO.H"
#include "STRING.H"
unsigned char seg_string[]="0123456789";//存输入字符
unsigned char seg_buf[18];//存转换后的数
unsigned char pos=0;

void main()
{	unsigned char n,m=0;
	unsigned char str[]="0";
	init_close();
	NumTran(seg_string,seg_buf);//翻译成十六进制
	for(n=0;n<8;n++)
	{	
		for(m= 0; m < 10; m++)
		{	
			str[0]=seg_buf[m];
			NumShow(str,n);//在第n位显示
			Delay(200);
		}
	}
	
//	for(n=0;n<16;n++,pos++)
//	{
//		NumTran(seg_string,seg_buf);
//		NumShow(seg_buf,pos);
//		if(pos==7 && flag == 0)
//		{
//			seg_string[]="9AbCdEF."
//			pos=0;
//			n=0;
//			flag=1;
//		}
//	}
}