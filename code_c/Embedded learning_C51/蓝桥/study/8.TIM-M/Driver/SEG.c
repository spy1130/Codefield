#include "SEG.h"
#include "STC15F2K60S2.H"
#include "STRING.h"

//函数名：“字符串”到“数码管段码”的转换函数
//入口参数：seg_string（10个字节）待转换的字符串；将转换完的段码存到seg_buf（8个字节）里
//返回值：无
//函数功能：将seg_string的各个位转换为数码管段码，存储到seg_buf数组中。
void NumTran(unsigned char* seg_string,unsigned char* seg_buf)
{	
	unsigned char i=0;
	unsigned char j=0;
	unsigned char temp;
	for(i=0;i<8;i++,j++)
	{	
		switch(seg_string[j]){
			case '0': temp=0xc0; break;
			case '1': temp=0xf9; break;
			case '2': temp=0xa4; break;
			case '3': temp=0xb0; break;
			case '4': temp=0x99; break;
			case '5': temp=0x92; break;
			case '6': temp=0x82; break;
			case '7': temp=0xf8; break;
			case '8': temp=0x80; break;
			case '9': temp=0x90; break;
			case 'A': temp=0x88; break;
			case 'b': temp=0x83; break;
			case 'C': temp=0xc6; break;
			case 'd': temp=0xa1; break;
			case 'E': temp=0x86; break;
			case 'F': temp=0x8e; break;
			case '-': temp = 0xbf; break;
			case ' ': temp = 0xff; break;
			
			default :temp=0xff; break;
		}
		if(seg_string[j+1]=='.'){//'.'和数字或者字母在同一个地方,所以要看下一个是不是'.'，再在temp里加
			temp &= 0x7f;
			j++;//跳过'.'
		}
		seg_buf[i]=temp;
	}
}

//函数名：将“段码数组”的第N位显示。
//入口参数：seg_buf（8个字节）是转换完的段码; pos待显示的seg_buf第pos位。
//返回值：无
//函数功能：将seg_buf的第pos位显示在数码管的第pos位
void NumShow(unsigned char* seg_buf,unsigned char pos)
{
	P0=0x01<<pos;
	P2 =P2 & 0X1f | 0XC0;//y6
	P2 &=0X1f ;
	
	P0 =0XFF;//clear
	P2 =P2 & 0X1f | 0XE0;//Y7
	P2 &=0X1f ;
	
	P0=seg_buf[pos];//传入
	P2 =P2 & 0X1f | 0XE0;//Y7
	P2 &=0X1f ;
}

