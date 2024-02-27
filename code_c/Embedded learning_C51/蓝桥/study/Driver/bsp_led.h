# include "STC15F2K60S2.H"


//函数名：LED显示函数
//入口参数：要显示数值的十六进制数据，例如0XAA=1010 1010，L8~L1的顺序
//返回值：无
//函数功能：按照入口参数顺序将LED点亮，1为亮，0为灭
void Led_Disp(unsigned char ucLed);



