# include "bsp_init.h"
# include "Int.h"
# include "bsp_led.h"



/*私有变量*/
unsigned char ucLed;//控制灯亮灭的字节变量，0x01=0001 0001 ，L8~L1




void main()
{
	Cls_Peripheral();//关闭外设
	Int_init();//外中断0和外中断1初始化
	while(1);
	
}

//函数名：外中断0的中断服务函数
//入口参数：无
//返回值：无
//函数功能：完成LEDX的翻转
isr_intr_0() interrupt 0
{
	
//传统方法	
//	if((ucLed & 0x01) == 0)
//		ucLed = ucLed | 0x01;//如果最后一位为0，将最后一位置1，其他位不能动。ucLed |= 0x01
//	else 
//		ucLed = ucLed & 0xFE;//如果最后一位为1，将最后一位复0，其他位不能动。
	
	
//现代promax方法
	ucLed ^= 0x01;
	//上述原理解释如下
	//ucLed = ucLed ^ 0x01，相异为1，相同为0，
	//用0作为靶子，和另外一个位异或，会保持原来的数不变
	//用1作为靶子，和另外一个位异或，会让原来的数翻转
	//ucLed = 0101 0101
	//        0000 0001
	//        0101 0100
	//所以说，如果想让某个变量的某一位单独翻转，不影响其他位数据，可以采用异或，靶子数值就是要瞄准的位。
	
	Led_Disp(ucLed);
	
}

//函数名：外中断1的中断服务函数
//入口参数：无
//返回值：无
//函数功能：完成LEDX的翻转
isr_intr_1() interrupt 2
{
	
//传统方法	
//	if((ucLed & 0x02) == 0)
//		ucLed = ucLed | 0x02;//如果最后一位为0，将最后一位置1，其他位不能动。
//	else 
//		ucLed = ucLed & 0xFD;//如果最后一位为1，将最后一位复0，其他位不能动。
	
	
//现代promax方法
	ucLed ^= 0x02;
	
	Led_Disp(ucLed);
	
}