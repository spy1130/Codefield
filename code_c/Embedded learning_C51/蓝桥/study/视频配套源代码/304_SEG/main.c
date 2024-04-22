# include "timer.h"
# include "bsp_seg.h"
# include "bsp_init.h"
# include "STDIO.H"

//全局变量
unsigned char disp_data = 123;//待显示的数据
unsigned char seg_string[10];//给小数点和结束符保留了2个空间的字符串数组
unsigned char seg_buf[8];//段码数组和数码管一一对应
unsigned char pos=0;//数码管要显示的位置



main()
{
	
	Cls_Peripheral();//关闭外设
	Timer1Init();//定时器1被征用到数码管显示1ms一次的控制。
	EA = 1;//打开总中断
	
	sprintf(seg_string, "%8u", (unsigned int)disp_data);//seg_string→_ _ _ _ _ 1 2 3 \0
	Seg_Tran(seg_string, seg_buf);//将字符串内容转换到数组中seg_buf→0xbf 0xbf 0xbf 0xbf 0xbf 0xf9 0xa4 0xb0 
	
	while(1);

}




//-----------------------------------------------
/* Timer1_interrupt routine */
void tm1_isr() interrupt 3
{
	Seg_Disp(seg_buf, pos);
	pos++;
	if(pos == 8)
		pos = 0;
}
