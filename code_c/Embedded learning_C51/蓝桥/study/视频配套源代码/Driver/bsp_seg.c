# include "bsp_seg.h"

//函数名：“字符串”到“数码管段码”的转换函数
//入口参数：seg_string（10个字节）待转换的字符串；将转换完的段码存到seg_buf（8个字节）里
//返回值：无
//函数功能：将seg_string的各个位转换为数码管段码，存储到seg_buf数组中。
void Seg_Tran(unsigned char *seg_string, unsigned char *seg_buf)
{
	unsigned char i=0;//buf[i],i=0~7
	unsigned char j=0;//seg_string[j],j=0~10	
	unsigned char temp;//字符串转换为段码的中间存储变量
	
	for(i=0;i<=7;i++,j++)
	{
		switch(seg_string[j])
		{
			case '0':	temp = 0xc0; break;			
			case '1':	temp = 0xf9; break;			
			case '2':	temp = 0xa4; break;			
			case '3':	temp = 0xb0; break;		
			case '4':	temp = 0x99; break;			
			case '5':	temp = 0x92; break;		
			case '6':	temp = 0x82; break;			
			case '7':	temp = 0xf8; break;		
			case '8':	temp = 0x80; break;			
			case '9':	temp = 0x90; break;		
					
			case 'A': temp = 0x88; break;
			case 'B': temp = 0x83; break;
			case 'C': temp = 0xc6; break;
			case 'D': temp = 0xA1; break;
			case 'E': temp = 0x86; break;
			case 'F': temp = 0x8E; break;
			
			case 'H': temp = 0x89; break;
			case 'L': temp = 0xC7; break;
			case 'N': temp = 0xC8; break;
			case 'P': temp = 0x8c; break;
			case 'U': temp = 0xC1; break;
			
			case '-': temp = 0xbf; break;
			case ' ': temp = 0xff; break;		
			
			default : temp = 0xff; break;//是"."时会选到这个
		
		}
	
		if(seg_string[j+1] == '.')//如果字符串里边出现了‘.’，要把刚刚出炉的temp值改变。
		{
			temp &= 0x7f;//把dp位点亮，不改变之前的temp值，只负责点亮dp位
			j++;//跳过带'.'的位置
		}
		
		seg_buf[i] = temp;//将转换后的段码值传递给Buf存储。

	}
}



//函数名：将“段码数组”的第N位显示。
//入口参数：seg_buf（8个字节）是转换完的段码; pos待显示的seg_buf第pos位。
//返回值：无
//函数功能：将seg_buf的第pos位显示在数码管的第pos位
void Seg_Disp(unsigned char *seg_buf, unsigned char pos)
{
	

	
	P0 = 1<<pos;//位码送入
	P2 = P2 & 0x1F | 0xC0; // 0xC0，选通Y6，也就是位码的锁存器，将数据透传过去
	P2 &= 0x1F; //将打开的锁存器关闭，使任何一个锁存器都不打开	
	
	P0 = 0xff;//清空屏幕
	P2 = P2 & 0x1F | 0xE0; // 0xE0，选通Y7，也就是段码的锁存器，将数据透传过去
	P2 &= 0x1F; //将打开的锁存器关闭，使任何一个锁存器都不打开		
	
	P0 = seg_buf[pos];//段码送入
	P2 = P2 & 0x1F | 0xE0; // 0xE0，选通Y7，也就是段码的锁存器，将数据透传过去
	P2 &= 0x1F; //将打开的锁存器关闭，使任何一个锁存器都不打开
}






