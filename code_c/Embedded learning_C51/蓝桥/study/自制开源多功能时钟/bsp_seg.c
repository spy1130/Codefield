# include "bsp_seg.h"



void Seg_Tran(unsigned char*Seg_String,unsigned char*Seg_Buf)
{
	unsigned char i=0;
	unsigned char j=0;
	unsigned char temp;
	for(i=0;i<=7;i++,j++)
	{
		switch(Seg_String[j])
		{
			case '0':  temp = 0xc0;break;
			case '1':  temp = 0xf9;break;
			case '2':  temp = 0xa4;break;
			case '3':  temp = 0xb0;break;
			case '4': temp = 0x99;break;
			case '5': temp = 0x92;break;
			case '6': temp = 0x82;break;
			case '7': temp = 0xf8;break;
			case '8': temp = 0x80;break;
			case '9': temp = 0x90;break;
			
			case '-': temp = 0xbf;break;
			case ' ' :temp = 0xff;break;
			case 'A': temp = 0x88; break;
			case 'B': temp = 0x83; break;
			case 'C': temp = 0xc6; break;
			case 'D': temp = 0xa1; break;
			case 'E': temp = 0x86; break;
			case 'F': temp = 0x8e; break;
			
			case 'H': temp = 0x89; break;
			case 'L': temp = 0xC7; break;
			case 'N': temp = 0xC8; break;
			case 'P': temp = 0x8c; break;
			case 'U': temp = 0xC1; break;
			
			default: temp = 0xff;break;
		}
		
		if(Seg_String[j+1]=='.')
		{
			temp&=0x7f;
			j++;
		}
		
		
		Seg_Buf[i] = temp;
	}
}
void Seg_Disp(unsigned char*Seg_Buf,unsigned char pos)
{
	P0 = 1<<pos;//位码送入
	P2 = P2 & 0x1F | 0xC0; // 0xC0，选通Y6，也就是位码的锁存器，将数据透传过去
	P2 &= 0x1F; //将打开的锁存器关闭，使任何一个锁存器都不打开	
	
	P0 = 0xff;//清空屏幕
	P2 = P2 & 0x1F | 0xE0; // 0xE0，选通Y7，也就是段码的锁存器，将数据透传过去
	P2 &= 0x1F; //将打开的锁存器关闭，使任何一个锁存器都不打开		
	
	P0 = Seg_Buf[pos];//段码送入
	P2 = P2 & 0x1F | 0xE0; // 0xE0，选通Y7，也就是段码的锁存器，将数据透传过去
	P2 &= 0x1F; 
}